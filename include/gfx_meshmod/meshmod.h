#pragma once
#ifndef GFX_MESHMOD_MESHMOD_H
#define GFX_MESHMOD_MESHMOD_H

#include "al2o3_platform/platform.h"

typedef struct MeshMod_Mesh *MeshMod_MeshHandle;

// the top byte is the MeshMod_Type (ascii 'V', 'E', 'P')
// the bottom two bytes of a tag are ignored by the registry and can be used to hold user data
typedef uint64_t MeshMod_Tag;

typedef enum MeshMod_Type {
	MeshMod_TypeVertex = 'V',
	MeshMod_TypeEdge = 'E',
	MeshMod_TypePolygon = 'P',
} MeshMod_Type;

#define MeshMod_InvalidIndex 0xFFFFFFFFFFFFFFFFUL
typedef size_t MeshMod_VertexIndex;
typedef size_t MeshMod_EdgeIndex;
typedef size_t MeshMod_PolygonIndex;

#endif // GFX_MESHMOD_MESHMOD_H