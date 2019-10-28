#pragma once

#include "al2o3_platform/platform.h"
#include "al2o3_cmath/vector.h"
#include "render_meshmod/meshmod.h"
#include "render_meshmod/mesh.h"

// similar vertex attributes are a index ring defining similar (close to each other) over
// an attribute. Often called Point Rep for position they are usually transitory

// makes certain spatial queryies fast and easy to reason about N dimenstional subset of a vertex

// Vertex, Edge and Polygon all have general rings for other cases

// similar reserves user tag P, N, U so far
#define MeshMod_VertexSimilarTag MESHMOD_VERTEXTAG('S', 'I', 'M', 'L', 'R')
typedef MeshMod_VertexHandle MeshMod_VertexSimilar;

AL2O3_FORCE_INLINE MeshMod_VertexSimilar * MeshMod_MeshVertexSimilarTagHandleToPtr(MeshMod_MeshHandle meshHandle, MeshMod_VertexHandle vertexHandle) {
	return  (MeshMod_VertexSimilar *) MeshMod_MeshVertexTagHandleToPtr(meshHandle, MeshMod_VertexSimilarTag, vertexHandle);
}
