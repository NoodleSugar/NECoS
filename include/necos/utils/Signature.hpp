#pragma once

#include "TypeSet.hpp"

#include <EASTL/bitset.h>
#include <EASTL/fixed_vector.h>
#include <EASTL/initializer_list.h>
#include <EASTL/iterator.h>

namespace necos
{
template<class ComponentTypeSet>
class Signature
{
	using ThisType = Signature<ComponentTypeSet>;

	static constexpr size_t ComponentTypeCount = ComponentTypeSet::size();

	using BitSet = eastl::bitset<ComponentTypeCount>;

	class Iterator;

public:
	Signature(std::initializer_list<ComponentTypeId> typeIdList)
	{
		for(auto typeId: typeIdList)
			m_bitset.set(typeId);
	}

	Signature()				   = default;
	Signature(const ThisType&) = default;
	Signature(ThisType&&)	   = default;

	Signature& operator=(const ThisType&) = default;
	Signature& operator=(ThisType&&)	  = default;

	auto begin() { return Iterator::createBegin(m_bitset); }
	auto end() { return Iterator::createEnd(m_bitset); }

	template<class C>
	void insert()
	{
		m_bitset.set(ComponentTypeSet::template idOf<C>());
	}

	template<class C>
	void erase()
	{
		m_bitset.set(ComponentTypeSet::template idOf<C>(), false);
	}

	void clear()
	{
		m_bitset.reset();
	}

	template<class C>
	bool contains() const
	{
		return m_bitset[ComponentTypeSet::template idOf<C>()];
	}

	bool doesFitIn(const ThisType& signature) const
	{
		return (signature.m_bitset | this->m_bitset) == signature.m_bitset;
	}

	template<class... Types>
	static ThisType create()
	{
		ThisType signature;

		using CTypeSet = TypeSet<Types...>;
		CTypeSet::forEach(
		 [&signature](auto c)
		 { signature.template insert<decltype(c)>(); });

		return signature;
	}

private:
	BitSet m_bitset;
};

template<class ComponentTypeSet>
class Signature<ComponentTypeSet>::Iterator
{
	using BitSet = Signature<ComponentTypeSet>::BitSet;

	Iterator(BitSet& rBitSet, size_t pos) :
		m_rBitSet(rBitSet), m_pos(pos) {}

public:
	using iterator_category = eastl::input_iterator_tag;
	using difference_type	= size_t;
	using value_type		= size_t;
	using pointer			= const size_t*;
	using reference			= const size_t&;

	static Iterator createBegin(BitSet& rBitSet)
	{
		return Iterator(rBitSet, rBitSet.find_first());
	}
	static Iterator createEnd(BitSet& rBitSet)
	{
		size_t pos = rBitSet.find_last();
		pos		   = pos == rBitSet.size() ? pos : pos + 1;
		return Iterator(rBitSet, pos);
	}

	value_type operator*() const
	{
		return m_pos;
	}

	Iterator& operator++()
	{
		m_pos = m_rBitSet.find_next(m_pos);
		return *this;
	}

	Iterator operator++(int)
	{
		Iterator tmp = *this;
		(*this)++;
		return tmp;
	}

	friend bool operator==(const Iterator& it1, const Iterator& it2)
	{
		return &it1.m_rBitSet == &it2.m_rBitSet && it1.m_pos == it2.m_pos;
	};

	friend bool operator!=(const Iterator& it1, const Iterator& it2)
	{
		return !(it1 == it2);
	};

private:
	BitSet& m_rBitSet;
	size_t	m_pos;
};

} // namespace necos