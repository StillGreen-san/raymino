#include "ostream.hpp" //! needs to be included before catch

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

TEST_CASE("spawnPosition", "[gameplay]")
{
	{
		const Tetromino tetromino{{}, {}, {{2, 2}, {1, 1, 1, 1}}};
		const XY expected{4, 4};
		REQUIRE(spawnPosition(tetromino, 4, 10) == expected);
	}
	{
		const Tetromino tetromino{{}, {}, {{3, 3}, {0, 0, 1, 0, 0, 1, 0, 1, 1}}};
		const XY expected{3, 4};
		REQUIRE(spawnPosition(tetromino, 4, 10) == expected);
	}
	{
		const Tetromino tetromino{{}, {}, {{3, 3}, {0, 0, 0, 1, 1, 0, 0, 1, 1}}};
		const XY expected{3, -1};
		REQUIRE(spawnPosition(tetromino, 0, 10) == expected);
	}
	{
		const Tetromino tetromino{{}, {}, {{4, 4}, {0, 0, 0, 0, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0}}};
		const XY expected{3, 1};
		REQUIRE(spawnPosition(tetromino, 2, 10) == expected);
	}
}
