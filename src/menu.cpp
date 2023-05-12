#include "menu.hpp"

#include "app.hpp"

#include <raygui.h>
#include <raylib-cpp.hpp>

namespace raymino
{
template<>
std::unique_ptr<IScene> MakeScene<Scene::Menu>([[maybe_unused]] App& app)
{
	return std::make_unique<Menu>();
}

Menu::Menu()
{
	::GuiSetStyle(::GuiControl::DEFAULT, ::GuiDefaultProperty::TEXT_SIZE, 30);
}

void Menu::UpdateDraw(App& app)
{
	::BeginDrawing();

	::ClearBackground(LIGHTGRAY);

	if(::GuiButton(Rectangle{60, 60, 210, 35}, "START GAME"))
	{
		app.SwitchScene(MakeScene<Scene::Game>(app));
	}

	::EndDrawing();
}
} // namespace raymino
