#pragma once
#ifndef GFX_MESHMOD_MESH_H
#define GFX_MESHMOD_MESH_H

#include "al2o3_platform/platform.h"
#include "gfx_meshmod/vertexarray.h"
#include "gfx_meshmod/meshmod.h"

AL2O3_EXTERN_C MeshMod_MeshHandle MeshMod_MeshCreate(MeshMod_TagRegistryHandle registry, char const* name);
AL2O3_EXTERN_C void MeshMod_MeshDestroy(MeshMod_MeshHandle handle);

AL2O3_EXTERN_C MeshMod_MeshResolveVertexIndexChanges(MeshMod_MeshHandle);

AL2O3_EXTERN_C MeshMod_DataContainerHandle MeshMod_GetVertices(MeshMod_MeshHandle handle);
AL2O3_EXTERN_C MeshMod_DataContainerHandle MeshMod_GetEdges(MeshMod_MeshHandle handle);
AL2O3_EXTERN_C MeshMod_DataContainerHandle MeshMod_GetPolygons(MeshMod_MeshHandle handle);

AL2O3_EXTERN_C size_t MeshMod_DataContainerSize(MeshMod_DataContainerHandle handle);
AL2O3_EXTERN_C void MeshMod_DataContainerResize(MeshMod_DataContainerHandle handle, size_t size);



#endif // GFX_MESHMOD_MESH_H