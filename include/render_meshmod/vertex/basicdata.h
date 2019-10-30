#pragma once

#include "al2o3_platform/platform.h"
#include "al2o3_cmath/vector.h"
#include "render_meshmod/mesh.h"
// all unsigned vertex tag ~0 is classified as invalid
#define MeshMod_VertexU8Tag MESHMOD_VERTEXTAG('D', 'A', 'T', 'U', '8')
#define MeshMod_VertexU16Tag MESHMOD_VERTEXTAG('D', 'A', 'U', '1', '6')
#define MeshMod_VertexU32Tag MESHMOD_VERTEXTAG('D', 'A', 'U', '3', '2')
#define MeshMod_VertexU64Tag MESHMOD_VERTEXTAG('D', 'A', 'U', '6', '4')

#define MeshMod_VertexFTag MESHMOD_VERTEXTAG('D', 'A', 'T', '1', 'F')
#define MeshMod_VertexVec2FTag MESHMOD_VERTEXTAG('D', 'A', 'T', '2', 'F')
#define MeshMod_VertexVec3FTag MESHMOD_VERTEXTAG('D', 'A', 'T', '3', 'F')
#define MeshMod_VertexVec4FTag MESHMOD_VERTEXTAG('D', 'A', 'T', '4', 'F')

#define MeshMod_VertexDTag MESHMOD_VERTEXTAG('D', 'A', 'T', '1', 'D')
#define MeshMod_VertexVec2DTag MESHMOD_VERTEXTAG('D', 'A', 'T', '2', 'D')
#define MeshMod_VertexVec3DTag MESHMOD_VERTEXTAG('D', 'A', 'T', '3', 'D')
#define MeshMod_VertexVec4DTag MESHMOD_VERTEXTAG('D', 'A', 'T', '4', 'D')

#define MESHMOD_VT_TYPE(postfix, type) typedef type MeshMod_Vertex##postfix; \
AL2O3_FORCE_INLINE MeshMod_Vertex##postfix * MeshMod_MeshMod_Vertex##postfix##TagHandleToPtr(MeshMod_MeshHandle meshHandle, MeshMod_VertexHandle vertexHandle, uint8_t userTag) { \
	return  (MeshMod_Vertex##postfix *) MeshMod_MeshVertexTagHandleToPtr(meshHandle, MeshMod_AddUserDataToVertexTag(MeshMod_Vertex##postfix##Tag, userTag), vertexHandle); \
}

#define MESHMOD_VT_VECTOR_TYPE(postfix) typedef Math_##postfix MeshMod_Vertex##postfix; \
AL2O3_FORCE_INLINE MeshMod_Vertex##postfix * MeshMod_MeshMod_Vertex##postfix##TagHandleToPtr(MeshMod_MeshHandle meshHandle, MeshMod_VertexHandle vertexHandle, uint8_t userTag) { \
	return  (MeshMod_Vertex##postfix *) MeshMod_MeshVertexTagHandleToPtr(meshHandle, MeshMod_AddUserDataToVertexTag(MeshMod_Vertex##postfix##Tag, userTag), vertexHandle); \
}

MESHMOD_VT_TYPE(U8, uint8_t)
MESHMOD_VT_TYPE(U16, uint16_t)
MESHMOD_VT_TYPE(U32, uint32_t)
MESHMOD_VT_TYPE(U64, uint64_t)

MESHMOD_VT_TYPE(F, float)
MESHMOD_VT_TYPE(D, double)

MESHMOD_VT_VECTOR_TYPE(Vec2F)
MESHMOD_VT_VECTOR_TYPE(Vec3F)
MESHMOD_VT_VECTOR_TYPE(Vec4F)
MESHMOD_VT_VECTOR_TYPE(Vec2D)
MESHMOD_VT_VECTOR_TYPE(Vec3D)
MESHMOD_VT_VECTOR_TYPE(Vec4D)

#undef MESHMOD_VT_TYPE
#undef MESHMOD_VT_VECTOR_TYPE
