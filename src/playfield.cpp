#include "playfield.hpp"

#include <algorithm>
#include <stdexcept>

namespace raymino
{
bool notZero(uint8_t thing)
{
	return thing != 0;
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
    const std::vector<Grid>& baseMinos, const std::function<Playfield::ShuffleBaseMinosFunc>& shuffleBaseMinos)
{
	std::vector<Grid> nextMinos;
	nextMinos.reserve(baseMinos.size());
	shuffleBaseMinos(baseMinos, std::back_inserter(nextMinos), baseMinos.size());
	return nextMinos;
}
Playfield::ActiveMino takeNextMino(
    Grid& field, std::vector<Grid>& nextMinos, const std::function<Playfield::StartingPositionFunc>& getStartPosition)
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
		shuffleBaseMinos(baseMinos, std::back_inserter(nextMinos), count - nextMinos.size());
	}
	return {std::begin(nextMinos), std::next(std::begin(nextMinos), static_cast<ptrdiff_t>(count))};
}

const Grid& Playfield::getField() const
{
	return field;
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
	if(field.overlapAt(next.position, next.collision) != 0)
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
	return field.overlapAt(activeMino.position, activeMino.collision) == 0;
}

unsigned Playfield::clearFullLines()
{
	const unsigned width = field.getSize().width;
	auto lineCheckBegin = std::as_const(field).rbegin();
	auto lineCheckEnd = std::next(lineCheckBegin, width);
	auto lineWriteBegin = field.rbegin();
	auto lineWriteEnd = std::next(lineWriteBegin, width);
	const auto fieldEnd = std::as_const(field).rend();
	unsigned linesCleared = 0;

	while(lineCheckEnd != fieldEnd)
	{
		if(std::all_of(lineCheckBegin, lineCheckEnd, notZero))
		{
			++linesCleared;
		}
		else
		{
			std::copy(lineCheckBegin, lineCheckEnd, lineWriteBegin);
			std::advance(lineWriteBegin, width);
			std::advance(lineWriteEnd, width);
		}
		std::advance(lineCheckBegin, width);
		std::advance(lineCheckEnd, width);
	}

	return linesCleared;
}
} // namespace raymino
