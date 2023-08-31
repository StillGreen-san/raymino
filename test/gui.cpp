#include "gui.hpp"

#include <catch2/catch_test_macros.hpp>

using namespace raymino;

TEST_CASE("TextList", "[Menu]")
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