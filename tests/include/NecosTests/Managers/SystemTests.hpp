#pragma once

#include <NecosTests/TestsHeader.hpp>

#define SYSTEM_MANAGER_INIT                     \
	EventManager	 eventManager;              \
	EntityManager	 entManager(eventManager);  \
	ComponentManager compManager(eventManager); \
	SystemManager	 sysManager(eventManager, compManager);

TEST_CASE("System initialization", "[SystemManager::SystemManager]")
{
	SYSTEM_MANAGER_INIT

	SECTION("Check systems are well initialized")
	{
		REQUIRE_NOTHROW(sysManager.getSystem<EmptySystem>());
		REQUIRE_NOTHROW(sysManager.getSystem<BasicSystem>());
	}
}

TEST_CASE("System activation", "[SystemManager::enableSystem]")
{
	SYSTEM_MANAGER_INIT

	SECTION("Check systems are well activated")
	{
		CHECK(sysManager.isSystemEnabled<EmptySystem>());
		CHECK(sysManager.isSystemEnabled<BasicSystem>());
	}
}

TEST_CASE("System deactivation", "[SystemManager::disableSystem]")
{
	SYSTEM_MANAGER_INIT

	SECTION("Check systems are well deactivated")
	{
		REQUIRE_NOTHROW(sysManager.disableSystem<EmptySystem>());
		CHECK(!sysManager.isSystemEnabled<EmptySystem>());
		CHECK(sysManager.isSystemEnabled<BasicSystem>());
	}

	SECTION("Check component is well reactived")
	{
		REQUIRE_NOTHROW(sysManager.disableSystem<EmptySystem>());
		REQUIRE_NOTHROW(sysManager.enableSystem<EmptySystem>());
		CHECK(sysManager.isSystemEnabled<EmptySystem>());
		CHECK(sysManager.isSystemEnabled<BasicSystem>());
	}
}

// TODO all

TEST_CASE("System update", "[SystemManager::update]")
{
	SYSTEM_MANAGER_INIT

	EntityId id = entManager.createEntity();
	compManager.attachComponent<BasicComponent>(id, 0);

	SECTION("Check component is well updated")
	{
		REQUIRE_NOTHROW(sysManager.update(0.0f));
		CHECK(1 == compManager.getComponent<BasicComponent>(id).integer);
	}
}