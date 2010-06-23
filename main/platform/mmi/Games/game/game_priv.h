/** ----------------------------------------------------------------------------
 * game_priv.h
 *
 * Copyright(c) 2007 Anylook
 *
 *
 * History:
 *
 * when            who               what and why
 * -----------  -----------    -----------------------------
 * 20010.06.23                created  for  w1100
 *
 * -----------------------------------------------------------------------------
 */

#ifndef GAME_PRIV_H
#define GAME_PRIV_H
#endif

#include "game.brh"
#include "AEEConfig.h"

#ifdef FEATRUE_SUPPORT_G_SENSOR
#include "G_Sensor.h"
#endif



/*==============================================================================
                                 
                                ����������Ͷ���
                                 
==============================================================================*/

typedef struct _GameMod
{
    DECLARE_VTBL( IModule)

    uint32   referenceCounter;
    
} GameMod;

//Application  �µļ���״̬
typedef enum GameState
{
    GAMEST_MAIN,

    GAMEST_GAME,
    
    GAMEST_PLAYER,

    GAMEST_DATA, 
 

    GAMEST_EXIT

} GameState;


typedef enum
{
    GAME_STOP=0x00,
    GAME_RUNNING,
    GAME_SUSPEND
} GAME_STATUS_e_type;

typedef enum DLGRetValue
{
    DLGRET_CREATE,
        
    DLGRET_GAME,

    DLGRET_MEDIA,

    DLGRET_MAIN,

    DLGRET_DATA,


    DLGRET_CANCELED

}DLGRetValue;

typedef enum NextFSMAction
{
   NFSMACTION_WAIT,
   NFSMACTION_CONTINUE
   
} NextFSMAction;


typedef struct _Point
{
    int x;
    int y;
} Point;

typedef struct _Game
{
    DECLARE_VTBL(IGame)
    uint32       referenceCounter;
    
    IModule     *m_pModule;
    IDisplay    *m_pDisplay;
    IShell      *m_pShell;

    IDialog     *m_pActiveIDlg;
    uint32       m_pActivedlgID;
    uint16       m_MainSel;   //һ���˵����
    DLGRetValue  m_eDlgReturn;

    GAME_STATUS_e_type  m_eAppStatus;   // Applet ��ǰ����״̬
    
    boolean     m_bDoNotOverwriteDlgResult; //�Ƿ���д�Ի�����    
    GameState             m_prevState;        // Appletǰһ״̬
    GameState             m_currState;        // Applet��ǰ״̬
    AEERect     m_rc;
} Game;

/*==============================================================================
                                 
                                 �궨��ͳ���
                                 
==============================================================================*/

#define  MAX_MATRIX_ITEMS 3

#define ICON_ANIMATED_FRAME     1 //����ͼƬ��֡��


#define GAME_RES_FILE_LANG        AEE_RES_LANGDIR GAME_RES_FILE

#define STARTARGPREFIX_VIEWPLAYER            'P'
#define STARTARGPREFIX_VIEWDATA              'D'




// �˺��õ�ǰ״̬������ǰ״̬������nextState״̬���µ�ǰ״̬
#define MOVE_TO_STATE(nextState)            \
{                                           \
    GameState tpState;                    \
    tpState = nextState;                    \
    pMe->m_prevState = pMe->m_currState;    \
    pMe->m_currState = tpState;             \
}

//�رնԻ���ĺ�
#define CLOSE_DIALOG(dlgResult)             \
{                                           \
    pMe->m_eDlgReturn = dlgResult;          \
    (void) ISHELL_EndDialog(pMe->m_pShell); \
}

/*==============================================================================

                                 ��������

==============================================================================*/

static int GameMod_CreateInstance( IModule *po,
    IShell      *pIShell,
    AEECLSID    ClsId,
    void        **ppObj
);

static uint32 GameMod_AddRef( IModule *po);

static uint32 GameMod_Release( IModule *po);

static void GameMod_FreeResources( IModule *po, IHeap *ph, IFileMgr *pfm);

