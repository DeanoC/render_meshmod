#pragma once
#ifndef GFX_MESHMOD_MESH_H
#define GFX_MESHMOD_MESH_H

#include "al2o3_platform/platform.h"
#include "gfx_meshmod/vertexarray.h"
#include "gfx_meshmod/meshmod.h"

AL2O3_EXTERN_C MeshMod_MeshHandle MeshMod_CreateMesh(char const* name);
AL2O3_EXTERN_C void MeshMod_Destroy(MeshMod_MeshHandle handle);

AL2O3_EXTERN_C MeshMod_VertexArrayContainerHandle MeshMod_GetVertices(MeshMod_MeshHandle handle);
AL2O3_EXTERN_C MeshMod_EdgeArrayContainerHandle MeshMod_GetEdges(MeshMod_MeshHandle handle);
AL2O3_EXTERN_C MeshMod_PolygonArrayContainerHandle eshMod_GetPolygons(MeshMod_MeshHandle handle);



#endif // GFX_MESHMOD_MESH_H