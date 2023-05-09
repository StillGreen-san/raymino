#include "ostream.hpp"

namespace raymino
{
std::ostream& operator<<(std::ostream& os, const XY& value)
{
	os << '{' << value.x << ", " << value.y << '}';
	return os;
}
std::ostream& operator<<(std::ostream& os, const Size& value)
{
	os << '{' << value.width << ", " << value.height << '}';
	return os;
}
std::ostream& operator<<(std::ostream& os, const Rect& value)
{
	os << '{' << static_cast<const XY&>(value) << ", " << static_cast<const Size&>(value) << '}';
	return os;
}
std::ostream& operator<<(std::ostream& os, const Offset& value)
{
	os << '{' << value.position << ", " << value.rotation << '}';
	return os;
}
std::ostream& operator<<(std::ostream& os, const Grid& value)
{
	os << '{';

	auto cellIt = value.begin();
	const auto cellEnd = value.end();

	for(; cellIt != cellEnd;)
	{
		os << *cellIt;
		++cellIt;
		if(cellIt != cellEnd)
		{
			os << ", ";
		}
	}

	os << '}';
	return os;
}
} // namespace raymino
