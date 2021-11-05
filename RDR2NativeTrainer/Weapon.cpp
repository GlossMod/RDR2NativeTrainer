#include "Weapon.h"

#include "scriptinfo.h"

#include <unordered_map>
#include <ctime>
#include <tchar.h>
#include <windows.h>
#include "Function.h"

//��ȡ��õ�����
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
//��ȡ����
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
	auto menu = new MenuBase(new MenuItemListTitle(GT("��ȡ����")));
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




//����ˤ��
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


// �䶯���ӵ�
class MenuItemWeaponAnimal : public MenuItemSwitchable
{
	virtual void OnFrame()
	{
		if (GetState())
		{
			Vector3 coords;
			if (WEAPON::GET_PED_LAST_WEAPON_IMPACT_COORD(PLAYER::PLAYER_PED_ID(), &coords))	//����Ƿ���������Ʒ
			{
				Player pay = PLAYER::PLAYER_ID();  //��ȡ���ID
				Entity entity;  //����һ��Entity ���͵ı���
				if (PLAYER::GET_PLAYER_TARGET_ENTITY(pay, &entity) || PLAYER::GET_ENTITY_PLAYER_IS_FREE_AIMING_AT(pay, &entity))  //�жϿ�ǹʱ�Ƿ���׼��NPC �� ����
				{
					// ɾ��ԭ�����NPC
					ENTITY::SET_ENTITY_HEALTH(entity, 0, true);	//����Ŀ������ֵΪ0
					ENTITY::SET_ENTITY_ALPHA(entity, 0, true);	// ����͸��
					ENTITY::SET_ENTITY_COLLISION(entity, false, false);	// �Ƴ���ײ ���������ص�ȥ
					ENTITY::SET_ENTITY_COMPLETELY_DISABLE_COLLISION(entity, true, false);	// �Ƴ���ײ ���������ص�ȥ

					int  index = rand() % animal.size();
					dataInfo animalData = animal[index];
					//SetStatusText(animalData.name);
					DWORD model = GAMEPLAY::GET_HASH_KEY(const_cast<char*>(animalData.code.c_str()));

					STREAMING::REQUEST_MODEL(model, FALSE);		// ����ģ��
					while (!STREAMING::HAS_MODEL_LOADED(model)) {
						WaitAndDraw(0); // WAIT(0);
					}
					Vector3 coords = ENTITY::GET_OFFSET_FROM_ENTITY_IN_WORLD_COORDS(entity, 0.0, 0, 0);	// ��ȡԭ��������λ��
					Ped ped = PED::CREATE_PED(model, coords.x, coords.y, coords.z, static_cast<float>(rand() % 360), 0, 0, 0, 0);	// ��������ģ��

					ENTITY::SET_ENTITY_COORDS(entity, 0, 0, -1000, 0, 0, 1, FALSE);	// ��ԭ���ﴫ�͵�һ��ȥ ������̫Զ��Ϸ���Զ�ɾ����

					PED::SET_PED_VISIBLE(ped, TRUE);				// ���ÿɼ�
					ENTITY::SET_ENTITY_HEALTH(ped, 100, true);		// ����Ŀ������ֵΪ100
					ENTITY::SET_PED_AS_NO_LONGER_NEEDED(&ped);
					STREAMING::SET_MODEL_AS_NO_LONGER_NEEDED(model);// ��Ϊ������Ҫ

				}
			}
		}
	}

public:
	MenuItemWeaponAnimal(string caption)
		: MenuItemSwitchable(caption) {}
};

// �䶯���ӵ�
class MenuItemWeaponShootAnimal : public MenuItemSwitchable
{
	virtual void OnFrame()
	{
		if (GetState())
		{
			Vector3 c2;	// ���λ��
			if (WEAPON::GET_PED_LAST_WEAPON_IMPACT_COORD(PLAYER::PLAYER_PED_ID(), &c2))	//����Ƿ���������Ʒ
			{
				Vector3 c1 = ENTITY::GET_OFFSET_FROM_ENTITY_IN_WORLD_COORDS(PLAYER::PLAYER_PED_ID(), 0, 3, 0.3);	// ���λ��

				int  index = rand() % animal.size();
				dataInfo animalData = animal[index];
				//SetStatusText(animalData.name);
				DWORD model = GAMEPLAY::GET_HASH_KEY(const_cast<char*>(animalData.code.c_str()));

				STREAMING::REQUEST_MODEL(model, FALSE);		// ����ģ��
				while (!STREAMING::HAS_MODEL_LOADED(model)) {
					WaitAndDraw(0); // WAIT(0);
				}
				Ped ped = PED::CREATE_PED(model, c1.x, c1.y, c1.z, static_cast<float>(rand() % 360), 0, 0, 0, 0);	// ��������ģ��
				PED::SET_PED_VISIBLE(ped, TRUE);				// ���ÿɼ�
				ENTITY::SET_ENTITY_HEALTH(ped, 100, true);		// ����Ŀ������ֵΪ100
				ENTITY::SET_PED_AS_NO_LONGER_NEEDED(&ped);
				STREAMING::SET_MODEL_AS_NO_LONGER_NEEDED(model);// ��Ϊ������Ҫ

			

				ENTITY::APPLY_FORCE_TO_ENTITY(ped, 2, 0, 100, 0, 0, 0, 0, 0, true, true, true, true, true);



			}

		}
	}

public:
	MenuItemWeaponShootAnimal(string caption)
		: MenuItemSwitchable(caption) {}
};

// �����ӵ�
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
	auto menu = new MenuBase(new MenuItemListTitle(GT("�����ӵ�")));
	controller->RegisterMenu(menu);


	menu->AddItem(new MenuItemWeaponSuperbullet(GT("�����ӵ�"), -1));

	//menu->AddItem(new MenuItemWeaponShootAnimal(GT("�����ӵ�")));
	menu->AddItem(new MenuItemWeaponAnimal(GT("�䶯���ӵ�")));

	menu->AddItem(new MenuItemRAGDOLL(GT("����ˤ��")));


	return menu;
}

//�����ӵ�
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

//����ǹ
class MenuItemTeleportGun : public MenuItemSwitchable
{
	Ped playerPed = PLAYER::PLAYER_PED_ID();
	virtual void OnSelect()
	{

		bool newState = !GetState();
		if (newState)
		{
			SetStatusText(GT("����ǹ��"));
		}
		else
		{
			SetStatusText(GT("����ǹ��"));
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

//��ȡ��������
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
		SetStatusText(GT("�ѻ�ȡ�������������û����ƥ�ϣ������������������"));
	}
public:
	MenuItemGetAllWeapon(string caption)
		: MenuItemDefault(caption) {}
};

//һ����ɱ
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

//��ս��ɱ
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

//���軻��
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

//��������
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

//׷���ӵ�
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

					Vector3 coords = ENTITY::GET_ENTITY_COORDS(ped, false, false/*idk*/);	// ��ȡped����

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
	auto menu = new MenuBase(new MenuItemTitle(GT("����ѡ��")));
	controller->RegisterMenu(menu);
	menu->AddItem(new MenuItemMenu(GT("��ȡ����"), CreateWeaponSelectMenu(controller)));
	menu->AddItem(new MenuItemMenu(GT("�����ӵ�"), CreateWeaponbulletMenu(controller)));
	menu->AddItem(new MenuItemGetAllWeapon(GT("��ȡ��������")));
	menu->AddItem(new MenuItemTeleportGun(GT("����ǹ")));

	menu->AddItem(new MenuItemWeaponTrackingGuns(GT("׷���ӵ�")));

	menu->AddItem(new MenuItemWeaponPowerfullGuns(GT("һ����ɱ")));
	menu->AddItem(new MenuItemWeaponPowerfullMelee(GT("��ս��ɱ")));
	menu->AddItem(new MenuItemWeaponInfiniteAmmo(GT("�����ӵ�")));
	menu->AddItem(new MenuItemWeaponNoReload(GT("���軻��")));
	menu->AddItem(new MenuItemWeaponDropCurrent(GT("��������")));

	return menu;
}
