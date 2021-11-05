#include "player.h"

#include "scriptinfo.h"

#include <unordered_map>
#include <ctime>
#include <tchar.h>
#include <windows.h>

//�Ƿ��޸�������ģ��
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
			SetStatusText(GT("���� / NUM9\n���� / NUM6"));
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
			SetStatusText(GT("��Һ���Ѫ������"));
		}
		else
			if (PED::IS_PED_IN_ANY_VEHICLE(playerPed, FALSE))
			{
				Vehicle veh = PED::GET_VEHICLE_PED_IS_USING(playerPed);
				ENTITY::SET_ENTITY_HEALTH(veh, ENTITY::GET_ENTITY_MAX_HEALTH(veh, FALSE), FALSE);
				SetStatusText(GT("��Һ��ؾ�Ѫ������"));
			}
			else
				SetStatusText(GT("���Ѫ����"));
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

//�桤������
class MenuItemPlayerSSuperJump : public MenuItemSwitchable
{
	int FlySpeed = 30;

	virtual void OnSelect()
	{
		Ped player = PLAYER::PLAYER_PED_ID();
		bool newstate = !GetState();
		if (newstate)
		{
			SetStatusText(GT("\"�桤������\"��\"����ģʽ\"�г�ͻ���벻Ҫͬʱʹ��"));
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

			if (IsKeyDownLong(0x57))	//����W��
			{
				ENTITY::APPLY_FORCE_TO_ENTITY(player, 1, 0, FlySpeed, 0, 0, 0, 0, 0, true, true, true, true, true);
				ENTITY::SET_ENTITY_VELOCITY(player, 0, 0, 0);
			}

			if (IsKeyDownLong(VK_CONTROL))	//����Ctrl��
			{
				ENTITY::APPLY_FORCE_TO_ENTITY(player, 1, 0, 0, -FlySpeed, 0, 0, 0, 0, true, true, true, true, true);
				ENTITY::SET_ENTITY_VELOCITY(player, 0, 0, 0);
			}

			if (IsKeyDownLong(VK_SPACE))	//�����ո��
			{
				ENTITY::APPLY_FORCE_TO_ENTITY(player, 1, 0, 0, FlySpeed, 0, 0, 0, 0, true, true, true, true, true);
				ENTITY::SET_ENTITY_VELOCITY(player, 0, 0, 0);
			}

			if (PED::IS_PED_RAGDOLL(player))	//��Ҵ��ڵ���״̬
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

//������
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

//�ӵ�ʱ��
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
		SetStatusText(GT("��ǰ�������:\n\n"
			"����: " + to_string(PURSUIT::_GET_BOUNTY_FOR_PLAYER(player) / 100) + "\n" +
			"ͨ���ȼ�: " + to_string(PURSUIT::_GET_WANTED_INTENSITY_FOR_PLAYER(player) / 100)));
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

//��������ֵ
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
		Vector3 coords = { 0,0,0 };		// ������Ϊ0,0,0��λ�ô���һ��NPC
		Ped ped = PED::CREATE_PED(model, coords.x, coords.y, coords.z, 0.0, 0, 0, 0, 0);
		PED::SET_PED_VISIBLE(ped, false);

		DECORATOR::DECOR_SET_INT(ped, "honor_override", Lave);	// ����NPC�ƶ�ȼ�

		AI::TASK_COMBAT_PED(ped, PLAYER::PLAYER_PED_ID(), 0, 0);	// ����NPC����ҳ��
		//ENTITY::SET_PED_AS_NO_LONGER_NEEDED(&ped);
		STREAMING::SET_MODEL_AS_NO_LONGER_NEEDED(model);	// ����ģ���Ѳ���Ҫ

		FIRE::ADD_OWNED_EXPLOSION(PLAYER::PLAYER_PED_ID(), coords.x, coords.y, coords.z, 11, 99999, true, false, 1);	// �������ݽ�NPCɱ��

	}
public:
	MenuItemMiscSetHonorS(string caption, int Set)
		: MenuItemDefault(caption), Lave(Set) {}
};


DWORD playerModel = NULL;

//�޸�����ģ��
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
		if (STREAMING::IS_MODEL_IN_CDIMAGE(model) && STREAMING::IS_MODEL_VALID(model))	//�ж�ģ���Ƿ����
		{
			{
				UINT64* ptr1 = getGlobalPtr(0x28) + 0x27;
				UINT64* ptr2 = getGlobalPtr(((DWORD)7 << 18) | 0x1D890E) + 2;		//ͨ����Ϸ����ָ���ȡ����һ����֪����ʲô�����int���͵�ֵ
				UINT64 bcp1 = *ptr1;
				UINT64 bcp2 = *ptr2;
				*ptr1 = *ptr2 = model;
				WaitAndDraw(1000);
				Ped playerPed = PLAYER::PLAYER_PED_ID();
				PED::SET_PED_VISIBLE(playerPed, TRUE);	//����ped�ɼ�
				if (ENTITY::GET_ENTITY_MODEL(playerPed) != model)	//�жϵ�ǰ�Ƿ�ΪҪ�޸ĵ�����
				{
					*ptr1 = bcp1;
					*ptr2 = bcp2;
				}


				////  �ָ�����ֵ
				//ATTRIBUTE::_ENABLE_ATTRIBUTE_CORE_OVERPOWER(PLAYER::PLAYER_PED_ID(), 0, 100, true);
				//ATTRIBUTE::_ENABLE_ATTRIBUTE_CORE_OVERPOWER(PLAYER::PLAYER_PED_ID(), 1, 100, true);
				//ATTRIBUTE::_ENABLE_ATTRIBUTE_CORE_OVERPOWER(PLAYER::PLAYER_PED_ID(), 2, 100, true);

				//// �������Եȼ����
				//ATTRIBUTE::SET_ATTRIBUTE_BASE_RANK(PLAYER::PLAYER_PED_ID(), ePedAttribute::PA_HEALTH, 10);
				//ATTRIBUTE::SET_ATTRIBUTE_BASE_RANK(PLAYER::PLAYER_PED_ID(), 1, 10);
				//ATTRIBUTE::SET_ATTRIBUTE_BASE_RANK(PLAYER::PLAYER_PED_ID(), 2, 10);

				ENTITY::SET_ENTITY_HEALTH(playerPed, ENTITY::GET_ENTITY_MAX_HEALTH(playerPed, FALSE), FALSE);	//�ظ��������ֵ
				PED::CLEAR_PED_WETNESS(playerPed);	// ���PEDʪ��
				PLAYER::RESTORE_PLAYER_STAMINA(PLAYER::PLAYER_ID(), 100.0);		//�ظ��������
				PLAYER::RESTORE_SPECIAL_ABILITY(PLAYER::PLAYER_ID(), -1, FALSE);	//�ظ������������

				
			}
			
			{
				//playerModel = model;
				//Player play = PLAYER::PLAYER_ID();
				//if (ENTITY::GET_ENTITY_MODEL(PLAYER::PLAYER_PED_ID()) != model)	//�жϵ�ǰ�Ƿ�ΪҪ�޸ĵ�����
				//{
				//	STREAMING::REQUEST_MODEL(model, true);	//��ȡģ��
				//	while (!STREAMING::HAS_MODEL_LOADED(model))	//ģ���Ƿ��Ѿ�����
				//	{
				//		WAIT(0);
				//	}
				//	PLAYER::SET_PLAYER_MODEL(play, model, false);	//��������ģ��
				//	//PED::SET_PED_RANDOM_COMPONENT_VARIATION(PLAYER::PLAYER_PED_ID(), 0);	//������������仯
				//	//Hash ptr1 = 0x28 + 0x27;
				//	int skinDetailMenuValue = 0;
				//	//PED::_SET_PED_COMPONENT_VARIATION(play, model, model, true, false, 0);	//������������仯
				//	int currentDrawable = 1;
				//	//PED::_SET_PED_COMPONENT_VARIATION(PLAYER::PLAYER_PED_ID(), skinDetailMenuValue, currentDrawable, 0, 0, 0);
				//	//PED::SET_PED_DEFAULT_COMPONENT_VARIATION(play);								
				//	WaitAndDraw(1000);
				//	Ped playerPed = PLAYER::PLAYER_PED_ID();
				//	PED::SET_PED_VISIBLE(playerPed, true);	//���ÿɼ�
				//	ENTITY::SET_ENTITY_HEALTH(playerPed, ENTITY::GET_ENTITY_MAX_HEALTH(playerPed, true), true);	//��������ֵ
				//	PED::CLEAR_PED_WETNESS(playerPed);	//���PEDʪ��
				//	PLAYER::RESTORE_PLAYER_STAMINA(playerPed, 100.0);	//�ָ��������
				//	PLAYER::RESTORE_SPECIAL_ABILITY(playerPed, -1, true);	//�ָ���������������֮�ۣ�
				//	STREAMING::SET_MODEL_AS_NO_LONGER_NEEDED(model);	//�����Ѳ�����Ҫģ��
				//	PLAYER::SET_PLAYER_LOCKON(play, true);		//�������
				//}
			}
			
		}
	}
public:
	MenuItemChangePlayerModel(string caption, string model)
		: MenuItemDefault(caption),
		m_model(model) { }
};

//��������
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
				//��������������ִ��
				DWORD model = GAMEPLAY::GET_HASH_KEY("PLAYER_ZERO");
				if (STREAMING::IS_MODEL_IN_CDIMAGE(model) && STREAMING::IS_MODEL_VALID(model))
				{
					UINT64* ptr1 = getGlobalPtr(0x28) + 0x27;
					UINT64* ptr2 = getGlobalPtr(((DWORD)7 << 18) | 0x1D890E) + 2;		//ͨ����Ϸ����ָ���ȡ����һ����֪����ʲô�����int���͵�ֵ

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

// ���ûƽ�����
class MenuItemPlayerCores : public MenuItemDefault
{
	int attributeIndex;
	bool inHorse;

	virtual void OnSelect()
	{
		if (inHorse)
		{
			//Ped horse = PLAYER::_GET_SADDLE_HORSE_FOR_PLAYER(PLAYER::PLAYER_ID());	// ��ȡ��ҵ���ƥ
			Ped horse = PLAYER::_GET_SADDLE_HORSE_FOR_PLAYER(PLAYER::PLAYER_ID());
			if (PED::IS_PED_ON_MOUNT(PLAYER::PLAYER_PED_ID()))
			{
				horse = PED::GET_MOUNT(PLAYER::PLAYER_PED_ID());	// ��ȡ�����˵���ƥ
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

// ���ú���ֵ
class MenuItemSetCoreRank : public MenuItemDefault
{
	int coreIndex;
	bool inHorse;

	virtual void OnSelect()
	{
		if (inHorse)
		{
			//Ped horse = PLAYER::_GET_SADDLE_HORSE_FOR_PLAYER(PLAYER::PLAYER_ID());	// ��ȡ��ҵ���ƥ
			Ped horse = PLAYER::_GET_SADDLE_HORSE_FOR_PLAYER(PLAYER::PLAYER_ID());
			if (PED::IS_PED_ON_MOUNT(PLAYER::PLAYER_PED_ID()))
			{
				horse = PED::GET_MOUNT(PLAYER::PLAYER_PED_ID());	// ��ȡ�����˵���ƥ
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


// ������ƥ���Եȼ�
class MenuItemSetHorseRank : public MenuItemValue
{

	/*
		PA_HEALTH,    // �����ȼ�
		PA_STAMINA,    // �����ȼ�
		PA_SPECIALABILITY,    // ����֮�۵ȼ�
		PA_COURAGE,    // ���������� 
		PA_AGILITY,    // ���� �ȼ�
		PA_SPEED,    // �ٶ� �ȼ�
		PA_ACCELERATION,    // ���ٶ�
		PA_BONDING,    // Ĭ����
		SA_HUNGER,    // ����ֵ
		SA_FATIGUED,    // ƣ��ֵ
		SA_INEBRIATED,    // ��Ƶȼ�
		SA_POISONED,    // �ж��ȼ�
		SA_BODYHEAT,    // ����
		SA_BODYWEIGHT,    // ����
		SA_OVERFED,   
		SA_SICKNESS,    // ����
		SA_DIRTINESS,    // ����
		SA_DIRTINESSHAT,    // ������
		MTR_STRENGTH,    // ����
		MTR_GRIT,   
		MTR_INSTINCT,    // ֱ��
		PA_UNRULINESS,    // Ұ��
		SA_DIRTINESSSKIN    // ��Ƥ	
	*/
	int attributeIndex;

	virtual void OnChange()
	{
		int Level = GetValue();

		Ped horse = PLAYER::_GET_SADDLE_HORSE_FOR_PLAYER(PLAYER::PLAYER_ID());
		if (PED::IS_PED_ON_MOUNT(PLAYER::PLAYER_PED_ID()))
		{
			horse = PED::GET_MOUNT(PLAYER::PLAYER_PED_ID());	// ��ȡ�����˵���ƥ
		}

		ATTRIBUTE::SET_ATTRIBUTE_BASE_RANK(horse, attributeIndex, Level);
	}
public:
	MenuItemSetHorseRank(string caption, int value, int max, int min, int attributeIndex)
		: MenuItemValue(caption, value, max, min), attributeIndex(attributeIndex){}
};


// ��ƥѡ��
MenuBase* CreatePlayerTransportMenu(MenuController* controller)
{
	MenuBase* menu = new MenuBase(new MenuItemTitle(GT("��ƥѡ��")));
	controller->RegisterMenu(menu);



	menu->AddItem(new MenuItemSetHorseRank(GT("Ĭ���ȼ�"), GetBaseRank(ePedAttribute::PA_BONDING, true), 4, 1, ePedAttribute::PA_BONDING));
	menu->AddItem(new MenuItemSetHorseRank(GT("����ֵ�ȼ�"), GetBaseRank(ePedAttribute::PA_HEALTH, true), 9, 1, ePedAttribute::PA_HEALTH));
	menu->AddItem(new MenuItemSetHorseRank(GT("�����ȼ�"), GetBaseRank(ePedAttribute::PA_STAMINA, true), 9, 1, ePedAttribute::PA_STAMINA));
	menu->AddItem(new MenuItemSetHorseRank(GT("�ٶȵȼ�"), GetBaseRank(ePedAttribute::PA_SPEED, true), 9, 1, ePedAttribute::PA_SPEED));
	menu->AddItem(new MenuItemSetHorseRank(GT("���ٶȵȼ�"), GetBaseRank(ePedAttribute::PA_ACCELERATION, true), 9, 1, ePedAttribute::PA_ACCELERATION));

	menu->AddItem(new MenuItemPlayerCores(GT("������Χ����ֵ"), 0, true));
	menu->AddItem(new MenuItemPlayerCores(GT("������Χ�ƽ�����"), 1, true));
	menu->AddItem(new MenuItemSetCoreRank(GT("�������Ļƽ�����"), 0, true));
	menu->AddItem(new MenuItemSetCoreRank(GT("�������Ļƽ�����"), 1, true));

	menu->AddItem(new MenuItemPlayerHorseInvincible(GT("��ƥ�޵�")));
	menu->AddItem(new MenuItemPlayerHorseUnlimStamina(GT("��ƥ��������")));
	//menu->AddItem(new MenuItemVehicleBoost(GT("������")));

	return menu;
}

MenuBase* CreatePlayerChangePlayerModel(MenuController* controller)
{
	auto menu = new MenuBase(new MenuItemTitle(GT("����")));
	controller->RegisterMenu(menu);

	menu->AddItem(new MenuItemChangePlayerModel(GT("��ɪ"), "PLAYER_ZERO"));
	menu->AddItem(new MenuItemChangePlayerModel(GT("Լ��"), "Player_Three"));

	return menu;
}

MenuBase* CreatePlayerChangeModelHorseMenu(MenuController* controller)
{
	auto menu = new MenuBase(new MenuItemListTitle(GT("��ƥģ��")));
	controller->RegisterMenu(menu);

	unordered_map<string, vector<pair<string, string>>> breeds;

	for each (auto & modelInfo in horse)
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
			breedMenu->AddItem(new MenuItemChangePlayerModel(kindAndModel.first, kindAndModel.second));
	}

	return menu;
}

MenuBase* CreatePlayerChangeModelAnimalMenu(MenuController* controller)
{
	auto menu = new MenuBase(new MenuItemTitle(GT("����ģ��")));
	controller->RegisterMenu(menu);

	menu->AddItem(new MenuItemMenu(GT("��ƥ"), CreatePlayerChangeModelHorseMenu(controller)));

	unordered_map<string, vector<pair<string, string>>> breeds;

	for each (auto & modelInfo in animal)
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
			breedMenu->AddItem(new MenuItemChangePlayerModel(kindAndModel.first, kindAndModel.second));
	}

	return menu;
}

MenuBase* CreatePlayerChangeModelHumanMenu(MenuController* controller)
{
	auto menu = new MenuBase(new MenuItemTitle(GT("���ģ��")));
	controller->RegisterMenu(menu);

	unordered_map<string, vector<pair<string, string>>> breeds;

	for each (auto & modelInfo in ped)
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
			breedMenu->AddItem(new MenuItemChangePlayerModel(kindAndModel.first, kindAndModel.second));
	}

	return menu;
}

MenuBase* CreatePlayerChangeModelMenu(MenuController* controller)
{
	auto menu = new MenuBase(new MenuItemTitle(GT("�޸�ģ��")));
	controller->RegisterMenu(menu);

	menu->AddItem(new MenuItemMenu(GT("����"), CreatePlayerChangePlayerModel(controller)));
	menu->AddItem(new MenuItemMenu(GT("����"), CreatePlayerChangeModelAnimalMenu(controller)));
	menu->AddItem(new MenuItemMenu(GT("����"), CreatePlayerChangeModelHumanMenu(controller)));
	menu->AddItem(new MenuFastItemDeathProtection(GT("��������")));


	return menu;
}

MenuBase* CreatePlayerWantedMenu(MenuController* controller)
{
	MenuBase* menu = new MenuBase(new MenuItemTitle(GT("ͨ��ѡ��")));
	controller->RegisterMenu(menu);

	menu->AddItem(new MenuItemPlayerClearWanted(GT("�������"), true, false));
	menu->AddItem(new MenuItemPlayerClearWanted(GT("���ͨ��"), true, true));
	menu->AddItem(new MenuItemPlayerNeverWanted(GT("����ͨ��")));


	return menu;
}

// ��������ֵ
MenuBase* MenuItemMiscSetHonor(MenuController* controller)
{
	MenuBase* menu = new MenuBase(new MenuItemTitle(GT("��������ֵ")));
	controller->RegisterMenu(menu);

	menu->AddItem(new MenuItemMiscSetHonorS(GT("�������ֵ"), -9999));
	menu->AddItem(new MenuItemMiscSetHonorS(GT("����һ��"), -100));
	menu->AddItem(new MenuItemMiscSetHonorS(GT("����һ��"), 100));
	menu->AddItem(new MenuItemMiscSetHonorS(GT("�������ֵ"), 9999));

	return menu;
}

// ������Ҵ�С
class MenuItemSetPlayerScal : public MenuItemValue
{

	virtual void OnChange()
	{
		float scale = GetValue();

		//Ped horse = PLAYER::_GET_SADDLE_HORSE_FOR_PLAYER(PLAYER::PLAYER_ID());	// ��ȡ��ҵ���ƥ
		Ped ped = PLAYER::PLAYER_PED_ID();
		PED::_SET_PED_SCALE(ped, scale);
	}
public:
	MenuItemSetPlayerScal(string caption)
		: MenuItemValue(caption, 1, 4, 0.1, true) {}
};


// ����ѡ��
MenuBase* CreatePlayerMiscMenu(MenuController* controller)
{
	MenuBase* menu = new MenuBase(new MenuItemTitle(GT("����ѡ��")));
	controller->RegisterMenu(menu);

	// _SET_PED_SCALE(Ped ped, float scale)
	menu->AddItem(new MenuItemSetPlayerScal(GT("��Ҵ�С")));


	menu->AddItem(new MenuItemPlayerEveryoneIgnored(GT("�������˺���")));
	menu->AddItem(new MenuItemPlayerNoiseless(GT("û������")));
	menu->AddItem(new MenuItemPlayerSuperJump(GT("������")));
	menu->AddItem(new MenuItemPlayerSSuperJump(GT("�桤������")));
	menu->AddItem(new MenuItemPlayerFastRun(GT("������")));
	menu->AddItem(new MenuItemPlayerBulletTime(GT("�ӵ�ʱ��[��ݼ�K]")));
	menu->AddItem(new MenuItemPlayerFastHeal(GT("���ٻ�Ѫ")));

	return menu;
}

// �����ֽ�
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
	MenuBase* menu = new MenuBase(new MenuItemTitle(GT("�����ֽ�")));
	controller->RegisterMenu(menu);

	menu->AddItem(new MenuItemPlayerMoney(GT("�ֽ� +100,000$"), 100000 * 100, false, false));
	menu->AddItem(new MenuItemPlayerMoney(GT("�ֽ� +10,000$"), 10000 * 100, false, false));
	menu->AddItem(new MenuItemPlayerMoney(GT("�ֽ� +1,000$"), 1000 * 100, false, false));
	menu->AddItem(new MenuItemPlayerMoney(GT("�ֽ� +100$"), 100 * 100, false, false));
	menu->AddItem(new MenuItemPlayerMoney(GT("��Ǯ -100$"), 100 * 100, true, false));
	menu->AddItem(new MenuItemPlayerMoney(GT("��Ǯ -1,000$"), 1000 * 100, true, false));
	menu->AddItem(new MenuItemPlayerMoney(GT("��Ǯ -10,000$"), 10000 * 100, true, false));
	menu->AddItem(new MenuItemPlayerMoney(GT("��Ǯ -100,000$"), 100000 * 100, true, false));

	return menu;
}


// ����������Եȼ�
class MenuItemSetPlayerRank : public MenuItemValue
{

	/*
		PA_HEALTH,    // �����ȼ�
		PA_STAMINA,    // �����ȼ�
		PA_SPECIALABILITY,    // ����֮�۵ȼ�
		PA_COURAGE,    // ����������
		PA_AGILITY,    // ���� �ȼ�
		PA_SPEED,    // �ٶ� �ȼ�
		PA_ACCELERATION,    // ���ٶ�
		PA_BONDING,    // Ĭ����
		SA_HUNGER,    // ����ֵ
		SA_FATIGUED,    // ƣ��ֵ
		SA_INEBRIATED,    // ��Ƶȼ�
		SA_POISONED,    // �ж��ȼ�
		SA_BODYHEAT,    // ����
		SA_BODYWEIGHT,    // ����
		SA_OVERFED,
		SA_SICKNESS,    // ����
		SA_DIRTINESS,    // ����
		SA_DIRTINESSHAT,    // ������
		MTR_STRENGTH,    // ����
		MTR_GRIT,
		MTR_INSTINCT,    // ֱ��
		PA_UNRULINESS,    // Ұ��
		SA_DIRTINESSSKIN    // ��Ƥ
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


// �������
MenuBase* MenuItemPlayerRank(MenuController* controller)
{
	auto menu = new MenuBase(new MenuItemTitle(GT("�������")));
	controller->RegisterMenu(menu);


	menu->AddItem(new MenuItemSetPlayerRank(GT("����ֵ�ȼ�"), GetBaseRank(ePedAttribute::PA_HEALTH), 9, 1, ePedAttribute::PA_HEALTH));
	menu->AddItem(new MenuItemSetPlayerRank(GT("�����ȼ�"), GetBaseRank(ePedAttribute::PA_STAMINA), 9, 1, ePedAttribute::PA_STAMINA));
	menu->AddItem(new MenuItemSetPlayerRank(GT("����֮�۵ȼ�"), GetBaseRank(ePedAttribute::PA_SPECIALABILITY), 9, 1, ePedAttribute::PA_SPECIALABILITY));

	menu->AddItem(new MenuItemPlayerCores(GT("������Χ����ֵ"), 0));
	menu->AddItem(new MenuItemPlayerCores(GT("������Χ����"), 1));
	menu->AddItem(new MenuItemPlayerCores(GT("������Χ����֮��"), 2));

	menu->AddItem(new MenuItemSetCoreRank(GT("������������ֵ"), 0));
	menu->AddItem(new MenuItemSetCoreRank(GT("������������"), 1));
	menu->AddItem(new MenuItemSetCoreRank(GT("������������֮��"), 2));


	return menu;
}

MenuBase* CreatePlayerMenu(MenuController* controller)
{
	MenuBase* menu = new MenuBase(new MenuItemTitle(GT("���ѡ��")));
	controller->RegisterMenu(menu);

	menu->AddItem(new MenuItemMenu(GT("��ƥѡ��"), CreatePlayerTransportMenu(controller)));
	menu->AddItem(new MenuItemMenu(GT("�޸�ģ��"), CreatePlayerChangeModelMenu(controller)));

	menu->AddItem(new MenuItemMenu(GT("ͨ��ѡ��"), CreatePlayerWantedMenu(controller)));
	menu->AddItem(new MenuItemMenu(GT("��������ֵ"), MenuItemMiscSetHonor(controller)));
	menu->AddItem(new MenuItemMenu(GT("�����ֽ�"), MenuItemMoneyAndGold(controller)));
	menu->AddItem(new MenuItemMenu(GT("��������"), MenuItemPlayerRank(controller)));


	menu->AddItem(new MenuItemPlayerFix(GT("˲����Ѫ")));
	menu->AddItem(new MenuItemPlayerUnlimAbility(GT("��������֮��")));
	menu->AddItem(new MenuItemPlayerInvincible(GT("����޵�")));
	menu->AddItem(new MenuItemPlayerUnlimStamina(GT("��������")));
	menu->AddItem(new MenuItemMenu(GT("����ѡ��"), CreatePlayerMiscMenu(controller)));

	return menu;
}