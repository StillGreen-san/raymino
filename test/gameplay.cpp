#include <catch2/catch_test_macros.hpp>

#include "gameplay.hpp"

using namespace raymino;

TEST_CASE("findTrueSize", "[gameplay]")
{
	{
		const Grid grid{{4, 4}, {0, 0, 1, 1, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0}};
		const Rect expected{{2, 0}, {2, 2}};
		REQUIRE(findTrueSize(grid) == expected);
	}
	{
		const Grid grid{{3, 3}, {0, 0, 1, 1, 1, 1, 0, 0, 0}};
		const Rect expected{{0, 0}, {3, 2}};
		REQUIRE(findTrueSize(grid) == expected);
	}
	{
		const Grid grid{{3, 3}, {0, 0, 0, 1, 1, 1, 0, 1, 0}};
		const Rect expected{{0, 1}, {3, 2}};
		REQUIRE(findTrueSize(grid) == expected);
	}
	{
		const Grid grid{{4, 3}, {0, 1, 1, 0, 0, 1, 0, 0, 0, 1, 0, 0}};
		const Rect expected{{1, 0}, {2, 3}};
		REQUIRE(findTrueSize(grid) == expected);
	}
}
