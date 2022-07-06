/*
	THIS FILE IS A PART OF RDR 2 SCRIPT HOOK SDK
				http://dev-c.com
			(C) Alexander Blade 2019
*/

#include "keyboard.h"
#include <string>
#include "KeyConfig.h"

const int KEYS_SIZE = 255;

struct {
	DWORD time;
	BOOL isWithAlt;
	BOOL wasDownBefore;
	BOOL isUpNow;
} keyStates[KEYS_SIZE];

void OnKeyboardMessage(DWORD key, WORD repeats, BYTE scanCode, BOOL isExtended, BOOL isWithAlt, BOOL wasDownBefore, BOOL isUpNow)
{
	if (key < KEYS_SIZE)
	{
		keyStates[key].time = GetTickCount64();
		keyStates[key].isWithAlt = isWithAlt;
		keyStates[key].wasDownBefore = wasDownBefore;
		keyStates[key].isUpNow = isUpNow;
	}
}

const int NOW_PERIOD = 100, MAX_DOWN = 5000, MAX_DOWN_LONG = 30000; // ms

bool IsKeyDown(DWORD key)
{
	return (key < KEYS_SIZE) ? ((GetTickCount64() < keyStates[key].time + MAX_DOWN) && !keyStates[key].isUpNow) : false;
}
bool IsKeyDown(std::string keyName)
{
	KeyConfig* key = get_config()->get_key_config()->get_key(keyName);

	if (key->keyCodes.size() > 0)
	{
		// 如果配置了多键
		bool result = false;
		for (auto val : key->keyCodes)
		{
			result = IsKeyDown(val);
			if (result)	break;
		}		
		return result;
	}
	else 
	{
		if (key == NULL || key->keyCode == 0)
		{
			return false;
		}
		bool result = IsKeyDown(key->keyCode);
		if (result && key->modAlt)
		{
			result = (GetKeyState(VK_MENU) & 0x8000) != 0;
		}
		if (result && key->modShift)
		{
			result = (GetKeyState(VK_SHIFT) & 0x8000) != 0;
		}
		if (result && key->modCtrl)
		{
			result = (GetKeyState(VK_LCONTROL) & 0x8000 || GetKeyState(VK_RCONTROL) & 0x8000);
		}
		return result;
	}
}


bool IsKeyDownLong(DWORD key)
{
	return (key < KEYS_SIZE) ? ((GetTickCount64() < keyStates[key].time + MAX_DOWN_LONG) && !keyStates[key].isUpNow) : false;
}

void ResetKeyState(DWORD key)
{
	if (key < KEYS_SIZE)
		memset(&keyStates[key], 0, sizeof(keyStates[0]));
}

bool IsKeyJustUp(DWORD key, bool exclusive)
{
	bool b = (key < KEYS_SIZE) ? (GetTickCount64() < keyStates[key].time + NOW_PERIOD && keyStates[key].isUpNow) : false;
	if (b && exclusive)
		ResetKeyState(key);
	return b;
}

bool IsKeyJustUp(std::string keyName, bool exclusive)
{
	KeyConfig* key = get_config()->get_key_config()->get_key(keyName);

	if (key->keyCodes.size() > 0)
	{
		// 如果配置了多键
		bool result = false;
		for (auto val : key->keyCodes)
		{
			result = IsKeyJustUp(val);
			if (result)	break;
		}
		return result;
	}
	else
	{
		if (key == NULL || key->keyCode == 0)
		{
			return false;
		}

		bool result = IsKeyJustUp(key->keyCode, exclusive);
		if (result && key->modAlt)
		{
			result = (GetKeyState(VK_MENU) & 0x8000) != 0;
		}
		if (result && key->modShift)
		{
			result = (GetKeyState(VK_SHIFT) & 0x8000) != 0;
		}
		if (result && key->modCtrl)
		{
			result = (GetKeyState(VK_LCONTROL) & 0x8000 || GetKeyState(VK_RCONTROL) & 0x8000);
		}
		return result;
	}
}



int keyNameToVal(char* input)
{
	for (int i = 0; i < (sizeof ALL_KEYS / sizeof ALL_KEYS[0]); i++)
	{
		if (strcmp(input, ALL_KEYS[i].name) == 0)
		{
			return ALL_KEYS[i].keyCode;
		}
	}
	return -1;
}

int keyNameToVal(std::string input)
{
	for (int i = 0; i < (sizeof ALL_KEYS / sizeof ALL_KEYS[0]); i++)
	{
		if (strcmp(input.c_str(), ALL_KEYS[i].name) == 0)
		{
			return ALL_KEYS[i].keyCode;
		}
	}
	return -1;
}