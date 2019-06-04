#include "al2o3_platform/platform.h"
#include "al2o3_memory/memory.h"
#include "al2o3_cadt/vector.h"
#include "al2o3_cadt/dict.h"
#include "gfx_meshmod/registry.h"
#include <stddef.h>

extern void MeshMod_AddVertexDefaultsToTagRegistry(MeshMod_RegistryHandle handle);
extern void MeshMod_AddEdgeDefaultsToTagRegistry(MeshMod_RegistryHandle handle);
extern void MeshMod_AddPolygonDefaultsToTagRegistry(MeshMod_RegistryHandle handle);

typedef struct MeshMod_RegistryItem {
	MeshMod_Type type;
	size_t elementSize;
	char const* description;

	MeshMod_RegistryCommonFunctionTable* commonFunctionTable;

	union {
		MeshMod_RegistryVertexFunctionTable* vertexFunctionTable;
		MeshMod_RegistryEdgeFunctionTable* edgeFunctionTable;
		MeshMod_RegistryPolygonFunctionTable* polygonFunctionTable;
	};

} MeshMod_RegistryItem;

typedef struct MeshMod_Registry {
	CADT_VectorHandle registry;
	CADT_DictU64Handle tagDictionary;
} MeshMod_Registry;

static MeshMod_RegistryItem* LookupRegistryItem(MeshMod_RegistryHandle handle, MeshMod_Tag tag) {
	ASSERT(handle);
	MeshMod_Registry* reg = (MeshMod_Registry*)handle;
	ASSERT(CADT_DictU64KeyExists(reg->tagDictionary, tag & ~0xFFU));

	uint64_t index = 0;
	bool okay = CADT_DictU64Lookup(reg->tagDictionary, tag & ~0xFFU, &index);
	if (!okay)
		return 0;

	ASSERT(index < CADT_VectorSize(reg->registry));
	return CADT_VectorAt(reg->registry, index);
}

AL2O3_EXTERN_C MeshMod_RegistryHandle MeshMod_RegistryCreate() {
	MeshMod_Registry* reg = MEMORY_CALLOC(1, sizeof(MeshMod_Registry));
	if (reg == NULL)
		return NULL;

	reg->registry = CADT_VectorCreate(sizeof(MeshMod_RegistryItem));
	if (reg->registry == NULL) {
		MEMORY_FREE(reg);
		return NULL;
	}
	reg->tagDictionary = CADT_DictU64Create();
	if (reg->tagDictionary == NULL) {
		CADT_VectorDestroy(reg->registry);
		MEMORY_FREE(reg);
	}

	return reg;
}

AL2O3_EXTERN_C MeshMod_RegistryHandle MeshMod_RegistryCreateWithDefaults() {
	MeshMod_RegistryHandle handle = MeshMod_RegistryCreate();

	MeshMod_AddVertexDefaultsToTagRegistry(handle);
	MeshMod_AddEdgeDefaultsToTagRegistry(handle);
	MeshMod_AddPolygonDefaultsToTagRegistry(handle);

	return handle;
}


AL2O3_EXTERN_C void MeshMod_RegistryDestroy(MeshMod_RegistryHandle handle) {
	ASSERT(handle);
	MeshMod_Registry* reg = (MeshMod_Registry*)handle;

	CADT_DictU64Destroy(reg->tagDictionary);
	CADT_VectorDestroy(reg->registry);
	MEMORY_FREE(reg);
}

AL2O3_EXTERN_C void MeshMod_RegistryAddVertexType(MeshMod_RegistryHandle handle,
																									MeshMod_Tag tag,
																									size_t elementSize,
																									MeshMod_RegistryCommonFunctionTable* commonFunctionTable,
																									MeshMod_RegistryVertexFunctionTable* vertexFunctionTable) {
	ASSERT(handle);
	MeshMod_Registry* reg = (MeshMod_Registry*)handle;

	MeshMod_RegistryItem item;
	item.type = MeshMod_TypeVertex;
	item.elementSize = elementSize;
	item.commonFunctionTable = commonFunctionTable;
	item.vertexFunctionTable = vertexFunctionTable;

	size_t index = CADT_VectorPushElement(reg->registry, &item);
	bool okay = CADT_DictU64Add(reg->tagDictionary, tag & ~0xFFU, (uint32_t)index);
	ASSERT(okay);
}

AL2O3_EXTERN_C void MeshMod_RegistryAddEdgeType(MeshMod_RegistryHandle handle,
																								MeshMod_Tag tag,
																								size_t elementSize,
																								MeshMod_RegistryCommonFunctionTable* commonFunctionTable,
																								MeshMod_RegistryEdgeFunctionTable* edgeFunctionTable) {
	ASSERT(handle);
	MeshMod_Registry* reg = (MeshMod_Registry*)handle;

	MeshMod_RegistryItem item;
	item.type = MeshMod_TypeEdge;
	item.elementSize = elementSize;
	item.commonFunctionTable = commonFunctionTable;
	item.edgeFunctionTable = edgeFunctionTable;

	size_t index = CADT_VectorPushElement(reg->registry, &item);
	bool okay = CADT_DictU64Add(reg->tagDictionary, tag & ~0xFFU, (uint32_t)index);
	ASSERT(okay);
}

AL2O3_EXTERN_C void MeshMod_RegistryAddPolygonType(	MeshMod_RegistryHandle handle,
																										MeshMod_Tag tag,
																										size_t elementSize,
																										MeshMod_RegistryCommonFunctionTable* commonFunctionTable,
																										MeshMod_RegistryPolygonFunctionTable* polygonFunctionTable) {
	ASSERT(handle);
	MeshMod_Registry* reg = (MeshMod_Registry*)handle;

	MeshMod_RegistryItem item;
	item.type = MeshMod_TypePolygon;
	item.elementSize = elementSize;
	item.commonFunctionTable = commonFunctionTable;
	item.polygonFunctionTable = polygonFunctionTable;

	size_t index = CADT_VectorPushElement(reg->registry, &item);
	bool okay = CADT_DictU64Add(reg->tagDictionary, tag & ~0xFFU, (uint32_t)index);
	ASSERT(okay);
}

AL2O3_EXTERN_C bool MeshMod_RegistryExists(MeshMod_RegistryHandle handle, MeshMod_Tag tag) {
	ASSERT(handle);
	MeshMod_Registry* reg = (MeshMod_Registry*)handle;
	return CADT_DictU64KeyExists(reg->tagDictionary, tag & ~0xFFU);
}

AL2O3_EXTERN_C size_t MeshMod_RegistryElementSize(MeshMod_RegistryHandle handle, MeshMod_Tag tag) {
	MeshMod_RegistryItem* item = LookupRegistryItem(handle, tag);
	return item->elementSize;
}

AL2O3_EXTERN_C MeshMod_Type MeshMod_RegistryType(MeshMod_RegistryHandle handle, MeshMod_Tag tag) {
	MeshMod_RegistryItem* item = LookupRegistryItem(handle, tag);
	return item->type;
}

AL2O3_EXTERN_C char const* MeshMod_RegistryDescription(MeshMod_RegistryHandle handle, MeshMod_Tag tag) {
	MeshMod_RegistryItem* item = LookupRegistryItem(handle, tag);
	if (item->commonFunctionTable->descriptionFunc)
		return item->commonFunctionTable->descriptionFunc();
	else return "No Description";
}

AL2O3_EXTERN_C void const* MeshMod_RegistryDefaultData(MeshMod_RegistryHandle handle, MeshMod_Tag tag) {
	MeshMod_RegistryItem* item = LookupRegistryItem(handle, tag);
	return item->commonFunctionTable->defaultDataFunc();
}
