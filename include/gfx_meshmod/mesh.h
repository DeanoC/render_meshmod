#pragma once
#ifndef GFX_MESHMOD_MESH_H
#define GFX_MESHMOD_MESH_H

#include "al2o3_platform/platform.h"
#include "gfx_meshmod/meshmod.h"
#include "gfx_meshmod/registry.h"
#include "gfx_meshmod/datacontainer.h"

AL2O3_EXTERN_C MeshMod_MeshHandle MeshMod_MeshCreateWithDefaultRegistry(char const* name);
AL2O3_EXTERN_C MeshMod_MeshHandle MeshMod_MeshCreate(MeshMod_RegistryHandle registry, char const* name);
AL2O3_EXTERN_C void MeshMod_MeshDestroy(MeshMod_MeshHandle handle);

AL2O3_EXTERN_C MeshMod_RegistryHandle MeshMod_MeshGetRegistry(MeshMod_MeshHandle handle);

AL2O3_EXTERN_C MeshMod_DataContainerHandle MeshMod_MeshGetVertices(MeshMod_MeshHandle handle);
AL2O3_EXTERN_C MeshMod_DataContainerHandle MeshMod_MeshGetEdges(MeshMod_MeshHandle handle);
AL2O3_EXTERN_C MeshMod_DataContainerHandle MeshMod_MeshGetPolygons(MeshMod_MeshHandle handle);

AL2O3_EXTERN_C MeshMod_MeshHandle MeshMod_MeshClone(MeshMod_MeshHandle handle);

// its often useful to attach single peice of typed data to a mesh (various bounding volumes etc.)
AL2O3_EXTERN_C void* MeshMod_MeshAddDataZeroed(MeshMod_MeshHandle handle, MeshMod_Tag tag, size_t dataLen);
AL2O3_EXTERN_C void* MeshMod_MeshAddData(MeshMod_MeshHandle handle, MeshMod_Tag tag, void* data, size_t dataLen);
AL2O3_EXTERN_C void* MeshMod_MeshGetData(MeshMod_MeshHandle handle, MeshMod_Tag tag);
AL2O3_EXTERN_C void MeshMod_MeshRemoveData(MeshMod_MeshHandle handle, MeshMod_Tag tag);
AL2O3_EXTERN_C bool MeshMod_MeshHasData(MeshMod_MeshHandle handle, MeshMod_Tag tag);

#endif // GFX_MESHMOD_MESH_H