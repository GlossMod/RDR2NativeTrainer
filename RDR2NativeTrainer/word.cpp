#include "word.h"

#include "scriptinfo.h"

#include <unordered_map>
#include <ctime>
#include <tchar.h>
#include <windows.h>
#include "Weather.h"
#include "Misc.h"




// ����ѩ���ǵȼ�
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

// ��ѩѡ��
MenuBase* CreateSnowMenu(MenuController* controller)
{
	auto menu = new MenuBase(new MenuItemListTitle(GT("��ѩѡ��")));
	controller->RegisterMenu(menu);

	menu->AddItem(new MenuItemWordSnow(GT("�ر�"), 1));
	menu->AddItem(new MenuItemWordSnow(GT("Сѩ����"), 2));
	menu->AddItem(new MenuItemWordSnow(GT("��ѩ����"), 3));

	return menu;
}

// û������
class CreateWordNobody : public MenuItemSwitchable
{	
	virtual void OnFrame()
	{
		if (GetState())
		{
			STREAMING::SET_POPULATION_BUDGET_MULTIPLIER(0);

			runOnAllNearbyPedsToPlayer([](auto ped) {
				//GET_SCREEN_COORD_FROM_WORLD_COORD
				//Vector3 pos = ENTITY::GET_ENTITY_COORDS(ped, false, false/*idk*/);	// ��ȡped����		//				
				//ENTITY::DELETE_ENTITY(&ped);
				if (ENTITY::DOES_ENTITY_EXIST(ped) && (ped != PLAYER::GET_PLAYER_PED(PLAYER::PLAYER_ID())))		// PED�����Ҳ������
				{
					//Vector3 pos = ENTITY::GET_ENTITY_COORDS(ped, false, false/*idk*/);	// ��ȡped����
					//float x, y;
					//GRAPHICS::GET_SCREEN_COORD_FROM_WORLD_COORD(pos.x, pos.y, pos.z, &x, &y);	//ȫ�ֻ�ȡ����λ��
					//drawBoxOutline(x, y, 0.01f, 0.002f, 255, 0, 0, 255);	// ���ƺ��

					//Ped pedHandle = PED::CLONE_PED(ped, true, true, true);  // ����ped���ݵ�pedHandle
					//PED::DELETE_PED(&pedHandle);
					//ENTITY::DELETE_ENTITY(&pedHandle);
					ENTITY::SET_ENTITY_HEALTH(ped, 0, true);	// ��PEDɱ��
					ENTITY::SET_ENTITY_COORDS(ped, 0, 0, -10000, 0, 0, 1, FALSE);	// ��PED�����ص���ȥ
				}
			});
			Vector3 pos = ENTITY::GET_ENTITY_COORDS(PLAYER::PLAYER_PED_ID(), false, false);	// ��ȡped����

			Vector3 v1 = { pos.x - 1000, pos.y - 1000, pos.z - 1000 };
			Vector3 v2 = { pos.x + 1000, pos.y + 1000, pos.z + 1000 };

			//GAMEPLAY::CLEAR_AREA(pos.x, pos.y, pos.z, 1000, true);


			PED::SET_PED_NON_CREATION_AREA(v1.x, v1.y, v1.z, v2.x, v2.y, v2.z);	// ��Ҿ��η�Χ1000���ڲ�����NPC

		}
		else
		{
			STREAMING::SET_POPULATION_BUDGET_MULTIPLIER(3);
			PED::CLEAR_PED_NON_CREATION_AREA();		// �رպ���ҷ�Χ��������NPC
		}

	}
public:
	CreateWordNobody(string caption)
		: MenuItemSwitchable(caption) {}
};


// ����ѡ��
MenuBase* CreateWordMenu(MenuController* controller)
{
	auto menu = new MenuBase(new MenuItemListTitle(GT("����ѡ��")));
	controller->RegisterMenu(menu);


	menu->AddItem(new MenuItemMenu(GT("����ѡ��"), CreateWeatherMenu(controller)));
	menu->AddItem(new MenuItemMenu(GT("ʱ��ѡ��"), CreateTimeMenu(controller)));
	menu->AddItem(new MenuItemMenu(GT("��ѩѡ��"), CreateSnowMenu(controller)));

	menu->AddItem(new CreateWordNobody(GT("û������")));




	return menu;
}