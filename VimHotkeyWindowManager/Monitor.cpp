#include "Monitor.h"

// Global List of Monitors
Monitor *primary_monitor = nullptr;

// This is a callback function for EnumDisplayMonitors
// Enumerate all the monitors and add them to the global list
// If the monitor is the primary monitor, add it to the head of the list
// If the monitors are enumerated successfully, return true
BOOL CALLBACK MonitorEnumProcToInitList(HMONITOR hMonitor, HDC hdcMonitor, LPRECT lprcMonitor, LPARAM dwData) {
	Monitor** head = reinterpret_cast<Monitor**>(dwData);

	MONITORINFO mi = {};
	mi.cbSize = sizeof(mi);
	if (GetMonitorInfo(hMonitor, &mi)) {
		Monitor* new_placement = new Monitor(hMonitor);

		// if there is no monitor in the list, add the new monitor to the list
		if (*head == nullptr) {
			*head = new_placement;
			return TRUE;
		}

		if (mi.dwFlags & MONITORINFOF_PRIMARY) {
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
	Monitor* last_monitor = primary_monitor;
	while (last_monitor->next_monitor) {
		last_monitor = last_monitor->next_monitor;
	}

	if (last_monitor != primary_monitor) {
		primary_monitor->SetPrevMonitor(last_monitor);
	}

	return true;
}
