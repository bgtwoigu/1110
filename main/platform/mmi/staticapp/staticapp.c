/** ----------------------------------------------------------------------------
 *Staticapp.c
 *
 * Copyright(c) 2007 Anylook 
 *
 *
 * History:
 *
 * when         who            what and why
 * -----------  -----------    -----------------------------
 * 2008.12.29            create  3 X 4 large icon style menu  &  list menu
 *
 * -----------------------------------------------------------------------------
 */


#include "customer.h"
#include "OEMFeatures.h"
#include "OEMClassIds.h"
#ifndef AEECLSID_MAIN_MENU
    #error AEECLSID_MAIN_MENU must be defined
#endif

#include "AEEShell.h"
#include "AEE_OEM.h"
#include "AEEModGen.h"
#include "AEEModTable.h"
#include "AEEMenu.h"
#include "AEEStdLib.h"
#include "AEEAnnunciator.h"
#include "staticapp.h"
#include "staticapp_priv.h" 

#include "ContApp.h"

#include "Appscommon.h"
#include "appscommon.brh"
#include "appscommonimages.brh"

#if !defined( AEE_SIMULATOR)
#include "AEERUIM.h" 
#include "OEMCFGI.h"
#endif //#if !defined( AEE_SIMULATOR)
#include "err.h"
#define PARAM_NOT_REF(x)

/*==============================================================================

                              ���غ�������

==============================================================================*/
static StaticappMod gStaticappMod;
static const VTBL( IModule) gModFuncs =
{
    StaticappMod_AddRef,
    StaticappMod_Release,
    StaticappMod_CreateInstance,
    StaticappMod_FreeResources
};

static uint32  Staticapp_AddRef( IStaticapp *pi);

static uint32  Staticapp_Release ( IStaticapp *pi);

static boolean Staticapp_HandleEvent( IStaticapp *pi,
    AEEEvent eCode,
    uint16  wParam,
    uint32 dwParam
);


static Staticapp gStaticapp={0};

static const VTBL( IStaticapp) gStaticappMethods =
{
    Staticapp_AddRef,
    Staticapp_Release,
    Staticapp_HandleEvent,
};

int StaticappMod_Load( IShell *pIShell, void *ph, IModule **ppMod);

int StaticappMod_New( int16 nSize,
    IShell              *pIShell,
    void                *ph,
    IModule             **ppMod,
    PFNMODCREATEINST    pfnMC,
    PFNFREEMODDATA      pfnMF
);

static int Staticapp_New( IShell *ps, IModule *pIModule, IStaticapp **ppObj);

static int CStaticapp_InitAppData(Staticapp *pMe);

static void CStaticapp_FreeAppData(Staticapp *pMe);

static void Staticapp_RunFSM(Staticapp *pMe);

//static void calculateScreenParameters(MainMenu *pMe);

static boolean StartApplet(Staticapp *pMe, int i);

boolean Staticapp_RouteDialogEvt(Staticapp *pMe,
    AEEEvent    eCode,
    uint16  wParam,
    uint32 dwParam
);

NextFSMAction Staticapp_ProcessState(Staticapp *pMe);

void Staticapp_ShowDialog(Staticapp  *pMe,  uint16 dlgResId);

// STATICST_MAIN ״̬������
static NextFSMAction STATICAPPST_MAIN_Handler(Staticapp *pMe);


//STATICST_EXIT  ״̬������
static NextFSMAction STATICAPPST_EXIT_Handler(Staticapp *pMe);
static boolean  Staticapp_ListMenuHandler(Staticapp *pMe, AEEEvent eCode, uint16 wParam, uint32 dwParam);



/*==============================================================================
                              
                              ���أ���̬������
                              
==============================================================================*/



//wlh 20090410 add  end �ƶ�Ч��
/*=============================================================================
FUNCTION:  StaticappMod_Load

DESCRIPTION: 

PARAMETERS:

RETURN VALUE:

COMMENTS:

SIDE EFFECTS:

SEE ALSO:

=============================================================================*/
int StaticappMod_Load( IShell *pIShell, void *ph, IModule **ppMod)
{
    return StaticappMod_New( sizeof( StaticappMod),
                    pIShell,
                    ph,
                    ppMod,
                    NULL,
                    NULL);
}

/*=============================================================================
FUNCTION:  StaticappMod_New

DESCRIPTION: 

PARAMETERS:

RETURN VALUE:

COMMENTS:

SIDE EFFECTS:

SEE ALSO:

=============================================================================*/
int  StaticappMod_New( int16 nSize,
    IShell   *pIShell,
    void     *ph,
    IModule  **ppMod,
    PFNMODCREATEINST pfnMC,
    PFNFREEMODDATA pfnMF
)
{
#if !defined( AEE_SIMULATOR)
    PARAM_NOT_REF( nSize)
    PARAM_NOT_REF( pfnMC)
    PARAM_NOT_REF( pfnMF)
    PARAM_NOT_REF( ph)
#endif

    if( NULL == ppMod)
    {
        return EFAILED;
    }

    MEMSET( &gStaticappMod, 0, sizeof( StaticappMod));

    INIT_VTBL( &gStaticappMod, IModule, gModFuncs);
    gStaticappMod.referenceCounter = 1;
    *ppMod = ( IModule *)&gStaticappMod;
    return AEE_SUCCESS;
}

/*=============================================================================
FUNCTION:  StaticappMod_AddRef

DESCRIPTION: 

PARAMETERS:

RETURN VALUE:

COMMENTS:

SIDE EFFECTS:

SEE ALSO:

=============================================================================*/
static uint32 StaticappMod_AddRef( IModule *po)
{
    return( ++( ( StaticappMod *)po)->referenceCounter);
}

/*=============================================================================
FUNCTION:  StaticappMod_Release

DESCRIPTION: 

PARAMETERS:

RETURN VALUE:

COMMENTS:

SIDE EFFECTS:

SEE ALSO:

=============================================================================*/
static uint32 StaticappMod_Release( IModule *po)
{
    if ( ( ( StaticappMod *) po)->referenceCounter == 0)
    {
        return 0;
    }

    return( --( ( StaticappMod *)po)->referenceCounter);
}

/*=============================================================================
FUNCTION:  StaticappMod_CreateInstance

DESCRIPTION: 

PARAMETERS:

RETURN VALUE:

COMMENTS:

SIDE EFFECTS:

SEE ALSO:

=============================================================================*/
static int  StaticappMod_CreateInstance( IModule *po,
    IShell *pIShell,
    AEECLSID ClsId,
    void **ppObj
)
{
    *ppObj = NULL;

    if( ClsId != AEECLSID_STATIC_APP|| Staticapp_New( pIShell, po, ( IStaticapp**)ppObj) != SUCCESS)
    {
        return EFAILED;
    }

    return SUCCESS;
}

/*=============================================================================
FUNCTION:  StaticappMod_FreeResources

DESCRIPTION: 

PARAMETERS:

RETURN VALUE:

COMMENTS:

SIDE EFFECTS:

SEE ALSO:

=============================================================================*/
static void StaticappMod_FreeResources( IModule *po, IHeap *ph, IFileMgr *pfm)
{
}

/*=============================================================================
FUNCTION:  Staticapp_New

DESCRIPTION: 

PARAMETERS:

RETURN VALUE:

COMMENTS:

SIDE EFFECTS:

SEE ALSO:

=============================================================================*/
static int Staticapp_New( IShell *ps, IModule *pIModule, IStaticapp **ppObj)
{
    int retVal = SUCCESS;
    if( 0 == gStaticapp.referenceCounter)
    {

        if( pIModule == NULL || ps == NULL)
        {
            return EFAILED;
        }

        MEMSET(&gStaticapp, 0, sizeof(gStaticapp));
        INIT_VTBL(&gStaticapp, IStaticapp, gStaticappMethods);

       
        gStaticapp.m_pShell     = ps;
        gStaticapp.m_pModule    = pIModule;
        retVal = CStaticapp_InitAppData(&gStaticapp);
        if(retVal != SUCCESS)
        {
            CStaticapp_FreeAppData((Staticapp*)&gStaticapp);
            return retVal;
        }
        (void) ISHELL_AddRef( ps);
        (void) IMODULE_AddRef( pIModule);
        gStaticapp.referenceCounter  = 0;

        
    }

    ++ gStaticapp.referenceCounter;
    *ppObj = ( IStaticapp*)&gStaticapp;
    
    return retVal;
}

/*==============================================================================
����:
    CStaticapp_InitAppData

˵��:
    ��ʼ�� Staticapp Applet �ṹ����ȱʡֵ��

����:
    pMe [in]: ָ�� Staticapp Applet ����ṹ��ָ�롣�ýṹ����С������ض���Ϣ��

����ֵ:
    EFAILED: ��ʼ��ʧ�ܡ�
    SUCCESS: ��ʼ���ɹ���

��ע:

==============================================================================*/
static int CStaticapp_InitAppData(Staticapp *pMe)
{
	int i;
	boolean iamgeflag = FALSE;
    if (NULL == pMe)
    {
        return EFAILED;
    }

    pMe->m_MainSel  = 0;
    
	
    if (ISHELL_CreateInstance(pMe->m_pShell, AEECLSID_DISPLAY, 
            (void **) &pMe->m_pDisplay) != SUCCESS)
    {
        return EFAILED;
    }

	if (ISHELL_CreateInstance(pMe->m_pShell,
									 AEECLSID_ANNUNCIATOR,
									 (void **) &pMe->m_pIAnn))
	{
			CStaticapp_FreeAppData(pMe);
			return FALSE;
	}
    return SUCCESS;
}

/*==============================================================================
����:
    CStaticapp_FreeAppData

˵��:
    �ͷ� Staticapp Applet �����Դ��

����:
    pMe [in]: ָ��Staticapp Applet ����ṹ��ָ�롣�ýṹ����С������ض���Ϣ��

����ֵ:
    none

��ע:

==============================================================================*/
static void CStaticapp_FreeAppData(Staticapp *pMe)
{
    if (NULL == pMe)
    {
        return;
    }

    pMe->m_eAppStatus = STATICAPP_STOP; 
    
    if (pMe->m_pDisplay != NULL)
    {
        (void) IDISPLAY_Release(pMe->m_pDisplay);
        pMe->m_pDisplay = NULL;
    }
	if (pMe->m_pIAnn)
    {
        IANNUNCIATOR_Release(pMe->m_pIAnn);
        pMe->m_pIAnn= NULL;
    }

}

/*=============================================================================
FUNCTION: Staticapp_AddRef

DESCRIPTION: 

PARAMETERS:

RETURN VALUE:

COMMENTS:

SIDE EFFECTS:

SEE ALSO:

=============================================================================*/
static uint32  Staticapp_AddRef( IStaticapp *pi)
{
    return ( ++gStaticapp.referenceCounter);
}

/*=============================================================================
FUNCTION:  Staticapp_Release

DESCRIPTION: 

PARAMETERS:

RETURN VALUE:

COMMENTS:

SIDE EFFECTS:

SEE ALSO:

=============================================================================*/
static uint32  Staticapp_Release( IStaticapp *pi)
{
    register Staticapp *pMe = (Staticapp*)pi;

    if (pMe->referenceCounter == 0)
    {
        return 0;
    }
    
    if( --pMe->referenceCounter)
    {
        return pMe->referenceCounter;
    }


    CStaticapp_FreeAppData(pMe);

    (void) ISHELL_Release(pMe->m_pShell);
    (void) IMODULE_Release(pMe->m_pModule);

    return 0;
}

/*==============================================================================
����:
    Staticapp_RunFSM

˵��:
    ��������״̬�����档

����:
    pMe [in]: ָ��Staticapp Applet����ṹ��ָ�롣�ýṹ����С������ض���Ϣ��
       
����ֵ:
    �ޡ�

��ע:

==============================================================================*/
static void Staticapp_RunFSM(Staticapp *pMe)
{
    NextFSMAction nextFSMAction = NFSMACTION_WAIT;
    for ( ; ; )
    {
        nextFSMAction = Staticapp_ProcessState(pMe);

        // ���ﲻ��Ҫ���ӵ�״̬�������Ϊ��״̬������������һ������������
        // ָʾdlgResult�Ƿ�Ӧ�ñ���д����ĳЩ���ⳡ�ϣ���״̬����ƣ���������
        // ���������˲���Ҫ�ĸ����ԣ�������ơ�
        if (pMe->m_bDoNotOverwriteDlgResult)
        {
            // We allow only one iteration without touching this variable. So,
            // let's reset it back to indicate that we are going to overwrite the
            // result for all subsequent iterations.
            pMe->m_bDoNotOverwriteDlgResult = FALSE;
        }
        else
        {
            // Overwrite the dlgResult to its default value. The next dialog that
            // we are going to display will set and return the dlgResult based on
            // the users action.
            pMe->m_eDlgReturn = DLGRET_CREATE;
        }

        // If the state machine processing engine indicates that it created a
        // dialog and requesting us to exit this loop, then we quit.
        if (nextFSMAction == NFSMACTION_WAIT)
        {
            break;
        }
    }
}

/*=============================================================================
FUNCTION:  Staticapp_HandleEvent

DESCRIPTION: 

PARAMETERS:

RETURN VALUE:

COMMENTS:

SIDE EFFECTS:

SEE ALSO:

=============================================================================*/
static boolean Staticapp_HandleEvent( IStaticapp *pi,
    AEEEvent eCode,
    uint16  wParam,
    uint32 dwParam
)
{
    Staticapp *pMe = (Staticapp*)pi;
    AEEAppStart* as = NULL;
    AEEDeviceInfo di; 

    ISHELL_GetDeviceInfo(pMe->m_pShell,&di); 
    switch ( eCode)
    {
        case EVT_APP_START:
			
            // ���¼�dwParamΪָ�룬��ӦΪ0
            if (dwParam == 0) 
            {
                return FALSE;
            }            
            as = (AEEAppStart*)dwParam;
   
            pMe->m_rc = as->rc;
            pMe->m_currState  = STATICAPPST_MAIN;
            pMe->m_eDlgReturn = DLGRET_CREATE;
            pMe->m_eAppStatus = STATICAPP_RUNNING;
            if(pMe->m_pIAnn != NULL)
            {
                IANNUNCIATOR_SetFieldIsActiveEx(pMe->m_pIAnn,FALSE);
            }
            {
                AECHAR WTitle[20] = {0};
    			(void)ISHELL_LoadResString(pMe->m_pShell,
                                        STATICAPP_RES_FILE_LANG,                                
                                        IDS_STATICAPP_LIST,
                                        WTitle,
                                        sizeof(WTitle));
                if(pMe->m_pIAnn != NULL)
                {
    			    IANNUNCIATOR_SetFieldText(pMe->m_pIAnn,WTitle);
                }
            }
            Staticapp_RunFSM(pMe);
            return TRUE;

        case EVT_APP_STOP:
            {
                if (pMe->m_pDisplay != NULL)
                {
                    (void) IDISPLAY_Release(pMe->m_pDisplay);
                    pMe->m_pDisplay = NULL;
                }
                pMe->m_eAppStatus = STATICAPP_STOP;
                
                return TRUE;
            }

        case EVT_APP_SUSPEND:
            pMe->m_eAppStatus = STATICAPP_SUSPEND;
            return TRUE;
        

        case EVT_APP_RESUME:
            {
                AEEAppStart* as = ( AEEAppStart*)dwParam;
                pMe->m_rc    = as->rc;
                pMe->m_rc.dy = di.cyScreen;
                pMe->m_eAppStatus = STATICAPP_RUNNING;
                Staticapp_RunFSM(pMe); 
                return TRUE;
            }

        case EVT_DIALOG_INIT:
            
            pMe->m_pActiveIDlg = (IDialog*)dwParam;
            pMe->m_pActivedlgID = wParam;
            
            return Staticapp_RouteDialogEvt(pMe,eCode,wParam,dwParam);

        case EVT_DIALOG_START:

            return Staticapp_RouteDialogEvt(pMe,eCode,wParam,dwParam);

        case EVT_USER_REDRAW:
            return Staticapp_RouteDialogEvt(pMe,eCode,wParam,dwParam);

        case EVT_DIALOG_END:
		  
            (void) Staticapp_RouteDialogEvt(pMe,eCode,wParam,dwParam);
            pMe->m_pActiveIDlg = NULL;
            pMe->m_pActivedlgID = 0;
            if(pMe->m_eAppStatus == STATICAPP_RUNNING)
            {
                Staticapp_RunFSM(pMe);
            }
            return TRUE;
            
        case EVT_KEY:
#if MIN_BREW_VERSION(3,0)
            // do not want to handle au
            if (((dwParam & KB_AUTOREPEAT) != 0) &&
                (wParam != AVK_UP) &&
                (wParam != AVK_DOWN) &&
                (wParam != AVK_LEFT) &&
                (wParam != AVK_RIGHT))
            {
                return TRUE;
            }
#endif
            return Staticapp_RouteDialogEvt(pMe,eCode,wParam,dwParam);   

        case EVT_KEY_PRESS:
        case EVT_KEY_RELEASE:
        case EVT_COMMAND:
            return Staticapp_RouteDialogEvt(pMe,eCode,wParam,dwParam); 
        default:
            return Staticapp_RouteDialogEvt(pMe,eCode,wParam,dwParam);
    }
}

/*==============================================================================
����:
   Staticapp_ProcessState

˵��:
    ���� pMe->m_currState, ������Ӧ��״̬��������

����:
    pMe [in]: ָ��Staticapp Applet����ṹ��ָ�롣�ýṹ����С������ض���Ϣ��

����ֵ:
    NFSMACTION_CONTINUE: ָʾ��ͣ״̬����
    NFSMACTION_WAIT: ָʾֹͣ״̬����

��ע:

==============================================================================*/
NextFSMAction Staticapp_ProcessState(Staticapp *pMe)
{
    NextFSMAction retVal = NFSMACTION_WAIT;
    
    if (NULL == pMe)
    {
        return retVal;
    }
    
    switch(pMe->m_currState)
    {
        case STATICAPPST_MAIN:
            return STATICAPPST_MAIN_Handler(pMe);

        case STATICAPPST_EXIT:
            return STATICAPPST_EXIT_Handler(pMe);            

        default:
            break;

    }
    return retVal;
}

/*==============================================================================
����:
    STATICAPPST_MAIN_Handler

˵��:
    STATICAPP_MAIN ״̬��������

����:
    pMe [in]: ָ��STATICAPP Applet����ṹ��ָ�롣�ýṹ����С������ض���Ϣ��

����ֵ:
    NFSMACTION_CONTINUE: ָʾ��ͣ״̬����
    NFSMACTION_WAIT: ָʾֹͣ״̬����

��ע:

==============================================================================*/
static NextFSMAction STATICAPPST_MAIN_Handler(Staticapp *pMe)
{
    if (NULL == pMe)
    {
        return NFSMACTION_WAIT;
    }
    switch (pMe->m_eDlgReturn)
    {
        // ����������
        case DLGRET_CREATE:
            {
                  Staticapp_ShowDialog(pMe, IDD_LIST_STATICAPP);

            }
            return NFSMACTION_WAIT;
        default:
            MOVE_TO_STATE(STATICAPPST_EXIT)
            return NFSMACTION_CONTINUE;
    }
}




/*==============================================================================
����:
    STATICAPPST_EXIT_Handler

˵��:
    STATICAPPST_EXIT ״̬��������

����:
    pMe [in]: ָ��STATICAPP  Applet����ṹ��ָ�롣�ýṹ����С������ض���Ϣ��

����ֵ:
    NFSMACTION_CONTINUE: ָʾ��ͣ״̬����
    NFSMACTION_WAIT: ָʾֹͣ״̬����

��ע:

==============================================================================*/
static NextFSMAction STATICAPPST_EXIT_Handler(Staticapp *pMe)
{
    (void) ISHELL_CloseApplet(pMe->m_pShell, FALSE);
    return NFSMACTION_WAIT;
} 


/*==============================================================================
����:
       Staticapp_ShowDialog

˵��:
    ������״̬����������������Ӧ�Ի�����ԴID�ĶԻ���

����:
    pMe [in]: ָ�� Staticapp Applet ����ṹ��ָ�롣�ýṹ����С������ض���Ϣ��
    dlgResId [in]: �Ի�����ԴID���������ݸ�ID�����Ի���

����ֵ:
    none

��ע:

==============================================================================*/
void Staticapp_ShowDialog(Staticapp  *pMe,  uint16 dlgResId)
{
    int nRet;

    // ÿ������һ���Ի���
    if (ISHELL_GetActiveDialog(pMe->m_pShell) != NULL)
    {
        // �����ǰ�Ѿ��жԻ��򱻴򿪣�ֱ�ӷ���
        DBGPRINTF("Trying to create dialog %d without closing previous one",dlgResId,0,0);
        return;
    }
    if (NULL != pMe->m_pDisplay)
    {
        AEEDeviceInfo di={0,};
       (void)IDISPLAY_SetPrefs(pMe->m_pDisplay, "a:1", STRLEN("a:1"));
        ISHELL_GetDeviceInfo(pMe->m_pShell, &di);
        pMe->m_rc.dx = di.cxScreen;
        pMe->m_rc.dy = di.cyScreen;
        IDISPLAY_SetClipRect(pMe->m_pDisplay, &pMe->m_rc);
        //calculateScreenParameters(pMe);
    }
    nRet = ISHELL_CreateDialog(pMe->m_pShell,STATICAPP_RES_FILE_LANG,dlgResId,NULL);
    if (nRet != SUCCESS)
    {
        DBGPRINTF("Failed to create the dialog %d in the MAINMENU applet.",dlgResId,0,0);
    }
}


/*==============================================================================
����:
      Staticapp_RouteDialogEvt
˵��:
       ������BREW�¼�·�ɸ���ǰ��Ի�����¼���������

����:
       pMe [In]: ָ��Staticapp Applet����ṹ��ָ��,�ýṹ����С������ض���Ϣ.
       eCode [in]���¼����롣
       wParam [in]�����¼�eCode���������ݡ�
       dwParam [in]�����¼�eCode���������ݡ�

����ֵ:
       TRUE�������¼�������
       FALSE�������¼�û������

��ע:

==============================================================================*/
boolean Staticapp_RouteDialogEvt(Staticapp *pMe,
    AEEEvent    eCode,
    uint16  wParam,
    uint32 dwParam
)

{
    if (NULL == pMe)
    {
        return FALSE;
    }

    if (NULL == pMe->m_pActiveIDlg)
    {
        return FALSE;
    }    
    switch( pMe->m_pActivedlgID)
    {

        case IDD_LIST_STATICAPP:
            return Staticapp_ListMenuHandler(pMe, eCode, wParam, dwParam);

        default:
            return FALSE;
    }
}



/*=============================================================================
FUNCTION: Staticapp_ListMenuHandler

DESCRIPTION:   �б�˵�

PARAMETERS:

=============================================================================*/

static boolean Staticapp_ListMenuHandler(Staticapp *pMe, AEEEvent eCode, uint16 wParam, uint32 dwParam)
{
    PARAM_NOT_REF(dwParam)
    IMenuCtl *pMenu = (IMenuCtl*)IDIALOG_GetControl(pMe->m_pActiveIDlg,IDC_STATICAPP_LIST);
    AECHAR WTitle[40] = {0};
    if (pMenu == NULL)
    {
        return FALSE;
    }
    if(pMe->m_pIAnn != NULL)
    {
	    IANNUNCIATOR_SetFieldIsActiveEx(pMe->m_pIAnn,FALSE);
    }
    switch (eCode)
    {
        case EVT_DIALOG_INIT:
			
			(void)ISHELL_LoadResString(pMe->m_pShell,
                                    STATICAPP_RES_FILE_LANG,                                
                                    IDS_STATICAPP_LIST,
                                    WTitle,
                                    sizeof(WTitle));
		
            if(pMe->m_pIAnn != NULL)
            {
			    IANNUNCIATOR_SetFieldText(pMe->m_pIAnn,WTitle);
            }
			
			IMENUCTL_AddItem(pMenu, STATICAPP_RES_FILE_LANG,IDS_STATICAPP_TITLE_1, IDS_STATICAPP_TITLE_1, NULL, 0);
		    IMENUCTL_AddItem(pMenu, STATICAPP_RES_FILE_LANG,IDS_STATICAPP_TITLE_2, IDS_STATICAPP_TITLE_2, NULL, 0);
			IMENUCTL_AddItem(pMenu, STATICAPP_RES_FILE_LANG,IDS_STATICAPP_TITLE_3, IDS_STATICAPP_TITLE_3, NULL, 0);
            return TRUE;
            
        case EVT_DIALOG_START:
            {  
                int i;
                uint16 wID;
                AECHAR pwsz[67] = {0};
                AECHAR pstr[64] = {0};
                AECHAR wsFmt[5] = {0};
                
                (void)STRTOWSTR("%d. ",wsFmt,sizeof(wsFmt));
                for (i=0;i<IMENUCTL_GetItemCount(pMenu);i++)
                {
                    wID = IMENUCTL_GetItemID(pMenu, i);
                    WSPRINTF(pwsz,sizeof(pwsz),wsFmt,i+1);
                    
                    ISHELL_LoadResString( pMe->m_pShell,
                          STATICAPP_RES_FILE_LANG,
                          wID,
                          pstr,
                          sizeof(pstr));
                    WSTRLCAT(pwsz,pstr,sizeof(pwsz));
                    IMENUCTL_SetItemText(pMenu, wID, NULL, NULL, pwsz);
                }
                
                IMENUCTL_SetProperties(pMenu, MP_UNDERLINE_TITLE|MP_WRAPSCROLL);
                IMENUCTL_SetOemProperties( pMenu, OEMMP_USE_MENU_STYLE);
                IMENUCTL_SetBottomBarType(pMenu,BTBAR_SELECT_BACK);
                IMENUCTL_SetSel(pMenu, pMe->m_MainSel);
                (void) ISHELL_PostEvent(pMe->m_pShell, AEECLSID_MAIN_MENU, EVT_USER_REDRAW,0,0);
            }
            return TRUE;
            
        case EVT_USER_REDRAW:
            (void)IMENUCTL_Redraw(pMenu);
            return TRUE;
            
        case EVT_DIALOG_END:
            return TRUE;

        case EVT_KEY:
            switch(wParam)
            {
                case AVK_1:
                case AVK_STAR:
                case AVK_POUND:
                    {
                        int Focus = (wParam - AVK_1);
                        if(Focus<IMENUCTL_GetItemCount(pMenu))
                        {
                            StartApplet(pMe, Focus);
                        }
                    }
                    return TRUE;

                case AVK_CLR:
                    CLOSE_DIALOG(DLGRET_CANCELED)
                    return TRUE;
                    
                default:
                    break;
          
            }
            return TRUE;
            
        case EVT_COMMAND:
            pMe->m_MainSel = wParam;
			StartApplet(pMe, wParam - IDS_STATICAPP_TITLE_1);
        default:
            break;
    }             
    return FALSE;
}




/*=============================================================================
FUNCTION:  StartApplet

DESCRIPTION:  ����applet

PARAMETERS:  ���APPLET �б䶯��ֻ��Ķ��κ���

=============================================================================*/
static boolean StartApplet(Staticapp *pMe, int i)
{
    int Result = FALSE;

   	//MSG_ERROR("StartApplet:::::%d",i,0,0);
    switch(i)
    {
       
        case 0:
        	#ifdef STATIC_BREW_APP_FOR_NASRANI_NOR_MUSLIM
			   OEM_SetBAM_ADSAccount(STATIC_BREW_APP_FLEXI_NASRANI);
               Result = ISHELL_StartApplet(pMe->m_pShell, AEECLSID_NASRANI);
			#else
			   OEM_SetBAM_ADSAccount(STATIC_BREW_APP_FLEXI_MUSLIM);
               Result = ISHELL_StartApplet(pMe->m_pShell, AEECLSID_MUSLIM);
			#endif
            break;
            
        case 1:
        	OEM_SetBAM_ADSAccount(STATIC_BREW_APP_FLEXI_PORTAL);
			Result = ISHELL_StartApplet(pMe->m_pShell, AEECLSID_FCHAT);
            break;
  		case 2:
  			OEM_SetBAM_ADSAccount(STATIC_BREW_APP_FLEXI_PORTAL);			
			Result = ISHELL_StartApplet(pMe->m_pShell, AEECLSID_FBROWSER);
            break;
  
		
        default:
            break;
    }
    return TRUE;
}
