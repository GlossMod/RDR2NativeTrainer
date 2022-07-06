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

			addLogs("生成模型成功：" + GetModel());
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

		Vector3 coords = ENTITY::GET_OFFSET_FROM_ENTITY_IN_WORLD_COORDS(PLAYER::PLAYER_PED_ID(), 0.0, 3.0, -0.3);	//获取玩家相对坐标

		AI::_0x73F0D0327BFA0812(MedicineHash);

		while (!AI::_0x5E5D96BE25E9DF68(MedicineHash)) {	// 可能是加载草药模型
			WaitAndDraw(0); // 等待(0);
		}
		Any a;
		auto medicine = AI::_0x5B4BBE80AD5972DC(MedicineHash, coords.x, coords.y, coords.z, 0, 0, &a, -1);


		SetStatusText(("创建完成"));
	}
public:
	MenuItemSpawnMedicine(string caption, string model)
		: MenuItemDefault(caption),
		m_model(model) { }
};


MenuBase* CreateHorseSpawnerMenu(MenuController* controller)
{
	auto menu = new MenuBase(new MenuItemListTitle(("刷出马匹")));
	controller->RegisterMenu(menu);

	menu->AddItem(new MenuItemSpawnHorseRandom(("随机马匹")));


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
	auto menu = new MenuBase(new MenuItemListTitle(("刷出动物")));
	controller->RegisterMenu(menu);

	menu->AddItem(new MenuItemSpawnAnimalRandom(("随机")));
	menu->AddItem(new MenuItemMenu(("马匹"), CreateHorseSpawnerMenu(controller)));

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
	auto menu = new MenuBase(new MenuItemListTitle(("创建人物")));
	controller->RegisterMenu(menu);

	menu->AddItem(new MenuItemSpawnPedRandom(("随机人物")));

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

// 创建草药
MenuBase* CreateMedicineSpawnerMenu(MenuController* controller)
{
	auto menu = new MenuBase(new MenuItemListTitle(("创建草药")));
	controller->RegisterMenu(menu);

	//menu->AddItem(new MenuItemSpawnMedicineRandom(("随机草药")));

	unordered_map<string, vector<pair<string, string>>> breeds;

	for (auto & modelInfo : medicine)
	{

		menu->AddItem(new MenuItemSpawnMedicine(modelInfo.name, modelInfo.code));
	}

	return menu;
}

// 新PED
MenuBase* CreateNewSpawnerMenu(MenuController* controller)
{
	auto menu = new MenuBase(new MenuItemListTitle(("新PED")));
	controller->RegisterMenu(menu);


	for (auto & modelInfo : newPed)
	{
		menu->AddItem(new MenuItemSpawnPed(modelInfo.name, modelInfo.code));
	}

	return menu;
}



//输入模型代码
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
	MenuItemInputModelCode(string caption)
		:MenuItemSpawnPed(caption, "") { }
};


MenuBase* CreateAllModel(MenuController* controller)
{
	auto menu = new MenuBase(new MenuItemListTitle(("创建选项")));
	controller->RegisterMenu(menu);

	menu->AddItem(new MenuItemMenu(("创建动物"), CreateAnimalSpawnerMenu(controller)));
	menu->AddItem(new MenuItemMenu(("创建人物"), CreateHumanSpawnerMenu(controller)));
	menu->AddItem(new MenuItemMenu(("创建草药"), CreateMedicineSpawnerMenu(controller)));
	menu->AddItem(new MenuItemMenu(("新PED"), CreateNewSpawnerMenu(controller)));

	menu->AddItem(new MenuItemInputModelCode(("输入模型代码")));

	return menu;
}
