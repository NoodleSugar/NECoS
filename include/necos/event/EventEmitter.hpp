#pragma once

#include <necos/manager/EventManager.hpp>

#include <utility>

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
		m_rEventManager.push(Event{std::forward<Args>(rArgs)...});
	}

private:
	EventManager& m_rEventManager;
};

} // namespace necos