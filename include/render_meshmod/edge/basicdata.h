#pragma once

#include "al2o3_platform/platform.h"
#include "al2o3_cmath/vector.h"

// all unsigned vertex tag ~0 is classified as invalid
#define MeshMod_EdgeU8Tag MESHMOD_EDGETAG('D', 'A', 'T', 'U', '8')
#define MeshMod_EdgeU16Tag MESHMOD_EDGETAG('D', 'A', 'U', '1', '6')
#define MeshMod_EdgeU32Tag MESHMOD_EDGETAG('D', 'A', 'U', '3', '2')
#define MeshMod_EdgeU64Tag MESHMOD_EDGETAG('D', 'A', 'U', '6', '4')

#define MeshMod_EdgeFTag MESHMOD_EDGETAG('D', 'A', 'T', '1', 'F')
#define MeshMod_EdgeVec2FTag MESHMOD_EDGETAG('D', 'A', 'T', '2', 'F')
#define MeshMod_EdgeVec3FTag MESHMOD_EDGETAG('D', 'A', 'T', '3', 'F')
#define MeshMod_EdgeVec4FTag MESHMOD_EDGETAG('D', 'A', 'T', '4', 'F')

#define MeshMod_EdgeDTag MESHMOD_EDGETAG('D', 'A', 'T', '1', 'D')
#define MeshMod_EdgeVec2DTag MESHMOD_EDGETAG('D', 'A', 'T', '2', 'D')
#define MeshMod_EdgeVec3DTag MESHMOD_EDGETAG('D', 'A', 'T', '3', 'D')
#define MeshMod_EdgeVec4DTag MESHMOD_EDGETAG('D', 'A', 'T', '4', 'D')

#define MESHMOD_ET_UNSIGNED(postfix, type) \
	typedef type MeshMod_Edge##postfix; \
	AL2O3_EXTERN_C bool MeshMod_Edge##postfix##Equal(void const* va, void const* vb, float const epsilon); \
	AL2O3_EXTERN_C double MeshMod_EdgeVec##postfix##Distance(void const* va, void const* vb); \
	AL2O3_EXTERN_C void MeshMod_Edge##postfix##Interpolate1D(void const* va,	void const* vb,	void* vr,	float const t); \
	AL2O3_EXTERN_C void MeshMod_Edge##postfix##Interpolate2D(void const* va, void const* vb, void const* vc, void* vr, float const u, float const v);


#define MESHMOD_ET_REAL(postfix, type) \
	typedef type MeshMod_Edge##postfix; \
	AL2O3_EXTERN_C bool MeshMod_Edge##postfix##Equal(void const* va, void const* vb, float const epsilon); \
	AL2O3_EXTERN_C double MeshMod_EdgeVec##postfix##Distance(void const* va, void const* vb); \
	AL2O3_EXTERN_C void MeshMod_Edge##postfix##Interpolate1D(void const* va,	void const* vb,	void* vr,	float const t); \
	AL2O3_EXTERN_C void MeshMod_Edge##postfix##Interpolate2D(void const* va, void const* vb, void const* vc, void* vr, float const u, float const v);


#define MESHMOD_ET_VECTOR_REAL(postfix) \
	typedef Math_##postfix MeshMod_Edge##postfix; \
	AL2O3_EXTERN_C bool MeshMod_Edge##postfix##Equal(void const* va, void const* vb, float const epsilon); \
	AL2O3_EXTERN_C double MeshMod_Edge##postfix##Distance(void const* va, void const* vb); \
	AL2O3_EXTERN_C void MeshMod_Edge##postfix##Interpolate1D(void const* va,	void const* vb,	void* vr,	float const t); \
	AL2O3_EXTERN_C void MeshMod_Edge##postfix##Interpolate2D(void const* va, void const* vb, void const* vc, void* vr, float const u, float const v);

MESHMOD_ET_UNSIGNED(U8, uint8_t)
MESHMOD_ET_UNSIGNED(U16, uint16_t)
MESHMOD_ET_UNSIGNED(U32, uint32_t)
MESHMOD_ET_UNSIGNED(U64, uint64_t)

MESHMOD_ET_REAL(F, float)
MESHMOD_ET_REAL(D, double)

MESHMOD_ET_VECTOR_REAL(Vec2F)
MESHMOD_ET_VECTOR_REAL(Vec3F)
MESHMOD_ET_VECTOR_REAL(Vec4F)
MESHMOD_ET_VECTOR_REAL(Vec2D)
MESHMOD_ET_VECTOR_REAL(Vec3D)
MESHMOD_ET_VECTOR_REAL(Vec4D)

#undef MESHMOD_ET_REAL
#undef MESHMOD_ET_VECTOR_REAL
