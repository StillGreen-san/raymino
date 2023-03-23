#pragma once

#include "app.hpp"

#include <raygui.h>
#include <raylib-cpp.hpp>
#include <rres-raylib.h>

struct Game : raymino::IScene
{
	Game()
	{
	}
	void UpdateDraw(raymino::App& app) override
	{
		::BeginDrawing();

		::ClearBackground(RColor::LightGray());

		::EndDrawing();
	}
};

template<>
std::unique_ptr<raymino::IScene> raymino::MakeScene<raymino::Scene::Game>()
{
	return std::make_unique<Game>();
}
