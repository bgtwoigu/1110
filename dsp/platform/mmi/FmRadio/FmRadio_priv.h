#ifndef __FM_RADIO_RIV_H
#define __FM_RADIO_RIV_H

/*==============================================================================
// �ļ���
//        fmRadio_priv.h
//        2007-11-11 Gemsea����汾(Draft Version)
//        ��Ȩ����(c) 2007 Anylook.,LTD.
//
// �ļ�˵����
//
// ���ߣ�2007-11-11
// �������ڣ�Gemsea
// ��ǰ�汾��Draft
==============================================================================*/

/*==============================================================================
�޸���ʷ��¼��
ʱ       ��      �� �� ��     �޸����ݡ�λ�ü�ԭ��
-----------      ----------     -----------------------------------------------
2007-11-11              ��ʼ�汾
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
   #include "FmRadio.bid"
#else
   #include "OEMClassIds.h"  // Applet Ids
   #ifndef AEECLSID_APP_FMRADIO
      #error AEECLSID_APP_FMRADIO must be defined
   #endif
#endif

#include "fmradiols.brh"
#include "fmradioln.brh"
#include "AEEShell.h"
#include "AEEStdLib.h"
#include "AEEModGen.h"
#include "AEEAppGen.h"
#include "FmRadio.h"
#ifndef WIN32
#include "db.h"
#else
#include "oemhelperfunctype.h"
#endif//WIN32
#include "AEEMenu.h"
#include "AEEConfig.h"
#include "AEEText.h"
#include "OEMCFGI.h"
#include "AEECM.h"
#include "OEMSMS.h"
#include "AEEText.h"
#include "AEEAnnunciator.h"

#if !defined( AEE_SIMULATOR)
#include "fm_radio.h"
#include "err.h"
#endif //#if !defined( AEE_SIMULATOR)


#define debug(...)

#define FEATURE_DIRECT_INPUT_CHANNEL_NUMBER                     1
#define FEATURE_TEST_VERSION_WITHOUT_HEADSET_PRESENCE_VERIFY    0
#define FEATURE_FMRADIO_TEST_VERSION
//#define FEATURE_FMRADIO_SIMPLE_VERSION							1
#if defined( FEATURE_Q1_SPECIAL_TARGET) || defined( FEATURE_Q2_SPECIAL_TARGET) || defined( FEATURE_CS09) || defined(FEATURE_FMRADIO_TEST_VERSION)
#define FEATURE_FMRADIO_NO_MODE_SELECT
#define FEATURE_FMRADIO_KEY_OK_TO_MUTE
#define FEATURE_FMRADIO_PROMPT_HEADSET_PLUG_OUT
#endif 

#define FEATURE_FMRADIO_SUPPORT_BACKGROUND  0

#define FEATURE_FMRADIO_CHANNEL_LIST_SUPPORT 1
#define COREAPP_PATH "fs:/mod/coreapp/"
#include "coreapp.brh"
#define WMSAPP_RES_FILE ("fs:/mod/wms/" AEE_RES_LANGDIR "wms.bar")
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
   //Add Value here
   //����״̬��ѡ��Option
   DLGRET_OPTION_SELECT,
   //��Option״̬��ѡ��List
   DLGRET_OPTION_LIST,

   DLGRET_CANCELED

} DLGRet_Value_e_Type;

/*----------------------״̬�����������������---------------------*/
// FmRadio Applet ״̬��״̬��
typedef enum _FmRadioState
{
   FMRADIOST_NONE,
   FMRADIOST_INIT,
   FMRADIOST_MAIN,
   FMRADIOST_OPTION,
   FMRADIOST_LIST,
   FMRADIOST_USER_MSG, 
   //Add State here
   FMRADIOST_EXIT
} FmRadioState;

// ״̬���������ظ�״̬������������ֵ����
typedef enum NextFSMAction
{
   NFSMACTION_WAIT,
   NFSMACTION_CONTINUE
} NextFSMAction;

// ����ģ����Ϣ��ģ�����ü����Ľṹ
typedef struct FmRadioMod
{
    DECLARE_VTBL(IModule)
    uint32   m_nRefs;
} FmRadioMod;

/*==============================================================================
                          ��FM RADIO������ص����ݺͶ���
==============================================================================*/
#define LOWEST_BAND_FREQ 87500
#define UPPEST_BAND_FREQ 108000
#if defined FEATURE_FM_SPACE_50KHZ
#define FM_MAX_CHANNEL  410
#define CHANNEL_SPACE 	50
#else
#define FM_MAX_CHANNEL  205
#define CHANNEL_SPACE 	100
#endif 
//�������Ի����µ����ֹ���ģʽ�����ź͵�Ƶ
typedef enum _OpMode
{
    FM_RADIO_OPMODE_PLAY,
    FM_RADIO_OPMODE_AUTO_TUNE,
#if FEATURE_FMRADIO_CHANNEL_LIST_SUPPORT
    FM_RADIO_OPMODE_REFRESH_CHANNEL_LIST_CONFIRM,
    FM_RADIO_OPMODE_REFRESH_CHANNEL_LIST,
#endif
    FM_RADIO_OPMODE_MODE_SELECTION,
    FM_RADIO_OPMODE_OPTION_SELECTION,
#if FEATURE_FMRADIO_CHANNEL_LIST_SUPPORT
    FM_RADIO_OPMODE_EDIT_CHANNEL_LIST,
    FM_RADIO_OPMODE_EDIT_CHANNEL_LIST_OPTION_SELECTION,
    FM_RADIO_OPMODE_EDIT_CHANNEL,
#endif

    FM_RADIO_OPMODE_USER_MSG,
#if FEATURE_FMRADIO_CHANNEL_LIST_SUPPORT
    FM_RADIO_OPMODE_CHANNEL_LIST_DELETE,
    FM_RADIO_OPMODE_CHANNEL_LIST_DELETE_ALL,
    FM_RADIO_OPMODE_PRMOPT_CHANNEL_LIST_IS_EMPTY,
#endif
   #if FEATURE_DIRECT_INPUT_CHANNEL_NUMBER
   FM_RADIO_OPMODE_DIRECT_INPUT_CHANNEL
   #endif
} OpModeTypeEnum;

typedef enum
{
#if FEATURE_FMRADIO_CHANNEL_LIST_SUPPORT
    FM_RADIO_TUNNING_MODE_LIST = 0,
    FM_RADIO_TUNNING_MODE_MANUAL,
    FM_RADIO_TUNNING_MODE_AUTO,
    FM_RADIO_TUNNING_MODE_LAST,
    FM_RADIO_TUNNING_MODE_NUMBER = FM_RADIO_TUNNING_MODE_LAST
#else
    FM_RADIO_TUNNING_MODE_MANUAL = 0,
    FM_RADIO_TUNNING_MODE_AUTO,
    FM_RADIO_TUNNING_MODE_LAST,
    FM_RADIO_TUNNING_MODE_NUMBER = FM_RADIO_TUNNING_MODE_LAST
#endif
    
} TuningModeEnum;

typedef enum
{
    FM_RADIO_LED_LIGHT_PLAYING = 0,
    FM_RADIO_LED_LIGHT_IDLE,
    FM_RADIO_LED_LIGHT_SEARCHING
} LEDLightTypeEnum;

typedef enum
{
    FM_RADIO_REFUSE_REASON_NOT_REFUSE = -1,
    FM_RADIO_REFUSE_REASON_VOICE_CALL_CONNECTED,
    FM_RADIO_REFUSE_REASON_NO_HEADSET,
    FM_RADIO_REFUSE_REASON_HEADSET_PLUG_OUT
} RefuseReasonEnum;


#if 0
typedef struct _sChanListNode sChanListNode;
struct _sChanListNode
{
    sChanListNode* pPreNode;
    sChanInfo       chanInfo;
    sChanListNode* pNextNode;
};
#endif

typedef struct
{
    TuningModeEnum  tuningMode;
    uint16          channel;
} FmRadioCfg;

typedef enum
{
    FM_OPTION_EDIT_BACK = 0,
    FM_SAVE_BACK,
    FM_SAVE_DELETE
} fm_channellist_bottom_type;

// FmRadio Applet����ṹ�壺
typedef struct _CFmRadio
{
    DECLARE_VTBL(IFmRadio)

    uint32           m_nRefs;               // �������ü�����
    IModule         *m_pModule;

    IDisplay        *m_pDisplay;
    IShell          *m_pShell;
    AEERect          m_rc;
    IConfig         *m_pConfig;
    IAnnunciator        *m_pIAnn;

    IDialog         *m_pActiveDlg;          // ��ǰ��Ի���IDialog�ӿ�ָ��
    uint16           m_pActiveDlgID;        // ��ǰ��Ի���ID
    DLGRet_Value_e_Type   m_eDlgRet;        // ��Ի���ر�ʱ�ķ��ؽ��
    boolean          m_bNotOverwriteDlgRet; // �Ƿ񲻸�д�Ի��򷵻ؽ����һ�������Ҫ��д(FALSE)��
    FmRadioState     m_ePreState;           // Applet ǰһ״̬
    FmRadioState     m_eCurState;           // Applet ��ǰ״̬

    boolean          m_bSuspending;         // Applet�Ƿ��ڹ���״̬
    boolean          m_bAppIsReady;         // Applet�Ƿ�׼���ÿ��Դ������������¼����ı�����Ҫ���ڿ��ٰ������������

    // change them from global to instanced.
    // @08.01.22
//    boolean bIsFmHardwareInit;
    byte byVolumeLevel;
    byte byChannelMax;

    OpModeTypeEnum opMode;


    sChanInfo		chanInfoList[MAX_FMRADIO_STORED_CHANNEL];
    int				enumCursor;
    int				enumCursorWithLoop;

    LEDLightTypeEnum    ledLightType;
    RefuseReasonEnum    refuseReason;
    boolean             drawRefreshListPrompt;
    boolean             seekChannelClockwise;
    boolean             globalSearching;
#if FEATURE_FMRADIO_SUPPORT_BACKGROUND 
    boolean             runOnBackground;
#endif
    boolean             startFromBackground;
    boolean             channelListIsEmptyHasPrompted;
    boolean             channelListIsEmpty;
    int                 demitintColor;
    int                 channelListMenuSelectedItemId;
    int                 channelListMenuSelectedItemIndex;
    int                 currentChannelListNodeIndex;

    AECHAR              originalChannelNumber[16];

    FmRadioCfg          cfg;
//    AEECallback         callback;

    IStatic             *pStatic;
    ITextCtl            *pText;
    IMenuCtl            *m_pMenu;


   fm_channellist_bottom_type   m_channellist_bottom_type;

    #if FEATURE_DIRECT_INPUT_CHANNEL_NUMBER
    AECHAR directInputChannel[8];
    #endif

    boolean alarmNow;
    uint32  wmsEventTimestamp;
    uint16  wmsEventTimer;
    boolean newSmsIncoming;
    uint16  newSmsIncomingWparam;

#if defined( FEATURE_FMRADIO_NO_MODE_SELECT)
    boolean tuneVolumeByLeftRightArrowKey;
#endif
    byte    keyBeepVol;
    boolean key_to_mute;
    AECHAR  EditChannel[8];
    uint16  CurrentItemId;
    boolean edit_chann_interrupt;
    IImage     *m_pVolumeImage;
} CFmRadio;

/*==============================================================================
                                 �궨��ͳ���
==============================================================================*/
// Ϊ��ֹ�û����ٰ���������¼�EVT_APPISREADY���¼��ɶ�ʱ��
// FmRadio_APPIsReadyTimer����
#define APPISREADY_TIMER       100           

#if defined(FEATURE_DISP_128X128)
//��ǰ�����ַ�����ʾyƫ��
#define FMRADIO_OPERATION_YOFFSET   	14
//Ƶ��ͼ����ʾ����xƫ��
#define FMRADIO_CHANNEL_XOFFSET      	12//30//12
//Ƶ��ͼ����ʾ����yƫ��
#define FMRADIO_CHANNEL_YOFFSET      	46//52
//Ƶ���ַ���ʾyƫ��
#define FMRADIO_CHANNEL_FREQ_YOFFSET 	10//97
//Ƶ��ָ��߶�
#define FMRADIO_CURSOR_DY           	4
//������ʾ����xƫ��
#define FMRADIO_VOLUME_XOFFSET        	30//49
//������ʾ���򳤶�
#define FMRADIO_VOLUME_LENGTH         	60//93
//������ʾ����yƫ��
#define FMRADIO_VOLUME_YOFFSET        	40//94//113
//���������С
#define FMRADIO_VOLUME_BLOCK_SIZE   	4//5
//״ָ̬ʾ�ƴ�С
#define FMRADIO_LED_LIGHT_SIZE          12
#elif defined(FEATURE_DISP_160X128)
//��ǰ�����ַ�����ʾyƫ��
#define FMRADIO_OPERATION_YOFFSET   	14
//Ƶ��ͼ����ʾ����xƫ��
#define FMRADIO_CHANNEL_XOFFSET      	12//30//12
//Ƶ��ͼ����ʾ����yƫ��
#define FMRADIO_CHANNEL_YOFFSET      	46//52
//Ƶ���ַ���ʾyƫ��
#define FMRADIO_CHANNEL_FREQ_YOFFSET 	10//97
//Ƶ��ָ��߶�
#define FMRADIO_CURSOR_DY           	4
//������ʾ����xƫ��
#define FMRADIO_VOLUME_XOFFSET        	30//49
//������ʾ���򳤶�
#define FMRADIO_VOLUME_LENGTH         	60//93
//������ʾ����yƫ��
#define FMRADIO_VOLUME_YOFFSET        	40//94//113
//���������С
#define FMRADIO_VOLUME_BLOCK_SIZE   	4//5
//״ָ̬ʾ�ƴ�С
#define FMRADIO_LED_LIGHT_SIZE          12

#elif defined(FEATURE_DISP_220X176)
//��ǰ�����ַ�����ʾyƫ��
#define FMRADIO_OPERATION_YOFFSET   	14
//Ƶ��ͼ����ʾ����xƫ��
#define FMRADIO_CHANNEL_XOFFSET      	12//30//12
//Ƶ��ͼ����ʾ����yƫ��
#define FMRADIO_CHANNEL_YOFFSET      	46//52
//Ƶ���ַ���ʾyƫ��
#define FMRADIO_CHANNEL_FREQ_YOFFSET 	10//97
//Ƶ��ָ��߶�
#define FMRADIO_CURSOR_DY           	4
//������ʾ����xƫ��
#define FMRADIO_VOLUME_XOFFSET        	30//49
//������ʾ���򳤶�
#define FMRADIO_VOLUME_LENGTH         	60//93
//������ʾ����yƫ��
#define FMRADIO_VOLUME_YOFFSET        	40//94//113
//���������С
#define FMRADIO_VOLUME_BLOCK_SIZE   	4//5
//״ָ̬ʾ�ƴ�С
#define FMRADIO_LED_LIGHT_SIZE          12


#elif defined(FEATURE_DISP_128X160)
//��ǰ�����ַ�����ʾyƫ��
#define FMRADIO_OPERATION_YOFFSET   	14
//Ƶ��ͼ����ʾ����xƫ��
#define FMRADIO_CHANNEL_XOFFSET      	12//30//12
//Ƶ��ͼ����ʾ����yƫ��
#define FMRADIO_CHANNEL_YOFFSET      	46//52
//Ƶ���ַ���ʾyƫ��
#define FMRADIO_CHANNEL_FREQ_YOFFSET 	10//97
//Ƶ��ָ��߶�
#define FMRADIO_CURSOR_DY           	4
//������ʾ����xƫ��
#define FMRADIO_VOLUME_XOFFSET        	30//49
//������ʾ���򳤶�
#define FMRADIO_VOLUME_LENGTH         	60//93
//������ʾ����yƫ��
#define FMRADIO_VOLUME_YOFFSET        	40//94//113
//���������С
#define FMRADIO_VOLUME_BLOCK_SIZE   	4//5
//״ָ̬ʾ�ƴ�С
#define FMRADIO_LED_LIGHT_SIZE          12

#elif defined(FEATURE_DISP_176X220)
//��ǰ�����ַ�����ʾyƫ��
#define FMRADIO_OPERATION_YOFFSET   	14
//Ƶ��ͼ����ʾ����xƫ��
#define FMRADIO_CHANNEL_XOFFSET      	12//30//12
//Ƶ��ͼ����ʾ����yƫ��
#define FMRADIO_CHANNEL_YOFFSET      	46//52
//Ƶ���ַ���ʾyƫ��
#define FMRADIO_CHANNEL_FREQ_YOFFSET 	10//97
//Ƶ��ָ��߶�
#define FMRADIO_CURSOR_DY           	4
//������ʾ����xƫ��
#define FMRADIO_VOLUME_XOFFSET        	30//49
//������ʾ���򳤶�
#define FMRADIO_VOLUME_LENGTH         	60//93
//������ʾ����yƫ��
#define FMRADIO_VOLUME_YOFFSET        	40//94//113
//���������С
#define FMRADIO_VOLUME_BLOCK_SIZE   	4//5
//״ָ̬ʾ�ƴ�С
#define FMRADIO_LED_LIGHT_SIZE          12

#elif defined(FEATURE_DISP_240X320)
//��ǰ�����ַ�����ʾyƫ��
#define FMRADIO_OPERATION_YOFFSET   	14
//Ƶ��ͼ����ʾ����xƫ��
#define FMRADIO_CHANNEL_XOFFSET      	12//30//12
//Ƶ��ͼ����ʾ����yƫ��
#define FMRADIO_CHANNEL_YOFFSET      	46//52
//Ƶ���ַ���ʾyƫ��
#define FMRADIO_CHANNEL_FREQ_YOFFSET 	10//97
//Ƶ��ָ��߶�
#define FMRADIO_CURSOR_DY           	4
//������ʾ����xƫ��
#define FMRADIO_VOLUME_XOFFSET        	30//49
//������ʾ���򳤶�
#define FMRADIO_VOLUME_LENGTH         	60//93
//������ʾ����yƫ��
#define FMRADIO_VOLUME_YOFFSET        	40//94//113
//���������С
#define FMRADIO_VOLUME_BLOCK_SIZE   	4//5
//״ָ̬ʾ�ƴ�С
#define FMRADIO_LED_LIGHT_SIZE          12

#elif defined(FEATURE_DISP_320X240)
//��ǰ�����ַ�����ʾyƫ��
#define FMRADIO_OPERATION_YOFFSET   	28
//Ƶ��ͼ����ʾ����xƫ��
#define FMRADIO_CHANNEL_XOFFSET      	24//30//12
//Ƶ��ͼ����ʾ����yƫ��
#define FMRADIO_CHANNEL_YOFFSET      	18//52
//Ƶ���ַ���ʾyƫ��
#define FMRADIO_CHANNEL_FREQ_YOFFSET 	18//97
//Ƶ��ָ��߶�
#define FMRADIO_CURSOR_DY           	4
//������ʾ����xƫ��
#define FMRADIO_VOLUME_XOFFSET        	30//49
//������ʾ���򳤶�
#define FMRADIO_VOLUME_LENGTH         	60//93
//������ʾ����yƫ��
#define FMRADIO_VOLUME_YOFFSET        	40//94//113
//���������С
#define FMRADIO_VOLUME_BLOCK_SIZE   	4//5
//״ָ̬ʾ�ƴ�С
#define FMRADIO_LED_LIGHT_SIZE          12


#else
//��ǰ�����ַ�����ʾyƫ��
#define FMRADIO_OPERATION_YOFFSET   	14
//Ƶ��ͼ����ʾ����xƫ��
#define FMRADIO_CHANNEL_XOFFSET      	12//30//12
//Ƶ��ͼ����ʾ����yƫ��
#define FMRADIO_CHANNEL_YOFFSET      	46//46//52
//Ƶ���ַ���ʾyƫ��
#define FMRADIO_CHANNEL_FREQ_YOFFSET 	10//77//97
//Ƶ��ָ��߶�
#define FMRADIO_CURSOR_DY           	4
//������ʾ����xƫ��
#define FMRADIO_VOLUME_XOFFSET        	30//49
//������ʾ���򳤶�
#define FMRADIO_VOLUME_LENGTH         	60//93
//������ʾ����yƫ��
#define FMRADIO_VOLUME_YOFFSET        	70//94//113
//���������С
#define FMRADIO_VOLUME_BLOCK_SIZE   	4//5
//״ָ̬ʾ�ƴ�С
#define FMRADIO_LED_LIGHT_SIZE          12
#endif

// ���� BREW 3 ����Ҫ���ض�����Դ�ļ���
#define  FMRADIOLS_RES_FILE_LANG (AEE_RES_LANGDIR FMRADIOLS_RES_FILE)

// ״̬�ƶ��꣺�Ƚ�ǰһ״̬�õ�ǰ״̬���£��ٽ���ǰ״̬��ΪnextState
#define MOVE_TO_STATE(nextState) {            \
            FmRadioState tpState;            \
            tpState = nextState;          \
            pMe->m_ePreState = pMe->m_eCurState; \
            pMe->m_eCurState = tpState;        \
        }

/*==============================================================================
                                 ȫ������
==============================================================================*/

/*==============================================================================
                                 ���أ���̬������
==============================================================================*/
//extern byte byVolumeLevel;
//
//extern byte iChannelMax;
//
//extern sChanInfo curChanInfo;
//
//extern sChanListNode* pChanListRoot;
//extern sChanListNode* pCurChanNode;
//
//extern OpModeType opMode;
//
//extern boolean bIsNodeChanged;       //�Ƿ��޸��˵�ǰ�ŵ�������
//
//extern boolean bIsListChanged;       //�Ƿ��޸����б������

/*==============================================================================
                                 ��������
==============================================================================*/
/*----------------------�Ի�����غ�������---------------------*/
/*==============================================================================
����:
       FmRadio_ShowDialog
˵��:
       ������״̬����������������Ӧ�Ի�����ԴID�ĶԻ���

����:
       pMe [In]: ָ��FmRadio Applet����ṹ��ָ��,�ýṹ����С������ض���Ϣ.
       dlgResId [in]���Ի�����ԴID���������ݸ�ID�����Ի���

����ֵ:
       ��

��ע:


==============================================================================*/
void FmRadio_ShowDialog(CFmRadio *pMe,uint16  dlgResId);

/*==============================================================================
����:
       FmRadio_RouteDialogEvent
˵��:
       ������BREW�¼�·�ɸ���ǰ��Ի�����¼���������

����:
       pMe [In]: ָ��FmRadio Applet����ṹ��ָ��,�ýṹ����С������ض���Ϣ.
       eCode [in]���¼����롣
       wParam [in]�����¼�eCode���������ݡ�
       dwParam [in]�����¼�eCode���������ݡ�

����ֵ:
       TRUE�������¼�������
       FALSE�������¼�û������

��ע:

==============================================================================*/
boolean FmRadio_RouteDialogEvent(CFmRadio *pMe,
    AEEEvent    eCode,
    uint16  wParam,
    uint32 dwParam
);

/*----------------------״̬����غ�������---------------------*/
/*==============================================================================
����:
       FmRadio_ProcessState
˵��:
       FmRadio Applet״̬��������������������pMe->m_currState������·����
       ��Ӧ�Ĵ�������

����:
       pMe [In]: ָ��FmRadio Applet����ṹ��ָ��,�ýṹ����С������ض���Ϣ.

����ֵ:
       NFSMACTION_CONTINUE��ָʾ������״̬��״̬������ֹͣ��
       NFSMACTION_WAIT��ָʾ��Ҫ��ʾ�Ի��������û���Ӧ����״̬����

��ע:

==============================================================================*/
NextFSMAction FmRadio_ProcessState(CFmRadio *pMe);

/*==============================================================================
����:
       FmRadio_AddNodeToTail
˵��:
       ��ָ����CHANNEL����Channel List�����

����:
       pChInfo [In]: ָ��sChanInfo�ṹ��ָ��.

����ֵ:
       TRUE: �ɹ�
       FALSE: ʧ��

��ע:

==============================================================================*/
//boolean FmRadio_AddNodeToTail(sChanInfo* pChInfo);

/*==============================================================================
����:
       FmRadio_AddChanListNode
˵��:
       ��ָ����CHANNEL����Channel List, ��������ָ��
       Node�ĺ���.

����:
       ppNode [IO]: ָ��sChanListNode�ṹָ���ָ��.
       pChInfo [In]: ָ��sChanInfo�ṹ��ָ��.

����ֵ:
       TRUE: �ɹ�
       FALSE: ʧ��

��ע:

==============================================================================*/
boolean FmRadio_AddChanListNode( sChanInfo* pChInfo);

/*==============================================================================
����:
       FmRadio_DeleteChanListNode
˵��:
       ��ָ����CHANNEL��Channel Listɾ��

����:
       pNode [In]: ָ��sChanListNode�ṹ��ָ��.

����ֵ:
       TRUE: �ɹ�
       FALSE: ʧ��

��ע:

==============================================================================*/
#if 0
boolean FmRadio_DeleteChanListNode(sChanListNode* pNode);
#endif
/*==============================================================================
����:
       FmRadio_SeekStatusTimerCB

˵��:
       ��ʱ���ص������������첽Seek���״̬��

����:
       pme [in]��void *����ָ�룬��ָCFmRadio�ṹָ�롣

����ֵ:
       �ޡ�

��ע:

==============================================================================*/
void FmRadio_SeekStatusTimerCB(void *pme);

/*==============================================================================
����:
       FmRadio_AutoSearchTimerCB

˵��:
       �Զ�����ȫƵ�Σ�����Ч�ĵ�̨���浽Channel List��

����:
       pme [in]��void *����ָ�룬��ָCFmRadio�ṹָ�롣

����ֵ:
       �ޡ�

��ע:

==============================================================================*/
void FmRadio_AutoSearchTimerCB(void *pme);

/*==============================================================================
����:
       FmRadio_WarningDisplayTimerCB

˵��:
       ��ʾ������Ϣ���ȴ�ʱ�䳬ʱ��Ļص�������

����:
       pme [in]��void *����ָ�룬��ָCFmRadio�ṹָ�롣

����ֵ:
       �ޡ�

��ע:

==============================================================================*/
void FmRadio_WarningDisplayTimerCB(void *pme);

/*==============================================================================
����:
       FmRadio_DisplayWarningMessage

˵��:
       ��ʾ������Ϣ

����:
       pme [in]��void *����ָ�룬��ָCFmRadio�ṹָ�롣

����ֵ:
       �ޡ�

��ע:

==============================================================================*/
void FmRadio_DisplayWarningMessage(void *pme, AECHAR* pSzMessage);

/*==============================================================================
����:
       FmRadio_SaveChannelList

˵��:
       ��Ƶ���б����NV, �����ǰ�б��е�Ƶ������
       MAX_FMRADIO_STORED_CHANNEL����ᱻ������

����:
       pme [in]��void *����ָ�룬��ָCFmRadio�ṹָ�롣

����ֵ:
       �ޡ�

��ע:

==============================================================================*/
void FmRadio_SaveChannelList( CFmRadio* pme);

/*==============================================================================
����:
       FmRadio_FindChanListNode

˵��:
       �����Ƿ��Ѿ����ڸ��б�
����:
       pNode [in]��sChanListNode *����ָ�롣

����ֵ:
       TRUE:  �Ѵ���
       FALSE: ������

��ע:

==============================================================================*/
boolean FmRadio_FindChanListNode(int channel);


/*==============================================================================
����:
       closeApp

˵��:
       �ر�App
����:
       pMe [in]��

����ֵ:

��ע:

==============================================================================*/

// bw: 20080808 : close app when pull out the headset
void closeApp( void *pme); //bw:

int  FmRadio_ShowMsgBoxDialog( CFmRadio *pMe, 
                               char*       pszResFile,
                               uint16      msgResId,
                               uint16      dlgResId,                               
                               AECHAR     *msg, 
                               int         msglen);
#endif
//__FM_RADIO_RIV_H

