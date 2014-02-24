/** ----------------------------------------------------------------------------
 *Game.c
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
#include "game.h"
#include "game_priv.h" 

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
static GameMod gGameMod;
static const VTBL( IModule) gModFuncs =
{
    GameMod_AddRef,
    GameMod_Release,
    GameMod_CreateInstance,
    GameMod_FreeResources
};

static uint32  Game_AddRef( IGame *pi);

static uint32  Game_Release ( IGame *pi);

static boolean Game_HandleEvent( IGame *pi,
    AEEEvent eCode,
    uint16  wParam,
    uint32 dwParam
);

static int     Game_MainMenuService( IGame *pi, GameServiceType eStype);

static Game gGame={0};

static const VTBL( IGame) gGameMethods =
{
    Game_AddRef,
    Game_Release,
    Game_HandleEvent,
    Game_MainMenuService
};

int GameMod_Load( IShell *pIShell, void *ph, IModule **ppMod);

int GameMod_New( int16 nSize,
    IShell              *pIShell,
    void                *ph,
    IModule             **ppMod,
    PFNMODCREATEINST    pfnMC,
    PFNFREEMODDATA      pfnMF
);

static int Game_New( IShell *ps, IModule *pIModule, IGame **ppObj);

static int CGame_InitAppData(Game *pMe);

static void CGame_FreeAppData(Game *pMe);

static void Game_RunFSM(Game *pMe);

static boolean StartApplet(Game *pMe, int i);

boolean Game_RouteDialogEvt(Game *pMe,
    AEEEvent    eCode,
    uint16  wParam,
    uint32 dwParam
);

NextFSMAction Game_ProcessState(Game *pMe);

void Game_ShowDialog(Game  *pMe,  uint16 dlgResId);

// GAMEST_MAIN ״̬������
static NextFSMAction GAMEST_MAIN_Handler(Game *pMe);


//GAMEST_EXIT  ״̬������
static NextFSMAction GAMEST_EXIT_Handler(Game *pMe);
static boolean  Game_ListMenuHandler(Game *pMe, AEEEvent eCode, uint16 wParam, uint32 dwParam);



/*==============================================================================
                              
                              ���أ���̬������
                              
==============================================================================*/



//wlh 20090410 add  end �ƶ�Ч��
/*=============================================================================
FUNCTION:  gameMod_Load

DESCRIPTION: 

PARAMETERS:

RETURN VALUE:

COMMENTS:

SIDE EFFECTS:

SEE ALSO:

=============================================================================*/
int GameMod_Load( IShell *pIShell, void *ph, IModule **ppMod)
{
    return GameMod_New( sizeof( GameMod),
                    pIShell,
                    ph,
                    ppMod,
                    NULL,
                    NULL);
}

/*=============================================================================
FUNCTION:  GameMod_New

DESCRIPTION: 

PARAMETERS:

RETURN VALUE:

COMMENTS:

SIDE EFFECTS:

SEE ALSO:

=============================================================================*/
int  GameMod_New( int16 nSize,
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

    MEMSET( &gGameMod, 0, sizeof( GameMod));

    INIT_VTBL( &gGameMod, IModule, gModFuncs);
    gGameMod.referenceCounter = 1;
    *ppMod = ( IModule *)&gGameMod;
    return AEE_SUCCESS;
}

/*=============================================================================
FUNCTION:  GameMod_AddRef

DESCRIPTION: 

PARAMETERS:

RETURN VALUE:

COMMENTS:

SIDE EFFECTS:

SEE ALSO:

=============================================================================*/
static uint32 GameMod_AddRef( IModule *po)
{
    return( ++( ( GameMod *)po)->referenceCounter);
}

/*=============================================================================
FUNCTION:  GameMod_Release

DESCRIPTION: 

PARAMETERS:

RETURN VALUE:

COMMENTS:

SIDE EFFECTS:

SEE ALSO:

=============================================================================*/
static uint32 GameMod_Release( IModule *po)
{
    if ( ( ( GameMod *) po)->referenceCounter == 0)
    {
        return 0;
    }

    return( --( ( GameMod *)po)->referenceCounter);
}

/*=============================================================================
FUNCTION:  GameMod_CreateInstance

DESCRIPTION: 

PARAMETERS:

RETURN VALUE:

COMMENTS:

SIDE EFFECTS:

SEE ALSO:

=============================================================================*/
static int  GameMod_CreateInstance( IModule *po,
    IShell *pIShell,
    AEECLSID ClsId,
    void **ppObj
)
{
    *ppObj = NULL;

    if( ClsId != AEECLSID_GAME || Game_New( pIShell, po, ( IGame**)ppObj) != SUCCESS)
    {
        return EFAILED;
    }

    return SUCCESS;
}

/*=============================================================================
FUNCTION:  GameMod_FreeResources

DESCRIPTION: 

PARAMETERS:

RETURN VALUE:

COMMENTS:

SIDE EFFECTS:

SEE ALSO:

=============================================================================*/
static void GameMod_FreeResources( IModule *po, IHeap *ph, IFileMgr *pfm)
{
}

/*=============================================================================
FUNCTION:  Game_New

DESCRIPTION: 

PARAMETERS:

RETURN VALUE:

COMMENTS:

SIDE EFFECTS:

SEE ALSO:

=============================================================================*/
static int Game_New( IShell *ps, IModule *pIModule, IGame **ppObj)
{
    int retVal = SUCCESS;
    if( 0 == gGame.referenceCounter)
    {

        if( pIModule == NULL || ps == NULL)
        {
            return EFAILED;
        }

        MEMSET(&gGame, 0, sizeof(gGame));
        INIT_VTBL(&gGame, IGame, gGameMethods);

       
        gGame.m_pShell     = ps;
        gGame.m_pModule    = pIModule;
        retVal = CGame_InitAppData(&gGame);
        if(retVal != SUCCESS)
        {
            CGame_FreeAppData((Game*)&gGame);
            return retVal;
        }
        (void) ISHELL_AddRef( ps);
        (void) IMODULE_AddRef( pIModule);
        gGame.referenceCounter  = 0;

        
    }

    ++ gGame.referenceCounter;
    *ppObj = ( IGame*)&gGame;
    
    return retVal;
}

/*==============================================================================
����:
    CGame_InitAppData

˵��:
    ��ʼ�� Game Applet �ṹ����ȱʡֵ��

����:
    pMe [in]: ָ�� Game Applet ����ṹ��ָ�롣�ýṹ����С������ض���Ϣ��

����ֵ:
    EFAILED: ��ʼ��ʧ�ܡ�
    SUCCESS: ��ʼ���ɹ���

��ע:

==============================================================================*/
static int CGame_InitAppData(Game *pMe)
{
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
	if (AEE_SUCCESS != ISHELL_CreateInstance(pMe->m_pShell,AEECLSID_ANNUNCIATOR,(void **)&pMe->m_pIAnn))
    {
        return EFAILED;
    }

    return SUCCESS;
}

/*==============================================================================
����:
    Game_FreeAppData

˵��:
    �ͷ� Game Applet �����Դ��

����:
    pMe [in]: ָ��Game Applet ����ṹ��ָ�롣�ýṹ����С������ض���Ϣ��

����ֵ:
    none

��ע:

==============================================================================*/
static void CGame_FreeAppData(Game *pMe)
{
    if (NULL == pMe)
    {
        return;
    }

    pMe->m_eAppStatus = GAME_STOP; 
    
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
FUNCTION:  Game_AddRef

DESCRIPTION: 

PARAMETERS:

RETURN VALUE:

COMMENTS:

SIDE EFFECTS:

SEE ALSO:

=============================================================================*/
static uint32  Game_AddRef( IGame *pi)
{
    return ( ++gGame.referenceCounter);
}

/*=============================================================================
FUNCTION:  Game_Release

DESCRIPTION: 

PARAMETERS:

RETURN VALUE:

COMMENTS:

SIDE EFFECTS:

SEE ALSO:

=============================================================================*/
static uint32  Game_Release( IGame *pi)
{
    register Game *pMe = (Game*)pi;

    if (pMe->referenceCounter == 0)
    {
        return 0;
    }
    
    if( --pMe->referenceCounter)
    {
        return pMe->referenceCounter;
    }


    CGame_FreeAppData(pMe);

    (void) ISHELL_Release(pMe->m_pShell);
    (void) IMODULE_Release(pMe->m_pModule);

    return 0;
}

/*==============================================================================
����:
    Game_RunFSM

˵��:
    ��������״̬�����档

����:
    pMe [in]: ָ��Application Applet����ṹ��ָ�롣�ýṹ����С������ض���Ϣ��
       
����ֵ:
    �ޡ�

��ע:

==============================================================================*/
static void Game_RunFSM(Game *pMe)
{
    NextFSMAction nextFSMAction = NFSMACTION_WAIT;
    for ( ; ; )
    {
        nextFSMAction = Game_ProcessState(pMe);

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
FUNCTION:  Game_HandleEvent

DESCRIPTION: 

PARAMETERS:

RETURN VALUE:

COMMENTS:

SIDE EFFECTS:

SEE ALSO:

=============================================================================*/
static boolean Game_HandleEvent( IGame *pi,
    AEEEvent eCode,
    uint16  wParam,
    uint32 dwParam
)
{
    Game *pMe = (Game*)pi;
    AEEAppStart* as = NULL;
    AEEDeviceInfo di; 

    ISHELL_GetDeviceInfo(pMe->m_pShell,&di); 
	MSG_FATAL("Game_HandleEvent...eCode=%x,wParam=%x,",eCode,wParam,0);
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
            pMe->m_currState  = GAMEST_MAIN;
            pMe->m_eDlgReturn = DLGRET_CREATE;
            pMe->m_eAppStatus = GAME_RUNNING;
            if(pMe->m_pIAnn != NULL)
            {
                IANNUNCIATOR_SetFieldIsActiveEx(pMe->m_pIAnn,FALSE); 
            }
            Game_RunFSM(pMe);
            return TRUE;

        case EVT_APP_STOP:
            {
                if (pMe->m_pDisplay != NULL)
                {
                    (void) IDISPLAY_Release(pMe->m_pDisplay);
                    pMe->m_pDisplay = NULL;
                }
                pMe->m_eAppStatus = GAME_STOP;
                
                return TRUE;
            }

        case EVT_APP_SUSPEND:
            pMe->m_eAppStatus = GAME_SUSPEND;
            return TRUE;
        

        case EVT_APP_RESUME:
            {
                AEEAppStart* as = ( AEEAppStart*)dwParam;
                pMe->m_rc    = as->rc;
                pMe->m_rc.dy = di.cyScreen;
                pMe->m_eAppStatus = GAME_RUNNING;
                Game_RunFSM(pMe); 
                return TRUE;
            }

        case EVT_DIALOG_INIT:
            
            pMe->m_pActiveIDlg = (IDialog*)dwParam;
            pMe->m_pActivedlgID = wParam;
            
            return Game_RouteDialogEvt(pMe,eCode,wParam,dwParam);

        case EVT_DIALOG_START:

            return Game_RouteDialogEvt(pMe,eCode,wParam,dwParam);

        case EVT_USER_REDRAW:
            return Game_RouteDialogEvt(pMe,eCode,wParam,dwParam);

        case EVT_DIALOG_END:
		  
            (void) Game_RouteDialogEvt(pMe,eCode,wParam,dwParam);
            pMe->m_pActiveIDlg = NULL;
            pMe->m_pActivedlgID = 0;
            if(pMe->m_eAppStatus == GAME_RUNNING)
            {
                Game_RunFSM(pMe);
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
            return Game_RouteDialogEvt(pMe,eCode,wParam,dwParam);   
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
					return Game_RouteDialogEvt(pMe,eCode,wParam,dwParam);
				}
#endif
        case EVT_PEN_UP:
			MSG_FATAL("00000000000000000000000000000 up....",0,0,0);
			return Game_RouteDialogEvt(pMe,eCode,wParam,dwParam); 
			break;
		case EVT_PEN_MOVE:
			MSG_FATAL("00000000000000000000000000000 move....",0,0,0);
			return Game_RouteDialogEvt(pMe,eCode,wParam,dwParam); 
			break;
		case EVT_PEN_DOWN:
        case EVT_KEY_PRESS:
        case EVT_KEY_RELEASE:
        case EVT_COMMAND:
			MSG_FATAL("00000000000000000000000000000",0,0,0);
            return Game_RouteDialogEvt(pMe,eCode,wParam,dwParam); 
        default:
            return Game_RouteDialogEvt(pMe,eCode,wParam,dwParam);
    }
}

/*==============================================================================
����:
   Game_ProcessState

˵��:
    ���� pMe->m_currState, ������Ӧ��״̬��������

����:
    pMe [in]: ָ��Game Applet����ṹ��ָ�롣�ýṹ����С������ض���Ϣ��

����ֵ:
    NFSMACTION_CONTINUE: ָʾ��ͣ״̬����
    NFSMACTION_WAIT: ָʾֹͣ״̬����

��ע:

==============================================================================*/
NextFSMAction Game_ProcessState(Game *pMe)
{
    NextFSMAction retVal = NFSMACTION_WAIT;
    
    if (NULL == pMe)
    {
        return retVal;
    }
    
    switch(pMe->m_currState)
    {
        case GAMEST_MAIN:
            return GAMEST_MAIN_Handler(pMe);

        case GAMEST_EXIT:
            return GAMEST_EXIT_Handler(pMe);            

        default:
            break;

    }
    return retVal;
}

/*==============================================================================
����:
    GAMEST_MAIN_Handler

˵��:
   GAMEST_MAIN ״̬��������

����:
    pMe [in]: ָ��GAME Applet����ṹ��ָ�롣�ýṹ����С������ض���Ϣ��

����ֵ:
    NFSMACTION_CONTINUE: ָʾ��ͣ״̬����
    NFSMACTION_WAIT: ָʾֹͣ״̬����

��ע:

==============================================================================*/
static NextFSMAction GAMEST_MAIN_Handler(Game *pMe)
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
                  Game_ShowDialog(pMe, IDD_LIST_GAME);

            }
            return NFSMACTION_WAIT;
        default:
            MOVE_TO_STATE(GAMEST_EXIT)
            return NFSMACTION_CONTINUE;
    }
}




/*==============================================================================
����:
    GAMEST_EXIT_Handler

˵��:
    GAMEST_EXIT ״̬��������

����:
    pMe [in]: ָ��GAME Applet����ṹ��ָ�롣�ýṹ����С������ض���Ϣ��

����ֵ:
    NFSMACTION_CONTINUE: ָʾ��ͣ״̬����
    NFSMACTION_WAIT: ָʾֹͣ״̬����

��ע:

==============================================================================*/
static NextFSMAction GAMEST_EXIT_Handler(Game *pMe)
{
    (void) ISHELL_CloseApplet(pMe->m_pShell, FALSE);
    return NFSMACTION_WAIT;
} 


/*==============================================================================
����:
       Game_ShowDialog

˵��:
    ������״̬����������������Ӧ�Ի�����ԴID�ĶԻ���

����:
    pMe [in]: ָ�� Game Applet ����ṹ��ָ�롣�ýṹ����С������ض���Ϣ��
    dlgResId [in]: �Ի�����ԴID���������ݸ�ID�����Ի���

����ֵ:
    none

��ע:

==============================================================================*/
void Game_ShowDialog(Game  *pMe,  uint16 dlgResId)
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
    nRet = ISHELL_CreateDialog(pMe->m_pShell,GAME_RES_FILE_LANG,dlgResId,NULL);
    if (nRet != SUCCESS)
    {
        return;
    }
}


/*==============================================================================
����:
      Game_RouteDialogEvt
˵��:
       ������BREW�¼�·�ɸ���ǰ��Ի�����¼���������

����:
       pMe [In]: ָ��Game Applet����ṹ��ָ��,�ýṹ����С������ض���Ϣ.
       eCode [in]���¼����롣
       wParam [in]�����¼�eCode���������ݡ�
       dwParam [in]�����¼�eCode���������ݡ�

����ֵ:
       TRUE�������¼�������
       FALSE�������¼�û������

��ע:

==============================================================================*/
boolean Game_RouteDialogEvt(Game *pMe,
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

        case IDD_LIST_GAME:
            return Game_ListMenuHandler(pMe, eCode, wParam, dwParam);

        default:
            return FALSE;
    }
}



/*=============================================================================
FUNCTION:  Game_ListMenuHandler

DESCRIPTION:   �б�˵�

PARAMETERS:

=============================================================================*/

static boolean Game_ListMenuHandler(Game *pMe, AEEEvent eCode, uint16 wParam, uint32 dwParam)
{
    PARAM_NOT_REF(dwParam)
    IMenuCtl *pMenu = (IMenuCtl*)IDIALOG_GetControl(pMe->m_pActiveIDlg,IDC_GAME_LIST);
    MSG_FATAL("Game_ListMenuHandler........00000",0,0,0);
    if (pMenu == NULL)
    {
        return FALSE;
    }
    MSG_FATAL("Game_ListMenuHandler........11111==%d",eCode,0,0);
    switch (eCode)
    {
        case EVT_DIALOG_INIT:
			#if 0
            IMENUCTL_SetTitle(pMenu, GAME_RES_FILE_LANG, IDS_GAME_LIST, NULL);
			#else
		    {
		  		AECHAR WTitle[40] = {0};
				#if defined(FEATURE_VERSION_K292_WSF_CN)
				(void)ISHELL_LoadResString(pMe->m_pShell,
                        GAME_RES_FILE_LANG,                                
                        IDS_GAME_LIST_909C,
                        WTitle,
                        sizeof(WTitle));
				#else
				(void)ISHELL_LoadResString(pMe->m_pShell,
                        GAME_RES_FILE_LANG,                                
                        IDS_GAME_LIST,
                        WTitle,
                        sizeof(WTitle));
				#endif
                if(pMe->m_pIAnn != NULL)
                {
				    IANNUNCIATOR_SetFieldTextEx(pMe->m_pIAnn,WTitle,FALSE);
                }
		    }
			#endif     
#if	defined(FEATURE_VERSION_K212)||defined(FEATURE_VERSION_K212_BH)
			IMENUCTL_AddItem(pMenu, GAME_RES_FILE_LANG,IDS_GAME_TITLE_TETRIS, IDS_GAME_TITLE_TETRIS, NULL, 0);
#else
#ifdef FEATURE_VERSION_VG68
			IMENUCTL_AddItem(pMenu, GAME_RES_FILE_LANG,IDS_GAME_TITLE_TETRIS, IDS_GAME_TITLE_TETRIS, NULL, 0);
#endif
#ifdef FEATURE_VERSION_C316      
            IMENUCTL_AddItem(pMenu, GAME_RES_FILE_LANG,IDS_GAME_TITLE_BRICK, IDS_GAME_TITLE_BRICK, NULL, 0);
            //IMENUCTL_AddItem(pMenu, GAME_RES_FILE_LANG,IDS_GAME_TITLE_GMFIVE, IDS_GAME_TITLE_GMFIVE, NULL, 0);
            IMENUCTL_AddItem(pMenu, GAME_RES_FILE_LANG,IDS_GAME_TITLE_TETRIS, IDS_GAME_TITLE_TETRIS, NULL, 0);                         
#else
#ifndef FEATURE_VERSION_W021_CT100_QVGA
            IMENUCTL_AddItem(pMenu, GAME_RES_FILE_LANG,IDS_GAME_TITLE_BLACKJACK, IDS_GAME_TITLE_BLACKJACK, NULL, 0);
#endif
            IMENUCTL_AddItem(pMenu, GAME_RES_FILE_LANG,IDS_GAME_TITLE_BRICK, IDS_GAME_TITLE_BRICK, NULL, 0);

#endif

#ifndef FEATURE_VERSION_W208S			
        #ifdef FEATURE_SMARTFREN_MAGIC
            IMENUCTL_AddItem(pMenu, GAME_RES_FILE_LANG,IDS_GAME_TITLE_MAGICSUSHI, IDS_GAME_TITLE_MAGICSUSHI, NULL, 0);
        #endif

        #ifdef FEATURE_SMARTFREN_TOMB
        	IMENUCTL_AddItem(pMenu, GAME_RES_FILE_LANG,IDS_GAME_TITLE_BOMB, IDS_GAME_TITLE_BOMB, NULL, 0);
		#endif
			//IMENUCTL_AddItem(pMenu, GAME_RES_FILE_LANG,IDS_GAME_TITLE_FIVEBALL, IDS_GAME_TITLE_FIVEBALL, NULL, 0);
            //IMENUCTL_AddItem(pMenu, GAME_RES_FILE_LANG,IDS_GAME_TITLE_GMFIVE, IDS_GAME_TITLE_GMFIVE, NULL, 0);
            #ifdef FEATURE_VERSION_X3
			IMENUCTL_AddItem(pMenu, GAME_RES_FILE_LANG,IDS_GAME_TITLE_PINTU, IDS_GAME_TITLE_PINTU, NULL, 0);
            IMENUCTL_AddItem(pMenu, GAME_RES_FILE_LANG,IDS_GAME_TITLE_TETRIS, IDS_GAME_TITLE_TETRIS, NULL, 0);
			#endif
#endif			
#ifdef FEATURE_VERSION_ESIA
            IMENUCTL_AddItem(pMenu, GAME_RES_FILE_LANG,IDS_GAME_TITLE_TETRIS, IDS_GAME_TITLE_TETRIS, NULL, 0);
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
#if !defined (FEATURE_VERSION_C337) &&  !defined (FEATURE_VERSION_IC241A_MMX)&& !defined(FEATURE_VERSION_K232_Y100A)
                (void)STRTOWSTR("%d. ",wsFmt,sizeof(wsFmt));
                for (i=0;i<IMENUCTL_GetItemCount(pMenu);i++)
                {
                    wID = IMENUCTL_GetItemID(pMenu, i);
                    WSPRINTF(pwsz,sizeof(pwsz),wsFmt,i+1);
                    
                    ISHELL_LoadResString( pMe->m_pShell,
                          GAME_RES_FILE_LANG,
                          wID,
                          pstr,
                          sizeof(pstr));
                    WSTRLCAT(pwsz,pstr,sizeof(pwsz));
                    IMENUCTL_SetItemText(pMenu, wID, NULL, NULL, pwsz);
                }
                
                IMENUCTL_SetProperties(pMenu, MP_UNDERLINE_TITLE|MP_WRAPSCROLL);
#else
				IMENUCTL_SetProperties(pMenu, MP_UNDERLINE_TITLE|MP_WRAPSCROLL|MP_BIND_ITEM_TO_NUMBER_KEY|MP_ACTIVE_NO_REDRAW);
#endif
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
                case IDS_GAME_TITLE_BLACKJACK:
                case IDS_GAME_TITLE_BRICK:
                case IDS_GAME_TITLE_MAGICSUSHI:
                case IDS_GAME_TITLE_BOMB:
				case IDS_GAME_TITLE_FIVEBALL:
				case IDS_GAME_TITLE_GMFIVE:
				case IDS_GAME_TITLE_PINTU:
				case IDS_GAME_TITLE_TETRIS:
                    StartApplet(pMe,wParam);
                    return TRUE;
            }
            return TRUE;
#ifdef FEATURE_LCD_TOUCH_ENABLE//andrew add for LCD touch
		case EVT_PEN_UP:
			{
				AEEDeviceInfo devinfo;
				int nBarH ;
				AEERect rc;
				int16 wXPos = (int16)AEE_GET_X(dwParam);
				int16 wYPos = (int16)AEE_GET_Y(dwParam);
                MSG_FATAL("Game_ListMenuHandler........22222",0,0,0);
				nBarH = GetBottomBarHeight(pMe->m_pDisplay);
        
				MEMSET(&devinfo, 0, sizeof(devinfo));
				ISHELL_GetDeviceInfo(pMe->m_pShell, &devinfo);
				SETAEERECT(&rc, 0, devinfo.cyScreen-nBarH, devinfo.cxScreen, nBarH);
                MSG_FATAL("wXPos=====%d,wYPos=====%d",wXPos,wYPos,0);
				MSG_FATAL("rc====%d,,,=%d,,,=%d",rc.dx,rc.dy,rc.y);
				if(TOUCH_PT_IN_RECT(wXPos,wYPos,rc))
				{
					if(wXPos >= rc.x && wXPos < rc.x + (rc.dx/3) )//��
					{
						
						boolean rt =  ISHELL_PostEvent(pMe->m_pShell,AEECLSID_GAME,EVT_KEY,AVK_SELECT,0);
						MSG_FATAL(".......................1111",0,0,0);
						return rt;
					}
					else if(wXPos >= rc.x + (rc.dx/3)   && wXPos < rc.x + (rc.dx/3)*2 )//��
					{
						 boolean rt = ISHELL_PostEvent(pMe->m_pShell,AEECLSID_GAME,EVT_KEY,AVK_INFO,0);
						 MSG_FATAL(".......................2222",0,0,0);
						 return rt;
					}
					else if(wXPos >= rc.x + (rc.dx/3)*2 && wXPos < rc.x + (rc.dx/3)*3 )//��
					{						
						 boolean rt = ISHELL_PostEvent(pMe->m_pShell,AEECLSID_GAME,EVT_KEY,AVK_CLR,0);
						 MSG_FATAL(".......................33333",0,0,0);
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
static boolean StartApplet(Game *pMe, int i)
{
    int Result = FALSE;

   	//MSG_ERROR("StartApplet:::::%d",i,0,0);
   	MSG_FATAL("***zzg Game StartApplet:::::%d***",i,0,0);
    switch(i)
    {
        case IDS_GAME_TITLE_BLACKJACK:
            Result = ISHELL_StartApplet(pMe->m_pShell, AEECLSID_BLACKJACK);
            break;
            
        case IDS_GAME_TITLE_BRICK:
            Result = ISHELL_StartApplet(pMe->m_pShell, AEECLSID_BRICK);
            break;
		case IDS_GAME_TITLE_FIVEBALL:
            Result = ISHELL_StartApplet(pMe->m_pShell, AEECLSID_FIVEBALL);
            break;
            
        case IDS_GAME_TITLE_GMFIVE:
            Result = ISHELL_StartApplet(pMe->m_pShell, AEECLSID_GMFIVE);
            MSG_FATAL("***zzg Game Start AEECLSID_GMFIVE Result=%d***",Result,0,0);
            break;
		case IDS_GAME_TITLE_PINTU:
            Result = ISHELL_StartApplet(pMe->m_pShell, AEECLSID_PINTU);
            break;
            
        case IDS_GAME_TITLE_TETRIS:
            Result = ISHELL_StartApplet(pMe->m_pShell, AEECLSID_TETRIS);
            MSG_FATAL("***zzg Game Start AEECLSID_TETRIS Result=%d***",Result,0,0);
            break;
           #if defined (FEATURE_SMARTFREN_STATIC_BREW_APP)
        case IDS_GAME_TITLE_MAGICSUSHI:
            Result = ISHELL_StartApplet(pMe->m_pShell, AEECLSID_SMARTFREN_MAGICSUSHI);
            break;

        case IDS_GAME_TITLE_BOMB:
            Result = ISHELL_StartApplet(pMe->m_pShell, AEECLSID_SMARTFREN_BOMB);
            break;
            #endif
		
        default:
            break;
    }
    return TRUE;
}

/*=============================================================================
FUNCTION:  Game_MainMenuService

DESCRIPTION:  �ⲿ�ӿڵ��ã����ٽ��뵽Gameĳһ���˵��б�

PARAMETERS:    
            eStype: ����app�ķ�ʽ

=============================================================================*/

static int  Game_MainMenuService( IGame *pi, GameServiceType eStype)
{
    char  *args = NULL;
    Game *pMe = (Game*)pi;
    int    nRet;  

    if (ISHELL_ActiveApplet(pMe->m_pShell) == AEECLSID_GAME)
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

    nRet = ISHELL_StartAppletArgs(pMe->m_pShell, AEECLSID_GAME, args);
        
    FREEIF(args);
    args = NULL;
    return nRet;    
}

