#pragma once
#include <string>

#define WM_NOTIFICATION (WM_USER + 100)
#define HOTKEY_LEFT 1
#define HOTKEY_DOWN 2
#define HOTKEY_UP 3
#define HOTKEY_RIGHT 4
#define HOTKEY_MINIMIZE 5
#define HOTKEY_RESTORE 6
#define HOTKEY_MAXIMIZE 7
#define HOTKEY_FOCUS_LEFT 8
#define HOTKEY_FOCUS_RIGHT 9
#define HOTKEY_FOCUS_UP 10
#define HOTKEY_FOCUS_DOWN 11

#define MENU_ACTIVATE 0x1
#define MENU_EXIT 0x2

#define NO_MINIMIZED_WINDOW 0x002

#define UNKNOWN_HOTKEY 0x2

// Used for configuration
enum Action {
	ACTION_MOVE_WINDOW_LEFT,
	ACTION_MOVE_WINDOW_DOWN,
	ACTION_MOVE_WINDOW_UP,
	ACTION_MOVE_WINDOW_RIGHT,
	ACTION_WINDOW_MINIMIZE,
	ACTION_WINDOW_RESTORE,
	ACTION_WINDOW_MAXIMIZE
};

// Default configuration text
const std::string DefaultConfig = "MoveWindowLeft=H\n"
								  "MoveWindowDown=J\n"
								  "MoveWindowUp=K\n"
								  "MoveWindowRight=L\n"
								  "MinimizeWindow=D\n"
								  "RestoreWindow=U\n"
								  "MaximizeWindow=G\n";		
