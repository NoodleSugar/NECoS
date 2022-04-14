#pragma once

#include <necos/utils/id/TypeIdFamily.hpp>

#include <EASTL/functional.h>
#include <EASTL/unique_ptr.h>
#include <EASTL/vector.h>
#include <EASTL/vector_map.h>

namespace necos
{

class EventManager
{
	template<class Settings>
	friend class Engine;

	class IHandler;

	template<class Event>
	class Handler;

	template<class Event>
	using Function = eastl::function<void(Event&&)>;

	struct EventTag;

	using EventTypeIdFamily = TypeIdFamily<EventTag>;

	using HandlerPtr	= eastl::unique_ptr<IHandler>;
	using HandlerVector = eastl::vector<HandlerPtr>;
	using HandlerMap	= eastl::vector_map<id_t, HandlerVector>;

public:
	EventManager() = default;

	template<class Event>
	void subscribe(Function<Event> handler)
	{
		m_handlers[EventTypeIdFamily::template idOf<Event>()].emplace_back(
		 eastl::make_unique<Handler<Event>>(eastl::move(handler)));
	}

	template<class Event>
	void push(Event&& rEvent)
	{
		for(const auto& handler: m_handlers[EventTypeIdFamily::template idOf<Event>()])
			callHandler<Event>(handler.get(), std::forward<Event>(rEvent));
	}

private:
	template<class Event>
	void callHandler(void* h, Event&& rEvent)
	{
		auto handler = static_cast<Handler<Event>*>(h);
		(*handler)(std::forward<Event>(rEvent));
	};

private:
	HandlerMap m_handlers;
};

class EventManager::IHandler
{
public:
	virtual ~IHandler() = default;
};

template<class Event>
class EventManager::Handler :
	public EventManager::IHandler,
	public EventManager::Function<Event>
{
public:
	Handler(Function<Event>&& h) :
		Function<Event>(eastl::move(h)) {}

	~Handler() override = default;
};

} // namespace necos