add_subdirectory(raylib)
add_library(raylib_bundle::raylib ALIAS raylib)

add_subdirectory(raylib-cpp)
add_library(raylib_bundle::raylib-cpp ALIAS raylib_cpp)
