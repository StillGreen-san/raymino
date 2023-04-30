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
} // namespace raymino
