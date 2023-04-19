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
	constexpr bool operator==(Offset other) const noexcept
	{
		return std::tie(position, rotation) == std::tie(other.position, other.rotation);
	}
};
struct Tetromino : public Offset
{
	TetrominoType type;
	Grid collision;
	Tetromino& operator+=(Offset other) noexcept
	{
		Offset::operator+=(other);
		collision.rotate(other.rotation);
		return *this;
	}
};

/**
 * @tparam TSys RotationSystem that the Tetrominos will be used with
 * @return Tetrominos in I, J, L, O, S, T, Z order
 */
template<RotationSystem TSys>
std::vector<Tetromino> makeBaseMinos();

/**
 * @tparam TSys RotationSystem to use for rotation
 * @param mino that needs to be rotated
 * @param rotation direction
 * @return Offset that can be applied to Tetromino
 */
template<RotationSystem TSys>
Offset basicRotation(const Tetromino& mino, int rotation);
} // namespace raymino
