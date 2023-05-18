#include <catch2/catch_test_macros.hpp>

#include "gameplay.hpp"

using namespace raymino;

TEST_CASE("basicRotation<Super>", "[gameplay][basicRotation]")
{
	std::vector<Tetromino> tetrominos = makeBaseMinos<RotationSystem::Super>();
	Tetromino& minoI = *find(tetrominos, TetrominoType::I);
	Tetromino& minoJ = *find(tetrominos, TetrominoType::J);
	Tetromino& minoS = *find(tetrominos, TetrominoType::S);

	{
		const Offset offset = basicRotation<RotationSystem::Super>(minoI, 2);
		minoI += offset;
		const Grid negative({4, 4}, {1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 1, 1, 1, 1});
		REQUIRE(negative.overlapAt(minoI.position, minoI.collision) == 0);
	}
	{
		const Offset offset = basicRotation<RotationSystem::Super>(minoI, -1);
		minoI += offset;
		const Grid negative({4, 4}, {1, 1, 0, 1, 1, 1, 0, 1, 1, 1, 0, 1, 1, 1, 0, 1});
		REQUIRE(negative.overlapAt(minoI.position, minoI.collision) == 0);
	}

	{
		const Offset offset = basicRotation<RotationSystem::Super>(minoJ, 3);
		minoJ += offset;
		const Grid negative({3, 3}, {1, 0, 1, 1, 0, 1, 0, 0, 1});
		REQUIRE(negative.overlapAt(minoJ.position, minoJ.collision) == 0);
	}
	{
		const Offset offset = basicRotation<RotationSystem::Super>(minoJ, 1);
		minoJ += offset;
		const Grid negative({3, 3}, {0, 1, 1, 0, 0, 0, 1, 1, 1});
		REQUIRE(negative.overlapAt(minoJ.position, minoJ.collision) == 0);
	}

	{
		const Offset offset = basicRotation<RotationSystem::Super>(minoS, -1);
		minoS += offset;
		const Grid negative({3, 3}, {0, 1, 1, 0, 0, 1, 1, 0, 1});
		REQUIRE(negative.overlapAt(minoS.position, minoS.collision) == 0);
	}
	{
		const Offset offset = basicRotation<RotationSystem::Super>(minoS, -3);
		minoS += offset;
		const Grid negative({3, 3}, {1, 0, 0, 0, 0, 1, 1, 1, 1});
		REQUIRE(negative.overlapAt(minoS.position, minoS.collision) == 0);
	}
}

TEST_CASE("basicRotation<Sega>", "[gameplay][basicRotation]")
{
	std::vector<Tetromino> tetrominos = makeBaseMinos<RotationSystem::Sega>();
	Tetromino& minoI = *find(tetrominos, TetrominoType::I);
	Tetromino& minoL = *find(tetrominos, TetrominoType::L);
	Tetromino& minoZ = *find(tetrominos, TetrominoType::Z);

	REQUIRE(basicRotation<RotationSystem::Sega>(minoI, 0) == Offset{{0, 0}, 0});
	REQUIRE(basicRotation<RotationSystem::Sega>(minoI, 4) == Offset{{0, 0}, 0});

	{
		const Offset offset = basicRotation<RotationSystem::Sega>(minoI, 2);
		minoI += offset;
		const Grid negative({4, 4}, {1, 1, 1, 1, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1});
		REQUIRE(negative.overlapAt(minoI.position, minoI.collision) == 0);
	}
	{
		const Offset offset = basicRotation<RotationSystem::Sega>(minoI, -1);
		minoI += offset;
		const Grid negative({4, 4}, {1, 1, 0, 1, 1, 1, 0, 1, 1, 1, 0, 1, 1, 1, 0, 1});
		REQUIRE(negative.overlapAt(minoI.position, minoI.collision) == 0);
	}
	{
		const Offset offset = basicRotation<RotationSystem::Sega>(minoI, -3);
		minoI += offset;
		const Grid negative({4, 4}, {1, 1, 1, 1, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1});
		REQUIRE(negative.overlapAt(minoI.position, minoI.collision) == 0);
	}

	{
		const Offset offset = basicRotation<RotationSystem::Sega>(minoL, 3);
		minoL += offset;
		const Grid negative({3, 3}, {1, 0, 1, 1, 0, 1, 1, 0, 0});
		REQUIRE(negative.overlapAt(minoL.position, minoL.collision) == 0);
	}
	{
		const Offset offset = basicRotation<RotationSystem::Sega>(minoL, -1);
		minoL += offset;
		const Grid negative({3, 3}, {1, 1, 1, 1, 1, 0, 0, 0, 0});
		REQUIRE(negative.overlapAt(minoL.position, minoL.collision) == 0);
	}

	{
		const Offset offset = basicRotation<RotationSystem::Sega>(minoZ, 1);
		minoZ += offset;
		const Grid negative({3, 3}, {1, 1, 0, 1, 0, 0, 1, 0, 1});
		REQUIRE(negative.overlapAt(minoZ.position, minoZ.collision) == 0);
	}
	{
		const Offset offset = basicRotation<RotationSystem::Sega>(minoZ, 2);
		minoZ += offset;
		const Grid negative({3, 3}, {1, 1, 0, 1, 0, 0, 1, 0, 1});
		REQUIRE(negative.overlapAt(minoZ.position, minoZ.collision) == 0);
	}
}

TEST_CASE("basicRotation<Arika>", "[gameplay][basicRotation]")
{
	std::vector<Tetromino> tetrominos = makeBaseMinos<RotationSystem::Arika>();
	Tetromino& minoO = *find(tetrominos, TetrominoType::O);
	Tetromino& minoT = *find(tetrominos, TetrominoType::T);
	Tetromino& minoS = *find(tetrominos, TetrominoType::S);

	{
		const Offset offset = basicRotation<RotationSystem::Arika>(minoO, 2);
		minoO += offset;
		const Grid negative({4, 4}, {0, 0, 1, 1, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1});
		REQUIRE(negative.overlapAt(minoO.position, minoO.collision) == 0);
	}
	{
		const Offset offset = basicRotation<RotationSystem::Arika>(minoO, -1);
		minoO += offset;
		const Grid negative({4, 4}, {0, 0, 1, 1, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1});
		REQUIRE(negative.overlapAt(minoO.position, minoO.collision) == 0);
	}

	{
		const Offset offset = basicRotation<RotationSystem::Arika>(minoT, 2);
		minoT += offset;
		const Grid negative({3, 3}, {1, 1, 1, 1, 0, 1, 0, 0, 0});
		REQUIRE(negative.overlapAt(minoT.position, minoT.collision) == 0);
	}
	{
		const Offset offset = basicRotation<RotationSystem::Arika>(minoT, -2);
		minoT += offset;
		const Grid negative({3, 3}, {1, 1, 1, 0, 0, 0, 1, 0, 1});
		REQUIRE(negative.overlapAt(minoT.position, minoT.collision) == 0);
	}

	{
		const Offset offset = basicRotation<RotationSystem::Arika>(minoS, -1);
		minoS += offset;
		const Grid negative({3, 3}, {0, 1, 1, 0, 0, 1, 1, 0, 1});
		REQUIRE(negative.overlapAt(minoS.position, minoS.collision) == 0);
	}
	{
		const Offset offset = basicRotation<RotationSystem::Arika>(minoS, 3);
		minoS += offset;
		const Grid negative({3, 3}, {1, 1, 1, 1, 0, 0, 0, 0, 1});
		REQUIRE(negative.overlapAt(minoS.position, minoS.collision) == 0);
	}
}

TEST_CASE("basicRotation<Original>", "[gameplay][basicRotation]")
{
	std::vector<Tetromino> tetrominos = makeBaseMinos<RotationSystem::Original>();
	Tetromino& minoI = *find(tetrominos, TetrominoType::I);
	Tetromino& minoJ = *find(tetrominos, TetrominoType::J);
	Tetromino& minoS = *find(tetrominos, TetrominoType::S);
	const Tetromino& minoO = *find(tetrominos, TetrominoType::O);

	REQUIRE(basicRotation<RotationSystem::Original>(minoO, 1) == Offset{{0, 0}, 0});

	{
		const Offset offset = basicRotation<RotationSystem::Original>(minoI, -4);
		minoI += offset;
		const Grid negative({4, 4}, {1, 1, 1, 1, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1});
		REQUIRE(negative.overlapAt(minoI.position, minoI.collision) == 0);
	}
	{
		const Offset offset = basicRotation<RotationSystem::Original>(minoI, 3);
		minoI += offset;
		const Grid negative({4, 4}, {1, 1, 0, 1, 1, 1, 0, 1, 1, 1, 0, 1, 1, 1, 0, 1});
		REQUIRE(negative.overlapAt(minoI.position, minoI.collision) == 0);
	}

	{
		const Offset offset = basicRotation<RotationSystem::Original>(minoJ, -2);
		minoJ += offset;
		const Grid negative({3, 3}, {0, 1, 1, 0, 0, 0, 1, 1, 1});
		REQUIRE(negative.overlapAt(minoJ.position, minoJ.collision) == 0);
	}
	{
		const Offset offset = basicRotation<RotationSystem::Original>(minoJ, -1);
		minoJ += offset;
		const Grid negative({3, 3}, {1, 0, 1, 1, 0, 1, 0, 0, 1});
		REQUIRE(negative.overlapAt(minoJ.position, minoJ.collision) == 0);
	}

	{
		const Offset offset = basicRotation<RotationSystem::Original>(minoS, 2);
		minoS += offset;
		const Grid negative({3, 3}, {1, 1, 1, 1, 0, 0, 0, 0, 1});
		REQUIRE(negative.overlapAt(minoS.position, minoS.collision) == 0);
	}
	{
		const Offset offset = basicRotation<RotationSystem::Original>(minoS, 3);
		minoS += offset;
		const Grid negative({3, 3}, {1, 0, 1, 1, 0, 0, 1, 1, 0});
		REQUIRE(negative.overlapAt(minoS.position, minoS.collision) == 0);
	}
}

TEST_CASE("basicRotation<NintendoLeft>", "[gameplay][basicRotation]")
{
	std::vector<Tetromino> tetrominos = makeBaseMinos<RotationSystem::NintendoLeft>();
	Tetromino& minoI = *find(tetrominos, TetrominoType::I);
	Tetromino& minoT = *find(tetrominos, TetrominoType::T);
	Tetromino& minoZ = *find(tetrominos, TetrominoType::Z);
	const Tetromino& minoO = *find(tetrominos, TetrominoType::O);

	REQUIRE(basicRotation<RotationSystem::NintendoLeft>(minoO, 1) == Offset{{0, 0}, 0});

	{
		const Offset offset = basicRotation<RotationSystem::NintendoLeft>(minoI, 6);
		minoI += offset;
		const Grid negative({4, 4}, {1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 1, 1, 1, 1});
		REQUIRE(negative.overlapAt(minoI.position, minoI.collision) == 0);
	}
	{
		const Offset offset = basicRotation<RotationSystem::NintendoLeft>(minoI, 5);
		minoI += offset;
		const Grid negative({4, 4}, {1, 1, 0, 1, 1, 1, 0, 1, 1, 1, 0, 1, 1, 1, 0, 1});
		REQUIRE(negative.overlapAt(minoI.position, minoI.collision) == 0);
	}

	{
		const Offset offset = basicRotation<RotationSystem::NintendoLeft>(minoT, -1);
		minoT += offset;
		const Grid negative({3, 3}, {1, 0, 1, 1, 0, 0, 1, 0, 1});
		REQUIRE(negative.overlapAt(minoT.position, minoT.collision) == 0);
	}
	{
		const Offset offset = basicRotation<RotationSystem::NintendoLeft>(minoT, 3);
		minoT += offset;
		const Grid negative({3, 3}, {1, 0, 1, 0, 0, 0, 1, 1, 1});
		REQUIRE(negative.overlapAt(minoT.position, minoT.collision) == 0);
	}

	{
		const Offset offset = basicRotation<RotationSystem::NintendoLeft>(minoZ, -2);
		minoZ += offset;
		const Grid negative({3, 3}, {1, 1, 1, 0, 0, 1, 1, 0, 0});
		REQUIRE(negative.overlapAt(minoZ.position, minoZ.collision) == 0);
	}
	{
		const Offset offset = basicRotation<RotationSystem::NintendoLeft>(minoZ, -1);
		minoZ += offset;
		const Grid negative({3, 3}, {1, 0, 1, 0, 0, 1, 0, 1, 1});
		REQUIRE(negative.overlapAt(minoZ.position, minoZ.collision) == 0);
	}
}

TEST_CASE("basicRotation<NintendoRight>", "[gameplay][basicRotation]")
{
	std::vector<Tetromino> tetrominos = makeBaseMinos<RotationSystem::NintendoRight>();
	Tetromino& minoI = *find(tetrominos, TetrominoType::I);
	Tetromino& minoJ = *find(tetrominos, TetrominoType::J);
	Tetromino& minoS = *find(tetrominos, TetrominoType::S);
	const Tetromino& minoO = *find(tetrominos, TetrominoType::O);

	REQUIRE(basicRotation<RotationSystem::NintendoRight>(minoO, 1) == Offset{{0, 0}, 0});

	{
		const Offset offset = basicRotation<RotationSystem::NintendoRight>(minoI, 1);
		minoI += offset;
		const Grid negative({4, 4}, {1, 1, 0, 1, 1, 1, 0, 1, 1, 1, 0, 1, 1, 1, 0, 1});
		REQUIRE(negative.overlapAt(minoI.position, minoI.collision) == 0);
	}
	{
		const Offset offset = basicRotation<RotationSystem::NintendoRight>(minoI, -1);
		minoI += offset;
		const Grid negative({4, 4}, {1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 1, 1, 1, 1});
		REQUIRE(negative.overlapAt(minoI.position, minoI.collision) == 0);
	}

	{
		const Offset offset = basicRotation<RotationSystem::NintendoRight>(minoJ, 1);
		minoJ += offset;
		const Grid negative({3, 3}, {1, 0, 1, 1, 0, 1, 0, 0, 1});
		REQUIRE(negative.overlapAt(minoJ.position, minoJ.collision) == 0);
	}
	{
		const Offset offset = basicRotation<RotationSystem::NintendoRight>(minoJ, 2);
		minoJ += offset;
		const Grid negative({3, 3}, {1, 0, 0, 1, 0, 1, 1, 0, 1});
		REQUIRE(negative.overlapAt(minoJ.position, minoJ.collision) == 0);
	}

	{
		const Offset offset = basicRotation<RotationSystem::NintendoRight>(minoS, 3);
		minoS += offset;
		const Grid negative({3, 3}, {1, 0, 1, 1, 0, 0, 1, 1, 0});
		REQUIRE(negative.overlapAt(minoS.position, minoS.collision) == 0);
	}
	{
		const Offset offset = basicRotation<RotationSystem::NintendoRight>(minoS, 3);
		minoS += offset;
		const Grid negative({3, 3}, {1, 1, 1, 1, 0, 0, 0, 0, 1});
		REQUIRE(negative.overlapAt(minoS.position, minoS.collision) == 0);
	}
}
