/*=================================================================================================================
�ļ���
      VideoPlayerDialogHandler.c
      ��Ȩ����(c) 2009 Anylook Tech. CO.,LTD.

 �ļ�˵����
 ����:  
 �������ڣ�2008-10-06
 ��ǰ�汾��Draft
=================================================================================================================*/

/*=================================================================================================================
�޸���ʷ��¼��
ʱ       ��      �� �� ��    ���ⵥ��  �޸����ݡ�λ�ü�ԭ��
-----------      ----------  --------  -----------------------------------------
=================================================================================================================*/


/*=================================================================================================================
                          ���ļ��������ⲿ�ļ�
=================================================================================================================*/
#include "VideoPlayer_priv.h"
#ifndef WIN32
//#include "mmd_ply_driver.h"
#else
#include "oemhelperfunctype.h"
#endif//WIN32
#ifdef FEATURE_PROJECT_FLEXI203
	#include "appscommon_images160x128_Flexi203.brh"
#else
	#include "appscommonimages.brh"
#endif
//#include "appscommon_color.brh"
#include "appscommon.brh"


boolean        FullScreen = FALSE;
/*=================================================================================================================
                             �궨��ͳ���
=================================================================================================================*/
// �رնԻ���
#define CLOSE_DIALOG(DlgRet)  {    \
                                     pMe->m_eDlgRet = DlgRet; \
                                     (void) ISHELL_EndDialog(pMe->m_pShell);  \
                                 }


/*=================================================================================================================
                             ���Ͷ���
=================================================================================================================*/


/*=================================================================================================================
                              ��������
=================================================================================================================*/

/*=================================================================================================================
                �Ի���IDD_VIDEOPLAYER�¼�������
=================================================================================================================*/

static boolean VPDVideoPlayer_HandleEvent(CVideoPlayer *pMe,AEEEvent eCode, uint16 wParam,uint32 dwParam); 

/*=================================================================================================================
                   �Ի���IDD_OPTION�¼�������
=================================================================================================================*/

//static boolean VPDOption_HandleEvent(CVideoPlayer *pMe, AEEEvent eCode, uint16 wParam, uint32 dwParam); 

/*=================================================================================================================
                   �Ի���IDD_MSGBOX�¼�������
=================================================================================================================*/

static boolean VPDMSGBOX_HandleEvent(CVideoPlayer *pMe,AEEEvent eCode,uint16 wParam,uint32 dwParam);

/*=================================================================================================================
                             ��Ƶ������ش���
=================================================================================================================*/

//�Ի���IDD_VIDEOPLAYER key�¼�������
static boolean VPDVideoPlayer_HandleKeyEvent(CVideoPlayer *pMe,AEEEvent eCode,uint16 wParam,uint32 dwParam);

//�Ի���IDD_OPTION key�¼�������
//static boolean VPDOption_HandleKeyEvent(CVideoPlayer *pMe,AEEEvent eCode,uint16 wParam,uint32 dwParam);

//�Ի���IDD_MSGBOX key�¼�������
//static boolean VPDMSGBOX_HandleKeyEvent(CVideoPlayer *pMe,AEEEvent eCode,uint16 wParam,uint32 dwParam);

//������Ƶ 
void VideoPlayer_PlayVideo(CVideoPlayer *pMe);

//����Ƶ���������ͼƬ
static void VideoPlayer_DrawImage(CVideoPlayer *pMe, char *resFile, int16 resId, int x, int y);

//������һ���ļ�
static void VideoPlayer_PlayNext(CVideoPlayer *pMe, boolean bDirection); 

//�ػ�player�����ϰ벿�ֵ��ļ���
static void VideoPlayer_RefreshPlayerFileName(CVideoPlayer *pMe);
static void VideoPlayer_ShowName(CVideoPlayer *pMe);

//�ػ�player�����°벿�ֵİ�ť����
static void VideoPlayer_UpdateButton(CVideoPlayer *pMe);
static void VideoPlayer_UpdateFRButton(CVideoPlayer *pMe);

//ˢ�²���ʱ��
static void VideoPlayer_RefreshPlayingTick(CVideoPlayer *pMe);

//ˢ������bar
static void VideoPlayer_RefreshVolBar(CVideoPlayer *pMe);

//ˢ�½�����
static void VideoPlayer_RefreshScheduleBar(CVideoPlayer *pMe);

// ɾ����ǰ�ļ�
//static boolean VideoPlayer_DeleteVideo(CVideoPlayer *pMe);

// ����CMX�ײ�ص�
static void VideoPlayer_VideoNotify(void * pUser, AEEMediaCmdNotify * pCmdNotify);

// ��ʱ����
static void DialogTimeoutCallback(void *pUser);

// ��ȡָ����Ƶ������
static int  VideoPlayer_GetFileID(CVideoPlayer *pMe);
static boolean VideoPlayer_ChangeScrState(CVideoPlayer* pMe,boolean isLockScr);

// ������
static void VideoPlayer_WriteTitleRes(CVideoPlayer *pMe,char* pResPath,int uResId);
static void VideoPlayer_WriteTitle(CVideoPlayer *pMe,AECHAR* pText);

// ����
static void VideoPlayer_Help(CVideoPlayer* pMe);
static boolean VideoPlayer_HelpInit(CVideoPlayer* pMe);
static void VideoPlayer_HelpTerminate(CVideoPlayer *pMe);
static boolean VideoPlayer_HelpHandleEvent(CVideoPlayer *pMe,AEEEvent eCode,uint16 wParam,uint32 dwParam);
static void VideoPlayer_DrawHelp(CVideoPlayer *pMe);
static uint16 VideoPlayer_SearchKey(uint16 wParam);





/*=================================================================================================================
                              ȫ������
=================================================================================================================*/

/*=================================================================================================================
                             ���أ���̬������
=================================================================================================================*/


/*=================================================================================================================
                                ��������
=================================================================================================================*/

/*=================================================================================================================
������VideoPlayer_ShowDialog

˵����
       ������״̬����������������Ӧ�Ի�����ԴID�ĶԻ���

������
       pMe [in]��ָ��VideoPlayer Applet����ṹ��ָ�롣�ýṹ����С������ض���Ϣ��
       dlgResId [in]���Ի�����ԴID���������ݸ�ID�����Ի���

����ֵ��
        ��

��ע��:

=================================================================================================================*/
int VideoPlayer_ShowDialog(CVideoPlayer *pMe , uint16 dlgResId)
{
    int nRet;
    
       // At most one dialog open at once
       if (ISHELL_GetActiveDialog(pMe->m_pShell) != NULL)
       {
           // Looks like there is one dialog already opened.
           // Flag an error an return without doing anything.
   #if defined(AEE_STATIC)
           ERR("Trying to create a dialog without closing the previous one",0,0,0);
   #endif
           return  EFAILED;
       } 
       
       nRet = ISHELL_CreateDialog(pMe->m_pShell,AEE_APPSVIDEOPLAYER_RES_FILE,dlgResId,NULL);    
    
   #if defined(AEE_STATIC)
           if (nRet != SUCCESS)
           {
               ERR("Failed to create the dialog in the videoplayer applet",0,0,0);
           }
   #endif
           return  nRet;

}

/*=================================================================================================================
����:
    VideoPlayer_ShowMsgBox

˵��:
    ������״̬����������������Ϣ�Ի�����ʾ��Ϣ��ʾ�û���

����:
    pMe [in]: ָ�� VIDEOPLAYER Applet ����ṹ��ָ�롣�ýṹ����С������ض���Ϣ��
    wTextResId [in]: ��Ϣ�ı���Դ ID��

����ֵ:
    none

��ע:

=================================================================================================================*/
int  VideoPlayer_ShowMsgBox(CVideoPlayer *pMe, uint16 wTextResId)
{
    pMe->m_nMsgResID = wTextResId;
     
    // ������Ϣ�Ի���.
    return  VideoPlayer_ShowDialog(pMe, IDD_MSGBOX);
}

/*=================================================================================================================
������VideoPlayer_RouteDialogEvent

˵����
       ������BREW�¼�·�ɸ���ǰ��Ի�����¼���������

������
       pMe [in]��ָ��VideoPlayer Applet����ṹ��ָ�롣�ýṹ����С������ض���Ϣ��
       eCode [in]���¼����롣
       dwParam [in]�����¼�eCode���������ݡ���

����ֵ��
        TRUE�������¼�������
        FALSE�������¼�û������

��ע��:

=================================================================================================================*/
boolean VideoPlayer_RouteDialogEvent( CVideoPlayer *pMe,AEEEvent eCode,uint16 wParam,uint32 dwParam)
{
    #if defined(AEE_STATIC)
        if (NULL == pMe)
        {
            return FALSE;
        }
    #endif
    
    if (NULL == pMe->m_pActiveDlg)//��ǰ��Ի���IDialog�ӿ�ָ��
    {
        return FALSE;
    }
    switch (pMe->m_pActiveDlgID)
    {     
        case IDD_VIDEOPLAYER:
            return VPDVideoPlayer_HandleEvent(pMe,eCode,wParam,dwParam);

        case IDD_MSGBOX:
            return VPDMSGBOX_HandleEvent(pMe,eCode,wParam,dwParam);
#if 0
        case IDD_OPTION:
            return VPDOption_HandleEvent(pMe,eCode,wParam,dwParam);
#endif  
        default:
            return FALSE;
     }
}




/*=================================================================================================================
������VPDVideoPlayer_HandleEvent

˵����
       VideoPlayer Applet ������Ի���IDD_VIDEOPLAYER�¼���������

������
       pMe [in]��ָ��VideoPlayer Applet����ṹ��ָ�롣�ýṹ����С������ض���Ϣ��
       eCode [in]���¼����롣
       wParam���¼�����
       dwParam [in]����wParam���������ݡ���

����ֵ��
       TRUE�������¼�������
       FALSE�������¼�û������

��ע��
=================================================================================================================*/
static  boolean VPDVideoPlayer_HandleEvent(CVideoPlayer *pMe,AEEEvent eCode,uint16 wParam,uint32 dwParam)
{   
    #if defined(AEE_STATIC)
        ASSERT(pMe != NULL);
    #endif
	DBGPRINTF("eCode = %d",eCode);
#ifdef VIDEOPLAYER_HELP
	if(pMe->m_pHelp != NULL)
	{
	   boolean nReturn = FALSE;
	   nReturn = VideoPlayer_HelpHandleEvent(pMe,eCode,wParam,dwParam);

	   if(nReturn) return TRUE;
	}
#endif

    switch (eCode)
    {
        case EVT_DIALOG_INIT:        
            return TRUE;
        
        case EVT_DIALOG_START:
            (void) ISHELL_PostEvent(pMe->m_pShell,AEECLSID_VIDEOPLAYER,EVT_USER_REDRAW,0,0);                                   
            return TRUE;
        
        case EVT_USER_REDRAW:
			{
#if 0			
            if(!pMe->IsPlay)
            {
                VideoPlayer_DrawImage(pMe,VIDEOPLAYER_IMAGES_RES_FILE,IDI_PLAYERPICTURE_PLAY, 0, 0);                 
            }
            else
            {					
                VideoPlayer_DrawImage(pMe,VIDEOPLAYER_IMAGES_RES_FILE,IDI_PLAYERPICTURE_PAUSE, 0, 0);                
            } 
#endif		
            AEERect rc = {0,0,SCR_W,SCR_H};         
            IDisplay_FillRect(pMe->m_pDisplay,&rc,0x0);
			VideoPlayer_WriteTitleRes(pMe,AEE_APPSVIDEOPLAYER_RES_FILE,IDS_TITLE);
			VideoPlayer_DrawImage(pMe,VIDEOPLAYER_IMAGES_RES_FILE,IDI_LOGO, VIDEOPLAYER_LOGO_X, VIDEOPLAYER_LOGO_Y);
            if(pMe->m_FileToPlay != NULL) // ������Ӵ��ж�����������pMe->m_FileToPlayΪNULLʱ����Ļ�ϻ���ʾ����
            {
                VideoPlayer_RefreshPlayerFileName(pMe); //ˢ���ļ���
            }
			VideoPlayer_RefreshVolBar(pMe);
            if(!pMe->IsPause && !pMe->IsPlay)                 
            {
                if(!pMe->UserStop)
                {
                    DRAW_BOTTOMBAR(BTBAR_GALLERY_BACK);
                }
                else
                {
                    if(pMe->m_IsPlaynext)
                    {
                        DRAW_BOTTOMBAR(BTBAR_GALLERY_PLAY_BACK);
                    }
                    else
                    {
                        DRAW_BOTTOMBAR(BTBAR_PLAY_BACK);
                    }
                }
            }
            if(pMe->IsPause)
            {
                DRAW_BOTTOMBAR(BTBAR_PLAY_STOP);
            }
            if(pMe->IsPlay)
            {
                DRAW_BOTTOMBAR(BTBAR_FULLSCREEN_PAUSE_STOP);
            } 
			VideoPlayer_RefreshPlayingTick(pMe);
            IDISPLAY_Update(pMe->m_pDisplay); 
            if(pMe->Is848Busy)
            {
                CLOSE_DIALOG(DLGRET_MSGBOX);
            }          
            if(pMe->IsGallery)
            {
                if(pMe->m_InitFailed != SUCCESS )
                {                    
                    CLOSE_DIALOG(DLGRET_MSGBOX);
                }
                else if(pMe->IsPlay)
                { 
                    VideoPlayer_PlayVideo(pMe);
                }
            } 
            return TRUE;
        	}
  
        case EVT_DIALOG_END:            
            return TRUE;
        
        case EVT_KEY: 
            if(pMe->IsGallery)
            {
                return  VPDVideoPlayer_HandleKeyEvent(pMe,eCode,wParam,dwParam); 
            }
            else 
            {
                switch(wParam)
                {
                    case AVK_SELECT:
                        #if 0
                        //����Mdeida Gallery applet                   
                        if (!ISHELL_CanStartApplet(pMe->m_pShell, AEECLSID_MEDIAGALLERY)) 
                        {
                            //��ǰ��������ָ�� Applet
                            DBGPRINTF("ERR!!!!!");
                            return FALSE;
                        }
                        if (SUCCESS != ISHELL_StartApplet(pMe->m_pShell, AEECLSID_MEDIAGALLERY)) 
                        {
                            DBGPRINTF("RIGHT~~~~~~");
                            return FALSE;
                        }
                        #endif
                        CMediaGallery_FileExplorer(GALLERY_VIDEO_ADDLIST,NULL);
                        return TRUE;
#ifdef VIDEOPLAYER_HELP
					case AVK_INFO:
						{
							VideoPlayer_Help(pMe);
							return TRUE;
						}
#endif
                    case AVK_CLR:
                        CLOSE_DIALOG(DLGRET_CANCELED);
                        break;
                        
                    default:
                        return TRUE;
                }
                     
            }

        case EVT_APPISREADY: 
            pMe->m_bAppIsReady  = TRUE;
            return TRUE;
           
        case EVT_DISPLAYDIALOGTIMEOUT:
            CLOSE_DIALOG(DLGRET_CANCELED);
            return TRUE;
           
        case EVT_COMMAND:
            return TRUE;
        case EVT_NO_CLOSEBACKLIGHT:
            if(pMe->IsPlay)
            {
                return TRUE;
            }
            break;
#ifdef FEATURE_LCD_TOUCH_ENABLE//WLH ADD FOR LCD TOUCH
		case EVT_PEN_UP:
			{
				int i;
				AEEDeviceInfo devinfo;
				int nBarH ;
				AEERect rc;
				int16 wXPos = (int16)AEE_GET_X((const char *)dwParam);
				int16 wYPos = (int16)AEE_GET_Y((const char *)dwParam);
				
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
#endif//FEATURE_LCD_TOUCH_ENABLE       
        default:
               break;
    }//switch (eCode)
        return FALSE;
}

/*=================================================================================================================

������VPDOption_HandleEvent

˵����
       VideoPlayer Applet ������Ի���IDD_OPTION�¼���������

������
       pMe [in]��ָ��VideoPlayer Applet����ṹ��ָ�롣�ýṹ����С������ض���Ϣ��
       eCode [in]���¼����롣
       wParam���¼�����
       dwParam [in]����wParam���������ݡ���

����ֵ��
        TRUE�������¼�������
        FALSE�������¼�û������

��ע��:

================================================================================================================*/
#if 0
static boolean VPDOption_HandleEvent(CVideoPlayer *pMe,AEEEvent eCode,uint16 wParam,uint32 dwParam)
{   
    
    IMenuCtl* pMenuOption = (IMenuCtl*)IDIALOG_GetControl(pMe->m_pActiveDlg,
                                                          IDC_OPTION_MENU);
    if( pMenuOption == NULL)
    {
        return FALSE;
    }

    switch (eCode)
    {     
        case EVT_DIALOG_INIT:  
            if(pMe->IsPlay)
            {
                IMENUCTL_AddItem(pMenuOption,AEE_APPSCOMMONRES_LANGFILE,IDS_PAUSE, IDS_PAUSE,0,0);
                IMENUCTL_AddItem(pMenuOption,AEE_APPSCOMMONRES_LANGFILE,IDS_FULLSCREEN,IDS_FULLSCREEN,0,0);
            }
            if(pMe->IsPause)
            {
                IMENUCTL_AddItem(pMenuOption,AEE_APPSCOMMONRES_LANGFILE,IDS_PLAY,IDS_PLAY,0,0); 
            }
            IMENUCTL_AddItem(pMenuOption,AEE_APPSVIDEOPLAYER_RES_FILE,IDS_DELETE, IDS_DELETE,0,0);
            
            return TRUE;
         
        case EVT_DIALOG_START:        
            IMENUCTL_SetPopMenuRect(pMenuOption);
            IMENUCTL_SetBottomBarType(pMenuOption, BTBAR_SELECT_BACK);
            (void) ISHELL_PostEvent(pMe->m_pShell,AEECLSID_VIDEOPLAYER,EVT_USER_REDRAW,0,0);
            return TRUE;

        case EVT_USER_REDRAW:
            (void)IMENUCTL_Redraw( pMenuOption);       
            return TRUE;

        case EVT_DIALOG_END:
            return TRUE;

        case EVT_KEY:            
            return VPDOption_HandleKeyEvent(pMe,eCode,wParam,dwParam); 

        case EVT_DISPLAYDIALOGTIMEOUT:
            CLOSE_DIALOG(DLGRET_CANCELED);
            return TRUE;

        //�Զ���ؼ��¼� ,�˵�����ʱ����
        case EVT_COMMAND:        
            pMe->m_nItemID=wParam;
            switch(wParam)
            {      
                case IDS_PLAY:
                    if(SUCCESS==pMe->m_InitFailed)
                    {    
                        VideoPlayer_DrawImage(pMe,VIDEOPLAYER_IMAGES_RES_FILE, IDI_PAUSE, 82, 179); //"||" 
                        //ˢ����Ļ������ʱ��ͽ�����
                        //VideoPlayer_RefreshPlayingTick(pMe);//ˢ��ʱ��
                        //VideoPlayer_RefreshScheduleBar(pMe);//ˢ�½�����                                       
                        CLOSE_DIALOG(DLGRET_CANCELED);
                        if(pMe->IsPause)
                        {
                            ISHELL_SetTimer(pMe->m_pShell,150, (PFNNOTIFY)(IMEDIA_Resume),pMe->m_pMedia); 
                        }
                        else
                        {
                            ISHELL_SetTimer(pMe->m_pShell,150, (PFNNOTIFY)VideoPlayer_PlayVideo,pMe);                            
                        }
                        pMe->IsPlay  = TRUE;
                        pMe->IsPause=FALSE;
                        pMe->UserStop=FALSE; 
 
                    } 
                    else CLOSE_DIALOG(DLGRET_MSGBOX);
                    break; 
                    
                case IDS_PAUSE:
                    VideoPlayer_DrawImage(pMe,VIDEOPLAYER_IMAGES_RES_FILE, IDI_PLAY, 82,179); //"|>"
                    CLOSE_DIALOG(DLGRET_CANCELED);
                    
                    //if(!pMe->PauseLock)//pMe->bTotalTime == pMe->bCurrentTime || pMe->bCurrentTime == 0ʱ,pMe->PauseLock=TRUE��
                                       //���Ը��ж������ݲ�����
                    //{                        
                        ISHELL_SetTimer(pMe->m_pShell,150, (PFNNOTIFY)(IMEDIA_Pause),pMe->m_pMedia);
                        pMe->IsPause = TRUE;
                        pMe->IsPlay = FALSE;
                        pMe->UserStop = FALSE;                            
                        
                    //} 
                    break;
                  
                case IDS_DELETE:                    
                    if(pMe->IsPlay)
                    {   
                        CLOSE_DIALOG(DLGRET_MSGBOX);
                        
                        ISHELL_SetTimer(pMe->m_pShell,1000, (PFNNOTIFY)(IMEDIA_Pause),pMe->m_pMedia);                                              
                        pMe->IsPause = TRUE;
                        pMe->IsPlay = FALSE;                        
                    }
                    //CLOSE_DIALOG(DLGRET_MSGBOX);
                    break;
                    
                case IDS_FULLSCREEN:
                    CLOSE_DIALOG(DLGRET_CANCELED);                    
                    VideoPlayer_PlayMod(pMe, 0);
                    break;
                     
                default:
                    break;
            }
            return TRUE;
      
        default:
            break;
    }
        return FALSE;
}
#endif
/*=================================================================================================================
������VPDMSGBOX_HandleEvent

˵����
       VideoPlayer Applet ������Ի���IDD_DELETE�¼���������

������
       pMe [in]��ָ��VideoPlayer Applet����ṹ��ָ�롣�ýṹ����С������ض���Ϣ��
       eCode [in]���¼����롣
       wParam���¼�����
       dwParam [in]����wParam���������ݡ���

����ֵ��
        TRUE�������¼�������
        FALSE�������¼�û������

��ע��:

=================================================================================================================*/

static boolean  VPDMSGBOX_HandleEvent(CVideoPlayer *pMe,AEEEvent eCode,uint16 wParam,uint32 dwParam)
{   
    static  IStatic * pStatic = NULL;
    AECHAR  wstrText[128];
    int     set_timer=0;
    
    if (NULL == pMe)
    {
        return FALSE;
    }

    switch(eCode)
    {
        case EVT_DIALOG_INIT:
            if (NULL == pStatic)
            {
                AEERect rect = {0};
                if (AEE_SUCCESS != ISHELL_CreateInstance(pMe->m_pShell,AEECLSID_STATIC,(void **)&pStatic))
                {
                    return FALSE;
                }
                ISTATIC_SetRect(pStatic, &rect);
            }
            return TRUE;
        

        case EVT_DIALOG_START: 
            ISHELL_PostEvent(pMe->m_pShell, AEECLSID_VIDEOPLAYER,EVT_USER_REDRAW,  0, 0);
            return TRUE;        

        case EVT_USER_REDRAW:            
            {
                PromptMsg_Param_type PromptMsg={0}; 
    
                if(pMe->m_nMsgResID==IDS_PLAYFAILED)
                {
                    DBGPRINTF("message");
                    PromptMsg.ePMsgType = MESSAGE_ERR;
                    set_timer=3000;//SETTIME_MIDDLE; // 1000
                    (void)ISHELL_LoadResString(pMe->m_pShell, AEE_APPSVIDEOPLAYER_RES_FILE, pMe->m_nMsgResID, wstrText, sizeof(wstrText));
                    PromptMsg.pwszMsg = wstrText;
                    DrawPromptMessage(pMe->m_pDisplay, pStatic, &PromptMsg);
                    IDISPLAY_UpdateEx(pMe->m_pDisplay,FALSE);
                    (void)ISHELL_CancelTimer(pMe->m_pShell, NULL, pMe);
                     //�������͹رնԻ����¼��Ķ�ʱ��               
                    ISHELL_SetTimer(pMe->m_pShell,set_timer,DialogTimeoutCallback,pMe);
                }
                else 
                {  
                    PromptMsg.ePMsgType =MESSAGE_CONFIRM;
                    PromptMsg.eBBarType = BTBAR_OK_BACK;
                    (void)ISHELL_LoadResString(pMe->m_pShell, AEE_APPSCOMMONRES_LANGFILE, pMe->m_nMsgResID, wstrText, sizeof(wstrText));                    
                    PromptMsg.pwszMsg = wstrText;
                    DrawPromptMessage(pMe->m_pDisplay, pStatic, &PromptMsg);
                    IDISPLAY_UpdateEx(pMe->m_pDisplay,FALSE);
                    (void)ISHELL_CancelTimer(pMe->m_pShell, NULL, pMe);
                } 
            }        
            return TRUE; 

        case EVT_DIALOG_END:        
            ISHELL_CancelTimer(pMe->m_pShell, DialogTimeoutCallback,pMe);
            ISTATIC_Release(pStatic);
            pStatic = NULL;
            FREE(wstrText);
            return TRUE;
        
        case EVT_KEY:           
            if(pMe->m_nMsgResID != IDS_PLAYFAILED)
            {
                switch(wParam)
                {
                    case AVK_SELECT:
                        pMe->Is848Busy = FALSE; 
                        if(app_media_scheduler()==APP_MEDIA_IMPACT_BY_FM)
                        {                        
                            ISHELL_SendEvent(pMe->m_pShell, AEECLSID_APP_FMRADIO, EVT_CLOSEAPP, 0, 0);
                        }
                        if(app_media_scheduler()==APP_MEDIA_IMPACT_BY_MP3)
                        {                           
                            ISHELL_SendEvent(pMe->m_pShell, AEECLSID_APP_MUSICPLAYER, EVT_CLOSEAPP, 0, 0);
                        } 
                        //��ʼ��Mediaָ�벢ע��ص�
                        //VideoPlayer_InitVideo(pMe); 
                        CLOSE_DIALOG(DLGRET_VIDEOPLAYER);
                        break;
                        
                    case AVK_CLR:
                        CLOSE_DIALOG(DLGRET_CANCELED);
                        break;
                        
                    default:
                        break;
                }
            }          
            //return VPDMSGBOX_HandleKeyEvent(pMe,eCode,wParam,dwParam);            

        case EVT_DISPLAYDIALOGTIMEOUT:            
            if(pMe->m_nMsgResID==IDS_PLAYFAILED)
            {
                CLOSE_DIALOG(DLGRET_CANCELED);
            }
#if 0
            if(pMe->m_nMsgResID==IDS_DEL || pMe->m_nMsgResID==IDS_DONE)
            {                
                CLOSE_DIALOG(DLGRET_VIDEOPLAYER);
            }  
#endif 
            return TRUE; 

        default:
            break;
    }
    return FALSE;
}
/*=================================================================================================================  
  ������Ƶ���ŵİ�ť�¼�
=================================================================================================================*/
#ifndef WIN32
extern /*MMD_PLYFILE*/uint32 pFileHandle;
#endif//WIN32
static boolean VPDVideoPlayer_HandleKeyEvent(CVideoPlayer *pMe,AEEEvent eCode,uint16 wParam ,uint32 dwParam)
{   
    if(pMe->IsFullScreen)
    {
		return VideoPlayer_PlayMod(pMe,wParam);
    }
    switch(wParam)
    {       
        //���Ż���ͣ
        case AVK_INFO:
            (void)ISHELL_CancelTimer(pMe->m_pShell, NULL, pMe);
            if(pMe->m_InitFailed != SUCCESS)
            {
                CLOSE_DIALOG(DLGRET_MSGBOX);
            }
            else // ��ʼ���ɹ�
            {
                if(!pMe->IsFullScreen)// ����ģʽ
                {                   
                    if(!pMe->IsPlay) // �ǲ���״̬
                    {    
#if 0
                        VideoPlayer_DrawImage(pMe,VIDEOPLAYER_IMAGES_RES_FILE, IDI_PAUSE, VIDEOPLAYER_PLAY_X, VIDEOPLAYER_PLAY_Y); //"||"
#endif
						pMe->m_rtype = TYPE_PLAYER;
                        DRAW_BOTTOMBAR(BTBAR_FULLSCREEN_PAUSE_STOP);
                        IDISPLAY_UpdateEx(pMe->m_pDisplay,FALSE);

                        if(pMe->IsPause)// ��ǰ״̬Ϊ��ͣʱ
                        {                             
                            IMEDIA_Resume((IMedia*)pMe->m_pMedia);
                            //ISHELL_SetTimer(pMe->m_pShell,50,(PFNNOTIFY)(IMEDIA_Resume),pMe->m_pMedia);
                            SetDeviceState(DEVICE_TYPE_MP4,DEVICE_MP4_STATE_ON);                              
                        }
                        else  
                        {
                            VideoPlayer_PlayVideo(pMe);
                            //ISHELL_SetTimer(pMe->m_pShell,50,(PFNNOTIFY)VideoPlayer_PlayVideo,pMe);
                        }                        
                        
                        pMe->IsPlay=TRUE;
                        pMe->IsPause=FALSE;
                        pMe->UserStop=FALSE; 
                        videoplayer_play_flag = TRUE;
                    }
                    else// ��ǰ״̬Ϊ����ʱ
                    {  
#if 0                    
                        VideoPlayer_DrawImage(pMe,VIDEOPLAYER_IMAGES_RES_FILE, IDI_PLAY, VIDEOPLAYER_PLAY_X,VIDEOPLAYER_PLAY_Y); //"|>"
#endif
						pMe->m_rtype = TYPE_PLAYER;
                        DRAW_BOTTOMBAR(BTBAR_PLAY_STOP); 
                        IDISPLAY_UpdateEx(pMe->m_pDisplay,FALSE);
                        //IMEDIA_Pause(pMe->m_pMedia); // ����ʱ��Ļ�������ˢ������
                        ISHELL_SetTimer(pMe->m_pShell,50,(PFNNOTIFY)(IMEDIA_Pause),pMe->m_pMedia); 
                        SetDeviceState(DEVICE_TYPE_MP4,DEVICE_MP4_STATE_OFF);  
                        pMe->IsPlay=FALSE;
                        pMe->IsPause=TRUE;
                        pMe->UserStop=FALSE;                       
                    }               
                }
                else // ȫ��ģʽ
                {                  
                    SetDeviceState(DEVICE_TYPE_MP4,DEVICE_MP4_STATE_ON);
                    if(!pMe->IsPlay) // �ǲ���״̬
                    { 
                        if(pMe->IsPause)// ��ǰ״̬Ϊ��ͣʱ
                        {
                            IMEDIA_Resume((IMedia*)pMe->m_pMedia);
                            //ISHELL_SetTimer(pMe->m_pShell,50,(PFNNOTIFY)(IMEDIA_Resume),pMe->m_pMedia);
                            
                        }
                        else// ��ǰ״̬Ϊֹͣʱ
                        {                            
                            VideoPlayer_PlayVideo(pMe);
                            //ISHELL_SetTimer(pMe->m_pShell,50,(PFNNOTIFY)VideoPlayer_PlayVideo,pMe); 
                        }
                        pMe->IsPlay=TRUE;;
                        pMe->IsPause=FALSE;
                        pMe->UserStop=FALSE;
                        videoplayer_play_flag = TRUE;
                    }
                    else // ��ǰ״̬Ϊ����ʱ
                    {   
                        IMEDIA_Pause((IMedia*)pMe->m_pMedia);
                        //ISHELL_SetTimer(pMe->m_pShell,50,(PFNNOTIFY)(IMEDIA_Pause),pMe->m_pMedia); 
                        
                        pMe->IsPlay=FALSE;
                        pMe->IsPause=TRUE;
                        pMe->UserStop=FALSE;
                    }
                }                
            }
            return TRUE; 
       
        case AVK_CLR:            
            (void)ISHELL_CancelTimer(pMe->m_pShell, NULL, pMe);
            /*����ͣ�򲥷�״̬��stop����oem�㶼�ᴫ����һ��MM_STATUS_DONE�ص��¼���
               ��ʱ��UI���ڻص��������ػ棬�˴���������*/
            if(!pMe->IsFullScreen)//��ȫ��״̬
            {
                
                if(pMe->IsPause || pMe->IsPlay)
                { 
#if 0                
                    VideoPlayer_DrawImage(pMe,VIDEOPLAYER_IMAGES_RES_FILE,IDI_PLAY, VIDEOPLAYER_PLAY_X,VIDEOPLAYER_PLAY_Y); //���м�ͼ��ˢ��Ϊ�����ε�Сͼ��      
#endif
					pMe->m_rtype = TYPE_PLAYER;
                    if(pMe->m_IsPlaynext)
                    {
                        DRAW_BOTTOMBAR(BTBAR_GALLERY_PLAY_BACK);
                    }
                    else
                    {
                        DRAW_BOTTOMBAR(BTBAR_PLAY_BACK);
                    }                
                    pMe->bCurrentTime=0; //����ǰ����ʱ����Ϊ0����ʱ�䲻�� 
                    VideoPlayer_RefreshPlayingTick(pMe);//ˢ��ʱ��
                    VideoPlayer_RefreshScheduleBar(pMe);//���������ڳ�ʼλ�ã���ˢ���ļ�������������
                    IDISPLAY_UpdateEx(pMe->m_pDisplay,FALSE);
                    
                    pMe->UserStop=TRUE;
                    pMe->IsPlay=FALSE;
                    pMe->IsPause=FALSE;
                    
                    if(pMe->m_pMedia)
					{
						IMEDIA_Stop((IMedia*)pMe->m_pMedia);
					}			
					VideoPlayer_ChangeScrState(pMe,FALSE);
                    SetDeviceState(DEVICE_TYPE_MP4,DEVICE_MP4_STATE_OFF);
                    //ISHELL_SetTimer(pMe->m_pShell,50,(PFNNOTIFY)(IMEDIA_Stop),pMe->m_pMedia);                                    
                }
                else
                {
                    VideoPlayer_ReleaseVideo(pMe);
                    CLOSE_DIALOG(DLGRET_CANCELED);
                } 
            }            
            else //ȫ��״̬��
            {
                {   
				// YY TODO:
                    //MMD_LCDRotate(0);
                    AEERect rc = {0,0,SCR_W,SCR_H};
                    pMe->IsFullScreen = FALSE;
		            FullScreen = FALSE;
							
					IDisplay_FillRect(pMe->m_pDisplay,&rc,0x0);
					VideoPlayer_WriteTitleRes(pMe,AEE_APPSVIDEOPLAYER_RES_FILE,IDS_TITLE);
					VideoPlayer_DrawImage(pMe,VIDEOPLAYER_IMAGES_RES_FILE,IDI_LOGO, VIDEOPLAYER_LOGO_X, VIDEOPLAYER_LOGO_Y);

#if 0                    
                    VideoPlayer_DrawImage(pMe,VIDEOPLAYER_IMAGES_RES_FILE,IDI_PLAYERPICTURE_PLAY, 0, 0);//����ͼ
#endif
                    if( pMe->IsPlay)
                    { 
#if 0                    
                        VideoPlayer_DrawImage(pMe,VIDEOPLAYER_IMAGES_RES_FILE, IDI_PAUSE, VIDEOPLAYER_PLAY_X, VIDEOPLAYER_PLAY_Y); // "||"   
#endif
						pMe->m_rtype = TYPE_PLAYER;
                        DRAW_BOTTOMBAR(BTBAR_FULLSCREEN_PAUSE_STOP);
                        
                    }
                    else if(pMe->IsPause)
                    {

#ifndef WIN32
						// YY TODO:
                        //MMD_Player_File_Pause(pFileHandle);     //add by wangjian 2009.2.5
#endif//WIN32
                        SetDeviceState(DEVICE_TYPE_MP4,DEVICE_MP4_STATE_OFF);
#if 0
                        VideoPlayer_DrawImage(pMe,VIDEOPLAYER_IMAGES_RES_FILE, IDI_PLAY, VIDEOPLAYER_PLAY_X, VIDEOPLAYER_PLAY_Y); //"|>"
#endif
						pMe->m_rtype = TYPE_PLAYER;
                        DRAW_BOTTOMBAR(BTBAR_PLAY_STOP);
                        
                    }
                    else
                    {
#if 0                   
                        VideoPlayer_DrawImage(pMe,VIDEOPLAYER_IMAGES_RES_FILE, IDI_PLAY, VIDEOPLAYER_PLAY_X, VIDEOPLAYER_PLAY_Y); //"|>"
#endif
						pMe->m_rtype = TYPE_PLAYER;
                        if(pMe->m_IsPlaynext)
                        {
                            DRAW_BOTTOMBAR(BTBAR_GALLERY_PLAY_BACK);
                        }
                        else
                        {
                            DRAW_BOTTOMBAR(BTBAR_PLAY_BACK);
                        }
                    }
                    
                    if(pMe->m_FileToPlay!=NULL )
                    {                       
                        VideoPlayer_RefreshPlayerFileName(pMe); //ˢ���ļ���                        
                    }                    
                
                    VideoPlayer_RefreshPlayingTick(pMe);//ˢ��ʱ��tick
                    VideoPlayer_RefreshVolBar(pMe);//ˢ������
                    VideoPlayer_RefreshScheduleBar(pMe);//ˢ�½�����            
                    IDISPLAY_UpdateEx(pMe->m_pDisplay,FALSE);                                   
                }
            }            
            return TRUE;
                              
        //ǰ��
        case AVK_POUND:
            if(pMe->IsPlay)
            {             
                if((pMe->bTotalTime - pMe->bCurrentTime) < 5)
                {
                    IMEDIA_FastForward((IMedia*)pMe->m_pMedia,pMe->bTotalTime - pMe->bCurrentTime);
                    pMe->bCurrentTime=pMe->bTotalTime;
                }
                else 
                {
                    IMEDIA_FastForward((IMedia*)pMe->m_pMedia,5000); 
                    pMe->bCurrentTime=pMe->bCurrentTime+5;
                }
                if(!pMe->IsFullScreen)
                {
                    VideoPlayer_RefreshPlayingTick(pMe);//ˢ��ʱ��
                    VideoPlayer_RefreshScheduleBar(pMe);//ˢ�²��Ž���
#if 0
                    VideoPlayer_DrawImage(pMe,VIDEOPLAYER_IMAGES_RES_FILE,IDI_FORWARD_SELECT, VIDEOPLAYER_FORWARD_X,VIDEOPLAYER_FORWARD_Y); //����������ȥ��Сͼ��
#endif
					pMe->m_rtype = TYPE_KUAIJIN;
                    IDISPLAY_UpdateEx(pMe->m_pDisplay,FALSE);
                    (void) ISHELL_SetTimer(pMe->m_pShell, 100, (PFNNOTIFY)VideoPlayer_UpdateFRButton, pMe);            
                }
            }
            return TRUE;
                
        //����
        case AVK_STAR:
            if(pMe->IsPlay)
            {               
                if( pMe->bCurrentTime < 5)
                {                  
                    IMEDIA_Rewind((IMedia*)pMe->m_pMedia,pMe->bCurrentTime);
                    pMe->bCurrentTime=0;
                }
                else
                {
                    IMEDIA_Rewind((IMedia*)pMe->m_pMedia,5000);   
                    pMe->bCurrentTime=pMe->bCurrentTime-5;
                }
                if(!pMe->IsFullScreen)
                {
                    VideoPlayer_RefreshPlayingTick(pMe);
                    VideoPlayer_RefreshScheduleBar(pMe);//ˢ�²��Ž�����
#if 0                    
                    VideoPlayer_DrawImage(pMe,VIDEOPLAYER_IMAGES_RES_FILE, IDI_REWIND_SELECT, VIDEOPLAYER_REWIND_X, VIDEOPLAYER_REWIND_Y); //����������ȥ��Сͼ��   
#endif
					pMe->m_rtype = TYPE_KUAITUI;
                    IDISPLAY_UpdateEx(pMe->m_pDisplay,FALSE);
                    (void) ISHELL_SetTimer(pMe->m_pShell, 100, (PFNNOTIFY)VideoPlayer_UpdateFRButton, pMe);            
                }
            }
            return TRUE;     
               
        //������һ��   
        case AVK_LEFT:           
            if(! pMe->m_IsPlaynext)
            {
                return TRUE;
            }
            ISHELL_SetTimer(pMe->m_pShell,APPISREADY_TIMER,VideoPlayer_APPIsReadyTimer,pMe);
            if(! pMe->m_bAppIsReady)
            {
                return TRUE;
            }
            if(!pMe->IsFullScreen)
            {
#if 0   
                VideoPlayer_DrawImage(pMe,VIDEOPLAYER_IMAGES_RES_FILE, IDI_BEFORE_SELECT, VIDEOPLAYER_PREVIOUS_X, VIDEOPLAYER_PREVIOUS_Y); //����������ȥ��Сͼ��
#endif
                pMe->m_rtype = TYPE_PREVIOUS;
                IDISPLAY_UpdateEx(pMe->m_pDisplay,FALSE);
                (void) ISHELL_SetTimer(pMe->m_pShell, 100, (PFNNOTIFY)VideoPlayer_UpdateButton, pMe);//100ms�󻭰�ť�����ͼ
                VideoPlayer_PlayNext(pMe, FALSE); 
            }
            else if(!pMe->IsPause)
            {
                VideoPlayer_PlayNext(pMe, FALSE); 
            }
            pMe->m_bAppIsReady=FALSE;
            return TRUE;
       
        //������һ��   
        case AVK_RIGHT:  
            if(! pMe->m_IsPlaynext)
            {
                return TRUE;
            }
            ISHELL_SetTimer(pMe->m_pShell,APPISREADY_TIMER,VideoPlayer_APPIsReadyTimer,pMe);
            if(! pMe->m_bAppIsReady)
            {
                return TRUE;
            }
            if(!pMe->IsFullScreen)
            { 
#if 0            
                VideoPlayer_DrawImage(pMe,VIDEOPLAYER_IMAGES_RES_FILE,IDI_NEXT_SELECT, VIDEOPLAYER_NEXT_X, VIDEOPLAYER_NEXT_Y);//����������ȥ��Сͼ��  
#endif
				pMe->m_rtype = TYPE_NEXT;
                IDISPLAY_UpdateEx(pMe->m_pDisplay,FALSE);
                (void) ISHELL_SetTimer(pMe->m_pShell, 100, (PFNNOTIFY)VideoPlayer_UpdateButton, pMe);//100ms�󻭰�ť�����ͼ
                VideoPlayer_PlayNext(pMe, TRUE);
            }            
            else if(!pMe->IsPause)//ȫ������״̬
            {
                DBGPRINTF("play next");
                VideoPlayer_PlayNext(pMe, TRUE); 
            }
            pMe->m_bAppIsReady=FALSE;
            return TRUE;          
          
        //��������   
        case AVK_UP: 
            if(pMe->m_bVolumeLevel < VOLUME_FIVE)
            {
                pMe->m_bVolumeLevel++;
                if(pMe->m_bVolumeLevel == VOLUME_FIVE)
                {
                    pMe->totalvolume = 100;//�������ֵ
                }
                else
                {
                    pMe->totalvolume = pMe->m_bVolumeLevel * VP_VOLUME_STEP;
                }
                    
                if(pMe->m_pMedia)
                {
                    (void)IMEDIA_SetVolume((IMedia*)pMe->m_pMedia,pMe->totalvolume); 
                } 
                if(!pMe->IsFullScreen)
                {
                    VideoPlayer_RefreshVolBar(pMe); // ����ˢ������bar
#if 0
                    VideoPlayer_DrawImage(pMe,VIDEOPLAYER_IMAGES_RES_FILE,IDI_ADD_SELECT, VIDEOPLAYER_VOLUME_ADD_X,VIDEOPLAYER_VOLUME_ADD_Y);// ����������ȥ��Сͼ��
#endif
					pMe->m_rtype = TYPE_ADDVOLUME;
                    IDISPLAY_UpdateEx(pMe->m_pDisplay,FALSE);                    
                    (void) ISHELL_SetTimer(pMe->m_pShell, 100, (PFNNOTIFY)VideoPlayer_UpdateButton, pMe);// 50ms�󻭰�ť�����ͼ                
                }
            }             
            return TRUE;          
          
        //��С����  
        case AVK_DOWN: 
            if(pMe->m_bVolumeLevel > VOLUME_OFF)
            {
                pMe->m_bVolumeLevel--;
                pMe->totalvolume = pMe->m_bVolumeLevel * VP_VOLUME_STEP;
                if(pMe->m_pMedia)                                                                                                                                                                   
                {
                    (void)IMEDIA_SetVolume((IMedia*)pMe->m_pMedia,pMe->totalvolume);
                } 
                if(!pMe->IsFullScreen)
                {
                    VideoPlayer_RefreshVolBar(pMe);// ����ˢ������bar 
#if 0
                    VideoPlayer_DrawImage(pMe,VIDEOPLAYER_IMAGES_RES_FILE,IDI_LOW_SELECT, VIDEOPLAYER_VOLUME_LOW_X, VIDEOPLAYER_VOLUME_LOW_Y);// ����������ȥ��Сͼ��
#endif
					pMe->m_rtype = TYPE_DECVOLUME;
                    IDISPLAY_UpdateEx(pMe->m_pDisplay,FALSE);
                    (void) ISHELL_SetTimer(pMe->m_pShell, 100, (PFNNOTIFY)VideoPlayer_UpdateButton, pMe);// 50ms�󻭰�ť�����ͼ   
                }
            }            
            return TRUE;
       
#if 0
        //ȫ������
        case AVK_0:
            return VideoPlayer_PlayMod(pMe);            
            break; 
            
        case AVK_SELECT:
            if(pMe->IsFullScreen)
            {                
                return TRUE;
            }
            else
            {
                CLOSE_DIALOG(DLGRET_OPTION);
                return TRUE;
            }               
#endif
        
        case AVK_GSENSOR_STAND:
        case AVK_GSENSOR_UPEND: 
        case AVK_GSENSOR_LEFT:
        case AVK_GSENSOR_RIGHT: 
 
        case AVK_SELECT:
            if(pMe->IsPlay)
            {
                FullScreen = TRUE;
                return VideoPlayer_PlayMod(pMe,wParam);
            }
            if(pMe->UserStop && pMe->m_IsPlaynext)
            {
                CMediaGallery_FileExplorer(GALLERY_VIDEO_ADDLIST,NULL);
                return TRUE;
            }
#ifdef FEATURE_LCD_TOUCH_ENABLE//WLH ADD FOR LCD TOUCH
		case EVT_PEN_UP:
			{
				int i;
				AEEDeviceInfo devinfo;
				int nBarH ;
				AEERect rc;
				int16 wXPos = (int16)AEE_GET_X((const char *)dwParam);
				int16 wYPos = (int16)AEE_GET_Y((const char *)dwParam);
				
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
#endif//FEATURE_LCD_TOUCH_ENABLE         
        default:
            break;
    }
    return TRUE;
}
/*=================================================================================================================
  ����OPTION�İ�ť�¼�
=================================================================================================================*/
#if 0
static boolean VPDOption_HandleKeyEvent(CVideoPlayer *pMe,AEEEvent eCode,uint16 wParam,uint32 dwParam)
{
    switch(wParam)
    {       
        case AVK_UP:
        case AVK_DOWN:
            {                    
                if(pMe->m_nItemID == IDS_PLAY || IDS_PAUSE)     
                {
                    pMe->m_nItemID = IDS_FULLSCREEN;
                }
                if(pMe->m_nItemID== IDS_FULLSCREEN) 
                {
                    pMe->m_nItemID = IDS_DELETE || IDS_GALLERY;
                }
                if(pMe->m_nItemID== IDS_GALLERY) 
                {
                    pMe->m_nItemID = IDS_PLAY || IDS_PAUSE;
                }
                else if(pMe->m_nItemID== IDS_DELETE) 
                {
                    pMe->m_nItemID = IDS_GALLERY;  
                }
            } 
            return TRUE;
  
        case AVK_INFO:
        case AVK_SELECT:
            CLOSE_DIALOG(DLGRET_MSGBOX);
            return TRUE;

        case AVK_CLR:
            CLOSE_DIALOG(DLGRET_CANCELED);
            return TRUE;   

        default:
            break;         
    }
    return TRUE;
}
#endif
/*=================================================================================================================
      ����MSGBOX�İ�ť�¼�
=================================================================================================================*/
#if 0
static boolean VPDMSGBOX_HandleKeyEvent(CVideoPlayer *pMe,AEEEvent eCode,uint16 wParam,uint32 dwParam)
{ 
    if(pMe->m_nMsgResID==IDS_DONE)
    {
        CLOSE_DIALOG(DLGRET_VIDEOPLAYER);
    }
    switch(wParam)
    {
        case AVK_SELECT:
            (void)ISHELL_CancelTimer(pMe->m_pShell, DialogTimeoutCallback,pMe);            
            /*
            if(pMe->m_nMsgResID==IDS_PLAYFAILED)
            {
                CLOSE_DIALOG(DLGRET_CANCELED);
            }
            */
            if(pMe->m_nMsgResID==IDS_DEL)
            {    
                //ɾ�����ڲ��ŵ���Ƶ�ļ�
                //VideoPlayer_DeleteVideo(pMe);
                //���ɾ���ɹ�������ʾDone�Ի���
                CLOSE_DIALOG(DLGRET_MSGBOX);//Done����
            }
            return TRUE;
                    
        case AVK_CLR:  
            (void)ISHELL_CancelTimer(pMe->m_pShell, DialogTimeoutCallback,pMe);
            if(pMe->m_nMsgResID==IDS_DEL || pMe->m_nMsgResID==IDS_DONE)
            {                
                CLOSE_DIALOG(DLGRET_VIDEOPLAYER);
            }
            return TRUE;
                   
        default:
            break;
    }

    return TRUE;
}   
#endif
/*=================================================================================================================
  ��ָ��λ�û�ͼ
=================================================================================================================*/
static void VideoPlayer_DrawImage(CVideoPlayer *pMe, char *resFile, int16 resId, int x, int y)
{
    IImage *image = ISHELL_LoadResImage( pMe->m_pShell, resFile, resId);
    if( image != NULL)
    {
        IIMAGE_Draw(image, x, y);
        IIMAGE_Release(image);
    }
    //IDISPLAY_UpdateEx(pMe->m_pDisplay,FALSE);
}
/*=================================================================================================================
   �ػ�player�°����İ�ť����
=================================================================================================================*/                
static  void VideoPlayer_UpdateButton(CVideoPlayer *pMe)
{
#if 0
    if(!pMe->IsPlay)
    {
        VideoPlayer_DrawImage(pMe,VIDEOPLAYER_IMAGES_RES_FILE, IDI_PLAY_PART, VIDEOPLAYER_PLAY_PART_X,VIDEOPLAYER_PLAY_PART_Y);//�ػ������ΰ�����,����Ĵ�ͼ   
    }
    else VideoPlayer_DrawImage(pMe,VIDEOPLAYER_IMAGES_RES_FILE, IDI_PAUSE_PART, VIDEOPLAYER_PAUSE_PART_X,VIDEOPLAYER_PAUSE_PART_Y);//�ػ�"||"������

	if(pMe->m_rtype == TYPE_ADDVOLUME)
	{
		VideoPlayer_DrawImage(pMe,VIDEOPLAYER_IMAGES_RES_FILE, IDI_ADD, VIDEOPLAYER_VOLUME_ADD_X,VIDEOPLAYER_VOLUME_ADD_Y);//�ػ������ΰ�����,����Ĵ�ͼ   
	}
	else if(pMe->m_rtype == TYPE_DECVOLUME)
	{
		VideoPlayer_DrawImage(pMe,VIDEOPLAYER_IMAGES_RES_FILE, IDI_LOW, VIDEOPLAYER_VOLUME_LOW_X,VIDEOPLAYER_VOLUME_LOW_Y);//�ػ������ΰ�����,����Ĵ�ͼ   
	}
	else if(pMe->m_rtype == TYPE_PREVIOUS)
	{
		VideoPlayer_DrawImage(pMe,VIDEOPLAYER_IMAGES_RES_FILE, IDI_BEFORE, VIDEOPLAYER_PREVIOUS_X,VIDEOPLAYER_PREVIOUS_Y);//�ػ������ΰ�����,����Ĵ�ͼ   
	}
	else if(pMe->m_rtype == TYPE_NEXT)
	{
		VideoPlayer_DrawImage(pMe,VIDEOPLAYER_IMAGES_RES_FILE, IDI_NEXT, VIDEOPLAYER_NEXT_X,VIDEOPLAYER_NEXT_Y);//�ػ������ΰ�����,����Ĵ�ͼ   
	}
	else if(pMe->m_rtype == TYPE_KUAIJIN)
	{
		VideoPlayer_DrawImage(pMe,VIDEOPLAYER_IMAGES_RES_FILE, IDI_FORWARD, VIDEOPLAYER_FORWARD_X,VIDEOPLAYER_FORWARD_Y);//�ػ������ΰ�����,����Ĵ�ͼ   
	}
	else if(pMe->m_rtype == TYPE_KUAITUI)
	{
		VideoPlayer_DrawImage(pMe,VIDEOPLAYER_IMAGES_RES_FILE, IDI_REWIND, VIDEOPLAYER_REWIND_X,VIDEOPLAYER_REWIND_Y);//�ػ������ΰ�����,����Ĵ�ͼ   
	}
    IDISPLAY_UpdateEx(pMe->m_pDisplay,FALSE);
#endif
}
static void VideoPlayer_UpdateFRButton(CVideoPlayer *pMe)
{
#if 0
    VideoPlayer_DrawImage(pMe,VIDEOPLAYER_IMAGES_RES_FILE, IDI_FR_PART, VIDEOPLAYER_FR_PART_X,VIDEOPLAYER_FR_PART_Y);
    IDISPLAY_UpdateEx(pMe->m_pDisplay,FALSE);
#endif
}
/*=================================================================================================================
��ʼ��video
=================================================================================================================*/

void  VideoPlayer_InitVideo(CVideoPlayer  *pMe)                  
{    
    //��дm_md stucture
    uint32 uiClsId = 0;
    pMe->m_md.clsData = MMD_FILE_NAME;
    pMe->m_md.pData = (void*)pMe->m_FileToPlay;
    pMe->m_md.dwSize = 0;
	pMe->m_pMediaUtil = 0;
	pMe->m_pHelp = NULL;
	//pMe->m_InitFailed = AEEMediaUtil_CreateMedia(pMe->m_pShell, &pMe->m_md, &pMe->m_pMedia);
	pMe->m_InitFailed = AEEMediaUtil_CreateMedia(pMe->m_pShell, &pMe->m_md,&(IMedia*)pMe->m_pMedia);
    DBGPRINTF("pMe->m_InitFailed=%d",pMe->m_InitFailed);

    if(pMe->m_InitFailed == SUCCESS)
    {     
        (void)IMEDIA_RegisterNotify((IMedia*)pMe->m_pMedia, VideoPlayer_VideoNotify, pMe);//ע��ײ�ص� 
        pMe->bCurrentTime = 0;
        pMe->bTotalTime = 0;
		uiClsId = IMEDIA_GetTotalTime((IMedia*)pMe->m_pMedia); 
		DBGPRINTF("(void)IMEDIA_GetTotalTime(pMe->m_pMedia); %d",uiClsId);
        (void)IMEDIA_SetVolume((IMedia*)pMe->m_pMedia, pMe->totalvolume); //���õ�ǰ������С
        //VideoPlayer_ChangeScrState(pMe,TRUE);
    }    
}
/*=================================================================================================================
  ������Ƶ
=================================================================================================================*/      
void VideoPlayer_PlayVideo(CVideoPlayer *pMe)
{     
    if(pMe->m_pMedia)
    {  
    	DBGPRINTF("YY Said : Play!!! ");
		VideoPlayer_ChangeScrState(pMe,TRUE);
        pMe->m_PlayFailed = IMEDIA_Play((IMedia*)pMe->m_pMedia);//����  
        SetDeviceState(DEVICE_TYPE_MP4,DEVICE_MP4_STATE_ON);
    }      
} 
                    
/*=================================================================================================================
   ������һ���ļ�, 
   bDirectionΪTRUE, ��һ���ļ�
   bDirectionΪFALSE, ǰһ���ļ�
   pMe->m_MenuSelected
=================================================================================================================*/
static  void VideoPlayer_PlayNext(CVideoPlayer *pMe, boolean bDirection)
{  
    uint16    videoID = 0;
    FileInfo  pInfo;
    
    videoID=VideoPlayer_GetFileID(pMe);
    DBGPRINTF("pMe->m_RecordCount=%d",pMe->m_RecordCount);
    //video������ָ���ļ�����
    if(videoID >= pMe->m_RecordCount)
    {  
        DBGPRINTF("videoID >= pMe->m_RecordCount");
        return ;
    }
    else
    {
        switch(pMe->m_RecordCount)
        {
            //�ļ���Ϊ0��ʲô������
            case 0:
                return;
        
            //�ļ���Ϊ1�����²��Ŵ��ļ�(ֱ�Ӳ���)   
            case 1:    
                pMe->bCurrentTime = 0; 
                if(! pMe->UserStop)
                {
                    (void)IMEDIA_Stop((IMedia*)pMe->m_pMedia); 
                }
                VideoPlayer_PlayVideo(pMe);   
                videoplayer_play_flag = TRUE;
                pMe->IsPlay   = TRUE;
                pMe->IsPause  = FALSE;
                pMe->UserStop = FALSE; 
                if( ! pMe->IsFullScreen && !pMe->IsPlay)
                {
#if 0
                    VideoPlayer_DrawImage(pMe,VIDEOPLAYER_IMAGES_RES_FILE,IDI_PAUSE, VIDEOPLAYER_PLAY_X,VIDEOPLAYER_PLAY_Y);            
#endif
                    // YY TODO:
					//DRAW_BOTTOMBAR(BTBAR_PAUSE_STOP); 
                    
                    VideoPlayer_RefreshPlayingTick(pMe);//ˢ��ʱ��
                    VideoPlayer_RefreshScheduleBar(pMe);//���������ڳ�ʼλ��
                    IDISPLAY_UpdateEx(pMe->m_pDisplay,FALSE);
                }
                break;
        
            //�ļ�������1��������һ��   
            default:  
                if(! pMe->UserStop)
                {
                    (void)IMEDIA_Stop((IMedia*)pMe->m_pMedia); 
                }
                //�ͷű�m_pMediaָ��
                VideoPlayer_ReleaseVideo(pMe);
                FREEIF(pMe->m_FileToPlay);
                if(bDirection)//��һ��
                {   
                    //��ǰ���ŵ��ļ�Ϊ��¼�е����һ��,�򲥷ż�¼�еĵ�һ���ļ�     
                    if(videoID +1 == pMe->m_RecordCount)
                    {                  
                        videoID=0;
                    }                    
                    else  videoID+=1;                    
                }                
                else//��һ��
                {
                    //��ǰ���ŵ��ļ�Ϊ��¼�еĵ�һ��,�򲥷ż�¼�е����һ���ļ�     
                    if(videoID == 0)
                    {     
                        videoID=pMe->m_RecordCount-1;
                    }                    
                    else  videoID-=1;
                } 
				// YY TODO:
                //CMediaGallery_GetFileInfoRecord(pMe->m_pFileDB,videoID, (FileInfo*)&pInfo);
                
                pMe->m_FileToPlay= STRDUP(pInfo.szName);
                if(pMe->m_pFileDB!=NULL)
                {
                    IDATABASE_Release(pMe->m_pFileDB);
                    pMe->m_pFileDB = NULL;
                }
           
                VideoPlayer_InitVideo(pMe);
                // �����ǰ��ƵΪ����״̬,����һ����ƵҲֱ����Ϊ����״̬;����,���ֶ�����                
                VideoPlayer_PlayVideo(pMe); 
                if(pMe->IsPause)
                {
                    IMedia_Pause((IMedia*)pMe->m_pMedia);                    
                }
                if(pMe->UserStop)
                {
                    IMedia_Stop((IMedia*)pMe->m_pMedia);                   
                }
                if(pMe->IsFullScreen)
                {
				// YY TODO:
					//MMD_LCDRotate(0);
                    //�����һ�ײ��ųɹ�����תΪȫ�������ɹ���������ģʽ��������ʧ�ܵĵ���
                    if(pMe->m_PlayFailed == SUCCESS)
                    {
                        //MMD_LCDRotate(1);                        
                    }
                    else
                    {
                        VideoPlayer_RefreshPlayerFileName(pMe);// ˢ���ļ��� 
                        IDISPLAY_UpdateEx(pMe->m_pDisplay,FALSE);    
                    }
                    /*ȫ����ͣʱ��playnext
                    if(pMe->IsPause)
                    {
						// YY TODO:
                        MMD_Player_File_Pause(pFileHandle); 
                    }
                    */
                }  
                
                if(!pMe->IsFullScreen)
                {
                    if(pMe->m_PlayFailed == SUCCESS)
                    {
                        VideoPlayer_RefreshPlayingTick(pMe);// ˢ�²���ʱ��             
                        VideoPlayer_RefreshScheduleBar(pMe);// ˢ�½����� 
                    }
                    VideoPlayer_RefreshPlayerFileName(pMe);// ˢ���ļ��� 
                    IDISPLAY_UpdateEx(pMe->m_pDisplay,FALSE);    
                }
            
                break;    
        }      
    }
    //���û��ֶ�ֹͣ
    //pMe->UserStop = FALSE;
}
void VideoPlayer_WriteTitle(CVideoPlayer *pMe,AECHAR* pText)
{
	AEERect   rc_name;
	VideoPlayer_DrawImage(pMe,VIDEOPLAYER_IMAGES_RES_FILE,IDI_NAME_PART, 0, 0);  
	SETAEERECT(&rc_name, VIDEOPLAYER_NAMEPART_X + 10,VIDEOPLAYER_NAMEPART_Y, VIDEOPLAYER_NAMEPART_W - 20, VIDEOPLAYER_NAMEPART_H);
	//дtitle
	DrawTextWithProfile(pMe->m_pShell, 
					pMe->m_pDisplay, 
					RGB_WHITE, //�ı�������RGBVAL��ɫֵ,RGB_BLACK
					AEE_FONT_BOLD,
					pText, 
					-1, 
					22, 
					22, 
					&rc_name, 
					IDF_ALIGN_CENTER|IDF_ALIGN_MIDDLE|IDF_TEXT_TRANSPARENT);	
}
void VideoPlayer_WriteTitleRes(CVideoPlayer *pMe,char* pResPath,int uResId)
{
  AECHAR pText[256] = {0};
  ISHELL_LoadResString(pMe->m_pShell,pResPath,uResId,pText,sizeof(pText));
  VideoPlayer_WriteTitle(pMe,pText);
}


/*=================================================================================================================
   ˢplayer�����ϰ벿�ֵ��ļ���
=================================================================================================================*/                                                                        
static  void VideoPlayer_RefreshPlayerFileName(CVideoPlayer *pMe)
{   
    char      title_copy[MAX_STR_LEN];  
    AECHAR    player_title[MAX_STR_LEN];
    char      *filename=NULL; 
    int       len;

    if(pMe->IsFullScreen) return;
        
    pMe->m_np=0;
    MEMSET(title_copy, 0, sizeof(title_copy));
    MEMSET(player_title, 0, sizeof(player_title));
    MEMSET(pMe->m_playname, 0, sizeof(pMe->m_playname));
    //��ȡ�ļ�·�����е��ļ�������ʾ�ڲ������� 
    filename= STRRCHR(pMe->m_FileToPlay, (int)DIRECTORY_CHAR);
    if(filename!=NULL)
    {
        filename=filename+1;
        STRCPY(title_copy,filename);
    } 
    else STRCPY(title_copy,pMe->m_FileToPlay);    
   
    //UTF8תAECHAR
    (void)UTF8TOWSTR((const byte *)title_copy,(int)STRLEN(title_copy),player_title,sizeof(player_title)); 
    (void)UTF8TOWSTR((const byte *)title_copy,(int)STRLEN(title_copy),pMe->m_playname,sizeof(pMe->m_playname));

    len=IDISPLAY_MeasureText(pMe->m_pDisplay,AEE_FONT_NORMAL,player_title); // �����ַ�������Ҫ������    
    if(len > MAX_NAME_LEN )
    {        
        //������ʾ����
        VideoPlayer_ShowName(pMe);  
    }
    else 
    {        
       VideoPlayer_WriteTitle(pMe,player_title);

    }
}
//������ʾ�ļ���
static void VideoPlayer_ShowName(CVideoPlayer *pMe) 
{ 
    int      str;
    AECHAR   *pp;   
    int      settime;

	if(pMe->IsFullScreen) return;
    
    pp=&pMe->m_playname[pMe->m_np]; 
    pMe->m_np += 1;
    str=IDISPLAY_MeasureText(pMe->m_pDisplay,AEE_FONT_NORMAL,pp);
	
	VideoPlayer_WriteTitle(pMe,pp);

    IDISPLAY_UpdateEx(pMe->m_pDisplay,TRUE); 
    if(str <= MAX_NAME_LEN)
    {
        pMe->m_np=0;
    }
    if(pMe->m_np==1)
    {
        settime=SETTIME_LONG; // 3000 
    }
    else settime=SETTIME_SHORT; // 300
    ISHELL_SetTimer(pMe->m_pShell,settime,(PFNNOTIFY) VideoPlayer_ShowName,pMe);
  
}
/*=================================================================================================================
   ˢ�²���ʱ��
=================================================================================================================*/   
static void VideoPlayer_RefreshPlayingTick(CVideoPlayer *pMe)
{
    AEERect rc_tick;    
    char    tick_time[MAX_STR_LEN];
    AECHAR  Wtick_time[MAX_STR_LEN]; 
    
    //���ٽ������Ϳ�ʼ��ʱ��������ͣ��������ܳ���
    if(pMe->bTotalTime == pMe->bCurrentTime || pMe->bCurrentTime == 0)
    {
        pMe->PauseLock = TRUE;
    }
    else
    {
        pMe->PauseLock = FALSE; 
    }      
   
    //��ʱ����ʾ����
    VideoPlayer_DrawImage(pMe,VIDEOPLAYER_IMAGES_RES_FILE,IDI_TIME_PART, VIDEOPLAYER_TIME_X, VIDEOPLAYER_TIME_Y); 
    //tick time
    SETAEERECT(&rc_tick, VIDEOPLAYER_TIME_X, VIDEOPLAYER_TIME_Y, VIDEOPLAYER_TIME_W, VIDEOPLAYER_TIME_H);

    
    MEMSET(tick_time,0,MAX_STR_LEN);

    SPRINTF(tick_time,"%02d:%02d|%02d:%02d",
		pMe->bCurrentTime/60,pMe->bCurrentTime%60,
		pMe->bTotalTime/60,pMe->bTotalTime%60);
    
    STRTOWSTR(tick_time, Wtick_time, sizeof(Wtick_time));
    DrawTextWithProfile(pMe->m_pShell, 
                        pMe->m_pDisplay, 
                        RGB_WHITE, 
                        AEE_FONT_BOLD,
                        Wtick_time, 
                        -1, 
                        21, 
                        57, 
                        &rc_tick, 
                        IDF_ALIGN_CENTER|IDF_ALIGN_MIDDLE|IDF_TEXT_TRANSPARENT);
}    

/*=================================================================================================================
  ˢ�½�����
=================================================================================================================*/
static void VideoPlayer_RefreshScheduleBar(CVideoPlayer *pMe)
{
#if 0
    AEERect  rc;
    AEERect  Rc;
    AEERect  OldClip;
    AEERect  Clip;
    int16    ma;
    
    //ȡСͼ��ͼƬ
    IImage *image =ISHELL_LoadResImage(pMe->m_pShell, VIDEOPLAYER_IMAGES_RES_FILE, IDI_GLIDER); 

    //��������
    VideoPlayer_DrawImage(pMe,VIDEOPLAYER_IMAGES_RES_FILE, IDI_SCHEDULE_EMPTY, VIDEOPLAYER_SCHEDULE_X, VIDEOPLAYER_SCHEDULE_Y); 
    SETAEERECT(&rc,VIDEOPLAYER_SCHEDULE_X,VIDEOPLAYER_SCHEDULE_Y,5,5);//������ʼλ�� 
    SETAEERECT(&Rc,VIDEOPLAYER_SCHEDULE_X + VIDEOPLAYER_SCHEDULE_W,VIDEOPLAYER_SCHEDULE_Y,5,5);//��������λ��    
    ma=Rc.x-rc.x;//��������ƶ��ĳ��ȣ�26������  
    if(pMe->bTotalTime == 0)
    {
        Clip.x=rc.x;        
    }   
    else 
    {
        Clip.x=rc.x + pMe->bCurrentTime * ma / pMe->bTotalTime;
    }
    
    if(pMe->bCurrentTime > pMe->bTotalTime)
    {
        Clip.x=Rc.x;        
    }  
    //VideoPlayer_DrawImage(pMe,VIDEOPLAYER_IMAGES_RES_FILE, IDI_GLIDER, Clip.x, 174);
    
    IDISPLAY_GetClipRect(pMe->m_pDisplay, &OldClip);//restore clip rect
    SETAEERECT(&Clip,Clip.x,VIDEOPLAYER_SCHEDULE_Y,5,5);//�����ƶ�ʱ��λ��    
    IDISPLAY_SetClipRect(pMe->m_pDisplay, &Clip);
    IIMAGE_Draw(image,Clip.x,VIDEOPLAYER_SCHEDULE_Y);    
    IIMAGE_Release(image);
    IDISPLAY_SetClipRect(pMe->m_pDisplay, &OldClip);//recover clip rect
#endif
}
/*=================================================================================================================
  ˢ������bar
 ================================================================================================================*/
static void VideoPlayer_RefreshVolBar(CVideoPlayer *pMe)
{
    uint16 ResID;   
    switch ((int)pMe->m_bVolumeLevel)
    {
        case 0:
            ResID = IDI_VOLUME2_0;//ͼƬ��ԴID 
            break;
            
        case 1:
            ResID = IDI_VOLUME2_1;
            break;
            
        case 2:
            ResID = IDI_VOLUME2_2;
            break;
            
        case 3:
            ResID = IDI_VOLUME2_3;
            break;
            
        case 4:
            ResID = IDI_VOLUME2_4;
            break;
            
        case 5:
            ResID = IDI_VOLUME2_5;
            break;
        
        default :
            ResID = IDI_VOLUME2_0;            
    }
    VideoPlayer_DrawImage( pMe, VIDEOPLAYER_IMAGES_RES_FILE,ResID, VIDEOPLAYER_VOLUME_X, VIDEOPLAYER_VOLUME_Y);  
}

/*=================================================================================================================
   ��Ƶ���ŵ�ģʽ:���� or ȫ��
=================================================================================================================*/
boolean VideoPlayer_PlayMod(CVideoPlayer *pMe, uint16 wParam)
{  
  
    if(pMe->IsPlay)
    {
        if(!pMe->IsFullScreen)
        {
            if((AVK_GSENSOR_STAND != wParam) && (AVK_GSENSOR_UPEND != wParam))
            {    
                //ISHELL_CancelTimer(pMe->m_pShell,NULL,pMe); 
                pMe->IsFullScreen = TRUE;
				IMEDIA_SetMediaParm((IMedia*)pMe->m_pMedia,MM_PARM_RECT,(int32)&pMe->m_rc,NULL);
				VideoPlayer_ChangeScrState(pMe,TRUE);
				// YY TODO:
                //MMD_LCDRotate(1);
            }
        }
        else
        {   
            if((AVK_GSENSOR_LEFT != wParam) && (AVK_GSENSOR_RIGHT != wParam))
            {   
                pMe->IsFullScreen = FALSE;
                FullScreen = FALSE;
				VideoPlayer_ChangeScrState(pMe,TRUE);
                
                VideoPlayer_DrawImage(pMe,VIDEOPLAYER_IMAGES_RES_FILE,IDI_PLAYERPICTURE_PAUSE, 0, 0);//����ͼ
                DRAW_BOTTOMBAR(BTBAR_FULLSCREEN_PAUSE_STOP);               
                VideoPlayer_RefreshPlayerFileName(pMe); //ˢ���ļ���               
                VideoPlayer_RefreshPlayingTick(pMe);
                VideoPlayer_RefreshVolBar(pMe);//ˢ������
                VideoPlayer_RefreshScheduleBar(pMe);//ˢ�½�����
                IDISPLAY_UpdateEx(pMe->m_pDisplay,FALSE); 
                
				// YY TODO:
                //MMD_LCDRotate(0);
            }
        return TRUE;
        }
    }    
   
    return TRUE;
}

/*=================================================================================================================
  ������������
=================================================================================================================*/
void Draw_Parser_Text(CVideoPlayer* pMe,const AECHAR* pText,uint16* height)
{
	AECHAR* pPosCur = (AECHAR*)pText;
	uint16 textw = 0;
	uint16 texth = 1;
	uint16 lh = 1;
	int    pos = 1,spacePos = 0;
	uint16 charw = 7,charh = 18;
	
	AEERect rc = {0,0,SCR_W,SCR_H};
	rc.x  = pMe->m_rc.x + 5;
	rc.dx = pMe->m_rc.dx - 5;
    rc.y = VIDEOPLAYER_NAMEPART_H + 5;
    rc.dy = pMe->m_rc.dy - VIDEOPLAYER_NAMEPART_H -  GetBottomBarHeight(pMe->m_pDisplay) - 5;

	if(pText == NULL) return ;
	if(GetLngCode() == LNG_SCHINESE)
	{
	  charw = charh;
	}

	IDISPLAY_FillRect(pMe->m_pDisplay,&rc,0x0);

	while(*pPosCur != '\0')
	{
		if(*pPosCur == ' ')
		{
		    spacePos = 0;
			if(pos == 1)
			{
			  pPosCur++;
			  continue;
			}
		}
		else if(*pPosCur == '\\')
		{
			switch(*(pPosCur + 1))
			{
			case 'n':
				{
					DrawTextWithProfile(pMe->m_pShell, 
						pMe->m_pDisplay, 
						RGB_WHITE, //�ı�������RGBVAL��ɫֵ,RGB_BLACK
						AEE_FONT_NORMAL,
						pPosCur - pos + 1,
						pos - 1, 
						rc.x, 
						VIDEOPLAYER_NAMEPART_H + texth - pMe->m_pHelp->m_Posy, 
						&rc, 
						IDF_TEXT_TRANSPARENT);
					texth += (lh + charh);
					textw = charw;
					pos = 0;
					pPosCur++;
					break;
				}
			default:
				{
					break;
				}
			}
		}

		if(textw + charw > VIDEO_TEXT_W)
		{
		   if(GetLngCode() != LNG_SCHINESE)
		   	{
	  			DrawTextWithProfile(pMe->m_pShell, 
	  				pMe->m_pDisplay, 
	  				RGB_WHITE, //�ı�������RGBVAL��ɫֵ,RGB_BLACK
	  				AEE_FONT_NORMAL,
	  				pPosCur - pos + 1,
	  				pos - spacePos,
	  				rc.x, 
	  				VIDEOPLAYER_NAMEPART_H + texth - pMe->m_pHelp->m_Posy, 
	  				&rc, 
	  				IDF_TEXT_TRANSPARENT);
	  			pPosCur -= spacePos;
		   	}else
		   	{
		   	    DrawTextWithProfile(pMe->m_pShell, 
	  				pMe->m_pDisplay, 
	  				RGB_WHITE, //�ı�������RGBVAL��ɫֵ,RGB_BLACK
	  				AEE_FONT_NORMAL,
	  				pPosCur - pos + 1,
	  				pos,
	  				rc.x, 
	  				VIDEOPLAYER_NAMEPART_H + texth - pMe->m_pHelp->m_Posy, 
	  				&rc, 
	  				IDF_TEXT_TRANSPARENT);
		   	}
			
			texth += (lh + charh);
			textw = charw;
			pos = 0;
			

		}

		textw += charw;
		pPosCur++;
		pos++;
		spacePos++;
	}
	DrawTextWithProfile(pMe->m_pShell, 
		pMe->m_pDisplay, 
		RGB_WHITE, //�ı�������RGBVAL��ɫֵ,RGB_BLACK
		AEE_FONT_NORMAL,
		pPosCur - pos + 1,
		pos - 1,
		rc.x, 
		VIDEOPLAYER_NAMEPART_H + texth - pMe->m_pHelp->m_Posy, 
		&rc, 
		IDF_TEXT_TRANSPARENT);
	*height = texth + (lh + charh);
}


/*=================================================================================================================
  �����������
=================================================================================================================*/
static void VideoPlayer_Help(CVideoPlayer* pMe)
{
  if(pMe->m_pHelp != NULL)
  	return ;
  
  pMe->m_pHelp = (CHelpPtr)MALLOC(sizeof(CHelp));
  if(pMe->m_pHelp == NULL)
  	 return ;
  
  pMe->m_pHelp->pText = NULL;
  pMe->m_pHelp->pTitle = NULL;	
  pMe->m_pHelp->m_Posy= 0;
  pMe->m_pHelp->m_Height = 0;
  pMe->m_pHelp->m_Key = 0;
  
  
  if(!VideoPlayer_HelpInit(pMe))
  	{
  	  SAFE_DELETE(pMe->m_pHelp);
  	}
}

static boolean VideoPlayer_HelpInit(CVideoPlayer* pMe)
{
	CHelpPtr pHelp;
	int      nReturn;

    pHelp = pMe->m_pHelp;
	
	if(!pHelp->pTitle)
	{
	    pHelp->pTitle = (AECHAR*)MALLOC(VIDEOPLAYER_HELP_TITLE);

		if(pHelp->pTitle == NULL)
			return FALSE;
		
		nReturn = ISHELL_LoadResString(pMe->m_pShell,
			AEE_APPSVIDEOPLAYER_RES_FILE,
			IDS_HELPTITLE,
			pHelp->pTitle,
			VIDEOPLAYER_HELP_TITLE);
		
		if(!nReturn)
		  SAFE_DELETE(pHelp->pTitle); 
	}
	if(!pHelp->pText)
	{
	    pHelp->pText = (AECHAR*)MALLOC(VIDEOPLAYER_HELP_TEXT);

		if(pHelp->pText == NULL)
			return FALSE;
		
		nReturn = ISHELL_LoadResString(pMe->m_pShell,
			AEE_APPSVIDEOPLAYER_RES_FILE,
			IDS_HELP,
			pHelp->pText,
			VIDEOPLAYER_HELP_TEXT);
		
		if(!nReturn)
		{
			SAFE_DELETE(pHelp->pTitle);
			return FALSE;
		}
	}
	VideoPlayer_DrawHelp(pMe);
	
	return TRUE;

}
/*=================================================================================================================
  ����ת��
=================================================================================================================*/
uint16 VideoPlayer_SearchKey(uint16 wParam)
{
	uint16 tempKey;
	switch(wParam)
		{
		case AVK_CLR:
			{
				tempKey = KEY_CLR;
				break;
			}
		case AVK_UP:
			{
				tempKey = KEY_UP;
				break;
			}
		case AVK_DOWN:
			{
				tempKey = KEY_DOWN;
			}
		default:
			{
				;
			}
		
		}
	return tempKey;
}
/*=================================================================================================================
  ������
=================================================================================================================*/
void VideoPlayer_DrawHelp(CVideoPlayer *pMe)
{
	CHelpPtr pHelp = pMe->m_pHelp;

	if(pHelp->m_Key & KEY_UP)
		{
				if((pHelp->m_Posy - VIDEO_SCROLL_SPEED) >= 0)
					pHelp->m_Posy -= VIDEO_SCROLL_SPEED;
		}
	else if(pHelp->m_Key & KEY_DOWN)
		{
				if(pHelp->m_Posy + VIDEO_SCROLL_SPEED< pHelp->m_Height)
				{
				  pHelp->m_Posy += VIDEO_SCROLL_SPEED;
				}	
		}
	
	Draw_Parser_Text(pMe,pHelp->pText,&pHelp->m_Height);
	VideoPlayer_WriteTitle(pMe,pHelp->pTitle);
	DRAW_BOTTOMBAR(BTBAR_BACK); 
	IDISPLAY_Update(pMe->m_pDisplay);
	ISHELL_SetTimer(pMe->m_pShell,50,(PFNNOTIFY)VideoPlayer_DrawHelp,pMe);

}
/*=================================================================================================================
  ��Ϣ������
=================================================================================================================*/
static boolean VideoPlayer_HelpHandleEvent(CVideoPlayer *pMe,AEEEvent eCode,uint16 wParam,uint32 dwParam)
{
  CHelpPtr pHelp = pMe->m_pHelp;
  uint16 key = 0;
  
  switch(eCode)
  	{
  	    //case EVT_KEY_HELD:
  	    case EVT_KEY_RELEASE:
			{
				key = VideoPlayer_SearchKey(wParam);
				pHelp->m_Key &= (~key);
				return TRUE;
  	    	}
		case EVT_KEY:
			{
				if(wParam == AVK_CLR)
				{
		          ISHELL_CancelTimer(pMe->m_pShell,(PFNNOTIFY)VideoPlayer_DrawHelp,pMe);
				  VideoPlayer_HelpTerminate(pMe);
				  SAFE_DELETE(pMe->m_pHelp);
				  ISHELL_PostEvent(pMe->m_pShell, AEECLSID_VIDEOPLAYER,EVT_USER_REDRAW,  0, 0);
				  return TRUE;
				}
			}
    	case EVT_KEY_PRESS:
			{
				key = VideoPlayer_SearchKey(wParam);
				pHelp->m_Key |= key;
				
			    return TRUE;

				break;
    		}
		case EVT_USER_REDRAW:
			{
				VideoPlayer_DrawHelp(pMe);
				return TRUE;
			}
		default:
			{
				return FALSE;
			}
  	}
  return TRUE;
}

static void VideoPlayer_HelpTerminate(CVideoPlayer *pMe)
{
  SAFE_DELETE(pMe->m_pHelp->pText);
  SAFE_DELETE(pMe->m_pHelp->pTitle);
}

#if 0
/*=================================================================================================================
   ɾ����ǰ���ŵ��ļ�
==================================================================================================================*/
static boolean VideoPlayer_DeleteVideo(CVideoPlayer *pMe)
{
    uint32  NumOfFile;//���ļ���

    NumOfFile=1;//��GALLERY���ȡ

    if(!pMe->UserStop)
    {
        pMe->UserStop=TRUE;        
        //IMEDIA_Stop(pMe->m_pMedia);
    }
    VideoPlayer_ReleaseVideo(pMe);
    //if(�ɹ�ɾ���ļ�)
    //{
    //    pMe->m_MenuSelected++;
          NumOfFile= NumOfFile-1; 
    //}
    switch(NumOfFile)
    {
        case 0:
            return FALSE;         
            
        default:
            //��Ϊ�ײ�ʹ�õ����¼����ݻ��ƣ���Ҫ���ײ�stop���ͷ�Video ��ʱ�䣬
            //���������˴���1���Ӻ��ʼ����һ��
            (void) ISHELL_SetTimer(pMe->m_pShell,1000,(PFNNOTIFY) VideoPlayer_InitVideo,pMe);
            break;
            
    }
    return TRUE;
}
#endif

/*=================================================================================================================
   ����CMX�ײ�ص�
=================================================================================================================*/ 
static void VideoPlayer_VideoNotify(void * pUser, AEEMediaCmdNotify * pCmdNotify)
{
    CVideoPlayer *   pMe = (CVideoPlayer *)pUser; 

    switch (pCmdNotify->nStatus)
    {   
        case MM_STATUS_SEEK_FAIL:     
        case MM_STATUS_PAUSE_FAIL:
        case MM_STATUS_RESUME_FAIL:
        default: 
            break;
    }
    
    //������Ƶ
    if (pCmdNotify->nCmd == MM_CMD_PLAY)
    {
        switch (pCmdNotify->nStatus)
        {   
            // playback done
            case MM_STATUS_DONE: 
                IMEDIA_Stop((IMedia*)pMe->m_pMedia);
                SetDeviceState(DEVICE_TYPE_MP4,DEVICE_MP4_STATE_OFF); 
                pMe->IsPlay=FALSE;
                pMe->IsPause=FALSE;
                pMe->UserStop=TRUE; 
                videoplayer_play_flag = FALSE;
        
                pMe->bCurrentTime=0;
                pMe->IsFullScreen=FALSE;                   
                VideoPlayer_RefreshPlayingTick(pMe);
                VideoPlayer_RefreshPlayerFileName(pMe);
                VideoPlayer_RefreshScheduleBar(pMe);//ˢ�½�����
                VideoPlayer_RefreshVolBar(pMe);// ˢ������
#if 0
                VideoPlayer_DrawImage(pMe,VIDEOPLAYER_IMAGES_RES_FILE, IDI_PLAY, VIDEOPLAYER_PLAY_X,VIDEOPLAYER_PLAY_Y); //"|>"
#endif
				pMe->m_rtype = TYPE_PLAYER;
                
                if(pMe->m_IsPlaynext)
                {
                    DRAW_BOTTOMBAR(BTBAR_GALLERY_PLAY_BACK);
                }
                else
                {
                    DRAW_BOTTOMBAR(BTBAR_PLAY_BACK);
                }
                
                IDISPLAY_UpdateEx(pMe->m_pDisplay,FALSE);                                    
                break;
                 
            // playback aborted,�쳣�ж�
            case MM_STATUS_ABORT:
                CLOSE_DIALOG(DLGRET_MSGBOX); // ��ʾ����ʧ�ܺ��˳�  
                break;  
            
            case MM_STATUS_PAUSE:
                //ȡ��ͣʱ��,�Ա����ʱ�ָ�����ǰλ��
                pMe->m_bPauseTime= (uint32)pCmdNotify->pCmdData;        
                break;

            case MM_STATUS_RESUME:
                //ȡ�ָ�ʱ��ʱ��,�Ա����ʱ�ָ�����ǰλ��
                //pMe->m_bResumeTime= (uint32)pCmdNotify->pCmdData;
                               
            //���ŵ�ʱ��ÿ��ᷢ����һ��
            case MM_STATUS_TICK_UPDATE: 
                pMe->bCurrentTime=((uint32)pCmdNotify->pCmdData) / 1000;   
                if(!pMe->IsFullScreen)
                {
                    VideoPlayer_RefreshScheduleBar(pMe);//ˢ�½�����
                    VideoPlayer_RefreshPlayingTick(pMe);  
                    IDISPLAY_UpdateEx(pMe->m_pDisplay,FALSE);
                }                        
                break; 
                        
            default:
                break;        
        }
    } 
    
    //return from IMEDIA_GetTotalTime
    if(pCmdNotify->nCmd == MM_CMD_GETTOTALTIME) 
    {
        pMe->bTotalTime = ((uint32)pCmdNotify->pCmdData) / 1000;
    }         
}
/*=================================================================================================================
����:
    DialogTimeoutCallback

˵��:
    ���ݸ�BREW��Ļص��������Ա��趨�Ķ�ʱ����ʱ������ʱ�¼�֪ͨ�Ի���

����:
    pUser [in]: ������ָ�� videoplayer Applet����ṹ��ָ�롣

����ֵ:
    none

��ע:

=================================================================================================================*/
static void DialogTimeoutCallback(void *pUser)
{
    CVideoPlayer *pMe = (CVideoPlayer*)pUser;  

    if (NULL == pMe)
    {
        return;
    }
    
    if (ISHELL_ActiveApplet(pMe->m_pShell) != AEECLSID_VIDEOPLAYER)
    {
        return;
    }
    (void) ISHELL_PostEvent(pMe->m_pShell,AEECLSID_VIDEOPLAYER,EVT_DISPLAYDIALOGTIMEOUT,0,0);
                         
}
#if 1
// ��ȡָ����Ƶ������
static int VideoPlayer_GetFileID(CVideoPlayer *pMe)
{   
    FileInfo  pInfo;
    uint16    fileID=0;//ufileID;
    
    if(pMe->m_pFileDB!=NULL)
    {
        IDATABASE_Release(pMe->m_pFileDB);
        pMe->m_pFileDB = NULL;
     }
 
    //��video���ݿ�
    CMediaGallery_OpenFileInfoDatabase(pMe->m_pShell,MG_VIDEOFILE_DB_NAME,TRUE,&pMe->m_pFileDB);
    if(pMe->m_pFileDB !=NULL)
    {
        // ��ȡ���ݿ��м�¼�ĸ���
        pMe->m_RecordCount=IDATABASE_GetRecordCount(pMe->m_pFileDB);  
        DBGPRINTF("pMe->m_RecordCount=%s",pMe->m_RecordCount);
    }
    for(fileID=0;fileID < pMe->m_RecordCount; fileID++)
    {
		// YY TODO:
        //CMediaGallery_GetFileInfoRecord(pMe->m_pFileDB,fileID, (FileInfo)&pInfo);  
            
        if(STRCMP(pInfo.szName,pMe->m_FileToPlay)== 0)
        {
            break;
        }
    }   

    return fileID;

    #if 0
    if(SUCCESS != IFILEMGR_EnumInit(pMe->pIFileMgr,const char * pszDir,FALSE))
    {
        return EFAILED;
    }  
    for(;;)
    {           
        ret= IFILEMGR_EnumNext(pMe->pIFileMgr,&pInfo);
        
        if(ret!=SUCCESS)
        {
            break;
        }
        //���ļ������ַ���FileName��,�Ա��ڲ���������ʱ����ȡ���ļ������г�ʼ��
       
    }   
    #endif

}

#endif
/*=================================================================================================================
  ��Ļ�л�
=================================================================================================================*/
boolean VideoPlayer_ChangeScrState(CVideoPlayer* pMe,boolean isLockScr)

{
  AEERect rc;
  MEMSET(&rc,NULL,sizeof(rc));
  if(isLockScr)
  	{
      if(pMe->IsFullScreen)
      	{
      	  rc.dx = pMe->m_rc.dx;
		  rc.dy = pMe->m_rc.dy;
      	}
      else
      	{
      	   rc.dx = pMe->m_rc.dx;
           rc.y = VIDEOPLAYER_NAMEPART_H;
           rc.dy = pMe->m_rc.dy - VIDEOPLAYER_NAMEPART_H -  GetBottomBarHeight(pMe->m_pDisplay);

      	}
  	}
  return (SUCCESS == IMEDIA_SetMediaParm((IMedia*)pMe->m_pMedia,MM_PARM_RECT,(int32)&rc,NULL));
}



