#pragma once
#include "al2o3_platform/platform.h"
#include "render_meshmod/meshmod.h"
#include "render_meshmod/mesh.h"

#define MeshMod_PolygonQuadBRepTag MESHMOD_POLYGONTAG('Q', 'U', 'D', 'B', 'R')
typedef struct MeshMod_PolygonQuadBRep {
	MeshMod_EdgeHandle edge[4];
} MeshMod_PolygonQuadBRep;

AL2O3_FORCE_INLINE MeshMod_PolygonQuadBRep * MeshMod_MeshPolygonQuadBRepTagHandleToPtr(MeshMod_MeshHandle meshHandle, MeshMod_PolygonHandle polygonHandle, uint8_t userTag) {
	return (MeshMod_PolygonQuadBRep *) MeshMod_MeshPolygonTagHandleToPtr(meshHandle, MeshMod_AddUserDataToPolygonTag(MeshMod_PolygonQuadBRepTag, userTag), polygonHandle);
}