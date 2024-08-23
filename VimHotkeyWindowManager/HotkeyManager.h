#pragma once
#include <Windows.h>
#include <string>
#include "definition.h"
#include "Config.h"

void RegisterHotKeys(HWND hWnd, Config* config);
void UnRegisterHotKeys(HWND hWnd);

