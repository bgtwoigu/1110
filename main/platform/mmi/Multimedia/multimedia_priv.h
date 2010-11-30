/** ----------------------------------------------------------------------------
 * mainmenu_priv.h
 *
 * Copyright(c) 2007 Anylook
 *
 *
 * History:
 *
 * when            who               what and why
 * -----------  -----------    -----------------------------
 * 2008.12.24                created  for  Q1
 *
 * -----------------------------------------------------------------------------
 */

#ifndef MAINMENU_PRIV_H
#define MAINMENU_PRIV_H
#endif

#include "multimedia.brh"
#include "AEEConfig.h"

#include "AEEAnnunciator.h"
#ifdef FEATRUE_SUPPORT_G_SENSOR
#include "G_Sensor.h"
#endif



/*==============================================================================
                                 
                                ����������Ͷ���
                                 
==============================================================================*/

typedef struct _MultimediaMod
{
    DECLARE_VTBL( IModule)

    uint32   referenceCounter;
    
} MultimediaMod;

//MAINMENU  �µļ���״̬
typedef enum MultimediaState
{
    MULTIMEDIAST_MAIN,

    MULTIMEDIAST_GAME,
    
    MULTIMEDIAST_PLAYER,

    MULTIMEDIAST_DATA, 

    MULTIMEDIAST_EXIT

} MultimediaState;

typedef enum
{
    MULTIMEDIA_STOP=0x00,
    MULTIMEDIA_RUNNING,
    MULTIMEDIA_SUSPEND
} MULTIMEDIA_STATUS_e_type;

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

typedef struct _Multimed
{
    DECLARE_VTBL(IMultimed)
    uint32          referenceCounter;
    
    IModule        *m_pModule;
    IDisplay       *m_pDisplay;
    IShell         *m_pShell;

    IDialog        *m_pActiveIDlg;
    uint32          m_pActivedlgID;
    uint16          m_MainSel;   //һ���˵����
    DLGRetValue     m_eDlgReturn;

    MULTIMEDIA_STATUS_e_type  m_eAppStatus;   // Applet ��ǰ����״̬
    
    boolean         m_bDoNotOverwriteDlgResult; //�Ƿ���д�Ի�����    
    MultimediaState m_prevState;        // Appletǰһ״̬
    MultimediaState m_currState;        // Applet��ǰ״̬
    AEERect         m_rc;
    
    IAnnunciator   *m_pIAnn;
} Multimed;

/*==============================================================================
                                 
                                 �궨��ͳ���
                                 
==============================================================================*/

#define MULTIMEDIA_RES_FILE_LANG        (AEE_RES_LANGDIR MULTIMEDIA_RES_FILE)

// �˺��õ�ǰ״̬������ǰ״̬������nextState״̬���µ�ǰ״̬
#define MOVE_TO_STATE(nextState)            \
{                                           \
    MultimediaState tpState;                    \
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

static int MultimedMod_CreateInstance( IModule *po,
    IShell      *pIShell,
    AEECLSID    ClsId,
    void        **ppObj
);

static uint32  MultimedMod_AddRef( IModule *pi);

static uint32  MultimedMod_Release ( IModule *pi);


static void MultimedMod_FreeResources( IModule *po, IHeap *ph, IFileMgr *pfm);

