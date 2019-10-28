#pragma once
#include "al2o3_platform/platform.h"
#include "render_meshmod/meshmod.h"
#include "al2o3_cmath/vector.h"
#include "render_meshmod/mesh.h"

#define MeshMod_PolygonPlaneEquationTag MESHMOD_POLYGONTAG('P', 'L', 'N', 'E', 'Q')
typedef struct MeshMod_PolygonPlaneEquation {
	Math_Vec4F planeEq;
} MeshMod_PolygonPlaneEquation;

AL2O3_FORCE_INLINE MeshMod_PolygonPlaneEquation * MeshMod_PolygonPlaneEquationTagHandleToPtr(MeshMod_MeshHandle meshHandle, MeshMod_PolygonHandle polygonHandle) {
	return (MeshMod_PolygonPlaneEquation *) MeshMod_MeshPolygonTagHandleToPtr(meshHandle,
																																						MeshMod_PolygonPlaneEquationTag,
																																			 polygonHandle);
}