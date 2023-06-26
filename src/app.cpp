#include "app.hpp"

#include <raylib-cpp.hpp>

#if defined(PLATFORM_WEB)
#include <emscripten/emscripten.h>
#endif

raymino::App::App() : playerName{}
{
	HighScoreEntry::copyInto("Mino", playerName);
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

bool raymino::App::HighScores::add(const char* namePtr, ptrdiff_t score, const Settings& settings)
{
	if(entries.empty())
	{
		entries.emplace_back(namePtr, score, settings);
		return true;
	}
	if(score > entries.front().score)
	{
		entries.emplace(entries.begin(), namePtr, score, settings);
		return true;
	}
	const auto scoreRangeEnd = std::upper_bound(entries.begin(), entries.end(), score,
	    [](ptrdiff_t score, const HighScoreEntry& entry)
	    {
		    return score > entry.score;
	    });
	const auto entryPos = entries.emplace(scoreRangeEnd, namePtr, score, settings);
	const auto recordPos = std::find_if(entries.begin(), entryPos,
	    [=](const HighScoreEntry& entry)
	    {
		    return entryPos->name == entry.name && entryPos->settings == entry.settings;
	    });
	return entryPos == recordPos;
}

bool raymino::App::Settings::operator==(const raymino::App::Settings& rhs) const noexcept
{
	return compare(rhs) == 0;
}
bool raymino::App::Settings::operator!=(const raymino::App::Settings& rhs) const noexcept
{
	return compare(rhs) != 0;
}
bool raymino::App::Settings::operator>(const raymino::App::Settings& rhs) const noexcept
{
	return compare(rhs) > 0;
}
bool raymino::App::Settings::operator<(const raymino::App::Settings& rhs) const noexcept
{
	return compare(rhs) < 0;
}
int raymino::App::Settings::compare(const raymino::App::Settings& rhs) const noexcept
{
	return std::memcmp(this, &rhs, sizeof(App::Settings));
}

raymino::App::HighScoreEntry::HighScoreEntry(
    const char* namePtr, ptrdiff_t score, const raymino::App::Settings& settings) :
    name{},
    score{score}, settings{settings}
{
	copyInto(namePtr, name);
}

size_t raymino::App::HighScoreEntry::copyInto(const char* inPtr, raymino::App::HighScoreEntry::NameT& outRef)
{
	if(!inPtr)
	{
		return 0;
	}
	size_t idx = 0;
	for(; inPtr[idx] != '\0' && idx < outRef.size() - 1; ++idx)
	{
		outRef[idx] = inPtr[idx];
	}
	outRef[idx] = '\0';
	return idx;
}
