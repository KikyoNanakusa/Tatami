#include <Windows.h>

#define WM_NOTIFYCATION (WM_USER + 100)

HMENU hMenu;
NOTIFYICONDATA nid;

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	POINT point;

	switch (message)
	{
	case WM_CREATE:
		// Register hotkey
		RegisterHotKey(hWnd, 1, MOD_WIN | MOD_SHIFT, 'H');
		RegisterHotKey(hWnd, 2, MOD_WIN | MOD_SHIFT, 'J');
		RegisterHotKey(hWnd, 3, MOD_WIN | MOD_SHIFT, 'K');
		RegisterHotKey(hWnd, 4, MOD_WIN | MOD_SHIFT, 'L');

		// Create context menu
		hMenu = CreatePopupMenu();
		AppendMenu(hMenu, MF_STRING, 1, TEXT("Item 1"));
		AppendMenu(hMenu, MF_STRING, 2, TEXT("Item 2"));
		break;
	case WM_HOTKEY:
		// Send notification to the main window
		break;
	case WM_CONTEXTMENU:
		break;
	case WM_NOTIFYCATION:
		switch (lParam)
		{
			case WM_RBUTTONDOWN:
				GetCursorPos(&point);
				TrackPopupMenu(hMenu, TPM_LEFTALIGN | TPM_RIGHTBUTTON, point.x, point.y, 0, hWnd, NULL);
				break;
		}
		break;
	case WM_DESTROY:
		// Unregister hotkey
		UnregisterHotKey(hWnd, 1);
		DestroyWindow(hWnd);
		Shell_NotifyIcon(NIM_DELETE, &nid);
		DestroyMenu(hMenu);
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	HWND hWnd;
	MSG msg;

	// Create window
	WNDCLASS wc;
	wc.style = 0;
	wc.lpfnWndProc = WndProc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = hInstance;
	wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wc.lpszMenuName = NULL;
	wc.lpszClassName = TEXT("VimHotkeyWindowManager");
	RegisterClass(&wc);

	hWnd = CreateWindow(
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
	
	//タスクトレイにアイコンを追加
	nid.cbSize = sizeof(NOTIFYICONDATA);
	nid.hWnd = hWnd;
	nid.uID = 1;
	nid.uFlags = NIF_ICON | NIF_MESSAGE | NIF_TIP;
	nid.uCallbackMessage = WM_NOTIFYCATION;
	nid.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	lstrcpy(nid.szTip, TEXT("VimHotkeyWindowManager"));
	Shell_NotifyIcon(NIM_ADD, &nid);

	//Message loop
	while (GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
}