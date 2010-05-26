/** ----------------------------------------------------------------------------
 * MainMenu.c
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
#include "mainmenu.h"
#include "mainmenu_priv.h" 

#include "ContApp.h"

#include "Appscommon.h"
#include "appscommon.brh"
#include "Appscommon_color.brh"

#if !defined( AEE_SIMULATOR)
#include "AEERUIM.h" 
#include "OEMCFGI.h"
#endif //#if !defined( AEE_SIMULATOR)

#define PARAM_NOT_REF(x)
#ifdef FEATURE_LCD_TOUCH_ENABLE//wlh add for LCD touch
#define MAINMENU_SELECT 10000
#define MAINMENU_PT_IN_RECT(a,b,rct)      (boolean)( ((a) >= (rct).x && (a) <= ((rct).x + (rct).dx)) && ((b) >= (rct).y && (b) <= ((rct).y + (rct).dy)) )
#endif//FEATURE_LCD_TOUCH_ENABLE
/*==============================================================================

                              ���غ�������

==============================================================================*/
static MainMenuMod gMainMenuMod;
static const VTBL( IModule) gModFuncs =
{
    MainMenuMod_AddRef,
    MainMenuMod_Release,
    MainMenuMod_CreateInstance,
    MainMenuMod_FreeResources
};

static uint32  MainMenu_AddRef( IMainMenu *pi);

static uint32  MainMenu_Release ( IMainMenu *pi);

static boolean MainMenu_HandleEvent( IMainMenu *pi,
    AEEEvent eCode,
    uint16  wParam,
    uint32 dwParam
);

static int     MainMenu_MainMenuService( IMainMenu *pi, MainMenuServiceType eStype);

static MainMenu gMainMenu={0};

static const VTBL( IMainMenu) gMainMenuMethods =
{
    MainMenu_AddRef,
    MainMenu_Release,
    MainMenu_HandleEvent,
    MainMenu_MainMenuService
};

int MainMenuMod_Load( IShell *pIShell, void *ph, IModule **ppMod);

int MainMenuMod_New( int16 nSize,
    IShell              *pIShell,
    void                *ph,
    IModule             **ppMod,
    PFNMODCREATEINST    pfnMC,
    PFNFREEMODDATA      pfnMF
);

static int MainMenu_New( IShell *ps, IModule *pIModule, IMainMenu **ppObj);

static int CMainMenu_InitAppData(MainMenu *pMe);

static void CMainMenu_FreeAppData(MainMenu *pMe);

static void MainMenu_RunFSM(MainMenu *pMe);

static void calculateScreenParameters(MainMenu *pMe);

static void MainMenu_DrawBackGround(MainMenu *pMe, AEERect *pRect);

// ��ʼ����������ȫ����ʼͼ��
static void DrawMatrix(MainMenu *pMe);

#ifdef FEATURE_ICON_MOVE_ANIMATION
static void DrawFocusMoveAnimation(MainMenu *pMe);
#endif

#ifdef FEATURE_FOCUS_ANIMATION
static void DrawFocusIconAnimation(MainMenu *pMe);
#else
static void DrawFocusIcon(MainMenu *pMe);
#endif

static void MoveCursorTo(MainMenu *pMe, int row, int column);

static void setCursor(MainMenu *pMe, int row, int column);

static boolean StartApplet(MainMenu *pMe, int i);

boolean MainMenu_RouteDialogEvt(MainMenu *pMe,
    AEEEvent    eCode,
    uint16  wParam,
    uint32 dwParam
);

NextFSMAction MainMenu_ProcessState(MainMenu *pMe);

void MainMenu_ShowDialog(MainMenu  *pMe,  uint16 dlgResId);

// MAINST_MAIN ״̬������
static NextFSMAction MAINST_MAIN_Handler(MainMenu *pMe);

//MAINST_GAME  ״̬������
static NextFSMAction MAINST_GAME_Handler(MainMenu *pMe);

//MAINST_PLAYER ״̬������
static NextFSMAction MAINST_PLAYER_Handler(MainMenu *pMe);

// MAINST_DATA ״̬������
static NextFSMAction MAINST_DATA_Handler(MainMenu *pMe);

//MAINST_MSGBOX ״̬������
#ifdef FEATRUE_SUPPORT_G_SENSOR
static NextFSMAction MAINST_MSGBOX_Handler(MainMenu *pMe);
#endif

//MAINST_EXIT  ״̬������
static NextFSMAction MAINST_EXIT_Handler(MainMenu *pMe);

static boolean  MainMenu_IconMenuHandler(MainMenu *pMe, AEEEvent eCode, uint16 wParam, uint32 dwParam);

static boolean  MainMenu_GameMenuHanlder(MainMenu *pMe, AEEEvent eCode, uint16 wParam, uint32 dwParam);

static boolean  MainMenu_ListMenuHandler(MainMenu *pMe, AEEEvent eCode, uint16 wParam, uint32 dwParam);

static boolean  MainMenu_PlayerMenuHandler(MainMenu *pMe, AEEEvent eCode, uint16 wParam, uint32 dwParam);

static boolean  MainMenu_DataMenuHandler(MainMenu *pMe, AEEEvent eCode, uint16 wParam, uint32 dwParam);

#ifdef FEATRUE_SUPPORT_G_SENSOR
static boolean  MainMenu_MsgBoxHandler(MainMenu *pMe, AEEEvent eCode, uint16 wParam, uint32 dwParam);
static void Mainmenu_DialogTimeout(void *pme);

boolean MainMenu_Set_Shake_Disable(MainMenu *pMe);
boolean MainMenu_Get_Shake_OnOrOff(MainMenu *pMe);
void MainMenu_CloseSportBgRun(MainMenu *pMe);


#endif

void MainMenu_DraImage(MainMenu *pMe,int16 index,int16 imgid);


/*==============================================================================
                              
                              ���أ���̬������
                              
==============================================================================*/
static char* ICON_ANI[] =
{
    ICON1_ANI,
    ICON2_ANI,
    ICON3_ANI,
    ICON4_ANI,
    ICON5_ANI,
    ICON6_ANI,
    ICON7_ANI,
    ICON8_ANI,
    ICON9_ANI,
#ifdef FEATURE_CARRIER_CHINA_TELCOM
    ICON10_ANI,
#else
#ifdef FEATURE_APP_NUMBERMANAGER
    ICON13_ANI,
#else    
    ICON10_ANI,
#endif
#endif
    ICON11_ANI,
    ICON12_ANI,
};
//wlh 20090410 add  start �ƶ�Ч��
static char* ICON_ANI_1[] =
{
    ICON1_ANI_1,
    ICON2_ANI_1,
    ICON3_ANI_1,
    ICON4_ANI_1,
    ICON5_ANI_1,
    ICON6_ANI_1,
    ICON7_ANI_1,
    ICON8_ANI_1,
    ICON9_ANI_1,
#ifdef FEATURE_CARRIER_CHINA_TELCOM
    ICON10_ANI,
#else
#ifdef FEATURE_APP_NUMBERMANAGER
    ICON13_ANI_1,
#else    
    ICON10_ANI_1,
#endif
#endif
    ICON11_ANI_1,
    ICON12_ANI_1,
};
static char* ICON_ANI_2[] =
{
    ICON1_ANI_2,
    ICON2_ANI_2,
    ICON3_ANI_2,
    ICON4_ANI_2,
    ICON5_ANI_2,
    ICON6_ANI_2,
    ICON7_ANI_2,
    ICON8_ANI_2,
    ICON9_ANI_2,
#ifdef FEATURE_CARRIER_CHINA_TELCOM
    ICON10_ANI,
#else
#ifdef FEATURE_APP_NUMBERMANAGER
    ICON13_ANI_2,
#else    
    ICON10_ANI_2,
#endif
#endif
    ICON11_ANI_2,
    ICON12_ANI_2,
};
//wlh 20090410 add  end �ƶ�Ч��
/*=============================================================================
FUNCTION:  MainMenuMod_Load

DESCRIPTION: 

PARAMETERS:

RETURN VALUE:

COMMENTS:

SIDE EFFECTS:

SEE ALSO:

=============================================================================*/
int MainMenuMod_Load( IShell *pIShell, void *ph, IModule **ppMod)
{
    return MainMenuMod_New( sizeof( MainMenuMod),
                    pIShell,
                    ph,
                    ppMod,
                    NULL,
                    NULL);
}

/*=============================================================================
FUNCTION:  MainMenuMod_New

DESCRIPTION: 

PARAMETERS:

RETURN VALUE:

COMMENTS:

SIDE EFFECTS:

SEE ALSO:

=============================================================================*/
int  MainMenuMod_New( int16 nSize,
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

    MEMSET( &gMainMenuMod, 0, sizeof( MainMenuMod));

    INIT_VTBL( &gMainMenuMod, IModule, gModFuncs);
    gMainMenuMod.referenceCounter = 1;
    *ppMod = ( IModule *)&gMainMenuMod;
    return AEE_SUCCESS;
}

/*=============================================================================
FUNCTION:  MainMenuMod_AddRef

DESCRIPTION: 

PARAMETERS:

RETURN VALUE:

COMMENTS:

SIDE EFFECTS:

SEE ALSO:

=============================================================================*/
static uint32 MainMenuMod_AddRef( IModule *po)
{
    return( ++( ( MainMenuMod *)po)->referenceCounter);
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
static uint32 MainMenuMod_Release( IModule *po)
{
    if ( ( ( MainMenuMod *) po)->referenceCounter == 0)
    {
        return 0;
    }

    return( --( ( MainMenuMod *)po)->referenceCounter);
}

/*=============================================================================
FUNCTION:  MainMenuMod_CreateInstance

DESCRIPTION: 

PARAMETERS:

RETURN VALUE:

COMMENTS:

SIDE EFFECTS:

SEE ALSO:

=============================================================================*/
static int  MainMenuMod_CreateInstance( IModule *po,
    IShell *pIShell,
    AEECLSID ClsId,
    void **ppObj
)
{
    *ppObj = NULL;

    if( ClsId != AEECLSID_MAIN_MENU || MainMenu_New( pIShell, po, ( IMainMenu**)ppObj) != SUCCESS)
    {
        return EFAILED;
    }

    return SUCCESS;
}

/*=============================================================================
FUNCTION:  MainMenuMod_FreeResources

DESCRIPTION: 

PARAMETERS:

RETURN VALUE:

COMMENTS:

SIDE EFFECTS:

SEE ALSO:

=============================================================================*/
static void MainMenuMod_FreeResources( IModule *po, IHeap *ph, IFileMgr *pfm)
{
}

/*=============================================================================
FUNCTION:  MainMenu_New

DESCRIPTION: 

PARAMETERS:

RETURN VALUE:

COMMENTS:

SIDE EFFECTS:

SEE ALSO:

=============================================================================*/
static int MainMenu_New( IShell *ps, IModule *pIModule, IMainMenu **ppObj)
{
    int retVal = SUCCESS;
    if( 0 == gMainMenu.referenceCounter)
    {

        if( pIModule == NULL || ps == NULL)
        {
            return EFAILED;
        }

        MEMSET(&gMainMenu, 0, sizeof(gMainMenu));
        INIT_VTBL(&gMainMenu, IMainMenu, gMainMenuMethods);

       
        gMainMenu.m_pShell     = ps;
        gMainMenu.m_pModule    = pIModule;
        retVal = CMainMenu_InitAppData(&gMainMenu);
        if(retVal != SUCCESS)
        {
            CMainMenu_FreeAppData((MainMenu*)&gMainMenu);
            return retVal;
        }
        (void) ISHELL_AddRef( ps);
        (void) IMODULE_AddRef( pIModule);
        gMainMenu.referenceCounter  = 0;

        
    }

    ++ gMainMenu.referenceCounter;
    *ppObj = ( IMainMenu*)&gMainMenu;
    
    return retVal;
}

/*==============================================================================
����:
    CMainMenuApp_InitAppData

˵��:
    ��ʼ�� MainMenu Applet �ṹ����ȱʡֵ��

����:
    pMe [in]: ָ�� MainMenu Applet ����ṹ��ָ�롣�ýṹ����С������ض���Ϣ��

����ֵ:
    EFAILED: ��ʼ��ʧ�ܡ�
    SUCCESS: ��ʼ���ɹ���

��ע:

==============================================================================*/
static int CMainMenu_InitAppData(MainMenu *pMe)
{
	int i;
	boolean iamgeflag = FALSE;
    if (NULL == pMe)
    {
        return EFAILED;
    }
    pMe->m_nRow        = 1;
    pMe->m_nColumn     = 1;
#ifdef FEATURE_ICON_MOVE_ANIMATION
    pMe->m_nPrevRow   = 1;
    pMe->m_nPrevColumn = 1;
    pMe->m_bMoveing = FALSE;
    pMe->m_pDevImage = NULL; 
#endif
    pMe->m_MainSel  = 0;
    pMe->m_MenuSel  = 0;
#ifdef FEATURE_FOCUS_ANIMATION
    pMe->m_nIconAniFrameIdx      = 0;
#endif
    pMe->m_bNormalStart = TRUE;


    // �ӿڴ�������س�ʼ��
    if (ISHELL_CreateInstance(pMe->m_pShell, AEECLSID_CONFIG,
            (void **)&pMe->m_pConfig) != SUCCESS)
    {
        return EFAILED;
    }
    
    if (ISHELL_CreateInstance(pMe->m_pShell, AEECLSID_DISPLAY, 
            (void **) &pMe->m_pDisplay) != SUCCESS)
    {
        return EFAILED;
    }
//wlh 20090409 add start
	pMe->m_pImageTurn[0] = ISHELL_LoadImage(pMe->m_pShell,ICON1_ANI_1);
	pMe->m_pImageTurn[1] = ISHELL_LoadImage(pMe->m_pShell,ICON2_ANI_1);
	pMe->m_pImageTurn[2] = ISHELL_LoadImage(pMe->m_pShell,ICON3_ANI_1);
	pMe->m_pImageTurn[3] = ISHELL_LoadImage(pMe->m_pShell,ICON4_ANI_1);
	pMe->m_pImageTurn[4] = ISHELL_LoadImage(pMe->m_pShell,ICON5_ANI_1);
	pMe->m_pImageTurn[5] = ISHELL_LoadImage(pMe->m_pShell,ICON6_ANI_1);
	pMe->m_pImageTurn[6] = ISHELL_LoadImage(pMe->m_pShell,ICON7_ANI_1);
	pMe->m_pImageTurn[7] = ISHELL_LoadImage(pMe->m_pShell,ICON8_ANI_1);
	pMe->m_pImageTurn[8] = ISHELL_LoadImage(pMe->m_pShell,ICON9_ANI_1);
	pMe->m_pImageTurn[9] = ISHELL_LoadImage(pMe->m_pShell,ICON10_ANI_1);
	pMe->m_pImageTurn[10] = ISHELL_LoadImage(pMe->m_pShell,ICON11_ANI_1);
	pMe->m_pImageTurn[11] = ISHELL_LoadImage(pMe->m_pShell,ICON13_ANI_1);
	pMe->m_pImageTurn[12] = ISHELL_LoadImage(pMe->m_pShell,ICON1_ANI_2);
	pMe->m_pImageTurn[13] = ISHELL_LoadImage(pMe->m_pShell,ICON2_ANI_2);
	pMe->m_pImageTurn[14] = ISHELL_LoadImage(pMe->m_pShell,ICON3_ANI_2);
	pMe->m_pImageTurn[15] = ISHELL_LoadImage(pMe->m_pShell,ICON4_ANI_2);
	pMe->m_pImageTurn[16] = ISHELL_LoadImage(pMe->m_pShell,ICON5_ANI_2);
	pMe->m_pImageTurn[17] = ISHELL_LoadImage(pMe->m_pShell,ICON6_ANI_2);
	pMe->m_pImageTurn[18] = ISHELL_LoadImage(pMe->m_pShell,ICON7_ANI_2);
	pMe->m_pImageTurn[19] = ISHELL_LoadImage(pMe->m_pShell,ICON8_ANI_2);
	pMe->m_pImageTurn[20] = ISHELL_LoadImage(pMe->m_pShell,ICON9_ANI_2);
	pMe->m_pImageTurn[21] = ISHELL_LoadImage(pMe->m_pShell,ICON10_ANI_2);
	pMe->m_pImageTurn[22] = ISHELL_LoadImage(pMe->m_pShell,ICON11_ANI_2);
	pMe->m_pImageTurn[23] = ISHELL_LoadImage(pMe->m_pShell,ICON13_ANI_2);

	for (i = 0; i < MAX_TURN_NUM; i ++)
	{
		if(pMe->m_pImageTurn[i] == NULL)
        {
			iamgeflag = TRUE;
			break;
		}
	}
	if(iamgeflag)
	{
		for (i = 0; i < MAX_TURN_NUM; i ++)
		{
			if(pMe->m_pImageTurn[i] != NULL)
			{
				IImage_Release(pMe->m_pImageTurn[i]);
				pMe->m_pImageTurn[i] = NULL;
			}
		}
		return EFAILED;
	}
//wlh 20090409 add end
    return SUCCESS;
}

/*==============================================================================
����:
    CMainMenu_FreeAppData

˵��:
    �ͷ� MainMenu Applet �����Դ��

����:
    pMe [in]: ָ��MainMenu Applet ����ṹ��ָ�롣�ýṹ����С������ض���Ϣ��

����ֵ:
    none

��ע:

==============================================================================*/
static void CMainMenu_FreeAppData(MainMenu *pMe)
{
    if (NULL == pMe)
    {
        return;
    }

    pMe->m_eAppStatus = MAINMENU_STOP;
    if (NULL != pMe->m_pConfig)
    {
        ICONFIG_Release(pMe->m_pConfig);
        pMe->m_pConfig = NULL;
    }    
    
    if (pMe->m_pDisplay != NULL)
    {
        (void) IDISPLAY_Release(pMe->m_pDisplay);
        pMe->m_pDisplay = NULL;
    }

    //�ͷ�ͼƬ��Դ
    {
        int i;
        
        if (pMe->m_pImageBg !=NULL)
        {
            (void) IIMAGE_Release(pMe->m_pImageBg);
            pMe->m_pImageBg = NULL;
        }

        for(i=0;i<MAX_MATRIX_ITEMS;i++)
        {
            if(pMe->m_pImageIcon[i] != NULL)
            {
                (void)IIMAGE_Release(pMe->m_pImageIcon[i]);
                pMe->m_pImageIcon[i] = NULL;
            }
        } 

        if(pMe->m_pAnimate != NULL)
        {
            (void)IIMAGE_Release(pMe->m_pAnimate);
            pMe->m_pAnimate = NULL;
        }

#ifdef FEATURE_ICON_MOVE_ANIMATION
        if(pMe->m_pDevImage != NULL)
        {
            (void)IBitmap_Release(pMe->m_pDevImage);
            pMe->m_pDevImage = NULL;
        }
#endif

		for (i = 0; i < MAX_TURN_NUM; i ++)
		{
			if(pMe->m_pImageTurn[i] != NULL)
        	{
				IImage_Release(pMe->m_pImageTurn[i]);
				pMe->m_pImageTurn[i] = NULL;
			}
		}
    }    
}

/*=============================================================================
FUNCTION:  MainMenu_AddRef

DESCRIPTION: 

PARAMETERS:

RETURN VALUE:

COMMENTS:

SIDE EFFECTS:

SEE ALSO:

=============================================================================*/
static uint32  MainMenu_AddRef( IMainMenu *pi)
{
    return ( ++gMainMenu.referenceCounter);
}

/*=============================================================================
FUNCTION:  MainMenu_Release

DESCRIPTION: 

PARAMETERS:

RETURN VALUE:

COMMENTS:

SIDE EFFECTS:

SEE ALSO:

=============================================================================*/
static uint32  MainMenu_Release( IMainMenu *pi)
{
    register MainMenu *pMe = (MainMenu*)pi;

    if (pMe->referenceCounter == 0)
    {
        return 0;
    }
    
    if( --pMe->referenceCounter)
    {
        return pMe->referenceCounter;
    }


    CMainMenu_FreeAppData(pMe);

    (void) ISHELL_Release(pMe->m_pShell);
    (void) IMODULE_Release(pMe->m_pModule);

    return 0;
}

/*==============================================================================
����:
    MainMenu_RunFSM

˵��:
    ��������״̬�����档

����:
    pMe [in]: ָ��MainMenu Applet����ṹ��ָ�롣�ýṹ����С������ض���Ϣ��
       
����ֵ:
    �ޡ�

��ע:

==============================================================================*/
static void MainMenu_RunFSM(MainMenu *pMe)
{
    NextFSMAction nextFSMAction = NFSMACTION_WAIT;
    for ( ; ; )
    {
        nextFSMAction = MainMenu_ProcessState(pMe);

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
static boolean MainMenu_HandleEvent( IMainMenu *pi,
    AEEEvent eCode,
    uint16  wParam,
    uint32 dwParam
)
{
    MainMenu *pMe = (MainMenu*)pi;
    AEEAppStart* as = NULL;
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

            pMe->m_currState = MAINST_MAIN;
            pMe->m_eDlgReturn = DLGRET_CREATE;
            pMe->m_eAppStatus = MAINMENU_RUNNING;
            pMe->m_bNormalStart = TRUE;
            if (as->pszArgs != NULL)
            {
                switch(as->pszArgs[0])
                {
                    case STARTARGPREFIX_VIEWPLAYER:
                        pMe->m_currState = MAINST_PLAYER;
                        break;

                    case STARTARGPREFIX_VIEWDATA:
                        pMe->m_currState = MAINST_DATA;
                        break;

                    default:
                        break;
                }
                pMe->m_bNormalStart = FALSE;
            }
            MainMenu_RunFSM(pMe);
            return TRUE;

        case EVT_APP_STOP:
            {
                int theFocus = 4;
                setCursor( pMe, theFocus / 3, theFocus % 3);
                pMe->m_MainSel  = 0;
                pMe->m_MenuSel  = 0;                  
                (void)ISHELL_CancelTimer( pMe->m_pShell, NULL, pMe);
                pMe->m_eAppStatus = MAINMENU_STOP;
                
                return TRUE;
            }

        case EVT_APP_SUSPEND:
            (void)ISHELL_CancelTimer(pMe->m_pShell, NULL, pMe);
            pMe->m_eAppStatus = MAINMENU_SUSPEND;
            
            return TRUE;
        

        case EVT_APP_RESUME:
            {
                AEEAppStart* as = ( AEEAppStart*)dwParam;
                pMe->m_rc    = as->rc;
                pMe->m_eAppStatus = MAINMENU_RUNNING;

                // ��ʼ��WMS״̬��
                MainMenu_RunFSM(pMe); 
                return TRUE;
            }

        case EVT_DIALOG_INIT:
            
            pMe->m_pActiveIDlg = (IDialog*)dwParam;
            pMe->m_pActivedlgID = wParam;
            
            return MainMenu_RouteDialogEvt(pMe,eCode,wParam,dwParam);

        case EVT_DIALOG_START:
            return MainMenu_RouteDialogEvt(pMe,eCode,wParam,dwParam);

        case EVT_USER_REDRAW:
            return MainMenu_RouteDialogEvt(pMe,eCode,wParam,dwParam);

        case EVT_DIALOG_END:
            (void) MainMenu_RouteDialogEvt(pMe,eCode,wParam,dwParam);
            pMe->m_pActiveIDlg = NULL;
            pMe->m_pActivedlgID = 0;
            if(pMe->m_eAppStatus == MAINMENU_RUNNING)
            {
                MainMenu_RunFSM(pMe);
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
            return MainMenu_RouteDialogEvt(pMe,eCode,wParam,dwParam);   

        case EVT_KEY_PRESS:
        case EVT_KEY_RELEASE:
        case EVT_COMMAND:
            return MainMenu_RouteDialogEvt(pMe,eCode,wParam,dwParam); 
			
#ifdef FEATURE_LCD_TOUCH_ENABLE//wlh for LCD TOUCH
		case EVT_PEN_UP:
		//case EVT_PEN_DOWN: 
		{
			return MainMenu_RouteDialogEvt(pMe,eCode,wParam,dwParam);
		}
        case EVT_USER:  
			if(eCode == EVT_USER)
			{
				if(dwParam != MAINMENU_SELECT)
				{
					if((wParam == AVK_SELECT) || (wParam == AVK_INFO))
					{
						eCode = EVT_COMMAND;
						return MainMenu_RouteDialogEvt(pMe,eCode,dwParam,0);//ע�������aeemenu�����˵�ǰ��menu id dwParam תΪ EVT_COMMAND �¼�ʱҪ��wParam ��
					}
				}
				else
				{	
					if((wParam == AVK_SELECT) || (wParam == AVK_INFO))
					{
						eCode = EVT_KEY;
						return MainMenu_RouteDialogEvt(pMe,eCode,wParam,dwParam);
					}
				}
				if(wParam == AVK_CLR)
				{
					eCode = EVT_KEY;
					return MainMenu_RouteDialogEvt(pMe,eCode,wParam,dwParam);
				}
				
			}
#endif//FEATURE_LCD_TOUCH_ENABLE

        default:
            return MainMenu_RouteDialogEvt(pMe,eCode,wParam,dwParam);
    }
}

/*==============================================================================
����:
    MainMenu_ProcessState

˵��:
    ���� pMe->m_currState, ������Ӧ��״̬��������

����:
    pMe [in]: ָ��MainMenu Applet����ṹ��ָ�롣�ýṹ����С������ض���Ϣ��

����ֵ:
    NFSMACTION_CONTINUE: ָʾ��ͣ״̬����
    NFSMACTION_WAIT: ָʾֹͣ״̬����

��ע:

==============================================================================*/
NextFSMAction MainMenu_ProcessState(MainMenu *pMe)
{
    NextFSMAction retVal = NFSMACTION_WAIT;
    
    if (NULL == pMe)
    {
        return retVal;
    }
    
    switch(pMe->m_currState)
    {
        case MAINST_GAME:
            return MAINST_GAME_Handler(pMe);

        case MAINST_MAIN:
            return MAINST_MAIN_Handler(pMe);

        case MAINST_PLAYER:
            return MAINST_PLAYER_Handler(pMe);

        case MAINST_DATA:
            return MAINST_DATA_Handler(pMe);
            
        #ifdef FEATRUE_SUPPORT_G_SENSOR 
        case MAINST_MSGBOX:
            return MAINST_MSGBOX_Handler(pMe);
        #endif
            
        case MAINST_EXIT:
            return MAINST_EXIT_Handler(pMe);            

        default:
            break;

    }
    return retVal;
}

/*==============================================================================
����:
    MAINST_MAIN_Handler

˵��:
    MAINST_MAIN ״̬��������

����:
    pMe [in]: ָ��MAINMENU Applet����ṹ��ָ�롣�ýṹ����С������ض���Ϣ��

����ֵ:
    NFSMACTION_CONTINUE: ָʾ��ͣ״̬����
    NFSMACTION_WAIT: ָʾֹͣ״̬����

��ע:

==============================================================================*/
static NextFSMAction MAINST_MAIN_Handler(MainMenu *pMe)
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
                byte menu_style;
                ICONFIG_GetItem(pMe->m_pConfig, CFGI_MENU_STYLE, &menu_style, sizeof(menu_style));
                if(menu_style == OEMNV_MENU_STYLE_ICON)
                {
                    MainMenu_ShowDialog(pMe, IDD_MAIN_MENU);
                }
                else
                {
                    MainMenu_ShowDialog(pMe, IDD_LIST_MENU);
                }
            }
            return NFSMACTION_WAIT;

        case DLGRET_GAME:
            MOVE_TO_STATE(MAINST_GAME)
            return NFSMACTION_CONTINUE;

        case DLGRET_MEDIA:
            MOVE_TO_STATE(MAINST_PLAYER)
            return NFSMACTION_CONTINUE;

        case DLGRET_DATA:
            MOVE_TO_STATE(MAINST_DATA)
            return NFSMACTION_CONTINUE;
            
        case DLGRET_CANCELED:
            MOVE_TO_STATE(MAINST_EXIT)
            return NFSMACTION_CONTINUE;

        default:
            MOVE_TO_STATE(MAINST_EXIT)
            return NFSMACTION_CONTINUE;
    }
}

/*==============================================================================
����:
    MAINST_GAME_Handler

˵��:
    MAINST_MAIN ״̬��������

����:
    pMe [in]: ָ��MAINMENU Applet����ṹ��ָ�롣�ýṹ����С������ض���Ϣ��

����ֵ:
    NFSMACTION_CONTINUE: ָʾ��ͣ״̬����
    NFSMACTION_WAIT: ָʾֹͣ״̬����

��ע:

==============================================================================*/
static NextFSMAction MAINST_GAME_Handler(MainMenu *pMe)
{

    if (NULL == pMe)
    {
        return NFSMACTION_WAIT;
    }
    switch (pMe->m_eDlgReturn)
    {
        // ����������
        case DLGRET_CREATE:
            MainMenu_ShowDialog(pMe, IDD_GAME_MENU);
            return NFSMACTION_WAIT;

        #ifdef FEATRUE_SUPPORT_G_SENSOR    
        case DLGRET_MSGBOX:
            MOVE_TO_STATE(MAINST_MSGBOX)
            return NFSMACTION_CONTINUE;  
       #endif 
       
        case DLGRET_CANCELED:
            MOVE_TO_STATE(MAINST_MAIN)
            return NFSMACTION_CONTINUE;

        default:
            MOVE_TO_STATE(MAINST_EXIT)
            return NFSMACTION_CONTINUE;            

    }
}

/*==============================================================================
����:
    MAINST_PLAYER_Handler

˵��:
    MAINST_PLAYER ״̬��������

����:
    pMe [in]: ָ��MAINMENU Applet����ṹ��ָ�롣�ýṹ����С������ض���Ϣ��

����ֵ:
    NFSMACTION_CONTINUE: ָʾ��ͣ״̬����
    NFSMACTION_WAIT: ָʾֹͣ״̬����

��ע:

==============================================================================*/
static NextFSMAction MAINST_PLAYER_Handler(MainMenu *pMe)
{
    
    if (NULL == pMe)
    {
        return NFSMACTION_WAIT;
    }
    switch (pMe->m_eDlgReturn)
    {
        // ����������
        case DLGRET_CREATE:
            MainMenu_ShowDialog(pMe, IDD_PLAYER_MENU);
            return NFSMACTION_WAIT;

        case DLGRET_CANCELED:
            if(pMe->m_bNormalStart)
            {
                MOVE_TO_STATE(MAINST_MAIN)
            }
            else
            {
                MOVE_TO_STATE(MAINST_EXIT)
            }
            return NFSMACTION_CONTINUE;

        default:
            MOVE_TO_STATE(MAINST_EXIT)
            return NFSMACTION_CONTINUE;            

    }
}

/*==============================================================================
����:
    MAINST_DATA_Handler

˵��:
    MAINST_MAIN ״̬��������

����:
    pMe [in]: ָ��MAINMENU Applet����ṹ��ָ�롣�ýṹ����С������ض���Ϣ��

����ֵ:
    NFSMACTION_CONTINUE: ָʾ��ͣ״̬����
    NFSMACTION_WAIT: ָʾֹͣ״̬����

��ע:

==============================================================================*/
static NextFSMAction MAINST_DATA_Handler(MainMenu *pMe)
{
    
    if (NULL == pMe)
    {
        return NFSMACTION_WAIT;
    }
    switch (pMe->m_eDlgReturn)
    {
        // ����������
        case DLGRET_CREATE:
            MainMenu_ShowDialog(pMe, IDD_DATA_MENU);
            return NFSMACTION_WAIT;

        case DLGRET_CANCELED:
            if(pMe->m_bNormalStart)
            {
                MOVE_TO_STATE(MAINST_MAIN)
            }
            else
            {
                MOVE_TO_STATE(MAINST_EXIT)
            }
            return NFSMACTION_CONTINUE;

        default:
            MOVE_TO_STATE(MAINST_EXIT)
            return NFSMACTION_CONTINUE;            

    }
}

/*==============================================================================
����:
    MAINST_MSGBOX_Handler

˵��:
    MAINST_MSGBOX ״̬��������

����:
    pMe [in]: ָ��MAINMENU Applet����ṹ��ָ�롣�ýṹ����С������ض���Ϣ��

����ֵ:
    NFSMACTION_CONTINUE: ָʾ��ͣ״̬����
    NFSMACTION_WAIT: ָʾֹͣ״̬����

��ע:

==============================================================================*/

#ifdef FEATRUE_SUPPORT_G_SENSOR
static NextFSMAction MAINST_MSGBOX_Handler(MainMenu *pMe)
{
    if (NULL == pMe)
    {
        return NFSMACTION_WAIT;
    }
    switch (pMe->m_eDlgReturn)
    {
        // ����������
        case DLGRET_CREATE:
            MainMenu_ShowDialog(pMe,IDD_MSGBOX_MENU);
            return NFSMACTION_WAIT;

        case DLGRET_GAME:
            MOVE_TO_STATE(MAINST_GAME)
            return NFSMACTION_CONTINUE;

        case DLGRET_CANCELED:
            MOVE_TO_STATE(MAINST_MAIN)
            return NFSMACTION_CONTINUE;

        default:
            MOVE_TO_STATE(MAINST_EXIT)
            return NFSMACTION_CONTINUE;            

    }

}
#endif


/*==============================================================================
����:
    MAINST_EXIT_Handler

˵��:
    MAINST_EXIT ״̬��������

����:
    pMe [in]: ָ��MAINMENU  Applet����ṹ��ָ�롣�ýṹ����С������ض���Ϣ��

����ֵ:
    NFSMACTION_CONTINUE: ָʾ��ͣ״̬����
    NFSMACTION_WAIT: ָʾֹͣ״̬����

��ע:

==============================================================================*/
static NextFSMAction MAINST_EXIT_Handler(MainMenu *pMe)
{
    (void) ISHELL_CloseApplet(pMe->m_pShell, FALSE);
    return NFSMACTION_WAIT;
} 


/*==============================================================================
����:
       MainMenu_ShowDialog

˵��:
    ������״̬����������������Ӧ�Ի�����ԴID�ĶԻ���

����:
    pMe [in]: ָ�� MAINMENU Applet ����ṹ��ָ�롣�ýṹ����С������ض���Ϣ��
    dlgResId [in]: �Ի�����ԴID���������ݸ�ID�����Ի���

����ֵ:
    none

��ע:

==============================================================================*/
void MainMenu_ShowDialog(MainMenu  *pMe,  uint16 dlgResId)
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
        
        if (dlgResId == IDD_MAIN_MENU)
        {
            (void)IDISPLAY_SetPrefs(pMe->m_pDisplay, "a:0", STRLEN("a:0"));
        }
        else
        {
            (void)IDISPLAY_SetPrefs(pMe->m_pDisplay, "a:1", STRLEN("a:1"));
        }
        
        ISHELL_GetDeviceInfo(pMe->m_pShell, &di);
        pMe->m_rc.dx = di.cxScreen;
        pMe->m_rc.dy = di.cyScreen;
        IDISPLAY_SetClipRect(pMe->m_pDisplay, &pMe->m_rc);
        calculateScreenParameters(pMe);
    }
    
    nRet = ISHELL_CreateDialog(pMe->m_pShell,MAINMENU_RES_FILE_LANG,dlgResId,NULL);
    if (nRet != SUCCESS)
    {
        DBGPRINTF("Failed to create the dialog %d in the MAINMENU applet.",dlgResId,0,0);
    }
}


/*==============================================================================
����:
       MainMenu_RouteDialogEvt
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
boolean MainMenu_RouteDialogEvt(MainMenu *pMe,
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
        case IDD_MAIN_MENU:
            return MainMenu_IconMenuHandler(pMe, eCode, wParam, dwParam);

        case IDD_GAME_MENU:
            return MainMenu_GameMenuHanlder(pMe, eCode, wParam, dwParam);

#ifdef FEATURE_MENU_STYLE
        case IDD_LIST_MENU:
            return MainMenu_ListMenuHandler(pMe, eCode, wParam, dwParam);
#endif

        case IDD_PLAYER_MENU:
            return MainMenu_PlayerMenuHandler(pMe, eCode, wParam, dwParam);

        case IDD_DATA_MENU:
            return MainMenu_DataMenuHandler(pMe, eCode, wParam, dwParam);

    #ifdef FEATRUE_SUPPORT_G_SENSOR
        case IDD_MSGBOX_MENU:
            return MainMenu_MsgBoxHandler(pMe, eCode, wParam, dwParam);
    #endif
        default:
            return FALSE;
    }
}

#ifdef FEATURE_LCD_TOUCH_ENABLE//WLH ADD FOR LCD TOUCH

#include "AEE.h"

/*=============================================================================
FUNCTION:  movepen_anddo

DESCRIPTION: ���㵱ǰͼƬ�����꣬����������Ӧ��������һ����ʾ���㣬�ǽ�������룩
=============================================================================*/
static movepen_anddo(MainMenu *pMe,uint32 dwParam)
{
	AEERect rc;
	uint16          iconSpaceHorizontal = 0;
    uint16          iconSpaceVertical   = 0;

	int16 wXPos = (int16)AEE_GET_X((const char *)dwParam);
	int16 wYPos = (int16)AEE_GET_Y((const char *)dwParam);

	SETAEERECT(&rc, pMe->m_IconFocus_Pt[0].x, pMe->m_IconFocus_Pt[0].y,pMe->m_IconFocus_Pt[2].x + ICON_ANIMATED_HEIGHT - pMe->m_IconFocus_Pt[0].x , pMe->m_IconFocus_Pt[10].y + ICON_ANIMATED_HEIGHT - pMe->m_IconFocus_Pt[0].y);
	if(!MAINMENU_PT_IN_RECT(wXPos,wYPos,rc))
	{

		AEEDeviceInfo devinfo;
		int nBarH ;
		AEERect rc;

		nBarH = GetBottomBarHeight(pMe->m_pDisplay);
        
		MEMSET(&devinfo, 0, sizeof(devinfo));
		ISHELL_GetDeviceInfo(pMe->m_pShell, &devinfo);
		SETAEERECT(&rc, 0, devinfo.cyScreen-nBarH, devinfo.cxScreen, nBarH);

		if(MAINMENU_PT_IN_RECT(wXPos,wYPos,rc))
		{
			if(wXPos >= rc.x && wXPos < rc.x + (rc.dx/3) )//��
			{
				boolean rt =  ISHELL_PostEvent(pMe->m_pShell,AEECLSID_MAIN_MENU,EVT_USER,AVK_SELECT,MAINMENU_SELECT);
				return rt;
			}
			else if(wXPos >= rc.x + (rc.dx/3)   && wXPos < rc.x + (rc.dx/3)*2 )//��
			{
				boolean rt = ISHELL_PostEvent(pMe->m_pShell,AEECLSID_MAIN_MENU,EVT_USER,AVK_INFO,MAINMENU_SELECT);
				return rt;
			}
			else if(wXPos >= rc.x + (rc.dx/3)*2 && wXPos < rc.x + (rc.dx/3)*3 )//��
			{						
				boolean rt = ISHELL_PostEvent(pMe->m_pShell,AEECLSID_MAIN_MENU,EVT_USER,AVK_CLR,0);
				return rt;
			}
		}
		return TRUE;
	}

	//���㽹��ͼƬ������
	if((wXPos > pMe->m_IconFocus_Pt[0].x) && (wXPos < pMe->m_IconFocus_Pt[0].x + ICON_ANIMATED_WIDTH))
	{
		iconSpaceVertical = 0;
	}
	else if((wXPos > pMe->m_IconFocus_Pt[1].x) && (wXPos < pMe->m_IconFocus_Pt[1].x + ICON_ANIMATED_WIDTH))
	{
		iconSpaceVertical = 1;
	}
	else if((wXPos > pMe->m_IconFocus_Pt[2].x) && (wXPos < pMe->m_IconFocus_Pt[2].x + ICON_ANIMATED_WIDTH))
	{
		iconSpaceVertical = 2;
	}

	if((wYPos > pMe->m_IconFocus_Pt[0].y) && (wYPos < pMe->m_IconFocus_Pt[0].y + ICON_ANIMATED_HEIGHT))
	{
		iconSpaceHorizontal = 0;
	}
	else if((wYPos > pMe->m_IconFocus_Pt[4].y) && (wYPos < pMe->m_IconFocus_Pt[4].y + ICON_ANIMATED_HEIGHT))
	{
		iconSpaceHorizontal = 1;
	}
	else if((wYPos > pMe->m_IconFocus_Pt[7].y) && (wYPos < pMe->m_IconFocus_Pt[7].y + ICON_ANIMATED_HEIGHT))
	{
		iconSpaceHorizontal = 2;
	}
	else if((wYPos > pMe->m_IconFocus_Pt[10].y) && (wYPos < pMe->m_IconFocus_Pt[10].y + ICON_ANIMATED_HEIGHT))
	{
		iconSpaceHorizontal = 3;
	}
	
	if((pMe->m_nRow != iconSpaceHorizontal) || (pMe->m_nColumn != iconSpaceVertical))//��ǰ���ǽ�����ֻ�ƶ�����
	{
		DrawMatrix(pMe);

		MoveCursorTo(pMe, iconSpaceHorizontal, iconSpaceVertical);

		return TRUE;
	}
	
	{		
		int i;
		ISHELL_CancelTimer (pMe->m_pShell, (PFNNOTIFY)DrawFocusIconAnimation, pMe);//������Ϊ�˷�ֹ����û�л����ȡ���������Ķ�ʱ��
		if (pMe->m_pAnimate != NULL)
        {
            IIMAGE_Release(pMe->m_pAnimate);
            pMe->m_pAnimate = NULL;
        }
		i = iconSpaceHorizontal * 3 + iconSpaceVertical;
		StartApplet(pMe, i);
	}

	return TRUE;
}
#endif//FEATURE_LCD_TOUCH_ENABLE

/*=============================================================================
FUNCTION:  MainMenu_IconMenuHandler

DESCRIPTION: 12����˵�

PARAMETERS:

=============================================================================*/
static boolean MainMenu_IconMenuHandler(MainMenu *pMe, AEEEvent eCode, uint16 wParam, uint32 dwParam)
{
    PARAM_NOT_REF( dwParam)
    switch ( eCode)
    {
    
#ifdef FEATURE_LCD_TOUCH_ENABLE//WLH ADD FOR LCD TOUCH
		case EVT_PEN_UP:
			return movepen_anddo(pMe,dwParam);
#endif//FEATURE_LCD_TOUCH_ENABLE

        case EVT_DIALOG_INIT:
            return TRUE;

        case EVT_DIALOG_START:
#ifdef FEATURE_RANDOM_MENU_COLOR
            (void)OEM_GetConfig(CFGI_MENU_BGCOLOR, &pMe->m_nBgColor, sizeof(pMe->m_nBgColor));
#endif
            if(pMe->m_pImageBg == NULL)
            {
#ifdef FEATURE_RANDOM_MENU_COLOR
                (void)OEM_GetConfig(CFGI_RANDOM_MENU, (void*)&pMe->m_nRandomMenu, sizeof(pMe->m_nRandomMenu));
                if(pMe->m_nRandomMenu != 0)
                {
                    pMe->m_pImageBg = ISHELL_LoadResImage(pMe->m_pShell, AEE_APPSCOMMONRES_IMAGESFILE, IDI_MENU_BACKGROUND_TRANS);
                }
                else
#endif
                {
                    pMe->m_pImageBg = ISHELL_LoadResImage(pMe->m_pShell, AEE_APPSCOMMONRES_IMAGESFILE, IDI_MENU_BACKGROUND);
                }
            }
            (void) ISHELL_PostEvent( pMe->m_pShell,
                                     AEECLSID_MAIN_MENU,
                                     EVT_USER_REDRAW,
                                     0,
                                     0);
            return TRUE;


        case EVT_USER_REDRAW:
            // ��ʼ����������ȫ����ʼͼ��
            DrawMatrix(pMe);
#ifdef FEATURE_ICON_MOVE_ANIMATION
            {
                IBitmap *pTempBmp = NULL;

                IDISPLAY_GetDeviceBitmap(pMe->m_pDisplay, &pTempBmp);
                if(NULL != pTempBmp)
                {
                    if(NULL == pMe->m_pDevImage)
                    {
                        IBITMAP_CreateCompatibleBitmap(pTempBmp, &pMe->m_pDevImage, pMe->m_rc.dx, pMe->m_rc.dy);
                    }
                    IBITMAP_BltIn(pMe->m_pDevImage, 0, 0, pMe->m_rc.dx, pMe->m_rc.dy, pTempBmp, 0, 0, AEE_RO_COPY);
                    IBITMAP_Release(pTempBmp); //added by chengxiao 2009.04.20
                }
            }
#endif
            
            // ���ƾ۽����̶���
            MoveCursorTo(pMe, pMe->m_nRow, pMe->m_nColumn);
            return TRUE;
            
        case EVT_DIALOG_END:
            {
                int i;
                
                if (pMe->m_pImageBg !=NULL)
                {
                    (void) IIMAGE_Release(pMe->m_pImageBg);
                    pMe->m_pImageBg = NULL;
                }

                for(i=0;i<12;i++)
                {
                    if(pMe->m_pImageIcon[i] != NULL)
                    {
                        (void)IIMAGE_Release(pMe->m_pImageIcon[i]);
                        pMe->m_pImageIcon[i] = NULL;
                    }
                } 
                
                if(pMe->m_pAnimate != NULL)
                {
                    (void)IIMAGE_Release(pMe->m_pAnimate);
                    pMe->m_pAnimate = NULL;
                }                
#ifdef FEATURE_ICON_MOVE_ANIMATION
                pMe->m_bMoveing = FALSE;
                if(pMe->m_pDevImage != NULL)
                {
                    (void)IBitmap_Release(pMe->m_pDevImage);
                    pMe->m_pDevImage = NULL;
                }
#endif
                ISHELL_CancelTimer(pMe->m_pShell, NULL, (void**)pMe);
            }
            return TRUE;


        case EVT_KEY:
            //ISHELL_CancelTimer(pMe->m_pShell, NULL, (void**)pMe);
            switch( wParam)
            {
                case AVK_CLR:
                    CLOSE_DIALOG(DLGRET_CANCELED)
                    return TRUE;
                    
                case AVK_UP:
                    {
                        int nRow = pMe->m_nRow;
                        int nCol = pMe->m_nColumn;
                        
                        if (nRow>0)
                        {
                            nRow--;
                        }
                        else
                        {
                            if (nCol>0)
                            {
                                nCol--;
                            }
                            else
                            {
                                nCol = MAX_MATRIX_COLS-1;
                            }
                            nRow = MAX_MATRIX_ROWS-1;
                        }
                        MoveCursorTo(pMe, nRow, nCol);
                    }
                    return TRUE;
                    
                case AVK_DOWN:
                    {
                        int nRow = pMe->m_nRow;
                        int nCol = pMe->m_nColumn;
                        
                        if (nRow<MAX_MATRIX_ROWS-1)
                        {
                            nRow++;
                        }
                        else
                        {
                            if (nCol<MAX_MATRIX_COLS-1)
                            {
                                nCol++;
                            }
                            else
                            {
                                nCol = 0;
                            }
                            nRow = 0;
                        }
                        MoveCursorTo(pMe, nRow, nCol);
                    }
                    return TRUE;
                    
                case AVK_LEFT:
                    {
                        int nRow = pMe->m_nRow;
                        int nCol = pMe->m_nColumn;
                        
                        if (nCol>0)
                        {
                            nCol--;
                        }
                        else
                        {
                            if (nRow>0)
                            {
                                nRow--;
                            }
                            else
                            {
                                nRow = MAX_MATRIX_ROWS-1;
                            }
                            nCol = MAX_MATRIX_COLS-1;
                        }
                        MoveCursorTo(pMe, nRow, nCol);
                    }
                    return TRUE;
                    
                case AVK_RIGHT:
                    {
                        int nRow = pMe->m_nRow;
                        int nCol = pMe->m_nColumn;
                        
                        if (nCol<MAX_MATRIX_COLS-1)
                        {
                            nCol++;
                        }
                        else
                        {
                            if (nRow<MAX_MATRIX_ROWS-1)
                            {
                                nRow++;
                            }
                            else
                            {
                                nRow = 0;
                            }
                            nCol = 0;
                        }
                        MoveCursorTo(pMe, nRow, nCol);
                    }
                    return TRUE;
                    
                case AVK_SELECT:
                case AVK_INFO:
                    {
                        int Focus = pMe->m_nRow * 3 + pMe->m_nColumn;
                        StartApplet(pMe, Focus);
                    }
                    return TRUE;

                case AVK_1:
                case AVK_2:
                case AVK_3:
                case AVK_4:
                case AVK_5:
                case AVK_6:
                case AVK_7:
                case AVK_8:
                case AVK_9:
                case AVK_STAR:
                    {
                        int Focus = (wParam - AVK_1);
                        StartApplet(pMe, Focus);
                    }
                    return TRUE;

                case AVK_0:
                    StartApplet(pMe, 10);
                    return TRUE;
                    
                case AVK_POUND:
                    StartApplet(pMe, 11);
                    return TRUE;

                default:
                    break;
            }

            return TRUE;
            
        default:
            break;
    }

    return FALSE;
}

/*=============================================================================
FUNCTION:  calculateScreenParameters

DESCRIPTION: ���㽹��ͼƬ�Լ�Ĭ��ͼƬ������

=============================================================================*/
static void calculateScreenParameters(MainMenu *pMe)
{
    AEEImageInfo    imageInfoIcon;
    uint16          iconSpaceHorizontal = 0;
    uint16          iconSpaceVertical   = 0;
    uint8           i                   = 0;

    //����Ĭ��ͼƬ������
    imageInfoIcon.cx = ICON_WIDTH;
    imageInfoIcon.cy = ICON_HEIGHT;

    //modified by chengxiao 2009.04.02
    /* icon size in all might be larger than screen*/
    if(pMe->m_rc.dx > imageInfoIcon.cx * MAX_MATRIX_COLS)
    {
        iconSpaceHorizontal = (pMe->m_rc.dx - imageInfoIcon.cx * MAX_MATRIX_COLS) / (MAX_MATRIX_COLS+1);
    }
    else
    {
        iconSpaceHorizontal = 0;
    }
    
    if(pMe->m_rc.dy > TITLEBAR_HEIGHT + imageInfoIcon.cy * MAX_MATRIX_ROWS)
    {
        iconSpaceVertical   = (pMe->m_rc.dy - TITLEBAR_HEIGHT - imageInfoIcon.cy * MAX_MATRIX_ROWS) / (MAX_MATRIX_ROWS+1);
    }
    else
    {
        iconSpaceVertical = 0;
    }
    //chengxiao modify end 2009.04.02
    
    for( i = 0; i < MAX_MATRIX_ITEMS; i ++)
    {
        pMe->m_Icondefault_Pt[i].x = iconSpaceHorizontal +
            ( imageInfoIcon.cx + iconSpaceHorizontal) * ( i % MAX_MATRIX_COLS);

        pMe->m_Icondefault_Pt[i].y = TITLEBAR_HEIGHT + iconSpaceVertical +
            ( imageInfoIcon.cy + iconSpaceVertical) * ( i / MAX_MATRIX_COLS);
        //added by chengxiao 2009.04.02
        //���㽹��ͼƬ������
        pMe->m_IconFocus_Pt[i].x = pMe->m_Icondefault_Pt[i].x - (ICON_ANIMATED_WIDTH - imageInfoIcon.cx)/2;

        pMe->m_IconFocus_Pt[i].y = pMe->m_Icondefault_Pt[i].y - (ICON_ANIMATED_HEIGHT- imageInfoIcon.cy)/2;
        //end added
    }
}

static void MainMenu_DrawBackGround(MainMenu *pMe, AEERect *pRect)
{
#ifdef FEATURE_RANDOM_MENU_COLOR
    IDISPLAY_FillRect(pMe->m_pDisplay, pRect, pMe->m_nBgColor);
    if(pMe->m_nRandomMenu != 0)
    {
        Appscommon_ResetBackground(pMe->m_pDisplay, 
                                                    pMe->m_pImageBg, 
                                                    pMe->m_nBgColor, 
                                                    pRect, 
                                                    APPSCOMMON_MENUBG_XPOS, 
                                                    APPSCOMMON_MENUBG_YPOS);
    }
    else
#endif
    {
        Appscommon_ResetBackground(pMe->m_pDisplay, 
                                                    pMe->m_pImageBg, 
                                                    APPSCOMMON_BG_COLOR, 
                                                    pRect, 
                                                    0, 
                                                    0);
    }
}

/*=============================================================================
FUNCTION:  calculateScreenParameters

DESCRIPTION: // ��ʼ����������ȫ����ʼͼ��

=============================================================================*/
static void DrawMatrix(MainMenu *pMe)
{
    int i = 0;
	BottomBar_Param_type BarParam={0};//wlh add
    
    if (NULL == pMe)
    {
        return;
    }
    //draw bg image

    MainMenu_DrawBackGround(pMe, &pMe->m_rc); //modified by chengxiao 2009.04.10

    //Draw icon
    for (i = 0; i < MAX_MATRIX_ITEMS; i ++)
    {
        if (pMe->m_pImageIcon[i] == NULL)
        {
            pMe->m_pImageIcon[i] = ISHELL_LoadResImage(pMe->m_pShell,
                                                    MAINMENU_RES_FILE_IMAGE,
                                                    IDI_MAIN_MENU_LARGE_ICON_1 + i);
        }

#ifdef FEATURE_APP_NUMBERMANAGER
        if(i == 9)
        {
            MainMenu_DraImage(pMe,i,IDI_MAIN_MENU_LARGE_ICON_NUMBER_10);
        }
#endif

#ifdef FEATURE_CARRIER_CHINA_TELCOM
        if(i == 3)
        {
            MainMenu_DraImage(pMe,i,IDI_MAIN_MENU_LARGE_ICON_5);
        }

        if(i == 4)
        {
            MainMenu_DraImage(pMe,i,IDI_MAIN_MENU_LARGE_ICON_ESURFING);
            }

        if(i == 5)
        {
            MainMenu_DraImage(pMe,i,IDI_MAIN_MENU_LARGE_ICON_NUMBER_10);
            }

        if(i == 7)
            {
            MainMenu_DraImage(pMe,i,IDI_MAIN_MENU_LARGE_ICON_6);

            }
                
        if(i == 9)
        {
            MainMenu_DraImage(pMe,i,IDI_MAIN_MENU_LARGE_ICON_CUSTSERVICE);
            }

        if(i == 11)
        {
            MainMenu_DraImage(pMe,i,IDI_MAIN_MENU_LARGE_ICON_8);
        }
#endif
        if (pMe->m_pImageIcon[i] != NULL)
        {
            IIMAGE_Draw(pMe->m_pImageIcon[i],
                        pMe->m_Icondefault_Pt[i].x,
                        pMe->m_Icondefault_Pt[i].y);
        }
    }  

    BarParam.eBBarType = BTBAR_SELECT_BACK;
    DrawBottomBar(pMe->m_pDisplay, &BarParam);//wlh 20090412 add
}

#ifdef FEATURE_ICON_MOVE_ANIMATION
static void DrawFocusMoveAnimation(MainMenu * pMe)
{
    static int nFrame = 0;
    int theFocus = pMe->m_nRow * 3 + pMe->m_nColumn,
        thePrevFocus = pMe->m_nPrevRow * 3 + pMe->m_nPrevColumn,
        xOldPos = pMe->m_IconFocus_Pt[thePrevFocus].x + (nFrame)*(pMe->m_IconFocus_Pt[theFocus].x - pMe->m_IconFocus_Pt[thePrevFocus].x)/ICON_ANIMATED_MOVE_FRAME, 
        yOldPos = pMe->m_IconFocus_Pt[thePrevFocus].y + (nFrame)*(pMe->m_IconFocus_Pt[theFocus].y - pMe->m_IconFocus_Pt[thePrevFocus].y)/ICON_ANIMATED_MOVE_FRAME;

    ISHELL_CancelTimer(pMe->m_pShell, (PFNNOTIFY)DrawFocusMoveAnimation, pMe);
    
    if(pMe->m_pAnimate == NULL)
    {
        nFrame = 0;
        pMe->m_pAnimate = ISHELL_LoadImage(pMe->m_pShell, ICON_ANI[theFocus]);
    }

    if(nFrame < (ICON_ANIMATED_MOVE_FRAME - 1) && pMe->m_pAnimate != NULL && (theFocus != thePrevFocus))
    {
        AEERect rect = {0};
        int xPos = pMe->m_IconFocus_Pt[thePrevFocus].x + (nFrame + 1)*(pMe->m_IconFocus_Pt[theFocus].x - pMe->m_IconFocus_Pt[thePrevFocus].x)/ICON_ANIMATED_MOVE_FRAME,
            yPos = pMe->m_IconFocus_Pt[thePrevFocus].y + (nFrame + 1)*(pMe->m_IconFocus_Pt[theFocus].y - pMe->m_IconFocus_Pt[thePrevFocus].y)/ICON_ANIMATED_MOVE_FRAME;

        if(pMe->m_pDevImage != NULL)
        {
            IDISPLAY_BitBlt(pMe->m_pDisplay, xOldPos, yOldPos,
                                ICON_ANIMATED_WIDTH, ICON_ANIMATED_HEIGHT, 
                                pMe->m_pDevImage, xOldPos, yOldPos, AEE_RO_COPY);
        }
        pMe->m_bMoveing = TRUE;
        SETAEERECT(&rect, xPos, yPos, ICON_ANIMATED_WIDTH, ICON_ANIMATED_HEIGHT);
        
        Appscommon_ResetBackground(pMe->m_pDisplay,pMe->m_pAnimate, 
#ifdef FEATURE_RANDOM_MENU_COLOR
                                                    pMe->m_nBgColor, 
#else
                                                    APPSCOMMON_BG_COLOR,
#endif
                                                    &rect, rect.x, rect.y);
        IDISPLAY_Update(pMe->m_pDisplay);
        nFrame++;
        ISHELL_SetTimer(pMe->m_pShell, ICON_ANIMATED_MOVE_RATE, (PFNNOTIFY)DrawFocusMoveAnimation, pMe);
    }
    else
    {
        nFrame = 0;
        pMe->m_bMoveing = FALSE;
        if(pMe->m_pDevImage != NULL)
        {
            IDISPLAY_BitBlt(pMe->m_pDisplay, xOldPos, yOldPos,
                                ICON_ANIMATED_WIDTH, ICON_ANIMATED_HEIGHT, 
                                pMe->m_pDevImage, xOldPos, yOldPos, AEE_RO_COPY);
        }
#ifdef FEATURE_FOCUS_ANIMATION
        DrawFocusIconAnimation(pMe);
#else
        DrawFocusIcon(pMe);
#endif
    }

}
#endif

#ifdef FEATURE_FOCUS_ANIMATION
/*=============================================================================
FUNCTION:  DrawFocusIconAnimation

DESCRIPTION: 

PARAMETERS:

RETURN VALUE:

COMMENTS:


=============================================================================*/
static void DrawFocusIconAnimation(MainMenu *pMe)
{
    int theFocus = pMe->m_nRow * 3 + pMe->m_nColumn;
    
    if (NULL == pMe)
    {
        return;
    }
            
    if (pMe->m_nIconAniFrameIdx == 0)
    {
        TitleBar_Param_type  titleBarParms;
        MEMSET( &titleBarParms, 0, sizeof( TitleBar_Param_type));
            
        titleBarParms.dwAlignFlags  = IDF_TEXT_TRANSPARENT | IDF_ALIGN_CENTER | IDF_ALIGN_MIDDLE;
        STRCPY( titleBarParms.strTitleResFile, MAINMENU_RES_FILE_LANG);
        titleBarParms.nTitleResID   = IDS_MAIN_MENU_TITLE_1 + theFocus;

#ifdef FEATURE_APP_NUMBERMANAGER
        if(theFocus == 9)
        {
            titleBarParms.nTitleResID = IDS_MAIN_MENU_TITLE_13;
        }
#endif

#ifdef FEATURE_CARRIER_CHINA_TELCOM
        if(theFocus == 3)
        {
            titleBarParms.nTitleResID = IDS_MAIN_MENU_TITLE_5;
        }

        if(theFocus == 4)
        {
            titleBarParms.nTitleResID = IDS_MAIN_MENU_TITLE_ESURFING;
        }

        if(theFocus == 5)
        {
            titleBarParms.nTitleResID = IDS_MAIN_MENU_TITLE_13;
        }

        if(theFocus == 7)
        {
            titleBarParms.nTitleResID = IDS_MAIN_MENU_TITLE_6;
        }  
        
        if(theFocus == 9)
        {
            titleBarParms.nTitleResID = IDS_MAIN_MENU_TITLE_CUSTSERVICE;
        } 

        if(theFocus == 11)
        {
            titleBarParms.nTitleResID = IDS_MAIN_MENU_TITLE_8;
        }
        
#endif /*FEATURE_CARRIER_CHINA_TELCOM*/

        DrawTitleBar(pMe->m_pDisplay, &titleBarParms);
    }
    
    if(pMe->m_pAnimate == NULL)
    {
        pMe->m_pAnimate = ISHELL_LoadImage(pMe->m_pShell, ICON_ANI[theFocus]);
    }
/*wlh 20090410 ��ʱ����
    if( pMe->m_pAnimate != NULL)
    {
        IIMAGE_SetParm(pMe->m_pAnimate, IPARM_NFRAMES, ICON_ANIMATED_FRAME, 0);        
        IIMAGE_DrawFrame(pMe->m_pAnimate, 
                        pMe->m_nIconAniFrameIdx + 1, 
                        pMe->m_IconFocus_Pt[theFocus].x, 
                        pMe->m_IconFocus_Pt[theFocus].y);

    }
    IDISPLAY_UpdateEx(pMe->m_pDisplay, TRUE);
    if(pMe->m_nIconAniFrameIdx < ICON_ANIMATED_FRAME - 1)
    {
        ISHELL_SetTimer (pMe->m_pShell,
                        120 - pMe->m_nIconAniFrameIdx * 20,
                        (PFNNOTIFY)DrawFocusIconAnimation,
                        pMe);

        ++pMe->m_nIconAniFrameIdx;
    }
    else
    {
        if (pMe->m_pAnimate != NULL)
        {
            IIMAGE_Release(pMe->m_pAnimate);
            pMe->m_pAnimate = NULL;
        }
    }
*/
	//wlh 20090410 add start
	if( pMe->m_pAnimate != NULL)
   {
		IIMAGE_Draw(pMe->m_pAnimate,
                         pMe->m_IconFocus_Pt[theFocus].x, 
                        pMe->m_IconFocus_Pt[theFocus].y);
	   IIMAGE_Release(pMe->m_pAnimate);
       pMe->m_pAnimate = NULL;
	   IDISPLAY_UpdateEx(pMe->m_pDisplay, TRUE);
	}
	//wlh 20090410 add end
}
#else
/*=============================================================================
FUNCTION:  DrawFocusIconAnimation

DESCRIPTION: 

PARAMETERS:

RETURN VALUE:

COMMENTS:


=============================================================================*/
static void DrawFocusIcon(MainMenu *pMe)
{
    int theFocus = pMe->m_nRow * 3 + pMe->m_nColumn;
    TitleBar_Param_type  titleBarParms;
    
    if (NULL == pMe)
    {
        return;
    }
            
    MEMSET( &titleBarParms, 0, sizeof( TitleBar_Param_type));
        
    titleBarParms.dwAlignFlags  = IDF_TEXT_TRANSPARENT | IDF_ALIGN_CENTER | IDF_ALIGN_MIDDLE;
    STRCPY( titleBarParms.strTitleResFile, MAINMENU_RES_FILE_LANG);
    titleBarParms.nTitleResID   = IDS_MAIN_MENU_TITLE_1 + theFocus;

#ifdef FEATURE_APP_NUMBERMANAGER
    if(theFocus == 9)
    {
        titleBarParms.nTitleResID = IDS_MAIN_MENU_TITLE_13;
    }
#endif
#ifdef FEATURE_CARRIER_CHINA_TELCOM
    if(theFocus == 3)
    {
        titleBarParms.nTitleResID = IDS_MAIN_MENU_TITLE_5;
    }

    if(theFocus == 4)
    {
        titleBarParms.nTitleResID = IDS_MAIN_MENU_TITLE_ESURFING;
    }

    if(theFocus == 5)
    {
        titleBarParms.nTitleResID = IDS_MAIN_MENU_TITLE_13;
    }

    if(theFocus == 7)
    {
        titleBarParms.nTitleResID = IDS_MAIN_MENU_TITLE_6;
    }  
    
    if(theFocus == 9)
    {
        titleBarParms.nTitleResID = IDS_MAIN_MENU_TITLE_CUSTSERVICE;
    } 

    if(theFocus == 11)
    {
        titleBarParms.nTitleResID = IDS_MAIN_MENU_TITLE_8;
    }
        
#endif /*FEATURE_CARRIER_CHINA_TELECOM*/
    DrawTitleBar(pMe->m_pDisplay, &titleBarParms);
    
    if(pMe->m_pAnimate == NULL)
    {
        pMe->m_pAnimate = ISHELL_LoadImage(pMe->m_pShell, ICON_ANI[theFocus]);
    }
/*
    if( pMe->m_pAnimate != NULL)
    {
        AEERect rect = {0};

        SETAEERECT(&rect, pMe->m_IconFocus_Pt[theFocus].x, pMe->m_IconFocus_Pt[theFocus].y, 
                                    ICON_ANIMATED_WIDTH, ICON_ANIMATED_HEIGHT);
        Appscommon_ResetBackground(pMe->m_pDisplay,pMe->m_pAnimate, 
#ifdef FEATURE_RANDOM_MENU_COLOR
                                                    pMe->m_nBgColor, 
#else
                                                    APPSCOMMON_BG_COLOR,
#endif
                                                    &rect, rect.x, rect.y);
        IIMAGE_Release(pMe->m_pAnimate);
        pMe->m_pAnimate = NULL;
    }
    IDISPLAY_Update(pMe->m_pDisplay);
*/
	if( pMe->m_pAnimate != NULL)
   {
		IIMAGE_Draw(pMe->m_pAnimate,
                         pMe->m_IconFocus_Pt[theFocus].x, 
                        pMe->m_IconFocus_Pt[theFocus].y);
	   IIMAGE_Release(pMe->m_pAnimate);
       pMe->m_pAnimate = NULL;
	   IDISPLAY_UpdateEx(pMe->m_pDisplay, TRUE);
	}
	//wlh 20090410 add end
	}
}
//end added
#endif

/*=============================================================================
FUNCTION:  MoveCursorTo

DESCRIPTION: 

PARAMETERS:

RETURN VALUE:

COMMENTS:

SIDE EFFECTS:

SEE ALSO:

=============================================================================*/
static void MoveCursorTo(MainMenu *pMe, int row, int column)
{
    int theFocus = pMe->m_nRow * 3 + pMe->m_nColumn;
    AEERect rect;
	
//#ifdef FEATURE_LCD_TOUCH_ENABLE//WLH ADD FOR LCD TOUCH
	//IImage      *m_pImageTurn[MAX_TURN_NUM];
	int i;
	AEERect turnrect;
	//int rowflag;//0 �����У�1��ǰ�У�2������
	//int columnflag;//0����У�1��ǰ�У�2�ұ���
	//int startx,starty;//ʵ�ʹ���ͼƬ�����50��50���ƫ��
	int lengthX,lengthY;//��һ������ľ���
	int lengthXX,lengthYY;//��һ������ľ���,����ֵ
	int turnnum;//��ǰҪ�������ٴ�
	int turnlengthx = 0;//ÿ��x�ƶ��ľ���
	int turnlengthy = 0;//ÿ��y�ƶ��ľ���
	int turnx = 0;//ÿ��x�ƶ��ľ�����
	int turny = 0;//ÿ��y�ƶ��ľ�����
	int x,y;
	IImage		*m_pImageTurns[4];//��ŵ�ǰЧ����Դ
	//wlh add 0404
    TitleBar_Param_type  titleBarParms;
    MEMSET( &titleBarParms, 0, sizeof( TitleBar_Param_type));
            
    titleBarParms.dwAlignFlags  = IDF_TEXT_TRANSPARENT | IDF_ALIGN_CENTER | IDF_ALIGN_MIDDLE;
    STRCPY( titleBarParms.strTitleResFile, MAINMENU_RES_FILE_LANG);
    titleBarParms.nTitleResID   = IDS_MAIN_MENU_TITLE_1 + theFocus;
#ifdef FEATURE_APP_NUMBERMANAGER
    if(theFocus == 9)
    {
        titleBarParms.nTitleResID = IDS_MAIN_MENU_TITLE_13;
    }
#endif
	//wlh end

	if((row != pMe->m_nRow) || (column != pMe->m_nColumn))//�����Ѿ��ǽ�������
	{
 
	  	DrawMatrix(pMe);
		DrawTitleBar(pMe->m_pDisplay, &titleBarParms);	//wlh add 0404
	/*
		if(row > pMe->m_nRow)
		{
			starty = 7;//(50-36)/2
		}			
		if(row == pMe->m_nRow)
		{
			starty = 7;//(50-36)/2
		}
		if(row < pMe->m_nRow)
		{
			starty = -7;//(50-36)/2
		}
		lengthY = ICON_ANIMATED_HEIGHT * (row - pMe->m_nRow);
	
		if(column > pMe->m_nColumn)
		{
			startx = 8 + 4;//(50 - 34)/2 
		}			
		if(column == pMe->m_nColumn)
		{
			startx = 8;//(50 - 34)/2 
		}			
		if(column < pMe->m_nColumn)
		{
			startx = 8 - 4;//(50 - 34)/2 
		}*/
		lengthX = ICON_ANIMATED_WIDTH * (column - pMe->m_nColumn);
	    lengthY = ICON_ANIMATED_HEIGHT* (row - pMe->m_nRow);
	
		turnrect.x	= pMe->m_IconFocus_Pt[theFocus].x;// + startx;// 8 + 4;//(50 - 34)/2 
		turnrect.y  = pMe->m_IconFocus_Pt[theFocus].y;// + starty;//7;//(50-36)/2
		turnrect.dx = ICON_ANIMATED_WIDTH;
		turnrect.dy = ICON_ANIMATED_HEIGHT;
	
		if(lengthX < 0)
		{
			lengthXX = 0 - lengthX;
		}
		else
		{
			lengthXX = lengthX;
		}
		if(lengthY < 0)
		{
			lengthYY = 0 - lengthY;
		}
		else
		{
			lengthYY = lengthY;
		}
	
		if(lengthXX > lengthYY)
		{
		  	turnnum = lengthXX/9;
		}
		else
		{
			turnnum = lengthYY/9;
		}

		turnlengthx = lengthXX/5;
		turnlengthy = lengthYY/5;
		//m_pImageTurns[0] = ISHELL_LoadImage(pMe->m_pShell,ICON_ANI_1[theFocus]);
		//m_pImageTurns[1] = ISHELL_LoadImage(pMe->m_pShell,ICON_ANI_2[theFocus]);
		//m_pImageTurns[2] = ISHELL_LoadImage(pMe->m_pShell,ICON_ANI_2[row * 3 + column]);
		//m_pImageTurns[3] = ISHELL_LoadImage(pMe->m_pShell,ICON_ANI_1[row * 3 + column]);

		m_pImageTurns[0] = pMe->m_pImageTurn[theFocus];
		m_pImageTurns[1] = pMe->m_pImageTurn[theFocus + 12];
		m_pImageTurns[2] = pMe->m_pImageTurn[row * 3 + column + 12];
		m_pImageTurns[3] = pMe->m_pImageTurn[row * 3 + column];

		turnnum = 4;


		
		for (i = 0; i < turnnum; i ++)
		{
			//if(i>4)
			//	IIMAGE_Draw(pMe->m_pImageTurn[i%5], turnrect.x, turnrect.y);
			//else
			//	IIMAGE_Draw(pMe->m_pImageTurn[i], turnrect.x, turnrect.y);
			
			if((i == 0) || (i == 3))
			{
				turnx = 8;
				turny = 8;
			}
			else
			{
				turnx = 18;
				turny = 18;
			}
			x = turnrect.x;
			y = turnrect.y;
	
			if(lengthXX > lengthYY)//��x��Ϊ����
			{
				if(lengthX > 0)
				{
					x = x + turnlengthx*(i+1) + turnx;
					if(lengthY != 0)
					{
						//turnrect.y = turnrect.y + lengthY/turnnum;
						turnlengthy = lengthY/4;
						
					}
					y = y + turnlengthy*(i+1) + turny;
				}					
				else
				{
					x = x - turnlengthx*(i+1) - turnx;
					if(lengthY != 0)
					{
						//turnrect.y = turnrect.y + lengthY/turnnum;
						turnlengthy = lengthY/4;
					}
					y = y + turnlengthy*(i+1) + turny;
				}
			}
			else//��y��Ϊ����
			{
				if(lengthY > 0)
				{
					y = y + turnlengthy*(i+1) + turny;
					if(lengthX != 0)
					{
						//turnrect.x = turnrect.x + lengthX/turnnum;
						turnlengthx = lengthX/4;
					}
					x = x + turnlengthx*(i+1) + turnx;
				}					
				else
				{
					y = y - turnlengthy*(i+1) - turny;
					if(lengthX != 0)
					{
						//turnrect.x = turnrect.x + lengthX/turnnum;
						turnlengthx = lengthX/4;
					}
					x = x + turnlengthx*(i+1) + turnx;
				}
			}	
			IIMAGE_Draw(m_pImageTurns[i], x, y);
			IDISPLAY_UpdateEx(pMe->m_pDisplay,FALSE);
			//AEEOS_Sleep(1000);
			DrawMatrix(pMe);
			DrawTitleBar(pMe->m_pDisplay, &titleBarParms);	//wlh add 0404
		}
		//DrawMatrix(pMe);
		//IDISPLAY_UpdateEx(pMe->m_pDisplay,FALSE);
		//for (i = 0; i < MAX_TURN_NUM; i ++)
		//{
		//	IImage_Release(m_pImageTurn[i]);
		//}
		//for (i = 0; i < turnnum; i ++)
		//{
		//	IImage_Release(m_pImageTurns[i]);
		//}
	}
//#endif//FEATURE_LCD_TOUCH_ENABLE

    ISHELL_CancelTimer (pMe->m_pShell, NULL, ( void*)&pMe);
    
    // ���ƾ۽�������ʼ����
    SETAEERECT(&rect, pMe->m_IconFocus_Pt[theFocus].x, 
                                pMe->m_IconFocus_Pt[theFocus].y, 
                                ICON_ANIMATED_WIDTH, 
                                ICON_ANIMATED_HEIGHT);
#ifdef FEATURE_ICON_MOVE_ANIMATION
    if(pMe->m_bMoveing)
    {
        pMe->m_bMoveing = FALSE;
        if(pMe->m_pDevImage != NULL)
        {
            IDISPLAY_BitBlt(pMe->m_pDisplay, 0, TITLEBAR_HEIGHT,
                            pMe->m_rc.dx, pMe->m_rc.dy - TITLEBAR_HEIGHT, 
                            pMe->m_pDevImage, 0, TITLEBAR_HEIGHT, AEE_RO_COPY);
        }
    }
    else
#endif
    {
        MainMenu_DrawBackGround(pMe, &rect);
    
        if (pMe->m_pImageIcon[theFocus])
        {
            IIMAGE_Draw(pMe->m_pImageIcon[theFocus],
                            pMe->m_Icondefault_Pt[theFocus].x, 
                            pMe->m_Icondefault_Pt[theFocus].y);
        }
    }
    // ��ʼ�۽���������
#ifdef FEATURE_LCD_TOUCH_ENABLE//WLH ADD FOR LCD TOUCH
	if (pMe->m_pAnimate != NULL)
	{
		IIMAGE_Release(pMe->m_pAnimate);
		pMe->m_pAnimate = NULL;
	}
#endif//FEATURE_LCD_TOUCH_ENABLE
    // ��ʼ�۽���������
    setCursor(pMe, row, column);

#ifdef FEATURE_ICON_MOVE_ANIMATION
    DrawFocusMoveAnimation(pMe);
#else //FEATURE_ICON_MOVE_ANIMATION
#ifdef FEATURE_FOCUS_ANIMATION
    DrawFocusIconAnimation(pMe);
#else //FEATURE_FOCUS_ANIMATION
    DrawFocusIcon(pMe);
#endif
#endif
}
/*=============================================================================
FUNCTION:  setCursor

DESCRIPTION: 

PARAMETERS:

RETURN VALUE:

COMMENTS:

SIDE EFFECTS:

SEE ALSO:

=============================================================================*/
static void setCursor(MainMenu *pMe, int row, int column)
{
#ifdef FEATURE_ICON_MOVE_ANIMATION
    pMe->m_nPrevRow   = pMe->m_nRow;
    pMe->m_nPrevColumn = pMe->m_nColumn;
#endif
    pMe->m_nRow        = row;
    pMe->m_nColumn     = column;
#ifdef FEATURE_FOCUS_ANIMATION
    pMe->m_nIconAniFrameIdx      = 0;
#endif
    if(pMe->m_pAnimate != NULL)
    {
        IImage_Release(pMe->m_pAnimate);
        pMe->m_pAnimate  = NULL;
    }
}


/*=============================================================================
FUNCTION:  MainMenu_GameMenuHanlder

DESCRIPTION:   ��Ϸ�˵��б�

PARAMETERS:

=============================================================================*/
#ifdef FEATURE_SPORTS_APP
    
    extern boolean is_g_sportsapp_pedometer_bground_flag();
#endif

static boolean MainMenu_GameMenuHanlder(MainMenu *pMe, AEEEvent eCode, uint16 wParam, uint32 dwParam)
{
    PARAM_NOT_REF(dwParam)
    IMenuCtl *pMenu = (IMenuCtl*)IDIALOG_GetControl(pMe->m_pActiveIDlg,IDC_GAME_MENU);
        
    if (pMenu == NULL)
    {
        return FALSE;
    }

    switch (eCode)
    {
#ifdef FEATURE_LCD_TOUCH_ENABLE//WLH ADD FOR LCD TOUCH
		case EVT_PEN_UP:
		//case EVT_PEN_DOWN:
		{
			return IMENUCTL_HandleEvent(pMenu,eCode,wParam,dwParam);
		}
#endif//FEATURE_LCD_TOUCH_ENABLE

        case EVT_DIALOG_INIT:
            IMENUCTL_SetTitle(pMenu, MAINMENU_RES_FILE_LANG, IDS_MAIN_MENU_TITLE_1, NULL);
#ifdef FEATURE_GAME_TETRIS            
            IMENUCTL_AddItem(pMenu, MAINMENU_RES_FILE_LANG,IDS_GAME_SUB_MENU_ITEM_1, IDS_GAME_SUB_MENU_ITEM_1, NULL, 0);
#endif
            
#ifdef FEATURE_GAME_BRICKATTACK
            IMENUCTL_AddItem(pMenu, MAINMENU_RES_FILE_LANG,IDS_GAME_SUB_MENU_ITEM_2, IDS_GAME_SUB_MENU_ITEM_2, NULL, 0);
#endif

#ifdef FEATURE_GAME_BLACKJACK
            IMENUCTL_AddItem(pMenu, MAINMENU_RES_FILE_LANG,IDS_GAME_SUB_MENU_ITEM_3, IDS_GAME_SUB_MENU_ITEM_3, NULL, 0);
#endif

#ifdef FEATURE_GAME_LKV
            IMENUCTL_AddItem(pMenu, MAINMENU_RES_FILE_LANG,IDS_MAIN_MENU_ROLL_BALL, IDS_MAIN_MENU_ROLL_BALL, NULL, 0); 
#endif
            return TRUE;

        case EVT_DIALOG_START:
            IMENUCTL_SetProperties(pMenu, MP_UNDERLINE_TITLE | MP_WRAPSCROLL | MP_BIND_ITEM_TO_NUMBER_KEY);
            IMENUCTL_SetOemProperties( pMenu, OEMMP_USE_MENU_STYLE);
            IMENUCTL_SetBottomBarType(pMenu,BTBAR_SELECT_BACK);  
            IMENUCTL_SetSel(pMenu, pMe->m_MenuSel);
            ISHELL_PostEvent(pMe->m_pShell,
                              AEECLSID_MAIN_MENU,
                              EVT_USER_REDRAW,
                              0,
                              0);
            return TRUE;

        case EVT_USER_REDRAW:
            IMENUCTL_Redraw(pMenu);
            return TRUE;
            
        case EVT_DIALOG_END:
            return TRUE;

        case EVT_KEY:
            switch(wParam)
            {
                case AVK_CLR:
                    pMe->m_MenuSel  = 0;
                    CLOSE_DIALOG(DLGRET_CANCELED);
                    return TRUE;

                default:
                    break;
            }
            return TRUE;

        case EVT_COMMAND:
            {
                pMe->m_MenuSel = wParam;
                switch (wParam)
                {
#ifdef FEATURE_GAME_TETRIS            
                    case IDS_GAME_SUB_MENU_ITEM_1:
                        ISHELL_StartApplet(pMe->m_pShell, AEECLSID_GAME_TETRIS);
                        break;
#endif                        
             
#ifdef FEATURE_GAME_BRICKATTACK           
                    case IDS_GAME_SUB_MENU_ITEM_2:
                        ISHELL_StartApplet(pMe->m_pShell, AEECLSID_BRICK);
                        break;                        
#endif

#ifdef FEATURE_GAME_BLACKJACK
                    case IDS_GAME_SUB_MENU_ITEM_3:
                        ISHELL_StartApplet(pMe->m_pShell, AEECLSID_BLACKJACK);
                        break;
#endif

#ifdef FEATURE_GAME_LKV
                    case IDS_MAIN_MENU_ROLL_BALL:
                        if(is_g_sportsapp_pedometer_bground_flag())
                        {
                            DBGPRINTF("IDS_MAIN_MENU_ROLL_BALL");
                            pMe->m_wMsgResID = IDS_GAME_PROCCESS;
                            CLOSE_DIALOG(DLGRET_MSGBOX)
                        }
                        else if(MainMenu_Get_Shake_OnOrOff(pMe))
                        {
                            pMe->m_wMsgResID = IDS_GAME_SHAKE;
                            CLOSE_DIALOG(DLGRET_MSGBOX)
                        }
                        else
                        {
                            #ifdef FEATURE_GAME_LKVFGTT
                                LKVFGTT_SetLkvFgttentry(FALSE);
                            #endif
                        ISHELL_StartApplet(pMe->m_pShell, AEECLSID_GAME_LKV);
                        }
                        break;
#endif
                    #ifdef FEATURE_GAME_LKVFGTT
                   case IDS_MAIN_MENU_ROLL_TOTEM:
                        if(is_g_sportsapp_pedometer_bground_flag())
                        {
                            DBGPRINTF("IDS_MAIN_MENU_ROLL_BALL");
                            pMe->m_wMsgResID = IDS_GAME_PROCCESS;
                            CLOSE_DIALOG(DLGRET_MSGBOX)
                        }
                        else if(MainMenu_Get_Shake_OnOrOff(pMe))
                        {
                            pMe->m_wMsgResID = IDS_GAME_SHAKE;
                            CLOSE_DIALOG(DLGRET_MSGBOX)
                        }
                        else
                        {
                            LKVFGTT_SetLkvFgttentry(TRUE);
                        ISHELL_StartApplet(pMe->m_pShell, AEECLSID_GAME_LKVFGTT);
                        }                    
                    break;
                    #endif
                    default:
                        break;
                }
                return TRUE;
            }

        default:
            break;
    }

    return FALSE;
}

/*=============================================================================
FUNCTION:  MainMenu_ListMenuHandler

DESCRIPTION:   �б�˵�

PARAMETERS:

=============================================================================*/
#ifdef FEATURE_MENU_STYLE
static boolean MainMenu_ListMenuHandler(MainMenu *pMe, AEEEvent eCode, uint16 wParam, uint32 dwParam)
{
    PARAM_NOT_REF(dwParam)
    IMenuCtl *pMenu = (IMenuCtl*)IDIALOG_GetControl(pMe->m_pActiveIDlg,IDC_MENU_LIST);
        
    if (pMenu == NULL)
    {
        return FALSE;
    }

    switch (eCode)
    {
        case EVT_DIALOG_INIT:
            IMENUCTL_SetTitle(pMenu, MAINMENU_RES_FILE_LANG, IDS_MENU_LIST, NULL);                
            IMENUCTL_AddItem(pMenu, MAINMENU_RES_FILE_LANG,IDS_MAIN_MENU_TITLE_1, IDS_MAIN_MENU_TITLE_1, NULL, 0);
            IMENUCTL_AddItem(pMenu, MAINMENU_RES_FILE_LANG,IDS_MAIN_MENU_TITLE_2, IDS_MAIN_MENU_TITLE_2, NULL, 0);
            IMENUCTL_AddItem(pMenu, MAINMENU_RES_FILE_LANG,IDS_MAIN_MENU_TITLE_3, IDS_MAIN_MENU_TITLE_3, NULL, 0);
            IMENUCTL_AddItem(pMenu, MAINMENU_RES_FILE_LANG,IDS_MAIN_MENU_TITLE_4, IDS_MAIN_MENU_TITLE_4, NULL, 0);
            IMENUCTL_AddItem(pMenu, MAINMENU_RES_FILE_LANG,IDS_MAIN_MENU_TITLE_5, IDS_MAIN_MENU_TITLE_5, NULL, 0);
            IMENUCTL_AddItem(pMenu, MAINMENU_RES_FILE_LANG,IDS_MAIN_MENU_TITLE_6, IDS_MAIN_MENU_TITLE_6, NULL, 0);
            IMENUCTL_AddItem(pMenu, MAINMENU_RES_FILE_LANG,IDS_MAIN_MENU_TITLE_7, IDS_MAIN_MENU_TITLE_7, NULL, 0);
            IMENUCTL_AddItem(pMenu, MAINMENU_RES_FILE_LANG,IDS_MAIN_MENU_TITLE_8, IDS_MAIN_MENU_TITLE_8, NULL, 0);
            IMENUCTL_AddItem(pMenu, MAINMENU_RES_FILE_LANG,IDS_MAIN_MENU_TITLE_9, IDS_MAIN_MENU_TITLE_9, NULL, 0);
#ifdef FEATURE_APP_NUMBERMANAGER
            IMENUCTL_AddItem(pMenu, MAINMENU_RES_FILE_LANG,IDS_MAIN_MENU_TITLE_13,IDS_MAIN_MENU_TITLE_13,NULL, 0);
#else
            IMENUCTL_AddItem(pMenu, MAINMENU_RES_FILE_LANG,IDS_MAIN_MENU_TITLE_10, IDS_MAIN_MENU_TITLE_10, NULL, 0);
#endif
            IMENUCTL_AddItem(pMenu, MAINMENU_RES_FILE_LANG,IDS_MAIN_MENU_TITLE_11, IDS_MAIN_MENU_TITLE_11, NULL, 0);
            IMENUCTL_AddItem(pMenu, MAINMENU_RES_FILE_LANG,IDS_MAIN_MENU_TITLE_12, IDS_MAIN_MENU_TITLE_12, NULL, 0); 
            return TRUE;
            
        case EVT_DIALOG_START:
            {  
                int i;
                for (i=1;i<=MAX_MATRIX_ITEMS;)
                {
                    AECHAR pwsz[67] = {0};
                    AECHAR pstr[64] = {0};
                    AECHAR wsFmt[5] = {0};
    
                    (void)STRTOWSTR("%d. ",wsFmt,sizeof(wsFmt));
                    WSPRINTF(pwsz,sizeof(pwsz),wsFmt,i);
                    
                    ISHELL_LoadResString( pMe->m_pShell,
                          MAINMENU_RES_FILE_LANG,
                          IDS_MAIN_MENU_TITLE_1 + i - 1,
                          pstr,
                          sizeof(pstr));
                    
#ifdef FEATURE_APP_NUMBERMANAGER
                    if(i == 10)
                    {
                        ISHELL_LoadResString( pMe->m_pShell,
                              MAINMENU_RES_FILE_LANG,
                              IDS_MAIN_MENU_TITLE_13,
                              pstr,
                              sizeof(pstr));                        
                    }
#endif
                    WSTRLCAT(pwsz,pstr,sizeof(pwsz));
#ifdef FEATURE_APP_NUMBERMANAGER
                    if(i == 10)
                    {
                        IMENUCTL_SetItemText(pMenu, IDS_MAIN_MENU_TITLE_13, NULL, NULL, pwsz);
                    }
                    else
#endif                        
                    {
                        IMENUCTL_SetItemText(pMenu, IDS_MAIN_MENU_TITLE_1 + i - 1, NULL, NULL, pwsz);
                    }
                    i++;
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
                case AVK_7:
                case AVK_8:
                case AVK_9:
                case AVK_STAR:
                    {
                        int Focus = (wParam - AVK_1);
                        StartApplet(pMe, Focus);
                    }
                    return TRUE;

                case AVK_0:
                    StartApplet(pMe, 10);
                    return TRUE;
                    
                case AVK_POUND:
                    StartApplet(pMe, 11);
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
            switch (wParam)
            {   
                case IDS_MAIN_MENU_TITLE_1:
                case IDS_MAIN_MENU_TITLE_2:
                case IDS_MAIN_MENU_TITLE_3:
                case IDS_MAIN_MENU_TITLE_4:
                case IDS_MAIN_MENU_TITLE_5:
                case IDS_MAIN_MENU_TITLE_6:
                case IDS_MAIN_MENU_TITLE_7:
                case IDS_MAIN_MENU_TITLE_8:
                case IDS_MAIN_MENU_TITLE_9:
                case IDS_MAIN_MENU_TITLE_10:
                case IDS_MAIN_MENU_TITLE_11:  
                case IDS_MAIN_MENU_TITLE_12:
                    StartApplet(pMe, wParam - IDS_MAIN_MENU_TITLE_1);
                    return TRUE;

                case IDS_MAIN_MENU_TITLE_13:
                    StartApplet(pMe, 9);
                    return TRUE;
            }
            return TRUE;
            
        default:
            break;
    }             
    return FALSE;
}
#endif

/*=============================================================================
FUNCTION:  MainMenu_PlayerMenuHandler

DESCRIPTION:  ��ý��˵��б�

PARAMETERS:

=============================================================================*/

static boolean MainMenu_PlayerMenuHandler(MainMenu *pMe, AEEEvent eCode, uint16 wParam, uint32 dwParam)
{
    PARAM_NOT_REF(dwParam)
    IMenuCtl *pMenu = (IMenuCtl*)IDIALOG_GetControl(pMe->m_pActiveIDlg,IDC_PLAYER_MENU);
    if (pMenu == NULL)
    {
        return FALSE;
    }

    switch (eCode)
    {
        case EVT_DIALOG_INIT:
            IMENUCTL_SetTitle(pMenu, MAINMENU_RES_FILE_LANG, IDS_MAIN_MENU_TITLE_11, NULL);                
            IMENUCTL_AddItem(pMenu, MAINMENU_RES_FILE_LANG,IDS_PLAYER_SUBMENU_ITEM1, IDS_PLAYER_SUBMENU_ITEM1, NULL, 0);
            IMENUCTL_AddItem(pMenu, MAINMENU_RES_FILE_LANG,IDS_PLAYER_SUBMENU_ITEM2, IDS_PLAYER_SUBMENU_ITEM2, NULL, 0);

#ifdef FEATURE_CARRIER_CHINA_TELCOM
            IMENUCTL_AddItem(pMenu, MAINMENU_RES_FILE_LANG,IDS_MAIN_MENU_TITLE_4, IDS_MAIN_MENU_TITLE_4, NULL, 0);
#endif
            
#if defined(FEATURE_FM_RADIO)
            IMENUCTL_AddItem(pMenu, MAINMENU_RES_FILE_LANG,IDS_PLAYER_SUBMENU_ITEM3, IDS_PLAYER_SUBMENU_ITEM3, NULL, 0);
#endif
            return TRUE;

        case EVT_DIALOG_START:
            IMENUCTL_SetProperties(pMenu, MP_UNDERLINE_TITLE | MP_WRAPSCROLL | MP_BIND_ITEM_TO_NUMBER_KEY);
            IMENUCTL_SetOemProperties( pMenu, OEMMP_USE_MENU_STYLE); 
#ifdef FEATURE_CARRIER_CHINA_VERTU
            IMENUCTL_SetBackGround(pMenu, AEE_APPSCOMMONRES_IMAGESFILE, IDI_MEDIA_BACKGROUND);
#endif
            IMENUCTL_SetBottomBarType(pMenu,BTBAR_SELECT_BACK); 
            IMENUCTL_SetSel(pMenu, pMe->m_MenuSel);
            
            ISHELL_PostEvent(pMe->m_pShell,
                              AEECLSID_MAIN_MENU,
                              EVT_USER_REDRAW,
                              0,
                              0);

            return TRUE;
        
        case EVT_DIALOG_END:
            return TRUE;

        case EVT_KEY:
            switch( wParam)
            {
                case AVK_CLR:
                    pMe->m_MenuSel  = 0;
                    CLOSE_DIALOG(DLGRET_CANCELED);
                    return TRUE;
                    

                default:
                    break;
            }
            return TRUE;
            
        case EVT_COMMAND:
            //pMe->m_MenuSel = IMENUCTL_GetSel(pMenu);
            pMe->m_MenuSel = wParam;
            switch (pMe->m_MenuSel)
            {   
                case IDS_PLAYER_SUBMENU_ITEM1:
                    (void)ISHELL_StartApplet(pMe->m_pShell, AEECLSID_APP_MUSICPLAYER);
                    break;
                    
                case IDS_PLAYER_SUBMENU_ITEM2:
                    (void)ISHELL_StartApplet(pMe->m_pShell, AEECLSID_VIDEOPLAYER);
                    break;
#if defined(FEATURE_FM_RADIO)  
                case IDS_PLAYER_SUBMENU_ITEM3:
                    (void)ISHELL_StartApplet(pMe->m_pShell, AEECLSID_APP_FMRADIO);
                    break;
#endif                    
                    
#ifdef FEATURE_CARRIER_CHINA_TELCOM
                case IDS_MAIN_MENU_TITLE_4:
                (void)ISHELL_StartApplet(pMe->m_pShell, AEECLSID_APP_CAMERA);
                break;
#endif
                
                default:
                    break;
            } 
            return TRUE;

        default:
            break;
    }
    
    return FALSE;
}

/*=============================================================================
FUNCTION:  MainMenu_DataMenuHandler

DESCRIPTION:  ����ҵ��˵��б�

PARAMETERS:

=============================================================================*/

static boolean MainMenu_DataMenuHandler(MainMenu *pMe, AEEEvent eCode, uint16 wParam, uint32 dwParam)
{
    PARAM_NOT_REF(dwParam)
    IMenuCtl *pMenu = (IMenuCtl*)IDIALOG_GetControl(pMe->m_pActiveIDlg,IDC_DATA_MENU);
    if (pMenu == NULL)
    {
        return FALSE;
    }
    switch (eCode)
    {
        case EVT_DIALOG_INIT:
            IMENUCTL_SetTitle(pMenu, MAINMENU_RES_FILE_LANG, IDS_MAIN_MENU_TITLE_12, NULL);    
            
#ifdef FEATURE_SUPPORT_WAP_APP            
            IMENUCTL_AddItem(pMenu, MAINMENU_RES_FILE_LANG,IDS_DATA_SUBMENU_ITEM1, IDS_DATA_SUBMENU_ITEM1, NULL, 0);
#endif
#ifdef FEATURE_SUPPORT_BREW_MOBILESHOP   
            IMENUCTL_AddItem(pMenu, MAINMENU_RES_FILE_LANG,IDS_DATA_SUBMENU_ITEM2, IDS_DATA_SUBMENU_ITEM2, NULL, 0);
#endif
            
#if defined(FEATURE_APP_UTK)  &&  defined(FEATURE_CARRIER_CHINA_TELCOM)
            IMENUCTL_AddItem(pMenu, MAINMENU_RES_FILE_LANG,IDS_DATA_SUBMENU_ITEM3, IDS_DATA_SUBMENU_ITEM3, NULL, 0);
#endif

            return TRUE;

        case EVT_DIALOG_START:
            IMENUCTL_SetProperties(pMenu, MP_UNDERLINE_TITLE | MP_WRAPSCROLL | MP_BIND_ITEM_TO_NUMBER_KEY);
            IMENUCTL_SetOemProperties( pMenu, OEMMP_USE_MENU_STYLE);
            IMENUCTL_SetBottomBarType(pMenu,BTBAR_SELECT_BACK); 
            IMENUCTL_SetSel(pMenu, pMe->m_MenuSel);
            
            ISHELL_PostEvent(pMe->m_pShell,
                              AEECLSID_MAIN_MENU,
                              EVT_USER_REDRAW,
                              0,
                              0);

            return TRUE;
        
        case EVT_DIALOG_END:
            return TRUE;

        case EVT_KEY:
            switch( wParam)
            {
                case AVK_CLR:
                    pMe->m_MenuSel  = 0;
                    CLOSE_DIALOG(DLGRET_CANCELED);
                    return TRUE;

                default:
                    break;
            }
            return TRUE;
            
        case EVT_COMMAND:
            //pMe->m_MenuSel = IMENUCTL_GetSel(pMenu);
            pMe->m_MenuSel = wParam;
            switch (pMe->m_MenuSel)
            {   
#ifdef FEATURE_SUPPORT_WAP_APP                
                case IDS_DATA_SUBMENU_ITEM1:
                    (void)ISHELL_StartApplet(pMe->m_pShell, AEECLSID_BRW_APP);
                    break;
#endif                    
                    
#ifdef FEATURE_SUPPORT_BREW_MOBILESHOP                    
                case IDS_DATA_SUBMENU_ITEM2:
                    (void)ISHELL_StartApplet(pMe->m_pShell, AEECLSID_APPMANAGER);
                    break;
#endif
                    
#if defined(FEATURE_APP_UTK)  &&  defined(FEATURE_CARRIER_CHINA_TELCOM)                    
                case IDS_DATA_SUBMENU_ITEM3:
                    (void)ISHELL_StartApplet(pMe->m_pShell, AEECLSID_APP_UTK);
                    break;      
#endif                    

                default:
                    break;
            } 
            return TRUE;

        default:
            break;
    }
    
    return FALSE;
}

#ifdef FEATRUE_SUPPORT_G_SENSOR
/*=============================================================================
FUNCTION:  MainMenu_MsgBoxHandler

DESCRIPTION:  �����˵�����ӶԻ���
PARAMETERS:

=============================================================================*/

static boolean  MainMenu_MsgBoxHandler(MainMenu *pMe, AEEEvent eCode, uint16 wParam, uint32 dwParam)
{
    static IStatic * pStatic = NULL;

    if (NULL == pStatic)
    {
        AEERect rect = {0};
        if (AEE_SUCCESS != ISHELL_CreateInstance(pMe->m_pShell,
                                                     AEECLSID_STATIC,
                                                     (void **)&pStatic))
        {
            return FALSE;
        }        
        ISTATIC_SetRect(pStatic, &rect);  
    }

    if ((NULL == pStatic) ||(NULL == pMe))
    {
        return FALSE;
    }

    switch(eCode)
    {
        case EVT_DIALOG_INIT:
            return TRUE;

        case EVT_DIALOG_START:
            (void) ISHELL_PostEventEx(pMe->m_pShell, 
                                    EVTFLG_ASYNC,
                                    AEECLSID_MAIN_MENU,
                                    EVT_USER_REDRAW,
                                    0, 
                                    0);

            return TRUE;

        case EVT_USER_REDRAW:
            {
                PromptMsg_Param_type  Msg_Param={0};
                AECHAR  wstrText[MSGBOX_MAXTEXTLEN] = {(AECHAR)'\0'};

                (void)ISHELL_LoadResString(pMe->m_pShell,
                                MAINMENU_RES_FILE_LANG,                                
                                pMe->m_wMsgResID,
                                wstrText,
                                sizeof(wstrText));
                                
                Msg_Param.ePMsgType = MESSAGE_CONFIRM;
                Msg_Param.pwszMsg = wstrText;

                Msg_Param.eBBarType = BTBAR_OK_CANCEL;

                
                DrawPromptMessage(pMe->m_pDisplay, pStatic, &Msg_Param);
            }

            IDISPLAY_UpdateEx(pMe->m_pDisplay, FALSE);
            #if 0
            (void) ISHELL_SetTimer(pMe->m_pShell,
                        PROMPTMSG_TIMER,
                        Mainmenu_DialogTimeout,
                        pMe);
            #endif
            
            return TRUE;

        case EVT_DIALOG_END:
                    #if 0
                     (void) ISHELL_CancelTimer(pMe->m_pShell,Morse_DialogTimeout, pMe);
                    #endif
            ISTATIC_Release(pStatic);
            pStatic = NULL;
            
            return TRUE;

        case EVT_KEY:
            switch( wParam)
            {
                case AVK_CLR:
                    CLOSE_DIALOG(DLGRET_GAME)
                    break;
                case AVK_INFO:
                case AVK_SELECT:
                    if(is_g_sportsapp_pedometer_bground_flag())
                    {
                        MainMenu_CloseSportBgRun(pMe);
                    }
                    else if(MainMenu_Get_Shake_OnOrOff(pMe))
                    {
                        MainMenu_Set_Shake_Disable(pMe);
                    }
                    CLOSE_DIALOG(DLGRET_GAME)
                    break;
                    default:
                        return TRUE;

            }
            return TRUE;
        
        case EVT_DISPLAYDIALOGTIMEOUT:
            CLOSE_DIALOG(DLGRET_GAME)
            return TRUE;

        default:
            break;
    }

    return FALSE;
}


#endif


/*=============================================================================
FUNCTION:  StartApplet

DESCRIPTION:  ����applet

PARAMETERS:  ���APPLET �б䶯��ֻ��Ķ��κ���

=============================================================================*/
static boolean StartApplet(MainMenu *pMe, int i)
{
#ifdef FEATURE_LCD_TOUCH_ENABLE//WLH ADD FOR LCD TOUCH
	if (pMe->m_pAnimate != NULL)
	{
		IIMAGE_Release(pMe->m_pAnimate);
		pMe->m_pAnimate = NULL;
	}
#endif//FEATURE_LCD_TOUCH_ENABLE

    switch(i)
    {
        case 0:
            CLOSE_DIALOG(DLGRET_GAME);
            break;
            
        case 1:
            ISHELL_StartApplet(pMe->m_pShell, AEECLSID_MEDIAGALLERY);
            break;
    
        case 2:
            ISHELL_StartApplet(pMe->m_pShell, AEECLSID_EXTRAMENU);
            break;
    
        case 3:
#ifdef FEATURE_CARRIER_CHINA_TELCOM
            {
                IContactApp *ca = NULL;
                if(SUCCESS != ISHELL_CreateInstance(pMe->m_pShell, AEECLSID_APP_CONTACT, (void**)&ca))
                {
                    return FALSE;
                }
                else
                {
                    ICONTAPP_MainMenu(ca);
                    IContactApp_Release(ca);
                }
            }            
#else
            ISHELL_StartApplet(pMe->m_pShell, AEECLSID_APP_CAMERA);
#endif
            break;
    
        case 4:
#ifndef FEATURE_CARRIER_CHINA_TELCOM
            {
                IContactApp *ca = NULL;
                if(SUCCESS != ISHELL_CreateInstance(pMe->m_pShell, AEECLSID_APP_CONTACT, (void**)&ca))
                {
                    return FALSE;
                }
                else
                {
                    ICONTAPP_MainMenu(ca);
                    IContactApp_Release(ca);
                }
            }
#else
            ISHELL_StartApplet(pMe->m_pShell, AEECLSID_ESURFING);
#endif
            break;
    
        case 5:
#ifndef FEATURE_CARRIER_CHINA_TELCOM
            ISHELL_StartApplet(pMe->m_pShell, AEECLSID_WMSAPP);
#else
            #ifdef FEATURE_APP_NUMBERMANAGER
            ISHELL_StartApplet(pMe->m_pShell, AEECLSID_NUMBERMANAGER);   
            #endif
#endif
            break;
    
        case 6:
            ISHELL_StartApplet(pMe->m_pShell, AEECLSID_APP_RECENTCALL);
            break;
    
        case 7:
#ifndef FEATURE_CARRIER_CHINA_TELCOM
            ISHELL_StartApplet(pMe->m_pShell, AEECLSID_APP_SETTINGMENU);
#else
            ISHELL_StartApplet(pMe->m_pShell, AEECLSID_WMSAPP);
#endif
            break;
    
        case 8:
            ISHELL_StartApplet(pMe->m_pShell, AEECLSID_SPORTSAPP);
            break;
    
        case 9:
#ifndef FEATURE_CARRIER_CHINA_TELCOM
#ifdef FEATURE_APP_NUMBERMANAGER
            // for �������ͨ
            ISHELL_StartApplet(pMe->m_pShell, AEECLSID_NUMBERMANAGER);            
#else
            ISHELL_StartApplet(pMe->m_pShell, AEECLSID_APP_SOUNDMENU);
#endif
#else
            ISHELL_StartApplet(pMe->m_pShell, AEECLSID_CUSTSERVICE);
#endif
            break;
    
        case 10:
            CLOSE_DIALOG(DLGRET_MEDIA);
            break;
    
        case 11:
            //ISHELL_StartApplet(pMe->m_pShell, AEECLSID_BRW_APP);
#ifndef FEATURE_CARRIER_CHINA_TELCOM
            CLOSE_DIALOG(DLGRET_DATA);
#else
            ISHELL_StartApplet(pMe->m_pShell, AEECLSID_APP_SETTINGMENU);
#endif
            break;

    }
    setCursor(pMe, i/MAX_MATRIX_COLS, i%MAX_MATRIX_COLS);
    return TRUE;
}


/*=============================================================================
FUNCTION:  StartApplet

DESCRIPTION:  ����applet

PARAMETERS:  ���APPLET �б䶯��ֻ��Ķ��κ���

=============================================================================*/
#ifdef FEATRUE_SUPPORT_G_SENSOR
static void Mainmenu_DialogTimeout(void *pme)
{
    MainMenu *pMe = (MainMenu*)pme;

    if (NULL == pMe)
    {
        return;
    }

    (void) ISHELL_PostEvent(pMe->m_pShell,
                            AEECLSID_MAIN_MENU,
                            EVT_DISPLAYDIALOGTIMEOUT,
                            0,
                            0);
}

/*=============================================================================
FUNCTION:  MainMenu_Set_Shake_Disable

DESCRIPTION:  ��shake ���ó�disable

PARAMETERS:  ���APPLET �б䶯��ֻ��Ķ��κ���

=============================================================================*/
boolean MainMenu_Set_Shake_Disable(MainMenu *pMe)
{
    
    dword shake;
    boolean isShakeOn = FALSE;
    (void)ICONFIG_GetItem(pMe->m_pConfig,CFGI_GSENSOR,&shake,sizeof(shake));
    if(shake)
    {
        
        if(shake & OEMNV_SHAKE_MUSIC_MASK)
        {
            isShakeOn = FALSE;                                      
            /*Set Shake Reset*/
            mmi_g_sensor_process(G_SENSOR_SHAKE_DISABLE);

            mmi_g_sensor_process(G_SENSOR_VIDEO_CIRC_DISABLE); 
        }
        
        else if(shake & OEMNV_SHAKE_FM_RADIO_MASK)
        {
            isShakeOn = FALSE;                                      
            mmi_g_sensor_process(G_SENSOR_SHAKE_DISABLE);
                        mmi_g_sensor_process(G_SENSOR_VIDEO_CIRC_DISABLE);
        }
        else if(shake & OEMNV_SHAKE_WALLPAPER_MASK)
        {
            isShakeOn = FALSE;                                      
            mmi_g_sensor_process(G_SENSOR_SHAKE_DISABLE);
                        mmi_g_sensor_process(G_SENSOR_VIDEO_CIRC_DISABLE);          
        
        }
        else if(shake & OEMNV_SHAKE_SNOOZE_ALARM_MASK)
        {
            isShakeOn = FALSE;                                      
            mmi_g_sensor_process(G_SENSOR_SHAKE_DISABLE);
                        mmi_g_sensor_process(G_SENSOR_VIDEO_CIRC_DISABLE);              
        }
        else if(shake & OEMNV_SHAKE_VIDEO_MASK)
        {
            isShakeOn = FALSE;      
            mmi_g_sensor_process(G_SENSOR_VIDEO_CIRC_DISABLE);
                        mmi_g_sensor_process(G_SENSOR_SHAKE_DISABLE);
            
        }

            (void)ICONFIG_SetItem(pMe->m_pConfig,CFGI_SHAKE_MUSIC_CHECK,&isShakeOn,
                            sizeof(isShakeOn));
            (void)ICONFIG_SetItem(pMe->m_pConfig,CFGI_SHAKE_FM_RADIO_CHECK,&isShakeOn,
                                    sizeof(isShakeOn));
            (void)ICONFIG_SetItem(pMe->m_pConfig,CFGI_SHAKE_WALLPAPER_CHECK,&isShakeOn,
                                    sizeof(isShakeOn));
            
            (void)ICONFIG_SetItem(pMe->m_pConfig,CFGI_SHAKE_SNOOZE_ALARM_CHECK,&isShakeOn,
                                    sizeof(isShakeOn));

            (void)ICONFIG_SetItem(pMe->m_pConfig,CFGI_SHAKE_VIDEO_CHECK,&isShakeOn,
                                    sizeof(isShakeOn)); 

            return TRUE;            
    
    }
    else
    {
        return FALSE;
    }
        

    
}

/*=============================================================================
FUNCTION:  MainMenu_Get_Shake_OnOrOff

DESCRIPTION:  ��shake ���ó�disable

PARAMETERS:  ���APPLET �б䶯��ֻ��Ķ��κ���

=============================================================================*/

boolean MainMenu_Get_Shake_OnOrOff(MainMenu *pMe)
{
    dword shake;
    (void)ICONFIG_GetItem(pMe->m_pConfig,CFGI_GSENSOR,&shake,sizeof(shake));

    if(shake)
    {
        return TRUE;
    }
    else
    {
        return FALSE;
    }

}
/*=============================================================================
FUNCTION:  MainMenu_CloseSportBgRun

DESCRIPTION:  ��shake ���ó�disable

PARAMETERS:  ���APPLET �б䶯��ֻ��Ķ��κ���

=============================================================================*/
void MainMenu_CloseSportBgRun(MainMenu *pMe)
{
    (void) ISHELL_PostEvent( pMe->m_pShell,AEECLSID_SPORTSAPP,
                                EVT_CLOSEAPP,
                                0,
                                0);
    
}

#endif


/*=============================================================================
FUNCTION:  MainMenu_MainMenuService

DESCRIPTION:  �ⲿ�ӿڵ��ã����ٽ��뵽MAINMENUĳһ���˵��б�

PARAMETERS:    
            eStype: ����app�ķ�ʽ

=============================================================================*/

static int  MainMenu_MainMenuService( IMainMenu *pi, MainMenuServiceType eStype)
{
    char  *args = NULL;
    MainMenu *pMe = (MainMenu*)pi;
    int    nRet;  

    if (ISHELL_ActiveApplet(pMe->m_pShell) == AEECLSID_MAIN_MENU)
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

    nRet = ISHELL_StartAppletArgs(pMe->m_pShell, AEECLSID_MAIN_MENU, args);
        
    FREEIF(args);
    args = NULL;
    
    return nRet;    
}
void MainMenu_DraImage(MainMenu *pMe,int16 index,int16 imgid)
{
        
        if(pMe->m_pImageIcon[index] != NULL)
        {
            IIMAGE_Release(pMe->m_pImageIcon[index]);
            pMe->m_pImageIcon[index] = NULL;
            
            pMe->m_pImageIcon[index] = ISHELL_LoadResImage(pMe->m_pShell,
                                                MAINMENU_RES_FILE_IMAGE,
                                                imgid);
        }
}
