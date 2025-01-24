#include "textbuffer.hpp"

#include <catch2/catch_test_macros.hpp>

#include <algorithm>
#include <iterator>
#include <string_view>

// NOLINTBEGIN(*-container-size-empty)

TEST_CASE("TextBuffer::TextBuffer()", "[TextBuffer]")
{
	const TextBuffer<16> buffer;
	CHECK(buffer.empty());
	CHECK(buffer.length() == 0);
	CHECK(buffer.size() == 15);
	CHECK(buffer.c_str() == buffer.c_end());
	CHECK(buffer == std::string_view{});
}

TEST_CASE("TextBuffer::TextBuffer(string_view)", "[TextBuffer]")
{
	constexpr std::string_view value("Hello World");
	const TextBuffer<16> buffer(value);
	CHECK_FALSE(buffer.empty());
	CHECK(buffer.length() == value.length());
	CHECK(buffer.size() == 15);
	CHECK(buffer.c_str() != buffer.c_end());
	CHECK(std::distance(buffer.c_str(), buffer.c_end()) == value.end() - value.begin());
	CHECK(buffer == value);
}

TEST_CASE("TextBuffer::operator=(string_view)", "[TextBuffer]")
{
	// ReSharper disable once CppDFAUnusedValue
	TextBuffer<16> buffer("Servus");
	CHECK(buffer.length() == 6);
	constexpr std::string_view expected{"Moin"};
	buffer = expected;
	CHECK(buffer == expected);
	CHECK(std::string_view{buffer.c_str(), buffer.length()} == expected);
}

TEST_CASE("TextBuffer::operator+=(string_view) < SIZE", "[TextBuffer]")
{
	TextBuffer<16> buffer("Hello");
	constexpr std::string_view expected{"Hello World"};
	buffer += " World";
	CHECK(buffer == expected);
	CHECK(TextBuffer<16>(expected) == buffer);
}

TEST_CASE("TextBuffer::operator+=(string_view) > SIZE", "[TextBuffer]")
{
	TextBuffer<8> buffer("Hello");
	constexpr std::string_view expected{"Hello W"};
	buffer += " World";
	CHECK(buffer == expected);
	CHECK(buffer == TextBuffer<16>(expected));
}

TEST_CASE("TextBuffer full contents", "[TextBuffer]")
{
	const TextBuffer<8> buffer("BUILD");
	constexpr std::string_view expected{"BUILD\0\0", 7};
	CHECK(std::string_view{buffer.c_str(), buffer.size()} == expected);
	// NOLINTNEXTLINE(*-suspicious-stringview-data-usage)
	CHECK(std::equal(buffer.c_str(), buffer.end_ptr(), expected.data(), std::next(expected.data(), expected.size())));
}

// NOLINTEND(*-container-size-empty)
