#include "al2o3_catch2/catch2.hpp"
#include "render_meshmod/registry.h"
#include "render_meshmod/mesh.h"
#include "render_meshmod/vertex/basicdata.h"
#include "render_meshmod/vertex/position.h"
#include "render_meshmod/edge/halfedge.h"
#include "render_meshmod/polygon/tribrep.h"
#include "render_meshmod/polygon/quadbrep.h"
#include "render_meshmod/polygon/convexbrep.h"
#include "render_meshmod/polygon/basicdata.h"

TEST_CASE("Mesh Create", "[MeshMod Mesh]") {

	MeshMod_RegistryHandle regHandle = MeshMod_RegistryCreateWithDefaults();
	REQUIRE(MeshMod_RegistryHandleIsValid(regHandle));
	MeshMod_MeshHandle handle = MeshMod_MeshCreate(regHandle, "Test Mesh");
	REQUIRE(MeshMod_MeshHandleIsValid(handle));
	MeshMod_MeshDestroy(handle);

	// ensure registy is still valid
	REQUIRE(MeshMod_RegistryDescription(regHandle, MeshMod_VertexPositionTag.tag) != NULL);
	MeshMod_RegistryDestroy(regHandle);
}

TEST_CASE("Mesh CreateWithDefaults", "[MeshMod Mesh]") {
	MeshMod_MeshHandle handle = MeshMod_MeshCreate({0}, "Test Mesh");
	REQUIRE(MeshMod_MeshHandleIsValid(handle));
	MeshMod_MeshDestroy(handle);
}

TEST_CASE("Mesh vertex", "[MeshMod Mesh]") {
	MeshMod_MeshHandle handle = MeshMod_MeshCreate({0},"Test Mesh");
	REQUIRE(MeshMod_MeshHandleIsValid(handle));

	MeshMod_MeshVertexTagEnsure(handle, MeshMod_VertexPositionTag);
	REQUIRE(MeshMod_MeshVertexTagExists(handle, MeshMod_VertexPositionTag));

	MeshMod_MeshVertexTagRemove(handle, MeshMod_VertexPositionTag);
	REQUIRE_FALSE(MeshMod_MeshVertexTagExists(handle, MeshMod_VertexPositionTag));

	// there are no tags, so we should always get an invalid handle if we try and alloc
	MeshMod_VertexHandle invalidVHandle = MeshMod_MeshVertexAlloc(handle);
	REQUIRE_FALSE(MeshMod_MeshVertexIsValid(handle, invalidVHandle));

	// now with a tag we should get valid handles
	MeshMod_MeshVertexTagEnsure(handle, MeshMod_VertexPositionTag);
	MeshMod_VertexHandle vhandle0 = MeshMod_MeshVertexAlloc(handle);
	REQUIRE(MeshMod_MeshVertexIsValid(handle, vhandle0));
	MeshMod_VertexHandle vhandle1 = MeshMod_MeshVertexAlloc(handle);
	REQUIRE(MeshMod_MeshVertexIsValid(handle, vhandle1));
	REQUIRE(memcmp(&vhandle0, &vhandle1, sizeof(MeshMod_VertexHandle)) != 0);

	MeshMod_MeshVertexRelease(handle, vhandle0);
	REQUIRE_FALSE(MeshMod_MeshVertexIsValid(handle, vhandle0));

	auto pos1 = (MeshMod_VertexPosition*) MeshMod_MeshVertexTagHandleToPtr(handle, MeshMod_VertexPositionTag, vhandle1);
	REQUIRE(pos1);
	REQUIRE(Math_IsNanVec3F(*pos1));

	MeshMod_VertexHandle vhandle2 = MeshMod_MeshVertexAlloc(handle);
	REQUIRE(MeshMod_MeshVertexIsValid(handle, vhandle2));

	pos1 = (MeshMod_VertexPosition*) MeshMod_MeshVertexTagHandleToPtr(handle, MeshMod_VertexPositionTag, vhandle1);
	REQUIRE(pos1);
	REQUIRE(Math_IsNanVec3F(*pos1));
	Math_Vec3F const zero3F = { 0 };
	Math_Vec3F const val0 = { 1.0f, 2.0f, 3.0f };
	*pos1 =  val0;
	REQUIRE(Math_ApproxEqualVec3F(*pos1, val0, 0.0f));

	MeshMod_VertexHandle vhandle3 = MeshMod_MeshVertexAlloc(handle);
	REQUIRE(MeshMod_MeshVertexIsValid(handle, vhandle3));
	pos1 = (MeshMod_VertexPosition*) MeshMod_MeshVertexTagHandleToPtr(handle, MeshMod_VertexPositionTag, vhandle1);
	REQUIRE(pos1);
	REQUIRE(Math_ApproxEqualVec3F(*pos1, val0, 0.0f));
	*pos1 = zero3F;

	MeshMod_VertexHandle vhandle4 = MeshMod_MeshVertexAlloc(handle);
	REQUIRE(MeshMod_MeshVertexIsValid(handle, vhandle4));
	pos1 = (MeshMod_VertexPosition*) MeshMod_MeshVertexTagHandleToPtr(handle, MeshMod_VertexPositionTag, vhandle1);
	REQUIRE(Math_ApproxEqualVec3F(*pos1, zero3F, 0.0f));
	MeshMod_MeshVertexRelease(handle, vhandle4);

	pos1 = (MeshMod_VertexPosition*) MeshMod_MeshVertexTagHandleToPtr(handle, MeshMod_VertexPositionTag, vhandle1);
	REQUIRE(Math_ApproxEqualVec3F(*pos1, zero3F, 0.0f));
	*pos1 = val0;

	auto pos2 = (MeshMod_VertexPosition*) MeshMod_MeshVertexTagHandleToPtr(handle, MeshMod_VertexPositionTag, vhandle2);
	REQUIRE(Math_IsNanVec3F(*pos2));
	MeshMod_MeshVertexSwap(handle, vhandle1, vhandle2);
	REQUIRE(Math_IsNanVec3F(*pos1));
	REQUIRE(Math_ApproxEqualVec3F(*pos2, val0, 0.0f));

	MeshMod_MeshVertexReplace(handle, vhandle2, vhandle1);
	REQUIRE(Math_ApproxEqualVec3F(*pos1, val0, 0.0f));
	REQUIRE(Math_ApproxEqualVec3F(*pos2, val0, 0.0f));

	MeshMod_MeshDestroy(handle);
}

TEST_CASE("Mesh vertex interpolation", "[MeshMod Mesh]") {
	MeshMod_MeshHandle handle = MeshMod_MeshCreate({0},"Test Mesh");
	REQUIRE(MeshMod_MeshHandleIsValid(handle));

	MeshMod_MeshVertexTagEnsure(handle, MeshMod_VertexVec2FTag);
	REQUIRE(sizeof(Math_Vec2F) == sizeof(MeshMod_VertexVec2F));

	auto vhandle0 = MeshMod_MeshVertexAlloc(handle);
	auto vhandle1 = MeshMod_MeshVertexAlloc(handle);
	auto vhandle2 = MeshMod_MeshVertexAlloc(handle);
	Math_Vec2F const a = { 0.0f, 0.0f };
	Math_Vec2F const b = { 0.0f, 1.0f };
	Math_Vec2F const c = { 1.0f, 0.0f };

	auto pos0 = (MeshMod_VertexVec2F*) MeshMod_MeshVertexTagHandleToPtr(handle, MeshMod_VertexVec2FTag, vhandle0);
	auto pos1 = (MeshMod_VertexVec2F*) MeshMod_MeshVertexTagHandleToPtr(handle, MeshMod_VertexVec2FTag, vhandle1);
	auto pos2 = (MeshMod_VertexVec2F*) MeshMod_MeshVertexTagHandleToPtr(handle, MeshMod_VertexVec2FTag, vhandle2);

	*(pos0) = a;
	*(pos1) = b;
	*(pos2) = c;
	REQUIRE(Math_ApproxEqualVec2F(a, *pos0, 1e-5f));
	REQUIRE(Math_ApproxEqualVec2F(b, *pos1, 1e-5f));
	REQUIRE(Math_ApproxEqualVec2F(c, *pos2, 1e-5f));

	MeshMod_VertexHandle inter0 = MeshMod_MeshVertexInterpolate1D(handle, vhandle0, vhandle1, 0.0f);
	REQUIRE(MeshMod_MeshVertexIsValid(handle, inter0));
	MeshMod_VertexHandle inter1 = MeshMod_MeshVertexInterpolate1D(handle, vhandle0, vhandle1, 1.0f);
	REQUIRE(MeshMod_MeshVertexIsValid(handle, inter1));
	MeshMod_VertexHandle inter2 = MeshMod_MeshVertexInterpolate2D(handle, vhandle0, vhandle1, vhandle2, 0.0f, 0.0f);
	REQUIRE(MeshMod_MeshVertexIsValid(handle, inter2));
	MeshMod_VertexHandle inter3 = MeshMod_MeshVertexInterpolate2D(handle, vhandle0, vhandle1, vhandle2, 1.0f, 0.0f);
	REQUIRE(MeshMod_MeshVertexIsValid(handle, inter3));
	MeshMod_VertexHandle inter4 = MeshMod_MeshVertexInterpolate2D(handle, vhandle0, vhandle1, vhandle2, 0.0f, 1.0f);
	REQUIRE(MeshMod_MeshVertexIsValid(handle, inter4));

	auto result0 = (MeshMod_VertexVec2F*) MeshMod_MeshVertexTagHandleToPtr(handle, MeshMod_VertexVec2FTag, inter0);
	REQUIRE(result0);
	auto result1 = (MeshMod_VertexVec2F*) MeshMod_MeshVertexTagHandleToPtr(handle, MeshMod_VertexVec2FTag, inter1);
	REQUIRE(result1);
	auto result2 = (MeshMod_VertexVec2F*) MeshMod_MeshVertexTagHandleToPtr(handle, MeshMod_VertexVec2FTag, inter2);
	REQUIRE(result2);
	auto result3 = (MeshMod_VertexVec2F*) MeshMod_MeshVertexTagHandleToPtr(handle, MeshMod_VertexVec2FTag, inter3);
	REQUIRE(result3);
	auto result4 = (MeshMod_VertexVec2F*) MeshMod_MeshVertexTagHandleToPtr(handle, MeshMod_VertexVec2FTag, inter4);
	REQUIRE(result4);


	REQUIRE(Math_ApproxEqualVec2F(a, *result0, 1e-5f));
	REQUIRE(Math_ApproxEqualVec2F(b, *result1, 1e-5f));
	REQUIRE(Math_ApproxEqualVec2F(a, *result2, 1e-5f));
	REQUIRE(Math_ApproxEqualVec2F(b, *result3, 1e-5f));
	REQUIRE(Math_ApproxEqualVec2F(c, *result4, 1e-5f));

	MeshMod_MeshDestroy(handle);
}

TEST_CASE("Mesh triangle", "[MeshMod Mesh]") {
	MeshMod_MeshHandle handle = MeshMod_MeshCreate({0},"Test Mesh");
	REQUIRE(MeshMod_MeshHandleIsValid(handle));

	MeshMod_MeshVertexTagEnsure(handle, MeshMod_VertexPositionTag);
	MeshMod_MeshEdgeTagEnsure(handle, MeshMod_EdgeHalfEdgeTag);
	MeshMod_MeshPolygonTagEnsure(handle, MeshMod_PolygonTriBRepTag);

	// 3 vertices, 3 edges, 1 triangle
	MeshMod_VertexHandle vhandles[3];

	vhandles[0] = MeshMod_MeshVertexAlloc(handle);
	vhandles[1] = MeshMod_MeshVertexAlloc(handle);
	vhandles[2] = MeshMod_MeshVertexAlloc(handle);
	MeshMod_EdgeHandle ehandles[3];
	ehandles[0] = MeshMod_MeshEdgeAlloc(handle);
	ehandles[1] = MeshMod_MeshEdgeAlloc(handle);
	ehandles[2] = MeshMod_MeshEdgeAlloc(handle);
	MeshMod_PolygonHandle phandles[1];
	phandles[0] = MeshMod_MeshPolygonAlloc(handle);

	REQUIRE(MeshMod_MeshVertexIsValid(handle, vhandles[0]));
	REQUIRE(MeshMod_MeshVertexIsValid(handle, vhandles[1]));
	REQUIRE(MeshMod_MeshVertexIsValid(handle, vhandles[2]));
	REQUIRE(MeshMod_MeshEdgeIsValid(handle, ehandles[0]));
	REQUIRE(MeshMod_MeshEdgeIsValid(handle, ehandles[1]));
	REQUIRE(MeshMod_MeshEdgeIsValid(handle, ehandles[2]));
	REQUIRE(MeshMod_MeshPolygonIsValid(handle, phandles[0]));

	auto pos0 = (MeshMod_VertexPosition*) MeshMod_MeshVertexTagHandleToPtr(handle, MeshMod_VertexPositionTag, vhandles[0]);
	auto pos1 = (MeshMod_VertexPosition*) MeshMod_MeshVertexTagHandleToPtr(handle, MeshMod_VertexPositionTag, vhandles[1]);
	auto pos2 = (MeshMod_VertexPosition*) MeshMod_MeshVertexTagHandleToPtr(handle, MeshMod_VertexPositionTag, vhandles[2]);
	REQUIRE(pos0);
	REQUIRE(pos1);
	REQUIRE(pos2);
	REQUIRE(Math_IsNanVec3F(*pos0));
	REQUIRE(Math_IsNanVec3F(*pos1));
	REQUIRE(Math_IsNanVec3F(*pos2));
	*pos0 = {0, 0, 0};
	*pos1 = {1, 0, 0};
	*pos2 = {1, 1, 0};
	auto edge0 = (MeshMod_EdgeHalfEdge*) MeshMod_MeshEdgeTagHandleToPtr(handle, MeshMod_EdgeHalfEdgeTag, ehandles[0]);
	auto edge1 = (MeshMod_EdgeHalfEdge*) MeshMod_MeshEdgeTagHandleToPtr(handle, MeshMod_EdgeHalfEdgeTag, ehandles[1]);
	auto edge2 = (MeshMod_EdgeHalfEdge*) MeshMod_MeshEdgeTagHandleToPtr(handle, MeshMod_EdgeHalfEdgeTag, ehandles[2]);
	edge0->vertex = vhandles[0];
	edge0->polygon = phandles[0];
	edge1->vertex = vhandles[1];
	edge1->polygon = phandles[0];
	edge2->vertex = vhandles[2];
	edge2->polygon = phandles[0];

	auto polygon0 = (MeshMod_PolygonTriBRep*) MeshMod_MeshPolygonTagHandleToPtr(handle, MeshMod_PolygonTriBRepTag, phandles[0]);
	polygon0->edge[0] = ehandles[0];
	polygon0->edge[1] = ehandles[1];
	polygon0->edge[2] = ehandles[2];

	MeshMod_PolygonHandle piterator = MeshMod_MeshPolygonIterate(handle, NULL);
	uint64_t polyCount = 0;
	while(MeshMod_MeshPolygonIsValid(handle, piterator)) {

		auto poly = (MeshMod_PolygonTriBRep*) MeshMod_MeshPolygonTagHandleToPtr(handle, MeshMod_PolygonTriBRepTag, piterator);
		REQUIRE(poly);
		REQUIRE(poly->edge[0].handle.handle == ehandles[0].handle.handle);
		REQUIRE(poly->edge[1].handle.handle == ehandles[1].handle.handle);
		REQUIRE(poly->edge[2].handle.handle == ehandles[2].handle.handle);
		polyCount++;
		piterator = MeshMod_MeshPolygonIterate(handle, &piterator);
	}
	REQUIRE(polyCount == 1);

	MeshMod_EdgeHandle eiterator = MeshMod_MeshEdgeIterate(handle, NULL);
	uint64_t edgeCount = 0;
	while(MeshMod_MeshEdgeIsValid(handle, eiterator)) {

		auto edge = (MeshMod_EdgeHalfEdge*) MeshMod_MeshEdgeTagHandleToPtr(handle, MeshMod_EdgeHalfEdgeTag, eiterator);
		REQUIRE(edge);
		REQUIRE(edge->polygon.handle.handle == phandles[0].handle.handle);
		REQUIRE(edge->vertex.handle.handle == vhandles[edgeCount].handle.handle);
		edgeCount++;
		eiterator = MeshMod_MeshEdgeIterate(handle, &eiterator);
	}
	REQUIRE(edgeCount == 3);

	MeshMod_MeshDestroy(handle);
}

TEST_CASE("Mesh clone triangle", "[MeshMod Mesh]") {
	MeshMod_MeshHandle handle = MeshMod_MeshCreate({0},"Test Mesh");
	REQUIRE(MeshMod_MeshHandleIsValid(handle));

	MeshMod_MeshVertexTagEnsure(handle, MeshMod_VertexPositionTag);
	MeshMod_MeshEdgeTagEnsure(handle, MeshMod_EdgeHalfEdgeTag);
	MeshMod_MeshPolygonTagEnsure(handle, MeshMod_PolygonTriBRepTag);

	// 3 vertices, 3 edges, 1 triangle
	MeshMod_VertexHandle vhandles[3];

	vhandles[0] = MeshMod_MeshVertexAlloc(handle);
	vhandles[1] = MeshMod_MeshVertexAlloc(handle);
	vhandles[2] = MeshMod_MeshVertexAlloc(handle);
	MeshMod_EdgeHandle ehandles[3];
	ehandles[0] = MeshMod_MeshEdgeAlloc(handle);
	ehandles[1] = MeshMod_MeshEdgeAlloc(handle);
	ehandles[2] = MeshMod_MeshEdgeAlloc(handle);
	MeshMod_PolygonHandle phandles[1];
	phandles[0] = MeshMod_MeshPolygonAlloc(handle);

	REQUIRE(MeshMod_MeshVertexIsValid(handle, vhandles[0]));
	REQUIRE(MeshMod_MeshVertexIsValid(handle, vhandles[1]));
	REQUIRE(MeshMod_MeshVertexIsValid(handle, vhandles[2]));
	REQUIRE(MeshMod_MeshEdgeIsValid(handle, ehandles[0]));
	REQUIRE(MeshMod_MeshEdgeIsValid(handle, ehandles[1]));
	REQUIRE(MeshMod_MeshEdgeIsValid(handle, ehandles[2]));
	REQUIRE(MeshMod_MeshPolygonIsValid(handle, phandles[0]));

	auto pos0 = (MeshMod_VertexPosition*) MeshMod_MeshVertexTagHandleToPtr(handle, MeshMod_VertexPositionTag, vhandles[0]);
	auto pos1 = (MeshMod_VertexPosition*) MeshMod_MeshVertexTagHandleToPtr(handle, MeshMod_VertexPositionTag, vhandles[1]);
	auto pos2 = (MeshMod_VertexPosition*) MeshMod_MeshVertexTagHandleToPtr(handle, MeshMod_VertexPositionTag, vhandles[2]);
	REQUIRE(pos0);
	REQUIRE(pos1);
	REQUIRE(pos2);
	REQUIRE(Math_IsNanVec3F(*pos0));
	REQUIRE(Math_IsNanVec3F(*pos1));
	REQUIRE(Math_IsNanVec3F(*pos2));
	*pos0 = {0, 0, 0};
	*pos1 = {1, 0, 0};
	*pos2 = {1, 1, 0};
	auto edge0 = (MeshMod_EdgeHalfEdge*) MeshMod_MeshEdgeTagHandleToPtr(handle, MeshMod_EdgeHalfEdgeTag, ehandles[0]);
	auto edge1 = (MeshMod_EdgeHalfEdge*) MeshMod_MeshEdgeTagHandleToPtr(handle, MeshMod_EdgeHalfEdgeTag, ehandles[1]);
	auto edge2 = (MeshMod_EdgeHalfEdge*) MeshMod_MeshEdgeTagHandleToPtr(handle, MeshMod_EdgeHalfEdgeTag, ehandles[2]);
	edge0->vertex = vhandles[0];
	edge0->polygon = phandles[0];
	edge1->vertex = vhandles[1];
	edge1->polygon = phandles[0];
	edge2->vertex = vhandles[2];
	edge2->polygon = phandles[0];

	auto polygon0 = (MeshMod_PolygonTriBRep*) MeshMod_MeshPolygonTagHandleToPtr(handle, MeshMod_PolygonTriBRepTag, phandles[0]);
	polygon0->edge[0] = ehandles[0];
	polygon0->edge[1] = ehandles[1];
	polygon0->edge[2] = ehandles[2];

	MeshMod_MeshHandle cloneHandle = MeshMod_MeshClone(handle);
	MeshMod_MeshDestroy(handle);

	MeshMod_PolygonHandle piterator = MeshMod_MeshPolygonIterate(cloneHandle, NULL);
	uint64_t polyCount = 0;
	while(MeshMod_MeshPolygonIsValid(cloneHandle, piterator)) {

		auto poly = (MeshMod_PolygonTriBRep*) MeshMod_MeshPolygonTagHandleToPtr(cloneHandle, MeshMod_PolygonTriBRepTag, piterator);
		REQUIRE(poly);
		REQUIRE(poly->edge[0].handle.handle == ehandles[0].handle.handle);
		REQUIRE(poly->edge[1].handle.handle == ehandles[1].handle.handle);
		REQUIRE(poly->edge[2].handle.handle == ehandles[2].handle.handle);
		polyCount++;
		piterator = MeshMod_MeshPolygonIterate(cloneHandle, &piterator);
	}
	REQUIRE(polyCount == 1);

	MeshMod_EdgeHandle eiterator = MeshMod_MeshEdgeIterate(cloneHandle, NULL);
	uint64_t edgeCount = 0;
	while(MeshMod_MeshEdgeIsValid(cloneHandle, eiterator)) {

		auto edge = (MeshMod_EdgeHalfEdge*) MeshMod_MeshEdgeTagHandleToPtr(cloneHandle, MeshMod_EdgeHalfEdgeTag, eiterator);
		REQUIRE(edge);
		REQUIRE(edge->polygon.handle.handle == phandles[0].handle.handle);
		REQUIRE(edge->vertex.handle.handle == vhandles[edgeCount].handle.handle);
		edgeCount++;
		eiterator = MeshMod_MeshEdgeIterate(cloneHandle, &eiterator);
	}
	REQUIRE(edgeCount == 3);

	MeshMod_MeshDestroy(cloneHandle);
}


TEST_CASE("Mesh quad", "[MeshMod Mesh]") {
	MeshMod_MeshHandle handle = MeshMod_MeshCreate({0},"Test Mesh");
	REQUIRE(MeshMod_MeshHandleIsValid(handle));

	MeshMod_MeshVertexTagEnsure(handle, MeshMod_VertexPositionTag);
	MeshMod_MeshEdgeTagEnsure(handle, MeshMod_EdgeHalfEdgeTag);
	MeshMod_MeshPolygonTagEnsure(handle, MeshMod_PolygonQuadBRepTag);

	// 4 vertices, 4 edges, 1 quad
	MeshMod_VertexHandle vhandles[4];

	vhandles[0] = MeshMod_MeshVertexAlloc(handle);
	vhandles[1] = MeshMod_MeshVertexAlloc(handle);
	vhandles[2] = MeshMod_MeshVertexAlloc(handle);
	vhandles[3] = MeshMod_MeshVertexAlloc(handle);
	MeshMod_EdgeHandle ehandles[4];
	ehandles[0] = MeshMod_MeshEdgeAlloc(handle);
	ehandles[1] = MeshMod_MeshEdgeAlloc(handle);
	ehandles[2] = MeshMod_MeshEdgeAlloc(handle);
	ehandles[3] = MeshMod_MeshEdgeAlloc(handle);
	MeshMod_PolygonHandle phandles[1];
	phandles[0] = MeshMod_MeshPolygonAlloc(handle);

	REQUIRE(MeshMod_MeshVertexIsValid(handle, vhandles[0]));
	REQUIRE(MeshMod_MeshVertexIsValid(handle, vhandles[1]));
	REQUIRE(MeshMod_MeshVertexIsValid(handle, vhandles[2]));
	REQUIRE(MeshMod_MeshVertexIsValid(handle, vhandles[3]));
	REQUIRE(MeshMod_MeshEdgeIsValid(handle, ehandles[0]));
	REQUIRE(MeshMod_MeshEdgeIsValid(handle, ehandles[1]));
	REQUIRE(MeshMod_MeshEdgeIsValid(handle, ehandles[2]));
	REQUIRE(MeshMod_MeshEdgeIsValid(handle, ehandles[3]));
	REQUIRE(MeshMod_MeshPolygonIsValid(handle, phandles[0]));

	auto pos0 = (MeshMod_VertexPosition*) MeshMod_MeshVertexTagHandleToPtr(handle, MeshMod_VertexPositionTag, vhandles[0]);
	auto pos1 = (MeshMod_VertexPosition*) MeshMod_MeshVertexTagHandleToPtr(handle, MeshMod_VertexPositionTag, vhandles[1]);
	auto pos2 = (MeshMod_VertexPosition*) MeshMod_MeshVertexTagHandleToPtr(handle, MeshMod_VertexPositionTag, vhandles[2]);
	auto pos3 = (MeshMod_VertexPosition*) MeshMod_MeshVertexTagHandleToPtr(handle, MeshMod_VertexPositionTag, vhandles[3]);
	REQUIRE(pos0);
	REQUIRE(pos1);
	REQUIRE(pos2);
	REQUIRE(pos3);
	REQUIRE(Math_IsNanVec3F(*pos0));
	REQUIRE(Math_IsNanVec3F(*pos1));
	REQUIRE(Math_IsNanVec3F(*pos2));
	REQUIRE(Math_IsNanVec3F(*pos3));
	*pos0 = {0, 0, 0};
	*pos1 = {1, 0, 0};
	*pos2 = {1, 1, 0};
	*pos3 = {0, 1, 0};
	auto edge0 = (MeshMod_EdgeHalfEdge*) MeshMod_MeshEdgeTagHandleToPtr(handle, MeshMod_EdgeHalfEdgeTag, ehandles[0]);
	auto edge1 = (MeshMod_EdgeHalfEdge*) MeshMod_MeshEdgeTagHandleToPtr(handle, MeshMod_EdgeHalfEdgeTag, ehandles[1]);
	auto edge2 = (MeshMod_EdgeHalfEdge*) MeshMod_MeshEdgeTagHandleToPtr(handle, MeshMod_EdgeHalfEdgeTag, ehandles[2]);
	auto edge3 = (MeshMod_EdgeHalfEdge*) MeshMod_MeshEdgeTagHandleToPtr(handle, MeshMod_EdgeHalfEdgeTag, ehandles[3]);
	edge0->vertex = vhandles[0];
	edge0->polygon = phandles[0];
	edge1->vertex = vhandles[1];
	edge1->polygon = phandles[0];
	edge2->vertex = vhandles[2];
	edge2->polygon = phandles[0];
	edge3->vertex = vhandles[3];
	edge3->polygon = phandles[0];

	auto polygon0 = (MeshMod_PolygonQuadBRep*) MeshMod_MeshPolygonTagHandleToPtr(handle, MeshMod_PolygonQuadBRepTag, phandles[0]);
	polygon0->edge[0] = ehandles[0];
	polygon0->edge[1] = ehandles[1];
	polygon0->edge[2] = ehandles[2];
	polygon0->edge[3] = ehandles[3];

	MeshMod_PolygonHandle piterator = MeshMod_MeshPolygonIterate(handle, NULL);
	uint64_t polyCount = 0;
	while(MeshMod_MeshPolygonIsValid(handle, piterator)) {

		auto poly = (MeshMod_PolygonQuadBRep*) MeshMod_MeshPolygonTagHandleToPtr(handle, MeshMod_PolygonQuadBRepTag, piterator);
		REQUIRE(poly);
		REQUIRE(poly->edge[0].handle.handle == ehandles[0].handle.handle);
		REQUIRE(poly->edge[1].handle.handle == ehandles[1].handle.handle);
		REQUIRE(poly->edge[2].handle.handle == ehandles[2].handle.handle);
		REQUIRE(poly->edge[3].handle.handle == ehandles[3].handle.handle);
		polyCount++;
		piterator = MeshMod_MeshPolygonIterate(handle, &piterator);
	}
	REQUIRE(polyCount == 1);

	MeshMod_EdgeHandle eiterator = MeshMod_MeshEdgeIterate(handle, NULL);
	uint64_t edgeCount = 0;
	while(MeshMod_MeshEdgeIsValid(handle, eiterator)) {

		auto edge = (MeshMod_EdgeHalfEdge*) MeshMod_MeshEdgeTagHandleToPtr(handle, MeshMod_EdgeHalfEdgeTag, eiterator);
		REQUIRE(edge);
		REQUIRE(edge->polygon.handle.handle == phandles[0].handle.handle);
		REQUIRE(edge->vertex.handle.handle == vhandles[edgeCount].handle.handle);
		edgeCount++;
		eiterator = MeshMod_MeshEdgeIterate(handle, &eiterator);
	}
	REQUIRE(edgeCount == 4);

	MeshMod_MeshDestroy(handle);
}

TEST_CASE("Mesh Convex", "[MeshMod Mesh]") {
	MeshMod_MeshHandle handle = MeshMod_MeshCreate({0},"Test Mesh");
	REQUIRE(MeshMod_MeshHandleIsValid(handle));

	MeshMod_MeshVertexTagEnsure(handle, MeshMod_VertexPositionTag);
	MeshMod_MeshEdgeTagEnsure(handle, MeshMod_EdgeHalfEdgeTag);
	MeshMod_MeshPolygonTagEnsure(handle, MeshMod_PolygonConvexBRepTag);

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

	auto pos0 = (MeshMod_VertexPosition*) MeshMod_MeshVertexTagHandleToPtr(handle, MeshMod_VertexPositionTag, vhandles[0]);
	auto pos1 = (MeshMod_VertexPosition*) MeshMod_MeshVertexTagHandleToPtr(handle, MeshMod_VertexPositionTag, vhandles[1]);
	auto pos2 = (MeshMod_VertexPosition*) MeshMod_MeshVertexTagHandleToPtr(handle, MeshMod_VertexPositionTag, vhandles[2]);
	auto pos3 = (MeshMod_VertexPosition*) MeshMod_MeshVertexTagHandleToPtr(handle, MeshMod_VertexPositionTag, vhandles[3]);
	auto pos4 = (MeshMod_VertexPosition*) MeshMod_MeshVertexTagHandleToPtr(handle, MeshMod_VertexPositionTag, vhandles[4]);
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
	auto edge0 = (MeshMod_EdgeHalfEdge*) MeshMod_MeshEdgeTagHandleToPtr(handle, MeshMod_EdgeHalfEdgeTag, ehandles[0]);
	auto edge1 = (MeshMod_EdgeHalfEdge*) MeshMod_MeshEdgeTagHandleToPtr(handle, MeshMod_EdgeHalfEdgeTag, ehandles[1]);
	auto edge2 = (MeshMod_EdgeHalfEdge*) MeshMod_MeshEdgeTagHandleToPtr(handle, MeshMod_EdgeHalfEdgeTag, ehandles[2]);
	auto edge3 = (MeshMod_EdgeHalfEdge*) MeshMod_MeshEdgeTagHandleToPtr(handle, MeshMod_EdgeHalfEdgeTag, ehandles[3]);
	auto edge4 = (MeshMod_EdgeHalfEdge*) MeshMod_MeshEdgeTagHandleToPtr(handle, MeshMod_EdgeHalfEdgeTag, ehandles[4]);
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

	auto polygon0 = (MeshMod_PolygonConvexBRep*) MeshMod_MeshPolygonTagHandleToPtr(handle, MeshMod_PolygonConvexBRepTag, phandles[0]);
	polygon0->numEdges = 5;
	polygon0->edge[0] = ehandles[0];
	polygon0->edge[1] = ehandles[1];
	polygon0->edge[2] = ehandles[2];
	polygon0->edge[3] = ehandles[3];
	polygon0->edge[4] = ehandles[4];

	uint64_t polyCount = 0;
	MeshMod_PolygonHandle piterator = MeshMod_MeshPolygonIterate(handle, NULL);
	while(MeshMod_MeshPolygonIsValid(handle, piterator)) {

		auto poly = (MeshMod_PolygonConvexBRep*) MeshMod_MeshPolygonTagHandleToPtr(handle, MeshMod_PolygonConvexBRepTag, piterator);
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
	while(MeshMod_MeshEdgeIsValid(handle, eiterator)) {

		auto edge = (MeshMod_EdgeHalfEdge*) MeshMod_MeshEdgeTagHandleToPtr(handle, MeshMod_EdgeHalfEdgeTag, eiterator);
		REQUIRE(edge);
		REQUIRE(edge->polygon.handle.handle == phandles[0].handle.handle);
		REQUIRE(edge->vertex.handle.handle == vhandles[edgeCount].handle.handle);
		edgeCount++;
		eiterator = MeshMod_MeshEdgeIterate(handle, &eiterator);
	}
	REQUIRE(edgeCount == 5);

	MeshMod_PolygonHandle phandleCopy = MeshMod_MeshPolygonDuplicate(handle, phandles[0]);
	polyCount = 0;
	piterator = MeshMod_MeshPolygonIterate(handle, NULL);
	while(MeshMod_MeshPolygonIsValid(handle, piterator)) {

		auto poly = (MeshMod_PolygonConvexBRep*) MeshMod_MeshPolygonTagHandleToPtr(handle, MeshMod_PolygonConvexBRepTag, piterator);
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

	MeshMod_MeshDestroy(handle);
}

TEST_CASE("Mesh Mixed triangle and quad", "[MeshMod Mesh]") {
	MeshMod_MeshHandle handle = MeshMod_MeshCreate({0},"Test Mesh");
	REQUIRE(MeshMod_MeshHandleIsValid(handle));

	MeshMod_MeshVertexTagEnsure(handle, MeshMod_VertexPositionTag);
	MeshMod_MeshEdgeTagEnsure(handle, MeshMod_EdgeHalfEdgeTag);
	MeshMod_MeshPolygonTagEnsure(handle, MeshMod_PolygonTriBRepTag);
	MeshMod_MeshPolygonTagEnsure(handle, MeshMod_PolygonQuadBRepTag);

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


	auto pos0 = (MeshMod_VertexPosition*) MeshMod_MeshVertexTagHandleToPtr(handle, MeshMod_VertexPositionTag, vhandles[0]);
	auto pos1 = (MeshMod_VertexPosition*) MeshMod_MeshVertexTagHandleToPtr(handle, MeshMod_VertexPositionTag, vhandles[1]);
	auto pos2 = (MeshMod_VertexPosition*) MeshMod_MeshVertexTagHandleToPtr(handle, MeshMod_VertexPositionTag, vhandles[2]);
	auto pos3 = (MeshMod_VertexPosition*) MeshMod_MeshVertexTagHandleToPtr(handle, MeshMod_VertexPositionTag, vhandles[3]);
	auto pos4 = (MeshMod_VertexPosition*) MeshMod_MeshVertexTagHandleToPtr(handle, MeshMod_VertexPositionTag, vhandles[4]);
	auto pos5 = (MeshMod_VertexPosition*) MeshMod_MeshVertexTagHandleToPtr(handle, MeshMod_VertexPositionTag, vhandles[5]);
	auto pos6 = (MeshMod_VertexPosition*) MeshMod_MeshVertexTagHandleToPtr(handle, MeshMod_VertexPositionTag, vhandles[6]);

	*pos0 = {0, 0, 0};
	*pos1 = {1, 0, 0};
	*pos2 = {1, 1, 0};
	*pos3 = {2, 0, 0};
	*pos4 = {3, 0, 0};
	*pos5 = {3, 1, 0};
	*pos6 = {2, 1, 0};

	auto edge0 = (MeshMod_EdgeHalfEdge*) MeshMod_MeshEdgeTagHandleToPtr(handle, MeshMod_EdgeHalfEdgeTag, ehandles[0]);
	auto edge1 = (MeshMod_EdgeHalfEdge*) MeshMod_MeshEdgeTagHandleToPtr(handle, MeshMod_EdgeHalfEdgeTag, ehandles[1]);
	auto edge2 = (MeshMod_EdgeHalfEdge*) MeshMod_MeshEdgeTagHandleToPtr(handle, MeshMod_EdgeHalfEdgeTag, ehandles[2]);
	auto edge3 = (MeshMod_EdgeHalfEdge*) MeshMod_MeshEdgeTagHandleToPtr(handle, MeshMod_EdgeHalfEdgeTag, ehandles[3]);
	auto edge4 = (MeshMod_EdgeHalfEdge*) MeshMod_MeshEdgeTagHandleToPtr(handle, MeshMod_EdgeHalfEdgeTag, ehandles[4]);
	auto edge5 = (MeshMod_EdgeHalfEdge*) MeshMod_MeshEdgeTagHandleToPtr(handle, MeshMod_EdgeHalfEdgeTag, ehandles[5]);
	auto edge6 = (MeshMod_EdgeHalfEdge*) MeshMod_MeshEdgeTagHandleToPtr(handle, MeshMod_EdgeHalfEdgeTag, ehandles[6]);

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

	auto polygon0 = (MeshMod_PolygonTriBRep*) MeshMod_MeshPolygonTagHandleToPtr(handle, MeshMod_PolygonTriBRepTag, phandles[0]);
	polygon0->edge[0] = ehandles[0];
	polygon0->edge[1] = ehandles[1];
	polygon0->edge[2] = ehandles[2];

	auto polygon1 = (MeshMod_PolygonQuadBRep*) MeshMod_MeshPolygonTagHandleToPtr(handle, MeshMod_PolygonQuadBRepTag, phandles[1]);
	polygon1->edge[0] = ehandles[3];
	polygon1->edge[1] = ehandles[4];
	polygon1->edge[2] = ehandles[5];
	polygon1->edge[3] = ehandles[6];

	// check the triangle
	MeshMod_PolygonHandle piterator0 = MeshMod_MeshPolygonTagIterate(handle, MeshMod_PolygonTriBRepTag, NULL);
	uint64_t triCount = 0;
	while(MeshMod_MeshPolygonIsValid(handle, piterator0)) {

		auto poly = (MeshMod_PolygonTriBRep*) MeshMod_MeshPolygonTagHandleToPtr(handle, MeshMod_PolygonTriBRepTag, piterator0);
		REQUIRE(poly);
		REQUIRE(poly->edge[0].handle.handle == ehandles[0].handle.handle);
		REQUIRE(poly->edge[1].handle.handle == ehandles[1].handle.handle);
		REQUIRE(poly->edge[2].handle.handle == ehandles[2].handle.handle);
		triCount++;
		piterator0 = MeshMod_MeshPolygonTagIterate(handle, MeshMod_PolygonTriBRepTag, &piterator0);
	}
	REQUIRE(triCount == 1);

	// check the quad
	MeshMod_PolygonHandle piterator1 = MeshMod_MeshPolygonTagIterate(handle, MeshMod_PolygonQuadBRepTag, NULL);
	uint64_t quadCount = 0;
	while(MeshMod_MeshPolygonIsValid(handle, piterator1)) {

		auto poly = (MeshMod_PolygonQuadBRep*) MeshMod_MeshPolygonTagHandleToPtr(handle, MeshMod_PolygonQuadBRepTag, piterator1);
		REQUIRE(poly);
		REQUIRE(poly->edge[0].handle.handle == ehandles[3].handle.handle);
		REQUIRE(poly->edge[1].handle.handle == ehandles[4].handle.handle);
		REQUIRE(poly->edge[2].handle.handle == ehandles[5].handle.handle);
		REQUIRE(poly->edge[3].handle.handle == ehandles[6].handle.handle);
		quadCount++;
		piterator1 = MeshMod_MeshPolygonTagIterate(handle, MeshMod_PolygonQuadBRepTag, &piterator1);
	}
	REQUIRE(quadCount == 1);


	MeshMod_MeshDestroy(handle);
}
