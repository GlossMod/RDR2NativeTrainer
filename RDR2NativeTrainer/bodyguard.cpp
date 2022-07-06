#pragma execution_character_set("utf-8")
#include "bodyguard.h"

#include "scriptinfo.h"

#include <unordered_map>
#include <ctime>
#include <tchar.h>
#include <windows.h>


//保镖参数
bool featureBodyguardInvincible = false;	//保镖无敌
bool featureBodyguardInfAmmo = false;	//保镖无限子弹
std::vector<Ped> spawnedBodyguards;	//存放保镖


//创建保镖
class MenuItemSpawnBodyguard : public MenuItemDefault
{
	string		m_model;

	virtual string GetModel() { return m_model; }

	virtual void OnSelect()
	{
		DWORD model = GAMEPLAY::GET_HASH_KEY(const_cast<char*>(GetModel().c_str()));		

		if (STREAMING::IS_MODEL_IN_CDIMAGE(model) && STREAMING::IS_MODEL_VALID(model))
		{
			//创建角色	
			STREAMING::REQUEST_MODEL(model, FALSE);		//获取模型

			while (!STREAMING::HAS_MODEL_LOADED(model))	//加载模型
			{
				WaitAndDraw(0); // 等待(0);
			}

			Vector3 coords = ENTITY::GET_OFFSET_FROM_ENTITY_IN_WORLD_COORDS(PLAYER::PLAYER_PED_ID(), 0.0, 3.0, -0.3);	//获取玩家相对坐标
			Ped ped = PED::CREATE_PED(model, coords.x, coords.y, coords.z, static_cast<float>(rand() % 360), 0, 0, 0, 0);	//创建角色

			Player PlayerID = PLAYER::PLAYER_ID();	//获取玩家ID
			int PlayerGroup = PLAYER::GET_PLAYER_GROUP(PlayerID);
			int groupId = PLAYER::GET_PLAYER_GROUP(PLAYER::GET_PLAYER_INDEX());	//获取玩家所在组ID
			Ped pedHandle = PED::CLONE_PED(ped, -852.4f, 160.0f, 65.6f);  // 复制ped数据到pedHandle
			//Ped pedHandle = ped;
			spawnedBodyguards.push_back(pedHandle);

			PED::SET_PED_RANDOM_COMPONENT_VARIATION(pedHandle, TRUE); //创建出一些随机的组建，以方便识别角色的不通，当然你也可以不要此段代码的
			PED::SET_PED_AS_GROUP_MEMBER(pedHandle, groupId);	//设置角色与玩家同组
			PED::SET_GROUP_SEPARATION_RANGE(groupId, 99999999);	//设置脱离组范围
			PED::SET_PED_AS_GROUP_LEADER(PLAYER::PLAYER_PED_ID(), groupId, true);//设置玩家为组长
					
			//给保镖武器
			string weaoon_name = "RIFLE_SPRINGFIELD";
			Hash hash = GAMEPLAY::GET_HASH_KEY(const_cast<char*>(("WEAPON_" + weaoon_name).c_str()));
			WEAPON::GIVE_DELAYED_WEAPON_TO_PED(pedHandle, hash, 100, 1, 0x2cd419dc);
			WEAPON::SET_PED_AMMO(pedHandle, hash, 999);
			WEAPON::SET_CURRENT_PED_WEAPON(pedHandle, hash, 1, 0, 0, 0);

			PED::SET_PED_ACCURACY(pedHandle, 99999); //瞄准精度
			PED::SET_PED_COMBAT_ABILITY(pedHandle, 99999); //战斗能力
			PED::SET_PED_COMBAT_RANGE(pedHandle, 99999);	//设置攻击范围

			// 设置攻击属性
			PED::SET_PED_COMBAT_ATTRIBUTES(pedHandle, eCombatAttribute::CA_ALWAYS_FIGHT, true);
			PED::SET_PED_COMBAT_ATTRIBUTES(pedHandle, eCombatAttribute::CA_PERFECT_ACCURACY, true);
			PED::SET_PED_COMBAT_ATTRIBUTES(pedHandle, eCombatAttribute::CA_SWITCH_TO_DEFENSIVE_IF_IN_COVER, true);
			PED::SET_PED_COMBAT_ATTRIBUTES(pedHandle, eCombatAttribute::CA_USE_VEHICLE_ATTACK, true);

			PED::SET_GROUP_FORMATION(groupId, 1);	//设置组队形


			//设置保镖无敌
			if (featureBodyguardInvincible)
				ENTITY::SET_ENTITY_INVINCIBLE(ped, true);
			else
				ENTITY::SET_ENTITY_INVINCIBLE(ped, false);
			//设置保镖无需换弹
			if (featureBodyguardInfAmmo)
				WEAPON::_SET_PED_INFINITE_AMMO_CLIP(ped, true);
			else
				WEAPON::_SET_PED_INFINITE_AMMO_CLIP(ped, false);


			AI::SET_PED_PATH_CAN_USE_CLIMBOVERS(ped, true);		//设置ped可以翻越
			AI::SET_PED_PATH_CAN_USE_LADDERS(ped, true);		//设置ped可以爬梯子
			AI::SET_PED_PATH_CAN_DROP_FROM_HEIGHT(ped, true);	//设置ped可以跳楼
			
			AI::TASK_COMBAT_HATED_TARGETS_AROUND_PED(pedHandle, 1000, 1, true);	//任务:PED周围与仇恨目标战斗

			PED::SET_GROUP_FORMATION(pedHandle, 1);	//设置组形成
			PED::DELETE_PED(&ped);	//删除人物
			ENTITY::SET_PED_AS_NO_LONGER_NEEDED(&ped);		//设置模型为不需要
			STREAMING::SET_MODEL_AS_NO_LONGER_NEEDED(model);	//不再需要模型

			addLogs("创建保镖成功：" + GetModel());
		}
		
	}
public:
	MenuItemSpawnBodyguard(string caption, string model)
		: MenuItemDefault(caption),
		m_model(model) { }
};

//清空保镖
class MenuItemEmptyBodyguar : public MenuItemDefault
{

	virtual void OnSelect()
	{
		for (auto ped : spawnedBodyguards)
		{
			PED::DELETE_PED(&ped);
		}
		spawnedBodyguards.clear();

		addLogs("清空所有保镖完成！");
	}
public:
	MenuItemEmptyBodyguar(string caption)
		: MenuItemDefault(caption) { }
};

//杀死保镖
class MenuItemKillBodyguar : public MenuItemDefault
{

	virtual void OnSelect()
	{
		for (auto ped : spawnedBodyguards)
		{
			ENTITY::SET_ENTITY_HEALTH(ped, 0, true);
		}
		spawnedBodyguards.clear();

		addLogs("杀死所有保镖完成！");
	}
public:
	MenuItemKillBodyguar(string caption)
		: MenuItemDefault(caption) { }
};

//复制自己
class MenuItemBreakCopyYourself : public MenuItemDefault
{
	string caption;
	virtual string GetModel() { return caption; }

	virtual void OnSelect()
	{
		Ped entity = PLAYER::GET_PLAYER_PED(PLAYER::PLAYER_ID());
		auto model = ENTITY::GET_ENTITY_MODEL(entity);
		if (STREAMING::IS_MODEL_IN_CDIMAGE(model) && STREAMING::IS_MODEL_VALID(model))
		{
			//创建角色	
			STREAMING::REQUEST_MODEL(model, FALSE);		//获取模型

			while (!STREAMING::HAS_MODEL_LOADED(model))	//加载模型
			{
				WaitAndDraw(0); // 等待(0);
			}

			Vector3 coords = ENTITY::GET_OFFSET_FROM_ENTITY_IN_WORLD_COORDS(PLAYER::PLAYER_PED_ID(), 0.0, 3.0, -0.3);	//获取玩家相对坐标
			Ped ped = PED::CREATE_PED(model, coords.x, coords.y, coords.z, static_cast<float>(rand() % 360), 0, 0, 0, 0);	//创建角色

			Player PlayerID = PLAYER::PLAYER_ID();	//获取玩家ID
			int PlayerGroup = PLAYER::GET_PLAYER_GROUP(PlayerID);
			int groupId = PLAYER::GET_PLAYER_GROUP(PLAYER::GET_PLAYER_INDEX());	//获取玩家所在组ID
			Ped pedHandle = PED::CLONE_PED(ped, -852.4f, 160.0f, 65.6f);  // 复制ped数据到pedHandle
			//Ped pedHandle = ped;
			spawnedBodyguards.push_back(pedHandle);

			PED::SET_PED_RANDOM_COMPONENT_VARIATION(pedHandle, TRUE); //创建出一些随机的组建，以方便识别角色的不通，当然你也可以不要此段代码的
			PED::SET_PED_AS_GROUP_MEMBER(pedHandle, groupId);	//设置角色与玩家同组
			PED::SET_GROUP_SEPARATION_RANGE(groupId, 99999999);	//设置脱离组范围
			PED::SET_PED_AS_GROUP_LEADER(PLAYER::PLAYER_PED_ID(), groupId, true);//设置玩家为组长

			//给保镖武器
			string weaoon_name = "RIFLE_SPRINGFIELD";
			Hash hash = GAMEPLAY::GET_HASH_KEY(const_cast<char*>(("WEAPON_" + weaoon_name).c_str()));
			WEAPON::GIVE_DELAYED_WEAPON_TO_PED(pedHandle, hash, 100, 1, 0x2cd419dc);
			WEAPON::SET_PED_AMMO(pedHandle, hash, 999);
			WEAPON::SET_CURRENT_PED_WEAPON(pedHandle, hash, 1, 0, 0, 0);

			PED::SET_PED_ACCURACY(pedHandle, 99999); //瞄准精度
			PED::SET_PED_COMBAT_ABILITY(pedHandle, 99999); //战斗能力
			PED::SET_PED_COMBAT_RANGE(pedHandle, 99999);	//设置攻击范围

			PED::SET_GROUP_FORMATION(groupId, 1);	//设置组队形


			//设置保镖无敌
			if (featureBodyguardInvincible)
				ENTITY::SET_ENTITY_INVINCIBLE(ped, true);
			else
				ENTITY::SET_ENTITY_INVINCIBLE(ped, false);
			//设置保镖无需换弹
			if (featureBodyguardInfAmmo)
				WEAPON::_SET_PED_INFINITE_AMMO_CLIP(ped, true);
			else
				WEAPON::_SET_PED_INFINITE_AMMO_CLIP(ped, false);

			AI::SET_PED_PATH_CAN_USE_CLIMBOVERS(ped, true);		//设置ped可以翻越
			AI::SET_PED_PATH_CAN_USE_LADDERS(ped, true);		//设置ped可以爬梯子
			AI::SET_PED_PATH_CAN_DROP_FROM_HEIGHT(ped, true);	//设置ped可以跳楼

			AI::TASK_COMBAT_HATED_TARGETS_AROUND_PED(pedHandle, 1000, 1, true);	//任务:PED周围与仇恨目标战斗

			PED::SET_GROUP_FORMATION(pedHandle, 1);	//设置组形成
			PED::DELETE_PED(&ped);	//删除人物
			ENTITY::SET_PED_AS_NO_LONGER_NEEDED(&ped);		//设置模型为不需要
			STREAMING::SET_MODEL_AS_NO_LONGER_NEEDED(model);	//不再需要模型

			addLogs("复制自己完成!");
		}
		
	}
public:
	MenuItemBreakCopyYourself(string caption)
		: MenuItemDefault(caption) { }
};

//随机保镖
class MenuItemSpawnBodyguardRandom : public MenuItemSpawnBodyguard
{
	virtual string GetModel()
	{
		while (true)
		{
			int  index = rand() % ped.size();

			dataInfo pedData = ped[index];

			SetStatusText(pedData.name);

			return pedData.code;

		}
	}
public:
	MenuItemSpawnBodyguardRandom(string caption)
		: MenuItemSpawnBodyguard(caption, "") { }
};

//输入保镖模型
class MenuItemInputBodyguarModelCode : public MenuItemSpawnBodyguard
{
	virtual string GetModel()
	{
		const char* ModerCode = "A_C_HORSE_ARABIAN_ROSEGREYBAY";
		GAMEPLAY::DISPLAY_ONSCREEN_KEYBOARD(1, "", "", ModerCode, "", "", "", 64);
		GAMEPLAY::CREATE_STRING(2, "", "");
		//GAMEPLAY::VAR_STRING(2, "", "");

		while (GAMEPLAY::UPDATE_ONSCREEN_KEYBOARD() == 0)
		{
			scriptWait(0);
		}
		if (GAMEPLAY::UPDATE_ONSCREEN_KEYBOARD() != 1)
		{
			return "";
		}
		DWORD model = GAMEPLAY::GET_HASH_KEY(GAMEPLAY::GET_ONSCREEN_KEYBOARD_RESULT());
		if (STREAMING::IS_MODEL_IN_CDIMAGE(model) && STREAMING::IS_MODEL_VALID(model))
		{
			SetStatusText(("成功"));
		}
		else
		{
			SetStatusText(("未找到此模型"));
		}
		ModerCode = GAMEPLAY::GET_ONSCREEN_KEYBOARD_RESULT();
		return GAMEPLAY::GET_ONSCREEN_KEYBOARD_RESULT();
	}
public:
	MenuItemInputBodyguarModelCode(string caption)
		:MenuItemSpawnBodyguard(caption, "") { }
};

//重新召回保镖
class MenuItemFeatureBodyguardForeverInTeam : public MenuItemDefault
{
	virtual void OnFrame()
	{
		for (auto ped : spawnedBodyguards)
		{
			Player PlayerID = PLAYER::PLAYER_ID();	//获取玩家ID
			int groupId = PLAYER::GET_PLAYER_GROUP(PLAYER::GET_PLAYER_INDEX());	//获取玩家所在组ID
			PED::SET_PED_AS_GROUP_MEMBER(ped, groupId);	//设置角色与玩家同组
			PED::SET_PED_AS_GROUP_LEADER(PLAYER::PLAYER_PED_ID(), groupId, true);//设置玩家为组长
			PED::SET_GROUP_FORMATION(ped, 1);	//设置组形成

			AI::TASK_COMBAT_HATED_TARGETS_AROUND_PED(ped, 1000, 1, true);	//任务:PED周围与仇恨目标战斗

		}
		addLogs("重新召回保镖完成！");
	}
public:
	MenuItemFeatureBodyguardForeverInTeam(string caption)
		: MenuItemDefault(caption) {	}
};

//给予保镖武器
class MenuItemGiveBodyguarWeaponList : public MenuItemDefault
{
	string		m_model;

	virtual string GetModel() { return m_model; }

	virtual void OnSelect()
	{
		for (auto ped : spawnedBodyguards)
		{
			WEAPON::REMOVE_ALL_PED_WEAPONS(ped, true, true);	//先移除武器

			Hash hash = GAMEPLAY::GET_HASH_KEY(const_cast<char*>(("WEAPON_" + m_model).c_str()));

			WEAPON::GIVE_DELAYED_WEAPON_TO_PED(ped, hash, 100, 1, 0x2cd419dc);
			WEAPON::SET_PED_AMMO(ped, hash, 100);
			WEAPON::SET_CURRENT_PED_WEAPON(ped, hash, 1, 0, 0, 0);
		}

		addLogs("给予保镖武器：" + GetModel());
	}
public:
	MenuItemGiveBodyguarWeaponList(string caption, string model)
		: MenuItemDefault(caption),
		m_model(model) { }
};

//是否无敌
class MenuItemFeatureBodyguardInvincible : public MenuItemSwitchable
{	
	virtual void OnFrame()
	{
		if (GetState()) {
			featureBodyguardInvincible = true;

			for (auto ped : spawnedBodyguards)
			{
				ENTITY::SET_ENTITY_INVINCIBLE(ped, true);
			}
		}
		else 
		{
			featureBodyguardInvincible = false;
			for (auto ped : spawnedBodyguards)
			{
				ENTITY::SET_ENTITY_INVINCIBLE(ped, false);
			}
		}
	}
public:
	MenuItemFeatureBodyguardInvincible(string caption)
		: MenuItemSwitchable(caption) {}
};

//是否无限子弹
class MenuItemFeatureBodyguardInfAmmo : public MenuItemSwitchable
{
	virtual void OnFrame()
	{
		if (GetState()) {
			featureBodyguardInfAmmo = true;

			for (auto ped : spawnedBodyguards)
			{
				WEAPON::_SET_PED_INFINITE_AMMO_CLIP(ped, true);
			}
		}
		else
		{
			featureBodyguardInfAmmo = false;
			for (auto ped : spawnedBodyguards)
			{
				WEAPON::_SET_PED_INFINITE_AMMO_CLIP(ped, false);
			}
		}
	}
public:
	MenuItemFeatureBodyguardInfAmmo(string caption)
		: MenuItemSwitchable(caption) {}
};


MenuBase* MenuItemGetBodyguar(MenuController* controller)
{
	auto menu = new MenuBase(new MenuItemTitle(("生成保镖")));
	controller->RegisterMenu(menu);

	menu->AddItem(new MenuItemSpawnBodyguardRandom(("随机保镖")));

	auto peopleMenu = new MenuBase(new MenuItemTitle(("生成人物")));
	controller->RegisterMenu(peopleMenu);
	menu->AddItem(new MenuItemMenu(("生成人物"),peopleMenu));
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
		peopleMenu->AddItem(new MenuItemMenu(breed.first, breedMenu));
		for (auto & kindAndModel : breed.second)
			breedMenu->AddItem(new MenuItemSpawnBodyguard(kindAndModel.first, kindAndModel.second));
	}

	auto animalMenu = new MenuBase(new MenuItemTitle(("生成动物")));
	controller->RegisterMenu(animalMenu);
	menu->AddItem(new MenuItemMenu(("生成动物"), animalMenu));
	unordered_map<string, vector<pair<string, string>>> albreeds;
	for (auto & modelInfo : animal)
	{
		size_t pos = modelInfo.name.find_first_of(' ');
		string breed = modelInfo.name.substr(0, pos);
		string kind = modelInfo.name.substr(pos + 1, modelInfo.name.size() - pos - 1);
		albreeds[breed].push_back({ kind, modelInfo.code });
	}
	for (auto & breed : albreeds)
	{
		auto breedMenu = new MenuBase(new MenuItemListTitle(breed.first));
		controller->RegisterMenu(breedMenu);
		animalMenu->AddItem(new MenuItemMenu(breed.first, breedMenu));
		for (auto & kindAndModel : breed.second)
			breedMenu->AddItem(new MenuItemSpawnBodyguard(kindAndModel.first, kindAndModel.second));
	}


	menu->AddItem(new MenuItemInputBodyguarModelCode(("输入模型代码")));


	return menu;
}

MenuBase* MenuItemGiveBodyguarWeapon(MenuController* controller)
{
	auto menu = new MenuBase(new MenuItemListTitle(("获取武器")));
	controller->RegisterMenu(menu);


	unordered_map<string, vector<pair<string, string>>> breeds;
	for (auto & modelInfo : weapon)
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
			breedMenu->AddItem(new MenuItemGiveBodyguarWeaponList(kindAndModel.first, kindAndModel.second));
	}
	return menu;
}

MenuBase* CreateBodyguardSpawnerMenu(MenuController* controller)
{
	auto menu = new MenuBase(new MenuItemTitle(("保镖")));
	controller->RegisterMenu(menu);

	menu->AddItem(new MenuItemMenu(("生成保镖"), MenuItemGetBodyguar(controller)));
	menu->AddItem(new MenuItemBreakCopyYourself(("复制自己")));

	menu->AddItem(new MenuItemMenu(("给予武器"), MenuItemGiveBodyguarWeapon(controller)));
	menu->AddItem(new MenuItemEmptyBodyguar(("清空保镖")));
	menu->AddItem(new MenuItemKillBodyguar(("杀死保镖")));

	menu->AddItem(new MenuItemFeatureBodyguardInvincible(("保镖无敌")));		//保镖无敌
	menu->AddItem(new MenuItemFeatureBodyguardInfAmmo(("无限子弹")));			//无限子弹
	
	return menu;
}