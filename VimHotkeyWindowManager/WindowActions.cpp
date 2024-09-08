#include "WindowActions.h"

Window* EnsureWindowMonitorTracking(HWND hWnd = nullptr) {
    if (hWnd == nullptr) {
		// Get the window that is currently focused (foreground window
		hWnd = GetForegroundWindow();
		if (hWnd == NULL) return nullptr;
    }

    // Get the monitor information where the window is located
	MONITORINFO monitorInfo = { sizeof(monitorInfo) };
	HMONITOR hMonitor = MonitorFromWindow(hWnd, MONITOR_DEFAULTTONEAREST);
	if (!GetMonitorInfo(hMonitor, &monitorInfo)) return nullptr;

	// Check if the monitor is in the list
    // TODO: Implement error handling (suggest reinitializing the monitor list)
    Monitor *monitor = FindMonitorByHmonitor(hMonitor);
	if (monitor == nullptr) {
		return nullptr;
	}

	// Check if the window is in the list, if not, add it
	Window *window = FindWindowByHwnd(hWnd);
	if (window == nullptr) {
		AddWindowToList(hWnd, monitor);
		window = head_window;
	}

    return window;
}

bool MinimizeWindow(HWND hWnd, HWND& minimizedWindow) {
    minimizedWindow = hWnd;
    return ShowWindow(hWnd, SW_MINIMIZE);
}

bool RestoreWindow(HWND& minimizedWindow) {
    if (minimizedWindow == NULL) return NO_MINIMIZED_WINDOW;
    bool ret = ShowWindow(minimizedWindow, SW_RESTORE);
    minimizedWindow = NULL;
    return ret;
}

// ウィンドウを中央もしくは端に移動
bool AdjustWindowPosition(HWND hWnd, const MONITORINFO& mi, const RECT& windowRect, bool moveToRight) {
    int monitorWidth = mi.rcMonitor.right - mi.rcMonitor.left;
    int windowWidth = windowRect.right - windowRect.left;
    int monitorCenterX = mi.rcMonitor.left + monitorWidth / 2;
    int windowCenterX = windowRect.left + windowWidth / 2;

    int newX, newY = windowRect.top;

    if ((moveToRight && windowCenterX < monitorCenterX) || (!moveToRight && windowCenterX > monitorCenterX)) {
        // ウィンドウをモニターの中央に移動
        newX = monitorCenterX - (windowWidth / 2);
    } else {
        // ウィンドウをモニターの端に移動（左または右）
        newX = moveToRight ? mi.rcMonitor.right - windowWidth : mi.rcMonitor.left;
    }

    return SetWindowPos(hWnd, NULL, newX, newY, 0, 0, SWP_NOSIZE | SWP_NOZORDER);
}

bool MoveWindowToOtherMonitor(Window *window, const RECT& windowRect, bool isMoveWindowNextMonitor) {
    Monitor* targetMonitor = nullptr;
    if (isMoveWindowNextMonitor) {
		targetMonitor = window->monitor->next_monitor;
        if (targetMonitor == nullptr) return false;
    }
    else {
		targetMonitor = window->monitor->prev_monitor;
		if (targetMonitor == nullptr) return false;
	}

    int targetMonitorLeft = targetMonitor->mi.rcWork.left;
    
    // To absorb the difference of the Dpi of the monitors, it is necessary to convert the coordinates of the window to the target monitor
    BOOL ret = SetWindowPos(
        window->hWnd, NULL, 
        targetMonitorLeft, 
        targetMonitor->mi.rcWork.top, 
        1, 
        1, 
        SWP_NOZORDER
    );

    ret = SetWindowPos(
        window->hWnd, NULL, 
        targetMonitorLeft, 
        targetMonitor->mi.rcWork.top, 
        GetMonitorWorkWidth(targetMonitor->mi) / 2,
        GetMonitorWorkHeight(targetMonitor->mi),
        SWP_NOZORDER
    );

    if (ret) {
		window->monitor = targetMonitor;
	}
	return ret;
}

bool MoveMaximizedWindow(Window *window, bool isMoveToLeft) {
    ShowWindow(window->hWnd, SW_RESTORE);
    int monitorWidth = GetMonitorWorkWidth(window->monitor->mi);
    int monitorHeight = GetMonitorWorkHeight(window->monitor->mi);

    if (isMoveToLeft) {
        BOOL ret = SetWindowPos(
            window->hWnd, 
            NULL, 
            window->monitor->mi.rcWork.left, 
            window->monitor->mi.rcWork.top, 
            monitorWidth / 2, 
            monitorHeight, 
            SWP_NOZORDER
        );

        if (ret) {
			window->horizontal_alignment = HA_LEFT;
		}

        return ret;
    }
    else {
		BOOL ret = SetWindowPos(
            window->hWnd, 
            NULL, 
            window->monitor->mi.rcWork.left + monitorWidth / 2, 
            window->monitor->mi.rcWork.top, 
            monitorWidth / 2, 
            monitorHeight, 
            SWP_NOZORDER
        );

		if (ret) {
			window->horizontal_alignment = HA_RIGHT;
		}

        return ret;
	}
}

bool MoveWindowToLeft(Window *window, const RECT& windowRect) {
    WINDOWPLACEMENT wp;
    wp.length = sizeof(WINDOWPLACEMENT);
    GetWindowPlacement(window->hWnd, &wp);
    if (wp.showCmd == SW_MAXIMIZE) return MoveMaximizedWindow(window, true);

    // ウィンドウがモニターの左端にある場合、前のモニターに移動
    if (windowRect.left <= window->monitor->mi.rcWork.left) {
        return MoveWindowToOtherMonitor(window, windowRect, false);
    }

    BOOL ret = SetWindowPos(
        window->hWnd, 
        NULL, 
        window->monitor->mi.rcWork.left, 
        window->monitor->mi.rcWork.top, 
        GetMonitorWorkWidth(window->monitor->mi) / 2,
        GetMonitorWorkHeight(window->monitor->mi),
        SWP_NOZORDER
    );

    // Set the horizontal alignment to left if the window is moved to the left
    if (ret) {
		window->horizontal_alignment = HA_LEFT;
	}

    return (bool)ret;
}

bool MoveWindowToRight(Window *window, const RECT& windowRect) {
    WINDOWPLACEMENT wp;
    wp.length = sizeof(WINDOWPLACEMENT);
    GetWindowPlacement(window->hWnd, &wp);
    if (wp.showCmd == SW_MAXIMIZE) return MoveMaximizedWindow(window, false);

    // ウィンドウがモニターの右端にある場合、次のモニターに移動
    if (windowRect.right >= window->monitor->mi.rcWork.right) return MoveWindowToOtherMonitor(window, windowRect, true);

    int monitorWidth = GetMonitorWorkWidth(window->monitor->mi);
	BOOL ret = SetWindowPos(
        window->hWnd, 
        NULL, 
        window->monitor->mi.rcWork.left + monitorWidth / 2, 
        window->monitor->mi.rcWork.top, 
        monitorWidth / 2, 
        GetMonitorWorkHeight(window->monitor->mi),
        SWP_NOZORDER
    );

    if (ret) {
        window->horizontal_alignment = HA_RIGHT;
    }

    return (bool)ret;
}

bool MoveWindowToDown(Window *window, const RECT& windowRect) {
	int monitorWidth = GetMonitorWorkWidth(window->monitor->mi);
	int monitorHeight = GetMonitorWorkHeight(window->monitor->mi);

	BOOL ret = SetWindowPos(
        window->hWnd, 
        NULL, windowRect.left, 
        window->monitor->mi.rcWork.top + monitorHeight / 2, 
        windowRect.right - windowRect.left, 
        monitorHeight / 2, 
        SWP_NOZORDER
    );

    if (ret) {
		window->vertical_alignment = VA_BOTTOM;
	}
    
    return (bool)ret;
}

bool MoveWindowToUp(Window *window, const RECT& windowRect) {
	int monitorWidth = GetMonitorWorkWidth(window->monitor->mi);
	int monitorHeight = GetMonitorWorkHeight(window->monitor->mi);
	BOOL ret = SetWindowPos(
        window->hWnd, 
        NULL, 
        windowRect.left, 
        window->monitor->mi.rcWork.top, 
        windowRect.right - windowRect.left, 
        monitorHeight / 2, 
        SWP_NOZORDER
    );

    if (ret) {
        window->vertical_alignment = VA_TOP;
    }

    return (bool)ret;
}

bool MaximizeWindow(Window *window) {
	BOOL ret = SetWindowPos(
        window->hWnd, 
        NULL, 
        window->monitor->mi.rcWork.left, 
        window->monitor->mi.rcWork.top, 
        GetMonitorWorkWidth(window->monitor->mi), 
        GetMonitorWorkHeight(window->monitor->mi), 
        SWP_NOZORDER
    );

    if (ret) {
        window->isMaximized = true;
    }

    return (bool)ret;
}

bool MoveFocusedWindow(int moveType, HWND& lastMinimizedWindow) {
    HWND hWnd = GetForegroundWindow();
    if (hWnd == NULL) return false;

    if (moveType == HOTKEY_MINIMIZE) {
        return MinimizeWindow(hWnd, lastMinimizedWindow);
    } else if (moveType == HOTKEY_RESTORE) {
        return RestoreWindow(lastMinimizedWindow);
    } else if (moveType == HOTKEY_MAXIMIZE) {
        // ウィンドウを最大化
		ShowWindow(hWnd, SW_MAXIMIZE);
	}

    Window *window = EnsureWindowMonitorTracking(hWnd);
    if (window == nullptr) return false;

    // Get the window information
    RECT rect;
    GetWindowRect(hWnd, &rect);

    switch (moveType) {
        case HOTKEY_MAXIMIZE: return MaximizeWindow(window); break;
		case HOTKEY_LEFT: return MoveWindowToLeft(window, rect); break;
        case HOTKEY_RIGHT: return MoveWindowToRight(window, rect); break;
        case HOTKEY_DOWN: return MoveWindowToDown(window, rect); break;
        case HOTKEY_UP: return MoveWindowToUp(window, rect); break;
		default: return UNKNOWN_HOTKEY;
    }
}

bool ChangeFocusToLeft(Window* window) {
    if (window->left_window == nullptr) return false;

    return SetForegroundWindow(window->left_window->hWnd);
}

bool ChangeFocusToRight(Window* window) {
	if (window->right_window == nullptr) return false;

	return SetForegroundWindow(window->right_window->hWnd);
}

bool ChangeFocusToUp(Window* window) {
	if (window->top_window == nullptr) return false;

	return SetForegroundWindow(window->top_window->hWnd);
}

bool ChangeFocusToDown(Window* window) {
	if (window->bottom_window == nullptr) return false;

	return SetForegroundWindow(window->bottom_window->hWnd);
}

bool ChangeFocusOfWindow(int moveType) {
    Window* window = EnsureWindowMonitorTracking();
    if (window == nullptr) return false;

    switch (moveType) {
        case HOTKEY_FOCUS_DOWN: return ChangeFocusToDown(window); break;
		case HOTKEY_FOCUS_UP: return ChangeFocusToUp(window); break;
		case HOTKEY_FOCUS_LEFT: return ChangeFocusToLeft(window); break;
        case HOTKEY_FOCUS_RIGHT: return ChangeFocusToRight(window); break;
    }

}
