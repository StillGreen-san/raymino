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
	for(unsigned col = 0; rit != rend; ++rit, ++unused)
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

void Game::update(App& app)
{
}

void Game::draw()
{
	::BeginDrawing();

	::ClearBackground(::RColor::LightGray());

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
	ActiveMino next(nextMinos.front(), getStartPosition(nextMinos.front()), 42);
	nextMinos.erase(nextMinos.begin());
	return next;
}

XY Game::getStartPosition(const Grid& mino)
{
	return {4 - (mino.getSize().height - unusedBottomRows(mino)), (mino.getSize().width + 1) / 2};
}

template<>
std::unique_ptr<IScene> MakeScene<Scene::Game>()
{
	return std::make_unique<Game>();
}
} // namespace raymino
