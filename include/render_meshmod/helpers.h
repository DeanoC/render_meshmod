#pragma once

#include "al2o3_platform/platform.h"
#include "render_meshmod/meshmod.h"
#include "render_meshmod/mesh.h"

AL2O3_EXTERN_C uint8_t MeshMod_HelperGetPolygonEdges(MeshMod_MeshHandle handle, MeshMod_PolygonHandle phandle, MeshMod_EdgeHandle* outEdges, uint8_t maxOutEdges);
AL2O3_EXTERN_C uint8_t MeshMod_HelperGetSimilarVertexPosition(MeshMod_MeshHandle handle, MeshMod_VertexHandle vhandle, MeshMod_VertexHandle* outVertices, uint8_t maxOutVertices);
AL2O3_EXTERN_C uint8_t MeshMod_HelperGetSimilarEdges(MeshMod_MeshHandle handle, MeshMod_EdgeHandle vhandle, MeshMod_EdgeHandle* outEdges, uint8_t maxOutEdges);

AL2O3_EXTERN_C bool MeshMod_HelperAreVertexPositionsSimilar(MeshMod_MeshHandle handle, MeshMod_VertexHandle a, MeshMod_VertexHandle b);
AL2O3_EXTERN_C bool MeshMod_HelperAreEdgesSimilar(MeshMod_MeshHandle handle, MeshMod_EdgeHandle a, MeshMod_EdgeHandle b);