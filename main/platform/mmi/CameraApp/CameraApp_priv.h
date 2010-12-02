#ifndef CAMERAAPP_PRIV_H
#define CAMERAAPP_PRIV_H
/*==============================================================================
// �ļ���CameraApp_priv.h
//        ��Ȩ����(c) 2009 Anylook Tech. CO.,LTD.
//        
// �ļ�˵����
//        
==============================================================================*/

/*==============================================================================
�޸���ʷ��¼��
ʱ       ��      �� �� ��     �޸����ݡ�λ�ü�ԭ��
-----------      ----------   -----------------------------------------------
==============================================================================*/

/*==============================================================================                      
                             ���ļ��������ⲿ�ļ�                         
==============================================================================*/
#ifndef AEE_SIMULATOR
#include "customer.h"         // Handset Feature Definitions
#else
#include "BREWSimFeatures.h"  // Simulator Feature Definitions
#endif

#if !defined(BREW_STATIC_APP)
//Use the .bid file if compiling as dynamic applet
#include "CameraApp.bid"
#else
#include "OEMClassIds.h"      // Applet Ids
#ifndef AEECLSID_APP_CAMERA
#error AEECLSID_APP_CAMERA must be defined before including cameraApp.h
#endif
#endif

#include "cameraApp.h"
#include "cameraapp.brh"
#include "AEEShell.h"
#include "AEEModTable.h"
#include "AEEStdLib.h"         // AEE StdLib Services
#include "AEEModGen.h"         // AEEMod Declaration
#include "AEEAppGen.h"         // AEEApplet Declaration
#include "OEMClassIDs.h"

#include "AEEMenu.h"           // AEE Menu Services
#include "AEEFile.h"           // AEE File Services
#include "AEEImage.h"
#include "AEEBitmap.h"
#include "AEEImageCtl.h"
#include "AEEText.h"           // AEE Text Services
#include "AEEMimeTypes.h"  
#ifndef AEE_SIMULATOR  
#include "AEEAlert.h"          // IAlert Services
#include "AEEGraphics.h"
#include "OEMCFGI.h"
#endif

//callback structure definitions.
#include "AEEVector.h"
#include "AEEAnnunciator.h"
#include "AEEBacklight.h"
#include "AEEConfig.h"
#include "CameraApp.h"
#include "AEECamera.h"
#include "AEECM.h"
#include "ICameraExt.h"
#include "OEMCamera.h"
#include "Appscommon.h"
#include "BREWVersion.h"
#include "Appscommon.h"
#include "AppComFunc.h"
#include "err.h"
#include "AEEAnnunciator.h"
#include "AEEMediaUtil.h"
#include "AEEMedia.h"
#include "AEECamera.h"
#include "cameraapp_image.brh"
#include "db.h"
#ifdef FEATURE_SUPPORT_VC0848
#include "Vc0848.h"
#endif
#include "Appscommon.brh"
#include "OEMSVC.h"
#include "AEEBacklight.h"

/*==============================================================================
                                 ���Ͷ���
==============================================================================*/
#define AEE_APPSCAMERAAPP_RES_FILE  AEE_RES_LANGDIR CAMERAAPP_RES_FILE
#if defined(FEATURE_DISP_128X128)
// camera quality
#define CAMERA_APP_QUALITY_HIGH  33
#define CAMERA_APP_QUALITY_MED   67
#define CAMERA_APP_QUALITY_LOW   100

// camera zoom
#define CAMERA_APP_ZOOM_UP       1
#define CAMERA_APP_ZOOM_DOWN     2

// bar and menu position
#define TOPBAR_ICON_Y         2
#define TOPBAR_ICON_WIDTH     17
#define TOPBAR_TITLE_TEXT_Y   6
#define CFGBAR_Y              160
#define CFGBAR_EFFECT_TEXT_Y  180
#define SCROLLBAR_X           60
#define SCROLLBAR_Y           160
#define POPMENU_BOTTOM_X      60
#define POPMENU_BOTTOM_Y      60
#define POPMENU_WIDTH         100 //117

//Add By zzg 2010_07_24
#define CFGBAR_TEXT_X		   2
#define CFGBAR_TEXT_Y		   18
#define CFGBAR_TEXT_WIDTH	   80
#define CFGBAR_TEXT_HEIGHT	   16		//NormalFont Height

#define CFGBAR_ARROW_WIDTH		8
#define CFGBAR_ARROW_HEIGHT		14
//Add End

// pop timeout
#define TIMEOUT_MS_MSGBOX     3000
#define TIMEOUT_MS_MSGDONE    1000
#define TIMEOUT_MS_QUICK      300

// file name lenth
#define MIN_FILE_NAME_LEN      64
#define MIN_PICS_NAME_LEN      128
#define MIN_PIC_CHAR_NAME_LEN  13
#define MIN_PIC_WCHAR_NAME_LEN 26
#define MIN_PIC_NUMOFSUM_LEN   4  
#define MIN_TEXT_HOTKEY_LEN    50  
#define MIN_TEXT_RES_LEN       20

#define MIN_FREE_MEMORY_CARD_SPACE   32
#define MIN_FREE_PHONE_SPACE         1

#elif defined(FEATURE_DISP_160X128)
// camera quality
#define CAMERA_APP_QUALITY_HIGH  33
#define CAMERA_APP_QUALITY_MED   67
#define CAMERA_APP_QUALITY_LOW   100

// camera zoom
#define CAMERA_APP_ZOOM_UP       1
#define CAMERA_APP_ZOOM_DOWN     2

// bar and menu position
#define TOPBAR_ICON_Y         2
#define TOPBAR_ICON_WIDTH     17
#define TOPBAR_TITLE_TEXT_Y   6
#define CFGBAR_Y              160
#define CFGBAR_EFFECT_TEXT_Y  180
#define SCROLLBAR_X           60
#define SCROLLBAR_Y           160
#define POPMENU_BOTTOM_X      60
#define POPMENU_BOTTOM_Y      60
#define POPMENU_WIDTH         100 //117

//Add By zzg 2010_07_24
#define CFGBAR_TEXT_X		   2
#define CFGBAR_TEXT_Y		   18
#define CFGBAR_TEXT_WIDTH	   80
#define CFGBAR_TEXT_HEIGHT	   16		//NormalFont Height

#define CFGBAR_ARROW_WIDTH		8
#define CFGBAR_ARROW_HEIGHT		14
//Add End

// pop timeout
#define TIMEOUT_MS_MSGBOX     3000
#define TIMEOUT_MS_MSGDONE    1000
#define TIMEOUT_MS_QUICK      300

// file name lenth
#define MIN_FILE_NAME_LEN      64
#define MIN_PICS_NAME_LEN      128
#define MIN_PIC_CHAR_NAME_LEN  13
#define MIN_PIC_WCHAR_NAME_LEN 26
#define MIN_PIC_NUMOFSUM_LEN   4  
#define MIN_TEXT_HOTKEY_LEN    50  
#define MIN_TEXT_RES_LEN       20

#define MIN_FREE_MEMORY_CARD_SPACE   32
#define MIN_FREE_PHONE_SPACE         1

#elif defined(FEATURE_DISP_220X176)
// camera quality
#define CAMERA_APP_QUALITY_HIGH  33
#define CAMERA_APP_QUALITY_MED   67
#define CAMERA_APP_QUALITY_LOW   100

// camera zoom
#define CAMERA_APP_ZOOM_UP       1
#define CAMERA_APP_ZOOM_DOWN     2

// bar and menu position
#define TOPBAR_ICON_Y         2
#define TOPBAR_ICON_WIDTH     21
#define TOPBAR_TITLE_TEXT_Y   6
#define CFGBAR_Y              160
#define CFGBAR_EFFECT_TEXT_Y  180
#define SCROLLBAR_X           60
#define SCROLLBAR_Y           160
#define POPMENU_BOTTOM_X      60
#define POPMENU_BOTTOM_Y      60
#define POPMENU_WIDTH         100 //117

//Add By zzg 2010_07_24
#define CFGBAR_TEXT_X		   2
#define CFGBAR_TEXT_Y		   21
#define CFGBAR_TEXT_WIDTH	   80
#define CFGBAR_TEXT_HEIGHT	   20		//NormalFont Height

#define CFGBAR_ARROW_WIDTH		8
#define CFGBAR_ARROW_HEIGHT		14
//Add End

// pop timeout
#define TIMEOUT_MS_MSGBOX     3000
#define TIMEOUT_MS_MSGDONE    1000
#define TIMEOUT_MS_QUICK      300

// file name lenth
#define MIN_FILE_NAME_LEN      64
#define MIN_PICS_NAME_LEN      128
#define MIN_PIC_CHAR_NAME_LEN  13
#define MIN_PIC_WCHAR_NAME_LEN 26
#define MIN_PIC_NUMOFSUM_LEN   4  
#define MIN_TEXT_HOTKEY_LEN    50  
#define MIN_TEXT_RES_LEN       20

#define MIN_FREE_MEMORY_CARD_SPACE   32
#define MIN_FREE_PHONE_SPACE         1

#elif defined(FEATURE_DISP_320X240)
// camera quality
#define CAMERA_APP_QUALITY_HIGH  33
#define CAMERA_APP_QUALITY_MED   67
#define CAMERA_APP_QUALITY_LOW   100

// camera zoom
#define CAMERA_APP_ZOOM_UP       1
#define CAMERA_APP_ZOOM_DOWN     2

// bar and menu position
#define TOPBAR_ICON_Y         2
#define TOPBAR_ICON_WIDTH     17
#define TOPBAR_TITLE_TEXT_Y   6
#define CFGBAR_Y              160
#define CFGBAR_EFFECT_TEXT_Y  180
#define SCROLLBAR_X           60
#define SCROLLBAR_Y           160
#define POPMENU_BOTTOM_X      60
#define POPMENU_BOTTOM_Y      60
#define POPMENU_WIDTH         100 //117

//Add By zzg 2010_07_24
#define CFGBAR_TEXT_X		   2
#define CFGBAR_TEXT_Y		   18
#define CFGBAR_TEXT_WIDTH	   80
#define CFGBAR_TEXT_HEIGHT	   16		//NormalFont Height

#define CFGBAR_ARROW_WIDTH		8
#define CFGBAR_ARROW_HEIGHT		14
//Add End

// pop timeout
#define TIMEOUT_MS_MSGBOX     3000
#define TIMEOUT_MS_MSGDONE    1000
#define TIMEOUT_MS_QUICK      300

// file name lenth
#define MIN_FILE_NAME_LEN      64
#define MIN_PICS_NAME_LEN      128
#define MIN_PIC_CHAR_NAME_LEN  13
#define MIN_PIC_WCHAR_NAME_LEN 26
#define MIN_PIC_NUMOFSUM_LEN   4  
#define MIN_TEXT_HOTKEY_LEN    50  
#define MIN_TEXT_RES_LEN       20

#define MIN_FREE_MEMORY_CARD_SPACE   32
#define MIN_FREE_PHONE_SPACE         1

#endif



// file path
#define FS_EFS_DIRECTORY_STR       "fs:/hsmm/"
#define FS_CARD_PICTURES_FOLDER    MG_PICTURES_FOLDER DIRECTORY_STR
#define FS_CARD_MUSIC_FOLDER       MG_MUSIC_FOLDER DIRECTORY_STR
#define FS_CARD_VIDEOS_FOLDER      MG_VIDEOS_FOLDER DIRECTORY_STR
#define FS_EFS_PICTURES_FOLDER     FS_EFS_DIRECTORY_STR FS_CARD_PICTURES_FOLDER

// video recorder timer
#define RUNNING_UPDATE_MSECS   1000
#define MSECS_PER_DAY         (3600*24*1000)

#define BYTE_SIZE  1024

#define WHITE_TEXT            MAKE_RGB(251, 251, 251)

// ������ײ���ʾ���궨��
#define DRAW_BOTTOMBAR(x)                           \
{                                                   \
    BottomBar_Param_type BarParam;                  \
    MEMSET(&BarParam, 0, sizeof(BarParam));         \
    BarParam.eBBarType = x;                         \
    DrawBottomBar(pMe->m_pDisplay, &BarParam);      \
}

typedef enum
{
    OPTION_BACK,
    SELECT_BACK,
    SAVE_DEL       
}BOTTOMBARTEX;

// ��������ò����б�
typedef enum 
{
    CAMERACFGENVIRMENT,
    CAMERACFGFIRST = CAMERACFGENVIRMENT,
    CAMERACFGQUALITY,
    CAMERACFGSIZE,
    CAMERACFGTONE,
    CAMERACFGSELFTIME,
    CAMERACFGBANDING,
    CAMERACFGRESET,
    CAMERACFGLAST = CAMERACFGRESET,
    CAMERACFGMAX
} CAMERACFG;

typedef enum 
{
	FILE_TYPE_JPG,
	FILE_TYPE_AMR,
	FILE_TYPE_3GP,
    FILE_TYPE_ERROR = 0XFF
} FILE_TYPE;

// Camera״̬
typedef enum
{
    CAM_START,
    CAM_PREVIEW,
    CAM_CAPTURE,
    CAM_SAVE,
    CAM_RECORD,
    CAM_STOP
} CAMERASTATE; 

/*----------------------�Ի������������������---------------------*/
// �Ի���ر�ʱ����ֵ�б�
typedef enum DLGRetValue
{
    DLGRET_CREATE,
    DLGRET_MAINMENU,
    DLGRET_PREVIEW,
    DLGRET_PICMENU,
    DLGRET_CAMERACFG,
    DLGRET_POPMSG,
    DLGRET_CANCELED,
    DLGRET_EXIT 
} DLGRetValue;

/*----------------------״̬�����������������---------------------*/
// CameraApp Applet ״̬��״̬��
typedef enum
{
    STATE_NULL,
    STATE_CMAINMENU,
    STATE_CPREVIEW,
    STATE_CCAMERACFG,
    STATE_CPIC,
    STATE_CPOPMSG,
    STATE_EXIT
} FSMState;

// ״̬���������ظ�״̬������������ֵ����
typedef enum NextFSMAction
{
    NFSMACTION_WAIT,
    NFSMACTION_CONTINUE
} NextFSMAction;

// ����ģ����Ϣ��ģ�����ü����Ľṹ
typedef struct _CameraAppMod
{
    DECLARE_VTBL(IModule)
    uint32   m_nRefs;
} CCameraAppMod;

// CameraApp Applet����ṹ�壺
typedef struct _CCameraApp
{
    DECLARE_VTBL(ICameraApp)
    uint32               m_nRefs;          // �������ü�����
    IModule              *m_pModule;
    IDisplay             *m_pDisplay;
    IShell               *m_pShell;    
    AEERect               m_rc;
    
    // ��ǰ��Ի���IDialog�ӿ�ָ��
    IDialog              *m_pActiveDlg;
    
    // ��ǰ��Ի���ID
    uint16               m_pActiveDlgID;
    
    // ��Ի���ر�ʱ�ķ��ؽ��
    DLGRetValue          m_eDlgRet;
     
    // �Ƿ񲻸�д�Ի��򷵻ؽ����һ�������Ҫ��д(FALSE)��
    boolean              m_bNotOverwriteDlgRet;
    
    // Applet ǰһ״̬
    FSMState             m_ePreState;
    
    // Applet ��ǰ״̬
    FSMState             m_eCurState;
    
    // Applet�Ƿ��ڹ���״̬
    boolean              m_bSuspending;
    
   // Applet�Ƿ�׼���ÿ��Դ������������¼����ı�����Ҫ���ڿ��ٰ������������
    boolean              m_bAppIsReady;
    
    boolean              m_bIsPreview;           // �ж�camera�Ƿ���preview״̬
    boolean              m_bRePreview;           // �ж��Ƿ���Ҫcamera����preview
    boolean              m_bCapturePic;          // �ж�camera�Ƿ�������״̬�����ڽ�ֹ�����¼�
    boolean              m_bCanCapture;          // ���洢�������ж��Ƿ��������
    boolean              m_bMemoryCardExist;     // ����Ƿ���ڴ洢��
    
    uint16               m_wMsgID;               // pop�Ի�����ʾ�ı�����ԴID��
    uint32               m_nMsgTimeout;           // pop�Ի����Զ��ر�����Ҫ��ʱ�� 

    uint32               m_nMainMenuItemSel;     // ���˵���ѡ��
    uint32               m_nSelfTimeItemSel;     // ����ʱ��˵���ѡ��
    
    uint32               m_nTicks;               // tickʱ��
    uint32               m_nNumDays;             // �ܱ��ܵ�����
    uint32               m_dwDispTime;           // ��ʾʱ��
    uint32               m_dwMemTotal;           // �洢�ռ�������
    uint32               m_dwMemFree;            // �洢�ռ�ʣ������

    int                  m_nLeftTime;
    int                  m_cxWidth; 
    int                  m_cyHeight;  
    int                  m_nItemH;               // �ı�Ĭ������߶�
        
    char                 m_sCurrentFileName[MIN_FILE_NAME_LEN];
    char                 m_sCaptureFileName[MIN_PICS_NAME_LEN];
    AEECallback          m_CallBack;
    
    CAMERACFG            m_nCameraCFG;           // camera����
    CAMERASTATE          m_nCameraState;         // camera������״̬
    
    OEMCAMERAQUALITY     m_nCameraQuality;       // camera��������
    OEMCAMERATONE        m_nCameraTone;          // camera��������
    OEMCAMERAENVIR       m_nCameraEnviroment;    // camera���ջ���  fps
    OEMCAMERASIZE        m_nCameraSize;          // camera���ճߴ�
    OEMCAMERABANDING     m_nCameraBanding;       // camera bandig����
    OEMCAMERASTORAGE     m_nCameraStorage;       // camera���մ洢λ��
    
    ICamera              *m_pCamera;
    IFileMgr             *m_pFM; 
    IConfig              *m_pConfig;              // IConfig interface
    IMedia               *m_pMedia;
    IFileMgr             *m_pFileMgr;
    IAnnunciator         *m_pIAnn;
    boolean              m_isFormQuicktest;
    IBacklight           *m_pBacklight;
    byte                 m_nBacklightVal;
    int32                 m_sensor_model; //�ж�����ͷΪ�������ص� add by xuhui
} CCameraApp;

/*==============================================================================                         
                                 �궨��ͳ���                        
==============================================================================*/
// Ϊ��ֹ�û����ٰ���������¼�EVT_APPISREADY���¼��ɶ�ʱ��
// CameraMenu_APPIsReadyTimer����
#define APPISREADY_TIMER    100                 
#define EVT_APPISREADY      (EVT_USER+1)

// ״̬�ƶ��꣺�Ƚ�ǰһ״̬�õ�ǰ״̬���£��ٽ���ǰ״̬��ΪnextState
#define MOVE_TO_STATE(nextState) {               \
            FSMState  tpState;                   \
            tpState = nextState;                 \
            pMe->m_ePreState = pMe->m_eCurState; \
            pMe->m_eCurState = tpState;          \
         }

// �رնԻ����
#define CLOSE_DIALOG(DlgRet) {                                           \
                                 pMe->m_eDlgRet = DlgRet;                \
                                 (void) ISHELL_EndDialog(pMe->m_pShell); \
                             }

/*==============================================================================
                                 ȫ������
==============================================================================*/

/*==============================================================================
                                 ���أ���̬������
==============================================================================*/

/*==============================================================================
                                 ��������
==============================================================================*/
/*----------------------�Ի�����غ�������---------------------*/
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
void CameraApp_ShowDialog(CCameraApp *pMe, uint16 dlgResId);

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
boolean CameraApp_RouteDialogEvent(CCameraApp *pMe,
                                   AEEEvent      eCode,
                                   uint16        wParam,
                                   uint32        dwParam);

/*----------------------״̬����غ�������---------------------*/
/*==============================================================================
����:
       CameraApp_ProcessState
˵��:
       CameraApp Applet״̬��������������������pMe->m_currState������·����
       ��Ӧ�Ĵ�������

����:
       pMe [In]: ָ��CameraApp Applet����ṹ��ָ��,�ýṹ����С������ض���Ϣ.

����ֵ:
       NFSMACTION_CONTINUE��ָʾ������״̬��״̬������ֹͣ��
       NFSMACTION_WAIT��ָʾ��Ҫ��ʾ�Ի��������û���Ӧ����״̬����

��ע:

==============================================================================*/
NextFSMAction CameraApp_ProcessState(CCameraApp *pMe);

/*============================================================================
   ���ٰ���������
============================================================================*/
void CCameraApp_APPIsReadyTimer(void *pme);
void CameraApp_AppEventNotify(CCameraApp *pMe, int16 nCmd, int16 nStatus);
void CameraApp_InitCameraCheck(void *po);
#endif // end cameraAPP_PRIV_H
