cmake_minimum_required(VERSION 3.20)

project(raygui C)

add_library(raygui)
set_target_properties(raygui PROPERTIES LINKER_LANGUAGE C)
target_sources(raygui PRIVATE raygui/src/raygui.h)
target_compile_definitions(raygui PRIVATE RAYGUI_IMPLEMENTATION)
target_include_directories(raygui PUBLIC raygui/src)
target_link_libraries(raygui PUBLIC raylib)
