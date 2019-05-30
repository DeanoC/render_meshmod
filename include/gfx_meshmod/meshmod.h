#pragma once
#ifndef GFX_MESHMOD_MESHMOD_H
#define GFX_MESHMOD_MESHMOD_H

#include "al2o3_platform/platform.h"
#include "al2o3_cadt/bagofvectors.h"

typedef struct MeshMod_Mesh *MeshMod_MeshHandle;

typedef struct MeshMod_DataContainer* MeshMod_DataContainerHandle;

typedef size_t MeshMod_VertexIndex;
typedef size_t MeshMod_EdgeIndex;
typedef size_t MeshMod_PolygonIndex;

typedef void* MeshMod_HalfEdgeArrayHandle;
typedef size_t MeshMod_HalfEdgeIndex;


#endif // GFX_MESHMOD_MESHMOD_H