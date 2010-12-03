/*=================================================================================================================
// �ļ���VideoPlayer.c
//        
//	  ��Ȩ����(c) 2004 Anylook
//	
//
// �ļ�˵����
//	  
//        
// ���ߣ�Gemsea
// �������ڣ�2008/10/14
=================================================================================================================*/

/*=================================================================================================================
�޸���ʷ��¼��
ʱ       ��      �� �� ��     �޸����ݡ�λ�ü�ԭ��
-----------      ----------     ------------------------------------------------
2008/10/14                    ��ʼ�汾
=================================================================================================================*/


/*=================================================================================================================
                         ���ļ��������ⲿ�ļ�
=================================================================================================================*/

#include "VideoPlayer_priv.h"      
#include "AEE_OEMEvent.h"

/*=================================================================================================================
              ���壨�궨�弰����������ǰ�������ͳ���
=================================================================================================================*/

/*=================================================================================================================
                                ���Ͷ���
=================================================================================================================*/

/*================================================================================================================
                                  ��������                                            
=================================================================================================================*/

int VideoPlayerMod_Load(IShell *ps, void * pHelpers, IModule ** pMod);

static int  VideoPlayer_CreateInstance(AEECLSID ClsId,IShell *pIShell,IModule *po,void **ppObj);

static int VideoPlayer_New(IShell * pIShell,IModule * po,void ** ppObj);

static uint32  VideoPlayer_AddRef(IVideoPlayer *pi);

static uint32  VideoPlayer_Release (IVideoPlayer *pi);

static boolean VideoPlayer_HandleEvent(IVideoPlayer *pi,AEEEvent eCode,uint16 wParam,uint32  dwParam);

int VideoPlayer_Play(IVideoPlayer *pi,char *VideoName);

static int VideoPlayer_InitAppData(CVideoPlayer *pMe);

static void VideoPlayer_FreeAppData(CVideoPlayer *pMe);

static void VideoPlayer_RunFSM(CVideoPlayer *pMe);

//��ʼ�������ȼ�
static void VideoPlayer_InitVolumeLevel(CVideoPlayer *pMe);
// YY TODO:
//extern int Rendering_UpdateEx(void);//wlh 20090409 add
/*=================================================================================================================
                                 ȫ������
=================================================================================================================*/


/*=================================================================================================================
                                 ���أ���̬������
=================================================================================================================*/
// ֻ����һ��VideoPlayerʵ����ÿ�δ���VideoPlayer Appletʱ��
// ����ͬһ�ṹָ���BREW�㡣
static CVideoPlayer gVideoPlayer;

//static const VTBL(IVideoPlayer) gVideoPlayerMethods = CONTAPP_METHODS;


static const VTBL(IVideoPlayer) gVideoPlayerMethods =
{
    VideoPlayer_AddRef,
    VideoPlayer_Release,
    VideoPlayer_HandleEvent,    
    VideoPlayer_Play   
};

boolean videoplayer_play_flag = FALSE;

/*----------------------ģ����غ�������---------------------*/
/*=================================================================================================================
����:
       VideoPlayerMod_Load

˵��:
       ģ��װ�غ�����

����:
       pIShell [in]��IShell�ӿ�ָ�롣
       ph ��û��ʹ��
       ppMod [out]��

����ֵ:
       int

��ע:

=================================================================================================================*/
#if defined(AEE_STATIC)
int VideoPlayerMod_Load(IShell *ps, void * pHelpers, IModule ** pMod)
{
    return(AEEStaticMod_New((int16)(sizeof(AEEMod)),
                             ps,
                             pHelpers,
                             pMod,
                             (PFNMODCREATEINST)VideoPlayer_CreateInstance,
                             (PFNFREEMODDATA)NULL));
}
#endif

/*=================================================================================================================
����:
       VideoPlayerMod_CreateInstance

˵��:
        ͨ������(��ʼ��)VideoPlayer����������ģ��ʵ����

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

=================================================================================================================*/
#if defined(AEE_STATIC)
static int VideoPlayer_CreateInstance(AEECLSID  ClsId,IShell *pIShell,IModule  *po,void  **ppObj)
#else
int AEEClsCreateInstance(AEECLSID ClsId,IShell  * pIShell, IModule * po,void  ** ppObj)
#endif
{
    *ppObj = NULL;

    if(ClsId != AEECLSID_VIDEOPLAYER)
    {
        return EFAILED;
    }

    if(VideoPlayer_New(pIShell, po, ppObj) != SUCCESS)
    {
        return EFAILED;
    }
    return SUCCESS;
}
/*----------------------Applet��غ�������---------------------*/
/*=================================================================================================================
����:
       VideoPlayer_New

˵��:
        ��ʼ��VideoPlayer Applet�������ڲ����ݱ�����ͬʱ�����ü�����1��

����:
       ps [in]��IShell�ӿڶ���ָ�롣
       pIModule [in]��pIModule�ӿڶ���ָ�롣
       ppObj [out]:��ָ��IClockApps����ָ���ָ�롣

����ֵ:
       EFAILED��ʧ�ܡ�
       SUCCESS���ɹ���

��ע:
       �����û�ȥ���á�
       ����Applet�ṹ�����ʹ�ö�̬�����ڴ�ķ�ʽ�������������õ���ʹ�þ�̬
       �����ķ�ʽ������Խṹ�Ƚϴ��Appletʹ�þ�̬�����ķ�ʽ�����ڶ�̬����
       �ڴ�ķ�ʽ��Ҫע���������ռ���ͷš�

=================================================================================================================*/
static int VideoPlayer_New(IShell *ps, IModule *pIModule, void **ppObj)
{
    CVideoPlayer *pMe;
    int retVal = SUCCESS;
   
    if(!ppObj || !ps || !pIModule)
    {      
        return EFAILED;
    }
#if defined(AEE_STATIC)
    pMe = &gVideoPlayer;
#else
    pMe = (CVideoPlayer *)MALLOC(sizeof(CVideoPlayer) + sizeof(VTBL(IVideoPlayer)));
#endif
    if (NULL == pMe)
    {
        return EFAILED;
    }
    if (0 == pMe->m_nRefs)
    {
    #if defined(AEE_STATIC)
        // Must initialize the object
        MEMSET(pMe,  0, sizeof(CVideoPlayer));
        INIT_VTBL(pMe, IVideoPlayer, gVideoPlayerMethods);
    #else
        //NOTE: In the dynamic applet, the extend classes will re-malloc the memery.
        //      So it run in simulator for test only. If you want run in simulator 
        //      correcttly Please modify the following code��       
        VTBL(IVideoPlayer) *pMod = (VTBL(IVideoPlayer) *)((byte *)pMe + sizeof(CVideoPlayer));
        MEMCPY((byte *)pMod, (byte *)&gVideoPlayerMethods, sizeof(VTBL(IVideoPlayer)));
        INIT_VTBL( pMe,IVideoPlayer,*pMod);
    #endif

        pMe->m_nRefs     = 0;
        pMe->m_pShell    = ps;
        pMe->m_pModule   = pIModule;
        (void) ISHELL_AddRef(pMe->m_pShell);
        (void) IMODULE_AddRef(pMe->m_pModule);

        retVal = VideoPlayer_InitAppData(pMe);
        if( SUCCESS != retVal )
        {
            VideoPlayer_FreeAppData(pMe);
            return retVal;
        }
    }
    
    ++pMe->m_nRefs;
    *ppObj = (IVideoPlayer*)pMe;

    return retVal;
}

/*=================================================================================================================
����:
       VideoPlayer_AddRef

˵��:
       VideoPlayer Applet ���ü�����1�������ص�ǰ����������

����:
       pi [in]��IVideoPlayer�ӿڶ���ָ�롣

����ֵ:
       ��ǰ��VideoPlayer Applet��������

��ע:

=================================================================================================================*/
static uint32  VideoPlayer_AddRef(IVideoPlayer *pi)
{
    CVideoPlayer *pMe = (CVideoPlayer*)pi;

    ASSERT(pMe != NULL);
    ASSERT(pMe->m_nRefs > 0);
    //ERR("m_nRefs+ IS = %d",pMe->m_nRefs,0,0);

    return (++pMe->m_nRefs);
}

/*=================================================================================================================
����:
       VideoPlayer_Release

˵��:
       �ͷŶ�VideoPlayer Applet�����ã����ü�����1�������ص�ǰ����������
        �����ǰ��������Ϊ�㣬�����ͷ�Applet�������Դ��

����:
       pi [in]��IVideoPlayer�ӿڶ���ָ�롣

����ֵ:
       ��ǰ��VideoPlayer Applet��������

��ע:

=================================================================================================================*/
static uint32  VideoPlayer_Release (IVideoPlayer *pi)
{
    register CVideoPlayer *pMe = (CVideoPlayer*)pi;

    ASSERT(pMe != NULL);

    if(pMe->m_nRefs == 0)
    {
        return 0;
    }

    if(--pMe->m_nRefs)
    {
        return pMe->m_nRefs;
    }

    // �ͷ�Applet�����Դ
    VideoPlayer_FreeAppData(pMe);

    (void) ISHELL_Release(pMe->m_pShell);
    (void) IMODULE_Release(pMe->m_pModule);

    // ע�⣺pMe�Ǿ�̬����ռ䣬��˲���Ҫ�ͷš�FREE()
    #if !defined(AEE_STATIC)
    (void)FREE(pMe); 
    #endif
    return 0;
}

/*=================================================================================================================
����:
       VideoPlayer_InitAppData

˵��:
       ��ʼ��VideoPlayer Applet�ṹ����ȱʡֵ��

����:
       pMe [in]��ָ��VideoPlayer Applet����ṹ��ָ�롣�ýṹ����С������ض���Ϣ��

����ֵ:
       SUCCESS����ʼ���ɹ���
       EFAILED����ʼ��ʧ�ܡ�

��ע:

=================================================================================================================*/
static int VideoPlayer_InitAppData(CVideoPlayer *pMe)
{
    AEEDeviceInfo  di;
    
#if defined(AEE_STATIC)
    ASSERT(pMe != NULL);
#endif
    ISHELL_GetDeviceInfo(pMe->m_pShell, &di);
   
    pMe->m_rc.x         = 0;
    pMe->m_rc.y         = 0;
    pMe->m_rc.dx        = (int16) di.cxScreen;
    pMe->m_rc.dy        = (int16) di.cyScreen;
    
    pMe->m_ePreState    = STATE_NONE;
    pMe->m_eCurState    = STATE_INIT;
    pMe->m_pActiveDlg   = NULL;
    pMe->m_pActiveDlgID = IDD_VIDEOPLAYER;
    pMe->m_eDlgRet      = DLGRET_CREATE;
    pMe->m_bNotOverwriteDlgRet = FALSE;
    pMe->m_bAppIsReady  = FALSE;
    pMe->m_pImage       = NULL;
    pMe->m_pMedia       = NULL;
    pMe->m_pMenuCtl     = NULL;
    pMe->m_FileToPlay   = NULL;
    pMe->m_bSuspending  = FALSE;
    pMe->totalvolume    = 0;

    pMe->UserStop       = FALSE; 
    pMe->PauseLock      = FALSE; 
    pMe->bCurrentTime   = 0;
    pMe->bTotalTime     = 0;
    pMe->m_IsPlaynext   = FALSE;
    pMe->IsPause        = FALSE;
    pMe->IsFullScreen   = FALSE;
    pMe->IsPlay         = FALSE; 
    pMe->IsGallery      = FALSE; 
    pMe->Is848Busy      = FALSE;
	pMe->m_InitFailed   = TRUE;
   
    //������Ҫ�Ľӿ�
    if (AEE_SUCCESS != ISHELL_CreateInstance(pMe->m_pShell,AEECLSID_CONFIG,(void **)&pMe->m_pConfig))
    {
        VideoPlayer_FreeAppData(pMe);                                        
        return EFAILED;                             
    }
    
    /* ��Ϊ848�����������⣬����ڽ����ý��ģ��󲻽�ֹ����������ᵼ�½ϳ�ʱ�������ͣ��, 
       ���û��������Ӱ��.�������ӽ���ģ��ʱ��ֹ���������˳�ʱ�ָ�����*/       
    (void)ICONFIG_GetItem(pMe->m_pConfig,CFGI_BEEP_VOL, &pMe->m_KeySound,sizeof(byte));    
    pMe->m_CKSound= pMe->m_KeySound;
    if(pMe->m_KeySound != 0)
    {  
        pMe->m_KeySound=0;
        (void)ICONFIG_SetItem(pMe->m_pConfig,CFGI_BEEP_VOL,&pMe->m_KeySound ,sizeof(byte));              
    }
    //���ó�ʼ����
    VideoPlayer_InitVolumeLevel(pMe);
    return SUCCESS;
}

/*=================================================================================================================
����:
       VideoPlayer_FreeAppData

˵��:
       �ͷ�VideoPlayer Appletʹ�õ������Դ��

����:
       pMe [in]��ָ��VideoPlayer Applet����ṹ��ָ�롣�ýṹ����С������ض���Ϣ��

����ֵ:
       �ޡ�

��ע:

=================================================================================================================*/
static void VideoPlayer_FreeAppData(CVideoPlayer *pMe)
{
   
    if (NULL == pMe)
    {
        return ;
    }
    //�ָ�������
    (void)ICONFIG_SetItem(pMe->m_pConfig,CFGI_BEEP_VOL,&pMe->m_CKSound,sizeof(byte));    
    
    if (pMe->m_pConfig)
    {
        ICONFIG_Release(pMe->m_pConfig);
        pMe->m_pConfig = NULL;
    }

    FREEIF(pMe->m_FileToPlay);
    pMe->m_FileToPlay=NULL;      
    if(NULL != pMe->m_pImage)
    {       
        IIMAGE_Release(pMe->m_pImage);
        pMe->m_pImage = NULL;
    }
    
    VideoPlayer_ReleaseVideo(pMe); 
}

/*=================================================================================================================
����:
       VideoPlayer_RunFSM

˵��:
       VideoPlayer Applet����״̬�����档

����:
       pMe [in]��ָ��VideoPlayer Applet����ṹ��ָ�롣�ýṹ����С������ض���Ϣ��

����ֵ:
       �ޡ�

��ע:

=================================================================================================================*/
static void VideoPlayer_RunFSM(CVideoPlayer *pMe)
{
    NextFSMAction nextFSMAction = NFSMACTION_WAIT;

    for ( ; ; )
    {
     
        nextFSMAction = VideoPlayer_ProcessState(pMe);

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

/*=================================================================================================================
����:
       VideoPlayer_HandleEvent

˵��:
       VideoPlayer  Applet�¼��������������������ڱ�Applet���¼���
       �������ɸú�������

����:
       pi [in]��ָ��IVideoPlayer�ӿڶ����ָ�롣
       eCode��BREW�¼����롣
       wParam�����¼�����������ݡ�
       dwParam [in]�����¼�����������ݡ�

����ֵ:
       TRUE�������¼�������
       FALSE�������¼�û������

��ע:

=================================================================================================================*/
#ifdef FEATRUE_SUPPORT_G_SENSOR
#include "g_sensor.h"
#endif

static boolean VideoPlayer_HandleEvent(IVideoPlayer *pi, AEEEvent  eCode, uint16  wParam,uint32 dwParam)
{
    CVideoPlayer *pMe = (CVideoPlayer*)pi;
    AEEAppStart  *as;
  
    switch (eCode)
    {
        case EVT_APP_START:
		// YY TODO:
			//Rendering_UpdateEx();//wlh add for 3D test
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
            pMe->m_bSuspending = FALSE;
            pMe->m_bActive = TRUE; 
            pMe->m_bAppIsReady  = FALSE;
            SetDeviceState(DEVICE_TYPE_MP4,DEVICE_MP4_STATE_OFF);
            if(app_media_scheduler() != APP_MEDIA_ALLOW)
            {
                pMe->Is848Busy = TRUE;
            }
            else
            {
                if(pMe->m_pMedia == NULL)
                {
#ifndef WIN32
                    if(as->pszArgs[0]==STARTARGS_GALLERY)
                    {
#endif//WIN32
                        pMe->IsGallery=TRUE;
                        pMe->IsPlay  = TRUE;
                        videoplayer_play_flag = TRUE;
#ifndef WIN32                        
                        pMe->m_FileToPlay=(char *)MALLOC(MAX_STR_LEN*sizeof(char));
                        (void)STRCPY(pMe->m_FileToPlay,&as->pszArgs[1]); 
                        //��ʼ��Mediaָ�벢ע��ص�
                        VideoPlayer_InitVideo(pMe); 
            	    }
#else
	//					VideoPlayer_InitVideo(pMe); 
#endif//WIN32
                }
            }
            {
                dword shake;
                OEM_GetConfig(CFGI_GSENSOR,&shake,sizeof(shake));
                if(shake & OEMNV_SHAKE_VIDEO_MASK)
                {
#ifdef FEATRUE_SUPPORT_G_SENSOR
                    mmi_g_sensor_process(G_SENSOR_ENABLE | G_SENSOR_VIDEO_CIRC);
#endif
                }
            }
            // ��ʼVideoPlayer״̬��           
            VideoPlayer_RunFSM(pMe);            
            return TRUE;         


        case EVT_APP_STOP: 
            SetDeviceState(DEVICE_TYPE_MP4,DEVICE_MP4_STATE_OFF);
             //�ָ�������
            (void)ICONFIG_SetItem(pMe->m_pConfig,CFGI_BEEP_VOL,&pMe->m_CKSound,sizeof(byte));
            (void)ISHELL_CancelTimer(pMe->m_pShell, NULL, pMe);
            pMe->m_bAppIsReady  = FALSE;
             videoplayer_play_flag = FALSE;
            if (pMe->m_pDisplay != NULL)
            {
                (void) IDISPLAY_Release(pMe->m_pDisplay);
                pMe->m_pDisplay = NULL;
            }
            pMe->m_bSuspending = TRUE; 
            {
                dword shake;
                OEM_GetConfig(CFGI_GSENSOR,&shake,sizeof(shake));             
                if(shake & OEMNV_SHAKE_VIDEO_MASK)
                {                    
#ifdef FEATRUE_SUPPORT_G_SENSOR
                    mmi_g_sensor_process(G_SENSOR_VIDEO_CIRC_DISABLE);
#endif
                }
            }
            return TRUE;

        case EVT_APP_SUSPEND:  
            SetDeviceState(DEVICE_TYPE_MP4,DEVICE_MP4_STATE_OFF);
            (void)ICONFIG_SetItem(pMe->m_pConfig,CFGI_BEEP_VOL,&pMe->m_CKSound,sizeof(byte));
            pMe->m_bSuspending = TRUE;                               
            (void)ISHELL_CancelTimer(pMe->m_pShell, NULL, pMe);
            
            //�ж���ز���(�Զ��ػ�,�͵����ػ�,����)            
            if(pMe->m_pMedia != NULL && !pMe->UserStop)
            {
                if(pMe->IsFullScreen) // (����ʱ)ȫ��������ص�����ģʽ
                {
                    VideoPlayer_PlayMod(pMe, wParam);
                }
                if(pMe->IsPlay)
                {
                    IMEDIA_Pause((IMedia*)pMe->m_pMedia);
                }
                IMedia_Stop((IMedia*)pMe->m_pMedia);
            }
            //�ж���ز���(�Զ��ػ�,�͵����ػ�,����)
            return TRUE;

        case EVT_APP_RESUME:  
            SetDeviceState(DEVICE_TYPE_MP4,DEVICE_MP4_STATE_OFF);           
            (void)ICONFIG_SetItem(pMe->m_pConfig,CFGI_BEEP_VOL,&pMe->m_KeySound,sizeof(byte));
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
 #if 0        
            if(pMe->m_eCurState != STATE_PLAYER)
            {
                pMe->m_eCurState=STATE_PLAYER;
            } 
#endif      
            //�ж���ز���(�Զ��ػ�,�͵����ػ�,����)
            if(pMe->IsPause || pMe->IsPlay)
            {
                IMEDIA_Play((IMedia*)pMe->m_pMedia);
                IMEDIA_FastForward((IMedia*)pMe->m_pMedia,pMe->m_bPauseTime);
                IMEDIA_Pause((IMedia*)pMe->m_pMedia);
            }
            //�ж���ز���(�Զ��ػ�,�͵����ػ�,����)
            if(pMe->m_pMedia == NULL)// ��ֹ�жϻָ�ʱ���ܵ�����
            {
                if(as->pszArgs[0]==STARTARGS_GALLERY)
                {           
                    pMe->IsGallery=TRUE;
                    pMe->IsPlay  = TRUE;
                    videoplayer_play_flag = TRUE;
                    pMe->m_FileToPlay=(char *)MALLOC(MAX_STR_LEN*sizeof(char));
                    (void)STRCPY(pMe->m_FileToPlay,&as->pszArgs[1]); 
                    //��ʼ��Mediaָ�벢ע��ص�
                    VideoPlayer_InitVideo(pMe);
                }
                else if(pMe->m_FileToPlay != NULL)
                {       
                    pMe->IsGallery=TRUE;
                    pMe->IsPlay  = TRUE;                  
                    pMe->m_IsPlaynext =TRUE;
                    
                    //��ʼ��Mediaָ�벢ע��ص�
                    VideoPlayer_InitVideo(pMe);                
                }
            }
 
            VideoPlayer_RunFSM(pMe);
            return TRUE;       

        case EVT_DIALOG_INIT:
            pMe->m_pActiveDlg = (IDialog*)dwParam;
            pMe->m_pActiveDlgID = wParam;
            pMe->m_bAppIsReady  = FALSE;
            return VideoPlayer_RouteDialogEvent(pMe,eCode,wParam,dwParam);

        case EVT_DIALOG_START:
			// YY TODO:
			//Rendering_UpdateEx();//wlh add for 3D test
            return VideoPlayer_RouteDialogEvent(pMe,eCode,wParam,dwParam);

        case EVT_KEY:
        case EVT_KEY_PRESS:
        case  EVT_KEY_RELEASE:
        case EVT_COMMAND:            
        case EVT_USER_REDRAW:            
            return VideoPlayer_RouteDialogEvent(pMe,eCode,wParam,dwParam); 
            
        case EVT_PLAYNEXT: 
            if(&(char*)dwParam !=NULL)
            {
                /*��videoplayer�����galleryѡ����Ƶ���ţ�����ֹͣ���ٽ���gallery��
                  ѡ����Ƶ����յ�playnext�¼������ͷ�ǰһ��pMe->m_FileToPlay��pMe->m_pMedia*/
                FREEIF(pMe->m_FileToPlay);
                pMe->m_FileToPlay=NULL;
                VideoPlayer_ReleaseVideo(pMe);
                pMe->m_FileToPlay=STRDUP((char*)dwParam); 
            }            
            return TRUE;
            
        case EVT_AUTO_POWERDOWN:           
            IMEDIA_Pause((IMedia*)pMe->m_pMedia);
            pMe->IsPlay = FALSE;
            pMe->IsPause = TRUE;
            videoplayer_play_flag = FALSE;
            return  TRUE;

        case EVT_DIALOG_END:   
            DBGPRINTF("EVT_DIALOG_END");
            if (wParam == 0)
            {                
                return TRUE;
            }
            pMe->m_bAppIsReady  = FALSE;
            (void) VideoPlayer_RouteDialogEvent(pMe,eCode,wParam,dwParam);
            pMe->m_pActiveDlg = NULL;

            // Applet�����𣬲���״̬�����ȴ�Applet���ء���ע�⣺EVT_APP_SUSPEND
            // �¼���EVT_DIALOG_END�¼�ǰ������)
            if (pMe->m_bSuspending == FALSE)
            {
                // ��ʼVideoPlayer״̬��
                VideoPlayer_RunFSM(pMe);
            }
            return TRUE;
        case EVT_APP_EXIT:
            ISHELL_CloseApplet(pMe->m_pShell, TRUE);
            return TRUE;
#ifdef FEATURE_LCD_TOUCH_ENABLE//WLH ADD FOR LCD TOUCH
	  case EVT_PEN_UP:
		  {
			  int i;
			  AEEDeviceInfo devinfo;
			  int nBarH ;
			  AEERect rc;
			  int16 wXPos = (int16)AEE_GET_X((const char *)dwParam);
			  int16 wYPos = (int16)AEE_GET_Y((const char *)dwParam);



			  //����
			  SETAEERECT(&rc,VIDEOPLAYER_PLAY_X,VIDEOPLAYER_PLAY_Y,VIDEOPLAYER_PLAY_W, VIDEOPLAYER_PLAY_H);
			  if(VIDEOPLAYER_PT_IN_RECT(wXPos,wYPos,rc))
			  {
				  return  ISHELL_PostEvent(pMe->m_pShell,AEECLSID_VIDEOPLAYER,EVT_USER,AVK_INFO,0);
			  }
			  //���
			  SETAEERECT(&rc,VIDEOPLAYER_FORWARD_X,VIDEOPLAYER_FORWARD_Y,VIDEOPLAYER_FORWARD_W, VIDEOPLAYER_FORWARD_H);
			  if(VIDEOPLAYER_PT_IN_RECT(wXPos,wYPos,rc))
			  {
				  return  ISHELL_PostEvent(pMe->m_pShell,AEECLSID_VIDEOPLAYER,EVT_USER,AVK_POUND,0);
			  }
			  //����
			  SETAEERECT(&rc,VIDEOPLAYER_REWIND_X,VIDEOPLAYER_REWIND_Y,VIDEOPLAYER_REWIND_W, VIDEOPLAYER_REWIND_H);
			  if(VIDEOPLAYER_PT_IN_RECT(wXPos,wYPos,rc))
			  {
				  return  ISHELL_PostEvent(pMe->m_pShell,AEECLSID_VIDEOPLAYER,EVT_USER,AVK_STAR,0);
			  }
			  //ǰһ��
			  SETAEERECT(&rc,VIDEOPLAYER_PREVIOUS_X,VIDEOPLAYER_PREVIOUS_Y,VIDEOPLAYER_PREVIOUS_W, VIDEOPLAYER_PREVIOUS_H);
			  if(VIDEOPLAYER_PT_IN_RECT(wXPos,wYPos,rc))
			  {
				  return  ISHELL_PostEvent(pMe->m_pShell,AEECLSID_VIDEOPLAYER,EVT_USER,AVK_LEFT,0);
			  }
			  //��һ��
			  SETAEERECT(&rc,VIDEOPLAYER_NEXT_X,VIDEOPLAYER_NEXT_Y,VIDEOPLAYER_NEXT_W, VIDEOPLAYER_NEXT_H);
			  if(VIDEOPLAYER_PT_IN_RECT(wXPos,wYPos,rc))
			  {
				  return  ISHELL_PostEvent(pMe->m_pShell,AEECLSID_VIDEOPLAYER,EVT_USER,AVK_RIGHT,0);
			  }
			  //��������
			  SETAEERECT(&rc,VIDEOPLAYER_VOLUME_ADD_X,VIDEOPLAYER_VOLUME_ADD_Y,VIDEOPLAYER_VOLUME_ADD_W, VIDEOPLAYER_VOLUME_ADD_H);
			  if(VIDEOPLAYER_PT_IN_RECT(wXPos,wYPos,rc))
			  {
				  return  ISHELL_PostEvent(pMe->m_pShell,AEECLSID_VIDEOPLAYER,EVT_USER,AVK_UP,0);
			  }
			  //��������
			  SETAEERECT(&rc,VIDEOPLAYER_VOLUME_LOW_X,VIDEOPLAYER_VOLUME_LOW_Y,VIDEOPLAYER_VOLUME_LOW_W, VIDEOPLAYER_VOLUME_LOW_H);
			  if(VIDEOPLAYER_PT_IN_RECT(wXPos,wYPos,rc))
			  {
				  return  ISHELL_PostEvent(pMe->m_pShell,AEECLSID_VIDEOPLAYER,EVT_USER,AVK_DOWN,0);
			  }
			  //ȫ��
			  //SETAEERECT(&rc,VIDEOPLAYER_FULLSCREEN_X,VIDEOPLAYER_FULLSCREEN_Y,VIDEOPLAYER_FULLSCREEN_W, VIDEOPLAYER_FULLSCREEN_H);
			  //if(VIDEOPLAYER_PT_IN_RECT(wXPos,wYPos,rc))
			  //{
			//	  return  ISHELL_PostEvent(pMe->m_pShell,AEECLSID_VIDEOPLAYER,EVT_USER,AVK_INFO,0);
			  //}
			  //�ײ�������
			  nBarH = GetBottomBarHeight(pMe->m_pDisplay);
			  MEMSET(&devinfo, 0, sizeof(devinfo));
			  ISHELL_GetDeviceInfo(pMe->m_pShell, &devinfo);
			  SETAEERECT(&rc, 0, devinfo.cyScreen-nBarH, devinfo.cxScreen, devinfo.cyScreen);
			  if ((wXPos>0)&&(wXPos<devinfo.cxScreen/3)&&(wYPos>rc.y)&&(wYPos<devinfo.cyScreen))
			  {
				  boolean rt =  ISHELL_PostEvent(pMe->m_pShell,AEECLSID_VIDEOPLAYER,EVT_USER,AVK_SELECT,1);
				  return rt;
			  }
			  else if ((wXPos>devinfo.cxScreen/3)&&(wXPos<(devinfo.cxScreen/3)*2)&&(wYPos>rc.y)&&(wYPos<devinfo.cyScreen))
			  {
				  boolean rt =  ISHELL_PostEvent(pMe->m_pShell,AEECLSID_VIDEOPLAYER,EVT_USER,AVK_INFO,0);
				  return rt;
			  }
			  else if ((wXPos>wXPos<(devinfo.cxScreen/3)*2)&&(wXPos<devinfo.cxScreen)&&(wYPos>rc.y)&&(wYPos<devinfo.cyScreen))
			  {
				  boolean rt =  ISHELL_PostEvent(pMe->m_pShell,AEECLSID_VIDEOPLAYER,EVT_USER,AVK_CLR,0);
				  return rt;
			  }
		  }
		  return TRUE;
	  case EVT_USER:
		  {
				eCode = EVT_KEY;
		  }
		  return VideoPlayer_RouteDialogEvent(pMe,eCode,wParam,dwParam); 
#endif//FEATURE_LCD_TOUCH_ENABLE            
        default:            
            // �����յ����¼�·������ǰ��ĶԻ����¼���������
            return VideoPlayer_RouteDialogEvent(pMe,eCode,wParam,dwParam);
    }
}
/*=================================================================================================================
������VideoPlayer_ReleaseVideo

˵����
       �ͷ�Mediaָ��

������
       pMe [in]��ָ��VideooPlayer Applet����ṹ��ָ�롣�ýṹ����С������ض���Ϣ��
       
����ֵ��
        ��

��ע��:

=================================================================================================================*/
void VideoPlayer_ReleaseVideo(CVideoPlayer *pMe)
{
    if(pMe->m_pMedia)
    {
        (void)IMEDIA_RegisterNotify((IMedia*)pMe->m_pMedia, NULL, pMe);
        IMEDIA_Release((IMedia*)pMe->m_pMedia);
        pMe->m_pMedia = NULL;
    }
} 
/*=================================================================================================================
����: VideoPlayer_Play

˵��:
      �ṩ��GALLERY���õ��ⲿ�ӿ�,����ѡ�����Ƶ     
      
����:
����ֵ:
��ע:
=================================================================================================================*/
int VideoPlayer_Play(IVideoPlayer *pi,char *VideoName)
{
    CVideoPlayer *pMe=(CVideoPlayer *)pi;
    char     *args = NULL;
    int       nRet;
    
    if ( pMe == NULL ) 
    {
        return EFAILED;
    }
    args=(char *)MALLOC((1+MAX_STR_LEN)*sizeof(char));
    args[0]=STARTARGS_GALLERY;
    STRCPY(&args[1],VideoName);
	
    nRet = ISHELL_StartAppletArgs(pMe->m_pShell,AEECLSID_VIDEOPLAYER,args);   
    if (args) 
    {
        FREE(args);
    }   
    
    return nRet;
}

/*=================================================================================================================
   ��ʼ�������ȼ��������뵱ǰϵͳ����һ��
=================================================================================================================*/ 
static void VideoPlayer_InitVolumeLevel(CVideoPlayer *pMe)
{
    pMe->m_bVolumeLevel = VOLUME_THREE;
    pMe->totalvolume = pMe->m_bVolumeLevel*VP_VOLUME_STEP;
} 

/*=================================================================================================================
����: 
       VideoPlayer_APPIsReadyTimer
       
˵��: 
       ��ʱ���ص���������Ҫ���ڿ��ƿ��ٰ�������Ĵ���
       
����: 
       pMe [in]��void *����ָ��
       
����ֵ:
       �ޡ�
       
��ע:
       
=================================================================================================================*/
void VideoPlayer_APPIsReadyTimer(void *pme)
{
    CVideoPlayer *pMe = (CVideoPlayer *)pme;

    if (NULL == pMe)
    {
        return;
    }

    (void) ISHELL_PostEvent( pMe->m_pShell,
                            AEECLSID_VIDEOPLAYER,
                            EVT_APPISREADY,
                            0,
                            0);
}
boolean is_videoplayer_play(void)
{
    return videoplayer_play_flag;
}
