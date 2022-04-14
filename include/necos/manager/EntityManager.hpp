#pragma once

#include <necos/event/EntityEvents.hpp>
#include <necos/event/EventEmitter.hpp>
#include <necos/utils/id/Id.hpp>

namespace necos
{

template<class IdAllocator>
class EntityManager : public EventEmitter
{
	template<class Settings>
	friend class Engine;

public:
	EntityManager(EventManager& rEventManager) :
		EventEmitter(rEventManager) {}

	EntityManager(const EntityManager&) = delete;
	EntityManager(EntityManager&&)		= delete;

	EntityManager& operator=(const EntityManager&) = delete;
	EntityManager& operator=(EntityManager&&)	   = delete;

	EntityId createEntity()
	{
		const EntityId id = m_idAllocator.allocate();
		emit<EntityCreatedEvent>(id);
		return id;
	}

	void deleteEntity(EntityId id)
	{
		m_idAllocator.free(id);
		emit<EntityDeletedEvent>(id);
	}

private:
	IdAllocator m_idAllocator;
};

} // namespace necos