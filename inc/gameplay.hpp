#pragma once

#include "grid.hpp"
#include "types.hpp"

#include <vector>

namespace raymino
{
struct Tetromino
{
	TetrominoType type;
	int rotation;
	XY position;
	Grid collision;
};

/**
 * @tparam TSys RotationSystem that the Tetrominos will be used with
 * @return Tetrominos in I, J, L, O, S, T, Z order
 */
template<RotationSystem TSys>
std::vector<Tetromino> makeBaseMinos();
} // namespace raymino
