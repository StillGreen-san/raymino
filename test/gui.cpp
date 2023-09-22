#include "gui.hpp"

#include <catch2/catch_test_macros.hpp>

#include <string_view>
using namespace std::string_view_literals;

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

TEST_CASE("splitCamel", "[GUI]")
{
	SECTION("small")
	{
		const std::string empty = splitCamel("");
		REQUIRE(empty.empty());

		const std::string one = splitCamel("A");
		REQUIRE(one == "A");

		const std::string six = splitCamel("AB");
		REQUIRE(six == "AB");

		const std::string ten = splitCamel("ABc");
		REQUIRE(ten == "ABc");
	}
	SECTION("camel")
	{
		const std::string one = splitCamel("someWords");
		REQUIRE(one == "some Words");

		const std::string two = splitCamel("someMoreDifferentWordsAB");
		REQUIRE(two == "some More Different Words AB");
	}
	SECTION("pascal")
	{
		const std::string one = splitCamel("MyText");
		REQUIRE(one == "My Text");

		const std::string two = splitCamel("MyText2ButDifferentXJ");
		REQUIRE(two == "My Text 2 But Different XJ");

		const std::string six = splitCamel("AB3");
		REQUIRE(six == "AB3");
	}
	SECTION("other")
	{
		const std::string one = splitCamel("IMPORTANT_THING-HERE7");
		REQUIRE(one == "IMPORTANT_THING-HERE7");

		const std::string two = splitCamel("Franz jagt im komplett verwahrlosten Taxi quer durch Bayern.123");
		REQUIRE(two == "Franz jagt im komplett verwahrlosten Taxi quer durch Bayern.123");

		const std::string six = splitCamel("AppleBoots64");
		REQUIRE(six == "Apple Boots 64");
	}
}

TEST_CASE("NumberBuffer", "[GUI]")
{
	SECTION("Constructor")
	{
		{
			NumberBuffer nb(0);
			REQUIRE(nb.value() == 0);
			REQUIRE(nb.c_str() == "0"sv);
		}
		{
			NumberBuffer nb(1234567, '#');
			REQUIRE(nb.value() == 1234567);
			REQUIRE(nb.c_str() == "1#234#567"sv);
		}
		{
			NumberBuffer nb(-9999);
			REQUIRE(nb.value() == -9999);
			REQUIRE(nb.c_str() == "-9.999"sv);
		}
	}
	SECTION("operator+=")
	{
		{
			NumberBuffer nb(0);
			nb += 512;
			REQUIRE(nb.value() == 512);
			REQUIRE(nb.c_str() == "512"sv);
		}
		{
			NumberBuffer nb(1234567, ',');
			nb += -234568;
			REQUIRE(nb.value() == 999999);
			REQUIRE(nb.c_str() == "999,999"sv);
		}
		{
			NumberBuffer nb(999);
			nb += 1000;
			REQUIRE(nb.value() == 1999);
			REQUIRE(nb.c_str() == "1.999"sv);
		}
	}
	SECTION("backFillSeparated")
	{
		{
			NumberBuffer::TBufferType buffer{};
			NumberBuffer::backFillSeparated(buffer, 123, '.');
			REQUIRE(std::search_n(buffer.begin(), buffer.end(), 23, '\0') == buffer.begin());
			REQUIRE(&buffer[23] == "123"sv);
		}
		{
			NumberBuffer::TBufferType buffer{};
			NumberBuffer::backFillSeparated(buffer, -1234, '.');
			REQUIRE(std::search_n(buffer.begin(), buffer.end(), 20, '\0') == buffer.begin());
			REQUIRE(&buffer[20] == "-1.234"sv);
		}
		{
			NumberBuffer::TBufferType buffer{};
			NumberBuffer::backFillSeparated(buffer, 123456, '!');
			REQUIRE(std::search_n(buffer.begin(), buffer.end(), 19, '\0') == buffer.begin());
			REQUIRE(&buffer[19] == "123!456"sv);
		}
	}
}
