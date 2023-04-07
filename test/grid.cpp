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

	CHECK_THROWS(Grid({4, 2}, fill));
}

TEST_CASE("Grid(Grid,function)", "[Grid]")
{
	const Size size{2, 2};
	const uint8_t fill = 7;
	auto func = [](uint8_t cell) -> uint8_t
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

TEST_CASE("Grid::overlapAt", "[Grid]")
{
	const Grid grid({4, 4}, {0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1});
	const Grid other({2, 2}, {2, 2, 0, 0});

	REQUIRE(grid.overlapAt({0, 0}, other) == false);
	REQUIRE(grid.overlapAt({1, 1}, other) == false);
	REQUIRE(grid.overlapAt({2, 2}, other) == true);
	REQUIRE(grid.overlapAt({3, 3}, other) == true);
	REQUIRE(grid.overlapAt({0, -1}, other) == true);
}

TEST_CASE("Grid::setAt", "[Grid]")
{
	Grid grid({3, 3}, {0, 0, 0, 0, 0, 0, 1, 1, 1});
	const Grid other({2, 2}, {2, 2, 0, 0});
	const auto equal = [](const Grid& grid, const std::vector<uint8_t>& expected)
	{
		return std::equal(grid.begin(), grid.end(), expected.begin(), expected.end());
	};

	grid.setAt({0, -1}, other);
	REQUIRE(equal(grid, {0, 0, 0, 0, 0, 0, 1, 1, 1}) == true);

	grid.setAt({2, 0}, other);
	REQUIRE(equal(grid, {0, 0, 2, 0, 0, 0, 1, 1, 1}) == true);

	grid.setAt({1, 1}, other);
	REQUIRE(equal(grid, {0, 0, 2, 0, 2, 2, 1, 1, 1}) == true);

	grid.setAt({0, 2}, other);
	REQUIRE(equal(grid, {0, 0, 2, 0, 2, 2, 3, 3, 1}) == true);
}
