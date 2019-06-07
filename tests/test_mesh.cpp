#include "al2o3_catch2/catch2.hpp"
#include "gfx_meshmod/registry.h"
#include "gfx_meshmod/mesh.h"
#include "gfx_meshmod/vertex/basicdata.h"
#include "gfx_meshmod/vertex/position.h"
#include "gfx_meshmod/edge/halfedge.h"
#include "gfx_meshmod/polygon/tribrep.h"
#include "gfx_meshmod/data/aabb.h"
#include "gfx_meshmod/basicalgos.h"

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
	CADT_VectorHandle posVec = MeshMod_DataContainerMutableLookup(vhandle, MeshMod_VertexPositionTag);
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
TEST_CASE("Mesh vertex interpolation", "[MeshMod Mesh]") {
	MeshMod_MeshHandle handle = MeshMod_MeshCreateWithDefaultRegistry("Test Mesh");
	REQUIRE(handle);

	MeshMod_DataContainerHandle vhandle = MeshMod_MeshGetVertices(handle);
	REQUIRE(MeshMod_DataContainerAdd(vhandle, MeshMod_VertexVec2FTag));
	REQUIRE(sizeof(Math_Vec2F_t) == sizeof(MeshMod_VertexVec2F));

	MeshMod_DataContainerResize(vhandle, 3);
	CADT_VectorHandle posVec = MeshMod_DataContainerMutableLookup(vhandle, MeshMod_VertexVec2FTag);
	Math_Vec2F_t* poses = (Math_Vec2F_t*)CADT_VectorAt(posVec, 0);
	REQUIRE(poses);
	Math_Vec2F_t const a = { 0.0f, 0.0f };
	Math_Vec2F_t const b = { 0.0f, 1.0f };
	Math_Vec2F_t const c = { 1.0f, 0.0f };
	*(poses + 0) = a;
	*(poses + 1) = b;
	*(poses + 2) = c;
	REQUIRE(Math_ApproxEqualVec2F(a, *(poses + 0), 1e-5f));
	REQUIRE(Math_ApproxEqualVec2F(b, *(poses + 1), 1e-5f));
	REQUIRE(Math_ApproxEqualVec2F(c, *(poses + 2), 1e-5f));

	// in future it might pick an invalid entry to use however for this purpose
	// it will always add to the end of the array
	REQUIRE(MeshMod_DataContainerVertexInterpolate1D(vhandle, 0, 1, 0.0f) == 3);
	REQUIRE(MeshMod_DataContainerVertexInterpolate1D(vhandle, 0, 1, 1.0f) == 4);
	REQUIRE(MeshMod_DataContainerVertexInterpolate2D(vhandle, 0, 1, 2, 0.0f, 0.0f) == 5);
	REQUIRE(MeshMod_DataContainerVertexInterpolate2D(vhandle, 0, 1, 2, 1.0f, 0.0f) == 6);
	REQUIRE(MeshMod_DataContainerVertexInterpolate2D(vhandle, 0, 1, 2, 0.0f, 1.0f) == 7);
	Math_Vec2F_t* result = (Math_Vec2F_t*)CADT_VectorAt(posVec, 0);
	REQUIRE(result);
	REQUIRE(Math_ApproxEqualVec2F(a, result[3], 1e-5f));
	REQUIRE(Math_ApproxEqualVec2F(b, result[4], 1e-5f));
	REQUIRE(Math_ApproxEqualVec2F(a, result[5], 1e-5f));
	REQUIRE(Math_ApproxEqualVec2F(b, result[6], 1e-5f));
	REQUIRE(Math_ApproxEqualVec2F(c, result[7], 1e-5f));

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

TEST_CASE("Mesh Position Extents", "[MeshMod Mesh]") {
	MeshMod_MeshHandle handle = MeshMod_MeshCreateWithDefaultRegistry("Test Mesh");
	REQUIRE(handle);

	MeshMod_DataContainerHandle vhandle = MeshMod_MeshGetVertices(handle);
	REQUIRE(MeshMod_DataContainerAdd(vhandle, MeshMod_VertexPositionTag));
	REQUIRE_FALSE(MeshMod_DataContainerAdd(vhandle, MeshMod_VertexPositionTag));

	Math_Vec3F_t tri[3] = {
		-1.0f, -2.0f, -3.0f,
		1.0f, 2.0f, 3.0f,
		-1.0f, 4.0f, -3.0f,
	};
	ASSERT(sizeof(Math_Vec3F_t) == sizeof(MeshMod_VertexPosition));
	CADT_VectorHandle posv = MeshMod_DataContainerMutableLookup(vhandle, MeshMod_VertexPositionTag);
	MeshMod_DataContainerResize(vhandle, 3);
	Math_Vec3F_t* data = (MeshMod_VertexPosition*)CADT_VectorData(posv);
	memcpy(data, tri, sizeof(Math_Vec3F_t) * 3);

	MeshMod_DataAabb3F const* aabb = MeshMod_MeshComputeExtents3F(handle, MeshMod_VertexPositionTag);
	REQUIRE(aabb->dataTag == MESHMOD_HASHTAG(MeshMod_VertexPositionTag));
	REQUIRE(aabb->hash != 0);
	REQUIRE(aabb->aabb.minExtent.x == Approx(-1.0f));
	REQUIRE(aabb->aabb.minExtent.y == Approx(-2.0f));
	REQUIRE(aabb->aabb.minExtent.z == Approx(-3.0f));
	REQUIRE(aabb->aabb.maxExtent.x == Approx(1.0f));
	REQUIRE(aabb->aabb.maxExtent.y == Approx(4.0f));
	REQUIRE(aabb->aabb.maxExtent.z == Approx(3.0f));
	uint64_t const hash = aabb->hash;
	aabb = MeshMod_MeshComputeExtents3F(handle, MeshMod_VertexPositionTag);
	REQUIRE(aabb->hash == hash);

	Math_Vec3F_t tri2[3] = {
	-10.0f, -2.0f, -30.0f,
	10.0f, 2.0f, 30.0f,
	-10.0f, 4.0f, -30.0f,
	};
	MeshMod_DataContainerResize(vhandle, 6);
	data = (MeshMod_VertexPosition*)CADT_VectorData(posv);
	memcpy(data + 3, tri2, sizeof(Math_Vec3F_t) * 3);
	aabb = MeshMod_MeshComputeExtents3F(handle, MeshMod_VertexPositionTag);
	REQUIRE(aabb->dataTag == MESHMOD_HASHTAG(MeshMod_VertexPositionTag));
	REQUIRE(aabb->hash != hash);
	REQUIRE(aabb->aabb.minExtent.x == Approx(-10.0f));
	REQUIRE(aabb->aabb.minExtent.y == Approx(-2.0f));
	REQUIRE(aabb->aabb.minExtent.z == Approx(-30.0f));
	REQUIRE(aabb->aabb.maxExtent.x == Approx(10.0f));
	REQUIRE(aabb->aabb.maxExtent.y == Approx(4.0f));
	REQUIRE(aabb->aabb.maxExtent.z == Approx(30.0f));
}