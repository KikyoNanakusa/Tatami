#include <windows.h>
#include <vector>

struct MonitorInfo {
    HMONITOR hMonitor;
    RECT rect;
};

// EnumDisplayMonitorsのコールバック関数
BOOL CALLBACK MonitorEnumProc(HMONITOR hMonitor, HDC hdcMonitor, LPRECT lprcMonitor, LPARAM dwData) {
    std::vector<MonitorInfo>* monitors = reinterpret_cast<std::vector<MonitorInfo>*>(dwData);
    MonitorInfo info = { hMonitor, *lprcMonitor };
    monitors->push_back(info);
    return TRUE;
}

// 指定されたウィンドウが表示されている次のモニターのHMONITORを返す
HMONITOR GetNextMonitor(HWND hWnd) {
    // すべてのモニターの情報を取得
    std::vector<MonitorInfo> monitors;
    EnumDisplayMonitors(NULL, NULL, MonitorEnumProc, (LPARAM)&monitors);

    // hWndが現在表示されているモニターを見つける
    HMONITOR hCurrentMonitor = MonitorFromWindow(hWnd, MONITOR_DEFAULTTONEAREST);
    for (size_t i = 0; i < monitors.size(); ++i) {
        if (monitors[i].hMonitor == hCurrentMonitor) {
            size_t nextIndex = (i + 1) % monitors.size();
            return monitors[nextIndex].hMonitor;
        }
    }

    return NULL; // 次のモニターが見つからなかった場合
}

HMONITOR GetPreviousMonitor(HWND hWnd) {
    // すべてのモニターの情報を取得
    std::vector<MonitorInfo> monitors;
    EnumDisplayMonitors(NULL, NULL, MonitorEnumProc, (LPARAM)&monitors);

    // hWndが現在表示されているモニターを見つける
    HMONITOR hCurrentMonitor = MonitorFromWindow(hWnd, MONITOR_DEFAULTTONEAREST);
    for (size_t i = 0; i < monitors.size(); ++i) {
        if (monitors[i].hMonitor == hCurrentMonitor) {
            // 前のモニターのインデックスを計算（0の場合はラップアラウンド）
            size_t prevIndex = (i == 0) ? monitors.size() - 1 : i - 1;
            return monitors[prevIndex].hMonitor;
        }
    }

    return NULL; // 前のモニターが見つからなかった場合
}

int GetMonitorCenter(MONITORINFO mi) {
	return (mi.rcWork.left + mi.rcWork.right) / 2;
}

int GetMonitorWorkWidth(MONITORINFO mi) {
	return mi.rcWork.right - mi.rcWork.left;
}

int GetMonitorWorkHeight(MONITORINFO mi) {
	return mi.rcWork.bottom - mi.rcWork.top;
}
