#include "al2o3_platform/platform.h"
#include "al2o3_memory/memory.h"
#include "al2o3_cadt/bagofvectors.h"
#include "al2o3_cadt/vector.h"
#include "al2o3_cadt/dict.h"
#include "render_meshmod/mesh.h"

#include "al2o3_lz4/lz4.h"
#include "datacontainer.h"

AL2O3_EXTERN_C MeshMod_DataContainer* MeshMod_DataContainerCreate(MeshMod_MeshHandle handle, MeshMod_Type type) {

	MeshMod_DataContainer* dc = (MeshMod_DataContainer*)MEMORY_CALLOC(1, sizeof(MeshMod_DataContainer));
	if (dc == NULL) return NULL;

	dc->bag = CADT_BagOfVectorsCreate();
	if (dc->bag == NULL) {
		MeshMod_DataContainerDestroy(dc);
		return NULL;
	}
	dc->owner = handle;
	dc->containerType = type;
	dc->vectorHashs = CADT_DictU64Create();
	if (dc->vectorHashs == NULL) {
		MeshMod_DataContainerDestroy(dc);
		return NULL;
	}
	dc->userData = CADT_DictU64Create();
	if (dc->userData == NULL) {
		MeshMod_DataContainerDestroy(dc);
		return NULL;
	}

	dc->handleManager = Handle_Manager64Create(sizeof(uint64_t), 1024 * 1024, 1024, true);
	if(dc->handleManager == NULL) {
		MeshMod_DataContainerDestroy(dc);
		return NULL;
	}

	return dc;
}

AL2O3_EXTERN_C void MeshMod_DataContainerDestroy(MeshMod_DataContainer* dc) {
	ASSERT(dc);

	CADT_DictU64Destroy(dc->vectorHashs);
	CADT_DictU64Destroy(dc->userData);
	Handle_Manager64Destroy(dc->handleManager);
	CADT_BagOfVectorsDestroy(dc->bag);
	MEMORY_FREE(dc);
}

AL2O3_EXTERN_C MeshMod_DataContainer* MeshMod_DataContainerClone(MeshMod_DataContainer* src, MeshMod_MeshHandle owner) {
	MeshMod_DataContainer* dst = MeshMod_DataContainerCreate(owner, src->containerType);

	dst->vectorHashs = CADT_DictU64Clone(src->vectorHashs);
	dst->userData = CADT_DictU64Clone(src->userData);
	dst->handleManager = Handle_Manager64Clone(src->handleManager);
	dst->bag = CADT_BagOfVectorsClone(src->bag);

	return dst;
}

AL2O3_EXTERN_C Handle_Handle64 MeshMod_DataContainerAlloc(MeshMod_DataContainer* dc) {
	static Handle_Handle64 const invalid = {0};
	ASSERT(dc);
	if(CADT_BagOfVectorsSize(dc->bag) == 0) {
		return invalid;
	}

	Handle_Handle64 handle = Handle_Manager64Alloc(dc->handleManager);
	ASSERT(Handle_Manager64IsValid(dc->handleManager, handle));

	// TODO theres a subtle bug here... the first handle (index 0) never reuses
	// it physical memory space. Its just one vertex thats wasted memory wise so meh
	if( *((uint64_t*)Handle_Manager64HandleToPtr(dc->handleManager, handle)) == 0) {
		MeshMod_RegistryHandle registry = MeshMod_MeshGetRegistry(dc->owner);
		size_t index = 0;

		for (size_t i = 0; i < CADT_BagOfVectorsSize(dc->bag); ++i) {
			CADT_VectorHandle vh = CADT_BagOfVectorsAt(dc->bag, i);

			MeshMod_Tag tag = CADT_BagOfVectorsGetKey(dc->bag, i);
			CADT_DictU64Replace(dc->vectorHashs, tag, 0);
			CADT_DictU64Replace(dc->userData, tag, 0);

			void const* defaultData = MeshMod_RegistryDefaultData(registry, tag);
			size_t localIndex  = CADT_VectorPushElement(vh, defaultData);
			if(i == 0) {
				index = localIndex;
			} else {
				ASSERT(index == localIndex);
			}
		}

		*((uint64_t*)Handle_Manager64HandleToPtr(dc->handleManager, handle)) = index;
	}


	return handle;
}
AL2O3_EXTERN_C void MeshMod_DataContainerRelease(MeshMod_DataContainer* dc, Handle_Handle64 handle) {
	ASSERT(dc);
	Handle_Manager64Release(dc->handleManager, handle);
}

AL2O3_EXTERN_C void MeshMod_DataContainerTagEnsure(MeshMod_DataContainer* dc, MeshMod_Tag tag) {
	ASSERT(dc);
	if ((uint8_t)(tag >> 56) != dc->containerType) {
		LOGERROR("MeshMod_DataContainerExists was passed invalid tag for this container type");
		return;
	}

	size_t count = 0;
	if(CADT_BagOfVectorsSize(dc->bag) > 0) {
		count = CADT_VectorSize(CADT_BagOfVectorsAt(dc->bag, 0));
	}

	// if the tag vector doesn't exist create it
	MeshMod_RegistryHandle registry = MeshMod_MeshGetRegistry(dc->owner);
	if(!MeshMod_DataContainerTagExists(dc, tag)) {
		size_t const elementSize = MeshMod_RegistryElementSize(registry, tag);
		CADT_VectorHandle data = CADT_BagOfVectorsAdd(dc->bag, tag, elementSize);
		CADT_VectorResizeWithDefault(data, count, MeshMod_RegistryDefaultData(registry, tag));
		CADT_DictU64Add(dc->vectorHashs, tag, 0);
		CADT_DictU64Add(dc->userData, tag, 0);
	}
}

AL2O3_EXTERN_C bool MeshMod_DataContainerTagExists(MeshMod_DataContainer* dc, MeshMod_Tag tag) {
	ASSERT(dc);
	if ((uint8_t)(tag >> 56) != dc->containerType) {
		LOGWARNING("Passed invalid tag for this container type");
		return false;
	}

	return CADT_BagOfVectorsKeyExists(dc->bag, tag);
}

AL2O3_EXTERN_C void MeshMod_DataContainerTagRemove(MeshMod_DataContainer* dc, MeshMod_Tag tag) {
	ASSERT(dc);
	if ((uint8_t)(tag >> 56) != dc->containerType) {
		LOGWARNING("Passed invalid tag for this container type");
		return;
	}

	if(!CADT_BagOfVectorsKeyExists(dc->bag, tag)) {
		return;
	}

	MeshMod_RegistryHandle registry = MeshMod_MeshGetRegistry(dc->owner);

	if(MeshMod_RegistryHasDestroy(registry, tag)) {
		// destroy each element
		MeshMod_RegistryDestroyFunc destroyFunc = MeshMod_RegistryGetCommonFunctionTable(registry, tag)->destroyFunc;
		CADT_VectorHandle vh = CADT_BagOfVectorsLookup(dc->bag, tag);
		for(size_t i = 0;i < CADT_VectorSize(vh);++i) {
			destroyFunc(CADT_VectorAt(vh, i));
		}
	}

	CADT_BagOfVectorsRemove(dc->bag, tag);
	CADT_DictU64Remove(dc->vectorHashs, tag);
	CADT_DictU64Remove(dc->userData, tag);
}


AL2O3_EXTERN_C bool MeshMod_DataContainerIsValid(MeshMod_DataContainer* dc, Handle_Handle64 handle) {
	ASSERT(dc);
	return Handle_Manager64IsValid(dc->handleManager, handle);
}

AL2O3_EXTERN_C void MeshMod_DataContainerReplace(MeshMod_DataContainer* dc, Handle_Handle64 srcHandle, Handle_Handle64 dstHandle) {
	ASSERT(dc);
	ASSERT(MeshMod_DataContainerIsValid(dc, srcHandle));
	ASSERT(MeshMod_DataContainerIsValid(dc, dstHandle));

	uint64_t srcIndex = *((uint64_t*)Handle_Manager64HandleToPtr(dc->handleManager, srcHandle));
	uint64_t dstIndex = *((uint64_t*)Handle_Manager64HandleToPtr(dc->handleManager, dstHandle));
	MeshMod_RegistryHandle registry = MeshMod_MeshGetRegistry(dc->owner);

	for (size_t i = 0; i < CADT_BagOfVectorsSize(dc->bag); ++i) {
		CADT_VectorHandle vh = CADT_BagOfVectorsAt(dc->bag, i);
		MeshMod_Tag tag = CADT_BagOfVectorsGetKey(dc->bag, i);

		if(MeshMod_RegistryHasDestroy(registry, tag)) {
			MeshMod_RegistryDestroyFunc destroyFunc = MeshMod_RegistryGetCommonFunctionTable(registry, tag)->destroyFunc;
			destroyFunc(CADT_VectorAt(vh, dstIndex));
		}

		CADT_VectorReplace(vh, srcIndex, dstIndex);
		CADT_DictU64Replace(dc->vectorHashs, tag, 0);
	}
}
AL2O3_EXTERN_C void MeshMod_DataContainerSwap(MeshMod_DataContainer* dc, Handle_Handle64 handle0, Handle_Handle64 handle1) {
	ASSERT(dc);
	ASSERT(MeshMod_DataContainerIsValid(dc, handle0));
	ASSERT(MeshMod_DataContainerIsValid(dc, handle1));

	uint64_t index0 = *((uint64_t*)Handle_Manager64HandleToPtr(dc->handleManager, handle0));
	uint64_t index1 = *((uint64_t*)Handle_Manager64HandleToPtr(dc->handleManager, handle1));

	for (size_t i = 0; i < CADT_BagOfVectorsSize(dc->bag); ++i) {
		CADT_VectorHandle vh = CADT_BagOfVectorsAt(dc->bag, i);
		MeshMod_Tag tag = CADT_BagOfVectorsGetKey(dc->bag, i);
		CADT_VectorSwap(vh, index0, index1);
		CADT_DictU64Replace(dc->vectorHashs, tag, 0);
	}
}

AL2O3_EXTERN_C void* MeshMod_DataContainerTagHandleToPtr(MeshMod_DataContainer* dc, MeshMod_Tag tag, Handle_Handle64 handle) {
	ASSERT(dc);
	ASSERT(MeshMod_DataContainerIsValid(dc, handle));

	uint64_t index = *((uint64_t*)Handle_Manager64HandleToPtr(dc->handleManager, handle));

	CADT_VectorHandle vh = CADT_BagOfVectorsLookup(dc->bag, tag);
	ASSERT(index < CADT_VectorSize(vh));
	return CADT_VectorAt(vh, index);
}

AL2O3_EXTERN_C CADT_VectorHandle MeshMod_DataContainerTagToPtr(MeshMod_DataContainer* dc, MeshMod_Tag tag) {
	ASSERT(dc);
	return CADT_BagOfVectorsLookup(dc->bag, tag);
}

AL2O3_EXTERN_C void MeshMod_DataContainerTagHandleToDefault(MeshMod_DataContainer* dc, MeshMod_Tag tag, Handle_Handle64 handle) {
	ASSERT(dc);
	MeshMod_RegistryHandle registry = MeshMod_MeshGetRegistry(dc->owner);

	CADT_VectorHandle v = MeshMod_DataContainerTagToPtr(dc, tag);
	uint64_t index0 = *((uint64_t*)Handle_Manager64HandleToPtr(dc->handleManager, handle));

	if(MeshMod_RegistryHasDestroy(registry, tag)) {
		MeshMod_RegistryDestroyFunc destroyFunc = MeshMod_RegistryGetCommonFunctionTable(registry, tag)->destroyFunc;
		destroyFunc(CADT_VectorAt(v, index0));
	}

	memcpy(CADT_VectorAt(v, index0), MeshMod_RegistryDefaultData(registry, tag), MeshMod_RegistryElementSize(registry, tag));
}

AL2O3_EXTERN_C void MeshMod_DataContainerTagSetTransitive(MeshMod_DataContainer* dc, MeshMod_Tag tag, bool transitive) {
	uint64_t userData = CADT_DictU64Get(dc->userData, tag);

	if(transitive) {
		userData |= MeshMod_DataContainerTransitiveFlag;
	} else {
		userData &= MeshMod_DataContainerTransitiveFlag;
	}

	CADT_DictU64Replace(dc->vectorHashs, tag, userData);
}

AL2O3_EXTERN_C void MeshMod_DataContainerMarkChanged(MeshMod_DataContainer* dc) {

	CADT_VectorHandle toDelete = CADT_VectorCreate(sizeof(MeshMod_Tag));
	CADT_VectorReserve(toDelete, CADT_BagOfVectorsSize(dc->bag));

	for (size_t i = 0; i < CADT_BagOfVectorsSize(dc->bag); ++i) {
		MeshMod_Tag tag = CADT_BagOfVectorsGetKey(dc->bag, i);
		CADT_DictU64Replace(dc->vectorHashs, tag, 0);

		uint64_t const flags = CADT_DictU64Get(dc->userData, tag) & MeshMod_DataContainerUserDataMask;
		if (flags & MeshMod_DataContainerTransitiveFlag) {
			CADT_VectorPushElement(toDelete, &tag);
		}
	}

	while(!CADT_VectorIsEmpty(toDelete)) {
		MeshMod_Tag tag;
		CADT_VectorPopElement(toDelete, &tag);
		MeshMod_DataContainerTagRemove(dc, tag);
	}

	CADT_VectorDestroy(toDelete);
}

AL2O3_EXTERN_C uint64_t MeshMod_DataContainerTagGetOrComputeHash(MeshMod_DataContainer* dc, MeshMod_Tag tag) {
	CADT_VectorHandle v = MeshMod_DataContainerTagToPtr(dc, tag);
	if (v == NULL || CADT_VectorSize(v) == 0) {
		return 0;
	}

	uint64_t hash = CADT_DictU64Get(dc->vectorHashs, tag);
	if (hash == 0) {
		for(size_t i = 0; i < CADT_VectorSize(v);++i) {
			Handle_Handle64 handle = Handle_Manager64IndexToHandle(dc->handleManager, i);
			if(Handle_Manager64IsValid(dc->handleManager, handle)) {
				hash = LZ4_XXHash(CADT_VectorAt(v,i), CADT_VectorElementSize(v), hash);
			}
		}
		ASSERT(hash != 0);
		CADT_DictU64Replace(dc->vectorHashs, tag, hash);
	}

	return hash;
}

AL2O3_EXTERN_C uint64_t MeshMod_DataContainerTagGetUserData(MeshMod_DataContainer* dc, MeshMod_Tag tag) {
	return CADT_DictU64Get(dc->userData, tag);
}

AL2O3_EXTERN_C void MeshMod_DataContainerTagSetUserData(MeshMod_DataContainer* dc, MeshMod_Tag tag, uint64_t userData) {

	uint64_t const flags = CADT_DictU64Get(dc->userData, tag) & MeshMod_DataContainerUserDataMask;
	uint64_t const combined = (userData & MeshMod_DataContainerUserDataMask) | flags;

	CADT_DictU64Replace(dc->vectorHashs, tag, combined);
}


AL2O3_EXTERN_C uint64_t MeshMod_DataContainerIndexCount(MeshMod_DataContainer* dc) {
	ASSERT(dc);

	return Thread_AtomicLoad64Relaxed(&dc->handleManager->totalHandlesAllocated);
}

AL2O3_FORCE_INLINE Handle_Handle64 MeshMod_DataContainerIndexToHandle(MeshMod_DataContainer* dc, uint64_t actualIndex) {
	ASSERT(dc);

	return Handle_Manager64IndexToHandle(dc->handleManager, actualIndex);
}
// only for vertex containers
AL2O3_EXTERN_C MeshMod_VertexHandle MeshMod_VertexDataContainerInterpolate1D(MeshMod_VertexDataContainerHandle vdchandle, MeshMod_VertexHandle handle0, MeshMod_VertexHandle handle1, float t) {
	MeshMod_DataContainer* dc = vdchandle.dc;
	MeshMod_RegistryHandle registry = MeshMod_MeshGetRegistry(dc->owner);
	ASSERT(dc->containerType == MeshMod_TypeVertex);

	ASSERT(MeshMod_DataContainerIsValid(dc, handle0.handle));
	ASSERT(MeshMod_DataContainerIsValid(dc, handle1.handle));

	uint64_t i0 = *((uint64_t*)Handle_Manager64HandleToPtr(dc->handleManager, handle0.handle));
	uint64_t i1 = *((uint64_t*)Handle_Manager64HandleToPtr(dc->handleManager, handle1.handle));

	Handle_Handle64 dstHandle = MeshMod_DataContainerAlloc(dc);
	uint64_t dstIndex = *((uint64_t*)Handle_Manager64HandleToPtr(dc->handleManager, dstHandle));

	for (size_t i = 0; i < CADT_BagOfVectorsSize(dc->bag); ++i) {
		CADT_VectorHandle vh = CADT_BagOfVectorsAt(dc->bag, i);
		MeshMod_Tag tag = CADT_BagOfVectorsGetKey(dc->bag, i);
		CADT_DictU64Replace(dc->vectorHashs, tag, 0);

		MeshMod_RegistryVertexFunctionTable* vtable = MeshMod_RegistryFunctionTable(registry, tag, MeshMod_TypeVertex);
		if (vtable->interpolate2DFunc) {
			void* src0 = CADT_VectorAt(vh, i0);
			void* src1 = CADT_VectorAt(vh, i1);
			void* dst = CADT_VectorAt(vh, dstIndex);
			vtable->interpolate1DFunc(src0, src1, dst, t);
		}
	}
	MeshMod_VertexHandle const vertexHandle = { dstHandle };
	return vertexHandle;
}

AL2O3_EXTERN_C MeshMod_VertexHandle MeshMod_VertexDataContainerInterpolate2D(MeshMod_VertexDataContainerHandle vdchandle, MeshMod_VertexHandle handle0, MeshMod_VertexHandle handle1, MeshMod_VertexHandle handle2, float u, float v) {
	MeshMod_DataContainer* dc = vdchandle.dc;
	MeshMod_RegistryHandle registry = MeshMod_MeshGetRegistry(dc->owner);
	ASSERT(dc->containerType == MeshMod_TypeVertex);

	ASSERT(MeshMod_DataContainerIsValid(dc, handle0.handle));
	ASSERT(MeshMod_DataContainerIsValid(dc, handle1.handle));
	ASSERT(MeshMod_DataContainerIsValid(dc, handle2.handle));

	uint64_t i0 = *((uint64_t*)Handle_Manager64HandleToPtr(dc->handleManager, handle0.handle));
	uint64_t i1 = *((uint64_t*)Handle_Manager64HandleToPtr(dc->handleManager, handle1.handle));
	uint64_t i2 = *((uint64_t*)Handle_Manager64HandleToPtr(dc->handleManager, handle2.handle));

	Handle_Handle64 dstHandle = MeshMod_DataContainerAlloc(dc);
	uint64_t dstIndex = *((uint64_t*)Handle_Manager64HandleToPtr(dc->handleManager, dstHandle));

	for (size_t i = 0; i < CADT_BagOfVectorsSize(dc->bag); ++i) {
		CADT_VectorHandle vh = CADT_BagOfVectorsAt(dc->bag, i);
		MeshMod_Tag tag = CADT_BagOfVectorsGetKey(dc->bag, i);
		CADT_DictU64Replace(dc->vectorHashs, tag, 0);

		MeshMod_RegistryVertexFunctionTable* vtable = MeshMod_RegistryFunctionTable(registry, tag, MeshMod_TypeVertex);
		if (vtable->interpolate2DFunc) {
			void* src0 = CADT_VectorAt(vh, i0);
			void* src1 = CADT_VectorAt(vh, i1);
			void* src2 = CADT_VectorAt(vh, i2);
			void* dst = CADT_VectorAt(vh, dstIndex);
			vtable->interpolate2DFunc(src0, src1, src2, dst, u, v);
		}
	}

	MeshMod_VertexHandle const vertexHandle = {dstHandle};
	return vertexHandle;
}

