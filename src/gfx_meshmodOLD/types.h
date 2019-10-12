#pragma once
/** \file MeshModTypes.h
	These are the types used all over the place. So stuck here to 
	hopefully avoid too many depenedenies.
   (c) 2006 Dean Calver
 */

#if !defined( MESH_MOD_TYPES_H_ )
#define MESH_MOD_TYPES_H_

#include "render_meshmod/indextypes.h"

namespace MeshMod
{
	extern const float s_epsilon1e_5;	//!< epsilon used in various places
	extern const float s_floatMarker;	//!< float NAN used to mark invalid

	//! Tag for vertex data, use this for custom vertex data types
	struct Vertex_{
		using IndexType = VertexIndex;
	};

	//! Tag for half edge data, use this for custom half edge data types
	struct HalfEdge_{
		using IndexType = HalfEdgeIndex;
	};

	//! Tag for face data, use this for custom per face data types
	struct Polygon_{
		using IndexType = PolygonIndex;
	};

	//! Tag for mesh data, use this for custom per mesh data types
	struct Mesh_{
		using IndexType = SceneNodeIndex;
	};

	//! Tag for material parameter data, use this for types of material parameters
	struct MaterialParameters_{
		using IndexType = MaterialIndex;
	};

} // end namespace

#endif