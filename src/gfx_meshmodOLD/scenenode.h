#pragma once
/** \file MeshModScene.h
	A container for a scene hierachy

(c) 2012 Dean Calver
 */

#if !defined( MESH_MOD_SCENE_NODE_H_ )
#define MESH_MOD_SCENE_NODE_H_

//---------------------------------------------------------------------------
// Defines
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
// Includes
//---------------------------------------------------------------------------
#include "indextypes.h"
#include "transform.h"
#include "sceneobject.h"
#include "property.h"
#include <functional>

//---------------------------------------------------------------------------
// Enums and typedefs
//---------------------------------------------------------------------------
namespace MeshMod {
	class SceneNode;

	class SceneNode : public std::enable_shared_from_this<SceneNode> {
	public:
		using Ptr = std::shared_ptr<SceneNode>;
		using ConstPtr = std::shared_ptr<SceneNode const>;
		using WeakPtr = std::weak_ptr<SceneNode>;

		void addChild( const SceneNode::Ptr child ) {
			// check for duplicates
			assert( std::find( children.begin(), children.end(), child ) == children.end() );
			children.push_back( child );
		}
		void removeChild( SceneNode::Ptr child ) {
			SceneNodeContainer::iterator chIt = 
				std::find( children.begin(), children.end(), child );
			assert( chIt != children.end() );
			children.erase( chIt );
		}

		uint32_t findChildIndex( const SceneNode::Ptr child ) const {
			SceneNodeContainer::const_iterator chIt = std::find( children.begin(), children.end(), child );
			return (uint32_t) std::distance( children.begin(), chIt );
		}

		unsigned int getChildCount() const {
			return (unsigned int) children.size();
		}
		Ptr getChild(uint32_t index) {
			return children[index];
		}
		ConstPtr getChild(uint32_t index) const {
			return children[index];
		}
		void addObject( SceneObject::Ptr obj ) {
			assert( std::find( objects.begin(), objects.end(), obj ) == objects.end() );
			objects.push_back( obj );
		}
		void removeObject( SceneObject::Ptr obj ) {
			SceneObjectContainer::iterator obIt = 
				std::find( objects.begin(), objects.end(), obj );
			assert( obIt != objects.end() );
			objects.erase( obIt );
		}

		unsigned int getObjectCount() const {
			return (unsigned int) objects.size();
		}
		SceneObject::Ptr getObject(unsigned int index) {
			return objects[index];
		}

		void visitDescendents(Math::mat4x4 const& rootMatrix, std::function<void(SceneNode const&, Math::mat4x4 const&)> func) const;
		void mutateDescendents(Math::mat4x4 const& rootMatrix, std::function<void(SceneNode&, Math::mat4x4 const&)> func);
		void visitObjects(std::function<void(SceneObject::ConstPtr)> func) const;
		void mutateObjects(std::function<void(SceneObject::Ptr)> func);

		// embedded transform TODO animation
		Transform										transform;
		std::string										type;
		std::string										name;
		std::vector< PropertyPtr > 						properties;

	protected:
		typedef std::vector< SceneNode::Ptr >			SceneNodeContainer;
		typedef std::vector< SceneObject::Ptr >			SceneObjectContainer;
		SceneNodeContainer								children;
		SceneObjectContainer							objects;

	};


} // end namespace
#endif // MESH_MOD_SCENE_NODE_H_
