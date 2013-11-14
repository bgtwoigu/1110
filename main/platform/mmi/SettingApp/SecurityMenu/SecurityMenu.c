/*==============================================================================
// �ļ���
//        SecurityMenur.c
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
-----------      ----------  --------  -----------------------------------------------
==============================================================================*/


/*==============================================================================
                                 ���ļ��������ⲿ�ļ�
==============================================================================*/
#include "SecurityMenu_priv.h"

/*==============================================================================
                                 �궨��ͳ���
==============================================================================*/

/*==============================================================================
                                 ���Ͷ���
==============================================================================*/

/*==============================================================================
                                 ��������
==============================================================================*/
static void SecurityMenu_APPIsReadyTimer(void *pme);

/*----------------------ģ����غ�������---------------------*/
/*static*/ int  SecurityMenuMod_Load(IShell   *pIShell,
                                        void     *ph,
                                        IModule  **ppMod);

int  SecurityMenuMod_New(int16              nSize,
                                        IShell             *pIShell,
                                        void               *ph,
                                        IModule            **ppMod,
                                        PFNMODCREATEINST   pfnMC,
                                        PFNFREEMODDATA     pfnMF);

static int  SecurityMenuMod_CreateInstance(IModule   *po,
                                        IShell    *pIShell,
                                        AEECLSID  ClsId,
                                        void      **ppObj);

static uint32 SecurityMenuMod_AddRef(IModule *po);

static uint32 SecurityMenuMod_Release(IModule *po);

static void SecurityMenuMod_FreeResources(IModule     *po,
                                        IHeap       *ph,
                                        IFileMgr    *pfm);

/*----------------------Applet��غ�������---------------------*/
static int SecurityMenu_New(IShell         *ps,
                                        IModule        *pIModule,
                                        ISecurityMenu  **ppObj);

static uint32  SecurityMenu_AddRef(ISecurityMenu *pi);

static uint32  SecurityMenu_Release (ISecurityMenu *pi);

static boolean SecurityMenu_HandleEvent(ISecurityMenu *pi,
                                        AEEEvent      eCode,
                                        uint16        wParam,
                                        uint32        dwParam);

static int SecurityMenu_InitAppData(CSecurityMenu *pMe);

static void SecurityMenu_FreeAppData(CSecurityMenu *pMe);

static void SecurityMenu_RunFSM(CSecurityMenu *pMe);

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
#endif//WIN32
#endif

/*==============================================================================
                                 ���أ���̬������
==============================================================================*/
static SecurityMenuMod gSecurityMenuMod;

static const VTBL(IModule) gModFuncs =
{
    SecurityMenuMod_AddRef,
    SecurityMenuMod_Release,
    SecurityMenuMod_CreateInstance,
    SecurityMenuMod_FreeResources
};

// ֻ����һ��SecurityMenuʵ����ÿ�δ���SecurityMenu Appletʱ��
// ����ͬһ�ṹָ���BREW�㡣
static CSecurityMenu gSecurityMenu;

static const VTBL(ISecurityMenu) gSecurityMenuMethods =
{
    SecurityMenu_AddRef,
    SecurityMenu_Release,
    SecurityMenu_HandleEvent
};

/*----------------------ģ����غ�������---------------------*/

/*==============================================================================
����:
       SecurityMenuMod_Load

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
__declspec(dllexport) int AEEMod_Load(IShell    *pIShell,
                                        void      *ph,
                                        IModule   **ppMod)
#else
/*static*/ int SecurityMenuMod_Load(IShell    *pIShell,
                                        void      *ph,
                                        IModule   **ppMod)
#endif
#else
/*static*/ int SecurityMenuMod_Load(IShell    *pIShell,
                                        void      *ph,
                                        IModule   **ppMod)
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

    return SecurityMenuMod_New(sizeof(SecurityMenuMod),
                                        pIShell,
                                        ph,
                                        ppMod,
                                        NULL,
                                        NULL);
}

/*==============================================================================
����:
       SecurityMenuMod_New

˵��:
       ������ʼ��SecurityMenuģ�������Ҳ�������ü����������ֺ�����

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
int  SecurityMenuMod_New(int16             nSize,
                                        IShell            *pIShell,
                                        void              *ph,
                                        IModule           **ppMod,
                                        PFNMODCREATEINST  pfnMC,
                                        PFNFREEMODDATA    pfnMF)
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
    MEMSET(&gSecurityMenuMod,0,sizeof(SecurityMenuMod));

    INIT_VTBL(&gSecurityMenuMod,IModule,gModFuncs);
    gSecurityMenuMod.m_nRefs = 1;
    *ppMod = (IModule *)&gSecurityMenuMod;
    return AEE_SUCCESS;
}

/*==============================================================================
����:
       SecurityMenuMod_AddRef

˵��:
       SecurityMenu ģ�����ü�����1�������ص�ǰ����������

����:
       po [in]��IModule�ӿڶ���ָ�롣

����ֵ:
       ��ǰ��SecurityMenu ģ���������

��ע:

==============================================================================*/
static uint32 SecurityMenuMod_AddRef(IModule *po)
{
    return(++((SecurityMenuMod *)po)->m_nRefs);
}

/*==============================================================================
����:
       SecurityMenuMod_Release

˵��:
       SecurityMenu ģ�����ü�����1�������ص�ǰ����������

����:
       po [in]��IModule�ӿڶ���ָ�롣

����ֵ:
       ��ǰ��SecurityMenu ģ���������

��ע:

==============================================================================*/
static uint32 SecurityMenuMod_Release(IModule *po)
{
    if (((SecurityMenuMod *) po)->m_nRefs == 0)
    {
        return 0;
    }

    return(--((SecurityMenuMod *)po)->m_nRefs);
}

/*==============================================================================
����:
       SecurityMenuMod_CreateInstance

˵��:
        ͨ������(��ʼ��)SecurityMenu����������ģ��ʵ����

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
static int  SecurityMenuMod_CreateInstance(IModule    *po,
                                        IShell     *pIShell,
                                        AEECLSID   ClsId,
                                        void       **ppObj)
{
    *ppObj = NULL;

    if (ClsId != AEECLSID_APP_SECURITYMENU)
    {
        return EFAILED;
    }

    if (SecurityMenu_New(pIShell, po, (ISecurityMenu**)ppObj) != SUCCESS)
    {
        return EFAILED;
    }
    return SUCCESS;
}

/*==============================================================================
����:
       SecurityMenuMod_FreeResources

˵��:
        �ͷ�SecurityMenuģ����Դ�ĺ�����

����:
       po ��IModule��
       ph ��IHeap�ӿ�ָ�롣
       pfm :IFileMgr�ӿ�ָ�롣

����ֵ:
       ��

��ע:
       �����û�ȥ���á�

==============================================================================*/
static void SecurityMenuMod_FreeResources(IModule  *po,
                                        IHeap    *ph,
                                        IFileMgr *pfm)
{

}

/*----------------------Applet��غ�������---------------------*/
/*==============================================================================
����:
       SecurityMenu_New

˵��:
        ��ʼ��SecurityMenu Applet�������ڲ����ݱ�����ͬʱ�����ü�����1��

����:
       ps [in]��IShell�ӿڶ���ָ�롣
       pIModule [in]��pIModule�ӿڶ���ָ�롣
       ppObj [out]:��ָ��ISecurityMenu����ָ���ָ�롣

����ֵ:
       EFAILED��ʧ�ܡ�
       SUCCESS���ɹ���

��ע:
       �����û�ȥ���á�
       ����Applet�ṹ�����ʹ�ö�̬�����ڴ�ķ�ʽ�������������õ���ʹ�þ�̬
       �����ķ�ʽ������Խṹ�Ƚϴ��Appletʹ�þ�̬�����ķ�ʽ�����ڶ�̬����
       �ڴ�ķ�ʽ��Ҫע���������ռ���ͷš�

==============================================================================*/
static int SecurityMenu_New(IShell         *ps,
                                        IModule        *pIModule,
                                        ISecurityMenu  **ppObj)
{
    int retVal = SUCCESS;

    if (0 == gSecurityMenu.m_nRefs)
    {
        // Must initialize the object
        MEMSET(&gSecurityMenu,  0, sizeof(CSecurityMenu));
        INIT_VTBL(&gSecurityMenu, ISecurityMenu, gSecurityMenuMethods);

        gSecurityMenu.m_nRefs     = 0;
        gSecurityMenu.m_pShell    = ps;
        gSecurityMenu.m_pModule   = pIModule;

        (void) ISHELL_AddRef(ps);
        (void) IMODULE_AddRef(pIModule);

        retVal = SecurityMenu_InitAppData(&gSecurityMenu);
    }

    ++gSecurityMenu.m_nRefs;
    *ppObj = (ISecurityMenu*)&gSecurityMenu;
    return retVal;
}

/*==============================================================================
����:
       SecurityMenu_AddRef

˵��:
       SecurityMenu Applet ���ü�����1�������ص�ǰ����������

����:
       pi [in]��ISecurityMenu�ӿڶ���ָ�롣

����ֵ:
       ��ǰ��SecurityMenu Applet��������

��ע:

==============================================================================*/
static uint32  SecurityMenu_AddRef(ISecurityMenu *pi)
{
    register CSecurityMenu *pMe = (CSecurityMenu*)pi;

    ASSERT(pMe != NULL);
    ASSERT(pMe->m_nRefs > 0);

    return (++pMe->m_nRefs);
}

/*==============================================================================
����:
       SecurityMenu_Release

˵��:
       �ͷŶ�SecurityMenu Applet�����ã����ü�����1�������ص�ǰ����������
        �����ǰ��������Ϊ�㣬�����ͷ�Applet�������Դ��

����:
       pi [in]��ISecurityMenu�ӿڶ���ָ�롣

����ֵ:
       ��ǰ��SecurityMenu Applet��������

��ע:

==============================================================================*/
static uint32  SecurityMenu_Release (ISecurityMenu *pi)
{
    register CSecurityMenu *pMe = (CSecurityMenu*)pi;

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
    SecurityMenu_FreeAppData(pMe);

    (void) ISHELL_Release(pMe->m_pShell);
    (void) IMODULE_Release(pMe->m_pModule);

    // ע�⣺pMe�Ǿ�̬����ռ䣬��˲���Ҫ�ͷš�FREE()
    return 0;
}

/*==============================================================================
����:
       SecurityMenu_InitAppData

˵��:
       ��ʼ��SecurityMenu Applet�ṹ����ȱʡֵ��

����:
       pMe [in]��ָ��SecurityMenu Applet����ṹ��ָ�롣�ýṹ����С������ض���Ϣ��

����ֵ:
       SUCCESS����ʼ���ɹ���
       EFAILED����ʼ��ʧ�ܡ�

��ע:

==============================================================================*/
static int SecurityMenu_InitAppData(CSecurityMenu *pMe)
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
    pMe->m_ePreState = SECURITYMENU_NONE;
    pMe->m_eCurState = SECURITYMENU_INIT;
    pMe->m_pActiveDlg = NULL;
    pMe->m_eDlgRet = DLGRET_CREATE;
    pMe->m_bNotOverwriteDlgRet = FALSE;
    pMe->m_PromtImage = NULL;
    pMe->m_lock_sel = SEC_SEL_NONE;
    //pMe->passwordInputState = 0;
    pMe->m_bIsConfirmPassword = FALSE;
    pMe->m_eRUIMSCode = ENTERPUK_STEP0;
    pMe->m_strPhonePWD = NULL;
	pMe->m_currDlgId = 0;
	pMe->m_nSubDlgId = 0;
	pMe->m_strPhoneNUM = NULL;
    if (ISHELL_CreateInstance(pMe->m_pShell,
                                 AEECLSID_ANNUNCIATOR,
                                 (void **) &pMe->m_pIAnn))
    {
        SecurityMenu_FreeAppData(pMe);
        return FALSE;
    }
	MSG_FATAL("IANNUNCIATOR_SetFieldIsActiveEx::::securitymenu111:::",0,0,0);
	//IANNUNCIATOR_SetFieldIsActiveEx(pMe->m_pIAnn,FALSE);
    if (AEE_SUCCESS != ISHELL_CreateInstance(pMe->m_pShell,
                                        AEECLSID_CONFIG,
                                        (void **)&pMe->m_pConfig))
    {
        SecurityMenu_FreeAppData(pMe);
        return EFAILED;
    }

    if (AEE_SUCCESS != ISHELL_CreateInstance(pMe->m_pShell,
                                        AEECLSID_STATIC,
                                        (void **)&pMe->m_pIStatic))
    {
        SecurityMenu_FreeAppData(pMe);
        return EFAILED;
    }
#ifndef WIN32
    // ���� IRUIM �ӿ�
    if (AEE_SUCCESS != ISHELL_CreateInstance(pMe->m_pShell,
                                 AEECLSID_RUIM,
                                 (void **) &pMe->m_pIRUIM))
    {
        SecurityMenu_FreeAppData(pMe);
        return EFAILED;
    }
#endif//WIN32 
    //pMe->m_pPhoneLockPassword = NULL;
    
    pMe->nOldPSWLength = 0;
    pMe->nNewPSWLength = 0;
    
    return SUCCESS;
}

/*==============================================================================
����:
       SecurityMenu_FreeAppData

˵��:
       �ͷ�SecurityMenu Appletʹ�õ������Դ��

����:
       pMe [in]��ָ��SecurityMenu Applet����ṹ��ָ�롣�ýṹ����С������ض���Ϣ��

����ֵ:
       �ޡ�

��ע:

==============================================================================*/
static void SecurityMenu_FreeAppData(CSecurityMenu *pMe)
{
    if (NULL == pMe)
    {
        return ;
    }

    FREEIF(pMe->m_strPhonePWD);
	FREEIF(pMe->m_strPhoneNUM);

    if (pMe->m_pConfig)
    {
        ICONFIG_Release(pMe->m_pConfig);
        pMe->m_pConfig = NULL;
    }

    if (pMe->m_pIStatic)
    {
        ISTATIC_Release(pMe->m_pIStatic);
    }

    if(pMe->m_PromtImage)
    {
        IIMAGE_Release(pMe->m_PromtImage);
        pMe->m_PromtImage = NULL;
    }

     // �ͷ� IRUIM �ӿ�
    if (pMe->m_pIRUIM != NULL)
    {
        IRUIM_Release(pMe->m_pIRUIM);
        pMe->m_pIRUIM = NULL;
    }

    if (pMe->m_pIAnn)
    {
        IANNUNCIATOR_Release(pMe->m_pIAnn);
        pMe->m_pIAnn= NULL;
    }
    if(pMe->m_strPhonePWD)
    {
      FREEIF(pMe->m_strPhonePWD);
    }
	if(pMe->m_strPhoneNUM)
	{
		FREEIF(pMe->m_strPhoneNUM);
	}
    /*if(pMe->m_oldPassword)
    {
        ISTATIC_Release(pMe->m_oldPassword);
        pMe->m_oldPassword = NULL;
    }

    if(pMe->m_newPassword)
    {
        ISTATIC_Release(pMe->m_newPassword);
        pMe->m_newPassword = NULL;
    }

    if(pMe->m_confirmPassword)
    {
        ISTATIC_Release(pMe->m_confirmPassword);
        pMe->m_confirmPassword = NULL;
    }
*/
    
    //FREEIF( pMe->m_pPhoneLockPassword);
}

/*==============================================================================
����:
       SecurityMenu_RunFSM

˵��:
       SecurityMenu Applet����״̬�����档

����:
       pMe [in]��ָ��SecurityMenu Applet����ṹ��ָ�롣�ýṹ����С������ض���Ϣ��

����ֵ:
       �ޡ�

��ע:

==============================================================================*/
static void SecurityMenu_RunFSM(CSecurityMenu *pMe)
{
    NextFSMAction nextFSMAction = NFSMACTION_WAIT;

    for ( ; ; )
    {
        nextFSMAction = SecurityMenu_ProcessState(pMe);

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
       SecurityMenu_HandleEvent

˵��:
       SecurityMenu  Applet�¼��������������������ڱ�Applet���¼���
       �������ɸú�������

����:
       pi [in]��ָ��ISecurityMenu�ӿ�t�����ָ�롣
       eCode��BREW�¼����롣
       wParam�����¼�����������ݡ�
       dwParam [in]�����¼�����������ݡ�

����ֵ:
       TRUE�������¼�������
       FALSE�������¼�û������

��ע:

==============================================================================*/
static boolean SecurityMenu_HandleEvent(ISecurityMenu  *pi,
                                        AEEEvent        eCode,
                                        uint16          wParam,
                                        uint32          dwParam)
{
    CSecurityMenu *pMe = (CSecurityMenu*)pi;
    AEEAppStart *as;
    extern boolean start_security_setting_by_user;
    //nv_item_type nvi;
    //SEC_ERR("%x,%x,%x,SecurityMenu_HandleEvent",eCode,wParam,dwParam);    
    AEEDeviceInfo di;
    MSG_FATAL("SecurityMenu_HandleEvent------eCode=%d----wParam=%d",eCode,wParam,0);
    ISHELL_GetDeviceInfo(pMe->m_pShell,&di);
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
#ifdef FEATURE_LCD_TOUCH_ENABLE            
            if ((as != NULL) && (as->pszArgs != NULL))	
			{
                if (STRNCMP(as->pszArgs, "InputPW", 7) == 0)
				{
                    pMe->m_pActiveTSIMInputID=0;
					MOVE_TO_STATE(SECURITYMENU_TSIMPASSWORDINPUT);				
				}
			}
#endif       
#ifdef FEATURE_VERSION_C316
			if ((as != NULL) && (as->pszArgs != NULL))	
			{
                if (STRNCMP(as->pszArgs, "Mobiletracker", 13) == 0)
				{
                    //pMe->m_pActiveTSIMInputID=0;
					MOVE_TO_STATE(MOBILE_TRACKER);				
				}
			}
#endif
            pMe->m_pDisplay = as->pDisplay;
            (void) IDISPLAY_AddRef(pMe->m_pDisplay);
            pMe->m_rc = as->rc;
            pMe->m_rc.y = pMe->m_rc.y - SECRUITY_MENU_SOFK_HIGHT;
            pMe->m_rc.dy = di.cyScreen;
            pMe->m_bSuspending = FALSE;
            if(pMe->m_pIAnn != NULL)
            {
                IANNUNCIATOR_SetFieldIsActiveEx(pMe->m_pIAnn,FALSE);
            }
            // ��ʼSecurityMenu״̬��
            SecurityMenu_RunFSM(pMe);
            start_security_setting_by_user = FALSE;
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
            pMe->m_rc.y = pMe->m_rc.y - SECRUITY_MENU_SOFK_HIGHT;
            pMe->m_rc.dy = di.cyScreen;
			if( start_security_setting_by_user)
			{
				pMe->m_ePreState = SECURITYMENU_NONE;
				pMe->m_eCurState = SECURITYMENU_INIT;
				pMe->m_currDlgId = 0;
				pMe->m_nSubDlgId = 0;
				start_security_setting_by_user = FALSE;
			}
            SecurityMenu_RunFSM(pMe);
            return TRUE;

        case EVT_DIALOG_INIT:
            if( wParam == OEM_IME_DIALOG)
			{
				return TRUE;
			}
            pMe->m_bAppIsReady = FALSE;
            pMe->m_pActiveDlg = (IDialog*)dwParam;
            pMe->m_pActiveDlgID = wParam;

            return SecurityMenu_RouteDialogEvent(pMe,eCode,wParam,dwParam);

        case EVT_DIALOG_START:
            if(OEM_IME_DIALOG == wParam)
			{
				return TRUE;
			}
             (void)ISHELL_SetTimer ( pMe->m_pShell,
                                                APPISREADY_TIMER,
                                                SecurityMenu_APPIsReadyTimer,
                                                pMe);
             
            return SecurityMenu_RouteDialogEvent(pMe,eCode,wParam,dwParam);

        case EVT_USER_REDRAW:
            (void) SecurityMenu_RouteDialogEvent(pMe,eCode,wParam,dwParam);

            /*
            (void)ISHELL_SetTimer ( pMe->m_pShell,
                                                APPISREADY_TIMER,
                                                SecurityMenu_APPIsReadyTimer,
                                                pMe);
            */
                
            return TRUE;

        case EVT_APPISREADY:
            pMe->m_bAppIsReady = TRUE;
            return TRUE;

        case EVT_KEY_PRESS:
        case EVT_KEY_RELEASE:
        case EVT_KEY:
        case EVT_COMMAND:
			#if !defined(FEATURE_VERSION_K212) && !defined(FEATURE_VERSION_EC99)&&!defined(FEATURE_QVGA_INHERIT_K212)&&!defined(FEATURE_VERSION_K212_HUALU)
            if (!pMe->m_bAppIsReady)
            {
                return TRUE;
            }
			#endif
            return SecurityMenu_RouteDialogEvent(pMe,eCode,wParam,dwParam);

        case EVT_DIALOG_END:
            if (wParam == 0)
            {
                return TRUE;
            }
            if(OEM_IME_DIALOG == wParam)
			{
				return ISHELL_PostEvent(pMe->m_pShell,AEECLSID_APP_CONTACT,EVT_USER_REDRAW,0,0);
			}
            pMe->m_bAppIsReady = FALSE;
            (void) SecurityMenu_RouteDialogEvent(pMe,eCode,wParam,dwParam);
            pMe->m_pActiveDlg = NULL;

            // Applet�����𣬲���״̬�����ȴ�Applet���ء���ע�⣺EVT_APP_SUSPEND
            // �¼���EVT_DIALOG_END�¼�ǰ������
            if (pMe->m_bSuspending == FALSE)
            {
                // ��ʼSecurityMenu״̬��
                SecurityMenu_RunFSM(pMe);
            }
            return TRUE;

#ifdef FEATURE_LCD_TOUCH_ENABLE//wlh add for LCD touch
		case EVT_PEN_UP:
		//case EVT_PEN_DOWN:
			{
				MSG_FATAL("EVT_PEN_UP......................",0,0,0);
				return SecurityMenu_RouteDialogEvent(pMe,eCode,wParam,dwParam);
			}
			break;

		case EVT_USER:
			{
					MSG_FATAL("EVT_USER......................",0,0,0);
				if((wParam == AVK_SELECT) || (wParam == AVK_INFO))
				{
					if(dwParam == DWPARMA_OTHER)
					{
						eCode = EVT_KEY_RELEASE;
						dwParam = 0;
					}
					else if(dwParam != 0)
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
					MSG_FATAL("AVK_CLR......................",0,0,0);
					eCode = EVT_KEY;
				}
				return SecurityMenu_RouteDialogEvent(pMe,eCode,wParam,dwParam);
			}
#endif//FEATURE_LCD_TOUCH_ENABLE
        default:
            // �����յ����¼�·������ǰ��ĶԻ����¼���������
            return SecurityMenu_RouteDialogEvent(pMe,eCode,wParam,dwParam);
    }
}

/*==============================================================================
����:
       SecurityMenu_APPIsReadyTimer

˵��:
       ��ʱ���ص���������Ҫ���ڿ��ƿ��ٰ�������Ĵ���

����:
       pme [in]��void *����ָ�룬��ָCSecurityMenu�ṹָ�롣

����ֵ:
       �ޡ�

��ע:

==============================================================================*/
static void SecurityMenu_APPIsReadyTimer(void *pme)
{
    CSecurityMenu *pMe = (CSecurityMenu *)pme;

    if (NULL == pMe)
    {
        return;
    }

    (void) ISHELL_PostEvent(pMe->m_pShell,
                                        AEECLSID_APP_SECURITYMENU,
                                        EVT_APPISREADY,
                                        0,
                                        0);
}
