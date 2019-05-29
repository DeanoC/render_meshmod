//
// Created by Computer on 26/07/2018.
//

#pragma once
#ifndef MESHMOD_VARIELEMENTS_H
#define MESHMOD_VARIELEMENTS_H

#include "core/core.h"
#include <string>
#include <vector>

namespace MeshMod {

enum DerivedType
{
	NotDerived = 0,
	DerivedFromTopology,
	DerivedFromPositions,
	DerivedFromAttributes,
};

/**
Short description.
Detailed description
*/
template<typename CT>
class Elements
{
public:
	using IndexType = typename CT::IndexType;
	
	virtual ~Elements() {};

	// name of this type of element ("position", "UV2D", "Colour", etc)
	std::string_view const name;
	// subname. changeable for multiple sets of m_name's ("worldspace", "Tex0",etc)
	std::string subName;

	// this only works if the dest is the same as the source else does nothing!
	virtual void unsafeCopyElementTo(Elements<CT>& dest_, IndexType srcIndex_, IndexType destIndex_) = 0;

	virtual void cloneElement(IndexType const index_) = 0;

	virtual void resize(size_t const size_) = 0;

	virtual size_t size() const = 0;

	virtual std::shared_ptr<Elements<CT>> clone() const = 0;

	virtual DerivedType derived() const = 0;

protected:
	Elements(std::string_view name_)
			: name(name_)
	{};

	Elements &operator=(Elements const&);

};

template<typename DT, typename CT, bool Interpol, DerivedType Derived, int reserveAmnt = 1024>
class BaseElements : public Elements<CT>
{
public:
	using DataType = DT;
	using ContainerType = CT;
	using IndexType = typename CT::IndexType;

	typedef typename std::vector<DataType>::iterator iterator;
	typedef typename std::vector<DataType>::const_iterator const_iterator;
	typedef BaseElements<DT, CT, Interpol, Derived, reserveAmnt> BaseElementType;

	std::vector<DataType> elements;

	BaseElements() : Elements<ContainerType>(DataType::getName())
	{
		elements.reserve(reserveAmnt);
	}

	static constexpr std::string_view const getName()
	{
		return DataType::getName();
	}

	DataType const &at(IndexType const i) const { return elements.at(size_t(i)); };
	DataType &at(IndexType const i) { return elements.at(size_t(i)); };
	DataType const &operator[](IndexType const i) const { return elements[size_t(i)]; };
	DataType &operator[](IndexType const i) { return elements[size_t(i)]; };

	iterator begin() { return elements.begin(); }
	iterator end() { return elements.end(); }
	const_iterator begin() const { return elements.begin(); }
	const_iterator end() const { return elements.end(); }

	const_iterator cbegin() const { return elements.cbegin(); }
	const_iterator cend() const { return elements.cend(); }

	void erase(const_iterator where_) { elements.erase(where_); }
	void erase(const_iterator first_, const_iterator last_) { elements.erase(first_, last_); }

	void push_back(DataType const &data)
	{
		elements.push_back(data);
	};

	IndexType distance(const_iterator it) const
	{
		return (IndexType) std::distance(elements.cbegin(), it);
	}

	IndexType distance(DataType const &data) const
	{
		DataType const* beginPtr = &(*elements.cbegin());
		IndexType index = (IndexType)(&data - beginPtr);
		return index;
	}


	template<typename INTER_TYPE>
	DataType interpolate(size_t const i0, size_t const i1, INTER_TYPE t)
	{
		return elements[i0].interpolate(elements[i1], t);
	}

	template<typename INTER_TYPE>
	DataType interpolate(size_t const i0, size_t const i1, size_t const i2, INTER_TYPE u, INTER_TYPE v)
	{
		return elements[i0].interpolate(elements[i1], elements[i2], u, v);
	}

	//--------
	// Elements interface implementation
	//--------

	void cloneElement(const IndexType index) final
	{
		elements.push_back(elements[size_t(index)]);
	};

	void unsafeCopyElementTo(Elements<CT>& dest, IndexType srcIndex, IndexType destIndex) final
	{
		assert(size_t(srcIndex) < elements.size());

		BaseElementType* ptr = dynamic_cast<BaseElementType*>(&dest);
		if (ptr)
		{
			assert(size_t(destIndex) < ptr->elements.size());
			ptr->elements[size_t(destIndex)] = elements[size_t(srcIndex)];
		}
	}
	void resize(size_t const size_) final
	{
		elements.resize(size_);
	};

	size_t size() const final
	{
		return elements.size();
	}

	std::shared_ptr<Elements<CT>> clone() const final
	{
		return std::make_shared<BaseElementType>(*this);
	}

	DerivedType derived() const final
	{
		return Derived;
	}

};

}

#endif //MESHMOD_VARIELEMENTS_H
