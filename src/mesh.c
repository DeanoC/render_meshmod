#include "al2o3_platform/platform.h"
#include "al2o3_memory/memory.h"
#include "al2o3_cadt/bagofvectors.h"
#include "gfx_meshmod/datacontainer.h"
#include "gfx_meshmod/mesh.h"

typedef struct MeshMod_Mesh {
	MeshMod_RegistryHandle registry;
	bool ownRegistry;
	char const * name;

	MeshMod_DataContainerHandle vertices;
	MeshMod_DataContainerHandle edges;
	MeshMod_DataContainerHandle polygons;
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

	return mesh;
}

AL2O3_EXTERN_C void MeshMod_MeshDestroy(MeshMod_MeshHandle handle) {
	ASSERT(handle);

	MeshMod_Mesh* mesh = (MeshMod_Mesh*)handle;

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
	return nmesh;
}
