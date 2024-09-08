#include "HotkeyManager.h"

bool isMoveWindowHotkey(UINT message) {
	return message == HOTKEY_LEFT || message == HOTKEY_DOWN || message == HOTKEY_UP || message == HOTKEY_RIGHT ||
		message == HOTKEY_MAXIMIZE || message == HOTKEY_MINIMIZE || message == HOTKEY_RESTORE;
}

void RegisterHotKeys(HWND hWnd, Config* config) {
    if (!config) {
       	MessageBox(hWnd, L"Invalid config pointer", L"Error", MB_ICONERROR);
		PostQuitMessage(1);
		return;
    }

    if (!RegisterHotKey(hWnd, HOTKEY_LEFT, MOD_ALT | MOD_SHIFT, config->getConfigValue(ACTION_MOVE_WINDOW_LEFT)) ||
        !RegisterHotKey(hWnd, HOTKEY_DOWN, MOD_ALT | MOD_SHIFT, config->getConfigValue(ACTION_MOVE_WINDOW_DOWN)) ||
        !RegisterHotKey(hWnd, HOTKEY_UP, MOD_ALT | MOD_SHIFT, config->getConfigValue(ACTION_MOVE_WINDOW_UP)) ||
        !RegisterHotKey(hWnd, HOTKEY_RIGHT, MOD_ALT | MOD_SHIFT, config->getConfigValue(ACTION_MOVE_WINDOW_RIGHT)) ||
        !RegisterHotKey(hWnd, HOTKEY_MINIMIZE, MOD_ALT | MOD_SHIFT, config->getConfigValue(ACTION_WINDOW_MINIMIZE)) ||
        !RegisterHotKey(hWnd, HOTKEY_RESTORE, MOD_ALT | MOD_SHIFT, config->getConfigValue(ACTION_WINDOW_RESTORE)) ||
        !RegisterHotKey(hWnd, HOTKEY_MAXIMIZE, MOD_ALT | MOD_SHIFT, config->getConfigValue(ACTION_WINDOW_MAXIMIZE))) {
		
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
