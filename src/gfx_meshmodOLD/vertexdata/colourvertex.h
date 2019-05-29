#pragma once
/** \file ColourVertex.h
   A vertex holding a colour.
 */

#if !defined( MESH_MOD_COLOUR_VERTEX_H_ )
#define MESH_MOD_COLOUR_VERTEX_H_

#include "meshmod/meshmod.h"
#include "meshmod/varielements.h"

namespace MeshMod { namespace VertexData {
//! a per vertex colour RGBA 4-tuple
struct FloatRGBAColour
{
	float r, g, b, a;

	//! default ctor, marks this normal as invalid
	FloatRGBAColour() :
			r(s_floatMarker), g(s_floatMarker), b(s_floatMarker), a(s_floatMarker) {};

	//! 4 float ctor
	FloatRGBAColour(float const r_, float const g_, float const b_, float const a_) :
			r(r_), g(g_), b(b_), a(a_) {}

	FloatRGBAColour interpolate(FloatRGBAColour const &b_, float const t_) const
	{
		const float one_minus_t = 1.0f - t_;
		return FloatRGBAColour((r * t_) + (b_.r * one_minus_t),
							   (g * t_) + (b_.g * one_minus_t),
							   (b * t_) + (b_.b * one_minus_t),
							   (a * t_) + (b_.a * one_minus_t));
	}

	FloatRGBAColour interpolate(FloatRGBAColour const &b_,
								FloatRGBAColour const &c_,
								float const u_,
								float const v_) const
	{
		const float w = 1.0f - v_ - u_;
		return FloatRGBAColour(
				(r * u_) + (b_.r * v_) + (c_.r * w),
				(g * u_) + (b_.g * v_) + (c_.g * w),
				(b * u_) + (b_.b * v_) + (c_.b * w),
				(a * u_) + (b_.a * v_) + (c_.a * w));
	}

	//! is the colour equal to the data passed in, using an epsilon parameter to decide
	bool equal(FloatRGBAColour const &other, float const epsilon = s_epsilon1e_5) const
	{

		if((fabsf(r - other.r) < epsilon) &&
		   (fabsf(g - other.g) < epsilon) &&
		   (fabsf(b - other.b) < epsilon) &&
		   (fabsf(a - other.a) < epsilon))
			return true;
		else return false;
	}

	//! is the colour equal to the data passed in, using an epsilon parameter to decide
	bool
	equal(float const r_, float const g_, float const b_, float const a_, float const epsilon = s_epsilon1e_5) const
	{
		return equal(FloatRGBAColour(r_, g_, b_, a_), epsilon);
	}

	//! is this colour valid
	bool isValid() const
	{
		// for float NAN we must use interger compares
		const unsigned int im = *(reinterpret_cast<const unsigned int *>(&s_floatMarker));

		const unsigned int ir = *(reinterpret_cast<const unsigned int *>(&r));
		const unsigned int ig = *(reinterpret_cast<const unsigned int *>(&g));
		const unsigned int ib = *(reinterpret_cast<const unsigned int *>(&b));
		const unsigned int ia = *(reinterpret_cast<const unsigned int *>(&b));

		// any of our marker NAN return false;
		if(ir == im || ig == im || ib == im || ia == im)
			return false;
		else return true;
	}

	//! name is used to get this data
	static constexpr std::string_view const getName() { 
		using namespace std::literals; 
		return "FloatRGBAColour"sv; };
};
//! float colour RGBA vertex element (r,g,b,a)
typedef BaseElements<VertexData::FloatRGBAColour, Vertex_, true, DerivedType::NotDerived> FloatRGBAColourVertexElements;

} } // end namespace

#endif
