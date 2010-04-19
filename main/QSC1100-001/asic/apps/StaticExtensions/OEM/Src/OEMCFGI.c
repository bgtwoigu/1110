/*===========================================================================
                              OEMCFGI.c

  OEM CONFIG CONTROL ROUTINES

  This file contains a reference implementation of a OEM Config
  control interfaces.

        Copyright � 1999-2007 QUALCOMM Incorporated.
               All Rights Reserved.
            QUALCOMM Proprietary/GTDR
============================================================================*/

/*=============================================================================

$Header: //depot/asic/msmshared/apps/StaticExtensions/OEM/Src/OEMCFGI.c#142 $
$DateTime: 2008/10/31 07:05:40 $

when       who     what, where, why
--------   ---     ----------------------------------------------------------
10/31/08   phani   Propagating the changes done for BMP build on 6290.
09/26/08   kps     Add support to replace tcxo_srm functions with tcxomgr.
07/03/08   yb      Added CFGI_TOOLKIT_CS_PS_PARALLEL
03/20/08   clm     Fix memory leak.
03/13/08   yc      Add CFGI_LP_SEC, CFGI_LTM_OFF, CFGI_DAYLT items
02/27/08   yc      Correct the setting for AUTO_ANSWER and TTY config items
02/13/08   sam     Remove include for CustomOEMConfigItems.h
02/06/07   sam     Remove CFGI_TTY and add it to CutomOEMConfigItems.
12/03/07   clm     Replace OEMFS funcs with efs funcs for brew 4.0
10/05/07   mjv     Tweaking implementation of CFGI_UNITTEST_ACTIVE
10/01/07   cvs     Remove calls to STRCPY()
09/28/07   clm     Add CFGI_NITZ_NW_INFO_MCC and CFGI_NITZ_NW_INFO_MNC items.
03/26/07   jas     Tweaking implementation of CFGI_UNITTEST_ACTIVE
03/06/07   jas     Removing rfm_ioctl proto, which is in the include file.
02/20/07   jas     Changing RF includes so that rfm_ioctl is prototyped
02/05/07   ypmw    Fix the compiler errors for the changes of sound dev 
                   attach variables
02/01/07   ypmw    Move the sound device attach variable from IConfig to 
                   uisnd_device_info_table
01/22/07   ayk     Support for CFGI_AUTO_ANSWER.
01/04/06   jfc/jas Applying changes for new RFM IOCTL drivers
12/08/06   jas     Adding support for CFGI_FIRMWARE_VERSION (for HDK)
12/07/06   jas     One more fix of prior checkin
12/07/06   jas     Correcting prior checkin; the FTM item was reimplemented
12/07/06   mv/jas  Removing CFGI_FACTORY_TEST_MODE, which is unused.
11/09/06   jas     Reverting prior changes; they are not yet adequate.
11/09/06   jfc/jas Changes for new, common RF driver
10/30/06   jas     Fixing a compile problem on HDK builds
10/20/06   jas     Fixing compile problem for UIM builds
10/19/06   jas     Mainlining uiOne HDK changes
09/27/06   jas     Fix RVCT compiler warnings.
09/14/06   jas     New item for availability of Manual PLMN selection
08/08/06   jas     Fixing a compiler warning
08/02/06   aps     Fix for CDMA parms being looked up in GSM mode
07/12/06   jas     Fixing RVCT compiler warnings.
06/28/06   jas     Correcting featurization to work on 6275 with BREW 3.1.4
06/21/06   cvs     Fix TTY init for 1x targets
06/12/06   jks     Ported IUsrSysClock to new BREW ISYSCLOCK interface
06/07/06   JD      removed CFGI_SMS_CHANNEL_SELECTION and MAX_MSG_PAYLOAD
06/13/06   JD      Added support for CFGI_VOICEPRIVACY
03/31/06   ac      Added support for QIPCALL debug info
02/21/06   cvs     Add support for persistent game mode
01/16/06   jas     Modifications to support RF driver changes
11/21/05   jas     New item for disabling background images
11/15/05   jas     New item for disabling display during calls
11/08/05   rsr/jas Changes to use rfgsm_get_debug_screen_data API to get gsm 
                   debug screen info.
10/21/05   RI      Added plmn_name config item.
10/10/05   PMD     Added Retry Interval; Changed Retry Period Implementation.
                   Updated OEMCFGLIST Version
09/22/05   cvs     Support for Stereo USB Carkit
09/27/05   cvs     Revert previous changes.  They need better featurization.
09/12/05   rsr     Changes to use rfgsm_get_debug_screen_data API to get gsm 
                   debug screen info.
09/08/05   PMD     Used OEMNV_SMS_RETRY_PERIOD for SMS Retry Period.
08/29/05   RI      Enable auto update of user time using MM info
07/27/05   cvs     Code Review cleanups
07/15/05   AT      Added CFGI items display_duration and back_light_intensity.
06/20/05   bmg     Added CFGI item for IntelliCeiver state
06/10/05   cvs     Support for Smart USB Carkit
04/22/05   RI      Fixed OEMPriv_SetItem_CFGI_TTY() to set the tty value correctly. 
04/19/05   sun     Added new Item for Manual PLMN Selection
04/06/05   RI      Support for TTY.
03/23/05   cvs     RVCT compiler fixes
03/02/05   JLG     Add CFGI_SUBSCRIBERID support.
01/12/05   AT      Set the NAM to NAM1 for the ROAM timer.
01/10/05   src     Added featurisation around references to RF GSM variables.
                   (Requires updated RFGSM header and source.)
12/14/04   AT      Fixed issues with call timers.
12/03/04   PMD     Updated the OEMCONFIGLIST_VERSION to 8
11/17/04   SUN     Added new CFGI Item for Beep Length
11/04/04   PMD     Added SMS GCF Flag CFGI Item.
10/30/04   bhas    Debug Screen support for UMTS targets
10/11/04   PMD     Changed path for oemconfig.dat
10/06/04   PMD     Changed Default SMS Segmentation Setting to 8-bit
10/04/04   sun     Removed CFGI Item for CPHS
09/27/04   sun     Added new CFGI Item to support CPHS
09/22/04   cvs     Add Contrast menu, lint fixes
09/17/04   dyc     Obtain RF hw and NV type for MSM6500
08/24/04   Rex     Changed sms retry default value to 30 seconds
08/16/04   NNG     Support for voice debug.
06/23/04   sun     Fixed Compile Error
06/07/04   sun     Added support for LCD Display
05/17/04   ak      Added HDR Debug Screen Related cache
05/13/04   SUN     Fixed Compile Warning
04/19/04   PA      Added CFGI_LAST_RX_DATA_COUNT, CFGI_LAST_TX_DATA_COUNT,
                   CFGI_TOTAL_RX_DATA_COUNT and CFGI_TOTAL_TX_DATA_COUNT
04/16/04   SUN     Fixed Compile Error
04/14/04   SUN     Added new CFGI item CFGI_VR & CFGI_TTY
03/31/04   dyc     Added 2nd chain RxAGC support for MSM6500.
                   Use int32 for CFGI_DEBUG_RX_AGC_IN_DBM item.
03/30/04   SUN     Check for return values
03/08/04   PMD     Added Support for WMS Application Mode and seperate
                   Voicemails for CDMA and GSM / WCDMA (GW)
03/04/04   dyc     Update for field debug support
02/20/04   sun     Fixed Compile Warning
02/14/04   PMD     Added SMS Retry Period Config Item
02/12/04   RAM     Added new CFGI items for CDMA/HDR diversity control.
02/12/04   SUN     Fixed Compile Errors
02/10/04   RAM     Read the AMR item on power-up.
02/10/04   SUN     Added support for CFGI_AMR
01/28/04   RAM     Added new CFGI item for SpeakerPhone.
01/26/04   PMD     Added SMS Segmentation Message Reference
01/26/04   PMD     Added SMS Segmentation Settings
12/09/03    lz     Added new CFGI item for BREW Download information
11/26/03   SUN     Added new CFGI item for CUG
11/23/03   SUN     Added new CFGI items for devices
11/06/03   AK/PMD  Changes for Brew 3.0
11/05/03   RA      Added PA,MSM,PMIC,CHARGER,BATTERY thermistor readings
11/04/03   PMD     Added CDMA and GW Default Template Settings for SMS
10/31/03   PMD     Added IConfig Items for SMS Cell Broadcast Settings
                   Added IConfig Items for SMS Alert Settings
10/17/03   kar     Fixed link errors
10/15/03   SUN     Fixed Compile error
08/20/03   ADM     Added code to retrieve call times information
08/19/03   SUN     Added Support for IMEI
08/05/03   PMD     Added Mode preference (CDMA / GW) NV Item for SMS to be
                   accessed using IConfig
06/02/03   PMD     Usage of NV items for retrieving / storing Voice Mail Number

=============================================================================*/
#include "target.h"
#include "customer.h"

#include "OEMFeatures.h"
#include "BREWVersion.h"
#include "AEEComdef.h"
#include "OEMConfig.h"
#include "AEEShell.h"
#include "AEE_OEM.h"
#include "AEEStdLib.h"
#if defined(FEATURE_BMP)
#include "AEEDownloadInfo.h"
#include "AEEFS.h"
#else
#include "AEEDownload.h"
#endif
#ifdef FEATURE_BREW_3_0
#include "AEE_OEMDispatch.h"
#endif

#include "OEMCFGI.h"
#include "OEMNV.h"
#include "fs_errno.h"
#include "fs_public.h"
#if MIN_BREW_VERSION(4,0)
#include "OEMefs.h"
#else
#include "OEMFS.h"
#endif //MIN_BREW_VERSION(4,0)
#include "snd.h"
#include "ui.h"
#include "hs.h"
#include "mobile.h"
#include "adc.h"

#ifdef FEATURE_ICM
#include "AEECM.h"
#endif //FEATURE_ICM


#include "rfm.h" /* RF Driver interface */ 
#ifndef FEATURE_RFM_IOCTL
#if defined (T_MSM6500) || defined (PLATFORM_LTK)
#include "rfi.h"
#include "rf.h"
#include "rfcom.h"
#endif
#ifdef FEATURE_INTELLICEIVER
#include "rf_intelliceiver.h"
#endif
#endif /* FEATURE_RFM_IOCTL */

#include "therm.h"

#if defined (FEATURE_CDMA_800) || defined (FEATURE_CDMA_1900)
#include "db.h"
#endif

#include "cm.h"
#include "disp.h"

#ifdef FEATURE_HDR
#error code not present
#endif

#ifdef FEATURE_QIPCALL
#include "qipcalldbg.h"
#endif

#if defined(FEATURE_GSM) || defined(FEATURE_WCDMA)
#error code not present
#endif /* defined(FEATURE_GSM) || defined(FEATURE_WCDMA) */


#if defined(FEATURE_WCDMA)
#error code not present
#endif // FEATURE_WCDMA


#if defined(FEATURE_GSM) || defined(FEATURE_WCDMA)
#error code not present
#endif

/*===========================================================================

                     DEFINES AND CONSTANTS

===========================================================================*/
////
// Version number of the OEMConfigListType structure.  This value
// must be incremented whenever an existing field in changed or deleted
// in OEMConfigListType.  It does not need to be incremented when a new
// field is added to the end of OEMConfigListType.
//
#define OEMCONFIGLIST_VERSION ( (uint16) 0x0009 )

////
// The EFS file that stores the OEM configuration.
#if MIN_BREW_VERSION(3,0)
#define OEMCONFIGLIST_FILE (AEEFS_SYS_DIR"priv/oemconfig.dat")
#else
#define OEMCONFIGLIST_FILE ("brew/oemconfig.dat")
#endif


#define CDMAMIN 1

////
// Used to define an entry in a OEM Config Item table
#define CFGTABLEITEM(cfgi,size)  \
         {cfgi, OEMPriv_GetItem_##cfgi, OEMPriv_SetItem_##cfgi, (size)}

////
// Used to define a read-only entry in a OEM Config Item table
#define CFGTABLEITEM_READONLY(cfgi,size)  \
         {cfgi, OEMPriv_GetItem_##cfgi, OEMPriv_SetItem_ReadOnly, (size)}

////
// Used to define an empty entry in a OEM Config Item table
#define CFGTABLEITEM_EMPTY(cfgi) {cfgi, NULL, NULL, -1}


/* Closed user group index */
typedef PACKED struct
{
   boolean                         present;
   byte                            msb;
   byte                            lsb;
} OEMConfigIndex;

/* Forward CUG info */
typedef PACKED struct
{
   boolean                         present;
   OEMConfigIndex                  cug_index;
   boolean                         suppress_pref_cug;
   boolean                         suppress_outgoing_access;
} OEMConfigCugInfo;


/* BREW Download Information */

#define OEMCFG_DEFAULT_CID           (27)
#define OEMCFG_DEFAULT_PLATFORM_ID   (600)
#define OEMCFG_DEFAULT_DL_SERVER     "oemdemo.qualcomm.com"
#define OEMCFG_DL_AKEY_SIZE           8
#define OEMCFG_DL_BKEY_SIZE           16
#define OEMCFG_DL_SERVER_LEN          64

typedef PACKED struct {
   uint32   dwCID;         // Carrier ID
   uint32   dwPID;         // Platform ID
   uint8    bBKey[OEMCFG_DL_BKEY_SIZE];     // BKey
   uint8    bAKey[OEMCFG_DL_AKEY_SIZE];      // AKey
   char     szServer[OEMCFG_DL_SERVER_LEN];  // DL Server
   uint16   wFlags;        // DL Flags
   uint16   wAuth;         // Auth policy
   uint16   wPrivP;        // Privacy policy
} OEMConfigDownloadInfo;


////
// Configuration items managed by the OEM layer.
//
// NOTE: Whenever a change is made to an existing memory ofthis structure,
//       OEMCONFIGLIST_VERSION must be incremented.   It does not need to be
//       incremented after adding a new item to the end of the structure.
//
typedef PACKED struct
{
   byte          alert_type;                   // CFGI_ALERT_TYPE
   byte          silence_all;                  // CFGI_SILENCEALL
   byte          headset_ringer;               // CFGI_HEADSET_RNG
   byte          missed_call_alert;            // CFGI_MISSED_CALL_ALERT
   byte          roaming_alert;                // CFGI_ROAMING_ALERT
   byte          internet_call_alert;          // CFGI_INTERNET_ALERT
   dword         internet_call_time;           // CFGI_BROWSER_CALL_TIMER
   byte          sms_service_option;           // CFGI_SMS_SERVICE_OPTION
   byte          sms_is41_workaround;          // CFGI_SMS_IS41_WORKAROUND
   byte          sms_mo_encoding;              // CFGI_SMS_MO_ENCODING
   byte          sms_mode;                     // CFGI_SMS_MODE
   byte          sms_app;                      // CFGI_SMS_APP
   byte          sms_cb_auto_delete;           // CFGI_SMS_CB_AUTO_DELETE
   byte          sms_cb_silent_alert;          // CFGI_SMS_CB_SILENT_ALERT
   byte          sms_cb_lang_filter;           // CFGI_SMS_CB_LANG_FILTER
   byte          sms_alert_display;            // CFGI_SMS_ALERT_DISPLAY
   byte          sms_alert_sound;              // CFGI_SMS_ALERT_SOUND
   byte          sms_alert_volume;             // CFGI_SMS_ALERT_VOLUME
   byte          sms_cdma_default_template;    // CFGI_SMS_CDMA_DEFAULT_TEMPLATE
   byte          sms_gw_default_template;      // CFGI_SMS_GW_DEFAULT_TEMPLATE
   byte          sms_segment_setting;          // CFGI_SMS_SEGMENT_SETTING
   word          sms_segment_ref_num;          // CFGI_SMS_SEGMENT_REF_NUM
   word          sms_retry_period;             // CFGI_SMS_RETRY_PERIOD
   word          sms_retry_interval;           // CFGI_SMS_RETRY_INTERVAL
   boolean       sms_gcf_flag;                 // CFGI_SMS_GCF_FLAG

   // CFGI_VOICEMAIL_NUMBER
   AECHAR        voicemail_number[OEMNV_VOICEMAIL_MAXLEN];

   // CFGI_VOICEMAIL_NUMBER_CDMA
   AECHAR        voicemail_number_cdma[OEMNV_VOICEMAIL_MAXLEN];

   // CFGI_VOICEMAIL_NUMBER_GW
   AECHAR        voicemail_number_gw[OEMNV_VOICEMAIL_MAXLEN];

   dword         recent_mt_call_time;          // CFGI_RECENT_MT_CALL_TIMER
   dword         recent_mo_call_time;          // CFGI_RECENT_MO_TIMER

   byte          minute_alert;                 // CFGI_MINUTE_ALERT
   boolean       any_key_answer;               // CFGI_ANYKEY_ANSWER
   byte          headset_autoanswer;           // CFGI_HEADSET_AUTOANSWER
   byte          time_format;                  // CFGI_TIME_FORMAT
   uint32        device_mask;                  // CFGI_HEADSET|CFGI_STEREO_HEADSET|
                                               // CFGI_USB_HFK_ATTACHED|CFGI_EXT_PWR | 
                                               // CFGI_STEREO_USB_HFK_ATTACHED

   OEMConfigCugInfo  cug;                         // CFGI_CUG
    boolean           vr;                         // CFGI_VR
   OEMConfigDownloadInfo  download_info;          // BREW Download information
   char szSubscriberID[DEFAULT_SUBSCRIBERID_LEN]; // CFGI_SUBSCRIBERID
   boolean       disable_in_call_disp;            // CFGI_DISABLE_IN_CALL_DISP
   boolean       disable_bg_image;              // CFGI_DISABLE_BG_IMAGE
   boolean       manual_plmn_sel_allowed;      // CFGI_MANUAL_PLMN_SEL_ALLOWED
 
} OEMConfigListType;


  /* International Mobile Equipment Identity */
#if defined (FEATURE_WCDMA) || defined (FEATURE_GSM)
#error code not present
#else
#define OEMCFG_IMEI_SIZE 10
#endif


////
// Configuration items managed by the NV subsystem.
typedef struct
{
  ///////////////////////////////////////////////////////////////////////
  // Sound
  //////////////////////////////////////////////////////////////////////
#ifdef FEATURE_SMART_SOUND
  boolean  smart_sound;                /*  CFGI_SMART_SOUND */
#endif
  byte  ringer_level;                      /* CFGI_RINGER_VOL */
  byte  handset_ear_level;               /* CFGI_EAR_VOL */
  byte  beep_level;                  /* CFGI_BEEP_VOL */
  byte  ringer_type;                  /* CFGI_RINGER_TYPE */
  byte  key_sound;            /* CFGI_KEYBEEP_SOUND */


  ////////////////////////////////////////////////////////////////
  // Display
  ////////////////////////////////////////////////////////////////
  byte contrast;          /* CFGI_CONTRAST_LVL */
  byte backlight;         /* CFGI_BACK_LIGHT */
  byte language;        /* CFGI_LANGUAGE_SELECTION */
  AECHAR banner[NV_MAX_LTRS/2+1]; /* CFGI_BANNER */
  byte menu_format;       /* CFGI_MENU_TYPE */

  ////////////////////////////////////////////////////////////////
  // Security
  ////////////////////////////////////////////////////////////////
  byte          lockcode[NV_LOCK_CODE_SIZE];  // CFGI_LOCKCODE

   ////////////////////////////////////////////////////////////////
   // Toolkit
   ////////////////////////////////////////////////////////////////
#ifdef FEATURE_GSTK_CS_PS_PARALLEL
  boolean toolkit_cs_ps_parallel;      /* CFGI_TOOLKIT_CS_PS_PARALLEL */
#endif /* FEATURE_GSTK_CS_PS_PARALLEL */

  ////////////////////////////////////////////////////////////////
  // ETC
  ////////////////////////////////////////////////////////////////
  dword   all_call_time;        /* CFGI_ALL_CALL_TIMER */
  byte    backlight_hfk;        /* CFGI_EXTPWR_BK_LIGHT */
  byte    svc_alert;            /* CFGI_SVC_ALERT */
  boolean amr;                  /*CFGI_AMR*/
  boolean cdma_rx_diversity_ctrl; /*CFGI_CDMA_RX_DIVERSITY_CTL*/
  boolean hdr_rx_diversity_ctrl;  /*HDR_CDMA_RX_DIVERSITY_CTL*/
  OEMTTY tty;
  boolean game_mode;            /*CFGI_GAME_MODE*/
  boolean game_mode_persist;    /*CFGI_GAME_MODE_PERSIST*/

  ////////////////////////////////////////////////////////////////
  // Data Call Counters
  ////////////////////////////////////////////////////////////////
  uint32 last_rx_data_count;    /* CFGI_LAST_RX_DATA_COUNT */
  uint32 last_tx_data_count;    /* CFGI_LAST_TX_DATA_COUNT */
  uint32 total_rx_data_count;   /* CFGI_TOTAL_RX_DATA_COUNT */
  uint32 total_tx_data_count;   /* CFGI_TOTAL_TX_DATA_COUNT */
  uint32 beep_length;           /* CFGI_BEEP_LENGTH*/
  boolean manual_plmn_sel;      /* CFGI_MANUAL_PLMN_SEL */
  OEMCCBS ccbs_sel;
  uint32  display_duration;      /* CFGI_LCD_TIMER */
  uint8   backlight_intensity;   /* CFGI_BACK_LIGHT_INTENSITY */
#if defined(FEATURE_USRSYSCLOCK) || defined(FEATURE_ISYSCLOCK)
  boolean auto_time_enable;      /* CFGI_ENABLE_NETWORK_TIME */
  AECHAR  plmn_name[NV_MAX_NW_NAME_CHARS + 1]; /* CFGI_NITZ_NW_INFO */
  uint32  mcc; /* CFGI_NITZ_NW_INFO_MCC */
  uint32  mnc; /* CFGI_NITZ_NW_INFO_MNC */
#endif 
  uint8 voice_priv;            /* CFGI_VOICEPRIVACY */
} NVConfigListType;

////
// Configuration items managed by the NV subsystem.
#ifdef FEATURE_HDR
#error code not present
#endif


////
// GetConfig/SetConfig function pointer.  Unlike the
// OEM_GetConfig()/OEM_SetConfig() functions, this function does
// not have an item parameter nor a size parameter.
//
// The item parameter is not required because there is a 1-1 mapping
// between config item and config function.
//
// The size parameter is not required because the size check is performed
// before calling this function (and variable sized config items are
// not supported).
//
typedef int (*PFNCFGITEMOP)(void *pBuff);

////
// OEM Config Item Table entry
typedef struct _ConfigItemTableEntry {
   AEEConfigItem item;  // the item id
   PFNCFGITEMOP  get;   // GetConfig function
   PFNCFGITEMOP  set;   // SetConfig function
   int32         size;  // data size of this item (note that variable sized
                        //                         items are not supported)
} ConfigItemTableEntry;


////
// OEM Config Table information
typedef struct _ConfigTableInfo {
   ConfigItemTableEntry const *tbl;  // pointer to the table
   uint32                      size; // number of elements in the table
} ConfigTableInfo;


typedef struct {
   char *num_buf;
   byte  num_len;
} EmergencyNumber;

/*===========================================================================

                     STATIC FUNCTION PROTOTYPES

===========================================================================*/


// Number-classification callback functions
static void OEMPriv_WriteOEMConfigList(void);
static void OEMPriv_ReadOEMConfigList(void);


// OEM Config Item Get/Set functions
static int OEMPriv_GetItem_CFGI_SILENCEALL(void *pBuff);
static int OEMPriv_SetItem_CFGI_SILENCEALL(void *pBuff);
static int OEMPriv_GetItem_CFGI_RECENT_MT_CALL_TIMER(void *pBuff);
static int OEMPriv_SetItem_CFGI_RECENT_MT_CALL_TIMER(void *pBuff);
static int OEMPriv_GetItem_CFGI_RECENT_MO_CALL_TIMER(void *pBuff);
static int OEMPriv_SetItem_CFGI_RECENT_MO_CALL_TIMER(void *pBuff);
static int OEMPriv_GetItem_CFGI_ALL_CALL_TIMER(void *pBuff);
static int OEMPriv_SetItem_CFGI_ALL_CALL_TIMER(void *pBuff);
static int OEMPriv_GetItem_CFGI_BROWSER_CALL_TIMER(void *pBuff);
static int OEMPriv_SetItem_CFGI_BROWSER_CALL_TIMER(void *pBuff);
static int OEMPriv_GetItem_CFGI_EXTPWR_BK_LIGHT(void *pBuff);
static int OEMPriv_SetItem_CFGI_EXTPWR_BK_LIGHT(void *pBuff);
static int OEMPriv_GetItem_CFGI_HEADSET_RNG(void *pBuff);
static int OEMPriv_SetItem_CFGI_HEADSET_RNG(void *pBuff);
static int OEMPriv_GetItem_CFGI_ANYKEY_ANSWER(void *pBuff);
static int OEMPriv_SetItem_CFGI_ANYKEY_ANSWER(void *pBuff);
static int OEMPriv_GetItem_CFGI_HEADSET_AUTOANSWER(void *pBuff);
static int OEMPriv_SetItem_CFGI_HEADSET_AUTOANSWER(void *pBuff);
static int OEMPriv_GetItem_CFGI_MISSED_CALL_ALERT(void *pBuff);
static int OEMPriv_SetItem_CFGI_MISSED_CALL_ALERT(void *pBuff);
static int OEMPriv_GetItem_CFGI_ROAMING_ALERT(void *pBuff);
static int OEMPriv_SetItem_CFGI_ROAMING_ALERT(void *pBuff);
static int OEMPriv_GetItem_CFGI_SVC_ALERT(void *pBuff);
static int OEMPriv_SetItem_CFGI_SVC_ALERT(void *pBuff);
static int OEMPriv_GetItem_CFGI_INTERNET_ALERT(void *pBuff);
static int OEMPriv_SetItem_CFGI_INTERNET_ALERT(void *pBuff);
static int OEMPriv_GetItem_CFGI_MINUTE_ALERT(void *pBuff);
static int OEMPriv_SetItem_CFGI_MINUTE_ALERT(void *pBuff);
static int OEMPriv_GetItem_CFGI_GAME_MODE(void *pBuff);
static int OEMPriv_SetItem_CFGI_GAME_MODE(void *pBuff);
static int OEMPriv_GetItem_CFGI_GAME_MODE_PERSIST(void *pBuff);
static int OEMPriv_SetItem_CFGI_GAME_MODE_PERSIST(void *pBuff);
static int OEMPriv_GetItem_CFGI_RINGER_VOL(void *pBuff);
static int OEMPriv_SetItem_CFGI_RINGER_VOL(void *pBuff);
static int OEMPriv_GetItem_CFGI_EAR_VOL(void *pBuff);
static int OEMPriv_SetItem_CFGI_EAR_VOL(void *pBuff);
static int OEMPriv_GetItem_CFGI_BEEP_VOL(void *pBuff);
static int OEMPriv_SetItem_CFGI_BEEP_VOL(void *pBuff);
static int OEMPriv_GetItem_CFGI_ALERT_TYPE(void *pBuff);
static int OEMPriv_SetItem_CFGI_ALERT_TYPE(void *pBuff);
#ifdef FEATURE_SMART_SOUND
static int OEMPriv_GetItem_CFGI_SMART_SOUND(void *pBuff);
static int OEMPriv_SetItem_CFGI_SMART_SOUND(void *pBuff);
#endif
static int OEMPriv_GetItem_CFGI_KEYBEEP_SOUND(void *pBuff);
static int OEMPriv_SetItem_CFGI_KEYBEEP_SOUND(void *pBuff);
static int OEMPriv_GetItem_CFGI_RINGER_TYPE(void *pBuff);
static int OEMPriv_SetItem_CFGI_RINGER_TYPE(void *pBuff);
static int OEMPriv_GetItem_CFGI_CONTRAST_LVL(void *pBuff);
static int OEMPriv_SetItem_CFGI_CONTRAST_LVL(void *pBuff);
static int OEMPriv_GetItem_CFGI_BACK_LIGHT(void *pBuff);
static int OEMPriv_SetItem_CFGI_BACK_LIGHT(void *pBuff);
static int OEMPriv_GetItem_CFGI_BACK_LIGHT_INTENSITY(void *pBuff);
static int OEMPriv_SetItem_CFGI_BACK_LIGHT_INTENSITY(void *pBuff);
static int OEMPriv_GetItem_CFGI_LANGUAGE_SELECTION(void *pBuff);
static int OEMPriv_SetItem_CFGI_LANGUAGE_SELECTION(void *pBuff);
static int OEMPriv_GetItem_CFGI_MENU_FORMAT(void *pBuff);
static int OEMPriv_SetItem_CFGI_MENU_FORMAT(void *pBuff);
static int OEMPriv_GetItem_CFGI_TIME_FORMAT(void *pBuff);
static int OEMPriv_SetItem_CFGI_TIME_FORMAT(void *pBuff);
static int OEMPriv_GetItem_CFGI_MANUAL_PLMN_SEL(void *pBuff);
static int OEMPriv_SetItem_CFGI_MANUAL_PLMN_SEL(void *pBuff);
#ifdef FEATURE_GSTK_CS_PS_PARALLEL
static int OEMPriv_GetItem_CFGI_TOOLKIT_CS_PS_PARALLEL(void *pBuff);
static int OEMPriv_SetItem_CFGI_TOOLKIT_CS_PS_PARALLEL(void *pBuff);
#endif /* FEATURE_GSTK_CS_PS_PARALLEL */
static int OEMPriv_GetItem_CFGI_IMEI(void *pBuff);
static int OEMPriv_SetItem_CFGI_IMEI(void *pBuff);
static int OEMPriv_GetItem_CFGI_CUG(void *pBuff);
static int OEMPriv_SetItem_CFGI_CUG(void *pBuff);
static int OEMPriv_GetItem_CFGI_VR(void *pBuff);
static int OEMPriv_SetItem_CFGI_VR(void *pBuff);
static int OEMPriv_GetItem_CFGI_AMR(void *pBuff);
static int OEMPriv_SetItem_CFGI_AMR(void *pBuff);
static int OEMPriv_GetItem_CFGI_LCD(void *pBuff);
static int OEMPriv_SetItem_CFGI_LCD(void *pBuff);
static int OEMPriv_GetItem_CFGI_LCD_TIMER(void *pBuff);
static int OEMPriv_SetItem_CFGI_LCD_TIMER(void *pBuff);
static int OEMPriv_GetItem_CFGI_CCBS(void *pBuff);
static int OEMPriv_SetItem_CFGI_CCBS(void *pBuff);
static int OEMPriv_GetItem_CFGI_LAST_RX_DATA_COUNT(void *pBuff);
static int OEMPriv_SetItem_CFGI_LAST_RX_DATA_COUNT(void *pBuff);
static int OEMPriv_GetItem_CFGI_LAST_TX_DATA_COUNT(void *pBuff);
static int OEMPriv_SetItem_CFGI_LAST_TX_DATA_COUNT(void *pBuff);
static int OEMPriv_GetItem_CFGI_TOTAL_RX_DATA_COUNT(void *pBuff);
static int OEMPriv_SetItem_CFGI_TOTAL_RX_DATA_COUNT(void *pBuff);
static int OEMPriv_GetItem_CFGI_TOTAL_TX_DATA_COUNT(void *pBuff);
static int OEMPriv_SetItem_CFGI_TOTAL_TX_DATA_COUNT(void *pBuff);
static int OEMPriv_GetItem_CFGI_BEEP_LENGTH(void *pBuff);
static int OEMPriv_SetItem_CFGI_BEEP_LENGTH(void *pBuff);
static int OEMPriv_GetItem_CFGI_CDMA_RX_DIVERSITY_CTL(void *pBuff);
static int OEMPriv_SetItem_CFGI_CDMA_RX_DIVERSITY_CTL(void *pBuff);
static int OEMPriv_GetItem_CFGI_HDR_RX_DIVERSITY_CTL(void *pBuff);
static int OEMPriv_SetItem_CFGI_HDR_RX_DIVERSITY_CTL(void *pBuff);
static int OEMPriv_GetItem_CFGI_SMS_SERVICE_OPTION(void *pBuff);
static int OEMPriv_SetItem_CFGI_SMS_SERVICE_OPTION(void *pBuff);
static int OEMPriv_GetItem_CFGI_SMS_TIMESTAMP(void *pBuff);
static int OEMPriv_SetItem_CFGI_SMS_TIMESTAMP(void *pBuff);
static int OEMPriv_GetItem_CFGI_SMS_IS41_WORKAROUND(void *pBuff);
static int OEMPriv_SetItem_CFGI_SMS_IS41_WORKAROUND(void *pBuff);
static int OEMPriv_GetItem_CFGI_SMS_MO_ENCODING(void *pBuff);
static int OEMPriv_SetItem_CFGI_SMS_MO_ENCODING(void *pBuff);
static int OEMPriv_GetItem_CFGI_SMS_MODE(void *pBuff);
static int OEMPriv_SetItem_CFGI_SMS_MODE(void *pBuff);
static int OEMPriv_GetItem_CFGI_SMS_APP(void *pBuff);
static int OEMPriv_SetItem_CFGI_SMS_APP(void *pBuff);
static int OEMPriv_GetItem_CFGI_SMS_CB_AUTO_DELETE(void *pBuff);
static int OEMPriv_SetItem_CFGI_SMS_CB_AUTO_DELETE(void *pBuff);
static int OEMPriv_GetItem_CFGI_SMS_CB_SILENT_ALERT(void *pBuff);
static int OEMPriv_SetItem_CFGI_SMS_CB_SILENT_ALERT(void *pBuff);
static int OEMPriv_GetItem_CFGI_SMS_CB_LANG_FILTER(void *pBuff);
static int OEMPriv_SetItem_CFGI_SMS_CB_LANG_FILTER(void *pBuff);

static int OEMPriv_GetItem_CFGI_SMS_ALERT_DISPLAY(void *pBuff);
static int OEMPriv_SetItem_CFGI_SMS_ALERT_DISPLAY(void *pBuff);
static int OEMPriv_GetItem_CFGI_SMS_ALERT_SOUND(void *pBuff);
static int OEMPriv_SetItem_CFGI_SMS_ALERT_SOUND(void *pBuff);

static int OEMPriv_GetItem_CFGI_SMS_CDMA_DEFAULT_TEMPLATE(void *pBuff);
static int OEMPriv_SetItem_CFGI_SMS_CDMA_DEFAULT_TEMPLATE(void *pBuff);
static int OEMPriv_GetItem_CFGI_SMS_GW_DEFAULT_TEMPLATE(void *pBuff);
static int OEMPriv_SetItem_CFGI_SMS_GW_DEFAULT_TEMPLATE(void *pBuff);
static int OEMPriv_GetItem_CFGI_SMS_SEGMENT_SETTING(void *pBuff);
static int OEMPriv_SetItem_CFGI_SMS_SEGMENT_SETTING(void *pBuff);
static int OEMPriv_GetItem_CFGI_SMS_SEGMENT_REF_NUM(void *pBuff);
static int OEMPriv_SetItem_CFGI_SMS_SEGMENT_REF_NUM(void *pBuff);

static int OEMPriv_GetItem_CFGI_SECCODE(void *pBuff);
static int OEMPriv_SetItem_CFGI_SECCODE(void *pBuff);
#ifdef FEATURE_ENABLE_OTKSL
static int OEMPriv_GetItem_CFGI_OTKSLCODE(void *pBuff);
static int OEMPriv_SetItem_CFGI_OTKSLCODE(void *pBuff);
#endif /* FEATURE_ENABLE_OTKSL */
static int OEMPriv_GetItem_CFGI_ESN(void *pBuff);
static int OEMPriv_GetItem_CFGI_RFCAL_VERSION(void *pBuff);
static int OEMPriv_GetItem_CFGI_RFCAL_DATE(void *pBuff);
static int OEMPriv_GetItem_CFGI_SLOTINDEX(void *pBuff);
static int OEMPriv_SetItem_CFGI_SLOTINDEX(void *pBuff);
static int OEMPriv_GetItem_CFGI_HOME_SIDNID_LIST(void *pBuff);
static int OEMPriv_SetItem_CFGI_HOME_SIDNID_LIST(void *pBuff);
static int OEMPriv_GetItem_CFGI_LOCK_SIDNID_LIST(void *pBuff);
static int OEMPriv_SetItem_CFGI_LOCK_SIDNID_LIST(void *pBuff);
static int OEMPriv_GetItem_CFGI_IMSI_MCC(void *pBuff);
static int OEMPriv_SetItem_CFGI_IMSI_MCC(void *pBuff);
static int OEMPriv_GetItem_CFGI_IMSI_11_12(void *pBuff);
static int OEMPriv_SetItem_CFGI_IMSI_11_12(void *pBuff);
static int OEMPriv_GetItem_CFGI_IMSI_S(void *pBuff);
static int OEMPriv_SetItem_CFGI_IMSI_S(void *pBuff);
static int OEMPriv_GetItem_CFGI_PRL_ENABLED(void *pBuff);
static int OEMPriv_SetItem_CFGI_PRL_ENABLED(void *pBuff);
static int OEMPriv_GetItem_CFGI_AOC(void *pBuff);
static int OEMPriv_SetItem_CFGI_AOC(void *pBuff);
static int OEMPriv_GetItem_CFGI_HOME_SID_REG(void *pBuff);
static int OEMPriv_SetItem_CFGI_HOME_SID_REG(void *pBuff);
static int OEMPriv_GetItem_CFGI_FORN_SID_REG(void *pBuff);
static int OEMPriv_SetItem_CFGI_FORN_SID_REG(void *pBuff);
static int OEMPriv_GetItem_CFGI_FORN_NID_REG(void *pBuff);
static int OEMPriv_SetItem_CFGI_FORN_NID_REG(void *pBuff);
#ifdef FEATURE_ACP
#error code not present
#endif /* FEATURE_ACP */
static int OEMPriv_GetItem_CFGI_DATA_QNC_ENABLED(void *pBuff);
static int OEMPriv_SetItem_CFGI_DATA_QNC_ENABLED(void *pBuff);
#ifdef FEATURE_FDN
static int OEMPriv_GetItem_CFGI_FDN_ENABLED(void *pBuff);
static int OEMPriv_SetItem_CFGI_FDN_ENABLED(void *pBuff);
#endif /* FEATURE_FDN */
static int OEMPriv_GetItem_CFGI_HEADSET(void *pBuff);
static int OEMPriv_SetItem_CFGI_HEADSET(void *pBuff);
static int OEMPriv_GetItem_CFGI_STEREO_HEADSET(void *pBuff);
static int OEMPriv_SetItem_CFGI_STEREO_HEADSET(void *pBuff);
static int OEMPriv_GetItem_CFGI_USB_HFK_ATTACHED(void *pBuff);
static int OEMPriv_SetItem_CFGI_USB_HFK_ATTACHED(void *pBuff);
static int OEMPriv_GetItem_CFGI_STEREO_USB_HFK_ATTACHED(void *pBuff);
static int OEMPriv_SetItem_CFGI_STEREO_USB_HFK_ATTACHED(void *pBuff);
static int OEMPriv_GetItem_CFGI_EXT_PWR(void *pBuff);
static int OEMPriv_SetItem_CFGI_EXT_PWR(void *pBuff);
static int OEMPriv_GetItem_CFGI_DEVICE(void *pBuff);
static int OEMPriv_GetItem_CFGI_DEBUG_CHANNEL(void *pBuff);
static int OEMPriv_GetItem_CFGI_DEBUG_PILOT_PN(void *pBuff);
static int OEMPriv_GetItem_CFGI_DEBUG_TX_AGC(void *pBuff);
static int OEMPriv_GetItem_CFGI_DEBUG_TX_AGC_IN_DBM(void *pBuff);
static int OEMPriv_GetItem_CFGI_DEBUG_RX_AGC(void *pBuff);
static int OEMPriv_GetItem_CFGI_DEBUG_RX_AGC_IN_DBM(void *pBuff);
static int OEMPriv_GetItem_CFGI_DEBUG_SID(void *pBuff);
static int OEMPriv_GetItem_CFGI_DEBUG_NID(void *pBuff);
static int OEMPriv_GetItem_CFGI_DEBUG_TX_ADJUST(void *pBuff);
static int OEMPriv_GetItem_CFGI_DEBUG_HDET(void *pBuff);
static int OEMPriv_GetItem_CFGI_DEBUG_VBATT(void *pBuff);
static int OEMPriv_GetItem_CFGI_DEBUG_ERRLOG(void *pBuff);
static int OEMPriv_GetItem_CFGI_DEBUG_LNA_STATE(void *pBuff);
static int OEMPriv_GetItem_CFGI_DEBUG_PA_STATE(void *pBuff);
static int OEMPriv_GetItem_CFGI_DEBUG_RATCHET_STATE(void *pBuff);

#if defined(FEATURE_WCDMA)
#error code not present
#endif

static int OEMPriv_GetItem_CFGI_CDMA_DYNAMIC_RANGE(void *pBuff);
static int OEMPriv_GetItem_CFGI_CDMA_MIN_RX_RSSI(void *pBuff);
#ifdef FEATURE_FACTORY_TESTMODE
static int OEMPriv_GetItem_CFGI_FACTORY_TEST_MODE(void *pBuff);
#endif /*FEATURE_FACTORY_TESTMODE*/
static int OEMPriv_GetItem_CFGI_DEBUG_THERM(void *pBuff);
static int OEMPriv_GetItem_CFGI_DEBUG_BAND(void *pBuff);

// Generic item is read-only function
static int OEMPriv_SetItem_ReadOnly(void *pBuff);

#ifdef T_MSM6500
static int OEMPriv_GetItem_CFGI_DEBUG_RF_CARD_NV_ID(void *pBuff);   
static int OEMPriv_GetItem_CFGI_DEBUG_THERM_BATT(void *pBuff);
static int OEMPriv_GetItem_CFGI_DEBUG_THERM_MSM(void *pBuff);
static int OEMPriv_GetItem_CFGI_DEBUG_THERM_PA(void *pBuff);
static int OEMPriv_GetItem_CFGI_DEBUG_THERM_PMIC(void *pBuff);
static int OEMPriv_GetItem_CFGI_DEBUG_THERM_CHRGR(void *pBuff);
#endif

#ifdef FEATURE_HDR
#error code not present
#endif

#ifdef FEATURE_QIPCALL
static int OEMPriv_GetItem_CFGI_QIPCALL_DEBUG_EVDO_IP_ADDR(void *pBuff);
static int OEMPriv_GetItem_CFGI_QIPCALL_DEBUG_EVDO_SIP_REG_STATUS(void *pBuff);
static int OEMPriv_GetItem_CFGI_QIPCALL_DEBUG_WLAN_IP_ADDR(void *pBuff);
static int OEMPriv_GetItem_CFGI_QIPCALL_DEBUG_WLAN_SIP_REG_STATUS(void *pBuff);
#endif

#if defined(FEATURE_GSM) || defined(FEATURE_WCDMA)
#error code not present
#endif /* defined(FEATURE_GSM) || defined(FEATURE_WCDMA) */

#ifdef FEATURE_INTELLICEIVER
static int OEMPriv_GetItem_CFGI_DEBUG_INTELLICEIVER_STATE(void *pBuff);
#endif /* FEATURE_INTELLICEIVER */

#if defined(FEATURE_USRSYSCLOCK) || defined(FEATURE_ISYSCLOCK)
static int OEMPriv_GetItem_CFGI_AUTO_TIME_ENABLE(void *pBuff);
static int OEMPriv_SetItem_CFGI_AUTO_TIME_ENABLE(void *pBuff);
#endif 

static int OEMPriv_GetItem_CFGI_DISABLE_IN_CALL_DISP(void *pBuff);
static int OEMPriv_SetItem_CFGI_DISABLE_IN_CALL_DISP(void *pBuff);

static int OEMPriv_GetItem_CFGI_DISABLE_BG_IMAGE(void *pBuff);
static int OEMPriv_SetItem_CFGI_DISABLE_BG_IMAGE(void *pBuff);
static int OEMPriv_GetItem_CFGI_VOICEPRIVACY(void *pBuff);
static int OEMPriv_SetItem_CFGI_VOICEPRIVACY(void *pBuff);
static int OEMPriv_GetItem_CFGI_MANUAL_PLMN_SEL_ALLOWED(void *pBuff);
static int OEMPriv_SetItem_CFGI_MANUAL_PLMN_SEL_ALLOWED(void *pBuff);

/*===========================================================================

                     STATIC/LOCAL DATA

===========================================================================*/

static AEECallback   gCBWriteOEMConfig;

////
// Cache of OEM configuration items (initialized with defaults)
//
// NOTE: Any changes made to these values should be mirrored in
//       OEM_RestoreFactorySettings()
static OEMConfigListType oemi_cache = {
   OEMNV_ALERTTYPE_RINGER,                          // CFGI_ALERT_TYPE
   OEMNV_SA_NORMAL,                                 // CFGI_SILENCEALL
   OEMNV_HEADSET_RNG_ON,                            // CFGI_HEADSET_RNG
   OEMNV_ALERT_ENABLE,                              // CFGI_MISSED_CALL_ALERT
   OEMNV_ALERT_ENABLE,                              // CFGI_ROAMING_ALERT
   OEMNV_ALERT_ENABLE,                              // CFGI_INTERNET_ALERT
   0,                                               // CFGI_BROWSER_CALL_TIMER
   OEMNV_SMS_SO_SO6,
   OEMNV_SMS_EMAIL_ADDRESSING_IS41,
   OEMNV_SMS_MO_ENCODING_7BIT,
   OEMNV_SMS_MODE_CDMA,
   OEMNV_SMS_APP_TEST,
   OEMNV_SMS_CB_DISABLE_AUTO_DELETE,
   OEMNV_SMS_CB_DISABLE_SILENT_ALERT,
   OEMNV_SMS_CB_DISABLE_LANG_FILTER,
   OEMNV_SMS_ALERT_DISPLAY_ENABLE,
   OEMNV_SMS_ALERT_SOUND_ENABLE,
   OEMNV_SMS_ALERT_VOLUME_2,
   0,                                              // CFGI_SMS_CDMA_DEFAULT_TEMPLATE
   0,                                              // CFGI_SMS_GW_DEFAULT_TEMPLATE
   8,                                              // CFGI_SMS_SEGMENT_SETTING
   0,                                              // CFGI_SMS_SEGMENT_REF_NUM
   OEMNV_SMS_RETRY_PERIOD,                         // CFGI_SMS_RETRY_PERIOD
   OEMNV_SMS_RETRY_INTERVAL,                       // CFGI_SMS_RETRY_INTERVAL
   FALSE,                                          // CFGI_SMS_GCF_FLAG
   {0,},                                           // CFGI_VOICEMAIL_NUMBER
   {0,},                                           // CFGI_VOICEMAIL_NUMBER_CDMA
   {0,},                                           // CFGI_VOICEMAIL_NUMBER_GW
   0,                                              // CFGI_RECENT_MT_CALL_TIMER
   0,                                              // CFGI_RECENT_MO_TIMER
   OEMNV_ALERT_DISABLE,                            // CFGI_MINUTE_ALERT
   FALSE,                                          // CFGI_ANYKEY_ANSWER
   OEMNV_HEADSET_AUTOANSWER_OFF,                   // CFGI_HEADSET_AUTOANSWER
   OEMNV_TIMEFORM_AMPM,
   0,                                              // CFGI_DEVICE
   {0,},                                           // CFGI_CUG
   FALSE,                                          // CFGI_VR
   {OEMCFG_DEFAULT_CID, OEMCFG_DEFAULT_PLATFORM_ID,   // Download Info
    {0,}, {0,},
    OEMCFG_DEFAULT_DL_SERVER, DIF_TEST_ALLOWED | DIF_MIN_FOR_SID,
    APOLICY_NONE, PPOLICY_BREW_OR_CARRIER
   },
   "0000000000000000000000000000000",              // CFGI_SUBSCRIBERID
   FALSE,                                          // CFGI_DISABLE_IN_CALL_DISP
   FALSE,                                          // CFGI_DISABLE_BG_IMAGE
   TRUE,                                           // CFGI_MANUAL_PLMN_SEL_ALLOWED
};

////
// Cache of NV configuration items (defaults are supplied by OEMNV.c)
static NVConfigListType nvi_cache;

#ifdef FEATURE_HDR
#error code not present
#endif


////
// Config table for the custom config items.
//
// IMPORTANT:
// The config items in this table MUST be in numeric sequence.  In
// other words, the expression
//
//       customItemTable[n].item == customItemTable[n-1].item + 1
//
// must hold for all n = 1...ARR_SIZE(customItemTable)-1
//
static ConfigItemTableEntry const customItemTable[] =
{
   //////////////////////////////////////////////////////////////////////
   // Setting Menu
   //////////////////////////////////////////////////////////////////////
   CFGTABLEITEM(CFGI_SILENCEALL, sizeof(byte)),
   CFGTABLEITEM_EMPTY(CFGI_AUTOKEYGUARD),
   CFGTABLEITEM(CFGI_RECENT_MT_CALL_TIMER, sizeof(dword)),
   CFGTABLEITEM(CFGI_RECENT_MO_CALL_TIMER, sizeof(dword)),
   CFGTABLEITEM(CFGI_ALL_CALL_TIMER, sizeof(dword)),
   CFGTABLEITEM(CFGI_BROWSER_CALL_TIMER, sizeof(dword)),
   CFGTABLEITEM(CFGI_HEADSET_RNG, sizeof(byte)),
   CFGTABLEITEM(CFGI_ANYKEY_ANSWER, sizeof(boolean)),
   CFGTABLEITEM(CFGI_HEADSET_AUTOANSWER, sizeof(byte)),
   CFGTABLEITEM(CFGI_MISSED_CALL_ALERT, sizeof(byte)),
   CFGTABLEITEM(CFGI_ROAMING_ALERT, sizeof(byte)),
   CFGTABLEITEM(CFGI_INTERNET_ALERT, sizeof(byte)),
   CFGTABLEITEM(CFGI_MINUTE_ALERT, sizeof(byte)),
   CFGTABLEITEM(CFGI_GAME_MODE, sizeof(boolean)),
   CFGTABLEITEM(CFGI_GAME_MODE_PERSIST, sizeof(boolean)),

   ////////////////////////////////////////////////////////////////
   // Sound Menu
   ////////////////////////////////////////////////////////////////

   CFGTABLEITEM(CFGI_RINGER_VOL, sizeof(byte)),
   CFGTABLEITEM(CFGI_EAR_VOL, sizeof(byte)),
   CFGTABLEITEM(CFGI_BEEP_VOL, sizeof(byte)),
   CFGTABLEITEM(CFGI_ALERT_TYPE, sizeof(byte)),
#ifdef FEATURE_SMART_SOUND
   CFGTABLEITEM(CFGI_SMART_SOUND, sizeof(boolean)),
#endif

   CFGTABLEITEM(CFGI_KEYBEEP_SOUND, sizeof(byte)),
   CFGTABLEITEM_EMPTY(CFGI_KEYTONE_LENGTH),
   CFGTABLEITEM(CFGI_RINGER_TYPE, sizeof(byte)),
   CFGTABLEITEM(CFGI_IMEI, OEMCFG_IMEI_SIZE),
   CFGTABLEITEM(CFGI_CUG, sizeof(OEMConfigCugInfo)),
   CFGTABLEITEM(CFGI_VR, sizeof(boolean)),
   CFGTABLEITEM(CFGI_AMR, sizeof(boolean)),
   CFGTABLEITEM(CFGI_LCD, sizeof(uint8)),
   CFGTABLEITEM(CFGI_LCD_TIMER, sizeof(uint32)),
   CFGTABLEITEM(CFGI_CCBS, sizeof(OEMCCBS)),

   ////////////////////////////////////////////////////////////////
   // Data Call Counters
   ////////////////////////////////////////////////////////////////
   CFGTABLEITEM(CFGI_LAST_RX_DATA_COUNT, sizeof(uint32)),
   CFGTABLEITEM(CFGI_LAST_TX_DATA_COUNT, sizeof(uint32)),
   CFGTABLEITEM(CFGI_TOTAL_RX_DATA_COUNT, sizeof(uint32)),
   CFGTABLEITEM(CFGI_TOTAL_TX_DATA_COUNT, sizeof(uint32)),
   CFGTABLEITEM(CFGI_BEEP_LENGTH, sizeof(uint32)),

   CFGTABLEITEM(CFGI_CDMA_RX_DIVERSITY_CTL, sizeof(boolean)),
   CFGTABLEITEM(CFGI_HDR_RX_DIVERSITY_CTL, sizeof(boolean)),

   ////////////////////////////////////////////////////////////////
   // Phone Number
   ////////////////////////////////////////////////////////////////
   CFGTABLEITEM_EMPTY(CFGI_PHONE_NUMBER),

   ////////////////////////////////////////////////////////////////
   // Display Menu
   ////////////////////////////////////////////////////////////////

   CFGTABLEITEM(CFGI_BACK_LIGHT_INTENSITY, sizeof(uint8)),

   CFGTABLEITEM(CFGI_LANGUAGE_SELECTION, sizeof(byte)),
   CFGTABLEITEM_EMPTY(CFGI_BANNER),
   CFGTABLEITEM(CFGI_MENU_FORMAT, sizeof(byte)),
   CFGTABLEITEM(CFGI_TIME_FORMAT, sizeof(byte)),
   CFGTABLEITEM(CFGI_MANUAL_PLMN_SEL, sizeof(byte)),
#if defined(FEATURE_USRSYSCLOCK) || defined(FEATURE_ISYSCLOCK)
   CFGTABLEITEM(CFGI_AUTO_TIME_ENABLE, sizeof(byte)),
   CFGTABLEITEM_EMPTY(CFGI_NITZ_NW_INFO),
   CFGTABLEITEM_EMPTY(CFGI_NITZ_NW_INFO_MCC),
   CFGTABLEITEM_EMPTY(CFGI_NITZ_NW_INFO_MNC),
#endif 

   ////////////////////////////////////////////////////////////////
   // Toolkit
   ////////////////////////////////////////////////////////////////
#ifdef FEATURE_GSTK_CS_PS_PARALLEL   
   CFGTABLEITEM(CFGI_TOOLKIT_CS_PS_PARALLEL, sizeof(boolean)),
#endif /* FEATURE_GSTK_CS_PS_PARALLEL */

   ////////////////////////////////////////////////////////////////
   // SMS Applet
   ////////////////////////////////////////////////////////////////

   CFGTABLEITEM(CFGI_SMS_SERVICE_OPTION, sizeof(byte)),
   CFGTABLEITEM(CFGI_SMS_TIMESTAMP, sizeof(byte)),
   CFGTABLEITEM(CFGI_SMS_IS41_WORKAROUND, sizeof(byte)),
   CFGTABLEITEM(CFGI_SMS_MO_ENCODING, sizeof(byte)),
   CFGTABLEITEM(CFGI_SMS_MODE, sizeof(byte)),
   CFGTABLEITEM(CFGI_SMS_APP, sizeof(byte)),
   CFGTABLEITEM(CFGI_SMS_CB_AUTO_DELETE, sizeof(byte)),
   CFGTABLEITEM(CFGI_SMS_CB_SILENT_ALERT, sizeof(byte)),
   CFGTABLEITEM(CFGI_SMS_CB_LANG_FILTER, sizeof(byte)),
   CFGTABLEITEM(CFGI_SMS_ALERT_DISPLAY, sizeof(byte)),
   CFGTABLEITEM(CFGI_SMS_ALERT_SOUND, sizeof(byte)),
   CFGTABLEITEM_EMPTY(CFGI_SMS_ALERT_VOLUME),
   CFGTABLEITEM(CFGI_SMS_CDMA_DEFAULT_TEMPLATE, sizeof(byte)),
   CFGTABLEITEM(CFGI_SMS_GW_DEFAULT_TEMPLATE, sizeof(byte)),
   CFGTABLEITEM(CFGI_SMS_SEGMENT_SETTING, sizeof(byte)),
   CFGTABLEITEM(CFGI_SMS_SEGMENT_REF_NUM, sizeof(word)),
   CFGTABLEITEM_EMPTY(CFGI_SMS_RETRY_PERIOD),
   CFGTABLEITEM_EMPTY(CFGI_SMS_RETRY_INTERVAL),
   CFGTABLEITEM_EMPTY(CFGI_SMS_GCF_FLAG),
   CFGTABLEITEM_EMPTY(CFGI_VOICEMAIL_NUMBER),
   CFGTABLEITEM_EMPTY(CFGI_VOICEMAIL_NUMBER_CDMA),
   CFGTABLEITEM_EMPTY(CFGI_VOICEMAIL_NUMBER_GW),

   ////////////////////////////////////////////////////////////////
   // RecentCalls App
   ////////////////////////////////////////////////////////////////

   CFGTABLEITEM_EMPTY(CFGI_LASTCALL_TIMER),
   CFGTABLEITEM_EMPTY(CFGI_AIR_TIMER),
   CFGTABLEITEM_EMPTY(CFGI_ROAM_TIMER),
   CFGTABLEITEM_EMPTY(CFGI_LIFE_TIMER),

   ////////////////////////////////////////////////////////////////
   // Service Programming
   ////////////////////////////////////////////////////////////////

   CFGTABLEITEM(CFGI_SECCODE,OEMNV_SECCODE_LENGTH * sizeof(AECHAR)),
#ifdef FEATURE_ENABLE_OTKSL
   CFGTABLEITEM(CFGI_OTKSLCODE,OEMNV_SECCODE_LENGTH * sizeof(AECHAR)),
#endif
   CFGTABLEITEM_READONLY(CFGI_ESN,sizeof(uint32)),
   CFGTABLEITEM(CFGI_SLOTINDEX,sizeof(uint8)),
   CFGTABLEITEM_EMPTY(CFGI_BUILD_VERSION),
   CFGTABLEITEM_READONLY(CFGI_RFCAL_VERSION, sizeof(AECHAR) *
                                             OEMNV_RFCALVER_LEN),
   CFGTABLEITEM_READONLY(CFGI_RFCAL_DATE, sizeof(dword)),
   CFGTABLEITEM(CFGI_HOME_SIDNID_LIST,sizeof(AEEConfigSIDNIDPairType) *
                                      OEMNV_HOME_SIDNID_ARRSIZE),
   CFGTABLEITEM(CFGI_LOCK_SIDNID_LIST,sizeof(AEEConfigSIDNIDPairType) *
                                      OEMNV_LOCK_SIDNID_ARRSIZE),
   CFGTABLEITEM(CFGI_IMSI_MCC,sizeof(uint16)),
   CFGTABLEITEM(CFGI_IMSI_11_12,sizeof(uint16)),
   CFGTABLEITEM(CFGI_IMSI_S,sizeof(AECHAR) * OEMNV_IMSI_S_LENGTH),
   CFGTABLEITEM(CFGI_PRL_ENABLED,sizeof(boolean)),
   CFGTABLEITEM_EMPTY(CFGI_PRI_CH_A),
   CFGTABLEITEM_EMPTY(CFGI_PRI_CH_B),
   CFGTABLEITEM_EMPTY(CFGI_SEC_CH_A),
   CFGTABLEITEM_EMPTY(CFGI_SEC_CH_B),
   CFGTABLEITEM(CFGI_AOC,sizeof(uint8)),
   CFGTABLEITEM(CFGI_HOME_SID_REG,sizeof(boolean)),
   CFGTABLEITEM(CFGI_FORN_SID_REG,sizeof(boolean)),
   CFGTABLEITEM(CFGI_FORN_NID_REG,sizeof(boolean)),
#ifdef FEATURE_ACP
#error code not present
#endif /* FEATURE_ACP */
   CFGTABLEITEM(CFGI_DATA_QNC_ENABLED,sizeof(boolean)),
   CFGTABLEITEM_EMPTY(CFGI_DATA_DIALSTRING),
   CFGTABLEITEM_EMPTY(CFGI_BREW_CID),
   CFGTABLEITEM_EMPTY(CFGI_BREW_PID),
   CFGTABLEITEM_EMPTY(CFGI_BREW_BKEY),
   CFGTABLEITEM_EMPTY(CFGI_BREW_AKEY),
   CFGTABLEITEM_EMPTY(CFGI_BREW_SERVER),
   CFGTABLEITEM_EMPTY(CFGI_BREW_FLAGS),
   CFGTABLEITEM_EMPTY(CFGI_BREW_AUTH),
   CFGTABLEITEM_EMPTY(CFGI_BREW_PRIVP),

   ////////////////////////////////////////////////////////////////
   // Security Menu
   ////////////////////////////////////////////////////////////////
   CFGTABLEITEM_EMPTY(CFGI_LOCKCODE),

#ifdef FEATURE_FDN
   ,
   CFGTABLEITEM(CFGI_FDN_ENABLED,sizeof(boolean))
#endif /* FEATURE_FDN */

   CFGTABLEITEM(CFGI_HEADSET,sizeof(boolean)),
   CFGTABLEITEM(CFGI_STEREO_HEADSET,sizeof(boolean)),
   CFGTABLEITEM(CFGI_USB_HFK_ATTACHED,sizeof(boolean)),
   CFGTABLEITEM(CFGI_STEREO_USB_HFK_ATTACHED,sizeof(boolean)),
   CFGTABLEITEM(CFGI_EXT_PWR,sizeof(boolean)),
#ifdef FEATURE_VOICE_FIXED_PATTERN_DEBUG
#error code not present
#endif /* FEATURE_VOICE_FIXED_PATTERN_DEBUG */

   CFGTABLEITEM(CFGI_DISABLE_IN_CALL_DISP, sizeof(boolean)),
   CFGTABLEITEM(CFGI_DISABLE_BG_IMAGE, sizeof(boolean)),
   CFGTABLEITEM(CFGI_MANUAL_PLMN_SEL_ALLOWED, sizeof(boolean)),

};

////
// Config table for the custom config state items.
//
// IMPORTANT:
// The config items in this table MUST be in numeric sequence.  In
// other words, the expression
//
//       customStateItemTable[n].item == customStateItemTable[n-1].item + 1
//
// must hold for all n = 1...ARR_SIZE(customStateItemTable)-1
//
static ConfigItemTableEntry const customStateItemTable[] =
{
   CFGTABLEITEM_READONLY(CFGI_DEVICE,sizeof(uint32)),

   // Debug Menu state items
   CFGTABLEITEM_READONLY(CFGI_DEBUG_CHANNEL,sizeof(uint16)),
   CFGTABLEITEM_READONLY(CFGI_DEBUG_PILOT_PN,sizeof(uint16)),
   CFGTABLEITEM_READONLY(CFGI_DEBUG_TX_AGC,sizeof(uint16)),
   CFGTABLEITEM_READONLY(CFGI_DEBUG_TX_AGC_IN_DBM,sizeof(int16)),
   CFGTABLEITEM_READONLY(CFGI_DEBUG_RX_AGC,sizeof(signed char)),
   CFGTABLEITEM_READONLY(CFGI_DEBUG_RX_AGC_IN_DBM,sizeof(int32)),
   CFGTABLEITEM_READONLY(CFGI_DEBUG_SID,sizeof(uint16)),
   CFGTABLEITEM_READONLY(CFGI_DEBUG_NID,sizeof(uint16)),
   CFGTABLEITEM_READONLY(CFGI_DEBUG_TX_ADJUST,sizeof(signed char)),
   CFGTABLEITEM_READONLY(CFGI_DEBUG_HDET,sizeof(byte)),
   CFGTABLEITEM_READONLY(CFGI_DEBUG_VBATT,sizeof(byte)),
   CFGTABLEITEM_READONLY(CFGI_DEBUG_ERRLOG,sizeof(OEMErrLogType)),
   CFGTABLEITEM_READONLY(CFGI_DEBUG_LNA_STATE,sizeof(byte)),
   CFGTABLEITEM_READONLY(CFGI_DEBUG_PA_STATE,sizeof(byte)),
   CFGTABLEITEM_READONLY(CFGI_DEBUG_RATCHET_STATE,sizeof(byte)),

#if defined(FEATURE_WCDMA)
#error code not present
#endif

#ifdef T_MSM6500
   CFGTABLEITEM_READONLY(CFGI_DEBUG_RF_CARD_NV_ID,sizeof(uint16)),
   CFGTABLEITEM_READONLY(CFGI_DEBUG_THERM_BATT,sizeof(byte)),
   CFGTABLEITEM_READONLY(CFGI_DEBUG_THERM_MSM,sizeof(byte)),
   CFGTABLEITEM_READONLY(CFGI_DEBUG_THERM_PA,sizeof(byte)),
   CFGTABLEITEM_READONLY(CFGI_DEBUG_THERM_PMIC,sizeof(byte)),
   CFGTABLEITEM_READONLY(CFGI_DEBUG_THERM_CHRGR,sizeof(byte)),
#endif

   // Items required for power calculation in Debug Menu
   CFGTABLEITEM_READONLY(CFGI_CDMA_DYNAMIC_RANGE,sizeof(uint16)),
   CFGTABLEITEM_READONLY(CFGI_CDMA_MIN_RX_RSSI,sizeof(uint16)),

#ifdef FEATURE_QIPCALL
   CFGTABLEITEM_READONLY(CFGI_QIPCALL_DEBUG_EVDO_IP_ADDR, sizeof(dword)),
   CFGTABLEITEM_READONLY(CFGI_QIPCALL_DEBUG_EVDO_SIP_REG_STATUS, sizeof(byte)),
   CFGTABLEITEM_READONLY(CFGI_QIPCALL_DEBUG_WLAN_IP_ADDR, sizeof(dword)),
   CFGTABLEITEM_READONLY(CFGI_QIPCALL_DEBUG_WLAN_SIP_REG_STATUS, sizeof(byte)),
#endif

#ifdef FEATURE_HDR
#error code not present
#endif

   CFGTABLEITEM_READONLY(CFGI_DEBUG_THERM,sizeof(word)),
   CFGTABLEITEM_READONLY(CFGI_DEBUG_BAND,sizeof(byte)),

   CFGTABLEITEM_EMPTY(CFGI_KEYGUARD_ENABLED),

#ifdef FEATURE_INTELLICEIVER
  CFGTABLEITEM_READONLY(CFGI_DEBUG_INTELLICEIVER_STATE,sizeof(byte)),
#endif /* FEATURE_INTELLICEIVER */
};

////
//  The table containing all the config tables.
static ConfigTableInfo const cfgTable[] =
{
   { customItemTable,      ARR_SIZE(customItemTable)      },
   { customStateItemTable, ARR_SIZE(customStateItemTable) }
};

/* Cache initialized ? */
static boolean cache_initialized = FALSE;
static boolean cfgi_lcd;

/*===========================================================================

                     FUNCTION DEFINITIONS

============================================================================*/

boolean   OEM_GetCachedLCDSetting()
{
  boolean returnVal;

  if(OEMPriv_GetItem_CFGI_LCD(&returnVal) == SUCCESS)
    return returnVal;
  else
    return TRUE;
}

/*===========================================================================

FUNCTION OEM_IsHeadsetPresent

DESCRIPTION
  Query if headset is currently detected

DEPENDENCIES


RETURN VALUE
  TRUE - if headset is detected
  FALSE - if headset is not detected

SIDE EFFECTS
  None

===========================================================================*/
boolean OEM_IsHeadsetPresent (void)
{

  return(HS_HEADSET_ON());

}

#ifdef FEATURE_FACTORY_TESTMODE
/*=============================================================================
FUNCTION:  OEM_IsFactoryTestMode

DESCRIPTION: Returns TRUE if the phone is in 'Factory Test Mode'.

PARAMETERS:
   None

RETURN VALUE:
   boolean

COMMENTS:

SIDE EFFECTS:

SEE ALSO:

=============================================================================*/
boolean OEM_IsFactoryTestMode(void)
{
   boolean bRet = FALSE;
   nv_item_type nvi;
      
   if (NV_DONE_S == OEMNV_Get(NV_FTM_MODE_I, &nvi)) {
      bRet = nvi.ftm_mode ? TRUE : FALSE;
   }
   return bRet;
}
#endif /*FEATURE_FACTORY_TESTMODE*/


/*===========================================================================
FUNCTION: OEM_RestoreFactorySetting

DESCRIPTION


RETURN VALUE

DEPENDENCIES

===========================================================================*/
void OEM_RestoreFactorySetting( void )
{
   nv_item_type nvi;
   disp_info_type di = disp_get_info();

   // Reset the OEM Configuration list
   //
   // NOTE: Any changes to these values should be mirrored in the
   //       initialization of the oemi_cache variable.
   oemi_cache.alert_type          = OEMNV_ALERTTYPE_RINGER;
   oemi_cache.silence_all         = OEMNV_SA_NORMAL;
   oemi_cache.headset_ringer      = OEMNV_HEADSET_RNG_ON;
   oemi_cache.missed_call_alert   = OEMNV_ALERT_ENABLE;
   oemi_cache.internet_call_alert = OEMNV_ALERT_ENABLE;
   oemi_cache.internet_call_time  = 0;
   oemi_cache.sms_service_option  = OEMNV_SMS_SO_SO6;
   oemi_cache.sms_is41_workaround = OEMNV_SMS_EMAIL_ADDRESSING_IS41;
   oemi_cache.sms_mo_encoding     = OEMNV_SMS_MO_ENCODING_7BIT;
   oemi_cache.sms_mode            = OEMNV_SMS_MODE_CDMA;
   oemi_cache.sms_app             = OEMNV_SMS_APP_TEST;
   oemi_cache.sms_cb_auto_delete  = OEMNV_SMS_CB_DISABLE_AUTO_DELETE;
   oemi_cache.sms_cb_silent_alert = OEMNV_SMS_CB_DISABLE_SILENT_ALERT;
   oemi_cache.sms_cb_lang_filter  = OEMNV_SMS_CB_DISABLE_LANG_FILTER;
   oemi_cache.sms_alert_display   = OEMNV_SMS_ALERT_DISPLAY_ENABLE;
   oemi_cache.sms_alert_sound     = OEMNV_SMS_ALERT_SOUND_ENABLE;
   oemi_cache.sms_alert_volume    = OEMNV_SMS_ALERT_VOLUME_2;
   oemi_cache.sms_cdma_default_template = 0;
   oemi_cache.sms_gw_default_template   = 0;
   oemi_cache.sms_segment_setting       = 0;
   oemi_cache.sms_segment_ref_num       = 0;
   oemi_cache.sms_retry_period          = OEMNV_SMS_RETRY_PERIOD;
   oemi_cache.sms_retry_interval        = OEMNV_SMS_RETRY_INTERVAL;
   oemi_cache.sms_gcf_flag              = FALSE;
   MEMSET((void *) oemi_cache.voicemail_number,
          0,
          sizeof(oemi_cache.voicemail_number));
   MEMSET((void *) oemi_cache.voicemail_number_cdma,
          0,
          sizeof(oemi_cache.voicemail_number_cdma));
   MEMSET((void *) oemi_cache.voicemail_number_gw,
          0,
          sizeof(oemi_cache.voicemail_number_gw));
   oemi_cache.time_format         = OEMNV_TIMEFORM_AMPM;

   MEMSET((void *)&oemi_cache.cug, 0, sizeof(OEMConfigCugInfo));

   oemi_cache.vr = 0;

   MEMSET((void *)&oemi_cache.download_info, 0, sizeof(OEMConfigDownloadInfo));
   oemi_cache.download_info.dwCID  = OEMCFG_DEFAULT_CID;
   oemi_cache.download_info.dwPID  = OEMCFG_DEFAULT_PLATFORM_ID;
   oemi_cache.download_info.wFlags = DIF_TEST_ALLOWED | DIF_MIN_FOR_SID;
   oemi_cache.download_info.wAuth  = APOLICY_NONE;
   oemi_cache.download_info.wPrivP = PPOLICY_BREW_OR_CARRIER;
   STRLCPY((char *)oemi_cache.download_info.szServer, 
           (const char *)OEMCFG_DEFAULT_DL_SERVER, 
           sizeof(oemi_cache.download_info.szServer));

   oemi_cache.disable_in_call_disp = FALSE;
   oemi_cache.disable_bg_image = FALSE;
   oemi_cache.manual_plmn_sel_allowed = TRUE;

   OEMPriv_WriteOEMConfigList();

   // Backlight with external pwr
   nvi.back_light_hfk = OEMNV_EXTPWR_BL_ON;
   (void) OEMNV_Put( NV_BACK_LIGHT_HFK_I, &nvi );
   nvi_cache.backlight_hfk = OEMNV_EXTPWR_BL_ON;

   // CFGI_RINGER_VOL:
   nvi.ringer_level = OEMNV_VOLUME_MID;
   (void) OEMNV_Put( NV_RINGER_LVL_I, &nvi );
   nvi_cache.ringer_level = OEMNV_VOLUME_MID;

   // CFGI_EAR_VOL:
   nvi.ear_level = OEMNV_VOLUME_MID;
   (void) OEMNV_Put( NV_EAR_LVL_I, &nvi );
   nvi_cache.handset_ear_level = OEMNV_VOLUME_MID;

   // CFGI_BEEP_VOL:
   nvi.beep_level = OEMNV_VOLUME_MID;
   (void) OEMNV_Put( NV_BEEP_LVL_I, &nvi );
   nvi_cache.beep_level = OEMNV_VOLUME_MID;

#ifdef FEATURE_SMART_SOUND
   // CFGI_SMART_SOUND:
   nvi.auto_volume_enabled = OEMNV_SMARTSOUND_DISABLE;
   (void) OEMNV_Put( NV_AUTO_VOLUME_ENABLED_I, &nvi );
   nvi_cache.smart_sound = OEMNV_SMARTSOUND_DISABLE;
   OEMSound_SmartSoundEnable(FALSE);
#endif

   // CFGI_RINGER_TYPE:
   nvi.ringer_type = (byte) SND_ALERT;
   (void) OEMNV_Put( NV_RINGER_TYPE_I, &nvi );
   nvi_cache.ringer_type = (byte) SND_ALERT;

   // CFGI_KEYBEEP_SOUND:
   nvi.key_sound = (nv_key_sound_enum_type) OEMNV_KEYBEEP_TONE;
   (void) OEMNV_Put( NV_KEY_SOUND_I, &nvi );
   nvi_cache.key_sound = OEMNV_KEYBEEP_TONE;

   // LCD contrast
   nvi.lcd = di.contrast_default;
   (void) OEMNV_Put( NV_LCD_I, &nvi );
   nvi_cache.contrast = di.contrast_default;

   // Menu Format
   nvi.menu_format = (nv_menu_format_enum_type) OEMNV_MENUFORM_LARGE;
   (void) OEMNV_Put( NV_MENU_FORMAT_I, &nvi );
   nvi_cache.menu_format = (byte) OEMNV_MENUFORM_LARGE;

   // NV_BACK_LIGHT_I
   nvi.back_light = OEMNV_BL_10S;
   (void) OEMNV_Put( NV_BACK_LIGHT_I, &nvi );
   nvi_cache.backlight = OEMNV_BL_10S;

   // Data Call Counters
   nvi.last_rx_data_count = 0;
   (void) OEMNV_Put( NV_LAST_RX_DATA_COUNT_I, &nvi);
   nvi_cache.last_rx_data_count = 0;

   nvi.last_tx_data_count = 0;
   (void) OEMNV_Put( NV_LAST_TX_DATA_COUNT_I, &nvi);
   nvi_cache.last_tx_data_count = 0;

   nvi.last_rx_data_count = 0;
   (void) OEMNV_Put( NV_TOTAL_RX_DATA_COUNT_I, &nvi);
   nvi_cache.total_rx_data_count = 0;

   nvi.total_tx_data_count = 0;
   (void) OEMNV_Put( NV_TOTAL_TX_DATA_COUNT_I, &nvi);
   nvi_cache.total_tx_data_count = 0;

#ifdef FEATURE_WCDMA
#error code not present
#else
   nvi.dtmf = OEMNV_DTMFBURST; // Burst
#endif
   (void) OEMNV_Put( NV_DTMF_I, &nvi);
   nvi_cache.beep_length = nvi.dtmf;

#ifdef FEATURE_TTY
   nvi.tty = (byte)OEMNV_TTY_OFF;
   (void) OEMNV_Put( NV_TTY_I, &nvi);
   nvi_cache.tty = (OEMTTY)nvi.tty;
#endif  /* FEATURE_TTY */

   nvi.game_mode_persist = 0;
   (void) OEMNV_Put( NV_GAME_MODE_PERSIST_I, &nvi);
   nvi_cache.game_mode_persist = nvi.game_mode_persist;

   nvi.game_mode = 0;
   (void) OEMNV_Put( NV_GAME_MODE_I, &nvi);
   nvi_cache.game_mode = nvi.game_mode;

   nvi.manual_plmn_sel = 0;
   (void) OEMNV_Put( NV_MANUAL_PLMN_SEL_I, &nvi);
   nvi_cache.manual_plmn_sel = nvi.manual_plmn_sel;

#ifdef FEATURE_GSTK_CS_PS_PARALLEL
   nvi.toolkit_cs_ps_parallel = FALSE;
   (void) OEMNV_Put( NV_TOOLKIT_CS_PS_PARALLEL_I, &nvi);
   nvi_cache.toolkit_cs_ps_parallel = nvi.toolkit_cs_ps_parallel;
#endif /* FEATURE_GSTK_CS_PS_PARALLEL */

#ifdef FEATURE_CCBS
#error code not present
#endif

   nvi.display_duration = 0;
   (void) OEMNV_Put( NV_DISPLAY_DURATION_I, &nvi);
   nvi_cache.display_duration = nvi.display_duration;

   nvi.back_light_intensity = 0;
   (void) OEMNV_Put( NV_BACK_LIGHT_INTENSITY_I, &nvi);
   nvi_cache.backlight_intensity = nvi.back_light_intensity;

#if defined(FEATURE_USRSYSCLOCK) || defined(FEATURE_ISYSCLOCK)
    nvi.auto_time_enable = 0;
   (void) OEMNV_Put( NV_AUTO_TIME_ENABLE_I, &nvi);
   nvi_cache.auto_time_enable = nvi.auto_time_enable;
   
   memset((void *) &nvi.nitz_nw_info, 0 , sizeof (nvi.nitz_nw_info));
   memset( (void *) &nvi_cache.plmn_name, 0 , sizeof (nvi_cache.plmn_name));
   (void) OEMNV_Put( NV_NITZ_NW_INFO_I, &nvi);

   nvi.nitz_nw_info_mcc = nvi_cache.mcc = 0;
   (void) OEMNV_Put( NV_NITZ_NW_INFO_MCC_I, &nvi);

   nvi.nitz_nw_info_mnc = nvi_cache.mnc = 0;
   (void) OEMNV_Put( NV_NITZ_NW_INFO_MNC_I, &nvi);
#endif 

   nvi.voice_priv = VOICEPRIVACY_STD;
   (void) OEMNV_Put( NV_VOICE_PRIV_I, &nvi);
   nvi_cache.voice_priv = nvi.voice_priv;  
}



/*=============================================================================
FUNCTION: OEMPriv_MCC_TO_DEC

DESCRIPTION:  Converts an MCC type number to a normal decimal number

PARAMETERS:
   mcc:

RETURN VALUE:
   uint16:

COMMENTS:

SIDE EFFECTS:

SEE ALSO:

=============================================================================*/
static uint16 OEMPriv_MCC_TO_DEC(uint16 mcc)
{
   static const char mintab[] = {
      '1', '2', '3', '4', '5', '6', '7', '8', '9', '0'
   };
   char   txt[4];

   txt[0] = mintab[ mcc/100 ];
   mcc %= 100;
   txt[1] = mintab[ mcc/10 ];
   txt[2] = mintab[ mcc%10 ];
   txt[3] = 0;

   return (uint16) ATOI(txt);
}


/*=============================================================================
FUNCTION: OEMPriv_MIN2_TO_DEC

DESCRIPTION:  Converts an MIN2 type number to a normal decimal number

PARAMETERS:
   mcc:

RETURN VALUE:
   uint16:

COMMENTS:

SIDE EFFECTS:

SEE ALSO:

=============================================================================*/
static uint16 OEMPriv_MIN2_TO_DEC(uint16 min2)
{
   static const char mintab[] = {
      '1', '2', '3', '4', '5', '6', '7', '8', '9', '0'
   };
   char   txt[4];

   txt[0] = mintab[ min2/100 ];
   min2 %= 100;
   txt[1] = mintab[ min2/10 ];
   txt[2] = mintab[ min2%10 ];
   txt[3] = 0;

   return (uint16) ATOI(txt);
}


/*=============================================================================
FUNCTION: OEMPriv_MIN1_TO_STR

DESCRIPTION: Convert a MIN1 type number to ASCII

PARAMETERS:
   min1: MIN1 to convert
   *txt: string buffer with enough room for eight characters

RETURN VALUE:
   None

COMMENTS:

SIDE EFFECTS:

SEE ALSO:

=============================================================================*/
static void OEMPriv_MIN1_TO_STR(uint32  min1,
                                char   *txt)
{
   static const char mintab[] = {
      '1', '2', '3', '4', '5', '6', '7', '8', '9', '0'
   };
   word temp;

   if (min1 == 0) {
      MEMSET(txt, '0', 7);
   } else {
      temp = (word) (min1>>14);
      *txt++ = mintab[temp/100];
      temp %= 100;
      *txt++ = mintab[temp/10];
      *txt++ = mintab[temp%10];

      min1 &= 0x3FFFL;                  /* get bottom 14 bits */
      /* next digit is top 4 bits */
      temp = (word) (( min1 >> 10 ) & 0xF );
      *txt++ = (char) ( ( ( temp == 10 ) ? 0 : temp ) + '0' );
      temp = (word) ( min1 & 0x3FF );   /* get bottom 10 bits */
      *txt++ = mintab[ temp/100 ];
      temp %= 100;
      *txt++ = mintab[ temp/10 ];
      *txt++ = mintab[ temp%10 ];
      *txt = 0;
   }


}

/*=============================================================================
FUNCTION:  OEM_InitPreference

DESCRIPTION:
   Initializes the OEM configuration information from NV

PARAMETERS:
   None

RETURN VALUE:
   None

COMMENTS:

SIDE EFFECTS:

SEE ALSO:

=============================================================================*/
void OEM_InitPreference(void)
{
#ifdef FEATURE_SMART_SOUND
  boolean        smart_data;
#endif
  nv_item_type   nvi;

  // Read the OEM Configuration List
  OEMPriv_ReadOEMConfigList();

  //
  // Load the NV Configuration List
  //

  //////////////////////////////////////////////////////////////////////
  // Setting Menu
  //////////////////////////////////////////////////////////////////////
  (void) OEMNV_Get( NV_LIFE_TIMER_G_I, &nvi );
  nvi_cache.all_call_time = nvi.life_timer_g.time;

  (void) OEMNV_Get( NV_BACK_LIGHT_HFK_I, &nvi );
  nvi_cache.backlight_hfk = nvi.back_light_hfk;

  (void) OEMNV_Get(NV_SVC_AREA_ALERT_I, &nvi);
  nvi_cache.svc_alert = nvi.svc_area_alert ? 1 : 0;

  //////////////////////////////////////////////////////////////////////
  // Sound Menu
  //////////////////////////////////////////////////////////////////////
  // VOLUMES
  /* Ringer volume level                             */
  (void) OEMNV_Get( NV_RINGER_LVL_I, &nvi );
  nvi_cache.ringer_level = nvi.ringer_level;

  /* Handset ear piece volume level                  */
  (void) OEMNV_Get( NV_EAR_LVL_I, &nvi );
  nvi_cache.handset_ear_level = nvi.ear_level;

  /* Key beep volume level                           */
  (void) OEMNV_Get( NV_BEEP_LVL_I, &nvi );
  nvi_cache.beep_level = nvi.beep_level;

#ifdef FEATURE_SMART_SOUND
  (void) OEMNV_Get( NV_AUTO_VOLUME_ENABLED_I, &nvi );
  nvi_cache.smart_sound = nvi.auto_volume_enabled;
#endif

  (void) OEMNV_Get( NV_RINGER_TYPE_I, &nvi );
  nvi_cache.ringer_type = nvi.ringer_type;

  (void) OEMNV_Get( NV_KEY_SOUND_I, &nvi );
  nvi_cache.key_sound = (byte) nvi.key_sound;


  ////////////////////////////////////////////////////////////////
  // Display Menu
  ////////////////////////////////////////////////////////////////

  (void) OEMNV_Get( NV_BACK_LIGHT_I, &nvi );
  nvi_cache.backlight = nvi.back_light;

  (void) OEMNV_Get( NV_BACK_LIGHT_INTENSITY_I, &nvi );
  nvi_cache.backlight_intensity = nvi.back_light_intensity;

  (void) OEMNV_Get( NV_LCD_I, &nvi );
  nvi_cache.contrast = nvi.lcd;

  (void) OEMNV_Get( NV_DISPLAY_DURATION_I, &nvi );
  nvi_cache.display_duration = nvi.display_duration;

  (void) OEMNV_Get( NV_LANGUAGE_SELECTION_I, &nvi );
  nvi_cache.language = (byte) nvi.language_selection;

  (void) OEMNV_Get( NV_BANNER_I, &nvi );
  {
     /* Convert the nv banner data from UTF-8 to Unicode */
     int bannerLen=0;

     while ( (bannerLen < (int) sizeof(nvi.banner.letters)) &&
             (nvi.banner.letters[bannerLen]) ) {
        ++bannerLen;
     }
     memset(nvi_cache.banner, 0, sizeof(nvi_cache.banner));
     (void) UTF8TOWSTR((byte *) nvi.banner.letters,
                       bannerLen,
                       nvi_cache.banner,
                       sizeof(nvi_cache.banner) - sizeof(AECHAR));
  }

  (void) OEMNV_Get( NV_MENU_FORMAT_I, &nvi );
  nvi_cache.menu_format = (byte)nvi.menu_format;

  (void) OEMNV_Get( NV_LOCK_CODE_I, &nvi );
  memcpy(nvi_cache.lockcode,
         (byte *) nvi.lock_code.digits,
         sizeof(nvi_cache.lockcode));

#ifdef FEATURE_SMART_SOUND
  // Tell the sound logic to turn on or off smartsound.
  (void) OEM_GetConfig(CFGI_SMART_SOUND, &smart_data, sizeof(boolean));
  OEMSound_SmartSoundEnable(smart_data);
#endif


#if defined(FEATURE_GSM) || defined (FEATURE_WCDMA)
#error code not present
#else
  nvi_cache.amr  = TRUE;
#endif

  //Diversity control for CDMA
#ifdef FEATURE_CDMA_RX_DIVERSITY
  (void) OEMNV_Get( NV_CDMA_RX_DIVERSITY_CTRL_I, &nvi );
  nvi_cache.cdma_rx_diversity_ctrl = nvi.cdma_rx_diversity_ctrl;
#else
  nvi_cache.cdma_rx_diversity_ctrl = FALSE;
#endif

  //Diversity control for HDR
#ifdef FEATURE_HDR
#error code not present
#else
  nvi_cache.hdr_rx_diversity_ctrl = FALSE;
#endif

  // TTY
  if( OEMNV_Get( NV_TTY_I, &nvi) != NV_DONE_S )
  {
   nvi.tty = (byte)OEMNV_TTY_OFF;
   (void) OEMNV_Put( NV_TTY_I, &nvi);
  }
  nvi_cache.tty  = (OEMTTY)nvi.tty;

  //game mode
  if( OEMNV_Get( NV_GAME_MODE_PERSIST_I, &nvi) != NV_DONE_S )
  {
   nvi.game_mode_persist = 0;
   (void) OEMNV_Put( NV_GAME_MODE_PERSIST_I, &nvi);
  }
  nvi_cache.game_mode_persist  = nvi.game_mode_persist;

  if( OEMNV_Get( NV_GAME_MODE_I, &nvi) != NV_DONE_S )
  {
   nvi.game_mode = 0;
   (void) OEMNV_Put( NV_GAME_MODE_I, &nvi);
  }
  
  //only set game mode to the NV value if persistent is on
  if (nvi_cache.game_mode_persist) {
    nvi_cache.game_mode  = nvi.game_mode;
  } else {
    nvi_cache.game_mode = 0;
  }

  // Data Call Counters
  (void) OEMNV_Get( NV_LAST_RX_DATA_COUNT_I, &nvi);
  nvi_cache.last_rx_data_count = nvi.last_rx_data_count;

  (void) OEMNV_Get( NV_LAST_TX_DATA_COUNT_I, &nvi);
  nvi_cache.last_tx_data_count = nvi.last_tx_data_count;

  (void) OEMNV_Get( NV_TOTAL_RX_DATA_COUNT_I, &nvi);
  nvi_cache.total_rx_data_count = nvi.last_rx_data_count;

  (void) OEMNV_Get( NV_TOTAL_TX_DATA_COUNT_I, &nvi);
  nvi_cache.total_tx_data_count = nvi.total_tx_data_count;

  if( OEMNV_Get( NV_DTMF_I, &nvi) != NV_DONE_S)
  {
#ifdef FEATURE_WCDMA
#error code not present
#else
   nvi.dtmf = OEMNV_DTMFBURST; // Burst
#endif
   (void) OEMNV_Put( NV_DTMF_I, &nvi);
  }
  nvi_cache.beep_length = nvi.dtmf;

  if( OEMNV_Get( NV_MANUAL_PLMN_SEL_I, &nvi) != NV_DONE_S)
  {
    nvi.manual_plmn_sel = 0; // Read From Card by Default
    (void) OEMNV_Put( NV_MANUAL_PLMN_SEL_I, &nvi);
  }
  nvi_cache.manual_plmn_sel = nvi.manual_plmn_sel;

#ifdef FEATURE_GSTK_CS_PS_PARALLEL
  if( OEMNV_Get( NV_TOOLKIT_CS_PS_PARALLEL_I, &nvi) != NV_DONE_S)
  {
    nvi.toolkit_cs_ps_parallel = 0; // Read From Card by Default
    (void) OEMNV_Put( NV_TOOLKIT_CS_PS_PARALLEL_I, &nvi);
  }
  nvi_cache.toolkit_cs_ps_parallel = nvi.toolkit_cs_ps_parallel;
#endif /* FEATURE_GSTK_CS_PS_PARALLEL */
    
#ifdef FEATURE_CCBS
#error code not present
#endif

  cfgi_lcd = TRUE;

#if defined(FEATURE_USRSYSCLOCK) || defined(FEATURE_ISYSCLOCK)
  if( OEMNV_Get( NV_AUTO_TIME_ENABLE_I, &nvi) != NV_DONE_S){
    nvi.auto_time_enable = FALSE;  
    (void) OEMNV_Put( NV_AUTO_TIME_ENABLE_I, &nvi);
  }
  nvi_cache.auto_time_enable = nvi.auto_time_enable;
  
  OEMNV_Get( NV_NITZ_NW_INFO_I, &nvi);
  {
      /* Convert the nv banner data from UTF-8 to Unicode */
     int Len =0;

     while ( (Len < (int) sizeof(nvi.nitz_nw_info.plmn_name)) &&
             (nvi.nitz_nw_info.plmn_name[Len]) ) {
        ++Len;
     }
     memset(nvi_cache.plmn_name, 0, sizeof(nvi_cache.plmn_name));
     (void) UTF8TOWSTR((byte *) nvi.nitz_nw_info.plmn_name,
                       Len,
                       nvi_cache.plmn_name,
                       sizeof(nvi_cache.plmn_name) - sizeof(AECHAR));
                       
  }
  
  (void)OEMNV_Get( NV_NITZ_NW_INFO_MCC_I, &nvi);
  nvi_cache.mcc = nvi.nitz_nw_info_mcc;

  (void)OEMNV_Get( NV_NITZ_NW_INFO_MNC_I, &nvi);
  nvi_cache.mnc = nvi.nitz_nw_info_mnc;
  
#endif 
  if( OEMNV_Get( NV_VOICE_PRIV_I, &nvi) != NV_DONE_S){
    nvi.voice_priv = VOICEPRIVACY_STD;  
    (void) OEMNV_Put( NV_VOICE_PRIV_I, &nvi);
  }
  nvi_cache.voice_priv = nvi.voice_priv;
  cache_initialized = TRUE;
}


/*==================================================================
Function: OEM_GetCachedConfig

Description: Retrieves cached configuration information from the OEM

Prototype:
   int  OEM_GetCachedConfig(AEEConfigItem i, void * pBuff, int nSize)

Parameter(s):

Return Value:  None
Comments:      None
Side Effects:  None
See Also:      None

==================================================================*/
int OEM_GetCachedConfig(AEEConfigItem i, void * pBuff, int nSize)
{
   uint32 j;
   nv_item_type nvi;

   if (!cache_initialized) {
      OEM_InitPreference();
   }

   if (pBuff == NULL) {
      ERR("GetConfig(): invalid parm size", 0, 0, 0);
      return EBADPARM;
   }

   // First check if the item can be found in any of the config
   // tables before resorting to the ugly switch() statement below...
   for (j = 0; j < ARR_SIZE(cfgTable); j++) {
      // Is the item in the range of this table?
      if ( (i >= cfgTable[j].tbl[0].item) &&
           (i <= cfgTable[j].tbl[cfgTable[j].size-1].item) ) {

         int idx = (int)i - (int) cfgTable[j].tbl[0].item;

         // Ensure the config item table is valid
         ASSERT(i == (int) cfgTable[j].tbl[idx].item);

         if (NULL == cfgTable[j].tbl[idx].get) {
            // Exit the for() loop immediately if we found the
            // entry and it had a NULL function.
            break;
         }

         // Perform the size check here so the item functions can
         // assume the size is ok.
         if (nSize != cfgTable[j].tbl[idx].size) {
            ERR("GetConfig(): invalid parm size", 0, 0, 0);
            return EBADPARM;
         }

         return cfgTable[j].tbl[idx].get(pBuff);
      }
   }

   // Didn't find the config item in the tables, maybe it's in this switch...
   switch (i) {
   case CFGI_BACK_LIGHT:
      if (nSize != sizeof(byte)) {
         ERR("GetConfig(): invalid parm size", 0, 0, 0);
         return EBADPARM;
      }
      return OEMPriv_GetItem_CFGI_BACK_LIGHT(pBuff);

   case CFGI_SVC_ALERT:
      if (nSize != sizeof(byte)) {
         ERR("GetConfig(): invalid parm size", 0, 0, 0);
         return EBADPARM;
      }
      return OEMPriv_GetItem_CFGI_SVC_ALERT(pBuff);

   case CFGI_EXTPWR_BK_LIGHT:
      if (nSize != sizeof(byte)) {
         ERR("GetConfig(): invalid parm size", 0, 0, 0);
         return EBADPARM;
      }
      return OEMPriv_GetItem_CFGI_EXTPWR_BK_LIGHT(pBuff);

   case CFGI_CONTRAST_LVL:
      if (nSize != sizeof(byte)) {
         ERR("GetConfig(): invalid parm size", 0, 0, 0);
         return EBADPARM;
      }
      return OEMPriv_GetItem_CFGI_CONTRAST_LVL(pBuff);
  
   case CFGI_VOICEPRIVACY:
      if (nSize != sizeof(byte)) {
         ERR("GetConfig(): invalid parm size", 0, 0, 0);
         return EBADPARM;
      }
      return OEMPriv_GetItem_CFGI_VOICEPRIVACY(pBuff);

   case CFGI_RTRE_CONFIG:
      if (nSize != sizeof(byte)) {
         ERR("GetConfig(): invalid parm size", 0, 0, 0);
         return EBADPARM;
      }
#ifdef FEATURE_UIM_RUN_TIME_ENABLE
      if (NV_DONE_S != OEMNV_Get(NV_RTRE_CONFIG_I, &nvi)) {
         // Default to R-UIM only.
         *(byte *) pBuff = RTRE_CONFIG_RUIMORDROPBACK;
         return SUCCESS;
      }
      *(byte *) pBuff = (byte)nvi.rtre_config;
      return SUCCESS;
#else
      *(byte *) pBuff = (byte)RTRE_CONFIG_DISABLED;
      return SUCCESS;
#endif /* FEATURE_UIM_RUN_TIME_ENABLE */


   case CFGI_UNITTEST_ACTIVE:
      if (nSize != (int)sizeof(boolean)) {
         ERR("GetConfig(): invalid parm size", 0, 0, 0);
         return EBADPARM;
      }
#ifdef srIMON
#error code not present
#else
      *(boolean *) pBuff = FALSE;
#endif /* srIMON */
      return SUCCESS;

   case CFGI_RADIO_TECH:
      if (nSize != sizeof(byte)) {
         ERR("GetConfig(): invalid parm size", 0, 0, 0);
         return EBADPARM;
      }

      *(byte *) pBuff = 0;
#ifdef FEATURE_CDMA1X
      *(byte *) pBuff |= RADIO_TECH_CDMA;
#endif
#ifdef FEATURE_GSM
#error code not present
#endif
#ifdef FEATURE_WCDMA
#error code not present
#endif
      return SUCCESS;

#ifdef AEE_SIMULATOR
   case CFGI_LNG:
      if (nSize != (int) sizeof(uint32)) {
         return EBADPARM;
      }

      // Default to Off
      *(uint32 *) pBuff = 0;

      if (NV_DONE_S == OEMNV_Get(NV_LANG_I, &nvi)) {
         *(uint32 *) pBuff = nvi.u;
      }
      return SUCCESS;

#endif

   case CFGI_FIRMWARE_VERSION:
      if (nSize != FIRMWAREVERSION_MAX_LEN * (int)sizeof(AECHAR)) {
         ERR("GetConfig(): invalid parm size", 0, 0, 0);
         return EBADPARM;
      }
      (void) STRTOWSTR(mob_sw_rev, 
                       (AECHAR*)pBuff, 
                       FIRMWAREVERSION_MAX_LEN *(int) sizeof(AECHAR));
      return SUCCESS;

   case CFGI_AUTO_ANSWER:
   {
      nv_item_type nvi;
      
      if (nSize != sizeof(boolean)) {
         ERR("GetConfig(): invalid parm size", 0, 0, 0);
         return EBADPARM;
      }
     
      if (OEMNV_Get(NV_AUTO_ANSWER_I, &nvi) != NV_DONE_S)
      {
         ERR("OEM_GetCachedConfig: OEMNV_Get failed",0,0,0);
         return EFAILED;   
      }
            
      *(boolean*)pBuff = nvi.auto_answer.enable;

      return SUCCESS;

   }

   case CFGI_TTY:
   {
      nv_item_type nvi;

      if (nSize != sizeof(uint32)){
         ERR("GetConfig(): invalid parm size", 0, 0, 0);
         return EBADPARM;
      }

      if (OEMNV_Get(NV_TTY_I, &nvi) != NV_DONE_S)
      {
         ERR("OEM_GetCachedConfig: OEMNV_Get failed",0,0,0);
         return EFAILED;
      }

      *(uint32 *)pBuff  = nvi.tty;

      return SUCCESS;
   }
   
   case CFGI_LP_SEC:
   {
      nv_item_type nvi;

      if (nSize != sizeof(uint8)){
         ERR("GetConfig(): invalid parm size", 0, 0, 0);
         return EBADPARM;
      }

      if (OEMNV_Get(NV_DB_LP_SEC_I, &nvi) != NV_DONE_S)
      {
         ERR("OEM_GetCachedConfig: OEMNV_Get failed",0,0,0);
         return EFAILED;
      }

      *(uint8 *)pBuff  = nvi.db_lp_sec;

      return SUCCESS;
   }
   
   
   
   case CFGI_LTM_OFF:
   {
      nv_item_type nvi;

      if (nSize != sizeof(uint8)){
         ERR("GetConfig(): invalid parm size", 0, 0, 0);
         return EBADPARM;
      }

      if (OEMNV_Get(NV_DB_LTM_OFF_I, &nvi) != NV_DONE_S)
      {
         ERR("OEM_GetCachedConfig: OEMNV_Get failed",0,0,0);
         return EFAILED;
      }

      *(uint8 *)pBuff  = nvi.db_ltm_off;

      return SUCCESS;
   }
   
   case CFGI_DAYLT:
   {
      nv_item_type nvi;

      if (nSize != sizeof(boolean)){
         ERR("GetConfig(): invalid parm size", 0, 0, 0);
         return EBADPARM;
      }

      if (OEMNV_Get(NV_DB_DAYLT_I, &nvi) != NV_DONE_S)
      {
         ERR("OEM_GetCachedConfig: OEMNV_Get failed",0,0,0);
         return EFAILED;
      }

      *(boolean *)pBuff  = nvi.db_daylt;

      return SUCCESS;
   }

   //////////////////////////////////////////////////////////////////////
   // Security Menu
   //////////////////////////////////////////////////////////////////////

   case CFGI_LOCKCODE:
      //check to see if we have enough buffer size
      if (nSize != (NV_LOCK_CODE_SIZE+1)) {
         return EBADPARM;
      }

      MEMCPY((byte *) pBuff,
             (byte *) nvi_cache.lockcode,
             NV_LOCK_CODE_SIZE);

      // Ensure the string is NULL terminated
      ((char *)pBuff)[NV_LOCK_CODE_SIZE] = '\0';
      return(0);

   ////////////////////////////////////////////////////////////////
   // Phone Number
   ////////////////////////////////////////////////////////////////
   case CFGI_PHONE_NUMBER:
      {
         char         digits[OEMNV_PHONENUMBER_MAXLEN];

         if (nSize < (int) sizeof(AECHAR) * OEMNV_PHONENUMBER_MAXLEN) {
            return EFAILED;
         }

         nvi.mob_dir_number.nam = (byte) CM_NAM_1;

         if (NV_DONE_S != OEMNV_Get(NV_DIR_NUMBER_PCS_I, &nvi)) {
           return EFAILED;
         }
         if (nSize <= nvi.mob_dir_number.n_digits) {
            return EBADPARM;
         }

         i = 0;
         for (;;) {
            if (i >= MIN((int)sizeof(digits)-1,
                          nvi.mob_dir_number.n_digits)) {
               digits[i] = '\0';
               break;
            }

            if (nvi.mob_dir_number.digitn[i] <= 9) {
               digits[i] = '0' + (char) nvi.mob_dir_number.digitn[i];
            } else if (0x0A == nvi.mob_dir_number.digitn[i]) {
               digits[i] = '0';
            } else {
               // Bad value!
               digits[i] = '\0';
               break;
            }
            i++;
         }

         STR_TO_WSTR(digits,
                     (AECHAR *) pBuff,
                     nSize);
         return SUCCESS;
      }


   ////////////////////////////////////////////////////////////////
   // Display Menu
   ////////////////////////////////////////////////////////////////
   case CFGI_BANNER:
      WSTRNCOPYN(pBuff,
                 nSize / (int) sizeof(AECHAR),
                 nvi_cache.banner,
                 (int) (sizeof(nvi_cache.banner) / sizeof(AECHAR)) );
      return 0;

   ////////////////////////////////////////////////////////////////
   // SMS Applet
   ////////////////////////////////////////////////////////////////

   case CFGI_SMS_ALERT_VOLUME:
   {
#ifdef FEATURE_WMS_APP

     if (OEMNV_Get(NV_ALERTS_LVL_I, &nvi) != NV_DONE_S)
     {
       return EFAILED;
     }
     MEMCPY((byte *)pBuff, (byte *)&nvi.alerts_lvl, 1);
#else
     MEMCPY((byte *)pBuff, (byte *)&oemi_cache.sms_alert_volume, 1);
#endif /* FEATURE_WMS_APP */
     return SUCCESS;
   }

#ifdef FEATURE_SMS_RETRY
   case CFGI_SMS_RETRY_PERIOD:
   {

     if (OEMNV_Get(NV_SMS_MO_RETRY_PERIOD_I, &nvi) != NV_DONE_S) 
     {
       return EFAILED;   
     }
     *(word*)pBuff = nvi.sms_mo_retry_period;
     return SUCCESS;
   }

   case CFGI_SMS_RETRY_INTERVAL:
   {

     if (OEMNV_Get(NV_SMS_MO_RETRY_INTERVAL_I, &nvi) != NV_DONE_S) 
     {
       return EFAILED;   
     }
     *(word*)pBuff = nvi.sms_mo_retry_interval;
     return SUCCESS;
   }
#endif /* FEATURE_SMS_RETRY */

#ifdef FEATURE_GWSMS
#error code not present
#endif /* FEATURE_GWSMS */

   case CFGI_VOICEMAIL_NUMBER:
   {
#ifdef FEATURE_WMS_APP

     if (OEMNV_Get(NV_SMS_VM_NUMBER_I, &nvi) != NV_DONE_S)
     {
       return EFAILED;
     }
     nvi.sms_vm_number.digits[nvi.sms_vm_number.num_digits] = 0;
     STRTOWSTR((char *)nvi.sms_vm_number.digits, pBuff, (nvi.sms_vm_number.num_digits+1) * sizeof(AECHAR));
#else
     WSTRNCOPYN(pBuff,
                nSize / (int) sizeof(AECHAR),
                (void *) oemi_cache.voicemail_number,
                (int) (sizeof(oemi_cache.voicemail_number) / sizeof(AECHAR)));
#endif /* FEATURE_WMS_APP */
     return SUCCESS;
   }

   case CFGI_VOICEMAIL_NUMBER_CDMA:
   {
     WSTRNCOPYN(pBuff,
                nSize / (int) sizeof(AECHAR),
                (void *) oemi_cache.voicemail_number_cdma,
                (int) (sizeof(oemi_cache.voicemail_number_cdma) / sizeof(AECHAR)));
     return SUCCESS;
   }

   case CFGI_VOICEMAIL_NUMBER_GW:
   {
     WSTRNCOPYN(pBuff,
                nSize / (int) sizeof(AECHAR),
                (void *) oemi_cache.voicemail_number_gw,
                (int) (sizeof(oemi_cache.voicemail_number_gw) / sizeof(AECHAR)));
     return SUCCESS;
   }

   ////////////////////////////////////////////////////////////////
   // RecentCalls App
   ////////////////////////////////////////////////////////////////

   // time of the last call
   case CFGI_LASTCALL_TIMER:
   {
     nv_item_type nvItem;

     if(NV_DONE_S == OEMNV_Get(NV_CALL_TIMER_G_I, &nvItem))
     {
       *(dword*)pBuff = (nvItem.call_timer_g.time);
       return SUCCESS;
     }
     else
       return EFAILED;
   }

   // total local (non-roaming) air time used
   case CFGI_AIR_TIMER:
   {
      nv_item_type nvItem;

      nvItem.air_timer.nam = (byte) CM_NAM_1;
      if(NV_DONE_S != OEMNV_Get(NV_AIR_TIMER_I, &nvItem))
      {
        return EFAILED;
      }

      *(dword*)pBuff = nvItem.air_timer.time;
      return SUCCESS;
   }

   // returns the total roaming time used.
   case CFGI_ROAM_TIMER:
   {
      nv_item_type nvItem;

      nvItem.roam_timer.nam = (byte) CM_NAM_1;
      if(NV_DONE_S != OEMNV_Get(NV_ROAM_TIMER_I, &nvItem))
      {
        return EFAILED;
      }

      *(dword*)pBuff = nvItem.roam_timer.time;
      return SUCCESS;
   }

   // returns the total air time used.
   case CFGI_LIFE_TIMER:
   {
      nv_item_type nvItem;

      if(NV_DONE_S == OEMNV_Get(NV_LIFE_TIMER_G_I, &nvItem))
      {
        *(dword*)pBuff = (nvItem.life_timer_g.time);
        return SUCCESS;
      }
      else
        return EFAILED;
   } // Recent Calls App

   ////////////////////////////////////////////////////////////////
   // Service Programming
   ////////////////////////////////////////////////////////////////

   case CFGI_BUILD_VERSION:
      STR_TO_WSTR((char *)ver_dir,
                  (AECHAR *) pBuff,
                  nSize);
      return SUCCESS;
   
#ifdef FEATURE_FACTORY_TESTMODE
   case CFGI_FACTORY_TEST_MODE:
      if (nSize != sizeof(boolean)) {
         ERR("GetConfig(): invalid parm size", 0, 0, 0);
         return EBADPARM;
      }
      return OEMPriv_GetItem_CFGI_FACTORY_TEST_MODE(pBuff);
#endif /*FEATURE_FACTORY_TESTMODE*/

   case CFGI_PRI_CH_A:
   case CFGI_PRI_CH_B:
      {

         if (nSize != (int) sizeof(uint16)) {
            return EBADPARM;
         }

         nvi.pcdmach.nam = (byte) CM_NAM_1;
         if (NV_DONE_S != OEMNV_Get(NV_PCDMACH_I, &nvi)) {
            return EFAILED;
         }

         if (CFGI_PRI_CH_A == i) {
            *(uint16 *) pBuff = nvi.pcdmach.channel_a;
         } else {
            *(uint16 *) pBuff = nvi.pcdmach.channel_b;
         }
         return SUCCESS;
      }

   case CFGI_SEC_CH_A:
   case CFGI_SEC_CH_B:
      {

         if (nSize != (int) sizeof(uint16)) {
            return EBADPARM;
         }

         nvi.scdmach.nam = (byte) CM_NAM_1;
         if (NV_DONE_S != OEMNV_Get(NV_SCDMACH_I, &nvi)) {
            return EFAILED;
         }

         if (CFGI_SEC_CH_A == i) {
            *(uint16 *) pBuff = nvi.scdmach.channel_a;
         } else {
            *(uint16 *) pBuff = nvi.scdmach.channel_b;
         }
         return SUCCESS;
      }

   case CFGI_DATA_DIALSTRING:
      {
         char         code[OEMNV_DATA_DIALSTRING_MAXLEN];

         if (nSize > ((int) sizeof(AECHAR) * OEMNV_DATA_DIALSTRING_MAXLEN)) {
            return EBADPARM;
         }

         if (NV_DONE_S != OEMNV_Get(NV_DATA_PKT_ORIG_STR_I, &nvi)) {
            return EFAILED;
         }

         MEMSET(code, '\0', sizeof(code));
         MEMCPY(code,
                (void *) nvi.data_pkt_orig_str.digits,
                nvi.data_pkt_orig_str.num_digits);

         STR_TO_WSTR(code,
                     (AECHAR *) pBuff,
                     nSize);

         return SUCCESS;
      }

   case CFGI_BUSY_CURSOR_OFFSET:
      {
         AEERect *rc;

         if (nSize != (int) sizeof(*rc)) {
            return EBADPARM;
         }

         // Position the busy icon far off the bottom
         // of the display so it will never be visible
         rc = (AEERect *) pBuff;
         rc->x = 0;
         rc->y = 10000;

         return SUCCESS;
      }

   case CFGI_DOWNLOAD:
      {
         AEEDownloadInfo * pdi = (AEEDownloadInfo *)pBuff;

         if(nSize != sizeof(AEEDownloadInfo))
            return(EBADPARM);

         MEMSET((byte *) pdi, 0, nSize);

         pdi->dwCarrierID  = oemi_cache.download_info.dwCID;
         pdi->dwPlatformID = oemi_cache.download_info.dwPID;
         pdi->nAuth        = oemi_cache.download_info.wAuth;
         pdi->wFlags       = oemi_cache.download_info.wFlags;
         pdi->nPolicy      = (AEEPrivacyPolicy)oemi_cache.download_info.wPrivP;
         STRLCPY((char *)pdi->szServer, 
                (const char *)oemi_cache.download_info.szServer, 
                 sizeof(oemi_cache.download_info.szServer));
         MEMCPY((void *)pdi->bBKey, (void *)oemi_cache.download_info.bBKey, DL_BKEY_SIZE);
         MEMCPY((void *)pdi->bAKey, (void *)oemi_cache.download_info.bAKey, DL_AKEY_SIZE);

         return(SUCCESS);
      }

   case CFGI_BREW_CID:
      {
         uint32 *val = (uint32 *)pBuff;
         if (nSize != (int) sizeof(*val)) {
            return EBADPARM;
         }
         *val = oemi_cache.download_info.dwCID;
         return(SUCCESS);
      }

   case CFGI_BREW_PID:
      {
         uint32 *val = (uint32 *)pBuff;
         if (nSize != (int) sizeof(*val)) {
            return EBADPARM;
         }
         *val = oemi_cache.download_info.dwPID;
         return(SUCCESS);
      }

   case CFGI_BREW_BKEY:
      {
         if (nSize > (int) DL_BKEY_SIZE * sizeof(AECHAR)) {
            return EBADPARM;
         }
         STRTOWSTR((char *)oemi_cache.download_info.bBKey, pBuff, DL_BKEY_SIZE * sizeof(AECHAR));
         return(SUCCESS);
      }

   case CFGI_BREW_AKEY:
      {
         if (nSize > (int) DL_AKEY_SIZE * sizeof(AECHAR)) {
            return EBADPARM;
         }
         STRTOWSTR((char *)oemi_cache.download_info.bAKey, pBuff, DL_AKEY_SIZE * sizeof(AECHAR));
         return(SUCCESS);
      }

   case CFGI_BREW_SERVER:
      {
         if (nSize > (int) DL_MAX_SERVER * sizeof(AECHAR)) {
            return EBADPARM;
         }
         STRTOWSTR((char *)oemi_cache.download_info.szServer, pBuff, DL_MAX_SERVER * sizeof(AECHAR));
         return(SUCCESS);
      }

   case CFGI_BREW_FLAGS:
      {
         uint16 *val = (uint16 *)pBuff;
         if (nSize != (int) sizeof(*val)) {
            return EBADPARM;
         }
         *val = oemi_cache.download_info.wFlags;
         return(SUCCESS);
      }

   case CFGI_BREW_AUTH:
      {
         byte *val = (byte *)pBuff;
         if (nSize != (int) sizeof(*val)) {
            return EBADPARM;
         }
         *val = oemi_cache.download_info.wAuth;
         return(SUCCESS);
      }

   case CFGI_BREW_PRIVP:
      {
         byte *val = (byte *)pBuff;
         if (nSize != (int) sizeof(*val)) {
            return EBADPARM;
         }
         *val = oemi_cache.download_info.wPrivP;
         return(SUCCESS);
      }

   case CFGI_SUBSCRIBERID:
      {
         if(nSize < (STRLEN((const char *)oemi_cache.szSubscriberID) + 1))
            return EBADPARM;

         STRLCPY((char*)pBuff, (const char *)oemi_cache.szSubscriberID, STRLEN((const char *)oemi_cache.szSubscriberID)+1);         

         return AEE_SUCCESS;
      }
#if defined(FEATURE_USRSYSCLOCK) || defined(FEATURE_ISYSCLOCK)
    case CFGI_NITZ_NW_INFO:
      WSTRNCOPYN(pBuff,
                 nSize / (int) sizeof(AECHAR),
                 nvi_cache.plmn_name,
                 (int) (sizeof(nvi_cache.plmn_name) / sizeof(AECHAR)) );
      return (SUCCESS);
   case CFGI_NITZ_NW_INFO_MCC:
      {
         uint32 *val = (uint32 *)pBuff;
         if (nSize != (int) sizeof(*val)) {
            return EBADPARM;
         }
         *val = nvi_cache.mcc;
         return(SUCCESS);
      }
   case CFGI_NITZ_NW_INFO_MNC:
      {
         uint32 *val = (uint32 *)pBuff;
         if (nSize != (int) sizeof(*val)) {
            return EBADPARM;
         }
         *val = nvi_cache.mnc;
         return(SUCCESS);
      }
#endif 

   default:
      return(EUNSUPPORTED);

   }
}


/*==================================================================
Function: OEM_SetCachedConfig

Description: Sets configuration information from the OEM

Prototype:
   int  OEM_SetCachedConfig(AEEConfigItem i, void * pBuff, int nSize)

Parameter(s):

Return Value:  None
Comments:      None
Side Effects:  None
See Also:      None

==================================================================*/
int OEM_SetCachedConfig(AEEConfigItem i, void * pBuff, int nSize)
{
   nv_item_type nvi;
   uint32       j;
#if defined(FEATURE_UIONE_HDK) && ! defined(AEE_SIMULATOR)
   disp_info_type di;
   uint32 contrast;
#endif

   if (!cache_initialized) {
      OEM_InitPreference();
   }

   if (pBuff == NULL) {
      ERR("GetConfig(): invalid parm size", 0, 0, 0);
      return EBADPARM;
   }

   // First check if the item can be found in any of the config
   // tables before resorting to the ugly switch() statement below...
   for (j = 0; j < ARR_SIZE(cfgTable); j++) {
      // Is the item in the range of this table?
      if ( (i >= cfgTable[j].tbl[0].item) &&
           (i <= cfgTable[j].tbl[cfgTable[j].size-1].item) ) {

         int idx = (int)i - (int) cfgTable[j].tbl[0].item;

         // Ensure the config item table is valid
         ASSERT(i == (int) cfgTable[j].tbl[idx].item);

         if (NULL == cfgTable[j].tbl[idx].get) {
            // Exit the for() loop immediately if we found the
            // entry and it had a NULL function.
            break;
         }

         // Perform the size check here so the item functions can
         // assume the size is ok.
         if (nSize != cfgTable[j].tbl[idx].size) {
            ERR("SetConfig(): invalid parm size", 0, 0, 0);
            return EBADPARM;
         }

         return cfgTable[j].tbl[idx].set(pBuff);
      }
   }

   // Didn't find the config item in the tables, maybe it's in this switch...
   switch(i) {
   case CFGI_BACK_LIGHT:
      if (nSize != sizeof(byte)) {
         ERR("GetConfig(): invalid parm size", 0, 0, 0);
         return EBADPARM;
      }
      return OEMPriv_SetItem_CFGI_BACK_LIGHT(pBuff);

   case CFGI_SVC_ALERT:
      if (nSize != sizeof(byte)) {
         ERR("GetConfig(): invalid parm size", 0, 0, 0);
         return EBADPARM;
      }
      return OEMPriv_SetItem_CFGI_SVC_ALERT(pBuff);

   case CFGI_EXTPWR_BK_LIGHT:
      if (nSize != sizeof(byte)) {
         ERR("GetConfig(): invalid parm size", 0, 0, 0);
         return EBADPARM;
      }
      return OEMPriv_SetItem_CFGI_EXTPWR_BK_LIGHT(pBuff);

   case CFGI_CONTRAST_LVL:
      if (nSize != sizeof(byte)) {
         ERR("GetConfig(): invalid parm size", 0, 0, 0);
         return EBADPARM;
      }

#if defined(FEATURE_UIONE_HDK) && ! defined(AEE_SIMULATOR)
      /* For HDK target builds, set the contrast here. */
      di = disp_get_info();

      //linearize the contrast value over the dynamic range.  We'll
      //account for integer arithmetic underflow as well
      contrast = (uint32)((di.contrast_max - di.contrast_min) * 100)
                 / ((uint32)CONTRAST_LVL_MAX - (uint32)CONTRAST_LVL_MIN);

      //interpolate and round up
      contrast = di.contrast_min + ((contrast * (*(byte *)pBuff) + 50)/100);

      disp_set_contrast( (uint16) contrast);
#endif /* FEATURE_UIONE_HDK && ! AEE_SIMULATOR */

      return OEMPriv_SetItem_CFGI_CONTRAST_LVL(pBuff);
   
   case CFGI_VOICEPRIVACY:
      if (nSize != sizeof(byte)) {
         ERR("GetConfig(): invalid parm size", 0, 0, 0);
         return EBADPARM;
      }
      return OEMPriv_SetItem_CFGI_VOICEPRIVACY(pBuff);

#ifdef FEATURE_UIM_RUN_TIME_ENABLE
   case CFGI_RTRE_CONFIG:
      if (nSize != sizeof(byte)) {
         ERR("GetConfig(): invalid parm size", 0, 0, 0);
         return EBADPARM;
      }

      nvi.rtre_config = (nv_rtre_configuration_type) *((byte*)pBuff);
      if (NV_DONE_S != OEMNV_Put(NV_RTRE_CONFIG_I, &nvi)) {
         return EFAILED;
      }
      return SUCCESS;
#endif /* FEATURE_UIM_RUN_TIME_ENABLE */

#ifdef srIMON
#error code not present
#endif

#ifdef AEE_SIMULATOR
   case CFGI_LNG:
      if ( (NULL == pBuff) || (nSize != (int) sizeof(uint32)) ) {
         return EBADPARM;
      }
      nvi.u = *(uint32 *) pBuff;

      if (NV_DONE_S == OEMNV_Put(NV_LANG_I, &nvi)) {
         // Notify the world that the language has changed
#if defined(AEE_SIMULATOR) && defined(AEE_STATIC)
         (void) AEE_IssueSystemCallback(AEE_SCB_DEVICE_INFO_CHANGED);
#endif
         result = SUCCESS;
      }
      return result;

#endif


   //////////////////////////////////////////////////////////////////////
   // Security Menu
   //////////////////////////////////////////////////////////////////////

   case CFGI_LOCKCODE:
      if (nSize != NV_LOCK_CODE_SIZE+1) {
         return EBADPARM;
      }

      if( MEMCMP((byte *) pBuff,
                 (byte *) nvi_cache.lockcode,
                 NV_LOCK_CODE_SIZE) ) {

         MEMCPY((byte *)nvi_cache.lockcode,
                (byte *) pBuff,
                NV_LOCK_CODE_SIZE);

         MEMCPY((byte *)nvi.lock_code.digits,
                (byte *) pBuff,
                NV_LOCK_CODE_SIZE);

         (void) OEMNV_Put( NV_LOCK_CODE_I, &nvi );
      }
      return SUCCESS;


   ////////////////////////////////////////////////////////////////
   // Phone Number
   ////////////////////////////////////////////////////////////////
   case CFGI_PHONE_NUMBER:
      {
         int32    len;
         AECHAR  *wPtr = pBuff;

         len = WSTRLEN(wPtr);

         if (len > NV_DIR_NUMB_PCS_SIZ) {
            return EBADPARM;
         }

         nvi.mob_dir_number.nam = (byte) CM_NAM_1;
         nvi.mob_dir_number.n_digits = (byte) len;

         for (j = 0; j < (uint32) len; j++) {

            if ('0' == wPtr[j]) {
               nvi.mob_dir_number.digitn[j] = 0x0A;
            } else if ( (wPtr[j] >= '1') || (wPtr[j] <= '9')) {
               nvi.mob_dir_number.digitn[j] = (byte) wPtr[j] - '0';
            } else {
               return EBADPARM;
            }
         }

         (void) OEMNV_Put( NV_DIR_NUMBER_PCS_I, &nvi );

         return SUCCESS;
      }

   //////////////////////////////////////////////////////////////////////
   // Display Menu
   //////////////////////////////////////////////////////////////////////
   case CFGI_BANNER:
      if(!pBuff) return EFAILED;

      WSTRNCOPYN(nvi_cache.banner,
                 sizeof(nvi_cache.banner)/sizeof(AECHAR),
                 (AECHAR*) pBuff,
                 -1);

      /* Convert the nv banner data from Unicode to UTF-8 */
      memset((void *)nvi.banner.letters, 0, sizeof(nvi.banner.letters));
      (void) WSTRTOUTF8((AECHAR*) pBuff,
                        WSTRLEN((AECHAR*) pBuff),
                        (byte *)nvi.banner.letters,
                        sizeof(nvi.banner.letters));


      (void) OEMNV_Put( NV_BANNER_I, &nvi );

      return 0;

   ////////////////////////////////////////////////////////////////
   // SMS Applet
   ////////////////////////////////////////////////////////////////
   case CFGI_SMS_ALERT_VOLUME:
   {
     if(!pBuff)
       return EFAILED;
#ifdef FEATURE_WMS_APP
    MEMCPY((byte*)&nvi.alerts_lvl, (byte*)pBuff, 1);

    if (OEMNV_Put(NV_ALERTS_LVL_I, &nvi) != NV_DONE_S)
    {
      return EFAILED;
    }
#else
    MEMCPY((byte*)&oemi_cache.sms_alert_volume, (byte*)pBuff, 1);
    OEMPriv_WriteOEMConfigList();
#endif
    return SUCCESS;
   }

#ifdef FEATURE_SMS_RETRY
   case CFGI_SMS_RETRY_PERIOD:
   {
     if ((!pBuff) || (nSize != (int)sizeof(word)))
     {
       return EBADPARM;
     }
     nvi.sms_mo_retry_period = *(word*)pBuff;
     if (OEMNV_Put(NV_SMS_MO_RETRY_PERIOD_I,&nvi) !=  NV_DONE_S)
     {
       return EFAILED;
     }
     return SUCCESS;
   }

   case CFGI_SMS_RETRY_INTERVAL:
   {
     if ((!pBuff) || (nSize != (int)sizeof(word)))
     {
       return EBADPARM;
     }
     nvi.sms_mo_retry_interval = *(word*)pBuff;
     if (OEMNV_Put(NV_SMS_MO_RETRY_INTERVAL_I,&nvi) !=  NV_DONE_S)
     {
       return EFAILED;
     }
     return SUCCESS;
   }
#endif /* FEATURE_SMS_RETRY */

   case CFGI_VOICEMAIL_NUMBER:
   {
     if(!pBuff)
       return EFAILED;
#ifdef FEATURE_WMS_APP

    nvi.sms_vm_number.num_digits = WSTRLEN(pBuff);
    WSTRTOSTR(pBuff, (char *)nvi.sms_vm_number.digits, nvi.sms_vm_number.num_digits+1);

    if (OEMNV_Put(NV_SMS_VM_NUMBER_I, &nvi) != NV_DONE_S)
    {
      return EFAILED;
    }
#else
    WSTRNCOPYN((void *) oemi_cache.voicemail_number,
                sizeof(oemi_cache.voicemail_number)/sizeof(AECHAR),
                (AECHAR*) pBuff,
                -1);
    OEMPriv_WriteOEMConfigList();
#endif
      return SUCCESS;
   }

   case CFGI_VOICEMAIL_NUMBER_CDMA:
   {
     if(!pBuff)
       return EFAILED;
     WSTRNCOPYN((void *) oemi_cache.voicemail_number_cdma,
                sizeof(oemi_cache.voicemail_number_cdma)/sizeof(AECHAR),
                (AECHAR*) pBuff, -1);
     OEMPriv_WriteOEMConfigList();
     return SUCCESS;
   }

   case CFGI_VOICEMAIL_NUMBER_GW:
   {
     if(!pBuff)
       return EFAILED;
     WSTRNCOPYN((void *) oemi_cache.voicemail_number_gw,
                sizeof(oemi_cache.voicemail_number_gw)/sizeof(AECHAR),
                (AECHAR*) pBuff, -1);
     OEMPriv_WriteOEMConfigList();
     return SUCCESS;
   }

   ////////////////////////////////////////////////////////////////
   // RecentCalls App
   ////////////////////////////////////////////////////////////////
   case CFGI_LASTCALL_TIMER:
     {
      if ((!pBuff) || (nSize != (int)sizeof(dword)))
        return EBADPARM;

      nvi.call_timer_g.time = *(dword*) pBuff;

      if(OEMNV_Put(NV_CALL_TIMER_G_I,&nvi)!= NV_DONE_S)
        return EFAILED;

      return SUCCESS;
     }

   case CFGI_AIR_TIMER:
     {
      if ((!pBuff) || (nSize != (int)sizeof(dword)))
        return EBADPARM;

      nvi.air_timer.nam = CM_NAM_1;
      nvi.air_timer.time = *(dword*) pBuff;

      if(OEMNV_Put(NV_AIR_TIMER_I,&nvi)!= NV_DONE_S)
        return EFAILED;

      return SUCCESS;
     }

   case CFGI_ROAM_TIMER:
     {
      if ((!pBuff) || (nSize != (int)sizeof(dword)))
        return EBADPARM;

      nvi.roam_timer.nam = CM_NAM_1;
      nvi.roam_timer.time = *(dword*) pBuff;

      if(OEMNV_Put(NV_ROAM_TIMER_I,&nvi)!= NV_DONE_S)
        return EFAILED;

      return SUCCESS;
     }

   case CFGI_LIFE_TIMER:
     {
      if ((!pBuff) || (nSize != (int)sizeof(dword)))
        return EBADPARM;

      nvi.life_timer_g.time = *(dword*) pBuff;

      if(OEMNV_Put(NV_LIFE_TIMER_G_I,&nvi)!= NV_DONE_S)
        return EFAILED;

      return SUCCESS;
     }
   ////////////////////////////////////////////////////////////////
   // Service Programming
   ////////////////////////////////////////////////////////////////
   case CFGI_BUILD_VERSION:
      // This item is read-only
      return EBADPARM;

   case CFGI_PRI_CH_A:
   case CFGI_PRI_CH_B:
      {
         if (nSize != (int) sizeof(uint16)) {
            return EBADPARM;
         }

         // Get current values
         nvi.pcdmach.nam = (byte) CM_NAM_1;
         if (NV_DONE_S != OEMNV_Get(NV_PCDMACH_I, &nvi)) {
            return EFAILED;
         }

         // Change either A or B
         nvi.pcdmach.nam = (byte) CM_NAM_1;
         if (CFGI_PRI_CH_A == i) {
            nvi.pcdmach.channel_a = *(uint16 *) pBuff;
         } else {
            nvi.pcdmach.channel_b = *(uint16 *) pBuff;
         }

         // Put it back
         if (NV_DONE_S != OEMNV_Put(NV_PCDMACH_I, &nvi)) {
            return EFAILED;
         }

         return SUCCESS;
      }

   case CFGI_SEC_CH_A:
   case CFGI_SEC_CH_B:
      {
         if (nSize != (int) sizeof(uint16)) {
            return EBADPARM;
         }

         // Get current values
         nvi.scdmach.nam = (byte) CM_NAM_1;
         if (NV_DONE_S != OEMNV_Get(NV_SCDMACH_I, &nvi)) {
            return EFAILED;
         }

         // Change either A or B
         nvi.scdmach.nam = (byte) CM_NAM_1;
         if (CFGI_SEC_CH_A == i) {
            nvi.scdmach.channel_a = *(uint16 *) pBuff;
         } else {
            nvi.scdmach.channel_b = *(uint16 *) pBuff;
         }

         // Put it back
         if (NV_DONE_S != OEMNV_Put(NV_SCDMACH_I, &nvi)) {
            return EFAILED;
         }

         return SUCCESS;
      }

   case CFGI_DATA_DIALSTRING:
      {
         char code[OEMNV_DATA_DIALSTRING_MAXLEN];

         if (nSize > ((int) sizeof(AECHAR) * OEMNV_DATA_DIALSTRING_MAXLEN)) {
            return EBADPARM;
         }

         WSTR_TO_STR((AECHAR *) pBuff,
                     code,
                     sizeof(code));

         // Length includes the NULL character
         nvi.data_pkt_orig_str.num_digits = (uint8) STRLEN(code) + 1;

         MEMCPY((void *) nvi.data_pkt_orig_str.digits,
                code,
                sizeof(nvi.data_pkt_orig_str.digits));

         if (NV_DONE_S != OEMNV_Put(NV_DATA_PKT_ORIG_STR_I, &nvi)) {
            return EFAILED;
         }
         return SUCCESS;
      }
 /*
   case CFGI_DATA_NETWORK:
      // This config item was added in BREW v2.0 but we don't support
      // it yet...
      ERR("CFGI_DATA_NETWORK not implemented", 0, 0, 0);
      return EUNSUPPORTED;
*/
   case CFGI_DOWNLOAD:
     {
        AEEDownloadInfo * pdi = (AEEDownloadInfo *)pBuff;

      if(!pBuff || nSize != sizeof(AEEDownloadInfo))
         return(EBADPARM);

      MEMSET((void *)&oemi_cache.download_info, 0, sizeof(OEMConfigDownloadInfo));
        oemi_cache.download_info.dwCID  = pdi->dwCarrierID;
        oemi_cache.download_info.dwPID  = pdi->dwPlatformID;
      oemi_cache.download_info.wAuth  = pdi->nAuth;
        oemi_cache.download_info.wFlags = pdi->wFlags;
      oemi_cache.download_info.wPrivP = pdi->nPolicy;
      STRLCPY((char *)oemi_cache.download_info.szServer, 
              (const char *)pdi->szServer, 
              sizeof(oemi_cache.download_info.szServer));
      MEMCPY((void *)oemi_cache.download_info.bBKey, (void *)pdi->bBKey, DL_BKEY_SIZE);
      MEMCPY((void *)oemi_cache.download_info.bAKey, (void *)pdi->bAKey, DL_AKEY_SIZE);
      OEMPriv_WriteOEMConfigList();
      return SUCCESS;
     }

   case CFGI_BREW_CID:
      {
         uint32 *val = (uint32 *)pBuff;
         if (!pBuff || nSize != (int) sizeof(*val)) {
            return EBADPARM;
         }
         oemi_cache.download_info.dwCID  = *val;
         OEMPriv_WriteOEMConfigList();
         return SUCCESS;
      }

   case CFGI_BREW_PID:
      {
         uint32 *val = (uint32 *)pBuff;
         if (!pBuff || nSize != (int) sizeof(*val)) {
            return EBADPARM;
         }
         oemi_cache.download_info.dwPID  = *val;
         OEMPriv_WriteOEMConfigList();
         return SUCCESS;
      }

   case CFGI_BREW_BKEY:
      {
         if (!pBuff || nSize > (int) DL_BKEY_SIZE * sizeof(AECHAR)) {
            return EBADPARM;
         }
         MEMSET((void*)oemi_cache.download_info.bBKey, 0, sizeof(oemi_cache.download_info.bBKey));
         WSTRTOSTR(pBuff, (char *)oemi_cache.download_info.bBKey, DL_BKEY_SIZE);
         OEMPriv_WriteOEMConfigList();
         return SUCCESS;
      }

   case CFGI_BREW_AKEY:
      {
         if (!pBuff || nSize > (int) DL_AKEY_SIZE * sizeof(AECHAR)) {
            return EBADPARM;
         }
         MEMSET((void*)oemi_cache.download_info.bAKey, 0, sizeof(oemi_cache.download_info.bAKey));
         WSTRTOSTR(pBuff, (char *)oemi_cache.download_info.bAKey, DL_AKEY_SIZE);
         OEMPriv_WriteOEMConfigList();
         return SUCCESS;
      }

   case CFGI_BREW_SERVER:
      {
         if (!pBuff || nSize > (int) DL_MAX_SERVER * sizeof(AECHAR)) {
            return EBADPARM;
         }
         MEMSET((void*)oemi_cache.download_info.szServer, 0, sizeof(oemi_cache.download_info.szServer));
         WSTRTOSTR(pBuff, (char *)oemi_cache.download_info.szServer, DL_MAX_SERVER);
         OEMPriv_WriteOEMConfigList();
         return SUCCESS;
      }

   case CFGI_BREW_FLAGS:
      {
         uint16 *val = (uint16 *)pBuff;
         if (!pBuff || nSize != (int) sizeof(*val)) {
            return EBADPARM;
         }
         oemi_cache.download_info.wFlags = *val;
         OEMPriv_WriteOEMConfigList();
         return SUCCESS;
      }

   case CFGI_BREW_AUTH:
      {
         byte *val = (byte *)pBuff;
         if (!pBuff || nSize != (int) sizeof(*val)) {
            return EBADPARM;
         }
         oemi_cache.download_info.wAuth  = *val;
         OEMPriv_WriteOEMConfigList();
         return SUCCESS;
      }

   case CFGI_BREW_PRIVP:
      {
         byte *val = (byte *)pBuff;
         if (!pBuff || nSize != (int) sizeof(*val)) {
            return EBADPARM;
         }
         oemi_cache.download_info.wPrivP = *val;
         OEMPriv_WriteOEMConfigList();
         return SUCCESS;
      }

   case CFGI_SUBSCRIBERID:
      {
         if(!pBuff || nSize >= DEFAULT_SUBSCRIBERID_LEN)
            return EBADPARM;

         STRLCPY((char *)oemi_cache.szSubscriberID, (char*)pBuff, DEFAULT_SUBSCRIBERID_LEN);
         OEMPriv_WriteOEMConfigList();
         return AEE_SUCCESS;
      }

#if defined(FEATURE_USRSYSCLOCK) || defined(FEATURE_ISYSCLOCK)
  case CFGI_NITZ_NW_INFO:
      if(!pBuff) return EFAILED;

      if (0 == WSTRNCMP(nvi_cache.plmn_name, (AECHAR*) pBuff, 
                        sizeof(nvi_cache.plmn_name)/sizeof(AECHAR)))
      {
        //Don't need to update if no change has occured.
        return SUCCESS;
      }

      WSTRNCOPYN(nvi_cache.plmn_name,
                 sizeof(nvi_cache.plmn_name)/sizeof(AECHAR),
                 (AECHAR*) pBuff,
                 -1);

      /* Convert the nv banner data from Unicode to UTF-8 */
      memset((void *)nvi.nitz_nw_info.plmn_name, 0, sizeof(nvi.nitz_nw_info.plmn_name));
      (void) WSTRTOUTF8((AECHAR*) pBuff,
                        WSTRLEN((AECHAR*) pBuff),
                        (byte *)nvi.nitz_nw_info.plmn_name,
                        sizeof(nvi.nitz_nw_info.plmn_name));


      (void) OEMNV_Put( NV_NITZ_NW_INFO_I, &nvi );

      return SUCCESS;
   case CFGI_NITZ_NW_INFO_MCC:
      {
         uint32 *val = (uint32 *)pBuff;
         if (!pBuff || nSize != (int) sizeof(*val)) {
            return EBADPARM;
         }

         if (nvi_cache.mcc == *val)
         {
           //Don't need to update if no change has occured.
           return SUCCESS;
         }
      
         nvi.nitz_nw_info_mcc = nvi_cache.mcc = *val;
         (void) OEMNV_Put( NV_NITZ_NW_INFO_MCC_I, &nvi );
         return SUCCESS;
      }
   case CFGI_NITZ_NW_INFO_MNC:
      {
         uint32 *val = (uint32 *)pBuff;
         if (!pBuff || nSize != (int) sizeof(*val)) {
            return EBADPARM;
         }

         if (nvi_cache.mnc == *val)
         {
           //Don't need to update if no change has occured.
           return SUCCESS;
         }

         nvi.nitz_nw_info_mnc = nvi_cache.mnc = *val;
         (void) OEMNV_Put( NV_NITZ_NW_INFO_MNC_I, &nvi );
         return SUCCESS;
      }
#endif

   case CFGI_AUTO_ANSWER:
      {
         nv_item_type nvi;

         if ( (NULL == pBuff) || (nSize != (int) sizeof(boolean)) ) {
            return EBADPARM;
         }
         nvi.auto_answer.enable = *(boolean *) pBuff;
         if (NV_DONE_S != OEMNV_Put(NV_AUTO_ANSWER_I, &nvi)) {
            return EFAILED;  
         }
         return SUCCESS;
      }
      
   case CFGI_TTY:
      {
         nv_item_type nvi;

         if ( (NULL == pBuff) || (nSize != (int) sizeof(uint32)) ) {
            return EBADPARM;
         }
         nvi.tty = *(uint32 *) pBuff;
         if (NV_DONE_S != OEMNV_Put(NV_TTY_I, &nvi)) {
            return EFAILED;
         }
         return SUCCESS;
      }
      
   case CFGI_LP_SEC:
      {
         nv_item_type nvi;

         if ( (NULL == pBuff) || (nSize != (int) sizeof(uint8)) ) {
            return EBADPARM;
         }
         nvi.db_lp_sec = *(uint8 *) pBuff;
         if (NV_DONE_S != OEMNV_Put(NV_DB_LP_SEC_I, &nvi)) {
            return EFAILED;
         }
         return SUCCESS;
      }
   
   case CFGI_LTM_OFF:
      {
         nv_item_type nvi;

         if ( (NULL == pBuff) || (nSize != (int) sizeof(uint8)) ) {
            return EBADPARM;
         }
         nvi.db_ltm_off = *(uint8 *) pBuff;
         if (NV_DONE_S != OEMNV_Put(NV_DB_LTM_OFF_I, &nvi)) {
            return EFAILED;
         }
         return SUCCESS;
      }
    
   case CFGI_DAYLT:
      {
         nv_item_type nvi;

         if ( (NULL == pBuff) || (nSize != (int) sizeof(boolean)) ) {
            return EBADPARM;
         }
         nvi.db_daylt = *(boolean *) pBuff;
         if (NV_DONE_S != OEMNV_Put(NV_DB_DAYLT_I, &nvi)) {
            return EFAILED;
         }
         return SUCCESS;
      }    

      
   default:
      return(EUNSUPPORTED);
   }
}

/*==================================================================
Function: OEM_GetPlatformID

Description: Retrieves Platform ID from OEM (required by
             OEM_GetDeviceInfo.

Prototype:
   void  OEM_SetCachedConfig(uint32 *pPlatformID)

Parameter(s):

Return Value:  EBADPARM or SUCCESS
Comments:      None
Side Effects:  None
See Also:      None

==================================================================*/
int OEM_GetPlatformID(uint32 *pPlatformID)
{
   if(!pPlatformID)
      return(EBADPARM);

   *pPlatformID = oemi_cache.download_info.dwPID;
   return SUCCESS;
}

/*=============================================================================
FUNCTION: OEMPriv_WriteOEMConfigListCB

DESCRIPTION: Callback that actually writes the OEM config list to EFS

PARAMETERS:
   *pUnused: Unused

RETURN VALUE:
   None

COMMENTS:

SIDE EFFECTS:

SEE ALSO:

=============================================================================*/
static void OEMPriv_WriteOEMConfigListCB(void *pUnused)
{
   uint16  version = OEMCONFIGLIST_VERSION;
   int  nErr = EFAILED;
   int  nFileNameLen = 0;
   char *pszBREWPath = OEMCONFIGLIST_FILE;  /* Path using BREW convention */
   char *pszFSPath = NULL;    /* Path with full EFS convention */
   int  fd = -1;

#if MIN_BREW_VERSION(4,0)
   nErr = OEMefs_GetNativePath(pszBREWPath, NULL, NULL, &nFileNameLen);
#else
   nErr = OEMFS_GetNativePath(pszBREWPath, NULL, &nFileNameLen);
#endif //MIN_BREW_VERSION(4,0)

   if ((nErr != SUCCESS) || (! nFileNameLen))
   {
      ERR("Unable to resolve OEM config file path", 0, 0, 0);
      return;
   }
        
   pszFSPath = (char *)MALLOC((uint32)nFileNameLen);
   if (! pszFSPath)
   {
      ERR("Unable to create OEM config file", 0, 0, 0);
      return;
   }
        
#if MIN_BREW_VERSION(4,0)
   nErr = OEMefs_GetNativePath(pszBREWPath, pszFSPath, nFileNameLen, &nFileNameLen);
#else
   nErr = OEMFS_GetNativePath(pszBREWPath, pszFSPath, &nFileNameLen);
#endif //MIN_BREW_VERSION(4,0)
   if ((nErr != SUCCESS))
   {
     ERR("Unable to resolve OEM config file path", 0, 0, 0);
     FREEIF(pszFSPath);
     return;
   }
   
   fd = efs_open(pszFSPath, O_RDWR);
   if (fd < 0) {
      fd = efs_open(pszFSPath, O_CREAT);
      if (fd < 0) {
         ERR("Unable to create OEM config file", 0, 0, 0);
         FREEIF(pszFSPath);
         return;
      }
   }
   FREEIF(pszFSPath);

   (void) efs_lseek(fd, 0, SEEK_SET);
   (void) efs_write(fd, &version, sizeof(version));
   (void) efs_write(fd, (void *) &oemi_cache, sizeof(oemi_cache));
   (void) efs_close(fd);
}

/*=============================================================================
FUNCTION: OEMPriv_WriteOEMConfigList

DESCRIPTION: Saves the OEM configuration list to EFS

PARAMETERS:
   None

RETURN VALUE:
   None

COMMENTS:

SIDE EFFECTS:

SEE ALSO:

=============================================================================*/
static void OEMPriv_WriteOEMConfigList(void)
{
   static boolean cbInitialized = FALSE;

   if (!cbInitialized) {
      CALLBACK_Init(&gCBWriteOEMConfig,
                    OEMPriv_WriteOEMConfigListCB,
                    NULL);
      cbInitialized = TRUE;
   }

   // Queue an AEE Callback instead of immediately writing the config
   // data out to EFS.  This prevents unnecessary EFS writes if the caller
   // is making numerious sequential calls to ICONFIG_SetItem()
#ifdef FEATURE_BREW_3_0
   AEE_APP_RESUME(&gCBWriteOEMConfig, NULL);
#else
   AEE_ResumeCallback(&gCBWriteOEMConfig,
                      AEE_RESUME_CB_SYS|AEE_RESUME_CB_PRIO);
#endif
}


/*=============================================================================
FUNCTION: OEMPriv_ReadOEMConfigList

DESCRIPTION: Reads the OEM configuration list from EFS

PARAMETERS:
   None

RETURN VALUE:
   None

COMMENTS:

SIDE EFFECTS:

SEE ALSO:

=============================================================================*/
static void OEMPriv_ReadOEMConfigList(void)
{
   uint16  version;
   int  nErr = EFAILED;
   int  nFileNameLen = 0;
   char *pszBREWPath = OEMCONFIGLIST_FILE;  /* Path using BREW convention */
   char *pszFSPath = NULL;    /* Path with full EFS convention */
   int  fd = -1;

#if MIN_BREW_VERSION(4,0)
   nErr = OEMefs_GetNativePath(pszBREWPath, NULL, NULL, &nFileNameLen);
#else
   nErr = OEMFS_GetNativePath(pszBREWPath, NULL, &nFileNameLen);
#endif //MIN_BREW_VERSION(4,0)

   if ((nErr != SUCCESS) || (! nFileNameLen))
   {
      ERR("Unable to resolve OEM config file path", 0, 0, 0);
      return;
   }
        
   pszFSPath = (char *)MALLOC((uint32)nFileNameLen);
   if (! pszFSPath)
   {
      ERR("Unable to create OEM config file", 0, 0, 0);
      return;
   }
        
#if MIN_BREW_VERSION(4,0)
   nErr = OEMefs_GetNativePath(pszBREWPath, pszFSPath, nFileNameLen, &nFileNameLen);
#else
   nErr = OEMFS_GetNativePath(pszBREWPath, pszFSPath, &nFileNameLen);
#endif //MIN_BREW_VERSION(4,0)
   if ((nErr != SUCCESS))
   {
     ERR("Unable to resolve OEM config file path", 0, 0, 0);
     FREEIF(pszFSPath);
     return;
   }

   fd = efs_open(pszFSPath, O_RDONLY);
   FREEIF(pszFSPath);
   if (fd < 0) {
      // Config file does not exist yet.  No big deal, just
      // use the defaults
      return;
   }

   (void) efs_lseek(fd, 0, SEEK_SET);
   (void) efs_read(fd, &version, sizeof(version));
   if (version != OEMCONFIGLIST_VERSION) {
      // Different version.  Ignore it and use the defaults
      (void) efs_close(fd);
      return;
   }
   (void) efs_read(fd, (void *) &oemi_cache, sizeof(oemi_cache));
   (void) efs_close(fd);

   //
   // Verify the config file data is valid
   //
   if (oemi_cache.alert_type > OEMNV_ALERTTYPE_VIBRINGER) {
      oemi_cache.alert_type = OEMNV_ALERTTYPE_RINGER;
   }

   if (oemi_cache.silence_all > OEMNV_SA_LITONLY) {
      oemi_cache.silence_all = OEMNV_SA_NORMAL;
   }

   if (oemi_cache.sms_service_option > OEMNV_SMS_SO_SO14) {
      oemi_cache.sms_service_option = OEMNV_SMS_SO_SO6;
   }

   if (oemi_cache.sms_is41_workaround > OEMNV_SMS_EMAIL_ADDRESSING_STANDARD) {
      oemi_cache.sms_is41_workaround = OEMNV_SMS_EMAIL_ADDRESSING_IS41;
   }

   if (oemi_cache.sms_mo_encoding > OEMNV_SMS_MO_ENCODING_UNICODE) {
      oemi_cache.sms_mo_encoding = OEMNV_SMS_MO_ENCODING_7BIT;
   }

   if (oemi_cache.sms_mode > OEMNV_SMS_MODE_GW) {
      oemi_cache.sms_mode = OEMNV_SMS_MODE_CDMA;
   }

   if (oemi_cache.sms_app > OEMNV_SMS_APP_TEST) {
      oemi_cache.sms_mode = OEMNV_SMS_APP_TEST;
   }

   if (oemi_cache.sms_cb_auto_delete > OEMNV_SMS_CB_ENABLE_AUTO_DELETE) {
      oemi_cache.sms_cb_auto_delete = OEMNV_SMS_CB_DISABLE_AUTO_DELETE;
   }

   if (oemi_cache.sms_cb_silent_alert > OEMNV_SMS_CB_ENABLE_SILENT_ALERT) {
      oemi_cache.sms_cb_silent_alert = OEMNV_SMS_CB_DISABLE_SILENT_ALERT;
   }

   if (oemi_cache.sms_cb_lang_filter > OEMNV_SMS_CB_ENABLE_LANG_FILTER) {
      oemi_cache.sms_cb_lang_filter = OEMNV_SMS_CB_DISABLE_LANG_FILTER;
   }

   if (oemi_cache.sms_alert_display > OEMNV_SMS_ALERT_DISPLAY_ENABLE) {
      oemi_cache.sms_alert_display = OEMNV_SMS_ALERT_DISPLAY_ENABLE;
   }

   if (oemi_cache.sms_alert_sound > OEMNV_SMS_ALERT_SOUND_ENABLE) {
      oemi_cache.sms_alert_sound = OEMNV_SMS_ALERT_SOUND_ENABLE;
   }

   if (oemi_cache.sms_alert_volume > OEMNV_SMS_ALERT_VOLUME_4) {
      oemi_cache.sms_alert_volume = OEMNV_SMS_ALERT_VOLUME_2;
   }

   if (oemi_cache.time_format > OEMNV_TIMEFORM_24HR) {
      oemi_cache.time_format = OEMNV_TIMEFORM_AMPM;
   }

   oemi_cache.device_mask = 0;

   MEMSET((void *)&oemi_cache.cug, 0, sizeof(OEMConfigCugInfo));
   if (STRCMP((const char *)oemi_cache.download_info.szServer, "") == 0)
   {
     // Download information not initialized
     oemi_cache.download_info.dwCID  = OEMCFG_DEFAULT_CID;
     oemi_cache.download_info.dwPID  = OEMCFG_DEFAULT_PLATFORM_ID;
     oemi_cache.download_info.wFlags = DIF_TEST_ALLOWED | DIF_MIN_FOR_SID;
     oemi_cache.download_info.wAuth  = APOLICY_NONE;
     oemi_cache.download_info.wPrivP = PPOLICY_BREW_OR_CARRIER;
     STRLCPY((char *)oemi_cache.download_info.szServer, 
             (const char *)OEMCFG_DEFAULT_DL_SERVER, 
             sizeof(oemi_cache.download_info.szServer));
   }

   // Force the value on the following members
   oemi_cache.disable_in_call_disp = FALSE;
   oemi_cache.disable_bg_image = FALSE;
   oemi_cache.manual_plmn_sel_allowed = TRUE;
}


/*===========================================================================

                     CONFIG ITEM GET/SET FUNCTION DEFINITIONS

===========================================================================*/


/*=============================================================================
FUNCTION: OEMPriv_GetItem_CFGI_SILENCEALL

DESCRIPTION:  Gets the current value of the CFGI_SILENCEALL config item.

PARAMETERS:
   *pBuff: Pointer to a buffer to store the config value

RETURN VALUE:
   int:  SUCCESS if successful, otherwise a BREW error code.

COMMENTS:
   Note that pBuff may be assumed to point to a correctly sized buffer for
   this config item.  OEM_GetConfig() peforms a size check before invoking
   this function.

SIDE EFFECTS:

SEE ALSO:

=============================================================================*/
static int OEMPriv_GetItem_CFGI_SILENCEALL(void *pBuff)
{
   *(byte *) pBuff = oemi_cache.silence_all;
   return SUCCESS;
}

/*=============================================================================
FUNCTION: OEMPriv_SetItem_CFGI_SILENCEALL

DESCRIPTION:  Sets the CFGI_SILENCEALL config item.

PARAMETERS:
   *pBuff: Pointer to the new config value

RETURN VALUE:
   int:  SUCCESS if successful, otherwise a BREW error code.

COMMENTS:
   Note that pBuff may be assumed to point to a correctly sized buffer for
   this config item.  OEM_SetConfig() peforms a size check before invoking
   this function.

SIDE EFFECTS:

SEE ALSO:

=============================================================================*/
static int OEMPriv_SetItem_CFGI_SILENCEALL(void *pBuff)
{
   if (oemi_cache.silence_all != *(byte *)pBuff) {
      oemi_cache.silence_all = *(byte *)pBuff;
      OEMPriv_WriteOEMConfigList();
   }

   return SUCCESS;
}

static int OEMPriv_GetItem_CFGI_RECENT_MT_CALL_TIMER(void *pBuff)
{
   *(dword *) pBuff = oemi_cache.recent_mt_call_time;
   return SUCCESS;
}


static int OEMPriv_SetItem_CFGI_RECENT_MT_CALL_TIMER(void *pBuff)
{
   if (oemi_cache.recent_mt_call_time != *(dword *)pBuff) {
      oemi_cache.recent_mt_call_time = *(dword *)pBuff;
      OEMPriv_WriteOEMConfigList();
   }

   return SUCCESS;
}


static int OEMPriv_GetItem_CFGI_RECENT_MO_CALL_TIMER(void *pBuff)
{
   *(dword *) pBuff = oemi_cache.recent_mo_call_time;
   return SUCCESS;
}


static int OEMPriv_SetItem_CFGI_RECENT_MO_CALL_TIMER(void *pBuff)
{
   if (oemi_cache.recent_mo_call_time != *(dword *)pBuff) {
      oemi_cache.recent_mo_call_time = *(dword *)pBuff;
      OEMPriv_WriteOEMConfigList();
   }

   return SUCCESS;
}


static int OEMPriv_GetItem_CFGI_ALL_CALL_TIMER(void *pBuff)
{
   *(dword *) pBuff = nvi_cache.all_call_time;
   return SUCCESS;
}

static int OEMPriv_SetItem_CFGI_ALL_CALL_TIMER(void *pBuff)
{
   nv_item_type nvi;

   if (nvi_cache.all_call_time != *((dword *)pBuff)) {
      nvi_cache.all_call_time = *((dword *)pBuff);
      nvi.life_timer_g.time = *((dword *)pBuff);
      (void) OEMNV_Put( NV_LIFE_TIMER_G_I, &nvi);
   }

   return SUCCESS;
}

static int OEMPriv_GetItem_CFGI_BROWSER_CALL_TIMER(void *pBuff)
{
   *(dword *) pBuff = oemi_cache.internet_call_time;
   return SUCCESS;
}

static int OEMPriv_SetItem_CFGI_BROWSER_CALL_TIMER(void *pBuff)
{
   if (oemi_cache.internet_call_time != *(dword *)pBuff) {
      oemi_cache.internet_call_time = *((dword *)pBuff);
      OEMPriv_WriteOEMConfigList();
   }

   return SUCCESS;
}

static int OEMPriv_GetItem_CFGI_EXTPWR_BK_LIGHT(void *pBuff)
{
   *(byte *) pBuff = nvi_cache.backlight_hfk;
   return SUCCESS;
}

static int OEMPriv_SetItem_CFGI_EXTPWR_BK_LIGHT(void *pBuff)
{
   nv_item_type nvi;

   if (nvi_cache.backlight_hfk != *((byte *)pBuff)) {
      nvi_cache.backlight_hfk = *((byte *)pBuff);
      nvi.back_light_hfk = *((byte *)pBuff);
      (void) OEMNV_Put( NV_BACK_LIGHT_HFK_I, &nvi );
   }

   return SUCCESS;
}

static int OEMPriv_GetItem_CFGI_HEADSET_RNG(void *pBuff)
{
   *(byte *)pBuff = oemi_cache.headset_ringer;
   return SUCCESS;
}

static int OEMPriv_SetItem_CFGI_HEADSET_RNG(void *pBuff)
{
   if (oemi_cache.headset_ringer != *(byte *)pBuff) {
      oemi_cache.headset_ringer = *((byte *)pBuff);
      OEMPriv_WriteOEMConfigList();
   }

   return SUCCESS;
}

static int OEMPriv_GetItem_CFGI_ANYKEY_ANSWER(void *pBuff)
{
   *(boolean *)pBuff = oemi_cache.any_key_answer;
   return SUCCESS;
}

static int OEMPriv_SetItem_CFGI_ANYKEY_ANSWER(void *pBuff)
{
   if (oemi_cache.any_key_answer != *(boolean *)pBuff) {
      oemi_cache.any_key_answer = *((boolean *)pBuff);
      OEMPriv_WriteOEMConfigList();
   }

   return SUCCESS;
}

static int OEMPriv_GetItem_CFGI_HEADSET_AUTOANSWER(void *pBuff)
{
   *(byte *)pBuff = oemi_cache.headset_autoanswer;
   return SUCCESS;
}

static int OEMPriv_SetItem_CFGI_HEADSET_AUTOANSWER(void *pBuff)
{
   if (oemi_cache.headset_autoanswer != *(byte *)pBuff) {
      oemi_cache.headset_autoanswer = *((byte *)pBuff);
      OEMPriv_WriteOEMConfigList();
   }

   return SUCCESS;
}


static int OEMPriv_GetItem_CFGI_MISSED_CALL_ALERT(void *pBuff)
{
   *(byte *)pBuff = oemi_cache.missed_call_alert;
   return SUCCESS;
}

static int OEMPriv_SetItem_CFGI_MISSED_CALL_ALERT(void *pBuff)
{
   if (oemi_cache.missed_call_alert != *(byte *)pBuff) {
      oemi_cache.missed_call_alert = *(byte *)pBuff;
      OEMPriv_WriteOEMConfigList();
   }

   return SUCCESS;
}

static int OEMPriv_GetItem_CFGI_ROAMING_ALERT(void *pBuff)
{
   *(byte *)pBuff = oemi_cache.roaming_alert;
   return SUCCESS;
}

static int OEMPriv_SetItem_CFGI_ROAMING_ALERT(void *pBuff)
{
   if (oemi_cache.roaming_alert != *(byte *)pBuff) {
      oemi_cache.roaming_alert = *((byte *)pBuff);
      OEMPriv_WriteOEMConfigList();
   }

   return SUCCESS;
}

static int OEMPriv_GetItem_CFGI_SVC_ALERT(void *pBuff)
{
   *((byte *)pBuff) = nvi_cache.svc_alert;
   return SUCCESS;
}

static int OEMPriv_SetItem_CFGI_SVC_ALERT(void *pBuff)
{
   byte b = *((byte *)pBuff) ? 1: 0;
   nv_item_type nvi;

   if (nvi_cache.svc_alert != b) {
      nvi_cache.svc_alert = b;
      nvi.svc_area_alert = b;
      (void) OEMNV_Put( NV_SVC_AREA_ALERT_I, &nvi );
   }
   return SUCCESS;
}

static int OEMPriv_GetItem_CFGI_INTERNET_ALERT(void *pBuff)
{
   *(byte *)pBuff = oemi_cache.internet_call_alert;
   return SUCCESS;
}

static int OEMPriv_SetItem_CFGI_INTERNET_ALERT(void *pBuff)
{
   if (oemi_cache.internet_call_alert != *(byte *)pBuff) {
      oemi_cache.internet_call_alert = *((byte *)pBuff);
      OEMPriv_WriteOEMConfigList();
   }

   return SUCCESS;
}


static int OEMPriv_GetItem_CFGI_MINUTE_ALERT(void *pBuff)
{
   *(boolean *) pBuff = oemi_cache.minute_alert;
   return SUCCESS;
}

static int OEMPriv_SetItem_CFGI_MINUTE_ALERT(void *pBuff)
{
   if (oemi_cache.minute_alert != *(boolean *)pBuff) {
      oemi_cache.minute_alert = *(boolean *)pBuff;
      OEMPriv_WriteOEMConfigList();
   }

   return SUCCESS;
}

static int OEMPriv_GetItem_CFGI_GAME_MODE_PERSIST(void *pBuff)
{
  *(boolean *) pBuff = nvi_cache.game_mode_persist;
  return SUCCESS;
}

//lint -save -esym(818, pBuff)  Suppress "could be declared as const pointer"
static int OEMPriv_SetItem_CFGI_GAME_MODE_PERSIST(void *pBuff)
{
  nv_item_type nvi;

  if (nvi_cache.game_mode_persist != *((boolean *)pBuff)) {
      nvi_cache.game_mode_persist = *((boolean *)pBuff);
      nvi.game_mode_persist = nvi_cache.game_mode_persist;
     (void) OEMNV_Put( NV_GAME_MODE_PERSIST_I, &nvi);
  }
  return SUCCESS;
}
//lint -restore

static int OEMPriv_GetItem_CFGI_GAME_MODE(void *pBuff)
{
  *(boolean *) pBuff = nvi_cache.game_mode;
  return SUCCESS;
}

//lint -save -esym(818, pBuff)  Suppress "could be declared as const pointer"
static int OEMPriv_SetItem_CFGI_GAME_MODE(void *pBuff)
{
  nv_item_type nvi;

  if (nvi_cache.game_mode != *((boolean *)pBuff)) {
      nvi_cache.game_mode = *((boolean *)pBuff);
      nvi.game_mode = nvi_cache.game_mode;
     (void) OEMNV_Put( NV_GAME_MODE_I, &nvi);
  }
  return SUCCESS;
}
//lint -restore

static int OEMPriv_GetItem_CFGI_RINGER_VOL(void *pBuff)
{
   *(byte *) pBuff = (byte)nvi_cache.ringer_level;
   return SUCCESS;
}

static int OEMPriv_SetItem_CFGI_RINGER_VOL(void *pBuff)
{
   nv_item_type nvi;

   if (nvi_cache.ringer_level != *((byte *)pBuff)) {
      nvi_cache.ringer_level = *((byte *)pBuff);
      nvi.ringer_level = *((byte *)pBuff);
      (void) OEMNV_Put( NV_RINGER_LVL_I, &nvi);
   }

   return SUCCESS;
}

static int OEMPriv_GetItem_CFGI_EAR_VOL(void *pBuff)
{
   *(byte *) pBuff = (byte)nvi_cache.handset_ear_level;
   return SUCCESS;
}

static int OEMPriv_SetItem_CFGI_EAR_VOL(void *pBuff)
{
   nv_item_type nvi;

   if (nvi_cache.handset_ear_level != *((byte *)pBuff)) {
      nvi_cache.handset_ear_level = *((byte *)pBuff);
      nvi.ear_level = *((byte *)pBuff);
      (void) OEMNV_Put( NV_EAR_LVL_I, &nvi);
   }

   return SUCCESS;
}

static int OEMPriv_GetItem_CFGI_BEEP_VOL(void *pBuff)
{
   *(byte *) pBuff = (byte)nvi_cache.beep_level;
   return SUCCESS;
}

static int OEMPriv_SetItem_CFGI_BEEP_VOL(void *pBuff)
{
   nv_item_type nvi;

   if (nvi_cache.beep_level != *((byte *)pBuff)) {
      nvi_cache.beep_level = *((byte *)pBuff);
      nvi.beep_level = *((byte *)pBuff);
      (void) OEMNV_Put( NV_BEEP_LVL_I, &nvi);
   }

   return SUCCESS;
}

static int OEMPriv_GetItem_CFGI_ALERT_TYPE(void *pBuff)
{
   *(byte *)pBuff = oemi_cache.alert_type;
   return SUCCESS;
}

static int OEMPriv_SetItem_CFGI_ALERT_TYPE(void *pBuff)
{
   if (oemi_cache.alert_type != *((byte *)pBuff)) {
      oemi_cache.alert_type = *((byte *)pBuff);
      OEMPriv_WriteOEMConfigList();
   }

   return SUCCESS;
}

#ifdef FEATURE_SMART_SOUND
static int OEMPriv_GetItem_CFGI_SMART_SOUND(void *pBuff)
{
   *(boolean *) pBuff = nvi_cache.smart_sound;
   return SUCCESS;
}

static int OEMPriv_SetItem_CFGI_SMART_SOUND(void *pBuff)
{
   nv_item_type nvi;

   if (nvi_cache.smart_sound != *((boolean *)pBuff)) {
      nvi_cache.smart_sound = *((boolean *)pBuff);
      nvi.auto_volume_enabled = *((boolean *)pBuff);
      (void) OEMNV_Put( NV_AUTO_VOLUME_ENABLED_I, &nvi);
   }

   return SUCCESS;
}
#endif

static int OEMPriv_GetItem_CFGI_KEYBEEP_SOUND(void *pBuff)
{
   *(byte *) pBuff = nvi_cache.key_sound;
   return SUCCESS;
}

static int OEMPriv_SetItem_CFGI_KEYBEEP_SOUND(void *pBuff)
{
   nv_item_type nvi;

   if (nvi_cache.key_sound != *((byte *)pBuff)) {
      nvi_cache.key_sound = *((byte *)pBuff);
      nvi.key_sound = (nv_key_sound_enum_type) *((byte *)pBuff);
      (void) OEMNV_Put( NV_KEY_SOUND_I, &nvi);
   }

   return SUCCESS;
}


static int OEMPriv_GetItem_CFGI_RINGER_TYPE(void *pBuff)
{
    // To do - ringer type
   return SUCCESS;
}

static int OEMPriv_SetItem_CFGI_RINGER_TYPE(void *pBuff)
{
   nv_item_type nvi;

   if (nvi_cache.ringer_type != *((byte *)pBuff)) {
      nvi_cache.ringer_type = *((byte *)pBuff);
      nvi.ringer_type = *((byte *)pBuff);
      (void) OEMNV_Put( NV_RINGER_TYPE_I, &nvi);
   }

   return SUCCESS;
}

static int OEMPriv_GetItem_CFGI_IMEI(void *pBuff)
{
#if defined (FEATURE_WCDMA) || defined (FEATURE_GSM)
#error code not present
#else
   ((AECHAR *) pBuff)[0] = (AECHAR) 0;
   return EUNSUPPORTED;
#endif
}

static int OEMPriv_SetItem_CFGI_IMEI(void *pBuff)
{
#if defined (FEATURE_WCDMA) || defined (FEATURE_GSM)
#error code not present
#else
   return EUNSUPPORTED;
#endif
}

static int OEMPriv_GetItem_CFGI_CUG(void *pBuff)
{
   MEMCPY(pBuff, (void*) &oemi_cache.cug, sizeof(OEMConfigCugInfo));
   return SUCCESS;
}

static int OEMPriv_SetItem_CFGI_CUG(void *pBuff)
{
   MEMCPY((void*) &oemi_cache.cug, pBuff, sizeof(OEMConfigCugInfo));
   OEMPriv_WriteOEMConfigList();
   return SUCCESS;
}


static int OEMPriv_GetItem_CFGI_VR(void *pBuff)
{
  *(boolean *) pBuff = oemi_cache.vr;
   return SUCCESS;
}

static int OEMPriv_SetItem_CFGI_VR(void *pBuff)
{
  if (oemi_cache.vr != *(boolean *)pBuff) {
      oemi_cache.vr  = *(boolean *)pBuff;
      OEMPriv_WriteOEMConfigList();
   }

   return SUCCESS;
}
static int OEMPriv_GetItem_CFGI_AMR(void *pBuff)
{
  *(boolean *) pBuff = nvi_cache.amr;
   return SUCCESS;
}

static int OEMPriv_SetItem_CFGI_AMR(void *pBuff)
{
#if defined(FEATURE_GSM) || defined (FEATURE_WCDMA)
#error code not present
#endif

  if (nvi_cache.amr != *((boolean *)pBuff)) {
      nvi_cache.amr = *((boolean *)pBuff);
#if defined(FEATURE_GSM) || defined (FEATURE_WCDMA)
#error code not present
#endif
  }
   return SUCCESS;
}


static int OEMPriv_SetItem_CFGI_LCD(void *pBuff)
{
  cfgi_lcd = *((boolean *)pBuff);
  return SUCCESS;
}

static int OEMPriv_GetItem_CFGI_LCD(void *pBuff)
{
  *((boolean *)pBuff)= cfgi_lcd;
   return SUCCESS;
}

static int OEMPriv_GetItem_CFGI_LCD_TIMER(void *pBuff)
{
  *((uint32 *)pBuff)= nvi_cache.display_duration;
   return SUCCESS;
}

static int OEMPriv_SetItem_CFGI_LCD_TIMER(void *pBuff)
{
  nv_item_type nvi;

  if (nvi_cache.display_duration != *((uint32 *)pBuff)) {
      nvi_cache.display_duration = *((uint32 *)pBuff);
      nvi.display_duration = nvi_cache.display_duration;
     (void) OEMNV_Put( NV_DISPLAY_DURATION_I, &nvi);
  }
  return SUCCESS;
}

static int OEMPriv_GetItem_CFGI_CDMA_RX_DIVERSITY_CTL(void *pBuff)
{
#ifdef FEATURE_CDMA_RX_DIVERSITY
   *(boolean *) pBuff = nvi_cache.cdma_rx_diversity_ctrl;
   return SUCCESS;
#else
  return EUNSUPPORTED;
#endif
}

static int OEMPriv_SetItem_CFGI_CDMA_RX_DIVERSITY_CTL(void *pBuff)
{
#ifdef FEATURE_CDMA_RX_DIVERSITY
  nv_item_type nvi;
  if (nvi_cache.cdma_rx_diversity_ctrl != *((boolean *)pBuff)) {
      nvi_cache.cdma_rx_diversity_ctrl = *((boolean *)pBuff);
      nvi.cdma_rx_diversity_ctrl = *((boolean *)pBuff);
      (void) OEMNV_Put( NV_CDMA_RX_DIVERSITY_CTRL_I, &nvi);
  }
   return SUCCESS;
#else
  return EUNSUPPORTED;
#endif
}

static int OEMPriv_GetItem_CFGI_HDR_RX_DIVERSITY_CTL(void *pBuff)
{
#ifdef FEATURE_HDR
#error code not present
#else
   return EUNSUPPORTED;
#endif
}

static int OEMPriv_SetItem_CFGI_HDR_RX_DIVERSITY_CTL(void *pBuff)
{
#ifdef FEATURE_HDR
#error code not present
#else
  return EUNSUPPORTED;
#endif
}

static int OEMPriv_GetItem_CFGI_LAST_RX_DATA_COUNT(void *pBuff)
{
  *(uint32 *) pBuff = nvi_cache.last_rx_data_count;
  return SUCCESS;
}

static int OEMPriv_SetItem_CFGI_LAST_RX_DATA_COUNT(void *pBuff)
{
  nv_item_type nvi;
  if (nvi_cache.last_rx_data_count != *((uint32 *)pBuff)) {
    nvi_cache.last_rx_data_count = *((uint32 *)pBuff);
    nvi.last_rx_data_count = *((uint32 *)pBuff);
    (void) OEMNV_Put(NV_LAST_RX_DATA_COUNT_I, &nvi);
  }
  return SUCCESS;
}

static int OEMPriv_GetItem_CFGI_LAST_TX_DATA_COUNT(void *pBuff)
{
  *(uint32 *) pBuff = nvi_cache.last_tx_data_count;
  return SUCCESS;
}

static int OEMPriv_SetItem_CFGI_LAST_TX_DATA_COUNT(void *pBuff)
{
  nv_item_type nvi;
  if (nvi_cache.last_tx_data_count != *((uint32 *)pBuff)) {
    nvi_cache.last_tx_data_count = *((uint32 *)pBuff);
    nvi.last_tx_data_count = *((uint32 *)pBuff);
    (void) OEMNV_Put(NV_LAST_TX_DATA_COUNT_I, &nvi);
  }
  return SUCCESS;
}

static int OEMPriv_GetItem_CFGI_TOTAL_RX_DATA_COUNT(void *pBuff)
{
  *(uint32 *) pBuff = nvi_cache.total_rx_data_count;
  return SUCCESS;
}

static int OEMPriv_SetItem_CFGI_TOTAL_RX_DATA_COUNT(void *pBuff)
{
  nv_item_type nvi;
  if (nvi_cache.total_rx_data_count != *((uint32 *)pBuff)) {
    nvi_cache.total_rx_data_count = *((uint32 *)pBuff);
    nvi.total_rx_data_count = *((uint32 *)pBuff);
    (void) OEMNV_Put(NV_TOTAL_RX_DATA_COUNT_I, &nvi);
  }
  return SUCCESS;
}

static int OEMPriv_GetItem_CFGI_TOTAL_TX_DATA_COUNT(void *pBuff)
{
  *(uint32 *) pBuff = nvi_cache.total_tx_data_count;
  return SUCCESS;
}

static int OEMPriv_SetItem_CFGI_TOTAL_TX_DATA_COUNT(void *pBuff)
{
  nv_item_type nvi;
  if (nvi_cache.total_tx_data_count != *((uint32 *)pBuff)) {
    nvi_cache.total_tx_data_count = *((uint32 *)pBuff);
    nvi.total_tx_data_count = *((uint32 *)pBuff);
    (void) OEMNV_Put(NV_TOTAL_TX_DATA_COUNT_I, &nvi);
  }
  return SUCCESS;
}

static int OEMPriv_GetItem_CFGI_BEEP_LENGTH(void *pBuff)
{
  *(uint32 *) pBuff = nvi_cache.beep_length;
  return SUCCESS;
}

static int OEMPriv_SetItem_CFGI_BEEP_LENGTH(void *pBuff)
{
  nv_item_type nvi;
  if (nvi_cache.beep_length != *((uint32 *)pBuff)) {
    nvi_cache.beep_length = *((uint32 *)pBuff);
    nvi.dtmf = *((uint32 *)pBuff);
    (void) OEMNV_Put(NV_DTMF_I, &nvi);
  }
  return SUCCESS;
}

static int OEMPriv_GetItem_CFGI_CONTRAST_LVL(void *pBuff)
{
   *(byte *) pBuff = nvi_cache.contrast;
   return SUCCESS;
}

static int OEMPriv_SetItem_CFGI_CONTRAST_LVL(void *pBuff)
{
   nv_item_type nvi;

   if (nvi_cache.contrast != *((byte *)pBuff)) {
      nvi_cache.contrast = *((byte *)pBuff);
      nvi.lcd = *((byte *)pBuff);
      (void) OEMNV_Put( NV_LCD_I, &nvi );
   }

   return SUCCESS;
}

static int OEMPriv_GetItem_CFGI_BACK_LIGHT_INTENSITY(void *pBuff)
{
   *(uint8 *) pBuff = nvi_cache.backlight_intensity;

   return SUCCESS;
}

static int OEMPriv_SetItem_CFGI_BACK_LIGHT_INTENSITY(void *pBuff)
{
   nv_item_type nvi;

   if (nvi_cache.backlight_intensity != *((uint8 *)pBuff)) {
      nvi_cache.backlight_intensity = *((uint8 *)pBuff);
      nvi.back_light_intensity = *((uint8 *)pBuff);
      (void) OEMNV_Put( NV_BACK_LIGHT_INTENSITY_I, &nvi );
   }

   return SUCCESS;
}

static int OEMPriv_GetItem_CFGI_BACK_LIGHT(void *pBuff)
{
   *(byte *) pBuff = nvi_cache.backlight;
   return SUCCESS;
}

static int OEMPriv_SetItem_CFGI_BACK_LIGHT(void *pBuff)
{
   nv_item_type nvi;

   if (nvi_cache.backlight != *((byte *)pBuff)) {
      nvi_cache.backlight = *((byte *)pBuff);
      nvi.back_light = *((byte *)pBuff);
      (void) OEMNV_Put( NV_BACK_LIGHT_I, &nvi );
   }

   return SUCCESS;
}

static int OEMPriv_GetItem_CFGI_LANGUAGE_SELECTION(void *pBuff)
{
   *(byte *) pBuff = nvi_cache.language;
   return SUCCESS;
}


static int OEMPriv_SetItem_CFGI_LANGUAGE_SELECTION(void *pBuff)
{
   nv_item_type nvi;

   if( nvi_cache.language != *((byte *)pBuff )) {
      nvi_cache.language = *((byte *)pBuff );
      nvi.language_selection = (nv_language_enum_type) *((byte *)pBuff);
      (void) OEMNV_Put( NV_LANGUAGE_SELECTION_I, &nvi );

      // NOTE!
      //
      // After setting the new language, the caller must call
      // ISHELL_CloseApplet(ps, TRUE) to exit all applets and return to
      // the Idle applet as the fonts may be different for the new language.
      // Currently only the Idle applet keeps track of the language
      // and takes appropriate action when it is resumed.
      //

   }

   return SUCCESS;
}

static int OEMPriv_GetItem_CFGI_MENU_FORMAT(void *pBuff)
{
   *(byte *) pBuff = nvi_cache.menu_format;
   return SUCCESS;
}

static int OEMPriv_SetItem_CFGI_MENU_FORMAT(void *pBuff)
{
   nv_item_type nvi;

   if (nvi_cache.menu_format != *((byte *)pBuff)) {
      nvi_cache.menu_format = *((byte *)pBuff);
      nvi.menu_format = (nv_menu_format_enum_type) *((byte *)pBuff);
      (void) OEMNV_Put( NV_MENU_FORMAT_I, &nvi );
   }

   return SUCCESS;
}

static int OEMPriv_GetItem_CFGI_TIME_FORMAT(void *pBuff)
{
   *(byte *) pBuff = oemi_cache.time_format;
   return SUCCESS;
}

static int OEMPriv_SetItem_CFGI_TIME_FORMAT(void *pBuff)
{
   if (oemi_cache.time_format != *(boolean *)pBuff) {
      oemi_cache.time_format = *(boolean *)pBuff;
      OEMPriv_WriteOEMConfigList();
   }

   return SUCCESS;
}

static int OEMPriv_GetItem_CFGI_MANUAL_PLMN_SEL(void *pBuff)
{
   *(boolean *) pBuff = nvi_cache.manual_plmn_sel;
   return SUCCESS;
}

static int OEMPriv_SetItem_CFGI_MANUAL_PLMN_SEL(void *pBuff)
{
   nv_item_type nvi;

   if (nvi_cache.manual_plmn_sel != *(boolean *)pBuff) {
      nvi_cache.manual_plmn_sel = *(boolean *)pBuff;
      nvi.manual_plmn_sel = nvi_cache.manual_plmn_sel;
      (void) OEMNV_Put(NV_MANUAL_PLMN_SEL_I, &nvi);
   }
   return SUCCESS;
}

#ifdef FEATURE_GSTK_CS_PS_PARALLEL
static int OEMPriv_GetItem_CFGI_TOOLKIT_CS_PS_PARALLEL(void *pBuff)
{
   *(boolean *) pBuff = nvi_cache.toolkit_cs_ps_parallel;
   return SUCCESS;
}

static int OEMPriv_SetItem_CFGI_TOOLKIT_CS_PS_PARALLEL(void *pBuff)
{
   nv_item_type nvi;

   if (nvi_cache.toolkit_cs_ps_parallel != *(boolean *)pBuff) {
      nvi_cache.toolkit_cs_ps_parallel = *(boolean *)pBuff;
      nvi.toolkit_cs_ps_parallel = nvi_cache.toolkit_cs_ps_parallel;
      (void) OEMNV_Put(NV_TOOLKIT_CS_PS_PARALLEL_I, &nvi);
   }
   return SUCCESS;
}
#endif /* FEATURE_GSTK_CS_PS_PARALLEL */

static int OEMPriv_GetItem_CFGI_CCBS(void *pBuff)
{
   *(OEMCCBS *) pBuff = nvi_cache.ccbs_sel;
   return SUCCESS;
}

static int OEMPriv_SetItem_CFGI_CCBS(void *pBuff)
{
#ifdef FEATURE_CCBS
#error code not present
#endif

   if (nvi_cache.ccbs_sel != *(OEMCCBS *)pBuff) {
      nvi_cache.ccbs_sel = *(OEMCCBS *)pBuff;
#ifdef FEATURE_CCBS
#error code not present
#endif
   }
   return SUCCESS;
}

static int OEMPriv_GetItem_CFGI_HEADSET(void *pBuff)
{
   if(oemi_cache.device_mask & OEMNV_HEADSET_MASK)
     *(boolean *) pBuff = TRUE;
   else{
     *(boolean *) pBuff = FALSE;
   }

   return SUCCESS;
}

static int OEMPriv_SetItem_CFGI_HEADSET(void *pBuff)
{
   if(*(boolean *) pBuff == TRUE){
     oemi_cache.device_mask |= OEMNV_HEADSET_MASK;
   }
   else{
     oemi_cache.device_mask &= ~OEMNV_HEADSET_MASK;
   }

   return SUCCESS;
}

static int OEMPriv_GetItem_CFGI_STEREO_HEADSET(void *pBuff)
{
   if(oemi_cache.device_mask & OEMNV_STEREO_HEADSET_MASK)
     *(boolean *) pBuff = TRUE;
   else{
    *(boolean *) pBuff= FALSE;
   }

   return SUCCESS;
}

static int OEMPriv_SetItem_CFGI_STEREO_HEADSET(void *pBuff)
{
   if(*(boolean *) pBuff == TRUE){
     oemi_cache.device_mask |= OEMNV_STEREO_HEADSET_MASK;
   }
   else{
     oemi_cache.device_mask &= ~OEMNV_STEREO_HEADSET_MASK;
   }

   return SUCCESS;
}

static int OEMPriv_GetItem_CFGI_USB_HFK_ATTACHED(void *pBuff)
{
   if(oemi_cache.device_mask & OEMNV_USB_HFK_ATTACHED_MASK)
     *(boolean *) pBuff = TRUE;
   else{
     *(boolean *) pBuff = FALSE;
   }

   return SUCCESS;
}

static int OEMPriv_SetItem_CFGI_USB_HFK_ATTACHED(void *pBuff)
{
   if(*(boolean *) pBuff == TRUE){
     oemi_cache.device_mask |= OEMNV_USB_HFK_ATTACHED_MASK;
   }
   else{
     oemi_cache.device_mask &= ~OEMNV_USB_HFK_ATTACHED_MASK;
   }

   return SUCCESS;
}

static int OEMPriv_GetItem_CFGI_STEREO_USB_HFK_ATTACHED(void *pBuff)
{
   if(oemi_cache.device_mask & OEMNV_STEREO_USB_HFK_ATTACHED_MASK)
     *(boolean *) pBuff = TRUE;
   else{
     *(boolean *) pBuff = FALSE;
   }

   return SUCCESS;
}

static int OEMPriv_SetItem_CFGI_STEREO_USB_HFK_ATTACHED(void *pBuff)
{
   if(*(boolean *) pBuff == TRUE){
     oemi_cache.device_mask |= OEMNV_STEREO_USB_HFK_ATTACHED_MASK;
   }
   else{
     oemi_cache.device_mask &= ~OEMNV_STEREO_USB_HFK_ATTACHED_MASK;
   }

   return SUCCESS;
}

static int OEMPriv_GetItem_CFGI_EXT_PWR(void *pBuff)
{
   if(oemi_cache.device_mask & OEMNV_EXT_PWR_MASK)
     *(boolean *) pBuff = TRUE;
   else{
     *(boolean *) pBuff = FALSE;
   }

   return SUCCESS;
}

static int OEMPriv_SetItem_CFGI_EXT_PWR(void *pBuff)
{
   if(*(boolean *) pBuff == TRUE){
     oemi_cache.device_mask |= OEMNV_EXT_PWR_MASK;
   }
   else{
     oemi_cache.device_mask &= ~OEMNV_EXT_PWR_MASK;
   }

   return SUCCESS;
}

static int OEMPriv_GetItem_CFGI_DEVICE(void *pBuff)
{
   *(uint32 *) pBuff = oemi_cache.device_mask;
   return SUCCESS;
}

static int OEMPriv_GetItem_CFGI_SMS_SERVICE_OPTION(void *pBuff)
{
   *(byte *)pBuff = oemi_cache.sms_service_option;
   return SUCCESS;
}

static int OEMPriv_SetItem_CFGI_SMS_SERVICE_OPTION(void *pBuff)
{
   if (oemi_cache.sms_service_option != *(byte *)pBuff) {
      oemi_cache.sms_service_option = *(byte *)pBuff;
      OEMPriv_WriteOEMConfigList();
   }

   return SUCCESS;
}

static int OEMPriv_GetItem_CFGI_SMS_TIMESTAMP(void *pBuff)
{
   nv_item_type nvi;

   if (NV_DONE_S != OEMNV_Get(NV_SMS_UTC_I, &nvi)) {
      return EFAILED;
   }

   if (nvi.sms_utc) {
      *(byte *) pBuff = OEMNV_SMS_TIMESTAMP_ADJUST;
   } else {
      *(byte *) pBuff = OEMNV_SMS_TIMESTAMP_ASRECEIVED;
   }

   return SUCCESS;
}

static int OEMPriv_SetItem_CFGI_SMS_TIMESTAMP(void *pBuff)
{
   nv_item_type nvi;

   if (OEMNV_SMS_TIMESTAMP_ADJUST == *(byte *) pBuff) {
      nvi.sms_utc = 1;
   } else {
      nvi.sms_utc = 0;
   }

   if (NV_DONE_S != OEMNV_Put(NV_SMS_UTC_I, &nvi)) {
      return EFAILED;
   }
   return SUCCESS;
}

static int OEMPriv_GetItem_CFGI_SMS_IS41_WORKAROUND(void *pBuff)
{
   *(byte *)pBuff = oemi_cache.sms_is41_workaround;
   return SUCCESS;
}

static int OEMPriv_SetItem_CFGI_SMS_IS41_WORKAROUND(void *pBuff)
{
   if (oemi_cache.sms_is41_workaround != *(byte *)pBuff) {
      oemi_cache.sms_is41_workaround = *(byte *)pBuff;
      OEMPriv_WriteOEMConfigList();
   }

   return SUCCESS;
}

static int OEMPriv_GetItem_CFGI_SMS_MO_ENCODING(void *pBuff)
{
   *(byte *)pBuff = oemi_cache.sms_mo_encoding;
   return SUCCESS;
}

static int OEMPriv_SetItem_CFGI_SMS_MO_ENCODING(void *pBuff)
{
   if (oemi_cache.sms_mo_encoding != *(byte *)pBuff) {
      oemi_cache.sms_mo_encoding = *(byte *)pBuff;
      OEMPriv_WriteOEMConfigList();
   }

   return SUCCESS;
}

static int OEMPriv_GetItem_CFGI_SMS_MODE(void *pBuff)
{
   *(byte *)pBuff = oemi_cache.sms_mode;
   return SUCCESS;
}

static int OEMPriv_SetItem_CFGI_SMS_MODE(void *pBuff)
{
   if (oemi_cache.sms_mode != *(byte *)pBuff) {
      oemi_cache.sms_mode = *(byte *)pBuff;
      OEMPriv_WriteOEMConfigList();
   }

   return SUCCESS;
}

static int OEMPriv_GetItem_CFGI_SMS_APP(void *pBuff)
{
   *(byte *)pBuff = oemi_cache.sms_app;
   return SUCCESS;
}

static int OEMPriv_SetItem_CFGI_SMS_APP(void *pBuff)
{
   if (oemi_cache.sms_app != *(byte *)pBuff) {
      oemi_cache.sms_app = *(byte *)pBuff;
      OEMPriv_WriteOEMConfigList();
   }

   return SUCCESS;
}

static int OEMPriv_GetItem_CFGI_SMS_CB_AUTO_DELETE(void *pBuff)
{
   *(byte *)pBuff = oemi_cache.sms_cb_auto_delete;
   return SUCCESS;
}

static int OEMPriv_SetItem_CFGI_SMS_CB_AUTO_DELETE(void *pBuff)
{
   if (oemi_cache.sms_cb_auto_delete != *(byte *)pBuff) {
      oemi_cache.sms_cb_auto_delete = *(byte *)pBuff;
      OEMPriv_WriteOEMConfigList();
   }
   return SUCCESS;
}

static int OEMPriv_GetItem_CFGI_SMS_CB_SILENT_ALERT(void *pBuff)
{
   *(byte *)pBuff = oemi_cache.sms_cb_silent_alert;
   return SUCCESS;
}

static int OEMPriv_SetItem_CFGI_SMS_CB_SILENT_ALERT(void *pBuff)
{
   if (oemi_cache.sms_cb_silent_alert != *(byte *)pBuff) {
      oemi_cache.sms_cb_silent_alert = *(byte *)pBuff;
      OEMPriv_WriteOEMConfigList();
   }
   return SUCCESS;
}

static int OEMPriv_GetItem_CFGI_SMS_CB_LANG_FILTER(void *pBuff)
{
   *(byte *)pBuff = oemi_cache.sms_cb_lang_filter;
   return SUCCESS;
}

static int OEMPriv_SetItem_CFGI_SMS_CB_LANG_FILTER(void *pBuff)
{
   if (oemi_cache.sms_cb_lang_filter != *(byte *)pBuff) {
      oemi_cache.sms_cb_lang_filter = *(byte *)pBuff;
      OEMPriv_WriteOEMConfigList();
   }
   return SUCCESS;
}

static int OEMPriv_GetItem_CFGI_SMS_ALERT_DISPLAY(void *pBuff)
{
   *(byte *)pBuff = oemi_cache.sms_alert_display;
   return SUCCESS;
}

static int OEMPriv_SetItem_CFGI_SMS_ALERT_DISPLAY(void *pBuff)
{
   if (oemi_cache.sms_alert_display != *(byte *)pBuff) {
      oemi_cache.sms_alert_display = *(byte *)pBuff;
      OEMPriv_WriteOEMConfigList();
   }
   return SUCCESS;
}

static int OEMPriv_GetItem_CFGI_SMS_ALERT_SOUND(void *pBuff)
{
   *(byte *)pBuff = oemi_cache.sms_alert_sound;
   return SUCCESS;
}

static int OEMPriv_SetItem_CFGI_SMS_ALERT_SOUND(void *pBuff)
{
   if (oemi_cache.sms_alert_sound != *(byte *)pBuff) {
      oemi_cache.sms_alert_sound = *(byte *)pBuff;
      OEMPriv_WriteOEMConfigList();
   }
   return SUCCESS;
}

static int OEMPriv_GetItem_CFGI_SMS_CDMA_DEFAULT_TEMPLATE(void *pBuff)
{
   *(byte *)pBuff = oemi_cache.sms_cdma_default_template;
   return SUCCESS;
}

static int OEMPriv_SetItem_CFGI_SMS_CDMA_DEFAULT_TEMPLATE(void *pBuff)
{
   if (oemi_cache.sms_cdma_default_template != *(byte *)pBuff) {
      oemi_cache.sms_cdma_default_template = *(byte *)pBuff;
      OEMPriv_WriteOEMConfigList();
   }
   return SUCCESS;
}

static int OEMPriv_GetItem_CFGI_SMS_GW_DEFAULT_TEMPLATE(void *pBuff)
{
   *(byte *)pBuff = oemi_cache.sms_gw_default_template;
   return SUCCESS;
}

static int OEMPriv_SetItem_CFGI_SMS_GW_DEFAULT_TEMPLATE(void *pBuff)
{
   if (oemi_cache.sms_gw_default_template != *(byte *)pBuff) {
      oemi_cache.sms_gw_default_template = *(byte *)pBuff;
      OEMPriv_WriteOEMConfigList();
   }
   return SUCCESS;
}

static int OEMPriv_GetItem_CFGI_SMS_SEGMENT_SETTING(void *pBuff)
{
   *(byte *)pBuff = oemi_cache.sms_segment_setting;
   return SUCCESS;
}

static int OEMPriv_SetItem_CFGI_SMS_SEGMENT_SETTING(void *pBuff)
{
   if (oemi_cache.sms_segment_setting != *(byte *)pBuff) {
      oemi_cache.sms_segment_setting = *(byte *)pBuff;
      OEMPriv_WriteOEMConfigList();
   }
   return SUCCESS;
}

static int OEMPriv_GetItem_CFGI_SMS_SEGMENT_REF_NUM(void *pBuff)
{
   *(byte *)pBuff = oemi_cache.sms_segment_ref_num;
   return SUCCESS;
}

static int OEMPriv_SetItem_CFGI_SMS_SEGMENT_REF_NUM(void *pBuff)
{
   if (oemi_cache.sms_segment_ref_num != *(byte *)pBuff) {
      oemi_cache.sms_segment_ref_num = *(byte *)pBuff;
      OEMPriv_WriteOEMConfigList();
   }
   return SUCCESS;
}

static int OEMPriv_GetItem_CFGI_SECCODE(void *pBuff)
{
   nv_item_type nvi;

   if (NV_DONE_S != OEMNV_Get(NV_SEC_CODE_I, &nvi)) {
      return EFAILED;
   }
   STR_TO_WSTR((char *)nvi.sec_code.digits,
               (AECHAR *) pBuff,
               OEMNV_SECCODE_LENGTH * sizeof(AECHAR));
   return SUCCESS;
}

static int OEMPriv_SetItem_CFGI_SECCODE(void *pBuff)
{
   char code[OEMNV_SECCODE_LENGTH];
   nv_item_type nvi;

   WSTR_TO_STR((AECHAR *) pBuff,
               code,
               sizeof(code));

   if (OEMNV_SECCODE_LENGTH-1 != STRLEN(code)) {
      return EBADPARM;
   }

   MEMCPY((void *) nvi.sec_code.digits,
          code,
          sizeof(nvi.sec_code.digits));

   if (NV_DONE_S != OEMNV_Put(NV_SEC_CODE_I, &nvi)) {
      return EFAILED;
   }
   return SUCCESS;
}

#ifdef FEATURE_ENABLE_OTKSL
static int OEMPriv_GetItem_CFGI_OTKSLCODE(void *pBuff)
{
   nv_item_type nvi;

   if (NV_DONE_S != OEMNV_Get(NV_OTKSL_I, &nvi)) {
      return EFAILED;
   }
   STR_TO_WSTR((char *)nvi.otksl.digits,
               (AECHAR *) pBuff,
               OEMNV_SECCODE_LENGTH * sizeof(AECHAR));
   return SUCCESS;
}

static int OEMPriv_SetItem_CFGI_OTKSLCODE(void *pBuff)
{
   char code[OEMNV_SECCODE_LENGTH];
   nv_item_type nvi;

   WSTR_TO_STR((AECHAR *) pBuff,
               code,
               sizeof(code));

   if (OEMNV_SECCODE_LENGTH-1 != STRLEN(code)) {
      return EBADPARM;
   }

   MEMCPY((void *) nvi.otksl.digits,
          code,
          sizeof(nvi.otksl.digits));

   if (NV_DONE_S != OEMNV_Put(NV_OTKSL_I, &nvi)) {
      return EFAILED;
   }
   return SUCCESS;
}

#endif /* FEATURE_ENABLE_OTKSL */

static int OEMPriv_GetItem_CFGI_ESN(void *pBuff)
{
   nv_item_type nvi;

   if (NV_DONE_S != OEMNV_Get(NV_ESN_I, &nvi)) {
      return EFAILED;
   }
   *(uint32 *) pBuff = nvi.esn.esn;

   return SUCCESS;
}



static int OEMPriv_GetItem_CFGI_RFCAL_VERSION(void *pBuff)
{
#ifdef FEATURE_RF_ZIF
   nv_item_type nvi;
   int i;

   if (NV_DONE_S != OEMNV_Get(NV_RF_CAL_VER_I, &nvi)) {
      return EFAILED;
   }

   for (i = 0; i < (OEMNV_RFCALVER_LEN - 1); i++) {
      ((AECHAR *) pBuff)[i] = (AECHAR) (nvi.rf_cal_ver[i] + '0');
   }

   ((AECHAR *) pBuff)[OEMNV_RFCALVER_LEN - 1] = (AECHAR) 0;

   return SUCCESS;
#else
#error code not present
#endif

}




static int OEMPriv_GetItem_CFGI_RFCAL_DATE(void *pBuff)
{
   nv_item_type nvi;

   if (NV_DONE_S != OEMNV_Get(NV_RF_CAL_DATE_I, &nvi)) {
      return EFAILED;
   }

#ifdef FEATURE_RF_ZIF
   *(dword *) pBuff = nvi.rf_cal_date;
   return SUCCESS;
#else
#error code not present
#endif

}



static int OEMPriv_GetItem_CFGI_SLOTINDEX(void *pBuff)
{
   nv_item_type nvi;

   if (NV_DONE_S != OEMNV_Get(NV_SLOT_CYCLE_INDEX_I, &nvi)) {
      return EFAILED;
   }
   *(uint8 *) pBuff = nvi.slot_cycle_index;
   return SUCCESS;
}

static int OEMPriv_SetItem_CFGI_SLOTINDEX(void *pBuff)
{
   nv_item_type nvi;

   nvi.slot_cycle_index = *(uint8 *) pBuff;

   if (NV_DONE_S != OEMNV_Put(NV_SLOT_CYCLE_INDEX_I, &nvi)) {
      return EFAILED;
   }
   return SUCCESS;
}

static int OEMPriv_GetItem_CFGI_HOME_SIDNID_LIST(void *pBuff)
{
   nv_item_type             nvi;
   int                      j;
   AEEConfigSIDNIDPairType *p;

   nvi.home_sid_nid.nam = (byte) CM_NAM_1;
   if (NV_DONE_S != OEMNV_Get(NV_HOME_SID_NID_I, &nvi)) {
      return EFAILED;
   }

   MEMSET(pBuff,
          0,
          sizeof(AEEConfigSIDNIDPairType) * OEMNV_HOME_SIDNID_ARRSIZE);


   for (j = 0, p = (AEEConfigSIDNIDPairType *) pBuff;
        j < NV_MAX_HOME_SID_NID;
        j++, p++) {
      p->sid = nvi.home_sid_nid.pair[j].sid;
      p->nid = nvi.home_sid_nid.pair[j].nid;
   }
   return SUCCESS;
}

static int OEMPriv_SetItem_CFGI_HOME_SIDNID_LIST(void *pBuff)
{
   nv_item_type             nvi;
   int                      j;
   AEEConfigSIDNIDPairType *p;

   nvi.home_sid_nid.nam = (byte) CM_NAM_1;

   for (j = 0, p = (AEEConfigSIDNIDPairType *) pBuff;
        j < NV_MAX_HOME_SID_NID;
        j++, p++) {
      nvi.home_sid_nid.pair[j].sid = p->sid;
      nvi.home_sid_nid.pair[j].nid = p->nid;
   }

   if (NV_DONE_S != OEMNV_Put(NV_HOME_SID_NID_I, &nvi)) {
      return EFAILED;
   }

   return SUCCESS;
}

static int OEMPriv_GetItem_CFGI_LOCK_SIDNID_LIST(void *pBuff)
{
   nv_item_type             nvi;
   int                      j;
   AEEConfigSIDNIDPairType *p;

   nvi.sid_nid_lock.nam = (byte) CM_NAM_1;
   if (NV_DONE_S != OEMNV_Get(NV_SID_NID_LOCK_I, &nvi)) {
      return EFAILED;
   }

   MEMSET(pBuff,
          0,
          sizeof(AEEConfigSIDNIDPairType) * OEMNV_LOCK_SIDNID_ARRSIZE);

   for (j = 0, p = (AEEConfigSIDNIDPairType *) pBuff;
        j < NV_MAX_SID_NID_LOCK;
        j++, p++) {
      p->sid = nvi.sid_nid_lock.pair[j].sid;
      p->nid = nvi.sid_nid_lock.pair[j].nid;
   }

   return SUCCESS;
}

static int OEMPriv_SetItem_CFGI_LOCK_SIDNID_LIST(void *pBuff)
{
   nv_item_type             nvi;
   int                      j;
   AEEConfigSIDNIDPairType *p;

   nvi.sid_nid_lock.nam = (byte) CM_NAM_1;

   for (j = 0, p = (AEEConfigSIDNIDPairType *) pBuff;
        j < NV_MAX_SID_NID_LOCK;
        j++, p++) {
      nvi.sid_nid_lock.pair[j].sid = p->sid;
      nvi.sid_nid_lock.pair[j].nid = p->nid;
   }

   if (NV_DONE_S != OEMNV_Put(NV_SID_NID_LOCK_I, &nvi)) {
      return EFAILED;
   }

   return SUCCESS;
}

static int OEMPriv_GetItem_CFGI_IMSI_MCC(void *pBuff)
{
   nv_item_type nvi;

   nvi.imsi_mcc.nam = (byte) CM_NAM_1;
   if (NV_DONE_S != OEMNV_Get(NV_IMSI_MCC_I, &nvi)) {
      return EFAILED;
   }

   *(uint16 *) pBuff = OEMPriv_MCC_TO_DEC(nvi.imsi_mcc.imsi_mcc);
   return SUCCESS;
}


static int OEMPriv_SetItem_CFGI_IMSI_MCC(void *pBuff)
{
   nv_item_type nvi;
   uint16       enc_mcc,
                user_mcc,
                digit;

   ERR("TODO", 0, 0, 0);

   user_mcc = *(uint16 *) pBuff;

   /* Encode MCC as per IS-95A 6.3.1.3 */
   digit = (user_mcc / 100) % 10;
   if (0 == digit) {
      digit = 10;
   }
   enc_mcc = digit;

   digit = (user_mcc / 10) % 10;
   if (0 == digit) {
      digit = 10;
   }
   enc_mcc = (uint16) (enc_mcc * 10) + digit;

   digit = user_mcc % 10;
   if (0 == digit) {
      digit = 10;
   }
   enc_mcc = (uint16) (enc_mcc * 10) + digit;

   enc_mcc -= 111;

   nvi.imsi_mcc.nam = (byte) CM_NAM_1;
   nvi.imsi_mcc.imsi_mcc = enc_mcc;
   if (NV_DONE_S != OEMNV_Put(NV_IMSI_MCC_I, &nvi)) {
      return EFAILED;
   }
   return SUCCESS;
}


static int OEMPriv_GetItem_CFGI_IMSI_11_12(void *pBuff)
{
   nv_item_type      nvi;
   uint16            mnc;
   uint16            digit;

   nvi.imsi_11_12.nam = (byte) CM_NAM_1;
   if (NV_DONE_S != OEMNV_Get(NV_IMSI_11_12_I, &nvi)) {
      return EFAILED;
   }

   /* The saved MNC value is encoded as IS-95A */
   nvi.imsi_11_12.imsi_11_12 %= 100;

   digit = (nvi.imsi_11_12.imsi_11_12 / 10) + 1;
   mnc   = 10 * (digit % 10);

   digit = (nvi.imsi_11_12.imsi_11_12 % 10) + 1;
   mnc  += (digit % 10);

   *(uint16 *) pBuff = mnc;
   return SUCCESS;
}


static int OEMPriv_SetItem_CFGI_IMSI_11_12(void *pBuff)
{
   nv_item_type nvi;
   uint16       user,
                enc,
                digit;

   /* Encode IMSI_11_12 as per IS-95A 6.3.1.2 */
   user = *(uint16 *) pBuff;

   digit = (user / 10) % 10;
   if (0 == digit) {
      digit = 10;
   }
   enc = digit;

   digit = user % 10;
   if (0 == digit) {
      digit = 10;
   }
   enc = (uint16) (enc * 10) + digit;

   enc -= 11;
   enc &= 0x00FF;

   nvi.imsi_11_12.nam = (byte) CM_NAM_1;
   nvi.imsi_11_12.imsi_11_12 = (uint8) enc;

   if (NV_DONE_S != OEMNV_Put(NV_IMSI_11_12_I, &nvi)) {
      return EFAILED;
   }

   return SUCCESS;
}


static int OEMPriv_GetItem_CFGI_IMSI_S(void *pBuff)
{
   char         imsi_s[11];
   nv_item_type nvi;
   uint16       min2;

   nvi.min2.nam = (byte) CM_NAM_1;
   if (NV_DONE_S != OEMNV_Get(NV_MIN2_I, &nvi)) {
      return EFAILED;
   }

   min2 = OEMPriv_MIN2_TO_DEC(nvi.min2.min2[CDMAMIN]);

   imsi_s[0] = '0' + (char) (min2 / 100);
   imsi_s[1] = '0' + (char) ( (min2 / 10) % 10);
   imsi_s[2] = '0' + (char) (min2 % 10);

   nvi.min1.nam = (byte) CM_NAM_1;
   if (NV_DONE_S != OEMNV_Get(NV_MIN1_I, &nvi)) {
      return EFAILED;
   }

   OEMPriv_MIN1_TO_STR(nvi.min1.min1[CDMAMIN], &imsi_s[3]);

   STR_TO_WSTR(imsi_s,
               (AECHAR *) pBuff,
               sizeof(AECHAR) * OEMNV_IMSI_S_LENGTH);

   return SUCCESS;
}


static int OEMPriv_SetItem_CFGI_IMSI_S(void *pBuff)
{
   nv_item_type nvi;
   AECHAR      *imsi;
   uint16       min2;
   uint32       min1;
   uint16       digit;
   int          i;

   // Validate the input string
   imsi = (AECHAR *) pBuff;
   if (WSTRLEN(imsi) != 10) {
      return EFAILED;
   }

   while (*imsi) {
      if ( (*imsi < (AECHAR) '0') ||
           (*imsi > (AECHAR) '9') ) {
         return EFAILED;
      }
      imsi++;
   }

   //
   // Encode the first three digits (IS-95A 6.3.1.1)
   //
   imsi = (AECHAR *) pBuff;
   min2 = 0;
   for (i = 0; i < 3; i++) {
      digit = *imsi - (AECHAR) '0';
      if (0 == digit) {
         digit = 10;
      }
      imsi++;

      min2 = (uint16) (min2 * 10) + digit;
   }

   min2 -= 111;

   //
   // Encode the last seven digits (IS-95A 6.3.1.1)
   //

   // Encode the second three digits into the ten most
   // significant bits (of the 24-bit number)...
   min1 = 0;
   for (i = 0; i < 3; i++) {
      digit = *imsi - (AECHAR) '0';
      if (0 == digit) {
         digit = 10;
      }
      imsi++;

      min1 = (uint32) (min1 * 10) + digit;
   }

   min1 -= 111;

   min1 <<= 14;
   min1 &= 0x00FFC000;

   // The fourth last digit is mapping as BCD into four bits
   digit = *imsi - (AECHAR) '0';
   if (0 == digit) {
      digit = 10;
   }
   imsi++;

   min1 = min1 | (0x00003C00 & (digit << 10));

   // Encode the last three digits into the ten least significant bits
   {
      uint32 tmp = 0;

      for (i = 0; i < 3; i++) {
         digit = *imsi - (AECHAR) '0';
         if (0 == digit) {
            digit = 10;
         }
         imsi++;

         tmp = (uint32) (tmp * 10) + digit;
      }

      tmp -= 111;
      tmp &= 0x000003FF;

      min1 |= tmp;
   }

   //
   // Write the encoded values out to NV...
   //

   nvi.min2.nam = (byte) CM_NAM_1;
   nvi.min2.min2[CDMAMIN] = min2;
   if (NV_DONE_S != OEMNV_Put(NV_MIN2_I, &nvi)) {
      return EFAILED;
   }

   nvi.min1.nam = (byte) CM_NAM_1;
   nvi.min1.min1[CDMAMIN] = min1;

   if (NV_DONE_S != OEMNV_Put(NV_MIN1_I, &nvi)) {
      return EFAILED;
   }

   return SUCCESS;
}


static int OEMPriv_GetItem_CFGI_PRL_ENABLED(void *pBuff)
{
   nv_item_type nvi;

   nvi.prl_enabled.nam = (byte) CM_NAM_1;
   if (NV_DONE_S != OEMNV_Get(NV_PRL_ENABLED_I, &nvi)) {
      return EFAILED;
   }

   *(boolean *) pBuff = nvi.prl_enabled.enabled;
   return SUCCESS;
}

static int OEMPriv_SetItem_CFGI_PRL_ENABLED(void *pBuff)
{
   nv_item_type nvi;

   nvi.prl_enabled.nam = (byte) CM_NAM_1;
   nvi.prl_enabled.enabled = *(boolean *) pBuff;

   if (NV_DONE_S != OEMNV_Put(NV_PRL_ENABLED_I, &nvi)) {
      return EFAILED;
   }

   return SUCCESS;
}

static int OEMPriv_GetItem_CFGI_AOC(void *pBuff)
{
   nv_item_type nvi;

   nvi.accolc.nam = (byte) CM_NAM_1;
   if (NV_DONE_S != OEMNV_Get(NV_ACCOLC_I, &nvi)) {
      return EFAILED;
   }

   *(uint8 *) pBuff = nvi.accolc.ACCOLCpClass[CDMAMIN];
   return SUCCESS;
}

static int OEMPriv_SetItem_CFGI_AOC(void *pBuff)
{
   nv_item_type nvi;

   nvi.accolc.nam = (byte) CM_NAM_1;
   nvi.accolc.ACCOLCpClass[CDMAMIN] = *(uint8 *) pBuff;
   if (NV_DONE_S != OEMNV_Put(NV_ACCOLC_I, &nvi)) {
      return EFAILED;
   }
   return SUCCESS;
}


static int OEMPriv_GetItem_CFGI_HOME_SID_REG(void *pBuff)
{
   nv_item_type nvi;

   nvi.mob_term_home.nam = (byte) CM_NAM_1;
   if (NV_DONE_S != OEMNV_Get(NV_MOB_TERM_HOME_I, &nvi)) {
      return EFAILED;
   }

   *(boolean *) pBuff = nvi.mob_term_home.enabled[CDMAMIN] ? TRUE:FALSE;
   return SUCCESS;
}

static int OEMPriv_SetItem_CFGI_HOME_SID_REG(void *pBuff)
{
   nv_item_type nvi;

   nvi.mob_term_home.nam = (byte) CM_NAM_1;
   nvi.mob_term_home.enabled[CDMAMIN] = *(boolean *) pBuff;

   if (NV_DONE_S != OEMNV_Put(NV_MOB_TERM_HOME_I, &nvi)) {
      return EFAILED;
   }

   return SUCCESS;
}

static int OEMPriv_GetItem_CFGI_FORN_SID_REG(void *pBuff)
{
   nv_item_type nvi;

   nvi.mob_term_for_sid.nam = (byte) CM_NAM_1;
   if (NV_DONE_S != OEMNV_Get(NV_MOB_TERM_FOR_SID_I, &nvi)) {
      return EFAILED;
   }

   *(boolean *) pBuff = nvi.mob_term_for_sid.enabled[CDMAMIN]?TRUE:FALSE;
   return SUCCESS;
}

static int OEMPriv_SetItem_CFGI_FORN_SID_REG(void *pBuff)
{
   nv_item_type nvi;

   nvi.mob_term_for_sid.nam = (byte) CM_NAM_1;
   nvi.mob_term_for_sid.enabled[CDMAMIN] = *(boolean *) pBuff;

   if (NV_DONE_S != OEMNV_Put(NV_MOB_TERM_FOR_SID_I, &nvi)) {
      return EFAILED;
   }

   return SUCCESS;
}

static int OEMPriv_GetItem_CFGI_FORN_NID_REG(void *pBuff)
{
   nv_item_type nvi;

   nvi.mob_term_for_nid.nam = (byte) CM_NAM_1;
   if (NV_DONE_S != OEMNV_Get(NV_MOB_TERM_FOR_NID_I, &nvi)) {
      return EFAILED;
   }

   *(boolean *) pBuff = nvi.mob_term_for_nid.enabled[CDMAMIN]?TRUE:FALSE;
   return SUCCESS;
}

static int OEMPriv_SetItem_CFGI_FORN_NID_REG(void *pBuff)
{
   nv_item_type nvi;

   nvi.mob_term_for_nid.nam = (byte) CM_NAM_1;
   nvi.mob_term_for_nid.enabled[CDMAMIN] = *(boolean *) pBuff;

   if (NV_DONE_S != OEMNV_Put(NV_MOB_TERM_FOR_NID_I, &nvi)) {
      return EFAILED;
   }

   return SUCCESS;
}

#ifdef FEATURE_ACP
#error code not present
#endif /* FEATURE_ACP */

static int OEMPriv_GetItem_CFGI_DATA_QNC_ENABLED(void *pBuff)
{
   nv_item_type nvi;

   if (NV_DONE_S != OEMNV_Get(NV_DATA_QNC_ENABLED_I, &nvi)) {
      return EFAILED;
   }

   *(boolean *) pBuff = nvi.data_qnc_enabled;
   return SUCCESS;
}

static int OEMPriv_SetItem_CFGI_DATA_QNC_ENABLED(void *pBuff)
{
   nv_item_type nvi;

   nvi.data_qnc_enabled = *(boolean *)pBuff;

   if (NV_DONE_S != OEMNV_Put(NV_DATA_QNC_ENABLED_I, &nvi)) {
      return EFAILED;
   }

   return SUCCESS;
}

#ifdef FEATURE_FDN

static int OEMPriv_GetItem_CFGI_FDN_ENABLED(void *pBuff)
{
   *(boolean *)pBuff = OEM_IsFDNEnabled();
   return SUCCESS;
}


static int OEMPriv_SetItem_CFGI_FDN_ENABLED(void *pBuff)
{
   if (OEM_EnableFDN(*(boolean *) pBuff)) {
      return SUCCESS;
   }

   return EUNSUPPORTED;
}

#endif /* FEATURE_FDN */


static int OEMPriv_GetItem_CFGI_DEBUG_CHANNEL(void *pBuff)
{
#if defined(FEATURE_RFM_IOCTL)
  rfcom_ioctl_pointer_type rfm_ioctl_data;
  if (rfm_ioctl( RFCOM_TRANSCEIVER_0, RFCOM_IOCTL_GET_DBG_SCR_CHANNEL, &rfm_ioctl_data, 2) 
      != RFCOM_IOCTL_NO_ERROR )
  {
    *(uint16 *)pBuff = 0;
    return EUNSUPPORTED;
  }
  *(uint16 *)pBuff = rfm_ioctl_data.uint16_field;
  return SUCCESS;

#elif defined (FEATURE_CDMA_800) || defined (FEATURE_CDMA_1900)
   db_items_value_type dbi;

   db_get(DB_CHANNEL, &dbi);
   *(uint16 *)pBuff = dbi.channel;

   return SUCCESS;

#elif defined (FEATURE_WCDMA) || defined (FEATURE_GSM)
#error code not present
#else
   return EUNSUPPORTED;
#endif
}


static int OEMPriv_GetItem_CFGI_DEBUG_PILOT_PN(void *pBuff)
{
#if defined (FEATURE_CDMA_800) || defined (FEATURE_CDMA_1900)
   db_items_value_type dbi;

   db_get(DB_PILOT_PN_OFF, &dbi);
   *(uint16 *)pBuff = dbi.pilot_pn_off;
   return SUCCESS;
#else
   return EUNSUPPORTED;
#endif
}


static int OEMPriv_GetItem_CFGI_DEBUG_TX_AGC(void *pBuff)
{
#ifndef FEATURE_ICM
   MSG_HIGH("OEMPriv_GetItem_CFGI_DEBUG_TX_AGC: FEATURE_ICM not turned on",0,0,0);
   *(uint16 *)pBuff = 0;
   return EFAILED;
#else //FEATURE_ICM
   ICM* pICM;
   boolean nReturnStatus;
   AEECMSysMode sys_mode;
  
   /* obtain shell instance */
   IShell *pIShell = AEE_GetShell();
   if (NULL == pIShell) {
    MSG_ERROR("OEMPriv_GetItem_CFGI_DEBUG_TX_AGC: NULL IShell Pointer",0,0,0);
    *(uint16 *)pBuff = 0;
    return EFAILED;
   }
   
   /* Create the Call Manager object. */
   nReturnStatus = ISHELL_CreateInstance(pIShell,
                                         AEECLSID_CM,
                                         (void **) &pICM);
   if ((pICM == NULL) || (nReturnStatus != SUCCESS))
   {
     MSG_ERROR("OEMPriv_GetItem_CFGI_DEBUG_TX_AGC: ICM object creation failed pICM: %d, nReturnStatus: %d",
               pICM,nReturnStatus,0);
     if (pICM != NULL)
     {
       (void) ICM_Release(pICM);
     }
     *(uint16 *)pBuff = 0;
     return EFAILED;
   }
   sys_mode = ICM_GetSysMode(pICM);
   (void) ICM_Release(pICM);

#if defined(FEATURE_RFM_IOCTL)
  sys_mode = sys_mode; /* not used but easier to do this than to featurize out */
  {
    rfcom_ioctl_pointer_type rfm_ioctl_data;
    if( rfm_ioctl( RFCOM_TRANSCEIVER_0, RFCOM_IOCTL_GET_DBG_SCR_TX_AGC, &rfm_ioctl_data, 2) 
        != RFCOM_IOCTL_NO_ERROR )
    {
      *(uint16 *)pBuff = 0;
      return EUNSUPPORTED;
    }
    *(uint16 *)pBuff = rfm_ioctl_data.uint16_field;
  }
  return SUCCESS;

#elif defined (FEATURE_CDMA_800) || defined (FEATURE_CDMA_1900)
   if (sys_mode == AEECM_SYS_MODE_CDMA)
   {
     *(uint16 *)pBuff = (uint16) rf_get_cdma_tx_agc();
     return SUCCESS;
   }
   *(uint16 *)pBuff = 0;
   return EUNSUPPORTED;
   
#elif defined (FEATURE_WCDMA)
#error code not present
#else
   *(uint16 *)pBuff = 0;
   return EUNSUPPORTED;
#endif 

#endif //FEATURE_ICM
}

static int OEMPriv_GetItem_CFGI_DEBUG_TX_AGC_IN_DBM(void *pBuff)
{
#ifndef FEATURE_ICM
   MSG_HIGH("OEMPriv_GetItem_CFGI_DEBUG_TX_AGC_IN_DBM: FEATURE_ICM not turned on",0,0,0);
   *(int16 *)pBuff = 0;
   return EFAILED;
#else //FEATURE_ICM
   ICM* pICM;
   boolean nReturnStatus;
   AEECMSysMode sys_mode;
   
   /* obtain shell instance */
   IShell *pIShell = AEE_GetShell();
   if (NULL == pIShell) {
    MSG_ERROR("OEMPriv_GetItem_CFGI_DEBUG_TX_AGC_IN_DBM: NULL IShell Pointer",0,0,0);
    *(int16 *)pBuff = 0;
    return EFAILED;
   }
   
   /* Create the Call Manager object. */
   nReturnStatus = ISHELL_CreateInstance(pIShell,
                                         AEECLSID_CM,
                                         (void **) &pICM);
   if ((pICM == NULL) || (nReturnStatus != SUCCESS))
   {
     MSG_ERROR("OEMPriv_GetItem_CFGI_DEBUG_TX_AGC_IN_DBM: ICM object creation failed pICM: %d, nReturnStatus: %d",
               pICM,nReturnStatus,0);
     if (pICM != NULL)
     {
       (void) ICM_Release(pICM);
     }
     *(int16 *)pBuff = 0;
     return EFAILED;
   }
   sys_mode = ICM_GetSysMode(pICM);
   (void) ICM_Release(pICM);

#if defined(FEATURE_RFM_IOCTL)
  sys_mode = sys_mode; /* not used but easier to do this than to featurize out */
  {
    rfcom_ioctl_pointer_type rfm_ioctl_data;
    if( rfm_ioctl( RFCOM_TRANSCEIVER_0, RFCOM_IOCTL_GET_DBG_SCR_TX_AGC_IN_DBM, &rfm_ioctl_data, 2) 
        != RFCOM_IOCTL_NO_ERROR )
    {
      *(int16 *)pBuff = 0;
      return EUNSUPPORTED;
    }
    *(int16 *)pBuff = (int16)(rfm_ioctl_data.uint16_field);
  }
  return SUCCESS;

#elif (defined (FEATURE_CDMA_800) || defined (FEATURE_CDMA_1900)) && defined(FEATURE_RF_ZIF) && defined (T_MSM6500)
   if (sys_mode == AEECM_SYS_MODE_CDMA)
   {
     *(int16 *)pBuff = rf_get_cdma_tx_agc_in_dBm();
     return SUCCESS;
   }
   *(int16 *)pBuff = 0;
   return EUNSUPPORTED;

#elif defined (FEATURE_WCDMA)
#error code not present
#else
   *(int16 *)pBuff = 0;
   return EUNSUPPORTED;
#endif 

#endif //FEATURE_ICM
}


static int OEMPriv_GetItem_CFGI_DEBUG_RX_AGC(void *pBuff)
{
#ifndef FEATURE_ICM
   MSG_HIGH("OEMPriv_GetItem_CFGI_DEBUG_RX_AGC: FEATURE_ICM not turned on",0,0,0);
   *(signed char *)pBuff = 0;
   return EFAILED;
#else //FEATURE_ICM
#if defined (FEATURE_CDMA1X)
   ICM* pICM;
   AEECMSysMode sys_mode;
   boolean nReturnStatus;
   IShell *pIShell;
   
   /* obtain shell instance */
   pIShell = AEE_GetShell();
   if (NULL == pIShell) {
     MSG_ERROR("OEMPriv_GetItem_CFGI_DEBUG_RX_AGC: NULL IShell Pointer",0,0,0);
     *(signed char *)pBuff = 0;
     return EFAILED;
   }
   
   /* Create the Call Manager object. */
   nReturnStatus = ISHELL_CreateInstance(pIShell,
                                         AEECLSID_CM,
                                         (void **) &pICM);
   if ((pICM == NULL) || (nReturnStatus != SUCCESS))
   {
     MSG_ERROR("OEMPriv_GetItem_CFGI_DEBUG_RX_AGC: ICM object creation failed pICM: %d, nReturnStatus: %d",
               pICM,nReturnStatus,0);
     if (pICM != NULL)
     {
       (void) ICM_Release(pICM);
     }
     *(signed char *)pBuff = 0;
     return EFAILED;
   }
   sys_mode = ICM_GetSysMode(pICM);
   (void) ICM_Release(pICM);

   if (sys_mode == AEECM_SYS_MODE_CDMA)
   {
#if defined(FEATURE_RFM_IOCTL)
     rfcom_ioctl_pointer_type rfm_ioctl_data;
     (void)rfm_ioctl( RFCOM_TRANSCEIVER_0, RFCOM_IOCTL_GET_DBG_SCR_RX_AGC, &rfm_ioctl_data, 1);
     *(byte *)pBuff = (byte)(rfm_ioctl_data.uint8_field);
#else
     *(signed char *)pBuff = rf_get_cdma_rx_agc();
#endif
     return SUCCESS;
   }
   else
   {
     *(signed char *)pBuff = 0;
     return EUNSUPPORTED;
   } 
#else /* FEATURE_CDMA1X */
   *(signed char *)pBuff = 0;
   return EUNSUPPORTED;
#endif //FEATURE_CDMA1X
#endif //FEATURE_ICM
}

static int OEMPriv_GetItem_CFGI_DEBUG_RX_AGC_IN_DBM(void *pBuff)
{
#ifndef FEATURE_ICM
   MSG_HIGH("OEMPriv_GetItem_CFGI_DEBUG_RX_AGC_IN_DBM: FEATURE_ICM not turned on",0,0,0);
   *(int32 *)pBuff = 0;
   return EFAILED;
#else //FEATURE_ICM

#ifndef FEATURE_RFM_IOCTL
#if (defined (FEATURE_CDMA_800) || defined (FEATURE_CDMA_1900)) && defined(FEATURE_RF_ZIF)
   int16 ch0_rxagc;
   int16 ch1_rxagc;
#endif
#endif
   ICM* pICM;
   boolean nReturnStatus;
   AEECMSysMode sys_mode;
   
   /* obtain shell instance */
   IShell *pIShell = AEE_GetShell();
   if (NULL == pIShell) {
    MSG_ERROR("OEMPriv_GetItem_CFGI_DEBUG_RX_AGC_IN_DBM: NULL IShell Pointer",0,0,0);
    *(int32 *)pBuff = 0;
    return EFAILED;
   }
   
   /* Create the Call Manager object. */
   nReturnStatus = ISHELL_CreateInstance(pIShell,
                                         AEECLSID_CM,
                                         (void **) &pICM);
   if ((pICM == NULL) || (nReturnStatus != SUCCESS))
   {
     MSG_ERROR("OEMPriv_GetItem_CFGI_DEBUG_RX_AGC_IN_DBM: ICM object creation failed pICM: %d, nReturnStatus: %d",
               pICM,nReturnStatus,0);
     if (pICM != NULL)
     {
       (void) ICM_Release(pICM);
     }
     *(int32 *)pBuff = 0;
     return EFAILED;
   }
   sys_mode = ICM_GetSysMode(pICM);
   (void) ICM_Release(pICM);

#if defined(FEATURE_RFM_IOCTL)
  sys_mode = sys_mode; /* not used but easier to do this than to featurize out */
  {
    rfcom_ioctl_pointer_type rfm_ioctl_data;
    if( rfm_ioctl( RFCOM_TRANSCEIVER_0, RFCOM_IOCTL_GET_DBG_SCR_RX_AGC_IN_DBM, &rfm_ioctl_data, 4) 
        != RFCOM_IOCTL_NO_ERROR )
    {
      *(int32 *)pBuff = 0;
      return EUNSUPPORTED;
    }
    *(int32 *)pBuff = (int32)(rfm_ioctl_data.uint32_field);
  }
  return SUCCESS;

#elif (defined (FEATURE_CDMA_800) || defined (FEATURE_CDMA_1900)) && defined(FEATURE_RF_ZIF)
   if (sys_mode == AEECM_SYS_MODE_CDMA)
   {
     /* obtain Rx AGC readings from both Rx chains */
     ch0_rxagc = rf_get_path_cdma_rx_agc_in_dBm( RFCOM_TRANSCEIVER_0 );
     ch1_rxagc = rf_get_path_cdma_rx_agc_in_dBm( RFCOM_RECEIVER_1 );
  
     /* assemble into a 32 bit packet */
     *(int32 *)pBuff = (ch0_rxagc << 16) | (ch1_rxagc &0xFFFF );
     return SUCCESS;
   }
   *(int32 *) pBuff = 0;
   return EUNSUPPORTED;

#elif defined (FEATURE_WCDMA)
#error code not present
#else
   *(int32 *)pBuff = 0;
   return EUNSUPPORTED;
#endif 

#endif //FEATURE_ICM
}

static int OEMPriv_GetItem_CFGI_DEBUG_SID(void *pBuff)
{
#if defined (FEATURE_CDMA_800) || defined (FEATURE_CDMA_1900)
   db_items_value_type dbi;

   db_get(DB_SID, &dbi);
   *(uint16 *)pBuff = dbi.sid;
   return SUCCESS;
#else
   return EUNSUPPORTED;
#endif
}


static int OEMPriv_GetItem_CFGI_DEBUG_NID(void *pBuff)
{
#if defined (FEATURE_CDMA_800) || defined (FEATURE_CDMA_1900)
   db_items_value_type dbi;

   db_get(DB_NID, &dbi);
   *(uint16 *)pBuff = dbi.nid;
   return SUCCESS;
#else
   return EUNSUPPORTED;
#endif
}


static int OEMPriv_GetItem_CFGI_DEBUG_TX_ADJUST(void *pBuff)
{
#ifndef FEATURE_ICM
   MSG_HIGH("OEMPriv_GetItem_CFGI_DEBUG_TX_ADJUST: FEATURE_ICM not turned on",0,0,0);
   *(signed char *)pBuff = 0;
   return EFAILED;
#else //FEATURE_ICM
#ifdef FEATURE_RF_ZIF
   ICM* pICM;
   boolean nReturnStatus;
   AEECMSysMode sys_mode;
   
   /* obtain shell instance */
   IShell *pIShell = AEE_GetShell();
   if (NULL == pIShell) {
    MSG_ERROR("OEMPriv_GetItem_CFGI_DEBUG_TX_ADJUST: NULL IShell Pointer",0,0,0);
    *(signed char *)pBuff = 0;
    return EFAILED;
   }
   
   /* Create the Call Manager object. */
   nReturnStatus = ISHELL_CreateInstance(pIShell,
                                         AEECLSID_CM,
                                         (void **) &pICM);
   if ((pICM == NULL) || (nReturnStatus != SUCCESS))
   {
     MSG_ERROR("OEMPriv_GetItem_CFGI_DEBUG_TX_ADJUST: ICM object creation failed pICM: %d, nReturnStatus: %d",
               pICM,nReturnStatus,0);
     if (pICM != NULL)
     {
       (void) ICM_Release(pICM);
     }
     *(signed char *)pBuff = 0;
     return EFAILED;
   }
   sys_mode = ICM_GetSysMode(pICM);
   (void) ICM_Release(pICM);

   if (sys_mode == AEECM_SYS_MODE_CDMA)
   {
#if defined(FEATURE_RFM_IOCTL)
     rfcom_ioctl_pointer_type rfm_ioctl_data;
     (void)rfm_ioctl( RFCOM_TRANSCEIVER_0, RFCOM_IOCTL_GET_DBG_SCR_TX_ADJ, &rfm_ioctl_data, 1);
     *(signed char *)pBuff = (signed char)(rfm_ioctl_data.uint8_field);
#else
     *(signed char *)pBuff = rf_get_tx_gain_adj();
#endif
     return SUCCESS;
   }
   else
   {
     *(signed char *)pBuff = 0;
     return EUNSUPPORTED;
   }
#else
#error code not present
#endif
#endif //FEATURE_ICM
}

static int OEMPriv_GetItem_CFGI_DEBUG_HDET(void *pBuff)
{
#if defined(FEATURE_RFM_IOCTL)
  rfcom_ioctl_pointer_type rfm_ioctl_data;
  if( rfm_ioctl( RFCOM_TRANSCEIVER_0, RFCOM_IOCTL_GET_DBG_SCR_HDET, &rfm_ioctl_data, 1) 
      != RFCOM_IOCTL_NO_ERROR )
  {
    *(byte *)pBuff = 0;
    return EUNSUPPORTED;
  }
  *(byte *)pBuff = (byte)(rfm_ioctl_data.uint8_field);
  return SUCCESS;

#elif defined(FEATURE_RF_ZIF)
   *(byte *)pBuff = (byte) rf_hdet_data; /* adc_read(ADC_HDET_CELL); */
   return SUCCESS;
#elif defined (FEATURE_WCDMA)
#error code not present
#else
#error code not present
#endif
}


static int OEMPriv_GetItem_CFGI_DEBUG_VBATT(void *pBuff)
{
#ifdef FEATURE_RF_ZIF
   *(byte *)pBuff = (byte) adc_read(ADC_VBATT);
   return SUCCESS;
#elif defined(FEATURE_WCDMA) || defined(FEATURE_GSM)
#error code not present
#else
#error code not present
#endif
}


static int OEMPriv_GetItem_CFGI_DEBUG_ERRLOG(void *pBuff)
{
   OEMErrLogType *p_errlog = (OEMErrLogType *)pBuff;
   nv_err_log_type nv_log;
   int i;

   MEMSET ((void *) &nv_log, 0, sizeof (nv_err_log_type));

   if (p_errlog->err_num < NV_MAX_ERR_LOG) {
      (void) err_get_log(p_errlog->err_num, &nv_log);
      if (nv_log.err_count) {
         p_errlog->err_count = nv_log.err_count;
         p_errlog->line_num = nv_log.line_num;
         p_errlog->fatal = nv_log.fatal;
         for (i = 0; i < NV_ERR_FILE_NAME_LEN; i++) {
            p_errlog->file_name[i] = nv_log.file_name[i];
         }
         return SUCCESS;
      }
   }
   return EFAILED;
}

#ifndef FEATURE_RFM_IOCTL
#if defined(FEATURE_RF_ZIF) && defined(T_MSM6500)
// Since these aren't prototyped in rf.h, we'll do it here.
extern lna_gain_state_type
       rf_get_path_lna_gain_state(rfcom_device_enum_type device);
extern boolean rf_is_sleeping ( rfcom_device_enum_type device);
#endif
#endif

static int OEMPriv_GetItem_CFGI_DEBUG_LNA_STATE(void *pBuff)
{

#if defined(FEATURE_RFM_IOCTL)
  rfcom_ioctl_pointer_type rfm_ioctl_data;
  if( rfm_ioctl( RFCOM_TRANSCEIVER_0, RFCOM_IOCTL_GET_DBG_SCR_LNA_STATE, &rfm_ioctl_data, 1) 
      != RFCOM_IOCTL_NO_ERROR )
  {
    *(byte *)pBuff = 0;
    return EUNSUPPORTED;
  }
  *(byte *)pBuff = (byte)(rfm_ioctl_data.uint8_field);
  return SUCCESS;

#elif defined(FEATURE_RF_ZIF) && defined(T_MSM6500)
   byte ch0_lna;
   byte ch1_lna;

   ch0_lna = rf_get_path_lna_gain_state( RFCOM_TRANSCEIVER_0 );
   ch1_lna = rf_get_path_lna_gain_state( RFCOM_RECEIVER_1 );

   *(byte *)pBuff = (ch0_lna << 4) | (ch1_lna & 0x0F);
   return SUCCESS;
#elif defined(FEATURE_RF_ZIF)
#error code not present
#elif defined(FEATURE_WCDMA) || defined(FEATURE_GSM)
#error code not present
#else
#error code not present
#endif
}

static int OEMPriv_GetItem_CFGI_DEBUG_PA_STATE(void *pBuff)
{
#ifndef FEATURE_ICM
   MSG_HIGH("OEMPriv_GetItem_CFGI_DEBUG_PA_STATE: FEATURE_ICM not turned on",0,0,0);
   *(byte *)pBuff = 0;
   return EFAILED;
#else //FEATURE_ICM
   ICM* pICM;
   boolean nReturnStatus;
   AEECMSysMode sys_mode;
   
   /* obtain shell instance */
   IShell *pIShell = AEE_GetShell();
   if (NULL == pIShell) {
    MSG_ERROR("OEMPriv_GetItem_CFGI_DEBUG_PA_STATE: NULL IShell Pointer",0,0,0);
    *(byte *)pBuff = 0;
    return EFAILED;
   }
   
   /* Create the Call Manager object. */
   nReturnStatus = ISHELL_CreateInstance(pIShell,
                                         AEECLSID_CM,
                                         (void **) &pICM);
   if ((pICM == NULL) || (nReturnStatus != SUCCESS))
   {
     MSG_ERROR("OEMPriv_GetItem_CFGI_DEBUG_PA_STATE: ICM object creation failed pICM: %d, nReturnStatus: %d",
               pICM,nReturnStatus,0);
     if (pICM != NULL)
     {
       (void) ICM_Release(pICM);
     }
     *(byte *)pBuff = 0;
     return EFAILED;
   }
   sys_mode = ICM_GetSysMode(pICM);
   (void) ICM_Release(pICM);

#if defined(FEATURE_RFM_IOCTL)
  sys_mode = sys_mode; /* not used but easier to do this than to featurize out */
  {
    rfcom_ioctl_pointer_type rfm_ioctl_data;
    if( rfm_ioctl( RFCOM_TRANSCEIVER_0, RFCOM_IOCTL_GET_DBG_SCR_PA_RANGE, &rfm_ioctl_data, 1) 
        != RFCOM_IOCTL_NO_ERROR )
    {
      *(byte *)pBuff = 0;
      return EUNSUPPORTED;
    }
    *(byte *)pBuff = (byte)(rfm_ioctl_data.uint8_field);
    return SUCCESS;
  }

#elif defined(FEATURE_RF_ZIF)
   if (sys_mode == AEECM_SYS_MODE_CDMA)
   {
     *(byte *)pBuff = (byte) rf_get_pa_state();
     return SUCCESS;
   }
   *(byte *)pBuff = 0;
   return EUNSUPPORTED;

#elif defined(FEATURE_WCDMA)
#error code not present
#else
#error code not present
#endif 

#endif //FEATURE_ICM
}

static int OEMPriv_GetItem_CFGI_DEBUG_RATCHET_STATE(void *pBuff)
{
#ifndef FEATURE_ICM
   MSG_HIGH("OEMPriv_GetItem_CFGI_DEBUG_RATCHET_STATE: FEATURE_ICM not turned on",0,0,0);
   *(byte *)pBuff = 0;
   return EFAILED;
#else //FEATURE_ICM
#ifdef FEATURE_RF_ZIF
   ICM* pICM;
   boolean nReturnStatus;
   AEECMSysMode sys_mode;
   
   /* obtain shell instance */
   IShell *pIShell = AEE_GetShell();
   if (NULL == pIShell) {
    MSG_ERROR("OEMPriv_GetItem_CFGI_DEBUG_RATCHET_STATE: NULL IShell Pointer",0,0,0);
    *(byte *)pBuff = 0;
    return EFAILED;
   }
   
   /* Create the Call Manager object. */
   nReturnStatus = ISHELL_CreateInstance(pIShell,
                                         AEECLSID_CM,
                                         (void **) &pICM);
   if ((pICM == NULL) || (nReturnStatus != SUCCESS))
   {
     MSG_ERROR("OEMPriv_GetItem_CFGI_DEBUG_RATCHET_STATE: ICM object creation failed pICM: %d, nReturnStatus: %d",
               pICM,nReturnStatus,0);
     if (pICM != NULL)
     {
       (void) ICM_Release(pICM);
     }
     *(byte *)pBuff = 0;
     return EFAILED;
   }
   sys_mode = ICM_GetSysMode(pICM);
   (void) ICM_Release(pICM);

   if (sys_mode == AEECM_SYS_MODE_CDMA)
   {
#if defined(FEATURE_RFM_IOCTL)
     rfcom_ioctl_pointer_type rfm_ioctl_data;
     (void)rfm_ioctl( RFCOM_TRANSCEIVER_0, RFCOM_IOCTL_GET_DBG_SCR_RATCHET_STATE, &rfm_ioctl_data, 1);
     *(byte *)pBuff = (byte)(rfm_ioctl_data.uint8_field);
#else
     *(byte *)pBuff = (byte) rf_get_ratchet_state();
#endif
     return SUCCESS;
   }
#endif //FEATURE_RF_ZIF
   *(byte *)pBuff = 0;
   return EUNSUPPORTED;
#endif //FEATURE_ICM
}

#if defined(FEATURE_WCDMA)
#error code not present
#endif  // defined(FEATURE_WCDMA)

static int OEMPriv_GetItem_CFGI_CDMA_DYNAMIC_RANGE(void *pBuff)
{
#ifdef FEATURE_RF_ZIF
   nv_item_type nvi;

   if (NV_DONE_S != OEMNV_Get(NV_CDMA_DYNAMIC_RANGE_I, &nvi)) {
      return EFAILED;
   }
   *(short *)pBuff = nvi.cdma_dynamic_range;
   return SUCCESS;
#else
#error code not present
#endif
}



static int OEMPriv_GetItem_CFGI_CDMA_MIN_RX_RSSI(void *pBuff)
{
#ifdef FEATURE_RF_ZIF
   nv_item_type nvi;

   if (NV_DONE_S != OEMNV_Get(NV_CDMA_MIN_RX_RSSI_I, &nvi)) {
      return EFAILED;
   }

   *(short *)pBuff = nvi.cdma_min_rx_rssi;
   return SUCCESS;
#else
#error code not present
#endif
}

#ifdef FEATURE_FACTORY_TESTMODE
static int OEMPriv_GetItem_CFGI_FACTORY_TEST_MODE(void *pBuff)
{
   *(boolean *) pBuff = OEM_IsFactoryTestMode();
   return SUCCESS;
}

#endif /*FEATURE_FACTORY_TESTMODE*/

static int OEMPriv_SetItem_ReadOnly(void *pBuff)
{
   // This item is read-only
   return EBADPARM;
}

#ifdef T_MSM6500
static int OEMPriv_GetItem_CFGI_DEBUG_RF_CARD_NV_ID(void *pBuff)
{
  byte rf_card, nv_type;

#if defined(FEATURE_RFM_IOCTL)
  rfcom_ioctl_pointer_type rfm_ioctl_data_hw, rfm_ioctl_data_nv;
  rfm_ioctl( RFCOM_TRANSCEIVER_0, RFCOM_IOCTL_GET_DBG_SCR_SUPPORTED_HW, &rfm_ioctl_data_hw, 1);
  rfm_ioctl( RFCOM_TRANSCEIVER_0, RFCOM_IOCTL_GET_DBG_SCR_NV_BAND, &rfm_ioctl_data_nv, 1);
  rf_card = (byte)(rfm_ioctl_data_hw.uint8_field);
  nv_type = (byte)(rfm_ioctl_data_nv.uint8_field);
#else
  rf_card = rf_get_supported_hw();
  nv_type = rf_get_nv_band( RFCOM_TRANSCEIVER_0 );
#endif
  *(word *)pBuff = (rf_card << 8)|(nv_type & 0xFF); 
  return SUCCESS;
}

static int OEMPriv_GetItem_CFGI_DEBUG_THERM_BATT(void *pBuff)
{
   *(word *)pBuff = (byte) adc_read(ADC_BATT_THERM_DEGC);
   return SUCCESS;
}
static int OEMPriv_GetItem_CFGI_DEBUG_THERM_MSM(void *pBuff)
{
   *(word *)pBuff = (byte) adc_read(ADC_MSM_THERM_DEGC);
   return SUCCESS;
}
static int OEMPriv_GetItem_CFGI_DEBUG_THERM_PA(void *pBuff)
{
   *(word *)pBuff = (byte) adc_read(ADC_PA_THERM_DEGC);
   return SUCCESS;
}
static int OEMPriv_GetItem_CFGI_DEBUG_THERM_PMIC(void *pBuff)
{
   *(word *)pBuff = (byte) adc_read(ADC_PMIC_THERM_DEGC);
   return SUCCESS;
}
static int OEMPriv_GetItem_CFGI_DEBUG_THERM_CHRGR(void *pBuff)
{
   *(word *)pBuff = (byte) adc_read(ADC_CHG_THERM_DEGC);
   return SUCCESS;
}

#endif


static int OEMPriv_GetItem_CFGI_DEBUG_THERM(void *pBuff)
{
#if defined(FEATURE_RF_ZIF) || defined(FEATURE_WCDMA)
   *(word *)pBuff = (byte) therm_read();
   *(word *)pBuff = (*(word *)pBuff) << 8;
   *(word *)pBuff |= (((byte) adc_read(ADC_THERM)) & 0xFF);
   return SUCCESS;
#else
#error code not present
#endif
}

static int OEMPriv_GetItem_CFGI_DEBUG_BAND(void *pBuff)
{
#if defined(FEATURE_RFM_IOCTL)
  rfcom_ioctl_pointer_type rfm_ioctl_data;
  rfi_band_type band;
  if ( (rfm_ioctl( RFCOM_TRANSCEIVER_0, RFCOM_IOCTL_GET_SLEEP_STATE, &rfm_ioctl_data, 1)
        == RFCOM_IOCTL_NO_ERROR ) && (rfm_ioctl_data.uint8_field == TRUE) )
  {
    *(byte *)pBuff = CFGI_DEBUG_BAND_SLEEP;
  }
  else if( rfm_ioctl( RFCOM_TRANSCEIVER_0, RFCOM_IOCTL_GET_DBG_SCR_BAND_1X, &rfm_ioctl_data, 1) 
           == RFCOM_IOCTL_NO_ERROR )
  {
    band = (rfi_band_type)(rfm_ioctl_data.uint8_field);
    switch (band)
    {
    case RFI_GPS_BAND:
      *(byte *)pBuff = CFGI_DEBUG_BAND_GPS;
      break;
    default:
      *(byte *)pBuff = band;
      break;
    }
  }
  else if( rfm_ioctl( RFCOM_TRANSCEIVER_0, RFCOM_IOCTL_GET_DBG_SCR_BAND_GSM, &rfm_ioctl_data, 1) 
           == RFCOM_IOCTL_NO_ERROR )
  {
    *(byte *)pBuff = CFGI_DEBUG_BAND_GSM;
  }
  else if( rfm_ioctl( RFCOM_TRANSCEIVER_0, RFCOM_IOCTL_GET_DBG_SCR_BAND_WCDMA, &rfm_ioctl_data, 1) 
           == RFCOM_IOCTL_NO_ERROR )
  {
    *(byte *)pBuff = CFGI_DEBUG_BAND_WCDMA;
  }
  else
  {
    *(byte *)pBuff = 0;
    return EUNSUPPORTED;
  }
  return SUCCESS;

#elif defined(FEATURE_RF_ZIF)
#ifdef T_MSM6500
  /* Use RF API to determine RF state */
  rf_card_band_type band;
  word chan;

  if ( rf_is_sleeping ( RFCOM_TRANSCEIVER_0 )) 
  {
     *(byte *)pBuff = CFGI_DEBUG_BAND_SLEEP;
  }
  else 
  {
     rf_get_band_chan( &band, &chan );

     switch ( band )
     {
     case RF_GPS_BAND:
       *(byte *)pBuff = CFGI_DEBUG_BAND_GPS;
       break;
     case RF_FM_BAND:
       *(byte *)pBuff = CFGI_DEBUG_BAND_AMPS;
       break;
     default:
       /* assign the RF band class */
       *(byte *)pBuff = band;
       break;
     }
  } // rf_is_sleeping check
#else  // !T_MSM6500
#error code not present
#endif //T_MSM6500
  return SUCCESS;

#elif defined(FEATURE_GSM) || defined(FEATURE_WCDMA)
#error code not present
#else  // !defined(FEATURE_GSM) || defined(FEATURE_WCDMA)
#error code not present
#endif //FEATURE_RF_ZIF
}


#ifdef FEATURE_HDR
#error code not present
#endif /* FEATURE_HDR */

#ifdef FEATURE_QIPCALL
static int OEMPriv_GetItem_CFGI_QIPCALL_DEBUG_EVDO_IP_ADDR(void *pBuff)
{
   qipcall_debug_screen_type   qipcall_debug_info;

   qipcall_get_debug_display(&qipcall_debug_info);

   *(uint32 *) pBuff = qipcall_debug_info.evdo_ip_addr;

   return SUCCESS;
}

static int OEMPriv_GetItem_CFGI_QIPCALL_DEBUG_EVDO_SIP_REG_STATUS(void *pBuff)
{
   qipcall_debug_screen_type   qipcall_debug_info;

   qipcall_get_debug_display(&qipcall_debug_info);

   *(byte *) pBuff = qipcall_debug_info.evdo_sip_reg_status;

   return SUCCESS;
}

static int OEMPriv_GetItem_CFGI_QIPCALL_DEBUG_WLAN_IP_ADDR(void *pBuff)
{
   qipcall_debug_screen_type   qipcall_debug_info;

   qipcall_get_debug_display(&qipcall_debug_info);

   *(uint32 *) pBuff = qipcall_debug_info.wlan_ip_addr;

   return SUCCESS;
}

static int OEMPriv_GetItem_CFGI_QIPCALL_DEBUG_WLAN_SIP_REG_STATUS(void *pBuff)
{
   qipcall_debug_screen_type   qipcall_debug_info;

   qipcall_get_debug_display(&qipcall_debug_info);

   *(byte *) pBuff = qipcall_debug_info.wlan_sip_reg_status;

   return SUCCESS;
}
#endif

#if defined(FEATURE_GSM) || defined (FEATURE_WCDMA)
#error code not present
#endif /* defined(FEATURE_GSM) || defined (FEATURE_WCDMA) */

#ifdef FEATURE_INTELLICEIVER
static int OEMPriv_GetItem_CFGI_DEBUG_INTELLICEIVER_STATE(void *pBuff)
{
#if defined(FEATURE_RFM_IOCTL)
  rfcom_ioctl_pointer_type rfm_ioctl_data;
  (void)rfm_ioctl( RFCOM_TRANSCEIVER_0, 
                   RFCOM_IOCTL_GET_DBG_SCR_INTELLICEIVER_POWER_MODE, 
                   &rfm_ioctl_data, 
                   1); 
  *(byte *)pBuff = (byte)(rfm_ioctl_data.uint8_field);

#else
  *(byte *)pBuff = (byte)rf_intelliceiver_get_power_mode( RF_PATH_0 );
#endif
  
  return SUCCESS;
}
#endif /* FEATURE_INTELLICEIVER */

#if defined(FEATURE_USRSYSCLOCK) || defined(FEATURE_ISYSCLOCK)
static int OEMPriv_GetItem_CFGI_AUTO_TIME_ENABLE(void *pBuff)
{
   *(boolean *) pBuff = nvi_cache.auto_time_enable;
   return SUCCESS;
}

static int OEMPriv_SetItem_CFGI_AUTO_TIME_ENABLE(void *pBuff)
{
   nv_item_type nvi;

   if (nvi_cache.auto_time_enable != *(boolean *)pBuff) {
      nvi_cache.auto_time_enable = *(boolean *)pBuff;
      nvi.auto_time_enable = nvi_cache.auto_time_enable;
      (void) OEMNV_Put(NV_AUTO_TIME_ENABLE_I, &nvi);
   }
   return SUCCESS;
}
#endif 

static int OEMPriv_GetItem_CFGI_DISABLE_IN_CALL_DISP(void *pBuff)
{
   *(boolean *)pBuff = oemi_cache.disable_in_call_disp;
   return SUCCESS;
}

//lint -save -esym(818, pBuff)  Suppress "could be declared as const pointer"
static int OEMPriv_SetItem_CFGI_DISABLE_IN_CALL_DISP(void *pBuff)
{

   oemi_cache.disable_in_call_disp = *(boolean *)pBuff;
   return SUCCESS;
}
//lint -restore

static int OEMPriv_GetItem_CFGI_DISABLE_BG_IMAGE(void *pBuff)
{
   *(boolean *)pBuff = oemi_cache.disable_bg_image;
   return SUCCESS;
}

//lint -save -esym(818, pBuff)  Suppress "could be declared as const pointer"
static int OEMPriv_SetItem_CFGI_DISABLE_BG_IMAGE(void *pBuff)
{
   oemi_cache.disable_bg_image = *(boolean *)pBuff;
   return SUCCESS;
}
//lint -restore

static int OEMPriv_GetItem_CFGI_VOICEPRIVACY(void *pBuff)
{
  *(uint8 *) pBuff = nvi_cache.voice_priv;
   return SUCCESS;
}

//lint -save -esym(818, pBuff)  Suppress "could be declared as const pointer"
static int OEMPriv_SetItem_CFGI_VOICEPRIVACY(void *pBuff)
{
  nv_item_type nvi;

   if (nvi_cache.voice_priv != *(uint8 *)pBuff) {
      nvi_cache.voice_priv = *(uint8 *)pBuff;
      nvi.voice_priv = nvi_cache.voice_priv;
      (void) OEMNV_Put(NV_VOICE_PRIV_I, &nvi);
   }
   return SUCCESS;
}
//lint -restore

static int OEMPriv_GetItem_CFGI_MANUAL_PLMN_SEL_ALLOWED(void *pBuff)
{
   *(boolean *)pBuff = oemi_cache.manual_plmn_sel_allowed;
   return SUCCESS;
}

//lint -save -esym(818, pBuff)  Suppress "could be declared as const pointer"
static int OEMPriv_SetItem_CFGI_MANUAL_PLMN_SEL_ALLOWED(void *pBuff)
{
   oemi_cache.manual_plmn_sel_allowed = *(boolean *)pBuff;
   return SUCCESS;
}

//lint -restore

