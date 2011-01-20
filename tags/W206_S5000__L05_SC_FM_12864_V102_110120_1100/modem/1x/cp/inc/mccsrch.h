#ifndef MCCSRCH_H
#define MCCSRCH_H
/*===========================================================================

                M C C S R C H   H E A D E R    F I L E

DESCRIPTION
  This file contains all of the definitions necessary to interface between
  the Main Control CDMA subtask and the Searcher task.

Copyright (c) 1991-2005 by QUALCOMM, Incorporated.  All Rights Reserved.

===========================================================================*/

/*===========================================================================

                      EDIT HISTORY FOR FILE

This section contains comments describing changes made to this file.
Notice that changes are listed in reverse chronological order.


$PVCSPath: L:/src/asw/MSM5100/CP_REL_A/vcs/mccsrch.h_v   1.11   01 Oct 2002 16:42:30   azafer  $
$Header: //source/qcom/qct/modem/1x/cp/rel/1h08/inc/mccsrch.h#1 $ $DateTime: 2009/05/29 03:54:56 $ $Author: prabhuk $

when       who     what, where, why
--------   ---     ----------------------------------------------------------
04/08/07   pg      Featurization and general code cleanup for memory reduction
                   effort.
11/21/06   sb      Added support for VOIP-1X handoffs.
09/25/06   fc      UMTS changes.
08/09/06   fc      Renamed mcc_srch_pseudo_idle_type to mcc_srch_idle_mode_type.
07/10/06   fc      Renamed MCC_SRCH_PSEUDO_IDLE_FOR_UMTS_HO to
                   MCC_SRCH_PSEUDO_IDLE_ACTIVE.
07/06/06   fc      Renamed SRCH_LOST_RF_RESOURCE_R to 
                   SRCH_REQUEST_SYS_RESTART_R.
06/29/06   fc      Added enum mcc_srch_pseudo_idle_type.
05/18/06   fc      Added new report SRCH_LOST_RF_RESOURCE_R.
02/17/06   fc      Added the new field require_system_exit to the payload of
                   SRCH_RAFAIL_R.
07/24/05   fh      Renamed SRCH_FIND_STRONGEST_PILOT_F, SRCH_STRONG_PILOT_F
                   and related report names, data types, and data structures           
03/28/05   fc      Added support for DCA.
03/23/05   fc      Merged the following from MSMSHARED:
03/14/05   an       Removed featurization around mccsrch_strong_pilot_info_type 
                    and mccsrch_strong_plt_rpt_type 
12/16/04   an      Removed mcc_srch_get_nbr_config()
                   Added mcc_srch_lookup_nghbr_info()
12/06/04   kwo     Added SRCH_ACCESS_READY_R and SRCH_STRONGEST_PILOT_MEAS_R
10/20/04   an      Replaced inclusion of srch.h with srchmc.h
09/27/04   va      Merged from REL B/C archive:
08/19/04   yll      Clean up the MC/SRCH interface.
08/02/04   yll      Added the function to allow Search to set signals for MC
                    when sending Search reports.
06/30/04   dna      Changes for FEATURE_SRCH_INACTIVE_STATE.
                    This feature adds SRCH_DEACTIVATE_DONE_R, to allow for
                    mmoc_rpt_prot_auto_deactd_ind() to be called only after
                    SRCH has really released the RF and other resources.
08/20/04   bkm     Lint cleanup.
06/03/04   bkm     Merged the following from IS2000_REL_B_C:
  06/02/04   sb      Changed pilot_pn in mccsrch_strong_pilot_info_type to be
                     unsigned (to make lint happy); pilots can't be negative.
01/13/04   bkm     Moved defs of switch report types.
01/05/04   bkm     Added switch reports to mccsrch_report_code_type.
11/19/03   fc      Added mcc_srch_get_nbr_config.
09/30/03   bkm     Changed Ec/Io parameter name in pc_meas and raho rpt and
                   mcc_srch_fast_raho().
09/29/03   bkm     Added Ec/Io parameters to mccsrch_raho_rpt_type and
                   mcc_srch_fast_raho() to support event reporting.
08/11/03   bkm     Added Ec/Io parameters to mccsrch_pc_meas_rpt_type.
08/11/03   bkm     Removed support for FEATURE_SPECIAL_MDR.
08/11/03   az      Added support for FEATURE_MODIFIED_SCI
08/06/03   sb      Added SRCH's estimate of off-time to GPS Visit Ready.
07/23/03   bkm     Mainlined FEATURE_ANTIREGISTRATION_BIAS and 
                   FEATURE_ENHANCED_STANDBY_III.
04/14/03   va      Added support for "strong_pilot" report from SRCH
01/10/03   fc      Featurized page match changes with FEATURE_PAGE_MATCH_REQUEST.
12/02/02   fc      Added page_match to mccsrch_wakeup_rpt_type.
10/01/02   az      Mainlined FEATURE_IS2000.
09/30/02   sb      Mainlined FEATURE_SD20.
06/13/02   yll     Changed the BCSMS feature name to be under Rel A CC.
04/10/02   yll     Added Broadcast SMS support on IS2000 RelA common channel.
03/11/02   va      Merged from common archive.
                   Updates in the functionality to perform GPS operation while MS
                   is in Acquisition State (for performing multi-visit operation).
02/22/02   fc      Added support for Release A common channels.
02/12/02   yll     Merged from the common archive CP VU 116.
                   01/30/02   abh
                   Added code to go into GPS state when Phone is in Acquisition state.
08/16/01   kk      Merge from mainline.
                   08/10/01   lcc     Changed mcc_srch_fast_raho to return a boolean.
02/26/01 mai     Changed FEATURE_GPSONE to FEATURE_GPS_MODE to move code needed
                   only for GPS part of position location to a new #define.
  02/16/01 fc      Added support for system determination 2.0. All changes
                   are featurized under FEATURE_SD20.
02/27/01   va      Added pilot rec types in CFS, PPM, tc_meas and
                   SCRM reports from SRCH.
09/08/00   jq      Added QPCH support in sleep state.
07/06/00   yll     Added GPS support
06/20/00   fc      Added SRCH_SCRM_MEAS_R and mccsrch_scrm_meas_rpt_type to
                   support SCRM pilot strength measurement.
05/26/00   jq      Added cur_slot_mask into mccsrch_wakeup_rpt_type
03/31/00   ry      Featurized mccsrch_get_aset() under FEATURE_IS2000
03/31/00   ry      Removed FEATURE_SRCH_DYNAMIC_BAND_CLASS featurization since
                   the feature will always be on
03/29/00   ry      Featurized the nbr_band parameter of mcc_srch_nbr_checking()
                   under FEATURE_SRCH_DYNAMIC_BAND_CLASS
           ry      Added the prototype mccsrch_get_aset() - it was deleted in
                   the previous merge
03/13/00   ry      Merged the following from PP (MSM3100_CP.03.00.10):
           ks      Removed the feature ENHANCED_STANDBY_II. Also added
                   mcc_srch_min_bs_p_rev function for ENHANCED_STANDBY_III.
           ck      Added the band class information to some SRCH reports and
                   mcc_srch_nbr_checking function.
02/22/00   jcw     Merged in the following change:
           cah     (ajn)Added a vaild time field for Wakeup Report.
11/15/99   va      IS2000 Changes:
                   Support for "SRCH_AT_R" to send a report when HO action time
                   has arrived.
08/24/99   fc      Added num_pilots to mccsrch_pilots_meas_rpt_type to allow
                   SRCH providing the count of pilots in Pilot Strength
                   Measurement report.
08/09/99   ry      Combined FEATURE_IS95B_HARD_HO and FEATURE_IS95B_CFS into
                   FEATURE_IS95B_MAHHO
           ry      Added struct definitions for SRCH_CFS_READY_R and
                   SRCH_CF_MEAS_DONE_R
           ry      Added CFS sequence number to mccsrch_cfnset_meas_rpt_type
07/09/99   ry      Added the SRCH_RIF_HHO_FAILURE_R, SRCH_RIF_HHO_ABORT_R, and
                   SRCH_CF_NSET_R reports
07/08/99   ych     Added support for processing Periodic Pilot Measurment
                   Message
05/14/99   kmp     Merged in the changes from the IS-95B Odie baseline
           kmp     Merged the changes listed below from the TGP IS-95B baseline.
           fc      Renamed all typedef names defined for pilot reporting to
                   end with _type. Renamed all typedef names defined for
                   pilot reporting which started with ACC_HO or ACCESS_HO to
                   AHO.
           kmp     Merged the changes listed below from the TGP IS-95B baseline.
           fc      Put in changes based on code review input.
           fc      Added ACCESS_HO_ALLOWED and ACCESS_ATTEMPTED in
                   MCCSRCH_PILOTS_MEAS_RPT_TYPE to support pilot reporting
                   performance improvement.
           kmp     merged in IS-95B changes, ported IS-95B changes to ARM,
           fc      Changed the size of the Pilot Strength Measurement report.
           fc      Updated comments in mccsrch_report_code_type and
                   mccsrch_pilots_meas_rpt_type for pilot reporting.
           ks      Updated mccsrch_pilots_meas_rpt_type for pilot reporting.
           lh      FEATURE_JSTD008 replaced with run-time P_REV and Band
                   Class checking.
           fc      Added SRCH_AHO_PLIST_R report type and
                   MCCSRCH_PILOTS_MEAS_RPT_TYPE structure for reporting
                   the Nset pilot strength to MC.
           ks      Moved the SRCH_THRESH_R before the SRCH_MAX_R report.
           ks      Added SRCH_THRESH_R report for reporting to MC.
02/26/99   ejv     Added ajn's timestamp SRCH_TRYSLEEP_R message.  Also merged
                   FEATURE_ANTIREGISTRATION_BIAS function prototypes.
09/14/98   ck      Added new report code type SRCH_TRYSLEEP_R
09/04/98   pms     Neared the variables to save ROM space.
06/27/97   rdh     Added SRCH_FING_DONE_R status reporting to MC.
05/15/96   dna     Added freq and band class fields for PCS
06/28/95   rdh     Added rude awakening status field to wakeup report.
04/28/95   rdh     Added slot_cycle_index field to WAKEUP_R command field.
03/30/95   rdh     Eliminated slam bug catch fields. Fixed in Search task.
03/23/95   rdh     Added SRCH_WAKEUP_R for deep sleep.
11/01/94   rdh     Added PC_ASSIGN command structure gotten from Don.
10/24/94   rdh     Added status field to slew command for 9600 baud paging
                   channel MSM bug workaround.
02/19/94   jah     Deleted unused commands awake SRCH_WAKEUP_R & SRCH_AWAKE_R,
                   and the associated union members.  Changed RAHO pilot to
                   an int2 to match others.
07/16/92   ptw     Created file.

===========================================================================*/

#include "srchmc.h"

/*===========================================================================

                        DATA DECLARATIONS

===========================================================================*/

/*--------------------------------------------------------------------------
                      SEARCHER TASK REPORT TYPES

--------------------------------------------------------------------------*/

/*--------------------------------------------------------------------------
                           SEARCH REPORT CODES

The following codes are for use with the 'rpt' field of the search report
header type defined below.
--------------------------------------------------------------------------*/

typedef enum
{
  SRCH_ACQ_R,                   /* Aquisition report. */
  SRCH_SLEW_R,                  /* Slew complete report. */
  SRCH_UNSLEW_R,                /* Unslew complete report. */
  SRCH_PC_MEAS_R,               /* Pilot report for paging channel. */
  SRCH_TC_MEAS_R,               /* Pilot Strength measurement report. */
  SRCH_REACQ_R,                 /* Fully reacquired after sleep report */
  SRCH_RAHO_R,                  /* Reacquired, but idle HandOff required report */
  SRCH_RAFAIL_R,                /* Reacquisition after sleep failed */
  SRCH_ACQFAIL_R,               /* Acquisition failed */
  SRCH_PC_ASSIGN_R,             /* Report strongest pilot in paging channel assign msg */
  SRCH_WAKEUP_R,                /* Report wakeup from sleep */
  SRCH_FING_DONE_R,             /* Report finger assignment completed after handoff */
  SRCH_TRYSLEEP_R,              /* Report we might be able to try to sleep again. */
  SRCH_ACCESS_READY_R,          /* Report access resources availability */
  SRCH_IDLE_STRONG_PILOT_MEAS_R,/* Report the best pilot */
  SRCH_SYS_MEAS_R,              /* Results from a System Measurement request */
  SRCH_THRESH_R,                /* EC Threshold Report from SRCH to MC */
  SRCH_AHO_PLIST_R,             /* Nset Pilot Strength Measurement report */
  SRCH_NO_SECTOR_R,             /* No reference sector */
  SRCH_FAST_SLEEP_R,            /* SRCH rxed a fast-sleep command and is sleeping */
  SRCH_FAST_WAKE_R,             /* SRCH rxed a fast-sleep command and is waking up */
  SRCH_PPM_R,                   /* Periodic Pilot Measurement report */
  
  SRCH_RIF_HHO_FAILURE_R,       /* Hard handoff failure report */
  SRCH_RIF_HHO_ABORT_R,         /* Hard handoff abort report */
  SRCH_CF_NSET_R,               /* Candidate Frequency Search Report */
  SRCH_CFS_READY_R,             /* Ready to do CFS */
  SRCH_CF_MEAS_DONE_R,          /* CF meas done */
  SRCH_GPS_VISIT_READY_R,
  SRCH_GPS_VISIT_DONE_R,
  
  SRCH_AT_R,                    /* Sent at AT of HO/ESCAM etc. sent in SRCH context in MC file */
  SRCH_SCRM_MEAS_R,             /* SCRM pilot strength measurements report */
  SRCH_QPCH_REACQ_R,            /* SRCH have doen QPCH REACQ and possibly gone to sleep */
  SRCH_T_ADD_ABORT_R,           /* T_ADD_ABORT report */
  SRCH_ACQ_ABORT_GPS_R,
  SRCH_DEACTIVATE_DONE_R,

  #ifdef FEATURE_IS2000_REL_B
  SRCH_TC_STRONG_PILOTS_MEAS_R, /* Report the list of strong pilots for TC to idle */
  #endif /* FEATURE_IS2000_REL_B */

  #ifdef FEATURE_IS2000_REL_C_DV
#error code not present
  #endif /* FEATURE_IS2000_REL_C_DV */

  #ifdef FEATURE_IS2000_REL_D
#error code not present
  #endif /* FEATURE_IS2000_REL_D */
 
  SRCH_REQUEST_SYS_RESTART_R,   /* SRCH request system restart */
  
  SRCH_TT_R,                    /* SRCH acquired the timing */
  SRCH_TT_FAILED_R,             /* SRCH failed to acquire the timing */

  SRCH_MAX_R         /* Gives number of report codes. MUST be last enum item. */

} mccsrch_report_code_type;

typedef struct
{
  cmd_hdr_type            rpt_hdr;
    /* Includes the queue links,  pointer to requesting task TCB,
       signal to set on completion,  and done queue. */

  mccsrch_report_code_type   rpt;
    /* Specifies which type of report this is. See enumerated report
       codes above. */

} mccsrch_rpt_hdr_type;


/*--------------------------------------------------------------------------
                         ACQUISITION REPORT

--------------------------------------------------------------------------*/

typedef struct
{
  mccsrch_rpt_hdr_type  hdr;

  /* !!! More fields to come !!! */
}
mccsrch_acq_rpt_type;


/*--------------------------------------------------------------------------
                     SLEW TO SYSTEM TIMING COMPLETE REPORT

--------------------------------------------------------------------------*/

typedef struct
{
  mccsrch_rpt_hdr_type  hdr;

  /* !!! More fields to come !!! */
}
mccsrch_slew_rpt_type;


/*--------------------------------------------------------------------------
                    UNSLEW TO SYNC TIMING COMPLETE REPORT

--------------------------------------------------------------------------*/

typedef struct
{
  mccsrch_rpt_hdr_type  hdr;

  /* !!! More fields to come !!! */
}
mccsrch_unslew_rpt_type;


/*--------------------------------------------------------------------------
            PAGING CHANNEL PILOT STRENGTH MEASUREMENT REPORT

--------------------------------------------------------------------------*/

typedef struct
{
  mccsrch_rpt_hdr_type  hdr;

  int2               pilot;
/* Removed FEATRUE_JSTD008 */
  byte               band_class;
  int2               freq;
  byte               ecio_old_pilot;   /* Ec/Io of old pilot */
  byte               ecio_new_pilot;   /* Ec/Io of new pilot */
}
mccsrch_pc_meas_rpt_type;


/*--------------------------------------------------------------------------
            TRAFFIC CHANNEL PILOT STRENGTH MEASUREMENT REPORT

--------------------------------------------------------------------------*/

typedef struct
{
  mccsrch_rpt_hdr_type  hdr;

  int2               ref_pilot;
  byte               ecio;
  byte               keep;

  word               meas_cnt;
    /* Number of pilot measurements in list. */
  byte               sf_rx_pwr;   /* received power spectral density */
  /* Record type and type specific fields for the ref pilot */
  cai_pilot_rec_type  ref_pilot_rec_type;
  caii_add_pilot_rec_type  ref_pilot_rec;
  struct
  {
    word  pn_phase;
    byte  ecio;
    byte  keep;
    /* Record type and type specific fields for this pilot */
    cai_pilot_rec_type  pilot_rec_type;
    caii_add_pilot_rec_type  pilot_rec;
  } meas[ SRCH_NSET_MAX + SRCH_ASET_MAX - 1 ];

}
mccsrch_tc_meas_rpt_type;

/*--------------------------------------------------------------------------
            Active Pilot and Nset Pilot Strength Measurement Report
--------------------------------------------------------------------------*/

typedef struct
{
  byte    pilot_strength;
  int2    pilot_pn;
  int2    pilot_pn_phase;
  boolean access_ho_allowed;
  boolean access_attempted;
  byte    band_class;
  int2    freq;
} pilot_rpt_list_type;

typedef struct
{
  mccsrch_rpt_hdr_type hdr;
  word num_pilots;
    /* Number of pilots reported by SRCH excluding Active pilot. This value
       can be different from the size of MC neighbor record in case of
       duplicate pilots in Neigbor List Message */
  pilot_rpt_list_type  pilot_list[ CAI_N8M + 1 ];
}
mccsrch_pilots_meas_rpt_type;

typedef struct
{
 mccsrch_rpt_hdr_type hdr;

 byte   total_rx_pwr;   /* Total received power on the TF */
 word   pilot_cnt;              /* Number of pilots included in this report */

 struct
 {
   word pilot;
   byte ecio;
 } pilot_rec[ SRCH_ASET_MAX ];
}
mccsrch_hho_fail_rpt_type;

typedef enum
{
  DIFF_RX_PWR_TOO_LOW,

  TOTAL_PILOT_EC_IO_TOO_LOW,

  FAIL_REASON_MAX
}
hho_fail_enum_type;

typedef struct
{
  mccsrch_rpt_hdr_type hdr;

  hho_fail_enum_type    hho_fail_reason;

}
mccsrch_hho_abort_rpt_type;

typedef struct
{
   mccsrch_rpt_hdr_type  hdr;  /* Common header */
   byte cfs_seq;
     /* The sequence number for the current search period */

}mccsrch_cfs_meas_done_rpt_type;

typedef struct
{
    mccsrch_rpt_hdr_type  hdr;  /* Common header */
    byte cfs_seq;
      /* The sequence number for the current search period */

} mccsrch_cfs_ready_rpt_type;

/*--------------------------------------------------------------------------
                  CANDIDATE FREQUENCY SEARCH COMPLETE REPORT

This report is sent when we have finished a Candidate Frequency search
--------------------------------------------------------------------------*/
typedef struct
{
    mccsrch_rpt_hdr_type  hdr;  /* Common header */
    byte cfs_seq;
      /* The sequence number for the current search period */

    byte  total_rx_pwr;    /*   CF total received power */

    word  pilot_cnt;
      /* Number of pilot shall be measured this time */
    struct
    {
        word  pilot;   /* Pilot offset */
        byte  ecio;
        cai_pilot_rec_type  pilot_rec_type;
        caii_add_pilot_rec_type  pilot_rec;
    } pilot_rec[SRCH_CFNSET_MAX];
} mccsrch_cfnset_meas_rpt_type;

/*--------------------------------------------------------------------------
                  GPS VISIT READY REQUEST AND GPS VISIT DONE REPORT
--------------------------------------------------------------------------*/
#ifdef FEATURE_IS2000_REL_B
/* The estimate of how long the GPS visit will take is used in the CDMA
   Off Time Report Message (section 2.7.2.3.2.40).  The units should comply
   with the CDMA_OFF_TIME_UNIT field in this message.
*/
typedef enum
{
  MCC_UNIT_80_MS,

  MCC_UNIT_HALF_A_SECOND,

  MCC_UNIT_ONE_SECOND
}
mcc_off_time_unit_enum_type;
#endif /* FEATURE_IS2000_REL_B */

typedef struct
{
    mccsrch_rpt_hdr_type  hdr;  /* Common header */

#ifdef FEATURE_IS2000_REL_B
  mcc_off_time_unit_enum_type  off_time_unit;       /* Unit of off_time_estimate */ 
  byte                         off_time_estimate;   /* Estimate of the search time*/
#endif /* FEATURE_IS2000_REL_B */

} mccsrch_gps_visit_ready_rpt_type;

typedef struct
{
    mccsrch_rpt_hdr_type  hdr;  /* Common header */

} mccsrch_gps_visit_done_rpt_type;

/*--------------------------------------------------------------------------
                  SYSTEM MEASUREMENT RESULTS REPORT

This report is sent when Search has finished a System Measurement request
--------------------------------------------------------------------------*/

typedef struct
{
  word rx_pwr;
    /* The Rx power of the largest channel in the requested member */

  word chan;
    /* The largest AMPS channel in the requested member */
}
mccsrch_meas_amps_rpt_type;

typedef struct
{
  word rx_pwr;
    /* The Rx power of the CDMA channel requested */

  word ecio;
    /* The strongest time offset's Ec/Io in the CDMA channel requested */
}
mccsrch_meas_cdma_rpt_type;

typedef struct
{
  mccsrch_rpt_hdr_type  hdr;
    /* Report header */

  word meas_id;
    /* Measurement ID */

  srch_meas_act_enum_type  action;
    /* Feedback on the action completed */

  word  list_cnt;
    /* Number of systems in the list below */

  struct
  {
    srch_meas_mem_enum_type  member_type;
      /* What type of member is this? */

    union
    {
      mccsrch_meas_amps_rpt_type  amps_results;
        /* AMPS system particulars */

      mccsrch_meas_cdma_rpt_type  cdma_results;
        /* CDMA system particulars */

      /* The following are included so that
         Search may use a report buffer to
         hold member info and intermediate
         results while performing measurement
         operations. This saves memory */

      srch_meas_amps_sys_type  amps_member;
        /* AMPS system particulars */

      srch_meas_cdma_sys_type  cdma_member;
        /* CDMA system particulars */
    }
    member;
  }
  meas_list[ MAX_SYS_MEAS_LIST_SIZ ];
    /* List of members to perform system measurements on */
} mccsrch_sys_meas_rpt_type;

/*--------------------------------------------------------------------------
                     SLEEP STATE REACQUIRED REPORT

This report is sent when the channel has been reacquired after a
period of sleep.  The sleep task should then be commanded to go to
the Paging Channel state to resume normal processing.
--------------------------------------------------------------------------*/

typedef struct
{
  mccsrch_rpt_hdr_type  hdr;

  /* !!! More fields to come !!! */
}
mccsrch_reacq_rpt_type;

typedef struct
{
  mccsrch_rpt_hdr_type  hdr;

  /* !!! More fields to come !!! */
}
mccsrch_qpch_reacq_rpt_type;

/*--------------------------------------------------------------------------
         SLEEP STATE REACQUIRED, IDLE HANDOFF REQUIRED REPORT

This report is sent when we have reacquired a neighbor set member
after a period of sleep.
--------------------------------------------------------------------------*/

typedef struct
{
  mccsrch_rpt_hdr_type  hdr;

  int2               pilot;
      /* The pilot offset of the reacquired pilot */
/* Removed FEATRUE_JSTD008 */
  byte               band_class;
  int2               freq;
      /* The frequency of the reacquired pilot */

  byte               ecio_old_pilot;   /* Ec/Io of old pilot */
  byte               ecio_new_pilot;   /* Ec/Io of new pilot */
}
mccsrch_raho_rpt_type;

/*--------------------------------------------------------------------------
               SLEEP STATE REACQUISITION FAILED REPORT

This report is sent when we have failed to reacquire the channel after a
period of sleep.
--------------------------------------------------------------------------*/

typedef struct
{
  mccsrch_rpt_hdr_type hdr;

  boolean require_system_exit;
    /* If TRUE, MS will perform system lost exit */
}
mccsrch_rafail_rpt_type;

/*--------------------------------------------------------------------------
                         ACQUISITION FAILED REPORT

--------------------------------------------------------------------------*/

typedef struct
{
  mccsrch_rpt_hdr_type  hdr;

  /* !!! More fields to come !!! */
}
mccsrch_acq_fail_type;

/*--------------------------------------------------------------------------
              PAGING CHANNEL ASSIGNMENT SEARCH COMPLETE REPORT
--------------------------------------------------------------------------*/

typedef struct
{
  mccsrch_rpt_hdr_type  hdr; /* New report type SRCH_PC_ASSIGN_R */

  int2  pilot; /* Strongest pilot in the list given in the pc_assign_cmd */
}
mccsrch_pc_assign_rpt_type;

/*--------------------------------------------------------------------------
                     SLEEP STATE WAKEUP REPORT

This report is sent when we have fully awakened from a period of deep sleep.
--------------------------------------------------------------------------*/

typedef struct
{
  mccsrch_rpt_hdr_type  hdr;

  int8  slot_cycle_index;
    /* Slot cycle index used in latest sleep period */

  boolean  rude_awakening;
    /* If TRUE, Search was rudely awakened */

  boolean  srch_slept;
    /* Set to TRUE if the wakeup report follows search actually sleeping,
       set to FALSE if we didn't actually get to sleep */

  qword    valid_time;
    /* Time stamp of when "Wakeup" officially occurs at, in 20ms */

  byte cur_slot_mask;
    /* the current slot mask SRCH is waking up to  */

#ifdef FEATURE_PAGE_MATCH_REQUEST
  boolean page_match;
    /* If TRUE, quick page match is expected to be performed in MUX */
#endif /* FEATURE_PAGE_MATCH_REQUEST */

  byte bcn[CAI_MAX_NON_PRI_BCCH_MONITOR];
    /* The broadcast control channel number which SRCH is waking up to for
       broadcast data. 0 marks the end of the valid bcn. */
}
mccsrch_wakeup_rpt_type;

/*--------------------------------------------------------------------------
                  FINGER ASSIGNMENT COMPLETED REPORT

This report is sent when we have assigned demodulation fingers after handoff
--------------------------------------------------------------------------*/

typedef struct
{
  mccsrch_rpt_hdr_type  hdr;
    /* Report header only */
}
mccsrch_fing_done_rpt_type;

/*--------------------------------------------------------------------------
                       STRONGEST PILOTS REPORT

This report is sent from SRCH to give MC a list of strong pilots
--------------------------------------------------------------------------*/
typedef struct
{
  uint16                   pilot_pn;
  uint32                   eng;
  cai_pilot_rec_type       pilot_rec_type;
  caii_add_pilot_rec_type  pilot_rec;
}
mccsrch_tc_strong_pilot_info_type;

typedef struct
{
  mccsrch_rpt_hdr_type  hdr;

  byte band_class;
  word cdma_freq;

  byte num_pilots;

  mccsrch_tc_strong_pilot_info_type pilot[SRCH_CSET_MAX + SRCH_ASET_MAX];
/*  struct
  {
    int2                     pilot_pn;
    uint32                   eng;
    cai_pilot_rec_type       pilot_rec_type;
    caii_add_pilot_rec_type  pilot_rec;
  }pilot[SRCH_CSET_MAX + SRCH_ASET_MAX];*/

}mccsrch_tc_strong_plts_rpt_type;

#ifdef FEATURE_IS2000_REL_D
#error code not present
#endif /* FEATURE_IS2000_REL_D */
 
typedef struct
{
  mccsrch_rpt_hdr_type  hdr;
  int2               ref_plt;     /* Time reference PN offset */
  byte               ecio;     /* Pilot Strength */
  byte               keep;        /* Keep pilot indicator */
  byte               sf_rx_pwr;   /* received power spectral density */
  byte               meas_cnt;   /* Number of pilot measurements */

  /* num_pilot occurrences of the following record */
  struct
  {
    word  pn_phase;
    byte  ecio;
    byte  keep;
    cai_pilot_rec_type  pilot_rec_type;
    caii_add_pilot_rec_type  pilot_rec;
  }meas[SRCH_CSET_MAX + SRCH_ASET_MAX - 1];

}mccsrch_ppm_rpt_type;

/*--------------------------------------------------------------------------
                  OK TO RETRY A VETOED SLEEP REPORT

This report is sent when conditions have changed and a previously vetoed
sleep command may be retried.
--------------------------------------------------------------------------*/

typedef struct
{
  mccsrch_rpt_hdr_type  hdr;
    /* Report header only */

  qword rpt_time;
    /* Time at which report is generated.  Allows MCC to ignore report
       lingering in queues during sleep. */
}
mccsrch_trysleep_rpt_type;

/*--------------------------------------------------------------------------
                    ACCESS READY REPORT

This report is sent response to an ACCESS_F to report the resources 
available for access.   
--------------------------------------------------------------------------*/
typedef struct
{
  mccsrch_rpt_hdr_type  hdr;
    /* Report header only */

  boolean acquired_rf_resources;  /* Acquired the necessary RF resources to
                                     perform an access attempt. */
} mccsrch_access_ready_rpt_type;

/*--------------------------------------------------------------------------
                               AT REPORT
--------------------------------------------------------------------------*/
typedef struct
{
  mccsrch_rpt_hdr_type  hdr;
  uint16             strongest_pilot;
  uint8              strongest_band_class;
  uint16             strongest_freq;
  uint16             strongest_ecio; /* Ec/Io of strongest pilot in 
                                        1/8 db (0 - -31.875 dB) */
  uint16             active_pilot;
  uint16             active_ecio;    /* Ec/Io of active pilot in
                                        1/8 db (0 - -31.875 dB) */
} mccsrch_idle_strong_pilot_meas_rpt_type;

/* This report is used to indicate to MC that AT for HO/ESCAM has arrived
   This rpt is sent in the srch context but by the function that srch calls at
   action time to get the active set */
typedef struct
{
  mccsrch_rpt_hdr_type  hdr;
    /* Report header only */
  byte transaction_num;
    /* trans num of the HO/ESCAM transaction */

} mccsrch_at_rpt_type;

#ifdef FEATURE_IS2000_R_SCH
/*--------------------------------------------------------------------------
       SUPPLEMENTAL CHANNEL REQUEST PILOT STRENGTH MEASUREMENT REPORT
--------------------------------------------------------------------------*/

typedef struct
{
  mccsrch_rpt_hdr_type  hdr;
  int2 ref_pn;           /* Time reference PN sequence offset */
  byte ref_pn_ecio;      /* Reference pilot strength */
  cai_pilot_rec_type  ref_pilot_rec_type;
  caii_add_pilot_rec_type  ref_pilot_rec;
  byte meas_cnt;         /* Number of pilot measurements */
  /* Pilots in ASet, Nset and Cset other than Reference Pilot */
  struct
  {
    word    pn_phase;    /* Pilot measured phase */
    byte    pn_ecio;     /* Pilot strength */
    boolean pn_in_aset;  /* Pilot in ASet or not */
    cai_pilot_rec_type  pilot_rec_type;
    caii_add_pilot_rec_type  pilot_rec;
  } meas[CAI_MAX_SCRM_RPT_PN];
} mccsrch_scrm_meas_rpt_type;

#endif /* FEATURE_IS2000_R_SCH */

#if defined(FEATURE_IS2000_REL_C_DV) || defined(FEATURE_IS2000_DV_SIG_TEST)
#error code not present
#endif /* FEATURE_IS2000_REL_C_DV || FEATURE_IS2000_DV_SIG_TEST */

/*--------------------------------------------------------------------------
                           NEIGHBOR INFO 
--------------------------------------------------------------------------*/
typedef enum
{
  NBR_BS_IMMEDIATE_REG,
    /* if IHO ocurrs, the MS needs an immediate zone or parameter
     * registration on the neighbor BS.
     */
  NBR_BS_OVHD_NOT_OK,
    /* Neighbor BS does NOT have an entry in th BS_INFO array,
     * or it does not have all overhead messages
     */
  NBR_BS_OVHD_OK
    /* Neighbor BS' OVHD MSG are current at the moment the funtion
     * is called
     */
} mcc_srch_nbr_ovhd_type;

typedef enum
{
  NBR_BS_SAME_PAGING_FREQ,
  NBR_BS_OTHER_PAGING_FREQ,
  NBR_BS_UNKNOWN_PAGING_FREQ
} mcc_srch_nbr_paging_freq_type;

typedef enum
{
  NBR_TYPE_UNKNOWN,
  NBR_TYPE_1,
  NBR_TYPE_2,
  NBR_TYPE_3,
  NBR_TYPE_4,
  NBR_TYPE_5,
  NBR_TYPE_6,
  NBR_TYPE_7,
  NBR_TYPE_8,
  NBR_TYPE_9,
  NBR_TYPE_10,
  NBR_TYPE_11,
  NBR_TYPE_12,
  NBR_TYPE_13,
  NBR_TYPE_14 
} mcc_srch_nghbr_type;

typedef struct
{
  byte nghbr_config;
  mcc_srch_nbr_ovhd_type nghbr_ovhd;
  mcc_srch_nghbr_type nghbr_type;
  qword nghbr_ovhd_time_stamp;
} mcc_srch_nghbr_info;

/*--------------------------------------------------------------------------
                      UNION OF ALL SEARCH REPORTS
--------------------------------------------------------------------------*/

typedef union
{
  mccsrch_rpt_hdr_type      hdr;
  mccsrch_acq_rpt_type      acq;
  mccsrch_slew_rpt_type     slew;
  mccsrch_unslew_rpt_type   unslew;
  mccsrch_pc_meas_rpt_type  pc_meas;
  mccsrch_idle_strong_pilot_meas_rpt_type  idle_strong_pilot_meas;
  mccsrch_tc_meas_rpt_type  tc_meas;
  mccsrch_reacq_rpt_type    reacq;
  mccsrch_raho_rpt_type     raho;
  mccsrch_rafail_rpt_type   rafail;
  mccsrch_acq_fail_type      aqfail;
  mccsrch_pc_assign_rpt_type pc_assign;
  mccsrch_wakeup_rpt_type   wakeup;
  mccsrch_fing_done_rpt_type fing_done;
  mccsrch_sys_meas_rpt_type  sys_meas;
  mccsrch_trysleep_rpt_type trysleep;
  mccsrch_pilots_meas_rpt_type pilots_meas;
  mccsrch_ppm_rpt_type  ppsm;
  mccsrch_hho_abort_rpt_type   hho_abort;
  mccsrch_hho_fail_rpt_type    hho_fail;
  mccsrch_cfnset_meas_rpt_type cfnset_meas_rpt; /* New added */
  mccsrch_cfs_ready_rpt_type   cfs_ready_rpt;
  mccsrch_cfs_meas_done_rpt_type cfs_meas_done_rpt;
  #ifdef FEATURE_GPS_MODE
#error code not present
  #endif /* FEATURE_GPS_MODE */
  mccsrch_at_rpt_type at_rpt;
  #ifdef FEATURE_IS2000_R_SCH
  mccsrch_scrm_meas_rpt_type     scrm_meas_rpt;
  mccsrch_scrm_meas_rpt_type t_add_abort_rpt;
  #endif /* FEATURE_IS2000_R_SCH */

  #ifdef FEATURE_IS2000_REL_B
  mccsrch_tc_strong_plts_rpt_type tc_strong_plt_rpt;
  #endif /* FEATURE_IS2000_REL_B */

  #ifdef FEATURE_IS2000_REL_C_DV
#error code not present
  #endif /* FEATURE_IS2000_REL_C_DV */

  #ifdef FEATURE_IS2000_REL_D
#error code not present
  #endif /* FEATURE_IS2000_REL_D */

} mccsrch_rpt_type;

extern q_type          mcc_srch_q;
extern q_type          mcc_srch_free_q;

/*===========================================================================

FUNCTION MCCSRCH_INIT

DESCRIPTION
  This function initializes the queues between the Searcher task and the CDMA
  Main Control task.  It also places buffers on the free queue.

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/

extern void mccsrch_init( void );

/*===========================================================================

FUNCTION MCC_SRCH_FAST_RAHO

DESCRIPTION
  This function is called in the context of the Searcher task during idle
  sleep to expedite MC's response to a fast RAHO.

DEPENDENCIES
  None.

RETURN VALUE
  Value of mccidl_cheap_idle_ho, which when TRUE indicates to SRCH that it
  can go back to sleep

SIDE EFFECTS
  The values of the global varibles mccidl_fast_raho_state and
  mccidl_cheap_idle_ho may be changed.

  Caution: this function may be called before "SRCH_WAKEUP_R" sleep-
  report.

===========================================================================*/

extern boolean mcc_srch_fast_raho
(
  word new_pilot, 
  byte ecio_old_pilot,
  byte ecio_new_pilot
);

/*===========================================================================

FUNCTION MCC_SRCH_GET_ACTIVE_INFO

DESCRIPTION
  This function retrieves active pilot information.

DEPENDENCIES
  None.

RETURN VALUE
  None

SIDE EFFECTS
  None.

===========================================================================*/

extern void mcc_srch_get_active_info
(
  srch_idle_chan_info_type *chan_info
);

/*===========================================================================

FUNCTION MCC_SRCH_GET_NBR_INFO

DESCRIPTION
  This function retrieves page channel, broadcast control channel or forward
  control channel info of neighbor BS with config = 0, 1 or 4. The function is
  called in the context of the Searcher task during a fast RAHO so there is
  enough time for the new PN long code mask to settle down.

  Note: This function is called before function mcc_srch_fast_raho() and
  will not change cur_bs_ptr.

DEPENDENCIES
  None.

RETURN VALUE
  TRUE -  Channel info is available
  FALSE - Channel info is not available.

SIDE EFFECTS
  None.

===========================================================================*/

extern boolean mcc_srch_get_nbr_info
(
  word pilot,
  srch_chan_config_cmd_type *chan_config
);

/*===========================================================================

FUNCTION MCC_SRCH_MIN_BS_P_REV

DESCRIPTION
  This function returns the minimum P_REV among the known base station on the
  CDMA channel of the ASET. The known base stations are those with current
  configuration parameters.

DEPENDENCIES
  None.

RETURN VALUE
  The minimum P_REV among the known base station.

SIDE EFFECTS
  None.

===========================================================================*/

extern byte mcc_srch_min_bs_p_rev(byte band, word freq);

/*===========================================================================

FUNCTION MCCSRCH_SET_SRCH_RPT_SIGNAL

DESCRIPTION
  This function allows the Search to set a signal for MC when sending a
  Search report.

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/

extern void mccsrch_set_srch_rpt_signal ( void );

/*===========================================================================

FUNCTION MCC_SRCH_LOOKUP_NGHBR_INFO

DESCRIPTION
  This function looksup the neighbor information for the specified neighbor.
  The information includes:
  1) Neighbor Config
  2) Status of Neighbor Overhead
  3) Neighbor Type        
  4) Overhead timestamp for that neighbor

DEPENDENCIES
  None

RETURN VALUE
  Neighbor info
  
SIDE EFFECTS
  None.

===========================================================================*/
void mcc_srch_lookup_nghbr_info
(
  word pilot_pn,
  byte band,
  word freq,
  mcc_srch_nghbr_info *nghbr_info
);
#endif /* MCCSRCH_H */

