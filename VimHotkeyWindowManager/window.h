#pragma once
#include <Windows.h>

typedef enum horizontal_alignment horizontal_alignment;
enum horizontal_alignment {
	HORIZ_LEFT,
	HORIZ_CENTER,
	HORIZ_RIGHT,
	HORIZ_FILL
};

typedef enum vertical_alignment vertical_alignment;
enum vertical_alignment {
	VERT_TOP,
	VERT_CENTER,
	VERT_BOTTOM,
	VERT_FILL
};

typedef struct Window Window;
struct Window {
	horizontal_alignment halign;
	vertical_alignment valign;
	HWND hwnd;

	// belows are for future use
	Window* left_window;
	Window* right_window;
	Window* top_window;
	Window* bottom_window;

};

typedef struct MonitorList WindowPlacement;
struct MonitorList {
	Window left{};
	Window right{};
	Window top{};
	Window bottom{};
	HMONITOR monitor;
	MonitorList* next_monitor = nullptr;
	MonitorList* prev_monitor = nullptr;

	MonitorList(HMONITOR monitor) : monitor(monitor) {}

	void SetNextMonitor(MonitorList *next) {
		next_monitor = next;
		if(next) next->prev_monitor = this;
	}

	void SetPrevMonitor(MonitorList *prev) {
		prev_monitor = prev;
		prev->next_monitor = this;
	}
};

extern MonitorList* primary_monitor;
BOOL CALLBACK MonitorEnumProcToInitList(HMONITOR hMonitor, HDC hdcMonitor, LPRECT lprcMonitor, LPARAM dwData);
bool InitializeMonitorList();
