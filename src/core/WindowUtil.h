#ifndef WINDOW_UTIL_H
#define WINDOW_UTIL_H

#include <Windows.h>
#include <string>

namespace Nekres::WindowUtil
{
	struct WindowSize {
		int Width;
		int Height;
		std::string Name;
	};

	extern const WindowSize WindowSizes[];
	extern const int WindowSizesCount;

	bool IsWindowedMode(HWND hWnd);
	void ResizeAndCenterWindow(HWND hWnd, int width, int height);
}

#endif
