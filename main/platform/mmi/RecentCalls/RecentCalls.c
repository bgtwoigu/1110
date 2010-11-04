/*==============================================================================
// �ļ���
//        recentcalls.c
//        2007-11-18 ��ϲ�����汾(Draft Version)
//        ��Ȩ����(c) 2007 Anylook
//        
// �ļ�˵����
//        
// ���ߣ�2007-11-18
// �������ڣ���ϲ��
// ��ǰ�汾��Draft
==============================================================================*/

/*==============================================================================
�޸���ʷ��¼��
ʱ       ��      �� �� ��       �޸����ݡ�λ�ü�ԭ��
-----------      ----------     -----------------------------------------------
07-11-18         ��ϲ��         ��ʼ�汾
==============================================================================*/


/*==============================================================================
                                 ���ļ��������ⲿ�ļ�
==============================================================================*/
#include "recentcalls_priv.h"

/*==============================================================================
                                 �궨��ͳ���
==============================================================================*/

/*==============================================================================
                                 ���Ͷ���
==============================================================================*/

/*==============================================================================
                                 ��������
==============================================================================*/
static void recentcalls_APPIsReadyTimer(void *pme);

/*----------------------ģ����غ�������---------------------*/

int  RecentCalls_Load(IShell   *pIShell,
    void     *ph,
    IModule **ppMod
);

int  recentcallsMod_New(int16  nSize,
    IShell   *pIShell,
    void     *ph,
    IModule  **ppMod,
    PFNMODCREATEINST pfnMC,
    PFNFREEMODDATA pfnMF
);

static int  recentcallsMod_CreateInstance(IModule *po,
    IShell *pIShell,
    AEECLSID ClsId,
    void **ppObj
);

static uint32 recentcallsMod_AddRef(IModule *po);

static uint32 recentcallsMod_Release(IModule *po);

static void recentcallsMod_FreeResources(IModule  *po,
    IHeap  *ph,
    IFileMgr *pfm
);

/*----------------------Applet��غ�������---------------------*/
static int recentcalls_New(IShell *ps,
    IModule *pIModule,
    IRecentCalls **ppObj
);

static uint32  recentcalls_AddRef(IRecentCalls *pi);

static uint32  recentcalls_Release (IRecentCalls *pi);

static boolean recentcalls_HandleEvent(IRecentCalls *pi,
    AEEEvent eCode,
    uint16  wParam,
    uint32 dwParam
);

static void    recentcalls_SendList (IRecentCalls *pi);

static void    recentcalls_CallList (IRecentCalls *pi, PFNSELECTEDRECENTCALLSCB cb, SelectRecordType type);

static void    recentcalls_MissedList (IRecentCalls *pi);

static int recentcalls_InitAppData(CRecentCalls *pMe);

static void recentcalls_FreeAppData(CRecentCalls *pMe);

static void recentcalls_RunFSM(CRecentCalls *pMe);

/*==============================================================================
                                 ȫ������
==============================================================================*/
// ��Ҫ��ʾ��
//        g_dwAEEStdLibEntryȫ�ֱ���������SDK ��
//        BREW SDK�û����ܸ�д��ֱ��ʹ�ñ�������������AEEStdLib��һ����ڵ㣬��
//        Ҫ�ƶ��������д��롣
#ifdef AEE_SIMULATOR
   uint32 g_dwAEEStdLibEntry;
   #error This applet was never intended to be used with the AEE Simulator.
#endif

/*==============================================================================
                                 ���أ���̬������
==============================================================================*/
static recentcallsMod grecentcallsMod;

static const VTBL(IModule) gModFuncs =
{
    recentcallsMod_AddRef,
    recentcallsMod_Release,
    recentcallsMod_CreateInstance,
    recentcallsMod_FreeResources
};

// ֻ����һ��recentcallsʵ����ÿ�δ���recentcalls Appletʱ��
// ����ͬһ�ṹָ���BREW�㡣
static CRecentCalls grecentcalls;

static const VTBL(IRecentCalls) grecentcallsMethods =
{
    recentcalls_AddRef,
    recentcalls_Release,
    recentcalls_HandleEvent,
    recentcalls_SendList,
    recentcalls_MissedList,
    recentcalls_CallList
};

/*----------------------ģ����غ�������---------------------*/

/*==============================================================================
����: 
       RecentCalls_Load
       
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
#ifdef AEE_SIMULATOR
__declspec(dllexport) int AEEMod_Load(IShell   *pIShell,
                                                        void     *ph,
                                                        IModule **ppMod)
#else
int RecentCalls_Load(IShell   *pIShell,
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

    return recentcallsMod_New(sizeof(recentcallsMod),
                    pIShell,
                    ph,
                    ppMod,
                    NULL,
                    NULL);
}

/*==============================================================================
����: 
       recentcallsMod_New
       
˵��: 
       ������ʼ��recentcallsģ�������Ҳ�������ü����������ֺ�����
       
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
int  recentcallsMod_New(int16  nSize,
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

    MEMSET(&grecentcallsMod,0,sizeof(recentcallsMod));

    INIT_VTBL(&grecentcallsMod,IModule,gModFuncs);
    grecentcallsMod.m_nRefs = 1;
    *ppMod = (IModule *)&grecentcallsMod;
    return AEE_SUCCESS;
}

/*==============================================================================
����: 
       recentcallsMod_AddRef
       
˵��: 
       recentcalls ģ�����ü�����1�������ص�ǰ����������
       
����: 
       po [in]��IModule�ӿڶ���ָ�롣
       
����ֵ:
       ��ǰ��recentcalls ģ���������
       
��ע:
       
==============================================================================*/
static uint32 recentcallsMod_AddRef(IModule *po)
{
    return(++((recentcallsMod *)po)->m_nRefs);
}

/*==============================================================================
����: 
       recentcallsMod_Release
       
˵��: 
       recentcalls ģ�����ü�����1�������ص�ǰ����������
       
����: 
       po [in]��IModule�ӿڶ���ָ�롣
       
����ֵ:
       ��ǰ��recentcalls ģ���������
       
��ע:
       
==============================================================================*/
static uint32 recentcallsMod_Release(IModule *po)
{
    if (((recentcallsMod *) po)->m_nRefs == 0)
    {
        return 0;
    }

    return(--((recentcallsMod *)po)->m_nRefs);
}

/*==============================================================================
����: 
       recentcallsMod_CreateInstance
       
˵��: 
        ͨ������(��ʼ��)recentcalls����������ģ��ʵ����
       
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
static int  recentcallsMod_CreateInstance(IModule *po,
    IShell *pIShell,
    AEECLSID ClsId,
    void **ppObj
)
{
    *ppObj = NULL;

    if (ClsId != AEECLSID_APP_RECENTCALL)
    {
        return EFAILED;
    }

    if (recentcalls_New(pIShell, po, (IRecentCalls**)ppObj) != SUCCESS)
    {
        return EFAILED;
    }
    return SUCCESS;
}

/*==============================================================================
����: 
       recentcallsMod_FreeResources
       
˵��: 
        �ͷ�recentcallsģ����Դ�ĺ�����
       
����: 
       po ��IModule��
       ph ��IHeap�ӿ�ָ�롣
       pfm :IFileMgr�ӿ�ָ�롣
       
����ֵ:
       ��
       
��ע:
       �����û�ȥ���á�
       
==============================================================================*/
static void recentcallsMod_FreeResources(IModule  *po,
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
       recentcalls_New
       
˵��: 
        ��ʼ��recentcalls Applet�������ڲ����ݱ�����ͬʱ�����ü�����1��
       
����: 
       ps [in]��IShell�ӿڶ���ָ�롣
       pIModule [in]��pIModule�ӿڶ���ָ�롣
       ppObj [out]:��ָ��IRecentCalls����ָ���ָ�롣
       
����ֵ:
       EFAILED��ʧ�ܡ�
       SUCCESS���ɹ���
       
��ע:
       �����û�ȥ���á�
       ����Applet�ṹ�����ʹ�ö�̬�����ڴ�ķ�ʽ�������������õ���ʹ�þ�̬
       �����ķ�ʽ������Խṹ�Ƚϴ��Appletʹ�þ�̬�����ķ�ʽ�����ڶ�̬����
       �ڴ�ķ�ʽ��Ҫע���������ռ���ͷš�
       
==============================================================================*/
static int recentcalls_New(IShell *ps,
    IModule *pIModule,
    IRecentCalls **ppObj
)
{
    int retVal = SUCCESS;

    if (0 == grecentcalls.m_nRefs)
    {
        // Must initialize the object
        MEMSET(&grecentcalls,  0, sizeof(CRecentCalls));
        INIT_VTBL(&grecentcalls, IRecentCalls, grecentcallsMethods);

        grecentcalls.m_nRefs     = 0;
        grecentcalls.m_pShell    = ps;
        grecentcalls.m_pModule   = pIModule;

        (void) ISHELL_AddRef(ps);
        (void) IMODULE_AddRef(pIModule);

        retVal = recentcalls_InitAppData(&grecentcalls);
    }

    ++grecentcalls.m_nRefs;
    *ppObj = (IRecentCalls*)&grecentcalls;
    return retVal;
}

/*==============================================================================
����: 
       recentcalls_AddRef
       
˵��: 
       recentcalls Applet ���ü�����1�������ص�ǰ����������
       
����: 
       pi [in]��IRecentCalls�ӿڶ���ָ�롣
       
����ֵ:
       ��ǰ��recentcalls Applet��������
       
��ע:
       
==============================================================================*/
static uint32  recentcalls_AddRef(IRecentCalls *pi)
{
    register CRecentCalls *pMe = (CRecentCalls*)pi;

    return (++pMe->m_nRefs);
}

/*==============================================================================
����: 
       recentcalls_Release
       
˵��: 
       �ͷŶ�recentcalls Applet�����ã����ü�����1�������ص�ǰ����������
        �����ǰ��������Ϊ�㣬�����ͷ�Applet�������Դ��
       
����: 
       pi [in]��IRecentCalls�ӿڶ���ָ�롣
       
����ֵ:
       ��ǰ��recentcalls Applet��������
       
��ע:
       
==============================================================================*/
static uint32  recentcalls_Release (IRecentCalls *pi)
{
    register CRecentCalls *pMe = (CRecentCalls*)pi;

    //ASSERT(pMe != NULL);

    if (pMe->m_nRefs == 0)
    {
        return 0;
    }

    if (--pMe->m_nRefs)
    {
        return pMe->m_nRefs;
    }

    // �ͷ�Applet�����Դ
    recentcalls_FreeAppData(pMe);

    (void) ISHELL_Release(pMe->m_pShell);
    (void) IMODULE_Release(pMe->m_pModule);

    // ע�⣺pMe�Ǿ�̬����ռ䣬��˲���Ҫ�ͷš�FREE()
    return 0;
}

/*==============================================================================
recentcalls_SendList

˵����
      �������水SEND����ʱ��,�г�������ĺ�����¼

������
       pi [in/out]��IRecentCalls�ӿڶ���ָ�롣

����ֵ��
       ��ǰ��SounMenu Applet��������

��ע��:

==============================================================================*/
static void  recentcalls_SendList (IRecentCalls *pi)
{
   register CRecentCalls *pMe = (CRecentCalls*)pi;
   char     *args;
   
   args = MALLOC(4);
   *args = STARTARG_OUTGCALL;

   pMe->m_callsCategory = AEECALLHISTORY_CALL_TYPE_TO;//CALLHISTORY_OUTGOING_CATEGORY;
   pMe->m_eStartMethod = STARTMETHOD_NORMAL;

   (void)ISHELL_StartAppletArgs(pMe->m_pShell,
                          AEECLSID_APP_RECENTCALL,
                          args);
   if(args)
   {
      FREE(args);
   }
}

/*==============================================================================
recentcalls_RecentCallList


˵����
      �������水SEND����ʱ��,�г�������ĺ�����¼

������
       pi [in/out]��IRecentCalls�ӿڶ���ָ�롣

����ֵ��
       ��ǰ��SounMenu Applet��������

��ע��:

==============================================================================*/
static void  recentcalls_CallList (IRecentCalls *pi, PFNSELECTEDRECENTCALLSCB cb, SelectRecordType type)
{
   CRecentCalls *pMe = (CRecentCalls*)pi;
   char     *args;
   char      buf[20];
   
   switch(type)
   {
       case SELECT_ONE_NUMBER:
           SPRINTF(buf, "%c%d", STARTARG_ONECALL, (uint32) cb);
           break;
           
       case SELECT_MULTI_NUMBER:
       default :
           SPRINTF(buf, "%c%d", STARTARG_ALLCALL, (uint32) cb);
           break;

   }
   
   args = STRDUP(buf);
    
   (void)ISHELL_StartAppletArgs(pMe->m_pShell,
                          AEECLSID_APP_RECENTCALL,
                          args);
   if(args)
   {
      FREE(args);
   }
}

/*==============================================================================
recentcalls_MissedList

˵����
      �ڴ������ʱ��,��SOFTKEY�����������б�

������
       pi [in/out]��IRecentCalls�ӿڶ���ָ�롣

����ֵ��
       ��ǰ��SounMenu Applet��������

��ע��:

==============================================================================*/
static void  recentcalls_MissedList (IRecentCalls *pi)
{
   register CRecentCalls *pMe = (CRecentCalls*)pi;
   char     *args;
   
   args = MALLOC(4);
   *args = STARTARG_MISSEDCALL;
   pMe->m_callsCategory = AEECALLHISTORY_CALL_TYPE_MISSED;//CALLHISTORY_MISSED_CATEGORY;
   pMe->m_eCurState = STATE_RECORD_LIST;
   pMe->record_selected = 0;
   (void)ISHELL_StartAppletArgs(pMe->m_pShell,
                          AEECLSID_APP_RECENTCALL,
                          args);
   (void) ISHELL_PostEvent(pMe->m_pShell,
                           AEECLSID_CALL,
                           EVT_USER_CLOSEAPP,
                           0,
                           0);      
   if(args)
   {
      FREE(args);
   }
}

/*==============================================================================
����: 
       recentcalls_InitAppData
       
˵��: 
       ��ʼ��recentcalls Applet�ṹ����ȱʡֵ��
       
����: 
       pMe [in]��ָ��recentcalls Applet����ṹ��ָ�롣�ýṹ����С������ض���Ϣ��
       
����ֵ:
       SUCCESS����ʼ���ɹ���
       EFAILED����ʼ��ʧ�ܡ�
       
��ע:
       
==============================================================================*/
static int recentcalls_InitAppData(CRecentCalls *pMe)
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
    pMe->m_ePreState = STATE_REXIT;
    pMe->m_eCurState = STATE_VERIFY_PASSWORD;
    pMe->m_pDialog = NULL;
    pMe->m_eDlgRet = DLGRET_CREATE;
    pMe->m_bNotOverwriteDlgRet = FALSE;
    pMe->key_enable = TRUE; 
    pMe->record_selected = 0;
    
    if (AEE_SUCCESS != ISHELL_CreateInstance(pMe->m_pShell,
                                             AEECLSID_CONFIG,
                                             (void **)&pMe->m_pConfig))
    
    {
        return EFAILED;
    }
#if 0
    if (AEE_SUCCESS != ISHELL_CreateInstance(pMe->m_pShell,
                                             AEECLSID_CALLLIST,
                                             (void **)&pMe->m_pCallList))
    
    {
        return EFAILED;
    }
#endif
    if (AEE_SUCCESS != ISHELL_CreateInstance(pMe->m_pShell,
                                             AEECLSID_CALLHISTORY,
                                             (void **)&pMe->m_pCallHistory))
    
    {
        recentcalls_FreeAppData( pMe); // 2008.12.2
        return EFAILED;
    }
    if( ISHELL_CreateInstance( pMe->m_pShell,
                             AEECLSID_TEXTCTL,
                             (void **)&pMe->m_pPhoneLockPasswordTextCtl
                         ) != AEE_SUCCESS
    )
    {
        recentcalls_FreeAppData( pMe); // 2008.03.14
        return EFAILED;
    }
    if( ISHELL_CreateInstance( pMe->m_pShell,
                             AEECLSID_STATIC,
                             (void **)&pMe->m_pStatic
                         ) != SUCCESS
    )
    {
        recentcalls_FreeAppData( pMe);
        return EFAILED;
    }    
    pMe->m_pPhoneLockPassword = NULL;

    if (ISHELL_CreateInstance(pMe->m_pShell,
                                 AEECLSID_ANNUNCIATOR,
                                 (void **) &pMe->m_pIAnn))
    {
        return EFAILED;
    }
	MSG_FATAL("IANNUNCIATOR_SetFieldIsActiveEx::::recentcall111:::",0,0,0);
	//IANNUNCIATOR_SetFieldIsActiveEx(pMe->m_pIAnn,FALSE);
#ifdef FEATURE_LED_CONTROL
    if (ISHELL_CreateInstance(pMe->m_pShell,
                                            AEECLSID_BACKLIGHT,
                                            (void **)&pMe->m_pBacklight)!=AEE_SUCCESS)
    {
        return EFAILED;
    }
#endif    
    pMe->m_selectedItem[0] = 0;
    pMe->m_selectedItem[1] = 0; 

    return SUCCESS;
}

/*==============================================================================
����: 
       recentcalls_FreeAppData
       
˵��: 
       �ͷ�recentcalls Appletʹ�õ������Դ��
       
����: 
       pMe [in]��ָ��recentcalls Applet����ṹ��ָ�롣�ýṹ����С������ض���Ϣ��
       
����ֵ:
       �ޡ�
       
��ע:
       
==============================================================================*/
static void recentcalls_FreeAppData(CRecentCalls *pMe)
{
    int j;
    
    if (NULL == pMe)
    {
        return ;
    }
    
    for(j=0; j<AEECALLHISTORY_MAX_CALLS; j++)
    {
        FREEIF(pMe->list_record[j].name);
        FREEIF(pMe->list_record[j].number);
    }
    
    if(pMe->Image){ 
       IIMAGE_Release(pMe->Image); 
       pMe->Image = NULL;
    }
    
    if(pMe->m_pConfig)
    {
       ICONFIG_Release(pMe->m_pConfig);
       pMe->m_pConfig = NULL;
    } 
    #if 0
    if(pMe->m_pCallList)
    {
       ICALLLIST_Release(pMe->m_pCallList);
       pMe->m_pCallList = NULL;
    } 
    #endif
    if(pMe->m_pCallHistory)
    {
       ICALLHISTORY_Release(pMe->m_pCallHistory);
       pMe->m_pCallHistory = NULL;
    }
    if( pMe->m_pPhoneLockPasswordTextCtl)
    {
        ITEXTCTL_Release( pMe->m_pPhoneLockPasswordTextCtl);
        pMe->m_pPhoneLockPasswordTextCtl = NULL;
    }
    if( pMe->m_pStatic != NULL)
    {
        ISTATIC_Release(pMe->m_pStatic);
        pMe->m_pStatic = NULL;        
    }
    FREEIF( pMe->m_pPhoneLockPassword); // 2008.03.18

    if (pMe->m_pIAnn)
    {
        IANNUNCIATOR_Release(pMe->m_pIAnn);
        pMe->m_pIAnn= NULL;
    }
#ifdef FEATURE_LED_CONTROL
    if(pMe->m_pBacklight)
    {
        IBACKLIGHT_Release(pMe->m_pBacklight);
        pMe->m_pBacklight=NULL;
    }
#endif

    RecentCalls_FreeSelectRecordListNode(pMe);

}

/*==============================================================================
����: 
       recentcalls_RunFSM
       
˵��: 
       recentcalls Applet����״̬�����档
       
����: 
       pMe [in]��ָ��recentcalls Applet����ṹ��ָ�롣�ýṹ����С������ض���Ϣ��
       
����ֵ:
       �ޡ�
       
��ע:
       
==============================================================================*/
static void recentcalls_RunFSM(CRecentCalls *pMe)
{
    NextFSMAction nextFSMAction = NFSMACTION_WAIT;

    for ( ; ; )
    {
        nextFSMAction = recentcalls_ProcessState(pMe);

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
       recentcalls_HandleEvent
       
˵��: 
       recentcalls  Applet�¼��������������������ڱ�Applet���¼���
       �������ɸú�������
       
����: 
       pi [in]��ָ��IRecentCalls�ӿ�t�����ָ�롣
       eCode��BREW�¼����롣
       wParam�����¼�����������ݡ�
       dwParam [in]�����¼�����������ݡ�
       
����ֵ:
       TRUE�������¼�������
       FALSE�������¼�û������
       
��ע:
       
==============================================================================*/
static boolean recentcalls_HandleEvent(IRecentCalls *pi,
    AEEEvent eCode,
    uint16  wParam,
    uint32 dwParam
)
{
    CRecentCalls *pMe = (CRecentCalls*)pi;
    AEEAppStart* as = 0;
    switch (eCode)
    {
        case EVT_APP_START:
            pMe->m_bAppIsReady = FALSE; 
            //ASSERT(dwParam != 0);
            as = (AEEAppStart*)dwParam;
            if (NULL != pMe->m_pDisplay)
            {
                (void) IDISPLAY_Release(pMe->m_pDisplay);
                pMe->m_pDisplay = NULL;
            }
            pMe->m_pDisplay = as->pDisplay;
            (void) IDISPLAY_AddRef(pMe->m_pDisplay);
            pMe->m_rc = as->rc;

            pMe->m_bSuspending = FALSE;
            #if 0
            if (TRUE != ICALLLIST_InitEnd(pMe->m_pCallList))
            {
                return TRUE;   
            }
            #endif
            if( as->pszArgs != NULL && STRLEN( as->pszArgs) > 0)
            {
                pMe->m_charAppStartArgs = as->pszArgs[0];
                pMe->m_pSelFldCB =(PFNSELECTEDRECENTCALLSCB)ATOI(as->pszArgs + 1);
            }
            else
            {
                pMe->m_charAppStartArgs = 0;
            }

            // ��ʼRecentCalls״̬��
            if(*as->pszArgs == STARTARG_OUTGCALL)
            {
               pMe->m_callsCategory = AEECALLHISTORY_CALL_TYPE_TO;//CALLHISTORY_OUTGOING_CATEGORY;
               //MOVE_TO_STATE(STATE_RECORD_LIST);
            }
            else if(*as->pszArgs == STARTARG_MISSEDCALL)
            {         
               pMe->m_callsCategory = AEECALLHISTORY_CALL_TYPE_MISSED;//CALLHISTORY_MISSED_CATEGORY;
               //MOVE_TO_STATE(STATE_RECORD_LIST);
            }
            // ��ʼrecentcalls״̬��
            IANNUNCIATOR_SetFieldIsActiveEx(pMe->m_pIAnn,FALSE);
            recentcalls_RunFSM(pMe);
            return TRUE;

        case EVT_APP_STOP:
            if (pMe->m_pDisplay != NULL)
            {
                (void) IDISPLAY_Release(pMe->m_pDisplay);
                pMe->m_pDisplay = NULL;
            }
            pMe->m_bSuspending = TRUE;
            (void)ISHELL_CancelTimer(pMe->m_pShell, NULL, pMe);
            return TRUE;

        case EVT_APP_SUSPEND:
            pMe->m_bSuspending = TRUE;
            (void)ISHELL_CancelTimer(pMe->m_pShell, NULL, pMe);
            return TRUE;

        case EVT_APP_RESUME:
            //ASSERT(dwParam != 0);
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
            pMe->key_enable = TRUE; 
            if(pMe->m_eCurState == STATE_RECORDDEAL)
            {
                pMe->m_eCurState = STATE_RECORD_LIST;
            }
            recentcalls_RunFSM(pMe);
            return TRUE;

        case EVT_DIALOG_INIT:
            pMe->m_bAppIsReady = FALSE;
            pMe->m_pDialog = (IDialog*)dwParam;
            pMe->m_pActiveDlgID = wParam;

            return recentcalls_RouteDialogEvent(pMe,eCode,wParam,dwParam);

        case EVT_DIALOG_START:
            return recentcalls_RouteDialogEvent(pMe,eCode,wParam,dwParam);

        case EVT_USER_REDRAW:
            (void)recentcalls_RouteDialogEvent(pMe,eCode,wParam,dwParam);
            (void)ISHELL_SetTimer ( pMe->m_pShell,
                                    APPISREADY_TIMER,
                                    recentcalls_APPIsReadyTimer,
                                    pMe);
            return TRUE;
        
        case EVT_APPISREADY:
            (void) recentcalls_RouteDialogEvent(pMe,eCode,wParam,dwParam);
            pMe->m_bAppIsReady = TRUE;
            return TRUE;
  
        case EVT_KEY_PRESS:
        case EVT_KEY_RELEASE:
        case EVT_KEY:
        case EVT_COMMAND:
            if (!pMe->m_bAppIsReady)
            {
                return TRUE;
            }
            return recentcalls_RouteDialogEvent(pMe,eCode,wParam,dwParam);

        case EVT_DIALOG_END:
            if (wParam == 0)
            {
                return TRUE;
            }
            pMe->m_bAppIsReady = FALSE; 
            (void) recentcalls_RouteDialogEvent(pMe,eCode,wParam,dwParam);
            pMe->m_pDialog = NULL;

            // Applet�����𣬲���״̬�����ȴ�Applet���ء���ע�⣺EVT_APP_SUSPEND
            // �¼���EVT_DIALOG_END�¼�ǰ������
            if (pMe->m_bSuspending == FALSE)
            {
                // ��ʼrecentcalls״̬��
                recentcalls_RunFSM(pMe);
            }
            return TRUE;
#ifdef FEATURE_LCD_TOUCH_ENABLE//wlh add for LCD touch
					case EVT_PEN_UP:
					case EVT_PEN_DOWN:
						{
							return recentcalls_RouteDialogEvent(pMe,eCode,wParam,dwParam);
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
							return recentcalls_RouteDialogEvent(pMe,eCode,wParam,dwParam);
						}
#endif

        default:
            // �����յ����¼�·������ǰ��ĶԻ����¼���������
            return recentcalls_RouteDialogEvent(pMe,eCode,wParam,dwParam);
    }
}

/*==============================================================================
������recentcalls_APPIsReadyTimer

˵����
       ��ʱ���ص���������Ҫ���ڿ��ƿ��ٰ�������Ĵ���

������
       pme [in]��void *����ָ��

����ֵ��
        �ޡ�

��ע��:
        ����ÿһ���Ի��������ڽ���׼���׵�ʱ����Applet����
        EVT_USER_REDRAW�¼����ڸ��¼�����������ö�ʱ������
        ��ͳһ����
        
==============================================================================*/
static void recentcalls_APPIsReadyTimer(void *pme)
{
    CRecentCalls *pMe = (CRecentCalls *)pme;

    if (NULL == pMe)
    {
        return;
    }

    (void) ISHELL_PostEvent( pMe->m_pShell,
                             AEECLSID_APP_RECENTCALL,
                             EVT_APPISREADY,
                             0,
                             0);
}


/*==============================================================================
������RecentCalls_FreeSelectRecordListNode

˵����
       �ͷ�SelectRecordList

������

����ֵ��

��ע��
       
==============================================================================*/
void RecentCalls_FreeSelectRecordListNode(CRecentCalls *pMe)
{
    extern sSelectRecordListNode* pSelectRecordListRoot;
    sSelectRecordListNode *pNode = pSelectRecordListRoot;
    
    while (pNode != NULL)
    {
        FREEIF(pNode->SelectRecord.number);
        FREEIF(pNode->SelectRecord.name);
        
        pSelectRecordListRoot = pNode->pNextNode;
        
        if(NULL != pNode)
        {
            FREE(pNode);
        }
        
        pNode = pSelectRecordListRoot;
    }
}
/*==================================================================================
FUNCTION 
        RecentCalls_GetRemainWMSNum
        
Description 
        ��ȡ���ŷ����б�ʣ���������
        
PARAMETERS:
        *pMe: CRecentCalls object pointer
        
RETURN VALUE:
        uint16: ʣ���������
   
COMMENTS:
   
SIDE EFFECTS:

===================================================================================*/
uint16 RecentCalls_GetRemainWMSNum(CRecentCalls *pMe)
{
    IWmsApp *pWmsApp = NULL;
    int     nRemainNum = 0;
    
    if (SUCCESS == ISHELL_CreateInstance(pMe->m_pShell, AEECLSID_WMSAPP, (void**)&pWmsApp))
    {
        (void)IWmsApp_GetLeftSendListNodeNum(pWmsApp, &nRemainNum);
        IWmsApp_Release(pWmsApp);
    }
    
    return (uint16)nRemainNum;
}

