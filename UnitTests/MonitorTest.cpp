#include "gtest/gtest.h"
#include "pch.h"
#include "Monitor.h"
#include "Monitor.cpp"

TEST(MonitorTest, TestConstractor) {
    // Get the handle of the desktop window
    HWND hwndDesktop = GetDesktopWindow();

    // Monitor of the desktop window is always the primary monitor
    HMONITOR hMainMonitor = MonitorFromWindow(hwndDesktop, MONITOR_DEFAULTTOPRIMARY);

	Monitor *monitor = new Monitor(hMainMonitor);
    EXPECT_EQ(monitor->hMonitor, hMainMonitor);
    EXPECT_EQ(monitor->mi.cbSize, sizeof(MONITORINFO));

    MONITORINFO mi = {};
    mi.cbSize = sizeof(mi);
    GetMonitorInfo(hMainMonitor, &mi);
    bool expectedIsPrimary = (mi.dwFlags & MONITORINFOF_PRIMARY) != 0;
    EXPECT_EQ(monitor->isPrimary, expectedIsPrimary);

    EXPECT_EQ(monitor->next_monitor, nullptr);
    EXPECT_EQ(monitor->prev_monitor, nullptr);
}

TEST(MonitorTest, FindMonitorByHmonitorReturnsCorrectMonitor) {
    // Get the handle of the desktop window
    HWND hwndDesktop = GetDesktopWindow();

    // Monitor of the desktop window is always the primary monitor
    HMONITOR hMainMonitor = MonitorFromWindow(hwndDesktop, MONITOR_DEFAULTTOPRIMARY);

    // Get the handle of the secondary monitor
    HMONITOR hSecondaryMonitor = MonitorFromWindow(GetDesktopWindow(), MONITOR_DEFAULTTONEAREST);

	primary_monitor = new Monitor(hMainMonitor);
	Monitor* secondary_monitor = new Monitor(hSecondaryMonitor);
	primary_monitor->SetNextMonitor(secondary_monitor);

    Monitor* foundMonitor = FindMonitorByHmonitor(hMainMonitor);
    EXPECT_NE(foundMonitor, nullptr);
    EXPECT_EQ(foundMonitor->hMonitor, hMainMonitor);

    Monitor* foundSecondaryMonitor = FindMonitorByHmonitor(hSecondaryMonitor);
    EXPECT_NE(foundSecondaryMonitor, nullptr);
    EXPECT_EQ(foundSecondaryMonitor->hMonitor, hSecondaryMonitor);

    // If the monitor is not found, return nullptr
    HMONITOR fakeMonitor = reinterpret_cast<HMONITOR>(0x12345678); // A fake monitor handle
    Monitor* notFoundMonitor = FindMonitorByHmonitor(fakeMonitor);
    EXPECT_EQ(notFoundMonitor, nullptr);
}

