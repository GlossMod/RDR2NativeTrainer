#include "Teleport.h"

#include "scriptinfo.h"

#include <unordered_map>
#include <ctime>
#include <tchar.h>
#include <windows.h>


bool TPing = false;	// 是否在传送中
Vector3 playerPos = { 0,0,0 };	// 玩家位置
Vector3 newPos = { 0,0,0 };		// 传送后的位置

//传送
class MenuItemPlayerTeleport : public MenuItemDefault
{
	Vector3 m_pos;
	virtual void OnSelect()
	{
		playerPos = ENTITY::GET_ENTITY_COORDS(PLAYER::PLAYER_PED_ID(), false, false);
		newPos = m_pos;
		TPing = true;

		Entity e = PLAYER::PLAYER_PED_ID();
		if (PED::IS_PED_ON_MOUNT(e))
			e = PED::GET_MOUNT(e);
		else
			if (PED::IS_PED_IN_ANY_VEHICLE(e, FALSE))
				e = PED::GET_VEHICLE_PED_IS_USING(e);
		ENTITY::SET_ENTITY_COORDS(e, m_pos.x, m_pos.y, m_pos.z, 0, 0, 1, FALSE);

		addLogs("传送到坐标：" + to_string(m_pos.x) + "," + to_string(m_pos.y) + "," + to_string(m_pos.z));

	}
public:
	MenuItemPlayerTeleport(string caption, Vector3 pos)
		: MenuItemDefault(caption),
		m_pos(pos) {}
};


//传送到标记点
class MenuItemPlayerTeleportToMarker : public MenuItemDefault
{
	virtual void OnSelect()
	{
		if (!MAP::IS_WAYPOINT_ACTIVE())
		{
			SetStatusText(GT("地图上没有目标点"));
			return;
		}

		Vector3 coords = MAP::_GET_WAYPOINT_COORDS();

		Entity e = PLAYER::PLAYER_PED_ID();
		if (PED::IS_PED_ON_MOUNT(e))
			e = PED::GET_MOUNT(e);
		else
			if (PED::IS_PED_IN_ANY_VEHICLE(e, FALSE))
				e = PED::GET_VEHICLE_PED_IS_USING(e);

		if (!GAMEPLAY::GET_GROUND_Z_FOR_3D_COORD(coords.x, coords.y, 100.0, &coords.z, FALSE))
		{
			static const float groundCheckHeight[] = {
				100.0, 150.0, 50.0, 0.0, 200.0, 250.0, 300.0, 350.0, 400.0,
				450.0, 500.0, 550.0, 600.0, 650.0, 700.0, 750.0, 800.0
			};
			for each (float height in groundCheckHeight)
			{
				ENTITY::SET_ENTITY_COORDS_NO_OFFSET(e, coords.x, coords.y, height, 0, 0, 1);
				WaitAndDraw(100);
				if (GAMEPLAY::GET_GROUND_Z_FOR_3D_COORD(coords.x, coords.y, height, &coords.z, FALSE))
				{
					coords.z += 3.0;
					break;
				}
			}
		}

		// GTA5式传送
		playerPos = ENTITY::GET_ENTITY_COORDS(PLAYER::PLAYER_PED_ID(), false, false);
		newPos = coords;
		TPing = true;

		ENTITY::SET_ENTITY_COORDS(e, coords.x, coords.y, coords.z, 0, 0, 1, FALSE);

		addLogs("传送到坐标：" + to_string(coords.x) + "," + to_string(coords.y) + "," + to_string(coords.z));

	}
public:
	MenuItemPlayerTeleportToMarker(string caption)
		: MenuItemDefault(caption) {}
};


//快速传送
class MenuFastItemPlayerTeleportToMarker : public MenuItemSwitchable
{
	virtual void OnSelect()
	{
		bool newState = !GetState();
		if (newState) {
			SetStatusText(GT("快速传送已开启"));
		}
		else
		{
			SetStatusText(GT("快速传送已关闭"));
		}
		SetState(newState);
	}
	virtual void OnFrame()
	{
		if (!GetState()) {
			return;
		}
		if (IsKeyJustUp(KeyConfig::KEY_HOT_3) || IsControllerButtonJustUp(KeyConfig::KEY_HOT_3))
		{
			if (!MAP::IS_WAYPOINT_ACTIVE())
			{
				SetStatusText(GT("地图上没有目标点"));
				return;
			}
			Vector3 coords = MAP::_GET_WAYPOINT_COORDS();
			Entity e = PLAYER::PLAYER_PED_ID();
			if (PED::IS_PED_ON_MOUNT(e))
				e = PED::GET_MOUNT(e);
			else
				if (PED::IS_PED_IN_ANY_VEHICLE(e, FALSE))
					e = PED::GET_VEHICLE_PED_IS_USING(e);
			if (!GAMEPLAY::GET_GROUND_Z_FOR_3D_COORD(coords.x, coords.y, 100.0, &coords.z, FALSE))
			{
				static const float groundCheckHeight[] = {
					100.0, 150.0, 50.0, 0.0, 200.0, 250.0, 300.0, 350.0, 400.0,
					450.0, 500.0, 550.0, 600.0, 650.0, 700.0, 750.0, 800.0
				};
				for each (float height in groundCheckHeight)
				{
					ENTITY::SET_ENTITY_COORDS_NO_OFFSET(e, coords.x, coords.y, height, 0, 0, 1);
					WaitAndDraw(100);
					if (GAMEPLAY::GET_GROUND_Z_FOR_3D_COORD(coords.x, coords.y, height, &coords.z, FALSE))
					{
						coords.z += 3.0;
						break;
					}
				}
			}

			// GTA5式传送
			playerPos = ENTITY::GET_ENTITY_COORDS(PLAYER::PLAYER_PED_ID(), false, false);
			newPos = coords;
			TPing = true;

			ENTITY::SET_ENTITY_COORDS(e, coords.x, coords.y, coords.z, 0, 0, 1, FALSE);

		}
	}
public:
	MenuFastItemPlayerTeleportToMarker(string caption)
		: MenuItemSwitchable(caption) {
		SetState(true);
	}
};

class MenuFastItemGTA5TPModer : public MenuItemSwitchable
{
	float initialZForGroundTest = 0;
	Vector3 location = { 0,0,0 };
	float groundZ = 0;
	float time = 300;
	float Xstep = (newPos.x - playerPos.x) / time;	// x轴每秒移动距离
	float Ystep = (newPos.y - playerPos.y) / time;	// y轴每秒移动距离
	float maxZ = 1000;	//最高视角是1千米

	virtual void OnFrame()
	{
		if (GetState())
		{
			if (TPing)
			{
				GAMEPLAY::SET_TIME_SCALE(0.2);	// 游戏减速至0.2倍

				// 创建上帝视角摄像机
				location.x = playerPos.x;
				location.y = playerPos.y;
				location.z = playerPos.z + 100;
				camShit::attachToLocation(location.x, location.y, location.z);	// 初始化摄像机
				ENTITY::FREEZE_ENTITY_POSITION(PLAYER::PLAYER_PED_ID(), true);	// 锁定

				Vector3 velocityFinal = playerPos;

				// 先上升视角
				velocityFinal.z += 50;
				if (velocityFinal.z >= maxZ)
				{
					// 视角上升到最高后 停止上升 
					velocityFinal.z = maxZ;

					// 开始平移视角
					velocityFinal.x += Xstep;
					velocityFinal.y += Ystep;

				}

				if (velocityFinal.x == newPos.x && velocityFinal.y == newPos.y)
				{
					// 如果已经移动到新的坐标位置 
					// 开始下降
					velocityFinal.z -= 50;
					if (velocityFinal.z <= newPos.z + 50)
					{
						// 如果已经到底
						// 结束传送
						TPing = false;
						// 回复正常操作
						camShit::resetCam();
						ENTITY::FREEZE_ENTITY_POSITION(PLAYER::PLAYER_PED_ID(), false);
						// 恢复正常游戏速度
						GAMEPLAY::SET_TIME_SCALE(1);
					}

				}

				camShit::velocityForFocus.x = velocityFinal.x;
				camShit::velocityForFocus.y = velocityFinal.y;
				camShit::velocityForFocus.z = velocityFinal.z;
				camShit::location.x = location.x;
				camShit::location.y = location.y;
				camShit::location.z = location.z;
				camShit::rotation.x = clamp360(-90.0f);//pitch
				camShit::rotation.y = 0;//roll

				GAMEPLAY::GET_GROUND_Z_FOR_3D_COORD(location.x, location.y, initialZForGroundTest, &groundZ, true);//true 使镜头对准地面

			}
		}
	}

public:
	MenuFastItemGTA5TPModer(string caption)
		: MenuItemSwitchable(caption) {}
};

//显示当前坐标
class MenuFastItemShowCurrentCoordinates : public MenuItemDefault
{
	virtual void OnSelect()
	{
		Vector3 coords = ENTITY::GET_OFFSET_FROM_ENTITY_IN_WORLD_COORDS(PLAYER::PLAYER_PED_ID(), 0.0, 0.0, 0.0);

		string x = to_string(coords.x);
		string y = to_string(coords.y);
		string z = to_string(coords.z);

		SetStatusText(GT("当前坐标为:x:" + x + ",y:" + y + ",z:" + z + "."));
		addLogs("显示坐标：" + to_string(coords.x) + "," + to_string(coords.y) + "," + to_string(coords.z));
	}
public:
	MenuFastItemShowCurrentCoordinates(string caption)
		: MenuItemDefault(caption) {}
};

//传送列表
MenuBase* CreatePlayerTeleportMenu(MenuController* controller)
{
	MenuBase* menu = new MenuBase(new MenuItemListTitle(GT("传送选项")));
	controller->RegisterMenu(menu);

	menu->AddItem(new MenuItemPlayerTeleportToMarker(GT("传送到标记点[快捷键F9]")));
	menu->AddItem(new MenuFastItemPlayerTeleportToMarker(GT("快速传送")));
	//menu->AddItem(new MenuFastItemGTA5TPModer(GT("GTA5式传送")));
	menu->AddItem(new MenuFastItemShowCurrentCoordinates(GT("显示当前坐标")));


	unordered_map<string, vector<pair<string, Vector3>>> breeds;
	for each (auto & modelInfo in teleport)
	{
		size_t pos = modelInfo.name.find_first_of(' ');
		string breed = modelInfo.name.substr(0, pos);
		string kind = modelInfo.name.substr(pos + 1, modelInfo.name.size() - pos - 1);
		breeds[breed].push_back({ kind, modelInfo.pos });
	}
	for each (auto & breed in breeds)
	{
		auto breedMenu = new MenuBase(new MenuItemListTitle(breed.first));
		controller->RegisterMenu(breedMenu);
		menu->AddItem(new MenuItemMenu(breed.first, breedMenu));
		for each (auto & kindAndModel in breed.second)
			breedMenu->AddItem(new MenuItemPlayerTeleport(kindAndModel.first, kindAndModel.second));
	}

	return menu;
}