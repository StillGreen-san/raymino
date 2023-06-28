#include "app.hpp"

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