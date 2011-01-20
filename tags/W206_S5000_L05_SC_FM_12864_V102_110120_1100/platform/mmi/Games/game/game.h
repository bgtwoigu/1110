/** ----------------------------------------------------------------------------
 * Game.h
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

#ifndef GAME_H
#define GAME_H
/*==============================================================================
                                 
                                ����������Ͷ���
                                 
==============================================================================*/
//Applet�����Ĳ�ͬ״̬
typedef enum _GameServiceType 
{
    STARTMETHOD_PLAYER,       // ISHELL_StartApplet()
    STARTMETHOD_DATA          // ISHELL_StartApplet()
}GameServiceType;

typedef struct _IGame  IGame;
 QINTERFACE(IGame)
{
    INHERIT_IApplet(IGame);

    // �ⲿ����MAINMENU�Ľӿں���
    int (*GameService)(IGame *p, GameServiceType eStype);    
};

#define IGame_AddRef(p)     GET_PVTBL(p,IGame)->AddRef(p)
#define IGame_Release(p)    GET_PVTBL(p,IGame)->Release(p)
#define IGame_HandleEvent(p,ec,wp,dw) GET_PVTBL(p,IGame)->HandleEvent(p, ec, wp, dw)


// �ⲿ���ٽ��뵽���ݷ����б�
#define IGame_StartData(p)            GET_PVTBL(p,IGame)->GameService(p, STARTMETHOD_DATA);

#endif
