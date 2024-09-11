#pragma once
#include <Windows.h>
#include "Window.h"
#include "Monitor.h"

BOOL SetForegroundWhenRestored(Window *window);
BOOL SetCursorToLeft(Monitor *monitor);
BOOL SetCursorToRight(Monitor *monitor);
BOOL SetCursorToTop(Monitor *monitor);
BOOL SetCursorToBottom(Monitor *monitor);
BOOL SetCursorToTopLeft(Monitor *monitor);
BOOL SetCursorToTopRight(Monitor *monitor);
BOOL SetCursorToBottomLeft(Monitor *monitor);
BOOL SetCursorToBottomRight(Monitor *monitor);


