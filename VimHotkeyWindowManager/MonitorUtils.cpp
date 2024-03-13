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
