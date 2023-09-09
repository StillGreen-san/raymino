#pragma once

#include "gameplay.hpp"
#include "grid.hpp"
#include "types.hpp"

#include <ostream>

namespace raymino
{
std::ostream& operator<<(std::ostream& ostream, const XY& value);
std::ostream& operator<<(std::ostream& ostream, const Size& value);
std::ostream& operator<<(std::ostream& ostream, const Rect& value);
std::ostream& operator<<(std::ostream& ostream, const Offset& value);
std::ostream& operator<<(std::ostream& ostream, const Grid& value);
std::ostream& operator<<(std::ostream& ostream, const TSpinCornerCountResult& value);
std::ostream& operator<<(std::ostream& ostream, const LevelState& value);
} // namespace raymino
