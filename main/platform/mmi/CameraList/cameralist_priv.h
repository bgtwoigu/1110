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

#include "cameralist.brh"
#include "AEEConfig.h"

#include "AEEAnnunciator.h"
#ifdef FEATRUE_SUPPORT_G_SENSOR
#include "G_Sensor.h"
#endif



/*==============================================================================
                                 
                                ����������Ͷ���
                                 
==============================================================================*/

typedef struct _CameralistMod
{
    DECLARE_VTBL( IModule)

    uint32   referenceCounter;
    
} CameralistMod;

//MAINMENU  �µļ���״̬
typedef enum CameralistState
{
    CAMERALIST_MAIN,
    CAMERALIST_EXIT
} CameralistState;

typedef enum
{
    CAMERALIST_STOP=0x00,
    CAMERALIST_RUNNING,
    CAMERALIST_SUSPEND
} CAMERALIST_STATUS_e_type;

typedef enum DLGRetValue
{
    DLGRET_CREATE,
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

typedef struct _Cameralist
{
    DECLARE_VTBL(ICameralist)
    uint32          referenceCounter;
    
    IModule        *m_pModule;
    IDisplay       *m_pDisplay;
    IShell         *m_pShell;

    IDialog        *m_pActiveIDlg;
    uint32          m_pActivedlgID;
    uint16          m_MainSel;   //һ���˵����
    DLGRetValue     m_eDlgReturn;

    CAMERALIST_STATUS_e_type  m_eAppStatus;   // Applet ��ǰ����״̬
    
    boolean         m_bDoNotOverwriteDlgResult; //�Ƿ���д�Ի�����    
    CameralistState m_prevState;        // Appletǰһ״̬
    CameralistState m_currState;        // Applet��ǰ״̬
    AEERect         m_rc;
    
    IAnnunciator    *m_pIAnn;
#if defined(FEATURE_VERSION_C316)
   char             *m_strPhonePWD;
   boolean          b_pwdWright;
#endif
} Cameralist;

/*==============================================================================
                                 
                                 �궨��ͳ���
                                 
==============================================================================*/

#define CAMERALIST_RES_FILE_LANG        (AEE_RES_LANGDIR CAMERALIST_RES_FILE)

// �˺��õ�ǰ״̬������ǰ״̬������nextState״̬���µ�ǰ״̬
#define MOVE_TO_STATE(nextState)            \
{                                           \
    CameralistState tpState;                    \
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

static int CameralistMod_CreateInstance( IModule *po,
    IShell      *pIShell,
    AEECLSID    ClsId,
    void        **ppObj
);

static uint32  CameralistMod_AddRef( IModule *pi);

static uint32  CameralistMod_Release ( IModule *pi);


static void CameralistMod_FreeResources( IModule *po, IHeap *ph, IFileMgr *pfm);

