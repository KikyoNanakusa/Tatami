#pragma once
#include <Windows.h>
#include "definition.h"
#include "HotkeyManager.h"
#include "Config.h"

HWND InitWindow(HINSTANCE hInstance, WNDPROC lpfnWndProc, Config* config);
bool OnDestroy(HWND hWnd, NOTIFYICONDATA nid, HMENU hMenu);
