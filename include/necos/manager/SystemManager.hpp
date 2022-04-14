#pragma once

#include <necos/ISystem.hpp>
#include <necos/event/ComponentEvents.hpp>
#include <necos/event/EventEmitter.hpp>
#include <necos/event/SystemEvents.hpp>
#include <necos/utils/Signature.hpp>

#include <EASTL/array.h>
#include <EASTL/bitset.h>
#include <EASTL/fixed_map.h>
#include <EASTL/vector_set.h>

#include <memory>

namespace necos
{

template<class ComponentTypeSet, class SystemTypeSet>
class SystemManager : public EventEmitter
{
	template<class Settings>
	friend class Engine;

	using ThisType = SystemManager<ComponentTypeSet, SystemTypeSet>;

	static constexpr size_t SystemTypeCount = SystemTypeSet::size();

	using ComponentManager = necos::ComponentManager<ComponentTypeSet>;
	using Signature		   = necos::Signature<ComponentTypeSet>;
	using System		   = ISystem<ComponentTypeSet>;
	using SystemPtr		   = std::unique_ptr<System>;
	using SystemMap		   = eastl::fixed_map<SystemTypeId, SystemPtr, SystemTypeCount>;
	using SystemStateMap   = eastl::bitset<SystemTypeCount>;
	using SystemOrder	   = eastl::array<SystemTypeId, SystemTypeCount>;

	template<class S>
	static inline constexpr SystemTypeId systemTypeIdOf = SystemTypeSet::template idOf<S>();

	template<class T>
	using InheritFromSystem = std::is_base_of<System, T>;

public:
	SystemManager(EventManager& rEventManager, ComponentManager& rComponentManager) :
		EventEmitter(rEventManager),
		m_rComponentManager(rComponentManager)
	{
		static_assert(SystemTypeSet::template allOf<InheritFromSystem>(),
					  "Systems Types in SystemTypeSet must inherit from ISystem");

		SystemTypeSet::forEach(
		 [this](auto s)
		 {
			 using S = TrueTypeOf(s);

			 auto system = new S;
			 system->setComponentManager(&m_rComponentManager);

			 m_systems.emplace(systemTypeIdOf<S>, SystemPtr(system));

			 enableSystem<S>();
		 });

		for(auto& [typeId, _]: m_systems)
			m_systemOrder[typeId] = typeId;

		rEventManager.subscribe<ComponentEnabledEvent>(
		 [this](ComponentEnabledEvent e)
		 {
			 for(auto& [_, system]: m_systems)
			 {
				 if(system->getSignature().doesFitIn(e.template getComponentsState<Signature>()))
					 system->m_selection.insert(e.entityId);
			 }
		 });
		rEventManager.subscribe<ComponentDisabledEvent>(
		 [this](ComponentDisabledEvent e)
		 {
			 for(auto& [_, system]: m_systems)
			 {
				 if(system->getSignature().doesFitIn(e.template getComponentsState<Signature>()))
					 system->m_selection.erase(e.entityId);
			 }
		 });

		rEventManager.subscribe<AllComponentDisabledEvent>(
		 [this](AllComponentDisabledEvent e)
		 {
			 for(auto& [_, system]: m_systems)
				 system->m_selection.erase(e.entityId);
		 });
	}

	SystemManager(const ThisType&) = delete;
	SystemManager(ThisType&&)	   = delete;

	SystemManager& operator=(const ThisType&) = delete;
	SystemManager& operator=(ThisType&&)	  = delete;

	void update(float dt) const
	{
		for(auto typeId: m_systemOrder)
		{
			auto& system = m_systems.at(typeId);
			system->template updateSelection<TrueTypeOf(*system.get())>(dt);
		}
	}

	// Systems access

	template<class S>
	S& getSystem()
	{
		return *static_cast<S*>(m_systems.at(systemTypeIdOf<S>).get());
	}

	// Systems activation

	template<class S>
	void enableSystem()
	{
		m_systemStates.set(systemTypeIdOf<S>);
	}

	template<class S>
	void disableSystem()
	{
		m_systemStates.set(systemTypeIdOf<S>, false);
	}

	template<class S>
	bool isSystemEnabled()
	{
		return m_systemStates[systemTypeIdOf<S>];
	}

	// Systems properties

	// template<class S>
	// void setSystemUpdateInterval(float ms);

	// template<class S>
	// void setSystemPriority(SystemPriority);

	// System dependencies

	// template<class S, class Dep>
	// void addDependency();

	// template<class S, class Dep, class... Deps>
	// void addDependency();

	// void updateSystemOrder();

private:
	ComponentManager& m_rComponentManager;

	SystemMap	   m_systems;
	SystemStateMap m_systemStates;
	SystemOrder	   m_systemOrder;

	// eastl::unordered_map<SystemTypeId, DependencySet> m_systemDependencies;
};

} // namespace necos