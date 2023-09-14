#include "app.hpp"

#include <algorithm>
#include <cstring>

namespace raymino
{
bool App::HighScores::add(const char* namePtr, int64_t score, const Settings& settings)
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
	    [](int64_t score, const HighScoreEntry& entry)
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

bool App::Settings::operator==(const App::Settings& rhs) const noexcept
{
	return compare(rhs) == 0;
}
bool App::Settings::operator!=(const App::Settings& rhs) const noexcept
{
	return compare(rhs) != 0;
}
bool App::Settings::operator>(const App::Settings& rhs) const noexcept
{
	return compare(rhs) > 0;
}
bool App::Settings::operator<(const App::Settings& rhs) const noexcept
{
	return compare(rhs) < 0;
}
int App::Settings::compare(const App::Settings& rhs) const noexcept
{
	return std::memcmp(this, &rhs, sizeof(App::Settings));
}

bool App::KeyBinds::operator==(const App::KeyBinds& rhs) const noexcept
{
	return compare(rhs) == 0;
}
bool App::KeyBinds::operator!=(const App::KeyBinds& rhs) const noexcept
{
	return compare(rhs) != 0;
}
bool App::KeyBinds::operator>(const App::KeyBinds& rhs) const noexcept
{
	return compare(rhs) > 0;
}
bool App::KeyBinds::operator<(const App::KeyBinds& rhs) const noexcept
{
	return compare(rhs) < 0;
}
int App::KeyBinds::compare(const App::KeyBinds& rhs) const noexcept
{
	return std::memcmp(this, &rhs, sizeof(App::KeyBinds));
}

App::HighScoreEntry::HighScoreEntry(const char* namePtr, int64_t score, const App::Settings& settings) noexcept :
    name{}, score{score}, settings{settings}
{
	copyInto(namePtr, name);
}

size_t App::HighScoreEntry::copyInto(const char* inPtr, App::HighScoreEntry::NameT& outRef) noexcept
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
} // namespace raymino
