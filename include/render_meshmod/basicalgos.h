#pragma once

#include "al2o3_platform/platform.h"
#include "render_meshmod/meshmod.h"
#include "render_meshmod/mesh.h"
#include "render_meshmod/data/aabb.h"

AL2O3_EXTERN_C MeshMod_DataAabb2F const* MeshMod_MeshComputeExtents2F(MeshMod_MeshHandle handle, MeshMod_VertexTag tag);
AL2O3_EXTERN_C MeshMod_DataAabb3F const* MeshMod_MeshComputeExtents3F(MeshMod_MeshHandle handle, MeshMod_VertexTag tag);


// afterwards MeshMod_PolygonConvexBRep from tri and/or quad brep type
AL2O3_EXTERN_C void MeshMod_MeshToConvexBRep(MeshMod_MeshHandle handle);

// afterward MeshMod_PolygonTriBRep from quad or convex brep types
AL2O3_EXTERN_C void MeshMod_MeshTrianglate(MeshMod_MeshHandle handle);
