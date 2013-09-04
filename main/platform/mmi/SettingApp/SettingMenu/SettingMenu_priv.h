#ifndef SETTINGMENUPRIV_H
#define SETTINGMENUPRIV_H

/*==============================================================================
// �ļ���
//        SettingMenu_priv.h
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
#ifndef AEE_SIMULATOR
#include "customer.h"   //Handset Feature Definitions
#else
#include "BREWSimFeatures.h"   //Simulator Feature Definitions
#endif

#if !defined(BREW_STATIC_APP)
// Use the .bid file if compiling as dynamic applet
   #include "SettingMenu.bid"
#else
   #include "OEMClassIds.h"  // Applet Ids 
   #ifndef AEECLSID_APP_SETTINGMENU
      #error AEECLSID_APP_SETTINGMENU must be defined
   #endif
#endif

#include "SettingMenu.brh"
#include "AEEShell.h"
#include "AEEModTable.h"
#include "AEEStdLib.h"
#include "AEEModGen.h"
#include "AEEAppGen.h"
#include "OEMClassIDs.h"
//#include "OEMMacros.h"
#include "SettingMenu.h"

#include "AEEMenu.h"
#include "AEEConfig.h"
#include "AppComFunc.h"
#include "AEEText.h"
#include "AEEAnnunciator.h"
//#include "AEEPhone.h"//add by lzhl20040823
#include "SoundMenu.h"
#ifdef FEATURE_USES_ZI
#include "OEMZIText.h" 
#else
#include "OEMText.h" 
#endif
#include "OEMFeatures.h"
#include "OEMCFGI.h"
#include "AppComFunc.h"
#include "Appscommon.h"
#include "AEERUIM.h"
#include "OEMNV.h"


/*==============================================================================
                                 ���Ͷ���
==============================================================================*/
#define  AEE_APPSSETTINGMENU_RES_FILE AEE_RES_LANGDIR SETTINGMENU_RES_FILE
#define SETTING_ERR(format, code1, code2, code3) MSG_FATAL(format, code1, code2, code3)

// ������ײ���ʾ���궨��
#define SETTING_MENU_DRAW_BOTTOMBAR(x)                           \
{                                                   \
    BottomBar_Param_type BarParam;                  \
    MEMSET(&BarParam, 0, sizeof(BarParam));         \
    BarParam.eBBarType = x;                         \
    DrawBottomBar(pMe->m_pDisplay, &BarParam);      \
}
/*----------------------�Ի������������������---------------------*/
// �Ի���ر�ʱ����ֵ�б�
typedef enum DLGRetValue
{
    DLGRET_CREATE,
    DLGRET_OK,  
    DLGRET_CALLSETTING,		   //IDD_CALLSETTING_MENU    
    DLGRET_PHONESETTING,	      //IDD_PHONESETTING_MENU
    DLGRET_CALLSETTINGSEL,     //IDD_CALLSETTINGSEL    
    DLGRET_NETWORKSETTING,  	//IDD_NETWORKSETTING_MENU   
    DLGRET_DIVERT,	            //IDD_DIVERT_MENU   
    DLGRET_CALLFORWARDSEL,     //IDD_CALLFORWARD_SEL_MENU
    DLGRET_CALLFORWARDINPUT,   //IDD_CALLFORWARD_INPUT_MENU      
    DLGRET_AKG,	               //IDD_AKG_DIALOG
    DLGRET_BANNER,             //IDD_BANNER_DIALOG
#ifdef FEATURE_TIME_DATA_SETTING
    DLGRET_TIMESETTING,
    DLGRET_DATESETTING,
#endif
#if defined(FEATURE_VERSION_C337)||defined(FEATURE_VERSION_W317A)
	DLGRET_TRACKERSMSSETTING,
#endif

#ifdef FEATURE_LANGUAGE
    DLGRET_LANGUAGE,	         //IDD_LANGUAGE_MENU   
#endif
    DLGRET_WARNING,            //IDD_WARNING_MESSEGE
#ifdef FEATURE_PLANEMODE
    DLGRET_PLANEMODE,            //IDD_PLANEMODE
#endif
#ifdef FEATURE_PERU_VERSION
    DLGRET_PHONE_NUMBER,
#endif
#ifdef FEATRUE_AUTO_POWER
    DLGRET_AUTO_POWER,
#endif
    DLGRET_AUTO_ANSWER_MODE,
#ifdef FEATRUE_SET_IP_NUMBER
    DLGRET_IP_NUMBER_SET,
#endif
    DLGRET_KEYLENGTH,
    DLGRET_AUTO_ANSWER_MODE_SUB,
    DLGRET_CANCELED,
    DLGRET_FMMODE,
#ifdef FEATURE_SUPPORT_G_SENSOR
    DLGRET_SHAKE,
    DLGRET_SHAKESUB,
#endif
    DLGRET_CALLRESTRICT,
    DLGRET_PASSWORD,
    DLGRET_INVALID,
    DLGRET_RESTRICTOUTGOING,
    DLGRET_RESTRICTINCOMING,
    DLGRET_MSGBOX_OK,
    DLGRET_MSGBOX_CANCEL
    #ifdef  FEATURE_DOUBLE_SIM_CARD
    ,DLGRET_SIMSETTING,
    DLGRET_SIMSETTING_CHOICE
    #endif
    #ifdef FEATURE_VERSION_MYANMAR
    ,DLGRET_SEARCHMODE
    ,DLGRET_TIMEFONTMODE
    #endif
#ifdef FEATURE_VERSION_W317A 
    ,DLGRET_AUTOCALLRECORD     //Add by pyuangui 20130104
#endif
#ifdef FEATURE_VERSION_W208S
    ,DLGRET_SMSRESTRICT
//    ,DLGRET_SMSRESTRICT_SEND
    ,DLGRET_SMSRESTRICT_RECEIVE
    ,DLGRET_SMSRESTRICT_RECEIVE_ADD
#endif
#ifdef FEATURE_SHOW_PHONE_INFO
    ,DLGRET_PHONE_INFO
    ,DLGRET_PHONE_INFO_SW    
    ,DLGRET_PHONE_INFO_HW
    ,DLGRET_PHONE_INFO_PRL
#endif	
#ifdef FEATURE_SOUND_BO
    ,DLGRET_SPEECH_SETTINGS   //IDD_SPEECH_MENU
#endif 

} DLGRet_Value_e_Type;

/*----------------------״̬�����������������---------------------*/
// SettingMenu Applet ״̬��״̬��
typedef enum _SettingMenuState
{
   SETTINGMENUST_NONE,
   SETTINGMENUST_INIT,
   SETTINGMENUST_MAIN,
   SETTINGMENUST_CALLSETTING,
   SETTINGMENUST_PHONESETTING,
   SETTINGMENUST_CALLSETTINGSEL,
   SETTINGMENUST_NETWORKSETTING,
   SETTINGMENUST_DIVERT,
   SETTINGMENUST_CALLFORWARDSEL,
   SETTINGMENUST_CALLFORWARDINPUT,
   SETTINGMENUST_AKG,
   SETTINGMENUST_BANNER,
#ifdef FEATURE_TIME_DATA_SETTING
   SETTINGMENUST_TIMESETTING,
   SETTINGMENUST_DATESETTING,
#endif
#if defined(FEATURE_VERSION_C337)||defined(FEATURE_VERSION_W317A)
   SETTINGMENUST_TRACKERSMSSETTING, 	//TRRACK SMS����
#endif

#ifdef FEATURE_LANGUAGE
   SETTINGMENUST_LANGUAGE,
#endif
#ifdef FEATURE_PERU_VERSION
   SETTINGMENUST_PHONENUMBER,
#endif
#ifdef FEATRUE_AUTO_POWER
   SETTINGMENUST_AUTO_POWER,
#endif
#ifdef FEATURE_SUPPORT_G_SENSOR
   SETTINGMENUST_SHAKE,
   SETTINGMENUST_SHAKESUB,
#endif
   SETTINGMENUST_AUTO_ANSWER,
   SETTINGMENUST_FMMODE,
#ifdef FEATRUE_SET_IP_NUMBER
   SETTINGMENUST_IP_NUMBER_SET,
#endif
   SETTINGMENUST_KEYLENGTH,
   SETTINGMENUST_AUTOANSWERSUB,
   SETTINGMENUST_EXIT,
   SETTINGMENUST_PASSWORD,
   SETTINGMENUST_CALLRESTRICT,
   SETTINGMENUST_RESTRICTOUTGOING,
   SETTINGMENUST_RESTRICTINCOMING
#ifdef FEATURE_PLANEMODE
   ,SETTINGMENUST_PLANEMODE
#endif
#ifdef  FEATURE_DOUBLE_SIM_CARD
   ,SETTINGMENUST_SIMSETTING,
   SETTINGMENUST_SIMSETTING_CHOICE
#endif
#ifdef FEATURE_VERSION_MYANMAR
   ,SEARCHMENUST_MODE
   ,TIMEFONTMENUST_MODE
#endif
#ifdef FEATURE_VERSION_W317A 
   ,SETTING_AUTOCALLRECORD     //Add by pyuangui 20130104
#endif
#ifdef FEATURE_VERSION_W208S
   ,SETTINGMENUST_SMSRESTRICT
//   ,SETTINGMENUST_SMSRESTRICT_SEND
   ,SETTINGMENUST_SMSRESTRICT_RECEIVE
   ,SETTINGMENUST_SMSRESTRICT_RECEIVE_ADD
#endif
#ifdef FEATURE_SHOW_PHONE_INFO
   ,SETTINGMENUST_PHONE_INFO
   ,SETTINGMENUST_PHONE_INFO_SW
   ,SETTINGMENUST_PHONE_INFO_HW
   ,SETTINGMENUST_PHONE_INFO_PRL
#endif   
#ifdef FEATURE_SOUND_BO
   ,SETTINGMENUST_SPEECH
#endif
} SettingMenuState;

// ״̬���������ظ�״̬������������ֵ����
typedef enum NextFSMAction
{
   NFSMACTION_WAIT,
   NFSMACTION_CONTINUE
} NextFSMAction;

// ����ģ����Ϣ��ģ�����ü����Ľṹ
typedef struct SettingMenuMod
{
    DECLARE_VTBL(IModule)
    uint32   m_nRefs;
} SettingMenuMod;

// ����ҵ������
typedef enum {
   CALLFORWARD_NONE,
   CALLFORWARD_BUSY,        //��æת��
   CALLFORWARD_NOANSWER,    //��Ӧ��ת��
   CALLFORWARD_NOCONNECT,   //δ��ͨת��
   CALLFORWARD_ANYWAY,      //������ת��
#ifdef FEATURE_CARRIER_SUDAN_SUDATEL
   CALLFORWARD_CNIR,
   CALLFORWARD_DND,
   CALLFORWARD_SPINA,
   CALLFORWARD_SPINI,
#endif
   CALLFORWARD_WAIT         //���еȴ�
} CALLFORWARD_TYPE;

// SHAKE��������
typedef enum {
   SET_SHAKE_NONE,
   SET_MUSIC_CHECK,     //����
   SET_FM_RADIO_CHECK,     //������
   SET_WALLPAPER_CHECK,      //ͼƬ
   SET_SNOOZE_ALARM_CHECK,   //����
   SET_VIDEO_CHECK           //��Ƶ
} SET_SHAKE_TYPE;
#ifdef FEATRUE_SET_IP_NUMBER
typedef struct _number_table
{    
    AECHAR   number[OEM_IP_NUMBER_MAX+1];
    struct _number_table *next;
}number_table;

typedef struct _Ip_number_table
{
    number_table * head;
    number_table * tial;
    AECHAR  default_ip[OEM_IP_NUMBER_MAX+1];
    uint16            ip_state;
    byte              count;
}Ip_number_table;
#endif //#ifdef FEATRUE_SET_IP_NUMBER
// SettingMenu Applet����ṹ�壺
typedef struct _CSettingMenu
{
    DECLARE_VTBL(ISettingMenu)
    uint32           m_nRefs;            // �������ü�����
    IModule          *m_pModule;
    IDisplay         *m_pDisplay;
    IShell           *m_pShell;
    AEERect          m_rc;               // ��ǰ��Ի���IDialog�ӿ�ָ��
    IDialog          *m_pActiveDlg;      // ��ǰ��Ի���ID
    uint16           m_pActiveDlgID;     // ��Ի���ر�ʱ�ķ��ؽ��
    DLGRet_Value_e_Type   m_eDlgRet;     // �Ƿ񲻸�д�Ի��򷵻ؽ����һ�������Ҫ��д(FALSE)��
    boolean          m_bNotOverwriteDlgRet;// Applet ǰһ״̬
    SettingMenuState m_ePreState;        // Applet ��ǰ״̬
    SettingMenuState m_eCurState;        // Applet�Ƿ��ڹ���״̬
    boolean          m_bSuspending;      // Applet�Ƿ�׼���ÿ��Դ������������¼����ı�����Ҫ���ڿ��ٰ������������
    boolean          m_bAppIsReady;
    IAnnunciator    *m_pAnn;
    //IOEM_TSGBridge  *m_pOEM_TSGBridge;
    IConfig         *m_pConfig;
    //IPhone          *m_phone;           
    CALLFORWARD_TYPE m_CFType;          // ����ת��/�ȴ�������

    uint16           m_nResCodeID;      //���к���ID��¼
    uint16           m_currDlgId;       //���浱ǰ��ѡ�˵�λ��
    uint16           m_nResID;          //����ҵ�����ID��¼
    uint16           m_nSubDlgId;		    //�μ��˵�λ��
    byte             m_RingVol;         //��������
    uint16           m_sSubDlgId;       //�����˵�λ��
    uint16           m_CallSettingSel;
    SET_SHAKE_TYPE            m_shaketype_sel;          // SHAKE����
    char             m_callnumber[25];
#ifdef FEATRUE_AUTO_POWER
    uint16           m_auto_power;
    IMenuCtl*        m_pState;
    IMenuCtl*     m_pStatus;
    ITimeCtl         *m_pTime;
    Auto_Power_Cfg   m_ClockCfg;
    int32            m_dwDispTime;       //��Ļ����ʾ��ʱ��
    int32            m_dwHour;           //��¼�����ּ����õ�Сʱ
    int32            m_dwMin;            //��¼�����ּ����õķ���
    uint16           m_nCtlID;           //�ؼ�ID
    uint16           m_nTimeCtlCount;    //0��������Сʱ��,1�����ڷ�����
    uint16           m_nNumKeyCount;     /*��¼������ʱ��ʱ�İ�������.0:Сʱʮλ
                                        1:Сʱ��λ;   2:����ʮλ  3:���Ӹ�λ*/
#ifdef SET_REP_MOD_LIST
    IMenuCtl         *m_pRepMode;         //IMenuCtlָ��,"�ظ�ģʽ"
#endif
    uint16           m_b_ampm_mode;
    boolean          m_b_selete_Pm;
#endif
    uint16           m_input_mode;
    IRUIM            *m_pIRUIM;
#ifdef FEATRUE_SET_IP_NUMBER
    Ip_number_table  ip_number_tab;
    boolean          need_save_default;
#endif //#ifdef FEATRUE_SET_IP_NUMBER
    uint16           m_ip_sel_num;
    uint16           m_msg_id ;
    char             *m_strPhonePWD; 
    nv_item_type    nviNewSimChoice;
    nv_item_type    nviOldSimChoice;
    nv_item_type    nvsearchmode;
	#ifdef FEATURE_LCD_TOUCH_ENABLE
	AEERect pL_Rect1;
	AEERect pL_Rect2;
	AEERect pR_Rect1;
	AEERect pR_Rect2;
	#endif
#ifdef FEATURE_VERSION_W208S    
    sms_restrict_recive_info		sms_restrict_reciveList[MAX_SMS_RESTRICT];
    IMenuCtl                        *m_pMenu;
#endif
} CSettingMenu;

/*==============================================================================
                                 �궨��ͳ���
==============================================================================*/
// Ϊ��ֹ�û����ٰ���������¼�EVT_APPISREADY���¼��ɶ�ʱ��
// SettingMenu_APPIsReadyTimer����
#define APPISREADY_TIMER       100             
#define EVT_APPISREADY   (EVT_USER+1)  
#define SEARCHMODE_AOTE     0
#define SEARCHMODE_FIVE     5*60
#define SEARCHMODE_TEN      10*60
#define SEARCHMODE_TUENTY   15*60
#define SEARCHMODE_THIRTY   30*60
#define TIMEFONTMODE_BIG    1
#define TIMEFONTMODE_MEDIUM 2
#define TIMEFONTMODE_SMALL  3

// �û����¸��½����¼�
//#define EVT_USER_REDRAW   (EVT_USER+2)

//���ڶ�ʱ���ص�
//#define EVT_DIALOGTIMEOUT (EVT_USER + 3)
#ifdef FEATRUE_AUTO_POWER
//#define EVT_FOCUS_SWITCH        (EVT_USER + 4)  //�����л�

#define MSSEC_PER_HOUR          3600000//(60*60*1000)  //һСʱ�ڵĺ�����
#define MSSEC_PER_MIN           (60*1000)     //һ�����ڵĺ�����

#if defined(FEATURE_VERSION_HITZ181)||defined(FEATURE_VERSION_MTM)
#define CONTROL_RECT_START_X    45  //�������ý���Ŀؼ���ʼλ��
#elif defined(FEATURE_VERSION_X3)
#define CONTROL_RECT_START_X    55  //�������ý���Ŀؼ���ʼλ��
#elif defined(FEATURE_VERSION_K212)
#define CONTROL_RECT_START_X    65  //�������ý���Ŀؼ���ʼλ��
#elif defined(FEATURE_VERSION_EC99)
#define CONTROL_RECT_START_X    65  //�������ý���Ŀؼ���ʼλ��
#else
#define CONTROL_RECT_START_X    40  //�������ý���Ŀؼ���ʼλ��
#endif

#if defined(FEATURE_VERSION_C306)||defined(FEATURE_VERSION_W0216A)
#define CONTROL_RECT_RESET_Y    10  
#else
#define CONTROL_RECT_RESET_Y    0
#endif

#define IDC_AUTO_POWER_STATUS 1 //�������ػ�����ѡ����
#define IDC_AUTO_POWER_STATE 2
#define IDC_AUTO_POWER_TIME   3
/*ARABIC*/
#define ARABIC_CONTROL_RECT_START_X   2
#define CONTROL_RECT_START_DX        (pMe->m_rc.dx - CONTROL_RECT_START_X - ARABIC_CONTROL_RECT_START_X) //86
#define AUTO_POWER_IMG_DX            8
#define RGB_GRAY    MAKE_RGB(128,128,128)
#endif
// ״̬�ƶ��꣺�Ƚ�ǰһ״̬�õ�ǰ״̬���£��ٽ���ǰ״̬��ΪnextState
#define MOVE_TO_STATE(nextState) {            \
            SettingMenuState tpState;            \
            tpState = nextState;          \
            pMe->m_ePreState = pMe->m_eCurState; \
            pMe->m_eCurState = tpState;        \
        }

#define SETTING_MENU_SOFK_HIGHT 0
//�����ʺ�������ַ���
#define  BANNER_MAXTXT       8
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
       SettingMenu_ShowDialog
˵��:
       ������״̬����������������Ӧ�Ի�����ԴID�ĶԻ���

����:
       pMe [In]: ָ��SettingMenu Applet����ṹ��ָ��,�ýṹ����С������ض���Ϣ.
       dlgResId [in]���Ի�����ԴID���������ݸ�ID�����Ի���

����ֵ:
       ��

��ע:


==============================================================================*/
void SettingMenu_ShowDialog(CSettingMenu *pMe,uint16  dlgResId);

/*==============================================================================
����:
       SettingMenu_RouteDialogEvent
˵��:
       ������BREW�¼�·�ɸ���ǰ��Ի�����¼���������

����:
       pMe [In]: ָ��SettingMenu Applet����ṹ��ָ��,�ýṹ����С������ض���Ϣ.
       eCode [in]���¼����롣
       wParam [in]�����¼�eCode���������ݡ�
       dwParam [in]�����¼�eCode���������ݡ�

����ֵ:
       TRUE�������¼�������
       FALSE�������¼�û������

��ע:

==============================================================================*/
boolean SettingMenu_RouteDialogEvent(CSettingMenu *pMe,
    AEEEvent    eCode,
    uint16  wParam,
    uint32 dwParam
);

/*----------------------״̬����غ�������---------------------*/
/*==============================================================================
����:
       SettingMenu_ProcessState
˵��:
       SettingMenu Applet״̬��������������������pMe->m_currState������·����
       ��Ӧ�Ĵ�������

����:
       pMe [In]: ָ��SettingMenu Applet����ṹ��ָ��,�ýṹ����С������ض���Ϣ.

����ֵ:
       NFSMACTION_CONTINUE��ָʾ������״̬��״̬������ֹͣ��
       NFSMACTION_WAIT��ָʾ��Ҫ��ʾ�Ի��������û���Ӧ����״̬����

��ע:

==============================================================================*/
NextFSMAction SettingMenu_ProcessState(CSettingMenu *pMe);
#ifdef FEATRUE_SET_IP_NUMBER
void SettingMenu_Print_IP_Number(CSettingMenu *pMe);
#endif
#endif
//SETTINGMENUPRIV_H

