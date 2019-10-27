#pragma once

#include "al2o3_platform/platform.h"
#include "render_meshmod/meshmod.h"

#define MeshMod_EdgeRingTag MESHMOD_EDGETAG('E', 'R', 'I', 'N', 'G')
typedef struct MeshMod_EdgeRing {
	MeshMod_EdgeHandle next;
} MeshMod_EdgeRing;
