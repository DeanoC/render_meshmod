#include "al2o3_platform/platform.h"
#include "al2o3_memory/memory.h"
#include "al2o3_cadt/bagofvectors.h"
#include "gfx_meshmod/mesh.h"

static uint8_t const MeshMod_ContainerType_Vertex = 0;
static uint8_t const MeshMod_ContainerType_Edge = 1;
static uint8_t const MeshMod_ContainerType_Polygon = 2;

typedef struct MeshMod_DataContainer {
	uint8_t containerType;
	CADT_BagOfVectorsHandle data;
	size_t elementCount;
	struct MeshMod_Mesh* owner;
} MeshMod_DataContainer;

typedef struct MeshMod_Mesh {
	MeshMod_TagRegistryHandle registry;
	char const * name;

	MeshMod_DataContainer vertices;
	MeshMod_DataContainer edges;
	MeshMod_DataContainer polygons;
} MeshMod_Mesh;


AL2O3_EXTERN_C MeshMod_MeshHandle MeshMod_MeshCreate(MeshMod_TagRegistryHandle registry, char const* name) {
	ASSERT(name);

	MeshMod_Mesh* mesh = (MeshMod_Mesh*)MEMORY_MALLOC(sizeof(MeshMod_Mesh));
	if(mesh == NULL) return NULL;
	mesh->name = MEMORY_MALLOC(strlen(name) + 1);
	if(mesh->name == NULL) return NULL;
	strcpy((char*)mesh->name, name);
	mesh->registry = registry;

	mesh->vertices.data = CADT_BagOfVectorsCreate();
	mesh->vertices.owner = mesh;
	mesh->vertices.containerType = MeshMod_ContainerType_Vertex;
	mesh->vertices.elementCount = 0;
	
	mesh->edges.data = CADT_BagOfVectorsCreate();
	mesh->edges.owner = mesh;
	mesh->edges.containerType = MeshMod_ContainerType_Edge;
	mesh->edges.elementCount = 0;
	
	mesh->polygons.data = CADT_BagOfVectorsCreate();
	mesh->polygons.owner = mesh;
	mesh->polygons.containerType = MeshMod_ContainerType_Polygon;
	mesh->polygons.elementCount = 0;

	return mesh;
}

AL2O3_EXTERN_C void MeshMod_MeshDestroy(MeshMod_MeshHandle handle) {
	ASSERT(handle);

	MeshMod_Mesh* mesh = (MeshMod_Mesh*)handle;

	CADT_BagOfVectorsDestroy(mesh->polygons.data);
	CADT_BagOfVectorsDestroy(mesh->edges.data);
	CADT_BagOfVectorsDestroy(mesh->vertices.data);

	MEMORY_FREE((void*)mesh->name);
	MEMORY_FREE(mesh);
}

AL2O3_EXTERN_C MeshMod_DataContainerHandle MeshMod_GetVertices(MeshMod_MeshHandle handle) {
	ASSERT(handle);
	MeshMod_Mesh * mesh = (MeshMod_Mesh *)handle;
	return &mesh->vertices;
}
AL2O3_EXTERN_C MeshMod_DataContainerHandle MeshMod_GetEdges(MeshMod_MeshHandle handle) {
	ASSERT(handle);
	MeshMod_Mesh * mesh = (MeshMod_Mesh *)handle;
	return &mesh->edges;
}
AL2O3_EXTERN_C MeshMod_DataContainerHandle MeshMod_GetPolygons(MeshMod_MeshHandle handle) {
	ASSERT(handle);
	MeshMod_Mesh * mesh = (MeshMod_Mesh *)handle;
	return &mesh->polygons;
}

AL2O3_EXTERN_C bool MeshMod_DataContainerAddNewContainer(MeshMod_DataContainerHandle handle, MeshMod_Tag tag) {
	ASSERT(handle);
	MeshMod_DataContainer* dc = (MeshMod_DataContainer*)handle;
	MeshMod_Mesh* mesh = (MeshMod_Mesh*)dc->owner;
	ASSERT(MeshMod_TagRegistryTagExists(mesh->registry, tag));
	size_t const elementSize = MeshMod_TagRegistryDataSize(mesh->registry, tag);
	return CADT_BagOfVectorsAdd(dc->data, tag, elementSize);
}
