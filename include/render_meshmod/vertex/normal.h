#pragma once

#include "al2o3_platform/platform.h"
#include "al2o3_cmath/vector.h"

#define MeshMod_VertexNormalTag MESHMOD_VERTEXTAG('N', 'R', 'M', '3', 'F')
typedef Math_Vec3F MeshMod_VertexNormal;

AL2O3_FORCE_INLINE MeshMod_VertexNormal * MeshMod_MeshMod_VertexNormalTagHandleToPtr(MeshMod_MeshHandle meshHandle, MeshMod_VertexHandle vertexHandle) {
	return  (MeshMod_VertexNormal *) MeshMod_MeshVertexTagHandleToPtr(meshHandle, MeshMod_VertexNormalTag, vertexHandle);
}
