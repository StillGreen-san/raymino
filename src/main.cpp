#include "app.hpp"

int main()
{
	raymino::App app = raymino::App::Make<raymino::Scene::Game>();

	app.Run();

	return 0;
}
