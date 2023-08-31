add_subdirectory(raylib)
add_library(raylib::lib ALIAS raylib)

add_subdirectory(raylib-cpp)
add_library(raylib::cpp ALIAS raylib_cpp)
target_link_libraries(raylib_cpp INTERFACE raylib)

add_library(raygui)
target_sources(raygui PRIVATE raygui.c)
target_include_directories(raygui PUBLIC raygui/src)
target_link_libraries(raygui PUBLIC raylib)
add_library(raylib::gui ALIAS raygui)

add_library(rres)
target_sources(rres PRIVATE rres.c)
target_include_directories(rres PUBLIC rres/src)
target_link_libraries(rres PUBLIC raylib)
add_library(raylib::res ALIAS rres)

add_subdirectory(magic_enum)

add_subdirectory(Catch2)
target_compile_features(Catch2 PUBLIC cxx_std_17)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} PARENT_SCOPE)
