#pragma once
#ifndef GFX_MESHMOD_VERTEXDATA_POSITION_H
#define GFX_MESHMOD_VERTEXDATA_POSITION_H
//! a position is a 3-tuple representing a physical position, its
//! a float per coordinate which should be enough for most applications
//! As position is a 'special' vertex attribute, if you require more
//! accuracy its probably a good idea to have a seperate more accurate
//! position attribute as well as the float position, cos currently the
//! internal system will often truncate to float anyway.
//! \todo make accuracy a compile time option
typedef Math_vec3F_t MeshMod_VertexDataPosition;


AL2O3_EXTERN_C MeshMod_VertexDataPosition MeshMod_VertexDataPositionInterpolate1D(MeshMod_VertexDataPosition const a, MeshMod_VertexDataPosition const b, float const t)
{
	const float one_minus_t = 1.0f - t;
	return Position((x * t) + (b.x * one_minus_t),
									(y * t) + (b.y * one_minus_t),
									(z * t) + (b.z * one_minus_t));
}

	Position interpolate(const Position& b, const Position& c, const float u, const float v) const
	{
		const float w = 1.0f - v - u;
		return Position((x * u) + (b.x * v) + (c.x * w),
										(y * u) + (b.y * v) + (c.y * w),
										(z * u) + (b.z * v) + (c.z * w));
	}

	//! is the position equal to the data passed in, using an epsilon parameter to decide
	bool equal(const Position& other, const float epsilon = s_epsilon1e_5) const
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
		return equal(Position(x_, y_, z_), epsilon);
	}

	//! is this position valid (has real vertex data)
	bool isValid() const
	{
		// for float NAN we must use interger compares
		const unsigned int im = *(reinterpret_cast<const unsigned int*>(&s_floatMarker));

		const unsigned int ix = *(reinterpret_cast<const unsigned int*>(&x));
		const unsigned int iy = *(reinterpret_cast<const unsigned int*>(&y));
		const unsigned int iz = *(reinterpret_cast<const unsigned int*>(&z));

		// either our marker NAN return false;
		if(ix == im || iy == im || iz == im)
			return false;
		else return true;
	}

	//! name used to get this kind of data
	static constexpr std::string_view const getName() {
		using namespace std::literals;
		return "Position"sv; };
};


#endif