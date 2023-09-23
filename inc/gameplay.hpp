#pragma once

#include "grid.hpp"
#include "types.hpp"

#include <algorithm>
#include <deque>
#include <memory>
#include <random>
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
	Offset operator-(Offset other) const noexcept
	{
		return {position - other.position, rotation - other.rotation};
	}
	Offset& operator+=(Offset other) noexcept
	{
		position += other.position;
		rotation += other.rotation;
		return *this;
	}
	Offset& operator-=(Offset other) noexcept
	{
		position -= other.position;
		rotation -= other.rotation;
		return *this;
	}
	constexpr bool operator==(Offset other) const noexcept
	{
		return std::tie(position, rotation) == std::tie(other.position, other.rotation);
	}
	constexpr bool operator!=(Offset other) const noexcept
	{
		return !operator==(other);
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
	Tetromino& operator-=(Offset other) noexcept
	{
		Offset::operator-=(other);
		collision.rotate(-other.rotation);
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
 * @param tsys RotationSystem
 * @return makeBaseMinos function pointer
 */
std::vector<Tetromino> (*makeBaseMinos(RotationSystem tsys) noexcept)();

/**
 * @return iterator to Tetromino of type or end
 */
template<typename TContainer>
inline auto find(TContainer& minos, TetrominoType type) noexcept
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
Offset basicRotation(const Tetromino& mino, int rotation) noexcept;

/**
 * @param tsys RotationSystem
 * @return basicRotation function pointer
 */
Offset (*basicRotation(RotationSystem tsys) noexcept)(const Tetromino& mino, int rotation) noexcept;

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
Offset wallKick(const Grid& field, const Tetromino& tetromino, Offset offset) noexcept;

/**
 * @param tsys WallKicks
 * @return wallKick function pointer
 */
Offset (*wallKick(WallKicks tsys) noexcept)(const Grid& field, const Tetromino& tetromino, Offset offset) noexcept;

/**
 * @param grid to query
 * @return Rect with offset inside grid & true size
 */
Rect findTrueSize(const Grid& grid) noexcept;

/**
 * @brief centered absolut position to place Tetromino (rounded left)
 * @param tetromino to position
 * @param highestUsedRow for any Mino in Tetromino
 * @param totalWidth of playfield
 * @return XY spawn position
 */
XY spawnPosition(const Tetromino& tetromino, int highestUsedRow, int totalWidth) noexcept;

/**
 * @param grid to modify
 * @return size_t number of lines erased
 */
size_t eraseFullLines(Grid& grid) noexcept;

/**
 * @param grid to check
 * @param tetromino to include when counting
 * @return size_t number of full lines
 */
size_t countFullLines(const Grid& grid, const Tetromino& tetromino) noexcept;

/**
 * @return true if all cells 0
 */
bool isEmpty(const Grid& grid) noexcept;

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
TSpinCornerCountResult tSpinCornerCount(const Grid& field, const Tetromino& tetromino) noexcept;

/**
 * @brief checks if tetromino cannot move in any direction
 * @param field playfield to test against
 * @param tetromino to test against
 * @return true if immobile
 */
bool isImmobile(const Grid& field, const Tetromino& tetromino) noexcept;

/**
 * @tparam TTSpin TSpin type
 * @param field playfield to test against
 * @param tetromino to test
 * @param lastMovement of tetromino
 * @return ScoreEvent LineClear, TSpinMini or TSpin
 */
template<TSpin TTSpin>
ScoreEvent tSpinCheck(const Grid& field, const Tetromino& tetromino, Offset lastMovement) noexcept;

/**
 * @param tspin TSpin
 * @return tSpinCheck function pointer
 */
ScoreEvent (*tSpinCheck(TSpin tspin) noexcept)(const Grid& field, const Tetromino& tetromino, Offset offset) noexcept;

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
	 * @return int64_t score for event
	 */
	[[nodiscard]] virtual int64_t process(ScoreEvent event, size_t lines, size_t level) noexcept = 0;
};

/**
 * @tparam TSys ScoringSystem to use
 * @return std::unique_ptr<IScoringSystem> of TSys ScoringSystem
 */
template<ScoringSystem TSys>
std::unique_ptr<IScoringSystem> makeScoringSystem();

/**
 * @param tsys ScoringSystem
 * @return makeScoringSystem function pointer
 */
std::unique_ptr<IScoringSystem> (*makeScoringSystem(ScoringSystem tsys) noexcept)();

/**
 * @brief potentially stateful shuffledIndices
 */
struct IShuffledIndices
{
	virtual ~IShuffledIndices() = default;

	/**
	 * @brief add random indices to indices
	 * @param indices index list
	 * @param minIndices in indices
	 * @param rng random engine
	 */
	virtual void fill(std::deque<size_t>& indices, size_t minIndices, std::mt19937_64& rng) = 0;
};

/**
 * @tparam TType ShuffleType
 * @param baseMinos to shuffle from
 * @return std::unique_ptr<IShuffledIndices> of TType ShuffleType
 */
template<ShuffleType TType>
std::unique_ptr<IShuffledIndices> makeShuffledIndices(const std::vector<Tetromino>& baseMinos);

/**
 * @param ttype ShuffleType
 * @return makeShuffledIndices function pointer
 */
std::unique_ptr<IShuffledIndices> (*makeShuffledIndices(ShuffleType ttype) noexcept)(
    const std::vector<Tetromino>& baseMinos);

struct LevelState
{
	static LevelState make(LevelGoal ttype) noexcept;
	size_t currentLevel;
	size_t linesCleared;
	size_t linesToClear;
	bool operator==(const LevelState& rhs) const noexcept
	{
		return std::tie(currentLevel, linesCleared, linesToClear) ==
		       std::tie(rhs.currentLevel, rhs.linesCleared, rhs.linesToClear);
	}
};

/**
 * @tparam TType LevelGoal
 * @param event ScoreEvent
 * @param lines cleared
 * @param state current state
 * @return LevelState
 */
template<LevelGoal TType>
LevelState levelUp(ScoreEvent event, size_t lines, LevelState state) noexcept;

/**
 * @param ttype LevelGoal
 * @return levelUp function pointer
 */
LevelState (*levelUp(LevelGoal ttype) noexcept)(ScoreEvent event, size_t lines, LevelState state) noexcept;
} // namespace raymino
