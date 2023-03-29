#include "playfield.hpp"

#include <algorithm>
#include <stdexcept>

namespace raymino
{
bool largerHeight(const Grid& lhs, const Grid& rhs)
{
	return lhs.getSize().height < rhs.getSize().height;
}
std::vector<Grid>&& processBaseMinos(std::vector<Grid>& baseMinos)
{
	if(baseMinos.empty())
	{
		throw std::invalid_argument("no base minos");
	}
	return std::move(baseMinos);
}
std::vector<Grid> initNextMinos(
    const std::vector<Grid>& baseMinos, std::function<Playfield::ShuffleBaseMinosFunc>& shuffleBaseMinos)
{
	std::vector<Grid> nextMinos;
	nextMinos.reserve(baseMinos.size());
	shuffleBaseMinos(baseMinos, std::back_inserter(nextMinos), baseMinos.size());
	return nextMinos;
}
Playfield::ActiveMino takeNextMino(
    Grid& field, std::vector<Grid>& nextMinos, std::function<Playfield::StartingPositionFunc>& getStartPosition)
{
	Grid& next = nextMinos.front();
	const XY startPos = getStartPosition(next, field.getSize().width);
	Playfield::ActiveMino activeMino = {std::move(next), startPos};
	nextMinos.erase(begin(nextMinos));
	return activeMino;
}

Playfield::Playfield(Size size, std::vector<Grid> baseMinos, std::function<ShuffleBaseMinosFunc> shuffleBaseMinos,
    std::function<StartingPositionFunc> getStartPosition) :
    baseMinos{processBaseMinos(baseMinos)},
    field(size, emptyCell), nextMinos{initNextMinos(this->baseMinos, shuffleBaseMinos)},
    activeMino{takeNextMino(this->field, this->nextMinos, getStartPosition)},
    shuffleBaseMinos{std::move(shuffleBaseMinos)}, getStartPosition{std::move(getStartPosition)}
{
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

Range<Playfield::FieldConstIterator> Playfield::getField() const
{
	return {std::begin(field), std::end(field)};
}

const Playfield::ActiveMino& Playfield::getActiveMino() const
{
	return activeMino;
}

void Playfield::resetField()
{
	field = Grid(field.getSize(), 0);
}

bool Playfield::moveActiveMino(XY translation, int rotation)
{
	ActiveMino next = activeMino;
	next.collision.rotate(rotation);
	next.position += translation;
	if(field.overlapAt(next.position, next.collision))
	{
		return false;
	}
	activeMino = next;
	return true;
}

bool Playfield::lockActiveMino()
{
	field.setAt(activeMino.position, activeMino.collision);
	if(nextMinos.empty())
	{
		shuffleBaseMinos(baseMinos, std::back_inserter(nextMinos), baseMinos.size());
	}
	activeMino = takeNextMino(field, nextMinos, getStartPosition);
	return !field.overlapAt(activeMino.position, activeMino.collision);
}
} // namespace raymino
