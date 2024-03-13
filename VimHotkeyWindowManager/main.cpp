#include <Windows.h>
#include "definition.h"
#include "MonitorUtils.h"


HMENU hMenu;
NOTIFYICONDATA nid;
HWND minimizedWindow = NULL;

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

HWND InitWindow(HINSTANCE hInstance) {
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

void InitTaskIcon(HWND hWnd) {
	ZeroMemory(&nid, sizeof(nid));
	nid.cbSize = sizeof(NOTIFYICONDATA);
	nid.hWnd = hWnd;
	nid.uID = 1;
	nid.uFlags = NIF_ICON | NIF_MESSAGE | NIF_TIP;
	nid.uCallbackMessage = WM_NOTIFICATION;
	nid.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	lstrcpy(nid.szTip, TEXT("VimHotkeyWindowManager"));
	Shell_NotifyIcon(NIM_ADD, &nid);
}

void RegisterHotKeys(HWND hWnd) {
	RegisterHotKey(hWnd, HOTKEY_LEFT, MOD_ALT | MOD_SHIFT, 'H');
	RegisterHotKey(hWnd, HOTKEY_DOWN, MOD_ALT | MOD_SHIFT, 'J');
	RegisterHotKey(hWnd, HOTKEY_UP, MOD_ALT | MOD_SHIFT, 'K');
	RegisterHotKey(hWnd, HOTKEY_RIGHT, MOD_ALT | MOD_SHIFT, 'L');
	RegisterHotKey(hWnd, HOTKEY_MINIMIZE, MOD_ALT | MOD_SHIFT, 'D');
	RegisterHotKey(hWnd, HOTKEY_RESTORE, MOD_ALT | MOD_SHIFT, 'U');
}

bool minimizeWindow(HWND hWnd) {
    minimizedWindow = hWnd;
    return ShowWindow(hWnd, SW_MINIMIZE);
}

bool restoreWindow() {
    if (minimizedWindow == NULL) return NO_MINIMIZED_WINDOW;
    bool ret = ShowWindow(minimizedWindow, SW_RESTORE);
    minimizedWindow = NULL;
    return ret;
}

bool AdjustWindowPosition(HWND hWnd, const MONITORINFO& mi, const RECT& windowRect, bool moveToRight) {
    int monitorWidth = mi.rcMonitor.right - mi.rcMonitor.left;
    int windowWidth = windowRect.right - windowRect.left;
    int monitorCenterX = mi.rcMonitor.left + monitorWidth / 2;
    int windowCenterX = windowRect.left + windowWidth / 2;

    int newX, newY = windowRect.top;

    if ((moveToRight && windowCenterX < monitorCenterX) || (!moveToRight && windowCenterX > monitorCenterX)) {
        // ウィンドウをモニターの中央に移動
        newX = monitorCenterX - (windowWidth / 2);
    } else {
        // ウィンドウをモニターの端に移動（左または右）
        newX = moveToRight ? mi.rcMonitor.right - windowWidth : mi.rcMonitor.left;
    }

    return SetWindowPos(hWnd, NULL, newX, newY, 0, 0, SWP_NOSIZE | SWP_NOZORDER);
}

bool MoveWindowToLeft(HWND hWnd, const MONITORINFO& mi, const RECT& windowRect) {
    return AdjustWindowPosition(hWnd, mi, windowRect, false); // 左側へ移動
}

bool MoveWindowToRight(HWND hWnd, const MONITORINFO& mi, const RECT& windowRect) {
    return AdjustWindowPosition(hWnd, mi, windowRect, true); // 右側へ移動
}

bool moveFocusedWindow(int moveType) {
    HWND hWnd = GetForegroundWindow();
    if (hWnd == NULL) return false;

    if (moveType == HOTKEY_MINIMIZE) {
        return minimizeWindow(hWnd);
    } else if (moveType == HOTKEY_RESTORE) {
        return restoreWindow();
    }

    // ウィンドウがあるモニターの情報を取得
    MONITORINFO mi = { sizeof(mi) };
    if (!GetMonitorInfo(MonitorFromWindow(hWnd, MONITOR_DEFAULTTONEAREST), &mi)) return false;

    // ウィンドウの位置とサイズを取得
    RECT rect;
    GetWindowRect(hWnd, &rect);

    switch (moveType) {
		case HOTKEY_LEFT: return MoveWindowToLeft(hWnd, mi, rect); break;
        case HOTKEY_RIGHT: return MoveWindowToRight(hWnd, mi, rect); break;
        case HOTKEY_DOWN: 
			return SetWindowPos(hWnd, NULL, 
				rect.left, mi.rcWork.bottom - (rect.bottom - rect.top), 0, 0, SWP_NOSIZE | SWP_NOZORDER);
        case HOTKEY_UP: 
			return SetWindowPos(hWnd, NULL, rect.left, mi.rcWork.top, 0, 0, SWP_NOSIZE | SWP_NOZORDER);
		default: return false;
    }

}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	HWND hWnd;
	MSG msg;

	//ウィンドウを初期化
	hWnd = InitWindow(hInstance);

	//タスクトレイにアイコンを追加
	InitTaskIcon(hWnd);

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
		RegisterHotKeys(hWnd);

		// Create context menu
		hMenu = CreatePopupMenu();
		AppendMenu(hMenu, MF_STRING, MENU_ACTIVATE, TEXT("有効無効切り替え"));
		AppendMenu(hMenu, MF_STRING, MENU_EXIT, TEXT("終了"));
		break;
	case WM_HOTKEY:
		// Send notification to the main window
		moveFocusedWindow(wParam);
		break;
	case WM_CONTEXTMENU:
		break;
	case WM_NOTIFICATION:
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
			OnDestroy(hWnd);
			PostQuitMessage(0);
			break;
		case MENU_ACTIVATE:
			//TODO:有効無効の切り替えを実装する
			MessageBox(hWnd, TEXT("TODO:有効無効の切り替えを実装する"), TEXT("Menu"), MB_OK);
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