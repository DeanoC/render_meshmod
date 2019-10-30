#pragma once

#include "al2o3_platform/platform.h"
#include "al2o3_cmath/vector.h"
#include "render_meshmod/mesh.h"

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

#define MESHMOD_PT_TYPE(postfix, type) typedef type MeshMod_Polygon##postfix; \
AL2O3_FORCE_INLINE MeshMod_Polygon##postfix * MeshMod_MeshMod_Polygon##postfix##TagHandleToPtr(MeshMod_MeshHandle meshHandle, MeshMod_PolygonHandle polygonHandle, uint8_t userTag) { \
	return  (MeshMod_Polygon##postfix *) MeshMod_MeshPolygonTagHandleToPtr(meshHandle, MeshMod_AddUserDataToPolygonTag(MeshMod_Polygon##postfix##Tag, userTag), polygonHandle); \
}

#define MESHMOD_PT_VECTOR_TYPE(postfix) typedef Math_##postfix MeshMod_Polygon##postfix; \
AL2O3_FORCE_INLINE MeshMod_Polygon##postfix * MeshMod_MeshMod_Polygon##postfix##TagHandleToPtr(MeshMod_MeshHandle meshHandle, MeshMod_PolygonHandle polygonHandle, uint8_t userTag) { \
	return  (MeshMod_Polygon##postfix *) MeshMod_MeshPolygonTagHandleToPtr(meshHandle, MeshMod_AddUserDataToPolygonTag(MeshMod_Polygon##postfix##Tag, userTag), polygonHandle); \
}


MESHMOD_PT_TYPE(U8, uint8_t)
MESHMOD_PT_TYPE(U16, uint16_t)
MESHMOD_PT_TYPE(U32, uint32_t)
MESHMOD_PT_TYPE(U64, uint64_t)

MESHMOD_PT_TYPE(F, float)
MESHMOD_PT_TYPE(D, double)

MESHMOD_PT_VECTOR_TYPE(Vec2F)
MESHMOD_PT_VECTOR_TYPE(Vec3F)
MESHMOD_PT_VECTOR_TYPE(Vec4F)
MESHMOD_PT_VECTOR_TYPE(Vec2D)
MESHMOD_PT_VECTOR_TYPE(Vec3D)
MESHMOD_PT_VECTOR_TYPE(Vec4D)

#undef MESHMOD_PT_TYPE
#undef MESHMOD_PT_VECTOR_TYPE
