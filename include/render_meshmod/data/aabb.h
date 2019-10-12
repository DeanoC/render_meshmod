#pragma once
#ifndef GFX_MESHMOD_DATAAABB_H
#define GFX_MESHMOD_DATAAABB_H

#include "al2o3_platform/platform.h"
#include "al2o3_cmath/aabb.h"

typedef struct MathMod_DataAabb2F {
	uint64_t hash;
	MeshMod_Tag dataTag;
	Math_Aabb2F aabb;
} MeshMod_DataAabb2F;

typedef struct MathMod_DataAabb3F {
	uint64_t hash;
	MeshMod_Tag dataTag;
	Math_Aabb3F aabb;
} MeshMod_DataAabb3F;

#endif