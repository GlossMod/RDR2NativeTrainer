/*
	THIS FILE IS A PART OF RDR 2 SCRIPT HOOK SDK
				http://dev-c.com
			(C) Alexander Blade 2019
*/

#pragma once

#include "script.h"
#include "keyboard.h"
#include "KeyConfig.h"
#include "Function.h"

#include <windows.h>
#include <vector>
#include <string>
#include "xinput.h"

#define KEY_DOWN(VK_NONAME) ((GetAsyncKeyState(VK_NONAME) & 0x8000) ? 1:0) //监听按键是否按下

using namespace std;

class MenuBase;
class MenuController;

struct ColorRgba
{
	int	r, g, b, a;
};

enum eMenuItemClass
{
	Base,
	Title,
	ListTitle,
	Default,
	Switchable,
	Menu,
	Value
};

class MenuItemBase
{	
	float		m_lineWidth;
	float		m_lineHeight;
	float		m_textLeft;
	ColorRgba	m_colorRect;
	ColorRgba	m_colorText;
	ColorRgba	m_colorRectActive;
	ColorRgba	m_colorTextActive;

	MenuBase *	m_menu;
protected:
	MenuItemBase(
		float lineWidth, float lineHeight, float textLeft, 
		ColorRgba colorRect, ColorRgba colorText, 
		ColorRgba colorRectActive = {}, ColorRgba colorTextActive = {})
		: m_lineWidth(lineWidth), m_lineHeight(lineHeight), m_textLeft(textLeft),
			m_colorRect(colorRect), m_colorText(colorText), 
			m_colorRectActive(colorRectActive), m_colorTextActive(colorTextActive)	{}
	
public:
	virtual ~MenuItemBase() {}
	void WaitAndDraw(int ms);
	void SetStatusText(string text, int ms = 2500);
	//void SetStatusText(string str, bool isGxtEntry = false);
	virtual eMenuItemClass GetClass() { return eMenuItemClass::Base; }
	virtual void OnDraw(float lineTop, float lineLeft, bool active);
	void FastPlayerTeleportToMarker();
	virtual	void OnSelect() {}
	virtual	void OnFrame() {}
	virtual void OnAdd(){}
	virtual void OnCut(){}
	virtual	string GetCaption() { return ""; }

	float GetLineWidth()  { return m_lineWidth;  }
	float GetLineHeight() { return m_lineHeight; }

	ColorRgba GetColorRect() { return m_colorRect; }
	ColorRgba GetColorText() { return m_colorText; }

	ColorRgba GetColorRectActive() { return m_colorRectActive; }
	ColorRgba GetColorTextActive() { return m_colorTextActive; }

	void SetMenu(MenuBase *menu) { m_menu = menu; };
	MenuBase *GetMenu() { return m_menu; };
	bool featurePlayerInvincible = false;
};


//标题相关样式
const float
	MenuItemTitle_lineWidth	 = 0.22f,
	MenuItemTitle_lineHeight = 0.06f,
	MenuItemTitle_textLeft	 = 0.01f;

const ColorRgba
	MenuItemTitle_colorRect { 243, 23, 84, 140 },
	MenuItemTitle_colorText { 255, 255, 255, 255 };

class MenuItemTitle : public MenuItemBase	
{
	string		m_caption;
public:
	MenuItemTitle(string caption)
		: MenuItemBase(
				MenuItemTitle_lineWidth, MenuItemTitle_lineHeight, MenuItemTitle_textLeft, 
				MenuItemTitle_colorRect, MenuItemTitle_colorText
		  ),
		  m_caption(caption) {}
	virtual eMenuItemClass GetClass() { return eMenuItemClass::Title; }
	virtual	string GetCaption() { return m_caption; }
};

class MenuItemListTitle : public MenuItemTitle
{
	int		m_currentItemIndex;
	int		m_itemsTotal;
public:
	MenuItemListTitle(string caption)
		: MenuItemTitle(caption), 
			m_currentItemIndex(0), m_itemsTotal(0) {}
	virtual eMenuItemClass GetClass() { return eMenuItemClass::ListTitle; }
	virtual	string GetCaption() { return MenuItemTitle::GetCaption() + "  " + to_string(m_currentItemIndex) + "/" + to_string(m_itemsTotal); }
	void SetCurrentItemInfo(int index, int total) { m_currentItemIndex = index, m_itemsTotal = total; }
};

//列表相关样式
const float
	MenuItemDefault_lineWidth	= 0.22f,
	MenuItemDefault_lineHeight	= 0.04f,
	MenuItemDefault_textLeft	= 0.01f;

const ColorRgba
	MenuItemDefault_colorRect			{ 200, 200, 200, 150 },		//默认背景颜色RGBA
	MenuItemDefault_colorText			{ 0, 0, 0, 255 },			//默认字体颜色RGBA

	MenuItemDefault_colorRectActive		{ 200, 200, 0, 255 },			//选中项背景颜色RGBA
	MenuItemDefault_colorTextActive		{ 0, 0, 0, 255 };		//选中项字体颜色RGBA

class MenuItemDefault : public MenuItemBase
{
	string		m_caption;
public:
	MenuItemDefault(string caption)
		: MenuItemBase(
			MenuItemDefault_lineWidth, MenuItemDefault_lineHeight, MenuItemDefault_textLeft,
			MenuItemDefault_colorRect, MenuItemDefault_colorText, MenuItemDefault_colorRectActive, MenuItemDefault_colorTextActive
		  ),
		  m_caption(caption) {}
	virtual eMenuItemClass GetClass() { return eMenuItemClass::Default; }
	virtual	string GetCaption() { return m_caption; }
};

class MenuItemSwitchable : public MenuItemDefault
{
	bool	m_state;
public:
	MenuItemSwitchable(string caption)
		: MenuItemDefault(caption),
		m_state(false) {
			// addLogs(caption);
		}
	virtual eMenuItemClass GetClass() { return eMenuItemClass::Switchable; }	
	virtual void OnDraw(float lineTop, float lineLeft, bool active);
	virtual void OnSelect() { m_state = !m_state; }
	void SetState(bool state) { m_state = state; }
	bool GetState() { return m_state; }
};

class MenuItemValue : public MenuItemDefault
{
	float	m_index;
	int m_max;
	int m_min;
	bool m_isfloat;
	
public:
	MenuItemValue(string caption, int value, int max = 5, int min = 0, bool isfloat = false)
		: MenuItemDefault(caption),
		m_index(value), m_max(max), m_min(min), m_isfloat(isfloat){}


	virtual eMenuItemClass GetClass() { return eMenuItemClass::Value; }
	virtual void OnDraw(float lineTop, float lineLeft, bool active);
	virtual void OnAdd() {
		if (m_isfloat)
			m_index = m_index + 0.1;
		else 
			m_index = m_index + 1;
		
		if (m_index > m_max)
		{
			m_index = m_max;
		}		
		OnChange(); 
	}
	virtual void OnCut() {
		if (m_isfloat)
			m_index = m_index - 0.1;
		else
			m_index = m_index - 1;

		if (m_index < m_min)
		{
			m_index = m_min;
		}

		OnChange(); 
	}
	virtual void OnChange() { }
	void SetValue(int index) { m_index = index; }
	float GetValue() { return m_index; }
};

class MenuItemMenu : public MenuItemDefault
{
	MenuBase *	m_menu;
public:
	MenuItemMenu(string caption, MenuBase *menu)
		: MenuItemDefault(caption),
		m_menu(menu) {}
	virtual eMenuItemClass GetClass() { return eMenuItemClass::Menu; }
	virtual void OnDraw(float lineTop, float lineLeft, bool active);
	virtual	void OnSelect();	
};

const int
	MenuBase_linesPerScreen = 11;

const float
	MenuBase_menuTop  = 0.05f,
	MenuBase_menuLeft = 0.01f,
	MenuBase_lineOverlap = 1.0f / 40.0f;

class MenuBase
{
	MenuItemTitle *				m_itemTitle;
	vector<MenuItemBase *>		m_items;

	int		m_activeLineIndex;
	int		m_activeScreenIndex;

	MenuController *			m_controller;
public:
	MenuBase(MenuItemTitle *itemTitle)
		: m_itemTitle(itemTitle), 
		  m_activeLineIndex(0), m_activeScreenIndex(0) {}
	~MenuBase()
	{
		for  (auto item : m_items)
			delete item;
	}
	void AddItem(MenuItemBase *item) { item->SetMenu(this); m_items.push_back(item); }
	int GetActiveItemIndex() { return m_activeScreenIndex * MenuBase_linesPerScreen + m_activeLineIndex; }
	void OnDraw();
	int OnInput();
	void OnFrame()
	{
		for (size_t i = 0; i < m_items.size(); i++)
			m_items[i]->OnFrame();
	}
	void SetController(MenuController *controller) { m_controller = controller; }
	MenuController *GetController() { return m_controller; }
};

struct MenuInputButtonState
{
	bool a, b, up, down, l, r, s;
};

class MenuInput
{
public:
	static bool MenuSwitchPressed()
	{
		//return IsKeyJustUp(KeyConfig::KEY_TOGGLE_MAIN_MENU);
		return IsKeyJustUp(KeyConfig::KEY_TOGGLE_MAIN_MENU) || IsControllerButtonDown(KeyConfig::KEY_TOGGLE_MAIN_MENU);	//启动按键
	}
	static MenuInputButtonState GetButtonState()
	{
		return {
			//IsKeyJustUp(KeyConfig::KEY_MENU_SELECT),		//IsKeyDown(VK_NUMPAD5) || IsKeyDown(VK_RETURN) || KEY_DOWN(VK_GAMEPAD_A),			
			//IsKeyJustUp(KeyConfig::KEY_MENU_BACK),			//IsKeyDown(VK_NUMPAD0) || IsKeyDown(VK_BACK) || KEY_DOWN(VK_GAMEPAD_B),
			//IsKeyJustUp(KeyConfig::KEY_MENU_UP),			//IsKeyDown(VK_NUMPAD8) || IsKeyDown(VK_UP) || KEY_DOWN(VK_GAMEPAD_LEFT_THUMBSTICK_UP),
			//IsKeyJustUp(KeyConfig::KEY_MENU_DOWN),			//IsKeyDown(VK_NUMPAD2) || IsKeyDown(VK_DOWN) || KEY_DOWN(VK_GAMEPAD_LEFT_THUMBSTICK_DOWN),
			//IsKeyJustUp(KeyConfig::KEY_MENU_LEFT),			//IsKeyDown(VK_NUMPAD4) || IsKeyDown(VK_LEFT) || KEY_DOWN(VK_GAMEPAD_DPAD_LEFT),
			//IsKeyJustUp(KeyConfig::KEY_MENU_RIGHT),			//IsKeyDown(VK_NUMPAD6) || IsKeyDown(VK_RIGHT) || KEY_DOWN(VK_GAMEPAD_DPAD_RIGHT),
			IsKeyJustUp(KeyConfig::KEY_MENU_SELECT)		||  IsControllerButtonDown(KeyConfig::KEY_MENU_SELECT),
			IsKeyJustUp(KeyConfig::KEY_MENU_BACK)		||  IsControllerButtonDown(KeyConfig::KEY_MENU_BACK),
			IsKeyDown(KeyConfig::KEY_MENU_UP)			||	IsControllerButtonDown(KeyConfig::KEY_MENU_UP),
			IsKeyDown(KeyConfig::KEY_MENU_DOWN)			||	IsControllerButtonDown(KeyConfig::KEY_MENU_DOWN),
			IsKeyDown(KeyConfig::KEY_MENU_LEFT)			||	IsControllerButtonDown(KeyConfig::KEY_MENU_LEFT),
			IsKeyDown(KeyConfig::KEY_MENU_RIGHT)		||	IsControllerButtonDown(KeyConfig::KEY_MENU_RIGHT),
			MenuSwitchPressed()
		};
	}
	static void MenuInputBeep()
	{
		AUDIO::STOP_SOUND_FRONTEND("NAV_RIGHT", "HUD_SHOP_SOUNDSET");
		AUDIO::PLAY_SOUND_FRONTEND("NAV_RIGHT", "HUD_SHOP_SOUNDSET", 1, 0);
	}
};


class MenuController
{
	vector<MenuBase *>		m_menuList;
	vector<MenuBase *>		m_menuStack;	

	DWORD	m_inputTurnOnTime;

	string	m_statusText;
	DWORD	m_statusTextMaxTicks;

	void InputWait(int ms)		{	m_inputTurnOnTime = GetTickCount64() + ms; }
	bool InputIsOnWait()		{	return m_inputTurnOnTime > GetTickCount64(); }
	MenuBase *GetActiveMenu()	{	return m_menuStack.size() ? m_menuStack[m_menuStack.size() - 1] : NULL; }
	void DrawStatusText();
	void OnDraw()
	{	
		if (auto menu = GetActiveMenu()) 
			menu->OnDraw(); 
		DrawStatusText();
	}
	void OnInput()
	{
		if (InputIsOnWait())
			return;
		if (auto menu = GetActiveMenu())
			if (int waitTime = menu->OnInput())
				InputWait(waitTime);
	}
	void OnFrame()
	{
		for (auto i = 0; i < m_menuList.size(); i++)
			m_menuList[i]->OnFrame();
	}
public:
	MenuController()
		: m_inputTurnOnTime(0), m_statusTextMaxTicks(0) {}
	~MenuController()
	{
		for (auto menu : m_menuList)
			delete menu;
	}
	bool HasActiveMenu()			{	return m_menuStack.size() > 0; }
	void PushMenu(MenuBase *menu)	{	if (IsMenuRegistered(menu)) m_menuStack.push_back(menu); }
	void PopMenu()					{   if (m_menuStack.size()) m_menuStack.pop_back(); }
	void MenuHide()					{	m_menuStack.clear() ; }
	void SetStatusText(string text, int ms) { m_statusText = text, m_statusTextMaxTicks = GetTickCount() + ms; }
	bool IsMenuRegistered(MenuBase *menu)
	{
		for (size_t i = 0; i < m_menuList.size(); i++)
			if (m_menuList[i] == menu)
				return true;
		return false;
	}
	void RegisterMenu(MenuBase *menu)
	{
		if (!IsMenuRegistered(menu))
		{
			menu->SetController(this);
			m_menuList.push_back(menu);
		}
	}


	void Update()
	{
		OnDraw();
		OnInput();
		OnFrame();
	}
};

