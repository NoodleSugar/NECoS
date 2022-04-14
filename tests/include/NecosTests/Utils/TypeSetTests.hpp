#pragma once

#include <NecosTests/TestsHeader.hpp>

TEST_CASE("Testing TypeSet", "[TypeSet]")
{
	int count = 0;
	CustomTypeSet::forEach(
	 [&](auto t)
	 { count++; });

	REQUIRE(3 == CustomTypeSet::size());
	REQUIRE(0 == CustomTypeSet::template idOf<int>());
	REQUIRE(1 == CustomTypeSet::template idOf<float>());
	REQUIRE(2 == CustomTypeSet::template idOf<char>());
	REQUIRE(3 == count);
}