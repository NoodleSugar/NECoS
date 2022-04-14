#pragma once

#include <necos/utils/id/Id.hpp>

namespace necos
{

namespace internal
{

struct ComponentEvent
{
	EntityId		entityId;
	ComponentTypeId componentTypeId;
	const void*		componentsState;

	template<class Signature>
	const Signature& getComponentsState()
	{
		return *static_cast<const Signature*>(componentsState);
	}
};

} // namespace internal

struct ComponentEnabledEvent : public internal::ComponentEvent
{
};

struct ComponentDisabledEvent : public internal::ComponentEvent
{
};

struct AllComponentDisabledEvent
{
	ComponentTypeId entityId;
};

} // namespace necos