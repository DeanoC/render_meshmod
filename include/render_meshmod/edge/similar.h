#pragma once

#include "al2o3_platform/platform.h"
#include "render_meshmod/mesh.h"

#define MeshMod_EdgeSimilarTag MESHMOD_EDGETAG('S', 'I', 'M', 'L', 'R')
typedef struct MeshMod_EdgeSimilar {
	MeshMod_EdgeHandle next;
} MeshMod_EdgeSimilar;

AL2O3_FORCE_INLINE MeshMod_EdgeSimilar * MeshMod_MeshEdgeSimilarTagHandleToPtr(MeshMod_MeshHandle meshHandle, MeshMod_EdgeHandle edgeHandle, uint8_t userTag) {
	return (MeshMod_EdgeSimilar *) MeshMod_MeshEdgeTagHandleToPtr(meshHandle, MeshMod_AddUserDataToEdgeTag(MeshMod_EdgeSimilarTag, userTag), edgeHandle);
}