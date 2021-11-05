#pragma once
#include <stdio.h>
#include <tchar.h>
#include <windows.h>


#include <map>
#include <vector>
#include "keyboard.h"
#include <string>

#include "json/json/json.h"
using json = Json::Value;

// 按键配置
class KeyConfig
{
public:

	static const std::string KEY_TOGGLE_MAIN_MENU;
	static const std::string KEY_TOGGLE_AIRBRAKE;

	static const std::string KEY_MENU_UP;
	static const std::string KEY_MENU_DOWN;
	static const std::string KEY_MENU_LEFT;
	static const std::string KEY_MENU_RIGHT;
	static const std::string KEY_MENU_SELECT;
	static const std::string KEY_MENU_BACK;

	static const std::string KEY_VEH_BOOST;
	static const std::string KEY_VEH_STOP;
	static const std::string KEY_VEH_ROCKETS;

	//static const std::string KEY_AIRBRAKE_UP;
	//static const std::string KEY_AIRBRAKE_DOWN;
	//static const std::string KEY_AIRBRAKE_ROTATE_LEFT;
	//static const std::string KEY_AIRBRAKE_ROTATE_RIGHT;
	//static const std::string KEY_AIRBRAKE_FORWARD;
	//static const std::string KEY_AIRBRAKE_BACK;
	//static const std::string KEY_AIRBRAKE_SPEED;
	//static const std::string KEY_AIRBRAKE_FREEZE_TIME;
	//static const std::string KEY_AIRBRAKE_HELP;

	//static const std::string KEY_OBJECTPLACER_UP;
	//static const std::string KEY_OBJECTPLACER_DOWN;
	//static const std::string KEY_OBJECTPLACER_FORWARD;
	//static const std::string KEY_OBJECTPLACER_BACK;
	//static const std::string KEY_OBJECTPLACER_ROTATE_LEFT;
	//static const std::string KEY_OBJECTPLACER_ROTATE_RIGHT;
	//static const std::string KEY_OBJECTPLACER_SPEED_CYCLE;
	//static const std::string KEY_OBJECTPLACER_SPEED_UP;
	//static const std::string KEY_OBJECTPLACER_SPEED_DOWN;
	//static const std::string KEY_OBJECTPLACER_FREEZE_TIME;
	//static const std::string KEY_OBJECTPLACER_FREEZE_POSITION;
	//static const std::string KEY_OBJECTPLACER_HELP;
	//static const std::string KeyConfig::KEY_OBJECTPLACER_ALT_MOVE;

	static const std::string KEY_HOT_AIRBRAKE_THROUGH_DOOR;

	static const std::string KEY_HOT_1;
	static const std::string KEY_HOT_2;
	static const std::string KEY_HOT_3;
	static const std::string KEY_HOT_4;
	static const std::string KEY_HOT_5;
	static const std::string KEY_HOT_6;
	static const std::string KEY_HOT_7;
	static const std::string KEY_HOT_8;
	static const std::string KEY_HOT_9;

	inline KeyConfig(int code)
	{
		this->keyCode = code;
	};
	inline KeyConfig()
	{

	}

	std::vector<int> keyCodes;

	void add_button(char* name);

	void add_button(std::string name);

	int keyCode;
	bool modCtrl = false;
	bool modAlt = false;
	bool modShift = false;
};

struct ButtonsWithNames
{
	std::string name;
	WORD buttonCode;
	//bool isAnalog;
};

// 手柄按键配置
class ControllerButtonConfig
{
public:
	static bool EnableController;
	//static const std::string CONTROLLER_BTN_X;
	//static const std::string CONTROLLER_BTN_Y;
	//static const std::string CONTROLLER_BTN_A;
	//static const std::string CONTROLLER_BTN_B;
	//static const std::string CONTROLLER_BTN_DPAD_L;
	//static const std::string CONTROLLER_BTN_DPAD_R;
	//static const std::string CONTROLLER_BTN_DPAD_U;
	//static const std::string CONTROLLER_BTN_DPAD_D;
	//static const std::string CONTROLLER_BTN_SHOULDER_L;
	//static const std::string CONTROLLER_BTN_SHOULDER_R;
	//static const std::string CONTROLLER_BTN_TRIGGER_L;
	//static const std::string CONTROLLER_BTN_TRIGGER_R;
	//static const std::string CONTROLLER_BTN_BACK;
	//static const std::string CONTROLLER_BTN_START;
	//static const std::string CONTROLLER_LSTICK_L;
	//static const std::string CONTROLLER_LSTICK_R;
	//static const std::string CONTROLLER_LSTICK_U;
	//static const std::string CONTROLLER_LSTICK_D;
	//static const std::string CONTROLLER_RSTICK_L;
	//static const std::string CONTROLLER_RSTICK_R;
	//static const std::string CONTROLLER_RSTICK_U;
	//static const std::string CONTROLLER_RSTICK_D;
	//static const std::string CONTROLLER_LSTICK_CLICK;
	//static const std::string CONTROLLER_RSTICK_CLICK;
	//static const std::string CONTROLLER_NOT_BOUND;

	static const std::string INPUT_FRONTEND_DOWN;
	static const std::string INPUT_FRONTEND_UP ;
	static const std::string INPUT_FRONTEND_PAD_DOWN;
	static const std::string INPUT_FRONTEND_PAD_UP ;
	static const std::string INPUT_FRONTEND_PAD_LEFT;
	static const std::string INPUT_FRONTEND_PAD_RIGHT ;
	static const std::string INPUT_FRONTEND_LEFT ;
	static const std::string INPUT_FRONTEND_RIGHT ;
	static const std::string INPUT_FRONTEND_RDOWN ;
	static const std::string INPUT_FRONTEND_RUP ;
	static const std::string INPUT_FRONTEND_RLEFT;
	static const std::string INPUT_FRONTEND_RRIGHT ;
	static const std::string INPUT_FRONTEND_LDOWN ;
	static const std::string INPUT_FRONTEND_LUP ;
	static const std::string INPUT_FRONTEND_LLEFT;
	static const std::string INPUT_FRONTEND_LRIGHT;
	static const std::string INPUT_FRONTEND_AXIS_X ;
	static const std::string INPUT_FRONTEND_AXIS_Y;
	static const std::string INPUT_FRONTEND_RIGHT_AXIS_X ;
	static const std::string INPUT_FRONTEND_RIGHT_AXIS_Y ;
	static const std::string INPUT_FRONTEND_PAUSE ;
	static const std::string INPUT_FRONTEND_PAUSE_ALTERNATE ;
	static const std::string INPUT_FRONTEND_ACCEPT ;
	static const std::string INPUT_FRONTEND_CANCEL;
	static const std::string INPUT_FRONTEND_X ;
	static const std::string INPUT_FRONTEND_Y ;
	static const std::string INPUT_FRONTEND_LB ;
	static const std::string INPUT_FRONTEND_RB;
	static const std::string INPUT_FRONTEND_LT ;
	static const std::string INPUT_FRONTEND_RT ;
	static const std::string INPUT_FRONTEND_LS ;
	static const std::string INPUT_FRONTEND_RS;

	static const std::string KEY_HOT_1;
	static const std::string KEY_HOT_2;
	static const std::string KEY_HOT_3;
	static const std::string KEY_HOT_4;
	static const std::string KEY_HOT_5;
	static const std::string KEY_HOT_6;
	static const std::string KEY_HOT_7;
	static const std::string KEY_HOT_8;
	static const std::string KEY_HOT_9;

	inline ControllerButtonConfig()
	{
	};

	void add_button(char* name);

	void add_button(std::string name);

	std::vector<ButtonsWithNames> buttonCodes;
};

/*一个用于保存当前键绑定的类.*/
class KeyInputConfig
{
public:
	KeyInputConfig();

	virtual ~KeyInputConfig();

	std::map<std::string, KeyConfig*> keyConfigs;

	std::map<std::string, ControllerButtonConfig*> controllerConfigs;

	KeyConfig* get_key(std::string function);

	ControllerButtonConfig* get_controller_button(std::string function);

	/*使用功能字符串和键字符串更改键绑定.*/
	void set_key(char* function, char* keyName, bool modCtrl = false, bool modAlt = false, bool modShift = false);

	void set_key(char* function, KeyConfig* config, bool modCtrl = false, bool modAlt = false, bool modShift = false);

	/*使用功能字符串和键字符串更改键绑定.*/
	void set_control(char* function, ControllerButtonConfig* config);

	bool is_hotkey_assigned(int i);

	KeyConfig* get_key_button(std::string function);

};

/*包含所有用户设置的类.*/
class TrainerConfig
{
public:
	TrainerConfig();
	KeyInputConfig* get_key_config() { return keyConfig; }

private:
	KeyInputConfig* keyConfig;
};

/*当前用户配置.*/
extern TrainerConfig* config;

/*从文件中读取配置.*/
void read_config_file();

/*获取当前的配置对象.*/
inline TrainerConfig* get_config() { return config; }


