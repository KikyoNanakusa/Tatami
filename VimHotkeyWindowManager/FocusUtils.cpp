#include "FocusUtils.h"

BOOL SetForegroundWhenRestored(Window* window) {
	if (window == nullptr) {
		return FALSE;
	}

	if (window->isMinimized || window->isMaximized) {
		return FALSE;
	}

	return SetForegroundWindow(window->hWnd);
}

// Set the cursor to the left side of the monitor
BOOL SetCursorToLeft(Monitor *monitor) {
	int x = (monitor->mi.rcMonitor.right - monitor->mi.rcMonitor.left) / 4;
	int y = monitor->mi.rcMonitor.top + (monitor->mi.rcMonitor.bottom - monitor->mi.rcMonitor.top) / 2;

	return SetCursorPos(x, y);
}

BOOL SetCursorToRight(Monitor* monitor) {
	int x = (monitor->mi.rcMonitor.right - monitor->mi.rcMonitor.left) * 3 / 4;
	int y = monitor->mi.rcMonitor.top + (monitor->mi.rcMonitor.bottom - monitor->mi.rcMonitor.top) / 2;

	return SetCursorPos(x, y);
}

BOOL SetCursorToTop(Monitor* monitor) {
	int x = monitor->mi.rcMonitor.left + (monitor->mi.rcMonitor.right - monitor->mi.rcMonitor.left) / 2;
	int y = monitor->mi.rcMonitor.top + (monitor->mi.rcMonitor.bottom - monitor->mi.rcMonitor.top) / 4;

	return SetCursorPos(x, y);
}

BOOL SetCursorToBottom(Monitor* monitor) {
	int x = monitor->mi.rcMonitor.left + (monitor->mi.rcMonitor.right - monitor->mi.rcMonitor.left) / 2;
	int y = monitor->mi.rcMonitor.top + (monitor->mi.rcMonitor.bottom - monitor->mi.rcMonitor.top) *3 / 4;

	return SetCursorPos(x, y);
}

BOOL SetCursorToTopLeft(Monitor* monitor) {
	int x = monitor->mi.rcMonitor.left + (monitor->mi.rcMonitor.right - monitor->mi.rcMonitor.left) / 4;
	int y = monitor->mi.rcMonitor.top + (monitor->mi.rcMonitor.bottom - monitor->mi.rcMonitor.top) / 4;

	return SetCursorPos(x, y);
}

BOOL SetCursorToTopRight(Monitor* monitor) {
	int x = (monitor->mi.rcMonitor.right - monitor->mi.rcMonitor.left) * 3 / 4;
	int y = monitor->mi.rcMonitor.top + (monitor->mi.rcMonitor.bottom - monitor->mi.rcMonitor.top) / 4;

	return SetCursorPos(x, y);
}

BOOL SetCursorToBottomLeft(Monitor* monitor) {
	int x = monitor->mi.rcMonitor.left + (monitor->mi.rcMonitor.right - monitor->mi.rcMonitor.left) / 4;
	int y = monitor->mi.rcMonitor.top + (monitor->mi.rcMonitor.bottom - monitor->mi.rcMonitor.top) * 3 / 4;

	return SetCursorPos(x, y);
}

BOOL SetCursorToBottomRight(Monitor* monitor) {
	int x = (monitor->mi.rcMonitor.right - monitor->mi.rcMonitor.left) * 3 / 4;
	int y = monitor->mi.rcMonitor.top + (monitor->mi.rcMonitor.bottom - monitor->mi.rcMonitor.top) * 3 / 4;

	return SetCursorPos(x, y);
}