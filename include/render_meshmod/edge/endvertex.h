#pragma once

#include "al2o3_platform/platform.h"
#include "render_meshmod/meshmod.h"
#include "render_meshmod/mesh.h"

// half edges only store the start vertex of an edge, which is ideal for polygon
// editing, however its often useful to know where a edge is going without knowing
// or caring about the polygon. EndVertex tag is for this reason.
#define MeshMod_EdgeEndVertexTag MESHMOD_EDGETAG('E', 'N', 'D', 'V', 'T')
typedef MeshMod_VertexHandle MeshMod_EdgeEndVertex;

AL2O3_FORCE_INLINE MeshMod_EdgeEndVertex * MeshMod_MeshEdgeEndVertexTagHandleToPtr(MeshMod_MeshHandle meshHandle, MeshMod_EdgeHandle edgeHandle, uint8_t userTag) {
	return (MeshMod_EdgeEndVertex *) MeshMod_MeshEdgeTagHandleToPtr(meshHandle, MeshMod_AddUserDataToEdgeTag(MeshMod_EdgeEndVertexTag, userTag), edgeHandle);
}