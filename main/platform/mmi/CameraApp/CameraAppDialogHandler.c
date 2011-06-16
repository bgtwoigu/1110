/*==============================================================================
//           CameraApp   A P P L E T  M O D U L E
// �ļ�:
//        CameraAppDialogHandler.c
//        ��Ȩ����(c) 2009 Anylook Tech. CO.,LTD.
//        
// �ļ�˵����
//        ��������ģ��Ի����¼�����������ļ�
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
#include "CameraApp_priv.h" 
#include "MediaGallery.h"
#include "appscommonimages.brh"
#ifdef FEATURE_DSP
#include "cam_IF_ait_api.h"
#endif
/*==============================================================================                                 
                                 �궨��ͳ���                                
==============================================================================*/

/*==============================================================================
                                 ��������
==============================================================================*/
// �Ի���IDD_CMAINMENU�¼�������
static boolean  CameraApp_MainMenuHandleEvent(CCameraApp *pMe, 
                                              AEEEvent eCode, 
                                              uint16 wParam, 
                                              uint32 dwParam);
// �Ի���IDD_CAMERA_PHOTO_MODE�¼�������
static boolean  CameraApp_CameraPhotoModeHandleEvent(CCameraApp *pMe, 
                                              AEEEvent eCode, 
                                              uint16 wParam, 
                                              uint32 dwParam);

// �Ի���IDD_CPREVIEW�¼�������
static boolean  CameraApp_PreviewHandleEvent(CCameraApp *pMe, 
                                             AEEEvent eCode, 
                                             uint16 wParam, 
                                             uint32 dwParam);

// �Ի���IDD_CCAMERACFG�¼�������
static boolean CameraApp_CameraCFGHandleEvent(CCameraApp *pMe, 
                                              AEEEvent eCode, 
                                              uint16 wParam, 
                                              uint32 dwParam);

// �Ի���IDD_CPIC�¼�������
static boolean CameraApp_PicHandleEvent(CCameraApp *pMe, 
                                        AEEEvent eCode, 
                                        uint16 wParam, 
                                        uint32 dwParam);

// �Ի���IDD_POPMSG�¼�������
static boolean  CameraApp_PopMSGHandleEvent(CCameraApp *pMe,
                                            AEEEvent   eCode,
                                            uint16     wParam,
                                            uint32     dwParam);

// PopMenu�¼�·�ɴ�����
static boolean CameraApp_RoutePopMenuCommandEvent(CCameraApp *pMe, 
                                                  uint16 wParam);
// PopMenu����ģʽ�˵�������                           
static boolean CameraApp_PopMenu_EnvironmentCommandHandleEvent(CCameraApp *pMe, 
                                                               uint16 wParam);
// PopMenu�����������ò˵�������                                                                                                                 
static boolean CameraApp_PopMenu_QualityCommandHandleEvent(CCameraApp *pMe, 
                                                           uint16 wParam);

// PopMenu���ճߴ����ò˵�������                                                                                                                      
static boolean CameraApp_PopMenu_SizeCommandHandleEvent(CCameraApp *pMe, 
                                                        uint16 wParam);
                                                                                                                    
// PopMenu�����������ò˵�������
static boolean CameraApp_PopMenu_ShutterToneCommandHandleEvent(CCameraApp *pMe, 
                                                               uint16 wParam);
                                                                                                                     
// PopMenu�������ò˵�������
static boolean CameraApp_PopMenu_SelfTimeCommandHandleEvent(CCameraApp *pMe, 
                                                            uint16 wParam);
                                                                                              
// PopMenu�չ��banding������
static boolean CameraApp_PopMenu_BandingCommandHandleEvent(CCameraApp *pMe, 
                                                           uint16 wParam);

// PopMenu camera/video���ø�λ������
static boolean CameraApp_PopMenu_ResetCFGCommandHandleEvent(CCameraApp *pMe, 
                                                            uint16 wParam);

// ���ò˵���ѡ��ͼ��
static void CameraApp_SetCFGMenuIcon(IMenuCtl *pm, 
                                     uint16 listId, 
                                     boolean bSel);

// ��ʼ�����ò˵�
static boolean CameraApp_InitpopMenu(CCameraApp *pMe, 
                                     IMenuCtl *popMenu);

// ��ʼ����������
static void CameraApp_InitCFGData(CCameraApp * pMe);

// ��ʼ�����ջ������ò˵�
static void CameraApp_PopMenu_EnvironmentInit(CCameraApp *pMe, 
                                              IMenuCtl *popMenu);

// ��ʼ�������������ò˵�
static void CameraApp_PopMenu_QualityInit(CCameraApp *pMe, 
                                          IMenuCtl *popMenu);

// ��ʼ�����ճߴ����ò˵�
static void CameraApp_PopMenu_SizeInit(CCameraApp *pMe, 
                                       IMenuCtl *popMenu);

// ��ʼ�����������ò˵�
static void CameraApp_PopMenu_ShutterToneInit(CCameraApp *pMe, 
                                              IMenuCtl *popMenu);

// ��ʼ������ʱ�����ò˵�
static void CameraApp_PopMenu_SelfTimeInit(CCameraApp *pMe, 
                                           IMenuCtl *popMenu);

// ��ʼ��Bandig���ò˵�
static void CameraApp_PopMenu_BandingInit(CCameraApp *pMe, 
                                          IMenuCtl *popMenu);

// ��ʼ���ָ�Ĭ�����ò˵�
static void CameraApp_PopMenu_ResetCFGInit(CCameraApp *pMe, 
                                           IMenuCtl *popMenu);

// ���տ�����������
static void CameraApp_PlayShutterSound(CCameraApp *pMe);

// ����������ص�����
static void CameraApp_MediaNotify(void *pUser, 
                                  AEEMediaCmdNotify *pCmdNotify);

//void CameraApp_AlphaDisplay(CCameraApp *pMe, IMenuCtl *pMenu);

// ���õ����˵�����
static void CameraApp_SetPopMenuRect(CCameraApp *pMe, 
                                     IMenuCtl *popMenu, 
                                     int menuItemSum);

// ��TopBar�ĺ���
static void CameraApp_DrawTopBar(CCameraApp *pMe);

//static void CameraApp_DrawpopMenuBg(CCameraApp *pMe, IMenuCtl *popMenu);

// ��Bottombar�ϵ�Camera/videoͼ�꺯��
static void CameraApp_DrawMidPic(CCameraApp *pMe);

// ��CFGBar����ʾ�ı�
static void CameraApp_DrawCFGPromptText(CCameraApp *pMe);

// Camera Preview����������
static void CameraApp_CPreviewStart(CCameraApp *pMe);

// Camera Snapshot�Ĵ�����
static void CameraApp_RecordSnapShot(CCameraApp *pMe);

// ��T���ĺ���
static boolean CameraApp_FindMemoryCardExist(CCameraApp *pMe);

// ��Sensor�ĺ���
//static boolean CameraApp_FindSensorExist(CCameraApp *pMe);

// Dialog�Ķ�ʱ������
static void CameraApp_DialogTimeout(void *pme);

// �洢�ռ�ļ�⺯��
static boolean CameraApp_IsEnoughfMemorySpace(CCameraApp *pMe);

// ��bottom bar�ϵ��ı�����
static void CameraApp_DrawBottomBarText(CCameraApp *pMe, 
                                        BottomBar_e_Type eBarType);

// ���Ĵ�����
static boolean CameraApp_SelfTimeRecordSnapShot(CCameraApp *pMe);

// ���������ļ���
static boolean CameraApp_SetDateForRecordFileName(CCameraApp *pMe);

// ��ȡ��ǰʱ��
static boolean CameraApp_GetDateForRecordFileName(CCameraApp *pMe,
                                                  char *pszDest, 
                                                  unsigned int fileType);

// �����������մ�С�Ĵ�����
static void CameraApp_SetCameraCaptureSize(CCameraApp *pMe, 
                                           uint16 wParam);

// ���պ���Ƭ������
static void CameraApp_HandleSnapshotPic(CCameraApp *pMe);


// ����Camera Preview�Ĳ���
static void CameraApp_SetParamAfterPreview(CCameraApp * pMe);

void CameraApp_InitCameraCheck(void *po);

static void CameraApp_CreateDirectory(CCameraApp *pMe);
static void CameraApp_UpdateFrame(CCameraApp *pMe);
static int CameraApp_SavePhoto(CCameraApp *pMe);
#ifdef FEATURE_DSP
static int CameraApp_UpdateInit(CCameraApp *pMe);
static int CameraApp_Update(CCameraApp *pMe);
#endif
/*==============================================================================
                                 ȫ������
==============================================================================*/

/*==============================================================================
                                 ���أ���̬������
==============================================================================*/

typedef struct
{
    uint16 dx;
    uint16 dy;
    AECHAR *pStr;
}CCameraSize;

// ���OEMNV_CAMERA_SIZE_MAX
static const CCameraSize g_CameraSizeCFG[] = 
{
#if defined(FEATURE_DISP_160X128)
    {160,128,L"160*128"}, // FULL Screen
    {220,176,L"220*176"}, // QCIF
    {320,240,L"320*240"}, // QVGA
    {640,480,L"640*480"}, // VGA  
#elif defined(FEATURE_DISP_220X176)
    {220,176,L"220*176"}, // QCIF
    {320,240,L"320*240"}, // QVGA
    {640,480,L"640*480"}, // VGA      
#elif defined(FEATURE_DISP_128X160)
    {128,160,L"128*160"}, // FULL Screen
    {176,220,L"176*220"}, // QCIF
    {240,320,L"240*320"}, // QVGA
    //{480,640,L"480*640"}, // VGA    
#elif defined(FEATURE_DISP_176X220)
    {176,220,L"176*220"}, // QCIF
    {240,320,L"240*320"}, // QVGA
    {480,640,L"480*640"}, // VGA    
#elif defined(FEATURE_DISP_240X320)
    //{128,160,L"128*160"}, // FULL Screen
    //{176,220,L"176*220"}, // QCIF
    {240,320,L"240*320"}, // QVGA
    {480,640,L"480*640"}, // VGA   
#elif defined(FEATURE_DISP_320X240)
    //{128,160,L"128*160"}, // FULL Screen
    //{176,220,L"176*220"}, // QCIF
    {240,320,L"320*240"}, // QVGA
    {480,640,L"640*480"}, // VGA       
#else
    {128,128,L"128*128"}, // FULL Screen
    {160,120,L"160*120"}, // QQVGA
    {220,176,L"220*176"}, // QCIF
    {320,240,L"320*240"}, // QVGA
    {640,480,L"640*480"}, // VGA
#endif
    {0,0,NULL}
};

//10�����ص�ѡ��
static const CCameraSize g_CameraSizeCFG_10[] = 
{
#if defined(FEATURE_DISP_160X128)
    {160,128,L"160*128"}, // FULL Screen
    {220,176,L"220*176"}, // QCIF
    {320,240,L"320*240"}, // QVGA
#elif defined(FEATURE_DISP_220X176)
    {220,176,L"220*176"}, // QCIF
    {320,240,L"320*240"}, // QVGA    
#elif defined(FEATURE_DISP_128X160)
    {128,160,L"128*160"}, // FULL Screen
    {176,220,L"176*220"}, // QCIF
    {320,240,L"240*320"}, // QVGA    
#elif defined(FEATURE_DISP_176X220)
    //{128,160,L"128*160"}, // FULL Screen
    {176,220,L"176*220"}, // QCIF
    {240,320,L"240*320"}, // QVGA   
#elif defined(FEATURE_DISP_240X320)
    //{128,160,L"128*160"}, // FULL Screen
    //{176,220,L"176*220"}, // QCIF
    {240,320,L"240*320"}, // QVGA    
#elif defined(FEATURE_DISP_320X240)
    //{128,160,L"128*160"}, // FULL Screen
    //{176,220,L"176*220"}, // QCIF
    {240,320,L"320*240"}, // QVGA        
#else
    {128,128,L"128*128"}, // FULL Screen
    {160,120,L"160*120"}, // QQVGA
    {220,176,L"220*176"}, // QCIF
    {320,240,L"320*240"}, // QVGA
#endif
    {0,0,NULL}
};

/*==============================================================================
                                 ��������
==============================================================================*/


/*==============================================================================
����:
       CameraApp_ShowDialog
˵��:
       ������״̬����������������Ӧ�Ի�����ԴID�ĶԻ���

����:
       pMe [In]: ָ��CameraApp Applet����ṹ��ָ��,�ýṹ����С������ض���Ϣ.
       dlgResId [in]���Ի�����ԴID���������ݸ�ID�����Ի���

����ֵ:
       ��

��ע:
       

==============================================================================*/
void CameraApp_ShowDialog(CCameraApp *pMe,uint16  dlgResId)
{
    int nRet;

    // At most one dialog open at once
    if(ISHELL_GetActiveDialog(pMe->m_pShell) != NULL)
    {
        // Looks like there is one dialog already opened.
        // Flag an error an return without doing anything.
        return;
    }
    
    if (NULL != pMe->m_pDisplay)
    {
        if (dlgResId == IDD_CMAINMENU)
        {
            (void)IDISPLAY_SetPrefs(pMe->m_pDisplay, "a:1", STRLEN("a:1"));
            
            SetDeviceState(DEVICE_TYPE_CAMERA, DEVICE_CAMERA_STATE_OFF);
        }
        else
        {
            if(dlgResId == IDD_CPREVIEW)
            {
                (void)IDISPLAY_SetPrefs(pMe->m_pDisplay, "a:0", STRLEN("a:0"));
            }
            
            SetDeviceState(DEVICE_TYPE_CAMERA, DEVICE_CAMERA_STATE_ON);
        }       
    }

    
    nRet = ISHELL_CreateDialog(pMe->m_pShell, AEE_APPSCAMERAAPP_RES_FILE, dlgResId, NULL);
    
    if(nRet != SUCCESS)
    {
        return;
    }
}

/*==============================================================================
����:
       CameraApp_RouteDialogEvent
˵��:
       ������BREW�¼�·�ɸ���ǰ��Ի�����¼���������

����:
       pMe [In]: ָ��CameraApp Applet����ṹ��ָ��,�ýṹ����С������ض���Ϣ.
       eCode [in]���¼����롣
       wParam [in]�����¼�eCode���������ݡ�
       dwParam [in]�����¼�eCode���������ݡ�

����ֵ:
       TRUE�������¼�������
       FALSE�������¼�û������

��ע:

==============================================================================*/
boolean CameraApp_RouteDialogEvent(CCameraApp *pMe, AEEEvent eCode, uint16 wParam, uint32 dwParam)
{   
    if((NULL == pMe) || (NULL == pMe->m_pActiveDlg))
    {
        return FALSE;
    }
    
    switch(pMe->m_pActiveDlgID)
    {
        case IDD_CMAINMENU:
            return CameraApp_MainMenuHandleEvent(pMe, eCode, wParam, dwParam);
            
        case IDD_CPREVIEW:
            return CameraApp_PreviewHandleEvent(pMe, eCode, wParam, dwParam);

        case IDD_CCAMERACFG:
            return CameraApp_CameraCFGHandleEvent(pMe, eCode, wParam, dwParam);
  
        case IDD_CPIC:
            return CameraApp_PicHandleEvent(pMe, eCode, wParam, dwParam);

        case IDD_POPMSG:
            return CameraApp_PopMSGHandleEvent(pMe, eCode, wParam, dwParam);

        case IDD_CAMERA_PHOTO_MODE:
            return CameraApp_CameraPhotoModeHandleEvent(pMe,eCode,wParam,dwParam);
                           
        default:
            return FALSE;
    }
}

/*==============================================================================
������
       CameraApp_MainMenuHandleEvent
˵����
       IDD_CMAINMENU�Ի����¼�������
       
������
       pMe [in]��ָ��CameraApp Applet����ṹ��ָ�롣�ýṹ����С������ض���Ϣ��
       eCode [in]���¼����롣
       wParam���¼�������ݡ�
       dwParam���¼�������ݡ�
       
����ֵ��
       TRUE�������¼�������
       FALSE�������¼������ԡ�
       
��ע��
       
==============================================================================*/
static boolean CameraApp_MainMenuHandleEvent(CCameraApp *pMe, AEEEvent eCode, uint16 wParam, uint32 dwParam)
{
    PARAM_NOT_REF(dwParam)
        
    IMenuCtl *pMenu = (IMenuCtl*)IDIALOG_GetControl(pMe->m_pActiveDlg, IDC_CAMERA_MAINMENU);
	AECHAR WTitle[40] = {0};
    
    if(pMenu == NULL)
    {
        return FALSE;
    }
   
    switch(eCode) 
    {
        case EVT_DIALOG_INIT:
            pMe->m_bIsPreview = FALSE;
            pMe->m_nCameraState = CAM_START;
            
            pMe->m_wMsgID = IDS_MSG_WAITING;
            pMe->m_nMsgTimeout = TIMEOUT_MS_MSGBOX;
            
			(void)ISHELL_LoadResString(pMe->m_pShell,
                                AEE_APPSCAMERAAPP_RES_FILE,                                
                                IDS_ITEM_CAMERA,
                                WTitle,
                                sizeof(WTitle));
			if(pMe->m_pIAnn != NULL)
        	{
		    	IANNUNCIATOR_SetFieldText(pMe->m_pIAnn,WTitle);
			}
                
            IDISPLAY_SetClipRect(pMe->m_pDisplay, NULL); 

            IANNUNCIATOR_EnableAnnunciatorBar(pMe->m_pIAnn,AEECLSID_DISPLAY1,TRUE);

            // ���ò˵�����
            IMENUCTL_SetProperties(pMenu, MP_WRAPSCROLL|MP_BIND_ITEM_TO_NUMBER_KEY);
            IMENUCTL_SetOemProperties(pMenu, OEMMP_USE_MENU_STYLE);
            
            // ��ʼ���˵���
            IMENUCTL_AddItem(pMenu, AEE_APPSCAMERAAPP_RES_FILE, IDS_ITEM_CAMERA, IDS_ITEM_CAMERA, NULL, NULL);
            IMENUCTL_AddItem(pMenu, AEE_APPSCAMERAAPP_RES_FILE, IDS_ITEM_CAMERA_GALLERY, IDS_ITEM_CAMERA_GALLERY, NULL, NULL);
            #if defined(FEATURE_VERSION_W515V3) || defined(FEATURE_VERSION_S1000T)
            IMENUCTL_AddItem(pMenu, AEE_APPSCAMERAAPP_RES_FILE, IDS_CAMERA_PHOTO_MODE, IDS_CAMERA_PHOTO_MODE, NULL, NULL);
            #endif
            IMENUCTL_SetSel(pMenu, pMe->m_nMainMenuItemSel);
            IMENUCTL_SetBottomBarType(pMenu, BTBAR_SELECT_BACK);
            
            return TRUE;
     
        case EVT_DIALOG_START:
            if(pMe->m_pCamera)
            {
                ICAMERA_Release(pMe->m_pCamera);
                pMe->m_pCamera = NULL;
            }
            (void)ISHELL_PostEvent(pMe->m_pShell, AEECLSID_APP_CAMERA, EVT_USER_REDRAW, NULL, NULL);            
            return TRUE;

        case EVT_DIALOG_END:
            return TRUE;

        case EVT_USER_REDRAW:
            (void)IMENUCTL_Redraw(pMenu);
      
            return TRUE;
            
        case EVT_COMMAND:          
            switch(wParam) 
            {
                case IDS_ITEM_CAMERA: 
#if defined(FEATURE_VERSION_W515V3) || defined(FEATURE_VERSION_S1000T)
                    {
                      boolean cameraphotopath = FALSE;
                      OEM_GetConfig(CFGI_CAMERA_PHOTO_MODE,&cameraphotopath, sizeof(cameraphotopath));
                      pMe->m_isStartFromFacebook = cameraphotopath;
                    } 
#endif
                    // set the annunciator disable
                    IANNUNCIATOR_EnableAnnunciatorBar(pMe->m_pIAnn,AEECLSID_DISPLAY1,FALSE);
                    
                    pMe->m_nMainMenuItemSel = IDS_ITEM_CAMERA;

                    pMe->m_bMemoryCardExist = CameraApp_FindMemoryCardExist(pMe);
#ifdef FEATURE_VERSION_FLEXI203P

                    // ���T�������ڣ�defualt�������ֻ���
                    if(!pMe->m_bMemoryCardExist)
                    {
                        pMe->m_nCameraStorage = OEMNV_CAMERA_STORAGE_PHONE;
                        
                        pMe->m_nCameraSize = OEMNV_CAMERA_SIZE_DEFAULT;

                        (void)ICONFIG_SetItem(pMe->m_pConfig,
                                              CFGI_CAMERA_STORAGE,
                                              &pMe->m_nCameraStorage,
                                              sizeof(pMe->m_nCameraStorage));  

                        (void)ICONFIG_SetItem(pMe->m_pConfig,
                                              CFGI_CAMERA_SIZE,
                                              &pMe->m_nCameraSize,
                                              sizeof(pMe->m_nCameraSize)); 
                    }
                    else
                    {
                        pMe->m_nCameraStorage = OEMNV_CAMERA_STORAGE_MEMORY_CARD;
                    }
#else
                    if ( pMe->m_isStartFromFacebook == TRUE )
					{
						pMe->m_nCameraStorage = OEMNV_CAMERA_STORAGE_PHONE;
					}
					else
					{
                    	pMe->m_nCameraStorage = OEMNV_CAMERA_STORAGE_MEMORY_CARD;
                    }
#endif
                    CLOSE_DIALOG(DLGRET_POPMSG);
                    break;

                case IDS_ITEM_CAMERA_GALLERY:
                    pMe->m_nMainMenuItemSel = IDS_ITEM_CAMERA_GALLERY;
                    CMediaGallery_FileExplorer(GALLERY_PHOTO_BROWSE, NULL);
                    break;
#if defined(FEATURE_VERSION_W515V3) || defined(FEATURE_VERSION_S1000T)                 
                case IDS_CAMERA_PHOTO_MODE:
                    pMe->m_nMainMenuItemSel = IDS_CAMERA_PHOTO_MODE;
                    MSG_FATAL("IDS_CAMERA_PHOTO_MODE-------------",0,0,0);
                    CLOSE_DIALOG(DLGRET_CAMERAPHOTOMODE);
                    break;
#endif

                    
                default:         
                    break;  
            }
            return TRUE;        
             
        case EVT_KEY:
            if(wParam == AVK_CLR)
            {
                CLOSE_DIALOG(DLGRET_CANCELED);
            }
            return TRUE;
   
        default:
            break;
    }
    return FALSE;
}

/*==============================================================================
������
       CameraApp_MainMenuHandleEvent
˵����
       IDD_CameraPhotoMode�Ի����¼�������
       
������
       pMe [in]��ָ��CameraApp Applet����ṹ��ָ�롣�ýṹ����С������ض���Ϣ��
       eCode [in]���¼����롣
       wParam���¼�������ݡ�
       dwParam���¼�������ݡ�
       
����ֵ��
       TRUE�������¼�������
       FALSE�������¼������ԡ�
       
��ע��
       
==============================================================================*/
static boolean CameraApp_CameraPhotoModeHandleEvent(CCameraApp *pMe, AEEEvent eCode, uint16 wParam, uint32 dwParam)
{
      
    IMenuCtl *pMenu = (IMenuCtl*)IDIALOG_GetControl(pMe->m_pActiveDlg, IDC_PHOTOMODE);
	AECHAR WTitle[40] = {0};  
    if(pMenu == NULL)
    {
        return FALSE;
    }
   
    switch(eCode) 
    {
        case EVT_DIALOG_INIT:
            
			(void)ISHELL_LoadResString(pMe->m_pShell,
                                AEE_APPSCAMERAAPP_RES_FILE,                                
                                IDS_CAMERA_PHOTO_MODE,
                                WTitle,
                                sizeof(WTitle));
			if(pMe->m_pIAnn != NULL)
        	{
		    	IANNUNCIATOR_SetFieldText(pMe->m_pIAnn,WTitle);
			}
                
            IDISPLAY_SetClipRect(pMe->m_pDisplay, NULL); 

            IANNUNCIATOR_EnableAnnunciatorBar(pMe->m_pIAnn,AEECLSID_DISPLAY1,TRUE);

            // ���ò˵�����
           // IMENUCTL_SetProperties(pMenu, MP_WRAPSCROLL|MP_BIND_ITEM_TO_NUMBER_KEY);
           // IMENUCTL_SetOemProperties(pMenu, OEMMP_USE_MENU_STYLE);
            
            // ��ʼ���˵���
            IMENUCTL_AddItem(pMenu, AEE_APPSCAMERAAPP_RES_FILE, IDS_CAMERA_PHOTO_PHONE, IDS_CAMERA_PHOTO_PHONE, NULL, NULL);
            IMENUCTL_AddItem(pMenu, AEE_APPSCAMERAAPP_RES_FILE, IDS_CAMERA_PHOTO_TFCARD, IDS_CAMERA_PHOTO_TFCARD, NULL, NULL);
            
           // IMENUCTL_SetSel(pMenu, pMe->m_nMainMenuItemSel);
            IMENUCTL_SetBottomBarType(pMenu, BTBAR_SELECT_BACK);
            
            return TRUE;
     
        case EVT_DIALOG_START:
            {
              boolean cameraphotopath = FALSE;
              uint16 ui16_return = IDS_CAMERA_PHOTO_TFCARD;
              OEM_GetConfig(CFGI_CAMERA_PHOTO_MODE,&cameraphotopath, sizeof(cameraphotopath));
             	if(cameraphotopath)
             	{
             		ui16_return = IDS_CAMERA_PHOTO_PHONE;
             	}
              InitMenuIcons(pMenu);
              SetMenuIcon(pMenu, ui16_return, TRUE);
              IMENUCTL_SetSel(pMenu, ui16_return);
              IMENUCTL_SetProperties(pMenu, MP_UNDERLINE_TITLE|MP_WRAPSCROLL|MP_TEXT_ALIGN_LEFT_ICON_ALIGN_RIGHT);
              IMENUCTL_SetOemProperties( pMenu , OEMMP_USE_MENU_STYLE);
              (void)ISHELL_PostEvent(pMe->m_pShell, AEECLSID_APP_CAMERA, EVT_USER_REDRAW, NULL, NULL);            
           }
            return TRUE;

        case EVT_DIALOG_END:
            return TRUE;

        case EVT_USER_REDRAW:
            (void)IMENUCTL_Redraw(pMenu);
      
            return TRUE;
            
        case EVT_COMMAND: 
            {
                boolean cameraphotopath = FALSE;
                switch(wParam) 
                {
                    case IDS_CAMERA_PHOTO_PHONE:
                        pMe->m_nMainMenuItemSel = IDS_CAMERA_PHOTO_PHONE;
                        cameraphotopath=TRUE;
                        break;
                    case IDS_CAMERA_PHOTO_TFCARD:
                        pMe->m_nMainMenuItemSel = IDS_CAMERA_PHOTO_TFCARD;
                        cameraphotopath=FALSE;
                        break;

                        
                    default:         
                        break;  
                }
                OEM_SetConfig(CFGI_CAMERA_PHOTO_MODE,&cameraphotopath, sizeof(cameraphotopath));
                CLOSE_DIALOG(DLGRET_MAINMENU);
            }
            return TRUE;        
             
        case EVT_KEY:
            if(wParam == AVK_CLR)
            {
                CLOSE_DIALOG(DLGRET_MAINMENU);
            }
            return TRUE;
   
        default:
            break;
    }
    return FALSE;
}

/*==============================================================================
������
       CameraApp_PreviewHandleEvent
˵����
       IDD_CPREVIEW�Ի����¼�������
       
������
       pMe [in]��ָ��CameraApp Applet����ṹ��ָ�롣�ýṹ����С������ض���Ϣ��
       eCode [in]���¼����롣
       wParam���¼�������ݡ�
       dwParam���¼�������ݡ�
       
����ֵ��
       TRUE�������¼�������
       FALSE�������¼������ԡ�
       
��ע��
       
==============================================================================*/
static boolean CameraApp_PreviewHandleEvent(CCameraApp *pMe, AEEEvent eCode, uint16 wParam, uint32 dwParam)
{
    int   nCameraSelfTime = 0;
    
    switch(eCode) 
    {
        case EVT_DIALOG_INIT:
            pMe->m_bCapturePic = FALSE;

            IDISPLAY_SetClipRect(pMe->m_pDisplay, NULL); 


            (void)ICONFIG_GetItem(pMe->m_pConfig,
                                  CFGI_CAMERA_SIZE,
                                  &pMe->m_nCameraSize,
                                  sizeof(pMe->m_nCameraSize));

            (void)ICONFIG_GetItem(pMe->m_pConfig,
                                  CFGI_CAMERA_QUALITY,
                                  &pMe->m_nCameraQuality,
                                  sizeof(pMe->m_nCameraQuality));
            

            (void)ICONFIG_GetItem(pMe->m_pConfig,
                                  CFGI_CAMERA_ENVIROMENT,
                                  &pMe->m_nCameraEnviroment,
                                  sizeof(pMe->m_nCameraEnviroment));

            (void)ICONFIG_GetItem(pMe->m_pConfig,
                                  CFGI_CAMERA_BANDING,
                                  &pMe->m_nCameraBanding,
                                  sizeof(pMe->m_nCameraBanding));
                                  
            IDIALOG_SetProperties((IDialog *)dwParam, DLG_NOT_REDRAW_AFTER_START);
            return TRUE;
            
        case EVT_DIALOG_START:     
        
			//Add By zzg 2010_09_01  			
#ifdef FEATURE_APP_MUSICPLAYER	
			if (app_media_scheduler() == APP_MEDIA_IMPACT_BY_MP3)
			{
				ISHELL_SendEvent(pMe->m_pShell,
				     AEECLSID_APP_MUSICPLAYER,
				     EVT_ALARM,
				     TRUE,
				     TRUE);
			}
#endif         
			//Add End
                	
            pMe->m_bCanCapture = CameraApp_IsEnoughfMemorySpace(pMe);
            // For redraw the dialog
            (void)ISHELL_PostEvent(pMe->m_pShell,
                                   AEECLSID_APP_CAMERA,
                                   EVT_USER_REDRAW,
                                   0,
                                   0);  
            return TRUE;
            
        case EVT_DIALOG_END:       

			//Add By zzg 2010_09_01  			
#ifdef FEATURE_APP_MUSICPLAYER
		    if (app_media_scheduler() == APP_MEDIA_IMPACT_BY_MP3)
		    {
		        ISHELL_SendEvent(pMe->m_pShell,
		                         AEECLSID_APP_MUSICPLAYER,
		                         EVT_ALARM,
		                         FALSE,
		                         TRUE);
		    }
 #endif
 			//Add End
            
            ISHELL_CancelTimer(pMe->m_pShell, NULL, pMe);
            return TRUE;
            
        case EVT_USER_REDRAW:
            // camera preview start....
#ifndef FEATURE_DSP            
            if(pMe->m_pCamera && (!pMe->m_bIsPreview))
            {
                CameraApp_CPreviewStart(pMe);
                return TRUE;
            }            
            if(pMe->m_bRePreview && pMe->m_pCamera)
            {
                CameraApp_CPreviewStart(pMe);
                pMe->m_bRePreview = FALSE;
                return TRUE;
                
            }
#else
			if(pMe->m_pCamera && (!pMe->m_bIsPreview))
            {
                CameraApp_CPreviewStart(pMe);
                return TRUE;
            }            
            if(pMe->m_bRePreview && pMe->m_pCamera)
            {
                CameraApp_CPreviewStart(pMe);
                pMe->m_bRePreview = FALSE;
                return TRUE;
                
            }
            pMe->m_nCameraState = CAM_PREVIEW;  
            CameraApp_UpdateInit(pMe);
#endif
            if(pMe->m_nLeftTime)
            {
                IImage *pImage = NULL;
                
                pImage = ISHELL_LoadResImage(pMe->m_pShell, CAMERAAPP_IMAGE_RES_FILE, IDI_COUNT_DOWN);
                
                if(pImage)
                {
                    IImage_SetDrawSize(pImage, 60, 60);
                    
                    IImage_SetOffset(pImage, (pMe->m_nLeftTime)*60, 0);

                    IImage_Draw(pImage, (pMe->m_rc.dx-60)/2, (pMe->m_rc.dy-60)/2);

                    IImage_Release(pImage);

                    pImage = NULL;
                }
            }
            
            CameraApp_DrawBottomBarText(pMe, BTBAR_OPTION_BACK);
            
            CameraApp_DrawMidPic(pMe);
#ifdef FEATURE_DSP
            CameraApp_Update(pMe);
#else
            IDISPLAY_UpdateEx(pMe->m_pDisplay, FALSE);
#endif
            return TRUE;

        case EVT_KEY:
            switch(wParam)
            {
                case AVK_SELECT: 
                    if(pMe->m_bCapturePic == FALSE)
                    {
                        CLOSE_DIALOG(DLGRET_CAMERACFG);
                    }
                    break;

                default:
                    break;
            }
            return TRUE;
            
        case EVT_KEY_RELEASE:
        case EVT_KEY_HELD:         
            return TRUE;

        case EVT_KEY_PRESS:           
            // �����ǰ�����գ�������AVK_END��������İ����¼�
            if((wParam != AVK_END) && (pMe->m_bCapturePic == TRUE))
            {
                return TRUE;
            }
            
            switch(wParam){
            case AVK_END:
                ISHELL_CancelTimer(pMe->m_pShell, NULL, pMe);
                if(pMe->m_nCameraState == CAM_PREVIEW)
                {
                    ICAMERA_Stop(pMe->m_pCamera);
                    pMe->m_nCameraState = CAM_STOP;
                }
                return FALSE;

            case AVK_CLR:
                if(pMe->m_isFormQuicktest)
                {
                    ISHELL_CancelTimer(pMe->m_pShell, NULL, pMe);
                    if(pMe->m_nCameraState == CAM_PREVIEW)
                    {
                        ICAMERA_Stop(pMe->m_pCamera);
                        pMe->m_nCameraState = CAM_STOP;
                    }                        
                    pMe->m_isFormQuicktest = FALSE;
                    pMe->m_nLeftTime = 0;
                    ISHELL_CloseApplet(pMe->m_pShell, FALSE);
                    return TRUE;
                }
                if(SUCCESS == ICAMERA_Stop(pMe->m_pCamera))
                {
                    pMe->m_bIsPreview = FALSE;
                    pMe->m_nLeftTime  = 0;
                    pMe->m_nCameraState = CAM_STOP;
                    CLOSE_DIALOG(DLGRET_CANCELED);
                }                    
                break;

            case AVK_INFO:
                // ��ֹ���ٰ���������hotkey Text������LCD�� 
                ISHELL_CancelTimer(pMe->m_pShell, NULL, pMe);
                
                if(!pMe->m_bCanCapture)
                {
                	if ( pMe->m_isStartFromFacebook == TRUE)
                	{
                    	pMe->m_wMsgID = IDS_MSG_NOMEMORY;
                    }
                    else
                    {
						if(pMe->m_bMemoryCardExist)
	                    {
	                        pMe->m_wMsgID = IDS_MSG_NOMEMORY;
	                    }
	                    else
	                    {
	                    #ifdef FEATURE_VERSION_FLEXI203P
							pMe->m_wMsgID = IDS_MSG_PHONE_MEMERY_FULL_AND_NOSDCARD;
                       	#else
                        	pMe->m_wMsgID = IDS_MSG_NOSDCARD;
                       	#endif
	                    }
                    }

                    pMe->m_nMsgTimeout = TIMEOUT_MS_MSGBOX;
                    ICAMERA_Stop(pMe->m_pCamera);
                    pMe->m_bIsPreview = FALSE;
                    CLOSE_DIALOG(DLGRET_POPMSG);
                    return TRUE;
                }
                
                switch(pMe->m_nSelfTimeItemSel)
                {
                    case IDS_SELFTIME_OFF:
                        nCameraSelfTime = 0;
                        break;

                    case IDS_SELFTIME_SEC1:
                        nCameraSelfTime = 6;
                        break;

                    case IDS_SELFTIME_SEC2:
                        nCameraSelfTime = 11;
                        break;

                    case IDS_SELFTIME_SEC3:
                        nCameraSelfTime = 16;
                        break;

                    default:
                        return FALSE;        
                }
                
                pMe->m_nLeftTime = nCameraSelfTime;              
                
                if(nCameraSelfTime == 0)
                {
                    pMe->m_nCameraState = CAM_CAPTURE;
#ifdef FEATURE_DSP
                    CameraApp_RecordSnapShot(pMe);
                    ICAMERA_Stop(pMe->m_pCamera);
                    CameraApp_SavePhoto(pMe);

                    CameraApp_PlayShutterSound(pMe);
                    CLOSE_DIALOG(DLGRET_PICMENU);
#else
                    ICAMERA_Stop(pMe->m_pCamera);
#endif
                }
                else
                {
                    // ����ʱ����                       
                    CameraApp_SelfTimeRecordSnapShot(pMe);
                }
                     
                break;
                    
                default:
                    break;     
            } 
            return TRUE;

        case EVT_NO_CLOSEBACKLIGHT:
            return TRUE;
            
        default:
            break;
    }
    
    return FALSE;
}

/*==============================================================================
������
       CameraApp_CameraCFGHandleEvent
˵����
       IDD_CCAMERACFG�Ի����¼�������
       
������
       pMe [in]��ָ��CameraApp Applet����ṹ��ָ�롣�ýṹ����С������ض���Ϣ��
       eCode [in]���¼����롣
       wParam���¼�������ݡ�
       dwParam���¼�������ݡ�
       
����ֵ��
       TRUE�������¼�������
       FALSE�������¼������ԡ�
       
��ע��
       
==============================================================================*/
static boolean CameraApp_CameraCFGHandleEvent(CCameraApp *pMe, AEEEvent eCode, uint16 wParam, uint32 dwParam)
{
    IMenuCtl *popMenu = (IMenuCtl*)IDIALOG_GetControl(pMe->m_pActiveDlg, IDC_CAMERA_CFGMENU);    
    
    if(popMenu == NULL)
    {
        return FALSE;
    }
    switch(eCode) 
    {
        case EVT_DIALOG_INIT:
            IMENUCTL_SetProperties(popMenu, MP_WRAPSCROLL|MP_TRANSPARENT_UNSEL|MP_NO_REDRAW);
            IMENUCTL_SetOemProperties(popMenu, OEMMP_GRAPHIC_UNDERLINE);
            
            CameraApp_InitpopMenu(pMe, popMenu);
            IDIALOG_SetProperties((IDialog *)dwParam, DLG_NOT_REDRAW_AFTER_START);
            IMENUCTL_SetActive(popMenu, TRUE);
            return TRUE;
     
        case EVT_DIALOG_START:      
        	//Add By zzg 2010_09_01          	
#ifdef FEATURE_APP_MUSICPLAYER
		    if (app_media_scheduler() == APP_MEDIA_IMPACT_BY_MP3)
		    {
		        ISHELL_SendEvent(pMe->m_pShell,
		                         AEECLSID_APP_MUSICPLAYER,
		                         EVT_ALARM,
		                         TRUE,
		                         TRUE);
		    }
 #endif
 			//Add End
#ifdef FEATURE_DSP
            ISHELL_PostEvent(pMe->m_pShell, AEECLSID_APP_CAMERA, EVT_USER_REDRAW, NULL, NULL); 
#endif
            return TRUE;
            
        case EVT_DIALOG_END:
        
        	//Add By zzg 2010_09_01          	
#ifdef FEATURE_APP_MUSICPLAYER
		    if (app_media_scheduler() == APP_MEDIA_IMPACT_BY_MP3)
		    {
		        ISHELL_SendEvent(pMe->m_pShell,
		                         AEECLSID_APP_MUSICPLAYER,
		                         EVT_ALARM,
		                         FALSE,
		                         TRUE);
		    }
 #endif
 			//Add End
            if(pMe->m_pDisplay != NULL)
            {
                IDISPLAY_SetClipRect(pMe->m_pDisplay, NULL);
            }
#ifdef FEATURE_CAMERA_NOFULLSCREEN
            IDISPLAY_FillRect(pMe->m_pDisplay, &pMe->m_rc, RGB_BLACK);
            IDISPLAY_Update(pMe->m_pDisplay);
#endif
            return TRUE;
      
        case EVT_USER_REDRAW: 
        	MSG_FATAL("CameraApp_CameraCFGHandleEvent.....EVT_USER_REDRAW",0,0,0);
#ifndef FEATURE_DSP
			if(pMe->m_bRePreview)
            {
                CameraApp_CPreviewStart(pMe);
                pMe->m_bRePreview = FALSE;
            }
#else
			if(pMe->m_bRePreview)
            {
                CameraApp_CPreviewStart(pMe);
                pMe->m_bRePreview = FALSE;
            }
            pMe->m_nCameraState = CAM_PREVIEW; 
            pMe->m_bRePreview = TRUE;
            CameraApp_UpdateInit(pMe);
#endif
            IDISPLAY_SetClipRect(pMe->m_pDisplay, NULL);
            CameraApp_DrawBottomBarText(pMe, BTBAR_SELECT_BACK);
            CameraApp_DrawTopBar(pMe);   
            CameraApp_DrawCFGPromptText(pMe);                        
            
            IMENUCTL_Redraw(popMenu);
#ifdef FEATURE_DSP
            CameraApp_Update(pMe);
#endif
            
            return TRUE;

        case EVT_KEY:
        case EVT_KEY_RELEASE:
        	#ifdef FEATURE_DSP
            ISHELL_SendEvent(pMe->m_pShell, AEECLSID_APP_CAMERA, EVT_USER_REDRAW, NULL, NULL);
			#endif
            return TRUE;
            
        case EVT_KEY_PRESS:
            switch(wParam)
            {
                case AVK_CLR:
                    CLOSE_DIALOG(DLGRET_CANCELED);
                    return TRUE;

                case AVK_LEFT:  
                    MSG_FATAL("CameraApp_CameraCFGHandleEvent:L:%d",pMe->m_nCameraCFG,0,0);
                    if(pMe->m_nCameraCFG == CAMERACFGFIRST)
                    {
                        pMe->m_nCameraCFG = CAMERACFGLAST;
                    }
                    else
                    {
                        pMe->m_nCameraCFG--;
                    }
                    break;
   
                case AVK_RIGHT:
                    MSG_FATAL("CameraApp_CameraCFGHandleEvent:R:%d",pMe->m_nCameraCFG,0,0);
                    if(pMe->m_nCameraCFG == CAMERACFGLAST)
                    {
					    pMe->m_nCameraCFG = CAMERACFGFIRST;
                    }
                    else
                    {
                        pMe->m_nCameraCFG++;
                    }
                    MSG_FATAL("CameraApp_CameraCFGHandleEvent RIGHT END:%d",pMe->m_nCameraCFG,0,0);
                    break;

                case AVK_UP:
                case AVK_DOWN:   
                	#ifdef FEATURE_DSP
            		ISHELL_SendEvent(pMe->m_pShell, AEECLSID_APP_CAMERA, EVT_USER_REDRAW, NULL, NULL);
					#endif
                    return TRUE;

                case AVK_END:
                	MSG_FATAL("AVK_END...................",0,0,0);
                	ISHELL_CancelTimer(pMe->m_pShell, NULL, pMe);
                	if(pMe->m_nCameraState == CAM_PREVIEW)
                	{
                    	ICAMERA_Stop(pMe->m_pCamera);
                    	pMe->m_nCameraState = CAM_STOP;
                	}
                	MSG_FATAL("AVK_END...................2",0,0,0);
                return FALSE;
                    
                default:
                    return TRUE;
            }
            
            //���Ҽ��л���ͬ�����ò˵�����Ҫ���³�ʼ���˵���
            CameraApp_InitpopMenu(pMe, popMenu);
#ifdef FEATURE_DSP
			
            ISHELL_SendEvent(pMe->m_pShell, AEECLSID_APP_CAMERA, EVT_USER_REDRAW, NULL, NULL);
#endif
            return TRUE;//ISHELL_SendEvent(pMe->m_pShell, AEECLSID_APP_CAMERA, EVT_USER_REDRAW, NULL, NULL);
            
        case EVT_COMMAND:        
            {
                int n = SUCCESS;
                MSG_FATAL("EVT_COMMAND......................",0,0,0);
#ifdef FEATURE_DSP
                
#endif
                DBGPRINTF("cfg key code ----------------%d\n", wParam);
                n = CameraApp_RoutePopMenuCommandEvent(pMe, wParam);
#ifdef FEATURE_DSP
				//if(pMe->m_bRePreview)
				//{
				//	CameraApp_CPreviewStart(pMe);
				//	pMe->m_bRePreview = FALSE;
				//}

                CameraApp_Update(pMe); 
                
#endif
                return n;
            } 
        case EVT_NO_CLOSEBACKLIGHT:
            return TRUE;
            
        default:
            break;
    }
    return FALSE;
}

/*==============================================================================
������
       CameraApp_PicHandleEvent
˵����
       IDD_CPIC�Ի����¼�������
       
������
       pMe [in]��ָ��CameraApp Applet����ṹ��ָ�롣�ýṹ����С������ض���Ϣ��
       eCode [in]���¼����롣
       wParam���¼�������ݡ�
       dwParam���¼�������ݡ�
       
����ֵ��
       TRUE�������¼�������
       FALSE�������¼������ԡ�
       
��ע��
       
==============================================================================*/
static boolean CameraApp_PicHandleEvent(CCameraApp *pMe, AEEEvent eCode, uint16 wParam, uint32 dwParam)
{  
    switch(eCode)
    {
        case EVT_DIALOG_INIT:          
            return TRUE;
 
        case EVT_DIALOG_START:   
        	//Add By zzg 2010_09_01  			
#ifdef FEATURE_APP_MUSICPLAYER	
			if (app_media_scheduler() == APP_MEDIA_IMPACT_BY_MP3)
			{
				ISHELL_SendEvent(pMe->m_pShell,
				     AEECLSID_APP_MUSICPLAYER,
				     EVT_ALARM,
				     TRUE,
				     TRUE);
			}
#endif         
			//Add End
			
            ISHELL_PostEvent(pMe->m_pShell, AEECLSID_APP_CAMERA, EVT_USER_REDRAW, NULL, NULL);
            return TRUE;
            
        //Add By zzg 2010_09_01      
        case EVT_DIALOG_END:
        {        	
#ifdef FEATURE_APP_MUSICPLAYER
		    if (app_media_scheduler() == APP_MEDIA_IMPACT_BY_MP3)
		    {
		        ISHELL_SendEvent(pMe->m_pShell,
		                         AEECLSID_APP_MUSICPLAYER,
		                         EVT_ALARM,
		                         FALSE,
		                         TRUE);
		    } 
#endif 			
 			return TRUE;
 		}
 		//Add End    
            
        case EVT_USER_REDRAW:
            IDISPLAY_SetClipRect(pMe->m_pDisplay, NULL); 
            IDISPLAY_ClearScreen(pMe->m_pDisplay);
            CameraApp_HandleSnapshotPic(pMe);
            return TRUE;
            
        case EVT_KEY:
            switch(wParam)
            {
                case AVK_CLR:                                                       
                    if(pMe->m_pFileMgr)
                    {
                        IFILEMGR_Remove(pMe->m_pFileMgr, pMe->m_sCurrentFileName);
                    }
                    break;
                    
                case AVK_SELECT:
                case AVK_INFO:
                    break;
                    
                default:
                    return TRUE;
            } 
            
            pMe->m_wMsgID = IDS_DONE;
            pMe->m_nMsgTimeout = TIMEOUT_MS_MSGDONE;
            CLOSE_DIALOG(DLGRET_POPMSG);
            return TRUE;
 
        case EVT_KEY_PRESS:   
            return TRUE;
 
        case EVT_KEY_RELEASE:
            return TRUE;
 
        case EVT_KEY_HELD:
            return TRUE;  
    }
    return FALSE;

}

/*==============================================================================
������
       CameraApp_PopMSGHandleEvent
˵����
       IDD_POPMSGC�Ի����¼�������
       
������
       pMe [in]��ָ��CameraApp Applet����ṹ��ָ�롣�ýṹ����С������ض���Ϣ��
       eCode [in]���¼����롣
       wParam���¼�������ݡ�
       dwParam���¼�������ݡ�
       
����ֵ��
       TRUE�������¼�������
       FALSE�������¼������ԡ�
       
��ע��
       
==============================================================================*/
static boolean  CameraApp_PopMSGHandleEvent(CCameraApp *pMe,
                                            AEEEvent   eCode,
                                            uint16     wParam,
                                            uint32     dwParam)
{
    static IStatic *pStatic = NULL;       
    
    switch (eCode)
    {
        case EVT_DIALOG_INIT: 
            return TRUE;
        
        case EVT_DIALOG_START:
        	//Add By zzg 2010_09_01  			
#ifdef FEATURE_APP_MUSICPLAYER	
			if (app_media_scheduler() == APP_MEDIA_IMPACT_BY_MP3)
			{
				ISHELL_SendEvent(pMe->m_pShell,
				     AEECLSID_APP_MUSICPLAYER,
				     EVT_ALARM,
				     TRUE,
				     TRUE);
			}
#endif         
			//Add End
			
            if((pMe->m_nMsgTimeout != 0) && (pMe->m_wMsgID != IDS_MSG_WAITING))
            {
                (void)ISHELL_SetTimer(pMe->m_pShell,
                                      pMe->m_nMsgTimeout,
                                      CameraApp_DialogTimeout,
                                      pMe);
            }
            
            // For redraw the dialog
            (void)ISHELL_PostEvent(pMe->m_pShell,
                                   AEECLSID_APP_CAMERA,
                                   EVT_USER_REDRAW,
                                   0,
                                   0);  
            if(NULL == pStatic)
            {
                 AEERect rect = {0};
                 
                 if(AEE_SUCCESS != ISHELL_CreateInstance(pMe->m_pShell,
                                                         AEECLSID_STATIC,
                                                         (void **)&pStatic))  
                 {
                     return FALSE;
                 }
                 
                 ISTATIC_SetRect(pStatic, &rect);  
            }
            return TRUE;                                                                       
            
        case EVT_USER_REDRAW:
            if(pMe->m_ePreState == STATE_CPIC)
            {
                IDISPLAY_ClearScreen(pMe->m_pDisplay);
            }
            
            {             
                PromptMsg_Param_type PromptMsg={0};  
                AECHAR  wstrText[128];  

                if((pMe->m_wMsgID == IDS_CONFIRM_OFF_FM)||(pMe->m_wMsgID == IDS_CONFIRM_OFF_MP))
                {
                    (void)ISHELL_LoadResString(pMe->m_pShell,
                                               AEE_APPSCOMMONRES_LANGFILE,
                                               pMe->m_wMsgID,
                                               wstrText,
                                               sizeof(wstrText));
                }
                else
                {      
                    (void)ISHELL_LoadResString(pMe->m_pShell,
                                               AEE_APPSCAMERAAPP_RES_FILE,
                                               pMe->m_wMsgID,
                                               wstrText,
                                               sizeof(wstrText));
                }
               
                switch(pMe->m_wMsgID)
                {
                  #ifdef FEATURE_VERSION_FLEXI203P
					case IDS_MSG_PHONE_MEMERY_FULL_AND_NOSDCARD:
                  #endif
                    case IDS_MSG_NOSDCARD:
                    case IDS_MSG_NOCAMERA:
                    case IDS_MSG_NOMEMORY:
                    case IDS_MSG_CAPTURE_FAILED:
                        PromptMsg.ePMsgType = MESSAGE_WARNNING;
                        break;

                    case IDS_DONE:
                        PromptMsg.ePMsgType = MESSAGE_INFORMATIVE;
                        break;

                    case IDS_CONFIRM_OFF_FM:
                    case IDS_CONFIRM_OFF_MP:
                        PromptMsg.ePMsgType = MESSAGE_CONFIRM;
                        PromptMsg.eBBarType = BTBAR_OK_BACK;
                        break;

                    case IDS_MSG_WAITING:
                        PromptMsg.ePMsgType = MESSAGE_WAITING;
                        break;
                        
                    default:
                        PromptMsg.ePMsgType = MESSAGE_NONE;
                        break;
                }
                
                PromptMsg.pwszMsg = wstrText;
                
                DrawPromptMessage(pMe->m_pDisplay, pStatic, &PromptMsg);                         
            }  
            
            IDISPLAY_UpdateEx(pMe->m_pDisplay,FALSE);
#ifdef FEATURE_DSP
            if(pMe->m_wMsgID == IDS_MSG_WAITING || pMe->m_wMsgID == IDS_DONE
               || pMe->m_wMsgID == IDS_MSG_NOSDCARD || pMe->m_wMsgID == IDS_MSG_NOMEMORY) // &&(!pMe->m_pCamera)
            {
               CameraApp_InitCameraCheck(pMe);    

                (void)ISHELL_PostEvent(pMe->m_pShell,
                                       AEECLSID_APP_CAMERA,
                                       EVT_APP_DIALOG_TIMEOUT,
                                       0,
                                       0);
            }
#else
            if((pMe->m_wMsgID == IDS_MSG_WAITING) &&(!pMe->m_pCamera))
            {
                CALLBACK_Init(&pMe->m_CallBack,
                              CameraApp_InitCameraCheck,
                              (void *)pMe);          
            
                ISHELL_Resume(pMe->m_pShell, &pMe->m_CallBack);

                (void)ISHELL_PostEvent(pMe->m_pShell,
                                       AEECLSID_APP_CAMERA,
                                       EVT_APP_DIALOG_TIMEOUT,
                                       0,
                                       0);
            }
#endif
            
            return TRUE;
            
        case EVT_DIALOG_END:
        	//Add By zzg 2010_09_01  			
#ifdef FEATURE_APP_MUSICPLAYER	
			if (app_media_scheduler() == APP_MEDIA_IMPACT_BY_MP3)
			{
				ISHELL_SendEvent(pMe->m_pShell,
				     AEECLSID_APP_MUSICPLAYER,
				     EVT_ALARM,
				     FALSE,
				     TRUE);
			}
#endif         
			//Add End
			
            if(pStatic != NULL)
            {
                ISTATIC_Release(pStatic); 
            }
            pStatic = NULL;                 
            (void)ISHELL_CancelTimer(pMe->m_pShell,
                                     CameraApp_DialogTimeout,
                                     pMe);
            return TRUE;
            
        case EVT_KEY:
            // ��Preview�� ʱ�����ư���
            if(pMe->m_wMsgID == IDS_MSG_WAITING)
            {
                return TRUE;
            }
            
            switch (wParam)
            {
                case AVK_INFO:
                    return TRUE;
                    
                case AVK_CLR:
                     CLOSE_DIALOG(DLGRET_CANCELED);
                     return TRUE;
                     
                case AVK_SELECT:
                    if(pMe->m_nMsgTimeout == 0)
                    {
                        int nMediaDevice;
                        nMediaDevice = app_media_scheduler();

                        switch(nMediaDevice)
                        {
                            case APP_MEDIA_IMPACT_BY_FM:
                                ISHELL_SendEvent(pMe->m_pShell, AEECLSID_APP_FMRADIO, EVT_CLOSEAPP, 0, 0);  
                                break;

                            case APP_MEDIA_IMPACT_BY_MP3:
                                ISHELL_SendEvent(pMe->m_pShell, AEECLSID_APP_MUSICPLAYER, EVT_CLOSEAPP, 0, 0);
                                break;

                            default:
                                break;
                        }
                  
                        CLOSE_DIALOG(DLGRET_MAINMENU);

                    }
                    else
                    {
                        // ���´��������ڴ˽���Ŀ��ٰ���
                        (void)ISHELL_SetTimer( pMe->m_pShell,
                                                TIMEOUT_MS_QUICK,
                                                CameraApp_DialogTimeout,
                                                pMe );
                    }
                    return TRUE;
                    
                default:
                    break;                    
            }
            return TRUE;

        
        case EVT_COMMAND:
            break;
            
        case EVT_APP_DIALOG_TIMEOUT:  
            if(pMe->m_wMsgID == IDS_MSG_WAITING)
            {
                CLOSE_DIALOG(DLGRET_PREVIEW);
            }
            else
            {
                CLOSE_DIALOG(DLGRET_CANCELED);
            }
            
            return TRUE;         
            
        default:
            break;            
    }
    
    return FALSE;
}

/*==============================================================================
������
       CameraApp_RoutePopMenuCommandEvent
˵����
       �ַ����ò˵������¼�����·�ɺ���
       
������
       pMe [in]��ָ��CameraApp Applet����ṹ��ָ�롣�ýṹ����С������ض���Ϣ��
       wParam���¼�������ݡ�
       
����ֵ��
       TRUE�������¼�������
       FALSE�������¼������ԡ�
       
��ע��
       
==============================================================================*/
static boolean CameraApp_RoutePopMenuCommandEvent(CCameraApp *pMe, uint16 wParam)
{
    if(NULL == pMe)
    {
        return FALSE;
    }

    if(NULL == pMe->m_pActiveDlg)
    {
        return FALSE;
    }

    if(pMe->m_pActiveDlgID == IDD_CCAMERACFG)
    {
        switch(pMe->m_nCameraCFG)
        {
            case CAMERACFGENVIRMENT:
                return CameraApp_PopMenu_EnvironmentCommandHandleEvent(pMe, wParam);

            case CAMERACFGQUALITY:
                return CameraApp_PopMenu_QualityCommandHandleEvent(pMe, wParam);
   
            case CAMERACFGSIZE:
                return CameraApp_PopMenu_SizeCommandHandleEvent(pMe, wParam);
   
            case CAMERACFGTONE:
                return CameraApp_PopMenu_ShutterToneCommandHandleEvent(pMe, wParam);
   
            case CAMERACFGSELFTIME:
                return CameraApp_PopMenu_SelfTimeCommandHandleEvent(pMe, wParam);
   #if 0
            case CAMERACFGBANDING:
                return CameraApp_PopMenu_BandingCommandHandleEvent(pMe, wParam);
                #endif
                
            case CAMERACFGRESET:
                return CameraApp_PopMenu_ResetCFGCommandHandleEvent(pMe, wParam);
   
            default:
                return FALSE;
        }
    }
    return TRUE;
}

/*==============================================================================
������
       CameraApp_PopMenu_EnvironmentCommandHandleEvent
˵����
       �������ò˵��������
       
������
       pMe [in]��ָ��CameraApp Applet����ṹ��ָ�롣�ýṹ����С������ض���Ϣ��
       wParam���¼�������ݡ�
       
����ֵ��
       TRUE�������¼�������
       FALSE�������¼������ԡ�

       
��ע��
       
==============================================================================*/
static boolean CameraApp_PopMenu_EnvironmentCommandHandleEvent(CCameraApp *pMe, uint16 wParam)
{ 
    uint32 dwFPS = 5;
        
    switch(pMe->m_pActiveDlgID)
    {
        case IDD_CCAMERACFG:
            switch(wParam)
            {
                case IDS_ENVIR_MODE_AUTO:
                    pMe->m_nCameraEnviroment = OEMNV_CAMERA_ENVIR_AUTO;
                    dwFPS = 0;
                    break;

                case IDS_ENVIR_MODE_NIGHT:
                    pMe->m_nCameraEnviroment = OEMNV_CAMERA_ENVIR_NIGHT;
                    dwFPS = 3;
                    break;

                case IDS_ENVIR_MODE_SUNNY:
                    pMe->m_nCameraEnviroment = OEMNV_CAMERA_ENVIR_SUNNY;
                    dwFPS = 1;
                    break;

                case IDS_ENVIR_MODE_CLOUDY:
                    pMe->m_nCameraEnviroment = OEMNV_CAMERA_ENVIR_CLOUDY;
                    dwFPS = 2;
                    break;

                default:
                    break;                 
            }

            (void)ICONFIG_SetItem(pMe->m_pConfig,
                                  CFGI_CAMERA_ENVIROMENT,
                                  &pMe->m_nCameraEnviroment,
                                  sizeof(pMe->m_nCameraEnviroment));
            
            ICAMERA_SetFramesPerSecond(pMe->m_pCamera, dwFPS);
            break;

        case IDD_CVIDEOCFG:
            switch(wParam)
            {
                case IDS_ENVIR_MODE_AUTO:
                    pMe->m_nCameraEnviroment = OEMNV_CAMERA_ENVIR_AUTO;
                    dwFPS = 0;
                    break;

                case IDS_ENVIR_MODE_NIGHT:
                    pMe->m_nCameraEnviroment = OEMNV_CAMERA_ENVIR_NIGHT;
                    dwFPS = 3;
                    break;

                case IDS_ENVIR_MODE_SUNNY:
                    pMe->m_nCameraEnviroment = OEMNV_CAMERA_ENVIR_SUNNY;
                    dwFPS = 1;
                    break;

                case IDS_ENVIR_MODE_CLOUDY:
                    pMe->m_nCameraEnviroment = OEMNV_CAMERA_ENVIR_CLOUDY;
                    dwFPS = 2;
                    break;

                default:
                    break;                 
            }

            (void)ICONFIG_SetItem(pMe->m_pConfig,
                                  CFGI_VIDEO_ENVIROMENT,
                                  &pMe->m_nCameraEnviroment,
                                  sizeof(pMe->m_nCameraEnviroment));
            
            ICAMERA_SetFramesPerSecond(pMe->m_pCamera, dwFPS);
            break;
            
    }
    
    CLOSE_DIALOG(DLGRET_CANCELED); 
    return TRUE;
}

/*==============================================================================
������
       CameraApp_PopMenu_QualityCommandHandleEvent
˵����
       �����������ò˵��������
       
������
       pMe [in]��ָ��CameraApp Applet����ṹ��ָ�롣�ýṹ����С������ض���Ϣ��
       wParam���¼�������ݡ�
       
����ֵ��
       TRUE�������¼�������
       FALSE�������¼������ԡ�
      
��ע��
       
==============================================================================*/
static boolean CameraApp_PopMenu_QualityCommandHandleEvent(CCameraApp *pMe, uint16 wParam)
{
    int16 quality = 0;
    
    (void)ICONFIG_GetItem(pMe->m_pConfig,
                          CFGI_CAMERA_QUALITY,
                          &pMe->m_nCameraQuality,
                          sizeof(pMe->m_nCameraQuality));

    if(wParam != pMe->m_nCameraQuality)
    {
        switch(wParam)
        {
            case IDS_QUALITY_HIGH:
                pMe->m_nCameraQuality = OEMNV_CAMERA_QUALITY_HIGH; 
                quality = CAMERA_APP_QUALITY_HIGH;
                break;

            case IDS_QUALITY_MED:
                pMe->m_nCameraQuality = OEMNV_CAMERA_QUALITY_MED;
                quality = CAMERA_APP_QUALITY_MED;
                break;

            case IDS_QUALITY_LOW:
                pMe->m_nCameraQuality = OEMNV_CAMERA_QUALITY_LOW;
                quality = CAMERA_APP_QUALITY_LOW;
                break;

            default:
                return FALSE;
        }
        
        (void)ICONFIG_SetItem(pMe->m_pConfig,
                              CFGI_CAMERA_QUALITY,
                              &pMe->m_nCameraQuality,
                              sizeof(pMe->m_nCameraQuality));

        ICAMERA_SetQuality(pMe->m_pCamera, quality);

        pMe->m_bRePreview = TRUE;
        ICAMERA_Stop(pMe->m_pCamera);
        pMe->m_nCameraState = CAM_STOP;
    }
    
    CLOSE_DIALOG(DLGRET_CANCELED);
    return TRUE;
}

/*==============================================================================
������
       CameraApp_PopMenu_SizeCommandHandleEvent
˵����
       ���մ�С���ò˵��������
       
������
       pMe [in]��ָ��CameraApp Applet����ṹ��ָ�롣�ýṹ����С������ض���Ϣ��
       wParam���¼�������ݡ�
       
����ֵ��
       TRUE�������¼�������
       FALSE�������¼������ԡ�
       
��ע��
       
==============================================================================*/
static boolean CameraApp_PopMenu_SizeCommandHandleEvent(CCameraApp *pMe, uint16 wParam)
{  
    (void)ICONFIG_GetItem(pMe->m_pConfig,
                          CFGI_CAMERA_SIZE,
                          &pMe->m_nCameraSize,
                          sizeof(pMe->m_nCameraSize));

    if(wParam != pMe->m_nCameraSize)
    { 
        CameraApp_SetCameraCaptureSize(pMe, wParam);
        pMe->m_bRePreview = TRUE;
        ICAMERA_Stop(pMe->m_pCamera);
        pMe->m_nCameraState = CAM_STOP;
    }
#if defined(FEATURE_DISP_128X128)
    if(pMe->m_nCameraSize == OEMNV_CAMERA_SIZE_INDEX_0)
    {
        ICAMERA_SetParm(pMe->m_pCamera, CAM_PARM_PREVIEWWITHFRAME, 1, 0);
    }
    else
#endif
    {
        ICAMERA_SetParm(pMe->m_pCamera, CAM_PARM_PREVIEWWITHFRAME, 0, 0);
    }
  
    CLOSE_DIALOG(DLGRET_CANCELED);
    return TRUE;
}

/*==============================================================================
������
       CameraApp_PopMenu_ShutterToneCommandHandleEvent
˵����
       �����������ò˵��������
       
������
       pMe [in]��ָ��CameraApp Applet����ṹ��ָ�롣�ýṹ����С������ض���Ϣ��
       wParam���¼�������ݡ�      
       
����ֵ��
       TRUE�������¼�������
       FALSE�������¼������ԡ�
       
��ע��
       
==============================================================================*/                                                                                                                    
static boolean CameraApp_PopMenu_ShutterToneCommandHandleEvent(CCameraApp *pMe, uint16 wParam)
{   
    switch(wParam)
    {
#ifdef FEATURE_CARRIER_CHINA_TELCOM
        case IDS_TONE_SHUTTER2:
            pMe->m_nCameraTone = OEMNV_CAMERA_SHUTTER_TONE_SHUTTER2; 
            break;

        case IDS_TONE_SHUTTER1:
            pMe->m_nCameraTone = OEMNV_CAMERA_SHUTTER_TONE_SHUTTER1;
            break;
#else    
        case IDS_TONE_DISABLE:
            pMe->m_nCameraTone = OEMNV_CAMERA_SHUTTER_TONE_DISABLE; 
            break;

        case IDS_TONE_ENABLE:
            pMe->m_nCameraTone = OEMNV_CAMERA_SHUTTER_TONE_ENABLE;
            break;
#endif			

        default:
            return FALSE;
    }
    
    (void)ICONFIG_SetItem(pMe->m_pConfig,
                          CFGI_CAMERA_TONE,
                          &pMe->m_nCameraTone,
                          sizeof(pMe->m_nCameraTone));
        
    CLOSE_DIALOG(DLGRET_CANCELED);
    return TRUE;
}

/*==============================================================================
������
       CameraApp_PopMenu_SelfTimeCommandHandleEvent
˵����
       ����ʱ�����ò˵��������
       
������
       pMe [in]��ָ��CameraApp Applet����ṹ��ָ�롣�ýṹ����С������ض���Ϣ��
       wParam���¼�������ݡ�
       
����ֵ��
       TRUE�������¼�������
       FALSE�������¼������ԡ�
       
��ע��
       
==============================================================================*/                                                                                                                       
static boolean CameraApp_PopMenu_SelfTimeCommandHandleEvent(CCameraApp *pMe, uint16 wParam)
{         
    switch(wParam)
    {
        case IDS_SELFTIME_OFF:
            pMe->m_nSelfTimeItemSel = IDS_SELFTIME_OFF;
            break;

        case IDS_SELFTIME_SEC1:
            pMe->m_nSelfTimeItemSel = IDS_SELFTIME_SEC1;
            break;

        case IDS_SELFTIME_SEC2:
            pMe->m_nSelfTimeItemSel = IDS_SELFTIME_SEC2;
            break;

        case IDS_SELFTIME_SEC3:
            pMe->m_nSelfTimeItemSel = IDS_SELFTIME_SEC3;
            break;

        default:
            return FALSE;
    }
    
    CLOSE_DIALOG(DLGRET_CANCELED); 
    return TRUE;
}

/*==============================================================================
������
       CameraApp_PopMenu_BandingCommandHandleEvent
˵����
       Banding���ò˵��������
       
������
       pMe [in]��ָ��CameraApp Applet����ṹ��ָ�롣�ýṹ����С������ض���Ϣ��
       wParam���¼�������ݡ�
       
����ֵ��
       TRUE�������¼�������
       FALSE�������¼������ԡ�
      
��ע��
       
==============================================================================*/                                                                                                                    
static boolean CameraApp_PopMenu_BandingCommandHandleEvent(CCameraApp *pMe, uint16 wParam)
{  
    switch(wParam)
    {
        case IDS_BANDING_MODE1:
            pMe->m_nCameraBanding = OEMNV_CAMERA_BANDING_50HZ;
            break;

        case IDS_BANDING_MODE2:
            pMe->m_nCameraBanding = OEMNV_CAMERA_BANDING_60HZ;
            break;

        default:
            return FALSE;
    }
    ICAMERA_SetParm(pMe->m_pCamera, CAM_PARM_BANDING, pMe->m_nCameraBanding, 0);
    (void)ICONFIG_SetItem(pMe->m_pConfig,
                          CFGI_CAMERA_BANDING,
                          &pMe->m_nCameraBanding,
                          sizeof(pMe->m_nCameraBanding));
    
    CLOSE_DIALOG(DLGRET_CANCELED);
    return TRUE;
}

/*==============================================================================
������
       CameraApp_PopMenu_ResetCFGCommandHandleEvent
˵����
       �ָ�Ĭ�����ò˵��������
       
������
       pMe [in]��ָ��CameraApp Applet����ṹ��ָ�롣�ýṹ����С������ض���Ϣ��
       wParam���¼�������ݡ�
       
����ֵ��
       TRUE�������¼�������
       FALSE�������¼������ԡ� 
       
��ע��
       
==============================================================================*/
static boolean CameraApp_PopMenu_ResetCFGCommandHandleEvent(CCameraApp *pMe, uint16 wParam)
{    
    if(wParam == IDS_CFG_RESET)
    {
        CameraApp_InitCFGData(pMe);    
    }

    if(SUCCESS == ICAMERA_Stop(pMe->m_pCamera))
    {
        pMe->m_bIsPreview = FALSE;
        pMe->m_nCameraState = CAM_STOP;
        CLOSE_DIALOG(DLGRET_CANCELED);
    }
    
    return TRUE;
}

/*==============================================================================
������
       CameraApp_SetCFGMenuIcon
˵����
       ���ò˵���ѡ��ͼ��
       
������      
       pm [in]��Imenuctl�˵��ؼ�
       listId:  ѡ�еĲ˵���
       bSel:    ѡ�������޵��ж�
             
����ֵ�� 
       
��ע��
       
==============================================================================*/
static void CameraApp_SetCFGMenuIcon(IMenuCtl *pm, uint16 listId, boolean bSel)
{
    CtlAddItem cai;
    MEMSET(&cai, 0, sizeof(cai));
    cai.pszResImage = AEE_APPSCOMMONRES_IMAGESFILE;
    cai.wImage = bSel ? IDB_RADIO_FILLED : IDB_RADIO_UNFILLED;
    (void)IMENUCTL_SetItem(pm, listId, MSIF_IMAGE, &cai);
    if (bSel)
    {
        IMENUCTL_SetSel (pm, listId);
    }
    IMENUCTL_SetStyle(pm, NULL, NULL);
}

/*==============================================================================
������
       CameraApp_InitpopMenu
˵����
       ��ʼ�����ò˵�
       
������      
       pMe [in]��ָ��CameraApp Applet����ṹ��ָ�롣�ýṹ����С������ض���Ϣ��
       popMenu [in]: ���õ����˵�
             
����ֵ�� 
       TRUE: ��ʼ�������˵����
       
��ע��
       
==============================================================================*/
static boolean CameraApp_InitpopMenu(CCameraApp *pMe, IMenuCtl *popMenu)
{  
    if(pMe->m_pActiveDlgID == IDD_CCAMERACFG)
    {
        switch(pMe->m_nCameraCFG)
        {
            case CAMERACFGENVIRMENT:
                MSG_FATAL("CAMERACFGQUALITY",0,0,0);
                CameraApp_PopMenu_EnvironmentInit(pMe, popMenu);
                break;

            case CAMERACFGQUALITY:
            	MSG_FATAL("CAMERACFGQUALITY",0,0,0);
                CameraApp_PopMenu_QualityInit(pMe, popMenu);
                break;
   
            case CAMERACFGSIZE:
                MSG_FATAL("CAMERACFGSIZE",0,0,0);
                CameraApp_PopMenu_SizeInit(pMe, popMenu);
                break;
   
            case CAMERACFGTONE:
            	MSG_FATAL("CAMERACFGQUALITY",0,0,0);
                CameraApp_PopMenu_ShutterToneInit(pMe, popMenu);
                break;
   
            case CAMERACFGSELFTIME:
            	MSG_FATAL("CAMERACFGSELFTIME",0,0,0);
                CameraApp_PopMenu_SelfTimeInit(pMe, popMenu);
                break;
   #if 0
            case CAMERACFGBANDING:
            	MSG_FATAL("CAMERACFGBANDING",0,0,0);
                CameraApp_PopMenu_BandingInit(pMe, popMenu);
                break;
#endif
            case CAMERACFGRESET:
            	MSG_FATAL("CAMERACFGRESET",0,0,0);
                CameraApp_PopMenu_ResetCFGInit(pMe, popMenu);
                break;
                
            default:
                break; 
        }
    }
    return TRUE;
}

/*==============================================================================
������
       CameraApp_InitCFGData
˵����
       ����Camera cfg��ΪĬ��ֵ
       
������      
       pMe [in]��ָ��CameraApp Applet����ṹ��ָ�롣�ýṹ����С������ض���Ϣ��
             
����ֵ�� 
       
��ע��
       
==============================================================================*/
static void CameraApp_InitCFGData(CCameraApp * pMe)
{
    if(pMe->m_pActiveDlgID == IDD_CCAMERACFG)
    {
        pMe->m_nCameraEnviroment = OEMNV_CAMERA_ENVIR_AUTO;
        pMe->m_nCameraQuality = OEMNV_CAMERA_QUALITY_HIGH;       
#ifdef FEATURE_CARRIER_CHINA_TELCOM
        pMe->m_nCameraTone = OEMNV_CAMERA_SHUTTER_TONE_SHUTTER1;
#else			
        pMe->m_nCameraTone = OEMNV_CAMERA_SHUTTER_TONE_ENABLE;
#endif
        pMe->m_nCameraBanding = OEMNV_CAMERA_BANDING_50HZ;        

#ifdef FEATURE_VERSION_FLEXI203P

        if(CameraApp_FindMemoryCardExist(pMe))
        {
            pMe->m_nCameraStorage = OEMNV_CAMERA_STORAGE_MEMORY_CARD;
	        pMe->m_nCameraSize = OEMNV_CAMERA_SIZE_DEFAULT;
        }

		
        else
        {
            pMe->m_nCameraStorage = OEMNV_CAMERA_STORAGE_PHONE;
            pMe->m_nCameraSize = OEMNV_CAMERA_SIZE_DEFAULT;
        }
#else

		//�ֻ��ڴ�С�����Թ̶��洢λ����T��
		//Add By zzg 2010_07_25
		pMe->m_nCameraStorage = OEMNV_CAMERA_STORAGE_MEMORY_CARD;
	    pMe->m_nCameraSize = OEMNV_CAMERA_SIZE_DEFAULT;
		//Add End
#endif        
        pMe->m_nSelfTimeItemSel = IDS_SELFTIME_OFF;
 
        (void)ICONFIG_SetItem(pMe->m_pConfig,
                              CFGI_CAMERA_ENVIROMENT,
                              &pMe->m_nCameraEnviroment,
                              sizeof(pMe->m_nCameraEnviroment));

        (void)ICONFIG_SetItem(pMe->m_pConfig,
                              CFGI_CAMERA_QUALITY,
                              &pMe->m_nCameraQuality,
                              sizeof(pMe->m_nCameraQuality));

        (void)ICONFIG_SetItem(pMe->m_pConfig,
                              CFGI_CAMERA_SIZE,
                              &pMe->m_nCameraSize,
                              sizeof(pMe->m_nCameraSize));

        (void)ICONFIG_SetItem(pMe->m_pConfig,
                              CFGI_CAMERA_TONE,
                              &pMe->m_nCameraTone,
                              sizeof(pMe->m_nCameraTone));

        (void)ICONFIG_SetItem(pMe->m_pConfig,
                              CFGI_CAMERA_BANDING,
                              &pMe->m_nCameraBanding,
                              sizeof(pMe->m_nCameraBanding));

        (void)ICONFIG_SetItem(pMe->m_pConfig,
                              CFGI_CAMERA_STORAGE,
                              &pMe->m_nCameraStorage,
                              sizeof(pMe->m_nCameraStorage)); 
    }
}

static void CameraApp_PopMenu_EnvironmentInit(CCameraApp *pMe, IMenuCtl *popMenu)
{
    IMENUCTL_DeleteAll(popMenu);
    CameraApp_SetPopMenuRect(pMe, popMenu, 4);
    IMENUCTL_AddItem(popMenu, 
                     AEE_APPSCAMERAAPP_RES_FILE, 
                     IDS_ENVIR_MODE_AUTO, 
                     IDS_ENVIR_MODE_AUTO, 
                     NULL, 
                     0);
    IMENUCTL_AddItem(popMenu, 
                     AEE_APPSCAMERAAPP_RES_FILE,
                     IDS_ENVIR_MODE_NIGHT, 
                     IDS_ENVIR_MODE_NIGHT, 
                     NULL,
                     0);
    IMENUCTL_AddItem(popMenu, 
                     AEE_APPSCAMERAAPP_RES_FILE, 
                     IDS_ENVIR_MODE_SUNNY, 
                     IDS_ENVIR_MODE_SUNNY, 
                     NULL,
                     0);
    IMENUCTL_AddItem(popMenu, 
                     AEE_APPSCAMERAAPP_RES_FILE, 
                     IDS_ENVIR_MODE_CLOUDY, 
                     IDS_ENVIR_MODE_CLOUDY, 
                     NULL, 
                     0);
    
    InitMenuIcons(popMenu);
    
    switch(pMe->m_pActiveDlgID)
    {
        case IDD_CCAMERACFG:
            (void)ICONFIG_GetItem(pMe->m_pConfig,
                                  CFGI_CAMERA_ENVIROMENT,
                                  &pMe->m_nCameraEnviroment,
                                  sizeof(pMe->m_nCameraEnviroment));
    
            switch(pMe->m_nCameraEnviroment)
            {
                case OEMNV_CAMERA_ENVIR_AUTO:
                    CameraApp_SetCFGMenuIcon(popMenu, IDS_ENVIR_MODE_AUTO, TRUE);
                    break;
    
                case OEMNV_CAMERA_ENVIR_NIGHT:
                    CameraApp_SetCFGMenuIcon(popMenu, IDS_ENVIR_MODE_NIGHT, TRUE);
                    break;
    
                case OEMNV_CAMERA_ENVIR_SUNNY:
                    CameraApp_SetCFGMenuIcon(popMenu, IDS_ENVIR_MODE_SUNNY, TRUE);
                    break;
    
                case OEMNV_CAMERA_ENVIR_CLOUDY:
                    CameraApp_SetCFGMenuIcon(popMenu, IDS_ENVIR_MODE_CLOUDY, TRUE);
                    break;

                default:
                    break;
            }
    
            break;
    
        case IDD_CVIDEOCFG:
            (void)ICONFIG_GetItem(pMe->m_pConfig,
                                  CFGI_VIDEO_ENVIROMENT,
                                  &pMe->m_nCameraEnviroment,
                                  sizeof(pMe->m_nCameraEnviroment));
    
            switch(pMe->m_nCameraEnviroment)
            {
                case OEMNV_CAMERA_ENVIR_AUTO:
                    CameraApp_SetCFGMenuIcon(popMenu, IDS_ENVIR_MODE_AUTO, TRUE);
                    break;
    
                case OEMNV_CAMERA_ENVIR_NIGHT:
                    CameraApp_SetCFGMenuIcon(popMenu, IDS_ENVIR_MODE_NIGHT, TRUE);
                    break;
    
                case OEMNV_CAMERA_ENVIR_SUNNY:
                    CameraApp_SetCFGMenuIcon(popMenu, IDS_ENVIR_MODE_SUNNY, TRUE);
                    break;
    
                case OEMNV_CAMERA_ENVIR_CLOUDY:
                    CameraApp_SetCFGMenuIcon(popMenu, IDS_ENVIR_MODE_CLOUDY, TRUE);
                    break;
    
                default:
                    break;
            }
    
            break;
                        
        default:
            break;
    }

}

static void CameraApp_PopMenu_QualityInit(CCameraApp *pMe, IMenuCtl *popMenu)
{
    IMENUCTL_DeleteAll(popMenu);
    CameraApp_SetPopMenuRect(pMe, popMenu, 3);
    IMENUCTL_AddItem(popMenu, 
                     AEE_APPSCAMERAAPP_RES_FILE, 
                     IDS_QUALITY_HIGH, 
                     IDS_QUALITY_HIGH, 
                     NULL,
                     0);
    IMENUCTL_AddItem(popMenu,
                     AEE_APPSCAMERAAPP_RES_FILE, 
                     IDS_QUALITY_MED,
                     IDS_QUALITY_MED, 
                     NULL, 
                     0);
    IMENUCTL_AddItem(popMenu, 
                     AEE_APPSCAMERAAPP_RES_FILE, 
                     IDS_QUALITY_LOW,
                     IDS_QUALITY_LOW, 
                     NULL, 
                     0);            
    InitMenuIcons(popMenu);
    
                
    (void)ICONFIG_GetItem(pMe->m_pConfig,
                          CFGI_CAMERA_QUALITY,
                          &pMe->m_nCameraQuality,
                          sizeof(pMe->m_nCameraQuality));
                                           
    switch(pMe->m_nCameraQuality)
    {
        case OEMNV_CAMERA_QUALITY_HIGH:
            CameraApp_SetCFGMenuIcon(popMenu, IDS_QUALITY_HIGH, TRUE);
            break;
                
        case OEMNV_CAMERA_QUALITY_MED:
            CameraApp_SetCFGMenuIcon(popMenu, IDS_QUALITY_MED, TRUE);
            break;
                
        case OEMNV_CAMERA_QUALITY_LOW:
            CameraApp_SetCFGMenuIcon(popMenu, IDS_QUALITY_LOW, TRUE);
            break;
                
        default:
            break;  
    }
}
           
static void CameraApp_PopMenu_SizeInit(CCameraApp *pMe, IMenuCtl *popMenu)
{
    IMENUCTL_DeleteAll(popMenu);
    if(pMe->m_nCameraStorage == OEMNV_CAMERA_STORAGE_MEMORY_CARD)
    {
        int i=0;

        while(1)
        {
        	MSG_FATAL("CameraApp_PopMenu_SizeInit...........",0,0,0);
            if(pMe->m_sensor_model == 30)
            {
                if(g_CameraSizeCFG[i].dx == 0)
                {
                    break;
                }
                IMENUCTL_AddItem(popMenu, 
                                 NULL, 
                                 0,
                                 i, 
                                 g_CameraSizeCFG[i].pStr, 
                                 NULL);
                i++;
            }
            else if(pMe->m_sensor_model == 10)
            {
                if(g_CameraSizeCFG_10[i].dx == 0)
                {
                    break;
                }
                IMENUCTL_AddItem(popMenu, 
                                 NULL, 
                                 0,
                                 i, 
                                 g_CameraSizeCFG_10[i].pStr, 
                                 NULL);
                i++;            
            }
            else
            {
            	if(g_CameraSizeCFG_10[i].dx == 0)
                {
                    break;
                }
                IMENUCTL_AddItem(popMenu, 
                                 NULL, 
                                 0,
                                 i, 
                                 g_CameraSizeCFG_10[i].pStr, 
                                 NULL);
                i++; 
            }
        }
        
        CameraApp_SetPopMenuRect(pMe, popMenu, i);
    }
    else
    {
        if(pMe->m_sensor_model == 30)
        {
            IMENUCTL_AddItem(popMenu, 
                             NULL, 
                             0,
                             0, 
                             g_CameraSizeCFG[OEMNV_CAMERA_SIZE_DEFAULT].pStr, 
                             NULL);
        }
        else if(pMe->m_sensor_model == 10)
        {
            IMENUCTL_AddItem(popMenu, 
                             NULL, 
                             0,
                             0, 
                             g_CameraSizeCFG_10[OEMNV_CAMERA_SIZE_DEFAULT].pStr, 
                             NULL);        
        }
        else
        {
        	IMENUCTL_AddItem(popMenu, 
                             NULL, 
                             0,
                             0, 
                             g_CameraSizeCFG_10[OEMNV_CAMERA_SIZE_DEFAULT].pStr, 
                             NULL);
        }
        CameraApp_SetPopMenuRect(pMe, popMenu, 1);
    }
    
    InitMenuIcons(popMenu);

    (void)ICONFIG_GetItem(pMe->m_pConfig,
                          CFGI_CAMERA_SIZE,
                          &pMe->m_nCameraSize,
                          sizeof(pMe->m_nCameraSize));

    CameraApp_SetCFGMenuIcon(popMenu, pMe->m_nCameraSize, TRUE);
}
         
static void CameraApp_PopMenu_ShutterToneInit(CCameraApp *pMe, IMenuCtl *popMenu)  
{
    IMENUCTL_DeleteAll(popMenu);   
    CameraApp_SetPopMenuRect(pMe, popMenu, 2);
#ifdef FEATURE_CARRIER_CHINA_TELCOM
    IMENUCTL_AddItem(popMenu,
                     AEE_APPSCAMERAAPP_RES_FILE, 
                     IDS_TONE_SHUTTER1, 
                     IDS_TONE_SHUTTER1,
                     NULL,
                     0);
    IMENUCTL_AddItem(popMenu, 
                     AEE_APPSCAMERAAPP_RES_FILE,
                     IDS_TONE_SHUTTER2, 
                     IDS_TONE_SHUTTER2, 
                     NULL, 
                     0);
#else		
    IMENUCTL_AddItem(popMenu,
                     AEE_APPSCAMERAAPP_RES_FILE, 
                     IDS_TONE_DISABLE, 
                     IDS_TONE_DISABLE,
                     NULL,
                     0);
    IMENUCTL_AddItem(popMenu, 
                     AEE_APPSCAMERAAPP_RES_FILE,
                     IDS_TONE_ENABLE, 
                     IDS_TONE_ENABLE, 
                     NULL, 
                     0);            
#endif	
    InitMenuIcons(popMenu);
    
    (void)ICONFIG_GetItem(pMe->m_pConfig,
                          CFGI_CAMERA_TONE,
                          &pMe->m_nCameraTone,
                          sizeof(pMe->m_nCameraTone));
    
    switch(pMe->m_nCameraTone)
    {
#ifdef FEATURE_CARRIER_CHINA_TELCOM
        case OEMNV_CAMERA_SHUTTER_TONE_SHUTTER2:
            CameraApp_SetCFGMenuIcon(popMenu, IDS_TONE_SHUTTER2, TRUE);
            break;
    
        case OEMNV_CAMERA_SHUTTER_TONE_SHUTTER1:
            CameraApp_SetCFGMenuIcon(popMenu, IDS_TONE_SHUTTER1, TRUE);
            break;
#else        
        case OEMNV_CAMERA_SHUTTER_TONE_DISABLE:
            CameraApp_SetCFGMenuIcon(popMenu, IDS_TONE_DISABLE, TRUE);
            break;
    
        case OEMNV_CAMERA_SHUTTER_TONE_ENABLE:
            CameraApp_SetCFGMenuIcon(popMenu, IDS_TONE_ENABLE, TRUE);
            break;
#endif			
    
        default:
            break;  
    }
}

static void CameraApp_PopMenu_SelfTimeInit(CCameraApp *pMe, IMenuCtl *popMenu)
{
    IMENUCTL_DeleteAll(popMenu);   
    CameraApp_SetPopMenuRect(pMe, popMenu, 4);
    IMENUCTL_AddItem(popMenu, 
                     AEE_APPSCAMERAAPP_RES_FILE, 
                     IDS_SELFTIME_OFF, 
                     IDS_SELFTIME_OFF, 
                     NULL,
                     0);
    IMENUCTL_AddItem(popMenu, 
                     AEE_APPSCAMERAAPP_RES_FILE, 
                     IDS_SELFTIME_SEC1, 
                     IDS_SELFTIME_SEC1, 
                     NULL, 
                     0);
    IMENUCTL_AddItem(popMenu, 
                     AEE_APPSCAMERAAPP_RES_FILE, 
                     IDS_SELFTIME_SEC2, 
                     IDS_SELFTIME_SEC2, 
                     NULL, 
                     0);
    IMENUCTL_AddItem(popMenu, 
                     AEE_APPSCAMERAAPP_RES_FILE,
                     IDS_SELFTIME_SEC3,
                     IDS_SELFTIME_SEC3,
                     NULL, 
                     0);         
    InitMenuIcons(popMenu);
    CameraApp_SetCFGMenuIcon(popMenu, pMe->m_nSelfTimeItemSel, TRUE);
}
            
static void CameraApp_PopMenu_BandingInit(CCameraApp *pMe, IMenuCtl *popMenu)
{
    IMENUCTL_DeleteAll(popMenu);  
    CameraApp_SetPopMenuRect(pMe, popMenu, 2);
    IMENUCTL_AddItem(popMenu, 
                     AEE_APPSCAMERAAPP_RES_FILE, 
                     IDS_BANDING_MODE1, 
                     IDS_BANDING_MODE1, 
                     NULL, 
                     0);
    IMENUCTL_AddItem(popMenu, 
                     AEE_APPSCAMERAAPP_RES_FILE, 
                     IDS_BANDING_MODE2, 
                     IDS_BANDING_MODE2, 
                     NULL, 
                     0);
               
    InitMenuIcons(popMenu);
    (void)ICONFIG_GetItem(pMe->m_pConfig,
                          CFGI_CAMERA_BANDING,
                          &pMe->m_nCameraBanding,
                          sizeof(pMe->m_nCameraBanding));
    
    switch(pMe->m_nCameraBanding)
    {
        case OEMNV_CAMERA_BANDING_50HZ:
            CameraApp_SetCFGMenuIcon(popMenu, IDS_BANDING_MODE1, TRUE);
            break;
    
        case OEMNV_CAMERA_BANDING_60HZ:
            CameraApp_SetCFGMenuIcon(popMenu, IDS_BANDING_MODE2, TRUE);
            break;
    
        default:
            break;
    }
}

static void CameraApp_PopMenu_ResetCFGInit(CCameraApp *pMe, IMenuCtl *popMenu)
{
    IMENUCTL_DeleteAll(popMenu);   
    CameraApp_SetPopMenuRect(pMe, popMenu, 1);
    IMENUCTL_AddItem(popMenu, 
                     AEE_APPSCAMERAAPP_RES_FILE, 
                     IDS_CFG_RESET, 
                     IDS_CFG_RESET, 
                     NULL, 
                     0);   

}

static void CameraApp_PlayShutterSound(CCameraApp *pMe)
{
    AEEMediaCmdNotify cmd;
    
    // ���pMe->m_pMedia�ӿ�Ϊ�գ������ӿ�
    if(!pMe->m_pMedia)
    {
        AEEMediaData      md;
       
        if(!pMe)
           return;

        md.clsData = MMD_FILE_NAME;  		   
        md.pData = (void *)"fs:/cameraapp/shutter.mid";
        md.dwSize = 0;
       
        (void)AEEMediaUtil_CreateMedia(pMe->m_pShell, &md, &pMe->m_pMedia);
    }
   
    if(pMe->m_pMedia)
    {        
        IMEDIA_SetVolume(pMe->m_pMedia, AEE_MAX_VOLUME); //max volum is 100
     
        if(IMEDIA_RegisterNotify(pMe->m_pMedia, CameraApp_MediaNotify, pMe) != SUCCESS)
        {
            cmd.nCmd    = MM_CMD_PLAY;
            cmd.nStatus = MM_STATUS_DONE;
            CameraApp_MediaNotify((void *)pMe, &cmd);
            return;
        }

        if(IMEDIA_Play(pMe->m_pMedia) != SUCCESS)
        {
            cmd.nCmd    = MM_CMD_PLAY;
            cmd.nStatus = MM_STATUS_DONE;
            CameraApp_MediaNotify((void *)pMe, &cmd);
            return;
        }
    }
    else
    {
        cmd.nCmd    = MM_CMD_PLAY;
        cmd.nStatus = MM_STATUS_DONE;
        CameraApp_MediaNotify((void *)pMe, &cmd);
    }
}

static void CameraApp_MediaNotify(void *pUser, AEEMediaCmdNotify *pCmdNotify)
{
    CCameraApp *pMe = (CCameraApp *)pUser;

    if(!pMe || !pCmdNotify)
        return;

    if(pCmdNotify->nCmd == MM_CMD_PLAY)  // IMEDIA_Play events
    {
        switch (pCmdNotify->nStatus)
        {
            case MM_STATUS_ABORT:            
                break;

            case MM_STATUS_DONE:    // playback done
                    //IMEDIA_Play(pMe->m_pMedia);              
#ifdef FEATURE_CARRIER_CHINA_TELCOM                    
                    pMe->m_pMedia = NULL;
#endif
                break;
            default:
                break;
        }
    }
}

/*void CameraApp_AlphaDisplay(CCameraApp *pMe, IMenuCtl *popMenu)
{
    IDisplay *pdis = pMe->m_pDisplay;
    IBitmap  *pBgBitmap=NULL;
   // IBitmap  *popMenuBg = NULL;
    IDIB     *pBgDIB=NULL;
    int      count;
    AEERect  prc;
    uint16   wClr;
    uint8 red, green, blue;
 
    // ��ȡ��ʾ��λͼ
    IDISPLAY_GetDeviceBitmap(pdis, &pBgBitmap);
    if(pBgBitmap == NULL)
        return;

    //popMenuBg = IDISPLAY_GetDestination(pdis);
    
    IMENUCTL_GetRect(popMenu, &prc);

    ERR("x=%d,y=%d,dx=%d",prc.x, prc.y, prc.dx);

    //IBITMAP_BltIn(popMenuBg, prc->x, prc->y, prc->dx, prc->dy, pBgBitmap, prc->x, prc->y, AEE_RO_COPY);
    
    // ��ȡ��ʾ������
    IBITMAP_QueryInterface(pBgBitmap, AEECLSID_DIB, (void**)&pBgDIB);
    if(pBgDIB == NULL)
        return;
 
    
    count = pBgDIB->cx * pBgDIB->cy;
    
    if(pBgDIB->nDepth == 16) 
    { 
        // Ĭ�ϵ�16λ����
        int16* bgpixel = (int16*)pBgDIB->pBmp;
     
        // ��alpha=0.5����
        while(count--) 
        {
            wClr = *bgpixel;
            // 565��ʽ, IDIB_COLORSCHEME_565
            red = ((wClr & 0x1f) >> 1);
            wClr >>= 5;
            green = ((wClr & 0x3f) >> 1);
            wClr >>= 6;
            blue = (wClr >> 1);
 #define _RGB16BIT(r, g, b) ( (b % 32) + ( (g % 64) << 5)+( (r % 32) << 11))
           *bgpixel = _RGB16BIT(red, green, blue);
           ++bgpixel;
       }
    }
   
    IDISPLAY_BitBlt(pdis, prc.x, prc.y, prc.dx, prc.dy, pBgBitmap, prc.x, prc.y, AEE_RO_COPY);
     //IDISPLAY_BitBlt(pdis, 0, 0, po->cxScreen, po->ButtonLine, pBgBitmap, 0, 0, AEE_RO_COPY);
 
     // �ͷ���Դ
    IDIB_Release(pBgDIB);
    IBITMAP_Release(pBgBitmap);
    //IBITMAP_Release(popMenuBg);   
}*/

static void CameraApp_SetPopMenuRect(CCameraApp *pMe, IMenuCtl* popMenu, int menuItemSum)
{
    AEERect mRec;
    int16  x,y,dx,dy;

    if(!pMe || !popMenu)
    {
        return;
    }
    
    dx = POPMENU_WIDTH;
    
    // the number of popMenu Item is no more than 4
    if(menuItemSum < 4)
    {
        //dy = menuItemSum*(pMe->m_nItemH+7);
        dy = menuItemSum*(pMe->m_nItemH+2);
    }
    else
    {
        //dy = 4*(pMe->m_nItemH+7);
        dy = 4*(pMe->m_nItemH+2);
    }

    x = CFGBAR_TEXT_X;
       
    //y = pMe->m_cyHeight - dy - POPMENU_BOTTOM_Y;
    y = 2*CFGBAR_TEXT_Y;
 
    SETAEERECT(&mRec, x, y, dx, dy);

    IMENUCTL_SetRect(popMenu, &mRec);
}

static void CameraApp_DrawTopBar(CCameraApp *pMe)
{
    int i;
    int16  nResID[CAMERACFGMAX];
    IImage *pTopBarImage = NULL; 
          
    // enviroment cfgID
    (void)ICONFIG_GetItem(pMe->m_pConfig,
                          CFGI_CAMERA_ENVIROMENT,
                          &pMe->m_nCameraEnviroment,
                          sizeof(pMe->m_nCameraEnviroment));
    
    switch(pMe->m_nCameraEnviroment)
    {
	    case OEMNV_CAMERA_ENVIR_AUTO:
	        nResID[CAMERACFGENVIRMENT] = IDI_ENV_AUTO;
	        break;
	        
	    case OEMNV_CAMERA_ENVIR_SUNNY:
	        nResID[CAMERACFGENVIRMENT] = IDI_ENV_SUNNY;
	        break;

	    case OEMNV_CAMERA_ENVIR_CLOUDY:
	        nResID[CAMERACFGENVIRMENT] = IDI_ENV_CLOUDY;
	        break;

	    case OEMNV_CAMERA_ENVIR_NIGHT:
	        nResID[CAMERACFGENVIRMENT] = IDI_ENV_NIGHT;
	        break;
	        
	    default:
	        nResID[CAMERACFGENVIRMENT] = IDI_ENV_AUTO;
	        break;
    }

    // quality cfgID
    (void)ICONFIG_GetItem(pMe->m_pConfig,
                          CFGI_CAMERA_QUALITY,
                          &pMe->m_nCameraQuality,
                          sizeof(pMe->m_nCameraQuality));
    
    switch(pMe->m_nCameraQuality)
    {
	    case OEMNV_CAMERA_QUALITY_HIGH:
	        nResID[CAMERACFGQUALITY] = IDI_QUALITY_HIGH;
	        break;

	    case OEMNV_CAMERA_QUALITY_MED:
	        nResID[CAMERACFGQUALITY] = IDI_QUALITY_MED;
	        break;

	    case OEMNV_CAMERA_QUALITY_LOW:
	        nResID[CAMERACFGQUALITY] = IDI_QUALITY_LOW;
	        break;
	        
		default:
	        nResID[CAMERACFGQUALITY] = IDI_QUALITY_HIGH;
	        break;
	}
    
    // size cfgID
    (void)ICONFIG_GetItem(pMe->m_pConfig,
                          CFGI_CAMERA_SIZE,
                         &pMe->m_nCameraSize,
                          sizeof(pMe->m_nCameraSize));

    switch(pMe->m_nCameraSize)
    {
	    case OEMNV_CAMERA_SIZE_INDEX_0:
	        nResID[CAMERACFGSIZE] = IDI_SIZE_160_128;
	        break;

	    case OEMNV_CAMERA_SIZE_INDEX_1:
	        nResID[CAMERACFGSIZE] = IDI_SIZE_220_176;
	        break;

	    case OEMNV_CAMERA_SIZE_INDEX_2:
	        nResID[CAMERACFGSIZE] = IDI_SIZE_320_240;
	        break;

	    case OEMNV_CAMERA_SIZE_INDEX_3:
	        nResID[CAMERACFGSIZE] = IDI_SIZE_640_480;
	        break;
	        
	    default:
	        nResID[CAMERACFGSIZE] = IDI_SIZE_160_128;
	        break;
    }

    // sound cfgID
    (void)ICONFIG_GetItem(pMe->m_pConfig,
                          CFGI_CAMERA_TONE,
                          &pMe->m_nCameraTone,
                          sizeof(pMe->m_nCameraTone));
    
    switch(pMe->m_nCameraTone)
    {       
	    case OEMNV_CAMERA_SHUTTER_TONE_ENABLE:
	        nResID[CAMERACFGTONE] = IDI_TONE_ENABLE;
	        break;

	    case OEMNV_CAMERA_SHUTTER_TONE_DISABLE:
	        nResID[CAMERACFGTONE] = IDI_TONE_DISABLE;
	        break;		
	        
	    default:
	        nResID[CAMERACFGTONE] = IDI_TONE_ENABLE;
	        break;
    }
    
    // draw selftime cfg    
    switch(pMe->m_nSelfTimeItemSel)
    {
	    case IDS_SELFTIME_OFF:
	        nResID[CAMERACFGSELFTIME] = IDI_SELFTIME_OFF;
	        break;

	    case IDS_SELFTIME_SEC1:
	        nResID[CAMERACFGSELFTIME] = IDI_SELFTIME_5;
	        break;

	    case IDS_SELFTIME_SEC2:
	        nResID[CAMERACFGSELFTIME] = IDI_SELFTIME_10;
	        break;

	    case IDS_SELFTIME_SEC3:
	        nResID[CAMERACFGSELFTIME] = IDI_SELFTIME_15;
	        break;
	        
		default:
	        nResID[CAMERACFGSELFTIME] = IDI_SELFTIME_OFF;
	        break;
    }
    #if 0
    // banding cfgID
    (void)ICONFIG_GetItem(pMe->m_pConfig,
                          CFGI_CAMERA_BANDING,
                          &pMe->m_nCameraBanding,
                          sizeof(pMe->m_nCameraBanding));
    
    switch(pMe->m_nCameraBanding){
    case OEMNV_CAMERA_BANDING_50HZ:
        nResID[CAMERACFGBANDING] = IDI_BANDING_50;
        break;

    case OEMNV_CAMERA_BANDING_60HZ:
        nResID[CAMERACFGBANDING] = IDI_BANDING_60;
        break;
        
    default:
        nResID[CAMERACFGBANDING] = IDI_BANDING_50;
        break;
    }
    #endif
    nResID[CAMERACFGRESET] = IDI_RESET;
    
    for(i = 0; i < CAMERACFGMAX; i++)
    {
        pTopBarImage = ISHELL_LoadResImage(pMe->m_pShell, 
                                           CAMERAAPP_IMAGE_RES_FILE, 
                                           nResID[i]);
        if(pTopBarImage)
        {
            IIMAGE_Draw(pTopBarImage, i*(TOPBAR_ICON_WIDTH+TOPBAR_ICON_SPACE), TOPBAR_ICON_Y);	// + 5
            IIMAGE_Release(pTopBarImage);
            pTopBarImage = NULL;
        }
    }

	//Add By zzg 2010_07_25
    {
	    IImage *pCameraCFGChooseIcon = ISHELL_LoadResImage(pMe->m_pShell, CAMERAAPP_IMAGE_RES_FILE, IDI_CHOOSE_REC);
	    
	    if (pCameraCFGChooseIcon)
	    {  
			IIMAGE_SetDrawSize(pCameraCFGChooseIcon, TOPBAR_ICON_WIDTH, CFGBAR_TEXT_HEIGHT);
			IIMAGE_Draw(pCameraCFGChooseIcon, (TOPBAR_ICON_SPACE+TOPBAR_ICON_WIDTH)*(pMe->m_nCameraCFG), TOPBAR_ICON_Y);	//Add By zzg 2010_07_25
			
	        IIMAGE_Release(pCameraCFGChooseIcon);
	        pCameraCFGChooseIcon = NULL;
	    }
	}
	//Add End
}

static void CameraApp_DrawMidPic(CCameraApp *pMe)
{
    IImage *pImage = NULL;
    AEEImageInfo myInfo;
    
    switch(pMe->m_pActiveDlgID)
    {
        case IDD_CPREVIEW:
            pImage = ISHELL_LoadResImage(pMe->m_pShell,
                                         CAMERAAPP_IMAGE_RES_FILE,
                                         IDI_MID_CAMERA);
            if(pImage != NULL)
            {
                IIMAGE_GetInfo(pImage, &myInfo);
                IIMAGE_Draw(pImage, (pMe->m_cxWidth - myInfo.cx)/2, pMe->m_cyHeight - myInfo.cy -1 );
                IIMAGE_Release(pImage);
                pImage = NULL;
            }
            break;
            
        default:
            break;
    }
}

static void CameraApp_DrawCFGPromptText(CCameraApp *pMe)
{  
    AEERect prc;
    int16   nResID = 0;       
    AECHAR  pcText[20]; 
    
    if(pMe->m_pActiveDlgID == IDD_CCAMERACFG)
    {
        switch(pMe->m_nCameraCFG)
        {
            case CAMERACFGENVIRMENT:
                nResID = IDS_CFG_ENVIR;
                break;
                
            case CAMERACFGQUALITY:
                nResID = IDS_CFG_QUALITY;
                break;
                
            case CAMERACFGSIZE:
                nResID = IDS_CFG_SIZE;
                break;
                
            case CAMERACFGTONE:
                nResID = IDS_CFG_TONE;
                break;
                
            case CAMERACFGSELFTIME:
                nResID = IDS_CFG_SELFTIMER;
                break;
           #if 0     
            case CAMERACFGBANDING:
                nResID = IDS_CFG_BANDING;
                break;
#endif
            case CAMERACFGRESET:
                nResID = IDS_CFG_RESET;
                break;
                
            default:
                break; 
        }
    }
    
	SETAEERECT(&prc, CFGBAR_TEXT_X, CFGBAR_TEXT_Y, CFGBAR_TEXT_WIDTH, CFGBAR_TEXT_HEIGHT);	//Add By zzg 2010_07_24

    MEMSET(pcText, 0, sizeof(pcText)); 
    (void)ISHELL_LoadResString(pMe->m_pShell, 
                               AEE_APPSCAMERAAPP_RES_FILE,       
                               nResID, 
                               pcText, 
                               sizeof(pcText));     

    if(pMe->m_nCameraSize != OEMNV_CAMERA_SIZE_INDEX_0|| (pMe->m_pActiveDlgID == IDD_CVIDEOCFG))
    {
        (void)IDISPLAY_SetColor(pMe->m_pDisplay, CLR_USER_TEXT, RGB_BLACK);
        
        DrawTextWithProfile(pMe->m_pShell, 
                            pMe->m_pDisplay, 
                            WHITE_TEXT, 
                            AEE_FONT_NORMAL, 
                            pcText, 
                            -1, 
                            0, 
                            0, 
                            &prc, 
                            IDF_ALIGN_LEFT|IDF_ALIGN_MIDDLE|IDF_TEXT_TRANSPARENT);	//IDF_ALIGN_CENTER

        (void)IDISPLAY_SetColor(pMe->m_pDisplay, CLR_USER_TEXT, RGB_WHITE);
    }
    else
    {   
        (void)IDISPLAY_SetColor(pMe->m_pDisplay, CLR_USER_TEXT, WHITE_TEXT);
        
        DrawTextWithProfile(pMe->m_pShell, 
                            pMe->m_pDisplay, 
                            RGB_BLACK, 
                            AEE_FONT_NORMAL, 
                            pcText, 
                            -1, 
                            0, 
                            0, 
                            &prc, 
                            IDF_ALIGN_LEFT|IDF_ALIGN_MIDDLE|IDF_TEXT_TRANSPARENT);	//IDF_ALIGN_CENTER

        (void)IDISPLAY_SetColor(pMe->m_pDisplay, CLR_USER_TEXT, RGB_WHITE);
    }
}

static void CameraApp_CPreviewStart(CCameraApp *pMe)
{
    AEESize displaySize, captureSize; 
    int16 quality = 0;
    if(pMe->m_sensor_model == 30)
    {
        captureSize.cx = g_CameraSizeCFG[pMe->m_nCameraSize].dx;
        captureSize.cy = g_CameraSizeCFG[pMe->m_nCameraSize].dy;
#ifndef FEATURE_CAMERA_NOFULLSCREEN
        displaySize.cx = g_CameraSizeCFG[0].dx;
        displaySize.cy = g_CameraSizeCFG[0].dy;
#endif
    }
    else if(pMe->m_sensor_model == 10)
    {
        captureSize.cx = g_CameraSizeCFG_10[pMe->m_nCameraSize].dx;
        captureSize.cy = g_CameraSizeCFG_10[pMe->m_nCameraSize].dy;
#ifndef FEATURE_CAMERA_NOFULLSCREEN
#ifdef T_QSC1110
        displaySize.cx = g_CameraSizeCFG_10[1].dx;
        displaySize.cy = g_CameraSizeCFG_10[1].dy;
#else
		displaySize.cx = g_CameraSizeCFG_10[0].dx;
        displaySize.cy = g_CameraSizeCFG_10[0].dy;
#endif
#endif
    }
#ifdef FEATURE_CAMERA_NOFULLSCREEN
#if defined(FEATURE_DISP_160X128)
    displaySize.cx = 96;
    displaySize.cy = 96;
#elif defined(FEATURE_DISP_220X176)
    displaySize.cx = 176;
    displaySize.cy = 128;
#elif defined(FEATURE_DISP_128X160)
    displaySize.cx = 96;
    displaySize.cy = 96; 
#elif defined(FEATURE_DISP_176X220)
    displaySize.cx = 176;
    displaySize.cy = 128;
#elif defined(FEATURE_DISP_128X160)
	displaySize.cx = 128;
    displaySize.cy = 96;
#else
    displaySize.cx = 96;
    displaySize.cy = 96;
#endif
    pMe->m_rcPreview.x  = pMe->m_rc.x+(pMe->m_rc.dx-displaySize.cx)/2;
    pMe->m_rcPreview.y  = pMe->m_rc.y+(pMe->m_rc.dy-displaySize.cy)/2;
    pMe->m_rcPreview.dx = displaySize.cx;
    pMe->m_rcPreview.dy = displaySize.cy;
    ICAMERA_SetParm(pMe->m_pCamera, CAM_PARM_LCD_DIRECT_ACCESS, (int32)FALSE, (int32)&(pMe->m_rcPreview));
#endif
    // set camera quality
    switch(pMe->m_nCameraQuality)
    {
        case OEMNV_CAMERA_QUALITY_HIGH:
            quality = CAMERA_APP_QUALITY_HIGH;
            break;

        case OEMNV_CAMERA_QUALITY_MED:
            quality = CAMERA_APP_QUALITY_MED;
            break;

        case OEMNV_CAMERA_QUALITY_LOW:
            quality = CAMERA_APP_QUALITY_LOW;
            break; 

        default:
            break;
    }

    MSG_FATAL("displaySize.cx=%d,displaySize.cy=%d",displaySize.cx,displaySize.cy,0);
#ifndef FEATURE_DSP
    ICAMERA_SetParm(pMe->m_pCamera, CAM_PARM_PREVIEW_TYPE, CAM_PREVIEW_SNAPSHOT, 0);       
    ICAMERA_SetParm(pMe->m_pCamera, CAM_PARM_MULTISHOT, 1, 0);
    ICAMERA_SetQuality(pMe->m_pCamera, quality);    
    ICAMERA_SetSize(pMe->m_pCamera, &captureSize);
    ICAMERA_SetDisplaySize(pMe->m_pCamera, &displaySize);
    
    ICAMERA_Preview(pMe->m_pCamera);
#endif

    pMe->m_nCameraState = CAM_PREVIEW;  

    CameraApp_SetParamAfterPreview(pMe);

    pMe->m_bIsPreview = TRUE;
}

static void CameraApp_RecordSnapShot(CCameraApp *pMe)
{ 
    char sFileName[MIN_PIC_CHAR_NAME_LEN];
    
    MEMSET(pMe->m_sCurrentFileName, '\0', sizeof(pMe->m_sCurrentFileName));
    MEMSET(sFileName, '\0', sizeof(sFileName));
    
    CameraApp_GetDateForRecordFileName(pMe, pMe->m_sCurrentFileName, FILE_TYPE_JPG);
    CameraApp_SetDateForRecordFileName(pMe);
   
    // copy the pic name to sFilename buffer
    if ( pMe->m_isStartFromFacebook == TRUE )
    {
		STRCPY(pMe->m_sCaptureFileName, pMe->m_sCurrentFileName+STRLEN(MG_PHONEPICTURE_PATH));
    }
    else
    {
	    if(pMe->m_nCameraStorage == OEMNV_CAMERA_STORAGE_MEMORY_CARD)
	    {
	        STRCPY(pMe->m_sCaptureFileName, pMe->m_sCurrentFileName+STRLEN(MG_MASSCARDPICTURE_PATH));
	    }
	    else
	    {
	        STRCPY(pMe->m_sCaptureFileName, pMe->m_sCurrentFileName+STRLEN(MG_PHONEPICTURE_PATH));
	    }
    }
    
    pMe->m_nCameraState = CAM_SAVE;
    pMe->m_bCapturePic  = TRUE;
#ifndef FEATURE_DSP
    (void)ICAMERA_DeferEncode(pMe->m_pCamera, TRUE);
#endif
    
    // ����״̬�Ĵ���
    if(SUCCESS != ICAMERA_RecordSnapshot(pMe->m_pCamera))
    {
        // ����ʧ��,Ĭ�ϱ����Ѿ����ճɹ�����Ƭ,�����ص�Ԥ������,����UI�������������
        // Vc848.c�д����,�������ʧ��,ֱ��ɾ��ʧ�ܵ��ļ�.
        pMe->m_wMsgID = IDS_MSG_CAPTURE_FAILED;
        pMe->m_nMsgTimeout = TIMEOUT_MS_MSGBOX;
        ICAMERA_Stop(pMe->m_pCamera);
        pMe->m_nCameraState = CAM_STOP;
        pMe->m_bRePreview = TRUE;
        CLOSE_DIALOG(DLGRET_POPMSG);
        return;
    }

    pMe->m_bCapturePic = FALSE;
}

static void CameraApp_DialogTimeout(void *pme)
{
    CCameraApp *pMe = (CCameraApp *)pme;
        
    if(NULL == pMe)
        return;

    (void)ISHELL_PostEvent(pMe->m_pShell,
                           AEECLSID_APP_CAMERA,
                           EVT_APP_DIALOG_TIMEOUT,
                           0,
                           0);
}

static boolean CameraApp_FindMemoryCardExist(CCameraApp *pMe)
{   
    return (IFILEMGR_Test(pMe->m_pFileMgr, AEEFS_CARD0_DIR)==SUCCESS)?TRUE:FALSE;	
}

// ������գ������Ƿ����㹻ʣ��ռ�����
static boolean CameraApp_IsEnoughfMemorySpace(CCameraApp * pMe)
{
    if ( pMe->m_isStartFromFacebook == TRUE)
	{
		IFILEMGR_GetFreeSpaceEx(pMe->m_pFileMgr, 
	                                MG_PHONE_ROOTDIR, 
	                                &pMe->m_dwMemTotal, 
	                                &pMe->m_dwMemFree);

        if((pMe->m_dwMemFree/(2*BYTE_SIZE) < MIN_FREE_PHONE_SPACE) 
           ||(pMe->m_dwMemFree/(2*BYTE_SIZE) == MIN_FREE_PHONE_SPACE))
        {               
            return FALSE;
        }
        else
        {
            return TRUE;
        }
	}
	else
	{
	    if(pMe->m_nCameraStorage == OEMNV_CAMERA_STORAGE_MEMORY_CARD)
	    {
	        IFILEMGR_GetFreeSpaceEx(pMe->m_pFileMgr, 
	                                MG_MASSCARD_ROOTDIR, 
	                                &pMe->m_dwMemTotal, 
	                                &pMe->m_dwMemFree);

	        if((pMe->m_dwMemFree/(2*BYTE_SIZE) < MIN_FREE_MEMORY_CARD_SPACE) 
	           ||(pMe->m_dwMemFree/(2*BYTE_SIZE) == MIN_FREE_MEMORY_CARD_SPACE))
	        {               
	            return FALSE;
	        }
	        else
	        {
	            return TRUE;
	        }
	    }
	    else
	    {
	        IFILEMGR_GetFreeSpaceEx(pMe->m_pFileMgr, 
	                                MG_PHONE_ROOTDIR, 
	                                &pMe->m_dwMemTotal, 
	                                &pMe->m_dwMemFree);

	        if((pMe->m_dwMemFree/(2*BYTE_SIZE) < MIN_FREE_PHONE_SPACE) 
	           ||(pMe->m_dwMemFree/(2*BYTE_SIZE) == MIN_FREE_PHONE_SPACE))
	        {               
	            return FALSE;
	        }
	        else
	        {
	            return TRUE;
	        }
	    }
    }
}


// ����
static boolean CameraApp_SelfTimeRecordSnapShot(CCameraApp *pMe)
{
    // ����ʱ�������Ļ
    //IDisplay_ClearScreen(pMe->m_pDisplay);

    (void)ISHELL_SendEvent(pMe->m_pShell,
                           AEECLSID_APP_CAMERA,
                           EVT_USER_REDRAW,
                           0,
                           0);
        
    if(pMe->m_nLeftTime  == 0)
    {
        ISHELL_CancelTimer(pMe->m_pShell, (PFNNOTIFY)CameraApp_SelfTimeRecordSnapShot, pMe);

        if(pMe->m_pCamera)
        {
            pMe->m_nCameraState = CAM_CAPTURE;
            ICAMERA_Stop(pMe->m_pCamera);
            //CameraApp_RecordSnapShot(pMe);
        }
        
        return TRUE;
    }
    else
    {  
        pMe->m_nLeftTime--;
        
        (void)ISHELL_SetTimer(pMe->m_pShell,
                              1000,
                              (PFNNOTIFY)CameraApp_SelfTimeRecordSnapShot,
                              pMe);
    }

    return TRUE;
}

// Draw Bottom Bar Text
static void CameraApp_DrawBottomBarText(CCameraApp *pMe, BottomBar_e_Type eBarType)
{
    uint16      nResID_L = 0;   // left
    uint16      nResID_R = 0;   // right
    uint16      nResID_M = 0;   // mid
    AEERect     rc = pMe->m_rc;
    AECHAR      wszBar_L[20]={0};// left
    AECHAR      wszBar_R[20]={0};// right
    AECHAR      wszBar_M[20]={0};// mid
    
    switch(eBarType)
    {
        case BTBAR_SELECT_BACK:
            nResID_L = IDS_OK; //IDS_SELECT;
            nResID_R = IDS_BACK;
            break;

        case BTBAR_OPTION_BACK:
            nResID_L = IDS_OPTION;
            nResID_R = IDS_BACK;
            break;

        case BTBAR_SAVE_DELETE:
            nResID_L = IDS_SAVE;
            nResID_R = IDS_DEL;
            break;

        default:
            return;
    }

    // ���ذ�ť�ı�
    if(nResID_L != 0)
    {
        (void)ISHELL_LoadResString(pMe->m_pShell,
                                   AEE_APPSCOMMONRES_LANGFILE,
                                   nResID_L,
                                   wszBar_L,
                                   sizeof(wszBar_L));
    }
    
    if(nResID_R != 0)
    {
        (void)ISHELL_LoadResString(pMe->m_pShell,
                                   AEE_APPSCOMMONRES_LANGFILE,
                                   nResID_R,
                                   wszBar_R,
                                   sizeof(wszBar_R));
    }
    
    if(nResID_M != 0)
    {
        (void)ISHELL_LoadResString(pMe->m_pShell,
                                   AEE_APPSCOMMONRES_LANGFILE,
                                   nResID_M,
                                   wszBar_M,
                                   sizeof(wszBar_M));
    }
            
    (void)IDISPLAY_SetColor(pMe->m_pDisplay, CLR_USER_TEXT, WHITE_TEXT);

    // �����ı�-���
    if(WSTRLEN(wszBar_L)>0)
    {      
        DrawTextWithProfile(pMe->m_pShell, 
                            pMe->m_pDisplay, 
                            RGB_BLACK, 
                            AEE_FONT_NORMAL, 
                            wszBar_L, 
                            -1, 
                            0, 
                            0, 
                            &rc, 
                            IDF_ALIGN_BOTTOM|IDF_ALIGN_LEFT|IDF_TEXT_TRANSPARENT);
    }

    // �����ı�-�Ҽ�
    if(WSTRLEN(wszBar_R)>0)
    {
        DrawTextWithProfile(pMe->m_pShell, 
                            pMe->m_pDisplay, 
                            RGB_BLACK, 
                            AEE_FONT_NORMAL, 
                            wszBar_R, 
                            -1, 
                            0, 
                            0, 
                            &rc, 
                            IDF_ALIGN_BOTTOM|IDF_ALIGN_RIGHT|IDF_TEXT_TRANSPARENT);
    }

     // �����ı�-�м��
    if(WSTRLEN(wszBar_M)>0)
    {
         DrawTextWithProfile(pMe->m_pShell, 
                            pMe->m_pDisplay, 
                            RGB_BLACK, 
                            AEE_FONT_NORMAL, 
                            wszBar_M, 
                            -1, 
                            0, 
                            0, 
                            &rc, 
                            IDF_ALIGN_BOTTOM|IDF_ALIGN_CENTER|IDF_TEXT_TRANSPARENT);
    }

    (void)IDISPLAY_SetColor(pMe->m_pDisplay, CLR_USER_TEXT, RGB_WHITE);
}

static boolean CameraApp_SetDateForRecordFileName(CCameraApp *pMe)
{
    AEEMediaData   md;
    
    //Fill media data
    md.clsData = MMD_FILE_NAME;
    md.pData = (void *)pMe->m_sCurrentFileName;
    md.dwSize = 0;

    (void)ICAMERA_SetMediaData(pMe->m_pCamera, &md, 0);

    return TRUE;
}

static boolean CameraApp_GetDateForRecordFileName(CCameraApp *pMe, char * pszDest, unsigned int fileType)
{
    JulianType julian;
 
    GETJULIANDATE(GETTIMESECONDS(), &julian);
   
    if(pMe->m_nCameraStorage == OEMNV_CAMERA_STORAGE_MEMORY_CARD)
    {
        STRCPY(pszDest, AEEFS_CARD0_DIR);
    }
    else
    {
        STRCPY(pszDest, MG_PHONE_ROOTDIR);
    }
    
    switch(fileType){
    case FILE_TYPE_JPG:
    default:
        STRCAT(pszDest, FS_CARD_PICTURES_FOLDER);
#ifdef FEATURE_JPEG_ENCODER
        SPRINTF(pszDest+STRLEN(pszDest), "%02d%02d%02d%02d.jpg", julian.wDay, julian.wHour, julian.wMinute, julian.wSecond);
#else
        SPRINTF(pszDest+STRLEN(pszDest), "%02d%02d%02d%02d.png", julian.wDay, julian.wHour, julian.wMinute, julian.wSecond);
#endif
        break;
   }
   return TRUE;
}

static void CameraApp_SetCameraCaptureSize(CCameraApp *pMe, uint16 wParam)
{
    AEESize displaySize, captureSize;
     
    pMe->m_nCameraSize = wParam;
    if(pMe->m_sensor_model == 30)
    {
        captureSize.cx = g_CameraSizeCFG[pMe->m_nCameraSize].dx;
        captureSize.cy = g_CameraSizeCFG[pMe->m_nCameraSize].dy;
        
        displaySize.cx = g_CameraSizeCFG[0].dx;
        displaySize.cy = g_CameraSizeCFG[0].dy;
    }
    else if(pMe->m_sensor_model == 10)
    {
        captureSize.cx = g_CameraSizeCFG_10[pMe->m_nCameraSize].dx;
        captureSize.cy = g_CameraSizeCFG_10[pMe->m_nCameraSize].dy;
        #ifdef T_QSC1110
        #if defined(FEATURE_DISP_128X160)
        displaySize.cx = g_CameraSizeCFG_10[1].dx;
        displaySize.cy = g_CameraSizeCFG_10[1].dy;
        #else
        displaySize.cx = g_CameraSizeCFG_10[0].dx;
        displaySize.cy = g_CameraSizeCFG_10[0].dy;
        #endif
        #else
        displaySize.cx = g_CameraSizeCFG_10[0].dx;
        displaySize.cy = g_CameraSizeCFG_10[0].dy;
        #endif
    }

    (void)ICONFIG_SetItem(pMe->m_pConfig,
                          CFGI_CAMERA_SIZE,
                          &pMe->m_nCameraSize,
                          sizeof(pMe->m_nCameraSize));
 
    ICAMERA_SetSize(pMe->m_pCamera, &captureSize);
    
    ICAMERA_SetDisplaySize(pMe->m_pCamera, &displaySize);
}

static void CameraApp_HandleSnapshotPic(CCameraApp *pMe)
{
    AECHAR wfileName[MIN_PIC_WCHAR_NAME_LEN];
    
    STRTOWSTR(pMe->m_sCaptureFileName, wfileName, MIN_PIC_WCHAR_NAME_LEN);
    
    // Load Image
    {
        IImage *pImage = ISHELL_LoadImage(pMe->m_pShell,pMe->m_sCurrentFileName);
        if(pImage)
        {
            AEEImageInfo myInfo;
            int x,y;
            IImage_GetInfo(pImage,&myInfo);
            
#ifdef FEATURE_BREW_SCALE
            if(myInfo.cy > 0 && pMe->m_rc.dy > 0)
            {
                if((myInfo.cx*1000)/myInfo.cy > (pMe->m_rc.dx*1000)/pMe->m_rc.dy)
                {
                    myInfo.cx = (myInfo.cx*pMe->m_rc.dy)/myInfo.cy;
                    myInfo.cy = pMe->m_rc.dy;
                }
                else
                {
                    myInfo.cy = (myInfo.cy*pMe->m_rc.dx)/myInfo.cx;
                    myInfo.cx = pMe->m_rc.dx;
                }
                
                IImage_SetParm(pImage,
                               IPARM_SCALE,
                               myInfo.cx,
                               myInfo.cy);
            }
#endif
            x = (myInfo.cx-pMe->m_rc.dx)/2;
            y = (myInfo.cy-pMe->m_rc.dy)/2;
            IImage_SetOffset(pImage,x,y);
            IImage_Draw(pImage,0,0);
            IImage_Release(pImage);
        }
    }
    
    CameraApp_DrawBottomBarText(pMe, BTBAR_SAVE_DELETE);
    
    IDisplay_SetColor(pMe->m_pDisplay, CLR_USER_TEXT, WHITE_TEXT);

    DrawTextWithProfile(pMe->m_pShell, 
                        pMe->m_pDisplay, 
                        RGB_BLACK, 
                        AEE_FONT_NORMAL, 
                        wfileName, 
                        -1, 
                        0, 
                        0, 
                        &pMe->m_rc, 
                        IDF_ALIGN_CENTER|IDF_ALIGN_TOP|IDF_TEXT_TRANSPARENT);
    
    IDisplay_SetColor(pMe->m_pDisplay, CLR_USER_TEXT, RGB_WHITE); 
    
    IDisplay_UpdateEx(pMe->m_pDisplay, FALSE); 
}

// CameraһЩ���ò���,��Ҫ��Preview�����ò�����Ч
static void CameraApp_SetParamAfterPreview(CCameraApp *pMe)
{
    uint32 dwFPS = 0;

    //set camera enviroment
    switch(pMe->m_nCameraEnviroment)
    {
        case OEMNV_CAMERA_ENVIR_AUTO:
            dwFPS = 0;
            break;

        case OEMNV_CAMERA_ENVIR_NIGHT:
            dwFPS = 3;
            break;

        case OEMNV_CAMERA_ENVIR_SUNNY:
            dwFPS = 1;
            break;
        
        case OEMNV_CAMERA_ENVIR_CLOUDY:
            dwFPS = 2;
            break;

        default:
            break;
    }

	MSG_FATAL("pMe->m_sensor_model=%d",pMe->m_sensor_model,0,0);
 	if ( pMe->m_sensor_model == 10)
    {
    	ICAMERA_SetParm(pMe->m_pCamera, CAM_PARM_ROTATE_PREVIEW, 90, 0);
    }
    ICAMERA_SetFramesPerSecond(pMe->m_pCamera, dwFPS);
    ICAMERA_SetParm(pMe->m_pCamera, CAM_PARM_BANDING, pMe->m_nCameraBanding, 0);
}

/*===========================================================================
FUNCTION CameraApp_UpdateFrame

  DESCRIPTION
    This function updates the display with the frame from the viewfinder.

  PARAMETERS:
    pMe - Pointer to  QCam struct

  RETURN VALUE:
    None
===========================================================================*/
static void CameraApp_UpdateFrame(CCameraApp *pMe)
{
  IBitmap *pFrame = NULL;

  if (!pMe)
    return;
#ifndef FEATURE_DSP
  (void)ICAMERA_GetFrame(pMe->m_pCamera, &pFrame);
#endif
  if (!pFrame)
    return;
  
  // Display the frame at center location of the screen
#ifdef FEATURE_CAMERA_NOFULLSCREEN
  IDISPLAY_FillRect(pMe->m_pDisplay, &pMe->m_rc, RGB_BLACK);
  IDISPLAY_BitBlt(pMe->m_pDisplay, pMe->m_rcPreview.x, pMe->m_rcPreview.y, pMe->m_rcPreview.dx, pMe->m_rcPreview.dy, pFrame, 0, 0, AEE_RO_COPY);
#else
  IDISPLAY_BitBlt(pMe->m_pDisplay, pMe->m_rc.x, pMe->m_rc.y, pMe->m_rc.dx, pMe->m_rc.dy, pFrame, 0, 0, AEE_RO_COPY);
#endif
  RELEASEIF(pFrame);
}
#ifdef FEATURE_DSP
static int CameraApp_UpdateInit(CCameraApp *pMe)
{
	IBitmap* pbmp = NULL;
    IDISPLAY_GetDeviceBitmap(pMe->m_pDisplay, &pbmp);
    if(pMe->m_nCameraState == CAM_PREVIEW)
    {
        IBITMAP_FillRect(pbmp, &pMe->m_rc, TRANS_COLOR,AEE_RO_COPY);//,
        IBITMAP_Release(pbmp);  
        //DrawRect(pMe->m_pDisplay, &pMe->m_rc, TRANS_COLOR,AEE_RO_COPY);
        return SUCCESS;
    }
    else
    {
        return EBADSTATE;
    }
        
}

static int CameraApp_Update(CCameraApp *pMe)
{
    //DBGPRINTF("camera state--------------%d\n", pMe->m_nCameraState);
    if(pMe->m_nCameraState == CAM_PREVIEW)
    {
        IBitmap* pbmp = NULL;
        IDIB* pdib = NULL;
        IDISPLAY_UpdateEx(pMe->m_pDisplay, FALSE); 
        IDISPLAY_GetDeviceBitmap(pMe->m_pDisplay, &pbmp);           
        IBITMAP_QueryInterface(pbmp, AEECLSID_DIB, (void**)&pdib);                
        ICAMERA_UpdateScreen(pMe->m_pCamera, (uint32)pdib->pBmp);        
        DBGPRINTF("bmp addr:%0x--- width:%d-----height:%d------depth:%d\n", pdib->pBmp, pdib->cx, pdib->cy, pdib->nDepth);
        IBITMAP_Release(pbmp);  
        IDIB_Release(pdib);
    }    
    else
    {
        return EBADSTATE;
    }
}
#endif
/*===========================================================================
FUNCTION CameraApp_SavePhoto

  DESCRIPTION
    This function starts the encoding processing to save a new photo to the file.

  PARAMETERS:
    pMe - pointer to QCam data struct

  RETURN VALUE:
    NONE
===========================================================================*/
static int CameraApp_SavePhoto(CCameraApp *pMe)
{
  AEEMediaData   md;
  
  if (!pMe)
    return EBADPARM;

  //Fill media data
  md.clsData = MMD_FILE_NAME;
  md.pData = (void *)pMe->m_sCurrentFileName;
  md.dwSize = 0;

  //Start encoding processing
  (void)ICAMERA_SetMediaData(pMe->m_pCamera, &md, 0);
  
  return ICAMERA_EncodeSnapshot(pMe->m_pCamera);
} /* END CameraApp_SavePhoto */

static void CameraApp_EventNotify(CCameraApp *pMe, AEECameraNotify *pcn)
{
    if (!pMe || !pcn || !pMe->m_pCamera)
        return;
    
    switch (pcn->nCmd){
    case CAM_CMD_START:
        switch (pcn->nStatus){
        case CAM_STATUS_FRAME:
            //It is the image from the viewfinder.
            CameraApp_UpdateFrame(pMe);
            ISHELL_PostEvent(pMe->m_pShell, AEECLSID_APP_CAMERA, EVT_USER_REDRAW, NULL, NULL);
            break;
            
        default:
            ISHELL_PostEvent(pMe->m_pShell, AEECLSID_APP_CAMERA, EVT_CAMERA_NOTIFY, pcn->nCmd, pcn->nStatus);
            break;
        }
        break;
    
    case CAM_CMD_SETPARM:
        switch (pcn->nStatus)
        {
        case CAM_STATUS_UPDATE:
        case CAM_STATUS_FAIL:
        case CAM_STATUS_DONE:
            break;
        default:
            break;
        }
        break;
    
    case CAM_CMD_ENCODESNAPSHOT:
        ISHELL_PostEvent(pMe->m_pShell, AEECLSID_APP_CAMERA, EVT_CAMERA_NOTIFY, pcn->nCmd, pcn->nStatus);
        break;
    
    default:
        break;
    }
}

void CameraApp_InitCameraCheck(void *po)
{
    CCameraApp *pMe = (CCameraApp *)po; 
    CameraApp_CreateDirectory(pMe);
 
    if(pMe->m_pCamera == NULL)
    {
    	MSG_FATAL("AEECLSID_CAMERA create.....m_pCamera....",0,0,0);
        ISHELL_CreateInstance(pMe->m_pShell, 
                              AEECLSID_CAMERA, 
                              (void **)&pMe->m_pCamera);
    }
    if(pMe->m_pCamera != NULL)
    {
#ifndef FEATURE_DSP
        MSG_FATAL("pMe->m_pCamera != NULL",0,0,0);
        ICAMERA_GetParm(pMe->m_pCamera, CAM_PARM_SENSOR_MODEL, &(pMe->m_sensor_model), NULL);
        MSG_FATAL("m_sensor_model=%d",pMe->m_sensor_model,0,0); 
#endif
    }
    else
    {
        MSG_FATAL("pMe->m_pCamera == NULL",0,0,0);
    }
    
    if(pMe->m_pCamera)
    {
#ifdef FEATURE_DSP
         ICAMERA_Preview(pMe->m_pCamera);
#else
         ICAMERA_RegisterNotify(pMe->m_pCamera,(PFNCAMERANOTIFY)CameraApp_EventNotify,po);
#endif
    }
}

void CameraApp_AppEventNotify(CCameraApp *pMe, int16 nCmd, int16 nStatus)
{
    if (!pMe || !pMe->m_pCamera)
        return;
    
    switch (nCmd){
    case CAM_CMD_START:
        switch (nStatus){
        case CAM_STATUS_START:
#ifdef FEATURE_CAMERA_NOFULLSCREEN
            IDISPLAY_FillRect(pMe->m_pDisplay, &pMe->m_rc, RGB_BLACK);
            IDISPLAY_Update(pMe->m_pDisplay);
#endif
            break;
            
        case CAM_STATUS_DONE:
            if(pMe->m_nCameraState == CAM_CAPTURE)
            {
                CameraApp_RecordSnapShot(pMe);
            }
            else if(pMe->m_nCameraState == CAM_SAVE)
            {
                CameraApp_SavePhoto(pMe);
                //CLOSE_DIALOG(DLGRET_PICMENU);
            }
            break;

        case CAM_STATUS_ABORT:
        case CAM_STATUS_FAIL:
            if(pMe->m_nCameraState == CAM_CAPTURE)
            {
                pMe->m_bRePreview = TRUE;
                pMe->m_wMsgID = IDS_MSG_CAPTURE_FAILED;
                pMe->m_nMsgTimeout = TIMEOUT_MS_MSGDONE;
                CLOSE_DIALOG(DLGRET_POPMSG);
            }
            break;
            
        default:
            break;
        }
        break;
    
    case CAM_CMD_SETPARM:
        switch (nStatus){
        case CAM_STATUS_UPDATE:
        case CAM_STATUS_FAIL:
        case CAM_STATUS_DONE:
            break;
        default:
            break;
        }
        break;
    case CAM_CMD_ENCODESNAPSHOT:
        switch (nStatus){
        case CAM_STATUS_ABORT:
        case CAM_STATUS_FAIL:
            if(pMe->m_nCameraState == CAM_SAVE)
            {
                pMe->m_bRePreview = TRUE;
                pMe->m_wMsgID = IDS_MSG_CAPTURE_FAILED;
                pMe->m_nMsgTimeout = TIMEOUT_MS_MSGDONE;
                CLOSE_DIALOG(DLGRET_POPMSG);
            }
            break;
            
        case CAM_STATUS_DONE:
            (void)ICONFIG_GetItem(pMe->m_pConfig,
                      CFGI_CAMERA_TONE,
                      &pMe->m_nCameraTone,
                      sizeof(pMe->m_nCameraTone));

#ifdef FEATURE_CARRIER_CHINA_TELCOM
            CameraApp_PlayShutterSound(pMe);
#else
            if(pMe->m_nCameraTone == OEMNV_CAMERA_SHUTTER_TONE_ENABLE)
            {
                CameraApp_PlayShutterSound(pMe);
            }
#endif
            CLOSE_DIALOG(DLGRET_PICMENU);
            break;
            
        default:
            break;
        }
        
        break;
        
   default:
    break;
  }
}

// �����ļ���Ŀ¼
static void CameraApp_CreateDirectory(CCameraApp *pMe)
{
    // �ֻ��ϵ���Ƭ����Ŀ¼
    if(SUCCESS != IFILEMGR_Test(pMe->m_pFileMgr, MG_PHONEPICTURE_PATH))
    {
        IFILEMGR_MkDir(pMe->m_pFileMgr, MG_PHONEPICTURE_PATH);
    }
	
    pMe->m_bMemoryCardExist = CameraApp_FindMemoryCardExist(pMe);	

    if(pMe->m_bMemoryCardExist)
    {
         // T���ϵ���Ƭ����Ŀ¼
         if(SUCCESS != IFILEMGR_Test(pMe->m_pFileMgr, MG_MASSCARDPICTURE_PATH))
         {
             IFILEMGR_MkDir(pMe->m_pFileMgr, MG_MASSCARDPICTURE_PATH);
         }
    
         // T���ϵ�¼�񱣴�Ŀ¼
         if(SUCCESS != IFILEMGR_Test(pMe->m_pFileMgr, MG_MASSCARDVIDEO_PATH))
         {
             IFILEMGR_MkDir(pMe->m_pFileMgr, MG_MASSCARDVIDEO_PATH);
         }   
    }           
}

