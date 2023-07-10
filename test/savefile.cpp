#include "savefile.hpp"

#include <catch2/catch_test_macros.hpp>

using namespace raymino;

TEST_CASE("SaveFile::Chunk::DataRange", "[SaveFile]")
{
	constexpr auto isNextToHeader = [](auto iterator, auto& header) -> bool
	{
		return static_cast<const void*>(iterator) == static_cast<const void*>((&header) + 1);
	};
	{
		SaveFile::Chunk::Header header{0, 0, 32};
		const SaveFile::Chunk::DataRange<uint32_t> range(header);
		REQUIRE(isNextToHeader(range.begin(), header));
		REQUIRE(std::distance(range.begin(), range.end()) == 8);
	}
	{
		SaveFile::Chunk::Header header{0, 0, 10};
		const SaveFile::Chunk::DataRange<const uint16_t> range(header);
		REQUIRE(isNextToHeader(range.begin(), header));
		REQUIRE(std::distance(range.begin(), range.end()) == 5);
	}
	{
		const SaveFile::Chunk::Header header{0, 0, 0};
		const SaveFile::Chunk::DataRange<const uint32_t> range(header);
		REQUIRE(isNextToHeader(range.begin(), header));
		REQUIRE(std::distance(range.begin(), range.end()) == 0);
	}
	{
		SaveFile::Chunk::Header header{0, 0, 25};
		REQUIRE_THROWS(
		    [&]()
		    {
			    const SaveFile::Chunk::DataRange<double> range(header);
		    }());
	}
}
