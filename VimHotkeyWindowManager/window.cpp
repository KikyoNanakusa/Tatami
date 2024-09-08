#include "Window.h"

// Global Lists of Monitors and Windows
Window *head_window = nullptr;

// Add the window to the global list by the HWND and the Monitor
// If the window is added successfully, return true
bool AddWindowToList(HWND hWnd, Monitor *monitor) {
	Window *current = head_window;
	head_window = new Window(hWnd, monitor);
	if (current != nullptr) {
		head_window->SetNextWindow(current);
	}

	return true;
}

// Find the window in the global list by the HWND
// If the window is found, return the window
// Otherwise, return nullptr
Window* FindWindowByHwnd(HWND hwnd) {
    Window *current = head_window;
    while (current) {
        if (current->hwnd == hwnd) {
            return current;
        }
        current = current->next_window; 
    }
    return nullptr; 
}

