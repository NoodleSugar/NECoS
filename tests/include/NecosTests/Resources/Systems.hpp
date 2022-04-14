#pragma once

#include "EngineSettings.hpp"

#include <necos/ISystem.hpp>

class EmptySystem : public necos::ISystem<ComponentTypeSet>
{
public:
	void update(float, EntityId) override {}

	const Signature& getSignature() const override
	{
		static const Signature s = Signature::template create<EmptyComponent>();
		return s;
	}
};

class BasicSystem : public necos::ISystem<ComponentTypeSet>
{
public:
	void update(float, EntityId id) override
	{
		getComponent<BasicComponent>(id).integer++;
	}

	const Signature& getSignature() const override
	{
		static const Signature s = Signature::template create<BasicComponent>();
		return s;
	}
};
