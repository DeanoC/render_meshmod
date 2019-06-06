#pragma once
#ifndef GFX_MESHMOD_DATAAABB_H
#define GFX_MESHMOD_DATAAABB_H

#include "al2o3_platform/platform.h"
#include "al2o3_cmath/aabb.h"

#define MeshMod_DataAabb2FTag MESHMOD_TAG('A', 'A', 'B', 'B', '2', 'F')
typedef Math_Aaab2F MeshMod_DataAabb2F;

#define MeshMod_DataAabb3FTag MESHMOD_TAG('A', 'A', 'B', 'B', '3', 'F')
typedef Math_Aaab3F MeshMod_DataAabb3F;

#endif