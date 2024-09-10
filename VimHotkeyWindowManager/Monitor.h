#pragma once
#include <Windows.h>

struct Window;

struct Monitor {
	HMONITOR hMonitor;
	MONITORINFO mi;

	Monitor* next_monitor = nullptr;
	Monitor* prev_monitor = nullptr;
	bool isPrimary = false;

	Window *left_window = nullptr;
	Window *right_window = nullptr;
	Window *top_window = nullptr;
	Window *bottom_window = nullptr;

	Window *top_left_window = nullptr;
	Window *top_right_window = nullptr;
	Window *bottom_left_window = nullptr;
	Window *bottom_right_window = nullptr;

	Monitor(HMONITOR monitor);
	void SetNextMonitor(Monitor* next);
	void SetPrevMonitor(Monitor* prev);
	void UnmapWindow(Window* window);
};

// Used in global scope
extern Monitor* primary_monitor;

BOOL CALLBACK MonitorEnumProcToInitList(HMONITOR hMonitor, HDC hdcMonitor, LPRECT lprcMonitor, LPARAM dwData);
bool InitializeMonitorList();
Monitor *FindMonitorByHmonitor(HMONITOR hMonitor);
