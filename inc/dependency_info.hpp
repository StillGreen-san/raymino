#pragma once

#include "cstring_view.hpp"

#include <algorithm>
#include <array>
#include <cstddef>

namespace raymino
{
struct LibraryInfo
{
	CStringView url;
	CStringView lic;
};
constexpr LibraryInfo PROJECT_INFO{"https://github.com/StillGreen-san/raymino"_csv, "MIT"_csv};
constexpr std::array<LibraryInfo, 7> DEPENDENCY_INFOS{{
    {"https://github.com/raysan5/raylib"_csv, "Zlib"_csv},
    {"https://github.com/RobLoach/raylib-cpp"_csv, "Zlib"_csv},
    {"https://github.com/raysan5/raygui"_csv, "Zlib"_csv},
    {"https://github.com/raysan5/rres"_csv, "MIT"_csv},
    {"https://github.com/Neargye/magic_enum"_csv, "MIT"_csv},
    {"https://github.com/catchorg/Catch2"_csv, "BSL"_csv},
    {"https://github.com/cpm-cmake"_csv, "MIT"_csv},
}};
constexpr size_t DEPENDENCY_INFOS_MAX_URL_LEN = std::max_element(DEPENDENCY_INFOS.begin(), DEPENDENCY_INFOS.end(),
    [](const auto& lhs, const auto& rhs)
    {
	    return lhs.url.size() < rhs.url.size();
    })->url.size();
} // namespace raymino
