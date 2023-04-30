#pragma once

#include "types.hpp"

#include <ostream>

namespace raymino
{
std::ostream& operator<<(std::ostream& os, const XY& value);
std::ostream& operator<<(std::ostream& os, const Size& value);
std::ostream& operator<<(std::ostream& os, const Rect& value);
} // namespace raymino
