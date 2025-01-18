#include "windows.hpp"

#include <cstddef>

#define UNICODE
#include <Windows.h>

// NOLINTBEGIN(*-pointer-arithmetic)
WinMainArgs::WinMainArgs()
{
	LPWSTR* wArgv = CommandLineToArgvW(GetCommandLineW(), &mArgc);
	if(!wArgv)
	{
		return;
	}

	mArgv = new char*[mArgc + 1];
	mArgv[mArgc] = nullptr;
	for(size_t argIdx = 0; argIdx < mArgc; ++argIdx)
	{
		const int outLen = WideCharToMultiByte(CP_UTF8, 0, wArgv[argIdx], -1, nullptr, 0, nullptr, nullptr);
		if(outLen == 0)
		{
			mArgv[argIdx] = nullptr;
			continue;
		}
		mArgv[argIdx] = new char[outLen];
		if(0 == WideCharToMultiByte(CP_UTF8, 0, wArgv[argIdx], -1, mArgv[argIdx], outLen, nullptr, nullptr))
		{
			mArgv[argIdx][0] = '\0';
		}
	}
	LocalFree(wArgv); // NOLINT(*-multi-level-implicit-pointer-conversion)
}

WinMainArgs::~WinMainArgs()
{
	for(int i = 0; i < mArgc; i++)
	{
		delete[] mArgv[i];
	}
	delete[] mArgv;
}
// NOLINTEND(*-pointer-arithmetic)
