#pragma once
/** \file MeshModNormalVertex.h
   A vertex holding a normal.
   (c) 2006 Dean Calver
 */

#if !defined( MESH_MOD_NORMAL_VERTEX_H_ )
#define MESH_MOD_NORMAL_VERTEX_H_

#include "meshmod/meshmod.h"
#include "meshmod/varielements.h"

namespace MeshMod { namespace VertexData {
//! a per vertex normal 3-tuple
struct Normal
{
	//! the 3D data
	float x, y, z;

	//! default ctor, marks this normal as invalid
	Normal() :
			x(s_floatMarker), y(s_floatMarker), z(s_floatMarker) {};

	//! 3 float ctor
	Normal(const float x_, const float y_, const float z_) :
			x(x_), y(y_), z(z_) {}

	Normal interpolate(const Normal &b, const float t) const
	{
		const float one_minus_t = 1.0f - t;
		return Normal((x * t) + (b.x * one_minus_t),
					  (y * t) + (b.y * one_minus_t),
					  (z * t) + (b.z * one_minus_t));
	}

	Normal interpolate(const Normal &b, const Normal &c, const float u, const float v) const
	{
		const float w = 1.0f - v - u;
		return Normal((x * u) + (b.x * v) + (c.x * w),
					  (y * u) + (b.y * v) + (c.y * w),
					  (z * u) + (b.z * v) + (c.z * w));
	}

	Math::vec3 getVec3() const
	{
		return Math::vec3(x, y, z);
	}

	//! is the position equal to the data passed in, using an epsilon parameter to decide
	bool equal(const Normal &other, const float epsilon = s_epsilon1e_5) const
	{

		if((fabsf(x - other.x) < epsilon) &&
		   (fabsf(y - other.y) < epsilon) &&
		   (fabsf(z - other.z) < epsilon))
			return true;
		else return false;
	}

	//! is the position equal to the data passed in, using an epsilon parameter to decide
	bool equal(const float x_, const float y_, const float z_, const float epsilon = s_epsilon1e_5) const
	{
		return equal(Normal(x_, y_, z_), epsilon);
	}

	//! is this normal valid
	bool isValid() const
	{
		// for float NAN we must use interger compares
		const unsigned int im = *(reinterpret_cast<const unsigned int *>(&s_floatMarker));

		const unsigned int ix = *(reinterpret_cast<const unsigned int *>(&x));
		const unsigned int iy = *(reinterpret_cast<const unsigned int *>(&y));
		const unsigned int iz = *(reinterpret_cast<const unsigned int *>(&z));

		// any of our marker NAN return false;
		if(ix == im || iy == im || iz == im)
			return false;
		else return true;
	}

	//! name is used to get this data
	static constexpr std::string_view const getName() {
		using namespace std::literals;
		return "Normal"sv; };
};
//! normal vertex element (x,y,z)
typedef BaseElements<VertexData::Normal, Vertex_, true, DerivedType::DerivedFromTopology> Normals;

} } // end namespace

#endif
