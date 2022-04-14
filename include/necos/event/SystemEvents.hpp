#pragma once

#include <necos/utils/id/Id.hpp>

namespace necos
{

namespace internal
{

struct SystemEvent
{
	SystemTypeId systemTypeId;
};

} // namespace internal

struct SystemAttachedEvent : public internal::SystemEvent
{
};

struct SystemDetachedEvent : public internal::SystemEvent
{
};

struct SystemEnabledEvent : public internal::SystemEvent
{
};

struct SystemDisabledEvent : public internal::SystemEvent
{
};

} // namespace necos