#pragma once
#include <Windows.h>
#include "Monitor.h"

struct Window;

enum HorizontalAlignment {
    HA_FREE,
    HA_LEFT,
    HA_CENTER,
    HA_RIGHT
};

enum VerticalAlignment {
    VA_FREE,
    VA_TOP,
    VA_CENTER,
    VA_BOTTOM
};

struct Window {
    HWND hwnd;
    Monitor *monitor;

    Window* left_window = nullptr;
    Window* right_window = nullptr;
    Window* top_window = nullptr;
    Window* bottom_window = nullptr;

    HorizontalAlignment horizontal_alignment = HA_FREE;
    VerticalAlignment vertical_alignment = VA_FREE;
    bool isMaximized = false;

    Window* next_window = nullptr;
    Window* prev_window = nullptr;

    Window(HWND hwnd, Monitor* monitor) 
        : hwnd(hwnd), monitor(monitor) {}

    void SetLeftWindow(Window* window) {
        left_window = window;
        if (window) {
            window->right_window = this;
        }
    }

    void SetRightWindow(Window* window) {
        right_window = window;
        if (window) {
            window->left_window = this;
        }
    }

    void SetTopWindow(Window* window) {
        top_window = window;
        if (window) {
            window->bottom_window = this;
        }
    }

    void SetBottomWindow(Window* window) {
        bottom_window = window;
        if (window) {
            window->top_window = this;
        }
    }

    void SetNextWindow(Window* next) {
        next_window = next;
        if (next) {
            next->prev_window = this;
        }
    }

    void SetPrevWindow(Window* prev) {
        prev_window = prev;
        if (prev) {
            prev->next_window = this;
        }
    }
};

extern Window *head_window;

bool AddWindowToList(HWND hWnd, Monitor* monitor);
Window* FindWindowByHwnd(HWND hwnd);

