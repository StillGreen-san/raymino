#pragma once

#include <array>
#include <stdexcept>
#include <vector>

template<typename TIn, typename TOut>
using same_const_as_t = std::conditional_t<std::is_const_v<TIn>, const TOut, TOut>;

namespace raymino
{
/**
 * @brief somewhat generic binary format
 */
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
			const uint32_t dataBytes; // NOLINT(cppcoreguidelines-avoid-const-or-ref-data-members)
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
			T* begin() const noexcept
			{
				return static_cast<T*>(first);
			}
			T* end() const noexcept
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
			explicit BaseIterator(THeader& header) noexcept : header{&header}
			{
			}
			BaseIterator& operator++() noexcept
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
			THeader& operator*() const noexcept
			{
				return *static_cast<THeader*>(header);
			}
			THeader* operator->() const noexcept
			{
				return static_cast<THeader*>(header);
			}
			bool operator==(BaseIterator rhs) const noexcept
			{
				return header == rhs.header;
			}
			bool operator!=(BaseIterator rhs) const noexcept
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
	[[nodiscard]] const Header& header() const noexcept;
	[[nodiscard]] Header& header() noexcept;
	[[nodiscard]] Chunk::ConstIterator begin() const noexcept;
	[[nodiscard]] Chunk::ConstIterator end() const noexcept;
	[[nodiscard]] Chunk::Iterator begin() noexcept;
	[[nodiscard]] Chunk::Iterator end() noexcept;

	/**
	 * @brief add chunk filled with data from first to last
	 * @tparam T iterator type
	 * @param type of header
	 * @param flags user property of header
	 * @param first data range begin
	 * @param last data range end
	 * @return Chunk::Header&
	 */
	template<typename T>
	Chunk::Header& appendChunk(uint16_t type, uint16_t flags, T first, T last)
	{
		using TValueType = std::remove_cv_t<std::remove_reference_t<decltype(*first)>>;
		const uint32_t bytes = static_cast<uint32_t>(std::distance(first, last)) * sizeof(TValueType);
		Chunk::Header& header = appendChunk(type, flags, bytes);
		std::uninitialized_copy(first, last, Chunk::DataRange<TValueType>(header).begin());
		return header;
	}

	/**
	 * @brief add empty chunk (zeroed)
	 * @param type of header
	 * @param flags user property of header
	 * @param bytes of chunk data
	 * @return Chunk::Header&
	 */
	Chunk::Header& appendChunk(uint16_t type, uint16_t flags, uint32_t bytes);

	[[nodiscard]] const std::vector<uint8_t>& getBuffer() const noexcept;
	[[nodiscard]] const uint8_t* data() const noexcept;
	[[nodiscard]] uint32_t size() const noexcept;

	/**
	 * @brief reset to empty save with reserved storage
	 * @param chunks number intended to be added
	 * @param totalBytes of chunk data intended to be added
	 */
	void reset(uint32_t chunks, uint32_t totalBytes);

	/**
	 * @brief create empty save with reserved storage
	 * @param chunks number intended to be added
	 * @param totalBytes of chunk data intended to be added
	 */
	SaveFile(uint32_t chunks, uint32_t totalBytes);

	/**
	 * @brief create save from existing buffer, only checks header
	 */
	explicit SaveFile(std::vector<uint8_t> data);

	static bool isValid(Header header) noexcept;

private:
	std::vector<uint8_t> dataBuffer;
};
} // namespace raymino