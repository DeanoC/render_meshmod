#include "al2o3_platform/platform.h"
#include "al2o3_memory/memory.h"
#include "gfx_meshmod/mesh.h"

typedef struct Mesh {
	char const * name;

	MeshMod_VertexArrayContainerHandle vertices;
	MeshMod_EdgeArrayContainerHandle edges;
	MeshMod_PolygonArrayContainerHandle polygon;
};

AL2O3_EXTERN_C MeshMod_MeshHandle MeshMod_CreateMesh(char const* name) {
	ASSERT(name);

	Mesh* mesh = (Mesh*)MEMORY_MALLOC(sizeof(Mesh));
	if(mesh == NULL) return NULL;
	mesh->name = MEMORY_MALLOC(strlen(name) + 1);
	if(mesh->name == NULL) return NULL;
	strcp(mesh->name, name);
	mesh->vertices = MeshMod_CTaggedVectorCreate()

}


AL2O3_EXTERN_C void MeshMod_Destroy(MeshMod_MeshHandle handle) {
}

AL2O3_EXTERN_C MeshMod_VertexArrayContainerHandle MeshMod_GetVertices(MeshMod_MeshHandle handle);
AL2O3_EXTERN_C MeshMod_EdgeArrayContainerHandle MeshMod_GetEdges(MeshMod_MeshHandle handle);
AL2O3_EXTERN_C MeshMod_PolygonArrayContainerHandle eshMod_GetPolygons(MeshMod_MeshHandle handle);
