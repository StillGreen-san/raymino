#include "app.hpp"

int main()
{
	raymino::App app = raymino::App::Make<raymino::Scene::Menu>();

	app.Run();

	return 0;
}
