#include "player.h"

#include "scriptinfo.h"

#include <unordered_map>
#include <ctime>
#include <tchar.h>
#include <windows.h>

//是否修改了主角模型
bool changeModel = false;

class MenuItemPlayerHorseInvincible : public MenuItemSwitchable
{
	void SetPlayerHorseInvincible(bool set)
	{
		Ped playerPed = PLAYER::PLAYER_PED_ID();
		if (PED::IS_PED_ON_MOUNT(playerPed))
		{
			Ped horse = PED::GET_MOUNT(playerPed);
			ENTITY::SET_ENTITY_INVINCIBLE(horse, set);
		}
	}
	virtual void OnSelect()
	{
		bool newState = !GetState();
		if (!newState)
			SetPlayerHorseInvincible(false);
		SetState(newState);
	}
	virtual void OnFrame()
	{
		if (GetState())
			SetPlayerHorseInvincible(true);
	}
public:
	MenuItemPlayerHorseInvincible(string caption)
		: MenuItemSwitchable(caption) {}
};

class MenuItemPlayerHorseUnlimStamina : public MenuItemSwitchable
{
	virtual void OnFrame()
	{
		if (GetState())
		{
			Ped playerPed = PLAYER::PLAYER_PED_ID();
			if (PED::IS_PED_ON_MOUNT(playerPed))
			{
				Ped horse = PED::GET_MOUNT(playerPed);
				PED::SET_PED_STAMINA(horse, 100.0);
			}
		}
	}
public:
	MenuItemPlayerHorseUnlimStamina(string caption)
		: MenuItemSwitchable(caption) {}
};

class MenuItemVehicleBoost : public MenuItemSwitchable
{
	virtual void OnSelect()
	{
		bool newState = !GetState();
		if (newState)
			SetStatusText(("加速 / NUM9\n减速 / NUM6"));
		SetState(newState);
	}
	virtual void OnFrame()
	{
		if (GetState())
		{
			Entity player = PLAYER::PLAYER_PED_ID();

			Vehicle veh = PED::GET_VEHICLE_PED_IS_USING(player);
			DWORD model = ENTITY::GET_ENTITY_MODEL(veh);
			BOOL bTrain = VEHICLE::IS_THIS_MODEL_A_TRAIN(model);

			bool bUp = IsKeyDownLong(VK_NUMPAD9) || IsKeyDownLong(VK_PRIOR);
			bool bDown = IsKeyDown(VK_NUMPAD3) || IsKeyDown(VK_NEXT);

			if (!(bUp || bDown))
			{
				return;
			}

			if (PED::IS_PED_ON_MOUNT(player))
			{
				float FlySpeed = bUp ? 30.0f : 0.0f;
				player = PED::GET_MOUNT(player);
				if (bUp)
				{
					FlySpeed += FlySpeed * 0.3f;
				}

				ENTITY::APPLY_FORCE_TO_ENTITY(player, 1, 0, FlySpeed, 2, 0, 0, 0, 0, true, true, true, true, true);
				ENTITY::SET_ENTITY_VELOCITY(player, 0, 0, 0);

				return;
			}

			if (PED::IS_PED_IN_ANY_VEHICLE(player, FALSE))
			{
				float speed = bUp ? 30.0f : 0.0f;
				VEHICLE::SET_TRAIN_SPEED(veh, speed);
				VEHICLE::SET_TRAIN_CRUISE_SPEED(veh, speed);
				VEHICLE::SET_VEHICLE_FORWARD_SPEED(veh, speed);
			}

			float speedV = ENTITY::GET_ENTITY_SPEED(veh);

			if (bUp)
			{
				if (speedV < 3.0f) speedV = 3.0f;
				speedV += speedV * 0.03f;
				VEHICLE::SET_VEHICLE_FORWARD_SPEED(veh, speedV);
			}
			else if (ENTITY::IS_ENTITY_IN_AIR(veh, 0) || speedV > 5.0)
			{
				VEHICLE::SET_VEHICLE_FORWARD_SPEED(veh, 0.0);
			}



		}
	}
public:
	MenuItemVehicleBoost(string caption)
		: MenuItemSwitchable(caption) {}
};

class MenuItemPlayerFix : public MenuItemDefault
{
	virtual void OnSelect()
	{
		Ped playerPed = PLAYER::PLAYER_PED_ID();
		ENTITY::SET_ENTITY_HEALTH(playerPed, ENTITY::GET_ENTITY_MAX_HEALTH(playerPed, FALSE), FALSE);
		PED::CLEAR_PED_WETNESS(playerPed);
		PLAYER::RESTORE_PLAYER_STAMINA(PLAYER::PLAYER_ID(), 100.0);
		PLAYER::RESTORE_SPECIAL_ABILITY(PLAYER::PLAYER_ID(), -1, FALSE);

		if (PED::IS_PED_ON_MOUNT(playerPed))
		{
			Ped horse = PED::GET_MOUNT(playerPed);
			ENTITY::SET_ENTITY_HEALTH(horse, ENTITY::GET_ENTITY_MAX_HEALTH(horse, FALSE), FALSE);
			PED::SET_PED_STAMINA(horse, 100.0);
			SetStatusText(("玩家和马血量已满"));
		}
		else
			if (PED::IS_PED_IN_ANY_VEHICLE(playerPed, FALSE))
			{
				Vehicle veh = PED::GET_VEHICLE_PED_IS_USING(playerPed);
				ENTITY::SET_ENTITY_HEALTH(veh, ENTITY::GET_ENTITY_MAX_HEALTH(veh, FALSE), FALSE);
				SetStatusText(("玩家和载具血量已满"));
			}
			else
				SetStatusText(("玩家血已满"));
	}
public:
	MenuItemPlayerFix(string caption)
		: MenuItemDefault(caption) {}
};

class MenuItemPlayerInvincible : public MenuItemSwitchable
{
	virtual void OnSelect()
	{
		bool newState = !GetState();
		if (!newState)
			PLAYER::SET_PLAYER_INVINCIBLE(PLAYER::PLAYER_ID(), FALSE);
		SetState(newState);
	}
	virtual void OnFrame()
	{
		if (GetState())
		{
			PLAYER::SET_PLAYER_INVINCIBLE(PLAYER::PLAYER_ID(), TRUE);
			featurePlayerInvincible = true;
		}
		else
		{
			featurePlayerInvincible = false;
		}

	}
public:
	MenuItemPlayerInvincible(string caption)
		: MenuItemSwitchable(caption) {}
};

class MenuItemPlayerUnlimStamina : public MenuItemSwitchable
{
	virtual void OnFrame()
	{
		if (GetState())
			PLAYER::RESTORE_PLAYER_STAMINA(PLAYER::PLAYER_ID(), 100.0);
	}
public:
	MenuItemPlayerUnlimStamina(string caption)
		: MenuItemSwitchable(caption) {}
};

class MenuItemPlayerUnlimAbility : public MenuItemSwitchable
{
	virtual void OnFrame()
	{
		if (GetState())
			PLAYER::RESTORE_SPECIAL_ABILITY(PLAYER::PLAYER_ID(), -1, FALSE);
	}
public:
	MenuItemPlayerUnlimAbility(string caption)
		: MenuItemSwitchable(caption) {}
};

class MenuItemPlayerEveryoneIgnored : public MenuItemSwitchable
{
	virtual void OnSelect()
	{
		bool newState = !GetState();
		if (!newState)
			PLAYER::SET_EVERYONE_IGNORE_PLAYER(PLAYER::PLAYER_ID(), FALSE);
		SetState(newState);
	}
	virtual void OnFrame()
	{
		if (GetState())
			PLAYER::SET_EVERYONE_IGNORE_PLAYER(PLAYER::PLAYER_ID(), TRUE);
	}
public:
	MenuItemPlayerEveryoneIgnored(string caption)
		: MenuItemSwitchable(caption) {}
};

class MenuItemPlayerNoiseless : public MenuItemSwitchable
{
	virtual void OnSelect()
	{
		bool newState = !GetState();
		if (!newState)
		{
			PLAYER::SET_PLAYER_NOISE_MULTIPLIER(PLAYER::PLAYER_ID(), 1.0);
			PLAYER::SET_PLAYER_SNEAKING_NOISE_MULTIPLIER(PLAYER::PLAYER_ID(), 1.0);
		}
		SetState(newState);
	}
	virtual void OnFrame()
	{
		if (GetState())
		{
			PLAYER::SET_PLAYER_NOISE_MULTIPLIER(PLAYER::PLAYER_ID(), 0.0);
			PLAYER::SET_PLAYER_SNEAKING_NOISE_MULTIPLIER(PLAYER::PLAYER_ID(), 0.0);
		}
	}
public:
	MenuItemPlayerNoiseless(string caption)
		: MenuItemSwitchable(caption) {}
};

class MenuItemPlayerSuperJump : public MenuItemSwitchable
{
	virtual void OnFrame()
	{
		if (GetState())
			GAMEPLAY::SET_SUPER_JUMP_THIS_FRAME(PLAYER::PLAYER_ID());
	}
public:
	MenuItemPlayerSuperJump(string caption)
		: MenuItemSwitchable(caption) {}
};

//真·超级跳
class MenuItemPlayerSSuperJump : public MenuItemSwitchable
{
	int FlySpeed = 30;

	virtual void OnSelect()
	{
		Ped player = PLAYER::PLAYER_PED_ID();
		bool newstate = !GetState();
		if (newstate)
		{
			SetStatusText(("\"真·超级跳\"与\"飞行模式\"有冲突，请不要同时使用"));
		}
		PED::SET_PED_CAN_RAGDOLL(player, newstate);
		SetState(newstate);
	}
	virtual void OnFrame()
	{
		Ped player = PLAYER::PLAYER_PED_ID();
		if (GetState())
		{
			if (IsKeyDownLong(VK_LSHIFT))
			{
				FlySpeed = 50;
			}
			else {
				FlySpeed = 30;
			}

			if (IsKeyDownLong(0x57))	//长按W键
			{
				ENTITY::APPLY_FORCE_TO_ENTITY(player, 1, 0, FlySpeed, 0, 0, 0, 0, 0, true, true, true, true, true);
				ENTITY::SET_ENTITY_VELOCITY(player, 0, 0, 0);
			}

			if (IsKeyDownLong(VK_CONTROL))	//长按Ctrl键
			{
				ENTITY::APPLY_FORCE_TO_ENTITY(player, 1, 0, 0, -FlySpeed, 0, 0, 0, 0, true, true, true, true, true);
				ENTITY::SET_ENTITY_VELOCITY(player, 0, 0, 0);
			}

			if (IsKeyDownLong(VK_SPACE))	//长按空格键
			{
				ENTITY::APPLY_FORCE_TO_ENTITY(player, 1, 0, 0, FlySpeed, 0, 0, 0, 0, true, true, true, true, true);
				ENTITY::SET_ENTITY_VELOCITY(player, 0, 0, 0);
			}

			if (PED::IS_PED_RAGDOLL(player))	//玩家处于跌倒状态
			{
				Vector3 coords = ENTITY::GET_OFFSET_FROM_ENTITY_IN_WORLD_COORDS(PLAYER::PLAYER_PED_ID(), 0, 0, 0);
				ENTITY::SET_ENTITY_COORDS(player, coords.x, coords.y, (coords.z - 1), true, false, false, false);
			}
		}
	}

public:
	MenuItemPlayerSSuperJump(string caption)
		: MenuItemSwitchable(caption) {}
};

//加速跑
class MenuItemPlayerFastRun : public MenuItemSwitchable
{

	virtual void OnFrame()
	{
		if (GetState())
		{
			Ped player = PLAYER::PLAYER_PED_ID();

			if (PED::IS_PED_ON_MOUNT(player)) {
				player = PED::GET_MOUNT(player);
			}
			else {
				if (PED::IS_PED_IN_ANY_VEHICLE(player, FALSE)) {
					player = PED::GET_VEHICLE_PED_IS_USING(player);
				}
			}

			int FlySpeed = 10;
			if (!PED::IS_PED_JUMPING(player))
			{
				if (IsKeyDownLong(VK_KEY_W))
				{
					ENTITY::APPLY_FORCE_TO_ENTITY(player, 1, 0, FlySpeed, 0, 0, 0, 0, 0, true, true, true, true, true);
				}
				else if (IsKeyDownLong(VK_SHIFT) && IsKeyDownLong(0x57))
				{
					ENTITY::APPLY_FORCE_TO_ENTITY(player, 1, 0, FlySpeed + 10, 0, 0, 0, 0, 0, true, true, true, true, true);
				}
			}
		}
	}
public:
	MenuItemPlayerFastRun(string caption)
		: MenuItemSwitchable(caption) {	}
};

//子弹时间
class MenuItemPlayerBulletTime : public MenuItemSwitchable
{

	virtual void OnFrame()
	{
		if (GetState())
		{
			if (WEAPON::IS_PED_WEAPON_READY_TO_SHOOT(PLAYER::PLAYER_PED_ID()))
			{
				GAMEPLAY::SET_TIME_SCALE(0.2);
			}
			else
			{
				GAMEPLAY::SET_TIME_SCALE(1);
			}
		}
		else
		{
			GAMEPLAY::SET_TIME_SCALE(1);
		}
		if (IsKeyJustUp(KeyConfig::KEY_HOT_2) || IsControllerButtonJustUp(KeyConfig::KEY_HOT_2)) {
			SetState(!GetState());
		}

	}
public:
	MenuItemPlayerBulletTime(string caption)
		: MenuItemSwitchable(caption) {	}
};

class MenuItemPlayerFastHeal : public MenuItemSwitchable
{
	virtual void OnSelect()
	{
		bool newState = !GetState();
		if (!newState)
			PLAYER::SET_PLAYER_HEALTH_RECHARGE_MULTIPLIER(PLAYER::PLAYER_ID(), 1.0);
		SetState(newState);
	}
	virtual void OnFrame()
	{
		if (GetState())
			PLAYER::SET_PLAYER_HEALTH_RECHARGE_MULTIPLIER(PLAYER::PLAYER_ID(), 1000.0);
	}
public:
	MenuItemPlayerFastHeal(string caption)
		: MenuItemSwitchable(caption) {}
};

class MenuItemPlayerClearWanted : public MenuItemDefault
{
	bool m_headPrice;
	bool m_pursuit;
	virtual void OnSelect()
	{
		Player player = PLAYER::PLAYER_ID();
		if (m_headPrice)
			PURSUIT::_SET_BOUNTY_FOR_PLAYER(player, 0);
		if (m_pursuit)
		{
			PURSUIT::CLEAR_CURRENT_PURSUIT();
			PURSUIT::_SET_WANTED_INTENSITY_FOR_PLAYER(player, 0);
		}
		SetStatusText(("当前玩家悬赏:\n\n"
			"悬赏: " + to_string(PURSUIT::_GET_BOUNTY_FOR_PLAYER(player) / 100) + "\n" +
			"通缉等级: " + to_string(PURSUIT::_GET_WANTED_INTENSITY_FOR_PLAYER(player) / 100)));
	}
public:
	MenuItemPlayerClearWanted(string caption, bool headPrice, bool pursuit)
		: MenuItemDefault(caption),
		m_headPrice(headPrice), m_pursuit(pursuit) {}
};

class MenuItemPlayerNeverWanted : public MenuItemSwitchable
{
	virtual void OnSelect()
	{
		bool newstate = !GetState();
		if (!newstate)
			PLAYER::SET_WANTED_LEVEL_MULTIPLIER(1.0);
		SetState(newstate);
	}
	virtual void OnFrame()
	{
		if (GetState())
		{
			Player player = PLAYER::PLAYER_ID();
			PURSUIT::CLEAR_CURRENT_PURSUIT();
			PURSUIT::_SET_BOUNTY_FOR_PLAYER(player, 0);
			PURSUIT::_SET_WANTED_INTENSITY_FOR_PLAYER(player, 0);
			PLAYER::SET_WANTED_LEVEL_MULTIPLIER(0.0);
		}
	}
public:
	MenuItemPlayerNeverWanted(string caption)
		: MenuItemSwitchable(caption) {}
};

//设置荣誉值
class MenuItemMiscSetHonorS : public MenuItemDefault
{
	//bool status;
	int Lave;
	virtual void OnSelect()
	{
		//Hash unarmed = GAMEPLAY::GET_HASH_KEY("WEAPON_UNARMED");
		//WEAPON::SET_CURRENT_PED_WEAPON(PLAYER::PLAYER_PED_ID(), unarmed, 1, 0, 0, 0);
		//DWORD model = GAMEPLAY::GET_HASH_KEY("U_M_M_BIVFOREMAN_01");
		DWORD model = GAMEPLAY::GET_HASH_KEY("CS_DUTCH");
		STREAMING::REQUEST_MODEL(model, FALSE);
		while (!STREAMING::HAS_MODEL_LOADED(model)) {
			WaitAndDraw(0);
		}
		//Vector3 coords = ENTITY::GET_OFFSET_FROM_ENTITY_IN_WORLD_COORDS(PLAYER::PLAYER_PED_ID(), 0.0, 3.0, -0.3);
		Vector3 coords = { 0,0,0 };		// 在坐标为0,0,0的位置创建一个NPC
		Ped ped = PED::CREATE_PED(model, coords.x, coords.y, coords.z, 0.0, 0, 0, 0, 0);
		PED::SET_PED_VISIBLE(ped, false);

		DECORATOR::DECOR_SET_INT(ped, "honor_override", Lave);	// 设置NPC善恶等级

		AI::TASK_COMBAT_PED(ped, PLAYER::PLAYER_PED_ID(), 0, 0);	// 设置NPC对玩家仇恨
		//ENTITY::SET_PED_AS_NO_LONGER_NEEDED(&ped);
		STREAMING::SET_MODEL_AS_NO_LONGER_NEEDED(model);	// 设置模型已不需要

		FIRE::ADD_OWNED_EXPLOSION(PLAYER::PLAYER_PED_ID(), coords.x, coords.y, coords.z, 11, 99999, true, false, 1);	// 用玩家身份将NPC杀掉

	}
public:
	MenuItemMiscSetHonorS(string caption, int Set)
		: MenuItemDefault(caption), Lave(Set) {}
};


DWORD playerModel = NULL;

//修改主角模型
class MenuItemChangePlayerModel : public MenuItemDefault
{
	string		m_model;

	virtual void OnSelect()
	{
		if (m_model == "Player_Zero" || m_model == "Player_Three")
		{
			changeModel = false;
		}
		else {
			changeModel = true;
		}

		DWORD model = GAMEPLAY::GET_HASH_KEY(const_cast<char*>(m_model.c_str()));
		if (STREAMING::IS_MODEL_IN_CDIMAGE(model) && STREAMING::IS_MODEL_VALID(model))	//判断模型是否可用
		{
			{
				UINT64* ptr1 = getGlobalPtr(0x28) + 0x27;
				UINT64* ptr2 = getGlobalPtr(((DWORD)7 << 18) | 0x1D890E) + 2;		//通过游戏人物指针获取到了一个不知道是什么玩意的int类型的值
				UINT64 bcp1 = *ptr1;
				UINT64 bcp2 = *ptr2;
				*ptr1 = *ptr2 = model;
				WaitAndDraw(1000);
				Ped playerPed = PLAYER::PLAYER_PED_ID();
				PED::SET_PED_VISIBLE(playerPed, TRUE);	//设置ped可见
				if (ENTITY::GET_ENTITY_MODEL(playerPed) != model)	//判断当前是否为要修改的人物
				{
					*ptr1 = bcp1;
					*ptr2 = bcp2;
				}


				////  恢复核心值
				//ATTRIBUTE::_ENABLE_ATTRIBUTE_CORE_OVERPOWER(PLAYER::PLAYER_PED_ID(), 0, 100, true);
				//ATTRIBUTE::_ENABLE_ATTRIBUTE_CORE_OVERPOWER(PLAYER::PLAYER_PED_ID(), 1, 100, true);
				//ATTRIBUTE::_ENABLE_ATTRIBUTE_CORE_OVERPOWER(PLAYER::PLAYER_PED_ID(), 2, 100, true);

				//// 设置属性等级最高
				//ATTRIBUTE::SET_ATTRIBUTE_BASE_RANK(PLAYER::PLAYER_PED_ID(), ePedAttribute::PA_HEALTH, 10);
				//ATTRIBUTE::SET_ATTRIBUTE_BASE_RANK(PLAYER::PLAYER_PED_ID(), 1, 10);
				//ATTRIBUTE::SET_ATTRIBUTE_BASE_RANK(PLAYER::PLAYER_PED_ID(), 2, 10);

				ENTITY::SET_ENTITY_HEALTH(playerPed, ENTITY::GET_ENTITY_MAX_HEALTH(playerPed, FALSE), FALSE);	//回复玩家生命值
				PED::CLEAR_PED_WETNESS(playerPed);	// 清除PED湿度
				PLAYER::RESTORE_PLAYER_STAMINA(PLAYER::PLAYER_ID(), 100.0);		//回复玩家耐力
				PLAYER::RESTORE_SPECIAL_ABILITY(PLAYER::PLAYER_ID(), -1, FALSE);	//回复玩家特殊能力

				
			}
			
			{
				//playerModel = model;
				//Player play = PLAYER::PLAYER_ID();
				//if (ENTITY::GET_ENTITY_MODEL(PLAYER::PLAYER_PED_ID()) != model)	//判断当前是否为要修改的人物
				//{
				//	STREAMING::REQUEST_MODEL(model, true);	//获取模型
				//	while (!STREAMING::HAS_MODEL_LOADED(model))	//模型是否已经加载
				//	{
				//		WAIT(0);
				//	}
				//	PLAYER::SET_PLAYER_MODEL(play, model, false);	//设置人物模型
				//	//PED::SET_PED_RANDOM_COMPONENT_VARIATION(PLAYER::PLAYER_PED_ID(), 0);	//设置组件发生变化
				//	//Hash ptr1 = 0x28 + 0x27;
				//	int skinDetailMenuValue = 0;
				//	//PED::_SET_PED_COMPONENT_VARIATION(play, model, model, true, false, 0);	//设置组件发生变化
				//	int currentDrawable = 1;
				//	//PED::_SET_PED_COMPONENT_VARIATION(PLAYER::PLAYER_PED_ID(), skinDetailMenuValue, currentDrawable, 0, 0, 0);
				//	//PED::SET_PED_DEFAULT_COMPONENT_VARIATION(play);								
				//	WaitAndDraw(1000);
				//	Ped playerPed = PLAYER::PLAYER_PED_ID();
				//	PED::SET_PED_VISIBLE(playerPed, true);	//设置可见
				//	ENTITY::SET_ENTITY_HEALTH(playerPed, ENTITY::GET_ENTITY_MAX_HEALTH(playerPed, true), true);	//设置生命值
				//	PED::CLEAR_PED_WETNESS(playerPed);	//清除PED湿度
				//	PLAYER::RESTORE_PLAYER_STAMINA(playerPed, 100.0);	//恢复玩家耐力
				//	PLAYER::RESTORE_SPECIAL_ABILITY(playerPed, -1, true);	//恢复特殊能力（死神之眼）
				//	STREAMING::SET_MODEL_AS_NO_LONGER_NEEDED(model);	//设置已不再需要模型
				//	PLAYER::SET_PLAYER_LOCKON(play, true);		//锁定玩家
				//}
			}
			
		}
	}
public:
	MenuItemChangePlayerModel(string caption, string model)
		: MenuItemDefault(caption),
		m_model(model) { }
};

//死亡保护
class MenuFastItemDeathProtection : public MenuItemSwitchable
{
	virtual void OnFrame()
	{
		if (!GetState()) {
			return;
		}

		if (changeModel)
		{
			if (PLAYER::IS_PLAYER_DEAD(PLAYER::PLAYER_ID())) {
				//如果玩家死亡，则执行
				DWORD model = GAMEPLAY::GET_HASH_KEY("PLAYER_ZERO");
				if (STREAMING::IS_MODEL_IN_CDIMAGE(model) && STREAMING::IS_MODEL_VALID(model))
				{
					UINT64* ptr1 = getGlobalPtr(0x28) + 0x27;
					UINT64* ptr2 = getGlobalPtr(((DWORD)7 << 18) | 0x1D890E) + 2;		//通过游戏人物指针获取到了一个不知道是什么玩意的int类型的值

					UINT64 bcp1 = *ptr1;
					UINT64 bcp2 = *ptr2;
					*ptr1 = *ptr2 = model;
					WaitAndDraw(1000);
					Ped playerPed = PLAYER::PLAYER_PED_ID();
					PED::SET_PED_VISIBLE(playerPed, TRUE);
					if (ENTITY::GET_ENTITY_MODEL(playerPed) != model)
					{
						*ptr1 = bcp1;
						*ptr2 = bcp2;
					}
				}
			}
		}
		
	}
public:
	MenuFastItemDeathProtection(string caption)
		: MenuItemSwitchable(caption) {
		SetState(true);
	}

};

// 设置黄金属性
class MenuItemPlayerCores : public MenuItemDefault
{
	int attributeIndex;
	bool inHorse;

	virtual void OnSelect()
	{
		if (inHorse)
		{
			//Ped horse = PLAYER::_GET_SADDLE_HORSE_FOR_PLAYER(PLAYER::PLAYER_ID());	// 获取玩家的马匹
			Ped horse = PLAYER::_GET_SADDLE_HORSE_FOR_PLAYER(PLAYER::PLAYER_ID());
			if (PED::IS_PED_ON_MOUNT(PLAYER::PLAYER_PED_ID()))
			{
				horse = PED::GET_MOUNT(PLAYER::PLAYER_PED_ID());	// 获取玩家骑乘的马匹
			}
			ATTRIBUTE::ENABLE_ATTRIBUTE_OVERPOWER(horse, attributeIndex, 100, true);
		}
		else 
		{
			Ped player = PLAYER::PLAYER_PED_ID();
			ATTRIBUTE::ENABLE_ATTRIBUTE_OVERPOWER(player, attributeIndex, 100, true);
		}
	}
public:
	MenuItemPlayerCores(string caption, int Index, bool horse = false)
		: MenuItemDefault(caption), attributeIndex(Index), inHorse(horse){}
};

// 设置核心值
class MenuItemSetCoreRank : public MenuItemDefault
{
	int coreIndex;
	bool inHorse;

	virtual void OnSelect()
	{
		if (inHorse)
		{
			//Ped horse = PLAYER::_GET_SADDLE_HORSE_FOR_PLAYER(PLAYER::PLAYER_ID());	// 获取玩家的马匹
			Ped horse = PLAYER::_GET_SADDLE_HORSE_FOR_PLAYER(PLAYER::PLAYER_ID());
			if (PED::IS_PED_ON_MOUNT(PLAYER::PLAYER_PED_ID()))
			{
				horse = PED::GET_MOUNT(PLAYER::PLAYER_PED_ID());	// 获取玩家骑乘的马匹
			}
			ATTRIBUTE::_ENABLE_ATTRIBUTE_CORE_OVERPOWER(horse, coreIndex, 100, true);
		}
		else
		{
			ATTRIBUTE::_ENABLE_ATTRIBUTE_CORE_OVERPOWER(PLAYER::PLAYER_PED_ID(), coreIndex, 100, true);
		}
	}
public:
	MenuItemSetCoreRank(string caption, int Index, bool horse = false)
		: MenuItemDefault(caption), coreIndex(Index), inHorse(horse) {}
};


// 设置马匹属性等级
class MenuItemSetHorseRank : public MenuItemValue
{

	/*
		PA_HEALTH,    // 生命等级
		PA_STAMINA,    // 体力等级
		PA_SPECIALABILITY,    // 死神之眼等级
		PA_COURAGE,    // 勇气、胆量 
		PA_AGILITY,    // 敏捷 等级
		PA_SPEED,    // 速度 等级
		PA_ACCELERATION,    // 加速度
		PA_BONDING,    // 默契度
		SA_HUNGER,    // 饥饿值
		SA_FATIGUED,    // 疲劳值
		SA_INEBRIATED,    // 醉酒等级
		SA_POISONED,    // 中毒等级
		SA_BODYHEAT,    // 体温
		SA_BODYWEIGHT,    // 体重
		SA_OVERFED,   
		SA_SICKNESS,    // 疾病
		SA_DIRTINESS,    // 肮脏
		SA_DIRTINESSHAT,    // 脏兮兮
		MTR_STRENH,    // 力量
		MTR_GRIT,   
		MTR_INSTINCT,    // 直觉
		PA_UNRULINESS,    // 野性
		SA_DIRTINESSSKIN    // 脏皮	
	*/
	int attributeIndex;

	virtual void OnChange()
	{
		int Level = GetValue();

		Ped horse = PLAYER::_GET_SADDLE_HORSE_FOR_PLAYER(PLAYER::PLAYER_ID());
		if (PED::IS_PED_ON_MOUNT(PLAYER::PLAYER_PED_ID()))
		{
			horse = PED::GET_MOUNT(PLAYER::PLAYER_PED_ID());	// 获取玩家骑乘的马匹
		}

		ATTRIBUTE::SET_ATTRIBUTE_BASE_RANK(horse, attributeIndex, Level);
	}
public:
	MenuItemSetHorseRank(string caption, int value, int max, int min, int attributeIndex)
		: MenuItemValue(caption, value, max, min), attributeIndex(attributeIndex){}
};


// 马匹选项
MenuBase* CreatePlayerTransportMenu(MenuController* controller)
{
	MenuBase* menu = new MenuBase(new MenuItemTitle(("马匹选项")));
	controller->RegisterMenu(menu);



	menu->AddItem(new MenuItemSetHorseRank(("默契等级"), GetBaseRank(ePedAttribute::PA_BONDING, true), 4, 1, ePedAttribute::PA_BONDING));
	menu->AddItem(new MenuItemSetHorseRank(("生命值等级"), GetBaseRank(ePedAttribute::PA_HEALTH, true), 9, 1, ePedAttribute::PA_HEALTH));
	menu->AddItem(new MenuItemSetHorseRank(("体力等级"), GetBaseRank(ePedAttribute::PA_STAMINA, true), 9, 1, ePedAttribute::PA_STAMINA));
	menu->AddItem(new MenuItemSetHorseRank(("速度等级"), GetBaseRank(ePedAttribute::PA_SPEED, true), 9, 1, ePedAttribute::PA_SPEED));
	menu->AddItem(new MenuItemSetHorseRank(("加速度等级"), GetBaseRank(ePedAttribute::PA_ACCELERATION, true), 9, 1, ePedAttribute::PA_ACCELERATION));

	menu->AddItem(new MenuItemPlayerCores(("满·外围生命值"), 0, true));
	menu->AddItem(new MenuItemPlayerCores(("满·外围黄金体力"), 1, true));
	menu->AddItem(new MenuItemSetCoreRank(("满·核心黄金体力"), 0, true));
	menu->AddItem(new MenuItemSetCoreRank(("满·核心黄金体力"), 1, true));

	menu->AddItem(new MenuItemPlayerHorseInvincible(("马匹无敌")));
	menu->AddItem(new MenuItemPlayerHorseUnlimStamina(("马匹无限体力")));
	//menu->AddItem(new MenuItemVehicleBoost(("助推器")));

	return menu;
}

MenuBase* CreatePlayerChangePlayerModel(MenuController* controller)
{
	auto menu = new MenuBase(new MenuItemTitle(("主角")));
	controller->RegisterMenu(menu);

	menu->AddItem(new MenuItemChangePlayerModel(("亚瑟"), "PLAYER_ZERO"));
	menu->AddItem(new MenuItemChangePlayerModel(("约翰"), "Player_Three"));

	return menu;
}

MenuBase* CreatePlayerChangeModelHorseMenu(MenuController* controller)
{
	auto menu = new MenuBase(new MenuItemListTitle(("马匹模型")));
	controller->RegisterMenu(menu);

	unordered_map<string, vector<pair<string, string>>> breeds;

	for (auto & modelInfo : horse)
	{
		size_t pos = modelInfo.name.find_first_of(' ');
		string breed = modelInfo.name.substr(0, pos);
		string kind = modelInfo.name.substr(pos + 1, modelInfo.name.size() - pos - 1);
		breeds[breed].push_back({ kind, modelInfo.code });
	}
	for (auto & breed : breeds)
	{
		auto breedMenu = new MenuBase(new MenuItemListTitle(breed.first));
		controller->RegisterMenu(breedMenu);
		menu->AddItem(new MenuItemMenu(breed.first, breedMenu));
		for (auto & kindAndModel : breed.second)
			breedMenu->AddItem(new MenuItemChangePlayerModel(kindAndModel.first, kindAndModel.second));
	}

	return menu;
}

MenuBase* CreatePlayerChangeModelAnimalMenu(MenuController* controller)
{
	auto menu = new MenuBase(new MenuItemTitle(("动物模型")));
	controller->RegisterMenu(menu);

	menu->AddItem(new MenuItemMenu(("马匹"), CreatePlayerChangeModelHorseMenu(controller)));

	unordered_map<string, vector<pair<string, string>>> breeds;

	for (auto & modelInfo : animal)
	{
		size_t pos = modelInfo.name.find_first_of(' ');
		string breed = modelInfo.name.substr(0, pos);
		string kind = modelInfo.name.substr(pos + 1, modelInfo.name.size() - pos - 1);
		breeds[breed].push_back({ kind, modelInfo.code });
	}
	for (auto & breed : breeds)
	{
		auto breedMenu = new MenuBase(new MenuItemListTitle(breed.first));
		controller->RegisterMenu(breedMenu);
		menu->AddItem(new MenuItemMenu(breed.first, breedMenu));
		for (auto & kindAndModel : breed.second)
			breedMenu->AddItem(new MenuItemChangePlayerModel(kindAndModel.first, kindAndModel.second));
	}

	return menu;
}

MenuBase* CreatePlayerChangeModelHumanMenu(MenuController* controller)
{
	auto menu = new MenuBase(new MenuItemTitle(("玩家模型")));
	controller->RegisterMenu(menu);

	unordered_map<string, vector<pair<string, string>>> breeds;

	for (auto & modelInfo : ped)
	{
		size_t pos = modelInfo.name.find_first_of(' ');
		string breed = modelInfo.name.substr(0, pos);
		string kind = modelInfo.name.substr(pos + 1, modelInfo.name.size() - pos - 1);
		breeds[breed].push_back({ kind, modelInfo.code });
	}
	for (auto & breed : breeds)
	{
		auto breedMenu = new MenuBase(new MenuItemListTitle(breed.first));
		controller->RegisterMenu(breedMenu);
		menu->AddItem(new MenuItemMenu(breed.first, breedMenu));
		for (auto & kindAndModel : breed.second)
			breedMenu->AddItem(new MenuItemChangePlayerModel(kindAndModel.first, kindAndModel.second));
	}


	return menu;
}

MenuBase* CreatePlayerChangeModelMenu(MenuController* controller)
{
	auto menu = new MenuBase(new MenuItemTitle(("修改模型")));
	controller->RegisterMenu(menu);

	menu->AddItem(new MenuItemMenu(("主角"), CreatePlayerChangePlayerModel(controller)));
	menu->AddItem(new MenuItemMenu(("动物"), CreatePlayerChangeModelAnimalMenu(controller)));
	menu->AddItem(new MenuItemMenu(("人物"), CreatePlayerChangeModelHumanMenu(controller)));
	menu->AddItem(new MenuFastItemDeathProtection(("死亡保护")));


	return menu;
}

MenuBase* CreatePlayerWantedMenu(MenuController* controller)
{
	MenuBase* menu = new MenuBase(new MenuItemTitle(("通缉选项")));
	controller->RegisterMenu(menu);

	menu->AddItem(new MenuItemPlayerClearWanted(("清除悬赏"), true, false));
	menu->AddItem(new MenuItemPlayerClearWanted(("清除通缉"), true, true));
	menu->AddItem(new MenuItemPlayerNeverWanted(("不被通缉")));


	return menu;
}

// 设置荣誉值
MenuBase* MenuItemMiscSetHonor(MenuController* controller)
{
	MenuBase* menu = new MenuBase(new MenuItemTitle(("设置荣誉值")));
	controller->RegisterMenu(menu);

	menu->AddItem(new MenuItemMiscSetHonorS(("最高荣誉值"), -9999));
	menu->AddItem(new MenuItemMiscSetHonorS(("提升一级"), -100));
	menu->AddItem(new MenuItemMiscSetHonorS(("降低一级"), 100));
	menu->AddItem(new MenuItemMiscSetHonorS(("最低荣誉值"), 9999));

	return menu;
}

// 设置玩家大小
class MenuItemSetPlayerScal : public MenuItemValue
{

	virtual void OnChange()
	{
		float scale = GetValue();

		//Ped horse = PLAYER::_GET_SADDLE_HORSE_FOR_PLAYER(PLAYER::PLAYER_ID());	// 获取玩家的马匹
		Ped ped = PLAYER::PLAYER_PED_ID();
		PED::_SET_PED_SCALE(ped, scale);
	}
public:
	MenuItemSetPlayerScal(string caption)
		: MenuItemValue(caption, 1, 4, 0.1, true) {}
};


// 其他选项
MenuBase* CreatePlayerMiscMenu(MenuController* controller)
{
	MenuBase* menu = new MenuBase(new MenuItemTitle(("其他选项")));
	controller->RegisterMenu(menu);

	// _SET_PED_SCALE(Ped ped, float scale)
	menu->AddItem(new MenuItemSetPlayerScal(("玩家大小")));


	menu->AddItem(new MenuItemPlayerEveryoneIgnored(("被所有人忽略")));
	menu->AddItem(new MenuItemPlayerNoiseless(("没有声音")));
	menu->AddItem(new MenuItemPlayerSuperJump(("超级跳")));
	menu->AddItem(new MenuItemPlayerSSuperJump(("真·超级跳")));
	menu->AddItem(new MenuItemPlayerFastRun(("加速跑")));
	menu->AddItem(new MenuItemPlayerBulletTime(("子弹时间[快捷键K]")));
	menu->AddItem(new MenuItemPlayerFastHeal(("快速回血")));

	return menu;
}

// 设置现金
class MenuItemPlayerMoney : public MenuItemDefault
{
	int m_value;
	bool status;
	bool Gold;
	virtual void OnSelect()
	{
		if (status)
		{
			MONEY::_MONEY_DECREMENT_CASH_BALANCE(m_value);
		}
		else
		{
			MONEY::_MONEY_INCREMENT_CASH_BALANCE(m_value, 0x2cd419dc);
		}
	}

public:
	MenuItemPlayerMoney(string caption, int value, bool add, bool isGold)
		: MenuItemDefault(caption),
		m_value(value), status(add), Gold(isGold) {}
};

MenuBase* MenuItemMoneyAndGold(MenuController* controller)
{
	MenuBase* menu = new MenuBase(new MenuItemTitle(("设置现金")));
	controller->RegisterMenu(menu);

	menu->AddItem(new MenuItemPlayerMoney(("现金 +100,000$"), 100000 * 100, false, false));
	menu->AddItem(new MenuItemPlayerMoney(("现金 +10,000$"), 10000 * 100, false, false));
	menu->AddItem(new MenuItemPlayerMoney(("现金 +1,000$"), 1000 * 100, false, false));
	menu->AddItem(new MenuItemPlayerMoney(("现金 +100$"), 100 * 100, false, false));
	menu->AddItem(new MenuItemPlayerMoney(("金钱 -100$"), 100 * 100, true, false));
	menu->AddItem(new MenuItemPlayerMoney(("金钱 -1,000$"), 1000 * 100, true, false));
	menu->AddItem(new MenuItemPlayerMoney(("金钱 -10,000$"), 10000 * 100, true, false));
	menu->AddItem(new MenuItemPlayerMoney(("金钱 -100,000$"), 100000 * 100, true, false));

	return menu;
}


// 设置玩家属性等级
class MenuItemSetPlayerRank : public MenuItemValue
{

	/*
		PA_HEALTH,    // 生命等级
		PA_STAMINA,    // 体力等级
		PA_SPECIALABILITY,    // 死神之眼等级
		PA_COURAGE,    // 勇气、胆量
		PA_AGILITY,    // 敏捷 等级
		PA_SPEED,    // 速度 等级
		PA_ACCELERATION,    // 加速度
		PA_BONDING,    // 默契度
		SA_HUNGER,    // 饥饿值
		SA_FATIGUED,    // 疲劳值
		SA_INEBRIATED,    // 醉酒等级
		SA_POISONED,    // 中毒等级
		SA_BODYHEAT,    // 体温
		SA_BODYWEIGHT,    // 体重
		SA_OVERFED,
		SA_SICKNESS,    // 疾病
		SA_DIRTINESS,    // 肮脏
		SA_DIRTINESSHAT,    // 脏兮兮
		MTR_STRENH,    // 力量
		MTR_GRIT,
		MTR_INSTINCT,    // 直觉
		PA_UNRULINESS,    // 野性
		SA_DIRTINESSSKIN    // 脏皮
	*/
	int attributeIndex;
	bool Bonus;

	virtual void OnChange()
	{
		int Level = GetValue();
			
		ATTRIBUTE::SET_ATTRIBUTE_BASE_RANK(PLAYER::PLAYER_PED_ID(), attributeIndex, Level);

		
	}
public:
	MenuItemSetPlayerRank(string caption, int value, int max, int min, int attributeIndex, bool Bonus = false)
		: MenuItemValue(caption, value, max, min), attributeIndex(attributeIndex), Bonus(Bonus){}
};


// 玩家属性
MenuBase* MenuItemPlayerRank(MenuController* controller)
{
	auto menu = new MenuBase(new MenuItemTitle(("玩家属性")));
	controller->RegisterMenu(menu);


	menu->AddItem(new MenuItemSetPlayerRank(("生命值等级"), GetBaseRank(ePedAttribute::PA_HEALTH), 9, 1, ePedAttribute::PA_HEALTH));
	menu->AddItem(new MenuItemSetPlayerRank(("体力等级"), GetBaseRank(ePedAttribute::PA_STAMINA), 9, 1, ePedAttribute::PA_STAMINA));
	menu->AddItem(new MenuItemSetPlayerRank(("死神之眼等级"), GetBaseRank(ePedAttribute::PA_SPECIALABILITY), 9, 1, ePedAttribute::PA_SPECIALABILITY));

	menu->AddItem(new MenuItemPlayerCores(("满·外围生命值"), 0));
	menu->AddItem(new MenuItemPlayerCores(("满·外围体力"), 1));
	menu->AddItem(new MenuItemPlayerCores(("满·外围死神之眼"), 2));

	menu->AddItem(new MenuItemSetCoreRank(("满·核心生命值"), 0));
	menu->AddItem(new MenuItemSetCoreRank(("满·核心体力"), 1));
	menu->AddItem(new MenuItemSetCoreRank(("满·核心死神之眼"), 2));


	return menu;
}


int textureId = -1;
// 设置服装
class PlayerChangeClothing : public MenuItemDefault
{
	
	virtual void OnSelect()
	{
		/*if (textureId != -1)
		{
			PED::_RESET_PED_TEXTURE_2(textureId);
			PED::_RELEASE_TEXTURE(textureId);
		}*/

		//Hash normal = GAMEPLAY::GET_HASH_KEY("15AE34D2");

		//Hash material = 0x7FC5B1E1;

		//if (PED::IS_PED_MALE(PLAYER::PLAYER_PED_ID()))
		//{
		//	material = 0x50A4BBA9;
		//}

		//textureId = PED::_REQUEST_TEXTURE(albedo, normal, material);
		//int overlay_id = PED::_ADD_TEXTURE_LAYER(textureId, 1, 0, 0, 0, 1, 0);
		//PED::_SET_TEXTURE_LAYER_PALLETE(textureId, overlay_id, 0);
		//PED::_SET_TEXTURE_LAYER_TINT(textureId, overlay_id, 0, 0, 0);
		//PED::_SET_TEXTURE_LAYER_SHEET_GRID_INDEX(textureId, overlay_id, 0);
		//PED::_SET_TEXTURE_LAYER_ALPHA(textureId, overlay_id, 1);

		//// PED::_IS_TEXTURE_VALID
		//while (PED::_IS_TEXTURE_VALID(textureId)) {
		//	WaitAndDraw(100); // WAIT(0);
		//}

		//PED::_OVERRIDE_TEXTURE_ON_PED(PLAYER::PLAYER_PED_ID(), componentHash::heads, textureId);
		//PED::_UPDATE_PED_TEXTURE(textureId);

		//while (STREAMING::HAS_MODEL_LOADED(component))
		//	WaitAndDraw(100); // WAIT(0);
		//
		//Hash component = GAMEPLAY::GET_HASH_KEY("hand_fr1_000_c0_000_mb");


		//PED::_UPDATE_PED_VARIATION(PLAYER::PLAYER_PED_ID(), true, true, true, true, true);
		//PED::_SET_PED_COMPONENT_ENABLED(PLAYER::PLAYER_PED_ID(), component, true, true, true);

		Hash textureId = GAMEPLAY::GET_HASH_KEY("hand_fr1_000_c0_000_mb");
		PED::_RESET_PED_TEXTURE_2(textureId);
		PED::_RELEASE_TEXTURE(textureId);
		int overlay_id = PED::_REQUEST_TEXTURE(0x83C128F6, 0x6856D24A, 0x50A4BBA9);
		PED::_SET_TEXTURE_LAYER_PALLETE(textureId, overlay_id, 0);
		PED::_SET_TEXTURE_LAYER_TINT(textureId, overlay_id, 0, 0, 0);
		
		PED::_SET_TEXTURE_LAYER_SHEET_GRID_INDEX(textureId, overlay_id, 0);
		PED::_SET_TEXTURE_LAYER_ALPHA(textureId, overlay_id, 1.0f);
		while (! PED::_IS_TEXTURE_VALID(textureId))
		{
			WaitAndDraw(100); // WAIT(0);
		}
		PED::_OVERRIDE_TEXTURE_ON_PED(PLAYER::PLAYER_PED_ID(), 0x378AD10C, textureId);
		PED::_UPDATE_PED_TEXTURE(textureId);		
		PED::_UPDATE_PED_VARIATION(PLAYER::PLAYER_PED_ID(), 0, 1, 1, 1, false);


		SetStatusText(to_string(textureId));
		//SetStatusText(("修改完成"));
	}

public:
	PlayerChangeClothing(string caption )
		: MenuItemDefault(caption) {}
};

// 修改服装
MenuBase* CreatePlayerChangeClothingMenu(MenuController* controller)
{
	auto menu = new MenuBase(new MenuItemTitle(("修改服装")));
	controller->RegisterMenu(menu);


	menu->AddItem(new PlayerChangeClothing(("修改服装")));


	return menu;
}




MenuBase* CreatePlayerMenu(MenuController* controller)
{
	MenuBase* menu = new MenuBase(new MenuItemTitle(("玩家选项")));
	controller->RegisterMenu(menu);

	menu->AddItem(new MenuItemMenu(("马匹选项"), CreatePlayerTransportMenu(controller)));
	menu->AddItem(new MenuItemMenu(("修改模型"), CreatePlayerChangeModelMenu(controller)));
	//menu->AddItem(new MenuItemMenu(("修改服装"), CreatePlayerChangeClothingMenu(controller)));	// 无效

	menu->AddItem(new MenuItemMenu(("通缉选项"), CreatePlayerWantedMenu(controller)));
	menu->AddItem(new MenuItemMenu(("设置荣誉值"), MenuItemMiscSetHonor(controller)));
	menu->AddItem(new MenuItemMenu(("设置现金"), MenuItemMoneyAndGold(controller)));
	menu->AddItem(new MenuItemMenu(("设置属性"), MenuItemPlayerRank(controller)));


	menu->AddItem(new MenuItemPlayerFix(("瞬间满血")));
	menu->AddItem(new MenuItemPlayerUnlimAbility(("无限死神之眼")));
	menu->AddItem(new MenuItemPlayerInvincible(("玩家无敌")));
	menu->AddItem(new MenuItemPlayerUnlimStamina(("无限体力")));
	menu->AddItem(new MenuItemMenu(("其他选项"), CreatePlayerMiscMenu(controller)));

	return menu;
}