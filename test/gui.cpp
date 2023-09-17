#include "gui.hpp"

#include <catch2/catch_test_macros.hpp>

using namespace raymino;

TEST_CASE("TextList", "[GUI]")
{
	SECTION("TextList()")
	{
		const TextList list;
		REQUIRE(list.size() == 0);
		REQUIRE(list.get(0).empty());
		REQUIRE(list.get().empty());
	}
	SECTION("TextList(string_view)")
	{
		const std::string_view items = "item1;item2;item3";
		const TextList list(items);
		REQUIRE(list.size() == 3);
		REQUIRE(list.get(0) == "item1");
		REQUIRE(list.get(1) == "item2");
		REQUIRE(list.get(2) == "item3");
		REQUIRE(list.get() == items);
	}
	SECTION("add")
	{
		TextList list;

		list.add("item1");
		REQUIRE(list.size() == 1);
		REQUIRE(list.get(0) == "item1");

		list.add("item2");
		REQUIRE(list.size() == 2);
		REQUIRE(list.get(1) == "item2");

		list.add("item3");
		REQUIRE(list.size() == 3);
		REQUIRE(list.get(2) == "item3");
		REQUIRE(list.get(3).empty());
	}
	SECTION("remove")
	{
		TextList list("item1;item2;item3;item4");

		list.remove(0);
		REQUIRE(list.size() == 3);
		REQUIRE(list.get(0) == "item2");

		list.remove(1);
		REQUIRE(list.size() == 2);
		REQUIRE(list.get(0) == "item2");
		REQUIRE(list.get(1) == "item4");
		REQUIRE(list.get(2).empty());

		list.remove(1);
		REQUIRE(list.size() == 1);
		REQUIRE(list.get(0) == "item2");
		REQUIRE(list.get(1).empty());

		list.remove(0);
		REQUIRE(list.size() == 0);
	}
}

TEST_CASE("splitUpper", "[GUI]")
{
	SECTION("small")
	{
		const std::string empty = splitUpper("");
		REQUIRE(empty.empty());

		const std::string one = splitUpper("A");
		REQUIRE(one == "A");

		const std::string six = splitUpper("AB");
		REQUIRE(six == "A B");

		const std::string ten = splitUpper("ABc");
		REQUIRE(ten == "A Bc");
	}
	SECTION("camel")
	{
		const std::string one = splitUpper("someWords");
		REQUIRE(one == "some Words");

		const std::string two = splitUpper("someMoreDifferentWordsAB");
		REQUIRE(two == "some More Different Words A B");
	}
	SECTION("pascal")
	{
		const std::string one = splitUpper("MyText");
		REQUIRE(one == "My Text");

		const std::string two = splitUpper("MyTextButDifferentXJ");
		REQUIRE(two == "My Text But Different X J");
	}
	SECTION("other")
	{
		const std::string one = splitUpper("IMPORTANT_THING-HERE");
		REQUIRE(one == "I M P O R T A N T_ T H I N G- H E R E");

		const std::string two = splitUpper("Franz jagt im komplett verwahrlosten Taxi quer durch Bayern.123");
		REQUIRE(two == "Franz jagt im komplett verwahrlosten  Taxi quer durch  Bayern.123");
	}
}
