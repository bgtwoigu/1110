#ifndef OEMNVINT_H
#define OEMNVINT_H

/*=============================================================================

FILE: oemnvint.h

SERVICES:  Valid values for CFGI items

GENERAL DESCRIPTION:

PUBLIC CLASSES AND STATIC FUNCTIONS:

INITIALIZATION AND SEQUENCING REQUIREMENTS:
   This file should not be included directly.  It is included by OEMCFGI.h

(c) COPYRIGHT 2002-2006 QUALCOMM Incorporated.
                    All Rights Reserved.

                    QUALCOMM Proprietary
=============================================================================*/


/*=============================================================================

  $Header: //depot/asic/msmshared/apps/StaticExtensions/OEM/Inc/oemnvint.h#19 $
$DateTime: 2008/02/13 14:31:16 $
  $Author: carls $
  $Change: 609141 $
                      EDIT HISTORY FOR FILE

  This section contains comments describing changes made to the module.
  Notice that changes are listed in reverse chronological order.

when       who     what, where, why
--------   ---     ---------------------------------------------------------
02/12/08   SAM     Typecasting OEMTTY to be uint32
10/19/06   JAS     Fixing backlight values to be seconds-based.
                   Removing VOICEPRIVACY values, now defined elsewhere
07/12/06   JAS     Adding explicit start value for the OEMTTY enum
06/14/06   JD      Removed defines for old SMS channel selection
06/13/06   JD      Added defines for VOICEPRIVACY
10/10/05   PMD     Added OEMNV_SMS_RETRY_INTERVAL define for 5 seconds
09/22/05   cvs     Support for Stereo USB Carkit
09/08/05   PMD     Added OEMNV_SMS_RETRY_PERIOD define for 240 seconds
06/10/05   cvs     Support for Smart USB Carkit
06/07/05   sun     Added support for CCBS
11/17/04   SUN     Added new enum for Beep Length
04/14/04   SUN     Added support for CFGI_TTY
03/08/04   PMD     Added Support for WMS Application Mode
01/28/03   RAM     Added mask for SpeakerPhone.
11/23/03   SUN     Added mask for CFGI Items
10/31/03   PMD     Added IConfig Items for SMS Cell Broadcast Settings
                   Added IConfig Items for SMS Alert Settings
08/05/03   PMD     Added Mode preferences (CDMA/GSM) for SMS
09/16/02   sg      Added max length of RF CAL Version string and an Error Log
                   structure for Debug Applet
09/13/02   mjv     Added Dynamic R-UIM support
09/11/02   mjv     CR #602: Removed the 'Digial Pref' and 'Analog Pref'
                   mode preference.
08/29/02   sg      Added the TIMEFORM values
08/27/02   mjv     Added values for CFGI_HEADSET_AUTOANSWER and
                   CFGI_ANYKEY_ANSWER
07/30/02   mjv     Added OEMNV_VOICEMAIL_MAXLEN
07/02/02   mjv     Added values relating to CFGI_SMS_MO_ENCODING,
                   CFGI_SMS_IS41_WORKAROUND, CFGI_IMSI_S, CFGI_MODE_PREF,
                   CFGI_AMPS_REG_TYPE, and CFGI_DATA_DIALSTRING.
06/14/02   mjv     Added values for the new Service Programming CFGI items,
                   better documented the existing items.
06/04/02   lnr     Fixed defects 384 & 402. Make the maximum SMS message
                   payload and the SMS MO Channel selection as configurable
                   items.
06/02/02   mjv     Removed OEMNV_PINLOCK_ON and OEMNV_PINLOCK_OFF
04/17/02   mjv     Swapped values of OEMNV_SMARTSOUND_DISABLE and
                   OEMNV_SMARTSOUND_ENABLE.  Their values are now 0 and 1
                   respectively.
01/23/02   mjv     Added edit history

=============================================================================*/

#if !defined( OEMCFGI_H) && !defined( AEE_SIMULATOR)
#error oemnvint.h must not be included directly, include OEMConfig.h instead
#endif
#include "OEMFeatures.h"
#include "OEMText.h"
#include "AEEFile.h"
/************************************************************************/
/*                                                                      */
/*  Setting Menu                                                        */
/*                                                                      */
/************************************************************************/
// Phone Disposition -- CFGI_PROFILE_CUR_NUMBER
///////////////////////////////////////////////////////////////////////// 
#define OEMNV_PROFILE_NORMALMODE          0                          // NORMALMODE
#define OEMNV_PROFILE_QUIETMODE           1                          // QUIETMODE
#define OEMNV_PROFILE_MEETING             2                          // MEETING
#define OEMNV_PROFILE_NOISEMODE           3                          // NOISEMODE
#define OEMNV_PROFILE_CARMODE             4                          // CARMODE 
#define PROFILENUMBER                     5                          // 情景模式种类
//默认的短信铃声
#define OEMNV_SMS_RINGER_ID 78 
/////////////////////////////////////////////////////////////////////////
// 情景模式出厂设置
/////////////////////////////////////////////////////////////////////////                     
#ifndef WIN32
// 来电提示 
#if defined FEATURE_CARRIER_THAILAND_CAT
// 来电提示 
#define OEMNV_ALERT_TYPE_INIT {OEMNV_ALERTTYPE_VIBANDRINGER,OEMNV_ALERTTYPE_OFF,OEMNV_ALERTTYPE_VIB,OEMNV_ALERTTYPE_VIBRINGER,OEMNV_ALERTTYPE_VIBRINGER} 
#else
// 来电提示 
#define OEMNV_ALERT_TYPE_INIT {OEMNV_ALERTTYPE_RINGER,OEMNV_ALERTTYPE_OFF,OEMNV_ALERTTYPE_VIB,OEMNV_ALERTTYPE_VIBRINGER,OEMNV_ALERTTYPE_VIBRINGER} 
#endif //defined FEATURE_CARRIER_THAILAND_CAT
// 短信提示  
#define OEMNV_SMS_RINGER_INIT {OEMNV_SMS_RING,OEMNV_SMS_OFF,OEMNV_SMS_VIBONLY,OEMNV_SMS_RINGVIB,OEMNV_SMS_RINGVIB} 
// 来电铃声选择           
#define OEMNV_CALL_RINGER_INIT {{{0}, OEMNV_DEFAULTRINGER, OEMNV_MID_RINGER},{{0}, OEMNV_DEFAULTRINGER, OEMNV_MID_RINGER},{{0},OEMNV_DEFAULTRINGER,OEMNV_MID_RINGER},{{0},OEMNV_DEFAULTRINGER,OEMNV_MID_RINGER},{{0},OEMNV_DEFAULTRINGER,OEMNV_MID_RINGER}} 
// 闹钟铃声选择              
#define OEMNV_ALARM_RINGER_INIT {{{0}, OEMNV_ALARM_RINGER, OEMNV_MID_RINGER},{{0}, OEMNV_ALARM_RINGER, OEMNV_MID_RINGER},{{0},OEMNV_ALARM_RINGER,OEMNV_MID_RINGER},{{0},OEMNV_ALARM_RINGER,OEMNV_MID_RINGER},{{0},OEMNV_ALARM_RINGER,OEMNV_MID_RINGER}} 
// 短信铃声选择       
#define OEMNV_SMS_RINGER_ID_INIT {{{0}, OEMNV_SMS_RINGER_ID, OEMNV_MID_RINGER},{{0}, OEMNV_SMS_RINGER_ID, OEMNV_MID_RINGER},{{0},OEMNV_SMS_RINGER_ID,OEMNV_MID_RINGER},{{0},OEMNV_SMS_RINGER_ID,OEMNV_MID_RINGER},{{0},OEMNV_SMS_RINGER_ID,OEMNV_MID_RINGER}}
// 未接来电提醒       
#define OEMNV_MISSED_CALL_ALERT_INIT {OEMNV_ALERT_ENABLE,OEMNV_ALERT_DISABLE,OEMNV_ALERT_DISABLE,OEMNV_ALERT_ENABLE,OEMNV_ALERT_ENABLE} 
// 开机音乐      
#define OEMNV_STARTUP_MUSIC_INIT {OEMNV_STARTUP_MUSIC,OEMNV_STARTUP_MUSIC,OEMNV_STARTUP_MUSIC,OEMNV_STARTUP_MUSIC,OEMNV_STARTUP_MUSIC} 
// 关机音乐        
#define OEMNV_SHUTDOWN_MUSIC_INIT {OEMNV_SHUTDOWN_MUSIC,OEMNV_SHUTDOWN_MUSIC,OEMNV_SHUTDOWN_MUSIC,OEMNV_SHUTDOWN_MUSIC,OEMNV_SHUTDOWN_MUSIC} 
// 开关机铃声提示       
#define OEMNV_POWERONOFF_ALERT_INIT {OEMNV_POWERONOFF_ENABLE,OEMNV_POWERONOFF_DISABLE,OEMNV_POWERONOFF_DISABLE,OEMNV_POWERONOFF_ENABLE,OEMNV_POWERONOFF_ENABLE}
// 自动接听                
#define OEMNV_HEADSET_AUTOANSWER_INIT {OEMNV_HEADSET_AUTOANSWER_OFF,OEMNV_HEADSET_AUTOANSWER_OFF,OEMNV_HEADSET_AUTOANSWER_OFF,OEMNV_HEADSET_AUTOANSWER_OFF,OEMNV_HEADSET_AUTOANSWER_5SEC} 
// 多彩按键音     
#define OEMNV_KEYSND_TYPE_INIT {SND_METHOD_KEY_BEEP,SND_METHOD_KEY_BEEP,SND_METHOD_KEY_BEEP,SND_METHOD_KEY_BEEP,SND_METHOD_KEY_BEEP} 
#ifdef FEATURE_KEYTONE_LONG
// 按键音长度     
#define OEMNV_KEYTONE_LENGTH_INIT {OEMNV_KEYTONE_LONG,OEMNV_KEYTONE_LONG,OEMNV_KEYTONE_LONG,OEMNV_KEYTONE_LONG,OEMNV_KEYTONE_LONG} 
#else
// 按键音长度     
#define OEMNV_KEYTONE_LENGTH_INIT {OEMNV_KEYTONE_NORMAL,OEMNV_KEYTONE_NORMAL,OEMNV_KEYTONE_NORMAL,OEMNV_KEYTONE_NORMAL,OEMNV_KEYTONE_NORMAL} 
#endif //

#else

// 来电提示 
#define OEMNV_ALERT_TYPE_INIT {OEMNV_ALERTTYPE_RINGER,OEMNV_ALERTTYPE_OFF,OEMNV_ALERTTYPE_VIB,OEMNV_ALERTTYPE_VIBRINGER,OEMNV_ALERTTYPE_VIBRINGER} 
// 短信提示  
#define OEMNV_SMS_RINGER_INIT {OEMNV_SMS_RING,OEMNV_SMS_OFF,OEMNV_SMS_VIBONLY,OEMNV_SMS_RINGVIB,OEMNV_SMS_RINGVIB} 
// 来电铃声选择           
#define OEMNV_CALL_RINGER_INIT {OEMNV_DEFAULTRINGER,OEMNV_DEFAULTRINGER,OEMNV_DEFAULTRINGER,OEMNV_DEFAULTRINGER,OEMNV_DEFAULTRINGER} 
// 闹钟铃声选择              
#define OEMNV_ALARM_RINGER_INIT {OEMNV_ALARM_RINGER,OEMNV_ALARM_RINGER,OEMNV_ALARM_RINGER,OEMNV_ALARM_RINGER,OEMNV_ALARM_RINGER} 
// 短信铃声选择       
#define OEMNV_SMS_RINGER_ID_INIT {OEMNV_SMS_RINGER_ID,OEMNV_SMS_RINGER_ID,OEMNV_SMS_RINGER_ID,OEMNV_SMS_RINGER_ID,OEMNV_SMS_RINGER_ID}
// 未接来电提醒       
#define OEMNV_MISSED_CALL_ALERT_INIT {OEMNV_ALERT_ENABLE,OEMNV_ALERT_DISABLE,OEMNV_ALERT_DISABLE,OEMNV_ALERT_ENABLE,OEMNV_ALERT_ENABLE} 
// 开机音乐      
#define OEMNV_STARTUP_MUSIC_INIT {OEMNV_STARTUP_MUSIC,OEMNV_STARTUP_MUSIC,OEMNV_STARTUP_MUSIC,OEMNV_STARTUP_MUSIC,OEMNV_STARTUP_MUSIC} 
// 关机音乐        
#define OEMNV_SHUTDOWN_MUSIC_INIT {OEMNV_SHUTDOWN_MUSIC,OEMNV_SHUTDOWN_MUSIC,OEMNV_SHUTDOWN_MUSIC,OEMNV_SHUTDOWN_MUSIC,OEMNV_SHUTDOWN_MUSIC} 
// 开关机铃声提示       
#define OEMNV_POWERONOFF_ALERT_INIT {OEMNV_POWERONOFF_ENABLE,OEMNV_POWERONOFF_DISABLE,OEMNV_POWERONOFF_DISABLE,OEMNV_POWERONOFF_ENABLE,OEMNV_POWERONOFF_ENABLE}
// 自动接听                
#define OEMNV_HEADSET_AUTOANSWER_INIT {OEMNV_HEADSET_AUTOANSWER_OFF,OEMNV_HEADSET_AUTOANSWER_OFF,OEMNV_HEADSET_AUTOANSWER_OFF,OEMNV_HEADSET_AUTOANSWER_OFF,OEMNV_HEADSET_AUTOANSWER_5SEC} 
// 多彩按键音     
#define OEMNV_KEYSND_TYPE_INIT {0,0,0,0,0} //{SND_METHOD_KEY_BEEP,SND_METHOD_KEY_BEEP,SND_METHOD_KEY_BEEP,SND_METHOD_KEY_BEEP,SND_METHOD_KEY_BEEP} 
#ifdef FEATURE_KEYTONE_LONG
// 按键音长度     
#define OEMNV_KEYTONE_LENGTH_INIT {OEMNV_KEYTONE_LONG,OEMNV_KEYTONE_LONG,OEMNV_KEYTONE_LONG,OEMNV_KEYTONE_LONG,OEMNV_KEYTONE_LONG} 
#else
// 按键音长度     
#define OEMNV_KEYTONE_LENGTH_INIT {OEMNV_KEYTONE_NORMAL,OEMNV_KEYTONE_NORMAL,OEMNV_KEYTONE_NORMAL,OEMNV_KEYTONE_NORMAL,OEMNV_KEYTONE_NORMAL} 
#endif //

#endif
// 铃声音量     
#define OEMNV_RINGER_VOL_INIT {3,0,0,4,4}  
// 耳机音量    
#define OEMNV_EAR_VOL_INIT {3,3,3,4,4}  
// 键盘音量     
#define OEMNV_BEEP_VOL_INIT {3,0,0,4,4}     

// Phone Disposition -- CFGI_SCREENSAVER_TIME
/////////////////////////////////////////////////////////////////////////
#define OEMNV_SCREENSAVER_TIME_30SEC       0
#define OEMNV_SCREENSAVER_TIME_2MIN        1
#define OEMNV_SCREENSAVER_TIME_5MIN        2  
/////////////////////////////////////////////////////////////////////////
// Phone Disposition -- CFGI_SCREENSAVER_ANIMATION
/////////////////////////////////////////////////////////////////////////
#define OEMNV_SCREENSAVER_ANIMATION0      0    
#define OEMNV_SCREENSAVER_ANIMATION1      1  
#define OEMNV_SCREENSAVER_ANIMATION2      2  
#define OEMNV_SCREENSAVER_ANIMATION3      3  
#define OEMNV_SCREENSAVER_ANIMATION4      4 

#define OEMNV_CONTRAST_OFFSET       0x8
#define OEMNV_CONTRAST_BASE           0x22

#define OEMNV_CONTRAST_LEVEL_1      (OEMNV_CONTRAST_BASE)
#define OEMNV_CONTRAST_LEVEL_2      (OEMNV_CONTRAST_BASE + OEMNV_CONTRAST_OFFSET)
#define OEMNV_CONTRAST_LEVEL_3      (OEMNV_CONTRAST_BASE + 2 * OEMNV_CONTRAST_OFFSET)
#define OEMNV_CONTRAST_LEVEL_4      (OEMNV_CONTRAST_BASE + 3 * OEMNV_CONTRAST_OFFSET)
#define OEMNV_CONTRAST_LEVEL_5      (OEMNV_CONTRAST_BASE + 4 * OEMNV_CONTRAST_OFFSET)
#define OEMNV_CONTRAST_LEVEL_6      (OEMNV_CONTRAST_BASE + 5 * OEMNV_CONTRAST_OFFSET)
#define OEMNV_CONTRAST_LEVEL_7      (OEMNV_CONTRAST_BASE + 6 * OEMNV_CONTRAST_OFFSET)

#define OEMNV_BACKLIGHT_BASE         0x1
#define OEMNV_BACKLIGHT_OFFSET       0x11

#ifndef CUST_EDITION
#define OEMNV_BACKLIGHT_LEVEL_1      (OEMNV_BACKLIGHT_BASE)
#define OEMNV_BACKLIGHT_LEVEL_2      (OEMNV_BACKLIGHT_BASE + OEMNV_BACKLIGHT_OFFSET)
#define OEMNV_BACKLIGHT_LEVEL_3      (OEMNV_BACKLIGHT_BASE + 2 * OEMNV_BACKLIGHT_OFFSET)
#define OEMNV_BACKLIGHT_LEVEL_4      (OEMNV_BACKLIGHT_BASE + 3 * OEMNV_BACKLIGHT_OFFSET)
#define OEMNV_BACKLIGHT_LEVEL_5      (OEMNV_BACKLIGHT_BASE + 4 * OEMNV_BACKLIGHT_OFFSET)
#define OEMNV_BACKLIGHT_LEVEL_6      (OEMNV_BACKLIGHT_BASE + 5 * OEMNV_BACKLIGHT_OFFSET)
#define OEMNV_BACKLIGHT_LEVEL_7      (OEMNV_BACKLIGHT_BASE + 7 * OEMNV_BACKLIGHT_OFFSET + OEMNV_BACKLIGHT_OFFSET) //增加差别，否则不明显。
#else
#define OEMNV_BACKLIGHT_LEVEL_DARK        1
#define OEMNV_BACKLIGHT_LEVEL_NORMAL      3
#define OEMNV_BACKLIGHT_LEVEL_BRIGHT      10
#endif

#define OEMNV_BACKLIGHT_LEVEL       (OEMNV_BACKLIGHT_LEVEL_NORMAL) 
//endif


#define OEMNV_CONTRAST_LEVEL_DEFULT 5
#define OEM_AUTO_ANSWER_MODE        0x4 /*only send key answer*/
#define OEM_ANYKEY_ANSWER_MODE    0x1 /*only any key answer*/

#ifdef FEATRUE_AUTO_POWER
typedef PACKED struct _Auto_Power_Cfg
{
    uint16     bStateOn;        //闹钟状态
    uint32     dwWATime;        //闹钟时间,毫秒表示
} Auto_Power_Cfg;
#endif

typedef PACKED struct _PowerDown_Alarm_Cfg
{
    uint16     bStateOn;        //闹钟状态
    uint16     alarm_id;
    uint32     dwWATime;        //闹钟时间,毫秒表示
} PowerDown_Alarm_Cfg;

#ifdef FEATRUE_SET_IP_NUMBER
#define OEM_IP_NUMBER_MAX           10
#define OEM_IP_NUMBER_COUNT       10
typedef PACKED struct _IP_Number_Cfg
{
    uint16     bStateOn;
    uint16     ip_count;
    char       default_number[OEM_IP_NUMBER_MAX+1];
    char       Number[OEM_IP_NUMBER_COUNT * (OEM_IP_NUMBER_MAX+1)+1];
}IP_Number_Cfg;
#endif
#ifdef FEATRUE_KEY_PAD_CTL
typedef PACKED struct _Key_pad_Cfg
{
    uint16     bStateOn;
    uint32     from_Time;
    uint32     to_Time;
} Key_pad_Cfg;
#endif
/////////////////////////////////////////////////////////////////////////
// Silence All -- CFGI_SILENCEALL
/////////////////////////////////////////////////////////////////////////
#define OEMNV_SA_NORMAL              0
#define OEMNV_SA_VIBONLY             1
#define OEMNV_SA_LITONLY             2

/////////////////////////////////////////////////////////////////////////
// Auto Key Guard -- CFGI_AUTOKEYGUARD
/////////////////////////////////////////////////////////////////////////
#define   OEMNV_AKG_OFF              0
#define   OEMNV_AKG_ON               1
#define   OEMNV_AKG_30SEC            2
#define   OEMNV_AKG_2MIN             3
#define   OEMNV_AKG_5MIN             4

/////////////////////////////////////////////////////////////////////////
// BackLight On/Off with External Power -- CFGI_EXTPWR_BK_LIGHT
/////////////////////////////////////////////////////////////////////////
#define   OEMNV_EXTPWR_BL_OFF         0
#define   OEMNV_EXTPWR_BL_ON          1


/////////////////////////////////////////////////////////////////////////
// Headset Ring  -- CFGI_HEADSET_RNG
/////////////////////////////////////////////////////////////////////////
#define   OEMNV_HEADSET_RNG_OFF       0
#define   OEMNV_HEADSET_RNG_ON        1

/////////////////////////////////////////////////////////////////////////
// Anykey Answer  -- CFGI_ANYKEY_ANSWER
/////////////////////////////////////////////////////////////////////////
#define   OEMNV_ANYKEY_ANSWER_OFF       0
#define   OEMNV_ANYKEY_ANSWER_ON        1

/////////////////////////////////////////////////////////////////////////
// Headset Autoanswer  -- CFGI_HEADSET_AUTOANSWER
/////////////////////////////////////////////////////////////////////////
#define   OEMNV_HEADSET_AUTOANSWER_OFF       0
#define   OEMNV_HEADSET_AUTOANSWER_2SEC      2
#define   OEMNV_HEADSET_AUTOANSWER_5SEC      5
#define   OEMNV_HEADSET_AUTOANSWER_10SEC    10


/////////////////////////////////////////////////////////////////////////
// Alert -- CFGI_MISSED_CALL_ALERT, CFGI_ROAMING_ALERT,
//          CFGI_SVC_ALERT, CFGI_INTERNET_ALERT, CFGI_MINUTE_ALERT
/////////////////////////////////////////////////////////////////////////
#define   OEMNV_ALERT_DISABLE         0
#define   OEMNV_ALERT_ENABLE          1

/************************************************************************/
/*                                                                      */
/*  Security Menu                                                      */
/*                                                                      */
/************************************************************************/

/////////////////////////////////////////////////////////////////////////
// Security Menu Software Based Lock Code -- CFGI_LOCKCODE
/////////////////////////////////////////////////////////////////////////

// maximum number of AECHARS in the lock code (including a NULL word)
#define OEMNV_LOCKPIN_MAXLEN 8
#define OEMNV_LOCKCODE_MAXLEN 5
#define OEMNV_LOCKUIM_MAXLEN 10

#define OEMNV_DEFAULT_BANNER     "OT-I650C"   //繁体版黑白机

#ifdef FEATURE_ANIMATION_POWERUPDOWN
#define  OEMNV_STARTUPANI   "\\image\\wallpaper\\kawind_8.bmp"  
#define  OEMNV_POWEROFFANI  "\\image\\wallpaper\\gdiamond_8.bmp"
#endif

#if defined FEATURE_APP_MEDIAGALLERY
#define  OEMNV_WALLPAPER    "fs:/hsmm/pictures/wallpaper01.png"
#else
#define  OEMNV_WALLPAPER    "fs:/image/wallpaper/wallpaper01.png"
#endif
#ifdef FEATURE_SCREEN_SAVE
#define  OEMNV_SCREENSAVE_TYPE "\\image\\wallpaper\\w1.bmp"
#endif
/*

*/
#define   OEMNV_LOCK_RUIM       1
#define   OEMNV_LOCK_MCCMNC_ARRSIZE 10
#define   OEMNV_PHONE_PASSWORD_CHECK       0                            // CFGI_PHONE_PASSWORD_CHECK,  //type = boolean
#define   OEMNV_RESTRICT_OUTGOING_ALL               1
#define   OEMNV_RESTRICT_OUTGOING_INTERNATIONAL     2
#define   OEMNV_RESTRICT_OUTGOING_INTERNAL          3
#define   OEMNV_RESTRICT_OUTGOING_OUTCONTACT        4
#define   OEMNV_RESTRICT_OUTGOING_OFF               0

#define   OEMNV_RESTRICT_INCOMING_ALL               1
#define   OEMNV_RESTRICT_INCOMING_OUTCONTACT        2
#define   OEMNV_RESTRICT_INCOMING_OFF               0
/***********************************************************************/
/*                                                                     */
/*  Sound Menu                                                       */
/*                                                                     */
/***********************************************************************/
/////////////////////////////////////////////////////////////////////////
// Volume -- CFGI_RINGER_VOL,CFGI_EAR_VOL,CFGI_BEEP_VOL
/////////////////////////////////////////////////////////////////////////
#define   OEMNV_VOLUME_OFF         0
#define   OEMNV_VOLUME_LOW         1
#define   OEMNV_VOLUME_MID         2
#define   OEMNV_VOLUME_HIGH        3
#define   OEMNV_VOLUME_ESCALATING  4  // only valid for CFGI_RINGER_VOL
#define   OEMNV_SMS_OFF      0
#define   OEMNV_SMS_RING     1
#define   OEMNV_SMS_VIBONLY  2
#define   OEMNV_SMS_RINGVIB  3
#define   OEMNV_SMS_VIBANDRINGER  4
#define   OEMNV_DEFAULTRINGER        4
#define   OEMNV_ALARM_RINGER  1 
#define   OEMNV_STARTUP_MUSIC 11
#define   OEMNV_SHUTDOWN_MUSIC 12

//开关机铃声提示
#define   OEMNV_POWERONOFF_ENABLE   1
#define   OEMNV_POWERONOFF_DISABLE  0

//多彩按键音
#define   OEMNV_KEYSND_DEFAULT       0  
#define   OEMNV_KEYSND_MUSICBOX      1
#define   OEMNV_KEYSND_GUITAR        2
#define   OEMNV_KEYSND_TRUMPET       3
/////////////////////////////////////////////////////////////////////////
// Key Tone Length -- CFGI_KEYTONE_LENGTH
/////////////////////////////////////////////////////////////////////////
#define   OEMNV_KEYTONE_NORMAL    0
#define   OEMNV_KEYTONE_LONG     1

/////////////////////////////////////////////////////////////////////////
// Key Beep Sound -- CFGI_KEYBEEP_SOUND
/////////////////////////////////////////////////////////////////////////
#define   OEMNV_KEYBEEP_TONE     0
#define   OEMNV_KEYBEEP_CLICK    1

/////////////////////////////////////////////////////////////////////////
// Smart Sound -- CFGI_SMART_SOUND
/////////////////////////////////////////////////////////////////////////
#define   OEMNV_SMARTSOUND_ENABLE   1
#define   OEMNV_SMARTSOUND_DISABLE  0

/////////////////////////////////////////////////////////////////////////
// Alert Type -- CFGI_ALERT_TYPE
/////////////////////////////////////////////////////////////////////////

#define   OEMNV_ALERTTYPE_OFF         0
#define   OEMNV_ALERTTYPE_RINGER      1
#define   OEMNV_ALERTTYPE_VIB         2
#define   OEMNV_ALERTTYPE_VIBRINGER   3
#define   OEMNV_ALERTTYPE_VIBANDRINGER   4

/***********************************************************************/
/*                                                                     */
/*  Display Menu                                                       */
/*                                                                     */
/***********************************************************************/
/////////////////////////////////////////////////////////////////////////
// Backlighting -- CFGI_BACK_LIGHT
/////////////////////////////////////////////////////////////////////////
#define   OEMNV_BL_OFF    0
#define   OEMNV_BL_10S     10
#define   OEMNV_BL_30S     30
#define   OEMNV_BL_50S     50
#define   OEMNV_BL_ALWAYS_ON     100
#define   OEMNV_BL_10CALL  3
#define   OEMNV_BL_30CALL  4

/////////////////////////////////////////////////////////////////////////
// Menu Format -- CFGI_MENU_FORMAT
/////////////////////////////////////////////////////////////////////////
#define   OEMNV_MENUFORM_LARGE    0
#define   OEMNV_MENUFORM_SMALL    1

/////////////////////////////////////////////////////////////////////////
// Time Format -- CFGI_TIME_FORMAT
/////////////////////////////////////////////////////////////////////////
#define   OEMNV_TIMEFORM_INVALID  0
#define   OEMNV_TIMEFORM_AMPM    2
#define   OEMNV_TIMEFORM_24HR    1
#ifdef FEATURE_TIME_DATA_SETTING
#define   OEMNV_DATEFORM_INVALID  0
#define   OEMNV_DATEFORM_DMY    1
#define   OEMNV_DATEFORM_MDY    2
#define   OEMNV_DATEFORM_YMD    3
#endif
#ifdef FEATURE_DOUBLE_SIM_CARD
#define   OEMNV_SIMFORM_INVALID  0
#define   OEMNV_SIMFORM_ONE    1
#define   OEMNV_SIMFORM_TWO    2
#endif
/////////////////////////////////////////////////////////////////////////
// Banner -- CFGI_BNNER
/////////////////////////////////////////////////////////////////////////

// maximum number of AECHARS in the banner (including the NULL word)
#define OEMNV_BANNER_MAXLEN  13

/************************************************************************/
/*                                                                      */
/*  SMS Settings                                                        */
/*                                                                      */
/************************************************************************/

/////////////////////////////////////////////////////////////////////////
// Maximum Message Payload -- CFGI_SMS_MAX_MSG_PAYLOAD
/////////////////////////////////////////////////////////////////////////
#define OEMNV_SMS_MAX_MSG_PAYLOAD      180

/////////////////////////////////////////////////////////////////////////
// SMS Service Option (to send a mobile originated SMS message) --
// CFGI_SMS_SERVICE_OPTION
/////////////////////////////////////////////////////////////////////////
#define OEMNV_SMS_SO_NONE           0
#define OEMNV_SMS_SO_SO6            1
#define OEMNV_SMS_SO_SO14           2

/////////////////////////////////////////////////////////////////////////
// SMS MT Timestamp Ajust -- CFGI_SMS_TIMESTAMP
/////////////////////////////////////////////////////////////////////////
#define OEMNV_SMS_TIMESTAMP_ADJUST        0
#define OEMNV_SMS_TIMESTAMP_ASRECEIVED    1

/////////////////////////////////////////////////////////////////////////
// SMS IS-41 Workaround -- CFGI_SMS_IS41_WORKAROUND
/////////////////////////////////////////////////////////////////////////

// Mobile station will originate SMS to an e-mail address using the IS41
// workaround solution.
#define OEMNV_SMS_EMAIL_ADDRESSING_IS41      0

// Mobile station will originate SMS to an e-mail address using
// IS637-A format.
#define OEMNV_SMS_EMAIL_ADDRESSING_STANDARD  1

/////////////////////////////////////////////////////////////////////////
// SMS MO Encoding -- CFGI_SMS_MO_ENCODING
/////////////////////////////////////////////////////////////////////////

// Encode Mobile Originated messages in 7-bit ASCII
#define OEMNV_SMS_MO_ENCODING_7BIT           0

// Encode Mobile Originated messages in UNICODE
#define OEMNV_SMS_MO_ENCODING_UNICODE        1

#if defined(FEATURE_CARRIER_ANGOLA_MOVICEL) || defined(FEATURE_CARRIER_MAROC_WANA)|| defined (FEATURE_CARRIER_ISRAEL_PELEPHONE)
// Encode Mobile Originated messages in Octet
#define OEMNV_SMS_MO_ENCODING_OCTET          2
#endif
// Encode mode to CDMA
#define OEMNV_SMS_MODE_CDMA                  0

// Encode mode to GSM / UMTS (GW)
#define OEMNV_SMS_MODE_GW                    1

// WMS Application User Mode
#define OEMNV_SMS_APP_USER                   0

// WMS Application Test Mode
#define OEMNV_SMS_APP_TEST                   1

// Disable Cell Broadcast Auto Delete
#define OEMNV_SMS_CB_DISABLE_AUTO_DELETE     0

// Enable Cell Broadcast Auto Delete
#define OEMNV_SMS_CB_ENABLE_AUTO_DELETE      1

// Disable Cell Broadcast Silent Alert
#define OEMNV_SMS_CB_DISABLE_SILENT_ALERT    0

// Enable Cell Broadcast Silent Alert
#define OEMNV_SMS_CB_ENABLE_SILENT_ALERT     1

// Disable Cell Broadcast Language Filter
#define OEMNV_SMS_CB_DISABLE_LANG_FILTER     0

// Enable Cell Broadcast Language Filter
#define OEMNV_SMS_CB_ENABLE_LANG_FILTER      1

// Disable SMS Display Alert
#define OEMNV_SMS_ALERT_DISPLAY_DISABLE      0

// Enable SMS Display Alert
#define OEMNV_SMS_ALERT_DISPLAY_ENABLE       1

// Disable SMS Sound Alert
#define OEMNV_SMS_ALERT_SOUND_DISABLE        0

// Enable SMS Sound Alert
#define OEMNV_SMS_ALERT_SOUND_ENABLE         1

// SMS Alert Volume Level 0 (Mute)
#define OEMNV_SMS_ALERT_VOLUME_0             0

// SMS Alert Volume Level 1
#define OEMNV_SMS_ALERT_VOLUME_1             1

// SMS Alert Volume Level 2
#define OEMNV_SMS_ALERT_VOLUME_2             2

// SMS Alert Volume Level 3
#define OEMNV_SMS_ALERT_VOLUME_3             3

// SMS Alert Volume Level 4
#define OEMNV_SMS_ALERT_VOLUME_4             4

// Retry Period
#define OEMNV_SMS_RETRY_PERIOD             240

// Retry Interval
#define OEMNV_SMS_RETRY_INTERVAL             5
// messages validity period - relative
#define OEMNV_SMS_VALIDITYPERIOD_MAX        245 
#define OEMNV_SMS_VALIDITYPERIOD_1OUR       11 
#define OEMNV_SMS_VALIDITYPERIOD_6OURS      71 
#define OEMNV_SMS_VALIDITYPERIOD_24OURS     167 
#define OEMNV_SMS_VALIDITYPERIOD_3DAYS      169 
#define OEMNV_SMS_VALIDITYPERIOD_1WEEK      173 

/////////////////////////////////////////////////////////////////////////
// Voice Mail Number -- CFGI_VOICEMAIL_NUMBER
/////////////////////////////////////////////////////////////////////////

// Maximum number of AECHARS in the voice mail number
// (including the NULL word)
#define OEMNV_VOICEMAIL_MAXLEN              32

/************************************************************************/
/*                                                                      */
/*  Service Programming                                                 */
/*                                                                      */
/************************************************************************/
/////////////////////////////////////////////////////////////////////////
// Phone Number -- CFGI_PHONE_NUMBER
/////////////////////////////////////////////////////////////////////////

// Maximum number of AECHARS in the phone number (including the NULL word)
#define OEMNV_PHONENUMBER_MAXLEN 16

/////////////////////////////////////////////////////////////////////////
// Service Programming Code -- CFGI_SECCODE
/////////////////////////////////////////////////////////////////////////

// Number of AECHARS in the SPC (including the NULL word)
#define OEMNV_SECCODE_LENGTH  7


/////////////////////////////////////////////////////////////////////////
// SysMenu Code -- CFGI_RFCAL_VERSION & CFGI_RFCAL_DATE
/////////////////////////////////////////////////////////////////////////

// The max string size of RF CAL Version
#define OEMNV_RFCALVER_LEN  9


/////////////////////////////////////////////////////////////////////////
// IMSI_S -- CFGI_IMSI_S
/////////////////////////////////////////////////////////////////////////

// Number of AECHARS in IMSI_S (including the NULL word)
#define OEMNV_IMSI_S_LENGTH  11

/////////////////////////////////////////////////////////////////////////
// "Home" SID+NID pairs for CDMA acquisition -- CFGI_HOME_SIDNID_LIST
/////////////////////////////////////////////////////////////////////////

// Number of AEEConfigSIDNIDPairTypes array elements
#define OEMNV_HOME_SIDNID_ARRSIZE 20

typedef PACKED struct {
  uint16 sid;
  uint16 nid;
} AEEConfigSIDNIDPairType;


/////////////////////////////////////////////////////////////////////////
// SID+NID pairs to be locked out from CDMA acquisition --
// CFGI_LOCK_SIDNID_LIST
/////////////////////////////////////////////////////////////////////////

// Number of AEEConfigSIDNIDPairTypes array elements
#define OEMNV_LOCK_SIDNID_ARRSIZE 10

#ifdef FEATURE_ACP
#error code not present
#endif /* FEATURE_ACP */

/////////////////////////////////////////////////////////////////////////
// Dial string for originating packet calls -- CFGI_DATA_DIALSTRING
/////////////////////////////////////////////////////////////////////////

// Maximum length in AECHARs of the data dial string (including the NULL word).
#define OEMNV_DATA_DIALSTRING_MAXLEN  17


/////////////////////////////////////////////////////////////////////////
// SysMenu - The following structure is required to fetch the error logs
/////////////////////////////////////////////////////////////////////////

typedef struct _OEMErrLogType {
   byte err_num;         // n-th error
   byte err_count;       // # of times the error has occurred
   byte file_name[8];    // file name in which it occurred
   word line_num;        // line number where error hit
   boolean fatal;        // whether fatal
} OEMErrLogType;


#define OEMNV_STEREO_HEADSET_MASK              0x00000001
#define OEMNV_HEADSET_MASK                     0x00000002
#define OEMNV_ANALOG_HFK_MASK                  0x00000004
#define OEMNV_USB_HFK_ATTACHED_MASK            0x00000008
#define OEMNV_USB_HFK_ROUTE_AUDIO_MASK         0x00000010
#define OEMNV_STEREO_USB_HFK_ATTACHED_MASK     0x00000020
#define OEMNV_SDAC_MASK                        0x00000040
#define OEMNV_EXT_PWR_MASK                     0x00000080
#define OEMNV_SPEAKERPHONE_MASK                0x00000100

#define OEMNV_SHAKE_MUSIC_MASK            0x0001                                                         
#define OEMNV_SHAKE_FM_RADIO_MASK         0x0002           
#define OEMNV_SHAKE_WALLPAPER_MASK        0x0004       
#define OEMNV_SHAKE_SNOOZE_ALARM_MASK     0x0008
#define OEMNV_SHAKE_VIDEO_MASK            0x0010
#define OEMNV_PEDOMETER_MASK              0x0020
typedef uint32 OEMTTY;

#define OEMNV_TTY_FULL  0
#define OEMNV_TTY_TALK  1
#define OEMNV_TTY_HEAR  2
#define OEMNV_TTY_OFF   3

typedef enum{
  OEMNV_DTMFOFF=1,
  OEMNV_DTMFCONT,
  OEMNV_DTMFBURST
}OEMDTMF;

typedef enum
{
  OEM_CCBS_ACCEPT,
  OEM_CCBS_REJECT,
  OEM_CCBS_HOLD_N_ACCEPT,
  OEM_CCBS_RELEASE_N_ACCEPT,
  OEM_CCBS_NONE
}OEMCCBS;

typedef enum {
    //DESKTOP_THEME_NONE = 0x00,
    DESKTOP_THEME_ORANGE    = 0x00,
    DESKTOP_THEME_RED,
    DESKTOP_THEME_YELLOW,
    DESKTOP_THEME_GREEN,
    DESKTOP_THEME_BLUE,
    DESKTOP_THEME_DEEP_BLUE
} DESKTOP_THEME;

#define MAX_FMRADIO_VOLUME  15
#define MAX_FMRADIO_NAME_CHAR 16

#if defined( FEATURE_CARRIER_THAILAND_HUTCH)
#define MAX_FMRADIO_STORED_CHANNEL 40
#else
#define MAX_FMRADIO_STORED_CHANNEL 20
#endif

typedef PACKED struct _sChanInfo
{
    AECHAR          szName[MAX_FMRADIO_NAME_CHAR + 1];              //广播台名
    uint16          wChannel;                                       //信道号
} sChanInfo;
typedef enum {
    OEMNV_RING_FIRST,
    OEMNV_MP3_RINGER,
    OEMNV_MID_RINGER,
    OEMNV_RING_LAST
} OEMRINGTYPE;

typedef PACKED struct _ringID
{
    char        szMusicname[MAX_FILE_NAME];
    uint16      midID;
    OEMRINGTYPE ringType;
    
}ringID;


#define OEMNV_MAX_SERVICEPROVIDER_NUMBER 50
#define OPERATOR_NAME_MAX_SIZE 32
#define OEMNV_MAX_MCC_SIZE 4
#define OEMNV_MAX_MNC_SIZE 3
typedef PACKED struct _ServiceProvider
{
    char   mcc[OEMNV_MAX_MCC_SIZE];
    char   mnc[OEMNV_MAX_MNC_SIZE];
    char  ids_name[OPERATOR_NAME_MAX_SIZE];
} ServiceProvider;
#ifdef FATRUE_LOCK_IMSI_MCCMNC
typedef PACKED struct _SetImsi
{
//add code for set lockimsi
    uint16 mcc;
    uint16 mnc;
    boolean bsetimsi;//set lockimsi state
} SetImsi;
#endif
/////////////////////////////////////////////////////////////////////////
// 输入法设置 -- CFGI_INPUTMODE
/////////////////////////////////////////////////////////////////////////

#ifdef WIN32//wlh 临时修改
#define OEM_MODE_T9_ZHUYIN  1
#endif//WIN32

#if defined FEATURE_LANG_THAI 
#define OEMNV_INPUTMODE_DEFAULT              OEM_MODE_T9_MT_THAI
#elif defined FEATURE_LANG_TCHINESE
#define OEMNV_INPUTMODE_DEFAULT              OEM_MODE_T9_ZHUYIN
#elif defined FEATURE_LANG_SPANISH
#define OEMNV_INPUTMODE_DEFAULT              OEM_MODE_T9_RAPID_SPANISH //OEM_MODE_T9_MT_SPANISH
#elif defined FEATURE_LANG_INDONESIAN
#define OEMNV_INPUTMODE_DEFAULT              OEM_MODE_T9_MT_INDONESIAN
//#elif defined FEATURE_LANG_PORTUGUESE
//#define OEMNV_INPUTMODE_DEFAULT              OEM_MODE_T9_MT_PORTUGUESE
#elif defined FEATURE_LANG_HEBREW
#define OEMNV_INPUTMODE_DEFAULT              OEM_MODE_T9_MT_HEBREW
//#elif defined(FEATURE_LANG_ARABIC) && !defined(FEATURE_LANG_FRENCH)
#elif defined FEATURE_CARRIER_SUDAN_SUDATEL
#define OEMNV_INPUTMODE_DEFAULT              OEM_MODE_T9_MT_ARABIC
//#elif defined(FEATURE_LANG_ARABIC) && defined(FEATURE_LANG_FRENCH)
#elif defined FEATURE_CARRIER_MAROC_WANA
#define OEMNV_INPUTMODE_DEFAULT              OEM_MODE_T9_RAPID_FRENCH
#else
#define OEMNV_INPUTMODE_DEFAULT              OEM_MODE_T9_MT_ENGLISH
#endif  

#ifdef FEATURE_SHORT_CODE_NAM_COUNT
#define   OEMNV_SHORT_CODE_NAM_COUNT         10   // CFGI_SHORT_CODE_NAM_COUNT
#endif //FEATURE_SHORT_CODE_NAM_COUNT
#define OEM_INIFILE    "oem.ini"
#define OEM_BAKFILE   "oem.bak"
#if defined (FEATURE_CARRIER_THAILAND_HUTCH)
#define OEM_INTERNATION_NUMBER "001"
#else
#define OEM_INTERNATION_NUMBER "00"
#endif
#define   OEMNV_MENU_STYLE_ICON    0
#define   OEMNV_MENU_STYLE_LIST     1

#ifdef FEATURE_PLANEMODE
#define   OEMNV_PLANEMODE_ON        (1)
#define   OEMNV_PLANEMODE_OFF       (2)
#define   OEMNV_PLANEMODE_QUERY     (3)
#endif

typedef enum {
    OEMNV_CAMERA_ENVIR_AUTO,
    OEMNV_CAMERA_ENVIR_NIGHT,
    OEMNV_CAMERA_ENVIR_SUNNY,
    OEMNV_CAMERA_ENVIR_CLOUDY
} OEMCAMERAENVIR;

typedef enum {
    OEMNV_CAMERA_QUALITY_HIGH,
    OEMNV_CAMERA_QUALITY_MED,
    OEMNV_CAMERA_QUALITY_LOW
} OEMCAMERAQUALITY;

typedef enum {
    OEMNV_CAMERA_SIZE_1600_1200,
    OEMNV_CAMERA_SIZE_1280_960,
    OEMNV_CAMERA_SIZE_640_480,
    OEMNV_CAMERA_SIZE_320_240,   
    OEMNV_CAMERA_SIZE_176_220
} OEMCAMERASIZE;

#ifdef FEATURE_CARRIER_CHINA_TELCOM
typedef enum {
    OEMNV_CAMERA_SHUTTER_TONE_SHUTTER1,
    OEMNV_CAMERA_SHUTTER_TONE_SHUTTER2
} OEMCAMERATONE;
#else
typedef enum {
    OEMNV_CAMERA_SHUTTER_TONE_DISABLE,
    OEMNV_CAMERA_SHUTTER_TONE_ENABLE
} OEMCAMERATONE;
#endif

typedef enum {
    OEMNV_CAMERA_BANDING_50HZ,
    OEMNV_CAMERA_BANDING_60HZ
} OEMCAMERABANDING;

typedef enum {
    OEMNV_CAMERA_STORAGE_PHONE,
    OEMNV_CAMERA_STORAGE_MEMORY_CARD
} OEMCAMERASTORAGE;

typedef enum {
    OEMNV_CAMERA_BRIGHTNESS_LEVEL1, //-2
    OEMNV_CAMERA_BRIGHTNESS_LEVEL2, //-1
    OEMNV_CAMERA_BRIGHTNESS_LEVEL3, //0
    OEMNV_CAMERA_BRIGHTNESS_LEVEL4, //1
    OEMNV_CAMERA_BRIGHTNESS_LEVEL5  //2
} OEMCAMERABRIGHTNESS;

typedef enum {
    OEMNV_CAMERA_ZOOM_LEVEL1,
    OEMNV_CAMERA_ZOOM_LEVEL2,
    OEMNV_CAMERA_ZOOM_LEVEL3,
    OEMNV_CAMERA_ZOOM_LEVEL4,
    OEMNV_CAMERA_ZOOM_LEVEL5
} OEMCAMERAZOOM;

typedef enum {
    OEMNV_CAMERA_FRAME_0,
    OEMNV_CAMERA_FRAME_1,
    OEMNV_CAMERA_FRAME_2,
    OEMNV_CAMERA_FRAME_3,
    OEMNV_CAMERA_FRAME_4,
    OEMNV_CAMERA_FRAME_5,
    OEMNV_CAMERA_FRAME_6,
    OEMNV_CAMERA_FRAME_7,
    OEMNV_CAMERA_FRAME_8
} OEMCAMERAFRAME;

typedef enum {
    OEMNV_CAMERA_COLOR_NORMAL = 8,
    OEMNV_CAMERA_COLOR_BANDW = 4,
    OEMNV_CAMERA_COLOR_SEPIA = 0,
    OEMNV_CAMERA_COLOR_NEGATIVE = 2,
    OEMNV_CAMERA_COLOR_REDONLY = 3,
    OEMNV_CAMERA_COLOR_GREENONLY = 7
} OEMCAMERACOLOR;
#endif  /* OEMNVINT_H */
