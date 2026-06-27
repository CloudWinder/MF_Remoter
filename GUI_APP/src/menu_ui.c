#include "menu_ui.h"
#include "windows.h"
#include "scrollbar.h"
#include "joystick.h"
#include "keyTask.h"
#include "display.h"
#include "menuL1_item.h"
#include "menuL2_item.h"


//�˵���λ��
#define   MEMU_POSX_1    20
#define   MEMU_POSY_1    19

MenuItem_Typedef*  CurMenu = mainMenu;
MenuItem_Typedef*  CurItem;

static int selected;
static int cur_sequence; //ĳһ���˵�ѡ����Ŀ�����
static bool isChangeMenu = true;
const uint8_t* defaultTitle = "ATK MiniFly";

//����
WINDOWS MenuWindow={
.x = 0,
.y = 0,	
.width = 128,
.height = 64,
.itemsperpage = 3,
.topitem = 0,
.title = "ATK MiniFly",
};

//�����������
Scrollbar_Typedef MenuScrollbar={
.x = 118,
.y = 14,
.width = 10,
.height = 50,
.itemsperpage = 3,
.topitem = 0,
.scbbarlen = 0,
};

uint8_t getMenuSelectitem(MenuItem_Typedef menu[])
{
	int i;
	for(i=0; i< menu->menuItemCount; i++)
	{
		if(menu[i].isSelect == true) 
			break;
	}
	return i;
}

void exitMenu(void)
{
	CurMenu = mainMenu;
	MenuWindow.title = defaultTitle;
	setShow_ui(MAIN_UI);
	isChangeMenu = true;
}

void gotoNextMenu(void)	
{
	if(CurItem->childrenMenu != NULL)
	{
		CurMenu = CurItem->childrenMenu;
		MenuWindow.title = CurItem->title +2;
		isChangeMenu = true;
	}
	else if(CurItem->Function!=NULL && CurItem->parentMenu==NULL)
	{
		CurItem->Function();
	}
}

void gotoLastMenu(void)
{
	if(CurItem->parentMenu != NULL)
	{
		//�ָ�Ĭ��ѡ��
		CurItem->isSelect = false;
		CurItem = CurMenu + selected;
		CurItem->isSelect = true;
		
		CurMenu = CurItem->parentMenu;
		if(CurMenu == mainMenu)
			MenuWindow.title = defaultTitle;
		else
			MenuWindow.title = CurItem->title +2;
		isChangeMenu = true;
	}
	else
	{
		exitMenu();
	}
}

/*��ʾ�˵�*/
void DisplayMenuInit(MenuItem_Typedef* menu)
{
	uint8_t topitem;
	uint8_t showItems;
	
	if(isChangeMenu == false) return;
	
	selected = getMenuSelectitem(menu);
	cur_sequence = selected;
	if(selected < menu->cursorPosition)
		menu->cursorPosition = 0;
	topitem = selected - menu->cursorPosition;
	if(menu->menuItemCount <= MenuWindow.itemsperpage)
		topitem = 0;

	MenuWindow.topitem = topitem;
	GUI_WindowsDraw(&MenuWindow);
	
	MenuScrollbar.topitem = topitem;
	MenuScrollbar.totalitems = menu->menuItemCount;
	GUI_ScrollbarDraw(&MenuScrollbar);
	
	showItems = MenuWindow.itemsperpage;
	if(menu->menuItemCount < MenuWindow.itemsperpage)
		showItems = menu->menuItemCount;
	for(int i=0; i<showItems; i++)
	{
		MenuItem_Typedef* Item = &menu[topitem+i];
		GUI_MenuItemDraw(MEMU_POSX_1, MEMU_POSY_1+i*15, Item);
	}
	CurItem = menu + cur_sequence;
	CurItem->isSelect = true;
	
	GUI_Refresh();//ˢ����Ļ
	
	isChangeMenu = false;
}

/*�˵�����*/
void Menu_Run(void)
{
	uint8_t keyState;
	enum dir_e joystick2;
	static int timeout;
	uint8_t showItems;
	
	DisplayMenuInit(CurMenu);
	
	joystick2 = getJoystick2Dir(0);
	if(joystick2 != CENTER)
	{
		timeout = 0;
	}
	switch(joystick2)
	{
		case BACK:	//PITCH���
			//�����������
			GUI_RectangleFill(1, 18, 117, 62, 0);
			CurItem = CurMenu + cur_sequence;
			CurItem->isSelect = false;
		
			//�˵���Ŀ���++
			cur_sequence++;
			if(cur_sequence >= CurMenu->menuItemCount-1)
				cur_sequence = CurMenu->menuItemCount-1;
			CurItem = CurMenu + cur_sequence;
			CurItem->isSelect = true;
			
			//���λ��++
			CurMenu->cursorPosition++;
			if(CurMenu->menuItemCount <= MenuWindow.itemsperpage)
			{
				showItems = CurMenu->menuItemCount;
				if(CurMenu->cursorPosition >= CurMenu->menuItemCount)
					CurMenu->cursorPosition = CurMenu->menuItemCount-1;
			}
			else
			{
				showItems = MenuWindow.itemsperpage;
				if(CurMenu->cursorPosition > MenuWindow.itemsperpage-1)
				{		
					CurMenu->cursorPosition= MenuWindow.itemsperpage-1;
					if(MenuWindow.topitem < CurMenu->menuItemCount-MenuWindow.itemsperpage)
						MenuWindow.topitem++;
				}
			}
			for(int i=0; i<showItems; i++)
			{
				MenuItem_Typedef* Item = CurMenu+MenuWindow.topitem+i;
				GUI_MenuItemDraw(MEMU_POSX_1, MEMU_POSY_1+i*15, Item);
			}
			MenuScrollbar.topitem = cur_sequence;
			GUI_Scrollbar_SetPos(&MenuScrollbar);				
			break;
					
		case FORWARD:	//PITCH��ǰ
			//�����������
			GUI_RectangleFill(1, 18, 117, 62, 0);
			CurItem = CurMenu + cur_sequence;
			CurItem->isSelect = false;
		
			//�˵���Ŀ���--
			cur_sequence--;
			if(cur_sequence <= 0)
				cur_sequence = 0;
			CurItem = CurMenu + cur_sequence;
			CurItem->isSelect = true;
			
			//���λ��--
			CurMenu->cursorPosition--;
			if(CurMenu->menuItemCount <= MenuWindow.itemsperpage)
			{	
				showItems = CurMenu->menuItemCount;
				if(CurMenu->cursorPosition < 0)
					CurMenu->cursorPosition = 0;
			}
			else
			{	
				showItems = MenuWindow.itemsperpage;				
				if(CurMenu->cursorPosition < 0)
				{		
					CurMenu->cursorPosition = 0;
					if(MenuWindow.topitem > 0)
						MenuWindow.topitem--;
				}
			}
			for(int i=0; i<showItems; i++)
			{
				MenuItem_Typedef* Item = CurMenu+MenuWindow.topitem+i;
				GUI_MenuItemDraw(MEMU_POSX_1, MEMU_POSY_1+i*15, Item);
			}
			MenuScrollbar.topitem = cur_sequence;
			GUI_Scrollbar_SetPos(&MenuScrollbar);      					
			break;
			
		case RIGHT:  //ROLL����
			gotoNextMenu();//������һ���˵�
			break;
			
		case LEFT:  //ROLL����
			gotoLastMenu();//������һ���˵�
			break;
		
		default :break;
	}
	
	keyState = getKeyState();
	/*����ҡ�˼�ִ�в˵���Ӧ�Ķ���*/
	if(keyState == KEY_J2_SHORT_PRESS)
	{	
		if(CurItem->Function != NULL)
		{
			CurItem->Function();
		}
	}
	/*��ʱ�˳��˵�*/
	if(timeout++ > 100)
	{
		timeout = 0;
		if(CurItem->parentMenu != NULL)
		{
			//�ָ�Ĭ��ѡ��
			CurItem->isSelect = false;
			CurItem = CurMenu + selected;
			CurItem->isSelect = true;
		}
		exitMenu();
	}
}

