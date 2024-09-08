#pragma once
#include <Windows.h>

struct Monitor {
	HMONITOR hMonitor;
	MONITORINFO mi;

	Monitor* next_monitor = nullptr;
	Monitor* prev_monitor = nullptr;
	bool isPrimary = false;
	int x_division_cnt = 0;
	int y_division_cnt = 0;

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
};

extern Monitor* primary_monitor;

BOOL CALLBACK MonitorEnumProcToInitList(HMONITOR hMonitor, HDC hdcMonitor, LPRECT lprcMonitor, LPARAM dwData);
bool InitializeMonitorList();
Monitor *FindMonitorByHmonitor(HMONITOR hMonitor);
