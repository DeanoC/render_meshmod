#pragma once
#include "al2o3_platform/platform.h"
#include "render_meshmod/meshmod.h"

#define MeshMod_PolygonTriBRepTag MESHMOD_POLYGONTAG('T', 'R', 'I', 'B', 'R')
typedef struct MeshMod_PolygonTriBRep {
	MeshMod_EdgeHandle edge[3];
} MeshMod_PolygonTriBRep;
