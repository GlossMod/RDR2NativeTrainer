#include "Weapon.h"

#include "scriptinfo.h"

#include <unordered_map>
#include <ctime>
#include <tchar.h>
#include <windows.h>
#include "Function.h"

//获取最好的武器
class MenuItemGETBESTPEDWEAPON : public MenuItemDefault
{

	virtual void OnSelect()
	{
		Ped playerPed = PLAYER::PLAYER_PED_ID();
		Hash hash = WEAPON::GET_BEST_PED_WEAPON(playerPed, true, true);

		WEAPON::GIVE_DELAYED_WEAPON_TO_PED(playerPed, hash, 100, 1, 0x2cd419dc);
		WEAPON::SET_PED_AMMO(playerPed, hash, 100);
		WEAPON::SET_CURRENT_PED_WEAPON(playerPed, hash, 1, 0, 0, 0);
	}
public:
	MenuItemGETBESTPEDWEAPON(string caption)
		: MenuItemDefault(caption) {}
};
//获取武器
class MenuItemGiveWeapon : public MenuItemDefault
{
	string m_name;

	virtual void OnSelect()
	{
		Hash hash = GAMEPLAY::GET_HASH_KEY(const_cast<char*>(("WEAPON_" + m_name).c_str()));
		//Ped playerPed = PLAYER::PLAYER_PED_ID();
		Player ply = PLAYER::PLAYER_ID();
		Ped playerPed = PLAYER::GET_PLAYER_PED(ply);
		WEAPON::GIVE_DELAYED_WEAPON_TO_PED(playerPed, hash, 100, true, 0x2cd419dc);
		WEAPON::SET_PED_AMMO(playerPed, hash, 100);
		WEAPON::SET_CURRENT_PED_WEAPON(playerPed, hash, 1, 0, 0, 0);
	}
public:
	MenuItemGiveWeapon(string caption, string weaponName)
		: MenuItemDefault(caption),
		m_name(weaponName) {}
};
MenuBase* CreateWeaponSelectMenu(MenuController* controller)
{
	auto menu = new MenuBase(new MenuItemListTitle(GT("获取武器")));
	controller->RegisterMenu(menu);


	unordered_map<string, vector<pair<string, string>>> breeds;
	for each (auto & modelInfo in weapon)
	{
		size_t pos = modelInfo.name.find_first_of(' ');
		string breed = modelInfo.name.substr(0, pos);
		string kind = modelInfo.name.substr(pos + 1, modelInfo.name.size() - pos - 1);
		breeds[breed].push_back({ kind, modelInfo.code });
	}
	for each (auto & breed in breeds)
	{
		auto breedMenu = new MenuBase(new MenuItemListTitle(breed.first));
		controller->RegisterMenu(breedMenu);
		menu->AddItem(new MenuItemMenu(breed.first, breedMenu));
		for each (auto & kindAndModel in breed.second)
			breedMenu->AddItem(new MenuItemGiveWeapon(kindAndModel.first, kindAndModel.second));
	}

	return menu;
}




//不会摔倒
class MenuItemRAGDOLL : public MenuItemSwitchable
{

	virtual void OnFrame()
	{
		Ped player = PLAYER::PLAYER_PED_ID();

		if (GetState())
		{
			PED::SET_PED_CAN_RAGDOLL(player, false);
		}
		else
		{
			PED::SET_PED_CAN_RAGDOLL(player, true);
		}
	}
public:
	MenuItemRAGDOLL(string caption)
		: MenuItemSwitchable(caption) {}
};


// 变动物子弹
class MenuItemWeaponAnimal : public MenuItemSwitchable
{
	virtual void OnFrame()
	{
		if (GetState())
		{
			Vector3 coords;
			if (WEAPON::GET_PED_LAST_WEAPON_IMPACT_COORD(PLAYER::PLAYER_PED_ID(), &coords))	//玩家是否命中了物品
			{
				Player pay = PLAYER::PLAYER_ID();  //获取玩家ID
				Entity entity;  //申明一个Entity 类型的变量
				if (PLAYER::GET_PLAYER_TARGET_ENTITY(pay, &entity) || PLAYER::GET_ENTITY_PLAYER_IS_FREE_AIMING_AT(pay, &entity))  //判断开枪时是否瞄准了NPC 或 动物
				{
					// 删除原动物或NPC
					ENTITY::SET_ENTITY_HEALTH(entity, 0, true);	//设置目标生命值为0
					ENTITY::SET_ENTITY_ALPHA(entity, 0, true);	// 设置透明
					ENTITY::SET_ENTITY_COLLISION(entity, false, false);	// 移除碰撞 让它掉到地底去
					ENTITY::SET_ENTITY_COMPLETELY_DISABLE_COLLISION(entity, true, false);	// 移除碰撞 让它掉到地底去

					int  index = rand() % animal.size();
					dataInfo animalData = animal[index];
					//SetStatusText(animalData.name);
					DWORD model = GAMEPLAY::GET_HASH_KEY(const_cast<char*>(animalData.code.c_str()));

					STREAMING::REQUEST_MODEL(model, FALSE);		// 搜索模型
					while (!STREAMING::HAS_MODEL_LOADED(model)) {
						WaitAndDraw(0); // WAIT(0);
					}
					Vector3 coords = ENTITY::GET_OFFSET_FROM_ENTITY_IN_WORLD_COORDS(entity, 0.0, 0, 0);	// 获取原动物坐标位置
					Ped ped = PED::CREATE_PED(model, coords.x, coords.y, coords.z, static_cast<float>(rand() % 360), 0, 0, 0, 0);	// 创建动物模型

					ENTITY::SET_ENTITY_COORDS(entity, 0, 0, -1000, 0, 0, 1, FALSE);	// 将原动物传送到一边去 （距离太远游戏会自动删除吗？

					PED::SET_PED_VISIBLE(ped, TRUE);				// 设置可见
					ENTITY::SET_ENTITY_HEALTH(ped, 100, true);		// 设置目标生命值为100
					ENTITY::SET_PED_AS_NO_LONGER_NEEDED(&ped);
					STREAMING::SET_MODEL_AS_NO_LONGER_NEEDED(model);// 设为不再需要

				}
			}
		}
	}

public:
	MenuItemWeaponAnimal(string caption)
		: MenuItemSwitchable(caption) {}
};

// 射动物子弹
class MenuItemWeaponShootAnimal : public MenuItemSwitchable
{
	virtual void OnFrame()
	{
		if (GetState())
		{
			Vector3 c2;	// 射击位置
			if (WEAPON::GET_PED_LAST_WEAPON_IMPACT_COORD(PLAYER::PLAYER_PED_ID(), &c2))	//玩家是否命中了物品
			{
				Vector3 c1 = ENTITY::GET_OFFSET_FROM_ENTITY_IN_WORLD_COORDS(PLAYER::PLAYER_PED_ID(), 0, 3, 0.3);	// 玩家位置

				int  index = rand() % animal.size();
				dataInfo animalData = animal[index];
				//SetStatusText(animalData.name);
				DWORD model = GAMEPLAY::GET_HASH_KEY(const_cast<char*>(animalData.code.c_str()));

				STREAMING::REQUEST_MODEL(model, FALSE);		// 搜索模型
				while (!STREAMING::HAS_MODEL_LOADED(model)) {
					WaitAndDraw(0); // WAIT(0);
				}
				Ped ped = PED::CREATE_PED(model, c1.x, c1.y, c1.z, static_cast<float>(rand() % 360), 0, 0, 0, 0);	// 创建动物模型
				PED::SET_PED_VISIBLE(ped, TRUE);				// 设置可见
				ENTITY::SET_ENTITY_HEALTH(ped, 100, true);		// 设置目标生命值为100
				ENTITY::SET_PED_AS_NO_LONGER_NEEDED(&ped);
				STREAMING::SET_MODEL_AS_NO_LONGER_NEEDED(model);// 设为不再需要

			

				ENTITY::APPLY_FORCE_TO_ENTITY(ped, 2, 0, 100, 0, 0, 0, 0, 0, true, true, true, true, true);



			}

		}
	}

public:
	MenuItemWeaponShootAnimal(string caption)
		: MenuItemSwitchable(caption) {}
};

// 超级子弹
class MenuItemWeaponSuperbullet : public MenuItemValue
{

	virtual void OnFrame()
	{
		if (GetValue() > -1)
		{

			/*Vector3 coords;
			if (WEAPON::GET_PED_LAST_WEAPON_IMPACT_COORD(PLAYER::PLAYER_PED_ID(), &coords))
			{
				GAMEPLAY::_FORCE_LIGHTNING_FLASH_AT_COORDS(coords.x, coords.y, coords.z, -1.0f);
			}*/
			Vector3 coords;
			if (WEAPON::GET_PED_LAST_WEAPON_IMPACT_COORD(PLAYER::PLAYER_PED_ID(), &coords))
			{
				FIRE::ADD_OWNED_EXPLOSION(PLAYER::PLAYER_PED_ID(), coords.x, coords.y, coords.z, GetValue(), 1000, true, false, 1);
			}
		}

	}
public:
	MenuItemWeaponSuperbullet(string caption, int value)
		: MenuItemValue(caption, value, 35, -1) {}
};

MenuBase* CreateWeaponbulletMenu(MenuController* controller)
{
	auto menu = new MenuBase(new MenuItemListTitle(GT("特殊子弹")));
	controller->RegisterMenu(menu);


	menu->AddItem(new MenuItemWeaponSuperbullet(GT("超级子弹"), -1));

	//menu->AddItem(new MenuItemWeaponShootAnimal(GT("动物子弹")));
	menu->AddItem(new MenuItemWeaponAnimal(GT("变动物子弹")));

	menu->AddItem(new MenuItemRAGDOLL(GT("不会摔倒")));


	return menu;
}

//无限子弹
class MenuItemWeaponInfiniteAmmo : public MenuItemSwitchable
{
	virtual void OnFrame()
	{
		Ped playerPed = PLAYER::PLAYER_PED_ID();
		if (GetState())
		{
			WEAPON::_SET_PED_INFINITE_AMMO_CLIP(playerPed, true);
		}
		else
		{
			WEAPON::_SET_PED_INFINITE_AMMO_CLIP(playerPed, false);
		}
	}
public:
	MenuItemWeaponInfiniteAmmo(string caption)
		: MenuItemSwitchable(caption) {}
};

//传送枪
class MenuItemTeleportGun : public MenuItemSwitchable
{
	Ped playerPed = PLAYER::PLAYER_PED_ID();
	virtual void OnSelect()
	{

		bool newState = !GetState();
		if (newState)
		{
			SetStatusText(GT("传送枪开"));
		}
		else
		{
			SetStatusText(GT("传送枪关"));
		}
		SetState(newState);

	}

	virtual void OnFrame()
	{
		if (GetState())
		{
			Entity e = PLAYER::PLAYER_PED_ID();
			if (PED::IS_PED_ON_MOUNT(e))
				e = PED::GET_MOUNT(e);
			else
				if (PED::IS_PED_IN_ANY_VEHICLE(e, FALSE))
					e = PED::GET_VEHICLE_PED_IS_USING(e);

			Vector3 coords;
			if (WEAPON::GET_PED_LAST_WEAPON_IMPACT_COORD(PLAYER::PLAYER_PED_ID(), &coords))
			{
				ENTITY::SET_ENTITY_COORDS(e, coords.x, coords.y, coords.z, 0, 0, 1, false);
			}
		}
	}
public:
	MenuItemTeleportGun(string caption)
		: MenuItemSwitchable(caption) {}
};

//获取所有武器
class MenuItemGetAllWeapon : public MenuItemDefault
{
	virtual void OnSelect()
	{
		for each (auto m_name in weapon)
		{
			Hash hash = GAMEPLAY::GET_HASH_KEY(const_cast<char*>(("WEAPON_" + m_name.code).c_str()));
			Ped playerPed = PLAYER::PLAYER_PED_ID();
			WEAPON::GIVE_DELAYED_WEAPON_TO_PED(playerPed, hash, 100, 1, 0x2cd419dc);
			WEAPON::SET_PED_AMMO(playerPed, hash, 100);
			WEAPON::SET_CURRENT_PED_WEAPON(playerPed, hash, 1, 0, 0, 0);
		}
		SetStatusText(GT("已获取所有武器，如果没在马匹上，多余的武器将被丢弃"));
	}
public:
	MenuItemGetAllWeapon(string caption)
		: MenuItemDefault(caption) {}
};

//一击必杀
class MenuItemWeaponPowerfullGuns : public MenuItemSwitchable
{
	virtual void OnSelect()
	{
		bool newState = !GetState();
		if (!newState)
			PLAYER::SET_PLAYER_WEAPON_DAMAGE_MODIFIER(PLAYER::PLAYER_ID(), 1.0);
		SetState(newState);
	}
	virtual void OnFrame()
	{
		if (GetState())
			PLAYER::SET_PLAYER_WEAPON_DAMAGE_MODIFIER(PLAYER::PLAYER_ID(), 100.0);
	}
public:
	MenuItemWeaponPowerfullGuns(string caption)
		: MenuItemSwitchable(caption) {}
};

//近战必杀
class MenuItemWeaponPowerfullMelee : public MenuItemSwitchable
{
	virtual void OnSelect()
	{
		bool newState = !GetState();
		if (!newState)
			PLAYER::SET_PLAYER_MELEE_WEAPON_DAMAGE_MODIFIER(PLAYER::PLAYER_ID(), 1.0);
		SetState(newState);
	}
	virtual void OnFrame()
	{
		if (GetState())
			PLAYER::SET_PLAYER_MELEE_WEAPON_DAMAGE_MODIFIER(PLAYER::PLAYER_ID(), 100.0);
	}
public:
	MenuItemWeaponPowerfullMelee(string caption)
		: MenuItemSwitchable(caption) {}
};

//无需换弹
class MenuItemWeaponNoReload : public MenuItemSwitchable
{
	virtual void OnFrame()
	{
		if (!GetState())
			return;
		Ped playerPed = PLAYER::PLAYER_PED_ID();
		Hash cur;
		if (WEAPON::GET_CURRENT_PED_WEAPON(playerPed, &cur, 0, 0, 0) && WEAPON::IS_WEAPON_VALID(cur))
		{
			int maxAmmo;
			if (WEAPON::GET_MAX_AMMO(playerPed, &maxAmmo, cur))
				WEAPON::SET_PED_AMMO(playerPed, cur, maxAmmo);
			maxAmmo = WEAPON::GET_MAX_AMMO_IN_CLIP(playerPed, cur, 1);
			if (maxAmmo > 0)
				WEAPON::SET_AMMO_IN_CLIP(playerPed, cur, maxAmmo);
		}
	}
public:
	MenuItemWeaponNoReload(string caption)
		: MenuItemSwitchable(caption) {}
};

//丢弃武器
class MenuItemWeaponDropCurrent : public MenuItemDefault
{
	virtual void OnSelect()
	{
		Ped playerPed = PLAYER::PLAYER_PED_ID();
		Hash unarmed = GAMEPLAY::GET_HASH_KEY("WEAPON_UNARMED");
		Hash cur;
		if (WEAPON::GET_CURRENT_PED_WEAPON(playerPed, &cur, 0, 0, 0) && WEAPON::IS_WEAPON_VALID(cur) && cur != unarmed)
			WEAPON::SET_PED_DROPS_INVENTORY_WEAPON(playerPed, cur, 0.0, 0.0, 0.0, 1);
	}
public:
	MenuItemWeaponDropCurrent(string caption)
		: MenuItemDefault(caption) {}
};

//追踪子弹
class MenuItemWeaponTrackingGuns : public MenuItemSwitchable
{
	
	virtual void OnFrame()
	{
		if (GetState()) {
			if (PED::IS_PED_SHOOTING(PLAYER::PLAYER_PED_ID()))
			{
				runOnAllNearbyPedsToPlayer([](auto ped) {

					Hash weaponHash = 0;
					PED::EXPLODE_PED_HEAD(ped, weaponHash);

					Vector3 coords = ENTITY::GET_ENTITY_COORDS(ped, false, false/*idk*/);	// 获取ped坐标

					FIRE::ADD_OWNED_EXPLOSION(PLAYER::PLAYER_PED_ID(), coords.x, coords.y, coords.z , eExplosionTag::EXP_TAG_BULLET, 1000, false, true, 0);


				});
			}
		}

	}
public:
	MenuItemWeaponTrackingGuns(string caption)
		: MenuItemSwitchable(caption) {}
};

MenuBase* CreateWeaponMenu(MenuController* controller)
{
	auto menu = new MenuBase(new MenuItemTitle(GT("武器选项")));
	controller->RegisterMenu(menu);
	menu->AddItem(new MenuItemMenu(GT("获取武器"), CreateWeaponSelectMenu(controller)));
	menu->AddItem(new MenuItemMenu(GT("特殊子弹"), CreateWeaponbulletMenu(controller)));
	menu->AddItem(new MenuItemGetAllWeapon(GT("获取所有武器")));
	menu->AddItem(new MenuItemTeleportGun(GT("传送枪")));

	menu->AddItem(new MenuItemWeaponTrackingGuns(GT("追踪子弹")));

	menu->AddItem(new MenuItemWeaponPowerfullGuns(GT("一击必杀")));
	menu->AddItem(new MenuItemWeaponPowerfullMelee(GT("近战必杀")));
	menu->AddItem(new MenuItemWeaponInfiniteAmmo(GT("无限子弹")));
	menu->AddItem(new MenuItemWeaponNoReload(GT("无需换弹")));
	menu->AddItem(new MenuItemWeaponDropCurrent(GT("丢弃武器")));

	return menu;
}
