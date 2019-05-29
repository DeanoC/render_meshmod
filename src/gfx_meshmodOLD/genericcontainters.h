//
// Created by Computer on 02/08/2018.
//

#ifndef MESHMOD_GENERICCONTAINTERS_H
#define MESHMOD_GENERICCONTAINTERS_H

namespace MeshMod
{
// boolean marker adds a boolean per vertex for marking visitation etc
struct BoolMarker
{
	bool marker = false;


	BoolMarker interpolate(const BoolMarker& b, const float t) const
	{
		if(t > 0.5f)
		{
			return BoolMarker{ b.marker };
		} else
		{
			return BoolMarker{ marker };
		}
	}

	BoolMarker interpolate(const BoolMarker& b, const BoolMarker& c, const float _u, const float _v) const
	{
		const float w = 1.0f - _v - _u;
		const float mark = (marker * _u) + (b.marker * _v) + (c.marker * w);
		return BoolMarker{ mark > 0.5f ? true : false };
	}

	static const std::string getName() { return "BoolMarker"; };
};

struct Float1Tuple
{
	float data = 0.0f;

	Float1Tuple interpolate(const Float1Tuple& b, const float t) const
	{
		const float one_minus_t = 1.0f - t;
		return Float1Tuple{ (data * t) + (b.data * one_minus_t) };
	}

	Float1Tuple interpolate(const Float1Tuple& b, const Float1Tuple& c, const float _u, const float _v) const
	{
		const float w = 1.0f - _v - _u;
		return Float1Tuple{ (data * _u) + (b.data * _v) + (c.data * w) };
	}

	static const std::string getName() { return "Float1Tuple"; };
};

struct UnsignedInt1Tuple
{
	unsigned int data = 0;

	UnsignedInt1Tuple interpolate(const Float1Tuple& b, const float ft) const
	{
		// doubles needed to ensure all ints 32 bit ints are covered
		const double t = (double) ft;
		const double one_minus_t = 1.0 - t;
		return UnsignedInt1Tuple{ (unsigned int) ((data * t) + (b.data * one_minus_t)) };
	}

	UnsignedInt1Tuple
	interpolate(const UnsignedInt1Tuple& b, const UnsignedInt1Tuple& c, const float _u, const float _v) const
	{
		const double u = (double) _u;
		const double v = (double) _v;
		const double w = 1.0 - v - u;
		return UnsignedInt1Tuple{ (unsigned int) ((data * _u) + (b.data * _v) + (c.data * w)) };
	}

	static const std::string getName() { return "UnsignedInt1Tuple"; };
};

//! boolean marker
template<typename type, DerivedType derived> using BoolMarkers = BaseElements<BoolMarker, type, true, derived>;

//! a single float (unspecifed use, upto you use subname to identify)
template<typename type, DerivedType derived> using Float1Tuples = BaseElements<Float1Tuple, type, true, derived> ;

//! a single unsigned int (unspecifed use, upto you use subname to identify)
template<typename type, DerivedType derived> using UnsignedInt1Tuples = BaseElements<UnsignedInt1Tuple, type, true, derived>;

}

#endif //NATIVESNAPSHOT_GENERICCONTAINTERS_H
