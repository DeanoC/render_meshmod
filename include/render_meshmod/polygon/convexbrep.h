#pragma once
#include "al2o3_platform/platform.h"
#include "render_meshmod/meshmod.h"
#include "render_meshmod/mesh.h"

#define MeshMod_PolygonConvexBRepTag MESHMOD_POLYGONTAG('C', 'V', 'X', 'B', 'R')
#define MeshMod_PolygonConvexMaxEdges 16

typedef struct MeshMod_PolygonConvexBRep {
	MeshMod_EdgeHandle edge[MeshMod_PolygonConvexMaxEdges];
	uint8_t numEdges;
} MeshMod_PolygonConvexBRep;

AL2O3_FORCE_INLINE MeshMod_PolygonConvexBRep * MeshMod_MeshPolygonConvexBRepTagHandleToPtr(MeshMod_MeshHandle meshHandle, MeshMod_PolygonHandle polygonHandle, uint8_t userTag) {
	return (MeshMod_PolygonConvexBRep *) MeshMod_MeshPolygonTagHandleToPtr(meshHandle, MeshMod_AddUserDataToPolygonTag(MeshMod_PolygonConvexBRepTag, userTag), polygonHandle);
}