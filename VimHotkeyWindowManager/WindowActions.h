#pragma once
#include <Windows.h>
#include <ShellScalingApi.h>
#include "definition.h"
#include "MonitorUtils.h"
#include "Window.h"
#include "Monitor.h"
#include "FocusUtils.h"

#pragma comment(lib, "Shcore.lib")

bool MoveFocusedWindow(int moveType, HWND& minimizedWindow);
bool MoveFocus(UINT moveType);
bool RestoreWindow(HWND& minimizedWindow);
bool AdjustWindowPosition(HWND hWnd, const MONITORINFO& mi, const RECT& windowRect, bool moveToRight);
