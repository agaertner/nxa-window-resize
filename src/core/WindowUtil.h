#ifndef WINDOW_UTIL_H
#define WINDOW_UTIL_H

#include <Windows.h>
#include <string>

namespace Nekres::WindowUtil
{
	struct WindowSize {
		int Width;
		int Height;
	};

	extern const WindowSize WindowSizes[];
	extern const int WindowSizesCount;

	WindowSize GetSmallestResolution();
	WindowSize GetDefaultResolution();
	std::string GetLabel(int width, int height);

	HWND GetWindowHandle();

	bool IsWindowedMode(HWND hWnd);
	void ResizeAndCenterWindow(HWND hWnd, int width, int height);
}

#endif
