#include "word.h"

#include "scriptinfo.h"

#include <unordered_map>
#include <ctime>
#include <tchar.h>
#include <windows.h>
#include "Weather.h"
#include "Misc.h"




// 设置雪覆盖等级
class MenuItemWordSnow : public MenuItemDefault
{
	int level;
	string title;
	virtual void OnSelect()
	{
		//GAMEPLAY::_SET_SNOW_LEVEL(level);
		GRAPHICS::_SET_SNOW_COVERAGE_TYPE(level);
		SetStatusText(title);

	}

public:
	MenuItemWordSnow(string caption, int value)
		: MenuItemDefault(caption),
		level(value), title(caption){}
};

// 下雪选项
MenuBase* CreateSnowMenu(MenuController* controller)
{
	auto menu = new MenuBase(new MenuItemListTitle(("下雪选项")));
	controller->RegisterMenu(menu);

	menu->AddItem(new MenuItemWordSnow(("关闭"), 1));
	menu->AddItem(new MenuItemWordSnow(("小雪覆盖"), 2));
	menu->AddItem(new MenuItemWordSnow(("大雪覆盖"), 3));

	return menu;
}

// 没有行人
class CreateWordNobody : public MenuItemSwitchable
{	
	virtual void OnFrame()
	{
		if (GetState())
		{
			STREAMING::SET_POPULATION_BUDGET_MULTIPLIER(0);

			runOnAllNearbyPedsToPlayer([](auto ped) {
				//GET_SCREEN_COORD_FROM_WORLD_COORD
				//Vector3 pos = ENTITY::GET_ENTITY_COORDS(ped, false, false/*idk*/);	// 获取ped坐标		//				
				//ENTITY::DELETE_ENTITY(&ped);
				if (ENTITY::DOES_ENTITY_EXIST(ped) && (ped != PLAYER::GET_PLAYER_PED(PLAYER::PLAYER_ID())))		// PED存在且不是玩家
				{
					//Vector3 pos = ENTITY::GET_ENTITY_COORDS(ped, false, false/*idk*/);	// 获取ped坐标
					//float x, y;
					//GRAPHICS::GET_SCREEN_COORD_FROM_WORLD_COORD(pos.x, pos.y, pos.z, &x, &y);	//全局获取坐标位置
					//drawBoxOutline(x, y, 0.01f, 0.002f, 255, 0, 0, 255);	// 绘制红框

					//Ped pedHandle = PED::CLONE_PED(ped, true, true, true);  // 复制ped数据到pedHandle
					//PED::DELETE_PED(&pedHandle);
					//ENTITY::DELETE_ENTITY(&pedHandle);
					ENTITY::SET_ENTITY_HEALTH(ped, 0, true);	// 将PED杀死
					ENTITY::SET_ENTITY_COORDS(ped, 0, 0, -10000, 0, 0, 1, FALSE);	// 将PED丢到地底下去
				}
			});
			Vector3 pos = ENTITY::GET_ENTITY_COORDS(PLAYER::PLAYER_PED_ID(), false, false);	// 获取ped坐标

			Vector3 v1 = { pos.x - 1000, pos.y - 1000, pos.z - 1000 };
			Vector3 v2 = { pos.x + 1000, pos.y + 1000, pos.z + 1000 };

			//GAMEPLAY::CLEAR_AREA(pos.x, pos.y, pos.z, 1000, true);


			PED::SET_PED_NON_CREATION_AREA(v1.x, v1.y, v1.z, v2.x, v2.y, v2.z);	// 玩家矩形范围1000米内不创建NPC

		}
		else
		{
			STREAMING::SET_POPULATION_BUDGET_MULTIPLIER(3);
			PED::CLEAR_PED_NON_CREATION_AREA();		// 关闭后玩家范围内允许创建NPC
		}

	}
public:
	CreateWordNobody(string caption)
		: MenuItemSwitchable(caption) {}
};


// 世界选项
MenuBase* CreateWordMenu(MenuController* controller)
{
	auto menu = new MenuBase(new MenuItemListTitle(("世界选项")));
	controller->RegisterMenu(menu);


	menu->AddItem(new MenuItemMenu(("天气选项"), CreateWeatherMenu(controller)));
	menu->AddItem(new MenuItemMenu(("时间选项"), CreateTimeMenu(controller)));
	menu->AddItem(new MenuItemMenu(("下雪选项"), CreateSnowMenu(controller)));

	menu->AddItem(new CreateWordNobody(("没有行人")));




	return menu;
}