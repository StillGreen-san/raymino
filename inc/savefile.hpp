#pragma once

#include <array>
#include <vector>

namespace raymino
{
class SaveFile
{
public:
	struct Header
	{
		const std::array<char, 4> magic; // NOLINT(cppcoreguidelines-avoid-const-or-ref-data-members)
		const uint8_t formatVersion;     // NOLINT(cppcoreguidelines-avoid-const-or-ref-data-members)
		const uint8_t _reserved_;        // NOLINT(cppcoreguidelines-avoid-const-or-ref-data-members)
		uint16_t userProp1;
		uint32_t userProp2;
		uint32_t userProp3;
	};
	static_assert(sizeof(Header) == 16);
	struct Chunk
	{
		struct alignas(int64_t) Header
		{
			uint16_t type;
			uint16_t userProperty;
			uint32_t dataBytes;
		};
		template<typename T>
		struct DataRange
		{
			using THeader = std::conditional_t<std::is_const_v<T>, const Header, Header>;
			DataRange(THeader& header);
			T* first;
			T* last;
			T* begin() const;
			T* end() const;
		};
		template<typename THeader>
		struct BaseIterator
		{
			void* ptr;
			BaseIterator& operator++();
			THeader& operator*() const;
			bool operator==(BaseIterator rhs) const;
			bool operator!=(BaseIterator rhs) const;
		};
		using Iterator = BaseIterator<Header>;
		using ConstIterator = BaseIterator<const Header>;
	};
	static_assert(sizeof(Chunk::Header) == 8);
	static constexpr std::array<char, 4> magic{'R', 'B', 'S', 'F'};
	[[nodiscard]] const Header& header() const;
	[[nodiscard]] Header& header();
	[[nodiscard]] Chunk::ConstIterator begin() const;
	[[nodiscard]] Chunk::ConstIterator end() const;
	[[nodiscard]] Chunk::Iterator begin();
	[[nodiscard]] Chunk::Iterator end();
	template<typename T>
	Chunk::Header& appendChunk(uint16_t type, uint16_t flags, T first, T last);
	Chunk::Header& appendChunk(uint16_t type, uint16_t flags, uint32_t minBytes);

	const std::vector<uint8_t>& getBuffer() const;
	const uint8_t* data() const;
	uint32_t size() const;
	void reset(uint32_t chunks, uint32_t totalbytes);

	SaveFile(uint32_t chunks, uint32_t totalbytes);
	SaveFile(void*, uint32_t);

private:
	std::vector<uint8_t> dataBuffer;
};
} // namespace raymino