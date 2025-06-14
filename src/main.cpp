#include "app.hpp"
#include "cstring_view.hpp"
#include "dependency_info.hpp"
#include "ostream.hpp"

#include <cstring>
#include <iomanip>
#include <iostream>
#include <iterator>

using namespace raymino; // NOLINT(google-build-using-namespace)

int main(int argc, const char* const argv[])
{
	if(argc == 2 && std::strcmp(*std::next(argv), "SGS_EXIT") == 0)
	{
		return 0;
	}

	std::cout << PROJECT_INFO.url << " [" << PROJECT_INFO.lic << "]\n";
	std::cout << "  build with:\n";
	for(const LibraryInfo& depInfo : DEPENDENCY_INFOS)
	{
		std::cout << depInfo.url;
		std::cout << std::setw(static_cast<int>(DEPENDENCY_INFOS_MAX_URL_LEN - depInfo.url.size()) + 2);
		std::cout << '[' << depInfo.lic << "]\n";
	}

	App app;

	app.Run();

	return 0;
}

#ifdef WIN32
#include "windows.hpp"
int WinMain()
{
	const WinMainArgs mainArgs;
	return main(mainArgs.argc(), mainArgs.argv());
}
#endif
