#pragma once
#include <Windows.h>
#include "definition.h"

bool MoveFocusedWindow(int moveType, HWND minimizedWindow);
bool minimizeWindow(HWND hWnd, HWND minimizedWindow);
bool restoreWindow(HWND minimizedWindow);
bool AdjustWindowPosition(HWND hWnd, const MONITORINFO& mi, const RECT& windowRect, bool moveToRight);
bool MoveWindowToLeft(HWND hWnd, const MONITORINFO& mi, const RECT& windowRect);
bool MoveWindowToRight(HWND hWnd, const MONITORINFO& mi, const RECT& windowRect);
