#include "gtest/gtest.h"
#include "pch.h"
#include "Window.h"
#include "Window.cpp"

TEST(WindowStructTests, Constructor) {
	HMONITOR hMonitor = (HMONITOR)1;
	HWND hWnd = (HWND)2;
	Monitor *monitor = new Monitor(hMonitor);
	Window window = Window(hWnd, monitor);

	EXPECT_EQ(hWnd, window.hWnd);
	EXPECT_EQ(monitor, window.monitor);
}

// [NOTE]:AddWindowToList add window to the head of the global list
TEST(WindowStructTests, AddWindow) {
	HMONITOR hMonitor = (HMONITOR)1;
	HWND hWnd = (HWND)2;
	Monitor *monitor = new Monitor(hMonitor);
	Window window = Window(hWnd, monitor);

	head_window = &window;

	HWND hWnd2 = (HWND)4;
	HMONITOR hMonitor2 = (HMONITOR)3;
	Monitor *monitor2 = new Monitor(hMonitor2);

	AddWindowToList((HWND)4, monitor2);

	EXPECT_EQ(hWnd2, head_window->hWnd);
	EXPECT_EQ(monitor2, head_window->monitor);
	EXPECT_EQ(&window, head_window->next_window);
}

TEST(WindowStructTests, FindWindowByHwnd) {
	HMONITOR hMonitor = (HMONITOR)1;
	HWND hWnd = (HWND)2;
	Monitor *monitor = new Monitor(hMonitor);
	Window window = Window(hWnd, monitor);

	head_window = &window;

	EXPECT_EQ(&window, FindWindowByHwnd(hWnd));

	// if the window is not found, return nullptr
	EXPECT_EQ(nullptr, FindWindowByHwnd((HWND)3));
}
