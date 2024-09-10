#pragma once
#include <Windows.h>
#include <string>
#include "definition.h"
#include "Config.h"

bool isMoveWindowHotkey(UINT message);
void RegisterHotKeys(HWND hWnd, Config* config);
void UnRegisterHotKeys(HWND hWnd);

