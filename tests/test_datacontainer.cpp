#include "al2o3_catch2/catch2.hpp"
#include "gfx_meshmod/registry.h"
#include "gfx_meshmod/datacontainer.h"
#include "gfx_meshmod/vertex/position.h"

TEST_CASE("DataContainer create/destroy", "[MeshMod DataContainer]") {

	MeshMod_DataContainerHandle vhandle = MeshMod_DataContainerCreate(NULL, MeshMod_TypeVertex);
	REQUIRE(vhandle);
	MeshMod_DataContainerDestroy(vhandle);

	MeshMod_DataContainerHandle ehandle = MeshMod_DataContainerCreate(NULL, MeshMod_TypeEdge);
	REQUIRE(ehandle);
	MeshMod_DataContainerDestroy(ehandle);

	MeshMod_DataContainerHandle phandle = MeshMod_DataContainerCreate(NULL, MeshMod_TypePolygon);
	REQUIRE(phandle);
	MeshMod_DataContainerDestroy(phandle);

}
