#pragma once

#include "grid.hpp"
#include "types.hpp"

#include <cstddef>
#include <functional>
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
	using ShuffleBaseMinosFunc = std::vector<Grid>(const std::vector<Grid>& baseMinos);
	using StartingPositionFunc = XY(const Grid& nextMino, unsigned fieldWidth);

	Playfield() = delete;
	Playfield(std::vector<Grid> baseMinos, std::function<ShuffleBaseMinosFunc> shuffledBaseMinos,
	    std::function<StartingPositionFunc> getStartPosition);

	size_t getHiddenHeight() const;
	Range<MinoConstIterator> getNextMinos(size_t count) const;
	Range<FieldConstIterator> getField(bool includeHidden) const;
	const ActiveMino& getActiveMino() const;

	bool moveActiveMino(XY translation, int rotation);
	bool lockActiveMino();

private:
	std::vector<Grid> baseMinos;
	Grid field;
	std::vector<Grid> nextMinos;
	ActiveMino activeMino;
	std::function<ShuffleBaseMinosFunc> shuffledBaseMinos;
	std::function<StartingPositionFunc> getStartPosition;
};
} // namespace raymino
