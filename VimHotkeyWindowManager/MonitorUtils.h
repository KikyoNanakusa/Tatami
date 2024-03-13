#pragma once
#include <Windows.h>
#include <vector>

struct MonitorInfo {
	HMONITOR hMonitor;
	RECT rect;
};

BOOL CALLBACK MonitorEnumProc(HMONITOR hMonitor, HDC hdcMonitor, LPRECT lprcMonitor, LPARAM dwData);
HMONITOR GetNextMonitor(HWND hWnd);
