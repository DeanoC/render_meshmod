#pragma once
/** \file MeshModMaterialParameter.h
	Material parameter containers attached a variable
	sized and typed list to a material
   (c) 2006 Dean Calver
 */

#if !defined( MESH_MOD_MATERIAL_PARAMETER_H_ )
#define MESH_MOD_MATERIAL_PARAMETER_H_

#include "meshmod/meshmod.h"
#include "meshmod/varicontainer.h"
#
namespace MeshMod { namespace MaterialData {
//! we now have a varicontainer for material parameters
typedef VariContainer< Elements<MaterialParameters_> > ParameterContainer;

//! a collection parameters that can be attached to a particular material
struct Parameters {
	static constexpr std::string_view const getName() {
		using namespace std::literals;
		return "Parameters"sv; };

	ParameterContainer			parameters;
};

//! material parameters elements to be attached to a mesh 
typedef BaseElements< MaterialData::Parameters, Mesh_ > MaterialParameterElements;

}

} // end MeshMod

#endif // MESH_MOD_MATERIAL_PARAMETER_H_
