cmake_minimum_required(VERSION 3.12.4)
get_directory_property(hasParent PARENT_DIRECTORY)
if(NOT hasParent)
	option(unittests "unittests" OFF)
	get_filename_component(_PARENT_DIR ${CMAKE_CURRENT_SOURCE_DIR} DIRECTORY)
	set_property(GLOBAL PROPERTY GLOBAL_FETCHDEPS_BASE ${_PARENT_DIR}/al2o3 )
	include(FetchContent)
	FetchContent_Declare( al2o3 GIT_REPOSITORY https://github.com/DeanoC/al2o3 GIT_TAG master )
	FetchContent_GetProperties(al2o3)
	if(NOT al2o3_POPULATED)
		FetchContent_Populate(al2o3)
		add_subdirectory(${al2o3_SOURCE_DIR} ${al2o3_BINARY_DIR})
	endif()
	INIT_AL2O3(${CMAKE_CURRENT_SOURCE_DIR})
endif ()

SET_MIN_VERSIONS()
set(LibName render_meshmod)
project(${LibName})

set(Interface
		registry.h
		meshmod.h
		mesh.h
		datacontainer.h
		basicalgos.h
		vertex/basicdata.h
		vertex/position.h
		vertex/normal.h
		vertex/uv.h
		vertex/similar.h
		edge/halfedge.h
		polygon/tribrep.h
		)

set(Src
		registry.c
		mesh.c
		datacontainer.c
		basicalgos.c
		vertex/basicdata.c
		vertex/default.c
		vertex/position.c
		vertex/normal.c
		vertex/uv.c
		vertex/similar.c
		edge/default.c
		edge/halfedge.c
		polygon/default.c
		polygon/tribrep.c
#		halfedges.cpp
#		mesh.cpp
#		polygons.cpp
#		scenenode.cpp
#		varicontainer.cpp
#		vertices.cpp
		)

set(Deps
		al2o3_platform
		al2o3_memory
		al2o3_cadt
		al2o3_cmath
		al2o3_vfile
		al2o3_stb
		al2o3_lz4
		utils_misccpp
		)

ADD_LIB(${LibName} "${Interface}" "${Src}" "${Deps}")

set( Tests
		runner.cpp
		test_tag.cpp
		test_datacontainer.cpp
		test_mesh.cpp
		)
set( TestDeps
		utils_simple_logmanager
		gfx_image_impl_basic
		al2o3_catch2 )
ADD_LIB_TESTS(${LibName} "${Interface}" "${Tests}" "${TestDeps}")

