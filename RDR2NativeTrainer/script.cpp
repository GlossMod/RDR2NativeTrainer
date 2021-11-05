#include "script.h"
//自定义方法引入
#include "Function.h"
#include "scriptmenu.h"
#include "keyboard.h"
#include "scriptinfo.h"
#include "CreateAllModel.h"
//自定义功能引入
#include "bodyguard.h"	//保镖
#include "Weapon.h"		//武器
#include "Weather.h"	//天气
#include "vehicle.h"	//载具
#include "player.h"		//玩家
#include "Teleport.h"	//传送
#include "word.h"		//世界

//系统项引入
#include <unordered_map>
#include <ctime>
#include <tchar.h>
#include "Misc.h"
#include "KeyConfig.h"




//更新
class CreateMiscOpenModWeb : public MenuItemDefault
{
	virtual void OnSelect()
	{
		ShellExecute(NULL, _T("open"), _T("explorer.exe"), _T("https://mod.3dmgame.com/mod/147160"), NULL, SW_SHOW);
	}
public:
	CreateMiscOpenModWeb(string caption)
		: MenuItemDefault(caption) {}
};


//刷新自定义数据
class MenuItemRefreshList : public MenuItemDefault
{
	virtual void OnSelect()
	{
		//UpDataJsonData();
		ScriptMain();

	}
public:
	MenuItemRefreshList(string caption)
		: MenuItemDefault(caption) {}
};



MenuBase* CreateMainMenu(MenuController* controller)
{

	string CU = CheckUpdates();


	auto menu = new MenuBase(new MenuItemTitle(GT("RDR2内置修改器 By:小莫")));
	controller->RegisterMenu(menu);

	menu->AddItem(new MenuItemMenu(GT("玩家选项"), CreatePlayerMenu(controller)));
	menu->AddItem(new MenuItemMenu(GT("传送选项"), CreatePlayerTeleportMenu(controller)));
	menu->AddItem(new MenuItemMenu(GT("生成模型"), CreateAllModel(controller)));
	menu->AddItem(new MenuItemMenu(GT("保镖选项"), CreateBodyguardSpawnerMenu(controller)));
	menu->AddItem(new MenuItemMenu(GT("载具选项"), CreateVehicleSpawnerMenu(controller)));
	menu->AddItem(new MenuItemMenu(GT("武器选项"), CreateWeaponMenu(controller)));
	menu->AddItem(new MenuItemMenu(GT("世界选项"), CreateWordMenu(controller)));
	menu->AddItem(new MenuItemMenu(GT("其他选项"), CreateMiscMenu(controller)));

	menu->AddItem(new MenuItemRefreshList(GT("重载自定义数据")));
	menu->AddItem(new CreateMiscOpenModWeb(CU));

	return menu;
}


void main()
{
	Clearlog();			// 清空日志
	UpDataJsonData();	// 初始化物品列表
	init_xinput();		// 初始化手柄配置
	read_config_file();	// 初始化按键配置
	auto menuController = new MenuController();
	auto mainMenu = CreateMainMenu(menuController);

	while (true)
	{
		if (!menuController->HasActiveMenu() && MenuInput::MenuSwitchPressed())
		{
			MenuInput::MenuInputBeep();
			menuController->PushMenu(mainMenu);
		}
		menuController->Update();
		WAIT(0);
	}
}

void ScriptMain()
{
	srand(GetTickCount());
	main();
}