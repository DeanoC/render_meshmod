#include "al2o3_catch2/catch2.hpp"
#include "gfx_meshmod/cbagofvectors.h"

TEST_CASE("MeshMod_CBagOfVectors create/destroy", "[MeshMod MeshMod_CBagOfVectors]") {
	MeshMod_CBagOfVectorsHandle handle = MeshMod_CBagOfVectorsCreate();
	REQUIRE(handle);
	MeshMod_CBagOfVectorsDestroy(handle);
}

static const uint64_t TestTag = 0xDCDCDCDCDCDCDCD0U;

TEST_CASE("MeshMod_CBagOfVectors add/exists", "[MeshMod MeshMod_CBagOfVectors]") {
	MeshMod_CBagOfVectorsHandle handle = MeshMod_CBagOfVectorsCreate();
	REQUIRE(handle);

	MeshMod_CBagOfVectorsAdd(handle,
														TestTag | '0',
														10);
	REQUIRE( MeshMod_CBagOfVectorsKeyExists(handle, TestTag | '0' ));
	MeshMod_CBagOfVectorsAdd(handle,
														TestTag | '1',
														20);
	REQUIRE( MeshMod_CBagOfVectorsKeyExists(handle, TestTag | '1' ));

	MeshMod_CBagOfVectorsDestroy(handle);
}
/*
TEST_CASE("MeshMod_CBagOfVectors lookup", "[MeshMod MeshMod_CBagOfVectors]") {
	MeshMod_CBagOfVectorsHandle handle = MeshMod_CBagOfVectorsCreate();
	REQUIRE(handle);

	MeshMod_CBagOfVectorsAdd(handle,
													 TestTag | '0',
													 10);
	MeshMod_CVectorHandle v0 = MeshMod_CBagOfVectorsGet(handle, TestTag | '0');
	REQUIRE(MeshMod_CVectorElementSizeFrom(v0) == 10);
	MeshMod_CBagOfVectorsAdd(handle,
													 TestTag | '1',
													 20);
	MeshMod_CVectorHandle v0a = MeshMod_CBagOfVectorsGet(handle, TestTag | '0');
	MeshMod_CVectorHandle v1a = MeshMod_CBagOfVectorsGet(handle, TestTag | '1');
	REQUIRE(MeshMod_CVectorElementSizeFrom(v0a) == 10);
	REQUIRE(MeshMod_CVectorElementSizeFrom(v1a) == 20);

	MeshMod_CBagOfVectorsDestroy(handle);
}
*/