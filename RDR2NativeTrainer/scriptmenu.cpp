/*
	THIS FILE IS A PART OF RDR 2 SCRIPT HOOK SDK
				http://dev-c.com
			(C) Alexander Blade 2019
*/

#include "scriptmenu.h"
#include "GbkToUtf8.h"
#include <iomanip>

void DrawText(float x, float y, char *str)
{
	//UI::DRAW_TEXT(GAMEPLAY::CREATE_STRING(10, "LITERAL_STRING", str), x, y);
	UIDEBUG::_BG_DISPLAY_TEXT(GAMEPLAY::CREATE_STRING(10, "LITERAL_STRING", str), x, y);
}

void DrawRect(float lineLeft, float lineTop, float lineWidth, float lineHeight, int r, int g, int b, int a)
{
	GRAPHICS::DRAW_RECT((lineLeft + (lineWidth * 0.5f)), (lineTop + (lineHeight * 0.5f)), lineWidth, lineHeight, r, g, b, a, 0, 0);
}



void MenuItemBase::WaitAndDraw(int ms)
{
	DWORD time = GetTickCount() + ms;
	bool waited = false;
	while (GetTickCount() < time || !waited)
	{
		WAIT(0);
		waited = true;
		if (auto menu = GetMenu())
			menu->OnDraw();
	}
}


void MenuItemBase::SetStatusText(string text, int ms)
{
	MenuController *controller;
	if (m_menu && (controller = m_menu->GetController()))
		controller->SetStatusText(text, ms);
}
//void MenuItemBase::SetStatusText(std::string str, bool isGxtEntry)
//{
	//UILOG::_0x49E58FE6EF40B987((isGxtEntry ? &str[0u] : "STRING"));
	//UI::_ADD_TEXT_COMPONENT_STRING(&str[0u]);
	//UILOG::_0x49E58FE6EF40B987(FALSE, FALSE); // _DRAW_NOTIFICATION(BOOL blink, BOOL p1)
//}

// 普通列表样式
void MenuItemBase::OnDraw(float lineTop, float lineLeft, bool active)
{
	// text
	ColorRgba color = active ? m_colorTextActive : m_colorText;

	UIDEBUG::_BG_SET_TEXT_SCALE(0.0, m_lineHeight * 7.0f);
	UI::SET_TEXT_COLOR_RGBA(color.r, color.g, color.b, color.a);

	DrawText(lineLeft + m_textLeft, lineTop + m_lineHeight / 4.5f, const_cast<char *>(GetCaption().c_str()));
	// rect
	color = active ? m_colorRectActive : m_colorRect;
	DrawRect(lineLeft, lineTop, m_lineWidth, m_lineHeight, color.r, color.g, color.b, color.a);
}

//有开关选项的列表
void MenuItemSwitchable::OnDraw(float lineTop, float lineLeft, bool active)
{	
	MenuItemDefault::OnDraw(lineTop, lineLeft, active);
	float lineWidth = GetLineWidth();
	float lineHeight = GetLineHeight();
	ColorRgba color = active ? GetColorTextActive() : GetColorText();

	UIDEBUG::_BG_SET_TEXT_SCALE(0.0, lineHeight * 8.0f);
	UI::SET_TEXT_COLOR_RGBA(color.r, color.g, color.b, static_cast<int>(color.a / 1.1f));

	DrawText(lineLeft + lineWidth - lineWidth / 6.35f, lineTop + lineHeight / 4.8f, GetState() ? "[开]" : "[关]");
}

//有数值的列表
void MenuItemValue::OnDraw(float lineTop, float lineLeft, bool active)
{
	MenuItemDefault::OnDraw(lineTop, lineLeft, active);
	float lineWidth = GetLineWidth();
	float lineHeight = GetLineHeight();
	ColorRgba color = active ? GetColorTextActive() : GetColorText();

	UIDEBUG::_BG_SET_TEXT_SCALE(0.0, lineHeight * 8.0f);
	UI::SET_TEXT_COLOR_RGBA(color.r, color.g, color.b, static_cast<int>(color.a / 1.1f));

	float intex = GetValue();
	std::stringstream str;
	str << std::setprecision(2) << intex;
	char const* CVal1 = (char const*)"<< ";
	char const* CVal2 = (char const*)" >>";
	std::string const& SVal = std::string(CVal1) + str.str() + std::string(CVal2);
	char const* CVal3 = SVal.c_str();


	DrawText(lineLeft + lineWidth - lineWidth / 5.0f, lineTop + lineHeight / 4.8f, (char*)CVal3);
}

// 有子菜单的列表
void MenuItemMenu::OnDraw(float lineTop, float lineLeft, bool active)	
{
	MenuItemDefault::OnDraw(lineTop, lineLeft, active);
	float lineWidth = GetLineWidth();
	float lineHeight = GetLineHeight();
	ColorRgba color = active ? GetColorTextActive() : GetColorText();

	UIDEBUG::_BG_SET_TEXT_SCALE(0.0, lineHeight * 8.0f);
	UI::SET_TEXT_COLOR_RGBA(color.r, color.g, color.b, color.a / 2);

	DrawText(lineLeft + lineWidth - lineWidth / 8, lineTop + lineHeight / 3.5f, ">>");
}



void MenuItemMenu::OnSelect()
{
	if (auto parentMenu = GetMenu())
		if (auto controller = parentMenu->GetController())
			controller->PushMenu(m_menu);
}

//列表基础样式
void MenuBase::OnDraw()
{
	float lineTop = MenuBase_menuTop;
	float lineLeft = MenuBase_menuLeft;
	if (m_itemTitle->GetClass() == eMenuItemClass::ListTitle)
		reinterpret_cast<MenuItemListTitle *>(m_itemTitle)->
			SetCurrentItemInfo(GetActiveItemIndex() + 1, static_cast<int>(m_items.size()));
	m_itemTitle->OnDraw(lineTop, lineLeft, false);
	lineTop += m_itemTitle->GetLineHeight();
	for (int i = 0; i < MenuBase_linesPerScreen; i++)
	{
		int itemIndex = m_activeScreenIndex * MenuBase_linesPerScreen + i;
		if (itemIndex == m_items.size())
			break;
		MenuItemBase *item = m_items[itemIndex];
		item->OnDraw(lineTop, lineLeft, m_activeLineIndex == i);
		lineTop += item->GetLineHeight() - item->GetLineHeight() * MenuBase_lineOverlap +0.01f;
	}
}

//列表操作
int MenuBase::OnInput()
{
	const int itemCount = static_cast<int>(m_items.size());
	const int itemsLeft = itemCount % MenuBase_linesPerScreen;
	const int screenCount = itemCount / MenuBase_linesPerScreen + (itemsLeft ? 1 : 0);
	const int lineCountLastScreen = itemsLeft ? itemsLeft : MenuBase_linesPerScreen;

	auto buttons = MenuInput::GetButtonState();	

	int waitTime = 0;

	if (buttons.a || buttons.b || buttons.up || buttons.down || buttons.l || buttons.r || buttons.s)
	{
		MenuInput::MenuInputBeep();
		waitTime = (buttons.b) ? 200 : 150;
	}

	if (buttons.a)
	{
		int activeItemIndex = GetActiveItemIndex();
		m_items[activeItemIndex]->OnSelect();
	} 
	else if (buttons.b)
	{ 
		if (auto controller = GetController())
		{
			controller->PopMenu();
		}
	}
	else if (buttons.up)
	{
		if (m_activeLineIndex-- == 0)
		{
			if (m_activeScreenIndex == 0)
			{
				m_activeScreenIndex = screenCount - 1;
				m_activeLineIndex = lineCountLastScreen - 1;
			} else
			{
				m_activeScreenIndex--;
				m_activeLineIndex = MenuBase_linesPerScreen - 1;
			}
		}
	} 
	else if (buttons.down)
	{
		m_activeLineIndex++;
		if (m_activeLineIndex == ((m_activeScreenIndex == (screenCount - 1)) ? lineCountLastScreen : MenuBase_linesPerScreen))
		{
			if (m_activeScreenIndex == screenCount - 1)
				m_activeScreenIndex = 0;
			else
				m_activeScreenIndex++;
			m_activeLineIndex = 0;
		}
	}
	else if (buttons.l)
	{
		int activeItemIndex = GetActiveItemIndex();
		if (m_items[activeItemIndex]->GetClass() == eMenuItemClass::Value)
		{
			// 如果是数值
			m_items[activeItemIndex]->OnCut();
			return waitTime;
		}

		m_activeScreenIndex--;
		if (m_activeScreenIndex < 0)
		{
			m_activeScreenIndex = screenCount-1;
			m_activeLineIndex = lineCountLastScreen - 1;
		}
		
	}
	else if (buttons.r) {
		int activeItemIndex = GetActiveItemIndex();
		if (m_items[activeItemIndex]->GetClass() == eMenuItemClass::Value)
		{
			// 如果是数值
			m_items[activeItemIndex]->OnAdd();
			return waitTime;
		}

		m_activeScreenIndex++;
		if (m_activeScreenIndex >= screenCount)
		{
			m_activeScreenIndex = 0;

		}
		else if (m_activeScreenIndex >= screenCount - 1)
		{
			m_activeLineIndex = lineCountLastScreen - 1;
		}
	}
	else if (buttons.s)
	{
		if (auto controller = GetController())
		{
			controller->MenuHide();
		}
	}
	return waitTime;
}

void MenuController::DrawStatusText()
{
	if (GetTickCount() < m_statusTextMaxTicks)
	{
		//UI::SET_TEXT_SCALE(0.55, 0.55);
		UIDEBUG::_BG_SET_TEXT_SCALE(0.55, 0.55);
		UI::SET_TEXT_COLOR_RGBA(255, 255, 255, 255);
		//UI::SET_TEXT_CENTRE(1);
		//UI::SET_TEXT_DROPSHADOW(0, 0, 0, 0, 0);
		DrawText(0.5, 0.5, const_cast<char *>(m_statusText.c_str()));
	}
}
