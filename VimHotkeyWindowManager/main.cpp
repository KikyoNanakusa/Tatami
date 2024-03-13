#include <Windows.h>

#define WM_NOTIFYCATION (WM_USER + 100)
#define HOTKEY_LEFT 1
#define HOTKEY_DOWN 2
#define HOTKEY_UP 3
#define HOTKEY_RIGHT 4
#define HOTKEY_MINIMIZE 5
#define HOTKEY_RESTORE 6

#define MENU_ACTIVATE 0x1
#define MENU_EXIT 0x2

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
	nid.uCallbackMessage = WM_NOTIFYCATION;
	nid.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	lstrcpy(nid.szTip, TEXT("VimHotkeyWindowManager"));
	Shell_NotifyIcon(NIM_ADD, &nid);
}

void RegisterHotKeys(HWND hWnd) {
	RegisterHotKey(hWnd, HOTKEY_LEFT, MOD_WIN | MOD_SHIFT, 'H');
	RegisterHotKey(hWnd, HOTKEY_DOWN, MOD_WIN | MOD_SHIFT, 'J');
	RegisterHotKey(hWnd, HOTKEY_UP, MOD_WIN | MOD_SHIFT, 'K');
	RegisterHotKey(hWnd, HOTKEY_RIGHT, MOD_WIN | MOD_SHIFT, 'L');
	RegisterHotKey(hWnd, HOTKEY_MINIMIZE, MOD_WIN | MOD_SHIFT, 'D');
	RegisterHotKey(hWnd, HOTKEY_RESTORE, MOD_WIN | MOD_SHIFT, 'U');
}

bool moveFocusedWindow(int moveType) {
	//フォーカスされているウィンドウを取得
    HWND hWnd = GetForegroundWindow();
    if (hWnd == NULL) return false;

    // ウィンドウがあるモニターの情報を取得
    HMONITOR hMonitor = MonitorFromWindow(hWnd, MONITOR_DEFAULTTONEAREST);
    MONITORINFO mi = { sizeof(mi) };
    if (!GetMonitorInfo(hMonitor, &mi)) return false;

    // ウィンドウの位置とサイズを取得
    RECT rect;
    GetWindowRect(hWnd, &rect);
    int windowWidth = rect.right - rect.left;
    int windowHeight = rect.bottom - rect.top;

    int newX = rect.left;
    int newY = rect.top;

    switch (moveType) {
        case HOTKEY_LEFT:
            newX = mi.rcWork.left;
            break;
        case HOTKEY_RIGHT:
            newX = mi.rcWork.right - windowWidth;
            break;
        case HOTKEY_DOWN:
            newY = mi.rcWork.bottom - windowHeight;
            break;
        case HOTKEY_UP:
            newY = mi.rcWork.top;
            break;
		case HOTKEY_MINIMIZE:
			minimizedWindow = hWnd;
			return ShowWindow(hWnd, SW_MINIMIZE);
		case HOTKEY_RESTORE:
			if (minimizedWindow == NULL) return true;
			bool ret = ShowWindow(minimizedWindow, SW_RESTORE);
			minimizedWindow = NULL;
			return ret;
    }

    // ウィンドウを新しい位置に移動
    return SetWindowPos(hWnd, NULL, newX, newY, 0, 0, SWP_NOSIZE | SWP_NOZORDER);
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