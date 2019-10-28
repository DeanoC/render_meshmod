#pragma once

#include "al2o3_platform/platform.h"
#include "al2o3_cmath/vector.h"

#define MeshMod_VertexUvTag MESHMOD_VERTEXTAG('U', 'V', '_', '2', 'F')
typedef Math_Vec2F MeshMod_VertexUv;

AL2O3_FORCE_INLINE MeshMod_VertexUv * MeshMod_VertexUvTagHandleToPtr(MeshMod_MeshHandle meshHandle, MeshMod_VertexHandle vertexHandle) {
	return  (MeshMod_VertexUv *) MeshMod_MeshVertexTagHandleToPtr(meshHandle, MeshMod_VertexUvTag, vertexHandle);
}
