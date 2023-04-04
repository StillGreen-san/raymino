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

bool constexpr operator<(Color lhs, Color rhs) noexcept
{
	return std::tie(lhs.r, lhs.g, lhs.b, lhs.a) < std::tie(rhs.r, rhs.g, rhs.b, rhs.a);
}

#include <algorithm>
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

template<size_t TColorNum>
struct Colors
{
	Colors() = delete;
	explicit Colors(std::array<Color, TColorNum> colors) : colors{colors}
	{
		std::sort(this->colors.begin(), this->colors.end());
	}
	uint8_t operator[](Color color)
	{
		const auto colorIt = std::lower_bound(colors.begin(), colors.end(), color);
		return std::distance(begin(colors), colorIt);
	}
	Color& operator[](uint8_t idx)
	{
		return colors[idx];
	}
	std::array<Color, TColorNum> colors;
};
Colors colors{
    std::array{LIGHTGRAY, GRAY, DARKGRAY, YELLOW, GOLD, ORANGE, PINK, RED, MAROON, GREEN, LIME, DARKGREEN, SKYBLUE,
        BLUE, DARKBLUE, PURPLE, VIOLET, DARKPURPLE, BEIGE, BROWN, DARKBROWN, WHITE, BLACK, BLANK, MAGENTA, RAYWHITE}};

void draw(const Grid& grid, int yOffset, XY at, int cellSize, bool background)
{
	const Size gridSize = grid.getSize();
	const int width = gridSize.width * cellSize;
	const int height = (gridSize.height - yOffset) * cellSize;
	colors[0] = background ? LIGHTGRAY : BLANK;

	if(background)
	{
		::DrawRectangle(at.x, at.y, width, height, colors[colors[GRAY]]);
	}

	for(int y = 0; y < gridSize.height - yOffset; ++y)
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

Grid colorize(Grid grid, uint8_t color)
{
	grid.transformCells(
	    [color](uint8_t current)
	    {
		    return current ? color : current;
	    });
	return grid;
}

std::vector<Grid> makeBaseMinos()
{
	return {
	    colorize({{4, 4}, {0, 0, 0, 0, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0}}, colors[SKYBLUE]),
	    colorize({{3, 3}, {1, 0, 0, 1, 1, 1, 0, 0, 0}}, colors[BLUE]),
	    colorize({{3, 3}, {0, 0, 1, 1, 1, 1, 0, 0, 0}}, colors[ORANGE]),
	    colorize({{2, 2}, {1, 1, 1, 1}}, colors[YELLOW]),
	    colorize({{3, 3}, {0, 1, 1, 1, 1, 0, 0, 0, 0}}, colors[GREEN]),
	    colorize({{3, 3}, {0, 1, 0, 1, 1, 1, 0, 0, 0}}, colors[PINK]),
	    colorize({{3, 3}, {1, 1, 0, 0, 1, 1, 0, 0, 0}}, colors[RED]),
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
		if(dropState.value || lockDelay.tick(delta))
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
		const size_t dropIdx = dropState.value ? scoredIdx + 4 : scoredIdx;
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

Game::Game(Playfield playfield, State state, Timer dropDelay, Timer lockDelay, KeyAction drop, KeyAction move,
    KeyAction rotate, size_t score) :
    playfield{std::move(playfield)},
    state{state}, dropDelay{dropDelay}, lockDelay{lockDelay}, drop{drop}, move{move}, rotate{rotate}, score{score}
{
}

template<>
std::unique_ptr<IScene> MakeScene<Scene::Game>()
{
	return std::make_unique<Game>(Playfield{Size{FIELD_WIDTH, FIELD_HEIGHT + HIDDEN_HEIGHT}, makeBaseMinos(),
	                                  std::function<Playfield::ShuffleBaseMinosFunc>{shuffleBaseMinos},
	                                  std::function<Playfield::StartingPositionFunc>{getStartPosition}},
	    Game::State::Drop, Timer{}, Timer{1}, KeyAction{0, KEY_DOWN},
	    KeyAction{delays[BASE_DELAY_IDX], KEY_RIGHT, KEY_LEFT}, KeyAction{delays[BASE_DELAY_IDX], KEY_D, KEY_A}, 0);
}
} // namespace raymino
