#pragma once

#include "gameplay.hpp"
#include "grid.hpp"
#include "types.hpp"

#include <ostream>

namespace raymino
{
std::ostream& operator<<(std::ostream& os, const XY& value);
std::ostream& operator<<(std::ostream& os, const Size& value);
std::ostream& operator<<(std::ostream& os, const Rect& value);
std::ostream& operator<<(std::ostream& os, const Offset& value);
std::ostream& operator<<(std::ostream& os, const Grid& value);
std::ostream& operator<<(std::ostream& os, const TSpinCornerCountResult& value);
} // namespace raymino
