#ifndef BREWUI_H
#define BREWUI_H
/*=============================================================================
//           U S E R   I N T E R F A C E   M A I N   H E A D E R
// �ļ���
//        brewui.h
//        
// �ļ�˵����
//        ���ļ�������ģ��ʹ�� oenui task ���ⲿ�ӿڡ�
//
//
// ���ߣ�2007-08-27
// ��ǰ�汾��
==============================================================================*/


/*==============================================================================
�޸���ʷ��¼��
ʱ       ��       �� �� ��     �޸����ݡ�λ�ü�ԭ��
-----------      ----------    -----------------------------------------------

==============================================================================*/

/*==============================================================================
                                 ���ļ��������ⲿ�ļ�
==============================================================================*/
#include "comdef.h"               /* basic definitions */
#include "queue.h"                /* queue services */
#include "rex.h"                  /* REX services */
#include "target.h"               /* Target configuration file */
#include "customer.h"             /* Customer Specific Definitions */

#if !defined(FEATURE_WCDMA) && !defined(FEATURE_GSM)
#include "mc.h"
#else
#error code not present
#endif 

#include "cm.h"
#ifdef FEATURE_MULTIMEDIA
#include "fs.h"
#include "cmx.h"
#endif

#ifdef FEATURE_UIM_TOOLKIT
#include "uim.h"
#endif
#include "hs.h"


/*==============================================================================
                            �����ⲿ����
===============================================================================*/

/*==============================================================================
                                 �궨��ͳ���
==============================================================================*/
// oemui task �źŶ���
#define UI_CMD_Q_SIG      0x0001UL  /* Something on the command queue */
#define UI_KEY_SIG        0x0002UL  /* Key from HS task */
#define UI_RPT_TIMER_SIG  0x0004UL  /* Time to kick watchdog */
#define UI_NV_SIG         0x0008UL  /* Return from NV */
#define UI_WAIT_SIG       0x00010UL
#define UI_RTC_SIG         0x0020UL  
#define UI_KEY_HELD_SIG     0x0040UL // Last key was held
#define UI_KEY_HELD_PWR_SIG   0x0080UL // Last key was held to make PWR

#define AEE_APP_SIG       0x10000UL
#define UI_RUIM_SIG		  0x20000UL
#define AEE_SLEEP_SIG     0x40000UL /* AEE_SLEEP_SIG for Brew 3.1 Sleep. */
#define AEE_SIO_SIG       0x80000UL
#ifdef CUST_EDITION
#define AEE_MEDIA_SIG     0x0400UL
#endif
#define UI_MULTI_STOP_SIG   0x0100UL  // Multitone has ended
#define UI_RING_SIG         0x0200UL  // End of a ring from SND task

#ifdef FEATURE_TOUCHPAD
#define UI_GET_PEN_SIG      0x0800UL /* return from vc task */
#endif
/*==============================================================================
                                 
                                 �������Ͷ���
                                 
==============================================================================*/
// oemui task ֧�ֵ�������
typedef enum 
{
    UI_PROACTIVE_UIM_F,
    
    UI_NUM_UI_COMMANDS,              /* End of command list (note no comma!)    */
    UI_ITEMS_ENUM_PAD = 0x7FFF
} ui_name_type;

// ���� buffer �У� oemui task ���ص�״̬
typedef enum
{
    UI_DONE_S,                      /* General OK return */
    UI_BUSY_S,                      /* Command is being processed */
    UI_BADCMD_S,                    /* Invalid command */
    UI_BADPARM_S                    /* Bad Parameters */
} ui_status_type;

// ͨ������ͷ���ͣ����з��� UI ����������Ը�ͷ���ṹ��ʼ
typedef struct 
{
    q_link_type        link;         /* Queue link */                          
    ui_name_type       cmd;          /* Command to execute */                  
    rex_tcb_type       *task_ptr;    /* Pointer to requesting task TCB */
    rex_sigs_type      sigs;         /* Signal to set upon cmd completion */
    ui_status_type     status;       /* Completion code status */
    q_type             *done_q_ptr;  /* Queue to place this cmd on when done */
} ui_hdr_type;

#ifdef FEATURE_UIM_TOOLKIT
// UIM ԭ����������
typedef struct
{
    ui_hdr_type                hdr;                       /* header */
    byte                       num_bytes;                 /* length */
    byte                       cmd_data[UIM_MAX_CHARS];   /* data */
} ui_proactive_uim_cmd_type;
#endif 

// ui ��������
// ȫ������������塣����ͷ���Ǵ��ڣ���ָ���������ͺ����ԡ����������������
// ������������������ͷ��
typedef union ui_cmd_u 
{
    ui_hdr_type                   hdr;

#ifdef FEATURE_UIM_TOOLKIT
    ui_proactive_uim_cmd_type     proactive_cmd;
#endif
} ui_cmd_type;

#ifdef FEATURE_TTY
typedef enum 
{
    UI_TTY_FULL,
    UI_TTY_TALK,
    UI_TTY_HEAR,
    UI_TTY_OFF
} ui_tty_option_type;
#endif

// ui task ʹ�õ�ȫ�ֲ�������������
typedef struct
{
    boolean offline;                /* phone is offline? */
    boolean pwr;                    /* power on or off? */
    boolean onhook;                 /* phone on hook? */
    boolean headset;                /* Headset attached ? */
    boolean headset_is_mono;        /* TRUE = mono headset */
    boolean extdev;                 /* External device ? */
    boolean mute;                   /* phone muted? */
    boolean spkr;                   /* hands free speaker? */
    boolean txdtmf;                 /* transmit dtmfs? */
    boolean isdebug;                /* debug screen on? */
    boolean powerup;                /* powered up */
    boolean extpwr;                 /* external power */
#ifdef FEATURE_AUDIO_AGC
    boolean       aagc_enable;
#endif /* FEATURE_AUDIO_AGC */

    char  mm_ringer_file[FS_FILENAME_MAX_LENGTH]; /* file to use for Ringer */

#ifdef FEATURE_USB_ISOC_AUDIO
    boolean usb_hfk;               /* USB HFK attached ?   */
#endif /* FEATURE_USB_ISOC_AUDIO */

#ifdef FEATURE_TTY
    ui_tty_option_type tty_option; /* TTY Options */
#endif /* FEATURE_TTY */

#ifdef FEATURE_ANALOG_HFK
    boolean       analog_hfk;
#endif /* FEATURE_ANALOG_HFK */

#ifdef FEATURE_STEREO_DAC
    boolean sdac;                 /* Stereo DAC attached ?        */
#endif
    
    boolean uimunlocked;          /* UIM ���Ƿ�ͨ��PIN��PUK�������¿�����Ϣ�ܹ���ȡ */
} brewui_global_data_type;

#ifndef FEATURE_REX_DYNA_MEM_UI
// free buffers for talking to UI
extern q_type ui_cmd_free_q;      
#endif 


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
boolean   brewui_headsetconnected(void);


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
boolean   brewui_isincall(void);

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
boolean   IsRunAsFactoryTestMode(void);

/*==============================================================================
����: 
    IsRunAsUIMVersion
       
˵��: 
    ���������жϵ�ǰ����Ƿ��п��汾���С�
       
����: 
    none
    
����ֵ:
    TRUE: ��ǰ�������Ϊ�п��汾
    FALSE: ��ǰ�������Ϊ�޿��汾
       
��ע:
    ʹ������İ汾�Ƿ��п��汾������ RTRE ������Σ�ʹ�õ�����汾Ҫ���޿���Ҫ��
    �п�������ֻ��ȡһ��������ѡ��!

==============================================================================*/
boolean   IsRunAsUIMVersion(void);

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
void ui_task(dword dummy   /*lint -esym(715,dummy) */);

#ifdef FEATURE_REX_DYNA_MEM_UI
/*==============================================================================
����: 
    ui_get_cmd
       
˵��: 
    ���������� ui ������û�����������ȡһ������ buffer ���ء�
       
����: 
    none
    
����ֵ:
    ui_cmd_type ָ�롣
       
��ע:
    REX ��NV �� DIAG �� HS ������������У�TMC heap is initialized.
    
==============================================================================*/
ui_cmd_type* ui_get_cmd(void);
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
void ui_cmd(ui_cmd_type *cmd_ptr);

/*==============================================================================
����: 
    ui_get_lcd_handle
       
˵��: 
    Gets the LCD_HANDLE
       
����: 
    none
    
����ֵ:
    lcd_handle_type
       
��ע:
    
==============================================================================*/
lcd_handle_type ui_get_lcd_handle(void);

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
kpd_handle_type ui_get_kpd_handle(void);
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
void oemui_setpwrdncomplete(boolean flag);
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
void oemui_cm_init(void);

/*===========================================================================
FUNCTION InitProvisioning

DESCRIPTION
  initialize provisioning

DEPENDENCIES
  the corresponding initialization functions

SIDE EFFECTS
  None
===========================================================================*/
void InitProvisioning(void);

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
void BrewUI_EnableKeys(boolean flag);

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
void oemui_setbusymode(boolean flag);

int oemui_is_lcd_test_state(int bb);

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
void oemui_unlockuim(void);

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
boolean oemui_uimisunlocked(void);
void oemui_set_powerup_state(boolean flg);
boolean oemui_get_powerup_state(void);

#define APP_MEDIA_ALLOW         0    
#define APP_MEDIA_IMPACT_BY_FM  1
#define APP_MEDIA_IMPACT_BY_MP3 2
extern int app_media_scheduler(void);
#endif // BREWUI_H
