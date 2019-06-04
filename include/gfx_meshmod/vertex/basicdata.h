#pragma once
#ifndef GFX_MESHMOD_VERTEXBASICDATA_H
#define GFX_MESHMOD_VERTEXBASICDATA_H

#include "al2o3_platform/platform.h"
#include "al2o3_cmath/vector.h"


#define MeshMod_VertexFTag MESHMOD_VERTEXTAG('D', 'A', 'T', '1', 'F')
#define MeshMod_VertexVec2FTag MESHMOD_VERTEXTAG('D', 'A', 'T', '2', 'F')
#define MeshMod_VertexVec3FTag MESHMOD_VERTEXTAG('D', 'A', 'T', '3', 'F')
#define MeshMod_VertexVec4FTag MESHMOD_VERTEXTAG('D', 'A', 'T', '4', 'F')

#define MeshMod_VertexDTag MESHMOD_VERTEXTAG('D', 'A', 'T', '1', 'D')
#define MeshMod_VertexVec2DTag MESHMOD_VERTEXTAG('D', 'A', 'T', '2', 'D')
#define MeshMod_VertexVec3DTag MESHMOD_VERTEXTAG('D', 'A', 'T', '3', 'D')
#define MeshMod_VertexVec4DTag MESHMOD_VERTEXTAG('D', 'A', 'T', '4', 'D')


#define MESHMOD_VT_REAL(postfix, type) \
	typedef type MeshMod_Vertex##postfix; \
	AL2O3_EXTERN_C bool MeshMod_Vertex##postfix##Equal(void const* va, void const* vb, float const epsilon); \
	AL2O3_EXTERN_C void MeshMod_Vertex##postfix##Interpolate1D(void const* va,	void const* vb,	void* vr,	float const t); \
	AL2O3_EXTERN_C void MeshMod_Vertex##postfix##Interpolate2D(void const* va, void const* vb, void const* vc, void* vr, float const u, float const v);


#define MESHMOD_VT_VECTOR_REAL(postfix) \
	typedef Math_Vec##postfix##_t MeshMod_VertexVec##postfix; \
	AL2O3_EXTERN_C bool MeshMod_VertexVec##postfix##Equal(void const* va, void const* vb, float const epsilon); \
	AL2O3_EXTERN_C void MeshMod_VertexVec##postfix##Interpolate1D(void const* va,	void const* vb,	void* vr,	float const t); \
	AL2O3_EXTERN_C void MeshMod_VertexVec##postfix##Interpolate2D(void const* va, void const* vb, void const* vc, void* vr, float const u, float const v);


MESHMOD_VT_REAL(F, float)
MESHMOD_VT_REAL(D, double)

MESHMOD_VT_VECTOR_REAL(2F)
MESHMOD_VT_VECTOR_REAL(3F)
MESHMOD_VT_VECTOR_REAL(4F)
MESHMOD_VT_VECTOR_REAL(2D)
MESHMOD_VT_VECTOR_REAL(3D)
MESHMOD_VT_VECTOR_REAL(4D)

#undef MESHMOD_VT_REAL
#undef MESHMOD_VT_VECTOR_REAL

#endif // GFX_MESHMOD_VERTEXBASICDATA_H