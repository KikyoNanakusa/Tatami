#include <windows.h>
#include <vector>

struct MonitorInfo {
    HMONITOR hMonitor;
    RECT rect;
};

// EnumDisplayMonitors�̃R�[���o�b�N�֐�
BOOL CALLBACK MonitorEnumProc(HMONITOR hMonitor, HDC hdcMonitor, LPRECT lprcMonitor, LPARAM dwData) {
    std::vector<MonitorInfo>* monitors = reinterpret_cast<std::vector<MonitorInfo>*>(dwData);
    MonitorInfo info = { hMonitor, *lprcMonitor };
    monitors->push_back(info);
    return TRUE;
}

// �w�肳�ꂽ�E�B���h�E���\������Ă��鎟�̃��j�^�[��HMONITOR��Ԃ�
HMONITOR GetNextMonitor(HWND hWnd) {
    // ���ׂẴ��j�^�[�̏����擾
    std::vector<MonitorInfo> monitors;
    EnumDisplayMonitors(NULL, NULL, MonitorEnumProc, (LPARAM)&monitors);

    // hWnd�����ݕ\������Ă��郂�j�^�[��������
    HMONITOR hCurrentMonitor = MonitorFromWindow(hWnd, MONITOR_DEFAULTTONEAREST);
    for (size_t i = 0; i < monitors.size(); ++i) {
        if (monitors[i].hMonitor == hCurrentMonitor) {
            size_t nextIndex = (i + 1) % monitors.size();
            return monitors[nextIndex].hMonitor;
        }
    }

    return NULL; // ���̃��j�^�[��������Ȃ������ꍇ
}
