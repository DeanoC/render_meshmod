#pragma once

#include "al2o3_platform/platform.h"
#include "render_meshmod/meshmod.h"

#define MeshMod_PolygonRingTag MESHMOD_POLYGONTAG('P', 'R', 'I', 'N', 'G')
typedef struct MeshMod_PolygonRing {
	MeshMod_PolygonHandle next;
} MeshMod_PolygonRing;
