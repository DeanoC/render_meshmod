#pragma once
#ifndef GFX_MESHMOD_POLYGONTRIBREP_H
#define GFX_MESHMOD_POLYGONTRIBREP_H

#include "al2o3_platform/platform.h"
#include "gfx_meshmod/meshmod.h"

#define MeshMod_PolygonTriBRepTag MESHMOD_POLYGONTAG('T', 'R', 'I', 'B', 'R')
typedef struct MeshMod_PolygonTriBRep {
	MeshMod_EdgeIndex edge[3];
} MeshMod_PolygonTriBRep;

#endif