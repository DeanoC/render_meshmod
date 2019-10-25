#pragma once

#include "al2o3_platform/platform.h"
#include "al2o3_cmath/vector.h"
#include "render_meshmod/meshmod.h"

// similar vertex attributes are a index ring defining similar (close to each other) over
// an attribute. Often called Point Rep for positionm they are transitory and need recomputing
// if any vertex change happens.
// makes certain spatial queryies fast and easy to reason about N dimenstional subset of a vertex
#define MeshMod_VertexSimilarTag MESHMOD_VERTEXTAG('S', 'I', 'M', 'L', 'R')
typedef MeshMod_VertexHandle MeshMod_VertexSimilar;
