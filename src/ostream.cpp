#include "ostream.hpp"

namespace raymino
{
std::ostream& operator<<(std::ostream& ostream, const XY& value)
{
	ostream << '{' << value.x << ", " << value.y << '}';
	return ostream;
}
std::ostream& operator<<(std::ostream& ostream, const Size& value)
{
	ostream << '{' << value.width << ", " << value.height << '}';
	return ostream;
}
std::ostream& operator<<(std::ostream& ostream, const Rect& value)
{
	ostream << '{' << static_cast<const XY&>(value) << ", " << static_cast<const Size&>(value) << '}';
	return ostream;
}
std::ostream& operator<<(std::ostream& ostream, const Offset& value)
{
	ostream << '{' << value.position << ", " << value.rotation << '}';
	return ostream;
}
std::ostream& operator<<(std::ostream& ostream, const Grid& value)
{
	ostream << '{';

	auto cellIt = value.begin();
	const auto cellEnd = value.end();

	for(; cellIt != cellEnd;)
	{
		ostream << *cellIt;
		++cellIt;
		if(cellIt != cellEnd)
		{
			ostream << ", ";
		}
	}

	ostream << '}';
	return ostream;
}
std::ostream& operator<<(std::ostream& ostream, const TSpinCornerCountResult& value)
{
	ostream << '{' << value.front << ", " << value.back << '}';
	return ostream;
}
std::ostream& operator<<(std::ostream& ostream, const LevelState& value)
{
	ostream << '{' << value.currentLevel << ", " << value.linesCleared << ", " << value.linesToClear << '}';
	return ostream;
}
} // namespace raymino
