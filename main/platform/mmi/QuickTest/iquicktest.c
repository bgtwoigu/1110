/*==============================================================================
// �ļ���
//        IQuickTest.c
//        ��Ȩ����(c) 2007 Anylook
//        
// �ļ�˵����
//        
// ���ߣ�Gemsea
// �������ڣ�2007-11-13
// ��ǰ�汾��Draft
==============================================================================*/

/*==============================================================================
�޸���ʷ��¼��
ʱ       ��      �� �� ��     �޸����ݡ�λ�ü�ԭ��
-----------      ----------     -----------------------------------------------
07-11-13                ��ʼ�汾
==============================================================================*/


/*==============================================================================
                                 ���ļ��������ⲿ�ļ�
==============================================================================*/
#include "iquicktest_priv.h"

/*==============================================================================
                                 �궨��ͳ���
==============================================================================*/

/*==============================================================================
                                 ���Ͷ���
==============================================================================*/

/*==============================================================================
                                 ��������
==============================================================================*/

static void QuickTest_APPIsReadyTimer(void *pme);

/*----------------------ģ����غ�������---------------------*/
#if defined(BREW_STATIC_APP)
                                    
int QuickTest_Load(IShell   *pIShell, 
                             void     *ph, 
                             IModule **ppMod);

static int QuickTest_CreateInstance( AEECLSID ClsId,
                                     IShell  *pIShell,
                                     IModule *po,
                                     void   **ppObj);
#endif

/*----------------------Applet��غ�������---------------------*/
static int     CQuickTest_New(IShell       *ps,
                              IModule      *pIModule,
                              IQuickTest **ppObj);
static uint32  CQuickTest_AddRef(IQuickTest *pi);
static uint32  CQuickTest_Release(IQuickTest *pi);
static boolean CQuickTest_HandleEvent(IQuickTest *pi,
                                      AEEEvent    eCode,
                                      uint16      wParam,
                                      uint32      dwParam);
static int     CQuickTest_InitData(CQuickTest *pMe);
static void    CQuickTest_FreeAppData(CQuickTest *pMe);
static void    CQuickTest_RunFSM(CQuickTest *pMe);
/*==============================================================================
                                 ȫ������
==============================================================================*/

/*==============================================================================
                                 ���أ���̬������
==============================================================================*/

// ֻ����һ��CQuickTestʵ����ÿ�δ���CQuickTest Appletʱ��
// ����ͬһ�ṹָ���BREW�㡣
//static CQuickTest gQuickTest;

static const VTBL(IQuickTest) gIQuickTestMethods =
{
   CQuickTest_AddRef,
   CQuickTest_Release,
   CQuickTest_HandleEvent
};

/*----------------------ģ����غ�������---------------------*/

#if defined(BREW_STATIC_APP)

/*==============================================================================
����: 
       QuickTest_Load
       
˵��: 
       ģ��װ�غ�����
       
����: 
       pIShell [in]��IShell�ӿ�ָ�롣
       ph ��û��ʹ��
       ppMod [out]��
       
����ֵ:
       int
       
��ע:
       
===========================================================================*/
int QuickTest_Load(IShell   *pIShell,
                             void     *ph,
                             IModule **ppMod)
{
   return AEEStaticMod_New(sizeof(AEEMod),
                           pIShell,
                           ph,
                           ppMod,
                           (PFNMODCREATEINST)QuickTest_CreateInstance,
                           (PFNFREEMODDATA)NULL);
}
#endif
/*==============================================================================
����: 
       QuickTest_CreateInstance
       
˵��: 
        ͨ������(��ʼ��)QuickTest����������ģ��ʵ����
       
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
#if defined(AEE_STATIC)
static int QuickTest_CreateInstance( AEECLSID ClsId,
                                     IShell  *pIShell,
                                     IModule *po,
                                     void   **ppObj)
#else
int AEEClsCreateInstance( AEECLSID   ClsId,
                          IShell    *pIShell,
                          IModule   *po,
                          void     **ppObj)
#endif
{
   *ppObj = NULL;

   if (ClsId != AEECLSID_QUICKTEST) {
      return EFAILED;
   }
   if (CQuickTest_New(pIShell, po, (IQuickTest**)ppObj) != SUCCESS) {
      return EFAILED;
   }
   return SUCCESS;
}
/*----------------------Applet��غ�������---------------------*/
/*==============================================================================
����: 
       CQuickTest_New
       
˵��: 
        CQuickTest Applet�������ڲ����ݱ�����ͬʱ�����ü�����1��
       
����: 
       ps [in]��IShell�ӿڶ���ָ�롣
       pIModule [in]��pIModule�ӿڶ���ָ�롣
       ppObj [out]:��ָ��IQuickTest����ָ���ָ�롣
       
����ֵ:
       EFAILED��ʧ�ܡ�
       SUCCESS���ɹ���
       
��ע:
       �����û�ȥ���á�
       ����Applet�ṹ�����ʹ�ö�̬�����ڴ�ķ�ʽ�������������õ���ʹ�þ�̬
       �����ķ�ʽ������Խṹ�Ƚϴ��Appletʹ�þ�̬�����ķ�ʽ�����ڶ�̬����
       �ڴ�ķ�ʽ��Ҫע���������ռ���ͷš�
       
==============================================================================*/
static int CQuickTest_New(IShell       *ps,
                          IModule      *pIModule,
                          IQuickTest **ppObj)
{
   CQuickTest *cq;
   int        Retval = SUCCESS;

   cq = MALLOC(sizeof(CQuickTest));
   if (NULL == cq) {
      return ENOMEMORY;
   }

   MEMSET(cq,
          0,
          sizeof(CQuickTest));

   INIT_VTBL(cq,
             IQuickTest,
             gIQuickTestMethods);

   cq->m_nRefs     = 1;
   cq->m_pShell    = ps;
   cq->m_pModule   = pIModule;

   ISHELL_AddRef(ps);
   IMODULE_AddRef(pIModule);

   Retval = CQuickTest_InitData(cq);

   *ppObj = (IQuickTest*)cq;
   return Retval;
}

/*==============================================================================
����: 
       CQuickTest_AddRef
       
˵��: 
       CQuickTest Applet ���ü�����1�������ص�ǰ����������
       
����: 
       p [in]��IQuickTest�ӿڶ���ָ�롣
       
����ֵ:
       ��ǰ��QuickTest Applet��������
       
��ע:
       
==============================================================================*/
static uint32 CQuickTest_AddRef(IQuickTest *p)
{
   register CQuickTest *pMe = (CQuickTest*)p;

   return ++pMe->m_nRefs;
}

/*==============================================================================
����: 
       CQuickTest_Release
       
˵��: 
       �ͷŶ�QuickTest Applet�����ã����ü�����1�������ص�ǰ����������
        �����ǰ��������Ϊ�㣬�����ͷ�Applet�������Դ��
       
����: 
       p [in]��IQuickTest�ӿڶ���ָ�롣
       
����ֵ:
       ��ǰ��QuickTest Applet��������
       
��ע:
       
==============================================================================*/
static uint32 CQuickTest_Release(IQuickTest *p)
{
   register CQuickTest *pMe = (CQuickTest*)p;

   if (--pMe->m_nRefs) {
      return pMe->m_nRefs;
   }

   CQuickTest_FreeAppData(pMe);

   ISHELL_Release(pMe->m_pShell);
   IMODULE_Release(pMe->m_pModule);

   FREE(pMe);

   return 0;
}

/*==============================================================================
����: 
       CQuickTest_InitData
       
˵��: 
       ��ʼ��CQuickTest Applet�ṹ����ȱʡֵ��
       
����: 
       pMe [in]��ָ��CQuickTest Applet����ṹ��ָ�롣�ýṹ����С������ض���Ϣ��
       
����ֵ:
       SUCCESS����ʼ���ɹ���
       EFAILED����ʼ��ʧ�ܡ�
       
��ע:
       
==============================================================================*/
static int CQuickTest_InitData(CQuickTest *pMe)
{
   AEEDeviceInfo  di;
   
   if(pMe == NULL)
   {
      return EFAILED;
   }
   
   ISHELL_GetDeviceInfo(pMe->m_pShell, &di);

   pMe->m_rc.x       = 0;
   pMe->m_rc.y       = 0;
   pMe->m_rc.dx      = (int16) di.cxScreen;
   pMe->m_rc.dy      = (int16) di.cyScreen;

   pMe->m_bAppIsReady = FALSE;
   pMe->m_ePreState = QUICKTESTST_NONE;
   pMe->m_eCurState = QUICKTESTST_INIT;
   pMe->m_pActiveDlg = NULL;
   pMe->m_pActiveDlgID = IDD_MAINMENU;
   pMe->m_eDlgRet = DLGRET_CREATE;
   pMe->m_bNotOverwriteDlgRet = FALSE;
   pMe->m_mainMenuSel = 0;
#ifdef FEATURE_BREW_CAMERA    
   pMe->m_isFormCamera = FALSE;
#endif
   pMe->m_quicktestmic = FALSE;
   pMe->m_testkeycount = 0;
   if (AEE_SUCCESS != ISHELL_CreateInstance(pMe->m_pShell,
                                            AEECLSID_RINGERMGR,
                                            (void **)&pMe->m_pRingerMgr)) 
   {
       CQuickTest_FreeAppData(pMe);
       return EFAILED;
   } 

   if (AEE_SUCCESS != ISHELL_CreateInstance(pMe->m_pShell,
                                            AEECLSID_SOUND,
                                            (void **)&pMe->m_pISound))
   {
       CQuickTest_FreeAppData(pMe);
       return EFAILED;
   }

   if (AEE_SUCCESS != ISHELL_CreateInstance(pMe->m_pShell,
                                            AEECLSID_ALERT,
                                            (void **)&pMe->m_pIAlert))
   {
       CQuickTest_FreeAppData(pMe);
       return EFAILED;
   }

   if (AEE_SUCCESS != ISHELL_CreateInstance(pMe->m_pShell,
                                            AEECLSID_CONFIG,
                                            (void **)&pMe->m_pConfig))
   {
       CQuickTest_FreeAppData(pMe);
       return EFAILED;
   }

   if (AEE_SUCCESS != ISHELL_CreateInstance(pMe->m_pShell,
                                            AEECLSID_BACKLIGHT,
                                            (void **)&pMe->m_pIBacklight))
   {
       CQuickTest_FreeAppData(pMe);
       return EFAILED;
   }   
   
    if (ISHELL_CreateInstance(pMe->m_pShell,
                                 AEECLSID_ANNUNCIATOR,
                                 (void **) &pMe->m_pIAnn))
    {
        CQuickTest_FreeAppData(pMe);
        return FALSE;
    }

   if (AEE_SUCCESS != ISHELL_CreateInstance(pMe->m_pShell,
                                            AEECLSID_RUIM,
                                            (void **)&pMe->m_pIRUIM))
   {
       CQuickTest_FreeAppData(pMe);
       return EFAILED;
   }
    
   (void) ICONFIG_GetItem(pMe->m_pConfig,
                           CFGI_BACKLIGHT_LEVEL,
                           &pMe->m_nlightlevel,
                           sizeof(pMe->m_nlightlevel));
   pMe->colorMask    = 0;

   return SUCCESS;
}

/*==============================================================================
����: 
       CQuickTest_FreeAppData
       
˵��: 
       �ͷ�CQuickTest Appletʹ�õ������Դ��
       
����: 
       pMe [in]��ָ��CQuickTest Applet����ṹ��ָ�롣�ýṹ����С������ض���Ϣ��
       
����ֵ:
       �ޡ�
       
��ע:
       
==============================================================================*/
static void CQuickTest_FreeAppData(CQuickTest *pMe)
{
    if (NULL == pMe)
    {
        return ;
    }

    if (pMe->m_pISound) 
    {
        ISOUND_Release(pMe->m_pISound);
        pMe->m_pISound = NULL;
    }
    
    if (pMe->m_pRingerMgr)
    {
        IRINGERMGR_Release(pMe->m_pRingerMgr);
        pMe->m_pRingerMgr = NULL;
    }
 
    if (pMe->m_pIAlert)
    {
        IALERT_Release(pMe->m_pIAlert);
        pMe->m_pIAlert = NULL;
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

    
    if (pMe->m_pIAnn) 
    {
        IANNUNCIATOR_Release(pMe->m_pIAnn);
        pMe->m_pIAnn= NULL;
    }

    if (pMe->m_pIRUIM != NULL)
    {
        IRUIM_Release(pMe->m_pIRUIM);
        pMe->m_pIRUIM = NULL;
    }
}

/*==============================================================================
����: 
       CQuickTest_RunFSM
       
˵��: 
       CQuickTest Applet����״̬�����档
       
����: 
       pMe [in]��ָ��CQuickTest Applet����ṹ��ָ�롣�ýṹ����С������ض���Ϣ��
       
����ֵ:
       �ޡ�
       
��ע:
       
==============================================================================*/
static void CQuickTest_RunFSM(CQuickTest *pMe)
{
    NextFSMAction nextFSMAction = NFSMACTION_WAIT;

    for ( ; ; )
    {
        nextFSMAction = QuickTest_ProcessState(pMe);

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
       CQuickTest_HandleEvent
       
˵��: 
       CQuickTest  Applet�¼��������������������ڱ�Applet���¼���
       �������ɸú�������
       
����: 
       pi [in]��ָ��IQuickTest�ӿ�t�����ָ�롣
       eCode��BREW�¼����롣
       wParam�����¼�����������ݡ�
       dwParam [in]�����¼�����������ݡ�
       
����ֵ:
       TRUE�������¼�������
       FALSE�������¼�û������
       
��ע:
       
==============================================================================*/
static boolean CQuickTest_HandleEvent(IQuickTest *pi,
    AEEEvent eCode,
    uint16  wParam,
    uint32 dwParam
)
{
    CQuickTest *pMe = (CQuickTest*)pi;
    AEEAppStart *as;

    //MSG_FATAL("***zzg CQuickTest_HandleEvent eCode=%x***", eCode, 0, 0);
    
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
			if(STRCMP(as->pszArgs,"*#39#") == 0)
            {
            	 pMe->m_quicktestmic  = TRUE;
                 MOVE_TO_STATE(QUICKTESTST_CALLTEST)
            }
            pMe->m_lineheight = IDISPLAY_GetFontMetrics(pMe->m_pDisplay, AEE_FONT_BOLD, NULL, NULL);

            pMe->m_bSuspending = FALSE;

            // ��ʼCQuickTest״̬��
            CQuickTest_RunFSM(pMe);
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
#ifdef FEATURE_BREW_CAMERA             
            if(pMe->m_isFormCamera)
            {
                MOVE_TO_STATE(QUICKTESTST_SDTEST)
                pMe->m_isFormCamera = FALSE;
            }
#endif            
            CQuickTest_RunFSM(pMe);
            return TRUE;

        case EVT_DIALOG_INIT:
            pMe->m_bAppIsReady = FALSE;
            QuickTest_UpdateActiveDialogInfo(pMe, wParam, dwParam);

            return QuickTest_RouteDialogEvent(pMe,eCode,wParam,dwParam);

        case EVT_DIALOG_START:
             (void)ISHELL_SetTimer ( pMe->m_pShell,
                            APPISREADY_TIMER,
                            QuickTest_APPIsReadyTimer,
                            pMe);
             
            return QuickTest_RouteDialogEvent(pMe,eCode,wParam,dwParam);

        case EVT_USER_REDRAW:
            MSG_FATAL("CQuickTest_HandleEvent EVT_USER_REDRAW",0,0,0);
            (void) QuickTest_RouteDialogEvent(pMe,eCode,wParam,dwParam);
            /*
            (void)ISHELL_SetTimer ( pMe->m_pShell,
                            APPISREADY_TIMER,
                            QuickTest_APPIsReadyTimer,
                            pMe);
            */
            return TRUE;

        case EVT_APPISREADY:
            (void) QuickTest_RouteDialogEvent(pMe,eCode,wParam,dwParam);
            pMe->m_bAppIsReady = TRUE;
            return TRUE;

        case EVT_KEY_PRESS:
			#if !defined(FEATURE_VERSION_K212) && !defined(FEATURE_VERSION_EC99) && !defined(FEATURE_QVGA_INHERIT_K212)
            if (!pMe->m_bAppIsReady)
            {
                return TRUE;
            }
			#endif
            MSG_FATAL("CQuickTest_HandleEvent EVT_KEY_PRESS",0,0,0);
            return QuickTest_RouteDialogEvent(pMe,eCode,wParam,dwParam);

        case EVT_KEY_RELEASE:
			#if !defined(FEATURE_VERSION_K212) && !defined(FEATURE_VERSION_EC99)&&!defined(FEATURE_QVGA_INHERIT_K212)
            if (!pMe->m_bAppIsReady)
            {
                return TRUE;
            }
			#endif
            MSG_FATAL("CQuickTest_HandleEvent EVT_KEY_RELEASE",0,0,0);
            return QuickTest_RouteDialogEvent(pMe,eCode,wParam,dwParam);

        case EVT_KEY:
			#if !defined(FEATURE_VERSION_K212) && !defined(FEATURE_VERSION_EC99)&&!defined(FEATURE_QVGA_INHERIT_K212)
            if (!pMe->m_bAppIsReady)
            {
            	MSG_FATAL("CQuickTest_HandleEvent EVT_KEY",0,0,0);
                return TRUE;
            }
			#endif
            MSG_FATAL("CQuickTest_HandleEvent EVT_KEY",0,0,0);
            return QuickTest_RouteDialogEvent(pMe,eCode,wParam,dwParam);

        case EVT_COMMAND:
			#if !defined(FEATURE_VERSION_K212) && !defined(FEATURE_VERSION_EC99)&&!defined(FEATURE_QVGA_INHERIT_K212)
            if (!pMe->m_bAppIsReady)
            {
                return TRUE;
            }
			#endif
            MSG_FATAL("CQuickTest_HandleEvent EVT_COMMAND",0,0,0);
            return QuickTest_RouteDialogEvent(pMe,eCode,wParam,dwParam);

        case EVT_DIALOG_END:
			
            if (wParam == 0)
            {
                return TRUE;
            }
            pMe->m_bAppIsReady = FALSE;
            MSG_FATAL("CQuickTest_HandleEvent EVT_DIALOG_END",0,0,0);
            (void) QuickTest_RouteDialogEvent(pMe,eCode,wParam,dwParam);
            pMe->m_pActiveDlg = NULL;

            // Applet�����𣬲���״̬�����ȴ�Applet���ء���ע�⣺EVT_APP_SUSPEND
            // �¼���EVT_DIALOG_END�¼�ǰ������
            if (pMe->m_bSuspending == FALSE)
            {
                // ��ʼCQuickTest״̬��
                CQuickTest_RunFSM(pMe);
            }
            return TRUE;

        default:
            // �����յ����¼�·������ǰ��ĶԻ����¼���������
            return QuickTest_RouteDialogEvent(pMe,eCode,wParam,dwParam);
    }
}

/*==============================================================================
����: 
       QuickTest_APPIsReadyTimer
       
˵��: 
       ��ʱ���ص���������Ҫ���ڿ��ƿ��ٰ�������Ĵ���
       
����: 
       pme [in]��void *����ָ�룬��ָCQuickTest�ṹָ�롣
       
����ֵ:
       �ޡ�
       
��ע:
       
==============================================================================*/
static void QuickTest_APPIsReadyTimer(void *pme)
{
    CQuickTest *pMe = (CQuickTest *)pme;

    if (NULL == pMe)
    {
        return;
    }

    (void) ISHELL_PostEvent( pMe->m_pShell,
                            AEECLSID_QUICKTEST,
                            EVT_APPISREADY,
                            0,
                            0);
}
