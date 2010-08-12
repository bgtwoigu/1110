#ifndef SCHEDULEAPP_PRIV_H
#define SCHEDULEAPP_PRIV_H

/** ----------------------------------------------------------------------------
 *
 * History:
 *
 * when         who            what and why
 * -----------  -----------    -----------------------------
 * -----------------------------------------------------------------------------
 */


#ifndef AEE_SIMULATOR
#include "OEMFeatures.h"   //Handset Feature Definitions
#else
#include "BREWSimFeatures.h"
                  //Simulator Feature Definitions
#endif

#include "AEEShell.h"
#include "AEEModTable.h"
#include "AEEStdLib.h"
#include "AEEModGen.h"
#include "AEEAppGen.h"
#include "AEE_OEM.h"

#include "OEMClassIDs.h"
#include "AEE.h"
#include "AEEAlarm.h"
#include "AEEAlert.h"
#include "AppComFunc.h"
#include "Appscommon.h"
//
#include "ScheduleApp.h"
#include "scheduleapp.brh"
#include "AEEDate.h"
#include "AEEMenu.h"
#include "AEETAPI.h "
#include "CalendarApp.brh"
//
#include "AEEDB.h"
#include "AEEText.h"
#include "AEETime.h"
#include "AEEFILE.h"
#include "AEEBacklight.h"
#include "AEEConfig.h"
#include "OEMCFGI.h"
#include "oemnvint.h"
#include "OEMCriticalSection.h"
#include "AEEAnnunciator.h"

#ifdef FEATURE_LCD_TOUCH_ENABLE//wlh add for LCD touch
#define NEVT_KEY_PRESSS 10000
#endif


#define debug

#ifdef FEATURE_LANG_CHINESE
#define     FEATURE_DRAW_LUNAR_CALENDAR     1
#else 
#define     FEATURE_DRAW_LUNAR_CALENDAR     0
#endif

#define     FEATURE_ONE_DB                  0
#define     FEATURE_TEST_HOOK               0
#define     HEIGHT_PROMPT_BAR   BOTTOMBAR_HEIGHT

#ifdef FEATURE_DRAW_LUNAR_CALENDAR
#define     LUNAR_RECT_HEIGHT              14  //added by chengxiao 2009.03.04
#endif

#define     MAX_INPUT_SUBJECT               20
#define     MAX_INPUT_LOCATION              128
#define     MAX_SPORTS_DISTANCE             5
#define     MAX_SPORTS_TIME                 3



#if defined( AEE_SIMULATOR)
	#define  AEE_SCHEDULEAPP_RES_FILE ("fs:/mod/scheduleapp/en/" SCHEDULEAPP_RES_FILE)
	#define  AEE_CALENDARAPP_RES_FILE ("fs:/mod/scheduleapp/en/" CALENDARAPP_RES_FILE)
#else
	#define  AEE_SCHEDULEAPP_RES_FILE (AEE_RES_LANGDIR SCHEDULEAPP_RES_FILE)
	#define  AEE_CALENDARAPP_RES_FILE (AEE_RES_LANGDIR CALENDARAPP_RES_FILE)
#endif

#define num( wch) ((wch) - ((AECHAR)'0'))

#define OEM_IME_DIALOG   (0x7FFF)

#define STARTARG_SPORT    ((char) 'A')  //�� sports ���� scheduleapp


// ״̬�ƶ��꣺�Ƚ�ǰһ״̬�õ�ǰ״̬���£��ٽ���ǰ״̬��ΪnextState
#define MOVE_TO_STATE(nextState)                 \
        {                                        \
            FSMState tpState;                    \
            tpState = nextState;                 \
            pme->m_ePreState = pme->m_eCurState; \
            pme->m_eCurState = tpState;          \
        }

//�¼�����
#define EVT_APPISREADY              (EVT_USER+5)       
#define EVT_LOAD_DATA               (EVT_USER + 6)
 
#define APPISREADY_TIMER            200         
#define FILECFG                     1       //�ڸó����������ֱ�������ķ���
                                            //һ����BREW�ṩ��CONFIG ����д��
                                            //һ���ļ�����⿪��Ϊ1ʱΪ����
//lunar calendar Macros
//�����ĺ궨��
#define START_YEAR                  1900
#define START_YEAR_M                1899
#define START_MONTH                 1
#define START_DAY                   1

#define YEAR_VIEW                   0
#define MONTH_VIEW                  1

#define MAX_LEN_MSGBOX              (128)
#define MAX_LEN_TITLE               (32)
#define TIME_ONE_SEC                (1000)
#define TIMEOUT_MS_MSGBOX           (2*TIME_ONE_SEC) //��ʾ��Ϣ����ʾ��ʱ��
#define MAXHISTORY                  20          //���ļ�¼��
#define MAXTEXT                     64          //ÿ����¼���������
#define TIMEOUT_MS_QUICK            100         //�����������ٰ�����ʱ����

#define CAL_DATABASE_PATH           "fs:/mod/scheduleapp"     //���ݿ��·����
#define CAL_DATABASE_NAME           "calendar.db"     //���ݿ���ļ���
#define FILE_SCHEDULE_CFG           "schedulecfg"  //�ճ̱�������ļ���
#define SCHEDULER_MIN_SIZE          1000           //
#define SCHEDULER_MIN_RECS          20             //�����ճ̱���֧�ֵ�����¼

#define CAL_CFG_VERSION             0x1001         //

#define NUMFLDS                     (FLD_LAST - FLD_NAME_BASE)//ÿ����¼�ж��ٸ��ֶ�
#define FLD_NAME_BASE               0x80
#define EMPTY_ALARM_ID              0xFFFF
#define MINUTES_PER_HOUR            (60)



#define MAKECOMPTIME(jdate,time)    (((jdate)<<16)|(time))
#define GETDATE(comptime)           (((comptime)>>16) & 0x0000FFFF)
#define GETTIME(comptime)           ((comptime) & 0x0000FFFF)
#define GET_HOUR(t)                 ((t)/MINUTES_PER_HOUR)
#define GET_MINUTE(t)               ((t)%MINUTES_PER_HOUR)

#define sendEvent( e)       ISHELL_SendEvent(pme->m_pShell, AEECLSID_SCHEDULEAPP, e, 0, 0)
#define postEvent( e)       ISHELL_PostEvent(pme->m_pShell, AEECLSID_SCHEDULEAPP, e, 0, 0)
#define loadData()          postEvent( EVT_LOAD_DATA)

#define drawBottomBar( bottomBarType)               \
{                                                   \
    BottomBar_Param_type BarParam;                  \
    MEMSET(&BarParam, 0, sizeof(BarParam));         \
    BarParam.eBBarType = bottomBarType;             \
    DrawBottomBar(pme->m_pDisplay, &BarParam);      \
}



#if FEATURE_DRAW_LUNAR_CALENDAR
typedef struct CCalendarApp {
   IShell      *c_pIShell;
   uint16      m_yYear;       //��ŵ�ǰ������, ��, ��
   uint16      m_yMonth;
   uint16      m_yDay;

   int         adjustDay;     //��Ű������ı����������

   AECHAR      mGz_String[121];              //���      60*2 +1
   AECHAR      mSx_String[25];               //����      12*2 +1
   AECHAR      mLunMonth_String[25];         //ũ����    12*2 +1
   AECHAR      mLunLeapMonth_String[37];     //ũ������  12*3 +1
   AECHAR      mLunDay_String[61];           //ũ����    30*2 +1
   AECHAR      *m_pResGz, *m_pResSx, *m_pResLunM, *m_pResLunD;

   uint32      gLunarMonthDay[152];   //����gLunarDay��������1900�굽2051��ÿ���е���������Ϣ��
   uint8       gLunarMonth[77];       //����gLanarMonth�������1899�굽2052�����µ��·�
   uint32      monthday[12];          //����monthday���0-11����������

   //boolean     m_OverFlag;     //���Խ���־
   boolean     m_LeapMonth;      //���±�־

}CCalendarApp;
#endif


/*----------------------�Ի������������������---------------------*/
// �Ի���ر�ʱ����ֵ�б�
typedef enum _DLGRetValue
{
    // ��ʼֵ��������Ҫ�����Ի���
    DLGRET_CREATE,

    // �Ի���ر�ʱ�ɷ��ص�ͨ��ֵ
    DLGRET_OK,
    DLGRET_CANCELED,

    // for option
    DLGRET_OPTION_NEW_EVENT,
    DLGRET_OPTION_VIEWDAY_DELETE,
    DLGRET_OPTION_VIEWMONTH_VIEW,
    DLGRET_OPTION_DELETE_TODAY,
    DLGRET_OPTION_DELETE_MONTH,
    DLGRET_OPTION_DELETE_ALL,
    DLGRET_OPTION_SETUP,
    DLGRET_OPTION_BACK_TO_VIEWMONTH,
    DLGRET_OPTION_BACK_TO_VIEWDAY,
    DLGRET_OPTION_VIEWDAY_TO_VIEWEVENT,

    // for event edit
    DLGRET_EVENT_EDIT_SUBJECT,
    DLGRET_EVENT_EDIT_NOTE,


    DLGRET_VIEWDAY_EDIT_EVENT,

    DLGRET_TO_SHOWALERT,
    DLGRET_SHOWALERT_TO_VIEWEVENT,

    DLGRET_PASS,
    DLGRET_FAILD,
    DLGRET_MSGBOX_CANCEL,
    DLGRET_MSGBOX_OK,
    DLGRET_GOTODATE
    

} DLGRetValue;

/*----------------------״̬�����������������---------------------*/
// ScheduleApp Applet ״̬��״̬��
typedef enum _FSMState
{
    STATE_NONE,
    STATE_INIT,
    STATE_PWD,
    STATE_VIEWMONTH,
    STATE_OPTION,
    STATE_GOTODATE,
    STATE_VIEWDAY,
    STATE_EVENT_EDIT,
    STATE_INPUTTEXT,
    STATE_SETUP,
    STATE_SHOWALERT,
    STATE_VIEWEVENT,
    STATE_EXIT
} FSMState;

typedef enum
{
    SUBSTATE_NONE,

    SUBSTATE_VIEWDAY_DELETE_CONFIRM,

    SUBSTATE_DELETE_ALL_CONFIRM,
    SUBSTATE_DELETE_TODAY_CONFIRM,
    SUBSTATE_DELETE_MONTH_CONFIRM,

    SUBSTATE_OPTION_VIEWMONTH,
    SUBSTATE_OPTION_VIEWDAY,

    SUBSTATE_EVENT_EDIT_NEW,
    SUBSTATE_EVENT_EDIT_EDIT
} FsmSubState;

typedef enum
{
    DATE_SEL_YEAR,
    DATE_SEL_MONTH,
    DATE_SEL_DAY
} Sel_e_type;


// ״̬���������ظ�״̬������������ֵ����
typedef enum _NextFSMAction
{
	NFSMACTION_WAIT,
	NFSMACTION_CONTINUE
} NextFSMAction;


//���ָ��ƺ�ת�����
typedef enum _Selected
{
	init,
	copyto,
	moveto
}Select;

//��ʾ��ʽ
typedef enum _AlertStates
{
	INIT,
	SILENCE,
	ALERTSET,
	VIBRATE,
	TIME_SMS
}AlertStates;

typedef enum _WalkMode
{
    WALK,//RUN 
    RUN //WALK
}WalkMode;

//���ݿ��ֶ�
typedef enum {
  FLD_ALARM = FLD_NAME_BASE,  // AEEDB_FT_BINARY
  FLD_DESC,                   // AEEDB_FT_STRING
  FLD_NOTE,                   // AEEDB_FT_STRING
  FLD_ID,                     // AEEDB_FT_WORD
  FLD_LAST
} RecordFieldTypeEnum;


//�ճ̱�����������Ϣ
typedef struct _CalCfg
{
   uint16       nAutoDelete;   //�Զ�ɾ��
   uint16       wDayStart;     //��ʼʱ��
   uint16       nSnooze;

#if !FEATURE_ONE_DB
   uint16       maxPermId;
#endif
} CalCfg;


//typedef boolean   (*PFNKEYHANDLER)  (CCalApp * pme, int nKeyCode);

//
//   For compactness and ease of sorting, the CalEvent struct stores
//   the date/time in a single long variable which is a composite
//   of the julian date and the starting time in minutes.
//
//      composite_time = (julian_date << 16) | start_time_in_minutes;
//

typedef struct _CalBaseFields
{
   uint32   dwTime;     // �¼������ں�ʱ��
   uint16   wDuration;  // Minutes ���¼����õĵ�ʱ��
   uint16   wAlarmTime; // Minutes ����Ҫ��ǰ���ٷ�����
   AlertStates  alertst;
   WalkMode walk;
   int      m_CurEvtType;//��ǰ������¼������ͣ�1 ��ʾ �ճ̱��¼�   2 ��ʾ sports �¼�
} CalBaseFields;


typedef struct CalEvent    CalEvent;
typedef struct _CCalApp    CCalApp;

struct CalEvent
{
   CalEvent    *  pNext;
   CalEvent    *  pPrev;
   uint16         nCurrRecID;  // current database Id for this record
                               // (only valid so long as the database is
                               //  open, may use IDATABASE_GetByRecordID()
                               //  with this value)
   CalBaseFields  m_b;
   uint32         m_lAlarmDateTime; //�������ѵ����ں�ʱ��
   uint16         nPermRecID;  // Perminant record Id stored with the record
                               // in the database.  Not necessarily the same
                               // as nCurrRecID.
};

typedef struct _CalDBRecFields
{
   uint16         nCurrRecID;  // current database Id for this record
                               // (only valid so long as the database is
                               // open, may use IDATABASE_GetByRecordID()
                               // with this value)
   CalBaseFields  b;
   AECHAR *       pszDesc;     // ָ��ǰ�¼��ı���
   int            nDescLen;    // �����ַ��ĳ���
   AECHAR *       pszNote;     //
   int            nNoteLen;
   uint16         nPermRecID;  // Perminant record Id stored with the record
                               // in the database.  Not necessarily the same
                               // as nCurrRecID.
} CalDBRecFields;

typedef enum
{
    NO_ALARM,
    NOT_EXPIRED,
    EXPIRED,
    DONE
} AppointmentStatus;

typedef struct
{
   uint16               dbId;
   uint16               permId;
   uint16               CurEveType;
   CalBaseFields        baseFields;
   AECHAR               subject[MAX_INPUT_SUBJECT];//distance
   AECHAR               location[MAX_INPUT_LOCATION];//time
   AppointmentStatus    status;
   JulianType           julian;
} Appointment;


//�����ݽṹ��Ҫ�������ݿ�Ķ�д����صĲ���
struct _CCalApp
{
   IShell           *m_pIShell;
   IDisplay         *m_pIDisplay;

   CalCfg            m_cfg;
   IDBMgr           *m_pIDBMgr;
   IDatabase        *m_pIDatabase;
#ifdef FEATURE_UIALARM
   IAlarm      *m_pIAlarm;
#endif


#if defined(FILECFG)
  IFileMgr          *m_pFileMgr;            //�����ļ��ӿ�
#endif

  IALERT            *m_alert;               //���������Ľӿ�

   uint32            m_lCurrentDay;         //��ǰ������
   uint32            m_lToday;

   AECHAR            m_szEventDes[MAX_INPUT_SUBJECT]; //Լ��ı���
   AECHAR            m_szNote[MAX_INPUT_LOCATION];     //Լ�������
   uint32            m_lEventDay;           //��ǰ����
   int32             m_lStartTime;          //Լ�Ὺʼʱ��
   int32             m_lDuration;           //Լ���ʱ��
   uint16            m_nAlarmTime;          //Լ������ʱ��

   CalEvent*         m_pceCurrent;          //���ٵ�ǰ��һ���¼�
   int               m_dbOperationResult;          //�������ݲ����ķ���ֵ
   uint32            m_dwMask;              //��¼ĳ���µ���Լ�������
   uint16            m_wPendingAlarmPermID; //�����¼�������ID
   uint16            m_nNextPermRecID;      //�������ݿ��¼ʱ�ļ�¼��ID
   AlertStates        m_AlertState;          //���浱ǰ�¼�����ķ�ʽ


   CalEvent*            m_pMonthEvents[31];
   JulianType           m_julianCurrentDay;
   uint32               m_dwTotalEventsNumber;
   uint32               m_dwThisMonthEventsNumber;
   uint16               m_viewDaySelectedEventID;
   Appointment**        m_expiredAppointment;
   Appointment*         m_appointmentToView;
   uint16               m_CurEvtType;    //1��ʾ �ճ̱��¼�   2��ʾ sports plan
   WalkMode             m_CurMode;
};

// ScheduleApp Applet����ṹ�壺
typedef struct _CScheduleApp
{
    DECLARE_VTBL(IScheduleApp)
    uint32              m_nRefs;          // �������ü�����
    IModule            *m_pModule;
    IDisplay           *m_pDisplay;
    IShell             *m_pShell;
    AEERect             m_rc;   

    CCalApp             m_CalMgr;

    IDialog            *m_pActiveDlg;
    uint16              m_pActiveDlgID;
    DLGRetValue         m_eDlgRet;
    boolean             m_bNotOverwriteDlgRet;
    FSMState            m_ePreState;
    FSMState            m_eCurState;
    boolean             m_bInactive;
    boolean             m_bSuspended;
    boolean             m_bAppIsReady;

    boolean             m_bStartAppletCommandQueued;
    AECHAR              m_pVDTitle[30];
    uint16              ALERTID;
    uint16              m_wMsgResID;
    uint16              m_nsel;
    ISound              *m_pSound;
    AECHAR              wstrYear[5];
    AECHAR              wstrMonth[3];
    AECHAR              wstrDay[3];
    byte                curSel;
    boolean             update;
    char                m_AppStartArgs;
    boolean             m_sports;// TRUE��ʾ�� sports ģ����룬  FALSE��ʾ��������


    AECHAR*             m_pszEventEditText;
    uint16              m_eventEditItem;
    boolean             m_closeByPowerKey;

    FSMState            m_stateToBackOption;
    FSMState            m_stateToBackEventEdit;
    FSMState            m_stateToBackSetup;
    FSMState            m_stateToBackShowAlert;
    FSMState            m_stateToBackViewEvent;

    FsmSubState         m_subStateOption;
    FsmSubState         m_subStateViewMonth;
    FsmSubState         m_subStateViewDay;
    FsmSubState         m_subStateEventEdit;


#if FEATURE_DRAW_LUNAR_CALENDAR
    //ũ�����ַ���
    CCalendarApp     m_calendar;
    AECHAR              m_LunarString[40];
#endif
    char               *m_strPhonePWD;
    boolean             m_bshowEventDetail; //modify for snooze and see detail when alerting
    boolean             m_bRepaint;  //modify for phone die in display loading menu when incoming call
    boolean             m_bDeferred; //for phone die in display loading menu when incoming call
    
    IAnnunciator *m_pIAnn;
} CScheduleApp;

/*=============================================================================
                               ȫ�ֱ�����������
===============================================================================*/
/*----------------------�Ի�����غ�������---------------------*/
/*==============================================================================
����:
	   CScheduleApp_ShowDialog
˵��:
	   ������״̬����������������Ӧ�Ի�����ԴID�ĶԻ���

����:
	   pme [In]: ָ��ScheduleApp Applet����ṹ��ָ��,�ýṹ����С������ض���Ϣ.
	   dlgResId [in]���Ի�����ԴID���������ݸ�ID�����Ի���

����ֵ:
	   SUCCESS: �����Ի���ɹ�
	   ��SUCCESS: �����Ի���ʧ��

��ע:


==============================================================================*/
int CScheduleApp_ShowDialog(CScheduleApp *pme, uint16  dlgResId);

/*==============================================================================
����:
	   CScheduleApp_RouteDialogEvent
˵��:
	   ������BREW�¼�·�ɸ���ǰ��Ի�����¼���������

����:
	   pme [In]: ָ��ScheduleApp Applet����ṹ��ָ��,�ýṹ����С������ض���Ϣ.
	   eCode [in]���¼����롣
	   wParam [in]�����¼�eCode���������ݡ�
	   dwParam [in]�����¼�eCode���������ݡ�

����ֵ:
	   TRUE�������¼�������
	   FALSE�������¼�û������

��ע:

==============================================================================*/
boolean CScheduleApp_RouteDialogEvent( CScheduleApp  *pme,
                                       AEEEvent   eCode,
                                       uint16     wParam,
                                       uint32     dwParam);


/*----------------------״̬����غ�������---------------------*/
/*==============================================================================
����:
	   CScheduleApp_ProcessState
˵��:
	   ScheduleApp Applet״̬��������������������pMe->m_currState������·����
	   ��Ӧ�Ĵ�������

����:
	   pme [In]: ָ��ScheduleApp Applet����ṹ��ָ��,�ýṹ����С������ض���Ϣ.

����ֵ:
	   NFSMACTION_CONTINUE��ָʾ������״̬��״̬������ֹͣ��
	   NFSMACTION_WAIT��ָʾ��Ҫ��ʾ�Ի��������û���Ӧ����״̬����

��ע:

==============================================================================*/
NextFSMAction CScheduleApp_ProcessState(CScheduleApp *pme);


/*==============================================================================
����:
	   CalMgr_InitAppData
˵��:
	   ��ʼCCalApp�ṹ������

����:
	   pme [In]: ָ��CalMgr ����ṹ��ָ��,�ýṹ����С������ض���Ϣ.

����ֵ:
	   TRUE�������¼�������
	   FALSE�������¼�û������

��ע:

==============================================================================*/

boolean CalMgr_InitAppData(CCalApp *pme, IShell* pIShell, IDisplay *pIDisplay);
/*==============================================================================
����:
	   CalMgr_FreeAppData
˵��:
	   �ͷ�CalMgr�ṹ������

����:
	   pme [In]: ָ��CCalApp����ṹ��ָ��,�ýṹ����С������ض���Ϣ.


����ֵ:
	   ��

��ע:

==============================================================================*/
void CalMgr_FreeAppData(CCalApp *pme);

/*==============================================================================
����:
	   Cal_HandleAlarm
˵��:
	   ���� EVT_ALARM �¼�����

����:
	   pme [In]: ָ��CScheduleApp����ṹ��ָ��,�ýṹ����С������ض���Ϣ.
	   wPermID �� ALARM��ID
����ֵ:

��ע:

==============================================================================*/

void Cal_HandleAlarm(CScheduleApp  *pme, uint16 wPermID);

/*==============================================================================
����:
	   CScheduleApp_ShowMsgDialog
˵��:
	   ������BREW�¼�·�ɸ���ǰ��Ի�����¼���������

����:
	   pme [In]: ָ��ScheduleApp Applet����ṹ��ָ��,�ýṹ����С������ض���Ϣ.
	   msgResId [in]��message id
	   msg [in]����Ϣָ��
	   msglen [in]����Ϣ���ȡ�

����ֵ:
	   TRUE�������¼�������
	   FALSE�������¼�û������

��ע:

==============================================================================*/
void  CScheduleApp_ShowMsgDialog(CScheduleApp   *pme,uint16 msgResId);

extern void OEMOS_Sleep(uint32 nMSecs);

#if FEATURE_DRAW_LUNAR_CALENDAR
boolean Calendar_InitAppData(CCalendarApp * pme, IShell* pIShell);
void Calendar_FreeAppData(CCalendarApp * pme);
#endif


void closeDatabaseIf( CCalApp *pme);
void deleteAllEventsFromDB( CCalApp *pme);

void repaint(CScheduleApp *pme, boolean deferred); 

#endif//SCHEDULEAPP_PRIV_H
