CPMAddPackage(
		NAME raylib
		GITHUB_REPOSITORY raysan5/raylib
		GIT_TAG 5.5
		SYSTEM YES
		GIT_SHALLOW YES
)
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

CPMAddPackage(
		NAME magic_enum
		GITHUB_REPOSITORY Neargye/magic_enum
		VERSION 0.9.7
		SYSTEM YES
		GIT_SHALLOW YES
)

CPMAddPackage(
		NAME Catch2
		GITHUB_REPOSITORY catchorg/Catch2
		VERSION 3.8.0
		SYSTEM YES
		GIT_SHALLOW YES
)
target_compile_features(Catch2 PUBLIC cxx_std_17)
list(APPEND CMAKE_MODULE_PATH "${Catch2_SOURCE_DIR}/extras")
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} PARENT_SCOPE)
