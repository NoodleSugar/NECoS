#pragma once

#include <EASTL/type_traits.h>

#include <boost/mp11/algorithm.hpp>
#include <boost/mp11/set.hpp>

#include <necos/utils/id/Id.hpp>

namespace necos
{

namespace internal
{
using namespace boost::mp11;

template<class T>
struct _TrueType
{
	using type = T;
};

template<class T>
struct _TrueType<mp_identity<T>>
{
	using type = T;
};

template<class T>
using TrueType = typename _TrueType<T>::type;

} // namespace internal

#define TrueTypeOf(s) internal::TrueType<decltype(s)>

template<class... Types>
class TypeSet : boost::mp11::mp_list<Types...>
{
	using ThisType = TypeSet<Types...>;

public:
	template<class T>
	static constexpr id_t idOf()
	{
		using Id = boost::mp11::mp_find<ThisType, internal::TrueType<T>>;

		static_assert(Id::value < ThisType::size(), "T must be inside the TypeSet");

		return Id::value;
	}

	static constexpr size_t size()
	{
		static_assert(boost::mp11::mp_is_set<ThisType>::value,
					  "TypeSet can't contain duplicates");
		return boost::mp11::mp_size<ThisType>::value;
	}

	template<class F>
	static void forEach(F f)
	{
		boost::mp11::mp_for_each<boost::mp11::mp_transform<boost::mp11::mp_identity, ThisType>>(f);
	}

	template<template<class> class Predicate>
	static constexpr bool allOf()
	{
		return boost::mp11::mp_all_of<ThisType, Predicate>::value;
	}
};

} // namespace necos