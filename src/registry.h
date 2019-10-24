#pragma once

typedef struct MeshMod_RegistryItem {
	MeshMod_Type type;
	size_t elementSize;
	char const* description;

	MeshMod_RegistryCommonFunctionTable* commonFunctionTable;

	union {
		void* functionTable;
		MeshMod_RegistryVertexFunctionTable* vertexFunctionTable;
		MeshMod_RegistryEdgeFunctionTable* edgeFunctionTable;
		MeshMod_RegistryPolygonFunctionTable* polygonFunctionTable;
	};

} MeshMod_RegistryItem;

typedef struct MeshMod_Registry {
	CADT_VectorHandle registry;
	CADT_DictU64Handle tagDictionary;
} MeshMod_Registry;

AL2O3_EXTERN_C Handle_Manager32* g_MeshMod_RegistryHandleManager;

AL2O3_FORCE_INLINE MeshMod_RegistryHandle MeshMod_RegistryHandleAlloc(void) {
	MeshMod_RegistryHandle handle;
	handle.handle = Handle_Manager32Alloc(g_MeshMod_RegistryHandleManager);
	return handle;
}
AL2O3_FORCE_INLINE void MeshMod_RegistryHandleRelease(MeshMod_RegistryHandle handle) {
	Handle_Manager32Release(g_MeshMod_RegistryHandleManager, handle.handle);
}

AL2O3_FORCE_INLINE MeshMod_Registry* MeshMod_RegistryHandleToPtr(MeshMod_RegistryHandle handle) {
	return (MeshMod_Registry*) Handle_Manager32HandleToPtr(g_MeshMod_RegistryHandleManager, handle.handle);
}
