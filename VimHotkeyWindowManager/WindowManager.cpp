#include "WindowManager.h"

bool OnDestroy(HWND hWnd, NOTIFYICONDATA nid, HMENU hMenu)
{
	// Unregister hotkey
	UnregisterHotKey(hWnd, HOTKEY_LEFT);
	UnregisterHotKey(hWnd, HOTKEY_UP);
	UnregisterHotKey(hWnd, HOTKEY_DOWN);
	UnregisterHotKey(hWnd, HOTKEY_RIGHT);

	// Destroy window
	DestroyWindow(hWnd);
	Shell_NotifyIcon(NIM_DELETE, &nid);
	DestroyMenu(hMenu);
	PostQuitMessage(0);
	return true;
}

HWND InitWindow(HINSTANCE hInstance ,WNDPROC lpfnWndProc) {
	// Create window
	WNDCLASS wc;
	wc.style = 0;
	wc.lpfnWndProc = lpfnWndProc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = hInstance;
	wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wc.lpszMenuName = NULL;
	wc.lpszClassName = TEXT("VimHotkeyWindowManager");
	RegisterClass(&wc);
	return CreateWindow(
		TEXT("VimHotkeyWindowManager"),
		TEXT("VimHotkeyWindowManager"),
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		640,
		480,
		NULL,
		NULL,
		hInstance,
		NULL
	);
}
