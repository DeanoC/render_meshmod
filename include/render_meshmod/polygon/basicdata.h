#pragma once

#include "al2o3_platform/platform.h"
#include "al2o3_cmath/vector.h"

// all unsigned vertex tag ~0 is classified as invalid
#define MeshMod_PolygonU8Tag MESHMOD_POLYGONTAG('D', 'A', 'T', 'U', '8')
#define MeshMod_PolygonU16Tag MESHMOD_POLYGONTAG('D', 'A', 'U', '1', '6')
#define MeshMod_PolygonU32Tag MESHMOD_POLYGONTAG('D', 'A', 'U', '3', '2')
#define MeshMod_PolygonU64Tag MESHMOD_POLYGONTAG('D', 'A', 'U', '6', '4')

#define MeshMod_PolygonFTag MESHMOD_POLYGONTAG('D', 'A', 'T', '1', 'F')
#define MeshMod_PolygonVec2FTag MESHMOD_POLYGONTAG('D', 'A', 'T', '2', 'F')
#define MeshMod_PolygonVec3FTag MESHMOD_POLYGONTAG('D', 'A', 'T', '3', 'F')
#define MeshMod_PolygonVec4FTag MESHMOD_POLYGONTAG('D', 'A', 'T', '4', 'F')

#define MeshMod_PolygonDTag MESHMOD_POLYGONTAG('D', 'A', 'T', '1', 'D')
#define MeshMod_PolygonVec2DTag MESHMOD_POLYGONTAG('D', 'A', 'T', '2', 'D')
#define MeshMod_PolygonVec3DTag MESHMOD_POLYGONTAG('D', 'A', 'T', '3', 'D')
#define MeshMod_PolygonVec4DTag MESHMOD_POLYGONTAG('D', 'A', 'T', '4', 'D')

#define MESHMOD_PT_UNSIGNED(postfix, type) \
	typedef type MeshMod_Polygon##postfix; \
	AL2O3_EXTERN_C bool MeshMod_Polygon##postfix##Equal(void const* va, void const* vb, float const epsilon); \
	AL2O3_EXTERN_C double MeshMod_PolygonVec##postfix##Distance(void const* va, void const* vb); \
	AL2O3_EXTERN_C void MeshMod_Polygon##postfix##Interpolate1D(void const* va,	void const* vb,	void* vr,	float const t); \
	AL2O3_EXTERN_C void MeshMod_Polygon##postfix##Interpolate2D(void const* va, void const* vb, void const* vc, void* vr, float const u, float const v);


#define MESHMOD_PT_REAL(postfix, type) \
	typedef type MeshMod_Polygon##postfix; \
	AL2O3_EXTERN_C bool MeshMod_Polygon##postfix##Equal(void const* va, void const* vb, float const epsilon); \
	AL2O3_EXTERN_C double MeshMod_PolygonVec##postfix##Distance(void const* va, void const* vb); \
	AL2O3_EXTERN_C void MeshMod_Polygon##postfix##Interpolate1D(void const* va,	void const* vb,	void* vr,	float const t); \
	AL2O3_EXTERN_C void MeshMod_Polygon##postfix##Interpolate2D(void const* va, void const* vb, void const* vc, void* vr, float const u, float const v);


#define MESHMOD_PT_VECTOR_REAL(postfix) \
	typedef Math_##postfix MeshMod_Polygon##postfix; \
	AL2O3_EXTERN_C bool MeshMod_Polygon##postfix##Equal(void const* va, void const* vb, float const epsilon); \
	AL2O3_EXTERN_C double MeshMod_Polygon##postfix##Distance(void const* va, void const* vb); \
	AL2O3_EXTERN_C void MeshMod_Polygon##postfix##Interpolate1D(void const* va,	void const* vb,	void* vr,	float const t); \
	AL2O3_EXTERN_C void MeshMod_Polygon##postfix##Interpolate2D(void const* va, void const* vb, void const* vc, void* vr, float const u, float const v);

MESHMOD_PT_UNSIGNED(U8, uint8_t)
MESHMOD_PT_UNSIGNED(U16, uint16_t)
MESHMOD_PT_UNSIGNED(U32, uint32_t)
MESHMOD_PT_UNSIGNED(U64, uint64_t)

MESHMOD_PT_REAL(F, float)
MESHMOD_PT_REAL(D, double)

MESHMOD_PT_VECTOR_REAL(Vec2F)
MESHMOD_PT_VECTOR_REAL(Vec3F)
MESHMOD_PT_VECTOR_REAL(Vec4F)
MESHMOD_PT_VECTOR_REAL(Vec2D)
MESHMOD_PT_VECTOR_REAL(Vec3D)
MESHMOD_PT_VECTOR_REAL(Vec4D)

#undef MESHMOD_PT_REAL
#undef MESHMOD_PT_VECTOR_REAL
