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

TEST_CASE("SaveFile & reset", "[SaveFile]")
{
	SaveFile save(2, 120);
	REQUIRE(save.header().userProp1 == 0);
	REQUIRE(save.header().userProp2 == 0);
	REQUIRE(save.header().userProp3 == 0);
	REQUIRE(save.begin() == save.end());

	save.header().userProp2 = 42;
	save.reset(0, 0);
	REQUIRE(save.header().userProp2 == 0);
}

TEST_CASE("SaveFile::appendChunk", "[SaveFile]")
{
	SaveFile save(2, 44);
	std::vector<int32_t> intVec{1, 2, 3, 4, 5, 6};

	SaveFile::Chunk::Header& header1 = save.appendChunk(13, 42, intVec.begin(), intVec.end());
	REQUIRE(header1.type == 13);
	REQUIRE(header1.userProperty == 42);
	REQUIRE(header1.dataBytes == 24);
	const SaveFile::Chunk::DataRange<int32_t> range1(header1);
	REQUIRE(std::equal(intVec.begin(), intVec.end(), range1.begin(), range1.end()));

	intVec.pop_back();
	const SaveFile::Chunk::Header& header2 = save.appendChunk(7, 7, intVec.cbegin(), intVec.cend());
	REQUIRE(header2.type == 7);
	REQUIRE(header2.userProperty == 7);
	REQUIRE(header2.dataBytes == 20);
	const SaveFile::Chunk::DataRange<const int32_t> range2(header2);
	REQUIRE(std::equal(intVec.cbegin(), intVec.cend(), range2.begin(), range2.end()));

	const SaveFile::Chunk::Header& header3 = save.appendChunk(69, 0, 0);
	REQUIRE(header3.type == 69);
	REQUIRE(header3.userProperty == 0);
	REQUIRE(header3.dataBytes == 0);
	const SaveFile::Chunk::DataRange<const double> range3(header3);
	REQUIRE(range3.begin() == range3.end());
}
