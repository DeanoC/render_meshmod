#pragma once
#ifndef GFX_MESHMOD_BASICALGOS_H
#define GFX_MESHMOD_BASICALGOS_H

#include "al2o3_platform/platform.h"
#include "render_meshmod/meshmod.h"
#include "render_meshmod/mesh.h"
#include "render_meshmod/data/aabb.h"

AL2O3_EXTERN_C MeshMod_DataAabb2F const* MeshMod_MeshComputeExtents2F(MeshMod_MeshHandle handle, MeshMod_Tag tag);
AL2O3_EXTERN_C MeshMod_DataAabb3F const* MeshMod_MeshComputeExtents3F(MeshMod_MeshHandle handle, MeshMod_Tag tag);

#endif // GFX_MESHMOD_BASICALGOS_H