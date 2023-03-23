#include "app.hpp"

#include <raylib-cpp.hpp>

#if defined(PLATFORM_WEB)
#include <emscripten/emscripten.h>
#endif

raymino::App::App()
{
	::InitWindow(600, 600, "raymino");
	::SetWindowState(FLAG_VSYNC_HINT);
}

void UpdateDraw(void* app)
{
	raymino::App& self = *static_cast<raymino::App*>(app);
	self.currentScene->UpdateDraw(self);
}

void raymino::App::Run()
{
#if defined(PLATFORM_WEB)
	emscripten_set_main_loop_arg(UpdateDraw, this, 0, 1);
#else
	while(!::WindowShouldClose())
	{
		UpdateDraw(this);
	}
#endif
}
