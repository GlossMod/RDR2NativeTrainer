#pragma once

#include "inc\natives.h"
#include "inc\types.h"
#include "inc\enums.h"
#include "inc\main.h"

#include "scriptmenu.h"
#include "keyboard.h"

#include "Function.h"

#include <windows.h>

std::string GT(const string& strGBK);

MenuBase* CreatePlayerMenu(MenuController* controller);