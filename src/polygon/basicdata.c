#include "al2o3_platform/platform.h"
#include "render_meshmod/polygon/basicdata.h"
#include "render_meshmod/registry.h"

#define PT_REAL(postfix, type) \
static char const* Polygon##postfix##Description() { \
	return "Polygon"#postfix; \
}; \
void const* MeshMod_Polygon##postfix##DefaultData() { \
	static type const nan = NAN; \
	return &nan; \
}; \
AL2O3_EXTERN_C bool MeshMod_Polygon##postfix##Equal(void const* va, void const* vb, float const epsilon) { \
	MeshMod_Polygon##postfix const* a = (MeshMod_Polygon##postfix const*)va; \
	MeshMod_Polygon##postfix const* b = (MeshMod_Polygon##postfix const*)vb; \
	return Math_ApproxEqual##postfix(*a, *b, epsilon); \
}; \
AL2O3_EXTERN_C double MeshMod_Polygon##postfix##Distance(void const* va, void const* vb) { \
	MeshMod_Polygon##postfix const* a = (MeshMod_Polygon##postfix const*)va; \
	MeshMod_Polygon##postfix const* b = (MeshMod_Polygon##postfix const*)vb; \
	return (double)*a - (double)*b; \
}; \
AL2O3_EXTERN_C void MeshMod_Polygon##postfix##AddToRegistry(MeshMod_RegistryHandle handle) { \
	static MeshMod_RegistryCommonFunctionTable CommonFunctionTable = { \
		.defaultDataFunc = &MeshMod_Polygon##postfix##DefaultData, \
		.equalFunc = &MeshMod_Polygon##postfix##Equal, \
		.destroyFunc = NULL,	\
		.descriptionFunc = &Polygon##postfix##Description, \
		.distanceFunc = &MeshMod_Polygon##postfix##Distance, \
	}; \
	static MeshMod_RegistryPolygonFunctionTable PolygonFunctionTable = { \
		0 \
	}; \
	MeshMod_RegistryAddType(handle, \
		MeshMod_Polygon##postfix##Tag.tag, \
		sizeof(MeshMod_Polygon##postfix), \
		&CommonFunctionTable, \
		&PolygonFunctionTable); \
}

#define PT_UNSIGNED(postfix, type) \
static char const* Polygon##postfix##Description() { \
	return "Polygon"#postfix; \
}; \
void const* MeshMod_Polygon##postfix##DefaultData() { \
	static type const invalid = (type)~0ull; \
	return &invalid; \
}; \
AL2O3_EXTERN_C bool MeshMod_Polygon##postfix##Equal(void const* va, void const* vb, float const epsilon) { \
	MeshMod_Polygon##postfix const* a = (MeshMod_Polygon##postfix const*)va; \
	MeshMod_Polygon##postfix const* b = (MeshMod_Polygon##postfix const*)vb; \
	return *a == *b; \
}; \
AL2O3_EXTERN_C double MeshMod_Polygon##postfix##Distance(void const* va, void const* vb) { \
	MeshMod_Polygon##postfix const* a = (MeshMod_Polygon##postfix const*)va; \
	MeshMod_Polygon##postfix const* b = (MeshMod_Polygon##postfix const*)vb; \
	return (double)(*a - *b); \
}; \
AL2O3_EXTERN_C void MeshMod_Polygon##postfix##AddToRegistry(MeshMod_RegistryHandle handle) { \
	static MeshMod_RegistryCommonFunctionTable CommonFunctionTable = { \
		.defaultDataFunc = &MeshMod_Polygon##postfix##DefaultData, \
		.equalFunc = &MeshMod_Polygon##postfix##Equal, \
		.destroyFunc = NULL,	\
		.descriptionFunc = &Polygon##postfix##Description, \
		.distanceFunc = &MeshMod_Polygon##postfix##Distance, \
	}; \
	static MeshMod_RegistryPolygonFunctionTable PolygonFunctionTable = { \
		0 \
	}; \
	MeshMod_RegistryAddType(handle, \
		MeshMod_Polygon##postfix##Tag.tag, \
		sizeof(MeshMod_Polygon##postfix), \
		&CommonFunctionTable, \
		&PolygonFunctionTable); \
}

static void const* PolygonVec4FDefaultData() {
	static Math_Vec4F nan = { NAN, NAN, NAN, NAN };
	return &nan;
}
static void const* PolygonVec4DDefaultData() {
	static Math_Vec4D nan = { NAN, NAN, NAN, NAN };
	return &nan;
}
static void const* PolygonVec2FDefaultData() { return PolygonVec4FDefaultData(); }
static void const* PolygonVec3FDefaultData() { return PolygonVec4FDefaultData(); }
static void const* PolygonVec2DDefaultData() { return PolygonVec4DDefaultData(); }
static void const* PolygonVec3DDefaultData() { return PolygonVec4DDefaultData(); }

#define PT_VECTOR_REAL(postfix, count) \
AL2O3_EXTERN_C bool MeshMod_Polygon##postfix##Equal(void const* va, void const* vb, float const epsilon) { \
	MeshMod_Polygon##postfix const* a = (MeshMod_Polygon##postfix const*)va; \
	MeshMod_Polygon##postfix const* b = (MeshMod_Polygon##postfix const*)vb; \
	return Math_ApproxEqual##postfix(*a, *b, epsilon); \
} \
AL2O3_EXTERN_C double MeshMod_Polygon##postfix##Distance(void const* va, void const* vb) { \
	MeshMod_Polygon##postfix const* a = (MeshMod_Polygon##postfix const*)va; \
	MeshMod_Polygon##postfix const* b = (MeshMod_Polygon##postfix const*)vb; \
	return (double)Math_Dot##postfix(*a, *b); \
} \
static char const* Polygon##postfix##Description() { \
	return "Polygon"#postfix; \
} \
AL2O3_EXTERN_C void MeshMod_Polygon##postfix##AddToRegistry(MeshMod_RegistryHandle handle) { \
	static MeshMod_RegistryCommonFunctionTable CommonFunctionTable = { \
		.defaultDataFunc = &Polygon##postfix##DefaultData, \
		.equalFunc = &MeshMod_Polygon##postfix##Equal, \
		.destroyFunc = NULL, \
		.descriptionFunc = &Polygon##postfix##Description, \
		.distanceFunc = &MeshMod_Polygon##postfix##Distance, \
	}; \
	static MeshMod_RegistryPolygonFunctionTable PolygonFunctionTable = { \
		0 \
	}; \
	MeshMod_RegistryAddType(handle, \
		MeshMod_Polygon##postfix##Tag.tag, \
		sizeof(MeshMod_Polygon##postfix), \
		&CommonFunctionTable, \
		&PolygonFunctionTable); \
}

PT_UNSIGNED(U8, uint8_t)
PT_UNSIGNED(U16, uint16_t)
PT_UNSIGNED(U32, uint32_t)
PT_UNSIGNED(U64, uint64_t)

PT_REAL(F, float);
PT_REAL(D, double);
PT_VECTOR_REAL(Vec2F, 2);
PT_VECTOR_REAL(Vec3F, 3);
PT_VECTOR_REAL(Vec4F, 4);
PT_VECTOR_REAL(Vec2D, 2);
PT_VECTOR_REAL(Vec3D, 3);
PT_VECTOR_REAL(Vec4D, 4);

