#pragma once

#include "al2o3_platform/platform.h"
#include "render_meshmod/meshmod.h"
#include "render_meshmod/mesh.h"

#define MeshMod_PolygonRingTag MESHMOD_POLYGONTAG('P', 'R', 'I', 'N', 'G')
typedef struct MeshMod_PolygonRing {
	MeshMod_PolygonHandle next;
} MeshMod_PolygonRing;

AL2O3_FORCE_INLINE MeshMod_PolygonRing * MeshMod_PolygonRingTagHandleToPtr(MeshMod_MeshHandle meshHandle, MeshMod_PolygonHandle polygonHandle) {
	return  (MeshMod_PolygonRing *) MeshMod_MeshPolygonTagHandleToPtr(meshHandle, MeshMod_PolygonRingTag, polygonHandle);
}
