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
void draw(const raymino::Grid& grid, int yOffset, raymino::XY at, int cellSize, bool background)
{
	const raymino::Size gridSize = grid.getSize();
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

void draw(
    raymino::Range<raymino::Playfield::MinoConstIterator> range, raymino::XY at, int firstCellSize, int restCellSize)
{
	draw(*range.first, 0, at, firstCellSize, false);
	at.y += (range.first->getSize().height * firstCellSize) + restCellSize;
	++range.first;
	for(const raymino::Grid& mino : range)
	{
		draw(mino, 0, at, restCellSize, false);
		at.y += (mino.getSize().height * restCellSize) + restCellSize;
	}
}
} // namespace

namespace raymino
{
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

void Game::dropMino()
{
	if(!playfield.moveActiveMino({0, 1}, 0))
	{
		state = State::Set;
	}
}

void Game::moveMino(float delta)
{
	if(const int xDir = (::IsKeyPressed(KEY_LEFT) ? -1 : 0) + (::IsKeyPressed(KEY_RIGHT) ? 1 : 0); xDir != 0)
	{
		playfield.moveActiveMino({xDir, 0}, 0);
		moveDelay.elapsed = 0;
	}
	else
	{
		if(const int xDir = (::IsKeyDown(KEY_LEFT) ? -1 : 0) + (::IsKeyDown(KEY_RIGHT) ? 1 : 0);
		    xDir != 0 && moveDelay.tick(delta))
		{
			playfield.moveActiveMino({xDir, 0}, 0);
		}
	}
}

void Game::rotateMino(float delta)
{
	if(const int rDir = (::IsKeyPressed(KEY_A) ? -1 : 0) + (::IsKeyPressed(KEY_D) ? 1 : 0); rDir != 0)
	{
		playfield.moveActiveMino({0, 0}, rDir);
		rotateDelay.elapsed = 0;
	}
	else
	{
		if(const int rDir = (::IsKeyDown(KEY_A) ? -1 : 0) + (::IsKeyDown(KEY_D) ? 1 : 0);
		    rDir != 0 && rotateDelay.tick(delta))
		{
			playfield.moveActiveMino({0, 0}, rDir);
		}
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

void Game::update(App& app)
{
	const float delta = ::GetFrameTime();
	switch(state)
	{
	case State::Drop:
	{
		const size_t dropSpeed = ::IsKeyDown(KEY_DOWN) ? (score / 8) + 6 : (score / 8) + 2;
		dropDelay.delay = delays[std::min(dropSpeed, maxSpeedLevel)];
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

template<>
std::unique_ptr<IScene> MakeScene<Scene::Game>()
{
	return std::make_unique<Game>();
}
} // namespace raymino
