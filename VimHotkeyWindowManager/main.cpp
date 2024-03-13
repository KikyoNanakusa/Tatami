#include <Windows.h>

#define WM_NOTIFYCATION (WM_USER + 100)
#define HOTKEY_LEFT 1
#define HOTKEY_DOWN 2
#define HOTKEY_UP 3
#define HOTKEY_RIGHT 4
#define MENU_ACTIVATE 0x1
#define MENU_EXIT 0x2

HMENU hMenu;
NOTIFYICONDATA nid;

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

bool OnDestroy(HWND hWnd)
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
	
	//�^�X�N�g���C�ɃA�C�R����ǉ�
	ZeroMemory(&nid, sizeof(nid));
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
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	POINT point;

	switch (message)
	{
	case WM_CREATE:
		// Register hotkey
		RegisterHotKey(hWnd, HOTKEY_LEFT, MOD_WIN | MOD_SHIFT, 'H');
		RegisterHotKey(hWnd, HOTKEY_DOWN, MOD_WIN | MOD_SHIFT, 'J');
		RegisterHotKey(hWnd, HOTKEY_UP, MOD_WIN | MOD_SHIFT, 'K');
		RegisterHotKey(hWnd, HOTKEY_RIGHT, MOD_WIN | MOD_SHIFT, 'L');

		// Create context menu
		hMenu = CreatePopupMenu();
		AppendMenu(hMenu, MF_STRING, MENU_ACTIVATE, TEXT("�L�������؂�ւ�"));
		AppendMenu(hMenu, MF_STRING, MENU_EXIT, TEXT("�I��"));
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
				//NOTE:SetForegroundWindow() is required to delete the context menu
				SetForegroundWindow(hWnd);
				GetCursorPos(&point);
				int cmd = TrackPopupMenu(hMenu, TPM_LEFTALIGN | TPM_RIGHTBUTTON | TPM_RETURNCMD, point.x, point.y, 0, hWnd, NULL);
				SendMessage(hWnd, WM_COMMAND, cmd, 0);
				break;
		}
		break;
	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		case MENU_EXIT:
			PostQuitMessage(0);
			break;
		case MENU_ACTIVATE:
			MessageBox(hWnd, TEXT("Item 2"), TEXT("Menu"), MB_OK);
			break;
		}
		break;
	case WM_DESTROY:
		OnDestroy(hWnd);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}