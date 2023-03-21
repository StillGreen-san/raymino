add_subdirectory(raylib)
add_library(raylib::lib ALIAS raylib)

add_subdirectory(raylib-cpp)
add_library(raylib::cpp ALIAS raylib_cpp)
target_link_libraries(raylib_cpp INTERFACE raylib)

include(raygui.cmake)
add_library(raylib::gui ALIAS raygui)

include(rres.cmake)
add_library(raylib::res ALIAS rres)
