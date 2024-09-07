#include "WindowManager.h"
#include "window.h"

bool OnDestroy(HWND hWnd, NOTIFYICONDATA nid, HMENU hMenu)
{
	// Unregister hotkey
	UnregisterHotKey(hWnd, HOTKEY_LEFT);
	UnregisterHotKey(hWnd, HOTKEY_UP);
	UnregisterHotKey(hWnd, HOTKEY_DOWN);
	UnregisterHotKey(hWnd, HOTKEY_RIGHT);
	UnregisterHotKey(hWnd, HOTKEY_MAXIMIZE);
	UnregisterHotKey(hWnd, HOTKEY_MINIMIZE);
	UnregisterHotKey(hWnd, HOTKEY_RESTORE);

	// Destroy monitor list
	Monitor* current = primary_monitor;
	while (current) {
		Monitor* next = current->next_monitor;
		delete current;
		current = next;
	}

	// Destroy window
	DestroyWindow(hWnd);
	Shell_NotifyIcon(NIM_DELETE, &nid);
	DestroyMenu(hMenu);
	PostQuitMessage(0);
	return true;
}

HWND InitWindow(HINSTANCE hInstance ,WNDPROC lpfnWndProc, Config* config) {
	// Create window
	WNDCLASS wc;
	wc.style = 0;
	wc.lpfnWndProc = lpfnWndProc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = sizeof(Config*);
	wc.hInstance = hInstance;
	wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wc.lpszMenuName = NULL;
	wc.lpszClassName = TEXT("VimHotkeyWindowManager");

	if (!RegisterClass(&wc)) {
		DWORD error = GetLastError();
		MessageBox(NULL, TEXT("Failed to register window class"), TEXT("ERROR"), MB_OK);
		return NULL;
	}

	// Use lParam to pass Config object
	HWND hWnd = CreateWindow(
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
		config
	);

	return hWnd;
}
