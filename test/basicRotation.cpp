#include <catch2/catch_test_macros.hpp>

#include "gameplay.hpp"

using namespace raymino;

TEST_CASE("basicRotation<Super>", "[basicRotation]")
{
	std::vector<Tetromino> tetrominos = makeBaseMinos<RotationSystem::Super>();
	Tetromino& minoI = tetrominos[0];
	Tetromino& minoJ = tetrominos[1];
	Tetromino& minoS = tetrominos[4];

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
