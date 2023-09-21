#include "gui.hpp"

namespace raymino
{
size_t find_nth(const std::string& str, size_t nth, char chr) noexcept
{
	size_t pos = nth == 0 ? 0 : -1;
	for(; nth > 0; --nth)
	{
		pos = str.find(chr, pos + 1);
		if(pos == std::string::npos)
		{
			break;
		}
	}
	return pos;
}

void TextList::add(std::string_view items)
{
	if(!items.empty())
	{
		if(!list.empty())
		{
			list.append(1, delimiter);
		}
		list.append(items);
	}
}
void TextList::remove(size_t index) noexcept
{
	const size_t delimPos = find_nth(list, index, delimiter);
	if(delimPos == std::string::npos)
	{
		return;
	}
	const size_t itemPos = delimPos + (index == 0 ? 0 : 1);
	const size_t itemEnd = list.find(delimiter, itemPos);
	const size_t itemLen = (itemEnd == std::string::npos ? list.size() - itemPos : itemEnd - itemPos) + 1;
	list.erase(list[delimPos] == delimiter ? delimPos : itemPos, itemLen);
}
std::string_view TextList::get(size_t index) const noexcept
{
	const size_t delimPos = find_nth(list, index, delimiter);
	if(delimPos == std::string::npos)
	{
		return {};
	}
	const size_t itemPos = delimPos + (index == 0 ? 0 : 1);
	const size_t itemEnd = list.find(delimiter, itemPos);
	const size_t itemLen = itemEnd == std::string::npos ? list.size() - itemPos : itemEnd - itemPos;
	return {&list[itemPos], itemLen};
}
std::string_view TextList::get() const noexcept
{
	return list;
}
TextList::operator const char*() const noexcept
{
	return list.c_str();
}
size_t TextList::size() const noexcept
{
	if(list.empty())
	{
		return 0;
	}
	return std::count(list.begin(), list.end(), delimiter) + 1;
}

std::string splitCamel(std::string_view text)
{
	if(text.empty())
	{
		return {};
	}

	struct CharType
	{
		explicit CharType(char chr) noexcept :
		    isUpper{std::isupper(chr) != 0}, isLower{std::islower(chr) != 0}, isDigit{std::isdigit(chr) != 0}
		{
		}
		bool isUpper;
		bool isLower;
		bool isDigit;
	};

	std::string split;
	CharType lastType(text.front());
	split.reserve(text.size());
	split.append(1, text.front());
	text.remove_prefix(1);
	for(const char chr : text)
	{
		const CharType type(chr);
		if((lastType.isLower && (type.isUpper || type.isDigit)) || (!type.isDigit && lastType.isDigit))
		{
			split.append(1, ' ');
		}
		split.append(1, chr);
		lastType = type;
	}
	return split;
}
} // namespace raymino
