#pragma once

#include "al2o3_platform/platform.h"
#include "render_meshmod/meshmod.h"
#include "render_meshmod/mesh.h"

#define MeshMod_EdgeHalfEdgeTag MESHMOD_EDGETAG('H', 'E', 'D', 'G', 'E')
typedef struct MeshMod_EdgeHalfEdge {
	MeshMod_VertexHandle vertex;
	MeshMod_PolygonHandle polygon;
} MeshMod_EdgeHalfEdge;

AL2O3_FORCE_INLINE MeshMod_EdgeHalfEdge * MeshMod_MeshEdgeHalfEdgeTagHandleToPtr(MeshMod_MeshHandle meshHandle, MeshMod_EdgeHandle edgeHandle) {
	return (MeshMod_EdgeHalfEdge *) MeshMod_MeshEdgeTagHandleToPtr(meshHandle,
																																 MeshMod_EdgeHalfEdgeTag,
																																edgeHandle);
}