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

TEST_CASE("wallKick<Arika>", "[wallKick][gameplay]")
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
		REQUIRE(wallKick<WallKicks::Arika>(field, minoL, offset) == expected);
	}
	{
		const Grid field{{3, 4}, {0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1, 0}};
		const Offset offset = basicRotation<RotationSystem::Arika>(minoJ, -1);
		const Offset expected{{1, 0}, -1};
		REQUIRE(wallKick<WallKicks::Arika>(field, minoJ, offset) == expected);
	}
	{
		const Grid field{{5, 8}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0,
		                             0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0}};
		Tetromino minoIr{minoI};
		minoIr += Offset{{-2, 1}, 1};
		const Offset rotateRight{{0, 0}, 1};
		const Offset rotateLeft{{0, 0}, -1};
		REQUIRE(wallKick<WallKicks::Arika>(field, minoIr, rotateLeft) == Offset{{2, 0}, -1});

		minoIr += Offset{{1, 0}, 0};
		REQUIRE(wallKick<WallKicks::Arika>(field, minoIr, rotateLeft) == Offset{{0, 0}, 0});

		minoIr += Offset{{0, 1}, 0};
		REQUIRE(wallKick<WallKicks::Arika>(field, minoIr, rotateLeft) == Offset{{1, 0}, -1});

		minoIr += Offset{{3, 0}, 0};
		REQUIRE(wallKick<WallKicks::Arika>(field, minoIr, rotateLeft) == Offset{{-1, 0}, -1});

		minoIr += Offset{{-1, 3}, -1};
		REQUIRE(wallKick<WallKicks::Arika>(field, minoIr, rotateRight) == Offset{{0, -1}, 1});
	}
	{
		const Grid field{{3, 3}, {0, 0, 0, 0, 0, 0, 1, 0, 1}};
		Tetromino minoTr{minoT};
		minoTr += basicRotation<RotationSystem::Arika>(minoTr, 1);
		const Offset offset = basicRotation<RotationSystem::Arika>(minoTr, 1);
		const Offset expected{{0, 0}, 1};
		REQUIRE(wallKick<WallKicks::Arika>(field, minoTr, offset) == expected);
	}
}

TEST_CASE("wallKick<Super>", "[wallKick][gameplay]")
{
	const std::vector<Tetromino> tetrominos = makeBaseMinos<RotationSystem::Super>();
	const Tetromino& minoI = *find(tetrominos, TetrominoType::I);
	const Tetromino& minoJ = *find(tetrominos, TetrominoType::J);
	const Tetromino& minoZ = *find(tetrominos, TetrominoType::Z);
	const Tetromino& minoT = *find(tetrominos, TetrominoType::T);

	{
		const Grid field{{3, 5}, {0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1, 0, 0}};
		const Offset offset = basicRotation<RotationSystem::Super>(minoJ, -1);
		const Offset expected{{1, 2}, -1};
		REQUIRE(wallKick<WallKicks::Super>(field, minoJ, offset) == expected);
	}
	{
		const Grid field{{4, 4}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 1, 1, 0}};
		const Offset offset = basicRotation<RotationSystem::Super>(minoI, 1);
		const Offset expected{{-2, 0}, 1};
		REQUIRE(wallKick<WallKicks::Super>(field, minoI, offset) == expected);
	}
	{
		const Grid field{{3, 5}, {0, 0, 0, 1, 0, 0, 1, 1, 0, 1, 0, 0, 1, 0, 1}};
		const Offset offset = basicRotation<RotationSystem::Super>(minoZ, 1);
		const Offset expected{{0, 2}, 1};
		REQUIRE(wallKick<WallKicks::Super>(field, minoZ, offset) == expected);
	}
	{
		const Grid field{{3, 3}, {0, 0, 0, 0, 0, 0, 0, 1, 1}};
		const Offset offset = basicRotation<RotationSystem::Super>(minoT, -1);
		const Offset expected{{0, 0}, 0};
		REQUIRE(wallKick<WallKicks::Super>(field, minoT, offset) == expected);
	}
	{
		const Grid field{{3, 5}, {0, 0, 1, 0, 0, 1, 1, 0, 0, 1, 0, 0, 1, 0, 1}};
		const Tetromino minoZr{Tetromino{minoZ} += basicRotation<RotationSystem::Super>(minoZ, 2)};
		const Offset offset = basicRotation<RotationSystem::Super>(minoZr, 1);
		const Offset expected{{1, 2}, 1};
		REQUIRE(wallKick<WallKicks::Super>(field, minoZr, offset) == expected);
	}
}

TEST_CASE("wallKick<None>", "[wallKick][gameplay]")
{
	const std::vector<Tetromino> tetrominos = makeBaseMinos<RotationSystem::Original>();
	const Tetromino& minoO = *find(tetrominos, TetrominoType::O);
	const Tetromino& minoL = *find(tetrominos, TetrominoType::L);
	const Tetromino& minoS = *find(tetrominos, TetrominoType::S);
	const Tetromino& minoT = *find(tetrominos, TetrominoType::T);

	const Grid field{{6, 6}, 0};
	const Offset offset{{1, 1}, 1};
	const Offset expected{{0, 0}, 0};

	REQUIRE(wallKick<WallKicks::None>(field, minoO, offset) == expected);
	REQUIRE(wallKick<WallKicks::None>(field, minoL, offset) == expected);
	REQUIRE(wallKick<WallKicks::None>(field, minoS, offset) == expected);
	REQUIRE(wallKick<WallKicks::None>(field, minoT, offset) == expected);
}

TEST_CASE("eraseFullLines", "[gameplay]")
{
	{
		Grid emptyGrid{{3, 3}, 0};
		Grid fullGrid{{3, 3}, 1};

		REQUIRE(eraseFullLines(emptyGrid) == 0);

		REQUIRE(eraseFullLines(fullGrid) == 3);
		REQUIRE(emptyGrid == fullGrid);
	}
	{
		Grid grid{{3, 4}, {0, 0, 0, 1, 0, 0, 1, 1, 1, 0, 1, 1}};
		const Grid expected{{3, 4}, {0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 1, 1}};

		REQUIRE(eraseFullLines(grid) == 1);
		REQUIRE(grid == expected);
	}
	{
		Grid grid{{3, 3}, {1, 0, 0, 1, 1, 0, 0, 1, 1}};
		const Grid expected{{3, 3}, {1, 0, 0, 1, 1, 0, 0, 1, 1}};

		REQUIRE(eraseFullLines(grid) == 0);
		REQUIRE(grid == expected);
	}
	{
		Grid grid{{3, 3}, {1, 1, 1, 0, 0, 0, 0, 0, 0}};
		const Grid expected{{3, 3}, {0, 0, 0, 0, 0, 0, 0, 0, 0}};

		REQUIRE(eraseFullLines(grid) == 1);
		REQUIRE(grid == expected);
	}
}

TEST_CASE("tSpinCornerCount", "[gameplay]")
{
	std::vector<Tetromino> tetrominos = makeBaseMinos<RotationSystem::Super>();
	Tetromino& minoT = *find(tetrominos, TetrominoType::T);

	{
		const Grid field{{3, 3}, {0, 0, 0, 0, 0, 0, 0, 1, 0}};
		const TSpinCornerCountResult expected{0, 0};
		REQUIRE(tSpinCornerCount(field, minoT) == expected);
	}

	minoT += Offset{{}, 1};

	{
		const Grid field{{3, 3}, {0, 0, 1, 0, 0, 0, 1, 0, 0}};
		const TSpinCornerCountResult expected{1, 1};
		REQUIRE(tSpinCornerCount(field, minoT) == expected);
	}

	minoT += Offset{{}, 1};

	{
		const Grid field{{3, 3}, {0, 1, 1, 0, 0, 0, 1, 0, 1}};
		const TSpinCornerCountResult expected{2, 1};
		REQUIRE(tSpinCornerCount(field, minoT) == expected);
	}
}

TEST_CASE("IScoringSystems", "[gameplay][IScoringSystem]")
{
	{
		const std::unique_ptr<IScoringSystem> scoreSys = makeScoringSystem<ScoringSystem::BPS>();
		REQUIRE(scoreSys->process(ScoreEvent::LineClear, 2, 4) == 100);
		REQUIRE(scoreSys->process(ScoreEvent::TSpin, 3, 1) == 300);
	}
	{
		const std::unique_ptr<IScoringSystem> scoreSys = makeScoringSystem<ScoringSystem::Sega>();
		REQUIRE(scoreSys->process(ScoreEvent::PerfectClear, 3, 3) == 0);
		REQUIRE(scoreSys->process(ScoreEvent::MiniTSpin, 2, 9) == 2000);
		REQUIRE(scoreSys->process(ScoreEvent::SoftDrop, 7, 2) == 7);
	}
	{
		const std::unique_ptr<IScoringSystem> scoreSys = makeScoringSystem<ScoringSystem::Nintendo>();
		REQUIRE(scoreSys->process(ScoreEvent::LineClear, 4, 1) == 1200);
		REQUIRE(scoreSys->process(ScoreEvent::LineClear, 2, 3) == 300);
	}
}
