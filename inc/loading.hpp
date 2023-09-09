#pragma once

#include "app.hpp"
#include "scenes.hpp"

namespace raymino
{
struct Loading : public IScene
{
	/**
	 * @brief attempts to load SaveFile, witches to Menu when done, may be asynchronous
	 */
	explicit Loading(App& app);

	void UpdateDraw(App& app) override;
	void PreDestruct(App& app) override;
};
} // namespace raymino
