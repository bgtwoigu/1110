/** ----------------------------------------------------------------------------
 * MainMenu.h
 * 
 * Copyright(c) 2007 Anylook
 * 
 * 
 * History:
 * 
 * when         who            what and why
 * -----------  -----------    -----------------------------
 * 2008.12.29            4 X 3 large icon style menu * list menu
 * 
 * -----------------------------------------------------------------------------
 */

#ifndef MAINMENU_H
#define MAINMENU_H
/*==============================================================================
                                 
                                ����������Ͷ���
                                 
==============================================================================*/
//Applet�����Ĳ�ͬ״̬
typedef enum _MainMenuServiceType 
{
    STARTMETHOD_PLAYER,       // ISHELL_StartApplet()
    STARTMETHOD_DATA          // ISHELL_StartApplet()
}MainMenuServiceType;

typedef struct _IMainMenu  IMainMenu;
 QINTERFACE(IMainMenu)
{
    INHERIT_IApplet(IMainMenu);

    // �ⲿ����MAINMENU�Ľӿں���
    int (*MainMenuService)(IMainMenu *p, MainMenuServiceType eStype);    
};

#define IMainMenu_AddRef(p)     GET_PVTBL(p,IMainMenu)->AddRef(p)
#define IMainMenu_Release(p)    GET_PVTBL(p,IMainMenu)->Release(p)
#define IMainMenu_HandleEvent(p,ec,wp,dw) GET_PVTBL(p,IMainMenu)->HandleEvent(p, ec, wp, dw)

// �ⲿ���ٽ��뵽��ý���б�
#define IMainMenu_StartPlayer(p)          GET_PVTBL(p,IMainMenu)->MainMenuService(p, STARTMETHOD_PLAYER);

// �ⲿ���ٽ��뵽���ݷ����б�
#define IMainMenu_StartData(p)            GET_PVTBL(p,IMainMenu)->MainMenuService(p, STARTMETHOD_DATA);

#endif
