#pragma once

#include <NecosTests/TestsHeader.hpp>

TEST_CASE("Event Manager", "[EventManager]")
{
	EventManager eventManager;
	int			 count = 0;
	int			 var   = 0;

	auto handler = [&](IntEvent&& e)
	{
		var = e.i;
		count++;
	};

	SECTION("Check handler handler")
	{
		REQUIRE_NOTHROW(eventManager.subscribe<IntEvent>(handler));
		REQUIRE_NOTHROW(eventManager.push<IntEvent>(IntEvent{10}));
		REQUIRE_NOTHROW(eventManager.push<EmptyEvent>(EmptyEvent{}));
		CHECK(10 == var);
		CHECK(1 == count);
	}
}