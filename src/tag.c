#include "al2o3_platform/platform.h"
#include "al2o3_memory/memory.h"
#include "al2o3_cadt/vector.h"
#include "al2o3_cadt/dict.h"
#include "gfx_meshmod/tag.h"

typedef struct MeshMod_TagRegistryItem {
	size_t dataSize;
	char const *description;
} MeshMod_TagRegistryItem;

typedef struct MeshMod_TagRegistry {
	CADT_VectorHandle registry;
	CADT_DictU64Handle tagDictionary;
} MeshMod_TagRegistry;

AL2O3_EXTERN_C MeshMod_TagRegistryHandle MeshMod_TagRegistryCreate() {
	MeshMod_TagRegistry *reg = MEMORY_CALLOC(1, sizeof(MeshMod_TagRegistry));
	if (reg == NULL)
		return NULL;

	reg->registry = CADT_VectorCreate(sizeof(MeshMod_TagRegistryItem));
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

AL2O3_EXTERN_C void MeshMod_TagRegistryDestroy(MeshMod_TagRegistryHandle handle) {
	ASSERT(handle);
	MeshMod_TagRegistry *reg = (MeshMod_TagRegistry *) handle;

	for (size_t i = 0; i < CADT_VectorSize(reg->registry); ++i) {
		MeshMod_TagRegistryItem *item = CADT_VectorElement(reg->registry, i);
		MEMORY_FREE((void *) item->description);
	}

	CADT_DictU64Destroy(reg->tagDictionary);
	CADT_VectorDestroy(reg->registry);
	MEMORY_FREE(reg);
}

AL2O3_EXTERN_C void MeshMod_TagRegistryAddTag(MeshMod_TagRegistryHandle handle,
																							MeshMod_Tag tag,
																							size_t dataSize,
																							char const *description) {
	ASSERT(handle);
	MeshMod_TagRegistry *reg = (MeshMod_TagRegistry *) handle;

	char *str = (char *) MEMORY_MALLOC(strlen(description) + 1);
	strcpy(str, description);

	MeshMod_TagRegistryItem item = {
			dataSize,
			str
	};

	size_t index = CADT_VectorPushElement(reg->registry, &item);
	bool okay = CADT_DictU64Add(reg->tagDictionary, tag & ~0xFFU, (uint32_t) index);
	ASSERT(okay);
}
AL2O3_EXTERN_C bool MeshMod_TagRegistryTagExists(MeshMod_TagRegistryHandle handle, MeshMod_Tag tag) {
	ASSERT(handle);
	MeshMod_TagRegistry *reg = (MeshMod_TagRegistry *) handle;
	return CADT_DictU64KeyExists(reg->tagDictionary, tag & ~0xFFU);
}

AL2O3_EXTERN_C size_t MeshMod_TagRegistryDataSize(MeshMod_TagRegistryHandle handle, MeshMod_Tag tag) {
	ASSERT(handle);
	MeshMod_TagRegistry *reg = (MeshMod_TagRegistry *) handle;
	ASSERT(CADT_DictU64KeyExists(reg->tagDictionary, tag & ~0xFFU));

	uint64_t index = 0;
	bool okay = CADT_DictU64Lookup(reg->tagDictionary, tag & ~0xFFU, &index);
	if (!okay)
		return 0;

	ASSERT(index < CADT_VectorSize(reg->registry));
	MeshMod_TagRegistryItem *item = CADT_VectorElement(reg->registry, index);
	return item->dataSize;
}
AL2O3_EXTERN_C char const *MeshMod_TagRegistryDescription(MeshMod_TagRegistryHandle handle,
																													MeshMod_Tag tag) {
	ASSERT(handle);
	MeshMod_TagRegistry *reg = (MeshMod_TagRegistry *) handle;
	ASSERT(CADT_DictU64KeyExists(reg->tagDictionary, tag & ~0xFFU));

	uint64_t index = 0;
	bool okay = CADT_DictU64Lookup(reg->tagDictionary, tag & ~0xFFU, &index);
	if (!okay)
		return NULL;

	ASSERT(index < CADT_VectorSize(reg->registry));
	MeshMod_TagRegistryItem *item = CADT_VectorElement(reg->registry, index);
	return item->description;
}
