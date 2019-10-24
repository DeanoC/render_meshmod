#pragma once
#ifndef GFX_MESHMOD_MESH_H
#define GFX_MESHMOD_MESH_H

#include "al2o3_platform/platform.h"
#include "render_meshmod/meshmod.h"
#include "render_meshmod/registry.h"

// For the Iterate functions
// pass in previous == NULL to start the iteration, pass in previous. stop if return handle is {0}

AL2O3_EXTERN_C MeshMod_MeshHandle MeshMod_MeshCreate(MeshMod_RegistryHandle registry, char const* name);
AL2O3_EXTERN_C void MeshMod_MeshDestroy(MeshMod_MeshHandle handle);
AL2O3_EXTERN_C MeshMod_RegistryHandle MeshMod_MeshGetRegistry(MeshMod_MeshHandle handle);
AL2O3_EXTERN_C void MeshMod_MeshVertexDataHasChanged(MeshMod_MeshHandle mhandle);
AL2O3_EXTERN_C void MeshMod_MeshEdgeDataHasChanged(MeshMod_MeshHandle mhandle);
AL2O3_EXTERN_C void MeshMod_MeshPolygonDataHasChanged(MeshMod_MeshHandle mhandle);

// Vertex API
AL2O3_EXTERN_C MeshMod_VertexHandle MeshMod_MeshVertexAlloc(MeshMod_MeshHandle handle);
AL2O3_EXTERN_C MeshMod_VertexHandle MeshMod_MeshVertexDuplicate(MeshMod_MeshHandle handle, MeshMod_VertexHandle srcHandle);
AL2O3_EXTERN_C void MeshMod_MeshVertexRelease(MeshMod_MeshHandle mhandle, MeshMod_VertexHandle vhandle);

AL2O3_EXTERN_C bool MeshMod_MeshVertexIsValid(MeshMod_MeshHandle mhandle,  MeshMod_VertexHandle handle);
AL2O3_EXTERN_C void MeshMod_MeshVertexReplace(MeshMod_MeshHandle mhandle,  MeshMod_VertexHandle srcHandle, MeshMod_VertexHandle dstHandle);
AL2O3_EXTERN_C void MeshMod_MeshVertexSwap(MeshMod_MeshHandle mhandle,  MeshMod_VertexHandle handle0, MeshMod_VertexHandle handle1);
AL2O3_EXTERN_C MeshMod_VertexHandle MeshMod_MeshVertexIterate(MeshMod_MeshHandle mhandle, MeshMod_VertexHandle* previous);

AL2O3_EXTERN_C void MeshMod_MeshVertexTagEnsure(MeshMod_MeshHandle mhandle, MeshMod_Tag tag);
AL2O3_EXTERN_C bool MeshMod_MeshVertexTagExists(MeshMod_MeshHandle mhandle, MeshMod_Tag tag);
AL2O3_EXTERN_C void MeshMod_MeshVertexTagRemove(MeshMod_MeshHandle mhandle, MeshMod_Tag tag);
AL2O3_EXTERN_C uint64_t MeshMod_MeshVertexTagComputeHash(MeshMod_MeshHandle mhandle, MeshMod_Tag tag);
AL2O3_EXTERN_C void* MeshMod_MeshVertexTagHandleToPtr(MeshMod_MeshHandle mhandle, MeshMod_Tag tag, MeshMod_VertexHandle handle);

AL2O3_EXTERN_C MeshMod_VertexHandle MeshMod_MeshVertexInterpolate1D(MeshMod_MeshHandle mhandle, MeshMod_VertexHandle handle0, MeshMod_VertexHandle handle1, float t);
AL2O3_EXTERN_C MeshMod_VertexHandle MeshMod_MeshVertexInterpolate2D(MeshMod_MeshHandle mhandle, MeshMod_VertexHandle handle0, MeshMod_VertexHandle handle1, MeshMod_VertexHandle handle2, float u, float v);

// Edge API
AL2O3_EXTERN_C MeshMod_EdgeHandle MeshMod_MeshEdgeAlloc(MeshMod_MeshHandle handle);
AL2O3_EXTERN_C MeshMod_EdgeHandle MeshMod_MeshEdgeDuplicate(MeshMod_MeshHandle handle, MeshMod_EdgeHandle srcHandle);
AL2O3_EXTERN_C void MeshMod_MeshEdgeRelease(MeshMod_MeshHandle mhandle, MeshMod_EdgeHandle vhandle);

AL2O3_EXTERN_C bool MeshMod_MeshEdgeIsValid(MeshMod_MeshHandle mhandle,  MeshMod_EdgeHandle handle);
AL2O3_EXTERN_C void MeshMod_MeshEdgeReplace(MeshMod_MeshHandle mhandle,  MeshMod_EdgeHandle srcHandle, MeshMod_EdgeHandle dstHandle);
AL2O3_EXTERN_C void MeshMod_MeshEdgeSwap(MeshMod_MeshHandle mhandle,  MeshMod_EdgeHandle handle0, MeshMod_EdgeHandle handle1);
AL2O3_EXTERN_C MeshMod_EdgeHandle MeshMod_MeshEdgeIterate(MeshMod_MeshHandle mhandle, MeshMod_EdgeHandle* previous);

AL2O3_EXTERN_C void MeshMod_MeshEdgeTagEnsure(MeshMod_MeshHandle mhandle, MeshMod_Tag tag);
AL2O3_EXTERN_C bool MeshMod_MeshEdgeTagExists(MeshMod_MeshHandle mhandle, MeshMod_Tag tag);
AL2O3_EXTERN_C void MeshMod_MeshEdgeTagRemove(MeshMod_MeshHandle mhandle, MeshMod_Tag tag);
AL2O3_EXTERN_C uint64_t MeshMod_MeshEdgeTagComputeHash(MeshMod_MeshHandle mhandle, MeshMod_Tag tag);
AL2O3_EXTERN_C void* MeshMod_MeshEdgeTagHandleToPtr(MeshMod_MeshHandle mhandle, MeshMod_Tag tag, MeshMod_EdgeHandle handle);

// Polygon API
AL2O3_EXTERN_C MeshMod_PolygonHandle MeshMod_MeshPolygonAlloc(MeshMod_MeshHandle handle);
AL2O3_EXTERN_C MeshMod_PolygonHandle MeshMod_MeshPolygonDuplicate(MeshMod_MeshHandle handle, MeshMod_PolygonHandle srcHandle);
AL2O3_EXTERN_C void MeshMod_MeshPolygonRelease(MeshMod_MeshHandle mhandle, MeshMod_PolygonHandle vhandle);

AL2O3_EXTERN_C bool MeshMod_MeshPolygonIsValid(MeshMod_MeshHandle mhandle,  MeshMod_PolygonHandle handle);
AL2O3_EXTERN_C void MeshMod_MeshPolygonReplace(MeshMod_MeshHandle mhandle,  MeshMod_PolygonHandle srcHandle, MeshMod_PolygonHandle dstHandle);
AL2O3_EXTERN_C void MeshMod_MeshPolygonSwap(MeshMod_MeshHandle mhandle,  MeshMod_PolygonHandle handle0, MeshMod_PolygonHandle handle1);
AL2O3_EXTERN_C MeshMod_PolygonHandle MeshMod_MeshPolygonIterate(MeshMod_MeshHandle mhandle, MeshMod_PolygonHandle* previous);

AL2O3_EXTERN_C void MeshMod_MeshPolygonTagEnsure(MeshMod_MeshHandle mhandle, MeshMod_Tag tag);
AL2O3_EXTERN_C bool MeshMod_MeshPolygonTagExists(MeshMod_MeshHandle mhandle, MeshMod_Tag tag);
AL2O3_EXTERN_C void MeshMod_MeshPolygonTagRemove(MeshMod_MeshHandle mhandle, MeshMod_Tag tag);
AL2O3_EXTERN_C uint64_t MeshMod_MeshPolygonTagComputeHash(MeshMod_MeshHandle mhandle, MeshMod_Tag tag);
AL2O3_EXTERN_C void* MeshMod_MeshPolygonTagHandleToPtr(MeshMod_MeshHandle mhandle, MeshMod_Tag tag, MeshMod_PolygonHandle handle);
AL2O3_EXTERN_C MeshMod_PolygonHandle MeshMod_MeshPolygonTagIterate(MeshMod_MeshHandle mhandle, MeshMod_Tag tag, MeshMod_PolygonHandle* previous);

// Mesh Data API
AL2O3_EXTERN_C void* MeshMod_MeshAddDataZeroed(MeshMod_MeshHandle handle, MeshMod_Tag tag, size_t dataLen);
AL2O3_EXTERN_C void* MeshMod_MeshAddData(MeshMod_MeshHandle handle, MeshMod_Tag tag, void* data, size_t dataLen);
AL2O3_EXTERN_C void* MeshMod_MeshGetData(MeshMod_MeshHandle handle, MeshMod_Tag tag);
AL2O3_EXTERN_C void MeshMod_MeshRemoveData(MeshMod_MeshHandle handle, MeshMod_Tag tag);
AL2O3_EXTERN_C bool MeshMod_MeshHasData(MeshMod_MeshHandle handle, MeshMod_Tag tag);

#endif // GFX_MESHMOD_MESH_H