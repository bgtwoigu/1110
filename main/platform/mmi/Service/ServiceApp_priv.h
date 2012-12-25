/** ----------------------------------------------------------------------------
 * IndexZoneApp_priv.h
 *
 * Copyright(c) 2012 zzg  
 *
 * History:
 *
 * when               who               what and why
 * -----------  -----------    -----------------------------
 * 20012.12.15      zzg                created 
 * -----------------------------------------------------------------------------
 */

#ifndef SERVICE_PRIV_H
#define SERVICE_PRIV_H
#endif

#include "service.brh"
#include "AEEConfig.h"

#include "AEEAnnunciator.h"

/*==============================================================================
                                 
                                ����������Ͷ���
                                 
==============================================================================*/

typedef struct _ServiceAppMod
{
    DECLARE_VTBL( IModule)

    uint32   referenceCounter;
    
} ServiceAppMod;


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

typedef struct _ServiceApp
{
    DECLARE_VTBL(IServiceApp)
    uint32       referenceCounter;
    
    IModule     *m_pModule;
    IDisplay    *m_pDisplay;
    IShell      *m_pShell;

    IDialog     *m_pActiveIDlg;
    uint32       m_pActivedlgID;
    uint16       m_MainSel;   				//һ���˵����
        
    boolean     m_bDoNotOverwriteDlgResult; //�Ƿ���д�Ի�����  
    
    AEERect     m_rc;
    IAnnunciator *m_pIAnn;
}ServiceApp;

/*==============================================================================
                                 
                                 �궨��ͳ���
                                 
==============================================================================*/

#define  MAX_MATRIX_ITEMS 3

#define  SERVICE_RES_FILE_LANG        AEE_RES_LANGDIR SERVICE_RES_FILE


//�رնԻ���ĺ�
#define CLOSE_DIALOG(dlgResult)             \
{                                           \
    pMe->m_eDlgReturn = dlgResult;          \
    (void) ISHELL_EndDialog(pMe->m_pShell); \
}

/*==============================================================================
                                 ��������
==============================================================================*/
static int ServiceAppMod_CreateInstance(IModule *po, IShell *pIShell, AEECLSID  ClsId, void  **ppObj);

static uint32 ServiceAppMod_AddRef(IModule *po);

static uint32 ServiceAppMod_Release(IModule *po);

static void ServiceAppMod_FreeResources(IModule *po, IHeap *ph, IFileMgr *pfm);

