#include "al2o3_platform/platform.h"
#include "render_meshmod/polygon/planeequation.h"
#include "render_meshmod/registry.h"
#include "render_meshmod/polygon/basicdata.h"

// borrow from basic data
AL2O3_EXTERN_C bool MeshMod_PolygonVec4FEqual(void const* va, void const* vb, float const epsilon);
AL2O3_EXTERN_C double MeshMod_PolygonVec4FDistance(void const* va, void const* vb);

static void const* PolygonPlaneEquationDefaultData() {
	static MeshMod_PolygonPlaneEquation const invalid = { {NAN, NAN, NAN, NAN} };
	return &invalid;
}

static AL2O3_EXTERN_C char const* PolygonPlaneEquationDescription() {
	return "Plane equation of the polygon";
}


AL2O3_EXTERN_C void MeshMod_PolygonPlaneEquationAddToRegistry(MeshMod_RegistryHandle handle) {

	static MeshMod_RegistryCommonFunctionTable CommonFunctionTable = {
			.defaultDataFunc = &PolygonPlaneEquationDefaultData,
			.equalFunc = &MeshMod_PolygonVec4FEqual,
			.destroyFunc = NULL,
			.descriptionFunc = &PolygonPlaneEquationDescription,
			.distanceFunc = MeshMod_PolygonVec4FDistance
	};
	static MeshMod_RegistryPolygonFunctionTable PolygonFunctionTable = {
			0
	};

	MeshMod_RegistryAddType(	handle,
														MeshMod_PolygonPlaneEquationTag.tag,
														sizeof(MeshMod_PolygonPlaneEquation),
														&CommonFunctionTable,
														&PolygonFunctionTable);
}


