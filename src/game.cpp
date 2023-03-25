#include "game.hpp"

#include "app.hpp"
#include "grid.hpp"
#include "scenes.hpp"
#include "types.hpp"

#include <raygui.h>
#include <raylib-cpp.hpp>
#include <rres-raylib.h>

#include <random>
#include <vector>

namespace raymino
{
struct IfSet
{
	uint8_t newVal;
	uint8_t operator()(uint8_t oldVal) const
	{
		if(oldVal)
		{
			return newVal;
		}
		return oldVal;
	}
};

bool bitAndTest(uint8_t lhs, uint8_t rhs)
{
	return lhs & rhs;
}

unsigned unusedBottomRows(const Grid& mino)
{
	const auto rend = mino.getCells().rend();
	auto rit = mino.getCells().rbegin();
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

ActiveMino::ActiveMino(const Grid& mino, XY position, uint8_t color) :
    color{mino, IfSet{color}}, collision{mino, IfSet{0xFF}}, position{position}
{
}

void Game::dropMino()
{
	const int xDir = (::IsKeyDown(KEY_LEFT) ? -1 : 0) + (::IsKeyDown(KEY_RIGHT) ? 1 : 0);
	const int yDir = xDir == 0 ? 1 : 0;
	XY nextPosition{activeMino.position.x + xDir, activeMino.position.y + yDir};
	if(nextPosition.x < 0 || nextPosition.x > 10 - activeMino.color.getSize().width)
	{
		nextPosition.x = activeMino.position.x;
		nextPosition.y += 1;
	}
	if(playArea.overlapAt(nextPosition, activeMino.collision))
	{
		state = State::Set;
	}
	else
	{
		activeMino.position = nextPosition;
	}
}

void Game::setMino()
{
	if(activeMino.position.y == getStartPosition(activeMino.color).y)
	{
		state = State::Over;
		return;
	}
	playArea.setAt(activeMino.position, activeMino.color);
	activeMino = takeNextMino();
	if(playArea.overlapAt({activeMino.position.x, activeMino.position.y + 1}, activeMino.collision))
	{
		state = State::Over;
	}
	else
	{
		state = State::Drop;
	}
}

void Game::update(App& app)
{
	switch(state)
	{
	case State::Drop:
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
			playArea = Grid(playArea.getSize(), 0);
		}
		break;
	}
}

void Game::drawPlayfield()
{
	::DrawRectangle(0, 0, 300, 600, ::RColor::Gray());

	Color colors[2]{RColor::LightGray(), RColor::DarkGray()};

	for(int y = 0; y < 20; ++y)
	{
		for(int x = 0; x < 10; ++x)
		{
			::DrawRectangle(x * 30, y * 30, 29, 29, colors[playArea.getAt({x, y + 4})]);
		}
	}

	for(int y = 0; y < activeMino.color.getSize().height; ++y)
	{
		for(int x = 0; x < activeMino.color.getSize().width; ++x)
		{
			if(activeMino.color.getAt({x, y}))
			{
				::DrawRectangle((x + activeMino.position.x) * 30,
				    (y + activeMino.position.y - 4) * 30, 29, 29,
				    colors[activeMino.color.getAt({x, y})]);
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

std::vector<Grid> Game::shuffledBaseMinos()
{
	std::shuffle(baseMinos.begin(), baseMinos.end(), std::default_random_engine{std::random_device{}()});
	return baseMinos;
}

ActiveMino Game::takeNextMino()
{
	if(nextMinos.empty())
	{
		nextMinos = shuffledBaseMinos();
	}
	ActiveMino next(nextMinos.front(), getStartPosition(nextMinos.front()), 1);
	nextMinos.erase(nextMinos.begin());
	return next;
}

XY Game::getStartPosition(const Grid& mino)
{
	return {5 - ((static_cast<int>(mino.getSize().width) + 1) / 2),
	    4 - (static_cast<int>(mino.getSize().height) - static_cast<int>(unusedBottomRows(mino)))};
}

template<>
std::unique_ptr<IScene> MakeScene<Scene::Game>()
{
	return std::make_unique<Game>();
}
} // namespace raymino
