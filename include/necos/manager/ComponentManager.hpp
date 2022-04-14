#pragma once

#include <necos/event/ComponentEvents.hpp>
#include <necos/event/EntityEvents.hpp>
#include <necos/event/EventEmitter.hpp>
#include <necos/internal/ComponentContainer.hpp>
#include <necos/utils/Signature.hpp>
#include <necos/utils/id/Id.hpp>

#include <boost/mpl/for_each.hpp>

#include <EASTL/fixed_map.h>
#include <EASTL/vector_map.h>

#include <cassert>
#include <memory>
#include <utility>

namespace necos
{

template<class ComponentTypeSet>
class ComponentManager : public EventEmitter
{
	template<class Settings>
	friend class Engine;

	using ThisType = ComponentManager<ComponentTypeSet>;

	static constexpr size_t ComponentTypeCount = ComponentTypeSet::size();

	using Signature				= necos::Signature<ComponentTypeSet>;
	using SignatureMap			= eastl::map<EntityId, Signature>;
	using ComponentContainerPtr = std::unique_ptr<IComponentContainer>;
	using ComponentContainerMap = eastl::fixed_map<ComponentTypeId,
												   ComponentContainerPtr,
												   ComponentTypeCount>;

	template<class C>
	static inline constexpr ComponentTypeId componentTypeIdOf = ComponentTypeSet::template idOf<C>();

public:
	ComponentManager(EventManager& rEventManager) :
		EventEmitter(rEventManager)
	{
		ComponentTypeSet::forEach(
		 [this](auto component)
		 {
			 using C = decltype(component);
			 m_componentContainers[componentTypeIdOf<C>] =
			  std::make_unique<ComponentContainer<C>>();
		 });

		rEventManager.subscribe<EntityCreatedEvent>(
		 [this](EntityCreatedEvent e)
		 {
			 m_entitySignatures.insert(e.entityId);
			 m_componentsStates.insert(e.entityId);
		 });
		rEventManager.subscribe<EntityDeletedEvent>(
		 [this](EntityDeletedEvent e)
		 {
			 detachAllComponents(e.entityId);
			 m_entitySignatures.erase(e.entityId);
			 m_componentsStates.erase(e.entityId);
		 });
	}

	ComponentManager(const ThisType&) = delete;
	ComponentManager(ThisType&&)	  = delete;

	ComponentManager& operator=(const ThisType&) = delete;
	ComponentManager& operator=(ThisType&&)		 = delete;

	// Components access

	template<class C, class... Args>
	void attachComponent(EntityId id, Args&&... rArgs)
	{
		assert(!getComponentContainer<C>().contains(id) &&
			   !isComponentEnabled<C>(id) &&
			   "Component type already attached to this entity");

		getComponentContainer<C>().insert(id, std::forward<Args>(rArgs)...);

		getEntitySignature(id).template insert<C>();

		auto& componentsState = getComponentsState(id);
		componentsState.template insert<C>();

		emit<ComponentEnabledEvent>(id, componentTypeIdOf<C>, &componentsState);
	}

	template<class C>
	void detachComponent(EntityId id)
	{
		assert(isComponentAttached<C>(id) &&
			   "Component type not attached to this entity");

		getComponentContainer<C>().erase(id);

		getEntitySignature(id).template erase<C>();

		auto& componentsState = getComponentsState(id);
		componentsState.template erase<C>();

		emit<ComponentDisabledEvent>(id, componentTypeIdOf<C>, &componentsState);
	}

	void detachAllComponents(EntityId id)
	{
		for(const auto componentTypeId: m_entitySignatures.at(id))
			m_componentContainers.at(componentTypeId)->erase(id);

		getEntitySignature(id).clear();
		getComponentsState(id).clear();

		emit<AllComponentDisabledEvent>(id);
	}

	template<class C>
	bool isComponentAttached(EntityId id)
	{
		return getComponentContainer<C>().contains(id);
	}

	template<class C>
	C& getComponent(EntityId id)
	{
		assert(isComponentAttached<C>(id) &&
			   "Component is not attached to this entity");

		return getComponentContainer<C>().at(id);
	}

	// Components activation

	template<class C>
	void enableComponent(EntityId id)
	{
		assert(!isComponentEnabled<C>(id) &&
			   "Component can not be enabled because it is not disabled");

		auto& componentState = getComponentsState(id);

		componentState.template insert<C>();
		emit<ComponentEnabledEvent>(id, componentTypeIdOf<C>, &componentState);
	}

	template<class C>
	void disableComponent(EntityId id)
	{
		assert(isComponentEnabled<C>(id) &&
			   "Component can not be disabled because it is not enabled");

		auto& componentState = getComponentsState(id);

		componentState.template erase<C>();
		emit<ComponentDisabledEvent>(id, componentTypeIdOf<C>, &componentState);
	}

	template<class C>
	bool isComponentEnabled(EntityId id)
	{
		return getComponentsState(id).template contains<C>();
	}

private:
	Signature& getEntitySignature(EntityId id)
	{
		assert(m_entitySignatures.count(id) == 1 &&
			   "No signature registered for this entity");

		return m_entitySignatures.at(id);
	}

	Signature& getComponentsState(EntityId id)
	{
		assert(m_componentsStates.count(id) == 1 &&
			   "No component state registered for this entity");

		return m_componentsStates.at(id);
	}

	template<class C>
	ComponentContainer<C>& getComponentContainer()
	{
		return static_cast<ComponentContainer<C>&>(
		 *m_componentContainers.at(componentTypeIdOf<C>).get());
	}

private:
	SignatureMap		  m_entitySignatures;
	SignatureMap		  m_componentsStates;
	ComponentContainerMap m_componentContainers;
};

} // namespace necos