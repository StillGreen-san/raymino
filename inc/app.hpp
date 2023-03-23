#pragma once

#include "scenes.hpp"

#include <memory>

namespace raymino
{
class App
{
public:
	template<Scene TScene>
	static App Make()
	{
		App app;
		app.scene = MakeScene<TScene>();
		return app;
	}

	void Run();

	std::unique_ptr<IScene> scene;
private:
	App();
};
} // namespace raymino
