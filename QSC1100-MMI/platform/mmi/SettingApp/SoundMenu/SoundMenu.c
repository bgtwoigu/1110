/*==============================================================================
// �ļ���
//        SoundMenu.c
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
2007-11-01                  ��ʼ�汾
==============================================================================*/


/*==============================================================================
                                 ���ļ��������ⲿ�ļ�
==============================================================================*/
#include "SoundMenu_priv.h"
#include "OEMHeap.h"
/*==============================================================================
                                 �궨��ͳ���
==============================================================================*/

/*==============================================================================
                                 ���Ͷ���
==============================================================================*/

/*==============================================================================
                                 ��������
==============================================================================*/
static void SoundMenu_FreeRingerList(CSoundMenu *pMe);

static void SoundMenu_APPIsReadyTimer(void *pme);

/*----------------------ģ����غ�������---------------------*/

/*static*/ int  SoundMenuMod_Load(IShell   *pIShell,
    void     *ph,
    IModule **ppMod
);

int  SoundMenuMod_New(int16  nSize,
    IShell   *pIShell,
    void     *ph,
    IModule  **ppMod,
    PFNMODCREATEINST pfnMC,
    PFNFREEMODDATA pfnMF
);

static int  SoundMenuMod_CreateInstance(IModule *po,
    IShell *pIShell,
    AEECLSID ClsId,
    void **ppObj
);

static uint32 SoundMenuMod_AddRef(IModule *po);

static uint32 SoundMenuMod_Release(IModule *po);

static void SoundMenuMod_FreeResources(IModule  *po,
    IHeap  *ph,
    IFileMgr *pfm
);

/*----------------------Applet��غ�������---------------------*/
static int SoundMenu_New(IShell *ps,
    IModule *pIModule,
    ISoundMenu **ppObj
);

static uint32  SoundMenu_AddRef(ISoundMenu *pi);

static uint32  SoundMenu_Release (ISoundMenu *pi);

static boolean SoundMenu_HandleEvent(ISoundMenu *pi,
    AEEEvent eCode,
    uint16  wParam,
    uint32 dwParam
);

static int SoundMenu_InitAppData(CSoundMenu *pMe);

static void SoundMenu_FreeAppData(CSoundMenu *pMe);

static void SoundMenu_RunFSM(CSoundMenu *pMe);

void SoundMenu_UpdateRingerListMenu(CSoundMenu *pMe, IMenuCtl *pMenu);

static void  SoundMenu_SoundList (ISoundMenu *pi);

static void  SoundMenu_Autoanswer(ISoundMenu *pi);

static void  SoundMenu_Ringer_List (ISoundMenu *pi,void*  ringer_id);
#ifdef FEATURE_APP_READER   
extern int Rendering_UpdateEx(void);//wlh 20090409 add
#endif  /*FEATURE_APP_READER*/
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
static SoundMenuMod gSoundMenuMod;

static const VTBL(IModule) gModFuncs =
{
    SoundMenuMod_AddRef,
    SoundMenuMod_Release,
    SoundMenuMod_CreateInstance,
    SoundMenuMod_FreeResources
};

// ֻ����һ��SoundMenuʵ����ÿ�δ���SoundMenu Appletʱ��
// ����ͬһ�ṹָ���BREW�㡣
static CSoundMenu gSoundMenu;

static const VTBL(ISoundMenu) gSoundMenuMethods =
{
    SoundMenu_AddRef,
    SoundMenu_Release,
    SoundMenu_HandleEvent,
    SoundMenu_SoundList,
    SoundMenu_Autoanswer,
    SoundMenu_Ringer_List
};

#if defined(BREW_STATIC_APP)
static const AEEAppInfo gaiSoundMenu =
{
    AEECLSID_APP_SOUNDMENU,
    AEE_APPSSOUNDMENU_RES_FILE,
    IDS_SOUNDMENU_TITLE,
    0,
    0,
    0,
    0,
    AFLAG_HIDDEN
};
#endif

/*----------------------ģ����غ�������---------------------*/

/*==============================================================================
����:
       SoundMenuMod_Load

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
/*static*/ int SoundMenuMod_Load(IShell   *pIShell,
                                                   void     *ph,
                                                   IModule **ppMod)
#endif
#else
/*static*/ int SoundMenuMod_Load(IShell   *pIShell,
                                                   void     *ph,
                                                   IModule **ppMod)
#endif//WIn32
{
    // ���ڶ�̬Applet����֤AEE STD��汾����ȷ�����뽨����ģ��ʱʹ�õĿ�汾ƥ��
    // ����AEE_SIMULATOR�²��ؽ��б���顣
#if !defined(BREW_STATIC_APP) && !defined(AEE_SIMULATOR)
    if (GET_HELPER_VER() != AEESTDLIB_VER)
    {
        return EVERSIONNOTSUPPORT;
    }
#endif

    return SoundMenuMod_New(sizeof(SoundMenuMod),
                    pIShell,
                    ph,
                    ppMod,
                    NULL,
                    NULL);
}

/*==============================================================================
����:
       SoundMenuMod_New

˵��:
       ������ʼ��SoundMenuģ�������Ҳ�������ü����������ֺ�����

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
int  SoundMenuMod_New(int16  nSize,
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
    MEMSET(&gSoundMenuMod,0,sizeof(SoundMenuMod));

    INIT_VTBL(&gSoundMenuMod,IModule,gModFuncs);
    gSoundMenuMod.m_nRefs = 1;
    *ppMod = (IModule *)&gSoundMenuMod;
    return AEE_SUCCESS;
}

/*==============================================================================
����:
       SoundMenuMod_AddRef

˵��:
       SoundMenu ģ�����ü�����1�������ص�ǰ����������

����:
       po [in]��IModule�ӿڶ���ָ�롣

����ֵ:
       ��ǰ��SoundMenu ģ���������

��ע:

==============================================================================*/
static uint32 SoundMenuMod_AddRef(IModule *po)
{
    return(++((SoundMenuMod *)po)->m_nRefs);
}

/*==============================================================================
����:
       SoundMenuMod_Release

˵��:
       SoundMenu ģ�����ü�����1�������ص�ǰ����������

����:
       po [in]��IModule�ӿڶ���ָ�롣

����ֵ:
       ��ǰ��SoundMenu ģ���������

��ע:

==============================================================================*/
static uint32 SoundMenuMod_Release(IModule *po)
{
    if (((SoundMenuMod *) po)->m_nRefs == 0)
    {
        return 0;
    }

    return(--((SoundMenuMod *)po)->m_nRefs);
}

/*==============================================================================
����:
       SoundMenuMod_CreateInstance

˵��:
        ͨ������(��ʼ��)SoundMenu����������ģ��ʵ����

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
static int  SoundMenuMod_CreateInstance(IModule *po,
    IShell *pIShell,
    AEECLSID ClsId,
    void **ppObj
)
{
    *ppObj = NULL;

    if (ClsId != AEECLSID_APP_SOUNDMENU)
    {
        return EFAILED;
    }

    if (SoundMenu_New(pIShell, po, (ISoundMenu**)ppObj) != SUCCESS)
    {
        return EFAILED;
    }
    return SUCCESS;
}

/*==============================================================================
����:
       SoundMenuMod_FreeResources

˵��:
        �ͷ�SoundMenuģ����Դ�ĺ�����

����:
       po ��IModule��
       ph ��IHeap�ӿ�ָ�롣
       pfm :IFileMgr�ӿ�ָ�롣

����ֵ:
       ��

��ע:
       �����û�ȥ���á�

==============================================================================*/
static void SoundMenuMod_FreeResources(IModule  *po,
    IHeap  *ph,
    IFileMgr *pfm
)
{

}

/*----------------------Applet��غ�������---------------------*/
/*==============================================================================
����:
       SoundMenu_New

˵��:
        ��ʼ��SoundMenu Applet�������ڲ����ݱ�����ͬʱ�����ü�����1��

����:
       ps [in]��IShell�ӿڶ���ָ�롣
       pIModule [in]��pIModule�ӿڶ���ָ�롣
       ppObj [out]:��ָ��ISoundMenu����ָ���ָ�롣

����ֵ:
       EFAILED��ʧ�ܡ�
       SUCCESS���ɹ���

��ע:
       �����û�ȥ���á�
       ����Applet�ṹ�����ʹ�ö�̬�����ڴ�ķ�ʽ�������������õ���ʹ�þ�̬
       �����ķ�ʽ������Խṹ�Ƚϴ��Appletʹ�þ�̬�����ķ�ʽ�����ڶ�̬����
       �ڴ�ķ�ʽ��Ҫע���������ռ���ͷš�

==============================================================================*/
static int SoundMenu_New(IShell *ps,
    IModule *pIModule,
    ISoundMenu **ppObj
)
{
    int retVal = SUCCESS;

    if (0 == gSoundMenu.m_nRefs)
    {
        // Must initialize the object
        MEMSET(&gSoundMenu,  0, sizeof(CSoundMenu));
        INIT_VTBL(&gSoundMenu, ISoundMenu, gSoundMenuMethods);

        gSoundMenu.m_nRefs     = 0;
        gSoundMenu.m_pShell    = ps;
        gSoundMenu.m_pModule   = pIModule;

        (void) ISHELL_AddRef(ps);
        (void) IMODULE_AddRef(pIModule);

        retVal = SoundMenu_InitAppData(&gSoundMenu);
    }

    ++gSoundMenu.m_nRefs;
    *ppObj = (ISoundMenu*)&gSoundMenu;
    return retVal;
}

/*==============================================================================
����:
       SoundMenu_AddRef

˵��:
       SoundMenu Applet ���ü�����1�������ص�ǰ����������

����:
       pi [in]��ISoundMenu�ӿڶ���ָ�롣

����ֵ:
       ��ǰ��SoundMenu Applet��������

��ע:

==============================================================================*/
static uint32  SoundMenu_AddRef(ISoundMenu *pi)
{
    register CSoundMenu *pMe = (CSoundMenu*)pi;

    ASSERT(pMe != NULL);
    ASSERT(pMe->m_nRefs > 0);

    return (++pMe->m_nRefs);
}

/*==============================================================================
����:
       SoundMenu_Release

˵��:
       �ͷŶ�SoundMenu Applet�����ã����ü�����1�������ص�ǰ����������
        �����ǰ��������Ϊ�㣬�����ͷ�Applet�������Դ��

����:
       pi [in]��ISoundMenu�ӿڶ���ָ�롣

����ֵ:
       ��ǰ��SoundMenu Applet��������

��ע:

==============================================================================*/
static uint32  SoundMenu_Release (ISoundMenu *pi)
{
    register CSoundMenu *pMe = (CSoundMenu*)pi;

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
    SoundMenu_FreeAppData(pMe);

    (void) ISHELL_Release(pMe->m_pShell);
    (void) IMODULE_Release(pMe->m_pModule);

    // ע�⣺pMe�Ǿ�̬����ռ䣬��˲���Ҫ�ͷš�FREE()
    return 0;
}

/*==============================================================================
SoundMenu_SoundList

˵����
      ���ò˵����������˵��г��ĵ�ǰ������龰ģʽ��

������
       pi [in/out]��ISoundMenu �ӿڶ���ָ�롣

����ֵ��
       ��ǰ��Setting Applet��������

��ע��:

==============================================================================*/
static void  SoundMenu_SoundList (ISoundMenu *pi)
{
   register CSoundMenu *pMe = (CSoundMenu*)pi;
   char     *args;

   args = MALLOC(4);
   *args = STARTARG_SOUNDMENU;

   pMe->m_eCurState = SOUNDMENUST_SOUNDMENU;
   (void)ISHELL_StartAppletArgs(pMe->m_pShell,
                                AEECLSID_APP_SOUNDMENU,
                                args);
   //(void) ISHELL_PostEvent(pMe->m_pShell,
   //                        AEECLSID_APP_SOUNDMENU,
   //                        EVT_USER_CLOSEAPP,
   //                        0,
   //                        0);
   if(args)
   {
      FREE(args);
   }
}

/*==============================================================================
SoundMenu_Autoanswer

˵����
      ���ò˵����������˵��г��ĵ�ǰ������龰ģʽ��

������
       pi [in/out]��ISoundMenu �ӿڶ���ָ�롣

����ֵ��
       ��ǰ��Setting Applet��������

��ע��:

==============================================================================*/
static void  SoundMenu_Autoanswer (ISoundMenu *pi)
{
#if 0
   register CSoundMenu *pMe = (CSoundMenu*)pi;
   char     *args;

   args = MALLOC(4);
   *args = STARTARG_AUTOANSWERSUB;

   pMe->m_eCurState = SOUNDMENUST_AUTOANSWERSUB;
   (void)ISHELL_StartAppletArgs(pMe->m_pShell,
                                AEECLSID_APP_SOUNDMENU,
                                args);
   //(void) ISHELL_PostEvent(pMe->m_pShell,
   //                        AEECLSID_APP_SOUNDMENU,
   //                        EVT_USER_CLOSEAPP,
   //                        0,
   //                        0);
   if(args)
   {
      FREE(args);
   }
#endif
}



/*==============================================================================
����:
       SoundMenu_InitAppData

˵��:
       ��ʼ��SoundMenu Applet�ṹ����ȱʡֵ��

����:
       pMe [in]��ָ��SoundMenu Applet����ṹ��ָ�롣�ýṹ����С������ض���Ϣ��

����ֵ:
       SUCCESS����ʼ���ɹ���
       EFAILED����ʼ��ʧ�ܡ�

��ע:

==============================================================================*/
static int SoundMenu_InitAppData(CSoundMenu *pMe)
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
    pMe->m_ePreState = SOUNDMENUST_NONE;
    pMe->m_eCurState = SOUNDMENUST_INIT;
    pMe->m_pActiveDlg = NULL;
    pMe->m_eDlgRet = DLGRET_CREATE;
    pMe->m_bNotOverwriteDlgRet = FALSE;

    pMe->m_currDlgId = 0;
    pMe->m_nSubDlgId = 0;
    pMe->m_sSubDlgId = 0;
    pMe->m_fSubDlgId = 0;

    pMe->m_lastRingerPlayed = NUM_OF_MAXRINGER;
#if defined(FEATURE_WMS_APP) && !defined(FEATURE_WMSAPP_ONLYSUPPORTVMAIL)
    pMe->m_lastSmsRingPlayed = 78;      //78�����������id,��sndid.h�ж���
#endif

    pMe->m_ProfileType = SET_NORMALMODE;
    pMe->m_VolType = SET_RINGER_VOL;
    //pMe->m_OtherVolType = SET_FLIP;
    pMe->m_RingerType = SET_RINGER;
    pMe->m_HintType = SET_CALLHINT ;
    pMe->m_ScenemodesubType = SET_ACTIVATE;
    pMe->m_bIsSuspend = FALSE;
    pMe->m_CurProfile = OEMNV_PROFILE_NORMALMODE;
    pMe->m_bNaturalStart = NORMAL_ENTRY;

    if (ISHELL_CreateInstance(pMe->m_pShell,
                                 AEECLSID_ANNUNCIATOR,
                                 (void **) &pMe->m_pIAnn))
    {
        SoundMenu_FreeAppData(pMe);
        return FALSE;
    }

    if (AEE_SUCCESS != ISHELL_CreateInstance(pMe->m_pShell,
                                            AEECLSID_CONFIG,
                                            (void **)&pMe->m_pConfig))
    {
        SoundMenu_FreeAppData(pMe);
        return EFAILED;
    }

    if (AEE_SUCCESS != ISHELL_CreateInstance(pMe->m_pShell,
                                            AEECLSID_RINGERMGR,
                                            (void **)&pMe->m_pRingerMgr))
    {
        SoundMenu_FreeAppData(pMe);
        return EFAILED;
    }

    if (AEE_SUCCESS != ISHELL_CreateInstance(pMe->m_pShell,
                                            AEECLSID_SOUND,
                                            (void **)(&pMe->m_pISound)))
    {
        SoundMenu_FreeAppData(pMe);
        return EFAILED;
    }

    if (AEE_SUCCESS != ISHELL_CreateInstance(pMe->m_pShell,
                                            AEECLSID_ALERT,
                                            (void **)(&pMe->m_pAlert)))
    {
        SoundMenu_FreeAppData(pMe);
        return EFAILED;
    }

#ifdef FLIPSND_ZY
    if (AEE_SUCCESS != ISHELL_CreateInstance(pMe->m_pShell,
                                            AEECLSID_SOUNDPLAYER,
                                            (void **)(&pMe->m_SndPlayer)))
    {
        SoundMenu_FreeAppData(pMe);
        return EFAILED;
    }
#endif /* FLIPSND_ZY */

    if (ISHELL_CreateInstance(pMe->m_pShell, AEECLSID_VECTOR/*AEECLSID_IVECTOR*/,
            (void**)&pMe->m_pRingerList) != SUCCESS )
    {
        SoundMenu_FreeAppData(pMe);
        return EFAILED;
    }
    else
    {
        (void) IVector_EnsureCapacity(pMe->m_pRingerList, 100);
        pMe->m_ringer_be = FALSE ;
        //SoundMenu_InitRingerList(pMe);
    }
    //if(ISHELL_RegisterNotify(pMe->m_pShell, AEECLSID_APP_SOUNDMENU,
    //    AEECLSID_ALERT_NOTIFIER, NMASK_ALERT_ONOFF | NMASK_ALERT_MUTED) != SUCCESS)
    //{
    //    ASSERT_NOT_REACHABLE;
    //}
    return SUCCESS;
}

/*==============================================================================
����:
       SoundMenu_FreeAppData

˵��:
       �ͷ�SoundMenu Appletʹ�õ������Դ��

����:
       pMe [in]��ָ��SoundMenu Applet����ṹ��ָ�롣�ýṹ����С������ض���Ϣ��

����ֵ:
       �ޡ�

��ע:

==============================================================================*/
static void SoundMenu_FreeAppData(CSoundMenu *pMe)
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

    if (pMe->m_pRingerMgr)
    {
        IRINGERMGR_Release(pMe->m_pRingerMgr);
        pMe->m_pRingerMgr = NULL;
    }

    if (pMe->m_pAlert != NULL)
    {
        IALERT_StopRingerAlert(pMe->m_pAlert);
        IALERT_Release(pMe->m_pAlert);
        pMe->m_pAlert = NULL;
    }

    if (pMe->m_pISound != NULL)
    {
        ISOUND_Release(pMe->m_pISound);
        pMe->m_pISound = NULL;
    }

#ifdef FLIPSND_ZY
    if (pMe->m_SndPlayer != NULL)
    {
        ISOUNDPLAYER_Stop(pMe->m_SndPlayer);
        ISOUNDPLAYER_Release(pMe->m_SndPlayer);
        pMe->m_SndPlayer = NULL;
    }
#endif /* FLIPSND_ZY */

    //�ͷ�����ӿں������������
    if (NULL != pMe->m_pRingerList)
    {
        SoundMenu_FreeRingerList(pMe);
        (void) IVector_Release(pMe->m_pRingerList);
        pMe->m_pRingerList=NULL;
    }

    if (pMe->m_pIAnn)
    {
        IANNUNCIATOR_Release(pMe->m_pIAnn);
        pMe->m_pIAnn= NULL;
    }
}

/*==============================================================================
����:
       SoundMenu_RunFSM

˵��:
       SoundMenu Applet����״̬�����档

����:
       pMe [in]��ָ��SoundMenu Applet����ṹ��ָ�롣�ýṹ����С������ض���Ϣ��

����ֵ:
       �ޡ�

��ע:

==============================================================================*/
static void SoundMenu_RunFSM(CSoundMenu *pMe)
{
    NextFSMAction nextFSMAction = NFSMACTION_WAIT;

    for ( ; ; )
    {
        nextFSMAction = SoundMenu_ProcessState(pMe);

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
       SoundMenu_HandleEvent

˵��:
       SoundMenu  Applet�¼��������������������ڱ�Applet���¼���
       �������ɸú�������

����:
       pi [in]��ָ��ISoundMenu�ӿ�t�����ָ�롣
       eCode��BREW�¼����롣
       wParam�����¼�����������ݡ�
       dwParam [in]�����¼�����������ݡ�

����ֵ:
       TRUE�������¼�������
       FALSE�������¼�û������

��ע:

==============================================================================*/
static boolean SoundMenu_HandleEvent(ISoundMenu *pi,
    AEEEvent eCode,
    uint16  wParam,
    uint32 dwParam
)
{
    CSoundMenu *pMe = (CSoundMenu*)pi;
    AEEAppStart *as;
    //SOUND_ERR("%x,%x,%x,SoundMenu_HandleEvent",eCode,wParam,dwParam);
    switch (eCode)
    {
        case EVT_APP_START:
#ifdef FEATURE_APP_READER               
			Rendering_UpdateEx();//wlh add for 3D test
#endif  /*FEATURE_APP_READER*/
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
            {
                AEEDeviceInfo di; 
                ISHELL_GetDeviceInfo(pMe->m_pShell,&di);
                pMe->m_rc.dy = di.cyScreen;
            }
            //end added
            pMe->m_bSuspending = FALSE;
             (void) ICONFIG_GetItem(pMe->m_pConfig,
                              CFGI_PROFILE_CUR_NUMBER,
                              &pMe->m_active,
                              sizeof(pMe->m_active));
             pMe->m_CurProfile = pMe->m_active;
            // ��ʼSOUNDMENUST_SOUNDMENU״̬��
#ifndef WIN32
            if(STRNCMP(as->pszArgs,"R",1) == 0)
            {
                char temp[16] = {0};
                STRLCPY(temp,as->pszArgs +1,sizeof(temp));
                pMe->m_enter_address = (AECHAR*)ATOI(temp);
                SOUND_ERR("%S %x SoundMenu_Ringer_List2",pMe->m_enter_address,&pMe->m_enter_address,0);
                pMe->m_bNaturalStart = ENTER_RINGER_LIST;
                pMe->m_RingerType =  SET_ANOTHER;
                {
                    if((pMe->m_enter_address[0] == (AECHAR)'\0'))
                    {
                        pMe->m_lastRingerPlayed = DEFAULT_MENU;
                    }
                    else
                    {
                        IRingerMgr      *RingerMgr = NULL;
                        int ringerId = AEE_RINGER_ID_NONE;
                        char        filename[MAX_FILE_NAME]={0};
                        
                        if (AEE_SUCCESS != ISHELL_CreateInstance(pMe->m_pShell,
                                                            AEECLSID_RINGERMGR,
                                                            (void **)&RingerMgr))
                        {
                            return FALSE;
                        }
                        
                        WSTRTOSTR(pMe->m_enter_address, filename, MAX_FILE_NAME);
                        
                        // Ϊ��֧��PEK ���ԣ��绰����RINGTONE �ֶ�ֻ�ܱ����ַ������ͣ��������·���������Ƿ���MID����
                        ringerId = IRINGERMGR_GetRingerID(RingerMgr, filename);
                        if(AEE_RINGER_ID_NONE == ringerId)
                        {
                            // ��mp3
                            pMe->m_lastRingerPlayed = DOWNLOAD_MENU;
                        }
                        else
                        {
                            // mid
                            pMe->m_lastRingerPlayed = ringerId;
                        }
                    
                    }

                }
                
                SOUND_ERR("tagnwq soundhandle first:%d",pMe->m_lastRingerPlayed,0,0);
                //pMe->m_lastRingerPlayed = (uint16) (pMe->m_enter_addr)->midID;
                MOVE_TO_STATE(SOUNDMENUST_RINGER);
            }

            if(*as->pszArgs == STARTARG_SOUNDMENU)
            {
                pMe->m_bNaturalStart = SOUNDLIST_ENTRY;
                MOVE_TO_STATE(SOUNDMENUST_SOUNDMENU);
            }
#endif//WIN32
            //��ʼSOUNDMENUST_AUTOANSWERSUB״̬��
            //if(*as->pszArgs == STARTARG_AUTOANSWERSUB)
            //{
            //   pMe->m_bNaturalStart = AUTOANSWER_ENTRY;
            //   MOVE_TO_STATE(SOUNDMENUST_AUTOANSWERSUB);
            //}

            SoundMenu_RunFSM(pMe);
            return TRUE;

        case EVT_APP_STOP:
            pMe->m_bNaturalStart = NORMAL_ENTRY;
            if (pMe->m_pDisplay != NULL)
            {
                (void) IDISPLAY_Release(pMe->m_pDisplay);
                pMe->m_pDisplay = NULL;
            }
            pMe->m_bSuspending = TRUE;
            ISHELL_CancelTimer(pMe->m_pShell, NULL, pMe);
            ISHELL_RegisterNotify(pMe->m_pShell, AEECLSID_APP_SOUNDMENU,AEECLSID_ALERT_NOTIFIER, 0);
            return TRUE;

        case EVT_APP_SUSPEND:
            pMe->m_bSuspending = TRUE;
            if(pMe->m_pActiveDlgID == IDD_RINGER_MENU)
            {
                //�������󷵻�ʱ�Ľ�������
                pMe->m_bIsSuspend = TRUE;
            }
            ISHELL_CancelTimer(pMe->m_pShell, NULL, pMe);
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
            pMe->m_rc.y = pMe->m_rc.y - SOUND_MENU_SOFK_HIGHT;
            SoundMenu_FreeRingerList(pMe);
            pMe->m_eMakeListMode = MAKELIST_INIT;
            SoundMenu_InitRingerList(pMe);
            SoundMenu_RunFSM(pMe);
            return TRUE;

        case EVT_NOTIFY:
        {
            AEENotify   *Notify_msg = (AEENotify *) dwParam;
            if(Notify_msg->cls == AEECLSID_ALERT_NOTIFIER)
            {
                switch(wParam)
                {
                    case NMASK_ALERT_ONOFF:
                        return TRUE;

                    case NMASK_ALERT_MUTED:
                        return TRUE;

                    default:
                        return FALSE;
                }
            }

        }

        case EVT_DIALOG_INIT:
            pMe->m_bAppIsReady = FALSE;
            pMe->m_pActiveDlg = (IDialog*)dwParam;
            pMe->m_pActiveDlgID = wParam;

            return SoundMenu_RouteDialogEvent(pMe,eCode,wParam,dwParam);

        case EVT_DIALOG_START:
#ifdef FEATURE_APP_READER               
			Rendering_UpdateEx();//wlh add for 3D test
#endif  /*FEATURE_APP_READER*/			
            return SoundMenu_RouteDialogEvent(pMe,eCode,wParam,dwParam);

        case EVT_USER_REDRAW:
            (void) SoundMenu_RouteDialogEvent(pMe,eCode,wParam,dwParam);
            (void)ISHELL_SetTimer ( pMe->m_pShell,
                            APPISREADY_TIMER,
                            SoundMenu_APPIsReadyTimer,
                            pMe);
            return TRUE;

        case EVT_APPISREADY:
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
            return SoundMenu_RouteDialogEvent(pMe,eCode,wParam,dwParam);

        case EVT_DIALOG_END:
            if (wParam == 0)
            {
                return TRUE;
            }
            pMe->m_bAppIsReady = FALSE;
            (void) SoundMenu_RouteDialogEvent(pMe,eCode,wParam,dwParam);
            pMe->m_pActiveDlg = NULL;

            // Applet�����𣬲���״̬�����ȴ�Applet���ء���ע�⣺EVT_APP_SUSPEND
            // �¼���EVT_DIALOG_END�¼�ǰ������
            if (pMe->m_bSuspending == FALSE)
            {
                // ��ʼSoundMenu״̬��
                SoundMenu_RunFSM(pMe);
            }
            return TRUE;
#ifdef FEATURE_LCD_TOUCH_ENABLE//wlh add for LCD touch
		case EVT_PEN_UP:
		//case EVT_PEN_DOWN:
			{
				return SoundMenu_RouteDialogEvent(pMe,eCode,wParam,dwParam);
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
				return SoundMenu_RouteDialogEvent(pMe,eCode,wParam,dwParam);
			}
#endif//FEATURE_LCD_TOUCH_ENABLE
        default:
            // �����յ����¼�·������ǰ��ĶԻ����¼���������
            return SoundMenu_RouteDialogEvent(pMe,eCode,wParam,dwParam);
    }
}

/*==============================================================================
����:
       SoundMenu_APPIsReadyTimer

˵��:
       ��ʱ���ص���������Ҫ���ڿ��ƿ��ٰ�������Ĵ���

����:
       pme [in]��void *����ָ�룬��ָCSoundMenu�ṹָ�롣

����ֵ:
       �ޡ�

��ע:

==============================================================================*/
static void SoundMenu_APPIsReadyTimer(void *pme)
{
    CSoundMenu *pMe = (CSoundMenu *)pme;

    if (NULL == pMe)
    {
        return;
    }

    (void) ISHELL_PostEvent( pMe->m_pShell,
                            AEECLSID_APP_SOUNDMENU,
                            EVT_APPISREADY,
                            0,
                            0);
}

/*==============================================================================
������
       SoundMenu_InitRingerList

˵����
       �������ڳ�ʼ��������

������
       pMe [in]��ָ��SoundMenu Applet����ṹ��ָ�롣
                           �ýṹ����С������ض���Ϣ��

����ֵ��
       �ޡ�

��ע��:

==============================================================================*/
void SoundMenu_InitRingerList(CSoundMenu *pMe)
{
    RingerMenuItemInfo   *pItemInfo;
    AEERingerInfo  ri;

    SOUND_ERR("Init ringer list begin!",0,0,0);
    if (pMe == NULL)
    {
        return;
    }
    if(pMe->m_ringer_be)
    {
        return;
    }

    IVector_RemoveAllElements(pMe->m_pRingerList);
#if !defined( FEATURE_RECORDER)
    pMe->m_ringer_be = TRUE ;
#endif

    if ( IRINGERMGR_EnumRingerInit(pMe->m_pRingerMgr) != SUCCESS)
    {
        return;
    }
    while (IRINGERMGR_EnumNextRinger(pMe->m_pRingerMgr, &ri))
    {
        if (ri.id > NUM_OF_MAXRINGER)
        {
            break;  // only support NUM_OF_MAXRINGER-1 ringers
        }
        else
        {
            pItemInfo = NULL;
            pItemInfo = (RingerMenuItemInfo *)sys_malloc(sizeof(RingerMenuItemInfo));
            if (NULL == pItemInfo)
            {
                return;
            }
            else
            {
#if defined( FEATURE_RECORDER)
			   if( STRBEGINS( RECORDER_MEMO_RINGTONE_PATH_NAME_PREFIX, ri.szFile))
			   {
			   	
               if( ( pMe->m_RingerType == SET_RINGER    ||
                     pMe->m_RingerType == SET_ALARMRING ||
                     pMe->m_RingerType == SET_SMSTONE   ||
                     pMe->m_RingerType == SET_ANOTHER
                   )                   
               )
               {
                   if( ( pMe->m_RingerType == SET_RINGER &&
                         STRBEGINS( RECORDER_MEMO_CALL_RINGTONE_PATH_NAME + 4, ri.szFile)
                       )
                       ||
                       ( pMe->m_RingerType == SET_ALARMRING &&
                         STRBEGINS( RECORDER_MEMO_ALARM_RINGTONE_PATH_NAME + 4, ri.szFile)
                       )
                       ||
                       ( pMe->m_RingerType == SET_SMSTONE &&
                         STRBEGINS( RECORDER_MEMO_SMS_RINGTONE_PATH_NAME + 4, ri.szFile)
                       )
                       ||
                       ( pMe->m_RingerType == SET_ANOTHER &&
                         STRBEGINS( RECORDER_MEMO_SMS_RINGTONE_PATH_NAME + 4, ri.szFile)
                       )
                   )
                   {
                       AECHAR memo[64] = {0};

                       if( ISHELL_LoadResString( pMe->m_pShell,
                                             APP_RECORDER_RES_FILE,
                                             IDS_RECORDER_MEMO_RINGER_NAME,
                                             memo,
                                             sizeof( memo)
                                         ) > 0
                       )
                       {
                           (void)WSTRCPY( ri.szName, memo);
                       }
                       else
                       {
                           (void)WSTRCPY( ri.szName, L"Voice memo");
                       }

                   }
                   else
                   {
					   FREE( pItemInfo);
                       continue;
                   }
               }
			   else
			   {
					FREE( pItemInfo);
                    continue;
			   }
			   }
#endif

                pItemInfo->id = ri.id;
                (void)WSTRCPY(pItemInfo->szName,  ri.szName);
                if ( IVector_AddElement(pMe->m_pRingerList, pItemInfo) != SUCCESS)
                {
                    FREE(pItemInfo);
                    return;
                }
            }
        }
    }

    SOUND_ERR("Init ringer list end!",0,0,0);
}

/*==============================================================================
������
       SoundMenu_FreeRingerList

˵����
       ���������ͷŶ�̬Ϊ���������������ڴ档

������
       pMe [in]��ָ��SoundMenu Applet����ṹ��ָ�롣
                           �ýṹ����С������ض���Ϣ��

����ֵ��
       �ޡ�

��ע��:

==============================================================================*/
static void SoundMenu_FreeRingerList(CSoundMenu *pMe)
{
    RingerMenuItemInfo   *pItemInfo = NULL;
    uint32       i, dwCount;

    SOUND_ERR("Free ringer list begin!",0,0,0);
    if (pMe == NULL)
    {
        return;
    }

    if (pMe->m_pRingerList == NULL)
    {
        return;
    }

    dwCount = IVector_Size(pMe->m_pRingerList);

    // free the data first
    for (i = 0; i < dwCount; i++)
    {
        pItemInfo = (RingerMenuItemInfo *)IVector_ElementAt(pMe->m_pRingerList, i);
        if (NULL != pItemInfo)
        {
            sys_free(pItemInfo);
            pItemInfo = NULL;
        }
    }

    IVector_RemoveAllElements(pMe->m_pRingerList);
    pMe->m_ringer_be = FALSE ;
    SOUND_ERR("Free ringer list end!",0,0,0);
}

/*==============================================================================
������
       SoundMenu_UpdateRingerListMenu

˵����
       ���������б�˵���

������
       pMe [in]��ָ��SoundMenu Applet����ṹ��ָ�롣
                           �ýṹ����С������ض���Ϣ��
       pMenu [in/out]��IMenuCtl�ӿ�ָ�롣

����ֵ��
        ��

��ע��

==============================================================================*/
void SoundMenu_UpdateRingerListMenu(CSoundMenu *pMe, IMenuCtl *pMenu)
{
    RingerMenuItemInfo  *pListItem = NULL;
    int16     wItemCount;
    int16    wBeginPos, wEndPos;
    int16    wTotalPages;
    CtlAddItem       mai;
    uint32 dwIndex;

    if ((NULL == pMe) || (NULL == pMenu))
    {
        return;
    }

    if (pMe->m_pRingerList == NULL)
    {
        return;
    }
    // ȡ��������
    wItemCount = IVector_Size(pMe->m_pRingerList);

    // ����"��������"�˵���
    wItemCount++;

    SOUND_ERR("wItemCount = %d",wItemCount,0,0);

    // ������Ҫ����ҳ��
    wTotalPages = wItemCount / MAXITEMS_ONEPAGE;
    if ((wItemCount % MAXITEMS_ONEPAGE) > 1)
    {
        wTotalPages++;
    }
    SOUND_ERR("wTotalPages = %d",wTotalPages,0,0);

    // ��������������һҳ��ʾ�Ĳ˵�����ʱ��������������Ҫ������˵��б��ģʽ
    if (((pMe->m_eMakeListMode == MAKELIST_NEXTPAGE) ||
         (pMe->m_eMakeListMode == MAKELIST_PREPAGE)) &&
        (wTotalPages <= 1))
    {
        int nCount;
        uint16 wItemID;

        nCount = IMENUCTL_GetItemCount(pMenu);

        if (nCount > 1)
        {
            if (pMe->m_eMakeListMode == MAKELIST_PREPAGE)
            {
                if (IMENUCTL_GetItemID(pMenu, 0) !=
                    IMENUCTL_GetSel(pMenu))
                {   // ���ǰ������
                    return;
                }
                wItemID = IMENUCTL_GetItemID(pMenu, nCount-1);
            }
            else
            {
                if (IMENUCTL_GetItemID(pMenu, nCount-1) !=
                    IMENUCTL_GetSel(pMenu))
                {   // ���ǰ��ҷ����
                    return;
                }
                wItemID = IMENUCTL_GetItemID(pMenu, 0);
            }

            // ����ǰѡ������Ϊĩβ�������ƶ��˵����������
            IMENUCTL_SetSel(pMenu, wItemID);

            pMe->m_lastRingerPlayed = wItemID;
            (void)IMENUCTL_Redraw(pMenu);
        }
        return;
    }
    SOUND_ERR("pMe->m_eMakeListMode = %d",pMe->m_eMakeListMode,0,0);
    // ȷ�������˵��б�ʱ����Ӧ��Ϣ�б�����λ��
    switch (pMe->m_eMakeListMode)
    {
        case MAKELIST_INIT:
            // ȷ���趨�������ڵ�ҳ
            {
                uint32   dwCurIndex = IVector_Size(pMe->m_pRingerList);

                wBeginPos = 0;
                wEndPos = MAXITEMS_ONEPAGE - 1;
                pMe->m_wCurPageNum = 0;

                for (dwIndex = 0; dwIndex < dwCurIndex; dwIndex++)
                {
                    pListItem = (RingerMenuItemInfo *)IVector_ElementAt(pMe->m_pRingerList, dwIndex);

                    if (NULL == pListItem)
                    {
                        break;
                    }

                    if (pListItem->id == pMe->m_RingID[pMe->m_CurProfile])
                    {
                        dwCurIndex = dwIndex;

                        pMe->m_wCurPageNum = dwCurIndex / MAXITEMS_ONEPAGE;

                        wBeginPos = pMe->m_wCurPageNum * MAXITEMS_ONEPAGE;
                        wEndPos = wBeginPos + MAXITEMS_ONEPAGE - 1;
                        break;
                    }
                }
            }
            break;

        case MAKELIST_NEXTPAGE:
            if ((pMe->m_wCurPageNum + 1) >= wTotalPages)
            {   // �ѵ��б�ĩβʱҪ�󻻵���һҳ, ������ͷ����ʼ
                wBeginPos = 0;
                wEndPos = MAXITEMS_ONEPAGE - 1;
                pMe->m_wCurPageNum = 0;
            }
            else
            {
                pMe->m_wCurPageNum++;
                wBeginPos = MAXITEMS_ONEPAGE * pMe->m_wCurPageNum;
                wEndPos = wBeginPos + MAXITEMS_ONEPAGE - 1;
            }
            break;

        case MAKELIST_PREPAGE:
            if (pMe->m_wCurPageNum == 0)
            {
                // �������һҳ��ʾ
                pMe->m_wCurPageNum = wTotalPages - 1;
                wBeginPos = MAXITEMS_ONEPAGE * pMe->m_wCurPageNum;
                wEndPos = wBeginPos + MAXITEMS_ONEPAGE - 1;

            }
            else
            {
                pMe->m_wCurPageNum--;
                wBeginPos = MAXITEMS_ONEPAGE * pMe->m_wCurPageNum;
                wEndPos = wBeginPos + MAXITEMS_ONEPAGE - 1;

            }
            break;

        case MAKELIST_RESUMECURPAGE:
            // ��ɾ���������� m_wCurPageNum ȡֵ��������
            if (pMe->m_wCurPageNum >= wTotalPages)
            {
                // ��ʾ���һҳ
                pMe->m_wCurPageNum = wTotalPages - 1;
                wBeginPos = MAXITEMS_ONEPAGE * pMe->m_wCurPageNum;
                wEndPos = wBeginPos + MAXITEMS_ONEPAGE - 1;

            }
            else
            {
                wBeginPos = MAXITEMS_ONEPAGE * pMe->m_wCurPageNum;
                wEndPos = wBeginPos + MAXITEMS_ONEPAGE - 1;
            }
            break;

        default:
            return;
    }

    // �����������Ϣ�б�
    (void)IMENUCTL_DeleteAll(pMenu);

    if(pMe->m_RingerType == SET_ANOTHER)
    {
        IMENUCTL_AddItem(pMenu, AEE_APPSSOUNDMENU_RES_FILE, IDS_DEFAULT, DEFAULT_MENU, NULL, 0); 
        IMENUCTL_AddItem(pMenu, AEE_APPSSOUNDMENU_RES_FILE, IDS_GALLERY_PROFILE, DOWNLOAD_MENU, NULL, 0);
    }

    if(pMe->m_RingerType == SET_ALARMRING || pMe->m_RingerType == SET_RINGER)
    {
        IMENUCTL_AddItem(pMenu, AEE_APPSSOUNDMENU_RES_FILE, IDS_GALLERY_PROFILE, DOWNLOAD_MENU, NULL, 0);
    }

    // �����˵����б�
    MEMSET(&mai, 0, sizeof(mai));
    //mai.pszResImage = AEEEXT_RES_FILE;
    mai.pszResText = NULL;
    mai.pImage = NULL;
    mai.wFont = AEE_FONT_NORMAL;
    mai.dwData = 0;
    for (dwIndex = wBeginPos; dwIndex <= wEndPos; dwIndex++)
    {
        {
            if ((dwIndex + 1) >= wItemCount)
            {   // "��������"�˵���

                if (pListItem->id == pMe->m_RingID[pMe->m_CurProfile])
                {
                    //mai.wImage = IDB_SELECT_ICON;
                    pMe->m_lastRingerPlayed = pListItem->id;
                }
                break;
            }
            else
            {
                pListItem = (RingerMenuItemInfo *)IVector_ElementAt(pMe->m_pRingerList, dwIndex);

                if (NULL != pListItem)
                {
                      //mai.pText = pListItem->szName;
                    mai.wItemID = pListItem->id;
#ifdef FEATURE_LANG_ARABIC
              switch (mai.wItemID)
              {
                case 1:
                   (void) ISHELL_LoadResString(pMe->m_pShell,
                                    AEE_APPSSOUNDMENU_RES_FILE,
                                    IDS_ARABIC_RINGTONE_1,
                                    pListItem->szName,
                                    sizeof(pListItem->szName));
                break;
                case 2:
                   (void) ISHELL_LoadResString(pMe->m_pShell,
                                    AEE_APPSSOUNDMENU_RES_FILE,
                                    IDS_ARABIC_RINGTONE_2,
                                    pListItem->szName,
                                    sizeof(pListItem->szName));
                break;
                   case 3:
                   (void) ISHELL_LoadResString(pMe->m_pShell,
                                    AEE_APPSSOUNDMENU_RES_FILE,
                                    IDS_ARABIC_RINGTONE_3,
                                    pListItem->szName,
                                    sizeof(pListItem->szName));
                break;
                   case 4:
                   (void) ISHELL_LoadResString(pMe->m_pShell,
                                    AEE_APPSSOUNDMENU_RES_FILE,
                                    IDS_ARABIC_RINGTONE_4,
                                    pListItem->szName,
                                    sizeof(pListItem->szName));
                break;
                   case 5:
                   (void) ISHELL_LoadResString(pMe->m_pShell,
                                    AEE_APPSSOUNDMENU_RES_FILE,
                                    IDS_ARABIC_RINGTONE_5,
                                    pListItem->szName,
                                    sizeof(pListItem->szName));
                break;
                case 6:
                   (void) ISHELL_LoadResString(pMe->m_pShell,
                                    AEE_APPSSOUNDMENU_RES_FILE,
                                    IDS_ARABIC_RINGTONE_6,
                                    pListItem->szName,
                                    sizeof(pListItem->szName));
                   break;
                case 7:
                   (void) ISHELL_LoadResString(pMe->m_pShell,
                                    AEE_APPSSOUNDMENU_RES_FILE,
                                    IDS_ARABIC_RINGTONE_7,
                                    pListItem->szName,
                                    sizeof(pListItem->szName));
                   break;
                case 8:
                   (void) ISHELL_LoadResString(pMe->m_pShell,
                                    AEE_APPSSOUNDMENU_RES_FILE,
                                    IDS_ARABIC_RINGTONE_8,
                                    pListItem->szName,
                                    sizeof(pListItem->szName));
                   break;
                case 9:
                   (void) ISHELL_LoadResString(pMe->m_pShell,
                                    AEE_APPSSOUNDMENU_RES_FILE,
                                    IDS_ARABIC_RINGTONE_9,
                                    pListItem->szName,
                                    sizeof(pListItem->szName));
                   break;
                case 10:
                   (void) ISHELL_LoadResString(pMe->m_pShell,
                                    AEE_APPSSOUNDMENU_RES_FILE,
                                    IDS_ARABIC_RINGTONE_10,
                                    pListItem->szName,
                                    sizeof(pListItem->szName));
                   break;
              }
#endif
                mai.pText = pListItem->szName;
                    // ��Ӵ�ͼ��˵���˵�
                    (void)IMENUCTL_AddItemEx(pMenu, &mai);
                    //ERR("dwIndex = %d",dwIndex , 0, 0);
                    }
                }
        }
    }

    if ((pMe->m_eMakeListMode == MAKELIST_NEXTPAGE) ||
        (pMe->m_eMakeListMode == MAKELIST_PREPAGE))
    {
        if (pMe->m_eMakeListMode == MAKELIST_PREPAGE)
        {
            // ����ǰѡ������Ϊĩβ�������ƶ��˵����������
            pMe->m_lastRingerPlayed = mai.wItemID;
        }

        if (pMe->m_eMakeListMode == MAKELIST_NEXTPAGE)
        {
            uint16 wItemID;
            wItemID = IMENUCTL_GetItemID(pMenu, 0);
            pMe->m_lastRingerPlayed = wItemID;
        }
    }
    pMe->m_fSubDlgId = pMe->m_lastRingerPlayed;

    IMENUCTL_SetSel(pMenu, pMe->m_lastRingerPlayed);
    (void)IMENUCTL_Redraw(pMenu);
    SOUND_ERR("UpdateRingerListMenu end!",0,0,0);
}

static void  SoundMenu_Ringer_List (ISoundMenu *pi,void*  ringer_id)
{
   register CSoundMenu *pMe = (CSoundMenu*)pi;
   char     args[16] = {0};
   SPRINTF(args, "R %u", ringer_id);
   ISHELL_StartAppletArgs(pMe->m_pShell,
                                AEECLSID_APP_SOUNDMENU,
                                args);
}
