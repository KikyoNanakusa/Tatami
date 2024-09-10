#include "Window.h"

// Global Lists of Monitors and Windows
Window *head_window = nullptr;


// Constructor of the Window struct
// Initialize the window with the HWND and the Monitor
Window::Window(HWND hwnd, Monitor* monitor)
	: hWnd(hwnd), monitor(monitor) {}

// Set the next window of the current window
void Window::SetNextWindow(Window* next) {
	next_window = next;
	if (next) {
		next->prev_window = this;
	}
}

// Set the previous window of the current window
void Window::SetPrevWindow(Window* prev) {
	prev_window = prev;
	if (prev) {
		prev->next_window = this;
	}
}


// Helper functions 

// Add the window to the global list by the HWND and the Monitor
// If the window is added successfully, return true
bool AddWindowToList(HWND hWnd, Monitor* monitor) {
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
        if (current->hWnd == hwnd) {
            return current;
        }
        current = current->next_window; 
    }
    return nullptr; 
}
