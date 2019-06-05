#include "al2o3_platform/platform.h"
#include "al2o3_memory/memory.h"
#include "al2o3_cadt/bagofvectors.h"
#include "gfx_meshmod/datacontainer.h"
#include "gfx_meshmod/mesh.h"

static uint8_t const MeshMod_ContainerType_Vertex = 0;
static uint8_t const MeshMod_ContainerType_Edge = 1;
static uint8_t const MeshMod_ContainerType_Polygon = 2;

typedef struct MeshMod_DataContainer {
	MeshMod_Type containerType;
	CADT_BagOfVectorsHandle bag;
	CADT_VectorHandle validVector; // TODO repalce with packed bit vector
	size_t elementCount;
	struct MeshMod_Mesh* owner;
} MeshMod_DataContainer;

AL2O3_EXTERN_C MeshMod_DataContainerHandle MeshMod_DataContainerCreate(struct MeshMod_Mesh* mesh, MeshMod_Type type) {

	MeshMod_DataContainer* dc = (MeshMod_DataContainer*)MEMORY_MALLOC(sizeof(MeshMod_DataContainer));
	if (dc == NULL) return NULL;

	dc->bag = CADT_BagOfVectorsCreate();
	if (dc->bag == NULL) {
		MEMORY_FREE(dc);
		return NULL;
	}

	dc->owner = mesh;
	dc->containerType = type;
	dc->elementCount = 0;
	dc->validVector = CADT_VectorCreate(sizeof(uint8_t));
	if (dc->bag == NULL) {
		CADT_BagOfVectorsDestroy(dc->bag);
		MEMORY_FREE(dc);
		return NULL;
	}
	return dc;
}

AL2O3_EXTERN_C void MeshMod_DataContainerDestroy(MeshMod_DataContainerHandle handle) {
	ASSERT(handle);

	MeshMod_DataContainer* dc = (MeshMod_DataContainer*)handle;
	CADT_VectorDestroy(dc->validVector);
	CADT_BagOfVectorsDestroy(dc->bag);
}

AL2O3_EXTERN_C MeshMod_DataContainerHandle MeshMod_DataContainerClone(MeshMod_DataContainerHandle handle, struct MeshMod_Mesh* newMesh) {
	ASSERT(handle);

	MeshMod_DataContainer* odc = (MeshMod_DataContainer*)handle;
	MeshMod_DataContainer* ndc = (MeshMod_DataContainer*)MEMORY_MALLOC(sizeof(MeshMod_DataContainer));
	if (ndc == NULL) return NULL;

	ndc->bag = CADT_BagOfVectorsClone(odc->bag);
	ndc->owner = newMesh;
	ndc->containerType = odc->containerType;
	ndc->elementCount = odc->elementCount;
	ndc->validVector = CADT_VectorClone(odc->validVector);
	return ndc;

}

AL2O3_EXTERN_C CADT_VectorHandle MeshMod_DataContainerAdd(MeshMod_DataContainerHandle handle, MeshMod_Tag tag) {
	ASSERT(handle);
	MeshMod_DataContainer* dc = (MeshMod_DataContainer*)handle;
	if ((uint8_t)(tag >> 56) != dc->containerType) {
		return NULL;
	}

	MeshMod_RegistryHandle registry = MeshMod_MeshGetRegistry(dc->owner);
	ASSERT(MeshMod_RegistryExists(registry, tag));
	size_t const elementSize = MeshMod_RegistryElementSize(registry, tag);
	return CADT_BagOfVectorsAdd(dc->bag, tag, elementSize);
}
AL2O3_EXTERN_C bool MeshMod_DataContainerExists(MeshMod_DataContainerHandle handle, MeshMod_Tag tag) {
	ASSERT(handle);
	MeshMod_DataContainer* dc = (MeshMod_DataContainer*)handle;
	if ((uint8_t)(tag >> 56) != dc->containerType) {
		return false;
	}

	return CADT_BagOfVectorsKeyExists(dc->bag, tag);
}

AL2O3_EXTERN_C CADT_VectorHandle MeshMod_DataContainerVectorLookupOrAdd(MeshMod_DataContainerHandle handle, MeshMod_Tag tag) {
	if (MeshMod_DataContainerExists(handle, tag)) {
		return MeshMod_DataContainerVectorLookup(handle, tag);
	}
	else {
		return MeshMod_DataContainerAdd(handle, tag);
	}
}

AL2O3_EXTERN_C size_t MeshMod_DataContainerSize(MeshMod_DataContainerHandle handle) {
	ASSERT(handle);
	MeshMod_DataContainer* dc = (MeshMod_DataContainer*)handle;
	return dc->elementCount;
}

AL2O3_EXTERN_C void MeshMod_DataContainerResize(MeshMod_DataContainerHandle handle, size_t size) {
	ASSERT(handle);
	MeshMod_DataContainer* dc = (MeshMod_DataContainer*)handle;
	MeshMod_RegistryHandle registry = MeshMod_MeshGetRegistry(dc->owner);

	for (size_t i = 0; i < CADT_BagOfVectorsSize(dc->bag); ++i) {
		CADT_VectorHandle vh = CADT_BagOfVectorsAt(dc->bag, i);
		MeshMod_Tag tag = CADT_BagOfVectorsGetKey(dc->bag, i);
		void const* defaultData = MeshMod_RegistryDefaultData(registry, tag);
		CADT_VectorResizeWithDefault(vh, size, defaultData);
	}
	uint8_t def = 1;
	CADT_VectorResizeWithDefault(dc->validVector, size, &def);
	dc->elementCount = size;
}

AL2O3_EXTERN_C CADT_VectorHandle MeshMod_DataContainerVectorLookup(MeshMod_DataContainerHandle handle, MeshMod_Tag tag) {
	ASSERT(handle);
	MeshMod_DataContainer* dc = (MeshMod_DataContainer*)handle;
	return CADT_BagOfVectorsLookup(dc->bag, tag);
}

AL2O3_EXTERN_C bool MeshMod_DataContainerIsValid(MeshMod_DataContainerHandle handle, size_t index) {
	ASSERT(handle);
	MeshMod_DataContainer* dc = (MeshMod_DataContainer*)handle;
	ASSERT(index < dc->elementCount);
	return *((uint8_t*)CADT_VectorAt(dc->validVector, index)) > 0;
}

AL2O3_EXTERN_C void MeshMod_DataContainerMarkInvalid(MeshMod_DataContainerHandle handle, size_t index) {
	ASSERT(handle);
	MeshMod_DataContainer* dc = (MeshMod_DataContainer*)handle;
	ASSERT(index < dc->elementCount);
	*((uint8_t*)CADT_VectorAt(dc->validVector, index)) = 0;
}

AL2O3_EXTERN_C void MeshMod_DataContainerReplace(MeshMod_DataContainerHandle handle, size_t srcIndex, size_t dstIndex) {
	MeshMod_DataContainer* dc = (MeshMod_DataContainer*)handle;
	for (size_t i = 0; i < CADT_BagOfVectorsSize(dc->bag); ++i) {
		CADT_VectorHandle vh = CADT_BagOfVectorsAt(dc->bag, i);
		CADT_VectorReplace(vh, srcIndex, dstIndex);
	}
	CADT_VectorReplace(dc->validVector, srcIndex, dstIndex);
}

AL2O3_EXTERN_C void MeshMod_DataContainerSwap(MeshMod_DataContainerHandle handle, size_t index0, size_t index1) {
	MeshMod_DataContainer* dc = (MeshMod_DataContainer*)handle;
	for (size_t i = 0; i < CADT_BagOfVectorsSize(dc->bag); ++i) {
		CADT_VectorHandle vh = CADT_BagOfVectorsAt(dc->bag, i);
		CADT_VectorSwap(vh, index0, index1);
	}
	CADT_VectorSwap(dc->validVector, index0, index1);
}

AL2O3_EXTERN_C void MeshMod_DataContainerRemove(MeshMod_DataContainerHandle handle, size_t index) {
	MeshMod_DataContainer* dc = (MeshMod_DataContainer*)handle;
	for (size_t i = 0; i < CADT_BagOfVectorsSize(dc->bag); ++i) {
		CADT_VectorHandle vh = CADT_BagOfVectorsAt(dc->bag, i);
		CADT_VectorRemove(vh, index);
	}
	CADT_VectorRemove(dc->validVector, index);
}
AL2O3_EXTERN_C void MeshMod_DataContainerSwapRemove(MeshMod_DataContainerHandle handle, size_t index) {
	MeshMod_DataContainer* dc = (MeshMod_DataContainer*)handle;
	for (size_t i = 0; i < CADT_BagOfVectorsSize(dc->bag); ++i) {
		CADT_VectorHandle vh = CADT_BagOfVectorsAt(dc->bag, i);
		CADT_VectorSwapRemove(vh, index);
	}
	CADT_VectorSwapRemove(dc->validVector, index);
}

AL2O3_EXTERN_C CADT_VectorHandle MeshMod_DataContainerGetValidRemappingTable(MeshMod_DataContainerHandle handle) {
	ASSERT(handle);
	MeshMod_DataContainer* dc = (MeshMod_DataContainer*)handle;
	CADT_VectorHandle remapper = CADT_VectorCreate(sizeof(size_t));
	CADT_VectorResize(remapper, MeshMod_DataContainerSize(dc));
	size_t newIndex = 0;
	for (size_t i = 0; i < MeshMod_DataContainerSize(dc); ++i) {
		bool valid = MeshMod_DataContainerIsValid(dc, i);
		*((size_t*)CADT_VectorAt(remapper, i)) = valid ? newIndex++ : MeshMod_InvalidIndex;
	}
	return remapper;
}

AL2O3_EXTERN_C void MeshMod_DataContainerCompact(MeshMod_DataContainerHandle handle) {
	ASSERT(handle);
	MeshMod_DataContainer* dc = (MeshMod_DataContainer*)handle;
	CADT_VectorHandle remapper = MeshMod_DataContainerGetValidRemappingTable(handle);
	size_t newCount = MeshMod_DataContainerRemap(handle, remapper);
	MeshMod_DataContainerResize(handle, newCount);
	CADT_VectorDestroy(remapper);

}

AL2O3_EXTERN_C size_t MeshMod_DataContainerRemap(MeshMod_DataContainerHandle handle, CADT_VectorHandle remapper) {
	ASSERT(handle);
	MeshMod_DataContainer* dc = (MeshMod_DataContainer*)handle;
	ASSERT(dc->elementCount == CADT_VectorSize(remapper));


	for (size_t i = 0; i < CADT_VectorSize(remapper); ++i) {
		size_t newIndex = *((size_t*)CADT_VectorAt(remapper, i));
		ASSERT(newIndex <= i);
		if (newIndex != MeshMod_InvalidIndex) {
			MeshMod_DataContainerReplace(handle, i, newIndex);
		}
	}

	// valid array
	size_t newCount = 0;
	for (size_t i = 0; i < CADT_VectorSize(remapper); ++i) {
		newCount++;
		size_t newIndex = *((size_t*)CADT_VectorAt(remapper, i));
		ASSERT(newIndex <= i);
		if (newIndex != MeshMod_InvalidIndex) {
			CADT_VectorSwap(dc->validVector, i, newIndex);
		}
	}
	return newCount;
}

// only for vertex containers
AL2O3_EXTERN_C MeshMod_VertexIndex MeshMod_DataContainerVertexInterpolate1D(MeshMod_DataContainerHandle handle, MeshMod_VertexIndex i0, MeshMod_VertexIndex i1, float t) {
	MeshMod_DataContainer* dc = (MeshMod_DataContainer*)handle;
	MeshMod_RegistryHandle registry = MeshMod_MeshGetRegistry(dc->owner);
	ASSERT(dc->containerType == MeshMod_TypeVertex);

	MeshMod_DataContainerResize(dc, dc->elementCount + 1);

	for (size_t i = 0; i < CADT_BagOfVectorsSize(dc->bag); ++i) {
		CADT_VectorHandle vh = CADT_BagOfVectorsAt(dc->bag, i);
		MeshMod_Tag tag = CADT_BagOfVectorsGetKey(dc->bag, i);

		MeshMod_RegistryVertexFunctionTable* vtable = MeshMod_RegistryFunctionTable(registry, tag, MeshMod_TypeVertex);
		if (vtable->interpolate2DFunc) {
			void* src0 = CADT_VectorAt(vh, i0);
			void* src1 = CADT_VectorAt(vh, i1);
			void* dst = CADT_VectorAt(vh, dc->elementCount - 1);
			vtable->interpolate1DFunc(src0, src1, dst, t);
		}
	}
	return dc->elementCount - 1;
}

AL2O3_EXTERN_C MeshMod_VertexIndex MeshMod_DataContainerVertexInterpolate2D(MeshMod_DataContainerHandle handle, MeshMod_VertexIndex i0, MeshMod_VertexIndex i1, MeshMod_VertexIndex i2, float u, float v) {
	MeshMod_DataContainer* dc = (MeshMod_DataContainer*)handle;
	MeshMod_RegistryHandle registry = MeshMod_MeshGetRegistry(dc->owner);
	ASSERT(dc->containerType == MeshMod_TypeVertex);

	MeshMod_DataContainerResize(dc, dc->elementCount + 1);

	for (size_t i = 0; i < CADT_BagOfVectorsSize(dc->bag); ++i) {
		CADT_VectorHandle vh = CADT_BagOfVectorsAt(dc->bag, i);
		MeshMod_Tag tag = CADT_BagOfVectorsGetKey(dc->bag, i);

		MeshMod_RegistryVertexFunctionTable* vtable = MeshMod_RegistryFunctionTable(registry, tag, MeshMod_TypeVertex);
		if (vtable->interpolate2DFunc) {
			void* src0 = CADT_VectorAt(vh, i0);
			void* src1 = CADT_VectorAt(vh, i1);
			void* src2 = CADT_VectorAt(vh, i2);
			void* dst = CADT_VectorAt(vh, dc->elementCount - 1);
			vtable->interpolate2DFunc(src0, src1, src2, dst, u, v);
		}
	}
	return dc->elementCount - 1;

}

AL2O3_EXTERN_C void MeshMod_DataContainerChanged(MeshMod_DataContainerHandle handle) {
	MeshMod_DataContainer* dc = (MeshMod_DataContainer*)handle;
	MeshMod_RegistryHandle registry = MeshMod_MeshGetRegistry(dc->owner);

#define TAG_REMOVE_SIZE 128
	MeshMod_Tag toDelete[TAG_REMOVE_SIZE];
	size_t toDeleteHead = 0;

	for (size_t i = 0; i < CADT_BagOfVectorsSize(dc->bag); ++i) {
		MeshMod_Tag tag = CADT_BagOfVectorsGetKey(dc->bag, i);
		MeshMod_RegistryCommonFunctionTable* vtable = MeshMod_RegistryGetCommonFunctionTable(registry, tag);
		if (vtable->isTransitoryFunc && vtable->isTransitoryFunc()) {
			toDelete[toDeleteHead++] = tag;
			ASSERT(toDeleteHead != TAG_REMOVE_SIZE)
			if (toDeleteHead == TAG_REMOVE_SIZE) {
				toDeleteHead = TAG_REMOVE_SIZE - 1;
			}
		}
	}
#undef TAG_REMOVE_SIZE

	for (size_t i = 0; i < toDeleteHead; ++i) {
		CADT_BagOfVectorsRemove(dc->bag, toDelete[i]);
	}
}
