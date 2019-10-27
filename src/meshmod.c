#include "al2o3_platform/platform.h"
#include "al2o3_memory/memory.h"
#include "al2o3_cadt/vector.h"
#include "al2o3_cadt/dict.h"
#include "render_meshmod/registry.h"
#include "registry.h"
#include "mesh.h"

// call these as you start and stop using meshmod, they setup a few globals safely
AL2O3_EXTERN_C void MeshMod_StartUp(void) {
	g_MeshMod_MeshHandleManager = Handle_Manager32Create(sizeof(MeshMod_Mesh), 16, 1024, true);
	g_MeshMod_RegistryHandleManager = Handle_Manager32Create(sizeof(MeshMod_Registry), 16, 256, false);

}

AL2O3_EXTERN_C void MeshMod_Shutdown(void) {
	if(g_MeshMod_MeshHandleManager) {
		Handle_Manager32Destroy(g_MeshMod_MeshHandleManager);
		g_MeshMod_MeshHandleManager = NULL;
	}

	if(g_MeshMod_RegistryHandleManager) {
		Handle_Manager32Destroy(g_MeshMod_RegistryHandleManager);
		g_MeshMod_RegistryHandleManager = NULL;
	}
}
