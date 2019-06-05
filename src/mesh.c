#include "al2o3_platform/platform.h"
#include "al2o3_memory/memory.h"
#include "al2o3_cadt/bagofvectors.h"
#include "al2o3_cadt/dict.h"
#include "gfx_meshmod/datacontainer.h"
#include "gfx_meshmod/mesh.h"

typedef struct MeshMod_Mesh {
	MeshMod_RegistryHandle registry;
	bool ownRegistry;
	char const * name;

	MeshMod_DataContainerHandle vertices;
	MeshMod_DataContainerHandle edges;
	MeshMod_DataContainerHandle polygons;

	CADT_VectorHandle arbitaryDataKeys;
	CADT_DictU64Handle arbitaryData;
	CADT_DictU64Handle arbitaryDataSizes;

} MeshMod_Mesh;

AL2O3_EXTERN_C MeshMod_MeshHandle MeshMod_MeshCreateWithDefaultRegistry(char const* name) {
	MeshMod_RegistryHandle registryHandle = MeshMod_RegistryCreateWithDefaults();
	MeshMod_MeshHandle meshh = MeshMod_MeshCreate(registryHandle, name);
	if (meshh == NULL) {
		MeshMod_RegistryDestroy(registryHandle);
		return NULL;
	}
	MeshMod_Mesh* mesh = (MeshMod_Mesh*)meshh;
	mesh->ownRegistry = true;
	return mesh;
}

AL2O3_EXTERN_C MeshMod_MeshHandle MeshMod_MeshCreate(MeshMod_RegistryHandle registry, char const* name) {
	ASSERT(name);

	MeshMod_Mesh* mesh = (MeshMod_Mesh*)MEMORY_MALLOC(sizeof(MeshMod_Mesh));
	if(mesh == NULL) return NULL;
	mesh->name = MEMORY_MALLOC(strlen(name) + 1);
	if(mesh->name == NULL) return NULL;
	strcpy((char*)mesh->name, name);
	mesh->registry = registry;
	mesh->ownRegistry = false;
	mesh->vertices = MeshMod_DataContainerCreate(mesh, MeshMod_TypeVertex);
	mesh->edges = MeshMod_DataContainerCreate(mesh, MeshMod_TypeEdge);
	mesh->polygons = MeshMod_DataContainerCreate(mesh, MeshMod_TypePolygon);

	mesh->arbitaryDataKeys = CADT_VectorCreate(sizeof(MeshMod_Tag));
	mesh->arbitaryData = CADT_DictU64Create();
	mesh->arbitaryDataSizes = CADT_DictU64Create();

	return mesh;
}

AL2O3_EXTERN_C void MeshMod_MeshDestroy(MeshMod_MeshHandle handle) {
	ASSERT(handle);

	MeshMod_Mesh* mesh = (MeshMod_Mesh*)handle;

	for (size_t i = 0; i < CADT_VectorSize(mesh->arbitaryDataKeys); ++i) {
		MeshMod_Tag tag = *(MeshMod_Tag*)CADT_VectorAt(mesh->arbitaryDataKeys, i);
		MEMORY_FREE((void*)CADT_DictU64Get(mesh->arbitaryData, tag));
	}
	CADT_DictU64Destroy(mesh->arbitaryDataSizes);
	CADT_DictU64Destroy(mesh->arbitaryData);
	CADT_VectorDestroy(mesh->arbitaryDataKeys);

	MeshMod_DataContainerDestroy(mesh->polygons);
	MeshMod_DataContainerDestroy(mesh->edges);
	MeshMod_DataContainerDestroy(mesh->vertices);

	if (mesh->ownRegistry) {
		MeshMod_RegistryDestroy(mesh->registry);
	}

	MEMORY_FREE((void*)mesh->name);
	MEMORY_FREE(mesh);
}

AL2O3_EXTERN_C MeshMod_RegistryHandle MeshMod_MeshGetRegistry(MeshMod_MeshHandle handle) {
	ASSERT(handle);
	MeshMod_Mesh* mesh = (MeshMod_Mesh*)handle;
	return mesh->registry;
}

AL2O3_EXTERN_C MeshMod_DataContainerHandle MeshMod_MeshGetVertices(MeshMod_MeshHandle handle) {
	ASSERT(handle);
	MeshMod_Mesh * mesh = (MeshMod_Mesh *)handle;
	return mesh->vertices;
}
AL2O3_EXTERN_C MeshMod_DataContainerHandle MeshMod_MeshGetEdges(MeshMod_MeshHandle handle) {
	ASSERT(handle);
	MeshMod_Mesh * mesh = (MeshMod_Mesh *)handle;
	return mesh->edges;
}
AL2O3_EXTERN_C MeshMod_DataContainerHandle MeshMod_MeshGetPolygons(MeshMod_MeshHandle handle) {
	ASSERT(handle);
	MeshMod_Mesh * mesh = (MeshMod_Mesh *)handle;
	return mesh->polygons;
}

AL2O3_EXTERN_C MeshMod_MeshHandle MeshMod_MeshClone(MeshMod_MeshHandle handle) {
	ASSERT(handle);
	MeshMod_Mesh* omesh = (MeshMod_Mesh*)handle;
	MeshMod_Mesh* nmesh = NULL;
	if (omesh->ownRegistry == true) {
		// TODO there is a bug here if ownedRegistry registry has been modified.
		nmesh = MeshMod_MeshCreateWithDefaultRegistry(omesh->name);
	}
	else {
		nmesh = MeshMod_MeshCreate(MeshMod_MeshGetRegistry(omesh), omesh->name);
	}
	if (nmesh == NULL) return NULL;

	nmesh->vertices = MeshMod_DataContainerClone(omesh->vertices, nmesh);
	nmesh->edges = MeshMod_DataContainerClone(omesh->edges, nmesh);
	nmesh->polygons = MeshMod_DataContainerClone(omesh->polygons, nmesh);

	nmesh->arbitaryDataKeys = CADT_VectorClone(omesh->arbitaryDataKeys);
	nmesh->arbitaryDataSizes = CADT_DictU64Clone(omesh->arbitaryDataSizes);

	// we can't just clone the data as we need to alloc and copy it
	nmesh->arbitaryData = CADT_DictU64Create();
	for (size_t i = 0; i < CADT_VectorSize(omesh->arbitaryDataKeys); ++i) {
		MeshMod_Tag tag = *(MeshMod_Tag*)CADT_VectorAt(omesh->arbitaryDataKeys, i);		
		void* data = (void*)CADT_DictU64Get(omesh->arbitaryData, tag);
		size_t dataLen = CADT_DictU64Get(omesh->arbitaryDataSizes, tag);

		void* ourData = MEMORY_MALLOC(dataLen);
		memcpy(ourData, data, dataLen);
		CADT_DictU64Add(nmesh->arbitaryData, tag, (uint64_t)ourData);
	}

	return nmesh;
}

AL2O3_EXTERN_C void* MeshMod_MeshAddData(MeshMod_MeshHandle handle, MeshMod_Tag tag, void* data, size_t dataLen) {
	ASSERT(handle);
	MeshMod_Mesh* mesh = (MeshMod_Mesh*)handle;
	ASSERT(MeshMod_MeshHasData(handle, tag) == false);
	void* ourData = MEMORY_MALLOC(dataLen);
	memcpy(ourData, data, dataLen);
	CADT_VectorPushElement(mesh->arbitaryDataKeys, tag);
	CADT_DictU64Add(mesh->arbitaryData, tag, (uint64_t)ourData);
	CADT_DictU64Add(mesh->arbitaryDataSizes, tag, dataLen);

	return ourData;
}
AL2O3_EXTERN_C void* MeshMod_MeshGetData(MeshMod_MeshHandle handle, MeshMod_Tag tag) {
	ASSERT(handle);
	MeshMod_Mesh* mesh = (MeshMod_Mesh*)handle;
	return (void*) CADT_DictU64Get(mesh->arbitaryData, tag);
}
AL2O3_EXTERN_C void MeshMod_MeshRemoveData(MeshMod_MeshHandle handle, MeshMod_Tag tag) {
	ASSERT(handle);
	MeshMod_Mesh* mesh = (MeshMod_Mesh*)handle;
	if (MeshMod_MeshHasData(handle, tag) == false) { return; }

	MEMORY_FREE((void*)CADT_DictU64Get(mesh->arbitaryData, tag));
	CADT_DictU64Remove(mesh->arbitaryData, tag);
	CADT_DictU64Remove(mesh->arbitaryDataSizes, tag);
	CADT_VectorSwapRemove(mesh->arbitaryDataKeys, CADT_VectorFind(mesh->arbitaryDataKeys, &tag));
}

AL2O3_EXTERN_C bool MeshMod_MeshHasData(MeshMod_MeshHandle handle, MeshMod_Tag tag) {
	ASSERT(handle);
	MeshMod_Mesh* mesh = (MeshMod_Mesh*)handle;
	return CADT_DictU64KeyExists(mesh->arbitaryData, tag);
}
