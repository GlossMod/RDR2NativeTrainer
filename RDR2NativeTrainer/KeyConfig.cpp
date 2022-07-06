#include "keyboard.h"
#include "KeyConfig.h"
#include "Function.h"


#include <iostream>  
#include <fstream> 
#include "xinput.h"


// A global Windows "basic string". Actual memory is allocated by the
// COM methods used by MSXML which take &keyconf_bstr. We must use SysFreeString() 
// to free this memory before subsequent uses, to prevent a leak.
//BSTR keyconf_bstr;

TrainerConfig* config = NULL;

/*从文件读取配置.*/
void read_config_file()
{
	TrainerConfig* result = new TrainerConfig();

	CoInitialize(NULL);

	string ConfigFile = "RDR2NativeTrainer\\Config.json5";
	//打开文件
	std::ifstream file(ConfigFile, fstream::in | fstream::out);

	if (!file.is_open()) 
	{
		PrintSubtitle(("按键配置读取失败"));
		config = result; //使用默认配置
		return;
	}

	// 解析json用Json::Reader   
	Json::Reader reader;
	// Json::Value是一种很重要的类型，可以代表任意类型。如int, string, object, array...   
	json json;

	if (reader.parse(file, json))
	{
		try
		{
			//char *cr = NULL;
			// 键盘按键
			if (!json["hotKey"].isNull())
			{
				for (auto var : json["hotKey"].getMemberNames())
				{
					//result->get_key_config()->set_key((char*)var.c_str(), (char*)json["hotKey"][var].asString().c_str(), false, false, false);

					KeyConfig* buttonConfig = new KeyConfig();
					for (auto var2 : json["hotKey"][var])
					{
						buttonConfig->add_button(var2.asString());
						addLogs("写入按键配置{" + var + "}为：" + var2.asString());
					}
					result->get_key_config()->set_key((char*)var.c_str(), buttonConfig);
				}

			}

			// 手柄按键
			if (!json["controller"].isNull())
			{
				for (auto var : json["controller"].getMemberNames())
				{
					if (var == "EnableController")
					{
						ControllerButtonConfig::EnableController = json["controller"][var].asBool();
					}
					else 
					{

						ControllerButtonConfig* buttonConfig = new ControllerButtonConfig();

						for (auto var2 : json["controller"][var])
						{
							buttonConfig->add_button(var2.asString());

							addLogs("写入按键配置{" + var + "}为：" + var2.asString());
						}

						result->get_key_config()->set_control((char*)var.c_str(), buttonConfig);
					}
				}
			}
			PrintSubtitle(("用户配置写入完成"));
		}
		catch (const std::exception& e)
		{
			addLogs(e.what());
			PrintSubtitle(("初始化失败,将使用默认配置"));
			config = result; //使用默认配置
		}
	}
	else 
	{
		PrintSubtitle(("配置读取失败,Config.json5格式错误"));
	}
	file.close();

	// 写入用户配置
	config = result;
}

void KeyInputConfig::set_key(char* function, char* keyName, bool modCtrl, bool modAlt, bool modShift)
{
	//std::ostringstream ss;
	//ss << "Key function " << function << " being given " << keyName;
	//write_text_to_log_file(ss.str());

	int vkID = keyNameToVal(keyName);
	if (vkID == -1)
	{
		//ss.str(""); ss.clear();
		//ss << "Key function " << keyName << " didn't correspond to a value";
		//write_text_to_log_file(ss.str());
		return;
	}

	auto match = keyConfigs.find(function);
	if (match != keyConfigs.end())
	{
		KeyConfig* oldConfig = match->second;
		match->second = new KeyConfig(vkID);
		match->second->modCtrl = modCtrl;
		match->second->modAlt = modAlt;
		match->second->modShift = modShift;
		delete oldConfig;
	}
	else
	{
		//ss.str(""); ss.clear();
		//ss << "Key function " << function << " didn't correspond to a known function";
		//write_text_to_log_file(ss.str());
	}
};

void KeyInputConfig::set_key(char* function, KeyConfig* config, bool modCtrl, bool modAlt, bool modShift)
{
	auto match = keyConfigs.find(function);
	if (match != keyConfigs.end())
	{
		KeyConfig* oldConfig = match->second;
		match->second = config;
		match->second->modCtrl = modCtrl;
		match->second->modAlt = modAlt;
		match->second->modShift = modShift;
		delete oldConfig;
	}
}
void KeyConfig::add_button(char* name)
{
	int btn = keyNameToVal(name);
	this->keyCodes.push_back(btn);
}
void KeyConfig::add_button(std::string name)
{
	int btn = keyNameToVal((char*)name.c_str());
	this->keyCodes.push_back(btn);
}

KeyConfig* KeyInputConfig::get_key_button(std::string function)
{
	auto match = keyConfigs.find(function);
	if (match != keyConfigs.end())
	{
		return match->second;
	}
	return NULL;
}

void KeyInputConfig::set_control(char* function, ControllerButtonConfig* config)
{
	//std::ostringstream ss;
	//ss << "Controller function " << function << " being set, has " << config->buttonCodes.size() << " buttons within";
	//addLogs(ss.str());

	for (ButtonsWithNames btn : config->buttonCodes)
	{
		//std::ostringstream ss;
		//ss << "\tIncluded button " << btn.name << " and value " << btn.buttonCode;
		//addLogs(ss.str());
	}

	auto match = controllerConfigs.find(function);
	if (match != controllerConfigs.end())
	{
		ControllerButtonConfig* oldConfig = match->second;
		match->second = config;
		delete oldConfig;
	}
	else
	{
		//ss.str(""); ss.clear();
		//ss << "Controller function " << function << " didn't correspond to a known function";
		//addLogs(ss.str());
	}
}

bool KeyInputConfig::is_hotkey_assigned(int i)
{
	std::string target;
	switch (i)
	{
	case 1:
		target = KeyConfig::KEY_HOT_1;
		break;
	case 2:
		target = KeyConfig::KEY_HOT_2;
		break;
	case 3:
		target = KeyConfig::KEY_HOT_3;
		break;
	case 4:
		target = KeyConfig::KEY_HOT_4;
		break;
	case 5:
		target = KeyConfig::KEY_HOT_5;
		break;
	case 6:
		target = KeyConfig::KEY_HOT_6;
		break;
	case 7:
		target = KeyConfig::KEY_HOT_7;
		break;
	case 8:
		target = KeyConfig::KEY_HOT_8;
		break;
	case 9:
		target = KeyConfig::KEY_HOT_9;
		break;
	default:
		return false;
	}

	KeyConfig* conf = this->keyConfigs[target];
	if (conf == NULL || conf->keyCode == 0)
	{
		return false;
	}
	return true;
}

TrainerConfig::TrainerConfig()
{
	this->keyConfig = new KeyInputConfig();
}

// 默认配置
KeyInputConfig::KeyInputConfig()
{
	this->keyConfigs[KeyConfig::KEY_TOGGLE_MAIN_MENU] = new KeyConfig(VK_F3);

	this->keyConfigs[KeyConfig::KEY_MENU_UP] = new KeyConfig();
	this->keyConfigs[KeyConfig::KEY_MENU_UP]->add_button("VK_NUMPAD8");
	this->keyConfigs[KeyConfig::KEY_MENU_UP]->add_button("VK_UP");

	this->keyConfigs[KeyConfig::KEY_MENU_DOWN] = new KeyConfig();
	this->keyConfigs[KeyConfig::KEY_MENU_DOWN]->add_button("VK_NUMPAD2");
	this->keyConfigs[KeyConfig::KEY_MENU_DOWN]->add_button("VK_DOWN");

	this->keyConfigs[KeyConfig::KEY_MENU_LEFT] = new KeyConfig();
	this->keyConfigs[KeyConfig::KEY_MENU_LEFT]->add_button("VK_NUMPAD4");
	this->keyConfigs[KeyConfig::KEY_MENU_LEFT]->add_button("VK_LEFT");

	this->keyConfigs[KeyConfig::KEY_MENU_RIGHT] = new KeyConfig();
	this->keyConfigs[KeyConfig::KEY_MENU_RIGHT]->add_button("VK_NUMPAD6");
	this->keyConfigs[KeyConfig::KEY_MENU_RIGHT]->add_button("VK_RIGHT");

	this->keyConfigs[KeyConfig::KEY_MENU_SELECT] = new KeyConfig();
	this->keyConfigs[KeyConfig::KEY_MENU_SELECT]->add_button("VK_NUMPAD5");
	this->keyConfigs[KeyConfig::KEY_MENU_SELECT]->add_button("VK_RETURN");

	this->keyConfigs[KeyConfig::KEY_MENU_BACK] = new KeyConfig();
	this->keyConfigs[KeyConfig::KEY_MENU_BACK]->add_button("VK_NUMPAD0");
	this->keyConfigs[KeyConfig::KEY_MENU_BACK]->add_button("VK_BACK");

	this->keyConfigs[KeyConfig::KEY_VEH_BOOST] = new KeyConfig(VK_NUMPAD9);
	this->keyConfigs[KeyConfig::KEY_VEH_STOP] = new KeyConfig(VK_NUMPAD3);
	this->keyConfigs[KeyConfig::KEY_VEH_ROCKETS] = new KeyConfig(VK_ADD);

	this->keyConfigs[KeyConfig::KEY_TOGGLE_AIRBRAKE] = new KeyConfig(VK_F6);

	this->keyConfigs[KeyConfig::KEY_HOT_AIRBRAKE_THROUGH_DOOR] = new KeyConfig(VK_DIVIDE);
	this->keyConfigs[KeyConfig::KEY_HOT_AIRBRAKE_THROUGH_DOOR]->modCtrl = true;

	//this->keyConfigs[KeyConfig::KEY_AIRBRAKE_UP] = new KeyConfig(VK_KEY_Q);
	//this->keyConfigs[KeyConfig::KEY_AIRBRAKE_DOWN] = new KeyConfig(VK_KEY_Z);
	//this->keyConfigs[KeyConfig::KEY_AIRBRAKE_FORWARD] = new KeyConfig(VK_KEY_W);
	//this->keyConfigs[KeyConfig::KEY_AIRBRAKE_BACK] = new KeyConfig(VK_KEY_S);
	//this->keyConfigs[KeyConfig::KEY_AIRBRAKE_ROTATE_LEFT] = new KeyConfig(VK_KEY_A);
	//this->keyConfigs[KeyConfig::KEY_AIRBRAKE_ROTATE_RIGHT] = new KeyConfig(VK_KEY_D);
	//this->keyConfigs[KeyConfig::KEY_AIRBRAKE_SPEED] = new KeyConfig(VK_SHIFT);
	//this->keyConfigs[KeyConfig::KEY_AIRBRAKE_FREEZE_TIME] = new KeyConfig(VK_KEY_T);
	//this->keyConfigs[KeyConfig::KEY_AIRBRAKE_HELP] = new KeyConfig(VK_KEY_H);

	//this->keyConfigs[KeyConfig::KEY_OBJECTPLACER_UP] = new KeyConfig(VK_KEY_Q);
	//this->keyConfigs[KeyConfig::KEY_OBJECTPLACER_DOWN] = new KeyConfig(VK_KEY_Z);
	//this->keyConfigs[KeyConfig::KEY_OBJECTPLACER_FORWARD] = new KeyConfig(VK_KEY_W);
	//this->keyConfigs[KeyConfig::KEY_OBJECTPLACER_BACK] = new KeyConfig(VK_KEY_S);
	//this->keyConfigs[KeyConfig::KEY_OBJECTPLACER_ROTATE_LEFT] = new KeyConfig(VK_KEY_A);
	//this->keyConfigs[KeyConfig::KEY_OBJECTPLACER_ROTATE_RIGHT] = new KeyConfig(VK_KEY_D);
	//this->keyConfigs[KeyConfig::KEY_OBJECTPLACER_SPEED_UP] = new KeyConfig(VK_OEM_6);
	//this->keyConfigs[KeyConfig::KEY_OBJECTPLACER_SPEED_DOWN] = new KeyConfig(VK_OEM_4);
	//this->keyConfigs[KeyConfig::KEY_OBJECTPLACER_FREEZE_TIME] = new KeyConfig(VK_KEY_T);
	//this->keyConfigs[KeyConfig::KEY_OBJECTPLACER_HELP] = new KeyConfig(VK_KEY_H);
	//this->keyConfigs[KeyConfig::KEY_OBJECTPLACER_FREEZE_POSITION] = new KeyConfig(VK_KEY_G);
	//this->keyConfigs[KeyConfig::KEY_OBJECTPLACER_ALT_MOVE] = new KeyConfig(VK_MENU);

	this->keyConfigs[KeyConfig::KEY_HOT_1] = new KeyConfig(VK_KEY_T);
	this->keyConfigs[KeyConfig::KEY_HOT_2] = new KeyConfig(VK_KEY_K);
	this->keyConfigs[KeyConfig::KEY_HOT_3] = new KeyConfig(VK_F9);
	this->keyConfigs[KeyConfig::KEY_HOT_4] = new KeyConfig(0);
	this->keyConfigs[KeyConfig::KEY_HOT_5] = new KeyConfig(0);
	this->keyConfigs[KeyConfig::KEY_HOT_6] = new KeyConfig(0);
	this->keyConfigs[KeyConfig::KEY_HOT_7] = new KeyConfig(0);
	this->keyConfigs[KeyConfig::KEY_HOT_8] = new KeyConfig(0);
	this->keyConfigs[KeyConfig::KEY_HOT_9] = new KeyConfig(0);

	// 默认手柄配置
	this->controllerConfigs[KeyConfig::KEY_TOGGLE_MAIN_MENU] = new ControllerButtonConfig();
	this->controllerConfigs[KeyConfig::KEY_TOGGLE_MAIN_MENU]->add_button(ControllerButtonConfig::INPUT_FRONTEND_Y);
	this->controllerConfigs[KeyConfig::KEY_TOGGLE_MAIN_MENU]->add_button(ControllerButtonConfig::INPUT_FRONTEND_RS);	// Y + 右摇杆

	this->controllerConfigs[KeyConfig::KEY_MENU_UP] = new ControllerButtonConfig();
	this->controllerConfigs[KeyConfig::KEY_MENU_UP]->add_button(ControllerButtonConfig::INPUT_FRONTEND_PAD_UP);			// 方向键 上
	this->controllerConfigs[KeyConfig::KEY_MENU_DOWN] = new ControllerButtonConfig();
	this->controllerConfigs[KeyConfig::KEY_MENU_DOWN]->add_button(ControllerButtonConfig::INPUT_FRONTEND_PAD_DOWN);		// 方向键 下
	this->controllerConfigs[KeyConfig::KEY_MENU_LEFT] = new ControllerButtonConfig();
	this->controllerConfigs[KeyConfig::KEY_MENU_LEFT]->add_button(ControllerButtonConfig::INPUT_FRONTEND_PAD_LEFT);		// 方向键 左
	this->controllerConfigs[KeyConfig::KEY_MENU_RIGHT] = new ControllerButtonConfig();
	this->controllerConfigs[KeyConfig::KEY_MENU_RIGHT]->add_button(ControllerButtonConfig::INPUT_FRONTEND_PAD_RIGHT);	// 方向键 右
	this->controllerConfigs[KeyConfig::KEY_MENU_SELECT] = new ControllerButtonConfig();
	this->controllerConfigs[KeyConfig::KEY_MENU_SELECT]->add_button(ControllerButtonConfig::INPUT_FRONTEND_ACCEPT);		// A 键确认
	this->controllerConfigs[KeyConfig::KEY_MENU_BACK] = new ControllerButtonConfig();
	this->controllerConfigs[KeyConfig::KEY_MENU_BACK]->add_button(ControllerButtonConfig::INPUT_FRONTEND_CANCEL);		// B 键返回

	this->controllerConfigs[KeyConfig::KEY_VEH_BOOST] = new ControllerButtonConfig();
	this->controllerConfigs[KeyConfig::KEY_VEH_BOOST]->add_button(ControllerButtonConfig::INPUT_FRONTEND_RT);
	this->controllerConfigs[KeyConfig::KEY_VEH_BOOST]->add_button(ControllerButtonConfig::INPUT_FRONTEND_RB);			// RT + RB 载具加速

	this->controllerConfigs[KeyConfig::KEY_VEH_STOP] = new ControllerButtonConfig();
	this->controllerConfigs[KeyConfig::KEY_VEH_STOP]->add_button(ControllerButtonConfig::INPUT_FRONTEND_LT);
	this->controllerConfigs[KeyConfig::KEY_VEH_STOP]->add_button(ControllerButtonConfig::INPUT_FRONTEND_LB);			// LT + LB 载具减速

	this->controllerConfigs[KeyConfig::KEY_VEH_ROCKETS] = new ControllerButtonConfig();
	this->controllerConfigs[KeyConfig::KEY_VEH_ROCKETS]->add_button(ControllerButtonConfig::INPUT_FRONTEND_LS);
	this->controllerConfigs[KeyConfig::KEY_VEH_ROCKETS]->add_button(ControllerButtonConfig::INPUT_FRONTEND_RS);			// LS + RS 发射RPG

	this->controllerConfigs[KeyConfig::KEY_TOGGLE_AIRBRAKE] = new ControllerButtonConfig();
	this->controllerConfigs[KeyConfig::KEY_TOGGLE_AIRBRAKE]->add_button(ControllerButtonConfig::INPUT_FRONTEND_ACCEPT);
	this->controllerConfigs[KeyConfig::KEY_TOGGLE_AIRBRAKE]->add_button(ControllerButtonConfig::INPUT_FRONTEND_RS);		// A + RS 忘了干嘛的了

	//this->controllerConfigs[KeyConfig::KEY_AIRBRAKE_UP] = new ControllerButtonConfig();
	//this->controllerConfigs[KeyConfig::KEY_AIRBRAKE_UP]->add_button(ControllerButtonConfig::CONTROLLER_BTN_TRIGGER_L);
	//this->controllerConfigs[KeyConfig::KEY_AIRBRAKE_DOWN] = new ControllerButtonConfig();
	//this->controllerConfigs[KeyConfig::KEY_AIRBRAKE_DOWN]->add_button(ControllerButtonConfig::CONTROLLER_BTN_TRIGGER_R);

	//this->controllerConfigs[KeyConfig::KEY_AIRBRAKE_FORWARD] = new ControllerButtonConfig();
	//this->controllerConfigs[KeyConfig::KEY_AIRBRAKE_FORWARD]->add_button(ControllerButtonConfig::CONTROLLER_LSTICK_U);
	//this->controllerConfigs[KeyConfig::KEY_AIRBRAKE_BACK] = new ControllerButtonConfig();
	//this->controllerConfigs[KeyConfig::KEY_AIRBRAKE_BACK]->add_button(ControllerButtonConfig::CONTROLLER_LSTICK_D);

	//this->controllerConfigs[KeyConfig::KEY_AIRBRAKE_ROTATE_LEFT] = new ControllerButtonConfig();
	//this->controllerConfigs[KeyConfig::KEY_AIRBRAKE_ROTATE_LEFT]->add_button(ControllerButtonConfig::CONTROLLER_LSTICK_L);
	//this->controllerConfigs[KeyConfig::KEY_AIRBRAKE_ROTATE_RIGHT] = new ControllerButtonConfig();
	//this->controllerConfigs[KeyConfig::KEY_AIRBRAKE_ROTATE_RIGHT]->add_button(ControllerButtonConfig::CONTROLLER_LSTICK_R);

	//this->controllerConfigs[KeyConfig::KEY_AIRBRAKE_SPEED] = new ControllerButtonConfig();
	//this->controllerConfigs[KeyConfig::KEY_AIRBRAKE_SPEED]->add_button(ControllerButtonConfig::CONTROLLER_BTN_A);
	//this->controllerConfigs[KeyConfig::KEY_AIRBRAKE_FREEZE_TIME] = new ControllerButtonConfig();
	//this->controllerConfigs[KeyConfig::KEY_AIRBRAKE_FREEZE_TIME]->add_button(ControllerButtonConfig::CONTROLLER_BTN_B);

	//this->controllerConfigs[KeyConfig::KEY_OBJECTPLACER_UP] = new ControllerButtonConfig();
	//this->controllerConfigs[KeyConfig::KEY_OBJECTPLACER_UP]->add_button(ControllerButtonConfig::CONTROLLER_BTN_TRIGGER_L);
	//this->controllerConfigs[KeyConfig::KEY_OBJECTPLACER_DOWN] = new ControllerButtonConfig();
	//this->controllerConfigs[KeyConfig::KEY_OBJECTPLACER_DOWN]->add_button(ControllerButtonConfig::CONTROLLER_BTN_TRIGGER_R);

	//this->controllerConfigs[KeyConfig::KEY_OBJECTPLACER_FORWARD] = new ControllerButtonConfig();
	//this->controllerConfigs[KeyConfig::KEY_OBJECTPLACER_FORWARD]->add_button(ControllerButtonConfig::CONTROLLER_LSTICK_U);
	//this->controllerConfigs[KeyConfig::KEY_OBJECTPLACER_BACK] = new ControllerButtonConfig();
	//this->controllerConfigs[KeyConfig::KEY_OBJECTPLACER_BACK]->add_button(ControllerButtonConfig::CONTROLLER_LSTICK_D);

	//this->controllerConfigs[KeyConfig::KEY_OBJECTPLACER_ROTATE_LEFT] = new ControllerButtonConfig();
	//this->controllerConfigs[KeyConfig::KEY_OBJECTPLACER_ROTATE_LEFT]->add_button(ControllerButtonConfig::CONTROLLER_LSTICK_L);
	//this->controllerConfigs[KeyConfig::KEY_OBJECTPLACER_ROTATE_RIGHT] = new ControllerButtonConfig();
	//this->controllerConfigs[KeyConfig::KEY_OBJECTPLACER_ROTATE_RIGHT]->add_button(ControllerButtonConfig::CONTROLLER_LSTICK_R);

	//this->controllerConfigs[KeyConfig::KEY_OBJECTPLACER_SPEED_CYCLE] = new ControllerButtonConfig();
	//this->controllerConfigs[KeyConfig::KEY_OBJECTPLACER_SPEED_CYCLE]->add_button(ControllerButtonConfig::CONTROLLER_BTN_A);
	//this->controllerConfigs[KeyConfig::KEY_OBJECTPLACER_FREEZE_TIME] = new ControllerButtonConfig();
	//this->controllerConfigs[KeyConfig::KEY_OBJECTPLACER_FREEZE_TIME]->add_button(ControllerButtonConfig::CONTROLLER_BTN_B);
	//this->controllerConfigs[KeyConfig::KEY_OBJECTPLACER_FREEZE_POSITION] = new ControllerButtonConfig();
	//this->controllerConfigs[KeyConfig::KEY_OBJECTPLACER_FREEZE_POSITION]->add_button(ControllerButtonConfig::CONTROLLER_BTN_Y);

	this->controllerConfigs[KeyConfig::KEY_HOT_1] = new ControllerButtonConfig();
	this->controllerConfigs[KeyConfig::KEY_HOT_2] = new ControllerButtonConfig();
	this->controllerConfigs[KeyConfig::KEY_HOT_3] = new ControllerButtonConfig();
	this->controllerConfigs[KeyConfig::KEY_HOT_4] = new ControllerButtonConfig();
	this->controllerConfigs[KeyConfig::KEY_HOT_5] = new ControllerButtonConfig();
	this->controllerConfigs[KeyConfig::KEY_HOT_6] = new ControllerButtonConfig();
	this->controllerConfigs[KeyConfig::KEY_HOT_7] = new ControllerButtonConfig();
	this->controllerConfigs[KeyConfig::KEY_HOT_8] = new ControllerButtonConfig();
	this->controllerConfigs[KeyConfig::KEY_HOT_9] = new ControllerButtonConfig();
}

KeyInputConfig::~KeyInputConfig()
{
	for (std::pair<std::string, KeyConfig*> conf : this->keyConfigs)
	{
		delete conf.second;
	}
	keyConfigs.clear();
}

KeyConfig* KeyInputConfig::get_key(std::string function)
{
	auto match = keyConfigs.find(function);
	if (match != keyConfigs.end())
	{
		return match->second;
	}
	return NULL;
}

ControllerButtonConfig* KeyInputConfig::get_controller_button(std::string function)
{
	auto match = controllerConfigs.find(function);
	if (match != controllerConfigs.end())
	{
		return match->second;
	}
	return NULL;
}

void ControllerButtonConfig::add_button(char* name)
{
	ButtonsWithNames btn = buttonNameToVal(name);
	this->buttonCodes.push_back(btn);
}

void ControllerButtonConfig::add_button(std::string name)
{
	ButtonsWithNames btn = buttonNameToVal((char*)name.c_str());
	this->buttonCodes.push_back(btn);
}

const std::string KeyConfig::KEY_TOGGLE_MAIN_MENU = std::string("toggle_main_menu");
const std::string KeyConfig::KEY_TOGGLE_AIRBRAKE = std::string("toggle_airbrake");

const std::string KeyConfig::KEY_MENU_UP = std::string("menu_up");
const std::string KeyConfig::KEY_MENU_DOWN = std::string("menu_down");
const std::string KeyConfig::KEY_MENU_LEFT = std::string("menu_left");
const std::string KeyConfig::KEY_MENU_RIGHT = std::string("menu_right");
const std::string KeyConfig::KEY_MENU_SELECT = std::string("menu_select");
const std::string KeyConfig::KEY_MENU_BACK = std::string("menu_back");

const std::string KeyConfig::KEY_VEH_BOOST = std::string("veh_boost");
const std::string KeyConfig::KEY_VEH_STOP = std::string("veh_stop");
const std::string KeyConfig::KEY_VEH_ROCKETS = std::string("veh_rockets");

//const std::string KeyConfig::KEY_AIRBRAKE_UP = std::string("airbrake_up");
//const std::string KeyConfig::KEY_AIRBRAKE_DOWN = std::string("airbrake_down");
//const std::string KeyConfig::KEY_AIRBRAKE_FORWARD = std::string("airbrake_forward");
//const std::string KeyConfig::KEY_AIRBRAKE_BACK = std::string("airbrake_back");
//const std::string KeyConfig::KEY_AIRBRAKE_ROTATE_LEFT = std::string("airbrake_rotate_left");
//const std::string KeyConfig::KEY_AIRBRAKE_ROTATE_RIGHT = std::string("airbrake_rotate_right");
//const std::string KeyConfig::KEY_AIRBRAKE_SPEED = std::string("airbrake_speed");
//const std::string KeyConfig::KEY_AIRBRAKE_FREEZE_TIME = std::string("airbrake_freeze_time");
//const std::string KeyConfig::KEY_AIRBRAKE_HELP = std::string("airbrake_help");
//const std::string KeyConfig::KEY_OBJECTPLACER_UP = std::string("objectplacer_up");
//const std::string KeyConfig::KEY_OBJECTPLACER_DOWN = std::string("objectplacer_down");
//const std::string KeyConfig::KEY_OBJECTPLACER_FORWARD = std::string("objectplacer_forward");
//const std::string KeyConfig::KEY_OBJECTPLACER_BACK = std::string("objectplacer_back");
//const std::string KeyConfig::KEY_OBJECTPLACER_ROTATE_LEFT = std::string("objectplacer_rotate_left");
//const std::string KeyConfig::KEY_OBJECTPLACER_ROTATE_RIGHT = std::string("objectplacer_rotate_right");
//const std::string KeyConfig::KEY_OBJECTPLACER_SPEED_CYCLE = std::string("objectplacer_speed_cycle");
//const std::string KeyConfig::KEY_OBJECTPLACER_SPEED_UP = std::string("objectplacer_speed_up");
//const std::string KeyConfig::KEY_OBJECTPLACER_SPEED_DOWN = std::string("objectplacer_speed_down");
//const std::string KeyConfig::KEY_OBJECTPLACER_FREEZE_TIME = std::string("objectplacer_freeze_time");
//const std::string KeyConfig::KEY_OBJECTPLACER_FREEZE_POSITION = std::string("objectplacer_freeze_position");
//const std::string KeyConfig::KEY_OBJECTPLACER_HELP = std::string("objectplacer_help");
//const std::string KeyConfig::KEY_OBJECTPLACER_ALT_MOVE = std::string("objectplacer_alt_move");
const std::string KeyConfig::KEY_HOT_AIRBRAKE_THROUGH_DOOR = std::string("hotkey_throughdoor");

const std::string KeyConfig::KEY_HOT_1 = std::string("hotkey_1");
const std::string KeyConfig::KEY_HOT_2 = std::string("hotkey_2");
const std::string KeyConfig::KEY_HOT_3 = std::string("hotkey_3");
const std::string KeyConfig::KEY_HOT_4 = std::string("hotkey_4");
const std::string KeyConfig::KEY_HOT_5 = std::string("hotkey_5");
const std::string KeyConfig::KEY_HOT_6 = std::string("hotkey_6");
const std::string KeyConfig::KEY_HOT_7 = std::string("hotkey_7");
const std::string KeyConfig::KEY_HOT_8 = std::string("hotkey_8");
const std::string KeyConfig::KEY_HOT_9 = std::string("hotkey_9");

// 手柄按键
//const bool ControllerButtonConfig::controller = false;
bool ControllerButtonConfig::EnableController = true;	// 是否启用手柄

//const std::string ControllerButtonConfig::CONTROLLER_BTN_X = std::string("BUTTON_X");			// X
//const std::string ControllerButtonConfig::CONTROLLER_BTN_Y = std::string("BUTTON_Y");			// Y
//const std::string ControllerButtonConfig::CONTROLLER_BTN_A = std::string("BUTTON_A");			// A
//const std::string ControllerButtonConfig::CONTROLLER_BTN_B = std::string("BUTTON_B");			// B
//const std::string ControllerButtonConfig::CONTROLLER_BTN_DPAD_L = std::string("DPAD_LEFT");		// 左摇杆
//const std::string ControllerButtonConfig::CONTROLLER_BTN_DPAD_R = std::string("DPAD_RIGHT");	// 右摇杆
//const std::string ControllerButtonConfig::CONTROLLER_BTN_DPAD_U = std::string("DPAD_UP");		// 上
//const std::string ControllerButtonConfig::CONTROLLER_BTN_DPAD_D = std::string("DPAD_DOWN");		// 下
//const std::string ControllerButtonConfig::CONTROLLER_BTN_SHOULDER_L = std::string("L_SHOULDER");// LS
//const std::string ControllerButtonConfig::CONTROLLER_BTN_SHOULDER_R = std::string("R_SHOULDER");// RS
//const std::string ControllerButtonConfig::CONTROLLER_BTN_TRIGGER_L = std::string("L_TRIGGER");	// LT
//const std::string ControllerButtonConfig::CONTROLLER_BTN_TRIGGER_R = std::string("R_TRIGGER");	// RT
//const std::string ControllerButtonConfig::CONTROLLER_BTN_BACK = std::string("BACK");			// 返回
//const std::string ControllerButtonConfig::CONTROLLER_BTN_START = std::string("START");			// 暂停
//const std::string ControllerButtonConfig::CONTROLLER_LSTICK_L = std::string("L_STICK_LEFT");	// 左摇杆左移
//const std::string ControllerButtonConfig::CONTROLLER_LSTICK_R = std::string("L_STICK_RIGHT");	// 左摇杆右移
//const std::string ControllerButtonConfig::CONTROLLER_LSTICK_U = std::string("L_STICK_UP");		// 左摇杆上移
//const std::string ControllerButtonConfig::CONTROLLER_LSTICK_D = std::string("L_STICK_DOWN");	// 左摇杆下移
//const std::string ControllerButtonConfig::CONTROLLER_RSTICK_L = std::string("R_STICK_LEFT");	// 右摇杆左移
//const std::string ControllerButtonConfig::CONTROLLER_RSTICK_R = std::string("R_STICK_RIGHT");	// 右摇杆右移
//const std::string ControllerButtonConfig::CONTROLLER_RSTICK_U = std::string("R_STICK_UP");		// 右摇杆上移
//const std::string ControllerButtonConfig::CONTROLLER_RSTICK_D = std::string("R_STICK_DOWN");	// 右摇杆下移
//const std::string ControllerButtonConfig::CONTROLLER_LSTICK_CLICK = std::string("L_STICK_CLICK");// 左摇杆按下
//const std::string ControllerButtonConfig::CONTROLLER_RSTICK_CLICK = std::string("R_STICK_CLICK");// 右摇杆按下
//const std::string ControllerButtonConfig::CONTROLLER_NOT_BOUND = std::string("NOT_BOUND");		// 无约束

const std::string ControllerButtonConfig::INPUT_FRONTEND_DOWN = std::string("FRONTEND_DOWN");
const std::string ControllerButtonConfig::INPUT_FRONTEND_UP = std::string("FRONTEND_UP");
const std::string ControllerButtonConfig::INPUT_FRONTEND_PAD_UP = std::string("FRONTEND_PAD_UP");					// 方向键 上
const std::string ControllerButtonConfig::INPUT_FRONTEND_PAD_DOWN = std::string("FRONTEND_PAD_DOWN");				// 方向键 下
const std::string ControllerButtonConfig::INPUT_FRONTEND_PAD_LEFT = std::string("FRONTEND_PAD_LEFT");				// 方向键 左
const std::string ControllerButtonConfig::INPUT_FRONTEND_PAD_RIGHT = std::string("FRONTEND_PAD_RIGHT");				// 方向键 右
const std::string ControllerButtonConfig::INPUT_FRONTEND_LEFT = std::string("FRONTEND_LEFT");
const std::string ControllerButtonConfig::INPUT_FRONTEND_RIGHT = std::string("FRONTEND_RIGHT");
const std::string ControllerButtonConfig::INPUT_FRONTEND_RUP = std::string("FRONTEND_RUP");							// 右摇杆 上（可能
const std::string ControllerButtonConfig::INPUT_FRONTEND_RDOWN = std::string("FRONTEND_RDOWN");						// 右摇杆 下（可能
const std::string ControllerButtonConfig::INPUT_FRONTEND_RLEFT = std::string("FRONTEND_RLEFT");						// 右摇杆 左（可能
const std::string ControllerButtonConfig::INPUT_FRONTEND_RRIGHT = std::string("FRONTEND_RRIGHT");					// 右摇杆 右（可能

const std::string ControllerButtonConfig::INPUT_FRONTEND_LUP = std::string("FRONTEND_LUP");							// 左摇杆 上（可能
const std::string ControllerButtonConfig::INPUT_FRONTEND_LDOWN = std::string("FRONTEND_LDOWN");						// 左摇杆 下（可能
const std::string ControllerButtonConfig::INPUT_FRONTEND_LLEFT = std::string("FRONTEND_LLEFT");						// 左摇杆 左（可能
const std::string ControllerButtonConfig::INPUT_FRONTEND_LRIGHT = std::string("FRONTEND_LRIGHT");					// 左摇杆 右（可能

const std::string ControllerButtonConfig::INPUT_FRONTEND_AXIS_X = std::string("FRONTEND_AXIS_X");					// 左摇杆 X轴（可能
const std::string ControllerButtonConfig::INPUT_FRONTEND_AXIS_Y = std::string("FRONTEND_AXIS_Y");					// 左摇杆 X轴（可能
const std::string ControllerButtonConfig::INPUT_FRONTEND_RIGHT_AXIS_X = std::string("FRONTEND_RIGHT_AXIS_X");		// 右摇杆 X轴
const std::string ControllerButtonConfig::INPUT_FRONTEND_RIGHT_AXIS_Y = std::string("FRONTEND_RIGHT_AXIS_Y");		// 右摇杆 Y轴
const std::string ControllerButtonConfig::INPUT_FRONTEND_PAUSE = std::string("FRONTEND_PAUSE");						// 暂停键	
const std::string ControllerButtonConfig::INPUT_FRONTEND_PAUSE_ALTERNATE = std::string("FRONTEND_PAUSE_ALTERNATE");	// 暂停键2
const std::string ControllerButtonConfig::INPUT_FRONTEND_ACCEPT = std::string("FRONTEND_ACCEPT");					// A 键
const std::string ControllerButtonConfig::INPUT_FRONTEND_CANCEL = std::string("FRONTEND_CANCEL");					// B 键
const std::string ControllerButtonConfig::INPUT_FRONTEND_X = std::string("FRONTEND_X");								// X 键
const std::string ControllerButtonConfig::INPUT_FRONTEND_Y = std::string("FRONTEND_Y");								// Y 键
const std::string ControllerButtonConfig::INPUT_FRONTEND_LB = std::string("FRONTEND_LB");							// LB 键
const std::string ControllerButtonConfig::INPUT_FRONTEND_RB = std::string("FRONTEND_RB");							// RB 键
const std::string ControllerButtonConfig::INPUT_FRONTEND_LT = std::string("FRONTEND_LT");							// LT 键
const std::string ControllerButtonConfig::INPUT_FRONTEND_RT = std::string("FRONTEND_RT");							// RT 键
const std::string ControllerButtonConfig::INPUT_FRONTEND_LS = std::string("FRONTEND_LS");							// LS 左摇杆
const std::string ControllerButtonConfig::INPUT_FRONTEND_RS = std::string("FRONTEND_RS");							// RS 右摇杆