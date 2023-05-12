cmake_minimum_required(VERSION 3.20)

project(raymino C CXX)

if (EMSCRIPTEN)
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -s USE_GLFW=3 -s ASSERTIONS=1 -s WASM=1")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -s USE_GLFW=3 -s ASSERTIONS=1 -s WASM=1")
    set(CMAKE_EXECUTABLE_SUFFIX ".html")
endif ()

add_subdirectory(ext)

option(WARNINGS_AS_ERRORS "Treat compiler warnings as errors" OFF)
option(ENABLE_CPPCHECK "Enable static analysis with cppcheck" OFF)
option(ENABLE_CLANG_TIDY "Enable static analysis with clang-tidy" OFF)
option(ENABLE_INCLUDE_WHAT_YOU_USE "Enable static analysis with include-what-you-use" OFF)

include(${CMAKE_CURRENT_SOURCE_DIR}/cmake/CompilerWarnings.cmake)
include(${CMAKE_CURRENT_SOURCE_DIR}/cmake/ProjectSettings.cmake)
include(${CMAKE_CURRENT_SOURCE_DIR}/cmake/StaticAnalyzers.cmake)

add_library(${PROJECT_NAME}-lib src/gameplay.cpp src/grid.cpp src/playfield.cpp src/input.cpp)
target_include_directories(${PROJECT_NAME}-lib PUBLIC inc)
target_compile_features(${PROJECT_NAME}-lib PUBLIC cxx_std_17)
target_link_libraries(${PROJECT_NAME}-lib PUBLIC raylib::lib raylib::cpp raylib::gui raylib::res)

add_executable(${PROJECT_NAME} src/main.cpp src/app.cpp src/game.cpp src/menu.cpp)
target_link_libraries(${PROJECT_NAME} PRIVATE ${PROJECT_NAME}-lib)

enable_testing()
include(Catch)

add_executable(${PROJECT_NAME}-test test/grid.cpp test/basicRotation.cpp test/gameplay.cpp src/ostream.cpp)
target_link_libraries(${PROJECT_NAME}-test PRIVATE Catch2::Catch2WithMain ${PROJECT_NAME}-lib)
catch_discover_tests(${PROJECT_NAME}-test)
