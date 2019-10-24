#pragma once

#include "datacontainer.h"

typedef struct MeshMod_Mesh {
	MeshMod_RegistryHandle registry;
	bool ownRegistry;
	char const * name;

	MeshMod_VertexDataContainerHandle vertices;
	MeshMod_EdgeDataContainerHandle edges;
	MeshMod_PolygonDataContainerHandle polygons;

	CADT_VectorHandle arbitaryDataKeys;
	CADT_DictU64Handle arbitaryData;
	CADT_DictU64Handle arbitaryDataSizes;

} MeshMod_Mesh;

AL2O3_EXTERN_C Handle_Manager32* g_MeshMod_MeshHandleManager;

AL2O3_FORCE_INLINE MeshMod_MeshHandle MeshMod_MeshHandleAlloc(void) {
	MeshMod_MeshHandle handle;
	handle.handle = Handle_Manager32Alloc(g_MeshMod_MeshHandleManager);
	return handle;
}

AL2O3_FORCE_INLINE void MeshMod_MeshHandleRelease(MeshMod_MeshHandle handle) {
	Handle_Manager32Release(g_MeshMod_MeshHandleManager, handle.handle);
}

AL2O3_FORCE_INLINE MeshMod_Mesh* MeshMod_MeshHandleToPtr(MeshMod_MeshHandle handle) {
	return (MeshMod_Mesh*) Handle_Manager32HandleToPtr(g_MeshMod_MeshHandleManager, handle.handle);
}
