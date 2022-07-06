#include "Weather.h"

#include "scriptinfo.h"

#include <unordered_map>
#include <ctime>
#include <tchar.h>
#include <windows.h>

/// <summary>
/// 选择天气
/// </summary>
class MenuItemWeatherSelect : public MenuItemDefault
{
	string m_name;
	
	virtual void OnSelect()
	{
		GAMEPLAY::CLEAR_OVERRIDE_WEATHER();
		Hash weather = GAMEPLAY::GET_HASH_KEY(const_cast<char*>(m_name.c_str()));
		GAMEPLAY::SET_WEATHER_TYPE(weather, TRUE, TRUE, FALSE, 0.0, FALSE);
		GAMEPLAY::CLEAR_WEATHER_TYPE_PERSIST();
	}
public:

	MenuItemWeatherSelect(string caption, string weaponName)
		: MenuItemDefault(caption),
		m_name(weaponName) {}
};

/// <summary>
/// 冻结天气
/// </summary>
class MenuItemWeatherFreeze : public MenuItemSwitchable
{
	virtual void OnSelect()
	{
		bool newstate = !GetState();
		if (!newstate)
			GAMEPLAY::FREEZE_WEATHER(false);
		SetState(newstate);
	}
	virtual void OnFrame()
	{
		if (GetState())
			GAMEPLAY::FREEZE_WEATHER(true);
	}
public:
	MenuItemWeatherFreeze(string caption)
		: MenuItemSwitchable(caption) {}
};

/// <summary>
/// 台风
/// </summary>
class MenuItemWeatherWind : public MenuItemSwitchable
{
	virtual void OnSelect()
	{
		bool newstate = !GetState();
		if (newstate)
		{
			GAMEPLAY::SET_WIND_SPEED(50.0);
			GAMEPLAY::SET_WIND_DIRECTION(ENTITY::GET_ENTITY_HEADING(PLAYER::PLAYER_PED_ID()));
		}
		else
		{
			GAMEPLAY::SET_WIND_SPEED(0.0);
		}
		SetState(newstate);
	}
public:
	MenuItemWeatherWind(string caption)
		: MenuItemSwitchable(caption) {}
};

/// <summary>
/// 天气列表
/// </summary>
MenuBase* CreateWeatherMenu(MenuController* controller)
{
	auto menu = new MenuBase(new MenuItemListTitle(("天气")));
	controller->RegisterMenu(menu);

	menu->AddItem(new MenuItemWeatherFreeze(("冻结天气")));
	menu->AddItem(new MenuItemWeatherWind(("台风")));

	for (auto var : weather)
	{
		menu->AddItem(new MenuItemWeatherSelect(var.name, var.code));
	}


	return menu;
}
