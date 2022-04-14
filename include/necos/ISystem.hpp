#pragma once

#include <necos/manager/ComponentManager.hpp>
#include <necos/utils/Signature.hpp>
#include <necos/utils/id/Id.hpp>

#include <EASTL/vector_set.h>

namespace necos
{

template<class ComponentTypeSet>
class ISystem
{
	template<class CTypeSet, class STypeSet>
	friend class SystemManager;

	using ThisType = ISystem<ComponentTypeSet>;

protected:
	using EntityId		   = necos::EntityId;
	using Signature		   = necos::Signature<ComponentTypeSet>;
	using Selection		   = eastl::vector_set<EntityId>;
	using ComponentManager = necos::ComponentManager<ComponentTypeSet>;

	template<class C>
	C& getComponent(EntityId id)
	{
		return m_pComponentManager->template getComponent<C>(id);
	}

public:
	ISystem() :
		m_pComponentManager(nullptr) {}

	virtual ~ISystem() = default;

	virtual void update(float dt, EntityId id) = 0;

	virtual const Signature& getSignature() const = 0;

private:
	void setComponentManager(ComponentManager* pComponentManager)
	{
		m_pComponentManager = pComponentManager;
	}

	template<class S>
	void updateSelection(float dt)
	{
		static_assert(std::is_same_v<S, decltype(*this)>,
					  "Template parameter System must be the real class of this object");

		for(EntityId id: m_selection)
		{
			auto& system = static_cast<S&>(*this);
			system.update(dt, id);
		}
	}

private:
	ComponentManager* m_pComponentManager;
	Selection		  m_selection;
};

} // namespace necos