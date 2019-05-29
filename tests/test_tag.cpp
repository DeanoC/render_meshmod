#include "al2o3_catch2/catch2.hpp"
#include "gfx_meshmod/tag.h"

TEST_CASE("TagRegistry create/destroy", "[MeshMod Tag]") {
	MeshMod_TagRegistryHandle handle = MeshMod_TagRegistryCreate();
	REQUIRE(handle);
	MeshMod_TagRegistryDestroy(handle);
}

static const MeshMod_Tag TestTag = MESHMOD_TAG('T','E','S','T', '0', '0', '0', 0);

TEST_CASE("TagRegistry add/exists", "[MeshMod Tag]") {
	MeshMod_TagRegistryHandle handle = MeshMod_TagRegistryCreate();
	REQUIRE(handle);

	MeshMod_TagRegistryAddTag(handle,
														TestTag | '0',
														10, "Test Description");
	REQUIRE( MeshMod_TagRegistryTagExists(handle, TestTag | '0' ));
	MeshMod_TagRegistryAddTag(handle,
														TestTag | '1',
														20, "Test Description again");
	REQUIRE( MeshMod_TagRegistryTagExists(handle, TestTag | '1' ));
	MeshMod_TagRegistryDestroy(handle);
}

TEST_CASE("TagRegistry lookup", "[MeshMod Tag]") {
	MeshMod_TagRegistryHandle handle = MeshMod_TagRegistryCreate();
	REQUIRE(handle);

	MeshMod_TagRegistryAddTag(handle,
														TestTag | '0',
														10, "Test Description");
	REQUIRE(MeshMod_TagRegistryDataSize(handle, TestTag | '0' ) == 10);
	REQUIRE(strcmp(MeshMod_TagRegistryDescription(handle, TestTag | '0' ), "Test Description") == 0);

	MeshMod_TagRegistryAddTag(handle,
														TestTag | '1',
														20, "Test Description again");
	REQUIRE(MeshMod_TagRegistryDataSize(handle, TestTag | '0' ) == 10);
	REQUIRE(strcmp(MeshMod_TagRegistryDescription(handle, TestTag | '0' ), "Test Description") == 0);
	REQUIRE(MeshMod_TagRegistryDataSize(handle, TestTag | '1' ) == 20);
	REQUIRE(strcmp(MeshMod_TagRegistryDescription(handle, TestTag | '1' ), "Test Description again") == 0);

	MeshMod_TagRegistryDestroy(handle);
}
