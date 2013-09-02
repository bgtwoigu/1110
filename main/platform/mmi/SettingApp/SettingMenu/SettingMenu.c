/*==============================================================================
// �ļ���
//        SettingMenu.c
//        ��Ȩ����(c) 2009 Anylook Tech. CO.,LTD.
//
// �ļ�˵����
//
// ���ߣ�Gemsea
// �������ڣ�2007-11-01
// ��ǰ�汾��Draft
==============================================================================*/

/*==============================================================================
�޸���ʷ��¼��
ʱ       ��      �� �� ��    ���ⵥ��  �޸����ݡ�λ�ü�ԭ��
-----------      ----------  --------  -----------------------------------------
==============================================================================*/


/*==============================================================================
                                 ���ļ��������ⲿ�ļ�
==============================================================================*/
#include "SettingMenu_priv.h"

/*==============================================================================
                                 �궨��ͳ���
==============================================================================*/

/*==============================================================================
                                 ���Ͷ���
==============================================================================*/

/*==============================================================================
                                 ��������
==============================================================================*/
static void SettingMenu_APPIsReadyTimer(void *pme);

/*----------------------ģ����غ�������---------------------*/

/*static*/ int  SettingMenuMod_Load(IShell   *pIShell,
    void     *ph,
    IModule **ppMod
);

int  SettingMenuMod_New(int16  nSize,
    IShell   *pIShell,
    void     *ph,
    IModule  **ppMod,
    PFNMODCREATEINST pfnMC,
    PFNFREEMODDATA pfnMF
);

static int  SettingMenuMod_CreateInstance(IModule *po,
    IShell *pIShell,
    AEECLSID ClsId,
    void **ppObj
);

static uint32 SettingMenuMod_AddRef(IModule *po);

static uint32 SettingMenuMod_Release(IModule *po);

static void SettingMenuMod_FreeResources(IModule  *po,
    IHeap  *ph,
    IFileMgr *pfm
);

/*----------------------Applet��غ�������---------------------*/
static int SettingMenu_New(IShell *ps,
    IModule *pIModule,
    ISettingMenu **ppObj
);

static uint32  SettingMenu_AddRef(ISettingMenu *pi);

static uint32  SettingMenu_Release (ISettingMenu *pi);

static boolean SettingMenu_HandleEvent(ISettingMenu *pi,
    AEEEvent eCode,
    uint16  wParam,
    uint32 dwParam
);

static int SettingMenu_InitAppData(CSettingMenu *pMe);

static void SettingMenu_FreeAppData(CSettingMenu *pMe);

static void SettingMenu_RunFSM(CSettingMenu *pMe);
#ifdef FEATRUE_SET_IP_NUMBER
static void SettingMenu_Get_Ip_Number(CSettingMenu *pMe);
static void SettingMenu_Free_And_Save_Ip_Number(CSettingMenu *pMe);
#endif /*FEATRUE_SET_IP_NUMBER*/

/*==============================================================================
                                 ȫ������
==============================================================================*/
// ��Ҫ��ʾ��
//        g_dwAEEStdLibEntryȫ�ֱ���������SDK ��
//        BREW SDK�û����ܸ�д��ֱ��ʹ�ñ�������������AEEStdLib��һ����ڵ㣬��
//        Ҫ�ƶ��������д��롣
#ifdef AEE_SIMULATOR
   uint32 g_dwAEEStdLibEntry;
#ifndef WIN32
   #error This applet was never intended to be used with the AEE Simulator.
#endif
#endif

/*==============================================================================
                                 ���أ���̬������
==============================================================================*/
static SettingMenuMod gSettingMenuMod;

static const VTBL(IModule) gModFuncs =
{
    SettingMenuMod_AddRef,
    SettingMenuMod_Release,
    SettingMenuMod_CreateInstance,
    SettingMenuMod_FreeResources
};

// ֻ����һ��SettingMenuʵ����ÿ�δ���SettingMenu Appletʱ��
// ����ͬһ�ṹָ���BREW�㡣
static CSettingMenu gSettingMenu;

static const VTBL(ISettingMenu) gSettingMenuMethods =
{
    SettingMenu_AddRef,
    SettingMenu_Release,
    SettingMenu_HandleEvent
};

/*----------------------ģ����غ�������---------------------*/

/*==============================================================================
����:
       SettingMenuMod_Load

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
#ifdef AEE_SIMULATOR
__declspec(dllexport) int AEEMod_Load(IShell   *pIShell,
                                                        void     *ph,
                                                        IModule **ppMod)
#else
/*static*/ int SettingMenuMod_Load(IShell   *pIShell,
                                                   void     *ph,
                                                   IModule **ppMod)
#endif
#else
/*static*/ int SettingMenuMod_Load(IShell   *pIShell,
                                                   void     *ph,
                                                   IModule **ppMod)
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

    return SettingMenuMod_New(sizeof(SettingMenuMod),
                    pIShell,
                    ph,
                    ppMod,
                    NULL,
                    NULL);
}

/*==============================================================================
����:
       SettingMenuMod_New

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
int  SettingMenuMod_New(int16  nSize,
    IShell   *pIShell,
    void     *ph,
    IModule  **ppMod,
    PFNMODCREATEINST pfnMC,
    PFNFREEMODDATA pfnMF
)
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
#ifndef WIN32
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
#else
	if (!pIShell)
    {
        return EFAILED;
    }
#endif//WIN32

    MEMSET(&gSettingMenuMod,0,sizeof(SettingMenuMod));

    INIT_VTBL(&gSettingMenuMod,IModule,gModFuncs);	
	
    gSettingMenuMod.m_nRefs = 1;
    *ppMod = (IModule *)&gSettingMenuMod;
    return AEE_SUCCESS;
}

/*==============================================================================
����:
       SettingMenuMod_AddRef

˵��:
       SettingMenu ģ�����ü�����1�������ص�ǰ����������

����:
       po [in]��IModule�ӿڶ���ָ�롣

����ֵ:
       ��ǰ��SettingMenu ģ���������

��ע:

==============================================================================*/
static uint32 SettingMenuMod_AddRef(IModule *po)
{	
    return(++((SettingMenuMod *)po)->m_nRefs);
}

/*==============================================================================
����:
       SettingMenuMod_Release

˵��:
       SettingMenu ģ�����ü�����1�������ص�ǰ����������

����:
       po [in]��IModule�ӿڶ���ָ�롣

����ֵ:
       ��ǰ��SettingMenu ģ���������

��ע:

==============================================================================*/
static uint32 SettingMenuMod_Release(IModule *po)
{
    if (((SettingMenuMod *) po)->m_nRefs == 0)
    {
        return 0;
    }

    return(--((SettingMenuMod *)po)->m_nRefs);
}

/*==============================================================================
����:
       SettingMenuMod_CreateInstance

˵��:
        ͨ������(��ʼ��)SettingMenu����������ģ��ʵ����

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
static int  SettingMenuMod_CreateInstance(IModule *po,
    IShell *pIShell,
    AEECLSID ClsId,
    void **ppObj
)
{
    *ppObj = NULL;

    if (ClsId != AEECLSID_APP_SETTINGMENU)
    {
        return EFAILED;
    }

    if (SettingMenu_New(pIShell, po, (ISettingMenu**)ppObj) != SUCCESS)
    {
        return EFAILED;
    }
    return SUCCESS;
}

/*==============================================================================
����:
       SettingMenuMod_FreeResources

˵��:
        �ͷ�SettingMenuģ����Դ�ĺ�����

����:
       po ��IModule��
       ph ��IHeap�ӿ�ָ�롣
       pfm :IFileMgr�ӿ�ָ�롣

����ֵ:
       ��

��ע:
       �����û�ȥ���á�

==============================================================================*/
static void SettingMenuMod_FreeResources(IModule  *po,
    IHeap  *ph,
    IFileMgr *pfm
)
{
    PARAM_NOT_REF(po)
    PARAM_NOT_REF(ph)
    PARAM_NOT_REF(pfm)
}

/*----------------------Applet��غ�������---------------------*/
/*==============================================================================
����:
       SettingMenu_New

˵��:
        ��ʼ��SettingMenu Applet�������ڲ����ݱ�����ͬʱ�����ü�����1��

����:
       ps [in]��IShell�ӿڶ���ָ�롣
       pIModule [in]��pIModule�ӿڶ���ָ�롣
       ppObj [out]:��ָ��ISettingMenu����ָ���ָ�롣

����ֵ:
       EFAILED��ʧ�ܡ�
       SUCCESS���ɹ���

��ע:
       �����û�ȥ���á�
       ����Applet�ṹ�����ʹ�ö�̬�����ڴ�ķ�ʽ�������������õ���ʹ�þ�̬
       �����ķ�ʽ������Խṹ�Ƚϴ��Appletʹ�þ�̬�����ķ�ʽ�����ڶ�̬����
       �ڴ�ķ�ʽ��Ҫע���������ռ���ͷš�

==============================================================================*/
static int SettingMenu_New(IShell *ps,
    IModule *pIModule,
    ISettingMenu **ppObj
)
{
    int retVal = SUCCESS;
    if (0 == gSettingMenu.m_nRefs)
    {
        // Must initialize the object
        MEMSET(&gSettingMenu,  0, sizeof(CSettingMenu));
        INIT_VTBL(&gSettingMenu, ISettingMenu, gSettingMenuMethods);

        gSettingMenu.m_nRefs     = 0;
        gSettingMenu.m_pShell    = ps;
        gSettingMenu.m_pModule   = pIModule;

        (void) ISHELL_AddRef(ps);
        (void) IMODULE_AddRef(pIModule);

        retVal = SettingMenu_InitAppData(&gSettingMenu);
    }

    ++gSettingMenu.m_nRefs;
    *ppObj = (ISettingMenu*)&gSettingMenu;
    return retVal;
}

/*==============================================================================
����:
       SettingMenu_AddRef

˵��:
       SettingMenu Applet ���ü�����1�������ص�ǰ����������

����:
       pi [in]��ISettingMenu�ӿڶ���ָ�롣

����ֵ:
       ��ǰ��SettingMenu Applet��������

��ע:

==============================================================================*/
static uint32  SettingMenu_AddRef(ISettingMenu *pi)
{
    register CSettingMenu *pMe = (CSettingMenu*)pi;	

    ASSERT(pMe != NULL);
    ASSERT(pMe->m_nRefs > 0);

    return (++pMe->m_nRefs);
}

/*==============================================================================
����:
       SettingMenu_Release

˵��:
       �ͷŶ�SettingMenu Applet�����ã����ü�����1�������ص�ǰ����������
        �����ǰ��������Ϊ�㣬�����ͷ�Applet�������Դ��

����:
       pi [in]��ISettingMenu�ӿڶ���ָ�롣

����ֵ:
       ��ǰ��SettingMenu Applet��������

��ע:

==============================================================================*/
static uint32  SettingMenu_Release (ISettingMenu *pi)
{
    register CSettingMenu *pMe = (CSettingMenu*)pi;

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
    SettingMenu_FreeAppData(pMe);

    (void) ISHELL_Release(pMe->m_pShell);
    (void) IMODULE_Release(pMe->m_pModule);

    // ע�⣺pMe�Ǿ�̬����ռ䣬��˲���Ҫ�ͷš�FREE()
    return 0;
}

/*==============================================================================
����:
       SettingMenu_InitAppData

˵��:
       ��ʼ��SettingMenu Applet�ṹ����ȱʡֵ��

����:
       pMe [in]��ָ��SettingMenu Applet����ṹ��ָ�롣�ýṹ����С������ض���Ϣ��

����ֵ:
       SUCCESS����ʼ���ɹ���
       EFAILED����ʼ��ʧ�ܡ�

��ע:

==============================================================================*/
static int SettingMenu_InitAppData(CSettingMenu *pMe)
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
    pMe->m_ePreState = SETTINGMENUST_NONE;
    pMe->m_eCurState = SETTINGMENUST_INIT;
    pMe->m_pActiveDlg = NULL;
    pMe->m_eDlgRet = DLGRET_CREATE;
    pMe->m_bNotOverwriteDlgRet = FALSE;

    pMe->m_currDlgId = 0;
    pMe->m_nSubDlgId = 0;
    pMe->m_input_mode = 0;
    pMe->m_ip_sel_num = 0;
    pMe->m_msg_id = 0;
    pMe->m_shaketype_sel = SET_SHAKE_NONE;
    pMe->m_strPhonePWD = NULL;

    if (AEE_SUCCESS != ISHELL_CreateInstance(pMe->m_pShell,
                                            AEECLSID_CONFIG,
                                            (void **)&pMe->m_pConfig))
    {
        SettingMenu_FreeAppData(pMe);
        return EFAILED;
    }
    if (AEE_SUCCESS != ISHELL_CreateInstance(pMe->m_pShell,
                                            AEECLSID_ANNUNCIATOR,
                                            (void **)&pMe->m_pAnn))
    {
        SettingMenu_FreeAppData(pMe);
        return EFAILED;
    }
	MSG_FATAL("IANNUNCIATOR_SetFieldIsActiveEx::::settingmunu111:::",0,0,0);
	//IANNUNCIATOR_SetFieldIsActiveEx(pMe->m_pAnn,FALSE);
#ifndef WIN32    
    // ���� IRUIM �ӿ�
    if (AEE_SUCCESS != ISHELL_CreateInstance(pMe->m_pShell,
                                 AEECLSID_RUIM,
                                 (void **) &pMe->m_pIRUIM))
    {
        SettingMenu_FreeAppData(pMe);
        return EFAILED;
    }
#endif//WIN32
	
    return SUCCESS;
}

/*==============================================================================
����:
       SettingMenu_FreeAppData

˵��:
       �ͷ�SettingMenu Appletʹ�õ������Դ��

����:
       pMe [in]��ָ��SettingMenu Applet����ṹ��ָ�롣�ýṹ����С������ض���Ϣ��

����ֵ:
       �ޡ�

��ע:

==============================================================================*/
static void SettingMenu_FreeAppData(CSettingMenu *pMe)
{
    if (NULL == pMe)
    {
        return ;
    }

    FREEIF(pMe->m_strPhonePWD);
#ifdef FEATRUE_SET_IP_NUMBER
    {
        SettingMenu_Free_And_Save_Ip_Number(pMe);
    }
#endif/*FEATRUE_SET_IP_NUMBER*/

    if (pMe->m_pConfig)
    {
        ICONFIG_Release(pMe->m_pConfig);
        pMe->m_pConfig = NULL;
    }
    if (pMe->m_pAnn != NULL)
    {
       IANNUNCIATOR_Release(pMe->m_pAnn);
        pMe->m_pAnn = NULL;
    }
    // �ͷ� IRUIM �ӿ�
    if (pMe->m_pIRUIM != NULL)
    {
        IRUIM_Release(pMe->m_pIRUIM);
        pMe->m_pIRUIM = NULL;
    }
	
}

/*==============================================================================
����:
       SettingMenu_RunFSM

˵��:
       SettingMenu Applet����״̬�����档

����:
       pMe [in]��ָ��SettingMenu Applet����ṹ��ָ�롣�ýṹ����С������ض���Ϣ��

����ֵ:
       �ޡ�

��ע:

==============================================================================*/
static void SettingMenu_RunFSM(CSettingMenu *pMe)
{
    NextFSMAction nextFSMAction = NFSMACTION_WAIT;

    for ( ; ; )
    {
        nextFSMAction = SettingMenu_ProcessState(pMe);

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
       SettingMenu_HandleEvent

˵��:
       SettingMenu  Applet�¼��������������������ڱ�Applet���¼���
       �������ɸú�������

����:
       pi [in]��ָ��ISettingMenu�ӿ�t�����ָ�롣
       eCode��BREW�¼����롣
       wParam�����¼�����������ݡ�
       dwParam [in]�����¼�����������ݡ�

����ֵ:
       TRUE�������¼�������
       FALSE�������¼�û������

��ע:

==============================================================================*/
static boolean SettingMenu_HandleEvent(ISettingMenu *pi,
    AEEEvent eCode,
    uint16  wParam,
    uint32 dwParam
)
{
    CSettingMenu *pMe = (CSettingMenu*)pi;
    AEEAppStart *as;
    AEEDeviceInfo di; 

    ISHELL_GetDeviceInfo(pMe->m_pShell,&di);    
    //SETTING_ERR("%x, %x ,%x,SettingMenu_HandleEvent",eCode,wParam,dwParam);
	
    switch (eCode)
    {
        case EVT_APP_START:			
            pMe->m_bAppIsReady = FALSE;
            ASSERT(dwParam != 0);
            as = (AEEAppStart*)dwParam;
            if (NULL != pMe->m_pDisplay)
            {
                (void) IDISPLAY_Release(pMe->m_pDisplay);
                pMe->m_pDisplay = NULL;
            }
#ifdef FEATRUE_SET_IP_NUMBER            
            SettingMenu_Get_Ip_Number(pMe);
#endif
            pMe->m_pDisplay = as->pDisplay;
            (void) IDISPLAY_AddRef(pMe->m_pDisplay);
            pMe->m_rc = as->rc;
            pMe->m_rc.dy = di.cyScreen;
            pMe->m_rc.y = pMe->m_rc.y - SETTING_MENU_SOFK_HIGHT;
            
            pMe->m_bSuspending = FALSE;
            if(pMe->m_pAnn != NULL)
            {
                IANNUNCIATOR_SetFieldIsActiveEx(pMe->m_pAnn,FALSE);
            }
            // ��ʼSettingMenu״̬��
            SettingMenu_RunFSM(pMe);
            return TRUE;

        case EVT_APP_STOP:
            if (pMe->m_pDisplay != NULL)
            {
                (void) IDISPLAY_Release(pMe->m_pDisplay);
                pMe->m_pDisplay = NULL;
            }
            pMe->m_bSuspending = TRUE;

            return TRUE;

        case EVT_APP_SUSPEND:
            pMe->m_bSuspending = TRUE;

            return TRUE;

        case EVT_APP_RESUME:
            ASSERT(dwParam != 0);
            as = (AEEAppStart*)dwParam;
            pMe->m_bSuspending = FALSE;

            if (pMe->m_pDisplay != NULL)
            {
                (void) IDISPLAY_Release(pMe->m_pDisplay);
                pMe->m_pDisplay = NULL;
            }
            pMe->m_pDisplay = as->pDisplay;
            (void) IDISPLAY_AddRef(pMe->m_pDisplay);
            pMe->m_rc = as->rc;
            pMe->m_rc.y = pMe->m_rc.y - SETTING_MENU_SOFK_HIGHT;
            pMe->m_rc.dy = di.cyScreen;
            ERR("pMe->m_rc.y:%d,pMe->m_rc.dy:%d",pMe->m_rc.y,pMe->m_rc.dy,0);
            SettingMenu_RunFSM(pMe);
            return TRUE;

        case EVT_DIALOG_INIT:			
			if(OEM_IME_DIALOG == wParam)
			{
				return TRUE;
			}
            pMe->m_bAppIsReady = FALSE;
            pMe->m_pActiveDlg = (IDialog*)dwParam;
            pMe->m_pActiveDlgID = wParam;

            return SettingMenu_RouteDialogEvent(pMe,eCode,wParam,dwParam);

        case EVT_DIALOG_START:
            if(OEM_IME_DIALOG == wParam)
			{
				return TRUE;
			}
            return SettingMenu_RouteDialogEvent(pMe,eCode,wParam,dwParam);

        case EVT_USER_REDRAW:
            (void) SettingMenu_RouteDialogEvent(pMe,eCode,wParam,dwParam);
            (void)ISHELL_SetTimer ( pMe->m_pShell,
                            APPISREADY_TIMER,
                            SettingMenu_APPIsReadyTimer,
                            pMe);
            return TRUE;

        case EVT_APPISREADY:
            pMe->m_bAppIsReady = TRUE;
            return TRUE;

        case EVT_KEY_PRESS:
        case EVT_KEY_RELEASE:
        case EVT_KEY:
        case EVT_COMMAND:
			#if !defined(FEATURE_VERSION_K212)
            if (!pMe->m_bAppIsReady)
            {
                return TRUE;
            }
			#endif
            return SettingMenu_RouteDialogEvent(pMe,eCode,wParam,dwParam);

        case EVT_DIALOG_END:
            if (wParam == 0)
            {
                return TRUE;
            }
            if(OEM_IME_DIALOG == wParam)
			{
				return ISHELL_PostEvent(pMe->m_pShell,AEECLSID_APP_SETTINGMENU,EVT_USER_REDRAW,0,0);
			}
            pMe->m_bAppIsReady = FALSE;
            (void) SettingMenu_RouteDialogEvent(pMe,eCode,wParam,dwParam);
            pMe->m_pActiveDlg = NULL;

            // Applet�����𣬲���״̬�����ȴ�Applet���ء���ע�⣺EVT_APP_SUSPEND
            // �¼���EVT_DIALOG_END�¼�ǰ������
            if (pMe->m_bSuspending == FALSE)
            {
                // ��ʼSettingMenu״̬��
                SettingMenu_RunFSM(pMe);
            }
            return TRUE;
#ifdef FEATURE_LCD_TOUCH_ENABLE//wlh add for LCD touch
		case EVT_PEN_UP:
		//case EVT_PEN_DOWN:
			{
				return SettingMenu_RouteDialogEvent(pMe,eCode,wParam,dwParam);
			}

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
				else if((wParam == AVK_CLR) || (wParam == AVK_LEFT) ||(wParam == AVK_RIGHT))
				{
					eCode = EVT_KEY;
				}
				return SettingMenu_RouteDialogEvent(pMe,eCode,wParam,dwParam);
			}
#endif//FEATURE_LCD_TOUCH_ENABLE
        default:
            // �����յ����¼�·������ǰ��ĶԻ����¼���������
            return SettingMenu_RouteDialogEvent(pMe,eCode,wParam,dwParam);
    }
}

/*==============================================================================
����:
       SettingMenu_APPIsReadyTimer

˵��:
       ��ʱ���ص���������Ҫ���ڿ��ƿ��ٰ�������Ĵ���

����:
       pme [in]��void *����ָ�룬��ָCSettingMenu�ṹָ�롣

����ֵ:
       �ޡ�

��ע:

==============================================================================*/
static void SettingMenu_APPIsReadyTimer(void *pme)
{
    CSettingMenu *pMe = (CSettingMenu *)pme;

    if (NULL == pMe)
    {
        return;
    }

    (void) ISHELL_PostEvent( pMe->m_pShell,
                            AEECLSID_APP_SETTINGMENU,
                            EVT_APPISREADY,
                            0,
                            0);
}
#ifdef FEATRUE_SET_IP_NUMBER
static void SettingMenu_Get_Ip_Number(CSettingMenu *pMe)
{
    IP_Number_Cfg  ip_cfg;
    MEMSET((void*)&ip_cfg,0 , sizeof(IP_Number_Cfg));

    ICONFIG_GetItem(pMe->m_pConfig,CFGI_IP_NUMBER,(void *)&ip_cfg,sizeof(IP_Number_Cfg));
    pMe->ip_number_tab.count = 0;
    pMe->ip_number_tab.head = NULL;
    pMe->ip_number_tab.tial = NULL;
    pMe->ip_number_tab.ip_state = ip_cfg.bStateOn;
    MEMSET(pMe->ip_number_tab.default_ip, 0 ,sizeof(pMe->ip_number_tab.default_ip));
    if(ip_cfg.ip_count>0)
    {
        char       c_buf[OEM_IP_NUMBER_MAX +1];
        char * point = NULL;
        char * head = NULL;
        point = STRCHR(ip_cfg.Number, ':');
        head = ip_cfg.Number;
        while(point)
        {
            number_table *num_tab = NULL;
            MEMSET((void *)c_buf, 0 ,sizeof(c_buf));
            STRNCPY(c_buf,head, point - head);
            head = point +1;
            point = STRCHR(head, ':');

            num_tab = (number_table *)MALLOC(sizeof(number_table));
            if(num_tab)
            {
                MEMSET(num_tab,0 ,sizeof(number_table));
                num_tab->next = NULL;
                STRTOWSTR(c_buf, num_tab->number, (OEM_IP_NUMBER_MAX+1)*sizeof(AECHAR));
                pMe->ip_number_tab.count ++;
            }
            if(pMe->ip_number_tab.head == NULL) //the first node
            {
                pMe->ip_number_tab.head = num_tab;
            }
            if(pMe->ip_number_tab.tial)
            {
                pMe->ip_number_tab.tial->next = num_tab;
            }
            pMe->ip_number_tab.tial = num_tab;
        }
        if(ip_cfg.default_number[0] != 0)
        {
            STRTOWSTR(ip_cfg.default_number,pMe->ip_number_tab.default_ip,sizeof(pMe->ip_number_tab.default_ip));
        }
        SettingMenu_Print_IP_Number(pMe);
    }
}

static void SettingMenu_Free_And_Save_Ip_Number(CSettingMenu *pMe)
{
    IP_Number_Cfg  ip_cfg;
    char       c_buf[OEM_IP_NUMBER_MAX +2];

    //if(pMe->ip_number_tab.head == NULL)
    //{
    //    return;
    //}

    MEMSET((void*)&ip_cfg,0 , sizeof(IP_Number_Cfg));
    if(pMe->ip_number_tab.head)
    {
        if(pMe->ip_number_tab.default_ip[0] != 0)
        {
            WSTRTOSTR(pMe->ip_number_tab.default_ip ,ip_cfg.default_number,sizeof(ip_cfg.default_number));
        }
        else
        {
            WSTRTOSTR(pMe->ip_number_tab.head->number,ip_cfg.default_number,sizeof(ip_cfg.default_number));
        }
    }
    SettingMenu_Print_IP_Number(pMe);
    while(pMe->ip_number_tab.head)
    {
        number_table *temp = NULL;
        temp = pMe->ip_number_tab.head;
        MEMSET((void *)c_buf, 0 ,sizeof(c_buf));
        WSTRTOSTR(temp->number,c_buf,OEM_IP_NUMBER_MAX+2);

        STRLCAT(c_buf,":",OEM_IP_NUMBER_MAX+2);
        STRLCAT(ip_cfg.Number,c_buf,OEM_IP_NUMBER_COUNT * (OEM_IP_NUMBER_MAX+1));
        ip_cfg.ip_count ++;
        pMe->ip_number_tab.head = temp->next;
        FREEIF(temp);
    }
    pMe->ip_number_tab.head = NULL;
    pMe->ip_number_tab.tial =NULL;
    pMe->ip_number_tab.count = 0;
    ip_cfg.bStateOn = (pMe->ip_number_tab.ip_state == 0 ? FALSE:TRUE);
    ICONFIG_SetItem(pMe->m_pConfig,CFGI_IP_NUMBER,(void *)&ip_cfg,sizeof(IP_Number_Cfg));
}

void SettingMenu_Print_IP_Number(CSettingMenu *pMe)
{
#if 0
    number_table *temp = pMe->ip_number_tab.head;
    if(pMe->ip_number_tab.default_ip[0] != 0)
    {
    }
    
    while(temp)
    {
        temp = temp->next;
    }
#endif
}
#endif /*FEATRUE_SET_IP_NUMBER*/
