#pragma once
#include "al2o3_platform/platform.h"
#include "render_meshmod/meshmod.h"
#include "render_meshmod/mesh.h"

#define MeshMod_PolygonPointTag MESHMOD_POLYGONTAG('P', 'O', 'I', 'N', 'T')

typedef MeshMod_VertexHandle MeshMod_PolygonPoint;

AL2O3_FORCE_INLINE MeshMod_PolygonPoint * MeshMod_MeshPolygonPointTagHandleToPtr(MeshMod_MeshHandle meshHandle, MeshMod_PolygonHandle polygonHandle, uint8_t userTag) {
	return  (MeshMod_PolygonPoint *) MeshMod_MeshPolygonTagHandleToPtr(meshHandle, MeshMod_AddUserDataToPolygonTag(MeshMod_PolygonPointTag, userTag), polygonHandle);
}
