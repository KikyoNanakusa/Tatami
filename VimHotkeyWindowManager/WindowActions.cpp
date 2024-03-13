#include "WindowActions.h"

bool minimizeWindow(HWND hWnd, HWND minimizedWindow) {
    minimizedWindow = hWnd;
    return ShowWindow(hWnd, SW_MINIMIZE);
}

bool restoreWindow(HWND minimizedWindow) {
    if (minimizedWindow == NULL) return NO_MINIMIZED_WINDOW;
    bool ret = ShowWindow(minimizedWindow, SW_RESTORE);
    minimizedWindow = NULL;
    return ret;
}

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

bool MoveWindowToLeft(HWND hWnd, const MONITORINFO& mi, const RECT& windowRect) {
    return AdjustWindowPosition(hWnd, mi, windowRect, false); // 左側へ移動
}

bool MoveWindowToRight(HWND hWnd, const MONITORINFO& mi, const RECT& windowRect) {
    return AdjustWindowPosition(hWnd, mi, windowRect, true); // 右側へ移動
}

bool MoveFocusedWindow(int moveType, HWND minimizedWindow) {
    HWND hWnd = GetForegroundWindow();
    if (hWnd == NULL) return false;

    if (moveType == HOTKEY_MINIMIZE) {
        return minimizeWindow(hWnd, minimizedWindow);
    } else if (moveType == HOTKEY_RESTORE) {
        return restoreWindow(minimizedWindow);
    }

    // ウィンドウがあるモニターの情報を取得
    MONITORINFO mi = { sizeof(mi) };
    if (!GetMonitorInfo(MonitorFromWindow(hWnd, MONITOR_DEFAULTTONEAREST), &mi)) return false;

    // ウィンドウの位置とサイズを取得
    RECT rect;
    GetWindowRect(hWnd, &rect);

    switch (moveType) {
		case HOTKEY_LEFT: return MoveWindowToLeft(hWnd, mi, rect); break;
        case HOTKEY_RIGHT: return MoveWindowToRight(hWnd, mi, rect); break;
        case HOTKEY_DOWN: 
			return SetWindowPos(hWnd, NULL, 
				rect.left, mi.rcWork.bottom - (rect.bottom - rect.top), 0, 0, SWP_NOSIZE | SWP_NOZORDER);
        case HOTKEY_UP: 
			return SetWindowPos(hWnd, NULL, rect.left, mi.rcWork.top, 0, 0, SWP_NOSIZE | SWP_NOZORDER);
		default: return false;
    }

}
