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
#include "AEEBattery.h"
#include "AEERUIM.h"
#ifdef FEATURE_ICM
#include "AEECM.h"
#else
#include "AEETelephone.h"
#include "AEETelDef.h"
#endif
#include "coreapp.brh"
#ifndef WIN32
#include "ui.h"
#else
#include "OEMHelperFuncType.h"
#define ASSERT(p)
#endif
#include "AEEImage.h"
#include "CallApp.h"
//#include "AEEPhone.h"
#ifdef FEATURE_UTK2
#include "UTKUtils.h"
#endif

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
#include "core_ui.h"
#include "AEECard.h"
#include "AEEDeviceNotifier.h"

/*==============================================================================
                                 
                                 �궨��ͳ���
                                 
==============================================================================*/
#ifndef RELEASEIF
#define RELEASEIF(x) if (x) { IBASE_Release((IBase *)(x)); (x) = 0; }
#endif

// ���� BREW 3 ����Ҫ���ض�����Դ�ļ���
#define  AEE_COREAPPRES_LANGFILE (AEE_RES_LANGDIR COREAPP_RES_FILE)
#ifdef FEATURE_POWERUP_REGISTER_CHINAUNICOM
#define IDLE_CFG_VERSION    1

#define     SENDREGINFOR_TIME   (2*60*1000)
#endif
#ifdef FEATURE_SEAMLESS_SMS
#define     SENDSEAMLESSSMS_TIME   (2*60*1000)
#endif
#define     RESETPROFILE_TIME      (1*60*1000)
// ���� Searching... ��ʾ�Ķ�ʱ��ʱ�� (����)
#define    IDLESEARCHINTIME       (10000)

//receive AEEBATTSTATUS_POWERDOWN ,set time to power down the phone
#define    POWER_DOWN_TIME       (5000)
// �ػ������ʾ�����ļ�
#define    CHARGING_ANIFILE             "fs:/image/charge/"
#define    CHARGING_FRAME_COUNT         (5)
#define    CHARGING_ANIMATE_RATE        (1000) //(500)

#define    CHARGE_FULL_STATE                (4)
#if defined(FEATURE_VERSION_C306)||defined(FEATURE_VERSION_W0216A)
#define AVK_SEND_TWO     2
#elif defined(FEAUTRE_VERSION_N450)|| defined(FEATURE_VERSION_N021) || defined(FEATURE_VERSION_C01)
#define AVK_SEND_TWO     2
#else
#define AVK_SEND_TWO     1
#endif

#ifdef FEATURE_APP_MUSICPLAYER
//#define    DISP_MUSICNAME_MAX            (16)
#define    DISP_BLANK_WIDTH               (8)
#endif

#ifdef FEATURE_USES_LOWMEM
#define    ANI_RATE                              (180)
#define    PWRON_ANI_FILE                        "fs:/image/pwronoffani/poweronani.jpg"
#define    PWRON_ANI_FRAME_COUNT                 (12)
#define    PWRON_ANI_RATE                        (380)
#define    PWROFF_ANI_FILE                       "fs:/image/pwronoffani/poweroffani.jpg"
#define    PWROFF_ANI_FRAME_COUNT                (12)
#define    PWROFF_ANI_RATE                       (380)
#else
// �ػ������ļ�����
#ifdef FEATURE_VERSION_FLEXI203
#define    PWRON_ANI_FILE                        "fs:/image/pwronoffani/poweronani.png"
#define    PWRON_ANI_FRAME_COUNT                 (17)
#define    PWRON_ANI_RATE                        (300)
#define    PWROFF_ANI_FILE                       "fs:/image/pwronoffani/poweronani.png"
#define    PWROFF_ANI_FRAME_COUNT                (17)
#define    PWROFF_ANI_RATE                       (300)
#elif defined FEATURE_VERSION_FLEXI203P  
#define    PWRON_ANI_FILE                        "fs:/image/pwronoffani/poweronani.gif"
#define    PWRON_ANI_FRAME_COUNT                 (1)
#define    PWRON_ANI_RATE                        (5000)
#define    PWROFF_ANI_FILE                       PWRON_ANI_FILE
#define    PWROFF_ANI_FRAME_COUNT                PWRON_ANI_FRAME_COUNT
#define    PWROFF_ANI_RATE                       PWRON_ANI_RATE
#elif defined FEATURE_VERSION_SMART
#define    PWRON_ANI_FILE                        "fs:/image/pwronoffani/poweronani.png"
#define    PWRON_ANI_FRAME_COUNT                 (10)
#define    PWRON_ANI_RATE                        (400)
#define    PWROFF_ANI_FILE                       "fs:/image/pwronoffani/poweroffani.png"
#define    PWROFF_ANI_FRAME_COUNT                (10)
#define    PWROFF_ANI_RATE                       (350)
#elif defined FEATURE_VERSION_M8
#define    PWRON_ANI_FILE                        "fs:/image/pwronoffani/poweronani.png"
#define    PWRON_ANI_FRAME_COUNT                 (10)
#define    PWRON_ANI_RATE                        (400)
#define    PWROFF_ANI_FILE                       "fs:/image/pwronoffani/poweroffani.png"
#define    PWROFF_ANI_FRAME_COUNT                (10)
#define    PWROFF_ANI_RATE                       (350)
#elif defined FEATURE_VERSION_M8021
#define    PWRON_ANI_FILE                        "fs:/image/pwronoffani/poweronani.png"
#define    PWRON_ANI_FRAME_COUNT                 (10)
#define    PWRON_ANI_RATE                        (400)
#define    PWROFF_ANI_FILE                       "fs:/image/pwronoffani/poweroffani.png"
#define    PWROFF_ANI_FRAME_COUNT                (10)
#define    PWROFF_ANI_RATE                       (350)
#elif defined FEATURE_VERSION_M8P
#define    PWRON_ANI_FILE                        "fs:/image/pwronoffani/poweronani.png"
#define    PWRON_ANI_FRAME_COUNT                 (10)
#define    PWRON_ANI_RATE                        (400)
#define    PWROFF_ANI_FILE                       "fs:/image/pwronoffani/poweroffani.png"
#define    PWROFF_ANI_FRAME_COUNT                (10)
#define    PWROFF_ANI_RATE                       (350)
#elif defined FEATURE_VERSION_SMARTP
#define    PWRON_ANI_FILE                        "fs:/image/pwronoffani/poweronani.png"
#define    PWRON_ANI_FRAME_COUNT                 (10)
#define    PWRON_ANI_RATE                        (400)
#define    PWROFF_ANI_FILE                       "fs:/image/pwronoffani/poweroffani.png"
#define    PWROFF_ANI_FRAME_COUNT                (10)
#define    PWROFF_ANI_RATE                       (350)
#elif defined FEATURE_VERSION_M8P
#define    PWRON_ANI_FILE                        "fs:/image/pwronoffani/poweronani.png"
#define    PWRON_ANI_FRAME_COUNT                 (10)
#define    PWRON_ANI_RATE                        (400)
#define    PWROFF_ANI_FILE                       "fs:/image/pwronoffani/poweroffani.png"
#define    PWROFF_ANI_FRAME_COUNT                (10)
#define    PWROFF_ANI_RATE                       (350)
#elif defined FEATURE_VERSION_FLEXI021
#define    PWRON_ANI_FILE                        "fs:/image/pwronoffani/poweronani.jpg"
#define    PWRON_ANI_FRAME_COUNT                 (12)
#define    PWRON_ANI_RATE                        (380)
#define    PWROFF_ANI_FILE                       "fs:/image/pwronoffani/poweroffani.jpg"
#define    PWROFF_ANI_FRAME_COUNT                (12)
#define    PWROFF_ANI_RATE                       (380)
#elif defined FEATURE_VERSION_ESIA021
#define    PWRON_ANI_FILE                        "fs:/image/pwronoffani/poweronani.jpg"
#define    PWRON_ANI_FRAME_COUNT                 (12)
#define    PWRON_ANI_RATE                        (380)
#define    PWROFF_ANI_FILE                       "fs:/image/pwronoffani/poweroffani.jpg"
#define    PWROFF_ANI_FRAME_COUNT                (12)
#define    PWROFF_ANI_RATE                       (380)
#elif defined FEATURE_VERSION_IVIO203
#define    PWRON_ANI_FILE                        "fs:/image/pwronoffani/poweronani.png"
#define    PWRON_ANI_FRAME_COUNT                 (15)
#define    PWRON_ANI_RATE                        (220)
#define    PWROFF_ANI_FILE                       "fs:/image/pwronoffani/poweronani.png"
#define    PWROFF_ANI_FRAME_COUNT                (15)
#define    PWROFF_ANI_RATE                       (220)
#elif defined (FEATURE_VERSION_C500BE)
#define    PWRON_ANI_FILE                        "fs:/image/pwronoffani/poweronani.png"
#define    PWRON_ANI_FRAME_COUNT                 (15)
#define    PWRON_ANI_RATE                        (220)
#define    PWROFF_ANI_FILE                       "fs:/image/pwronoffani/poweronani.png"
#define    PWROFF_ANI_FRAME_COUNT                (15)
#define    PWROFF_ANI_RATE                       (220)
#elif defined FEATURE_VERSION_IVIO021
#define    PWRON_ANI_FILE                        "fs:/image/pwronoffani/poweronani.jpg"
#define    PWRON_ANI_FRAME_COUNT                 (12)
#define    PWRON_ANI_RATE                        (380)
#define    PWROFF_ANI_FILE                       "fs:/image/pwronoffani/poweroffani.jpg"
#define    PWROFF_ANI_FRAME_COUNT                (12)
#define    PWROFF_ANI_RATE                       (380)
#elif defined FEATURE_VERSION_H19C
#define    PWRON_ANI_FILE                        "fs:/image/pwronoffani/poon.gif"
#define    PWRON_ANI_FRAME_COUNT                 (1)
#define    PWRON_ANI_RATE                        (5000)
#define    PWROFF_ANI_FILE                       "fs:/image/pwronoffani/pooff.gif"
#define    PWROFF_ANI_FRAME_COUNT                PWRON_ANI_FRAME_COUNT
#define    PWROFF_ANI_RATE                       PWRON_ANI_RATE
#elif defined FEATURE_VERSION_ITEL
#define    PWRON_ANI_FILE                        "fs:/image/pwronoffani/onoff.gif"
#define    PWRON_ANI_FRAME_COUNT                 (1)
#define    PWRON_ANI_RATE                        (5000)
#define    PWROFF_ANI_FILE                       PWRON_ANI_FILE
#define    PWROFF_ANI_FRAME_COUNT                PWRON_ANI_FRAME_COUNT
#define    PWROFF_ANI_RATE                       PWRON_ANI_RATE
#elif defined FEATURE_VERSION_KARBONN     //add by yangdecai
#define    PWRON_ANI_FILE                        "fs:/image/pwronoffani/poweron.gif"
#define    PWRON_ANI_FRAME_COUNT                 (1)
#define    PWRON_ANI_RATE                        (5000)
#define    PWROFF_ANI_FILE                       PWRON_ANI_FILE
#define    PWROFF_ANI_FRAME_COUNT                PWRON_ANI_FRAME_COUNT
#define    PWROFF_ANI_RATE                       PWRON_ANI_RATE
#elif defined FEATURE_VERSION_C01
#define    PWRON_ANI_FILE                        "fs:/image/pwronoffani/poweronoffani.png"
#define    PWRON_ANI_FRAME_COUNT                 (1)
#define    PWRON_ANI_RATE                        (4000)
#define    PWROFF_ANI_FILE                       "fs:/image/pwronoffani/poweronoffani.png"
#define    PWROFF_ANI_FRAME_COUNT                PWRON_ANI_FRAME_COUNT
#define    PWROFF_ANI_RATE                       PWRON_ANI_RATE
#elif defined FEATURE_VERSION_HITZ181
#define    PWRON_ANI_FILE                        "fs:/image/pwronoffani/poweronani.png"
#define    PWRON_ANI_FRAME_COUNT                 (5)
#define    PWRON_ANI_RATE                        (800)
#define    PWROFF_ANI_FILE                       "fs:/image/pwronoffani/poweroffani.png"
#define    PWROFF_ANI_FRAME_COUNT                (6)
#define    PWROFF_ANI_RATE                       (660)
#elif defined FEATURE_VERSION_HITZ181c
#define    PWRON_ANI_FILE                        "fs:/image/pwronoffani/poweronani.png"
#define    PWRON_ANI_FRAME_COUNT                 (5)
#define    PWRON_ANI_RATE                        (800)
#define    PWROFF_ANI_FILE                       "fs:/image/pwronoffani/poweroffani.png"
#define    PWROFF_ANI_FRAME_COUNT                (6)
#define    PWROFF_ANI_RATE                       (660) 
#elif defined FEATURE_VERSION_W515V3
#define    PWRON_ANI_FILE                        "fs:/image/pwronoffani/poweronoff.jpg"
#define    PWRON_ANI_FRAME_COUNT                 (1)
#define    PWRON_ANI_RATE                        (4000)
#define    PWROFF_ANI_FILE                       "fs:/image/pwronoffani/poweronoff.jpg"
#define    PWROFF_ANI_FRAME_COUNT                (1)
#define    PWROFF_ANI_RATE                       (4000) 
#elif defined FEATURE_VERSION_OLIVE203
#define    PWRON_ANI_FILE                        "fs:/image/pwronoffani/poweronoff.gif"
#define    PWRON_ANI_FRAME_COUNT                 (1)
#define    PWRON_ANI_RATE                        (5000)
#define    PWROFF_ANI_FILE                       PWRON_ANI_FILE
#define    PWROFF_ANI_FRAME_COUNT                PWRON_ANI_FRAME_COUNT
#define    PWROFF_ANI_RATE                       PWRON_ANI_RATE
#elif defined FEATURE_VERSION_C306
#define    PWRON_ANI_FILE                        "fs:/image/pwronoffani/poweronani.gif"
#define    PWRON_ANI_FRAME_COUNT                 (1)
#define    PWRON_ANI_RATE                        (5000)
#define    PWROFF_ANI_FILE                       "fs:/image/pwronoffani/poweroffani.gif"
#define    PWROFF_ANI_FRAME_COUNT                PWRON_ANI_FRAME_COUNT
#define    PWROFF_ANI_RATE                       PWRON_ANI_RATE
#elif defined FEATURE_VERSION_MYANMAR
#define    PWRON_ANI_FILE                        "fs:/image/pwronoffani/poweronani.gif"
#define    PWRON_ANI_FRAME_COUNT                 (1)
#define    PWRON_ANI_RATE                        (5000)
#define    PWROFF_ANI_FILE                       "fs:/image/pwronoffani/poweroffani.gif"
#define    PWROFF_ANI_FRAME_COUNT                PWRON_ANI_FRAME_COUNT
#define    PWROFF_ANI_RATE                       PWRON_ANI_RATE
#elif defined FEATURE_VERSION_N021
#define    PWRON_ANI_FILE                        "fs:/image/pwronoffani/poweronoff.gif"
#define    PWRON_ANI_FRAME_COUNT                 (1)
#define    PWRON_ANI_RATE                        (4000)
#define    PWROFF_ANI_FILE                       PWRON_ANI_FILE
#define    PWROFF_ANI_FRAME_COUNT                PWRON_ANI_FRAME_COUNT
#define    PWROFF_ANI_RATE                       PWRON_ANI_RATE
#elif defined FEATURE_VERSION_W0216A
#define    PWRON_ANI_FILE                        "fs:/image/pwronoffani/poweronoffani.gif"
#define    PWRON_ANI_FRAME_COUNT                 (1)
#define    PWRON_ANI_RATE                        (4000)
#define    PWROFF_ANI_FILE                       PWRON_ANI_FILE
#define    PWROFF_ANI_FRAME_COUNT                PWRON_ANI_FRAME_COUNT
#define    PWROFF_ANI_RATE                       PWRON_ANI_RATE

#elif defined FEATURE_VERSION_S1000
#define    PWRON_ANI_FILE                        "fs:/image/pwronoffani/poweron.gif"
#define    PWRON_ANI_FRAME_COUNT                 (1)
#define    PWRON_ANI_RATE                        (4000)
#define    PWROFF_ANI_FILE                       "fs:/image/pwronoffani/poweroff.gif"
#define    PWROFF_ANI_FRAME_COUNT                PWRON_ANI_FRAME_COUNT
#define    PWROFF_ANI_RATE                       PWRON_ANI_RATE
#elif defined FEATURE_VERSION_S106
#define    PWRON_ANI_FILE                        "fs:/image/pwronoffani/poweron.gif"
#define    PWRON_ANI_FRAME_COUNT                 (1)
#define    PWRON_ANI_RATE                        (4000)
#define    PWROFF_ANI_FILE                       "fs:/image/pwronoffani/poweroff.gif"
#define    PWROFF_ANI_FRAME_COUNT                PWRON_ANI_FRAME_COUNT
#define    PWROFF_ANI_RATE                       PWRON_ANI_RATE
#elif defined FEAUTRE_VERSION_N450
#define    PWRON_ANI_FILE                        "fs:/image/pwronoffani/poweron.gif"
#define    PWRON_ANI_FRAME_COUNT                 (1)
#define    PWRON_ANI_RATE                        (4000)
#define    PWROFF_ANI_FILE                       "fs:/image/pwronoffani/poweroff.gif"
#define    PWROFF_ANI_FRAME_COUNT                PWRON_ANI_FRAME_COUNT
#define    PWROFF_ANI_RATE                       PWRON_ANI_RATE
#else
#define    PWRON_ANI_FILE                        "fs:/image/pwronoffani/poweronoff.gif"
#define    PWRON_ANI_FRAME_COUNT                 (1)
#define    PWRON_ANI_RATE                        (5000)
#define    PWROFF_ANI_FILE                       PWRON_ANI_FILE
#define    PWROFF_ANI_FRAME_COUNT                PWRON_ANI_FRAME_COUNT
#define    PWROFF_ANI_RATE                       PWRON_ANI_RATE
#endif
#endif //#ifdef FEATURE_USES_LOWMEM

#define IDS_CORE_DISPADN        0xFFFE
#ifdef FEATURE_USES_BLACKBERRY
#define    IDLE_BLACKBERRY_ITEMMAX                 6
#ifdef FEATURE_DISP_160X128
#define    IDLE_BLACKBERRY_DRAWDX                  32
#define    IDLE_BLACKBERRY_DRAWDY                  32
#define    IDLE_BLACKBERRY_1                       "fs:/image/BlackBerry/messages.png"
#define    IDLE_BLACKBERRY_SEL_1                   "fs:/image/BlackBerry/messages_sel.png"
#define    IDLE_BLACKBERRY_2                       "fs:/image/BlackBerry/contact.png"
#define    IDLE_BLACKBERRY_SEL_2                   "fs:/image/BlackBerry/contact_sel.png"
#define    IDLE_BLACKBERRY_3                       "fs:/image/BlackBerry/alarm.png"
#define    IDLE_BLACKBERRY_SEL_3                   "fs:/image/BlackBerry/alarm_sel.png"
#define    IDLE_BLACKBERRY_4                       "fs:/image/BlackBerry/calendar.png"
#define    IDLE_BLACKBERRY_SEL_4                   "fs:/image/BlackBerry/calendar_sel.png"
#define    IDLE_BLACKBERRY_5                       "fs:/image/BlackBerry/mediaplayer.png"
#define    IDLE_BLACKBERRY_SEL_5                   "fs:/image/BlackBerry/mediaplayer_sel.png"
#define    IDLE_BLACKBERRY_6                       "fs:/image/BlackBerry/fmradio.png"
#define    IDLE_BLACKBERRY_SEL_6                   "fs:/image/BlackBerry/fmradio_sel.png"

#else
#define    IDLE_BLACKBERRY_DRAWDX                  32
#define    IDLE_BLACKBERRY_DRAWDY                  32
#define    IDLE_BLACKBERRY_1                       "fs:/image/BlackBerry/messages.png"
#define    IDLE_BLACKBERRY_SEL_1                   "fs:/image/BlackBerry/messages_sel.png"
#define    IDLE_BLACKBERRY_2                       "fs:/image/BlackBerry/contact.png"
#define    IDLE_BLACKBERRY_SEL_2                   "fs:/image/BlackBerry/contact_sel.png"
#define    IDLE_BLACKBERRY_3                       "fs:/image/BlackBerry/alarm.png"
#define    IDLE_BLACKBERRY_SEL_3                   "fs:/image/BlackBerry/alarm_sel.png"
#define    IDLE_BLACKBERRY_4                       "fs:/image/BlackBerry/calendar.png"
#define    IDLE_BLACKBERRY_SEL_4                   "fs:/image/BlackBerry/calendar_sel.png"
#define    IDLE_BLACKBERRY_5                       "fs:/image/BlackBerry/mediaplayer.png"
#define    IDLE_BLACKBERRY_SEL_5                   "fs:/image/BlackBerry/mediaplayer_sel.png"
#define    IDLE_BLACKBERRY_6                       "fs:/image/BlackBerry/fmradio.png"
#define    IDLE_BLACKBERRY_SEL_6                   "fs:/image/BlackBerry/fmradio_sel.png"

#endif
#endif
// ���б���Ϊ��ʱ����
#define PHONEPASSWORDLENTH      OEMNV_LOCKCODE_MAXLEN
#define LOCKUIMPASSWORDLENTH      OEMNV_LOCKUIM_MAXLEN
#define IDLE_ICON_HEIGHT        16

#define PINCODE_LENGTH          UIM_MAX_CHV_DIGITS
#define PUKCODE_LENGTH          UIM_MAX_CHV_DIGITS

#define MAX_STRING_LENGTH      64
#define MAX_BATTERY_POP_COUNT  3

#ifdef FEATURE_VERSION_MYANMAR
#define MUSIC_WIDTH            (SCREEN_HEIGHT-42) //88
#define MUSIC_START_WIDTH      (SCREEN_HEIGHT-40) //90
#else
#ifdef FEATURE_USES_BLACKBERRY
#define MUSIC_WIDTH            (SCREEN_HEIGHT-82) //78
#define MUSIC_START_WIDTH      (SCREEN_HEIGHT-80) //80
#else
#define MUSIC_WIDTH            (SCREEN_HEIGHT-52) //78
#define MUSIC_START_WIDTH      (SCREEN_HEIGHT-50) //80

#endif
#endif

#ifdef FEATURE_SPN_FROM_BSMCCMNC
#define OPERATOR_NAME_MAX_SIZE   32

typedef struct _ServiceProviderList
{
    word   mcc;
    byte   mnc;
    char  ids_name[OPERATOR_NAME_MAX_SIZE];
} ServiceProviderList;
#endif //FEATURE_SPN_FROM_BSMCCMNC

#define CORE_ERR(...)

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

#if (defined(FEATURE_PLANEMODE) || defined(FEATURE_OEMOMH))
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
    UIMERR_UNKNOW,       // δ֪״̬
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

typedef struct _CCoreApp
{
    // ���ṹ�ĵ�һ�����ݳ�Ա������ AEEApplet ��
    AEEApplet a ;
    
    // �ⲿ�ӿ�ָ��
    IDisplay            *m_pDisplay;
    IAnnunciator        *m_pIAnn;
    IConfig             *m_pConfig;
    IBattery            *m_pBatt;
    IRUIM               *m_pIRUIM;
#ifdef FEATURE_ICM
    ICM                 *m_pCM;
#else
    ITelephone          *m_pITelephone;
	ICallMgr            *m_pICallMgr;
	IPhoneCtl           *m_pIPhoneCtl;
#endif
    //IPhone       *m_pPhone;
    
    // ��ǰ��Ի���IDialog�ӿ�ָ��
    IDialog             *m_pActiveDlg;
    
    // ��ǰ��Ի�����¼�������ָ��
    PFNDLGHANDLER       m_pDialogHandler;

    boolean             m_bSuspended; 
    
    boolean             m_bActive;
    
    boolean             m_pAppStart;

    boolean             m_bBatteryActive;
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
    AECHAR              m_wPIN[PINCODE_LENGTH + 1];
    AECHAR              m_wPIN2[PINCODE_LENGTH + 1];
    AECHAR              m_wPUK[PUKCODE_LENGTH + 1];
    
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
    boolean             m_bChargFull;

    IALERT              *m_pAlert;           //IALERTָ��
#ifndef FEATURE_USES_LOWMEM
    //��������ͼƬ
    IImage             *m_pStartupAniImg;
#else
#endif
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
	boolean             m_battery_msgup;
#ifdef FEATURE_UTK2
    uint16     wRefreshMask;
#endif
#ifndef FEATURE_USES_LOWMEM
    IImage *            m_battery_Image;
#endif
    byte                m_battery_count;
#ifdef FEATURE_KEYGUARD
    boolean             m_b_set_lock;
#endif
#ifdef FEATRUE_SET_ANN_FULL_SCREEN
    db_capture_type     m_capture;
#endif
    boolean             m_bProvisioned; 
    boolean             m_bConfigSent;
    AECHAR             *m_cdg_msgptr;

#if defined( FEATURE_POWERDOWN_ALARM)
	boolean             powerupByAlarm;
#endif
    boolean             bunlockuim;

#ifdef FEATURE_PLANEMODE
    boolean             bPlaneModeOn;
#endif
#ifdef FEATURE_APP_MUSICPLAYER
    uint16              m_nAutoScrollIdx;
    AECHAR              *m_pMusicName;
#endif

#ifdef FEATURE_TORCH_SUPPORT
	boolean TorchOn;
#endif

    ICard *             m_pICard;               /* pointer to icard interface */
    uint8               m_nCardStatus;
    /***********************************
          For Security Module
    **********************************/
    AEECallback         m_sCallback;
    AEECardPinCode      m_sPIN;
    AEECardPinCode      m_sPIN2;
    AEECardPinCode      m_sPUK;
    AEECardPinCmdStatus m_sPinActionStatus;
    boolean             m_bVerifying;
	boolean             m_isShift;
	#ifdef FEATURE_USES_BLACKBERRY
	uint8               m_CurMainItems;    //��ǰ���˵�ITEMS
	IImage              *m_pImageIcon[IDLE_BLACKBERRY_ITEMMAX];
	IImage              *m_pImageSelIcon[IDLE_BLACKBERRY_ITEMMAX];
	
	#endif
	//wangliang add!
	byte                m_active;
	byte                m_CurProfile;      //��ǰ�龰ģʽ
	#ifdef FEATURE_KEYGUARD
	boolean             m_iskeypadtime;
	#endif
	boolean             m_IsSametime;
    AECHAR             *m_pADNName;
    AECHAR             *m_pADNNumber;
#ifdef FEATURE_OEMOMH 
    AEERect            timeRc;
#endif
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
#ifdef FEATURE_ICM
boolean CoreApp_IsEmergencyMode(ICM* pICM);
#else
boolean CoreApp_IsEmergencyMode(ITelephone* pICM);
#endif
void CoreApp_UpdateAnnunciator(CCoreApp *pMe);

extern AEE_EXPORTS IShell    *AEE_GetShell(void);

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
void CoreApp_load_uim_esn(CCoreApp *pMe);
int CoreApp_GetBatteryLevel(CCoreApp *pMe);
#ifdef FEATURE_SEAMLESS_SMS
void CoreApp_SendSeamlessSMSTimer(void *pme);
int  CoreApp_SendSeamlessSMS(CCoreApp *pMe);
#endif //#ifdef FEATURE_SEAMLESS_SMS
#ifdef FEATURE_ICM
void InitAfterPhInfo(CCoreApp *pMe, AEECMOprtMode mode);
#else
void InitAfterPhInfo(CCoreApp *pMe, AEETOprtMode mode);
#endif
void CoreApp_ProcessSubscriptionStatus (CCoreApp *pMe);
#endif
