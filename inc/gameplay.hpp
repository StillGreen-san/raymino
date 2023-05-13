#pragma once

#include "grid.hpp"
#include "types.hpp"

#include <algorithm>
#include <memory>
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
 * @return iterator to Tetromino of type or end
 */
template<typename TContainer>
inline auto find(TContainer& minos, TetrominoType type)
{
	return std::find_if(minos.begin(), minos.end(),
	    [=](const Tetromino& mino)
	    {
		    return mino.type == type;
	    });
}

/**
 * @tparam TSys RotationSystem to use for rotation
 * @param mino that needs to be rotated
 * @param rotation direction
 * @return Offset that can be applied to Tetromino
 */
template<RotationSystem TSys>
Offset basicRotation(const Tetromino& mino, int rotation);

/**
 * @tparam TSys WallKicks to use for kicks
 * @param field playfield to test against
 * @param tetromino to kick
 * @param offset of basic rotation
 * @return Offset that can be applied to Tetromino
 * @pre rotation in offset == -1 || 1
 * @warning assumes basic rotation failed
 */
template<WallKicks TSys>
Offset wallKick(const Grid& field, const Tetromino& tetromino, Offset offset);

/**
 * @param grid to query
 * @return Rect with offset inside grid & true size
 */
Rect findTrueSize(const Grid& grid);

/**
 * @brief centered absolut position to place Tetromino (rounded left)
 * @param tetromino to position
 * @param highestUsedRow for any Mino in Tetromino
 * @param totalWidth of playfield
 * @return XY spawn position
 */
XY spawnPosition(const Tetromino& tetromino, int highestUsedRow, int totalWidth);

/**
 * @param grid to modify
 * @return size_t number of lines erased
 */
size_t eraseFullLines(Grid& grid);

/**
 * @brief amount of occupied front&back corners of a T in a playfield
 */
struct TSpinCornerCountResult
{
	int front;
	int back;
	constexpr bool operator==(TSpinCornerCountResult other) const noexcept
	{
		return std::tie(front, back) == std::tie(other.front, other.back);
	}
};

/**
 * @param field playfield to test against
 * @param tetromino to test against
 * @return TSpinCornerCountResult
 */
TSpinCornerCountResult tSpinCornerCount(const Grid& field, const Tetromino& tetromino);

/**
 * @brief potentially stateful ScoringSystem
 */
struct IScoringSystem
{
	virtual ~IScoringSystem() = default;

	/**
	 * @param event ScoreEvent to process
	 * @param lines cleared/dropped
	 * @param level
	 * @return ptrdiff_t score for event
	 */
	virtual ptrdiff_t process(ScoreEvent event, int lines, int level) = 0;
};

/**
 * @tparam TSys ScoringSystem to use
 * @return std::unique_ptr<IScoringSystem> of TSys ScoringSystem
 */
template<ScoringSystem TSys>
std::unique_ptr<IScoringSystem> makeScoringSystem();
} // namespace raymino
