#pragma once

#include "gameplay.hpp"
#include "types.hpp"

#include <ostream>

namespace raymino
{
std::ostream& operator<<(std::ostream& os, const XY& value);
std::ostream& operator<<(std::ostream& os, const Size& value);
std::ostream& operator<<(std::ostream& os, const Rect& value);
std::ostream& operator<<(std::ostream& os, const Offset& value);
} // namespace raymino
