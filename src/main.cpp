#include "app.hpp"

#ifdef WIN32
int WinMain()
#else
int main()
#endif
{
	raymino::App app;

	app.Run();

	return 0;
}
