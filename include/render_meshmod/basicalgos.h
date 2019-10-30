#pragma once

#include "al2o3_platform/platform.h"
#include "render_meshmod/meshmod.h"
#include "render_meshmod/mesh.h"
#include "render_meshmod/data/aabb.h"
#include "al2o3_cadt/vector.h"

AL2O3_EXTERN_C MeshMod_DataAabb2F const* MeshMod_MeshComputeExtents2F(MeshMod_MeshHandle handle, MeshMod_VertexTag tag);
AL2O3_EXTERN_C MeshMod_DataAabb3F const* MeshMod_MeshComputeExtents3F(MeshMod_MeshHandle handle, MeshMod_VertexTag tag);


// afterwards MeshMod_PolygonConvexBRep from tri and/or quad brep type
AL2O3_EXTERN_C void MeshMod_MeshToConvexBRep(MeshMod_MeshHandle handle);

// afterward MeshMod_PolygonTriBRep from quad or convex brep types
AL2O3_EXTERN_C void MeshMod_MeshTrianglate(MeshMod_MeshHandle handle);

typedef int (*MeshMod_VertexSortFunc)(MeshMod_MeshHandle handle, MeshMod_VertexHandle, MeshMod_VertexHandle);
typedef int (*MeshMod_EdgeSortFunc)(MeshMod_MeshHandle handle, MeshMod_EdgeHandle,MeshMod_EdgeHandle);
typedef int (*MeshMod_PolygonSortFunc)(MeshMod_MeshHandle handle, MeshMod_PolygonHandle,MeshMod_PolygonHandle);

// these sorts are currently single threaded, you can only sort 1 mesh at a time! caller has to destroy the vector returned
AL2O3_EXTERN_C CADT_VectorHandle MeshMod_MeshVertexTagSort(MeshMod_MeshHandle handle, MeshMod_VertexTag tag, MeshMod_VertexSortFunc sortFunc);
AL2O3_EXTERN_C CADT_VectorHandle MeshMod_MeshEdgeTagSort(MeshMod_MeshHandle handle, MeshMod_EdgeTag tag, MeshMod_EdgeSortFunc sortFunc);
AL2O3_EXTERN_C CADT_VectorHandle MeshMod_MeshPolygonTagSort(MeshMod_MeshHandle handle, MeshMod_PolygonTag tag, MeshMod_PolygonSortFunc sortFunc);

// Compute a similarity position ring in MeshMod_VertexSimilarTag user field 'P' (single threaded due to sort)
AL2O3_EXTERN_C void MeshMod_MeshComputeSimilarPositions(MeshMod_MeshHandle handle, float similarity);

AL2O3_EXTERN_C void MeshMod_MeshGenerateEdgeEndVertex(MeshMod_MeshHandle handle);

AL2O3_EXTERN_C void MeshMod_MeshComputeVertex2Edges(MeshMod_MeshHandle handle);

AL2O3_EXTERN_C void MeshMod_MeshComputeSimilarEdges(MeshMod_MeshHandle handle);
