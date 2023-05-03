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

TEST_CASE("wallKick<Arika>", "[gameplay]")
{
	const std::vector<Tetromino> tetrominos = makeBaseMinos<RotationSystem::Super>();
	const Tetromino& minoI = *find(tetrominos, TetrominoType::I);
	const Tetromino& minoL = *find(tetrominos, TetrominoType::L);
	const Tetromino& minoJ = *find(tetrominos, TetrominoType::J);
	const Tetromino& minoT = *find(tetrominos, TetrominoType::T);

	{
		const Grid field{{3, 3}, {0, 0, 0, 0, 0, 0, 0, 1, 0}};
		const Offset offset = basicRotation<RotationSystem::Arika>(minoL, 1);
		const Offset expected{{0, 0}, 0};
		REQUIRE(wallKick<RotationSystem::Arika>(field, minoL, offset) == expected);
	}
	{
		const Grid field{{3, 4}, {0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1, 0}};
		const Offset offset = basicRotation<RotationSystem::Arika>(minoJ, -1);
		const Offset expected{{1, 0}, -1};
		REQUIRE(wallKick<RotationSystem::Arika>(field, minoJ, offset) == expected);
	}
	{
		const Grid field{{5, 8}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0,
		                             0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0}};
		Tetromino minoIr{minoI};
		minoIr += Offset{{-2, 1}, 1};
		const Offset rotateRight{{0, 0}, 1};
		const Offset rotateLeft{{0, 0}, -1};
		REQUIRE(wallKick<RotationSystem::Arika>(field, minoIr, rotateLeft) == Offset{{2, 0}, -1});

		minoIr += Offset{{1, 0}, 0};
		REQUIRE(wallKick<RotationSystem::Arika>(field, minoIr, rotateLeft) == Offset{{0, 0}, 0});

		minoIr += Offset{{0, 1}, 0};
		REQUIRE(wallKick<RotationSystem::Arika>(field, minoIr, rotateLeft) == Offset{{1, 0}, -1});

		minoIr += Offset{{3, 0}, 0};
		REQUIRE(wallKick<RotationSystem::Arika>(field, minoIr, rotateLeft) == Offset{{-1, 0}, -1});

		minoIr += Offset{{-1, 3}, -1};
		REQUIRE(wallKick<RotationSystem::Arika>(field, minoIr, rotateRight) == Offset{{0, -1}, 1});
	}
	{
		const Grid field{{3, 3}, {0, 0, 0, 0, 0, 0, 1, 0, 1}};
		Tetromino minoTr{minoT};
		minoTr += basicRotation<RotationSystem::Arika>(minoTr, 1);
		const Offset offset = basicRotation<RotationSystem::Arika>(minoTr, 1);
		const Offset expected{{0, 0}, 1};
		REQUIRE(wallKick<RotationSystem::Arika>(field, minoTr, offset) == expected);
	}
}
