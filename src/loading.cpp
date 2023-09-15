#include "loading.hpp"

#include "app.hpp"

#include <raygui.h>
#include <raylib-cpp.hpp>

#if defined(PLATFORM_WEB)
#include <emscripten/emscripten.h>
#endif

namespace raymino
{
template<>
std::unique_ptr<IScene> MakeScene<Scene::Loading>(App& app)
{
	return std::make_unique<Loading>(app);
}

Loading::Loading(App& app)
{
#if defined(PLATFORM_WEB)
	::emscripten_idb_async_load(
	    App::IDB_PATH, App::FILE_PATH, &app,
	    [](void* user, void* data, int size)
	    {
		    App& app = *static_cast<App*>(user);
		    app.deserialize(App::decompressFile(data, static_cast<uint32_t>(size)));
		    app.QueueSceneSwitch(Scene::Menu);
		    ::TraceLog(LOG_INFO, "FILEIO: [%s] File loaded successfully", App::FILE_PATH);
	    },
	    [](void* user)
	    {
		    App& app = *static_cast<App*>(user);
		    app.QueueSceneSwitch(Scene::Menu);
		    ::TraceLog(LOG_INFO, "FILEIO: [%s] Failed to load file", App::FILE_PATH);
	    });
#else
	unsigned bytes{};
	unsigned char* fileData = ::LoadFileData(App::FILE_PATH, &bytes);
	if(fileData)
	{
		app.deserialize(App::decompressFile(fileData, bytes));
		::UnloadFileData(fileData);
	}
	app.QueueSceneSwitch(Scene::Menu);
#endif
}

void Loading::PreDestruct([[maybe_unused]] App& app)
{
}

void Loading::UpdateDraw([[maybe_unused]] App& app)
{
	::BeginDrawing();

	::ClearBackground(::GetColor(static_cast<unsigned>(::GuiGetStyle(DEFAULT, BACKGROUND_COLOR))));

	::DrawText("LOADING", 190, 278, 50, ::GetColor(static_cast<unsigned>(::GuiGetStyle(DEFAULT, TEXT_COLOR_FOCUSED))));

	::EndDrawing();
}
} // namespace raymino
