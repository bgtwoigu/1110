#ifndef SOUNDMENUPRIV_H
#define SOUNDMENUPRIV_H

/*==============================================================================
// �ļ���
//        SoundMenu_priv.h
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
   #include "SoundMenu.bid"
#else
   #include "OEMClassIds.h"  // Applet Ids
   #ifndef AEECLSID_APP_SOUNDMENU
      #error AEECLSID_APP_SOUNDMENU must be defined
   #endif
#endif

#include "SoundMenu.brh"
#include "appscommon.brh"
#include "AEEShell.h"
#include "AEEModTable.h"
#include "AEEStdLib.h"
#include "AEEModGen.h"
#include "AEEAppGen.h"
#include "OEMClassIDs.h"
//#include "OEMMacros.h"
#include "SoundMenu.h"

#include "AEEMenu.h"
#include "AEEConfig.h"
#include "AppComFunc.h"
#include "Appscommon.h"
#include "AEERinger.h"
#include "AEEAlert.h"
//#include "AEEPhone.h"
//#include "cm.h"
//#include "cmcall.h"

#include "AEEVector.h"

#include "AEEAnnunciator.h"         // for ANNUN_ALERT
#include "OEMCFGI.h"

#if defined( FEATURE_RECORDER)
#include "recorder.h"
#include "recorder.brh"
#define  APP_RECORDER_RES_FILE ("fs:/mod/recorder/" AEE_RES_LANGDIR RECORDER_RES_FILE)
#endif

#ifdef FEATURE_LCD_TOUCH_ENABLE//wlh add for LCD touch
#define SOUNDMENU_PT_IN_RECT(a,b,rct)      (boolean)( ((a) >= (rct).x && (a) <= ((rct).x + (rct).dx)) && ((b) >= (rct).y && (b) <= ((rct).y + (rct).dy)) )
#endif//FEATURE_LCD_TOUCH_ENABLE
/*----------------------�Ի������������������---------------------*/
#define  AEE_APPSSOUNDMENU_RES_FILE AEE_RES_LANGDIR SOUNDMENU_RES_FILE
#define SOUND_ERR(format, code1, code2, code3) //ERR(format, code1, code2, code3)
// �Ի���ر�ʱ����ֵ�б�
typedef enum DLGRetValue
{
   DLGRET_CREATE,          //��ʼֵ��������Ҫ�����Ի���
   DLGRET_OK,              //�Ի���ر�ʱ�ɷ��ص�ͨ��ֵ
   DLGRET_CANCELED,
   DLGRET_SCENEMODESUB,    //IDD_SCENEMODE_SUB
   DLGRET_SOUNDMENU,       //IDD_SOUNDMENU_MENU
   DLGRET_HINT,              //IDD_HINT_MENU
   DLGRET_RINGER,            //IDD_RINGER_MENU
#if defined(FEATURE_WMS_APP) && !defined(FEATURE_WMSAPP_ONLYSUPPORTVMAIL)
   DLGRET_SMSRING,         //IDD_SMSRING_MENU
#endif   
   DLGRET_OTHERSEL,        //IDD_OTHERSEL_MENU
   //DLGRET_AUTOANSWERSUB,   //IDD_AUTOANSWER_SUB
   DLGRET_KEYSND,          //IDD_KEYSND_MENU
   //DLGRET_KEYLENGTH,       //IDD_KEYTONE_LENGTH
   DLGRET_VOLUME,          //IDD_VOLUME_MENU
   DLGRET_VOLUMESUB,       //IDD_VOLUME_SUB
#if 0
   DLGRET_WARNING,         //IDD_WARNING_MESSEGE
#endif
   DLGRET_MESSAGE,          //IDD_MESSAGE_DIALOG
   DLGRET_MSGBOX_OK,
   DLGRET_FMMODE,
   DLGRET_MSGBOX_CANCEL

} DLGRet_Value_e_Type;

/*----------------------״̬�����������������---------------------*/
// SoundMenu Applet ״̬��״̬��
typedef enum _SoundMenuState
{
   SOUNDMENUST_NONE,
   SOUNDMENUST_INIT,
   SOUNDMENUST_MAIN,
   SOUNDMENUST_SCENEMODESUB,
   SOUNDMENUST_SOUNDMENU,
   SOUNDMENUST_HINT,
   SOUNDMENUST_RINGER,
#if defined(FEATURE_WMS_APP) && !defined(FEATURE_WMSAPP_ONLYSUPPORTVMAIL)
   SOUNDMENUST_SMSRING,
#endif   
   SOUNDMENUST_OTHERSEL,
   //SOUNDMENUST_AUTOANSWERSUB,
   SOUNDMENUST_KEYSND,
   //SOUNDMENUST_KEYLENGTH,
   SOUNDMENUST_VOLUME,
   SOUNDMENUST_VOLUMESUB,
   SOUNDMENUST_FMMODE,
#if 0
   SOUNDMENUST_WARNING,
#endif
   SOUNDMENUST_EXIT

} SoundMenuState;

// ״̬���������ظ�״̬������������ֵ����
typedef enum NextFSMAction
{
   NFSMACTION_WAIT,
   NFSMACTION_CONTINUE
} NextFSMAction;

// ����ģ����Ϣ��ģ�����ü����Ľṹ
typedef struct SoundMenuMod
{
    DECLARE_VTBL(IModule)
    uint32   m_nRefs;
} SoundMenuMod;

// �龰ģʽ
typedef enum {
   SET_PROFILE_NONE,
   SET_NORMALMODE,      //����ģʽ
   SET_QUIETMODE,       //����ģʽ
   SET_MEETING,         //����ģʽ
   SET_NOISEMODE,       //����ģʽ
   SET_CARMODE          //����ģʽ
#ifdef FEATURE_VERSION_C316
   ,SET_BLUETOOTH       //����ģʽ
#endif
} SET_PROFILE_TYPE;

// �龰ģʽ��������
typedef enum {
   SET_SCENEMODESUB_NONE,
   SET_ACTIVATE,         //����
   SET_PROFILE_RESTORE   //�ָ�Ĭ������
} SET_SCENEMODESUB_TYPE;

// ��������
typedef enum {
   SET_NONE,
   SET_RINGER_VOL,      //����
   SET_EARPIECE_VOL,    //����
   SET_KEYBEEP_VOL      //����
} SET_VOLUME_TYPE;

// ������������
typedef enum {
    SET_OTHERVOL_NONE,
    SET_MISSEDCALLALERT, //δ����������   
    SET_POWERONOFFALERT//���ػ�������ʾ
    //SET_STARTUP,         //��������
    //SET_SHUTDOWN        //�ػ����� 
} SET_OTHERVOL_TYPE;

// ������������
typedef enum {
    SET_RINGER_NONE,
    SET_RINGER,             //��������
    SET_ALARMRING,        //��������
    SET_STARTUP,           //��������
    SET_SHUTDOWN,        //�ػ�����
#if 1//def FEATURE_SMSTONETYPE_MID
    SET_SMSTONE,        //��������
#endif //#if defined FEATURE_SMSTONETYPE_MID		    
    SET_ANOTHER       //�������� 
} SET_RINGER_TYPE;

//��ʾ��ʽ
typedef enum {
    SET_Hint_NONE,
    SET_CALLHINT,        //������ʾ��ʽ
    SET_SMSHINT          //������ʾ��ʽ
} SET_HINT_TYPE;

typedef struct  _RingerMenuItemInfo
{
    uint32  id;
    AECHAR szName[MAX_RINGER_NAME];
} RingerMenuItemInfo;

// �������������б�˵�ҳ�ķ�ʽ
typedef enum _MAKE_MenuList_Mode
{
   MAKELIST_INIT,         // ���б�ʼ������
   MAKELIST_PREPAGE,      // �˵��б�ص�ǰһҳ
   MAKELIST_NEXTPAGE,     // �˵��б�ǰ��һҳ
   MAKELIST_RESUMECURPAGE // ʹ�õ�ǰ�˵��б�
} MAKE_MenuList_Mode;
// SoundMenu Applet����ṹ�壺
typedef struct _CSoundMenu
{
    DECLARE_VTBL(ISoundMenu)
    uint32          m_nRefs;          // �������ü�����
    IModule        *m_pModule;
    IDisplay       *m_pDisplay;
    IShell         *m_pShell;

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
    SoundMenuState m_ePreState;
    // Applet ��ǰ״̬
    SoundMenuState m_eCurState;
    // Applet�Ƿ��ڹ���״̬
    boolean m_bSuspending;
    // Applet�Ƿ�׼���ÿ��Դ������������¼����ı�����Ҫ���ڿ��ٰ������������
    boolean         m_bAppIsReady;

    IConfig         *m_pConfig;        //IConfig ָ��

    uint16           m_currDlgId;      //һ���˵�λ��
    uint16           m_nSubDlgId;      //�μ��˵�λ��
    uint16           m_sSubDlgId;      //�����˵�λ��
    uint16           m_fSubDlgId;      //�ļ��˵�λ��

    IRingerMgr      *m_pRingerMgr;
    IALERT          *m_pAlert;
    ISound          *m_pISound;
    //IPhone          *m_phone;

    IVector         * m_pRingerList;

    ringID            m_RingerID[PROFILENUMBER];
    // �û����õ����� ID
    uint16            m_RingID[PROFILENUMBER];

    uint16           m_lastRingerPlayed; //���һ�����ŵ�����
#if defined(FEATURE_WMS_APP) && !defined(FEATURE_WMSAPP_ONLYSUPPORTVMAIL)
    uint16           m_lastSmsRingPlayed;//���һ�����ŵ�SMS����
#endif    

    SET_PROFILE_TYPE   m_ProfileType;   //�龰ģʽ������
    SET_VOLUME_TYPE    m_VolType;       //����������
    SET_OTHERVOL_TYPE  m_OtherVolType;  //������������
    SET_RINGER_TYPE    m_RingerType;    //������������
    SET_HINT_TYPE      m_HintType;      //��ʾ��ʽ
    SET_SCENEMODESUB_TYPE  m_ScenemodesubType;  //�龰ģʽ��������

    uint16             m_wResID;          //����ID

    //IMenuCtl        *m_pMenu;  
    boolean            m_bIsSuspend;      //��������ͼƬapplet���غ�ѡ������λ��

    MAKE_MenuList_Mode m_eMakeListMode;

    uint16             m_wCurPageNum;

    byte               m_RingVol;          //��������
    byte                m_CurProfile;      //��ǰ�龰ģʽ
    byte                m_bNaturalStart;   //�ǳ��淽ʽ������״̬��
    byte                m_btTep;
    byte                m_active;          //��ǰ�����龰ģʽֵ
    byte                m_RingCurVol[PROFILENUMBER];   //��ǰ��������
    IAnnunciator         *m_pIAnn;
    AECHAR              *m_enter_address;
    boolean            m_ringer_be;
	//MODI BY YANGDECAI 09-27
	uint16             m_slecet_id;
    //boolean             m_bBTHeadSetConnected;      //Add By zzg 2013_01_09
} CSoundMenu;

/*==============================================================================
                                 �궨��ͳ���
==============================================================================*/
// ���������б�˵�ʱһҳ�������Ŀ��
#define MAXITEMS_ONEPAGE   40

// ���²˵��б��¼�
//#define EVT_UPDATAMENU (EVT_USER+16)

// Ϊ��ֹ�û����ٰ���������¼�EVT_APPISREADY���¼��ɶ�ʱ��
// SoundMenu_APPIsReadyTimer����
#define APPISREADY_TIMER       100                   
#define EVT_APPISREADY   (EVT_USER+1)                

// �û����¸��½����¼�
//#define EVT_USER_REDRAW   (EVT_USER+2)

// ��ʾ��Ϣ�ص��¼�
//#define EVT_DIALOGTIMEOUT    (EVT_USER+3)

//�رճ����¼�(�����龰ģʽ�Ӳ˵���Post��SettingMenu)
//#define EVT_USER_CLOSEAPP (EVT_USER+13)

// ״̬�ƶ��꣺�Ƚ�ǰһ״̬�õ�ǰ״̬���£��ٽ���ǰ״̬��ΪnextState
#define MOVE_TO_STATE(nextState) {            \
            SoundMenuState tpState;            \
            tpState = nextState;          \
            pMe->m_ePreState = pMe->m_eCurState; \
            pMe->m_eCurState = tpState;        \
        }
#define  SOUND_MENU_SOFK_HIGHT 0
#define  NUM_OF_MAXRINGER   256                 //���������
#define  DEFAULT_MENU       NUM_OF_MAXRINGER+1  // Ĭ��ID , for contact
#define  DOWNLOAD_MENU      NUM_OF_MAXRINGER+2  //��������ID
#if defined(FEATURE_WMS_APP) && !defined(FEATURE_WMSAPP_ONLYSUPPORTVMAIL)
#define  MAX_SMS_RINGER     6                   //������������
#endif

#define STARTARG_SOUNDMENU  ((char) 'S')        //�����ò˵����������˵�ʱ��ı�־�ַ�,Gemsea050301 add
#define STARTARG_AUTOANSWERSUB  ((char) 'a')    //��ͨ�����ò˵������Զ������˵�ʱ��ı�־�ַ�,Gemsea050310 add
#define STARTARG_RINGER_LIST  ((char) 'R')

#define NORMAL_ENTRY        0                   //���龰ģʽ�˵����ó��淽������SoundMenu Applet
#define SOUNDLIST_ENTRY     1                   //���������ò˵�����ISoundMenu_SoundList�ӿ�����SoundMenu Applet
#define AUTOANSWER_ENTRY    2                   //���Զ��ػ��˵�����ISoundMenu_Autoanswer�ӿ�����SoundMenu Applet
#define ENTER_RINGER_LIST    3 
#define SOUNDMENU_VOLUME_LEVELS    6
#define POWERONRINGID         13   //ADD BY YANGDECAI 09-27
#define POWEROFFRINGID        14

#define SOUNDMENU_VOLUME_IMAGE     "fs:/image/notice/volume.png"
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
       SoundMenu_ShowDialog
˵��:
       ������״̬����������������Ӧ�Ի�����ԴID�ĶԻ���

����:
       pMe [In]: ָ��SoundMenu Applet����ṹ��ָ��,�ýṹ����С������ض���Ϣ.
       dlgResId [in]���Ի�����ԴID���������ݸ�ID�����Ի���

����ֵ:
       ��

��ע:


==============================================================================*/
void SoundMenu_ShowDialog(CSoundMenu *pMe,uint16  dlgResId);

/*==============================================================================
����:
       SoundMenu_RouteDialogEvent
˵��:
       ������BREW�¼�·�ɸ���ǰ��Ի�����¼���������

����:
       pMe [In]: ָ��SoundMenu Applet����ṹ��ָ��,�ýṹ����С������ض���Ϣ.
       eCode [in]���¼����롣
       wParam [in]�����¼�eCode���������ݡ�
       dwParam [in]�����¼�eCode���������ݡ�

����ֵ:
       TRUE�������¼�������
       FALSE�������¼�û������

��ע:

==============================================================================*/
boolean SoundMenu_RouteDialogEvent(CSoundMenu *pMe,
    AEEEvent    eCode,
    uint16  wParam,
    uint32 dwParam
);

/*----------------------״̬����غ�������---------------------*/
/*==============================================================================
����:
       SoundMenu_ProcessState
˵��:
       SoundMenu Applet״̬��������������������pMe->m_currState������·����
       ��Ӧ�Ĵ�������

����:
       pMe [In]: ָ��SoundMenu Applet����ṹ��ָ��,�ýṹ����С������ض���Ϣ.

����ֵ:
       NFSMACTION_CONTINUE��ָʾ������״̬��״̬������ֹͣ��
       NFSMACTION_WAIT��ָʾ��Ҫ��ʾ�Ի��������û���Ӧ����״̬����

��ע:

==============================================================================*/
NextFSMAction SoundMenu_ProcessState(CSoundMenu *pMe);

/*==============================================================================
������
       SoundMenu_InitRingerList

˵����
       �������ڳ�ʼ��������

������
       pMe [in]��ָ��SoundMenu Applet����ṹ��ָ�롣
                           �ýṹ����С������ض���Ϣ��

����ֵ��
       �ޡ�

��ע��:

==============================================================================*/
void SoundMenu_InitRingerList(CSoundMenu *pMe);

/*==============================================================================
������
       SoundMenu_UpdateRingerListMenu

˵����
       ���������б�˵���

������
       pMe [in]��ָ��SoundMenu Applet����ṹ��ָ�롣
                           �ýṹ����С������ض���Ϣ��
       pMenu [in/out]��IMenuCtl�ӿ�ָ�롣

����ֵ��
        ��

��ע��

==============================================================================*/
void SoundMenu_UpdateRingerListMenu(CSoundMenu *pMe, IMenuCtl *pMenu);


#endif
//SOUNDMENUPRIV_H

