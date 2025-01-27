#pragma once

#include "textbuffer.hpp"

#include <cstddef>
#include <string>
#include <string_view>

/**
 * @brief a string view to a null terminated string that is never null
 */
class CStringView : private std::string_view
{
public:
	explicit CStringView(const std::string& string) : std::string_view(string)
	{
	}

	template<size_t N>
	explicit CStringView(const TextBuffer<N>& text) : std::string_view(text)
	{
	}

	/**
	 * @param string view to a null terminated string or empty
	 * @param alternative to use when string is empty
	 * @return CStringView constructed from string or alternative
	 */
	[[nodiscard]] static constexpr CStringView assumeTerminated(std::string_view string, CStringView alternative)
	{
		return CStringView{string.empty() ? alternative : string};
	}

	/**
	 * @return const char* to the beginnig of the view (never null)
	 */
	[[nodiscard]] constexpr const char* c_str() const
	{
		return data();
	}

	[[nodiscard]] constexpr std::string_view view() const
	{
		return *this;
	}

	friend bool operator==(const CStringView& lhs, const CStringView& rhs)
	{
		return static_cast<const std::string_view&>(lhs) == rhs;
	}

	friend bool operator==(const CStringView& lhs, std::string_view rhs)
	{
		return static_cast<const std::string_view&>(lhs) == rhs;
	}

	using std::string_view::at;
	using std::string_view::back;
	using std::string_view::begin;
	using std::string_view::cbegin;
	using std::string_view::cend;
	using std::string_view::compare;
	using std::string_view::copy;
	using std::string_view::crbegin;
	using std::string_view::crend;
	using std::string_view::data;
	using std::string_view::empty;
	using std::string_view::end;
	using std::string_view::find;
	using std::string_view::find_first_not_of;
	using std::string_view::find_first_of;
	using std::string_view::find_last_not_of;
	using std::string_view::find_last_of;
	using std::string_view::front;
	using std::string_view::length;
	using std::string_view::max_size;
	using std::string_view::rbegin;
	using std::string_view::remove_prefix;
	using std::string_view::rend;
	using std::string_view::rfind;
	using std::string_view::size;
	using std::string_view::operator[];

private:
	friend constexpr CStringView operator""_csv(const char* str, size_t len) noexcept;

	constexpr explicit CStringView(std::string_view string) : std::string_view(string)
	{
	}
};

constexpr CStringView operator""_csv(const char* str, size_t len) noexcept
{
	return CStringView{std::string_view{str, len}};
}
