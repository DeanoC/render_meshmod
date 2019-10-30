#include "al2o3_catch2/catch2.hpp"
#include "render_meshmod/registry.h"
#include "render_meshmod/mesh.h"
#include "render_meshmod/vertex/basicdata.h"
#include "render_meshmod/vertex/position.h"
#include "render_meshmod/vertex/similar.h"
#include "render_meshmod/vertex/vertex2edges.h"
#include "render_meshmod/edge/halfedge.h"
#include "render_meshmod/edge/endvertex.h"
#include "render_meshmod/edge/similar.h"
#include "render_meshmod/polygon/tribrep.h"
#include "render_meshmod/polygon/quadbrep.h"
#include "render_meshmod/polygon/convexbrep.h"
#include "render_meshmod/polygon/basicdata.h"
#include "render_meshmod/data/aabb.h"
#include "render_meshmod/basicalgos.h"
#include "render_meshmod/helpers.h"


static void AddTriAndQuadToMesh(MeshMod_MeshHandle handle) {
	REQUIRE(MeshMod_MeshHandleIsValid(handle));

	MeshMod_MeshVertexTagEnsure(handle, MeshMod_VertexPositionTag);
	MeshMod_MeshEdgeTagEnsure(handle, MeshMod_EdgeHalfEdgeTag);
	MeshMod_MeshPolygonTagEnsure(handle, MeshMod_PolygonTriBRepTag);
	MeshMod_MeshPolygonTagEnsure(handle, MeshMod_PolygonQuadBRepTag);
	MeshMod_MeshPolygonTagEnsure(handle, MeshMod_AddUserDataToPolygonTag(MeshMod_PolygonU8Tag, 0xF));

	// 3 vertices, 3 edges, 1 triangle AND
	// 4 vertices, 4 edges, 1 quad
	MeshMod_VertexHandle vhandles[7];

	vhandles[0] = MeshMod_MeshVertexAlloc(handle);
	vhandles[1] = MeshMod_MeshVertexAlloc(handle);
	vhandles[2] = MeshMod_MeshVertexAlloc(handle);
	vhandles[3] = MeshMod_MeshVertexAlloc(handle);
	vhandles[4] = MeshMod_MeshVertexAlloc(handle);
	vhandles[5] = MeshMod_MeshVertexAlloc(handle);
	vhandles[6] = MeshMod_MeshVertexAlloc(handle);

	MeshMod_EdgeHandle ehandles[7];
	ehandles[0] = MeshMod_MeshEdgeAlloc(handle);
	ehandles[1] = MeshMod_MeshEdgeAlloc(handle);
	ehandles[2] = MeshMod_MeshEdgeAlloc(handle);
	ehandles[3] = MeshMod_MeshEdgeAlloc(handle);
	ehandles[4] = MeshMod_MeshEdgeAlloc(handle);
	ehandles[5] = MeshMod_MeshEdgeAlloc(handle);
	ehandles[6] = MeshMod_MeshEdgeAlloc(handle);

	MeshMod_PolygonHandle phandles[2];
	phandles[0] = MeshMod_MeshPolygonAlloc(handle);
	phandles[1] = MeshMod_MeshPolygonAlloc(handle);

	auto pos0 = MeshMod_MeshVertexPositionTagHandleToPtr(handle,vhandles[0], 0);
	auto pos1 = MeshMod_MeshVertexPositionTagHandleToPtr(handle,vhandles[1], 0);
	auto pos2 = MeshMod_MeshVertexPositionTagHandleToPtr(handle,vhandles[2], 0);
	auto pos3 = MeshMod_MeshVertexPositionTagHandleToPtr(handle,vhandles[3], 0);
	auto pos4 = MeshMod_MeshVertexPositionTagHandleToPtr(handle,vhandles[4], 0);
	auto pos5 = MeshMod_MeshVertexPositionTagHandleToPtr(handle,vhandles[5], 0);
	auto pos6 = MeshMod_MeshVertexPositionTagHandleToPtr(handle,vhandles[6], 0);

	*pos0 = {0, 0, 0};
	*pos1 = {1, 0, 0};
	*pos2 = {1, 1, 0};
	*pos3 = {2, 0, 0};
	*pos4 = {3, 0, 0};
	*pos5 = {3, 1, 0};
	*pos6 = {2, 1, 0};

	auto edge0 = MeshMod_MeshEdgeHalfEdgeTagHandleToPtr(handle, ehandles[0], 0);
	auto edge1 = MeshMod_MeshEdgeHalfEdgeTagHandleToPtr(handle, ehandles[1], 0);
	auto edge2 = MeshMod_MeshEdgeHalfEdgeTagHandleToPtr(handle, ehandles[2], 0);
	auto edge3 = MeshMod_MeshEdgeHalfEdgeTagHandleToPtr(handle, ehandles[3], 0);
	auto edge4 = MeshMod_MeshEdgeHalfEdgeTagHandleToPtr(handle, ehandles[4], 0);
	auto edge5 = MeshMod_MeshEdgeHalfEdgeTagHandleToPtr(handle, ehandles[5], 0);
	auto edge6 = MeshMod_MeshEdgeHalfEdgeTagHandleToPtr(handle, ehandles[6], 0);

	edge0->vertex = vhandles[0];
	edge0->polygon = phandles[0];
	edge1->vertex = vhandles[1];
	edge1->polygon = phandles[0];
	edge2->vertex = vhandles[2];
	edge2->polygon = phandles[0];

	edge3->vertex = vhandles[3];
	edge3->polygon = phandles[1];
	edge4->vertex = vhandles[4];
	edge4->polygon = phandles[1];
	edge5->vertex = vhandles[5];
	edge5->polygon = phandles[1];
	edge6->vertex = vhandles[6];
	edge6->polygon = phandles[1];

	auto polygon0 =
			(MeshMod_PolygonTriBRep *) MeshMod_MeshPolygonTagHandleToPtr(handle, MeshMod_PolygonTriBRepTag, phandles[0]);
	polygon0->edge[0] = ehandles[0];
	polygon0->edge[1] = ehandles[1];
	polygon0->edge[2] = ehandles[2];
	auto polygon1 =
			(MeshMod_PolygonQuadBRep *) MeshMod_MeshPolygonTagHandleToPtr(handle, MeshMod_PolygonQuadBRepTag, phandles[1]);
	polygon1->edge[0] = ehandles[3];
	polygon1->edge[1] = ehandles[4];
	polygon1->edge[2] = ehandles[5];
	polygon1->edge[3] = ehandles[6];

	static MeshMod_PolygonTag const polyU8tag = MeshMod_AddUserDataToPolygonTag(MeshMod_PolygonU8Tag, 0xF);

	// mark in a custom U8 per polygon field a source number (to test post triangulation etc.
	auto polyUD0 = MeshMod_MeshPolygonU8TagHandleToPtr(handle, phandles[0], 0xF);
	*polyUD0 = 0;
	auto polyUD1 = MeshMod_MeshPolygonU8TagHandleToPtr(handle, phandles[1], 0xF);
	*polyUD1 = 1;
}

TEST_CASE("Mesh GetPolygonEdge", "[MeshMod Helpers]") {
	MeshMod_MeshHandle handle = MeshMod_MeshCreate({0}, "Tri and Quad");

	AddTriAndQuadToMesh(handle);

	MeshMod_MeshToConvexBRep(handle);

	MeshMod_PolygonHandle piterator = MeshMod_MeshPolygonIterate(handle, NULL);
	while (MeshMod_MeshPolygonIsValid(handle, piterator)) {

		auto poly = MeshMod_MeshPolygonConvexBRepTagHandleToPtr(handle, piterator, 0);
		auto polyUD = MeshMod_MeshPolygonU8TagHandleToPtr(handle, piterator, 0xF);
		REQUIRE(poly);
		REQUIRE(polyUD);

		MeshMod_EdgeHandle edges[16];
		uint8_t const numEdges = MeshMod_HelperGetPolygonEdges(handle, piterator, edges, 16);

		if (poly->numEdges == 3) {
			REQUIRE(numEdges == 3);
			REQUIRE(edges[0].handle.handle == (Handle_MaxHandles64+1));
			REQUIRE(edges[1].handle.handle == 1);
			REQUIRE(edges[2].handle.handle == 2);
		}

		if (poly->numEdges == 4) {
			REQUIRE(numEdges == 4);
			REQUIRE(edges[0].handle.handle == 3);
			REQUIRE(edges[1].handle.handle == 4);
			REQUIRE(edges[2].handle.handle == 5);
			REQUIRE(edges[3].handle.handle == 6);
		}

		piterator = MeshMod_MeshPolygonIterate(handle, &piterator);
	}

	MeshMod_MeshDestroy(handle);
}

TEST_CASE("Mesh GetSimilarVertexPosition", "[MeshMod Helpers]") {
	MeshMod_MeshHandle handle = MeshMod_MeshCreate({0}, "Tri and Quad");

	// do it twice so we have similar (same) vertices
	AddTriAndQuadToMesh(handle);
	AddTriAndQuadToMesh(handle);

	MeshMod_MeshComputeSimilarPositions(handle, 1e-5f);

	MeshMod_VertexHandle vertices[7*2];
	uint64_t vertexCount = 0;

	MeshMod_VertexHandle viterator = MeshMod_MeshVertexTagIterate(handle, MeshMod_VertexPositionTag, NULL);
	while (MeshMod_MeshVertexIsValid(handle, viterator)) {

		vertices[vertexCount++] = viterator;
		viterator = MeshMod_MeshVertexTagIterate(handle, MeshMod_VertexPositionTag, &viterator);
	}
	REQUIRE(vertexCount == 14);

	MeshMod_VertexHandle similarVertices[1];
	for(uint64_t i = 0; i < vertexCount/2;++i) {

		uint8_t const numSimilarVertices  = MeshMod_HelperGetSimilarVertexPosition(handle, vertices[i], similarVertices, 1);
		REQUIRE(numSimilarVertices == 1);
		REQUIRE(Handle_HandleEqual64(vertices[i+7].handle, similarVertices[0].handle));
	}

	MeshMod_MeshDestroy(handle);

}

TEST_CASE("Mesh AreVertexPositionsSimilar", "[MeshMod Helpers]") {
	MeshMod_MeshHandle handle = MeshMod_MeshCreate({0}, "Tri and Quad");

	// do it twice so we have similar (same) vertices
	AddTriAndQuadToMesh(handle);
	AddTriAndQuadToMesh(handle);

	MeshMod_MeshComputeSimilarPositions(handle, 1e-5f);

	MeshMod_VertexHandle vertices[7*2];
	uint64_t vertexCount = 0;

	MeshMod_VertexHandle viterator = MeshMod_MeshVertexTagIterate(handle, MeshMod_VertexPositionTag, NULL);
	while (MeshMod_MeshVertexIsValid(handle, viterator)) {

		vertices[vertexCount++] = viterator;
		viterator = MeshMod_MeshVertexTagIterate(handle, MeshMod_VertexPositionTag, &viterator);
	}
	REQUIRE(vertexCount == 14);

	for(uint64_t i = 0; i < vertexCount/2;++i) {
		bool const iSimilar = MeshMod_HelperAreVertexPositionsSimilar(handle, vertices[i], vertices[i + 7]);
		REQUIRE(iSimilar);
	}

	MeshMod_MeshDestroy(handle);

}


TEST_CASE("Mesh AreEdgesSimilar 2", "[MeshMod Helpers]") {
	MeshMod_MeshHandle handle = MeshMod_MeshCreate({0}, "Tri and Quad");

	// do it twice so we have similar (same) edges
	AddTriAndQuadToMesh(handle);
	AddTriAndQuadToMesh(handle);

	MeshMod_MeshComputeSimilarEdges(handle);

	MeshMod_EdgeHandle edges[7*2];
	uint64_t edgeCount = 0;

	MeshMod_EdgeHandle eiterator = MeshMod_MeshEdgeTagIterate(handle, MeshMod_EdgeHalfEdgeTag, NULL);
	while (MeshMod_MeshEdgeIsValid(handle, eiterator)) {

		edges[edgeCount++] = eiterator;
		eiterator = MeshMod_MeshEdgeTagIterate(handle, MeshMod_EdgeHalfEdgeTag, &eiterator);
	}
	REQUIRE(edgeCount == 14);

	for(uint64_t i = 0; i < edgeCount/2;++i) {
		bool const iSimilar = MeshMod_HelperAreEdgesSimilar(handle, edges[i], edges[i + 7]);
		REQUIRE(iSimilar);
	}

	MeshMod_MeshDestroy(handle);

}