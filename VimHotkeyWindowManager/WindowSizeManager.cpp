#include "WindowSizeManager.h"

bool ResizeWindow(HWND hWnd, int width, int height)
{
	RECT rc = { 0, 0, width, height };
	AdjustWindowRect(&rc, WS_OVERLAPPEDWINDOW, FALSE);
	return true;
}
