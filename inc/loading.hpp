#pragma once

#include "app.hpp"
#include "scenes.hpp"

namespace raymino
{
struct Loading : public IScene
{
	Loading(App& app);

	void UpdateDraw(App& app) override;
};
} // namespace raymino
