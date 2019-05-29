#pragma once
/** \file VariContainer.h
   Used by the mesh system for holding variable amounts of vectors (vertex, faces).
   (c) 2002 Dean Calver
 */

#if !defined( MESH_MOD_VARICONTAINER_H_ )
#define MESH_MOD_VARICONTAINER_H_

#include "core/core.h"
#include <string>
#include <vector>
#include <string_view>
#include "varielements.h"

namespace MeshMod {

template<typename CT>
class VariContainer
{
public:
	using ContainerType = CT;
	using IndexType = typename CT::IndexType;

	~VariContainer() { clear(); }
	void clear();

	auto isValid(IndexType elementIndex) const -> bool
	{
		if(elementIndex == IndexType(~0u)) return false;
		return !notValids.at(size_t(elementIndex));
	}

	void setValid(IndexType elementIndex_, bool valid_)
	{
		notValids.at(size_t(elementIndex_)) = !valid_;
	}

	void resetValidFlags() 
	{
		for(auto& nv : notValids)
		{
			nv = false;
		}
	}
	auto resizeForNewElement() -> IndexType;

	void resize(size_t const size_);

	auto size() const -> size_t;

	void cloneTo(VariContainer<ContainerType> &nvc_) const;

	//! add the element with default subname
	auto addElement(std::shared_ptr<ContainerType> ele_, std::string_view subName_ = {}) -> void;

	//! remove the elements
	void removeElements(std::shared_ptr<ContainerType> ele_);

	//! gets the first element named name (ther may be others with different subnames)
	auto getElement(std::string_view name_) -> std::shared_ptr<ContainerType>;
	auto getElement(std::string_view name_) const -> std::shared_ptr<ContainerType const>;

	//! gets all elements of the input name
	void getAllElementsNamed(std::string_view name_, std::vector<std::shared_ptr<ContainerType>> &out_);
	void getAllElementsNamed(std::string_view name_, std::vector<std::shared_ptr<ContainerType const>> &out_) const;

	// gets the element of name and sub name
	auto getElementFromNameAndSubName(std::string_view name_, std::string_view subName_) -> std::shared_ptr<ContainerType>;
	auto getElementFromNameAndSubName(std::string_view name_, std::string_view subName_) const -> std::shared_ptr<ContainerType const>;

	// how many elements do we have.
	auto getSizeOfElementContainer() const -> size_t { return elements.size(); }

	auto getElementContainer(size_t index) -> std::shared_ptr<ContainerType> { return elements[index]; }
	auto getElementContainer(size_t index) const -> std::shared_ptr<ContainerType const > { return elements[index]; }

	template<typename Type>
	auto addElements(std::string_view subName = {}) -> std::shared_ptr<Type>
	{
		auto ele = std::make_shared<Type>();
		addElement(ele, ele->getName());
		return ele;
	}

	template<typename Type>
	void removeElements()
	{
		std::vector<std::shared_ptr<Type>> elementsToRemove;
		getAllElements(elementsToRemove);

		for (auto toRemove : elementsToRemove)
		{
			auto it = std::find(elements.cbegin(), elements.cend(), toRemove);
			if (it != elements.cend())
			{
				elements.erase(it);
			}
		}
	}


	template<typename Type>
	auto getOrAddElement(std::string_view subName = {}) -> std::shared_ptr<Type>
	{
		std::shared_ptr<Type> ele;
		if(!subName.empty())
		{
			ele = std::static_pointer_cast<Type>(getElementFromNameAndSubName(Type::getName(), subName));
			if(!ele)
			{
				ele = std::make_shared<Type>();
				addElement(ele, subName);
			}
		} else
		{
			ele = std::static_pointer_cast<Type>(getElement(Type::getName()));
			if(!ele)
			{
				ele = std::make_shared<Type>();
				addElement(ele);
			}
		}

		assert(ele);
		return ele;
	}

	template<typename Type>
	void pushBack(std::string_view subName = {}, typename Type::DataType const &data = {})
	{
		// if we adding a new type we don't wanna always resize the buffer after the add
		// cos adding a new type resizes it to the size of the array. This encapsulates
		// the slight complicated and quite frankly odd logic.. it basically does
		// what I expect by pushBack
		if(getElementsFromNameAndSubName(Type::getName(), subName) == 0)
		{
			std::shared_ptr<Type> pType = getOrAddElement<Type>(subName);
			if(pType->size() == 0)
			{
				pType->push_back(data);
				notValids.push_back(false);
			} else
			{
				pType->getElement(0) = data;
			}
		} else
		{
			Type *pType = getElement<Type>(subName);
			pType->push_back(data);
			notValids.push_back(false);
		}
	}

	template<typename Type>
	auto getElement() -> std::shared_ptr<Type>
	{
		return std::static_pointer_cast<Type>(getElement(Type::DataType::getName()));
	}

	template<typename Type>
	auto getElement() const -> std::shared_ptr<Type const>
	{
		return std::static_pointer_cast<Type const>(getElement(Type::DataType::getName()));
	}

	template<typename Type>
	auto getElement(std::string const &subName) -> std::shared_ptr<Type>
	{
		return std::static_pointer_cast<Type>(getElementFromNameAndSubName(Type::DataType::getName(), subName));
	}

	template<typename Type>
	auto getElement(std::string const &subName) const -> std::shared_ptr<const Type>
	{
		return std::static_pointer_cast<Type const>(getElementFromNameAndSubName(Type::DataType::getName(), subName));
	}

	//! gets all elements of the types name
	template<typename Type>
	void getAllElements(std::vector<std::shared_ptr<Type>> &out)
	{
		for(auto& ptr : elements)
		{
			if(ptr->name == Type::DataType::getName())
			{
				out.push_back(std::static_pointer_cast<Type>(ptr));
			}
		}
	}

	//! gets all elements of the types name (const)
	template<typename Type>
	void getAllElements(std::vector<Type const *> &out) const
	{
		for (auto const& ptr : elements)
		{
			if (ptr->name == Type::DataType::getName())
			{				
				out.push_back(std::static_pointer_cast<Type const>(ptr));
			}
		}
	}

	auto cloneElement(IndexType elementToCopy) -> IndexType;

	void removeDerived(DerivedType change);

private:
	// we have a flag per element item saying if that element item is not valid
	using elementContainer = std::vector<std::shared_ptr<ContainerType>>;
	using notValidContainer = std::vector<uint8_t>;

	elementContainer elements;
	notValidContainer notValids;
};

template<typename CT>
inline void VariContainer<CT>::clear()
{
	elements.clear();
	notValids.clear();
}

template<typename CT>
inline auto VariContainer<CT>::size() const -> size_t
{
	return notValids.size();
}

template<typename CT>
inline auto VariContainer<CT>::resizeForNewElement() -> IndexType
{
	size_t initialSize = notValids.size();
	resize(initialSize + 1);
	return IndexType(initialSize);
}

template<typename CT>
inline void VariContainer<CT>::resize(size_t const size)
{
	for (auto& con : elements)
	{
		con->resize(size);
	}
	notValids.resize(size);
}

template<typename CT>
inline auto VariContainer<CT>::addElement(std::shared_ptr<ContainerType> ele_, std::string_view subName_) -> void
{
	elements.push_back(ele_);
	ele_->resize(notValids.size());
	ele_->subName = subName_;
}

template<typename CT>
inline void VariContainer<CT>::removeElements(std::shared_ptr<ContainerType> ele)
{
	auto it = std::find(elements.begin(), elements.end(), ele);
	if (it != elements.end())
	{
		elements.erase(it);
	}
}


template<typename CT>
inline auto VariContainer<CT>::getElement(std::string_view name_) -> std::shared_ptr<ContainerType>
{
	for(auto ptr : elements)
	{
		if(ptr->name == name_)
			return ptr;
	}
	return {};
}

template<typename CT>
inline auto VariContainer<CT>::getElement(std::string_view name_) const -> std::shared_ptr<ContainerType const>
{
	for (auto ptr : elements)
	{
		if (ptr->name == name_)
			return ptr;
	}
	return {};
}

template<typename CT>
inline void VariContainer<CT>::getAllElementsNamed(std::string_view name_, std::vector<std::shared_ptr<ContainerType>> &out_)
{
	for (auto ptr : elements)
	{
		if (ptr->name == name_)
			out_.push_back(ptr);
	}
}

template<typename CT>
inline void VariContainer<CT>::getAllElementsNamed(std::string_view name_,
												  std::vector<std::shared_ptr<ContainerType const>> &out_) const
{
	for (auto ptr : elements)
	{
		if (ptr->name == name_)
			out_.push_back(ptr);
	}
}

template<typename CT>
inline auto VariContainer<CT>::getElementFromNameAndSubName(std::string_view name_,
													  std::string_view subName_) -> std::shared_ptr<ContainerType>
{
	for (auto ptr : elements)
	{
		if (ptr->name == name_ && ptr->subName == subName_)
			return ptr;
	}
	return {};
}

template<typename CT>
inline auto VariContainer<CT>::getElementFromNameAndSubName(std::string_view name_,
															std::string_view subName_) const -> std::shared_ptr<ContainerType const>
{
	for (auto ptr : elements)
	{
		if (ptr->name == name_ && ptr->subName == subName_)
			return ptr;
	}

	return {};
}

template<typename CT>
inline auto VariContainer<CT>::cloneElement(IndexType elementToCopy_) -> IndexType
{
	for (auto ptr : elements)
	{
		ptr->cloneElement(elementToCopy_);
	}
	notValids.push_back(notValids[(size_t)elementToCopy_]);
	return IndexType(size() - 1);
}

template<typename CT>
inline void VariContainer<CT>::cloneTo(VariContainer<CT> &nvc) const
{
	for (auto const& ptr : elements)
	{
		nvc.elements.push_back(ptr->clone());
	}
	nvc.notValids = notValids;
}

template<typename CT>
inline void VariContainer<CT>::removeDerived(DerivedType change)
{
	assert(change != DerivedType::NotDerived);

	std::vector<std::shared_ptr<ContainerType>> derivedElements;
	// wipe derived data
	for (size_t i = 0; i < getSizeOfElementContainer(); ++i)
	{
		auto elementContainer = getElementContainer(i);
		if (elementContainer->derived() >= change)
		{
			derivedElements.push_back(elementContainer);
		}
	}

	for (auto toRemove : derivedElements)
	{
		auto it = std::find(elements.cbegin(), elements.cend(), toRemove);
		if(it != elements.cend())
		{
			elements.erase(it);
		}
	}
}

} // end namespace
#endif
