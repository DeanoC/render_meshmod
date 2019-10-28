#pragma once
#include "al2o3_platform/platform.h"
#include "render_meshmod/meshmod.h"
#include "render_meshmod/mesh.h"

#define MeshMod_PolygonTriBRepTag MESHMOD_POLYGONTAG('T', 'R', 'I', 'B', 'R')

typedef struct MeshMod_PolygonTriBRep {
	MeshMod_EdgeHandle edge[3];
} MeshMod_PolygonTriBRep;

AL2O3_FORCE_INLINE MeshMod_PolygonTriBRep * MeshMod_MeshPolygonTriBRepTagHandleToPtr(MeshMod_MeshHandle meshHandle, MeshMod_PolygonHandle polygonHandle) {
	return  (MeshMod_PolygonTriBRep *) MeshMod_MeshPolygonTagHandleToPtr(meshHandle, MeshMod_PolygonTriBRepTag, polygonHandle);
}
