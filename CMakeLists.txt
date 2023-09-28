cmake_minimum_required(VERSION 3.25)

project(raymino C CXX)

if (EMSCRIPTEN)
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -sNO_DISABLE_EXCEPTION_CATCHING")
	set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -sUSE_GLFW=3 -sSTACK_SIZE=2MB")
	set(CMAKE_EXECUTABLE_SUFFIX ".html")
endif ()

add_subdirectory(ext SYSTEM)

option(WARNINGS_AS_ERRORS "Treat compiler warnings as errors" OFF)
option(ENABLE_CPPCHECK "Enable static analysis with cppcheck" OFF)
option(ENABLE_CLANG_TIDY "Enable static analysis with clang-tidy" OFF)
option(ENABLE_INCLUDE_WHAT_YOU_USE "Enable static analysis with include-what-you-use" OFF)

include(${CMAKE_CURRENT_SOURCE_DIR}/cmake/CompilerWarnings.cmake)
include(${CMAKE_CURRENT_SOURCE_DIR}/cmake/ProjectSettings.cmake)
include(${CMAKE_CURRENT_SOURCE_DIR}/cmake/StaticAnalyzers.cmake)

add_library(${PROJECT_NAME}-lib src/gameplay.cpp src/grid.cpp src/input.cpp src/app-types.cpp src/savefile.cpp src/gui.cpp)
target_include_directories(${PROJECT_NAME}-lib PUBLIC inc)
target_compile_features(${PROJECT_NAME}-lib PUBLIC cxx_std_17)
target_link_libraries(${PROJECT_NAME}-lib PUBLIC raylib::lib raylib::cpp raylib::gui raylib::res)

add_executable(${PROJECT_NAME} WIN32 src/main.cpp src/graphics.cpp src/app.cpp src/loading.cpp
		src/menu.cpp src/game.cpp)
if (WIN32)
	target_sources(${PROJECT_NAME} PRIVATE src/windows.cpp)
endif ()
target_link_libraries(${PROJECT_NAME} PRIVATE ${PROJECT_NAME}-lib magic_enum::magic_enum)

enable_testing()
include(Catch)

add_executable(${PROJECT_NAME}-test test/grid.cpp test/basicRotation.cpp test/gameplay.cpp
		test/app-types.cpp src/ostream.cpp test/savefile.cpp test/gui.cpp)
target_link_libraries(${PROJECT_NAME}-test PRIVATE Catch2::Catch2WithMain ${PROJECT_NAME}-lib)
if (NOT EMSCRIPTEN)
	catch_discover_tests(${PROJECT_NAME}-test)
endif ()
