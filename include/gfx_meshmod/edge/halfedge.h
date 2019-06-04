#pragma once
#ifndef GFX_MESHMOD_EDGEHALFEDGE_H
#define GFX_MESHMOD_EDGEHALFEDGE_H

#include "al2o3_platform/platform.h"
#include "gfx_meshmod/meshmod.h"

#define MeshMod_EdgeHalfEdgeTag MESHMOD_EDGETAG('H', 'E', 'D', 'G', 'E')
typedef struct MeshMod_EdgeHalfEdge {
	MeshMod_VertexIndex vertex;
	MeshMod_PolygonIndex polygon;
} MeshMod_EdgeHalfEdge;

#endif