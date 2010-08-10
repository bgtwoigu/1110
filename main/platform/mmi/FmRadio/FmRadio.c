/*==============================================================================
// �ļ���
//        fmRadio.c
//        2007-11-11 Gemsea����汾(Draft Version)
//        ��Ȩ����(c) 2007 Anylook.,LTD.
//
// �ļ�˵����
//
// ���ߣ�2007-11-11
// �������ڣ�Gemsea
// ��ǰ�汾��Draft
==============================================================================*/

/*==============================================================================
�޸���ʷ��¼��
ʱ       ��      �� �� ��     �޸����ݡ�λ�ü�ԭ��
-----------      ----------     -----------------------------------------------
2007-11-11              ��ʼ�汾
==============================================================================*/


/*==============================================================================
                                 ���ļ��������ⲿ�ļ�
==============================================================================*/
#include "FmRadio_priv.h"
#ifdef FEATURE_APP_MUSICPLAYER
#include "MusicPlayer.h"
#endif 
#include "Appscommon.h"
#include "hs.h"
#include "snd.h"
/*==============================================================================
                                 �궨��ͳ���
==============================================================================*/
#define EVT_UI_EXIT                 1150

/*==============================================================================
                                 ���Ͷ���
==============================================================================*/

/*==============================================================================
                                 ��������
==============================================================================*/
//static void FmRadio_APPIsReadyTimer(void *pme);

/*----------------------ģ����غ�������---------------------*/
 
int  FmRadioMod_Load(IShell   *pIShell,
    void     *ph,
    IModule **ppMod
);

int  FmRadioMod_New(int16  nSize,
    IShell   *pIShell,
    void     *ph,
    IModule  **ppMod,
    PFNMODCREATEINST pfnMC,
    PFNFREEMODDATA pfnMF
);

static int  FmRadioMod_CreateInstance(IModule *po,
    IShell *pIShell,
    AEECLSID ClsId,
    void **ppObj
);

static uint32 FmRadioMod_AddRef(IModule *po);

static uint32 FmRadioMod_Release(IModule *po);

static void FmRadioMod_FreeResources(IModule  *po,
    IHeap  *ph,
    IFileMgr *pfm
);

/*----------------------Applet��غ�������---------------------*/
static int FmRadio_New(IShell *ps,
    IModule *pIModule,
    IFmRadio **ppObj
);

static uint32  FmRadio_AddRef(IFmRadio *pi);

static uint32  FmRadio_Release (IFmRadio *pi);

static boolean FmRadio_HandleEvent(IFmRadio *pi,
    AEEEvent eCode,
    uint16  wParam,
    uint32 dwParam
);

static int FmRadio_InitAppData(CFmRadio *pMe);
static void FmRadio_FreeAppData(CFmRadio *pMe);
static void FmRadio_RunFSM(CFmRadio *pMe);
static void FmRadio_InitFmRadioResource(CFmRadio *pMe);
static void FmRadio_PowerDown( CFmRadio *pMe);
static void FmRadio_PowerUp(void *pme);
static void FmRadio_AppIsReadyCB(void *pUser);

/*==============================================================================
                                 ȫ������
==============================================================================*/
// ��Ҫ��ʾ��
//        g_dwAEEStdLibEntryȫ�ֱ���������SDK ��
//        BREW SDK�û����ܸ�д��ֱ��ʹ�ñ�������������AEEStdLib��һ����ڵ㣬��
//        Ҫ�ƶ��������д��롣
#ifdef AEE_SIMULATOR
   uint32 g_dwAEEStdLibEntry;
#endif

#if FEATURE_FMRADIO_CHANNEL_LIST_SUPPORT
void FmRadio_ClearChanList( CFmRadio* pMe);
void FmRadio_ChanList_EnumInit( CFmRadio* pMe);
void FmRadio_ChanList_EnumInit_WithLoop( CFmRadio* pMe);
boolean FmRadio_ChanList_EnumNext( CFmRadio* pMe);
boolean FmRadio_ChanList_EnumPrevious_WithLoop( CFmRadio* pMe);
boolean FmRadio_ChanList_EnumNext_WithLoop( CFmRadio* pMe);
sChanInfo* FmRadio_ChanList_GetCurrent( CFmRadio* pMe);
sChanInfo* FmRadio_ChanList_GetCurrent_WithLoop( CFmRadio* pMe);
sChanInfo* FmRadio_ChanList_GetByIndex( CFmRadio* pMe, int index);
boolean FmRadio_ChanList_DeleteByIndex( CFmRadio* pMe, int index);
boolean FmRadio_IsChannelValid( uint16 channel);
#endif
void Fm_Shake_Open(void);
/*==============================================================================
                                 ���أ���̬������
==============================================================================*/
static FmRadioMod gFmRadioMod;

static const VTBL(IModule) gModFuncs =
{
    FmRadioMod_AddRef,
    FmRadioMod_Release,
    FmRadioMod_CreateInstance,
    FmRadioMod_FreeResources
};

// ֻ����һ��FmRadioʵ����ÿ�δ���FmRadio Appletʱ��
// ����ͬһ�ṹָ���BREW�㡣
static CFmRadio gFmRadio;

static const VTBL(IFmRadio) gFmRadioMethods =
{
    FmRadio_AddRef,
    FmRadio_Release,
    FmRadio_HandleEvent
};

static boolean g_m_fm_is_on = FALSE;
/*----------------------ģ����غ�������---------------------*/

/*==============================================================================
����:
       FmRadioMod_Load

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
#ifndef BREW_STATIC_APP
__declspec(dllexport) int AEEMod_Load(IShell   *pIShell,
                                                        void     *ph,
                                                        IModule **ppMod)
#else
int FmRadioMod_Load(IShell   *pIShell,
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

    return FmRadioMod_New(sizeof(FmRadioMod),
                          pIShell,
                          ph,
                          ppMod,
                          NULL,
                          NULL);
}

/*==============================================================================
����:
       FmRadioMod_New

˵��:
       ������ʼ��FmRadioģ�������Ҳ�������ü����������ֺ�����

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
int  FmRadioMod_New(int16  nSize,
                    IShell   *pIShell,
                    void     *ph,
                    IModule  **ppMod,
                    PFNMODCREATEINST pfnMC,
                    PFNFREEMODDATA pfnMF
                   )
{

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

    MEMSET(&gFmRadioMod,0,sizeof(FmRadioMod));
    INIT_VTBL(&gFmRadioMod, IModule, gModFuncs);

    gFmRadioMod.m_nRefs = 1;
    *ppMod = (IModule *)&gFmRadioMod;

    return AEE_SUCCESS;
}

/*==============================================================================
����:
       FmRadioMod_AddRef

˵��:
       FmRadio ģ�����ü�����1�������ص�ǰ����������

����:
       po [in]��IModule�ӿڶ���ָ�롣

����ֵ:
       ��ǰ��FmRadio ģ���������

��ע:

==============================================================================*/
static uint32 FmRadioMod_AddRef(IModule *po)
{
    return(++((FmRadioMod *)po)->m_nRefs);
}

/*==============================================================================
����:
       FmRadioMod_Release

˵��:
       FmRadio ģ�����ü�����1�������ص�ǰ����������

����:
       po [in]��IModule�ӿڶ���ָ�롣

����ֵ:
       ��ǰ��FmRadio ģ���������

��ע:

==============================================================================*/
static uint32 FmRadioMod_Release(IModule *po)
{
    if (((FmRadioMod *) po)->m_nRefs == 0)
    {
        return 0;
    }

    return(--((FmRadioMod *)po)->m_nRefs);
}

/*==============================================================================
����:
       FmRadioMod_CreateInstance

˵��:
        ͨ������(��ʼ��)FmRadio����������ģ��ʵ����

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
static int  FmRadioMod_CreateInstance(IModule   *po,
                                      IShell    *pIShell,
                                      AEECLSID  ClsId,
                                      void      **ppObj
                                     )
{
    *ppObj = NULL;

    if (ClsId != AEECLSID_APP_FMRADIO)
    {
        return EFAILED;
    }

    if (FmRadio_New(pIShell, po, (IFmRadio**)ppObj) != SUCCESS)
    {
        return EFAILED;
    }
    return SUCCESS;
}

/*==============================================================================
����:
       FmRadioMod_FreeResources

˵��:
        �ͷ�FmRadioģ����Դ�ĺ�����

����:
       po ��IModule��
       ph ��IHeap�ӿ�ָ�롣
       pfm :IFileMgr�ӿ�ָ�롣

����ֵ:
       ��

��ע:
       �����û�ȥ���á�

==============================================================================*/
static void FmRadioMod_FreeResources(IModule  *po,
    IHeap  *ph,
    IFileMgr *pfm
)
{

}

/*----------------------Applet��غ�������---------------------*/
/*==============================================================================
����:
       FmRadio_New

˵��:
        ��ʼ��FmRadio Applet�������ڲ����ݱ�����ͬʱ�����ü�����1��

����:
       ps [in]��IShell�ӿڶ���ָ�롣
       pIModule [in]��pIModule�ӿڶ���ָ�롣
       ppObj [out]:��ָ��IFmRadio����ָ���ָ�롣

����ֵ:
       EFAILED��ʧ�ܡ�
       SUCCESS���ɹ���

��ע:
       �����û�ȥ���á�
       ����Applet�ṹ�����ʹ�ö�̬�����ڴ�ķ�ʽ�������������õ���ʹ�þ�̬
       �����ķ�ʽ������Խṹ�Ƚϴ��Appletʹ�þ�̬�����ķ�ʽ�����ڶ�̬����
       �ڴ�ķ�ʽ��Ҫע���������ռ���ͷš�

==============================================================================*/
static int FmRadio_New(IShell *ps,
                       IModule *pIModule,
                       IFmRadio **ppObj
                      )
{
    int retVal = SUCCESS;

    if (0 == gFmRadio.m_nRefs)
    {
        // Must initialize the object
        MEMSET(&gFmRadio,  0, sizeof(CFmRadio));
        INIT_VTBL(&gFmRadio, IFmRadio, gFmRadioMethods);

        gFmRadio.m_nRefs     = 0;
        gFmRadio.m_pShell    = ps;
        gFmRadio.m_pModule   = pIModule;

        (void) ISHELL_AddRef(ps);
        (void) IMODULE_AddRef(pIModule);

        retVal = FmRadio_InitAppData(&gFmRadio);
    }

    ++gFmRadio.m_nRefs;
    *ppObj = (IFmRadio*)&gFmRadio;
    return retVal;
}

/*==============================================================================
����:
       FmRadio_AddRef

˵��:
       FmRadio Applet ���ü�����1�������ص�ǰ����������

����:
       pi [in]��IFmRadio�ӿڶ���ָ�롣

����ֵ:
       ��ǰ��FmRadio Applet��������

��ע:

==============================================================================*/
static uint32  FmRadio_AddRef(IFmRadio *pi)
{
    register CFmRadio *pMe = (CFmRadio*)pi;

    return (++pMe->m_nRefs);
}

/*==============================================================================
����:
       FmRadio_Release

˵��:
       �ͷŶ�FmRadio Applet�����ã����ü�����1�������ص�ǰ����������
        �����ǰ��������Ϊ�㣬�����ͷ�Applet�������Դ��

����:
       pi [in]��IFmRadio�ӿڶ���ָ�롣

����ֵ:
       ��ǰ��FmRadio Applet��������

��ע:

==============================================================================*/
static uint32  FmRadio_Release (IFmRadio *pi)
{
    register CFmRadio *pMe = (CFmRadio*)pi;

    if (pMe->m_nRefs == 0)
    {
        return 0;
    }

    if (--pMe->m_nRefs)
    {
        return pMe->m_nRefs;
    }

    // �ͷ�Applet�����Դ
    FmRadio_FreeAppData(pMe);

    (void) ISHELL_Release(pMe->m_pShell);
    (void) IMODULE_Release(pMe->m_pModule);

    // ע�⣺pMe�Ǿ�̬����ռ䣬��˲���Ҫ�ͷš�FREE()
    return 0;
}

static int FmRadio_InitAppData(CFmRadio *pMe)
{
	if (NULL  == pMe)
    {
        return EFAILED;
    }

    if( (AEE_SUCCESS != ISHELL_CreateInstance(pMe->m_pShell, AEECLSID_CONFIG, (void **)&pMe->m_pConfig)) ||
        (AEE_SUCCESS != ISHELL_CreateInstance(pMe->m_pShell, AEECLSID_MENUCTL, (void **)&pMe->m_pMenu))  ||
        (AEE_SUCCESS != ISHELL_CreateInstance(pMe->m_pShell, AEECLSID_STATIC, (void **)&pMe->pStatic))   ||
        (AEE_SUCCESS != ISHELL_CreateInstance(pMe->m_pShell, AEECLSID_TEXTCTL, (void **)&pMe->pText))    ||
        (AEE_SUCCESS != ISHELL_CreateInstance(pMe->m_pShell, AEECLSID_ANNUNCIATOR, (void **)&pMe->m_pIAnn))
#if !defined( AEE_SIMULATOR)
        ||
        ISHELL_RegisterNotify( pMe->m_pShell, AEECLSID_APP_FMRADIO, AEECLSID_CM_NOTIFIER, NMASK_CM_VOICE_CALL | NMASK_CM_OTHER_CALL | NMASK_CM_TEST_CALL) != SUCCESS
#endif
    )
    {
        FmRadio_FreeAppData( pMe);
        return EFAILED;
    }
    IANNUNCIATOR_SetHasTitleText(pMe->m_pIAnn, FALSE);
    pMe->m_ePreState            = FMRADIOST_NONE;
    pMe->m_eCurState            = FMRADIOST_INIT;
    pMe->m_pActiveDlg           = NULL;
    pMe->m_eDlgRet              = DLGRET_CREATE;
    pMe->m_bNotOverwriteDlgRet  = FALSE;
    pMe->startFromBackground    = FALSE;
    pMe->m_pVolumeImage = NULL;
    //��ʼ����Ҫ������
    FmRadio_InitFmRadioResource( pMe);
    return SUCCESS;
}

/*==============================================================================
����:
       FmRadio_InitFmRadioResource

˵��:
       FmRadio  Applet�¼��������������������ڱ�Applet���¼���
       �������ɸú�������

����:

����ֵ:

��ע:

==============================================================================*/
static void FmRadio_InitFmRadioResource(CFmRadio *pMe)
{

	byte byMax = 0;

#if FEATURE_FMRADIO_CHANNEL_LIST_SUPPORT
	FmRadio_ClearChanList( pMe);
#endif
	//��ʼ��FM Radio����
	(void) ICONFIG_GetItem(pMe->m_pConfig,
						   CFGI_FMRADIO_VOLUME,
						   &pMe->byVolumeLevel,
						   sizeof(byte));
	if( pMe->byVolumeLevel > MAX_FMRADIO_VOLUME || pMe->byVolumeLevel == 0)
	{
		pMe->byVolumeLevel = 3;//(MAX_FMRADIO_VOLUME + 1) / 2;
	}

#if FEATURE_FMRADIO_CHANNEL_LIST_SUPPORT
	//��ʼ����Ƶ������Ϣ
	(void) ICONFIG_GetItem(pMe->m_pConfig,
						   CFGI_FMRADIO_CHAN_INFO,
						   (void*) pMe->chanInfoList,
						   sizeof( pMe->chanInfoList));

	//��ʼ��Ƶ������
	(void) ICONFIG_GetItem(pMe->m_pConfig,
						   CFGI_FMRADIO_CHAN_TOTAL,
						   &byMax,
						   sizeof(byte));

	//�����潫Node��ӵ�List�Ĺ�����byChannelMax���Զ����ӡ�
	if( byMax > MAX_FMRADIO_STORED_CHANNEL)
	{
		byMax = MAX_FMRADIO_STORED_CHANNEL;
	}

	pMe->byChannelMax                     = byMax;
	pMe->channelListMenuSelectedItemId    = -1;
	FmRadio_ChanList_EnumInit( pMe);
	FmRadio_ChanList_EnumInit_WithLoop( pMe);

	if( ISHELL_GetPrefs( pMe->m_pShell, AEECLSID_APP_FMRADIO, 1, &pMe->cfg, sizeof(pMe->cfg)) == SUCCESS)
	{

		while( FmRadio_ChanList_EnumNext( pMe))
		{
			if( FmRadio_ChanList_GetCurrent( pMe)->wChannel == pMe->cfg.channel)
			{
				goto __FmRadio_InitFmRadioResource_end__;
			}
		}

		if( pMe->cfg.tuningMode < 0 || pMe->cfg.tuningMode > 2)
		{
			pMe->cfg.tuningMode = FM_RADIO_TUNNING_MODE_LIST;
		}
	}
	else
	{

		if( FmRadio_ChanList_EnumNext( pMe))
		{
			pMe->cfg.channel = FmRadio_ChanList_GetCurrent( pMe)->wChannel;
		}
		else
		{
			pMe->cfg.channel = 0;
		}

		pMe->cfg.tuningMode = FM_RADIO_TUNNING_MODE_LIST;
	}
#else
	pMe->cfg.channel = 96;
	pMe->cfg.tuningMode = FM_RADIO_TUNNING_MODE_MANUAL;
#endif


__FmRadio_InitFmRadioResource_end__:
	pMe->opMode         = FM_RADIO_OPMODE_PLAY;
	pMe->refuseReason   = FM_RADIO_REFUSE_REASON_NOT_REFUSE;
}

/*==============================================================================
����:
       FmRadio_FreeAppData

˵��:
       �ͷ�FmRadio Appletʹ�õ������Դ��

����:
       pMe [in]��ָ��FmRadio Applet����ṹ��ָ�롣�ýṹ����С������ض���Ϣ��

����ֵ:
       �ޡ�

��ע:

==============================================================================*/
static void FmRadio_FreeAppData(CFmRadio *pMe)
{
    if (NULL == pMe)
    {
        return ;
    }

    ISHELL_RegisterNotify( pMe->m_pShell,
                           AEECLSID_APP_FMRADIO,
                           AEECLSID_CM_NOTIFIER,
                           0
                      );

    if (pMe->m_pConfig)
    {
        ICONFIG_Release(pMe->m_pConfig);
        pMe->m_pConfig = NULL;
    }

    if(pMe->m_pMenu)
    {
        IMENUCTL_Release(pMe->m_pMenu);
        pMe->m_pMenu = NULL;
    }
    if(pMe->pStatic)
    {
        ISTATIC_Release(pMe->pStatic);
        pMe->pStatic = NULL;
    }
    if(pMe->pText)
    {
        ITEXTCTL_Release(pMe->pText);
        pMe->pText = NULL;
    }
    if(pMe->m_pIAnn)
    {
        IANNUNCIATOR_SetHasTitleText(pMe->m_pIAnn, TRUE);
        IANNUNCIATOR_Release( pMe->m_pIAnn);
        pMe->m_pIAnn = NULL;
    }
    if (pMe->m_pVolumeImage)
    {
       IIMAGE_Release(pMe->m_pVolumeImage);
       pMe->m_pVolumeImage = NULL;
    }    
}

/*==============================================================================
����:
       FmRadio_RunFSM

˵��:
       FmRadio Applet����״̬�����档

����:
       pMe [in]��ָ��FmRadio Applet����ṹ��ָ�롣�ýṹ����С������ض���Ϣ��

����ֵ:
       �ޡ�

��ע:

==============================================================================*/
static void FmRadio_RunFSM(CFmRadio *pMe)
{
    NextFSMAction nextFSMAction = NFSMACTION_WAIT;

    for ( ; ; )
    {
        nextFSMAction = FmRadio_ProcessState(pMe);

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
       FmRadio_HandleEvent

˵��:
       FmRadio  Applet�¼��������������������ڱ�Applet���¼���
       �������ɸú�������

����:
       pi [in]��ָ��IFmRadio�ӿ�t�����ָ�롣
       eCode��BREW�¼����롣
       wParam�����¼�����������ݡ�
       dwParam [in]�����¼�����������ݡ�

����ֵ:
       TRUE�������¼�������
       FALSE�������¼�û������

��ע:

==============================================================================*/
static void FmRadio_CheckRefuse( CFmRadio* pMe)
{
#if !FEATURE_TEST_VERSION_WITHOUT_HEADSET_PRESENCE_VERIFY
    boolean             headsetPresent   = TRUE;
#endif
    db_items_value_type dbItemValue      = {0};

#if !FEATURE_TEST_VERSION_WITHOUT_HEADSET_PRESENCE_VERIFY
    //ICONFIG_GetItem(pMe->m_pConfig, CFGI_HEADSET_PRESENT, &headsetPresent, sizeof(boolean));
    headsetPresent = HS_HEADSET_ON();

	DBGPRINTF("***zzg FmRadio headsetPresent=%d***", headsetPresent);
	
#endif
#if 0 //��������ʱ�����Զ����𣬲���Ҫ����������
    db_get( DB_IN_USE, &dbItemValue);

    pMe->refuseReason = FM_RADIO_REFUSE_REASON_NOT_REFUSE;
    if( dbItemValue.in_use)
    {
        pMe->refuseReason = FM_RADIO_REFUSE_REASON_VOICE_CALL_CONNECTED;
    }
#endif    
#if !FEATURE_TEST_VERSION_WITHOUT_HEADSET_PRESENCE_VERIFY
    if( !headsetPresent)
    {
        pMe->refuseReason = FM_RADIO_REFUSE_REASON_NO_HEADSET;
    }
#endif
    // bw:<< 20080808 : close app when pull out the headset
#if !FEATURE_TEST_VERSION_WITHOUT_HEADSET_PRESENCE_VERIFY
    if( headsetPresent)
    {
        ISHELL_CancelTimer( pMe->m_pShell, closeApp, (void*)pMe);
    }
    else
    {
        ISHELL_SetTimer( pMe->m_pShell, 2000, closeApp, (void*)pMe);
    }
#endif
    //bw:>>
}

static boolean FmRadio_HandleEvent(IFmRadio *pi,
                                   AEEEvent eCode,
                                   uint16  wParam,
                                   uint32 dwParam
                                  )
{
    CFmRadio *pMe = (CFmRadio*)pi;
    AEEAppStart *as;

    switch (eCode)
    {
        case EVT_APP_START:
        case EVT_APP_RESUME:
        {      
			DBGPRINTF("***zzg FmRadio_HandleEvent eCode=%d***", eCode);
			
            pMe->startFromBackground = FALSE;
            as = (AEEAppStart*)dwParam;
            pMe->m_pDisplay = as->pDisplay;
            pMe->m_rc = as->rc;
            {
                AEEDeviceInfo di; 
                ISHELL_GetDeviceInfo(pMe->m_pShell,&di);
                pMe->m_rc.dx = di.cxScreen;
                pMe->m_rc.dy = di.cyScreen;
            }
            if(pMe->m_pIAnn != NULL)
            {
                IANNUNCIATOR_SetHasTitleText(pMe->m_pIAnn, FALSE);            
                IANNUNCIATOR_Redraw(pMe->m_pIAnn);
            }

            pMe->m_bSuspending  = FALSE;

			DBGPRINTF("***zzg app_media_scheduler()=%d***", app_media_scheduler());
			
            if(APP_MEDIA_IMPACT_BY_MP3 == app_media_scheduler())
            {
                pMe->opMode = FM_RADIO_OPMODE_USER_MSG;
                pMe->m_eCurState = FMRADIOST_USER_MSG;
#if FEATURE_FMRADIO_SUPPORT_BACKGROUND                
                pMe->runOnBackground = TRUE;
#endif
                pMe->m_bAppIsReady = TRUE;
                FmRadio_RunFSM( pMe);
                return TRUE;
            }
			
            pMe->m_eCurState = FMRADIOST_INIT;  
            pMe->m_bAppIsReady  = FALSE;

			//Add By zzg 2010_07_18
#if FEATURE_FMRADIO_SUPPORT_BACKGROUND 			
			if (!pMe->runOnBackground )
#endif                
			
#if !defined( AEE_SIMULATOR)
            FmRadio_PowerUp( pMe);
			if (HS_HEADSET_ON())
			{
				fm_mute(FALSE);
			}		//Del By zzg 2010_07_18
			
#endif//#if !defined( AEE_SIMULATOR)
#if FEATURE_FMRADIO_SUPPORT_BACKGROUND 
            pMe->runOnBackground = FALSE;
#endif            
            if(NULL != as->pszArgs)
            {
                if( 'e' == as->pszArgs[0] )
                {
#if FEATURE_FMRADIO_SUPPORT_BACKGROUND                 
                    pMe->runOnBackground = TRUE;
#endif
                    ISHELL_CloseApplet(pMe->m_pShell, FALSE);
                    return TRUE;
                }
            }
            FmRadio_RunFSM( pMe);
        }
        return TRUE;

        case EVT_APP_STOP:
			DBGPRINTF("***zzg FmRadio_HandleEvent EVT_APP_STOP***");
            if( pMe->startFromBackground) 
            {
            	ISHELL_CancelTimer( pMe->m_pShell, 0, pMe);
            	return TRUE;
            }
			
#if !defined( AEE_SIMULATOR)
#if FEATURE_FMRADIO_SUPPORT_BACKGROUND 
            if( !pMe->runOnBackground)
#endif                
            {
            	//Add By zzg 2010_07_18
				if (HS_HEADSET_ON())
				{
					fm_mute(TRUE);
				}	
				else
				{
					//wl add!  2010-08-09
					snd_set_device(SND_DEVICE_HANDSET, SND_MUTE_UNMUTED, SND_MUTE_UNMUTED, NULL, NULL);
				}
				//Add End
				
                FmRadio_PowerDown( pMe);								
            #if FEATURE_FMRADIO_CHANNEL_LIST_SUPPORT
				FmRadio_SaveChannelList( pMe);
			#endif
				ISHELL_SetPrefs( pMe->m_pShell, AEECLSID_APP_FMRADIO, 1, &pMe->cfg, sizeof(pMe->cfg));
            }
#endif//#if !defined( AEE_SIMULATOR)
            pMe->m_bSuspending = TRUE;
#if FEATURE_FMRADIO_SUPPORT_BACKGROUND 
            *((boolean*)dwParam) = !pMe->runOnBackground;
#endif
            g_m_fm_is_on = FALSE;
            (void)ISHELL_CancelTimer( pMe->m_pShell,FmRadio_AppIsReadyCB,pMe);
            return TRUE;

        case EVT_APP_SUSPEND:
			DBGPRINTF("***zzg FmRadio_HandleEvent EVT_APP_SUSPEND***");
#if !defined( AEE_SIMULATOR)
            FmRadio_PowerDown( pMe);			
#endif//#if !defined( AEE_SIMULATOR)
#if FEATURE_FMRADIO_SUPPORT_BACKGROUND 
			//Add By zzg 2010_07_18
			if( !pMe->runOnBackground)
#endif                
			{
				if (HS_HEADSET_ON())
				{
					fm_mute(TRUE);
				}			
			}
			//Add End
			
            pMe->m_bSuspending = TRUE;
            (void)ISHELL_CancelTimer( pMe->m_pShell,FmRadio_AppIsReadyCB,pMe);
            return TRUE;

        case EVT_DIALOG_INIT:
			DBGPRINTF("***zzg FmRadio_HandleEvent EVT_DIALOG_INIT***");
            pMe->m_pActiveDlg = (IDialog*)dwParam;
            pMe->m_pActiveDlgID = wParam;
        case EVT_DIALOG_START:
        case EVT_USER_REDRAW:
			DBGPRINTF("***zzg FmRadio_HandleEvent EVT_DIALOG_START ecode=%x***", eCode);
			
            if( eCode == EVT_USER_REDRAW)
            {
                (void)ISHELL_SetTimer( pMe->m_pShell,                                       
                                       APPISREADY_TIMER,
                                       FmRadio_AppIsReadyCB,
                                       pMe);
            		//pMe->m_bAppIsReady = TRUE;                         
            }
            (void) FmRadio_RouteDialogEvent(pMe,eCode,wParam,dwParam);
            return TRUE;

        case EVT_KEY:
        case EVT_KEY_PRESS:
            if( pMe->m_bAppIsReady &&
                ( ( (wParam == AVK_CLR || wParam == AVK_SOFT2 || wParam == AVK_SELECT || wParam == AVK_SOFT1) &&
                    pMe->refuseReason != FM_RADIO_REFUSE_REASON_NOT_REFUSE
                  ) || wParam == AVK_END
                )
            )
            {
                //pMe->runOnBackground = (wParam == AVK_END);
#if FEATURE_FMRADIO_SUPPORT_BACKGROUND 
				//Add By zzg 2010_07_30
				if ((pMe->refuseReason != FM_RADIO_REFUSE_REASON_NO_HEADSET) 
					&& (pMe->refuseReason != FM_RADIO_REFUSE_REASON_HEADSET_PLUG_OUT))
				{
					pMe->runOnBackground = (wParam == AVK_END);
				}
				else
				{
					pMe->runOnBackground = FALSE;
				}
				//Add End
#endif				
				
                pMe->opMode = FM_RADIO_OPMODE_PLAY;
                ISHELL_CloseApplet( pMe->m_pShell, wParam == AVK_END ? TRUE : FALSE);
                return TRUE;
            }
        case EVT_KEY_RELEASE:
        case EVT_COMMAND:
            if (!pMe->m_bAppIsReady || pMe->refuseReason != FM_RADIO_REFUSE_REASON_NOT_REFUSE || pMe->newSmsIncoming)
            {
                return TRUE;
            }
            return FmRadio_RouteDialogEvent(pMe,eCode,wParam,dwParam);

        case EVT_DIALOG_END:
			DBGPRINTF("***zzg FmRadio_HandleEvent EVT_DIALOG_END***");
            if (wParam == 0)
            {
                return TRUE;
            }
            (void) FmRadio_RouteDialogEvent(pMe,eCode,wParam,dwParam);
            pMe->m_pActiveDlg = NULL;

            // Applet�����𣬲���״̬�����ȴ�Applet���ء���ע�⣺EVT_APP_SUSPEND
            // �¼���EVT_DIALOG_END�¼�ǰ������
            if( !pMe->m_bSuspending)
            {
                // ��ʼFmRadio״̬��
                FmRadio_RunFSM(pMe);
            }
            return TRUE;

        case EVT_NOTIFY:
        {
			DBGPRINTF("***zzg FmRadio_HandleEvent EVT_NOTIFY***");
#if FEATURE_FMRADIO_SUPPORT_BACKGROUND             
#if !defined( AEE_SIMULATOR)
            if( pMe->runOnBackground)
            {

                AEENotify* notify = (AEENotify*)dwParam;
                notify->st = NSTAT_IGNORED;
                if( notify->cls == AEECLSID_CM_NOTIFIER)
                {

                    if( ( notify->dwMask & NMASK_CM_VOICE_CALL))
                    {
                        AEECMNotifyInfo* eventInfo = (AEECMNotifyInfo *)(notify->pData);


                        if( eventInfo->event == AEECM_EVENT_CALL_ORIG    ||
                            eventInfo->event == AEECM_EVENT_CALL_INCOM   ||
                            eventInfo->event == AEECM_EVENT_CALL_END
                        )
                        {
                            if( eventInfo->event == AEECM_EVENT_CALL_END)
                            {
                                FmRadio_PowerUp( pMe);
                            }
                            else
                            {
                                FmRadio_PowerDown( pMe);
                            }

                            notify->st = NSTAT_PROCESSED;
                        }
                    }
                }
            }
#endif
#endif
            return TRUE;
        }

        case EVT_CLOSEAPP:
        {
			DBGPRINTF("***zzg FmRadio_HandleEvent EVT_CLOSEAPP***");
            pMe->startFromBackground = FALSE;
#if FEATURE_FMRADIO_SUPPORT_BACKGROUND             
            pMe->runOnBackground = FALSE;
#endif
            ISHELL_CloseApplet( pMe->m_pShell, FALSE);           
            return TRUE;
        }
        
        case EVT_HEADSET:
        {
        	if ( wParam == AVK_HEADSET_CONNECT )
        	{
				if (HS_HEADSET_ON())
				{
					fm_mute(FALSE);
				}
        	}
        	else
        	{
	            if( pMe->startFromBackground)
	            {
	                ISHELL_CloseApplet( pMe->m_pShell, FALSE);
	            }
	            else
	            {
	                FmRadio_PowerUp( pMe);
					if( pMe->refuseReason == FM_RADIO_REFUSE_REASON_NOT_REFUSE)
					{
						extern void resume( CFmRadio* pMe);
						resume( pMe);
					}
					else
					{
						if( IMENUCTL_IsActive( pMe->m_pMenu))
						{
							IMENUCTL_SetActive( pMe->m_pMenu, FALSE);
						}
						if( ITEXTCTL_IsActive( pMe->pText))
						{
							ITEXTCTL_SetActive( pMe->pText, FALSE);
						}

#if defined( FEATURE_FMRADIO_PROMPT_HEADSET_PLUG_OUT)
						if( pMe->refuseReason == FM_RADIO_REFUSE_REASON_NO_HEADSET)
						{
							pMe->refuseReason = FM_RADIO_REFUSE_REASON_HEADSET_PLUG_OUT;
						}
#endif
					}
					ISHELL_PostEvent( pMe->m_pShell,
	                             AEECLSID_APP_FMRADIO,
	                             EVT_USER_REDRAW,
	                             0,
	                             0
	                         );
	            }
            }
            
        }
        return TRUE;

        case EVT_WMS_PROCESS_NEW_MSG:
        {
            if( pMe->startFromBackground)
            {
                ISHELL_CloseApplet( pMe->m_pShell, FALSE);
            }
            else
            {

#if !defined( FEATURE_NO_SMS_VOICE_PROMPT)
                FmRadio_PowerDown( pMe);
#endif
                pMe->wmsEventTimestamp = GETTIMESECONDS();
                pMe->wmsEventTimer     = 2;
#if !defined( FEATURE_NO_SMS_VOICE_PROMPT)
                ISHELL_SetTimer( pMe->m_pShell, 2000, FmRadio_PowerUp, (void*)pMe);
#endif
                pMe->newSmsIncoming = TRUE;
#if FEATURE_FMRADIO_SUPPORT_BACKGROUND 
                if( !pMe->runOnBackground)
#endif                    
                {
                    FmRadio_RouteDialogEvent( pMe,eCode,wParam,dwParam);
                }
            }
        }
        return TRUE;

        case EVT_ALARM:
        {

            if( pMe->startFromBackground)
            {
                ISHELL_CloseApplet( pMe->m_pShell, FALSE);
            }
            else
            {
                pMe->alarmNow = wParam;
                if( wParam)
                {
                    FmRadio_PowerDown( pMe);
                }
                else
                {
                    FmRadio_PowerUp( pMe);
                }
            }
        }
        return TRUE;

        case EVT_FLIP:
        {
#if FEATURE_FMRADIO_SUPPORT_BACKGROUND             
            if( !wParam)
            {
                pMe->runOnBackground = TRUE;
            }
#endif            
        }
        return TRUE;
        case EVT_OPENSHAKE:
            Fm_Shake_Open();
            return TRUE;
            
        case EVT_USER:
        {
            switch(wParam) 
            {
                case EVT_UI_EXIT:            
                    /* AEE is about to exit, set the exit flag to TRUE */
                    pMe->startFromBackground = FALSE;
#if FEATURE_FMRADIO_SUPPORT_BACKGROUND                     
                    pMe->runOnBackground = FALSE;
#endif
                    ISHELL_CloseApplet( pMe->m_pShell, FALSE);
                    return TRUE;
                default:
                    break;
            }
        }

        default:
            // �����յ����¼�·������ǰ��ĶԻ����¼���������
            return FmRadio_RouteDialogEvent(pMe,eCode,wParam,dwParam);
    }
}

static boolean fm_set_key_beep = 0; // 1:set, 0:free
/*closed key beep if it is open.*/
static void FM_CloseKeyBeepVol(CFmRadio *pMe)
{
    byte mute = OEMSOUND_MUTE_VOL;

    ICONFIG_GetItem( pMe->m_pConfig, CFGI_BEEP_VOL, &pMe->keyBeepVol, sizeof(byte));
    ICONFIG_SetItem( pMe->m_pConfig, CFGI_BEEP_VOL, &mute, sizeof(byte));
    fm_set_key_beep = 1;
}

/*resume key beep.*/
static void Fm_ResumeKeyBeepVol(CFmRadio * pMe)
{
    if(1 == fm_set_key_beep)    //free keybeep setting.
    {
        ICONFIG_SetItem( pMe->m_pConfig, CFGI_BEEP_VOL, &pMe->keyBeepVol, sizeof(byte));
        fm_set_key_beep = 0;
    
    }
}

#ifdef FEATRUE_SUPPORT_G_SENSOR
#include "g_sensor.h"
#endif
#ifdef FEATURE_SPORTS_APP
extern boolean is_g_sportsapp_pedometer_bground_flag(void);
#endif
extern boolean is_alarm_time_reach(void);

void Fm_Shake_Open(void)
{
#ifdef FEATRUE_SUPPORT_G_SENSOR
    dword shake;
    if((MMI_GSENSOR_SHAKE_OPEN == mmi_g_sensor_state) 
        ||(MMI_GSENSOR_SHAKE_OPEN_IN_IDLE == mmi_g_sensor_state))   //return if sensor has been open.
    {
        return;
    }

    if(is_g_sportsapp_pedometer_bground_flag() == FALSE)
    {
        OEM_GetConfig(CFGI_GSENSOR,&shake,sizeof(shake));
        if(shake & OEMNV_SHAKE_FM_RADIO_MASK)
        {
#ifdef FEATRUE_SUPPORT_G_SENSOR
            mmi_g_sensor_process(G_SENSOR_ENABLE | G_SENSOR_SHAKE);
#endif
            mmi_g_sensor_state = MMI_GSENSOR_SHAKE_OPEN;
        }    
    }
#endif 
}

void Fm_Shake_Close(void)
{
#ifdef FEATRUE_SUPPORT_G_SENSOR
    dword shake;

    if((MMI_GSENSOR_SHAKE_CLOSE == mmi_g_sensor_state) 
        ||(MMI_GSENSOR_SHAKE_CLOSE_IN_IDLE == mmi_g_sensor_state))  //return if sensor has been closed.
    {
        return;
    }
    
    if(is_g_sportsapp_pedometer_bground_flag() == FALSE)
    {
        OEM_GetConfig(CFGI_GSENSOR,&shake,sizeof(shake));
        if((shake & OEMNV_SHAKE_SNOOZE_ALARM_MASK) && (TRUE == is_alarm_time_reach()))
        {
            return;
        }
       // if(shake & OEMNV_SHAKE_FM_RADIO_MASK)
        {
#ifdef FEATRUE_SUPPORT_G_SENSOR
            mmi_g_sensor_process(G_SENSOR_SHAKE_DISABLE);
#endif
            mmi_g_sensor_state = MMI_GSENSOR_SHAKE_CLOSE;
        }            
    }
#endif
}

static void FmRadio_PowerDown( CFmRadio *pMe)
{
    boolean b_FMBackground = FALSE;
 #if FEATURE_FMRADIO_SUPPORT_BACKGROUND     
    ICONFIG_SetItem( pMe->m_pConfig, CFGI_FM_BACKGROUND, &b_FMBackground, sizeof(b_FMBackground));
#endif 
    IANNUNCIATOR_SetField( pMe->m_pIAnn, ANNUN_FIELD_FMRADIO, ANNUN_STATE_FMRADIO_OFF/*ANNUN_STATE_OFF*/);
    g_m_fm_is_on = FALSE;
#ifndef WIN32
	if (HS_HEADSET_ON())
	{
		fm_mute(TRUE);
	}
    fm_radio_power_down();
#endif//WIN32
    Fm_Shake_Close(); 
    Fm_ResumeKeyBeepVol(pMe);

}

static void FmRadio_PowerUp(void *pme)
{

    CFmRadio *pMe = (CFmRadio *)pme;

    if( pMe->alarmNow ||
        (GETTIMESECONDS() - pMe->wmsEventTimestamp) < pMe->wmsEventTimer
    )
    {
        return;
    }

    pMe->newSmsIncoming = FALSE;

    FmRadio_CheckRefuse( pMe);

	DBGPRINTF("***zzg FmRadio_PowerUp pMe->refuseReason=%d***", pMe->refuseReason);
	
    if( pMe->refuseReason == FM_RADIO_REFUSE_REASON_VOICE_CALL_CONNECTED)
    {
#ifndef WIN32
        if( fm_radio_is_power_up())
        {
            FmRadio_PowerDown( pMe);
        }
#endif//WIN32
        return;
    }

#if !defined( AEE_SIMULATOR)
	DBGPRINTF("***zzg FmRadio_PowerUp fm_radio_is_inited()=%d***", fm_radio_is_inited());
    if( !fm_radio_is_inited())
    {
        fm_radio_init();
        ISHELL_SetTimer( pMe->m_pShell, 1000, FmRadio_PowerUp, pMe);
        return;
    }
    g_m_fm_is_on = TRUE;

	DBGPRINTF("***zzg FmRadio_fm_radio_is_power_up()=%d***", fm_radio_is_power_up());
    if( !fm_radio_is_power_up())
    {
        boolean b_FMBackground = TRUE;
#if FEATURE_FMRADIO_SUPPORT_BACKGROUND          
        ICONFIG_SetItem(pMe->m_pConfig, CFGI_FM_BACKGROUND, &b_FMBackground, sizeof(b_FMBackground));
#endif
        IANNUNCIATOR_SetField (pMe->m_pIAnn, ANNUN_FIELD_FMRADIO, ANNUN_STATE_FMRADIO_ON/*ANNUN_STATE_ON*/);
        fm_radio_power_up();
        FM_CloseKeyBeepVol(pMe);
        Fm_Shake_Open();

        if( fm_tune_channel((LOWEST_BAND_FREQ + CHANNEL_SPACE * pMe->cfg.channel)/100) == FM_RADIO_SUCCESSFUL)
        {
            pMe->ledLightType = FM_RADIO_LED_LIGHT_PLAYING;
        }
        else
        {
            pMe->ledLightType = FM_RADIO_LED_LIGHT_IDLE;
        }
        if (HS_HEADSET_ON())
        {
            fm_set_volume( pMe->byVolumeLevel);
        }
    }
#endif//#if !defined( AEE_SIMULATOR)
}
boolean get_fm_play_state(void)
{
    //Note:if g_m_fm_is_on = 0 ,FM not use or in background;
    return g_m_fm_is_on;
}

#if FEATURE_FMRADIO_CHANNEL_LIST_SUPPORT
/*==============================================================================
����:
       FmRadio_AddChannel
˵��:
       ��ָ����CHANNEL����Channel List, ��������ָ��
       Node�ĺ���.

����:
       ppNode [IO]: ָ��sChanListNode�ṹָ���ָ��.
       pChInfo [In]: ָ��sChanInfo�ṹ��ָ��.

����ֵ:
       TRUE: �ɹ�
       FALSE: ʧ��

��ע:

==============================================================================*/
boolean FmRadio_AddChanListNode( sChanInfo* pChInfo)
{
	if( pChInfo != NULL &&
	    gFmRadio.byChannelMax < MAX_FMRADIO_STORED_CHANNEL &&
	    FmRadio_IsChannelValid( pChInfo->wChannel)
	)
	{

		gFmRadio.chanInfoList[gFmRadio.byChannelMax] = *pChInfo;
		gFmRadio.byChannelMax ++;
		return TRUE;
	}

    return FALSE;
}

int FmRadio_GetChannelTotal()
{
    return gFmRadio.byChannelMax;
}
/*==============================================================================
����:
       FmRadio_DeleteChanListNode
˵��:
       ��ָ����CHANNEL��Channel Listɾ��

����:
       pNode [In]: ָ��sChanListNode�ṹ��ָ��.

����ֵ:
       TRUE: �ɹ�
       FALSE: ʧ��

��ע:

==============================================================================*/
boolean FmRadio_ChanList_DeleteByIndex( CFmRadio* pMe, int index)
{

	if( pMe->byChannelMax > 0 && index >= 0 && index < pMe->byChannelMax)
	{

		int i = 0;
		int j = pMe->byChannelMax - 1;
		for( i = index; i < j; i ++)
		{
			pMe->chanInfoList[i] = pMe->chanInfoList[i + 1];
		}
		pMe->chanInfoList[pMe->byChannelMax - 1].wChannel = 0xffff;
		pMe->byChannelMax --;

		return TRUE;
	}

	return FALSE;
}


sChanInfo* FmRadio_ChanList_GetByIndex( CFmRadio* pMe, int index)
{

	if( index >= 0 && index < pMe->byChannelMax)
	{
		return &pMe->chanInfoList[index];
	}
	else
	{
		return NULL;
	}
}


void FmRadio_SaveChannelList( CFmRadio* pMe)
{

	byte byMax = pMe->byChannelMax;

	if( pMe->byChannelMax > MAX_FMRADIO_STORED_CHANNEL)
	{
		byMax = MAX_FMRADIO_STORED_CHANNEL;
	}

	(void) ICONFIG_SetItem(pMe->m_pConfig,
						   CFGI_FMRADIO_CHAN_TOTAL,
						   &byMax,
						   sizeof(byte));
	(void) ICONFIG_SetItem(pMe->m_pConfig,
						   CFGI_FMRADIO_CHAN_INFO,
						   (void*) pMe->chanInfoList,
						   sizeof(pMe->chanInfoList));
}

void FmRadio_ClearChanList( CFmRadio* pMe)
{
	int i = 0;
	sChanInfo info = { {0}, 0xffff};

	for( i = 0; i < MAX_FMRADIO_STORED_CHANNEL; i ++)
	{
		pMe->chanInfoList[i] = info;
	}
	pMe->byChannelMax = 0;
}


boolean FmRadio_IsChannelValid( uint16 channel)
{

	return channel <= FM_MAX_CHANNEL;
}

void FmRadio_ChanList_EnumInit( CFmRadio* pMe)
{

	pMe->enumCursor = -1;
}

void FmRadio_ChanList_EnumInit_WithLoop( CFmRadio* pMe)
{

	pMe->enumCursorWithLoop = -1;
}

boolean FmRadio_ChanList_EnumNext( CFmRadio* pMe)
{

	if( pMe->byChannelMax > 0)
	{
		pMe->enumCursor ++;
		if( pMe->enumCursor >= pMe->byChannelMax)
		{
			return FALSE;
		}
		return FmRadio_IsChannelValid( pMe->chanInfoList[pMe->enumCursor].wChannel);
	}
	else
	{
		return FALSE;
	}
}

boolean FmRadio_ChanList_EnumPrevious_WithLoop( CFmRadio* pMe)
{

	if( pMe->byChannelMax > 0)
	{
		pMe->enumCursorWithLoop --;
		if( pMe->enumCursorWithLoop < 0)
		{
			pMe->enumCursorWithLoop = pMe->byChannelMax - 1;
		}
		return FmRadio_IsChannelValid( pMe->chanInfoList[pMe->enumCursorWithLoop].wChannel);
	}
	else
	{
		return FALSE;
	}
}

boolean FmRadio_ChanList_EnumNext_WithLoop( CFmRadio* pMe)
{

	if( pMe->byChannelMax > 0)
	{
		pMe->enumCursorWithLoop ++;
		if( pMe->enumCursorWithLoop >= pMe->byChannelMax)
		{
			pMe->enumCursorWithLoop = 0;
		}
		return FmRadio_IsChannelValid( pMe->chanInfoList[pMe->enumCursorWithLoop].wChannel);
	}
	else
	{
		return FALSE;
	}
}

sChanInfo* FmRadio_ChanList_GetCurrent( CFmRadio* pMe)
{

	return FmRadio_ChanList_GetByIndex( pMe, pMe->enumCursor);
}

sChanInfo* FmRadio_ChanList_GetCurrent_WithLoop( CFmRadio* pMe)
{

	return FmRadio_ChanList_GetByIndex( pMe, pMe->enumCursorWithLoop);
}


boolean FmRadio_FindChanListNode(int channel)
{

	int i = 0;

	for( i = 0; i < MAX_FMRADIO_STORED_CHANNEL; i++)
	{
		if( channel == gFmRadio.chanInfoList[i].wChannel)
		{
			return TRUE;
		}
	}


    return FALSE;

}
#endif
/*==============================================================================
����:
       closeApp

˵��:
       �ر�App
����:
       pMe [in]��

����ֵ:

��ע:

==============================================================================*/
// bw:<< 20080808 : close app when pull out the headset
void closeApp( void *pme) //bw:
{
    CFmRadio *pMe = pme;   
#if FEATURE_FMRADIO_SUPPORT_BACKGROUND     
    pMe->runOnBackground = FALSE;
#endif
    ISHELL_CloseApplet( pMe->m_pShell, FALSE);
}
// bw:>> 20080808 : close app when pull out the headset

/*==============================================================================
����:
       FmRadio_APPIsReadyTimer

˵��:
       ��ʱ���ص���������Ҫ���ڿ��ƿ��ٰ�������Ĵ���

����:
       pme [in]��void *����ָ�룬��ָCFmRadio�ṹָ�롣

����ֵ:
       �ޡ�

��ע:

==============================================================================*/
static void FmRadio_AppIsReadyCB(void *pUser)
{
    CFmRadio *pMe = (CFmRadio *)pUser;

    if (NULL == pMe)
    {
        return;
    }
    
    pMe->m_bAppIsReady = TRUE;
}// FmRadio_AppIsReadyCB
