#include "app.hpp"
#include "dependency_info.hpp"

#include <iomanip>
#include <iostream>

#ifdef WIN32
int WinMain()
#else
int main()
#endif
{
	std::cout << PROJECT_INFO.url << " [" << PROJECT_INFO.lic << ']' << std::endl;
	std::cout << "  build with:" << std::endl;
	for(const LibraryInfo& depInfo : DEPENDENCY_INFOS)
	{
		std::cout << depInfo.url;
		std::cout << std::setw(static_cast<std::streamsize>(DEPENDENCY_INFOS_MAX_URL_LEN - depInfo.url.size()) + 2);
		std::cout << '[' << depInfo.lic << ']' << std::endl;
	}

	raymino::App app;

	app.Run();

	return 0;
}
