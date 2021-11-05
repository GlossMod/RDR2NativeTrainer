#include "vehicle.h"

#include "scriptinfo.h"

#include <unordered_map>
#include <ctime>
#include <tchar.h>
#include <windows.h>


bool Aslongasthetrailer = false;


class MenuItemSpawnVehicle : public MenuItemDefault
{
	string		m_model;
	Vector3		m_pos;
	float		m_heading;
	bool		m_resetHeading;

	MenuItemSwitchable*	m_noPeds;
	MenuItemSwitchable* m_menuItemWrapIn;
	MenuItemSwitchable* m_menuItemSetProperly;

	virtual void OnSelect()
	{
		DWORD model = GAMEPLAY::GET_HASH_KEY(const_cast<char*>(m_model.c_str()));
		if (STREAMING::IS_MODEL_IN_CDIMAGE(model) && STREAMING::IS_MODEL_VALID(model))
		{
			STREAMING::REQUEST_MODEL(model, FALSE);
			while (!STREAMING::HAS_MODEL_LOADED(model)) {
				WaitAndDraw(0); // WAIT(0);
			}
			Ped playerPed = PLAYER::PLAYER_PED_ID();
			float playerHeading = ENTITY::GET_ENTITY_HEADING(playerPed) + 5.0f;
			float heading = playerHeading + m_heading;
			Vector3 coords = ENTITY::GET_OFFSET_FROM_ENTITY_IN_WORLD_COORDS(playerPed, m_pos.x, m_pos.y, m_pos.z);
			Vehicle veh = VEHICLE::CREATE_VEHICLE(model, coords.x, coords.y, coords.z, heading, 0, 0, m_noPeds->GetState(), 0);
			DECORATOR::DECOR_SET_BOOL(veh, "wagon_block_honor", TRUE);
			bool wrapIn = m_menuItemWrapIn && m_menuItemWrapIn->GetState();
			bool setProperly = m_menuItemSetProperly && m_menuItemSetProperly->GetState();
			if (setProperly)
			{
				VEHICLE::SET_VEHICLE_ON_GROUND_PROPERLY(veh, 0);
				WaitAndDraw(100);
			}
			if (m_resetHeading || wrapIn)
				ENTITY::SET_ENTITY_HEADING(veh, wrapIn ? playerHeading : heading);
			if (wrapIn)
				PED::SET_PED_INTO_VEHICLE(playerPed, veh, -1);
			ENTITY::SET_VEHICLE_AS_NO_LONGER_NEEDED(&veh);
			STREAMING::SET_MODEL_AS_NO_LONGER_NEEDED(model);
		}
	}
public:
	MenuItemSpawnVehicle(string name, string model,
		Vector3 pos, float heading,
		MenuItemSwitchable* menuItemWrapIn,
		MenuItemSwitchable* menuItemSetProperly,
		bool resetHeading,
		MenuItemSwitchable* noPeds)
		: MenuItemDefault(name),
		m_model(model),
		m_pos(pos), m_heading(heading),
		m_menuItemWrapIn(menuItemWrapIn),
		m_menuItemSetProperly(menuItemSetProperly),
		m_resetHeading(resetHeading),
		m_noPeds(noPeds) { }
};

//MenuBase* CreateWagonSpawnerMenu(MenuController* controller,
//	MenuItemSwitchable* menuItemWrapIn, MenuItemSwitchable* menuItemSetProperly, bool noPeds)
//{
//	auto menu = new MenuBase(new MenuItemListTitle(GT("拖车选项")));
//	controller->RegisterMenu(menu);
//
//	for each (auto & model in vehicle)
//		if (GetVehicleTypeUsingModel(model.code) == vtWagon)
//			menu->AddItem(new MenuItemSpawnVehicle(model.name, model.code, { 1.0, 5.0, 0.0 }, 90.0, menuItemWrapIn, menuItemSetProperly, true, noPeds));
//
//	return menu;
//}

MenuBase* CreateVehicleSpawnerMenu(MenuController* controller)
{
	auto menu = new MenuBase(new MenuItemTitle(GT("载具选项")));
	controller->RegisterMenu(menu);

	auto menuItemWrapIn = new MenuItemSwitchable(GT("生成后进入"));
	menu->AddItem(menuItemWrapIn);

	auto menuItemSetProperly = new MenuItemSwitchable(GT("摆正"));
	menu->AddItem(menuItemSetProperly);

	auto Aslongasthetrailer = new MenuItemSwitchable(GT("只要拖车"));
	menu->AddItem(Aslongasthetrailer);

	unordered_map<string, vector<pair<string, string>>> breeds;
	for each (auto & modelInfo in vehicle)
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
			breedMenu->AddItem(new MenuItemSpawnVehicle(kindAndModel.first, kindAndModel.second, { 1.0, 5.0, 0.0 }, 90.0, menuItemWrapIn, menuItemSetProperly, true, Aslongasthetrailer));

	}

	return menu;
}