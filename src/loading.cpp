#include "loading.hpp"

#include "app.hpp"
#include "scenes.hpp"

#include <FileData.hpp>
#include <raygui.h>
#include <raylib.h>

#include <memory>

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
	    App::IDB_PATH, App::SAVE_PATH, &app,
	    [](void* user, void* data, int size)
	    {
		    App& app = *static_cast<App*>(user);
		    app.deserialize(App::decompressFile(data, static_cast<uint32_t>(size)));
		    app.QueueSceneSwitch(Scene::Menu);
		    ::TraceLog(LOG_INFO, "FILEIO: [%s] File loaded successfully", App::SAVE_PATH);
	    },
	    [](void* user)
	    {
		    App& app = *static_cast<App*>(user);
		    app.QueueSceneSwitch(Scene::Menu);
		    ::TraceLog(LOG_INFO, "FILEIO: [%s] Failed to load file", App::SAVE_PATH);
	    });
#else
	const raylib::FileData fileData(App::SAVE_PATH);
	app.deserialize(App::decompressFile(fileData.GetData(), static_cast<uint32_t>(fileData.GetBytesRead())));
	app.QueueSceneSwitch(Scene::Menu);
#endif
}

void Loading::PreDestruct([[maybe_unused]] App& app)
{
	// nothing to do
}

void Loading::Update([[maybe_unused]] App& app)
{
	// nothing to do
}

void Loading::Draw([[maybe_unused]] App& app)
{
	::ClearBackground(::GetColor(static_cast<unsigned>(::GuiGetStyle(DEFAULT, BACKGROUND_COLOR))));

	::DrawText("LOADING", 190, 278, 50, ::GetColor(static_cast<unsigned>(::GuiGetStyle(DEFAULT, TEXT_COLOR_FOCUSED))));
}
} // namespace raymino
