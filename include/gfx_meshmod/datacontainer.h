#pragma once
#ifndef GFX_MESHMOD_DATACONTAINER_H
#define GFX_MESHMOD_DATACONTAINER_H

#include "al2o3_platform/platform.h"
#include "al2o3_cadt/vector.h"
#include "al2o3_cadt/bagofvectors.h"
#include "gfx_meshmod/meshmod.h"
#include "gfx_meshmod/registry.h"

typedef struct MeshMod_DataContainer* MeshMod_DataContainerHandle;

AL2O3_EXTERN_C MeshMod_DataContainerHandle MeshMod_DataContainerCreate(struct MeshMod_Mesh* mesh, MeshMod_Type type);
AL2O3_EXTERN_C void MeshMod_DataContainerDestroy(MeshMod_DataContainerHandle handle);

AL2O3_EXTERN_C MeshMod_DataContainerHandle MeshMod_DataContainerClone(MeshMod_DataContainerHandle handle, struct MeshMod_Mesh* newMesh);

AL2O3_EXTERN_C size_t MeshMod_DataContainerSize(MeshMod_DataContainerHandle handle);
AL2O3_EXTERN_C void MeshMod_DataContainerResize(MeshMod_DataContainerHandle handle, size_t size);

// keys should be MeshMod_Tag with 16 bits of user data or'ed in (lowest bits)
AL2O3_EXTERN_C bool MeshMod_DataContainerAdd(MeshMod_DataContainerHandle handle, uint64_t key);
AL2O3_EXTERN_C CADT_VectorHandle MeshMod_DataContainerVectorLookup(MeshMod_DataContainerHandle handle, uint64_t key);
AL2O3_EXTERN_C bool MeshMod_DataContainerIsValid(MeshMod_DataContainerHandle handle, size_t index);
AL2O3_EXTERN_C void MeshMod_DataContainerMarkInvalid(MeshMod_DataContainerHandle handle, size_t index);
AL2O3_EXTERN_C void MeshMod_DataContainerReplace(MeshMod_DataContainerHandle handle, size_t srcIndex, size_t dstIndex);
AL2O3_EXTERN_C void MeshMod_DataContainerSwap(MeshMod_DataContainerHandle handle, size_t index0, size_t index1);
AL2O3_EXTERN_C void MeshMod_DataContainerRemove(MeshMod_DataContainerHandle handle, size_t index);
AL2O3_EXTERN_C void MeshMod_DataContainerSwapRemove(MeshMod_DataContainerHandle handle, size_t index);

// caller owns returns Vector and must destroy when done
AL2O3_EXTERN_C CADT_VectorHandle MeshMod_DataContainerGetValidRemappingTable(MeshMod_DataContainerHandle handlex);
AL2O3_EXTERN_C void MeshMod_DataContainerCompact(MeshMod_DataContainerHandle handle);
// remap is only if the remap table is sorted as its inline use Scatter for general any index use
AL2O3_EXTERN_C size_t MeshMod_DataContainerRemap(MeshMod_DataContainerHandle handle, CADT_VectorHandle remapper);

// only for vertex containers
AL2O3_EXTERN_C MeshMod_VertexIndex MeshMod_DataContainerVertexInterpolate1D(MeshMod_DataContainerHandle handle, MeshMod_VertexIndex i0, MeshMod_VertexIndex i1, float t);
AL2O3_EXTERN_C MeshMod_VertexIndex MeshMod_DataContainerVertexInterpolate2D(MeshMod_DataContainerHandle handle, MeshMod_VertexIndex i0, MeshMod_VertexIndex i1, MeshMod_VertexIndex i2, float u, float v);


#endif // GFX_MESHMOD_DATACONTAINER_H