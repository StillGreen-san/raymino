add_subdirectory(raylib)
add_library(raylib_bundle::raylib ALIAS raylib)

add_subdirectory(raylib-cpp)
add_library(raylib_bundle::raylib-cpp ALIAS raylib_cpp)
target_link_libraries(raylib_cpp INTERFACE raylib)

include(raygui.cmake)
add_library(raylib_bundle::raygui ALIAS raygui)

include(rres.cmake)
add_library(raylib_bundle::rres ALIAS rres)
