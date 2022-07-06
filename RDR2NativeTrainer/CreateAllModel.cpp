#pragma execution_character_set("utf-8")
#include "CreateAllModel.h"

#include "scriptinfo.h"

#include <unordered_map>
#include <ctime>
#include <tchar.h>
#include <windows.h>


class MenuItemSpawnPed : public MenuItemDefault
{
	string		m_model;

	virtual string GetModel() { return m_model; }

	virtual void OnSelect()
	{
		DWORD model = GAMEPLAY::GET_HASH_KEY(const_cast<char*>(GetModel().c_str()));
		if (STREAMING::IS_MODEL_IN_CDIMAGE(model) && STREAMING::IS_MODEL_VALID(model))
		{
			STREAMING::REQUEST_MODEL(model, FALSE);
			while (!STREAMING::HAS_MODEL_LOADED(model))
				WaitAndDraw(100); // WAIT(0);
			Vector3 coords = ENTITY::GET_OFFSET_FROM_ENTITY_IN_WORLD_COORDS(PLAYER::PLAYER_PED_ID(), 0.0, 3.0, -0.3);
			Ped ped = PED::CREATE_PED(model, coords.x, coords.y, coords.z, static_cast<float>(rand() % 360), 0, 0, 0, 0);
			PED::SET_PED_VISIBLE(ped, TRUE);
			ENTITY::SET_PED_AS_NO_LONGER_NEEDED(&ped);
			STREAMING::SET_MODEL_AS_NO_LONGER_NEEDED(model);

			addLogs("����ģ�ͳɹ���" + GetModel());
		}
	}
public:
	MenuItemSpawnPed(string caption, string model)
		: MenuItemDefault(caption),
		m_model(model) { }
};



class MenuItemSpawnAnimalRandom : public MenuItemSpawnPed
{
	virtual string GetModel()
	{
		while (true)
		{
			int  index = rand() % animal.size();


			dataInfo animalData = animal[index];

			SetStatusText(animalData.name);
			return animalData.code;
		}
	}
public:
	MenuItemSpawnAnimalRandom(string caption)
		: MenuItemSpawnPed(caption, "") { }
};

class MenuItemSpawnPedRandom : public MenuItemSpawnPed
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
	MenuItemSpawnPedRandom(string caption)
		: MenuItemSpawnPed(caption, "") { }
};

class MenuItemSpawnHorseRandom : public MenuItemSpawnPed
{
	virtual string GetModel()
	{
		while (true)
		{
			int  index = rand() % horse.size();

			dataInfo horseData = horse[index];
						
			SetStatusText(horseData.name);
			return horseData.code;
		}
	}
public:
	MenuItemSpawnHorseRandom(string caption)
		: MenuItemSpawnPed(caption, "") { }
};

class MenuItemSpawnMedicine : public MenuItemDefault
{
	string		m_model;

	virtual string GetModel() { return m_model; }

	virtual void OnSelect()
	{
		
		Hash MedicineHash = GAMEPLAY::GET_HASH_KEY(const_cast<char*>(GetModel().c_str()));

		Vector3 coords = ENTITY::GET_OFFSET_FROM_ENTITY_IN_WORLD_COORDS(PLAYER::PLAYER_PED_ID(), 0.0, 3.0, -0.3);	//��ȡ����������

		AI::_0x73F0D0327BFA0812(MedicineHash);

		while (!AI::_0x5E5D96BE25E9DF68(MedicineHash)) {	// �����Ǽ��ز�ҩģ��
			WaitAndDraw(0); // �ȴ�(0);
		}
		Any a;
		auto medicine = AI::_0x5B4BBE80AD5972DC(MedicineHash, coords.x, coords.y, coords.z, 0, 0, &a, -1);


		SetStatusText(("�������"));
	}
public:
	MenuItemSpawnMedicine(string caption, string model)
		: MenuItemDefault(caption),
		m_model(model) { }
};


MenuBase* CreateHorseSpawnerMenu(MenuController* controller)
{
	auto menu = new MenuBase(new MenuItemListTitle(("ˢ����ƥ")));
	controller->RegisterMenu(menu);

	menu->AddItem(new MenuItemSpawnHorseRandom(("�����ƥ")));


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
			breedMenu->AddItem(new MenuItemSpawnPed(kindAndModel.first, kindAndModel.second));
	}



	return menu;
}


MenuBase* CreateAnimalSpawnerMenu(MenuController* controller)
{
	auto menu = new MenuBase(new MenuItemListTitle(("ˢ������")));
	controller->RegisterMenu(menu);

	menu->AddItem(new MenuItemSpawnAnimalRandom(("���")));
	menu->AddItem(new MenuItemMenu(("��ƥ"), CreateHorseSpawnerMenu(controller)));

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
			breedMenu->AddItem(new MenuItemSpawnPed(kindAndModel.first, kindAndModel.second));
	}

	return menu;
}

MenuBase* CreateHumanSpawnerMenu(MenuController* controller)
{
	auto menu = new MenuBase(new MenuItemListTitle(("��������")));
	controller->RegisterMenu(menu);

	menu->AddItem(new MenuItemSpawnPedRandom(("�������")));

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
			breedMenu->AddItem(new MenuItemSpawnPed(kindAndModel.first, kindAndModel.second));
	}

	return menu;
}

// ������ҩ
MenuBase* CreateMedicineSpawnerMenu(MenuController* controller)
{
	auto menu = new MenuBase(new MenuItemListTitle(("������ҩ")));
	controller->RegisterMenu(menu);

	//menu->AddItem(new MenuItemSpawnMedicineRandom(("�����ҩ")));

	unordered_map<string, vector<pair<string, string>>> breeds;

	for (auto & modelInfo : medicine)
	{

		menu->AddItem(new MenuItemSpawnMedicine(modelInfo.name, modelInfo.code));
	}

	return menu;
}

// ��PED
MenuBase* CreateNewSpawnerMenu(MenuController* controller)
{
	auto menu = new MenuBase(new MenuItemListTitle(("��PED")));
	controller->RegisterMenu(menu);


	for (auto & modelInfo : newPed)
	{
		menu->AddItem(new MenuItemSpawnPed(modelInfo.name, modelInfo.code));
	}

	return menu;
}



//����ģ�ʹ���
const char* ModerCode = "A_C_HORSE_ARABIAN_ROSEGREYBAY";
class MenuItemInputModelCode : public MenuItemSpawnPed
{
	virtual string GetModel()
	{

		GAMEPLAY::DISPLAY_ONSCREEN_KEYBOARD(1, "", "", ModerCode, "", "", "", 64);
		GAMEPLAY::CREATE_STRING(2, "", "");

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
			SetStatusText(("�ɹ�"));
		}
		else
		{
			SetStatusText(("δ�ҵ���ģ��"));
		}

		ModerCode = GAMEPLAY::GET_ONSCREEN_KEYBOARD_RESULT();

		return GAMEPLAY::GET_ONSCREEN_KEYBOARD_RESULT();
	}
public:
	MenuItemInputModelCode(string caption)
		:MenuItemSpawnPed(caption, "") { }
};


MenuBase* CreateAllModel(MenuController* controller)
{
	auto menu = new MenuBase(new MenuItemListTitle(("����ѡ��")));
	controller->RegisterMenu(menu);

	menu->AddItem(new MenuItemMenu(("��������"), CreateAnimalSpawnerMenu(controller)));
	menu->AddItem(new MenuItemMenu(("��������"), CreateHumanSpawnerMenu(controller)));
	menu->AddItem(new MenuItemMenu(("������ҩ"), CreateMedicineSpawnerMenu(controller)));
	menu->AddItem(new MenuItemMenu(("��PED"), CreateNewSpawnerMenu(controller)));

	menu->AddItem(new MenuItemInputModelCode(("����ģ�ʹ���")));

	return menu;
}
