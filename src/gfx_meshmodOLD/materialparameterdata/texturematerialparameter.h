#pragma once
/** \file TextureMaterialParameter.h
A texture material parameter
(c) 2006 Dean Calver
*/

#if !defined( MESH_MOD_TEXTURE_MATERIAL_PARAMETER_H_ )
#define MESH_MOD_TEXTURE_MATERIAL_PARAMETER_H_

#include "meshmod/meshmod.h"
#include "meshmod/varicontainer.h"
#include "meshmod/materialparameterdata/materialparameterbase.h"

namespace MeshMod { namespace MaterialData {

//! Texture Parameter 
struct TextureParameter : public ParameterBase
{
	enum TextureType
	{
		TT_CUBE = 0,
		TT_1D = 1,
		TT_2D = 2,
		TT_3D = 3,
	};
	TextureType type;
	std::string textureName;

	//! default ctor, marks this textureName as invalid
	TextureParameter() {};

	//! type and texture name
	TextureParameter(TextureType _type, const std::string &_name) :
			type(_type), textureName(_name) {}

	//! is the position equal to the data passed in, using an epsilon parameter to decide
	bool equal(const TextureParameter &other) const
	{
		return (type == other.type && textureName == other.textureName);
	}

	//! is this Texture valid
	bool isValid() const
	{
		return !textureName.empty();
	}


	//! name is used to get this data
	static constexpr std::string_view const getName() {
		using namespace std::literals;
		return "Texture"; };
};

//! Material Parameters Texture data
typedef BaseElements<TextureParameter, MaterialParameters_> TextureParameterElements;

} } // end namespace

#endif // MESH_MOD_TEXTURE_MATERIAL_PARAMETER_H_
