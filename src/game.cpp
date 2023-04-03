#include "game.hpp"

#include "app.hpp"
#include "grid.hpp"
#include "playfield.hpp"
#include "scenes.hpp"
#include "timer.hpp"
#include "types.hpp"

#include <raygui.h>
#include <raylib-cpp.hpp>
#include <rres-raylib.h>

#include <charconv>
#include <numeric>
#include <random>
#include <vector>

namespace
{
using namespace raymino;

constexpr int HIDDEN_HEIGHT = 4;
constexpr int FIELD_WIDTH = 10;
constexpr int FIELD_HEIGHT = 20;
constexpr size_t BASE_DELAY_IDX = 2;

void draw(const Grid& grid, int yOffset, XY at, int cellSize, bool background)
{
	const Size gridSize = grid.getSize();
	const int width = gridSize.width * cellSize;
	const int height = (gridSize.height - yOffset) * cellSize;
	const Color colors[]{background ? LIGHTGRAY : BLANK, DARKGRAY, GRAY};

	if(background)
	{
		::DrawRectangle(at.x, at.y, width, height, colors[2]);
	}

	for(int y = 0; y < gridSize.height; ++y)
	{
		for(int x = 0; x < gridSize.width; ++x)
		{
			const uint8_t colorId = grid.getAt({x, y + yOffset});
			::DrawRectangle((x * cellSize) + at.x, (y * cellSize) + at.y, cellSize - 1, cellSize - 1, colors[colorId]);
		}
	}
}

void draw(Range<Playfield::MinoConstIterator> range, XY at, int firstCellSize, int restCellSize)
{
	draw(*range.first, 0, at, firstCellSize, false);
	at.y += (range.first->getSize().height * firstCellSize) + restCellSize;
	++range.first;
	for(const Grid& mino : range)
	{
		draw(mino, 0, at, restCellSize, false);
		at.y += (mino.getSize().height * restCellSize) + restCellSize;
	}
}

unsigned unusedBottomRows(const Grid& mino)
{
	const auto rend = mino.rend();
	auto rit = mino.rbegin();
	const unsigned cols = mino.getSize().width;
	unsigned unused = 0;
	for(unsigned col = 0; rit != rend; ++rit)
	{
		if(*rit)
		{
			break;
		}
		++col;
		if(col == cols)
		{
			++unused;
			col = 0;
		}
	}
	return unused;
}

void shuffleBaseMinos(
    const std::vector<Grid>& baseMinos, std::back_insert_iterator<std::vector<Grid>> nextMinosInserter, size_t minCount)
{
	std::default_random_engine rng{std::random_device{}()};
	size_t inserted = 0;
	while(inserted < minCount)
	{
		std::vector<Grid> copy = baseMinos;
		std::shuffle(copy.begin(), copy.end(), rng);
		std::for_each(copy.begin(), copy.end(),
		    [&](Grid& mino)
		    {
			    nextMinosInserter = std::move(mino);
		    });
		inserted += baseMinos.size();
	}
}

XY getStartPosition(const Grid& mino, unsigned fieldWidth)
{
	return {static_cast<int>(fieldWidth / 2) - (static_cast<int>(mino.getSize().width + 1) / 2),
	    HIDDEN_HEIGHT - (static_cast<int>(mino.getSize().height) - static_cast<int>(unusedBottomRows(mino)))};
}

std::vector<Grid> makeBaseMinos()
{
	return {{{4, 4}, {0, 0, 0, 0, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0}}, {{3, 3}, {1, 0, 0, 1, 1, 1, 0, 0, 0}},
	    {{3, 3}, {0, 0, 1, 1, 1, 1, 0, 0, 0}}, {{2, 2}, {1, 1, 1, 1}}, {{3, 3}, {0, 1, 1, 1, 1, 0, 0, 0, 0}},
	    {{3, 3}, {0, 1, 0, 1, 1, 1, 0, 0, 0}}, {{3, 3}, {1, 1, 0, 0, 1, 1, 0, 0, 0}}};
}
} // namespace

namespace raymino
{

void Game::dropMino()
{
	if(!playfield.moveActiveMino({0, 1}, 0))
	{
		state = State::Set;
	}
}

void Game::moveMino(float delta)
{
	const KeyAction::Return action = move.tick(delta);
	switch(action.state)
	{
	case KeyAction::State::Pressed:
	case KeyAction::State::Repeated:
		playfield.moveActiveMino({action.value, 0}, 0);
		break;
	default:
		break;
	}
}

void Game::rotateMino(float delta)
{
	const KeyAction::Return action = rotate.tick(delta);
	switch(action.state)
	{
	case KeyAction::State::Pressed:
	case KeyAction::State::Repeated:
		playfield.moveActiveMino({0, 0}, action.value);
		break;
	default:
		break;
	}
}

void Game::setMino()
{
	if(playfield.lockActiveMino())
	{
		score += playfield.clearFullLines();
		state = State::Drop;
	}
	else
	{
		state = State::Over;
	}
}

void Game::update([[maybe_unused]] App& app)
{
	const float delta = ::GetFrameTime();
	switch(state)
	{
	case State::Drop:
	{
		const size_t scoredIdx = (score / 8) + BASE_DELAY_IDX;
		const size_t dropIdx = drop.tick(delta).value ? scoredIdx + 4 : scoredIdx;
		dropDelay.delay = delays[std::min(dropIdx, maxSpeedLevel)];
		moveMino(delta);
		rotateMino(delta);
		if(dropDelay.tick(delta))
		{
			dropMino();
		}
	}
	break;
	case State::Set:
		setMino();
		break;
	case State::Over:
		if(::IsKeyReleased(KEY_HOME))
		{
			state = State::Drop;
			playfield.resetField();
			score = 0;
		}
		break;
	}
}

void Game::draw()
{
	::BeginDrawing();

	::ClearBackground(::RColor::LightGray());

	::draw(playfield.getField(), HIDDEN_HEIGHT, {0, 0}, 30, true);

	const Playfield::ActiveMino& activeMino = playfield.getActiveMino();
	::draw(
	    activeMino.collision, 0, {activeMino.position.x * 30, (activeMino.position.y - HIDDEN_HEIGHT) * 30}, 30, false);

	::draw(playfield.getNextMinos(8), {330, 25}, 26, 16);

	::DrawText("SCORE:", 450, 30, 30, DARKGRAY);
	std::array<char, 16> buffer{};
	std::to_chars(buffer.data(), buffer.data() + (buffer.size() - 1), score);
	::DrawText(buffer.data(), 450, 60, 30, DARKGRAY);

	if(state == State::Over)
	{
		::DrawText("GAME OVER", 150, 250, 45, RED);
	}

	::EndDrawing();
}

void Game::UpdateDraw(App& app)
{
	update(app);
	draw();
}

Game::Game(Playfield playfield, State state, Timer dropDelay, KeyAction drop, KeyAction move, KeyAction rotate, size_t score) :
    playfield{std::move(playfield)}, state{state}, drop{drop}, move{move}, rotate{rotate}, score{score}
{
}

template<>
std::unique_ptr<IScene> MakeScene<Scene::Game>()
{
	return std::make_unique<Game>(Playfield{Size{FIELD_WIDTH, FIELD_HEIGHT + HIDDEN_HEIGHT}, makeBaseMinos(),
	                                  std::function<Playfield::ShuffleBaseMinosFunc>{shuffleBaseMinos},
	                                  std::function<Playfield::StartingPositionFunc>{getStartPosition}},
	    State::Drop, Timer{}, KeyAction{0, KEY_DOWN}, KeyAction{delays[BASE_DELAY_IDX], KEY_RIGHT, KEY_LEFT},
	    KeyAction{delays[BASE_DELAY_IDX], KEY_D, KEY_A}, 0);
}
} // namespace raymino
