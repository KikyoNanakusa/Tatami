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


bool MoveWindowToOtherMonitor(HWND hwnd, const MONITORINFO& mi, const RECT& windowRect, bool isMoveWindowNextMonitor) {
    HMONITOR hTargetMonitor = NULL;
    if (isMoveWindowNextMonitor) {
		hTargetMonitor = GetNextMonitor(hwnd);
		if (hTargetMonitor == NULL) return false;
    }
    else {
		hTargetMonitor = GetPreviousMonitor(hwnd);
		if (hTargetMonitor == NULL) return false;
	}

	MONITORINFO miNext = { sizeof(miNext) };
	if (!GetMonitorInfo(hTargetMonitor, &miNext)) return false;

    std::pair<int, int> newSize = CalculateNewSizeForMonitor(windowRect, mi, miNext);

    int newLeft = miNext.rcWork.left;
    
	return SetWindowPos(hwnd, NULL, newLeft, miNext.rcWork.top, newSize.first, newSize.second, SWP_NOZORDER);
}

bool MoveMaximizedWindow(HWND hWnd, const MONITORINFO& mi, bool isMoveToLeft) {
    ShowWindow(hWnd, SW_RESTORE);
    int monitorWidth = GetMonitorWorkWidth(mi);
    int monitorHeight = GetMonitorWorkHeight(mi);
    if (isMoveToLeft) {
        return SetWindowPos(hWnd, NULL, mi.rcWork.left, mi.rcWork.top, monitorWidth / 2, monitorHeight, SWP_NOZORDER);
    }
    else {
		return SetWindowPos(hWnd, NULL, mi.rcWork.left + monitorWidth / 2, mi.rcWork.top, monitorWidth / 2, monitorHeight, SWP_NOZORDER);
	}
}

bool MoveWindowToLeft(HWND hWnd, const MONITORINFO& mi, const RECT& windowRect) {
    WINDOWPLACEMENT wp;
    wp.length = sizeof(WINDOWPLACEMENT);
    GetWindowPlacement(hWnd, &wp);
    if (wp.showCmd == SW_MAXIMIZE) return MoveMaximizedWindow(hWnd, mi, true);

    // ウィンドウがモニターの左端にある場合、前のモニターに移動
    if (windowRect.left <= mi.rcWork.left) {
        return MoveWindowToOtherMonitor(hWnd, mi, windowRect, false);
    }

    int monitorWidth = GetMonitorWorkWidth(mi);
    int monitorHeight = GetMonitorWorkHeight(mi);
    return SetWindowPos(hWnd, NULL, mi.rcWork.left, mi.rcWork.top, monitorWidth / 2, monitorHeight, SWP_NOZORDER);
}

bool MoveWindowToRight(HWND hWnd, const MONITORINFO& mi, const RECT& windowRect) {
    WINDOWPLACEMENT wp;
    wp.length = sizeof(WINDOWPLACEMENT);
    GetWindowPlacement(hWnd, &wp);
    if (wp.showCmd == SW_MAXIMIZE) return MoveMaximizedWindow(hWnd, mi, false);

    // ウィンドウがモニターの右端にある場合、次のモニターに移動
    if (windowRect.right >= mi.rcWork.right) return MoveWindowToOtherMonitor(hWnd, mi, windowRect, true);

    int monitorWidth = GetMonitorWorkWidth(mi);
    int monitorHeight = GetMonitorWorkHeight(mi);
	return SetWindowPos(hWnd, NULL, mi.rcWork.left + monitorWidth / 2, mi.rcWork.top, monitorWidth / 2, monitorHeight, SWP_NOZORDER);
}

bool MoveWindowToDown(HWND hWnd, const MONITORINFO& mi, const RECT& windowRect) {
	int monitorWidth = GetMonitorWorkWidth(mi);
	int monitorHeight = GetMonitorWorkHeight(mi);
	return SetWindowPos(hWnd, NULL, windowRect.left, mi.rcWork.top + monitorHeight / 2, windowRect.right - windowRect.left, monitorHeight / 2, SWP_NOZORDER);
}

bool MoveWindowToUp(HWND hWnd, const MONITORINFO& mi, const RECT& windowRect) {
	int monitorWidth = GetMonitorWorkWidth(mi);
	int monitorHeight = GetMonitorWorkHeight(mi);
	return SetWindowPos(hWnd, NULL, windowRect.left, mi.rcWork.top, windowRect.right - windowRect.left, monitorHeight / 2, SWP_NOZORDER);
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
		return ShowWindow(hWnd, SW_MAXIMIZE);
	}

    // ウィンドウがあるモニターの情報を取得
    MONITORINFO monitorInfo = { sizeof(monitorInfo) };
    if (!GetMonitorInfo(MonitorFromWindow(hWnd, MONITOR_DEFAULTTONEAREST), &monitorInfo)) return false;

    // ウィンドウの位置とサイズを取得
    RECT rect;
    GetWindowRect(hWnd, &rect);

    switch (moveType) {
		case HOTKEY_LEFT: return MoveWindowToLeft(hWnd, monitorInfo, rect); break;
        case HOTKEY_RIGHT: return MoveWindowToRight(hWnd, monitorInfo, rect); break;
        case HOTKEY_DOWN: return MoveWindowToDown(hWnd, monitorInfo, rect); break;
        case HOTKEY_UP: return MoveWindowToUp(hWnd, monitorInfo, rect); break;
		default: return UNKNOWN_HOTKEY;
    }
}
