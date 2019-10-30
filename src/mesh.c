#include "al2o3_platform/platform.h"
#include "al2o3_memory/memory.h"
#include "al2o3_cadt/bagofvectors.h"
#include "al2o3_cadt/dict.h"
#include "al2o3_thread/atomic.h"
#include "render_meshmod/mesh.h"
#include "render_meshmod/registry.h"
#include "datacontainer.h"
#include "mesh.h"


AL2O3_EXTERN_C Handle_Manager32* g_MeshMod_MeshHandleManager;

AL2O3_EXTERN_C MeshMod_MeshHandle MeshMod_MeshCreate(MeshMod_RegistryHandle registry, char const* name) {
	static MeshMod_MeshHandle const invalid = {0};
	ASSERT(name);

	bool ownsRegistry = false;
	if(!MeshMod_RegistryHandleIsValid(registry)) {
		registry = MeshMod_RegistryCreateWithDefaults();
		ownsRegistry = true;
	}

	MeshMod_MeshHandle handle = MeshMod_MeshHandleAlloc();
	if(!MeshMod_MeshHandleIsValid(handle)) {
		MeshMod_MeshDestroy(handle);
		return invalid;
	}

	MeshMod_Mesh* mesh = MeshMod_MeshHandleToPtr(handle);
	mesh->name = MEMORY_MALLOC(strlen(name) + 1);
	if(mesh->name == NULL) {
		MeshMod_MeshDestroy(handle);
		return invalid;
	}

	strcpy((char*)mesh->name, name);
	mesh->registry = registry;
	mesh->ownRegistry = ownsRegistry;
	mesh->vertices.dc = MeshMod_DataContainerCreate(handle, MeshMod_TypeVertex);
	mesh->edges.dc = MeshMod_DataContainerCreate(handle, MeshMod_TypeEdge);
	mesh->polygons.dc = MeshMod_DataContainerCreate(handle, MeshMod_TypePolygon);

	mesh->arbitaryDataKeys = CADT_VectorCreate(sizeof(MeshMod_Tag));
	mesh->arbitaryData = CADT_DictU64Create();
	mesh->arbitaryDataSizes = CADT_DictU64Create();

	return handle;
}

AL2O3_EXTERN_C void MeshMod_MeshDestroy(MeshMod_MeshHandle handle) {
	ASSERT(MeshMod_MeshHandleIsValid(handle));
	MeshMod_Mesh* mesh = MeshMod_MeshHandleToPtr(handle);

	for (size_t i = 0; i < CADT_VectorSize(mesh->arbitaryDataKeys); ++i) {
		MeshMod_Tag tag = *(MeshMod_Tag*)CADT_VectorAt(mesh->arbitaryDataKeys, i);
		MEMORY_FREE((void*)CADT_DictU64Get(mesh->arbitaryData, tag));
	}
	CADT_DictU64Destroy(mesh->arbitaryDataSizes);
	CADT_DictU64Destroy(mesh->arbitaryData);
	CADT_VectorDestroy(mesh->arbitaryDataKeys);

	MeshMod_DataContainerDestroy(mesh->polygons.dc);
	MeshMod_DataContainerDestroy(mesh->edges.dc);
	MeshMod_DataContainerDestroy(mesh->vertices.dc);

	if (mesh->ownRegistry) {
		MeshMod_RegistryDestroy(mesh->registry);
	}

	MEMORY_FREE((void*)mesh->name);
	MeshMod_MeshHandleRelease(handle);
}
AL2O3_EXTERN_C MeshMod_MeshHandle MeshMod_MeshClone(MeshMod_MeshHandle handle) {
	static MeshMod_MeshHandle const invalid = {0};

	ASSERT(MeshMod_MeshHandleIsValid(handle));
	MeshMod_Mesh* src = MeshMod_MeshHandleToPtr(handle);
	MeshMod_MeshHandle newHandle = MeshMod_MeshHandleAlloc();
	MeshMod_Mesh* dst = MeshMod_MeshHandleToPtr(newHandle);

	dst->name = MEMORY_MALLOC(strlen(src->name) + 1);
	if(!dst->name) {
		MeshMod_MeshDestroy(newHandle);
		return invalid;
	}
	strcpy((char*)dst->name, src->name);

	if(dst->ownRegistry) {
		dst->registry = MeshMod_RegistryClone(src->registry);
		dst->ownRegistry = true;
	}

	dst->arbitaryDataKeys = CADT_VectorClone(src->arbitaryDataKeys);
	for (size_t i = 0; i < CADT_VectorSize(src->arbitaryDataKeys); ++i) {
		MeshMod_Tag tag = *(MeshMod_Tag*)CADT_VectorAt(src->arbitaryDataKeys, i);
		CADT_DictU64Handle cloneDict = CADT_DictU64Clone((CADT_DictU64Handle)CADT_DictU64Get(src->arbitaryData, tag));
		*(CADT_DictU64Handle*)CADT_VectorAt(dst->arbitaryDataKeys, i) = cloneDict;
	}
	dst->arbitaryDataSizes = CADT_DictU64Clone(src->arbitaryDataSizes);
	dst->arbitaryData = CADT_DictU64Clone(src->arbitaryData);

	dst->polygons.dc = MeshMod_DataContainerClone(src->polygons.dc, newHandle);
	dst->edges.dc = MeshMod_DataContainerClone(src->edges.dc, newHandle);
	dst->vertices.dc = MeshMod_DataContainerClone(src->vertices.dc, newHandle);

	return newHandle;
}

AL2O3_EXTERN_C MeshMod_RegistryHandle MeshMod_MeshGetRegistry(MeshMod_MeshHandle handle) {
	ASSERT(MeshMod_MeshHandleIsValid(handle));
	MeshMod_Mesh* mesh = MeshMod_MeshHandleToPtr(handle);
	return mesh->registry;
}

AL2O3_EXTERN_C void MeshMod_MeshTopologyHasChanged(MeshMod_MeshHandle mhandle) {
	if(!MeshMod_MeshHandleIsValid(mhandle)) {
		return;
	}
	MeshMod_Mesh* mesh = (MeshMod_Mesh*) MeshMod_MeshHandleToPtr(mhandle);
	MeshMod_DataContainerMarkChanged(mesh->vertices.dc);
	MeshMod_DataContainerMarkChanged(mesh->edges.dc);
	MeshMod_DataContainerMarkChanged(mesh->polygons.dc);
}

// Mesh Data API
AL2O3_EXTERN_C void* MeshMod_MeshAddDataZeroed(MeshMod_MeshHandle handle, MeshMod_Tag tag, size_t dataLen) {
	ASSERT(MeshMod_MeshHandleIsValid(handle));
	MeshMod_Mesh* mesh = MeshMod_MeshHandleToPtr(handle);
	ASSERT(MeshMod_MeshHasData(handle, tag) == false);
	void* outData = MEMORY_CALLOC(1, dataLen);
	CADT_VectorPushElement(mesh->arbitaryDataKeys, &tag);
	CADT_DictU64Add(mesh->arbitaryData, tag, (uint64_t)outData);
	CADT_DictU64Add(mesh->arbitaryDataSizes, tag, dataLen);

	return outData;
}
AL2O3_EXTERN_C void* MeshMod_MeshAddData(MeshMod_MeshHandle handle, MeshMod_Tag tag, void* data, size_t dataLen) {
	void* outData = MeshMod_MeshAddDataZeroed(handle, tag, dataLen);
	memcpy(outData, data, dataLen);
	return outData;
}

AL2O3_EXTERN_C void* MeshMod_MeshGetData(MeshMod_MeshHandle handle, MeshMod_Tag tag) {
	ASSERT(MeshMod_MeshHandleIsValid(handle));
	MeshMod_Mesh* mesh = MeshMod_MeshHandleToPtr(handle);
	return (void*) CADT_DictU64Get(mesh->arbitaryData, tag);
}

AL2O3_EXTERN_C void MeshMod_MeshRemoveData(MeshMod_MeshHandle handle, MeshMod_Tag tag) {
	ASSERT(MeshMod_MeshHandleIsValid(handle));
	MeshMod_Mesh* mesh = MeshMod_MeshHandleToPtr(handle);
	if (MeshMod_MeshHasData(handle, tag) == false) { return; }

	MEMORY_FREE((void*)CADT_DictU64Get(mesh->arbitaryData, tag));
	CADT_DictU64Remove(mesh->arbitaryData, tag);
	CADT_DictU64Remove(mesh->arbitaryDataSizes, tag);
	CADT_VectorSwapRemove(mesh->arbitaryDataKeys, CADT_VectorFind(mesh->arbitaryDataKeys, &tag));
}

AL2O3_EXTERN_C bool MeshMod_MeshHasData(MeshMod_MeshHandle handle, MeshMod_Tag tag) {
	ASSERT(MeshMod_MeshHandleIsValid(handle));
	MeshMod_Mesh* mesh = MeshMod_MeshHandleToPtr(handle);
	return CADT_DictU64KeyExists(mesh->arbitaryData, tag);
}

// Vertex API
AL2O3_EXTERN_C MeshMod_VertexHandle MeshMod_MeshVertexAlloc(MeshMod_MeshHandle handle) {
	MeshMod_VertexHandle vhandle = {0};
	if(!MeshMod_MeshHandleIsValid(handle)) {
		return vhandle;
	}

	MeshMod_Mesh* mesh = (MeshMod_Mesh*) MeshMod_MeshHandleToPtr(handle);
	vhandle.handle = MeshMod_DataContainerAlloc(mesh->vertices.dc);

	return vhandle;
}
AL2O3_EXTERN_C MeshMod_VertexHandle MeshMod_MeshVertexDuplicate(MeshMod_MeshHandle handle, MeshMod_VertexHandle srcHandle) {
	MeshMod_VertexHandle vhandle = {0};
	vhandle = MeshMod_MeshVertexAlloc(handle);
	if(!MeshMod_MeshVertexIsValid(handle, vhandle)) {
		return vhandle;
	}

	MeshMod_MeshVertexReplace(handle, srcHandle, vhandle);
	return vhandle;
}
AL2O3_EXTERN_C void MeshMod_MeshVertexRelease(MeshMod_MeshHandle mhandle, MeshMod_VertexHandle vhandle) {
	if(!MeshMod_MeshHandleIsValid(mhandle)) {
		return;
	}
	MeshMod_Mesh* mesh = (MeshMod_Mesh*) MeshMod_MeshHandleToPtr(mhandle);

	MeshMod_DataContainerRelease(mesh->vertices.dc, vhandle.handle);
}

AL2O3_EXTERN_C bool MeshMod_MeshVertexIsValid(MeshMod_MeshHandle mhandle,  MeshMod_VertexHandle vhandle) {
	if(vhandle.handle.handle == 0) {
		return false;
	}
	if(!MeshMod_MeshHandleIsValid(mhandle)) {
		return false;
	}
	MeshMod_Mesh* mesh = (MeshMod_Mesh*) MeshMod_MeshHandleToPtr(mhandle);
	return MeshMod_DataContainerIsValid(mesh->vertices.dc, vhandle.handle);
}
AL2O3_EXTERN_C void MeshMod_MeshVertexReplace(MeshMod_MeshHandle mhandle,  MeshMod_VertexHandle srcHandle, MeshMod_VertexHandle dstHandle) {
	if(!MeshMod_MeshHandleIsValid(mhandle)) {
		return;
	}
	MeshMod_Mesh* mesh = (MeshMod_Mesh*) MeshMod_MeshHandleToPtr(mhandle);
	MeshMod_DataContainerReplace(mesh->vertices.dc, srcHandle.handle, dstHandle.handle);
}

AL2O3_EXTERN_C void MeshMod_MeshVertexSwap(MeshMod_MeshHandle mhandle,  MeshMod_VertexHandle handle0, MeshMod_VertexHandle handle1) {
	if(!MeshMod_MeshHandleIsValid(mhandle)) {
		return;
	}
	MeshMod_Mesh* mesh = (MeshMod_Mesh*) MeshMod_MeshHandleToPtr(mhandle);
	MeshMod_DataContainerSwap(mesh->vertices.dc, handle0.handle, handle1.handle);
}
AL2O3_EXTERN_C MeshMod_VertexHandle MeshMod_MeshVertexIterate(MeshMod_MeshHandle mhandle, MeshMod_VertexHandle* previous) {
	static MeshMod_VertexHandle const invalid = {0 };

	if(!MeshMod_MeshHandleIsValid(mhandle)) {
		return invalid;
	}

	MeshMod_Mesh* mesh = (MeshMod_Mesh*) MeshMod_MeshHandleToPtr(mhandle);

	if(previous == NULL) {
		// start at the beginning valid handle
		for (uint64_t i = 0u; i < MeshMod_DataContainerIndexCount(mesh->vertices.dc); ++i) {
			Handle_Handle64 handle = MeshMod_DataContainerIndexToHandle(mesh->vertices.dc, i);
			if(MeshMod_DataContainerIsValid(mesh->vertices.dc, handle)) {
				MeshMod_VertexHandle vhandle = { handle };
				return vhandle;
			}
		}
		return invalid;
	}

	for (uint64_t i = (previous->handle.handle & Handle_MaxHandles64)+1 ; i < MeshMod_DataContainerIndexCount(mesh->vertices.dc); ++i) {
		Handle_Handle64 handle = MeshMod_DataContainerIndexToHandle(mesh->vertices.dc, i);
		if(MeshMod_DataContainerIsValid(mesh->vertices.dc, handle)) {
			MeshMod_VertexHandle vhandle = { handle };
			return vhandle;
		}
	}
	return invalid;
}


AL2O3_EXTERN_C void MeshMod_MeshVertexTagEnsure(MeshMod_MeshHandle mhandle, MeshMod_VertexTag tag) {
	if(!MeshMod_MeshHandleIsValid(mhandle)) {
		return;
	}
	MeshMod_Mesh* mesh = (MeshMod_Mesh*) MeshMod_MeshHandleToPtr(mhandle);
	MeshMod_DataContainerTagEnsure(mesh->vertices.dc, tag.tag);
}
AL2O3_EXTERN_C bool MeshMod_MeshVertexTagExists(MeshMod_MeshHandle mhandle, MeshMod_VertexTag tag) {
	if(!MeshMod_MeshHandleIsValid(mhandle)) {
		return false;
	}
	MeshMod_Mesh* mesh = (MeshMod_Mesh*) MeshMod_MeshHandleToPtr(mhandle);
	return MeshMod_DataContainerTagExists(mesh->vertices.dc, tag.tag);
}
AL2O3_EXTERN_C void MeshMod_MeshVertexTagRemove(MeshMod_MeshHandle mhandle, MeshMod_VertexTag tag) {
	if(!MeshMod_MeshHandleIsValid(mhandle)) {
		return;
	}
	MeshMod_Mesh* mesh = (MeshMod_Mesh*) MeshMod_MeshHandleToPtr(mhandle);
	MeshMod_DataContainerTagRemove(mesh->vertices.dc, tag.tag);
}
AL2O3_EXTERN_C uint64_t MeshMod_MeshVertexTagGetOrComputeHash(MeshMod_MeshHandle mhandle, MeshMod_VertexTag tag) {
	if(!MeshMod_MeshHandleIsValid(mhandle)) {
		return 0;
	}
	MeshMod_Mesh* mesh = (MeshMod_Mesh*) MeshMod_MeshHandleToPtr(mhandle);
	return MeshMod_DataContainerTagGetOrComputeHash(mesh->vertices.dc, tag.tag);
}

AL2O3_EXTERN_C void MeshMod_MeshVertexTagSetTransitive(MeshMod_MeshHandle mhandle, MeshMod_VertexTag tag, bool transitive) {
	if(!MeshMod_MeshHandleIsValid(mhandle)) {
		return;
	}
	MeshMod_Mesh* mesh = (MeshMod_Mesh*) MeshMod_MeshHandleToPtr(mhandle);
	MeshMod_DataContainerTagSetTransitive(mesh->vertices.dc, tag.tag, transitive);
}

AL2O3_EXTERN_C void* MeshMod_MeshVertexTagHandleToPtr(MeshMod_MeshHandle mhandle, MeshMod_VertexTag tag, MeshMod_VertexHandle handle) {
	if(!MeshMod_MeshHandleIsValid(mhandle)) {
		return NULL;
	}
	MeshMod_Mesh* mesh = (MeshMod_Mesh*) MeshMod_MeshHandleToPtr(mhandle);
	return MeshMod_DataContainerTagHandleToPtr(mesh->vertices.dc, tag.tag, handle.handle);
}
AL2O3_EXTERN_C void MeshMod_MeshVertexTagHandleToDefault(MeshMod_MeshHandle mhandle, MeshMod_VertexTag tag, MeshMod_VertexHandle handle) {
	if(!MeshMod_MeshHandleIsValid(mhandle)) {
		return;
	}
	MeshMod_Mesh* mesh = (MeshMod_Mesh*) MeshMod_MeshHandleToPtr(mhandle);
	MeshMod_DataContainerTagHandleToDefault(mesh->vertices.dc, tag.tag, handle.handle);
}
AL2O3_EXTERN_C MeshMod_VertexHandle MeshMod_MeshVertexTagIterate(MeshMod_MeshHandle mhandle, MeshMod_VertexTag tag, MeshMod_VertexHandle* previous) {
	MeshMod_Mesh *mesh = MeshMod_MeshHandleToPtr(mhandle);
	MeshMod_VertexHandle handle = MeshMod_MeshVertexIterate(mhandle, previous);

	while (MeshMod_MeshVertexIsValid(mhandle, handle)) {
		if (!MeshMod_RegistryIsDefaultData(mesh->registry, tag.tag, MeshMod_MeshVertexTagHandleToPtr(mhandle, tag, handle))) {
			return handle;
		}
		handle = MeshMod_MeshVertexIterate(mhandle, &handle);
	}
	return handle;
}

AL2O3_EXTERN_C MeshMod_VertexHandle MeshMod_MeshVertexInterpolate1D(MeshMod_MeshHandle mhandle, MeshMod_VertexHandle handle0, MeshMod_VertexHandle handle1, float t) {
	if(!MeshMod_MeshHandleIsValid(mhandle)) {
		static MeshMod_VertexHandle const invalid = {0};
		return invalid;
	}
	MeshMod_Mesh* mesh = (MeshMod_Mesh*) MeshMod_MeshHandleToPtr(mhandle);
	return MeshMod_VertexDataContainerInterpolate1D(mesh->vertices, handle0, handle1, t);
}

AL2O3_EXTERN_C MeshMod_VertexHandle MeshMod_MeshVertexInterpolate2D(MeshMod_MeshHandle mhandle, MeshMod_VertexHandle handle0, MeshMod_VertexHandle handle1, MeshMod_VertexHandle handle2, float u, float v) {
	if(!MeshMod_MeshHandleIsValid(mhandle)) {
		MeshMod_VertexHandle invalid = {0};
		return invalid;
	}
	MeshMod_Mesh* mesh = (MeshMod_Mesh*) MeshMod_MeshHandleToPtr(mhandle);
	return MeshMod_VertexDataContainerInterpolate2D(mesh->vertices, handle0, handle1, handle2, u, v);
}


// Edge API
AL2O3_EXTERN_C MeshMod_EdgeHandle MeshMod_MeshEdgeAlloc(MeshMod_MeshHandle handle) {
	MeshMod_EdgeHandle ehandle = {0};
	if(!MeshMod_MeshHandleIsValid(handle)) {
		return ehandle;
	}

	MeshMod_Mesh* mesh = (MeshMod_Mesh*) MeshMod_MeshHandleToPtr(handle);
	ehandle.handle = MeshMod_DataContainerAlloc(mesh->edges.dc);
	return ehandle;

}
AL2O3_EXTERN_C MeshMod_EdgeHandle MeshMod_MeshEdgeDuplicate(MeshMod_MeshHandle handle, MeshMod_EdgeHandle srcHandle) {
	MeshMod_EdgeHandle ehandle = {0};
	ehandle = MeshMod_MeshEdgeAlloc(handle);
	if(!MeshMod_MeshEdgeIsValid(handle, ehandle)) {
		return ehandle;
	}

	MeshMod_MeshEdgeReplace(handle, srcHandle, ehandle);
	return ehandle;
}
AL2O3_EXTERN_C void MeshMod_MeshEdgeRelease(MeshMod_MeshHandle mhandle, MeshMod_EdgeHandle ehandle) {
	if(!MeshMod_MeshHandleIsValid(mhandle)) {
		return;
	}
	MeshMod_Mesh* mesh = (MeshMod_Mesh*) MeshMod_MeshHandleToPtr(mhandle);
	MeshMod_DataContainerRelease(mesh->edges.dc, ehandle.handle);

}

AL2O3_EXTERN_C bool MeshMod_MeshEdgeIsValid(MeshMod_MeshHandle mhandle,  MeshMod_EdgeHandle ehandle) {
	if(ehandle.handle.handle == 0) {
		return false;
	}
	if(!MeshMod_MeshHandleIsValid(mhandle)) {
		return false;
	}
	MeshMod_Mesh* mesh = (MeshMod_Mesh*) MeshMod_MeshHandleToPtr(mhandle);
	return MeshMod_DataContainerIsValid(mesh->edges.dc, ehandle.handle);
}
AL2O3_EXTERN_C void MeshMod_MeshEdgeReplace(MeshMod_MeshHandle mhandle,  MeshMod_EdgeHandle srcHandle, MeshMod_EdgeHandle dstHandle) {
	if(!MeshMod_MeshHandleIsValid(mhandle)) {
		return;
	}
	MeshMod_Mesh* mesh = (MeshMod_Mesh*) MeshMod_MeshHandleToPtr(mhandle);
	MeshMod_DataContainerReplace(mesh->edges.dc, srcHandle.handle, dstHandle.handle);

}
AL2O3_EXTERN_C void MeshMod_MeshEdgeSwap(MeshMod_MeshHandle mhandle,  MeshMod_EdgeHandle handle0, MeshMod_EdgeHandle handle1) {
	if(!MeshMod_MeshHandleIsValid(mhandle)) {
		return;
	}
	MeshMod_Mesh* mesh = (MeshMod_Mesh*) MeshMod_MeshHandleToPtr(mhandle);
	MeshMod_DataContainerSwap(mesh->edges.dc, handle0.handle, handle1.handle);

}
AL2O3_EXTERN_C MeshMod_EdgeHandle MeshMod_MeshEdgeIterate(MeshMod_MeshHandle mhandle, MeshMod_EdgeHandle* previous) {
	static MeshMod_EdgeHandle const invalid = {0 };

	if(!MeshMod_MeshHandleIsValid(mhandle)) {
		return invalid;
	}

	MeshMod_Mesh* mesh = (MeshMod_Mesh*) MeshMod_MeshHandleToPtr(mhandle);

	if(previous == NULL) {
		// start at the beginning valid handle
		for (uint64_t i = 0u; i < MeshMod_DataContainerIndexCount(mesh->edges.dc); ++i) {
			Handle_Handle64 handle = MeshMod_DataContainerIndexToHandle(mesh->edges.dc, i);
			if(MeshMod_DataContainerIsValid(mesh->edges.dc, handle)) {
				MeshMod_EdgeHandle ehandle = { handle };
				return ehandle;
			}
		}
		return invalid;
	}

	for (uint64_t i = (previous->handle.handle & Handle_MaxHandles64)+1 ; i < MeshMod_DataContainerIndexCount(mesh->edges.dc); ++i) {
		Handle_Handle64 handle = MeshMod_DataContainerIndexToHandle(mesh->edges.dc, i);
		if(MeshMod_DataContainerIsValid(mesh->edges.dc, handle)) {
			MeshMod_EdgeHandle ehandle = { handle };
			return ehandle;
		}
	}
	return invalid;
}

AL2O3_EXTERN_C void MeshMod_MeshEdgeTagEnsure(MeshMod_MeshHandle mhandle, MeshMod_EdgeTag tag) {
	if(!MeshMod_MeshHandleIsValid(mhandle)) {
		return;
	}
	MeshMod_Mesh* mesh = (MeshMod_Mesh*) MeshMod_MeshHandleToPtr(mhandle);
	MeshMod_DataContainerTagEnsure(mesh->edges.dc, tag.tag);

}
AL2O3_EXTERN_C bool MeshMod_MeshEdgeTagExists(MeshMod_MeshHandle mhandle, MeshMod_EdgeTag tag) {
	if(!MeshMod_MeshHandleIsValid(mhandle)) {
		return false;
	}
	MeshMod_Mesh* mesh = (MeshMod_Mesh*) MeshMod_MeshHandleToPtr(mhandle);
	return MeshMod_DataContainerTagExists(mesh->edges.dc, tag.tag);
}
AL2O3_EXTERN_C void MeshMod_MeshEdgeTagRemove(MeshMod_MeshHandle mhandle, MeshMod_EdgeTag tag) {
	if(!MeshMod_MeshHandleIsValid(mhandle)) {
		return;
	}
	MeshMod_Mesh* mesh = (MeshMod_Mesh*) MeshMod_MeshHandleToPtr(mhandle);
	MeshMod_DataContainerTagRemove(mesh->edges.dc, tag.tag);
}
AL2O3_EXTERN_C uint64_t MeshMod_MeshEdgeTagGetOrComputeHash(MeshMod_MeshHandle mhandle, MeshMod_EdgeTag tag) {
	if(!MeshMod_MeshHandleIsValid(mhandle)) {
		return 0;
	}
	MeshMod_Mesh* mesh = (MeshMod_Mesh*) MeshMod_MeshHandleToPtr(mhandle);
	return MeshMod_DataContainerTagGetOrComputeHash(mesh->edges.dc, tag.tag);
}
AL2O3_EXTERN_C void MeshMod_MeshEdgeTagSetTransitive(MeshMod_MeshHandle mhandle, MeshMod_EdgeTag tag, bool transitive) {
	if(!MeshMod_MeshHandleIsValid(mhandle)) {
		return;
	}
	MeshMod_Mesh* mesh = (MeshMod_Mesh*) MeshMod_MeshHandleToPtr(mhandle);
	MeshMod_DataContainerTagSetTransitive(mesh->edges.dc, tag.tag, transitive);
}
AL2O3_EXTERN_C void* MeshMod_MeshEdgeTagHandleToPtr(MeshMod_MeshHandle mhandle, MeshMod_EdgeTag tag, MeshMod_EdgeHandle handle) {
	if(!MeshMod_MeshHandleIsValid(mhandle)) {
		return NULL;
	}
	MeshMod_Mesh* mesh = (MeshMod_Mesh*) MeshMod_MeshHandleToPtr(mhandle);
	return MeshMod_DataContainerTagHandleToPtr(mesh->edges.dc, tag.tag, handle.handle);
}
AL2O3_EXTERN_C void MeshMod_MeshEdgeTagHandleToDefault(MeshMod_MeshHandle mhandle, MeshMod_EdgeTag tag, MeshMod_EdgeHandle handle) {
	if(!MeshMod_MeshHandleIsValid(mhandle)) {
		return;
	}
	MeshMod_Mesh* mesh = (MeshMod_Mesh*) MeshMod_MeshHandleToPtr(mhandle);
	MeshMod_DataContainerTagHandleToDefault(mesh->edges.dc, tag.tag, handle.handle);
}
AL2O3_EXTERN_C MeshMod_EdgeHandle MeshMod_MeshEdgeTagIterate(MeshMod_MeshHandle mhandle, MeshMod_EdgeTag tag, MeshMod_EdgeHandle* previous) {
	MeshMod_Mesh *mesh = MeshMod_MeshHandleToPtr(mhandle);
	MeshMod_EdgeHandle handle = MeshMod_MeshEdgeIterate(mhandle, previous);

	while (MeshMod_MeshEdgeIsValid(mhandle, handle)) {
		if (!MeshMod_RegistryIsDefaultData(mesh->registry, tag.tag, MeshMod_MeshEdgeTagHandleToPtr(mhandle, tag, handle))) {
			return handle;
		}
		handle = MeshMod_MeshEdgeIterate(mhandle, &handle);
	}
	return handle;
}

// Polygon API
AL2O3_EXTERN_C MeshMod_PolygonHandle MeshMod_MeshPolygonAlloc(MeshMod_MeshHandle handle) {
	MeshMod_PolygonHandle phandle = {0};
	if(!MeshMod_MeshHandleIsValid(handle)) {
		return phandle;
	}

	MeshMod_Mesh* mesh = (MeshMod_Mesh*) MeshMod_MeshHandleToPtr(handle);
	phandle.handle = MeshMod_DataContainerAlloc(mesh->polygons.dc);
	return phandle;

}
AL2O3_EXTERN_C MeshMod_PolygonHandle MeshMod_MeshPolygonDuplicate(MeshMod_MeshHandle handle, MeshMod_PolygonHandle srcHandle) {
	MeshMod_PolygonHandle phandle = {0};
	phandle = MeshMod_MeshPolygonAlloc(handle);
	if(!MeshMod_MeshPolygonIsValid(handle, phandle)) {
		return phandle;
	}

	MeshMod_MeshPolygonReplace(handle, srcHandle, phandle);
	return phandle;
}

AL2O3_EXTERN_C void MeshMod_MeshPolygonRelease(MeshMod_MeshHandle mhandle, MeshMod_PolygonHandle phandle) {
	if(!MeshMod_MeshHandleIsValid(mhandle)) {
		return;
	}
	MeshMod_Mesh* mesh = (MeshMod_Mesh*) MeshMod_MeshHandleToPtr(mhandle);
	MeshMod_DataContainerRelease(mesh->polygons.dc, phandle.handle);
}

AL2O3_EXTERN_C bool MeshMod_MeshPolygonIsValid(MeshMod_MeshHandle mhandle,  MeshMod_PolygonHandle phandle) {
	if(phandle.handle.handle == 0) {
		return false;
	}
	if(!MeshMod_MeshHandleIsValid(mhandle)) {
		return false;
	}
	MeshMod_Mesh* mesh = (MeshMod_Mesh*) MeshMod_MeshHandleToPtr(mhandle);
	return MeshMod_DataContainerIsValid(mesh->polygons.dc, phandle.handle);
}

AL2O3_EXTERN_C void MeshMod_MeshPolygonReplace(MeshMod_MeshHandle mhandle,  MeshMod_PolygonHandle srcHandle, MeshMod_PolygonHandle dstHandle) {
	if(!MeshMod_MeshHandleIsValid(mhandle)) {
		return;
	}
	MeshMod_Mesh* mesh = (MeshMod_Mesh*) MeshMod_MeshHandleToPtr(mhandle);
	MeshMod_DataContainerReplace(mesh->polygons.dc, srcHandle.handle, dstHandle.handle);

}
AL2O3_EXTERN_C void MeshMod_MeshPolygonSwap(MeshMod_MeshHandle mhandle,  MeshMod_PolygonHandle handle0, MeshMod_PolygonHandle handle1) {
	if(!MeshMod_MeshHandleIsValid(mhandle)) {
		return;
	}
	MeshMod_Mesh* mesh = (MeshMod_Mesh*) MeshMod_MeshHandleToPtr(mhandle);
	MeshMod_DataContainerSwap(mesh->polygons.dc, handle0.handle, handle1.handle);

}
AL2O3_EXTERN_C MeshMod_PolygonHandle MeshMod_MeshPolygonIterate(MeshMod_MeshHandle mhandle, MeshMod_PolygonHandle* previous) {
	static MeshMod_PolygonHandle const invalid = {0 };

	if(!MeshMod_MeshHandleIsValid(mhandle)) {
		return invalid;
	}

	MeshMod_Mesh* mesh = (MeshMod_Mesh*) MeshMod_MeshHandleToPtr(mhandle);

	if(previous == NULL) {
		// start at the beginning valid handle
		for (uint64_t i = 0u; i < MeshMod_DataContainerIndexCount(mesh->polygons.dc); ++i) {
			Handle_Handle64 handle = MeshMod_DataContainerIndexToHandle(mesh->polygons.dc, i);
			if(MeshMod_DataContainerIsValid(mesh->polygons.dc, handle)) {
				MeshMod_PolygonHandle phandle = { handle };
				return phandle;
			}
		}
		return invalid;
	}

	for (uint64_t i = (previous->handle.handle & Handle_MaxHandles64)+1 ; i < MeshMod_DataContainerIndexCount(mesh->polygons.dc); ++i) {
		Handle_Handle64 handle = MeshMod_DataContainerIndexToHandle(mesh->polygons.dc, i);
		if(MeshMod_DataContainerIsValid(mesh->polygons.dc, handle)) {
			MeshMod_PolygonHandle phandle = { handle };
			return phandle;
		}
	}
	return invalid;
}

AL2O3_EXTERN_C void MeshMod_MeshPolygonTagEnsure(MeshMod_MeshHandle mhandle, MeshMod_PolygonTag tag) {
	if(!MeshMod_MeshHandleIsValid(mhandle)) {
		return;
	}
	MeshMod_Mesh* mesh = (MeshMod_Mesh*) MeshMod_MeshHandleToPtr(mhandle);
	MeshMod_DataContainerTagEnsure(mesh->polygons.dc, tag.tag);
}

AL2O3_EXTERN_C bool MeshMod_MeshPolygonTagExists(MeshMod_MeshHandle mhandle, MeshMod_PolygonTag tag) {
	if(!MeshMod_MeshHandleIsValid(mhandle)) {
		return false;
	}
	MeshMod_Mesh* mesh = (MeshMod_Mesh*) MeshMod_MeshHandleToPtr(mhandle);
	return MeshMod_DataContainerTagExists(mesh->polygons.dc, tag.tag);
}
AL2O3_EXTERN_C void MeshMod_MeshPolygonTagRemove(MeshMod_MeshHandle mhandle, MeshMod_PolygonTag tag) {
	if(!MeshMod_MeshHandleIsValid(mhandle)) {
		return;
	}
	MeshMod_Mesh* mesh = (MeshMod_Mesh*) MeshMod_MeshHandleToPtr(mhandle);
	MeshMod_DataContainerTagRemove(mesh->polygons.dc, tag.tag);
}
AL2O3_EXTERN_C uint64_t MeshMod_MeshPolygonTagGetOrComputeHash(MeshMod_MeshHandle mhandle, MeshMod_PolygonTag tag) {
	if(!MeshMod_MeshHandleIsValid(mhandle)) {
		return 0;
	}
	MeshMod_Mesh* mesh = (MeshMod_Mesh*) MeshMod_MeshHandleToPtr(mhandle);
	return MeshMod_DataContainerTagGetOrComputeHash(mesh->polygons.dc, tag.tag);
}
AL2O3_EXTERN_C void MeshMod_MeshPolygonTagSetTransitive(MeshMod_MeshHandle mhandle, MeshMod_PolygonTag tag, bool transitive) {
	if(!MeshMod_MeshHandleIsValid(mhandle)) {
		return;
	}
	MeshMod_Mesh* mesh = (MeshMod_Mesh*) MeshMod_MeshHandleToPtr(mhandle);
	MeshMod_DataContainerTagSetTransitive(mesh->polygons.dc, tag.tag, transitive);
}

AL2O3_EXTERN_C void* MeshMod_MeshPolygonTagHandleToPtr(MeshMod_MeshHandle mhandle, MeshMod_PolygonTag tag, MeshMod_PolygonHandle handle) {
	if(!MeshMod_MeshHandleIsValid(mhandle)) {
		return NULL;
	}
	MeshMod_Mesh* mesh = (MeshMod_Mesh*) MeshMod_MeshHandleToPtr(mhandle);
	return MeshMod_DataContainerTagHandleToPtr(mesh->polygons.dc, tag.tag, handle.handle);
}
AL2O3_EXTERN_C void MeshMod_MeshPolygonTagHandleToDefault(MeshMod_MeshHandle mhandle, MeshMod_PolygonTag tag, MeshMod_PolygonHandle handle) {
	if(!MeshMod_MeshHandleIsValid(mhandle)) {
		return;
	}
	MeshMod_Mesh* mesh = (MeshMod_Mesh*) MeshMod_MeshHandleToPtr(mhandle);
	MeshMod_DataContainerTagHandleToDefault(mesh->polygons.dc, tag.tag, handle.handle);
}
AL2O3_EXTERN_C MeshMod_PolygonHandle MeshMod_MeshPolygonTagIterate(MeshMod_MeshHandle mhandle, MeshMod_PolygonTag tag, MeshMod_PolygonHandle* previous) {
	MeshMod_Mesh *mesh = MeshMod_MeshHandleToPtr(mhandle);
	MeshMod_PolygonHandle handle = MeshMod_MeshPolygonIterate(mhandle, previous);

	while (MeshMod_MeshPolygonIsValid(mhandle, handle)) {
		if (!MeshMod_RegistryIsDefaultData(mesh->registry, tag.tag, MeshMod_MeshPolygonTagHandleToPtr(mhandle, tag, handle))) {
			return handle;
		}
		handle = MeshMod_MeshPolygonIterate(mhandle, &handle);
	}
	return handle;
}
