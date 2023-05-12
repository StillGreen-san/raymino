#pragma once

#include "scenes.hpp"

namespace raymino
{
struct Menu : public IScene
{
	Menu();
	void UpdateDraw(class App& app) override;
};
} // namespace raymino
