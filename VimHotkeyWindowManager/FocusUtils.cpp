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