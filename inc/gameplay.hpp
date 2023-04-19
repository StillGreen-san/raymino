#pragma once

#include "grid.hpp"
#include "types.hpp"

#include <vector>

namespace raymino
{
struct Offset
{
	XY position;
	int rotation;
	Offset operator+(Offset other) const noexcept
	{
		return {position + other.position, rotation + other.rotation};
	}
	Offset& operator+=(Offset other) noexcept
	{
		position += other.position;
		rotation += other.rotation;
		return *this;
	}
};
struct Tetromino : public Offset
{
	TetrominoType type;
	Grid collision;
};

/**
 * @tparam TSys RotationSystem that the Tetrominos will be used with
 * @return Tetrominos in I, J, L, O, S, T, Z order
 */
template<RotationSystem TSys>
std::vector<Tetromino> makeBaseMinos();
} // namespace raymino
