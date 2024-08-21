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

#define MENU_ACTIVATE 0x1
#define MENU_EXIT 0x2

#define NO_MINIMIZED_WINDOW 0x002

#define UNKNOWN_HOTKEY 0x2

#define ACTION_MOVE_LEFT 0x1
#define ACTION_MOVE_DOWN 0x2
#define ACTION_MOVE_UP 0x3
#define ACTION_MOVE_RIGHT 0x4
#define ACTION_MINIMIZE 0x5
#define ACTION_RESTORE 0x6
#define ACTION_MAXIMIZE 0x7

const std::string DefaultConfig = "MoveWindowLeft=H\n"
								  "MoveWindowDown=J\n"
								  "MoveWindowUp=K\n"
								  "MoveWindowRight=L\n"
								  "MinimizeWindow=D\n"
								  "RestoreWindow=U\n"
								  "MaximizeWindow=G\n";		
