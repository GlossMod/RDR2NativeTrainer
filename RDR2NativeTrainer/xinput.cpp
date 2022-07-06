/*
		THIS FILE IS A PART OF GTA V SCRIPT HOOK SDK
		http://dev-c.com
		(C) Alexander Blade 2015
		*/

#include "xinput.h"
//#include "..\features\script.h"
//#include "..\features\misc.h"

#include <sstream>
#include "Function.h"

CXBOXController* controller;

XINPUT_STATE state;
XINPUT_STATE prevState;

void init_xinput()
{
	controller = new CXBOXController(1);
	addLogs("初始化手柄配置");
}

void end_xinput()
{
	delete controller;
}

bool IsControllerButtonDown(std::string btnName)
{
	if (!ControllerButtonConfig::EnableController)
	{
		return false;
	}
	ControllerButtonConfig* buttonConf = get_config()->get_key_config()->get_controller_button(btnName);

	bool result = false;

	for  (ButtonsWithNames btn : buttonConf->buttonCodes)
	{
		WORD code = btn.buttonCode;
		result = PAD::IS_CONTROL_PRESSED(0, code);

		//addLogs("code=" + to_string(code));
	}

	return result;


}

bool IsControllerButtonJustUp(std::string btnName)
{
	//addLogs("监听手柄按下");
	if (!ControllerButtonConfig::EnableController)
	{
		return false;
	}

	ControllerButtonConfig* buttonConf = get_config()->get_key_config()->get_controller_button(btnName);
	
	bool result = false;
	for (ButtonsWithNames btn : buttonConf->buttonCodes)
	{
		WORD code = btn.buttonCode;
		result = PAD::IS_CONTROL_JUST_RELEASED(0, code);

		//addLogs("code=" + to_string(code));
	}

	return result;
}

bool UpdateXInputControlState()
{
	if (controller == NULL || !controller->IsConnected())
	{
		return false;
	}

	XINPUT_STATE newState = controller->GetState();
	if (newState.dwPacketNumber > prevState.dwPacketNumber)
	{
		prevState = state;
		state = newState;
		return true;
	}

	return false;
}

ButtonsWithNames buttonNameToVal(char * input)
{
	std::string inputS = std::string(input);
	for (ButtonsWithNames button : ALL_BUTTONS)
	{
		if (button.name.compare(inputS) == 0)
		{
			return button;
		}
	}
	return ButtonsWithNames{ std::string(""), 0};
}
//
//bool IsAnalogControlPressed(int ourID, XINPUT_STATE state)
//{
//	if (ourID == XINPUT_L_TRIGGER)
//	{
//		/*
//		std::ostringstream ss;
//		ss << "Left trigger: " << state.Gamepad.bLeftTrigger << " vs " << ENT_XINPUT_GAMEPAD_TRIGGER_THRESHOLD;
//		set_status_text_centre_screen(ss.str());
//		*/
//
//		return (state.Gamepad.bLeftTrigger && state.Gamepad.bLeftTrigger > ENT_XINPUT_GAMEPAD_TRIGGER_THRESHOLD);
//	}
//	else if (ourID == XINPUT_R_TRIGGER)
//	{
//		return (state.Gamepad.bRightTrigger && state.Gamepad.bRightTrigger > ENT_XINPUT_GAMEPAD_TRIGGER_THRESHOLD);
//	}
//	else if (ourID == XINPUT_L_STICK_DOWN)
//	{
//		/*
//		std::ostringstream ss;
//		ss << "Left stick down: " << state.Gamepad.sThumbLY << " vs " << ENT_XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE;
//		set_status_text_centre_screen(ss.str());*/
//
//		return /*(state.Gamepad.sThumbLX < ENT_XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE && 
//			state.Gamepad.sThumbLX > -ENT_XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE &&*/
//			(state.Gamepad.sThumbLY < -ENT_XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE );
//	}
//	else if (ourID == XINPUT_L_STICK_UP)
//	{
//		return /*(state.Gamepad.sThumbLX < ENT_XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE &&
//			state.Gamepad.sThumbLX > -ENT_XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE &&*/
//			(state.Gamepad.sThumbLY > ENT_XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE);
//	}
//	else if (ourID == XINPUT_L_STICK_LEFT)
//	{
//		return (state.Gamepad.sThumbLX < -ENT_XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE);/* &&
//			state.Gamepad.sThumbLY > -ENT_XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE &&
//			state.Gamepad.sThumbLY < ENT_XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE);*/
//	}
//	else if (ourID == XINPUT_L_STICK_RIGHT)
//	{
//		return (state.Gamepad.sThumbLX > ENT_XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE);/* &&
//			state.Gamepad.sThumbLY > -ENT_XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE &&
//			state.Gamepad.sThumbLY < ENT_XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE);*/
//	}
//	else if (ourID == XINPUT_R_STICK_DOWN)
//	{
//		return (state.Gamepad.sThumbRX < ENT_XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE &&
//			state.Gamepad.sThumbRX > -ENT_XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE &&
//			state.Gamepad.sThumbRY < -ENT_XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE);
//	}
//	else if (ourID == XINPUT_R_STICK_UP)
//	{
//		return (state.Gamepad.sThumbRX < ENT_XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE &&
//			state.Gamepad.sThumbRX > -ENT_XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE &&
//			state.Gamepad.sThumbRY > ENT_XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE);
//	}
//	else if (ourID == XINPUT_R_STICK_LEFT)
//	{
//		return (state.Gamepad.sThumbRX < -ENT_XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE &&
//			state.Gamepad.sThumbRY > -ENT_XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE &&
//			state.Gamepad.sThumbRY < ENT_XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE);
//	}
//	else if (ourID == XINPUT_R_STICK_RIGHT)
//	{
//		return (state.Gamepad.sThumbRX > ENT_XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE &&
//			state.Gamepad.sThumbRY > -ENT_XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE &&
//			state.Gamepad.sThumbRY < ENT_XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE);
//	}
//	return false;
//}