#pragma once
#include "al2o3_platform/platform.h"
#include "render_meshmod/meshmod.h"

#define MeshMod_PolygonConvexBRepTag MESHMOD_POLYGONTAG('C', 'V', 'X', 'B', 'R')
#define MeshMod_PolygonConvexMaxEdges 16

typedef struct MeshMod_PolygonConvexBRep {
	MeshMod_EdgeHandle edge[MeshMod_PolygonConvexMaxEdges];
	uint8_t numEdges;
} MeshMod_PolygonConvexBRep;
