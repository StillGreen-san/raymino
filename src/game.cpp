#include "game.hpp"

#include "app.hpp"
#include "grid.hpp"
#include "playfield.hpp"
#include "scenes.hpp"
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

void Game::moveMino()
{
	const int rDir = (::IsKeyReleased(KEY_A) ? -1 : 0) + (::IsKeyReleased(KEY_D) ? 1 : 0);
	if(rDir != 0)
	{
		playfield.moveActiveMino({0, 0}, rDir);
	}

	const int xDir = (::IsKeyReleased(KEY_LEFT) ? -1 : 0) + (::IsKeyReleased(KEY_RIGHT) ? 1 : 0);
	playfield.moveActiveMino({xDir, 0}, 0);
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
	switch(state)
	{
	case State::Drop:
		moveMino();
		time += ::GetFrameTime() * (::IsKeyDown(KEY_DOWN) ? 3 : 1);
		if(time > delay)
		{
			dropMino();
			time -= delay;
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
