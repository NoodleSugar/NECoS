#pragma once

#include <necos/manager/ComponentManager.hpp>
#include <necos/manager/EntityManager.hpp>
#include <necos/manager/EventManager.hpp>
#include <necos/manager/SystemManager.hpp>

namespace necos
{

template<class Settings>
class Engine
{
	using IdAllocator	   = typename Settings::IdAllocator;
	using ComponentTypeSet = typename Settings::ComponentTypeSet;
	using SystemTypeSet	   = typename Settings::SystemTypeSet;

	using EntityManager	   = necos::EntityManager<IdAllocator>;
	using ComponentManager = necos::ComponentManager<ComponentTypeSet>;
	using SystemManager	   = necos::SystemManager<ComponentTypeSet, SystemTypeSet>;

public:
	Engine() :
		m_entityManager(m_eventManager),
		m_componentManager(m_eventManager),
		m_systemManager(m_eventManager, m_componentManager) {}

	Engine(const Engine&) = delete;
	Engine(Engine&&)	  = delete;

	Engine& operator=(const Engine&) = delete;
	Engine& operator=(Engine&&)		 = delete;

	EventManager&	  getEventManager() { return m_eventManager; }
	EntityManager&	  getEntityManager() { return m_entityManager; }
	ComponentManager& getComponentManager() { return m_componentManager; }
	SystemManager&	  getSystemManager() { return m_systemManager; }

private:
	EventManager	 m_eventManager;
	EntityManager	 m_entityManager;
	ComponentManager m_componentManager;
	SystemManager	 m_systemManager;
};

} // namespace necos