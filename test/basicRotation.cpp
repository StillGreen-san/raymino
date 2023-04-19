#include <catch2/catch_test_macros.hpp>

#include "gameplay.hpp"

using namespace raymino;

TEST_CASE("basicRotation<Super>", "[basicRotation]")
{
	std::vector<Tetromino> tetrominos = makeBaseMinos<RotationSystem::Super>();

	{
		const Offset offset = basicRotation<RotationSystem::Super>(tetrominos[0], 2);
		REQUIRE(offset == Offset{{0, 0}, 2});
		tetrominos[0] += offset;
		const Grid negative({4, 4}, {1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 1, 1, 1, 1});
		REQUIRE(negative.overlapAt(tetrominos[0].position, tetrominos[0].collision) == false);
	}
	{
		const Offset offset = basicRotation<RotationSystem::Super>(tetrominos[0], -1);
		REQUIRE(offset == Offset{{0, 0}, -1});
		tetrominos[0] += offset;
		const Grid negative({4, 4}, {1, 1, 0, 1, 1, 1, 0, 1, 1, 1, 0, 1, 1, 1, 0, 1});
		REQUIRE(negative.overlapAt(tetrominos[0].position, tetrominos[0].collision) == false);
	}

	{
		const Offset offset = basicRotation<RotationSystem::Super>(tetrominos[1], 3);
		REQUIRE(offset == Offset{{0, 0}, 3});
		tetrominos[1] += offset;
		const Grid negative({3, 3}, {1, 0, 1, 1, 0, 1, 0, 0, 1});
		REQUIRE(negative.overlapAt(tetrominos[1].position, tetrominos[1].collision) == false);
	}
	{
		const Offset offset = basicRotation<RotationSystem::Super>(tetrominos[1], 1);
		REQUIRE(offset == Offset{{0, 0}, 1});
		tetrominos[1] += offset;
		const Grid negative({3, 3}, {0, 1, 1, 0, 0, 0, 1, 1, 1});
		REQUIRE(negative.overlapAt(tetrominos[1].position, tetrominos[1].collision) == false);
	}

	{
		const Offset offset = basicRotation<RotationSystem::Super>(tetrominos[4], -1);
		REQUIRE(offset == Offset{{0, 0}, -1});
		tetrominos[4] += offset;
		const Grid negative({3, 3}, {0, 1, 1, 0, 0, 1, 1, 0, 1});
		REQUIRE(negative.overlapAt(tetrominos[4].position, tetrominos[4].collision) == false);
	}
	{
		const Offset offset = basicRotation<RotationSystem::Super>(tetrominos[4], -3);
		REQUIRE(offset == Offset{{0, 0}, -3});
		tetrominos[4] += offset;
		const Grid negative({3, 3}, {1, 0, 0, 0, 0, 1, 1, 1, 1});
		REQUIRE(negative.overlapAt(tetrominos[4].position, tetrominos[4].collision) == false);
	}
}
