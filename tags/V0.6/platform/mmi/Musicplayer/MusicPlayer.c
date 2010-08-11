/*==============================================================================
// �ļ���
//        MusicPlayer.c
//        ��Ȩ����(c) 2009 Anylook Tech. CO.,LTD.
//        
// �ļ�˵����
//        
==============================================================================*/

/*==============================================================================
�޸���ʷ��¼��
ʱ       ��      �� �� ��     �޸����ݡ�λ�ü�ԭ��
-----------      ----------     -----------------------------------------------
==============================================================================*/


/*==============================================================================
                                 ���ļ��������ⲿ�ļ�
==============================================================================*/
#include "MusicPlayer_priv.h"
#include "AEE_OEMEvent.h"
#include "MediaGallery.h"
/*==============================================================================
                                 �궨��ͳ���
==============================================================================*/

/*==============================================================================
                                 ���Ͷ���
==============================================================================*/

/*==============================================================================
                                 ��������
==============================================================================*/
/*----------------------Applet��غ�������---------------------*/
int MusicPlayer_Load(IShell *ps, void * pHelpers, IModule ** pMod);
#if defined(AEE_STATIC)
static int MusicPlayer_CreateInstance(AEECLSID    ClsId,
                                       IShell     *pIShell,
                                       IModule    *po,
                                       void      **ppObj);
#endif

static int MusicPlayer_New( IShell *ps,
                             IModule *pIModule,
                             void **ppObj);  

static uint32  IMusicPlayer_AddRef(IMusicPlayer *pi);

static uint32  IMusicPlayer_Release(IMusicPlayer *pi);

static boolean IMusicPlayer_HandleEvent(IMusicPlayer *pi,
                                         AEEEvent       eCode,
                                         uint16         wParam,
                                         uint32         dwParam
                                        );
/*��Gallery���м򵥲��ŵĽӿ�*/
static int IMusicPlayer_SimplePlayer(IMusicPlayer *pi,char *musicName);
/*���Ʋ������ò��ŵĽӿ�*/
static int IMusicPlayer_PedometerCallPlayer(IMusicPlayer *pi);
static int CMusicPlayer_InitAppData(CMusicPlayer *pMe);

static void CMusicPlayer_FreeAppData(CMusicPlayer *pMe);

static void CMusicPlayer_RunFSM(CMusicPlayer *pMe);
//��Ҫ���ڿ��ƿ��ٰ������ֵ�����
static void CMusicPlayer_AppIsReadyCB(void * pUser);

//�жϴ���
static void MP3_InterruptHandle(CMusicPlayer *pMe);

/*�жϻָ�����*/
static void MP3_ResumeHandle(CMusicPlayer *pMe);

/*�رռ�����*/
static void MP3_CloseKeyBeepVol(CMusicPlayer *pMe);

/*�ָ�������*/
static void MP3_ResumeKeyBeepVol(CMusicPlayer *pMe);
static void Music_Shake_Open(void);

static void Music_Shake_Close(void);
static void MP3_DecodeStartArgs(CMusicPlayer *pMe, char *args);
static void MP3_SetStartCurState(CMusicPlayer *pMe);
static void MP3_SetStartStatus(CMusicPlayer *pMe);

static void MP3_Build_DefaultPlaylist(CMusicPlayer *pMe);
/*==============================================================================
                                 ȫ������
==============================================================================*/
static Mp3Player_Status g_eMp3PlayerStatus = MP3STATUS_NONE;
#ifdef FEATURE_SUPPORT_BT_APP
static boolean g_bPlayMP3ByBt = FALSE;
#endif
static int16 g_nInterruptRef = 0;
/* ���´����Ϊ��������Ƥ��������չ*/
static AEERect MP3SKIN1_RECT[] = {
               {0,0,},               // �������ͼ����
               {15,10,149,22},       // ������������
               {4,134,46,6},        // ����������
               {9,168,50,18},        // ʱ����ʾ����
               {126,142,48,18},      // ������ʾ����
               {82,136,},            // ������������
               {82,174,},            // ������������
               {79,150,},            // ������ͣ����
               {65,153,},            // ��һ������
               {101,153,},           // ��һ������
               {131,168,},           // ������ʾ����
               {40,140,},            // �������
               {3,140,},             // ��������
               {0,0},                   // ���ٲ鿴���ڲ��ŵ��б��ͼ������
               {60,129,52,50}        // ������һ����һ�ײ�����ͣ�ȵļ��о��ε�����
};

/*==============================================================================
                                 ���أ���̬������
==============================================================================*/
#define MUSICPLAYER_METHODS     \
        {                           \
             IMusicPlayer_AddRef,     \
             IMusicPlayer_Release,     \
             IMusicPlayer_HandleEvent,     \
             IMusicPlayer_SimplePlayer,     \
             IMusicPlayer_PedometerCallPlayer   \
        }

#if defined(AEE_STATIC)
// ֻ����һ��MusicPlayerʵ����ÿ�δ���MusicPlayer Appletʱ��
// ����ͬһ�ṹָ���BREW�㡣
static CMusicPlayer gMusicPlayer;

static const VTBL(IMusicPlayer) gMusicPlayerMethods = MUSICPLAYER_METHODS;
#endif

/*==============================================================================
                                 ��������
==============================================================================*/
/*----------------------Applet��غ�������---------------------*/
#if defined(AEE_STATIC)
int MusicPlayer_Load(IShell *ps, void * pHelpers, IModule ** pMod)
{
    return(AEEStaticMod_New( (int16)(sizeof(AEEMod)),
                             ps,
                             pHelpers,
                             pMod,
                             (PFNMODCREATEINST)MusicPlayer_CreateInstance,
                             (PFNFREEMODDATA)NULL));
}

#endif

/*==============================================================================
����: 
       MusicPlayer_CreateInstance
// DESCRIPTION
//      This function is invoked while the app is being loaded. All Modules must provide this 
//  function. Ensure to retain the same name and parameters for this function.
//
// PARAMETERS
//  ClsId: [in]: Specifies the ClassID of the applet which is being loaded
//  pIShell: [in]: Contains pointer to the IShell interface. 
//  pIModule: [in]: Contains pointer to the IModule interface to the current module to which
//  this app belongs
//  ppObj: [out]: On return, *ppObj must point to a valid AEEApplet structure. Allocation
//  of memory for this structure and initializing the base data members is done by AEEApplet_New().
//
// Return Value:
//      
==============================================================================*/
#if defined(AEE_STATIC)
static int MusicPlayer_CreateInstance( AEECLSID    ClsId, 
                                        IShell      *pIShell,
                                        IModule     *po, 
                                        void        **ppObj)
#else
int AEEClsCreateInstance( AEECLSID      ClsId, 
                          IShell        * pIShell, 
                          IModule       * po, 
                          void          ** ppObj)
#endif
{
    if (ClsId == AEECLSID_APP_MUSICPLAYER)
    {
        return MusicPlayer_New(pIShell, po, ppObj);
    }
    else
    {
        //ERR("CLS ID IS NOT",0,0,0);
        return EUNSUPPORTED;
    }
}
/*==============================================================================
������MusicPlayer_New

˵����
       ��ʼ��MusicPlayer Applet�������ڲ����ݱ�����ͬʱMusicPlayer Applet
       ���ü�����1��

������
       ps [in]��IShell�ӿڶ���ָ�롣
       pIModule [in/out]��pIModule�ӿڶ���ָ�롣
       ppObj [out]:��ָ��IMusicPlayer����ָ���ָ�롣

����ֵ��
       EFAILED��ʧ�ܡ�
       SUCCESS���ɹ���
       
��ע��:
       �����û�ȥ���á�
       ����Applet�ṹ�����ʹ�ö�̬�����ڴ�ķ�ʽ�������������õ���ʹ�þ�̬
       �����ķ�ʽ������Խṹ�Ƚϴ��Appletʹ�þ�̬�����ķ�ʽ�����ڶ�̬����
       �ڴ�ķ�ʽ��Ҫע���������ռ���ͷš�

==============================================================================*/
static int MusicPlayer_New(IShell *ps, IModule *pIModule, void **ppObj)
{
    CMusicPlayer *pMe;
    int retVal = SUCCESS;
    if(!ppObj || !ps || !pIModule)
    {
        //ERR("STH IS NULL",0,0,0);
        return EFAILED;
    }
#if defined(AEE_STATIC)
    pMe = &gMusicPlayer;
#else
    pMe = (CMusicPlayer *)MALLOC(sizeof(CMusicPlayer) + sizeof(VTBL(IMusicPlayer)));
#endif
    DBGPRINTF("new");
    if (NULL == pMe)
    {
        return EFAILED;
    }
    if (0 == pMe->m_nRefs)
    {
#if defined(AEE_STATIC)
        // Must initialize the object
        MEMSET(pMe,  0, sizeof(CMusicPlayer));
        INIT_VTBL(pMe, IMusicPlayer, gMusicPlayerMethods);
#else
//NOTE: In the dynamic applet, the extend classes will re-malloc the memery.
//      So it run in simulator for test only. If you want run in simulator correcttly
//      Please modify the following code.
        VTBL(IMusicPlayer) gMusicPlayerMethods = MUSICPLAYER_METHODS;
        VTBL(IMusicPlayer) *pMod = (VTBL(IMusicPlayer) *)((byte *)pMe + sizeof(CMusicPlayer));
        MEMCPY((byte *)pMod, (byte *)&gMusicPlayerMethods, sizeof(VTBL(IMusicPlayer)));
        INIT_VTBL( pMe,IMusicPlayer*pMod);
#endif

        pMe->m_nRefs     = 0;
        pMe->m_pShell    = ps;
        pMe->m_pModule   = pIModule;
        (void) ISHELL_AddRef(pMe->m_pShell);
        (void) IMODULE_AddRef(pMe->m_pModule);

        retVal = CMusicPlayer_InitAppData(pMe);
        if( SUCCESS != retVal )
        {
            CMusicPlayer_FreeAppData(pMe);
            return retVal;
        }
    }
    
    ++pMe->m_nRefs;
    *ppObj = (IMusicPlayer*)pMe;

    return retVal;
}

/*==============================================================================
������IMusicPlayer_AddRef

˵����
       MusicPlayer Applet ���ü�����1�������ص�ǰ����������

������
       pi [in/out]��IMusicPlayer�ӿڶ���ָ�롣

����ֵ��
       ��ǰ��MusicPlayer Applet��������

��ע��:

==============================================================================*/
static uint32 IMusicPlayer_AddRef(IMusicPlayer *pi)
{
    CMusicPlayer *pMe = (CMusicPlayer*)pi;
#if defined(AEE_STATIC)
    ASSERT(pMe != NULL);
    ASSERT(pMe->m_nRefs > 0);
#endif

    return (++pMe->m_nRefs);
}

/*==============================================================================
������IMusicPlayer_Release

˵����
       �ͷŶ�SounMenu Applet�����ã����ü�����1�������ص�ǰ����������
       �����ǰ��������Ϊ�㣬�����ͷ�Applet�������Դ��

������
       pi [in/out]��IMusicPlayer�ӿڶ���ָ�롣

����ֵ��
       ��ǰ��MusicPlayer Applet��������

��ע��:

==============================================================================*/
static uint32  IMusicPlayer_Release (IMusicPlayer *pi)
{
    CMusicPlayer *pMe = (CMusicPlayer*)pi;

#if defined(AEE_STATIC)
    ASSERT(pMe != NULL);
#endif
    if (pMe->m_nRefs == 0)
    {
        return 0;
    }

    if (--pMe->m_nRefs)
    {
        return pMe->m_nRefs;
    }
    // �ͷ�Applet�����Դ
    CMusicPlayer_FreeAppData(pMe);
    (void) ISHELL_Release(pMe->m_pShell);
    (void) IMODULE_Release(pMe->m_pModule);

    // ע�⣺pMe�Ǿ�̬����ռ䣬��˲���Ҫ�ͷš�FREE()
#if !defined(AEE_STATIC)
    (void)FREE(pMe);
#endif
    return 0;
}

/*==============================================================================
������CMusicPlayer_InitAppData

˵����
       ��ʼ��CMusicPlayer Applet�ṹ����ȱʡֵ��

������
       pMe [in]��ָ��MusicPlayer Applet����ṹ��ָ�롣�ýṹ����С������ض���Ϣ��

����ֵ��
       EFAILED����ʼ��ʧ�ܡ�
       SUCCESS����ʼ���ɹ���

��ע��:

==============================================================================*/
static int CMusicPlayer_InitAppData(CMusicPlayer *pMe)
{
   AEEDeviceInfo  di;
   int i;
  #if defined(AEE_STATIC)
    ASSERT(pMe != NULL);
  #endif
    ISHELL_GetDeviceInfo(pMe->m_pShell, &di);
    pMe->m_rc.x       = 0;
    pMe->m_rc.y       = 0;
    pMe->m_rc.dx      = (int16) di.cxScreen;
    pMe->m_rc.dy      = (int16) di.cyScreen;
    pMe->m_ePreState = STATE_NONE;
    pMe->m_eCurState = STATE_INIT;
    pMe->m_pActiveDlg = NULL;
    //pMe->m_pActiveDlgID = IDD_WINDOWS;
    pMe->m_eStartMethod=STARTMETHOD_NORMAL;
    pMe->m_eDlgRet = DLGRET_CREATE;
    pMe->m_ePreMp3Status = MP3STATUS_NONE;
    pMe->m_bNotOverwriteDlgRet = FALSE;
    pMe->m_pImage = NULL;
    pMe->m_pMedia = NULL;
    pMe->m_bSuspending = FALSE;
    pMe->m_nCurPlaylistID=0;
    pMe->m_nPlaylistNum=0;
    pMe->m_nCurMusiclistID=0;
    pMe->m_nPlaylistMusicNum=0;
    pMe->m_nPlayinglistMusicNum=0;
    pMe->m_nCtlID = IDC_MUSICPLAY_MODE;
    pMe->m_bPlaying = FALSE;
    pMe->m_bPaused = FALSE;
    pMe->m_bUserStopped = FALSE;
    pMe->m_bUserPressNext = FALSE;
    pMe->m_bAppReady = FALSE;
    pMe->m_bRenameOpt = FALSE;
    pMe->m_nAutoScrollIdx = 0;
    pMe->m_nCurrentTime = 0;
    pMe->m_nTotalTime = 0;
    pMe->m_nSimPlayCurTime = 0;
    pMe->m_keyBeepVol = 0;
    pMe->m_bInterrupt = FALSE;
    pMe->m_pMp3FileToPlay = NULL;
    pMe->m_bStartApp = FALSE;
    pMe->m_bSimPlayFailed = FALSE;
    g_nInterruptRef = 0;
    pMe->m_nRandNum = 0;
#ifdef FEATURE_SUPPORT_BT_APP
#ifndef WIN32
    pMe->m_nBTID = BCMAPP_AG_BAD_APPID;
#endif
    pMe->m_bUse848 = TRUE;
    pMe->m_bUseBT = FALSE;
    SetIsPlayMP3ByBt(FALSE);
    pMe->m_bInterruptByBT = FALSE;
#endif //FEATURE_SUPPORT_BT_APP
  if(SUCCESS != ISHELL_GetPrefs(pMe->m_pShell,
                               AEECLSID_APP_MUSICPLAYER,
                               MUSICPLAYERCFG_VERSION,
                               &pMe->m_MusicPlayerCfg,
                               sizeof(CMusicPlayerCfg)))
  {
    pMe->m_MusicPlayerCfg.eMusicPlayMode =PLAYMODE_ORDER;
    pMe->m_MusicPlayerCfg.eMusicVolume=VOLUME_THREE;
    pMe->m_MusicPlayerCfg.eMusicSortBy=SORT_NONE;
#ifdef FEATURE_MP3PLAYER_SWITCHSKIN
    pMe->m_MusicPlayerCfg.eMp3Skin = PLAYERSKIN1;
#endif
    pMe->m_MusicPlayerCfg.lastPlayMusicID=0;
    pMe->m_MusicPlayerCfg.lastPlayPlaylist[0]='\0';
  }
#ifdef FEATURE_MP3PLAYER_SWITCHSKIN
  pMe->m_pMP3FaceRect = GetPlayerSkinRect(pMe->m_MusicPlayerCfg.eMp3Skin);
#else
  pMe->m_pMP3FaceRect = MP3SKIN1_RECT;
#endif
  pMe->m_nPlayMode = pMe->m_MusicPlayerCfg.eMusicPlayMode;
  pMe->m_nVolume = pMe->m_MusicPlayerCfg.eMusicVolume;
  pMe->m_nMusicSort = pMe->m_MusicPlayerCfg.eMusicSortBy;
  pMe->m_nCurrentVolume=pMe->m_MusicPlayerCfg.eMusicVolume*AEE_MAX_VOLUME/5;
  if (AEE_SUCCESS != ISHELL_CreateInstance(pMe->m_pShell,AEECLSID_ANNUNCIATOR,(void **)&pMe->m_pIAnn))
    {
        return EFAILED;
    }
  IANNUNCIATOR_SetHasTitleText(pMe->m_pIAnn, FALSE);
   if ( SUCCESS != ISHELL_CreateInstance( pMe->m_pShell,
                                          AEECLSID_FILEMGR,
                                          (void **)&pMe->m_pFileMgr))
    {
        return EFAILED;
    }
   
   if (AEE_SUCCESS != ISHELL_CreateInstance(pMe->m_pShell,
                                               AEECLSID_CONFIG,
                                               (void **)&pMe->m_pConfig))
   {
        return EFAILED;
   }
     // Create IDisplay instance
    if ( SUCCESS != ISHELL_CreateInstance( pMe->m_pShell,
                                           AEECLSID_DISPLAY,
                                           (void **)&pMe->m_pDisplay))
    {
        return EFAILED;
    }
    if( ISHELL_CreateInstance( pMe->m_pShell, AEECLSID_BACKLIGHT, (void **)&pMe->m_pBackLight)!=AEE_SUCCESS)
    {
        return EFAILED;
    }
   for(i=0;i<MUSIC_MAX_FILESNUM;i++)
   {
     MEMSET(pMe->m_Musiclist[i].pMusicName,0,MP3NAME_MAX_LEN*sizeof(char));
     MEMSET(pMe->m_PlayingMusiclist[i].pMusicName,0,MP3NAME_MAX_LEN*sizeof(char));
   }
   MP3_Build_DefaultPlaylist(pMe);
   if(pMe->m_MusicPlayerCfg.lastPlayPlaylist[0]!='\0')
   {
     (void) CMusicPlayer_ReadMusiclist(pMe,pMe->m_MusicPlayerCfg.lastPlayPlaylist,TRUE);
   }
   else
   {
    for(i=0;i<MUSIC_MAX_FILESNUM;i++)
    {
     MEMSET(pMe->m_PlayingMusiclist[i].pMusicName,0,MP3NAME_MAX_LEN*sizeof(char));
    }
   }
   MP3_ResetRandonIdentifier(pMe);
   pMe->m_pMp3FileToPlay = (AECHAR *)MALLOC(MP3NAME_MAX_LEN * sizeof(AECHAR));
   return SUCCESS;
  
}

/*==============================================================================
������CMusicPlayer_FreeAppData

˵����
       �ͷ�MusicPlayer Applet�����Դ��

������
       pMe [in]��ָ��MusicPlayer Applet����ṹ��ָ�롣�ýṹ����С������ض���Ϣ��

����ֵ��
       �ޡ�

��ע��:

==============================================================================*/
static void CMusicPlayer_FreeAppData(CMusicPlayer *pMe)
{ 
 #if defined(AEE_STATIC)
    ASSERT(pMe != NULL);
  #endif

    (void)ISHELL_CancelTimer(pMe->m_pShell, NULL, pMe);
  	if(pMe->m_pIAnn)
    {
        IANNUNCIATOR_SetHasTitleText(pMe->m_pIAnn, TRUE);
        IANNUNCIATOR_Release(pMe->m_pIAnn);
        pMe->m_pIAnn = NULL;
    }
	
    if (pMe->m_pDisplay != NULL)
    {
        (void) IDISPLAY_Release(pMe->m_pDisplay);
        pMe->m_pDisplay = NULL;
    }
    
    if (pMe->m_pFileMgr != NULL)
    {
        (void) IFILEMGR_Release(pMe->m_pFileMgr);
        pMe->m_pFileMgr = NULL;
    } 
    if (pMe->m_pConfig)
    {
        ICONFIG_Release(pMe->m_pConfig);
        pMe->m_pConfig = NULL;
    }
     if(pMe->m_pBackLight)
    {
        IBACKLIGHT_Release(pMe->m_pBackLight);
        pMe->m_pBackLight = NULL;
    }
    ISHELL_RegisterNotify( pMe->m_pShell,
                           AEECLSID_APP_MUSICPLAYER,
                           AEECLSID_CM_NOTIFIER,
                           0
                          );
    FREEIF(pMe->m_pMsgBox_Msg);
    FREEIF(pMe->m_pInputListName);
    FREEIF(pMe->m_pBuffer);
    FREEIF(pMe->m_pMp3FileToPlay);
    FREEIF(pMe->m_pSimplePlayMusicName);
   if(pMe->m_pImage)
   {
      IIMAGE_Release(pMe->m_pImage);
      pMe->m_pImage = NULL;
   }
    if(pMe->m_paniImage)
    {
        IIMAGE_Release(pMe->m_paniImage);
        pMe->m_paniImage = NULL;
    }
    if(pMe->m_pMode)
    {
        IMENUCTL_Release(pMe->m_pMode);
        pMe->m_pMode = NULL;
    }
    if(pMe->m_pVolume)
    {
        IMENUCTL_Release(pMe->m_pVolume);
        pMe->m_pVolume = NULL;
    }
    if(pMe->m_pSort)
    {
        IMENUCTL_Release(pMe->m_pSort);
        pMe->m_pSort = NULL;
    }
#ifdef FEATURE_SUPPORT_BT_APP
#ifndef WIN32
    if(pMe->m_bUseBT)
    {
        bcmapp_ag_audio_close(pMe->m_nBTID);
        pMe->m_nBTID = BCMAPP_AG_BAD_APPID;
    }
#endif
    //g_bPlayMP3ByBt = FALSE;
    SetIsPlayMP3ByBt(FALSE);
    pMe->m_bUseBT = FALSE;
#endif
    CMusicPlayer_ReleaseMedia(pMe);
} // CMusicPlayer_FreeAppData

/*==============================================================================
������CMusicPlayer_RunFSM

˵����
       MusicPlayer Applet����״̬�����档

������
       pMe [in]��ָ��MusicPlayer Applet����ṹ��ָ�롣�ýṹ����С������ض���Ϣ��

����ֵ��
       �ޡ�

��ע��:

==============================================================================*/
static void CMusicPlayer_RunFSM(CMusicPlayer *pMe)
{ 
    NextFSMAction nextFSMAction = NFSMACTION_WAIT;
   
#if defined(AEE_STATIC)
    ASSERT(pMe != NULL);
#endif
    for ( ; ; )
    {
        nextFSMAction = CMusicPlayer_ProcessState(pMe);

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
} // CMP_RunFSM
extern AEECLSID            AEE_Active(void);
/*==============================================================================
������IMusicPlayer_HandleEvent

˵����
       MusicPlayer Applet�¼��������������������ڱ�Applet���¼����������ɸú�����
       ��

������
       pi [in]��ָ��IMusicPlayer�ӿ�t�����ָ�롣
       eCode��BREW�¼����롣
       wParam���¼�����
       dwParam [in]����wParam���������ݡ���

����ֵ��
        TRUE�������¼�������
        FALSE�������¼�û������

��ע��:
        �ú�����BREW���ã��û����õ��á�
        
==============================================================================*/
static boolean IMusicPlayer_HandleEvent( IMusicPlayer *pi,
                                         AEEEvent    eCode,
                                         uint16      wParam,
                                         uint32  dwParam)
{
    CMusicPlayer *pMe = (CMusicPlayer*)pi;
    AEEAppStart *as;
    switch (eCode)
    {
        case EVT_APP_START_BACKGROUND:
        case EVT_APP_START:
        #if defined(AEE_STATIC)
            ASSERT(dwParam != 0);
        #endif
            as = (AEEAppStart*)dwParam;
            pMe->m_rc = as->rc;
            pMe->m_bSuspending = FALSE;
            pMe->m_bAppReady = FALSE;
            pMe->m_bActive = TRUE;
            MP3_CloseKeyBeepVol(pMe);  //close keybeepvol
            Music_Shake_Open();
            MP3_DecodeStartArgs(pMe,(char *)as->pszArgs);
            MP3_SetStartCurState(pMe);
           if(GetMp3PlayerStatus() == MP3STATUS_NONE && APP_MEDIA_ALLOW == app_media_scheduler()
                    && pMe->m_eStartMethod != STARTMETHOD_SIMPLEPLAYER)
            {
                pMe->m_bStartApp = TRUE;
                (void) ISHELL_SetTimer(pMe->m_pShell,200,(PFNNOTIFY)MP3_InitMusicCB,pMe);
            }
            MP3_SetStartStatus(pMe);
            if(ISHELL_RegisterNotify( pMe->m_pShell, 
                              AEECLSID_APP_MUSICPLAYER,
                              AEECLSID_CM_NOTIFIER,
                              NMASK_CM_VOICE_CALL | NMASK_CM_OTHER_CALL | NMASK_CM_TEST_CALL) != SUCCESS)
            {
               CMusicPlayer_FreeAppData(pMe);
               return FALSE;
            }
            
            // ��ʼMusicPlayer״̬��
            CMusicPlayer_RunFSM(pMe);
            return TRUE;

        case EVT_APP_STOP:
           if(GetMp3PlayerStatus()!=MP3STATUS_RUNONBACKGROUND)
           {
                pMe->m_bActive = FALSE;
                SetMp3PlayerStatus(pMe,MP3STATUS_NONE);
                if(pMe->m_eStartMethod != STARTMETHOD_PEDOMETERCALLPLAYER)
                {
                 (void) ISHELL_SetPrefs(pMe->m_pShell,
                                       AEECLSID_APP_MUSICPLAYER,
                                       MUSICPLAYERCFG_VERSION,
                                       &pMe->m_MusicPlayerCfg,
                                       sizeof(CMusicPlayerCfg));
                }
               MP3_ResumeKeyBeepVol(pMe);  //resume keybeepvol
               Music_Shake_Close();
           }
           else
           {
              ISHELL_CancelTimer(pMe->m_pShell,(PFNNOTIFY)MP3_MusicNameAutoScroll,pMe);
             (void)ISHELL_PostEvent( pMe->m_pShell,
                                        AEECLSID_CORE_APP,
                                        EVT_DRAWMUSICNAME,
                                        0,
                                        (uint32)pMe->m_pMp3FileToPlay);
             *((boolean*)dwParam) = FALSE;
           }
            pMe->m_bSuspending = TRUE;
            //*((boolean*)dwParam) = !g_bMp3PlayBackground;

            return TRUE;

        case EVT_APP_SUSPEND:
            pMe->m_bSuspending = TRUE;
            MP3_InterruptHandle(pMe);
            return TRUE;

        case EVT_APP_RESUME:
        #if defined(AEE_STATIC)
            ASSERT(dwParam != 0);
        #endif
            as = (AEEAppStart*)dwParam;
            pMe->m_bSuspending = FALSE;
            pMe->m_rc = as->rc;
            if(pMe->m_eCurState == STATE_PLAYLIST_OPTS || pMe->m_eCurState == STATE_VIEW_OPTS
                || pMe->m_eCurState == STATE_MAINOPTSMENU )
            {
                pMe->m_eCurState = pMe->m_ePreState;
            }
            MP3_ResumeHandle(pMe);
            CMusicPlayer_RunFSM(pMe);
            return TRUE;

        case EVT_DIALOG_INIT:
            // ���¶Ի�����Ʋ�����Ϣ������
            pMe->m_pActiveDlg = (IDialog*)dwParam;
            pMe->m_pActiveDlgID = wParam;
            pMe->m_bAppReady = FALSE;

            //Add By zzg 2010_08_09
            if (pMe->m_pIAnn != NULL)
            {
                IANNUNCIATOR_SetFieldIsActiveEx(pMe->m_pIAnn, FALSE);
                IANNUNCIATOR_SetHasTitleText(pMe->m_pIAnn, TRUE);
        	}
            //Add End
            
            return CMusicPlayer_RouteDialogEvent(pMe,eCode,wParam,dwParam);

        case EVT_DIALOG_END:
            if (wParam == 0)
            {
                return TRUE;
            }

            (void) CMusicPlayer_RouteDialogEvent(pMe,eCode,wParam,dwParam);
            pMe->m_pActiveDlg = NULL;
            // Applet�����𣬲���״̬�����ȴ�Applet���ء���ע�⣺EVT_APP_SUSPEND
            // �¼���EVT_DIALOG_END�¼�ǰ������
            if (pMe->m_bSuspending == FALSE)
            {
                // ��ʼMusicPlayer״̬��
                CMusicPlayer_RunFSM(pMe);
            }
            return TRUE;
        case EVT_USER_REDRAW:
           (void) CMusicPlayer_RouteDialogEvent(pMe,eCode,wParam,dwParam);
            (void)ISHELL_SetTimer( pMe->m_pShell,
                                   MP3APPISREADY_TIME,
                                   CMusicPlayer_AppIsReadyCB,
                                   pMe);
            return TRUE;
        case EVT_ALARM:
            { 
                if(wParam)    
                {  
                   MP3_InterruptHandle(pMe);
                }
                else
                {    
                   MP3_ResumeHandle(pMe);
                }
     
            }
            return TRUE;

        case EVT_NOTIFY:
            {
                if(GetMp3PlayerStatus()== MP3STATUS_RUNONBACKGROUND)
                {
                    AEENotify* notify = (AEENotify*)dwParam;
                    notify->st = NSTAT_IGNORED;
                    if( notify->cls == AEECLSID_CM_NOTIFIER)
                    {
                        if( ( notify->dwMask & NMASK_CM_VOICE_CALL)||( notify->dwMask & NMASK_CM_TEST_CALL))
                        {
                            AEECMNotifyInfo* eventInfo = (AEECMNotifyInfo *)(notify->pData);
                           if( eventInfo->event == AEECM_EVENT_CALL_ORIG ||
                                eventInfo->event == AEECM_EVENT_CALL_INCOM )
                            {
                               MP3_InterruptHandle(pMe);
                            }
                           else if(eventInfo->event == AEECM_EVENT_CALL_END)
                           {
                            // ���ڽ���AEECM_EVENT_CALL_END�¼������ϻָ�ʱ���������ڸ�Ϊͨ������EVT_ALARM���ָ�����ʱע�͵������
                             //MP3_ResumeHandle(pMe);
                           }
                            notify->st = NSTAT_PROCESSED;
                        }
                    }
                }
                return TRUE;
            }
        case EVT_GSENSOR_SHAKE:
            switch(wParam)
            {
                case AVK_GSENSOR_FORWARD:
                    CMusicPlayer_PlayNext(pMe,FALSE );//������һ��
                    return TRUE;

                case AVK_GSENSOR_BACKWARD:
                    pMe->m_bUserPressNext = TRUE;
                    CMusicPlayer_PlayNext(pMe,TRUE);//������һ��
                    pMe->m_bUserPressNext = FALSE;
                    return TRUE;
            }
#ifdef FEATURE_LCD_TOUCH_ENABLE//WLH ADD FOR LCD TOUCH
		case EVT_PEN_UP:
		//case EVT_PEN_DOWN:
#endif//FEATURE_LCD_TOUCH_ENABLE
        case EVT_KEY_PRESS:
        case EVT_KEY:
        case EVT_KEY_RELEASE:
        case EVT_COMMAND:
            if(!pMe->m_bAppReady)
            {
                return TRUE;
            }
            return CMusicPlayer_RouteDialogEvent(pMe,eCode,wParam,dwParam);
        case EVT_CLOSEAPP:
            SetMp3PlayerStatus(pMe,MP3STATUS_NONE);
            if(AEECLSID_CORE_APP == AEE_Active())
            {
                FREEIF(pMe->m_pMp3FileToPlay);
                (void)ISHELL_PostEvent( pMe->m_pShell,
                                        AEECLSID_CORE_APP,
                                        EVT_DRAWMUSICNAME,
                                        0,
                                        (uint32)pMe->m_pMp3FileToPlay);
            }
            ISHELL_CloseApplet(pMe->m_pShell, FALSE);
            return TRUE;

        case EVT_DRAWMUSICNAME:
            (void)ISHELL_PostEvent( pMe->m_pShell,
                                    AEECLSID_CORE_APP,
                                    EVT_DRAWMUSICNAME,
                                    0,
                                    (uint32)pMe->m_pMp3FileToPlay);
            return TRUE;
            
        case EVT_OPENSHAKE:
            Music_Shake_Open();
            return TRUE;
        case EVT_POPMSGDIALOG:
            pMe->m_nMsgResID = wParam;//IDS_PLAYFAILED;
            if(pMe->m_eCurState == STATE_PLAYLIST_OPTS || pMe->m_eCurState == STATE_VIEW_OPTS
                || pMe->m_eCurState == STATE_MAINOPTSMENU )
            {
                pMe->m_eCurState = pMe->m_ePreState;
            }
            pMe->m_eMsgRetState = pMe->m_eCurState;
            pMe->m_eMsgType = MESSAGE_WARNNING;
            //pMe->m_eMsgRetState = STATE_PLAYMUSIC_WINDOWS;
            MOVE_TO_STATE(STATE_MSG);
            CLOSE_DIALOG(DLGRET_CREATE);
            return TRUE;

#ifdef FEATURE_SUPPORT_BT_APP
        case AVK_BT_HEADSET_CONNECT:
             if(pMe->m_bInterruptByBT)
             {
                return TRUE;
             }
#ifndef WIN32
             pMe->m_nBTID = bcmapp_is_ag_connected();
             if(BCMAPP_AG_BAD_APPID != pMe->m_nBTID)
             {
                (void)bcmapp_ag_audio_open(pMe->m_nBTID, 
                                            (PFN_AG_NOTIFY)MP3_PlayMusicByBTCallBack,
                                            pMe, 
                                            pMe->m_MusicPlayerCfg.eMusicVolume,
                                            pMe->m_bUse848); 
                pMe->m_bUseBT = TRUE;
                //g_bPlayMP3ByBt = TRUE;
                SetIsPlayMP3ByBt(TRUE);
             }
#endif
             return TRUE;
             
        case AVK_BT_HEADSET_DISCONNECT:
            if(pMe->m_bInterruptByBT)
            {
                return TRUE;
            }
            if(pMe->m_bUseBT)
            {
#ifndef WIN32
                bcmapp_ag_audio_close(pMe->m_nBTID);
                pMe->m_nBTID = BCMAPP_AG_BAD_APPID;
#endif
                pMe->m_bUseBT = FALSE;
                SetIsPlayMP3ByBt(FALSE);
            }
            return TRUE;
            
       case EVT_APP_EXIT:
           if(!pMe->m_bPlaying && !pMe->m_bPaused)
           {
             SetMp3PlayerStatus(pMe,MP3STATUS_NONE);
             ISHELL_CloseApplet(pMe->m_pShell, TRUE);
           }
           return TRUE;
#endif //FEATURE_SUPPORT_BT_APP
#ifdef FEATURE_LCD_TOUCH_ENABLE//WLH ADD FOR LCD TOUCH
		case EVT_USER:
			{
				if (wParam == AVK_SELECT)
				{
					if (dwParam == 0)
					{
						eCode = EVT_KEY;
					}
					else if(dwParam == KEY_PRESS_DWPARM)
					{
						eCode = EVT_KEY_PRESS;
						dwParam = 0;
					}
					else
					{
						eCode = EVT_COMMAND;
						wParam = dwParam;
					}
				}
				else if (wParam == AVK_INFO)
				{
					eCode = EVT_KEY;
				}
				else if (wParam == AVK_CLR)
				{
					eCode = EVT_KEY;
				}
				else if ((wParam == AVK_LEFT)||(wParam == AVK_RIGHT))
				{
					eCode = EVT_KEY;
				}
                
			}
			return CMusicPlayer_RouteDialogEvent(pMe,eCode,wParam,dwParam);
#endif//FEATURE_LCD_TOUCH_ENABLE

        default:
            // �����յ����¼�·������ǰ��ĶԻ����¼���������
            return CMusicPlayer_RouteDialogEvent(pMe,eCode,wParam,dwParam);
    }
} // CMusicPlayer_HandleEvent
/*==============================================================================
������ IMusicPlayer_SimplePlayer

˵����
      ������ȡָ�������б������������������

������
      pi:IMusicPlayer *����ָ��
      musicName:����·���ͺ�׺���ļ���,�ɵ����ߴ���
      
����ֵ��
    ����ֵͬISHELL_StartAppletArgs�ķ���ֵ

��ע��:
        ��
        
==============================================================================*/

static int IMusicPlayer_SimplePlayer(IMusicPlayer *pi,char *musicName)
{
    CMusicPlayer *pMe=(CMusicPlayer *)pi;
    char     *args = NULL;
    int       nRet;
    
    if ( pMe == NULL ) 
    {
        return EFAILED;
    }
    args=(char *)MALLOC((1+MP3NAME_MAX_LEN)*sizeof(char));
    args[0]=STARTARGS_SIMPLEPLAYER;
    STRCPY(&args[1],musicName);
    nRet = ISHELL_StartAppletArgs(  pMe->m_pShell,
                                    AEECLSID_APP_MUSICPLAYER, 
                                    args);
    if (args) 
    {
        FREE(args);
    }
    
    return nRet;
}
/*==============================================================================
������ IMusicPlayer_PedometerCallPlayer

˵����
      ������ȡָ�������б������������������

������
      pi:IMusicPlayer *����ָ��
      musicName:����·���ͺ�׺���ļ���,�ɵ����ߴ���
      
����ֵ��
    ����ֵͬISHELL_StartAppletArgs�ķ���ֵ

��ע��:
        ��
        
==============================================================================*/
static int IMusicPlayer_PedometerCallPlayer(IMusicPlayer *pi)
{
    CMusicPlayer *pMe=(CMusicPlayer *)pi;
    char     *args = NULL;
    int       nRet;
    
    if ( pMe == NULL ) 
    {
        return EFAILED;
    }
    args=(char *)MALLOC(2*sizeof(char));
    args[0]=STARTARGS_PEDOMETERCALLPLAYER;
    args[1]='\0';
    nRet = ISHELL_StartBackgroundApplet( pMe->m_pShell,
                                        AEECLSID_APP_MUSICPLAYER, 
                                        args);
    if (args) 
    {
        FREE(args);
    }
    
    return nRet;
}

/*==============================================================================
������ CMusicPlayer_ReadMusiclist

˵����
      ������ȡָ�������б������������������

������
      pMe:CMusicPlayer *����ָ��
      curfileName:������·���ͺ�׺���ļ���
      isPlaying:TRUE:�Ѵ��ļ��ж�ȡ�����ݷŵ�pMe->m_PlayingMusiclist��
                FALSE:�Ѵ��ļ��ж�ȡ�����ݴ�ŵ�pMe->m_Musiclist��

����ֵ��
    SUCCESS:��ȡ�ɹ�
    EFAILED:��ȡʧ��

��ע��:
        ��
        
==============================================================================*/
int CMusicPlayer_ReadMusiclist(CMusicPlayer *pMe , char *curFileName,boolean isPlaying)
{
    IFile  *pFile;
    char fileName[MP3_MAX_FILE_NAME];
    if(pMe==NULL||curFileName==NULL)
    {
     return EFAILED;
    }
    (void)STRCPY(fileName,MUSICPLAYLIST_DIR);
    (void)STRCAT(fileName,"/");
    (void)STRCAT(fileName,curFileName);
    (void)STRCAT(fileName,".txt");
    pFile = IFILEMGR_OpenFile(pMe->m_pFileMgr, fileName, _OFM_READ);
    if(pFile == NULL)
    {
        ERR("FILE IS NULL",0,0,0);
        // ��δ������ʹ��Ĭ��ֵ
        return EFAILED;
    } //else ���ļ�����ִ��

    // ���ļ���ʼ��ȡ����
    if(SUCCESS != IFILE_Seek(pFile, _SEEK_START, 0))
    {
        (void)IFILE_Release(pFile);
        pFile = NULL;
        return EFAILED;
    }
    if(TRUE == isPlaying)
    {
      if(0==IFILE_Read(pFile, pMe->m_PlayingMusiclist, sizeof(pMe->m_PlayingMusiclist)))
      {
        pMe->m_nPlayinglistMusicNum = 0;
        (void)IFILE_Release(pFile);
        pFile = NULL;
        return EFAILED;
      }
      else
      {
        CMusicPlayer_PlayingMusiclistSortBy(pMe);
      }
    }
    else
    {
      if(IFILE_Read(pFile, pMe->m_Musiclist, sizeof(pMe->m_Musiclist))==0)
      {
        pMe->m_nPlaylistMusicNum = 0;
        (void)IFILE_Release(pFile);
        pFile = NULL;
        return EFAILED;
      }
      else
      {
        CMusicPlayer_MusiclistSortBy(pMe);
      }
    }
    (void)IFILE_Release(pFile);
    pFile = NULL;
    return SUCCESS;
}
/*==============================================================================
������ CMusicPlayer_WriteMusiclist

˵����
       ����д��ָ�������б������������������

������
      pMe:CMusicPlayer *����ָ��
      curfileName:������·���ͺ�׺���ļ���

����ֵ��
    SUCCESS:��ȡ�ɹ�
    EFAILED:��ȡʧ��

��ע��:
        ��
        
==============================================================================*/
int CMusicPlayer_WriteMusiclist(CMusicPlayer *pMe , char *curFileName)
{
    IFile  *pFile;
    char fileName[MP3_MAX_FILE_NAME];
    if(pMe==NULL||fileName==NULL)
    {
    return EFAILED;
    }
    (void)STRCPY(fileName,MUSICPLAYLIST_DIR);
    (void)STRCAT(fileName,"/");
    (void)STRCAT(fileName,curFileName);
    (void)STRCAT(fileName,".txt");

    pFile = IFILEMGR_OpenFile(pMe->m_pFileMgr, fileName, _OFM_READWRITE);
    if(pFile == NULL)
    {
        ERR("FILE IS NULL",0,0,0);
        // ��δ������ʹ��Ĭ��ֵ
        return EFAILED;
    } //else ���ļ�����ִ��

    // ���ļ���ͷд������
    if(SUCCESS != IFILE_Seek(pFile, _SEEK_START, 0))
    {
        (void)IFILE_Release(pFile);
        pFile = NULL;
        return EFAILED;
    }
    (void) IFILE_Write(pFile, pMe->m_Musiclist, sizeof(pMe->m_Musiclist));
    (void)IFILE_Release(pFile);
    pFile = NULL;
    return SUCCESS;
}
/*==============================================================================
����:
       CMusicPlayer_APPIsReadyTimer

˵��:
       ��ʱ���ص���������Ҫ���ڿ��ƿ��ٰ�������Ĵ���

����:
       pme [in]��void *����ָ�룬��ָCMusicPlayer�ṹָ�롣

����ֵ:
       �ޡ�

��ע:

==============================================================================*/
static void CMusicPlayer_AppIsReadyCB(void * pUser)
{
    CMusicPlayer *pMe = (CMusicPlayer *)pUser;

    if (NULL == pMe)
    {
        return;
    }
    
    pMe->m_bAppReady = TRUE;
}

/*�жϴ�����*/
static void MP3_InterruptHandle(CMusicPlayer *pMe)
{
    if(!pMe)
    {
        return;
    }
    g_nInterruptRef ++;
    DBGPRINTF("MP3_InterruptHandle_g_nInterruptRef is %d",g_nInterruptRef);
    if(g_nInterruptRef > 1)
    {
        return;
    }
#ifdef FEATURE_SUPPORT_BT_APP
    if(pMe->m_bUseBT)
    {
#ifndef WIN32
        bcmapp_ag_audio_close(pMe->m_nBTID);
        pMe->m_nBTID = BCMAPP_AG_BAD_APPID;
#endif
        pMe->m_bInterruptByBT = TRUE;
        SetIsPlayMP3ByBt(FALSE);
    }
#endif
    if(pMe->m_eStartMethod == STARTMETHOD_SIMPLEPLAYER)
    {
        //ISHELL_CancelTimer(pMe->m_pShell,(PFNNOTIFY)MP3_MusicNameAutoScroll,pMe);
        (void)ISHELL_CancelTimer(pMe->m_pShell, NULL, pMe);
        CMusicPlayer_ReleaseMedia(pMe);
        pMe->m_bPaused = FALSE;
        pMe->m_bPlaying = FALSE;
    }
    else
    {
        //ISHELL_CancelTimer(pMe->m_pShell,(PFNNOTIFY)MP3_MusicNameAutoScroll,pMe);
        (void)ISHELL_CancelTimer(pMe->m_pShell, NULL, pMe);
        CMusicPlayer_ReleaseMedia(pMe);
    }
}

/*�жϻָ�����*/
static void MP3_ResumeHandle(CMusicPlayer *pMe)
{
    if(!pMe)
    {
        return;
    }
    g_nInterruptRef --;
    DBGPRINTF("MP3_ResumeHandle_g_nInterruptRef is %d",g_nInterruptRef);
    if(g_nInterruptRef > 0)
    {
        return;
    }
    else
    {
        g_nInterruptRef = 0;
    }
    pMe->m_bInterrupt = TRUE;
    MP3_CloseKeyBeepVol(pMe);  //close keybeepvol again ��ֹ�����ڽ������ð����������
    Music_Shake_Open(); // open again if be closed by other app
    if(pMe->m_eStartMethod == STARTMETHOD_SIMPLEPLAYER)
    {
      return;   
    }
#ifdef FEATURE_SUPPORT_BT_APP
    if(pMe->m_bUseBT)
    {
        pMe->m_bInterruptByBT = FALSE;
        SetIsPlayMP3ByBt(TRUE);
#ifndef WIN32
        pMe->m_nBTID = bcmapp_is_ag_connected();
        if(BCMAPP_AG_BAD_APPID == pMe->m_nBTID)
        {
           (void)ISHELL_PostEvent(pMe->m_pShell,
                                  AEECLSID_APP_MUSICPLAYER,
                                  EVT_POPMSGDIALOG,
                                  IDS_MSG_UNCONNECT_BT,
                                  0);
        }
        else
        {
           (void)bcmapp_ag_audio_open(pMe->m_nBTID, 
                                (PFN_AG_NOTIFY)MP3_PlayMusicByBTCallBack,
                                pMe, 
                                pMe->m_MusicPlayerCfg.eMusicVolume,
                                pMe->m_bUse848);
        }
#endif
    }
#endif
    CMusicPlayer_ReleaseMedia(pMe);
    ISHELL_SetTimer(pMe->m_pShell,300,(PFNNOTIFY)CMusicPlayer_InitMusic,pMe);
    if(pMe->m_bPlaying || pMe->m_bPaused)
    {
      ISHELL_SetTimer(pMe->m_pShell,800,(PFNNOTIFY) CMusicPlayer_PlayMusic,pMe);
      ISHELL_SetTimer(pMe->m_pShell,1000, (PFNNOTIFY)CMusicPlayer_SeekMusic,pMe);
    }
    if(pMe->m_bPaused)
     {
        ISHELL_SetTimer(pMe->m_pShell,1100, (PFNNOTIFY)CMusicPlayer_PauseMusic,pMe);
     }
}
/*�رռ�����*/
static void MP3_CloseKeyBeepVol(CMusicPlayer *pMe)
{
    byte mute = OEMSOUND_MUTE_VOL;
    byte curBeepVol = 0;
    ICONFIG_GetItem( pMe->m_pConfig, CFGI_BEEP_VOL, &curBeepVol, sizeof(byte));
    if(curBeepVol !=0)
    {
        ICONFIG_GetItem( pMe->m_pConfig, CFGI_BEEP_VOL, &pMe->m_keyBeepVol, sizeof(byte));
        ICONFIG_SetItem( pMe->m_pConfig, CFGI_BEEP_VOL, &mute, sizeof(byte));
    }
}

/*�ָ�������*/
static void MP3_ResumeKeyBeepVol(CMusicPlayer * pMe)
{
    ICONFIG_SetItem( pMe->m_pConfig, CFGI_BEEP_VOL, &pMe->m_keyBeepVol, sizeof(byte));
}

extern boolean is_alarm_time_reach();
#ifdef FEATURE_SPORTS_APP
extern boolean is_g_sportsapp_pedometer_bground_flag();
#endif

void MP3_InitMusicCB(CMusicPlayer *pMe)
{
    if(pMe == NULL)
    {
        return;
    }
    Music_Shake_Open(); // open again ,because FM able close it
    MP3_CloseKeyBeepVol(pMe); // close again ,because FM able resume KeyBeeVol
#ifdef FEATURE_SUPPORT_BT_APP
    if(pMe->m_bStartApp)
    {
        char *            pf = NULL;

#ifndef WIN32
        pMe->m_nBTID = bcmapp_is_ag_connected();
        if(BCMAPP_AG_BAD_APPID != pMe->m_nBTID)
        {
            pf = STRRCHR(pMe->m_PlayingMusiclist[pMe->m_MusicPlayerCfg.lastPlayMusicID].pMusicName, '.');
            pf++;
            if(0 == STRICMP(pf,MP3_TYPE))
            {
               pMe->m_bUse848 = TRUE;
            }
            else
            {
                pMe->m_bUse848 = FALSE;
            }
            (void)bcmapp_ag_audio_open(pMe->m_nBTID, 
                                        (PFN_AG_NOTIFY)MP3_PlayMusicByBTCallBack,
                                        pMe, 
                                        pMe->m_MusicPlayerCfg.eMusicVolume,
                                        pMe->m_bUse848); 
            pMe->m_bUseBT = TRUE;
            SetIsPlayMP3ByBt(TRUE);
        }
#endif
    }
#endif
   //�ж��Ƿ��ǵ�һ��ʹ�ò�����
   if(FALSE==CMusicPlayer_InitMusic(pMe))
   {
     if(pMe->m_bStartApp)
     {
        CMusicPlayer_ReleaseMedia(pMe);
        (void)STRCPY(pMe->m_MusicPlayerCfg.lastPlayPlaylist,DEFAULT_PLAYLIST);
        CMusicPlayer_ReadMusiclist(pMe,
                                   pMe->m_MusicPlayerCfg.lastPlayPlaylist,
                                   TRUE);
        pMe->m_MusicPlayerCfg.lastPlayMusicID=0;
  
        (void)CMusicPlayer_InitMusic(pMe);
     }
     else if((GetMp3PlayerStatus() == MP3STATUS_RUNONFOREGROUND && pMe->m_eCurState == STATE_PLAYMUSIC_WINDOWS)
           || (pMe->m_eStartMethod == STARTMETHOD_PEDOMETERCALLPLAYER && pMe->m_bStartApp)) 
     {
      // if(!pMe->m_bStartApp)
       //{
            pMe->m_bPlaying = FALSE;
            pMe->m_bPaused = FALSE;
            (void)ISHELL_PostEvent(pMe->m_pShell, AEECLSID_APP_MUSICPLAYER, EVT_POPMSGDIALOG, IDS_MSG_ERR_FORMAT, 0);
            //���Ի�Ĭ�����ֵĲ���
            CMusicPlayer_ReleaseMedia(pMe);
            pMe->m_MusicPlayerCfg.lastPlayPlaylist[0]='\0';
            (void)STRCPY(pMe->m_PlayingMusiclist[0].pMusicName,"1fs:/hsmm/music/Away.mp3");
            pMe->m_MusicPlayerCfg.lastPlayMusicID=0;
            pMe->m_nPlayinglistMusicNum = 1;
        (void)CMusicPlayer_InitMusic(pMe);
        if(pMe->m_eStartMethod != STARTMETHOD_PEDOMETERCALLPLAYER)
        {
           ISHELL_CancelTimer(pMe->m_pShell,(PFNNOTIFY) CMusicPlayer_PlayMusic,pMe);
        }
    }
    else if(GetMp3PlayerStatus() == MP3STATUS_RUNONBACKGROUND && pMe->m_MusicPlayerCfg.eMusicPlayMode == PLAYMODE_ORDER
            && pMe->m_MusicPlayerCfg.lastPlayMusicID == pMe->m_nPlayinglistMusicNum - 1)
    {
        (void)ISHELL_PostEvent(pMe->m_pShell, AEECLSID_APP_MUSICPLAYER, EVT_CLOSEAPP, 0, 0);
    }
    else
    {
        (void)ISHELL_PostEvent(pMe->m_pShell, AEECLSID_APP_MUSICPLAYER, EVT_GSENSOR_SHAKE, AVK_GSENSOR_BACKWARD, 0);
    }
   }
   pMe->m_bStartApp = FALSE;
}
/*decode app startargs*/
static void MP3_DecodeStartArgs(CMusicPlayer *pMe, char *args)
{
    if(NULL == pMe || NULL == args)
    {
        DBGPRINTF("args is NULL");
        return;
    }
    DBGPRINTF("args is %s",args);
    switch(args[0])
    {
        case STARTARGS_SIMPLEPLAYER:
            pMe->m_eStartMethod=STARTMETHOD_SIMPLEPLAYER;
            pMe->m_pSimplePlayMusicName=(char *)MALLOC(MP3NAME_MAX_LEN*sizeof(char));
            (void)STRCPY(pMe->m_pSimplePlayMusicName,&args[1]);
            break;

       case STARTARGS_PEDOMETERCALLPLAYER:
           pMe->m_eStartMethod = STARTMETHOD_PEDOMETERCALLPLAYER;
           break;

       default:
          break;
    }
}
/*set app start curState*/
static void MP3_SetStartCurState(CMusicPlayer *pMe)
{
    if(NULL == pMe)
    {
        return;
    }
    if(GetMp3PlayerStatus()==MP3STATUS_RUNONBACKGROUND)
    {   
        pMe->m_eStartMethod = STARTMETHOD_NORMAL;
        pMe->m_eCurState = STATE_PLAYMUSIC_WINDOWS;
    }
#ifndef WIN32
    else if(APP_MEDIA_IMPACT_BY_FM == app_media_scheduler())
    {
        pMe->m_eCurState = STATE_MSGFMBGPLAYPROMPT;
    }
#endif//WIN32
    else
    {
        pMe->m_eCurState = STATE_INIT;
    }
    
}
/*set app start status*/
static void MP3_SetStartStatus(CMusicPlayer *pMe)
{
    if(NULL == pMe)
    {
        return;
    }
    if(STARTMETHOD_PEDOMETERCALLPLAYER == pMe->m_eStartMethod)
    {
        SetMp3PlayerStatus(pMe,MP3STATUS_RUNONBACKGROUND);
    }
    else
    {
      SetMp3PlayerStatus(pMe,MP3STATUS_RUNONFOREGROUND);
    }
}

static void Music_Shake_Open(void)
{
#ifdef FEATRUE_SUPPORT_G_SENSOR
    dword shake;
    //g_bMp3PlayerRun = TRUE;
    if((MMI_GSENSOR_SHAKE_OPEN == mmi_g_sensor_state) 
        ||(MMI_GSENSOR_SHAKE_OPEN_IN_IDLE == mmi_g_sensor_state))   //return if sensor has been open.
    {
        return;
    }
    if(is_g_sportsapp_pedometer_bground_flag() == FALSE)
    {
        OEM_GetConfig(CFGI_GSENSOR,&shake,sizeof(shake));
        if(shake & OEMNV_SHAKE_MUSIC_MASK)
        {
#ifdef FEATRUE_SUPPORT_G_SENSOR
#ifndef WIN32//wlh ��ʱ
            mmi_g_sensor_process(G_SENSOR_ENABLE | G_SENSOR_SHAKE);
#endif//WIN32
#endif
            mmi_g_sensor_state = MMI_GSENSOR_SHAKE_OPEN;
        }    
    }
#endif
}

static void Music_Shake_Close(void)
{
    dword shake;
#ifdef FEATRUE_SUPPORT_G_SENSOR
     //g_bMp3PlayerRun = FALSE;
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
        //if(shake & OEMNV_SHAKE_MUSIC_MASK) 
        {
#ifdef FEATRUE_SUPPORT_G_SENSOR
#ifndef WIN32//wlh ��ʱ
            mmi_g_sensor_process(G_SENSOR_SHAKE_DISABLE);
#endif//WIN32
#endif
            mmi_g_sensor_state = MMI_GSENSOR_SHAKE_CLOSE;
        }            
    }
    #endif
}

Mp3Player_Status GetMp3PlayerStatus(void)
{
    return g_eMp3PlayerStatus;
}

boolean IsMp3PlayerStatusNone(void)
{
    return (g_eMp3PlayerStatus == MP3STATUS_NONE);
}

boolean IsMp3PlayerStatusOnBG(void)
{
    return (g_eMp3PlayerStatus == MP3STATUS_RUNONBACKGROUND);
}


void SetMp3PlayerStatus(CMusicPlayer *pMe,Mp3Player_Status eSt)
{
    pMe->m_ePreMp3Status = g_eMp3PlayerStatus;
    g_eMp3PlayerStatus = eSt;
}

void MP3_ResetRandonIdentifier(CMusicPlayer *pMe)
{
    MEMSET(pMe->m_aIdentifier,0,MUSIC_MAX_FILESNUM * sizeof(boolean));
    pMe->m_aIdentifier[pMe->m_MusicPlayerCfg.lastPlayMusicID] = 1;
    pMe->m_nRandNum = 1;   
}

AEERect *GetPlayerSkinRect(CPlayerSkin ePlayerSkin)
{
    switch(ePlayerSkin)
    {
        case PLAYERSKIN1:
            return MP3SKIN1_RECT;
    }
	return NULL;
}
#ifdef FEATURE_SUPPORT_BT_APP
boolean GetIsPlayMP3ByBt(void)
{
    return g_bPlayMP3ByBt;
}
void SetIsPlayMP3ByBt(boolean bPlayMp3ByBt)
{
    g_bPlayMP3ByBt = bPlayMp3ByBt;
}
#endif

//extern boolean CMediaGallery_GetTflashStatus(void);
static void MP3_Build_DefaultPlaylist(CMusicPlayer *pMe)
{
    FileName szFileName;
    IFile *pFile;
    AEEFileInfoEx  fi;
    char destFileName[PLAYLISTNAME_MAX_LEN];
    
    ASSERT(pMe != NULL); 
    if(pMe == NULL||pMe->m_pFileMgr==NULL)
    {
     return;
    }
    // �����Ŀ¼�����ڴ���Ŀ¼
    if(SUCCESS != IFILEMGR_Test(pMe->m_pFileMgr, MUSICPLAYLIST_DIR))
    {
     (void)IFILEMGR_MkDir(pMe->m_pFileMgr, MUSICPLAYLIST_DIR);
    }
    (void)STRCPY(destFileName,MUSICPLAYLIST_DIR);
    (void)STRCAT(destFileName,"/");
    (void)STRCAT(destFileName,DEFAULT_PLAYLIST);
    (void)STRCAT(destFileName,".txt");
    //(void)STRCPY(destFileName,DEFAULT_PLAYLIST);
    // �����ļ��Ƿ���ڣ��������򴴽���
    if(SUCCESS != IFILEMGR_Test(pMe->m_pFileMgr, destFileName))
    {
        pFile=IFILEMGR_OpenFile(pMe->m_pFileMgr, destFileName, _OFM_CREATE);
        if(NULL != pFile)
        {
          IFILE_Release(pFile);
          pFile = NULL;
        }
    }

    // ��ʼ�������
   // ���fi�ṹ��
   MEMSET(&fi, 0, sizeof(fi));
   fi.nStructSize = sizeof(fi);
   fi.pszFile = szFileName.m_szFileName;
   fi.nMaxFile = sizeof(szFileName.m_szFileName);   

	//��ǰ����T��ʱĬ�ϼ��صľ�ֻ��T����������ļ���û��T��ʱ��ֻ����PHONE��������ļ�
	//������PHONE + T��(�����)

   (void)IFILEMGR_EnumInit(pMe->m_pFileMgr, MP3_PHONE_MUSIC_DIR, FALSE);

   pMe->m_nPlaylistMusicNum = 0;

   while (IFILEMGR_EnumNextEx(pMe->m_pFileMgr, &fi))
   {
	   char *			 psz = NULL;
	   char *			 pf = NULL;
	   
	   if(pMe->m_nPlaylistMusicNum >= MUSIC_MAX_FILESNUM)
	   {
		 break;
	   }
	   psz = STRRCHR(fi.pszFile, '.');
	   if (NULL == psz)
	   {
		 continue; 
	   }
	   psz++;
	   
	   pf = STRRCHR(fi.pszFile, '/');
	   if (NULL == pf)
	   {
		 continue;
	   }  
	   pf++;
	  
	   if ((*psz) && (*pf) && (0 == STRICMP(psz,MP3_TYPE) || 0 == STRICMP(psz,MIDI_TYPE)) 
		   && pMe->m_nPlaylistMusicNum < MUSIC_MAX_FILESNUM)
	   {					 
		 pMe->m_Musiclist[pMe->m_nPlaylistMusicNum].pMusicName[0]=(pMe->m_nPlaylistMusicNum+48);
		 STRLCPY(pMe->m_Musiclist[pMe->m_nPlaylistMusicNum].pMusicName+1,
				 fi.pszFile,MP3_MAX_FILE_NAME * sizeof(char));
		 pMe->m_nPlaylistMusicNum++;
		  continue; 
	   }
	   else
	   {					
		 continue;
	   }
   }
   
    //ö��playlist�ļ��� 
    if(CMediaGallery_GetTflashStatus())		//��T ��ʱ
    {
       (void)IFILEMGR_EnumInit(pMe->m_pFileMgr, MP3_SD_MUSIC_DIR, FALSE);

	     while (IFILEMGR_EnumNextEx(pMe->m_pFileMgr, &fi))
	     {
	          char *            psz = NULL;
	          char *            pf = NULL;
	          
	          if(pMe->m_nPlaylistMusicNum >= MUSIC_MAX_FILESNUM)
	          {
	            break;
	          }
	          psz = STRRCHR(fi.pszFile, '.');
	          if (NULL == psz)
	          {
	            continue; 
	          }
	          psz++;
	          
	          pf = STRRCHR(fi.pszFile, '/');
	          if (NULL == pf)
	          {
	            continue;
	          }  
	          pf++;
	          // �����musicplayer ֧�ֵ��ļ���ӽ���(��֧�ָ�����Ҫ�ڴ����)
	          if ((*psz) && (*pf) && (0 == STRICMP(psz,MP3_TYPE) || 0 == STRICMP(psz,MIDI_TYPE)) 
	              && pMe->m_nPlaylistMusicNum < MUSIC_MAX_FILESNUM)
	          {
	            //Ϊ�˱�����Ӳ����б��и�����˳�������õ�һλ���洢�ø��������˳��������'0'��ʼ��50���ַ���ʾ
	            // PS:ѡ��ĸö��ַ���ò��ܺ���'/''.'�������ַ�
	            pMe->m_Musiclist[pMe->m_nPlaylistMusicNum].pMusicName[0]=(pMe->m_nPlaylistMusicNum+48);
	            STRLCPY(pMe->m_Musiclist[pMe->m_nPlaylistMusicNum].pMusicName+1,
	                    fi.pszFile,MP3_MAX_FILE_NAME * sizeof(char));
	            pMe->m_nPlaylistMusicNum++;
	             continue; 
	          }
	          else
	          {
	            // ������ǲ�֧�ֵ��ļ������ö����һ���ļ�
	            continue;
	          }
	     }
    }
   
     CMusicPlayer_WriteMusiclist(pMe,DEFAULT_PLAYLIST);
     (void)CMediaGallery_GetTflashStatus();
}
