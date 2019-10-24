#pragma once

#include "al2o3_platform/platform.h"
#include "al2o3_handle/handle.h"

typedef struct { Handle_Handle32 handle; } MeshMod_MeshHandle;
typedef struct { Handle_Handle32 handle; } MeshMod_RegistryHandle;

typedef struct { Handle_Handle64 handle; } MeshMod_VertexHandle;
typedef struct { Handle_Handle64 handle; } MeshMod_EdgeHandle;
typedef struct { Handle_Handle64 handle; } MeshMod_PolygonHandle;

// exposed so you can validate mesh and register outside the library
AL2O3_EXTERN_C Handle_Manager32* g_MeshMod_MeshHandleManager;
AL2O3_EXTERN_C Handle_Manager32* g_MeshMod_RegistryHandleManager;

AL2O3_FORCE_INLINE bool MeshMod_MeshHandleIsValid(MeshMod_MeshHandle handle) {
	return Handle_Manager32IsValid(g_MeshMod_MeshHandleManager, handle.handle);
}

AL2O3_FORCE_INLINE bool MeshMod_RegistryHandleIsValid(MeshMod_RegistryHandle handle) {
	return Handle_Manager32IsValid(g_MeshMod_RegistryHandleManager, handle.handle);
}
