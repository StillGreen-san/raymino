#pragma once

#include <array>
#include <stdexcept>
#include <vector>

template<typename TIn, typename TOut>
using same_const_as_t = std::conditional_t<std::is_const_v<TIn>, const TOut, TOut>;

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
		class DataRange
		{
			using THeader = same_const_as_t<T, Header>;
			using TVoidStore = same_const_as_t<T, void>;

		public:
			explicit DataRange(THeader& header) : first{&header + 1}, last{begin() + (header.dataBytes / sizeof(T))}
			{
				if(header.dataBytes % sizeof(T) != 0)
				{
					throw std::range_error("data buffer size mismatch");
				}
			}
			T* begin() const
			{
				return static_cast<T*>(first);
			}
			T* end() const
			{
				return static_cast<T*>(last);
			}

		private:
			TVoidStore* first;
			TVoidStore* last;
		};
		template<typename THeader>
		class BaseIterator
		{
			using TVoidStore = same_const_as_t<THeader, void>;
			using TIntType = same_const_as_t<THeader, uint8_t>;

		public:
			explicit BaseIterator(THeader& header) : header{&header}
			{
			}
			BaseIterator& operator++()
			{
				auto* ptr = static_cast<TIntType*>(header);
				auto paddingBytes = operator*().dataBytes & (alignof(THeader) - 1);
				if(paddingBytes != 0)
				{
					paddingBytes = alignof(THeader) - paddingBytes;
				}
				ptr += sizeof(THeader) + operator*().dataBytes + paddingBytes;
				header = ptr;
				return *this;
			}
			THeader& operator*() const
			{
				return *static_cast<THeader*>(header);
			}
			THeader* operator->() const
			{
				return static_cast<THeader*>(header);
			}
			bool operator==(BaseIterator rhs) const
			{
				return header == rhs.header;
			}
			bool operator!=(BaseIterator rhs) const
			{
				return header != rhs.header;
			}

		private:
			TVoidStore* header;
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