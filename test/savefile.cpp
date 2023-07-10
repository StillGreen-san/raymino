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

TEST_CASE("SaveFile::Chunk::BaseIterator", "[SaveFile]")
{
	static constexpr struct
	{
		SaveFile::Chunk::Header header1{1, 1, 16};
		std::array<uint32_t, 4> data1{11, 11, 11, 11};
		SaveFile::Chunk::Header header2{2, 2, 5};
		std::array<uint8_t, 8> data2{22, 22, 22, 22, 22, 0, 0, 0};
		SaveFile::Chunk::Header header3{3, 3, 0};
		SaveFile::Chunk::Header header4{4, 4, 16};
		std::array<uint32_t, 8> data4{44, 44, 44, 44, 44, 44, 44, 44};
	} dummyData;
	{
		SaveFile::Chunk::ConstIterator iterator(dummyData.header1);
		REQUIRE((*iterator).type == 1);
		REQUIRE((*iterator).dataBytes == 16);
		++iterator;
		REQUIRE(iterator->type == 2);
		REQUIRE(iterator->dataBytes == 5);
		++iterator;
		REQUIRE(iterator->type == 3);
		REQUIRE(iterator->dataBytes == 0);
		++iterator;
		REQUIRE(iterator->type == 4);
	}
	{
		SaveFile::Chunk::Header header{0, 0, 0};
		SaveFile::Chunk::Iterator iterator(header);
		++iterator;
	}
}
