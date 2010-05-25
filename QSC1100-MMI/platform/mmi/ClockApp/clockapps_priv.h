#ifndef CLOCKAPPSPRIV_H
#define CLOCKAPPSPRIV_H

/*==============================================================================
// �ļ���
//        ClockApps_priv.h
//        2007-11-12 Gemsea����汾(Draft Version)
//        ��Ȩ����(c) 2007 Anylook
//
// �ļ�˵����
//
// ���ߣ�2007-11-12
// �������ڣ�Gemsea
// ��ǰ�汾��Draft
==============================================================================*/

/*==============================================================================
�޸���ʷ��¼��
ʱ       ��      �� �� ��     �޸����ݡ�λ�ü�ԭ��
-----------      ----------     -----------------------------------------------
2007-11-12            ��ʼ�汾
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
   #include "ClockApps.bid"
#else
   #include "OEMClassIds.h"  // Applet Ids
   #ifndef AEECLSID_ALARMCLOCK
      #error AEECLSID_ALARMCLOCK must be defined
   #endif
#endif

#include "clockapps.brh"
#include "clockapps_images.brh"
#include "AEEShell.h"
#include "AEEModTable.h"
#include "AEEStdLib.h"
#include "AEEModGen.h"
#include "AEEAppGen.h"
#include "OEMClassIDs.h"
#include "clockapps.h"

#include "AEEMenu.h"
#include "AEETime.h"
#include "OEMText.h"
#ifdef FEATURE_UIALARM
#include "AEEAlarm.h"
#endif

#include "AEEBacklight.h"
#include "Appscommon.h"
#include "appscommon.brh"
#ifdef FEATURE_COLOR_DISPLAY
#include "appscommon_color.brh"
#else
#include "appscommon_momo.brh"
#endif
#ifdef FEATURE_SUB_LCD
#include "AEEDisp.h"
#endif
#include "AEE_OEM.h"
#include "AEEConfig.h"
#include "OEMCFGI.h"
#include "AEEText.h"

#ifndef WIN32
#if defined( AEE_SIMULATOR)
    #define ERR
#else
    #include "AEEPhone.h"
    #include "AEEAlert.h"
    #include "ERR.h"
    #include "oemfeatures.h"
    #include "oemnvint.h"
    #include "OEMRTC.h"
#endif
#else
	#define ERR
#include "oemhelperfunctype.h"
#include "oemfeatures.h"
#endif//WIN32

#include "AEEAnnunciator.h"
#if defined( FEATURE_POWERDOWN_ALARM)
void registerPowerdownAlarmclock( void);
#endif

#define FEATURE_ON_OFF_USE_LOCAL_LANGUAGE
#define FEATURE_ONCE_ALARM
/*==============================================================================
                                 �궨��ͳ���
==============================================================================*/
#if defined( AEE_SIMULATOR)
    #define  AEE_CLOCKAPPS_RES_FILE "fs:/mod/clockapps/en/clockapps.bar"
#else
    #define  AEE_CLOCKAPPS_RES_FILE (AEE_RES_LANGDIR CLOCKAPPS_RES_FILE)
#endif

// Ϊ��ֹ�û����ٰ���������¼�EVT_APPISREADY���¼��ɶ�ʱ��
// ClockApps_APPIsReadyTimer���� 
#define APPISREADY_TIMER       400            
#define EVT_APPISREADY   (EVT_USER+1)  
 
#define OEM_IME_DIALOG   (0x7FFF)

// �û����¸��½����¼�
//#define EVT_USER_REDRAW   (EVT_USER+2)

#define IDC_CLOCK_ST_LIST   (1)
#define IDC_CLOCK_TIME      (2)
#define IDC_REP_MOD_LIST    (3)
#define IDC_CLOCK_SNOOZE    (4)

#define debug DBGPRINTF

// ״̬�ƶ��꣺�Ƚ�ǰһ״̬�õ�ǰ״̬���£��ٽ���ǰ״̬��ΪnextState
#define MOVE_TO_STATE(nextState) {            \
            ClockAppsState tpState;            \
            tpState = nextState;          \
            pMe->m_ePreState = pMe->m_eCurState; \
            pMe->m_eCurState = tpState;        \
        }

// �رնԻ����
#define CLOSE_DIALOG(DlgRet)  {    \
                                     pMe->m_eDlgRet = DlgRet; \
                                     (void) ISHELL_EndDialog(pMe->m_pShell);  \
                                 }
// ������ײ���ʾ���궨��
#define CLOCK_DRAW_BOTTOMBAR(x)                           \
{                                                   \
    BottomBar_Param_type BarParam;                  \
    MEMSET(&BarParam, 0, sizeof(BarParam));         \
    BarParam.eBBarType = x;                         \
    DrawBottomBar(pMe->m_pDisplay, &BarParam);      \
}
//#define EVT_ALARMOFF            (EVT_USER + 3)  //���Ӷ�ʱ���ֻص�
//#define EVT_FOCUS_SWITCH        (EVT_USER + 4)  //�����л�
//#define EVT_SAVE_ALERT_TIMEOUT  (EVT_USER + 5)  //������ʾ�Ի�����Ϻ�Ļص�
#define CLOCK_CFG_VERSION       0x1002
#define MAX_ALARM_MSECS         (55000)         //���û������Ӧ�ر�����
#define ANI_CLOCK_IMG_MSECS     200             //������ӵ�����
#define NUM_OF_ALARMCLOCK       6   //������Ŀ
#define MAX_ALARM_INFO          8   //��ʾ��Ϣ����ַ���
#define BOTTOM_IMG_HIGHT        14  //�˵������߶�
#define CONTROL_RECT_START_X    45  //�������ý���Ŀؼ���ʼλ��
#define AECHAR_OF_STATIC_INFO   64  //���ӽ���ľ�̬�ı��ַ���
#define LENTH_OF_WALLPAPER_NAME 128 //wallpaper�ַ�����
#define HEIGHT_OF_SUB_ICON      16  //С��ICON���ĸ߶�
#define HEIGHT_OF_FONT             14   //���ֵĸ߶�
#define CLOCK_SAVE_ALERT_MSECS  1000//������ʾ��Ϣ��ͣ��ʱ��
#define SAVE_ALERT_X            4  //��ʾ��Ϣ�ı���ʼ��x����
#define MSSEC_PER_HOUR          (60*60*1000)  //һСʱ�ڵĺ�����
#define MSSEC_PER_MIN           (60*1000)     //һ�����ڵĺ�����
/*==============================================================================
                                 ���Ͷ���
==============================================================================*/
/*----------------------�Ի������������������---------------------*/
// �Ի���ر�ʱ����ֵ�б�
typedef enum DLGRetValue
{
   // ��ʼֵ��������Ҫ�����Ի���
   DLGRET_CREATE,

   // �Ի���ر�ʱ�ɷ��ص�ͨ��ֵ
   DLGRET_OK,
   DLGRET_CANCELED,

   DLGRET_ALARMCLOCK_OPTION,

   DLGRET_ALARMCLOCK_SUB,
   DLGRET_POWERALARM_SUB,
   DLGRET_ALARMTIME_REACH,
   DLGRET_DELCONFIRM,
   DLGRET_MSGBOX_OK,
} DLGRet_Value_e_Type;

/*----------------------״̬�����������������---------------------*/
// ClockApps Applet ״̬��״̬��
typedef enum _ClockAppsState
{
   ALARMST_NONE,
   ALARMST_INIT,
   ALARMST_MAIN,
   ALARMST_OPTION,
   ALARMST_SUB,
   ALARMST_REACH,
   ALARMST_EXIT,

} ClockAppsState;

// ״̬���������ظ�״̬������������ֵ����
typedef enum NextFSMAction
{
   NFSMACTION_WAIT,
   NFSMACTION_CONTINUE
} NextFSMAction;

// ����ģ����Ϣ��ģ�����ü����Ľṹ
typedef struct ClockAppsMod
{
    DECLARE_VTBL(IModule)
    uint32   m_nRefs;
} ClockAppsMod;

//��������
typedef enum _AlarmType
{
   // ����1
   WEEK_ALARM_1 = 0,

   // ����2
   WEEK_ALARM_2,

   // ����3
   WEEK_ALARM_3,

   // ����4
   WEEK_ALARM_4,

   // ����5
   WEEK_ALARM_5,

   // ����6
   WEEK_ALARM_6,

   //����7
   //WEEK_ALARM_7,

   //����8
   //WEEK_ALARM_8
} AlarmType;


//��������
typedef enum _RepModeType
{
   // ÿ��
   WEEK_ALARM_REP1 = 3,

   // ��һ~~����
   WEEK_ALARM_REP2,

   // ��һ~~����
   WEEK_ALARM_REP3,

   // ��һ
   WEEK_ALARM_REP4,

   // �ܶ�
   WEEK_ALARM_REP5,

   // ����
   WEEK_ALARM_REP6,

   // ����
   WEEK_ALARM_REP7,

   // ����
   WEEK_ALARM_REP8,

   // ����
   WEEK_ALARM_REP9,

   // ����
   WEEK_ALARM_REP10,

   // һ��
   WEEK_ALARM_REP11,
} RepModeType;
typedef enum _SnoozeType
{
//5 min
    SNOOZE_1 = 1,

//10min
    SNOOZE_2,

//15min
    SNOOZE_3,

//�ر�
    SNOOZE_4
}SnoozeType;
//������ѡ������ݽṹ
typedef struct _ClockAppCfg
{
    boolean     bStateOn[NUM_OF_ALARMCLOCK];        //����״̬
    RepModeType RepMode[NUM_OF_ALARMCLOCK];         //�ظ���ʽ
    uint32      Snooze[NUM_OF_ALARMCLOCK];          //SNOOZE  ����ʱ��
    uint32      dwWATime[NUM_OF_ALARMCLOCK];        //����ʱ��,�����ʾ
#if 1
    AECHAR      wszInfo[NUM_OF_ALARMCLOCK][MAX_ALARM_INFO + 1]; //������ʾ��Ϣ
#endif
} ClockAppCfg;


// ClockApps Applet����ṹ�壺
typedef struct _CClockApps
{
    DECLARE_VTBL(IClockApps)
    uint32          m_nRefs;          // �������ü�����
    IModule        *m_pModule;
    IDisplay       *m_pDisplay;
    IShell         *m_pShell;
    IStatic        *m_pStatic;
    AEERect       m_rc;
    // ��ǰ��Ի���IDialog�ӿ�ָ��
    IDialog             *m_pActiveDlg;
    // ��ǰ��Ի���ID
    uint16 m_pActiveDlgID;
    // ��Ի���ر�ʱ�ķ��ؽ��
    DLGRet_Value_e_Type   m_eDlgRet;
    // �Ƿ񲻸�д�Ի��򷵻ؽ����һ�������Ҫ��д(FALSE)��
    boolean m_bNotOverwriteDlgRet;
    // Applet ǰһ״̬
    ClockAppsState m_ePreState;
    // Applet ��ǰ״̬
    ClockAppsState m_eCurState;
    // Applet�Ƿ��ڹ���״̬
    boolean m_bSuspending;
    // Applet�Ƿ�׼���ÿ��Դ������������¼����ı�����Ҫ���ڿ��ٰ������������
    boolean         m_bAppIsReady;

    uint16          m_currDlgId;        //���浱ǰ��ѡ�˵�λ��

    IConfig         *m_pConfig;         //IConfig�ӿ�ָ��

    ClockAppCfg     m_ClockCfg;         //������ѡ������ݽṹ

    AlarmType       m_eCurAlarmType;    //��������

    boolean         m_bWeekAlarmEnabled;//������Ч

    boolean         m_Snooze;           //SNOOZE�Ƿ���Ч

    IBacklight     *m_pBacklight;       //����ӿ�ָ��

    ISound         *m_pSound;

#if !defined( AEE_SIMULATOR)
    IALERT         *m_pAlert;           //IALERTָ��
#endif
    PowerDown_Alarm_Cfg   m_PowerDownAlarmCfg;

    IMenuCtl       *m_pState;           //IMenuCtlָ��,"״̬"

    ITimeCtl       *m_pTime;            //ITimeCtlָ��,"ʱ��"

    IMenuCtl       *m_pRepMode;         //IMenuCtlָ��,"�ظ�ģʽ"

    IMenuCtl       *m_pSnooze;          //IMenuCtl ָ�룬snooze

    ITextCtl       *m_pText;            //ITextCtlָ��,"��ʾ��Ϣ"

    uint16          m_nCtlID;           //�ؼ�ID

    uint16          m_nTimeCtlCount;    //0��������Сʱ��,1�����ڷ�����

    uint16          m_return;

#ifdef FEATURE_SUB_LCD
   IDisplay       *m_pDisplaySub;

   IBitmap         *m_pWallSubImage;
#endif

    uint16          m_nNumKeyCount;     /*��¼������ʱ��ʱ�İ�������.0:Сʱʮλ
                                        1:Сʱ��λ;   2:����ʮλ  3:���Ӹ�λ*/

    int32           m_dwDispTime;       //��Ļ����ʾ��ʱ��

    int32           m_dwHour;           //��¼�����ּ����õ�Сʱ

    int32           m_dwMin;            //��¼�����ּ����õķ���

    boolean         m_bAlarmOff;        //�������ֽ�����־

    boolean         m_DelAll;           //�Ƿ�ɾ��ȫ������

    //BOTTOMBAR       m_BottomBar;

    //boolean         m_AlarmReach;       //Ϊ�˽��cr933,��ע�����ѵ�����Ҫ��ʾ��

    IAnnunciator        *m_pIAnn;

#if defined( FEATURE_CARRIER_SUDAN_SUDATEL) && !defined( AEE_SIMULATOR)
    nv_language_enum_type language;
#endif
   AEERect rectLine[4];
   boolean          m_bAlarmSuspend;
} CClockApps;


/*==============================================================================
                                 ȫ������
==============================================================================*/
extern boolean bFlipClosed;    //�����Ƿ�ر�
/*==============================================================================
                                 ���أ���̬������
==============================================================================*/

/*==============================================================================
                                 ��������
==============================================================================*/

/*----------------------�Ի�����غ�������---------------------*/
/*==============================================================================
����:
       ClockApps_ShowDialog
˵��:
       ������״̬����������������Ӧ�Ի�����ԴID�ĶԻ���

����:
       pMe [In]: ָ��ClockApps Applet����ṹ��ָ��,�ýṹ����С������ض���Ϣ.
       dlgResId [in]���Ի�����ԴID���������ݸ�ID�����Ի���

����ֵ:
       ��

��ע:


==============================================================================*/
void ClockApps_ShowDialog(CClockApps *pMe,uint16  dlgResId);

/*==============================================================================
����:
       ClockApps_RouteDialogEvent
˵��:
       ������BREW�¼�·�ɸ���ǰ��Ի�����¼���������

����:
       pMe [In]: ָ��ClockApps Applet����ṹ��ָ��,�ýṹ����С������ض���Ϣ.
       eCode [in]���¼����롣
       wParam [in]�����¼�eCode���������ݡ�
       dwParam [in]�����¼�eCode���������ݡ�

����ֵ:
       TRUE�������¼�������
       FALSE�������¼�û������

��ע:

==============================================================================*/
boolean ClockApps_RouteDialogEvent(CClockApps *pMe,
    AEEEvent    eCode,
    uint16  wParam,
    uint32 dwParam
);

/*==============================================================================
����:
       ClockApps_CanAlert
˵��:
       ��������ȷ���Ƿ���Ի������������棬Ŀǰ����ͨ����������жϣ��Ժ�������
       Ҫ����ڴ���ӡ�

������
       pMe [in]��ָ�� AlarmClock Applet����ṹ��ָ�롣�ýṹ����С������ض���Ϣ��

����ֵ��
       TRUE: ������ʾ
       FALSE: ��������ʾ


��ע:


==============================================================================*/
boolean ClockApps_CanAlert(CClockApps *pMe);


/*----------------------״̬����غ�������---------------------*/
/*==============================================================================
����:
       ClockApps_ProcessState
˵��:
       ClockApps Applet״̬��������������������pMe->m_currState������·����
       ��Ӧ�Ĵ�������

����:
       pMe [In]: ָ��ClockApps Applet����ṹ��ָ��,�ýṹ����С������ض���Ϣ.

����ֵ:
       NFSMACTION_CONTINUE��ָʾ������״̬��״̬������ֹͣ��
       NFSMACTION_WAIT��ָʾ��Ҫ��ʾ�Ի��������û���Ӧ����״̬����

��ע:

==============================================================================*/
NextFSMAction ClockApps_ProcessState(CClockApps *pMe);


//��ʱ��������������Alarm�Clock Applet����SNOOZE�¼�
void CClockApps_Snooze(CClockApps *pme);

#endif
//CLOCKAPPSPRIV_H

