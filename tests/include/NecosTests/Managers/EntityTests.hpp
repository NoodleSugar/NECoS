#pragma once

#include <NecosTests/TestsHeader.hpp>

TEST_CASE("Reallocable id allocation", "[ReusableIdAllocator::allocate]")
{
	ReusableIdAllocator allocator;

	SECTION("Allocate multiple id")
	{
		constexpr int steps = 10;

		std::vector<id_t> ids;
		ids.reserve(steps);

		for(int i = 0; i < steps; ++i)
		{
			id_t id = allocator.allocate();
			REQUIRE_THAT(ids, !VectorContains(id));
			ids.push_back(id);
		}
	}

	SECTION("Reallocate released id")
	{
		id_t firstId = allocator.allocate();
		allocator.free(firstId);
		id_t secondId = allocator.allocate();

		CHECK(firstId == secondId);
	}
}

TEST_CASE("Reallocable id release", "[ReusableIdAllocator::free]")
{
	ReusableIdAllocator allocator;

	constexpr int steps = 10;

	std::vector<id_t> ids;
	ids.reserve(steps);

	for(int i = 0; i < steps; ++i)
		ids.push_back(allocator.allocate());

	SECTION("Release multiple ids")
	{
		for(id_t i = 0; i < steps; ++i)
			REQUIRE_NOTHROW(allocator.free(i));
	}
}

TEST_CASE("Single use id allocation", "[SingleUseIdAllocator::allocate]")
{
	SingleUseIdAllocator allocator;

	SECTION("Allocate multiple ids")
	{
		constexpr int steps = 10;

		std::vector<id_t> ids;
		ids.reserve(steps);

		for(int i = 0; i < steps; ++i)
		{
			id_t id = allocator.allocate();
			REQUIRE_THAT(ids, !VectorContains(id));
			ids.push_back(id);
		}
	}

	SECTION("Reallocate released id")
	{
		id_t firstId = allocator.allocate();
		allocator.free(firstId);
		id_t secondId = allocator.allocate();

		CHECK(firstId != secondId);
	}
}

TEST_CASE("Single use id release", "[SingleUseIdAllocator::free]")
{
	SingleUseIdAllocator allocator;

	constexpr int steps = 10;

	std::vector<id_t> ids;
	ids.reserve(steps);

	for(int i = 0; i < steps; ++i)
		ids.push_back(allocator.allocate());

	SECTION("Release multiple ids")
	{
		for(id_t i = 0; i < steps; ++i)
			REQUIRE_NOTHROW(allocator.free(i));
	}
}

TEST_CASE("Entity Creation", "[EntityManager::createEntity]")
{
	EventManager  eventManager;
	EntityManager entManager(eventManager);

	SECTION("Check uniqueness of entity ids")
	{
		constexpr int steps = 10;

		std::vector<EntityId> entities;
		entities.reserve(steps);

		for(int i = 0; i < steps; ++i)
		{
			EntityId id = entManager.createEntity();
			REQUIRE_THAT(entities, !VectorContains(id));
			entities.push_back(id);
		}
	}
}