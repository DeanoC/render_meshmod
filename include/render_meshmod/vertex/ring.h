#pragma once

#include "al2o3_platform/platform.h"
#include "render_meshmod/meshmod.h"

#define MeshMod_VertexRingTag MESHMOD_VERTEXTAG('V', 'R', 'I', 'N', 'G')
typedef struct MeshMod_VertexRing {
	MeshMod_VertexHandle next;
} MeshMod_VertexRing;
