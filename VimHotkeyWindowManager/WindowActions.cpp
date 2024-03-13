#include "WindowActions.h"

bool minimizeWindow(HWND hWnd, HWND& minimizedWindow) {
    minimizedWindow = hWnd;
    return ShowWindow(hWnd, SW_MINIMIZE);
}

bool restoreWindow(HWND& minimizedWindow) {
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
        // �E�B���h�E�����j�^�[�̒����Ɉړ�
        newX = monitorCenterX - (windowWidth / 2);
    } else {
        // �E�B���h�E�����j�^�[�̒[�Ɉړ��i���܂��͉E�j
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

std::pair<int, int> CalculateDpiAdjustedSize(HWND hwnd, const RECT& windowRect, HMONITOR hDstMonitor) {
    // �ړ������j�^�[��DPI���擾
    HMONITOR hSrcMonitor = MonitorFromWindow(hwnd, MONITOR_DEFAULTTONEAREST);
    UINT dpiXSrc, dpiYSrc;
    GetDpiForMonitor(hSrcMonitor, MDT_EFFECTIVE_DPI, &dpiXSrc, &dpiYSrc);

    // �ړ��惂�j�^�[��DPI���擾
    UINT dpiXDst, dpiYDst;
    GetDpiForMonitor(hDstMonitor, MDT_EFFECTIVE_DPI, &dpiXDst, &dpiYDst);

    // DPI�X�P�[�����O����v�Z
    float scalingFactorX = static_cast<float>(dpiXDst) / dpiXSrc;
    float scalingFactorY = static_cast<float>(dpiYDst) / dpiYSrc;

    // �X�P�[�����O���ꂽ�T�C�Y���v�Z
    int scaledWidth = static_cast<int>((windowRect.right - windowRect.left) / scalingFactorX);
    int scaledHeight = static_cast<int>((windowRect.bottom - windowRect.top) / scalingFactorY);

    return std::make_pair(scaledWidth, scaledHeight);
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
    //newSize = CalculateDpiAdjustedSize(hwnd, windowRect, hNextMonitor);

    //int newLeft = miNext.rcWork.left + (miNext.rcWork.right - newSize.first) / 2;
    int newLeft = miNext.rcWork.left;
    
	return SetWindowPos(hwnd, NULL, newLeft, miNext.rcWork.top, newSize.first, newSize.second, SWP_NOZORDER);
}

bool MoveWindowToLeft(HWND hWnd, const MONITORINFO& mi, const RECT& windowRect) {
    // �E�B���h�E�����j�^�[�̍��[�ɂ���ꍇ�A�O�̃��j�^�[�Ɉړ�
    if (windowRect.left <= mi.rcWork.left) return MoveWindowToOtherMonitor(hWnd, mi, windowRect, false);
    return AdjustWindowPosition(hWnd, mi, windowRect, false); // �����ֈړ�
}

bool MoveWindowToRight(HWND hWnd, const MONITORINFO& mi, const RECT& windowRect) {
    // �E�B���h�E�����j�^�[�̉E�[�ɂ���ꍇ�A���̃��j�^�[�Ɉړ�
    if (windowRect.right >= mi.rcWork.right) return MoveWindowToOtherMonitor(hWnd, mi, windowRect, true);
    return AdjustWindowPosition(hWnd, mi, windowRect, true); // �E���ֈړ�
}

bool MoveFocusedWindow(int moveType, HWND& minimizedWindow) {
    HWND hWnd = GetForegroundWindow();
    if (hWnd == NULL) return false;

    if (moveType == HOTKEY_MINIMIZE) {
        return minimizeWindow(hWnd, minimizedWindow);
    } else if (moveType == HOTKEY_RESTORE) {
        return restoreWindow(minimizedWindow);
    } else if (moveType == HOTKEY_MAXIMIZE) {
		return ShowWindow(hWnd, SW_MAXIMIZE);
	}

    // �E�B���h�E�����郂�j�^�[�̏����擾
    MONITORINFO mi = { sizeof(mi) };
    if (!GetMonitorInfo(MonitorFromWindow(hWnd, MONITOR_DEFAULTTONEAREST), &mi)) return false;

    // �E�B���h�E�̈ʒu�ƃT�C�Y���擾
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
		default: return UNKNOWN_HOTKEY;
    }
}
