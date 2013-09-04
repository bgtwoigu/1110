/*==============================================================================
// �ļ���
//        DisplayMenu.c
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
#include "DisplayMenu_priv.h"

/*==============================================================================
                                 �궨��ͳ���
==============================================================================*/

/*==============================================================================
                                 ���Ͷ���
==============================================================================*/

/*==============================================================================
                                 ��������
==============================================================================*/
static void DisplayMenu_APPIsReadyTimer(void *pme);

/*----------------------ģ����غ�������---------------------*/

/*static*/ int  DisplayMenuMod_Load(IShell   *pIShell,
    void     *ph,
    IModule **ppMod
);

int  DisplayMenuMod_New(int16  nSize,
    IShell   *pIShell,
    void     *ph,
    IModule  **ppMod,
    PFNMODCREATEINST pfnMC,
    PFNFREEMODDATA pfnMF
);

static int  DisplayMenuMod_CreateInstance(IModule *po,
    IShell *pIShell,
    AEECLSID ClsId,
    void **ppObj
);

static uint32 DisplayMenuMod_AddRef(IModule *po);

static uint32 DisplayMenuMod_Release(IModule *po);

static void DisplayMenuMod_FreeResources(IModule  *po,
    IHeap  *ph,
    IFileMgr *pfm
);

/*----------------------Applet��غ�������---------------------*/
static int DisplayMenu_New(IShell *ps,
    IModule *pIModule,
    IDisplayMenu **ppObj
);

static uint32  DisplayMenu_AddRef(IDisplayMenu *pi);

static uint32  DisplayMenu_Release (IDisplayMenu *pi);

static boolean DisplayMenu_HandleEvent(IDisplayMenu *pi,
    AEEEvent eCode,
    uint16  wParam,
    uint32 dwParam
);

static void DisplayMenu_ChangePictrue(IDisplayMenu * pMe, boolean direction);
static int DisplayMenu_InitAppData(CDisplayMenu *pMe);

static void DisplayMenu_FreeAppData(CDisplayMenu *pMe);

static void DisplayMenu_RunFSM(CDisplayMenu *pMe);

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
static DisplayMenuMod gDisplayMenuMod;

static const VTBL(IModule) gModFuncs =
{
    DisplayMenuMod_AddRef,
    DisplayMenuMod_Release,
    DisplayMenuMod_CreateInstance,
    DisplayMenuMod_FreeResources
};

// ֻ����һ��DisplayMenuʵ����ÿ�δ���DisplayMenu Appletʱ��
// ����ͬһ�ṹָ���BREW�㡣
static CDisplayMenu gDisplayMenu;

static const VTBL(IDisplayMenu) gDisplayMenuMethods =
{
    DisplayMenu_AddRef,
    DisplayMenu_Release,
    DisplayMenu_HandleEvent,
    DisplayMenu_ChangePictrue
};

/*----------------------ģ����غ�������---------------------*/

/*==============================================================================
����:
       DisplayMenuMod_Load

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
/*static*/ int DisplayMenuMod_Load(IShell   *pIShell,
                                                   void     *ph,
                                                   IModule **ppMod)
#endif
#else
/*static*/ int DisplayMenuMod_Load(IShell   *pIShell,
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

    return DisplayMenuMod_New(sizeof(DisplayMenuMod),
                    pIShell,
                    ph,
                    ppMod,
                    NULL,
                    NULL);
}

/*==============================================================================
����:
       DisplayMenuMod_New

˵��:
       ������ʼ��DisplayMenuģ�������Ҳ�������ü����������ֺ�����

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
int  DisplayMenuMod_New(int16  nSize,
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
    if (!pIShell)
    {
        return EFAILED;
    }

    MEMSET(&gDisplayMenuMod,0,sizeof(DisplayMenuMod));

    INIT_VTBL(&gDisplayMenuMod,IModule,gModFuncs);
    gDisplayMenuMod.m_nRefs = 1;
    *ppMod = (IModule *)&gDisplayMenuMod;
    return AEE_SUCCESS;
}

/*==============================================================================
����:
       DisplayMenuMod_AddRef

˵��:
       DisplayMenu ģ�����ü�����1�������ص�ǰ����������

����:
       po [in]��IModule�ӿڶ���ָ�롣

����ֵ:
       ��ǰ��DisplayMenu ģ���������

��ע:

==============================================================================*/
static uint32 DisplayMenuMod_AddRef(IModule *po)
{
    return(++((DisplayMenuMod *)po)->m_nRefs);
}

/*==============================================================================
����:
       DisplayMenuMod_Release

˵��:
       DisplayMenu ģ�����ü�����1�������ص�ǰ����������

����:
       po [in]��IModule�ӿڶ���ָ�롣

����ֵ:
       ��ǰ��DisplayMenu ģ���������

��ע:

==============================================================================*/
static uint32 DisplayMenuMod_Release(IModule *po)
{
    if (((DisplayMenuMod *) po)->m_nRefs == 0)
    {
        return 0;
    }

    return(--((DisplayMenuMod *)po)->m_nRefs);
}

/*==============================================================================
����:
       DisplayMenuMod_CreateInstance

˵��:
        ͨ������(��ʼ��)DisplayMenu����������ģ��ʵ����

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
static int  DisplayMenuMod_CreateInstance(IModule *po,
    IShell *pIShell,
    AEECLSID ClsId,
    void **ppObj
)
{
    *ppObj = NULL;

    if (ClsId != AEECLSID_APP_DISPLAYMENU)
    {
        return EFAILED;
    }

    if (DisplayMenu_New(pIShell, po, (IDisplayMenu**)ppObj) != SUCCESS)
    {
        return EFAILED;
    }
    return SUCCESS;
}

/*==============================================================================
����:
       DisplayMenuMod_FreeResources

˵��:
        �ͷ�DisplayMenuģ����Դ�ĺ�����

����:
       po ��IModule��
       ph ��IHeap�ӿ�ָ�롣
       pfm :IFileMgr�ӿ�ָ�롣

����ֵ:
       ��

��ע:
       �����û�ȥ���á�

==============================================================================*/
static void DisplayMenuMod_FreeResources(IModule  *po,
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
       DisplayMenu_New

˵��:
        ��ʼ��DisplayMenu Applet�������ڲ����ݱ�����ͬʱ�����ü�����1��

����:
       ps [in]��IShell�ӿڶ���ָ�롣
       pIModule [in]��pIModule�ӿڶ���ָ�롣
       ppObj [out]:��ָ��IDisplayMenu����ָ���ָ�롣

����ֵ:
       EFAILED��ʧ�ܡ�
       SUCCESS���ɹ���

��ע:
       �����û�ȥ���á�
       ����Applet�ṹ�����ʹ�ö�̬�����ڴ�ķ�ʽ�������������õ���ʹ�þ�̬
       �����ķ�ʽ������Խṹ�Ƚϴ��Appletʹ�þ�̬�����ķ�ʽ�����ڶ�̬����
       �ڴ�ķ�ʽ��Ҫע���������ռ���ͷš�

==============================================================================*/
static int DisplayMenu_New(IShell *ps,
    IModule *pIModule,
    IDisplayMenu **ppObj
)
{
    int retVal = SUCCESS;

    if (0 == gDisplayMenu.m_nRefs)
    {
        // Must initialize the object
        MEMSET(&gDisplayMenu,  0, sizeof(CDisplayMenu));
        INIT_VTBL(&gDisplayMenu, IDisplayMenu, gDisplayMenuMethods);

        gDisplayMenu.m_nRefs     = 0;
        gDisplayMenu.m_pShell    = ps;
        gDisplayMenu.m_pModule   = pIModule;

        (void) ISHELL_AddRef(ps);
        (void) IMODULE_AddRef(pIModule);

        retVal = DisplayMenu_InitAppData(&gDisplayMenu);
    }

    ++gDisplayMenu.m_nRefs;
    *ppObj = (IDisplayMenu*)&gDisplayMenu;
    return retVal;
}

/*==============================================================================
����:
       DisplayMenu_AddRef

˵��:
       DisplayMenu Applet ���ü�����1�������ص�ǰ����������

����:
       pi [in]��IDisplayMenu�ӿڶ���ָ�롣

����ֵ:
       ��ǰ��DisplayMenu Applet��������

��ע:

==============================================================================*/
static uint32  DisplayMenu_AddRef(IDisplayMenu *pi)
{
    register CDisplayMenu *pMe = (CDisplayMenu*)pi;

    ASSERT(pMe != NULL);
    ASSERT(pMe->m_nRefs > 0);

    return (++pMe->m_nRefs);
}

/*==============================================================================
����:
       DisplayMenu_Release

˵��:
       �ͷŶ�DisplayMenu Applet�����ã����ü�����1�������ص�ǰ����������
        �����ǰ��������Ϊ�㣬�����ͷ�Applet�������Դ��

����:
       pi [in]��IDisplayMenu�ӿڶ���ָ�롣

����ֵ:
       ��ǰ��DisplayMenu Applet��������

��ע:

==============================================================================*/
static uint32  DisplayMenu_Release (IDisplayMenu *pi)
{
    register CDisplayMenu *pMe = (CDisplayMenu*)pi;

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
    DisplayMenu_FreeAppData(pMe);

    (void) ISHELL_Release(pMe->m_pShell);
    (void) IMODULE_Release(pMe->m_pModule);

    // ע�⣺pMe�Ǿ�̬����ռ䣬��˲���Ҫ�ͷš�FREE()
    return 0;
}

/*==============================================================================
����:
       DisplayMenu_InitAppData

˵��:
       ��ʼ��DisplayMenu Applet�ṹ����ȱʡֵ��

����:
       pMe [in]��ָ��DisplayMenu Applet����ṹ��ָ�롣�ýṹ����С������ض���Ϣ��

����ֵ:
       SUCCESS����ʼ���ɹ���
       EFAILED����ʼ��ʧ�ܡ�

��ע:

==============================================================================*/
static int DisplayMenu_InitAppData(CDisplayMenu *pMe)
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
    pMe->m_ePreState = DISPLAYMENUST_NONE;
    pMe->m_eCurState = DISPLAYMENUST_INIT;
    pMe->m_pActiveDlg = NULL;
    pMe->m_eDlgRet = DLGRET_CREATE;
    pMe->m_bNotOverwriteDlgRet = FALSE;

    pMe->m_currDlgId = 0;
    pMe->m_nSubDlgId = 0;
    pMe->m_nExSubDlgId = 0;

    pMe->m_pWallPaper = NULL;
    pMe->m_pAniPaper = NULL;
    //pMe->m_pTitleImg = NULL;

    pMe->m_DelImageBack = FALSE;    //Ĭ��ȡͼƬ��NVֵ
    pMe->m_bOkToDo = TRUE;
    pMe->m_msg_id = 0;
    if (AEE_SUCCESS != ISHELL_CreateInstance(pMe->m_pShell,
                                            AEECLSID_CONFIG,
                                            (void **)&pMe->m_pConfig))
    {
        DisplayMenu_FreeAppData(pMe);
        return EFAILED;
    }

    if (AEE_SUCCESS != ISHELL_CreateInstance(pMe->m_pShell,
                                            AEECLSID_BACKLIGHT,
                                            (void **)&pMe->m_pIBacklight))
    {
        DisplayMenu_FreeAppData(pMe);
        return EFAILED;
    }
#if defined(FEATURE_BACKLIGHT_KEYPAD)
    if (AEE_SUCCESS != ISHELL_CreateInstance(pMe->m_pShell,
                                            AEECLSID_BACKLIGHT_KEYPAD,
                                            (void **)&pMe->m_pKeypadBacklight))
    {
        DisplayMenu_FreeAppData(pMe);
        return EFAILED;
    }
#endif
    if (AEE_SUCCESS != ISHELL_CreateInstance(pMe->m_pShell,AEECLSID_ANNUNCIATOR,(void **)&pMe->m_pIAnn))
    {
        DisplayMenu_FreeAppData(pMe);
        return EFAILED;
    }
    
    if (AEE_SUCCESS != ISHELL_CreateInstance(pMe->m_pShell,
                                            AEECLSID_FILEMGR,
                                            (void **)&pMe->m_pIFileMgr))
    {
        DisplayMenu_FreeAppData(pMe);
        return EFAILED;
    }
    return SUCCESS;
}

/*==============================================================================
����:
       DisplayMenu_FreeAppData

˵��:
       �ͷ�DisplayMenu Appletʹ�õ������Դ��

����:
       pMe [in]��ָ��DisplayMenu Applet����ṹ��ָ�롣�ýṹ����С������ض���Ϣ��

����ֵ:
       �ޡ�

��ע:

==============================================================================*/
static void DisplayMenu_FreeAppData(CDisplayMenu *pMe)
{
    if (NULL == pMe)
    {
        return ;
    }

    if (pMe->m_pConfig)
    {
        ICONFIG_Release(pMe->m_pConfig);
        pMe->m_pConfig = NULL;
    }

    if(pMe->m_pIBacklight)
    {
        IBACKLIGHT_Release(pMe->m_pIBacklight);
        pMe->m_pIBacklight = NULL;
    }
#if defined(FEATURE_BACKLIGHT_KEYPAD)
    if(pMe->m_pKeypadBacklight)
    {
        IBACKLIGHT_Release(pMe->m_pKeypadBacklight);
        pMe->m_pKeypadBacklight = NULL;
    }
#endif
    if(pMe->m_pIAnn)
    {
        IANNUNCIATOR_Release(pMe->m_pIAnn);
        pMe->m_pIAnn = NULL;
    }
    
    if(pMe->m_pIFileMgr)
    {
        IFILEMGR_Release(pMe->m_pIFileMgr);
        pMe->m_pIFileMgr = NULL;
    }

    if (pMe->m_pWallPaper != NULL)
    {
       IIMAGE_Release(pMe->m_pWallPaper);
       pMe->m_pWallPaper = NULL;
    }

    if (pMe->m_pAniPaper != NULL)
    {
       IIMAGE_Release(pMe->m_pAniPaper);
       pMe->m_pAniPaper = NULL;
    }

    //if (pMe->m_pTitleImg != NULL)
    //{
    //   IIMAGE_Release(pMe->m_pTitleImg);
    //   pMe->m_pTitleImg = NULL;
    //}
}

/*==============================================================================
����:
       DisplayMenu_RunFSM

˵��:
       DisplayMenu Applet����״̬�����档

����:
       pMe [in]��ָ��DisplayMenu Applet����ṹ��ָ�롣�ýṹ����С������ض���Ϣ��

����ֵ:
       �ޡ�

��ע:

==============================================================================*/
static void DisplayMenu_RunFSM(CDisplayMenu *pMe)
{
    NextFSMAction nextFSMAction = NFSMACTION_WAIT;

    for ( ; ; )
    {
        nextFSMAction = DisplayMenu_ProcessState(pMe);

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
       DisplayMenu_HandleEvent

˵��:
       DisplayMenu  Applet�¼��������������������ڱ�Applet���¼���
       �������ɸú�������

����:
       pi [in]��ָ��IDisplayMenu�ӿ�t�����ָ�롣
       eCode��BREW�¼����롣
       wParam�����¼�����������ݡ�
       dwParam [in]�����¼�����������ݡ�

����ֵ:
       TRUE�������¼�������
       FALSE�������¼�û������

��ע:

==============================================================================*/
static boolean DisplayMenu_HandleEvent(IDisplayMenu *pi,
    AEEEvent eCode,
    uint16  wParam,
    uint32 dwParam
)
{
    CDisplayMenu *pMe = (CDisplayMenu*)pi;
    AEEAppStart *as;
    //DISP_ERR("%x, %x ,%x,DisplayMenu_HandleEvent",eCode,wParam,dwParam);
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

            pMe->m_pDisplay = as->pDisplay;
            (void) IDISPLAY_AddRef(pMe->m_pDisplay);
            pMe->m_rc = as->rc;
            pMe->m_rc.y = pMe->m_rc.y - DISPLAY_MENU_SOFK_HIGHT;
            pMe->m_bSuspending = FALSE;

            // ��ʼDisplayMenu״̬��
            DisplayMenu_RunFSM(pMe);
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
            pMe->m_rc.y = pMe->m_rc.y - DISPLAY_MENU_SOFK_HIGHT;
            //���𷵻�ʱ�����ͼƬΪ���һ�ο���������
            pMe->m_DelImageBack = TRUE;

            DisplayMenu_RunFSM(pMe);
            return TRUE;

        case EVT_DIALOG_INIT:
            pMe->m_bAppIsReady = FALSE;
            pMe->m_pActiveDlg = (IDialog*)dwParam;
            pMe->m_pActiveDlgID = wParam;

            return DisplayMenu_RouteDialogEvent(pMe,eCode,wParam,dwParam);

        case EVT_DIALOG_START:
             (void)ISHELL_SetTimer ( pMe->m_pShell,
                            APPISREADY_TIMER,
                            DisplayMenu_APPIsReadyTimer,
                            pMe);
             
            return DisplayMenu_RouteDialogEvent(pMe,eCode,wParam,dwParam);

        case EVT_USER_REDRAW:
            (void) DisplayMenu_RouteDialogEvent(pMe,eCode,wParam,dwParam);

            /*
            (void)ISHELL_SetTimer ( pMe->m_pShell,
                            APPISREADY_TIMER,
                            DisplayMenu_APPIsReadyTimer,
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
			#if !defined(FEATURE_VERSION_K212)
            if (!pMe->m_bAppIsReady)
            {
                return TRUE;
            }
			#endif
            return DisplayMenu_RouteDialogEvent(pMe,eCode,wParam,dwParam);

        case EVT_DIALOG_END:
            if (wParam == 0)
            {
                return TRUE;
            }

            pMe->m_bAppIsReady = FALSE;
            (void) DisplayMenu_RouteDialogEvent(pMe,eCode,wParam,dwParam);
            pMe->m_pActiveDlg = NULL;

            // Applet�����𣬲���״̬�����ȴ�Applet���ء���ע�⣺EVT_APP_SUSPEND
            // �¼���EVT_DIALOG_END�¼�ǰ������
            if (pMe->m_bSuspending == FALSE)
            {
                // ��ʼDisplayMenu״̬��
                DisplayMenu_RunFSM(pMe);
            }
            return TRUE;
#ifdef FEATURE_LCD_TOUCH_ENABLE//wlh add for LCD touch
		case EVT_PEN_UP:
		//case EVT_PEN_DOWN:
			{
				return DisplayMenu_RouteDialogEvent(pMe,eCode,wParam,dwParam);
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
				else if(wParam == AVK_CLR)
				{
					eCode = EVT_KEY;
				}
				else if(wParam == AVK_DOWN)//wlh ������ͼƬ
				{
					eCode = EVT_KEY;
				}
				return DisplayMenu_RouteDialogEvent(pMe,eCode,wParam,dwParam);
			}
#endif//FEATURE_LCD_TOUCH_ENABLE
        default:
            // �����յ����¼�·������ǰ��ĶԻ����¼���������
            return DisplayMenu_RouteDialogEvent(pMe,eCode,wParam,dwParam);
    }
}

/*==============================================================================
����:
       DisplayMenu_APPIsReadyTimer

˵��:
       ��ʱ���ص���������Ҫ���ڿ��ƿ��ٰ�������Ĵ���

����:
       pme [in]��void *����ָ�룬��ָCDisplayMenu�ṹָ�롣

����ֵ:
       �ޡ�

��ע:

==============================================================================*/
static void DisplayMenu_APPIsReadyTimer(void *pme)
{
    CDisplayMenu *pMe = (CDisplayMenu *)pme;

    if (NULL == pMe)
    {
        return;
    }

    (void) ISHELL_PostEvent( pMe->m_pShell,
                            AEECLSID_APP_DISPLAYMENU,
                            EVT_APPISREADY,
                            0,
                            0);
}

/****************************************************************/
static double_list *p_wallpaper = NULL; 

static double_list * Display_Creat_Double_list(void)
{
    double_list * temp = NULL;
    DISP_ERR("Display_Creat_Double_list",0,0,0);
    temp = (double_list*) MALLOC(sizeof(double_list));
    if(temp)
    {
        temp->phead = NULL;
        temp->ptail    = NULL;
        temp->pcur   = NULL;
        temp->count =0;
    }
    return temp;
}

static void Display_Free_Double_list(double_list * plist )
{
    image_name_table *list_head = NULL;
    image_name_table *temp = NULL;

    //DISP_ERR("DisplayMenu_Free_Double_list",0,0,0);

    if((plist == NULL) ||(plist->phead == NULL))
    {
        return;
    }
    DISP_ERR("%x Display_Free_Double_list start",plist->phead,0,0);
    list_head = plist->phead ;

    while(list_head)
    {
        temp =list_head->next;
        FREE(list_head);
        list_head = temp;
        temp = NULL;
    }

    list_head = NULL;

}

static void Display_Add_Node_To_Double_list_Tail(double_list * plist ,char * image_name)
{
    image_name_table *temp = NULL;

    DISP_ERR("DisplayMenu_Add_Node_To_Double_list_Tail",0,0,0);

    if((plist == NULL) ||(image_name == NULL))
    {
        return;
    }

    temp = (image_name_table*)MALLOC(sizeof(image_name_table));

    if(temp)
    {
        if(plist->count == 0)/*�յ�˫������*/
        {
            temp->next = NULL;
            temp->pref = NULL;
            plist->phead = temp;
            plist->ptail   = temp;
        }
        else
        {
            temp->next = NULL;
            temp->pref = plist->ptail;
            plist->ptail->next = temp;
            plist->ptail  = temp;
        }

        STRCPY(temp->imange_name , image_name);
        plist->count ++ ;
    }
    else
    {
        DISP_ERR("MALLOC FAIL,Cann't add",0,0,0);
    }
}

static image_name_table * Display_Search_Name_In_Double_list(double_list * plist ,char * image_name , uint16 * count)
{
    image_name_table *b_find = NULL;
    image_name_table *temp   = NULL;
    uint16 i = 1;

    //DISP_ERR("DisplayMenu_Search_Name_In_Double_list",0,0,0);

    if(plist == NULL || image_name == NULL)
    {
        return NULL;
    }

    temp =  plist->phead;

    while(temp != NULL)
    {
        if(0 == STRCMP (image_name,temp->imange_name))
        {
            b_find = temp;
            break;
        }
        temp = temp->next;
        i ++ ;
    }

    if(b_find)
    {
        DISP_ERR("ok,find it %d",i,0,0);
    }
    else
    {
        i = 0;
        DISP_ERR("fail,not find",0,0,0);
    }

    *count = i;

    return b_find;
}

static image_name_table * Display_Search_Next_In_Double_list(double_list * plist ,image_name_table * p_cur,uint16 * img_id)
{
    if((p_cur ==plist->phead)&& (p_cur ==plist->ptail)) /*Only one*/
    {
        * img_id = 1;
        return p_cur;
    }
    else if ((p_cur !=plist->phead)&& (p_cur ==plist->ptail))  /*the tail */
    {
        * img_id = 1;
        return plist->phead;
    }
    else
    {
        (* img_id) ++;
        return p_cur->next;
    }

}

static image_name_table * Display_Search_Pref_In_Double_list(double_list * plist ,image_name_table * p_cur,uint16 * img_id)
{
    if((p_cur ==plist->phead)&& (p_cur ==plist->ptail)) /*Only one*/
    {
        * img_id = 1;
        return p_cur;
    }
    else if ((p_cur ==plist->phead)&& (p_cur !=plist->ptail))  /*the head */
    {
        * img_id = plist->count;
        return plist->ptail;
    }
    else
    {
        (* img_id) --;
        return p_cur->pref;
    }

}
/*==============================================================================
������
       Display_EnumFile
˵����
       ����ö�ٳ����е�ǽֽ�򶯻�

������
       pMe [in]��ָ��DisplayMenu Applet����ṹ��ָ�롣�ýṹ����С������ض���Ϣ��
       PICTURE_TYPE [in]��PIC���͡�

����ֵ��
       void

��ע��

==============================================================================*/
static void Display_EnumFile(CDisplayMenu *pMe)
{
    FileInfo  Photopos;
    int         Start_bit = 0; /*�ж���ǽֽ�������������ǹػ�����*/
    char      PhotoName[AEE_MAX_FILE_NAME/*FILESPECLEN*/];

    p_wallpaper = Display_Creat_Double_list();
    
    if(IFILEMGR_EnumInit(pMe->m_pIFileMgr, "fs:/image/wallpaper/", FALSE)  == SUCCESS)
    {
        while(IFILEMGR_EnumNext(pMe->m_pIFileMgr, &Photopos))
        {                	    
		    //�ж�������ǽֽ�������������ǹػ��������ֱ���w��k��g��ĸ��ͷ
		    if(Photopos.attrib & AEE_FA_CONST)
		    {
		    	MEMSET(PhotoName,0,sizeof(PhotoName));                 
		    	STRCPY(PhotoName,Photopos.szName);   /*���оٳ����ļ�������PhotoName��*/
		        //DISP_ERR("Photopos.szName: %d,Photopos.attrib: %d",Photopos.szName ,Photopos.attrib,0);
		        if(p_wallpaper)
		        {
		            Display_Add_Node_To_Double_list_Tail(p_wallpaper,PhotoName);
		        }
		    }
         }
    }
}


static void DisplayMenu_ChangePictrue(IDisplayMenu * pme, boolean direction)
{
    CDisplayMenu *pMe = (CDisplayMenu *)pme;
    char wallnv[AEE_MAX_FILE_NAME/*FILESPECLEN*/];
    
    pMe->m_count = 0;

    if (NULL == pMe)
    {
        return;
    }
    
    Display_EnumFile(pMe);
        
    (void) ICONFIG_GetItem(pMe->m_pConfig,
                            CFGI_WALLPAPER,
                            wallnv,
                            sizeof(wallnv));
    
    pMe->m_CurPaper = Display_Search_Name_In_Double_list(p_wallpaper,wallnv,&pMe->m_count);

    if(pMe->m_CurPaper != NULL)
    {
    if(direction)
    {
        pMe->m_CurPaper = Display_Search_Pref_In_Double_list(p_wallpaper,pMe->m_CurPaper,&pMe->m_count);
    }
    else
    {
        pMe->m_CurPaper = Display_Search_Next_In_Double_list(p_wallpaper,pMe->m_CurPaper,&pMe->m_count);
    }
    (void) ICONFIG_SetItem(pMe->m_pConfig,
                            CFGI_WALLPAPER,
                            pMe->m_CurPaper->imange_name,
                            sizeof(pMe->m_CurPaper->imange_name));
    }
    else
    {
        // ��ʼ��ǽֽ
        MEMSET(wallnv, 0x00, sizeof(wallnv));

        (void) ICONFIG_SetItem(pMe->m_pConfig,
                                CFGI_WALLPAPER,
                                OEMNV_WALLPAPER,
                                sizeof(OEMNV_WALLPAPER));

    }

    Display_Free_Double_list(p_wallpaper);
    if(p_wallpaper)
    {
        FREE(p_wallpaper);
        p_wallpaper = NULL;
    }
    
    if(pMe->m_CurPaper != NULL)
    {
        pMe->m_CurPaper = NULL;
    }
}

