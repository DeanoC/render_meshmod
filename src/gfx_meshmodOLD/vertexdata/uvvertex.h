#pragma once
/** \file MeshModUVVertex.h
   A vertex holding a apirt of 2D UV coordinates
   (c) 2006 Dean Calver
 */

#if !defined( MESH_MOD_UV_VERTEX_H_ )
#define MESH_MOD_UV_VERTEX_H_

#include "meshmod/meshmod.h"
#include "meshmod/varielements.h"

namespace MeshMod { namespace VertexData {
struct UV
{
	float u;
	float v;

	UV() :
			u(s_floatMarker), v(s_floatMarker) {}

	UV(const float u_, const float v_) :
			u(u_), v(v_) {}

	UV interpolate(const UV &b, const float t) const
	{
		const float one_minus_t = 1.0f - t;
		return UV((u * t) + (b.u * one_minus_t),
				  (v * t) + (b.v * one_minus_t));
	}

	UV interpolate(const UV &b, const UV &c, const float _u, const float _v) const
	{
		const float w = 1.0f - _v - _u;
		return UV((u * _u) + (b.u * _v) + (c.u * w),
				  (v * _u) + (b.v * _v) + (c.v * w));
	}

	bool equal(const UV &other, const float epsilon = s_epsilon1e_5) const
	{
		if((fabsf(u - other.u) < epsilon) &&
		   (fabsf(v - other.v) < epsilon))
			return true;
		else return false;
	}

	bool equal(const float u_, const float v_, const float epsilon = s_epsilon1e_5) const
	{
		return equal(UV(u, v), epsilon);
	}

	bool isValid() const
	{
		// for float NAN we must use integer compares
		const unsigned int im = *(reinterpret_cast<const unsigned int *>(&s_floatMarker));

		const unsigned int iu = *(reinterpret_cast<const unsigned int *>(&u));
		const unsigned int iv = *(reinterpret_cast<const unsigned int *>(&v));

		// either our marker NAN return false;
		if(iu == im || iv == im)
			return false;
		else return true;
	}

	static constexpr std::string_view const getName() {
		using namespace std::literals;
		return "UV"sv; };
};
//! 2D texture coords
typedef BaseElements<VertexData::UV, Vertex_, true, DerivedType::NotDerived> UVs;

} } // end MeshMod

#endif // MESH_MOD_POINT_REP_VERTEX_H_
