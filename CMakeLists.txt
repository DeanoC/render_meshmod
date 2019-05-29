#if building standalone and not specified build tests
option(BUILD_TESTS "Whether to build tests" ON)
include(FetchContent)
FetchContent_Declare( al2o3 GIT_REPOSITORY https://github.com/DeanoC/al2o3 GIT_TAG master )
FetchContent_GetProperties(al2o3)
if(NOT al2o3_POPULATED)
	FetchContent_Populate(al2o3)
	add_subdirectory(${al2o3_SOURCE_DIR} ${al2o3_BINARY_DIR})
endif()

SET_MIN_VERSIONS()
set(LibName gfx_meshmod)
project(${LibName})

set(Interface
		cvector.h
		cdict.h
		cbagofvectors.h
		tag.h
		meshmod.h
		vertexarray.h
		mesh.h
#		halfedgedata/halfedgecontainers.h
#		halfedgedata/halfedgedata.h
#		materialparameterdata/materialparameter.h
#		materialparameterdata/materialparameterbase.h
#		materialparameterdata/materialvertexbindings.h
#		materialparameterdata/rgbmaterialparameter.h
#		materialparameterdata/scalarmaterialparameter.h
#		materialparameterdata/texturematerialparameter.h
#		meshdata/materialcontainers.h
#		meshdata/materialdata.h
#		polygonsdata/polygoncontainers.h
#		polygonsdata/polygondata.h
#		genericcontainters.h
#		halfedges.h
#		indextypes.h
#		mesh.h
#		meshmod.h
#		polygons.h
#		property.h
#		scene.h
#		scenenode.h
#		sceneobject.h
#		transform.h
#		triangle.h
#		types.h
#		varicontainer.h
#		varielements.h
#		vertices.h
		)

set(Src
		cvector.c
		cdict.c
		cbagofvectors.c
		tag.c
		meshmod.cpp
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
		al2o3_cmath
		al2o3_vfile
		al2o3_stb
		utils_misccpp
		)
ADD_LIB(${LibName} "${Interface}" "${Src}" "${Deps}")

set( Tests
		runner.cpp
		test_cvector.cpp
		test_cdict.cpp
		test_cbagofvectors.cpp
		test_tag.cpp
		)
set( TestDeps
		utils_simple_logmanager
		gfx_image_impl_basic
		al2o3_catch2 )
ADD_LIB_TESTS(${LibName} "${Interface}" "${Tests}" "${TestDeps}")

