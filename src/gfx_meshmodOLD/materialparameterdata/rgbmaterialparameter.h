#pragma once
/** \file MeshModRGBMaterialParameter.h
An RGB material parameter
   (c) 2006 Dean Calver
 */

#if !defined( MESH_MOD_RGB_MATERIAL_PARAMETER_H_ )
#define MESH_MOD_RGB_MATERIAL_PARAMETER_H_

#include "meshmod/meshmod.h"
#include "meshmod/varicontainer.h"
#include "materialparameterbase.h"

namespace MeshMod { namespace MaterialData {

//! colour triplet
struct RGBColour : public ParameterBase
{
	float r, g, b;

	//! default ctor, marks this RGB as invalid
	RGBColour() :
			r(s_floatMarker), g(s_floatMarker), b(s_floatMarker) {};

	//! 3 float ctor
	RGBColour(const float r_, const float g_, const float b_) :
			r(r_), g(g_), b(b_) {}

	//! 3 double ctor (currently just truncated to float
	RGBColour(const double r_, const double g_, const double b_) :
			r(static_cast<float>(r_)), g(static_cast<float>(g_)), b(static_cast<float>(b_)) {}

	//! is the position equal to the data passed in, using an epsilon parameter to decide
	bool equal(const RGBColour &other, const float epsilon = s_epsilon1e_5) const
	{

		if((fabsf(r - other.r) < epsilon) &&
		   (fabsf(g - other.g) < epsilon) &&
		   (fabsf(b - other.b) < epsilon))
			return true;
		else return false;
	}

	//! is the position equal to the data passed in, using an epsilon parameter to decide
	bool equal(const float r_, const float g_, const float b_, const float epsilon = s_epsilon1e_5) const
	{
		return equal(RGBColour(r_, g_, b_), epsilon);
	}

	//! is this normal valid
	bool isValid() const
	{
		// for float NAN we must use interger compares
		const unsigned int im = *(reinterpret_cast<const unsigned int *>(&s_floatMarker));

		const unsigned int ir = *(reinterpret_cast<const unsigned int *>(&r));
		const unsigned int ig = *(reinterpret_cast<const unsigned int *>(&g));
		const unsigned int ib = *(reinterpret_cast<const unsigned int *>(&b));

		// either our marker NAN return false;
		if(ir == im || ig == im || ib == im)
			return false;
		else return true;
	}

	//! name is used to get this data
	static constexpr std::string_view const getName() {
		using namespace std::literals;
		return "RGBColour"sv; };
};

//! Material Parameters RGB data
typedef BaseElements<RGBColour, MaterialParameters_> RGBElements;
} } // end namespace

#endif // MESH_MOD_RGB_MATERIAL_PARAMETER_H_
