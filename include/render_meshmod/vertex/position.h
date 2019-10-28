#pragma once

#include "al2o3_platform/platform.h"
#include "al2o3_cmath/vector.h"

#define MeshMod_VertexPositionTag MESHMOD_VERTEXTAG('P', 'O', 'S', '3', 'F')
#define MeshMod_VertexPositionHashTag MESHMOD_VERTEXTAG('P', 'O', 'S', '3', 'F')
typedef Math_Vec3F MeshMod_VertexPosition;

AL2O3_FORCE_INLINE MeshMod_VertexPosition * MeshMod_MeshVertexPositionTagHandleToPtr(MeshMod_MeshHandle meshHandle, MeshMod_VertexHandle vertexHandle) {
	return  (MeshMod_VertexPosition *) MeshMod_MeshVertexTagHandleToPtr(meshHandle, MeshMod_VertexPositionTag, vertexHandle);
}
