/** ----------------------------------------------------------------------------
 *Application.c
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
#include "application.h"
#include "application_priv.h" 

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
static ApplicationMod gApplicationMod;
static const VTBL( IModule) gModFuncs =
{
    ApplicationMod_AddRef,
    ApplicationMod_Release,
    ApplicationMod_CreateInstance,
    ApplicationMod_FreeResources
};

static uint32  Application_AddRef( IApplication *pi);

static uint32  Application_Release ( IApplication *pi);

static boolean Application_HandleEvent( IApplication *pi,
    AEEEvent eCode,
    uint16  wParam,
    uint32 dwParam
);

static int     Application_MainMenuService( IApplication *pi, ApplicationServiceType eStype);

static Application gApplication={0};

static const VTBL( IApplication) gApplicationMethods =
{
    Application_AddRef,
    Application_Release,
    Application_HandleEvent,
    Application_MainMenuService
};

int ApplicationMod_Load( IShell *pIShell, void *ph, IModule **ppMod);

int ApplicationMod_New( int16 nSize,
    IShell              *pIShell,
    void                *ph,
    IModule             **ppMod,
    PFNMODCREATEINST    pfnMC,
    PFNFREEMODDATA      pfnMF
);

static int Application_New( IShell *ps, IModule *pIModule, IApplication **ppObj);

static int CApplication_InitAppData(Application *pMe);

static void CApplication_FreeAppData(Application *pMe);

static void Application_RunFSM(Application *pMe);

//static void calculateScreenParameters(MainMenu *pMe);

static boolean StartApplet(Application *pMe, int i);

boolean Application_RouteDialogEvt(Application *pMe,
    AEEEvent    eCode,
    uint16  wParam,
    uint32 dwParam
);

NextFSMAction Application_ProcessState(Application *pMe);

void Application_ShowDialog(Application  *pMe,  uint16 dlgResId);

// APPLICATIONST_MAIN ״̬������
static NextFSMAction APPLICATIONST_MAIN_Handler(Application *pMe);


//APPLICATIONST_EXIT  ״̬������
static NextFSMAction APPLICATIONST_EXIT_Handler(Application *pMe);
static boolean  Application_ListMenuHandler(Application *pMe, AEEEvent eCode, uint16 wParam, uint32 dwParam);



/*==============================================================================
                              
                              ���أ���̬������
                              
==============================================================================*/



//wlh 20090410 add  end �ƶ�Ч��
/*=============================================================================
FUNCTION:  ApplicationMod_Load

DESCRIPTION: 

PARAMETERS:

RETURN VALUE:

COMMENTS:

SIDE EFFECTS:

SEE ALSO:

=============================================================================*/
int ApplicationMod_Load( IShell *pIShell, void *ph, IModule **ppMod)
{
    return ApplicationMod_New( sizeof( ApplicationMod),
                    pIShell,
                    ph,
                    ppMod,
                    NULL,
                    NULL);
}

/*=============================================================================
FUNCTION:  ApplicationMod_New

DESCRIPTION: 

PARAMETERS:

RETURN VALUE:

COMMENTS:

SIDE EFFECTS:

SEE ALSO:

=============================================================================*/
int  ApplicationMod_New( int16 nSize,
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

    MEMSET( &gApplicationMod, 0, sizeof( ApplicationMod));

    INIT_VTBL( &gApplicationMod, IModule, gModFuncs);
    gApplicationMod.referenceCounter = 1;
    *ppMod = ( IModule *)&gApplicationMod;
    return AEE_SUCCESS;
}

/*=============================================================================
FUNCTION:  ApplicationMod_AddRef

DESCRIPTION: 

PARAMETERS:

RETURN VALUE:

COMMENTS:

SIDE EFFECTS:

SEE ALSO:

=============================================================================*/
static uint32 ApplicationMod_AddRef( IModule *po)
{
    return( ++( ( ApplicationMod *)po)->referenceCounter);
}

/*=============================================================================
FUNCTION:  MainMenuMod_Release

DESCRIPTION: 

PARAMETERS:

RETURN VALUE:

COMMENTS:

SIDE EFFECTS:

SEE ALSO:

=============================================================================*/
static uint32 ApplicationMod_Release( IModule *po)
{
    if ( ( ( ApplicationMod *) po)->referenceCounter == 0)
    {
        return 0;
    }

    return( --( ( ApplicationMod *)po)->referenceCounter);
}

/*=============================================================================
FUNCTION:  ApplicationMod_CreateInstance

DESCRIPTION: 

PARAMETERS:

RETURN VALUE:

COMMENTS:

SIDE EFFECTS:

SEE ALSO:

=============================================================================*/
static int  ApplicationMod_CreateInstance( IModule *po,
    IShell *pIShell,
    AEECLSID ClsId,
    void **ppObj
)
{
    *ppObj = NULL;

    if( ClsId != AEECLSID_APPLICATION || Application_New( pIShell, po, ( IApplication**)ppObj) != SUCCESS)
    {
        return EFAILED;
    }

    return SUCCESS;
}

/*=============================================================================
FUNCTION:  ApplicationMod_FreeResources

DESCRIPTION: 

PARAMETERS:

RETURN VALUE:

COMMENTS:

SIDE EFFECTS:

SEE ALSO:

=============================================================================*/
static void ApplicationMod_FreeResources( IModule *po, IHeap *ph, IFileMgr *pfm)
{
}

/*=============================================================================
FUNCTION:  Application_New

DESCRIPTION: 

PARAMETERS:

RETURN VALUE:

COMMENTS:

SIDE EFFECTS:

SEE ALSO:

=============================================================================*/
static int Application_New( IShell *ps, IModule *pIModule, IApplication **ppObj)
{
    int retVal = SUCCESS;
    if( 0 == gApplication.referenceCounter)
    {

        if( pIModule == NULL || ps == NULL)
        {
            return EFAILED;
        }

        MEMSET(&gApplication, 0, sizeof(gApplication));
        INIT_VTBL(&gApplication, IApplication, gApplicationMethods);

       
        gApplication.m_pShell     = ps;
        gApplication.m_pModule    = pIModule;
        retVal = CApplication_InitAppData(&gApplication);
        if(retVal != SUCCESS)
        {
            CApplication_FreeAppData((Application*)&gApplication);
            return retVal;
        }
        (void) ISHELL_AddRef( ps);
        (void) IMODULE_AddRef( pIModule);
        gApplication.referenceCounter  = 0;

        
    }

    ++ gApplication.referenceCounter;
    *ppObj = ( IApplication*)&gApplication;
    
    return retVal;
}

/*==============================================================================
����:
    CApplicationApp_InitAppData

˵��:
    ��ʼ�� Application Applet �ṹ����ȱʡֵ��

����:
    pMe [in]: ָ�� Application Applet ����ṹ��ָ�롣�ýṹ����С������ض���Ϣ��

����ֵ:
    EFAILED: ��ʼ��ʧ�ܡ�
    SUCCESS: ��ʼ���ɹ���

��ע:

==============================================================================*/
static int CApplication_InitAppData(Application *pMe)
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
			CApplication_FreeAppData(pMe);
			return FALSE;
	}
    return SUCCESS;
}

/*==============================================================================
����:
    CApplication_FreeAppData

˵��:
    �ͷ� Application Applet �����Դ��

����:
    pMe [in]: ָ��Application Applet ����ṹ��ָ�롣�ýṹ����С������ض���Ϣ��

����ֵ:
    none

��ע:

==============================================================================*/
static void CApplication_FreeAppData(Application *pMe)
{
    if (NULL == pMe)
    {
        return;
    }

    pMe->m_eAppStatus = APPLICATION_STOP; 
    
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
FUNCTION:  Application_AddRef

DESCRIPTION: 

PARAMETERS:

RETURN VALUE:

COMMENTS:

SIDE EFFECTS:

SEE ALSO:

=============================================================================*/
static uint32  Application_AddRef( IApplication *pi)
{
    return ( ++gApplication.referenceCounter);
}

/*=============================================================================
FUNCTION:  Application_Release

DESCRIPTION: 

PARAMETERS:

RETURN VALUE:

COMMENTS:

SIDE EFFECTS:

SEE ALSO:

=============================================================================*/
static uint32  Application_Release( IApplication *pi)
{
    register Application *pMe = (Application*)pi;

    if (pMe->referenceCounter == 0)
    {
        return 0;
    }
    
    if( --pMe->referenceCounter)
    {
        return pMe->referenceCounter;
    }


    CApplication_FreeAppData(pMe);

    (void) ISHELL_Release(pMe->m_pShell);
    (void) IMODULE_Release(pMe->m_pModule);

    return 0;
}

/*==============================================================================
����:
    Application_RunFSM

˵��:
    ��������״̬�����档

����:
    pMe [in]: ָ��Application Applet����ṹ��ָ�롣�ýṹ����С������ض���Ϣ��
       
����ֵ:
    �ޡ�

��ע:

==============================================================================*/
static void Application_RunFSM(Application *pMe)
{
    NextFSMAction nextFSMAction = NFSMACTION_WAIT;
    for ( ; ; )
    {
        nextFSMAction = Application_ProcessState(pMe);

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
FUNCTION:  MainMenu_HandleEvent

DESCRIPTION: 

PARAMETERS:

RETURN VALUE:

COMMENTS:

SIDE EFFECTS:

SEE ALSO:

=============================================================================*/
static boolean Application_HandleEvent( IApplication *pi,
    AEEEvent eCode,
    uint16  wParam,
    uint32 dwParam
)
{
    Application *pMe = (Application*)pi;
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
            pMe->m_currState  = APPLICATIONST_MAIN;
            pMe->m_eDlgReturn = DLGRET_CREATE;
            pMe->m_eAppStatus = APPLICATION_RUNNING;
            if(pMe->m_pIAnn != NULL)
            {
                IANNUNCIATOR_SetFieldIsActiveEx(pMe->m_pIAnn,FALSE);
            }
            Application_RunFSM(pMe);
            return TRUE;

        case EVT_APP_STOP:
            {
                if (pMe->m_pDisplay != NULL)
                {
                    (void) IDISPLAY_Release(pMe->m_pDisplay);
                    pMe->m_pDisplay = NULL;
                }
                pMe->m_eAppStatus = APPLICATION_STOP;
                
                return TRUE;
            }

        case EVT_APP_SUSPEND:
            pMe->m_eAppStatus = APPLICATION_SUSPEND;
            return TRUE;
        

        case EVT_APP_RESUME:
            {
                AEEAppStart* as = ( AEEAppStart*)dwParam;
                pMe->m_rc    = as->rc;
                pMe->m_rc.dy = di.cyScreen;
                pMe->m_eAppStatus = APPLICATION_RUNNING;
                Application_RunFSM(pMe); 
                return TRUE;
            }

        case EVT_DIALOG_INIT:
            
            pMe->m_pActiveIDlg = (IDialog*)dwParam;
            pMe->m_pActivedlgID = wParam;
            
            return Application_RouteDialogEvt(pMe,eCode,wParam,dwParam);

        case EVT_DIALOG_START:

            return Application_RouteDialogEvt(pMe,eCode,wParam,dwParam);

        case EVT_USER_REDRAW:
            return Application_RouteDialogEvt(pMe,eCode,wParam,dwParam);

        case EVT_DIALOG_END:
		  
            (void) Application_RouteDialogEvt(pMe,eCode,wParam,dwParam);
            pMe->m_pActiveIDlg = NULL;
            pMe->m_pActivedlgID = 0;
            if(pMe->m_eAppStatus == APPLICATION_RUNNING)
            {
                Application_RunFSM(pMe);
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
            return Application_RouteDialogEvt(pMe,eCode,wParam,dwParam);   

        case EVT_KEY_PRESS:
        case EVT_KEY_RELEASE:
        case EVT_COMMAND:
            return Application_RouteDialogEvt(pMe,eCode,wParam,dwParam); 
        default:
            return Application_RouteDialogEvt(pMe,eCode,wParam,dwParam);
    }
}

/*==============================================================================
����:
   Application_ProcessState

˵��:
    ���� pMe->m_currState, ������Ӧ��״̬��������

����:
    pMe [in]: ָ��Application Applet����ṹ��ָ�롣�ýṹ����С������ض���Ϣ��

����ֵ:
    NFSMACTION_CONTINUE: ָʾ��ͣ״̬����
    NFSMACTION_WAIT: ָʾֹͣ״̬����

��ע:

==============================================================================*/
NextFSMAction Application_ProcessState(Application *pMe)
{
    NextFSMAction retVal = NFSMACTION_WAIT;
    
    if (NULL == pMe)
    {
        return retVal;
    }
    
    switch(pMe->m_currState)
    {
        case APPLICATIONST_MAIN:
            return APPLICATIONST_MAIN_Handler(pMe);

        case APPLICATIONST_EXIT:
            return APPLICATIONST_EXIT_Handler(pMe);            

        default:
            break;

    }
    return retVal;
}

/*==============================================================================
����:
    APPLICATIONST_MAIN_Handler

˵��:
    APPLICATIONST_MAIN ״̬��������

����:
    pMe [in]: ָ��APPLICATION Applet����ṹ��ָ�롣�ýṹ����С������ض���Ϣ��

����ֵ:
    NFSMACTION_CONTINUE: ָʾ��ͣ״̬����
    NFSMACTION_WAIT: ָʾֹͣ״̬����

��ע:

==============================================================================*/
static NextFSMAction APPLICATIONST_MAIN_Handler(Application *pMe)
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
                  Application_ShowDialog(pMe, IDD_LIST_APPLICATION);

            }
            return NFSMACTION_WAIT;
        default:
            MOVE_TO_STATE(APPLICATIONST_EXIT)
            return NFSMACTION_CONTINUE;
    }
}




/*==============================================================================
����:
    APPLICATIONST_EXIT_Handler

˵��:
    APPLICATIONST_EXIT ״̬��������

����:
    pMe [in]: ָ��APPLICATION  Applet����ṹ��ָ�롣�ýṹ����С������ض���Ϣ��

����ֵ:
    NFSMACTION_CONTINUE: ָʾ��ͣ״̬����
    NFSMACTION_WAIT: ָʾֹͣ״̬����

��ע:

==============================================================================*/
static NextFSMAction APPLICATIONST_EXIT_Handler(Application *pMe)
{
    (void) ISHELL_CloseApplet(pMe->m_pShell, FALSE);
    return NFSMACTION_WAIT;
} 


/*==============================================================================
����:
       Application_ShowDialog

˵��:
    ������״̬����������������Ӧ�Ի�����ԴID�ĶԻ���

����:
    pMe [in]: ָ�� Application Applet ����ṹ��ָ�롣�ýṹ����С������ض���Ϣ��
    dlgResId [in]: �Ի�����ԴID���������ݸ�ID�����Ի���

����ֵ:
    none

��ע:

==============================================================================*/
void Application_ShowDialog(Application  *pMe,  uint16 dlgResId)
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
    nRet = ISHELL_CreateDialog(pMe->m_pShell,APPLICATION_RES_FILE_LANG,dlgResId,NULL);
    if (nRet != SUCCESS)
    {
        DBGPRINTF("Failed to create the dialog %d in the MAINMENU applet.",dlgResId,0,0);
    }
}


/*==============================================================================
����:
      Application_RouteDialogEvt
˵��:
       ������BREW�¼�·�ɸ���ǰ��Ի�����¼���������

����:
       pMe [In]: ָ��SettingMenu Applet����ṹ��ָ��,�ýṹ����С������ض���Ϣ.
       eCode [in]���¼����롣
       wParam [in]�����¼�eCode���������ݡ�
       dwParam [in]�����¼�eCode���������ݡ�

����ֵ:
       TRUE�������¼�������
       FALSE�������¼�û������

��ע:

==============================================================================*/
boolean Application_RouteDialogEvt(Application *pMe,
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

        case IDD_LIST_APPLICATION:
            return Application_ListMenuHandler(pMe, eCode, wParam, dwParam);

        default:
            return FALSE;
    }
}



/*=============================================================================
FUNCTION:  Application_ListMenuHandler

DESCRIPTION:   �б�˵�

PARAMETERS:

=============================================================================*/

static boolean Application_ListMenuHandler(Application *pMe, AEEEvent eCode, uint16 wParam, uint32 dwParam)
{
    PARAM_NOT_REF(dwParam)
    IMenuCtl *pMenu = (IMenuCtl*)IDIALOG_GetControl(pMe->m_pActiveIDlg,IDC_APPLICATION_LIST);
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
			#if 0
            IMENUCTL_SetTitle(pMenu, APPLICATION_RES_FILE_LANG, IDS_APPLICATION_LIST, NULL);
			#else
			#ifdef FEATURE_VERSION_IVIO
			(void)ISHELL_LoadResString(pMe->m_pShell,
                                    APPLICATION_RES_FILE_LANG,                                
                                    IDS_APPLICATION_LIST,
                                    WTitle,
                                    sizeof(WTitle));
			#else
			(void)ISHELL_LoadResString(pMe->m_pShell,
                                    APPLICATION_RES_FILE_LANG,                                
                                    IDS_TOOLS_LIST,
                                    WTitle,
                                    sizeof(WTitle));
			#endif
            if(pMe->m_pIAnn != NULL)
            {
			    IANNUNCIATOR_SetFieldText(pMe->m_pIAnn,WTitle);
            }
			#endif
			#ifdef FEATURE_VERSION_IVIO
			IMENUCTL_AddItem(pMenu, APPLICATION_RES_FILE_LANG,IDS_TOOLS_TITLE_1, IDS_TOOLS_TITLE_1, NULL, 0);
            IMENUCTL_AddItem(pMenu, APPLICATION_RES_FILE_LANG,IDS_TOOLS_TITLE_2, IDS_TOOLS_TITLE_2, NULL, 0);
            IMENUCTL_AddItem(pMenu, APPLICATION_RES_FILE_LANG,IDS_TOOLS_TITLE_3, IDS_TOOLS_TITLE_3, NULL, 0);
            IMENUCTL_AddItem(pMenu, APPLICATION_RES_FILE_LANG,IDS_TOOLS_TITLE_4, IDS_TOOLS_TITLE_4, NULL, 0);
            IMENUCTL_AddItem(pMenu, APPLICATION_RES_FILE_LANG,IDS_TOOLS_TITLE_5, IDS_TOOLS_TITLE_5, NULL, 0);
            IMENUCTL_AddItem(pMenu, APPLICATION_RES_FILE_LANG,IDS_TOOLS_TITLE_6, IDS_TOOLS_TITLE_6, NULL, 0);
			#else   
            IMENUCTL_AddItem(pMenu, APPLICATION_RES_FILE_LANG,IDS_APPLICATION_TITLE_1, IDS_APPLICATION_TITLE_1, NULL, 0);
            IMENUCTL_AddItem(pMenu, APPLICATION_RES_FILE_LANG,IDS_APPLICATION_TITLE_2, IDS_APPLICATION_TITLE_2, NULL, 0);
            IMENUCTL_AddItem(pMenu, APPLICATION_RES_FILE_LANG,IDS_APPLICATION_TITLE_3, IDS_APPLICATION_TITLE_3, NULL, 0);
            IMENUCTL_AddItem(pMenu, APPLICATION_RES_FILE_LANG,IDS_APPLICATION_TITLE_4, IDS_APPLICATION_TITLE_4, NULL, 0);
            IMENUCTL_AddItem(pMenu, APPLICATION_RES_FILE_LANG,IDS_APPLICATION_TITLE_5, IDS_APPLICATION_TITLE_5, NULL, 0);
            IMENUCTL_AddItem(pMenu, APPLICATION_RES_FILE_LANG,IDS_APPLICATION_TITLE_6, IDS_APPLICATION_TITLE_6, NULL, 0);
			IMENUCTL_AddItem(pMenu, APPLICATION_RES_FILE_LANG,IDS_APPLICATION_TITLE_7, IDS_APPLICATION_TITLE_7, NULL, 0);
            #if defined (FEATURE_DISP_128X128)
            IMENUCTL_AddItem(pMenu, APPLICATION_RES_FILE_LANG,IDS_APPLICATION_TITLE_8, IDS_APPLICATION_TITLE_8, NULL, 0);
            IMENUCTL_AddItem(pMenu, APPLICATION_RES_FILE_LANG,IDS_APPLICATION_TITLE_9, IDS_APPLICATION_TITLE_9, NULL, 0);
            IMENUCTL_AddItem(pMenu, APPLICATION_RES_FILE_LANG,IDS_APPLICATION_TITLE_10, IDS_APPLICATION_TITLE_10, NULL, 0);
            #endif
			#endif

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
                          APPLICATION_RES_FILE_LANG,
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
                case AVK_2:
                case AVK_3:
                case AVK_4:
                case AVK_5:
                case AVK_6:
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
			#ifdef FEATURE_VERSION_IVIO
			StartApplet(pMe, wParam - IDS_TOOLS_TITLE_1);
			#else
            StartApplet(pMe, wParam - IDS_APPLICATION_TITLE_1);
			#endif
            return TRUE;
            
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
static boolean StartApplet(Application *pMe, int i)
{
    int Result = FALSE;

   	//MSG_ERROR("StartApplet:::::%d",i,0,0);
    switch(i)
    {
        #ifdef FEATURE_VERSION_IVIO
        case 0:
            Result = ISHELL_StartApplet(pMe->m_pShell, AEECLSID_ALARMCLOCK);
            break;
            
        case 1:
            Result = ISHELL_StartApplet(pMe->m_pShell, AEECLSID_CALCAPP);
            break;
    
        case 2:
            Result = ISHELL_StartApplet(pMe->m_pShell, AEECLSID_CONVERTER);
            break;
    
        case 3:
            Result = ISHELL_StartApplet(pMe->m_pShell, AEECLSID_STOPWATCH);
            break;
            
        case 4:
            Result = ISHELL_StartApplet(pMe->m_pShell, AEECLSID_APPTIMER);
            break;
            
        case 5:
            Result = ISHELL_StartApplet(pMe->m_pShell, AEECLSID_WORLDTIME);
            break;
		#else
		case 0:
            Result = ISHELL_StartApplet(pMe->m_pShell, AEECLSID_APPMANAGER);
            break;
            
        case 1:
            Result = ISHELL_StartApplet(pMe->m_pShell, AEECLSID_APPTIMER);
            break;
    
        case 2:
            {
                Result = ISHELL_StartApplet(pMe->m_pShell, AEECLSID_STOPWATCH);
            }
            break;
    
        case 3:
            Result = ISHELL_StartApplet(pMe->m_pShell, AEECLSID_ALARMCLOCK);
            break;
        case 4:
            Result = ISHELL_StartApplet(pMe->m_pShell, AEECLSID_CONVERTER);
            break;
        case 5:
            Result = ISHELL_StartApplet(pMe->m_pShell, AEECLSID_WORLDTIME);
            break;
		case 6:
            Result = ISHELL_StartApplet(pMe->m_pShell, AEECLSID_RECORDER);
            break;
        case 7:
            Result = ISHELL_StartApplet(pMe->m_pShell, AEECLSID_SCHEDULEAPP);
            break;
        case 8:
            Result = ISHELL_StartApplet(pMe->m_pShell, AEECLSID_GAME);
            break;
        case 9:
            Result = ISHELL_StartApplet(pMe->m_pShell, AEECLSID_CALCAPP);
            break;
		#endif
            
        default:
            break;
    }
    return TRUE;
}

/*=============================================================================
FUNCTION:  Application_MainMenuService

DESCRIPTION:  �ⲿ�ӿڵ��ã����ٽ��뵽applicationĳһ���˵��б�

PARAMETERS:    
            eStype: ����app�ķ�ʽ

=============================================================================*/

static int  Application_MainMenuService( IApplication *pi, ApplicationServiceType eStype)
{
    char  *args = NULL;
    Application *pMe = (Application*)pi;
    int    nRet;  

    if (ISHELL_ActiveApplet(pMe->m_pShell) == AEECLSID_APPLICATION)
    {
        // applet is already running
        return EFAILED;
    }    
    
    switch (eStype)
    {
        case STARTMETHOD_PLAYER: 
            args = (char *)MALLOC(sizeof(char));
            if(args)
            {
                args[0] = STARTARGPREFIX_VIEWPLAYER;
            }
            break;
        
        case STARTMETHOD_DATA:
            args = (char *)MALLOC(sizeof(char));
            if(args)
            {
                args[0] = STARTARGPREFIX_VIEWDATA;
            }
            break;

        default:
            break;
    }

    if(args == NULL)
    {
        return EFAILED;
    }

    nRet = ISHELL_StartAppletArgs(pMe->m_pShell, AEECLSID_APPLICATION, args);
        
    FREEIF(args);
    args = NULL;
    return nRet;    
}

