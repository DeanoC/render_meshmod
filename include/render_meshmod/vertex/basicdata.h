#pragma once

#include "al2o3_platform/platform.h"
#include "al2o3_cmath/vector.h"

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

#define MESHMOD_VT_UNSIGNED(postfix, type) \
	typedef type MeshMod_Vertex##postfix; \
	AL2O3_EXTERN_C bool MeshMod_Vertex##postfix##Equal(void const* va, void const* vb, float const epsilon); \
	AL2O3_EXTERN_C double MeshMod_VertexVec##postfix##Distance(void const* va, void const* vb); \
	AL2O3_EXTERN_C void MeshMod_Vertex##postfix##Interpolate1D(void const* va,	void const* vb,	void* vr,	float const t); \
	AL2O3_EXTERN_C void MeshMod_Vertex##postfix##Interpolate2D(void const* va, void const* vb, void const* vc, void* vr, float const u, float const v);


#define MESHMOD_VT_REAL(postfix, type) \
	typedef type MeshMod_Vertex##postfix; \
	AL2O3_EXTERN_C bool MeshMod_Vertex##postfix##Equal(void const* va, void const* vb, float const epsilon); \
	AL2O3_EXTERN_C double MeshMod_VertexVec##postfix##Distance(void const* va, void const* vb); \
	AL2O3_EXTERN_C void MeshMod_Vertex##postfix##Interpolate1D(void const* va,	void const* vb,	void* vr,	float const t); \
	AL2O3_EXTERN_C void MeshMod_Vertex##postfix##Interpolate2D(void const* va, void const* vb, void const* vc, void* vr, float const u, float const v);


#define MESHMOD_VT_VECTOR_REAL(postfix) \
	typedef Math_##postfix MeshMod_Vertex##postfix; \
	AL2O3_EXTERN_C bool MeshMod_Vertex##postfix##Equal(void const* va, void const* vb, float const epsilon); \
	AL2O3_EXTERN_C double MeshMod_Vertex##postfix##Distance(void const* va, void const* vb); \
	AL2O3_EXTERN_C void MeshMod_Vertex##postfix##Interpolate1D(void const* va,	void const* vb,	void* vr,	float const t); \
	AL2O3_EXTERN_C void MeshMod_Vertex##postfix##Interpolate2D(void const* va, void const* vb, void const* vc, void* vr, float const u, float const v);

MESHMOD_VT_UNSIGNED(U8, uint8_t)
MESHMOD_VT_UNSIGNED(U16, uint16_t)
MESHMOD_VT_UNSIGNED(U32, uint32_t)
MESHMOD_VT_UNSIGNED(U64, uint64_t)

MESHMOD_VT_REAL(F, float)
MESHMOD_VT_REAL(D, double)

MESHMOD_VT_VECTOR_REAL(Vec2F)
MESHMOD_VT_VECTOR_REAL(Vec3F)
MESHMOD_VT_VECTOR_REAL(Vec4F)
MESHMOD_VT_VECTOR_REAL(Vec2D)
MESHMOD_VT_VECTOR_REAL(Vec3D)
MESHMOD_VT_VECTOR_REAL(Vec4D)

#undef MESHMOD_VT_REAL
#undef MESHMOD_VT_VECTOR_REAL
