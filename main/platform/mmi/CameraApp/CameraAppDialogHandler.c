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
#if defined(FEATURE_DISP_160X128)
#include "Appscommon_160x128.brh"
#elif defined(FEATURE_DISP_128X128)
#include "Appscommon_color.brh"
#else
#include "Appscommon_color.brh"
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

// �Ի���IDD_CFUN�¼�������
static boolean CameraApp_FunHandleEvent(CCameraApp *pMe, 
                                        AEEEvent eCode, 
                                        uint16 wParam, 
                                        uint32 dwParam);

// �Ի���IDD_CFUN frame�¼�������
static boolean CameraApp_CFunFrameEffectHandle(CCameraApp *pMe, 
                                               AEEEvent eCode, 
                                               uint16 wParam, 
                                               int dwParam);

// �Ի���IDD_CFUN color�¼�������
static boolean CameraApp_CFunColorEffectHandle(CCameraApp *pMe, 
                                               AEEEvent eCode, 
                                               uint16 wParam, 
                                               int dwParam);


// �Ի���IDD_CVIDEO�¼�������
static boolean CameraApp_VideoHandleEvent(CCameraApp *pMe, 
                                          AEEEvent eCode, 
                                          uint16 wParam, 
                                          uint32 dwParam);

// �Ի���IDD_CVIDEOCFG�¼�������
static boolean CameraApp_VideoCFGHandleEvent(CCameraApp *pMe, 
                                             AEEEvent eCode, 
                                             uint16 wParam, 
                                             uint32 dwParam);

// �Ի���IDD_CVIDEOSAVE�¼�������
static boolean CameraApp_VideoSaveHandleEvent(CCameraApp *pMe, 
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

// PopMenu video/camera�л��˵�������
static boolean CameraApp_PopMenu_ChooseCommandHandleEvent(CCameraApp *pMe, 
                                                          uint16 wParam);

// PopMenu����ģʽ�˵�������                           
static boolean CameraApp_PopMenu_EnvironmentCommandHandleEvent(CCameraApp *pMe, 
                                                               uint16 wParam);

// PopMenuȤζģʽ�˵�������
static boolean CameraApp_PopMenu_FunCommandHandleEvent(CCameraApp *pMe, 
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

// PopMenu����ģʽ�˵�������                                                                                                                  
static boolean CameraApp_PopMenu_CaptureCommandHandleEvent(CCameraApp *pMe, 
                                                           uint16 wParam);
                                                                                              
// PopMenu�չ��banding������
static boolean CameraApp_PopMenu_BandingCommandHandleEvent(CCameraApp *pMe, 
                                                           uint16 wParam);

// PopMenu���մ洢λ�ô�����
static boolean CameraApp_PopMenu_StorageCommandHandleEvent(CCameraApp *pMe, 
                                                           uint16 wParam);

// PopMenu camera/video���ø�λ������
static boolean CameraApp_PopMenu_ResetCFGCommandHandleEvent(CCameraApp *pMe, 
                                                            uint16 wParam);

// HotKey avk_1 �������������� 
static boolean CameraApp_HotKeyAVK_1_QualityHandleEvent(CCameraApp *pMe, 
                                                        AEEEvent eCode, 
                                                        uint16 wParam, 
                                                        uint32 dwParam);

// HotKey avk_2 camera/video�л�������
static boolean CameraApp_HotKeyAVK_2_ChooseHandleEvent(CCameraApp *pMe, 
                                                       AEEEvent eCode, 
                                                       uint16 wParam, 
                                                       uint32 dwParam);

// HotKey avk_3 ����ģʽ������ 
static boolean CameraApp_HotKeyAVK_3_CaptureHandleEvent(CCameraApp *pMe, 
                                                        AEEEvent eCode, 
                                                        uint16 wParam, 
                                                        uint32 dwParam);
// Hotkey avk_4 ������������
static boolean CameraApp_HotKeyAVK_4_ShutterToneHandleEvent(CCameraApp *pMe, 
                                                            AEEEvent eCode, 
                                                            uint16 wParam, 
                                                            uint32 dwParam);

// Hotkey avk_5 ���ջ���������
static boolean CameraApp_HotKeyAVK_5_EnvironmentHandleEvent(CCameraApp *pMe, 
                                                            AEEEvent eCode, 
                                                            uint16 wParam, 
                                                            uint32 dwParam);

// Hotkey avk_6 �������
static boolean CameraApp_HotKeyAVK_6_FrameHandleEvent(CCameraApp *pMe, 
                                                      AEEEvent eCode, 
                                                      uint16 wParam, 
                                                      uint32 dwParam);


// Hotkey avk_7 ��ɫ������
static boolean CameraApp_HotKeyAVK_7_ColorHandleEvent(CCameraApp *pMe, 
                                                      AEEEvent eCode, 
                                                      uint16 wParam, 
                                                      uint32 dwParam);

// Hotkey avk_8 ���Ĵ�����
static boolean CameraApp_HotKeyAVK_8_SelfTimeHandleEvent(CCameraApp *pMe, 
                                                        AEEEvent eCode, 
                                                        uint16 wParam, 
                                                        uint32 dwParam);

// Hotkey avk_star ���ճߴ紦����
static boolean CameraApp_HotKeyAVK_STAR_SizeHandleEvent(CCameraApp *pMe, 
                                                        AEEEvent eCode, 
                                                        uint16 wParam, 
                                                        uint32 dwParam);

// Hotkey avk_pound ����ͼ�괦����
static boolean CameraApp_HotKeyAVK_POUND_HideIconHandleEvent(CCameraApp * pMe, 
                                                             AEEEvent eCode, 
                                                             uint16 wParam, 
                                                             uint32 dwParam);

// ��ʾ Hotkey ��ʾ�ַ�������
static boolean CameraApp_DisplayHotKeyText(CCameraApp *pMe, 
                                           AEEEvent eCode, 
                                           uint16 wParam, 
                                           uint32 dwParam);
                                                
// ��ʱ��������
static void CameraApp_HotKeyDisplayTimeout(CCameraApp *pMe);

// ���ò˵���ѡ��ͼ��
static void CameraApp_SetCFGMenuIcon(IMenuCtl *pm, 
                                     uint16 listId, 
                                     boolean bSel);

// ��ʼ�����ò˵�
static boolean CameraApp_InitpopMenu(CCameraApp *pMe, 
                                     IMenuCtl *popMenu);

// ��ʼ����������
static void CameraApp_InitCFGData(CCameraApp * pMe);

// ��ʼ��Camera/video�л����ò˵�
static void CameraApp_PopMenu_ChooseInit(CCameraApp *pMe, 
                                         IMenuCtl *popMenu);

// ��ʼ�����ջ������ò˵�
static void CameraApp_PopMenu_EnvironmentInit(CCameraApp *pMe, 
                                              IMenuCtl *popMenu);

// ��ʼ��Ȥζģʽ���ò˵�
static void CameraApp_PopMenu_FunInit(CCameraApp *pMe, 
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

// ��ʼ������ģʽ���ò˵�
static void CameraApp_PopMenu_CaptureInit(CCameraApp *pMe, 
                                          IMenuCtl *popMenu);

// ��ʼ��Bandig���ò˵�
static void CameraApp_PopMenu_BandingInit(CCameraApp *pMe, 
                                          IMenuCtl *popMenu);

// ��ʼ�����մ洢λ�����ò˵�
static void CameraApp_PopMenu_StorageInit(CCameraApp *pMe, 
                                          IMenuCtl *popMenu);

// ��ʼ���ָ�Ĭ�����ò˵�
static void CameraApp_PopMenu_ResetCFGInit(CCameraApp *pMe, 
                                           IMenuCtl *popMenu);

// ��ʼ����ݼ�˵�����ò˵�
static void CameraApp_PopMenu_HotKeyInit(CCameraApp *pMe, 
                                         IMenuCtl *popMenu);

// Video¼��ʱ�Ĵ�����
static void CameraApp_VideoRunning(CCameraApp *pMe);

// Video¼���õ�ǰʱ�䴦����
static void CameraApp_VideoGettime(CCameraApp *pMe, 
                                   uint32 nTicks);

// Video¼��ֹͣ������
static void CameraApp_VideoStop(CCameraApp *pMe);

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

// ����176x220���������
static void CameraApp_SetCameraFunFrame(CCameraApp *pMe);

// ��Video��CFGBar����
static void CameraApp_DrawVCFGBar(CCameraApp * pMe);

// ����ͼƬX����
static uint16 CameraApp_SetPositonX(CCameraApp *pMe);

// ��CFGBar����ʾ�ı�
static void CamreaApp_DrawCFGPromptText(CCameraApp *pMe);

// ����ӦBar�ı���ͼ
static void CameraApp_DrawBarBg(CCameraApp *pMe, 
                                int16 nResID, 
                                int x, 
                                int y);

// ��Camera��CFGBar����
static void CameraApp_DrawCCFGBar(CCameraApp * pMe);

// ��Video��TopBar����
static void CameraApp_DrawVideoTopBar(CCameraApp *pMe);

#ifndef FEATURE_CAMERAAPP_FUN_FRAME
// ����ʾeffect cloro�����ò˵�
static void CameraApp_HandlePopMenuEffectCFG(CCameraApp *pMe, 
                                             IMenuCtl *popMenu, 
                                             boolean bHandleEffect);
#endif

// Camera Preview����������
static void CameraApp_CPreviewStart(CCameraApp *pMe);

// Camera Snapshot�Ĵ�����
static void CameraApp_RecordSnapShot(CCameraApp *pMe);

// Color effect�Ĵ�����
static void CameraApp_HandleColorEffectCFG(CCameraApp *pMe);

// ����color effect����
static void CameraApp_SetEffectColor(CCameraApp *pMe,
                                     int color);

// ��Bar�ϻ��ı��ַ��ĺ���
static void CameraApp_DrawBarText(CCameraApp *pMe, 
                                  int16 nResID, 
                                  int postionY);

// ����ɫ����ʾ�ַ�
static void CameraApp_DrawColorPromptText(CCameraApp *pMe);

// ���������ʾ�ַ�
static void CameraApp_DrawFramePromptText(CCameraApp *pMe);

// color effect��ѡ�д�����
static int CameraApp_ColorEffectSel(CCameraApp *pMe);

// ������ɫ��Ч
static int CameraApp_SetColorEffect(CCameraApp *pMe);

// ��ȡ��ɫ��Ч������Դid
static int16 CameraApp_GetColorEffectResID(CCameraApp *pMe);

// ��T���ĺ���
static boolean CameraApp_FindMemoryCardExist(CCameraApp *pMe);

// ��Sensor�ĺ���
//static boolean CameraApp_FindSensorExist(CCameraApp *pMe);

// Dialog�Ķ�ʱ������
static void CameraApp_DialogTimeout(void *pme);

// Camera Previewʱ��δ�ҵ�T���Ĵ�����
static boolean CameraApp_PreviewFindNoMemoryCardExistHandle(CCameraApp *pMe);

// �洢�ռ�ļ�⺯��
static boolean CameraApp_IsEnoughfMemorySpace(CCameraApp *pMe);


// ���ı���Χ�ӱ߿�Ĵ�����
static void CameraApp_DrawHotKeyTextWithFrame(CCameraApp *pMe);

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

// ¼���¼���ļ�������
static void CameraApp_HandleVideoFile(CCameraApp *pMe);

// Jpg���봦����
void CameraApp_DecodeJPG(CCameraApp *pMe);

// Jpg����Ļص�����
void CameraApp_JPGNotify(void *po, IImage *pIImage, AEEImageInfo *pii, int nErr);

// ����Camera Preview�Ĳ���
static void CameraApp_SetParamAfterPreview(CCameraApp * pMe);

void CameraApp_InitCameraCheck(void *po);

static void CameraApp_CreateDirectory(CCameraApp *pMe);

#ifndef FEATURE_CAMERAAPP_FUN_FRAME
static void CameraApp_SetFunColorMenuItemSel(CCameraApp *pMe, 
                                             uint16 wParam);

static void CameraApp_SetColorCFGMenuIcon(CCameraApp *pMe, 
                                          IMenuCtl *popMenu);
#endif

/*==============================================================================
                                 ȫ������
==============================================================================*/
int     jpgSize;
boolean refreshJPG = FALSE;

/*==============================================================================
                                 ���أ���̬������
==============================================================================*/
static boolean jpg_decoder_done = FALSE;

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
            if(dlgResId == IDD_CPREVIEW || dlgResId == IDD_CVIDEO)
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

        case IDD_CVIDEO:
            return CameraApp_VideoHandleEvent(pMe, eCode, wParam, dwParam);

        case IDD_CVIDEOCFG:
            return CameraApp_VideoCFGHandleEvent(pMe, eCode, wParam, dwParam);

        case IDD_CVIDEOSAVE:
            return CameraApp_VideoSaveHandleEvent(pMe, eCode, wParam, dwParam);

        case IDD_CFUN:
            return CameraApp_FunHandleEvent(pMe, eCode, wParam, dwParam);
            
        case IDD_POPMSG:
            return CameraApp_PopMSGHandleEvent(pMe, eCode, wParam, dwParam);
                           
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
   
    if(pMenu == NULL)
    {
        return FALSE;
    }
   
    switch(eCode) 
    {
        case EVT_DIALOG_INIT:
            pMe->m_bIsPreview = FALSE;
            pMe->m_nCameraState = CAM_START;
            //pMe->m_bCalculateLeftPicNum = TRUE;
			
            pMe->m_nCameraCFG = CAMERACFGCHOOSE;
	        pMe->m_nVideoCFG = VIDEOCFGCHOOSE;
            pMe->m_nCFGBarChoosedID = 0; 
            pMe->m_nCFGBarOffset = 0;
		 
            pMe->m_wMsgID = IDS_MSG_WAITING;
            pMe->m_nMsgTimeout = TIMEOUT_MS_MSGBOX;

            IDISPLAY_SetClipRect(pMe->m_pDisplay, 0); 

            IANNUNCIATOR_EnableAnnunciatorBar(pMe->m_pIAnn,AEECLSID_DISPLAY1,TRUE);

            // �ر�Camera��ʼ������
            #if 0
            if(pMe->m_pBusyStateImage)
            {
                IImage_Stop(pMe->m_pBusyStateImage);
                IImage_Release(pMe->m_pBusyStateImage);
                pMe->m_pBusyStateImage = NULL;                
            } 
            #endif

            // ���ò˵�����
            IMENUCTL_SetProperties(pMenu, MP_WRAPSCROLL|MP_BIND_ITEM_TO_NUMBER_KEY);
            IMENUCTL_SetOemProperties(pMenu, OEMMP_USE_MENU_STYLE);
                     
            // ��ʼ���˵���
            IMENUCTL_AddItem(pMenu, AEE_APPSCAMERAAPP_RES_FILE, IDS_ITEM_CAMERA, IDS_ITEM_CAMERA, NULL, NULL);
#ifdef FEATURE_VIDEO_ENCODE
            IMENUCTL_AddItem(pMenu, AEE_APPSCAMERAAPP_RES_FILE, IDS_ITEM_VIDEO, IDS_ITEM_VIDEO, NULL, NULL);
#endif
            IMENUCTL_AddItem(pMenu, AEE_APPSCAMERAAPP_RES_FILE, IDS_ITEM_CAMERA_GALLERY, IDS_ITEM_CAMERA_GALLERY, NULL, NULL);
#ifdef FEATURE_APP_MPEG4
            IMENUCTL_AddItem(pMenu, AEE_APPSCAMERAAPP_RES_FILE, IDS_ITEM_VIDEO_GALLERY, IDS_ITEM_VIDEO_GALLERY, NULL, NULL);
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
                    // set the annunciator disable
                    IANNUNCIATOR_EnableAnnunciatorBar(pMe->m_pIAnn,AEECLSID_DISPLAY1,FALSE);
                    
                    pMe->m_nMainMenuItemSel = IDS_ITEM_CAMERA;

                    pMe->m_bMemoryCardExist = CameraApp_FindMemoryCardExist(pMe);
                    
                    // ���T�������ڣ�defualt�������ֻ���
                    if(!pMe->m_bMemoryCardExist)
                    {
                        pMe->m_nCameraStorage = OEMNV_CAMERA_STORAGE_PHONE;
                        
                        pMe->m_nCameraSize = OEMNV_CAMERA_SIZE_176_220;

                        (void)ICONFIG_SetItem(pMe->m_pConfig,
                                              CFGI_CAMERA_STORAGE,
                                              &pMe->m_nCameraStorage,
                                              sizeof(pMe->m_nCameraStorage));  

                        (void)ICONFIG_SetItem(pMe->m_pConfig,
                                              CFGI_CAMERA_SIZE,
                                              &pMe->m_nCameraSize,
                                              sizeof(pMe->m_nCameraSize)); 
                    }
         
                    CLOSE_DIALOG(DLGRET_POPMSG);
                    break;
                  
                case IDS_ITEM_VIDEO:
                    // set the annunciator disable
                    IANNUNCIATOR_EnableAnnunciatorBar(pMe->m_pIAnn,AEECLSID_DISPLAY1,FALSE);
                    
                    pMe->m_nMainMenuItemSel = IDS_ITEM_VIDEO;

                    pMe->m_bMemoryCardExist = CameraApp_FindMemoryCardExist(pMe);
                    
                    if(!pMe->m_bMemoryCardExist)
                    {
                        pMe->m_wMsgID = IDS_MSG_NOSDCARD;
                        pMe->m_nMsgTimeout = TIMEOUT_MS_MSGBOX;
                        
                    }
                    
                    CLOSE_DIALOG(DLGRET_POPMSG);
                    break;

                case IDS_ITEM_CAMERA_GALLERY:
                    pMe->m_nMainMenuItemSel = IDS_ITEM_CAMERA_GALLERY;
                    CMediaGallery_FileExplorer(GALLERY_PHOTO_BROWSE, NULL);
                    break;


                 case IDS_ITEM_VIDEO_GALLERY:
                    pMe->m_nMainMenuItemSel = IDS_ITEM_VIDEO_GALLERY;
                    CMediaGallery_FileExplorer(GALLERY_VIDEO_BROWSE, NULL);
                    break;
                                  
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
    AEERect rectStatic;
    int   nCameraSelfTime = 0;
    
    switch(eCode) 
    {
        case EVT_DIALOG_INIT:         
            pMe->m_nSnapShotPicNameIndex = 0;

            pMe->m_bCapturePic = FALSE;

            IDISPLAY_SetClipRect(pMe->m_pDisplay, 0);

            (void)ICONFIG_GetItem(pMe->m_pConfig,
                                  CFGI_CAMERA_ICON,
                                  &pMe->m_bCameraHideIcon,
                                  sizeof(pMe->m_bCameraHideIcon));

            (void)ICONFIG_GetItem(pMe->m_pConfig,
                                  CFGI_CAMERA_SIZE,
                                  &pMe->m_nCameraSize,
                                  sizeof(pMe->m_nCameraSize));

            (void)ICONFIG_GetItem(pMe->m_pConfig,
                                  CFGI_CAMERA_COLOR,
                                  &pMe->m_nCameraColor,
                                  sizeof(pMe->m_nCameraColor));

            (void)ICONFIG_GetItem(pMe->m_pConfig,
                                  CFGI_CAMERA_QUALITY,
                                  &pMe->m_nCameraQuality,
                                  sizeof(pMe->m_nCameraQuality));
            

            (void)ICONFIG_GetItem(pMe->m_pConfig,
                                  CFGI_CAMERA_ENVIROMENT,
                                  &pMe->m_nCameraEnviroment,
                                  sizeof(pMe->m_nCameraEnviroment));

            (void)ICONFIG_GetItem(pMe->m_pConfig,
                                  CFGI_CAMERA_FRAME,
                                  &pMe->m_nCameraFrame,
                                  sizeof(pMe->m_nCameraFrame));

            (void)ICONFIG_GetItem(pMe->m_pConfig,
                                  CFGI_CAMERA_BRIGHTNESS,
                                  &pMe->m_nCameraBrightness,
                                  sizeof(pMe->m_nCameraBrightness));

            (void)ICONFIG_GetItem(pMe->m_pConfig,
                                  CFGI_CAMERA_BANDING,
                                  &pMe->m_nCameraBanding,
                                  sizeof(pMe->m_nCameraBanding));

            (void)ICONFIG_GetItem(pMe->m_pConfig,
                                  CFGI_CAMERA_STORAGE,
                                  &pMe->m_nCameraStorage,
                                  sizeof(pMe->m_nCameraStorage));            
            
            /*pMe->m_pImage = ISHELL_LoadResImage(pMe->m_pShell, 
                                                CAMERAAPP_IMAGE_RES_FILE, 
                                                IDI_CAMERA_BUSY);*/
            
            // ����IStatic�ؼ����ԣ�������ʾ��ݼ���ʾ�ַ�
            SETAEERECT(&rectStatic, 30, 74, 116, 40);

            ISTATIC_SetRect(pMe->m_pStatic, &rectStatic);
            
            ISTATIC_SetProperties(pMe->m_pStatic, ST_MIDDLETEXT|ST_CENTERTEXT|ST_TRANSPARENTBACK);
            return TRUE;
     
        case EVT_DIALOG_START:              
            pMe->m_bCanCapture = CameraApp_IsEnoughfMemorySpace(pMe);
            
            ISHELL_PostEvent(pMe->m_pShell, AEECLSID_APP_CAMERA, EVT_USER_REDRAW, NULL, NULL);  
            return TRUE;
         
        case EVT_DIALOG_END:            
            ISHELL_CancelTimer(pMe->m_pShell, NULL, pMe);
            return TRUE;
      
        case EVT_USER_REDRAW:
            // ��ֹ���ٰ��������趨�Ķ�ʱ��δȡ��            
            ISHELL_CancelTimer(pMe->m_pShell, (PFNNOTIFY)CameraApp_HotKeyDisplayTimeout, pMe);

            /*if(pMe->m_bshowBusyState)
            {
                if(pMe->m_pImage)
                {
                    IDisplay_SetColor(pMe->m_pDisplay, CLR_USER_BACKGROUND, RGB_BLACK);
                    IIMAGE_SetFrameCount(pMe->m_pImage, 6);
                    IIMAGE_Start(pMe->m_pImage, 53, 75);
                }
 
                pMe->m_bshowBusyState = FALSE;
            }*/

            // camera preview start....
            if(pMe->m_pCamera && (!pMe->m_bIsPreview))
            {   
                CameraApp_CPreviewStart(pMe);                
            }

            if(pMe->m_bRePreview)
            {
                ICAMERA_Preview(pMe->m_pCamera);
                pMe->m_nCameraState = CAM_PREVIEW;
                CameraApp_SetParamAfterPreview(pMe);
                pMe->m_bRePreview = FALSE;
            }
     
            if(pMe->m_nCameraSize != OEMNV_CAMERA_SIZE_176_220)
            {
#ifdef FEATURE_CARRIER_CHINA_VERTU
                CameraApp_DrawBarBg(pMe, IDI_TOPBAR_BG_VERTU, 0, 0);

                CameraApp_DrawBarBg(pMe, IDI_CFG_BG_VERTU, 0, 160); 
#else            
                CameraApp_DrawBarBg(pMe, IDI_TOPBAR_BG, 0, 0);

                CameraApp_DrawBarBg(pMe, IDI_CFG_BG, 0, 160); 
#endif                
                DRAW_BOTTOMBAR(BTBAR_OPTION_BACK);
            
            }
            else
            {
                IDisplay_ClearScreen(pMe->m_pDisplay);

                CameraApp_SetCameraFunFrame(pMe);

                CameraApp_DrawBottomBarText(pMe, BTBAR_OPTION_BACK);             
            }

            CameraApp_DrawMidPic(pMe);

            if(!pMe->m_bCameraHideIcon)
            {
                CameraApp_DrawTopBar(pMe);
            }         
                        
            IDISPLAY_UpdateEx(pMe->m_pDisplay, FALSE);

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
            
            switch(wParam)
            {
                case AVK_0:
                    return TRUE;
                    
                case AVK_1:
                    return CameraApp_HotKeyAVK_1_QualityHandleEvent(pMe, eCode, wParam, dwParam);
                    
                case AVK_2:
                    return CameraApp_HotKeyAVK_2_ChooseHandleEvent(pMe, eCode, wParam, dwParam);
                    
                case AVK_3:
                    return CameraApp_HotKeyAVK_3_CaptureHandleEvent(pMe, eCode, wParam, dwParam);
                    
                case AVK_4:
                    return CameraApp_HotKeyAVK_4_ShutterToneHandleEvent(pMe, eCode, wParam, dwParam);
                    
                case AVK_5:
                    return CameraApp_HotKeyAVK_5_EnvironmentHandleEvent(pMe, eCode, wParam, dwParam);
                    
                case AVK_6:
                    return CameraApp_HotKeyAVK_6_FrameHandleEvent(pMe, eCode, wParam, dwParam);
                    
                case AVK_7:
                    return CameraApp_HotKeyAVK_7_ColorHandleEvent(pMe, eCode, wParam, dwParam);
                    
                case AVK_8:
                    return CameraApp_HotKeyAVK_8_SelfTimeHandleEvent(pMe, eCode, wParam, dwParam);

                case AVK_STAR:
                    return CameraApp_HotKeyAVK_STAR_SizeHandleEvent(pMe, eCode, wParam, dwParam);

                case AVK_POUND:
                    return CameraApp_HotKeyAVK_POUND_HideIconHandleEvent(pMe, eCode, wParam, dwParam); 

                case AVK_END:
                    ISHELL_CancelTimer(pMe->m_pShell, NULL, pMe);
                    if(pMe->m_nCameraState == CAM_PREVIEW)
                    {
                        ICAMERA_Stop(pMe->m_pCamera);
                        pMe->m_nCameraState = CAM_STOP;
                    }                   
                    return FALSE;

                case AVK_CLR:   
                    if(SUCCESS == ICAMERA_Stop(pMe->m_pCamera))
                    {
                        pMe->m_bIsPreview = FALSE;
                        pMe->m_nCameraState = CAM_STOP;
                        CLOSE_DIALOG(DLGRET_CANCELED);
                    }                    
                    break;
   
                case AVK_INFO:
                    // ��ֹ���ٰ���������hotkey Text������LCD��                                      
                    CameraApp_HotKeyDisplayTimeout(pMe);

                    ISHELL_CancelTimer(pMe->m_pShell, NULL, pMe);

                    if(!pMe->m_bCanCapture)
                    {
                        pMe->m_wMsgID = IDS_MSG_NOMEMORY;
                        pMe->m_nMsgTimeout = TIMEOUT_MS_MSGBOX;
                        ICAMERA_Stop(pMe->m_pCamera);
                        pMe->m_bIsPreview = FALSE;
                        CLOSE_DIALOG(DLGRET_POPMSG);
                        return TRUE;
                    }

                    MEMSET(pMe->m_sCaptureFileName, '\0', sizeof(pMe->m_sCaptureFileName));

                    // find sensor
                    #if 0
                    if((pMe->m_nCameraStorage == OEMNV_CAMERA_STORAGE_SDCARD) && (!CameraApp_FindMemoryCardExist(pMe)))
                    {
                        if(CameraApp_PreviewFindNoMemoryCardExistHandle(pMe))
                        {
                            return TRUE;
                        }
                    }
                    #endif

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

                    switch(pMe->m_nCaptureItemSel)
                    {
                        case IDS_CAPTURE_OFF:
                        case IDS_CAPTURE_MODE1:
                            pMe->m_nSnapShotTimes = 1;
                            pMe->m_nSnapShotPicNum = 1;
                            break;

                        case IDS_CAPTURE_MODE2:
                            pMe->m_nSnapShotTimes = 3;
                            pMe->m_nSnapShotPicNum = 3;
                            break;

                        case IDS_CAPTURE_MODE3:
                            pMe->m_nSnapShotTimes = 5;
                            pMe->m_nSnapShotPicNum = 5;
                            break;

                        default:
                            break;
                    }
                    
                    pMe->m_nLeftTime = nCameraSelfTime;              
                    pMe->m_bCleanScreen = TRUE;
                        
                    if(nCameraSelfTime == 0)
                    {
                        CameraApp_RecordSnapShot(pMe);
                    }
                    else
                    {
                        // ����ʱ����                       
                        CameraApp_SelfTimeRecordSnapShot(pMe);
                    }
                         
                    break;

                case AVK_UP:
                    // ��ֹ���ٰ���������hotkey Text������LCD��
                    CameraApp_HotKeyDisplayTimeout(pMe);                

                    // ����zoom�ȼ���176x220Ϊ5����320x240Ϊ4����640x480,1280x960Ϊ2����1600x1200Ϊ1��
                    switch(pMe->m_nCameraSize)
                    {
                        case OEMNV_CAMERA_SIZE_176_220:
                            if(pMe->m_nCameraZoom != OEMNV_CAMERA_ZOOM_LEVEL5)
                            {
                                pMe->m_nCameraZoom++;
                            }
                            else
                            {
                                return TRUE;
                            }
                            
                            break;
                            

                        case OEMNV_CAMERA_SIZE_320_240:
                            if(pMe->m_nCameraZoom != OEMNV_CAMERA_ZOOM_LEVEL4)
                            {
                                pMe->m_nCameraZoom++;
                            }
                            else
                            {
                                return TRUE;
                            }
                            
                            break;

                        case OEMNV_CAMERA_SIZE_640_480:
                            if(pMe->m_nCameraZoom != OEMNV_CAMERA_ZOOM_LEVEL2)
                            {
                                pMe->m_nCameraZoom++;
                            }
                            else
                            {
                                return TRUE;
                            }
                            
                            break; 

                        case OEMNV_CAMERA_SIZE_1280_960:
                            if(pMe->m_nCameraZoom != OEMNV_CAMERA_ZOOM_LEVEL2)
                            {
                                pMe->m_nCameraZoom++;
                            }
                            else
                            {
                                return TRUE;
                            }
                            
                            break;

                        case OEMNV_CAMERA_SIZE_1600_1200: 
                        default:
                            return TRUE; 
                    }
         
                    ICAMERA_SetZoom(pMe->m_pCamera, pMe->m_nCameraZoom); 

                    // ����zoom��camera preview����Ҫ����
                    return ISHELL_SendEvent(pMe->m_pShell, AEECLSID_APP_CAMERA, EVT_USER_REDRAW, NULL, NULL);
                    
                case AVK_DOWN:
                    // ��ֹ���ٰ���������hotkey Text������LCD��
                    CameraApp_HotKeyDisplayTimeout(pMe);                    
                     
                    if(pMe->m_nCameraZoom != OEMNV_CAMERA_ZOOM_LEVEL1)
                    {
                        pMe->m_nCameraZoom--;
                    }
                    else
                    {
                        return TRUE;
                    }
                    
                    ICAMERA_SetZoom(pMe->m_pCamera, pMe->m_nCameraZoom); 
                    
                    return ISHELL_SendEvent(pMe->m_pShell, 
                                            AEECLSID_APP_CAMERA, 
                                            EVT_USER_REDRAW, 
                                            NULL, 
                                            NULL);;

                case AVK_LEFT:
                    //��ֹ���ٰ���������hotkey Text������LCD��
                    CameraApp_HotKeyDisplayTimeout(pMe);
                    
                    if(pMe->m_nCameraBrightness != OEMNV_CAMERA_BRIGHTNESS_LEVEL1)
                    {
                        pMe->m_nCameraBrightness--;
                    }
                    else
                    {
                        return TRUE;
                    }

                    ICAMERA_SetBrightness(pMe->m_pCamera, pMe->m_nCameraBrightness);
                    
                    (void)ICONFIG_SetItem(pMe->m_pConfig,
                                          CFGI_CAMERA_BRIGHTNESS,
                                          &pMe->m_nCameraBrightness,
                                          sizeof(pMe->m_nCameraBrightness));
                    
                    return ISHELL_SendEvent(pMe->m_pShell, 
                                            AEECLSID_APP_CAMERA, 
                                            EVT_USER_REDRAW, 
                                            NULL, 
                                            NULL);;

                case AVK_RIGHT:
                    //��ֹ���ٰ���������hotkey Text������LCD��
                    CameraApp_HotKeyDisplayTimeout(pMe);
                    
                    if(pMe->m_nCameraBrightness != OEMNV_CAMERA_BRIGHTNESS_LEVEL5)
                    {
                        pMe->m_nCameraBrightness++;
                    }
                    else
                    {
                        return TRUE;
                    }

                    ICAMERA_SetBrightness(pMe->m_pCamera, pMe->m_nCameraBrightness);
                    
                    (void)ICONFIG_SetItem(pMe->m_pConfig,
                                          CFGI_CAMERA_BRIGHTNESS,
                                          &pMe->m_nCameraBrightness,
                                          sizeof(pMe->m_nCameraBrightness));
                    
                    return ISHELL_SendEvent(pMe->m_pShell, 
                                            AEECLSID_APP_CAMERA, 
                                            EVT_USER_REDRAW, 
                                            NULL, 
                                            NULL);
                                      
                default:
                    break;     
            } 
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
    AEERect eRec;
    
    IMenuCtl *popMenu = (IMenuCtl*)IDIALOG_GetControl(pMe->m_pActiveDlg, IDC_CAMERA_CFGMENU);    
           
    if(popMenu == NULL)
    {
        return FALSE;
    }

    SETAEERECT(&eRec, 0, 72, 176, 126);
          
    switch(eCode) 
    {
        case EVT_DIALOG_INIT:
            pMe->m_bFirstShowDlg = TRUE;
            pMe->m_bSelNotChanged = TRUE;

            IMENUCTL_SetProperties(popMenu, MP_WRAPSCROLL|MP_TRANSPARENT_UNSEL|MP_NO_REDRAW);
            IMENUCTL_SetOemProperties(popMenu, OEMMP_GRAPHIC_UNDERLINE);
            
            CameraApp_InitpopMenu(pMe, popMenu);             
            return TRUE;
     
        case EVT_DIALOG_START:                   
            ISHELL_PostEvent(pMe->m_pShell, AEECLSID_APP_CAMERA, EVT_USER_REDRAW, NULL, NULL); 
            return TRUE;
         
        case EVT_DIALOG_END:
#ifndef FEATURE_CAMERAAPP_FUN_FRAME
            if(pMe->m_nCameraCFG == CAMERACFGFUN)
            {
                CameraApp_HandlePopMenuEffectCFG(pMe, popMenu, FALSE);
            }
#endif
            return TRUE;
      
        case EVT_USER_REDRAW:
            if(pMe->m_bFirstShowDlg == TRUE)
            {                           
    			if(pMe->m_bRePreview)
                {
                    ICAMERA_Preview(pMe->m_pCamera);
                    pMe->m_nCameraState = CAM_PREVIEW;
                    CameraApp_SetParamAfterPreview(pMe);
                    pMe->m_bRePreview = FALSE;
                }
                      
                IDISPLAY_SetClipRect(pMe->m_pDisplay, 0);

                IDISPLAY_ClearScreen(pMe->m_pDisplay);

                if(pMe->m_nCameraSize != OEMNV_CAMERA_SIZE_176_220)
                {
#ifdef FEATURE_CARRIER_CHINA_VERTU
                    CameraApp_DrawBarBg(pMe, IDI_TOPBAR_BG_VERTU, 0, 0);
#else                
                    CameraApp_DrawBarBg(pMe, IDI_TOPBAR_BG, 0, 0);
#endif

                    DRAW_BOTTOMBAR(BTBAR_SELECT_BACK);
                }
                else
                {
                    CameraApp_SetCameraFunFrame(pMe);
                    
                    CameraApp_DrawBottomBarText(pMe, BTBAR_SELECT_BACK);           
                }

                CameraApp_DrawCCFGBar(pMe);
                     
                if(!pMe->m_bCameraHideIcon)
                {
                    CameraApp_DrawTopBar(pMe);   
                }
                    
                CamreaApp_DrawCFGPromptText(pMe);                        

                IMENUCTL_Redraw(popMenu);

                pMe->m_bFirstShowDlg = FALSE;
            }
            else
            {
                if(pMe->m_nCameraSize == OEMNV_CAMERA_SIZE_176_220)
                {
                    IDISPLAY_SetClipRect(pMe->m_pDisplay, &eRec);
                
                    IDisplay_EraseRect(pMe->m_pDisplay, &eRec);

                    CameraApp_SetCameraFunFrame(pMe);
                }
                else
                {
                    IDISPLAY_SetClipRect(pMe->m_pDisplay, 0);
                
                    IDisplay_EraseRect(pMe->m_pDisplay, &eRec);
                }

                CameraApp_DrawCCFGBar(pMe);

                CamreaApp_DrawCFGPromptText(pMe); 

                IMENUCTL_Redraw(popMenu);

                if(pMe->m_nCameraSize == OEMNV_CAMERA_SIZE_176_220)
                {
                    IDISPLAY_SetClipRect(pMe->m_pDisplay, 0);
                }
            }        
            return TRUE;

        case EVT_KEY:
        case EVT_KEY_RELEASE:
            return TRUE;
            
        case EVT_KEY_PRESS:
            switch(wParam)
            {
                case AVK_CLR:
                    CLOSE_DIALOG(DLGRET_CANCELED);
                    return TRUE;

                case AVK_LEFT:   
                    if(pMe->m_nCameraCFG == CAMERACFGCHOOSE)
                    {
                        pMe->m_nCameraCFG = CAMERACFGHOTKEY;
                    }
                    else
                    {
#ifndef FEATURE_CAMERAAPP_FUN_FRAME
                        if(pMe->m_nCameraCFG == CAMERACFGFUN)
                        {
                            CameraApp_HandlePopMenuEffectCFG(pMe, popMenu, FALSE);  
                        }                      
#endif
                        pMe->m_nCameraCFG--;
                    }

                    if(pMe->m_nCameraCFG == CAMERACFGHOTKEY)
                    {
                        pMe->m_nCFGBarChoosedID = 5;
                        pMe->m_nCFGBarOffset = 6;
                    }
                    else
                    {
                        if(pMe->m_nCameraCFG == CAMERACFGCHOOSE)
                        {
                            pMe->m_nCFGBarChoosedID--;
                        }
                        else
                        {
                            if(pMe->m_nCFGBarChoosedID != 1)
                            {
                                pMe->m_nCFGBarChoosedID--;
                            }
                            else
                            {
                                pMe->m_nCFGBarOffset--;
                            }
                        }
                    }
                    break;
   
                case AVK_RIGHT:
                    if(pMe->m_nCameraCFG == CAMERACFGHOTKEY)
                    {
                        pMe->m_nCameraCFG = CAMERACFGCHOOSE;
                    }
                    else
                    {
#ifndef FEATURE_CAMERAAPP_FUN_FRAME
                        if(pMe->m_nCameraCFG == CAMERACFGFUN)
                        {
                            CameraApp_HandlePopMenuEffectCFG(pMe, popMenu, FALSE);  
                        }                      
#endif
                        pMe->m_nCameraCFG++;
                    }
                    
                    if(pMe->m_nCameraCFG == CAMERACFGCHOOSE)
                    {
                        pMe->m_nCFGBarChoosedID = 0;
                        pMe->m_nCFGBarOffset = 0;
                    }
                    else
                    {
                        if(pMe->m_nCameraCFG == CAMERACFGHOTKEY)
                        {
                            pMe->m_nCFGBarChoosedID++;
                        }
                        else
                        {
                            if(pMe->m_nCFGBarChoosedID != 4)
                            {
                                pMe->m_nCFGBarChoosedID++;
                            }
                            else
                            {
                                pMe->m_nCFGBarOffset++;
                            }
                        }
                    }

                    break;

                case AVK_UP:
                case AVK_DOWN:                 
                    pMe->m_bSelNotChanged = FALSE;
                    return TRUE;

                case AVK_END:
                    ICAMERA_Stop(pMe->m_pCamera);
                    pMe->m_nCameraState = CAM_STOP;
                    return FALSE;
                    
                default:
                    return TRUE;
            }
            
            //���Ҽ��л���ͬ�����ò˵�����Ҫ���³�ʼ���˵���
            CameraApp_InitpopMenu(pMe, popMenu);

            return ISHELL_SendEvent(pMe->m_pShell, AEECLSID_APP_CAMERA, EVT_USER_REDRAW, NULL, NULL);

        case EVT_CTL_SEL_CHANGED:
            //��һ������Dlg���������Sel�иı䣬�ᴥ��EVT_CTL_SEL_CHANGED�¼�
            if(pMe->m_bSelNotChanged)
            {
                return TRUE;
            }
            else
            {      
                AEERect popMenuRec;
        
                IMENUCTL_GetRect(popMenu, &popMenuRec);
    
                IDISPLAY_SetClipRect(pMe->m_pDisplay, &popMenuRec);
       
                IDISPLAY_ClearScreen(pMe->m_pDisplay);
       
                //CameraApp_DrawpopMenuBg(pMe, popMenu);
        
                IMENUCTL_Redraw(popMenu);
                
#ifndef FEATURE_CAMERAAPP_FUN_FRAME
                //ѡ����������ͬ�Ĳ˵���ʱ����ʱ��ӦCamera��ͬ����Ч����
                if(pMe->m_nCameraCFG == CAMERACFGFUN)
                {
                    CameraApp_HandlePopMenuEffectCFG(pMe, popMenu, TRUE);
                }
#endif      
                return TRUE;
            }
            
        case EVT_COMMAND:
            return CameraApp_RoutePopMenuCommandEvent(pMe, wParam);
  
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
            ISHELL_PostEvent(pMe->m_pShell, AEECLSID_APP_CAMERA, EVT_USER_REDRAW, NULL, NULL);
            return TRUE;
 
        case EVT_USER_REDRAW:
            IDISPLAY_SetClipRect(pMe->m_pDisplay,0);
            CameraApp_HandleSnapshotPic(pMe);                     
            return TRUE;
 
        case EVT_KEY:
            #ifdef FEATURE_SUPPORT_VC0848
            // jpg ����δ��ɣ���ֹ�����¼�
            if(pMe->m_nCameraStorage == OEMNV_CAMERA_STORAGE_MEMORY_CARD)
            {
                extern vc_jpeg_decode display_jpg_done;
                if(display_jpg_done == VC_JPEG_DECODE_DOING)
                {
                    return TRUE;
                }                               
            }
            else
            {
                if(!jpg_decoder_done)
                {
                    return TRUE;
                }
            }
            #endif
            MEMSET(pMe->m_sCurrentFileName, '\0', STRLEN(pMe->m_sCurrentFileName));
        
            if(pMe->m_nCameraStorage == OEMNV_CAMERA_STORAGE_MEMORY_CARD)
            {
                STRCPY(pMe->m_sCurrentFileName, AEEFS_CARD0_DIR);
                STRCPY(pMe->m_sCurrentFileName+STRLEN(pMe->m_sCurrentFileName),pMe->m_sFileNameWithAllPath);
            }
            else
            {
                STRCPY(pMe->m_sCurrentFileName, pMe->m_sFileNameWithAllPath);
            }           
        
            DBGPRINTF("pMe->m_sCurrentFileName = %s", pMe->m_sCurrentFileName);
                    
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
       CameraApp_FunHandleEvent
˵����
       IDD_CFUN�Ի����¼�������
       
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
static boolean CameraApp_FunHandleEvent(CCameraApp *pMe, AEEEvent eCode, uint16 wParam, uint32 dwParam)
{
    if(pMe->m_bfunFrameSelected)
    {
        return CameraApp_CFunFrameEffectHandle(pMe, eCode, wParam, dwParam); 
    }
    else
    {
        return CameraApp_CFunColorEffectHandle(pMe, eCode, wParam, dwParam); 
    }
}

/*==============================================================================
������
       CameraApp_CFunFrameEffectHandle
˵����
       IDD_CFUN�Ի����¼���Frame������
       
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
static boolean CameraApp_CFunFrameEffectHandle(CCameraApp *pMe, AEEEvent eCode, uint16 wParam, int dwParam)
{
    switch(eCode)
    {
        case EVT_DIALOG_INIT:         
            return TRUE;

        case EVT_DIALOG_START:
            (void)ICONFIG_GetItem(pMe->m_pConfig,
                                  CFGI_CAMERA_FRAME,
                                  &pMe->m_nCameraFrame,
                                  sizeof(pMe->m_nCameraFrame));                             
           
            return ISHELL_PostEvent(pMe->m_pShell, AEECLSID_APP_CAMERA, EVT_USER_REDRAW, NULL, NULL); 

        case EVT_DIALOG_END:           
            return TRUE;

        case EVT_USER_REDRAW:   
            IDISPLAY_SetClipRect(pMe->m_pDisplay, 0);
            IDisplay_ClearScreen(pMe->m_pDisplay);
            
            //draw top bar         
            CameraApp_SetCameraFunFrame(pMe);           
            CameraApp_DrawBarText(pMe, IDS_FUN_FRAME, TOPBAR_TITLE_TEXT_Y);  
  
            //draw cfg bar          
            CameraApp_DrawBarBg(pMe, IDI_LEFT_ARROW, 0, CFGBAR_EFFECT_TEXT_Y);
            CameraApp_DrawBarBg(pMe, IDI_RIGHT_ARROW, 168, CFGBAR_EFFECT_TEXT_Y);
            
            CameraApp_DrawFramePromptText(pMe);

            //draw bottom bar
            CameraApp_DrawBottomBarText(pMe, BTBAR_SELECT_BACK);
            
            IDISPLAY_UpdateEx(pMe->m_pDisplay, FALSE);
            return TRUE;
            
        case EVT_KEY_PRESS:
            switch(wParam)
            {
                case AVK_LEFT:
                    if(pMe->m_nCameraFrame == OEMNV_CAMERA_FRAME_0)
                    {
                        pMe->m_nCameraFrame = OEMNV_CAMERA_FRAME_8;
                    }
                    else
                    {
                        pMe->m_nCameraFrame--;
                    }
                    
                    break;

                case AVK_RIGHT:
                    if(pMe->m_nCameraFrame == OEMNV_CAMERA_FRAME_8)
                    {
                        pMe->m_nCameraFrame = OEMNV_CAMERA_FRAME_0;
                    }
                    else
                    {
                        pMe->m_nCameraFrame++;
                    } 
                    
                    break;

                case AVK_SELECT:
                case AVK_INFO:
                    (void)ICONFIG_SetItem(pMe->m_pConfig,
                                          CFGI_CAMERA_FRAME,
                                          &pMe->m_nCameraFrame,
                                          sizeof(pMe->m_nCameraFrame));
                    CLOSE_DIALOG(DLGRET_PREVIEW);
                    return TRUE;

                case AVK_CLR:
                    CLOSE_DIALOG(DLGRET_CANCELED);
                    return TRUE;

                case AVK_END:
                    ICAMERA_Stop(pMe->m_pCamera);
                    pMe->m_nCameraState = CAM_STOP;
                    return FALSE;

                default:
                    return TRUE;
            }             
            return ISHELL_SendEvent(pMe->m_pShell, AEECLSID_APP_CAMERA, EVT_USER_REDRAW, NULL, NULL); 
 
        default:
            break;  
    }

    return FALSE;
}

/*==============================================================================
������
       CameraApp_CFunFrameEffectHandle
˵����
       IDD_CFUN�Ի����¼���Color������
       
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
static boolean CameraApp_CFunColorEffectHandle(CCameraApp *pMe, AEEEvent eCode, uint16 wParam, int dwParam)
{ 
    switch(eCode)
    {
        case EVT_DIALOG_INIT:              
            switch(pMe->m_ePreState)
            {
                case STATE_CCAMERACFG:
                    (void)ICONFIG_GetItem(pMe->m_pConfig,
                                          CFGI_CAMERA_COLOR,
                                          &pMe->m_nCameraColor,
                                          sizeof(pMe->m_nCameraColor));
                    break;

                case STATE_CVIDEOCFG:
                    (void)ICONFIG_GetItem(pMe->m_pConfig,
                                          CFGI_VIDEO_COLOR,
                                          &pMe->m_nCameraColor,
                                          sizeof(pMe->m_nCameraColor));
                    break;

                default:
                    break;
            }

            pMe->m_nColorSel = CameraApp_ColorEffectSel(pMe);
            
            return TRUE;

        case EVT_DIALOG_START:          
            ISHELL_PostEvent(pMe->m_pShell, 
                             AEECLSID_APP_CAMERA, 
                             EVT_USER_REDRAW, 
                             NULL, 
                             NULL);
            
            return TRUE;

        case EVT_DIALOG_END:           
            return TRUE;

        case EVT_USER_REDRAW:
            IDISPLAY_SetClipRect(pMe->m_pDisplay, 0);
            
            if((pMe->m_nCameraSize != OEMNV_CAMERA_SIZE_176_220) || (pMe->m_ePreState == STATE_CVIDEOCFG))
            {
#ifdef FEATURE_CARRIER_CHINA_VERTU
                //draw top bar bg
                CameraApp_DrawBarBg(pMe, IDI_TOPBAR_BG_VERTU, 0, 0);

                //draw cfg bar bg
                CameraApp_DrawBarBg(pMe, IDI_SCROLLBAR_BG_VERTU, 0, 160);
#else            
                //draw top bar bg
                CameraApp_DrawBarBg(pMe, IDI_TOPBAR_BG, 0, 0);

                //draw cfg bar bg
                CameraApp_DrawBarBg(pMe, IDI_SCROLLBAR_BG, 0, 160);
#endif
                //draw bottom bar
                DRAW_BOTTOMBAR(BTBAR_SELECT_BACK);
            }
            else
            {
                IDisplay_ClearScreen(pMe->m_pDisplay);

                CameraApp_SetCameraFunFrame(pMe);

                //draw left yellow arrow
                CameraApp_DrawBarBg(pMe, IDI_LEFT_ARROW, 0, CFGBAR_EFFECT_TEXT_Y);

                //draw right yellow arrow
                CameraApp_DrawBarBg(pMe, IDI_RIGHT_ARROW, 168, CFGBAR_EFFECT_TEXT_Y);

                //draw bootom bar text
                CameraApp_DrawBottomBarText(pMe, BTBAR_SELECT_BACK);
            }
            
            //draw top bar bar text
            CameraApp_DrawBarText(pMe, IDS_FUN_COLOR, TOPBAR_TITLE_TEXT_Y);  

            //draw cfg bar text
            CameraApp_DrawColorPromptText(pMe);
    
            IDISPLAY_UpdateEx(pMe->m_pDisplay, FALSE);
            return TRUE;
            
        case EVT_KEY_PRESS:
            switch(wParam)
            {
                case AVK_LEFT:                
                    if(pMe->m_nColorSel == COLORNORMAL)
                    {
                        pMe->m_nColorSel = COLORGREEN;
                    }
                    else
                    {
                        pMe->m_nColorSel--;
                    }
                    
                    break;

                case AVK_RIGHT:
                    if(pMe->m_nColorSel == COLORGREEN)
                    {
                        pMe->m_nColorSel = COLORNORMAL;
                    }
                    else
                    {
                        pMe->m_nColorSel++;
                    } 
                    
                    break;

                case AVK_CLR:
                    CameraApp_HandleColorEffectCFG(pMe);
                    CLOSE_DIALOG(DLGRET_CANCELED);
                    return TRUE;
                    
                case AVK_SELECT:
                case AVK_INFO:
                    CameraApp_SetEffectColor(pMe, pMe->m_nCameraColor);
                    CameraApp_HandleColorEffectCFG(pMe);
                    if(STATE_CCAMERACFG == pMe->m_ePreState)
                    {
                    CLOSE_DIALOG(DLGRET_PREVIEW);
                    }
		           else if(STATE_CVIDEOCFG == pMe->m_ePreState)
		           {
		      	   CLOSE_DIALOG(DLGRET_VIDEO);
		           }
                    return TRUE;

                case AVK_END:
                    ICAMERA_Stop(pMe->m_pCamera);
                    pMe->m_nCameraState = CAM_STOP;
                    return FALSE;
                    
                default:
                    return TRUE;
            }

            pMe->m_nCameraColor = CameraApp_SetColorEffect(pMe);
            
            ICAMERA_SetParm(pMe->m_pCamera, CAM_PARM_EFFECT, pMe->m_nCameraColor, 0);
            
            return ISHELL_SendEvent(pMe->m_pShell, 
                                    AEECLSID_APP_CAMERA, 
                                    EVT_USER_REDRAW, 
                                    NULL, 
                                    NULL); 
 
        default:
            break;
    }

    return FALSE;
}

/*==============================================================================
������
       CameraApp_VideoHandleEvent
˵����
       IDD_CVIDEO�Ի����¼�������
       
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
static boolean CameraApp_VideoHandleEvent(CCameraApp *pMe, AEEEvent eCode, uint16 wParam, uint32 dwParam)
{  
    AEERect rectTime,rectStatic;
    
    switch(eCode) 
    {
        case EVT_DIALOG_INIT:  
            pMe->m_nCameraStorage = OEMNV_CAMERA_STORAGE_MEMORY_CARD;
            
            IDISPLAY_SetClipRect(pMe->m_pDisplay, 0);
            
            SETAEERECT(&rectTime, 5, 160, 68, 17);    
            
            ITIMECTL_SetRect(pMe->m_pVideoRecTimeCtl, &rectTime);                
#ifdef FEATURE_CARRIER_CHINA_VERTU
            ITIMECTL_SetBgColor(pMe->m_pVideoRecTimeCtl, MAKE_RGB(0x10, 0x20, 0x3A));//added by chengxiao 2009.04.02
#endif			
            
            //����ʱ��ؼ�������
            ITIMECTL_SetFont(pMe->m_pVideoRecTimeCtl, AEE_FONT_NORMAL, AEE_FONT_NORMAL);
            
            pMe->m_dwDispTime = 0;
            
            ITIMECTL_SetProperties(pMe->m_pVideoRecTimeCtl, TP_NO_MSECONDS|TP_LEFT_JUSTIFY);
            
            SETAEERECT(&rectStatic, 30, 74, 116, 40);
            
            ISTATIC_SetRect(pMe->m_pStatic, &rectStatic);

            ISTATIC_SetProperties(pMe->m_pStatic, ST_MIDDLETEXT|ST_CENTERTEXT|ST_TRANSPARENTBACK);  

            (void)ICONFIG_GetItem(pMe->m_pConfig,
                                  CFGI_VIDEO_ICON,
                                  &pMe->m_bVideoHideIcon,
                                  sizeof(pMe->m_bVideoHideIcon));

            (void)ICONFIG_GetItem(pMe->m_pConfig,
                                  CFGI_VIDEO_BRIGHTNESS,
                                  &pMe->m_nCameraBrightness,
                                  sizeof(pMe->m_nCameraBrightness));
            
            (void)ICONFIG_GetItem(pMe->m_pConfig,
                                  CFGI_VIDEO_COLOR,
                                  &pMe->m_nCameraColor,
                                  sizeof(pMe->m_nCameraColor));

            (void)ICONFIG_GetItem(pMe->m_pConfig,
                                  CFGI_VIDEO_ENVIROMENT,
                                  &pMe->m_nCameraEnviroment,
                                  sizeof(pMe->m_nCameraEnviroment));

            return TRUE;
     
        case EVT_DIALOG_START:
            pMe->m_bCanCapture = CameraApp_IsEnoughfMemorySpace(pMe);
            
            ISHELL_PostEvent(pMe->m_pShell, AEECLSID_APP_CAMERA, EVT_USER_REDRAW, NULL, NULL);  
            return TRUE;
         
        case EVT_DIALOG_END:      
            ISHELL_CancelTimer(pMe->m_pShell, NULL, pMe);
            return TRUE;
      
        case EVT_USER_REDRAW:
            // ��ֹ���ٰ��������趨�Ķ�ʱ��δȡ��            
            ISHELL_CancelTimer(pMe->m_pShell, (PFNNOTIFY)CameraApp_HotKeyDisplayTimeout, pMe);
            
            if(pMe->m_pCamera && (!pMe->m_bIsPreview))
            {                
                AEESize fsize,psize;

                fsize.cx=176;
                fsize.cy=144;
                psize.cx = 176;
                psize.cy = 132;

                ICAMERA_SetParm(pMe->m_pCamera, CAM_PARM_PREVIEW_TYPE, CAM_PREVIEW_MOVIE, 0);
               
                ICAMERA_SetDisplaySize(pMe->m_pCamera, &psize);
                
                ICAMERA_SetSize(pMe->m_pCamera, &fsize);

                ICAMERA_SetParm(pMe->m_pCamera, CAM_PARM_EFFECT, pMe->m_nCameraColor, 0);
            
                ICAMERA_Preview(pMe->m_pCamera);

                pMe->m_nCameraState = CAM_PREVIEW;

                CameraApp_SetParamAfterPreview(pMe);

                pMe->m_bIsPreview = TRUE;
            }

            if(pMe->m_bRePreview)
            {
                ICAMERA_Preview(pMe->m_pCamera);
                pMe->m_nCameraState = CAM_PREVIEW;
                CameraApp_SetParamAfterPreview(pMe);
                pMe->m_bRePreview = FALSE;
            }
#ifdef FEATURE_CARRIER_CHINA_VERTU
            CameraApp_DrawBarBg(pMe, IDI_TOPBAR_BG_VERTU, 0, 0);
#else                
            CameraApp_DrawBarBg(pMe, IDI_TOPBAR_BG, 0, 0);
#endif
            if(!pMe->m_bVideoHideIcon)
            {
                CameraApp_DrawVideoTopBar(pMe);
            }

#ifdef FEATURE_CARRIER_CHINA_VERTU
            CameraApp_DrawBarBg(pMe, IDI_CFG_BG_VERTU, 0, 160);
#else            
            CameraApp_DrawBarBg(pMe, IDI_CFG_BG, 0, 160);
#endif

            ITIMECTL_Redraw(pMe->m_pVideoRecTimeCtl); 

            if(pMe->m_bVideoRecStart)
            {
                DRAW_BOTTOMBAR(BTBAR_CANCEL)
            }
            else
            {
                DRAW_BOTTOMBAR(BTBAR_OPTION_BACK);
            }

            CameraApp_DrawMidPic(pMe);          

            (void)ITIMECTL_SetTimeEx(pMe->m_pVideoRecTimeCtl, (int32)pMe->m_dwDispTime, TRUE);                 
       
            return TRUE;

        case EVT_KEY:
            switch(wParam)
            {
                case AVK_SELECT: 
                    if(!pMe->m_bVideoRecStart)
                    {
                        CLOSE_DIALOG(DLGRET_VIDEOCFG);
                    }
                    break;

                default:
                    break;
            }
            return TRUE;
            
        case EVT_KEY_RELEASE:
            return TRUE;
            
        case EVT_KEY_PRESS:
            if(pMe->m_bVideoRecStart && 
               !((wParam == AVK_INFO) || 
               (wParam == AVK_CLR) || 
               (wParam == AVK_END)))
            {
                return TRUE;
            }
            
            switch(wParam)
            {
                case AVK_2:
                    return CameraApp_HotKeyAVK_2_ChooseHandleEvent(pMe, eCode, wParam, dwParam);
                   
                case AVK_5:
                    return CameraApp_HotKeyAVK_5_EnvironmentHandleEvent(pMe, eCode, wParam, dwParam);
                    
                case AVK_7:
                    return CameraApp_HotKeyAVK_7_ColorHandleEvent(pMe, eCode, wParam, dwParam);

                case AVK_POUND:
                    return CameraApp_HotKeyAVK_POUND_HideIconHandleEvent(pMe, eCode, wParam, dwParam);
  
                case AVK_CLR:
                    if(pMe->m_bVideoRecStart)
                    {   
                        CameraApp_VideoStop(pMe);

                        pMe->m_dwDispTime = 0;
                        
                        ICAMERA_Stop(pMe->m_pCamera);

                        pMe->m_nCameraState = CAM_STOP;

                        pMe->m_bIsPreview = FALSE;                      
                        
                        if(pMe->m_pFileMgr != NULL)
                        {
                            IFILEMGR_Remove(pMe->m_pFileMgr, pMe->m_sCurrentFileName);
                        }

                        pMe->m_bVideoRecStart = FALSE;
                            
                        ISHELL_PostEvent(pMe->m_pShell, AEECLSID_APP_CAMERA, EVT_USER_REDRAW, NULL, NULL);  
                    }
                    else
                    {
                        ICAMERA_Stop(pMe->m_pCamera);                       
                        pMe->m_nCameraState = CAM_STOP;
                        pMe->m_bIsPreview = FALSE;
                        CLOSE_DIALOG(DLGRET_CANCELED);
                    }
                    break;

                case AVK_INFO: 
                    if(!pMe->m_bCanCapture)
                    {
                        pMe->m_wMsgID = IDS_MSG_NOMEMORY;
                        pMe->m_nMsgTimeout = TIMEOUT_MS_MSGBOX;
                        ICAMERA_Stop(pMe->m_pCamera);
                        pMe->m_bIsPreview = FALSE;
                        CLOSE_DIALOG(DLGRET_POPMSG);
                        return TRUE;
                    }
                    
                    if(!pMe->m_bVideoRecStart)
                    { 
                        //��ֹ���ٰ���������hotkey Text������LCD��
                        CameraApp_HotKeyDisplayTimeout(pMe);                 
                        
                        MEMSET(pMe->m_sCurrentFileName, '\0', sizeof(pMe->m_sCurrentFileName));
                        CameraApp_GetDateForRecordFileName(pMe, pMe->m_sCurrentFileName, FILE_TYPE_3GP);
                        CameraApp_SetDateForRecordFileName(pMe);
                        
                        pMe->m_bVideoRecStart = TRUE;  
                        
                        ISHELL_SendEvent(pMe->m_pShell, 
                                         AEECLSID_APP_CAMERA, 
                                         EVT_USER_REDRAW, 
                                         NULL, 
                                         NULL);                  
                        
                        pMe->m_nTicks = ISHELL_GetTimeMS(pMe->m_pShell) - pMe->m_dwDispTime; 
#ifdef FEATURE_SUPPORT_VC0848
                        VC_SetVideoRecorderDoneState(FALSE); 
                        #endif
                        CameraApp_VideoRunning(pMe);

                        ICAMERA_RecordMovie(pMe->m_pCamera); 
                        
                        pMe->m_nCameraState = CAM_RECORD;                                   
                    }
                    else
                    {   
                        pMe->m_bVideoRecStart = FALSE;
                        CameraApp_VideoStop(pMe);
                        ICAMERA_Stop(pMe->m_pCamera);
                        pMe->m_nCameraState = CAM_STOP;
                        
                        CLOSE_DIALOG(DLGRET_VIDEOSAVE);
                    }
                    break;
             
                case AVK_LEFT:
                    //��ֹ���ٰ���������hotkey Text������LCD��
                    CameraApp_HotKeyDisplayTimeout(pMe);
                    
                    if(pMe->m_nCameraBrightness != OEMNV_CAMERA_BRIGHTNESS_LEVEL1)
                    {
                        pMe->m_nCameraBrightness--;
                    }
                    else
                    {
                        return TRUE;
                    }

                    ICAMERA_SetBrightness(pMe->m_pCamera, pMe->m_nCameraBrightness);
                    
                    (void)ICONFIG_SetItem(pMe->m_pConfig,
                                          CFGI_VIDEO_BRIGHTNESS,
                                          &pMe->m_nCameraBrightness,
                                          sizeof(pMe->m_nCameraBrightness));
                    
                    return ISHELL_SendEvent(pMe->m_pShell, 
                                            AEECLSID_APP_CAMERA, 
                                            EVT_USER_REDRAW, 
                                            NULL, 
                                            NULL);

                case AVK_RIGHT:
                    //��ֹ���ٰ���������hotkey Text������LCD��
                    CameraApp_HotKeyDisplayTimeout(pMe);
                    
                    if(pMe->m_nCameraBrightness != OEMNV_CAMERA_BRIGHTNESS_LEVEL5)
                    {
                        pMe->m_nCameraBrightness++;
                    }
                    else
                    {
                        return TRUE;
                    }

                    ICAMERA_SetBrightness(pMe->m_pCamera, pMe->m_nCameraBrightness);
                    
                    (void)ICONFIG_SetItem(pMe->m_pConfig,
                                          CFGI_VIDEO_BRIGHTNESS,
                                          &pMe->m_nCameraBrightness,
                                          sizeof(pMe->m_nCameraBrightness));
                    
                    return ISHELL_SendEvent(pMe->m_pShell, 
                                            AEECLSID_APP_CAMERA, 
                                            EVT_USER_REDRAW, 
                                            NULL, 
                                            NULL);

                case AVK_END:
                    ICAMERA_Stop(pMe->m_pCamera); 
                    
                    pMe->m_nCameraState = CAM_STOP;
                    
                    if((pMe->m_pFileMgr != NULL) && pMe->m_bVideoRecStart)
                    {
                        IFILEMGR_Remove(pMe->m_pFileMgr, pMe->m_sCurrentFileName);
                        
                        pMe->m_bVideoRecStart = FALSE;
                    }
                    
                    return FALSE;

                default:
                    break;
            }
            
            return TRUE;
            
        default:
            break; 
    }
    
    return FALSE;
}

/*==============================================================================
������
       CameraApp_VideoCFGHandleEvent
˵����
       IDD_CVIDEOCFG�Ի����¼�������
       
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
static boolean CameraApp_VideoCFGHandleEvent(CCameraApp *pMe, AEEEvent eCode, uint16 wParam, uint32 dwParam)
{  
    AEERect eRec;
    
    IMenuCtl *popMenu = (IMenuCtl*)IDIALOG_GetControl(pMe->m_pActiveDlg, IDC_VIDEO_CFGMENU);
        
    if(popMenu == NULL)
    {
        return FALSE;
    }

    SETAEERECT(&eRec, 0, 72, 176, 126);
                 
    switch(eCode) 
    {
        case EVT_DIALOG_INIT:
            pMe->m_bFirstShowDlg = TRUE;
            pMe->m_bSelNotChanged = TRUE;

            IMENUCTL_SetProperties(popMenu, MP_WRAPSCROLL|MP_TRANSPARENT_UNSEL|MP_NO_REDRAW);
            IMENUCTL_SetOemProperties(popMenu, OEMMP_GRAPHIC_UNDERLINE);
            
            CameraApp_InitpopMenu(pMe, popMenu);                     
            return TRUE;
     
        case EVT_DIALOG_START:                  
            ISHELL_PostEvent(pMe->m_pShell, AEECLSID_APP_CAMERA, EVT_USER_REDRAW, NULL, NULL);  
            return TRUE;
         
        case EVT_DIALOG_END:
            return TRUE;
      
        case EVT_USER_REDRAW:
            if(pMe->m_bFirstShowDlg == TRUE)
            {              
                if(pMe->m_bRePreview)
                {
                    ICAMERA_Preview(pMe->m_pCamera);
                    pMe->m_nCameraState = CAM_PREVIEW;
                    CameraApp_SetParamAfterPreview(pMe);
                    pMe->m_bRePreview = FALSE;
                }
                
                IDISPLAY_SetClipRect(pMe->m_pDisplay, 0);

                IDISPLAY_ClearScreen(pMe->m_pDisplay);

#ifdef FEATURE_CARRIER_CHINA_VERTU
               CameraApp_DrawBarBg(pMe, IDI_TOPBAR_BG_VERTU, 0, 0);
#else
                CameraApp_DrawBarBg(pMe, IDI_TOPBAR_BG, 0, 0);
#endif

                if(!pMe->m_bVideoHideIcon)
                {
                    CameraApp_DrawVideoTopBar(pMe);   
                }                                       
                
                DRAW_BOTTOMBAR(BTBAR_SELECT_BACK);
           
                CameraApp_DrawVCFGBar(pMe);
                    
                CamreaApp_DrawCFGPromptText(pMe);            
                
                IMENUCTL_Redraw(popMenu);

                 pMe->m_bFirstShowDlg = FALSE;
            }
            else
            {
                IDISPLAY_SetClipRect(pMe->m_pDisplay, 0);
                
                IDisplay_EraseRect(pMe->m_pDisplay, &eRec);

                CameraApp_DrawVCFGBar(pMe);
                    
                CamreaApp_DrawCFGPromptText(pMe);            
                
                IMENUCTL_Redraw(popMenu);
            }        
            return TRUE;

        case EVT_KEY:
        case EVT_KEY_RELEASE:
            return TRUE;
            
        case EVT_KEY_PRESS:
            switch(wParam)
            {
                case AVK_CLR:
                    CLOSE_DIALOG(DLGRET_CANCELED);
                    return TRUE;

                case AVK_LEFT: 
                    if(pMe->m_nVideoCFG == VIDEOCFGCHOOSE)
                    {
                        pMe->m_nVideoCFG = VIDEOCFGHOTKEY;
                    }
                    else
                    {
                        pMe->m_nVideoCFG--;
                    }                  
                    break;
                    
                case AVK_RIGHT:
                    if(pMe->m_nVideoCFG == VIDEOCFGHOTKEY)
                    {
                        pMe->m_nVideoCFG = VIDEOCFGCHOOSE;
                    }
                    else
                    {
                        pMe->m_nVideoCFG++;
                    } 
                    break;

                case AVK_UP:
                case AVK_DOWN:
                    pMe->m_bSelNotChanged = FALSE;
                    return TRUE;

                case AVK_END:
                    ICAMERA_Stop(pMe->m_pCamera);
                    pMe->m_nCameraState = CAM_STOP;
                    return FALSE;
                    
                default:
                    return TRUE;
            }

            //���Ҽ��л���ͬ�����ò˵�����Ҫ���³�ʼ���˵���
            CameraApp_InitpopMenu(pMe, popMenu);
            
            return ISHELL_SendEvent(pMe->m_pShell, AEECLSID_APP_CAMERA, EVT_USER_REDRAW, NULL, NULL); 
            
        case EVT_CTL_SEL_CHANGED:
            if(pMe->m_bSelNotChanged)
            {                
                return TRUE;
            }         
            else
            {  

                AEERect popMenuRec;  
            
                SETAEERECT(&popMenuRec, 0, 72, 176, 88);          
                 
                IDISPLAY_SetClipRect(pMe->m_pDisplay, &popMenuRec);       
          
                IMENUCTL_Redraw(popMenu);

                return TRUE;
            }
            
        case EVT_COMMAND:
            return CameraApp_RoutePopMenuCommandEvent(pMe, wParam);
  
        default:
            break;
    }
    return FALSE;
}

/*==============================================================================
������
       CameraApp_VideoSaveHandleEvent
˵����
       IDD_CVIDEOREC�Ի����¼�������
       
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
static boolean CameraApp_VideoSaveHandleEvent(CCameraApp *pMe, AEEEvent eCode, uint16 wParam, uint32 dwParam)
{      
    AEERect topRect,bottomRect;
    
    SETAEERECT(&topRect, 0, 0, 176, 28);
    SETAEERECT(&bottomRect, 0, 160, 176, 60);
    
    switch(eCode)
    {
        case EVT_DIALOG_INIT:  
            return TRUE;

        case EVT_DIALOG_START:
            ISHELL_PostEvent(pMe->m_pShell, AEECLSID_APP_CAMERA, EVT_USER_REDRAW, NULL, NULL);
            return TRUE;

        case EVT_DIALOG_END:
            return TRUE;

        case EVT_USER_REDRAW:
            CameraApp_HandleVideoFile(pMe);
            return TRUE;

        case EVT_KEY:
            DBGPRINTF("pMe->m_sCurrentFileName = %s", pMe->m_sCurrentFileName);
            
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

        default:
            break; 
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

    switch (eCode)
    {
        case EVT_DIALOG_INIT: 
            return TRUE;
        
        case EVT_DIALOG_START:
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
   
            return TRUE;                                                                       
            
        case EVT_USER_REDRAW:
            if((pMe->m_ePreState == STATE_CPIC) || (pMe->m_ePreState == STATE_CVIDEOSAVE))
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
            
            return TRUE;
            
        case EVT_DIALOG_END:
            ISTATIC_Release(pStatic); 
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
                if(pMe->m_nMainMenuItemSel == IDS_ITEM_CAMERA)
                {
                    CLOSE_DIALOG(DLGRET_PREVIEW);
                }
                else
                {
                    CLOSE_DIALOG(DLGRET_VIDEO);
                }                      
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
            case CAMERACFGCHOOSE:
                return CameraApp_PopMenu_ChooseCommandHandleEvent(pMe, wParam);
    
            case CAMERACFGENVIRMENT:
                return CameraApp_PopMenu_EnvironmentCommandHandleEvent(pMe, wParam);
    
            case CAMERACFGFUN:
                return CameraApp_PopMenu_FunCommandHandleEvent(pMe, wParam);
   
            case CAMERACFGQUALITY:
                return CameraApp_PopMenu_QualityCommandHandleEvent(pMe, wParam);
   
            case CAMERACFGSIZE:
                return CameraApp_PopMenu_SizeCommandHandleEvent(pMe, wParam);
   
            case CAMERACFGTONE:
                return CameraApp_PopMenu_ShutterToneCommandHandleEvent(pMe, wParam);
   
            case CAMERACFGSELFTIME:
                return CameraApp_PopMenu_SelfTimeCommandHandleEvent(pMe, wParam);
   
            case CAMERACFGCAPTURE:
                return CameraApp_PopMenu_CaptureCommandHandleEvent(pMe, wParam);
   
            case CAMERACFGBANDING:
                return CameraApp_PopMenu_BandingCommandHandleEvent(pMe, wParam);
   
            case CAMERACFGSTORAGE:
                return CameraApp_PopMenu_StorageCommandHandleEvent(pMe, wParam);
   
            case CAMERACFGRESET:
                return CameraApp_PopMenu_ResetCFGCommandHandleEvent(pMe, wParam);
   
            default:
                return FALSE;
        }
    }

    if(pMe->m_pActiveDlgID == IDD_CVIDEOCFG)
    {
        switch(pMe->m_nVideoCFG)
        {
            case VIDEOCFGCHOOSE:
                return CameraApp_PopMenu_ChooseCommandHandleEvent(pMe, wParam);

            case VIDEOCFGENVIROMENT:
                return CameraApp_PopMenu_EnvironmentCommandHandleEvent(pMe, wParam);

            case VIDEOCFGFUN:
                return CameraApp_PopMenu_FunCommandHandleEvent(pMe, wParam);
 
            case VIDEOCFGRESET:
                return CameraApp_PopMenu_ResetCFGCommandHandleEvent(pMe, wParam);
           
            default:
                return FALSE;
        }
    }
    return TRUE;
}

/*==============================================================================
������
       CameraApp_PopMenu_ChooseCommandHandleEvent
˵����
       camera/video�л����ò˵��������
       
������
       pMe [in]��ָ��CameraApp Applet����ṹ��ָ�롣�ýṹ����С������ض���Ϣ��
       wParam���¼�������ݡ�
       
����ֵ��
       
��ע��
       TRUE�������¼�������
       FALSE�������¼������ԡ�

       
==============================================================================*/
static boolean CameraApp_PopMenu_ChooseCommandHandleEvent(CCameraApp *pMe, uint16 wParam)
{                
    switch(wParam)
    {
        case IDS_ITEM_CAMERA:   
            pMe->m_nMainMenuItemSel = IDS_ITEM_CAMERA;
   
             // �����CameraCFG Dlg�£��������½�Camera preview
            if(pMe->m_pActiveDlgID == IDD_CVIDEOCFG)
            {
                pMe->m_bIsPreview = FALSE;
                ICAMERA_Stop(pMe->m_pCamera);
                pMe->m_nCameraState = CAM_STOP;
            }

            CLOSE_DIALOG(DLGRET_PREVIEW);
            break;

        case IDS_ITEM_VIDEO:
            if(CameraApp_PreviewFindNoMemoryCardExistHandle(pMe))
            {
                return TRUE;
            }
            
            pMe->m_nMainMenuItemSel = IDS_ITEM_VIDEO;
                     
            // �����VideoCFG Dlg�£��������½�Video preview
            if(pMe->m_pActiveDlgID == IDD_CCAMERACFG)
            {     
                pMe->m_bIsPreview = FALSE;
                ICAMERA_Stop(pMe->m_pCamera);
                pMe->m_nCameraState = CAM_STOP;
            }

            CLOSE_DIALOG(DLGRET_VIDEO);
            break;

        default:
            return FALSE;
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
       CameraApp_PopMenu_FunCommandHandleEvent
˵����
       Ȥζģʽ���ò˵��������
       
������
       pMe [in]��ָ��CameraApp Applet����ṹ��ָ�롣�ýṹ����С������ض���Ϣ��
       wParam���¼�������ݡ�
       
����ֵ��
       TRUE�������¼�������
       FALSE�������¼������ԡ�
      
��ע��
       
==============================================================================*/
static boolean CameraApp_PopMenu_FunCommandHandleEvent(CCameraApp *pMe, uint16 wParam)
{ 
#ifdef FEATURE_CAMERAAPP_FUN_FRAME
    switch(wParam)
    {
        case IDS_FUN_FRAME:
            pMe->m_bfunFrameSelected = TRUE;
            break;

        case IDS_FUN_COLOR:
            pMe->m_bfunFrameSelected = FALSE;        
            break;

        default:
            return TRUE;
    }
  
    CLOSE_DIALOG(DLGRET_FUN);
    return TRUE;
#else 
    CameraApp_SetFunColorMenuItemSel(pMe, wParam);
    ICAMERA_SetParm(pMe->m_pCamera, CAM_PARM_EFFECT,  pMe->m_nCameraColor, 0);

    switch(pMe->m_pActiveDlgID)
    {
        case IDD_CCAMERACFG:       
            (void)ICONFIG_SetItem(pMe->m_pConfig,
                                  CFGI_CAMERA_COLOR,
                                  &pMe->m_nCameraColor,
                                  sizeof(pMe->m_nCameraColor));           
            break;

        case IDD_CVIDEOCFG:
            (void)ICONFIG_SetItem(pMe->m_pConfig,
                                  CFGI_VIDEO_COLOR,
                                  &pMe->m_nCameraColor,
                                  sizeof(pMe->m_nCameraColor));
            break;

        default:
            break;
    }
    
    CLOSE_DIALOG(DLGRET_CANCELED);
    return TRUE;
#endif  
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

    if(pMe->m_nCameraSize == OEMNV_CAMERA_SIZE_176_220)
    {
        ICAMERA_SetParm(pMe->m_pCamera, CAM_PARM_PREVIEWWITHFRAME, 1, 0);
    }
    else
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
       CameraApp_PopMenu_CaptureCommandHandleEvent
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
static boolean CameraApp_PopMenu_CaptureCommandHandleEvent(CCameraApp *pMe, uint16 wParam)
{
    //uint16 capture_mode = 1;
    
    switch(wParam)
    {
        case IDS_CAPTURE_OFF:
            pMe->m_nCaptureItemSel = IDS_CAPTURE_OFF; 
            pMe->m_nSnapShotTimes = 1;
            pMe->m_nSnapShotPicNum = 1;
            break;

        case IDS_CAPTURE_MODE1:
            pMe->m_nCaptureItemSel = IDS_CAPTURE_MODE1;
            pMe->m_nSnapShotTimes = 1;
            pMe->m_nSnapShotPicNum = 1;
            break;

        case IDS_CAPTURE_MODE2:
            pMe->m_nCaptureItemSel = IDS_CAPTURE_MODE2;
            pMe->m_nSnapShotTimes = 3;
            pMe->m_nSnapShotPicNum = 3;
            break;

        case IDS_CAPTURE_MODE3:
            pMe->m_nCaptureItemSel = IDS_CAPTURE_MODE3;
            pMe->m_nSnapShotTimes = 5;
            pMe->m_nSnapShotPicNum = 5;
            break;

        default:
            return FALSE;
    }
    ICAMERA_SetParm(pMe->m_pCamera, CAM_PARM_MULTISHOT, 1, 0);
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
       CameraApp_PopMenu_StorageCommandHandleEvent
˵����
       �洢���ò˵��������
       
������
       pMe [in]��ָ��CameraApp Applet����ṹ��ָ�롣�ýṹ����С������ض���Ϣ��
       wParam���¼�������ݡ�
       
����ֵ��
       TRUE�������¼�������
       FALSE�������¼������ԡ�
       
��ע��
       
==============================================================================*/
static boolean CameraApp_PopMenu_StorageCommandHandleEvent(CCameraApp *pMe, uint16 wParam)
{ 
    switch(wParam)
    {
        case IDS_STORAGE_PHONE:
            pMe->m_nCameraStorage = OEMNV_CAMERA_STORAGE_PHONE;

            if(pMe->m_nCameraSize != OEMNV_CAMERA_SIZE_176_220)
            {
                pMe->m_nCameraSize = OEMNV_CAMERA_SIZE_176_220;
                
                (void)ICONFIG_SetItem(pMe->m_pConfig,
                                      CFGI_CAMERA_SIZE,
                                      &pMe->m_nCameraSize,
                                      sizeof(pMe->m_nCameraSize));  

                ICAMERA_Stop(pMe->m_pCamera);
    
                pMe->m_bIsPreview = FALSE;
            }

            //pMe->m_bCalculateLeftPicNum = TRUE;
            break;

        case IDS_STORAGE_SDCARD:
            if(CameraApp_PreviewFindNoMemoryCardExistHandle(pMe))
            {
                return TRUE;
            }
            
            pMe->m_nCameraStorage = OEMNV_CAMERA_STORAGE_MEMORY_CARD;
            break;

        default:
            return FALSE;
    }
    
    (void)ICONFIG_SetItem(pMe->m_pConfig,
                          CFGI_CAMERA_STORAGE,
                          &pMe->m_nCameraStorage,
                          sizeof(pMe->m_nCameraStorage));    
    
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
       CameraApp_HotKeyAVK_1_QualityHandleEvent
˵����
       ��ݼ�AVK_1������Quality�����ô�����
       
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
static boolean CameraApp_HotKeyAVK_1_QualityHandleEvent(CCameraApp *pMe, 
                                                        AEEEvent eCode, 
                                                        uint16 wParam, 
                                                        uint32 dwParam)
{
    int16 nResID = 0; 
    int n = 0;
    int16 quality = 0;
   
    MEMSET(pMe->m_szHotKeyTextBuf, 0, sizeof(pMe->m_szHotKeyTextBuf));
    MEMSET(pMe->m_szResTextBuf, 0, sizeof(pMe->m_szResTextBuf));
 
    (void)ICONFIG_GetItem(pMe->m_pConfig,
                          CFGI_CAMERA_QUALITY,
                          &pMe->m_nCameraQuality,
                          sizeof(pMe->m_nCameraQuality));
    
    if(pMe->m_nCameraQuality == OEMNV_CAMERA_QUALITY_LOW)
    {
        pMe->m_nCameraQuality = OEMNV_CAMERA_QUALITY_HIGH;
    }
    else
    {
        pMe->m_nCameraQuality++;
    }
  
    n = ISHELL_LoadResString(pMe->m_pShell, 
                             AEE_APPSCAMERAAPP_RES_FILE,       
                             IDS_QUALITY_TITLE, 
                             pMe->m_szHotKeyTextBuf, 
                             sizeof(pMe->m_szHotKeyTextBuf)); 
    
    pMe->m_szHotKeyTextBuf[n++] = (AECHAR)'\n';  
                     
    switch(pMe->m_nCameraQuality)
    {      
        case OEMNV_CAMERA_QUALITY_HIGH: 
            nResID = IDS_QUALITY_HIGH;
            quality = CAMERA_APP_QUALITY_HIGH;
            break;

        case OEMNV_CAMERA_QUALITY_MED:
            nResID = IDS_QUALITY_MED;
            quality = CAMERA_APP_QUALITY_MED;
            break;

        case OEMNV_CAMERA_QUALITY_LOW:
            nResID = IDS_QUALITY_LOW;
            quality = CAMERA_APP_QUALITY_LOW;
            break;
   
        default:
            break;
    }
    
    ISHELL_LoadResString(pMe->m_pShell, 
                         AEE_APPSCAMERAAPP_RES_FILE,       
                         nResID, 
                         pMe->m_szResTextBuf, 
                         sizeof(pMe->m_szResTextBuf));
    
    WSTRCPY(pMe->m_szHotKeyTextBuf + n, pMe->m_szResTextBuf);
    
    (void)ICONFIG_SetItem(pMe->m_pConfig,
                          CFGI_CAMERA_QUALITY,
                          &pMe->m_nCameraQuality,
                          sizeof(pMe->m_nCameraQuality));
    
    ICAMERA_SetQuality(pMe->m_pCamera, quality);

    ICAMERA_Stop(pMe->m_pCamera);

    pMe->m_nCameraState = CAM_STOP;

    ICAMERA_Preview(pMe->m_pCamera);

    pMe->m_nCameraState = CAM_PREVIEW;

    CameraApp_SetParamAfterPreview(pMe);
    
    return CameraApp_DisplayHotKeyText(pMe, eCode, wParam, dwParam); 
}

/*==============================================================================
������
       CameraApp_HotKeyAVK_2_ChooseHandleEvent
˵����
       ��ݼ�AVK_2������Camera/Video�л������ô�����
       
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
static boolean CameraApp_HotKeyAVK_2_ChooseHandleEvent(CCameraApp *pMe, 
                                                       AEEEvent eCode, 
                                                       uint16 wParam, 
                                                       uint32 dwParam)
{  
    IDISPLAY_SetClipRect(pMe->m_pDisplay, 0);
       
    switch(pMe->m_pActiveDlgID)
    {
        case IDD_CPREVIEW:
            if(CameraApp_PreviewFindNoMemoryCardExistHandle(pMe))
            {
                return TRUE;
            }
            
            pMe->m_nMainMenuItemSel = IDS_ITEM_VIDEO;            
            CLOSE_DIALOG(DLGRET_VIDEO);           
            break;

        case IDD_CVIDEO:
            pMe->m_nMainMenuItemSel = IDS_ITEM_CAMERA;  
            CLOSE_DIALOG(DLGRET_PREVIEW);           
            break;

        default:
            break; 
    }

    pMe->m_bIsPreview = FALSE;
    
    ICAMERA_Stop(pMe->m_pCamera);

    pMe->m_nCameraState = CAM_STOP;

    return TRUE; //�жϷ���ֵ�����޸ġ���20081215
}

/*==============================================================================
������
       CameraApp_HotKeyAVK_3_CaptureHandleEvent
˵����
       ��ݼ�AVK_3����������ģʽ�л������ô�����
       
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
static boolean CameraApp_HotKeyAVK_3_CaptureHandleEvent(CCameraApp *pMe, 
                                                        AEEEvent eCode, 
                                                        uint16 wParam, 
                                                        uint32 dwParam)
{
    int n = 0;
   
    MEMSET(pMe->m_szHotKeyTextBuf, 0, sizeof(pMe->m_szHotKeyTextBuf));
    MEMSET(pMe->m_szResTextBuf, 0, sizeof(pMe->m_szResTextBuf));    

    n = ISHELL_LoadResString(pMe->m_pShell, 
                         AEE_APPSCAMERAAPP_RES_FILE,       
                         IDS_CAPTURE_MODE_TITLE,
                         pMe->m_szHotKeyTextBuf, 
                         sizeof(pMe->m_szHotKeyTextBuf));
    
    pMe->m_szHotKeyTextBuf[n++] = (AECHAR)'\n';
    
    if(pMe->m_nCaptureItemSel == IDS_CAPTURE_MODE3)
    {
        pMe->m_nCaptureItemSel = IDS_CAPTURE_OFF;
    }
    else
    {
        pMe->m_nCaptureItemSel++;
    }
    
    ISHELL_LoadResString(pMe->m_pShell, 
                         AEE_APPSCAMERAAPP_RES_FILE,       
                         pMe->m_nCaptureItemSel, 
                         pMe->m_szResTextBuf, 
                         sizeof(pMe->m_szResTextBuf));
     
    WSTRCPY(pMe->m_szHotKeyTextBuf + n, pMe->m_szResTextBuf);

    switch(pMe->m_nCaptureItemSel)
    {
        case IDS_CAPTURE_OFF:
        case IDS_CAPTURE_MODE1:
            pMe->m_nSnapShotTimes = 1;
            pMe->m_nSnapShotPicNum = 1;
            break;

        case IDS_CAPTURE_MODE2:
            pMe->m_nSnapShotTimes = 3;
            pMe->m_nSnapShotPicNum = 3;
            break;

        case IDS_CAPTURE_MODE3:
            pMe->m_nSnapShotTimes = 5;
            pMe->m_nSnapShotPicNum = 5;
            break;

        default:
            return FALSE; 
    }
    ICAMERA_SetParm(pMe->m_pCamera, CAM_PARM_MULTISHOT, 1, 0);
    return CameraApp_DisplayHotKeyText(pMe, eCode, wParam, dwParam); 
}

/*==============================================================================
������
       CameraApp_HotKeyAVK_4_ShutterToneHandleEvent
˵����
       ��ݼ�AVK_4�����ڿ����������ô�����
       
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
static boolean CameraApp_HotKeyAVK_4_ShutterToneHandleEvent(CCameraApp *pMe, 
                                                            AEEEvent eCode, 
                                                            uint16 wParam, 
                                                            uint32 dwParam)
{
    int16 nResID = 0;
    int n = 0;
   
    MEMSET(pMe->m_szHotKeyTextBuf, 0, sizeof(pMe->m_szHotKeyTextBuf));
    MEMSET(pMe->m_szResTextBuf, 0, sizeof(pMe->m_szResTextBuf));
    
    n = ISHELL_LoadResString(pMe->m_pShell, 
                             AEE_APPSCAMERAAPP_RES_FILE,       
                             IDS_TONE_TITLE, 
                             pMe->m_szHotKeyTextBuf, 
                             sizeof(pMe->m_szHotKeyTextBuf));     
                 
    pMe->m_szHotKeyTextBuf[n++] = (AECHAR)'\n';
    
    (void)ICONFIG_GetItem(pMe->m_pConfig,
                          CFGI_CAMERA_TONE,
                          &pMe->m_nCameraTone,
                          sizeof(pMe->m_nCameraTone));
#ifdef FEATURE_CARRIER_CHINA_TELCOM
    if(pMe->m_nCameraTone == OEMNV_CAMERA_SHUTTER_TONE_SHUTTER2)
    {
        pMe->m_nCameraTone = OEMNV_CAMERA_SHUTTER_TONE_SHUTTER1;
    }
    else
    {
        pMe->m_nCameraTone = OEMNV_CAMERA_SHUTTER_TONE_SHUTTER2;
    }
#else        
    if(pMe->m_nCameraTone == OEMNV_CAMERA_SHUTTER_TONE_DISABLE)
    {
        pMe->m_nCameraTone = OEMNV_CAMERA_SHUTTER_TONE_ENABLE;
    }
    else
    {
        pMe->m_nCameraTone = OEMNV_CAMERA_SHUTTER_TONE_DISABLE;
    }
#endif	
    
    switch(pMe->m_nCameraTone)
    {      
#ifdef FEATURE_CARRIER_CHINA_TELCOM
        case OEMNV_CAMERA_SHUTTER_TONE_SHUTTER1:
            nResID = IDS_TONE_SHUTTER1;           
            break;

        case OEMNV_CAMERA_SHUTTER_TONE_SHUTTER2:
            nResID = IDS_TONE_SHUTTER2;
            break;
#else        
        case OEMNV_CAMERA_SHUTTER_TONE_ENABLE:
            nResID = IDS_TONE_ENABLE;           
            break;

        case OEMNV_CAMERA_SHUTTER_TONE_DISABLE:
            nResID = IDS_TONE_DISABLE;
            break;
#endif			
      
        default:
            break;
    }

    ISHELL_LoadResString(pMe->m_pShell, 
                                 AEE_APPSCAMERAAPP_RES_FILE,       
                                 nResID, 
                                 pMe->m_szResTextBuf, 
                                 sizeof(pMe->m_szResTextBuf));
    
    WSTRCPY(pMe->m_szHotKeyTextBuf + n, pMe->m_szResTextBuf); 
    
    (void)ICONFIG_SetItem(pMe->m_pConfig,
                          CFGI_CAMERA_TONE,
                          &pMe->m_nCameraTone,
                          sizeof(pMe->m_nCameraTone));
    return CameraApp_DisplayHotKeyText(pMe, eCode, wParam, dwParam); 
}

/*==============================================================================
������
       CameraApp_HotKeyAVK_5_EnvironmentHandleEvent
˵����
       ��ݼ�AVK_5���������ջ��������ô�����
       
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
static boolean CameraApp_HotKeyAVK_5_EnvironmentHandleEvent(CCameraApp *pMe, 
                                                            AEEEvent eCode, 
                                                            uint16 wParam, 
                                                            uint32 dwParam)
{
    int16 nResID = 0;
    uint32 dwFPS = 5;
    
    //�����ִ����ַ�����
    int n = 0;
   
    MEMSET(pMe->m_szHotKeyTextBuf, 0, sizeof(pMe->m_szHotKeyTextBuf));
    MEMSET(pMe->m_szResTextBuf, 0, sizeof(pMe->m_szResTextBuf));
    
    n = ISHELL_LoadResString(pMe->m_pShell, 
                             AEE_APPSCAMERAAPP_RES_FILE,       
                             IDS_ENVIR_TITLE, 
                             pMe->m_szHotKeyTextBuf, 
                             sizeof(pMe->m_szHotKeyTextBuf));

    pMe->m_szHotKeyTextBuf[n++] = (AECHAR)'\n';

    switch(pMe->m_pActiveDlgID)
    {
        case IDD_CPREVIEW:
            (void)ICONFIG_GetItem(pMe->m_pConfig,
                                  CFGI_CAMERA_ENVIROMENT,
                                  &pMe->m_nCameraEnviroment,
                                  sizeof(pMe->m_nCameraEnviroment));
        
            if(pMe->m_nCameraEnviroment == OEMNV_CAMERA_ENVIR_CLOUDY)
            {
                pMe->m_nCameraEnviroment = OEMNV_CAMERA_ENVIR_AUTO;
            }
            else
            {
                pMe->m_nCameraEnviroment ++;
            }
            
            switch(pMe->m_nCameraEnviroment)
            {      
                case OEMNV_CAMERA_ENVIR_AUTO:   
                    nResID = IDS_ENVIR_MODE_AUTO;
                    dwFPS = 0;
                    break;

                case OEMNV_CAMERA_ENVIR_NIGHT:
                    nResID = IDS_ENVIR_MODE_NIGHT;
                    dwFPS = 3;
                    break;

                case OEMNV_CAMERA_ENVIR_SUNNY:
                    nResID = IDS_ENVIR_MODE_SUNNY;
                    dwFPS = 1;
                    break;

                case OEMNV_CAMERA_ENVIR_CLOUDY:
                    nResID = IDS_ENVIR_MODE_CLOUDY;
                    dwFPS = 2;
                    break;
          
                default:
                    break;
            }

            ISHELL_LoadResString(pMe->m_pShell, 
                                 AEE_APPSCAMERAAPP_RES_FILE,       
                                 nResID, 
                                 pMe->m_szResTextBuf, 
                                 sizeof(pMe->m_szResTextBuf));
            
            WSTRCPY(pMe->m_szHotKeyTextBuf + n, pMe->m_szResTextBuf); 

            (void)ICONFIG_SetItem(pMe->m_pConfig,
                                  CFGI_CAMERA_ENVIROMENT,
                                  &pMe->m_nCameraEnviroment,
                                  sizeof(pMe->m_nCameraEnviroment));
            break;

        case IDD_CVIDEO:
            (void)ICONFIG_GetItem(pMe->m_pConfig,
                                  CFGI_VIDEO_ENVIROMENT,
                                  &pMe->m_nCameraEnviroment,
                                  sizeof(pMe->m_nCameraEnviroment));
        
            if(pMe->m_nCameraEnviroment == OEMNV_CAMERA_ENVIR_CLOUDY)
            {
                pMe->m_nCameraEnviroment = OEMNV_CAMERA_ENVIR_AUTO;
            }
            else
            {
                pMe->m_nCameraEnviroment ++;
            }
        
            switch(pMe->m_nCameraEnviroment)
            {      
                case OEMNV_CAMERA_ENVIR_AUTO:   
                    nResID = IDS_ENVIR_MODE_AUTO;
                    dwFPS = 0;
                    break;

                case OEMNV_CAMERA_ENVIR_NIGHT:
                    nResID = IDS_ENVIR_MODE_NIGHT;
                    dwFPS = 3;
                    break;

                case OEMNV_CAMERA_ENVIR_SUNNY:
                    nResID = IDS_ENVIR_MODE_SUNNY;
                    dwFPS = 1;
                    break;

                case OEMNV_CAMERA_ENVIR_CLOUDY:
                    nResID = IDS_ENVIR_MODE_CLOUDY;
                    dwFPS = 2;
                    break;
      
                default:
                    break;
            }
            
            ISHELL_LoadResString(pMe->m_pShell, 
                                 AEE_APPSCAMERAAPP_RES_FILE,       
                                 nResID, 
                                 pMe->m_szResTextBuf, 
                                 sizeof(pMe->m_szResTextBuf));
                        
            WSTRCPY(pMe->m_szHotKeyTextBuf + n, pMe->m_szResTextBuf); 
      
            (void)ICONFIG_SetItem(pMe->m_pConfig,
                                  CFGI_VIDEO_ENVIROMENT,
                                  &pMe->m_nCameraEnviroment,
                                  sizeof(pMe->m_nCameraEnviroment));
            break;

        default:
            break;        
    }

    ICAMERA_SetFramesPerSecond(pMe->m_pCamera, dwFPS);
    
    return CameraApp_DisplayHotKeyText(pMe, eCode, wParam, dwParam); 
}

/*==============================================================================
������
       CameraApp_HotKeyAVK_6_FrameHandleEvent
˵����
       ��ݼ�AVK_6�������������ô�����
       
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
static boolean CameraApp_HotKeyAVK_6_FrameHandleEvent(CCameraApp *pMe, 
                                                      AEEEvent eCode, 
                                                      uint16 wParam, 
                                                      uint32 dwParam)
{
    if(pMe->m_nCameraSize != OEMNV_CAMERA_SIZE_176_220)
    {
        return TRUE;
    }

    if(pMe->m_nCameraFrame == OEMNV_CAMERA_FRAME_8)
    {
        pMe->m_nCameraFrame = OEMNV_CAMERA_FRAME_0;
    }
    else
    {
        pMe->m_nCameraFrame++;
    } 

    (void)ICONFIG_SetItem(pMe->m_pConfig,
                          CFGI_CAMERA_FRAME,
                          &pMe->m_nCameraFrame,
                          sizeof(pMe->m_nCameraFrame));

    return ISHELL_SendEvent(pMe->m_pShell, AEECLSID_APP_CAMERA, EVT_USER_REDRAW, NULL, NULL);
}

/*==============================================================================
������
       CameraApp_HotKeyAVK_7_ColorHandleEvent
˵����
       ��ݼ�AVK_6�������˾������ô�����
       
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
static boolean CameraApp_HotKeyAVK_7_ColorHandleEvent(CCameraApp * pMe, 
                                                      AEEEvent eCode, 
                                                      uint16 wParam, 
                                                      uint32 dwParam)
{
    int16 nResID = 0;
    int n = 0;
       
    MEMSET(pMe->m_szHotKeyTextBuf, 0, sizeof(pMe->m_szHotKeyTextBuf));
    MEMSET(pMe->m_szResTextBuf, 0, sizeof(pMe->m_szResTextBuf));
    
    n = ISHELL_LoadResString(pMe->m_pShell, 
                             AEE_APPSCAMERAAPP_RES_FILE,       
                             IDS_COLOR_TITLE, 
                             pMe->m_szHotKeyTextBuf, 
                             sizeof(pMe->m_szHotKeyTextBuf));

    pMe->m_szHotKeyTextBuf[n++] = (AECHAR)'\n';

    switch(pMe->m_pActiveDlgID)
    {
        case IDD_CPREVIEW:
            (void)ICONFIG_GetItem(pMe->m_pConfig,
                                  CFGI_CAMERA_COLOR,
                                  &pMe->m_nCameraColor,
                                  sizeof(pMe->m_nCameraColor));
                    
            break;

        case IDD_CVIDEO:
            (void)ICONFIG_GetItem(pMe->m_pConfig,
                                  CFGI_VIDEO_COLOR,
                                  &pMe->m_nCameraColor,
                                  sizeof(pMe->m_nCameraColor));
            
            break;

        default:
            break;        
    }

    pMe->m_nColorSel = CameraApp_ColorEffectSel(pMe);

    if(pMe->m_nColorSel == COLORGREEN)
    {
        pMe->m_nColorSel = COLORNORMAL;
    }
    else
    {
        pMe->m_nColorSel++;
    }

    CameraApp_SetColorEffect(pMe);
    
    nResID = CameraApp_GetColorEffectResID(pMe);
    
    ISHELL_LoadResString(pMe->m_pShell, 
                         AEE_APPSCAMERAAPP_RES_FILE,       
                         nResID, 
                         pMe->m_szResTextBuf, 
                         sizeof(pMe->m_szResTextBuf));
    
    WSTRCPY(pMe->m_szHotKeyTextBuf + n, pMe->m_szResTextBuf);

    switch(pMe->m_pActiveDlgID)
    {
        case IDD_CPREVIEW:
            (void)ICONFIG_SetItem(pMe->m_pConfig,
                                  CFGI_CAMERA_COLOR,
                                  &pMe->m_nCameraColor,
                                  sizeof(pMe->m_nCameraColor));
                    
            break;

        case IDD_CVIDEO:
            (void)ICONFIG_SetItem(pMe->m_pConfig,
                                  CFGI_VIDEO_COLOR,
                                  &pMe->m_nCameraColor,
                                  sizeof(pMe->m_nCameraColor));
            
            break;

        default:
            break;        
    }
    
    ICAMERA_SetParm(pMe->m_pCamera, CAM_PARM_EFFECT,  pMe->m_nCameraColor, 0);

    return CameraApp_DisplayHotKeyText(pMe, eCode, wParam, dwParam);    
}

/*==============================================================================
������
       CameraApp_HotKeyAVK_8_SelfTimeHandleEvent
˵����
       ��ݼ�AVK_8���������ĵ����ô�����
       
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
static boolean CameraApp_HotKeyAVK_8_SelfTimeHandleEvent(CCameraApp * pMe, 
                                                         AEEEvent eCode, 
                                                         uint16 wParam, 
                                                         uint32 dwParam)
{
    int n = 0;
   
    MEMSET(pMe->m_szHotKeyTextBuf, 0, sizeof(pMe->m_szHotKeyTextBuf));
    MEMSET(pMe->m_szResTextBuf, 0, sizeof(pMe->m_szResTextBuf));
    
    n = ISHELL_LoadResString(pMe->m_pShell, 
                         AEE_APPSCAMERAAPP_RES_FILE,       
                         IDS_SELFTIME_TITLE, 
                         pMe->m_szHotKeyTextBuf, 
                         sizeof(pMe->m_szHotKeyTextBuf));     
   
    pMe->m_szHotKeyTextBuf[n++] = (AECHAR)'\n';
    
    if(pMe->m_nSelfTimeItemSel == IDS_SELFTIME_SEC3)
    {
        pMe->m_nSelfTimeItemSel = IDS_SELFTIME_OFF;
    }
    else
    {
        pMe->m_nSelfTimeItemSel++;
    }
    
    ISHELL_LoadResString(pMe->m_pShell, 
                         AEE_APPSCAMERAAPP_RES_FILE,       
                         pMe->m_nSelfTimeItemSel, 
                         pMe->m_szResTextBuf, 
                         sizeof(pMe->m_szResTextBuf));
    
    WSTRCPY(pMe->m_szHotKeyTextBuf + n, pMe->m_szResTextBuf);
    return CameraApp_DisplayHotKeyText(pMe, eCode, wParam, dwParam);     
}

/*==============================================================================
������
       CameraApp_HotKeyAVK_STAR_SizeHandleEvent
˵����
       ��ݼ�AVK_STAR���������մ�С�����ô�����
       
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
static boolean CameraApp_HotKeyAVK_STAR_SizeHandleEvent(CCameraApp * pMe, 
                                                        AEEEvent eCode, 
                                                        uint16 wParam, 
                                                        uint32 dwParam)
{  
    int n = 0;

    if(pMe->m_nCameraStorage == OEMNV_CAMERA_STORAGE_PHONE)
    {
        return TRUE;
    }
   
    MEMSET(pMe->m_szHotKeyTextBuf, 0, sizeof(pMe->m_szHotKeyTextBuf));
    MEMSET(pMe->m_szResTextBuf, 0, sizeof(pMe->m_szResTextBuf));
 
   (void)ICONFIG_GetItem(pMe->m_pConfig,
                         CFGI_CAMERA_SIZE,
                         &pMe->m_nCameraSize,
                         sizeof(pMe->m_nCameraSize));
    
    if(pMe->m_nCameraSize == OEMNV_CAMERA_SIZE_176_220)
    {
        pMe->m_nCameraSize = OEMNV_CAMERA_SIZE_1600_1200;
    }
    else
    {
        pMe->m_nCameraSize++;
    }
  
    n = ISHELL_LoadResString(pMe->m_pShell, 
                             AEE_APPSCAMERAAPP_RES_FILE,       
                             IDS_CAPTURE_SIZE_TITLE, 
                             pMe->m_szHotKeyTextBuf, 
                             sizeof(pMe->m_szHotKeyTextBuf)); 
    
    pMe->m_szHotKeyTextBuf[n++] = (AECHAR)'\n'; 

    CameraApp_SetCameraCaptureSize(pMe, wParam);

    switch(pMe->m_nCameraSize)
    {
        case OEMNV_CAMERA_SIZE_176_220:
            WSTRCPY(pMe->m_szResTextBuf, SIZE_176_220);
            break;

        case OEMNV_CAMERA_SIZE_320_240:
            WSTRCPY(pMe->m_szResTextBuf, SIZE_320_240);
            break;

        case OEMNV_CAMERA_SIZE_640_480:
            WSTRCPY(pMe->m_szResTextBuf, SIZE_640_480);
            break;

        case OEMNV_CAMERA_SIZE_1280_960:
            WSTRCPY(pMe->m_szResTextBuf, SIZE_1280_960);
            break;

        case OEMNV_CAMERA_SIZE_1600_1200:
            WSTRCPY(pMe->m_szResTextBuf, SIZE_1600_1200);
            break;

        default:
            break;
                       
    }
                          
    WSTRCPY(pMe->m_szHotKeyTextBuf + n, pMe->m_szResTextBuf);
    
    ICAMERA_Stop(pMe->m_pCamera);

    pMe->m_nCameraState = CAM_STOP;

    pMe->m_bIsPreview = FALSE;
     
    return CameraApp_DisplayHotKeyText(pMe, eCode, wParam, dwParam); 
}

/*==============================================================================
������
       CameraApp_HotKeyAVK_POUND_HideIconHandleEvent
˵����
       ��ݼ�AVK_POUND������ICON��ʾ�����ô�����
       
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
static boolean CameraApp_HotKeyAVK_POUND_HideIconHandleEvent(CCameraApp * pMe, 
                                                             AEEEvent eCode, 
                                                             uint16 wParam, 
                                                             uint32 dwParam)
{
    switch(pMe->m_pActiveDlgID)
    {
        case IDD_CPREVIEW: 
            pMe->m_bCameraHideIcon = (!pMe->m_bCameraHideIcon);
                            
            (void)ICONFIG_SetItem(pMe->m_pConfig,
                                  CFGI_CAMERA_ICON,
                                  &pMe->m_bCameraHideIcon,
                                  sizeof(pMe->m_bCameraHideIcon));
            break;

        case IDD_CVIDEO:
            pMe->m_bVideoHideIcon = (!pMe->m_bVideoHideIcon);
                            
            (void)ICONFIG_SetItem(pMe->m_pConfig,
                                  CFGI_VIDEO_ICON,
                                  &pMe->m_bVideoHideIcon,
                                  sizeof(pMe->m_bVideoHideIcon));
            break;

        default:
            break;            
    }

    IDISPLAY_ClearScreen(pMe->m_pDisplay);
    
    return ISHELL_SendEvent(pMe->m_pShell, 
                            AEECLSID_APP_CAMERA, 
                            EVT_USER_REDRAW, 
                            NULL, 
                            NULL);
}

/*==============================================================================
������
       CameraApp_DisplayHotKeyText
˵����
       ��ʾ Hotkey ��ʾ�ַ�������
       
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
static boolean CameraApp_DisplayHotKeyText(CCameraApp *pMe, 
                                           AEEEvent eCode, 
                                           uint16 wParam, 
                                           uint32 dwParam)
{ 

    //��ֹ���ٰ��������趨�Ķ�ʱ��δȡ�� 
    ISHELL_CancelTimer(pMe->m_pShell, NULL, pMe);   
    
    IDISPLAY_ClearScreen(pMe->m_pDisplay);
    
    ISHELL_SendEvent(pMe->m_pShell, AEECLSID_APP_CAMERA, EVT_USER_REDRAW, NULL, NULL);
    
    CameraApp_DrawHotKeyTextWithFrame(pMe);    

    // ���ö�ʱ����ˢ����Ļ
    ISHELL_SetTimer(pMe->m_pShell, 1500, (PFNNOTIFY)CameraApp_HotKeyDisplayTimeout, pMe);
    
    return TRUE;
}

/*==============================================================================
������
       CameraApp_HotKeyDisplayTimeout
˵����
       ��ʱ��������
       
������      
       pMe [in]��ָ��CameraApp Applet����ṹ��ָ�롣�ýṹ����С������ض���Ϣ��
             
����ֵ�� 
       
��ע��
       
==============================================================================*/
static void CameraApp_HotKeyDisplayTimeout(CCameraApp *pMe)
{   
    IDISPLAY_ClearScreen(pMe->m_pDisplay);

    ISHELL_SendEvent(pMe->m_pShell, AEECLSID_APP_CAMERA, EVT_USER_REDRAW, NULL, NULL);    
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
            case CAMERACFGCHOOSE:               
                    CameraApp_PopMenu_ChooseInit(pMe, popMenu);
                    break;
        
                case CAMERACFGENVIRMENT:
                    CameraApp_PopMenu_EnvironmentInit(pMe, popMenu);
                    break;
        
                case CAMERACFGFUN:
                    CameraApp_PopMenu_FunInit(pMe, popMenu);
                    break;
       
                case CAMERACFGQUALITY:
                    CameraApp_PopMenu_QualityInit(pMe, popMenu);
                    break;
       
                case CAMERACFGSIZE:
                    CameraApp_PopMenu_SizeInit(pMe, popMenu);
                    break;
       
                case CAMERACFGTONE:
                    CameraApp_PopMenu_ShutterToneInit(pMe, popMenu);
                    break;
       
                case CAMERACFGSELFTIME:
                    CameraApp_PopMenu_SelfTimeInit(pMe, popMenu);
                    break;
       
                case CAMERACFGCAPTURE:
                    CameraApp_PopMenu_CaptureInit(pMe, popMenu);
                    break;
       
                case CAMERACFGBANDING:
                    CameraApp_PopMenu_BandingInit(pMe, popMenu);
                    break;
       
                case CAMERACFGSTORAGE:
                    CameraApp_PopMenu_StorageInit(pMe, popMenu);
                    break;
       
                case CAMERACFGRESET:
                    CameraApp_PopMenu_ResetCFGInit(pMe, popMenu);
                    break;
       
                case CAMERACFGHOTKEY:
                    CameraApp_PopMenu_HotKeyInit(pMe, popMenu);
                    break;
       
                default:
                    break; 
        }
    }

    if(pMe->m_pActiveDlgID == IDD_CVIDEOCFG)
    {
        switch(pMe->m_nVideoCFG)
        {
            case VIDEOCFGCHOOSE:
                CameraApp_PopMenu_ChooseInit(pMe, popMenu);
                break;

            case VIDEOCFGENVIROMENT:
                 CameraApp_PopMenu_EnvironmentInit(pMe, popMenu);
                 break;

            case VIDEOCFGFUN:
                CameraApp_PopMenu_FunInit(pMe, popMenu);
                break;
 
            case VIDEOCFGRESET:
                 CameraApp_PopMenu_ResetCFGInit(pMe, popMenu);
                 break;

            case VIDEOCFGHOTKEY:
                CameraApp_PopMenu_HotKeyInit(pMe, popMenu);
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
        pMe->m_nCameraBrightness = OEMNV_CAMERA_BRIGHTNESS_LEVEL3;
        pMe->m_nCameraFrame = OEMNV_CAMERA_FRAME_0;
        pMe->m_nCameraColor = OEMNV_CAMERA_COLOR_NORMAL;
        pMe->m_nCameraZoom = OEMNV_CAMERA_ZOOM_LEVEL1;

        if(CameraApp_FindMemoryCardExist(pMe))
        {
            pMe->m_nCameraStorage = OEMNV_CAMERA_STORAGE_MEMORY_CARD;
	     pMe->m_nCameraSize = OEMNV_CAMERA_SIZE_176_220;
        }
        else
        {
            pMe->m_nCameraStorage = OEMNV_CAMERA_STORAGE_PHONE;
            pMe->m_nCameraSize = OEMNV_CAMERA_SIZE_176_220;
        }
        
        pMe->m_nSelfTimeItemSel = IDS_SELFTIME_OFF;
        pMe->m_nCaptureItemSel = IDS_CAPTURE_OFF;
        pMe->m_bCameraHideIcon = FALSE;
 
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
        
        (void)ICONFIG_SetItem(pMe->m_pConfig,
                              CFGI_CAMERA_BRIGHTNESS,
                              &pMe->m_nCameraBrightness,
                              sizeof(pMe->m_nCameraBrightness)); 

        (void)ICONFIG_SetItem(pMe->m_pConfig,
                              CFGI_CAMERA_FRAME,
                              &pMe->m_nCameraFrame,
                              sizeof(pMe->m_nCameraFrame));

        (void)ICONFIG_SetItem(pMe->m_pConfig,
                              CFGI_CAMERA_COLOR,
                              &pMe->m_nCameraColor,
                              sizeof(pMe->m_nCameraColor));

        (void)ICONFIG_SetItem(pMe->m_pConfig,
                              CFGI_CAMERA_ICON,
                              &pMe->m_bCameraHideIcon,
                              sizeof(pMe->m_bCameraHideIcon));

    }
    else
    {
        OEMCAMERAENVIR      nCameraEnviroment = OEMNV_CAMERA_ENVIR_AUTO;
        OEMCAMERABRIGHTNESS nCameraBrightness = OEMNV_CAMERA_BRIGHTNESS_LEVEL3;
        OEMCAMERACOLOR      nCameraColor = OEMNV_CAMERA_COLOR_NORMAL;

        pMe->m_bVideoHideIcon = FALSE;

        (void)ICONFIG_SetItem(pMe->m_pConfig,
                              CFGI_VIDEO_ENVIROMENT,
                              &nCameraEnviroment,
                              sizeof(nCameraEnviroment));

        (void)ICONFIG_SetItem(pMe->m_pConfig,
                              CFGI_VIDEO_BRIGHTNESS,
                              &nCameraBrightness,
                              sizeof(nCameraBrightness)); 

        (void)ICONFIG_SetItem(pMe->m_pConfig,
                              CFGI_VIDEO_COLOR,
                              &nCameraColor,
                              sizeof(nCameraColor));

        (void)ICONFIG_SetItem(pMe->m_pConfig,
                              CFGI_VIDEO_ICON,
                              &pMe->m_bVideoHideIcon,
                              sizeof(pMe->m_bVideoHideIcon));
    }
}

// camera/video�л������ò˵��ĳ�ʼ��
static void CameraApp_PopMenu_ChooseInit(CCameraApp *pMe, IMenuCtl *popMenu)
{
    IMENUCTL_DeleteAll(popMenu);

    CameraApp_SetPopMenuRect(pMe, popMenu, 2);
        
    IMENUCTL_AddItem(popMenu, 
                     AEE_APPSCAMERAAPP_RES_FILE, 
                     IDS_ITEM_CAMERA, 
                     IDS_ITEM_CAMERA,
                     NULL, 
                     NULL);
    
    IMENUCTL_AddItem(popMenu,  
                     AEE_APPSCAMERAAPP_RES_FILE, 
                     IDS_ITEM_VIDEO, 
                     IDS_ITEM_VIDEO, 
                     NULL, 
                     NULL);
    
    InitMenuIcons(popMenu);
    
    switch(pMe->m_pActiveDlgID)
    {
        case IDD_CCAMERACFG:
            CameraApp_SetCFGMenuIcon(popMenu, IDS_ITEM_CAMERA, TRUE);
            break;
    
        case IDD_CVIDEOCFG:
            CameraApp_SetCFGMenuIcon(popMenu, IDS_ITEM_VIDEO, TRUE);
            break;
    
        default:
            break;                    
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
           
static void CameraApp_PopMenu_FunInit(CCameraApp *pMe, IMenuCtl *popMenu)
{
    (void)ICONFIG_GetItem(pMe->m_pConfig,
                          CFGI_CAMERA_SIZE,
                          &pMe->m_nCameraSize,
                          sizeof(pMe->m_nCameraSize));
    
    IMENUCTL_DeleteAll(popMenu);
    
#ifdef  FEATURE_CAMERAAPP_FUN_FRAME
    switch(pMe->m_pActiveDlgID)
    {
        case IDD_CCAMERACFG:
            if(pMe->m_nCameraSize != OEMNV_CAMERA_SIZE_176_220)
            {               
                CameraApp_SetPopMenuRect(pMe, popMenu, 1);

                IMENUCTL_AddItem(popMenu,
                                 AEE_APPSCAMERAAPP_RES_FILE,
                                 IDS_FUN_COLOR, 
                                 IDS_FUN_COLOR,
                                 NULL, 
                                 0);
            }
            else
            {
                CameraApp_SetPopMenuRect(pMe, popMenu, 2);

                IMENUCTL_AddItem(popMenu,
                                 AEE_APPSCAMERAAPP_RES_FILE,
                                 IDS_FUN_FRAME, 
                                 IDS_FUN_FRAME,
                                 NULL, 
                                 0);

                IMENUCTL_AddItem(popMenu,
                                 AEE_APPSCAMERAAPP_RES_FILE,
                                 IDS_FUN_COLOR, 
                                 IDS_FUN_COLOR,
                                 NULL, 
                                 0);

                if(pMe->m_bfunFrameSelected)
                {
                    IMENUCTL_SetSel(popMenu, IDS_FUN_FRAME);
                }
                else
                {
                    IMENUCTL_SetSel(popMenu, IDS_FUN_COLOR);
                }
            }
            
            break;

        case IDD_CVIDEOCFG:          
            CameraApp_SetPopMenuRect(pMe, popMenu, 1);
            
            IMENUCTL_AddItem(popMenu,
                             AEE_APPSCAMERAAPP_RES_FILE,
                             IDS_FUN_COLOR, 
                             IDS_FUN_COLOR,
                             NULL, 
                             0);
            break;

        default:
            break;
    }

#else
    CameraApp_SetPopMenuRect(pMe, popMenu, 4);

    IMENUCTL_AddItem(popMenu,
                     AEE_APPSCAMERAAPP_RES_FILE,
                     IDS_NORMAL, 
                     IDS_NORMAL,
                     NULL, 
                     0);

    IMENUCTL_AddItem(popMenu,
                     AEE_APPSCAMERAAPP_RES_FILE,
                     IDS_COLOR_BANDW, 
                     IDS_COLOR_BANDW,
                     NULL, 
                     0);

    IMENUCTL_AddItem(popMenu,
                     AEE_APPSCAMERAAPP_RES_FILE,
                     IDS_COLOR_SEPIA, 
                     IDS_COLOR_SEPIA,
                     NULL, 
                     0);

    IMENUCTL_AddItem(popMenu,
                     AEE_APPSCAMERAAPP_RES_FILE,
                     IDS_COLOR_NEGATIVE, 
                     IDS_COLOR_NEGATIVE,
                     NULL, 
                     0);

    IMENUCTL_AddItem(popMenu,
                     AEE_APPSCAMERAAPP_RES_FILE,
                     IDS_COLOR_RED, 
                     IDS_COLOR_RED,
                     NULL, 
                     0);

    IMENUCTL_AddItem(popMenu,
                     AEE_APPSCAMERAAPP_RES_FILE,
                     IDS_COLOR_GREEN, 
                     IDS_COLOR_GREEN,
                     NULL, 
                     0);

    InitMenuIcons(popMenu);
    
    switch(pMe->m_pActiveDlgID)
    {
        case IDD_CCAMERACFG:
            (void)ICONFIG_GetItem(pMe->m_pConfig,
                                 CFGI_CAMERA_COLOR,
                                 &pMe->m_nCameraColor,
                                 sizeof(pMe->m_nCameraColor));      
            break;
    
        case IDD_CVIDEOCFG: 
            (void)ICONFIG_GetItem(pMe->m_pConfig,
                                 CFGI_VIDEO_COLOR,
                                 &pMe->m_nCameraColor,
                                 sizeof(pMe->m_nCameraColor));          
            break;
                        
        default:
            break;
    }

    CameraApp_SetColorCFGMenuIcon(pMe, popMenu);
#endif
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
        CameraApp_SetPopMenuRect(pMe, popMenu, 4);
        IMENUCTL_AddItem(popMenu, 
                         AEE_APPSCAMERAAPP_RES_FILE, 
                         IDS_CAMERA_SIZE_1600_1200,
                         IDS_CAMERA_SIZE_1600_1200, 
                         SIZE_1600_1200, 
                         NULL);

        IMENUCTL_AddItem(popMenu,
                         AEE_APPSCAMERAAPP_RES_FILE, 
                         IDS_CAMERA_SIZE_1280_960, 
                         IDS_CAMERA_SIZE_1280_960, 
                         SIZE_1280_960,
                         NULL);

        IMENUCTL_AddItem(popMenu, 
                         AEE_APPSCAMERAAPP_RES_FILE, 
                         IDS_CAMERA_SIZE_640_480,
                         IDS_CAMERA_SIZE_640_480, 
                         SIZE_640_480, 
                         NULL);

        IMENUCTL_AddItem(popMenu,
                         AEE_APPSCAMERAAPP_RES_FILE, 
                         IDS_CAMERA_SIZE_320_240,
                         IDS_CAMERA_SIZE_320_240, 
                         SIZE_320_240, 
                         NULL);

        IMENUCTL_AddItem(popMenu, 
                         AEE_APPSCAMERAAPP_RES_FILE, 
                         IDS_CAMERA_SIZE_176_220, 
                         IDS_CAMERA_SIZE_176_220, 
                         SIZE_176_220, 
                         NULL);
    }
    else
    {
        CameraApp_SetPopMenuRect(pMe, popMenu, 1);
        
        IMENUCTL_AddItem(popMenu, 
                         AEE_APPSCAMERAAPP_RES_FILE, 
                         IDS_CAMERA_SIZE_176_220, 
                         IDS_CAMERA_SIZE_176_220, 
                         SIZE_176_220, 
                         NULL);
    }
    
    InitMenuIcons(popMenu);

    (void)ICONFIG_GetItem(pMe->m_pConfig,
                          CFGI_CAMERA_SIZE,
                          &pMe->m_nCameraSize,
                          sizeof(pMe->m_nCameraSize));

    switch(pMe->m_nCameraSize)
    {
        case OEMNV_CAMERA_SIZE_176_220:
            CameraApp_SetCFGMenuIcon(popMenu, IDS_CAMERA_SIZE_176_220, TRUE);
            break;

        case OEMNV_CAMERA_SIZE_320_240:
            CameraApp_SetCFGMenuIcon(popMenu, IDS_CAMERA_SIZE_320_240, TRUE);
            break;

        case OEMNV_CAMERA_SIZE_640_480:
            CameraApp_SetCFGMenuIcon(popMenu, IDS_CAMERA_SIZE_640_480, TRUE);
            break;

        case OEMNV_CAMERA_SIZE_1280_960:
            CameraApp_SetCFGMenuIcon(popMenu, IDS_CAMERA_SIZE_1280_960, TRUE);
            break;

        case OEMNV_CAMERA_SIZE_1600_1200:
            CameraApp_SetCFGMenuIcon(popMenu, IDS_CAMERA_SIZE_1600_1200, TRUE);
            break;

        default:
            break;
    }    
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
          
static void CameraApp_PopMenu_CaptureInit(CCameraApp *pMe, IMenuCtl *popMenu)
{
    IMENUCTL_DeleteAll(popMenu);  
    CameraApp_SetPopMenuRect(pMe, popMenu, 4);
    IMENUCTL_AddItem(popMenu, 
                     AEE_APPSCAMERAAPP_RES_FILE, 
                     IDS_CAPTURE_OFF, 
                     IDS_CAPTURE_OFF, 
                     NULL, 
                     0);
    IMENUCTL_AddItem(popMenu, 
                     AEE_APPSCAMERAAPP_RES_FILE, 
                     IDS_CAPTURE_MODE1, 
                     IDS_CAPTURE_MODE1, 
                     NULL,
                     0);
    IMENUCTL_AddItem(popMenu, 
                     AEE_APPSCAMERAAPP_RES_FILE, 
                     IDS_CAPTURE_MODE2, 
                     IDS_CAPTURE_MODE2,
                     NULL, 
                     0);
    IMENUCTL_AddItem(popMenu, 
                     AEE_APPSCAMERAAPP_RES_FILE, 
                     IDS_CAPTURE_MODE3,
                     IDS_CAPTURE_MODE3,
                     NULL,
                     0);           
    InitMenuIcons(popMenu);
    CameraApp_SetCFGMenuIcon(popMenu, pMe->m_nCaptureItemSel, TRUE);
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

static void CameraApp_PopMenu_StorageInit(CCameraApp *pMe, IMenuCtl *popMenu)
{
    IMENUCTL_DeleteAll(popMenu);  
    CameraApp_SetPopMenuRect(pMe, popMenu, 2);
    IMENUCTL_AddItem(popMenu, 
                     AEE_APPSCAMERAAPP_RES_FILE, 
                     IDS_STORAGE_PHONE, 
                     IDS_STORAGE_PHONE, 
                     NULL, 
                     0);
    IMENUCTL_AddItem(popMenu, 
                     AEE_APPSCAMERAAPP_RES_FILE, 
                     IDS_STORAGE_SDCARD, 
                     IDS_STORAGE_SDCARD, 
                     NULL,
                     0);
    
    InitMenuIcons(popMenu);
    (void)ICONFIG_GetItem(pMe->m_pConfig,
                          CFGI_CAMERA_STORAGE,
                          &pMe->m_nCameraStorage,
                          sizeof(pMe->m_nCameraStorage));

    switch(pMe->m_nCameraStorage)
    {
        case OEMNV_CAMERA_STORAGE_PHONE:
            CameraApp_SetCFGMenuIcon(popMenu, IDS_STORAGE_PHONE, TRUE);
            break;

        case OEMNV_CAMERA_STORAGE_MEMORY_CARD:
            CameraApp_SetCFGMenuIcon(popMenu, IDS_STORAGE_SDCARD, TRUE);
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

static void CameraApp_PopMenu_HotKeyInit(CCameraApp *pMe, IMenuCtl *popMenu)
{
    IMENUCTL_DeleteAll(popMenu);

    switch(pMe->m_pActiveDlgID)
    {
        case IDD_CCAMERACFG:
            CameraApp_SetPopMenuRect(pMe, popMenu, 12);
            
            IMENUCTL_AddItem(popMenu, 
                             AEE_APPSCAMERAAPP_RES_FILE, 
                             IDS_HOTKEY_QUALITY, 
                             IDS_HOTKEY_QUALITY, 
                             NULL, 
                             0);
            
            IMENUCTL_AddItem(popMenu, 
                             AEE_APPSCAMERAAPP_RES_FILE, 
                             IDS_HOTKEY_CHOOSE, 
                             IDS_HOTKEY_CHOOSE, 
                             NULL, 
                             0);
            
            IMENUCTL_AddItem(popMenu, 
                             AEE_APPSCAMERAAPP_RES_FILE, 
                             IDS_HOTKEY_CAPMODE, 
                             IDS_HOTKEY_CAPMODE, 
                             NULL, 
                             0);
            
            IMENUCTL_AddItem(popMenu, 
                             AEE_APPSCAMERAAPP_RES_FILE, 
                             IDS_HOTKEY_TONE, 
                             IDS_HOTKEY_TONE, 
                             NULL, 
                             0);
            
            IMENUCTL_AddItem(popMenu, 
                             AEE_APPSCAMERAAPP_RES_FILE, 
                             IDS_HOTKEY_ENVIR, 
                             IDS_HOTKEY_ENVIR, 
                             NULL, 
                             0);
            
            IMENUCTL_AddItem(popMenu, 
                             AEE_APPSCAMERAAPP_RES_FILE, 
                             IDS_HOTKEY_FRAME, 
                             IDS_HOTKEY_FRAME, 
                             NULL, 
                             0);
            
            IMENUCTL_AddItem(popMenu, 
                             AEE_APPSCAMERAAPP_RES_FILE, 
                             IDS_HOTKEY_COLOR, 
                             IDS_HOTKEY_COLOR, 
                             NULL, 
                             0);
            
            IMENUCTL_AddItem(popMenu, 
                             AEE_APPSCAMERAAPP_RES_FILE, 
                             IDS_HOTKEY_SELFTIME, 
                             IDS_HOTKEY_SELFTIME, 
                             NULL, 
                             0);
            
            IMENUCTL_AddItem(popMenu, 
                             AEE_APPSCAMERAAPP_RES_FILE, 
                             IDS_HOTKEY_ZOOM, 
                             IDS_HOTKEY_ZOOM, 
                             NULL, 
                             0);
            
            IMENUCTL_AddItem(popMenu, 
                             AEE_APPSCAMERAAPP_RES_FILE, 
                             IDS_HOTKEY_BRIGHT, 
                             IDS_HOTKEY_BRIGHT, 
                             NULL, 
                             0);
            
            IMENUCTL_AddItem(popMenu, 
                             AEE_APPSCAMERAAPP_RES_FILE, 
                             IDS_HOTKEY_RES, 
                             IDS_HOTKEY_RES, 
                             NULL, 
                             0);
            
            IMENUCTL_AddItem(popMenu, 
                             AEE_APPSCAMERAAPP_RES_FILE, 
                             IDS_HOTKEY_HIDEICO, 
                             IDS_HOTKEY_HIDEICO, 
                             NULL, 
                             0);

            break;

        case IDD_CVIDEOCFG:
            CameraApp_SetPopMenuRect(pMe, popMenu, 5);
            IMENUCTL_AddItem(popMenu, 
                             AEE_APPSCAMERAAPP_RES_FILE, 
                             IDS_HOTKEY_CHOOSE, 
                             IDS_HOTKEY_CHOOSE, 
                             NULL, 
                             0);
            
            IMENUCTL_AddItem(popMenu, 
                             AEE_APPSCAMERAAPP_RES_FILE, 
                             IDS_HOTKEY_ENVIR, 
                             IDS_HOTKEY_ENVIR, 
                             NULL, 
                             0);
            
            IMENUCTL_AddItem(popMenu, 
                             AEE_APPSCAMERAAPP_RES_FILE, 
                             IDS_HOTKEY_COLOR, 
                             IDS_HOTKEY_COLOR, 
                             NULL, 
                             0);
                       
            IMENUCTL_AddItem(popMenu, 
                             AEE_APPSCAMERAAPP_RES_FILE, 
                             IDS_HOTKEY_BRIGHT, 
                             IDS_HOTKEY_BRIGHT, 
                             NULL, 
                             0);

             IMENUCTL_AddItem(popMenu, 
                             AEE_APPSCAMERAAPP_RES_FILE, 
                             IDS_HOTKEY_HIDEICO, 
                             IDS_HOTKEY_HIDEICO, 
                             NULL, 
                             0);
             
            break;

        default:
            break;           
    }
}

static void CameraApp_VideoRunning(CCameraApp *pMe)
{
    uint32   nMS;
    
    pMe->m_bVideoRecStart = TRUE;
    
    // ���ص�ǰʱ�䣨���룩��
    // ���ص�ֵȡ���� BREW �ƶ��豸�ӻ�վ��ȡ���豸��ǰʱ��ֵ
    nMS = ISHELL_GetTimeMS(pMe->m_pShell);

    CameraApp_VideoGettime(pMe, nMS); // ������ǰ��ʱ��
    
    // �㷨����������1s��ʱ��
    nMS = (RUNNING_UPDATE_MSECS - (ISHELL_GetTimeMS(pMe->m_pShell) - nMS)) + 1;

    DBGPRINTF("nMS = %d",nMS);
#ifdef FEATURE_SUPPORT_VC0848    
    if(VC_GetVideoRecorderDoneState())
    {
        pMe->m_bVideoRecStart = FALSE;
        CameraApp_VideoStop(pMe);
        ICAMERA_Stop(pMe->m_pCamera);
        pMe->m_nCameraState = CAM_STOP;
        
        CLOSE_DIALOG(DLGRET_VIDEOSAVE);
        return;
    }
#endif
    (void)ISHELL_SetTimer(pMe->m_pShell,
                          (int32)nMS,
                          (PFNNOTIFY)CameraApp_VideoRunning,
                          pMe);
}

static void CameraApp_VideoGettime(CCameraApp *pMe, uint32 nTicks)
{
   // Handle case where stop watch runs through midnight (23:59 - 00:00)  
   if((nTicks - pMe->m_nTicks) > MSECS_PER_DAY ) 
   {
      pMe->m_nNumDays++;
      pMe->m_nTicks = 0;
   }

    pMe->m_dwDispTime = ((nTicks - pMe->m_nTicks) + (pMe->m_nNumDays * MSECS_PER_DAY));
    
   // ����ʱ��ؼ�����Ϊÿ�μ���ˢ�ºͲ���ʾ������
   ITIMECTL_SetProperties(pMe->m_pVideoRecTimeCtl, TP_NO_MSECONDS|TP_LEFT_JUSTIFY);

   
   (void)ITIMECTL_SetTimeEx(pMe->m_pVideoRecTimeCtl, (int32)pMe->m_dwDispTime, TRUE);
}

static void CameraApp_VideoStop(CCameraApp *pMe) 
{  
    pMe->m_bVideoRecStart = FALSE;
    
   (void)ISHELL_CancelTimer(pMe->m_pShell, 
                            (PFNNOTIFY)CameraApp_VideoRunning, 
                            pMe);
                             
   pMe->m_dwDispTime = ISHELL_GetTimeMS(pMe->m_pShell) - pMe->m_nTicks;
   
   ITIMECTL_SetProperties(pMe->m_pVideoRecTimeCtl, TP_NO_MSECONDS|TP_LEFT_JUSTIFY);

   
   (void)ITIMECTL_SetTimeEx(pMe->m_pVideoRecTimeCtl, (int32)pMe->m_dwDispTime, TRUE);
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
#ifdef FEATURE_CARRIER_CHINA_TELCOM   
   if(pMe->m_nCameraTone == OEMNV_CAMERA_SHUTTER_TONE_SHUTTER1)
   {
   	md.pData = (void *)"fs:/cameraapp/shutter.mid";
   }
   else
   {
   	md.pData = (void *)"fs:/cameraapp/trend.mid";
   }
#else   		   
       md.pData = (void *)"fs:/cameraapp/shutter.mid";
#endif
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
        dy = menuItemSum*(pMe->m_nItemH+7);
    }
    else
    {
        dy = 4*(pMe->m_nItemH+7);
    }

    x = CameraApp_SetPositonX(pMe);
       
    y = pMe->m_cyHeight - dy - POPMENU_BOTTOM_Y;
 
    SETAEERECT(&mRec, x, y, dx, dy);

    IMENUCTL_SetRect(popMenu, &mRec);
}

static void CameraApp_DrawTopBar(CCameraApp *pMe)
{
    int i;
    int16  nResID[10];
    IImage *pTopBarImage = NULL; 
          
    // enviroment cfgID
    (void)ICONFIG_GetItem(pMe->m_pConfig,
                          CFGI_CAMERA_ENVIROMENT,
                          &pMe->m_nCameraEnviroment,
                          sizeof(pMe->m_nCameraEnviroment));
    
    switch(pMe->m_nCameraEnviroment)
    {
        case OEMNV_CAMERA_ENVIR_AUTO:
            nResID[0] = IDI_ENV_AUTO;
            break;
            
        case OEMNV_CAMERA_ENVIR_SUNNY:
            nResID[0] = IDI_ENV_SUNNY;
            break;

        case OEMNV_CAMERA_ENVIR_CLOUDY:
            nResID[0] = IDI_ENV_CLOUDY;
            break;

        case OEMNV_CAMERA_ENVIR_NIGHT:
            nResID[0] = IDI_ENV_NIGHT;
            break;
            
       default:
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
            nResID[1] = IDI_QUALITY_HIGH;
            break;

        case OEMNV_CAMERA_QUALITY_MED:
            nResID[1] = IDI_QUALITY_MED;
            break;

        case OEMNV_CAMERA_QUALITY_LOW:
            nResID[1] = IDI_QUALITY_LOW;
            break;
            
       default:
            break;
    }
    
     // size cfgID
     (void)ICONFIG_GetItem(pMe->m_pConfig,
                          CFGI_CAMERA_SIZE,
                          &pMe->m_nCameraSize,
                          sizeof(pMe->m_nCameraSize));
     
    switch(pMe->m_nCameraSize)
    {
        case OEMNV_CAMERA_SIZE_176_220:
            nResID[2] = IDI_SIZE_176_220;
            break;

        case OEMNV_CAMERA_SIZE_320_240:
            nResID[2] = IDI_SIZE_320_240;
            break;

        case OEMNV_CAMERA_SIZE_640_480:
            nResID[2] = IDI_SIZE_640_480;
            break;

       case OEMNV_CAMERA_SIZE_1280_960:
            nResID[2] = IDI_SIZE_1280_960;
            break;

       case OEMNV_CAMERA_SIZE_1600_1200:
            nResID[2] = IDI_SIZE_1600_1200;
            break;
            
       default:
            break;
    }

    // sound cfgID
    (void)ICONFIG_GetItem(pMe->m_pConfig,
                          CFGI_CAMERA_TONE,
                          &pMe->m_nCameraTone,
                          sizeof(pMe->m_nCameraTone));
    
    switch(pMe->m_nCameraTone)
    {
#ifdef FEATURE_CARRIER_CHINA_TELCOM
        case OEMNV_CAMERA_SHUTTER_TONE_SHUTTER1:
	 case OEMNV_CAMERA_SHUTTER_TONE_SHUTTER2:		
            nResID[3] = IDI_TONE_ENABLE;
            break;
#else        
        case OEMNV_CAMERA_SHUTTER_TONE_ENABLE:
            nResID[3] = IDI_TONE_ENABLE;
            break;

        case OEMNV_CAMERA_SHUTTER_TONE_DISABLE:
            nResID[3] = IDI_TONE_DISABLE;
            break;
#endif			
            
       default:
            break;
    }

     // draw selftime cfg    
    switch(pMe->m_nSelfTimeItemSel)
    {
        case IDS_SELFTIME_OFF:
            nResID[4] = IDI_SELFTIME_OFF;
            break;

        case IDS_SELFTIME_SEC1:
            nResID[4] = IDI_SELFTIME_5;
            break;

        case IDS_SELFTIME_SEC2:
            nResID[4] = IDI_SELFTIME_10;
            break;

        case IDS_SELFTIME_SEC3:
            nResID[4] = IDI_SELFTIME_15;
            break;
            
       default:
            break;
    }

    // capture cfgID
    switch(pMe->m_nCaptureItemSel)
    {
        case IDS_CAPTURE_OFF:
            nResID[5] = IDI_CAPTURE_OFF;
            break;

        case IDS_CAPTURE_MODE1:
            nResID[5] = IDI_CAPTURE_1;
            break;

        case IDS_CAPTURE_MODE2:
            nResID[5] = IDI_CAPTURE_3;
            break;

        case IDS_CAPTURE_MODE3:
            nResID[5] = IDI_CAPTURE_5;
            break;
            
       default:
            break;
    }

    // banding cfgID
    (void)ICONFIG_GetItem(pMe->m_pConfig,
                          CFGI_CAMERA_BANDING,
                          &pMe->m_nCameraBanding,
                          sizeof(pMe->m_nCameraBanding));
    
    switch(pMe->m_nCameraBanding)
    {
        case OEMNV_CAMERA_BANDING_50HZ:
            nResID[6] = IDI_BANDING_50;
            break;

        case OEMNV_CAMERA_BANDING_60HZ:
            nResID[6] = IDI_BANDING_60;
            break;
            
       default:
            break;
    }

    // storage cfgID
    (void)ICONFIG_GetItem(pMe->m_pConfig,
                          CFGI_CAMERA_STORAGE,
                          &pMe->m_nCameraStorage,
                          sizeof(pMe->m_nCameraStorage));
    
    switch(pMe->m_nCameraStorage)
    {
        case OEMNV_CAMERA_STORAGE_PHONE:
            nResID[7] = IDI_STORAGE_PHONE;
            break;

        case OEMNV_CAMERA_STORAGE_MEMORY_CARD:
            nResID[7] = IDI_STORAGE_SDCARD;
            break;
            
       default:
            break;
    }

     // brightness cfgID
    (void)ICONFIG_GetItem(pMe->m_pConfig,
                          CFGI_CAMERA_BRIGHTNESS,
                          &pMe->m_nCameraBrightness,
                          sizeof(pMe->m_nCameraBrightness));
     
    switch(pMe->m_nCameraBrightness)
    {
        case OEMNV_CAMERA_BRIGHTNESS_LEVEL1:
            nResID[8] = IDI_BRIGHTNESS_LEVEL1;
            break;

        case OEMNV_CAMERA_BRIGHTNESS_LEVEL2:
            nResID[8] = IDI_BRIGHTNESS_LEVEL2;
            break;

        case OEMNV_CAMERA_BRIGHTNESS_LEVEL3:
            nResID[8] = IDI_BRIGHTNESS_LEVEL3;
            break;

        case OEMNV_CAMERA_BRIGHTNESS_LEVEL4:
            nResID[8] = IDI_BRIGHTNESS_LEVEL4;
            break;

        case OEMNV_CAMERA_BRIGHTNESS_LEVEL5:
            nResID[8] = IDI_BRIGHTNESS_LEVEL5;
            break;
            
        default:
            break;
    }

    // zoom cfgID
    switch(pMe->m_nCameraZoom)
    {
        case OEMNV_CAMERA_ZOOM_LEVEL1:
            nResID[9] = IDI_ZOOM_1;
            break;

        case OEMNV_CAMERA_BRIGHTNESS_LEVEL2:
            nResID[9] = IDI_ZOOM_2;
            break;

        case OEMNV_CAMERA_BRIGHTNESS_LEVEL3:
            nResID[9] = IDI_ZOOM_3;
            break;

        case OEMNV_CAMERA_BRIGHTNESS_LEVEL4:
            nResID[9] = IDI_ZOOM_4;
            break;

        case OEMNV_CAMERA_BRIGHTNESS_LEVEL5:
            nResID[9] = IDI_ZOOM_5;
            break;
            
        default:
            break;
    }

    for(i = 0; i < 10; i++)
    {
        pTopBarImage = ISHELL_LoadResImage(pMe->m_pShell, 
                                           CAMERAAPP_IMAGE_RES_FILE, 
                                           nResID[i]);
        if(pTopBarImage)
        {
            IIMAGE_Draw(pTopBarImage, i*TOPBAR_ICON_WIDTH + 5, TOPBAR_ICON_Y);
            IIMAGE_Release(pTopBarImage);
            pTopBarImage = NULL;
        }
    }

}

static void CameraApp_DrawCCFGBar(CCameraApp * pMe)
{
    IImage   *pCameraCFGBarBg = NULL;    
    IImage   *pCameraCFGScrollBar = NULL;
    IImage   *pCameraCFGChooseIcon = NULL;
    IImage   *pCameraCFGBottomBar = NULL; 
    
#ifdef FEATURE_CARRIER_CHINA_VERTU
    pCameraCFGBarBg = ISHELL_LoadResImage(pMe->m_pShell,
                                          CAMERAAPP_IMAGE_RES_FILE,
                                          IDI_SCROLLBAR_BG_VERTU);
#else    
    pCameraCFGBarBg = ISHELL_LoadResImage(pMe->m_pShell,
                                          CAMERAAPP_IMAGE_RES_FILE,
                                          IDI_SCROLLBAR_BG);
#endif

    pCameraCFGScrollBar = ISHELL_LoadResImage(pMe->m_pShell,
                                              CAMERAAPP_IMAGE_RES_FILE,
                                              IDI_SCROLLBAR);

    pCameraCFGChooseIcon = ISHELL_LoadResImage(pMe->m_pShell,
                                               CAMERAAPP_IMAGE_RES_FILE,
                                               IDI_CHOOSE_REC);

    pCameraCFGBottomBar = ISHELL_LoadResImage(pMe->m_pShell,
                                              CAMERAAPP_IMAGE_RES_FILE,
                                              IDI_CCFG_BAR);

    if(pCameraCFGBarBg)
    {
        if(pMe->m_nCameraSize == OEMNV_CAMERA_SIZE_176_220)
        {
            CameraApp_DrawBarBg(pMe, IDI_LEFT_ARROW, 0, CFGBAR_EFFECT_TEXT_Y);        
            CameraApp_DrawBarBg(pMe, IDI_RIGHT_ARROW, 168, CFGBAR_EFFECT_TEXT_Y);
        }
        else
        {       
            IIMAGE_Draw(pCameraCFGBarBg, 0, 160); 
            IIMAGE_Release(pCameraCFGBarBg);
            pCameraCFGBarBg = NULL;
        }
    }

    if(pCameraCFGScrollBar)
    {
        if(pMe->m_nCFGBarChoosedID > 2)
        {
            IIMAGE_Draw(pCameraCFGScrollBar, SCROLLBAR_X, 160); 
        }
        else
        {
            IIMAGE_Draw(pCameraCFGScrollBar, 0, 160);         
        }
        
        IIMAGE_Release(pCameraCFGScrollBar);
        pCameraCFGScrollBar = NULL;
    }

    if(pCameraCFGChooseIcon)
    {      
        IIMAGE_Draw(pCameraCFGChooseIcon, (10+27*(pMe->m_nCFGBarChoosedID)), 176); 
        IIMAGE_Release(pCameraCFGChooseIcon);
        pCameraCFGChooseIcon = NULL;
    }
    
    if(pCameraCFGBottomBar)
    {
    
        IIMAGE_SetDrawSize(pCameraCFGBottomBar, 160, 22);
        IIMAGE_SetOffset(pCameraCFGBottomBar, 27*(pMe->m_nCFGBarOffset), 0);
        IIMAGE_Draw(pCameraCFGBottomBar, 10, 176);     
        IIMAGE_Release(pCameraCFGBottomBar);
        pCameraCFGBottomBar = NULL;
    }
}

/*static void CameraApp_DrawpopMenuBg(CCameraApp *pMe, IMenuCtl *popMenu)
{
    AEERect  mRec;
    IImage   *popMenuBgImg = NULL; 

    //draw popMenu bg
    popMenuBgImg = ISHELL_LoadResImage(pMe->m_pShell,
                                       CAMERAAPP_IMAGE_RES_FILE,
                                       IDI_POPMENU_BG);
    
    IMENUCTL_GetRect(popMenu, &mRec);
    IIMAGE_SetDrawSize(popMenuBgImg, mRec.dx, mRec.dy);
    IIMAGE_Draw(popMenuBgImg, mRec.x, mRec.y);
    if(popMenuBgImg)
    {
        IIMAGE_Release(popMenuBgImg);
        popMenuBgImg = NULL;
    }     
}*/

static void CameraApp_DrawMidPic(CCameraApp *pMe)
{
    IImage *pImage = NULL;
    switch(pMe->m_pActiveDlgID)
    {
        case IDD_CPREVIEW:
            pImage = ISHELL_LoadResImage(pMe->m_pShell,
                                         CAMERAAPP_IMAGE_RES_FILE,
                                         IDI_MID_CAMERA);
            if(pImage != NULL)
            {
                IIMAGE_Draw(pImage, 78, pMe->m_cyHeight - 22);
                IIMAGE_Release(pImage);
                pImage = NULL;
            }
            break;

        case IDD_CVIDEO:
            pImage = ISHELL_LoadResImage(pMe->m_pShell,
                                         CAMERAAPP_IMAGE_RES_FILE,                                         
                                         IDI_MID_VIDEO);
            if(pImage != NULL)
            {
                IIMAGE_Draw(pImage, 78, pMe->m_cyHeight - 22);
                IIMAGE_Release(pImage);
                pImage = NULL;
            }
            break;

        default:
            break;
    }
}

static void CameraApp_SetCameraFunFrame(CCameraApp *pMe)
{
    IImage *pImage = NULL;
    int16  nFrameID = 0;

    //previewʱ���������Сͼ176*220����Ҫ��ȡframe�����ã�����ʾframe
    if((pMe->m_pActiveDlgID == IDD_CPREVIEW) || (pMe->m_pActiveDlgID == IDD_CCAMERACFG))
    {
        (void)ICONFIG_GetItem(pMe->m_pConfig, 
                              CFGI_CAMERA_FRAME, 
                              &pMe->m_nCameraFrame, 
                              sizeof(pMe->m_nCameraFrame));        
    }
    
    switch(pMe->m_nCameraFrame)
    {
        case OEMNV_CAMERA_FRAME_0:
            break;

        case OEMNV_CAMERA_FRAME_1:
            nFrameID = IDI_FRAME_1;
            break;
           /* 
        case OEMNV_CAMERA_FRAME_2:
            nFrameID = IDI_FRAME_2;
            break;

        case OEMNV_CAMERA_FRAME_3:
            nFrameID = IDI_FRAME_3;
            break;

        case OEMNV_CAMERA_FRAME_4:
            nFrameID = IDI_FRAME_4;
            break;

        case OEMNV_CAMERA_FRAME_5:
            nFrameID = IDI_FRAME_5;
            break;

        case OEMNV_CAMERA_FRAME_6:
            nFrameID = IDI_FRAME_6;
            break;

        case OEMNV_CAMERA_FRAME_7:
            nFrameID = IDI_FRAME_7;
            break;

        case OEMNV_CAMERA_FRAME_8:
            nFrameID = IDI_FRAME_8;
            break;  */                         
    }

    if(nFrameID != 0)
    {
        pImage = ISHELL_LoadResImage(pMe->m_pShell, 
                                     CAMERAAPP_IMAGE_RES_FILE,
                                     nFrameID);

        if(pImage)
        {
            IIMAGE_Draw(pImage, 0, 0);
            IIMAGE_Release(pImage);
            pImage = NULL;
        }
    }   
}

static void CameraApp_DrawBarBg(CCameraApp *pMe, int16 nResID, int x, int y)
{
    IImage *pImageBg = NULL;

    pImageBg = ISHELL_LoadResImage(pMe->m_pShell, 
                                   CAMERAAPP_IMAGE_RES_FILE,
                                   nResID);

    if(pImageBg)
    {
        IIMAGE_Draw(pImageBg, x, y);
        IIMAGE_Release(pImageBg);
        pImageBg = NULL;
    }   
}

static void CameraApp_DrawVCFGBar(CCameraApp *pMe)
{
    IImage   *pCFGBarBg = NULL;    
    IImage   *pCFGScrollBar = NULL;
    IImage   *pCFGChooseIcon = NULL;
    IImage   *pCFGBottomBar = NULL; 
    
#ifdef FEATURE_CARRIER_CHINA_VERTU
    pCFGBarBg = ISHELL_LoadResImage(pMe->m_pShell,
                                    CAMERAAPP_IMAGE_RES_FILE,
                                    IDI_SCROLLBAR_BG_VERTU);
#else    
    pCFGBarBg = ISHELL_LoadResImage(pMe->m_pShell,
                                    CAMERAAPP_IMAGE_RES_FILE,
                                    IDI_SCROLLBAR_BG);
#endif

    pCFGScrollBar = ISHELL_LoadResImage(pMe->m_pShell,
                                        CAMERAAPP_IMAGE_RES_FILE,
                                        IDI_SCROLLBAR);

    pCFGChooseIcon = ISHELL_LoadResImage(pMe->m_pShell,
                                         CAMERAAPP_IMAGE_RES_FILE,
                                         IDI_CHOOSE_REC);

    pCFGBottomBar = ISHELL_LoadResImage(pMe->m_pShell,
                                        CAMERAAPP_IMAGE_RES_FILE,
                                        IDI_VCFG_BAR);

    if(pCFGBarBg)
    {      
        IIMAGE_Draw(pCFGBarBg, 0, 160); 
        IIMAGE_Release(pCFGBarBg);
        pCFGBarBg = NULL;
    }

    if(pCFGScrollBar)
    {
        if(pMe->m_nVideoCFG > 2)
        {
            IIMAGE_Draw(pCFGScrollBar, SCROLLBAR_X, 160); 
        }
        else
        {
            IIMAGE_Draw(pCFGScrollBar, 0, 160);         
        }
        
        IIMAGE_Release(pCFGScrollBar);
        pCFGScrollBar = NULL;
    }
    

    if(pCFGChooseIcon)
    {      
        IIMAGE_Draw(pCFGChooseIcon, (15+31*(pMe->m_nVideoCFG)), 176); 
        IIMAGE_Release(pCFGChooseIcon);
        pCFGChooseIcon = NULL;
    }
    
    if(pCFGBottomBar)
    {
    
        IIMAGE_Draw(pCFGBottomBar, 0, 176);     
        IIMAGE_Release(pCFGBottomBar);
        pCFGBottomBar = NULL;
    }
}

static uint16 CameraApp_SetPositonX(CCameraApp *pMe)
{
    uint16 X = 0;

    if(pMe->m_pActiveDlgID == IDD_CCAMERACFG)
    {
        if(pMe->m_nCFGBarChoosedID > 2)
        {
            X = SCROLLBAR_X;
        }
        else
        {
            X = 0;
        }
    }
    else
    {
        switch(pMe->m_nVideoCFG)
        {
            case VIDEOCFGCHOOSE:
                break;

            case VIDEOCFGENVIROMENT:
                break;

            case VIDEOCFGFUN:
                break;

            case VIDEOCFGRESET:
                X = SCROLLBAR_X;
                break;

            case VIDEOCFGHOTKEY:
                X = SCROLLBAR_X;
                break;

            default:
                break;
        }
    }

    return X;   
}

static void CamreaApp_DrawCFGPromptText(CCameraApp *pMe)
{  
    AEERect prc;
    int16   nResID = 0;       
    AECHAR  pcText[20]; 
    
    if(pMe->m_pActiveDlgID == IDD_CCAMERACFG)
    {
        switch(pMe->m_nCameraCFG)
        {
            case CAMERACFGCHOOSE:
                nResID = IDS_CFG_CHOOSE;
                break;
                
            case CAMERACFGENVIRMENT:
                nResID = IDS_CFG_ENVIR;
                break;
                
            case CAMERACFGFUN:
                nResID = IDS_CFG_FUN;
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
                
            case CAMERACFGCAPTURE:
                nResID = IDS_CFG_CAPMODE;
                break;
                
            case CAMERACFGBANDING:
                nResID = IDS_CFG_BANDING;
                break;
                
            case CAMERACFGSTORAGE:
                nResID = IDS_CFG_STORAGE;
                break;
                
            case CAMERACFGRESET:
                nResID = IDS_CFG_RESET;
                break;
                
            case CAMERACFGHOTKEY:
                nResID = IDS_CFG_HOTKEY;
                break;

            default:
                break; 
        }
         if(pMe->m_nCFGBarChoosedID > 2)
         {
            SETAEERECT(&prc, 59, 162, 118, 16);
         }
         else
         {
            SETAEERECT(&prc, 0, 162, 118, 16);
         }
    }
    else
    {
        switch(pMe->m_nVideoCFG)
        {
            case VIDEOCFGCHOOSE:
                nResID = IDS_CFG_CHOOSE;
                SETAEERECT(&prc, 0, 162, 118, 16);
                break;

            case VIDEOCFGENVIROMENT:
                nResID = IDS_CFG_ENVIR;
                SETAEERECT(&prc, 0, 162, 118, 16);
                break;

            case VIDEOCFGFUN:
                nResID = IDS_CFG_FUN;
                SETAEERECT(&prc, 0, 162, 118, 16);
                break;

            case VIDEOCFGRESET:
                nResID = IDS_CFG_RESET;
                SETAEERECT(&prc, 59, 162, 118, 16);
                break;

            case VIDEOCFGHOTKEY:
                nResID = IDS_CFG_HOTKEY;
                SETAEERECT(&prc, 59, 162, 118, 16);
                break;                                                                   
        }
    }

    MEMSET(pcText, 0, sizeof(pcText)); 
    (void)ISHELL_LoadResString(pMe->m_pShell, 
                               AEE_APPSCAMERAAPP_RES_FILE,       
                               nResID, 
                               pcText, 
                               sizeof(pcText));     

    if(pMe->m_nCameraSize != OEMNV_CAMERA_SIZE_176_220|| (pMe->m_pActiveDlgID == IDD_CVIDEOCFG))
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
                            IDF_ALIGN_CENTER|IDF_ALIGN_MIDDLE|IDF_TEXT_TRANSPARENT);

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
                            IDF_ALIGN_CENTER|IDF_ALIGN_MIDDLE|IDF_TEXT_TRANSPARENT);

        (void)IDISPLAY_SetColor(pMe->m_pDisplay, CLR_USER_TEXT, RGB_WHITE);
    }
}

static void CameraApp_DrawVideoTopBar(CCameraApp *pMe)
{
    int i;
    int16  nResID[2];
    IImage *pTopBarImage = NULL; 
          
    //enviroment cfgID
    (void)ICONFIG_GetItem(pMe->m_pConfig,
                          CFGI_VIDEO_ENVIROMENT,
                          &pMe->m_nCameraEnviroment,
                          sizeof(pMe->m_nCameraEnviroment));
    
    switch(pMe->m_nCameraEnviroment)
    {
        case OEMNV_CAMERA_ENVIR_AUTO:
            nResID[0] = IDI_ENV_AUTO;
            break;
            
        case OEMNV_CAMERA_ENVIR_SUNNY:
            nResID[0] = IDI_ENV_SUNNY;
            break;

        case OEMNV_CAMERA_ENVIR_CLOUDY:
            nResID[0] = IDI_ENV_CLOUDY;
            break;

        case OEMNV_CAMERA_ENVIR_NIGHT:
            nResID[0] = IDI_ENV_NIGHT;
            break;
            
       default:
            break;
    }

    //brightness cfgID
    (void)ICONFIG_GetItem(pMe->m_pConfig,
                          CFGI_VIDEO_BRIGHTNESS,
                          &pMe->m_nCameraBrightness,
                          sizeof(pMe->m_nCameraBrightness));
    
    switch(pMe->m_nCameraBrightness)
    {
        case OEMNV_CAMERA_BRIGHTNESS_LEVEL1:
            nResID[1] = IDI_BRIGHTNESS_LEVEL1;
            break;

        case OEMNV_CAMERA_BRIGHTNESS_LEVEL2:
            nResID[1] = IDI_BRIGHTNESS_LEVEL2;
            break;

        case OEMNV_CAMERA_BRIGHTNESS_LEVEL3:
            nResID[1] = IDI_BRIGHTNESS_LEVEL3;
            break;

        case OEMNV_CAMERA_BRIGHTNESS_LEVEL4:
            nResID[1] = IDI_BRIGHTNESS_LEVEL4;
            break;

        case OEMNV_CAMERA_BRIGHTNESS_LEVEL5:
            nResID[1] = IDI_BRIGHTNESS_LEVEL5;
            break;
            
        default:
            break;
    }

    for(i = 0; i < 2; i++)
    {
        pTopBarImage = ISHELL_LoadResImage(pMe->m_pShell, 
                                           CAMERAAPP_IMAGE_RES_FILE, 
                                           nResID[i]);
        if(pTopBarImage)
        {
            IIMAGE_Draw(pTopBarImage, i*TOPBAR_ICON_WIDTH + 5, TOPBAR_ICON_Y);
            IIMAGE_Release(pTopBarImage);
            pTopBarImage = NULL;
        }
    }
   
}

#ifndef FEATURE_CAMERAAPP_FUN_FRAME
static void CameraApp_HandlePopMenuEffectCFG(CCameraApp *pMe, IMenuCtl *popMenu, boolean bHandleEffect)
{
    uint16 menuSel = 0;

    int32 color_effect = 0;

    menuSel = IMENUCTL_GetSel(popMenu);

    (void)ICONFIG_GetItem(pMe->m_pConfig,
                          CFGI_CAMERA_COLOR,
                          &pMe->m_nCameraColor,
                          sizeof(pMe->m_nCameraColor)); 

    
    (void)ICONFIG_GetItem(pMe->m_pConfig,
                          CFGI_CAMERA_BANDING,
                          &pMe->m_nCameraBanding,
                          sizeof(pMe->m_nCameraBanding)); 
    
    if(bHandleEffect)
    {
        switch(pMe->m_nCameraCFG)
        {
            case CAMERACFGFUN:
                switch(menuSel)
                {
                    case IDS_NORMAL:
                        color_effect = OEMNV_CAMERA_COLOR_NORMAL;
                        break;

                    case IDS_COLOR_BANDW:
                        color_effect = OEMNV_CAMERA_COLOR_BANDW;
                        break;

                    case IDS_COLOR_SEPIA:
                        color_effect = OEMNV_CAMERA_COLOR_SEPIA;           
                        break;

                    case IDS_COLOR_NEGATIVE:
                        color_effect = OEMNV_CAMERA_COLOR_NEGATIVE;
                        break;

                    case IDS_COLOR_RED:
                        color_effect = OEMNV_CAMERA_COLOR_REDONLY;
                        break;

                    case IDS_COLOR_GREEN:
                        color_effect = OEMNV_CAMERA_COLOR_GREENONLY;
                        break;
                        
                    default:
                        break;                                   
                }
                ICAMERA_SetParm(pMe->m_pCamera, CAM_PARM_EFFECT, color_effect, 0);
                break;

            case CAMERACFGBANDING:
                switch(menuSel)
                {
                    case IDS_BANDING_MODE1:
                        break;
                        
                    case IDS_BANDING_MODE2:
                        break;              
        
                    default:
                        break;                                   
                }

                break;

            default:
                break;
        }
    }
    else
    {
        ICAMERA_SetParm(pMe->m_pCamera, CAM_PARM_EFFECT, pMe->m_nCameraColor, 0);      
    }       
}
#endif

static void CameraApp_CPreviewStart(CCameraApp *pMe)
{
    AEESize displaySize, captureSize; 
    int16 quality = 0;
                                       
    // set camera size
    switch(pMe->m_nCameraSize)
    {
        case OEMNV_CAMERA_SIZE_176_220:
            captureSize.cx = 176;
            captureSize.cy = 220;
            break;

        case OEMNV_CAMERA_SIZE_320_240:
            captureSize.cx = 320;
            captureSize.cy = 240;
            break;

        case OEMNV_CAMERA_SIZE_640_480:
            captureSize.cx = 640;
            captureSize.cy = 480;
            break;

        case OEMNV_CAMERA_SIZE_1280_960:
            captureSize.cx = 1280;
            captureSize.cy = 960;
            break;

        case OEMNV_CAMERA_SIZE_1600_1200:
            captureSize.cx = 1600;
            captureSize.cy = 1200;
            break;

        default:
            break;                    
    }

    if(pMe->m_nCameraSize == OEMNV_CAMERA_SIZE_176_220)
    {
        displaySize.cx = 176;
        displaySize.cy = 220;

        ICAMERA_SetParm(pMe->m_pCamera, CAM_PARM_DISPLAY_OFFSET, 0, 0);
        ICAMERA_SetParm(pMe->m_pCamera, CAM_PARM_PREVIEWWITHFRAME, 1, 0);

    }
    else
    {
        displaySize.cx = 176;
        displaySize.cy = 132;

        ICAMERA_SetParm(pMe->m_pCamera, CAM_PARM_DISPLAY_OFFSET, 0, 28);
        ICAMERA_SetParm(pMe->m_pCamera, CAM_PARM_PREVIEWWITHFRAME, 0, 0);

    }

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
    
    ICAMERA_SetParm(pMe->m_pCamera, CAM_PARM_PREVIEW_TYPE, CAM_PREVIEW_SNAPSHOT, 0);       
    ICAMERA_SetParm(pMe->m_pCamera, CAM_PARM_EFFECT, pMe->m_nCameraColor, 0);
    ICAMERA_SetParm(pMe->m_pCamera, CAM_PARM_MULTISHOT, 1, 0);
    ICAMERA_SetQuality(pMe->m_pCamera, quality);    
    ICAMERA_SetSize(pMe->m_pCamera, &captureSize);
    ICAMERA_SetDisplaySize(pMe->m_pCamera, &displaySize);         

    //ICAMERA_RegisterNotify(pMe->m_pCamera, (PFNCAMERANOTIFY)CameraApp_CNotify, pMe);
                  
    /*if(SUCCESS == ICAMERA_Preview(pMe->m_pCamera))
    {
        IImage_Stop(pMe->m_pImage);
    }*/  
    
    ICAMERA_Preview(pMe->m_pCamera); 

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
    if(pMe->m_nCameraStorage == OEMNV_CAMERA_STORAGE_MEMORY_CARD)
    {
        STRCPY(sFileName, pMe->m_sCurrentFileName+STRLEN(MG_MASSCARDPICTURE_PATH));
    }
    else
    {
        STRCPY(sFileName, pMe->m_sCurrentFileName+STRLEN(MG_PHONEPICTURE_PATH));
    }

    DBGPRINTF("sFileName = %s",sFileName);

    // copy the pic name to m_sCaptureFileName, this is for capture mode
    STRCPY(pMe->m_sCaptureFileName+STRLEN(pMe->m_sCaptureFileName), sFileName);

    DBGPRINTF("m_sCaptureFileName = %s",pMe->m_sCaptureFileName);
        
    if(pMe->m_nCameraSize == OEMNV_CAMERA_SIZE_176_220)
    {
        IDisplay_SetClipRect(pMe->m_pDisplay, 0);
        
        // ��������ʱ������һ������ʱ�������������α������
        if(pMe->m_bCleanScreen)
        {
            IDisplay_ClearScreen(pMe->m_pDisplay);
            pMe->m_bCleanScreen = FALSE;
        }
        CameraApp_SetCameraFunFrame(pMe);

        IDisplay_UpdateEx(pMe->m_pDisplay, FALSE);
    }

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

    pMe->m_nCameraState = CAM_CAPTURE;
    pMe->m_bCapturePic = TRUE;

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

    pMe->m_nSnapShotTimes--;

    if(pMe->m_nSnapShotTimes == 0)
    {
        pMe->m_bCapturePic = FALSE;
        CLOSE_DIALOG(DLGRET_PICMENU);  
    }
    else
    {
        ICAMERA_Preview(pMe->m_pCamera);        

        pMe->m_nCameraState = CAM_PREVIEW;

        CameraApp_SetParamAfterPreview(pMe);
        
        (void)ISHELL_SetTimer(pMe->m_pShell,
                              300,
                              (PFNNOTIFY)CameraApp_RecordSnapShot,
                              pMe); 
    }
}

static void CameraApp_HandleColorEffectCFG(CCameraApp *pMe)
{
    OEMCAMERACOLOR color_effect;

    switch(pMe->m_ePreState)
    {
        case STATE_CCAMERACFG:
            (void)ICONFIG_GetItem(pMe->m_pConfig,
                                  CFGI_CAMERA_COLOR,
                                  &color_effect,
                                  sizeof(color_effect));
            break;

        case STATE_CVIDEOCFG:
            (void)ICONFIG_GetItem(pMe->m_pConfig,
                                  CFGI_VIDEO_COLOR,
                                  &color_effect,
                                  sizeof(color_effect));
            break;

        default:
            break;   
    }
 
    ICAMERA_SetParm(pMe->m_pCamera, CAM_PARM_EFFECT, color_effect, 0);
}

static void CameraApp_SetEffectColor(CCameraApp *pMe, int color)
{   
    // set camera/video color effect
    OEMCAMERACOLOR color_effect = color;
    
    switch(pMe->m_ePreState)
    {
        case STATE_CCAMERACFG:
            (void)ICONFIG_SetItem(pMe->m_pConfig,
                                  CFGI_CAMERA_COLOR,
                                  &color_effect,
                                  sizeof(color_effect));
            break;

        case STATE_CVIDEOCFG:
            (void)ICONFIG_SetItem(pMe->m_pConfig,
                                  CFGI_VIDEO_COLOR,
                                  &color_effect,
                                  sizeof(color_effect));
            break;

        default:
            break;   
    }
}

static void CameraApp_DrawBarText(CCameraApp *pMe, int16 nResID, int postionY)
{
    AECHAR  pcText[20];
    
    MEMSET(pcText, 0, sizeof(pcText));
    
    (void)ISHELL_LoadResString(pMe->m_pShell, 
                               AEE_APPSCAMERAAPP_RES_FILE,       
                               nResID, 
                               pcText, 
                               sizeof(pcText)); 
    
    (void)IDISPLAY_SetColor(pMe->m_pDisplay, CLR_USER_TEXT, WHITE_TEXT);

    DrawTextWithProfile(pMe->m_pShell, 
                        pMe->m_pDisplay, 
                        RGB_BLACK, 
                        AEE_FONT_NORMAL, 
                        pcText, 
                        -1, 
                        0, 
                        postionY, 
                        NULL, 
                        IDF_ALIGN_CENTER|IDF_TEXT_TRANSPARENT);

    
    (void)IDISPLAY_SetColor(pMe->m_pDisplay, CLR_USER_TEXT, RGB_WHITE);
}

static void CameraApp_DrawColorPromptText(CCameraApp *pMe)
{
    int16 nResID = 0;

    nResID = CameraApp_GetColorEffectResID(pMe);
    
    CameraApp_DrawBarText(pMe, nResID, CFGBAR_EFFECT_TEXT_Y); 
}

static void CameraApp_DrawFramePromptText(CCameraApp *pMe)
{
    AECHAR  pcText[20];
    AECHAR  pcNum[2];
    int     n = 0;
    int16   nResID = 0;     

    if(pMe->m_nCameraFrame == OEMNV_CAMERA_FRAME_0)
    {
        nResID = IDS_NORMAL;
    }
    else
    {
        nResID = IDS_FRAME;
    }
    
    MEMSET(pcText, 0, sizeof(pcText));
    MEMSET(pcNum, 0 , sizeof(pcNum));
    
    n = ISHELL_LoadResString(pMe->m_pShell, 
                               AEE_APPSCAMERAAPP_RES_FILE,       
                               nResID, 
                               pcText, 
                               sizeof(pcText));

    WSPRINTF(pcNum,
             sizeof(pcNum),
             L"%u",
             pMe->m_nCameraFrame);

    WSTRCPY(pcText + n, pcNum);
    
    (void)IDISPLAY_SetColor(pMe->m_pDisplay, CLR_USER_TEXT, WHITE_TEXT);

    if(pMe->m_nCameraFrame == OEMNV_CAMERA_FRAME_0)
    {
        CameraApp_DrawBarText(pMe, IDS_NORMAL, CFGBAR_EFFECT_TEXT_Y);
    }
    else
    {
        DrawTextWithProfile(pMe->m_pShell, 
                            pMe->m_pDisplay, 
                            RGB_BLACK, 
                            AEE_FONT_NORMAL, 
                            pcText, 
                            -1, 
                            0, 
                            CFGBAR_EFFECT_TEXT_Y, 
                            NULL, 
                            IDF_ALIGN_CENTER|IDF_TEXT_TRANSPARENT);
    }

    (void)IDISPLAY_SetColor(pMe->m_pDisplay, CLR_USER_TEXT, RGB_WHITE);
} 

static int CameraApp_ColorEffectSel(CCameraApp *pMe)
{
    switch(pMe->m_nCameraColor)
    {
        case OEMNV_CAMERA_COLOR_NORMAL:          
            pMe->m_nColorSel = COLORNORMAL;
            break;

        case OEMNV_CAMERA_COLOR_BANDW:          
            pMe->m_nColorSel = COLORBANDW;
            break; 

        case OEMNV_CAMERA_COLOR_SEPIA:          
            pMe->m_nColorSel = COLORSEPIA;
            break;

        case OEMNV_CAMERA_COLOR_NEGATIVE:          
            pMe->m_nColorSel = COLORNEGATIVE;
            break;

        case OEMNV_CAMERA_COLOR_REDONLY:          
            pMe->m_nColorSel = COLORRED;
            break;

        case OEMNV_CAMERA_COLOR_GREENONLY:          
            pMe->m_nColorSel = COLORGREEN;
            break;

        default:
            break;
    }

    return pMe->m_nColorSel;
}

static int CameraApp_SetColorEffect(CCameraApp *pMe)
{
    switch(pMe->m_nColorSel)
    {
        case COLORNORMAL:          
            pMe->m_nCameraColor = OEMNV_CAMERA_COLOR_NORMAL;
            break;

        case COLORBANDW:          
            pMe->m_nCameraColor = OEMNV_CAMERA_COLOR_BANDW;
            break; 

        case COLORSEPIA:          
            pMe->m_nCameraColor = OEMNV_CAMERA_COLOR_SEPIA;
            break;

        case COLORNEGATIVE:          
            pMe->m_nCameraColor = OEMNV_CAMERA_COLOR_NEGATIVE;
            break;

        case COLORRED:          
            pMe->m_nCameraColor = OEMNV_CAMERA_COLOR_REDONLY;
            break;

        case COLORGREEN:          
            pMe->m_nCameraColor = OEMNV_CAMERA_COLOR_GREENONLY;
            break;

        default:
            break;
    }

    return pMe->m_nCameraColor;
}

static int16 CameraApp_GetColorEffectResID(CCameraApp *pMe)
{
    int16 nResID = 0;
    
    switch(pMe->m_nCameraColor)
    {      
        case OEMNV_CAMERA_COLOR_NORMAL: 
            nResID = IDS_NORMAL;    
            break;

        case OEMNV_CAMERA_COLOR_BANDW:
            nResID = IDS_COLOR_BANDW;
            break;

        case OEMNV_CAMERA_COLOR_SEPIA:
            nResID = IDS_COLOR_SEPIA;
            break;

        case OEMNV_CAMERA_COLOR_NEGATIVE:
            nResID = IDS_COLOR_NEGATIVE;
            break;

        case OEMNV_CAMERA_COLOR_REDONLY:
            nResID = IDS_COLOR_RED;
            break;

        case OEMNV_CAMERA_COLOR_GREENONLY:
            nResID = IDS_COLOR_GREEN;
            break;
  
        default:
            break;
    }

    return nResID;
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
    #ifdef FEATURE_SUPPORT_VC0848
    static vc_union_type vc_data;
    VC_DeviceControl(VC_ITM_SD_STAT_I, VC_FUNC_GET_PARM, &vc_data);
    
    if( vc_data.sd == VC_SD_FIND_ERR)
    {
       return FALSE;
    }
    else
    {
        return TRUE;
    }
    #else
    return FALSE;
    #endif
}


// camera preview��û�ҵ��洢���Ĵ���
static boolean CameraApp_PreviewFindNoMemoryCardExistHandle(CCameraApp *pMe)
{
    if(!pMe->m_bMemoryCardExist)
    {
        pMe->m_wMsgID = IDS_MSG_NOSDCARD;
        pMe->m_nMsgTimeout = TIMEOUT_MS_MSGBOX;
        ICAMERA_Stop(pMe->m_pCamera);
        pMe->m_nCameraState = CAM_STOP;
        pMe->m_bRePreview = TRUE;
        IDisplay_SetClipRect(pMe->m_pDisplay, 0);
        CLOSE_DIALOG(DLGRET_POPMSG);
        return TRUE;
    }

    return FALSE;
}

// ������գ������Ƿ����㹻ʣ��ռ�����
static boolean CameraApp_IsEnoughfMemorySpace(CCameraApp * pMe)
{
    if((pMe->m_pActiveDlgID == IDD_CVIDEO) || (pMe->m_nCameraStorage == OEMNV_CAMERA_STORAGE_MEMORY_CARD))
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


// ����
static boolean CameraApp_SelfTimeRecordSnapShot(CCameraApp *pMe)
{
    // ����ʱ�������Ļ
    IDisplay_ClearScreen(pMe->m_pDisplay);

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
            CameraApp_RecordSnapShot(pMe);
        }

        return TRUE;
    }
    else
    {  
        IImage *pImage = NULL;
        
        pMe->m_nLeftTime--;
        
        pImage = ISHELL_LoadResImage(pMe->m_pShell, CAMERAAPP_IMAGE_RES_FILE, IDI_COUNT_DOWN);

        if(pImage)
        {
            IImage_SetDrawSize(pImage, 60, 60);
            
            IImage_SetOffset(pImage, (pMe->m_nLeftTime)*60, 0);

            IImage_Draw(pImage, 58, 60);

            IImage_Release(pImage);

            pImage = NULL;

            IDisplay_UpdateEx(pMe->m_pDisplay, FALSE);
        }

        (void)ISHELL_SetTimer(pMe->m_pShell,
                              1000,
                              (PFNNOTIFY)CameraApp_SelfTimeRecordSnapShot,
                              pMe);
    }

    return TRUE;
}

// Draw HotKey Text With Frame
static void CameraApp_DrawHotKeyTextWithFrame(CCameraApp *pMe)
{
    AEERect topRec, bottomRec, leftRec, rightRec, midRec;
    
    SETAEERECT(&topRec, 30, 73, 116, 40);
    SETAEERECT(&bottomRec, 30, 75, 116, 40);
    SETAEERECT(&leftRec, 29, 74, 116, 40);
    SETAEERECT(&rightRec, 33, 74, 116, 40);
    SETAEERECT(&midRec, 30, 74, 116, 40);

    IDisplay_SetColor(pMe->m_pDisplay, CLR_USER_TEXT, RGB_BLACK);
     
    ISTATIC_SetText(pMe->m_pStatic, NULL, pMe->m_szHotKeyTextBuf, NULL, AEE_FONT_NORMAL); 
     
    //draw top static text
    ISTATIC_SetRect(pMe->m_pStatic, &topRec);   
    ISTATIC_Redraw(pMe->m_pStatic);

    //draw bottom static text
    ISTATIC_SetRect(pMe->m_pStatic, &bottomRec);
    ISTATIC_Redraw(pMe->m_pStatic);

    //draw left static text
    ISTATIC_SetRect(pMe->m_pStatic, &leftRec);    
    ISTATIC_Redraw(pMe->m_pStatic);

    //draw right static text
    ISTATIC_SetRect(pMe->m_pStatic, &leftRec);
    ISTATIC_Redraw(pMe->m_pStatic);

    IDisplay_SetColor(pMe->m_pDisplay, CLR_USER_TEXT, WHITE_TEXT);

    //draw mid static text
    ISTATIC_SetRect(pMe->m_pStatic, &midRec);   
    ISTATIC_Redraw(pMe->m_pStatic); 

    IDisplay_SetColor(pMe->m_pDisplay, CLR_USER_TEXT, RGB_WHITE);
}

// Draw Bottom Bar Text
static void CameraApp_DrawBottomBarText(CCameraApp *pMe, BottomBar_e_Type eBarType)
{
    uint16      nResID_L = 0;   // left
    uint16      nResID_R = 0;   // right
    uint16      nResID_M = 0;   // mid
    AEERect     rc;
    AECHAR      wszBar_L[20]={0};// left
    AECHAR      wszBar_R[20]={0};// right
    AECHAR      wszBar_M[20]={0};// mid
    
    SETAEERECT(&rc, 5, 198, 166, 22);
    
    switch(eBarType)
    {
        case BTBAR_SELECT_BACK:
            nResID_L = IDS_SELECT;
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
                            IDF_ALIGN_BOTTOM|IDF_ALIGN_LEFT|IDF_TEXT_TRANSPARENT);
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

   DBGPRINTF("pszDest = %s",pszDest);
 
   switch(fileType)
   {
       default:
       case FILE_TYPE_JPG:
           STRCAT(pszDest, FS_CARD_PICTURES_FOLDER);          
           SPRINTF(pszDest+STRLEN(pszDest), "%02d%02d%02d%02d.jpg", julian.wDay, julian.wHour, julian.wMinute, julian.wSecond);
           break;
 
       case FILE_TYPE_AMR:
           STRCAT(pszDest, FS_CARD_MUSIC_FOLDER);            
           SPRINTF(pszDest+STRLEN(pszDest), "%02d%02d%02d%02d.amr", julian.wDay, julian.wHour, julian.wMinute, julian.wSecond);
           break;
    
       case FILE_TYPE_3GP:
           STRCAT(pszDest, FS_CARD_VIDEOS_FOLDER); 
           SPRINTF(pszDest+STRLEN(pszDest), "%02d%02d%02d%02d.3gp", julian.wDay, julian.wHour, julian.wMinute, julian.wSecond);
       break;
   }
   return TRUE;
}

static void CameraApp_SetCameraCaptureSize(CCameraApp *pMe, uint16 wParam)
{
     AEESize displaySize, captureSize;
     
     pMe->m_nCameraZoom = OEMNV_CAMERA_ZOOM_LEVEL1;// ��������capture��С����������zoom
        
     switch(wParam)
     {
         case IDS_CAMERA_SIZE_176_220:   
             captureSize.cx = 176;
             captureSize.cy = 220;
             pMe->m_nCameraSize = OEMNV_CAMERA_SIZE_176_220;
             break;
 
         case IDS_CAMERA_SIZE_320_240:     
             captureSize.cx = 320;
             captureSize.cy = 240;
             pMe->m_nCameraSize = OEMNV_CAMERA_SIZE_320_240;
             break;
 
         case IDS_CAMERA_SIZE_640_480:    
             captureSize.cx = 640;
             captureSize.cy = 480;
             pMe->m_nCameraSize = OEMNV_CAMERA_SIZE_640_480;
             break;
 
         case IDS_CAMERA_SIZE_1280_960:            
             captureSize.cx = 1280;
             captureSize.cy = 960;
             pMe->m_nCameraSize = OEMNV_CAMERA_SIZE_1280_960;
             break;
 
         case IDS_CAMERA_SIZE_1600_1200:        
             captureSize.cx = 1600;
             captureSize.cy = 1200;
             pMe->m_nCameraSize = OEMNV_CAMERA_SIZE_1600_1200;
             break;
 
         default:
             break;
     }
     
     if(pMe->m_nCameraSize == OEMNV_CAMERA_SIZE_176_220)
     {
         displaySize.cx = 176;
         displaySize.cy = 220;
         ICAMERA_SetParm(pMe->m_pCamera, CAM_PARM_DISPLAY_OFFSET, 0, 0);

 
     }
     else
     {
         displaySize.cx = 176;
         displaySize.cy = 132;
         ICAMERA_SetParm(pMe->m_pCamera, CAM_PARM_DISPLAY_OFFSET, 0, 28);
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
    AEERect topRect,numTextRect;   
    char sfileName[MIN_PIC_CHAR_NAME_LEN];
    AECHAR wfileName[MIN_PIC_WCHAR_NAME_LEN];
    AECHAR numOfSnapshot[MIN_PIC_NUMOFSUM_LEN];
    int index = pMe->m_nSnapShotPicNameIndex;

    int x = index+1;
    int y = pMe->m_nSnapShotPicNum+index;
    
    SETAEERECT(&topRect, 0, 0, 176, 28);
    SETAEERECT(&numTextRect, 5, 160, 166, 22);
             
    MEMSET(sfileName, '\0', sizeof(sfileName));
    MEMSET(pMe->m_sFileNameWithAllPath, '\0', sizeof(pMe->m_sFileNameWithAllPath));
    MEMSET(wfileName, '\0', sizeof(wfileName));
    MEMSET(numOfSnapshot, '\0', sizeof(numOfSnapshot));

    DBGPRINTF("index = %d", index);
    
    STRLCPY(sfileName, (pMe->m_sCaptureFileName+((MIN_PIC_CHAR_NAME_LEN-1)*index)), MIN_PIC_CHAR_NAME_LEN);
    
    sfileName[MIN_PIC_CHAR_NAME_LEN-1] = '\0';
    
    DBGPRINTF("sfileName = %s", sfileName);

    STRTOWSTR(sfileName, wfileName, MIN_PIC_WCHAR_NAME_LEN);

    DBGPRINTF("wfileName = %s", wfileName);

    if(pMe->m_nCameraStorage == OEMNV_CAMERA_STORAGE_MEMORY_CARD)
    {
        STRCPY(pMe->m_sFileNameWithAllPath, FS_CARD_PICTURES_FOLDER);
    }
    else
    {
        STRCPY(pMe->m_sFileNameWithAllPath, MG_PHONEPICTURE_PATH);
    }   

    STRCPY(pMe->m_sFileNameWithAllPath+STRLEN(pMe->m_sFileNameWithAllPath), sfileName);

    DBGPRINTF("m_sFileNameWithAllPath = %s", pMe->m_sFileNameWithAllPath);

    WSPRINTF(numOfSnapshot,
             sizeof(numOfSnapshot),
             L"%d/%d",
             x,
             y);

    DBGPRINTF("numOfSnapshot = %S",numOfSnapshot);

    WSTRCPY(pMe->m_wFileName, wfileName);

    WSTRCPY(pMe->m_nNumOfSnapshot, numOfSnapshot);

    #if 1
    // jpg���룬ˢ�������浽�ֻ��ϣ���ȡqualcomm�Ľ��뷽ʽ���Ƚ��룬��ˢ��
    if(pMe->m_nCameraStorage == OEMNV_CAMERA_STORAGE_PHONE)
    {
        IImage *pIImage;
        
        jpg_decoder_done = FALSE;
        
        pIImage = ISHELL_LoadImage(pMe->m_pShell, pMe->m_sFileNameWithAllPath);
        
        if(NULL != pIImage) 
        { 
            IIMAGE_Notify(pIImage, CameraApp_JPGNotify, (void*)pMe);    
        }   
    }
    else
    {
        IDISPLAY_ClearScreen(pMe->m_pDisplay);
        
        if(pMe->m_nCameraSize != OEMNV_CAMERA_SIZE_176_220)
        {      
#ifdef FEATURE_CARRIER_CHINA_VERTU
            CameraApp_DrawBarBg(pMe, IDI_TOPBAR_BG_VERTU, 0, 0);           
            CameraApp_DrawBarBg(pMe, IDI_CFG_BG_VERTU, 0, 160);
#else        
            CameraApp_DrawBarBg(pMe, IDI_TOPBAR_BG, 0, 0);           
            CameraApp_DrawBarBg(pMe, IDI_CFG_BG, 0, 160);          
#endif			
            DRAW_BOTTOMBAR(BTBAR_SAVE_DELETE);               
        }
        else
        {  
            CameraApp_DrawBottomBarText(pMe, BTBAR_SAVE_DELETE);
        }
        
        IDisplay_SetColor(pMe->m_pDisplay, CLR_USER_TEXT, WHITE_TEXT);

        DrawTextWithProfile(pMe->m_pShell, 
                            pMe->m_pDisplay, 
                            RGB_BLACK, 
                            AEE_FONT_NORMAL, 
                            wfileName, 
                            -1, 
                            0, 
                            0, 
                            &topRect, 
                            IDF_ALIGN_CENTER|IDF_ALIGN_MIDDLE|IDF_TEXT_TRANSPARENT);

        DrawTextWithProfile(pMe->m_pShell, 
                            pMe->m_pDisplay, 
                            RGB_BLACK, 
                            AEE_FONT_NORMAL, 
                            numOfSnapshot, 
                            -1, 
                            0, 
                            0, 
                            &numTextRect, 
                            IDF_ALIGN_RIGHT|IDF_ALIGN_MIDDLE|IDF_TEXT_TRANSPARENT);

        jpgSize = pMe->m_nCameraSize;

        refreshJPG = TRUE;

        IDisplay_SetColor(pMe->m_pDisplay, CLR_USER_TEXT, RGB_WHITE); 
        
        IDisplay_UpdateEx(pMe->m_pDisplay, FALSE);
         
        refreshJPG = FALSE;

        CameraApp_DecodeJPG(pMe);
    }
    #endif
    
    pMe->m_nSnapShotPicNum--;
    
    pMe->m_nSnapShotPicNameIndex++;    
}

static void CameraApp_HandleVideoFile(CCameraApp *pMe)
{
    AEERect topRect,bottomRect;  
    char    sfileName[MIN_PIC_CHAR_NAME_LEN];
    AECHAR  wfileName[MIN_PIC_WCHAR_NAME_LEN];
    int     strlen = 0; 

    SETAEERECT(&topRect, 0, 0, 176, 28);
    SETAEERECT(&bottomRect, 0, 160, 176, 60);
  
    MEMSET(sfileName, '\0', sizeof(sfileName));
    MEMSET(wfileName, '\0', sizeof(wfileName));

    // copy the 3gp name to sfilename buffer without path name
    strlen = STRLEN(MG_MASSCARDVIDEO_PATH);
    STRCPY(sfileName, pMe->m_sCurrentFileName+strlen);

    // switch the type of 3gp name to wchar  
    STRTOWSTR(sfileName, wfileName, MIN_PIC_WCHAR_NAME_LEN); 
 
    // set first clip rect
    IDisplay_SetClipRect(pMe->m_pDisplay, &topRect); 
    
#ifdef FEATURE_CARRIER_CHINA_VERTU
    CameraApp_DrawBarBg(pMe, IDI_TOPBAR_BG_VERTU, 0, 0);
#else    
    CameraApp_DrawBarBg(pMe, IDI_TOPBAR_BG, 0, 0); 
#endif

    IDisplay_SetColor(pMe->m_pDisplay, CLR_USER_TEXT, WHITE_TEXT);

    DrawTextWithProfile(pMe->m_pShell, 
                        pMe->m_pDisplay, 
                        RGB_BLACK, 
                        AEE_FONT_NORMAL, 
                        wfileName, 
                        -1, 
                        0, 
                        0, 
                        &topRect, 
                        IDF_ALIGN_CENTER|IDF_ALIGN_MIDDLE|IDF_TEXT_TRANSPARENT);

    IDisplay_SetColor(pMe->m_pDisplay, CLR_USER_TEXT, RGB_WHITE);
    
    IDISPLAY_UpdateEx(pMe->m_pDisplay, FALSE);
    
    // set second clip rect
    IDisplay_SetClipRect(pMe->m_pDisplay, &bottomRect);   
    
    DRAW_BOTTOMBAR(BTBAR_SAVE_DELETE);
    
    IDISPLAY_UpdateEx(pMe->m_pDisplay, FALSE);

    IDisplay_SetClipRect(pMe->m_pDisplay, 0);
}

// VIM Jpg decode��ʽ
void CameraApp_DecodeJPG(CCameraApp *pMe)
{
    #ifdef FEATURE_SUPPORT_VC0848
    vc_union_type vc_data;
    
    vc_data.play_info.cam_size = pMe->m_nCameraSize;
    vc_data.play_info.entry = VC_JPEG_ENTRY_CAM;
    
    if(pMe->m_nCameraStorage == OEMNV_CAMERA_STORAGE_MEMORY_CARD)
    {
        vc_data.play_info.file_scr = VC_IN_CORE_FILE;
    }
    else
    {
         vc_data.play_info.file_scr = VC_IN_BASEBAND_RAM;
    }
    
    STRCPY((char *)&vc_data.play_info.szFileName, (char *)&pMe->m_sFileNameWithAllPath);
    
    DBGPRINTF("VC JPEG Decode:%s, file_src:%d", (char *)&vc_data.play_info.szFileName, vc_data.play_info.file_scr);
    
    VC_DeviceControl(VC_ITM_JPG_DECODE_I, VC_FUNC_PLAY_ON, &vc_data);
    #endif
}

// jpg decode�ص�����,176x220���㵽�ֻ���,jpg decode��ȡqualcomm��ʽ
void CameraApp_JPGNotify(void *po, IImage *pIImage, AEEImageInfo *pii, int nErr)
{   
    AEERect topRect,numTextRect; 

    CCameraApp *pMe = (CCameraApp *)po;

    SETAEERECT(&topRect, 0, 0, 176, 28);
    SETAEERECT(&numTextRect, 5, 160, 166, 22);
    
    if(SUCCESS == nErr) 
    {
         IIMAGE_Draw(pIImage, 0, 0);

         CameraApp_DrawBottomBarText(pMe, BTBAR_SAVE_DELETE);
         
         IDisplay_SetColor(pMe->m_pDisplay, CLR_USER_TEXT, WHITE_TEXT);
 
         DrawTextWithProfile(pMe->m_pShell, 
                             pMe->m_pDisplay, 
                             RGB_BLACK, 
                             AEE_FONT_NORMAL, 
                             pMe->m_wFileName, 
                             -1, 
                             0, 
                             0, 
                             &topRect, 
                             IDF_ALIGN_CENTER|IDF_ALIGN_MIDDLE|IDF_TEXT_TRANSPARENT);
 
         DrawTextWithProfile(pMe->m_pShell, 
                             pMe->m_pDisplay, 
                             RGB_BLACK, 
                             AEE_FONT_NORMAL, 
                             pMe->m_nNumOfSnapshot, 
                             -1, 
                             0, 
                             0, 
                             &numTextRect, 
                             IDF_ALIGN_RIGHT|IDF_ALIGN_MIDDLE|IDF_TEXT_TRANSPARENT);

        IDisplay_SetColor(pMe->m_pDisplay, CLR_USER_TEXT, RGB_WHITE);
 
        IDISPLAY_UpdateEx(pMe->m_pDisplay,FALSE);

        IIMAGE_Release(pIImage);

        jpg_decoder_done = TRUE;
    }   
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

    ICAMERA_SetFramesPerSecond(pMe->m_pCamera, dwFPS);
    ICAMERA_SetParm(pMe->m_pCamera, CAM_PARM_BANDING, pMe->m_nCameraBanding, 0);
    ICAMERA_SetZoom(pMe->m_pCamera, pMe->m_nCameraZoom); 
        
    ICAMERA_SetBrightness(pMe->m_pCamera, pMe->m_nCameraBrightness);   
}

void CameraApp_InitCameraCheck(void *po)
{
    CCameraApp *pMe = (CCameraApp *)po; 

    CameraApp_CreateDirectory(pMe);
 
    if(pMe->m_pCamera == NULL)
    {
        ISHELL_CreateInstance(pMe->m_pShell, 
                              AEECLSID_CAMERA, 
                              (void **)&pMe->m_pCamera);
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

#ifndef FEATURE_CAMERAAPP_FUN_FRAME
static void CameraApp_SetFunColorMenuItemSel(CCameraApp *pMe, uint16 wParam)
{
    switch(wParam)
    {
        case IDS_NORMAL:
            pMe->m_nCameraColor = OEMNV_CAMERA_COLOR_NORMAL;                  
            break;

        case IDS_COLOR_BANDW:
            pMe->m_nCameraColor = OEMNV_CAMERA_COLOR_BANDW;                   
            break;

        case IDS_COLOR_SEPIA:
            pMe->m_nCameraColor = OEMNV_CAMERA_COLOR_SEPIA;                   
            break;

        case IDS_COLOR_NEGATIVE:
            pMe->m_nCameraColor = OEMNV_CAMERA_COLOR_NEGATIVE;                   
            break;

        case IDS_COLOR_RED:
            pMe->m_nCameraColor = OEMNV_CAMERA_COLOR_REDONLY;                  
            break;
            
        case IDS_COLOR_GREEN:
            pMe->m_nCameraColor = OEMNV_CAMERA_COLOR_GREENONLY;
            break;

        default:
            break;
    }   
}

static void CameraApp_SetColorCFGMenuIcon(CCameraApp *pMe, IMenuCtl *popMenu)
{
    switch(pMe->m_nCameraColor)
    {
        case OEMNV_CAMERA_COLOR_NORMAL:
            CameraApp_SetCFGMenuIcon(popMenu, IDS_NORMAL, TRUE);
            break;

        case OEMNV_CAMERA_COLOR_BANDW:
            CameraApp_SetCFGMenuIcon(popMenu, IDS_COLOR_BANDW, TRUE);
            break;

        case OEMNV_CAMERA_COLOR_SEPIA:
            CameraApp_SetCFGMenuIcon(popMenu, IDS_COLOR_SEPIA, TRUE);
            break;

        case OEMNV_CAMERA_COLOR_NEGATIVE:
            CameraApp_SetCFGMenuIcon(popMenu, IDS_COLOR_NEGATIVE, TRUE);
            break;
       
        case OEMNV_CAMERA_COLOR_REDONLY:
            CameraApp_SetCFGMenuIcon(popMenu, IDS_COLOR_RED, TRUE);
            break;

        case OEMNV_CAMERA_COLOR_GREENONLY:
            CameraApp_SetCFGMenuIcon(popMenu, IDS_COLOR_GREEN, TRUE);
            
        default:
            break;
    }
}
#endif
