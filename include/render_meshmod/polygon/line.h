#pragma once
#include "al2o3_platform/platform.h"
#include "render_meshmod/meshmod.h"
#include "render_meshmod/mesh.h"

#define MeshMod_PolygonLineTag MESHMOD_POLYGONTAG('L', 'I', 'N', 'E', ' ')

typedef struct MeshMod_PolygonLine {
	MeshMod_EdgeHandle edge;
} MeshMod_PolygonLine;

AL2O3_FORCE_INLINE MeshMod_PolygonLine * MeshMod_MeshPolygonLineTagHandleToPtr(MeshMod_MeshHandle meshHandle, MeshMod_PolygonHandle polygonHandle, uint8_t userTag) {
	return  (MeshMod_PolygonLine *) MeshMod_MeshPolygonTagHandleToPtr(meshHandle, MeshMod_AddUserDataToPolygonTag(MeshMod_PolygonLineTag, userTag), polygonHandle);
}
