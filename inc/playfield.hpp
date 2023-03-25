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
	Playfield() = delete;
	Playfield(std::vector<Grid> baseMinos);

	struct ActiveMino
	{
		Grid collision;
		XY position;
	};

	static constexpr uint8_t emptyCell = 0;

	using FieldConstIterator = std::vector<uint8_t>::const_iterator;
	using MinoConstIterator = std::vector<Grid>::const_iterator;

	size_t getHiddenHeight() const;
	Range<MinoConstIterator> getNextMinos(size_t count) const;
	Range<FieldConstIterator> getField(bool includeHidden) const;
	const ActiveMino& getActiveMino() const;

	bool moveActiveMino(XY direction);
	bool rotateActiveMino(int direction);
	void lockActiveMino();

private:
	std::vector<Grid> baseMinos;
	Grid field;
	std::vector<Grid> nextMinos;
	ActiveMino activeMino;
};
} // namespace raymino
