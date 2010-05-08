#ifndef COREAPP_PRIV_H
#define COREAPP_PRIV_H
/*==============================================================================
//           U S E R   I N T E R F A C E   M A I N  M O D U L E
// �ļ���
//        CoreApp_priv.h
//        ��Ȩ����(c)2009 Anylook Tech. CO.,LTD.
//        
// �ļ�˵����
//        ��Ӧ���� UI ����ʼ�㣬�ڿ���ʱ�� BREW �Զ�������         
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
// Brew libraries
#include "OEMFeatures.h"

#include "AEE.h"
#include "AEEShell.h"
#include "AEEStdLib.h"
#include "AEEAppGen.h"
#include "AEEAnnunciator.h"
#include "AEE_OEM.h"
#include "AEEConfig.h"
#include "OEMCFGI.h"
#include "AEEBatt.h"
#include "AEERUIM.h"
#include "AEECM.h"
#include "coreapp.brh"
#ifndef WIN32
#include "oemui.h"
#else
#include "OEMHelperFuncType.h"
#define ASSERT(p)
#endif
#include "AEEImage.h"
#include "CallApp.h"
//#include "AEEPhone.h"
#include "UTKUtils.h"

#if defined(AEE_STATIC)
#include "OEMClassIDs.h"
#endif
#ifdef FEATURE_UIALARM
#include "AEEAlarm.h"
#endif

#include "AEEAlert.h"
#include "recentcalls.h"
#if defined(FEATURE_WMS_APP)
#include "wms.h"
#include "OEMWMS_CacheInfo.h"
#include "WMSApp.h"
#endif
#include "AEEAddrBook.h"
#include "AEEBacklight.h"
#ifdef FEATURE_KEYGUARD
#include "OEMKeyguard.h"
#endif
#ifdef FEATURE_APP_MUSICPLAYER
#include "MusicPlayer.h"
#endif 
#include "Mainmenu.h"


/*==============================================================================
                                 
                                 �궨��ͳ���
                                 
==============================================================================*/
// ���� BREW 3 ����Ҫ���ض�����Դ�ļ���
#define  AEE_COREAPPRES_LANGFILE (AEE_RES_LANGDIR COREAPP_RES_FILE)
#ifdef FEATURE_POWERUP_REGISTER_CHINAUNICOM
#define IDLE_CFG_VERSION    1

#define     SENDREGINFOR_TIME   (2*60*1000)
#endif

// ���� Searching... ��ʾ�Ķ�ʱ��ʱ�� (����)
#define    IDLESEARCHINTIME       (20000)

//receive AEEBATTSTATUS_POWERDOWN ,set time to power down the phone
#define    POWER_DOWN_TIME       (3000)
// �ػ������ʾ�����ļ�
#define    CHARGING_ANIFILE             "fs:/image/charge/"
#define    CHARGING_FRAME_COUNT         (5)
#define    CHARGING_ANIMATE_RATE        (1000) //(500)

#define    CHARGE_FULL_STATE                (4)

#ifdef FEATURE_APP_MUSICPLAYER
//#define    DISP_MUSICNAME_MAX            (16)
#define    DISP_BLANK_WIDTH               (12)
#endif

// �ػ������ļ�����
#define    PWRON_ANI_FILE                        "fs:/image/pwronoffani/tianyion.png"
#define    PWRON_ANI_FRAME_COUNT                 (12)
#define    PWRON_ANI_RATE                        (300)
#define    PWROFF_ANI_FILE                       "fs:/image/pwronoffani/tianyioff.png"
#define    PWROFF_ANI_FRAME_COUNT                (12)
#define    PWROFF_ANI_RATE                       (300)


// ���б���Ϊ��ʱ����
#define PHONEPASSWORDLENTH      OEMNV_LOCKCODE_MAXLEN
#define LOCKUIMPASSWORDLENTH      OEMNV_LOCKUIM_MAXLEN
#define IDLE_ICON_HEIGHT        16

#define PINCODE_LENGTH          UIM_MAX_CHV_DIGITS
#define PUKCODE_LENGTH          UIM_MAX_CHV_DIGITS

#define MAX_STRING_LENGTH      64
#define MAX_BATTERY_POP_COUNT  3

//wlh 20090427 add start
//ʱ����
#define TIMEBACK_X 0
#define TIMEBACK_Y 0
#define TIMEBACK_W 176
#define TIMEBACK_H 24

//����
#define DIAL_X 0
#define DIAL_Y 0
#define DIAL_W 73
#define DIAL_H 73

//ʱ����
#define HOUR_X 
#define HOUR_Y 
#define HOUR_W 
#define HOUR_H
//������
#define MINUTE_X 
#define MINUTE_Y 
#define MINUTE_W 
#define MINUTE_H

//add by ydc 090522
//�Ƕ�sinֵ
#define SIN_0  0.000
#define SIN_6  0.105
#define SIN_12 0.208
#define SIN_18 0.309
#define SIN_24 0.407
#define SIN_30 0.500
#define SIN_36 0.588
#define SIN_42 0.669
#define SIN_48 0.743
#define SIN_54 0.809
#define SIN_60 0.866
#define SIN_66 0.914
#define SIN_72 0.951
#define SIN_78 0.978
#define SIN_84 0.996
#define SIN_90 1.000

//wlh 20090427 add end

#ifdef FEATURE_SPN_FROM_BSMCCMNC
#define OPERATOR_NAME_MAX_SIZE   32

typedef struct _ServiceProviderList
{
    word   mcc;
    byte   mnc;
    char  ids_name[OPERATOR_NAME_MAX_SIZE];
} ServiceProviderList;

static ServiceProviderList List_SP[] = 
{
#ifdef FEATURE_CARRIER_ISRAEL_PELEPHONE       
       {000,0,"Pelephone"},  
#endif       
#ifdef FEATURE_CARRIER_VENEZUELA_MOVILNET       
       {000,0,"Movilnet"},  
#endif 

#ifdef FEATURE_CARRIER_MEXICO_IUSACELL       
       {000,0,"Iusacell"},  
#endif 

       //{834,1,"China Unicom"}, 
       {809,0,"TRICOM"},
       {671,15,"Guamcell"},             // �ص�
       {310,37,"Guamcell"},             // �ص�
       {454,5,"Hutchison"},             // ���
       {460,3,"China Unicom"},          // Chinese Unicom
       {466,5,"APBW"},                  // ̨��
       {520,0,"Hutch"},                 // ̩��??  CAT
       {520,2,"CAT"},                   // ̩��
       {425,3,"Pelephone"},             // ��ɫ��
       {604,5,"WANA"},                  // Morocco WANA
       
       {421,3,"Yemen Mobile"},
       {470,6,"CITYCELL"},
       {470,4,"CITYCELL"},
       {470,3,"CITYCELL"},
       {470,2,"CITYCELL"},
       {428,0,"Skytel"},
       
       {302,64,"Bell Mobility"},        // ���ô�
       
       {244,0,"Movicel"},               // Angola Movicel
       {58,0,"Telefonica"},             // Venezuela Movistar
       {51,6,"Telefonica"},             // Peru Movistar
       
       {450,0,"SKT"},                   // ����
       {450,3,"SKT"},                   // ����
       {450,5,"SKT"},                   // ����
       {450,22,"SKT"},                  // ����
       
       {505,11,"Telstra"},              // �Ĵ�����
       
       {302,0,"Telus"},                 // ���ô�
       {302,11,"Telus"},                // ���ô�
       {302,86,"Telus"},                // ���ô�
       
       {530,02,"TML NZ"},               // ������
       
       {434,06,"Perfectum"},            // Perfectum // �������
       
       {510,0, "Flexi"},                // Indonesia flexi
       {510,3, "starone"},              // Indonesia starone
       {510,28,"Flen"},                 // Indonesia flen
       {510,99,"esia"},                 // Indonesia esia
       
#ifdef FEATURE_CARRIER_ANGOLA_MOVICEL
       {310,0,"Movicel"},               
#else 
       {310,0,"Verizon"},               // ����
#endif       
       {724,6,"VIVO"},                  //����
       {724,8,"VIVO"},                  //����
       {724,9,"VIVO"},                  //����
       {724,10,"VIVO"},                 //����
       {724,11,"VIVO"},                 //����
       {724,12,"VIVO"},                 //����
       {724,13,"VIVO"},                 //����
       {724,14,"VIVO"},                 //����
       {724,17,"VIVO"},                 //����
       {724,18,"VIVO"},                 //����
       {724,19,"VIVO"},                 //����
       {724,20,"VIVO"},                 //����
       {724,21,"VIVO"},                 //����
       {724,22,"VIVO"},                 //����
       
       {440,53,"KDDI"},                 //�ձ�
       {440,54,"KDDI"},                 //�ձ�
       {440,70,"KDDI"},                 //�ձ�
       {440,71,"KDDI"},                 //�ձ�
       {440,72,"KDDI"},                 //�ձ�
       {440,73,"KDDI"},                 //�ձ�
       {440,74,"KDDI"},                 //�ձ�
       {440,75,"KDDI"},                 //�ձ�
       {440,76,"KDDI"},                 //�ձ�
       {440,78,"KDDI"},                 //�ձ�
       
       {310,12,"Sprint"},               //����
       {310,0,"Sprint"},                //����
       
       {334,0,"Iusacell"},              //ī����
       
       {450,6,"LGT"},                   //����
       
       {452,3,"Stel"},                  //Խ��
       
       {455,2,"MacauUnicom"},           //����
       
       {111,1,"Syniverse"},             //����ת����
       
       {404,0,"Reliance"},              //ӡ��   
       {404,1,"Aircell Digilink"},   
       {404,2,"Bharti Mobile"},   
       {404,3,"Bharti Telenet"},   
       {404,4,"Idea Cellular"},   
       {404,5,"Fascel"},   
       {404,6,"Bharti Mobile"},   
       {404,7,"Idea Cellular"},   
       {404,9,"Reliance Telecom"},   
       {404,10,"Bharti Cellular"},   
       {404,11,"Sterling Cellular"},   
       {404,12,"Escotel Mobile"},   
       {404,13,"Hutchinson Essar South"},   
       {404,14,"Spice"},   
       {404,15,"Aircell Digilink"},
       {404,16,"Hexcom"},
       {404,18,"Reliance Telecom"},
       {404,19,"Escotel Mobile"},
       {255,04,"Inter"}, //intertelecom
       {255,23,"CDMA Ukraine"} //CDMA Ukraine       
       
};
#endif //FEATURE_SPN_FROM_BSMCCMNC

//#define CORE_DEBUG
#ifdef FEATURE_OEM_DEBUG
#define CORE_ERR  DBGPRINTF
#else
#define CORE_ERR(format, ...) //ERR(format, code1, code2, code3)
#endif
// �رնԻ����
#define CLOSE_DIALOG(DlgRet)                    \
{                                               \
    pMe->m_eDlgRet = DlgRet;                    \
    (void) ISHELL_EndDialog(pMe->a.m_pIShell);  \
}

// ״̬�ƶ��꣺�Ƚ�ǰһ״̬�õ�ǰ״̬���£��ٽ���ǰ״̬��ΪnextState
#define MOVE_TO_STATE(nextState)            \
{                                           \
    CoreAppState    State;                  \
    State = nextState;                      \
    pMe->m_ePreState = pMe->m_eCurState;    \
    pMe->m_eCurState = State;               \
}

// ������ײ���ʾ���궨��
#define COREAPP_DRAW_BOTTOMBAR(x)                           \
{                                                   \
    BottomBar_Param_type BarParam;                  \
    MEMSET(&BarParam, 0, sizeof(BarParam));         \
    BarParam.eBBarType = x;                         \
    DrawBottomBar(pMe->m_pDisplay, &BarParam);      \
}
/*==============================================================================
                                 
                                �ڲ��������Ͷ���
                                 
==============================================================================*/
// Core Applet �Ի���ر�ʱ����ֵ�б�
typedef enum DLGRetValue
{
    // ��ʼֵ��������Ҫ�����Ի���
    DLGRET_CREATE,/*0*/
    
    DLGRET_ACCEPTED,/*1*/
    DLGRET_REJECTED,/*2*/
    
    DLGRET_EMGCALL,/*3*/
    
    // ��������
    DLGRET_CONTINPUT,/*4*/
    
    DLGRET_ENTEROK,/*5*/
    
    DLGRET_DIAL,/*6*/
    DLGRET_MSG,/*7*/
    
    // ������ʾ
#if defined(FEATURE_WMS_APP)
    DLGRET_SMSTIPS,/*8*/
    DLGRET_VIEWSMS,/*9*/
    DLGRET_SMSTIPS_OK,/*10*/
#endif    
    
    // �Ի���ر�ʱ�ɷ��ص�ͨ��ֵ
    DLGRET_OK,/*11*/
    DLGRET_MSGOK,/*12*/
    
    DLGRET_INITED,/*13*/
    DLGRET_CANCELED,/*14*/
    DLGRET_BATT_INFO,/*15*/
    DLGRET_RTC,
    DLGRET_ENTER,
    DLGRET_GS_SHAKE

#ifdef FEATURE_PLANEMODE
    ,DLGRET_YES
    ,DLGRET_NO
#endif
} DLGRetValue;

// Core Applet ״̬���������ظ�״̬��������ֵ����
typedef enum NextFSMAction
{
    NFSMACTION_WAIT,
    NFSMACTION_CONTINUE
} NextFSMAction;


// Core Applet ״̬��״̬��
typedef enum _CoreAppState
{
   COREST_NONE,//0
   
   // ����Ҫ�ĳ�ʼ������,�޽���
   COREST_INIT,/*1*/
   
   COREST_ALARM,/*2*/
   // �ػ����
   COREST_LPM,/*3*/
   
   // ��������֤
   COREST_SERVICEREQ,/*4*/
   
   // �ֻ�������֤
   COREST_VERIFYPHONEPWD,/*5*/
   
   // UIM �����֤(����PIN��PUK���Ϸ�����֤)
   COREST_VERIFYUIM,/*6*/
   
   // UIM��������ʾ(����PIN��PUK���Ϸ�����֤ʧ�ܡ��п��汾ʱû���뿨)
   COREST_UIMERR,/*7*/
   
   // ��������
   COREST_EMERGENCYCALL,/*8*/
   
   // ����ʱ,�û������֤ͨ������뿪������ǰ�Ĺ���״̬��
   // ���������ֺ�ʱ�ٵ�ϵͳ��Ҫ�ĳ�ʼ��
   COREST_POWERONSYSINIT,/*9*/
   
   // ���ſ�������
   COREST_STARTUPANI,/*10*/
   
   // ����Ӧ�� Cache ���ݳ�ʼ��
   COREST_POWERONAPPSDATAINIT,/*11*/
   
   // ����״̬
   COREST_STANDBY,/*12*/
   
#if defined(FEATURE_WMS_APP)
   COREST_SMSTIP,/*13*/
#endif   
   
   // ͨ����Ϣ��ʾ���¼�֪ͨ״̬
   COREST_NOTICE,/*14*/
   
   // ˯��(��Ļ����״̬)
   COREST_DEEPSLEEP,/*15*/
   
   // �ػ���ʾ
   COREST_POWEROFF,/*16*/

#ifdef FEATURE_UTK2
   //UTK refresh
   COREST_UTKREFRESH,
#endif //FEATURE_UTK2   

    //��������
#ifdef FATRUE_LOCK_IMSI_MCCMNC
   COREST_IMSIMCCPWD,/*17*/
#endif
} CoreAppState;

typedef enum
{
    ENTER_NONE,         // û��Ҫ������ PIN �� PUK ��
    ENTERPIN_ONLY,      // Ҫ������ PIN ��
    ENTERPUK_STEP0,     // Ҫ������ PUK ���һ�������� PUK ��
    ENTERPUK_STEP1,     // Ҫ������ PUK ��ڶ����������� PIN ��
    ENTERPUK_STEP2      // Ҫ������ PUK ���������ȷ���� PIN ��
} Enter_UIMSecurityCode_e_Type;

typedef enum
{
    UIMERR_NONE,         // UIM ����������
    UIMERR_NOUIM,        // û���� UIM ��
    UIMERR_BLOCKED,      // UIM ����Ч(ԭ����PUK�볬���涨�������¿�ʧЧ)
    UIMERR_LOCKED        // UIM ����Ч(ԭ���ǿ���������Ӫ��Ҫ���Ҫ������)
} UIMERR_e_Type;

typedef enum
{
    POWERDOWN_NORMAL,               // �����ػ�
    POWERDOWN_RESET                 // �ػ�������
#ifdef FEATURE_UIM
    , POWERDOWN_RUIM_DOOR_RESET     // ��Ҫ��ػ�����
#endif
} PowerDownType;
#ifndef  FEATURE_2008_POWERON_LOGIC
typedef enum
{
    ON_LINE_FROM_NONE = 0x00,
    ON_LINE_FROM_NORMAL,
    ON_LINE_FROM_EMERGENCY
} On_line_from;
#endif

#ifdef FEATURE_POWERUP_REGISTER_CHINAUNICOM
typedef struct _REGISTER_ITEM
{
    char        szMobileIMSI[16];   // �ֻ� IMSI ��
    boolean     m_bRegistered;      // �Ƿ�ʹ�ø� IMSI ��ע��
} REGISTER_ITEM;

// ���� Applet ���ýṹ
typedef struct _IdleAPP_Config_Type
{
    REGISTER_ITEM    m_RegItem;
    
    // ����ָʾ������Ϣ�Ƿ��ʼ�������ض�ֵ IDLE_CFG_VERSION��ʾ�ѳ�ʼ��
    int   nInitIndictor;  
} IdleAPP_Config_Type;
#endif

// �Ի����¼�������ָ�����Ͷ���
typedef boolean (*PFNDLGHANDLER)(void *pUser, AEEEvent evt, uint16 w, uint32 dw);
//wlh 20090521 add start 
typedef  struct _xIcon_AppList//һ��11��С����
{
	boolean  m_b_Max;//true��ʾ��ͼ��
	boolean  m_b_Move;//true��ʾ�����ƶ�
	AEERect  m_rc;   //ֻ����m_b_MaxΪtrue�������ʹ�ã���ʾ��ǰ�Ĵ�ͼ������
	int      xIconAppNum;//��ǰxIcon_AppList�����е�С������ʾ��ţ�1��������
	IImage              *m_ImageSmall;
	IImage              *m_ImageSmallCheck;
	IImage              *m_ImageBig;
} xIcon_AppList;
typedef struct _IdleIcon
{
	boolean             m_b_leftflag;//true ��ʾ��߼�ͷ��ʾ
	boolean             m_b_rightflag;//true ��ʾ�ұ߼�ͷ��ʾ
	boolean             m_b_show;//true ��ʾ��ʾ������
	AEERect             m_icon_rc;
	IImage *            m_iconback_Image;//������
	IImage *            m_arrow_uncheck;//��ʾ��ͷ
	IImage *            m_arrow_check;//���ؼ�ͷ
	IImage *            m_arrowleft_Image;//���ͷ
	IImage *            m_arrowright_Image;//�Ҽ�ͷ
	xIcon_AppList       *m_IconList[11];
	int                 m_fisticon;//��ǰ��һ��icon,ָСͼ��
	int                 m_checkicon;//��ǰѡ�е�icon,ָСͼ��
	//Сͼ��
	//IImage              *m_ImageSmall[11];
	//IImage              *m_ImageSmallCheck[11];
	/*
	IImage *            m_blue_ImageSmall;//����             1
	IImage *            m_browser_ImageSmall;//���������    2
	IImage *            m_calc_ImageSmall;//������           3
	IImage *            m_date_ImageSmall;//�ճ̱�           4
	IImage *            m_file_ImageSmall;//��ý���ļ���     5
	IImage *            m_health_ImageSmall;//����           6
	IImage *            m_mediaplay_ImageSmall;//��Ƶ������  7
	IImage *            m_music_ImageSmall;//MP3             8
	IImage *            m_sms_ImageSmall;//����              9
	IImage *            m_sports_ImageSmall;//�Ʋ���         10
	IImage *            m_usb_ImageSmall;//USB               11
	*/
	//��ͼ��
	//IImage              *m_ImageBig[11];
	/*
	IImage *            m_blue_ImageBig;//����                1
	IImage *            m_browser_ImageBig;//���������       2
	IImage *            m_calc_ImageBig;//������              3
	IImage *            m_date_ImageBig;//�ճ̱�              4
	IImage *            m_file_ImageBig;//��ý���ļ���        5
	IImage *            m_health_ImageBig;//����              6
	IImage *            m_mediaplay_ImageBig;//��Ƶ������     7
	IImage *            m_music_ImageBig;//MP3                8
	IImage *            m_sms_ImageBig;//����                 9
	IImage *            m_sports_ImageBig;//�Ʋ���            10
	IImage *            m_usb_ImageBig;//USB                  11
	*/
}IdleIcon;
//wlh 20090521 add end 

typedef struct _CCoreApp
{
    // ���ṹ�ĵ�һ�����ݳ�Ա������ AEEApplet ��
    AEEApplet a ;
    
    // �ⲿ�ӿ�ָ��
    IDisplay            *m_pDisplay;
    IAnnunciator        *m_pIAnn;
    IConfig             *m_pConfig;
    IBatt               *m_pBatt;
    IRUIM               *m_pIRUIM;
    ICM                 *m_pCM;
    //IPhone       *m_pPhone;
    
    // ��ǰ��Ի���IDialog�ӿ�ָ��
    IDialog             *m_pActiveDlg;
    
    // ��ǰ��Ի�����¼�������ָ��
    PFNDLGHANDLER       m_pDialogHandler;

    boolean             m_bSuspended; 
    
    boolean            m_bActive;
#ifdef FEATURE_UIALARM
    IAlarm       *m_pAlarm;
#endif    
    
    // ���� Applet �ľ�����ʾ����
    AEERect             m_rc;
    
    // ��Ի���ر�ʱ�ķ��ؽ��
    DLGRetValue         m_eDlgRet;
    
    // ��ǰ��Ի���ID
    uint16              m_wActiveDlgID;
    
    // �Ƿ񲻸�д�Ի��򷵻ؽ����һ�������Ҫ��д(FALSE)��
    boolean             m_bNotOverwriteDlgRet;
    
    // Applet ǰһ״̬
    CoreAppState        m_ePreState;
    
    // Applet ��ǰ״̬
    CoreAppState        m_eCurState;
    
    // Applet ��һ״̬
    CoreAppState        m_eNextState;
    
    // ���ڱ����û�������ֻ�����
    char                m_strPhonePWD[PHONEPASSWORDLENTH + 1];
    char                m_strLockuimPWD[LOCKUIMPASSWORDLENTH + 1];
    uint16             m_strlockuimPWD;
    UIMERR_e_Type       m_eUIMErrCode;
    Enter_UIMSecurityCode_e_Type  m_eRUIMSCode;
    
    char                m_strPIN1[PINCODE_LENGTH + 1];
    char                m_strPIN2[PINCODE_LENGTH + 1];
    char                m_strPUK[PUKCODE_LENGTH + 1];
    
    // ��Ϣ�ı��ַ�������Դ�ļ��е� ID
    uint16              m_nMsgID;
    
    // �Ƿ񲶻�ϵͳʱ��
    boolean             m_bAcquiredTime;
    
    // ���ڼ�¼�����²���ʱ��һ����������
    AECHAR              m_wstrEnterNum[4];
    
    // LARGE ����߶�
    int                 m_nLargeFontHeight;
    
    // Normal ����߶�
    int                 m_nNormalFontHeight;
    
    // �Ƿ�Ҫ�˳� Core Applet : ֻ���ڹػ�ʱ������Ϊ TRUE
    boolean             m_bExit;
    
    PowerDownType       m_ePowerDownType;

   // �Ƿ���ӵ�Դ
    boolean             m_bExtPwrState;
    boolean             m_bemergencymode;

    IALERT              *m_pAlert;           //IALERTָ��
    
    //��������ͼƬ
    IImage             *m_pStartupAniImg;
    //IImage             *m_pSubStartupAniImg;//not used,2008/03/31
    
    // ��������ÿ֡��ʾ�ĺ�����
    uint16             m_wStartupAniFrameRate;
    
    // ������������ʱ��
    uint16             m_wStartupAniTime;
#ifdef FEATRUE_AUTO_POWER
    boolean            m_b_autopower_off;
    boolean            m_b_needclose_core;
#endif
#ifndef CUST_EDITION
    PowerDown_Alarm_Cfg   m_PowerDownAlarmCfg;
#endif
    IAddrBook          *m_pAddrPhone;     // IADDRBOOK interface for phone
#ifdef FEATURE_RUIM_PHONEBOOK
    IAddrBook          *m_pAddrRUIM;      // IADDRBOOK interface for RUIM
#endif /* FEATURE_RUIM_PHONEBOOK */	
    boolean             m_SYS_MODE_NO_SRV;
    
#if defined(FEATURE_WMS_APP)
    boolean             m_bsmstipscheck;
#endif
    AECHAR              svc_p_name[UIM_CDMA_HOME_SERVICE_SIZE+1];
    IBacklight*         m_pBacklight;
    byte                m_battery_time;
    boolean             m_battery_state;
#ifdef FEATURE_UTK2
    uint16     wRefreshMask;
#endif
    
    IImage *            m_battery_Image;
    byte                m_battery_count;
#ifdef FEATURE_KEYGUARD
    boolean             m_b_set_lock;
#endif
#ifdef FEATRUE_SET_ANN_FULL_SCREEN
    db_capture_type    m_capture;
#endif
    boolean             m_b_PH_INFO_AVAIL;
    AECHAR            *m_cdg_msgptr;

#if defined( FEATURE_POWERDOWN_ALARM)
	boolean powerupByAlarm;
#endif
boolean bunlockuim;

#ifdef FEATURE_PLANEMODE
boolean bPlaneModeOn;
#endif
#ifdef FEATURE_APP_MUSICPLAYER
    uint16  m_nAutoScrollIdx;
    AECHAR  *m_pMusicName;
#endif
#ifndef  FEATURE_2008_POWERON_LOGIC
    On_line_from             m_b_online_from;
#endif
	//wlh 20090427 add start ���� Dialʱ����� ����ʾ����
	boolean             m_b_dialflag;//true ��ʾ���̣�false ��ʾʱ����
	//boolean             m_b_dialmove;//true ��ʾ�ƶ���false û�ƶ�
    AEERect             m_dial_rc;
	IImage *            m_dial_Image;
	IImage *            m_time_Image;
	
	//add by ydc 20090525
	IImage *            m_dial_minue_image[60];
	IImage *            m_dial_hour_image[60];

	uint16              m_move_xstation;
	uint16              m_move_ystation;
	uint16              m_down_xstation;
	uint16              m_down_ystation;
	uint16              m_up_xstation;
	uint16              m_up_ystation;
	int                 m_now_Num;//��ǰ�ƶ��Ķ������0��ʱ��,Ĭ��-1
	//wlh 20090427 add end 
	IdleIcon *          m_Idle_Icon;
} CCoreApp;

/*==============================================================================

                                 ��������

==============================================================================*/
#ifdef FEATURE_UIALARM
void CoreApp_EnableShellAlarms(CCoreApp  *pMe, boolean  bEnabled);
#endif

/*==============================================================================
����:
    CoreApp_ShowDialog
       
˵��:
    ������״̬����������������Ӧ�Ի�����ԴID�ĶԻ���

����:
    pMe [In]: ָ�� Core Applet����ṹ��ָ��,�ýṹ����С������ض���Ϣ.
    dlgResId [in]���Ի�����ԴID���������ݸ�ID�����Ի���

����ֵ:
    none

��ע:

==============================================================================*/
void CoreApp_ShowDialog(CCoreApp *pMe,uint16  dlgResId);

/*==============================================================================
����:
    CoreApp_ShowMsgDialog
       
˵��:
    ����������ʾָ����Ϣ�ĶԻ���

����:
    pMe [In]: ָ�� Core Applet����ṹ��ָ��,�ýṹ����С������ض���Ϣ.
    nResId [in]����Ϣ�ַ�������Դ�ļ��е� ID �š�

����ֵ:
    none

��ע:

==============================================================================*/
void CoreApp_ShowMsgDialog(CCoreApp *pMe,uint16  nResId);

/*==============================================================================
����:
    CoreApp_RouteDialogEvent
       
˵��:
    ������BREW�¼�·�ɸ���ǰ��Ի�����¼���������

����:
    pMe [In]: ָ�� Core Applet����ṹ��ָ��,�ýṹ����С������ض���Ϣ.
    eCode [in]���¼����롣
    wParam [in]�����¼�eCode���������ݡ�
    dwParam [in]�����¼�eCode���������ݡ�

����ֵ:
    TRUE�������¼�������
    FALSE�������¼�û������

��ע:

==============================================================================*/
boolean CoreApp_RouteDialogEvent(CCoreApp *pMe,
    AEEEvent    eCode,
    uint16  wParam,
    uint32 dwParam
);

/*==============================================================================
����:
    CoreApp_SetDialogHandler
       
˵��:
    �������� pMe->m_wActiveDlgID �趨�Ի��¼�������ָ�� m_pDialogHandler ��Ӧֵ��

����:
    pMe [In]: ָ�� Core Applet����ṹ��ָ��,�ýṹ����С������ض���Ϣ.

����ֵ:
    none

��ע:

==============================================================================*/
void CoreApp_SetDialogHandler(CCoreApp *pMe);

/*==============================================================================
����:
    CoreApp_ProcessState
       
˵��:
    Core Applet״̬�������������������� pMe->m_eCurState ������·����
    ��Ӧ��״̬��������

����:
    pMe [In]: ָ�� Core Applet ����ṹ��ָ��,�ýṹ����С������ض���Ϣ.

����ֵ:
    NFSMACTION_CONTINUE: ָʾ������״̬��״̬������ֹͣ��
    NFSMACTION_WAIT: ָʾ��Ҫ��ʾ�Ի��������û���Ӧ�˳�״̬����

��ע:

==============================================================================*/
NextFSMAction CoreApp_ProcessState(CCoreApp *pMe);

/*==============================================================================
����:
    CoreApp_SetOperatingModeOnline

˵��:
    ���ڽ������������߲���ģʽ��

����:
    pMe [in]:ָ�� Core Applet����ṹ��ָ�롣�ýṹ����С������ض���Ϣ��

����ֵ:
    none

��ע:
       
==============================================================================*/
void CoreApp_SetOperatingModeOnline(CCoreApp *pMe);
/*==============================================================================
����:
    CoreApp_RegisterNotify

˵��:
    ���ڽ������������߲���ģʽ��

����:
    pMe [in]:ָ�� Core Applet����ṹ��ָ�롣�ýṹ����С������ض���Ϣ��

����ֵ:
    none

��ע:
       
==============================================================================*/
boolean CoreApp_RegisterNotify(CCoreApp *pMe);

/*==============================================================================
����:
    CoreApp_InitExtInface

˵��:
    ���ڽ������������߲���ģʽ��

����:
    pMe [in]:ָ�� Core Applet����ṹ��ָ�롣�ýṹ����С������ض���Ϣ��

����ֵ:
    none

��ע:
       
==============================================================================*/
boolean CoreApp_InitExtInterface(CCoreApp *pMe);

/*==============================================================================
����      :  CoreApp_IsIdle

˵��      :  �������ڻ�ȡCOREAPP�Ƿ���IDLE״̬
==============================================================================*/
boolean CoreApp_IsIdle(void);
boolean CoreApp_IsEmergencyMode(ICM* pICM);

void CoreApp_UpdateAnnunciator(CCoreApp *pMe);

void CoreApp_Draw_Charger_image(void *pp);

void CoreApp_Poweroff_Phone(void *pp);
#ifdef FEATURE_POWERUP_REGISTER_CHINAUNICOM
/*==============================================================================
������
    CoreApp_SendReginfoTimer

˵����
    ���ڷ��� EVT_SENDREGINFOR �� AEECLSID_CORE_APP ֪ͨ�������ֻ�ע���¼���

������
    pme [in]��ָ��CCoreApp Applet����ṹ��ָ�롣�ýṹ����С������ض���Ϣ��

����ֵ��
    �ޡ�

��ע��
    �� Applet �����𣬽������κβ���ֱ�ӷ��ء�
       
==============================================================================*/
void CoreApp_SendReginfoTimer(void *pme);

/*==============================================================================
������
    CoreApp_SendReginfo

˵����
    ������������ע����Ϣ��

������
    pMe [in]��ָ��CCoreApp Applet����ṹ��ָ�롣�ýṹ����С������ض���Ϣ��
       
����ֵ��
    int 

��ע:

==============================================================================*/
int CoreApp_SendReginfo(CCoreApp *pMe);
#endif
#ifndef  FEATURE_2008_POWERON_LOGIC
void CoreApp_load_uim_esn(CCoreApp *pMe);
#endif
#endif
