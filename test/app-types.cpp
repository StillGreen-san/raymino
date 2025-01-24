#include "app.hpp"
#include "ostream.hpp" //! needs to be included before catch

#include <catch2/catch_test_macros.hpp>

#include <cstring>
#include <string_view>

using namespace raymino;

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
