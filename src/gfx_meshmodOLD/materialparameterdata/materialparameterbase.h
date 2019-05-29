#pragma once
/** \file MeshModMaterialParameterBase.h
	ALL material parameter should derive off this
   (c) 2006 Dean Calver
 */

#if !defined( MESH_MOD_MATERIAL_PARAMETER_BASE_H_ )
#define MESH_MOD_MATERIAL_PARAMETER_BASE_H_

#include "meshmod/varicontainer.h"

//---------------------------------------------------------------------------
// Enums and typedefs
//---------------------------------------------------------------------------
namespace MeshMod { namespace MaterialData {
//! parameters share some base properies
struct ParameterBase
{
	bool			isAnimated;

	//! default ctor
	ParameterBase( bool animated = false) :
		isAnimated(animated)
	{}
};

} } // end MeshMod

#endif // MESH_MOD_MATERIAL_PARAMETER_BASE_H_
