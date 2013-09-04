#ifndef SECURITYMENUPRIV_H
#define SECURITYMENUPRIV_H

/*==============================================================================
// �ļ���
//        SecurityMenu_priv.h
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
    #include "SecurityMenu.bid"
#else
    #include "OEMClassIds.h"  // Applet Ids
    #ifndef AEECLSID_APP_SECURITYMENU
        #error AEECLSID_APP_SECURITYMENU must be defined
    #endif
#endif

#include "SecurityMenu.brh"
#include "AEEShell.h"
#include "AEEModTable.h"
#include "AEEStdLib.h"
#include "AEEModGen.h"
#include "AEEAppGen.h"
#include "OEMClassIDs.h"
//#include "OEMMacros.h"
#include "SecurityMenu.h"


#include "AEEText.h"
//#include "AEEPhone.h"
#include "AppComFunc.h"
#include "Appscommon.h"
#ifdef FEATURE_LCD_TOUCH_ENABLE
#include "appscommonimages.brh"
#endif
//#include "OEMRUIM.h"
//#include "OEMAppFuncs.h"
#include "AEEConfig.h"
//#include "OEMTSGBridge.h"
//#include "uiutils.h"
#include "OEMNV.h"
#ifdef FEATURE_NV_RUIM
#ifndef WIN32
#include "nvruimi.h"
#endif//WIN32
#endif /*FEATURE_NV_RUIM*/
#ifndef WIN32
#include "nv.h"
#endif//WIN32
//#include "OEMAnnun.h"         // for ANNUN_ALERT
#include "AEEAnnunciator.h"
#include "OEMCFGI.h"
#include "AEERUIM.h"
#include "AEE_OEM.h"
//#include "AEECallList.h"
#include "AEECallHistory.h"


#include "OEMDisp.h"       // TBD - hack until available somewhere else
#include "OEMSound.h"      // TBD - hack until available somewhere else
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
#ifdef FEATURE_OEMOMH 
#include "AEERUIM.h"
#endif

#include "AEECallHistoryExt.h"

#include "appscommonimages.brh"




#ifdef FEATURE_LCD_TOUCH_ENABLE//wlh add for LCD touch
#define SECURITYMENU_PT_IN_RECT(a,b,rct)      (boolean)( ((a) >= (rct).x && (a) <= ((rct).x + (rct).dx)) && ((b) >= (rct).y && (b) <= ((rct).y + (rct).dy)) )
#define DWPARMA_OTHER 10000
#endif//FEATURE_LCD_TOUCH_ENABLE
/*==============================================================================
                                 ���Ͷ���
==============================================================================*/
#define  AEE_APPSSECURITYMENU_RES_FILE ( "fs:/mod/securitymenu/" AEE_RES_LANGDIR SECURITYMENU_RES_FILE)
#define SEC_ERR(format, code1, code2, code3) //ERR(format, code1, code2, code3)

// ������ײ���ʾ���궨��
#define SEC_MENU_DRAW_BOTTOMBAR(x)                           \
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
   // ��ʼֵ��������Ҫ�����Ի���
   DLGRET_CREATE,

   DLGRET_MSGBOX_OK,
   DLGRET_MSGBOX_CANCEL,
   DLGRET_MSGBOX_CONFIRM,

   // �Ի���ر�ʱ�ɷ��ص�ͨ��ֵ
   DLGRET_OK,
   DLGRET_CANCELED,
   //main menu
   DLG_ASKPIN,
   DLGRET_TOSHOWMSG,

   DLG_APPLICATIONLOCK,
   DLG_PHONEPASSWORD,
   DLG_PHONEPASSWORDINPUT,

   DLG_PINCHECK,
   DLG_ASKCALLPASSWORD,
   DLG_PINCHANGE,
   DLGRET_VALIDPINPASS,
   DLGRET_SETPINPASS,
   DLGRET_VALIDPINFAILED,
   DLGRET_SETPINFAILED,
   DLGRET_AFFIRMPASSWORD,
   DLGRET_ISCOMFIRMPASSWORD,
   DLGRET_EMGCALL,
   DLGRET_RESTRICT,
   DLGRET_RESTRICTOUTGOING,
   DLGRET_RESTRICTINCOMING,
   DLGRET_RESTORE,
   DLGRET_RESTORE_MESSAGE,
   DLGRET_CHANGECODE,
   DLGRET_DELETE,
#ifdef FEATURE_LCD_TOUCH_ENABLE
   DLGRET_TSIMPASSWORDINPUT,
#endif
   DLGRET_ARKPASSWORD
   #if defined(FEATURE_VERSION_HITZ181)||defined(FEATURE_VERSION_MTM)||defined(FEATURE_VERSION_W317A)
   ,DLGRET_KEYLOCK
   #endif
   #if defined(FEATURE_VERSION_W317A)||defined(FEATURE_VERSION_C337)||defined(FEATURE_VERSION_C316)
   ,DLGRET_MOBILE_TRACKER
   #endif
#ifdef FEATURE_VERSION_C316	
    ,DLGRET_ONEKEY_LOCK_KEYPAD
#endif
} DLGRet_Value_e_Type;

/*----------------------״̬�����������������---------------------*/
// SecurityMenu Applet ״̬��״̬��
typedef enum _SecurityMenuState
{
   SECURITYMENU_NONE,
   SECURITYMENU_INIT,
   SECURITYMENU_MAIN,
   SECURITYMENU_APPLICATIONLOCK,
   SECURITYMENU_PHONEPASSWORD,
   SECURITYMENU_CHANGECODE,
   SECURITYMENU_PHONEPASSWORDINPUT,
   SECURITYMENU_PINCHECK,
   SECURITYMENU_PINCHANGE,
   SECURITYMENU_ASKPIN,
#ifdef FEATURE_LCD_TOUCH_ENABLE
   SECURITYMENU_TSIMPASSWORDINPUT,
#endif
   SECURITYMENU_ASKPASSWORD,
   SECURITYMENU_ASKCALLPASSWORD,
   SECURITYMENU_AFFIRMPASSWORD,
   SECURITYMENU_ASKPUKPASSWORD,
   SECURITYMENU_UIMERR,
   SECURITYMENU_EMERGENCYCALL,
   SECURITYMENU_RESTRICT,
   SECURITYMENU_RESTRICTOUTGOING,
   SECURITYMENU_RESTRICTINCOMING,
   SECURITYMENU_RESTORE,
   #if defined(FEATURE_VERSION_HITZ181)||defined(FEATURE_VERSION_MTM)||defined(FEATURE_VERSION_W317A)
   SECURITYMENU_KEYLOCK,
   #endif
   #if defined(FEATURE_VERSION_W317A)||defined(FEATURE_VERSION_C337)||defined(FEATURE_VERSION_C316)
   MOBILE_TRACKER,
   #endif

#ifdef FEATURE_VERSION_C316	
    SECURITYMENU_ONEKEY_LOCK_KEYPAD,
#endif
   SECURITYMENU_EXIT

} SecurityMenuState;
typedef enum
{
    ENTERPUK_STEP0,     // Ҫ������ PUK ���һ�������� PUK ��
    ENTERPUK_STEP1,     // Ҫ������ PUK ��ڶ����������� PIN ��
    ENTERPUK_STEP2      // Ҫ������ PUK ���������ȷ���� PIN ��
} Enter_UIMSecurityCode_e_Type;

// ״̬���������ظ�״̬������������ֵ����
typedef enum NextFSMAction
{
   NFSMACTION_WAIT,
   NFSMACTION_CONTINUE
} NextFSMAction;

// ����ģ����Ϣ��ģ�����ü����Ľṹ
typedef struct SecurityMenuMod
{
    DECLARE_VTBL(IModule)
    uint32   m_nRefs;
} SecurityMenuMod;
//#ifndef PINCODE_LENGTH
//#define PINCODE_LENGTH UIM_MAX_CHV_DIGITS
//#endif
typedef enum
{
    SEC_SEL_NONE,
    SEC_SEL_PHONE_LOCK,
    SEC_SEL_PHONEBOOK_LOCK,
    SEC_SEL_RECENTCALL_LOCK,
    SEC_SEL_SMS_LOCK,
    SEC_SEL_KEY_LOCK
#ifdef FEATURE_VERSION_C316
    ,SEC_SEL_ONE_KEY_LOCK_KEYPAD
#endif
}SEC_LOCK_SEL;
// SecurityMenu Applet����ṹ�壺
typedef struct _CSecurityMenu
{
    DECLARE_VTBL(ISecurityMenu)
    uint32              m_nRefs;          // �������ü�����
    IModule             *m_pModule;
    IDisplay            *m_pDisplay;
    IShell              *m_pShell;
    IImage              *m_PromtImage;

    AEERect             m_rc;
     // message resource ID
    // ��ǰ��Ի���IDialog�ӿ�ָ��
    IDialog             *m_pActiveDlg;
    // ��ǰ��Ի���ID
    uint16              m_pActiveDlgID;
    // ��Ի���ر�ʱ�ķ��ؽ��
    DLGRet_Value_e_Type m_eDlgRet;
    // �Ƿ񲻸�д�Ի��򷵻ؽ����һ�������Ҫ��д(FALSE)��
    boolean             m_bNotOverwriteDlgRet;
    // Applet ǰһ״̬
    SecurityMenuState   m_ePreState;
    // Applet ��ǰ״̬
    SecurityMenuState   m_eCurState;
    // Applet�Ƿ��ڹ���״̬
    boolean             m_bSuspending;
    // Applet�Ƿ�׼���ÿ��Դ������������¼����ı�����Ҫ���ڿ��ٰ������������
    boolean             m_bAppIsReady;
//������Ч��־
    //��ǰ��ѡ�˵���ID
    uint16              m_currDlgId;
    uint16              m_nSubDlgId; //�μ��˵�λ��
    IStatic             *m_pIStatic;
    IConfig             *m_pConfig;  // IConfig interface
#ifdef FEATURE_LCD_TOUCH_ENABLE
    boolean             m_bup;
    int16               m_i;
    uint16              m_pActiveTSIMInputID;
#endif

    uint32              ui_esn;
    uint16              m_wMsgID;
    int                 m_nCanAttemptTimes;
    uint16              m_PassWord;
    boolean             m_IsPin;
    char                m_Pin[UIM_MAX_CHV_DIGITS+1];
    //IPhone            *m_phone; 
    SEC_LOCK_SEL        m_lock_sel;
    IRUIM               *m_pIRUIM;
    IAnnunciator         *m_pIAnn;
    
    int                     nOldPSWLength;
    int                     nNewPSWLength;
    boolean              m_bIsConfirmPassword;
    Enter_UIMSecurityCode_e_Type  m_eRUIMSCode;
    char                  m_strPUK[UIM_MAX_CHV_DIGITS + 1];
    char                  *m_strPhonePWD;
    char                  m_strPIN[UIM_MAX_CHV_DIGITS + 1]; 
	char                  *m_strPhoneNUM;
} CSecurityMenu;


/*==============================================================================
                                 �궨��ͳ���
==============================================================================*/
// Ϊ��ֹ�û����ٰ���������¼�EVT_APPISREADY���¼��ɶ�ʱ��
// SecurityMenu_APPIsReadyTimer����
#define APPISREADY_TIMER       100             
#define EVT_APPISREADY    (EVT_USER + 1)       

// �û����¸��½����¼�
//#define EVT_USER_REDRAW   (EVT_USER + 2)

//���ڶ�ʱ���ص�
//#define EVT_DIALOGTIMEOUT (EVT_USER + 3)

// ״̬�ƶ��꣺�Ƚ�ǰһ״̬�õ�ǰ״̬���£��ٽ���ǰ״̬��ΪnextState
#define MOVE_TO_STATE(nextState) {            \
            SecurityMenuState tpState;            \
            tpState = nextState;          \
            pMe->m_ePreState = pMe->m_eCurState; \
            pMe->m_eCurState = tpState;        \
        }
#define SECRUITY_MENU_SOFK_HIGHT 0
#define FIRSR_MENU_LINE 33
#define LAST_MENU_GAP   27
#if defined(FEATURE_VERSION_C306)||defined(FEATURE_VERSION_W0216A)
#define AVK_SEND_TWO     2
#elif defined(FEAUTRE_VERSION_N450)|| defined(FEATURE_VERSION_N021)|| defined(FEATURE_VERSION_C01)|| defined(FEATURE_VERSION_W516) || defined(FEATURE_VERSION_W027)
#define AVK_SEND_TWO     2
#else
#define AVK_SEND_TWO     1
#endif

#ifdef FEATURE_LCD_TOUCH_ENABLE
#define CALC_ITEM                    15
#define STARTY                       50
#define STARX                        0
#define SPACEX                       0
#define NUMWINDTH                    80
#define NUMHEIGHT                    44

#define PWTEXT_MINX                  0
#define PWTEXT_MINY                  25
#define PWTEXT_MAXX                  240
#define PWTEXT_MAXY                  150
#endif

// Max number of attempts to enter the security code.  This
// value is only used in the non R-UIM build or when CHV1
// is enabled.
//#define MAX_PIN_ATTEMPTS        3
//#define MAX_PASSWORD_NUMBER     4

////
// Number of milliseconds to display a message before automatically closing it
#define MSG_TIMEOUT_MS   (2 * 1000L)

////
// This command event (EVT_COMMAND) is sent to the applet by itself
// when the message timeout occurs (see CSecurtyMenu_MessageTimeoutCB)
#define ID_MSG_EVENT            (EVT_USER)

#define xOffset                 (5)
#ifdef FEATURE_VERSION_K212
#define SECURITY_PROMPTMSG_TIMER (500)
#else
#define SECURITY_PROMPTMSG_TIMER (1500)
#endif
/*==============================================================================
                                 ȫ������
==============================================================================*/

/*==============================================================================
                                 ���أ���̬������
==============================================================================*/

/*==============================================================================
                                 ��������
==============================================================================*/
#if 0
void PhoneLock_ShowMessageBoxThatPasswordIsInvalid(CSecurityMenu *pMe, IDisplay* pDisplay, IStatic* pStatic, AECHAR* pPrompt);
#endif

/*==============================================================================
����:
   Security_ShowMsgBox

˵��:
    ������״̬��������������ȫ���ڶԻ���

����:
    pMe [in]: ָ�� Security Applet ����ṹ��ָ�롣�ýṹ����С������ض���Ϣ��
    wTextResId [in]: ��Ϣ�ı���Դ ID��

����ֵ:
    none

��ע:

==============================================================================*/
void Security_ShowMsgBox(CSecurityMenu *pMe, uint16 wTextResId);

/*----------------------�Ի�����غ�������---------------------*/
/*==============================================================================
����:
       SecurityMenu_ShowDialog
˵��:
       ������״̬����������������Ӧ�Ի�����ԴID�ĶԻ���

����:
       pMe [In]: ָ��SecurityMenu Applet����ṹ��ָ��,�ýṹ����С������ض���Ϣ.
       dlgResId [in]���Ի�����ԴID���������ݸ�ID�����Ի���

����ֵ:
       ��

��ע:


==============================================================================*/
void SecurityMenu_ShowDialog(CSecurityMenu *pMe,uint16  dlgResId);

/*==============================================================================
����:
       SecurityMenu_RouteDialogEvent
˵��:
       ������BREW�¼�·�ɸ���ǰ��Ի�����¼���������

����:
       pMe [In]: ָ��SecurityMenu Applet����ṹ��ָ��,�ýṹ����С������ض���Ϣ.
       eCode [in]���¼����롣
       wParam [in]�����¼�eCode���������ݡ�
       dwParam [in]�����¼�eCode���������ݡ�

����ֵ:
       TRUE�������¼�������
       FALSE�������¼�û������

��ע:

==============================================================================*/
boolean SecurityMenu_RouteDialogEvent(CSecurityMenu *pMe,
                                      AEEEvent      eCode,
                                      uint16        wParam,
                                      uint32        dwParam);

/*----------------------״̬����غ�������---------------------*/
/*==============================================================================
����:
       SecurityMenu_ProcessState
˵��:
       SecurityMenu Applet״̬��������������������pMe->m_currState������·����
       ��Ӧ�Ĵ�������

����:
       pMe [In]: ָ��SecurityMenu Applet����ṹ��ָ��,�ýṹ����С������ض���Ϣ.

����ֵ:
       NFSMACTION_CONTINUE��ָʾ������״̬��״̬������ֹͣ��
       NFSMACTION_WAIT��ָʾ��Ҫ��ʾ�Ի��������û���Ӧ����״̬����

��ע:

==============================================================================*/
NextFSMAction SecurityMenu_ProcessState(CSecurityMenu *pMe);

#endif
//SECURITYMENUPRIV_H

