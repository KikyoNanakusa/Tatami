#pragma once
#include <Windows.h>
#include <ShellScalingApi.h>
#include "definition.h"
#include "MonitorUtils.h"
#include "Window.h"
#include "Monitor.h"

#pragma comment(lib, "Shcore.lib")

bool MoveFocusedWindow(int moveType, HWND& minimizedWindow);
bool MinimizeWindow(HWND hWnd, HWND& minimizedWindow);
bool RestoreWindow(HWND& minimizedWindow);
bool AdjustWindowPosition(HWND hWnd, const MONITORINFO& mi, const RECT& windowRect, bool moveToRight);
bool MoveWindowToLeft(HWND hWnd, const MONITORINFO& mi, const RECT& windowRect);
bool MoveWindowToRight(HWND hWnd, const MONITORINFO& mi, const RECT& windowRect);
