#ifndef _SPORT_APP_C_
#define _SPORT_APP_C_
/*
********************************************************************************

ģ����       : <SportApplication>
�ļ���       : <SportSApp.c>
����ļ�     : 
�ļ�ʵ�ֹ��� : Sports Application
����         : 
�汾         : 1.0
--------------------------------------------------------------------------------
��ע         : 
--------------------------------------------------------------------------------
�޸ļ�¼ : 

******************************************************************************/
#include "SportSApp_priv.h"



/*----------------------ģ����غ�������---------------------*/
int SportsMod_Load(IShell *pIShell,void *ph,IModule **ppMod);
int SportsMod_New(int16  nSize,IShell *pIShell,void *ph,IModule  **ppMod,PFNMODCREATEINST pfnMC,
    				PFNFREEMODDATA pfnMF);
static int SportsMod_CreateInstance(IModule *po,IShell *pIShell,AEECLSID ClsId,void **ppObj);
static uint32 SportsMod_AddRef(IModule *po);
static uint32 SportsMod_Release(IModule *po);
static void SportsMod_FreeResources(IModule  *po,IHeap  *ph,IFileMgr *pfm);

/*----------------------Applet��غ�������---------------------*/
static int SportSApp_New(IShell *ps,IModule *pIModule,ISportsMenu **ppObj);
static uint32 SportSApp_AddRef(ISportsMenu *pi);
static uint32 SportSApp_Release (ISportsMenu *pi);
static boolean SportSApp_HandleEvent(ISportsMenu *pi,AEEEvent eCode,uint16  wParam,uint32 dwParam);

static void SportSApp_APPIsReadyTimer(void *pme);
static int SportSApp_InitAppData(CSportsMenu *pMe);
static void SportSApp_FreeAppData(CSportsMenu *pMe);
static void SportSApp_RunFSM(CSportsMenu *pMe);

#ifdef AEE_SIMULATOR
   uint32 g_dwAEEStdLibEntry;
#ifndef WIN32
   #error This applet was never intended to be used with the AEE Simulator.
#endif//WIN32
#endif

extern int Rendering_UpdateEx(void);//wlh 20090409 add
/*-------------------------------------------------------------------
            Static variable Definitions
-------------------------------------------------------------------*/
static SportsMenuMod gSportsMenuMod;

static const VTBL(IModule) gModFuncs =
{
    SportsMod_AddRef,
    SportsMod_Release,
    SportsMod_CreateInstance,
    SportsMod_FreeResources
};

// ֻ����һ��SettingMenuʵ����ÿ�δ���SettingMenu Appletʱ��
// ����ͬһ�ṹָ���BREW�㡣
static CSportsMenu gSportsMenu;

static const VTBL(ISportsMenu) gSportsMenuMethods =
{
    SportSApp_AddRef,
    SportSApp_Release,
    SportSApp_HandleEvent
};

/* =========================================================================
                     FUNCTION DEFINITIONS
   ========================================================================= */

/*----------------------ģ����غ�������---------------------*/

/*==============================================================================
����:
       SportsMod_Load

˵��:
       ģ��װ�غ�����

����:
       pIShell [in]��IShell�ӿ�ָ�롣
       ph ��û��ʹ��
       ppMod [out]��

����ֵ:
       int

��ע:

==============================================================================*/
#ifndef WIN32
#ifndef AEE_SIMULATOR
int SportsMod_Load(IShell *pIShell,void *ph,IModule **ppMod)
#else
__declspec(dllexport) int AEEMod_Load(IShell *pIShell,void *ph,IModule **ppMod)

#endif
#else
int SportsMod_Load(IShell *pIShell,void *ph,IModule **ppMod)
#endif//WIN32
{
    // ���ڶ�̬Applet����֤AEE STD��汾����ȷ�����뽨����ģ��ʱʹ�õĿ�汾ƥ��
    // ����AEE_SIMULATOR�²��ؽ��б���顣
#if !defined(BREW_STATIC_APP) && !defined(AEE_SIMULATOR)
    if (GET_HELPER_VER() != AEESTDLIB_VER)
    {
        return EVERSIONNOTSUPPORT;
    }
#endif
    return SportsMod_New(sizeof(SportsMenuMod),pIShell,ph,ppMod,NULL,NULL);
}

/*==============================================================================
����:
       SportsMod_New

˵��:
       ������ʼ��SettingMenuģ�������Ҳ�������ü����������ֺ�����

����:
       nSize:        û��ʹ��
       pIShell [In]: IShell�ӿ�ָ��
       ph ��û��ʹ��
       ppMod [out]��
       pfnMC:        û��ʹ��
       pfnMF:        û��ʹ��

����ֵ:
       int

��ע:

==============================================================================*/
int  SportsMod_New(int16 nSize,IShell *pIShell,void *ph,IModule **ppMod,
 					PFNMODCREATEINST pfnMC,PFNFREEMODDATA pfnMF)
{
    PARAM_NOT_REF(nSize)
    PARAM_NOT_REF(pfnMC)
    PARAM_NOT_REF(pfnMF)
#ifndef AEE_SIMULATOR
    PARAM_NOT_REF(ph)
#endif
    if (NULL == ppMod)
    {
        return EFAILED;
    }
    *ppMod = NULL;

    // ��Ҫ��ʾ��:
    //        g_dwAEEStdLibEntryȫ�ֱ���������SDK ��
    //        BREW SDK�û����ܸ�д��ֱ��ʹ�ñ�������������AEEStdLib��һ����ڵ�
    //        ��
    
#ifdef AEE_SIMULATOR
    g_dwAEEStdLibEntry = (uint32)ph;
    if (!pIShell || !ph)
    {
        return EFAILED;
    }
#else
    if (!pIShell)
    {
        return EFAILED;
    }
#endif
    MEMSET(&gSportsMenuMod,0,sizeof(SportsMenuMod));
    INIT_VTBL(&gSportsMenuMod,IModule,gModFuncs);
    gSportsMenuMod.m_nRefs = 1;
    *ppMod = (IModule *)&gSportsMenuMod;
    return AEE_SUCCESS;
}

/*==============================================================================
����:
       SportsMod_AddRef

˵��:
       Sports ģ�����ü�����1�������ص�ǰ����������

����:
       po [in]��IModule�ӿڶ���ָ�롣

����ֵ:
       ��ǰ��Sports ģ���������

��ע:

==============================================================================*/
static uint32 SportsMod_AddRef(IModule *po)
{
    return(++((SportsMenuMod *)po)->m_nRefs);
}

/*==============================================================================
����:
       SportsMod_Release

˵��:
       Sports ģ�����ü�����1�������ص�ǰ����������

����:
       po [in]��IModule�ӿڶ���ָ�롣

����ֵ:
       ��ǰ��Sports ģ���������

��ע:

==============================================================================*/
static uint32 SportsMod_Release(IModule *po)
{
    if (((SportsMenuMod *) po)->m_nRefs == 0)
    {
        return 0;
    }

    return(--((SportsMenuMod *)po)->m_nRefs);
}

/*==============================================================================
����:
       SportsMod_CreateInstance

˵��:
        ͨ������(��ʼ��)Sports����������ģ��ʵ����

����:
       po [in]��IModule�ӿڶ���ָ�롣
       pIShell [in]��IShell�ӿڶ���ָ�롣
       ClsId [in]��Ҫ������ʵ������ID��
       ppObj [out]:�����ڱ������ʵ����ָ���ָ�������

����ֵ:
       EFAILED;����ʧ�ܡ�
       SUCCESS;�����ɹ���

��ע:
       �����û�ȥ���á�

==============================================================================*/
static int  SportsMod_CreateInstance(IModule *po,
    IShell *pIShell,
    AEECLSID ClsId,
    void **ppObj
)
{
    *ppObj = NULL;

    if (ClsId != AEECLSID_SPORTSAPP)
    {
        return EFAILED;
    }

    if (SportSApp_New(pIShell, po, (ISportsMenu**)ppObj) != SUCCESS)
    {
        return EFAILED;
    }
    return SUCCESS;
}

/*==============================================================================
����:
       SportsMod_FreeResources

˵��:
        �ͷ�Sportsģ����Դ�ĺ�����

����:
       po ��IModule��
       ph ��IHeap�ӿ�ָ�롣
       pfm :IFileMgr�ӿ�ָ�롣

����ֵ:
       ��

��ע:
       �����û�ȥ���á�

==============================================================================*/
static void SportsMod_FreeResources(IModule * po,IHeap * ph,IFileMgr * pfm)
{

    PARAM_NOT_REF(po)
    PARAM_NOT_REF(ph)
    PARAM_NOT_REF(pfm)
}

/*----------------------Applet��غ�������---------------------*/
/*==============================================================================
����:
       SportSApp_New

˵��:
        ��ʼ��SportSApp Applet�������ڲ����ݱ�����ͬʱ�����ü�����1��

����:
       ps [in]��IShell�ӿڶ���ָ�롣
       pIModule [in]��pIModule�ӿڶ���ָ�롣
       ppObj [out]:��ָ��ISportsMenu����ָ���ָ�롣

����ֵ:
       EFAILED��ʧ�ܡ�
       SUCCESS���ɹ���

��ע:
       �����û�ȥ���á�
       ����Applet�ṹ�����ʹ�ö�̬�����ڴ�ķ�ʽ�������������õ���ʹ�þ�̬
       �����ķ�ʽ������Խṹ�Ƚϴ��Appletʹ�þ�̬�����ķ�ʽ�����ڶ�̬����
       �ڴ�ķ�ʽ��Ҫע���������ռ���ͷš�

==============================================================================*/
static int SportSApp_New(IShell *ps,IModule *pIModule,ISportsMenu **ppObj)
{
    int retVal = SUCCESS;
    if (0 == gSportsMenu.m_nRefs)
    {
        // Must initialize the object
        MEMSET(&gSportsMenu,  0, sizeof(CSportsMenu));
        INIT_VTBL(&gSportsMenu, ISportsMenu, gSportsMenuMethods);

        gSportsMenu.m_nRefs     = 0;
        gSportsMenu.m_pShell    = ps;
        gSportsMenu.m_pModule   = pIModule;

        (void) ISHELL_AddRef(ps);
        (void) IMODULE_AddRef(pIModule);

        retVal = SportSApp_InitAppData(&gSportsMenu);
    }

    ++gSportsMenu.m_nRefs;
    *ppObj = (ISportsMenu*)&gSportsMenu;
    return retVal;
}

/*==============================================================================
����:
       SportSApp_AddRef

˵��:
       Sports Applet ���ü�����1�������ص�ǰ����������

����:
       pi [in]��SportsApp�ӿڶ���ָ�롣

����ֵ:
       ��ǰ��SportsApp Applet��������

��ע:

==============================================================================*/
static uint32  SportSApp_AddRef(ISportsMenu *pi)
{
    register CSportsMenu *pMe = (CSportsMenu*)pi;

    ASSERT(pMe != NULL);
    ASSERT(pMe->m_nRefs > 0);

    return (++pMe->m_nRefs);
}

/*==============================================================================
����:
       SportSApp_Release

˵��:
       �ͷŶ�SportsApp Applet�����ã����ü�����1�������ص�ǰ����������
        �����ǰ��������Ϊ�㣬�����ͷ�Applet�������Դ��

����:
       pi [in]��ISportsMenu�ӿڶ���ָ�롣

����ֵ:
       ��ǰ��ISportsApp Applet��������

��ע:

==============================================================================*/
static uint32  SportSApp_Release (ISportsMenu *pi)
{
    register CSportsMenu *pMe = (CSportsMenu*)pi;

    ASSERT(pMe != NULL);

    if (pMe->m_nRefs == 0)
    {
        return 0;
    }

    if (--pMe->m_nRefs)
    {
        return pMe->m_nRefs;
    }

    // �ͷ�Applet�����Դ
    SportSApp_FreeAppData(pMe);

    (void) ISHELL_Release(pMe->m_pShell);
    (void) IMODULE_Release(pMe->m_pModule);

    // ע�⣺pMe�Ǿ�̬����ռ䣬��˲���Ҫ�ͷš�FREE()
    return 0;
}

/*==============================================================================
����:
       SportSApp_InitAppData

˵��:
       ��ʼ��SportsMenu Applet�ṹ����ȱʡֵ��

����:
       pMe [in]��ָ��CSportsMenu Applet����ṹ��ָ�롣�ýṹ����С������ض���Ϣ��

����ֵ:
       SUCCESS����ʼ���ɹ���
       EFAILED����ʼ��ʧ�ܡ�

��ע:

==============================================================================*/
static int SportSApp_InitAppData(CSportsMenu *pMe)
{
    	AEEDeviceInfo  di;
	
    	if (NULL  == pMe)
    	{
        	return EFAILED;
    	}
    	ISHELL_GetDeviceInfo(pMe->m_pShell, &di);
    	pMe->m_rc.x       = 0;
    	pMe->m_rc.y       = 0;
    	pMe->m_rc.dx      = (int16) di.cxScreen;
    	pMe->m_rc.dy      = (int16) di.cyScreen;

	pMe->m_bAppIsReady = FALSE;
	pMe->m_ePreState = SPORT_STATE_NONE;
	pMe->m_eCurState = SPORT_STATE_INIT;
	pMe->m_pActiveDlg = NULL;
	pMe->m_eDlgRet = DLGRET_CREATE;
	pMe->m_bNotOverwriteDlgRet = FALSE;
	pMe->m_currDlgId = 0;
	pMe->m_nSubDlgId = 0;
	/* background*/
	pMe->runOnBackgrounds =FALSE;
	pMe->GobackIdleFlag = FALSE;
	/*background end */
	/*Init Pedometer Data About Pedometer and UnitData */
	InitPedoMeter(pMe);
	
    if(AEE_SUCCESS != ISHELL_CreateInstance(pMe->m_pShell,AEECLSID_LISTCTL,
	(void **)&pMe->m_pSportsAppMenu.m_pSAppMenu))
	{
		SportSApp_FreeAppData(pMe);
	    return FALSE;
	}

	if (AEE_SUCCESS != ISHELL_CreateInstance(pMe->m_pShell, AEECLSID_STOPWATCHCTL, 
		(void **) &pMe->m_pSportsAppMenu.m_pSAppTime))
	{
	    SportSApp_FreeAppData(pMe);
	    return FALSE;
	}

	if(AEE_SUCCESS != ISHELL_CreateInstance(pMe->m_pShell, AEECLSID_CONFIG,
		(void **) &pMe->m_pConfig))
	{
		SportSApp_FreeAppData(pMe);
		return FALSE;
	}

	if ( SUCCESS != ISHELL_CreateInstance( pMe->m_pShell,
	                                   AEECLSID_FILEMGR,
	                                   (void **)&pMe->m_pSportsAppMenu.m_pFileMgr))
	{
	    return EFAILED;
	}

       /*small title icon create resource*/
        if (ISHELL_CreateInstance(pMe->m_pShell,
                                     AEECLSID_ANNUNCIATOR,
                                     (void **) &pMe->m_pIAnn))
        {
            return FALSE;
        }    
    return SUCCESS;
}

/*==============================================================================
����:
       SportSApp_FreeAppData

˵��:
       �ͷ�SettingMenu Appletʹ�õ������Դ��

����:
       pMe [in]��ָ��SettingMenu Applet����ṹ��ָ�롣�ýṹ����С������ض���Ϣ��

����ֵ:
       �ޡ�

��ע:

==============================================================================*/
static void SportSApp_FreeAppData(CSportsMenu *pMe)
{
    if (NULL == pMe)
    {
        return;
    }

    if (pMe->m_pSportsAppMenu.m_pSAppMenu != NULL)
	{
       IMENUCTL_Release(pMe->m_pSportsAppMenu.m_pSAppMenu);
       pMe->m_pSportsAppMenu.m_pSAppMenu = NULL;
    }

    if (pMe->m_pSportsAppMenu.m_pSAppTime != NULL) 
	{
       ITIMECTL_Release(pMe->m_pSportsAppMenu.m_pSAppTime);
       pMe->m_pSportsAppMenu.m_pSAppTime = NULL;
    }

    if (pMe->m_pConfig!= NULL) 
    	{
    		ICONFIG_Release(pMe->m_pConfig);
		pMe->m_pConfig = NULL;
    	}

	/*Release FileMgr handle*/
    if (pMe->m_pSportsAppMenu.m_pFileMgr != NULL)
    {
        (void) IFILEMGR_Release(pMe->m_pSportsAppMenu.m_pFileMgr);
        pMe->m_pSportsAppMenu.m_pFileMgr = NULL;
    }
    /*small title icon freee resource*/
    if (pMe->m_pIAnn)
    {
        IANNUNCIATOR_Release(pMe->m_pIAnn);
        pMe->m_pIAnn= NULL;
    }
	ISHELL_CancelTimer(pMe->m_pShell,NULL,pMe);
}

/*==============================================================================
����:
       SportSApp_RunFSM

˵��:
       SportsMenu Applet����״̬�����档

����:
       pMe [in]��ָ��SettingMenu Applet����ṹ��ָ�롣�ýṹ����С������ض���Ϣ��

����ֵ:
       �ޡ�

��ע:

==============================================================================*/
static void SportSApp_RunFSM(CSportsMenu *pMe)
{
    NextFSMAction nextFSMAction = NFSMACTION_WAIT;

    for ( ; ; )
    {
        nextFSMAction = SportSApp_ProcessState(pMe);

        if (pMe->m_bNotOverwriteDlgRet)
        {
            pMe->m_bNotOverwriteDlgRet = FALSE;
        }
        else
        {
            pMe->m_eDlgRet = DLGRET_CREATE;
        }

        if (nextFSMAction == NFSMACTION_WAIT)
        {
            break;
        }
    }
}

/*==============================================================================
����:
       SportSApp_HandleEvent

˵��:
       SportsApp  Applet�¼��������������������ڱ�Applet���¼���
       �������ɸú�������

����:
       pi [in]��ָ��ISportMenu�ӿ�t�����ָ�롣
       eCode��BREW�¼����롣
       wParam�����¼�����������ݡ�
       dwParam [in]�����¼�����������ݡ�

����ֵ:
       TRUE�������¼�������
       FALSE�������¼�û������

��ע:

==============================================================================*/
extern boolean g_sportsapp_pedometer_bground_flag;
#ifdef FEATURE_SPORTS_APP
extern boolean g_gobacktosportflag;
#endif

static boolean SportSApp_HandleEvent(ISportsMenu *pi,
    AEEEvent eCode,
    uint16  wParam,
    uint32 dwParam
)
{
    CSportsMenu *pMe = (CSportsMenu*)pi;
    AEEAppStart *as;
	
    SPORTS_ERR("%x, %x ,%x,SettingMenu_HandleEvent",eCode,wParam,dwParam);
	
    switch (eCode)
    {
        case EVT_APP_START:
			Rendering_UpdateEx();//wlh add for 3D test
            /*event application start*/
            as = (AEEAppStart*)dwParam;

            #ifdef FEATURE_SPORTS_APP
            if(as->pszArgs != NULL)
            {
                uint16 bytNewData;                
                DBGPRINTF("as->pszArgs != NULL ");
                
                (void)ICONFIG_GetItem(pMe->m_pConfig,
                CFGI_SPORT_FLAG,
                &bytNewData, sizeof(bytNewData));

                if(bytNewData >= 3)
                {
                    pMe->m_pSportsAppMenu.m_nSelProFile = 0;
                }
                else
                {
                    pMe->m_pSportsAppMenu.m_nSelProFile = bytNewData;
                }
                /*Get Sport Data From File System*/
                SportSApp_LoadSportData(pMe);
                
                pMe->FromScheduleFlag = TRUE;
                pMe->m_eCurState = SPORT_STATE_PETOMETER;
                pMe->m_eDlgRet = DLGRET_CREATE;
                //return TRUE;
            }
            #endif
            
            pMe->m_bAppIsReady = FALSE;
            ASSERT(dwParam != 0);
            as = (AEEAppStart*)dwParam;
            if (NULL != pMe->m_pDisplay)
            {
                (void) IDISPLAY_Release(pMe->m_pDisplay);
                pMe->m_pDisplay = NULL;
            }
            pMe->m_pDisplay = as->pDisplay;
            (void) IDISPLAY_AddRef(pMe->m_pDisplay);
            pMe->m_rc = as->rc;
            pMe->m_rc.y = pMe->m_rc.y - SPORTS_MENU_SOFK_HIGHT;
            pMe->m_bSuspending = FALSE;

            // ��ʼSportSApp״̬��
            SportSApp_RunFSM(pMe);
            return TRUE;

        case EVT_APP_STOP:
		if(!pMe->runOnBackgrounds)
		{
            if (pMe->m_pDisplay != NULL)
            {
                (void) IDISPLAY_Release(pMe->m_pDisplay);
                pMe->m_pDisplay = NULL;
            }
		}
            pMe->m_bSuspending = TRUE;
		*((boolean*)dwParam) = !pMe->runOnBackgrounds;
            return TRUE;

        case EVT_APP_SUSPEND:
            pMe->m_bSuspending = TRUE;

            return TRUE;

        case EVT_APP_RESUME:
            ASSERT(dwParam != 0);
            as = (AEEAppStart*)dwParam;

#ifdef FEATURE_SPORTS_APP

            if(g_gobacktosportflag == TRUE)
            {
                uint16 bytNewData;
                InitPedoMeter(pMe);
                
                (void)ICONFIG_GetItem(pMe->m_pConfig,
                CFGI_SPORT_FLAG,
                &bytNewData, sizeof(bytNewData));

                if(bytNewData >= 3)
                {
                    pMe->m_pSportsAppMenu.m_nSelProFile = 0;
                }
                else
                {
                    pMe->m_pSportsAppMenu.m_nSelProFile = bytNewData;
                }
                /*Get Sport Data From File System*/
                SportSApp_LoadSportData(pMe);		
                
                pMe->FromScheduleFlag = TRUE;
                pMe->m_eCurState = SPORT_STATE_PETOMETER;
                pMe->m_eDlgRet = DLGRET_CREATE;
                g_gobacktosportflag = FALSE;
                //CLOSE_DIALOG(DLGRET_PEDOMETER);
                
                }
            #endif
            pMe->m_bSuspending = FALSE;

            if (pMe->m_pDisplay != NULL)
            {
                (void) IDISPLAY_Release(pMe->m_pDisplay);
                pMe->m_pDisplay = NULL;
            }
            pMe->m_pDisplay = as->pDisplay;
            (void) IDISPLAY_AddRef(pMe->m_pDisplay);
            pMe->m_rc = as->rc;
            pMe->m_rc.y = pMe->m_rc.y - SPORTS_MENU_SOFK_HIGHT;
            SportSApp_RunFSM(pMe);
            return TRUE;

        case EVT_DIALOG_INIT:
			if(OEM_IME_DIALOG == wParam)
			{
				return TRUE;
			}
            pMe->m_bAppIsReady = FALSE;
            pMe->m_pActiveDlg = (IDialog*)dwParam;
            pMe->m_pActiveDlgID = wParam;

            return SportSApp_RouteDialogEvent(pMe,eCode,wParam,dwParam);

        case EVT_DIALOG_START:
			if(OEM_IME_DIALOG == wParam)
			{
				return TRUE;
			}
			Rendering_UpdateEx();//wlh add for 3D test
            return SportSApp_RouteDialogEvent(pMe,eCode,wParam,dwParam);
        case EVT_CLOSEAPP:
        {
            #ifdef FEATRUE_SUPPORT_G_SENSOR
            mmi_g_sensor_process(G_SENSOR_PEDOMETER_DISABLE);
            #endif
            Pedometer_Reset(pMe);
            pMe->m_pSportsAppMenu.m_nEntryRecordState = REC_LAST;
            pMe->runOnBackgrounds = FALSE;
            g_sportsapp_pedometer_bground_flag =  FALSE;  
            IANNUNCIATOR_SetField(pMe->m_pIAnn,ANNUN_FIELD_SPORTS, ANNUN_STATE_SPORTS_OFF);
            /*Switch Fm or Music stop*/
            //SportSapp_SwitchFmAndMusic(pMe); 
            /*entry From Schedule False flag*/
            pMe->FromScheduleFlag = FALSE;    

            ISHELL_CloseApplet( pMe->m_pShell, FALSE);
            return TRUE;
        }
        case EVT_USER_REDRAW:
            (void) SportSApp_RouteDialogEvent(pMe,eCode,wParam,dwParam);
            (void)ISHELL_SetTimer ( pMe->m_pShell,
                            APPISREADY_TIMER,
                            SportSApp_APPIsReadyTimer,
                            pMe);
            return TRUE;

        case EVT_APPISREADY:
            pMe->m_bAppIsReady = TRUE;
            return TRUE;
#ifdef FEATURE_LCD_TOUCH_ENABLE//wlh add for LCD touch
		case EVT_PEN_UP:
		//case EVT_PEN_DOWN:
			{
				return SportSApp_RouteDialogEvent(pMe,eCode,wParam,dwParam);
			}
#endif
        case EVT_KEY_PRESS:
        case EVT_KEY_RELEASE:
        case EVT_KEY:
        case EVT_COMMAND:
            if (!pMe->m_bAppIsReady)
            {
                return TRUE;
            }
            return SportSApp_RouteDialogEvent(pMe,eCode,wParam,dwParam);

        case EVT_DIALOG_END:
			if(OEM_IME_DIALOG == wParam)
			{
				return ISHELL_PostEvent(pMe->m_pShell,AEECLSID_SPORTSAPP,EVT_USER_REDRAW,0,0);
			}
            if (wParam == 0)
            {
                return TRUE;
            }
            pMe->m_bAppIsReady = FALSE;
            (void) SportSApp_RouteDialogEvent(pMe,eCode,wParam,dwParam);
            pMe->m_pActiveDlg = NULL;

            if (pMe->m_bSuspending == FALSE)
            {
                SportSApp_RunFSM(pMe);
            }
            return TRUE;
#ifdef FEATURE_LCD_TOUCH_ENABLE//wlh add for LCD touch
		case EVT_USER:
			{
				if (wParam == AVK_SELECT)
				{
					if (dwParam>0)
					{
						eCode = EVT_COMMAND;
						wParam = dwParam;
					}
					else
					{
						eCode = EVT_KEY;
					}
					
				}
				else if (wParam == AVK_INFO)
				{
					eCode = EVT_KEY;
				}
				else if (wParam ==  AVK_CLR)
				{
					if (dwParam == NEVT_KEY_PRESSS)
					{
						eCode = EVT_KEY_PRESS;
					}
					else
					{
						eCode = EVT_KEY;
					}
					
				}
				else if ((wParam == AVK_LEFT)||(wParam == AVK_RIGHT))
				{
					eCode = EVT_KEY_PRESS;
				}
				return  SportSApp_RouteDialogEvent(pMe,eCode,wParam,dwParam);

			}
			return TRUE;
#endif

        default:
            return SportSApp_RouteDialogEvent(pMe,eCode,wParam,dwParam);
    }
}

/*==============================================================================
����:
       SportSApp_APPIsReadyTimer

˵��:
       ��ʱ���ص���������Ҫ���ڿ��ƿ��ٰ�������Ĵ���

����:
       pme [in]��void *����ָ�룬��ָSportMenu�ṹָ�롣

����ֵ:
       �ޡ�

��ע:

==============================================================================*/
static void SportSApp_APPIsReadyTimer(void *pme)
{
    CSportsMenu *pMe = (CSportsMenu *)pme;

    if (NULL == pMe)
    {
        return;
    }

    (void) ISHELL_PostEvent( pMe->m_pShell,AEECLSID_SPORTSAPP,EVT_APPISREADY,0,0);
}

#endif




