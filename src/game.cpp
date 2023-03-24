#include "game.hpp"

#include "app.hpp"
#include "grid.hpp"
#include "scenes.hpp"
#include "types.hpp"

#include <raygui.h>
#include <raylib-cpp.hpp>
#include <rres-raylib.h>

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

template<>
std::unique_ptr<IScene> MakeScene<Scene::Game>()
{
	return std::make_unique<Game>();
}
} // namespace raymino
