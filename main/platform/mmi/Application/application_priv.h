/** ----------------------------------------------------------------------------
 * application_priv.h
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

#ifndef APPLICATION_PRIV_H
#define APPLICATION_PRIV_H
#endif

#include "application.brh"
#include "AEEConfig.h"

#ifdef FEATRUE_SUPPORT_G_SENSOR
#include "G_Sensor.h"
#endif



/*==============================================================================
                                 
                                ����������Ͷ���
                                 
==============================================================================*/

typedef struct _ApplicationMod
{
    DECLARE_VTBL( IModule)

    uint32   referenceCounter;
    
} ApplicationMod;

//Application  �µļ���״̬
typedef enum ApplicationState
{
    APPLICATIONST_MAIN,

    APPLICATIONST_GAME,
    
    APPLICATIONST_PLAYER,

    APPLICATIONST_DATA, 
 

    APPLICATIONST_EXIT

} ApplicationState;


typedef enum
{
    APPLICATION_STOP=0x00,
    APPLICATION_RUNNING,
    APPLICATION_SUSPEND
} APPLICATION_STATUS_e_type;

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

typedef struct _Application
{
    DECLARE_VTBL(IApplication)
    uint32       referenceCounter;
    
    IModule     *m_pModule;
    IDisplay    *m_pDisplay;
    IShell      *m_pShell;

    IDialog     *m_pActiveIDlg;
    uint32       m_pActivedlgID;
    uint16       m_MainSel;   //һ���˵����
    DLGRetValue  m_eDlgReturn;

    APPLICATION_STATUS_e_type  m_eAppStatus;   // Applet ��ǰ����״̬
    
    boolean     m_bDoNotOverwriteDlgResult; //�Ƿ���д�Ի�����    
    ApplicationState             m_prevState;        // Appletǰһ״̬
    ApplicationState             m_currState;        // Applet��ǰ״̬
    AEERect     m_rc;
} Application;

/*==============================================================================
                                 
                                 �궨��ͳ���
                                 
==============================================================================*/
#if defined (FEATURE_DISP_128X128)
#define  MAX_MATRIX_ITEMS 9
#elif defined(FEATURE_DISP_160X128)
#define  MAX_MATRIX_ITEMS 6
#endif
#define ICON_ANIMATED_FRAME     1 //����ͼƬ��֡��


#define APPLICATION_RES_FILE_LANG        AEE_RES_LANGDIR APPLICATION_RES_FILE

#define STARTARGPREFIX_VIEWPLAYER            'P'
#define STARTARGPREFIX_VIEWDATA              'D'




// �˺��õ�ǰ״̬������ǰ״̬������nextState״̬���µ�ǰ״̬
#define MOVE_TO_STATE(nextState)            \
{                                           \
    ApplicationState tpState;                    \
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

static int ApplicationMod_CreateInstance( IModule *po,
    IShell      *pIShell,
    AEECLSID    ClsId,
    void        **ppObj
);

static uint32 ApplicationMod_AddRef( IModule *po);

static uint32 ApplicationMod_Release( IModule *po);

static void ApplicationMod_FreeResources( IModule *po, IHeap *ph, IFileMgr *pfm);

