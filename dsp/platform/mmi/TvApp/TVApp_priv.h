#ifndef TVAPP_PRIV_H
#define TVAPP_PRIV_H

#define AEE_APPSTVAPP_RES_FILE  AEE_RES_LANGDIR TVAPP_RES_FILE

/*==============================================================================
// �ļ���TVApp_priv.h
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
#include "TVApp.bid"
#else
#include "OEMClassIds.h"      // Applet Ids
#ifndef AEECLSID_TVAPP
#error AEECLSID_TVAPP must be defined before including cameraApp.h
#endif
#endif

//#include "TVApp.h"
#include "tvapp.brh"
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
#include "TVApp.h"
//#include "AEECamera.h"
#include "AEECM.h"
//#include "ICameraExt.h"
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
#include "tvapp_image.brh"
#include "db.h"
#ifdef FEATURE_SUPPORT_VC0848
#include "Vc0848.h"
#endif
#include "Appscommon.brh"
#include "OEMSVC.h"
#include "AEEBacklight.h"
#include "AEETv.h"
#include "IMMITv.h"
//#include "TvSettingCfg.h"
#include "CustomOEMConfigItems.h"

/*==============================================================================
                                 ���Ͷ���
==============================================================================*/
#define AEE_APPSTVAPP_RES_FILE  AEE_RES_LANGDIR TVAPP_RES_FILE
#define SEARCH_ANI_FILE "fs:/image/tvapp/search.gif"
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


#define NrOfTools 9  // tool num

#define LISTIMAGE_ADD_W	16 
#define LISTIMAGE_ADD_H	16
#define LISTIMAGE_DEFAULT_W 16
#define LISTIMAGE_DEFAULT_H 16
#define LISTIMAGE_FAVICON_W 16
#define LISTIMAGE_FAVICON_H 16
#define BROWSER_BOOKMARK_BLOCK_SIZE 1024
#define BROWSER_BOOKMARK_CAPACITY 1024*1024
#define MAX_CHANNEL_NUMBER  32 //�����32������̨

/*����*/
#define HORIZONTAL_TV_WIDTH      320
#define HORIZONTAL_TV_HEIGHT     240
/*����*/ 
#define VERTICAL_TV_WIDTH        240
#define VERTICAL_TV_HEIGHT       192  

typedef struct 
{
	boolean				b_IsIconHiden;
	//TTvPropertyValue 	ResolutionID_DC;	// Resolution for snapshot
	int32 	ResolutionID_DV;	// Resolution for recording
	int32	BrightnessStep; /*��������0~15*/
    int32   ContrastStep; /*���öԱȶ�0~15*/
    int32   DefinitionStep; /*����������0~63*/
    int32   SaturationStep; /*����ɫ�ʱ��Ͷ�0~255*/
    int32   SoundStep;/*����*/
    int ChannelCountAble;//�ϴ��ѵ������źŵ�̨����
    int CurrentChannel;//��һ���˳�֮ǰ�����Ǹ�̨����    
    TvBookmark   Bookmark[32];//�����Ա���32����ǩ
    int   Bookmarktotal;       //��ǰ�洢Ƶ������
    TLG_REGION_CODE region;//��ǰ���õ�����
}MMITV_SETTINGS;

#if 0
// ������ײ���ʾ���궨��
//#define DRAW_BOTTOMBAR(x)                           \
//{                                                   \
 //   BottomBar_Param_type BarParam;                  \
//MEMSET(&BarParam, 0, sizeof(BarParam));         \
//
//    BarParam.eBBarType = x;                         \
//    DrawBottomBar(pMe->m_pDisplay, &BarParam);      \
//}
#endif

typedef enum
{
    OPTION_BACK,
    SELECT_BACK,
    SAVE_DEL       
}BOTTOMBARTEX;

// ��������ò����б�
typedef enum 
{
    TVCFGENVIRMENT,
    TVCFGFIRST = TVCFGENVIRMENT,
    TVCFGQUALITY,
    TVCFGSIZE,
    TVCFGTONE,
    TVCFGSELFTIME,
    TVCFGBANDING,
    TVCFGRESET,
    TVCFGLAST = TVCFGRESET,
    TVCFGMAX
} TVCFG;

typedef enum 
{
	FILE_TYPE_JPG,
	FILE_TYPE_AMR,
	FILE_TYPE_3GP,
    FILE_TYPE_ERROR = 0XFF
} FILE_TYPE;

// TV״̬
typedef enum
{
    TV_START,
    TV_PREVIEW,
    TV_CAPTURE,
    TV_SAVE,
    TV_RECORD,
    TV_STOP
} TVSTATE; 

/*----------------------�Ի������������������---------------------*/
// �Ի���ر�ʱ����ֵ�б�
typedef enum DLGRetValue
{
    DLGRET_CREATE,
    DLGRET_MAINMENU,
    DLGRET_PREVIEW,
    DLGRET_PICMENU,
    DLGRET_TVCFG,
    DLGRET_POPMSG,
    DLGRET_CANCELED,
    DLGRET_REGION,
    DLGRET_ASIA,
    DLGRET_LATIN,
    DLGRET_EUROPE,
    DLGRET_AFRICA,
    DLGRET_OCEANIA,
    DLGRET_DRAWTOPBAR,
    DLGRET_BAR1,
    DLGRET_BAR2,
    DLGRET_BAR3,
    DLGRET_BAR4,
    DLGRET_BAR5,
    DLGRET_BAR6,
    DLGRET_BAR7,
    DLGRET_AUTOSCAN,
    DLGRET_BOOKMARK,
    DLGRET_EXIT 
} DLGRetValue;

/*----------------------״̬�����������������---------------------*/
// TVApp Applet ״̬��״̬��
typedef enum
{
    STATE_NULL,
    STATE_CMAINMENU,
    STATE_CPREVIEW,
    STATE_CCAMERACFG,
    STATE_CPIC,
    STATE_CPOPMSG,
    STATE_REGION,
    STATE_ASIA,
    STATE_LATIN,
    STATE_EUROPE,
    STATE_AFRICA,
    STATE_OCEANIA,
    STATE_DRAWTOPBAR,
    STATE_BAR1,
    STATE_BAR2,
    STATE_BAR3,
    STATE_BAR4,
    STATE_BAR5,
    STATE_BAR6,
    STATE_BAR7,
    STATE_AUTOSCAN,
    STATE_BOOKMARK,
    STATE_EXIT
} FSMState;

// ״̬���������ظ�״̬������������ֵ����
typedef enum NextFSMAction
{
    NFSMACTION_WAIT,
    NFSMACTION_CONTINUE
} NextFSMAction;

// ����ģ����Ϣ��ģ�����ü����Ľṹ
typedef struct _TVAppMod
{
    DECLARE_VTBL(IModule)
    uint32   m_nRefs;
} CTVAppMod;

// TVApp Applet����ṹ�壺
typedef struct _CTVApp
{
    DECLARE_VTBL(ITVApp)
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

	ITlgAtv				*pITv;
	IMMITv				*pIMMITv;
	ICBMMITv			myICBMMITv;
    TVCFG                m_nTVCFG;           // camera����
    TVSTATE              m_nTVState;         // TV������״̬
    //CAMERASTATE          m_nCameraState;         // camera������״̬
    
    OEMCAMERAQUALITY     m_nTVQuality;       // camera��������
    OEMCAMERATONE        m_nTVTone;          // camera��������
    OEMCAMERAENVIR       m_nTVEnviroment;    // camera���ջ���  fps
    OEMCAMERASIZE        m_nTVSize;          // camera���ճߴ�
    OEMCAMERABANDING     m_nTVBanding;       // camera bandig����
    OEMCAMERASTORAGE     m_nTVStorage;       // camera���մ洢λ��

	IImage               *m_pImage;
	ICamera            *m_pCamera;
    ICamera            *m_pTV;
    IFileMgr             *m_pFM; 
    IConfig              *m_pConfig;              // IConfig interface
    IMedia               *m_pMedia;
    IFileMgr             *m_pFileMgr;
    IAnnunciator         *m_pIAnn;
    boolean              m_isFormQuicktest;
    IBacklight           *m_pBacklight;
    byte                 m_nBacklightVal;
    int32                 m_sensor_model; //�ж�����ͷΪ�������ص� add by xuhui

	AEEApplet        	a;
    IShell 				*pIShell;

	CFG_TvSetting	*pTvSetting;
	
	uint16   currentlyChannel; //��ǰ���ڲ��ŵ�Ƶ��
    uint16   currentlyChannelIndex;//��ǰ���ڲ��ŵ�Ƶ������ǩ�е��߼�����
} CTVApp;

/*==============================================================================                         
                                 �궨��ͳ���                        
==============================================================================*/
// Ϊ��ֹ�û����ٰ���������¼�EVT_APPISREADY���¼��ɶ�ʱ��
// TVMenu_APPIsReadyTimer����
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
       TVApp_ShowDialog
˵��:
       ������״̬����������������Ӧ�Ի�����ԴID�ĶԻ���

����:
       pMe [In]: ָ��TVApp Applet����ṹ��ָ��,�ýṹ����С������ض���Ϣ.
       dlgResId [in]���Ի�����ԴID���������ݸ�ID�����Ի���

����ֵ:
       ��

��ע:
       

==============================================================================*/
void TVApp_ShowDialog(CTVApp *pMe, uint16 dlgResId);

/*==============================================================================
����:
       TVApp_RouteDialogEvent
˵��:
       ������BREW�¼�·�ɸ���ǰ��Ի�����¼���������

����:
       pMe [In]: ָ��TVApp Applet����ṹ��ָ��,�ýṹ����С������ض���Ϣ.
       eCode [in]���¼����롣
       wParam [in]�����¼�eCode���������ݡ�
       dwParam [in]�����¼�eCode���������ݡ�

����ֵ:
       TRUE�������¼�������
       FALSE�������¼�û������

��ע:

==============================================================================*/
boolean TVApp_RouteDialogEvent(CTVApp *pMe,
                                   AEEEvent      eCode,
                                   uint16        wParam,
                                   uint32        dwParam);

/*----------------------״̬����غ�������---------------------*/
/*==============================================================================
����:
       TVApp_ProcessState
˵��:
       TVApp Applet״̬��������������������pMe->m_currState������·����
       ��Ӧ�Ĵ�������

����:
       pMe [In]: ָ��TVApp Applet����ṹ��ָ��,�ýṹ����С������ض���Ϣ.

����ֵ:
       NFSMACTION_CONTINUE��ָʾ������״̬��״̬������ֹͣ��
       NFSMACTION_WAIT��ָʾ��Ҫ��ʾ�Ի��������û���Ӧ����״̬����

��ע:

==============================================================================*/
NextFSMAction TVApp_ProcessState(CTVApp *pMe);

/*============================================================================
   ���ٰ���������
============================================================================*/
void CTVApp_APPIsReadyTimer(void *pme);
void TVApp_AppEventNotify(CTVApp *pMe, int16 nCmd, int16 nStatus);
void TVApp_InitTVCheck(void *po);


#endif // end cameraAPP_PRIV_H
