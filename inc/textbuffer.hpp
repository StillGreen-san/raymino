#pragma once

#include <algorithm>
#include <array>
#include <cstddef>
#include <cstring>
#include <string_view>

/**
 * @brief fixed size text buffer with null terminator
 * @tparam NTotalSize total object size including the null terminator (at least 2)
 */
template<size_t NTotalSize>
class TextBuffer
{
public:
	/**
	 * @brief empty buffer
	 */
	TextBuffer() = default;

	/**
	 * @brief construct with value, excess data in value is ignored
	 * @param value to copy
	 */
	explicit TextBuffer(std::string_view value)
	{
		operator+=(value);
	}

	/**
	 * @brief set to value, excess data in value is ignored
	 * @param other str to copy
	 * @return this ref
	 */
	TextBuffer& operator=(std::string_view other)
	{
		buffer.fill(DELIMITER);
		operator+=(other);
		return *this;
	}

	/**
	 * @brief append value, excess data in value is ignored
	 * @param other str to append
	 * @return this ref
	 */
	TextBuffer& operator+=(std::string_view other)
	{
		const auto len = static_cast<ptrdiff_t>(length());
		const auto rem = size() - static_cast<size_t>(len);
		std::copy_n(other.data(), std::min(rem, other.length()), std::next(data(), len));
		return *this;
	}

	/**
	 * @return const char* to the null-terminated buffer data
	 */
	[[nodiscard]] const char* data() const
	{
		return buffer.data();
	}

	/**
	 * @return char* to the null-terminated buffer data
	 */
	[[nodiscard]] char* data()
	{
		return buffer.data();
	}

	/**
	 * @return size_t length of the text, not the whole buffer
	 */
	[[nodiscard]] size_t length() const
	{
		return std::strlen(data());
	}

	/**
	 * @return ptrdtiff_t length of the text, not the whole buffer
	 */
	[[nodiscard]] ptrdiff_t slength() const
	{
		return static_cast<ptrdiff_t>(length());
	}

	/**
	 * @brief the total size of the buffer, not including the null terminator
	 */
	static constexpr size_t SIZE = NTotalSize - 1;

	/**
	 * @return size_t the total size of the buffer, not including the null terminator
	 */
	[[nodiscard]] size_t size() const
	{
		return SIZE;
	}

	/**
	 * @return bool true if the text is empty, false otherwise.
	 */
	[[nodiscard]] bool empty() const
	{
		return buffer.front() == DELIMITER;
	}

	/**
	 * @return const char* to the null-terminated text in this buffer
	 */
	[[nodiscard]] const char* c_str() const
	{
		return data();
	}

	/**
	 * @return char* to the end of the text (not the whole buffer)
	 */
	[[nodiscard]] char* c_end()
	{
		return std::next(data(), slength());
	}

	/**
	 * @return const char* to the end of the text (not the whole buffer)
	 */
	[[nodiscard]] const char* c_end() const
	{
		return std::next(data(), slength());
	}

	/**
	 * @return iterator to the beginning of the buffer
	 */
	[[nodiscard]] auto begin()
	{
		return buffer.begin();
	}

	/**
	 * @return iterator to the end of the buffer
	 */
	[[nodiscard]] auto end()
	{
		return buffer.end();
	}

	/**
	 * @return char* to the end of the buffer
	 */
	[[nodiscard]] char* end_ptr()
	{
		return &delimiter;
	}

	/**
	 * @return const char* to the end of the buffer
	 */
	[[nodiscard]] const char* end_ptr() const
	{
		return &delimiter;
	}

	/**
	 * @brief implicit conversion to std::string_view
	 * @return std::string_view of the text
	 */
	[[nodiscard]] operator std::string_view() const // NOLINT(*-explicit-constructor, *-explicit-conversions)
	{
		return c_str();
	}

	/**
	 * @return bool true if the string_view representation of lhs is equal to rhs, false otherwise.
	 */
	[[nodiscard]] friend bool operator==(const TextBuffer& lhs, std::string_view rhs)
	{
		return std::string_view{lhs} == rhs;
	}

	/**
	 * @brief null-terminator
	 */
	static constexpr char DELIMITER = '\0';

private:
	static_assert(NTotalSize >= 2, "minimum size for TextBuffer is 2");
	std::array<char, NTotalSize - 1> buffer{};
	/*const*/ char delimiter = DELIMITER;
};
