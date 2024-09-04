#include "window.h"

MonitorList *primary_monitor = nullptr;

BOOL CALLBACK MonitorEnumProcToInitList(HMONITOR hMonitor, HDC hdcMonitor, LPRECT lprcMonitor, LPARAM dwData) {
	MonitorList** head = reinterpret_cast<MonitorList**>(dwData);

	MONITORINFO mi = {};
	mi.cbSize = sizeof(mi);
	if (GetMonitorInfo(hMonitor, &mi)) {
		MonitorList* new_placement = new MonitorList(hMonitor);

		// if there is no monitor in the list, add the new monitor to the list
		if (*head == nullptr) {
			*head = new_placement;
			return TRUE;
		}

		if (mi.dwFlags & MONITORINFOF_PRIMARY) {
				new_placement->SetNextMonitor(*head);
				*head = new_placement;
		}
		else {
			MonitorList* current = *head;
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

bool InitializeMonitorList() {
	if (!EnumDisplayMonitors(NULL, NULL, MonitorEnumProcToInitList, reinterpret_cast<LPARAM>(&primary_monitor))) {
		return false;
	}
	return true;
}

