#pragma once
#ifndef GFX_MESHMOD_EDGEHALFEDGE_H
#define GFX_MESHMOD_EDGEHALFEDGE_H

#include "al2o3_platform/platform.h"
#include "render_meshmod/meshmod.h"

#define MeshMod_EdgeHalfEdgeTag MESHMOD_EDGETAG('H', 'E', 'D', 'G', 'E')
typedef struct MeshMod_EdgeHalfEdge {
	MeshMod_VertexHandle vertex;
	MeshMod_PolygonHandle polygon;
} MeshMod_EdgeHalfEdge;

#endif