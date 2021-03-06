/*
		THIS FILE IS A PART OF GTA V SCRIPT HOOK SDK
					http://dev-c.com
				(C) Alexander Blade 2015
*/

#pragma once

#include <windows.h>
//#include "config_io.h"
#include "inc\xinput\CXBOXController.h"

#include "inc\natives.h"
#include "inc\types.h"
#include "inc\enums.h"
#include "inc\main.h"
#include "KeyConfig.h"

static const WORD XINPUT_L_STICK_UP = 1;
static const WORD XINPUT_L_STICK_DOWN = 2;
static const WORD XINPUT_L_STICK_LEFT = 3;
static const WORD XINPUT_L_STICK_RIGHT = 4;
static const WORD XINPUT_R_STICK_UP = 5;
static const WORD XINPUT_R_STICK_DOWN = 6;
static const WORD XINPUT_R_STICK_LEFT = 7;
static const WORD XINPUT_R_STICK_RIGHT = 8;
static const WORD XINPUT_L_TRIGGER = 9;
static const WORD XINPUT_R_TRIGGER = 10;

#define ENT_XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE  11000 
#define ENT_XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE 8689
#define ENT_XINPUT_GAMEPAD_TRIGGER_THRESHOLD    30

class XboxControl {
public:
	static const WORD INPUT_FRONTEND_DOWN = 97156178;
	static const WORD INPUT_FRONTEND_UP = 1662638961;
	static const WORD INPUT_FRONTEND_PAD_DOWN = 416407838;
	static const WORD INPUT_FRONTEND_PAD_UP = 1714166746;
	static const WORD INPUT_FRONTEND_PAD_LEFT = 575419516;
	static const WORD INPUT_FRONTEND_PAD_RIGHT = 2362840370;
	static const WORD INPUT_FRONTEND_LEFT = 2791226036;
	static const WORD INPUT_FRONTEND_RIGHT = 3736290067;
	static const WORD INPUT_FRONTEND_RDOWN = 1463068996;
	static const WORD INPUT_FRONTEND_RUP = 3621677854;
	static const WORD INPUT_FRONTEND_RLEFT = 959670863;
	static const WORD INPUT_FRONTEND_RRIGHT = 1531509048;
	static const WORD INPUT_FRONTEND_LDOWN = 2595139801;
	static const WORD INPUT_FRONTEND_LUP = 3861192820;
	static const WORD INPUT_FRONTEND_LLEFT = 493286666;
	static const WORD INPUT_FRONTEND_LRIGHT = 915256155;
	static const WORD INPUT_FRONTEND_AXIS_X = 4216773979;
	static const WORD INPUT_FRONTEND_AXIS_Y = 152139984;
	static const WORD INPUT_FRONTEND_RIGHT_AXIS_X = 1025725594;
	static const WORD INPUT_FRONTEND_RIGHT_AXIS_Y = 3946918111;
	static const WORD INPUT_FRONTEND_PAUSE = 3626896338;
	static const WORD INPUT_FRONTEND_PAUSE_ALTERNATE = 1250966545;
	static const WORD INPUT_FRONTEND_ACCEPT = 3350541322;
	static const WORD INPUT_FRONTEND_CANCEL = 359624985;
	static const WORD INPUT_FRONTEND_X = 1840825903;
	static const WORD INPUT_FRONTEND_Y = 2080465600;
	static const WORD INPUT_FRONTEND_LB = 3901091606;
	static const WORD INPUT_FRONTEND_RB = 398377320;
	static const WORD INPUT_FRONTEND_LT = 1360019509;
	static const WORD INPUT_FRONTEND_RT = 1877832124;
	static const WORD INPUT_FRONTEND_LS = 1137550768;
	static const WORD INPUT_FRONTEND_RS = 2107936042;
};

static const ButtonsWithNames ALL_BUTTONS[] =
{
	{ ControllerButtonConfig::INPUT_FRONTEND_DOWN,				XboxControl::INPUT_FRONTEND_DOWN },
	{ ControllerButtonConfig::INPUT_FRONTEND_UP,				XboxControl::INPUT_FRONTEND_UP },
	{ ControllerButtonConfig::INPUT_FRONTEND_PAD_DOWN,			XboxControl::INPUT_FRONTEND_PAD_DOWN },
	{ ControllerButtonConfig::INPUT_FRONTEND_PAD_UP,			XboxControl::INPUT_FRONTEND_PAD_UP },
	{ ControllerButtonConfig::INPUT_FRONTEND_PAD_LEFT,			XboxControl::INPUT_FRONTEND_PAD_LEFT },
	{ ControllerButtonConfig::INPUT_FRONTEND_PAD_RIGHT,			XboxControl::INPUT_FRONTEND_PAD_RIGHT },
	{ ControllerButtonConfig::INPUT_FRONTEND_LEFT,				XboxControl::INPUT_FRONTEND_LEFT },
	{ ControllerButtonConfig::INPUT_FRONTEND_RIGHT,				XboxControl::INPUT_FRONTEND_RIGHT },
	{ ControllerButtonConfig::INPUT_FRONTEND_RDOWN,				XboxControl::INPUT_FRONTEND_RDOWN },
	{ ControllerButtonConfig::INPUT_FRONTEND_RUP,				XboxControl::INPUT_FRONTEND_RUP },
	{ ControllerButtonConfig::INPUT_FRONTEND_RLEFT,				XboxControl::INPUT_FRONTEND_RLEFT },
	{ ControllerButtonConfig::INPUT_FRONTEND_RRIGHT,			XboxControl::INPUT_FRONTEND_RRIGHT },
	{ ControllerButtonConfig::INPUT_FRONTEND_LDOWN,				XboxControl::INPUT_FRONTEND_LDOWN },
	{ ControllerButtonConfig::INPUT_FRONTEND_LUP,				XboxControl::INPUT_FRONTEND_LUP },
	{ ControllerButtonConfig::INPUT_FRONTEND_LLEFT,				XboxControl::INPUT_FRONTEND_LLEFT },
	{ ControllerButtonConfig::INPUT_FRONTEND_LRIGHT,			XboxControl::INPUT_FRONTEND_LRIGHT },
	{ ControllerButtonConfig::INPUT_FRONTEND_AXIS_X,			XboxControl::INPUT_FRONTEND_AXIS_X },
	{ ControllerButtonConfig::INPUT_FRONTEND_AXIS_Y,			XboxControl::INPUT_FRONTEND_AXIS_Y },
	{ ControllerButtonConfig::INPUT_FRONTEND_RIGHT_AXIS_X,		XboxControl::INPUT_FRONTEND_RIGHT_AXIS_X },
	{ ControllerButtonConfig::INPUT_FRONTEND_RIGHT_AXIS_Y,		XboxControl::INPUT_FRONTEND_RIGHT_AXIS_Y },
	{ ControllerButtonConfig::INPUT_FRONTEND_PAUSE,				XboxControl::INPUT_FRONTEND_PAUSE },
	{ ControllerButtonConfig::INPUT_FRONTEND_PAUSE_ALTERNATE,	XboxControl::INPUT_FRONTEND_PAUSE_ALTERNATE },
	{ ControllerButtonConfig::INPUT_FRONTEND_ACCEPT,			XboxControl::INPUT_FRONTEND_ACCEPT },
	{ ControllerButtonConfig::INPUT_FRONTEND_CANCEL,			XboxControl::INPUT_FRONTEND_CANCEL },
	{ ControllerButtonConfig::INPUT_FRONTEND_X,					XboxControl::INPUT_FRONTEND_X },
	{ ControllerButtonConfig::INPUT_FRONTEND_Y,					XboxControl::INPUT_FRONTEND_Y },
	{ ControllerButtonConfig::INPUT_FRONTEND_LB,				XboxControl::INPUT_FRONTEND_LB },
	{ ControllerButtonConfig::INPUT_FRONTEND_RB,				XboxControl::INPUT_FRONTEND_RB },
	{ ControllerButtonConfig::INPUT_FRONTEND_LT,				XboxControl::INPUT_FRONTEND_LT },
	{ ControllerButtonConfig::INPUT_FRONTEND_RT,				XboxControl::INPUT_FRONTEND_RT },
	{ ControllerButtonConfig::INPUT_FRONTEND_LS,				XboxControl::INPUT_FRONTEND_LS },
	{ ControllerButtonConfig::INPUT_FRONTEND_RS,				XboxControl::INPUT_FRONTEND_RS }
};

void init_xinput();

void end_xinput();

bool IsControllerButtonDown(std::string function);

bool IsControllerButtonJustUp(std::string btnName);

//bool IsAnalogControlPressed(int ourID, XINPUT_STATE state);

bool UpdateXInputControlState();

ButtonsWithNames buttonNameToVal(char * input);