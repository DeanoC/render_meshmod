#pragma once
#include "al2o3_platform/platform.h"
#include "render_meshmod/meshmod.h"

#define MeshMod_PolygonQuadBRepTag MESHMOD_POLYGONTAG('Q', 'U', 'D', 'B', 'R')
typedef struct MeshMod_PolygonQuadBRep {
	MeshMod_EdgeHandle edge[4];
} MeshMod_PolygonQuadBRep;
