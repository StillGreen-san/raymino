#include "windows.hpp"

#define UNICODE
#include <Windows.h>

WinMainArgs::WinMainArgs()
{
	LPWSTR wCmdLine = GetCommandLineW();
	int wArgc = 0;
	LPWSTR* wArgv = CommandLineToArgvW(wCmdLine, &wArgc);
	if(!wArgv)
	{
		return;
	}

	mArgc = wArgc;
	mArgv = new char*[wArgc + 1];
	for(size_t wIdx = 0, aIdx = 0; wIdx < wArgc; ++wIdx)
	{
		const int outLen = WideCharToMultiByte(CP_ACP, 0, wArgv[wIdx], -1, nullptr, 0, nullptr, nullptr);
		if(outLen == 0)
		{
			mArgc -= 1;
			continue;
		}
		mArgv[aIdx] = new char[outLen];
		const int result = WideCharToMultiByte(CP_ACP, 0, wArgv[wIdx], -1, mArgv[aIdx], outLen, nullptr, nullptr);
		if(result == 0)
		{
			delete[] mArgv[aIdx];
			mArgc -= 1;
			continue;
		}
		++aIdx;
	}
	LocalFree(wArgv);
}
WinMainArgs::~WinMainArgs()
{
	for(int i = 0; i < mArgc; i++)
	{
		delete[] mArgv[i];
	}
	delete[] mArgv;
}
