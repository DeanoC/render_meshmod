#pragma once

#include "al2o3_platform/platform.h"
#include "render_meshmod/meshmod.h"
#include "render_meshmod/mesh.h"

#define MeshMod_VertexRingTag MESHMOD_VERTEXTAG('V', 'R', 'I', 'N', 'G')
typedef struct MeshMod_VertexRing {
	MeshMod_VertexHandle next;
} MeshMod_VertexRing;

AL2O3_FORCE_INLINE MeshMod_VertexRing * MeshMod_VertexRingTagHandleToPtr(MeshMod_MeshHandle meshHandle, MeshMod_VertexHandle vertexHandle, uint8_t userTag) {
	return  (MeshMod_VertexRing *) MeshMod_MeshVertexTagHandleToPtr(meshHandle, MeshMod_AddUserDataToVertexTag(MeshMod_VertexRingTag, userTag), vertexHandle);
}
