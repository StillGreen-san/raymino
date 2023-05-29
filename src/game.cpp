#include "game.hpp"

#include "app.hpp"
#include "graphics.hpp"
#include "grid.hpp"
#include "playfield.hpp"
#include "scenes.hpp"
#include "timer.hpp"
#include "types.hpp"

#include <raygui.h>
#include <raylib-cpp.hpp>
#include <rres-raylib.h>

bool constexpr operator<(Color lhs, Color rhs) noexcept
{
	return std::tie(lhs.r, lhs.g, lhs.b, lhs.a) < std::tie(rhs.r, rhs.g, rhs.b, rhs.a);
}

#include <algorithm>
#include <charconv>
#include <random>
#include <vector>

namespace
{
using namespace raymino;

constexpr int HIDDEN_HEIGHT = 4;
constexpr int FIELD_WIDTH = 10;
constexpr int FIELD_HEIGHT = 20;
constexpr size_t BASE_DELAY_IDX = 2;

ColorMap minoColors{
    {LIGHTGRAY, GRAY, DARKGRAY, YELLOW, GOLD, ORANGE, PINK, RED, MAROON, GREEN, LIME, DARKGREEN, SKYBLUE, BLUE,
        DARKBLUE, PURPLE, VIOLET, DARKPURPLE, BEIGE, BROWN, DARKBROWN, WHITE, BLACK, BLANK, MAGENTA, RAYWHITE}};

void drawPreview(Range<Playfield::MinoConstIterator> range, XY at, int firstCellSize, int restCellSize)
{
	drawCells(*range.first, at, firstCellSize, 1, minoColors);
	at.y += (range.first->getSize().height * firstCellSize) + restCellSize;
	++range.first;
	for(const Grid& mino : range)
	{
		drawCells(mino, at, restCellSize, 1, minoColors);
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
		if(*rit != 0)
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

Grid colorize(Grid grid, Grid::Cell color)
{
	grid.transformCells(
	    [color](Grid::Cell current)
	    {
		    return current != 0 ? color : current;
	    });
	return grid;
}

std::vector<Grid> makeBaseMinos()
{
	return {
	    colorize({{4, 4}, {0, 0, 0, 0, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0}}, minoColors[SKYBLUE]),
	    colorize({{3, 3}, {1, 0, 0, 1, 1, 1, 0, 0, 0}}, minoColors[BLUE]),
	    colorize({{3, 3}, {0, 0, 1, 1, 1, 1, 0, 0, 0}}, minoColors[ORANGE]),
	    colorize({{2, 2}, {1, 1, 1, 1}}, minoColors[YELLOW]),
	    colorize({{3, 3}, {0, 1, 1, 1, 1, 0, 0, 0, 0}}, minoColors[GREEN]),
	    colorize({{3, 3}, {0, 1, 0, 1, 1, 1, 0, 0, 0}}, minoColors[PINK]),
	    colorize({{3, 3}, {1, 1, 0, 0, 1, 1, 0, 0, 0}}, minoColors[RED]),
	};
}

void kickRotate(Playfield& playfield, int8_t rotation)
{
	static constexpr std::array<XY, 25> leftRotations{{{0, 0}, {0, 1}, {0, 2}, {1, 0}, {1, 1}, {1, 2}, {-1, 0}, {-1, 1},
	    {-1, 2}, {2, 0}, {-2, 0}, {2, 1}, {-2, 1}, {2, 2}, {-2, 2}, {0, -1}, {1, -1}, {-1, -1}, {2, -1}, {-2, -1},
	    {0, -2}, {1, -2}, {-1, -2}, {2, -2}, {-2, -2}}};
	static constexpr std::array<XY, 25> rightRotations{{{0, 0}, {0, 1}, {0, 2}, {-1, 0}, {-1, 1}, {-1, 2}, {1, 0},
	    {1, 1}, {1, 2}, {-2, 0}, {2, 0}, {-2, 1}, {2, 1}, {-2, 2}, {2, 2}, {0, -1}, {-1, -1}, {1, -1}, {-2, -1},
	    {2, -1}, {0, -2}, {-1, -2}, {1, -2}, {-2, -2}, {2, -2}}};
	const auto& kicks = rotation < 0 ? leftRotations : rightRotations;
	for(XY const kick : kicks)
	{
		if(playfield.moveActiveMino(kick, rotation))
		{
			playfield.moveActiveMino({0, 1}, 0);
			break;
		}
	}
}
} // namespace

namespace raymino
{

void Game::dropMino()
{
	if(!playfield.moveActiveMino({0, 1}, 0))
	{
		if(state != State::Lock)
		{
			lockDelay.elapsed = 0;
		}
		state = State::Lock;
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
	if(action.value == 0)
	{
		return;
	}
	switch(action.state)
	{
	case KeyAction::State::Pressed:
	case KeyAction::State::Repeated:
	{
		kickRotate(playfield, action.value);
	}
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
	const KeyAction::Return dropState = drop.tick(delta);
	switch(state)
	{
	case State::Lock:
	{
		if(dropState.value != 0 || lockDelay.tick(delta))
		{
			state = State::Drop;
			dropMino();
			if(state == State::Lock)
			{
				state = State::Set;
			}
			break;
		}
	}
	case State::Drop:
	{
		const size_t scoredIdx = (score / 8) + BASE_DELAY_IDX;
		const size_t dropIdx = dropState.value != 0 ? scoredIdx + 4 : scoredIdx;
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
		if(::IsKeyReleased(KEY_END))
		{
			app.QueueSceneSwitch(MakeScene<Scene::Menu>(app));
		}
		break;
	}
}

void Game::draw()
{
	::BeginDrawing();

	::ClearBackground(LIGHTGRAY);

	drawBackground(playfield.getField(), {0, 0 - (HIDDEN_HEIGHT * 30)}, 29, 1, LIGHTGRAY, GRAY);
	drawCells(playfield.getField(), {0, 0 - (HIDDEN_HEIGHT * 30)}, 29, 1, minoColors);

	const Playfield::ActiveMino& activeMino = playfield.getActiveMino();
	drawCells(activeMino.collision, {activeMino.position.x * 30, ((activeMino.position.y - HIDDEN_HEIGHT) * 30)}, 29, 1,
	    minoColors);

	::drawPreview(playfield.getNextMinos(8), {330, 25}, 26, 16);

	::DrawText("SCORE:", 450, 30, 30, DARKGRAY);
	std::array<char, 16> buffer{};
	std::to_chars(buffer.data(), buffer.data() + (buffer.size() - 1), score);
	::DrawText(buffer.data(), 450, 60, 30, DARKGRAY);

	if(state == State::Over)
	{
		::DrawRectangle(145, 250, 280, 42, {77, 77, 77, 222});
		::DrawText("GAME OVER", 150, 250, 45, RED);
	}

	::EndDrawing();
}

void Game::UpdateDraw(App& app)
{
	update(app);
	draw();
}

Game::Game(Playfield playfield, State state, Timer dropDelay, Timer lockDelay, KeyAction drop, KeyAction move,
    KeyAction rotate, size_t score) :
    playfield{std::move(playfield)},
    state{state}, dropDelay{dropDelay}, lockDelay{lockDelay}, drop{drop}, move{move}, rotate{rotate}, score{score}
{
}

template<>
std::unique_ptr<IScene> MakeScene<Scene::Game>([[maybe_unused]] class App& app)
{
	return std::make_unique<Game>(Playfield{Size{FIELD_WIDTH, FIELD_HEIGHT + HIDDEN_HEIGHT}, makeBaseMinos(),
	                                  std::function<Playfield::ShuffleBaseMinosFunc>{shuffleBaseMinos},
	                                  std::function<Playfield::StartingPositionFunc>{getStartPosition}},
	    Game::State::Drop, Timer{}, Timer{1}, KeyAction{0, KEY_DOWN},
	    KeyAction{delays[BASE_DELAY_IDX + 2], KEY_RIGHT, KEY_LEFT}, KeyAction{delays[BASE_DELAY_IDX], KEY_D, KEY_A}, 0);
}
} // namespace raymino
