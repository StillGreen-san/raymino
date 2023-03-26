#include "playfield.hpp"

namespace raymino
{
size_t Playfield::getHiddenHeight() const
{
	return baseMinos.front().getSize().height;
}

Range<Playfield::MinoConstIterator> Playfield::getNextMinos(size_t count) const
{
	if(nextMinos.size() < count)
	{
		nextMinos.reserve(count);
		shuffleBaseMinos(baseMinos, std::back_inserter(nextMinos), nextMinos.size() - count);
	}
	return {std::begin(nextMinos), std::next(std::begin(nextMinos), count)};
}

Range<Playfield::FieldConstIterator> Playfield::getField(bool includeHidden) const
{
	if(includeHidden)
	{
		return {std::begin(field), std::end(field)};
	}
	return {std::next(std::begin(field), getHiddenHeight()), std::end(field)};
}

const Playfield::ActiveMino& Playfield::getActiveMino() const
{
	return activeMino;
}
} // namespace raymino
