CPMAddPackage(
		NAME raylib
		GITHUB_REPOSITORY raysan5/raylib
		GIT_TAG 5.5
		SYSTEM YES
		GIT_SHALLOW YES
)
add_library(raylib::lib ALIAS raylib)

CPMAddPackage(
		NAME raylib-cpp
		GITHUB_REPOSITORY RobLoach/raylib-cpp
		VERSION 5.5.0
		SYSTEM YES
		GIT_SHALLOW YES
)
add_library(raylib::cpp ALIAS raylib_cpp)
target_link_libraries(raylib_cpp INTERFACE raylib)

CPMAddPackage(
		NAME raygui
		GITHUB_REPOSITORY raysan5/raygui
		GIT_TAG 4.0
		SYSTEM YES
		GIT_SHALLOW YES
		DOWNLOAD_ONLY YES
)
add_library(raygui)
target_sources(raygui PRIVATE raygui.c)
target_include_directories(raygui PUBLIC ${raygui_SOURCE_DIR}/src)
target_link_libraries(raygui PUBLIC raylib)
add_library(raylib::gui ALIAS raygui)

CPMAddPackage(
		NAME rres
		GITHUB_REPOSITORY raysan5/rres
		GIT_TAG a5aeda67a06bfddeab97add7a05cad2aaf40e400 # 2025.01.01
		SYSTEM YES
		GIT_SHALLOW YES
		DOWNLOAD_ONLY YES
)
add_library(rres)
target_sources(rres PRIVATE rres.c)
target_include_directories(rres SYSTEM PUBLIC ${rres_SOURCE_DIR}/src)
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
