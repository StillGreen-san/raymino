#include "app.hpp"

#include <algorithm>
#include <cstdint>
#include <cstring>
#include <string_view>

namespace raymino
{
bool App::HighScores::add(std::string_view name, int64_t score, const Settings& settings)
{
	if(entries.empty())
	{
		entries.emplace_back(name, score, settings);
		return true;
	}
	if(score > entries.front().score)
	{
		entries.emplace(entries.begin(), name, score, settings);
		return true;
	}
	const auto scoreRangeEnd = std::upper_bound(entries.begin(), entries.end(), score,
	    [](int64_t score, const HighScoreEntry& entry)
	    {
		    return score > entry.score;
	    });
	const auto entryPos = entries.emplace(scoreRangeEnd, name, score, settings);
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
} // namespace raymino
