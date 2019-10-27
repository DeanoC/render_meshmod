#include "al2o3_catch2/catch2.hpp"
#include "render_meshmod/registry.h"
#include "render_meshmod/vertex/position.h"
#include "render_meshmod/edge/halfedge.h"
#include "render_meshmod/polygon/tribrep.h"

TEST_CASE("TagRegistry create/destroy", "[MeshMod Tag]") {
	MeshMod_RegistryHandle handle = MeshMod_RegistryCreate();
	REQUIRE(MeshMod_RegistryHandleIsValid(handle));
	MeshMod_RegistryDestroy(handle);
}
AL2O3_EXTERN_C MeshMod_RegistryCommonFunctionTable MeshMod_VertexPositionCommonFunctionTable;
AL2O3_EXTERN_C MeshMod_RegistryVertexFunctionTable MeshMod_VertexPositionVertexFunctionTable;

AL2O3_EXTERN_C void MeshMod_VertexPositionAddToRegistry(MeshMod_RegistryHandle handle);

TEST_CASE("TagRegistry add/exists", "[MeshMod Tag]") {
	MeshMod_RegistryHandle handle = MeshMod_RegistryCreate();
	REQUIRE(MeshMod_RegistryHandleIsValid(handle));

	MeshMod_VertexPositionAddToRegistry(handle);
	REQUIRE(MeshMod_RegistryExists(handle, MeshMod_VertexPositionTag.tag));
	REQUIRE(MeshMod_RegistryType(handle, MeshMod_VertexPositionTag.tag) == MeshMod_TypeVertex);
	MeshMod_RegistryDestroy(handle);
}
TEST_CASE("TagRegistry exists with defaults", "[MeshMod Tag]") {
	MeshMod_RegistryHandle handle = MeshMod_RegistryCreateWithDefaults();
	REQUIRE(MeshMod_RegistryHandleIsValid(handle));

	REQUIRE(MeshMod_RegistryExists(handle, MeshMod_VertexPositionTag.tag));
	REQUIRE(MeshMod_RegistryType(handle, MeshMod_VertexPositionTag.tag) == MeshMod_TypeVertex);
	REQUIRE(MeshMod_RegistryExists(handle, MeshMod_EdgeHalfEdgeTag.tag));
	REQUIRE(MeshMod_RegistryType(handle, MeshMod_EdgeHalfEdgeTag.tag) == MeshMod_TypeEdge);
	REQUIRE(MeshMod_RegistryExists(handle, MeshMod_PolygonTriBRepTag.tag));
	REQUIRE(MeshMod_RegistryType(handle, MeshMod_PolygonTriBRepTag.tag) == MeshMod_TypePolygon);

	MeshMod_RegistryDestroy(handle);
}

TEST_CASE("TagRegistry lookup", "[MeshMod Tag]") {
	MeshMod_RegistryHandle handle = MeshMod_RegistryCreateWithDefaults();
	REQUIRE(MeshMod_RegistryHandleIsValid(handle));

	REQUIRE(MeshMod_RegistryElementSize(handle, MeshMod_VertexPositionTag.tag) == sizeof(MeshMod_VertexPosition));
	REQUIRE(strcmp(MeshMod_RegistryDescription(handle, MeshMod_VertexPositionTag.tag), "Vertex Position") == 0);

	MeshMod_RegistryDestroy(handle);
}
