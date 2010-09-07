/** ----------------------------------------------------------------------------
 * staticapp_priv.h
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

#ifndef STATICAPP_PRIV_H
#define STATICAPP_PRIV_H
#endif

#include "staticapp.brh"
#include "AEEConfig.h"
#include "AEEAnnunciator.h"



/*==============================================================================
                                 
                                ����������Ͷ���
                                 
==============================================================================*/

typedef struct _StaticappMod
{
    DECLARE_VTBL( IModule)

    uint32   referenceCounter;
    
} StaticappMod;

//STATICAPP  �µļ���״̬
typedef enum StaticappState
{
    STATICAPPST_MAIN,

    STATICAPPST_GAME,
    
    STATICAPPST_PLAYER,

    STATICAPPST_DATA, 
 

    STATICAPPST_EXIT

} StaticappState;


typedef enum
{
    STATICAPP_STOP=0x00,
    STATICAPP_RUNNING,
    STATICAPP_SUSPEND
} STATICAPP_STATUS_e_type;

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

typedef struct _Staticapp
{
    DECLARE_VTBL(IStaticapp)
    uint32       referenceCounter;
    
    IModule     *m_pModule;
    IDisplay    *m_pDisplay;
    IShell      *m_pShell;

    IDialog     *m_pActiveIDlg;
    uint32       m_pActivedlgID;
    uint16       m_MainSel;   //һ���˵����
    DLGRetValue  m_eDlgReturn;

    STATICAPP_STATUS_e_type  m_eAppStatus;   // Applet ��ǰ����״̬
    
    boolean     m_bDoNotOverwriteDlgResult; //�Ƿ���д�Ի�����    
    StaticappState             m_prevState;        // Appletǰһ״̬
    StaticappState             m_currState;        // Applet��ǰ״̬
    AEERect     m_rc;
    IAnnunciator                    *m_pIAnn;
} Staticapp;

/*==============================================================================
                                 
                                 �궨��ͳ���
                                 
==============================================================================*/
#if defined (FEATURE_DISP_128X128)
#define  MAX_MATRIX_ITEMS 10
#elif defined(FEATURE_DISP_160X128)
#define  MAX_MATRIX_ITEMS 7
#endif
#define ICON_ANIMATED_FRAME     1 //����ͼƬ��֡��


#define STATICAPP_RES_FILE_LANG        AEE_RES_LANGDIR STATICAPP_RES_FILE






// �˺��õ�ǰ״̬������ǰ״̬������nextState״̬���µ�ǰ״̬
#define MOVE_TO_STATE(nextState)            \
{                                           \
    StaticappState tpState;                    \
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

static int StaticappMod_CreateInstance( IModule *po,
    IShell      *pIShell,
    AEECLSID    ClsId,
    void        **ppObj
);

static uint32 StaticappMod_AddRef( IModule *po);

static uint32 StaticappMod_Release( IModule *po);

static void StaticappMod_FreeResources( IModule *po, IHeap *ph, IFileMgr *pfm);

