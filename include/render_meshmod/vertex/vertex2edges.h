#pragma once

#include "al2o3_platform/platform.h"
#include "render_meshmod/meshmod.h"
#include "render_meshmod/mesh.h"

#define MeshMod_Vertex2EdgesTag MESHMOD_VERTEXTAG('V', 'T', '2', 'E', 'D')
#define MeshMod_Vertex2EdgesMaxEmbedded 4

typedef struct MeshMod_Vertex2Edges {
	MeshMod_EdgeHandle *edges;
	MeshMod_EdgeHandle embedded[MeshMod_Vertex2EdgesMaxEmbedded];

	uint8_t numEdges;
} MeshMod_Vertex2Edges;

AL2O3_FORCE_INLINE MeshMod_Vertex2Edges * MeshMod_MeshVertex2EdgesTagHandleToPtr(MeshMod_MeshHandle meshHandle, MeshMod_VertexHandle vertexHandle, uint8_t userTag) {
	return  (MeshMod_Vertex2Edges *) MeshMod_MeshVertexTagHandleToPtr(meshHandle, MeshMod_AddUserDataToVertexTag(MeshMod_Vertex2EdgesTag, userTag), vertexHandle);
}
