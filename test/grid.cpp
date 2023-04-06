#include <catch2/catch_test_macros.hpp>

#include <algorithm>

#include "grid.hpp"

using namespace raymino;

TEST_CASE("Grid(Size,uint8_t)", "[Grid]")
{
	const Size size{2, 2};
	const uint8_t fill = 7;

	const Grid grid(size, fill);

	REQUIRE(grid.isSquare() == true);
	REQUIRE(grid.getSize() == size);
	REQUIRE(std::count(grid.begin(), grid.end(), fill) == size.area());
}

TEST_CASE("Grid(Size,vector<uint8_t>)", "[Grid]")
{
	const Size size{3, 2};
	const std::vector<uint8_t> fill{1, 2, 3, 4, 5, 6};

	const Grid grid(size, fill);

	REQUIRE(grid.isSquare() == false);
	REQUIRE(grid.getSize() == size);
	REQUIRE(std::equal(grid.begin(), grid.end(), fill.begin(), fill.end()) == true);
	REQUIRE(grid.getAt({0, 0}) == fill[0]);
	REQUIRE(grid.getAt({2, 1}) == fill[5]);
}

TEST_CASE("Grid(Grid,function)", "[Grid]")
{
	const Size size{2, 2};
	const uint8_t fill = 7;
	auto func = [](uint8_t cell)
	{
		return cell * 2;
	};

	const Grid grid(Grid(size, fill), func);

	REQUIRE(grid.isSquare() == true);
	REQUIRE(grid.getSize() == size);
	REQUIRE(std::count(grid.begin(), grid.end(), fill * 2) == size.area());
	REQUIRE(grid.getAt({1, 0}) == fill * 2);
	REQUIRE(grid.getAt({1, 2}) == Grid::oobVal);
	REQUIRE(grid.getAt({0, -1}) == Grid::oobVal);
}
