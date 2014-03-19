/*==============================================================================
//           W M S   A P P L E T  M O D U L E
// �ļ�:
//        WMSApp.c
//        ��Ȩ����(c) 2009 Anylook Tech. CO.,LTD.
//        
// �ļ�˵����
//        ����Ϣģ�����ļ�
//                 
==============================================================================*/

/*==============================================================================
�޸���ʷ��¼��
ʱ       ��       �� �� ��     �޸����ݡ�λ�ü�ԭ��
-----------      ----------    -----------------------------------------------

==============================================================================*/


/*==============================================================================
                             
                             ���ļ��������ⲿ�ļ�
                             
==============================================================================*/
#include "WmsApp_priv.h"      // ��Appletͷ�ļ�
#include "WMSUtil.h"          // Applet����ģ��ͷ�ļ�
#include "OEMClassIDs.h"
#ifdef FEATURE_USES_MMS
#include "WMSMmsTest.h"
#include "WMSMms.h"
#endif
/*==============================================================================
                                 
                                 �궨��ͳ���
                                 
==============================================================================*/


/*==============================================================================
                                 
                                ����������Ͷ���
                                 
==============================================================================*/


/*==============================================================================

                              ���غ�������

==============================================================================*/
int CWMSMod_Load(IShell  *pIShell, void  *ph, IModule **ppMod);

int CWMSMod_New(int16               nSize, 
                IShell              *pIShell,
                void                *ph,   
                IModule             **ppMod,
                PFNMODCREATEINST    pfnMC, 
                PFNFREEMODDATA      pfnMF);

static uint32 CWMSMod_AddRef(IModule *po);
static uint32 CWMSMod_Release(IModule *po);
static int CWMSMod_CreateInstance(IModule *po, 
                                  IShell  *pIShell,
                                  AEECLSID ClsId, 
                                  void   **ppObj);

static void CWMSMod_FreeResources(IModule  *po, IHeap    *ph, IFileMgr *pfm);

// Applet�������ͷź���
static int CWmsApp_New(IShell *ps, IModule *pIModule, IWmsApp **ppObj);

static uint32  CWmsApp_AddRef  (IWmsApp *pi);
static uint32  CWmsApp_Release (IWmsApp *pi);

// �¼�������
static boolean CWmsApp_HandleEvent(IWmsApp  *pi, 
                                   AEEEvent eCode,
                                   uint16   wParam, 
                                   uint32   dwParam);

static int CWmsApp_MessageService(IWmsApp *p, 
                                  WMSServiceType eStype, 
                                  AECHAR *toAddrStr, 
                                  AECHAR *pwstrText);
static int CWmsApp_GetLeftSendListNodeNum(IWmsApp *p, int *leftNum);
static int CWmsApp_DeleteAllNvCdmaSms(IWmsApp *p);

// �ڲ�Applet��������
static int CWmsApp_InitAppData(WmsApp *pMe);
static void CWmsApp_FreeAppData(WmsApp *pMe);

// Applet��״̬��������
static void CWmsApp_RunFSM(WmsApp *pMe);

// ��������֪ͨ�¼��Ĵ�����
static boolean WmsApp_HandleNotifyEvent(WmsApp *pMe, AEENotify *pN);

// �������� AEECLSID_CM_NOTIFIER ֪ͨ�¼��ĺ���
static boolean WmsApp_HandleCMNotify(WmsApp * pMe, AEENotify *pNotify);

// ��ȡȺ����Ϣ����
static IVector * WmsApp_GetSendList(void);

static IVector * WmsApp_GetUserDataMOList(void);


static char * CWmsApp_BuildStartArgs(WMSServiceType eStype);

static void WmsApp_ArgsStartInit(WmsApp *pMe, const char *pszArgs);

void WmsApp_CfgCb(wms_cfg_event_e_type event, wms_cfg_event_info_s_type *pInfo);

void WmsApp_MsgCb(wms_msg_event_e_type       event,
                  wms_msg_event_info_s_type  *pInfo,
                  boolean                    *shared);

#ifdef FEATURE_BROADCAST_SMS
void WmsApp_BcCb(wms_bc_event_e_type  event, wms_bc_event_info_s_type  *pInfo);
#endif

#ifdef  FEATURE_CDSMS
void WmsApp_DcCb(wms_dc_event_e_type  event,wms_dc_event_info_s_type  *info_ptr);
#endif

static void WmsApp_Init(WmsApp *pMe);

static void WmsApp_UpdateMemoryStatus(WmsApp *pMe, wms_memory_status_s_type *ptr);

static uint16 WmsApp_GetMsgICONID(wms_cache_info_node * pNode);

static void WmsApp_ReservedMsgTimer(void * pUser);
#ifdef MMS_TEST
void WmsApp_debug_CheckChariFromRawDaTa(WmsApp *pMe);
#endif
/*==============================================================================
                              ȫ������
==============================================================================*/
// ��Ҫ��ʾ::
//        g_dwAEEStdLibEntryȫ�ֱ���������SDK !
//        BREW SDK�û����ܸ�д��ֱ��ʹ�ñ�������������AEEStdLib��һ����ڵ㣬��
//        Ҫ�ƶ��������д��롣
#ifdef AEE_SIMULATOR
   uint32 g_dwAEEStdLibEntry;
#ifndef WIN32
   #error The WMS applet was never intended to be used with the AEE Simulator.
#endif
#endif   

#ifdef FEATURE_USES_MMS
//MMSData		                   g_mmsDataInfoList[MAX_MMS_STORED];
//MMSData		                   g_mmsDataInfoList[2];
uint8                          g_mmsDataInfoMax = 0;
#endif
/*==============================================================================
                              
                              ���أ���̬������
                              
==============================================================================*/
static CWMSMod gWMSMod;

static const VTBL(IModule) gModFuncs =
{
   CWMSMod_AddRef,
   CWMSMod_Release,
   CWMSMod_CreateInstance,
   CWMSMod_FreeResources
};

// ֻ����һ�� WmsApp ʵ����ÿ�δ��� WMS Applet ʱ������ͬһ�ṹָ��� BREW �㡣
static WmsApp gWmsApp={0};
boolean gbWmsVMailNtf = FALSE;
boolean gbWmsSMSNtf   = FALSE;
boolean gbWmsMMSNtf = FALSE;
boolean gbWmsLastNtfIsSMS = FALSE;

static const VTBL(IWmsApp) gWmsAppMethods =
{
   CWmsApp_AddRef,
   CWmsApp_Release,
   CWmsApp_HandleEvent,
   CWmsApp_MessageService,
   CWmsApp_GetLeftSendListNodeNum,
   CWmsApp_DeleteAllNvCdmaSms
};

/*==============================================================================
����:
    CWMSMod_Load

˵��:
    ģ��װ�غ�����

����:
    pIShell [in]:IShell�ӿ�ָ�롣
    ph :û��ʹ��
    ppMod [out]:

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
int CWMSMod_Load(IShell   *pIShell,
                        void     *ph,
                        IModule **ppMod)
#endif
#else
int CWMSMod_Load(IShell   *pIShell,
                        void     *ph,
                        IModule **ppMod)
#endif
{
    // ���ڶ�̬Applet����֤AEE STD��汾����ȷ�����뽨����ģ��ʱʹ�õĿ�汾ƥ��
    // ����AEE_SIMULATOR�²��ؽ��б���顣
#if !defined(BREW_STATIC_APP) && !defined(AEE_SIMULATOR)
    if (GET_HELPER_VER() != AEESTDLIB_VER)
    {
        return EVERSIONNOTSUPPORT;
    }
#endif

    return CWMSMod_New(sizeof(CWMSMod), pIShell, ph, ppMod, NULL, NULL);
}

/*==============================================================================
����:
    CWMSMod_New

˵��:
    ������ʼ�� WMS ģ�������Ҳ�������ü����������ֺ�����

����:
    nSize: û��ʹ��
    pIShell [in]: IShell �ӿ�ָ�롣
    ph : û��ʹ��
    ppMod [out]:
    pfnMC: û��ʹ��
    pfnMF: û��ʹ��

����ֵ:
    int

��ע:

==============================================================================*/
int CWMSMod_New(int16           nSize,
                IShell          *pIShell,
                void            *ph,
                IModule         **ppMod,
                PFNMODCREATEINST pfnMC,
                PFNFREEMODDATA   pfnMF)
{
    if (!ppMod)
    {
        return EFAILED;
    }

    *ppMod = NULL;

    // ��Ҫ��ʾ:
    //        g_dwAEEStdLibEntryȫ�ֱ���������SDK !
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

    MEMSET(&gWMSMod,0,sizeof(CWMSMod));

    INIT_VTBL(&gWMSMod,IModule,gModFuncs);

    gWMSMod.m_nRefs = 1;

    *ppMod = (IModule *)&gWMSMod;

    return AEE_SUCCESS;
}

/*==============================================================================
����:
    CWMSMod_AddRef

˵��:
    WMS ģ�����ü�����1�������ص�ǰ����������

����:
    po [in/out]:IModule�ӿڶ���ָ�롣

����ֵ:
    ��ǰ��SounMenu ģ���������

��ע:

==============================================================================*/
static uint32 CWMSMod_AddRef(IModule *po)
{
    return(++((CWMSMod *)po)->m_nRefs);
}

/*==============================================================================
����:
    CWMSMod_Release

˵��:
    WMS ģ�����ü�����1�������ص�ǰ����������

����:
    po [in/out]:IModule�ӿڶ���ָ�롣

����ֵ:
    ��ǰ��SounMenu ģ���������

��ע:

==============================================================================*/
static uint32 CWMSMod_Release(IModule *po)
{
    if (((CWMSMod *) po)->m_nRefs == 0)
    {
        return 0;
    }

    return(--((CWMSMod *)po)->m_nRefs);
}

/*==============================================================================
����:
    CWMSMod_CreateInstance

˵��:
    ͨ������(��ʼ��)WMS����������ģ��ʵ����

����:
    po [in]:IModule�ӿڶ���ָ�롣
    pIShell [in]:IShell�ӿڶ���ָ�롣
    ClsId [in]:Ҫ������ʵ������ID��
    ppObj [out]::���ڱ������ʵ����ָ���ָ�������

����ֵ:
    none

��ע:
    �����û�ȥ���á�

==============================================================================*/
static int CWMSMod_CreateInstance(IModule   *po,
                                  IShell    *pIShell,
                                  AEECLSID  ClsId,
                                  void      **ppObj)
{
    *ppObj = NULL;

    if (ClsId != AEECLSID_WMSAPP)
    {
        return EFAILED;
    }

    if (CWmsApp_New(pIShell, po, (IWmsApp**)ppObj) != SUCCESS)
    {
        return EFAILED;
    }

    return SUCCESS;
}

/*==============================================================================
����:
    CWMSMod_FreeResources

˵��:
    �ͷ�WMSģ����Դ�ĺ�����

����:
    po :û��ʹ�á�
    IHeap :û��ʹ�á�
    pfm ::û��ʹ�á�

����ֵ:
    none

��ע:
    �����û�ȥ���á�

==============================================================================*/
static void CWMSMod_FreeResources(IModule *po, IHeap  *ph, IFileMgr *pfm)
{
    ;
}

/*==============================================================================
����:
    CWmsApp_New

˵��:
    ��ʼ�� WMS Applet �������ڲ����ݱ�����ͬʱ WMS Applet ���ü�����1��

����:
    ps [in]: IShell�ӿڶ���ָ�롣
    pIModule [in/out]: pIModule�ӿڶ���ָ�롣
    ppObj [out]: ָ�� IWmsApp ����ָ���ָ�롣

����ֵ:
    EFAILED: ʧ�ܡ�
    SUCCESS: �ɹ���
       
��ע:
    �����û�ȥ���á�
    ����Applet�ṹ�����ʹ�ö�̬�����ڴ�ķ�ʽ�������������õ���ʹ�þ�̬
    �����ķ�ʽ������Խṹ�Ƚϴ��Appletʹ�þ�̬�����ķ�ʽ�����ڶ�̬����
    �ڴ�ķ�ʽ��Ҫע���������ռ���ͷš�

==============================================================================*/
static int CWmsApp_New(IShell *ps, IModule *pIModule, IWmsApp **ppObj)
{
    int retVal = SUCCESS;

    if (0 == gWmsApp.m_nRefs)
    {
        char *pszArgs = gWmsApp.m_pszArgs;
        MEMSET(&gWmsApp, 0, sizeof(gWmsApp));
        gWmsApp.m_pszArgs = pszArgs;
        INIT_VTBL(&gWmsApp, IWmsApp, gWmsAppMethods);

        gWmsApp.m_nRefs     = 0;
        gWmsApp.m_pShell    = ps;
        gWmsApp.m_pModule   = pIModule;

        (void) ISHELL_AddRef(ps);
        (void) IMODULE_AddRef(pIModule);

        retVal = CWmsApp_InitAppData(&gWmsApp);
    }

    ++gWmsApp.m_nRefs;

    *ppObj = (IWmsApp*)&gWmsApp;

    return retVal;
}

/*==============================================================================
����:
    CWmsApp_AddRef

˵��:
    WMS Applet ���ü�����1�������ص�ǰ����������

����:
    pi [in/out]: IWmsApp �ӿڶ���ָ�롣

����ֵ:
    ��ǰ�� WMS Applet ��������

��ע:

==============================================================================*/
static uint32 CWmsApp_AddRef(IWmsApp *p)
{
    register WmsApp *pMe = (WmsApp*)p;

    return (++pMe->m_nRefs);
}

/*==============================================================================
����:
    CWmsApp_Release

˵��:
    �ͷŶ� WMS Applet �����ã����ü�����1�������ص�ǰ����������
    �����ǰ��������Ϊ�㣬�����ͷ� Applet �������Դ��

����:
    pi [in/out]: IWmsApp �ӿڶ���ָ�롣

����ֵ:
    ��ǰ�� WMS Applet ��������

��ע:

==============================================================================*/
static uint32 CWmsApp_Release(IWmsApp *p)
{
    register WmsApp *pMe = (WmsApp*)p;

    if (pMe->m_nRefs == 0)
    {
        return 0;
    }

    if (--pMe->m_nRefs)
    {
        return pMe->m_nRefs;
    }

    CWmsApp_FreeAppData(pMe);

    (void) ISHELL_Release(pMe->m_pShell);
    (void) IMODULE_Release(pMe->m_pModule);

    // ע��:pMe�Ǿ�̬����ռ䣬��˲���Ҫ�ͷš�FREE()
    return 0;
}

/*==============================================================================
����:
    CWmsApp_InitAppData

˵��:
    ��ʼ�� WMS Applet �ṹ����ȱʡֵ��

����:
    pMe [in]: ָ�� WMS Applet ����ṹ��ָ�롣�ýṹ����С������ض���Ϣ��

����ֵ:
    EFAILED: ��ʼ��ʧ�ܡ�
    SUCCESS: ��ʼ���ɹ���

��ע:

==============================================================================*/
static int CWmsApp_InitAppData(WmsApp *pMe)
{
    MSG_FATAL("CWmsApp_InitAppData Start",0,0,0);
    if (NULL == pMe)
    {
        return EFAILED;
    }
#ifdef FEATURE_USES_MMS    
    pMe->m_pMMSImage = NULL;
    pMe->m_pMMSSOUND = NULL;
    pMe->m_pMMSVIDEO = NULL;
    pMe->m_isMMS = FALSE;
    pMe->m_MMSData = NULL;
    MEMSET(&pMe->m_EncData,NULL,sizeof(MMS_WSP_ENCODE_SEND));
    MEMSET(&pMe->m_ResData,NULL,sizeof(WSP_MMS_RESOURCE));
    MEMSET(&pMe->m_DeliveryEncData,NULL,sizeof(MMS_WSP_ENCODE_SEND));
    pMe->m_pMedia = NULL;
    pMe->m_GetStatus = HTTP_CODE_OK;
    pMe->m_wSelectStore = 1;
    pMe->m_pMMSMenuHasFocus = FALSE;
    pMe->m_CurrentState = PLAYER_IDLE;
    pMe->m_isForward = FALSE;
    pMe->m_insertMMSType = 0;
    pMe->m_hasImage = FALSE;
    pMe->m_hasSound = FALSE;
    pMe->m_hasVideo = FALSE;    
#endif
    // ��ʼ������Ա����
    pMe->m_prevState = WMSST_NONE;
    pMe->m_currState = WMSST_MAIN;
    pMe->m_STSwitchToEditMsg = WMSST_NONE;
    pMe->m_eDlgReturn = DLGRET_CREATE;
    pMe->m_eAppStatus = WMSAPP_RUNNING;
    pMe->m_eMBoxType = WMS_MB_NONE;
    pMe->m_eCreateWMSType = SEND_MSG_NEW;
    pMe->m_SendStatus = WMS_RPT_OK;

    pMe->m_bNaturalStart = TRUE;
    pMe->m_CopyType = NONECOPY;
    pMe->m_eMakeListMode = MAKEMSGLIST_INIT;
    pMe->m_eInsertType = INSERT_NONE; 
    pMe->m_strPhonePWD = NULL;
    pMe->m_bincommend = FALSE;
    pMe->m_bwriteclr  = FALSE;
    #ifdef FEATURE_FLASH_SMS
	pMe->m_bflash_sms = FALSE;              //add by yangdecai 2011-04-01
	#endif
    pMe->m_bIsEsn = FALSE;                 
    // ȡ�����������Ϣ
    WmsApp_GetPrefs(pMe);
    if (pMe->m_cfg.nInitIndictor != WMS_CFG_VERSION) // ��ǰ�汾δ��ʼ��
    {
       // ���ó�ʼ�����
       pMe->m_cfg.nInitIndictor = WMS_CFG_VERSION;
       
       pMe->m_cfg.m_bAutoSave = TRUE;
       
       pMe->m_cfg.m_bAutoDelete = FALSE;
       
#ifdef FEATURE_SMS_UDH
       pMe->m_cfg.m_msg_ref = 1;
#endif    
    
       pMe->m_cfg.m_msgid = 1;
       // ��������
       WmsApp_StorePrefs(pMe);
    }
    
    // ��ȡȺ������
    pMe->m_pSendList = WmsApp_GetSendList();
    if (NULL == pMe->m_pSendList)
    {
        return EFAILED;
    }
    if( ISHELL_CreateInstance( pMe->m_pShell, AEECLSID_SOUND, (void**)&pMe->m_pSound) != SUCCESS)
    {
        return EFAILED;
    }
    // ��ȡ��Ϣ�û���������
    pMe->m_pUserDataMOList = WmsApp_GetUserDataMOList();
    if (NULL == pMe->m_pUserDataMOList)
    {
        return EFAILED;
    }
    
    // �ӿڴ�������س�ʼ��
    if (ISHELL_CreateInstance(pMe->m_pShell, AEECLSID_CONFIG,
            (void **)&pMe->m_pConfig) != SUCCESS)
    {
        CWmsApp_FreeAppData(pMe);
        return EFAILED;
    }

    if (ISHELL_CreateInstance(pMe->m_pShell, AEECLSID_VECTOR,
            (void**)&pMe->m_pSaveNumList) != SUCCESS )
    {
        pMe->m_pSaveNumList = NULL;
        CWmsApp_FreeAppData(pMe);
        return EFAILED;
    }
    (void) IVector_EnsureCapacity(pMe->m_pSaveNumList, 60);
    
    if (ISHELL_CreateInstance(pMe->m_pShell, AEECLSID_WMS, 
            (void **) &pMe->m_pwms) != SUCCESS)
    {
        return EFAILED;
    }
    
#ifdef FEATURE_ICM
    if (ISHELL_CreateInstance(pMe->m_pShell, AEECLSID_CALLMANAGER, 
            (void **) &pMe->m_pICM) != SUCCESS)
    {
        return EFAILED;
    }
#endif    

    if (ISHELL_CreateInstance(pMe->m_pShell, AEECLSID_ANNUNCIATOR, 
            (void **) &pMe->m_pIAnn) != SUCCESS)
    {
        return EFAILED;
    }
    IANNUNCIATOR_SetFieldIsActiveEx(pMe->m_pIAnn,FALSE);
    //IANNUNCIATOR_SetFieldIsActiveEx(pMe->m_pIAnn,FALSE);
    // Create IAlert instance
    if (ISHELL_CreateInstance(pMe->m_pShell, AEECLSID_ALERT, 
            (void **) &pMe->m_pAlert) != SUCCESS)
    {
        return EFAILED;
    }
    
    // Create IDisplay instance
    if (ISHELL_CreateInstance(pMe->m_pShell, AEECLSID_DISPLAY, 
            (void **) &pMe->m_pDisplay) != SUCCESS)
    {
        return EFAILED;
    }

    if( ISHELL_CreateInstance( pMe->m_pShell, AEECLSID_BACKLIGHT, (void **)&pMe->m_pBacklight)!=AEE_SUCCESS)
    {
        CWmsApp_FreeAppData(pMe);
        return EFAILED;
    }
	if (AEE_SUCCESS != ISHELL_CreateInstance(pMe->m_pShell,
                                            AEECLSID_RUIM,
                                            (void **)&pMe->m_pIRUIM))
	{
		CWmsApp_FreeAppData(pMe);
		return EFAILED;
	}
    CALLBACK_Init(&pMe->m_callback, 
                  (PFNNOTIFY) WmsApp_wmscommandreport,
                  (void*)pMe);
    
    // Register Callbacks
    
    (void)IWMS_Init(pMe->m_pwms, WMS_CLIENT_TYPE_WMS_APP, &pMe->m_clientId);
    (void)IWMS_RegisterCfgCb(pMe->m_pwms, pMe->m_clientId, WmsApp_CfgCb );
    (void)IWMS_RegisterMsgCb(pMe->m_pwms, pMe->m_clientId, WmsApp_MsgCb );

#ifdef FEATURE_BROADCAST_SMS
    (void)IWMS_RegisterBcCb(pMe->m_pwms, pMe->m_clientId, WmsApp_BcCb);
#endif

#ifdef FEATURE_CDSMS
    (void)IWMS_RegisterDcCb(pMe->m_pwms, pMe->m_clientId, WmsApp_DcCb);
#endif
#ifdef FEATURE_USES_MMS  
    ICONFIG_GetItem(pMe->m_pConfig,
        CFGI_WMS_MMSNOTIFY,
        &pMe->m_isCheckMMSNotify,
        sizeof(pMe->m_isCheckMMSNotify));
    ICONFIG_GetItem(pMe->m_pConfig,
        CFGI_WMS_MMSDELIVERYREPORT,
        &pMe->m_isMMSDeliveryReport,
        sizeof(pMe->m_isMMSDeliveryReport));
    ICONFIG_GetItem(pMe->m_pConfig,
        CFGI_WMS_READREPLY,
        &pMe->m_isMMSReadReply,
        sizeof(pMe->m_isMMSReadReply));
    ICONFIG_GetItem(pMe->m_pConfig,
        CFGI_WMS_REPORTALLOWED,
        &pMe->m_isMMSReporyAllowed,
        sizeof(pMe->m_isMMSReporyAllowed));
    ICONFIG_GetItem(pMe->m_pConfig,
        CFGI_WMS_SENDERVISIBILITY,
        &pMe->m_isMMSSenderVisibility,
        sizeof(pMe->m_isMMSSenderVisibility));
        pMe->m_Animate = NULL;
#endif

    (void)IWMS_Activate(pMe->m_pwms,pMe->m_clientId);
    MSG_FATAL("CWmsApp_InitAppData End",0,0,0);
    return SUCCESS;
}

/*==============================================================================
����:
    CWmsApp_FreeAppData

˵��:
    �ͷ� WMS Applet �����Դ��

����:
    pMe [in]: ָ�� WMS Applet ����ṹ��ָ�롣�ýṹ����С������ض���Ϣ��

����ֵ:
    none

��ע:

==============================================================================*/
static void CWmsApp_FreeAppData(WmsApp *pMe)
{
    int i = 0;
    MSG_FATAL("CWmsApp_FreeAppData Start",0,0,0);
    
    if (NULL == pMe)
    {
        return;
    }
#ifdef FEATURE_USES_MMS
    if(pMe->m_pMMSImage != NULL)
    {
        char MMSImageName[MG_MAX_FILE_NAME]={'/0'};
        MSG_FATAL("pMe->m_pMMSImage != NULL",0,0,0);
        ICONFIG_SetItem(pMe->m_pConfig, CFGI_MMSIMAGE,MMSImageName, sizeof(MMSImageName));       
        RELEASEIF(pMe->m_pMMSImage);
    }
    if(pMe->m_pMMSSOUND!= NULL)
    {
        char MMSImageName[MG_MAX_FILE_NAME]={'/0'};
        MSG_FATAL("pMe->m_pMMSSOUND != NULL",0,0,0);
        ICONFIG_SetItem(pMe->m_pConfig, CFGI_MMSSOUND,MMSImageName, sizeof(MMSImageName));       
        RELEASEIF(pMe->m_pMMSSOUND);
    }    

    if(pMe->m_pMMSVIDEO!= NULL)
    {
        char MMSImageName[MG_MAX_FILE_NAME]={'/0'};
        MSG_FATAL("pMe->m_pMMSVIDEO != NULL",0,0,0);
        ICONFIG_SetItem(pMe->m_pConfig, CFGI_MMSVIDEO,MMSImageName, sizeof(MMSImageName));       
        RELEASEIF(pMe->m_pMMSVIDEO);
    }   
    if(pMe->m_pIFileMgr != NULL)
    {
        (void)IFILEMGR_Release(pMe->m_pIFileMgr);
    }
    FREEIF(pMe->m_DecData.message.hBody);
    FREEIF(pMe->m_DecData.message.mms_data.head_info.pContent);
    RELEASEIF(pMe->m_pMedia);
    for(i = 0;i < pMe->m_DecData.message.mms_data.frag_num;i++)
    {
        FREEIF(pMe->m_DecData.message.mms_data.fragment[i].pContent);
    }
    FREEIF(pMe->m_EncData.pReadReport);
    FREEIF(pMe->m_EncData.pDeliveryacknowledgement);
    FREEIF(pMe->m_EncData.pNotifyresp);
    FREEIF(pMe->m_EncData.pMessage);
    FREEIF(pMe->m_DeliveryEncData.pReadReport);
    FREEIF(pMe->m_DeliveryEncData.pDeliveryacknowledgement);
    FREEIF(pMe->m_DeliveryEncData.pNotifyresp);
    FREEIF(pMe->m_DeliveryEncData.pMessage);    
    FREEIF(pMe->m_MMSData);
    pMe->m_isMMS = FALSE;
    pMe->m_isForward = FALSE;
    pMe->m_wSelectStore = 1;
    pMe->m_pMMSMenuHasFocus = FALSE;
    WMS_MMS_RELEASE();
    if(pMe->m_Animate != NULL)
    {
        DBGPRINTF("CWmsApp_FreeAppData pMe->m_Animate=0x%x", pMe->m_Animate);
        IIMAGE_Stop(pMe->m_Animate);
        IIMAGE_Release(pMe->m_Animate);
        pMe->m_Animate = NULL;
    }
    
#endif    

    pMe->m_eAppStatus = WMSAPP_STOP;
    FREEIF(pMe->m_strPhonePWD);

    if (NULL != pMe->m_pConfig)
    {
        ICONFIG_Release(pMe->m_pConfig);
        pMe->m_pConfig = NULL;
    }

    //------------------------------------------------------------------------------------------------------
    if (NULL != pMe->m_pwms)
    {
        IWMS_Deactivate(pMe->m_pwms,pMe->m_clientId);		
        IWMS_Release(pMe->m_pwms);
        pMe->m_pwms = NULL;
    }
    
#ifdef FEATURE_ICM
    if (NULL != pMe->m_pICM)
    {
        ICM_Release(pMe->m_pICM);
        pMe->m_pICM = NULL;
    }
#endif  

    if (pMe->m_pIAnn) 
    {
        IANNUNCIATOR_Release(pMe->m_pIAnn);
        pMe->m_pIAnn= NULL;
    }
    if(pMe->m_pSound != NULL)
    {
        ISOUND_Release(pMe->m_pSound);
        pMe->m_pSound = NULL;
    }
    if (pMe->m_pAlert != NULL)
    {
        IALERT_Release(pMe->m_pAlert);
        pMe->m_pAlert = NULL;
    }
    
    if (pMe->m_pDisplay != NULL)
    {
        (void) IDISPLAY_Release(pMe->m_pDisplay);
        pMe->m_pDisplay = NULL;
    }
    
    // ע��˳��
    WmsApp_FreeSaveNumList(pMe);
    if (NULL != pMe->m_pSaveNumList)
    {
        IVector_Release(pMe->m_pSaveNumList);
        pMe->m_pSaveNumList = NULL;
    }
    WmsApp_FreeSendClentMsgList(pMe);  //add by yangdecai 2010-08-25
    // �ͷ��û������б�
    WmsApp_FreeUserDataMOList(pMe->m_pUserDataMOList);
    WmsApp_FreeMultiSendList(pMe->m_pSendList);
    WmsApp_MemberReset(pMe);
    if(pMe->m_pBacklight)
    {
        IBACKLIGHT_Release(pMe->m_pBacklight);
        pMe->m_pBacklight=NULL;
    }
	if (pMe->m_pIRUIM != NULL)
	{
	    IRUIM_Release(pMe->m_pIRUIM);
	    pMe->m_pIRUIM = NULL;
	}
    WMSMessageStruct_Reset(&pMe->m_msSend);
    MSG_FATAL("CWmsApp_FreeAppData End",0,0,0);
}
#ifdef FEATURE_CDSMS
/*=========================================================================
FUNCTION
  WMSAPP_CheckCDMAFlashSMSMessage

DESCRIPTION
  Check if a CDMA message is a Flash SMS Message.
  This is a Synchronous function call.

DEPENDENCIES
  None

RETURN VALUE
  TRUE:  Is Flash SMS Message
  FALSE: Is Flash SMS Message

SIDE EFFECTS
  None

=========================================================================*/
static boolean WMSAPP_CheckCDMAFlashSMSMessage
(
  const wms_client_bd_s_type *pClientBD
)
{
  boolean ret_value = FALSE;
  int temp = wms_ts_encode_relative_time(&pClientBD->validity_relative);
  MSG_MED("Function WMSAPP_CheckCDMAFlashSMSMessage entered", 0, 0, 0);

  if (NULL == pClientBD)
  {
    MSG_ERROR("Null Parameter Passed in WMSAPP_CheckCDMAFlashSMSMessage", 0, 0, 0);
    return ret_value;
  }
  MSG_FATAL("temp================%d,,pClientBD->mask==%d",temp,pClientBD->mask,0);
  MSG_FATAL("pClientBD->display_mode========%d,==%d,,===%d",pClientBD->display_mode,pClientBD->mask & WMS_MASK_BD_VALID_REL,pClientBD->mask & WMS_MASK_BD_DISPLAY_MODE);
  if((pClientBD->mask & WMS_MASK_BD_VALID_REL)
     && (WMSAPP_RELATIVE_VALIDITY_IMMEDIATE == 
         wms_ts_encode_relative_time(&pClientBD->validity_relative)))
  {
    ret_value = TRUE;
  }
  else if ( (pClientBD->mask & WMS_MASK_BD_DISPLAY_MODE)
            && (WMS_DISPLAY_MODE_IMMEDIATE == pClientBD->display_mode) )
  {
    ret_value = TRUE;
  }

  MSG_FATAL("Function WMSAPP_CheckCDMAFlashSMSMessage result = %d", ret_value, 0, 0);
  return ret_value;

}

#endif

/*==============================================================================
����:
    CWmsApp_HandleEvent

˵��:
    WMS Applet�¼��������������������ڱ�Applet���¼����������ɸú�������

����:
    pi [in]:ָ��IWmsApp�ӿ�t�����ָ�롣
    eCode:BREW�¼����롣
    wParam:�¼�����
    dwParam [in]:��wParam���������ݡ���

����ֵ:
    TRUE:�����¼�������
    FALSE:�����¼�û������

��ע:
    �ú�����BREW���ã��û����õ��á�
        
==============================================================================*/
static boolean CWmsApp_HandleEvent(IWmsApp  *pi,
    AEEEvent     eCode,
    uint16       wParam,
    uint32       dwParam
)
{
    WmsApp *pMe = (WmsApp*)pi;
    AEEAppStart *as=NULL;
    MSG_FATAL("CWmsApp_HandleEvent eCode=0x%x, wParam=0x%x, dwParam=0x%x",eCode,wParam,dwParam);
    IANNUNCIATOR_SetFieldIsActiveEx(pMe->m_pIAnn,FALSE);
    switch (eCode)
    {
        case EVT_APP_START:
            MSG_FATAL("EVT_APP_START %x %d",dwParam,pMe->m_currState,0);
            // ���¼�dwParamΪָ�룬��ӦΪ0
            if (dwParam == 0) 
            {
                return FALSE;
            }
            as = (AEEAppStart*)dwParam;
    
            pMe->m_eAppStatus = WMSAPP_RUNNING;
    
            pMe->m_rc = as->rc;
            {
                AEEDeviceInfo di; 
                
                ISHELL_GetDeviceInfo(pMe->m_pShell,&di);
                pMe->m_rc.dy = di.cyScreen;
            }

            if(as && STRCMP(as->pszArgs,"NEWSMS") == 0)
            {
                pMe->m_currState = WMSST_WMSNEW;
            }
            #ifdef FEATURE_USES_MMS
            else if(as && STRCMP(as->pszArgs,"NEWMMS") == 0)
            {
                pMe->m_currState = WMSST_INBOX_MMS;
            }
            #endif
			#ifdef FEATURE_VERSION_K202_LM129C //xxzhen
			else if(as && STRCMP(as->pszArgs,"WMSST_INBOXES") == 0)
            {
                pMe->m_currState = WMSST_INBOXES;
            }
			else if(as && STRCMP(as->pszArgs,"WMSST_WRITEMSG") == 0)
            {
                pMe->m_currState = WMSST_WRITEMSG;
            }
			#endif
#if defined(FEATURE_VERSION_C316)	
            else if(as && STRCMP(as->pszArgs,"CallApp") == 0)
            {
                pMe->m_currState = WMSST_TEMPLATES;
            }
#endif
            else
            {
                pMe->m_currState = WMSST_MAIN;
            }
            pMe->m_eDlgReturn = DLGRET_CREATE;
            pMe->m_bNaturalStart = TRUE;
			pMe->m_bActive = TRUE;

#ifdef WIN32
			pMe->m_bCdmaWmsReady = TRUE;
			pMe->m_refresh_in_progress = 0;
#endif
            
            if (((pMe->m_bCdmaWmsReady == FALSE) || pMe->m_refresh_in_progress))
            {
                // ��Ϣû��ʼ�����,�����������ز���.
                pMe->m_eDlgReturn = DLGRET_CREATE;
                
                pMe->m_currState = WMSST_INIT;
                if (pMe->m_pszArgs != NULL)
                {
                    if (STARTARGPREFIX_POWERONSTART == *pMe->m_pszArgs)
                    {
                        pMe->m_currState = WMSST_EXIT;
                    }
                    sys_free(pMe->m_pszArgs);
                    pMe->m_pszArgs = NULL;
                }
            }
            else if (pMe->m_pszArgs != NULL)
            {
                pMe->m_bNaturalStart = FALSE;
                
                WmsApp_ArgsStartInit(pMe, pMe->m_pszArgs);
                
                sys_free(pMe->m_pszArgs);
                pMe->m_pszArgs = NULL;
            }
            else if (pMe->m_bNeedContinueSend)
            {
                MOVE_TO_STATE(WMSST_CONTINUESEND_QUERY)
                pMe->m_stcontinuesendbk = WMSST_MAIN;
            }
            
#if defined(FEATURE_WMSAPP_ONLYSUPPORTVMAIL)
            if (pMe->m_currState != WMSST_EXIT)
            {
                pMe->m_currState = WMSST_MAIN;
            }
#endif
            if(pMe->m_bNaturalStart == TRUE)
            {
                boolean bsmslock;
                (void) ICONFIG_GetItem(pMe->m_pConfig,
                                       CFGI_SMS_LOCK_CHECK,
                                       &bsmslock,
                                       sizeof(bsmslock));
                //bsmslock = FALSE;
                if(bsmslock)
                {
                    pMe->m_currState = WMSST_CHKPWD;
                    pMe->m_stchkpwdbk = WMSST_MAIN;                    
                }
            }
            MSG_FATAL("EVT_APP_START %d",pMe->m_currState,0,0);
            // ��ʼ��WMS״̬��
            CWmsApp_RunFSM(pMe);          
            return TRUE;

        case EVT_APP_STOP: 
			if(pMe->m_bincommend)
        	{
        		int32  nItems = 0;
                uint16 nMsgs = 0;
                MSG_FATAL("EVT_DIALOG_END....IDD_WRITEMSG_Handler........2",0,0,0);
                // �ͷ��û������б�
                WmsApp_FreeUserDataMOList(pMe->m_pUserDataMOList);
                
                // �����Ϣ
                WmsApp_PrepareUserDataMOList(pMe);
                pMe->m_idxUserdata = 0;
                
                nItems = IVector_Size(pMe->m_pUserDataMOList);
                
                // ��ȡ�ݸ�����Ϣ��
                wms_cacheinfolist_getcounts(WMS_MB_DRAFT, NULL, NULL, &nMsgs);
                
                if ((nMsgs+nItems) <= DRAFT_MAX)
                {// �洢�ռ��㹻�������жϵ����뵽�ݸ���
                    wms_client_message_s_type *pClientMsg = NULL;
                    int nRet;
                    boolean Is_Notend = FALSE;
					ICONFIG_SetItem(pMe->m_pConfig,CFGI_WMSWRITD_END_STATUS,&Is_Notend,sizeof(Is_Notend));
                    WmsApp_FreeMultiSendList(pMe->m_pSendList);
                    
                    pClientMsg = WmsApp_GetClientMsgMO(pMe, FALSE);
                    while (pClientMsg != NULL)
                    {
                        // Must modify message tag!
                        pClientMsg->msg_hdr.tag = WMS_TAG_MO_DRAFT;
                        MSG_FATAL("EVT_DIALOG_END....IDD_WRITEMSG_Handler........3",0,0,0);
                        // ������Ϣ
                        nRet = ENOMEMORY;
                        do 
                        {
                            nRet = IWMS_MsgWrite(pMe->m_pwms, 
                                                 pMe->m_clientId, 
                                                 &pMe->m_callback,
                                                 (void*)pMe,
                                                 WMS_WRITE_MODE_INSERT,
                                                 pClientMsg);
                                                 
#ifndef WIN32
                            if (nRet == SUCCESS)
                            {// ����10������ȷ����ʱ��ִ�б�����Ϣ�Ĳ���
                                MSLEEP(10);
                            }
#endif
                        } while(nRet != SUCCESS);
                                            
                        FREE(pClientMsg);
                        pClientMsg = WmsApp_GetClientMsgMO(pMe, FALSE);
                    }
                    
                    WmsApp_FreeMultiSendList(pMe->m_pSendList);
                    
                    // �ͷ��û������б�
                    WmsApp_FreeUserDataMOList(pMe->m_pUserDataMOList);
                    
                    pMe->m_idxUserdata = 0;
                }
                else
                {
                    // �ͷ��û������б�
                    WmsApp_FreeUserDataMOList(pMe->m_pUserDataMOList);
                }
        	}
			//�ͷŲ鿴����Ϣ�ڴ�
			MSG_FATAL("EVT_APP_STOP....IDD_WRITEMSG_Handler........",0,0,0);
			WMSMessageStruct_Free(pMe);
			//ADD BY YANGDECAI 2010-08-16
            pMe->m_bNaturalStart = TRUE;
            pMe->m_eAppStatus = WMSAPP_STOP;
    		pMe->m_bActive = TRUE;
    		pMe->m_bincommend = FALSE;
            if(pMe->m_pImage)
            {
                IIMAGE_Release(pMe->m_pImage);
                pMe->m_pImage = NULL;
            }
        
            // תΪ��̨ģʽ
            *((boolean *)dwParam) = FALSE;
            
            // ����������Ҫ�ı������ù���
            WmsApp_MemberReset(pMe);
            pMe->m_wMainMenuSel = 0;//�����˵������ù����ָ��������һ�ν������ģ��ʹ���ͣ���ڵ�һ����
            return TRUE;

        case EVT_APP_SUSPEND:
        	MSG_FATAL("EVT_APP_SUSPEND....IDD_WRITEMSG_Handler........",0,0,0);
            pMe->m_eAppStatus = WMSAPP_SUSPEND;
            pMe->m_bActive = FALSE;
            return TRUE;
    
        case EVT_APP_RESUME:
        	MSG_FATAL("EVT_APP_RESUME....IDD_WRITEMSG_Handler........",0,0,0);
            if (dwParam == 0) 
            {
                return FALSE;
            }
            as = (AEEAppStart*)dwParam;
    
            pMe->m_eAppStatus = WMSAPP_RUNNING;
            
            pMe->m_rc = as->rc;
            {
                AEEDeviceInfo di; 
                
                ISHELL_GetDeviceInfo(pMe->m_pShell,&di);
                pMe->m_rc.dy = di.cyScreen;
            }
            pMe->m_bNaturalStart = TRUE;
            pMe->m_bActive = TRUE;
            if ((pMe->m_bCdmaWmsReady == FALSE) || pMe->m_refresh_in_progress)
            {
                // ��Ϣû��ʼ�����,�����������ز���.
                pMe->m_eDlgReturn = DLGRET_CREATE;
                
                pMe->m_currState = WMSST_INIT;
                if (pMe->m_pszArgs != NULL)
                {
                    sys_free(pMe->m_pszArgs);
                    pMe->m_pszArgs = NULL;
                    if (STARTARGPREFIX_POWERONSTART == *pMe->m_pszArgs)
                    {
                        pMe->m_currState = WMSST_EXIT;
                    }
                }
            }
            else if (pMe->m_pszArgs != NULL)
            {
                pMe->m_bNaturalStart = FALSE;
                WmsApp_ArgsStartInit(pMe, pMe->m_pszArgs);
                
                sys_free(pMe->m_pszArgs);
                pMe->m_pszArgs = NULL;
            }
            else if (pMe->m_bNeedContinueSend)
            {
                MOVE_TO_STATE(WMSST_CONTINUESEND_QUERY)
                pMe->m_stcontinuesendbk = WMSST_MAIN;
            }
            else if (pMe->m_currState == WMSST_WRITEMSG)
            {
                if (pMe->m_bTextFullAlert)
                {
                    pMe->m_eDlgReturn = DLGRET_TEXTFULL;
                    pMe->m_bTextFullAlert = FALSE;
                }
            }
            
#if defined(FEATURE_WMSAPP_ONLYSUPPORTVMAIL)
            pMe->m_currState = WMSST_MAIN;
#endif
            if(pMe->m_isPopMenu)
            {
                pMe->m_eDlgReturn = DLGRET_CANCELED;
            }
            
            // ��ʼ��WMS״̬��
            CWmsApp_RunFSM(pMe); 
            return TRUE;

        case EVT_DIALOG_INIT:
			if(OEM_IME_DIALOG == wParam)
			{
				return TRUE;
			}
            pMe->m_pActiveIDlg = (IDialog*)dwParam;
            pMe->m_wActiveDlgID = wParam;
            WmsApp_SetDialogHandler(pMe);
            
            // Route the received event to the current active dialog handler.
            return WmsApp_RouteDialogEvt(pMe,eCode,wParam,dwParam);

        case EVT_DIALOG_START:
			if(OEM_IME_DIALOG == wParam)
			{
				return TRUE;
			}
            (void) WmsApp_RouteDialogEvt(pMe,eCode,wParam,dwParam);

            return TRUE;
            
        case EVT_DIALOG_END:
			pMe->m_bincommend = FALSE;
            MSG_FATAL("EVT_DIALOG_END %d %d %d",wParam,pMe->m_eAppStatus,pMe->m_currState);
			if(OEM_IME_DIALOG == wParam)
			{
				return ISHELL_PostEvent(pMe->m_pShell,AEECLSID_WMSAPP,EVT_USER_REDRAW,0,0);
			}
            if (wParam == 0)
            {
                return TRUE;
            }
    
            (void) WmsApp_RouteDialogEvt(pMe,eCode,wParam,dwParam);
    
            pMe->m_pActiveIDlg = NULL;
            pMe->m_wActiveDlgID = 0;
            pMe->m_pDialogHandler = NULL;
    
            // �¼�EVT_APP_SUSPEND����EVT_DIALOG_END�¼����������
            // ������Applet���ڹ���״̬ʱ������״̬����
            if (pMe->m_eAppStatus == WMSAPP_RUNNING)
            {                
                // ��ʼ��WMS״̬��
                CWmsApp_RunFSM(pMe);
            }
            return TRUE;

        case EVT_USER_REDRAW:
            (void)WmsApp_RouteDialogEvt(pMe,eCode,wParam,dwParam);
            return TRUE;

        case EVT_UPDATE:
            (void)WmsApp_RouteDialogEvt(pMe,eCode,wParam,dwParam);
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
            return WmsApp_RouteDialogEvt(pMe,eCode,wParam,dwParam);
            
        case EVT_COMMAND:        	
        case EVT_KEY_PRESS:
        case EVT_KEY_RELEASE:
            return WmsApp_RouteDialogEvt(pMe,eCode,wParam,dwParam);

        case EVT_BUSY:
            // The applet is never 'busy'
            break;
            
        case EVT_NOTIFY:
            return WmsApp_HandleNotifyEvent(pMe, (AEENotify*)dwParam);
       
        case EVT_WMS_CFG_MEMORY_STATUS:
            WmsApp_UpdateMemoryStatus(pMe,&((wms_cfg_event_info_s_type *)dwParam)->memory_status);
            WMSAPPU_SYSFREE(dwParam);
            MSG_FATAL("***zzg WmsApp WmsApp_UpdateAnnunciators 111***", 0, 0, 0);
            WmsApp_UpdateAnnunciators(pMe);
            return TRUE;
        
        // ��Ϣ��ʼ�����
        case EVT_WMS_CFG_CDMA_READY:
            pMe->m_mode = WMS_MESSAGE_MODE_CDMA;
            WmsApp_Init(pMe);
            WMSAPPU_SYSFREE(dwParam)
            return TRUE;
            
        case EVT_WMS_CFG_REFRESH_DONE:
            pMe->m_refresh_in_progress = FALSE;
            WMSAPPU_SYSFREE(dwParam)
            return TRUE;
            
        case EVT_WMS_CFG_ROUTES:
            {
                wms_cfg_event_info_s_type *Info = (wms_cfg_event_info_s_type*)dwParam;
                
                (void)MEMCPY(&pMe->m_routes, &Info->routes, sizeof(pMe->m_routes));
            }
            WMSAPPU_SYSFREE(dwParam)
            return TRUE;
            
        case EVT_WMS_MSG_WRITE:
        case EVT_WMS_MSG_READ:
        case EVT_WMS_MSG_WRITE_TEMPLATE:
        case EVT_WMS_MSG_READ_TEMPLATE:
            (void)WmsApp_RouteDialogEvt(pMe,eCode,wParam,dwParam);
            WMSAPPU_SYSFREE(dwParam)

            return TRUE;
            
        case EVT_WMS_MSG_MODIFY_TAG:
            WMSAPPU_SYSFREE(dwParam)

            MSG_FATAL("***zzg WmsApp WmsApp_UpdateAnnunciators 222***", 0, 0, 0);

            // ����ͼ��
            WmsApp_UpdateAnnunciators(pMe);
            
            return TRUE;
            
        case EVT_WMS_MSG_SUBMIT_REPORT:
        case EVT_WMS_MSG_SEND:
            MSG_FATAL("CWmsApp_HandleEvent EVT_WMS_MSG_SEND eCode=0X%x",eCode,0,0);
			if (((wms_msg_event_info_s_type *)dwParam)->submit_report_info.client_id == WMS_CLIENT_TYPE_WMS_APP)            
            {
                WmsApp_ProcessStatus(pMe, &((wms_msg_event_info_s_type *)dwParam)->submit_report_info);
            }
			
            WMSAPPU_SYSFREE((wms_msg_event_info_s_type*)dwParam);

            return TRUE;

#ifdef FEATURE_USES_MMS
        case EVT_MMS_MSG_SEND_FINSH:
            {
                MSG_FATAL("CWmsApp_HandleEvent EVT_MMS_MSG_SEND_FINSH",0,0,0);
                pMe->m_SendStatus = wParam;
                WmsApp_ProcessMMSStatus(pMe);
                
            }
            return TRUE;
            
        case EVT_MMS_MSG_GET_FINISH:
            {
                pMe->m_GetStatus = wParam;
                WmsApp_ProcessMMSStatus(pMe);
            }
            return TRUE;

#endif
        case EVT_WMS_MSG_RECEIVED_MESSAGE:
            {
            	#define HEADER_INFO 3

				int data_len;
				uint8 *data = NULL;
				int header_len = 0;
				uint8 *body;
				int body_len;
                wms_msg_event_info_s_type *info = ((wms_msg_event_info_s_type*)dwParam);
                boolean bRet;
                uint8 notify_buf[256] = {0};                
                int nRet;
#ifdef FEATURE_SMS_UDH
                boolean bUDHPortPresent = FALSE;
#endif
                
                
                MSG_FATAL("WMSApp received new message teleservice=%d",info->mt_message_info.message.u.cdma_message.teleservice,0,0);

#if 0//def MMS_TEST
                {
                    IFile* pIFile = NULL;
        		    IFileMgr *pIFileMgr = NULL;
        		    int result = 0;

        			result = ISHELL_CreateInstance(AEE_GetShell(), AEECLSID_FILEMGR,(void **)&pIFileMgr);
        			if (SUCCESS != result)
        		    {
        				MSG_FATAL("MRS: Open file error %x", result,0,0);
        				return;
        		    }

        		    pIFile = IFILEMGR_OpenFile( pIFileMgr, "fs:/hsmm/pictures/raw_ts_data.txt", _OFM_READWRITE);
        			if ( pIFile != NULL )
        	        {
        	            IFILE_Seek(pIFile, _SEEK_START, 0);
        	            IFILE_Write( pIFile, info->mt_message_info.message.u.cdma_message.raw_ts.data, info->mt_message_info.message.u.cdma_message.raw_ts.len);

        	            MSG_FATAL("IFILEMGR_OpenFile pDataFragment.size=%d",info->mt_message_info.message.u.cdma_message.raw_ts.len,0,0);
        	            IFILE_Release( pIFile);
        	            pIFile = NULL;
        	            IFILEMGR_Release(pIFileMgr);
        	            pIFileMgr = NULL;
        	        }
        	        else
        	        {
        				pIFile = IFILEMGR_OpenFile( pIFileMgr, "fs:/hsmm/pictures/raw_ts_data.txt", _OFM_CREATE);
        				if ( pIFile != NULL )
        		        {
        		            IFILE_Write( pIFile, info->mt_message_info.message.u.cdma_message.raw_ts.data, info->mt_message_info.message.u.cdma_message.raw_ts.len);

        		            MSG_FATAL("IFILEMGR_OpenFile pDataFragment.size=%d",info->mt_message_info.message.u.cdma_message.raw_ts.len,0,0);
        		            IFILE_Release( pIFile);
        		            pIFile = NULL;
        		            IFILEMGR_Release(pIFileMgr);
        		            pIFileMgr = NULL;
        		        }
        	        }
                }
#endif   
                // �� CDMA ģʽ��Ϣ��������
                if (info->mt_message_info.message.msg_hdr.message_mode != WMS_MESSAGE_MODE_CDMA)
                {
                    WMSAPPU_SYSFREE(dwParam);
                    return TRUE;
                }
                
                bRet = IWMS_CfgCheckWapPushMsg(pMe->m_pwms, 
                            &info->mt_message_info.message);
                
				if (bRet == TRUE)
                {
					(void)MEMSET(&pMe->m_CltTsdata, 0 , sizeof(pMe->m_CltTsdata));
					MEMCPY(&pMe->m_message,&info->mt_message_info.message,sizeof(wms_client_message_s_type));
                     
	                 nRet = IWMS_TsDecode(pMe->m_pwms, 
	                             &info->mt_message_info.message.u.cdma_message.raw_ts, 
	                             &pMe->m_CltTsdata);

#ifdef FEATURE_USES_MMS
	            	 nRet = IWMS_MMsDecodeNotifyBody(pMe->m_pwms,&pMe->m_CltTsdata,notify_buf);
	            

					 //wangliang add! pass the notify message body to process
					 //MMS notify is exist
					 /*
					 ** buffer:
					 ** MSG_TYPE       			8       00000000
					 ** TOTAL_SEGMENTS         	8       Total  number of segments
					 ** SEGMENT_NUMBER       	8       The Segment Number
					 ** DATAGRAM      (NUM_FIELDS �C 3)*8         Segmented WDP Datagram
					 **
					 ** WDP Datagram Segment
					 ** SOURCE_PORT       16     Source Port
					 ** DESTINATION_PORT       16     Destination Port(2948)
					 ** DATA        N*8  N bytes of Data from the Layer above WDP
					 */
					if ( nRet > 0 )
					{
						data_len = nRet;
						
						if ( data_len < HEADER_INFO )
						{
							goto Exit;
						}


						//get URL,we only need the first sms nofity
						if ( notify_buf[2] == 0 )
						{
							//WDP body							
								
							data = &notify_buf[7];
							header_len = data[HEADER_INFO-1];
							if(header_len == 0x1f)
							{
							    header_len = data[HEADER_INFO];
							}
							else if(header_len > 0x1f)
							{
							    header_len = 0;
							    while(data[HEADER_INFO - 1 + header_len] != 0)	
							    {
							        header_len ++;
							    }
							}
							
							if( data_len <= HEADER_INFO + header_len ) 
							{
								goto Exit;
							}
							else
							{
								uint8 *header = NULL;
								uint8 *ptrCur = NULL;
								header = (uint8 *)MALLOC(header_len+1);
                                MSG_FATAL("WMS address=0x%x", header, 0, 0);
                                
								if( NULL == header)
								{
									MSG_FATAL("SMSPUSH_ALLOC_ERROR1",0,0,0);
									goto Exit;
								}

								MEMCPY( header, &data[HEADER_INFO], header_len );
								header[header_len] = '\0';

								if( !STRSTR((char*)header, "application/vnd.wap.mms-message"))
								{
									FREEIF( header );
									goto Exit;
								}
								// yy add header judgement
								body = data + HEADER_INFO + header_len;
								ptrCur = (uint8*)STRCHR((char*)body,0xb4);
								if(ptrCur == NULL || ptrCur[1] != 0x87)
								{
									FREEIF( header );
									goto Exit;
								}

								ptrCur = (uint8*)STRCHR((char*)body,0xaf);
								if(ptrCur == NULL || ptrCur[1] != 0x84)
								{
									FREEIF( header );
									goto Exit;
								}
				
								header_len += 4;
								body_len = data_len - HEADER_INFO - header_len;
								FREEIF( header );
							}

							body = (uint8*)sys_malloc(body_len);
                            MSG_FATAL("WMS address=0x%x", body, 0, 0);
							if( NULL == body )
							{
								MSG_FATAL("SMSPUSH_ALLOC_ERROR2",0,0,0);
								goto Exit;
							}

							MEMCPY( (void*)body, (void*)(data + HEADER_INFO + header_len), body_len);

							//now, body need pass to function WMS_MMS_PDU_Decode to decode the URL.
							MSG_FATAL(" MMS_WSP_DEC_DATA decData",0,0,0);
                           
							ISHELL_PostEventEx(pMe->m_pShell,
							    EVTFLG_ASYNC,
							    AEECLSID_WMSAPP,
							    EVT_MMS_PDUDECODE,
							    body_len,
							    (int32)body);
                            DBGPRINTF("body_len=%d", body_len);
                            DBGPRINTF("body=%s", (char*)body);
							    
						}
						else
						{
							goto Exit;
						}
					}
                    MSG_FATAL("IWMS_MMsDecodeNotifyBody=%d", nRet, 0, 0);
                    WMSAPPU_SYSFREE(dwParam);
                    return TRUE;
#endif

                    {
Exit:                    
                    	WMSAPPU_SYSFREE(dwParam);
                    	return TRUE;
                        
                    }
                    
                }
                else
                {
                
#ifdef FEATURE_SMS_UDH
                    // �����Ϣ�Ƿ�� Port ͷ���ṹ�����ýṹ����Ϣ���ض�Ӧ�ô���
                    (void)MEMSET(&pMe->m_CltTsdata, 0 , sizeof(pMe->m_CltTsdata));
                        
                    nRet = IWMS_TsDecode(pMe->m_pwms, 
                                 &info->mt_message_info.message.u.cdma_message.raw_ts, 
                                 &pMe->m_CltTsdata);
                                 
                    if (nRet == SUCCESS)
                    {
                    	#ifdef FEATURE_CDSMS
                    	// Check for Flash SMS Message
                    	boolean  i = WMSAPP_CheckCDMAFlashSMSMessage(&pMe->m_CltTsdata.u.cdma);
                    	MSG_FATAL("i=======================%d",i,0,0);
                    	if (WMSAPP_CheckCDMAFlashSMSMessage(&pMe->m_CltTsdata.u.cdma) == TRUE)
                    	{
                      		
                      		#ifdef FEATURE_FLASH_SMS
    						pMe->m_bflash_sms = TRUE;              //add by yangdecai 2011-04-01
    						#else
    						/* Drop this Message since it is not supported */
                      		MSG_HIGH("CDMA Flash SMS Message Recieved and Dropped", 0, 0, 0);
                      		WMSAPPU_FREE(dwParam);
                      		return TRUE;
    						#endif
                    	}
                    	#endif
                        if (pMe->m_CltTsdata.u.cdma.message_id.udh_present)
                        {
                            uint8 i=0;
                            
                            for (i=0; i<pMe->m_CltTsdata.u.cdma.user_data.num_headers; i++)
                            {
                                if ((pMe->m_CltTsdata.u.cdma.user_data.headers[i].header_id == WMS_UDH_PORT_8) ||
                                    (pMe->m_CltTsdata.u.cdma.user_data.headers[i].header_id == WMS_UDH_PORT_16))
                                {
                                    bUDHPortPresent = TRUE;
                                    break;
                                }
                            }
                        }
                    }
                        
                    if (bUDHPortPresent)
                    {
                        // ��������������Χ�ڵ���Ϣ
                        MSG_HIGH("Message with UDH Port Header Recieved and Dropped", 0, 0, 0);
                        WMSAPPU_SYSFREE(dwParam);
                        return TRUE;
                    }
#endif // FEATURE_SMS_UDH
                }
            }
            // ע�⣺������ EVT_WMS_MSG_STATUS_REPORT ����
        case EVT_WMS_MSG_STATUS_REPORT:
            {
                wms_msg_event_info_s_type *info = ((wms_msg_event_info_s_type*)dwParam);
#ifdef FEATURE_ICM
				uint16 num = 0;
#endif


#ifdef FEATURE_CALL_RESTRICT
                {
                    //������Ѻ������﷢���Ķ��Ÿ�ɾ�������Ҳ���ʾ֪ͨ���û���
                    char    szFrom[32+1];
                    AECHAR  szRestricName[32 + 1]={0}; 
                    byte    *pDst;
                    int     nOffset = 0;
                    uint8   i;
                    uint8   byMax = 0;
                    wms_address_s_type           address;
                    call_restrict_info        call_restrict_list[MAX_CALL_RESTRICT];   
                    
                    address = info->mt_message_info.message.u.cdma_message.address;
                    
                    pDst = (byte*)szFrom;

                    if (address.number_type == WMS_NUMBER_INTERNATIONAL) 
                    {
                        if (nOffset < sizeof(szFrom)) 
                        {
                            pDst[nOffset] = '+';
                            nOffset++;
                        }
                    }

                    if (address.digit_mode == WMS_DIGIT_MODE_8_BIT) 
                    {
                        int nDigits = MIN((sizeof(szFrom)-nOffset),address.number_of_digits);
                        memcpy(pDst + nOffset, address.digits, nDigits);
                        nOffset += nDigits;
                    }   
                    else if (address.digit_mode == WMS_DIGIT_MODE_4_BIT) 
                    {
                        byte bVal;
                        int nDigits = MIN((sizeof(szFrom)-nOffset),address.number_of_digits);

                        for (i = 0; i < nDigits; i++) 
                        {
                            bVal = (byte)address.digits[i] & 0x0f;
                            pDst[nOffset] = GetDigit(bVal) ;
                            nOffset++;
                        }
                    }
                    pDst[nOffset] = '\0';
                    DBGPRINTF("szFrom=%s", szFrom);
                   
                    //��ʼ���պ�����������
                    (void) ICONFIG_GetItem(pMe->m_pConfig,
                                           CFGI_CALL_RESTRICT_TOTAL,
                                           &byMax,
                                           sizeof(uint8));  
                    MSG_FATAL("EVT_WMS_MSG_STATUS_REPORT byMax=%d",byMax,0,0);
                    //��ʼ�����ն��ź���������Ϣ
                    (void) ICONFIG_GetItem(pMe->m_pConfig,
                                           CFGI_CALL_RESTRICT_INFO,
                                           (void*)call_restrict_list,
                                           sizeof(call_restrict_list));
                                      
                    STRTOWSTR(szFrom, szRestricName, sizeof(szRestricName));                         
                    if(byMax > 0)
                    {
                        for(i=0; i < byMax; ++i)
                        {
                            //������ź�����ʱ����ɾ������Ķ��ţ���ֱ�ӷ���TRUESW
                            //DBGPRINTF("EVT_WMS_MSG_STATUS_REPORT i=%d szName=%S", i,call_restrict_list[i].szName);
                            if(WSTRCMP(szRestricName, call_restrict_list[i].szNumber)== 0)
                            {
                                uint16 wIndex=0;
                                wms_cache_info_node  *pnode = NULL;
                                int i;
                                wms_cache_info_node  *pnode2 = NULL;   
                                wms_cache_info_node  *TempCurMsgNodes[LONGSMS_MAX_PACKAGES];
                                int nRet,nCount=0;
                                wms_cache_info_list   *pList = NULL;
                                boolean bUIMSMS = FALSE;
                                pList = wms_get_cacheinfolist(WMS_MB_INBOX);
                                if (NULL != pList)
                                {
                                    wIndex = pList->nBranches+1;
                                }         
                                else
                                {
                                    wIndex = 0;
                                }
                                MSG_FATAL("szRestricName == szName",wIndex,0,0);
                                // ȡ��Ϣ cache info �ڵ�
                                if (wIndex>=RUIM_MSGINDEX_BASE)
                                {
                                    wIndex = wIndex - RUIM_MSGINDEX_BASE;
                                    pnode = wms_cacheinfolist_getnode(WMS_MB_INBOX, WMS_MEMORY_STORE_RUIM, wIndex);
                                }
                                else
                                {
                                    pnode = wms_cacheinfolist_getnode(WMS_MB_INBOX, WMS_MEMORY_STORE_NV_CDMA, wIndex);
                                }
                                
                                if (pnode != NULL)
                                {
                                    MEMSET(TempCurMsgNodes, 0, sizeof(TempCurMsgNodes));
                                    MEMCPY(TempCurMsgNodes, pMe->m_CurMsgNodes, sizeof(TempCurMsgNodes));
                                    // ���õ�ǰ��Ϣ�б�
                                    MEMSET(pMe->m_CurMsgNodes, 0, sizeof(pMe->m_CurMsgNodes));
                                    WmsApp_FreeMsgNodeMs(pMe);
                                    
                                    pMe->m_idxCur = 0;
#ifdef FEATURE_SMS_UDH
                                    if (pnode->pItems != NULL)
                                    {
                                        MEMCPY(pMe->m_CurMsgNodes, pnode->pItems, sizeof(pMe->m_CurMsgNodes));
                                        
                                        for (; pMe->m_idxCur<LONGSMS_MAX_PACKAGES; pMe->m_idxCur++)
                                        {
                                            if (pMe->m_CurMsgNodes[pMe->m_idxCur] != NULL)
                                            {
                                                pnode = pMe->m_CurMsgNodes[pMe->m_idxCur];
                                                break;
                                            }
                                        }
                                    }
                                    else
#endif
                                    {
                                        pMe->m_CurMsgNodes[0] = pnode;
                                    }
        
                                    for (i=0; i<LONGSMS_MAX_PACKAGES; i++)
                                    {
                                        if (pMe->m_CurMsgNodes[i] != NULL)
                                        {
                                            pnode2 = pMe->m_CurMsgNodes[i];
                                            
                                            // ����ɾ����Ϣ����
                                            nRet = ENOMEMORY;
                                            do
                                            {
                                                nRet = IWMS_MsgDelete(pMe->m_pwms,
                                                                   pMe->m_clientId,
                                                                   &pMe->m_callback,
                                                                   (void *)pMe,
                                                                   pnode2->mem_store,
                                                                   pnode2->index);
                                            } while(nRet != SUCCESS);
                                            pMe->m_CurMsgNodes[i] = NULL;
                                        }
                                    }
                                    MEMSET(pMe->m_CurMsgNodes, 0, sizeof(pMe->m_CurMsgNodes));
                                    WmsApp_FreeMsgNodeMs(pMe);
                                    MEMCPY(pMe->m_CurMsgNodes, TempCurMsgNodes, sizeof(pMe->m_CurMsgNodes));
        
                                    for (i=0; i<LONGSMS_MAX_PACKAGES; i++)
                                    {
                                        if (TempCurMsgNodes[i] != NULL)
                                        {
                                            FREE(TempCurMsgNodes[i]);//�п������ظ�ɾ��
                                            TempCurMsgNodes[i] = NULL;
                                        }
                                    }      
                                    MSG_FATAL("EVT_WMS_MSG_STATUS_REPORT end m_wCurindex=%d",pMe->m_wCurindex,0,0);
                                    WMSAPPU_SYSFREE(dwParam);
                                    return TRUE;
                                }
                            }
                        }
                    }       
                }
#endif


#ifdef FEATURE_VERSION_W208S 

                {
                    //������Ѻ������﷢���Ķ��Ÿ�ɾ�������Ҳ���ʾ֪ͨ���û���
                    char szFrom[32+1];
                    AECHAR          szRestricName[32 + 1]={0}; 
                    byte * pDst;
                    int nOffset = 0;
                    uint8 i;
                    uint8 byMax = 0;
                    wms_address_s_type           address;
                    sms_restrict_recive_info        sms_restrict_reciveList[MAX_SMS_RESTRICT];   
                    MSG_FATAL("EVT_WMS_MSG_STATUS_REPORT 1",0,0,0);
                    address = info->mt_message_info.message.u.cdma_message.address;
                    
                    pDst = (byte*)szFrom;

                    if (address.number_type == WMS_NUMBER_INTERNATIONAL) 
                    {
                        if (nOffset < sizeof(szFrom)) 
                        {
                            pDst[nOffset] = '+';
                            nOffset++;
                        }
                    }

                    if (address.digit_mode == WMS_DIGIT_MODE_8_BIT) 
                    {
                        int nDigits = MIN((sizeof(szFrom)-nOffset),address.number_of_digits);
                        memcpy(pDst + nOffset, address.digits, nDigits);
                        nOffset += nDigits;
                    }   
                    else if (address.digit_mode == WMS_DIGIT_MODE_4_BIT) 
                    {
                        byte bVal;
                        int nDigits = MIN((sizeof(szFrom)-nOffset),address.number_of_digits);

                        for (i = 0; i < nDigits; i++) 
                        {
                            bVal = (byte)address.digits[i] & 0x0f;
                            pDst[nOffset] = GetDigit(bVal) ;
                            nOffset++;
                        }
                    }
                    pDst[nOffset] = '\0';
                    DBGPRINTF("szFrom=%s", szFrom);
   
                    //��ʼ���պ�����������
                    (void) ICONFIG_GetItem(pMe->m_pConfig,
                                           CFGI_SMS_RESTRICT_RECEIVE_TOTAL,
                                           &byMax,
                                           sizeof(uint8));  
                    MSG_FATAL("EVT_WMS_MSG_STATUS_REPORT byMax=%d",byMax,0,0);
                    //��ʼ�����ն��ź���������Ϣ
                    (void) ICONFIG_GetItem(pMe->m_pConfig,
                                           CFGI_SMS_RESTRICT_RECEIVE_INFO,
                                           (void*)sms_restrict_reciveList,
                                           sizeof(sms_restrict_reciveList));
                    STRTOWSTR(szFrom, szRestricName, sizeof(szRestricName));                         
                    if(byMax > 0)
                    {
                        for(i=0; i < byMax; ++i)
                        {
                            //������ź�����ʱ����ɾ������Ķ��ţ���ֱ�ӷ���TRUESW
                            DBGPRINTF("EVT_WMS_MSG_STATUS_REPORT i=%d szName=%S", i,sms_restrict_reciveList[i].szName);
                            if(WSTRCMP(szRestricName, sms_restrict_reciveList[i].szName)== 0)
                            {
                                uint16 wIndex=0;
                                wms_cache_info_node  *pnode = NULL;
                                int i;
                                wms_cache_info_node  *pnode2 = NULL;   
                                wms_cache_info_node             *TempCurMsgNodes[LONGSMS_MAX_PACKAGES];
                                int nRet,nCount=0;
                                wms_cache_info_list   *pList = NULL;
                                boolean bUIMSMS = FALSE;
                                pList = wms_get_cacheinfolist(WMS_MB_INBOX);
                                if (NULL != pList)
                                {
                                    wIndex = pList->nBranches+1;
                                }         
                                else
                                {
                                    wIndex = 0;
                                }
                                MSG_FATAL("szRestricName == szName",wIndex,0,0);
                                // ȡ��Ϣ cache info �ڵ�
                                if (wIndex>=RUIM_MSGINDEX_BASE)
                                {
                                    wIndex = wIndex - RUIM_MSGINDEX_BASE;
                                    pnode = wms_cacheinfolist_getnode(WMS_MB_INBOX, WMS_MEMORY_STORE_RUIM, wIndex);
                                }
                                else
                                {
                                    pnode = wms_cacheinfolist_getnode(WMS_MB_INBOX, WMS_MEMORY_STORE_NV_CDMA, wIndex);
                                }
                                
                                if (pnode != NULL)
                                {
                                    MEMSET(TempCurMsgNodes, 0, sizeof(TempCurMsgNodes));
                                    MEMCPY(TempCurMsgNodes, pMe->m_CurMsgNodes, sizeof(TempCurMsgNodes));
                                    // ���õ�ǰ��Ϣ�б�
                                    MEMSET(pMe->m_CurMsgNodes, 0, sizeof(pMe->m_CurMsgNodes));
                                    WmsApp_FreeMsgNodeMs(pMe);
                                    
                                    pMe->m_idxCur = 0;
#ifdef FEATURE_SMS_UDH
                                    if (pnode->pItems != NULL)
                                    {
                                        MEMCPY(pMe->m_CurMsgNodes, pnode->pItems, sizeof(pMe->m_CurMsgNodes));
                                        
                                        for (; pMe->m_idxCur<LONGSMS_MAX_PACKAGES; pMe->m_idxCur++)
                                        {
                                            if (pMe->m_CurMsgNodes[pMe->m_idxCur] != NULL)
                                            {
                                                pnode = pMe->m_CurMsgNodes[pMe->m_idxCur];
                                                break;
                                            }
                                        }
                                    }
                                    else
#endif
                                    {
                                        pMe->m_CurMsgNodes[0] = pnode;
                                    }
        
                                    for (i=0; i<LONGSMS_MAX_PACKAGES; i++)
                                    {
                                        if (pMe->m_CurMsgNodes[i] != NULL)
                                        {
                                            pnode2 = pMe->m_CurMsgNodes[i];
                                            
                                            // ����ɾ����Ϣ����
                                            nRet = ENOMEMORY;
                                            do
                                            {
                                                nRet = IWMS_MsgDelete(pMe->m_pwms,
                                                                   pMe->m_clientId,
                                                                   &pMe->m_callback,
                                                                   (void *)pMe,
                                                                   pnode2->mem_store,
                                                                   pnode2->index);
                                            } while(nRet != SUCCESS);
                                            pMe->m_CurMsgNodes[i] = NULL;
                                        }
                                    }
                                    MEMSET(pMe->m_CurMsgNodes, 0, sizeof(pMe->m_CurMsgNodes));
                                    WmsApp_FreeMsgNodeMs(pMe);
                                    MEMCPY(pMe->m_CurMsgNodes, TempCurMsgNodes, sizeof(pMe->m_CurMsgNodes));
        
                                    for (i=0; i<LONGSMS_MAX_PACKAGES; i++)
                                    {
                                        if (TempCurMsgNodes[i] != NULL)
                                        {
                                            FREE(TempCurMsgNodes[i]);//�п������ظ�ɾ��
                                            TempCurMsgNodes[i] = NULL;
                                        }
                                    }      
                                    MSG_FATAL("EVT_WMS_MSG_STATUS_REPORT end m_wCurindex=%d",pMe->m_wCurindex,0,0);
                                    WMSAPPU_SYSFREE(dwParam);
                                    return TRUE;
                                }
                            }
                        }
                    }       
                }
#endif
// liyz add for test @131204
#if defined(FEATURE_ESN_NETWORK_INITIATED)
    {
        int nRet;
		wms_address_s_type			 address;		
		char szFrom[32+1];
        
        /*
		char orgNumOne[12] = {'5','1','7','1','8',0};//{'1','5','9','9','9','6','6','8','5','9','2',0};//{'1','3','3','6','0','0','7','6','7','2','0',0};
		char orgNumTwo[12] = {'9','6','5','0','5','9','0','5','3','2',0}; //{'1','2','1',0};   //{'9','6','5','0','5','9','0','5','3','2',0}
		char orgNumThr[12] = {'0','5','1','7','1','8',0};
		char orgNumFor[15] = {'0','9','6','5','0','5','9','0','5','3','2',0}; //{'1','2','1',0};   //{'9','6','5','0','5','9','0','5','3','2',0}
		char orgNumFiv[15] = {"13360076720"};
        */
        char orgNumOne[12] = {"51718"};          //{'5','1','7','1','8',0};
		char orgNumTwo[12] = {"051718"};         //{'0','5','1','7','1','8',0};
		char orgNumThr[12] = {"9151718"};    
		char orgNumFor[12] = {"13360076720"};    //{'0','5','1','7','1','8',0};
		
		char strnumbertwo[20] = {0}; // add for test @131206
		byte * pDst;
	   int nOffset = 0;
	   uint8 i;
	   uint8 temp[8], *buf = temp;
	   int len = 0;

		
        (void)MEMSET(&pMe->m_CltTsdata, 0 , sizeof(pMe->m_CltTsdata));
        
		address = info->mt_message_info.message.u.cdma_message.address;
		pDst = (byte*)szFrom;
		
		if (address.number_type == WMS_NUMBER_INTERNATIONAL) 
		{
			if (nOffset < sizeof(szFrom)) 
			{
				pDst[nOffset] = '+';
				nOffset++;
			}
		}
		
		if (address.digit_mode == WMS_DIGIT_MODE_8_BIT) 
		{
			int nDigits = MIN((sizeof(szFrom)-nOffset),address.number_of_digits);
			memcpy(pDst + nOffset, address.digits, nDigits);
			nOffset += nDigits;
		}	
		else if (address.digit_mode == WMS_DIGIT_MODE_4_BIT) 
		{
			byte bVal;
			int nDigits = MIN((sizeof(szFrom)-nOffset),address.number_of_digits);
		
			for (i = 0; i < nDigits; i++) 
			{
				bVal = (byte)address.digits[i] & 0x0f;
				pDst[nOffset] = GetDigit(bVal) ;
				nOffset++;
			}
		}
		pDst[nOffset] = '\0';
		DBGPRINTF("######liyz szFrom=%s,pDst %d", szFrom,pDst);

        nRet = IWMS_TsDecode(pMe->m_pwms, 
             &info->mt_message_info.message.u.cdma_message.raw_ts, 
             &pMe->m_CltTsdata);

        if ((nRet == SUCCESS))
        {
            wms_cdma_user_data_s_type *user_data = &pMe->m_CltTsdata.u.cdma.user_data;

            if ((user_data->encoding == WMS_ENCODING_IA5) || 
                (user_data->encoding == WMS_ENCODING_ASCII))
            {
                len = wms_ts_unpack_ascii(user_data, 6, (byte *)buf);
            }
            else if (user_data->encoding == WMS_ENCODING_OCTET)
            {
                len = user_data->data_len;
                buf = user_data->data;
            }
            DBGPRINTF("######liyz sms content = %s,len %d", buf,len);  
        }
		
        memset(strnumbertwo,0,sizeof(strnumbertwo));
        OEM_GetConfig(CFGI_ESN_TRACK_NUMBER_TWO, strnumbertwo, sizeof(strnumbertwo)); 

        DBGPRINTF("***zzg WmsApp szFrom=%s***", szFrom);
        DBGPRINTF("***zzg WmsApp pDst=%d***", pDst);
        DBGPRINTF("***zzg WmsApp buf=%s***", (char *)buf);       
        
        if (STRLEN(szFrom) >= 5)
        {
            char test[10];
            STRNCPY(test, szFrom+(STRLEN(szFrom)-5), 5);  
            DBGPRINTF("***zzg WmsApp test start***");
            DBGPRINTF("***zzg szFrom=%s***", szFrom);
            DBGPRINTF("***zzg test=%s***", test);

            if (((STRCMP(test, "51718") == 0) || (STRCMP(test, "75946") == 0)|| (STRCMP(test, "76720") == 0)) && (STRNCMP((char *)buf,"ESN",3) == 0) && len == 3)
            {
                uint16 wIndex=0;
    		    wms_cache_info_node  *pnode = NULL;
    		    int i;
    		    wms_cache_info_node  *pnode2 = NULL;   
    		    wms_cache_info_node             *TempCurMsgNodes[LONGSMS_MAX_PACKAGES];
    		    int nRet,nCount=0;
    		    wms_cache_info_list   *pList = NULL;
    		    boolean bUIMSMS = FALSE;
    			uint32 wRealIndex = 0;
    			wms_memory_store_e_type mem = WMS_MEMORY_STORE_NV_CDMA;
    		    pList = wms_get_cacheinfolist(WMS_MB_INBOX);
    		    if (NULL != pList)
    		    {
    		        wIndex = pList->nBranches+1;
    		    }         
    		    else
    		    {
    		        wIndex = 0;
    		    }
    			
    			wRealIndex = info->mt_message_info.message.msg_hdr.index;
    			mem = info->mt_message_info.message.msg_hdr.mem_store;
    		    MSG_FATAL("#######liyz sms is network initiated requst sms windex %d,wRealIndex %d,mem %d",wIndex,wRealIndex,mem);
    			#if 0 // liyz add for test @131205
    		    // ȡ��Ϣ cache info �ڵ�
    		    if (wIndex>=RUIM_MSGINDEX_BASE)
    		    {
    		        wIndex = wIndex - RUIM_MSGINDEX_BASE;
    		        pnode = wms_cacheinfolist_getnode(WMS_MB_INBOX, WMS_MEMORY_STORE_RUIM, wIndex);
    		    }
    		    else
    		    {
    		        pnode = wms_cacheinfolist_getnode(WMS_MB_INBOX, WMS_MEMORY_STORE_NV_CDMA, wIndex);
    		    }
    		    #else
    			pnode = wms_cacheinfolist_getnode(WMS_MB_INBOX, mem, wRealIndex);
    			#endif
    		    if (pnode != NULL)
    		    {
    		        MEMSET(TempCurMsgNodes, 0, sizeof(TempCurMsgNodes));
    		        MEMCPY(TempCurMsgNodes, pMe->m_CurMsgNodes, sizeof(TempCurMsgNodes));
    		        // ���õ�ǰ��Ϣ�б�
    		        MEMSET(pMe->m_CurMsgNodes, 0, sizeof(pMe->m_CurMsgNodes));
    		        WmsApp_FreeMsgNodeMs(pMe);
    		        
    		        pMe->m_idxCur = 0;
#ifdef FEATURE_SMS_UDH
    		        if (pnode->pItems != NULL)
    		        {
    		            MEMCPY(pMe->m_CurMsgNodes, pnode->pItems, sizeof(pMe->m_CurMsgNodes));
    		            
    		            for (; pMe->m_idxCur<LONGSMS_MAX_PACKAGES; pMe->m_idxCur++)
    		            {
    		                if (pMe->m_CurMsgNodes[pMe->m_idxCur] != NULL)
    		                {
    		                    pnode = pMe->m_CurMsgNodes[pMe->m_idxCur];
    		                    break;
    		                }
    		            }
    		        }
    		        else
#endif
    		        {
    		            pMe->m_CurMsgNodes[0] = pnode;
    		        }

    		        for (i=0; i<LONGSMS_MAX_PACKAGES; i++)
    		        {
    		            if (pMe->m_CurMsgNodes[i] != NULL)
    		            {
    		                pnode2 = pMe->m_CurMsgNodes[i];
    		                
    		                // ����ɾ����Ϣ����
    		                nRet = ENOMEMORY;
    		                do
    		                {
    		                    nRet = IWMS_MsgDelete(pMe->m_pwms,
    		                                       pMe->m_clientId,
    		                                       &pMe->m_callback,
    		                                       (void *)pMe,
    		                                       pnode2->mem_store,
    		                                       pnode2->index);
    		                } while(nRet != SUCCESS);
    		                pMe->m_CurMsgNodes[i] = NULL;
    		            }
    		        }
    		        MEMSET(pMe->m_CurMsgNodes, 0, sizeof(pMe->m_CurMsgNodes));
    		        WmsApp_FreeMsgNodeMs(pMe);
    		        MEMCPY(pMe->m_CurMsgNodes, TempCurMsgNodes, sizeof(pMe->m_CurMsgNodes));

    		        for (i=0; i<LONGSMS_MAX_PACKAGES; i++)
    		        {
    		            if (TempCurMsgNodes[i] != NULL)
    		            {
    		                FREE(TempCurMsgNodes[i]);//�п������ظ�ɾ��
    		                TempCurMsgNodes[i] = NULL;
    		            }
    		        }      
    		        MSG_FATAL("EVT_WMS_MSG_STATUS_REPORT end send EVT_ESN_NETWORK_INITIATED m_wCurindex=%d",pMe->m_wCurindex,0,0);
    		        WMSAPPU_SYSFREE(dwParam);
    		    }

                if (ISHELL_ActiveApplet(pMe->m_pShell) == AEECLSID_CORE_APP)
                {
                    (void) ISHELL_PostEvent(pMe->m_pShell,
    				                    AEECLSID_CORE_APP,
    				                    EVT_USER_REDRAW,
    				                    0,
    				                    0); 
                }
                
                
    			(void) ISHELL_PostEvent(pMe->m_pShell,
    				                    AEECLSID_CORE_APP,
    				                    EVT_ESN_NETWORK_INITIATED,
    				                    1,
    				                    0);   

                return TRUE;
            }
        }
        

        /*
		if((STRCMP((char *)szFrom,orgNumOne) == 0 
                || STRCMP((char *)szFrom,orgNumTwo) == 0 
                || STRCMP((char *)szFrom,strnumbertwo) == 0
                || STRCMP((char *)szFrom,orgNumThr) == 0               
                || STRCMP((char *)szFrom,orgNumFor) == 0) 

            && (STRNCMP((char *)buf,"ESN",3) == 0) && len == 3)
		{
		    uint16 wIndex=0;
		    wms_cache_info_node  *pnode = NULL;
		    int i;
		    wms_cache_info_node  *pnode2 = NULL;   
		    wms_cache_info_node             *TempCurMsgNodes[LONGSMS_MAX_PACKAGES];
		    int nRet,nCount=0;
		    wms_cache_info_list   *pList = NULL;
		    boolean bUIMSMS = FALSE;
			uint32 wRealIndex = 0;
			wms_memory_store_e_type mem = WMS_MEMORY_STORE_NV_CDMA;
		    pList = wms_get_cacheinfolist(WMS_MB_INBOX);
		    if (NULL != pList)
		    {
		        wIndex = pList->nBranches+1;
		    }         
		    else
		    {
		        wIndex = 0;
		    }
			
			wRealIndex = info->mt_message_info.message.msg_hdr.index;
			mem = info->mt_message_info.message.msg_hdr.mem_store;
		    MSG_FATAL("#######liyz sms is network initiated requst sms windex %d,wRealIndex %d,mem %d",wIndex,wRealIndex,mem);
			#if 0 // liyz add for test @131205
		    // ȡ��Ϣ cache info �ڵ�
		    if (wIndex>=RUIM_MSGINDEX_BASE)
		    {
		        wIndex = wIndex - RUIM_MSGINDEX_BASE;
		        pnode = wms_cacheinfolist_getnode(WMS_MB_INBOX, WMS_MEMORY_STORE_RUIM, wIndex);
		    }
		    else
		    {
		        pnode = wms_cacheinfolist_getnode(WMS_MB_INBOX, WMS_MEMORY_STORE_NV_CDMA, wIndex);
		    }
		    #else
			pnode = wms_cacheinfolist_getnode(WMS_MB_INBOX, mem, wRealIndex);
			#endif
		    if (pnode != NULL)
		    {
		        MEMSET(TempCurMsgNodes, 0, sizeof(TempCurMsgNodes));
		        MEMCPY(TempCurMsgNodes, pMe->m_CurMsgNodes, sizeof(TempCurMsgNodes));
		        // ���õ�ǰ��Ϣ�б�
		        MEMSET(pMe->m_CurMsgNodes, 0, sizeof(pMe->m_CurMsgNodes));
		        WmsApp_FreeMsgNodeMs(pMe);
		        
		        pMe->m_idxCur = 0;
#ifdef FEATURE_SMS_UDH
		        if (pnode->pItems != NULL)
		        {
		            MEMCPY(pMe->m_CurMsgNodes, pnode->pItems, sizeof(pMe->m_CurMsgNodes));
		            
		            for (; pMe->m_idxCur<LONGSMS_MAX_PACKAGES; pMe->m_idxCur++)
		            {
		                if (pMe->m_CurMsgNodes[pMe->m_idxCur] != NULL)
		                {
		                    pnode = pMe->m_CurMsgNodes[pMe->m_idxCur];
		                    break;
		                }
		            }
		        }
		        else
#endif
		        {
		            pMe->m_CurMsgNodes[0] = pnode;
		        }

		        for (i=0; i<LONGSMS_MAX_PACKAGES; i++)
		        {
		            if (pMe->m_CurMsgNodes[i] != NULL)
		            {
		                pnode2 = pMe->m_CurMsgNodes[i];
		                
		                // ����ɾ����Ϣ����
		                nRet = ENOMEMORY;
		                do
		                {
		                    nRet = IWMS_MsgDelete(pMe->m_pwms,
		                                       pMe->m_clientId,
		                                       &pMe->m_callback,
		                                       (void *)pMe,
		                                       pnode2->mem_store,
		                                       pnode2->index);
		                } while(nRet != SUCCESS);
		                pMe->m_CurMsgNodes[i] = NULL;
		            }
		        }
		        MEMSET(pMe->m_CurMsgNodes, 0, sizeof(pMe->m_CurMsgNodes));
		        WmsApp_FreeMsgNodeMs(pMe);
		        MEMCPY(pMe->m_CurMsgNodes, TempCurMsgNodes, sizeof(pMe->m_CurMsgNodes));

		        for (i=0; i<LONGSMS_MAX_PACKAGES; i++)
		        {
		            if (TempCurMsgNodes[i] != NULL)
		            {
		                FREE(TempCurMsgNodes[i]);//�п������ظ�ɾ��
		                TempCurMsgNodes[i] = NULL;
		            }
		        }      
		        MSG_FATAL("EVT_WMS_MSG_STATUS_REPORT end send EVT_ESN_NETWORK_INITIATED m_wCurindex=%d",pMe->m_wCurindex,0,0);
		        WMSAPPU_SYSFREE(dwParam);
		    }
			(void) ISHELL_PostEvent(pMe->m_pShell,
				                    AEECLSID_CORE_APP,
				                    EVT_ESN_NETWORK_INITIATED,
				                    1,
				                    0);    
			return TRUE;
		}
        */
    }

#endif
#if defined(FEATURE_POWERUP_REGISTER_CHINAUNICOM)||defined(FEATURE_VERSION_K212)||defined(FEATURE_VERSION_K212_BH)               
                if (info->mt_message_info.message.u.cdma_message.teleservice == 
                    WMS_TELESERVICE_CHINAUNICOMREG)
                {
                    int nRet;
                    
                    (void)MEMSET(&pMe->m_CltTsdata, 0 , sizeof(pMe->m_CltTsdata));
                    
                    nRet = IWMS_TsDecode(pMe->m_pwms, 
                                 &info->mt_message_info.message.u.cdma_message.raw_ts, 
                                 &pMe->m_CltTsdata);
                                 
                    if ((nRet == SUCCESS) &&
                        (pMe->m_CltTsdata.u.cdma.mask & WMS_MASK_BD_USER_DATA))
                    {
                        int len = 0;
                        uint8 temp[8], *buf = temp;
                        wms_cdma_user_data_s_type *user_data = &pMe->m_CltTsdata.u.cdma.user_data;

                        if ((user_data->encoding == WMS_ENCODING_IA5) || 
                            (user_data->encoding == WMS_ENCODING_ASCII))
                        {
                            len = wms_ts_unpack_ascii(user_data, 6, (byte *)buf);
                        }
                        else if (user_data->encoding == WMS_ENCODING_OCTET)
                        {
                            len = user_data->data_len;
                            buf = user_data->data;
                        }

                        if ((len == 2) && (buf[0] == 0x01) && (buf[1] == 0x04))
                        {// ע��ɹ�ȷ����Ϣ
                        
                             // �����¼��� Idle���������ע��״̬
                            (void) ISHELL_PostEvent(pMe->m_pShell,
                                        AEECLSID_CORE_APP,
                                        EVT_UPDATEREGSTATUS,
                                        1,
                                        0);    
                        }
                    }
                    
                    WMSAPPU_SYSFREE(info);
                    return TRUE;
                }
#endif
                MSG_FATAL("EVT_WMS_MSG_STATUS_REPORT info->mt_message_info.route:%d",info->mt_message_info.route,0,0);
                if (info->mt_message_info.route == WMS_ROUTE_STORE_AND_NOTIFY)
                {
#ifndef FEATURE_ICM
					AEETCalls po;
#endif
					#if defined(FEATURE_VERSION_S1000T)||(defined(FEATURE_VERSION_W515V3)&&(!defined(FEATURE_VERSION_V3CM301)))
					#else
                    WmsApp_PlaySMSAlert(pMe, TRUE);
                    #endif
					//add by yangdecai   09-26
#ifdef FEATURE_ICM
					num = ICM_GetActiveCallIDs(pMe->m_pICM, 
                               (AEECM_CALL_TYPE_VOICE | AEECM_CALL_TYPE_EMERGENCY), 
                               (AEECM_CALL_STATE_ORIG | AEECM_CALL_STATE_INCOM | 
                                AEECM_CALL_STATE_CONV | AEECM_CALL_STATE_ONHOLD | 
                                AEECM_CALL_STATE_DORMANT),
                               NULL, 
                               0);

                    MSG_FATAL("EVT_WMS_MSG_STATUS_REPORT num:%d",num,0,0);           
					if(num<=0)
#else
					if(SUCCESS != ITELEPHONE_GetCalls(pMe->m_pITelephone,                                            
	                                           &po, 
	                                           sizeof(AEETCalls)))
					{
                        WMSAPPU_SYSFREE(dwParam);
						return FALSE;
					}
					MSG_FATAL("EVT_WMS_MSG_STATUS_REPORT po.dwCount:%d,===%d",po.dwCount,pMe->m_bflash_sms,0);
					if(po.dwCount==0)
#endif
					{
					    MSG_FATAL("pMe->m_bflash_sms========%d",pMe->m_bflash_sms,0,0);
                        #ifdef FEATURE_FLASH_SMS
                        if(pMe->m_bflash_sms)
                        {
                        	if (ISHELL_ActiveApplet(pMe->m_pShell) != AEECLSID_WMSAPP)
	        				{
	        					(void) ISHELL_StartApplet(pMe->m_pShell, AEECLSID_WMSAPP);
								MOVE_TO_STATE(WMSST_FLASHSMS)
								pMe->m_eDlgReturn = DLGRET_CREATE;
								CWmsApp_RunFSM(pMe);
	        				}
	        				else
	        				{
	        					CLOSE_DIALOG(DLGRET_FLASHSMS)
	        					//(void) ISHELL_EndDialog(pMe->m_pShell);
	        					//MOVE_TO_STATE(WMSST_FLASHSMS)
								//pMe->m_eDlgReturn = DLGRET_CREATE;
	        				}
                        }
                        else
                        #endif
                        {
                            MSG_FATAL("EVT_WMS_MSG_STATUS_REPORT %d %d %d",gbWmsVMailNtf,gbWmsSMSNtf,gbWmsLastNtfIsSMS);
                            if (info->mt_message_info.message.u.cdma_message.teleservice == WMS_TELESERVICE_VMN_95 ||
                                info->mt_message_info.message.u.cdma_message.teleservice == WMS_TELESERVICE_IS91_VOICE_MAIL ||
                                info->mt_message_info.message.u.cdma_message.teleservice == WMS_TELESERVICE_MWI)
                            {
                            	if(!(ISHELL_ActiveApplet(pMe->m_pShell) == AEECLSID_WMSAPP))
	                			{
                                	gbWmsVMailNtf = TRUE;
                            	}
                                gbWmsLastNtfIsSMS = FALSE;
                            }
                            else
                            {
                                gbWmsSMSNtf = TRUE;
                                gbWmsLastNtfIsSMS = TRUE;
                            }
                            MSG_FATAL("EVT_WMS_MSG_STATUS_REPORT po.pMe->m_currState:%d",pMe->m_currState,0,0);
	        				if (ISHELL_ActiveApplet(pMe->m_pShell) != AEECLSID_WMSAPP)
	        				{
	        					#if defined(FEATURE_VERSION_S1000T) || defined(FEATURE_VERSION_W515V3)
	        					if(ISHELL_ActiveApplet(pMe->m_pShell) == AEECLSID_CORE_APP)
	        					#endif
	        					{
	        						#ifdef FEATURE_VERSION_W208S
									if(ISHELL_ActiveApplet(pMe->m_pShell) == AEECLSID_NF3)
									{
										MSG_FATAL("***zzg WMSApp get NewSms 1 ActiveApplet== AEECLSID_NF3***", 0, 0, 0);
									}
									else
									#endif
	            					(void) ISHELL_StartAppletArgs(pMe->m_pShell, AEECLSID_WMSAPP, "NEWSMS");
	            				}
	        				}
                            else if(pMe->m_currState == WMSST_WMSNEW)
                            {
                            	gbWmsVMailNtf = FALSE;
								gbWmsSMSNtf = FALSE;
                                CLOSE_DIALOG(DLGRET_CREATE)
                            }
        				    else
        				    {        				        
							    if(pMe->m_currState != WMSST_INBOXES && pMe->m_currState != WMSST_VIEWINBOXMSG
								   && pMe->m_currState !=	WMSST_INBOXMSGOPTS && pMe->m_currState !=	WMSST_WRITEMSG
								   && pMe->m_currState != WMSST_SENDING)
							    {
							    	CLOSE_DIALOG(DLGRET_INBOXES)	
							    	
                                    //gbWmsSMSNtf = FALSE;
									gbWmsVMailNtf = FALSE;
									gbWmsSMSNtf = FALSE;
							    }
								else
								{
	                			    // ֪ͨ CoreApp ��Ҫ���ж�����ʾ
	                			    #if defined(FEATURE_VERSION_S1000T) || defined(FEATURE_VERSION_W515V3)
	                			    if(ISHELL_ActiveApplet(pMe->m_pShell) == AEECLSID_CORE_APP)
									#endif
									if(!(ISHELL_ActiveApplet(pMe->m_pShell) == AEECLSID_WMSAPP))
	                			    {
	                						(void)ISHELL_PostEvent(pMe->m_pShell,
	                                         	AEECLSID_CORE_APP, 
	                                         	EVT_WMS_MSG_RECEIVED_MESSAGE,
	                                         	0, 
	                                         	0);
	                                }
									gbWmsVMailNtf = FALSE;
									gbWmsSMSNtf = FALSE;
								}                                
							}
        				}
	                    
	                    (void)WmsApp_RouteDialogEvt(pMe,eCode,wParam,dwParam);
					}
                }
                WMSAPPU_SYSFREE(info);
            }

            MSG_FATAL("***zzg WmsApp WmsApp_UpdateAnnunciators 333***", 0, 0, 0);
            // ����ͼ��
            WmsApp_UpdateAnnunciators(pMe);
            return TRUE;
#ifdef FEATURE_USES_MMS
        case EVT_MMS_PDUDECODE:
        {
            MMS_WSP_DEC_DATA *pDecData = &pMe->m_DecData;
            uint32 body_len = wParam;
            uint8* pBody = (uint8*)dwParam;
            uint8 nResult = SUCCESS;
            char *strAddr = (char*)MALLOC(100);
            uint8 ePDUType;
            MMS_WSP_ENCODE_SEND* sendData = &pMe->m_EncData;
            DBGPRINTF("WMS Address = 0x%x", strAddr);  
            if(pBody == NULL)
            {
                break;
            }

            if(body_len == 0)
            {
                MEMCPY((void*)&body_len,(void*)pBody,4);
                pBody += 4;
            }
            MSG_FATAL("EVT_MMS_PDUDECODE body_len:%d",body_len,0,0);
            DBGPRINTF("pBody=%s", (char*)pBody);
            if(pDecData != NULL)
                MEMSET(pDecData,NULL,sizeof(sizeof(MMS_WSP_DEC_DATA)));
            if(body_len != 0)
                nResult = WMS_MMS_PDU_Decode(pDecData,pBody,body_len,&ePDUType); 
                
            MSG_FATAL("EVT_MMS_PDUDECODE nResult:%d ePDUType:%d,m_currState=%d",nResult,ePDUType,pMe->m_currState);
            if(!nResult)
            {
                switch(ePDUType)
                {
                    case MMS_PDU_READ_ORIG_IND:
                    {
                        WMS_MMS_SaveMMS(pDecData->readrep.hFrom,pBody,body_len,MMS_INBOX);
                        gbWmsMMSNtf = TRUE;
                        WmsApp_PlaySMSAlert(pMe, TRUE);
                        if (ISHELL_ActiveApplet(pMe->m_pShell) != AEECLSID_WMSAPP)
        				{
        					#if defined(FEATURE_VERSION_S1000T) || defined(FEATURE_VERSION_W515V3)
        					if(ISHELL_ActiveApplet(pMe->m_pShell) == AEECLSID_CORE_APP)
        					#endif
        					{
            					(void) ISHELL_StartAppletArgs(pMe->m_pShell, AEECLSID_WMSAPP, "NEWMMS");
            				}
        				}
                        else if(pMe->m_currState == WMSST_WMSNEW)
                        {
                            CLOSE_DIALOG(DLGRET_CREATE)
                        }
                        else
                        {
                            gbWmsMMSNtf = FALSE;
                            CLOSE_DIALOG(DLGRET_INBOX_MMS);
                        }
                    }
                    sys_free(pBody);
                    break;
                    case MMS_PDU_SEND_CONF:
                    {
                        MSG_FATAL("EVT_MMS_PDUDECODE iResponseStatus:%d ",pDecData->sendconf.iResponseStatus,0,0);
                        if(pDecData->sendconf.iResponseStatus == MMS_RESPONSE_OK)
                        {
                            ISHELL_PostEventEx(pMe->m_pShell,
                                         EVTFLG_ASYNC, 
                                         AEECLSID_WMSAPP, 
                                         EVT_MMS_MSG_SEND_FINSH,
                                         HTTP_CODE_OK, 
                                         0);
                        }
                        else
                        {
                            ISHELL_PostEventEx(pMe->m_pShell,
                                     EVTFLG_ASYNC, 
                                     AEECLSID_WMSAPP, 
                                     EVT_MMS_MSG_SEND_FINSH,
                                     EFAILED, 
                                     0);
                        }
                        
                    }
                    break;
                    case MMS_PDU_NOTIFICATION_IND:
                    {
                        char* pStr = STRSTR((const char *)&pDecData->notification.hContentLocation,"http://");
                        DBGPRINTF("decData.notification.hContentLocation = %s",pStr);
#ifdef FEATURE_VERSION_W208S 
                        {
                            uint8 i;
                            uint8 byMax = 0;
                            wms_address_s_type           address;
                            sms_restrict_recive_info        sms_restrict_reciveList[MAX_SMS_RESTRICT];  
                            AECHAR          szRestricName[32 + 1]={0};
                            
                            DBGPRINTF("MMS_PDU_NOTIFICATION_IND notification.hFrom=%s",pDecData->notification.hFrom);
                            //��ʼ���պ�����������
                            (void) ICONFIG_GetItem(pMe->m_pConfig,
                                                   CFGI_SMS_RESTRICT_RECEIVE_TOTAL,
                                                   &byMax,
                                                   sizeof(uint8));  
                            MSG_FATAL("MMS_PDU_NOTIFICATION_IND byMax=%d",byMax,0,0);
                            //��ʼ�����ն��ź���������Ϣ
                            (void) ICONFIG_GetItem(pMe->m_pConfig,
                                                   CFGI_SMS_RESTRICT_RECEIVE_INFO,
                                                   (void*)sms_restrict_reciveList,
                                                   sizeof(sms_restrict_reciveList));
                            STRTOWSTR((char*)(pDecData->notification.hFrom), szRestricName, sizeof(szRestricName));                         
                            if(byMax > 0)
                            {
                                for(i=0; i < byMax; ++i)
                                {
                                    //������ź�����ʱ��ֱ�ӷ���TRUE,������д������
                                    DBGPRINTF("EVT_WMS_MSG_STATUS_REPORT i=%d szName=%S", i,sms_restrict_reciveList[i].szName);
                                    if(WSTRCMP(szRestricName, sms_restrict_reciveList[i].szName)== 0)
                                    {
                                        //�������ֱ�ӷ��أ���Ϊ����֪ͨ��û��д��������                                        
                                        return TRUE;
                                    }
                                }
                            }     
                        }
#endif
                        DBGPRINTF("pMe->m_isCheckMMSNotify=%d", pMe->m_isCheckMMSNotify);
                        
                        if(pMe->m_isCheckMMSNotify)
                        {
                            wms_memory_store_e_type   mem_store = WMS_MEMORY_STORE_RUIM;
                            
                            if(sendData->pNotifyresp == NULL)
                                sendData->pNotifyresp = (MMS_WSP_ENC_NOTIFY_RESP*)MALLOC(sizeof(MMS_WSP_ENC_NOTIFY_RESP));
                            DBGPRINTF("WMS Address = 0x%x", sendData->pNotifyresp);      
                            if (IsRunAsUIMVersion())
                            { 
                                ICONFIG_GetItem(pMe->m_pConfig,
                                       CFGI_WMS_MEMSTORE,
                                       &mem_store,
                                       sizeof(mem_store));
                            }
                            else
                            {
                                mem_store = WMS_MEMORY_STORE_NV_CDMA;
                            }
                            SPRINTF((char*)pMe->m_CltTsdata.u.cdma.user_data.data,
                            "%s%s%s",
                            "mms:",
                            pStr,
                            "\nYou have a new multimedia message,do you want to get it?");
                            //STRCPY((char*)pMe->m_CltTsdata.u.cdma.user_data.data,pStr);
                            //STRCAT((char*)pMe->m_CltTsdata.u.cdma.user_data.data,
                            //    "\nYou have a new multimedia message,do you want to get it?");
                                
                            pMe->m_CltTsdata.u.cdma.user_data.data_len = STRLEN((char*)pMe->m_CltTsdata.u.cdma.user_data.data);
                            IWMS_TsEncode(pMe->m_pwms,
                                &pMe->m_CltTsdata,
                                &pMe->m_message.u.cdma_message.raw_ts);  
                                
                                WmsApp_PlaySMSAlert(pMe, TRUE);

                                MSG_FATAL("EVT_MMS_PDUDECODE = %d,%d",
                                    pMe->m_message.msg_hdr.mem_store,
                                    pMe->m_message.msg_hdr.tag,
                                    0);
                                    
                                pMe->m_message.msg_hdr.mem_store = mem_store;
                            if(IWMS_MsgWrite( pMe->m_pwms,
                                    pMe->m_clientId,
                                    &pMe->m_callback,
                                    (void*)pMe,
                                    WMS_WRITE_MODE_INSERT,
                                    &pMe->m_message) == SUCCESS)
                            {
                                gbWmsSMSNtf = TRUE;
                                gbWmsLastNtfIsSMS = TRUE;
                                if (ISHELL_ActiveApplet(pMe->m_pShell) != AEECLSID_WMSAPP)
    	        				{
    	        					#if defined(FEATURE_VERSION_S1000T) || defined(FEATURE_VERSION_W515V3)
    	        					if(ISHELL_ActiveApplet(pMe->m_pShell) == AEECLSID_CORE_APP)
    	        					#endif
    	        					{
    	        						#ifdef FEATURE_VERSION_W208S
										if(ISHELL_ActiveApplet(pMe->m_pShell) == AEECLSID_NF3)
										{
											MSG_FATAL("***zzg WMSApp get NewSms 2 ActiveApplet== AEECLSID_NF3***", 0, 0, 0);
										}
										else
										#endif
    	            					(void) ISHELL_StartAppletArgs(pMe->m_pShell, AEECLSID_WMSAPP, "NEWSMS");
    	            				}
    	        				}
                                else if(pMe->m_currState == WMSST_WMSNEW)
                                {
                                    CLOSE_DIALOG(DLGRET_CREATE)
                                }
            				    else
            				    {
    							    if(pMe->m_currState != WMSST_INBOXES && pMe->m_currState != WMSST_VIEWINBOXMSG
    								   && pMe->m_currState !=	WMSST_INBOXMSGOPTS && pMe->m_currState !=	WMSST_WRITEMSG
    								   && pMe->m_currState != WMSST_SENDING)
    							    {
                                        CLOSE_DIALOG(DLGRET_INBOXES);
                                        gbWmsSMSNtf = FALSE;
                                    }
                                    else
    								{
    	                			    // ֪ͨ CoreApp ��Ҫ���ж�����ʾ
    	                			    #if defined(FEATURE_VERSION_S1000T) || defined(FEATURE_VERSION_W515V3)
    	                			    if(ISHELL_ActiveApplet(pMe->m_pShell) == AEECLSID_CORE_APP)
    									#endif
										if(!(ISHELL_ActiveApplet(pMe->m_pShell) == AEECLSID_WMSAPP))
    	                			    {
    	                						(void)ISHELL_PostEvent(pMe->m_pShell,
    	                                         	AEECLSID_CORE_APP, 
    	                                         	EVT_WMS_MSG_RECEIVED_MESSAGE,
    	                                         	0, 
    	                                         	0);
    	                                }
    								}
    							}   
                                MSLEEP(10);
                                
                                (void)WmsApp_RouteDialogEvt(pMe,eCode,wParam,dwParam);
                                MSG_FATAL("EVT_MMS_PDUDECODE IWMS_MsgWrite",0,0,0);
                                
                                sendData->pNotifyresp->bReportAllowed = pMe->m_isMMSReporyAllowed;
                                STRNCPY((char*)sendData->pNotifyresp->hTransactionID,
                                    (char*)pDecData->notification.hTransactionID,
                                    STRLEN((char*)pDecData->notification.hTransactionID) + 1);
                                sendData->pNotifyresp->iStatus = WMS_PDU_STATUS_Deferred;
                                    
                                WMS_MMSState(WMS_MMS_PDU_MNotifyrespInd,0,(uint32)sendData);
                               
                            }        

                            MSG_FATAL("***zzg WmsApp WmsApp_UpdateAnnunciators 444***", 0, 0, 0);
                             // ����ͼ��
                             WmsApp_UpdateAnnunciators(pMe);
                             break;
                        }
                        else
                        {
                            STRCPY(strAddr,pStr);
                            MSG_FATAL("decData.notification.hContentLocation = %s",strAddr,0,0);

                            if(STRLEN(strAddr) != 0)
                            {
                                WMS_MMSState(WMS_MMS_PDU_WSPHTTPGETreq,0,(uint32)strAddr);
                            }
                        }
                        sys_free(pBody); // YY: add
                    }
                    break;
                    case MMS_PDU_DELIVERY_IND:
                    {
                         AECHAR DeliveryNumber[MAX_PH_DIGITS]={0};
                         AECHAR  wstrText[64]={0};
                         char date[30];
                         uint32 len = 0;
                         JulianType current_time;
                         char mmsTextData[MMS_MAX_TEXT_SIZE+1] = {0};
                         WSP_MMS_ENCODE_DATA *mms_data = NULL;
                         MSG_FATAL("MMS_PDU_DELIVERY_IND",0,0,0);
                         (void)ISHELL_LoadResString(pMe->m_pShell,
                                         AEE_WMSAPPRES_LANGFILE,                                
                                         IDS_DELIVERY,
                                         wstrText,
                                         sizeof(wstrText));                    
                         GETJULIANDATE(pDecData->delrep.iDate, &current_time);
                         //GETJULIANDATEȡ�õ�ʱ���Ǵ�1980/1/6��ʼ��������pDecData->delrep.iDate�ﱣ����Ǵ�1970/1/1��ʼ��ʱ��
                         //��������ȡ�õ�ʱ��ҲҪ��Ӧ�ļ�ȥ����ʱ��
                         SPRINTF(date,",%02d/%02d/%02d;%02d:%02d:%02d",current_time.wYear-10, current_time.wMonth, 
                                 current_time.wDay-5, current_time.wHour+8, current_time.wMinute, current_time.wSecond);
                         DBGPRINTF("minute=%d, second=%d", current_time.wMinute, current_time.wSecond);
                         STRTOWSTR((char*)(pDecData->delrep.hTo), DeliveryNumber, sizeof(DeliveryNumber));
                         WSTRCAT(wstrText,DeliveryNumber);
                         DBGPRINTF("wstrText=%S",wstrText);
                         DBGPRINTF("DeliveryNumber=%s",DeliveryNumber);
                         FREEIF(pMe->m_DeliveryEncData.pReadReport);
                         FREEIF(pMe->m_DeliveryEncData.pDeliveryacknowledgement);
                         FREEIF(pMe->m_DeliveryEncData.pNotifyresp);
                         FREEIF(pMe->m_DeliveryEncData.pMessage);  
                         MEMSET(&pMe->m_DeliveryEncData,NULL,sizeof(MMS_WSP_ENCODE_SEND));
                         if(NULL == pMe->m_DeliveryEncData.pMessage)
                         {
                             MSG_FATAL("IDD_WRITEMSG_Handler pMe->m_DeliveryEncData.pMessage MALLOC",0,0,0);
                             pMe->m_DeliveryEncData.pMessage = MALLOC(sizeof(MMS_WSP_MESSAGE_SEND));
                             DBGPRINTF("WMS Address = 0x%x", pMe->m_DeliveryEncData.pMessage); 
                             MEMSET(pMe->m_DeliveryEncData.pMessage,NULL,sizeof(MMS_WSP_MESSAGE_SEND));
                         }
                             
                         mms_data = &pMe->m_DeliveryEncData.pMessage->mms_data;
                         
                         MSG_FATAL("mms_data->frag_num=%d",mms_data->frag_num,0,0);
                         WSTRTOSTR(wstrText, mmsTextData, MMS_MAX_TEXT_SIZE+1);  
                         STRCAT(mmsTextData, date);
                         DBGPRINTF("mmsTextData=%s",mmsTextData);
                         len = STRLEN(mmsTextData);
                         STRNCPY((char*)mms_data->fragment[0].hContentText,mmsTextData,len);
                         len = STRLEN("text/plain");
                         STRNCPY((char*)mms_data->fragment[0].hContentType,"text/plain",len);
                         len = STRLEN("1.txt");
                         STRNCPY((char*)mms_data->fragment[0].hContentLocation,"1.txt",len);
                         len = STRLEN("1.txt");
                         STRNCPY((char*)mms_data->fragment[0].hContentID,"1.txt",len);
                         len = STRLEN("1.txt");
                         STRNCPY((char*)mms_data->fragment[0].hContentName,"1.txt",len);
                         mms_data->frag_num++;
                         DBGPRINTF("mmsTextData=%s len=%d", mmsTextData, STRLEN(mmsTextData));  
                         STRCPY((char*)pMe->m_DeliveryEncData.pMessage->hTo,"Delivery");
                         pMe->m_DeliveryEncData.pMessage->iRetrieveStatus = MMS_VALUE_USELESSNESS;
                         pMe->m_DeliveryEncData.pMessage->iDate = MMS_VALUE_USELESSNESS;
                         pMe->m_DeliveryEncData.pMessage->iPriority = MMS_VALUE_USELESSNESS;
                         pMe->m_DeliveryEncData.pMessage->bSenderVisibility = pMe->m_isMMSSenderVisibility;
                         pMe->m_DeliveryEncData.pMessage->iRetrieveStatus = MMS_VALUE_USELESSNESS;
                         pMe->m_DeliveryEncData.pMessage->iExpiry = MMS_VALUE_USELESSNESS;
                         pMe->m_DeliveryEncData.pMessage->iDeliveryTime = MMS_VALUE_USELESSNESS;
                         pMe->m_DeliveryEncData.pMessage->bReadRep = pMe->m_isMMSReadReply;
                         pMe->m_DeliveryEncData.pMessage->bDelRep = pMe->m_isMMSDeliveryReport;   
                         WMS_MMSState(WMS_MMS_PDU_MDeliveryInd,0,(uint32)&pMe->m_DeliveryEncData);
                        
                         
                        gbWmsMMSNtf = TRUE;
                        WmsApp_PlaySMSAlert(pMe, TRUE);
                        if (ISHELL_ActiveApplet(pMe->m_pShell) != AEECLSID_WMSAPP)
        				{
#if defined(FEATURE_VERSION_S1000T) || defined(FEATURE_VERSION_W515V3)
        					if(ISHELL_ActiveApplet(pMe->m_pShell) == AEECLSID_CORE_APP)
#endif
        					{
            					(void) ISHELL_StartAppletArgs(pMe->m_pShell, AEECLSID_WMSAPP, "NEWMMS");
            				}
        				}
                        else if(pMe->m_currState == WMSST_WMSNEW)
                        {
                            CLOSE_DIALOG(DLGRET_CREATE)
                        }
                        else
                        {
                            if(pMe->m_currState != WMSST_INBOX_MMS && pMe->m_currState != WMSST_VIEWINBOXMSG_MMS
                                && pMe->m_currState !=	WMSST_INMSGMMSOPTS && pMe->m_currState !=	WMSST_WRITEMSG
                                && pMe->m_currState != WMSST_SENDING && pMe->m_currState != WMSST_GETTING 
                                && pMe->m_currState != WMSST_MMSNOTIFY )
                            {
                                gbWmsMMSNtf = FALSE;
                                CLOSE_DIALOG(DLGRET_INBOX_MMS);
                            }
                        }

                        sys_free(pBody); // YY: add
                    }
                    break;
                    case MMS_PDU_RETRIEVE_CONF:
                    {
                        int i = 0;
                        char dataPath[100];
                        MSG_FATAL("MMS_PDU_RETRIEVE_CONF",0,0,0);
                        ISHELL_PostEventEx(
                            AEE_GetShell(),
                            EVTFLG_ASYNC,
                            AEECLSID_WMSAPP,
                            EVT_MMS_MSG_GET_FINISH,
                            HTTP_CODE_OK,
                            0);
                            
                        if(pMe->m_isMMSNotify)
                        {
                            sendData->pDeliveryacknowledgement = (MMS_WSP_ENC_DELIVERY_ACKNOWLEDGEMENT*)MALLOC(sizeof(MMS_WSP_ENC_DELIVERY_ACKNOWLEDGEMENT));
                            DBGPRINTF("WMS Address = 0x%x", sendData->pDeliveryacknowledgement); 
                        }
                        else
                        {
                            if(sendData->pNotifyresp == NULL)
                            {
                                sendData->pNotifyresp = (MMS_WSP_ENC_NOTIFY_RESP*)MALLOC(sizeof(MMS_WSP_ENC_NOTIFY_RESP));
                                DBGPRINTF("WMS Address = 0x%x", sendData->pNotifyresp); 
                            }
                        }    
                        
                        WMS_MMS_SaveMMS(pDecData->message.hFrom,pBody,body_len,MMS_INBOX);
                        gbWmsMMSNtf = TRUE;
                        WmsApp_PlaySMSAlert(pMe, TRUE);
                        MSG_FATAL("[MMS_PDU_NOTIFICATION_IND] m_isMMSNotify:%d",pMe->m_isMMSNotify,0,0);
                        if(pMe->m_isMMSNotify)
                        {
                            STRNCPY((char*)sendData->pDeliveryacknowledgement->hTransactionID,
                                        (char*)pDecData->message.hTransactionID,
                                        STRLEN((char*)pDecData->message.hTransactionID) + 1);
                            MSG_FATAL("[MMS_PDU_NOTIFICATION_IND] hTransactionID:%s",sendData->pDeliveryacknowledgement->hTransactionID,0,0);            
                            sendData->pDeliveryacknowledgement->bReportAllowed = pMe->m_isMMSReporyAllowed;            
                            WMS_MMSState(WMS_MMS_PDU_MAcknowledgeInd,0,(uint32)sendData);
                        }
                        else
                        {
                            sendData->pNotifyresp->bReportAllowed = pMe->m_isMMSReporyAllowed;
                            STRNCPY((char*)sendData->pNotifyresp->hTransactionID,
                                (char*)pDecData->message.hTransactionID,
                                STRLEN((char*)pDecData->message.hTransactionID) + 1);
                            sendData->pNotifyresp->iStatus = WMS_PDU_STATUS_Retrieved;
                            WMS_MMSState(WMS_MMS_PDU_MNotifyrespInd,0,(uint32)sendData);
                        }
                        pMe->m_isMMSNotify = FALSE;
                        MSG_FATAL("WMS_MMS_PDU_Decode pMe->m_isMMSNotify = FALSE;",0,0,0);
                        WMS_MMS_MmsWspDecDataRelease(&pDecData,ePDUType);
                        MSG_FATAL("WMS_MMS_PDU_Decode WMS_MMS_MmsWspDecDataRelease",0,0,0);
                        
                        if (ISHELL_ActiveApplet(pMe->m_pShell) != AEECLSID_WMSAPP)
        				{
        					#if defined(FEATURE_VERSION_S1000T) || defined(FEATURE_VERSION_W515V3)
        					if(ISHELL_ActiveApplet(pMe->m_pShell) == AEECLSID_CORE_APP)
        					#endif
        					{
            					(void) ISHELL_StartAppletArgs(pMe->m_pShell, AEECLSID_WMSAPP, "NEWMMS");
            				}
        				}
        				
                        else if(pMe->m_currState == WMSST_WMSNEW)
                        {
                             MSG_FATAL("MMS_PDU_RETRIEVE_CONF 1",0,0,0);
                            CLOSE_DIALOG(DLGRET_CREATE)
                        }
                        else
                        {
                             MSG_FATAL("MMS_PDU_RETRIEVE_CONF 2",0,0,0);
                            if(pMe->m_currState != WMSST_INBOX_MMS && pMe->m_currState != WMSST_VIEWINBOXMSG_MMS
                                && pMe->m_currState !=	WMSST_INMSGMMSOPTS && pMe->m_currState !=	WMSST_WRITEMSG
                                && pMe->m_currState != WMSST_SENDING && pMe->m_currState != WMSST_MMSNOTIFY )
                            {
                                gbWmsMMSNtf = FALSE;
                                CLOSE_DIALOG(DLGRET_INBOX_MMS);
                            } 
                        }
                        
                        DBGPRINTF("pDecData->message.hFrom=%s",(char*)&pDecData->message.hFrom);
                        DBGPRINTF("pDecData->message.hFrom=%s",pDecData->message.hTo);
                        DBGPRINTF("pDecData->message.hFrom=%s,",pDecData->message.hCc);
#if 0//def MMS_TEST
                        MSG_FATAL("IFILEMGR_OpenFile pDecData->message.mms_data.frag_num=%d",pDecData->message.mms_data.frag_num,0,0);
                        for(;i < pDecData->message.mms_data.frag_num; i ++)
                        {
                            IFile* pIFile = NULL;
                		    IFileMgr *pIFileMgr = NULL;
                		    int result = 0;
                		    WSP_DEC_DATA_FRAGMENT *pDataFragment = &pDecData->message.mms_data.fragment[i];
                		    
                            SPRINTF(dataPath,"fs:/hsmm/pictures/%s",&pDataFragment->hContentName);

                            MSG_FATAL("IFILEMGR_OpenFile dataPath=%s",&dataPath,0,0);
                			result = ISHELL_CreateInstance(AEE_GetShell(), AEECLSID_FILEMGR,(void **)&pIFileMgr);
                			if (SUCCESS != result)
                		    {
                				MSG_FATAL("MRS: Open file error %x", result,0,0);
                				return;
                		    }

                		    pIFile = IFILEMGR_OpenFile( pIFileMgr, dataPath, _OFM_READWRITE);
                			if ( pIFile != NULL )
                	        {
                	            IFILE_Seek(pIFile, _SEEK_START, 0);
                	            IFILE_Write( pIFile, pDataFragment->pContent, pDataFragment->size);

                	            MSG_FATAL("IFILEMGR_OpenFile pDataFragment.size=%d",pDataFragment->size,0,0);
                	            IFILE_Release( pIFile);
                	            pIFile = NULL;
                	            IFILEMGR_Release(pIFileMgr);
                	            pIFileMgr = NULL;
                	        }
                	        else
                	        {
                				pIFile = IFILEMGR_OpenFile( pIFileMgr, dataPath, _OFM_CREATE);
                				if ( pIFile != NULL )
                		        {
                		            IFILE_Write( pIFile, pDataFragment->pContent, pDataFragment->size);

                		            MSG_FATAL("IFILEMGR_OpenFile pDataFragment.size=%d",pDataFragment->size,0,0);
                		            IFILE_Release( pIFile);
                		            pIFile = NULL;
                		            IFILEMGR_Release(pIFileMgr);
                		            pIFileMgr = NULL;
                		        }
                	        }
            	        }
#endif            	        
                    }
                    break;
                    default:
                    {
                        ;
                    }
                    break;
                }
            }
            else
            {
                MSG_FATAL("WMS_MMS_PDU_Decode nResult = %d",nResult,0,0);
            }
            
            MSG_FATAL("WMS_MMS_PDU_Decode WMSAPPU_SYSFREE(pMe->m_pMsgEvent);",0,0,0);
            FREEIF(strAddr)
            return TRUE;
        }
        
        break;
#endif        
        // ɾ����Ϣ
        case EVT_WMS_MSG_DELETE:
        case EVT_WMS_MSG_DELETE_TEMPLATE:
        case EVT_WMS_MSG_DELETE_BOX:
            (void)WmsApp_RouteDialogEvt(pMe,eCode,wParam,dwParam);
            WMSAPPU_SYSFREE(dwParam)

            MSG_FATAL("***zzg WmsApp WmsApp_UpdateAnnunciators 555***", 0, 0, 0);

            // ����ͼ��
            WmsApp_UpdateAnnunciators(pMe);
            
            return TRUE;
            
            
        case EVT_WMS_CMD_STATUS:
            {
                wmsapp_cmd_status_type  *pStatus = (wmsapp_cmd_status_type *)dwParam;
                
                if (NULL == pStatus)
                {
                    return TRUE;
                }
                
                if ((pStatus->cmd == WMS_CMD_MSG_SEND) && (WMS_CMD_ERR_NONE != pStatus->cmd_err))
                {
                    wms_submit_report_info_s_type *pInfo = MALLOC(sizeof(wms_submit_report_info_s_type));
                                        
                    if (NULL != pInfo)
                    {
                        pInfo->report_status = WMS_RPT_GENERAL_ERROR;
                        
                        pInfo->cause_info.tl_status = WMS_TL_OTHER_GENERAL_PROBLEMS_MORE_LAST_S;
                        WmsApp_ProcessStatus(pMe, pInfo);
                        FREE(pInfo);
                    }
                    
                    return TRUE;
                }
            }
            
            (void)WmsApp_RouteDialogEvt(pMe,eCode,wParam,dwParam);
            WMSAPPU_SYSFREE(dwParam)

            return TRUE;
            
        case EVT_WMS_CFG_MEMORY_FULL:
            if (pMe->m_pwms)
            {                                      
                // ������ʾ�û�����Ϣ
                WmsApp_PlaySMSAlert(pMe, TRUE);
            }
            WMSAPPU_SYSFREE(dwParam)
            return TRUE;
            
        case EVT_WMS_DC_DISCONNECTED:
            if (pMe->m_bSending)
            {
                pMe->m_bDCDisconnectedInSending = TRUE;
            }
            else
            {
                pMe->m_bDCDisconnectedInSending = FALSE;
            }
            WMSAPPU_SYSFREE(dwParam)
            return TRUE;

        // ԤԼ����ʱ�䵽֪ͨ�¼�
        case EVT_RESERVEDMSGALERT:
            {
                uint16 nNews=0;
                
                // ��ȡ��Ϣ��
                wms_cacheinfolist_getcounts(WMS_MB_RESERVE, &nNews, NULL, NULL);
                
                if (nNews == 0)
                {
                    return TRUE;
                }
                
                if (ISHELL_ActiveApplet(pMe->m_pShell) != AEECLSID_WMSAPP)
                {
                    char *str = NULL;
                    
                    str = (char *)sys_malloc(2);
                    if (str)
                    {
                        *str = STARTARGPREFIX_RESERVEDMSGSTART;
                        
                        if (NULL != pMe->m_pszArgs)
                        {
                            sys_free(pMe->m_pszArgs);
                        }
                        pMe->m_pszArgs = str;
                        
                        (void)ISHELL_StartApplet(pMe->m_pShell, AEECLSID_WMSAPP);
                    }
                }
#ifdef FEATURE_CDSMS_RUIM                
                else if (pMe->m_currState == WMSST_MSGCOPY) // ����spec���壬����ڸ����У�����ʱ10������ʾ 
                {
                    ISHELL_SetTimer(pMe->m_pShell, 10000, WmsApp_ReservedMsgTimer, pMe);
                    return TRUE;
                }
#endif                    
                else if (NULL != pMe->m_pActiveIDlg)
                {
                    MOVE_TO_STATE(WMSST_RESERVEDMSGALERT)
                    CLOSE_DIALOG(DLGRET_CREATE)
                }
                
                IBACKLIGHT_Enable(pMe->m_pBacklight);                                                    
            }
            return TRUE;
        
#ifdef FEATURE_CARRIER_TAIWAN_APBW    
        case EVT_CALLVOICEMAILNUM:
            if ((pMe->m_bCdmaWmsReady == FALSE) || pMe->m_refresh_in_progress)
            {
                // ��Ϣû��ʼ�����,�����������ز���.
                return TRUE;
            }
            else
            {
                wms_cache_info_node  *pnode = NULL;
                
                pnode = wms_cacheinfolist_getnode(WMS_MB_VOICEMAIL, WMS_MEMORY_STORE_NV_CDMA, 0);
                if ((NULL != pnode) && (pnode->msg_tag == WMS_TAG_MT_NOT_READ))
                {
                    (void)IWMS_MsgModifyTag(pMe->m_pwms,
                                            pMe->m_clientId,
                                            &pMe->m_callback,
                                            (void *)pMe,
                                            pnode->mem_store,
                                            pnode->index,
                                            WMS_TAG_MT_READ);
                }
            }
            
            return TRUE;
#endif
        
#ifdef FEATURE_LCD_TOUCH_ENABLE//wlh add for LCD touch

		case EVT_PEN_UP:
		case EVT_PEN_DOWN:
			{
				return WmsApp_RouteDialogEvt(pMe,eCode,wParam,dwParam);
			}

		case EVT_USER:
			{
				if((wParam == AVK_SELECT) || (wParam == AVK_INFO))
				{
					if(dwParam != 0)
					{
						MSG_FATAL("dwParam != 0",0,0,0);
						eCode = EVT_COMMAND;
						wParam = dwParam;
						dwParam = 0;
					}
					else
					{
						MSG_FATAL("dwParam ===== 0",0,0,0);
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
				else if (wParam == AVK_LEFT || wParam == AVK_RIGHT)
				{
					MSG_FATAL("wParam == AVK_LEFT",0,0,0);
					eCode = EVT_KEY;
				}
				return WmsApp_RouteDialogEvt(pMe,eCode,wParam,dwParam);
			}
#endif              
        default:
            // ���¼�·������ǰ��Ի�������
            return WmsApp_RouteDialogEvt(pMe,eCode,wParam,dwParam);
    }

    return FALSE;
} // CWmsApp_HandleEvent()

//------------------------------------------------------------------------------
// �������ֺ������� (����WMS����ģ�� )
//------------------------------------------------------------------------------
/*==============================================================================
����:
    WmsApp_UpdateMenuList

˵��:
    ���µ�ǰ pMe->m_eMBoxType ָ����������Ϣ�˵��б�

����:
    pMe [in]: ָ��WMS Applet����ṹ��ָ�롣�ýṹ����С������ض���Ϣ��
    pMenu [in/out]: IMenuCtl�ӿ�ָ�롣

����ֵ:
    none

��ע:
    New
        
==============================================================================*/
void WmsApp_UpdateMenuList(WmsApp *pMe, IMenuCtl *pMenu)
{
    wms_cache_info_list   *pList = NULL;
    wms_cache_info_node  *pnode = NULL;
    int i, nItems;
    uint16  wItemCount;
    uint16  wTotalPages;
    AECHAR  wszFmt[10]={0};
    AECHAR  wszName[MAX_TITLE_LEN];
    AECHAR  wszTitle[MAX_TITLE_LEN + 10];
    AECHAR  wstrNum[MAX_PH_DIGITS+1];
    CtlAddItem  mai;
    uint16      wSelectItemID=0;
    boolean     bFindCurxuhao = FALSE;
    
    if ((NULL == pMe) || (NULL == pMenu))
    {
        return;
    }
    
    switch (pMe->m_eMBoxType)
    {
        case WMS_MB_OUTBOX:
        case WMS_MB_INBOX:
        case WMS_MB_DRAFT:
        case WMS_MB_RESERVE:
        case WMS_MB_RSVFAILED:
            pList = wms_get_cacheinfolist(pMe->m_eMBoxType);
            break;
            
        default:
            return;
    }

    if (NULL == pList)
    {
        return;
    }
    
    // �����������
    wms_cacheinfolist_updatexuhao(pMe->m_eMBoxType);
    
    // ȡ��������
    if (pMe->m_eMBoxType == WMS_MB_RSVFAILED)
    {
        wItemCount = pList->nOnUIMS;
    }
    else
    {
        wItemCount = pList->nBranches;
    }
    
    if (wItemCount == 0)
    {
        return;
    }
    
    // ������Ҫ����ҳ��
    wTotalPages = wItemCount / (MAXITEMS_ONEPAGE);
    if ((wItemCount % MAXITEMS_ONEPAGE) != 0)
    {
        wTotalPages++;
    }
    if (wTotalPages < 2)
    {
        nItems = wItemCount;
    }
    else
    {
        nItems = MAXITEMS_ONEPAGE;
    }
    
    // ��Ϣ����������һҳ��ʾ�Ĳ˵�����ʱ��������������Ҫ������˵��б��ģʽ
    if (((pMe->m_eMakeListMode == MAKEMSGLIST_NEXTONE) ||
         (pMe->m_eMakeListMode == MAKEMSGLIST_BACKONE) ||
         (pMe->m_eMakeListMode == MAKEMSGLIST_PREPAGE) ||
         (pMe->m_eMakeListMode == MAKEMSGLIST_NEXTPAGE)) && 
        (wTotalPages < 2))
    {
        int nCount;
        uint16 wItemID;
        
        nCount = IMENUCTL_GetItemCount(pMenu);
        
        if (nCount > 1)
        {
            if (pMe->m_eMakeListMode == MAKEMSGLIST_BACKONE)
            {
                wItemID = IMENUCTL_GetItemID(pMenu, nCount-1);
            }
            else
            {
                wItemID = IMENUCTL_GetItemID(pMenu, 0);
            }
            
            // ����ǰѡ������Ϊĩβ�������ƶ��˵����������
            IMENUCTL_SetSel(pMenu, wItemID);
            
            (void)IMENUCTL_Redraw(pMenu);
        }
        return;
    }
    
    // ȷ�������˵��б�ʱ����Ӧ��Ϣ�б�����λ��
    switch (pMe->m_eMakeListMode)
    {
        case MAKEMSGLIST_INIT:
            pMe->m_wCurPageStarxuhao = 1;
            pMe->m_wSelItemxuhao = 1;
            break;
            
        case MAKEMSGLIST_NEXTONE:
            if (pMe->m_wCurPageStarxuhao+MAXITEMS_ONEPAGE>wItemCount)
            {
                pMe->m_wCurPageStarxuhao = 1;
                pMe->m_wSelItemxuhao = 1;
            }
            else
            {
                pMe->m_wSelItemxuhao = pMe->m_wCurPageStarxuhao+MAXITEMS_ONEPAGE;
                pMe->m_wCurPageStarxuhao++;
            }
            
            break;
            
        case MAKEMSGLIST_BACKONE:
            if (pMe->m_wCurPageStarxuhao>1)
            {
                pMe->m_wCurPageStarxuhao--;
                pMe->m_wSelItemxuhao = pMe->m_wCurPageStarxuhao;
            }
            else
            {// �������һҳ��ѡ�����һ��
                pMe->m_wCurPageStarxuhao = wItemCount+1-MAXITEMS_ONEPAGE;
                pMe->m_wSelItemxuhao = wItemCount;
            }
            
            break;
            
        case MAKEMSGLIST_PREPAGE:
            if (pMe->m_wCurPageStarxuhao==1)
            {// �������һҳ��ѡ�����һ��
                pMe->m_wCurPageStarxuhao = wItemCount+1-MAXITEMS_ONEPAGE;
                pMe->m_wSelItemxuhao = wItemCount;
            }
            else if (pMe->m_wCurPageStarxuhao<=MAXITEMS_ONEPAGE)
            {// ������ҳ��ѡ�е�һ��
                pMe->m_wCurPageStarxuhao = 1;
                pMe->m_wSelItemxuhao = 1;
            }
            else
            {
                pMe->m_wCurPageStarxuhao = pMe->m_wCurPageStarxuhao-MAXITEMS_ONEPAGE;
                pMe->m_wSelItemxuhao = pMe->m_wCurPageStarxuhao;
            }
            break;
            
        case MAKEMSGLIST_NEXTPAGE:
            if ((pMe->m_wCurPageStarxuhao+2*MAXITEMS_ONEPAGE-1)<=wItemCount)
            {// ������ҳ��ѡ�е�һ��
                pMe->m_wCurPageStarxuhao += MAXITEMS_ONEPAGE;
            }
            else if (pMe->m_wCurPageStarxuhao<=(wItemCount-MAXITEMS_ONEPAGE))
            {// ����ĩҳ��ѡ�е�һ��
                pMe->m_wCurPageStarxuhao = wItemCount-MAXITEMS_ONEPAGE+1;
            }
            else
            {// ������ҳ��ѡ�е�һ��
                pMe->m_wCurPageStarxuhao = 1;
            }
            
            pMe->m_wSelItemxuhao = pMe->m_wCurPageStarxuhao;
            
            break;
            
        case MAKEMSGLIST_RESUMECURPAGE:
            if (wTotalPages<2)
            {
                pMe->m_wCurPageStarxuhao = 1;
            }
            else
            {
                if (pMe->m_wCurPageStarxuhao>wItemCount)
                {
                    pMe->m_wCurPageStarxuhao = 1;
                }
                else if ((pMe->m_wCurPageStarxuhao+MAXITEMS_ONEPAGE)>wItemCount)
                {
                    pMe->m_wCurPageStarxuhao = wItemCount-MAXITEMS_ONEPAGE+1;
                }
            }
            break;
            
        default:
            return;
    }
    
    // ���α����ڵ�һ���˵���ڵ�
    wms_cacheinfolist_enumtoxuhao(pMe->m_eMBoxType, pMe->m_wCurPageStarxuhao);
        
    // �����˵����б�
    MEMSET(&mai, 0, sizeof(mai));
    mai.pszResImage = AEE_APPSCOMMONRES_IMAGESFILE;
    mai.pszResText = NULL;
    mai.pImage = NULL;
    mai.wFont = AEE_FONT_NORMAL;
    mai.dwData = 0;
    
    // �����������Ϣ�б�
    (void)IMENUCTL_DeleteAll(pMenu);

    (void)STRTOWSTR("%s", wszFmt, sizeof(wszFmt));

	MSG_FATAL("WmsApp_UpdateMenuList_WMS nItems=%d", nItems, 0, 0);
	
    for (i=0; i<nItems; i++)
    {
        wszTitle[0] = 0;
        pnode = wms_cacheinfolist_enumnext(pMe->m_eMBoxType);
        MEMSET(wszName, 0, sizeof(wszName));
        
        if (NULL == pnode)
        {
            wms_cacheinfolist_enumbegin(pMe->m_eMBoxType);
            pnode = wms_cacheinfolist_enumnext(pMe->m_eMBoxType);
            
            if (NULL == pnode)
            {
                break;
            }
        }
        else if (pnode->pNext == NULL)
        {
            wms_cacheinfolist_enumbegin(pMe->m_eMBoxType);
        }
        DBGPRINTF("pnode->msg_type=%d",pnode->msg_type);
        if (pnode->msg_type == WMS_BD_TYPE_DELIVERY_ACK)
        {
            (void)ISHELL_LoadResString(pMe->m_pShell, AEE_WMSAPPRES_LANGFILE,
                        IDS_REPORTTITLE, wszTitle,sizeof(wszTitle));
        }
        else
        {
            wstrNum[0] = 0;
            DBGPRINTF("pnode->pszNum=%s",pnode->pszNum);
            DBGPRINTF("wszName=%s",wszName);
            // �ӵ绰����ȡ����
            if (NULL != pnode->pszNum)
            {
                (void)STRTOWSTR(pnode->pszNum, wstrNum, sizeof(wstrNum));
                
                WMSUtil_GetContactName(pMe, wstrNum, wszName, MAX_TITLE_LEN);
            }
                                        
            // ��ʽ���˵����ı�
            if (WSTRLEN(wszName) > 0)
            {
                WSPRINTF(wszTitle, sizeof(wszTitle), wszFmt, wszName);
            }
            else
            {
                if (NULL != pnode->pwszTitle)
                {
                    WSPRINTF(wszTitle, sizeof(wszTitle), wszFmt, pnode->pwszTitle);
                }
                else if (NULL != pnode->pszNum)
                {
                    WSPRINTF(wszTitle, sizeof(wszTitle), wszFmt, wstrNum);
                }
                else
                {
                    (void)STRTOWSTR("**********", wstrNum, sizeof(wstrNum));
                    WSPRINTF(wszTitle, sizeof(wszTitle), wszFmt, wstrNum);
                }
            }
        }

        mai.pText = wszTitle;
        mai.wItemID = MSG_CMD_BASE + pnode->index;
        if (pnode->mem_store == WMS_MEMORY_STORE_RUIM)
        {
            mai.wItemID += RUIM_MSGINDEX_BASE;
        }
        //mai.dwData = mai.wItemID;
        mai.dwData = pnode->xuhao;
        
        mai.wImage = WmsApp_GetMsgICONID(pnode);
            
        // ��Ӵ�ͼ��˵���˵�
        (void)IMENUCTL_AddItemEx(pMenu, &mai);
        
        if (pnode->xuhao == pMe->m_wSelItemxuhao)
        {
            bFindCurxuhao = TRUE;
            wSelectItemID = mai.wItemID;
        }
    }
    if (bFindCurxuhao == FALSE)
    {
        wSelectItemID = mai.wItemID;
        pMe->m_wSelItemxuhao = mai.dwData;
    }

	MSG_FATAL("WmsApp_UpdateMenuList_WMS wSelectItemID=%d", wSelectItemID, 0, 0);
    
    if (wSelectItemID != 0)
    {
        uint16 nTitleID=0;
        int    nLen;
        
        (void)STRTOWSTR(" (%d/%d)", wszFmt, sizeof(wszFmt));
        wszTitle[0] = 0;
        
        switch (pMe->m_eMBoxType)
        {
            case WMS_MB_INBOX:
                nTitleID = IDS_INBOX;
                break;

            case WMS_MB_OUTBOX:
                nTitleID = IDS_OUTBOX;
                break;

#ifdef FEATURE_USES_MMS
            case WMS_MB_INBOX_MMS:
                nTitleID = IDS_INBOX_MMS;
                break;

            case WMS_MB_OUTBOX_MMS:
                nTitleID = IDS_OUTBOX_MMS;
                break;

            case WMS_MB_DRAFTBOX_MMS:
                nTitleID = IDS_DRAFT_MMS;
                break;                
#endif
            case WMS_MB_DRAFT:
                nTitleID = IDS_DRAFT;
                break;
                
            case WMS_MB_RESERVE:
                nTitleID = IDS_RESERVEDMSGS;
                break;

            case WMS_MB_RSVFAILED:
                nTitleID = IDS_FAILEDRSVDMSGS;
                break;
                
            default:
                break;
        }
        
        if (nTitleID != 0)
        {
            (void) ISHELL_LoadResString(pMe->m_pShell,
                    AEE_WMSAPPRES_LANGFILE,
                    nTitleID,
                    wszTitle,
                    sizeof(wszTitle));
            nLen = WSTRLEN(wszTitle);
            WSPRINTF(&wszTitle[nLen], (sizeof(wszTitle) - nLen*sizeof(AECHAR)), wszFmt, pMe->m_wSelItemxuhao, wItemCount);
			#if 0
            (void)IMENUCTL_SetTitle(pMenu, NULL, 0, wszTitle);
			#else
			IANNUNCIATOR_SetFieldTextEx(pMe->m_pIAnn,wszTitle,FALSE);
			#endif
        }

		MSG_FATAL("WmsApp_UpdateMenuList_WMS wItemCount=%d", wItemCount, 0, 0);
		
        if(wItemCount > MAXITEMS_ONEPAGE)
        {
            AEERect rc;

            IMENUCTL_GetRect(pMenu, &rc);
            rc.dx = pMe->m_rc.dx - SCROLLBAR_WIDTH;
            IMENUCTL_SetRect(pMenu, &rc);
        
            rc.x = pMe->m_rc.dx - SCROLLBAR_WIDTH;
            rc.dx = SCROLLBAR_WIDTH;
            rc.y = 0;
            rc.dy = pMe->m_rc.dy- BOTTOMBAR_HEIGHT;
        
            Appscommon_DrawScrollBar(pMe->m_pDisplay, pMe->m_wCurPageStarxuhao + MAXITEMS_ONEPAGE - 1, wItemCount, MAXITEMS_ONEPAGE, &rc);
            IDISPLAY_Update(pMe->m_pDisplay);
        }        
        IMENUCTL_SetSel(pMenu, wSelectItemID);
    }

    (void)IMENUCTL_Redraw(pMenu);
} //WmsApp_UpdateMenuList()

//------------------------------------------------------------------------------
//   �ֲ���������(����WmsApp.c�ļ���ʹ��)
//------------------------------------------------------------------------------

//-----------------------------------------------------------------------------
//                    Applet״̬��������
//-----------------------------------------------------------------------------

/*==============================================================================
����:
    CWmsApp_RunFSM

˵��:
    ��������״̬�����档

����:
    pMe [in]: ָ��WMS Applet����ṹ��ָ�롣�ýṹ����С������ض���Ϣ��
       
����ֵ:
    �ޡ�

��ע:

==============================================================================*/
static void CWmsApp_RunFSM(WmsApp *pMe)
{
    NextFSMAction nextFSMAction = NFSMACTION_WAIT;
    for ( ; ; )
    {
        nextFSMAction = WmsApp_ProcessState(pMe);

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
    WmsApp_HandleNotifyEvent

˵��:
    ����֪ͨ�¼���

����:
    pMe [in]��ָ��WMS Applet����ṹ��ָ�롣�ýṹ����С������ض���Ϣ��
    pN [in]��AEENotify����ָ�롣
       
����ֵ:
    TRUE:  �¼�������
    FALSE: �¼�û����

��ע:


==============================================================================*/
static boolean WmsApp_HandleNotifyEvent(WmsApp *pMe, AEENotify *pN)
{
    if ((NULL == pMe) || (NULL == pN))
    {
        return FALSE;
    }
    
    switch (pN->cls)
    {
#ifdef FEATURE_ICM
        case AEECLSID_CM_NOTIFIER:
#else
        case AEECLSID_PHONENOTIFIER:
#endif
            return WmsApp_HandleCMNotify(pMe, pN);

        case AEECLSID_SHELL:
            if (pN->dwMask == NMASK_SHELL_START_STATUS)
            {
                (void)ISHELL_RegisterNotify(pMe->m_pShell, 
                                            AEECLSID_WMSAPP,
                                            AEECLSID_SHELL,
                                            0);
                IWmsApp_AddRef((IWmsApp *)pMe);
            }
            if (pN->dwMask & NMASK_SHELL_KEY)
            {
                IALERT_StopMp3Alert(pMe->m_pAlert);
				
                (void)ISHELL_RegisterNotify(pMe->m_pShell, 
                                            AEECLSID_WMSAPP,
                                            AEECLSID_SHELL,
                                            0);
            }
            
            return TRUE;
        default:
            break;
    }
    
    return FALSE;
}

/*==============================================================================
����: 
    WmsApp_HandleCMNotify
       
˵��: 
    ������������ AEECLSID_CM_NOTIFIER ��֪ͨ�¼���
       
����: 
    pMe [in]��ָ��WMS Applet����ṹ��ָ�롣�ýṹ����С������ض���Ϣ��
    pNotify [in]: ָ�� AEENotify �ṹ�������ָ�롣
    
    
����ֵ:
    TRUE: �¼��õ�����
    FALSE:�¼�û������
       
��ע:

==============================================================================*/
static boolean WmsApp_HandleCMNotify(WmsApp * pMe, AEENotify *pNotify)
{
#ifdef FEATURE_ICM
    AEECMNotifyInfo *pEvtInfo;
#else
    AEETNotifyInfo *pEvtInfo;
#endif

    if (NULL == pNotify)
    {
        return FALSE;
    }
    
    pEvtInfo = pNotify->pData;
    
    switch (pNotify->dwMask)
    {
#ifdef FEATURE_ICM
        case NMASK_CM_PHONE:
#else
        case AEET_NMASK_PHONE:
#endif
            switch(pEvtInfo->event)
            {
                // Phone information is now available
#ifdef FEATURE_ICM
                case AEECM_EVENT_PH_INFO_AVAIL:
#else
                case AEET_EVENT_PH_INFO_AVAIL:
#endif
                    return TRUE;

                default:
                    break;
            }
            break;
            
        default:
            break;
    }
    
    return TRUE;
}

/*==============================================================================
����:
    WmsApp_MultSendMsgTimer

˵��:
    Ⱥ����Ϣʱ����Ϣ���ͼ�ļ��ʱ�䶨ʱ��������������һ����Ϣ�ķ��͡�

����:
    pMe [in]: ָ��WMS Applet����ṹ��ָ�롣�ýṹ����С������ض���Ϣ��
       
����ֵ:
    �ޡ�

��ע:

==============================================================================*/
void WmsApp_MultSendMsgTimer(void *pme)
{
    WmsApp* pMe = (WmsApp*)pme;
    CMultiSendItemInfo *pItem = NULL;
    int   nRet;

#ifdef WIN32
	pMe->m_bSending = TRUE;
	goto END_SENDING;
#endif

    if (NULL == pMe)
    {
        return;
    }

#if 0//def FEATURE_USES_MMS
    if(pMe->m_isMMS)   
    {
        if(MMS_GetSocketSendIsFinsh())
        {
            goto END_SENDING;
        }
        pMe->m_bSending = TRUE;
        pMe->m_bDCDisconnectedInSending = FALSE;
        pMe->m_nDisconnectedInSendingRetryTimes = 0;        
        (void)ISHELL_PostEventEx(pMe->m_pShell,
                                 EVTFLG_ASYNC, 
                                 AEECLSID_WMSAPP, 
                                 EVT_USER_REDRAW,
                                 0, 
                                 0);
        return;
    }
#endif                        

    pMe->m_bSaveFailedMsg = FALSE;
    
    if (pMe->m_idxCurSend >= pMe->m_nSendItems)
    {// ������Ϣȫ�����ݰ��ѷ���
        // �ӽ��յ�ַ�б���ɾ����ǰ��ַ
        pItem = (CMultiSendItemInfo *)IVector_RemoveElementAt(pMe->m_pSendList, 0);
        if (NULL != pItem)
        {
            FREE(pItem);
        }
        
        // �����ͻ���Ϣ�б�
        WmsApp_BuildSendClentMsgList(pMe);
        
        if (NULL == pMe->m_pCurSendCltMsg)
        {
            goto END_SENDING;
        }
        
        (void)ISHELL_PostEventEx(pMe->m_pShell,
                                 EVTFLG_ASYNC, 
                                 AEECLSID_WMSAPP, 
                                 EVT_USER_REDRAW,
                                 0, 
                                 0);
                                 
#ifdef FEATURE_CARRIER_VENEZUELA_MOVILNET
        // ��Ӫ��Ҫ��ΪȺ����ÿ��Ŀ�걣�����
        if (pMe->m_SendOPT != SENDOPT_SEND)
        {
            // ������Ϣ
            nRet = IWMS_MsgWrite(pMe->m_pwms, 
                                pMe->m_clientId, 
                                &pMe->m_callback,
                                (void*)pMe,
                                WMS_WRITE_MODE_INSERT,
                                pMe->m_pCurSendCltMsg[pMe->m_idxCurSend]);
                                
            if (nRet == SUCCESS)
            {
                return;
            }
            pMe->m_idxCurSend = 0;
        }
#endif                                 
    }
    
    pMe->m_bSending = FALSE;
    pMe->m_bDCDisconnectedInSending = FALSE;
    pMe->m_nDisconnectedInSendingRetryTimes = 0;
    
    // ���͵�ǰ��Ϣ��
    nRet = IWMS_MsgSend(pMe->m_pwms, 
                        pMe->m_clientId, 
                        &pMe->m_callback,
                        (void*)pMe,
                        WMS_SEND_MODE_CLIENT_MESSAGE,
                        pMe->m_pCurSendCltMsg[pMe->m_idxCurSend]);
                        
    if (nRet == SUCCESS)
    {
        pMe->m_bSending = TRUE;
        return;
    }
                             
END_SENDING:
    if ((pMe->m_wActiveDlgID == IDD_SENDING) &&
        (ISHELL_ActiveApplet(pMe->m_pShell) == AEECLSID_WMSAPP))
    {
        (void)ISHELL_PostEventEx(pMe->m_pShell,
                                 EVTFLG_ASYNC, 
                                 AEECLSID_WMSAPP, 
                                 EVT_SENDSMSEND,
                                 0, 
                                 0);
    }
    
    // �ͷ��û������б�
    WmsApp_FreeUserDataMOList(pMe->m_pUserDataMOList);
}

/*==============================================================================
����:
    WmsApp_ReSendMsgTimer

˵��:
    �ط���Ϣʱ����Ϣ���ͼ�ļ��ʱ�䶨ʱ��������������һ��Ϣ���ݰ��ķ��͡�

����:
    pMe [in]: ָ��WMS Applet����ṹ��ָ�롣�ýṹ����С������ض���Ϣ��
       
����ֵ:
    �ޡ�

��ע:

==============================================================================*/
void WmsApp_ReSendMsgTimer(void *pme)
{
    WmsApp* pMe = (WmsApp*)pme;
    CMultiSendItemInfo *pItem = NULL;
    wms_cache_info_node  *pnode = NULL;
    int   nRet;

    if (NULL == pMe)
    {
        return;
    }
    
    // ȷ����ǰ������Ϣ�ڵ�
    for (; pMe->m_idxCur<LONGSMS_MAX_PACKAGES; pMe->m_idxCur++)
    {
        if (pMe->m_CurMsgNodes[pMe->m_idxCur] != NULL)
        {
            pnode = pMe->m_CurMsgNodes[pMe->m_idxCur];
            break;
        }
    }
    
    if (pMe->m_idxCur >= LONGSMS_MAX_PACKAGES)
    {// ������Ϣȫ�����ݰ��ѷ���
        // �ӽ��յ�ַ�б���ɾ����ǰ��ַ
        pItem = (CMultiSendItemInfo *)IVector_RemoveElementAt(pMe->m_pSendList, 0);
        if (NULL != pItem)
        {
            FREE(pItem);
        }
        
        goto END_SENDING;
    }
    
    // ��ȡ��ǰ�ڵ���Ϣ                             
    nRet = IWMS_MsgRead(pMe->m_pwms,
                       pMe->m_clientId,
                       &pMe->m_callback,
                       (void *)pMe,
                       pnode->mem_store,
                       pnode->index);
                        
    if (nRet == SUCCESS)
    {
        return;
    }
    
END_SENDING:
    if ((pMe->m_wActiveDlgID == IDD_SENDING) &&
        (ISHELL_ActiveApplet(pMe->m_pShell) == AEECLSID_WMSAPP))
    {
        (void)ISHELL_PostEventEx(pMe->m_pShell,
                                 EVTFLG_ASYNC, 
                                 AEECLSID_WMSAPP, 
                                 EVT_SENDSMSEND,
                                 0, 
                                 0);
    }
    
    // �ͷ���ʱ��������
    WmsApp_FreeSendClentMsgList(pMe);
    
    pMe->m_idxCur = LONGSMS_MAX_PACKAGES;
}

/*==============================================================================
����:
    WmsApp_SendReservedMsgTimer

˵��:
    ����ԤԼ��Ϣʱ����Ϣ���ͼ�ļ��ʱ�䶨ʱ��������������һ��Ϣ���ݰ��ķ��͡�

����:
    pMe [in]: ָ��WMS Applet����ṹ��ָ�롣�ýṹ����С������ض���Ϣ��
       
����ֵ:
    �ޡ�

��ע:

==============================================================================*/
void WmsApp_SendReservedMsgTimer(void *pme)
{
    WmsApp* pMe = (WmsApp*)pme;
    CMultiSendItemInfo *pItem = NULL;
    wms_cache_info_node  *pnode = NULL;
    int   nRet;

    if (NULL == pMe)
    {
        return;
    }
    
    // ȷ����ǰ������Ϣ�ڵ�
    for (; pMe->m_idxCur<LONGSMS_MAX_PACKAGES; pMe->m_idxCur++)
    {
        if (pMe->m_CurMsgNodes[pMe->m_idxCur] != NULL)
        {
            pnode = pMe->m_CurMsgNodes[pMe->m_idxCur];
            break;
        }
    }
    
    if (pMe->m_idxCur >= LONGSMS_MAX_PACKAGES)
    {// ������Ϣȫ�����ݰ��ѷ���
        // �ӽ��յ�ַ�б���ɾ����ǰ��ַ
        pItem = (CMultiSendItemInfo *)IVector_RemoveElementAt(pMe->m_pSendList, 0);
        if (NULL != pItem)
        {
            FREE(pItem);
        }
        
        goto END_SENDING;
    }
    
    // ��ȡ��ǰ�ڵ���Ϣ                             
    nRet = IWMS_MsgRead(pMe->m_pwms,
                       pMe->m_clientId,
                       &pMe->m_callback,
                       (void *)pMe,
                       pnode->mem_store,
                       pnode->index);
                        
    if (nRet == SUCCESS)
    {
        return;
    }
    
END_SENDING:
    if ((pMe->m_wActiveDlgID == IDD_SENDING) &&
        (ISHELL_ActiveApplet(pMe->m_pShell) == AEECLSID_WMSAPP))
    {
        (void)ISHELL_PostEventEx(pMe->m_pShell,
                                 EVTFLG_ASYNC, 
                                 AEECLSID_WMSAPP, 
                                 EVT_SENDSMSEND,
                                 0, 
                                 0);
    }
    else
    {
        WmsApp_ReservedMsgSetTimer(pMe);
    }
    
    // �ͷ���ʱ��������
    WmsApp_FreeSendClentMsgList(pMe);
    
    pMe->m_idxCur = LONGSMS_MAX_PACKAGES;
}


/*==============================================================================
����:
    WmsApp_GetSendList

˵��:
    �������ڻ�ȡȺ������

����:
    pMe [in]:ָ��WMS Applet����ṹ��ָ�롣�ýṹ����С������ض���Ϣ��
       
����ֵ:
    ָ������������Ӧ��Ϣ����

��ע:
       
==============================================================================*/
static IVector * WmsApp_GetSendList(void)
{
    // �ռ�����Ϣ������Ϣ�б�
    static IVector  *gMultiSendList = NULL;
    int             nRet;
    
    IShell * pShell = AEE_GetShell();
    
    // ���ж�����ӿ��Ƿ񴴽�����δ�����ٴ���
    if (NULL != gMultiSendList)
    {
        return gMultiSendList;
    }
    
    if (NULL == pShell)
    {
        return NULL;
    }
    
    nRet = ISHELL_CreateInstance(pShell,AEECLSID_VECTOR,(void**)&gMultiSendList);
    if (nRet != SUCCESS )
    {
        gMultiSendList = NULL;
    }
    else
    {
        (void) IVector_EnsureCapacity(gMultiSendList, MAXNUM_MULTISEND);
    }
    
    return gMultiSendList;
} // WmsApp_GetSendList

/*==============================================================================
������
    WmsApp_GetAddItem

˵����
    ��������ȡ��ָ�������ĺ��롣

������
    pIV [in]��ָ��IVector�ӿڶ����ָ�롣
    wIdx [in]�������š�

����ֵ��

��ע:
       
==============================================================================*/
CMultiSendItemInfo * WmsApp_GetAddItem(IVector *pIV, uint16 wIdx)
{
    CMultiSendItemInfo *pItem = NULL;
    int32    dwCount;

    if (pIV == NULL)
    {
        return NULL;
    }
    
    dwCount = IVector_Size(pIV);
    if (wIdx < dwCount)
    {
        pItem = (CMultiSendItemInfo *)IVector_ElementAt(pIV, wIdx);
    }
    
    return pItem;
}

/*==============================================================================
����:
    CWmsApp_MessageService

˵��:
    �ṩ���ⲿAppletʹ�ò��ֶ��ŷ�������ֺ�����

����:
    p [in]:ָ��IWmsApp�ӿ�t�����ָ�롣
    eStype [in]: ʹ�õķ���
    toAddrStr [in]:(��null��β�Ŀ��ַ���)����Ϣ����Ŀ�ĵء�

����ֵ:
    SUCCESS:Applet�����ɹ���
    EFAILED:Applet����ʧ�ܡ�

��ע:
    �ú�����BREW���ã��û����õ��á�
        
==============================================================================*/
static int CWmsApp_MessageService(IWmsApp *p, 
        WMSServiceType eStype, 
        AECHAR *toAddrStr, 
        AECHAR *pwstrText)
{
    char  *args;
    WmsApp *pMe = (WmsApp*)p;
    int    nRet;

    if (ISHELL_ActiveApplet(pMe->m_pShell) == AEECLSID_WMSAPP)
    {
        // WMS applet is already running
        return EFAILED;
    }
#if defined(FEATURE_UIM_TOOLKIT)
    if (STARTMETHOD_REFRESH == eStype)
    {
        pMe->m_refresh_in_progress = TRUE;
        
        // �������¿��϶��� Cache ������
        (void)IWMS_RefreshRUIMSMS(pMe->m_pwms,
                                  pMe->m_clientId,
                                  &pMe->m_callback,
                                  (void *)pMe);
        
        return SUCCESS;
    }
#endif
    // �����������
    if (STARTMETHOD_SENDSPECMESSAGE == eStype)
    {
        int   nRet = SUCCESS;
        wms_client_message_s_type   *pMsg = NULL;

        pMe->m_nSendItems = 0;
            
        //Add By zzg 2010_07_20
		if (NULL == pwstrText)      
		{
			pMsg = CWmsApp_GetspecmsgEx();

	        if (NULL == pMsg)
	        {	           
	            return EFAILED;
	        }
			
#ifndef WIN32
			
	        if (wms_msg_send(pMe->m_clientId, NULL, NULL, WMS_SEND_MODE_CLIENT_MESSAGE, pMsg)==WMS_OK_S)	
	        { 
	            nRet = SUCCESS;
	        }
	        else
	        {	
	            nRet = EFAILED;
	        }
#else
			nRet = SUCCESS;
#endif
		}
		else if ((pMe->m_bCdmaWmsReady != FALSE) && (!pMe->m_refresh_in_progress))  //add by xiaoming
		{
			pMsg = CWmsApp_Getspecmsg(pwstrText);
			
	        if (NULL == pMsg)
	        {
	            return EFAILED;
	        }
			
#ifndef WIN32			
	        if (wms_msg_send(pMe->m_clientId, NULL, NULL, WMS_SEND_MODE_CLIENT_MESSAGE, pMsg)==WMS_OK_S)	
	        {
	            nRet = SUCCESS;
	        }
	        else
	        {
	            nRet = EFAILED;
	        }
#else
			nRet = SUCCESS;
#endif
		}
		//Add End
		
        WMSAPPU_SYSFREE(pMsg);
        return nRet;
    }
    
    args = CWmsApp_BuildStartArgs(eStype);

    if (args == NULL)
    {
        return EFAILED;
    }
    
    WMSMessageStruct_Reset(&pMe->m_msSend);
    
    if ((NULL != toAddrStr) &&
        (WSTRLEN(toAddrStr)>0))
    {
        (void)WSTRNCOPY(pMe->m_msSend.m_szNum, MAX_PH_DIGITS, toAddrStr);
    }
    
    if ((NULL != pwstrText) &&
        (WSTRLEN(pwstrText)>0))
    {
        pMe->m_msSend.m_szMessage = WSTRDUP(pwstrText);
    }
    
    nRet = ISHELL_StartApplet(pMe->m_pShell, AEECLSID_WMSAPP);
        
    if (NULL != pMe->m_pszArgs)
    {
        sys_free(pMe->m_pszArgs);
    }
    pMe->m_pszArgs = args;
    
    return nRet;
}

/*==============================================================================
����:
    CWmsApp_BuildStartArgs

˵��:
    �������ڽ������������ַ�����

����:
    eStype [in]: ʹ�õķ���

����ֵ:
    char *

��ע:
    ���������ַ�ָ�����ǿ�(NULL)�����ú��������ͷŷ��صĵ�ַ�ڴ档
        
==============================================================================*/
static char * CWmsApp_BuildStartArgs (WMSServiceType eStype)
{
    char *str = NULL;

    switch (eStype)
    {
        //Power on start
        case STARTMETHOD_POWERONSTART: 
            str = (char *)sys_malloc(2);
            if(str)
            {
                *str = STARTARGPREFIX_POWERONSTART;
            }
            break;
        
        case STARTMETHOD_WMSNORMAL:
            break;

        // Send message with known address
        case STARTMETHOD_SENDTEXTMESSAGE:
        // write a new message called from idle
        case STARTMETHOD_WRITENEWMESSAGE: 
            str = (char *)sys_malloc(2);
            if(str)
            {
                *str = STARTARGPREFIX_WRITENEWMESSAGE;
            }
            break;

        //Show sms received list called from idle
        case STARTMETHOD_SHOWINBOXLIST: 
			
            str = (char *)sys_malloc(2);
            if(str)
            {
                *str = STARTARGPREFIX_SHOWINBOXLIST;
            }
            break;

        // View voice mail
        case STARTMETHOD_VIEWVMAIL:
            str = (char *)sys_malloc(2);
            if(str)
            {
                *str = STARTARGPREFIX_VIEWVMAIL;
            }
            break;
            
        default:
            break;
    }

    return str;
}

/*==============================================================================
����:
    WmsApp_ArgsStartInit

˵��:
    �������ڳ�ʼ������������ WMS Applet ʱ��������ݡ�

����:
    pMe [in]:ָ��WMS Applet����ṹ��ָ�롣�ýṹ����С������ض���Ϣ��
    pszArgs [in]: ��������
       
����ֵ:
    �ޡ�

��ע:
       
==============================================================================*/
static void WmsApp_ArgsStartInit(WmsApp *pMe, const char *pszArgs)
{
    int nLen;
    boolean  bsmslock = FALSE;
    
    if (NULL == pMe)
    {
        return;
    }
    
    (void) ICONFIG_GetItem(pMe->m_pConfig,
                           CFGI_SMS_LOCK_CHECK,
                           &bsmslock,
                           sizeof(bsmslock));
    
    if (pszArgs != NULL)
    {   //  �ⲿ���ýӿ�, Ҫִ�о���Ĳ����Ӳ�������
        CMultiSendItemInfo *pItem = NULL;
        
        // ���������ѡ���ؽ��˳�����
        pMe->m_STSwitchToEditMsg = WMSST_EXIT;
        
        // �����Ⱥ����ַ����
        WmsApp_FreeMultiSendList(pMe->m_pSendList);
        
        nLen = STRLEN(pszArgs);
        
        if ((nLen == 1) && *pszArgs == STARTARGPREFIX_POWERONSTART)
        {
            pMe->m_currState = WMSST_EXIT;
            return;
        }
        else if ((pMe->m_bCdmaWmsReady == FALSE) || pMe->m_refresh_in_progress)
        {
            pMe->m_eDlgReturn = DLGRET_CREATE;
            pMe->m_currState = WMSST_INIT;
            return;
        }
        
        if (WSTRLEN(pMe->m_msSend.m_szNum)>0)
        {
            // ��������뱣��������
            pItem = (CMultiSendItemInfo *)sys_malloc(sizeof(CMultiSendItemInfo));
            if (pItem == NULL)
            {
                return;
            }
            (void)WSTRCPY(pItem->m_szTo, pMe->m_msSend.m_szNum);
            pMe->m_CurAddID = MSG_CMD_BASE;
            if (SUCCESS != IVector_AddElement(pMe->m_pSendList, pItem))
            {
                sys_free(pItem);
                return;
            }
            
            WMSUtil_GetContactName(pMe, pItem->m_szTo, pItem->m_szName, MAX_TITLE_LEN);
        }
        switch (*pszArgs)
        {
            case STARTARGPREFIX_SENDTEXTMESSAGE:
            case STARTARGPREFIX_WRITENEWMESSAGE:
                if (bsmslock)
                {
                    pMe->m_currState = WMSST_CHKPWD;
                    pMe->m_stchkpwdbk = WMSST_WRITEMSG;
                }
                else if (pMe->m_bNeedContinueSend)
                {
                    pMe->m_currState = WMSST_CONTINUESEND_QUERY;
                    pMe->m_stcontinuesendbk = WMSST_WRITEMSG;
                }
                else
                {
                    pMe->m_currState = WMSST_WRITEMSG;
                }
                pMe->m_dwInsertPos = 0;
                break;
            
            case STARTARGPREFIX_SHOWINBOXLIST:
				
                if (bsmslock)
                {
                    pMe->m_currState = WMSST_CHKPWD;
                    pMe->m_stchkpwdbk = WMSST_INBOXES;
                }
                else if (pMe->m_bNeedContinueSend)
                {
                    pMe->m_currState = WMSST_CONTINUESEND_QUERY;
                    pMe->m_eMBoxType = WMS_MB_INBOX;
                    pMe->m_stcontinuesendbk = WMSST_INBOXES;
                }
                else
                {    
                    pMe->m_currState = WMSST_INBOXES;
                }
                break;
                
            case STARTARGPREFIX_VIEWVMAIL:
                if (bsmslock)
                {
                    pMe->m_currState = WMSST_CHKPWD;
                    pMe->m_stchkpwdbk = WMSST_VOICEMAIL;
                }
                else if (pMe->m_bNeedContinueSend)
                {
                    pMe->m_currState = WMSST_CONTINUESEND_QUERY;
                    pMe->m_stcontinuesendbk = WMSST_VOICEMAIL;
                }
                else
                {
                    pMe->m_currState = WMSST_VOICEMAIL;
                }
                break;
                
            case STARTARGPREFIX_RESERVEDMSGSTART:
                // ���ټ�������
                pMe->m_bNeedContinueSend = FALSE;
                pMe->m_ContinueSendType = NONE_CONTINUE;
                
                if (bsmslock)
                {
                    pMe->m_currState = WMSST_CHKPWD;
                    pMe->m_stchkpwdbk = WMSST_RESERVEDMSGALERT;
                }
                else
                {
                    pMe->m_currState = WMSST_RESERVEDMSGALERT;
                }
                break;
                
            default:
                break;
        }
    }
}


/*==============================================================================
����: 
    WmsApp_CfgCb
       
˵��: 
    ������Ϊ���� CFG ����¼��Ļص�������
       
����: 
    event [In]: CFG ����¼� ��
    pInfo [In]: wms_cfg_event_info_s_type �ṹָ�룬�����¼������Ϣ��
    
       
����ֵ:
    none
       
��ע:
    Posts the Event to itself

==============================================================================*/
void WmsApp_CfgCb(wms_cfg_event_e_type event, wms_cfg_event_info_s_type *pInfo)
{
    wms_cfg_event_info_s_type   *pInfobuf=NULL;
    IShell                      *pShell = AEE_GetShell();
    uint8                       btRet; 
    AEEEvent                    evt=0;
    
    if (pShell == NULL)
    {
        return;
    }
    
    pInfobuf = sys_malloc(sizeof(wms_cfg_event_info_s_type));
    if (pInfobuf == NULL)
    {
        return;
    }
    
    (void)MEMCPY(pInfobuf, pInfo, sizeof(wms_cfg_event_info_s_type));

    switch (event)
    {
//        case WMS_CFG_EVENT_GW_READY:
//            evt = EVT_WMS_CFG_GW_READY;
//            break;
            
        case WMS_CFG_EVENT_CDMA_READY:
            evt = EVT_WMS_CFG_CDMA_READY;
            break;
            
        case WMS_CFG_EVENT_REFRESH_DONE:
            evt = EVT_WMS_CFG_REFRESH_DONE;
            break;
            
        case WMS_CFG_EVENT_ROUTES:
            evt = EVT_WMS_CFG_ROUTES;
            break;
            
        case WMS_CFG_EVENT_MEMORY_STATUS:
            evt = EVT_WMS_CFG_MEMORY_STATUS;
            break;
            
        case WMS_CFG_EVENT_MEMORY_FULL:
            evt = EVT_WMS_CFG_MEMORY_FULL;
            break;
            
        case WMS_CFG_EVENT_MESSAGE_LIST:
            evt = EVT_WMS_CFG_MESSAGE_LIST;
            break;
            
//        case WMS_CFG_EVENT_GW_DOMAIN_PREF:
//            evt = EVT_WMS_CFG_GW_DOMAIN_PREF;
//            break;
            
//        case WMS_CFG_EVENT_CELL_CHANGE:
//            evt = EVT_WMS_CFG_CELL_CHANGE;
//            break;
            
//        case WMS_CFG_EVENT_PRIMARY_CLIENT_SET:
//            evt = EVT_WMS_CFG_PRIMARY_CLIENT_SET;
//            break;
      
//        case WMS_CFG_EVENT_MEMORY_STATUS_SET:
//        case WMS_CFG_EVENT_LINK_CONTROL:
//            WMSAPPU_FREE(pInfobuf)
//            break;
            
        default:
            break;
    }
    
    if (evt != 0)
    {
        btRet = ISHELL_PostEventEx(pShell,
                                   EVTFLG_ASYNC, 
                                   AEECLSID_WMSAPP, 
                                   evt,
                                   0, 
                                   (uint32) pInfobuf);
        if (btRet == FALSE)
        {
            WMSAPPU_SYSFREE(pInfobuf)
        }
    }
    else
    {
        WMSAPPU_SYSFREE(pInfobuf);
    }
} // WmsApp_CfgCb

/*==============================================================================
����: 
    WmsApp_MsgCb
       
˵��: 
    ������Ϊ���� MSG ����¼��Ļص�������
       
����: 
    event [In]: CFG ����¼� ��
    pInfo [In]: wms_msg_event_info_s_type �ṹָ�룬�����¼������Ϣ��
    
       
����ֵ:
    none
       
��ע:
    Posts the Event to itself

==============================================================================*/
void WmsApp_MsgCb(wms_msg_event_e_type       event,
                  wms_msg_event_info_s_type  *pInfo,
                  boolean                    *shared)
{
    wms_msg_event_info_s_type   *pInfobuf=NULL;
    IShell                      *pShell = AEE_GetShell();
    uint8                       btRet; 
    AEEEvent                    evt=0;
    
    if (pShell == NULL)
    {
        return;
    }
    
    pInfobuf = sys_malloc(sizeof(wms_msg_event_info_s_type));
    if (pInfobuf == NULL)
    {
        return;
    }
        
    (void)MEMCPY(pInfobuf, pInfo, sizeof(wms_msg_event_info_s_type));
    switch (event)
    {
        case WMS_MSG_EVENT_SEND:	
            if ((pInfobuf->submit_report_info.client_id == WMS_CLIENT_TYPE_WMS_APP)            
				&& (pInfobuf->submit_report_info.report_status != WMS_RPT_OK))
            {
                evt = EVT_WMS_MSG_SEND;
            }
            break;
            
//        case WMS_MSG_EVENT_ACK:
//            evt = EVT_WMS_MSG_ACK;
//            break;
            
        case WMS_MSG_EVENT_READ:
            MSG_FATAL("WMS_MSG_EVENT_READ......................",0,0,0);
            evt = EVT_WMS_MSG_READ;
            break;
            
        case WMS_MSG_EVENT_WRITE:
            evt = EVT_WMS_MSG_WRITE;
            break;
            
        case WMS_MSG_EVENT_DELETE:
            evt = EVT_WMS_MSG_DELETE;
            break;
            
        case WMS_CMD_MSG_DELETE_BOX:
            evt = EVT_WMS_MSG_DELETE_BOX;
            break;
            
//        case WMS_CMD_MSG_COPY:
//            evt = EVT_WMS_MSG_COPY;
//            break;
            
        case WMS_MSG_EVENT_MODIFY_TAG:
            evt = EVT_WMS_MSG_MODIFY_TAG;
            break;
            
        case WMS_MSG_EVENT_READ_TEMPLATE:
            evt = EVT_WMS_MSG_READ_TEMPLATE;
            break;
            
        case WMS_MSG_EVENT_WRITE_TEMPLATE:
            evt = EVT_WMS_MSG_WRITE_TEMPLATE;
            break;
            
        case WMS_MSG_EVENT_DELETE_TEMPLATE:
            evt = EVT_WMS_MSG_DELETE_TEMPLATE;
            break;
        
        case WMS_MSG_EVENT_RECEIVED_MESSAGE:
            evt = EVT_WMS_MSG_RECEIVED_MESSAGE;
            break;
            
        case WMS_MSG_EVENT_SUBMIT_REPORT:            
            pInfobuf->submit_report_info.alpha_id.data = (uint8 *)sys_malloc(WMS_ALPHA_ID_MAX);
            pInfobuf->submit_report_info.alpha_id.len = pInfo->submit_report_info.alpha_id.len;
            
            if ((pInfo->submit_report_info.alpha_id.data != NULL) &&
                (pInfo->submit_report_info.alpha_id.len > 0))
            {
                (void)MEMCPY(pInfobuf->submit_report_info.alpha_id.data,
                            pInfo->submit_report_info.alpha_id.data,
                            pInfo->submit_report_info.alpha_id.len);
            }

            //Add By zzg 2010_08_07
            
            if (pInfobuf->submit_report_info.client_id == WMS_CLIENT_TYPE_WMS_APP)
            {
                evt = EVT_WMS_MSG_SUBMIT_REPORT;
            }
            //Add End

            //evt = EVT_WMS_MSG_SUBMIT_REPORT;
            
            break;
            
        case WMS_MSG_EVENT_STATUS_REPORT:
            evt = EVT_WMS_MSG_STATUS_REPORT;
            break;
            
//        case WMS_MSG_EVENT_DELETE_ALL:
//            evt = EVT_WMS_MSG_DELETE_ALL;
//            break;
//        
//        case WMS_MSG_EVENT_DELETE_TEMPLATE_ALL:
//            evt = EVT_WMS_MSG_DELETE_TEMPLATE_ALL;
//            break;
            
//        case WMS_MSG_EVENT_READ_STS_REPORT:
//            evt = EVT_WMS_MSG_READ_STATUS_REPORT;
//            break;
            
//        case WMS_MSG_EVENT_DELETE_STS_REPORT:
//            evt = EVT_WMS_MSG_DELETE_STATUS_REPORT;
//            break;
            
//        case WMS_MSG_EVENT_ACK_REPORT:
//            WMSAPPU_FREE(pInfobuf)
//            break;
            
//        case WMS_MSG_EVENT_DUPLICATE_CB_PAGE:
//            evt = EVT_WMS_MSG_DUPLICATE_CB_PAGE;
//            break;
            
        default:
            break;
    } // switch
    
    if (evt != 0)
    {
        btRet = ISHELL_PostEventEx(pShell,
                                   EVTFLG_ASYNC, 
                                   AEECLSID_WMSAPP, 
                                   evt,
                                   0, 
                                   (uint32) pInfobuf);
                        
        if (btRet == FALSE)
        {
            WMSAPPU_SYSFREE(pInfobuf)
        }
    }
    else
    {
        WMSAPPU_SYSFREE(pInfobuf);
    }
} // WmsApp_MsgCb() 

#ifdef FEATURE_BROADCAST_SMS
/*==============================================================================
����: 
    WmsApp_BcCb
       
˵��: 
    ������Ϊ���� WMS  Broadcast ����¼��Ļص�������
       
����: 
    event [In]: WMS  Broadcast ����¼� ��
    pInfo [In]: wms_bc_event_info_s_type �ṹָ�룬�����¼������Ϣ��
    
       
����ֵ:
    none
       
��ע:
    Posts the Event to itself

==============================================================================*/
void WmsApp_BcCb(wms_bc_event_e_type  event, wms_bc_event_info_s_type  *pInfo)
{
//wangliang close!  2010-08-14
#if 0    

    wms_bc_event_info_s_type    *pInfobuf = NULL;
    IShell                      *pShell = AEE_GetShell();
    uint8                       btRet; 
    AEEEvent                    evt = EVT_WMS_BC_EVENT;


    if (pShell == NULL)
    {
        return;
    }
    
    pInfobuf = sys_malloc(sizeof(wms_bc_event_info_s_type));
    if (pInfobuf == NULL)
    {
        return;
    }
    
    (void)MEMCPY(pInfobuf, pInfo, sizeof(wms_bc_event_info_s_type));
    
    switch (event)
    {
        case WMS_BC_EVENT_CONFIG:
        case WMS_BC_EVENT_PREF:
        case WMS_BC_EVENT_TABLE:
        case WMS_BC_EVENT_SRV_IDS:
        case WMS_BC_EVENT_SRV_INFO:
        case WMS_BC_EVENT_SRV_ADDED:
        case WMS_BC_EVENT_SRV_DELETED:
        case WMS_BC_EVENT_SRV_UPDATED:
            
        // lower layer (CM & CP) failure
        case WMS_BC_EVENT_ENABLE_FAILURE:  
        
        // lower layer (CM & CP) failure
        case WMS_BC_EVENT_DISABLE_FAILURE:
             
        case WMS_BC_EVENT_ALL_SRV_DELETED:
            break;
            
        default:
            evt = 0;
            WMSAPPU_SYSFREE(pInfobuf)
            break;
    }
    
    if (evt != 0)
    {
        btRet = ISHELL_PostEventEx(pShell,
                                   EVTFLG_ASYNC, 
                                   AEECLSID_WMSAPP, 
                                   evt,
                                   (uint16)event, 
                                   (uint32)pInfobuf);
        if (btRet == FALSE)
        {
            WMSAPPU_SYSFREE(pInfobuf)
        }
    }
#endif
}
#endif /* FEATURE_BROADCAST_SMS */

#ifdef  FEATURE_CDSMS
/*==============================================================================
����: 
    WmsApp_DcCb
       
˵��: 
    ������Ϊ���� DC ����¼��Ļص�������
       
����: 
    event [In]: DC ����¼� ��
    info_ptr [In]: wms_dc_event_info_s_type �ṹָ�룬�����¼������Ϣ��
    
����ֵ:
    none
       
��ע:
    Posts the Event to itself

==============================================================================*/
void WmsApp_DcCb(wms_dc_event_e_type  event,wms_dc_event_info_s_type  *info_ptr)
{
    wms_dc_event_info_s_type    *pInfobuf = NULL;
    IShell                      *pShell = AEE_GetShell();
    uint8                       btRet; 
    AEEEvent                    evt=0;
    
    if (pShell == NULL)
    {
        return;
    }
  
    pInfobuf = sys_malloc(sizeof(wms_dc_event_info_s_type));
    if(pInfobuf == NULL)
    {
        return;
    }
    
    (void) MEMCPY(pInfobuf, info_ptr, sizeof(wms_dc_event_info_s_type));
    switch(event)
    {
//        case WMS_DC_EVENT_CONNECTED:
//            if ((pInfobuf->so != WMS_DC_SO_6) &&
//                (pInfobuf->so != WMS_DC_SO_14))
//            {
//                MSG_MED("NOT a WMS call", 0, 0, 0);
//            }
//            else
//            {
//                evt = EVT_WMS_DC_CONNECTED;
//            }
//            WMSAPPU_FREE(pInfobuf)
//            break;
            
//        case WMS_DC_EVENT_ABORTED:
//            evt = EVT_WMS_DC_ABORTED;
//            WMSAPPU_FREE(pInfobuf)
//            break;
            
//        case WMS_DC_EVENT_ENABLE_AUTO_DISCONNECT:
//            evt = EVT_WMS_DC_ENABLE_AUTO_DISCONNECT;
//            WMSAPPU_FREE(pInfobuf)
//            break;
            
//        case WMS_DC_EVENT_DISABLE_AUTO_DISCONNECT:
//            evt = EVT_WMS_DC_DISABLE_AUTO_DISCONNECT;
//            WMSAPPU_FREE(pInfobuf)
//            break;

        case WMS_DC_EVENT_DISCONNECTED:
            evt = EVT_WMS_DC_DISCONNECTED;
            break;
            
        case WMS_DC_EVENT_INCOMING:
        case WMS_DC_EVENT_CONNECTING:
        default:
            break;
    }
    
    if (evt != 0)
    {
        btRet = ISHELL_PostEventEx(pShell,
                        EVTFLG_ASYNC, 
                        AEECLSID_WMSAPP, 
                        evt,
                        0, 
                        0);
                        
        if(btRet == FALSE)
        {
            MSG_HIGH("Event  %x could not be posted!", evt,0,0);
        }
    }
    else
    {
        WMSAPPU_SYSFREE(pInfobuf);
    }

} /* WmsApp_DcCb */
#endif

/*==============================================================================
����: 
    WmsApp_Init
       
˵��: 
    �ڳ�ʼ�������У�Wms Applet ����һЩ����� WMS �Ը�����Ϣ�б��ڴ�״̬�ȡ�
       
����: 
    pMe [In]: ָ��WMS Applet����ṹ��ָ�롣�ýṹ����С������ض���Ϣ��
    
����ֵ:
    none
       
��ע:
    WMS Applet ���յ���Ӧ�������Ӧ�¼���

==============================================================================*/
static void WmsApp_Init(WmsApp *pMe)
{
    int nRet;


    (void)IWMS_SetPrimaryClient(pMe->m_pwms, 
                        WMS_CLIENT_TYPE_WMS_APP, 
                        &pMe->m_callback, 
                        (void*)pMe, 
                        TRUE, FALSE);          

   
#ifdef FEATURE_CDSMS
    if (pMe->m_mode == WMS_MESSAGE_MODE_CDMA)
    {
        (void)IWMS_CfgGetRoutes(pMe->m_pwms,
                               pMe->m_clientId,
                               &pMe->m_callback,
                               (void*)pMe);
        
#ifdef FEATURE_CDSMS_RUIM
        // ��ȡ RUIM ������Ϣ�б�
        /*
        (void)IWMS_CfgGetMessageList(pMe->m_pwms,
                                     pMe->m_clientId,
                                     &pMe->m_callback,
                                     (void*)pMe,
                                     WMS_MEMORY_STORE_RUIM,
                                     WMS_TAG_NONE);
                                     
        // ��ȡ RUIM ����ģ���б�
        (void)IWMS_CfgGetMessageList(pMe->m_pwms,
                                     pMe->m_clientId,
                                     &pMe->m_callback,(void*)pMe,
                                     WMS_MEMORY_STORE_RUIM,
                                     WMS_TAG_MO_TEMPLATE);
        */                             
        
        // ��ȡ RUIM ���洢״̬
        (void)IWMS_CfgGetMemoryStatus(pMe->m_pwms,
                                      pMe->m_clientId,
                                      &pMe->m_callback,
                                      (void*)pMe,
                                      WMS_MEMORY_STORE_RUIM,
                                      WMS_TAG_NONE);
#endif // FEATURE_CDSMS_RUIM

        /*
        // ��ȡ NV ��Ϣ�б�
        (void)IWMS_CfgGetMessageList(pMe->m_pwms,
                                     pMe->m_clientId,
                                     &pMe->m_callback,
                                     (void*)pMe,
                                     WMS_MEMORY_STORE_NV_CDMA,
                                     WMS_TAG_NONE);
        */
        
        // ��ȡ NV �洢״̬
        (void)IWMS_CfgGetMemoryStatus(pMe->m_pwms,
                                      pMe->m_clientId,
                                      &pMe->m_callback,
                                      (void*)pMe,
                                      WMS_MEMORY_STORE_NV_CDMA,
                                      WMS_TAG_NONE);

#ifdef FEATURE_BROADCAST_SMS
        // ��ʼ���㲥�������á��ο���ҵ���
#ifdef FEATURE_CDSMS_BROADCAST
        nRet = IWMS_BcMmGetConfig(pMe->m_pwms,
                                  pMe->m_clientId,
                                  &pMe->m_callback,
                                  NULL,
                                  WMS_MESSAGE_MODE_CDMA);
        
        nRet = IWMS_BcMmGetPref(pMe->m_pwms,
                                pMe->m_clientId,
                                &pMe->m_callback,
                                NULL,
                                WMS_MESSAGE_MODE_CDMA);
        
        nRet = IWMS_BcMmGetTable(pMe->m_pwms,
                                 pMe->m_clientId,
                                 &pMe->m_callback,
                                 NULL,
                                 WMS_MESSAGE_MODE_CDMA);
#else // FEATURE_CDSMS_BROADCAST
        
        nRet = IWMS_BcGetConfig(pMe->m_pwms,
                                pMe->m_clientId,
                                &pMe->m_callback,
                                NULL);
        
        nRet = IWMS_BcGetPref(pMe->m_pwms,
                              pMe->m_clientId,
                              &pMe->m_callback,
                              NULL);
        
        nRet = IWMS_BcGetTable(pMe->m_pwms,
                               pMe->m_clientId,
                               &pMe->m_callback,
                               NULL);
#endif // FEATURE_CDSMS_BROADCAST
#endif // FEATURE_BROADCAST_SMS

        nRet = IWMS_DcEnableAutoDisconnect(pMe->m_pwms,
                                           pMe->m_clientId,
                                           &pMe->m_callback, 
                                           NULL,
                                           WMSAPP_AUTO_DISCONNECT_TIME);
    }
#ifndef FEATURE_OEMOMH
    (void)IWMS_MsgSetRetryPeriod(pMe->m_pwms, pMe->m_clientId, WMS_MAX_RETRY_PERIOD);
#endif
    pMe->m_bCdmaWmsReady = TRUE;

    pMe->m_pImage = NULL;
#ifdef FEATURE_AUTOREPLACE
    {
        boolean autoreplace = FALSE;
        
        (void) ICONFIG_GetItem(pMe->m_pConfig,
                               CFGI_WMS_AUTOREPLACE,
                               &autoreplace,
                               sizeof(autoreplace));
        
        (void)IWMS_SetAutoreplace(pMe->m_pwms, 
                                  pMe->m_clientId, 
                                  &pMe->m_callback,
                                  (void*)pMe,
                                  autoreplace);
    }
#endif /* FEATURE_AUTOREPLACE */    
    
    // ����ԤԼ���Ŷ�ʱ��
    WmsApp_ReservedMsgSetTimer(pMe);
#endif // FEATURE_CDSMS
}


/*==============================================================================
����: 
    WmsApp_wmscommandreport
       
˵��: 
    WMS �ô˺�����������߻㱨���������
       
����: 
    cmd [in]: wms �������͡�
    user_data[in]: ��������ʱ������û�����ָ�롣
    cmd_err [in]: ����״̬��������͡�
    
����ֵ:
    none
       
��ע:

==============================================================================*/
void WmsApp_wmscommandreport(wms_cmd_id_e_type   cmd,
                             void                *user_data,
                             wms_cmd_err_e_type  cmd_err)
{
    WmsApp * pMe = (WmsApp *) (user_data);
  
    if (pMe != NULL)
    {
        wmsapp_cmd_status_type *cmd_status = sys_malloc(sizeof(wmsapp_cmd_status_type));
        
        if (cmd_status == NULL)
        {
            return;
        }
        cmd_status->cmd = cmd;
        cmd_status->cmd_err = cmd_err;
        
        (void)ISHELL_PostEventEx(pMe->m_pShell, 
                                 EVTFLG_ASYNC,
                                 AEECLSID_WMSAPP,
                                 EVT_WMS_CMD_STATUS, 
                                 0, 
                                 (uint32)cmd_status);
    }
}

/*==============================================================================
����:
    WmsApp_UpdateMemoryStatus

˵��:
    �������µ�ǰ����֧�ֵĴ洢����״̬��

����:
    pMe [in]: ָ�� WMS Applet����ṹ��ָ�롣�ýṹ����С������ض���Ϣ��

����ֵ:
    none

��ע:
       
==============================================================================*/
static void WmsApp_UpdateMemoryStatus(WmsApp *pMe, wms_memory_status_s_type *ptr)
{
    switch (ptr->mem_store)
    {
        case WMS_MEMORY_STORE_RUIM:
            if (ptr->tag == WMS_TAG_NONE)
            {
                (void)MEMCPY(&(pMe->m_memruimStatuse),
                             ptr, sizeof(wms_memory_status_s_type));
            }
            break;
            
        case WMS_MEMORY_STORE_NV_CDMA:
            break;
        
        default:
            break;
    }
}

/*==============================================================================
����:
    WmsApp_FreeSaveNumList

˵��:
    �ͷ������е�ȫ�����ݡ�

����:
    pMe [in]: ָ�� WMS Applet����ṹ��ָ�롣�ýṹ����С������ض���Ϣ��

����ֵ:
    none

��ע:
       
==============================================================================*/
void WmsApp_FreeSaveNumList(WmsApp *pMe)
{
    int32     i, dwCount;
    AECHAR    *pItem = NULL;

    if (pMe == NULL)
    {
        return;
    }
    
    if (pMe->m_pSaveNumList == NULL)
    {
        return;
    }
    
    dwCount = IVector_Size(pMe->m_pSaveNumList);

    for (i=dwCount-1; i >= 0; i--)
    {
        // ��������ɾ��
        pItem = (AECHAR *)IVector_RemoveElementAt(pMe->m_pSaveNumList, i);
        if (NULL != pItem)
        {
            // �ͷŶ�̬����Ŀռ�
            FREE(pItem);
            pItem = NULL;
        }
    }
}

/*==============================================================================
����:
    WmsApp_FreeMultiSendList

˵��:
    �ͷ������е�ȫ�����ݡ�

����:
    pIV [in]: ָ��IVector�ӿڶ����ָ�롣

����ֵ:
    none

��ע:
       
==============================================================================*/
void WmsApp_FreeMultiSendList(IVector *pIV)
{
    int32       i, dwCount;
    CMultiSendItemInfo *pItem = NULL;

    if (pIV == NULL)
    {
        return;
    }
    
    dwCount = IVector_Size(pIV);

    for (i=dwCount-1; i >= 0; i--)
    {
        pItem = (CMultiSendItemInfo *)IVector_RemoveElementAt(pIV, i);
        if (NULL != pItem)
        {
            // �ͷŶ�̬����Ŀռ�
            sys_free(pItem);
            pItem = NULL;
        }
    }
}

/*==============================================================================
����:
    WmsApp_FreeMsgNodeMs

˵��:
    �����ͷ� WmsApp �ṹ�� m_CurMsgNodesMS ��Ա��������ݡ�

����:
    pMe [in]: ָ�� WMS Applet����ṹ��ָ�롣�ýṹ����С������ض���Ϣ��

����ֵ:
    none

��ע:
       
==============================================================================*/
void WmsApp_FreeMsgNodeMs(WmsApp *pMe)
{
    int i;
    
    for (i=0; i<LONGSMS_MAX_PACKAGES; i++)
    {
        if (pMe->m_CurMsgNodesMS[i] != NULL)
        {
            WMSMessageStruct_Reset(pMe->m_CurMsgNodesMS[i]);
            FREE(pMe->m_CurMsgNodesMS[i]);
            pMe->m_CurMsgNodesMS[i] = NULL;
        }
    }
}

/*==============================================================================
����:
    WmsApp_CombinateMsg

˵��:
    ������ WmsApp �ṹ�� m_CurMsgNodesMS ��Ա�������Ϣ���ݺϲ��� m_msCur �С�

����:
    pMe [in]: ָ�� WMS Applet����ṹ��ָ�롣�ýṹ����С������ض���Ϣ��

����ֵ:
    none

��ע:
       
==============================================================================*/
void WmsApp_CombinateMsg(WmsApp *pMe)
{
    WMSMessageStruct *pTep;
    int i, nlen=0, nMaxIdx=1;
    boolean bFirst = TRUE;
    int         nBranches=0;
    int         nCurBranchNum=0;
#ifdef FEATURE_SMS_UDH
    uint8       total_sm;   // ��Ϣ���ݰ�����
    uint8       seq_num;    // ��Ϣ���
#endif    
    AECHAR *pMsgText = NULL;
    AECHAR *pNotRecieved = NULL;
    int32  nSize;
    
    // �ṹ��������
    WMSMessageStruct_Reset(&pMe->m_msCur);
    
    // �Ȳ�����һ�ǿսڵ�
    for (i=0; i<LONGSMS_MAX_PACKAGES; i++)
    {
        pTep = pMe->m_CurMsgNodesMS[i];
        if (pTep != NULL)
        {
            break;
        }
    }
    if (NULL == pTep)
    {// ��һ�ǿսڵ�û�ҵ�
        MSG_FATAL("NULL    IS    pTep.......................",0,0,0);
        return;
    }
    
#ifdef FEATURE_SMS_UDH
    total_sm = pTep->total_sm;
    seq_num = pTep->seq_num;
    if (total_sm > 1)
    {
        // ���㳤���ŷ�֧����(ÿ����֧��� LONGSMS_MAX_PACKAGES �����ݰ�)
        nBranches = total_sm / (LONGSMS_MAX_PACKAGES);
        if ((total_sm % (LONGSMS_MAX_PACKAGES)) != 0)
        {
            nBranches++;
        }
        
        // ȷ����ǰ����ķ�֧��(0,...,nBranches-1)
        nCurBranchNum = (seq_num - 1) / (LONGSMS_MAX_PACKAGES);
        if (nBranches>1)
        {
            nlen += 20;
            
            if (nCurBranchNum < (nBranches-1))
            {
                nMaxIdx = LONGSMS_MAX_PACKAGES;
            }
            else
            {
                nMaxIdx = total_sm - nCurBranchNum*LONGSMS_MAX_PACKAGES;
            }
        }
        else
        {
            nMaxIdx = total_sm;
        }
    }
#endif    
    
    // ������Ϣ���ܳ���
    for (i=0; i<nMaxIdx; i++)
    {
        if (pMe->m_CurMsgNodesMS[i] != NULL)
        {
            if (NULL != (pMe->m_CurMsgNodesMS[i])->m_szMessage)
            {
                nlen += WSTRLEN((pMe->m_CurMsgNodesMS[i])->m_szMessage);
            }
        }
        else
        {
            nlen += STRLEN(DATA_NOT_RECEIVED);
        }
    }
    
    nSize = (STRLEN(DATA_NOT_RECEIVED)+1)*sizeof(AECHAR);
    pNotRecieved = (AECHAR *)sys_malloc(nSize);
    if (NULL == pNotRecieved)
    {
        return;
    }
    (void)STRTOWSTR(DATA_NOT_RECEIVED, pNotRecieved, nSize);
    
    nSize = (nlen+1)*sizeof(AECHAR);
    pMsgText = (AECHAR *)sys_malloc(nSize);
    if (NULL == pMsgText)
    {
        WMSAPPU_SYSFREE(pNotRecieved);
        return;
    }
    
    if (nBranches>1)
    {
        AECHAR  wszFmt[20] = {0};
        
        (void)STRTOWSTR("(%d/%d)", wszFmt, sizeof(wszFmt));
        WSPRINTF(pMsgText, nSize, wszFmt, (nCurBranchNum+1), nBranches);
    }
    MSG_FATAL("conmm msg...........................",0,0,0);
    // ��ʼ�ϲ���Ϣ
    for (i=0; i<nMaxIdx; i++)
    {
        if (pMe->m_CurMsgNodesMS[i] != NULL)
        {
        	MSG_FATAL("conmm msg...........................2",0,0,0);
            if (bFirst)
            {
            	MSG_FATAL("conmm msg...........................3",0,0,0);
                bFirst = FALSE;
                MEMCPY(&pMe->m_msCur, pMe->m_CurMsgNodesMS[i], sizeof(WMSMessageStruct));
                pMe->m_msCur.m_szMessage = pMsgText;
                DBGPRINTF("pMe->m_msCur.m_szMessage=%S",pMe->m_msCur.m_szMessage);
            }
            if (NULL != (pMe->m_CurMsgNodesMS[i])->m_szMessage)
            {
                (void)WSTRCAT(pMsgText, (pMe->m_CurMsgNodesMS[i])->m_szMessage);
            }
        }
        else
        {
            (void)WSTRCAT(pMsgText, pNotRecieved);
        }
    }
    
    WMSAPPU_SYSFREE(pNotRecieved);
}

/*==============================================================================
����:
    WmsApp_CopyCurMsgForSend

˵��:
    ��������ǰ�������Ϣ���ݸ��Ƶ� m_msSend �С�

����:
    pMe [in]: ָ�� WMS Applet����ṹ��ָ�롣�ýṹ����С������ض���Ϣ��

����ֵ:
    none

��ע:
       
==============================================================================*/
void WmsApp_CopyCurMsgForSend(WmsApp *pMe)
{
    int i, nlen=0;
    AECHAR *pMsgText = NULL;
    int32 nSize = 0;
    
    // �ṹ��������
    WMSMessageStruct_Reset(&pMe->m_msSend);

    // ������Ϣ���ܳ���
    for (i=0; i<LONGSMS_MAX_PACKAGES; i++)
    {
        if ((pMe->m_CurMsgNodesMS[i] != NULL) &&
            (NULL != (pMe->m_CurMsgNodesMS[i])->m_szMessage))
        {
            nlen += WSTRLEN((pMe->m_CurMsgNodesMS[i])->m_szMessage);
        }
    }
    
    nSize = (nlen+1)*sizeof(AECHAR);
    pMsgText = (AECHAR *)sys_malloc(nSize);
    if (NULL == pMsgText)
    {
        return;
    }
    
    // ��ʼ������Ϣ
    for (i=0; i<LONGSMS_MAX_PACKAGES; i++)
    {
        if ((pMe->m_CurMsgNodesMS[i] != NULL) &&
            (NULL != (pMe->m_CurMsgNodesMS[i])->m_szMessage))
        {
            (void)WSTRCAT(pMsgText, (pMe->m_CurMsgNodesMS[i])->m_szMessage);
			WMSMessageStruct_Reset(pMe->m_CurMsgNodesMS[i]);
        }
    }
    
    pMe->m_msSend.m_szMessage = pMsgText;
}

/*==============================================================================
����:
    WmsApp_FreeUserDataMOList

˵��:
    �����ͷ� WmsApp �ṹ�� m_pUserDataMOList ��Ա��������ݡ�

����:
    pIV [in]: ָ��IVector�ӿڶ����ָ�롣

����ֵ:
    none

��ע:
       
==============================================================================*/
void WmsApp_FreeUserDataMOList(IVector *pIV)
{
    int32       i, dwCount;
    wms_cdma_user_data_s_type *pItem = NULL;

    if (pIV == NULL)
    {
        return;
    }
    
    dwCount = IVector_Size(pIV);

    for (i=dwCount-1; i >= 0; i--)
    {
        // ��������ɾ��
        pItem = (wms_cdma_user_data_s_type *)IVector_RemoveElementAt(pIV, i);
        if (NULL != pItem)
        {
            // �ͷŶ�̬����Ŀռ�
            FREE(pItem);
            pItem = NULL;
        }
    }
}

/*==============================================================================
����:
    WmsApp_GetUserDataMOList

˵��:
    �������ڻ�ȡȺ������

����:
    none
       
����ֵ:
    IVector *

��ע:
       
==============================================================================*/
static IVector * WmsApp_GetUserDataMOList(void)
{
    // �ռ�����Ϣ������Ϣ�б�
    static IVector  *gUserDataMOList = NULL;
    int             nRet;
    
    IShell * pShell = AEE_GetShell();
    
    // ���ж�����ӿ��Ƿ񴴽�����δ�����ٴ���
    if (NULL != gUserDataMOList)
    {
        return gUserDataMOList;
    }
    
    if (NULL == pShell)
    {
        return NULL;
    }
    
    nRet = ISHELL_CreateInstance(pShell,AEECLSID_VECTOR,(void**)&gUserDataMOList);
    if (nRet != SUCCESS )
    {
        gUserDataMOList = NULL;
    }
    
    return gUserDataMOList;
} // WmsApp_GetUserDataMOList

/*==============================================================================
����:
    WmsApp_PrepareUserDataMOList

˵��:
    ��������ǰ���͵���Ϣ���� m_msSend ת���� m_pUserDataMOList �С�

����:
    pMe [in]: ָ�� WMS Applet����ṹ��ָ�롣�ýṹ����С������ض���Ϣ��

����ֵ:
    none

��ע:
    ��Ӧ�÷��������Ŵ��ӷ�ʽ�̶�ʹ�� concat_8
==============================================================================*/
#ifdef FEATURE_OEMOMH
extern boolean nvruim_sms_ems_support(void);
#endif

void WmsApp_PrepareUserDataMOList(WmsApp *pMe)
{
    int i;
    int nItems = 0;     // ����Ϣ���ֺ�õ���������
    int nMaxItemChars = 0;     // ÿ������ַ���
    int nLen = 0;
    int nChars=0;
    int nEnd;
    int nMaxChars_UNICODE = MAX_UNICODMSG_PAYLOAD;
    int nMaxChars_ASCII = MAX_ASCIIMSG_PAYLOAD;
    int32 nSize;
    wms_user_data_encoding_e_type      encoding;
    wms_cdma_user_data_s_type          *pUserdata = NULL;
#ifdef FEATURE_OEMOMH
    boolean bSupportEMS = nvruim_sms_ems_support();
#endif
    // �ͷ���Ϣ�û���������
    WmsApp_FreeUserDataMOList(pMe->m_pUserDataMOList);
    
    if (NULL != pMe->m_msSend.m_szMessage)
    {
        nLen = WSTRLEN(pMe->m_msSend.m_szMessage);
    }
    
    if (nLen == 0)
    {// ����Ϣ
        nSize = sizeof(wms_cdma_user_data_s_type);
        
        pUserdata = (wms_cdma_user_data_s_type *)MALLOC(nSize);
        if (NULL != pUserdata)
        {
            MEMSET(pUserdata, 0, nSize);
            pUserdata->encoding = WMS_ENCODING_ASCII;
            (void) IVector_EnsureCapacity(pMe->m_pUserDataMOList, 1);
            
            if (SUCCESS != IVector_AddElement(pMe->m_pUserDataMOList, pUserdata))
            {
                FREE(pUserdata);
            }
        }
        
        return;
    }
    // ȷ����Ϣ���뷽ʽ���軮�ֵ�����
    if (WMSUtil_HaveNoneASCIIChar(pMe->m_msSend.m_szMessage, NULL))
    {
        DBGPRINTF("DRAFT encoding = WMS_ENCODING_UNICODE");
        encoding = WMS_ENCODING_UNICODE;
    }
    else
    {
#if defined (FEATURE_CARRIER_ANGOLA_MOVICEL)
        byte  mo_encoding=0;
        
        (void) ICONFIG_GetItem(pMe->m_pConfig, 
                               CFGI_SMS_MO_ENCODING,
                               &mo_encoding,
                               sizeof(mo_encoding));
        if (mo_encoding == OEMNV_SMS_MO_ENCODING_7BIT )
        {
            encoding = WMS_ENCODING_ASCII;
        }
        else
        {
            encoding = WMS_ENCODING_LATIN;
        }
#elif defined(FEATURE_CARRIER_ISRAEL_PELEPHONE)        
        encoding = WMS_ENCODING_LATIN_HEBREW;
#else //FEATURE_CARRIER_ANGOLA_MOVICEL
        if (nLen<=MAX_OCTETMSG_PAYLOAD)
        {
#if defined(FEATURE_SMS_GSM7BIT)
#if defined (FEATURE_CARRIER_VENEZUELA_MOVILNET)
            encoding = WMS_ENCODING_ASCII;
#else
            encoding = WMS_ENCODING_LATIN;
#endif
#else
            encoding = WMS_ENCODING_ASCII;
#endif               
        }
        else
        {
            encoding = WMS_ENCODING_ASCII;
        }
        DBGPRINTF("DRAFT encoding = WMS_ENCODING_ASCII");
#endif //FEATURE_CARRIER_ANGOLA_MOVICEL
    }

    // ȷ����Ϣ���뷽ʽ���軮�ֵ�����
    if (WMS_ENCODING_UNICODE == encoding)
    {        
        DBGPRINTF("WMS_ENCODING_UNICODE == encoding");
        if (nLen<=nMaxChars_UNICODE)
        {
            nMaxItemChars = nMaxChars_UNICODE;
        }
        else
        {
#ifdef FEATURE_SMS_UDH
#ifdef FEATURE_OEMOMH
            if(!bSupportEMS)
            {
                nMaxItemChars = nMaxChars_UNICODE;
            }
            else
#endif
            nMaxItemChars = (nMaxChars_UNICODE-3);
#else
            nMaxItemChars = nMaxChars_UNICODE;
#endif            
        }
    }
    else if (WMS_ENCODING_ASCII == encoding)
    {      
        DBGPRINTF("WMS_ENCODING_ASCII == encoding");
        if (nLen<=nMaxChars_ASCII)
        {
            nMaxItemChars = nMaxChars_ASCII;
        }
        else
        {
#ifdef FEATURE_SMS_UDH
#ifdef FEATURE_OEMOMH
            if(!bSupportEMS)
            {
                nMaxItemChars = nMaxChars_ASCII;
            }
            else
#endif
            nMaxItemChars = (nMaxChars_ASCII-7);
#else
            nMaxItemChars = nMaxChars_ASCII;
#endif            
        }
    }
    else
    {        
        DBGPRINTF("XXXXXXXXXXXXXXXX == encoding");
        if (nLen <= MAX_OCTETMSG_PAYLOAD)
        {
            nMaxItemChars = MAX_OCTETMSG_PAYLOAD;
        }
        else
        {
#ifdef FEATURE_SMS_UDH
#ifdef FEATURE_OEMOMH
            if(!bSupportEMS)
            {
                nMaxItemChars = MAX_OCTETMSG_PAYLOAD;
            }
            else
#endif
            nMaxItemChars = MAX_OCTETMSG_PAYLOAD-6;
#else
            nMaxItemChars = MAX_OCTETMSG_PAYLOAD;
#endif 
        }           
    }
    
    nItems = nLen / nMaxItemChars;
    if (nLen % nMaxItemChars != 0)
    {
        nItems++;
    }
    
    (void) IVector_EnsureCapacity(pMe->m_pUserDataMOList, nItems);
    DBGPRINTF("nItems=%d",nItems);
    // ��ʼ������Ϣ�û������б�
    for (i=0; i<nItems; i++)
    {
        nSize = sizeof(wms_cdma_user_data_s_type);
        
        pUserdata = (wms_cdma_user_data_s_type *)MALLOC(nSize);
        if (NULL == pUserdata)
        {
            break;
        }
        
        MEMSET(pUserdata, 0, nSize);
        pUserdata->encoding = encoding;
#ifdef FEATURE_SMS_UDH
#ifdef FEATURE_OEMOMH
        if(!bSupportEMS)
        {
            // Nothing TODO
        }
        else
#endif
        if (nItems>1)
        {
        	MSG_FATAL("nItems================%d",nItems,0,0);
            pUserdata->num_headers = 1;
            pUserdata->headers[0].header_id = WMS_UDH_CONCAT_8;
            
            // ע��: msg_ref ���Ժ�����
            //pUserdata->headers[0].u.concat_8.msg_ref = ?;
            pUserdata->headers[0].u.concat_8.total_sm = nItems;
            pUserdata->headers[0].u.concat_8.seq_num = i+1;
        }
#endif 

        if (i==nItems-1)
        {
            nChars = nLen - i*nMaxItemChars;
        }
        else
        {
            nChars = nMaxItemChars;
        }
        nEnd = i*nMaxItemChars + nChars;
        
        // ��ʼ��������       
        if (encoding == WMS_ENCODING_ASCII)
        {
            char  *szText = NULL;
#if !defined(FEATURE_SMS_GSM7BIT)
            AECHAR wChar = pMe->m_msSend.m_szMessage[nEnd];
#endif            
            
            nSize = (nChars+1)*sizeof(char);
            szText = (char *)MALLOC(nSize);
            if (NULL == szText)
            {
                FREE(pUserdata);                
                return;
            }
                
#if defined(FEATURE_SMS_GSM7BIT)
            {
                int     j;
                word    c_temp;
                char    *strTep = szText;
                
                for (j=i*nMaxItemChars; j<nEnd; j++, strTep++)
                {
                    c_temp = Ucs2ToGsm[pMe->m_msSend.m_szMessage[j]];
                    
                    if (c_temp == NON_GSM)
                    {
                        c_temp = 0x20;//' '
                    }
                    *strTep = (char)c_temp;
                }
            }
#else            
            pMe->m_msSend.m_szMessage[nEnd] = 0;
            (void)WSTRTOSTR(&pMe->m_msSend.m_szMessage[i*nMaxItemChars], szText, nSize);
            pMe->m_msSend.m_szMessage[nEnd] = wChar;
            DBGPRINTF("DRAFT szText1=%s", szText);
#endif
            
            pUserdata->data_len = nChars;
            pUserdata->number_of_digits = wms_ts_pack_ascii(szText,
                                                        pUserdata->data,
                                                        &pUserdata->data_len,
                                                        &pUserdata->padding_bits);

			

			
            DBGPRINTF("DRAFT szText2=%s", szText);
            FREE(szText);
            if (SUCCESS != IVector_AddElement(pMe->m_pUserDataMOList, pUserdata))
            {
                FREE(pUserdata);
                return;
            }
        }
        else if (encoding == WMS_ENCODING_UNICODE)
        {
            uint16  *pTep = (uint16 *)pUserdata->data;
            int j;
            
            pUserdata->number_of_digits = nChars;
            pUserdata->data_len = nChars*sizeof(AECHAR);
            pUserdata->padding_bits = 0;
            
            for (j=i*nMaxItemChars; j<nEnd; j++,pTep++)
            {
                *pTep = HTONS(pMe->m_msSend.m_szMessage[j]);
            }
            
            if (SUCCESS != IVector_AddElement(pMe->m_pUserDataMOList, pUserdata))
            {
                FREE(pUserdata);
                return;
            }
        }
#ifdef FEATURE_LANG_HEBREW
        else if (encoding == WMS_ENCODING_LATIN_HEBREW)
        {
            byte    *pTep = (byte *)pUserdata->data;
            int     j;
            AECHAR  wChar;
            
            pUserdata->number_of_digits = nChars;
            pUserdata->data_len = nChars;
            pUserdata->padding_bits = 0;
            
            for (j=i*nMaxItemChars; j<nEnd; j++,pTep++)
            {
                if (pMe->m_msSend.m_szMessage[j] == 0x203E)
                {
                    *pTep = 0xAF;
                }
                else if (pMe->m_msSend.m_szMessage[j] == 0x00D7)
                {
                    *pTep = 0xAA;
                }
                else
                {
                    wChar = Ucs2ToHebrew[pMe->m_msSend.m_szMessage[j] & 0x00FF];
                    
                    *pTep = (wChar == NON_HEB ? 0x20 : (byte)wChar);
                }
            }
            
            if (SUCCESS != IVector_AddElement(pMe->m_pUserDataMOList, pUserdata))
            {
                FREE(pUserdata);
                return;
            }
        }
#endif        
        else
        {// encoding == WMS_ENCODING_OCTET
            byte  *pTep = (byte *)pUserdata->data;
            int j;
            
            pUserdata->number_of_digits = nChars;
            pUserdata->data_len = nChars;
            pUserdata->padding_bits = 0;
            
            for (j=i*nMaxItemChars; j<nEnd; j++,pTep++)
            {
                *pTep = (byte)pMe->m_msSend.m_szMessage[j];
            }
            
            if (SUCCESS != IVector_AddElement(pMe->m_pUserDataMOList, pUserdata))
            {
                FREE(pUserdata);
                return;
            }
        }
    }
}

/*==============================================================================
����:
    WmsApp_GetClientMsgMO

˵��:
    �������� wms_cdma_user_data_s_type �ṹ��Ϣ���� wms_client_message_s_type ��
    �� MO ��Ϣ��

����:
    pMe [in]: ָ�� WMS Applet����ṹ��ָ�롣�ýṹ����С������ض���Ϣ��
    bSend [in]: ��Ϣ�Ƿ���Ҫ���͡�
    

����ֵ:
    none

��ע:
    �������� buffer ϵ��̬���䣬�ɵ����߸����ͷš�

==============================================================================*/
wms_client_message_s_type *WmsApp_GetClientMsgMO(WmsApp *pMe, boolean bSend)
{
    int32 nSize;
    int32 dwCount;
    wms_client_ts_data_s_type *pCltTsData = NULL;
    wms_client_message_s_type *pCltMsg = NULL;
    wms_cdma_user_data_s_type *pUserdata = NULL;
#ifdef FEATURE_SMS_UDH
    boolean udh_present = FALSE;
#ifdef FEATURE_OEMOMH
    boolean bSupportEMS = nvruim_sms_ems_support();
#endif
#endif
    char  strNum[MAX_PH_DIGITS+20];
    
    if ((IVector_Size(pMe->m_pSendList) == 0) && bSend)
    {// ��Ϣ�ѷ������
        return NULL;
    }
    dwCount = IVector_Size(pMe->m_pUserDataMOList);
#ifdef FEATURE_SMS_UDH
#ifdef FEATURE_OEMOMH
    if(bSupportEMS)
    {
#endif
    if ((dwCount>1) && (pMe->m_idxUserdata == 0))
    {// ���ó����ŵ���Ϣ�ο���
        int  i;
        uint8 msg_ref;
        
        msg_ref = WmsApp_GetMsgRef(pMe);
        for (i=0; i<dwCount; i++)
        {
            pUserdata = (wms_cdma_user_data_s_type *)IVector_ElementAt(pMe->m_pUserDataMOList, i);
            
            if (NULL != pUserdata)
            {
                pUserdata->headers[0].u.concat_8.msg_ref = msg_ref;
            }
        }
    }
    
    if (dwCount>1)
    {
        udh_present = TRUE;
    }
#ifdef FEATURE_OEMOMH
    }
#endif
#endif 
    
    if (pMe->m_idxUserdata >= dwCount)
    {
        return NULL;
    }
    
    pUserdata = (wms_cdma_user_data_s_type *)IVector_ElementAt(pMe->m_pUserDataMOList, pMe->m_idxUserdata);
    if (NULL == pUserdata)
    {
        return NULL;
    }
    pMe->m_idxUserdata++;
    
    nSize = sizeof(wms_client_ts_data_s_type);
    pCltTsData = (wms_client_ts_data_s_type *)sys_malloc(nSize);
    if (NULL == pCltTsData)
    {
        return NULL;
    }
    MEMSET(pCltTsData, 0, nSize);
    
    nSize = sizeof(wms_client_message_s_type);
    pCltMsg = (wms_client_message_s_type *)sys_malloc(nSize);
    if (NULL == pCltMsg)
    {
        WMSAPPU_SYSFREE(pCltTsData);
        return NULL;
    }
    
    // -------- ��� wms_client_ts_data_s_type �ṹ --------
    pCltTsData->format = WMS_FORMAT_CDMA;
    pCltTsData->u.cdma.mask = 0;

    // ��Ϣ ID ����
    pCltTsData->u.cdma.message_id.id_number = WmsApp_GetMsgIDNum(pMe);
    pCltTsData->u.cdma.message_id.type = WMS_BD_TYPE_SUBMIT;
#ifdef FEATURE_SMS_UDH
    pCltTsData->u.cdma.message_id.udh_present = udh_present;
#endif
    pCltTsData->u.cdma.mask |= WMS_MASK_BD_MSG_ID;

    // ��Ϣ�û����ݲ���
    nSize = sizeof(wms_cdma_user_data_s_type);

	//MSG_FATAL("pUserdata->data_len==%d,number_of_digits==%d",pUserdata->data_len,pUserdata->number_of_digits,0);
	//MSG_FATAL("pUserdata->encoding==%d,padding_bits==%d",pUserdata->encoding,pUserdata->padding_bits,0);
	//MSG_FATAL("pUserdata->is91ep_type==%d,num_headers==%d",pUserdata->is91ep_type,pUserdata->num_headers,0);
	
    MEMCPY(&pCltTsData->u.cdma.user_data, pUserdata, nSize);
    pCltTsData->u.cdma.mask |= WMS_MASK_BD_USER_DATA;
    
    // ʱ���
    {
    	uint32	sec = 0;
		
		/*
    	byte	btTimeStamp = 0;
		
    	(void) ICONFIG_GetItem(pMe->m_pConfig,
	                           CFGI_SMS_TIMESTAMP,
	                           &btTimeStamp,
	                           sizeof(btTimeStamp));
		
		if (btTimeStamp == OEMNV_SMS_TIMESTAMP_ADJUST)
		{
			sec = GETUTCSECONDS();
			MSG_FATAL("***zzg GETUTCSECONDS 1 sec=%d***", sec, 0, 0);
		}
		else
		{
			sec = GETTIMESECONDS();
			MSG_FATAL("***zzg GETTIMESECONDS 1 sec=%d***", sec, 0, 0);
		}
		*/

		
		sec = GETTIMESECONDS();	

		MSG_FATAL("***zzg GETUTCSECONDS 1 m_eCreateWMSType=%d, dwSecs=%d***", pMe->m_eCreateWMSType, pMe->m_rsvDateTime.dwSecs, 0);
        
        if ((pMe->m_eCreateWMSType == SEND_MSG_RESERVE) ||
            (pMe->m_eCreateWMSType == SEND_MSG_EDITRESERVE))
        {
            sec = pMe->m_rsvDateTime.dwSecs;
        }

		MSG_FATAL("***zzg GETUTCSECONDS 1 final_sec=%d***", sec, 0, 0);
		
        ConvertMStoMcTime(sec, &pCltTsData->u.cdma.mc_time);
    }
    pCltTsData->u.cdma.mask |= WMS_MASK_BD_MC_TIME;
    
    // ������Чʱ��: TODO
    
    // �����Чʱ��:
    {
        byte   btValidity = OEMNV_SMS_VALIDITYPERIOD_MAX;
        
        (void) ICONFIG_GetItem(pMe->m_pConfig,
                               CFGI_WMS_VALIDITYPERIOD,
                               &btValidity,
                               sizeof(btValidity));
                               
        wms_ts_decode_relative_time(btValidity, &(pCltTsData->u.cdma.validity_relative));
        pCltTsData->u.cdma.mask |= WMS_MASK_BD_VALID_REL;
        #ifdef FEATURE_FLASH_SMS
        if(btValidity == 246)
        {
            //pCltTsData->u.cdma.mask |= WMS_MASK_BD_DISPLAY_MODE;
        }
        #endif
    }
    
    
    // �����ӳ�ʱ��: TODO
    
    
    // ����ӳ�ʱ��: TODO
    
    
    // ���ȼ�: 
    pCltTsData->u.cdma.priority = pMe->m_msSend.m_epriority;
    pCltTsData->u.cdma.mask |= WMS_MASK_BD_PRIORITY;
    
    // ��˽����: TODO
    
    
    // �ظ�ѡ��:
    pCltTsData->u.cdma.reply_option.user_ack_requested = FALSE;
#ifndef FEATURE_CARRIER_TAIWAN_APBW
    pCltTsData->u.cdma.reply_option.delivery_ack_requested = pMe->m_msSend.m_bDeliveryReport;
#else
    pCltTsData->u.cdma.reply_option.delivery_ack_requested = FALSE;
#endif    
    pCltTsData->u.cdma.reply_option.read_ack_requested = FALSE;
    if(pCltTsData->u.cdma.reply_option.delivery_ack_requested)
    {
        pCltTsData->u.cdma.mask |= WMS_MASK_BD_REPLY_OPTION;
    }
    
    pCltTsData->u.cdma.num_messages = 1;
    
#ifndef FEATURE_CARRIER_TAIWAN_APBW
    // Callback Number
    if (WSTRLEN(pMe->m_msSend.m_szCallBkNum)>0)
    {
        MEMSET(strNum, 0, sizeof(strNum));
        (void)WSTRTOSTR(pMe->m_msSend.m_szCallBkNum, strNum, sizeof(strNum));

        MSG_FATAL("***zzg WMSApp strNum[0]=%d, strNum[1]=%d, strNum[2]=%d***", strNum[0], strNum[1], strNum[2]);
        MSG_FATAL("***zzg WMSApp strNum[3]=%d***", strNum[3], 0, 0);

        if (pCltTsData != NULL)
        {
            MSG_FATAL("***zzg WMSApp callback.number_type=%d, callback.digit_mode=%d***", 
                    pCltTsData->u.cdma.callback.number_type, pCltTsData->u.cdma.callback.digit_mode, 0);
        }
        
        //if (strNum[0] == '+')
        if ((strNum[0] == '0') || (strNum[1] == '0') || (strNum[2] == '9')|| (strNum[3] == '1'))
        {
            //pCltTsData->u.cdma.callback.number_of_digits = IWMS_TsAsciiToDtmf(pMe->m_pwms, &strNum[1], pCltTsData->u.cdma.callback.digits);
            pCltTsData->u.cdma.callback.number_of_digits = IWMS_TsAsciiToDtmf(pMe->m_pwms, strNum, pCltTsData->u.cdma.callback.digits);
            pCltTsData->u.cdma.callback.digit_mode = WMS_DIGIT_MODE_8_BIT;
        }
        else
        {
            pCltTsData->u.cdma.callback.number_of_digits = IWMS_TsAsciiToDtmf(pMe->m_pwms, strNum, pCltTsData->u.cdma.callback.digits);
            pCltTsData->u.cdma.callback.digit_mode = WMS_DIGIT_MODE_4_BIT;
        }
        //pCltTsData->u.cdma.callback.digit_mode = WMS_DIGIT_MODE_4_BIT;
        pCltTsData->u.cdma.callback.number_mode = WMS_NUMBER_MODE_NONE_DATA_NETWORK;
        pCltTsData->u.cdma.callback.number_type = WMS_NUMBER_UNKNOWN;
        pCltTsData->u.cdma.callback.number_plan = WMS_NUMBER_PLAN_TELEPHONY;
        pCltTsData->u.cdma.mask |= WMS_MASK_BD_CALLBACK;
    }
#endif    
    
    // -------- ��� wms_client_message_s_type �ṹ --------
    pCltMsg->msg_hdr.message_mode = WMS_MESSAGE_MODE_CDMA;
    if ((pMe->m_eCreateWMSType == SEND_MSG_RESERVE) ||
        (pMe->m_eCreateWMSType == SEND_MSG_EDITRESERVE))
    {
        pCltMsg->msg_hdr.tag = WMS_TAG_RESERVE;
    }
    else
    {
        pCltMsg->msg_hdr.tag = WMS_TAG_MO_NOT_SENT;
    }
    pCltMsg->msg_hdr.mem_store = WMS_MEMORY_STORE_NV_CDMA;
    pCltMsg->msg_hdr.index = WMS_DUMMY_MESSAGE_INDEX;
    
    pCltMsg->u.cdma_message.is_mo = TRUE;
    pCltMsg->u.cdma_message.teleservice = WMS_TELESERVICE_CMT_95;
    
#ifdef FEATURE_SMS_UDH
    if (udh_present)
    {
        pCltMsg->u.cdma_message.concat_8.msg_ref = pUserdata->headers[0].u.concat_8.msg_ref;
        pCltMsg->u.cdma_message.concat_8.total_sm = pUserdata->headers[0].u.concat_8.total_sm;
        pCltMsg->u.cdma_message.concat_8.seq_num = pUserdata->headers[0].u.concat_8.seq_num;
    }
#endif    
#ifdef FEATURE_OEMOMH
#ifdef FEATURE_SMS_UDH
    if(udh_present)
    {
        pCltMsg->u.cdma_message.teleservice = WMS_TELESERVICE_WEMT;
    }
#endif
#endif
    // ���յ�ַ
    MEMSET(strNum, 0, sizeof(strNum));
    MSG_FATAL("......................................",0,0,0);
    (void)WSTRTOSTR(pMe->m_msSend.m_szNum, strNum, sizeof(strNum));

    MSG_FATAL("***zzg WmsApp_GetClientMsgMO strNum[0]=%d, strNum[1]=%d, strNum[2]=%d***", strNum[0], strNum[1], strNum[2]);
    MSG_FATAL("***zzg WmsApp_GetClientMsgMO strNum[3]=%d***", strNum[3], 0, 0);

    if (pCltMsg != NULL)
    {
        MSG_FATAL("***zzg WmsApp_GetClientMsgMO address.number_type=%d, address.digit_mode=%d***", 
                pCltMsg->u.cdma_message.address.number_type, pCltMsg->u.cdma_message.address.digit_mode, 0);
    }
   
    
    if (strNum[0] == '+')    {
        char * pcBuf = (char*)(strNum + 1);
         
        //pCltMsg->u.cdma_message.address.number_of_digits = IWMS_TsAsciiToDtmf(pMe->m_pwms, strNum, pCltMsg->u.cdma_message.address.digits);
        pCltMsg->u.cdma_message.address.number_of_digits = (byte)STRLEN((char *)strNum) -1;
        
        MEMSET((byte*)pCltMsg->u.cdma_message.address.digits, 0, sizeof(pCltMsg->u.cdma_message.address.digits));
        // Remove the plus character and copy
        STRNCPY((char*)pCltMsg->u.cdma_message.address.digits, pcBuf, sizeof(pCltMsg->u.cdma_message.address.digits) - 1);        

        pCltMsg->u.cdma_message.address.number_type = WMS_NUMBER_INTERNATIONAL;
        pCltMsg->u.cdma_message.address.digit_mode = WMS_DIGIT_MODE_8_BIT; 
    }
    else
    {
        pCltMsg->u.cdma_message.address.number_of_digits = IWMS_TsAsciiToDtmf(pMe->m_pwms, strNum, pCltMsg->u.cdma_message.address.digits);
        pCltMsg->u.cdma_message.address.number_type = WMS_NUMBER_UNKNOWN;
        pCltMsg->u.cdma_message.address.digit_mode = WMS_DIGIT_MODE_4_BIT;
    }
    //pCltMsg->u.cdma_message.address.digit_mode = WMS_DIGIT_MODE_4_BIT;
    pCltMsg->u.cdma_message.address.number_mode = WMS_NUMBER_MODE_NONE_DATA_NETWORK;
    pCltMsg->u.cdma_message.address.number_plan = WMS_NUMBER_PLAN_TELEPHONY;
    
    pCltMsg->u.cdma_message.is_tl_ack_requested = TRUE;
    pCltMsg->u.cdma_message.is_service_present = FALSE;

    (void)IWMS_TsEncode(pMe->m_pwms, pCltTsData, &pCltMsg->u.cdma_message.raw_ts);
	//MSG_FATAL("IWMS_TsEncode======%d",0,0,0);
    
    // ��Ϣ�������
    
    WMSAPPU_SYSFREE(pCltTsData);
    
    return pCltMsg;
}

/*==============================================================================
����:
    WmsApp_ProcessStatus

˵��:
    ������������ wms �ı�����Ϣ��

����:
    pMe [in]: ָ�� WMS Applet����ṹ��ָ�롣�ýṹ����С������ض���Ϣ��
    pRptInfo [in]: wms_submit_report_info_s_type �ṹָ�롣

����ֵ:
    none

��ע:

==============================================================================*/
void WmsApp_ProcessStatus(WmsApp *pMe, wms_submit_report_info_s_type *pRptInfo)
{
    pMe->m_SendStatus = pRptInfo->report_status;
    pMe->m_SendtlStatus = pRptInfo->cause_info.tl_status;
    
    switch (pRptInfo->report_status)
    {
        // Success: submit_report_ack info is available for GSM/WCDMA
        case WMS_RPT_OK:
        // Lower layer errors;
        // For GSM/WCDMA: cause_value = LL Cause, not available to clients.
        case WMS_RPT_LL_ERROR:
        
        case WMS_RPT_OUT_OF_RESOURCES:
        case WMS_RPT_SMR_NO_RESOURCES:

        case WMS_RPT_NETWORK_NOT_READY:

        case WMS_RPT_PHONE_NOT_READY:

        case WMS_RPT_NO_ACK:

        case WMS_RPT_CDMA_TL_ERROR:
            break;

        case WMS_RPT_ACCESS_TOO_LARGE:
            // �ӽ����ŵ�����ʧ�ܣ�����ҵ���ŵ�
//            if (WMSAPPMN_DcConnect(pMe, pMe->m_AutoDcSendSO))
//            {
//                return TRUE;
//            }
        /* Fall Through */
            return;

        case WMS_RPT_DC_TOO_LARGE:

        case WMS_RPT_NOT_ALLOWED_IN_AMPS:
            
        case WMS_RPT_CANNOT_SEND_BROADCAST:
            
        case WMS_RPT_RP_ERROR:
            
        case WMS_RPT_CP_ERROR:
            
        case WMS_RPT_SMR_TR1M_TIMEOUT:
            
        case WMS_RPT_SMR_TR2M_TIMEOUT:
            
        // Any other error
        default:
            break;
    }      
    
    ERR("pRptInfo->report_status = %d", pRptInfo->report_status, 0, 0);
    
    if (pRptInfo->report_status != WMS_RPT_OK && 
        pMe->m_nDisconnectedInSendingRetryTimes==0 &&
        pMe->m_bDCDisconnectedInSending)
    {
        int nRet;

        pMe->m_bDCDisconnectedInSending = FALSE;
        // ������½���Ϣ�ط�һ��
        if ((NULL != pMe->m_pCurSendCltMsg) &&
            (pMe->m_idxCurSend < pMe->m_nSendItems))
        {
            pMe->m_nDisconnectedInSendingRetryTimes = 1;
            nRet = IWMS_MsgSend(pMe->m_pwms, 
                                pMe->m_clientId, 
                                &pMe->m_callback,
                                (void*)pMe,
                                WMS_SEND_MODE_CLIENT_MESSAGE,
                                pMe->m_pCurSendCltMsg[pMe->m_idxCurSend]);
                                
            if (nRet == SUCCESS)
            {
                pMe->m_bSending = TRUE;
                return;
            }
        }
    }
    
    if ((pRptInfo->report_status == WMS_RPT_OK) &&
        (pMe->m_eCreateWMSType != SEND_MSG_RESERVE))
    {// �޸���Ϣ��ʶ    	
		
        if ((NULL != pMe->m_pCurSendCltMsg) &&
            (pMe->m_idxCurSend < pMe->m_nSendItems))
        {
            wms_client_message_s_type *pItem = pMe->m_pCurSendCltMsg[pMe->m_idxCurSend];
             
            if ((NULL != pItem) &&
                (pItem->msg_hdr.index != WMS_DUMMY_MESSAGE_INDEX))
            {
                (void)IWMS_MsgModifyTag(pMe->m_pwms,
                                        pMe->m_clientId,
                                        &pMe->m_callback,
                                        (void *)pMe,
                                        pItem->msg_hdr.mem_store,
                                        pItem->msg_hdr.index,
                                        WMS_TAG_MO_SENT);
            }
        }
    }
    
    pMe->m_nDisconnectedInSendingRetryTimes = 0;
    
    if ((pMe->m_wActiveDlgID == IDD_SENDING) &&
        (ISHELL_ActiveApplet(pMe->m_pShell) == AEECLSID_WMSAPP))
    {
        (void)ISHELL_PostEventEx(pMe->m_pShell,
                                 EVTFLG_ASYNC, 
                                 AEECLSID_WMSAPP, 
                                 EVT_UPDATE,
                                 0, 
                                 0);
    }
    else
    {
        if (pMe->m_eCreateWMSType == SEND_MSG_RESERVE)
        {// ��ԤԼʧ�ܴ���
            WmsApp_ReservedMsgSetTimer(pMe);
        }
    }
}

/*==============================================================================
����:
    WmsApp_MemberReset

˵��:
    ������ WmsApp �ṹ���ֳ�Ա�����������á�

����:
    pMe [in]: ָ�� WMS Applet����ṹ��ָ�롣�ýṹ����С������ض���Ϣ��

����ֵ:
    none

��ע:

==============================================================================*/
void WmsApp_MemberReset(WmsApp *pMe)
{
    if (NULL == pMe)
    {
        return;
    }
    
    // ȡ�����Ͷ��ŵĶ�ʱ��
    (void)ISHELL_CancelTimer(pMe->m_pShell, WmsApp_MultSendMsgTimer, pMe);
    (void)ISHELL_CancelTimer(pMe->m_pShell, WmsApp_ReSendMsgTimer, pMe);
    
    if (NULL != pMe->m_pCurCltPhrase)
    {
        FREE(pMe->m_pCurCltPhrase);
        pMe->m_pCurCltPhrase = NULL;
    }
    
    WMSMessageStruct_Reset(&pMe->m_msCur);
    
    MEMSET(pMe->m_CurMsgNodes, 0, sizeof(pMe->m_CurMsgNodes));
    WmsApp_FreeMsgNodeMs(pMe);

    pMe->m_prevState = 0;
    pMe->m_bDoNotOverwriteDlgResult = FALSE;
    pMe->m_eMBoxType = WMS_MB_NONE;
    pMe->m_eMakeListMode = MAKEMSGLIST_INIT;
}

/*==============================================================================
������
    WmsApp_UpdateAnnunciators

˵����
    ���ڵ�ǰ��������(���������ʼ�)����������ʾͼ�ꡣ

������
    pMe [in]: ָ�� WMS Applet����ṹ��ָ�롣�ýṹ����С������ض���Ϣ��

����ֵ��
    �ޡ�

��ע��
    ��ʹApplet������Ҳ����¡�
        
==============================================================================*/
void WmsApp_UpdateAnnunciators(WmsApp * pMe)
{
    uint16  nNews = 0, nVmNews = 0, nVmMsgs = 0;
    uint16  nMsgs=0;
    uint16  nOnUIMs=0;
    wms_routing_s_type  rt;
    
    // 0-full 1-VM 2-SMS
    boolean smsiconstatus[3] = {0};
    
    if ((!pMe->m_bCdmaWmsReady) || (pMe->m_refresh_in_progress))
    {// ��ʼ����û���
        return;
    }
    
    rt.route = WMS_ROUTE_STORE_AND_NOTIFY;
    rt.mem_store = WMS_MEMORY_STORE_NV_CDMA;
    
    wms_cacheinfolist_getcounts(WMS_MB_VOICEMAIL, &nVmNews, NULL, &nVmMsgs);
    wms_cacheinfolist_getcounts(WMS_MB_INBOX, &nNews, &nOnUIMs, &nMsgs);
#ifdef FEATURE_LED_CONTROL
    if ((nVmNews>0) || (nNews>0))
    {
        IBACKLIGHT_SigLedDisable(pMe->m_pBacklight);    //cancel the previous LED control
        IBACKLIGHT_SigLedEnable(pMe->m_pBacklight, SIG_LED_SMS);
        IBACKLIGHT_Enable(pMe->m_pBacklight);
    }
    else
    {
        IBACKLIGHT_SigLedDisable(pMe->m_pBacklight);
    }
#endif /*FEATURE_LED_CONTROL */    
    
    // �������֪ͨ����
#if defined(FEATURE_CARRIER_VENEZUELA_MOVILNET)
    if (nVmMsgs)
#else
    if (nVmNews || gbWmsVMailNtf)
#endif        
    {
        smsiconstatus[1] = TRUE;
    }
    
    // ����ռ���
    if (nNews)
    {
        smsiconstatus[2] = TRUE;
        
    }
    
    // �洢�ռ����ؼ��
#ifdef FEATURE_CDSMS_RUIM
    if (IsRunAsUIMVersion())
    {// �п��汾
        uint16  nInMsgs=0;
        
        (void) ICONFIG_GetItem(pMe->m_pConfig,
                               CFGI_WMS_MEMSTORE,
                               &rt.mem_store,
                               sizeof(rt.mem_store));

        nInMsgs = nMsgs - nOnUIMs;
        if (rt.mem_store == WMS_MEMORY_STORE_RUIM)
        {// �û�����ʹ��RUIM
            if (pMe->m_memruimStatuse.used_tag_slots >= pMe->m_memruimStatuse.max_slots)
            {// UIM �洢�ռ�����
#if defined(FEATURE_VERSION_C337) || defined(FEATURE_VERSION_IC241A_MMX)|| defined(FEATURE_VERSION_K232_Y100A)|| defined (FEATURE_VERSION_KK5)        
                if (nInMsgs >= IN_MAX-1)
                {// �洢�ռ�����
 		    wms_cache_info_list   *pList = NULL;
 		    wms_cache_info_node   *pnode = NULL;
		    int nRet=EFAILED;	
		    int i = 0;	
 		    MSG_FATAL("WmsApp_UpdateAnnunciators nInMsgs=%d", nInMsgs, 0, 0);
 		    pList = wms_get_cacheinfolist(WMS_MB_INBOX);
 		    if (NULL == pList)
 		    {
 		    	 MSG_FATAL("WmsApp_UpdateAnnunciators NULL == pList", 0, 0, 0);
 		    }
 		    else
 		    {
	 		    pnode = pList->pHead;
	 		    if(pnode == NULL)
	 		    {
	 		    	MSG_FATAL("WmsApp_UpdateAnnunciators1 pnode == NULL", 0, 0, 0);
	 		    }
	 		    while (NULL != pnode)
	 		    {
	 		    	MSG_FATAL("mem_store=%d, msg_tag=%x", pnode->mem_store , pnode->msg_tag, 0);
	 		        if ((++i <= IN_MAX) && (pnode->mem_store == WMS_MEMORY_STORE_NV_CDMA) &&
				     (pnode->msg_tag != WMS_TAG_MT_NOT_READ))
	 		        {
	 		            MSG_FATAL("WmsApp_UpdateAnnunciators pnode != NULL", 0, 0, 0); 	
	 		            break;
	 		        }
	 		        
	 		        pnode = pnode->pNext;
	 		    }
			    MSG_FATAL("WmsApp_UpdateAnnunciators i=%d", i, 0, 0); 	
	 		    if(pnode == NULL)
	 		    {
	 		    	MSG_FATAL("WmsApp_UpdateAnnunciators2 pnode == NULL", 0, 0, 0);
	 		    }
			    else
			    {
#ifdef FEATURE_SMS_UDH
				if (pnode->pItems != NULL)
				{
					MEMCPY(pMe->m_CurMsgNodes, pnode->pItems, sizeof(pMe->m_CurMsgNodes));
					MSG_FATAL("pnode->pItems != NUL",0,0,0);
					for (; pMe->m_idxCur<LONGSMS_MAX_PACKAGES; pMe->m_idxCur++)
					{
						if (pMe->m_CurMsgNodes[pMe->m_idxCur] != NULL)
						{
							pnode = pMe->m_CurMsgNodes[pMe->m_idxCur];
							break;
						}
					}
				}
				else
#endif
				{
					MSG_FATAL("pnode->pItems is  NULL",0,0,0);
					pMe->m_CurMsgNodes[0] = pnode;
				}
				for (i=0; i<LONGSMS_MAX_PACKAGES; i++)
				{
					if (pMe->m_CurMsgNodes[i] != NULL)
					{
						pnode = pMe->m_CurMsgNodes[i];
						// ����ɾ����Ϣ����
						nRet = ENOMEMORY;
						do
						{
							nRet = IWMS_MsgDelete(pMe->m_pwms,
							pMe->m_clientId,
							&pMe->m_callback,
							(void *)pMe,
							pnode->mem_store,
							pnode->index);
							MSG_FATAL("DELEING...................",0,0,0);
						} while(nRet != SUCCESS);
						MSG_FATAL("DELEING...............nRet=%d",nRet,0,0);
						pMe->m_CurMsgNodes[i] = NULL;
					}
				}			    
			    }
 		    }
                    rt.route = WMS_ROUTE_STORE_AND_NOTIFY;
                    rt.mem_store = WMS_MEMORY_STORE_NV_CDMA;			
                 //   rt.route = WMS_ROUTE_TRANSFER_ONLY;
               //     smsiconstatus[0] = TRUE;
                }
#else
		if (nInMsgs >= IN_MAX)
		{
                    rt.route = WMS_ROUTE_TRANSFER_ONLY;
                    smsiconstatus[0] = TRUE;		
		}
#endif
                else
                {// �����ռ��仹�д洢�ռ�
                    rt.route = WMS_ROUTE_STORE_AND_NOTIFY;
                    rt.mem_store = WMS_MEMORY_STORE_NV_CDMA;
                }
            }
            else
            {// RUIM �����пռ�
                rt.route = WMS_ROUTE_STORE_AND_NOTIFY;
            }
        }
        else
        {// �û�����ʹ���ֻ��ڴ�
#if defined(FEATURE_VERSION_C337) || defined(FEATURE_VERSION_IC241A_MMX)|| defined(FEATURE_VERSION_K232_Y100A) || defined (FEATURE_VERSION_KK5)             
                if (nInMsgs >= IN_MAX-1)
                {// �洢�ռ�����
                   if (pMe->m_memruimStatuse.used_tag_slots >= pMe->m_memruimStatuse.max_slots)
                   {
	 		    wms_cache_info_list   *pList = NULL;
	 		    wms_cache_info_node   *pnode = NULL;
			    int nRet=EFAILED;	
			    int i = 0;	
	 		    MSG_FATAL("WmsApp_UpdateAnnunciators nInMsgs=%d", nInMsgs, 0, 0);
	 		    pList = wms_get_cacheinfolist(WMS_MB_INBOX);
	 		    if (NULL == pList)
	 		    {
	 		    	 MSG_FATAL("WmsApp_UpdateAnnunciators NULL == pList", 0, 0, 0);
	 		    }
	 		    else
	 		    {
		 		    pnode = pList->pHead;
		 		    if(pnode == NULL)
		 		    {
		 		    	MSG_FATAL("WmsApp_UpdateAnnunciators1 pnode == NULL", 0, 0, 0);
		 		    }
		 		    while (NULL != pnode)
		 		    {
		 		    	MSG_FATAL("mem_store=%d, msg_tag=%x", pnode->mem_store , pnode->msg_tag, 0);
		 		        if ((++i <= IN_MAX) && (pnode->mem_store == WMS_MEMORY_STORE_NV_CDMA) &&
					     (pnode->msg_tag != WMS_TAG_MT_NOT_READ))
		 		        {
		 		            MSG_FATAL("WmsApp_UpdateAnnunciators pnode != NULL", 0, 0, 0); 	
		 		            break;
		 		        }
		 		        
		 		        pnode = pnode->pNext;
		 		    }
				    MSG_FATAL("WmsApp_UpdateAnnunciators i=%d", i, 0, 0); 	
		 		    if(pnode == NULL)
		 		    {
		 		    	MSG_FATAL("WmsApp_UpdateAnnunciators2 pnode == NULL", 0, 0, 0);
		 		    }
				    else
				    {
#ifdef FEATURE_SMS_UDH
					if (pnode->pItems != NULL)
					{
						MEMCPY(pMe->m_CurMsgNodes, pnode->pItems, sizeof(pMe->m_CurMsgNodes));
						MSG_FATAL("pnode->pItems != NUL",0,0,0);
						for (; pMe->m_idxCur<LONGSMS_MAX_PACKAGES; pMe->m_idxCur++)
						{
							if (pMe->m_CurMsgNodes[pMe->m_idxCur] != NULL)
							{
								pnode = pMe->m_CurMsgNodes[pMe->m_idxCur];
								break;
							}
						}
					}
					else
#endif
					{
						MSG_FATAL("pnode->pItems is  NULL",0,0,0);
						pMe->m_CurMsgNodes[0] = pnode;
					}
					for (i=0; i<LONGSMS_MAX_PACKAGES; i++)
					{
						if (pMe->m_CurMsgNodes[i] != NULL)
						{
							pnode = pMe->m_CurMsgNodes[i];
							// ����ɾ����Ϣ����
							nRet = ENOMEMORY;
							do
							{
								nRet = IWMS_MsgDelete(pMe->m_pwms,
								pMe->m_clientId,
								&pMe->m_callback,
								(void *)pMe,
								pnode->mem_store,
								pnode->index);
								MSG_FATAL("DELEING...................",0,0,0);
							} while(nRet != SUCCESS);
							MSG_FATAL("DELEING...............nRet=%d",nRet,0,0);
							pMe->m_CurMsgNodes[i] = NULL;
						}
					}			    
				    }
	 		    }
	                    rt.route = WMS_ROUTE_STORE_AND_NOTIFY;
	                    rt.mem_store = WMS_MEMORY_STORE_NV_CDMA;			
	                }
			  else
			  {
	                    rt.route = WMS_ROUTE_STORE_AND_NOTIFY;
	                    rt.mem_store = WMS_MEMORY_STORE_RUIM;			  
			  }
                }
#else		
            if (nInMsgs >= IN_MAX)
            {// �����ռ�������
                if (pMe->m_memruimStatuse.used_tag_slots >= pMe->m_memruimStatuse.max_slots)
                {// �洢�ռ�����
                    rt.route = WMS_ROUTE_TRANSFER_ONLY;
                    smsiconstatus[0] = TRUE;
                }
                else
                {// RUIM �����пռ�
                    rt.route = WMS_ROUTE_STORE_AND_NOTIFY;
                    rt.mem_store = WMS_MEMORY_STORE_RUIM;
                }
            }
#endif			
            else
            {// �����ռ��仹�д洢�ռ�
                rt.route = WMS_ROUTE_STORE_AND_NOTIFY;
            }
        }
    }
    else
#endif   
    {// �޿��汾
        if (nMsgs >= IN_MAX)
        {// �洢�ռ�����
            rt.route = WMS_ROUTE_TRANSFER_ONLY;
            smsiconstatus[0] = TRUE;
        }
    }
    
    if ((pMe->m_routes.pp_routes[WMS_MESSAGE_CLASS_CDMA].mem_store != rt.mem_store) ||
        (rt.route != pMe->m_routes.pp_routes[WMS_MESSAGE_CLASS_CDMA].route))
    {
        pMe->m_routes.pp_routes[WMS_MESSAGE_CLASS_CDMA].route = rt.route;
        pMe->m_routes.pp_routes[WMS_MESSAGE_CLASS_CDMA].mem_store = rt.mem_store;
        
        if (rt.route != WMS_ROUTE_TRANSFER_ONLY)
        {
            (void)IWMS_CfgSetRoutes(pMe->m_pwms,
                                   pMe->m_clientId,
                                   &pMe->m_callback,
                                   (void*)pMe,
                                   &pMe->m_routes);
        }
                               
        (void)IWMS_SetMemoryFull(pMe->m_pwms, 
                                 pMe->m_clientId, 
                                 &pMe->m_callback,
                                 (void*)pMe,
                                 (rt.route == WMS_ROUTE_TRANSFER_ONLY ? TRUE : FALSE));
    }

    MSG_FATAL("WmsApp_UpdateAnnunciators %d %d %d",smsiconstatus[0],smsiconstatus[1],smsiconstatus[2]);
    if (smsiconstatus[0])
    {
        IANNUNCIATOR_SetField(pMe->m_pIAnn, ANNUN_FIELD_SMS, ANNUN_STATE_SMS_MAILFULL_ON|ANNUN_STATE_BLINK);
    }
    else if (smsiconstatus[1])
    {
        IANNUNCIATOR_SetField(pMe->m_pIAnn, ANNUN_FIELD_SMS, ANNUN_STATE_SMS_VMAIL_ON);
    }
    else if (smsiconstatus[2])
    {
        IANNUNCIATOR_SetField(pMe->m_pIAnn, ANNUN_FIELD_SMS, ANNUN_STATE_SMS_SMAIL_ON);
    }
    else
    {
        IANNUNCIATOR_SetField(pMe->m_pIAnn, ANNUN_FIELD_SMS, ANNUN_STATE_OFF);
    }
}

/*==============================================================================
������
    WmsApp_GetmemAlertID

˵����
    ������ȡ��ǰ�洢�ռ�澯�ַ�������Դ�ļ��е� ID��

������
    pMe [in]: ָ�� WMS Applet����ṹ��ָ�롣�ýṹ����С������ض���Ϣ��
    eBox[in]: wms_box_e_type

����ֵ��
    �澯�ַ�������Դ�ļ��е� ID��

��ע��
    ������ֵΪ0����ʾ����Ҫ�澯
        
==============================================================================*/
uint16 WmsApp_GetmemAlertID(WmsApp * pMe, wms_box_e_type eBox)
{
    uint16  nMsgs=0;
    uint16  nOnUIMs=0;
    uint16  nMsgID = 0;
    MSG_FATAL("WmsApp_GetmemAlertID Start eBox=%d",eBox,0,0);
    // ��ȡ��Ϣ��
    wms_cacheinfolist_getcounts(eBox, NULL, &nOnUIMs, &nMsgs);
    
    switch (eBox)
    {
        case WMS_MB_VOICEMAIL:
            if (nMsgs == 0)
            {
                nMsgID = IDS_EMPTY;
            }
            break;
            
        case WMS_MB_INBOX:   
            if (nMsgs == 0)
            {
                nMsgID = IDS_EMPTY;
            }
#ifdef FEATURE_CARRIER_THAILAND_HUTCH 
#ifdef FEATURE_CDSMS_RUIM
            else if (IsRunAsUIMVersion())
            {
                uint16  nInMsgs=0;
                nInMsgs = nMsgs - nOnUIMs;
                if ((nInMsgs >= IN_MAX) && (pMe->m_memruimStatuse.used_tag_slots >= pMe->m_memruimStatuse.max_slots))
                {
                    nMsgID = IDS_INBOXSFULL;
                }  
            }
            else
#endif //FEATURE_CDSMS_RUIM                
            {
                if (nMsgs >= IN_MAX)
                {
                    nMsgID = IDS_INBOXSFULL;
                }              
            }
#else //FEATURE_CARRIER_THAILAND_HUTCH            
#ifdef FEATURE_CDSMS_RUIM
            else if (IsRunAsUIMVersion())
            {
                uint16  nInMsgs=0;
                wms_memory_store_e_type   mem_store = WMS_MEMORY_STORE_RUIM;

                (void) ICONFIG_GetItem(pMe->m_pConfig,
                                       CFGI_WMS_MEMSTORE,
                                       &mem_store,
                                       sizeof(mem_store));
                nInMsgs = nMsgs - nOnUIMs;
                
                if (mem_store == WMS_MEMORY_STORE_RUIM)
                {
                    if ((pMe->m_memruimStatuse.max_slots>0) && (pMe->m_memruimStatuse.used_tag_slots >= pMe->m_memruimStatuse.max_slots))
                    {
                    	
                        if (nInMsgs >= IN_MAX)
                        {
                            nMsgID = IDS_INBOXSFULL;
                        }
                        else
                        {
                            nMsgID = IDS_RUIMFULL;
                        }
                    }
                    else if ((pMe->m_memruimStatuse.max_slots>0)&&(pMe->m_memruimStatuse.used_tag_slots >= (pMe->m_memruimStatuse.max_slots*8/10)))
                    {
                        nMsgID = IDS_RUIMWILLFULL;
                    }
                    else if (nInMsgs >= IN_MAX)
                    {
                        nMsgID = IDS_INBOXSFULL;
                    }
                    else if (nInMsgs >= IN_WATERMARK)
                    {
                        nMsgID = IDS_INBOXWILLFULL;
                    }
                }
                else
                {
                    if (nInMsgs >= IN_MAX)
                    {
                        if (pMe->m_memruimStatuse.used_tag_slots >= pMe->m_memruimStatuse.max_slots)
                        {
                            nMsgID = IDS_INBOXSFULL;
                        }
                        else
                        {
                            nMsgID = IDS_INBOXSFULL;
                        }
                    }
                    else if (nInMsgs >= IN_WATERMARK)
                    {
                        nMsgID = IDS_INBOXWILLFULL;
                    }
                    else if (pMe->m_memruimStatuse.max_slots>0&&(pMe->m_memruimStatuse.used_tag_slots >= pMe->m_memruimStatuse.max_slots))//modi by yangdecai 2010-08-05
                    {
                        nMsgID = IDS_RUIMFULL;
                    }
                    else if ((pMe->m_memruimStatuse.max_slots>0)&&(pMe->m_memruimStatuse.used_tag_slots >= (pMe->m_memruimStatuse.max_slots*8/10)))
                    {
                        nMsgID = IDS_RUIMWILLFULL;
                    }
                }
            }
            else
#endif   
            {
                if (nMsgs >= IN_MAX)
                {
                    nMsgID = IDS_INBOXSFULL;
                }
                else if (nMsgs >= IN_WATERMARK)
                {
                    nMsgID = IDS_INBOXWILLFULL;
                }
            }
#endif //#if defined FEATURE_CARRIER_THAILAND_HUTCH  
            break;
            
        case WMS_MB_OUTBOX:   
            if (nMsgs == 0)
            {
                nMsgID = IDS_EMPTY;
            }
            else if (nMsgs-nOnUIMs >= OUT_MAX)
            {
                nMsgID = IDS_OUTBOXFULL;
            }
            else if (nMsgs-nOnUIMs >= OUT_WATERMARK)
            {
                nMsgID = IDS_OUTBOXWILLFULL;
            }
            break;
            
        case WMS_MB_DRAFT:   
            if (nMsgs == 0)
            {
                nMsgID = IDS_EMPTY;
            }
            else if (nMsgs >= DRAFT_MAX)
            {
                nMsgID = IDS_DRAFTFULL;
            }
            break;
    
#ifdef FEATURE_USES_MMS
        case WMS_MB_INBOX_MMS:
        case WMS_MB_OUTBOX_MMS: 
        case WMS_MB_DRAFTBOX_MMS:
            MSG_FATAL("WMS_MB_OUTBOX_MMS g_mmsDataInfoMax=%d",g_mmsDataInfoMax,0,0);
            if (g_mmsDataInfoMax == 0)
            {
                nMsgID = IDS_EMPTY;
            }
/*            
            else if (g_mmsDataInfoMax >= 10)
            {
                nMsgID = IDS_OUTBOXFULL;
            }
*/            
            break;
#endif
        default:
            break;
    }
    
    return  nMsgID;
}

/*==============================================================================
������
    WmsApp_PlaySMSAlert

˵����
    �����������Ŷ�����ʾ��

������
    pMe [in]: ָ�� WMS Applet����ṹ��ָ�롣�ýṹ����С������ض���Ϣ��
    bsmsin[in]: TRUE - ����  FALSE - ȥ��

����ֵ��
    none

��ע��
        
==============================================================================*/
void WmsApp_PlaySMSAlert(WmsApp * pMe, boolean bsmsin)
{
    byte    btActiveProfile;
    ringID  SmsRingerID[PROFILENUMBER]={0};

    IBACKLIGHT_Enable(pMe->m_pBacklight); 

#ifdef FEATURE_VIDEOPLAYER
#ifndef WIN32
#ifdef FEATURE_VERSION_W208S              
     if(ISHELL_ActiveApplet(pMe->m_pShell) == AEECLSID_DIALER)
     {
        MSG_FATAL("WmsApp_PlaySMSAlert AEECLSID_DIALER",0,0,0);
        ISOUND_Vibrate(pMe->m_pSound,TIME_MS_SMSVIBRATE);
        return;
     }
#endif     
     if(GetDeviceState(DEVICE_TYPE_MP4) == DEVICE_MP4_STATE_ON)
     {
         ISOUND_Vibrate(pMe->m_pSound,TIME_MS_SMSVIBRATE);
         return;
     }
#endif
#endif
    (void) ICONFIG_GetItem(pMe->m_pConfig,
                    CFGI_PROFILE_CUR_NUMBER,
                    &btActiveProfile,
                    sizeof(btActiveProfile));

    (void) ICONFIG_GetItem(pMe->m_pConfig,
                    CFGI_PROFILE_SMS_RINGER_ID, 
                    (void*)SmsRingerID, 
                    sizeof(SmsRingerID));
    
    if (btActiveProfile<PROFILENUMBER)
    {
    	extern boolean MediaGallery_CheckUdiskStat(void);
        // IALERT_StopMp3Alert(pMe->m_pAlert);
        if(MediaGallery_CheckUdiskStat())
        {
            (void) IALERT_StartSMSAlert(pMe->m_pAlert, SmsRingerID[btActiveProfile].midID);
        }
        else
        {
            if(SmsRingerID[btActiveProfile].ringType == OEMNV_MP3_RINGER)
            {

                // ע�ᰴ��notify�¼������а���ʱ�ر�MP3������ʾ��
                MSG_FATAL("IALERT_StartSMSAlert::::::::::::::::::::111111111111111111111111111",0,0,0);
                (void)ISHELL_RegisterNotify(pMe->m_pShell, 
                                            AEECLSID_WMSAPP,
                                            AEECLSID_SHELL,
                                            ( ( (uint32)(NOTIFIER_VAL_ANY) ) << 16 ) | NMASK_SHELL_KEY);
                    
                if ((IALERT_StartMp3Alert(pMe->m_pAlert, SmsRingerID[btActiveProfile].szMusicname,ALERT_SMS_SND) != SUCCESS))
                {
                	MSG_FATAL("IALERT_StartSMSAlert::::::::::::::::::::111111111",0,0,0);
                    (void) IALERT_StartSMSAlert(pMe->m_pAlert, SmsRingerID[btActiveProfile].midID);
                }            
            }
            else
            {
            	MSG_FATAL("IALERT_StartSMSAlert::::::::::::::::::::2222222222222222222",0,0,0);
                (void) IALERT_StartSMSAlert(pMe->m_pAlert, SmsRingerID[btActiveProfile].midID);
            }
        }

    }    
}

/*==============================================================================
����:
    WmsApp_FreeSendClentMsgList

˵��:
    �ͷŷ��Ϳͻ���Ϣ���е�ȫ�������

����:
    pMe [in]: ָ�� WMS Applet����ṹ��ָ�롣�ýṹ����С������ض���Ϣ��

����ֵ:
    none

��ע:
       
==============================================================================*/
void WmsApp_FreeSendClentMsgList(WmsApp * pMe)
{
    int    i;
    wms_client_message_s_type *pItem = NULL;

    if (pMe->m_pCurSendCltMsg == NULL)
    {
        pMe->m_idxCurSend = 0;
        pMe->m_nSendItems = 0;
        return;
    }
    
    for (i=0; i<pMe->m_nSendItems; i++)
    {
        pItem = pMe->m_pCurSendCltMsg[i];
        if (NULL != pItem)
        {
            // �ͷŶ�̬����Ŀռ�
            WMSAPPU_SYSFREE(pItem);
        }
    }
    
    pMe->m_idxCurSend = 0;
    pMe->m_nSendItems = 0;
    
    WMSAPPU_SYSFREE(pMe->m_pCurSendCltMsg);
    pMe->m_pCurSendCltMsg = NULL;
}

/*==============================================================================
����:
    WmsApp_BuildSendClentMsgList

˵��:
    �������Ϳͻ���Ϣ�б�

����:
    pMe [in]: ָ�� WMS Applet����ṹ��ָ�롣�ýṹ����С������ض���Ϣ��

����ֵ:
    none

��ע:
       
==============================================================================*/
extern char charsvc_p_name[UIM_CDMA_HOME_SERVICE_SIZE+1];
void WmsApp_BuildSendClentMsgList(WmsApp * pMe)
{
    int    i;
    int32  nItems = 0;
    int32  nSize;
    CMultiSendItemInfo *pItem = NULL;
    wms_client_message_s_type *pClientMsg = NULL;
	AECHAR TempTo[MAX_EMAILADD_LEN+1] = {0};
	AECHAR TempToStr[MAX_EMAILADD_LEN+1] = {0};
	char temp[MAX_EMAILADD_LEN+1] = {0};

    MSG_FATAL("***zzg WmsApp_BuildSendClentMsgList***", 0, 0, 0);

    WmsApp_FreeSendClentMsgList(pMe);
    
    if (0 == IVector_Size(pMe->m_pSendList))
    {// ���յ�ַ�б�Ϊ��
        return;
    }
    
    nItems = IVector_Size(pMe->m_pUserDataMOList);
    if (0 == nItems)
    {
        return;
    }
    
    pItem = (CMultiSendItemInfo *)IVector_ElementAt(pMe->m_pSendList, 0);
    
    if (NULL == pItem)
    {
        goto BuildList_ERR;
    }

    // ������ǰ����
    pMe->m_msSend.m_szNum[0] = 0;
	WSTRTOSTR(pItem->m_szTo,temp,MAX_EMAILADD_LEN+1);
    
#ifdef FEATURE_VERSION_K232_Y101
    if(STRISTR(temp,"+"))
    {
        /*
        boolean b_Prefix = FALSE;
        ICONFIG_GetItem(pMe->m_pConfig, CFGI_PREFIX_AUTO_MANUAL, &b_Prefix,  sizeof(boolean));        
       
        MSG_FATAL("b_Prefix==============%d",b_Prefix,0,0);
        
        if (!b_Prefix)
        {
            AECHAR wstr[FEATURE_CODE_MAX_LENTH];
            ICONFIG_GetItem(pMe->m_pConfig, CFGI_PREFIX, wstr, FEATURE_CODE_MAX_LENTH);           

            WSTRCPY(TempToStr,wstr);
    		WSTRCPY(TempTo,pItem->m_szTo+1);
    		WSTRCAT(TempToStr,TempTo);
    		DBGPRINTF("TempToStr=%S", TempTo);
    		DBGPRINTF("TempToStr=%S", TempToStr);
    		DBGPRINTF("pItem->m_szTo=%S", pItem->m_szTo);
    		MEMSET(pItem->m_szTo,0,(sizeof(pItem->m_szTo))+1);
    		WSTRCPY(pItem->m_szTo,TempToStr);
    		DBGPRINTF("pItem->m_szTo=%S", pItem->m_szTo);
    		MSG_FATAL("pMe->m_msSend.m_szNum........111111111",0,0,0);
        }        
        */
        
        WSTRCPY(TempToStr,L"+00");
		WSTRCPY(TempTo,pItem->m_szTo+1);
		WSTRCAT(TempToStr,TempTo);
		DBGPRINTF("TempToStr=%S", TempTo);
		DBGPRINTF("TempToStr=%S", TempToStr);
		DBGPRINTF("pItem->m_szTo=%S", pItem->m_szTo);
		MEMSET(pItem->m_szTo,0,(sizeof(pItem->m_szTo))+1);
		WSTRCPY(pItem->m_szTo,TempToStr);
		DBGPRINTF("pItem->m_szTo=%S", pItem->m_szTo);
		MSG_FATAL("pMe->m_msSend.m_szNum........111111111",0,0,0);
          
    }
#else
	if((STRISTR (charsvc_p_name,"tata"))&&(STRISTR(temp,"+91"))) 
	{
		WSTRCPY(TempToStr,L"0091");
		WSTRCPY(TempTo,pItem->m_szTo+3);
		WSTRCAT(TempToStr,TempTo);
		DBGPRINTF("TempToStr=%S", TempTo);
		DBGPRINTF("TempToStr=%S", TempToStr);
		DBGPRINTF("pItem->m_szTo=%S", pItem->m_szTo);
		MEMSET(pItem->m_szTo,0,(sizeof(pItem->m_szTo))+1);
		WSTRCPY(pItem->m_szTo,TempToStr);
		DBGPRINTF("pItem->m_szTo=%S", pItem->m_szTo);
		MSG_FATAL("pMe->m_msSend.m_szNum........111111111",0,0,0);
	}   
#endif    
	
    (void)WSTRCPY(pMe->m_msSend.m_szNum, pItem->m_szTo);
    
    nSize = nItems*sizeof(wms_client_message_s_type *);
    pMe->m_pCurSendCltMsg = (wms_client_message_s_type **)MALLOC(nSize);
    if (NULL == pMe->m_pCurSendCltMsg)
    {
        goto BuildList_ERR;
    }
    
    pMe->m_idxUserdata = 0;
    for (i=0; i<nItems; i++)
    {
        pClientMsg = WmsApp_GetClientMsgMO(pMe, TRUE);
        if (NULL == pClientMsg)
        {
            goto BuildList_ERR;
        }
        
        pMe->m_pCurSendCltMsg[i] = pClientMsg;
    }
    
    pMe->m_idxCurSend = 0;
    pMe->m_nSendItems = nItems;
    return;
                    
BuildList_ERR:
    WmsApp_FreeSendClentMsgList(pMe);
}

/*==============================================================================
����:
    WmsApp_GetMsgICONID

˵��:
    ��ȡ����ʹ�õ�ͼ�ꡣ

����:
    pNode [in]: ָ�� wms_cache_info_node �ṹ��ָ�롣

����ֵ:
    uint16

��ע:
       
==============================================================================*/
static uint16 WmsApp_GetMsgICONID(wms_cache_info_node * pNode)
{
    uint16 wID = 0;
    
    if (NULL == pNode)
    {
        return wID;
    }
    
    if (pNode->mem_store == WMS_MEMORY_STORE_RUIM)
    {
        if (pNode->msg_type == WMS_BD_TYPE_DELIVERY_ACK)
        {
            if (pNode->msg_tag == WMS_TAG_MT_READ)
            {
                wID = IDB_READ_UIM_R;
            }
            else
            {
                wID = IDB_UREAD_UIM_R;
            }
            
            return wID;
        }
        
        switch (pNode->msg_tag)
        {
            case WMS_TAG_MT_READ:
                if (pNode->priority == WMS_PRIORITY_URGENT)
                {
                    wID = IDB_READ_UIM_URGENT;
                }
                else if (pNode->priority == WMS_PRIORITY_EMERGENCY)
                {
                    wID = IDB_READ_UIM_EMERGENCY;
                }
                else
                {
                    wID = IDB_READ_UIM_NORMAL;
                }
                break;
                
            case WMS_TAG_MT_NOT_READ:
                if (pNode->priority == WMS_PRIORITY_URGENT)
                {
                    wID = IDB_UREAD_UIM_URGENT;
                }
                else if (pNode->priority == WMS_PRIORITY_EMERGENCY)
                {
                    wID = IDB_UREAD_UIM_EMERGENCY;
                }
                else
                {
                    wID = IDB_UREAD_UIM_NORMAL;
                }
                break;
                
            case WMS_TAG_MO_SENT:
                if (pNode->priority == WMS_PRIORITY_URGENT)
                {
                    wID = IDB_SENT_URGENT;
                }
                else if (pNode->priority == WMS_PRIORITY_EMERGENCY)
                {
                    wID = IDB_SENT_EMERGENCY;
                }
                else
                {
                    wID = IDB_SENT_NORMAL;
                }
                break;
                
            case WMS_TAG_RESERVE:
            case WMS_TAG_MO_NOT_SENT:
                if (pNode->priority == WMS_PRIORITY_URGENT)
                {
                    wID = IDB_UNSENT_URGENT;
                }
                else if (pNode->priority == WMS_PRIORITY_EMERGENCY)
                {
                    wID = IDB_UNSENT_EMERGENCY;
                }
                else
                {
                    wID = IDB_UNSENT_NORMAL;
                }
                break;
                
            case WMS_TAG_MO_TEMPLATE:
                wID = IDB_TEMPLATE;
                break;
                
            case WMS_TAG_MO_DRAFT:
                wID = IDB_DRAFT;
                break;
                
            default:
                break; 
            
        }
    }
    else
    {
        if (pNode->msg_type == WMS_BD_TYPE_DELIVERY_ACK)
        {
            if (pNode->msg_tag == WMS_TAG_MT_READ)
            {
                wID = IDB_READ_PH_R;
            }
            else
            {
                wID = IDB_UREAD_PH_R;
            }
            
            return wID;
        }
        
        switch (pNode->msg_tag)
        {
            case WMS_TAG_MT_READ:
                if (pNode->priority == WMS_PRIORITY_URGENT)
                {
                    wID = IDB_READ_PH_URGENT;
                }
                else if (pNode->priority == WMS_PRIORITY_EMERGENCY)
                {
                    wID = IDB_READ_PH_EMERGENCY;
                }
                else
                {
                    wID = IDB_READ_PH_NORMAL;
                }
                break;
                
            case WMS_TAG_MT_NOT_READ:
                if (pNode->priority == WMS_PRIORITY_URGENT)
                {
                    wID = IDB_UREAD_PH_URGENT;
                }
                else if (pNode->priority == WMS_PRIORITY_EMERGENCY)
                {
                    wID = IDB_UREAD_PH_EMERGENCY;
                }
                else
                {
                    wID = IDB_UREAD_PH_NORMAL;
                }
                break;
                
            case WMS_TAG_RESERVE:
            case WMS_TAG_MO_SENT:
                if (pNode->priority == WMS_PRIORITY_URGENT)
                {
                    wID = IDB_SENT_URGENT;
                }
                else if (pNode->priority == WMS_PRIORITY_EMERGENCY)
                {
                    wID = IDB_SENT_EMERGENCY;
                }
                else
                {
                    wID = IDB_SENT_NORMAL;
                }
                break;
                
            case WMS_TAG_RSVFAILED:
            case WMS_TAG_MO_NOT_SENT:
                if (pNode->priority == WMS_PRIORITY_URGENT)
                {
                    wID = IDB_UNSENT_URGENT;
                }
                else if (pNode->priority == WMS_PRIORITY_EMERGENCY)
                {
                    wID = IDB_UNSENT_EMERGENCY;
                }
                else
                {
                    wID = IDB_UNSENT_NORMAL;
                }
                break;
                
            case WMS_TAG_MO_TEMPLATE:
                wID = IDB_TEMPLATE;
                break;
                
            case WMS_TAG_MO_DRAFT:
                wID = IDB_DRAFT;
                break;
                
            default:
                break; 
            
        }
    }
    
    return wID;
}

/*==============================================================================
������
    WmsApp_CurmessageIsFullSendout

˵����
    ���������жϵ�ǰ���͵���Ϣ�Ƿ�����ϡ�

������
    pMe [in]: ָ�� WMS Applet����ṹ��ָ�롣�ýṹ����С������ض���Ϣ��

����ֵ��
    TRUE - ���
    FALSE - ��δ���

��ע��
        
==============================================================================*/
boolean WmsApp_CurmessageIsFullSendout(WmsApp * pMe)
{
    boolean bRet = TRUE;
    MSG_FATAL("WmsApp_CurmessageIsFullSendout Start m_eCreateWMSType=%d",pMe->m_eCreateWMSType,0,0);
    if (pMe->m_eCreateWMSType == SEND_MSG_RESEND)
    {
        pMe->m_idxCur++;
        // ȷ����ǰ������Ϣ�ڵ��Ƿ������
        for (; pMe->m_idxCur<LONGSMS_MAX_PACKAGES; pMe->m_idxCur++)
        {
            if (pMe->m_CurMsgNodes[pMe->m_idxCur] != NULL)
            {
                break;
            }
        }
        if (pMe->m_idxCur<LONGSMS_MAX_PACKAGES)
        {
            bRet = FALSE;
        }
    }
    else if (pMe->m_eCreateWMSType == SEND_MSG_RESERVE)
    {
        if (pMe->m_SendStatus == WMS_RPT_OK)
        {
            // ������Ϣ��������
            (void)IWMS_MsgWrite(pMe->m_pwms, 
                                pMe->m_clientId, 
                                &pMe->m_callback,
                                (void*)pMe,
                                WMS_WRITE_MODE_INSERT,
                                pMe->m_pCurSendCltMsg[pMe->m_idxCurSend]);
            
            // ��ԤԼ������ɾ���˽ڵ�
            if (pMe->m_CurMsgNodes[pMe->m_idxCur] != NULL)
            {
                // ����ɾ����Ϣ����
                (void)IWMS_MsgDelete(pMe->m_pwms,
                                     pMe->m_clientId,
                                     &pMe->m_callback,
                                     (void *)pMe,
                                     (pMe->m_CurMsgNodes[pMe->m_idxCur])->mem_store,
                                     (pMe->m_CurMsgNodes[pMe->m_idxCur])->index);
            }
        }
        else
        {// ����ʧ����ԤԼʧ�ܴ���
            (void)IWMS_MsgModifyTag(pMe->m_pwms,
                                    pMe->m_clientId,
                                    &pMe->m_callback,
                                    (void *)pMe,
                                    (pMe->m_CurMsgNodes[pMe->m_idxCur])->mem_store,
                                    (pMe->m_CurMsgNodes[pMe->m_idxCur])->index,
                                    WMS_TAG_RSVFAILED);
            {
                uint16 nMsgs,nNews;
                int i;
                int nRet=EFAILED;
                wms_cache_info_node  *pFailnode = NULL;
                wms_cache_info_node  *ptepnode  = NULL;

                wms_cacheinfolist_getcounts(WMS_MB_RESERVE, &nNews, NULL, &nMsgs);
                
                if(nMsgs - nNews > MAX_RSVFAILD)
                {
                    wms_cacheinfolist_enumbegin(WMS_MB_RSVFAILED);
                    pFailnode = wms_cacheinfolist_enumnext(WMS_MB_RSVFAILED);
					#ifdef FEATURE_SMS_UDH
                    for (i=0; i<LONGSMS_MAX_PACKAGES; i++)
                    {
                        ptepnode = pFailnode->pItems[i];
                        if (ptepnode != NULL)
                        {
                            
                            // ����ɾ����Ϣ����
                            nRet = ENOMEMORY;
                            do
                            {
                                nRet = IWMS_MsgDelete(pMe->m_pwms,
                                                   pMe->m_clientId,
                                                   &pMe->m_callback,
                                                   (void *)pMe,
                                                   pFailnode->mem_store,
                                                   pFailnode->index);
                            } while(nRet != SUCCESS);
                            ptepnode = NULL;
                        }     
                    }
					#endif
                }

            }
        }
        pMe->m_idxCur++;
		#ifdef FEATURE_SMS_UDH
        // ȷ����ǰ������Ϣ�ڵ��Ƿ������
        for (; pMe->m_idxCur<LONGSMS_MAX_PACKAGES; pMe->m_idxCur++)
        {
            if (pMe->m_CurMsgNodes[pMe->m_idxCur] != NULL)
            {
                break;
            }
        }
        if (pMe->m_idxCur<LONGSMS_MAX_PACKAGES)
        {
            bRet = FALSE;
        }
		#endif
    }
	else if(pMe->m_eCreateWMSType == SEND_MSG_NEW)
	{
		pMe->m_idxCurSend++;
        if (pMe->m_idxCurSend<pMe->m_nSendItems)
        {
            // ���������ʾ
            bRet = FALSE;
        }
	}
    else
    {
        pMe->m_idxCurSend++;
        if (pMe->m_idxCurSend<pMe->m_nSendItems)
        {
            // ���������ʾ
            bRet = FALSE;
        }
    }
    MSG_FATAL("WmsApp_CurmessageIsFullSendout End bRet=%d", bRet,0,0);
    return bRet;
}

/*==============================================================================
������
    WmsApp_ReservedMsgStatusUpdate

˵����
    �����������ԤԼ�����б�, ���ڹ��ڵĶ������޸����ǡ�

������
    pMe [in]: ָ�� WMS Applet����ṹ��ָ�롣�ýṹ����С������ض���Ϣ��

����ֵ��
    none

��ע��
        
==============================================================================*/
static void WmsApp_ReservedMsgStatusUpdate(WmsApp * pMe)
{
#ifdef FEATURE_SMS_UDH
    int i;
    wms_cache_info_node  *ptepnode = NULL;
#endif    
    uint32  dwSecs;
    wms_cache_info_node  *pnode = NULL;

	/*
	byte	btTimeStamp = 0;
	
	(void) ICONFIG_GetItem(pMe->m_pConfig,
                           CFGI_SMS_TIMESTAMP,
                           &btTimeStamp,
                           sizeof(btTimeStamp));

	if (btTimeStamp == OEMNV_SMS_TIMESTAMP_ADJUST)
	{
		dwSecs = GETUTCSECONDS();
		MSG_FATAL("***zzg GETUTCSECONDS 2 dwSecs=%d***", dwSecs, 0, 0);
	}
	else
	{
		dwSecs = GETTIMESECONDS();
		MSG_FATAL("***zzg GETTIMESECONDS 2 dwSecs=%d***", dwSecs, 0, 0);
	}    
	*/

	dwSecs = GETTIMESECONDS();
    
    wms_cacheinfolist_enumbegin(WMS_MB_RESERVE);
    pnode = wms_cacheinfolist_enumnext(WMS_MB_RESERVE);
    while ((NULL != pnode) && (pnode->dwTime < dwSecs))
    {
#ifdef FEATURE_SMS_UDH
        if (pnode->pItems != NULL)
        {
            for (i=0; i<LONGSMS_MAX_PACKAGES; i++)
            {
                ptepnode = pnode->pItems[i];
                
                if (NULL != ptepnode)
                {
                    (void)IWMS_MsgModifyTag(pMe->m_pwms,
                                            pMe->m_clientId,
                                            &pMe->m_callback,
                                            (void *)pMe,
                                            ptepnode->mem_store,
                                            ptepnode->index,
                                            WMS_TAG_RSVFAILED);
                }
            }
        }
        else
#endif            
        {
            (void)IWMS_MsgModifyTag(pMe->m_pwms,
                                    pMe->m_clientId,
                                    &pMe->m_callback,
                                    (void *)pMe,
                                    pnode->mem_store,
                                    pnode->index,
                                    WMS_TAG_RSVFAILED);
        }

        {
            uint16 nMsgs,nNews;
            int i;
            int nRet=EFAILED;
            wms_cache_info_node  *pFailnode = NULL;

            wms_cacheinfolist_getcounts(WMS_MB_RESERVE, &nNews, NULL, &nMsgs);
            
            if(nMsgs - nNews > MAX_RSVFAILD)
            {
            #ifdef  FEATURE_SMS_UDH
                wms_cacheinfolist_enumbegin(WMS_MB_RSVFAILED);
                pFailnode = wms_cacheinfolist_enumnext(WMS_MB_RSVFAILED);
                for (i=0; i<LONGSMS_MAX_PACKAGES; i++)
                {
                    ptepnode = pFailnode->pItems[i];
                    if (ptepnode != NULL)
                    {
                        
                        // ����ɾ����Ϣ����
                        nRet = ENOMEMORY;
                        do
                        {
                            nRet = IWMS_MsgDelete(pMe->m_pwms,
                                               pMe->m_clientId,
                                               &pMe->m_callback,
                                               (void *)pMe,
                                               pFailnode->mem_store,
                                               pFailnode->index);
                        } while(nRet != SUCCESS);
                        ptepnode = NULL;
                    }     
                }
				#endif
            }

        }

        pnode = wms_cacheinfolist_enumnext(WMS_MB_RESERVE);
    }
}

/*==============================================================================
������
    WmsApp_ReservedMsgTimer

˵����
    ԤԼ���Ŷ�ʱ���ص�������

������
    pUser [in]: ָ�� WMS Applet����ṹ��ָ�롣�ýṹ����С������ض���Ϣ��

����ֵ��
    none

��ע��
        
==============================================================================*/
static void WmsApp_ReservedMsgTimer(void * pUser)
{
    WmsApp * pMe = (WmsApp *)pUser;
    
    if (NULL == pMe)
    {
        return;
    }
    
    (void) ISHELL_PostEventEx(pMe->m_pShell, 
                            EVTFLG_ASYNC,
                            AEECLSID_WMSAPP,
                            EVT_RESERVEDMSGALERT,
                            0, 
                            0);
}

/*==============================================================================
������
    WmsApp_ReservedMsgSetTimer

˵����
    �����������ԤԼ�����б�, ���ڹ��ڵĶ������޸�����, ������ЧԤԼ���ţ�����
    ԤԼ���Ŷ�ʱ����

������
    pMe [in]: ָ�� WMS Applet ����ṹ��ָ�롣�ýṹ����С������ض���Ϣ��

����ֵ��
    none

��ע��
        
==============================================================================*/
void WmsApp_ReservedMsgSetTimer(WmsApp * pMe)
{
    uint32  dwSecs;
	byte	btTimeStamp = 0;
    wms_cache_info_node  *pnode = NULL;
    
    AEE_CancelTimer(WmsApp_ReservedMsgTimer, pMe);
	
	/*
	(void) ICONFIG_GetItem(pMe->m_pConfig,
                           CFGI_SMS_TIMESTAMP,
                           &btTimeStamp,
                           sizeof(btTimeStamp));

	if (btTimeStamp == OEMNV_SMS_TIMESTAMP_ADJUST)
	{
		dwSecs = GETUTCSECONDS();
		MSG_FATAL("***zzg GETUTCSECONDS 3 dwSecs=%d***", dwSecs, 0, 0);
	}
	else
	{
		dwSecs = GETTIMESECONDS();
		MSG_FATAL("***zzg GETTIMESECONDS 3 dwSecs=%d***", dwSecs, 0, 0);
	}    
	*/

	dwSecs = GETTIMESECONDS();
    
    // ���ԤԼ�����б�, ���ڹ��ڵĶ������޸�����
    WmsApp_ReservedMsgStatusUpdate(pMe);
    
    wms_cacheinfolist_enumbegin(WMS_MB_RESERVE);
    pnode = wms_cacheinfolist_enumnext(WMS_MB_RESERVE);
    if (NULL != pnode)
    {
        AEE_SetSysTimer((pnode->dwTime - dwSecs)*1000, WmsApp_ReservedMsgTimer, pMe);
    }
}

/*==============================================================================
������
    WmsApp_IsNeedContinueSendTask

˵����
    ���������ж��Ƿ���Ҫ����δ�����ŷ�������

������
    pMe [in]: ָ�� WMS Applet����ṹ��ָ�롣�ýṹ����С������ض���Ϣ��

����ֵ��
    TRUE - ��δ�����ŷ������������
    FALSE - ��δ�����ŷ�������

��ע��
    �Ƿ���δ�����ŷ��������ж���������Ϊ:
    1�����ñ�����ʱ��ǰ״̬���账�� WMSST_SENDING ;
    2�����͵��ض�����Ķ������ݰ��Ƿ������;
    3��Ⱥ���Ƿ������;
        
==============================================================================*/
boolean WmsApp_IsNeedContinueSendTask(WmsApp * pMe)
{
    boolean bRet = FALSE;
    int i, nAddrs;
    
    if ((NULL == pMe) || (pMe->m_currState != WMSST_SENDING))
    {
        return bRet;
    }
    
    nAddrs = IVector_Size(pMe->m_pSendList);
    if (nAddrs < 1)
    {
        return bRet;
    }
    
    if (pMe->m_eCreateWMSType == SEND_MSG_RESERVE)
    {
        return bRet;
    }
    
    if (pMe->m_eCreateWMSType == SEND_MSG_RESEND)
    {// �ط�
        i = pMe->m_idxCur+1;
        
        // ȷ����ǰ������Ϣ�ڵ��Ƿ������
        for (; (i<LONGSMS_MAX_PACKAGES) && (pMe->m_CurMsgNodes[i] != NULL); i++)
        {
            ;
        }
        if (i<LONGSMS_MAX_PACKAGES)
        {
            bRet = TRUE;
            pMe->m_ContinueSendType = CONTINUE_CURPERSON;
        }
    }
    else
    {
        if ((pMe->m_idxCurSend+1)<pMe->m_nSendItems)
        {
            bRet = TRUE;
            pMe->m_ContinueSendType = CONTINUE_CURPERSON;
        }
        else if (nAddrs > 1)
        {
            bRet = TRUE;
            pMe->m_ContinueSendType = CONTINUE_NEXTPERSON;
        }
    }
    
    if (!bRet)
    {
        pMe->m_ContinueSendType = NONE_CONTINUE;
    }
    
    return bRet;
}

/*==============================================================================
����:
    CWmsApp_GetLeftSendListNodeNum

˵��:
    �ṩ���ⲿApplet������ȡsendList��໹����Ӻ���ĸ���

����:
    p [in]:ָ��IWmsApp�ӿ�t�����ָ�롣
    leftNum [out]: ʣ��ĵ�ַ����

����ֵ:
    SUCCESS:Applet�����ɹ���
    EFAILED:Applet����ʧ�ܡ�

��ע:
        
==============================================================================*/
static int CWmsApp_GetLeftSendListNodeNum(IWmsApp *p, int *leftNum)
{
    WmsApp *pMe = (WmsApp*)p;

    if (ISHELL_ActiveApplet(pMe->m_pShell) == AEECLSID_WMSAPP)
    {
        // WMS applet is already running
        return EFAILED;
    }
    
    *leftNum = MAXNUM_MULTISEND - IVector_Size(pMe->m_pSendList);
    
    return SUCCESS;
}

/*==============================================================================
����:
    CWmsApp_GetLeftSendListNodeNum

˵��:
    �ṩ���ⲿApplet������ȡsendList��໹����Ӻ���ĸ���

����:
    p [in]:ָ��IWmsApp�ӿ�t�����ָ�롣
    leftNum [out]: ʣ��ĵ�ַ����

����ֵ:
    SUCCESS:Applet�����ɹ���
    EFAILED:Applet����ʧ�ܡ�

��ע:
        
==============================================================================*/
static int CWmsApp_DeleteAllNvCdmaSms(IWmsApp *p)
{
    WmsApp *pMe = (WmsApp*)p;
    int    nRet;

    if (ISHELL_ActiveApplet(pMe->m_pShell) == AEECLSID_WMSAPP)
    {
        // WMS applet is already running
        return EFAILED;
    }

    //  ɾ���ռ�������ֻ��ϵ���Ϣ
    nRet = IWMS_MsgDeleteBox(pMe->m_pwms,
                pMe->m_clientId, 
                &pMe->m_callback,
                (void*)pMe,
                WMS_INBOX_DEL_PHONE);    

    // ɾ�����������Ϣ
    if(nRet == SUCCESS)
    {
        nRet = IWMS_MsgDeleteBox(pMe->m_pwms,
                    pMe->m_clientId, 
                    &pMe->m_callback,
                    (void*)pMe,
                    WMS_OUTBOX_DEL_ALL);        
    }

    // ɾ���ݸ������Ϣ
    if(nRet == SUCCESS)
    {
        nRet = IWMS_MsgDeleteBox(pMe->m_pwms,
                    pMe->m_clientId, 
                    &pMe->m_callback,
                    (void*)pMe,
                    WMS_DRAFT_DEL_ALL);        
    }

    // ɾ��ԤԼ��Ϣ
    #ifdef FEATURE_RESERVEDMSG
    if(nRet == SUCCESS)
    {
        nRet = IWMS_MsgDeleteBox(pMe->m_pwms,
                    pMe->m_clientId, 
                    &pMe->m_callback,
                    (void*)pMe,
                    WMS_RSVANDRSVFAILED_DEL_ALL);        
    }
	#endif
    
    return nRet;
}

#ifdef FEATURE_USES_MMS
/*==============================================================================
����:
    WmsApp_ProcessMMSStatus

˵��:
    ������������ wms �ı�����Ϣ��

����:
    pMe [in]: ָ�� WMS Applet����ṹ��ָ�롣�ýṹ����С������ض���Ϣ��
    pRptInfo [in]: wms_submit_report_info_s_type �ṹָ�롣

����ֵ:
    none

��ע:

==============================================================================*/
void WmsApp_ProcessMMSStatus(WmsApp *pMe)
{
    MSG_FATAL("WmsApp_ProcessMMSStatus Start",0,0,0);
    //pMe->m_SendStatus = MMS_GetSocketReadStatus();
    
    ERR("m_SendStatus = %d", pMe->m_SendStatus, 0, 0);    
    
    if (ISHELL_ActiveApplet(pMe->m_pShell) == AEECLSID_WMSAPP)
    {
        if((pMe->m_wActiveDlgID == IDD_SENDING) 
            || (pMe->m_wActiveDlgID == IDD_GETTING))
        {
            pMe->m_isMMS = TRUE;
            (void)ISHELL_PostEventEx(pMe->m_pShell,
                                 EVTFLG_ASYNC, 
                                 AEECLSID_WMSAPP, 
                                 EVT_UPDATE,
                                 0, 
                                 0);
        }
        else
        {
            MOVE_TO_STATE(pMe->m_currState)
        }
    }
    MSG_FATAL("WmsApp_ProcessMMSStatus End",0,0,0);
}
extern uint8* WMS_MMS_BUFFERGet();

void WmsApp_debug_CheckChariFromRawDaTa(WmsApp *pMe)
{
    uint8* pRawData = WMS_MMS_BUFFERGet();
    IFileMgr* pFileMgr = NULL;
    IFile* pFile = NULL;
    uint32 nSize = 0;
    int nRet = SUCCESS;
    uint8* body = NULL;
    wms_raw_ts_data_s_type  raw_data;
    uint32 header_len = 0;
    uint32 body_len = 0;
    
    raw_data.format = WMS_FORMAT_CDMA;
    raw_data.tpdu_type = WMS_TPDU_DELIVER;
    
    if(ISHELL_CreateInstance(AEE_GetShell(),AEECLSID_FILEMGR,(void**)&pFileMgr))
    {
        return;
    }

    pFile =  IFILEMGR_OpenFile(pFileMgr,"fs:/hsmm/pictures/rawdata.txt",_OFM_READ);
    if(pFile)
    {
        raw_data.len = IFILE_Read(pFile,raw_data.data,WMS_MAX_LEN);

        RELEASEIF(pFile);
            
        nRet = IWMS_TsDecode(pMe->m_pwms, 
	                             &raw_data, 
	                             &pMe->m_CltTsdata);
	                             
	    pFile = IFILEMGR_OpenFile(pFileMgr,"fs:/hsmm/pictures/rawdataDec.txt",_OFM_CREATE);
	    if(pFile)
	    {
	        nSize = IFILE_Write(pFile,pMe->m_CltTsdata.u.cdma.user_data.data,pMe->m_CltTsdata.u.cdma.user_data.data_len);
	    }
	    
        RELEASEIF(pFile);

        nSize = IWMS_MMsDecodeNotifyBody(pMe->m_pwms,&pMe->m_CltTsdata,pRawData);

        pFile = IFILEMGR_OpenFile(pFileMgr,"fs:/hsmm/pictures/rawdataChari.txt",_OFM_CREATE);
	    if(pFile && nSize > 0)
	    {
	        nSize = IFILE_Write(pFile,pRawData,nSize);
	    }

        RELEASEIF(pFile);

        if ( pRawData[2] == 0 )
		{
			//WDP body							
			uint8* data = NULL;
			
			#define HEADER_INFO 3
			
			data = &pRawData[7];
			header_len = data[HEADER_INFO-1];
			if(header_len == 0x1f)
			{
			    header_len = data[HEADER_INFO];
			}
			else if(header_len > 0x1f)
			{
			    header_len = 0;
			    while(data[HEADER_INFO - 1 + header_len] != 0)	
			    {
			        header_len ++;
			    }
			}
			
			if( nSize <= HEADER_INFO + header_len ) 
			{
				goto Exit;
			}
			else
			{
				uint8 *header = NULL;							
				uint8 *ptrCur = NULL;
				header = (uint8 *)MALLOC(header_len+1);

				if( NULL == header)
				{
					MSG_FATAL("SMSPUSH_ALLOC_ERROR1",0,0,0);
					goto Exit;
				}

				MEMCPY( header, &data[HEADER_INFO], header_len );
				header[header_len] = '\0';

				if( !STRSTR((char*)header, "application/vnd.wap.mms-message"))
				{
					FREEIF( header );
					goto Exit;
				}
				
				
				// yy add header judgement
				body = data + HEADER_INFO + header_len;
				ptrCur = (uint8*)STRCHR((char*)body,0xb4);
				if(ptrCur == NULL || ptrCur[1] != 0x87)
				{
					FREEIF( header );
					goto Exit;
				}

				ptrCur = (uint8*)STRCHR((char*)body,0xaf);
				if(ptrCur == NULL || ptrCur[1] != 0x84)
				{
					FREEIF( header );
					goto Exit;
				}
				
				header_len += 4;

				body_len = nSize - HEADER_INFO - header_len;
				FREEIF( header );
			}

			body = (uint8*)sys_malloc(body_len);
			if( NULL == body )
			{
				MSG_FATAL("SMSPUSH_ALLOC_ERROR2",0,0,0);
				goto Exit;
			}

			MEMCPY( (void*)body, (void*)(data + HEADER_INFO + header_len), body_len);

			//now, body need pass to function WMS_MMS_PDU_Decode to decode the URL.
			MSG_FATAL(" MMS_WSP_DEC_DATA decData",0,0,0);
           
			ISHELL_PostEventEx(pMe->m_pShell,
			    EVTFLG_ASYNC,
			    AEECLSID_WMSAPP,
			    EVT_MMS_PDUDECODE,
			    body_len,
			    (int32)body);
			    
		}
    }
    
Exit:    
    RELEASEIF(pFile);
    RELEASEIF(pFileMgr);
}
void WmsApp_UpdateMenuList_MMS(WmsApp *pMe, IMenuCtl *pMenu)
{
    int i, j, nItems, index;
    uint16  wItemCount;
    uint16  wTotalPages;
    AECHAR  wszFmt[10]={0};
    AECHAR  wszName[MAX_TITLE_LEN];
    AECHAR  wszTitle[MAX_TITLE_LEN + 10];
    AECHAR  wstrNum[MAX_PH_DIGITS+1];
    CtlAddItem  mai;
    uint16      wSelectItemID=0;
    boolean     bFindCurxuhao = FALSE;
    MMSData		mmsDataInfoList[MAX_MMS_STORED];
    MMSData		mmsDataInfoListTemp[MAX_MMS_STORED];
    int nboxType = 0;
    int nCountType = 0;
    MSG_FATAL("WmsApp_UpdateMenuList_MMS Start",0,0,0);
    if ((NULL == pMe) || (NULL == pMenu))
    {
        return;
    }
    switch(pMe->m_eMBoxType)
    {
        case WMS_MB_OUTBOX_MMS:
        {
            nCountType = CFGI_MMS_OUTCOUNT;
            nboxType = CFGI_MMSOUTDATA_INFO;
        }
        break;

        case WMS_MB_DRAFTBOX_MMS:
        {
            nCountType = CFGI_MMS_DRAFTCOUNT;
            nboxType = CFGI_MMSDRAFTDATA_INFO;
        }
        break;
        
        default:
        case WMS_MB_INBOX_MMS:
        {
            nCountType = CFGI_MMS_INCOUNT;
            nboxType = CFGI_MMSINDATA_INFO;
        }
        break;
    }    
    (void) ICONFIG_GetItem(pMe->m_pConfig,nCountType,&g_mmsDataInfoMax,sizeof(g_mmsDataInfoMax));
    // ȡ��������
    wItemCount = g_mmsDataInfoMax;
    MSG_FATAL("WmsApp_UpdateMenuList_MMS wItemCount=%d",wItemCount,0,0);
    if (wItemCount == 0)
    {
        return;
    }
    
    // ������Ҫ����ҳ��
    wTotalPages = (wItemCount / (MAXITEMS_ONEPAGE));
    if ((wItemCount % MAXITEMS_ONEPAGE) != 0)
    {
        wTotalPages++;
    }
    if (wTotalPages < 2)
    {
        nItems = wItemCount;
    }
    else
    {
        nItems = MAXITEMS_ONEPAGE;
    }
    MSG_FATAL("WmsApp_UpdateMenuList_MMS nItems=%d",nItems,0,0);
    // ��Ϣ����������һҳ��ʾ�Ĳ˵�����ʱ��������������Ҫ������˵��б��ģʽ
    if (((pMe->m_eMakeListMode == MAKEMSGLIST_NEXTONE) ||
         (pMe->m_eMakeListMode == MAKEMSGLIST_BACKONE) ||
         (pMe->m_eMakeListMode == MAKEMSGLIST_PREPAGE) ||
         (pMe->m_eMakeListMode == MAKEMSGLIST_NEXTPAGE)) && 
        (wTotalPages < 2))
    {
        int nCount;
        uint16 wItemID;
        
        nCount = IMENUCTL_GetItemCount(pMenu);
        MSG_FATAL("WmsApp_UpdateMenuList_MMS nCount=%d",nCount,0,0);
        if (nCount > 1)
        {
            if (pMe->m_eMakeListMode == MAKEMSGLIST_BACKONE)
                {
                wItemID = IMENUCTL_GetItemID(pMenu, nCount-1);
            }
            else
            {
                wItemID = IMENUCTL_GetItemID(pMenu, 0);
            }
            
            // ����ǰѡ������Ϊĩβ�������ƶ��˵����������
            IMENUCTL_SetSel(pMenu, wItemID);
            
            (void)IMENUCTL_Redraw(pMenu);
        }
        return;
    }

	// ȷ�������˵��б�ʱ����Ӧ��Ϣ�б�����λ��
	MSG_FATAL("m_eMakeListMode=%d, m_wCurPageStarxuhao=%d,m_wSelItemxuhao=%d",
	           pMe->m_eMakeListMode,pMe->m_wCurPageStarxuhao,pMe->m_wSelItemxuhao);
    switch (pMe->m_eMakeListMode)
    {
        case MAKEMSGLIST_INIT:
            pMe->m_wCurPageStarxuhao = 1;
            pMe->m_wSelItemxuhao = 1;
            break;
            
        case MAKEMSGLIST_NEXTONE:
            if (pMe->m_wCurPageStarxuhao+MAXITEMS_ONEPAGE>wItemCount)
            {
                pMe->m_wCurPageStarxuhao = 1;
                pMe->m_wSelItemxuhao = 1;
            }
            else
            {
                pMe->m_wSelItemxuhao = pMe->m_wCurPageStarxuhao+MAXITEMS_ONEPAGE;
                pMe->m_wCurPageStarxuhao++;
            }
            
            break;
            
        case MAKEMSGLIST_BACKONE:
            if ((pMe->m_wCurPageStarxuhao>1) && (pMe->m_wSelItemxuhao > 1))
            {
                pMe->m_wCurPageStarxuhao--;
                pMe->m_wSelItemxuhao = pMe->m_wCurPageStarxuhao;
            }
            else
            {// �������һҳ��ѡ�����һ��
                pMe->m_wCurPageStarxuhao = wItemCount+1-MAXITEMS_ONEPAGE;
                pMe->m_wSelItemxuhao = wItemCount;
            }
            
            break;
            
        case MAKEMSGLIST_PREPAGE:
            if (pMe->m_wCurPageStarxuhao==1)
            {// �������һҳ��ѡ�����һ��
                pMe->m_wCurPageStarxuhao = wItemCount+1-MAXITEMS_ONEPAGE;
                pMe->m_wSelItemxuhao = wItemCount;
            }
            else if (pMe->m_wCurPageStarxuhao<=MAXITEMS_ONEPAGE)
            {// ������ҳ��ѡ�е�һ��
                pMe->m_wCurPageStarxuhao = 1;
                pMe->m_wSelItemxuhao = 1;
            }
            else
            {
                pMe->m_wCurPageStarxuhao = pMe->m_wCurPageStarxuhao-MAXITEMS_ONEPAGE;
                pMe->m_wSelItemxuhao = pMe->m_wCurPageStarxuhao;
            }
            break;
            
        case MAKEMSGLIST_NEXTPAGE:
            if ((pMe->m_wCurPageStarxuhao+2*MAXITEMS_ONEPAGE-1)<=wItemCount)
            {// ������ҳ��ѡ�е�һ��
                pMe->m_wCurPageStarxuhao += MAXITEMS_ONEPAGE;
            }
            else if (pMe->m_wCurPageStarxuhao<=(wItemCount-MAXITEMS_ONEPAGE))
            {// ����ĩҳ��ѡ�е�һ��
                pMe->m_wCurPageStarxuhao = wItemCount-MAXITEMS_ONEPAGE+1;
            }
            else
            {// ������ҳ��ѡ�е�һ��
                pMe->m_wCurPageStarxuhao = 1;
            }
            
            pMe->m_wSelItemxuhao = pMe->m_wCurPageStarxuhao;
            
            break;
            
        case MAKEMSGLIST_RESUMECURPAGE:
            if (wTotalPages<2)
            {
                pMe->m_wCurPageStarxuhao = 1;
            }
            else
            {
                if (pMe->m_wCurPageStarxuhao>wItemCount)
                {
                    pMe->m_wCurPageStarxuhao = 1;
                }
                else if ((pMe->m_wCurPageStarxuhao+MAXITEMS_ONEPAGE)>wItemCount)
                {
                    pMe->m_wCurPageStarxuhao = wItemCount-MAXITEMS_ONEPAGE+1;
                }
            }
            break;
            
        default:
            return;
    }
        
    // �����˵����б�
    MEMSET(&mai, 0, sizeof(mai));
    mai.pszResImage = AEE_APPSCOMMONRES_IMAGESFILE;
    mai.pszResText = AEE_WMSAPPRES_LANGFILE;
    mai.pImage = NULL;
    mai.wFont = AEE_FONT_NORMAL;
    mai.dwData = 0;
    
    // �����������Ϣ�б�
    (void)IMENUCTL_DeleteAll(pMenu);
    (void) ICONFIG_GetItem(pMe->m_pConfig,
                       nboxType,
                       (void*)mmsDataInfoList,
                       sizeof(mmsDataInfoList));
    {
        //only for test
        MSG_FATAL("WmsApp_UpdateMenuList_MMS g_mmsDataInfoMax=%d", g_mmsDataInfoMax, 0, 0);
        DBGPRINTF("WmsApp_UpdateMenuList_MMS phoneNumber=%s, length=%d",mmsDataInfoList[g_mmsDataInfoMax-1].phoneNumber, STRLEN(mmsDataInfoList[g_mmsDataInfoMax-1].phoneNumber));
        DBGPRINTF("MMSDataFileName=%s",mmsDataInfoList[g_mmsDataInfoMax-1].MMSDataFileName);
    }
    MSG_FATAL("WmsApp_UpdateMenuList_MMS nItems=%d", nItems, 0, 0);
    (void)STRTOWSTR("%s", wszFmt, sizeof(wszFmt));	

	if (pMe->m_wSelItemxuhao > MAXITEMS_ONEPAGE)
	{
		j = pMe->m_wSelItemxuhao - MAXITEMS_ONEPAGE;
	}	
	else
	{
		j = 0;
	}

	MSG_FATAL("***zzg UpdateMenuList_MMS pMe->m_wSelItemxuhao=%d, wItemCount=%d, j=%d***", pMe->m_wSelItemxuhao, wItemCount, j);
    index = 0;
    if(pMe->m_eMBoxType == WMS_MB_INBOX_MMS)
    {
        for(i=0, index=0; i < wItemCount; ++i)
        {
            //������δ�����Ŷ����ڶ�����ǰ��
            if(!mmsDataInfoList[i].MMSDataReaded)
            {
                MEMCPY(&(mmsDataInfoListTemp[index++]), &(mmsDataInfoList[i]), sizeof(mmsDataInfoList[i]));
            }
        }
        //��δ������ʱ���Ż���������FORѭ���������ý�
        if(index > 0)
        {
            for(i=0; i < wItemCount; ++i)
            {
                //�������Ѷ����Ŷ����ڶ��������
                if(mmsDataInfoList[i].MMSDataReaded)
                {
                    MEMCPY(&(mmsDataInfoListTemp[index++]), &(mmsDataInfoList[i]), sizeof(mmsDataInfoList[i]));
                }
            }   
            ICONFIG_SetItem(pMe->m_pConfig, CFGI_MMSINDATA_INFO, (void*)&mmsDataInfoListTemp, sizeof(mmsDataInfoListTemp));    
            for(i=0; i < wItemCount; ++i)
            {
                MEMCPY(&(mmsDataInfoList[i]), &(mmsDataInfoListTemp[i]), sizeof(mmsDataInfoListTemp[i]));
                DBGPRINTF("mmsDataInfoListTemp[%d].MMSDataFileName=%s", i, mmsDataInfoListTemp[i].MMSDataFileName);
            }
        }
    }
    
    //for (i=0; i<nItems; i++)
    for (i=j; i<(j+nItems); i++)    //Add By zzg 2012_02_27
    {
        wszTitle[0] = 0;
        MEMSET(wszName, 0, sizeof(wszName));
        wstrNum[0] = 0;
        DBGPRINTF("mmsDataInfoList[i].phoneNumber=%s, length=%d",mmsDataInfoList[i].phoneNumber, STRLEN(mmsDataInfoList[i].phoneNumber));
       // �ӵ绰����ȡ����
        if (0 != STRLEN(mmsDataInfoList[i].phoneNumber))
        {
            (void)STRTOWSTR(mmsDataInfoList[i].phoneNumber, wstrNum, sizeof(wstrNum));
            
            WMSUtil_GetContactName(pMe, wstrNum, wszName, MAX_TITLE_LEN);
        }
                                    
        // ��ʽ���˵����ı�
        if (WSTRLEN(wszName) > 0)
        {
            WSPRINTF(wszTitle, sizeof(wszTitle), wszFmt, wszName);
        }
        else
        {
            if (STRLEN(mmsDataInfoList[i].phoneNumber)>0)
            {
                WSPRINTF(wszTitle, sizeof(wszTitle), wszFmt, wstrNum);
            }
            else
            {
                mai.wText = IDS_CDG2_FAIL1;
                MEMSET(wszTitle, 0, sizeof(wszTitle));
            }
        }
        if(STRCMP(mmsDataInfoList[i].phoneNumber, "Delivery")== 0)
        {
            (void)ISHELL_LoadResString(pMe->m_pShell, AEE_WMSAPPRES_LANGFILE,
                        IDS_MMSDELIVERYREPORT, wszTitle,sizeof(wszTitle));          
        }        
        if(WSTRLEN(wszTitle)>0)
        {
            mai.pText = wszTitle;
        }
		
        mai.wItemID = i+1;

        mai.dwData = mai.wItemID;

		MSG_FATAL("***zzg UpdateMenuList_MMS wItemID=%d, %d MMSDataReaded=%d", mai.wItemID, i, mmsDataInfoList[i].MMSDataReaded);
        if((!mmsDataInfoList[i].MMSDataReaded) && (pMe->m_eMBoxType == WMS_MB_INBOX_MMS))
        {
            mai.wImage = IDB_UREAD_PH_NORMAL;
        }
        else if(pMe->m_eMBoxType == WMS_MB_OUTBOX_MMS)
        {
            mai.wImage = IDB_SENT_NORMAL;
        }
        else if(pMe->m_eMBoxType == WMS_MB_DRAFTBOX_MMS)
        {
            mai.wImage = IDB_DRAFT;
        }
        else
        {
            mai.wImage = IDB_READ_PH_NORMAL;
        }
            
        // ��Ӵ�ͼ��˵���˵�
        (void)IMENUCTL_AddItemEx(pMenu, &mai);		
    }

	//wSelectItemID =  mai.wItemID;
	wSelectItemID = pMe->m_wSelItemxuhao;	//Add By zzg 2012_02_27

	MSG_FATAL("***zzg UpdateMenuList_MMS wSelectItemID=%d***", wSelectItemID, 0, 0);	
    
    if (wSelectItemID != 0)
    {
        uint16 nTitleID=0;
        int    nLen;
        
        (void)STRTOWSTR(" (%d/%d)", wszFmt, sizeof(wszFmt));
        wszTitle[0] = 0;
        
        if(pMe->m_eMBoxType == WMS_MB_OUTBOX_MMS)
        {
            nTitleID = IDS_OUTBOX_MMS;
        }
        else if(pMe->m_eMBoxType == WMS_MB_INBOX_MMS)
        {
            nTitleID = IDS_INBOX_MMS;
        }
        else if(pMe->m_eMBoxType == WMS_MB_DRAFTBOX_MMS)
        {
            nTitleID = IDS_DRAFT_MMS;
        }      
        
        if (nTitleID != 0)
        {
            (void) ISHELL_LoadResString(pMe->m_pShell,
                    AEE_WMSAPPRES_LANGFILE,
                    nTitleID,
                    wszTitle,
                    sizeof(wszTitle));
            nLen = WSTRLEN(wszTitle);
            WSPRINTF(&wszTitle[nLen], (sizeof(wszTitle) - nLen*sizeof(AECHAR)), wszFmt, wSelectItemID, wItemCount);
            DBGPRINTF("wszTitle=%S, wSelectItemID=%d, wItemCount=%d",wszTitle, wSelectItemID, wItemCount);
            //����ı�����ʾ������
			IANNUNCIATOR_SetFieldTextEx(pMe->m_pIAnn,wszTitle,FALSE);
        }
        if(wItemCount > MAXITEMS_ONEPAGE)
        {
            AEERect rc;

            IMENUCTL_GetRect(pMenu, &rc);
            rc.dx = pMe->m_rc.dx - SCROLLBAR_WIDTH;
            IMENUCTL_SetRect(pMenu, &rc);
        
            rc.x = pMe->m_rc.dx - SCROLLBAR_WIDTH;
            rc.dx = SCROLLBAR_WIDTH;
            rc.y = 0;
            rc.dy = pMe->m_rc.dy- BOTTOMBAR_HEIGHT;
        
            Appscommon_DrawScrollBar(pMe->m_pDisplay, pMe->m_wCurPageStarxuhao + MAXITEMS_ONEPAGE - 1, wItemCount, MAXITEMS_ONEPAGE, &rc);
            IDISPLAY_Update(pMe->m_pDisplay);
        }        
        IMENUCTL_SetSel(pMenu, wSelectItemID);
    }

    (void)IMENUCTL_Redraw(pMenu);
} //WmsApp_UpdateMenuList_MMS()
#endif

