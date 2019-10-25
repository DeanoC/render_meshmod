#define CATCH_CONFIG_RUNNER
#include "al2o3_catch2/catch2.hpp"
#include "utils_simple_logmanager/logmanager.h"
#include "render_meshmod/meshmod.h"
int main(int argc, char const *argv[]) {
	auto logger = SimpleLogManager_Alloc();
	LOGINFO("Starting test");
	MeshMod_StartUp();

	auto ret = Catch::Session().run(argc, (char **) argv);

	MeshMod_Shutdown();

	SimpleLogManager_Free(logger);

	return ret;
}

