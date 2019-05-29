#pragma once
/** \file property.h

(c) 2012 Dean Calver
 */

#if !defined( MESH_MOD_PROPERTY_H_ )
#define MESH_MOD_PROPERTY_H_

#include "core/any.h"

namespace MeshMod {

	class Property {
	public:
		Property( const std::string& _name, const std::any _val ) :
			name( _name ),
			value( _val ) {}

		const std::string& getName() const { return name; }
		const std::any getValue() const { return value; }

		template< typename T>
		bool isType() const { return value.type() == typeid(T); }

		template< typename T>
		const T getAs() const { 
			return std::any_cast<T>(value);
		}

	protected:
		const std::string 	name;
		const std::any	value;
	};
	typedef std::shared_ptr<Property> 	PropertyPtr;

}

#endif
