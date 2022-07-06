#include "Misc.h"

#include "scriptinfo.h"

#include <unordered_map>
#include <ctime>
#include <tchar.h>
#include <windows.h>

class MenuItemTimeTitle : public MenuItemTitle
{
	virtual string GetCaption()
	{
		time_t now = time(0);
		tm t;
		localtime_s(&t, &now);
		char str[32];
		sprintf_s(str, "%02d%s%02d",
			TIME::GET_CLOCK_HOURS(),
			t.tm_sec % 2 ? ":" : " ",
			TIME::GET_CLOCK_MINUTES()
		);
		return MenuItemTitle::GetCaption() + "   " + str;
	}
public:
	MenuItemTimeTitle(string caption)
		: MenuItemTitle(caption) {}
};

class MenuItemTimeAdjust : public MenuItemDefault
{
	int m_difHours;
	virtual void OnSelect()
	{
		TIME::ADD_TO_CLOCK_TIME(m_difHours, 0, 0);
	}
public:
	MenuItemTimeAdjust(string caption, int difHours)
		: MenuItemDefault(caption),
		m_difHours(difHours) {}
};

class MenuItemTimePause : public MenuItemSwitchable
{
	virtual void OnSelect()
	{
		bool newState = !GetState();
		TIME::PAUSE_CLOCK(newState, 0);
		SetState(newState);
	}
public:
	MenuItemTimePause(string caption)
		: MenuItemSwitchable(caption) {}
};

class MenuItemTimeRealistic : public MenuItemSwitchable
{
	int m_difHour;
	int m_difMin;
	virtual void OnSelect()
	{
		bool newState = !GetState();
		if (newState)
		{
			time_t now = time(0);
			tm t;
			localtime_s(&t, &now);
			m_difHour = TIME::GET_CLOCK_HOURS() - t.tm_hour;
			m_difMin = TIME::GET_CLOCK_MINUTES() - t.tm_min;
		}
		SetState(newState);
	}
	virtual void OnFrame()
	{
		if (!GetState())
			return;
		time_t now = time(0);
		tm t;
		localtime_s(&t, &now);
		int hours = t.tm_hour + m_difHour;
		int mins = t.tm_min + m_difMin;
		if (mins >= 60)
		{
			mins -= 60;
			hours++;
		}
		else
			if (mins < 0)
			{
				mins += 60;
				hours--;
			}
		if (hours >= 24)
			hours -= 24;
		else
			if (hours < 0)
				hours += 24;
		TIME::SET_CLOCK_TIME(hours, mins, t.tm_sec);
	}
public:
	MenuItemTimeRealistic(string caption)
		: MenuItemSwitchable(caption),
		m_difHour(0), m_difMin(0) {}
};

class MenuItemTimeSystemSynced : public MenuItemSwitchable
{
	virtual void OnFrame()
	{
		if (GetState())
		{
			time_t now = time(0);
			tm t;
			localtime_s(&t, &now);
			TIME::SET_CLOCK_TIME(t.tm_hour, t.tm_min, t.tm_sec);
		}
	}
public:
	MenuItemTimeSystemSynced(string caption)
		: MenuItemSwitchable(caption) {}
};


class MenuItemMiscHideHud : public MenuItemSwitchable
{
	virtual void OnFrame()
	{
		if (GetState())
			UI::HIDE_HUD_AND_RADAR_THIS_FRAME();
	}
public:
	MenuItemMiscHideHud(string caption)
		: MenuItemSwitchable(caption) {}
};


class MenuItemMiscRevealMap : public MenuItemDefault
{
	virtual void OnSelect()
	{
		MAP::SET_MINIMAP_HIDE_FOW(TRUE);
		MAP::_REVEAL_MINIMAP_FOW(0);
		SetStatusText(("显示地图"));
	}
public:
	MenuItemMiscRevealMap(string caption) :
		MenuItemDefault(caption) {}
};

class MenuItemMiscTransportGuns : public MenuItemSwitchable
{
	bool m_isHorse;
	bool m_isBullet;

	DWORD m_lastShootTime;

	virtual void OnSelect()
	{
		bool newstate = !GetState();
		if (newstate)
			SetStatusText(("警告: 破坏某些对象可能导致 ERR_GFX_STATE 错误\n\n 按小键盘 + 号键启用"), 5000);
		SetState(newstate);
	}

	virtual void OnFrame()
	{
		if (!GetState())
			return;

		if (!IsKeyDownLong(VK_ADD) && !IsKeyDownLong(VK_INSERT)) // num plus / insert
			return;

		if (m_lastShootTime + (m_isBullet ? 50 : 250) > GetTickCount())
			return;

		Player player = PLAYER::PLAYER_ID();
		Ped playerPed = PLAYER::PLAYER_PED_ID();

		if (!PLAYER::IS_PLAYER_CONTROL_ON(player))
			return;

		Entity transport;
		if (m_isHorse)
			if (PED::IS_PED_ON_MOUNT(playerPed))
				transport = PED::GET_MOUNT(playerPed);
			else
				return;
		else
			if (PED::IS_PED_IN_ANY_VEHICLE(playerPed, 0))
				transport = PED::GET_VEHICLE_PED_IS_USING(playerPed);
			else
				return;

		Vector3 v0, v1;
		GAMEPLAY::GET_MODEL_DIMENSIONS(ENTITY::GET_ENTITY_MODEL(transport), &v0, &v1);

		Hash modelHash = m_isBullet ? 0 : GAMEPLAY::GET_HASH_KEY("S_CANNONBALL");
		Hash weaponHash = GAMEPLAY::GET_HASH_KEY(m_isBullet ? "WEAPON_TURRET_GATLING" : "WEAPON_TURRET_REVOLVING_CANNON");

		if (modelHash && !STREAMING::HAS_MODEL_LOADED(modelHash))
		{
			STREAMING::REQUEST_MODEL(modelHash, FALSE);
			while (!STREAMING::HAS_MODEL_LOADED(modelHash))
				WAIT(0);
		}

		Vector3 coords0from = ENTITY::GET_OFFSET_FROM_ENTITY_IN_WORLD_COORDS(transport, -(v1.x + 0.25f), v1.y + 1.25f, 0.1);
		Vector3 coords1from = ENTITY::GET_OFFSET_FROM_ENTITY_IN_WORLD_COORDS(transport, (v1.x + 0.25f), v1.y + 1.25f, 0.1);
		Vector3 coords0to = ENTITY::GET_OFFSET_FROM_ENTITY_IN_WORLD_COORDS(transport, -v1.x, v1.y + 100.0f, 0.1f);
		Vector3 coords1to = ENTITY::GET_OFFSET_FROM_ENTITY_IN_WORLD_COORDS(transport, v1.x, v1.y + 100.0f, 0.1f);

		GAMEPLAY::SHOOT_SINGLE_BULLET_BETWEEN_COORDS(coords0from.x, coords0from.y, coords0from.z,
			coords0to.x, coords0to.y, coords0to.z,
			250, 1, weaponHash, playerPed, 1, 1, -1.0, 0);
		GAMEPLAY::SHOOT_SINGLE_BULLET_BETWEEN_COORDS(coords1from.x, coords1from.y, coords1from.z,
			coords1to.x, coords1to.y, coords1to.z,
			250, 1, weaponHash, playerPed, 1, 1, -1.0, 0);

		if (m_isBullet)
		{
			weaponHash = GAMEPLAY::GET_HASH_KEY("WEAPON_SNIPERRIFLE_CARCANO");
			GAMEPLAY::SHOOT_SINGLE_BULLET_BETWEEN_COORDS(coords0from.x, coords0from.y, coords0from.z,
				coords0to.x, coords0to.y, coords0to.z,
				250, 1, weaponHash, playerPed, 1, 0, -1.0, 0);
			GAMEPLAY::SHOOT_SINGLE_BULLET_BETWEEN_COORDS(coords1from.x, coords1from.y, coords1from.z,
				coords1to.x, coords1to.y, coords1to.z,
				250, 1, weaponHash, playerPed, 1, 0, -1.0, 0);
		}

		m_lastShootTime = GetTickCount();
	}
public:
	MenuItemMiscTransportGuns(string caption, bool isHorse, bool isBullet)
		: MenuItemSwitchable(caption),
		m_isHorse(isHorse), m_isBullet(isBullet),
		m_lastShootTime(0) {}
};



MenuBase* CreateTimeMenu(MenuController* controller)
{
	auto menu = new MenuBase(new MenuItemTimeTitle(("时间")));
	controller->RegisterMenu(menu);

	menu->AddItem(new MenuItemTimeAdjust(("前进1小时"), 1));
	menu->AddItem(new MenuItemTimeAdjust(("后退1小时"), -1));
	menu->AddItem(new MenuItemTimePause(("暂停时间")));
	menu->AddItem(new MenuItemTimeRealistic(("真实时间")));
	menu->AddItem(new MenuItemTimeSystemSynced(("同步系统时间")));

	return menu;
}

class MenuItemMiscFlightMode : public MenuItemSwitchable
{

	virtual void OnSelect()
	{
		Ped player = PLAYER::PLAYER_PED_ID();

		bool newstate = !GetState();
		if (newstate)
		{
			PED::SET_PED_GRAVITY(player, false);
			SetStatusText(("飞行模式开"));
		}
		else
		{
			PED::SET_PED_GRAVITY(player, true);
			PED::SET_PED_CAN_RAGDOLL(player, true);
			PLAYER::SET_PLAYER_CAN_USE_COVER(player, true);

			SetStatusText(("飞行模式关"));

		}
		SetState(newstate);
	}

	virtual void OnFrame()
	{
		if (GetState())
		{
			Ped player = PLAYER::PLAYER_PED_ID();
			int FlySpeed = 30;
			PED::SET_PED_GRAVITY(player, false);
			if (IsKeyDownLong(VK_LSHIFT))
			{
				FlySpeed = 40;
			}
			else {
				FlySpeed = 30;
			}

			if (IsKeyDownLong(0x57))	//长按W键
			{
				ENTITY::APPLY_FORCE_TO_ENTITY(player, 1, 0, FlySpeed, 0, 0, 0, 0, 0, true, true, true, true, true);
				ENTITY::SET_ENTITY_VELOCITY(player, 0, 0, 0);
			}

			if (IsKeyDownLong(0x53))	//长按S键
			{
				ENTITY::APPLY_FORCE_TO_ENTITY(player, 1, 0, -FlySpeed, 0, 0, 0, 0, 0, true, true, true, true, true);
				ENTITY::SET_ENTITY_VELOCITY(player, 0, 0, 0);
			}

			if (IsKeyDownLong(0x53))	//长按A键
			{
				ENTITY::APPLY_FORCE_TO_ENTITY(player, 1, -FlySpeed, 0, 0, 0, 0, 0, 0, true, true, true, true, true);
				ENTITY::SET_ENTITY_VELOCITY(player, 0, 0, 0);
			}

			if (IsKeyDownLong(0x53))	//长按D键
			{
				ENTITY::APPLY_FORCE_TO_ENTITY(player, 1, FlySpeed, 0, 0, 0, 0, 0, 0, true, true, true, true, true);
				ENTITY::SET_ENTITY_VELOCITY(player, 0, 0, 0);
			}

			if (IsKeyDownLong(VK_SPACE))	//长按空格键
			{
				ENTITY::APPLY_FORCE_TO_ENTITY(player, 1, 0, 0, FlySpeed, 0, 0, 0, 0, true, true, true, true, true);
				ENTITY::SET_ENTITY_VELOCITY(player, 0, 0, 0);
				if (PED::IS_PED_JUMPING(player))
				{
					Vector3 coords = ENTITY::GET_OFFSET_FROM_ENTITY_IN_WORLD_COORDS(PLAYER::PLAYER_PED_ID(), 0, 0, 0);

					ENTITY::SET_ENTITY_COORDS(player, coords.x, coords.y, (coords.z - 1), true, false, false, false);
				}

			}

			if (IsKeyDownLong(VK_CONTROL))	//长按Ctrl键
			{
				ENTITY::APPLY_FORCE_TO_ENTITY(player, 1, 0, 0, -FlySpeed, 0, 0, 0, 0, true, true, true, true, true);
				ENTITY::SET_ENTITY_VELOCITY(player, 0, 0, 0);
			}

			if (PED::IS_PED_JUMPING(player) || PED::IS_PED_RAGDOLL(player))	//玩家处于跳跃状态 或跌倒状态
			{
				Vector3 coords = ENTITY::GET_OFFSET_FROM_ENTITY_IN_WORLD_COORDS(PLAYER::PLAYER_PED_ID(), 0, 0, 0);

				ENTITY::SET_ENTITY_COORDS(player, coords.x, coords.y, (coords.z - 1), true, false, false, false);
			}
		}
	}

public:
	MenuItemMiscFlightMode(string caption)
		: MenuItemSwitchable(caption) {}
};

class MenuItemMiscThroughMode : public MenuItemSwitchable
{

	virtual void OnSelect()
	{
		Ped player = PLAYER::PLAYER_PED_ID();

		bool newstate = !GetState();
		if (newstate)
		{
			ENTITY::SET_ENTITY_COLLISION(player, false, true);
			PED::SET_PED_GRAVITY(player, false);
			SetStatusText(("穿墙模式开"));
		}
		else
		{
			ENTITY::SET_ENTITY_COLLISION(player, true, true);
			PED::SET_PED_GRAVITY(player, true);
			SetStatusText(("穿墙模式关"));
		}
		SetState(newstate);
	}

public:
	MenuItemMiscThroughMode(string caption)
		: MenuItemSwitchable(caption) {}
};


//出完美皮
class MenuItemOnrush : public MenuItemSwitchable
{
	virtual void OnFrame()
	{
		if (GetState()) {

			Vector3 coords;
			if (WEAPON::GET_PED_LAST_WEAPON_IMPACT_COORD(PLAYER::PLAYER_PED_ID(), &coords))	//玩家是否命中了物品
			{
				Player pay = PLAYER::PLAYER_ID();  //获取玩家ID
				Entity entity;  //申明一个Entity 类型的变量

				if (PLAYER::GET_PLAYER_TARGET_ENTITY(pay, &entity) || PLAYER::GET_ENTITY_PLAYER_IS_FREE_AIMING_AT(pay, &entity))  //判断开枪时是否瞄准了NPC 或 动物
				{
					DWORD model =  ENTITY::GET_ENTITY_MODEL(entity);
					if (STREAMING::IS_MODEL_IN_CDIMAGE(model) && STREAMING::IS_MODEL_VALID(model))
					{
						// 删除原动物
						ENTITY::SET_ENTITY_HEALTH(entity, 0, true);	//设置目标生命值为0
						ENTITY::SET_ENTITY_ALPHA(entity, 0, true);	// 设置透明
						ENTITY::SET_ENTITY_COLLISION(entity, false, false);	// 移除碰撞 让它掉到地底去
						ENTITY::SET_ENTITY_COMPLETELY_DISABLE_COLLISION(entity, true, false);	// 移除碰撞 让它掉到地底去

						// 创建一个一模一样的动物
						STREAMING::REQUEST_MODEL(model, FALSE);		// 搜索模型
						while (!STREAMING::HAS_MODEL_LOADED(model))
							WaitAndDraw(0); // WAIT(0);
						//Vector3 coords = ENTITY::GET_OFFSET_FROM_ENTITY_IN_WORLD_COORDS(PLAYER::PLAYER_PED_ID(), 0.0, 3.0, -0.3);	// 获取原动物坐标位置
						Vector3 coords = ENTITY::GET_OFFSET_FROM_ENTITY_IN_WORLD_COORDS(entity, 0.0, 0, 0);	// 获取原动物坐标位置
						Ped ped = PED::CREATE_PED(model, coords.x, coords.y, coords.z, static_cast<float>(rand() % 360), 0, 0, 0, 0);	// 创建动物模型

						ENTITY::SET_ENTITY_COORDS(entity, 0, 0, -1000, 0, 0, 1, FALSE);	// 将原动物传送到一边去 （距离太远游戏会自动删除吗？

						PED::SET_PED_VISIBLE(ped, TRUE);				// 设置可见
						ENTITY::SET_ENTITY_HEALTH(ped, 0, true);		// 设置目标生命值为0
						ENTITY::SET_PED_AS_NO_LONGER_NEEDED(&ped);		
						STREAMING::SET_MODEL_AS_NO_LONGER_NEEDED(model);// 设为不再需要

					}
				}
			}

			// 旧版按T方法
			if (IsKeyJustUp(KeyConfig::KEY_HOT_1) || IsControllerButtonJustUp(KeyConfig::KEY_HOT_1))
			{
				Player pay = PLAYER::PLAYER_ID();  //获取玩家ID
				Entity entity;  //申明一个Entity 类型的变量

				if (PLAYER::GET_PLAYER_TARGET_ENTITY(pay, &entity) || PLAYER::GET_ENTITY_PLAYER_IS_FREE_AIMING_AT(pay, &entity))  //判断是否瞄准了NPC
				{
					DWORD model = ENTITY::GET_ENTITY_MODEL(entity);
					if (STREAMING::IS_MODEL_IN_CDIMAGE(model) && STREAMING::IS_MODEL_VALID(model))
					{
						// 删除原动物
						ENTITY::SET_ENTITY_HEALTH(entity, 0, true);	//设置目标生命值为0
						ENTITY::SET_ENTITY_ALPHA(entity, 0, true);	// 设置透明
						ENTITY::SET_ENTITY_COLLISION(entity, false, false);	// 移除碰撞 让它掉到地底去
						ENTITY::SET_ENTITY_COMPLETELY_DISABLE_COLLISION(entity, true, false);	// 移除碰撞 让它掉到地底去

						// 创建一个一模一样的动物
						STREAMING::REQUEST_MODEL(model, FALSE);		// 搜索模型
						while (!STREAMING::HAS_MODEL_LOADED(model))
							WaitAndDraw(0); // WAIT(0);
						//Vector3 coords = ENTITY::GET_OFFSET_FROM_ENTITY_IN_WORLD_COORDS(PLAYER::PLAYER_PED_ID(), 0.0, 3.0, -0.3);	// 获取原动物坐标位置
						Vector3 coords = ENTITY::GET_OFFSET_FROM_ENTITY_IN_WORLD_COORDS(entity, 0.0, 0, 0);	// 获取原动物坐标位置
						Ped ped = PED::CREATE_PED(model, coords.x, coords.y, coords.z, static_cast<float>(rand() % 360), 0, 0, 0, 0);	// 创建动物模型
						PED::SET_PED_VISIBLE(ped, TRUE);				// 设置可见
						ENTITY::SET_ENTITY_HEALTH(ped, 0, true);		// 设置目标生命值为0
						ENTITY::SET_PED_AS_NO_LONGER_NEEDED(&ped);
						STREAMING::SET_MODEL_AS_NO_LONGER_NEEDED(model);// 设为不再需要


					}
				}
			}
		}
	}
public:
	MenuItemOnrush(string caption)
		: MenuItemSwitchable(caption) { }
};


MenuBase* CreateMiscMenu(MenuController* controller)
{
	auto menu = new MenuBase(new MenuItemTitle(("其他选项")));
	controller->RegisterMenu(menu);

	menu->AddItem(new MenuItemMiscHideHud(("隐藏小地图")));

	menu->AddItem(new MenuItemMiscTransportGuns(("马附机枪"), true, true));
	menu->AddItem(new MenuItemMiscTransportGuns(("马附RPG"), true, false));
	menu->AddItem(new MenuItemMiscTransportGuns(("马车机枪"), false, true));
	menu->AddItem(new MenuItemMiscTransportGuns(("马车RPG"), false, false));
	menu->AddItem(new MenuItemMiscFlightMode(("飞行模式")));
	menu->AddItem(new MenuItemMiscThroughMode(("穿墙模式")));
	menu->AddItem(new MenuItemOnrush(("出完美皮")));


	return menu;
}