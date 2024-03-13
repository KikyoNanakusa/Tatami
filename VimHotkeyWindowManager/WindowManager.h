#pragma once
#include <Windows.h>
#include "definition.h"

HWND InitWindow(HINSTANCE hInstance, WNDPROC lpfnWndProc);
bool OnDestroy(HWND hWnd, NOTIFYICONDATA nid, HMENU hMenu);
