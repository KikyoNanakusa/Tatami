#include "Monitor.h"

// Global List of Monitors
Monitor *primary_monitor = nullptr;


// Constructor of the Monitor struct
Monitor::Monitor(HMONITOR monitor) : hMonitor(monitor) {
	this->mi.cbSize = sizeof(MONITORINFO);

	// TODO: Implement error handling
	GetMonitorInfo(hMonitor, &mi);
	isPrimary = mi.dwFlags & MONITORINFOF_PRIMARY;
}

// Set the next monitor of the monitor
// Prev monitor of the next monitor is set to the monitor
void Monitor::SetNextMonitor(Monitor* next) {
	next_monitor = next;
	if (next) next->prev_monitor = this;
}

// Set the previous monitor of the monitor
// Next monitor of the previous monitor is set to the monitor
void Monitor::SetPrevMonitor(Monitor* prev) {
	prev_monitor = prev;
	prev->next_monitor = this;
}

// Remove all window(s) pointer from the monitor struct
void Monitor::UnmapWindow(Window* window) {
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


// This is a callback function for EnumDisplayMonitors
// Enumerate all the monitors and add them to the global list
// If the monitor is the primary monitor, add it to the head of the list
// If the monitors are enumerated successfully, return true
BOOL CALLBACK MonitorEnumProcToInitList(HMONITOR hMonitor, HDC hdcMonitor, LPRECT lprcMonitor, LPARAM dwData) {
	Monitor** head = reinterpret_cast<Monitor**>(dwData);

	Monitor* new_placement = new Monitor(hMonitor);

	if (*head == nullptr) {
		*head = new_placement;
		return TRUE;
	}

	if (new_placement->isPrimary) {
			new_placement->SetNextMonitor(*head);
			new_placement->isPrimary = true;
			*head = new_placement;
	}
	else {
		Monitor* current = *head;
		if (!current) {
			*head = new_placement;
		}
		else {
			while (current->next_monitor) {
				current = current->next_monitor;
			}
			current->SetNextMonitor(new_placement);
		}
	}

	return TRUE;
}

// Initialize the global list of monitors
// Enumerate all the monitors and add them to the list by the callback function
// If the monitors are enumerated successfully, return true
// Otherwise, return false
bool InitializeMonitorList() {
	if (!EnumDisplayMonitors(NULL, NULL, MonitorEnumProcToInitList, reinterpret_cast<LPARAM>(&primary_monitor))) {
		return false;
	}

	// Set the previous monitor of the primary monitor to the last monitor
	Monitor *last_monitor = primary_monitor;
	while (last_monitor->next_monitor) {
		last_monitor = last_monitor->next_monitor;
	}

	primary_monitor->SetPrevMonitor(last_monitor);

	return true;
}

// Find the monitor in the global list by the HMONITOR
// If the monitor is found, return the pointer to the monitor
// Otherwise, return nullptr
Monitor *FindMonitorByHmonitor(HMONITOR hMonitor) {
	Monitor* current = primary_monitor;
	while (current) {
		if (current->hMonitor == hMonitor) {
			return current;
		}
		current = current->next_monitor;
	}

	return nullptr;
}
