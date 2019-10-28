#pragma once

#include "al2o3_platform/platform.h"
#include "al2o3_cmath/vector.h"
#include "render_meshmod/mesh.h"

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

#define MESHMOD_ET_TYPE(postfix, type) typedef type MeshMod_Edge##postfix; \
AL2O3_FORCE_INLINE MeshMod_Edge##postfix * MeshMod_MeshMod_Edge##postfix##TagHandleToPtr(MeshMod_MeshHandle meshHandle, MeshMod_EdgeHandle edgeHandle) { \
	return  (MeshMod_Edge##postfix *) MeshMod_MeshEdgeTagHandleToPtr(meshHandle, MeshMod_Edge##postfix##Tag, edgeHandle); \
}

#define MESHMOD_ET_VECTOR_TYPE(postfix) typedef Math_##postfix MeshMod_Edge##postfix; \
AL2O3_FORCE_INLINE MeshMod_Edge##postfix * MeshMod_MeshMod_Edge##postfix##TagHandleToPtr(MeshMod_MeshHandle meshHandle, MeshMod_EdgeHandle edgeHandle) { \
	return  (MeshMod_Edge##postfix *) MeshMod_MeshEdgeTagHandleToPtr(meshHandle, MeshMod_Edge##postfix##Tag, edgeHandle); \
}

MESHMOD_ET_TYPE(U8, uint8_t)
MESHMOD_ET_TYPE(U16, uint16_t)
MESHMOD_ET_TYPE(U32, uint32_t)
MESHMOD_ET_TYPE(U64, uint64_t)

MESHMOD_ET_TYPE(F, float)
MESHMOD_ET_TYPE(D, double)

MESHMOD_ET_VECTOR_TYPE(Vec2F)
MESHMOD_ET_VECTOR_TYPE(Vec3F)
MESHMOD_ET_VECTOR_TYPE(Vec4F)
MESHMOD_ET_VECTOR_TYPE(Vec2D)
MESHMOD_ET_VECTOR_TYPE(Vec3D)
MESHMOD_ET_VECTOR_TYPE(Vec4D)

#undef MESHMOD_ET_REAL
#undef MESHMOD_ET_VECTOR_REAL
