#pragma once

#include <necos/utils/id/Id.hpp>

namespace necos
{

namespace internal
{

struct EntityEvent
{
	EntityId entityId;
};

} // namespace internal

struct EntityCreatedEvent : public internal::EntityEvent
{
};

struct EntityDeletedEvent : public internal::EntityEvent
{
};

} // namespace necos