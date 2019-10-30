#pragma once

#include "al2o3_platform/platform.h"
#include "render_meshmod/meshmod.h"
#include "render_meshmod/mesh.h"

#define MeshMod_EdgeRingTag MESHMOD_EDGETAG('E', 'R', 'I', 'N', 'G')
typedef MeshMod_EdgeHandle MeshMod_EdgeRing;

AL2O3_FORCE_INLINE MeshMod_EdgeRing * MeshMod_EdgeSimilarTagHandleToPtr(MeshMod_MeshHandle meshHandle, MeshMod_EdgeHandle edgeHandle, uint8_t userTag) {
	return (MeshMod_EdgeRing *) MeshMod_MeshEdgeTagHandleToPtr(meshHandle, MeshMod_AddUserDataToEdgeTag(MeshMod_EdgeRingTag, userTag), edgeHandle);
}