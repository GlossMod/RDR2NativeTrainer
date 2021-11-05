
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

MenuBase* CreateHorseSpawnerMenu(MenuController* controller)
{
	auto menu = new MenuBase(new MenuItemListTitle(GT("刷出马匹")));
	controller->RegisterMenu(menu);

	menu->AddItem(new MenuItemSpawnHorseRandom(GT("随机马匹")));


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
			breedMenu->AddItem(new MenuItemSpawnPed(kindAndModel.first, kindAndModel.second));
	}



	return menu;
}


MenuBase* CreateAnimalSpawnerMenu(MenuController* controller)
{
	auto menu = new MenuBase(new MenuItemTitle(GT("刷出动物")));
	controller->RegisterMenu(menu);

	menu->AddItem(new MenuItemSpawnAnimalRandom(GT("随机")));
	menu->AddItem(new MenuItemMenu(GT("马匹"), CreateHorseSpawnerMenu(controller)));

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
			breedMenu->AddItem(new MenuItemSpawnPed(kindAndModel.first, kindAndModel.second));
	}

	return menu;
}

MenuBase* CreateHumanSpawnerMenu(MenuController* controller)
{
	auto menu = new MenuBase(new MenuItemTitle(GT("人物")));
	controller->RegisterMenu(menu);

	menu->AddItem(new MenuItemSpawnPedRandom(GT("随机人物")));

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
			breedMenu->AddItem(new MenuItemSpawnPed(kindAndModel.first, kindAndModel.second));
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
			SetStatusText(GT("成功"));
		}
		else
		{
			SetStatusText(GT("未找到此模型"));
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
	auto menu = new MenuBase(new MenuItemTitle(GT("模型选项")));
	controller->RegisterMenu(menu);

	menu->AddItem(new MenuItemMenu(GT("动物"), CreateAnimalSpawnerMenu(controller)));
	menu->AddItem(new MenuItemMenu(GT("人物"), CreateHumanSpawnerMenu(controller)));
	menu->AddItem(new MenuItemInputModelCode(GT("输入模型代码")));

	return menu;
}