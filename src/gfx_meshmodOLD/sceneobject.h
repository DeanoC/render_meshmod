#pragma once
/** \file MeshModSceneObject.h
A Scene object is a base class that things like meshes and camera derive of
as something is in a scene.
A scenenode links together a transform, N SceneObjects and hierachy of
other nodes.

(c) 2006 Dean Calver
 */

#include <memory>

#if !defined( MESH_MOD_SCENE_OBJECT_H_ )
#define MESH_MOD_SCENE_OBJECT_H_

#pragma once

//---------------------------------------------------------------------------
// Defines
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
// Includes
//---------------------------------------------------------------------------
#include "indextypes.h"


//---------------------------------------------------------------------------
// Enums and typedefs
//---------------------------------------------------------------------------
namespace MeshMod
{
	/** Scene Object
	**/
	class SceneObject : public std::enable_shared_from_this<SceneObject>
	{
	public:
		using Ptr = std::shared_ptr<SceneObject>;
		using ConstPtr = std::shared_ptr<SceneObject const>;
		using WeakPtr = std::weak_ptr<SceneObject>;

		virtual ~SceneObject(){}
		SceneObject( const std::string& _type, const std::string& _name ) :
		  type( _type ), name( _name ){}

		//! returns the type of this object
		const std::string& getType() const { return type; }

		//! all objects in a scene have a name 
		const std::string& getName() const { return name; }

	protected:
		const std::string type; //!< type of this scene object
		std::string name; //!< name of this scene object
	};
	

} // end namespace
//---------------------------------------------------------------------------
// Prototypes
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
// Externals
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
// End Header file
//---------------------------------------------------------------------------
#endif
