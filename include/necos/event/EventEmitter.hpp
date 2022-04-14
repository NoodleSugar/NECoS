#pragma once

#include <necos/manager/EventManager.hpp>

#include <EASTL/type_traits.h>

namespace necos
{

class EventEmitter
{
public:
	EventEmitter(EventManager& rEventManager) :
		m_rEventManager(rEventManager) {}

	virtual ~EventEmitter() = default;

	template<class Event, class... Args>
	void emit(Args&&... rArgs)
	{
		m_rEventManager.push(Event{eastl::forward<Args>(rArgs)...});
	}

private:
	EventManager& m_rEventManager;
};

} // namespace necos