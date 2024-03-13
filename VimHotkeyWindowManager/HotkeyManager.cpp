#include "HotkeyManager.h"

void RegisterHotKeys(HWND hWnd) {
	RegisterHotKey(hWnd, HOTKEY_LEFT, MOD_ALT | MOD_SHIFT, 'H');
	RegisterHotKey(hWnd, HOTKEY_DOWN, MOD_ALT | MOD_SHIFT, 'J');
	RegisterHotKey(hWnd, HOTKEY_UP, MOD_ALT | MOD_SHIFT, 'K');
	RegisterHotKey(hWnd, HOTKEY_RIGHT, MOD_ALT | MOD_SHIFT, 'L');
	RegisterHotKey(hWnd, HOTKEY_MINIMIZE, MOD_ALT | MOD_SHIFT, 'D');
	RegisterHotKey(hWnd, HOTKEY_RESTORE, MOD_ALT | MOD_SHIFT, 'U');
}
