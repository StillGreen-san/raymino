#pragma once

class WinMainArgs
{
public:
	WinMainArgs();
	WinMainArgs(const WinMainArgs&) = delete;
	WinMainArgs(WinMainArgs&&) = delete;
	WinMainArgs& operator=(const WinMainArgs&) = delete;
	WinMainArgs& operator=(WinMainArgs&&) = delete;
	~WinMainArgs();
	[[nodiscard]] int argc() const noexcept
	{
		return mArgc;
	}
	[[nodiscard]] const char* const* argv() const noexcept
	{
		return mArgv;
	}

private:
	int mArgc = 0;
	char** mArgv = nullptr;
};
