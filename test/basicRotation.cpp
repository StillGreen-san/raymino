#include <catch2/catch_test_macros.hpp>

#include "gameplay.hpp"

using namespace raymino;

TEST_CASE("basicRotation<Super>", "[basicRotation]")
{
	std::vector<Tetromino> tetrominos = makeBaseMinos<RotationSystem::Super>();
	Tetromino& minoI = *find(tetrominos, TetrominoType::I);
	Tetromino& minoJ = *find(tetrominos, TetrominoType::J);
	Tetromino& minoS = *find(tetrominos, TetrominoType::S);

	{
		const Offset offset = basicRotation<RotationSystem::Super>(minoI, 2);
		minoI += offset;
		const Grid negative({4, 4}, {1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 1, 1, 1, 1});
		REQUIRE(negative.overlapAt(minoI.position, minoI.collision) == false);
	}
	{
		const Offset offset = basicRotation<RotationSystem::Super>(minoI, -1);
		minoI += offset;
		const Grid negative({4, 4}, {1, 1, 0, 1, 1, 1, 0, 1, 1, 1, 0, 1, 1, 1, 0, 1});
		REQUIRE(negative.overlapAt(minoI.position, minoI.collision) == false);
	}

	{
		const Offset offset = basicRotation<RotationSystem::Super>(minoJ, 3);
		minoJ += offset;
		const Grid negative({3, 3}, {1, 0, 1, 1, 0, 1, 0, 0, 1});
		REQUIRE(negative.overlapAt(minoJ.position, minoJ.collision) == false);
	}
	{
		const Offset offset = basicRotation<RotationSystem::Super>(minoJ, 1);
		minoJ += offset;
		const Grid negative({3, 3}, {0, 1, 1, 0, 0, 0, 1, 1, 1});
		REQUIRE(negative.overlapAt(minoJ.position, minoJ.collision) == false);
	}

	{
		const Offset offset = basicRotation<RotationSystem::Super>(minoS, -1);
		minoS += offset;
		const Grid negative({3, 3}, {0, 1, 1, 0, 0, 1, 1, 0, 1});
		REQUIRE(negative.overlapAt(minoS.position, minoS.collision) == false);
	}
	{
		const Offset offset = basicRotation<RotationSystem::Super>(minoS, -3);
		minoS += offset;
		const Grid negative({3, 3}, {1, 0, 0, 0, 0, 1, 1, 1, 1});
		REQUIRE(negative.overlapAt(minoS.position, minoS.collision) == false);
	}
}

TEST_CASE("basicRotation<Sega>", "[basicRotation]")
{
	std::vector<Tetromino> tetrominos = makeBaseMinos<RotationSystem::Sega>();
	Tetromino& minoI = *find(tetrominos, TetrominoType::I);
	Tetromino& minoL = *find(tetrominos, TetrominoType::L);
	Tetromino& minoZ = *find(tetrominos, TetrominoType::Z);

	{
		const Offset offset = basicRotation<RotationSystem::Sega>(minoI, 2);
		minoI += offset;
		const Grid negative({4, 4}, {1, 1, 1, 1, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1});
		REQUIRE(negative.overlapAt(minoI.position, minoI.collision) == false);
	}
	{
		const Offset offset = basicRotation<RotationSystem::Sega>(minoI, -1);
		minoI += offset;
		const Grid negative({4, 4}, {1, 1, 0, 1, 1, 1, 0, 1, 1, 1, 0, 1, 1, 1, 0, 1});
		REQUIRE(negative.overlapAt(minoI.position, minoI.collision) == false);
	}
	{
		const Offset offset = basicRotation<RotationSystem::Sega>(minoI, -3);
		minoI += offset;
		const Grid negative({4, 4}, {1, 1, 1, 1, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1});
		REQUIRE(negative.overlapAt(minoI.position, minoI.collision) == false);
	}

	{
		const Offset offset = basicRotation<RotationSystem::Sega>(minoL, 3);
		minoL += offset;
		const Grid negative({3, 3}, {1, 0, 1, 1, 0, 1, 1, 0, 0});
		REQUIRE(negative.overlapAt(minoL.position, minoL.collision) == false);
	}
	{
		const Offset offset = basicRotation<RotationSystem::Sega>(minoL, -1);
		minoL += offset;
		const Grid negative({3, 3}, {1, 1, 1, 1, 1, 0, 0, 0, 0});
		REQUIRE(negative.overlapAt(minoL.position, minoL.collision) == false);
	}

	{
		const Offset offset = basicRotation<RotationSystem::Sega>(minoZ, 1);
		minoZ += offset;
		const Grid negative({3, 3}, {1, 1, 0, 1, 0, 0, 1, 0, 1});
		REQUIRE(negative.overlapAt(minoZ.position, minoZ.collision) == false);
	}
	{
		const Offset offset = basicRotation<RotationSystem::Sega>(minoZ, 2);
		minoZ += offset;
		const Grid negative({3, 3}, {1, 1, 0, 1, 0, 0, 1, 0, 1});
		REQUIRE(negative.overlapAt(minoZ.position, minoZ.collision) == false);
	}
}

TEST_CASE("basicRotation<Arika>", "[basicRotation]")
{
	std::vector<Tetromino> tetrominos = makeBaseMinos<RotationSystem::Arika>();
	Tetromino& minoO = *find(tetrominos, TetrominoType::O);
	Tetromino& minoT = *find(tetrominos, TetrominoType::T);
	Tetromino& minoS = *find(tetrominos, TetrominoType::S);

	{
		const Offset offset = basicRotation<RotationSystem::Arika>(minoO, 2);
		minoO += offset;
		const Grid negative({4, 4}, {0, 0, 1, 1, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1});
		REQUIRE(negative.overlapAt(minoO.position, minoO.collision) == false);
	}
	{
		const Offset offset = basicRotation<RotationSystem::Arika>(minoO, -1);
		minoO += offset;
		const Grid negative({4, 4}, {0, 0, 1, 1, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1});
		REQUIRE(negative.overlapAt(minoO.position, minoO.collision) == false);
	}

	{
		const Offset offset = basicRotation<RotationSystem::Arika>(minoT, 2);
		minoT += offset;
		const Grid negative({3, 3}, {1, 1, 1, 1, 0, 1, 0, 0, 0});
		REQUIRE(negative.overlapAt(minoT.position, minoT.collision) == false);
	}
	{
		const Offset offset = basicRotation<RotationSystem::Arika>(minoT, -2);
		minoT += offset;
		const Grid negative({3, 3}, {1, 1, 1, 0, 0, 0, 1, 0, 1});
		REQUIRE(negative.overlapAt(minoT.position, minoT.collision) == false);
	}

	{
		const Offset offset = basicRotation<RotationSystem::Arika>(minoS, -1);
		minoS += offset;
		const Grid negative({3, 3}, {0, 1, 1, 0, 0, 1, 1, 0, 1});
		REQUIRE(negative.overlapAt(minoS.position, minoS.collision) == false);
	}
	{
		const Offset offset = basicRotation<RotationSystem::Arika>(minoS, 3);
		minoS += offset;
		const Grid negative({3, 3}, {1, 1, 1, 1, 0, 0, 0, 0, 1});
		REQUIRE(negative.overlapAt(minoS.position, minoS.collision) == false);
	}
}

TEST_CASE("basicRotation<Original>", "[basicRotation]")
{
	std::vector<Tetromino> tetrominos = makeBaseMinos<RotationSystem::Original>();
	Tetromino& minoI = *find(tetrominos, TetrominoType::I);
	Tetromino& minoJ = *find(tetrominos, TetrominoType::J);
	Tetromino& minoS = *find(tetrominos, TetrominoType::S);

	{
		const Offset offset = basicRotation<RotationSystem::Original>(minoI, -4);
		minoI += offset;
		const Grid negative({4, 4}, {1, 1, 1, 1, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1});
		REQUIRE(negative.overlapAt(minoI.position, minoI.collision) == false);
	}
	{
		const Offset offset = basicRotation<RotationSystem::Original>(minoI, 3);
		minoI += offset;
		const Grid negative({4, 4}, {1, 1, 0, 1, 1, 1, 0, 1, 1, 1, 0, 1, 1, 1, 0, 1});
		REQUIRE(negative.overlapAt(minoI.position, minoI.collision) == false);
	}

	{
		const Offset offset = basicRotation<RotationSystem::Original>(minoJ, -2);
		minoJ += offset;
		const Grid negative({3, 3}, {0, 1, 1, 0, 0, 0, 1, 1, 1});
		REQUIRE(negative.overlapAt(minoJ.position, minoJ.collision) == false);
	}
	{
		const Offset offset = basicRotation<RotationSystem::Original>(minoJ, -1);
		minoJ += offset;
		const Grid negative({3, 3}, {1, 0, 1, 1, 0, 1, 0, 0, 1});
		REQUIRE(negative.overlapAt(minoJ.position, minoJ.collision) == false);
	}

	{
		const Offset offset = basicRotation<RotationSystem::Original>(minoS, 2);
		minoS += offset;
		const Grid negative({3, 3}, {1, 1, 1, 1, 0, 0, 0, 0, 1});
		REQUIRE(negative.overlapAt(minoS.position, minoS.collision) == false);
	}
	{
		const Offset offset = basicRotation<RotationSystem::Original>(minoS, 3);
		minoS += offset;
		const Grid negative({3, 3}, {1, 0, 1, 1, 0, 0, 1, 1, 0});
		REQUIRE(negative.overlapAt(minoS.position, minoS.collision) == false);
	}
}
