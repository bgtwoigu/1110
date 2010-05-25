#ifndef UIXSND_H
#define UIXSND_H

/*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*


         U S E R   I N T E R F A C E   S O U N D   M A N A G E R

GENERAL DESCRIPTION
  This manages the nasty details of what volume and which path to use
  at which time.

EXTERNALIZED FUNCTIONS

INITIALIZATION AND SEQUENCING REQUIREMENTS

      Copyright (c) 1991 - 2006 by QUALCOMM, Inc.  All Rights Reserved.
*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*/

/* <EJECT> */
/*===========================================================================
                        EDIT HISTORY FOR MODULE

$Header: //depot/asic/qsc60x0/apps/ui/uixsnd.h#2 $

when       who     what, where, why
--------   ---     ----------------------------------------------------------
06/22/06   jks     Added forced audio path support
11/06/02   kar     Mainlined F_MULTIPLE_RINGER_TYPES
03/29/02   kar     Initial MM version
===========================================================================*/

/* <EJECT> */
/*===========================================================================

                     INCLUDE FILES FOR MODULE

===========================================================================*/
#include "target.h"
#include "customer.h"
#include "comdef.h"

#include "snd.h"
#include "nv.h"

/* <EJECT> */
/*===========================================================================

            LOCAL DEFINITIONS AND DECLARATIONS FOR MODULE

This section contains local definitions for constants, macros, types,
variables and other items needed by this module.

===========================================================================*/
// ��������
#define UISND_1ST_VOL 0x1F
#define UISND_2ND_VOL 0x6F
#define UISND_3RD_VOL 0xAF
#define UISND_4TH_VOL 0xFF

// ƽ������
#define UISND_AVG_VOL UISND_2ND_VOL

// The "Off/Silent" setting
#define UISND_VOL_OFF 0
// Maximum settings
#define UISND_MAX_VOL UISND_4TH_VOL

/*
** Define UISND_ESC_VOL for targets that does NOT support escalating
** ringing as well -- in uinsstrt we need to overwrite UISND_ESC_VOL
** to UISND_AVE_VOL value for such targets.
*/
#define UISND_ESC_VOL 0x8F


// UI ϣ�����ŵ��������Ͷ���
typedef enum 
{
    UI_PWRUP_SND,              /* Sounds for power on, ext power off/on   */
                               /*  in use generator     = ringer          */
                               /*  not in use generator = ringer          */
                               /*  volume used          = key beep volume */
    UI_BEEP_SND,               /* Beep snd for most keys                  */
                               /*  in use generator     = ringer          */
                               /*  not in use generator = ringer          */
                               /*  volume used          = key beep volume */
    UI_EARBEEP_SND,            /* Beep for low battery, minute beep etc.. */
                               /*  in use generator     = ear piece       */
                               /*  not in use generator = ringer          */
                               /*  volume used          = ear volume      */
                               /*  in car kit volume    = ringer volume   */
    UI_ALERT_SND,              /* alert ring sound - includes signalling  */
                               /*  in use generator     = ringer          */
                               /*  not in use generator = ringer          */
                               /*  volume used          = ringer volume   */
    UI_CALL_SND,               /* Call feedback sound ringer/earpiece     */
                               /*  in use generator     = ear piece       */
                               /*  not in use generator = ringer          */
                               /*  volume used          = constant        */
    UI_MSG_SND,                /* Message notifications                   */
                               /*  in use generator     = ear piece       */
                               /*  not in use generator = ringer          */
                               /*  volume used          = ringer volume   */
    UI_EXT_PWR_SND,            /* External Power sounds                   */
                               /*  in use generator     = ringer          */
                               /*  not in use generator = ringer          */
                               /*  volume used          = constant        */
    UI_SVC_ALERT_SND,          /* Service Alert sound */
                               /*  in use generator     = ear piece       */
                               /*  not in use generator = ringer          */
                               /*  volume used          = constant        */
    UI_DTMF_SND,               /* DTMF sounds during a call               */
                               /*  in use generator     = ear piece       */
                               /*  not in use generator =  n/a            */
                               /*  volume used          = up to snd task  */
    UI_CALL_EAR_SND            /* CAll feedback sound earpiece/earpiece   */
                               /*  in use generator     = ear piece       */
                               /*  not in use generator = ear piece       */
                               /*  volume used          = constant        */
                               /* these will be generated at the earpiece */
                               /* even if the phone is not in use         */
} uisnd_snd_type;


/*==============================================================================
����: 
    uisnd_voice_vol
       
˵��: 
    Set the TTY device volume. Then also tell sound task.
       
����: 
    vol [in]: the volume
    nv_item [in]: nv item indicating the device
    
����ֵ:
    none
       
��ע:
    ���ñ�����ǰ������ uisnd_snd_init �Ѿ�������
    
==============================================================================*/
void uisnd_voice_vol(byte vol, nv_items_enum_type nv_item);

#ifdef FEATURE_TTY
/*==============================================================================
����: 
    uisnd_tty_vol
       
˵��: 
    Set the TTY device volume. Then also tell sound task.
       
����: 
    vol [in]: the volume
    tty_dev [in]: the TTY device type
    
����ֵ:
    none
       
��ע:
    ���ñ�����ǰ������ uisnd_snd_init �Ѿ�������
    
==============================================================================*/
void uisnd_tty_vol(byte vol, snd_device_type tty_dev);
#endif /* FEATURE_TTY */

#ifdef FEATURE_MULTIMEDIA
/*==============================================================================
����: 
    uisnd_mm_vol
       
˵��: 
    Set the Multimedia volume. Then also tell sound task.
       
����: 
    vol [in]: the volume
    nv_item [in]: nv item indicating the device
    
����ֵ:
    none
       
��ע:
    ���ñ�����ǰ������ uisnd_snd_init �Ѿ�������
    
==============================================================================*/
void uisnd_mm_vol(byte vol, nv_items_enum_type nv_item);

#endif

/*==============================================================================
����: 
    uisnd_beep_vol
       
˵��: 
    Set the beep volume.
       
����: 
    vol [in]: the volume
    nv_item [in]: nv item indicating the device
    
����ֵ:
    none
       
��ע:
    ���ñ�����ǰ������ uisnd_snd_init �Ѿ�������
    
==============================================================================*/
void uisnd_beep_vol(byte vol,  nv_items_enum_type nv_item);

/*==============================================================================
����: 
    uisnd_alert_vol
       
˵��: 
    Set the SMS alert volume.SMS Volume
       
����: 
    vol [in]: the volume
    nv_item [in]: nv item indicating the device
    
����ֵ:
    none
       
��ע:
    ���ñ�����ǰ������ uisnd_snd_init �Ѿ�������
    
==============================================================================*/
void uisnd_alert_vol(byte vol, nv_items_enum_type nv_item);

/*==============================================================================
����: 
    uisnd_GetSoundInfo
       
˵��: 
    Set the ring volume, and tell sound task also.
       
����: 
    vol [in]: the volume
    nv_item [in]: nv item indicating the device
    
����ֵ:
    none
       
��ע:
    ���ñ�����ǰ������ uisnd_snd_init �Ѿ�������
    
==============================================================================*/
void uisnd_ring_vol(byte vol, nv_items_enum_type nv_item);

/*==============================================================================
����: 
    uisnd_set_device
       
˵��: 
    Set the normal and mute input and output device.  If the global gbForceAudioPath
    flag is set, force the audio device to the global gForceAudioDevice, else set the
    audio device through auto-discovery
       
����: 
    speaker_mute [in]: Mute, unmute output
    microphone_mute [in]: Mute, unmute microphone
    change_mute [in]: �Ƿ�ı侲������ want change?
    callback_ptr [in]: Call back function
    client_data [in]: Pointer to Client data
    
����ֵ:
    none
       
��ע:
    ���ñ�����ǰ������ uisnd_snd_init �Ѿ�������
    
==============================================================================*/
void uisnd_set_device(snd_mute_control_type speaker_mute,
                      snd_mute_control_type microphone_mute,
                      boolean               change_mute,
                      snd_cb_func_ptr_type  callback_ptr,
                      const void            *client_data);

/*==============================================================================
����: 
    uisnd_set_forced_audio_path
       
˵��: 
    Get the forced input and output audio path. 
    SND_DEVICE_CURRENT indicated autoselect
       
����: 
    none
    
����ֵ:
    none
       
��ע:
    ���ñ�����ǰ������ uisnd_snd_init �Ѿ�������
    
==============================================================================*/
uint32 uisnd_get_forced_audio_path(void);
                      
/*==============================================================================
����: 
    uisnd_set_forced_audio_path
       
˵��: 
    Set the normal input and output audio path to something specific.
    This setting remains until the requested audio path is changed.
       
����: 
    new_device [in]: �����豸����
    speaker_mute [in]: Mute, unmute output
    microphone_mute [in]: Mute, unmute microphone
    callback_ptr [in]: Call back function
    client_data [in]: Pointer to Client data
    
����ֵ:
    none
       
��ע:
    ���ñ�����ǰ������ uisnd_snd_init �Ѿ�������
    
==============================================================================*/
void uisnd_set_forced_audio_path(snd_device_type        new_device,
                                 snd_mute_control_type  speaker_mute,
                                 snd_mute_control_type  microphone_mute,
                                 snd_cb_func_ptr_type   callback_ptr,
                                 const void             *client_data);

/*==============================================================================
����: 
    uisnd_clear_forced_audio_path
       
˵��: 
    Clear the forced input and output audio path
       
����: 
    none
    
����ֵ:
    none
       
��ע:
    A context switch might occurs
    
==============================================================================*/
void uisnd_clear_forced_audio_path(void);

/*==============================================================================
����: 
    uisnd_snd
       
˵��: 
    Play a sound
       
����: 
    type [in]: �������� type of sound 
    snd [in]:  ��������  which sound
    
����ֵ:
    none
       
��ע:
    ���ñ�����ǰ������ uisnd_snd_init �Ѿ�������
    
==============================================================================*/
void uisnd_snd(uisnd_snd_type  type, snd_sound_id_type   snd);

/*==============================================================================
����: 
    uisnd_sample_snd
       
˵��: 
    Play a sample sound
       
����: 
    type [in]: �������� type of sound 
    snd [in]:  ��������  which sound
    vol [in]: ����
    
����ֵ:
    none
       
��ע:
    ���ñ�����ǰ������ uisnd_snd_init �Ѿ�������
    
==============================================================================*/
void uisnd_sample_snd(uisnd_snd_type  type, snd_sound_id_type snd, byte  vol);

/*==============================================================================
����: 
    uisnd_snd_stop
       
˵��: 
    Stop any sound
       
����: 
    none
    
����ֵ:
    none
       
��ע:
    ���ñ�����ǰ������ uisnd_snd_init �Ѿ�������
    
==============================================================================*/
void uisnd_snd_stop(void);

/*==============================================================================
����: 
    uisnd_multi
       
˵��: 
    Play a multitone
       
����: 
    dotx [in]: transmit over air?
    sound [in]: list of tones
    received [in]: ulti tones received from BS
    
����ֵ:
    none
       
��ע:
    ���ñ�����ǰ������ uisnd_snd_init �Ѿ�������
    
==============================================================================*/
void uisnd_multi(boolean  dotx, snd_compact_tone_type *sound, boolean received);

/*==============================================================================
����: 
    uisnd_multi_stop
       
˵��: 
    Stop any multi tone
       
����: 
    none
    
����ֵ:
    none
       
��ע:
    ���ñ�����ǰ������ uisnd_snd_init �Ѿ�������
    
==============================================================================*/
void uisnd_multi_stop(void);

/*==============================================================================
����: 
    uisnd_tone
       
˵��: 
    Play a tone
       
����: 
    type [in]: �������� type of sound 
    tone [in]: ��������  which tone
    time [in]: ���ų���ʱ�䣬��λΪ���� duration in ms
    
����ֵ:
    none
       
��ע:
    ���ñ�����ǰ������ uisnd_snd_init �Ѿ�������
    
==============================================================================*/
void uisnd_tone(uisnd_snd_type  type,  snd_tone_type  tone, word   time);

/*==============================================================================
����: 
    uisnd_sample_tone
       
˵��: 
    Play a sample tone while adjusting volume
       
����: 
    type [in]: �������� type of sound
    tone [in]: �������� which tone
    time [in]: ���ų���ʱ�䣬��λΪ���� duration in ms
    vol [in]: ����ʹ�õ�����
    
����ֵ:
    none
       
��ע:
    ���ñ�����ǰ������ uisnd_snd_init �Ѿ�������
    
==============================================================================*/
void uisnd_sample_tone(uisnd_snd_type type, snd_tone_type tone, word time, byte vol);

/*==============================================================================
����: 
    uisnd_dtmf
       
˵��: 
    Play a dtmf - a special tone which may be transmitted, and is always
    played out the speaker
       
����: 
    tone [in]: ��������
    time [in]: ���ų���ʱ�䣬��λΪ����
    
����ֵ:
    none
       
��ע:
    ���ñ�����ǰ������ uisnd_snd_init �Ѿ�������
    
==============================================================================*/
void uisnd_dtmf(snd_tone_type   tone,  word time);

#ifdef FEATURE_AUDIO_AGC
/*==============================================================================
����: 
    uisnd_set_aagc
       
˵��: 
    �����򿪻�ر� Audio AGC ������
       
����: 
    enable [in]: ���ر�־
    
����ֵ:
    none
       
��ע:
    ���ñ�����ǰ������ uisnd_snd_init �Ѿ�������
    
==============================================================================*/
void uisnd_set_aagc(boolean enable);
#endif /* FEATURE_AUDIO_AGC */


/*==============================================================================
����: 
    uisnd_tone_stop
       
˵��: 
    Stop any tone
       
����: 
    none
    
����ֵ:
    none
       
��ע:
    ���ñ�����ǰ������ uisnd_snd_init �Ѿ�������
    
==============================================================================*/
void uisnd_tone_stop(void);

/*==============================================================================
����: 
    uisnd_snd_init
       
˵��: 
    Initialize gbrewui -> snd buffers
       
����: 
    none
    
����ֵ:
    none
       
��ע:
    ������������һ��
    
==============================================================================*/
void uisnd_snd_init(void);


/*==============================================================================
����: 
    uisnd_key_sound
       
˵��: 
    Do the beep associated with a key being pressed.
       
����: 
    key [in]: ������
    time [in]: ����ʱ��
    
����ֵ:
    none
       
��ע:
    ���ñ�����ǰ������ uisnd_snd_init �Ѿ�������
    
==============================================================================*/
void uisnd_key_sound(word key,word time);


/*==============================================================================
����: 
    uisnd_call_fail
       
˵��: 
    play call fail sound
       
����: 
    snd [in]: which sound
    
����ֵ:
    none
       
��ע:
    ���ñ�����ǰ������ uisnd_snd_init �Ѿ�������
    
==============================================================================*/
void uisnd_call_fail(snd_sound_id_type snd);

/*==============================================================================
����: 
    uisnd_play_signal
       
˵��: 
    play tone signal or ISDN alert or IS-54B alert
       
����: 
    type [in]: tone signal/ISDN alerting/IS-54B alert
    signal [in]: indicate which signals to play
    pitch [in]: medium, high or low
    
����ֵ:
    none
       
��ע:
    ���ñ�����ǰ������ uisnd_snd_init �Ѿ�������
    
==============================================================================*/
void uisnd_play_signal(byte signal_type, byte signal, byte pitch);

#endif  /* #ifndef UIXSND_H */

