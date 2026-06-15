#include "WindowUtil.h"

namespace Nekres::WindowUtil
{
	const WindowSize WindowSizes[] = {
		{1024, 768, "1024x768"},
		{1152, 864, "1152x864"},
		{1176, 664, "1176x664"},
		{1280, 720, "1280x720"},
		{1280, 768, "1280x768"},
		{1280, 800, "1280x800"},
		{1280, 960, "1280x960"},
		{1280, 1024, "1280x1024"},
		{1360, 768, "1360x768"},
		{1366, 768, "1366x768"},
		{1440, 1080, "1440x1080"},
		{1600, 900, "1600x900"},
		{1600, 1024, "1600x1024"},
		{1600, 1200, "1600x1200"},
		{1680, 1050, "1680x1050"},
		{1720, 1440, "1720x1440"},
		{1920, 1080, "1920x1080"},
		{1920, 1200, "1920x1200"},
		{1920, 1440, "1920x1440"},
		{2048, 1152, "2048x1152"},
		{2560, 1440, "2560x1440"},
		{3440, 1440, "3440x1440"}
	};

	const int WindowSizesCount = sizeof(WindowSizes) / sizeof(WindowSize);

	bool IsWindowedMode(HWND hWnd)
	{
		if (!hWnd) return false;
		LONG_PTR style = GetWindowLongPtr(hWnd, GWL_STYLE);
		return (style & WS_POPUP) == 0 && (style & WS_BORDER) != 0;
	}

	void ResizeAndCenterWindow(HWND hWnd, int width, int height)
	{
		if (!hWnd) return;
		HMONITOR hMonitor = MonitorFromWindow(hWnd, MONITOR_DEFAULTTONEAREST);
		MONITORINFO mi = { sizeof(mi) };
		if (GetMonitorInfo(hMonitor, &mi))
		{
			int monitorWidth = mi.rcMonitor.right - mi.rcMonitor.left;
			int monitorHeight = mi.rcMonitor.bottom - mi.rcMonitor.top;
			int x = mi.rcMonitor.left + (monitorWidth - width) / 2;
			int y = mi.rcMonitor.top + (monitorHeight - height) / 2;
			SetWindowPos(hWnd, nullptr, x, y, width, height, SWP_NOZORDER | SWP_FRAMECHANGED | SWP_ASYNCWINDOWPOS);
		}
	}
}
