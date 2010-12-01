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
#include "appscommonimages.brh"
#ifdef FEATURE_RANDOM_MENU_REND
#include "DisplayRendUtils.h"
#endif
#if !defined( AEE_SIMULATOR)
#include "AEERUIM.h" 
#include "OEMCFGI.h"
#endif //#if !defined( AEE_SIMULATOR)

#define PARAM_NOT_REF(x)
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

static MainMenu gMainMenu={0};

static const VTBL( IMainMenu) gMainMenuMethods =
{
    MainMenu_AddRef,
    MainMenu_Release,
    MainMenu_HandleEvent
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

static void CalculateScreenParameters(MainMenu *pMe);

static void MainMenu_DrawBackGround(MainMenu *pMe, AEERect *pRect);

// ��ʼ����������ȫ����ʼͼ��
static void DrawMatrix(MainMenu *pMe);

static void DrawFocusIcon(MainMenu *pMe);

static void MoveCursorTo(MainMenu *pMe, int row, int column);

static void SetCursor(MainMenu *pMe, int row, int column);

static int StartApplet(MainMenu *pMe, int i);

boolean MainMenu_RouteDialogEvt(MainMenu *pMe,
    AEEEvent    eCode,
    uint16  wParam,
    uint32 dwParam
);

NextFSMAction MainMenu_ProcessState(MainMenu *pMe);

void MainMenu_ShowDialog(MainMenu  *pMe,  uint16 dlgResId);

// MAINST_MAIN ״̬������
static NextFSMAction MAINST_MAIN_Handler(MainMenu *pMe);
//MAINST_EXIT  ״̬������
static NextFSMAction MAINST_EXIT_Handler(MainMenu *pMe);
static boolean  MainMenu_IconMenuHandler(MainMenu *pMe, AEEEvent eCode, uint16 wParam, uint32 dwParam);

/*==============================================================================
                              
                              ���أ���̬������
                              
==============================================================================*/
static char* ICON_ANI[] =
{
#if defined FEATURE_VERSION_H19C
    ICON_ANI_BG,
#else
    ICON1_ANI,
    ICON2_ANI,
    ICON3_ANI,
    ICON4_ANI,
    ICON5_ANI,
    ICON6_ANI,
    ICON7_ANI,
    ICON8_ANI,
    ICON9_ANI,
#if defined (FEATURE_DISP_160X128)
    ICON10_ANI,
    ICON11_ANI,
    ICON12_ANI,
#elif defined (FEATURE_DISP_220X176)
    ICON10_ANI,
    ICON11_ANI,
    ICON12_ANI,    
#elif defined (FEATURE_DISP_128X160)
    ICON10_ANI,
    ICON11_ANI,
    ICON12_ANI,
#elif defined (FEATURE_DISP_176X220)
    ICON10_ANI,
    ICON11_ANI,
    ICON12_ANI,
#elif defined (FEATURE_DISP_240X320)
    ICON10_ANI,
    ICON11_ANI,
    ICON12_ANI,    
#elif defined (FEATURE_DISP_320X240)
    ICON10_ANI,
    ICON11_ANI,
    ICON12_ANI,        
#endif
#endif
};

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
#if defined (FEATURE_DISP_160X128)
    ICON10_ANI_1,
    ICON11_ANI_1,
    ICON12_ANI_1,
#elif defined (FEATURE_DISP_220X176)
    ICON10_ANI_1,
    ICON11_ANI_1,
    ICON12_ANI_1,    
#elif defined (FEATURE_DISP_128X160)
    ICON10_ANI_1,
    ICON11_ANI_1,
    ICON12_ANI_1,   
#elif defined (FEATURE_DISP_176X220)
    ICON10_ANI_1,
    ICON11_ANI_1,
    ICON12_ANI_1,  
#elif defined (FEATURE_DISP_240X320)
    ICON10_ANI_1,
    ICON11_ANI_1,
    ICON12_ANI_1,     
#elif defined (FEATURE_DISP_320X240)
    ICON10_ANI_1,
    ICON11_ANI_1,
    ICON12_ANI_1,      
#endif
};

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
    if (NULL == pMe)
    {
        return EFAILED;
    }
#if defined (FEATURE_DISP_160X128)

//#if defined(FEATURE_PROJECT_SMART) || defined(FEATURE_PROJECT_M8)
#if defined(FEATURE_MENU_FOCUS_INDEX)	
	pMe->m_nRow        = 1;
    pMe->m_nColumn     = 2;
#else
    pMe->m_nRow        = 1;
    pMe->m_nColumn     = 1;
#endif

#elif defined (FEATURE_DISP_220X176)
	pMe->m_nRow        = 1;
    pMe->m_nColumn     = 1;
#elif defined (FEATURE_DISP_128X128)
	pMe->m_nRow        = 1;
    pMe->m_nColumn     = 1;
#elif defined (FEATURE_DISP_128X160)
	pMe->m_nRow        = 1;
    pMe->m_nColumn     = 1;	
#elif defined (FEATURE_DISP_176X220)
	pMe->m_nRow        = 1;
    pMe->m_nColumn     = 1;	
#elif defined (FEATURE_DISP_240X320)
	pMe->m_nRow        = 1;
    pMe->m_nColumn     = 1;		
#elif defined (FEATURE_DISP_320X240)
	pMe->m_nRow        = 1;
    pMe->m_nColumn     = 1;			
#endif
    
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
	if (AEE_SUCCESS != ISHELL_CreateInstance(pMe->m_pShell,
                                            AEECLSID_ANNUNCIATOR,
                                            (void **)&pMe->m_pIAnn))
    {    	
        return EFAILED;
    }
    
    // ��ʼ���˵�Title
#if defined (FEATURE_DISP_160X128)
#if defined	(FEATURE_VERSION_FLEXI203) || defined	(FEATURE_VERSION_IVIO203)
    pMe->m_IconTitle[0]     = IDS_MAIN_MENU_MEDIAGALLERY;
    pMe->m_IconTitle[1]     = IDS_MAIN_MENU_CONTACTS;
    pMe->m_IconTitle[2]     = IDS_MAIN_MENU_UTK;
    pMe->m_IconTitle[3]     = IDS_MAIN_MENU_TOOLS;
    pMe->m_IconTitle[4]     = IDS_MAIN_MENU_RECENTCALLS;
    pMe->m_IconTitle[5]     = IDS_MAIN_MENU_MESSAGES;
    pMe->m_IconTitle[6]     = IDS_MAIN_MENU_MULTIMEDIA;
    pMe->m_IconTitle[7]     = IDS_MAIN_MENU_STATIC_APPLICATION;
    pMe->m_IconTitle[8]     = IDS_MAIN_MENU_SCHEDULER;
    pMe->m_IconTitle[9]     = IDS_MAIN_MENU_SETTINGS;
    pMe->m_IconTitle[10]    = IDS_MAIN_MENU_USERPROFILE;
    pMe->m_IconTitle[11]    = IDS_MAIN_MENU_GAMES;
#elif defined (FEATURE_VERSION_SMART)
    pMe->m_IconTitle[0]     = IDS_MAIN_MENU_SMARTWORLD;
    pMe->m_IconTitle[1]     = IDS_MAIN_MENU_SMARTFRENACCESS;
    pMe->m_IconTitle[2]     = IDS_MAIN_MENU_MUSICPLAYER;
    pMe->m_IconTitle[3]     = IDS_MAIN_MENU_FMRADIO;
    pMe->m_IconTitle[4]     = IDS_MAIN_MENU_CALENDAR;
    pMe->m_IconTitle[5]     = IDS_MAIN_MENU_CONTACT;
    pMe->m_IconTitle[6]     = IDS_MAIN_MENU_SMS;
    pMe->m_IconTitle[7]     = IDS_MAIN_MENU_GAMES;
    pMe->m_IconTitle[8]     = IDS_MAIN_MENU_RECENTCALLS;
    pMe->m_IconTitle[9]     = IDS_MAIN_MENU_GALLERY;
    pMe->m_IconTitle[10]    = IDS_MAIN_MENU_APPLICATION;
    pMe->m_IconTitle[11]    = IDS_MAIN_MENU_SETTINGS;
#elif defined (FEATURE_VERSION_M8)
    pMe->m_IconTitle[0]     = IDS_MAIN_MENU_BLIVE;
    pMe->m_IconTitle[1]     = IDS_MAIN_MENU_SMARTFRENACCESS;
    pMe->m_IconTitle[2]     = IDS_MAIN_MENU_MUSICPLAYER;
    pMe->m_IconTitle[3]     = IDS_MAIN_MENU_FMRADIO;
    pMe->m_IconTitle[4]     = IDS_MAIN_MENU_FRENDUO;
    pMe->m_IconTitle[5]     = IDS_MAIN_MENU_CONTACT;
    pMe->m_IconTitle[6]     = IDS_MAIN_MENU_SMS;
    pMe->m_IconTitle[7]     = IDS_MAIN_MENU_GAMES;
    pMe->m_IconTitle[8]     = IDS_MAIN_MENU_RECENTCALLS;
    pMe->m_IconTitle[9]     = IDS_MAIN_MENU_GALLERY;
    pMe->m_IconTitle[10]    = IDS_MAIN_MENU_APPLICATION;
    pMe->m_IconTitle[11]    = IDS_MAIN_MENU_SETTINGS;
#else
    pMe->m_IconTitle[0]     = IDS_MAIN_MENU_MEDIAGALLERY;
    pMe->m_IconTitle[1]     = IDS_MAIN_MENU_CONTACTS;
    pMe->m_IconTitle[2]     = IDS_MAIN_MENU_UTK;
    pMe->m_IconTitle[3]     = IDS_MAIN_MENU_CALCULATOR;
    pMe->m_IconTitle[4]     = IDS_MAIN_MENU_RECENTCALLS;
    pMe->m_IconTitle[5]     = IDS_MAIN_MENU_MESSAGES;
    pMe->m_IconTitle[6]     = IDS_MAIN_MENU_MULTIMEDIA;
    pMe->m_IconTitle[7]     = IDS_MAIN_MENU_APPLICATION;
    pMe->m_IconTitle[8]     = IDS_MAIN_MENU_SCHEDULER;
    pMe->m_IconTitle[9]     = IDS_MAIN_MENU_SETTINGS;
    pMe->m_IconTitle[10]    = IDS_MAIN_MENU_GAMES;
    pMe->m_IconTitle[11]    = IDS_MAIN_MENU_MUSICPLAYER;
#endif
#elif defined (FEATURE_DISP_220X176)
    pMe->m_IconTitle[0]     = IDS_MAIN_MENU_MEDIAGALLERY;
    pMe->m_IconTitle[1]     = IDS_MAIN_MENU_CONTACTS;
    pMe->m_IconTitle[2]     = IDS_MAIN_MENU_UTK;
    pMe->m_IconTitle[3]     = IDS_MAIN_MENU_FMRADIO;
    pMe->m_IconTitle[4]     = IDS_MAIN_MENU_MESSAGES;
    pMe->m_IconTitle[5]     = IDS_MAIN_MENU_RECENTCALLS;
    pMe->m_IconTitle[6]     = IDS_MAIN_MENU_MUSICPLAYER;
    pMe->m_IconTitle[7]     = IDS_MAIN_MENU_SETTINGS;
    pMe->m_IconTitle[8]     = IDS_MAIN_MENU_APPLICATION;
    pMe->m_IconTitle[9]     = IDS_MAIN_MENU_TIMER;
    pMe->m_IconTitle[10]    = IDS_MAIN_MENU_SCHEDULER;
    pMe->m_IconTitle[11]    = IDS_MAIN_MENU_CALCULATOR;
#elif defined (FEATURE_DISP_128X128)
#ifndef FEATURE_VERSION_H19C
    pMe->m_IconTitle[0]     = IDS_MAIN_MENU_MEDIAGALLERY;
    pMe->m_IconTitle[1]     = IDS_MAIN_MENU_CONTACTS;
    pMe->m_IconTitle[2]     = IDS_MAIN_MENU_UTK;
    pMe->m_IconTitle[3]     = IDS_MAIN_MENU_FMRADIO;
    pMe->m_IconTitle[4]     = IDS_MAIN_MENU_MESSAGES;
    pMe->m_IconTitle[5]     = IDS_MAIN_MENU_RECENTCALLS;
    pMe->m_IconTitle[6]     = IDS_MAIN_MENU_MUSICPLAYER;
    pMe->m_IconTitle[7]     = IDS_MAIN_MENU_SETTINGS;
    pMe->m_IconTitle[8]     = IDS_MAIN_MENU_APPLICATION;
#else
    pMe->m_IconTitle[0]     = IDS_MAIN_MENU_RECENTCALLS;
    pMe->m_IconTitle[1]     = IDS_MAIN_MENU_MULTIMEDIA;
    pMe->m_IconTitle[2]     = IDS_MAIN_MENU_CONTACTS;
    pMe->m_IconTitle[3]     = IDS_MAIN_MENU_USERPROFILE;
    pMe->m_IconTitle[4]     = IDS_MAIN_MENU_MESSAGES;
    pMe->m_IconTitle[5]     = IDS_MAIN_MENU_TOOLS;
    pMe->m_IconTitle[6]     = IDS_MAIN_MENU_SETTINGS;
    pMe->m_IconTitle[7]     = IDS_MAIN_MENU_GAMES;
    pMe->m_IconTitle[8]     = IDS_MAIN_MENU_UTK;
#endif
#elif defined (FEATURE_DISP_128X160)
    pMe->m_IconTitle[0]     = IDS_MAIN_MENU_MEDIAGALLERY;
    pMe->m_IconTitle[1]     = IDS_MAIN_MENU_CONTACTS;
    pMe->m_IconTitle[2]     = IDS_MAIN_MENU_UTK;
    pMe->m_IconTitle[3]     = IDS_MAIN_MENU_MULTIMEDIA;
    pMe->m_IconTitle[4]     = IDS_MAIN_MENU_MESSAGES;
    pMe->m_IconTitle[5]     = IDS_MAIN_MENU_RECENTCALLS;
    pMe->m_IconTitle[6]     = IDS_MAIN_MENU_MUSICPLAYER;
    pMe->m_IconTitle[7]     = IDS_MAIN_MENU_SETTINGS;
    pMe->m_IconTitle[8]     = IDS_MAIN_MENU_APPLICATION;
    pMe->m_IconTitle[9]     = IDS_MAIN_MENU_TIMER;
    pMe->m_IconTitle[10]    = IDS_MAIN_MENU_SCHEDULER;
    pMe->m_IconTitle[11]    = IDS_MAIN_MENU_CALCULATOR;
#elif defined (FEATURE_DISP_176X220)
    pMe->m_IconTitle[0]     = IDS_MAIN_MENU_MEDIAGALLERY;
    pMe->m_IconTitle[1]     = IDS_MAIN_MENU_CONTACTS;
    pMe->m_IconTitle[2]     = IDS_MAIN_MENU_UTK;
    pMe->m_IconTitle[3]     = IDS_MAIN_MENU_MULTIMEDIA;
    pMe->m_IconTitle[4]     = IDS_MAIN_MENU_MESSAGES;
    pMe->m_IconTitle[5]     = IDS_MAIN_MENU_RECENTCALLS;
    pMe->m_IconTitle[6]     = IDS_MAIN_MENU_MUSICPLAYER;
    pMe->m_IconTitle[7]     = IDS_MAIN_MENU_SETTINGS;
    pMe->m_IconTitle[8]     = IDS_MAIN_MENU_APPLICATION;
    pMe->m_IconTitle[9]     = IDS_MAIN_MENU_TIMER;
    pMe->m_IconTitle[10]    = IDS_MAIN_MENU_SCHEDULER;
    pMe->m_IconTitle[11]    = IDS_MAIN_MENU_CALCULATOR;
#elif defined (FEATURE_DISP_240X320)
    pMe->m_IconTitle[0]     = IDS_MAIN_MENU_MEDIAGALLERY;
    pMe->m_IconTitle[1]     = IDS_MAIN_MENU_CONTACTS;
    pMe->m_IconTitle[2]     = IDS_MAIN_MENU_UTK;
    pMe->m_IconTitle[3]     = IDS_MAIN_MENU_MULTIMEDIA;
    pMe->m_IconTitle[4]     = IDS_MAIN_MENU_MESSAGES;
    pMe->m_IconTitle[5]     = IDS_MAIN_MENU_RECENTCALLS;
    pMe->m_IconTitle[6]     = IDS_MAIN_MENU_MUSICPLAYER;
    pMe->m_IconTitle[7]     = IDS_MAIN_MENU_SETTINGS;
    pMe->m_IconTitle[8]     = IDS_MAIN_MENU_APPLICATION;
    pMe->m_IconTitle[9]     = IDS_MAIN_MENU_TIMER;
    pMe->m_IconTitle[10]    = IDS_MAIN_MENU_SCHEDULER;
    pMe->m_IconTitle[11]    = IDS_MAIN_MENU_CALCULATOR;
#elif defined (FEATURE_DISP_320X240)
    pMe->m_IconTitle[0]     = IDS_MAIN_MENU_MEDIAGALLERY;
    pMe->m_IconTitle[1]     = IDS_MAIN_MENU_CONTACTS;
    pMe->m_IconTitle[2]     = IDS_MAIN_MENU_UTK;
    pMe->m_IconTitle[3]     = IDS_MAIN_MENU_MULTIMEDIA;
    pMe->m_IconTitle[4]     = IDS_MAIN_MENU_MESSAGES;
    pMe->m_IconTitle[5]     = IDS_MAIN_MENU_RECENTCALLS;
    pMe->m_IconTitle[6]     = IDS_MAIN_MENU_MUSICPLAYER;
    pMe->m_IconTitle[7]     = IDS_MAIN_MENU_SETTINGS;
    pMe->m_IconTitle[8]     = IDS_MAIN_MENU_APPLICATION;
    pMe->m_IconTitle[9]     = IDS_MAIN_MENU_TIMER;
    pMe->m_IconTitle[10]    = IDS_MAIN_MENU_SCHEDULER;
    pMe->m_IconTitle[11]    = IDS_MAIN_MENU_CALCULATOR;
#endif
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
	if (pMe->m_pIAnn)
    {
        IANNUNCIATOR_Release(pMe->m_pIAnn);
		pMe->m_pIAnn = NULL;
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
#ifdef FEATURE_RANDOM_MENU_REND
            DisplayRend_Enable(TRUE);
#endif
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
            IANNUNCIATOR_SetFieldIsActiveEx(pMe->m_pIAnn,TRUE); 
            MainMenu_RunFSM(pMe);
            return TRUE;

        case EVT_APP_STOP:
            {
                int theFocus = 4;
                SetCursor( pMe, theFocus / 3, theFocus % 3);
                (void)ISHELL_CancelTimer( pMe->m_pShell, NULL, pMe);
                pMe->m_eAppStatus = MAINMENU_STOP;
#ifdef FEATURE_RANDOM_MENU_REND
                DisplayRend_Enable(FALSE);
#endif
                return TRUE;
            }

        case EVT_APP_SUSPEND:
            (void)ISHELL_CancelTimer(pMe->m_pShell, NULL, pMe);
            pMe->m_eAppStatus = MAINMENU_SUSPEND;
#ifdef FEATURE_RANDOM_MENU_REND
            DisplayRend_Push();
            DisplayRend_Start(FALSE);
#endif
            return TRUE;
        

        case EVT_APP_RESUME:
            {
                AEEAppStart* as = ( AEEAppStart*)dwParam;
                pMe->m_rc    = as->rc;
                pMe->m_eAppStatus = MAINMENU_RUNNING;
#ifdef FEATURE_RANDOM_MENU_REND
                DisplayRend_Push();
                DisplayRend_Start(TRUE);
#endif
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
        case MAINST_MAIN:
            return MAINST_MAIN_Handler(pMe);
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
                MainMenu_ShowDialog(pMe, IDD_MAIN_MENU);
            }
            return NFSMACTION_WAIT;
            
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
        return;
    }
   
    if (NULL != pMe->m_pDisplay)
    {
        AEEDeviceInfo di={0,};
#ifdef FEATURE_VERSION_H19C      
        if(pMe->m_pIAnn != NULL)
        {
            if (dlgResId == IDD_MAIN_MENU)
            {
                IANNUNCIATOR_SetHasTitleText(pMe->m_pIAnn, FALSE);
            }
            else
            {
                IANNUNCIATOR_SetHasTitleText(pMe->m_pIAnn, TRUE);
            }
            IANNUNCIATOR_Redraw(pMe->m_pIAnn);
        }
        (void)IDISPLAY_SetPrefs(pMe->m_pDisplay, "a:1", STRLEN("a:1"));
#else
        if (dlgResId == IDD_MAIN_MENU)
        {
            (void)IDISPLAY_SetPrefs(pMe->m_pDisplay, "a:0", STRLEN("a:0"));
        }
        else
        {
            (void)IDISPLAY_SetPrefs(pMe->m_pDisplay, "a:1", STRLEN("a:1"));
        }
#endif
        ISHELL_GetDeviceInfo(pMe->m_pShell, &di);
        pMe->m_rc.dx = di.cxScreen;
        pMe->m_rc.dy = di.cyScreen;
        IDISPLAY_SetClipRect(pMe->m_pDisplay, &pMe->m_rc);
        CalculateScreenParameters(pMe);
    }
    
    nRet = ISHELL_CreateDialog(pMe->m_pShell,MAINMENU_RES_FILE_LANG,dlgResId,NULL);
    if (nRet != SUCCESS)
    {
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
            
        default:
            return FALSE;
    }
}

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
        case EVT_DIALOG_INIT:			
            IDIALOG_SetProperties((IDialog *)dwParam, DLG_NOT_REDRAW_AFTER_START);
            return TRUE;

        case EVT_DIALOG_START:			
#ifdef FEATURE_RANDOM_MENU_COLOR
            (void)OEM_GetConfig(CFGI_MENU_BGCOLOR, &pMe->m_nBgColor, sizeof(pMe->m_nBgColor));
#endif
            if(pMe->m_pImageBg == NULL)
            {
#ifdef FEATURE_RANDOM_MENU_COLOR
                (void)OEM_GetConfig(CFGI_RANDOM_MENU, (void*)&pMe->m_nRandomMenu, sizeof(pMe->m_nRandomMenu));
				#if 0
                if(pMe->m_nRandomMenu != 0)
                {
                    pMe->m_pImageBg = ISHELL_LoadResImage(pMe->m_pShell, AEE_APPSCOMMONRES_IMAGESFILE, IDI_MENU_BACKGROUND_TRANS);
                }
                else
				#endif 
#endif
                {
#if defined FEATURE_VERSION_H19C  
                    pMe->m_pImageBg = ISHELL_LoadImage(pMe->m_pShell,ICON_ANI[0]);
#else
                    pMe->m_pImageBg = ISHELL_LoadResImage(pMe->m_pShell, AEE_APPSCOMMONRES_IMAGESFILE, IDB_BACKGROUND);//modi by yangdecai
#endif                    
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
           // ERR("EVT_USER_REDRAW:::::::::::1111111111",0,0,0);			
            DrawMatrix(pMe);
            
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
                ISHELL_CancelTimer(pMe->m_pShell, NULL, (void**)pMe);
            }
            return TRUE;


        case EVT_KEY:
            //ISHELL_CancelTimer(pMe->m_pShell, NULL, (void**)pMe);
            switch( wParam)
            {
                case AVK_CLR:
                   // ERR("AVK_CLR:::::::::::::::::::!111111111111",0,0,0);
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
                        int Focus = pMe->m_nRow * MAX_MATRIX_COLS + pMe->m_nColumn;
                        StartApplet(pMe, pMe->m_IconTitle[Focus]);
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
                    StartApplet(pMe, pMe->m_IconTitle[wParam - AVK_1]);
                    return TRUE;
                    
                case AVK_0:
                    StartApplet(pMe, pMe->m_IconTitle[9]);
                    return TRUE;

                case AVK_STAR:
                    StartApplet(pMe, pMe->m_IconTitle[10]);
                    return TRUE;
                    
                case AVK_POUND:
                    StartApplet(pMe, pMe->m_IconTitle[11]);
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
FUNCTION:  CalculateScreenParameters

DESCRIPTION: ���㽹��ͼƬ�Լ�Ĭ��ͼƬ������

=============================================================================*/
static void CalculateScreenParameters(MainMenu *pMe)
{
    AEEImageInfo    imageInfoIcon;
    uint16          iconSpaceHorizontal = 0;
    uint16          iconSpaceVertical   = 0;
    uint8           i                   = 0;

    //����Ĭ��ͼƬ������
    imageInfoIcon.cx = ICON_WIDTH;
    imageInfoIcon.cy = ICON_HEIGHT;

    /* icon size in all might be larger than screen*/
#ifdef FEATURE_VERSION_H19C   
    iconSpaceHorizontal = 12;
    iconSpaceVertical = 2;
#else
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
        iconSpaceVertical   = (pMe->m_rc.dy - TITLEBAR_HEIGHT*2 - imageInfoIcon.cy * MAX_MATRIX_ROWS) / (MAX_MATRIX_ROWS+1);
    }
    else
    {
        iconSpaceVertical = 0;
    }
#endif
    for( i = 0; i < MAX_MATRIX_ITEMS; i ++)
    {
        pMe->m_Icondefault_Pt[i].x = iconSpaceHorizontal +
            ( imageInfoIcon.cx + iconSpaceHorizontal) * ( i % MAX_MATRIX_COLS);
        
        pMe->m_Icondefault_Pt[i].y = TITLEBAR_HEIGHT + iconSpaceVertical +
            ( imageInfoIcon.cy + iconSpaceVertical) * ( i / MAX_MATRIX_COLS);

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
                                                    0, 
                                                    0);
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
FUNCTION:  DrawMatrix

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
    MainMenu_DrawBackGround(pMe, &pMe->m_rc);
#ifndef FEATURE_VERSION_H19C  
    //Draw icon
    for (i = 0; i < MAX_MATRIX_ITEMS; i ++)
    {
        if (pMe->m_pImageIcon[i] == NULL)
        {
            pMe->m_pImageIcon[i] = ISHELL_LoadImage(pMe->m_pShell,
                                                    ICON_ANI[i]);
        }

        if (pMe->m_pImageIcon[i] != NULL)
        {
            IIMAGE_Draw(pMe->m_pImageIcon[i],
                        pMe->m_Icondefault_Pt[i].x,
                        pMe->m_Icondefault_Pt[i].y);
        }
    }  
#endif
    BarParam.eBBarType = BTBAR_SELECT_BACK;
    DrawBottomBar(pMe->m_pDisplay, &BarParam);//wlh 20090412 add
}

/*=============================================================================
FUNCTION:  DrawFocusIcon

DESCRIPTION: 

PARAMETERS:

RETURN VALUE:

COMMENTS:


=============================================================================*/
static void DrawFocusIcon(MainMenu *pMe)
{
    int theFocus = pMe->m_nRow * MAX_MATRIX_COLS + pMe->m_nColumn;
    TitleBar_Param_type  titleBarParms;
    
    if (NULL == pMe)
    {
        return;
    }
            
    MEMSET( &titleBarParms, 0, sizeof( TitleBar_Param_type));
    
    titleBarParms.dwAlignFlags  = IDF_TEXT_TRANSPARENT | IDF_ALIGN_CENTER | IDF_ALIGN_MIDDLE;
    STRCPY( titleBarParms.strTitleResFile, MAINMENU_RES_FILE_LANG);
	titleBarParms.nTitleResID   = pMe->m_IconTitle[theFocus];
    DrawTitleBar(pMe->m_pDisplay, &titleBarParms);
    
 #if defined FEATURE_VERSION_H19C
    if(pMe->m_pAnimate != NULL)
    {
        IIMAGE_Stop(pMe->m_pAnimate);
        IIMAGE_Release(pMe->m_pAnimate);
        pMe->m_pAnimate = NULL;
    }
#endif    
    if(pMe->m_pAnimate == NULL)
    {
        pMe->m_pAnimate = ISHELL_LoadImage(pMe->m_pShell, ICON_ANI_1[theFocus]);
    }

	if( pMe->m_pAnimate != NULL)
    {
#if defined FEATURE_VERSION_H19C
        IIMAGE_Start(pMe->m_pAnimate,
                     pMe->m_IconFocus_Pt[theFocus].x, 
                     pMe->m_IconFocus_Pt[theFocus].y);
#else        
		IIMAGE_Draw(pMe->m_pAnimate,
                    pMe->m_IconFocus_Pt[theFocus].x, 
                    pMe->m_IconFocus_Pt[theFocus].y);
	    IIMAGE_Release(pMe->m_pAnimate);
        pMe->m_pAnimate = NULL;
#endif       
	    IDISPLAY_UpdateEx(pMe->m_pDisplay, TRUE);
	}
}

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
#ifndef FEATURE_VERSION_H19C
    int theFocus = pMe->m_nRow * MAX_MATRIX_COLS + pMe->m_nColumn;
    AEERect rect;
    
    // ���ƾ۽�������ʼ����
    SETAEERECT(&rect, pMe->m_IconFocus_Pt[theFocus].x, 
                      pMe->m_IconFocus_Pt[theFocus].y, 
                      ICON_ANIMATED_WIDTH, 
                      ICON_ANIMATED_HEIGHT);
    
    MainMenu_DrawBackGround(pMe, &rect);
    
    if (pMe->m_pImageIcon[theFocus])
    {
        IIMAGE_Draw(pMe->m_pImageIcon[theFocus],
                    pMe->m_Icondefault_Pt[theFocus].x, 
                    pMe->m_Icondefault_Pt[theFocus].y);
    }
#endif
    // ��ʼ�۽���������
    SetCursor(pMe, row, column);
    DrawFocusIcon(pMe);
}
/*=============================================================================
FUNCTION:  SetCursor

DESCRIPTION: 

PARAMETERS:

RETURN VALUE:

COMMENTS:

SIDE EFFECTS:

SEE ALSO:

=============================================================================*/
static void SetCursor(MainMenu *pMe, int row, int column)
{
    pMe->m_nRow        = row;
    pMe->m_nColumn     = column;
    
    if(pMe->m_pAnimate != NULL)
    {
        IImage_Release(pMe->m_pAnimate);
        pMe->m_pAnimate  = NULL;
    }
}

/*=============================================================================
FUNCTION:  StartApplet

DESCRIPTION:  ����applet

PARAMETERS:  ���APPLET �б䶯��ֻ��Ķ��κ���

=============================================================================*/
static int StartApplet(MainMenu *pMe, int i)
{
    int Result = EUNSUPPORTED;
    
#ifdef FEATURE_VERSION_H19C  
    if(pMe->m_pIAnn != NULL)
    {
        IANNUNCIATOR_SetHasTitleText(pMe->m_pIAnn, TRUE);
    }
#endif
    switch(i){
    case IDS_MAIN_MENU_GALLERY:
    case IDS_MAIN_MENU_MEDIAGALLERY:
        Result = ISHELL_StartApplet(pMe->m_pShell, AEECLSID_MEDIAGALLERY);
        break;
        
    case IDS_MAIN_MENU_CONTACT:
    case IDS_MAIN_MENU_CONTACTS:
        {
            IContactApp *ca = NULL;
            Result = ISHELL_CreateInstance(pMe->m_pShell,AEECLSID_APP_CONTACT, (void**)&ca);
            if(SUCCESS == Result)
            {
                ICONTAPP_MainMenu(ca);
                IContactApp_Release(ca);
            }
        }
        break;
        
    case IDS_MAIN_MENU_SMARTFRENACCESS:
    case IDS_MAIN_MENU_UTK:
        Result = ISHELL_StartApplet(pMe->m_pShell, AEECLSID_APP_UTK);
        break;
        
    case IDS_MAIN_MENU_TOOLS:
    case IDS_MAIN_MENU_APPLICATION:
        Result = ISHELL_StartApplet(pMe->m_pShell, AEECLSID_APPLICATION);
        break;
        
    case IDS_MAIN_MENU_RECENTCALLS:
        Result = ISHELL_StartApplet(pMe->m_pShell, AEECLSID_APP_RECENTCALL);
        break;
        
    case IDS_MAIN_MENU_SMS:
    case IDS_MAIN_MENU_MESSAGES:
        Result = ISHELL_StartApplet(pMe->m_pShell, AEECLSID_WMSAPP);
        break;
        
    case IDS_MAIN_MENU_MULTIMEDIA:
        Result = ISHELL_StartApplet(pMe->m_pShell, AEECLSID_MULTIMEDIA_LIST);
        break;
        
    case IDS_MAIN_MENU_STATIC_APPLICATION:
#ifdef FEATURE_FLEXI_STATIC_BREW_APP
        Result = ISHELL_StartApplet(pMe->m_pShell, AEECLSID_STATIC_APP);
#else
        Result = ISHELL_StartApplet(pMe->m_pShell, AEECLSID_APPMANAGER);
#endif
        break;
        
    case IDS_MAIN_MENU_CALENDAR:
    case IDS_MAIN_MENU_SCHEDULER:
        Result = ISHELL_StartApplet(pMe->m_pShell, AEECLSID_SCHEDULEAPP);
        break;
        
    case IDS_MAIN_MENU_SETTINGS:
        Result = ISHELL_StartApplet(pMe->m_pShell, AEECLSID_APP_SETTINGMENU);
        break;
        
    case IDS_MAIN_MENU_USERPROFILE:
        Result = ISHELL_StartApplet(pMe->m_pShell, AEECLSID_APP_SOUNDMENU);
        break;
        
    case IDS_MAIN_MENU_GAMES:
        Result = ISHELL_StartApplet(pMe->m_pShell, AEECLSID_GAME);
        break;
        
    case IDS_MAIN_MENU_CALCULATOR:
        Result = ISHELL_StartApplet(pMe->m_pShell, AEECLSID_CALCAPP);
        break;
        
    case IDS_MAIN_MENU_FMRADIO:
        Result = ISHELL_StartApplet(pMe->m_pShell, AEECLSID_APP_FMRADIO);
        break;
        
    case IDS_MAIN_MENU_MUSICPLAYER:
        Result = ISHELL_StartApplet(pMe->m_pShell, AEECLSID_APP_MUSICPLAYER);
        break;
        
    case IDS_MAIN_MENU_SMARTWORLD:
    case IDS_MAIN_MENU_BLIVE:
#ifdef FEATURE_SMARTFREN_STATIC_BREW_APP
		Result = ISHELL_StartApplet(pMe->m_pShell, AEECLSID_STATIC_APP);
#else
        Result = ISHELL_StartApplet(pMe->m_pShell, AEECLSID_APPMANAGER);
#endif  
        break;
    
    case IDS_MAIN_MENU_FRENDUO:
#ifdef FEATURE_VERSION_M8
		Result = ISHELL_StartApplet(pMe->m_pShell, AEECLSID_FRENDUO);
#endif
        break;
    
    case IDS_MAIN_MENU_TIMER:
        Result = ISHELL_StartApplet(pMe->m_pShell, AEECLSID_APPTIMER);
        break;
        
    default:
        break;

    }
    return Result;
}

