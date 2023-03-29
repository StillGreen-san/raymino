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

#include <numeric>
#include <random>
#include <vector>

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
		dropDelay.delay = ::IsKeyDown(KEY_DOWN) ? 1.f/10.f : 1.f/3.f;
		moveMino(delta);
		rotateMino(delta);
		if(dropDelay.tick(delta))
		{
			dropMino();
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
		}
		break;
	}
}

void Game::drawPlayfield()
{
	::DrawRectangle(0, 0, 300, 600, ::RColor::Gray());

	Color colors[2]{RColor::LightGray(), RColor::DarkGray()};

	Range field = playfield.getField();
	std::advance(field.begin, HIDDEN_HEIGHT * 10);

	for(int y = 0; y < 20; ++y)
	{
		for(int x = 0; x < 10; ++x)
		{
			::DrawRectangle(x * 30, y * 30, 29, 29, colors[*field.begin]);
			++field.begin;
		}
	}

	const Playfield::ActiveMino& activeMino = playfield.getActiveMino();

	for(int y = 0; y < activeMino.collision.getSize().height; ++y)
	{
		for(int x = 0; x < activeMino.collision.getSize().width; ++x)
		{
			if(activeMino.collision.getAt({x, y}))
			{
				::DrawRectangle((x + activeMino.position.x) * 30, (y + activeMino.position.y - 4) * 30, 29, 29,
				    colors[activeMino.collision.getAt({x, y})]);
			}
		}
	}
}

void Game::draw()
{
	::BeginDrawing();

	::ClearBackground(::RColor::LightGray());

	drawPlayfield();

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
