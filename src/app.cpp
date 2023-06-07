#include "app.hpp"

#include <raylib-cpp.hpp>

#if defined(PLATFORM_WEB)
#include <emscripten/emscripten.h>
#endif

raymino::App::App()
{
	::InitWindow(Settings::SCREEN_WIDTH, Settings::SCREEN_HEIGHT, "raymino");
	::SetWindowState(FLAG_VSYNC_HINT);
	::SetExitKey(KEY_NULL);
}

void UpdateDraw(void* app)
{
	raymino::App& self = *static_cast<raymino::App*>(app);
	self.UpdateDraw();
}

void raymino::App::UpdateDraw()
{
	if(nextScene)
	{
		currentScene = std::move(nextScene);
	}
	currentScene->UpdateDraw(*this);
}

void raymino::App::Run()
{
#if defined(PLATFORM_WEB)
	emscripten_set_main_loop_arg(::UpdateDraw, this, 0, 1);
#else
	while(!::WindowShouldClose())
	{
		::UpdateDraw(this);
	}
#endif
}

void raymino::App::QueueSceneSwitch(std::unique_ptr<IScene> newScene)
{
	nextScene = std::move(newScene);
}

bool raymino::App::Settings::operator==(const raymino::App::Settings& rhs) const
{
	return rotationSystem == rhs.rotationSystem && wallKicks == rhs.wallKicks && lockDown == rhs.lockDown &&
	       softDrop == rhs.softDrop && instantDrop == rhs.instantDrop && tSpin == rhs.tSpin &&
	       shuffleType == rhs.shuffleType && scoringSystem == rhs.scoringSystem && levelGoal == rhs.levelGoal &&
	       holdPiece == rhs.holdPiece && fieldWidth == rhs.fieldWidth && fieldHeight == rhs.fieldHeight &&
	       previewCount == rhs.previewCount;
}
bool raymino::App::Settings::operator!=(const raymino::App::Settings& rhs) const
{
	return rotationSystem != rhs.rotationSystem || wallKicks != rhs.wallKicks || lockDown != rhs.lockDown ||
	       softDrop != rhs.softDrop || instantDrop != rhs.instantDrop || tSpin != rhs.tSpin ||
	       shuffleType != rhs.shuffleType || scoringSystem != rhs.scoringSystem || levelGoal != rhs.levelGoal ||
	       holdPiece != rhs.holdPiece || fieldWidth != rhs.fieldWidth || fieldHeight != rhs.fieldHeight ||
	       previewCount != rhs.previewCount;
}
