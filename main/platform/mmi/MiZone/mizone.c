/*==============================================================================
// �ļ���
//        IMiZone.c
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
#include "Mizone_priv.h"

/*==============================================================================
                                 �궨��ͳ���
==============================================================================*/

/*==============================================================================
                                 ���Ͷ���
==============================================================================*/

/*==============================================================================
                                 ��������
==============================================================================*/

static void MiZone_APPIsReadyTimer(void *pme);
/*----------------------ģ����غ�������---------------------*/
#if defined(BREW_STATIC_APP)
                                    
int MiZone_Load(IShell   *pIShell, 
                             void     *ph, 
                             IModule **ppMod);

static int MiZone_CreateInstance( AEECLSID ClsId,
                                     IShell  *pIShell,
                                     IModule *po,
                                     void   **ppObj);
#endif

/*----------------------Applet��غ�������---------------------*/

/*----------------------Applet��غ�������---------------------*/
static int     CMiZone_New(IShell       *ps,
                              IModule      *pIModule,
                              IMiZone **ppObj);
static uint32  CMiZone_AddRef(IMiZone *pi);
static uint32  CMiZone_Release(IMiZone *pi);
static boolean CMiZone_HandleEvent(IMiZone *pi,
                                      AEEEvent    eCode,
                                      uint16      wParam,
                                      uint32      dwParam);
static int     CMiZone_InitData(CMiZone *pMe);
static void    CMiZone_FreeAppData(CMiZone *pMe);
static void    CMiZone_RunFSM(CMiZone *pMe);
/*==============================================================================
                                 ȫ������
==============================================================================*/

/*==============================================================================
                                 ���أ���̬������
==============================================================================*/

// ֻ����һ��CMiZoneʵ����ÿ�δ���CMiZone Appletʱ��
// ����ͬһ�ṹָ���BREW�㡣
//static CMiZone gMiZone;

static const VTBL(IMiZone) gIMiZoneMethods =
{
   CMiZone_AddRef,
   CMiZone_Release,
   CMiZone_HandleEvent
};

/*----------------------ģ����غ�������---------------------*/

#if defined(BREW_STATIC_APP)

/*==============================================================================
����: 
       MiZone_Load
       
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
int MiZone_Load(IShell   *pIShell,
                             void     *ph,
                             IModule **ppMod)
{
   return AEEStaticMod_New(sizeof(AEEMod),
                           pIShell,
                           ph,
                           ppMod,
                           (PFNMODCREATEINST)MiZone_CreateInstance,
                           (PFNFREEMODDATA)NULL);
}
#endif
/*==============================================================================
����: 
       MiZone_CreateInstance
       
˵��: 
        ͨ������(��ʼ��)MiZone����������ģ��ʵ����
       
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
static int MiZone_CreateInstance( AEECLSID ClsId,
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

   if (ClsId != AEECLSID_MiZone) {
      return EFAILED;
   }
   if (CMiZone_New(pIShell, po, (IMiZone**)ppObj) != SUCCESS) {
      return EFAILED;
   }
   return SUCCESS;
}
/*----------------------Applet��غ�������---------------------*/
/*==============================================================================
����: 
       CMiZone_New
       
˵��: 
        CMiZone Applet�������ڲ����ݱ�����ͬʱ�����ü�����1��
       
����: 
       ps [in]��IShell�ӿڶ���ָ�롣
       pIModule [in]��pIModule�ӿڶ���ָ�롣
       ppObj [out]:��ָ��IMiZone����ָ���ָ�롣
       
����ֵ:
       EFAILED��ʧ�ܡ�
       SUCCESS���ɹ���
       
��ע:
       �����û�ȥ���á�
       ����Applet�ṹ�����ʹ�ö�̬�����ڴ�ķ�ʽ�������������õ���ʹ�þ�̬
       �����ķ�ʽ������Խṹ�Ƚϴ��Appletʹ�þ�̬�����ķ�ʽ�����ڶ�̬����
       �ڴ�ķ�ʽ��Ҫע���������ռ���ͷš�
       
==============================================================================*/
static int CMiZone_New(IShell       *ps,
                          IModule      *pIModule,
                          IMiZone **ppObj)
{
   CMiZone *cq;
   int        Retval = SUCCESS;
   MSG_FATAL("***CMiZone_New****",0,0,0);
   cq = MALLOC(sizeof(CMiZone));
   if (NULL == cq) {
      return ENOMEMORY;
   }

   MEMSET(cq,
          0,
          sizeof(CMiZone));

   INIT_VTBL(cq,
             IMiZone,
             gIMiZoneMethods);

   cq->m_nRefs     = 1;
   cq->m_pShell    = ps;
   cq->m_pModule   = pIModule;

   ISHELL_AddRef(ps);
   IMODULE_AddRef(pIModule);

   Retval = CMiZone_InitData(cq);

   *ppObj = (IMiZone*)cq;
   return Retval;
}

/*==============================================================================
����: 
       CMiZone_AddRef
       
˵��: 
       CMiZone Applet ���ü�����1�������ص�ǰ����������
       
����: 
       p [in]��IMiZone�ӿڶ���ָ�롣
       
����ֵ:
       ��ǰ��MiZone Applet��������
       
��ע:
       
==============================================================================*/
static uint32 CMiZone_AddRef(IMiZone *p)
{
   register CMiZone *pMe = (CMiZone*)p;

   return ++pMe->m_nRefs;
}

/*==============================================================================
����: 
       CMiZone_Release
       
˵��: 
       �ͷŶ�MiZone Applet�����ã����ü�����1�������ص�ǰ����������
        �����ǰ��������Ϊ�㣬�����ͷ�Applet�������Դ��
       
����: 
       p [in]��IMiZone�ӿڶ���ָ�롣
       
����ֵ:
       ��ǰ��MiZone Applet��������
       
��ע:
       
==============================================================================*/
static uint32 CMiZone_Release(IMiZone *p)
{
   register CMiZone *pMe = (CMiZone*)p;

   if (--pMe->m_nRefs) {
      return pMe->m_nRefs;
   }

   CMiZone_FreeAppData(pMe);

   ISHELL_Release(pMe->m_pShell);
   IMODULE_Release(pMe->m_pModule);

   FREE(pMe);

   return 0;
}

/*==============================================================================
����: 
       CMiZone_InitData
       
˵��: 
       ��ʼ��CMiZone Applet�ṹ����ȱʡֵ��
       
����: 
       pMe [in]��ָ��CMiZone Applet����ṹ��ָ�롣�ýṹ����С������ض���Ϣ��
       
����ֵ:
       SUCCESS����ʼ���ɹ���
       EFAILED����ʼ��ʧ�ܡ�
       
��ע:
       
==============================================================================*/
static int CMiZone_InitData(CMiZone *pMe)
{
   AEEDeviceInfo  di;
   
   if(pMe == NULL)
   {
      return EFAILED;
   }
   MSG_FATAL("***CMiZone_InitData****1",0,0,0);
   ISHELL_GetDeviceInfo(pMe->m_pShell, &di);

   pMe->m_rc.x       = 0;
   pMe->m_rc.y       = 0;
   pMe->m_rc.dx      = (int16) di.cxScreen;
   pMe->m_rc.dy      = (int16) di.cyScreen;

   pMe->m_bAppIsReady = FALSE;
   pMe->m_ePreState = MIZONE_NONE;
   pMe->m_eCurState = MIZONE_MAIN;
   pMe->m_pActiveDlg = NULL;
   pMe->m_pActiveDlgID = IDD_MAINMENU;
   pMe->m_eDlgRet = DLGRET_CREATE;
   pMe->m_bNotOverwriteDlgRet = FALSE;
   pMe->m_mainMenuSel = 0;
   
    if(AEE_SUCCESS != ISHELL_CreateInstance(pMe->m_pShell,
                                 AEECLSID_ANNUNCIATOR,
                                (void **) &pMe->m_pIAnn))
    {
       MSG_FATAL("***CMiZone_InitData****2",0,0,0);
       CMiZone_FreeAppData(pMe);
       return FALSE;
    }
    if(ISHELL_CreateInstance(pMe->m_pShell, AEECLSID_STATIC,
                                (void **)&pMe->m_pStatic) != SUCCESS)
    {
        DBGPRINTF("The creation of static interface has failed!" );
        return FALSE;
    }  
   if (AEE_SUCCESS != ISHELL_CreateInstance(pMe->m_pShell,AEECLSID_CONFIG,
                                            (void **)&pMe->m_pConfig))
    {
       return FALSE;
    }
   if (AEE_SUCCESS != ISHELL_CreateInstance(pMe->m_pShell,AEECLSID_RUIM,
                                            (void **)&pMe->m_pIRUIM))
    {
       return FALSE;
    }
     MSG_FATAL("***CMiZone_InitData****pMe->m_eCurState=%d",pMe->m_eCurState,0,0);
   return SUCCESS;
}

/*==============================================================================
����: 
       CMiZone_FreeAppData
       
˵��: 
       �ͷ�CMiZone Appletʹ�õ������Դ��
       
����: 
       pMe [in]��ָ��CMiZone Applet����ṹ��ָ�롣�ýṹ����С������ض���Ϣ��
       
����ֵ:
       �ޡ�
       
��ע:
       
==============================================================================*/
static void CMiZone_FreeAppData(CMiZone *pMe)
{
    if (NULL == pMe)
    {
        return ;
    }
    MSG_FATAL("***CMiZone_FreeAppData****",0,0,0);

    if (pMe->m_pIAnn) 
    {
        IANNUNCIATOR_Release(pMe->m_pIAnn);
        pMe->m_pIAnn= NULL;
    }
    if(pMe->m_pStatic != NULL)
    {
        ISTATIC_Release(pMe->m_pStatic);
        pMe->m_pStatic = NULL; 
    }
    if (pMe->m_pConfig)
    {
        ICONFIG_Release(pMe->m_pConfig);
        pMe->m_pConfig = NULL;
    }
    if (pMe->m_pIRUIM != NULL)
    {
        IRUIM_Release(pMe->m_pIRUIM);
        pMe->m_pIRUIM = NULL;
    }
}

/*==============================================================================
����: 
       CMiZone_RunFSM
       
˵��: 
       CMiZone Applet����״̬�����档
       
����: 
       pMe [in]��ָ��CMiZone Applet����ṹ��ָ�롣�ýṹ����С������ض���Ϣ��
       
����ֵ:
       �ޡ�
       
��ע:
       
==============================================================================*/
static void CMiZone_RunFSM(CMiZone *pMe)
{
    NextFSMAction nextFSMAction = NFSMACTION_WAIT;

    for ( ; ; )
    {
        nextFSMAction = MiZone_ProcessState(pMe);

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
       CMiZone_HandleEvent
       
˵��: 
       CMiZone  Applet�¼��������������������ڱ�Applet���¼���
       �������ɸú�������
       
����: 
       pi [in]��ָ��IMiZone�ӿ�t�����ָ�롣
       eCode��BREW�¼����롣
       wParam�����¼�����������ݡ�
       dwParam [in]�����¼�����������ݡ�
       
����ֵ:
       TRUE�������¼�������
       FALSE�������¼�û������
       
��ע:
       
==============================================================================*/
static boolean CMiZone_HandleEvent(IMiZone *pi,
    AEEEvent eCode,
    uint16  wParam,
    uint32 dwParam
)
{
    CMiZone *pMe = (CMiZone*)pi;
    AEEAppStart *as;
    MSG_FATAL("***CMiZone_HandleEvent****eCode=%d",eCode,0,0);
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
            pMe->m_lineheight = IDISPLAY_GetFontMetrics(pMe->m_pDisplay, AEE_FONT_BOLD, NULL, NULL);

            pMe->m_bSuspending = FALSE;

            // ��ʼCMiZone״̬��
            CMiZone_RunFSM(pMe);
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
            CMiZone_RunFSM(pMe);
            return TRUE;

        case EVT_DIALOG_INIT:
            pMe->m_bAppIsReady = FALSE;
            MiZone_UpdateActiveDialogInfo(pMe, wParam, dwParam);

            return MiZone_RouteDialogEvent(pMe,eCode,wParam,dwParam);

        case EVT_DIALOG_START:
            (void)ISHELL_SetTimer ( pMe->m_pShell,
                            APPISREADY_TIMER,
                            MiZone_APPIsReadyTimer,
                            pMe);
            return MiZone_RouteDialogEvent(pMe,eCode,wParam,dwParam);

        case EVT_USER_REDRAW:
            MSG_FATAL("CMiZone_HandleEvent EVT_USER_REDRAW",0,0,0);
            (void) MiZone_RouteDialogEvent(pMe,eCode,wParam,dwParam);
            /*
            (void)ISHELL_SetTimer ( pMe->m_pShell,
                            APPISREADY_TIMER,
                            MiZone_APPIsReadyTimer,
                            pMe);
            */
            return TRUE;

        case EVT_APPISREADY:
            (void) MiZone_RouteDialogEvent(pMe,eCode,wParam,dwParam);
            pMe->m_bAppIsReady = TRUE;
            return TRUE;

        case EVT_KEY_PRESS:
			#if !defined(FEATURE_VERSION_K212) && !defined(FEATURE_VERSION_EC99)&&!defined(FEATURE_QVGA_INHERIT_K212)&& !defined(FEATURE_LOW_MEM_BIGFONT)&&!defined(FEATURE_VERSION_IN50A)
            if (!pMe->m_bAppIsReady)
            {
                return TRUE;
            }
			#endif
            MSG_FATAL("CMiZone_HandleEvent EVT_KEY_PRESS",0,0,0);
            return MiZone_RouteDialogEvent(pMe,eCode,wParam,dwParam);

        case EVT_KEY_RELEASE:
			#if !defined(FEATURE_VERSION_K212) && !defined(FEATURE_VERSION_EC99)&&!defined(FEATURE_QVGA_INHERIT_K212)&& !defined(FEATURE_LOW_MEM_BIGFONT)&&!defined(FEATURE_VERSION_IN50A)
            if (!pMe->m_bAppIsReady)
            {
                return TRUE;
            }
			#endif
            MSG_FATAL("CMiZone_HandleEvent EVT_KEY_RELEASE",0,0,0);
            return MiZone_RouteDialogEvent(pMe,eCode,wParam,dwParam);

        case EVT_KEY:
			#if !defined(FEATURE_VERSION_K212) && !defined(FEATURE_VERSION_EC99)&&!defined(FEATURE_QVGA_INHERIT_K212)&& !defined(FEATURE_LOW_MEM_BIGFONT)&&!defined(FEATURE_VERSION_IN50A)
            if (!pMe->m_bAppIsReady)
            {
            	MSG_FATAL("CMiZone_HandleEvent EVT_KEY",0,0,0);
                return TRUE;
            }
			#endif
            MSG_FATAL("CMiZone_HandleEvent EVT_KEY",0,0,0);
            return MiZone_RouteDialogEvent(pMe,eCode,wParam,dwParam);

        case EVT_COMMAND:
			#if !defined(FEATURE_VERSION_K212) && !defined(FEATURE_VERSION_EC99)&&!defined(FEATURE_QVGA_INHERIT_K212)&& !defined(FEATURE_LOW_MEM_BIGFONT)&&!defined(FEATURE_VERSION_IN50A)
            if (!pMe->m_bAppIsReady)
            {
                return TRUE;
            }
			#endif
            MSG_FATAL("CMiZone_HandleEvent EVT_COMMAND",0,0,0);
            return MiZone_RouteDialogEvent(pMe,eCode,wParam,dwParam);

        case EVT_DIALOG_END:
			
            if (wParam == 0)
            {
                return TRUE;
            }
            pMe->m_bAppIsReady = FALSE;
            MSG_FATAL("CMiZone_HandleEvent EVT_DIALOG_END",0,0,0);
            (void) MiZone_RouteDialogEvent(pMe,eCode,wParam,dwParam);
            pMe->m_pActiveDlg = NULL;

            // Applet�����𣬲���״̬�����ȴ�Applet���ء���ע�⣺EVT_APP_SUSPEND
            // �¼���EVT_DIALOG_END�¼�ǰ������
            if (pMe->m_bSuspending == FALSE)
            {
                // ��ʼCMiZone״̬��
                CMiZone_RunFSM(pMe);
            }
            return TRUE;

        default:
            // �����յ����¼�·������ǰ��ĶԻ����¼���������
            return MiZone_RouteDialogEvent(pMe,eCode,wParam,dwParam);
    }
}

/*==============================================================================
����: 
       MiZone_APPIsReadyTimer
       
˵��: 
       ��ʱ���ص���������Ҫ���ڿ��ƿ��ٰ�������Ĵ���
       
����: 
       pme [in]��void *����ָ�룬��ָCMiZone�ṹָ�롣
       
����ֵ:
       �ޡ�
       
��ע:
       
==============================================================================*/
static void MiZone_APPIsReadyTimer(void *pme)
{
    CMiZone *pMe = (CMiZone *)pme;

    if (NULL == pMe)
    {
        return;
    }

    (void) ISHELL_PostEvent( pMe->m_pShell,
                            AEECLSID_MiZone,
                            EVT_APPISREADY,
                            0,
                            0);
}
