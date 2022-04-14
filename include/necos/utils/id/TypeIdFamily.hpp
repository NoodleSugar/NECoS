#pragma once

#include "Id.hpp"

namespace necos
{

namespace internal
{

template<class Tag>
class TypeIdFamily final
{
public:
	static id_t next() noexcept
	{
		static id_t value = 0;
		return value++;
	}
};

} // namespace internal

template<class Tag>
class TypeIdFamily final
{
public:
	template<class T>
	static id_t idOf() noexcept
	{
		static const id_t id = internal::TypeIdFamily<Tag>::next();
		return id;
	}
};

} // namespace necos