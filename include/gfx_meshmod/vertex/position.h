#pragma once
#ifndef GFX_MESHMOD_VERTEXPOSITION_H
#define GFX_MESHMOD_VERTEXPOSITION_H

#include "al2o3_platform/platform.h"
#include "al2o3_cmath/vector.h"

#define MeshMod_VertexPositionTag MESHMOD_VERTEXTAG('P', 'O', 'S', '3', 'F')

//! a position is a 3-tuple representing a physical position, its
//! a float per coordinate which should be enough for most applications
//! As position is a 'special' vertex attribute, if you require more
//! accuracy its probably a good idea to have a seperate more accurate
//! position attribute as well as the float position, cos currently the
//! internal system will often truncate to float anyway.
//! \todo make accuracy a compile time option
typedef Math_Vec3F_t MeshMod_VertexPosition;

#endif