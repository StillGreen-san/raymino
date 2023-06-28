#include "loading.hpp"

#include "app.hpp"

#include <raygui.h>
#include <raylib-cpp.hpp>

namespace raymino
{
template<>
std::unique_ptr<IScene> MakeScene<Scene::Loading>(App& app)
{
	return std::make_unique<Loading>(app);
}

Loading::Loading(App& app)
{
}

void Loading::UpdateDraw(App& app)
{
	BeginDrawing();

	ClearBackground(GetColor(static_cast<unsigned>(GuiGetStyle(DEFAULT, BACKGROUND_COLOR))));

	EndDrawing();
}
} // namespace raymino
