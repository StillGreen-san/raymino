#include "ostream.hpp" //! needs to be included before catch

#include <catch2/catch_test_macros.hpp>

#include "app.hpp"

#include <string_view>

using namespace raymino;

TEST_CASE("App::HighScoreEntry::copyInto", "[App]")
{
	App::HighScoreEntry::NameT nameBuffer{};

	{
		constexpr std::string_view nameStr = "Mino";
		REQUIRE(App::HighScoreEntry::copyInto(nameStr.data(), nameBuffer) == nameStr.size());
		REQUIRE(std::strncmp(nameBuffer.data(), nameStr.data(), nameStr.size() + 1) == 0);
	}
	{
		constexpr std::string_view nameStr = "Tetromino";
		constexpr std::string_view nameExpected = "Tetromi";
		REQUIRE(App::HighScoreEntry::copyInto(nameStr.data(), nameBuffer) == nameBuffer.size() - 1);
		REQUIRE(std::strncmp(nameBuffer.data(), nameExpected.data(), nameBuffer.size()) == 0);
	}
	{
		constexpr std::string_view nameStr = "Me";
		REQUIRE(App::HighScoreEntry::copyInto(nameStr.data(), nameBuffer) == nameStr.size());
		REQUIRE(std::strncmp(nameBuffer.data(), nameStr.data(), nameStr.size() + 1) == 0);
	}
	REQUIRE(App::HighScoreEntry::copyInto("", nameBuffer) == 0);
	REQUIRE(App::HighScoreEntry::copyInto(nullptr, nameBuffer) == 0);
}

TEST_CASE("App::HighScores::add", "[App]")
{
	App::Settings settings;
	App::HighScores scores;

	REQUIRE(scores.add("name", 1, settings) == true);
	REQUIRE(scores.add("name", 2, settings) == true);
	REQUIRE(scores.add("name", 4, settings) == true);
	REQUIRE(scores.add("name", 3, settings) == false);
	REQUIRE(scores.add("mino", 3, settings) == true);
	settings.ghostPiece = false;
	REQUIRE(scores.add("name", 3, settings) == true);
	REQUIRE(scores.add("name", 4, settings) == true);
	REQUIRE(scores.add("name", 3, settings) == false);
	settings.ghostPiece = true;
	REQUIRE(scores.add("mino", 3, settings) == false);
	REQUIRE(scores.entries.size() == 9);
}
