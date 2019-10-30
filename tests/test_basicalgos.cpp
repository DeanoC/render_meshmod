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


TEST_CASE("Mesh Position 3D Extents", "[MeshMod BasicAlgos]") {
	MeshMod_MeshHandle handle = MeshMod_MeshCreate({0}, "Test Mesh");
	REQUIRE(MeshMod_MeshHandleIsValid(handle));

	MeshMod_MeshVertexTagEnsure(handle, MeshMod_VertexPositionTag);

	Math_Vec3F tri[3] = {
			-1.0f, -2.0f, -3.0f,
			1.0f, 2.0f, 3.0f,
			-1.0f, 4.0f, -3.0f,
	};
	MeshMod_VertexHandle vhandle0 = MeshMod_MeshVertexAlloc(handle);
	MeshMod_VertexHandle vhandle1 = MeshMod_MeshVertexAlloc(handle);
	MeshMod_VertexHandle vhandle2 = MeshMod_MeshVertexAlloc(handle);

	auto pos0 = (Math_Vec3F *) MeshMod_MeshVertexTagHandleToPtr(handle, MeshMod_VertexPositionTag, vhandle0);
	auto pos1 = (Math_Vec3F *) MeshMod_MeshVertexTagHandleToPtr(handle, MeshMod_VertexPositionTag, vhandle1);
	auto pos2 = (Math_Vec3F *) MeshMod_MeshVertexTagHandleToPtr(handle, MeshMod_VertexPositionTag, vhandle2);
	*pos0 = tri[0];
	*pos1 = tri[1];
	*pos2 = tri[2];

	MeshMod_DataAabb3F const *aabb = MeshMod_MeshComputeExtents3F(handle, MeshMod_VertexPositionTag);
	REQUIRE(aabb->dataTag == (MeshMod_VertexTagToHashTag(MeshMod_VertexPositionTag) | 'A'));
	REQUIRE(aabb->hash != 0);
	REQUIRE(aabb->aabb.minExtent.x == Approx(-1.0f));
	REQUIRE(aabb->aabb.minExtent.y == Approx(-2.0f));
	REQUIRE(aabb->aabb.minExtent.z == Approx(-3.0f));
	REQUIRE(aabb->aabb.maxExtent.x == Approx(1.0f));
	REQUIRE(aabb->aabb.maxExtent.y == Approx(4.0f));
	REQUIRE(aabb->aabb.maxExtent.z == Approx(3.0f));
	uint64_t hash = aabb->hash;
	aabb = MeshMod_MeshComputeExtents3F(handle, MeshMod_VertexPositionTag);
	REQUIRE(aabb->hash == hash);

	Math_Vec3F tri2[3] = {
			-10.0f, -20.0f, -30.0f,
			10.0f, 2.0f, 30.0f,
			-10.0f, 4.0f, -30.0f,
	};

	MeshMod_VertexHandle vhandle3 = MeshMod_MeshVertexAlloc(handle);
	MeshMod_VertexHandle vhandle4 = MeshMod_MeshVertexAlloc(handle);
	MeshMod_VertexHandle vhandle5 = MeshMod_MeshVertexAlloc(handle);
	auto pos3 = (Math_Vec3F *) MeshMod_MeshVertexTagHandleToPtr(handle, MeshMod_VertexPositionTag, vhandle3);
	auto pos4 = (Math_Vec3F *) MeshMod_MeshVertexTagHandleToPtr(handle, MeshMod_VertexPositionTag, vhandle4);
	auto pos5 = (Math_Vec3F *) MeshMod_MeshVertexTagHandleToPtr(handle, MeshMod_VertexPositionTag, vhandle5);
	*pos3 = tri2[0];
	*pos4 = tri2[1];
	*pos5 = tri2[2];

	MeshMod_MeshTopologyHasChanged(handle);

	aabb = MeshMod_MeshComputeExtents3F(handle, MeshMod_VertexPositionTag);
	REQUIRE(aabb->dataTag == (MeshMod_VertexTagToHashTag(MeshMod_VertexPositionTag) | 'A'));
	REQUIRE(aabb->hash != hash);
	REQUIRE(aabb->aabb.minExtent.x == Approx(-10.0f));
	REQUIRE(aabb->aabb.minExtent.y == Approx(-20.0f));
	REQUIRE(aabb->aabb.minExtent.z == Approx(-30.0f));
	REQUIRE(aabb->aabb.maxExtent.x == Approx(10.0f));
	REQUIRE(aabb->aabb.maxExtent.y == Approx(4.0f));
	REQUIRE(aabb->aabb.maxExtent.z == Approx(30.0f));
	hash = aabb->hash;

	MeshMod_MeshVertexRelease(handle, vhandle3);
	MeshMod_MeshTopologyHasChanged(handle);
	aabb = MeshMod_MeshComputeExtents3F(handle, MeshMod_VertexPositionTag);
	REQUIRE(aabb->dataTag == (MeshMod_VertexTagToHashTag(MeshMod_VertexPositionTag) | 'A'));
	REQUIRE(aabb->hash != hash);
	REQUIRE(aabb->aabb.minExtent.x == Approx(-10.0f));
	REQUIRE(aabb->aabb.minExtent.y == Approx(-2.0f));
	REQUIRE(aabb->aabb.minExtent.z == Approx(-30.0f));
	REQUIRE(aabb->aabb.maxExtent.x == Approx(10.0f));
	REQUIRE(aabb->aabb.maxExtent.y == Approx(4.0f));
	REQUIRE(aabb->aabb.maxExtent.z == Approx(30.0f));

}

TEST_CASE("Mesh Position 2D Extents", "[MeshMod BasicAlgos]") {
	MeshMod_MeshHandle handle = MeshMod_MeshCreate({0}, "Test Mesh");
	REQUIRE(MeshMod_MeshHandleIsValid(handle));

	MeshMod_MeshVertexTagEnsure(handle, MeshMod_VertexVec2FTag);

	Math_Vec2F tri[3] = {
			-1.0f, -2.0f,
			1.0f, 2.0f,
			-1.0f, 4.0f,
	};
	MeshMod_VertexHandle vhandle0 = MeshMod_MeshVertexAlloc(handle);
	MeshMod_VertexHandle vhandle1 = MeshMod_MeshVertexAlloc(handle);
	MeshMod_VertexHandle vhandle2 = MeshMod_MeshVertexAlloc(handle);

	auto pos0 = (Math_Vec2F *) MeshMod_MeshVertexTagHandleToPtr(handle, MeshMod_VertexVec2FTag, vhandle0);
	auto pos1 = (Math_Vec2F *) MeshMod_MeshVertexTagHandleToPtr(handle, MeshMod_VertexVec2FTag, vhandle1);
	auto pos2 = (Math_Vec2F *) MeshMod_MeshVertexTagHandleToPtr(handle, MeshMod_VertexVec2FTag, vhandle2);
	*pos0 = tri[0];
	*pos1 = tri[1];
	*pos2 = tri[2];

	MeshMod_DataAabb2F const *aabb = MeshMod_MeshComputeExtents2F(handle, MeshMod_VertexVec2FTag);
	REQUIRE(aabb->dataTag == (MeshMod_VertexTagToHashTag(MeshMod_VertexVec2FTag) | 'A'));
	REQUIRE(aabb->hash != 0);
	REQUIRE(aabb->aabb.minExtent.x == Approx(-1.0f));
	REQUIRE(aabb->aabb.minExtent.y == Approx(-2.0f));
	REQUIRE(aabb->aabb.maxExtent.x == Approx(1.0f));
	REQUIRE(aabb->aabb.maxExtent.y == Approx(4.0f));
	uint64_t hash = aabb->hash;
	aabb = MeshMod_MeshComputeExtents2F(handle, MeshMod_VertexVec2FTag);
	REQUIRE(aabb->hash == hash);

	Math_Vec2F tri2[3] = {
			-10.0f, -20.0f,
			10.0f, 2.0f,
			-10.0f, 4.0f,
	};

	MeshMod_VertexHandle vhandle3 = MeshMod_MeshVertexAlloc(handle);
	MeshMod_VertexHandle vhandle4 = MeshMod_MeshVertexAlloc(handle);
	MeshMod_VertexHandle vhandle5 = MeshMod_MeshVertexAlloc(handle);
	auto pos3 = (Math_Vec2F *) MeshMod_MeshVertexTagHandleToPtr(handle, MeshMod_VertexVec2FTag, vhandle3);
	auto pos4 = (Math_Vec2F *) MeshMod_MeshVertexTagHandleToPtr(handle, MeshMod_VertexVec2FTag, vhandle4);
	auto pos5 = (Math_Vec2F *) MeshMod_MeshVertexTagHandleToPtr(handle, MeshMod_VertexVec2FTag, vhandle5);
	*pos3 = tri2[0];
	*pos4 = tri2[1];
	*pos5 = tri2[2];

	MeshMod_MeshTopologyHasChanged(handle);

	aabb = MeshMod_MeshComputeExtents2F(handle, MeshMod_VertexVec2FTag);
	REQUIRE(aabb->dataTag == (MeshMod_VertexTagToHashTag(MeshMod_VertexVec2FTag) | 'A'));
	REQUIRE(aabb->hash != hash);
	REQUIRE(aabb->aabb.minExtent.x == Approx(-10.0f));
	REQUIRE(aabb->aabb.minExtent.y == Approx(-20.0f));
	REQUIRE(aabb->aabb.maxExtent.x == Approx(10.0f));
	REQUIRE(aabb->aabb.maxExtent.y == Approx(4.0f));
	hash = aabb->hash;

	MeshMod_MeshVertexRelease(handle, vhandle3);
	MeshMod_MeshTopologyHasChanged(handle);
	aabb = MeshMod_MeshComputeExtents2F(handle, MeshMod_VertexVec2FTag);
	REQUIRE(aabb->dataTag == (MeshMod_VertexTagToHashTag(MeshMod_VertexVec2FTag) | 'A'));
	REQUIRE(aabb->hash != hash);
	REQUIRE(aabb->aabb.minExtent.x == Approx(-10.0f));
	REQUIRE(aabb->aabb.minExtent.y == Approx(-2.0f));
	REQUIRE(aabb->aabb.maxExtent.x == Approx(10.0f));
	REQUIRE(aabb->aabb.maxExtent.y == Approx(4.0f));

}

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

static void Add2ConvexPolygons(MeshMod_MeshHandle handle) {
	MeshMod_MeshVertexTagEnsure(handle, MeshMod_VertexPositionTag);
	MeshMod_MeshEdgeTagEnsure(handle, MeshMod_EdgeHalfEdgeTag);
	MeshMod_MeshPolygonTagEnsure(handle, MeshMod_PolygonConvexBRepTag);
	MeshMod_MeshPolygonTagEnsure(handle, MeshMod_AddUserDataToPolygonTag(MeshMod_PolygonU8Tag, 0xF));

	// 5 vertices, 5 edges, 1 convex polygon
	MeshMod_VertexHandle vhandles[5];

	vhandles[0] = MeshMod_MeshVertexAlloc(handle);
	vhandles[1] = MeshMod_MeshVertexAlloc(handle);
	vhandles[2] = MeshMod_MeshVertexAlloc(handle);
	vhandles[3] = MeshMod_MeshVertexAlloc(handle);
	vhandles[4] = MeshMod_MeshVertexAlloc(handle);
	MeshMod_EdgeHandle ehandles[5];
	ehandles[0] = MeshMod_MeshEdgeAlloc(handle);
	ehandles[1] = MeshMod_MeshEdgeAlloc(handle);
	ehandles[2] = MeshMod_MeshEdgeAlloc(handle);
	ehandles[3] = MeshMod_MeshEdgeAlloc(handle);
	ehandles[4] = MeshMod_MeshEdgeAlloc(handle);
	MeshMod_PolygonHandle phandles[1];
	phandles[0] = MeshMod_MeshPolygonAlloc(handle);

	REQUIRE(MeshMod_MeshVertexIsValid(handle, vhandles[0]));
	REQUIRE(MeshMod_MeshVertexIsValid(handle, vhandles[1]));
	REQUIRE(MeshMod_MeshVertexIsValid(handle, vhandles[2]));
	REQUIRE(MeshMod_MeshVertexIsValid(handle, vhandles[3]));
	REQUIRE(MeshMod_MeshVertexIsValid(handle, vhandles[4]));
	REQUIRE(MeshMod_MeshEdgeIsValid(handle, ehandles[0]));
	REQUIRE(MeshMod_MeshEdgeIsValid(handle, ehandles[1]));
	REQUIRE(MeshMod_MeshEdgeIsValid(handle, ehandles[2]));
	REQUIRE(MeshMod_MeshEdgeIsValid(handle, ehandles[3]));
	REQUIRE(MeshMod_MeshEdgeIsValid(handle, ehandles[4]));
	REQUIRE(MeshMod_MeshPolygonIsValid(handle, phandles[0]));

	auto pos0 =
			(MeshMod_VertexPosition *) MeshMod_MeshVertexTagHandleToPtr(handle, MeshMod_VertexPositionTag, vhandles[0]);
	auto pos1 =
			(MeshMod_VertexPosition *) MeshMod_MeshVertexTagHandleToPtr(handle, MeshMod_VertexPositionTag, vhandles[1]);
	auto pos2 =
			(MeshMod_VertexPosition *) MeshMod_MeshVertexTagHandleToPtr(handle, MeshMod_VertexPositionTag, vhandles[2]);
	auto pos3 =
			(MeshMod_VertexPosition *) MeshMod_MeshVertexTagHandleToPtr(handle, MeshMod_VertexPositionTag, vhandles[3]);
	auto pos4 =
			(MeshMod_VertexPosition *) MeshMod_MeshVertexTagHandleToPtr(handle, MeshMod_VertexPositionTag, vhandles[4]);
	REQUIRE(pos0);
	REQUIRE(pos1);
	REQUIRE(pos2);
	REQUIRE(pos3);
	REQUIRE(pos4);
	REQUIRE(Math_IsNanVec3F(*pos0));
	REQUIRE(Math_IsNanVec3F(*pos1));
	REQUIRE(Math_IsNanVec3F(*pos2));
	REQUIRE(Math_IsNanVec3F(*pos3));
	REQUIRE(Math_IsNanVec3F(*pos4));
	*pos0 = {0, 0, 0};
	*pos1 = {1, 0, 0};
	*pos2 = {1, 1, 0};
	*pos3 = {0, 1, 0};
	*pos4 = {-0.5f, 0.5f, 0};
	auto edge0 = (MeshMod_EdgeHalfEdge *) MeshMod_MeshEdgeTagHandleToPtr(handle, MeshMod_EdgeHalfEdgeTag, ehandles[0]);
	auto edge1 = (MeshMod_EdgeHalfEdge *) MeshMod_MeshEdgeTagHandleToPtr(handle, MeshMod_EdgeHalfEdgeTag, ehandles[1]);
	auto edge2 = (MeshMod_EdgeHalfEdge *) MeshMod_MeshEdgeTagHandleToPtr(handle, MeshMod_EdgeHalfEdgeTag, ehandles[2]);
	auto edge3 = (MeshMod_EdgeHalfEdge *) MeshMod_MeshEdgeTagHandleToPtr(handle, MeshMod_EdgeHalfEdgeTag, ehandles[3]);
	auto edge4 = (MeshMod_EdgeHalfEdge *) MeshMod_MeshEdgeTagHandleToPtr(handle, MeshMod_EdgeHalfEdgeTag, ehandles[4]);
	edge0->vertex = vhandles[0];
	edge0->polygon = phandles[0];
	edge1->vertex = vhandles[1];
	edge1->polygon = phandles[0];
	edge2->vertex = vhandles[2];
	edge2->polygon = phandles[0];
	edge3->vertex = vhandles[3];
	edge3->polygon = phandles[0];
	edge4->vertex = vhandles[4];
	edge4->polygon = phandles[0];

	auto polygon0 = MeshMod_MeshPolygonConvexBRepTagHandleToPtr(handle, phandles[0], 0);
	polygon0->numEdges = 5;
	polygon0->edge[0] = ehandles[0];
	polygon0->edge[1] = ehandles[1];
	polygon0->edge[2] = ehandles[2];
	polygon0->edge[3] = ehandles[3];
	polygon0->edge[4] = ehandles[4];
	auto polyUD0 = MeshMod_MeshPolygonU8TagHandleToPtr(handle, phandles[0], 0xF);
	*polyUD0 = 0;

	uint64_t polyCount = 0;
	MeshMod_PolygonHandle piterator = MeshMod_MeshPolygonIterate(handle, NULL);
	while (MeshMod_MeshPolygonIsValid(handle, piterator)) {

		auto poly = MeshMod_MeshPolygonConvexBRepTagHandleToPtr(handle, piterator, 0);

		REQUIRE(poly);
		REQUIRE(poly->numEdges == 5);
		REQUIRE(poly->edge[0].handle.handle == ehandles[0].handle.handle);
		REQUIRE(poly->edge[1].handle.handle == ehandles[1].handle.handle);
		REQUIRE(poly->edge[2].handle.handle == ehandles[2].handle.handle);
		REQUIRE(poly->edge[3].handle.handle == ehandles[3].handle.handle);
		REQUIRE(poly->edge[4].handle.handle == ehandles[4].handle.handle);
		polyCount++;
		piterator = MeshMod_MeshPolygonIterate(handle, &piterator);
	}
	REQUIRE(polyCount == 1);

	MeshMod_EdgeHandle eiterator = MeshMod_MeshEdgeIterate(handle, NULL);
	uint64_t edgeCount = 0;
	while (MeshMod_MeshEdgeIsValid(handle, eiterator)) {

		auto edge = MeshMod_MeshEdgeHalfEdgeTagHandleToPtr(handle,eiterator, 0);
		REQUIRE(edge);
		REQUIRE(edge->polygon.handle.handle == phandles[0].handle.handle);
		REQUIRE(edge->vertex.handle.handle == vhandles[edgeCount].handle.handle);
		edgeCount++;
		eiterator = MeshMod_MeshEdgeIterate(handle, &eiterator);
	}
	REQUIRE(edgeCount == 5);

	MeshMod_PolygonHandle phandleCopy = MeshMod_MeshPolygonDuplicate(handle, phandles[0]);
	auto polyUD1 = MeshMod_MeshPolygonU8TagHandleToPtr(handle, phandleCopy, 0xF);
	*polyUD1 = 1;
	polyCount = 0;
	piterator = MeshMod_MeshPolygonIterate(handle, NULL);
	while (MeshMod_MeshPolygonIsValid(handle, piterator)) {

		auto poly = MeshMod_MeshPolygonConvexBRepTagHandleToPtr(handle, piterator, 0);
		REQUIRE(poly);
		REQUIRE(poly->numEdges == 5);
		REQUIRE(poly->edge[0].handle.handle == ehandles[0].handle.handle);
		REQUIRE(poly->edge[1].handle.handle == ehandles[1].handle.handle);
		REQUIRE(poly->edge[2].handle.handle == ehandles[2].handle.handle);
		REQUIRE(poly->edge[3].handle.handle == ehandles[3].handle.handle);
		REQUIRE(poly->edge[4].handle.handle == ehandles[4].handle.handle);
		polyCount++;
		piterator = MeshMod_MeshPolygonIterate(handle, &piterator);
	}
	REQUIRE(polyCount == 2);
}

TEST_CASE("Mesh tri and quad to convex polygons", "[MeshMod BasicAlgos]") {
	MeshMod_MeshHandle handle = MeshMod_MeshCreate({0}, "Tri and Quad");

	AddTriAndQuadToMesh(handle);

	MeshMod_MeshToConvexBRep(handle);

	uint64_t polyCount = 0;
	uint64_t triCount = 0;
	uint64_t quadCount = 0;
	MeshMod_PolygonHandle piterator = MeshMod_MeshPolygonIterate(handle, NULL);
	while (MeshMod_MeshPolygonIsValid(handle, piterator)) {

		auto poly = MeshMod_MeshPolygonConvexBRepTagHandleToPtr(handle, piterator, 0);
		auto polyUD = MeshMod_MeshPolygonU8TagHandleToPtr(handle, piterator, 0xF);
		REQUIRE(poly);
		REQUIRE(polyUD);

		if (poly->numEdges == 3) {
			REQUIRE(*polyUD == 0);
			triCount++;
		}
		if (poly->numEdges == 4) {
			REQUIRE(*polyUD == 1);
			quadCount++;
		}
		polyCount++;
		piterator = MeshMod_MeshPolygonIterate(handle, &piterator);
	}
	REQUIRE(triCount == 1);
	REQUIRE(quadCount == 1);
	REQUIRE(polyCount == 2);
	MeshMod_MeshDestroy(handle);
}

TEST_CASE("Mesh tri and quad triangulate", "[MeshMod BasicAlgos]") {
	MeshMod_MeshHandle handle = MeshMod_MeshCreate({0}, "Tri and Quad");

	AddTriAndQuadToMesh(handle);

	MeshMod_MeshTrianglate(handle);

	REQUIRE_FALSE(MeshMod_MeshPolygonTagExists(handle, MeshMod_PolygonQuadBRepTag));
	REQUIRE_FALSE(MeshMod_MeshPolygonTagExists(handle, MeshMod_PolygonConvexBRepTag));

	uint64_t triCount = 0;
	MeshMod_PolygonHandle piterator = MeshMod_MeshPolygonIterate(handle, NULL);
	while (MeshMod_MeshPolygonIsValid(handle, piterator)) {

		auto tri = MeshMod_MeshPolygonTriBRepTagHandleToPtr(handle, piterator, 0);
		auto triUD = MeshMod_MeshPolygonU8TagHandleToPtr(handle, piterator, 0xF);
		REQUIRE(tri);
		REQUIRE(triUD);

		if (triCount == 0) {
			REQUIRE(tri->edge[0].handle.handle == 0x010000000000);
			REQUIRE(tri->edge[1].handle.handle == 1);
			REQUIRE(tri->edge[2].handle.handle == 2);
			REQUIRE(*triUD == 0);
		} else if (triCount == 1) {
			REQUIRE(tri->edge[0].handle.handle == 3);
			REQUIRE(tri->edge[1].handle.handle == 4);
			REQUIRE(tri->edge[2].handle.handle == 5);
			REQUIRE(*triUD == 1);
		} else if (triCount == 2) {
			REQUIRE(tri->edge[0].handle.handle == 7);
			REQUIRE(tri->edge[1].handle.handle == 6);
			REQUIRE(tri->edge[2].handle.handle == 8);
			REQUIRE(*triUD == 1);
		}
		triCount++;
		piterator = MeshMod_MeshPolygonIterate(handle, &piterator);
	}
	REQUIRE(triCount == 3);
	MeshMod_MeshDestroy(handle);
}

TEST_CASE("Mesh 2 convex to triangles", "[MeshMod BasicAlgos]") {
	MeshMod_MeshHandle handle = MeshMod_MeshCreate({0}, "2 convex");

	Add2ConvexPolygons(handle);

	MeshMod_MeshTrianglate(handle);

	uint64_t polyCount = 0;
	uint64_t triCount = 0;
	uint64_t quadCount = 0;
	MeshMod_PolygonHandle piterator = MeshMod_MeshPolygonTagIterate(handle, MeshMod_PolygonTriBRepTag, NULL);
	while (MeshMod_MeshPolygonIsValid(handle, piterator)) {

		auto poly = MeshMod_MeshPolygonTriBRepTagHandleToPtr(handle, piterator, 0);
		auto polyUD = MeshMod_MeshPolygonU8TagHandleToPtr(handle, piterator, 0xF);
		REQUIRE(poly);
		REQUIRE(polyUD);
		REQUIRE(((*polyUD == 0) || (*polyUD == 1)));

		triCount++;
		piterator = MeshMod_MeshPolygonTagIterate(handle, MeshMod_PolygonTriBRepTag, &piterator);
	}
	REQUIRE(triCount == 6);

	MeshMod_MeshDestroy(handle);
}

// sort biggest x first index, biggest y second
static int VertexPositionXSort(MeshMod_MeshHandle handle, MeshMod_VertexHandle ah, MeshMod_VertexHandle bh) {
	auto const a = MeshMod_MeshVertexPositionTagHandleToPtr(handle, ah, 0);
	auto const b = MeshMod_MeshVertexPositionTagHandleToPtr(handle, bh, 0);

	int const xd = (int) ((b->x * 10000.0f) - (a->x * 10000.0f));
	int const yd = (int) ((b->y * 10000.0f) - (a->y * 10000.0f));

	if (xd == 0) {
		return yd;
	}
	return xd;
}

TEST_CASE("Mesh sort vertex", "[MeshMod BasicAlgos]") {
	MeshMod_MeshHandle handle = MeshMod_MeshCreate({0}, "Tri and Quad");

	AddTriAndQuadToMesh(handle);

	CADT_VectorHandle sortMap = MeshMod_MeshVertexTagSort(handle, MeshMod_VertexPositionTag, &VertexPositionXSort);
	REQUIRE(CADT_VectorSize(sortMap) == 7);
	for (size_t i = 0; i < CADT_VectorSize(sortMap); ++i) {
		MeshMod_VertexHandle vh = *(MeshMod_VertexHandle *) CADT_VectorAt(sortMap, i);
		auto v = MeshMod_MeshVertexPositionTagHandleToPtr(handle,vh, 0);
		switch (i) {
			case 0: {
				REQUIRE(v->x == Approx(3.0f));
				REQUIRE(v->y == Approx(1.0f));
				REQUIRE(v->z == Approx(0.0f));
				break;
			}
			case 1: {
				REQUIRE(v->x == Approx(3.0f));
				REQUIRE(v->y == Approx(0.0f));
				REQUIRE(v->z == Approx(0.0f));
				break;
			}
			case 2: {
				REQUIRE(v->x == Approx(2.0f));
				REQUIRE(v->y == Approx(1.0f));
				REQUIRE(v->z == Approx(0.0f));
				break;
			}
			case 3: {
				REQUIRE(v->x == Approx(2.0f));
				REQUIRE(v->y == Approx(0.0f));
				REQUIRE(v->z == Approx(0.0f));
				break;
			}
			case 4: {
				REQUIRE(v->x == Approx(1.0f));
				REQUIRE(v->y == Approx(1.0f));
				REQUIRE(v->z == Approx(0.0f));
				break;
			}
			case 5: {
				REQUIRE(v->x == Approx(1.0f));
				REQUIRE(v->y == Approx(0.0f));
				REQUIRE(v->z == Approx(0.0f));
				break;
			}
			case 6: {
				REQUIRE(v->x == Approx(0.0f));
				REQUIRE(v->y == Approx(0.0f));
				REQUIRE(v->z == Approx(0.0f));
				break;
			}
			default: break;
		}
	}
	MeshMod_MeshDestroy(handle);
}

TEST_CASE("Mesh similar position compute", "[MeshMod BasicAlgos]") {
	MeshMod_MeshHandle handle = MeshMod_MeshCreate({0}, "Tri and Quad");

//	*pos0 = {0, 0, 0};
//	*pos1 = {1, 0, 0};
//	*pos2 = {1, 1, 0};
//	*pos3 = {2, 0, 0};
//	*pos4 = {3, 0, 0};
//	*pos5 = {3, 1, 0};
//	*pos6 = {2, 1, 0};
	AddTriAndQuadToMesh(handle);

	MeshMod_VertexHandle new7 = MeshMod_MeshVertexAlloc(handle);
	MeshMod_VertexHandle new8 = MeshMod_MeshVertexAlloc(handle);
	MeshMod_VertexHandle new9 = MeshMod_MeshVertexAlloc(handle);
	MeshMod_VertexHandle new10 = MeshMod_MeshVertexAlloc(handle);
	auto pos7 = MeshMod_MeshVertexPositionTagHandleToPtr(handle, new7, 0);
	auto pos8 = MeshMod_MeshVertexPositionTagHandleToPtr(handle, new8, 0);
	auto pos9 = MeshMod_MeshVertexPositionTagHandleToPtr(handle, new9, 0);
	auto pos10 = MeshMod_MeshVertexPositionTagHandleToPtr(handle, new10, 0);
	*pos7 = { 0, 0, 0 };
	*pos8 = { 2.001f, 1, 0 };
	*pos9 = { 2.01f, 1, 2 };
	*pos10 = { 2.01f, 1.01f, 0 };

	MeshMod_MeshComputeSimilarPositions(handle, 0.1f);

	MeshMod_VertexHandle viterator = MeshMod_MeshVertexTagIterate(handle, MeshMod_VertexPositionTag, NULL);
	size_t vertexCount = 0;
	MeshMod_VertexHandle handle0 = {0};
	MeshMod_VertexHandle handle6 = {0};
	while (MeshMod_MeshVertexIsValid(handle, viterator)) {

		auto v = MeshMod_MeshVertexPositionTagHandleToPtr(handle,viterator, 0);
		auto s = MeshMod_MeshVertexSimilarTagHandleToPtr(handle, viterator, 'P');

		switch (vertexCount) {
			case 0: {
				REQUIRE(v->x == Approx(0.0f));
				REQUIRE(v->y == Approx(0.0f));
				REQUIRE(v->z == Approx(0.0f));
				REQUIRE(s->handle.handle != viterator.handle.handle);
				handle0 = viterator;
				REQUIRE(s->handle.handle == new7.handle.handle);
				REQUIRE(MeshMod_HelperAreVertexPositionsSimilar(handle, viterator, new7));
				break;
			}
			case 1: {
				REQUIRE(v->x == Approx(1.0f));
				REQUIRE(v->y == Approx(0.0f));
				REQUIRE(v->z == Approx(0.0f));
				REQUIRE(s->handle.handle == viterator.handle.handle);
				REQUIRE(!MeshMod_HelperAreVertexPositionsSimilar(handle, viterator, new7));
				break;
			}
			case 2: {
				REQUIRE(v->x == Approx(1.0f));
				REQUIRE(v->y == Approx(1.0f));
				REQUIRE(v->z == Approx(0.0f));
				REQUIRE(s->handle.handle == viterator.handle.handle);
				break;
			}
			case 3: {
				REQUIRE(v->x == Approx(2.0f));
				REQUIRE(v->y == Approx(0.0f));
				REQUIRE(v->z == Approx(0.0f));
				REQUIRE(s->handle.handle == viterator.handle.handle);
				break;
			}
			case 4: {
				REQUIRE(v->x == Approx(3.0f));
				REQUIRE(v->y == Approx(0.0f));
				REQUIRE(v->z == Approx(0.0f));
				REQUIRE(s->handle.handle == viterator.handle.handle);
				break;
			}
			case 5: {
				REQUIRE(v->x == Approx(3.0f));
				REQUIRE(v->y == Approx(1.0f));
				REQUIRE(v->z == Approx(0.0f));
				REQUIRE(s->handle.handle == viterator.handle.handle);
				break;
			}
			case 6: {
				REQUIRE(v->x == Approx(2.0f));
				REQUIRE(v->y == Approx(1.0f));
				REQUIRE(v->z == Approx(0.0f));
				REQUIRE(s->handle.handle == new10.handle.handle);
				handle6 = viterator;
				REQUIRE(MeshMod_HelperAreVertexPositionsSimilar(handle, viterator, new10));
				REQUIRE(MeshMod_HelperAreVertexPositionsSimilar(handle, viterator, new8));
				break;
			}
			case 7: {
				REQUIRE(v->x == Approx(0.0f));
				REQUIRE(v->y == Approx(0.0f));
				REQUIRE(v->z == Approx(0.0f));
				REQUIRE(s->handle.handle == handle0.handle.handle);
				break;
			}
			case 8: {
				REQUIRE(v->x == Approx(2.001f));
				REQUIRE(v->y == Approx(1.0f));
				REQUIRE(v->z == Approx(0.0f));
				REQUIRE(s->handle.handle == handle6.handle.handle);
				break;
			}
			case 9: {
				REQUIRE(v->x == Approx(2.01f));
				REQUIRE(v->y == Approx(1.0f));
				REQUIRE(v->z == Approx(2.0f));
				REQUIRE(s->handle.handle == viterator.handle.handle);
				break;
			}
			case 10: {
				REQUIRE(v->x == Approx(2.01f));
				REQUIRE(v->y == Approx(1.01f));
				REQUIRE(v->z == Approx(0.0f));
				REQUIRE(s->handle.handle == new8.handle.handle);
				break;
			}

			default: break;
		}
		vertexCount++;
		viterator = MeshMod_MeshVertexTagIterate(handle, MeshMod_VertexPositionTag, &viterator);
	}

	MeshMod_MeshDestroy(handle);
}

TEST_CASE("Mesh vertex 2 edge compute", "[MeshMod BasicAlgos]") {
	MeshMod_MeshHandle handle = MeshMod_MeshCreate({0}, "Tri and Quad");

	AddTriAndQuadToMesh(handle);

	MeshMod_MeshComputeVertex2Edges(handle);

	MeshMod_VertexHandle viterator = MeshMod_MeshVertexTagIterate(handle, MeshMod_VertexPositionTag, NULL);
	size_t vertexCount = 0;
	while (MeshMod_MeshVertexIsValid(handle, viterator)) {

		auto v2e = MeshMod_MeshVertex2EdgesTagHandleToPtr(handle,viterator, 0);

		switch (vertexCount) {
			case 0: {
				REQUIRE(v2e->numEdges == 2);
				REQUIRE(v2e->edges[0].handle.handle == 2); // incoming
				REQUIRE(v2e->edges[1].handle.handle == (Handle_MaxHandles64 + 1)); // zero is a weird case... out
				break;
			}
			case 1: {
				REQUIRE(v2e->numEdges == 2);
				REQUIRE(v2e->edges[0].handle.handle == (Handle_MaxHandles64 + 1)); // zero is a weird case... in
				REQUIRE(v2e->edges[1].handle.handle == 1); // out
				break;
			}
			case 2: {
				REQUIRE(v2e->numEdges == 2);
				REQUIRE(v2e->edges[0].handle.handle == 1); // in
				REQUIRE(v2e->edges[1].handle.handle == 2); // out
				break;
			}
			case 3: {
				REQUIRE(v2e->numEdges == 2);
				REQUIRE(v2e->edges[0].handle.handle == 6); // in
				REQUIRE(v2e->edges[1].handle.handle == 3); // out
				break;
			}
			case 4: {
				REQUIRE(v2e->numEdges == 2);
				REQUIRE(v2e->edges[0].handle.handle == 3); // in
				REQUIRE(v2e->edges[1].handle.handle == 4); // out
				break;
			}
			case 5: {
				REQUIRE(v2e->numEdges == 2);
				REQUIRE(v2e->edges[0].handle.handle == 4); // in
				REQUIRE(v2e->edges[1].handle.handle == 5); // out
				break;
			}
			case 6: {
				REQUIRE(v2e->numEdges == 2);
				REQUIRE(v2e->edges[0].handle.handle == 5); // in
				REQUIRE(v2e->edges[1].handle.handle == 6); // out
				break;
			}

			default: break;
		}
		vertexCount++;
		viterator = MeshMod_MeshVertexTagIterate(handle, MeshMod_VertexPositionTag, &viterator);
	}

	MeshMod_MeshDestroy(handle);
}

TEST_CASE("Mesh generate edge end vertex", "[MeshMod BasicAlgos]") {
	MeshMod_MeshHandle handle = MeshMod_MeshCreate({0}, "Tri and Quad");

	AddTriAndQuadToMesh(handle);

	MeshMod_MeshGenerateEdgeEndVertex(handle);

	MeshMod_EdgeHandle eiterator = MeshMod_MeshEdgeTagIterate(handle, MeshMod_EdgeEndVertexTag, NULL);
	size_t edgeCount = 0;
	while (MeshMod_MeshEdgeIsValid(handle, eiterator)) {

		auto endvertex = MeshMod_MeshEdgeEndVertexTagHandleToPtr(handle,eiterator, 0);

		switch (edgeCount) {
			case 0: {
				REQUIRE(endvertex->handle.handle == 1);
				break;
			}
			case 1: {
				REQUIRE(endvertex->handle.handle == 2);
				break;
			}
			case 2: {
				REQUIRE(endvertex->handle.handle == (Handle_MaxHandles64+1));
				break;
			}
			case 3: {
				REQUIRE(endvertex->handle.handle == 4);
				break;
			}
			case 4: {
				REQUIRE(endvertex->handle.handle == 5);
				break;
			}
			case 5: {
				REQUIRE(endvertex->handle.handle == 6);
				break;
			}
			case 6: {
				REQUIRE(endvertex->handle.handle == 3);
				break;
			}

			default: break;
		}
		edgeCount++;
		eiterator = MeshMod_MeshEdgeTagIterate(handle, MeshMod_EdgeEndVertexTag, &eiterator);
	}

	MeshMod_MeshDestroy(handle);
}

TEST_CASE("Mesh similar edge compute 1", "[MeshMod BasicAlgos]") {
	MeshMod_MeshHandle handle = MeshMod_MeshCreate({0}, "Tri and Quad");

	AddTriAndQuadToMesh(handle);

	MeshMod_MeshComputeSimilarEdges(handle);

	MeshMod_EdgeHandle eiterator = MeshMod_MeshEdgeTagIterate(handle, MeshMod_EdgeEndVertexTag, NULL);
	while (MeshMod_MeshEdgeIsValid(handle, eiterator)) {

		auto similarEdges = MeshMod_MeshEdgeSimilarTagHandleToPtr(handle,eiterator, 0);
		REQUIRE(similarEdges->handle.handle == eiterator.handle.handle);
		eiterator = MeshMod_MeshEdgeTagIterate(handle, MeshMod_EdgeEndVertexTag, &eiterator);
	}
	MeshMod_MeshDestroy(handle);
}


TEST_CASE("Mesh similar edge compute 2", "[MeshMod BasicAlgos]") {
	MeshMod_MeshHandle handle = MeshMod_MeshCreate({0}, "Tri and Quad");

	// add twice for similarity checks
	AddTriAndQuadToMesh(handle);
	AddTriAndQuadToMesh(handle);

	MeshMod_MeshComputeSimilarEdges(handle);

	MeshMod_EdgeHandle edgeHandles[7 * 2];

	MeshMod_EdgeHandle eiterator = MeshMod_MeshEdgeTagIterate(handle, MeshMod_EdgeEndVertexTag, NULL);
	size_t edgeCount = 0;
	while (MeshMod_MeshEdgeIsValid(handle, eiterator)) {

		auto similarEdges = MeshMod_MeshEdgeSimilarTagHandleToPtr(handle,eiterator, 0);
		REQUIRE(similarEdges->handle.handle != eiterator.handle.handle);
		edgeHandles[edgeCount++] = eiterator;

		eiterator = MeshMod_MeshEdgeTagIterate(handle, MeshMod_EdgeEndVertexTag, &eiterator);
	}
	for(size_t i = 0;i < edgeCount/2;i++) {

	}

	MeshMod_MeshDestroy(handle);
}

TEST_CASE("Mesh similar edge compute 3", "[MeshMod BasicAlgos]") {
	MeshMod_MeshHandle handle = MeshMod_MeshCreate({0}, "Tri and Quad");

	// do it thrice so we have similar (same) edges
	AddTriAndQuadToMesh(handle);
	AddTriAndQuadToMesh(handle);
	AddTriAndQuadToMesh(handle);

	MeshMod_MeshComputeSimilarEdges(handle);

	MeshMod_EdgeHandle edges[7*3];
	uint64_t edgeCount = 0;

	MeshMod_EdgeHandle eiterator = MeshMod_MeshEdgeTagIterate(handle, MeshMod_EdgeHalfEdgeTag, NULL);
	while (MeshMod_MeshEdgeIsValid(handle, eiterator)) {

		edges[edgeCount++] = eiterator;
		eiterator = MeshMod_MeshEdgeTagIterate(handle, MeshMod_EdgeHalfEdgeTag, &eiterator);
	}
	REQUIRE(edgeCount == 21);

	for(uint64_t i = 0; i < edgeCount/3;++i) {
		// should be 3 edges in each similar, and should be some permutation of i + 0, i + 7, i + 14
		bool found[3] = { false, false, false };
		int similarCount = 0;
		MeshMod_EdgeSimilar const* similar = MeshMod_MeshEdgeSimilarTagHandleToPtr(handle, edges[i], 0);
		while(!Handle_HandleEqual64(similar->handle, edges[i + 0].handle)) {
			similarCount++;

			REQUIRE_FALSE(Handle_HandleEqual64(similar->handle, edges[i + 0].handle));

			if(Handle_HandleEqual64(similar->handle, edges[i + 7].handle)) {
				REQUIRE_FALSE(found[1]);
				found[1] = true;
			} else if(Handle_HandleEqual64(similar->handle, edges[i + 14].handle)) {
				REQUIRE_FALSE(found[2]);
				found[2] = true;
			}
			similar = MeshMod_MeshEdgeSimilarTagHandleToPtr(handle, *similar, 0);
			if(Handle_HandleEqual64(similar->handle, edges[i + 0].handle)) {
				REQUIRE_FALSE(found[0]);
				found[0] = true;
				similarCount++;
			}
		}
		REQUIRE(similarCount == 3);
		REQUIRE(found[0]);
		REQUIRE(found[1]);
		REQUIRE(found[2]);
	}

	MeshMod_MeshDestroy(handle);

}
