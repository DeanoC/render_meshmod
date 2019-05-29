#pragma once
/** \file MeshModTransform.h
	Holds a transform for a scene node
   (c) 2006 Dean Calver
 */

#if !defined( MESH_MOD_TRANSFORM_H_ )
#define MESH_MOD_TRANSFORM_H_

#include "math/vector_math.h"

namespace MeshMod
{

//! an object holding 
class Transform
{
public:
	Transform() :
			position(0,0,0),
			orientation( 0,0,0,1 ),
			scale( 1,1,1 ) 
	{}
	bool isIdentity() const {
		if ( (Math::dot( position, position ) < 1e-5f) &&
			 (1 - Math::dot(orientation, orientation) < 1e-5f) &&
			 (1 - Math::dot(scale, scale) < 1e-5f) ) {
			return true;
		} else {
			return false;
		}
	}

	// just one of many options for this...
	Math::mat4x4 MakeMatrix() const
	{
		using namespace Math;
		mat4x4 transMat = translate(identity<mat4x4>(), position);
		mat4x4 rotMat = toMat4(orientation);
		mat4x4 scaleMat = Math::scale(identity<mat4x4>(), scale);

		return transMat * rotMat * scaleMat;
	}

	//! position
	Math::vec3		position;
	//! orientation
	Math::quat	orientation;
	//! scale
	Math::vec3		scale;
};

};

#endif // end MESH_MOD_TRANSFORM_H_