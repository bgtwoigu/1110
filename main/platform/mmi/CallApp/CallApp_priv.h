#ifndef CALLAPP_PRIV_H
#define CALLAPP_PRIV_H

/*==============================================================================
// �ļ���
//        CallApp_priv.h
//        2007-11-01 Gemsea����汾(Draft Version)
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
-----------      ----------  --------  -----------------------------------------------
2007-11-01                  ��ʼ�汾
==============================================================================*/


/*==============================================================================
                                 ���ļ��������ⲿ�ļ�
==============================================================================*/
#ifndef AEE_SIMULATOR
#include "customer.h"   //Handset Feature Definitions
#else
#include "BREWSimFeatures.h"   //Simulator Feature Definitions
#endif

#if defined( FEATURE_CALL_RECORDER)
#include "recorder.h"
#include "recorder.brh"
#include "recorder_images.brh"
#if defined(FEATURE_DISP_160X128)
#include "Appscommon_160x128.brh"
#elif defined(FEATURE_DISP_128X128)
#include "Appscommon_color.brh"
#else
#include "Appscommon_color.brh"
#endif

#include "appscommon.h"
#define  APP_RECORDER_RES_FILE ("fs:/mod/recorder/" AEE_RES_LANGDIR RECORDER_RES_FILE)
#define  APP_RECORDER_IMAGES_RES_FILE ("fs:/mod/recorder/" RECORDER_IMAGES_RES_FILE)
#endif


#include "AEECM.h"
#include "CallApp.brh"
#include "AEEShell.h"
#include "AEEModTable.h"
#include "AEEStdLib.h"
#include "AEEModGen.h"
#include "AEEAppGen.h"
#ifdef CUST_EDITION
#include "OEMClassIDs.h"
#include "OEMFeatures.h"
#endif
#include "CallApp.h"
#include "AEE.h"
#include "AEEHeap.h"
#include "AEEText.h"
#include "AEETime.h"
#include "AEEDate.h"
#include "AEEMimeTypes.h"
//#ifdef FEATURE_LANG_CHINESE
//#include "aeeext_res.h"
//#endif /* FEATURE_LANG_CHINESE */
#ifdef FEATURE_REFUIALARM
#include "AEEAlarm.h"
#endif
#include "AEEAlert.h"
#include "OEMText.h"  // for TP_LARGE_FONT
//#include "OEMRUIM.h"
#include "OEMDisp.h"       // TBD - hack until available somewhere else
#include "OEMSound.h"      // TBD - hack until available somewhere else
#include "AEEConfig.h"
#include "AEE_OEM.h"
#include "ContApp.h"
#include "AEESoundPlayer.h"
#include "AEEMedia.h"  // for voice recording
#include "AEEBacklight.h"
#include "OEMFS.h"

#ifdef FEATURE_LED
#include "led.h"
#endif//FEATURE_LED
//#include "OEMFS.h"
#ifndef WIN32
#include "db.h"
#endif
#ifdef FEATURE_UTK2
#include "UTKUtils.h"
#endif
#include "Appscommon.h"
#include "AppComFunc.h"
#include "AEECallHistory.h"
#include "AEECallHistoryExt.h"
#include "AEEAnnunciator.h"
#include "OEMCFGI.h"
// ͼƬ��Դ�ļ�
#if defined(FEATURE_DISP_160X128)
#include "Appscommon_160x128.brh"
#elif defined(FEATURE_DISP_128X128)
#include "Appscommon_color.brh"
#else
#include "Appscommon_color.brh"
#endif


#ifdef FEATURE_LCD_TOUCH_ENABLE//wlh add for LCD touch
//#include "AEEVirtualkey.h"//wlh for vkey number
#define CALLAPP_PT_IN_RECT(a,b,rct)      (boolean)( ((a) >= (rct).x && (a) <= ((rct).x + (rct).dx)) && ((b) >= (rct).y && (b) <= ((rct).y + (rct).dy)) )
#endif
#ifdef WIN32

#include "oemhelperfunctype.h"
#define KEYSND_ZY
/* Type to specify which language to use for the user interface */
typedef  word  nv_language_enum_type;
  /* Use English language */
  #define  NV_LANGUAGE_ENGLISH                           ((nv_language_enum_type)0)
  /* Use Spanish language */
  #define  NV_LANGUAGE_SPANISH                           ((nv_language_enum_type)1)
  /* Use French language */
  #define  NV_LANGUAGE_FRENCH                            ((nv_language_enum_type)2)
  /* Use Portuguese language */
  #define  NV_LANGUAGE_PORTUGUESE                        ((nv_language_enum_type)3)
   /* Use TChinese language    */
  #define NV_LANGUAGE_TCHINESE                           ((nv_language_enum_type)4)  
   /* Use Chinese language    */
  #define NV_LANGUAGE_CHINESE                            ((nv_language_enum_type)5)
   /* Use Japanese  language    */
  #define NV_LANGUAGE_JAPANESE                           ((nv_language_enum_type)6)
   /* Use Korean language    */
  #define NV_LANGUAGE_KOREAN                             ((nv_language_enum_type)7)  
   /* Use Hebrew language    */
  #define NV_LANGUAGE_HEBREW                             ((nv_language_enum_type)8)
   /* Use Thai language    */
  #define NV_LANGUAGE_THAI                               ((nv_language_enum_type)9)
   /* Use Italian language    */
  #define NV_LANGUAGE_ITALIAN                            ((nv_language_enum_type)10)  
   /* Use Indonesian language    */
  #define NV_LANGUAGE_INDONESIAN                         ((nv_language_enum_type)11)
   /* Use Arabic language    */
  #define NV_LANGUAGE_ARABIC                             ((nv_language_enum_type)12)
   /* Use Hindi language    */
  #define NV_LANGUAGE_HINDI                              ((nv_language_enum_type)13)      
   /* Use Vietnamese language    */
  #define NV_LANGUAGE_VIETNAMESE                         ((nv_language_enum_type)14)
/* Type to specify auto redial rings and enable/disable */
typedef PACKED struct { 
  /* TRUE if auto redial enabled */
  boolean                                          enable;
  /* Number of 10 seconds system busy before redial */
  byte                                             rings;
} nv_auto_redial_type;


#define CAI_DTMF_ON_95  0x00
  /* 95 ms recommended pulse width */
#define CAI_DTMF_ON_150 0x01
  /* 150 ms recommended pulse width */
#define CAI_DTMF_ON_200 0x02
  /* 200 ms recommended pulse width */
#define CAI_DTMF_ON_250 0x03
  /* 250 ms recommended pulse width */
#define CAI_DTMF_ON_300 0x04
  /* 300 ms recommended pulse width */
#define CAI_DTMF_ON_350 0x05
  /* 350 ms recommended pulse width */
#ifdef FEATURE_JCDMA
#define CAI_DTMF_ON_SMS 0x07
  /* SMS Tx special pulse width */
#endif /* FEATURE_JCDMA */

/*-   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -*/
/*                    DTMF_OFF_LENGTHS                                      */
/*-   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -*/
/* See IS-95A Table 6.7.2.3.2.7-2 Recommended Minimum Interdigit Interval  */

#define CAI_DTMF_OFF_60  0x00
  /* 60 ms recommended minimum interdigit interval */
#define CAI_DTMF_OFF_100 0x01
  /* 100 ms recommended minimum interdigit interval */
#define CAI_DTMF_OFF_150 0x02
  /* 150 ms recommended minimum interdigit interval */
#define CAI_DTMF_OFF_200 0x03
  /* 200 ms recommended minimum interdigit interval */

#endif


#define  AEE_APPSCALLAPP_RES_FILE AEE_RES_LANGDIR CALLAPP_RES_FILE
#define MAX_SIZE_GETIMSI                    20
#define MAX_SIZE_BANNER_TEXT           20
#define MAX_SIZE_DIALER_TEXT            128
#define MAX_SIZE_NAME_TEXT              20
#define MAX_SIZE_DIAL_STR                  AEECM_MAX_DIAL_STRING +1//33
#define TIMEOUT_MS_ONE_SECOND        1000
//#define DIALER_TEXT_LEN_MAX      32
#define MAX_COUNT_TO_CHANGE           5
#define DIALERAPP_MAX_NUM_CALLS     5
#define CALL_PICTURE_NAME_LENTH      64
//#define DIALOG_SOFTKEY_HIGH        (pMe->m_LineHeight + 2)
#define DIALERAPP_NUM_CALLHISTORY_FIELDS 5

#define CALL_ANIMATION_WIDTH           36
#define CALL_ANNU_HEIGHT                   5//16
#define CALL_SOFT_KEY_HIGHT              16//15//  (pMe->m_LargeLineHeight)
//#define CALL_TITLE_BAR_HEIGHT   14 

#define SOFT_KEY_MENU_X                    0
#define SOFT_KEY_MENU_Y                    CALL_ANNU_HEIGHT
#define SOFT_KEY_MENU_DX                  (pMe->m_rc.dx)
#define SOFT_KEY_MENU_DY                  (pMe->m_rc.dy - CALL_ANNU_HEIGHT - CALL_SOFT_KEY_HIGHT)

#define CALL_SCREEN_X                         0
#define CALL_SCREEN_Y                         0
#define CALL_SCREEN_DX                       (pMe->m_rc.dx)
#define CALL_SCREEN_DY                       (pMe->m_rc.dy - CALL_SOFT_KEY_HIGHT)
/*���߸���2������*/
#define CALL_TEXT_DX                           (pMe->m_rc.dx- 2*CALL_TEXT_X)
#define CALL_NAME_DX                          (pMe->m_rc.dx- 2*CALL_TEXT_X -CALL_ANIMATION_WIDTH)
#define CALL_NUM_DX                            (pMe->m_rc.dx- 2*CALL_TEXT_X -CALL_ANIMATION_WIDTH)
/*���߸���2������*/

#define CALL_TEXT_X                             (3)
#define CALL_NAME_X                            (2*CALL_TEXT_X + CALL_ANIMATION_WIDTH)
#define CALL_NUM_X                              (2*CALL_TEXT_X + CALL_ANIMATION_WIDTH)

//#define CALL_LINE_HIGHT           (pMe->m_LineHeight + 3)

/*����ȥ�綯��ͼƬ�߶�*/
#define CALL_ANIM_IMG_HEIGHT              (60)

/*���¸���3������*/
//#define CALL_LINE_HIGHT             ((int)((pMe->m_rc.dy - CALL_ANNU_HEIGHT - CALL_SOFT_KEY_HIGHT)/4))
/*���¸���3������*/
#define CALL_LINE_HIGHT                      (20) //18ΪNORMAL����߶ȣ�Ŀǰֻ��Ҫ����߶Ⱦ͹���

#define CALL_FIRST_LINE_Y                   (CALL_ANNU_HEIGHT)
#define CALL_SECOND_LINE_Y               (CALL_FIRST_LINE_Y +    CALL_LINE_HIGHT)
#define CALL_THIRD_LINE_Y                  (CALL_FIRST_LINE_Y +2*CALL_LINE_HIGHT )
#define CALL_FOURTH_LINE_Y               (CALL_FIRST_LINE_Y +3*CALL_LINE_HIGHT )

/*����ȥ�綯��ͼƬY����*/
#define CALL_ANIM_IMG_Y                  (CALL_FIRST_LINE_Y + 2*CALL_LINE_HIGHT+CALL_LINE_HIGHT/2)

/*���Ŵ�����ͼƬ�о�*/
#define BETWEEN_LINE_PIXEL                (6)

#define MISSED_TEXT_Y                         CALL_ANNU_HEIGHT
#define MISSED_CALL_Y                         (CALL_ANNU_HEIGHT + pMe->m_LineHeight+2)
/*Temp add */
#define FEATURE_APP_PAUSE_TIMER
#define FEATURE_IMAGE_DIALING_DIGITS
#ifdef FEATURE_IMAGE_DIALING_DIGITS
#if defined(FEATURE_DISP_176X220)
    #define NUM_IMAGE_HIGHT                    (30)
    #define NUM_IMAGE_WIDTH                    (20)
    #define REFUI_CALL_MAX_IMAGSIZE       (16)
#elif defined(FEATURE_DISP_128X128)
	#define NUM_IMAGE_HIGHT                    (25)
    #define NUM_IMAGE_WIDTH                    (20)
    #define REFUI_CALL_MAX_IMAGSIZE       (16)
#elif defined(FEATURE_DISP_160X128)
	#define NUM_IMAGE_HIGHT                    (25)
    #define NUM_IMAGE_WIDTH                    (20)
    #define REFUI_CALL_MAX_IMAGSIZE       (16)
#else
    #define NUM_IMAGE_HIGHT                    (25)
    #define NUM_IMAGE_WIDTH                    (20)
    #define REFUI_CALL_MAX_IMAGSIZE       (16)
#endif
#endif
//#define KEYSND_ZY
//#define FEATURE_UTK2
//#define FEATURE_PERU_VERSION
#define FEATURE_VOICE_PRIVACY_ICON
//#define T_MSM6000
//#define FEATURE_CARRIER_VENEZUELA_MOVILNET
//#define FEATURE_LARGE_DIALING_DIGITS
//#define FEATURE_IS2000_SCC_CODES
/*Temp add*/
#define DIALER_PAUSE_AECHAR             ((AECHAR)'P')
#define DIALER_PLUS_AECHAR               ((AECHAR)'+')
#ifdef FEATURE_APP_PAUSE_TIMER
#define DIALER_TIMER_AECHAR              ((AECHAR)'T')
#endif
#define FEATURE_EDITABLE_NUMBER

#define FEATURE_DIALER_ANIMAION_SUPPORT	0

/*==============================================================================
                                 ���Ͷ���
==============================================================================*/
typedef struct
{
   uint8 nOn;
   uint8 nOff;
} DTMFToneDuration;
#ifndef Temp_remove
//#define OEMEVT_USER_HEADSET 1111
//#define AVK_USER_HEADSET 5555

typedef struct
{
#ifdef Temp_remove
    PhoneTimeState         timeState;
#endif/*Temp_remove*/
    int                    lastSrvState;
    int                    lastRoamState;
    boolean                IsEmgCall;
    boolean                uimLocked;
    boolean                inLPM;
} idle_state_info_type;
#endif/*Temp_remove*/
//�����ȼ�
typedef enum CVolumeLevel
{
    VOLUME_OFF,
    VOLUME_ONE,
    VOLUME_TWO,
    VOLUME_THREE,
    VOLUME_FOUR,
    VOLUME_FIVE,
    VOLUME_SIX,
    VOLUME_SEVEN
} CVolumeLevel;

// ͨ����������ʾ��������С����Ҫ�ڵ���ʱ��ʾ��
typedef enum ERefreshVol
{
    REFRESHVOL_OFF,
    REFRESHVOL_WAIT,
    REFRESHVOL_ON
} ERefreshVol;

/*----------------------�Ի������������������---------------------*/
// �Ի���ر�ʱ����ֵ�б�
typedef enum DLGRetValue
{
   // ��ʼֵ��������Ҫ�����Ի���
   DLGRET_CREATE,

   DLGRET_CALL_ATTEMPT_FAILED,     // Returned by several dialogs when
   DLGRET_DIALER_INVALID_EMGNUMBER,// Returned by the Dialer dialog when
                                   // an invalid emergency number is dialed
                                   // while in the 'Emergency Only' dialer
   DLGRET_NUMEDIT,                 /*3*/
   DLGRET_CALLING,          //4          
   DLGRET_INCOMING,                //5
   DLGRET_CONNECT,                 //6
   DLGRET_CALL_END,                //7
   DLGRET_DIALER_RESTRICT,         //8
   DLGRET_CONV_DIAL,               //9
   DLGRET_BACK_TO_IDLE,            // Returned by Incoming Call dialog10
   DLGRET_CONNECT_OR_NUMEDIT,      //11
   DLGRET_MSGBOX,
   DLGRET_SEL_IP_NUMBER,
   DLGRET_EMGNUMBER_CALL_ONLY,
   DLGRET_NUM_TOO_LONG,
#ifdef FEATURE_SUPPORT_WAP_APP
#ifdef FEATURE_CARRIER_CHINA_TELCOM
   DLGRET_INVALID_LINK_NUM,
#endif
#endif
   // �Ի���ر�ʱ�ɷ��ص�ͨ��ֵ
   DLGRET_OK,
   DLGRET_CANCELED
} DLGRet_Value_e_Type;

/*----------------------״̬�����������������---------------------*/
// CallApp Applet ״̬��״̬��
typedef enum _CallAppState
{
   STATE_NULL,
   STATE_INIT,            /*1*/
   STATE_CALLING_FROM_ANOTHERAPP,      /*2*/
   STATE_NUMBER_FROM_IDLE,/*3 This state is  the first digt from coreApp*/
   STATE_OUT_RESTRICT,    /*4*/
   STATE_CALLING,         /*5*/
   STATE_CONVERSATION,    /*6*/
   STATE_CONV_DIALER,     /*7 DTMF*/
   STATE_ENDCALL,         /*8*/
   STATE_MISSEDCALL,      /*9*/
   STATE_INCOMINGCALL,    /*10 a*/
#ifdef FEATURE_UTK2
   STATE_CALLCONFIRM,     /*11b */
#endif
   STATE_SEL_IP_NUMBER,//12 c
   //STATE_NUMEDIT_OPTION,//12 c
   //STATE_CONNECT_OPTION,//13 d
   //STATE_INCOMING_OPTION,//14 e
#ifdef FEATURE_EDITABLE_RECORD
   STATE_EDIT_REC_NUMBER,
#endif
   STATE_EXIT             /*13d*/

} CallAppState;

// ״̬���������ظ�״̬������������ֵ����
typedef enum NextFSMAction
{
   NFSMACTION_WAIT,
   NFSMACTION_CONTINUE
} NextFSMAction;

// ����ģ����Ϣ��ģ�����ü����Ľṹ
typedef struct CallAppMod
{
    DECLARE_VTBL(IModule)
    uint32   m_nRefs;
} CallAppMod;

typedef struct  _Dialer_call_table
{
    AECHAR     call_number[AEECM_MAX_DIGITS_LENGTH];
    AECHAR     call_name[MAX_SIZE_NAME_TEXT];
    uint8      call_id;
    uint32     start_time;
    uint32     end_time;
    //uint16     call_status;
    uint16     call_type;
    callPIType type;
    char       picture_name[AEE_MAX_FILE_NAME];
    AECHAR       ringer[MAX_FILE_NAME];
    boolean    in_phonebook;
    boolean    b_emerg;
    boolean    b_restict;
    struct _Dialer_call_table *next;
}Dialer_call_table;
typedef struct  _Dialer_cont_cache
{
    AECHAR     call_number[AEECM_MAX_DIGITS_LENGTH];
    AECHAR     call_name[MAX_SIZE_NAME_TEXT];
    char       picture_name[AEE_MAX_FILE_NAME];
    AECHAR       ringer[MAX_FILE_NAME];
}Dialer_cont_cache;
typedef struct _cdg_dsp_info
{
    boolean               b_last_rec;
    boolean               m_b_show_cdg;
    AEECMEvent        event;
    AEECMDispType   disp_type;
    AEECMPI             pi;
    AECHAR              alpha[AEECM_MAX_ALPHA_TAG_LENGTH];    
}
cdg_dsp_info;
enum
{
    RETURN_NONE,
    RETURN_ZERO,
    RETURN_POUND,
    RETURN_STAR,
    RETURN_P,
#ifdef FEATURE_APP_PAUSE_TIMER
    RETURN_T,
#endif
    RETURN_PLUS,
    RETURN_MAX
};
typedef enum _MAKE_CALL_VALUE
{
    CALL_SUCESS = 0x00,
    CALL_FAIL_EMERGENCY_MODE,
    CALL_FAIL_RESTICT,
    CALL_FAIL_INVIDE_NUMBER,
    CALL_FAIL_ANOTHER
}MAKE_CALL_VALUE;
#ifdef FEATURE_SUPPORT_BT_APP
typedef enum _send_bt_audio
{
    SEND_NONE= 0x00,
    SEND_BT_CALL,
    SEND_BT_AUDIO
}send_bt_audio;
#endif
// CallApp Applet����ṹ�壺
typedef struct _CCallApp
{
    DECLARE_VTBL(ICallApp)
    uint32                  m_nRefs;          // �������ü�����
    IModule                *m_pModule;
    IDisplay               *m_pDisplay;
    IShell                 *m_pShell;

    ISound                 *m_pSound;      // Sound interface
    IALERT                 *m_pAlert;      // IAlert interface
    IConfig                *m_pConfig;     // IConfig interface
    //IStatic                *m_IStatic;       // Used in the IDD_SILENCED& IDD_MSGBOX dialog
    ICM                    *m_pICM;                         // instance of ICM
    //IAnnunciator         m_pIAnn;
    AEERect                m_rc;
    IDialog                *m_pActiveDlg;
    uint16                 m_pActiveDlgID;
    // ��Ի���ر�ʱ�ķ��ؽ��
    DLGRet_Value_e_Type    m_eDlgRet;
    // �Ƿ񲻸�д�Ի��򷵻ؽ����һ�������Ҫ��д(FALSE)��
    boolean                m_bNotOverwriteDlgRet;
    // Applet ǰһ״̬
    CallAppState         m_ePreState;
    // Applet ��ǰ״̬
    CallAppState         m_eCurState;
    // Applet�Ƿ��ڹ���״̬
    boolean                m_bSuspending;

#ifdef FEATURE_SUB_LCD
    IDisplaySub            *m_pDisplaySub;
    AEERect                m_RectSub;
    IBitmap                *m_pWallSubImage;
#endif

    //IImage                *m_pBgImage;
    IImage                *m_pCallingImage;
    //IImage               *m_pDialerImage;
    IImage                 *m_pConvImage;
    //IImage               *m_pCallendImage;


#ifdef Temp_remove
    IIdle                   *m_pIdle;
#endif/*Temp_remove*/
    //AECHAR                  *m_msg_text;      //Used by message box. Set this text before
    //showing the message box
    // while the Idle applet is suspended.

    int                     m_LineHeight;       // Height of each text line for normal font
    //int                     m_LargeLineHeight;  // Height of each text line for large font


    // Remaining DTMF digits to send after the pause has been acked
    AECHAR                  m_PauseString[MAX_SIZE_DIALER_TEXT];
#ifdef FEATURE_APP_PAUSE_TIMER
    int                     m_PauseTimer;
    AECHAR                  m_TimerString[MAX_SIZE_DIALER_TEXT];
#endif

    AECHAR                  m_RedirNumStr[AEECM_MAX_DIGITS_LENGTH];  // redir_num_rec contents
    //AECHAR                  m_DispInfo[AEECM_MAX_DIGITS_LENGTH+1];
    //for CDG test, CNAP with Forwarding
    // Presentation indicator for m_RedirNumStr
    callPIType              m_RedirectingNumberPI;
    // Extended Display record contents
    //AECHAR                  m_ExtendedDispInfo[AEECM_MAX_DIGITS_LENGTH+1];
    //AECHAR                  m_ExtendedDispInfo2[AEECM_MAX_DIGITS_LENGTH+1];
    //AECHAR                  *m_ScroolStr;
    int                     m_cdg_row;
    // Current number entered in the dialer dialog
    AECHAR                  m_DialString[MAX_SIZE_DIALER_TEXT];//IDLE input number ,can include 'P'  'T'

    AEECMCallState           m_lastCallState;
    boolean                  m_Is3Way;     // TRUE if the current call was a three-way call
    uint32                   m_LastMinuteAlert;//ÿ������ʾ��

    byte                      m_CallVolume; // current volume for active call
    boolean                   m_CallMuted;
    boolean                   m_userCanceled;   // TRUE if the user pressed END while in the
    uint32                    m_MissedCallCount;

    AEECMEndStatus            m_callEndStatus;
    boolean                   m_callEndInOrig; // TRUE if the last call was ended
    boolean                   m_makeCallAfterOTAPA; // TRUE if CallApp_MakeCall() should be
    // invoked after the OTAPA call is torn down

    // Dynamically allocated array of tones, used to give audible feedback
    // to the user when a hard pause is released.
    AEESoundToneData          *m_toneData;
    int                       m_nToneCount;
    int                       m_nToneCurrIndex;

#ifdef FEATURE_IS2000_SCC_CODES
    // TRUE when the call is on 'Answer Hold'
    boolean                   m_bAnswerHold;
#endif /* FEATURE_IS2000_SCC_CODES */
    // TRUE when the incoming call has been put on 'Answer Hold'

    boolean                   m_bmissCallWaiting;  //���еȴ�ʱ�����Ƿ�Ҫдδ�Ӻ��롣TRUE:дδ�Ӻ��롣
    boolean                   m_running;       // Set to TRUE between EVT_APP_START and state init
    IMedia *                  m_pMedia;         // IMedia-based object
    boolean                   m_bCloseAllApplet;
    start_call_type           m_nStartCallType;
    boolean                   incoming_id; //�ж������Ƿ�Ҫ��������
    //boolean                   IncomIsready;//���������ٶ�
    boolean                   IsStartCall;
    boolean                   IsRestrictNumber;
    //�����ȼ�
    CVolumeLevel              m_bVolumeLevel;
    AEEAppStart               *AppStart;
    IBacklight                *m_pBacklight;
#ifdef KEYSND_ZY
    ISoundPlayer               *m_SndPlayer;       //���Ŷ�ʰ������ӿ�
#endif /* KEYSND_ZY */

    boolean                    m_bIsPrivacy;

#ifdef  FEATURE_PERU_VERSION
    //to save another incoming call in conversation;
    boolean                    in_convert;
#endif

    AECHAR                     wszPrivateString_tw[20];
    ERefreshVol                m_bRefreshVol;      //for display conversation text

    //IFont                      *m_pBigNumFont;       //IFont interface for the dialing big number
    int                        m_large_Num_Height;   //Dialing Number's Height,onle used for NumEdit
    boolean                    m_bShowPopMenu;//indicate current focus is pop_menu
    IMenuCtl                  *m_pMenu;          // pop menu
    boolean                    m_bHandFree;    //���������Ƿ������������

#ifdef FEATURE_UTK2
    boolean                    m_bEndcurcallForUTKCall;
#endif
    Dialer_call_table          *m_CallsTable;//always point the link_head
    int                        m_CallsTable_Count;//link count
    idle_state_info_type       idle_info;
    boolean                    m_bReceiveCallerID;
    boolean                    m_btime_out;
    byte                       m_return_value;
    uint16                     m_bincoming_rsk;
    //boolean                    m_b_show_cdg;
    int16                      m_b_callend_time;
    boolean                    m_bprocess_held;
    byte                       m_anykey_answer;
    Dialer_call_table          m_call_info;
    boolean                    m_b_incoming;
    IAnnunciator               *m_pIAnn;
    boolean                    m_b_auto_redial;
    uint16                     m_auto_redial_count;
    boolean                    m_b_incall;//is in connect??
    uint16                     m_msg_text_id;
    boolean                    m_b_from_numedit;
    boolean                   m_b_draw_dot;
#ifdef FEATURE_IMAGE_DIALING_DIGITS
    IImage                     *num_image[REFUI_CALL_MAX_IMAGSIZE];
#endif
#ifdef FEATRUE_SET_IP_NUMBER
    boolean                    m_b_have_set_ip;
    AECHAR                    m_b_ip_call[OEM_IP_NUMBER_MAX +1];
#endif
#ifdef FEATRUE_AUTO_POWER
    boolean                    m_b_powerdown;
#endif
    keyToneLength              m_dtmf_length;
    callPIType                    m_b_is_PI_ALLOWED;
    boolean                    m_b_miss_notify;
    cdg_dsp_info             m_cdg_dsp_info;
#ifdef FEATURE_KEYGUARD
    boolean                    m_b_set_keyguard;
#endif
    //boolean                    m_b_from_another;
    boolean                    m_b_press_1;
#ifdef FEATURE_SUPPORT_BT_APP
    send_bt_audio          m_bt_audio;
    boolean                    m_b_add_btag_menu;
#endif
    AECHAR      *m_pwstrDialStringkeep;


#if defined( FEATURE_CALL_RECORDER)
    Media						m_Media;
    boolean						m_bRecorderOn;
    IImage*						m_pImage[6];
    boolean						m_bSettingVolume;
#endif

#ifdef FEATURE_EDITABLE_NUMBER
    int                           m_nCursorPos;
#endif
#ifdef FEATURE_EDITABLE_RECORD
    boolean                    m_bEditRecNumber;
#endif
    Dialer_cont_cache              m_cont_cache;
#ifdef WIN32//wlh for VKey_Number
	IVkeyCtl * m_pIVkeyCtl;
#endif
} CCallApp;

/*==============================================================================
                                 �궨��ͳ���
==============================================================================*/
// Ϊ��ֹ�û����ٰ���������¼�EVT_APPISREADY���¼��ɶ�ʱ��
#define APPISREADY_TIMER        200
#define EVT_APPISREADY          (EVT_USER+10)

//�رճ����¼�(�������ͨ����Post��Call)
//#define EVT_USER_CLOSEAPP       (EVT_USER+13)
//����ʱ�����ٶ�
//#define EVT_INCOMISREADY        (EVT_USER+14)
//#define DIALER_MEM_CHECK
// ������ײ���ʾ���궨��
#define REFUI_DRAW_BOTTOMBAR(x)                           \
{                                                   \
    BottomBar_Param_type BarParam;                  \
    MEMSET(&BarParam, 0, sizeof(BarParam));         \
    BarParam.eBBarType = x;                         \
    DrawBottomBar(pMe->m_pDisplay, &BarParam);      \
}
#define CALLAPP_MALLOC(x) CallApp_Malloc((x), __LINE__)
#define CALLAPP_FREE(x) CallApp_Free((x))
// ״̬�ƶ��꣺�Ƚ�ǰһ״̬�õ�ǰ״̬���£��ٽ���ǰ״̬��ΪnextState
#define MOVE_TO_STATE(nextState) {            \
            CallAppState tpState;            \
            tpState = nextState;          \
            pMe->m_ePreState = pMe->m_eCurState; \
            pMe->m_eCurState = tpState;        \
        }
#define CLOSE_DIALOG(DlgRet)  {    \
                                     pMe->m_eDlgRet = DlgRet; \
                                     (void) ISHELL_EndDialog(pMe->m_pShell);  \
                              }
#define MAXIMUM_TIMES_OF_REDIAL               5
#define MAX_SPEEDDIAL_CHARS               (1)

// How long a CDMA display event message is shown to the user
#define TIMEOUT_MS_DISPLAYEVENT           5000
#ifdef FEATURE_FDN
// Time until the Call Origination Not Allowed closes automatically
#define TIMEOUT_MS_MO_NOT_ALLOWED_SCREEN  (4*TIMEOUT_MS_ONE_SECOND)
#endif /* FEATURE_FDN */

// Time until the EndCal dialog closes automatically
#define TIMEOUT_MS_ENDCALLTIMER            3000

// Time between CallApp_HandleEndCallBlinkTimer() callbacks
#define TIMEOUT_MS_ENDCALLBLINKTIMER        500

// Time in milliseconds between CallApp_HandleConversationTimer() callbacks
#define TIMEOUT_MS_CONV_TIMER               (TIMEOUT_MS_ONE_SECOND)

// Time in milliseconds that the "Invalid Emergency Number" dialog will be
// displayed
#define TIMEOUT_MS_INVALIDEMGNUMDIALOG_TIMER (2*TIMEOUT_MS_ONE_SECOND)

// Time before the In-band Incoming Call Dialog is automatically closed.
#define TIMEOUT_MS_INBANDCALL_TIMEOUT        (40*TIMEOUT_MS_ONE_SECOND)

// Time (in milliseconds) that each tone in a pause string is played
// to the user through the speaker or earpiece.
//
// Note that this value has nothing to do with the duration of the DTMF
// tones sent across the network.
#define PAUSE_TONE_DURATION                  250

// Which second (in a minute) that the minute reminder should start on.
// Currently set to second 50 (ie, 10 seconds before the next minute)
#define MINUTE_REMINDER_SECOND               50


//#define INCOMINGCALLISREADY_TIMER            500

//#define INCOMING_IMAGE "image\\incomingcall.bmp"

//#define CALL_IMAGE "image\\aboutcall_160.bmp"

//#define WARNING_IMAGE "image\\warning.bmp"

//#ifdef FEATURE_APP_PAUSE_TIMER
#define CALL_TIMER_SET                       3000    //Set Timer of T key
//#endif
//#define DIALER_DUBEG
#ifdef FEATURE_OEM_DEBUG
    //#define CALL_PRINT(str,str1,str2)        CallApp_Printf(str,str1,str2)
    #define CALL_ERR( str, a, b, c )         ERR( str,a,b,c )
    #define CALL_FUN_START( str, a, b, c )   ERR( str,a,b,c )
    #define CALL_PRINTF  DBGPRINTF
#else
    //#define CALL_PRINT(str,str1,str2)
    #define CALL_ERR( str, a, b, c )
    #define CALL_FUN_START( str, a, b, c )
    #define CALL_MSG_HIGH( str, a, b, c )
    #define CALL_MSG_LOW( str, a, b, c )
    #define CALL_PRINTF( str,... )
#endif


/*==============================================================================
                                 ȫ������
==============================================================================*/
//extern uint8   TMP_RSSI_VALUE;
//extern boolean Tmp_ROAM_Off;
//extern boolean Globe_Media_Start;  //�Ӷ�ý�岥�Ž��룬���Ϸ��ǣ����ܹ��Ҷϵ绰��
//extern boolean bFlipClosed;    //�жϷ����Ƿ�ر�
//extern boolean Camera_quick_in;
/*==============================================================================
                                 ���أ���̬������
==============================================================================*/

/*==============================================================================
                                 ��������
==============================================================================*/
/*----------------------�Ի�����غ�������---------------------*/
//Create dialog according to dialog ID
void CallApp_ShowDialog(CCallApp *pMe,uint16  dlgResId);

//Sets the current sound path
void CallApp_SetupCallAudio(CCallApp *pMe);

//Handles the details of making a call
MAKE_CALL_VALUE CallApp_MakeCall(CCallApp *pMe);

//Timer callback used by the conversation dialog to update
//the call timer every second.
void CallApp_HandleConversationTimer(void *pUser);

// Start an applet
boolean CallApp_LaunchApplet(CCallApp   *pMe, AEECLSID  classID);
//Play next tone
void CallApp_NextTonePlay(void *pUser);

//Increase or decreases the current call volume
void CallApp_ChangeCallVolume(CCallApp  *pMe,  boolean      louder);

//Answer an incoming or in-band call
boolean CallApp_AnswerCall(CCallApp  *pMe,
                                        boolean bAnswerHold,
                                        AEEEvent eCode,
                                        uint16 wParam,
                                        boolean auto_answer);

//look up name ringer picture from phonebook according number
boolean CallApp_NumberLookup(CCallApp      *pMe,
                             AECHAR        *Number,
                             AECHAR        *Name,
                             uint32         szName,
                             AECHAR        *RingerID,
                             char          *PictureName);

//judge that if the number is restricted
boolean CallApp_IsRestictCallNumber_Ex(CCallApp   *pMe,
                                        AECHAR       *pNumber,
                                        boolean      IsOutgoing);

boolean CallApp_IsEmergencyMode(ICM* pICM);

boolean CallApp_IsEmergency_Number(AECHAR *pNumber);

#ifdef KEYSND_ZY
//��ʰ�����
void CallApp_SpecialKeySnd(CCallApp *pMe, uint16 wParam);
#endif /* KEYSND_ZY */

boolean CallApp_RouteDialogEvent(CCallApp *pMe,
                                        AEEEvent    eCode,
                                        uint16      wParam,
                                        uint32      dwParam);

NextFSMAction CallApp_ProcessState(CCallApp *pMe);

int CallApp_Compare_Number(AECHAR * Number1, AECHAR * Number2);

Dialer_call_table * CallApp_Search_Number_In_Call_Table(CCallApp *pMe,AECHAR *number);

boolean CallApp_Add_Number_To_Call_Table(CCallApp *pMe,AECHAR *number,
                                        uint8 call_id,
                                        uint16 call_type,
                                        //uint16 call_status,
                                        callPIType type,
                                        boolean b_force_add ,boolean is_energency, boolean is_restict);

boolean CallApp_Modify_Number_To_Call_Table(CCallApp *pMe,AECHAR *number,
                                        uint8 call_id, uint16 call_type/*uint16 call_status*/,callPIType type);

boolean CallApp_Get_CallName_In_Call_Table(CCallApp *pMe,AECHAR *name);

Dialer_call_table* CallApp_Get_First_Entry_In_Call_Table(CCallApp *pMe);

void CallApp_Free_All_Call_Table(CCallApp *pMe);

void CallApp_Change_Call_Table_Call_Start_Time(CCallApp *pMe,AECHAR *number);

void CallApp_Change_Call_Table_Call_End_Time(CCallApp *pMe,AECHAR *number);

void CallApp_Reset_Call_Table_Call_End_Time(CCallApp *pMe,AECHAR *number);

void CallApp_Change_Call_Table_All_Call_End_Time(CCallApp *pMe);

void CallApp_Change_Call_Table_Call_ID(CCallApp *pMe,AECHAR *number,uint8 call_id);

void CallApp_Change_Call_Table_Call_History_State(CCallApp *pMe,
                                        AECHAR *number,
                                        uint16 type/*uint16 status*/);

void CallApp_Add_All_Call_History(CCallApp *pMe);

void CallApp_DrawTextWithProfile(IShell* pShell,
                                        IDisplay * pDisplay,
                                        RGBVAL ProfileColor,
                                        AEEFont Font,
                                        const AECHAR * pcText,
                                        int nChars,
                                        int x,
                                        int y,
                                        const AEERect * prcBackground,
                                        uint32 dwFlags
                                        );


//void CallApp_Init_Msgbox_String(CCallApp *pMe,uint16 str_id,AECHAR * pach_string);
void CallApp_ShowMsgBox(CCallApp *pMe, uint16 str_id);

void CallApp_HandleAutoAnswerTimer(void *pUser);

//Timer callback to flash the call timer.
void CallApp_HandleEndCallBlinkTimer(void *pUser);
void * CallApp_Malloc(uint32 size, int line);

void CallApp_Free(void *p) ;
#ifdef DIALER_MEM_CHECK
void CallApp_show_Mem(CCallApp *pMe);
#endif
//#ifdef DIALER_DUBEG
//void CallApp_Printf(const char * file_name,AECHAR * string1,AECHAR * string2);
//#endif
boolean CallApp_Process_EVT_FLIP_Event(CCallApp *pMe, uint32 wParam);
#ifdef FEATURE_SUPPORT_BT_APP
void bt_ui_process_incall_org_state(void);
boolean bt_ui_process_vol_change(byte vol);
boolean bt_ui_process_cmcall_notify(CCallApp *pMe,uint32 event/*,void  *pCallInfo*/,boolean cm_event);
#endif
void CallAppNotifyMP3PlayerAlertEvent(CCallApp *pMe, boolean toStartAlert);
#endif
//CALLAPP_PRIV_H

