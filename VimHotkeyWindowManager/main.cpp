#include <Windows.h>
#include <string>
#include "definition.h"
#include "MonitorUtils.h"
#include "WindowManager.h"
#include "HotkeyManager.h"
#include "WindowActions.h"
#include "resource.h"
#include "ConfigReader.h"
#include "Config.h"
#include "Window.h"
#include "Monitor.h"


HMENU hMenu;
NOTIFYICONDATA nid;
HWND minimizedWindow = NULL;
BOOL isEnabled = TRUE;

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

void InitTaskIcon(HWND hWnd) {
	ZeroMemory(&nid, sizeof(nid));
	nid.cbSize = sizeof(NOTIFYICONDATA);
	nid.hWnd = hWnd;
	nid.uID = 1;
	nid.uFlags = NIF_ICON | NIF_MESSAGE | NIF_TIP;
	nid.uCallbackMessage = WM_NOTIFICATION;
	nid.hIcon = LoadIcon(GetModuleHandle(NULL), MAKEINTRESOURCE(IDI_ICON1));
	lstrcpy(nid.szTip, TEXT("VimHotkeyWindowManager"));
	Shell_NotifyIcon(NIM_ADD, &nid);
}


int WINAPI WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPSTR lpCmdLine, _In_ int nCmdShow)
{
	HWND hWnd;
	MSG msg;

	// モニター情報を取得
	if (!InitializeMonitorList()) {
		MessageBox(NULL, TEXT("Failed to initialize monitor list"), TEXT("ERROR"), MB_OK);
		return 1;
	}

	// 設定ファイルを読む
	std::string configString = ReadConfigFile();
	if (configString.empty()) {
		MessageBox(NULL, TEXT("Failed to read config file"), TEXT("ERROR"), MB_OK);
		return 1;
	}

	// 設定ファイルを解析
	Config config;
	if (!config.loadConfig(configString)) {
		MessageBox(NULL, TEXT("Failed to parse config file"), TEXT("ERROR"), MB_OK);
		return 1;
	}

	//ウィンドウを初期化
	hWnd = InitWindow(hInstance, WndProc, &config);

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

	switch (message) {
		case WM_CREATE:
			// Make scope for pCreate, Config
			{
				CREATESTRUCT* pCreate = reinterpret_cast<CREATESTRUCT*>(lParam);
				Config* config = reinterpret_cast<Config*>(pCreate->lpCreateParams);

				if (!config) {
					MessageBox(hWnd, L"Invalid config pointer", L"Error", MB_ICONERROR);
					PostQuitMessage(1);
					return -1;
				}

				// Register hotkey
				RegisterHotKeys(hWnd, config);
			}

			// Create context menu
			hMenu = CreatePopupMenu();
			AppendMenu(hMenu, MF_STRING, MENU_ACTIVATE, TEXT("無効にする"));
			AppendMenu(hMenu, MF_STRING, MENU_EXIT, TEXT("終了"));
			break;

		case WM_HOTKEY:
			if (isEnabled) {
				if (isMoveWindowHotkey(wParam)) {
					MoveFocusedWindow(wParam, minimizedWindow);
				}
				else {
					MoveFocus(wParam);
				}
			}
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
				OnDestroy(hWnd, nid, hMenu);
				PostQuitMessage(0);
				break;
			case MENU_ACTIVATE:
				if (isEnabled) {
					// メニュー項目を「無効にする」に変更
					ModifyMenu(hMenu, MENU_ACTIVATE, MF_STRING, MENU_ACTIVATE, TEXT("有効にする"));
				}
				else {
					// メニュー項目を「有効にする」に変更
					ModifyMenu(hMenu, MENU_ACTIVATE, MF_STRING, MENU_ACTIVATE, TEXT("無効にする"));
				}
				isEnabled = !isEnabled; // 状態を反転
				break;
			}
			break;
		case WM_DESTROY:
			OnDestroy(hWnd, nid, hMenu);
			break;
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}