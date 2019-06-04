#include "al2o3_catch2/catch2.hpp"
#include "gfx_meshmod/registry.h"
#include "gfx_meshmod/mesh.h"
#include "gfx_meshmod/vertex/position.h"
#include "gfx_meshmod/edge/halfedge.h"
#include "gfx_meshmod/polygon/tribrep.h"

TEST_CASE("Mesh Create", "[MeshMod Mesh]") {

	MeshMod_RegistryHandle regHandle = MeshMod_RegistryCreateWithDefaults();
	REQUIRE(regHandle);
	MeshMod_MeshHandle handle = MeshMod_MeshCreate(regHandle, "Test Mesh");
	REQUIRE(handle);
	MeshMod_MeshDestroy(handle);

	// ensure registy is still valid
	REQUIRE(MeshMod_RegistryDescription(regHandle, MeshMod_VertexPositionTag) != NULL);
	MeshMod_RegistryDestroy(regHandle);
}

TEST_CASE("Mesh CreateWithDefaults", "[MeshMod Mesh]") {
	MeshMod_MeshHandle handle = MeshMod_MeshCreateWithDefaultRegistry("Test Mesh");
	REQUIRE(handle);
	MeshMod_MeshDestroy(handle);
}

TEST_CASE("Mesh vertex", "[MeshMod Mesh]") {
	MeshMod_MeshHandle handle = MeshMod_MeshCreateWithDefaultRegistry("Test Mesh");
	REQUIRE(handle);

	MeshMod_DataContainerHandle vhandle = MeshMod_MeshGetVertices(handle);
	REQUIRE(MeshMod_DataContainerAdd(vhandle, MeshMod_VertexPositionTag));
	REQUIRE_FALSE(MeshMod_DataContainerAdd(vhandle, MeshMod_VertexPositionTag));

	REQUIRE_FALSE(MeshMod_DataContainerAdd(vhandle, MeshMod_EdgeHalfEdgeTag));
	REQUIRE_FALSE(MeshMod_DataContainerAdd(vhandle, MeshMod_PolygonTriBRepTag));
	
	REQUIRE(MeshMod_DataContainerSize(vhandle) == 0);
	MeshMod_DataContainerResize(vhandle, 100);
	REQUIRE(MeshMod_DataContainerSize(vhandle) == 100);
	MeshMod_DataContainerResize(vhandle, 1);
	REQUIRE(MeshMod_DataContainerSize(vhandle) == 1);
	CADT_VectorHandle posVec = MeshMod_DataContainerVectorLookup(vhandle, MeshMod_VertexPositionTag);
	REQUIRE(posVec);
	MeshMod_VertexPosition* pos0 = (MeshMod_VertexPosition *)CADT_VectorAt(posVec, 0);
	REQUIRE(pos0);
	REQUIRE(Math_IsNan3F(*pos0));

	MeshMod_DataContainerResize(vhandle, 100);
	REQUIRE(MeshMod_DataContainerSize(vhandle) == 100);
	pos0 = (MeshMod_VertexPosition*)CADT_VectorAt(posVec, 0);
	MeshMod_VertexPosition* pos10 = (MeshMod_VertexPosition*)CADT_VectorAt(posVec, 10);
	REQUIRE(pos0);
	REQUIRE(Math_IsNan3F(*pos0));
	REQUIRE(pos10);
	REQUIRE(Math_IsNan3F(*pos10));
	Math_Vec3F_t const val = { 10.0f, 20.0f, 30.0f };
	*pos10 =  val;
	pos10 = (MeshMod_VertexPosition*)CADT_VectorAt(posVec, 10);
	REQUIRE(pos10);
	REQUIRE(Math_ApproxEqualVec3F(*pos10, val, 0.0f));
	MeshMod_DataContainerResize(vhandle, 1000);
	REQUIRE(MeshMod_DataContainerSize(vhandle) == 1000);
	pos10 = (MeshMod_VertexPosition*)CADT_VectorAt(posVec, 10);
	REQUIRE(pos10);
	REQUIRE(Math_ApproxEqualVec3F(*pos10, val, 0.0f));
	MeshMod_DataContainerResize(vhandle, 11);
	REQUIRE(MeshMod_DataContainerSize(vhandle) == 11);
	pos10 = (MeshMod_VertexPosition*)CADT_VectorAt(posVec, 10);
	REQUIRE(pos10);
	REQUIRE(Math_ApproxEqualVec3F(*pos10, val, 0.0f));

	REQUIRE(MeshMod_DataContainerIsValid(vhandle, 0));
	REQUIRE(MeshMod_DataContainerIsValid(vhandle, 10));
	MeshMod_DataContainerMarkInvalid(vhandle, 10);
	REQUIRE(MeshMod_DataContainerIsValid(vhandle, 0));
	REQUIRE_FALSE(MeshMod_DataContainerIsValid(vhandle, 10));
	MeshMod_DataContainerResize(vhandle, 10000);
	REQUIRE(MeshMod_DataContainerSize(vhandle) == 10000);
	REQUIRE(MeshMod_DataContainerIsValid(vhandle, 0));
	REQUIRE_FALSE(MeshMod_DataContainerIsValid(vhandle, 10));
	MeshMod_DataContainerSwap(vhandle, 0, 10);
	REQUIRE(MeshMod_DataContainerIsValid(vhandle, 10));
	REQUIRE_FALSE(MeshMod_DataContainerIsValid(vhandle, 0));
	MeshMod_VertexPosition* pos9 = (MeshMod_VertexPosition*)CADT_VectorAt(posVec, 9);
	pos10 = (MeshMod_VertexPosition*)CADT_VectorAt(posVec, 10);
	Math_Vec3F_t const val9 = { 9.0f, 18.0f, 27.0f };
	*pos9 = val9;
	*pos10 = val;
	REQUIRE(Math_ApproxEqualVec3F(*pos9, val9, 0.0f));
	REQUIRE(Math_ApproxEqualVec3F(*pos10, val, 0.0f));
	MeshMod_DataContainerSwap(vhandle, 9, 10);
	REQUIRE_FALSE(Math_ApproxEqualVec3F(*pos10, val, 0.0f));
	REQUIRE(Math_ApproxEqualVec3F(*pos9, val, 0.0f));
	REQUIRE(Math_ApproxEqualVec3F(*pos10, val9, 0.0f));
	MeshMod_DataContainerReplace(vhandle, 9, 10);
	REQUIRE(Math_ApproxEqualVec3F(*pos9, val, 0.0f));
	REQUIRE(Math_ApproxEqualVec3F(*pos10, val, 0.0f));
	MeshMod_DataContainerRemove(vhandle, 9);

	pos10 = (MeshMod_VertexPosition*)CADT_VectorAt(posVec, 9);
	REQUIRE(Math_ApproxEqualVec3F(*pos10, val, 0.0f));

	MeshMod_DataContainerResize(vhandle, 12);
	REQUIRE(MeshMod_DataContainerSize(vhandle) == 12);
	MeshMod_VertexPosition* pos11 = (MeshMod_VertexPosition*)CADT_VectorAt(posVec, 11);
	*pos11 = val9;
	MeshMod_DataContainerSwapRemove(vhandle, 9);
	pos10 = (MeshMod_VertexPosition*)CADT_VectorAt(posVec, 9);
	REQUIRE(Math_ApproxEqualVec3F(*pos10, val9, 0.0f));

	MeshMod_MeshDestroy(handle);
}

TEST_CASE("Mesh edge", "[MeshMod Mesh]") {
	MeshMod_MeshHandle handle = MeshMod_MeshCreateWithDefaultRegistry("Test Mesh");
	REQUIRE(handle);
	MeshMod_DataContainerHandle ehandle = MeshMod_MeshGetEdges(handle);
	REQUIRE_FALSE(MeshMod_DataContainerAdd(ehandle, MeshMod_VertexPositionTag));

	REQUIRE(MeshMod_DataContainerAdd(ehandle, MeshMod_EdgeHalfEdgeTag));
	REQUIRE_FALSE(MeshMod_DataContainerAdd(ehandle, MeshMod_EdgeHalfEdgeTag));

	REQUIRE_FALSE(MeshMod_DataContainerAdd(ehandle, MeshMod_VertexPositionTag));
	REQUIRE_FALSE(MeshMod_DataContainerAdd(ehandle, MeshMod_PolygonTriBRepTag));
	REQUIRE(MeshMod_DataContainerSize(ehandle) == 0);

	MeshMod_MeshDestroy(handle);
}
