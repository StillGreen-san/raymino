#include "app.hpp"

int main()
{
	raymino::App app = raymino::App::Make<raymino::Scene::Cellular>();

	app.Run();

	return 0;
}
