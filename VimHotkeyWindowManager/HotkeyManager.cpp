#include "HotkeyManager.h"

void RegisterHotKeys(HWND hWnd, Config* config) {
    if (!RegisterHotKey(hWnd, HOTKEY_LEFT, MOD_ALT | MOD_SHIFT, 'H') ||
        !RegisterHotKey(hWnd, HOTKEY_DOWN, MOD_ALT | MOD_SHIFT, 'J') ||
        !RegisterHotKey(hWnd, HOTKEY_UP, MOD_ALT | MOD_SHIFT, 'K') ||
        !RegisterHotKey(hWnd, HOTKEY_RIGHT, MOD_ALT | MOD_SHIFT, 'L') ||
        !RegisterHotKey(hWnd, HOTKEY_MINIMIZE, MOD_ALT | MOD_SHIFT, 'D') ||
        !RegisterHotKey(hWnd, HOTKEY_RESTORE, MOD_ALT | MOD_SHIFT, 'U') ||
        !RegisterHotKey(hWnd, HOTKEY_MAXIMIZE, MOD_ALT | MOD_SHIFT, 'G')) {
        
        MessageBox(hWnd, L"ホットキーの登録に失敗しました。プログラムを終了します。", L"エラー", MB_ICONERROR);
        UnRegisterHotKeys(hWnd);
        PostQuitMessage(1); 
        return;
    }
}

void UnRegisterHotKeys(HWND hWnd) {
	UnregisterHotKey(hWnd, HOTKEY_LEFT);
	UnregisterHotKey(hWnd, HOTKEY_DOWN);
	UnregisterHotKey(hWnd, HOTKEY_UP);
	UnregisterHotKey(hWnd, HOTKEY_RIGHT);
	UnregisterHotKey(hWnd, HOTKEY_MINIMIZE);
	UnregisterHotKey(hWnd, HOTKEY_RESTORE);
	UnregisterHotKey(hWnd, HOTKEY_MAXIMIZE);
}
