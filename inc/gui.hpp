#pragma once

#include <string>
#include <string_view>

namespace raymino
{
/**
 * @brief a ';' seperated list of strings
 */
class TextList
{
public:
	static constexpr char delimiter = ';';

	/**
	 * @brief empty list
	 */
	TextList() = default;

	/**
	 * @brief list with items
	 * @param items to add (0 or more, seperated by ';')
	 */
	TextList(std::string_view items) : list{items}
	{
	}

	/**
	 * @param items to add (0 or more, seperated by ';')
	 */
	void add(std::string_view items);

	/**
	 * @brief remove item at index if present
	 */
	void remove(size_t index);

	/**
	 * @return item at index or empty view
	 */
	[[nodiscard]] std::string_view get(size_t index) const;

	/**
	 * @return complete list
	 */
	[[nodiscard]] std::string_view get() const;

	/**
	 * @return number of items
	 */
	[[nodiscard]] size_t size() const;

private:
	std::string list;
};
} // namespace raymino
