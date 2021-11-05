#pragma once

#include "inc\natives.h"
#include "inc\types.h"
#include "inc\enums.h"
#include "inc\main.h"

#include "scriptmenu.h"
#include "keyboard.h"

#include "Function.h"

#include <windows.h>
#include <sstream>
#include <iomanip>

std::string GT(const string& strGBK);

MenuBase* CreateTimeMenu(MenuController* controller);
MenuBase* CreateMiscMenu(MenuController* controller);