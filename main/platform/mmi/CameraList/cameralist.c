/** ----------------------------------------------------------------------------
 *Cameralist.c
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
#ifndef AEECLSID_CAMERA_LIST
    #error AEECLSID_CAMERA_LIST must be defined
#endif
#include "AppComFunc.h"
#include "AEEShell.h"
#include "AEE_OEM.h"
#include "AEEModGen.h"
#include "AEEModTable.h"
#include "AEEMenu.h"
#include "AEEStdLib.h"
#include "AEEAnnunciator.h"
#include "cameralist.h"
#include "cameralist_priv.h" 

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
static CameralistMod gCameralistMod;
static const VTBL( IModule) gModFuncs =
{
    CameralistMod_AddRef,
    CameralistMod_Release,
    CameralistMod_CreateInstance,
    CameralistMod_FreeResources
};

static uint32  Cameralist_AddRef( ICameralist *pi);

static uint32  Cameralist_Release ( ICameralist *pi);

static boolean Cameralist_HandleEvent( ICameralist *pi,
    AEEEvent eCode,
    uint16  wParam,
    uint32 dwParam
);




static Cameralist gCameralist={0};

static const VTBL( ICameralist) gCameralistMethods =
{
    Cameralist_AddRef,
    Cameralist_Release,
    Cameralist_HandleEvent,
};

int CameralistMod_Load( IShell *pIShell, void *ph, IModule **ppMod);

int CameralistMod_New( int16 nSize,
    IShell              *pIShell,
    void                *ph,
    IModule             **ppMod,
    PFNMODCREATEINST    pfnMC,
    PFNFREEMODDATA      pfnMF
);

static int Cameralist_New( IShell *ps, IModule *pIModule, ICameralist **ppObj);

static int Cameralist_InitAppData(Cameralist *pMe);

static void Cameralist_FreeAppData(Cameralist *pMe);

static void Cameralist_RunFSM(Cameralist *pMe);

static int StartApplet(Cameralist *pMe, int i);

boolean Cameralist_RouteDialogEvt(Cameralist *pMe,
    AEEEvent    eCode,
    uint16  wParam,
    uint32 dwParam
);

NextFSMAction Cameralist_ProcessState(Cameralist *pMe);

void Cameralist_ShowDialog(Cameralist  *pMe,  uint16 dlgResId);

// MAINST_MAIN ״̬������
static NextFSMAction MAINST_CAMERALIST_Handler(Cameralist *pMe);

//MAINST_EXIT  ״̬������
static NextFSMAction CAMERALISTST_EXIT_Handler(Cameralist *pMe);
static boolean Cameralist_ListMenuHandler(Cameralist *pMe, AEEEvent eCode, uint16 wParam, uint32 dwParam);



/*==============================================================================
                              
                              ���أ���̬������
                              
==============================================================================*/




/*=============================================================================
FUNCTION:  CameralistMod_Load

DESCRIPTION: 

PARAMETERS:

RETURN VALUE:

COMMENTS:

SIDE EFFECTS:

SEE ALSO:

=============================================================================*/
int CameralistMod_Load( IShell *pIShell, void *ph, IModule **ppMod)
{
    MSG_FATAL("***zzg CameralistMod_Load***", 0, 0, 0);
    return CameralistMod_New( sizeof(CameralistMod),
                    pIShell,
                    ph,
                    ppMod,
                    NULL,
                    NULL);
}

/*=============================================================================
FUNCTION:  CameralistMod_New

DESCRIPTION: 

PARAMETERS:

RETURN VALUE:

COMMENTS:

SIDE EFFECTS:

SEE ALSO:

=============================================================================*/
int  CameralistMod_New( int16 nSize,
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

    MSG_FATAL("***zzg CameralistMod_New***", 0, 0, 0);

    if( NULL == ppMod)
    {
        return EFAILED;
    }

    MEMSET( &gCameralistMod, 0, sizeof(CameralistMod));

    INIT_VTBL( &gCameralistMod, IModule, gModFuncs);
    gCameralistMod.referenceCounter = 1;
    *ppMod = ( IModule *)&gCameralistMod;
    return AEE_SUCCESS;
}

/*=============================================================================
FUNCTION:  CameralistMod_AddRef

DESCRIPTION: 

PARAMETERS:

RETURN VALUE:

COMMENTS:

SIDE EFFECTS:

SEE ALSO:

=============================================================================*/
static uint32 CameralistMod_AddRef( IModule *po)
{
    return( ++( ( CameralistMod *)po)->referenceCounter);
}

/*=============================================================================
FUNCTION:  CameralistMod_Release

DESCRIPTION: 

PARAMETERS:

RETURN VALUE:

COMMENTS:

SIDE EFFECTS:

SEE ALSO:

=============================================================================*/
static uint32 CameralistMod_Release( IModule *po)
{
    if ( ( ( CameralistMod *) po)->referenceCounter == 0)
    {
        
        return 0;
    }
    return( --( ( CameralistMod *)po)->referenceCounter);
}

/*=============================================================================
FUNCTION:  Cameralist_CreateInstance

DESCRIPTION: 

PARAMETERS:

RETURN VALUE:

COMMENTS:

SIDE EFFECTS:

SEE ALSO:

=============================================================================*/
static int  CameralistMod_CreateInstance( IModule *po,
    IShell *pIShell,
    AEECLSID ClsId,
    void **ppObj
)
{
    MSG_FATAL("***zzg CameralistMod_CreateInstance Start***",0,0,0);
    
    *ppObj = NULL;

    if( ClsId != AEECLSID_CAMERA_LIST || Cameralist_New( pIShell, po, ( ICameralist**)ppObj) != SUCCESS)
    {
        return EFAILED;
    }
    MSG_FATAL("CameralistMod_CreateInstance End",0,0,0);
    return SUCCESS;
}

/*=============================================================================
FUNCTION:  Cameralist_FreeResources

DESCRIPTION: 

PARAMETERS:

RETURN VALUE:

COMMENTS:

SIDE EFFECTS:

SEE ALSO:

=============================================================================*/
static void CameralistMod_FreeResources( IModule *po, IHeap *ph, IFileMgr *pfm)
{
}

/*=============================================================================
FUNCTION:  Cameralist_New

DESCRIPTION: 

PARAMETERS:

RETURN VALUE:

COMMENTS:

SIDE EFFECTS:

SEE ALSO:

=============================================================================*/
static int Cameralist_New( IShell *ps, IModule *pIModule, ICameralist **ppObj)
{
    int retVal = SUCCESS;
    MSG_FATAL("***zzg Cameralist_New Start***",0,0,0);
    if( 0 == gCameralist.referenceCounter)
    {

        if( pIModule == NULL || ps == NULL)
        {
            return EFAILED;
        }

        MEMSET(&gCameralist, 0, sizeof(gCameralist));
        INIT_VTBL(&gCameralist, ICameralist, gCameralistMethods);

       
        gCameralist.m_pShell     = ps;
        gCameralist.m_pModule    = pIModule;
        retVal = Cameralist_InitAppData(&gCameralist);
        MSG_FATAL("Cameralist_New retVal=%d",retVal,0,0);
        if(retVal != SUCCESS)
        {
            Cameralist_FreeAppData((Cameralist*)&gCameralist);
            return retVal;
        }
        (void) ISHELL_AddRef( ps);
        (void) IMODULE_AddRef( pIModule);
        gCameralist.referenceCounter  = 0;

        
    }

    ++ gCameralist.referenceCounter;
    *ppObj = ( ICameralist*)&gCameralist;
    MSG_FATAL("Cameralist_New retVal=%d",retVal,0,0);
    return retVal;
}

/*==============================================================================
����:
    Cameralist_InitAppData

˵��:
    ��ʼ�� Cameralist Applet �ṹ����ȱʡֵ��

����:
    pMe [in]: ָ�� Cameralist Applet ����ṹ��ָ�롣�ýṹ����С������ض���Ϣ��

����ֵ:
    EFAILED: ��ʼ��ʧ�ܡ�
    SUCCESS: ��ʼ���ɹ���

��ע:

==============================================================================*/
static int Cameralist_InitAppData(Cameralist *pMe)
{
    if (NULL == pMe)
    {
        return EFAILED;
    }
	
	
    pMe->m_MainSel  = 0;
    
    if (ISHELL_CreateInstance(pMe->m_pShell, AEECLSID_DISPLAY, 
            (void **) &pMe->m_pDisplay) != SUCCESS)
    {
        MSG_FATAL("Cameralist_InitAppData EFAILED",0,0,0);
        return EFAILED;
    }
    
	if (AEE_SUCCESS != ISHELL_CreateInstance(pMe->m_pShell,AEECLSID_ANNUNCIATOR,(void **)&pMe->m_pIAnn))
    {
        MSG_FATAL("Cameralist_InitAppData EFAILED",0,0,0);
        return EFAILED;
    }
    return SUCCESS;
}

/*==============================================================================
����:
    CameralistFreeAppData

˵��:
    �ͷ� Cameralist Applet �����Դ��

����:
    pMe [in]: Cameralist Applet ����ṹ��ָ�롣�ýṹ����С������ض���Ϣ��

����ֵ:
    none

��ע:

==============================================================================*/
static void Cameralist_FreeAppData(Cameralist *pMe)
{
    if (NULL == pMe)
    {
        return;
    }

    pMe->m_eAppStatus = CAMERALIST_STOP; 
    
    if (pMe->m_pDisplay != NULL)
    {
        (void) IDISPLAY_Release(pMe->m_pDisplay);
        pMe->m_pDisplay = NULL;
    }
	if(pMe->m_pIAnn)
    {
        IANNUNCIATOR_Release(pMe->m_pIAnn);
        pMe->m_pIAnn = NULL;
    }

}

/*=============================================================================
FUNCTION:  Cameralist_AddRef

DESCRIPTION: 

PARAMETERS:

RETURN VALUE:

COMMENTS:

SIDE EFFECTS:

SEE ALSO:

=============================================================================*/
static uint32  Cameralist_AddRef( ICameralist *pi)
{
    return ( ++gCameralist.referenceCounter);
}

/*=============================================================================
FUNCTION:  Cameralist_Release

DESCRIPTION: 

PARAMETERS:

RETURN VALUE:

COMMENTS:

SIDE EFFECTS:

SEE ALSO:

=============================================================================*/
static uint32  Cameralist_Release( ICameralist *pi)
{
    register Cameralist *pMe = (Cameralist*)pi;
   
    if (pMe->referenceCounter == 0)
    {
        return 0;
    }
    
    if( --pMe->referenceCounter)
    {
        return pMe->referenceCounter;
    }


    Cameralist_FreeAppData(pMe);

    (void) ISHELL_Release(pMe->m_pShell);
    (void) IMODULE_Release(pMe->m_pModule);
   
    return 0;
}

/*==============================================================================
����:
    Cameralist_RunFSM

˵��:
    ��������״̬�����档

����:
    pMe [in]: ָ��Application Applet����ṹ��ָ�롣�ýṹ����С������ض���Ϣ��
       
����ֵ:
    �ޡ�

��ע:

==============================================================================*/
static void Cameralist_RunFSM(Cameralist *pMe)
{
    NextFSMAction nextFSMAction = NFSMACTION_WAIT;
    for ( ; ; )
    {
        nextFSMAction = Cameralist_ProcessState(pMe);

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
/*==============================================================================
����:
     Cameralist_RouteDialogEvt
˵��:
       ������BREW�¼�·�ɸ���ǰ��Ի�����¼���������

����:
       pMe [In]: ָ��Cameralist Applet����ṹ��ָ��,�ýṹ����С������ض���Ϣ.
       eCode [in]���¼����롣
       wParam [in]�����¼�eCode���������ݡ�
       dwParam [in]�����¼�eCode���������ݡ�

����ֵ:
       TRUE�������¼�������
       FALSE�������¼�û������

��ע:

==============================================================================*/
boolean Cameralist_RouteDialogEvt(Cameralist *pMe,
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

        case IDD_CAMERALIST_DIALOGS:
            return Cameralist_ListMenuHandler(pMe, eCode, wParam, dwParam);        
default:
            return FALSE;
    }
}


/*=============================================================================
FUNCTION:  Cameralist_HandleEvent

DESCRIPTION: 

PARAMETERS:

RETURN VALUE:

COMMENTS:

SIDE EFFECTS:

SEE ALSO:

=============================================================================*/
static boolean Cameralist_HandleEvent( ICameralist *pi,
    AEEEvent eCode,
    uint16  wParam,
    uint32 dwParam
)
{
    Cameralist *pMe = (Cameralist*)pi;
    AEEAppStart* as = NULL;
    AEEDeviceInfo di; 
    
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
            pMe->m_currState  = CAMERALIST_MAIN;
            pMe->m_eDlgReturn = DLGRET_CREATE;
            pMe->m_eAppStatus = CAMERALIST_RUNNING;
            if(pMe->m_pIAnn != NULL)
            {
                IANNUNCIATOR_SetFieldIsActiveEx(pMe->m_pIAnn,FALSE); 
            }
            Cameralist_RunFSM(pMe);
            return TRUE;

        case EVT_APP_STOP:
            {
                if (pMe->m_pDisplay != NULL)
                {
                    (void) IDISPLAY_Release(pMe->m_pDisplay);
                    pMe->m_pDisplay = NULL;
                }
                pMe->m_eAppStatus = CAMERALIST_STOP;
                
                return TRUE;
            }

        case EVT_APP_SUSPEND:
          
            pMe->m_eAppStatus = CAMERALIST_SUSPEND;
            
            return TRUE;
        

        case EVT_APP_RESUME:
            {
                AEEAppStart* as = ( AEEAppStart*)dwParam;
                
                ISHELL_GetDeviceInfo(pMe->m_pShell,&di); 
                pMe->m_rc    = as->rc;
                pMe->m_rc.dy = di.cyScreen;
                pMe->m_eAppStatus = CAMERALIST_RUNNING;
                Cameralist_RunFSM(pMe); 
                return TRUE;
            }

        case EVT_DIALOG_INIT:
           
            pMe->m_pActiveIDlg = (IDialog*)dwParam;
            pMe->m_pActivedlgID = wParam;
            
            return Cameralist_RouteDialogEvt(pMe, eCode, wParam, dwParam);

        case EVT_DIALOG_START:
            
            return Cameralist_RouteDialogEvt(pMe, eCode, wParam, dwParam);

        case EVT_USER_REDRAW:
            return Cameralist_RouteDialogEvt(pMe, eCode, wParam, dwParam);

        case EVT_DIALOG_END:
		 
            (void)Cameralist_RouteDialogEvt(pMe, eCode, wParam, dwParam);
            pMe->m_pActiveIDlg = NULL;
            pMe->m_pActivedlgID = 0;
            if(pMe->m_eAppStatus == CAMERALIST_RUNNING)
            {
                Cameralist_RunFSM(pMe);
            }
            return TRUE;
#ifdef FEATURE_LCD_TOUCH_ENABLE
		case EVT_USER:
			{
				if((wParam == AVK_SELECT) || (wParam == AVK_INFO))
				{
					if(dwParam != 0)
					{
						eCode = EVT_COMMAND;
						wParam = dwParam;
						dwParam = 0;
					}
					else
					{
						eCode = EVT_KEY;
					}
				}
				else if(wParam == AVK_CLR)
				{
					eCode = EVT_KEY;
				}
				else if(wParam == AVK_DOWN)//wlh ������ͼƬ
				{
					eCode = EVT_KEY;
				}
				else if (wParam == AVK_POUND)
				{
					eCode = EVT_KEY_PRESS;
				}
				return Cameralist_RouteDialogEvt(pMe,eCode,wParam,dwParam);
			}
#endif

			
        case EVT_PEN_UP:
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
            return Cameralist_RouteDialogEvt(pMe, eCode, wParam, dwParam);

        case EVT_KEY_PRESS:
        case EVT_KEY_RELEASE:
        case EVT_COMMAND:
            return Cameralist_RouteDialogEvt(pMe, eCode, wParam, dwParam); 
        default:
            return Cameralist_RouteDialogEvt(pMe, eCode, wParam, dwParam);
    }
}

/*==============================================================================
����:
   Cameralist_ProcessState

˵��:
    ���� pMe->m_currState, ������Ӧ��״̬��������

����:
    pMe [in]: ָ��Game Applet����ṹ��ָ�롣�ýṹ����С������ض���Ϣ��

����ֵ:
    NFSMACTION_CONTINUE: ָʾ��ͣ״̬����
    NFSMACTION_WAIT: ָʾֹͣ״̬����

��ע:

==============================================================================*/
NextFSMAction Cameralist_ProcessState(Cameralist *pMe)
{
    NextFSMAction retVal = NFSMACTION_WAIT;
    
    if (NULL == pMe)
    {
        return retVal;
    }
    MSG_FATAL("Cameralist_ProcessState=%d",pMe->m_currState,0,0);
    switch(pMe->m_currState)
    {
        case CAMERALIST_MAIN:
            return MAINST_CAMERALIST_Handler(pMe);

        case CAMERALIST_EXIT:
            return CAMERALISTST_EXIT_Handler(pMe);  

        default:
            break;

    }
    return retVal;
}

/*==============================================================================
����:
    Cameralist_MAIN_Handler

˵��:
   GAMEST_MAIN ״̬��������

����:
    pMe [in]: ָ��GAME Applet����ṹ��ָ�롣�ýṹ����С������ض���Ϣ��

����ֵ:
    NFSMACTION_CONTINUE: ָʾ��ͣ״̬����
    NFSMACTION_WAIT: ָʾֹͣ״̬����

��ע:

==============================================================================*/
static NextFSMAction MAINST_CAMERALIST_Handler(Cameralist *pMe)
{
	boolean locksel;
    if (NULL == pMe)
    {
        return NFSMACTION_WAIT;
    }
	{
    	switch (pMe->m_eDlgReturn)
    	{
        	// ����������
        	case DLGRET_CREATE:
            	Cameralist_ShowDialog(pMe, IDD_CAMERALIST_DIALOGS);
            	return NFSMACTION_WAIT;
            
        	default:
            	MOVE_TO_STATE(CAMERALIST_EXIT)
            	return NFSMACTION_CONTINUE;
    	}
	}
}


/*==============================================================================
����:
    Cameralist_EXIT_Handler

˵��:
    GAMEST_EXIT ״̬��������

����:
    pMe [in]: ָ��GAME Applet����ṹ��ָ�롣�ýṹ����С������ض���Ϣ��

����ֵ:
    NFSMACTION_CONTINUE: ָʾ��ͣ״̬����
    NFSMACTION_WAIT: ָʾֹͣ״̬����

��ע:

==============================================================================*/
static NextFSMAction CAMERALISTST_EXIT_Handler(Cameralist * pMe)
{
    (void) ISHELL_CloseApplet(pMe->m_pShell, FALSE);
    return NFSMACTION_WAIT;
} 
/*
       Cameralist_ShowDialog

˵��:
    ������״̬����������������Ӧ�Ի�����ԴID�ĶԻ���

����:
    pMe [in]: ָ�� MAINMENU Applet ����ṹ��ָ�롣�ýṹ����С������ض���Ϣ��
    dlgResId [in]: �Ի�����ԴID���������ݸ�ID�����Ի���

����ֵ:
    none

��ע:

==============================================================================*/
void Cameralist_ShowDialog(Cameralist  *pMe,  uint16 dlgResId)
{
    int nRet;
    // ÿ������һ���Ի���
    if (ISHELL_GetActiveDialog(pMe->m_pShell) != NULL)
    {
        // �����ǰ�Ѿ��жԻ��򱻴򿪣�ֱ�ӷ���
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
    }
    
    nRet = ISHELL_CreateDialog(pMe->m_pShell,CAMERALIST_RES_FILE_LANG,dlgResId,NULL);
    if (nRet != SUCCESS)
    {
    }
    
}


/*=============================================================================
FUNCTION: Cameralist_ListMenuHandler

DESCRIPTION:   �б�˵�

PARAMETERS:

=============================================================================*/

static boolean Cameralist_ListMenuHandler(Cameralist *pMe, AEEEvent eCode, uint16 wParam, uint32 dwParam)
{
    PARAM_NOT_REF(dwParam)
    IMenuCtl *pMenu = (IMenuCtl*)IDIALOG_GetControl(pMe->m_pActiveIDlg,IDC_CAMERALIST);
        
    if (pMenu == NULL)
    {
        return FALSE;
    }

    switch (eCode)
    {
        case EVT_DIALOG_INIT:
			#if 0
            IMENUCTL_SetTitle(pMenu, CAMERALIST_RES_FILE_LANG, IDS_CAMERALIST_TITLE, NULL);
			#else
		    {
		  		AECHAR WTitle[40] = {0};
				(void)ISHELL_LoadResString(pMe->m_pShell,
                        CAMERALIST_RES_FILE_LANG,                                
                        IDS_CAMERALIST_TITLE,
                        WTitle,
                        sizeof(WTitle));
                if(pMe->m_pIAnn != NULL)
                {
				    IANNUNCIATOR_SetFieldTextEx(pMe->m_pIAnn,WTitle,FALSE);
                }
		    }
			#endif
            IANNUNCIATOR_Redraw(pMe->m_pIAnn);
            
 
			IMENUCTL_AddItem(pMenu, CAMERALIST_RES_FILE_LANG,IDS_CAMERALIST_CAMERA, IDS_CAMERALIST_CAMERA, NULL, 0);
            IMENUCTL_AddItem(pMenu, CAMERALIST_RES_FILE_LANG,IDS_CAMERALIST_VIDEOPLAYER, IDS_CAMERALIST_VIDEOPLAYER, NULL, 0);
            IMENUCTL_AddItem(pMenu, CAMERALIST_RES_FILE_LANG,IDS_CAMERALIST_VIDEO_CAMERA, IDS_CAMERALIST_VIDEO_CAMERA, NULL, 0);

            return TRUE;
            
        case EVT_DIALOG_START:
            {  
                int i;

				IMENUCTL_SetProperties(pMenu, MP_UNDERLINE_TITLE|MP_WRAPSCROLL|MP_BIND_ITEM_TO_NUMBER_KEY|MP_ACTIVE_NO_REDRAW);
				
                IMENUCTL_SetOemProperties( pMenu, OEMMP_USE_MENU_STYLE);
                IMENUCTL_SetBottomBarType(pMenu,BTBAR_SELECT_BACK);
                IMENUCTL_SetSel(pMenu, pMe->m_MainSel);
            }
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
                case AVK_7:
                case AVK_8:
                case AVK_9:
                    StartApplet(pMe, IMENUCTL_GetItemID(pMenu, (wParam - AVK_1)));
                    return TRUE;
                    
                case AVK_0:
                    StartApplet(pMe, IMENUCTL_GetItemID(pMenu, 9));
                    return TRUE;
                    
                case AVK_STAR:
                    StartApplet(pMe, IMENUCTL_GetItemID(pMenu, 10));
                    return TRUE;
                    
                case AVK_POUND:
                    StartApplet(pMe, IMENUCTL_GetItemID(pMenu, 11));
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
            StartApplet(pMe, wParam);
            return TRUE;
            
        #ifdef FEATURE_LCD_TOUCH_ENABLE//andrew add for LCD touch
		case EVT_PEN_UP:
			{
				AEEDeviceInfo devinfo;
				int nBarH ;
				AEERect rc;
				int16 wXPos = (int16)AEE_GET_X(dwParam);
				int16 wYPos = (int16)AEE_GET_Y(dwParam);

				nBarH = GetBottomBarHeight(pMe->m_pDisplay);
        
				MEMSET(&devinfo, 0, sizeof(devinfo));
				ISHELL_GetDeviceInfo(pMe->m_pShell, &devinfo);
				SETAEERECT(&rc, 0, devinfo.cyScreen-nBarH, devinfo.cxScreen, nBarH);

				if(TOUCH_PT_IN_RECT(wXPos,wYPos,rc))
				{
					if(wXPos >= rc.x && wXPos < rc.x + (rc.dx/3) )//��
					{
						boolean rt =  ISHELL_PostEvent(pMe->m_pShell,AEECLSID_CAMERA_LIST,EVT_KEY,AVK_SELECT,0);
						return rt;
					}
					else if(wXPos >= rc.x + (rc.dx/3)   && wXPos < rc.x + (rc.dx/3)*2 )//��
					{
						 boolean rt = ISHELL_PostEvent(pMe->m_pShell,AEECLSID_CAMERA_LIST,EVT_KEY,AVK_INFO,0);
						 return rt;
					}
					else if(wXPos >= rc.x + (rc.dx/3)*2 && wXPos < rc.x + (rc.dx/3)*3 )//��
					{						
						 boolean rt = ISHELL_PostEvent(pMe->m_pShell,AEECLSID_CAMERA_LIST,EVT_KEY,AVK_CLR,0);
						 return rt;
					}
				}

			}
			break;
#endif
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
static int StartApplet(Cameralist *pMe, int i)
{
    int Result = EUNSUPPORTED;
    switch(i)
    {
#if defined (FEATURE_BREW_CAMERA)&&defined(FEATURE_MOVIE_RECORD_SUPPORT)
		case IDS_CAMERALIST_VIDEO_CAMERA:
			Result = ISHELL_StartAppletArgs(pMe->m_pShell, AEECLSID_APP_CAMERA, "record");
            break;
#endif
        case IDS_CAMERALIST_CAMERA:
            Result = ISHELL_StartApplet(pMe->m_pShell, AEECLSID_APP_CAMERA);
            break;      
        
#ifdef FEATURE_APP_MPEG4
		case IDS_CAMERALIST_VIDEOPLAYER:
			Result = ISHELL_StartApplet(pMe->m_pShell, AEECLSID_VIDEOPLAYER);
            break;
#endif
		default:
			break;
        
    }
    return Result;
}

