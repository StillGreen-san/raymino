#pragma once

#include "grid.hpp"
#include "types.hpp"

#include <cstddef>
#include <functional>
#include <iterator>
#include <vector>

namespace raymino
{
class Playfield
{
public:
	struct ActiveMino
	{
		Grid collision;
		XY position;
	};

	static constexpr uint8_t emptyCell = 0;

	using FieldConstIterator = std::vector<uint8_t>::const_iterator;
	using MinoConstIterator = std::vector<Grid>::const_iterator;
	using ShuffleBaseMinosFunc = void(const std::vector<Grid>& baseMinos,
	    std::back_insert_iterator<std::vector<Grid>> nextMinosInserter, size_t minCount);
	using StartingPositionFunc = XY(const Grid& nextMino, unsigned fieldWidth);

	Playfield() = delete;
	Playfield(Size size, std::vector<Grid> baseMinos, std::function<ShuffleBaseMinosFunc> shuffleBaseMinos,
	    std::function<StartingPositionFunc> getStartPosition);

	Range<MinoConstIterator> getNextMinos(size_t count) const;
	Range<FieldConstIterator> getField() const;
	const ActiveMino& getActiveMino() const;

	void resetField();

	/**
	 * @return true if moved successfully
	 */
	bool moveActiveMino(XY translation, int rotation);
	/**
	 * @return false if next Mino overlaps at spawn
	 */
	bool lockActiveMino();

private:
	std::vector<Grid> baseMinos;
	Grid field;
	mutable std::vector<Grid> nextMinos;
	ActiveMino activeMino;
	std::function<ShuffleBaseMinosFunc> shuffleBaseMinos;
	std::function<StartingPositionFunc> getStartPosition;
};
} // namespace raymino
