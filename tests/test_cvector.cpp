#include "al2o3_catch2/catch2.hpp"
#include "gfx_meshmod/cvector.h"

TEST_CASE("CVector create/destroy", "[MeshMod CVector]") {
	MeshMod_CVectorHandle handle = MeshMod_CVectorCreate(10);
	REQUIRE(handle);
	MeshMod_CVectorDestroy(handle);
}

TEST_CASE("CVector Initial Gets", "[MeshMod CVector]") {
	MeshMod_CVectorHandle handle = MeshMod_CVectorCreate(10);
	REQUIRE(handle);
	REQUIRE( MeshMod_CVectorCapacityFrom(handle) == 0);
	REQUIRE( MeshMod_CVectorSizeFrom(handle) >= 0);
	REQUIRE( MeshMod_CVectorDataFrom(handle) == NULL);
	MeshMod_CVectorDestroy(handle);
}

TEST_CASE("CVector Resizes", "[MeshMod CVector]") {
	MeshMod_CVectorHandle handle = MeshMod_CVectorCreate(10);
	REQUIRE(handle);
	MeshMod_CVectorResize(handle, 10);
	REQUIRE( MeshMod_CVectorSizeFrom(handle) == 10);
	REQUIRE( MeshMod_CVectorCapacityFrom(handle) >= 10);
	REQUIRE( MeshMod_CVectorDataFrom(handle) != NULL);
	MeshMod_CVectorResize(handle, 5);
	REQUIRE( MeshMod_CVectorSizeFrom(handle) == 5);
	REQUIRE( MeshMod_CVectorCapacityFrom(handle) >= 10);
	MeshMod_CVectorResize(handle, 15);
	REQUIRE( MeshMod_CVectorSizeFrom(handle) == 15);
	REQUIRE( MeshMod_CVectorCapacityFrom(handle) >= 15);

	MeshMod_CVectorDestroy(handle);
}
TEST_CASE("CVector Push/Pop/Peek", "[MeshMod CVector]") {
	MeshMod_CVectorHandle handle = MeshMod_CVectorCreate(10);
	REQUIRE(handle);
	uint8_t e[10] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };
	MeshMod_CVectorPushElement(handle, e);
	REQUIRE( MeshMod_CVectorSizeFrom(handle) == 1);
	REQUIRE( MeshMod_CVectorCapacityFrom(handle) >= 1);
	REQUIRE( memcmp(MeshMod_CVectorPeekElementFrom(handle), e, 10) == 0);
	uint8_t r[10];
	MeshMod_CVectorPopElementFrom(handle, r);
	REQUIRE( MeshMod_CVectorSizeFrom(handle) == 0);
	REQUIRE( MeshMod_CVectorCapacityFrom(handle) >= 1);
	REQUIRE( memcmp(r, e, 10) == 0);

	MeshMod_CVectorDestroy(handle);
}

TEST_CASE("CVector ElementFrom", "[MeshMod CVector]") {
	MeshMod_CVectorHandle handle = MeshMod_CVectorCreate(10);
	REQUIRE(handle);
	uint8_t e0[10] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };
	uint8_t e1[10] = { 10, 11, 12, 13, 14, 15, 16, 17, 18, 19 };

	REQUIRE(MeshMod_CVectorPushElement(handle, e0) == 0);
	REQUIRE(MeshMod_CVectorPushElement(handle, e1) == 1);

	uint8_t* e0p = (uint8_t*)MeshMod_CVectorElementFrom(handle, 0);
	REQUIRE(memcmp(e0p, e0, 10) == 0);
	uint8_t* e1p = (uint8_t*)MeshMod_CVectorElementFrom(handle, 1);
	REQUIRE(memcmp(e1p, e1, 10) == 0);

	MeshMod_CVectorResize(handle, 1);
	e0p = (uint8_t*)MeshMod_CVectorElementFrom(handle, 0);
	REQUIRE(memcmp(e0p, e0, 10) == 0);
	MeshMod_CVectorPopAndDiscardElementFrom(handle);
	REQUIRE(MeshMod_CVectorSizeFrom(handle) == 0);

	MeshMod_CVectorDestroy(handle);

}

TEST_CASE("CVector IsEmpty", "[MeshMod CVector]") {
	MeshMod_CVectorHandle handle = MeshMod_CVectorCreate(10);
	REQUIRE(handle);
	REQUIRE(MeshMod_CVectorIsEmpty(handle));
	uint8_t e[10] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };
	MeshMod_CVectorPushElement(handle, e);
	REQUIRE(!MeshMod_CVectorIsEmpty(handle));
	MeshMod_CVectorPushElement(handle, e);
	REQUIRE(!MeshMod_CVectorIsEmpty(handle));
	MeshMod_CVectorPopAndDiscardElementFrom(handle);
	MeshMod_CVectorPopAndDiscardElementFrom(handle);
	REQUIRE(MeshMod_CVectorIsEmpty(handle));

}