/*==============================================================================
//           U S E R   I N T E R F A C E   M A I N  M O D U L E
// �ļ���
//        brewui.c
//        ��Ȩ����(c) 2009 Anylook Tech. CO.,LTD.
//        
// �ļ�˵����
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
#include "oemui.h"
#include "AEE_OEM.h"
#include "AEE_OEMEvent.h"
#include "err.h"
#include "OEMFeatures.h"
#include "AEE.h"
#include "AEEShell.h"
#include "hsui.h"
#include "lcd.h"
#include "AEEBacklight.h"
#include "AEEAlert.h"
//#include "AEEPhone.h"
#ifdef  FEATURE_UIM_RUIM
#include "nvruimi.h"
#endif
#include "OEMNV.h"
#ifdef CUST_EDITION
#include "uimtk.h"
#include "ftm.h"
#else
#include "ftm_enum.h"
#endif
#ifndef AEE_SIMULATOR
#include "keypad.h"
#endif
#include "uixsnd.h"
#include "fs_public.h"
#include "OEMClassIDs.h"
#ifdef  FEATURE_CDSMS_CACHE_USELIST
#include "OEMWMS_CacheInfo.h"
#endif
#include "prl.h"
#include "hw.h"
#include "AEECM.h"
#include "OEMCFGI.h"
#include "OEMConfig.h"
#include "OEMFS.h"
#ifdef FEATURE_UTK2
#include "UTKUtils.h"
#endif //FEATURE_UTK2
#include "OEMRTC.h"
#include "AEECM.h"
#ifdef FEATURE_KEYGUARD
#include "OEMKeyguard.h"
#endif
#include "diag.h"      /* Interface to DIAG task */

#ifdef FEATURE_SUPPORT_BT_APP
#include "bt_ui_app.h"
#endif
#ifdef FEATURE_APP_MUSICPLAYER
#include "MusicPlayer.h"
#endif 
#include "oemdevicenotifier.h"
#ifdef FEATURE_SUPPORT_VC0848
#include "Mediagallery.h"
#endif

#ifdef FEATURE_TOUCHPAD
#include "touchpad.h"
#include "OEMPointerHelpers.h"
#endif

/*==============================================================================
                                 
                                 �궨��ͳ���
                                 
==============================================================================*/
#ifdef FEATURE_MULTIMODE_ARCH
#define SLEEP_BETWEEN_TMC_CHECKS_MS         75
#endif

// �������������¼���Ҫ�������ְ���״̬��ʱ�� (����)
#define KEY_HELD_TIME           1250

// ������Դ���������¼��谴�����ְ���״̬��ʱ�� (����)
//#define KEY_HELD_PWR_TIME       3000

// ����ɾ����Դ�ļ�ʱ�����ڲ�����ļ�϶ (����)
#define SLEEP_AFTER_RESOURCE_FILE_DELETE_MS     50

// oemui task �������Ԥ���������
#define UI_CMD_BUFFERS  10

// Ring buffer for holding keycodes.  When read index == write index, it is
// empty.  When read index is 1 ahead of write index, the buffer is full.
// Note:  this "burns" 1 datum (a byte) in order to tell full from empty.
#define UI_KEY_BUF_SIZE         32      /* Max keycodes to buffer    */
#define UI_KEY_BUF_MASK         0x1f    /* Mask for modulus in ring  */

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
/* Constants and Masks for commands and responses related to  R-UIM        */
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

#ifdef FEATURE_UIM_RUIM
#define RUIM_ESN_USAGE_MASK        0x01
#define RUIM_STORE_ESN_LENGTH      0x04
#define RUIM_STORE_ESN_USE_UIM_ID  0x10

#define RUIM_CHV1_STATUS_OFFSET    13
#define RUIM_CHV2_STATUS_OFFSET    20
#define RUIM_CHV_STATUS_MASK       0x80

#define RUIM_SELECT_RSP_NUM_BYTES  22
#endif /* FEATURE_UIM_RUIM */

#ifdef FEATURE_SLEEP
#define UI_ENABLE_SLEEP()                                       \
{                                                               \
    if (!brewui_ok_to_sleep)                                    \
    {                                                           \
        (void)rex_set_sigs(&sleep_tcb, SLEEP_UI_OKTS_SIG);      \
        MSG_LOW( "UI Sleep enable", 0, 0, 0 );                  \
        brewui_ok_to_sleep = TRUE;                              \
    }                                                           \
}

#define UI_DISABLE_SLEEP()                                      \
{                                                               \
    if (brewui_ok_to_sleep)                                     \
    {                                                           \
        (void)rex_clr_sigs(&sleep_tcb, SLEEP_UI_OKTS_SIG);      \
        MSG_LOW( "UI Sleep disable", 0, 0, 0 );                 \
        brewui_ok_to_sleep = FALSE;                             \
    }                                                           \
}
#endif 


#define KEY(h,a,c,b)    {HS_##h##_K, AVK_##a, KC_##c, b}

// Provisioning defaults 
// FM Min is min #0, CDMA Min is min #1 
#define FMMIN 0
#define CDMAMIN 1

#ifdef FEATURE_CDMA_800
// Primary Paging Channel Defaults
#define PCH_A_DEF 283    // Primary Channel A Default
#define PCH_B_DEF 384    // Primary Channel B Default

// Secondary Paging Channel Defaults
#define SCH_A_DEF 691    // Secondary Channel A Default 
#define SCH_B_DEF 777    // Secondary Channel B Default 

// Primary Paging Channel Defaults
#define PCH_A_DEFAULT 283  // Primary Channel A Default
#define PCH_B_DEFAULT 384  // Primary Channel B Default

// Secondary Paging Channel Defaults
#define SCH_A_DEFAULT 691  // Secondary Channel A Default 
#define SCH_B_DEFAULT 777  // Secondary Channel B Default

#endif // FEATURE_CDMA_800

#define NID_DEFAULTS 65535

/*==============================================================================
                                 
                                �ڲ��������Ͷ���
                                 
==============================================================================*/
typedef enum KeyClass
{
    KC_IGNORE,        // ���ఴ��������¼������ԣ��Ҳ��贫�ݸ� AEE
    KC_NORMAL,        // ���ఴ��������¼��贫�ݸ� AEE, �����ظ����ͣ��ޱ����¼�
    KC_HELD,          // ���ఴ���б����¼�
    KC_POWER,         // ��Դ��
    KC_POWERHELD,     // ���ఴ��������¼����ᱻ�ظ����ͣ��ޱ����¼����������ᴥ
                      // ����Դ�������µ��¼�

    KC_OEMEVENT       // �ض��¼�ӳ��Ϊ�����¼�
} KeyClass;

typedef struct KeyEntry
{
    hs_key_type    hs_key;      // HS ����
    AVKType        aee_key;     // AEE ����
    KeyClass       kclass;      // ����������
    boolean        bKeyEvtDown; // ��ΪTRUE����һ���¾ͷ��� EVT_KEY �¼�
} KeyEntry;

/*==============================================================================
                                 
                                 ����ȫ�ֱ�������
                                 
==============================================================================*/
static  ui_cmd_type     ui_buffers[UI_CMD_BUFFERS];
q_type                  ui_cmd_free_q;  // ������У������������ buffer
static  q_type          ui_cmd_q;       // ������У��������������

// ���������Ƿ����ϲ� UI ���Ͱ�������¼��ı�־
static boolean gbsend_keys = TRUE;

// �����Ƿ�������״̬
static boolean gbOffline = FALSE;

// ʹ������İ汾�Ƿ��п��汾������ RTRE ������Σ�ʹ�õ�����汾Ҫ���޿���Ҫ��
// �п�������ֻ��ȡһ��������ѡ��!��ʼֵ����������������ú;���״̬������
static boolean gbRunAsUIMVersion = TRUE;

// �����жϵ�ǰ����Ƿ�����Ϊ����ģʽ
static boolean gbRunAsFactoryTestMode = FALSE;

// �����˰������ֵļ� EMKeyTable �е�����ֵ��
static int gKeyActionIndex = -1;

// �����������ֻ��ظ��Ķ�ʱ��
static rex_timer_type oemui_keyhold_timer;

// oemui task ʹ�õ� BREW ��ؽӿڶ��塣�ӿ�һ�����������ļ�ȫ��ʹ�ã������ͷš�
// �����˳� BREW
static IShell       *gpShell = NULL;  
static IBacklight   *gpBacklight = NULL;
static IBacklight   *gpKeyBacklight = NULL;
static IALERT       *gpAlert = NULL;
static ISound       *gpSound= NULL;
//static IPhone       *gpPhone = NULL;
static ICM            *gpICM = NULL;
// �߹���ʱ��
static rex_timer_type oemui_rpt_timer;

const KeyEntry OEMKeyTable[] = 
{
    // ������
    KEY(LEFT, LEFT, NORMAL, TRUE),
    KEY(RIGHT, RIGHT, NORMAL, TRUE),
    KEY(UP, UP, NORMAL, TRUE),
    KEY(DOWN, DOWN, NORMAL, TRUE),
    KEY(INFO, INFO, NORMAL, TRUE),
    
    // ���Ƽ�
    KEY(SEND, SEND, HELD, TRUE),
    KEY(END, END, NORMAL, TRUE),
    KEY(CLR, CLR, HELD, TRUE),
    KEY(SEL, SELECT, NORMAL, TRUE),
    KEY(PWR, POWER, NORMAL, TRUE),
    KEY(SD1, VOLUME_UP, NORMAL, TRUE),      // ����
    KEY(SD2, VOLUME_DOWN, NORMAL, TRUE),    // ����

    // ���϶���
    KEY(HEADSET, HEADSET_CONNECT, OEMEVENT, TRUE),
    // �Ͽ�����
    KEY(HEADSET_OFF, HEADSET_DISCONNECT, OEMEVENT, TRUE),
    
#ifdef FEATURE_HEADSET_SWITCH
    // ���϶���
    KEY(HEADSET_SWITCH, HEADSET_SWITCH, HELD, TRUE),
    // �Ͽ�����
    KEY(HEADSET_SWITCH_OFF, HEADSET_SWITCH_OFF, HELD, TRUE),
#endif //FEATURE_HEADSET_SWITCH    

    // ���ּ�
    KEY(0, 0, HELD, TRUE),
    KEY(1, 1, HELD, TRUE),
    KEY(2, 2, HELD, TRUE),
    KEY(3, 3, HELD, TRUE),
    KEY(4, 4, HELD, TRUE),
    KEY(5, 5, HELD, TRUE),
    KEY(6, 6, HELD, TRUE),
    KEY(7, 7, HELD, TRUE),
    KEY(8, 8, HELD, TRUE),
    KEY(9, 9, HELD, TRUE),
    KEY(STAR, STAR, HELD, TRUE),
    KEY(POUND, POUND, HELD, TRUE),
    
    KEY(FLIP_OPEN, FLIP_OPEN, OEMEVENT, TRUE),
    KEY(FLIP_CLOSE, FLIP_CLOSE, OEMEVENT, TRUE),

    #ifdef FEATRUE_SUPPORT_G_SENSOR
    KEY(GSENSOR_FORWARD, GSENSOR_FORWARD, OEMEVENT, TRUE),
    KEY(GSENSOR_BACKWARD,GSENSOR_BACKWARD, OEMEVENT, TRUE),
    KEY(GSENSOR_STAND,GSENSOR_STAND, NORMAL, TRUE),
    KEY(GSENSOR_LEFT,GSENSOR_LEFT, NORMAL, TRUE),
    KEY(GSENSOR_RIGHT,GSENSOR_RIGHT, NORMAL, TRUE),
    KEY(GSENSOR_UPEND,GSENSOR_UPEND, NORMAL, TRUE),
    KEY(GSENSOR_STEP,GSENSOR_STEP, NORMAL, TRUE),    
    #endif

    /* IGNORE anything else */
    KEY(NONE, UNDEFINED, IGNORE, TRUE)
};

// Keypad Handle
static kpd_handle_type ui_kpd_handle;

// keycode input ring buffer
LOCAL struct 
{
    byte rd_idx;                        /* read index             */
    byte wr_idx;                        /* write index            */
    byte data[ UI_KEY_BUF_SIZE ];       /* data buffer            */
#ifdef FEATURE_KEYPAD_MULTI_KEY
    byte key_parm[UI_KEY_BUF_SIZE];     /* key parm buffer        */
#endif //FEATURE_KEYPAD_MULTI_KEY
} oemui_key_buffer;


#ifdef  FEATURE_UIM_RUIM
uim_rpt_type    oemui_uim_rpt_buf;
#endif  

// ui task ʹ�õ�ȫ�ֲ������������壬���ֵ�ı�ʱӦͨ����Ӧ����ȡ���ã��� ui
// task ����ļ���ֱֹ�Ӵ�ȡ��ȫ�ֱ���
brewui_global_data_type  gbrewui={0,};

#ifdef FEATURE_SLEEP
static boolean brewui_ok_to_sleep = FALSE;
#endif

// LCD ���
static lcd_handle_type oemui_lcd_handle;

#ifdef FEATURE_OTASP
static boolean ui_pwrdown_complete = FALSE;
#endif
#ifdef FEATURE_OTASP
#define STAR228 "*228"
#define STANDARD_OTASP_LENGTH 6
  /* Table specifying default hard-coded special numbers and their
  ** corresponding call type.
  **
  ** Note that for OTASP numbers the corresponding activation code
  ** is also specified.
  */
  static const struct 
  {
      byte                     *num_str;      /* number string */
      cm_call_type_e_type      call_type;     /* associated call type */
      cm_activate_code_e_type  activate_code; /* OTASP activation code */
  } 
  otasp_num_tbl[] = 
  {

      /* number */   /* call type */               /* OTASP activate code */
#ifdef FEATURE_CDMA_800
      { (byte *) "*22800",    CM_CALL_TYPE_STD_OTASP,       CM_ABAND     },
      { (byte *) "*22801",    CM_CALL_TYPE_STD_OTASP,       CM_BBAND     },
#endif
#ifdef FEATURE_CDMA_1900
      { (byte *) "*22802",    CM_CALL_TYPE_STD_OTASP,       CM_ABLOCK    },
      { (byte *) "*22803",    CM_CALL_TYPE_STD_OTASP,       CM_BBLOCK    },
      { (byte *) "*22804",    CM_CALL_TYPE_STD_OTASP,       CM_CBLOCK    },
      { (byte *) "*22805",    CM_CALL_TYPE_STD_OTASP,       CM_DBLOCK    },
      { (byte *) "*22806",    CM_CALL_TYPE_STD_OTASP,       CM_EBLOCK    },
      { (byte *) "*22807",    CM_CALL_TYPE_STD_OTASP,       CM_FBLOCK    },
#endif
#ifdef FEATURE_UI_OTASP_STAR2281X
#error code not present
#endif
  };

#endif /* FEATURE_OTASP */
static cm_client_id_type client_ui;

static boolean oemui_busy = FALSE;

#ifdef FEATRUE_AUTO_SET_NEED_NV_VALUE
extern boolean bIsResetOemNv;
#endif

#ifdef FEATURE_SUPPORT_VC0848
boolean disp_init_status();
#endif
/*==============================================================================
                                 
                                 ��������������
                                 
==============================================================================*/
static void oemui_pre_start_init(void);

static void oemui_init_resources(void);

static void oemui_post_start_init(void);

static void route_brew_event(AEEEvent evt, uint16 wParam);

static void send_key_event(AEEEvent evt, const KeyEntry *key);

static const KeyEntry*  lookup_key(hs_key_type key);

static void process_key_sig(void);

static void process_key_held_sig(void);

#ifdef FEATURE_KEYPAD_MULTI_KEY
static void process_key(keypad_key_event_type   key);
#else
static void process_key(kpd_key_event_type      key);
#endif

static void  oemui_shutdown_brew(void);

static void oemui_stop(void);

static void oemui_offline(void);

static void oemui_handlecmd(ui_cmd_type *cmd_ptr);

static void process_command_sig(void);

static void oemui_keyinit(void);

static void oemui_lcdinit(void);

#ifdef FEATURE_TOUCHPAD
static void oemui_touchpadinit(void);
static void oemui_post_pointer_data(void);
#endif

void VoteForSleep(boolean flag);

#ifdef FEATURE_KEYPAD_MULTI_KEY
byte    get_key(keypad_key_event_type *key_event_ptr);
#else
byte    get_key(void);
#endif

extern boolean OEM_IsCPUHigh(void);
#ifdef FEATURE_LOAD_DEFAULT_SETTING
static void OEM_STRTRIM(char *szItem);
    
static void OEM_ProcessOeminitItem(char *szItem, char *pItembuf, int nSize);

static int OEM_ImportOeminitdata(IShell *m_pShell);
#endif/*FEATURE_LOAD_DEFAULT_SETTING*/

void oemui_get_call_type(
  const cm_num_s_type     *orig_num_ptr,    // pointer to origination #
  cm_call_type_e_type     *call_type_ptr,   // associated call type, save result
  cm_activate_code_e_type *otasp_code_ptr   // OTASP activation code(OTASP call)
);

boolean oemui_call_is_allowed(
  const cm_num_s_type *orig_num_ptr  // pointer to origination #
);
#ifdef FEATURE_CARRIER_THAILAND_HUTCH
/* ben.wen for hutch demand:
In idle screen with sleep mode, when pressing Number key, "*" and "#", 
it shall activate backlight and enter the corresponding key. However, 
when pressing other keys, it shall activate backlight only. Please modify.

During conversation with sleep mode, when pressing Number key, "*" and "#",end key, 
it shall activate backlight and enter the corresponding key. However, 
when pressing other keys, it shall activate backlight only. Please modify.
*/
boolean oemui_ispasskeyevent(KeyEntry *pKeyEntry);
#endif
/*==============================================================================
����: 
    IsRunAsFactoryTestMode
       
˵��: 
    ���������жϵ�ǰ����Ƿ񰴳���ģʽ���С�
       
����: 
    none
    
����ֵ:
    TRUE: ��ǰ�������Ϊ����ģʽ
    FALSE: ��ǰ�������Ϊ�ǳ���ģʽ
       
��ע:
    ʹ������İ汾�Ƿ��п��汾������ RTRE ������Σ�ʹ�õ�����汾Ҫ���޿���Ҫ��
    �п�������ֻ��ȡһ��������ѡ��!

==============================================================================*/
boolean   IsRunAsFactoryTestMode(void)
{
    return gbRunAsFactoryTestMode;
}


/*==============================================================================
����: 
    IsRunAsUIMVersion
       
˵��: 
    ���������жϵ�ǰ����Ƿ��п��汾���С�
       
����: 
    none
    
����ֵ:
    TRUE: ��ǰ�������Ϊ�п��汾
    FALSE: ��ǰ�������Ϊ�޿�?�?
       
��ע:
    ʹ������İ汾�Ƿ��п��汾������ RTRE ������Σ�ʹ�õ�����汾Ҫ���޿���Ҫ��
    �п�������ֻ��ȡһ��������ѡ��!

==============================================================================*/
boolean   IsRunAsUIMVersion(void)
{
    return gbRunAsUIMVersion;
}

/*==============================================================================
����: 
    oemui_rex_wait
       
˵��: 
    �����ȴ� oemui task ��������ָ���źź󷵻ء��ڵȴ������л�ά�� watchdog ��ʱ
    ����
       
����: 
    waitSig [in]: Ҫ�ȵ��źš�
    
����ֵ:
    none
       
��ע:
    ���ȴ� UI task ������ָ���ź�,���ô˺���,����ֱ�ӵ��� rex_wait ! ���ܲ���
    ���Դ������źţ���ֻҪ����֮һ���֣�������������!! ���ú���֮ǰ���������
    ����ź�!
==============================================================================*/
void oemui_rex_wait(rex_sigs_type waitSig)
{
    rex_sigs_type sigs;
    
    // �ȴ��ض��źŵ�ͬʱ��ע���߹�
    do 
    {
        sigs = rex_wait(UI_RPT_TIMER_SIG | waitSig);
    
        (void) rex_clr_sigs(rex_self(), (UI_RPT_TIMER_SIG | waitSig));
        
        if ((sigs & UI_RPT_TIMER_SIG) != 0) 
        {
            // Kick the dog
            (void) rex_set_timer(&oemui_rpt_timer, DOG_UI_RPT_TIME);
            dog_report(DOG_UI_RPT);
        }
    
    } while ((sigs & waitSig) == 0);
}

/*==============================================================================
����: 
    oemui_sleep_cb
       
˵��: 
    oemui_sleep ʹ�õĶ�ʱ�ص�������
       
����: 
    dummy [in]: ��ʹ�õĲ�����
    
����ֵ:
    none
       
��ע:
    
==============================================================================*/
static void oemui_sleep_cb(int32 dummy /*lint -esym(715,dummy) */)
{
   (void) rex_set_sigs(&ui_tcb, UI_WAIT_SIG);
}


/*==============================================================================
����: 
    oemui_sleep
       
˵��: 
    �����ӳ�ִ�� oemui task �ض�ʱ�䡣���ӳ�ʱ���ڣ�oemui watchdog ��ʱ���ᱻά
    ����
       
����: 
    ms [in]: �ӳ�ʱ�䣬��λ: ����
    
����ֵ:
    none
       
��ע:
    ����ʹ�� UI_WAIT_SIG �ź�
    
==============================================================================*/
void oemui_sleep(int32 ms)
{
    clk_cb_type      cbWait;

    // ����
    clk_def(&cbWait);

    (void) rex_clr_sigs(rex_self(), UI_WAIT_SIG);
    
    // ע��
    clk_reg(&cbWait, oemui_sleep_cb, ms, 0, FALSE);

    // �ȴ���ʱ���ص��������� UI_WAIT_SIG �ź�
    oemui_rex_wait(UI_WAIT_SIG);
    
    // ע����ʱ��
    clk_dereg(&cbWait);
}

#ifdef FEATURE_FTM_RF
/*==============================================================================
����: 
    oemui_wait_in_ftm_rf
       
˵��: 
    ���� RF FTM_MODE , UI ������ִ���κι��ܡ�
    ����
       
����: 
    none
    
����ֵ:
    none
       
��ע:
 
     
==============================================================================*/
static void oemui_wait_in_ftm_rf(void) 
{
    rex_sigs_type sigs;

    // Wait in FTM RF mode and kick the dog forever
    for (;;) 
    {
        sigs = rex_wait(UI_RPT_TIMER_SIG);
        (void) rex_clr_sigs(rex_self(), sigs);
        
        if ((sigs & UI_RPT_TIMER_SIG) != 0) 
        {
            // kick the dog 
            (void) rex_set_timer(&oemui_rpt_timer, DOG_UI_RPT_TIME);
            dog_report(DOG_UI_RPT);
        }
    }
}
#endif


/*==============================================================================
����: 
    oemui_pre_start_init
       
˵��: 
    ������ task ����ǰ��һЩ��ʼ��������
       
����: 
    none
    
����ֵ:
    none
       
��ע:
    
==============================================================================*/
static void oemui_pre_start_init(void)
{
    int i;
    
    // �����߹���ʱ��
    rex_def_timer(&oemui_rpt_timer, &ui_tcb, UI_RPT_TIMER_SIG);
    
    // ���尴���ظ������ֶ�ʱ��
    rex_def_timer(&oemui_keyhold_timer, &ui_tcb, UI_KEY_HELD_SIG);
    
    // ��ʼ���� oemui task ͨ�ŵ��������
    (void) q_init(&ui_cmd_q);
    (void) q_init(&ui_cmd_free_q);
    for (i=0; i<UI_CMD_BUFFERS; i++) 
    {
        ui_buffers[i].hdr.done_q_ptr = &ui_cmd_free_q;
        q_put(&ui_cmd_free_q, q_link(&ui_buffers[i], &ui_buffers[i].hdr.link));
    }
    
    // ��� UI Task ����ǰ OEM ��Ҫ�ĳ�ʼ��
#ifndef OEMHEAP
    // �ѵĳ�ʼ��--Ŀǰ�����ô˷�ʽ
    mem_init_heap(&brew_heap, 
                  brew_heap_space, 
                  sizeof(brew_heap_space),
                  oom_hook);
#endif

#if defined(FEATURE_WMS_APP) && (defined(FEATURE_CDSMS_CACHE) || defined(FEATURE_CDSMS_CACHE_USELIST))
    // Register this callback to WMS.  This callback is for the purpose of
    // sharing cache that is stored in WMS.  WMS does NOT have to be
    // initialized for this callback to be registered.
#ifdef   FEATURE_CDSMS_CACHE
    wms_reg_msg_info_cache_cb(OEMWMS_MsgInfoCacheCb);
#else
    wms_reg_msg_info_cache_cb(OEMWMS_MsgInfoCacheCbExt);
#endif    
#endif

    // ������ʼ��
    uisnd_snd_init();
    
    // ���̳�ʼ��
    oemui_keyinit();
    
    // LCD ��ʼ��
    oemui_lcdinit();
}

/*==============================================================================
����: 
    oemui_init_resources
       
˵��: 
    ������ʼ�� BREW ��Դ�ļ���
       
����: 
    none
    
����ֵ:
    none
       
��ע:
    �������ܻ��޸� gbOffline ��ȡֵ
    
==============================================================================*/
static void oemui_init_resources(void)
{
    char             *strMsg = NULL;
    int i;
    struct fs_stat sbuf;
    const char *brew_dirs[] =
    {
        "brew",
        "brew/sys",
        "brew/shared",
        "brew/mod",
        "brew/mif",
        "user",
        "user/sound",
        "user/sound/ringer"
    };
    
    int nDirNum = ARR_SIZE(brew_dirs); //Number of 'other' brew directories.
    
    for (i=0; i < nDirNum; i++) 
    {
        if (efs_stat((char*) brew_dirs[i], &sbuf) != 0)
        {
            if (efs_mkdir((char*)brew_dirs[i], 0555)== -1)
            {
                ERR("Unable to create directory", 0, 0, 0);
                gbOffline = TRUE;
            }
        }
    }

    if (gbOffline) 
    {
        if (NULL == strMsg) 
        {
            strMsg = "Unable to "
                     "create directory";
        }
        
        lcd_message(strMsg);
    }
}

#ifdef FEATURE_UIM
/*==============================================================================
����: 
    oemui_ruim_report
       
˵��: 
    ������������ R-UIM ��״̬���档״̬������ȫ�ֱ����У��������ź�֪ͨ oemui 
    task
       
����: 
    none
    
����ֵ:
    none
       
��ע:
    ������������
    
==============================================================================*/
void oemui_ruim_report(uim_rpt_type *report)
{
    oemui_uim_rpt_buf = *report;
    rex_set_sigs(&ui_tcb, UI_RUIM_SIG);
} 

/*==============================================================================
����: 
    oemui_set_uim_dir_present
       
˵��: 
    ����ͨ��������ȷ�����Ƿ���ϡ������� UIM DIR ָʾ��
       
����: 
    none
    
����ֵ:
    none
       
��ע:
    
==============================================================================*/
void oemui_set_uim_dir_present(void)
{
    uim_cmd_type        *uim_cmd_ptr = NULL;
    byte                ui_uim_dir_indicator = NVRUIM_NO_DIR_PRESENT;

    // Execute a Select on the MF
    if ((uim_cmd_ptr = (uim_cmd_type*) q_get( &uim_free_q )) != NULL)
    {
        // ���������ز���
        uim_cmd_ptr->hdr.command = UIM_SELECT_F;
        uim_cmd_ptr->select.file_type = UIM_MF;
        uim_cmd_ptr->hdr.rpt_function = oemui_ruim_report;
        uim_cmd_ptr->hdr.options = UIM_OPTION_ALWAYS_RPT;
        uim_cmd_ptr->hdr.protocol = UIM_CDMA;
        uim_cmd_ptr->hdr.cmd_hdr.task_ptr = NULL;
        uim_cmd_ptr->hdr.cmd_hdr.sigs = NULL;

        // ������ź�
        (void) rex_clr_sigs(&ui_tcb, UI_RUIM_SIG);
        
        // ��������
        uim_cmd(uim_cmd_ptr);

        // �ȴ��������Ϸ���
        (void) oemui_rex_wait(UI_RUIM_SIG);

        if (oemui_uim_rpt_buf.rpt_status != UIM_PASS)
        {
            // ��û����
			MSG_ERROR( "rpt_status UIM_MF FAIL!", 0, 0, 0 );            
            nvruim_set_uim_dir_present(ui_uim_dir_indicator);
            return;
        }
        
        // The MF presents
        ui_uim_dir_indicator |= NVRUIM_MF_PRESENT;
    }
    else /* No free queue */
    {
        MSG_ERROR( "No free queue for UIM", 0, 0, 0 );
    }

    /* Execute a Select on the CDMA DF */
    if ((uim_cmd_ptr = (uim_cmd_type*) q_get( &uim_free_q )) != NULL)
    {
        // ���������ز���
        uim_cmd_ptr->hdr.command          = UIM_SELECT_F;
        uim_cmd_ptr->select.file_type     = UIM_DF;
        uim_cmd_ptr->select.dir           = UIM_CDMA_DF;
        uim_cmd_ptr->hdr.rpt_function     = oemui_ruim_report;
        uim_cmd_ptr->hdr.options          = UIM_OPTION_ALWAYS_RPT;
        uim_cmd_ptr->hdr.protocol         = UIM_CDMA;
        uim_cmd_ptr->hdr.cmd_hdr.task_ptr = NULL;
        uim_cmd_ptr->hdr.cmd_hdr.sigs     = NULL;
        
        // ������ź�
        (void) rex_clr_sigs( &ui_tcb, UI_RUIM_SIG );
        
        // ��������
        uim_cmd( uim_cmd_ptr );
        
        // �ȴ��������Ϸ���
        (void) oemui_rex_wait(UI_RUIM_SIG);
        
        if (oemui_uim_rpt_buf.rpt_status == UIM_PASS)
        {
            ui_uim_dir_indicator |= NVRUIM_CDMA_DF_PRESENT;
        }
        else
        {
	     	MSG_ERROR( "rpt_status UIM_DF FAIL!", 0, 0, 0 );                    
        }
    }
    else /* No free queue */
    {
        MSG_ERROR( "No free queue for UIM", 0, 0, 0 );
    }
    
    nvruim_set_uim_dir_present(ui_uim_dir_indicator);
}

#ifdef  FEATURE_UIM_RUN_TIME_ENABLE
/*==============================================================================
����: 
    oemui_send_rtre_command
       
˵��: 
    �������� NV_RTRE_OP_CONFIG_F �������õ�ǰ����汾��
       
����: 
    data_ptr [in]: ָ�������������ָ��
    
����ֵ:
    none
       
��ע:
    none
       
==============================================================================*/
static void oemui_send_rtre_command(void)
{
    nv_cmd_type     nvi;
    nv_item_type    ui_nv_buf;
    nv_rtre_configuration_type  rtre_config;
    
    nvi.tcb_ptr = rex_self();
    nvi.sigs = UI_NV_SIG;
    nvi.done_q_ptr = NULL;
    
    // ��Ҫ�������
    nvi.item = NV_RTRE_CONFIG_I;
    
    // ����������
    nvi.cmd = NV_RTRE_OP_CONFIG_F;
    
    // ����ָ��������ص�����
    nvi.data_ptr = &ui_nv_buf;
    
    // ȱʡֵ
    ui_nv_buf.rtre_config = NV_RTRE_CONFIG_RUIM_OR_DROP_BACK;
    
    if (NV_DONE_S != OEMNV_Get(NV_RTRE_CONFIG_I, &ui_nv_buf))
    {
        MSG_ERROR("Could not read NV_RTRE_CONFIG_I, assuming DROP_BACK",0,0,0);
    }
    
    // Hutch Ҫ�����Ϊ�п����Ҳ��ܸ�������
#if defined(FEATURE_CARRIER_THAILAND_HUTCH)
    ui_nv_buf.rtre_config = NV_RTRE_CONFIG_RUIM_ONLY;
#endif
    
    rtre_config = ui_nv_buf.rtre_config;
        
    // ��������źŲ��ȴ���Ӧ
    (void) rex_clr_sigs(rex_self(), UI_NV_SIG);
    nv_cmd(&nvi);
    oemui_rex_wait(UI_NV_SIG);
    
    // ���ڿ�ȷ��ʹ�õ�����Ƿ�֧�ֿ���
    if (NV_RTRE_CONTROL_USE_RUIM != nv_rtre_control())
    {
        gbRunAsUIMVersion = FALSE;
        oemui_unlockuim();
    }
}
#endif


/*==============================================================================
����: 
    oemui_wait_ruim_inited
       
˵��: 
    �����ȴ�ֱ�� uim ������ɡ�
       
����: 
    none
    
����ֵ:
    none
       
��ע:
    �������� uim_get_uim_status ����ֵ���ȴ�ֱ�� UIM_INITIALIZED_S ״̬���ػ�
    ��ʱ��
    
    ע��: �ڱ������о�Ҫȷ�����ֻ�Ӧʹ�õ�����汾�����п������޿�������ֻ��ȡһ
==============================================================================*/
static void oemui_wait_ruim_inited(void)
{
    
    //int status;
    //boolean bCardConnected = FALSE;
    nv_item_type nvi; // TODO
    
    // ���� UTK �ĸ�Ҫ���� (Profile Download) ���ֻ�ESN���ڿ��ϵȶ�д����������
    // �����������⵽�������֤ͨ�������
   
    // ��ȡ�ֻ� ESN �����档BREW ��Ȩ������Ӧ�û��õ�
    nvruim_set_esn_usage( NV_RUIM_USE_ESN );
    (void)OEMNV_Get(NV_ESN_I, &nvi);
    OEM_SetMEESN(nvi.esn.esn);
    //temp add ,only for test
    //nvi.esn.esn = 0;
    //temp add ,only for test
    //DBGPRINTF("nvi.esn.esn %d",nvi.esn.esn);
    if(nvi.esn.esn == 0)
    {
        gbRunAsFactoryTestMode = TRUE;
    }
}
#endif

/*==============================================================================
����: 
    oemui_post_start_init
       
˵��: 
    ��������������������κα�Ҫ��ʼ����
       
����: 
    none
    
����ֵ:
    none
       
��ע:
    
==============================================================================*/
static void oemui_post_start_init(void)
{
#ifdef FEATURE_FTM_RF
    nv_item_type nvi;
    
    // ���ȼ�� NV_FTM_MODE_I NV ���Ƿ����á����� FTM_MODE ����������ת���ģʽ��
    if (OEMNV_Get(NV_FTM_MODE_I, &nvi) == NV_DONE_S) 
    {
        if (nvi.ftm_mode == (int) FTM_MODE) 
        {
            rex_set_sigs(&ui_tcb, UI_RPT_TIMER_SIG);
            oemui_wait_in_ftm_rf();
        }
    }
#endif
  
#ifdef FEATURE_MULTIMODE_ARCH
    // Do not confuse tmc/cm by issuing any command until after tmc_init
    // has had a chance to complete its initialization.  tmc_init will
    // send the first command to either go online or offline, so simply
    // wait for it.
    while (tmc_get_state() == TMC_PWROFF) 
    {
        // Wait some milliseconds
        oemui_sleep(SLEEP_BETWEEN_TMC_CHECKS_MS);
    }
  
#else

    // �� LPM ģʽ��ʼ�������ܿ췢�� LPM ���������֮ǰ MC ���ڵȴ�״̬��
    // �յ������ MC ���ܼ������ʼ����
    (void) cm_ph_cmd_oprt_mode(NULL,
                             NULL,
                             CM_CLIENT_ID_ANONYMOUS,
                             SYS_OPRT_MODE_LPM);
#endif

    // link correct Standard C Lib
    // (void) setlocale(LC_ALL, "C");

    // ��ʼ����Դ�ļ�
    oemui_init_resources();

    if (FALSE == gbOffline) 
    {
        // ע��˺�������Ƕ�׵��õĺ��������漰�� UIM ���Ĳ���!!!
        // �Ա��ִ���ε���Զ���
        
#ifdef FEATURE_UIM
        // ִ�п���ز���ǰ�����鿨�Ƿ���ϣ������� nvruim_set_uim_dir_present
        oemui_set_uim_dir_present();

#ifdef FEATURE_UIM_RUN_TIME_ENABLE
        // �ȸ����������õ�ǰ���
        oemui_send_rtre_command();
#else        
        // �޶�̬ȷ�����ܣ��������ʱ��ȷ�������Ϊ�п��汾
        gbRunAsUIMVersion = TRUE;        
#endif
        
        if (gbRunAsUIMVersion)
        {
            // �ȴ� RUIM ��ʼ�����
            oemui_wait_ruim_inited();
        }
#else
        // �޶�̬ȷ�����ܣ��������ʱ��ȷ�������Ϊ�޿��汾
        gbRunAsUIMVersion = FALSE; 
        oemui_unlockuim();       
#endif // FEATURE_UIM

        gpShell = AEE_Init(AEE_APP_SIG);

        if (NULL == gpShell) 
        {
            gbOffline = TRUE;
            lcd_message("BREW startup"
                        "failed.     ");
        } 
        else 
        {// �����ﴴ�� oemui task �õ��� BREW ����ؽӿ�
#ifndef  FEATURE_2008_POWERON_LOGIC
            (void)ISHELL_RegisterNotify(gpShell,
                                        AEECLSID_DIALER,
                                        AEECLSID_SHELL,
                                        NMASK_SHELL_START_STATUS);
            (void)ISHELL_RegisterNotify(gpShell,
                                        AEECLSID_WMSAPP,
                                        AEECLSID_SHELL,
                                        NMASK_SHELL_START_STATUS);
            oemui_cm_init();
#endif
#ifdef FEATURE_SUPPORT_VC0848
            while(disp_init_status() == FALSE)
            {
                rex_sleep(100);
            }
#endif
            (void) ISHELL_CreateInstance(gpShell,
                                 AEECLSID_BACKLIGHT,
                                 (void**)&gpBacklight);
                                 
            // ���⴦��
            // Turn on the backlight immediately on startup so the
            // user can see the startup animation at night 
            if (gpBacklight) 
            {
                IBACKLIGHT_Enable(gpBacklight);
            } 
            
            (void) ISHELL_CreateInstance(gpShell,
                                 AEECLSID_BACKLIGHT_KEYPAD,
                                 (void**)&gpKeyBacklight);
            if (gpKeyBacklight) 
            {
                IBACKLIGHT_Enable(gpKeyBacklight);
            } 
                                 
            (void) ISHELL_CreateInstance(gpShell,
                                 AEECLSID_ALERT,
                                 (void**)&gpAlert);

            (void) ISHELL_CreateInstance(gpShell,
                                 AEECLSID_SOUND,
                                 (void**)&gpSound);
                                 
            //(void) ISHELL_CreateInstance(gpShell,
            //                     AEECLSID_PHONE,
            //                     (void**)&gpPhone);

            (void) ISHELL_CreateInstance(gpShell,
                                 AEECLSID_CM,
                                 (void**)&gpICM);
#ifdef FEATURE_SUPPORT_VC0848
            {
                IFileMgr * pFileMgr = NULL;
                (void) ISHELL_CreateInstance(gpShell,
                                 AEECLSID_FILEMGR,
                                 (void**)&pFileMgr);
                if(pFileMgr)
                {
                    if(IFILEMGR_Test(pFileMgr, AEEFS_ROOT_DIR MG_MUSIC_FOLDER) == SUCCESS)
                    {
                        int nRet;
                        nRet = IFILEMGR_EnumInit(pFileMgr, AEEFS_ROOT_DIR MG_MUSIC_FOLDER, FALSE);
                        if(nRet == SUCCESS)
                        {
                            FileInfo info;
                            while(IFILEMGR_EnumNext(pFileMgr, &info))
                            {
                                IFILEMGR_Remove(pFileMgr, info.szName);
                            }
                        }
                    }
                    IFILEMGR_Release(pFileMgr);
                    pFileMgr = NULL;
                }
            }
#endif /* FEATURE_SUPPORT_VC0848 */
#ifdef FEATURE_TOUCHPAD
            oemui_touchpadinit();
#endif

        }
    }

    if (gbOffline) 
    {
        VoteForSleep(TRUE);

        // send offline command to everybody else
        (void) cm_ph_cmd_oprt_mode(NULL,
                                NULL,
                                CM_CLIENT_ID_ANONYMOUS,
                                SYS_OPRT_MODE_OFFLINE
                                );
    } 
    else 
    {
        // Don't want to sleep until we're done starting up
        VoteForSleep(FALSE);

        // ��������� BREW OEM ��صĳ�ʼ��
        // oemui_oem_post_brew_init();

    }
#ifdef FEATURE_LOAD_DEFAULT_SETTING
    OEM_ImportOeminitdata(gpShell);
#endif /*FEATURE_LOAD_DEFAULT_SETTING*/

#ifdef FEATRUE_AUTO_SET_NEED_NV_VALUE
    if(TRUE == bIsResetOemNv)
    {
        OEM_RestoreFactorySetting();
    }
#endif
}


/*==============================================================================
����: 
    ui_task
       
˵��: 
    ��Ҫ�û��ӿں�����������һ���������ѭ���������������źš�������Ҫ������
    ת��Ϊ�¼���Ҳ������水���ı���Ͱ�����
       
����: 
    dummy [in]: �ݲ��á�
    
����ֵ:
    �Ӳ�����
       
��ע:
    REX ��NV �� DIAG �� HS �������������
    
==============================================================================*/
void ui_task(dword dummy   /*lint -esym(715,dummy) */)
{
#ifdef FEATURE_KEYPAD_MULTI_KEY
    keypad_key_event_type   key;
#else
    kpd_key_event_type      key;
#endif
    rex_sigs_type aSig;
    
    // ��� task ����ǰ��һЩ��ʼ������
    oemui_pre_start_init();

    // �������� oemui task ���˺󣬿������ʹ������ task ��Ҫ�ĳ�ʼ����
#ifdef FEATURE_MULTIMODE_ARCH
    task_start(UI_RPT_TIMER_SIG, DOG_UI_RPT, &oemui_rpt_timer);
#else
    mc_task_start(UI_RPT_TIMER_SIG, DOG_UI_RPT, &oemui_rpt_timer);
#endif /* FEATURE_MULTIMODE_ARCH */
    (void) rex_clr_sigs(rex_self(), TASK_START_SIG);

    // ����������������κα�Ҫ��ʼ��
    oemui_post_start_init();

    // ���Կ�����ʼ�������е��κΰ��������������� END �����ܱ����֣���
    // �ᴥ���ػ�!
    for (;;) 
    {
#ifdef FEATURE_KEYPAD_MULTI_KEY
        key.key_code = get_key(&key);
#else
        key.key_code = (hs_key_type)get_key();
#endif

        if (key.key_code == HS_NONE_K) 
        {
            break;
        }

        // ���԰���...
    }

    // �߹����������Ǳ�Ҫ�ġ��� task_start �� mc_task_start ����֤ refui_rpt_timer
    // ���У��ʿ���ֻҪ��������ʱ����һֱ�ղ��� UI_RPT_TIMER_SIG �ź�
    (void) rex_set_sigs(rex_self(), UI_RPT_TIMER_SIG);

    for (;;) 
    {
        // �ȴ���ע���źŽ���
        rex_sigs_type sigs = rex_wait(UI_RPT_TIMER_SIG
                                      | TASK_OFFLINE_SIG
                                      | TASK_STOP_SIG
                                      | AEE_APP_SIG
#ifdef FEATURE_BREW_SIO
                                      | AEE_SIO_SIG
#endif
                                      | UI_KEY_SIG
                                      | UI_KEY_HELD_SIG
#ifdef FEATURE_TOUCHPAD
                                      |UI_GET_PEN_SIG
#endif
                                      | UI_CMD_Q_SIG
                                      | UI_RTC_SIG);

        // �����źţ�������������ٴ���
        (void) rex_clr_sigs(rex_self(), sigs);

        // ����ÿһ���ź�
        while (sigs) 
        {
            // �����ź�
            aSig = sigs ^ (sigs & (sigs - 1));
            sigs ^= aSig;
            
            // �������ľ����ź�
            switch (aSig)
            {
                case UI_RPT_TIMER_SIG:
                    (void) rex_set_timer(&oemui_rpt_timer, DOG_UI_RPT_TIME);
                    dog_report(DOG_UI_RPT);
#ifdef FEATURE_OEM_DEBUG
                    {
                        extern uint32 AEEkHeap_GetMemStatsExt(uint32 * pdwTotal, uint32 * pdwMax);
                        uint32 dwFree = 0, dwTotal = 0, dwMaxBolckSize = 0;
                        
                        dwFree = AEEkHeap_GetMemStatsExt(&dwTotal, &dwMaxBolckSize);
                        DBGPRINTF("Free = %ld  Total = %ld  MaxBolckSize = %ld", dwFree, dwTotal, dwMaxBolckSize);
                    }
#endif

#ifdef FEATURE_SUPPORT_BT_BCM
                    bcmapp_dm_set_sleep_state(TRUE);
#endif
                    break;

                case TASK_OFFLINE_SIG:
                    // �� MC ȷ�ϣ����������
#ifdef FEATURE_MULTIMODE_ARCH
                    task_offline();
#else
                    (void) rex_set_sigs( &mc_tcb, MC_ACK_SIG );
#endif
                    oemui_offline();
                    break;

                case TASK_STOP_SIG:
                    // �� MC ȷ�ϣ����������
#ifdef FEATURE_MULTIMODE_ARCH
                    task_stop();
#else
                    (void) rex_set_sigs( &mc_tcb, MC_ACK_SIG );
#endif
                    oemui_stop();
                    break;

                case AEE_APP_SIG:
                case UI_RUIM_SIG:
                    // Notify AEE that the signal was received 
                    if (gpShell) 
                    {
                        uint32 ret;
                        
                        VoteForSleep(FALSE);
                        ret = AEE_Dispatch();
                    
                        if ((!OEM_IsCPUHigh()) && (ret == 0) && !oemui_busy)
                        {
                            VoteForSleep(TRUE);
                        }
                        else 
                        {
                            VoteForSleep(FALSE);
                        }
                    }
                    break;

                case UI_KEY_SIG:
                    (void)rex_clr_timer(&oemui_keyhold_timer);
                    process_key_sig();
                    break;
                    
                case UI_KEY_HELD_SIG:
                    process_key_held_sig();
                    break;

                case UI_CMD_Q_SIG:
                    process_command_sig();
                    break;
         
#ifdef FEATURE_BREW_SIO
                case AEE_SIO_SIG:
                    { 
                        extern void OEMSIO_TaskHandler(void);
                        
                        (void) OEMSIO_TaskHandler();
                    }
                    break;
#endif
                case UI_RTC_SIG:
                    //DBGPRINTF("UI_RTC_SIG");
                    ISHELL_SendEvent( gpShell, AEECLSID_CORE_APP, EVT_RTC, 0, 0);
                    OEMRTC_Run_Call_Back_Function(); 
                    //OEMRTC_Process_Timer_List();
                    break;

#ifdef FEATURE_TOUCHPAD
                case UI_GET_PEN_SIG:
                    oemui_post_pointer_data();
                    break;
#endif
                default:
                    MSG_ERROR("Unhandled signal %08lx!", (long) aSig, 0, 0);
                    break;
            }
        }
    }
}

/*==============================================================================
����: 
    route_brew_event
       
˵��: 
    ���������� BREW ���Ͱ�������¼���
       
����: 
    evt [in]: Ҫ���͵� BREW �¼�
    key [in]: ��
    
����ֵ:
    none
       
��ע:
    
==============================================================================*/
#ifdef FEATURE_SPORTS_APP
extern boolean g_sportsapp_entry_pedometer_flag;
#endif
static void route_brew_event(AEEEvent evt, uint16 wParam)
{
    AEECLSID cls = AEE_Active();
#ifdef FEATURE_KEYGUARD
    if (!OEMKeyguard_HandleEvent(evt, wParam)) 
#endif
    {
#ifdef FEATURE_HEADSET_SWITCH
#if 0
        // AVK_HEADSET_SWITCH_OFF means headset key release
        if(wParam == AVK_HEADSET_SWITCH_OFF)
        {
            if(AEECM_IS_VOICECALL_CONNECTED(gpICM))
            {
                wParam = AVK_END;
            }
        }
#endif
        // AVK_HEADSET_SWITCH means headset key press
        if (wParam == AVK_HEADSET_SWITCH)      	
        {
            if(AEECM_IS_VOICECALL_CONNECTED(gpICM))
            {
                // AVK_END instead of AVK_HEADSET_SWITCH in conversation
                wParam = AVK_END;
            }
            else
            {
                // AVK_SEND instead of AVK_HEADSET_SWITCH
                wParam = AVK_SEND;
            }
        }
#endif //FEATURE_HEADSET_SWITCH        
        
         if (cls == AEECLSID_SVCPRGAPP && wParam == AVK_END)      	
         {
           wParam = AVK_ENDCALL;
         }
        if (cls == AEECLSID_DIALER && wParam == AVK_END)      	
        {// AVK_END ��ص�ȫ�����򣬻ص��������棬������ת�������������
            db_items_value_type dbItemValue;
            
            db_get(DB_IN_USE, &dbItemValue);
            if (dbItemValue.in_use)
            {
                wParam = AVK_ENDCALL;
            }
        }    
 #ifdef FEATURE_APP_MUSICPLAYER
        if(cls==AEECLSID_APP_MUSICPLAYER && wParam == AVK_END)
        {
            wParam = AVK_BGPLAY;
        }
 #endif
        if(cls == AEECLSID_APP_SECURITYMENU && wParam == AVK_END)
        {
            wParam = AVK_WITHDRAW;
        }
        //end added
				
				#ifdef FEATURE_SPORTS_APP
				if(cls == AEECLSID_SPORTSAPP && wParam == AVK_END)
				{ 
					if(g_sportsapp_entry_pedometer_flag)
					{
						wParam = AVK_BGRUN;
					}
				}
				#endif

                        #ifdef FEATURE_MORSE
                        if(cls == AEECLSID_MORSE && wParam == AVK_END)
                        {

                                {
                                    wParam = AVK_MORSE_BG;
                                }
                        }
                        #endif

        (void) AEE_Event(evt, wParam, 0);

        switch (evt) 
        {
            case EVT_KEY_PRESS:
                if (gpAlert) 
                {
                    IALERT_KeyBeep(gpAlert, (AVKType) wParam, TRUE);
                }
                break;

            case EVT_KEY:
                if (AVK_END == (AVKType) wParam) 
                {
                    // If the Idle applet wasn't the recepient of the
                    // AVK_END, send an OEMEVT_KEY to the Idle applet
                    // so it knows that it needs to return all the way to 
                    // back to the Idle screen. 
                    //
                    // Note the use of ISHELL_PostEvent() instead of
                    // ISHELL_SendEvent().  This is to ensure that the
                    // Idle applet has been completely resumed by BREW
                    // before sending it the event. 
                    if (AEECLSID_CORE_APP != AEE_Active()) 
                    {
                        (void) ISHELL_PostEvent(gpShell,
                                               AEECLSID_CORE_APP,
                                               AVK_END,
                                               wParam,
                                               0);
                    }
                }
                break;

            case EVT_KEY_RELEASE:
                if (oemui_busy || OEM_IsCPUHigh())
                {
                    VoteForSleep(FALSE);
                }
                else
                {
                    VoteForSleep(TRUE);
                }
                if (gpAlert) 
                {
                    IALERT_KeyBeep(gpAlert, (AVKType) wParam, FALSE);
                }
                break;
            
            default:
                break;
        }
    }
}

/*==============================================================================
����: 
    send_key_event
       
˵��: 
    ���������� BREW ���Ͱ�������¼���
       
����: 
    evt [in]: Ҫ���͵� BREW �¼�
    key [in]: ��
    
����ֵ:
    none
       
��ע:
    
==============================================================================*/
////extern boolean is_g_bMp3PlayerRun();
extern boolean is_alarm_time_reach();
static void send_key_event(AEEEvent evt, const KeyEntry *key)
{
    // Only reset the phone with the END key when offline
    // and BREW isn't running (ie, not in service programming applet)
    if (TRUE == gbOffline)
    {
        if ((EVT_KEY_PRESS == evt) && (AVK_END == key->aee_key)) 
        {
            // Reset phone
            (void) cm_ph_cmd_oprt_mode( NULL,
                                     NULL,
                                     CM_CLIENT_ID_ANONYMOUS,
                                     SYS_OPRT_MODE_RESET
                                     );

            lcd_message("Resetting");
        }
    } 
    else 
    {
        if (gpShell) 
        {
            if (KC_OEMEVENT == key->kclass) 
            {
                // Ĭ�Ͻ����¼���Ӧ�ó���
                AEECLSID cls = AEE_Active();/*AEECLSID_CORE_APP;*/
                
                // ������һ���¼�(�����ͷ��¼�)
                if (EVT_KEY_PRESS == evt) 
                {
                    switch (key->aee_key)
                    {
                        case AVK_HEADSET_CONNECT:
                        case AVK_HEADSET_DISCONNECT:
                            {
                                AEESoundInfo soundStuff;     
                                boolean b_FMBackground = FALSE;
                                int8 classitems = 0;
                                
                                ISOUND_Get(gpSound, &soundStuff);
                                soundStuff.eDevice = (key->aee_key == AVK_HEADSET_CONNECT ? 
                                                                AEE_SOUND_DEVICE_HEADSET:AEE_SOUND_DEVICE_HANDSET);
                                (void) ISOUND_Set(gpSound,  &soundStuff);
                                ISOUND_SetDevice(gpSound);                         
                                //AEE_Event (EVT_HEADSET, (key->aee_key == AVK_HEADSET_CONNECT ? TRUE:FALSE), 0);
                                {
                                    AEEDeviceNotify nt={0,0};
                                    
                                    nt.wParam = (key->aee_key == AVK_HEADSET_CONNECT ? TRUE:FALSE);
                                    AEE_SEND_HEADSET_EVT(&nt);    
                                }
                                
                                OEM_GetConfig(CFGI_FM_BACKGROUND,&b_FMBackground, sizeof(b_FMBackground));
                                if(TRUE == b_FMBackground)
                                {
                                    classitems = 2;
                                }
                                else
                                {
                                    classitems = 1;                                
                                }
                                
                                {
                                    static AEECLSID classIDs[] = {
                                            AEECLSID_CORE_APP,
                                            AEECLSID_APP_FMRADIO
                                    };
                                    int     i               = 0;
                                    boolean headsetPresent  = key->aee_key == AVK_HEADSET_CONNECT ? TRUE : FALSE;

                                    OEM_SetConfig( CFGI_HEADSET_PRESENT, &headsetPresent, sizeof( headsetPresent));
                                    for( i = 0; i < classitems; i ++)
                                    {
                                        if( cls != classIDs[i])
                                        {
                                            ISHELL_PostEvent( gpShell,
                                                      classIDs[i],
                                                      EVT_HEADSET,
                                                      headsetPresent,
                                                      0
                                                  );
                                        }
                                    }
                                }
                            }
#ifndef CUST_EDITION
                            {
#ifdef FEATURE_SUPPORT_VC0848                                
                                extern void audio_speaker_ctrl(boolean on);
#endif
                                if(!IsMp3PlayerStatusNone()
                                	|| (TRUE == MediaGallery_GetPlayMusicStatus())
                                    || (TRUE == is_videoplayer_play()))
                                {
#ifdef FEATURE_SUPPORT_VC0848                                                                
                                    audio_speaker_ctrl(1);
#else
                                    ;
#endif
                                }
                            
                            }
#endif
                            return;
                            
                        case AVK_FLIP_OPEN:
                            // �ϸǴ�
                            AEE_Event (EVT_FLIP, TRUE, 0);
                            break;
                            
                        case AVK_FLIP_CLOSE:
                            // �ϸǹ���
                            AEE_Event (EVT_FLIP, FALSE, 0);
                            if (NULL != gpBacklight)
                            {
                               IBACKLIGHT_TurnOff(gpBacklight);
                            }
#ifdef FEATRUE_DEVICETYPE_FLIP
                            if(brewui_isincall() == FALSE && cls != AEECLSID_CORE_APP && cls != AEECLSID_QUICKTEST)
                            {
                                ISHELL_CloseApplet(gpShell, TRUE);
                            }
                            else if(brewui_isincall() && cls != AEECLSID_DIALER)
                            {
                                ISHELL_PostEvent( gpShell,
                                                      AEECLSID_DIALER,
                                                      EVT_FLIP,
                                                      0,
                                                      0
                                                  );
                            }
#endif
                            break;
#ifdef FEATRUE_SUPPORT_G_SENSOR
                        case AVK_GSENSOR_FORWARD:
                        case AVK_GSENSOR_BACKWARD:
                            {
                                boolean b_FMBackground = FALSE;
                                dword shake;
                                
                                OEM_GetConfig(CFGI_GSENSOR,&shake,sizeof(shake));
                                OEM_GetConfig(CFGI_FM_BACKGROUND,&b_FMBackground, sizeof(b_FMBackground));

                                if((shake & OEMNV_SHAKE_SNOOZE_ALARM_MASK) && (TRUE == is_alarm_time_reach()))
                                {
                                    ISHELL_PostEvent( gpShell,
                                              AEECLSID_ALARMCLOCK,
                                              EVT_GSENSOR_SHAKE,
                                              key->aee_key,
                                              0);
                                }
                                else if((shake & OEMNV_SHAKE_FM_RADIO_MASK) && (TRUE == b_FMBackground))
                                {
                                    ISHELL_PostEvent( gpShell,
                                              AEECLSID_APP_FMRADIO,
                                              EVT_GSENSOR_SHAKE,
                                              key->aee_key,
                                              0);
                                }
                                else if((shake & OEMNV_SHAKE_MUSIC_MASK) && (!IsMp3PlayerStatusNone()))
                                {
                                    ISHELL_PostEvent( gpShell,
                                              AEECLSID_APP_MUSICPLAYER,
                                              EVT_GSENSOR_SHAKE,
                                              key->aee_key,
                                              0);
                                }
                                
                                else if(shake & OEMNV_SHAKE_WALLPAPER_MASK)
                                {
                                    ISHELL_PostEvent( gpShell,
                                            AEECLSID_CORE_APP,
                                            EVT_GSENSOR_SHAKE,
                                            key->aee_key,
                                            0);
                                }
                                break;
                            }
#endif                        
                        default:
                            break;
                    }
                    
                    (void) ISHELL_SendEvent(gpShell,
                                         cls,
                                         (uint16) key->aee_key,
                                         0,
                                         0);
                }
            }
            else 
            {
                route_brew_event(evt, (uint16) key->aee_key);

                if (evt == EVT_KEY_PRESS && key->bKeyEvtDown) 
                {
                    // �ڰ��������¼����ͺϳɵ�"KEY"�¼�
                    route_brew_event(EVT_KEY, (uint16) key->aee_key);
                }
            }
        }
    }
}

/*==============================================================================
����: 
    lookup_key
       
˵��: 
    ������������һ�� HS ������ OEMKeyTable ���еĶ�Ӧ�
       
����: 
    key [in]: HS ����
    
����ֵ:
    HS ������ OEMKeyTable ���еĶ�Ӧ��ĵ�ַ�����޶�Ӧ��� NULL ��
       
��ע:
    
==============================================================================*/
static const KeyEntry*  lookup_key(hs_key_type key)
{
    unsigned i;
    int  nMaxItems = sizeof(OEMKeyTable)/sizeof(OEMKeyTable[0]);
    
    for (i=0; i<nMaxItems; ++i) 
    {
        if (OEMKeyTable[i].hs_key == key) 
        {
            // ����ƥ����
            return &OEMKeyTable[i];
        }
    }
    
    // One last check for the wildcard
    if (OEMKeyTable[nMaxItems-1].hs_key == HS_NONE_K) 
    {
        // Return wildcard match
        return &OEMKeyTable[nMaxItems-1];
    }
    
    // ��ƥ����
    return NULL;
}


/*==============================================================================
����: 
    process_key_sig
       
˵��: 
    ���������������� HS task �İ����źš�
       
����: 
    none
    
����ֵ:
    none
       
��ע:
    
==============================================================================*/
static void process_key_sig(void)
{
#ifdef FEATURE_KEYPAD_MULTI_KEY
    keypad_key_event_type   key;
#else
    kpd_key_event_type      key;
#endif
    
    for (;;)
    {
#ifdef FEATURE_KEYPAD_MULTI_KEY
        key.key_code = get_key(&key);
#else
        key.key_code = (hs_key_type)get_key();
#endif
        
        if (HS_NONE_K == key.key_code)
        {
            break;
        }
        
        // ���尴������
        process_key(key);
    }
}

/*==============================================================================
����: 
    process_key_held_sig
       
˵��: 
    �����������������źš�
       
����: 
    none
    
����ֵ:
    none
       
��ע:
    
==============================================================================*/
static void process_key_held_sig(void)
{
//#ifdef FEATURE_KEYPAD_MULTI_KEY
//    keypad_key_event_type   key;
//#else
//    kpd_key_event_type      key;
//#endif
    
    // �������ȷ�������¼���Ч
    if (gKeyActionIndex < 0) 
    {
        return;
    }
    
    /*
    if (OEMKeyTable[gKeyActionIndex].kclass == KC_POWERHELD)
    {
        // ��װ���ܵ��˵�Դ�����¼�
        key.key_code = HS_PWR_K;
        
        process_key(key);
    }
    else
    */
    {
        // ���Ͱ��������¼�
        send_key_event(EVT_KEY_HELD, &OEMKeyTable[gKeyActionIndex]);
    }
}

boolean is_dummy_key(byte key_code, byte key_parm)
{
    switch(key_code)
    {
#ifdef FEATRUE_SUPPORT_G_SENSOR    
        case HS_GSENSOR_STEP_K:
            return TRUE;
#endif            
        case HS_RELEASE_K:
        {
            switch(key_parm)
            {
#ifdef FEATRUE_SUPPORT_G_SENSOR            
                case HS_GSENSOR_STEP_K:
                case HS_GSENSOR_FORWARD_K:   
                case HS_GSENSOR_BACKWARD_K:
                case HS_GSENSOR_STAND_K:
                case HS_GSENSOR_LEFT_K:
                case HS_GSENSOR_RIGHT_K:
                case HS_GSENSOR_UPEND_K:                    
                    return TRUE;
#endif                    
                default:
                    return FALSE;                   
            }
        }
        
        default:
            return FALSE;
    }

}


/*==============================================================================
����: 
    process_key
       
˵��: 
    ��������������尴����
       
����: 
    key [in]: HS �����¼�
    
����ֵ:
    none
       
��ע:
    
==============================================================================*/
#ifdef FEATURE_KEYPAD_MULTI_KEY
static void process_key(keypad_key_event_type   key)
#else
static void process_key(kpd_key_event_type      key)
#endif
{
    static KeyEntry *pKeyEntry = NULL;
#ifdef FEATURE_SUPPORT_VC0848
    db_items_value_type db_item;
#endif
    
    if (HS_NONE_K == key.key_code)
    {
        return;
    }
#ifdef FEATURE_SUPPORT_VC0848
    db_get(DB_BACKLIGHT_ON, &db_item);
#endif

    // ���⴦��
    if(FALSE == is_dummy_key(key.key_code, key.key_parm))//Don't turn on blight at dummy key event of g_sensor.
    {
        if (gpKeyBacklight) 
        {
            if (HS_FLIP_CLOSE_K == key.key_code)
            {
                IBACKLIGHT_TurnOff(gpKeyBacklight);
            }
            else if(HS_FLIP_OPEN_K == key.key_code)
            {
                IBACKLIGHT_TurnOn(gpKeyBacklight);
            }
            else
            {
                IBACKLIGHT_Enable(gpKeyBacklight);
            }
        }   
        
        if (gpBacklight)
        {
            if (HS_FLIP_CLOSE_K == key.key_code)
            {
                IBACKLIGHT_TurnOff(gpBacklight);
            }
            else if(HS_FLIP_OPEN_K == key.key_code)
            {
                IBACKLIGHT_TurnOn(gpBacklight);
            }
            else
            {
                if(FALSE == IBACKLIGHT_IsEnabled(gpBacklight))
                {
                    if(oemui_is_lcd_test_state(-1) == 1) //is lcd test  && HS_1_K == key.key_code
                    {
                        if(HS_1_K == key.key_parm && HS_RELEASE_K == key.key_code)
                        {
                            oemui_is_lcd_test_state(0);
                        }
                        else if(HS_RELEASE_K == key.key_code)
                        {
                        }
                        else
                        {
                            return;
                        }                    
                    }
                    else if(HS_HEADSET_K == key.key_code || HS_HEADSET_OFF_K == key.key_code 
#ifdef FEATURE_HEADSET_SWITCH
                        ||HS_HEADSET_SWITCH_K == key.key_code ||HS_HEADSET_SWITCH_OFF_K== key.key_code
#endif
                        ||HS_RELEASE_K == key.key_code)
                    {
                        IBACKLIGHT_Enable(gpBacklight);
                    }
#ifdef FEATURE_CARRIER_THAILAND_HUTCH
                else 
                {
                    IBACKLIGHT_Enable(gpBacklight);
                    pKeyEntry = (KeyEntry *)lookup_key(key.key_code);

                    if ( oemui_ispasskeyevent(pKeyEntry) )
                    {
                        VoteForSleep(FALSE);
                        
                        send_key_event(EVT_KEY_PRESS, pKeyEntry);
                        
                        gKeyActionIndex = -1;
                    }
                    return;
                }
#else //#ifdef FEATURE_CARRIER_THAILAND_HUTCH
                else
                {
                    IBACKLIGHT_Enable(gpBacklight);
#ifdef FEATURE_SUPPORT_VC0848
                       if(db_item.bBacklightOn == FALSE)
#endif /* FEATURE_SUPPORT_VC0848 */                     
                       {
                          return;
                       }
                }                
#endif //#ifdef FEATURE_CARRIER_THAILAND_HUTCH
                }
                else
                {
                    IBACKLIGHT_Enable(gpBacklight);
                }
            }
        }    
    }    
#ifdef FEATURE_SUPPORT_VC0848
    // ����Ƿ���Ҫ���Ͱ����¼�
    if (gbsend_keys == FALSE && db_item.bBacklightOn == FALSE)
#else
    if (gbsend_keys == FALSE)
#endif /* FEATURE_SUPPORT_VC0848 */
    {
        return;
    }
    
    switch (key.key_code)
    {
        case HS_EXT_PWR_ON_K:
        case HS_EXT_PWR_OFF_K:
            // ���ڲ���Ҫ֪ͨ BREW ��, OEMBatt.c ���ж�ʱ����ʱ��鲢֪ͨע���û�
            break;            

        case HS_RELEASE_K:
            // Check to see if we need to send a release notification
#ifdef FEATURE_KEYPAD_MULTI_KEY
            pKeyEntry = (KeyEntry *)lookup_key(key.key_parm);
#endif            
            if ((pKeyEntry != NULL) && 
                (pKeyEntry->kclass != KC_IGNORE))
            {
                // Must send a key release event to AEE
                send_key_event(EVT_KEY_RELEASE, pKeyEntry);
            }
            break;
            
        default:
            // ��鰴��ȷ���Ƿ���Ҫ����
            pKeyEntry = (KeyEntry *)lookup_key(key.key_code);
            
            // ����Ƿ���԰���
            if ((!pKeyEntry) || (pKeyEntry->kclass == KC_IGNORE))
            {
                break;
            }
            
            if (pKeyEntry->hs_key == HS_PWR_K)
            {
                db_items_value_type  db_item;
                
                db_get(DB_POWERUPTYPE, &db_item);
                if (db_item.db_poweruptype == DB_POWERUP_BYKEY)
                {
                    db_get(DB_PWKCLEARED, &db_item);
                    if (!db_item.db_pwkcleared)
                    {
                        break;
                    }
                }
            }
    
            VoteForSleep(FALSE);
            
            send_key_event(EVT_KEY_PRESS, pKeyEntry);
            
            gKeyActionIndex = -1;
            
            // Now see if we need to register the call back to detect held
            // and or repeat times
            switch (pKeyEntry->kclass) 
            {
                case KC_HELD:
                    // Look for key being held KEY_HELD_TIME
                    gKeyActionIndex = pKeyEntry-OEMKeyTable;
                    (void)rex_set_timer(&oemui_keyhold_timer, KEY_HELD_TIME);
                    break;

                /*
                case KC_POWERHELD:
                    // Look for key being held KEY_HELD_PWR_TIME
                    gKeyActionIndex = pKeyEntry-OEMKeyTable;
                    (void)rex_set_timer(&oemui_keyhold_timer, KEY_HELD_PWR_TIME);
                    break;
                */
                    
                case KC_OEMEVENT:
                case KC_NORMAL:
                case KC_IGNORE:
                default:
                    // nothing additional to do
                    break;
            }
            break;                
    }
}



/*==============================================================================
����: 
    oemui_shutdown_brew
       
˵��: 
    ���������˳� BREW , ������֮ǰ����ؽӿڵ���������
       
����: 
    none
    
����ֵ:
    none
       
��ע:
    
==============================================================================*/
static void  oemui_shutdown_brew(void)
{
    // �ͷ� Brew ��ؽӿ�
    if (gpBacklight) 
    {
        IBACKLIGHT_Release(gpBacklight);
        gpBacklight = NULL;
    }
    
    if (gpKeyBacklight) 
    {
        IBACKLIGHT_Release(gpKeyBacklight);
        gpKeyBacklight = NULL;
    }
    
    if (gpAlert) 
    {
        IALERT_Release(gpAlert);
        gpAlert = NULL;
    }

    if (gpSound) 
    {
        ISOUND_Release(gpSound);
        gpSound = NULL;
    }    

    //if (gpPhone) 
    //{
    //    IPHONE_Release(gpPhone);
    //    gpPhone = NULL;
    //}
    
    if (gpICM) 
    {
        ICM_Release(gpICM);
        gpICM = NULL;
    }
    
    // Shut down BREW
    if (gpShell) 
    {
        AEE_Exit();
        gpShell = NULL;
    }

    // Allow sleep 
    VoteForSleep(TRUE);
}


/*==============================================================================
����: 
    oemui_stop
       
˵��: 
    �������� oemui task �յ��� task stop �źš�
       
����: 
    none
    
����ֵ:
    none
       
��ע:
    
==============================================================================*/
static void oemui_stop(void)
{
#ifdef FEATURE_OTASP
    /* If MC initiates the power-down we will get the signal, before */
    /* we went through our power-down procedure. In that case we do  */
    /* not want to ack right now.                                    */
    if (ui_pwrdown_complete == TRUE)
    {
        task_stop();
        (void) rex_clr_sigs(rex_self(), TASK_STOP_SIG);

//        ISHELL_PostEventEx(gpShell, 
//                           EVTFLG_ASYNC, 
//                           AEECLSID_CORE_APP,
//                           EVT_USER,
//                           EVT_UI_EXIT,0L);

        /* Terminate BREW */
        oemui_shutdown_brew();

        ui_pwrdown_complete = FALSE ;    /* clear flag */
    }
    else
    {
        // TASK_STOP_SIG �źŻ�����������������
        (void) rex_set_sigs(rex_self(), TASK_STOP_SIG);
        
//        ISHELL_PostEventEx(gpShell, 
//                           EVTFLG_ASYNC, 
//                           AEECLSID_CORE_APP,
//                           EVT_USER,
//                           EVT_MC_PWR_DOWN,0L);
    }
#else /* FEATURE_OTASP */

    task_stop();

//    ISHELL_PostEventEx(gpShell, 
//                       EVTFLG_ASYNC, 
//                       AEECLSID_CORE_APP,
//                       EVT_USER,
//                       EVT_UI_EXIT,0L);
                       
    /* Terminate BREW */
    oemui_shutdown_brew();
#endif /* FEATURE_OTASP */
    
}

/*==============================================================================
����: 
    oemui_offline
       
˵��: 
    �������� oemui task �յ��� task offline �źš�
       
����: 
    none
    
����ֵ:
    none
       
��ע:
    
==============================================================================*/
static void oemui_offline(void)
{
//    ISHELL_PostEventEx(gpShell, 
//                       EVTFLG_ASYNC, 
//                       AEECLSID_CORE_APP,
//                       EVT_USER,
//                       EVT_TASK_OFFLINE,0L);
    //oemui_shutdown_brew();
    (void)rex_clr_timer(&oemui_keyhold_timer);
    AEE_Suspend();

    if (gbOffline != TRUE) 
    {
        lcd_message("OFFLINE"); 
    }
    
    gbOffline = TRUE;
    
    task_offline();
}

/*==============================================================================
����: 
    oemui_handlecmd
       
˵��: 
    ���������� oemui task �����
       
����: 
    none
    
����ֵ:
    none
       
��ע:
    
==============================================================================*/
static void oemui_handlecmd(ui_cmd_type *cmd_ptr)
{
    ERR("oemui_handlecmd:0X%x,",cmd_ptr->hdr.cmd,0,0);

    switch(cmd_ptr->hdr.cmd)
    {
#ifdef FEATURE_UIM_TOOLKIT
        case UI_PROACTIVE_UIM_F:
#ifdef FEATURE_UIM_TOOLKIT_UTK
#ifdef FEATURE_UTK2
            if (gpShell != NULL)
            {
                byte cmd_type;
                static boolean first_set_menu = TRUE;
                cmd_type = UTK_parse_proactive_command(cmd_ptr->proactive_cmd.cmd_data,
                               cmd_ptr->proactive_cmd.num_bytes);
                DBGPRINTF("UTK cmd_type = 0x%02x %d",cmd_type,first_set_menu,0);
                if(first_set_menu == TRUE && cmd_type == UIM_TK_SETUP_MENU)
                {
                    first_set_menu = FALSE;
                    return;
                }  
                (void)ISHELL_PostEvent(gpShell,
                                       AEECLSID_APP_UTK,
                                       (int)EVT_RUIM_PROACTIVE,
                                       cmd_type,
                                       0);
            }
#endif //FEATURE_UTK2
#endif
            break;
#endif

        default:
            ERR( "ui command 0X%x is ignored!", cmd_ptr->hdr.cmd, 0, 0 );
            break;
    } 
}

/*==============================================================================
����: 
    process_command_sig
       
˵��: 
    �������� oemui task �յ��������źš�
       
����: 
    none
    
����ֵ:
    none
       
��ע:
    
==============================================================================*/
static void process_command_sig(void)
{
    ui_cmd_type     *cmd_ptr;
    rex_tcb_type    *ctask_ptr;
    rex_sigs_type   csigs;
    
    // ������������е�����ֱ������Ϊ��
    while ((cmd_ptr = (ui_cmd_type *) q_get(&ui_cmd_q)) != NULL)
    {
        // ʵ�ʴ�������
        oemui_handlecmd(cmd_ptr);
        
        ctask_ptr = cmd_ptr->hdr.task_ptr;
        csigs = cmd_ptr->hdr.sigs;
        
#ifdef FEATURE_REX_DYNA_MEM_UI
        mem_free(&tmc_heap, cmd_ptr);
#else
        if (cmd_ptr->hdr.done_q_ptr)
        {
            q_put( cmd_ptr->hdr.done_q_ptr, &cmd_ptr->hdr.link );
        }
#endif        
        
        if (ctask_ptr)
        {
            (void) rex_set_sigs(ctask_ptr, csigs);
        }
    }
}

/*==============================================================================
����: 
    ui_key_event_cb
       
˵��: 
    Queue the keycode on the ring buffer, and signals the UI task. If there
    is no room in the buffer, an error is logged and the task is signalled
    anyway.
       
����: 
    none
    
����ֵ:
    none
       
��ע:
    
==============================================================================*/
#ifdef FEATURE_KEYPAD_MULTI_KEY
void ui_key_event_cb(keypad_key_event_type key_event)
#else
void ui_key_event_cb(kpd_key_event_type key_event)
#endif
{
    word isave;                   /* PSW parameter for INTLOCK/FREE_SAV    */

    // If the ring buffer is not full (full is when the write index is
    // circularly 1 less than the read index), put the keycode into the
    // ring buffer.
    INTLOCK_SAV(isave);

    if (((oemui_key_buffer.wr_idx + 1) & UI_KEY_BUF_MASK) != oemui_key_buffer.rd_idx) 
    {
        oemui_key_buffer.data[oemui_key_buffer.wr_idx] = key_event.key_code;
#ifdef FEATURE_KEYPAD_MULTI_KEY
        oemui_key_buffer.key_parm[oemui_key_buffer.wr_idx] = key_event.key_parm;
#endif
        oemui_key_buffer.wr_idx = (oemui_key_buffer.wr_idx+1) & UI_KEY_BUF_MASK;
    } 
    else 
    {
        ERR("Input buffer overflow (size %d)", UI_KEY_BUF_SIZE, 0, 0 );
    }

    INTFREE_SAV( isave );       /* Restore interrupts (PSW) */

    (void) rex_set_sigs( &ui_tcb, UI_KEY_SIG );

#ifdef FEATURE_KEYPAD_MULTI_KEY
    MSG_LOW("Keycode = 0x%x, Keyparm = 0x%x", key_event.key_code, key_event.key_parm, 0);
#else
    MSG_LOW("Keycode = 0x%x", key_event.key_code, 0, 0);
#endif
} 

/*==============================================================================
����: 
    ui_kpd_sub_cb
       
˵��: 
    Call back function to report the status of keypad servie subscription
       
����: 
    none
    
����ֵ:
    none
       
��ע:
    
==============================================================================*/
void ui_kpd_sub_cb(kpd_handle_type kpd_handle)
{
    ui_kpd_handle = kpd_handle;
}

/*==============================================================================
����: 
    oemui_keyinit
       
˵��: 
    Initializes the key handler and subscribes to the keypad key events
       
����: 
    none
    
����ֵ:
    none
       
��ע:
    
==============================================================================*/
void oemui_keyinit(void)
{
    kpd_sub_srvc(ui_kpd_sub_cb);
    
    if (ui_kpd_handle.status == HS_SUBSCRIBED) 
    {
        kpd_reg_key_event(ui_kpd_handle, ui_key_event_cb);
    }
    else 
    {
        ERR_FATAL("Couldn't subscribe to KEYPAD service.",0,0,0);
    }
}

/*==============================================================================
����: 
    ui_get_kpd_handle
       
˵��: 
    Gets the KPD_HANDLE
       
����: 
    none
    
����ֵ:
    kpd_handle_type
       
��ע:
    
==============================================================================*/
kpd_handle_type ui_get_kpd_handle(void)
{
    return ui_kpd_handle;
}


/*==============================================================================
����: 
    ui_get_lcd_handle
       
˵��: 
    Gets the LCD_HANDLE
       
����: 
    none
    
����ֵ:
    none
       
��ע:
    
==============================================================================*/
lcd_handle_type ui_get_lcd_handle(void)
{
    return oemui_lcd_handle;
}


/*==============================================================================
����: 
    oemui_lcd_sub_cb
       
˵��: 
    Handle LCD display driver call back.
       
����: 
    none
    
����ֵ:
    none
       
��ע:
    
==============================================================================*/
void oemui_lcd_sub_cb(lcd_handle_type lcd_handle)
{
    oemui_lcd_handle = lcd_handle;
    
    if (oemui_lcd_handle.status != HS_SUBSCRIBED)
    {
        ERR_FATAL("Couldn't subscribe to LCD service.",0,0,0);
    }
}


/*==============================================================================
����: 
    oemui_lcdinit
       
˵��: 
    Initializes the key handler and subscribes to the keypad key events
       
����: 
    none
    
����ֵ:
    none
       
��ע:
    
==============================================================================*/
void oemui_lcdinit(void)
{
    // Subscribe to LCD display driver services
    (void) lcd_sub_srvc(oemui_lcd_sub_cb);
}

#ifdef FEATURE_TOUCHPAD
boolean oemui_pen_callback(pen_event_type pen_event)
{
	rex_set_sigs(&ui_tcb,UI_GET_PEN_SIG);
	return TRUE;
}

static void oemui_touchpadinit(void)
{
    touchpad_register(oemui_pen_callback);
    touchpad_set_scan_mode( PEN_NORMAL_MODE);
}

static void oemui_post_pointer_data(void)
{
	pen_event_type pen_event;
	uint32 dwParam;
	AEEEvent evtCode;
	
	IBACKLIGHT_Enable(gpBacklight);
	VoteForSleep(FALSE);
    ERR("oemui_post_pointer_data",0,0,0);
    
	while(touchpad_get_pen_position(&pen_event))
	{
		if(touchpad_get_scan_mode() == PEN_NORMAL_MODE)
		{
			if(pen_event.pen_state == PEN_DOWN)
			{
				evtCode = EVT_POINTER_DOWN;
			}
			else if(pen_event.pen_state == PEN_MOVE)
			{
				evtCode = EVT_POINTER_MOVE;
			}
			else
			{
				evtCode = EVT_POINTER_UP;
			}
            AEE_POINTER_SEND_XY(evtCode,pen_event.pen_x,pen_event.pen_y);
		}
		else
		{
            ERR("EVENT Pen not in HW Mode",0,0,0);
	    }
	}
}

#endif

/*==============================================================================
����: 
    get_key
       
˵��: 
    Read a keycode from the input buffer.
       
����: 
    none
    
����ֵ:
    This procedure returns the oldest unread keycode.  If no keycode
    is currently available, HS_NONE_K is returned.
       
��ע:
    
==============================================================================*/
#ifdef FEATURE_KEYPAD_MULTI_KEY
byte    get_key(keypad_key_event_type *key_event_ptr)
#else
byte    get_key( void )
#endif
{
    byte keycode;

    if (oemui_key_buffer.wr_idx == oemui_key_buffer.rd_idx) 
    { /* buffer is empty */
        keycode = HS_NONE_K;
        
#ifdef FEATURE_KEYPAD_MULTI_KEY
        key_event_ptr->key_code = HS_NONE_K;
#endif
    } 
    else 
    { /* get code & increment ring read index */
        keycode = oemui_key_buffer.data[oemui_key_buffer.rd_idx];
        
#ifdef FEATURE_KEYPAD_MULTI_KEY
        key_event_ptr->key_code = oemui_key_buffer.data[oemui_key_buffer.rd_idx];
        key_event_ptr->key_parm = oemui_key_buffer.key_parm[oemui_key_buffer.rd_idx];
#endif
        oemui_key_buffer.rd_idx = (oemui_key_buffer.rd_idx+1) & UI_KEY_BUF_MASK;
    }
    
    return(keycode);
}

/*==============================================================================
����: 
    BrewUI_EnableKeys
       
˵��: 
    Tells the UI that the Coreapp is ready to start receiving keys
       
����: 
    flag [in]: �Ƿ񼤻��� UI ���Ͱ����¼��ı�־
    
����ֵ:
    none
       
��ע:
    
==============================================================================*/
void BrewUI_EnableKeys(boolean flag)
{
    gbsend_keys = flag;

    if (gbsend_keys == FALSE)
    {
#ifndef AEE_SIMULATOR
        keypad_lock_keypad(gbsend_keys);
#endif

        return;
    }
    
    // �������˲����Խ�����ֻ���һ�ο���ʱ���̱��ⲻϨ�������CR6935
    if (gpKeyBacklight) 
    {
        IBACKLIGHT_Enable(gpKeyBacklight);
    }
    if (gpBacklight) 
    {
        IBACKLIGHT_Enable(gpBacklight);
    }

    (void) rex_set_sigs (&ui_tcb, UI_KEY_SIG);
}

/*==============================================================================
����: 
    brewui_headsetconnected
       
˵��: 
    ���������жϵ�ǰ�����Ƿ������˶�����
       
����: 
    none
    
����ֵ:
    TRUE: ���������˶���
    FALSE: ����û�����˶���
       
��ע:
    Brew ����������

==============================================================================*/
boolean   brewui_headsetconnected(void)
{
    return FALSE;
}

/*==============================================================================
����: 
    brewui_isincall
       
˵��: 
    ���������жϵ�ǰ�����Ƿ���ͨ��״̬��
       
����: 
    none
    
����ֵ:
    TRUE: ��������ͨ��״̬
    FALSE: �������ڷ�ͨ��״̬
       
��ע:
    Brew ����������

==============================================================================*/
boolean   brewui_isincall(void)
{    
    if ((NULL == gpShell) || (NULL == gpICM))
    {
        return FALSE;
    }

    return AEECM_IS_VOICECALL_CONNECTED(gpICM);
}
#ifdef FEATURE_REX_DYNA_MEM_UI
/*==============================================================================
����: 
    ui_get_cmd
       
˵��: 
    ����������̬����һ�� ui ���� buffer ���ء�
       
����: 
    none
    
����ֵ:
    ui_cmd_type ָ�롣
       
��ע:
    REX ��NV �� DIAG �� HS ������������У�TMC heap is initialized.
    
==============================================================================*/
ui_cmd_type* ui_get_cmd(void)
{
    ui_cmd_type* cmd_ptr = NULL;
    
    cmd_ptr = (ui_cmd_type *)mem_malloc(&(tmc_heap), sizeof(ui_cmd_type));
    if (cmd_ptr == NULL)
    {
        ERR("Could not allocate memory for Call object!", 0, 0, 0);
    }
    else
    {
        // Initialize to a default value 
        cmd_ptr->hdr.cmd = UI_NUM_UI_COMMANDS;  
    }
    
    return cmd_ptr;
} 
#endif

/*==============================================================================
����: 
    ui_cmd
       
˵��: 
    Allows other tasks to queue up messages to the UI.  Grab a buffer from the
    ui_cmd_free_q, fill it in, and call ui_cmd with it.  All this will
    do is queue it and set a signal for the UI task.  When the command
    is done, the buffer will be queued as directed in the command header,
    and a task you specify may be signaled.
       
����: 
    cmd_ptr [in]: ui �����ָ��
    
����ֵ:
    none
       
��ע:
    REX ��NV �� DIAG �� HS ������������У�ui_cmd_q already initialized.
    
==============================================================================*/
void ui_cmd(ui_cmd_type *cmd_ptr)
{
    MSG_HIGH( "UI cmd %d", cmd_ptr->hdr.cmd, 0, 0 );
    
    // init link
    (void) q_link(cmd_ptr, &cmd_ptr->hdr.link);
    
    // and queue it
    q_put(&ui_cmd_q, &cmd_ptr->hdr.link);
    
    // signal the UI task
    (void) rex_set_sigs(&ui_tcb, UI_CMD_Q_SIG);
}

/*==============================================================================
����: 
    VoteForSleep
       
˵��: 
    This function determines to vote okay or not okay to sleep depending on the
    apps running. 
       
����: 
    flag [in]: TRUE-����˯��; FALSE-����˯��
    
����ֵ:
    none
       
��ע:
    
==============================================================================*/
void VoteForSleep(boolean flag)
{
    static boolean is_sleeping = FALSE;
    
    if (is_sleeping == flag) 
    {// �뵱ǰ״̬��ͬ������
        return;
    }
    
    is_sleeping = flag;
    
#ifdef FEATURE_SLEEP
    if (flag == FALSE)
    {
        // ����˯��
        UI_DISABLE_SLEEP();
    }
    else
    {
        // ����˯��
        UI_ENABLE_SLEEP();
    }
#endif    
}

#ifdef FEATURE_OTASP
/*==============================================================================
����: 
    VoteForSleep
       
˵��: 
    Set the ui_pwrdown_complete static variable. 
       
����: 
    flag [in]: TRUE-�ػ����; FALSE-�ػ���δ���
    
����ֵ:
    none
       
��ע:
    
==============================================================================*/
void oemui_setpwrdncomplete(boolean flag)
{
    ui_pwrdown_complete = flag;
}
#endif

/*===========================================================================
FUNCTION   oemui_cm_init

DESCRIPTION
  initialize UI-CM interface

DEPENDENCIES
  None

RETURNS
  None

SIDE EFFECTS
  None
===========================================================================*/
void oemui_cm_init(void)
{
    cm_client_status_e_type return_status;
    static boolean b_run = FALSE;
    if(b_run)
    {
        return;
    }
    b_run =  TRUE;
    return_status = cm_client_init(CM_CLIENT_TYPE_UI, &client_ui);
    
    if ( return_status != CM_CLIENT_OK)
    {
        ERR_FATAL("cm_client_init() returned bad status %d", return_status, 0, 0);
    }

    // register phone book - number classification functions

    cm_nc_reg(oemui_get_call_type, oemui_call_is_allowed, (cm_nc_pre_pending_dial_f_type*)NULL);
    
    (void) cm_client_act(client_ui);
    
    // This will set the RSSI Delta reporting threshold. Use 5 dBm
    (void) cm_ph_cmd_rssi_delta_threshold(NULL, NULL, client_ui, 5);
}

/*==============================================================================
����:
    initialize_imsi_components
       
˵��:
    This function computes the default for all IMSI components, and then
    writes them to NV. ACCOLC is also drived from the last digit of the
    IMSI.
       
����: 
    
    
����ֵ:
    None
       
��ע:

==============================================================================*/
void initialize_imsi_components(byte nam)
{
    nv_item_type nvi;
    
    nvi.imsi_addr_num.nam = nam;
    nvi.imsi_addr_num.num = IMSI_CLASS0_ADDR_NUM ;
    (void) OEMNV_Put( NV_IMSI_ADDR_NUM_I, &nvi ) ; 
}

#ifdef FEATURE_IS95B_TRUE_IMSI
/*==============================================================================
����:
    ui_default_imsi_t_s1
       
˵��:
    This function computes the default IMSI_T_S1 value as per IS-683 section 3.1.
       
����: 
    
    
����ֵ:
    This function returns the encoded value of IMSI_T_S1 with the four least
    significant digits set to ESNp, converted directly from binary to decimal,
    modulo 10000.  The other digits are set to zero.
       
��ע:
    Reading ESN from NV will change structure nvi.

==============================================================================*/
dword ui_default_imsi_t_s1(void)
{
    word zero;     /* Encoding of three zero digits */
    word fourth;   /* Fourth from last decimal digit of the ESN */
    word last3;    /* Last three decimal digits of the ESN */
    nv_item_type nvi;

    (void) OEMNV_Get( NV_ESN_I, &nvi );   /* CFGI_ESN */
    /* Encode digits as per JSTD-008 section 2.3.1.1 */
    zero = 1110 - 111;
    last3 = (nvi.esn.esn % 1000);
    last3 += ((last3 / 100) == 0) ? 1000 : 0;
    last3 += (((last3 % 100) / 10) == 0) ? 100 : 0;
    last3 += ((last3 % 10) == 0) ? 10 : 0;
    last3 -= 111;
    fourth = ((nvi.esn.esn % 10000) / 1000); /* In range 0-9 */
    if (fourth == 0)
    {
        fourth = 10;
    }

    /* Concatenate results and return 24 bit value for imsi_s1 */
    /* Example: esn = 120406
    **      imsi_s1 = 000  0  406
    **  encodes to -> 999  10 395
    **       in hex = 3e7  a  18b
    **    in binary = 1111100111 1010 0110001011
    */
    return (((dword) zero << 14) | (fourth << 10) | last3);
}


/*===========================================================================

FUNCTION UI_INITIALIZE_IMSI_T_COMPONENTS

DESCRIPTION
  This function computes the default for all TRUE IMSI components, and then
  writes them to NV.

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/
static void initialize_imsi_t_components(byte nam)
{
    nv_item_type nvi;

    /* Default MCC is 000 in ASCII and 999 in min */
    nvi.imsi_t_mcc.nam = nam;
    nvi.imsi_t_mcc.imsi_mcc = IMSI_MCC_0;      /* default to '000' */
    (void) OEMNV_Put( NV_IMSI_T_MCC_I, &nvi );   /* CFGI_IMSI_T_MCC */

    /* Default MNC is 00 in ASCII and 99 in min */
    nvi.imsi_t_11_12.nam = nam;
    nvi.imsi_t_11_12.imsi_11_12 = IMSI_11_12_0;  /* default to '00' */
    (void) OEMNV_Put( NV_IMSI_T_11_12_I, &nvi );   /* CFGI_IMSI_T_11_12 */

    nvi.imsi_t_addr_num.nam = nam;
    nvi.imsi_t_addr_num.num = IMSI_CLASS0_ADDR_NUM ;
    OEMNV_Put( NV_IMSI_T_ADDR_NUM_I, &nvi ) ;   /* CFGI_IMSI_T_ADDR_NUM */

    /* Default S2 is 000 in ASCII and 999 in min */
    nvi.imsi_t_s2.nam = nam;
    nvi.imsi_t_s2.min2[FMMIN] = IMSI_S2_0 ;
    nvi.imsi_t_s2.min2[CDMAMIN] = IMSI_S2_0 ;
    OEMNV_Put( NV_IMSI_T_S2_I, &nvi ) ;   /* CFGI_IMSI_T_S2 */

#ifdef  FEATURE_IS95B_TRUE_IMSI
    nvi.imsi_t_s1.min1[CDMAMIN] =  ui_default_imsi_t_s1() ;
#endif
    /* The following assignment must not be done, before the function above */
    /* because that function will read ESN which will corrupt the min1.nam. */
    nvi.imsi_t_s1.nam = nam;
    nvi.imsi_t_s1.min1[FMMIN] = IMSI_S1_0 ;
    OEMNV_Put(NV_IMSI_T_S1_I, &nvi) ;   /* CFGI_IMSI_T_S1 */
}
#endif //FEATURE_IS95B_TRUE_IMSI


#ifdef FEATURE_SSPR_ENHANCEMENTS
/*===========================================================================

FUNCTION IS_CELLULAR_SYS_A_CHANNEL

DESCRIPTION
  Check if the CDMA channel is with Cellular Band System A.

DEPENDENCIES
  None

RETURN VALUE
  boolean: TRUE - if the channel is within Cellular Band System A
           FALSE - otherwise.

SIDE EFFECTS
  None

===========================================================================*/
static boolean is_cellular_sys_a_channel
(
  word cdma_channel       /* The channel to examine */
)
{
#ifdef FEATURE_T53_RF

    /* According to T53 Section 6.1.1.1, the CDMA channel must be even, and
     * be in the range of 51-749, 851-989, or 1091-1149.
     */
    if (((cdma_channel & 0x0001) == 0) &&
        (((cdma_channel > 51)   && (cdma_channel <  749))  ||
        ((cdma_channel > 851)  && (cdma_channel <  989))  ||
        ((cdma_channel > 1091) && (cdma_channel < 1149))))
    {
        return TRUE;
    }
    else
    {
        return FALSE;
    }
#else
    return (PRL_IS_CHAN_IN_CELLULAR_SYS_A(cdma_channel)); /* Sys A */
#endif /* FEATURE_T53_RF */
} /* is_cellular_sys_a_channel */

/*===========================================================================

FUNCTION IS_CELLULAR_SYS_B_CHANNEL


DESCRIPTION
  Check if the CDMA channel is with Cellular Band System B

DEPENDENCIES
  None

RETURN VALUE
  boolean: TRUE - if the channel is within Cellular Band System B
           FALSE - otherwise.

SIDE EFFECTS
  None

===========================================================================*/
static boolean is_cellular_sys_b_channel
(
  word cdma_channel       /* The channel to examine */
)
{
#ifdef FEATURE_T53_RF
    /* T53 doesn't use B-side cellular, so return FALSE */
    return FALSE;
#else
    return (PRL_IS_CHAN_IN_CELLULAR_SYS_B(cdma_channel)); /* Sys B */
#endif /* FEATURE_T53_RF */
} /* is_cellular_sys_b_channel */
#endif


/*===========================================================================
FUNCTION InitProvisioning

DESCRIPTION
  initialize provisioning

DEPENDENCIES
  the corresponding initialization functions

SIDE EFFECTS
  None
===========================================================================*/
void InitProvisioning(void)
{
    boolean good=TRUE, goodnam=TRUE;
    
#if defined(FEATURE_CDMA_800) || defined(FEATURE_CDMA_1900)
    nv_item_type nvi;
    int i,j;
    uint8 nam;

    boolean hascdma[NV_MAX_NAMS], hasfm[NV_MAX_NAMS];
    boolean hasacdma[NV_MAX_NAMS], hasbcdma[NV_MAX_NAMS];

    /* use these flags to treat all IMSI components as one */
    boolean initialize_imsi[NV_MAX_NAMS] ;
    boolean imsi_all_zeros[NV_MAX_NAMS] ;

#ifdef FEATURE_IS95B_TRUE_IMSI
    boolean initialize_imsi_t[NV_MAX_NAMS];
    boolean imsi_t_all_zeros[NV_MAX_NAMS];
#endif

    for (i=0; i<NV_MAX_NAMS; i++)
    {
        initialize_imsi[i] = FALSE ; /* assume no need */
        imsi_all_zeros[i] = TRUE ;   /* assume true    */
    }

#ifdef FEATURE_IS95B_TRUE_IMSI
    for (i=0; i<NV_MAX_NAMS; i++)
    {
        initialize_imsi_t[i] = FALSE ; /* assume no need */
        imsi_t_all_zeros[i] = TRUE ;   /* assume true    */
    }
#endif // FEATURE_IS95B_TRUE_IMSI

    (void) OEMNV_Get(NV_ESN_CHKSUM_I, &nvi);   /* CFGI_ESN_CHKSUM */
    if (nvi.esn_chksum.chksum != NV_VALID_ESN_CHKSUM)
    {
        MSG_HIGH("BAD ESN CHECKSUM",0,0,0);
        good = FALSE;
    }

    /* Get all the NAM/MIN information that we need */
    (void) OEMNV_Get( NV_CURR_NAM_I, &nvi ); /* get current nam */   /* CFGI_CURR_NAM */
    /* If the NAM is out of range, default to the first NAM */
    if ( nvi.curr_nam >= NV_MAX_NAMS )
    {
        nvi.curr_nam = 0;
        (void) OEMNV_Put ( NV_CURR_NAM_I, &nvi );   /* CFGI_CURR_NAM */
    }

    nam = nvi.curr_nam;

    for (i=0; i<NV_MAX_NAMS; i++)
    {
        hascdma[i] = TRUE; /* Assume the NAM is good for CDMA */
#ifdef FEATURE_ACP
        hasfm[i] = TRUE;  /* Assume NAM is good for AMPS */
#else
        hasfm[i] = FALSE; /* No FEATURE_ACP, No AMPS! */
#endif /* FEATURE_ACP */
#ifdef FEATURE_ACP
        /*********************************/
        /* Determine if we have valid FM */
        /*********************************/
        nvi.analog_home_sid.nam = (byte) i; /* has analog if home_sid != 0 */
        /* if inactive or has invalid range ( i.e > 32767 */
        if ( NV_NOTACTIVE_S == OEMNV_Get( NV_ANALOG_HOME_SID_I, &nvi ) ||
            nvi.analog_home_sid.sid &0x8000 )   /* CFGI_AMPS_HOME_SID */
        {
            /* then initialize it */
            nvi.analog_home_sid.sid = 0 ;
            (void) OEMNV_Put( NV_ANALOG_HOME_SID_I, &nvi );   /* CFGI_AMPS_HOME_SID */
        }
#endif /* FEATURE_ACP */

#ifdef FEATURE_CDMA_800
        /***********************************/
        /* Determine if we have valid CDMA */
        /***********************************/
        nvi.pcdmach.nam = (byte) i;  /* has CDMA if any of the 8 channels set */
        {
            boolean need_nv_save = FALSE;
            if ( NV_NOTACTIVE_S == OEMNV_Get( NV_PCDMACH_I, &nvi ) )   /* CFGI_PRI_CH_A */
            {
                nvi.pcdmach.nam = i;
                nvi.pcdmach.channel_a = PCH_A_DEFAULT;
                nvi.pcdmach.channel_b = PCH_B_DEFAULT;
                need_nv_save = TRUE;
            }
#ifdef FEATURE_SSPR_ENHANCEMENTS
            else
            {
                if (!is_cellular_sys_a_channel(nvi.pcdmach.channel_a))
                {
                    /* Channel A number is invalid, default it */
                    nvi.pcdmach.channel_a = PCH_A_DEF;
                    need_nv_save = TRUE;
                }

                if (!is_cellular_sys_b_channel(nvi.pcdmach.channel_b))
                {
                    /* Channel B number is invalid, default it */
                    nvi.pcdmach.channel_b = PCH_B_DEF;
                    need_nv_save = TRUE;
                }
            }
#endif
            /* If any of the value changed, update NV */
            if (need_nv_save)
            {
                (void) OEMNV_Put( NV_PCDMACH_I, &nvi );   /* CFGI_PRI_CH_A, CFGI_PRI_CH_B */
            }
        }

        hasacdma[i] = (boolean) nvi.pcdmach.channel_a;
        hasbcdma[i] = (boolean) nvi.pcdmach.channel_b;
        nvi.scdmach.nam = (byte) i;
        {
            boolean need_nv_save = FALSE;
            if ( NV_NOTACTIVE_S == OEMNV_Get( NV_SCDMACH_I, &nvi ) )   /* CFGI_SEC_CH_A, CFGI_SEC_CH_B */
            {
                nvi.scdmach.nam = i;
                nvi.scdmach.channel_a = SCH_A_DEFAULT;
                nvi.scdmach.channel_b = SCH_B_DEFAULT;
                need_nv_save = TRUE;
            }
#ifdef FEATURE_SSPR_ENHANCEMENTS
            else
            {
                if (!is_cellular_sys_a_channel(nvi.scdmach.channel_a))
                {
                    nvi.scdmach.channel_a = SCH_A_DEF;
                    need_nv_save = TRUE;
                }

                if (!is_cellular_sys_b_channel(nvi.scdmach.channel_b))
                {
                    nvi.scdmach.channel_b = SCH_B_DEF;
                    need_nv_save = TRUE;
                }
            }
#endif
            if (need_nv_save)
            {
                (void) OEMNV_Put( NV_SCDMACH_I, &nvi );   /* CFGI_SEC_CH_A, CFGI_SEC_CH_B */
            }
        }

        if (hasacdma[i])
        {
            hasacdma[i] = (boolean) nvi.scdmach.channel_a;
        }
        if (hasbcdma[i])
        {
            hasbcdma[i] = (boolean) nvi.scdmach.channel_b;
        }

        hascdma[i] = hasacdma[i] | hasbcdma[i];
#endif /* FEATURE_CDMA_800 */

#if defined(FEATURE_SSPR_ENHANCEMENTS)
        {
            byte indx;
            boolean need_nv_save = FALSE;

            /**********************************/
            /* Check for a valid SID/NID List */
            /**********************************/
            nvi.home_sid_nid.nam = (byte) i;
            /* Is it active and in valid range */
            if ( NV_NOTACTIVE_S == OEMNV_Get( NV_HOME_SID_NID_I, &nvi ) )   /* CFGI_HOME_SIDNID_LIST */
            {
                for( indx=0; indx<NV_MAX_HOME_SID_NID; indx++)
                {
                    /*  NID default */
                    nvi.home_sid_nid.pair[indx].sid = 0;
                    nvi.home_sid_nid.pair[indx].nid = NID_DEFAULTS ;
                }
                need_nv_save = TRUE;
            }
            else
            {
                for( indx=0; indx<NV_MAX_HOME_SID_NID; indx++)
                {
                    /* Is range valid ? */
                    if ( nvi.home_sid_nid.pair[indx].sid & 0x8000 )
                    {
                        /*  NID default */
                        nvi.home_sid_nid.pair[indx].sid = 0;
                        nvi.home_sid_nid.pair[indx].nid = NID_DEFAULTS ;
                        need_nv_save = TRUE;
                    }
                }
            }
            if (need_nv_save)
            {
                (void) OEMNV_Put( NV_HOME_SID_NID_I, &nvi );   /* CFGI_HOME_SIDNID_LIST */
            }
#if (defined (FEATURE_CDMA_800) && defined (FEATURE_CDMA_1900) )
            /* SID/NID Lockout not allowed for TriMode */
#else

            /******************************************/
            /* Check for a valid Lockout SID/NID List */
            /******************************************/

            nvi.sid_nid_lock.nam = (byte) i;
            need_nv_save = FALSE;
            /* Is it active and in valid range */
            if ( NV_NOTACTIVE_S == OEMNV_Get( NV_SID_NID_LOCK_I, &nvi ) )   /* CFGI_LOCK_SIDNID_LIST */
            {
                for( indx=0; indx<NV_MAX_SID_NID_LOCK; indx++)
                {
                    /*  NID default */
                    nvi.sid_nid_lock.pair[indx].sid = 0;
                    nvi.sid_nid_lock.pair[indx].nid = NID_DEFAULTS ;
                }
                need_nv_save = TRUE;
            }
            else
            {
                for( indx=0; indx<NV_MAX_SID_NID_LOCK; indx++)
                {
                    /* Is range valid ? */
                    if ( nvi.sid_nid_lock.pair[indx].sid & 0x8000 )
                    {
                        /*  NID default */
                        nvi.sid_nid_lock.pair[indx].sid = 0;
                        nvi.sid_nid_lock.pair[indx].nid = NID_DEFAULTS ;
                        need_nv_save = TRUE;
                    }
                }
            }
            if (need_nv_save)
            {
                (void) OEMNV_Put( NV_SID_NID_LOCK_I, &nvi );   /* CFGI_LOCK_SIDNID_LIST */
            }
#endif /* TriMode */
        }
#else
        {
            int sid, issid;
            boolean need_nv_save = FALSE;

            /*************************/
            /* Check for a valid SID */
            /*************************/
            nvi.sid_nid.nam = (byte) i;
            /* Is it active and in valid range */
            if ( NV_NOTACTIVE_S == OEMNV_Get( NV_SID_NID_I, &nvi ) )   /* None */
            {
                for( sid=0; sid<NV_MAX_SID_NID; sid++)
                {
                /*  NID default */
                nvi.sid_nid.pair[NV_CDMA_MIN_INDEX][sid].sid = 0;
                nvi.sid_nid.pair[NV_CDMA_MIN_INDEX][sid].nid = NID_DEFAULTS ;
                }
                need_nv_save = TRUE;
            }
            else
            {
                for( sid=0; sid<NV_MAX_SID_NID; sid++)
                {
                    /* Is range valid ? */
                    if ( nvi.sid_nid.pair[NV_CDMA_MIN_INDEX][i].sid & 0x8000 )
                    {
                        /*  NID default */
                        nvi.sid_nid.pair[NV_CDMA_MIN_INDEX][sid].sid = 0;
                        nvi.sid_nid.pair[NV_CDMA_MIN_INDEX][sid].nid = NID_DEFAULTS ;
                        need_nv_save = TRUE;
                    }
                }
            }
            if (need_nv_save)
            {
                (void) OEMNV_Put( NV_SID_NID_I, &nvi );   /* None */
            }
            issid = 0;
            for( sid=0; sid<NV_MAX_SID_NID; sid++ )
            {
                issid |= (int)nvi.sid_nid.pair[NV_CDMA_MIN_INDEX][sid].sid;
            }
        }

#if defined(FEATURE_CDMA_800)
        /******************************************/
        /* Check for a valid Lockout SID List */
        /******************************************/
        {
            byte indx;
            boolean need_nv_save = FALSE;

            nvi.cdma_sid_lock.nam = (byte) i;
            /* Is it active and in valid range */
            if ( NV_NOTACTIVE_S == OEMNV_Get( NV_CDMA_SID_LOCK_I, &nvi ) )   /* CFGI_CDMA_SID_LOCK */
            {
                for( indx=0; indx<NV_MAX_SID_LOCK; indx++)
                {
                    /*  default */
                    nvi.cdma_sid_lock.sid[indx] = 0;
                }
                need_nv_save = TRUE;
            }
            else
            {
                for( indx=0; indx<NV_MAX_SID_NID; indx++)
                {
                    /* Is range valid ? */
                    if ( nvi.cdma_sid_lock.sid[indx] & 0x8000 )
                    {
                        /*  default */
                        nvi.cdma_sid_lock.sid[indx] = 0;
                        need_nv_save = TRUE;
                    }
                }
            }
            if (need_nv_save)
            {
                (void) OEMNV_Put( NV_CDMA_SID_LOCK_I, &nvi );   /* CFGI_CDMA_SID_LOCK */
            }
        }
#endif /* FEATURE_CDMA_800 */
#endif /* FEATURE_SSPR_ENHANCEMENTS */
#ifdef FEATURE_SSPR_800
        /******************************************/
        /* Check for a valid PRL Enabled          */
        /******************************************/
        nvi.prl_enabled.nam = (byte) i;

#if (defined (FEATURE_CDMA_800) && defined (FEATURE_CDMA_1900) )
        nvi.prl_enabled.enabled = TRUE;
        (void) OEMNV_Put( NV_PRL_ENABLED_I, &nvi );   /* CFGI_PRL_ENABLED */
#else
        /* Is it active and in valid range */
        if ( NV_NOTACTIVE_S == OEMNV_Get( NV_PRL_ENABLED_I, &nvi ) )   /* CFGI_PRL_ENABLED */
        {
            nvi.prl_enabled.enabled = FALSE;
            (void) OEMNV_Put( NV_PRL_ENABLED_I, &nvi );   /* CFGI_PRL_ENABLED */
        }
#endif /* TriMode */
#endif /* FEATURE_SSPR_800 */

#if (defined (FEATURE_CDMA_800) && defined (FEATURE_CDMA_1900) )
        /*--- It is done in ID_S minor state ---*/
#else
#if defined( FEATURE_ACP ) && defined( FEATURE_CDMA_800 )
        if (!hascdma[i])
        {
            sys_plmn_id_s_type    plmn;
            sys_plmn_undefine_plmn_id(&plmn);

            /*
            ** There is NO valid CDMA. In this case we default the mode
            ** to AUTOMATIC.  Note in this, phone goes offline.
            */
            (void) cm_ph_cmd_sys_sel_pref(
                            NULL,
                            NULL,
                            client_ui,
                            CM_MODE_PREF_AUTOMATIC,
                            CM_PREF_TERM_PERMANENT,
                            0,
                            CM_GW_ACQ_ORDER_PREF_NO_CHANGE,
                            CM_BAND_PREF_CELL,
                            CM_ROAM_PREF_NO_CHANGE,
                            CM_HYBR_PREF_NO_CHANGE,
                            CM_SRV_DOMAIN_PREF_NO_CHANGE,
                            CM_NETWORK_SEL_MODE_PREF_NO_CHANGE,
                            &plmn
                            );
        }
        else
        {
            if (!hasfm[i]))
            {
                sys_plmn_id_s_type    plmn;
                sys_plmn_undefine_plmn_id(&plmn);
    
                /*
                ** There is a valid CDMA. In this case we default the mode to
                ** DIGITAL_ONLY if there is NO valid FM.
                */
                (void) cm_ph_cmd_sys_sel_pref(
                                NULL,
                                NULL,
                                client_ui,
                                CM_MODE_PREF_DIGITAL_ONLY,
                                CM_PREF_TERM_PERMANENT,
                                0,
                                CM_GW_ACQ_ORDER_PREF_NO_CHANGE,
                                CM_BAND_PREF_CELL,
                                CM_ROAM_PREF_NO_CHANGE,
                                CM_HYBR_PREF_NO_CHANGE,
                                CM_SRV_DOMAIN_PREF_NO_CHANGE,
                                CM_NETWORK_SEL_MODE_PREF_NO_CHANGE,
                                &plmn
                                );
            }
        }

#ifndef FEATURE_SSPR_ENHANCEMENTS
        nvi.cdma_pref_serv.nam = i ;
        /* is it active and in valid range ? */
        if ( NV_NOTACTIVE_S == OEMNV_Get( NV_CDMA_PREF_SERV_I, &nvi ) ||
            nvi.cdma_pref_serv.sys > NV_SYS_HOME_PREF )   /* None */
        {
            /* no, so default it */
            nvi.cdma_pref_serv.nam = i ;
            nvi.cdma_pref_serv.sys = NV_SYS_HOME_PREF;
            (void) OEMNV_Put(NV_CDMA_PREF_SERV_I, &nvi);   /* None */
        }

        nvi.analog_pref_serv.nam = i ;
        /* is it active and in valid range ? */
        if ( NV_NOTACTIVE_S == OEMNV_Get( NV_ANALOG_PREF_SERV_I, &nvi ) ||
               nvi.analog_pref_serv.sys > NV_SYS_HOME_PREF )   /* None */
        {
            /* no, so default it */
            nvi.analog_pref_serv.nam = i ;
            nvi.analog_pref_serv.sys = NV_SYS_HOME_PREF;
            (void) OEMNV_Put(NV_ANALOG_PREF_SERV_I, &nvi);   /* None */
        }
#endif /* FEATURE_SSPR_ENHANCEMENTS */
#endif /* defined( FEATURE_ACP ) && defined( FEATURE_CDMA_800 ) */
#endif /* (defined (FEATURE_CDMA_800) && defined (FEATURE_CDMA_1900) ) */

        /**********************************************/
        /* Make sure MCC and IMSI_11_12 are set       */
        /**********************************************/
        nvi.imsi_mcc.nam = i;
        if ( NV_NOTACTIVE_S == OEMNV_Get( NV_IMSI_MCC_I, &nvi ) )   /* CFGI_IMSI_MCC */
        {
            initialize_imsi[i] = TRUE ;
        }
        else
        {
            if ( nvi.imsi_mcc.imsi_mcc > IMSI_MCC_0 )
            {
                initialize_imsi[i] = TRUE ; /* invalid range */
            }
            if ( nvi.imsi_mcc.imsi_mcc != IMSI_MCC_0 )
            {
                imsi_all_zeros[i] = FALSE ;
            }
        }

        nvi.imsi_11_12.nam = i;
        if ( NV_NOTACTIVE_S == OEMNV_Get( NV_IMSI_11_12_I, &nvi ) )   /* CFGI_IMSI_11_12 */
        {
            initialize_imsi[i] = TRUE ;
        }
        else
        {
            if ( nvi.imsi_11_12.imsi_11_12 > IMSI_11_12_0 )
            {
              initialize_imsi[i] = TRUE ; /* invalid range */
            }
            if ( nvi.imsi_11_12.imsi_11_12 != IMSI_11_12_0 )
            {
              imsi_all_zeros[i] = FALSE ;
            }
        }

        nvi.imsi_addr_num.nam = i;
        if ( NV_NOTACTIVE_S == OEMNV_Get( NV_IMSI_ADDR_NUM_I, &nvi ) )   /* None */
        {
            initialize_imsi[i] = TRUE ;
        }

#ifdef FEATURE_IS95B_TRUE_IMSI
        /*  Add five TRUE NV items checking
        **               IMSI_T_S1_I,
        **               IMSI_T_S2_I,
        **               IMSI_T_MCC_I,
        **               IMSI_T_11_12_I,
        **               IMSI_T_ADDR_NUM_I
        **
        */

        MSG_MED ("Reading NV_IMSI_T_S1_I", 0, 0, 0);
        /* Loop through the number of NAMs-1 and read the IMSI_T_S1 for each NAM */
        /* (NOTE:  We are looping to queue NV reads via uinv_access() with
        ** a NULL as the callback, then we queue the last NV read for the
        ** last NAM with this same routine (uicall_strtup_checking ()) as
        ** the callback */

        /*************************/
        /* Read the IMSI_T_S1 from NV */
        /*************************/

        /* Get and validate NV_IMSI_T_S1_I code goes here */
        nvi.imsi_t_s1.nam = (byte) i;     /* get the MINs for the NAM */
        if ( NV_NOTACTIVE_S == OEMNV_Get( NV_IMSI_T_S1_I, &nvi ) )   /* CFGI_IMSI_T_S1 */
        {
            initialize_imsi_t[i] = TRUE ;
        }
        else
        {
            /* Since it is valid for only CDMA, only CDMA MIN is checked. FMMIN is ignored */
            if ( ( ( nvi.imsi_t_s1.min1[CDMAMIN] & 0xFF000000 ) != 0 ) ||
                 ( ( ( nvi.imsi_t_s1.min1[CDMAMIN] & 0x00FFC000 ) >> 14 ) > 999 ) ||
                 ( ( ( nvi.imsi_t_s1.min1[CDMAMIN] & 0x00003C00 ) >> 10 ) > 10 ) ||
                 ( ( ( nvi.imsi_t_s1.min1[CDMAMIN] & 0x00003C00 ) >> 10 ) == 0 ) ||
                 ( ( ( nvi.imsi_t_s1.min1[CDMAMIN] & 0x000003FF ) > 999 ) ) )
            {
                /* Invalid Range See IS-95A section 6.3.1 */
                initialize_imsi_t[i] = TRUE ;
            }
            if ( nvi.imsi_t_s1.min1[CDMAMIN] != IMSI_S1_0 )
            {
                imsi_t_all_zeros[i] = FALSE ;
            }
        }

        nvi.imsi_t_s2.nam = (byte) i;     /* get the MINs for the NAM */
        if ( NV_NOTACTIVE_S == OEMNV_Get( NV_IMSI_T_S2_I, &nvi ) )   /* CFGI_IMSI_T_S2 */
        {
            initialize_imsi_t[i] = TRUE ;
        }
        else
        {
            /* Since it is valid for only CDMA, only CDMA MIN is checked. FMMIN is ignored */
            if ( nvi.imsi_t_s2.min2[CDMAMIN] > IMSI_S2_0 )
            {
                /* invalid range */
                initialize_imsi_t[i] = TRUE ;
            }
            if ( nvi.imsi_t_s2.min2[CDMAMIN] != IMSI_S2_0 )
            {
                imsi_t_all_zeros[i] = FALSE ;
            }
        }

        nvi.imsi_t_11_12.nam = (byte) i;     /* get the MINs for the NAM */
        if ( NV_NOTACTIVE_S == OEMNV_Get( NV_IMSI_T_11_12_I, &nvi ) )   /* CFGI_IMSI_T_11_12 */
        {
            initialize_imsi_t[i] = TRUE ;
        }
        else
        {
            if ( nvi.imsi_t_11_12.imsi_11_12 > IMSI_11_12_0 )
            {
                initialize_imsi_t[i] = TRUE ; /* invalid range */
            }
            if ( nvi.imsi_t_11_12.imsi_11_12 != IMSI_11_12_0 )
            {
                imsi_t_all_zeros[i] = FALSE ;
            }
        }

        nvi.imsi_t_mcc.nam = (byte) i;     /* get the MINs for the NAM */
        if ( NV_NOTACTIVE_S == OEMNV_Get( NV_IMSI_T_MCC_I, &nvi ) )   /* CFGI_IMSI_T_MCC */
        {
            initialize_imsi_t[i] = TRUE ;
        }
        else
        {
            if ( nvi.imsi_t_mcc.imsi_mcc > IMSI_MCC_0 )
            {
                initialize_imsi_t[i] = TRUE ; /* invalid range */
            }
            if ( nvi.imsi_t_mcc.imsi_mcc != IMSI_MCC_0 )
            {
                imsi_t_all_zeros[i] = FALSE ;
            }
        }

        nvi.imsi_t_addr_num.nam = (byte) i;     /* get the MINs for the NAM */
        if ( NV_NOTACTIVE_S == OEMNV_Get( NV_IMSI_T_ADDR_NUM_I, &nvi ) )   /* CFGI_IMSI_T_ADDR_NUM */
        {
            initialize_imsi_t[i] = TRUE ;
        }
        else
        if (!initialize_imsi_t[i])
        {
            /* Check if the IMSI_T_ADDR_NUM is invalid */
            if ((nvi.imsi_t_addr_num.num > 7) &&
               (nvi.imsi_t_addr_num.num != 0xFF))
            {
                ERR ("Invalid IMSI_T_ADDR_NUM for NAM %d", i, 0, 0);

                /* Set the flag that we need to default this NAM's IMSI data */
                initialize_imsi_t[i] = TRUE;
            }
        }
#endif /*** End of FEATURE_IS95B_TRUE_IMSI ***/

        /**********************************/
        /* Read out the MINs for each NAM */
        /**********************************/
        nvi.min1.nam = (byte) i;     /* get the MINs for the NAM */
        if ( NV_NOTACTIVE_S == OEMNV_Get( NV_MIN1_I, &nvi ) )   /* CFGI_IMSI_S */
        {
            initialize_imsi[i] = TRUE ;
        }
        else
        {
            /* Loop through the number of MINs */
            for (j = 0; j < NV_MAX_MINS; j++)
            {
                if (((nvi.min1.min1[j] & 0xFF000000 ) != 0) ||
                    (((nvi.min1.min1[j] & 0x00FFC000 ) >> 14) > 999) ||
                    (((nvi.min1.min1[j] & 0x00003C00 ) >> 10) > 10) ||
                    (((nvi.min1.min1[j] & 0x00003C00 ) >> 10) == 0) ||
                    (((nvi.min1.min1[j] & 0x000003FF ) > 999)))
                {
                    /* Invalid Range See IS-95A section 6.3.1 */
                    initialize_imsi[i] = TRUE ;
                }
                if ( nvi.min1.min1[j] != IMSI_S1_0 )
                {
                    imsi_all_zeros[i] = FALSE ;
                }
            } /* end for (j = 0; j < NV_MAX_MINS; j++) */
        }

        nvi.min2.nam = (byte) i;
        if ( NV_NOTACTIVE_S == OEMNV_Get( NV_MIN2_I, &nvi ) )   /* CFGI_IMSI_S */
        {
            initialize_imsi[i] = TRUE ;
        }
        else
        {
            /* Loop through each MIN */
            for (j = 0; j < NV_MAX_MINS; j++)
            {
                if ( nvi.min2.min2[j] > IMSI_S2_0 )
                {
                    /* invalid range */
                    initialize_imsi[i] = TRUE ;
                }
                if ( nvi.min2.min2[j] != IMSI_S2_0 )
                {
                    imsi_all_zeros[i] = FALSE ;
                }
            } /* end for (j = 0; j < NV_MAX_MINS; j++) */
        }

    } /* for ( i=0; i<NV_MAX_NAMS; i++ ) */

    /*****************************************************************/
    /* By this point all imsi components are read from NV. The flags */
    /* are set indicating if any one of the components were inactive */
    /* or if the whole IMSI was all zeros. In either case, we must   */
    /* initialize all components of IMSI to zero, except the last    */
    /* four digits of MIN1 will be taken from the ESN                */
    /*****************************************************************/
    for ( i=0; i<NV_MAX_NAMS; i++ )
    {
        if ( initialize_imsi[i] == TRUE || imsi_all_zeros[i] == TRUE )
        {
            initialize_imsi_components((byte)i) ;
        }
    }

#ifdef FEATURE_IS95B_TRUE_IMSI
    /* HHD check all items */
    for ( i=0; i<NV_MAX_NAMS; i++ )
    {
        if ( initialize_imsi_t[i] == TRUE || imsi_t_all_zeros[i] == TRUE )
        {
            initialize_imsi_t_components ( (byte)i ) ;
        }
    }
#endif /* END of FEATURE_IS95B_TRUE_IMSI */

#ifdef FEATURE_CDMA_800
    /***********************************/
    /* Check if CDMA is valid or not ? */
    /***********************************/
    if( !hascdma[nam] )
    {
        goodnam = FALSE;
        MSG_HIGH( "Bad NAM!!!", 0, 0, 0 );
    }
#endif /* FEATURE_CDMA_800 */
#endif /* FEATURE_CDMA_800 || FEATURE_CDMA_1900 */

    /******************************************************************/
    /* Tell MC whether UI thinks we can go online or not based on the */
    /* state of the NAMs.                                             */
    /******************************************************************/

    /***************************************************/
    /* Hardware has determined that we must go offline */
    /***************************************************/
    if( hw_force_offline() )
    {  /* hardware force offline */
        MSG_HIGH( "Hardware force offline is shorted (on)", 0, 0, 0 );
        good = FALSE;
    }
    {
        AEECMPhInfo phInfo;
        ICM  *pCM = NULL;
        int nRet;
        
        if (NULL == gpShell)
        {
            return;
        }
        
        nRet = ISHELL_CreateInstance(gpShell,
                                     AEECLSID_CALLMANAGER,
                                     (void **) &pCM);
        if ((nRet != SUCCESS) || (pCM == NULL))
        {
            return;
        }

        ICM_GetPhoneInfo(pCM, &phInfo, sizeof(phInfo));
        
        if (phInfo.oprt_mode != AEECM_OPRT_MODE_FTM)
        {
            if (good && goodnam && phInfo.oprt_mode != AEECM_OPRT_MODE_OFFLINE)
            {
                cm_ph_cmd_subscription_available(NULL,
                                                 NULL,
                                                 client_ui,
                                                 CM_SUBSCRIPTION_STATUS_CHANGE,
                                                 CM_SUBSCRIPTION_STATUS_NO_CHANGE,
                                                 CM_SUBSCRIPTION_STATUS_NO_CHANGE);
                cm_ph_cmd_oprt_mode(NULL, NULL, client_ui, SYS_OPRT_MODE_ONLINE);
            }
            else
            {
                cm_ph_cmd_oprt_mode(NULL, NULL, client_ui, SYS_OPRT_MODE_OFFLINE);
            }
        }
        
        ICM_Release(pCM);
    }
} /* End InitProvisioning */


void oemui_set_uim_esn(void)
{
  uim_cmd_type   *uim_cmd_ptr;              /* UIM command pointer */
  /* Buffer to store the contents of EF Usage Indicator */
  byte ui_uim_buffer[5];

#ifdef FEATURE_UIM_TOOLKIT_UTK
  //uim_svc_table_return_type svc;            /* service allocated and activated*/
#endif /* FEATURE_UIM_TOOLKIT_UTK */

    /* For RUIM card */
    if ( nvruim_dir_present & NVRUIM_CDMA_DF_PRESENT)
    {
        //if we find uim card , the phone enter normal mode
        gbRunAsFactoryTestMode = FALSE;
    /* FOR THOSE SPECIFICATIONS THAT DO NOT MATCH IS-820, THE RESPONSE
       TO STORE_ESN_ME IS IGNORED AND EF USAGE INDICATOR IS ACCESSED TO
       FIND THE PREFERENCE FOR ESN. THE RESPONSE TO STORE_ESN_ME IS CACHED
       IN THE UIM SERVER */

    /* Execute the STORE_ESN_ME command */
    if ((uim_cmd_ptr = (uim_cmd_type*) q_get( &uim_free_q )) != NULL)
    {
      uim_cmd_ptr->hdr.command = UIM_STORE_ESN_ME_F;

      /* Set the usage bit to use UIM_ID and set the length. */
      uim_cmd_ptr->store_esn.len_and_usage =
                 RUIM_STORE_ESN_USE_UIM_ID | RUIM_STORE_ESN_LENGTH;

      #ifdef FEATURE_MULTIMODE_ARCH
        uim_cmd_ptr->store_esn.esn = tmc_get_stored_esn_me();
      #else
        uim_cmd_ptr->store_esn.esn = mc_get_esn_me();
      #endif

      /* Fill in the header information */
      uim_cmd_ptr->hdr.rpt_function = NULL;
      uim_cmd_ptr->hdr.protocol = UIM_CDMA;
      uim_cmd_ptr->hdr.options = UIM_OPTION_NONE;

      /* Fill in necessary fields of command block */
      uim_cmd_ptr->hdr.cmd_hdr.task_ptr = NULL;
      uim_cmd_ptr->hdr.cmd_hdr.sigs = NULL;

      /* Clear the signal */
      (void) rex_clr_sigs( &ui_tcb, UI_RUIM_SIG );

      /* Send the command */
      uim_cmd( uim_cmd_ptr );
    }

    /* Access the EF UIM_ID Usage Indicator */
    if ((uim_cmd_ptr = (uim_cmd_type*) q_get( &uim_free_q )) != NULL)
    {
      /* Access the EF in Read Mode */
      uim_cmd_ptr->hdr.command = UIM_ACCESS_F;
      uim_cmd_ptr->access_uim.access = UIM_READ_F;
      /* Access the Usage Indicator */
      uim_cmd_ptr->access_uim.item = UIM_CDMA_UIM_ID_USAGE_IND;
      /* Offset to access the EF */
      uim_cmd_ptr->access_uim.offset = 0;
      /* Number of bytes to read from the EF */
      uim_cmd_ptr->access_uim.num_bytes = 1;
      /* Set the report function */
      uim_cmd_ptr->access_uim.data_ptr = ui_uim_buffer;
      /* Set the option to always report */
      uim_cmd_ptr->access_uim.hdr.options = UIM_OPTION_ALWAYS_RPT;
      /* Set the Callback function */
      uim_cmd_ptr->hdr.rpt_function = oemui_ruim_report;
      /* Protocol is set to CDMA mode */
      uim_cmd_ptr->hdr.protocol = UIM_CDMA;

      /* Fill in necessary fields of command block */
      uim_cmd_ptr->hdr.cmd_hdr.task_ptr = NULL;
      uim_cmd_ptr->hdr.cmd_hdr.sigs = NULL;

      /* Clear the signal */
      (void) rex_clr_sigs( &ui_tcb, UI_RUIM_SIG );

      /* Send the command */
      uim_cmd( uim_cmd_ptr );

      /* Wait for the command to be done */
      (void) oemui_rex_wait( UI_RUIM_SIG );
    }

    if ((oemui_uim_rpt_buf.rpt_type == UIM_ACCESS_R) &&
        (oemui_uim_rpt_buf.rpt_status == UIM_PASS))
    {
      /* If the usage indicator bit is set */
      if (ui_uim_buffer[0] & RUIM_ESN_USAGE_MASK)
      {
         /* Set the ESN Usage to read the RUIM_ID for ESN */
         nvruim_set_esn_usage( NV_RUIM_USE_RUIM_ID );
      }
      else
      {
         /* Set the ESN Usage to read the actual ESN from NV */
         nvruim_set_esn_usage( NV_RUIM_USE_ESN );
      }
    }
    } /* end if - CDMA_DF_PRESENT */

#if defined( FEATURE_UIM_RUIM_W_GSM_ACCESS ) && \
   !defined( FEATURE_UIM_RUN_TIME_ENABLE )
    else if ( nvruim_dir_present & (NVRUIM_GSM_DF_PRESENT | NVRUIM_DCS1800_DF_PRESENT))
    {
    /* Set the ESN usage to read from NV */
    nvruim_set_esn_usage( NV_RUIM_USE_ESN );
  }
#endif
    else
    {
    nvruim_set_esn_usage( NV_RUIM_ZERO_ESN );
  }

  /* Call the diag function to update its cache since ESN settings are now
     complete. This will force DIAG to reload the ESN and IMSI data */
  diag_otasp_update();

#if 0//def FEATURE_UIM_TOOLKIT_UTK
  /* Initialize buffer for reading CDMA service table */
  if ((uim_cmd_ptr = (uim_cmd_type*) q_get( &uim_free_q )) != NULL)
  {
    /* Read CDMA service table */

    /* Fill up the command header info */
    uim_cmd_ptr->hdr.command = UIM_ACCESS_F;
    /* Set the Protocol to CDMA */
    uim_cmd_ptr->hdr.protocol = UIM_CDMA;
    /* Fill in the callback function */
    uim_cmd_ptr->hdr.rpt_function = oemui_ruim_report;
    /* Set the report option to always report upon completion of the cmd */
    uim_cmd_ptr->hdr.options = UIM_OPTION_ALWAYS_RPT;
    /* The task and signal are set to NULL */
    uim_cmd_ptr->hdr.cmd_hdr.task_ptr = NULL;
    uim_cmd_ptr->hdr.cmd_hdr.sigs = NULL;

    /* Access info */
    uim_cmd_ptr->access_uim.item = UIM_CDMA_CDMA_SVC_TABLE;
    /* Read Access to UIM */
    uim_cmd_ptr->access_uim.access = UIM_READ_F;
    /* Indicates the offset for read */
    uim_cmd_ptr->access_uim.offset = 0;
    /* Buffer to return the data in */
    uim_cmd_ptr->access_uim.data_ptr = ui.uim_cdma_svc_table_buffer;
    /* Number of bytes to access */
    uim_cmd_ptr->access_uim.num_bytes = UIM_CDMA_SVC_TABLE_SIZE;

    /* Clear the signal */
    (void) rex_clr_sigs( &ui_tcb, UI_RUIM_SIG );

    /* Send the command */
    uim_cmd( uim_cmd_ptr );

    /* Wait for the command to be done */
    (void) oemui_rex_wait( UI_UIM_STATUS_SIG );

    if ((oemui_uim_rpt_buf.rpt_type == UIM_ACCESS_R) &&
      (oemui_uim_rpt_buf.rpt_status == UIM_PASS))
    {
      /* Check to see if UIM is Pro-Active */
      svc = uim_return_cdma_svc_availabililty ( UIM_CDMA_SVC_PROACTIVE_UIM,
            ui.uim_cdma_svc_table_buffer);

      /* Check if Proactive uim has been allocated and activated */
      if ( svc.activated && svc.allocated )
      {
        /* The UIM is kept on for the proactive UIM. */
        uim_power_control( UIM_PROACTIVE_UIM, TRUE);

        /* Set the info on Pro-active UIM */
        uim_set_proactive_uim ( TRUE );

        /* Send TERMINAL PROFILE command */
        if ((uim_cmd_ptr = (uim_cmd_type*) q_get( &uim_free_q )) != NULL)
        {
          /* Fill up header info */
          uim_cmd_ptr->hdr.command = UIM_TERMINAL_PROFILE_F;
          /* Set the Protocol to CDMA */
          uim_cmd_ptr->hdr.protocol = UIM_CDMA;
          /* Fill in the callback function */
          uim_cmd_ptr->hdr.rpt_function = oemui_ruim_report;
          /* Set the report option to always report upon completion of the cmd */
          uim_cmd_ptr->hdr.options = UIM_OPTION_ALWAYS_RPT;
          /* The task and signal are set to NULL */
          uim_cmd_ptr->hdr.cmd_hdr.task_ptr = NULL;
          uim_cmd_ptr->hdr.cmd_hdr.sigs = NULL;

          /* Number of bytes sent in the command */
          uim_cmd_ptr->terminal_profile.num_bytes =
                      UIM_TK_TERMINAL_PROFILE_DATA_LENGTH;

          /* Initialize buffer for sending terminal profile command */
          memset (uim_cmd_ptr->terminal_profile.data, 0,
                UIM_TK_TERMINAL_PROFILE_DATA_LENGTH);

          /* Fill bit fields for command data */
          uim_cmd_ptr->terminal_profile.data[0]  |=
                ( UIM_TK_B1_PROFILE_DOWNLOAD | UIM_TK_B1_CDMA_SMS_PP_DOWNLOAD );
          uim_cmd_ptr->terminal_profile.data[2]  |=
                UIM_TK_B3_DISPLAY_TEXT;
#ifdef FEATURE_UIM_DEBUG_TOOLKIT_SEND_SMS
          /* Set terminal profile bytes specific to the use of our application.
          ** Setting the bytes to 0xFF enables specific proactive commands, in
          ** particular SET UP MENU to invoke CDMA Send SMS.
          */
          uim_cmd_ptr->terminal_profile.data[0]  = 0xFF;
          uim_cmd_ptr->terminal_profile.data[1]  = 0xFF;
          uim_cmd_ptr->terminal_profile.data[2]  = 0xFF;
          uim_cmd_ptr->terminal_profile.data[3]  = 0xFF;
          uim_cmd_ptr->terminal_profile.data[7]  = 0xFF;
          uim_cmd_ptr->terminal_profile.data[8]  = 0xFF;
#endif /* FEATURE_UIM_DEBUG_TOOLKIT_SEND_SMS */
          uim_cmd_ptr->terminal_profile.data[19] |=
                ( UIM_TK_B20_CDMA_SEND_SMS | UIM_TK_B20_CDMA_SMS_PP_DOWNLOAD );

          /* Clear the signal */
          (void) rex_clr_sigs( &ui_tcb, UI_RUIM_SIG );

          /* Send the command */
          uim_cmd( uim_cmd_ptr );

          /* Wait for the command to be done */
          (void) oemui_rex_wait( UI_RUIM_SIG );
        } /* buffer available */
      } /* svc allocated and activated */
    } /* UIM_PASS */
  }
  else
  {
    MSG_HIGH ("Buffer not available to read CDMA service table",0,0,0);
  }
#endif /* FEATURE_UIM_TOOLKIT_UTK */
}

#ifdef FEATURE_LOAD_DEFAULT_SETTING
//#define OEM_INIFILE   "oem.ini"
/*==============================================================================
������
    OEM_STRTRIM
    
˵����
    OEM_STRTRIM ��������ȥ�������ַ����е�ǰ�󵼿ո�
       
������
    szItem [in]���ַ�����
       
����ֵ��
    none
       
��ע:
    �ú����Ὣ�����е�ǰ�󵼿ո�ȥ��
==============================================================================*/
static void OEM_STRTRIM(char *szItem)
{
    int i=0, nLen;
    
    if (NULL == szItem)
    {
        return;
    }
    nLen = STRLEN(szItem);
    
    // ȥǰ���ո�
    for (i=0; i<nLen; i++)
    {
        if (szItem[i] != ' ')
        {
            if (i != 0)
            {
                MEMMOVE(szItem, &szItem[i], (nLen-i));
                nLen = nLen-i;
                szItem[nLen]=0;
            }
            break;
        }
    }
    
    // ȥ�󵼿ո�
    for (i=nLen-1; i>=0; i--)
    {
        if (szItem[i] != ' ')
        {
            break;
        }
        
        szItem[i] = 0;
    }
}


/*==============================================================================
������
    OEM_ProcessOeminitItem
    
˵����
    OEM_ProcessOeminitItem �������������ļ� oem.ini �е�һ������
       
������
    szItem [in]: ����
    pItembuf[in]: ���Ӧֵ
    nSize[in]: ֵ��������С
       
����ֵ��
    none
       
��ע:

==============================================================================*/
static void OEM_ProcessOeminitItem(char *szItem, char *pItembuf, int nSize)
{
    if (NULL == szItem || NULL == pItembuf || 0 == nSize)
    {
        return;
    }
    
    OEM_STRTRIM(pItembuf);
    
    //DBGPRINTF("%s  %s OEM_ProcessOeminitItem", szItem, pItembuf);
    
    if (STRCMP(szItem, "N00") == 0) /*CFGI_EMERGENCYNUM_TABLE*/
    {
        EmergencyNum_Table m_emerg_buf;
        //char m_emerg_buf[10][10];
        char *psz_Start = pItembuf;
        char *psz_Point = pItembuf;
        int i = 0;
        psz_Point = STRCHR(pItembuf, '&');
        MEMSET((void*)&m_emerg_buf, 0 ,sizeof(m_emerg_buf));
        while(psz_Point)
        {
            if(i <MAX_EMERGENCY_NUM )
            {
                int len = psz_Point-psz_Start;
                if(len <MAX_EMERGENCY_NUM_LEN && len >0)
                {
                    STRNCPY((char*)m_emerg_buf.emerg_num[i].num_buf, psz_Start, len);
                    //(char*)m_emerg_buf.emerg_num[i].num_buf[len] = 0;
                    (int)m_emerg_buf.emerg_num[i].num_len = len;
                    i++;
                }
                //else
                //{
                //    i --;
                //}
            }
            //i++;
            psz_Point ++;
            pItembuf  = psz_Point ;
            psz_Start = psz_Point ;
            psz_Point = STRCHR(pItembuf, '&');
        }
        //The last Emergency Number
        STRLCPY(m_emerg_buf.emerg_num[i].num_buf,pItembuf,MAX_EMERGENCY_NUM_LEN);
        (int)m_emerg_buf.emerg_num[i].num_len = STRLEN(pItembuf);
        
        m_emerg_buf.emert_size = i +1;
        OEM_SetConfig(CFGI_EMERGENCYNUM_TABLE, (void *)& m_emerg_buf, sizeof(EmergencyNum_Table));
        return;
    }

    if (STRCMP(szItem, "N01") == 0) /*CFGI_CALLFORWARD_BUSY_ENABLE*/
    {
        OEM_SetConfig(CFGI_CALLFORWARD_BUSY_ENABLE, (void *)pItembuf ,FEATURE_CODE_MAX_LENTH);
        return;
    }
    if (STRCMP(szItem, "N02") == 0) /*CFGI_CALLFORWARD_BUSY_DISABLE*/
    {
        OEM_SetConfig(CFGI_CALLFORWARD_BUSY_DISABLE, (void *)pItembuf ,FEATURE_CODE_MAX_LENTH);
        return;
    }
    if (STRCMP(szItem, "N03") == 0) /*CFGI_CALLFORWARD_NOANSWER_ENABLE*/
    {
        OEM_SetConfig(CFGI_CALLFORWARD_NOANSWER_ENABLE, (void *)pItembuf ,FEATURE_CODE_MAX_LENTH);
        return;
    }
    if (STRCMP(szItem, "N04") == 0) /*CFGI_CALLFORWARD_NOANSWER_DISABLE*/
    {
        OEM_SetConfig(CFGI_CALLFORWARD_NOANSWER_DISABLE, (void *)pItembuf ,FEATURE_CODE_MAX_LENTH);
        return;
    }
    if (STRCMP(szItem, "N05") == 0) /*CFGI_CALLFORWARD_NOCONNECT_ENABLE*/
    {
        OEM_SetConfig(CFGI_CALLFORWARD_NOCONNECT_ENABLE, (void *)pItembuf ,FEATURE_CODE_MAX_LENTH);
        return;
    }
    if (STRCMP(szItem, "N06") == 0) /*CFGI_CALLFORWARD_NOCONNECT_DISABLE*/
    {
        OEM_SetConfig(CFGI_CALLFORWARD_NOCONNECT_DISABLE, (void *)pItembuf ,FEATURE_CODE_MAX_LENTH);
        return;
    }
    if (STRCMP(szItem, "N07") == 0) /*CFGI_CALLFORWARD_ANYWAY_ENABLE*/
    {
        OEM_SetConfig(CFGI_CALLFORWARD_ANYWAY_ENABLE, (void *)pItembuf ,FEATURE_CODE_MAX_LENTH);
        return;
    }
    if (STRCMP(szItem, "N08") == 0) /*CFGI_CALLFORWARD_ANYWAY_DISABLE*/
    {
        OEM_SetConfig(CFGI_CALLFORWARD_ANYWAY_DISABLE, (void *)pItembuf ,FEATURE_CODE_MAX_LENTH);
        return;
    }
    if (STRCMP(szItem, "N09") == 0) /*CFGI_CALLFORWARD_DISABLE_ALL*/
    {
        OEM_SetConfig(CFGI_CALLFORWARD_DISABLE_ALL, (void *)pItembuf ,FEATURE_CODE_MAX_LENTH);
        return;
    }
    if (STRCMP(szItem, "N10") == 0) /*CFGI_CALLFORWARD_WAIT_ENABLE*/
    {
        OEM_SetConfig(CFGI_CALLFORWARD_WAIT_ENABLE, (void *)pItembuf ,FEATURE_CODE_MAX_LENTH);
        return;
    }
    if (STRCMP(szItem, "N11") == 0) /*CFGI_CALLFORWARD_WAIT_DISABLE*/
    {
        OEM_SetConfig(CFGI_CALLFORWARD_WAIT_DISABLE, (void *)pItembuf ,FEATURE_CODE_MAX_LENTH);
        return;
    }
    if (STRCMP(szItem, "N12") == 0) /*CFGI_CALLFORWARD_VOICEMAIL_ENABLE*/
    {
        OEM_SetConfig(CFGI_CALLFORWARD_VOICEMAIL_ENABLE, (void *)pItembuf ,FEATURE_CODE_MAX_LENTH);
        return;
    }
    if (STRCMP(szItem, "N13") == 0) /*CFGI_EMERGENCYNUM_TABLE*/
    {
        ServiceProvider List_SP[OEMNV_MAX_SERVICEPROVIDER_NUMBER];
        //char m_emerg_buf[10][10];
        char *psz_Start = pItembuf;
        char *psz_Point = pItembuf;
        int i = 0;
        OEM_GetConfig(CFGI_SERVICE_PROVIDER,(void*) List_SP,sizeof(ServiceProvider) * OEMNV_MAX_SERVICEPROVIDER_NUMBER);
        psz_Point = STRCHR(pItembuf, '&');
        //MEMSET((void*)&List_SP, 0 ,sizeof(List_SP));
        while(psz_Point)
        {
            if(i <OEMNV_MAX_SERVICEPROVIDER_NUMBER )
            {
                int len = psz_Point-psz_Start;
                if(len <OEMNV_MAX_MCC_SIZE && len >0)
                {
                    STRNCPY(List_SP[i].mcc, psz_Start, len);
                    i++;
                }
                //else
                //{
                //    i --;
                //}
            }
            //i++;
            psz_Point ++;
            pItembuf  = psz_Point ;
            psz_Start = psz_Point ;
            psz_Point = STRCHR(pItembuf, '&');
        }
        //The last Emergency Number
        STRLCPY((char*)List_SP[i].mcc,pItembuf,OEMNV_MAX_MCC_SIZE);  
        OEM_SetConfig(CFGI_SERVICE_PROVIDER,(void*) List_SP,sizeof(ServiceProvider) * OEMNV_MAX_SERVICEPROVIDER_NUMBER);
        return;
    }

    if (STRCMP(szItem, "N14") == 0) /*CFGI_EMERGENCYNUM_TABLE*/
    {
        ServiceProvider List_SP[OEMNV_MAX_SERVICEPROVIDER_NUMBER];
        //char m_emerg_buf[10][10];
        char *psz_Start = pItembuf;
        char *psz_Point = pItembuf;
        int i = 0;
        OEM_GetConfig(CFGI_SERVICE_PROVIDER,(void*) List_SP,sizeof(ServiceProvider) * OEMNV_MAX_SERVICEPROVIDER_NUMBER);        
        psz_Point = STRCHR(pItembuf, '&');
        //MEMSET((void*)&List_SP, 0 ,sizeof(List_SP));
        while(psz_Point)
        {
            if(i <OEMNV_MAX_SERVICEPROVIDER_NUMBER )
            {
                int len = psz_Point-psz_Start;
                if(len <OEMNV_MAX_MNC_SIZE && len >0)
                {                
                    STRNCPY(List_SP[i].mnc, psz_Start, len);
                    i++;
                }
                //else
                //{
                //    i --;
                //}
            }
            //i++;
            psz_Point ++;
            pItembuf  = psz_Point ;
            psz_Start = psz_Point ;
            psz_Point = STRCHR(pItembuf, '&');
        }
        //The last Emergency Number
        STRLCPY((char*)List_SP[i].mnc,pItembuf,OEMNV_MAX_MNC_SIZE);  
        OEM_SetConfig(CFGI_SERVICE_PROVIDER,(void*) List_SP,sizeof(ServiceProvider) * OEMNV_MAX_SERVICEPROVIDER_NUMBER);
        return;
    }   

    if (STRCMP(szItem, "N15") == 0) /*CFGI_EMERGENCYNUM_TABLE*/
    {
        ServiceProvider List_SP[OEMNV_MAX_SERVICEPROVIDER_NUMBER];        
        //char m_emerg_buf[10][10];
        char *psz_Start = pItembuf;
        char *psz_Point = pItembuf;
        int i = 0;
        OEM_GetConfig(CFGI_SERVICE_PROVIDER,(void*) List_SP,sizeof(ServiceProvider) * OEMNV_MAX_SERVICEPROVIDER_NUMBER);
        psz_Point = STRCHR(pItembuf, '&');
        //MEMSET((void*)&List_SP, 0 ,sizeof(List_SP));
        while(psz_Point)
        {
            if(i <OEMNV_MAX_SERVICEPROVIDER_NUMBER )
            {
                int len = psz_Point-psz_Start;
                if(len <OPERATOR_NAME_MAX_SIZE && len >0)
                {                
                    STRNCPY(List_SP[i].ids_name, psz_Start, len);
                    i++;
                }
                //else
                //{
                //    i --;
                //}
            }
            //i++;
            psz_Point ++;
            pItembuf  = psz_Point ;
            psz_Start = psz_Point ;
            psz_Point = STRCHR(pItembuf, '&');
        }
        //The last Emergency Number
        STRLCPY((char*)List_SP[i].ids_name,pItembuf,OPERATOR_NAME_MAX_SIZE);  
        OEM_SetConfig(CFGI_SERVICE_PROVIDER,(void*) List_SP,sizeof(ServiceProvider) * OEMNV_MAX_SERVICEPROVIDER_NUMBER);
        return;
    }     
    if (STRCMP(szItem, "N16") == 0) /*CFGI_BANNER*/
    {
        AECHAR buf[NV_MAX_LTRS+1] = {0};
        STRTOWSTR(pItembuf,buf,sizeof(buf));
        OEM_SetConfig(CFGI_BANNER, (void *)buf ,(NV_MAX_LTRS+1));
        return;
    }
    if (STRCMP(szItem, "N17") == 0) /*CFGI_VOICEMAIL_NUMBER*/
    {
        AECHAR buf[OEMNV_VOICEMAIL_MAXLEN] = {0};
        STRTOWSTR(pItembuf,buf,sizeof(buf));
        OEM_SetConfig(CFGI_VOICEMAIL_NUMBER, (void *)buf ,(sizeof(buf)/*OEMNV_VOICEMAIL_MAXLEN*/));
        return;    
    }	
#ifdef FEATURE_CARRIER_SUDAN_SUDATEL
   if (STRCMP(szItem, "N18") == 0) /*CFGI_CALLFORWARD_BUSY_VOICEMAIL*/
    {
        OEM_SetConfig(CFGI_CALLFORWARD_BUSY_VOICEMAIL, (void *)pItembuf ,FEATURE_CODE_MAX_LENTH);
        return;
    }
    if (STRCMP(szItem, "N19") == 0) /*CFGI_CALLFORWARD_ANYWAY_VOICEMAIL*/
    {
        OEM_SetConfig(CFGI_CALLFORWARD_ANYWAY_VOICEMAIL, (void *)pItembuf ,FEATURE_CODE_MAX_LENTH);
        return;
    }
    if (STRCMP(szItem, "N20") == 0) /*CFGI_CALLFORWARD_NOANSWER_VOICEMAIL*/
    {
        OEM_SetConfig(CFGI_CALLFORWARD_NOANSWER_VOICEMAIL, (void *)pItembuf ,FEATURE_CODE_MAX_LENTH);
        return;
    }
    if (STRCMP(szItem, "N21") == 0) /*CFGI_CALLFORWARD_NOCONNECT_VOICEMAIL*/
    {
        OEM_SetConfig(CFGI_CALLFORWARD_NOCONNECT_VOICEMAIL, (void *)pItembuf ,FEATURE_CODE_MAX_LENTH);
        return;
    }
    if (STRCMP(szItem, "N22") == 0) /*CFGI_CALLFORWARD_WAIT_TEMP_DISABLE*/
    {
        OEM_SetConfig(CFGI_CALLFORWARD_WAIT_TEMP_DISABLE, (void *)pItembuf ,FEATURE_CODE_MAX_LENTH);
        return;
    }
    if (STRCMP(szItem, "N23") == 0) /*CFGI_CALLFORWARD_CNIR_DISABLE*/
    {
        OEM_SetConfig(CFGI_CALLFORWARD_CNIR_DISABLE, (void *)pItembuf ,FEATURE_CODE_MAX_LENTH);
        return;
    }
    if (STRCMP(szItem, "N24") == 0) /*CFGI_CALLFORWARD_CNIR_ENABLE*/
    {
        OEM_SetConfig(CFGI_CALLFORWARD_CNIR_ENABLE, (void *)pItembuf ,FEATURE_CODE_MAX_LENTH);
        return;
    }
    if (STRCMP(szItem, "N25") == 0) /*CFGI_CALLFORWARD_DND_DISABLE*/
    {
        OEM_SetConfig(CFGI_CALLFORWARD_DND_DISABLE, (void *)pItembuf ,FEATURE_CODE_MAX_LENTH);
        return;
    }
    if (STRCMP(szItem, "N26") == 0) /*CFGI_CALLFORWARD_DND_ENABLE */
    {
        OEM_SetConfig(CFGI_CALLFORWARD_DND_ENABLE , (void *)pItembuf ,FEATURE_CODE_MAX_LENTH);
        return;
    }
#endif

    
}

/*==============================================================================
������
    OEM_ImportOeminitdata
    
˵����
    OEM_ImportOeminitdata �����������ļ� oem.ini ת��Ϊ�ڲ�����
       
������
    
       
����ֵ��
    SUCCESS : �����û������ļ��ɹ���
    EFAILED : �����û������ļ�ʧ�ܻ���Ҫ��
       
��ע:

==============================================================================*/

static int OEM_ImportOeminitdata(IShell *m_pShell)
{
    
    int     nRet = EFAILED;
    byte    *pdatabuf = NULL;
    byte    *pItembuf = NULL;
    int     i,j=0,k=0;
    boolean bNewItemStart, bNewItemEnd, bNewItemValStart;
    boolean bRemark;
    char    szItem[10];
    boolean bUnicode = FALSE;
    
    IFileMgr *pIFileMgr = NULL; 
    FileInfo pInfo;
    IFile    *pFile = NULL;

    if(m_pShell == NULL)
    {
        return EFAILED;
    }
    
    if (AEE_SUCCESS != ISHELL_CreateInstance(m_pShell, AEECLSID_FILEMGR,(void **)&pIFileMgr))
    {
        return EFAILED;
    }
    
    //ERR("OEM_ImportOeminitdata",0,0,0);
    
    if (IFILEMGR_Test (pIFileMgr,AEEFS_ROOT_DIR OEM_INIFILE) != SUCCESS)
    {
        //ERR("OEM_ImportOeminitdata1",0,0,0);
        goto EXIT_PROCESS;
    }
    
    if (IFILEMGR_GetInfo(pIFileMgr,AEEFS_ROOT_DIR OEM_INIFILE, &pInfo) != SUCCESS)
    {
        //ERR("OEM_ImportOeminitdata2",0,0,0);
        goto EXIT_PROCESS;
    }
    
    pdatabuf = (byte *)MALLOC(pInfo.dwSize);
    pItembuf = (byte *)MALLOC(256);
    
    pFile = IFILEMGR_OpenFile(pIFileMgr,AEEFS_ROOT_DIR OEM_INIFILE,_OFM_READ);
    if ((pFile == NULL) || (NULL == pdatabuf) || (NULL == pItembuf))
    {
        //ERR("OEM_ImportOeminitdata3",0,0,0);
        goto EXIT_PROCESS;
    }
    
    IFILE_Seek(pFile, _SEEK_START, 0);
    // ���������ݶ��� Buffer ��
    if (IFILE_Read(pFile,pdatabuf,pInfo.dwSize) != pInfo.dwSize)
    {
        //ERR("OEM_ImportOeminitdata4",0,0,0);
        goto EXIT_PROCESS;
    }
    
    if (pFile)
    {
        IFILE_Release(pFile);
        pFile = NULL;
    }
    
    if (pIFileMgr)
    {
        IFILEMGR_Release (pIFileMgr);
        pIFileMgr = NULL;
    }
    
    // ��ʼ��������
    bNewItemStart = FALSE;
    bNewItemEnd = FALSE;
    bNewItemValStart = FALSE;
    bRemark = FALSE;
    for (i=0;i<pInfo.dwSize-1;i++)
    {
        if ((bRemark) && (pdatabuf[i] == 0x0D) && (pdatabuf[i+1] == 0x0A))
        {// ���з�������ע�ͽ���
            i++;
            bRemark = FALSE;
            continue;
        }
        else if(bRemark)
        {// ע�����ݣ�����
            continue;
        }
        else if ((pdatabuf[i] == 0x0D) && (pdatabuf[i+1] == 0x0A))
        {// ���з�������
            continue;
        }
        else if ((pdatabuf[i] == '/') && (pdatabuf[i+1] == '/'))
        {// ����ע�Ϳ�ʼ��ǣ�����
            i++;
            bRemark = TRUE;
            continue;
        }
        else if ((pdatabuf[i] < 0x20) && (!bUnicode))
        {// ���ɼ����Ʒ�������
            continue;
        }
        else if (pdatabuf[i] == '[')
        {// �����ǿ�ʼ
            if (bNewItemStart && bNewItemEnd && bNewItemValStart)
            {// ���ʼ,ǰ�����,�����������
                OEM_ProcessOeminitItem(szItem, (char *)pItembuf, 256);
            }
            bNewItemStart = TRUE;
            bNewItemEnd = FALSE;
            bNewItemValStart = FALSE;
            bUnicode = FALSE;
            
            MEMSET(szItem, 0, sizeof(szItem));
            MEMSET(pItembuf, 0, 256);
            j=0;
            k=0;
        }
        else if (pdatabuf[i] == ']' && bNewItemStart)
        {// �����ǽ���
            bNewItemEnd = TRUE;
            continue;
        }
        else if (pdatabuf[i] == '=' && bNewItemStart && bNewItemEnd)
        {// ��ֵ��ʼ
            bNewItemValStart = TRUE;
        }
        else if (bNewItemStart && !bNewItemEnd)
        {
            if (k<9)
            {
                szItem[k]=pdatabuf[i];
                k++;
            }
        }
        else if (bNewItemValStart)
        {
            if (j<254)
            {
               // pItembuf[j]=pdatabuf[i];
                if ((pdatabuf[i]=='^') && (!bUnicode))
                {
                    pItembuf[j]=pdatabuf[i];
                    bUnicode = TRUE;
                }
                else if ((pdatabuf[i]=='^') && bUnicode)
                {
                    bUnicode = FALSE;
                }
                else
                {
                    pItembuf[j]=pdatabuf[i];
                }
                j++;
            }
        }
    }   
    
    if (bNewItemStart && bNewItemEnd && bNewItemValStart)
    {// �������������
        OEM_ProcessOeminitItem(szItem, (char*)pItembuf,256);
    }   
    
    // ɾ�� INI �ļ�
    //OEMFS_Remove(AEEFS_ROOT_DIR OEM_INIFILE);
    OEMFS_Rename(AEEFS_ROOT_DIR OEM_INIFILE,AEEFS_ROOT_DIR OEM_BAKFILE);
    
    nRet = SUCCESS;
    
EXIT_PROCESS:    

    if (pFile)
    {
        IFILE_Release(pFile);
        pFile = NULL;
    }
    
    if (pIFileMgr)
    {
        IFILEMGR_Release (pIFileMgr);
        pIFileMgr = NULL;
    }
    
    if (pdatabuf != NULL)
    {
        FREE(pdatabuf);
    }
    
    if (NULL != pItembuf)
    {
        FREE(pItembuf);
    }
    return nRet;
}
#endif /*FEATURE_LOAD_DEFAULT_SETTING*/

/* <EJECT> */
/*===========================================================================
FUNCTION   oemui_get_call_type

DESCRIPTION
  Determine call type for a origination #

DEPENDENCIES
  None

RETURNS
  None, result will be put into call_type_ptr and otasp_code_ptr

SIDE EFFECTS
  None
===========================================================================*/
void oemui_get_call_type(
  const cm_num_s_type     *orig_num_ptr,    // pointer to origination #
  cm_call_type_e_type     *call_type_ptr,   // associated call type, save result
  cm_activate_code_e_type *otasp_code_ptr   // OTASP activation code(OTASP call)
)
{
#ifdef FEATURE_OTASP
    int   i;
#endif /* FEATURE_OTASP */

    /* If this is NOT a voice call, return now  */
    if( *call_type_ptr != CM_CALL_TYPE_VOICE ) 
    {
        return;
    }

    //if (ui_emergency_number(orig_num_ptr->buf, orig_num_ptr->len))
    if(OEM_IsEmergency_Number((char*)orig_num_ptr->buf,(int)orig_num_ptr->len))
    {
        *call_type_ptr = CM_CALL_TYPE_EMERGENCY;
        return;
    }

#ifdef FEATURE_OTASP
    /* "*228" is checked specially. */
    if ( orig_num_ptr->len == strlen(STAR228) &&
        ! memcmp(orig_num_ptr->buf, STAR228, strlen(STAR228)) )
    {
        *call_type_ptr  = CM_CALL_TYPE_NON_STD_OTASP;
        *otasp_code_ptr = CM_PREFERRED;
        return;
    }

    /* Consult the default hard-coded otasp numbers table to
    ** determine the originating number call type.
    */
    for( i=0; i< (int)ARR_SIZE(otasp_num_tbl); i++ )
    {
        /* Check whether one of the otasp numbers match the origination #.
        */
        if( orig_num_ptr->len >= STANDARD_OTASP_LENGTH &&
            ! memcmp(orig_num_ptr->buf, otasp_num_tbl[i].num_str, STANDARD_OTASP_LENGTH) )
        {
            /* If we found a match, change the call type to the corresponding,
            ** type in the table and indicate the OTASP activation code.
            */
            *call_type_ptr  = otasp_num_tbl[i].call_type;
            *otasp_code_ptr = otasp_num_tbl[i].activate_code;
            return;
        }
    } /* end of for */

#endif /* FEATURE_OTASP */
}


/* <EJECT> */
/*===========================================================================
FUNCTION   oemui_call_is_allowed

DESCRIPTION
  Determine if a call is allowed from phone book

DEPENDENCIES
  None

RETURNS
  None

SIDE EFFECTS
  None
===========================================================================*/
boolean oemui_call_is_allowed(
  const cm_num_s_type *orig_num_ptr  // pointer to origination #
)
{
    cm_call_type_e_type     call_type;
    cm_activate_code_e_type otasp_activation_code;
    boolean call_allowed = TRUE;       /* Is call still allowed? */

    /* Check incoming parameters. */
    if ((orig_num_ptr == NULL))
    {
        ERR("Bad parameter from CM %x ", orig_num_ptr, 0, 0);
        return FALSE;
    }

    call_type = CM_CALL_TYPE_VOICE; //initialization
    /* Get the call type.  */
    oemui_get_call_type(orig_num_ptr, &call_type, &otasp_activation_code);

    /* First, check if its an emergency call. If it is, all processing stops
    * now and the call is allowed wherever it can be.  */
    if (call_type == CM_CALL_TYPE_EMERGENCY)
    {
        return TRUE;
    }
    return call_allowed;
}

#ifdef FEATURE_UTK2
void set_UTK_session_status(byte st)
{
	ui_cmd_type  *ui_buf_ptr;

#ifdef FEATURE_REX_DYNA_MEM_UI
	ui_buf_ptr = ui_get_cmd();
#else
	if( (ui_buf_ptr = (ui_cmd_type*) q_get( &ui_cmd_free_q )) == NULL )
	{
		ERR("Out of UI cmd buffer", 0,0,0);
		return;
	}
	ui_buf_ptr->proactive_cmd.hdr.done_q_ptr = &ui_cmd_free_q;;
#endif /* FEATURE_REX_DYNA_MEM_UI */

	ui_buf_ptr->proactive_cmd.hdr.cmd        = UI_PROACTIVE_UIM_F;
	ui_buf_ptr->proactive_cmd.hdr.task_ptr   = NULL;
	ui_buf_ptr->proactive_cmd.hdr.sigs       = 0;
	//ui_buf_ptr->proactive_cmd.hdr.done_q_ptr = &ui_cmd_free_q;

	ui_buf_ptr->proactive_cmd.num_bytes = 1;
	ui_buf_ptr->proactive_cmd.cmd_data[0] = st; //UIM_TK_END_PROACTIVE_SESSION;

	/* send command to ui */
	ui_cmd( ui_buf_ptr );

	if (st == UIM_TK_END_PROACTIVE_SESSION)
	{
	    uim_power_control( UIM_PROACTIVE_UIM, FALSE);
	    
	    /* Make sure task can process this control */
	    (void) rex_set_sigs( &uim_tcb, UIM_STATE_TOGGLE_SIG);
	}
}
#endif

/*==============================================================================
����: 
    oemui_setbusymode
       
˵��: 
    Set ui task busy mode value
       
����: 
    flag [in]: �Ƿ���æ��ģʽ�ı�־
    
����ֵ:
    none
       
��ע:
    
==============================================================================*/
void oemui_setbusymode(boolean flag)
{
    oemui_busy = flag;
}
int oemui_is_lcd_test_state(int bb)
{
    static int g_is_lcd_test = 0;
    if (bb>=0)
    {
        g_is_lcd_test = bb;
    }
    return g_is_lcd_test;	
}

#ifdef FEATURE_CARRIER_THAILAND_HUTCH
/*==============================================================================
����: 
    oemui_ispasskeyevent
       
˵��: 
    
       
����: 
    
    
����ֵ:
    
       
��ע:
    // ben: for hutch
==============================================================================*/
boolean oemui_ispasskeyevent(KeyEntry *pKeyEntry)
{
    if ( (pKeyEntry->hs_key >= HS_0_K && pKeyEntry->hs_key <= HS_9_K)
        || pKeyEntry->hs_key == HS_POUND_K
        || pKeyEntry->hs_key == HS_STAR_K )
    {
        return TRUE;
    }

#if defined FEATURE_CARRIER_THAILAND_HUTCH
    if ( (pKeyEntry->hs_key == HS_SEND_K) && (1 == Appscomm_is_incoming_state(-1))) 
    {
        return TRUE;
    }
#endif

    if (  pKeyEntry->hs_key == HS_END_K && brewui_isincall())
    {
        return TRUE;
    }
    return FALSE;
}

#endif //#ifdef FEATURE_CARRIER_THAILAND_HUTCH

#ifdef FEATURE_UIM
/*==============================================================================
����: 
    oemui_unlockuim
       
˵��: 
    �������� gbrewui.uimunlocked Ϊ�棬��ʾ UI ��ͨ�� PIN/PUK �������֤������
    ��Ϣ������ȷ��ȡ�ˡ�
       
����: 
    
����ֵ:
    none
       
��ע:
    
==============================================================================*/
void oemui_unlockuim(void)
{
    gbrewui.uimunlocked = TRUE;
}

/*==============================================================================
����: 
    oemui_uimisunlocked
       
˵��: 
    �������� gbrewui.uimunlocked ֵ�������߸��ݷ���ֵ��ȷ��������Ϣ������ȷ��ȡ��
       
����: 
    
����ֵ:
    TRUE: UI ��ͨ�� PIN/PUK ��֤
    FALSE: UI ��δͨ�� PIN/PUK ��֤
       
��ע:
    
==============================================================================*/
boolean oemui_uimisunlocked(void)
{
    return gbrewui.uimunlocked;
}

#endif
void oemui_set_powerup_state(boolean flg)
{
    gbrewui.powerup = flg;
}

boolean oemui_get_powerup_state(void)
{
    return gbrewui.powerup;
}
/*==============================================================================
����: 
    app_media_scheduler
       
˵��: 
    �жϵ�ǰ��Ҫ�Ķ�ý���豸(VC0848)�Ƿ��ѱ�ռ��
       
����: 
    none
����ֵ:
    APP_MEDIA_ALLOW:            ����ʹ��
    APP_MEDIA_IMPACT_BY_FM:     �ѱ�FMռ��
    APP_MEDIA_IMPACT_BY_MP3:    �ѱ�MP3ռ��
       
��ע:
    
==============================================================================*/
int app_media_scheduler(void)
{
    boolean b_FMBackground = FALSE;
    
    OEM_GetConfig(CFGI_FM_BACKGROUND,&b_FMBackground, sizeof(b_FMBackground));
    if((TRUE == b_FMBackground) && (AEECLSID_APP_FMRADIO != ISHELL_ActiveApplet(AEE_GetShell())))
    {
        return APP_MEDIA_IMPACT_BY_FM;
    }
#ifdef FEATURE_APP_MUSICPLAYER    
    else if(IsMp3PlayerStatusOnBG()) 
    {
        return APP_MEDIA_IMPACT_BY_MP3;        
    }
#endif    
    return APP_MEDIA_ALLOW;
}

