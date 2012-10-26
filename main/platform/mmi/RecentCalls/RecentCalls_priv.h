#ifndef RECENTCALLPRIV_H
#define RECENTCALLPRIV_H

/*==============================================================================
// �ļ���
//        recentcalls_priv.h
//        2007-11-18 ��ϲ�����汾(Draft Version)
//        ��Ȩ����(c) 2007 Anylook
//        
// �ļ�˵����
//        
// ���ߣ�2007-11-18
// �������ڣ���ϲ��
// ��ǰ�汾��Draft
==============================================================================*/

/*==============================================================================
�޸���ʷ��¼��
ʱ       ��      �� �� ��     �޸����ݡ�λ�ü�ԭ��
-----------      ----------     -----------------------------------------------
07-11-18         ��ϲ��       ��ʼ�汾
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
 Use the .bid file if compiling as dynamic applet
   #include "recentcalls.bid"
#else
   #include "OEMClassIds.h"  // Applet Ids
   #ifndef AEECLSID_APP_RECENTCALL
      #error AEECLSID_APP_RECENTCALL must be defined
   #endif
#endif

#include "recentcalls.brh"
#include "AEEShell.h"
#include "AEEModTable.h"
#include "AEEStdLib.h"
#include "AEEModGen.h"
#include "AEEAppGen.h"
#include "OEMClassIDs.h"
#include "recentcalls.h"
#include "AEEMenu.h"
#include "AeeTime.h"
#include "AeeDate.h"
//#include "AEECallList.h"
#include "AEECallHistory.h"
#include "AEECallHistoryExt.h"
#include "AEEConfig.h"
#include "OEMCFGI.h"
#include "AppComFunc.h"
#ifdef FEATURE_USES_ZI
#include "OEMZIText.h"
#else
#include "OEMText.h"
#endif
#include "OEMFeatures.h"
#if defined(FEATURE_WMS_APP) && !defined(FEATURE_WMSAPP_ONLYSUPPORTVMAIL)
#include "WMSApp.h"
#endif
#include "ContApp.h"
#include "CallApp.h"
#include "Appscommon.h"
#include "AEE.h"

#include "AEEText.h"
#include "AEEAnnunciator.h"
#ifdef FEATURE_LED_CONTROL
#include "AEEBacklight.h"
#endif



/*==============================================================================
                                 ���Ͷ���
==============================================================================*/
/*----------------------�Ի������������������---------------------*/
// �Ի���ر�ʱ����ֵ�б�
typedef enum DLGRetValue
{
   // ��ʼֵ��������Ҫ�����Ի���
   DLGRET_CREATE,

   // �Ի���IDD_SOUNDMENU_DIALOG�����ض�ֵ
   DLGRET_MISSEDCALL,
   DLGRET_INCOMCALL,
   DLGRET_OUTGCALL,
   DLGRET_TIMEMENU,
   DLGRET_DELMENU,
   
   // �Ի���IDD_RLISTRECORD�����ض�ֵ
   DLGRET_RECORDDEAL,

   // �Ի���IDD_RTIME_MENU�����ض�ֵ
   DLGRET_TIME,

   // �Ի���IDD_RTIME_MENU�����ض�ֵ
   DLGRET_WARN,
   
   // �Ի���IDD_RDEAL_RECORD�����ض�ֵ
   DLGRET_DETAIL,
   DLGRET_DELONE,

   //�Ի���IDD_RTIME�����ض�ֵ
   DLGRET_TIMECLEAR,
   
   // �Ի���ر�ʱ�ɷ��ص�ͨ��ֵ
   DLGRET_OK,
   DLGRET_CANCELED,
   
   // IDD_VERIFY_PASSWORD, 
   DLGRET_VERIFY_PASSWORD_PASSED,

   DLGRET_INPUT_INVALID,

   DLGRET_MSGBOX_OK,
   DLGRET_MAX_WMS_CONTACTS,

   DLGRET_REFRESH,
   DLGRET_VALIDPINPASS,
   DLGRET_VALIDPINFAILED
   
} DLGRetValue;

/*----------------------״̬�����������������---------------------*/
// recentcalls Applet ״̬��״̬��
typedef enum _recentcallsState
{
    STATE_VERIFY_PASSWORD,
    STATE_RMAINMENU,
    STATE_RECORD_LIST,
    STATE_RTIME_MENU,
    STATE_RDEL_MENU,
    STATE_RECORDDEAL,
    STATE_RWARN,
    STATE_RTIME,
    STATE_DETAIL,
    STATE_REXIT,
    STATE_SELECT_RETURN,
    STATE_ASKPASSWORD
} recentcallsState;

// ״̬���������ظ�״̬������������ֵ����
typedef enum NextFSMAction
{
   NFSMACTION_WAIT,
   NFSMACTION_CONTINUE
} NextFSMAction;

// ����ģ����Ϣ��ģ�����ü����Ľṹ
typedef struct recentcallsMod
{
    DECLARE_VTBL(IModule)
    uint32   m_nRefs;
} recentcallsMod;
/*
typedef struct _Record{
    AECHAR *number;
    AECHAR *name;
    boolean is_number;
    uint16 category;
    uint32 time_stamp;                 // years to seconds
    //dword time_stamp;                 // years to seconds
    uint32 duration;
}Record;
*/
// recentcalls Applet����ṹ�壺
typedef struct _CRecentCalls
{
    DECLARE_VTBL(IRecentCalls)
    uint32          m_nRefs; // �������ü�����
    IModule        *m_pModule;
    IDisplay       *m_pDisplay;
    IShell         *m_pShell;
    IAnnunciator *m_pIAnn;
#ifdef FEATURE_LED_CONTROL
    IBacklight     *m_pBacklight;
#endif    
    AEERect         m_rc;
    // ��ǰ��Ի���IDialog�ӿ�ָ��
    IDialog        *m_pDialog;
    // ��ǰ��Ի���ID
    uint16          m_pActiveDlgID;
    // ��Ի���ر�ʱ�ķ��ؽ��
    DLGRetValue     m_eDlgRet;
    // �Ƿ񲻸�д�Ի��򷵻ؽ����һ�������Ҫ��д(FALSE)��
    boolean         m_bNotOverwriteDlgRet;
    // Applet ǰһ״̬
    recentcallsState m_ePreState;
    // Applet ��ǰ״̬
    recentcallsState m_eCurState;
    // Applet start type
    AppletStartMethod   m_eStartMethod;
    // Applet�Ƿ��ڹ���״̬
    boolean         m_bSuspending;
    // Applet�Ƿ�׼���ÿ��Դ������������¼����ı�����Ҫ���ڿ��ٰ������������
    boolean         m_bAppIsReady;
    
    IMenuCtl       *pMenu;  //�˵�����ָ��
    IImage         *Image;  //ͼƬָ��
    IConfig        *m_pConfig;//nv����ָ��
    Record          list_record[AEECALLHISTORY_MAX_CALLS];
    uint16           m_callsCategory;
    uint16          m_selectedItem[2]; //�˵��ּ���¼��ǰѡ�����    
    uint16          record_selected; //��¼��ǰѡ�����ĸ�record��¼
    uint16          selectState;
    uint16          record_count; //ͨ����¼�ĸ���
     // message resource ID
    uint16              m_wMsgResID;
    boolean         key_enable; //�Ƿ��ֹ�����¼�
    //ICallList         *m_pCallList;
    ICallHistory    *m_pCallHistory;
    PromptMsg_Param_type m_PromptMsg;
    
    ITextCtl*       m_pPhoneLockPasswordTextCtl;
    char            m_charAppStartArgs;
    IStatic*        m_pStatic;
    char*         m_pPhoneLockPassword;
    PFNSELECTEDRECENTCALLSCB m_pSelFldCB;
    uint16           m_nRemainWMSNum;
	uint16           m_currDlgId;
} CRecentCalls;

/*==============================================================================
                                 �궨��ͳ���
==============================================================================*/
// ���� BREW 3 ����Ҫ���ض�����Դ�ļ���
#define  AEE_RECENTCALLSRES_LANGFILE (AEE_RES_LANGDIR RECENTCALLS_RES_FILE)
#ifdef FEATURE_LCD_TOUCH_ENABLE//wlh add for LCD touch
#define RECENTCALLS_PT_IN_RECT(a,b,rct)      (boolean)( ((a) >= (rct).x && (a) <= ((rct).x + (rct).dx)) && ((b) >= (rct).y && (b) <= ((rct).y + (rct).dy)) )
#endif

// Ϊ��ֹ�û����ٰ���������¼�EVT_APPISREADY���¼��ɶ�ʱ��
// recentcalls_APPIsReadyTimer����
#if defined(FEATURE_VERSION_C306)||defined(FEATURE_VERSION_W0216A)
#define AVK_SEND_TWO     2
#elif defined(FEAUTRE_VERSION_N450)|| defined(FEATURE_VERSION_N021)|| defined(FEATURE_VERSION_C01)|| defined(FEATURE_VERSION_W516)|| defined(FEATURE_VERSION_W027)
#define AVK_SEND_TWO     2
#else
#define AVK_SEND_TWO     1
#endif

#define APPISREADY_TIMER       200
#define EVT_APPISREADY   (EVT_USER+1)         

#define EVT_USER_CLOSEAPP (EVT_USER+13)
#define xOffset                 (5)

#define RECENTCALLS_DRAW_BOTTOMBAR(x)                           \
{                                                   \
    BottomBar_Param_type BarParam;                  \
    MEMSET(&BarParam, 0, sizeof(BarParam));         \
    BarParam.eBBarType = x;                         \
    DrawBottomBar(pMe->m_pDisplay, &BarParam);      \
}

// ״̬�ƶ��꣺�Ƚ�ǰһ״̬�õ�ǰ״̬���£��ٽ���ǰ״̬��ΪnextState
#define MOVE_TO_STATE(nextState) {            \
            recentcallsState tpState;            \
            tpState = nextState;          \
            pMe->m_ePreState = pMe->m_eCurState; \
            pMe->m_eCurState = tpState;        \
        }

#define STARTARG_OUTGCALL    ((char) 'A')  //��SEND����������б�ʱ��ı�־�ַ�
#define STARTARG_MISSEDCALL  ((char) 'B')  //�������������б�ʱ��ı�־�ַ�
#define STARTARG_ALLCALL     ((char) 'C')  //���е�ͨ����¼�������ŵ�ʱ�����Ҫѡ�����еĺ���
#define STARTARG_ONECALL     ((char) 'D') //��ȡһ��ͨ����¼
#define MAX_STRING_LEN       (38)
#define BETWEEN_LINE_PIXELS (3)
#define PIXELS_TO_EDGE          (6)

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
       recentcalls_ShowMsgBox
˵��:
       ������״̬����������������Ӧ�Ի�����ԴID�ĶԻ���

����:
       pMe [In]: ָ��recentcalls Applet����ṹ��ָ��,�ýṹ����С������ض���Ϣ.
       wTextResId [in]���Ի�����ԴID���������ݸ�ID�����Ի���

����ֵ:
       ��

��ע:
       

==============================================================================*/

void recentcalls_ShowMsgBox(CRecentCalls *pMe, uint16 wTextResId);
/*==============================================================================
����:
       recentcalls_ShowDialog
˵��:
       ������״̬����������������Ӧ�Ի�����ԴID�ĶԻ���

����:
       pMe [In]: ָ��recentcalls Applet����ṹ��ָ��,�ýṹ����С������ض���Ϣ.
       dlgResId [in]���Ի�����ԴID���������ݸ�ID�����Ի���

����ֵ:
       ��

��ע:
       

==============================================================================*/
void recentcalls_ShowDialog(CRecentCalls *pMe,uint16  dlgResId);

/*==============================================================================
����:
       recentcalls_RouteDialogEvent
˵��:
       ������BREW�¼�·�ɸ���ǰ��Ի�����¼���������

����:
       pMe [In]: ָ��recentcalls Applet����ṹ��ָ��,�ýṹ����С������ض���Ϣ.
       eCode [in]���¼����롣
       wParam [in]�����¼�eCode���������ݡ�
       dwParam [in]�����¼�eCode���������ݡ�

����ֵ:
       TRUE�������¼�������
       FALSE�������¼�û������

��ע:

==============================================================================*/
boolean recentcalls_RouteDialogEvent(CRecentCalls *pMe,
    AEEEvent    eCode,
    uint16  wParam,
    uint32 dwParam
);

/*----------------------״̬����غ�������---------------------*/
/*==============================================================================
����:
       recentcalls_ProcessState
˵��:
       recentcalls Applet״̬��������������������pMe->m_currState������·����
       ��Ӧ�Ĵ�������

����:
       pMe [In]: ָ��recentcalls Applet����ṹ��ָ��,�ýṹ����С������ض���Ϣ.

����ֵ:
       NFSMACTION_CONTINUE��ָʾ������״̬��״̬������ֹͣ��
       NFSMACTION_WAIT��ָʾ��Ҫ��ʾ�Ի��������û���Ӧ����״̬����

��ע:

==============================================================================*/
NextFSMAction recentcalls_ProcessState(CRecentCalls *pMe);

void RecentCalls_FreeSelectRecordListNode(CRecentCalls *pMe);

// ��ȡ���ŷ����б�ʣ���������
uint16 RecentCalls_GetRemainWMSNum(CRecentCalls *pMe);

#endif
//RECENTCALLPRIV_H
#ifdef FEATURE_THAILAND  
int Recntcall_is_view_state(int bb) ;
#endif

