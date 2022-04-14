#pragma once

#include <NecosTests/TestsHeader.hpp>

#define COMPONENT_MANAGER_INIT                 \
	EventManager	 eventManager;             \
	EntityManager	 entManager(eventManager); \
	ComponentManager compManager(eventManager);

TEST_CASE("Component attachment", "[ComponentManager::attachComponent]")
{
	COMPONENT_MANAGER_INIT

	EntityId entity = entManager.createEntity();

	SECTION("Check component is well attached")
	{
		REQUIRE_NOTHROW(compManager.attachComponent<EmptyComponent>(entity));
		REQUIRE_NOTHROW(compManager.attachComponent<BasicComponent>(entity));
		REQUIRE(compManager.isComponentAttached<EmptyComponent>(entity));
		REQUIRE(compManager.isComponentAttached<BasicComponent>(entity));
	}

	SECTION("Check attached component reference")
	{
		compManager.attachComponent<EmptyComponent>(entity);
		CHECK(&compManager.getComponent<EmptyComponent>(entity) ==
			  &compManager.getComponent<EmptyComponent>(entity));
	}

	SECTION("Check attached component value")
	{
		compManager.attachComponent<BasicComponent>(entity, 2);
		CHECK(2 == compManager.getComponent<BasicComponent>(entity).integer);
	}
}

TEST_CASE("Component detachment", "[ComponentManager::detachComponent]")
{
	COMPONENT_MANAGER_INIT

	EntityId entity = entManager.createEntity();
	compManager.attachComponent<EmptyComponent>(entity);
	compManager.attachComponent<BasicComponent>(entity);

	SECTION("Check component is well detached")
	{
		REQUIRE_NOTHROW(compManager.detachComponent<EmptyComponent>(entity));
		REQUIRE(!compManager.isComponentAttached<EmptyComponent>(entity));
		REQUIRE(compManager.isComponentAttached<BasicComponent>(entity));
	}
}

TEST_CASE("Component full detachment", "[ComponentManager::detachAllComponents]")
{
	COMPONENT_MANAGER_INIT

	EntityId entity = entManager.createEntity();
	compManager.attachComponent<EmptyComponent>(entity);
	compManager.attachComponent<BasicComponent>(entity);

	SECTION("Check components are well detached")
	{
		REQUIRE_NOTHROW(compManager.detachAllComponents(entity));
		REQUIRE(!compManager.isComponentAttached<EmptyComponent>(entity));
		REQUIRE(!compManager.isComponentAttached<BasicComponent>(entity));
	}
}

TEST_CASE("Component activation", "[ComponentManager::enableComponent]")
{
	COMPONENT_MANAGER_INIT

	EntityId entity = entManager.createEntity();
	compManager.attachComponent<EmptyComponent>(entity);
	compManager.attachComponent<BasicComponent>(entity);

	SECTION("Check component is well activated")
	{
		REQUIRE(compManager.isComponentEnabled<EmptyComponent>(entity));
		REQUIRE(compManager.isComponentEnabled<BasicComponent>(entity));
	}
}

TEST_CASE("Component deactivation", "[ComponentManager::disableComponent]")
{
	COMPONENT_MANAGER_INIT

	EntityId entity = entManager.createEntity();
	compManager.attachComponent<EmptyComponent>(entity);
	compManager.attachComponent<BasicComponent>(entity);

	SECTION("Check component is well deactivated")
	{
		REQUIRE_NOTHROW(compManager.disableComponent<EmptyComponent>(entity));
		REQUIRE(!compManager.isComponentEnabled<EmptyComponent>(entity));
		REQUIRE(compManager.isComponentEnabled<BasicComponent>(entity));
	}

	SECTION("Check component is well reactived")
	{
		REQUIRE_NOTHROW(compManager.disableComponent<EmptyComponent>(entity));
		REQUIRE_NOTHROW(compManager.enableComponent<EmptyComponent>(entity));
		REQUIRE(compManager.isComponentEnabled<EmptyComponent>(entity));
		REQUIRE(compManager.isComponentEnabled<BasicComponent>(entity));
	}
}