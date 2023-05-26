#include <catch2/catch_test_macros.hpp>

#include <algorithm>

#include "grid.hpp"

using namespace raymino;

TEST_CASE("Grid(Size,Grid::Cell)", "[Grid]")
{
	const Size size{2, 2};
	const Grid::Cell fill = 7;

	const Grid grid(size, fill);

	REQUIRE(grid.isSquare() == true);
	REQUIRE(grid.getSize() == size);
	REQUIRE(std::count(grid.begin(), grid.end(), fill) == size.area());
}

TEST_CASE("Grid(Size,vector<Grid::Cell>)", "[Grid]")
{
	const Size size{3, 2};
	const std::vector<Grid::Cell> fill{1, 2, 3, 4, 5, 6};

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
	const Grid::Cell fill = 7;
	auto func = [](Grid::Cell cell) -> Grid::Cell
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

	REQUIRE(grid.overlapAt({0, 0}, other) == 0);
	REQUIRE(grid.overlapAt({1, 1}, other) == 0);
	REQUIRE(grid.overlapAt({2, 2}, other) != 0);
	REQUIRE(grid.overlapAt({3, 3}, other) != 0);
	REQUIRE(grid.overlapAt({0, -1}, other) != 0);
}

TEST_CASE("Grid::setAt", "[Grid]")
{
	Grid grid({3, 3}, {0, 0, 0, 0, 0, 0, 1, 1, 1});
	const Grid other({2, 2}, {2, 2, 0, 0});

	grid.setAt({0, -1}, other);
	REQUIRE(grid == Grid{{3, 3}, {0, 0, 0, 0, 0, 0, 1, 1, 1}});

	grid.setAt({2, 0}, other);
	REQUIRE(grid == Grid{{3, 3}, {0, 0, 2, 0, 0, 0, 1, 1, 1}});

	grid.setAt({1, 1}, other);
	REQUIRE(grid == Grid{{3, 3}, {0, 0, 2, 0, 2, 2, 1, 1, 1}});

	grid.setAt({0, 2}, other);
	REQUIRE(grid == Grid{{3, 3}, {0, 0, 2, 0, 2, 2, 3, 3, 1}});
}

TEST_CASE("Grid::transpose", "[Grid]")
{
	Grid gridS({3, 3}, {1, 2, 3, 4, 5, 6, 7, 8, 9});
	gridS.transpose();
	REQUIRE(gridS == Grid{{3, 3}, {1, 4, 7, 2, 5, 8, 3, 6, 9}});

	Grid gridR({2, 4}, {1, 2, 3, 4, 5, 6, 7, 8});
	gridR.transpose();
	REQUIRE(gridR == Grid{{4, 2}, {1, 3, 5, 7, 2, 4, 6, 8}});
}

TEST_CASE("Grid::reverseRows", "[Grid]")
{
	Grid gridS({3, 3}, {1, 2, 3, 4, 5, 6, 7, 8, 9});
	gridS.reverseRows();
	REQUIRE(gridS == Grid{{3, 3}, {3, 2, 1, 6, 5, 4, 9, 8, 7}});
	gridS.reverseRows();
	REQUIRE(gridS == Grid{{3, 3}, {1, 2, 3, 4, 5, 6, 7, 8, 9}});

	Grid gridR({2, 4}, {1, 2, 3, 4, 5, 6, 7, 8});
	gridR.reverseRows();
	REQUIRE(gridR == Grid{{2, 4}, {2, 1, 4, 3, 6, 5, 8, 7}});
}

TEST_CASE("Grid::rotate", "[Grid]")
{
	Grid grid({2, 2}, {1, 2, 3, 4});

	grid.rotate(1);
	REQUIRE(grid == Grid{{2, 2}, {3, 1, 4, 2}});

	grid.rotate(2);
	REQUIRE(grid == Grid{{2, 2}, {2, 4, 1, 3}});

	grid.rotate(-1);
	REQUIRE(grid == Grid{{2, 2}, {4, 3, 2, 1}});

	grid.rotate(-6);
	REQUIRE(grid == Grid{{2, 2}, {1, 2, 3, 4}});
}

TEST_CASE("Grid::transformCells", "[Grid]")
{
	Grid grid({2, 2}, {1, 2, 3, 4});

	grid.transformCells(
	    [](Grid::Cell cell) -> Grid::Cell
	    {
		    return cell + 3;
	    });
	REQUIRE(grid == Grid{{2, 2}, {4, 5, 6, 7}});
}
