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
