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

	Monitor(HMONITOR monitor) : hMonitor(monitor) {
		this->mi.cbSize = sizeof(MONITORINFO); 	

		// TODO: Implement error handling
		GetMonitorInfo(hMonitor, &mi);
		isPrimary = mi.dwFlags & MONITORINFOF_PRIMARY;
	}

	void SetNextMonitor(Monitor *next) {
		next_monitor = next;
		if(next) next->prev_monitor = this;
	}

	void SetPrevMonitor(Monitor *prev) {
		prev_monitor = prev;
		prev->next_monitor = this;
	}

	void UnmapWindow(Window* window) {
		if (window == left_window) {
			left_window = nullptr;
		}
		else if (window == right_window) {
			right_window = nullptr;
		}
		else if (window == top_window) {
			top_window = nullptr;
		}
		else if (window == bottom_window) {
			bottom_window = nullptr;
		}
		else if (window == top_left_window) {
			top_left_window = nullptr;
		}
		else if (window == top_right_window) {
			top_right_window = nullptr;
		}
		else if (window == bottom_left_window) {
			bottom_left_window = nullptr;
		}
		else if (window == bottom_right_window) {
			bottom_right_window = nullptr;
		}
	}
};

extern Monitor* primary_monitor;

BOOL CALLBACK MonitorEnumProcToInitList(HMONITOR hMonitor, HDC hdcMonitor, LPRECT lprcMonitor, LPARAM dwData);
bool InitializeMonitorList();
Monitor *FindMonitorByHmonitor(HMONITOR hMonitor);
