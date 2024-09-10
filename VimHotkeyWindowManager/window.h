#pragma once
#include <Windows.h>
#include "Monitor.h"

struct Window;

enum HorizontalAlignment {
    HA_FREE,
    HA_LEFT,
    HA_CENTER,
    HA_RIGHT,
    HA_MAXIMIZE,
};

enum VerticalAlignment {
    VA_FREE,
    VA_TOP,
    VA_CENTER,
    VA_BOTTOM,
    VA_MAXIMIZE,
};

struct Window {
    HWND hWnd;
    Monitor *monitor;

    HorizontalAlignment horizontal_alignment = HA_FREE;
    VerticalAlignment vertical_alignment = VA_FREE;
    bool isMaximized = false;

    Window* next_window = nullptr;
    Window* prev_window = nullptr;

    Window(HWND hwnd, Monitor* monitor);

    void SetNextWindow(Window* next);
    void SetPrevWindow(Window* prev);
};

// Used in global scope
extern Window *head_window;

bool AddWindowToList(HWND hWnd, Monitor* monitor);
Window* FindWindowByHwnd(HWND hwnd);

