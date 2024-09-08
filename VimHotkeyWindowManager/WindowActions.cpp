#include "WindowActions.h"

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

std::pair<int, int> CalculateNewSizeForMonitor(const RECT& windowRect, const MONITORINFO& originMonitorInfo, const MONITORINFO& nextMonitorInfo) {
    int windowWidth = windowRect.right - windowRect.left;
    int windowHeight = windowRect.bottom - windowRect.top;

    int origMonitorWidth = GetMonitorWorkWidth(originMonitorInfo);
    int origMonitorHeight = GetMonitorWorkHeight(originMonitorInfo);

    int nextMonitorWidth = GetMonitorWorkWidth(nextMonitorInfo);
    int nextMonitorHeight = GetMonitorWorkHeight(nextMonitorInfo);

    float widthRatio = static_cast<float>(nextMonitorWidth) / origMonitorWidth;
    float heightRatio = static_cast<float>(nextMonitorHeight) / origMonitorHeight;

    int newWidth = static_cast<int>(windowWidth * widthRatio);
    int newHeight = static_cast<int>(windowHeight * heightRatio);

    return std::make_pair(newWidth, newHeight);
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

    std::pair<int, int> newSize = CalculateNewSizeForMonitor(windowRect, window->monitor->mi, targetMonitor->mi);

    int newLeft = targetMonitor->mi.rcWork.left;
    
    BOOL ret = SetWindowPos(window->hWnd, NULL, newLeft, 
        targetMonitor->mi.rcWork.top, newSize.first, newSize.second, SWP_NOZORDER);
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
        return SetWindowPos(
            window->hWnd, 
            NULL, 
            window->monitor->mi.rcWork.left, 
            window->monitor->mi.rcWork.top, 
            monitorWidth / 2, 
            monitorHeight, SWP_NOZORDER
        );
    }
    else {
		return SetWindowPos(
            window->hWnd, 
            NULL, 
            window->monitor->mi.rcWork.left + monitorWidth / 2, 
            window->monitor->mi.rcWork.top, 
            monitorWidth / 2, 
            monitorHeight, 
            SWP_NOZORDER);
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

    int monitorWidth = GetMonitorWorkWidth(window->monitor->mi);
    int monitorHeight = GetMonitorWorkHeight(window->monitor->mi);
    return SetWindowPos(
        window->hWnd, 
        NULL, 
        window->monitor->mi.rcWork.left, 
        window->monitor->mi.rcWork.top, 
        monitorWidth / 2, 
        monitorHeight, 
        SWP_NOZORDER
    );
}

bool MoveWindowToRight(Window *window, const RECT& windowRect) {
    WINDOWPLACEMENT wp;
    wp.length = sizeof(WINDOWPLACEMENT);
    GetWindowPlacement(window->hWnd, &wp);
    if (wp.showCmd == SW_MAXIMIZE) return MoveMaximizedWindow(window, false);

    // ウィンドウがモニターの右端にある場合、次のモニターに移動
    if (windowRect.right >= window->monitor->mi.rcWork.right) return MoveWindowToOtherMonitor(window, windowRect, true);

    int monitorWidth = GetMonitorWorkWidth(window->monitor->mi);
    int monitorHeight = GetMonitorWorkHeight(window->monitor->mi);
	return SetWindowPos(
        window->hWnd, 
        NULL, 
        window->monitor->mi.rcWork.left + monitorWidth / 2, 
        window->monitor->mi.rcWork.top, 
        monitorWidth / 2, 
        monitorHeight, 
        SWP_NOZORDER
    );
}

bool MoveWindowToDown(Window *window, const RECT& windowRect) {
	int monitorWidth = GetMonitorWorkWidth(window->monitor->mi);
	int monitorHeight = GetMonitorWorkHeight(window->monitor->mi);
	return SetWindowPos(
        window->hWnd, 
        NULL, windowRect.left, 
        window->monitor->mi.rcWork.top + monitorHeight / 2, 
        windowRect.right - windowRect.left, 
        monitorHeight / 2, 
        SWP_NOZORDER
    );
}

bool MoveWindowToUp(Window *window, const RECT& windowRect) {
	int monitorWidth = GetMonitorWorkWidth(window->monitor->mi);
	int monitorHeight = GetMonitorWorkHeight(window->monitor->mi);
	return SetWindowPos(
        window->hWnd, 
        NULL, 
        windowRect.left, 
        window->monitor->mi.rcWork.top, 
        windowRect.right - windowRect.left, 
        monitorHeight / 2, 
        SWP_NOZORDER
    );
}

bool MaximizeWindow(Window *window, const MONITORINFO& mi) {
	return SetWindowPos(window->hWnd, NULL, mi.rcWork.left, mi.rcWork.top, GetMonitorWorkWidth(mi), GetMonitorWorkHeight(mi), SWP_NOZORDER);
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


    // ウィンドウがあるモニターの情報を取得
    MONITORINFO monitorInfo = { sizeof(monitorInfo) };
    HMONITOR hMonitor = MonitorFromWindow(hWnd, MONITOR_DEFAULTTONEAREST);
    if (!GetMonitorInfo(hMonitor, &monitorInfo)) return false;
    
    // モニターがリストにあるか確認
    //TODO: エラーハンドリング
    Monitor *monitor = FindMonitorByHmonitor(hMonitor);
    if (monitor == nullptr) {
        return false;
	}

    // ウィンドウがリストにあるか確認、無ければ追加
    Window *window = FindWindowByHwnd(hWnd);
    if (window == nullptr) {
        AddWindowToList(hWnd, monitor);
        window = head_window;
    }

    // ウィンドウの位置とサイズを取得
    RECT rect;
    GetWindowRect(hWnd, &rect);

    switch (moveType) {
        case HOTKEY_MAXIMIZE: return MaximizeWindow(window, monitorInfo); break;
		case HOTKEY_LEFT: return MoveWindowToLeft(window, rect); break;
        case HOTKEY_RIGHT: return MoveWindowToRight(window, rect); break;
        case HOTKEY_DOWN: return MoveWindowToDown(window, rect); break;
        case HOTKEY_UP: return MoveWindowToUp(window, rect); break;
		default: return UNKNOWN_HOTKEY;
    }
}
