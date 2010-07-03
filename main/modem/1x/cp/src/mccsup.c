/*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*

     C D M A   M A I N   C O N T R O L   C O M M O N   F U N C T I O N S

GENERAL DESCRIPTION
  This module contains functions common amoung the INITIALIZATION, IDLE,
  SYSTEM ACCESS, and TRAFFIC CHANNEL states.

EXTERNALIZED FUNCTIONS
  MCC_RESET_AUTH - Resets the Authentication task.
  PERMUTE_ESN - Permutes the ESN.
  MCC_GET_MSG - Retrieves an overhead message.
  MCC_RESET_PC_FRAMES - Resets duplicate detection timestamps.
  MCC_ENTRY_INIT - Initializes and entry for a new Base Station.
  MCC_PUT_SEQ - Updates the message sequence for the current Base Station.
  MCC_PUT_MSG - Stores an overhead message for the current Base Station.
  PROCESS_REORDER - Process a Reorder Order.
  PROCESS_INTERCEPT - Process an Intercept Order.
  MCC_ENCODE_DTMF - Translates ASCII digits into their BCD equivalents.
  MCC_DECODE_DTMF - Translates BCD digits into their ASCII equivalents.
  MCC_ROAM - Determines if the phone is roaming.
  MCC_DETECT_DUPLICATE - Determines if a message is a duplicate.
  XLATE_DIALED_NUM - Translates a dialed number into its OTA equivalent.
  MCC_MS_ORIG - Process an origination request.
  MCC_MS_SMS - Process and SMS message transmission request.
  MCC_COMP_ADDR - Compares the mobile's address to a received address.
  MCC_MATCH_MSG - Determines if we should process a received message.
  MCC_PARSE_PAGE - Determines if we should process a page message.
  MCC_VALIDATE_SO_REQ - Determines if the requested service option is OK.
  MCC_IDLE_HO_RX - Clears received messages and sets up for Idle Handoff.
  MCC_SEND_SRCH_PAGECH - Sends search new PC_ASET (w/paging channel and freq).
  MCC_SEND_SRCH_ASSIGN - Has searcher process a paging channel assignemnt.
  MCC_PC_ASSIGN - Do processing for paging channel assignment.
  MCC_QUEUE_AUTH_RPT - Queue a report from auth (called by AUTH task)
  MCC_INFO_REC - Builds an information record to be transmitted.
  PROCESS_INFO_RECORDS - Processes received information records.
  MCC_GET_STATE - Returns the current state of CDMA Main Control.
  MCC_SEND_SRCH_PARMS - Sends the updated search parameters to the searcher

  MCC_CHECK_ALERT_SIGNAL_INFO - checks whether the info record contains
                                a signal info record for alert.
  MCC_SEND_MO_SMS_STATUS - Send MO SMS status to CM.
  MCC_SEND_ORIG_FAILURE  - Send Origination status to CM.
  MCC_SEND_CALL_END_ACK  - Send call end ack to CM
  mc_map_deact_to_mcc_exit_reason - Map MMoC's deactivate reason to mcc exit
                                    reason.
  mc_map_gen_cmd_to_mcc_exit_reason - Map MMoC's generic command to mcc exit
                                    reason.

INITIALIZATION AND SEQUENCING REQUIREMENTS
  NONE

Copyright (c) 1990-2009 by QUALCOMM, Incorporated.  All Rights Reserved.

*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*/

/*===========================================================================

                        EDIT HISTORY FOR MODULE

$PVCSPath: L:/src/asw/MSM5100/CP_REL_A/vcs/mccsup.c_v   1.82   07 Oct 2002 14:31:56   fchan  $
$Header: //source/qcom/qct/modem/1x/cp/rel/1h08/src/mccsup.c#2 $ $DateTime: 2009/06/12 01:45:11 $ $Author: anishg $

when       who     what, where, why
--------  ---     ----------------------------------------------------------
06/12/09   ag      Fixed the issue of using NULL pointer in 
                   mcc_cdma_chans_are_supported().
04/28/09   ag      Fixed the issue of missing BCSMS when MS transitions from
                   system access to idle state.
03/30/09   pk      Added new information record MEID_ME and support for ESN_ME
                   to be supported for all releases under FEATURE_HWID_SUPPORT
03/18/09   jj      Fixed the issue of setting SCI incorretly.
09/07/08   ss      Fixed a bug in FULL_TIME_SHDR_3 code, so as to ignore page
                   regardless of the service option used in the page.
08/21/08   ag      Included cdma2kdiag.h as part of DIAG VU decoupling process
05/15/08   bb      Changes for "MVS API Migration" feature
03/04/08   ss      Inhibit the processing of network ordered registration 
                   if the DDTM setting suppresses the registration.
01/16/08   pg/yll  Added support for FEATURE_FULL_TIME_SHDR_3 so that it
                   allows 1x to do acq on RX0 regardless of the state of DO.
01/08/08   ag      Mainlined FEATURE_LPM
12/14/07   an      Added Passport Mode
12/13/07   sb      Check ddtm only in Idle state, not System Access.
10/29/07   grl     Added full time SHDR support.
06/07/07   pg      Lint clean up.
05/11/07   pg      More Lint Cleanup.
03/21/07   pg      Rename FEATURE_JCDMA_2 to FEATURE_JCDMA.
03/19/07   pg      Lint Cleanup.
03/09/07   pg      removed dependency on WMS and make sure it only depends on 
                   CM for SMS.
03/08/07   an      Fixed Critical Lint Errors
01/15/07   sb      VOIP-1X handoff updates.
11/28/06   pg      Treat an EGSRDM record with num_chans = 0 as a valid record.
11/27/06   sb      Fixed braces in mcc_set_address.
11/21/06   sb      Added support for VOIP-1X handoffs.
11/14/06   pg      Replaced mob_model in (Extended) Terminal Information Record
10/27/06   fh      AKA/AES: changed CRC-8 calculation algorithm to handle more 
                            general cases.
10/25/06   fc      Changes per code review comments.
10/24/06   fc      Changes to support reverse link timing transfer 
                   enhancement for UMTS to 1x Handover.
10/16/06   fc      Unfeaturize pseudo idle support.
10/11/06   fc      Fixed lint errors.
09/30/06   fc      UMTS changes.
09/30/06   fh      AKA/AES: moved the 8-bit CRC calculation from mccaes.c into
                            mccsup.c
09/26/06   sb      Fixed featurization for Redirecting Number info rec.
09/25/06   fc      UMTS changes.
09/25/06   sb      Added support for Band Class 15.
09/02/06   fh      AKA/AES: Miscellaneous code review changes
08/29/06   fh      AKA/AES: changes according to the renaming of 
                            MCCAP_DF_AES_UI to MCCAP_DF_AES_UI_VOICE
08/29/06   fh      AKA/AES: cleaned up the usage of AES capability functions
08/16/06   pg      Added support for release D Packet Zone Hysteresis.
08/09/06   fc      Added idle mode reporting to searcher for UMTS. 
07/17/06   fc      Optimize the implementation of pseudo bs.
07/13/06   fh      AKA/AES: changed an AES function call according to AES changes.
07/11/06   fc      Added reporting of base id for UMTS.
07/10/06   fc      Fixed featurization.
07/06/06   fc      Renamed SRCH_LOST_RF_RESOURCE_R to 
                   SRCH_REQUEST_SYS_RESTART_R.
06/23/06   pg      RPC cleanup.
06/15/06   fc      Changes for umts.
06/12/06   an      Modified the EXT_TERM_INFO to return as many SO's as possible
06/02/06   fc      Cleanup umts interface.
06/01/06   an      Added the bypass registration indicator changes
05/31/06   pg      Changed function mcc_inform_sd_redirection() interface.
                   Release D EGSRM code cleanup.
05/24/06   fh      AKA/AES: misclaneous fixes within AKA/AES features
05/24/06   fc      Fixed the issue of coupling encryption with voice privacy.
05/23/06   fc      Renamed the variable and function related to waiting for
                   authentication signature.
05/19/06   fc      Added support for SRCH_LOST_RF_RESOURCE_R.
05/18/06   fc      Changes on SCI reporting in extended term info rec.
05/17/06   fh      Merged in new development of AKA/AES features
04/28/06   fh      Fixed a bug that corrupts queue in function of
                   mcc_request_transceiver_resource 
04/26/06   an      Fixed lint errors
04/24/06   fc      Fixed Rel A and D featurization.
04/24/06   pg      Fixed a bug in Extended SO Info Record.
04/18/06   pg      Added support for Extended SO Info Record.
04/12/06   pg      Added support for release D EGSRM.
04/02/06   fh      for AES, moved the AES BS support info processing from the 
                   moment it receives overhead message to later processing in
                   specific overhead process functions.
03/27/06   fh      Added AES Support
03/14/06   sb      Added fix for KDDI related to high persistence.
03/09/06   fc      Corrected the issue of sending SRCH_CHAN_CONFIG_F before
                   SRCH_IDLE_ASET_F for PCH assignment.
02/21/06   fc      Fixed merge error.
02/07/06   fc      Mainlined FEATURE_MC_SUPPORTS_SHDR.
02/07/06   fc      Fixed the issue of MCC and IMSI_11_12 values in ESPM/MCRRPM
                   being corrupted by MS's MCC and IMSI_11_12 values if after
                   IHO, first rxed message is GPM that validated the freshness
                   stored overhead of the new pilot.
02/07/06   fc      Removed duplicate function prototypes.
12/13/06   an      Added ECCLM Cross Band Hashing Support
01/20/06   fc      Merged the following :
           yll     Added support for AKA.
01/19/06   fc      Added support for EXT UIM ID info rec.
01/06/06   fc      Update NCC supervision based on OOS changes.
01/06/06   sb      Replaced cm_mc_rpt_free_q with cm_mc_rpt_get_buf_else_err_fatal().
01/06/06   fc      Moved function call cm_mc_unlock_resources to mcc_wait.
11/30/05   fc      Fixed the issue of incorrectly extending the freshness of
                   stored overheads in idle handoff.
10/16/05   fc      Fixed slot cycle index reported in Terminal Info rec.
                   Renamed mcc_convert_to_ota_sci to 
                   Renamed mcc_convert_pref_sci_to_ota_sci to 
11/13/05   an      Included header file rf.h for rf_get_sub_class function
11/07/05   fh      Added the CM function call to unlock RF held by HDR before
                   upgrading RF resource request while mobile is in AEHO or 
                   ACCESS state
10/28/05   an      Made band class/subclass code review changes
10/27/05   an      Added Band Class/ Subclass Reporting Feature
10/19/05   sb      Added support for Band Class 14, Band Subclass I, J, K.
10/18/05   va      MOved "+" code dialing support (digit mode determination) 
                   to CP.
10/15/05   fc      Added support for rescan bit on new common channel.
                   Default plcm_type to MEID for P_REV_IN_USE 11 or greater.
09/29/05   sb      Removed DB_VOICE_PRIVACY.
09/13/05   pg      Only hold encoder clock on for early jump command during
                   call setup.
09/09/05   sb      Added exclusive band class mode under JCDMA.
08/19/05   fc      Obsolete CM_REORDER_F and use CM_CALL_ORIG_FAIL_F instead.
08/19/05   sb      Miscellaneous fixes for info recs.
08/18/05   fh      Added a function call to let HDR unlock RF when 1x is requesting
                   RF resources.
07/26/05   fc      Fixed lint errors.
07/25/05   fc      Changes for Rel D Addendum f-csh messages.
07/24/05   sb      Added support for tri-band under FEATURE_T53.
07/22/05   va      Removed is_jcdma_emergency flag
07/15/05   fc      Fixed lint errors.
07/11/05   fh      Cleaned up header file including from MUX:
                     added including of rxcmc.h
07/08/05   fh      Replaced mcc_send_srch_access_cmd with the following two 
                   functions for SHDR phase II design:
                       mcc_wait_transceiver_resource
                       mcc_request_transceiver_resource
07/12/05   ht      Added MEID Addressing support
07/12/05   fc      Fixed lint/compile errors.
07/11/05   va      Added MEID_ECAM check and plcm_type check in 
                   mcc_check_if_traf_chan_assgn_is_okay
07/07/05   va      Merged the following from MSMSHARED:
07/05/05   sb       Added MEID support
06/03/05   fc      Merged the following from MSMSHARED:
06/02/05   fc       Removed obsolete comment.
05/31/05   fc      Merged the following from MSMSHARED:
05/31/05   va       Removed RF tuning from MC as part of SHDR changes.
05/23/05   pg      Set random number of chips backoff to 0 for jump to 
                   hyperspace before getting in or when it's in CDMA_TC.
05/23/05   fc      Fixed lint error.
05/20/05   fc      Merged the following from MSMSHARED:
05/20/05   an       Added band subclass info record
05/20/05   pg      Removed FEATURE_IS2000_REL_D_JUMP.
05/20/05   pg      Renamed FEATURE_JUMP to FEATURE_IS2000_REL_D_JUMP.
05/20/05   pg      Added support for early jump to hyperspace under FEATURE_JUMP.
05/10/05   fc      Merged the following from MSMSHARED:
05/05/05   az       Changed the function call to get the band mask to 
                    rfm_get_band_mask()
05/03/05   fc      Merged the following from MSMSHARED:
04/27/05   an       Added support for 4GV service option
04/27/05   az      Submitted changes for CR 62361. This also requires 
                   corresponding changes in NV. Changed the name of a #define
04/25/05   fc      Merged the following from MSMSHARED:
04/20/04   fc       Changes for the update of reg_sid, reg_nid, reg_reg_zone, 
                    auth and rand during AHO/APHO.
04/04/05   fc      Merged the following from MSMSHARED:
03/31/05   fc       Added support for EPZID.
03/28/05   fc      Added support for DCA.
03/25/05   az      Changed the label from MCC_USE_DEFAULT_SCI to MCC_NULL_SCI
03/25/05   az      Initializing cdma.last_reg_sci to MCC_INIT_LAST_REG_SCI
                   when (sid, nid) is not in SID_NID_LIST.
03/23/05   fc      Merged the following from MSMSHARED:
03/22/05   an       Mainlined FEATURE_ENHANCED_BAND_CLASS. Eliminated 
                    FEATURE_CDMA_800 and FEATURE_CDMA_1900
03/21/05   an       Mainlined IS2000 features and implemented the new
                    Dynamic Feature Design.
03/14/05   fh       Added a line to set burst_source to be CM_DBM_FROM_1X in the 
                    function call of mcc_send_mt_dbm_data. CM added this new 
                    interface to differentiate the DBM sources, and we are 
                    requested to add this for 1X DBM function requests.
03/14/05   an       Added special dup detection function for ecam
02/25/05   fc      Merged the following from MSMSHARED:
02/17/05   an       Added call_id information in the line_ctrl record.
02/15/05   an       removed CAI_LINE_CTRL_REC from the current featurization of WLL
02/18/05   fc      Merged the following from MSMSHARED:
02/09/05   fh       Changed including of ds.h to be ds707.h, as data team requested
02/08/05   an       Mainlined IS95B features
02/10/05   va      Merged the following from REL_B_C archive:
02/03/05   fc       Fixed lint error.
02/01/05   sb       Fixed setting of fields for BC 10, 11 and 12.
01/27/05   fh       Fixed the mispelling of FEATUR_APIONE to FEATURE_APIONE
01/20/05   az      Removed Lint errors
01/07/05   an       Removed feature FEATURE_SMS and FEATURE_UASMS
12/20/04   yll      Added support for SRCH_ACCESS_F.
01/31/05   sb      Added support for the SYNC ID info rec.
01/20/05   az      Fixed lint errors
01/10/05   va      Merged with REL_B_C archive:
12/10/04   sb       Fixed lint warnings.
12/06/04   yll      Added the interface change for apiOne.
12/22/04   pg      Moved mcc_flush_rxtx_q() to mccsup.c.
12/01/04   az      Added support for updating 
                   cur_bs_ptr->csp.sp.neg_slot_cycle_index_sup from MC-RR msg
11/03/04   az      Added fix so that the MS registers with a positive SCI 
                   if the Rev D BS does not support negative SCI.
10/29/04   pg      Added support for bypass 2 good frames at TCI state.
10/25/04   fc      Featurize ECAM msg copy stuff with FEATURE_IS2000_REL_D
                   rather than FEATURE_IS2000_REL_D_DV.
09/30/04   sb      Fixed compiler warning.
09/27/04   va      Removed the p_rev_in_use based init which was put in during merge.
09/27/04   va      Merge from REL B/C archive
08/19/04   yll      Clean up MC/SRCH interface.
08/12/04   pg       Added Enhanced Multi-character Extended Record.
08/10/04   sb       Added processing for the Redirecting Number info rec.
07/29/04   fc       Renamed bcch_index to bcn.
07/27/04   va       Added updates to DB in case of PC assignment in ECAM
07/23/04   yll      Mainlined FEATURE_INT_ROAMING per Rotem's request.
07/21/04   sj       Added support for umts to 1x handover.
09/22/04   va      Changes for REL D IRs
09/21/04   az      Changed feature name from FEATURE_REL_D_NEG_SCI to 
                   FEATURE_IS2000_REL_D_NEG_SCI
09/16/04   fc      Added support for reverse DV.
08/24/04   fc      Fixed lint error.
08/23/04   az      Rel D Negative SCI - changes for dynamic feature
08/23/04   pg      Added support for new RETRY_TYPE: '100' and '101'.
08/23/04   az      Added FEATURE_REL_D_NEG_SCI to mcc_convert_to_ota_sci()
08/18/04   fc      Removed silk incl flags on overhead messages.
08/17/04   az      Rel D Negative SCI changes
08/09/04   fc      Service negotiation changes for Release C and D.
08/02/04   bkm     Lint cleanup for Rel B, C, D undefined.
08/02/04   pg      Took out an unnecessary if statement to avoid lint error.
07/28/04   pg      Added Enhanced Multi-character Extended Record.
07/21/04   pg      Rel D changes for A41RAND message.
06/17/04   bkm     Rel C addendum changes for ECAM.
06/16/04   fc      Bundle ASET update with channel config update.
                   Rel C adddendum changes on GNLM, UNLM and ECAM.
06/03/04   fc      Added support for Release D common channel message.
06/03/04   bkm     Merged the following from IS2000_REL_B_C:
  05/21/04   fc      Lint clean up.
05/28/04   bkm     Merged the following from IS2000_REL_B_C:
  05/19/04   sb      Made following merge official via integrate.
  05/14/04   sb      Merged the following from msmshared:
    03/11/04   yll     Added support for KDDI requirement to 
                       ignore async data and async fax 1x pages during HDR 
                       connected state.
    03/05/04   bkm     Lint cleanup.
    03/03/04   ph      Removed featurization around mci.h  
  04/02/04   fc      Fixed the setting of msg integrity fields.
  03/20/04   va      Fixed a compilation error
05/21/04   bkm     Fixed compile issue.
04/08/04   bkm     In mcc_match_msg use switch rather than if to control
                   logic of setting fpc_pri_chan for assign_mode 5.
04/08/04   bkm     Use cai_ext_chind_type enums rather than numeric values.
03/26/04   bkm     Merged the following from IS2000_REL_B_C
  03/12/04   fc      Removed function mcc_return_to_idle_init.
  03/09/04   va      Included mci.h unconditionally
  02/27/04   yll     Fixed a problem of not checking the P_REV when setting the
                   Rel A signalling encryption fields.
  02/24/04   RC      Added support for FEATURE_INT_ROAMING
02/05/04   bkm     Merged from IS2000_REL_C_B
  02/05/04   va      Merged following from MSMSHARED
    01/05/04   bkm     Fixed featurization around setting of is_jcdma_emergency.
    01/05/04   pg      Mainlined FEATURE_MULTIMODE_ARCH.
12/17/03   yll     Added debug messages when DDTM is on.
01/13/04   bkm     Use REL_C_DV featurization around AM '101' support.
01/05/04   bkm     Set FPC default values for AM '101' in mcc_match_msg().
12/02/03   bkm     Adding support for ECAM AssignMode = '101' for DV.
12/19/03   va      Merged following from msmshared:
 12/16/03  sb      Fixed setting of new_p_rev_in_use in mcc_put_msg.
 11/25/03  bkm     Allow T53/JCDMA with BC6.
 11/21/03  ph      Fixed featurization
 11/20/03  ph      Set rp.* from Acc Parm Message.
 11/13/03  yll     Moved the resetting of cdma.call_orig outside the function
                   mcc_send_orig_failure().
 11/11/03  yll     Added support for JCDMA emergency calls.
 11/10/03  va      Added call_id for CM_NSS_REL_F cmd
11/11/03   fc      Changed mcc_info_rec to set capability info fields
                   correctly.
11/04/03   yll     Changes with srch_tune_rfm() interface.
11/02/03   fc      Fixed length calculation for MC extended display info rec.
11/03/03   yll     Fixed the problem in HDR DDTM mode.
10/30/03   az      Added a function mcc_is_current_band_class_in_group
                   for FEATURE_ENHANCED_BAND_CLASS
10/29/03   sst     T_M6500 in last checkin changed to proper T_MSM6500
10/29/03   sst     Surrounded SRCH_HAS_PILOT_GAIN_CNTL with T_M6500
10/29/03   va      RELC ECAM processing
10/28/03   va      Changed threex_ccsh_supported to threex_cch_supported
10/24/03   sst     Changed RF tuning calls with srch_tune_rfm(), featurized 
                   as SRCH_HAS_PILOT_GAIN_CNTL
10/23/03   yll     Added support for updating HDR PANID.
10/17/03   ph      added support for new DIAG interface to retrieve MC state.
10/15/03   fc      Changed info rec size calculation to match parser.
09/29/03   sj      Added FTM deactivate reason.
09/29/03   bkm     Put code under FEATURE_DS_VOICE_AS_DATA under FEATURE_DS 
                   instead.
09/26/03   fc      Added checking to make sure announcement page match not 
                   overriding other page match.
09/11/03   yll     Added the featurization for ACP include files.
09/04/03   fc      Added support for UIM_ID and ESN_ME info recs.
08/20/03   yll     Changed FEATURE_HDR_HYBRID to FEATURE_HDR.
08/11/03   bkm     Removed support for FEATURE_SPECIAL_MDR.
08/11/03   az      Added support for FEATURE_MODIFIED_SCI
08/08/03   fc      Added support for Release C messages. 
08/06/03   sb      Added support for CDMA Off Time Reporting.
08/01/03   yll     Added support for Dynamic Feature.
08/01/03   dyc     Added support for FEATURE_MULTI_RX_CHAIN
07/31/03   yll     Removed the TEMP_MC from HDR related feature.
07/30/03   az      Added support to return overflow size for MO SDB
07/30/03   az      Added support for FEATURE_SO_8026
07/30/03   yll     Merged in the following changes:
           dna     Added support for HDR.
07/25/03   va      Claned up compile errors when REL_B is off
07/17/03   jqi     Added Extended PRL support
                   -Changing band class to band bit mask in sd_sys_s_type.
07/03/03   fc      Corrected the sanity check for segmented UPM page match.
07/01/03   az      Added FEATURE_ENHANCED_BAND_CLASS
06/02/03   bkm     Lint cleanup.
05/27/03   yll     Changes for simultaneous monitoring of FCCCH and 2nd BCCH.
05/27/03   az      Fixed the reporting of IMSI hash key during RAHO
05/23/03   ph      fixed typo in feature comment (FEATURE_FACTORY_TESTMODE)
05/16/03   sj      Removed informing mccsyobj of new SID upon sync channel msg.
05/09/03   fc      Fixed the update of QPCH IMSI hash key.
05/08/03   fc      Added support for reporting PCH/BCCH acquired event for
                   channel hashing and idle handoff.
04/17/03   bkm     Fixed T53 handling of last_rec in mcc_process_info_records(). 
04/14/03   va      Added support for TC to Idle transition. Support
                   for new fn to reterive ovhd channel info.
04/07/03   fc      Added the support for simulatenous monitoing BCCH and FCCCH
                   in substate CDMA_OVHD.
03/31/03   sb      Moved cadence_type to fixed part of Para Alert info rec.
03/28/03   va      Added support for p_rev 8 Info Record fields.
03/21/03   fc      Added PCH BI support.
02/20/03   fc      Corrected feature name.
02/27/03   fc      Removed redundant assignments for fields cci_supported and
                   bi_supported in mcc_put_msg.
02/11/03   va      Moved copying of call_id outside the REL_A feature.
02/05/03   lcc     Default mcc and imsi_11_12 to previous values if ESPM or MCRRPM
                   has not been received yet in mcc_comp_addr().
02/05/03   fc      Cleanup for f-csh forward compatability.
01/30/03   fc      Fixed compiler error.
01/27/03   jqi     Added band class 6 support.
                   Fixed-CR23081 CR23082
01/24/03   lh      Modified pzid change processing interface.
01/23/03   yll     Added mcc_sdb_supported() function.
01/10/03   fc      Changes for forward compatibility of f-csh.
12/02/02   yll     Added event logging for pref system reselection.
11/22/02   sj      Corrected misplaced else.
11/18/02   va      Moved the fn mcc_send_orig_fail_retry_delay from mccsa.c to
                   here.
11/14/02   sb      Featurized rand parameters under FEATURE_AUTH.
11/13/02   sb      Fixed featurization.
11/07/02   az      Removed FEATURE_IS2000_REV0_PREADDENDUM
11/07/02   az      Mainlined FEATURE_IS95B
11/07/02   az      Removed the feature TMOBILE
11/05/02   bkm     Added support for JCDMA Max Rate feature.
10/31/02   az      Added support for HDR Data dedicated transmission mode
                   access prevention.
10/24/02   yll     Moved several parms from sleep command to parm command.
10/17/02   sb      Added access function for slot_cycle_index_p.
10/07/02   fc      Changes to invalidate unlm in mcc_init_ovhd_info.
10/03/02   va      Added converting CC substates into proper DIAG substates
                   when Rel_A is off.
10/03/02   sb      Linted the code.
10/02/02   az      Mainlined FEATURE_IS2000, FEATURE_IS2000_2_1_ADDENDUM, and
                   FEATURE_COMPLETE_SNM
10/02/02   sb      Renamed is_silent_redial, silent_redial_count to orig_reason, orig_count.
09/30/02   sb      Mainlined FEATURE_SD20.
09/26/02   fc      Fixed featurization issue for FEATURE_IS2000_REL_A_CC.
09/02/02   fc      Changes based on UPM code review.
09/03/02   ts      Added support for Run Time R-UIM Enable
08/28/02   fc      Updated comments.
08/16/02   fc      Updated debug message.
08/13/02   sh      Discard CDMA Packet data page if HDR RLP is active
08/08/02   fc      Fixed access issues caused by changes of correcting MS
                   unnecessary staying awake for changes in APM and EAPM.
06/25/02   HQ      Added FEATURE_CDSMS.
08/01/02   fc      Corrected the processing of pending broadcast page.
                   Removed the switch of channel monitoring from
                   FCCCH to BCCH caused by access message sequence change in
                   received page message. This change is done to sync up the
                   standby optimization alresady done for periodic change in
                   EAPM.
07/29/02   fc      Changes to the processing of pending broadcast page to
                   support UPM.
07/18/02   fc      Fixed merge error.
07/16/02   fc      Forced MS to pick up latest APM/EAPM if SPM, PM, GPM or UPM
                   indicated that access message sequence has been changed.
07/10/02   fc/lc   Corrected MS unnecessary staying awake for changes in APM
                   and EAPM.
07/08/02   az      Added FEATURE_DBM encapsulating Short Data Burst and
                   Position Determination
07/02/02   az      Added new voice service option FEATURE_SO_VOICE_SMV
06/27/02   fc      Changed UPM function names.
06/25/02   az      Changed the name of the union for IMSI type specific
                   subfields from class to class_type.
06/21/02   ph      changed cmmc.h to cmxll.h
06/19/02   lcc     Added initialization of rl_gain_adj when CAM is received with
                   traffic channel assignment.
06/19/02   fc      Corrected the length for broadcast address copy.
06/18/02   fc      Added page record length checking for UPM.
                   Fixed compile errors when REL A is turned off.
06/13/02   yll     Changed the BCSMS feature name to be under Rel A CC.
06/07/02   sj      Added CP support for multimode architecture.
05/22/02   fc      Added clearing of segmented UPM reassembly buffer upon
                   switching of BS.
05/20/02   fc      Added support for Universal Page Message.
05/10/02   sb      Removed reset of qpch_num from mcc_entry_init.
05/07/02   ph      Fixed compile errors when REL A is turned off.
05/06/02   yll     Changed Broadcast SMS function names.
04/30/02   sb      Added layer to payload of mcc_send_orig_failure.
04/17/02   sb      Added reg_zone event, removed redundant band_class event.
04/12/02   lh      Added support for subaddress in Orig_C.
04/09/02   yll     Added Broadcast SMS support on IS2000 RelA common channel.
03/25/02   fc      Added mcc_is_upd_ovhd_msg_okay.
03/08/02   sb      Updated Ansi-41 to be in sync with Addendum 2 published version.
03/11/02   va      Merged from CP VU 119.
  02/20/02 az      Featurized 13K voice service option
02/22/02   fc      Added support for Release A common channels.
02/12/02   yll     Merged from the common archive CP VU 116.
                   02/06/02   sh      Added hybird operation support
                   01/25/02   ph
                   Fixed a dword-alignment problem in mcc_check_alert_signal_info()
                   12/14/01   fc
                   Removed function call to mccreg_blksys_bc_init.
                   12/11/01   sj
                   Added code for populating the roam_indi field in PC
                   Registration accept with roam ind order message.
                   12/07/01   jqi
                   Added support for band class 5 in mcc_info_rec if
                   FEATURE_BAND_CLASS_5 was defined.
02/07/02   lh      Added missing call_ids.
01/24/02   ph      Fixed checking of QOS parms length.
01/14/02   va      Removed is_cur_bs_cs_supported function
01/08/02   lh      Remove FEATURE_IS2000_REL_A_SVD from MC-CM interface.
12/11/01   ph      merged from SVD branch
                   08/28/01   lh      Added support for Concurrent Service.
12/03/01   lh      Removed some FEATURE_IS2000_REL_A.
10/02/01   kk      Qos Parameters Processing Added.
09/06/01   fc      Added extern definitions mccidl_get_bcast_slot_cycle,
                   mccidl_is_bcast_init, mccidl_save_bcast_page and
                   mccidl_update_sleep_slot_mask.
                   Change function prototype mcc_parse_page.
08/22/01   sb      Added field mipli to Ansi-41; Added new A-2 encryption fields.
08/21/01   kk      Removed encryption related fields, Addendum 2 change.
08/16/01   kk      Merge from common archive
                   08/13/01   wli     Read SPC status from RUIM instead of from NV.
08/09/01   ph      Merged in from MSM 1.39
                   08/07/01   fc      Included sdprl.h when SD2.0 is defined.
                   08/06/01   fc      Added the support of powerup registration for block or sys
                                      or band class change brought by Paging Channel Assignment
                                      Message.
07/09/01   ph      Fixed bugs in callback timer function
07/06/01   ph      Added support for Callback Timers
06/26/01   kk      Merged from common archive:
06/14/01   lcc     Removed reference to octet_aligned_data in
                   caii_chan_cfg_capa_info_type.  It is removed in Addendum 2
                   of IS2000 Rel 0.
06/06/01   lcc     Added support for retry order for RRM.
05/29/01   fc      Changed mcc_inform_sd_redirection function prototype
                   (SD20 only).
05/25/01   ts      Added an interface to the new UIM server.
06/14/01   va      Added support for new EXT_ALERT_WITH_INFO message, also
                   removed references to CDMA_CONV substate.
05/17/01   fc      Merged the following from common archive:
                   05/03/01   lh      Fixed another possible mis-alignment in
                                      mcc_process_info_records().
                   05/02/01   lh      Fixed possible mis-alignment in
                                      mcc_process_info_records().
                   05/01/01   ych     Enabled SPECIAL_MDR support for base
                                      station P_REV greater than 5
                   04/12/01   fc      Added support of events report: SMS sent.
                                      Moved all events logging data structures
                                      /functions to mclog.c and mclog.h.
                   04/11/01   lcc     Corrected a merge error.
                   04/10/01   lcc     Added support for P2 mode.
                   04/09/01   ych     Added code that temporarly enables certain
                                      Data service options for FEATURE_JCDMA_DS.
                                      This is to support the reject DATA service
                                      options JCDMA feature
                   03/26/01   fc      Removed the redundant #include mccap.h.
05/04/01   va      Fixed ovhd_chan init for cur_bs_ptr.
04/24/01   ph      Cleaned up Band Class record to reflect that internal-format caii
                   always has all fields for all builds.
04/18/01   ph      updated to reflect new position of cadence_type in caii format.
03/28/01   lh      Merged following from common archive.
                    03/16/01   ych     Modified T53 code in mcc_process_info_records to fix
                                       the array out of bounds issue, compiler warnings and made
                                       code readible.
                    03/14/01   va      Fixed a compile error if FEATURE_PLT is defined.
                    03/07/01   fc      Corrected #ifdef and datatype cast problems in
                                       mcc_report_event_cp_state.
                    03/07/01   fc      Removed duplicate functions.
                    03/06/01   fc      Added support of events report: Message received, Message
                                       transmitted, Timer expired, Counter threshold and Call
                                       Processing state change.
03/07/01   ph      changed mcc_info_rec() to not calculate ext_fmt_rec_size -
                   it is being done in compute_independent_msg_len().
03/01/02   ph      Merged in from MSM_CP.05.00.55
                     02/26/01   sko     Removed JCDMA feature to poll CDMA_CH by CM.
                     02/16/01   fc      Added support for system determination 2.0. All changes
                                          are featurized under FEATURE_SD20.
                     02/16/01   sh      Added support for MC_END_F ack
                     02/16/01   bgc     Changed FEATURE_FACTORY_TESTMODE to be a runtime mode.
                     02/12/01   yll     Added OTAPA SO in mcc_info_rec.
                     01/24/01   ych     Merged T-53/JCDMA features for SK.
                     01/16/01   jq      Moved/renamed mccsch_retry_delay_type, mccsch_retry_delay to
                                         here.
                     01/12/01   va      Added inclusion of plt_mc.h under FEATURE_PLT.
                     11/28/00   ck      Added a function call to set the NAM LOCK STATE in the R-UIM
                                         when an OTAPA session is begun.
                     11/20/00   lcc     In mcc_info_rec, put where service options are explicitly
                                         reported (for SO and Terminal requests) inside
                                         #ifndef FEATURE_COMPLET_SNM as those service options are
                                         already handled with SNM.
                     11/16/00   bgc     Updated support for FTM and PLT.
02/27/01   kk      After reception of an Origination release A specific info
                   is saved in cdma_ms_type.
02/26/01   ph      removed mcc_check_ext_encryption_fields_is_okay() (rel A)
02/26/01   ph      removed REL A featurization for code copying add pilot rec.
01/29/01   ph      CAI_MC_EXT_DISP_REC, released unused MC-CM command buffer.
01/29/01   ph      mcc_info_rec(), for HOOK_STATUS, fixed a bug related to fall through.
                   For CAI_MC_EXT_DISP_REC, added a comment about dropping message.
01/22/01   ph      In mcc_match_msg(), added code that copied in the New Pilot Record
                   Types from the caii-format to mc-format (for ECAM Assign Mode '100')
01/10/01   va      Changed msg_okay macros into function calls. Defines new functions
                   is_ovhd_msg_okay, is_aeho_msg_okay, is_aho_msg_okay, clear_ovhd_msgs,
                   clear_ovhd_msgs_prev_mismatch
01/10/01   ph      1. Added support for Encryption, SR3 fields of CAM/ECAM in mcc_match_msg().
                   2. Added support for Reg Acc Ord (roam ind & Encryption) and
                      Security Mode Command Msg in mcc_match_msg().
                   3. Added support for new band classes, new adv cap fields (flex, var
                      rates, SR3 etc), new chan cfg capa fields, Hook Status,
                      para alert (cadence type - WLL), MC-Ext Display type  in mcc_info_rec().
                   4. Added new func mcc_check_layer2_encryption_fields_is_okay() that
                      is used by mccidl and mccsa to verify L2 encryption fields of
                      mobile-directed messages.
11/13/00   lcc     1. Added support for reporting band class and CDMA channel
                      change events
                   2. Added function mcc_update_bs_p_rev.
11/10/00   lcc     Corrected age validation (10 minute) of overhead msg in
                   mcc_get_bs_info.
11/08/00   ht      Turn off the service indicator if we are in the midst of
                   scanning CDMA channel which belongs to the same block
                   as the OTASP activation code.
11/01/00   lcc     Added copying of DRS bit from origination command to cdma.drs.
10/02/00   yll     Added support for Position Determination DBM.
09/20/00   jq      Cleaned all lint errors.
09/06/00   jq      Implemented FEATURE_FACTORY_TESTMODE based on FEATURE_PLT
08/02/00   fc      Added support for retry order.
07/14/00   ks      Added case statements in mcc_put_msg() for processing of
                   CAI_EXT_CDMA_CHN_LIST_MSG & CAI_EXT_GLB_REDIR_MSG.
07/14/00   va      Support for Rev-FCH gating and pwr_cntl_delay in matched
                   channel assignment message.
07/07/00   va      Removed some functionality not fully supported yet
07/07/00   yll     Added external function to provide the MSID and registered
                   BS info, include: band class, channel, bs ID, SID and NID.
07/07/00   va      Support for geo_loc_info and band_subclass IR.
06/13/00   jq      Changed mcc_detect_duplicate() to remove dependency on IS95B
                   so BC SMS works in pre-IS95B builds
06/09/00   jq      Changed mcc_detect_duplicate() to disable detection for BCSMS
06/05/00   jq      Added bc_pending as a parameter to mcc_parse_page
                   Implemented duplicate detection in multislotted mode BC
06/02/00   lcc     1. Added update of ack_type in mcc_layer2 in mcc_parse_page for
                      IS2000 compliance.
                   2. Change RLP_BLOB in capability info to RLP_CAP_BLOB.
05/26/00   jq      Added many enhancements to BC SMS: using absolute slot
                   number in all scheduling calculate, introduced slot_mask,
                   allow BC SMS early sleep
04/11/00   va      Fixed a compile issue for #ifndef FEATURE_IS2000
04/11/00   lcc     Added function mcc_get_bs_info.
04/06/00   va      Merged the following from PP (MSM3100_CP.03.00.17):
03/24/00   ks      If ESPM is not received, address matching is done using
                   IMSI_S only. The MS will hold the Page even if it did not
                   receive the (ESPM).
03/16/00   jq      Added broadcast SMS support.
03/14/00   ram     In mcc_info_rec() while processing the Terminal info
                   request, we were only checking the P-REV and the
                   op-mode, but not the band-class. We were sending
                   the Station Class Mark incorrectly since the P_REV
                   is the same for PCS and CELL when the op-mode is 4 or
                   above. Added band-class checking before sending the SCM.
03/14/00   ry      Added OTAPA support
03/08/00   dna     Changed ERR_FATAL to ERR in mcc_resync_auth CR12790.
03/06/00   kmp     Modified mcc_put_msg() to take check for mcc & imsi_11_12
                   "wildcard" cases and move it to idle_msg_handler() in
                   mccidl.c. There will now be only one set of processing
                   for the mcc=0x3FF and imsi_11_12 = 0x7F processing.
03/31/00   ry      Removed the featurization of rf_digital_band_select()
                   since FEATURE_SRCH_DYNAMIC_BAND_CLASS will always be on
03/30/00   ry      Featurized rf_digital_band_select() in mcc_send_srch_assign()
                   under FEATURE_SRCH_DYNAMIC_BAND_CLASS
03/29/00   ry      Featurized rf_digital_band_select() in mcc_send_srch_assign()
                   under FEATURE_CDMA_800 and FEATURE_CDMA_1900
03/23/00   ry      Added IS2000 featurization
           ry      Merged the following from PP (MSM3100_CP.03.00.10):
           ht      Add check for NULL pointer on mcc_rxtx_q and mcc_srch_q.
           ks      Added condition (#ifndef FEATURE_IS95B) while checking for
                   mcc == 0x3FF && imsi_11_12 == 0x7F) in mcc_put_msg()
           fc      Added support for IS-733 service option number 17.
           vn      Merged the following from trimode build TM1005:
           ht      Add call to mcsyspr_set_last_cdma after rf_tune_to_chan.
           ck      Featurised Extended Channel Assignment under FEATURE_IS95B.
           ram     Added support for trimode in mcc_info_rec() while building
                   the Band Class Information Record. We were assuming if
                   FEATURE_KPCS is defined, then cellular band cannot be
                   supported. But for trimode builds, cellular band can be
                   supported.
           ck      Included band class information in mcc_send_srch_pagech
                   while sending srch to paging channel
                   Included band class parameter in mcc_old_time() and check for
                   band class within the funtion.
                   In the function mcc_send_srch_assign(), band class info is
                   included for SRCH and also RF is notified of the band class
                   if Freq_incl is set to true in the CAM/ECAM.
02/13/00   va      Addendum Changes.
01/27/00   va      Merged in changes from GV40401
                   Modified mcc_info_rec to allow op codes 0 and 1 for IS-95B
                   MS in cellular or PCS band.
12/20/99   va      Support for the IS2000 reverse Information Records
11/15/99   va      IS2000 Changes:
                   Support to parse the ne ECAM.
                   New argument in mcc_info_rec to return the size of the rec when in
                   external format.
11/03/99   lcc     Merged from GD4021:
           doj     Merged changes by YLL (see next line) into ASW files.
           yll     Featurized appending activation code in *228 OTASP orig.
           doj     Modified PACKET_ZONE_ID reporting such that it will now
                   report for IS-95B as well as for FEATURE_SPECIAL_MDR.
                   Changed so that getting P_REV is done from MC macro for
                   IS-95B but still from MCCSYOBJ for FEATURE_SPECIAL_MDR.
           abh     Added checking PRL enabled in mcc_ignore_activation_code
           ks      Setting the addr_type variable properly in case of General
                   page Message in mcc_detect_duplicate() function.
           ks      Removed a compiler warning by adding cast to q_get().
           jq      Changed mcc_pc_assign() to fix problems of overhead info
                   and neighbor list not been updated when (E)CAM,
                   assign_mode=001/011 is received and MS tunes to a new
                   pilot in idle state.
10/28/99   lcc     Moved PLT related code to plt_mc.c.
09/16/99   lcc     Merged in PLT support from PLT archive:
                   1. Used default values for search parameters command for PLT.
08/13/99   lh      CM and SMS phase 2 implementations.
08/13/99   jq      Take out the FEATURE_NEW_CM ifdefs and endifs
08/06/99   nmn     Removed IS-96
07/30/99   lcc     In mcc_validate_so_req, rearranged some code block to make
                   them more meaningful.  Instead of EVRC SO management, SO
                   evaluation, and then prearrangement, it's now prearrange,
                   EVRC SO management, and then evaluation.  This will correct
                   some problems with prearrangement feature which now uses
                   hardcoded SOs.
07/28/99   lh      CM and two-way SMS support added.
07/27/99   fc      Type casted pointer in mcc_info_rec() to eliminate errors
                   with ARM 2.50 ompiler.
07/19/99   nmn     * Removed some checks to change service option IS-96A to
                     IS-96
                   * Removed some service options from status response message:
                     Old Markov
                     IS-96
                     CAI_SO_DATA (mobile to mobile data only)
                   * Removed hard-coded Data service options from status
                     response message, they are supported by SNM.
                   * Removed some #ifdef around SNM.
                   * Added a comment in status response message for customers.
                   * Changed #ifdef ANALOG_FAX to FEATURE_ANALOG_FAX
07/14/99   lcc     In mcc_entry_init, added checking in call to packet zone ID
                   change handler to take care of cases when rx.esp_rx indicates
                   ESPM is not sent by BS rather than the message is actually
                   received.
                   Also, moved reporting of SID and PZID in mcc_put_msg to mccidl.
07/13/99   lcc     In mcc_entry_init, added reporting of packet zone ID when the
                   ESPM is retrived from the stored database.
06/30/99   kmp     Added #include of mcsyspr.h instead of mcsys.h
06/22/99   ck      Added check to see if we have a valid PRL in mcc_is_spc_locked
                   and renamed the function to mcc_ignore_activation_code() (yll)
06/21/99   lcc     In mcc_info_rec, corrected multiplex options request such that
                   IS-95-B mux. options (12 & 15) are reported separately in different
                   records to reflect that they are forward link only.
06/21/99   lh      Two-way SMS and CM support.
06/01/99   sh      added mcc_check_alert_signal_info
05/24/99   kmp     Added a hack until UI is merged and updated. The hack is
                   FEATURE_NEW_CM for FEATURE_IS95B_EXT_DISP. This was a
                   fundamental design change for all cases, but was
                   introduced in IS-95B Feature development.
05/18/99   kmp     Merged in the following IS-95B features, and replaced
                   T_AUTH, T_SMS, and T_ACP with FEATURE_AUTH, FEATURE_SMS,
                   and FEATURE_ACP.
           kmp     Modified mcc_detect_duplicate() and mcc_reset_pc_frames()
                   for IS-95B to also use the ADDR_TYPE for duplicate page
                   detection. Also update mcc_info_rec() for default ballot
                   version comments for the Op mode record.
           jq      Fixed a merge problem which required one more else in
                   mcc_match_msg.
           lh      Bug fixes for IMSI_T and IMSI_M in status response msg.
           kmp     Merged the changes listed below from the TGP IS-95B baseline.
           jq      Modified mcc_match_msg due to code review
           kmp     Merged in IS-95B changes, ported IS-95B changes to ARM
           jq      Modified mcc_match_msg(), mcc_pc_assign(),
                   mcc_send_srch_assign() to support ECAM
           lh      Authentication using IMSI_M.
           ks      Added FEATURE_IS95B while setting CAI_REG_MOD.
           sh      added mcc_send_srch_parms, added more comments to #error
           lh      FEATURE_JSTD008 replaced with run-time P_REV and Band
                   Class checking.
           kmp     Added support for Phased Deployment. Modified mcc_info_rec(),
                   and mcc_match_msg()
           jcw     Added support for 95B extended display records and grouping
                   indication for info records sent to UI.
           lh      Applied necessary run-time P_REV checking to IMSI_T.
           ks      Added IS95B Overhead Message processing and run-time p_rev
                   checking.  Also modifed mcc_put_msg to modularize.
           lh      Fix scm for backward compatibility.
           lh      IS-95B True IMSI implementation added.
           lh      IS-95B WLL support.
04/27/99   abh     Moved the code to initialise the Overhead Info in bs_info array up in
                   the process of CDMA System Acquisition.
04/27/99   ram     Fixed the following problem in mcc_info_rec - When
                   FEATURE_IS95B_MDR or FEATURE_SPECIAL_MDR is defined,
                   if we get a Status Request Message from a P_REV 3 (or lower)
                   base station requesting mux option information, we still
                   sent the MDR mux options in the Status Response Message
                   resulting in some call failiures. This is fixed so that
                   when the base station P_REV is 3 or lower, we only send the
                   non-MDR mux options that we support.
04/22/99   abh     Fixed problem with initialisation of cdma.call_orig field in OTASP call.
04/08/99   dna     Merged the two changes below.
03/18/99   pms     Fixed CR9217. Reordered the Service Options to accomodate
                   the way Motorola BS's parse the status request message.
03/10/99   lh      Fixed CR9069. Wrong MUX option due to an ARM compiler issue.
04/07/99   nmn     Added ifdefs to make sure service options implemented in the
                   SNM are only added to info records once
02/01/99   ck      Fixed the problem where the MS was responding to a SO 14(SMS)
                   page with SO 3(EVRC) when Home Page SO was set to WILDCARD.
12/03/98   ck      Defaulted the service option to IS96A whenever the special
                   service was FALSE while validating a service option.
02/12/99   lcc     Added PZID, PREV, and SID change support for special MDR.
02/04/99   nmn     Surrounded SNM Info Rec modifications with
                   FEATURE_SPECIAL_MDR ifdefs
01/27/99   ck      Featurised all the changes for EVRC Service Negotiation
                   under FEATURE_EVRC_SO_MGMT
01/27/99   nmn     Updated code for Information Records to handle MDR & SNM.
01/22/99   lcc     Corrected a problem when mobile is paged with default service
                   option in mcc_validate_so_req, which may cause the mobile to
                   miss pages.
01/12/98   lcc     Merged from MDR:
                   mcc_validate_so_req modified to use SNM.
10/20/98   ldg     Added new SO's for IS-707A analog end to end fax.
10/09/98   ck      Added support for EVRC Service Negotation
09/15/98   pms     Backed out _near pointers
09/04/98   pms     Neared the variables to save ROM space.
08/25/98   ck      Added function mcc_queue_dh_rpt.
08/25/98   ck      Added Rate Set 2 SMS to the info records.
08/24/98   dna     Retain message encryption on CDMA->AMPS handoff by
                   separating auth_reset from auth_resync.
08/20/98   ck      Using new voc_capability_supported interface
                   Using VOC_CAP_IS733 instead of VOC_CAP_ISXXXX
                   Using VOC_CAP_IS127 instead of VOC_CAP_EVRC
08/12/98   lh      Change FEATURE_IS95B to FEATURE_IS95B_WLL.
08/12/98   lh      IS-95B WLL support.
08/11/98   ck      Added support for EVRC Voice Option
08/06/98   ych     Fixed the Lint warnings
08/04/98   ych     Merged SSPR, E911 and Acquisition improvements.
08/03/98   lcc     Minor change to conform to portability guidelines as part of ARM
                   porting effort.
07/06/98   pms     Separated the DS_CALL_FAILED_CMD from process_reorder()
                   and created process_ds_call_failed().  This allows one to be
                   called w/o the other.  Removed mcc_dscall from both functions
                   as it not necessary, nor is it ever set properly.
06/25/98   kis     Added support for ASCII digit mode in mcc_ms_orig() function.
06/22/98   lcc     Changed prearrangement so that either CAI_SO_G3_FAX_13K_PRE707 or
                   CAI_SO_ASYNC_DATA_13K_PRE707 are responded depending on the value
                   of the db item "voice_as_data".
05/26/98   lcc     Added support for "Answer Voice as Data" feature (pre-arrangement)
                   CAI_SO_G3_FAX_13K_PRE707 returned in page response in this version.
                   This feature is enabled by "FEATURE_DS_VOICE_AS_DATA".
04/03/98   dna     Fixed SMS valid_ack to be set properly
02/02/98   day     Consolidating feature #define's for Korean PCS support.
                   Changed FEATURE_CP_KPCS to FEATURE_KPCS.
01/26/98   fkm     FEATURE_xxx Updates (via Automatic Scripts)
01/16/98   lh      "customer.h" included.
01/15/98   lh      Added IS-95B Line Control info record processing.
                   Fixed info record pointer updating problem.
01/14/98   dna     Add support for Korean PCS in Band Class Status Response
11/03/97   ldg     Replaced references to specific data service options with
                   a macro which represents the same information.
                   Added support for additional sets of data service options;
                   IS-707.0, Qualcomm proprietary, and IS-707 are now
                   supported.
08/06/97   jrs     For preferred roaming targets, changed mcc_roam() to get
                   the roaming indicator value from system determination.
07/15/97   dna     mcc_put_seq() changed, overhead message processing mods.
06/16/97   dna     New func. sets hold_orig and timer when returning to idle
05/29/97   na      Added some missing support for rateset2 data service options
                   Changes under ifdef DS.
04/18/97   dna     Changed some #ifdefs to make class 1 IMSI its own feature
02/17/97   lh      Always exit to system determination after OTASP orig.
01/25/97   dna     Support for TGP status request
12/20/96   rdh     Added table-driven permute_esn function to save ROM.
12/13/96   jca     Added functions mcc_process_otasp_orig() and
                   mcc_process_force_amps_orig().
11/14/96   jjw     Added Rate Set 2 data service capabilities
11/07/96   RC      Added service option param. in a CM_DATACALL_F cmd. to UI.
10/18/96   dna     Changed mcc_put_seq() to not negate temp neighbor list
10/11/96   dna     Changed entry_init to keep neighbor lists.
09/11/96   jca     Changed mcc_xlate_dtmf() to mcc_encode_dtmf().  Added new
                   function mcc_decode_dtmf().
07/10/96   day     Fix layer 2 duplicate detection.  Separate page and
                   non-page timestamp checks.
06/25/96   dna     #defined IMSI_CLASS_0_ADDR_NUM
06/24/96   jrs     Added support for Preferred Roaming.
05/30/96   dna     Moved common functions from mccdma.c to create this file.
                   Look in mccdma.c for history prior to this date.

===========================================================================*/

/*===========================================================================

                     INCLUDE FILES FOR MODULE

===========================================================================*/
#include <memory.h>
#include "cai.h"
#include "caii.h"
#include "caix.h"
#include "comdef.h"
#include "db.h"
#include "diag.h"
#include "cdma2kdiag.h"
#include "err.h"
#include "mccdma.h"
#include "rxcmc.h"
#include "mccreg.h"
#include "mccrxtx.h"
#include "mccsrch.h"
#include "msg.h"
#include "queue.h"
#include "qw.h"
#include "rex.h"
#include "rfm.h"
#include "rf.h"
#include "ts.h"
#include "srv.h"
#include "target.h"
#include "customer.h"
#include "snm.h"
#include "mccsyobj.h"
#include "parm.h"
#ifdef FEATURE_FULL_TIME_SHDR
#error code not present
#endif

#ifdef FEATURE_MVS_MIGRATE
#error code not present
#endif

#ifdef FEATURE_AUTH
#include "auth.h"
#endif

#include "mci.h"

#if defined(FEATURE_DS) || defined(FEATURE_HDR)
#include "ds707_extif.h"
#endif /* FEATURE_DS || FEATURE_HDR */

#include "tmsi.h"

#if defined(FEATURE_OTASP) || !defined(FEATURE_NSOTASP)

#ifdef FEATURE_ACP
#error code not present
#endif /* FEATURE_ACP */

#include "sdprl.h"
#endif /* FEATURE_OTASP || !FEATURE_NSOTASP */

#include "cmxll.h"
#include "cm.h"

#ifdef FEATURE_CDSMS
#include "wmsbc.h"
#endif /* FEATURE_CDSMS */

#include "mccap.h"

#ifdef FEATURE_FACTORY_TESTMODE
#include "test_mc.h"

#ifdef FEATURE_PLT
#error code not present
#endif /* FEATURE_PLT */

#endif /* FEATURE_FACTORY_TESTMODE */

#include "event.h"

#ifdef FEATURE_IS2000_REL_A_SVD
#include "mccsrid.h"
#endif /* FEATURE_IS2000_REL_A_SVD */

#ifdef FEATURE_IS2000_REL_A_CC
#include "bit.h"
#endif /* FEATURE_IS2000_REL_A_CC */

#include "mccbcsms.h"
#include "mccccim.h"
#include "mccccl.h"
#include "mclog.h"

#if (defined (FEATURE_HDR_HANDOFF) && defined (FEATURE_IS2000_REL_A))
#error code not present
#endif /* FEATURE_HDR_HANDOFF && FEATURE_IS2000_REL_A */

#if defined(FEATURE_IS2000_REL_C_FCS) || defined(FEATURE_IS2000_REL_D)
#include "mccsyncid.h"
#endif /* FEATURE_IS2000_REL_C_FCS || FEATURE_IS2000_REL_D */

#ifdef FEATURE_IS2000_REL_C_DV
#error code not present
#endif /* FEATURE_IS2000_REL_C_DV */

#include "ran.h"

#if defined(FEATURE_IS2000_REL_C_AKA) || defined(FEATURE_IS2000_REL_A_AES)
#error code not present
#endif /* FEATURE_IS2000_REL_C_AKA || FEATURE_IS2000_REL_A_AES */

#include "rfm.h"

#if defined(FEATURE_AUTH) && defined(FEATURE_UMTS_1X_HANDOVER_1XMSM)
#error code not present
#endif /* FEATURE_AUTH && FEATURE_UMTS_1X_HANDOVER_1XMSM */

/*lint -e655 -e818 */

/*===========================================================================

                DEFINITIONS AND DECLARATIONS FOR MODULE

This section contains definitions for constants, macros, types, variables
and other items needed by this module.

===========================================================================*/
#define MCC_VALID_CONFIG (cai_tmo.t31m / 20L)
   /* Maximum time for which configuration parameters are considered
      valid (10 minutes in 20ms frames) */

#define FENDPOS( type, field ) \
  (FPOS( type, field ) + FSIZ( type, field ))
   /* Macro to compute the offset, in bytes, of the end of a specified field
      of a specified structure type */

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
/* Information about acquired base stations                                */

cdma_bs_type bs_info[ MC_MAX_BASES ];

/* Pointer to information for currently acquired base station */
cdma_bs_type *cur_bs_ptr = bs_info;

#ifdef FEATURE_IS2000_REL_D
#error code not present
#endif /* FEATURE_IS2000_REL_D */

#ifdef FEATURE_BROADCAST_SMS
/* Information of Broadcast or Enhanced Broadcast Page record to be matched */
static mccbcsms_bc_info_type mcc_bc_info;
#endif /* FEATURE_BROADCAST_SMS */

#ifdef FEATURE_IS2000_REL_A_CC
/* Information of Universal Page Record to be matched */
static caii_univ_page_rec_type mcc_upm_info;
#endif /* FEATURE_IS2000_REL_A_CC */

#ifdef FEATURE_IS2000_REL_A_AES
#error code not present
#endif /* FEATURE_IS2000_REL_A_AES */

/* The maximum that can be supported now is 
   MAX_BAND_GROUPS (2). Please increase the value of 
   MAX_BAND_GROUPS when adding more than 2 Band Groups in
   Band Class table
*/
/*lint -e785*/
mcc_band_group_type mcc_band_table[MAX_BAND_GROUPS] = 
{
  {
    MCC_CELLULAR_GROUP,
    0    
  },
  {
    MCC_PCS_GROUP,
    0
  }
};
/*lint +e785*/

/* mcc_burst_overflow_size contains the number of bytes the Burst message
   (PD or SDB) is exceeding its limit of successful transmission. */
uint16 mcc_burst_overflow_size;

/* Used to save the info of current BS prior to switching to pseudo BS entry. */
cdma_bs_type *mcc_saved_current_bs_ptr;

/* Indicates if we are in the process of calculating auth key. */
boolean mcc_calc_key_in_progress;

/* Retry delay function */
static void (*retry_delay_inform)(retry_delay_type retry_parm) = NULL;

#ifdef FEATURE_AUTH
/*===========================================================================

FUNCTION MCC_RESYNC_AUTH

DESCRIPTION
  This function sends a command to re-synchronize the Authentication Task
  command queue by queueing a resync command and waiting for it to be
  processed.  If auth has other commands on its queue or is currently
  processing some other command, we will wait for it to finish everything
  and eventually get to our resync command and send the resync report.
  Any other reports it may send before that are discarded and returned to the
  free queue, so they will not be received later when they are not expected.

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/

void mcc_resync_auth( void )
{
  boolean resync = FALSE;
    /* Indicator if auth has been successfully re-synchronized. */
  auth_cmd_type *auth_cmd_ptr;
    /* Pointer to authentication command */
  auth_rpt_type *auth_rpt_ptr;
    /* Pointer to report from authentication task */

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  /* If after SSD update during key exchange, the channel fades before the
     re-authentication is performed and keys committed, we need to reset
     the new_ssd_valid flag to FALSE so that the old SSD is used for authr
     calculations */
  new_ssd_valid = FALSE;

  /* when we exit the TC state, a_key_temp_valid is set to FALSE */
  a_key_temp_valid = FALSE;

  if ((auth_cmd_ptr = (auth_cmd_type*) q_get( &auth_free_q )) != NULL)
  {
    auth_cmd_ptr->hdr.command = AUTH_RESYNC_F;
    auth_cmd_ptr->resync.rpt_function = mcc_queue_auth_rpt;
    mcc_auth_cmd( auth_cmd_ptr );

    if ( auth_cmd_ptr->hdr.status == AUTH_RECEIVED_S )
    {
      do
      {
        /* Wait for auth to report back */

        /* ----------------------------------
        ** Wait for MCC_AUTH_Q_SIG to be set.
        ** ---------------------------------- */
        MSG_MED( "Waiting for AUTH to sync", 0, 0, 0 );

        (void) mcc_wait( MCC_AUTH_Q_SIG );

        if ((auth_rpt_ptr = (auth_rpt_type*) q_get( &mc_auth_q )) != NULL)
        {
          /* Check if this is the report we are waiting for */
          if (auth_rpt_ptr->rpt_type == AUTH_RESYNC_R)
          {
            resync = TRUE;
          }

          /* Replace buffer on free_q */
          cmd_done( &auth_rpt_ptr->rpt_hdr );
        }
        else
        {
          ERR_FATAL( "Wait returned sig for empty Q!", 0, 0, 0 );
        }
      } while (!resync);
    }
    else /* Status returned from command was an error condition */
    {
      ERR( "AUTH can't resync %d", auth_cmd_ptr->hdr.status, 0, 0 );
    }
  }
  else
  {
    ERR( "No free buffers on auth_free_q", 0, 0, 0 );
  }

} /* mcc_resync_auth() */
#endif /* FEATURE_AUTH */

/*===========================================================================

FUNCTION PERMUTE_ESN

DESCRIPTION
  This function permutes the ESN as described in CAI 2.0 section 6.1.3.1.8.

DEPENDENCIES
  None.

RETURN VALUE
  Permuted esn.

SIDE EFFECTS
  None.

===========================================================================*/

dword permute_esn
(
  dword esn  /* ESN to permute */
)
{
  /* Array describing the permutation of esn, the index in the array
     is equal to bit position in the esn ( i.e. first entry matches
     the LSB of the esn, the entry itself is the index of the permuted
     esn ( again 0 = LSB ) */

  static char permutation_array[] =
  {
    31, // E0
     6, // E1
    13, // E2
    20, // E3
    27, // E4
     3, // E5
    10, // E6
    17, // E7
    24, // E8
     0, // E9
     7, // E10
    14, // E11
    21, // E12
    28, // E13
     4, // E14
    11, // E15
    18, // E16
    25, // E17
     1, // E18
     8, // E19
    15, // E20
    22, // E21
    29, // E22
     5, // E23
    12, // E24
    19, // E25
    26, // E26
     2, // E27
     9, // E28
    16, // E29
    23, // E30
    30, // E31
  };

  /* Local variables */
  int     i;        /* Loop counter */
  dword   bit_mask; /* Bit mask */
  dword   nesn;     /* new ESN */

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  /* initialize new esn to 0 */
  nesn = 0L;

  for (i = 0, bit_mask = 1L; i < 32; ++i, bit_mask <<= 1)
  {
    if (esn & bit_mask)
    {
      nesn |= 1L << permutation_array[i];
    }
  }

  return( nesn );

}/* permute_esn */

/*===========================================================================

FUNCTION MCC_GET_MSG

DESCRIPTION
  This procedure checks inside the base station information array for a
  message received for the currently active base station and returns a
  pointer to the requested message if a valid message is found.

DEPENDENCIES
  Requested Message must be one that is stored in the base station
  information array.

RETURN VALUE
  A pointer to the requested message if a valid message is found.  NULL if
  the message is not found.

SIDE EFFECTS
  During this procedure the base station information array will be checked
  for old information.  Any base station whose information is over 10
  minutes old will be marked inactive.

===========================================================================*/

void *mcc_get_msg
(
  word msg_type
    /* message type to retrieve */
)
{
  void *msg_ptr;
    /* message pointer to return */

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  /* initialize return pointer to null */
  msg_ptr = NULL;

  /* -----------------------------------------------
  ** Check to see if chosen message exits and return
  ** a pointer to the message if it is found.
  ** ----------------------------------------------- */

  switch (msg_type )
  {

    #ifdef FEATURE_IS2000_REL_A_CC
    case CAI_EN_AC_PARM_MSG:
    {
      if (OVHD_CHAN_IS_BCCH && cur_bs_ptr->rx.eapm_rx &&
          (cur_bs_ptr->eapm.acc_msg_seq == cur_bs_ptr->rx.acc_msg_seq))
      {
        /* Return pointer to enhanced access parameters message */
        msg_ptr = &cur_bs_ptr->eapm;
      }
      break;
    }
    #endif // FEATURE_IS2000_REL_A_CC

    case CAI_SYS_PARM_MSG:
    {
      if ((OVHD_CHAN_IS_PCH && SP_MSG_IS_RECVD)
          #ifdef FEATURE_IS2000_REL_A_CC
          || (OVHD_CHAN_IS_BCCH && ANSI41SP_MSG_IS_RECVD && MCRRP_MSG_IS_RECVD)
          #endif /* FEATURE_IS2000_REL_A_CC */
         )
      {
        /* Return pointer to system parameters message */
        msg_ptr = &cur_bs_ptr->csp.sp;
      }
      break;
    }

    case CAI_ACC_PARM_MSG:
    {
      if (OVHD_CHAN_IS_PCH && cur_bs_ptr->rx.ap_rx &&
          (cur_bs_ptr->ap.acc_msg_seq == cur_bs_ptr->rx.acc_msg_seq))
      {
        /* Return pointer to access parameters message */
        msg_ptr = &cur_bs_ptr->ap;
      }
      break;
    }

    case CAI_EXT_SYS_PARM_MSG:
    {
      if ( (OVHD_CHAN_IS_PCH && ESP_MSG_IS_RECVD)
           #ifdef FEATURE_IS2000_REL_A_CC
           || (OVHD_CHAN_IS_BCCH && ANSI41SP_MSG_IS_RECVD && MCRRP_MSG_IS_RECVD)
           #endif /* FEATURE_IS2000_REL_A_CC */
         )
      {
        /* Return pointer to Extended System Parameters message */
        msg_ptr = &cur_bs_ptr->csp.esp;
      }
      break;
    }

    default:
    {
      ERR_FATAL( "mc_get_msg bad msg_type %d", msg_type, 0 , 0 ); 
    }
  } /* end of switch */

  return(msg_ptr);

} /* mcc_get_msg */

/*===========================================================================

FUNCTION MCC_GET_BS_INFO

DESCRIPTION
  This procedure checks inside the base station information array for an
  entry for the specified paging channel and returns a pointer to the
  information if the entry is found.  The time input parameter specifies
  when the information message will be used (thus can be used to check for
  expiration).  If NULL (a pointer) time is specified, the information will
  be returned without checking for expiration.

DEPENDENCIES
  None

RETURN VALUE
  TRUE if the requested information is found.  FALSE otherwise.

SIDE EFFECTS
  During this procedure the base station information array will be checked
  for old information.  Any base station whose information is over 10
  minutes old will be marked inactive.

===========================================================================*/

boolean mcc_get_bs_info
(
  bs_info_id_type *bs_info_id,
    /* specifies the broadcast control control or paging channel to which the
       message belongs */
  qword info_use_time,
    /* the time the bs_info is used */
  cdma_bs_type **bs
    /* for returning the BS info */
)
{
  uint16 i;
  qword delta_time;

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  /* Search base station information array for entry */
  for ( i = 0; i < MC_MAX_BASES; i++ )
  {
    /* Note: band_class is not checked here because this function does
       note support multiple band class yet.  When the Tri-mode code is
       merged in, the band_class checking here can be added */
    if ((bs_info[i].valid_pn) &&
        (bs_info[i].pilot_pn == bs_info_id->pilot_pn) &&
        (bs_info[i].cdma_ch == bs_info_id->cdma_ch) &&
        (bs_info[i].band_class == bs_info_id->band_class) &&
        (bs_info[i].ovhd_chan_info.ovhd_walsh_ch == bs_info_id->ovhd_walsh_ch) &&
        (bs_info[i].ovhd_chan_info.ovhd_chan == bs_info_id->ovhd_chan))
    {
      if ( info_use_time != 0 ) // Use time specified, check for expiration
      {
        /* Check timestamp of last overhead msg on this paging channel */
        qw_sub( delta_time, info_use_time, bs_info[i].rx.msg_time );

        if ( !(qw_hi( delta_time ) == 0) ||
             (qw_lo( delta_time ) > MCC_VALID_CONFIG))
        {
          /* Overhead msg is stale (older than 10 minutes) */
          /* We can return failure here */
          return FALSE;
        }
      }

      // If we are here, we pass all criteria and is time to return info.
      *bs = &bs_info[i];
      return TRUE;
    }
  } /* end of for */

  /* If we ever get here, we couldn't find the specified paging channel or
     broadcast control channel so return FALSE here */
  return FALSE;
 
} /* mcc_get_bs_info */

/*===========================================================================

FUNCTION MCC_RESET_IDLE_FRAMES

DESCRIPTION
  This function resets the mcc_idle_frames and mcc_idle_pg_frames array
  to indicate that no messages or page records received on the Paging
  Channel or Forward Common Control Channel will be duplicates.

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/

void mcc_reset_idle_frames( void )
{
  word i,j,k;

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  for (i=0; i < (word) MCC_MAX_ACK_TYPES; i++)
  {
    for (j=0; j < (word) MCC_MAX_MSG_SEQ; j++)
    {
      for (k=0; k < (word) MCC_MAX_ADDR_TYPE; k++)
      {
        qw_set(mcc_idle_frames[i][j][k], (dword) 0, (dword) 0);
      }
    }
  }

  /* Initialize timestamps for page records */
  for (i=0; i < (word) MCC_MAX_MSG_SEQ; i++)
  {
    qw_set(mcc_idle_pg_frames[i], (dword) 0, (dword) 0);
  }

} /* mcc_reset_idle_frames */

/*===========================================================================
FUNCTION MCC_INIT_OVHD_INFO

DESCRIPTION
 This function initialises the Base station Info array to ensure that
 all the overhead messages are marked as invalid. This is done so that
 the Overhead Messages are processed properly when we re-enter CDMA.

 This function must be called each time CDMA is entered.

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

=============================================================================*/

void mcc_init_ovhd_info()
{
  word i;
    /* index to loop through array */

  static boolean initialized = FALSE;
    /* One time full initialization */

  /* --------------------------------------------------------------
  ** The initialization of the bs_info ensures that once the phone
  ** exits CDMA mode (after a call, for example) that all overhead
  ** messages are re-processed correctly when we re-enter CDMA.
  ** The Neighbor List is left in the array, if possible, to keep
  ** the phone well informed during quick idle handoffs upon entry.
  ** -------------------------------------------------------------- */
  if ( !initialized )
  {
    /* One time, initialization of bs_info database. */
    memset(bs_info, 0, sizeof(bs_info));
  }

  for (i=0; i< MC_MAX_BASES; i++)
  {
    if (!initialized)
    {
      bs_info[i].valid_pn  = FALSE;
      bs_info[i].rx.nl_rx  = FALSE;

      bs_info[i].rx.gnl_rx = FALSE;

      #ifdef FEATURE_IS2000_REL_A_CC
      bs_info[i].rx.unlm_rx = FALSE;
      #endif /* FEATURE_IS2000_REL_A_CC */

      initialized = TRUE;
    }

    bs_info[i].ovhd_chan_info_valid = FALSE;
    bs_info[i].nl_config_msg_seq = INVALID_SEQ;
    bs_info[i].rx.acc_msg_seq = INVALID_SEQ;
    bs_info[i].rx.sy_rx  = FALSE;
    bs_info[i].rx.sp_rx  = FALSE;
    bs_info[i].rx.cl_rx  = FALSE;
    bs_info[i].rx.ap_rx  = FALSE;
    bs_info[i].rx.esp_rx = FALSE;
    bs_info[i].rx.gsd_rx = FALSE;

    bs_info[i].gnl_config_msg_seq = INVALID_SEQ;

    bs_info[i].rx.egsd_rx = FALSE;
    bs_info[i].rx.ecl_rx = FALSE;

    #ifdef FEATURE_IS2000_REL_A_CC
    bs_info[i].unlm_config_msg_seq = INVALID_SEQ;
    bs_info[i].rx.a41spm_rx = FALSE;
    bs_info[i].rx.mcrrpm_rx = FALSE;
    bs_info[i].rx.eapm_rx = FALSE;

    #ifdef FEATURE_AUTH
    bs_info[i].rx.a41randm_rx = FALSE;
    #endif /* FEATURE_AUTH */

    #endif /* FEATURE_IS2000_REL_A_CC */
  }

} /* mcc_init_ovhd_info */

#ifdef FEATURE_IS2000_REL_B
/*===========================================================================

FUNCTION MCC_IS_PCH_BCCH_INFO_AVAILABLE

DESCRIPTION
  This function checks if a PCH/BCCH info is available for a given BS 
  identified by CDMA_FREQ/BAND_CLASS/PILOT_PN. If both paging and BCCH info are
  present for this BS, the first BCCH info present will be returned.  

DEPENDENCIES
  For PCH/BCCH info to be valid, ovhd mesgs need not be current.

RETURN VALUE
  None.

SIDE EFFECTS
 None

===========================================================================*/

boolean mcc_is_pch_bcch_info_available
(
  bs_info_id_type * bs_info_id,
  int * index_found
)
{
  int i;
  /* Get current system time in units of 20 msecs */
  boolean status = FALSE;

  /* Search base station information array for entry */
  for (i=0; i< MC_MAX_BASES; i++)
  {
    if ((bs_info[i].valid_pn) &&
        (bs_info[i].pilot_pn == bs_info_id->pilot_pn) &&
        (bs_info[i].cdma_ch == bs_info_id->cdma_ch) &&
        (bs_info[i].band_class == bs_info_id->band_class) &&
        bs_info[i].ovhd_chan_info_valid
       )
    {
      /* Okay we found this BS in our stored info, return its index back*/
      *index_found = i;
      status = TRUE;
      /* If we found a REL A common channel entry, quit right away, 
      this helps with cases when we have an old and a new common channel
      entry for the same BS */
      if (bs_info[i].ovhd_chan_info.ovhd_chan == CAIX_BCCH)
      {
        break;
      }
    }
  } /* end of for */

  return status;
 
}
#endif /* FEATURE_IS2000_REL_B */

/*===========================================================================

FUNCTION MCC_ENTRY_INIT

DESCRIPTION
  This procedure initializes an entry in the base station information array
  with the parameters for the base station having the given pilot_pn and
  using the given cdma channel and paging channel.  If an entry already exists
  for the base station with the given pilot_pn and channel configuration, a
  new entry will not be created.  Since a base station may have different
  parameters for different channels, a separate entry will be kept for each
  different channel we use.  This procedure will set the current base station
  pointer to the correct entry.

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  If the base station information array is full, this procedure will
  replace the entry with the oldest time stamp with this entry.

===========================================================================*/

static qword mcc_old_time;
  /* Temporary holder for smallest time stamp (oldest entry) */

void mcc_entry_init
(
  bs_info_id_type *bs_info_id,
    /* Keys to identify entry in stored BS info */
  boolean         td_supported
    /* Indicates if Transmit Diversity is supported */
)
{
  word i;
    /* index to loop through array */
  qword current_time;
    /* temporary holder for current time stamp */
  word old_index = 0;
    /* index of oldest item on the list */
  boolean good_old = FALSE;
    /* assume old data for neighbor not good */
  boolean wipe_out_bs_entry = TRUE;
    /* Indicates if bs entry content shalll be wipe out. */

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/


  /* Get current system time in units of 20 msecs */
  ts_get_20ms_frame_time( current_time );

  /* Search base station information array for entry */
  for (i=0; i< MC_MAX_BASES; i++)
  {
    if ( bs_info_id->pilot_pn == PSEUDO_PILOT_PN ) 
    {
      if ( bs_info[i].pilot_pn == bs_info_id->pilot_pn ) 
      {
        /* Old entry of Pseudo BS found. */
        break;
      }
    }
    else if ( (bs_info[i].valid_pn) 
              && (bs_info[i].pilot_pn == bs_info_id->pilot_pn) 
              && (bs_info[i].cdma_ch == bs_info_id->cdma_ch) 
              && (bs_info[i].band_class == bs_info_id->band_class) 
              && (bs_info[i].ovhd_chan_info.ovhd_walsh_ch == bs_info_id->ovhd_walsh_ch)
              && (bs_info[i].ovhd_chan_info.ovhd_chan == bs_info_id->ovhd_chan)
            )
    {
      /* MS previously visited the BS. Don't wipe out the entry content as
         overhead may not be changed. */
      wipe_out_bs_entry = FALSE;

      /* In idle state of Pseudo-Idle mode, don't check the freshness (T31M) of stored 
         overheads.  Count on config_msg_seq to detect freshneess of overheads. */
      if ( ( MAIN_STATE(cdma.curr_state) == CDMA_ORIG ) /*lint !e641 */
           && ( cdma.entry_ptr->entry == MCC_PSEUDO_IDLE ) )
      {
        MSG_MED( "Pseudo-Idle, ignore T31m requirement", 0, 0, 0 );
        good_old = TRUE;
      } 
      else 
      {
        /* Check timestamp of last overhead msg on this paging channel */
        qw_sub( mcc_old_time, current_time, bs_info[i].rx.msg_time );

        if ( ( qw_hi( mcc_old_time ) == 0 ) &&
             ( qw_lo( mcc_old_time ) < MCC_VALID_CONFIG ) )
        {
          /* Last overhead msg received within last 10 minutes */
          good_old = TRUE;
        }
      }

      break;
    }

  } /* end of for */

  if (i == MC_MAX_BASES)
  {
    /* -------------------------------------------------------------------
    ** Base station not already in the array - Search through base station
    ** info array looking for an empty entry or the oldest time stamp
    ** ------------------------------------------------------------------- */

    /* Initialize comparator */
    qw_set( mcc_old_time, 0xFFFFFFFFUL, 0xFFFFFFFFUL );

    for (i=0; i < MC_MAX_BASES; i++)
    {
      if (!bs_info[i].valid_pn)           /* empty entry */
      {
        break;
      }
      else if (qw_cmp( bs_info[i].rx.msg_time, mcc_old_time ) < 0)
      {                                     /* if old timestamp */
        old_index = i;                      /* set oldest entry */
        qw_equ( mcc_old_time, bs_info[i].rx.msg_time );  /* set oldest time */
      }
    } /* end of for */
  } /* if */

  if (i == MC_MAX_BASES)
  {
    /* no empty entry was found -- have to overwrite oldest entry */

    i = old_index;
  }

  /* Initialize the new bs entry. */
  if ( wipe_out_bs_entry )
  {
    memset(&bs_info[i], 0, sizeof(cdma_bs_type));
  }

  /* Initialize flags in new entry if needed */
  if (!good_old)
  {
    /* Place current time in the rx.msg_time field */
    qw_equ( bs_info[i].rx.msg_time, current_time );

    bs_info[i].rx.config_msg_seq = INVALID_SEQ;
    bs_info[i].rx.acc_msg_seq = INVALID_SEQ;
    bs_info[i].rx.sy_rx  = FALSE;
    bs_info[i].rx.sp_rx  = FALSE;
    bs_info[i].rx.cl_rx  = FALSE;
    bs_info[i].rx.nl_rx  = FALSE;
    bs_info[i].rx.ap_rx  = FALSE;
    bs_info[i].rx.esp_rx = FALSE;
    bs_info[i].rx.gsd_rx = FALSE;

    bs_info[i].rx.gnl_rx = FALSE;

    bs_info[i].rx.egsd_rx = FALSE;
    bs_info[i].rx.ecl_rx = FALSE;

    #ifdef FEATURE_IS2000_REL_A_CC
    bs_info[i].rx.a41spm_rx = FALSE;
    bs_info[i].rx.mcrrpm_rx = FALSE;
    bs_info[i].rx.unlm_rx = FALSE;
    bs_info[i].rx.eapm_rx = FALSE;

    #ifdef FEATURE_AUTH
    bs_info[i].rx.a41randm_rx = FALSE;
    #endif /* FEATURE_AUTH */

    #endif /* FEATURE_IS2000_REL_A_CC */

    /* No copying of anchor BS info is needed for Pseudo BS entry. */
    if ( bs_info_id->pilot_pn != PSEUDO_PILOT_PN ) 
    {
      /* If ESPM or MCRRM is not received, do address match using IMSI_S only. 
         Otherwise if a MS directed message or Page Message is received before 
         those, it will be discarded */
      bs_info[i].csp.esp.mcc = cdma.mcc;
      bs_info[i].csp.esp.imsi_11_12 = cdma.imsi_11_12;

      /* Initialize SPM / ANSI41SPM and ESPM / MCRRPM from old pilot,
         so that TCI has valid parameters to use in case ECAM comes before
         those overhead messages are received after APHO and AHO */

      if ( (cdma.ho_state == CDMA_AHO) || (cdma.ho_state == CDMA_APHO) )
      {
        bs_info[i].csp = cur_bs_ptr->csp;
      }
    }
  }

  bs_info[i].valid_pn = TRUE;
  bs_info[i].pilot_pn = bs_info_id->pilot_pn;
  bs_info[i].cdma_ch = bs_info_id->cdma_ch;
  bs_info[i].band_class = bs_info_id->band_class;
  bs_info[i].ovhd_chan_info.ovhd_chan = bs_info_id->ovhd_chan;
  bs_info[i].td_supported = td_supported;

  /* No copying of anchor BS info is needed for Pseudo BS entry. */
  if ( bs_info_id->pilot_pn != PSEUDO_PILOT_PN ) 
  {
    bs_info[i].ovhd_chan_info_valid = TRUE;

    /* Ovhd Walsh (for PCH or pri-BCCH channel) */
    bs_info[i].ovhd_chan_info.ovhd_walsh_ch = bs_info_id->ovhd_walsh_ch;

    /* 
      Note: this key has to be remembered for QPCH operation
      We can't generate the QPCH number here yet because the ESPM
      may not have been received.
    */
    bs_info[i].imsi_hash_key = cdma.imsi_hash_key;

    #ifdef FEATURE_AUTH
    /*-----------------------------------------------------------------------
    ** If mobile is in AHO/APHO, authentication parameters remain the same 
    ** as anchor base station and no update from new base station is allowed.
    **----------------------------------------------------------------------- */
    if ((cdma.curr_state == CDMA_ORIG) || (cdma.curr_state == CDMA_PAGE_RES)) /*lint !e641 */
    {
      bs_info[i].rp = cur_bs_ptr->rp;
    }
    #endif /* FEATURE_AUTH */

    #ifdef FEATURE_IS2000_REL_B
    if (td_supported)
    {
      bs_info[i].td_mode = cdma.td_mode;
      bs_info[i].td_power_level = cdma.td_power_level;
    }

    /* PRAT is only valid if ovhd_chan is PC */
    bs_info[i].ovhd_chan_info.prat = cdma.prat;

    /* BRAT  and BCCH code rate is only valid if ovhd_chan is BCCH */
    bs_info[i].ovhd_chan_info.brat = cdma.brat;
    bs_info[i].ovhd_chan_info.bcch_code_rate = cdma.bcch_code_rate;
    #endif /* FEATURE_IS2000_REL_B */

  }

  #ifdef FEATURE_IS2000_REL_D
#error code not present
  #endif /* FEATURE_IS2000_REL_D */

  /* set current pointer to new entry */
  cur_bs_ptr = &bs_info[i];

  #ifdef FEATURE_IS2000_REL_A_CC
  /* ------------------------------------------------------------
  ** Since MS handoff to another BS, empty UPM reassembly buffer.
  ** ------------------------------------------------------------ */
  mcc_empty_seg_upm_buffer();
  #endif /* FEATURE_IS2000_REL_A_CC */

  /* ---------------------------------------------------------------------
  ** We have changed paging channel, frequency, or pilot PN.  Any messages
  ** we receive in the future should not be considered duplicates.
  ** --------------------------------------------------------------------- */
  mcc_reset_idle_frames();

  if ( bs_info_id->pilot_pn != PSEUDO_PILOT_PN ) 
  {
    /* Initialize paging channel acquisition flag */
    mclog_report_event_pch_acquired(FALSE, 0, 0);

    #ifdef FEATURE_IS2000_REL_A_CC
    /* Initialize broadcast control channel acquisition flag */
    mclog_report_event_bcch_acquired(FALSE, 0, 0);
    #endif /* FEATURE_IS2000_REL_A_CC */
  }
 
} /* mcc_entry_init */

/*===========================================================================

FUNCTION MCC_PUT_SEQ

DESCRIPTION
  This procedure places a message sequence number (overhead or access
  parameters) into the base station information array for the currently
  active base station.

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  Message receipt flags for the base station may change.

===========================================================================*/

void mcc_put_seq
(
  byte seq_num,                /* sequence number to insert */
  boolean acc_seq              /* TRUE = acc_msg_seq, FALSE = ovhd_msg_seq */
)
{
  if (acc_seq)
  {
    cur_bs_ptr->rx.acc_msg_seq = seq_num;
  }
  else
  {
    cur_bs_ptr->rx.config_msg_seq = seq_num;
  }
} /* mcc_put_seq */

/*===========================================================================

FUNCTION MCC_PUT_MSG

DESCRIPTION
  This procedure places a message for the currently active base station
  into the base station information array.

DEPENDENCIES
  Message to be placed in base station information array must be a
  valid type.

RETURN VALUE
  None.

SIDE EFFECTS
  This procedure will examine the message sequences of the messages
  being placed into the base station information array and adjust the
  message receipt flags and the message sequence numbers accordingly.

===========================================================================*/

void mcc_put_msg
(
  byte msg_type,      /* message type being placed in base station info */
  caii_rx_msg_type *msg_ptr          /* pointer to message to be placed */
)
{
  word i;

  #ifndef FEATURE_IS2000_REL_A_CC
  boolean qpch_in_use;
  #endif /* !FEATURE_IS2000_CC */

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  switch( msg_type )
  {
    case MC_SYNC_MSG:
      /* store sync msg and mark msg received */
      cur_bs_ptr->sy = *(caii_sync_type *)msg_ptr ;
      cur_bs_ptr->rx.sy_rx = TRUE;

      // Notify reception of PREV and SID.  For special MDR, we use base station's
      // prev instead of prev_in_use
      break;

    case CAI_SYS_PARM_MSG:
      /* Store system parameters msg and mark message as received */

      /* ------------------------------------------------------------------
      ** For AHO and APHO, MS uses the anchor base station's REG_ZONE value 
      ** instead of the value from the new base station. So, no need to 
      ** detect REG_ZONE change. 
      ** ------------------------------------------------------------------ */
      if ((cdma.curr_state != CDMA_ORIG) && (cdma.curr_state != CDMA_PAGE_RES)) /*lint !e641 */
      {
        /* Event report: Reg zone change */
        if (SP_MSG_NOT_RECVD ||
            (cur_bs_ptr->csp.sp.reg_zone != msg_ptr->sysparm.reg_zone))
        {
          event_report_payload(EVENT_REG_ZONE_CHANGE,
            sizeof(event_reg_zone_change_type), &msg_ptr->sysparm.reg_zone);
        }
      }

      #ifndef FEATURE_IS2000_REL_A_CC
      /* Event report: QPCH in use */
      if ((cdma.curr_state == CDMA_SLOTTED) &&
          ((cur_bs_ptr->csp.sp.ext_sys_parameter
            != msg_ptr->sysparm.ext_sys_parameter) &&
           !msg_ptr->sysparm.ext_sys_parameter))
      {
        qpch_in_use = FALSE;
        event_report_payload(EVENT_QPCH_IN_USE,
          sizeof(event_qpch_in_use_type), &qpch_in_use);
      }
      #endif /* !defined(FEATURE_IS2000_CC */

      cur_bs_ptr->csp.sp = *(caii_sysparm_type *)msg_ptr;
      cur_bs_ptr->rx.config_msg_seq = cur_bs_ptr->csp.sp.config_msg_seq;
      cur_bs_ptr->rx.sp_rx = TRUE;
      break;

    case CAI_ACC_PARM_MSG:
      /* Store access parameters msg and mark message as received */
      cur_bs_ptr->ap = *(caii_accparm_type *)msg_ptr ;

      #ifdef FEATURE_AUTH
      /* store rand/auth parameters */
      cur_bs_ptr->rp.auth = cur_bs_ptr->ap.auth;
      cur_bs_ptr->rp.rand = cur_bs_ptr->ap.rand;
      #endif /* FEATURE_AUTH */

      cur_bs_ptr->rx.acc_msg_seq = cur_bs_ptr->ap.acc_msg_seq ;
      cur_bs_ptr->rx.ap_rx = TRUE;
      break;

    case CAI_EXT_NL_MSG:
      /* Set the proper configuration values */
      cur_bs_ptr->nl_config_msg_seq = msg_ptr->ext_nghbr.config_msg_seq;
      cur_bs_ptr->rx.config_msg_seq = msg_ptr->ext_nghbr.config_msg_seq;
      cur_bs_ptr->rx.nl_rx = TRUE;
      break;

    case CAI_NL_MSG:
      /* Set the proper configuration values */
      cur_bs_ptr->nl_config_msg_seq = msg_ptr->nghbr.config_msg_seq;
      cur_bs_ptr->rx.config_msg_seq = msg_ptr->nghbr.config_msg_seq;
      cur_bs_ptr->rx.nl_rx = TRUE;
      break;

    case CAI_GEN_NL_MSG:
      /* Set the proper configuration values */
      cur_bs_ptr->rx.config_msg_seq = msg_ptr->gen_nghbr.config_msg_seq;
      cur_bs_ptr->gnl_config_msg_seq = cur_bs_ptr->rx.config_msg_seq;
      cur_bs_ptr->rx.gnl_rx = TRUE;

      /* ----------------------------------------------------------------
      ** If NGHBR_CONFIG_PN_INCL is equal to 1 and FREQ_FIELDS_INCL
      ** is equal to 1 set the config_msg_seq of NLM and ENLM as current
      ** and mark these messages are received.
      ** ---------------------------------------------------------------*/
      if ((msg_ptr->gen_nghbr.nghbr_config_pn_incl) &&
          (msg_ptr->gen_nghbr.freq_fields_incl))
      {
        cur_bs_ptr->nl_config_msg_seq   = msg_ptr->gen_nghbr.config_msg_seq;
        cur_bs_ptr->rx.nl_rx = TRUE;
      }
      break;

    case CAI_CL_MSG:
      /* Store Channel List in structure and mark message as received */
      cur_bs_ptr->cl.pilot_pn       = msg_ptr->chnlist.pilot_pn;
      cur_bs_ptr->cl.config_msg_seq = msg_ptr->chnlist.config_msg_seq;
      cur_bs_ptr->cl.num_freq       = msg_ptr->chnlist.num_freq;

      for (i=0; i < msg_ptr->chnlist.num_freq; i++)
      {
        cur_bs_ptr->cl.cdma_freq[i] = msg_ptr->chnlist.cdma_freq[i];
      }

      cur_bs_ptr->rx.config_msg_seq = cur_bs_ptr->cl.config_msg_seq;
      cur_bs_ptr->rx.cl_rx = TRUE;
      break;

    case CAI_EXT_CDMA_CHN_LIST_MSG:
      cur_bs_ptr->ecl = *(caii_ext_chnlist_type *)msg_ptr ;
      cur_bs_ptr->rx.config_msg_seq = cur_bs_ptr->ecl.config_msg_seq;
      cur_bs_ptr->rx.ecl_rx = TRUE;
      break;

    case CAI_EXT_SYS_PARM_MSG:
      /* Store Extended System Parameters msg and mark message as received */

      #ifndef FEATURE_IS2000_REL_A_CC
      /* Event report: QPCH in use */
      if (cdma.curr_state == CDMA_SLOTTED)
      {
        if (P_REV_IS_6_OR_GREATER && (msg_ptr->ext_sysparm.p_rev < P_REV_IS2000))
        { 
          qpch_in_use = FALSE;
          event_report_payload(EVENT_QPCH_IN_USE,
            sizeof(event_qpch_in_use_type), &qpch_in_use); 
        }
        else if (!P_REV_IS_6_OR_GREATER && 
                 (msg_ptr->ext_sysparm.p_rev >= P_REV_IS2000))
        {
          if (cdma.mob_cai_rev >= P_REV_IS2000)
          {
            qpch_in_use = msg_ptr->ext_sysparm.qpch_supported;
          }
          else
          {
            qpch_in_use = FALSE;
          }

          event_report_payload(EVENT_QPCH_IN_USE,
            sizeof(event_qpch_in_use_type), &qpch_in_use);
        }
        else if (cur_bs_ptr->csp.esp.qpch_supported != msg_ptr->ext_sysparm.qpch_supported)
        {
          event_report_payload(EVENT_QPCH_IN_USE, 
            sizeof(event_qpch_in_use_type), 
            &msg_ptr->ext_sysparm.qpch_supported);
        }
      }
      #endif /* !FEATURE_IS2000_REL_A_CC */

      cur_bs_ptr->csp.esp = *(caii_ext_sysparm_type *)msg_ptr ;
      cur_bs_ptr->csp.ota_mcc         = cur_bs_ptr->csp.esp.mcc;
      cur_bs_ptr->csp.ota_imsi_11_12  = cur_bs_ptr->csp.esp.imsi_11_12;
     
      cur_bs_ptr->rx.config_msg_seq = cur_bs_ptr->csp.esp.config_msg_seq;
      cur_bs_ptr->rx.esp_rx = TRUE;
      break;

    case CAI_EXT_GLB_REDIR_MSG:
     /* Store Global Service Redirection msg and mark message as received */
      cur_bs_ptr->egsd = *(caii_ext_glb_redir_type *)msg_ptr ;
      cur_bs_ptr->rx.config_msg_seq = cur_bs_ptr->egsd.config_msg_seq;
      cur_bs_ptr->rx.egsd_rx = TRUE;
      break;


    case CAI_GLOBAL_REDIR_MSG:
      /* Store Global Service Redirection msg and mark message as received */
      cur_bs_ptr->gsd = *(caii_glob_redirect_type *)msg_ptr ;

      /* Initialize IS2000 related fields */
      if (!P_REV_IS_6_OR_GREATER)
      {
        cur_bs_ptr->gsd.excl_p_rev_ms = 0;
      }
      cur_bs_ptr->rx.config_msg_seq = cur_bs_ptr->gsd.config_msg_seq;
      cur_bs_ptr->rx.gsd_rx = TRUE;
      break;

    #ifdef FEATURE_IS2000_REL_A_CC
    case CAI_MC_RR_PARM_MSG:
    {
      caii_mc_rr_parm_type * temp_msg_ptr;
      temp_msg_ptr = (caii_mc_rr_parm_type *) msg_ptr;

      /* Store all fields (including all optional fields since parser
         has initialized them properly) of this message in SPM and
         ESPM data structures. */
      cur_bs_ptr->csp.sp.pilot_pn = temp_msg_ptr->pilot_pn;
      cur_bs_ptr->csp.sp.base_id = temp_msg_ptr->base_id;
      cur_bs_ptr->csp.sp.rescan = FALSE;
      cur_bs_ptr->csp.esp.p_rev = temp_msg_ptr->p_rev;
      cur_bs_ptr->csp.esp.min_p_rev = temp_msg_ptr->min_p_rev;
      /* We are not supporting Spreading-Rate 3 for now */
      cur_bs_ptr->csp.sp.srch_win_a = temp_msg_ptr->srch_win_a;
      cur_bs_ptr->csp.sp.srch_win_r = temp_msg_ptr->srch_win_r;
      cur_bs_ptr->csp.sp.t_add = temp_msg_ptr->t_add;
      cur_bs_ptr->csp.sp.t_drop = temp_msg_ptr->t_drop;
      cur_bs_ptr->csp.sp.t_comp = temp_msg_ptr->t_comp;
      cur_bs_ptr->csp.sp.t_tdrop = temp_msg_ptr->t_tdrop;
      cur_bs_ptr->csp.sp.nghbr_max_age = temp_msg_ptr->nghbr_max_age ;
      cur_bs_ptr->csp.esp.soft_slope = temp_msg_ptr->soft_slope;
      cur_bs_ptr->csp.esp.add_intercept = temp_msg_ptr->add_intercept ;
      cur_bs_ptr->csp.esp.drop_intercept = temp_msg_ptr->drop_intercept ;
      cur_bs_ptr->csp.esp.enc_supported = temp_msg_ptr->enc_supported;
      cur_bs_ptr->csp.esp.sig_encrypt_sup = temp_msg_ptr->sig_encrypt_sup;
      cur_bs_ptr->csp.esp.ui_encrypt_sup = temp_msg_ptr->ui_encrypt_sup;

      if (temp_msg_ptr->add_fields_len > 0)
      {
        MSG_MED("MC-RR parms msg: ADD_FIELDS_LEN > 0", 0, 0, 0);
      }

      cur_bs_ptr->csp.esp.mcc = temp_msg_ptr->mcc ;
      cur_bs_ptr->csp.esp.imsi_11_12 = temp_msg_ptr->imsi_11_12 ;
      
      cur_bs_ptr->csp.ota_mcc         = temp_msg_ptr->mcc;
      cur_bs_ptr->csp.ota_imsi_11_12  = temp_msg_ptr->imsi_11_12;
      
      cur_bs_ptr->csp.sp.max_slot_cycle_index = temp_msg_ptr->max_slot_cycle_index;
      cur_bs_ptr->csp.sp.pwr_rep_thresh = temp_msg_ptr->pwr_rep_thresh;
      cur_bs_ptr->csp.sp.pwr_rep_frames = temp_msg_ptr->pwr_rep_frames;
      cur_bs_ptr->csp.sp.pwr_thresh_enable = temp_msg_ptr->pwr_thresh_enable;
      cur_bs_ptr->csp.sp.pwr_period_enable = temp_msg_ptr->pwr_period_enable;
      cur_bs_ptr->csp.sp.pwr_rep_delay = temp_msg_ptr->pwr_rep_delay;
      cur_bs_ptr->csp.esp.reselect_included = temp_msg_ptr->reselect_included ;
      cur_bs_ptr->csp.esp.ec_thresh = temp_msg_ptr->ec_thresh ;
      cur_bs_ptr->csp.esp.ec_io_thresh = temp_msg_ptr->ec_io_thresh ;
      cur_bs_ptr->csp.sp.base_lat = temp_msg_ptr->base_lat ;
      cur_bs_ptr->csp.sp.base_long = temp_msg_ptr->base_long ;
      cur_bs_ptr->csp.esp.pilot_report = temp_msg_ptr->pilot_report ;
      cur_bs_ptr->csp.esp.acc_ent_ho_order = temp_msg_ptr->acc_ent_ho_order ;
      cur_bs_ptr->csp.esp.access_ho = temp_msg_ptr->access_ho ;
      cur_bs_ptr->csp.esp.access_ho_msg_rsp = temp_msg_ptr->access_ho_msg_rsp ;
      cur_bs_ptr->csp.esp.access_probe_ho = temp_msg_ptr->access_probe_ho ;
      cur_bs_ptr->csp.esp.acc_ho_list_upd = temp_msg_ptr->acc_ho_list_upd ;
      cur_bs_ptr->csp.esp.acc_probe_ho_other_msg = temp_msg_ptr->acc_probe_ho_other_msg ;
      cur_bs_ptr->csp.esp.max_num_probe_ho = temp_msg_ptr->max_number_probe_ho ;
      cur_bs_ptr->csp.fccch_info.num_fccch = temp_msg_ptr->num_fccch;
      cur_bs_ptr->csp.fccch_info.fccch_rate = (cai_fccch_rate_type) temp_msg_ptr->fccch_rate;
      cur_bs_ptr->csp.fccch_info.fccch_code_rate = (cai_code_rate_type) temp_msg_ptr->fccch_code_rate;

      for (i=0; i< temp_msg_ptr->num_fccch; i++)
      {
        cur_bs_ptr->csp.fccch_info.fccch_code_chan[i] = temp_msg_ptr->fccch_code_chan[i];
      }

      cur_bs_ptr->csp.esp.bcast_index = temp_msg_ptr->bcast_index;
      cur_bs_ptr->csp.bcast_bcch_info.num_bcch_bcast = temp_msg_ptr->num_bcch_bcast;

      for (i=0; i<temp_msg_ptr->num_bcch_bcast; i++)
      {
        cur_bs_ptr->csp.bcast_bcch_info.bcch_info[i].bcch_code_chan =
          temp_msg_ptr->bcch_info[i].bcch_code_chan;
        cur_bs_ptr->csp.bcast_bcch_info.bcch_info[i].brat =
          (cai_bcch_rate_type)temp_msg_ptr->bcch_info[i].brat;
        cur_bs_ptr->csp.bcast_bcch_info.bcch_info[i].bcch_code_rate =
          (cai_code_rate_type)temp_msg_ptr->bcch_info[i].bcch_code_rate;
      }

      cur_bs_ptr->csp.esp.qpch_supported = temp_msg_ptr->qpch_supported ;
      cur_bs_ptr->csp.esp.num_qpch = temp_msg_ptr->num_qpch ;
      cur_bs_ptr->csp.esp.qpch_rate = temp_msg_ptr->qpch_rate ;
      cur_bs_ptr->csp.esp.qpch_power_level_page =
        temp_msg_ptr->qpch_power_level_page ;
      cur_bs_ptr->csp.esp.qpch_cci_supported = temp_msg_ptr->qpch_cci_supported ;
      cur_bs_ptr->csp.esp.qpch_power_level_config =
        temp_msg_ptr->qpch_power_level_config;

      for (i=0; i < temp_msg_ptr->num_qpch; i++)
      {
        cur_bs_ptr->csp.qpch_code_chan[i] = temp_msg_ptr->qpch_code_chan[i];
      }

      cur_bs_ptr->csp.esp.qpch_bi_supported = temp_msg_ptr->qpch_bi_supported;
      cur_bs_ptr->csp.esp.qpch_power_level_bcast = temp_msg_ptr->qpch_power_level_bcast ;
      cur_bs_ptr->csp.esp.sdb_supported = temp_msg_ptr->sdb_supported ;
      cur_bs_ptr->csp.esp.broadcast_gps_asst = temp_msg_ptr->broadcast_gps_asst ;
      cur_bs_ptr->csp.esp.rlgain_traffic_pilot = temp_msg_ptr->rlgain_traffic_pilot ;
      cur_bs_ptr->csp.esp.rev_pwr_cntl_delay_incl =
        temp_msg_ptr->rev_pwr_cntl_delay_incl ;
      cur_bs_ptr->csp.esp.rev_pwr_cntl_delay = temp_msg_ptr->rev_pwr_cntl_delay ;
      cur_bs_ptr->csp.esp.mob_qos = temp_msg_ptr->mob_qos;
      cur_bs_ptr->csp.esp.use_sync_id = temp_msg_ptr->use_sync_id;

      /* Initialize the _incl fields to be compatible with existing code  */
      cur_bs_ptr->csp.esp.is95b_incl =
      cur_bs_ptr->csp.sp.is2000_incl = TRUE;

      /* ----------------------------------------------------------------------
      ** Set ext_sys_parameter and ext_chan_list to be compatible with existing
      ** code. Extended System Parameters Message is not sent on BCCH and all
      ** of its values are filled in by MC-RR Parameters Message and ANSI-41
      ** System Parameters Message.  Extended Channel List Message is always
      ** sent on BCCH.
      ** ---------------------------------------------------------------------*/
      cur_bs_ptr->csp.sp.ext_chan_list = TRUE;
      cur_bs_ptr->csp.sp.ext_sys_parameter = TRUE;

      #ifdef FEATURE_AUTH
      cur_bs_ptr->csp.sending_a41_rand = temp_msg_ptr->sending_rand;
      #endif /* FEATURE_AUTH */

      cur_bs_ptr->csp.sp.pri_nghbr_list = temp_msg_ptr->pri_nghbr_list;
      cur_bs_ptr->csp.sp.user_zone_id = temp_msg_ptr->user_zone_id;
      cur_bs_ptr->csp.sp.ext_global_redirect = temp_msg_ptr->ext_global_redirect;
      cur_bs_ptr->csp.esp.pilot_info_req_supported = temp_msg_ptr->pilot_info_req_supported;

      #ifdef FEATURE_IS2000_REL_B
      cur_bs_ptr->csp.esp.band_class_info_req = temp_msg_ptr->band_class_info_req;
      cur_bs_ptr->csp.esp.alt_band_class = temp_msg_ptr->alt_band_class;
      cur_bs_ptr->csp.esp.cdma_off_time_rep_sup_ind = temp_msg_ptr->cdma_off_time_rep_sup_ind;
      cur_bs_ptr->csp.esp.cdma_off_time_rep_threshold_unit = temp_msg_ptr->cdma_off_time_rep_threshold_unit;
      cur_bs_ptr->csp.esp.cdma_off_time_rep_threshold = temp_msg_ptr->cdma_off_time_rep_threshold;
      #endif /* FEATURE_IS2000_REL_B */

      #ifdef FEATURE_IS2000_REL_C
      cur_bs_ptr->csp.esp.chm_supported = temp_msg_ptr->chm_supported;
      cur_bs_ptr->csp.esp.release_to_idle_ind = temp_msg_ptr->release_to_idle_ind;
      cur_bs_ptr->csp.esp.reconnect_msg_ind = temp_msg_ptr->reconnect_msg_ind;
      cur_bs_ptr->csp.sp.t_tdrop_range_incl = temp_msg_ptr->t_tdrop_range_incl;
      cur_bs_ptr->csp.sp.t_tdrop_range = temp_msg_ptr->t_tdrop_range;
      cur_bs_ptr->csp.esp.for_pdch_supported = temp_msg_ptr->for_pdch_supported;
      cur_bs_ptr->csp.esp.pdch_chm_supported = temp_msg_ptr->pdch_chm_supported;
      cur_bs_ptr->csp.esp.pdch_parms_incl = temp_msg_ptr->pdch_parms_incl;
      cur_bs_ptr->csp.esp.for_pdch_rlgain_incl = temp_msg_ptr->for_pdch_rlgain_incl;
      cur_bs_ptr->csp.esp.rlgain_ackch_pilot = temp_msg_ptr->rlgain_ackch_pilot;
      cur_bs_ptr->csp.esp.rlgain_cqich_pilot = temp_msg_ptr->rlgain_cqich_pilot;
      cur_bs_ptr->csp.esp.num_soft_switching_frames = temp_msg_ptr->num_soft_switching_frames;
      cur_bs_ptr->csp.esp.num_softer_switching_frames = temp_msg_ptr->num_softer_switching_frames;
      cur_bs_ptr->csp.esp.num_soft_switching_slots = temp_msg_ptr->num_soft_switching_slots;
      cur_bs_ptr->csp.esp.num_softer_switching_slots = temp_msg_ptr->num_softer_switching_slots;
      cur_bs_ptr->csp.esp.pdch_soft_switching_delay = temp_msg_ptr->pdch_soft_switching_delay;
      cur_bs_ptr->csp.esp.pdch_softer_switching_delay = temp_msg_ptr->pdch_softer_switching_delay;
      cur_bs_ptr->csp.esp.walsh_table_id = temp_msg_ptr->walsh_table_id;
      cur_bs_ptr->csp.esp.num_pdcch = temp_msg_ptr->num_pdcch;

      for (i=0; i<=cur_bs_ptr->csp.esp.num_pdcch; i++)
      {
        cur_bs_ptr->csp.esp.for_pdcch_walsh[i] = temp_msg_ptr->for_pdcch_walsh[i];
      }

      cur_bs_ptr->csp.esp.imsi_10_incl = temp_msg_ptr->imsi_10_incl;
      cur_bs_ptr->csp.esp.imsi_10 = temp_msg_ptr->imsi_10;
      #endif /* FEATURE_IS2000_REL_C */

      #ifdef FEATURE_IS2000_REL_D
#error code not present
      #endif /* FEATURE_IS2000_REL_D */

      cur_bs_ptr->mcrrpm_config_msg_seq = msg_ptr->mcrrpm.config_msg_seq;
      cur_bs_ptr->rx.config_msg_seq = msg_ptr->mcrrpm.config_msg_seq;
      cur_bs_ptr->rx.mcrrpm_rx = TRUE;

      break;
    }

    case CAI_ANSI41_SYS_PARM_MSG:
    {
      caii_a41_sys_parm_type *temp_msg_ptr;
      temp_msg_ptr = (caii_a41_sys_parm_type *) msg_ptr;

      /* ------------------------------------------------------------------
      ** For AHO and APHO, MS uses the anchor base station's REG_ZONE value 
      ** instead of the value from the new base station. So, no need to 
      ** detect REG_ZONE change. 
      ** ------------------------------------------------------------------ */
      if ((cdma.curr_state != CDMA_ORIG) && (cdma.curr_state != CDMA_PAGE_RES)) /*lint !e641 */
      {
        /* Event report: Reg zone change */
        if (ANSI41SP_MSG_NOT_RECVD ||
            (cur_bs_ptr->csp.sp.reg_zone != temp_msg_ptr->reg_zone))
        {
          /* Event report: Reg Zone change */
          event_report_payload(EVENT_REG_ZONE_CHANGE,
                               sizeof(event_reg_zone_change_type), &temp_msg_ptr->reg_zone);
        }
      }

      /* Store all fields (including all optional fields since parser
         has initialized them properly) of this message in SPM and
         ESPM data structures. */
      cur_bs_ptr->csp.sp.pilot_pn = temp_msg_ptr->pilot_pn;
      cur_bs_ptr->csp.sp.sid = temp_msg_ptr->sid;
      cur_bs_ptr->csp.sp.nid = temp_msg_ptr->nid;
      cur_bs_ptr->csp.sp.reg_zone = temp_msg_ptr->reg_zone;
      cur_bs_ptr->csp.sp.total_zones = temp_msg_ptr->total_zones;
      cur_bs_ptr->csp.sp.zone_timer = temp_msg_ptr->zone_timer;
      cur_bs_ptr->csp.sp.mult_sids = temp_msg_ptr->mult_sids;
      cur_bs_ptr->csp.sp.mult_nids = temp_msg_ptr->mult_nids;
      cur_bs_ptr->csp.sp.home_reg = temp_msg_ptr->home_reg;
      cur_bs_ptr->csp.sp.for_sid_reg = temp_msg_ptr->for_sid_reg;
      cur_bs_ptr->csp.sp.for_nid_reg = temp_msg_ptr->for_nid_reg;
      cur_bs_ptr->csp.sp.power_up_reg = temp_msg_ptr->power_up_reg;
      cur_bs_ptr->csp.sp.power_down_reg = temp_msg_ptr->power_down_reg;
      cur_bs_ptr->csp.sp.parameter_reg = temp_msg_ptr->parameter_reg;
      cur_bs_ptr->csp.sp.reg_prd = temp_msg_ptr->reg_prd ;
      cur_bs_ptr->csp.sp.reg_dist = 0;
      cur_bs_ptr->csp.sp.reg_dist = temp_msg_ptr->reg_dist;
      cur_bs_ptr->csp.esp.packet_zone_id = temp_msg_ptr->packet_zone_id;
      cur_bs_ptr->csp.esp.delete_for_tmsi = temp_msg_ptr->delete_for_tmsi;
      /* Convert 2 bit pref_ms_id_type and 1 bit use_tmsi into a 3-bit
         pref_msid_type (similar to esp processing in caix) */
      cur_bs_ptr->csp.esp.pref_msid_type = temp_msg_ptr->pref_msid_type |
          (temp_msg_ptr->use_tmsi << 2);   /*lint !e734*/
      cur_bs_ptr->csp.esp.tmsi_zone_len = temp_msg_ptr->tmsi_zone_len;
      qw_equ(cur_bs_ptr->csp.esp.tmsi_zone,temp_msg_ptr->tmsi_zone);
      cur_bs_ptr->csp.esp.imsi_t_supported = temp_msg_ptr->imsi_t_supported;
      cur_bs_ptr->csp.esp.max_num_alt_so = temp_msg_ptr->max_num_alt_so;
      cur_bs_ptr->csp.esp.auto_msg_supported = temp_msg_ptr->auto_msg_supported;
      cur_bs_ptr->csp.esp.auto_msg_interval = temp_msg_ptr->auto_msg_interval ;
      cur_bs_ptr->csp.esp.cs_supported = temp_msg_ptr->cs_supported ;
      cur_bs_ptr->csp.esp.ms_init_pos_loc_sup_ind = temp_msg_ptr->ms_init_pos_loc_sup_ind;

      #ifdef FEATURE_IS2000_REL_C
      cur_bs_ptr->csp.esp.msg_integrity_sup = temp_msg_ptr->msg_integrity_sup;
      cur_bs_ptr->csp.esp.sig_integrity_sup_incl = temp_msg_ptr->sig_integrity_sup_incl;
      cur_bs_ptr->csp.esp.sig_integrity_sup = temp_msg_ptr->sig_integrity_sup;
      #endif /* FEATURE_IS2000_REL_C */

      #ifdef FEATURE_IS2000_REL_D
#error code not present
      #endif /* FEATURE_IS2000_REL_D */

      cur_bs_ptr->a41spm_config_msg_seq = msg_ptr->a41spm.config_msg_seq;
      cur_bs_ptr->rx.config_msg_seq = msg_ptr->a41spm.config_msg_seq;
      cur_bs_ptr->rx.a41spm_rx = TRUE;

      break;
    }

    case CAI_EN_AC_PARM_MSG:
    {
      cur_bs_ptr->eapm = * (caii_en_ac_parm_type *) msg_ptr;
      cur_bs_ptr->rx.acc_msg_seq = msg_ptr->enacpm.acc_msg_seq ;
      cur_bs_ptr->rx.eapm_rx = TRUE;
      break;
    }

    case CAI_UNIV_NL_MSG:
    {
      cur_bs_ptr->unlm_config_msg_seq = msg_ptr->unlm.config_msg_seq;
      cur_bs_ptr->rx.config_msg_seq = msg_ptr->unlm.config_msg_seq;
      cur_bs_ptr->rx.unlm_rx = TRUE;
      break;
    }

    #ifdef FEATURE_AUTH
    case CAI_ANSI41_RAND_MSG:
    {
      cur_bs_ptr->rx.a41randm_rx = TRUE;

      #ifdef FEATURE_IS2000_REL_D
#error code not present
      #endif /* FEATURE_IS2000_REL_D */

      break;
    }
    #endif /* FEATURE_AUTH */

    #endif /* FEATURE_IS2000_REL_A_CC */

    default:
      ERR_FATAL( "mc_put_msg bad msg_type %d", msg_type, 0, 0 );  
      break;

  } /* end of switch */
 
} /* mcc_put_msg */

/*===========================================================================

FUNCTION MCC_GET_CUR_MSG_SEQ

DESCRIPTION
  This function gets the current config message sequence number in use
  on this serving base station. This function is called from the context
  of RX task.

DEPENDENCIES
  None.

RETURN VALUE
  Returns the current config_msg_seq.

SIDE EFFECTS
  None.

===========================================================================*/

word mcc_get_cur_msg_seq( void )
{
   return((word)cur_bs_ptr->rx.config_msg_seq);

} /* mcc_get_cur_msg_seq */

/*===========================================================================

FUNCTION  MCC_IS_OVHD_CFG_OKAY

DESCRIPTION
  This function checks if all ovhd messages' config message sequence are
  current.

DEPENDENCIES
  None.

RETURN VALUE
  TRUE if all ovhd msgs' config message sequence are current , FALSE
  otherwise.

SIDE EFFECTS
  None.

===========================================================================*/

boolean mcc_is_ovhd_cfg_okay( void )
{
  boolean ovhd_cfg_okay = FALSE;

  if (OVHD_CHAN_IS_PCH)
  {
    if ((cur_bs_ptr->csp.sp.config_msg_seq  == cur_bs_ptr->rx.config_msg_seq) &&
        (cur_bs_ptr->nl_config_msg_seq  == cur_bs_ptr->rx.config_msg_seq) &&
        (cur_bs_ptr->cl.config_msg_seq  == cur_bs_ptr->rx.config_msg_seq) &&
        (cur_bs_ptr->csp.esp.config_msg_seq == cur_bs_ptr->rx.config_msg_seq) &&
        (cur_bs_ptr->gsd.config_msg_seq == cur_bs_ptr->rx.config_msg_seq) &&
        cur_bs_ptr->rx.sp_rx && cur_bs_ptr->rx.nl_rx  &&
        cur_bs_ptr->rx.cl_rx && cur_bs_ptr->rx.esp_rx  &&
        cur_bs_ptr->rx.gsd_rx
        && (cur_bs_ptr->gnl_config_msg_seq  == cur_bs_ptr->rx.config_msg_seq)
        && cur_bs_ptr->rx.gnl_rx
        && (cur_bs_ptr->ecl.config_msg_seq  == cur_bs_ptr->rx.config_msg_seq)
        && (cur_bs_ptr->egsd.config_msg_seq == cur_bs_ptr->rx.config_msg_seq)
        && cur_bs_ptr->rx.ecl_rx && cur_bs_ptr->rx.egsd_rx
       )
    {
      ovhd_cfg_okay = TRUE;
    }
  }

  #ifdef FEATURE_IS2000_REL_A_CC
  else if (OVHD_CHAN_IS_BCCH)
  {
     if ((cur_bs_ptr->a41spm_config_msg_seq == cur_bs_ptr->rx.config_msg_seq) &&
         (cur_bs_ptr->mcrrpm_config_msg_seq == cur_bs_ptr->rx.config_msg_seq) &&
         (cur_bs_ptr->unlm_config_msg_seq == cur_bs_ptr->rx.config_msg_seq) &&
         (cur_bs_ptr->ecl.config_msg_seq == cur_bs_ptr->rx.config_msg_seq) &&
         (cur_bs_ptr->egsd.config_msg_seq == cur_bs_ptr->rx.config_msg_seq) &&
         cur_bs_ptr->rx.a41spm_rx && cur_bs_ptr->rx.mcrrpm_rx &&
         cur_bs_ptr->rx.unlm_rx && cur_bs_ptr->rx.ecl_rx &&
         cur_bs_ptr->rx.egsd_rx)
     {
       ovhd_cfg_okay = TRUE;
     }
  }
  #endif /* FEATURE_IS2000_REL_A_CC */

  else
  {
    /* Should never be here */
    ERR("Invalid overhead channel %d", cur_bs_ptr->ovhd_chan_info.ovhd_chan, 0, 0);
  }

  return (ovhd_cfg_okay);

} /* mcc_is_ovhd_cfg_okay */

/*===========================================================================

FUNCTION  MCC_IS_OVHD_MSG_OKAY

DESCRIPTION
  This function checks if all ovhd messages are current

DEPENDENCIES
  None.

RETURN VALUE
  TRUE if all ovhd msgs are current , FALSE otherwise.

SIDE EFFECTS
  None.

===========================================================================*/

boolean mcc_is_ovhd_msg_okay( void )
{
  boolean ovhd_okay = FALSE;

  if (OVHD_CHAN_IS_PCH)
  {
    if ((cur_bs_ptr->csp.sp.config_msg_seq  == cur_bs_ptr->rx.config_msg_seq) &&
        (cur_bs_ptr->nl_config_msg_seq  == cur_bs_ptr->rx.config_msg_seq) &&
        (cur_bs_ptr->cl.config_msg_seq  == cur_bs_ptr->rx.config_msg_seq) &&
        (cur_bs_ptr->csp.esp.config_msg_seq == cur_bs_ptr->rx.config_msg_seq) &&
        (cur_bs_ptr->gsd.config_msg_seq == cur_bs_ptr->rx.config_msg_seq) &&
        (cur_bs_ptr->ap.acc_msg_seq == cur_bs_ptr->rx.acc_msg_seq) &&
        cur_bs_ptr->rx.sp_rx && cur_bs_ptr->rx.nl_rx  &&
        cur_bs_ptr->rx.cl_rx && cur_bs_ptr->rx.esp_rx  &&
        cur_bs_ptr->rx.gsd_rx && cur_bs_ptr->rx.ap_rx
        && (cur_bs_ptr->gnl_config_msg_seq  == cur_bs_ptr->rx.config_msg_seq)
        && cur_bs_ptr->rx.gnl_rx
        && (cur_bs_ptr->ecl.config_msg_seq  == cur_bs_ptr->rx.config_msg_seq)
        && (cur_bs_ptr->egsd.config_msg_seq == cur_bs_ptr->rx.config_msg_seq)
        && cur_bs_ptr->rx.ecl_rx && cur_bs_ptr->rx.egsd_rx
       )
    {
      ovhd_okay = TRUE;
    }
  }

  #ifdef FEATURE_IS2000_REL_A_CC
  else if (OVHD_CHAN_IS_BCCH)
  {
     if ((cur_bs_ptr->a41spm_config_msg_seq == cur_bs_ptr->rx.config_msg_seq) &&
         (cur_bs_ptr->mcrrpm_config_msg_seq == cur_bs_ptr->rx.config_msg_seq) &&
         (cur_bs_ptr->unlm_config_msg_seq == cur_bs_ptr->rx.config_msg_seq) &&
         (cur_bs_ptr->ecl.config_msg_seq == cur_bs_ptr->rx.config_msg_seq) &&
         (cur_bs_ptr->egsd.config_msg_seq == cur_bs_ptr->rx.config_msg_seq) &&
         (cur_bs_ptr->eapm.acc_msg_seq == cur_bs_ptr->rx.acc_msg_seq) &&
         cur_bs_ptr->rx.a41spm_rx && cur_bs_ptr->rx.mcrrpm_rx &&
         cur_bs_ptr->rx.unlm_rx && cur_bs_ptr->rx.ecl_rx &&
         cur_bs_ptr->rx.egsd_rx && cur_bs_ptr->rx.eapm_rx)
     {
       ovhd_okay = TRUE;
     }
  }
  #endif /* FEATURE_IS2000_REL_A_CC */

  else
  {
    /* Should never be here */
    ERR("Invalid overhead channel %d", cur_bs_ptr->ovhd_chan_info.ovhd_chan, 0, 0);
  }

  return (ovhd_okay);

} /* mcc_is_ovhd_msg_okay */

#ifdef FEATURE_IS2000_REL_A_CC
/*===========================================================================

FUNCTION  MCC_IS_UPD_OVHD_MSG_OKAY

DESCRIPTION
  This function checks if all ovhd messages necessary for Update Overhead
  info substate are current

DEPENDENCIES
  None.

RETURN VALUE
  TRUE if all ovhd msgs are current , FALSE otherwise.

SIDE EFFECTS
  None.

===========================================================================*/

boolean mcc_is_upd_ovhd_msg_okay( void )
{
  boolean ovhd_okay = FALSE;

  if (OVHD_CHAN_IS_BCCH)
  {
     if ((cur_bs_ptr->a41spm_config_msg_seq == cur_bs_ptr->rx.config_msg_seq) &&
         (cur_bs_ptr->mcrrpm_config_msg_seq == cur_bs_ptr->rx.config_msg_seq) &&
         (cur_bs_ptr->unlm_config_msg_seq == cur_bs_ptr->rx.config_msg_seq) &&
         (cur_bs_ptr->ecl.config_msg_seq == cur_bs_ptr->rx.config_msg_seq) &&
         (cur_bs_ptr->egsd.config_msg_seq == cur_bs_ptr->rx.config_msg_seq) &&
         (cur_bs_ptr->eapm.acc_msg_seq != INVALID_SEQ) &&
         (cur_bs_ptr->rx.acc_msg_seq == INVALID_SEQ) &&
         cur_bs_ptr->rx.a41spm_rx && cur_bs_ptr->rx.mcrrpm_rx &&
         cur_bs_ptr->rx.unlm_rx && cur_bs_ptr->rx.ecl_rx &&
         cur_bs_ptr->rx.egsd_rx && cur_bs_ptr->rx.eapm_rx)
     {
       ovhd_okay = TRUE;
     }
  }
  else if (!OVHD_CHAN_IS_PCH)
  {
    /* Should never be here */
    ERR("Invalid overhead channel %d", cur_bs_ptr->ovhd_chan_info.ovhd_chan, 0, 0);
  }

  return (ovhd_okay);

} /* mcc_is_upd_ovhd_msg_okay */
#endif /* FEATURE_IS2000_REL_A_CC */

/*===========================================================================

FUNCTION  MCC_IS_AHO_OVHD_MSG_OKAY

DESCRIPTION
  This function checks if ovhd messages necessary for Access HO are current.

DEPENDENCIES
  None.

RETURN VALUE
  TRUE if all ovhd msgs are current , FALSE otherwise.

SIDE EFFECTS
  None.

===========================================================================*/

boolean mcc_is_aho_ovhd_msg_okay( void )
{
  boolean ovhd_okay = FALSE;

  if (OVHD_CHAN_IS_PCH)
  {
    if ((cur_bs_ptr->csp.sp.config_msg_seq == cur_bs_ptr->rx.config_msg_seq) &&
        (cur_bs_ptr->nl_config_msg_seq == cur_bs_ptr->rx.config_msg_seq) &&
        (cur_bs_ptr->gnl_config_msg_seq == cur_bs_ptr->rx.config_msg_seq) &&
        (cur_bs_ptr->csp.esp.config_msg_seq == cur_bs_ptr->rx.config_msg_seq) &&
        (cur_bs_ptr->ap.acc_msg_seq == cur_bs_ptr->rx.acc_msg_seq) &&
        cur_bs_ptr->rx.sp_rx && cur_bs_ptr->rx.nl_rx  &&
        cur_bs_ptr->rx.gnl_rx && cur_bs_ptr->rx.esp_rx && cur_bs_ptr->rx.ap_rx)
    {
      ovhd_okay = TRUE;
    }
  }

  #ifdef FEATURE_IS2000_REL_A_CC
  else if (OVHD_CHAN_IS_BCCH)
  {
    if ((cur_bs_ptr->a41spm_config_msg_seq == cur_bs_ptr->rx.config_msg_seq) &&
        (cur_bs_ptr->mcrrpm_config_msg_seq == cur_bs_ptr->rx.config_msg_seq) &&
        (cur_bs_ptr->unlm_config_msg_seq == cur_bs_ptr->rx.config_msg_seq) &&
        (cur_bs_ptr->eapm.acc_msg_seq == cur_bs_ptr->rx.acc_msg_seq) &&
        cur_bs_ptr->rx.a41spm_rx && cur_bs_ptr->rx.mcrrpm_rx &&
        cur_bs_ptr->rx.unlm_rx && cur_bs_ptr->rx.eapm_rx)
    {
      ovhd_okay = TRUE;
    }
  }
  #endif /* FEATURE_IS2000_REL_A_CC */

  else
  {
    /* Should never be here */
    ERR("Invalid overhead channel %d", cur_bs_ptr->ovhd_chan_info.ovhd_chan, 0, 0);
  }

  return (ovhd_okay);

} /* mcc_is_aho_ovhd_msg_okay */

/*===========================================================================

FUNCTION  MCC_IS_AEHO_OVHD_MSG_OKAY

DESCRIPTION
  This function checks if ovhd messages necessary for Access Entry HO are
  current.

DEPENDENCIES
  None.

RETURN VALUE
  TRUE if all ovhd msgs are current , FALSE otherwise.

SIDE EFFECTS
  None.

===========================================================================*/

boolean mcc_is_aeho_ovhd_msg_okay( void )
{
  boolean ovhd_okay = FALSE;

  if (OVHD_CHAN_IS_PCH)
  {
    if ((cur_bs_ptr->csp.sp.config_msg_seq == cur_bs_ptr->rx.config_msg_seq) &&
        (cur_bs_ptr->nl_config_msg_seq == cur_bs_ptr->rx.config_msg_seq) &&
        (cur_bs_ptr->cl.config_msg_seq == cur_bs_ptr->rx.config_msg_seq) &&
        (cur_bs_ptr->csp.esp.config_msg_seq == cur_bs_ptr->rx.config_msg_seq) &&
        cur_bs_ptr->rx.sp_rx  && cur_bs_ptr->rx.nl_rx  &&
        cur_bs_ptr->rx.cl_rx && cur_bs_ptr->rx.esp_rx
        && cur_bs_ptr->rx.gnl_rx
        && (cur_bs_ptr->gnl_config_msg_seq  == cur_bs_ptr->rx.config_msg_seq)
        && cur_bs_ptr->rx.ecl_rx
        && (cur_bs_ptr->ecl.config_msg_seq == cur_bs_ptr->rx.config_msg_seq)
       )
     {
       ovhd_okay = TRUE;
     }
  }

  #ifdef FEATURE_IS2000_REL_A_CC
  else if (OVHD_CHAN_IS_BCCH)
  {
    if ((cur_bs_ptr->a41spm_config_msg_seq == cur_bs_ptr->rx.config_msg_seq) &&
        (cur_bs_ptr->mcrrpm_config_msg_seq == cur_bs_ptr->rx.config_msg_seq) &&
        (cur_bs_ptr->unlm_config_msg_seq == cur_bs_ptr->rx.config_msg_seq) &&
        (cur_bs_ptr->ecl.config_msg_seq == cur_bs_ptr->rx.config_msg_seq) &&
        cur_bs_ptr->rx.a41spm_rx && cur_bs_ptr->rx.mcrrpm_rx &&
        cur_bs_ptr->rx.unlm_rx && cur_bs_ptr->rx.ecl_rx)
    {
      ovhd_okay = TRUE;
    }
  }
  #endif /* FEATURE_IS2000_REL_A_CC */

  else
  {
    /* Should never be here */
    ERR("Invalid overhead channel %d", cur_bs_ptr->ovhd_chan_info.ovhd_chan, 0, 0);
  }

  return (ovhd_okay);

} /* mcc_is_aeho_ovhd_msg_okay */

#ifdef FEATURE_AUTH
/*===========================================================================

FUNCTION  MCC_ARE_RAND_PARMS_OKAY

DESCRIPTION
  This function checks if rand parameters are current.

DEPENDENCIES
  None.

RETURN VALUE
  TRUE if we are on the Paging Channel, since Rand Parameters always come
  in the Access Parameters Message on PCH.
  FALSE if the base station is sending ANSI-41 RAND Message and
  it is not current; TRUE otherwise.

SIDE EFFECTS
  None.

===========================================================================*/

boolean mcc_are_rand_parms_okay(void)
{
  boolean rand_okay = TRUE;

  #ifdef FEATURE_IS2000_REL_A_CC
  if (OVHD_CHAN_IS_BCCH)
  {
     if (cur_bs_ptr->csp.sending_a41_rand && !cur_bs_ptr->rx.a41randm_rx)
     {
       rand_okay = FALSE;
     }
  }
  #endif /* FEATURE_IS2000_REL_A_CC */

  return (rand_okay);

} /* mcc_are_rand_parms_okay */
#endif /* FEATURE_AUTH */

/*===========================================================================

FUNCTION  MCC_CLEAR_OVHD_MSGS

DESCRIPTION
  This function clears the "rx" flag of all messages.

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/

void mcc_clear_ovhd_msgs( void )
{
  cur_bs_ptr->rx.config_msg_seq = INVALID_SEQ;
  cur_bs_ptr->csp.sp.config_msg_seq = INVALID_SEQ;
  cur_bs_ptr->nl_config_msg_seq = INVALID_SEQ;
  cur_bs_ptr->cl.config_msg_seq = INVALID_SEQ;
  cur_bs_ptr->ap.acc_msg_seq = INVALID_SEQ;
  cur_bs_ptr->csp.esp.config_msg_seq = INVALID_SEQ;
  cur_bs_ptr->gsd.config_msg_seq = INVALID_SEQ;
  cur_bs_ptr->rx.sp_rx  = FALSE;
  cur_bs_ptr->rx.nl_rx  = FALSE;
  cur_bs_ptr->rx.cl_rx  = FALSE;
  cur_bs_ptr->rx.ap_rx  = FALSE;
  cur_bs_ptr->rx.esp_rx = FALSE;
  cur_bs_ptr->rx.gsd_rx = FALSE;
  cur_bs_ptr->gnl_config_msg_seq = INVALID_SEQ;
  cur_bs_ptr->rx.gnl_rx = FALSE;
  cur_bs_ptr->egsd.config_msg_seq = INVALID_SEQ;
  cur_bs_ptr->ecl.config_msg_seq = INVALID_SEQ;
  cur_bs_ptr->rx.egsd_rx = FALSE;
  cur_bs_ptr->rx.ecl_rx = FALSE;

  #ifdef FEATURE_IS2000_REL_A_CC
  cur_bs_ptr->a41spm_config_msg_seq = INVALID_SEQ;
  cur_bs_ptr->mcrrpm_config_msg_seq = INVALID_SEQ;
  cur_bs_ptr->unlm_config_msg_seq = INVALID_SEQ;
  cur_bs_ptr->eapm.acc_msg_seq = INVALID_SEQ;
  cur_bs_ptr->rx.a41spm_rx = FALSE;
  cur_bs_ptr->rx.mcrrpm_rx = FALSE;
  cur_bs_ptr->rx.unlm_rx = FALSE;
  cur_bs_ptr->rx.eapm_rx = FALSE;

  #ifdef FEATURE_AUTH
  cur_bs_ptr->rx.a41randm_rx = FALSE;
  #endif /* FEATURE_AUTH */

  #endif /* FEATURE_IS2000_REL_A_CC */

} /* mcc_clear_ovhd_msgs */

/*===========================================================================

FUNCTION  MCC_CLEAR_OVHD_MSGS_PREV_MISMATCH

DESCRIPTION
  This function marks all messages that depends on p_rev for processing.

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/

void mcc_clear_ovhd_msgs_prev_mismatch( void )
{
  if (OVHD_CHAN_IS_PCH)
  {
    /* We don't need to clear esp_rx flag because this function is called
       after Extended System Parameters Message is received */
    cur_bs_ptr->csp.sp.config_msg_seq = INVALID_SEQ;
    cur_bs_ptr->nl_config_msg_seq = INVALID_SEQ;
    cur_bs_ptr->ap.acc_msg_seq = INVALID_SEQ;
    cur_bs_ptr->gsd.config_msg_seq = INVALID_SEQ;
    cur_bs_ptr->cl.config_msg_seq = INVALID_SEQ;
    cur_bs_ptr->rx.sp_rx  = FALSE;
    cur_bs_ptr->rx.nl_rx  = FALSE;
    cur_bs_ptr->rx.ap_rx  = FALSE;
    cur_bs_ptr->rx.gsd_rx = FALSE;
    cur_bs_ptr->rx.cl_rx = FALSE;

    cur_bs_ptr->gnl_config_msg_seq = INVALID_SEQ;
    cur_bs_ptr->rx.gnl_rx = FALSE;

    cur_bs_ptr->egsd.config_msg_seq = INVALID_SEQ;
    cur_bs_ptr->ecl.config_msg_seq = INVALID_SEQ;
    cur_bs_ptr->rx.egsd_rx = FALSE;
    cur_bs_ptr->rx.ecl_rx = FALSE;
  }

  #ifdef FEATURE_IS2000_REL_A_CC
  else if (OVHD_CHAN_IS_BCCH)
  {
    /* We don't need to clear mcrrpm_rx flag because this function is called
       after MC-RR Parameters Message is received */
    cur_bs_ptr->a41spm_config_msg_seq = INVALID_SEQ;
    cur_bs_ptr->unlm_config_msg_seq = INVALID_SEQ;
    cur_bs_ptr->eapm.acc_msg_seq = INVALID_SEQ;
    cur_bs_ptr->egsd.config_msg_seq = INVALID_SEQ;
    cur_bs_ptr->ecl.config_msg_seq = INVALID_SEQ;
    cur_bs_ptr->rx.a41spm_rx = FALSE;
    cur_bs_ptr->rx.unlm_rx = FALSE;
    cur_bs_ptr->rx.eapm_rx = FALSE;
    cur_bs_ptr->rx.egsd_rx = FALSE;
    cur_bs_ptr->rx.ecl_rx = FALSE;

    #ifdef FEATURE_AUTH
    cur_bs_ptr->rx.a41randm_rx = FALSE;
    #endif /* FEATURE_AUTH */

  }
  #endif /* FEATURE_IS2000_REL_A_CC */

  else
  {
    /* Should never be here */
    ERR("Invalid overhead channel %d", cur_bs_ptr->ovhd_chan_info.ovhd_chan, 0, 0);
  }

} /* mcc_clear_ovhd_msgs_prev_mismatch */

/*===========================================================================

FUNCTION  MCC_UPDATE_ACC_MSG_SEQ

DESCRIPTION
  This function updates the stored access message sequence.

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/

void mcc_update_acc_msg_seq
(
  byte acc_msg_seq /* Access message sequence */
)
{
  if (OVHD_CHAN_IS_PCH)
  {
    if (cur_bs_ptr->rx.ap_rx &&
        (acc_msg_seq != cur_bs_ptr->ap.acc_msg_seq))
    {
      /* Stored APM out-dated, clear it to ensure MS process the latest APM */
      cur_bs_ptr->ap.acc_msg_seq = INVALID_SEQ;
      cur_bs_ptr->rx.ap_rx  = FALSE;
    }
    else
    {
      mcc_put_seq(acc_msg_seq, TRUE);
    }
  }

  #ifdef FEATURE_IS2000_REL_A_CC
  else if (OVHD_CHAN_IS_BCCH)
  {
    if (cur_bs_ptr->rx.eapm_rx &&
        (acc_msg_seq != cur_bs_ptr->eapm.acc_msg_seq))
    {
      /* Stored EAPM uot-dated , clear it to ensure MS process the latest EAPM */
      cur_bs_ptr->eapm.acc_msg_seq = INVALID_SEQ;
      cur_bs_ptr->rx.eapm_rx = FALSE;
    }
    else
    {
      mcc_put_seq(acc_msg_seq, TRUE);
    }
  }
  #endif /* FEATURE_IS2000_REL_A_CC */

  else
  {
    /* Should never be here */
    ERR("Invalid overhead channel %d", cur_bs_ptr->ovhd_chan_info.ovhd_chan, 0, 0);
  }

} /* mcc_update_acc_msg_seq */

/*===========================================================================

FUNCTION PROCESS_INTERCEPT

DESCRIPTION
  This function processes an intercept order.

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/

void process_intercept( word curr_state )
{
  cm_mc_rpt_type *cmd_ptr;    /* Pointer to handset command */

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  /* Start intercept tone */
  if ((cmd_ptr = cm_mc_rpt_get_buf_else_err_fatal()) != NULL)
  {
    cmd_ptr->inter.hdr.cmd = CM_INTERCEPT_F;

    /* This function should also be called from CDMA_ORIG. But for
    ** historical reason, we do a state checking here to preserve the
    ** old behavior.
    */

    if (curr_state == CDMA_ORIG) /*lint !e641 */
    {
      cmd_ptr->inter.call_id = cdma.call_id;
    }
    else
    {
      cmd_ptr->inter.call_id = CM_CALL_ID_UNASSIGNED;
    }

    mcc_cm_mc_rpt( cmd_ptr );
  }
  else
  {
    MSG_HIGH( "No free buffers on cm_mc_rpt_free_q", 0, 0, 0 );
  }

} /* process_intercept */

/*===========================================================================

FUNCTION MCC_ENCODE_DTMF

DESCRIPTION
  This function converts a called-address digit received from UI
  (in ASCII format) to BCD format.

DEPENDENCIES
  None

RETURN VALUE
  Translated character.

SIDE EFFECTS
  None

===========================================================================*/

byte mcc_encode_dtmf
(
  byte ui_char
    /* ASCII digit received from the user interface */
)
{
  byte dtmf;
    /* DTMF digit to be sent in message */

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  /* Initialize dtmf to avoid compiler warning */
  dtmf = MCC_DIAL_0;

  switch (ui_char)
  {
    case '1':
    case '2':
    case '3':
    case '4':
    case '5':
    case '6':
    case '7':
    case '8':
    case '9':
      dtmf = ui_char - '0';
      break;

    case '0':
      dtmf = MCC_DIAL_0;
      break;

    case '#':
      dtmf = MCC_DIAL_PND;
      break;

    case '*':
      dtmf = MCC_DIAL_STAR;
      break;

    default:
      ERR_FATAL( "Illegal digit code %d", ui_char, 0, 0 );  
  } /* end switch */

  return (dtmf);

} /* end mcc_encode_dtmf() */

/*===========================================================================

FUNCTION MCC_DECODE_DTMF

DESCRIPTION
  This function converts a BCD encoded called-address digit
  to ASCII format.

DEPENDENCIES
  None

RETURN VALUE
  Translated character.

SIDE EFFECTS
  None

===========================================================================*/

byte mcc_decode_dtmf
(
  byte bcd_digit
    /* BCD digit to be converted to ASCII equivalent */
)
{
  byte ascii_digit;
    /* ASCII equivalent digit to be returned from function */

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  /* Initialize ascii_digit to avoid compiler warning */
  ascii_digit = '0';

  switch (bcd_digit)
  {
    case 1:
    case 2:
    case 3:
    case 4:
    case 5:
    case 6:
    case 7:
    case 8:
    case 9:
      ascii_digit = bcd_digit + '0';
      break;

    case 10:
      ascii_digit = '0';
      break;

    case 11:
      ascii_digit = '*';
      break;

    case 12:
      ascii_digit = '#';
      break;

    default:
      ERR_FATAL( "Illegal digit code %d", bcd_digit, 0, 0 );  
  } /* end switch (bcd_digit) */

  return (ascii_digit);

} /* end mcc_decode_dtmf() */

/*===========================================================================

FUNCTION MCC_DETECT_DUPLICATE_ECAM

DESCRIPTION
  This function detects whether a duplicate ECAM message has been received
  on the Paging Channel. This function does not record the time, because the
  main mcc_detect_duplicate function takes care of that. This function is 
  called only for detecting duplicate ECAMs so that we dont consider it as
  an ack for the previous transmission.

DEPENDENCIES
  None

RETURN VALUE
  TRUE  - A duplicate ECAM was received on the Paging Channel.
  FALSE - The received ECAM was NOT a duplicate.

SIDE EFFECTS
  None

===========================================================================*/

boolean mcc_detect_duplicate_ecam 
(
  /* Header of ECAM to process layer 2 fields of */
  caii_pc_hdr_type *hdr
)
{
  qword current_time;
    /* Holder for current time stamp */
  qword delta_time;
    /* Elapsed time between PC msgs rxed with same layer 2 fields */
  boolean duplicate_ecam;
    /* Returned status */
  
  /* Check to make sure header fields are in range */
  if ((hdr->ack_req >= MCC_MAX_ACK_TYPES)
      || (hdr->addr_type >= MCC_MAX_ADDR_TYPE)
      || (hdr->msg_seq >= MCC_MAX_MSG_SEQ))
  {
     MSG_HIGH( "Invalid ACK_REQ, ADDR_TYPE, or MSG_SEQ field rxed", 0, 0, 0 );
     return FALSE;
  }

  /* Get current system time in units of 20 msecs */
  ts_get_20ms_frame_time( current_time );

  /* Convert current time to 1 msec units */
  qw_mul( current_time, current_time, 20UL );

  /* --------------------------------------------------------------
  ** Check how much time has elapsed since a ECAM was received with the 
  ** same layer 2 field values
  ** -------------------------------------------------------------- */
  qw_sub(delta_time, current_time,
         mcc_idle_frames[hdr->ack_req][hdr->msg_seq][hdr->addr_type]);


  if ((qw_hi( delta_time ) == 0) && (qw_lo( delta_time ) <= cai_tmo.t4m))
  {
    MSG_HIGH( "Duplicate ECAM rxed", 0, 0, 0 );
    duplicate_ecam = TRUE;
  }
  else
  {
    duplicate_ecam = FALSE;
  }

  return (duplicate_ecam);
 
} /* mcc_detect_duplicate_ecam */

/*===========================================================================

FUNCTION MCC_DETECT_DUPLICATE

DESCRIPTION
  This function detects whether a duplicate message has been received
  on the Paging Channel.  If a duplicate message was NOT received this
  function records the time at which the message was received.

DEPENDENCIES
  None

RETURN VALUE
  TRUE  - A duplicate message was received on the Paging Channel.
  FALSE - The received Paging Channel message was NOT a duplicate.

SIDE EFFECTS
  None

===========================================================================*/

boolean mcc_detect_duplicate
(
  boolean page,
    /* Identify if page or non-page message */
  caii_pc_hdr_type *hdr
    /* Header of paging channel message to process layer 2 fields of */
)
{
  qword current_time;
    /* Holder for current time stamp */
  qword delta_time;
    /* Elapsed time between PC msgs rxed with same layer 2 fields */
  boolean duplicate_msg;
    /* Returned status */

  /* -----------------------------------------------------------------------
  ** If the phone is not an IS-95B capable phone, then this index is always
  ** set to 0, i.e. 2D array only using msg_seq and ack_req fields for
  ** detecting duplicate pages.
  ** ----------------------------------------------------------------------*/
  byte addr_type = 0;

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  /* -----------------------------------------------------------------------
  ** If the phone is an IS-95B capable phone, the addr_type field is also
  ** used for duplicate page detection.  This field is not used in case of
  ** of General Page message, where 'page' is set to TRUE.
  ** ----------------------------------------------------------------------*/
  if (!page)
  {
    addr_type = hdr->addr_type;
  }

  /* Check to make sure header fields are in range */
  if ((hdr->ack_req >= MCC_MAX_ACK_TYPES)
      || (addr_type >= MCC_MAX_ADDR_TYPE)
      || (hdr->msg_seq >= MCC_MAX_MSG_SEQ))
  {
     MSG_HIGH( "Invalid ACK_REQ, ADDR_TYPE, or MSG_SEQ field rxed", 0, 0, 0 );
     return FALSE;
  }

  /* Get current system time in units of 20 msecs */
  ts_get_20ms_frame_time( current_time );

  /* Convert current time to 1 msec units */
  qw_mul( current_time, current_time, 20UL );

  if (page)
  {
    /* --------------------------------------------------------------
    ** Check how much time has elapsed since a Paging Channel message
    ** was received with the same layer 2 field values for a page record
    ** -------------------------------------------------------------- */
    qw_sub(delta_time, current_time,
           mcc_idle_pg_frames[hdr->msg_seq]);
  }
  /* Received a non-page message */
  else
  {
    /* --------------------------------------------------------------
    ** Check how much time has elapsed since a Paging Channel message
    ** was received with the same layer 2 field values
    ** -------------------------------------------------------------- */
    qw_sub(delta_time, current_time,
           mcc_idle_frames[hdr->ack_req][hdr->msg_seq][addr_type]);
  }

  #ifdef FEATURE_BROADCAST_SMS
  if (hdr->addr_type == CAI_BCAST_ADDR_TYPE)
  {
    /* for BCSMS the layer 2 duplicate detection is disabled because BCSMS has
     * its own duplicate detection. the current layer 2 dectection will limit
     * the number of BCSMSs to 8 within T4m. */
    MSG_LOW("Skipping Layer2 dup. detection for BCSMS",0,0,0);
    duplicate_msg = FALSE;
  }
  else
  #endif /* FEATURE_BROADCAST_SMS */

  if ((qw_hi( delta_time ) == 0) && (qw_lo( delta_time ) <= cai_tmo.t4m))
  {
    MSG_HIGH( "Duplicate Paging Channel msg rxed", 0, 0, 0 );
    duplicate_msg = TRUE;
  }
  else
  {
    duplicate_msg = FALSE;

    /* ------------------------------------------------------------
    ** Record the time that the Paging Channel message was received
    ** ------------------------------------------------------------ */
    /* Update page message timestamps */
    if (page)
    {
      qw_set(mcc_idle_pg_frames[hdr->msg_seq],
             qw_hi(current_time), qw_lo(current_time));
    }
    /* Update non-page message timestamps */
    else
    {
      qw_set(mcc_idle_frames[hdr->ack_req][hdr->msg_seq][addr_type],
             qw_hi(current_time), qw_lo(current_time));
    }
  }

  return (duplicate_msg);
 
} /* end mcc_detect_duplicate */

/*===========================================================================

FUNCTION XLATE_DIALED_NUM

DESCRIPTION
  This function fills a cdma_dial_type buffer with the phone number
  dialed by the user translated into the digit code required by the
  base station. It will also fill in the number of digits dialed into
  the cdma_dial_type buffer.

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.


===========================================================================*/

void xlate_dialed_num
(
 mc_origination_type * orig_parms,
    /* pointer to recieved message */
  cdma_dial_type *buf_ptr
    /* pointer to buffer to place translated phone number into */
)
{
  word i;
    /* index through dialed digits */

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  boolean cm_digit_mode = orig_parms->digit_mode;

  if (orig_parms->cnt > 0)
  {
    /* To support "+" code dialing, we need to drop the leading +.
       In addition we should convert the digit mode to ASCII if 
       this is not REL C, If it is REL C, OTA std supports sending
       number type and number plan over the air even for DTMF digit mode and 
       that saves bits over the air */
    if ((orig_parms->called_address[0] == '+') 
        &&
        (CAI_IS_SO_VOICE(orig_parms->service_option)
         || (orig_parms->service_option== CAI_SO_VOICE_WILDCARD)
        )
       )
    {
      #ifdef FEATURE_IS2000_REL_C
      if (P_REV_IS_9_OR_GREATER)
      {
        /* p rev in use is >=9, so digit mode 0 may be used
           if all digits are in DTMF range */
        /* Note : We are not explicitly checking for all digits here; That
           is done in the for loop below */
        orig_parms->digit_mode = 0;
        orig_parms->number_type = CAI_INT_NUM;
      }
      else
      #endif /* FEATURE_IS2000_REL_C */
      {
        /* P_REV in use is less than 9, make sure digit_mode
           is ASCII to convey number type and plan over the air */
        orig_parms->digit_mode = 1;
        orig_parms->number_type = CAI_INT_NUM;
      }

      /* Drop the '+" */
      MSG_HIGH(" + is being stripped",0,0,0);

      for (i=0; i <orig_parms->cnt - 1; i++)
      {
        orig_parms->called_address[i] =
           orig_parms->called_address[i+1];
          if ((orig_parms->called_address[i] >= '0'
              && orig_parms->called_address[i] <= '9')
              || orig_parms->called_address[i] == '#'
              || orig_parms->called_address[i] == '*'
             )
          {
            /* These chars are in the range of digit mode 0 */
          }
          else
          {
            /* Beter change digit mode to ASCII */
            orig_parms->digit_mode = 1;
          }
      } /* for loop for dropping + */
      orig_parms->cnt--; 
    } /* Check to see to if this a + code voice call */
    else
    {
      /* Not a plus code call ; do sanity checking on digit mode, if it is 0 */
      if (orig_parms->digit_mode == 0)
      {
        for (i=0; i <orig_parms->cnt; i++)
        {
          if ((orig_parms->called_address[i] >= '0'
              && orig_parms->called_address[i] <= '9')
              || orig_parms->called_address[i] == '#'
              || orig_parms->called_address[i] == '*'
             )
          {
            /* Can Leave digit mode as 0 */
          }
          else
          {
            /* Better change digit mode to  ASCII (i.e 1) */
            orig_parms->digit_mode = 1;
            /* Break from the for loop now, no point checking
             * remaining digits */
            break;
          }
        } /* for loop */
      } /* if digit_mode is 0 */
    } /* else block for not plus code */
  } /* If origination.cnt > 0 */


  if (cm_digit_mode != orig_parms->digit_mode)
  {
    MSG_HIGH("Digit Mode was changed by MC to %d",
              orig_parms->digit_mode,0,0);
  }

  /* Check all digits sent in command are valid and store in buffer */
  for (i=0; i < orig_parms->cnt; i++)
  {
     /* No Need to encode the digits for ASCII digit mode */
    if( orig_parms->digit_mode )
      buf_ptr->digits[i] = orig_parms->called_address[i];
    else
      buf_ptr->digits[i] =
        mcc_encode_dtmf( orig_parms->called_address[i] );
  }

  buf_ptr->num_digits = orig_parms->cnt;

  MSG_LOW( "Digit cnt %d", buf_ptr->num_digits, 0, 0 );
  MSG_LOW( "Dialed num: %d %d %d", buf_ptr->digits[0], buf_ptr->digits[1],
                                   buf_ptr->digits[2] );
  MSG_LOW( "Dialed num: %d %d %d", buf_ptr->digits[3], buf_ptr->digits[4],
                                   buf_ptr->digits[5] );
  MSG_LOW( "Dialed num: %d %d %d", buf_ptr->digits[6], buf_ptr->digits[7],
                                   buf_ptr->digits[8] );
  MSG_LOW( "Dialed num: %d %d %d", buf_ptr->digits[9], buf_ptr->digits[10],
                                   buf_ptr->digits[11] );
 
} /* end xlate_dialed_num() */

/*===========================================================================

FUNCTION MCC_MOB_QOS_IS_SUPPORTED

DESCRIPTION
  This function returns if the Mob QoS is supported by the current
  Base Station.

DEPENDENCIES
  None.

RETURN VALUE
  TRUE or FALSE.

SIDE EFFECTS
  None.

===========================================================================*/

boolean mcc_mob_qos_is_supported
(
  void
)
{
  #ifdef FEATURE_IS2000_REL_A
  if ( P_REV_IS_7_OR_GREATER )
  {
    if ( MAIN_STATE(cdma.curr_state) == CDMA_TC ) 
    {
      return cdma.mob_qos;
    }
    else if ( ( OVHD_CHAN_IS_PCH && ESP_MSG_IS_RECVD )
              #ifdef FEATURE_IS2000_REL_A_CC
              || ( OVHD_CHAN_IS_BCCH && ANSI41SP_MSG_IS_RECVD )
              #endif /* FEATURE_IS2000_REL_A_CC */ 
            )
    {
      return cur_bs_ptr->csp.esp.mob_qos;
    }
  }
  #endif /* FEATURE_IS2000_REL_A */

  return FALSE;

} /* mcc_mob_qos_is_supported */

/*===========================================================================

FUNCTION MCC_MS_ORIG

DESCRIPTION
  This function processes the Mobile Station Origination operation.
  During this operation the number dialed by the user is stored.

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  If the header within the input mc_msg_type requests that the buffer
  be placed on a queue after processing is completed this procedure will
  do so.

===========================================================================*/

void mcc_ms_orig
(
  mc_origination_type * orig_parms
    /* Pointer to message received from the handset */
)
{

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  /* -----------------------------------------------
  ** First store digits dialed in dial array so that
  ** the System Access substate can access them.
  ** ----------------------------------------------- */
  xlate_dialed_num( orig_parms, &cdma.dial );

  cdma.so_req = orig_parms->service_option;

  cdma.drs = orig_parms->drs;

  #ifdef FEATURE_IS2000_REL_A
  /* In order to use received information from CM,
     we need to store the following values. */

  /* Indicate if this is a silent redial due to layer 2 failure. */
  cdma.orig_reason = orig_parms->orig_reason;
  if (orig_parms->orig_count > CAI_SILENT_REDIAL_MAX )
  {
    cdma.orig_count = CAI_SILENT_REDIAL_MAX;
  }
  else
  {
    cdma.orig_count = orig_parms->orig_count;
  }

  /* Packet data indicators */
  cdma.is_packet_data = orig_parms->is_packet_data ;
  cdma.is_dormant_packet_data = orig_parms->is_dormant_packet_data;

  /* CM will do the digit translation to indicate that this is an emergency call.*/
  cdma.is_emergency = orig_parms->is_emergency;

  /* Process QoS parameters */
  if ((orig_parms->qos_parms_incl) &&
      (orig_parms->is_packet_data))
  {
    /* if the Base Station had not allowed MS to propose QoS, MS
       should not include those parameters in the outgoing Origination
       Message.
       Note that there's no CC instance created to save this info
       at the time of Origination if Data Call is the first call
       Hence cdma variable is used temporarily. */
    if ( mcc_mob_qos_is_supported() &&
         (orig_parms->qos_parms_len < CAI_ORIG_QOS_LEN_MAX ) )
    {
      cdma.qos_parms_incl = orig_parms->qos_parms_incl;
      cdma.qos_parms_len = orig_parms->qos_parms_len;
      (void) memcpy
        ( cdma.qos_parms,
          orig_parms->qos_parms,
          orig_parms->qos_parms_len
        );
    }
    else
    {
      cdma.qos_parms_incl = 0;
      cdma.qos_parms_len = 0;
    }
  }

  cdma.is_sr_id_valid = orig_parms->sr_id_included;
  cdma.sr_id = orig_parms->sr_id;
  #endif /* FEATURE_IS2000_REL_A */

  cdma.call_id = orig_parms->call_id;

  cdma.digit_mode = orig_parms->digit_mode;

  cdma.number_plan = orig_parms->number_plan;
  cdma.number_type = orig_parms->number_type;

  /* Copy the complete address info and waste some memory */
  cdma.calling_party_number = orig_parms->calling_party_number;
  cdma.called_party_subaddr = orig_parms->called_party_subaddr;
  cdma.calling_party_subaddr = orig_parms->calling_party_subaddr;

  cdma.call_orig = TRUE;  /* Call is mobile originated */
 
} /* end mcc_ms_orig */

/*===========================================================================

FUNCTION MCC_MS_SMS

DESCRIPTION
  This function processes the Mobile Station SMS
  Origination operation.  During this operation the SMS
  message created by the user is stored.

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  If the header within the input mc_msg_type requests that the buffer
  be placed on a queue after processing is completed this procedure will
  do so.

===========================================================================*/

void mcc_ms_sms
(
  mc_msg_type *cmd_ptr
    /* Pointer to message received from the handset */
/*lint -esym(715,cmd_ptr) */
)
{

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  /* ------------------------------------------
  ** The layer 2 and 3 are connected.
  ** A MS originated SMS message is not an ack.
  ** ------------------------------------------ */
  mcc_layer2.valid_ack = FALSE;

  /* -----------------------------------------------
  ** Store SMS message in SMS transmit buffer so
  ** the System Access substate can access them.
  ** ----------------------------------------------- */
  /* Need to rework UI interface for data burst to */
  /* use new generic UI interface message.         */

  #if (defined(FEATURE_CDSMS) || defined (FEATURE_DBM))
  /* only one sms data burst is sent each time */
  /* sms msg is already stored in the global 'cdma_uasms_msg'. */
  /* position determination msg is stored in upper layer appl. */
  #else
  MSG_MED( "Mobile-originated SMS capability not supported", 0, 0, 0);
  #endif /* FEATURE_CDSMS || FEATURE_DBM */
 
} /* end mcc_ms_sms */

/*===========================================================================

FUNCTION MCC_COMP_ADDR

DESCRIPTION
  This function takes a pointer to the header of a paging channel message
  and checks to see if it is addressed  to the mobile.
  When IMSI addressing gets added this function will become much larger.
  This function is called from mccidl.c and mccsa.c.

DEPENDENCIES
  None.

RETURN VALUE
  TRUE if the address matches, FALSE otherwise.

SIDE EFFECTS
  None.

===========================================================================*/

boolean mcc_comp_addr
(
  caii_pc_hdr_type *msg_hdr,
    /* Header of paging channel message to match address of */
  qword msg_frame
    /* Msg frame, in 20 ms */
)
{
  boolean addr_found = FALSE;
    /* Flag if address matches */
  caii_imsi_class_0_type *imsi0;
    /* Shorthand to get at the class 0 IMSI */
  caii_imsi_class_1_type *imsi1;
    /* Shorthand to get at the class 1 IMSI */

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  /* In case ESPM or MCCRRM is not received, mcc_entry_init() already
     defaulted cur_bs_ptr->csp.esp.mcc and cur_bs_ptr->csp.esp.imsi_11_12 
     values to MS's MCC and IMSI_11_12. So, safe to reference them for IMSI 
     address match. */

  switch (msg_hdr->addr_type)
  {
    case CAI_MIN_ADDR_TYPE:
      if ((msg_hdr->addr.type0.min1 == cdma.imsi_s1) &&
          (msg_hdr->addr.type0.min2 == cdma.imsi_s2))
      {
        addr_found = TRUE;
      }
      break;

    case CAI_ESN_ADDR_TYPE:
      if ((qw_hi( msg_hdr->addr.type1.esn ) == 0) &&
          (qw_lo( msg_hdr->addr.type1.esn ) == cdma.esn))
      {
        addr_found = TRUE;
      }
      break;

    case CAI_IMSI_ADDR_TYPE:
      /* IMSI_CLASS_0_ADDR_NUM denotes the phone has a class 0 IMSI */
      if ((msg_hdr->addr.type2.imsi.imsi_class == 0) &&
          (cdma.imsi_addr_num == IMSI_CLASS_0_ADDR_NUM))
      {
        /* We have a class zero IMSI and have been sent a class zero IMSI */
        imsi0 = &msg_hdr->addr.type2.imsi.class_type.zero;
        switch (imsi0->imsi_class_0_type)
        {
          case 0:
            if ((qw_cmp( imsi0->type.zero.imsi_s, cdma.imsi_s ) == 0) &&
                (cur_bs_ptr->csp.esp.mcc == cdma.mcc) &&
                (cur_bs_ptr->csp.esp.imsi_11_12 == cdma.imsi_11_12))
            {
              addr_found = TRUE;
            }
            break;

          case 1:
            if ((qw_cmp( imsi0->type.one.imsi_s, cdma.imsi_s ) == 0) &&
                (cur_bs_ptr->csp.esp.mcc == cdma.mcc) &&
                (imsi0->type.one.imsi_11_12 == cdma.imsi_11_12))
            {
              addr_found = TRUE;
            }
            break;

          case 2:
            if ((qw_cmp( imsi0->type.two.imsi_s, cdma.imsi_s ) == 0) &&
                (imsi0->type.two.mcc == cdma.mcc) &&
                (cur_bs_ptr->csp.esp.imsi_11_12 == cdma.imsi_11_12))
            {
              addr_found = TRUE;
            }
            break;

          case 3:
            if ((qw_cmp( imsi0->type.three.imsi_s, cdma.imsi_s ) == 0) &&
                (imsi0->type.three.mcc == cdma.mcc) &&
                (imsi0->type.three.imsi_11_12 == cdma.imsi_11_12))
            {
              addr_found = TRUE;
            }
            break;

          default:
            break;
        } /* end case on class 0 imsi type */
      } /* end if this is a class 0 imsi */
      /* IMSI_CLASS_0_ADDR_NUM denotes the phone has a class 0 IMSI */
      else if ((msg_hdr->addr.type2.imsi.imsi_class == 1) &&
               (cdma.imsi_addr_num != IMSI_CLASS_0_ADDR_NUM))
      {
        imsi1 = &msg_hdr->addr.type2.imsi.class_type.one;
        switch (imsi1->imsi_class_1_type)
        {
          case 0:
            if ((qw_cmp( imsi1->type.zero.imsi_s, cdma.imsi_s ) == 0) &&
                (cur_bs_ptr->csp.esp.mcc == cdma.mcc) &&
                (imsi1->type.zero.imsi_addr_num == cdma.imsi_addr_num) &&
                (imsi1->type.zero.imsi_11_12 == cdma.imsi_11_12))
            {
              addr_found = TRUE;
            }
            break;

          case 1:
            if ((qw_cmp( imsi1->type.one.imsi_s, cdma.imsi_s ) == 0) &&
                (imsi1->type.one.mcc == cdma.mcc) &&
                (imsi1->type.one.imsi_addr_num == cdma.imsi_addr_num) &&
                (imsi1->type.one.imsi_11_12 == cdma.imsi_11_12))
            {
              addr_found = TRUE;
            }
            break;

          default:
            break;
        } /* end case on class 1 imsi type */
      } /* end else class 1 imsi */
      break;

    #ifdef FEATURE_IS2000_REL_D
#error code not present
    #endif /* FEATURE_IS2000_REL_D */

    case CAI_TMSI_ADDR_TYPE:
      addr_found = tmsi_address_match (msg_hdr);
      break;

    #ifdef FEATURE_BROADCAST_SMS
    case CAI_BCAST_ADDR_TYPE:
      addr_found = mccbcsms_comp_addr(msg_hdr, msg_frame);
      /* Check if this is a valid BCSMS. Irrespective of whether it is 
       * duplicate or not, set BCSMS in progress flag to FALSE. We just
       * need to be sure that this BCSMS is the same which has started
       * a little while ago. */
      if(cm_wms_bc_addr_match(msg_hdr->addr.type5.bc_addr))
      {	      
        mcc_bcsms_in_progress = FALSE;
        qw_set(mcc_bcsms_start_slot, 0, 0);
      }	
      break;
    #endif /*  FEATURE_BROADCAST_SMS */

    default:
      MSG_MED( "Got message with unknown address type", 0, 0, 0);
      break;

  } /* end switch on address type */

  return (addr_found);
 
} /* mcc_comp_addr */

/*===========================================================================

FUNCTION MCC_MATCH_MSG

DESCRIPTION
  This function checks all messages except page messages to see if the
  address matches.  If so the message is coppied into matched_message.

DEPENDENCIES
  None

RETURN VALUE
  TRUE if the address of the message matches and the message is
  not a duplicate, FALSE otherwise.

SIDE EFFECTS
  None

===========================================================================*/

boolean mcc_match_msg
(
  mccrxtx_msg_type *msg_ptr,
    /* Pointer to received message */
  mccdma_matched_msg_type *mm_ptr,
    /* Pointer to struct to copy message into if match is found. */
  boolean store_chnasn_for_tci,
    /* Indicates if TC CAM and ECAM should be stored for TCI operation. */
  word curr_state
    /* State currently being processed */
)
{
  boolean match_found = FALSE;
    /* Flag if address matches */
  caii_ca_ord_type *ca_ptr;
    /* Pointer to channel assignment with matching address */
  caii_ext_ca_ord_type *ext_ca_ptr;
    /* Pointer to extended channel assignment with matching address */
  word i;
    /* Counter to look through bundled messages */

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  #ifdef FEATURE_HDR
#error code not present
  #endif /* FEATURE_HDR */

  /**************************************
  * Extended Channel Assignment Message *
  **************************************/

  if ( ( msg_ptr->msg.gen.msg_type == CAI_EXT_CHN_ASN_MSG )
       #ifdef FEATURE_MEID_SUPPORT
       || ( msg_ptr->msg.gen.msg_type == CAI_MEID_EXT_CHN_ASN_MSG )
       #endif /* FEATURE_MEID_SUPPORT */
     )
  {
    for (i=0; i < msg_ptr->msg.ext_chnasn.num_chns; i++)
    {
      if (mcc_comp_addr( &msg_ptr->msg.ext_chnasn.chns[i]->gen.hdr,
                         msg_ptr->frm.frame))
      {
        /* -----------------------------------------------------
        ** Found a Extended Channel Assignment record addressed to mobile.
        ** ----------------------------------------------------- */
        MSG_HIGH( "Rxed Extended Channel Assignment Msg", 0, 0, 0 );

        match_found = TRUE;
        break;
      } /* end if (address matches) */
    } /* end for */

    if (match_found)
    {
      /* -------------------------------------------------------
      ** Copy extended channel assignment message over to matched_message
      ** ------------------------------------------------------- */
      /* Get a pointer for shorthand during copying */
      ext_ca_ptr = msg_ptr->msg.ext_chnasn.chns[i];

      mm_ptr->chnasn.msg_type = msg_ptr->msg.ext_chnasn.msg_type;
      mm_ptr->chnasn.hdr = ext_ca_ptr->gen.hdr;
      mm_ptr->chnasn.assign_mode = ext_ca_ptr->gen.assign_mode;

      switch (mm_ptr->chnasn.assign_mode)
      {
        case CAI_TRAFFIC_CHAN_ASSIGN:
          mm_ptr->chnasn.mode.am0.freq_incl = ext_ca_ptr->am0.freq_incl;
          mm_ptr->chnasn.mode.am0.default_config =
            ext_ca_ptr->am0.default_config;
          mm_ptr->chnasn.mode.am0.bypass_alert_answer =
            ext_ca_ptr->am0.bypass_alert_answer;
          /* the internal num_pilots is the actual num of pilots, as oppose
           * to actual - 1 that is specified in IS95-B std. */
          mm_ptr->chnasn.mode.am0.num_pilots = ext_ca_ptr->am0.num_pilots+1;
          mm_ptr->chnasn.mode.am0.granted_mode=ext_ca_ptr->am0.granted_mode;
          mm_ptr->chnasn.mode.am0.cdma_freq = ext_ca_ptr->am0.cdma_freq;
          mm_ptr->chnasn.mode.am0.band_class = ext_ca_ptr->am0.band_class;
          mm_ptr->chnasn.mode.am0.frame_offset=ext_ca_ptr->am0.frame_offset;
          mm_ptr->chnasn.mode.am0.encrypt_mode=ext_ca_ptr->am0.encrypt_mode;

          #ifdef FEATURE_IS2000_REL_C
          // Default EXT_CH_IND in am0 when field not included in OTA msg
          mm_ptr->chnasn.mode.am0.ext_ch_ind = CAI_EXT_CHIND_UNSET; /*lint !e641 */
          #endif /* FEATURE_IS2000_REL_C */

          #ifdef FEATURE_IS2000_REL_A
          /* Signaling Encryption Fields */
          mm_ptr->chnasn.mode.am0.d_sig_encrypt_mode = ext_ca_ptr->am0.d_sig_encrypt_mode;
          mm_ptr->chnasn.mode.am0.key_size = ext_ca_ptr->am0.enc_key_size;
          mm_ptr->chnasn.mode.am0.c_sig_encrypt_mode_incl = ext_ca_ptr->am0.c_sig_encrypt_mode_incl;
          mm_ptr->chnasn.mode.am0.c_sig_encrypt_mode = ext_ca_ptr->am0.c_sig_encrypt_mode;
          #endif /* FEATURE_IS2000_REL_A */

          for (i=0; i<mm_ptr->chnasn.mode.am0.num_pilots; i++)
          {
            mm_ptr->chnasn.mode.am0.pilot_rec[i].pilot_pn =
                    ext_ca_ptr->am0.pilot_rec[i].pilot_pn;
            mm_ptr->chnasn.mode.am0.pilot_rec[i].pwr_comb_ind =
                    ext_ca_ptr->am0.pilot_rec[i].pwr_comb_ind;
            mm_ptr->chnasn.mode.am0.pilot_rec[i].code_chan_fch =
                    ext_ca_ptr->am0.pilot_rec[i].code_chan;
            mm_ptr->chnasn.mode.am0.pilot_rec[i].qof_mask_id_fch = 0;
            mm_ptr->chnasn.mode.am0.pilot_rec[i].add_pilot_rec_incl = 0;

            MSG_MED( "ECAM rec'd, pn=%d, pwr=%d, chan=%d",
                    ext_ca_ptr->am0.pilot_rec[i].pilot_pn,
                    ext_ca_ptr->am0.pilot_rec[i].pwr_comb_ind,
                    ext_ca_ptr->am0.pilot_rec[i].code_chan);

          }
          /* The following are some defaults, The actual RCs where
             necessary will be deduced later for p_rev_in use less than 6 */
          mm_ptr->chnasn.mode.am0.for_rc = CAI_RC_1;
          mm_ptr->chnasn.mode.am0.rev_rc = CAI_RC_1;

          if (P_REV_IS_6_OR_GREATER)
          {
             mm_ptr->chnasn.mode.am0.for_rc = (cai_radio_config_type) ext_ca_ptr->am0.for_fch_rc;
             mm_ptr->chnasn.mode.am0.rev_rc = (cai_radio_config_type) ext_ca_ptr->am0.rev_fch_rc;
             mm_ptr->chnasn.mode.am0.fpc_fch_init_setpt = ext_ca_ptr->am0.fpc_fch_init_setpt;
             mm_ptr->chnasn.mode.am0.fpc_fch_min_setpt = ext_ca_ptr->am0.fpc_fch_min_setpt;
             mm_ptr->chnasn.mode.am0.fpc_fch_max_setpt = ext_ca_ptr->am0.fpc_fch_max_setpt;
             mm_ptr->chnasn.mode.am0.fpc_fch_fer = ext_ca_ptr->am0.fpc_fch_fer;
             mm_ptr->chnasn.mode.am0.fpc_subchan_gain = ext_ca_ptr->am0.fpc_subchan_gain;
             mm_ptr->chnasn.mode.am0.rl_gain_adj = ext_ca_ptr->am0.rl_gain_adj;
             mm_ptr->chnasn.mode.am0.rev_fch_gating_on = ext_ca_ptr->am0.rev_fch_gating_mode;
             mm_ptr->chnasn.mode.am0.rev_pwr_cntl_delay_incl =
                                      ext_ca_ptr->am0.rev_pwr_cntl_delay_incl;
             if (mm_ptr->chnasn.mode.am0.rev_pwr_cntl_delay_incl)
             {
               mm_ptr->chnasn.mode.am0.rev_pwr_cntl_delay =
                 ext_ca_ptr->am0.rev_pwr_cntl_delay;
             }
             #if defined(FEATURE_IS2000_REL_C) || defined(FEATURE_MEID_SUPPORT)
             #ifdef FEATURE_IS2000_REL_C
             if (P_REV_IS_9_OR_GREATER)
             {
               mm_ptr->chnasn.mode.am0.msg_int_info_incl = 
                 ext_ca_ptr->am0.msg_int_info_incl;
               if (mm_ptr->chnasn.mode.am0.msg_int_info_incl)
               {
                 mm_ptr->chnasn.mode.am0.change_keys = 
                   ext_ca_ptr->am0.change_keys;
                 mm_ptr->chnasn.mode.am0.use_uak = 
                   ext_ca_ptr->am0.use_uak;
               }
               mm_ptr->chnasn.mode.am0.plcm_type_incl = 
                 ext_ca_ptr->am0.plcm_type_incl;
               if (mm_ptr->chnasn.mode.am0.plcm_type_incl)
               {
                 mm_ptr->chnasn.mode.am0.plcm_type = 
                   ext_ca_ptr->am0.plcm_type;
                 qw_equ(mm_ptr->chnasn.mode.am0.plcm_39,  
                        ext_ca_ptr->am0.plcm_39);
               }
             }
             else
             #endif /* FEATURE_IS2000_REL_C */
             {
               mm_ptr->chnasn.mode.am0.msg_int_info_incl = FALSE; 
               mm_ptr->chnasn.mode.am0.plcm_type_incl = FALSE;
             }
             #endif /* FEATURE_IS2000_REL_C || FEATURE_MEID_SUPPORT */

          }
          else
          {
            if (( ext_ca_ptr->am0.granted_mode == CAI_GRANT_USE_DC) &&
               (ext_ca_ptr->am0.default_config == CAI_DEF_CONFIG_FOR1_REV1))
            {
               // Set correct values  for RC if not present in message
               mm_ptr->chnasn.mode.am0.for_rc = CAI_RC_1;
               mm_ptr->chnasn.mode.am0.rev_rc = CAI_RC_1;

            }
            else if  (( ext_ca_ptr->am0.granted_mode == CAI_GRANT_USE_DC) &&
               (ext_ca_ptr->am0.default_config == CAI_DEF_CONFIG_FOR2_REV2))
            {
               // Set correct values  for RC if not present in message
               mm_ptr->chnasn.mode.am0.for_rc = CAI_RC_2;
               mm_ptr->chnasn.mode.am0.rev_rc = CAI_RC_2;
            }
            else
            {
              /* Should not happen but if BS assigns assymetric rate set which we
                 don't support, we'll get here */
              mm_ptr->chnasn.mode.am0.for_rc = CAI_RC_1;
              mm_ptr->chnasn.mode.am0.rev_rc = CAI_RC_1;
            }
            /* Initialize IS2000 related fields */
            mm_ptr->chnasn.mode.am0.fpc_fch_init_setpt =
            mm_ptr->chnasn.mode.am0.fpc_subchan_gain =
            mm_ptr->chnasn.mode.am0.rl_gain_adj =
            mm_ptr->chnasn.mode.am0.fpc_fch_fer =
            mm_ptr->chnasn.mode.am0.fpc_fch_min_setpt =
            mm_ptr->chnasn.mode.am0.fpc_fch_max_setpt =
            mm_ptr->chnasn.mode.am0.rev_fch_gating_on =
            mm_ptr->chnasn.mode.am0.rev_pwr_cntl_delay_incl =
            mm_ptr->chnasn.mode.am0.rev_pwr_cntl_delay = 0;
          }

          mm_ptr->chnasn.mode.am0.ch_ind = CAI_CHIND_FCH_ONLY;
          mm_ptr->chnasn.mode.am0.fpc_pri_chan = 0;

          /* If P_rev in use is less than 6, no need to attempt any defaults now
             The FFPC parameters will not be used in this case, as for RC, we can deduce
             that when we resolve the granted mode */

          if (store_chnasn_for_tci)
          {
            /* ----------------------------------
            ** Store channel assignment for use
            ** when we go to the traffic channel.
            ** ---------------------------------- */
            cdma.chnasn = mm_ptr->chnasn;
          }

          break;

        case CAI_PAGING_CHAN_ASSIGN:
          mm_ptr->chnasn.mode.am1.respond = ext_ca_ptr->am1.respond;
          mm_ptr->chnasn.mode.am1.freq_incl = ext_ca_ptr->am1.freq_incl;
          mm_ptr->chnasn.mode.am1.band_class = ext_ca_ptr->am1.band_class;
          mm_ptr->chnasn.mode.am1.cdma_freq = ext_ca_ptr->am1.cdma_freq;
          mm_ptr->chnasn.mode.am1.num_pilots = ext_ca_ptr->am1.num_pilots+1;
          for (i=0; i<mm_ptr->chnasn.mode.am1.num_pilots; i++)
          {
            mm_ptr->chnasn.mode.am1.pilot_pn[i] = ext_ca_ptr->am1.pilot_pn[i];
          }
          break;

        case CAI_ACQ_ANALOG_SYSTEM:
          mm_ptr->chnasn.mode.am2.respond = ext_ca_ptr->am2.respond;
          mm_ptr->chnasn.mode.am2.analog_sys = ext_ca_ptr->am2.analog_sys;
          mm_ptr->chnasn.mode.am2.use_analog_sys=ext_ca_ptr->am2.use_analog_sys;
          mm_ptr->chnasn.mode.am2.band_class = ext_ca_ptr->am2.band_class;
          break;

        case CAI_ANALOG_VCHAN_ASSIGN:
          mm_ptr->chnasn.mode.am3.sid = ext_ca_ptr->am3.sid;
          mm_ptr->chnasn.mode.am3.vmac = ext_ca_ptr->am3.vmac;
          mm_ptr->chnasn.mode.am3.analog_chan = ext_ca_ptr->am3.analog_chan;
          mm_ptr->chnasn.mode.am3.scc = ext_ca_ptr->am3.scc;
          mm_ptr->chnasn.mode.am3.mem = ext_ca_ptr->am3.mem;
          mm_ptr->chnasn.mode.am3.an_chan_type=ext_ca_ptr->am3.an_chan_type;
          mm_ptr->chnasn.mode.am3.band_class = ext_ca_ptr->am3.band_class;
          break;

        case CAI_EXT_TRAF_CHAN_ASSIGN:
        {
          /* A short-hand for the am0 structure */
          mccdma_chnasn_am0_type *am0_ptr = &(mm_ptr->chnasn.mode.am0);

          #ifdef FEATURE_IS2000_REL_D
#error code not present
          #endif /* FEATURE_IS2000_REL_D */

          am0_ptr->freq_incl = ext_ca_ptr->am4.freq_incl;
          am0_ptr->default_config = ext_ca_ptr->am4.default_config;
          am0_ptr->bypass_alert_answer = ext_ca_ptr->am4.bypass_alert_answer;
          /* the internal num_pilots is the actual num of pilots, as oppose
          * to actual - 1 that is specified in IS95-B std. */
          am0_ptr->num_pilots = ext_ca_ptr->am4.num_pilots+1;
          am0_ptr->granted_mode = ext_ca_ptr->am4.granted_mode;
          am0_ptr->cdma_freq = ext_ca_ptr->am4.cdma_freq;
          am0_ptr->band_class = ext_ca_ptr->am4.band_class;
          am0_ptr->frame_offset=ext_ca_ptr->am4.frame_offset;
          am0_ptr->encrypt_mode=ext_ca_ptr->am4.encrypt_mode;
          am0_ptr->ch_ind= (cai_chind_type)ext_ca_ptr->am4.ch_ind;

          #ifdef FEATURE_IS2000_REL_A
          /* Initialize to default values, they should be updated in following section */
          am0_ptr->x3_fch_info_incl = FALSE;
          am0_ptr->x3_dcch_info_incl = FALSE;
          #endif /* FEATURE_IS2000_REL_A */

          #ifdef FEATURE_IS2000_REL_C
          am0_ptr->sr_id_restore = ext_ca_ptr->am4.sr_id_restore;

          /* Default EXT_CH_IND in am0 when field not included in OTA msg */
          am0_ptr->ext_ch_ind = CAI_EXT_CHIND_UNSET; /*lint !e641 */
          #endif /* FAETURE_IS2000_REL_C */

          #ifdef FEATURE_IS2000_REL_D
#error code not present
          #endif /* FEATURE_IS2000_REL_D */

          switch (ext_ca_ptr->am4.ch_ind)
          {
            case CAI_CHIND_FCH_ONLY:
            {
              am0_ptr->fpc_pri_chan = CAI_FPC_PRI_CHAN_FCH; /*lint !e641 */
              am0_ptr->fpc_fch_init_setpt = ext_ca_ptr->am4.ch_rec.ind1.fpc_fch_init_setpt;
              am0_ptr->fpc_fch_min_setpt = ext_ca_ptr->am4.ch_rec.ind1.fpc_fch_min_setpt;
              am0_ptr->fpc_fch_max_setpt = ext_ca_ptr->am4.ch_rec.ind1.fpc_fch_max_setpt;
              am0_ptr->fpc_fch_fer = ext_ca_ptr->am4.ch_rec.ind1.fpc_fch_fer;

              #ifdef FEATURE_IS2000_REL_A
              am0_ptr->x3_fch_info_incl = ext_ca_ptr->am4.ch_rec.ind1.x3_fch_info_incl;
              #endif /* FEATURE_IS2000_REL_A */

              for (i=0; i<am0_ptr->num_pilots; i++)
              {
                am0_ptr->pilot_rec[i].pilot_pn = ext_ca_ptr->am4.ch_rec.ind1.pilot_rec[i].pilot_pn;
                am0_ptr->pilot_rec[i].add_pilot_rec_incl = ext_ca_ptr->am4.ch_rec.ind1.pilot_rec[i].add_pilot_rec_incl;
                am0_ptr->pilot_rec[i].pilot_rec_type = ext_ca_ptr->am4.ch_rec.ind1.pilot_rec[i].pilot_rec_type;
                am0_ptr->pilot_rec[i].add_pilot_rec = ext_ca_ptr->am4.ch_rec.ind1.pilot_rec[i].add_pilot_rec;
                am0_ptr->pilot_rec[i].pwr_comb_ind = ext_ca_ptr->am4.ch_rec.ind1.pilot_rec[i].pwr_comb_ind;
                am0_ptr->pilot_rec[i].code_chan_fch = ext_ca_ptr->am4.ch_rec.ind1.pilot_rec[i].code_chan_fch;
                am0_ptr->pilot_rec[i].qof_mask_id_fch = ext_ca_ptr->am4.ch_rec.ind1.pilot_rec[i].qof_mask_id_fch;

                #ifdef FEATURE_IS2000_REL_A
                am0_ptr->fch_info[i] = ext_ca_ptr->am4.ch_rec.ind1.fch_info[i];
                #endif /* FEATURE_IS2000_REL_A */

              }
            }
            break;

            case CAI_CHIND_DCCH_ONLY:
            {
              am0_ptr->fpc_pri_chan = CAI_FPC_PRI_CHAN_DCCH; /*lint !e641 */
              am0_ptr->fpc_dcch_init_setpt = ext_ca_ptr->am4.ch_rec.ind2.fpc_dcch_init_setpt;
              am0_ptr->fpc_dcch_min_setpt = ext_ca_ptr->am4.ch_rec.ind2.fpc_dcch_min_setpt;
              am0_ptr->fpc_dcch_max_setpt = ext_ca_ptr->am4.ch_rec.ind2.fpc_dcch_max_setpt;
              am0_ptr->fpc_dcch_fer = ext_ca_ptr->am4.ch_rec.ind2.fpc_dcch_fer;

              #ifdef FEATURE_IS2000_REL_A
              am0_ptr->x3_dcch_info_incl = ext_ca_ptr->am4.ch_rec.ind2.x3_dcch_info_incl;
              #endif /* FEATURE_IS2000_REL_A */

              for (i=0; i<am0_ptr->num_pilots; i++)
              {
                am0_ptr->pilot_rec[i].pilot_pn = ext_ca_ptr->am4.ch_rec.ind2.pilot_rec[i].pilot_pn;
                am0_ptr->pilot_rec[i].add_pilot_rec_incl = ext_ca_ptr->am4.ch_rec.ind2.pilot_rec[i].add_pilot_rec_incl;
                am0_ptr->pilot_rec[i].pilot_rec_type = ext_ca_ptr->am4.ch_rec.ind2.pilot_rec[i].pilot_rec_type;
                am0_ptr->pilot_rec[i].add_pilot_rec = ext_ca_ptr->am4.ch_rec.ind2.pilot_rec[i].add_pilot_rec;
                am0_ptr->pilot_rec[i].pwr_comb_ind = ext_ca_ptr->am4.ch_rec.ind2.pilot_rec[i].pwr_comb_ind;
                am0_ptr->pilot_rec[i].code_chan_dcch = ext_ca_ptr->am4.ch_rec.ind2.pilot_rec[i].code_chan_dcch;
                am0_ptr->pilot_rec[i].qof_mask_id_dcch = ext_ca_ptr->am4.ch_rec.ind2.pilot_rec[i].qof_mask_id_dcch;

                #ifdef FEATURE_IS2000_REL_A
                am0_ptr->dcch_info[i] = ext_ca_ptr->am4.ch_rec.ind2.dcch_info[i];
                #endif /* FEATURE_IS2000_REL_A */

                #ifdef FEATURE_IS2000_REL_D
#error code not present
                #endif /* FEATURE_IS2000_REL_D */

              }
            }
            break;

            case CAI_CHIND_FCH_AND_DCCH:
            {
              am0_ptr->fpc_pri_chan = ext_ca_ptr->am4.ch_rec.ind3.fpc_pri_chan;
              am0_ptr->fpc_fch_init_setpt = ext_ca_ptr->am4.ch_rec.ind3.fpc_fch_init_setpt;
              am0_ptr->fpc_fch_min_setpt = ext_ca_ptr->am4.ch_rec.ind3.fpc_fch_min_setpt;
              am0_ptr->fpc_fch_max_setpt = ext_ca_ptr->am4.ch_rec.ind3.fpc_fch_max_setpt;
              am0_ptr->fpc_fch_fer = ext_ca_ptr->am4.ch_rec.ind3.fpc_fch_fer;
              am0_ptr->fpc_dcch_init_setpt = ext_ca_ptr->am4.ch_rec.ind3.fpc_dcch_init_setpt;
              am0_ptr->fpc_dcch_min_setpt = ext_ca_ptr->am4.ch_rec.ind3.fpc_dcch_min_setpt;
              am0_ptr->fpc_dcch_max_setpt = ext_ca_ptr->am4.ch_rec.ind3.fpc_dcch_max_setpt;
              am0_ptr->fpc_dcch_fer= ext_ca_ptr->am4.ch_rec.ind3.fpc_dcch_fer;

              #ifdef FEATURE_IS2000_REL_A
              am0_ptr->x3_fch_info_incl = ext_ca_ptr->am4.ch_rec.ind3.x3_fch_info_incl;
              am0_ptr->x3_dcch_info_incl = ext_ca_ptr->am4.ch_rec.ind3.x3_dcch_info_incl;
              #endif /* FEATURE_IS2000_REL_A */

              for (i=0; i<am0_ptr->num_pilots; i++)
              {
                am0_ptr->pilot_rec[i].pilot_pn = ext_ca_ptr->am4.ch_rec.ind3.pilot_rec[i].pilot_pn;
                am0_ptr->pilot_rec[i].add_pilot_rec_incl = ext_ca_ptr->am4.ch_rec.ind3.pilot_rec[i].add_pilot_rec_incl;
                am0_ptr->pilot_rec[i].pilot_rec_type = ext_ca_ptr->am4.ch_rec.ind3.pilot_rec[i].pilot_rec_type;
                am0_ptr->pilot_rec[i].add_pilot_rec = ext_ca_ptr->am4.ch_rec.ind3.pilot_rec[i].add_pilot_rec;
                am0_ptr->pilot_rec[i].pwr_comb_ind = ext_ca_ptr->am4.ch_rec.ind3.pilot_rec[i].pwr_comb_ind;
                am0_ptr->pilot_rec[i].code_chan_dcch = ext_ca_ptr->am4.ch_rec.ind3.pilot_rec[i].code_chan_dcch;
                am0_ptr->pilot_rec[i].qof_mask_id_dcch = ext_ca_ptr->am4.ch_rec.ind3.pilot_rec[i].qof_mask_id_dcch;
                am0_ptr->pilot_rec[i].code_chan_fch = ext_ca_ptr->am4.ch_rec.ind3.pilot_rec[i].code_chan_fch;
                am0_ptr->pilot_rec[i].qof_mask_id_fch = ext_ca_ptr->am4.ch_rec.ind3.pilot_rec[i].qof_mask_id_fch;

                #ifdef FEATURE_IS2000_REL_A
                am0_ptr->fch_info[i] = ext_ca_ptr->am4.ch_rec.ind3.fch_info[i];
                am0_ptr->dcch_info[i] = ext_ca_ptr->am4.ch_rec.ind3.dcch_info[i];
                #endif /* FEATURE_IS2000_REL_A */

                #ifdef FEATURE_IS2000_REL_D
#error code not present
                #endif /* FEATURE_IS2000_REL_D */

              }
            }

            break;

            default:
              break;
          }

          am0_ptr->for_rc = (cai_radio_config_type) ext_ca_ptr->am4.for_rc;
          am0_ptr->rev_rc = (cai_radio_config_type) ext_ca_ptr->am4.rev_rc;
          am0_ptr->rl_gain_adj = ext_ca_ptr->am4.rlgain_adj;
          am0_ptr->fpc_subchan_gain = ext_ca_ptr->am4.fpc_subchan_gain;
          am0_ptr->rev_fch_gating_on = ext_ca_ptr->am4.rev_fch_gating_mode;
          am0_ptr->rev_pwr_cntl_delay_incl = ext_ca_ptr->am4.rev_pwr_cntl_delay_incl;

          if (am0_ptr->rev_pwr_cntl_delay_incl)
          {
            am0_ptr->rev_pwr_cntl_delay = ext_ca_ptr->am4.rev_pwr_cntl_delay;
          }

          #ifdef FEATURE_IS2000_REL_A
          /* Signaling Encryption Fields */
          am0_ptr->d_sig_encrypt_mode = ext_ca_ptr->am4.d_sig_encrypt_mode;
          am0_ptr->key_size = ext_ca_ptr->am4.enc_key_size;
          am0_ptr->c_sig_encrypt_mode_incl = ext_ca_ptr->am4.c_sig_encrypt_mode_incl;
          am0_ptr->c_sig_encrypt_mode = ext_ca_ptr->am4.c_sig_encrypt_mode;
          am0_ptr->x3fl_1xrl_incl = ext_ca_ptr->am4.x3fl_1xrl_incl;
          am0_ptr->x1rl_freq_offset = ext_ca_ptr->am4.x1rl_freq_offset;
          #endif /* FEATURE_IS2000_REL_A */

          #if defined(FEATURE_IS2000_REL_C) || defined(FEATURE_MEID_SUPPORT)
          if (
              #ifdef FEATURE_IS2000_REL_C
              (P_REV_IS_9_OR_GREATER && (msg_ptr->msg.gen.msg_type == CAI_EXT_CHN_ASN_MSG))
              #endif /* FEATURE_IS2000_REL_C */

              #if defined(FEATURE_IS2000_REL_C) && defined(FEATURE_MEID_SUPPORT)
              ||
              #endif /* FEATURE_IS2000_REL_C && FEATURE_MEID_SUPPORT */

              #ifdef FEATURE_MEID_SUPPORT
              (msg_ptr->msg.gen.msg_type == CAI_MEID_EXT_CHN_ASN_MSG)
              #endif /* FEATURE_MEID_SUPPORT */
             )
          {
            mm_ptr->chnasn.mode.am0.msg_int_info_incl = 
              ext_ca_ptr->am4.msg_int_info_incl;
            if (mm_ptr->chnasn.mode.am0.msg_int_info_incl)
            {
              mm_ptr->chnasn.mode.am0.change_keys = 
                ext_ca_ptr->am4.change_keys;
              mm_ptr->chnasn.mode.am0.use_uak = 
                ext_ca_ptr->am4.use_uak;
            }
            mm_ptr->chnasn.mode.am0.plcm_type_incl = 
              ext_ca_ptr->am4.plcm_type_incl;
            if (mm_ptr->chnasn.mode.am0.plcm_type_incl)
            {
              mm_ptr->chnasn.mode.am0.plcm_type = 
                ext_ca_ptr->am4.plcm_type;
              qw_equ(mm_ptr->chnasn.mode.am0.plcm_39,  
                     ext_ca_ptr->am4.plcm_39);
            }
          }
          else
          {
            mm_ptr->chnasn.mode.am0.msg_int_info_incl = FALSE;
            mm_ptr->chnasn.mode.am0.plcm_type_incl = FALSE;
          }
          #endif /* FEATURE_IS2000_REL_C || FEATURE_MEID_SUPPORT */

          if (store_chnasn_for_tci)
          {
            /* ----------------------------------
            ** Store channel assignment for use
            ** when we go to the traffic channel.
            ** ---------------------------------- */
            cdma.chnasn = mm_ptr->chnasn;
          }

          break;
        }

        #if defined(FEATURE_IS2000_REL_C_DV) || defined(IS2000_DV_SIG_TEST)
        /* Note that CAI_PACKET_DATA_TRAF_CHAN_ASSIGN (ECAM - PREV>=9) and 
           CAI_EXT_PAGING_CHAN_ASSIGN (CAM - P_REV<8) are both value 5. 
           Because there are separate switch statements for CAM and ECAM 
           assign modes in this function this is not a problem. */

        case CAI_PACKET_DATA_TRAF_CHAN_ASSIGN:
        {
          /* Copy fields from caii am5 structure to "common" mccdma
          am0 structure used for Traffic, Ext Traffic and DV - also set
          any defaults for the mccdma_chnasn_am0_type fields that are not
          part of am5. */

          /* A short-hand for the am0 structure */
          mccdma_chnasn_am0_type *am0_ptr = &(mm_ptr->chnasn.mode.am0);
          MSG_HIGH("DV ASSIGNMENT - EXTCHIND %d",ext_ca_ptr->am5.ext_ch_ind,0,0);

          #ifdef FEATURE_IS2000_REL_D
#error code not present
          #endif /* FEATURE_IS2000_REL_D */

          am0_ptr->freq_incl = ext_ca_ptr->am5.freq_incl;
          am0_ptr->default_config = CAI_DEF_CONFIG_USE_ASSGN_RC;  // Field not in am5
          am0_ptr->band_class = ext_ca_ptr->am5.band_class;
          am0_ptr->cdma_freq = ext_ca_ptr->am5.cdma_freq;
          am0_ptr->bypass_alert_answer = ext_ca_ptr->am5.bypass_alert_answer;
          am0_ptr->granted_mode=ext_ca_ptr->am5.granted_mode;
          am0_ptr->sr_id_restore = ext_ca_ptr->am5.sr_id_restore;

          #ifdef FEATURE_IS2000_REL_D
#error code not present
          #endif /* FEATURE_IS2000_REL_D */

          am0_ptr->frame_offset=ext_ca_ptr->am5.frame_offset;
          am0_ptr->encrypt_mode=ext_ca_ptr->am5.encrypt_mode;
          am0_ptr->d_sig_encrypt_mode = ext_ca_ptr->am5.d_sig_encrypt_mode;
          am0_ptr->key_size = ext_ca_ptr->am5.enc_key_size;
          am0_ptr->c_sig_encrypt_mode_incl = ext_ca_ptr->am5.c_sig_encrypt_mode_incl;
          am0_ptr->c_sig_encrypt_mode = ext_ca_ptr->am5.c_sig_encrypt_mode;
          am0_ptr->x1rl_freq_offset = 0;  // Field not in am5
          am0_ptr->msg_int_info_incl = ext_ca_ptr->am5.msg_int_info_incl;
          am0_ptr->change_keys = ext_ca_ptr->am5.change_keys;
          am0_ptr->use_uak = ext_ca_ptr->am5.use_uak;
          am0_ptr->plcm_type_incl = ext_ca_ptr->am5.plcm_type_incl;
          am0_ptr->plcm_type = ext_ca_ptr->am5.plcm_type;
          qw_equ(am0_ptr->plcm_39, ext_ca_ptr->am5.plcm_39);
          am0_ptr->rl_gain_adj = ext_ca_ptr->am5.rlgain_adj;
          am0_ptr->ch_ind = 0; // Field not in am5
          am0_ptr->ext_ch_ind = ext_ca_ptr->am5.ext_ch_ind;
          am0_ptr->fpc_subchan_gain = ext_ca_ptr->am5.fpc_subchan_gain;
          am0_ptr->rev_fch_gating_on = ext_ca_ptr->am5.rev_fch_gating_mode;
          am0_ptr->rev_pwr_cntl_delay_incl = ext_ca_ptr->am5.rev_pwr_cntl_delay_incl;
          am0_ptr->rev_pwr_cntl_delay = ext_ca_ptr->am5.rev_pwr_cntl_delay;
          am0_ptr->full_ci_feedback_ind = ext_ca_ptr->am5.full_ci_feedback_ind;
          am0_ptr->rev_cqich_frame_offset = ext_ca_ptr->am5.rev_cqich_frame_offset;
          am0_ptr->rev_cqich_reps = ext_ca_ptr->am5.rev_cqich_reps;
          am0_ptr->rev_ackch_reps = ext_ca_ptr->am5.rev_ackch_reps;
          am0_ptr->for_pdch_rc = ext_ca_ptr->am5.for_pdch_rc;

          #ifdef FEATURE_IS2000_REL_D
#error code not present
          #endif /* FEATURE_IS2000_REL_D */

          am0_ptr->for_rc = ext_ca_ptr->am5.for_fch_dcch_rc;
          am0_ptr->rev_rc = ext_ca_ptr->am5.rev_fch_dcch_rc;
          /* Assign Mode 5 does not always include fpc_pri_chan
          ** determine value based on fwd channels from ext_ch_ind and
          ** set the message field to simplify other processing.
          */
          switch(am0_ptr->ext_ch_ind)
          {
            case CAI_EXT_CHIND_PC_F:
            case CAI_EXT_CHIND_PC_D:

            #ifdef FEATURE_IS2000_REL_D
#error code not present
            #endif /* FEATURE_IS2000_REL_D */

              /* Primary inner loop estimation on received F-CPCCH */
              am0_ptr->fpc_pri_chan = CAI_FPC_PRI_CHAN_CPCCH;
              break;
           
            case CAI_EXT_CHIND_PF_F:
            case CAI_EXT_CHIND_PF_FD:

            #ifdef FEATURE_IS2000_REL_D
#error code not present
            #endif /* FEATURE_IS2000_REL_D */

              /* Primary inner loop estimation on received F-FCH */
              /* Only F-FCH for Config 3 and 5 */ 
              am0_ptr->fpc_pri_chan = CAI_FPC_PRI_CHAN_FCH;
              break;

            case CAI_EXT_CHIND_PD_D:

            #ifdef FEATURE_IS2000_REL_D
#error code not present
            #endif /* FEATURE_IS2000_REL_D */

              /* Primary inner loop estimation on received F-DCCH */
              am0_ptr->fpc_pri_chan = CAI_FPC_PRI_CHAN_DCCH;
              break;

            case CAI_EXT_CHIND_PFD_FD:

            #ifdef FEATURE_IS2000_REL_D
#error code not present
            #endif /* FEATURE_IS2000_REL_D */

              /* Use value from message */ 
              am0_ptr->fpc_pri_chan = ext_ca_ptr->am5.fpc_pri_chan;
              break;

            default:
              MSG_ERROR("Unknown ext_ch_ind 0x%x", am0_ptr->ext_ch_ind, 0, 0);
              break;
          }

          am0_ptr->fpc_fch_init_setpt= ext_ca_ptr->am5.fpc_fch_init_setpt;
          am0_ptr->fpc_fch_fer = ext_ca_ptr->am5.fpc_fch_fer;
          am0_ptr->fpc_fch_min_setpt= ext_ca_ptr->am5.fpc_fch_min_setpt;
          am0_ptr->fpc_fch_max_setpt= ext_ca_ptr->am5.fpc_fch_max_setpt;
          am0_ptr->fpc_dcch_init_setpt= ext_ca_ptr->am5.fpc_dcch_init_setpt;
          am0_ptr->fpc_dcch_fer = ext_ca_ptr->am5.fpc_dcch_fer;
          am0_ptr->fpc_dcch_min_setpt= ext_ca_ptr->am5.fpc_dcch_min_setpt;
          am0_ptr->fpc_dcch_max_setpt= ext_ca_ptr->am5.fpc_dcch_max_setpt;
          am0_ptr->pdch_group_ind_incl= ext_ca_ptr->am5.pdch_group_ind_incl;
          am0_ptr->for_pdch_parms_incl= ext_ca_ptr->am5.for_pdch_parms_incl;
          am0_ptr->for_pdch_rlgain_incl= ext_ca_ptr->am5.for_pdch_parm.for_pdch_rlgain_incl;
          am0_ptr->rlgain_ackch_pilot = ext_ca_ptr->am5.for_pdch_parm.rlgain_ackch_pilot;
          am0_ptr->rlgain_cqich_pilot = ext_ca_ptr->am5.for_pdch_parm.rlgain_cqich_pilot;
          am0_ptr->num_soft_switching_frames = ext_ca_ptr->am5.for_pdch_parm.num_soft_switching_frames;
          am0_ptr->num_softer_switching_frames = ext_ca_ptr->am5.for_pdch_parm.num_softer_switching_frames;
          am0_ptr->num_soft_switching_slots = ext_ca_ptr->am5.for_pdch_parm.num_soft_switching_slots;
          am0_ptr->num_softer_switching_slots = ext_ca_ptr->am5.for_pdch_parm.num_softer_switching_slots;
          am0_ptr->chm_switching_params_incl = ext_ca_ptr->am5.for_pdch_parm.chm_switching_params_incl;
          am0_ptr->num_soft_switching_frames_chm = ext_ca_ptr->am5.for_pdch_parm.num_soft_switching_frames_chm;
          am0_ptr->num_softer_switching_frames_chm = ext_ca_ptr->am5.for_pdch_parm.num_softer_switching_frames_chm;
          am0_ptr->num_soft_switching_slots_chm = ext_ca_ptr->am5.for_pdch_parm.num_soft_switching_slots_chm;
          am0_ptr->num_softer_switching_slots_chm = ext_ca_ptr->am5.for_pdch_parm.num_softer_switching_slots_chm;
          am0_ptr->pdch_soft_switching_delay = ext_ca_ptr->am5.for_pdch_parm.pdch_soft_switching_delay;
          am0_ptr->pdch_softer_switching_delay = ext_ca_ptr->am5.for_pdch_parm.pdch_softer_switching_delay;

          #ifdef FEATURE_IS2000_REL_D
#error code not present
          #endif /* FEATURE_IS2000_REL_D */

          am0_ptr->num_pilots = ext_ca_ptr->am5.num_pilots+1;
          for (i=0; i<am0_ptr->num_pilots; i++)
          {
            // mccdma_pilot_info_type is the same structure as
            // caii_ext_chnasn_pilot_info_type - just copy the bits
            am0_ptr->pilot_rec[i] = ext_ca_ptr->am5.pilot_rec[i];
          }

          if (store_chnasn_for_tci)
          {
            /* ----------------------------------
            ** Store channel assignment for use
            ** when we go to the traffic channel.
            ** ---------------------------------- */
            cdma.chnasn = mm_ptr->chnasn;
          }

          break;
        }
        #endif /* FEATURE_IS2000_REL_C_DV */

        default:
          /* --------------------------------------------------
          ** Extended Channel Assignment with illegal assign mode!
          ** -------------------------------------------------- */
          ERR("Illegal assignment mode received %d",
            msg_ptr->msg.ext_chnasn.chns[i]->gen.assign_mode, 0, 0);
          break;

      } /* end switch (mm_ptr->chnasn.assign_mode) */
    } /* end if (this was a match) */
  } /* end if (this is a extended channel assignment) */

  /******************************
  * Channel Assignment Message *
  ******************************/

  else if (msg_ptr->msg.gen.msg_type == CAI_CHN_ASN_MSG)
  {
    for (i=0; i < msg_ptr->msg.chnasn.num_chns; i++)
    {
      if (mcc_comp_addr( &msg_ptr->msg.chnasn.chns[i].gen.hdr,
                         msg_ptr->frm.frame))
      {
        /* -----------------------------------------------------
        ** Found a Channel Assignment record addressed to mobile.
        ** ----------------------------------------------------- */
        MSG_HIGH( "Rxed Channel Assignment Msg", 0, 0, 0 );

        match_found = TRUE;
        break;
      } /* end if (address matches) */
    } /* end for */

    if (match_found)
    {
      /* -------------------------------------------------------
      ** Copy channel assignment message over to matched_message
      ** ------------------------------------------------------- */
      /* Get a pointer for shorthand during copying */
      ca_ptr = &msg_ptr->msg.chnasn.chns[i];

      mm_ptr->chnasn.msg_type = msg_ptr->msg.chnasn.msg_type;
      mm_ptr->chnasn.hdr = ca_ptr->gen.hdr;
      mm_ptr->chnasn.assign_mode = ca_ptr->gen.assign_mode;

      switch (mm_ptr->chnasn.assign_mode)
      {
        case CAI_TRAFFIC_CHAN_ASSIGN:
          /* Copy the fields from CAM */
          mm_ptr->chnasn.mode.am0.freq_incl = ca_ptr->am0.freq_incl;
          mm_ptr->chnasn.mode.am0.cdma_freq = ca_ptr->am0.cdma_freq;
          mm_ptr->chnasn.mode.am0.frame_offset = ca_ptr->am0.frame_offset;
          mm_ptr->chnasn.mode.am0.encrypt_mode = ca_ptr->am0.encrypt_mode;

          #ifdef FEATURE_IS2000_REL_A
          /* Signaling Encryption Fields */
          mm_ptr->chnasn.mode.am0.d_sig_encrypt_mode = ca_ptr->am0.d_sig_encrypt_mode;
          mm_ptr->chnasn.mode.am0.key_size = ca_ptr->am0.enc_key_size;
          mm_ptr->chnasn.mode.am0.c_sig_encrypt_mode_incl = ca_ptr->am0.c_sig_encrypt_mode_incl;
          mm_ptr->chnasn.mode.am0.c_sig_encrypt_mode = ca_ptr->am0.c_sig_encrypt_mode;
          #endif /* FEATURE_IS2000_REL_A */

          /* Initialize fields which are not used by CAM with defaults */
          mm_ptr->chnasn.mode.am0.default_config = CAI_DEF_CONFIG_FOR1_REV1;
          mm_ptr->chnasn.mode.am0.bypass_alert_answer = 0;
          mm_ptr->chnasn.mode.am0.num_pilots = 1;
          mm_ptr->chnasn.mode.am0.granted_mode = 0;
          mm_ptr->chnasn.mode.am0.band_class = cdma.band_class;
          mm_ptr->chnasn.mode.am0.pilot_rec[0].pilot_pn = cur_bs_ptr->pilot_pn;
          mm_ptr->chnasn.mode.am0.pilot_rec[0].pwr_comb_ind = 0;
          MSG_MED( "CAM rec'd, code_chan=%d", ca_ptr->am0.code_chan,0,0);

          mm_ptr->chnasn.mode.am0.pilot_rec[0].code_chan_fch =
                  ca_ptr->am0.code_chan;
          mm_ptr->chnasn.mode.am0.pilot_rec[0].qof_mask_id_fch = 0;
          mm_ptr->chnasn.mode.am0.pilot_rec[0].add_pilot_rec_incl = 0;
          mm_ptr->chnasn.mode.am0.for_rc = CAI_RC_1;
          mm_ptr->chnasn.mode.am0.rev_rc = CAI_RC_1;
          mm_ptr->chnasn.mode.am0.ch_ind = CAI_CHIND_FCH_ONLY;
          mm_ptr->chnasn.mode.am0.fpc_pri_chan = 0;
          mm_ptr->chnasn.mode.am0.rev_fch_gating_on = FALSE;
          mm_ptr->chnasn.mode.am0.rev_pwr_cntl_delay_incl = FALSE;
          mm_ptr->chnasn.mode.am0.rl_gain_adj = 0;

          #ifdef FEATURE_IS2000_REL_A
          mm_ptr->chnasn.mode.am0.x3_fch_info_incl = FALSE;
          mm_ptr->chnasn.mode.am0.x3_dcch_info_incl = FALSE;
          mm_ptr->chnasn.mode.am0.x3fl_1xrl_incl = FALSE;
          #endif

          #if defined(FEATURE_IS2000_REL_C) || defined(FEATURE_MEID_SUPPORT)
          mm_ptr->chnasn.mode.am0.msg_int_info_incl = FALSE;
          mm_ptr->chnasn.mode.am0.plcm_type_incl = FALSE;
          #endif

          if (store_chnasn_for_tci)
          {
            /* ----------------------------------
            ** Store channel assignment for use
            ** when we go to the traffic channel.
            ** ---------------------------------- */
            cdma.chnasn = mm_ptr->chnasn;
          }

          break;

        case CAI_PAGING_CHAN_ASSIGN:
          mm_ptr->chnasn.mode.am1.respond = ca_ptr->am1.respond;
          mm_ptr->chnasn.mode.am1.freq_incl = ca_ptr->am1.freq_incl;
          mm_ptr->chnasn.mode.am1.cdma_freq = ca_ptr->am1.cdma_freq;
          mm_ptr->chnasn.mode.am1.num_pilots = ca_ptr->am1.num_pilots;
          for (i=0; i<mm_ptr->chnasn.mode.am1.num_pilots; i++)
          {
            mm_ptr->chnasn.mode.am1.pilot_pn[i] = ca_ptr->am1.pilot_pn[i];
          }

          /* Initialize fields which are not used by CAM with defaults */
          mm_ptr->chnasn.mode.am1.band_class = cdma.band_class;

          break;

        case CAI_ACQ_ANALOG_SYSTEM:
          mm_ptr->chnasn.mode.am2.respond = ca_ptr->am2.respond;
          mm_ptr->chnasn.mode.am2.analog_sys = ca_ptr->am2.analog_sys;
          mm_ptr->chnasn.mode.am2.band_class = ca_ptr->am2.band_class;
          if( P_REV_IS_1 )
          {
            /* -------------------------------------------------------
            ** In J-STD-008 the Channel Assignment Message forces the
            ** analog_sys field to be set so we hard code the internal
            ** use_analog_sys field to TRUE.
            ** ------------------------------------------------------- */
            mm_ptr->chnasn.mode.am2.use_analog_sys = TRUE;
          }
          else
          {
            mm_ptr->chnasn.mode.am2.use_analog_sys =
              ca_ptr->am2.use_analog_sys;
          }
          break;

        case CAI_ANALOG_VCHAN_ASSIGN:
          mm_ptr->chnasn.mode.am3.sid = ca_ptr->am3.sid;
          mm_ptr->chnasn.mode.am3.vmac = ca_ptr->am3.vmac;
          mm_ptr->chnasn.mode.am3.analog_chan = ca_ptr->am3.analog_chan;
          mm_ptr->chnasn.mode.am3.scc = ca_ptr->am3.scc;
          mm_ptr->chnasn.mode.am3.mem = ca_ptr->am3.mem;
          mm_ptr->chnasn.mode.am3.an_chan_type = ca_ptr->am3.an_chan_type;
          mm_ptr->chnasn.mode.am3.band_class = ca_ptr->am3.band_class;
          break;

        case CAI_EXT_TRAF_CHAN_ASSIGN:
          mm_ptr->chnasn.mode.am0.freq_incl = ca_ptr->am4.freq_incl;
          mm_ptr->chnasn.mode.am0.bypass_alert_answer =
                                  ca_ptr->am4.bypass_alert_answer;
          mm_ptr->chnasn.mode.am0.granted_mode = ca_ptr->am4.grant_mode;

          /* The following are some defaults, The actual RCs where
             necessary will be deduced later for p_rev_in use less than 6 */
          mm_ptr->chnasn.mode.am0.for_rc = CAI_RC_1;
          mm_ptr->chnasn.mode.am0.rev_rc = CAI_RC_1;
          mm_ptr->chnasn.mode.am0.ch_ind = CAI_CHIND_FCH_ONLY;
          mm_ptr->chnasn.mode.am0.fpc_pri_chan = 0;
          mm_ptr->chnasn.mode.am0.rev_fch_gating_on = FALSE;
          mm_ptr->chnasn.mode.am0.rev_pwr_cntl_delay_incl = FALSE;
          mm_ptr->chnasn.mode.am0.rl_gain_adj = 0;

          if (P_REV_IS_4_OR_GREATER)
          {
             mm_ptr->chnasn.mode.am0.default_config = ca_ptr->am4.default_config;
          }
          else
          {
             mm_ptr->chnasn.mode.am0.default_config = 0;
          }

          mm_ptr->chnasn.mode.am0.pilot_rec[0].code_chan_fch =
                  ca_ptr->am4.code_chan;
          mm_ptr->chnasn.mode.am0.pilot_rec[0].qof_mask_id_fch = 0;
          mm_ptr->chnasn.mode.am0.pilot_rec[0].add_pilot_rec_incl = 0;

          mm_ptr->chnasn.mode.am0.frame_offset = ca_ptr->am4.frame_offset;
          mm_ptr->chnasn.mode.am0.encrypt_mode = ca_ptr->am4.encrypt_mode;
          mm_ptr->chnasn.mode.am0.band_class = ca_ptr->am4.band_class;
          mm_ptr->chnasn.mode.am0.cdma_freq = ca_ptr->am4.cdma_freq;

          #ifdef FEATURE_IS2000_REL_A
          /* Signaling Encryption Fields */
          mm_ptr->chnasn.mode.am0.d_sig_encrypt_mode = ca_ptr->am4.d_sig_encrypt_mode;
          mm_ptr->chnasn.mode.am0.key_size = ca_ptr->am4.enc_key_size;
          mm_ptr->chnasn.mode.am0.c_sig_encrypt_mode_incl = ca_ptr->am4.c_sig_encrypt_mode_incl;
          mm_ptr->chnasn.mode.am0.c_sig_encrypt_mode = ca_ptr->am4.c_sig_encrypt_mode;
          #endif /* FEATURE_IS2000_REL_A */

          /* Initialize fields which are not used by CAM with defaults */
          mm_ptr->chnasn.mode.am0.num_pilots = 1;
          mm_ptr->chnasn.mode.am0.pilot_rec[0].pilot_pn = cur_bs_ptr->pilot_pn;
          mm_ptr->chnasn.mode.am0.pilot_rec[0].pwr_comb_ind = 0;
          MSG_MED("CAM rec'd, code_chan=%d", ca_ptr->am4.code_chan,0,0);


          #ifdef FEATURE_IS2000_REL_A
          mm_ptr->chnasn.mode.am0.x3_fch_info_incl = FALSE;
          mm_ptr->chnasn.mode.am0.x3_dcch_info_incl = FALSE;
          mm_ptr->chnasn.mode.am0.x3fl_1xrl_incl = FALSE;
          #endif /* FEATURE_IS2000_REL_A */

          #if defined(FEATURE_IS2000_REL_C) || defined(FEATURE_MEID_SUPPORT)
          mm_ptr->chnasn.mode.am0.msg_int_info_incl = FALSE;
          mm_ptr->chnasn.mode.am0.plcm_type_incl = FALSE;
          #endif

          if (store_chnasn_for_tci)
          {
            /* ----------------------------------
            ** Store channel assignment for use
            ** when we go to the traffic channel.
            ** ---------------------------------- */
            cdma.chnasn = mm_ptr->chnasn;
          }

          break;

        case CAI_EXT_PAGING_CHAN_ASSIGN:
          mm_ptr->chnasn.mode.am1.respond = ca_ptr->am5.respond;
          mm_ptr->chnasn.mode.am1.freq_incl = ca_ptr->am5.freq_incl;
          mm_ptr->chnasn.mode.am1.band_class = ca_ptr->am5.band;
          mm_ptr->chnasn.mode.am1.cdma_freq = ca_ptr->am5.cdma_freq;
          mm_ptr->chnasn.mode.am1.num_pilots = ca_ptr->am5.num_pilots;
          for (i=0; i<mm_ptr->chnasn.mode.am1.num_pilots; i++)
          {
            mm_ptr->chnasn.mode.am1.pilot_pn[i] = ca_ptr->am5.pilot_pn[i];
          }
          break;

        default:
          /* --------------------------------------------------
          ** Channel Assignment with illegal assign mode!
          ** -------------------------------------------------- */
          ERR( "Illegal assignment mode received %d",
              msg_ptr->msg.chnasn.chns[i].gen.assign_mode, 0, 0 );
          break;

      } /* end switch (mm_ptr->chnasn.assign_mode) */
    } /* end if (this was a match) */
  } /* end if (this is a channel assignment) */

  /*****************
   * Order Message *
   *****************/

  else if (msg_ptr->msg.gen.msg_type == CAI_PC_ORD_MSG)
  {
    for (i=0; i < msg_ptr->msg.pc_ord.num_ords; i++)
    {
      if (mcc_comp_addr( &msg_ptr->msg.pc_ord.ords[i].gen.hdr,
                         msg_ptr->frm.frame))
      {
        /* -----------------------------------------
        ** Found an order record addressed to mobile
        ** ----------------------------------------- */
        MSG_HIGH( "Rxed PC Order Message", 0, 0, 0 );

        match_found = TRUE;
        break;
      }
    } /* end for */

    if (match_found)
    {
      /* ------------------------------------------
      ** Copy order message over to matched_message
      ** ------------------------------------------ */

      mm_ptr->pc_ord.msg_type = msg_ptr->msg.pc_ord.msg_type;
      mm_ptr->pc_ord.hdr = msg_ptr->msg.pc_ord.ords[i].gen.hdr;
      mm_ptr->pc_ord.order = msg_ptr->msg.pc_ord.ords[i].gen.order;
      mm_ptr->pc_ord.ordq = msg_ptr->msg.pc_ord.ords[i].ordq.ordq;

      if (mm_ptr->pc_ord.order == CAI_BS_CHAL_CONF_ORD)
      {
        mm_ptr->pc_ord.authbs = msg_ptr->msg.pc_ord.ords[i].chal.authbs;
      }
      else if (mm_ptr->pc_ord.order == CAI_RETRY_ORD)
      {
        mm_ptr->pc_ord.retry_type = msg_ptr->msg.pc_ord.ords[i].retry.retry_type;
        mm_ptr->pc_ord.retry_delay = msg_ptr->msg.pc_ord.ords[i].retry.retry_delay;
      }

      #ifdef FEATURE_IS2000_REL_A
      else if ((mm_ptr->pc_ord.order == CAI_REG_ORD) &&
               (mm_ptr->pc_ord.ordq == CAI_REG_ACCEPT_ORDER_ROAM_IND_AND_ENC_INC)
              )
      {
        mm_ptr->pc_ord.roam_indi = msg_ptr->msg.pc_ord.ords[i].regacc.roam_indi;
        mm_ptr->pc_ord.c_sig_encrypt_mode = msg_ptr->msg.pc_ord.ords[i].regacc.c_sig_encrypt_mode;
        mm_ptr->pc_ord.key_size = msg_ptr->msg.pc_ord.ords[i].regacc.key_size;

        #ifdef FEATURE_IS2000_REL_C
        mm_ptr->pc_ord.msg_int_info_incl = msg_ptr->msg.pc_ord.ords[i].regacc.msg_int_info_incl;
        mm_ptr->pc_ord.change_keys = msg_ptr->msg.pc_ord.ords[i].regacc.change_keys;
        mm_ptr->pc_ord.use_uak = msg_ptr->msg.pc_ord.ords[i].regacc.use_uak;
        #endif /* FEATURE_IS2000_REL_C */       
      }
      #endif /* FEATURE_IS2000_REL_A */

      else if (( mm_ptr->pc_ord.order == CAI_REG_ORD ) &&
               ( mm_ptr->pc_ord.ordq == CAI_REG_ACC ) )
      {
         mm_ptr->pc_ord.roam_indi = msg_ptr->msg.pc_ord.ords[i].regacc.roam_indi;
      }
    } /* end if (match_found) */
  }

  /************************************
   * Any Other Paging Channel Message *
   ************************************/

  else
  {
    if (mcc_comp_addr( &msg_ptr->msg.gen_pc.hdr, msg_ptr->frm.frame))
    {
      match_found = TRUE;

      /* ------------------------------------
      ** Copy message over to matched_message
      ** ------------------------------------ */
      switch ( msg_ptr->msg.gen_pc.msg_type )
      {
        case CAI_PC_TMSI_ASGN_MSG:
          mm_ptr->pc_tmsi = msg_ptr->msg.pc_tmsi;
          break;

        case CAI_PC_BURST_MSG:

          #ifdef FEATURE_HDR
#error code not present
          #endif /* FEATURE_HDR */

          mm_ptr->pc_burst = msg_ptr->msg.pc_burst;
          break;

        case CAI_PC_STREQ_MSG:
          mm_ptr->pc_streq = msg_ptr->msg.pc_streq;
          break;

        case CAI_PC_AUTH_MSG:
          mm_ptr->pc_auth = msg_ptr->msg.pc_auth;
          break;

        case CAI_PC_SSD_UP_MSG:
          mm_ptr->pc_ssd = msg_ptr->msg.pc_ssd;
          break;

        case CAI_FEATURE_MSG:
          mm_ptr->pc_feat = msg_ptr->msg.pc_feat;
          break;

        case CAI_SERVICE_REDIR_MSG:
          mm_ptr->serv_redir = msg_ptr->msg.serv_redir;
          break;

        case CAI_PACA_MSG:
          mm_ptr->paca = msg_ptr->msg.paca;
          break;

        #ifdef FEATURE_IS2000_REL_A
        case CAI_SEC_MODE_CMD_MSG:
          mm_ptr->sec_mode_cmd = msg_ptr->msg.smcm;
          break;
        #endif /* FEATURE_IS2000_REL_A */

        #ifdef FEATURE_IS2000_REL_C
        case CAI_AUTH_REQ_MSG:
          mm_ptr->auth_req    = msg_ptr->msg.auth_req;
          /*lint -fallthrough */
        #endif /* FEATURE_IS2000_REL_C */

        default:
          /* ------------------------------------------------
          ** The mobile station may ignore any other messages
          ** ------------------------------------------------ */
          break;

      } /* end switch */
    } /* end if (address matches and not duplicate) */
  }

  return (match_found);
 
} /* mcc_match_msg() */

/*===========================================================================

FUNCTION MCC_PARSE_PM

DESCRIPTION
  This function takes a pointer to a Slotted Page Message or Page Message and
  searches through all the pages in it to see if any are addressed to the
  mobile.  If one is found, information from the page is coppied into a
  general structure to avoid having to parse the page twice.

DEPENDENCIES
  None.

RETURN VALUE
  PAGE_MATCH if a page is found which is addressed to the mobile.
  Otherwise, NO_MATCH.

SIDE EFFECTS
  None.

===========================================================================*/

mccdma_page_match_result_type mcc_parse_pm
(
  mccrxtx_msg_type *msg_ptr,
    /* Pointer to received message */
  mcc_page_info_type *page_info
    /* Structure to return infomation stored in the page */
)
{
  mccdma_page_match_result_type match_found = NO_MATCH;
    /* Indicator that a page match was found */
  caii_page_body_type *page_body_ptr;
    /* Pointer to the body of the page we are checking address of. */
  word num_pages = 0; /* Number of pages in page message */
  word i = 0, j = 0;  /* Indicies */

  if (msg_ptr->msg.gen.msg_type == CAI_SLT_PAGE_MSG)
  {
    num_pages = msg_ptr->msg.slt_page.num_pages;
  }
  else if (msg_ptr->msg.gen.msg_type == CAI_PAGE_MSG)
  {
    num_pages = msg_ptr->msg.page.num_pages;
  }

  /* ------------------------------------------------------------
  ** Search through pages in message looking for matching address
  ** ------------------------------------------------------------ */

  while ((i < num_pages) && (match_found != PAGE_MATCH))
  {
    page_body_ptr = (msg_ptr->msg.gen.msg_type == CAI_SLT_PAGE_MSG) ?
      &msg_ptr->msg.slt_page.pages[i] : &msg_ptr->msg.page.pages[i];

    if (page_body_ptr->ext_addr)
    {
      if ((page_body_ptr->min1 == cdma.imsi_s1) &&
          (page_body_ptr->min2 == cdma.imsi_s2))
      {
        match_found = PAGE_MATCH;
        page_info->msg_seq = page_body_ptr->msg_seq;
        page_info->special_service = page_body_ptr->special_service;
        page_info->service_option = page_body_ptr->service_option;
        page_info->ack_type = 0;
      }
    }
    else
    {
      if (page_body_ptr->min1 == cdma.imsi_s1)
      {
        /* We have no extended address field, but min1 matches.   */
        /* If one of our (sid,nid) pairs matches the current base */
        /* station we can still make a match.                     */

        #ifdef FEATURE_SSPR_ENHANCEMENTS
        for (j=0;j<NV_MAX_HOME_SID_NID;j++)
        #else
        for (j=0;j<NV_MAX_SID_NID;j++)
        #endif

        {
          if ((cur_bs_ptr->csp.sp.sid == cdma.pair[j].sid) &&
              (cur_bs_ptr->csp.sp.nid == cdma.pair[j].nid))
          {
            match_found = PAGE_MATCH;
            page_info->msg_seq = page_body_ptr->msg_seq;
            page_info->special_service = page_body_ptr->special_service;
            page_info->service_option = page_body_ptr->service_option;
            page_info->ack_type = 0;
            break;
          }
        }
      } /* end if (min1's match) */
    } /* end else no extended address */

    i++; /* Look at the next page in the array */
  } /* end while */

  return (match_found);
 
} /* mcc_parse_pm */

/*===========================================================================

FUNCTION MCC_PARSE_GPM

DESCRIPTION
  This function takes a pointer to a General Page Message and searches through
  all the pages in it to see if any are addressed to the mobile.  If one is
  found, information from the page is copied into a general structure
  to avoid having to parse the page twice. Since a mobile specific page match
  has highest priority, if a match is found, all other page records can be
  discarded, if not found, all page records (including broadcast pages if
  supported) need to be checked.

DEPENDENCIES
  None.

RETURN VALUE
  PAGE_MATCH if a page is found which is addressed to the mobile.
  BROADCAST_MATCH if no page is found which is addressed to the mobile but a
    broadcast page is found which passed the filter and is not a duplicate.
  Otherwise, NO_MATCH.

SIDE EFFECTS
  If broadcast paging is supported and broadcast pages are found, the
  BURST_TYPEs and BC_ADDRs fields are copied into mccdma_bc_page_rec[].

===========================================================================*/

mccdma_page_match_result_type mcc_parse_gpm
(
  boolean pending_bcast,
    /* Indicates if it is a pending broadcast page */
  mccrxtx_msg_type *msg_ptr,
    /* Pointer to received message */
  mcc_page_info_type *page_info
    /* Structure to return infomation stored in the page */
)
{
  mccdma_page_match_result_type match_found = NO_MATCH;
    /* Indicator that a page match was found */
  caii_gen_page_rec_type *gen_ptr;
    /* Pointer to the body of the general page we are checking address of. */
  word num_pages;    /* Number of pages in page message */
  word i = 0;        /* Index */

  #ifdef FEATURE_BROADCAST_SMS
  byte  t_bc_page_num=1; /* Broadcast Page number within the GPM */
  #endif /*  FEATURE_BROADCAST_SMS */

  /* In case ESPM or MCCRRM is not received, mcc_entry_init() already
     defaulted cur_bs_ptr->csp.esp.mcc and cur_bs_ptr->csp.esp.imsi_11_12 
     values to MS's MCC and IMSI_11_12. So, safe to reference them for IMSI 
     address match. */

  num_pages = msg_ptr->msg.gen_page.num_pages;

  /* ------------------------------------------------------------
  ** Search through pages in message looking for matching address
  ** ------------------------------------------------------------ */
  while ((i < num_pages) && (match_found != PAGE_MATCH))
  {
    gen_ptr = &msg_ptr->msg.gen_page.gen_page[i];

    /* IMSI_CLASS_0_ADDR_NUM denotes the phone has a class 0 IMSI */
    if (!pending_bcast && (gen_ptr->page_class == CAI_PAGE_CLASS_0) &&  /*lint !e641 */
        (cdma.imsi_addr_num == IMSI_CLASS_0_ADDR_NUM))
    {
      switch (gen_ptr->page_subclass)
      {
        case CAI_PAGE_SUBCLASS_0:
          if ((qw_cmp( gen_ptr->rec.format0.imsi_s, cdma.imsi_s ) == 0) &&
              (cur_bs_ptr->csp.esp.mcc == cdma.mcc) &&
              (cur_bs_ptr->csp.esp.imsi_11_12 == cdma.imsi_11_12))
          {
            match_found = PAGE_MATCH;
            page_info->msg_seq = gen_ptr->rec.format0.msg_seq;
            page_info->special_service = gen_ptr->rec.format0.special_service;
            page_info->service_option = gen_ptr->rec.format0.service_option;
          }
          break;

        case CAI_PAGE_SUBCLASS_1:
          if ((qw_cmp( gen_ptr->rec.format1.imsi_s, cdma.imsi_s ) == 0) &&
              (cur_bs_ptr->csp.esp.mcc == cdma.mcc) &&
              (gen_ptr->rec.format1.imsi_11_12 == cdma.imsi_11_12))
          {
            match_found = PAGE_MATCH;
            page_info->msg_seq = gen_ptr->rec.format1.msg_seq;
            page_info->special_service = gen_ptr->rec.format1.special_service;
            page_info->service_option = gen_ptr->rec.format1.service_option;
          }
          break;

        case CAI_PAGE_SUBCLASS_2:
          if ((qw_cmp( gen_ptr->rec.format2.imsi_s, cdma.imsi_s ) == 0) &&
              (gen_ptr->rec.format2.mcc == cdma.mcc) &&
              (cur_bs_ptr->csp.esp.imsi_11_12 == cdma.imsi_11_12))
          {
            match_found = PAGE_MATCH;
            page_info->msg_seq = gen_ptr->rec.format2.msg_seq;
            page_info->special_service = gen_ptr->rec.format2.special_service;
            page_info->service_option = gen_ptr->rec.format2.service_option;
          }
          break;

        case CAI_PAGE_SUBCLASS_3:
          if ((qw_cmp( gen_ptr->rec.format3.imsi_s, cdma.imsi_s ) == 0) &&
              (gen_ptr->rec.format3.mcc == cdma.mcc) &&
              (gen_ptr->rec.format3.imsi_11_12 == cdma.imsi_11_12))
          {
            match_found = PAGE_MATCH;
            page_info->msg_seq = gen_ptr->rec.format3.msg_seq;
            page_info->special_service = gen_ptr->rec.format3.special_service;
            page_info->service_option = gen_ptr->rec.format3.service_option;
          }
          break;

        default:
          break;
      } /* end switch sub type */

      if (match_found == PAGE_MATCH)
      {
        page_info->ack_type = 2;
      }
    } /* end if class 0 */

    /* IMSI_CLASS_0_ADDR_NUM denotes the phone has a class 0 IMSI */
    else if (!pending_bcast && (gen_ptr->page_class == CAI_PAGE_CLASS_1) &&  /*lint !e641 */
             (cdma.imsi_addr_num != IMSI_CLASS_0_ADDR_NUM))
    {
      switch (gen_ptr->page_subclass)
      {
        case CAI_PAGE_SUBCLASS_0:
          if ((qw_cmp( gen_ptr->rec.format4.imsi_s, cdma.imsi_s ) == 0) &&
              (cur_bs_ptr->csp.esp.mcc == cdma.mcc) &&
              (gen_ptr->rec.format4.imsi_addr_num == cdma.imsi_addr_num) &&
              (gen_ptr->rec.format4.imsi_11_12 == cdma.imsi_11_12))
          {
            match_found = PAGE_MATCH;
            page_info->msg_seq = gen_ptr->rec.format4.msg_seq;
            page_info->special_service = gen_ptr->rec.format4.special_service;
            page_info->service_option = gen_ptr->rec.format4.service_option;
          }
          break;

        case CAI_PAGE_SUBCLASS_1:
          if ((qw_cmp( gen_ptr->rec.format5.imsi_s, cdma.imsi_s ) == 0) &&
              (gen_ptr->rec.format5.mcc == cdma.mcc) &&
              (gen_ptr->rec.format5.imsi_addr_num == cdma.imsi_addr_num) &&
              (gen_ptr->rec.format5.imsi_11_12 == cdma.imsi_11_12))
          {
            match_found = PAGE_MATCH;
            page_info->msg_seq = gen_ptr->rec.format5.msg_seq;
            page_info->special_service = gen_ptr->rec.format5.special_service;
            page_info->service_option = gen_ptr->rec.format5.service_option;
          }
          break;

        default:
          break;
      } /* end switch sub type */

      if (match_found == PAGE_MATCH)
      {
        page_info->ack_type = 2;
      }
    } /* end if class 1 */

    else if (!pending_bcast && (gen_ptr->page_class == CAI_PAGE_CLASS_2))  /*lint !e641 */
    {
      match_found = tmsi_page_match (gen_ptr, page_info);  

      if (match_found == PAGE_MATCH)
      {
        /* ----------------------------------------------------------------
        ** According to 2.1.1.2.1.2 in IS2000 Volume 4. The ack_type in the
        ** page response is set to 3 when page class is 2.
        ** ---------------------------------------------------------------- */
        page_info->ack_type = 3;
      }
    } /* end if class 2 */

    else if (gen_ptr->page_class == CAI_PAGE_CLASS_3)  /*lint !e641 */
    {
       #ifdef FEATURE_BROADCAST_SMS
       if (gen_ptr->page_subclass == CAI_PAGE_SUBCLASS_0) /*lint !e641 */
       {
         MSG_MED("Found BC Page Number #%d", t_bc_page_num, 0, 0);

         /* Fill in broadcast page info for page match */
         qw_equ(mcc_bc_info.frame, msg_ptr->frm.frame);
         mcc_bc_info.bc_type = BROADCAST_PAGE;
         mcc_bc_info.burst_type = gen_ptr->rec.format12.burst_type;
         mcc_bc_info.addr_len = gen_ptr->rec.format12.addr_len;
         memcpy(mcc_bc_info.bc_addr, gen_ptr->rec.format12.bc_addr,
           CAII_BC_ADDR_LEN);
         mcc_bc_info.page.bc.num_bc_page = t_bc_page_num++;

         /* Perform broadcast page match */
         if (mccbcsms_page_match(&mcc_bc_info))
         {
           /* Broadcast page match */
           match_found = BROADCAST_MATCH;
         }
       }
       else
       #endif /* FEATURE_BROADCAST_SMS */

       #ifdef FEATURE_IS2000_REL_A_CC_BCSMS
       if ((gen_ptr->page_subclass == CAI_PAGE_SUBCLASS_3) &&  /*lint !e641 */
           (gen_ptr->page_subclass_ext == CAI_PAGE_SUBCLASS_EXT_0)) /*lint !e641 */
       {
         MSG_MED("Found En BC Page", 0, 0, 0);
         /* Fill in broadcast page info for page match */
         qw_equ(mcc_bc_info.frame, msg_ptr->frm.frame);
         mcc_bc_info.bc_type = ENHANCED_BROADCAST_PAGE;
         mcc_bc_info.burst_type = gen_ptr->rec.format15_0.burst_type;
         mcc_bc_info.addr_len = gen_ptr->rec.format15_0.addr_len;
         memcpy(mcc_bc_info.bc_addr, gen_ptr->rec.format15_0.bc_addr,
           CAII_BC_ADDR_LEN);
         mcc_bc_info.page.enh_bc.bcn = gen_ptr->rec.format15_0.bcn;
         mcc_bc_info.page.enh_bc.time_offset =
           gen_ptr->rec.format15_0.time_offset;
         mcc_bc_info.page.enh_bc.repeat_time_offset =
           gen_ptr->rec.format15_0.repeat_time_offset;

         /* Perform broadcast page match */
         if (mccbcsms_page_match(&mcc_bc_info))
         {
           /* Broadcast page match */
           match_found = BROADCAST_MATCH;
         }
       }
       else
       #endif /* FEATURE_IS2000_REL_A_CC_BCSMS */

       if (!pending_bcast)
       {
         /* there's no other subclasses for this page class */
         #ifdef FEATURE_IS2000_REL_A
         MSG_ERROR("Unknown page class %d subclass %d ext %d",
                    gen_ptr->page_class, gen_ptr->page_subclass,
                    gen_ptr->page_subclass_ext);
         #else
         MSG_ERROR("Unknown page class %d subclass %d",
                    gen_ptr->page_class, gen_ptr->page_subclass, 0);
         #endif /* FEATURE_IS2000_REL_A */
       }

    } /* end if class 3 */

    i++; /* Look at the next page in the array */
  } /* end while */

  return (match_found);
 
} /* mcc_parse_gpm */

#ifdef FEATURE_IS2000_REL_A_CC
/*===========================================================================

FUNCTION MCC_PARSE_UPM

DESCRIPTION
  This function takes a pointer to a Universal Page Records and searches
  through all the pages in it to see if any are addressed to the mobile.  If
  one is found, information from the page is coppied into a general structure
  to avoid having to parse the page twice. Since a mobile specific page match
  has highest priority, if a match is found, all other page records can be
  discarded, if not found, all page records (including broadcast pages if
  supported) need to be checked.
  mccsa.c.

DEPENDENCIES
  For segmented Universal Page Message, all segments must be received.
  and there is partial address match.

RETURN VALUE
  PAGE_MATCH if a page is found which is addressed to the mobile.
  BROADCAST_MATCH if no page is found which is addressed to the mobile but a
    broadcast page is found which passed the filter and is not a duplicate.
  ANNOUNCEMENT_PAGE_MATCH if an announcement page is found which is directed
    to the mobile.
  Otherwise, NO_MATCH.

SIDE EFFECTS
  If broadcast paging is supported and broadcast pages are found, the
  BURST_TYPEs and BC_ADDRs fields are copied into mccdma_bc_page_rec[].

===========================================================================*/

mccdma_page_match_result_type mcc_parse_upm
(
  mccrxtx_msg_type *msg_ptr,
    /* Pointer to received message */
  mcc_page_info_type *page_info
    /* Structure to return infomation stored in the page */
)
{
  mccdma_page_match_result_type match_found = NO_MATCH;
    /* Indicator that a page match was found */
  caii_univ_page_interleaved_addr_type *addr_ptr;
    /* Pointer to partial address information */
  byte *buf_ptr;
    /* Pointer to page block . */
  word len;
    /* Length of the body of page records */
  word *offset;
    /* Pointer to the bit offset to start translation of next page record */
  word num_pages = 0;     /* Number of pages in page message */
  qword imsi_match;       /* IMSI record partial address match result */
  dword imsi_s_33_16 = 0; /* 33rd - 16th bit of IMSI_S */
  word imsi_rec_offset;   /* IMSI record offset */
  qword tmsi_match;       /* TMSI record partial address match result */
  word tmsi_rec_offset;   /* TMSI record offset */
  boolean continue_match = TRUE; /* Indicates if continue to do page match */
  word i = 0;             /* Index */

  /* In case ESPM or MCCRRM is not received, mcc_entry_init() already
     defaulted cur_bs_ptr->csp.esp.mcc and cur_bs_ptr->csp.esp.imsi_11_12 
     values to MS's MCC and IMSI_11_12. So, safe to reference them for IMSI 
     address match. */

  if (msg_ptr->msg.gen.msg_type == CAI_UNIVERSAL_PAGE_MSG)
  {
    #ifdef FEATURE_IS2000_REL_A_CC_BCSMS
    /* Save offset of first page record for use in case of pending broadcast */
    msg_ptr->msg.upm.page_block.first_page_rec_offset =
      msg_ptr->msg.upm.page_block.offset;
    #endif /* FEATURE_IS2000_REL_A_CC_BCSMS */

    addr_ptr = &msg_ptr->msg.upm.interleaved_addr;
    buf_ptr = msg_ptr->msg.upm.page_block.data;
    offset = &msg_ptr->msg.upm.page_block.offset;
    len = msg_ptr->msg.upm.page_block.length;
  }
  else if (msg_ptr->msg.gen.msg_type == CAI_UPM_FINAL_SEG_MSG)
  {

    #ifdef FEATURE_IS2000_REL_A_CC_BCSMS
    /* Save offset of first page record for use in case of pending broadcast */
    mccdma_seg_univ_page.page_block.first_page_rec_offset =
      msg_ptr->msg.upm.page_block.offset;
    #endif /* FEATURE_IS2000_REL_A_CC_BCSMS */

    if (!mccdma_seg_univ_page.valid)
    {
      /* Sanity check, should never be here */
      MSG_ERROR("Seg UPM buffer error", 0, 0, 0);
     return (match_found);
    }
    else
    {
      addr_ptr = &mccdma_seg_univ_page.interleaved_addr;
      buf_ptr = mccdma_seg_univ_page.page_block.data;
      offset = &mccdma_seg_univ_page.page_block.offset;
      len = mccdma_seg_univ_page.page_block.length;
    }
  }
  else
  {
    /* Should never be here */
    MSG_ERROR("Invalid UPM id = %d", msg_ptr->msg.gen.msg_type,
      0, 0);
    return (match_found);
  }

  num_pages = addr_ptr->iaddr_portion_len;
  if (addr_ptr->res_type_included)
  {
    /* Ignore reserved type page records */
    num_pages = num_pages - addr_ptr->num_res_type - 1; /*lint !e834 */
  }

  if (addr_ptr->bcast_included)
  {
    imsi_rec_offset = addr_ptr->num_bcast + 1;
  }
  else
  {
    imsi_rec_offset = 0;
  }
  qw_equ(imsi_match, addr_ptr->imsi_match);
  imsi_s_33_16 = (dword) (((qw_hi(cdma.imsi_s) << 30) >> 14) |
                           (qw_lo(cdma.imsi_s) >> 16));

  if (addr_ptr->imsi_included)
  {
    tmsi_rec_offset = imsi_rec_offset + addr_ptr->num_imsi + 1;
  }
  else
  {
    tmsi_rec_offset = imsi_rec_offset;
  }
  qw_equ(tmsi_match, addr_ptr->tmsi_match);

  /* ------------------------------------------------------
  ** Search through pages in message looking for page match
  ** ------------------------------------------------------ */
  while (continue_match  && (i < num_pages) &&
         (*offset + CAI_UNIV_PAGE_VAR_MIN <= len))
  {
    if (xlate_ext_univ_page_record(buf_ptr, offset, &mcc_upm_info)
        == CAIX_DONE_S)
    {
      /* Enhanced Broadcast page records */
      if (addr_ptr->bcast_included && (i <= addr_ptr->num_bcast))
      {

        #ifdef FEATURE_IS2000_REL_A_CC_BCSMS
        if (addr_ptr->bcast_match)
        {
          /* Perform broadcast page match */

          if ((mcc_upm_info.page_class == CAI_PAGE_CLASS_3) &&  /*lint !e641 */
              (mcc_upm_info.page_subclass == CAI_PAGE_SUBCLASS_3) && /*lint !e641 */
              (mcc_upm_info.page_subclass_ext == CAI_PAGE_SUBCLASS_EXT_0)) /*lint !e641 */
          {
            /* Fill in broadcast page info for page match */
            qw_equ(mcc_bc_info.frame, msg_ptr->frm.frame);
            mcc_bc_info.bc_type = ENHANCED_BROADCAST_PAGE;
            mcc_bc_info.burst_type = addr_ptr->burst_type[i];
            mcc_bc_info.addr_len = mcc_upm_info.type.fmt15_0.addr_len;
            mcc_bc_info.bc_addr[0] =
              mcc_upm_info.type.fmt15_0.bc_addr_remainder[0];
            mcc_bc_info.bc_addr[1] =
              mcc_upm_info.type.fmt15_0.bc_addr_remainder[1];
            mcc_bc_info.bc_addr[2] = addr_ptr->bc_addr[i][0];
            mcc_bc_info.bc_addr[3] = addr_ptr->bc_addr[i][1];
            mcc_bc_info.bc_addr[4] =
              mcc_upm_info.type.fmt15_0.bc_addr_remainder[2];
            mcc_bc_info.page.enh_bc.bcn = mcc_upm_info.type.fmt15_0.bcn;
            mcc_bc_info.page.enh_bc.time_offset =
              mcc_upm_info.type.fmt15_0.time_offset;
            mcc_bc_info.page.enh_bc.repeat_time_offset =
              mcc_upm_info.type.fmt15_0.repeat_time_offset;

            /* Perform broadcast page match */
            if (mccbcsms_page_match(&mcc_bc_info))
            {
              /* Broadcast page match */
              match_found = BROADCAST_MATCH;
            }
          }
        }
        #endif /* FEATURE_IS2000_REL_A_CC_BCSMS */

        if ((i == addr_ptr->num_bcast) &&
            (qw_hi(addr_ptr->imsi_match) == 0) &&
            (qw_lo(addr_ptr->imsi_match) == 0)
            && (qw_hi(addr_ptr->tmsi_match) == 0)
            && (qw_lo(addr_ptr->tmsi_match) == 0)
           )
        {
          /* ---------------------------------------------------------
          ** Last broadcast page record, IMSI partial address mismatch
          ** and TMSI partial address mismatch, done with page match
          ** operation.
          ** --------------------------------------------------------- */
          continue_match = FALSE;
        }
      }

      /* IMSI page records */
      else if (addr_ptr->imsi_included &&
               (i <= imsi_rec_offset+addr_ptr->num_imsi))
      {
        if (qw_hi(imsi_match) & (dword) (0x80000000))
        {
          /* Partial IMSI address match for this IMSI record */

          /* IMSI_CLASS_0_ADDR_NUM denotes the phone has a class 0 IMSI */
          if ((mcc_upm_info.page_class == CAI_PAGE_CLASS_0) && /*lint !e641 */
              (cdma.imsi_addr_num == IMSI_CLASS_0_ADDR_NUM))
          {
            switch (mcc_upm_info.page_subclass)
            {
              case CAI_PAGE_SUBCLASS_0:
                if ( (mcc_upm_info.type.fmt0.imsi_s_33_16 == imsi_s_33_16) &&
                     (cur_bs_ptr->csp.esp.mcc == cdma.mcc ) &&
                     (cur_bs_ptr->csp.esp.imsi_11_12 == cdma.imsi_11_12 ) )
                {
                  match_found = PAGE_MATCH;
                  page_info->msg_seq = mcc_upm_info.type.fmt0.msg_seq;
                  page_info->special_service = TRUE;
                  page_info->service_option =
                    mcc_upm_info.type.fmt0.service_option;
                }
                break;

              case CAI_PAGE_SUBCLASS_1:
                if ( (mcc_upm_info.type.fmt1.imsi_s_33_16 == imsi_s_33_16) &&
                     (cur_bs_ptr->csp.esp.mcc == cdma.mcc ) && 
                     (mcc_upm_info.type.fmt1.imsi_11_12 == cdma.imsi_11_12) )
                {
                  match_found = PAGE_MATCH;
                  page_info->msg_seq = mcc_upm_info.type.fmt1.msg_seq;
                  page_info->special_service = TRUE;
                  page_info->service_option =
                    mcc_upm_info.type.fmt1.service_option;
                }
                break;

              case CAI_PAGE_SUBCLASS_2:
                if ( (mcc_upm_info.type.fmt2.imsi_s_33_16 == imsi_s_33_16) &&
                     (mcc_upm_info.type.fmt2.mcc == cdma.mcc) &&
                     (cur_bs_ptr->csp.esp.imsi_11_12 == cdma.imsi_11_12 ) )
                {
                  match_found = PAGE_MATCH;
                  page_info->msg_seq = mcc_upm_info.type.fmt2.msg_seq;
                  page_info->special_service = TRUE;
                  page_info->service_option =
                    mcc_upm_info.type.fmt2.service_option;
                }
                break;

              case CAI_PAGE_SUBCLASS_3:
                if ( (mcc_upm_info.type.fmt3.imsi_s_33_16 == imsi_s_33_16) &&
                     (mcc_upm_info.type.fmt3.mcc == cdma.mcc) &&
                     (mcc_upm_info.type.fmt3.imsi_11_12 == cdma.imsi_11_12) )
                {
                  match_found = PAGE_MATCH;
                  page_info->msg_seq = mcc_upm_info.type.fmt3.msg_seq;
                  page_info->special_service = TRUE;
                  page_info->service_option =
                    mcc_upm_info.type.fmt3.service_option;
                }
                break;

              default:
                break;
            } /* end switch sub type */

            if (match_found == PAGE_MATCH)
            {
              /* ----------------------------------------------------------
              ** According to 2.1.1.2.1.2 in IS2000 Volume 4.  The ack_type
              ** in the page response is set to 2 when page class is 0.
              ** ---------------------------------------------------------- */
              page_info->ack_type = 2;
            }
          } /* end if class 0 */

          /* IMSI_CLASS_1_ADDR_NUM denotes the phone has a class 1 IMSI */
          else if ((mcc_upm_info.page_class == CAI_PAGE_CLASS_1) && /*lint !e641 */
                   (cdma.imsi_addr_num != IMSI_CLASS_0_ADDR_NUM))
          {
            switch (mcc_upm_info.page_subclass)
            {
              case CAI_PAGE_SUBCLASS_0:
                if ((mcc_upm_info.type.fmt4.imsi_s_33_16 == imsi_s_33_16)
                    && (mcc_upm_info.type.fmt4.imsi_addr_num ==
                        cdma.imsi_addr_num)
                    && (mcc_upm_info.type.fmt4.imsi_11_12 == cdma.imsi_11_12))
                {
                  match_found = PAGE_MATCH;
                  page_info->msg_seq = mcc_upm_info.type.fmt4.msg_seq;
                  page_info->special_service = TRUE;
                  page_info->service_option =
                  mcc_upm_info.type.fmt4.service_option;
                }
                break;

              case CAI_PAGE_SUBCLASS_1:
                if ((mcc_upm_info.type.fmt5.imsi_s_33_16 == imsi_s_33_16)
                    && (mcc_upm_info.type.fmt5.mcc == cdma.mcc)
                    && (mcc_upm_info.type.fmt5.imsi_addr_num ==
                        cdma.imsi_addr_num)
                    && (mcc_upm_info.type.fmt5.imsi_11_12 == cdma.imsi_11_12))
                {
                  match_found = PAGE_MATCH;
                  page_info->msg_seq = mcc_upm_info.type.fmt5.msg_seq;
                  page_info->special_service = TRUE;
                  page_info->service_option =
                    mcc_upm_info.type.fmt5.service_option;
                }
                break;

              default:
                break;
            } /* end switch sub type */

            if (match_found == PAGE_MATCH)
            {
              /* ----------------------------------------------------------
              ** According to 2.1.1.2.1.2 in IS2000 Volume 4.  The ack_type
              ** in the page response is set to 2 when page class is 1.
              ** ---------------------------------------------------------- */
              page_info->ack_type = 2;
            }
          } /* end if class 1 */
          else if ((mcc_upm_info.page_class == CAI_PAGE_CLASS_3) && /*lint !e641 */
                   (mcc_upm_info.page_subclass == CAI_PAGE_SUBCLASS_3) &&  /*lint !e641 */
                   (mcc_upm_info.page_subclass_ext == CAI_PAGE_SUBCLASS_EXT_1))  /*lint !e641 */
          {
            if (match_found == NO_MATCH)
            {
              /* Make sure announcement page match never override other page
                 match */
              match_found = ANNOUNCEMENT_MATCH;
            }
          } /* end if class 3 */
        }
        /* Left shift IMSI partial address match result by 1 bit to
           prepare for next record page match operation */
        qw_shift(imsi_match, 1);

        if ((qw_hi(imsi_match) == 0) && (qw_lo(imsi_match) == 0))
        {
          /* No more IMSI record with partial address match */
          if ((qw_hi(addr_ptr->tmsi_match) == 0) &&
              (qw_lo(addr_ptr->tmsi_match) == 0))
          {
            /* TMSI partial address mismatch, done with page match operation */
            continue_match = FALSE;
          }
        }
      }

      /* TMSI page records */
      else if (addr_ptr->tmsi_included &&
               (i <= tmsi_rec_offset+addr_ptr->num_tmsi))
      {
        if (qw_hi(tmsi_match) & (dword) (0x80000000))
        {
          /* Partial IMSI address match for this TMSI record */

          if ((mcc_upm_info.page_class == CAI_PAGE_CLASS_2) ||  /*lint !e641 */
              (mcc_upm_info.page_class == CAI_PAGE_CLASS_3))  /*lint !e641 */
          {
            mccdma_page_match_result_type old_match_found = match_found;

            match_found = tmsi_upm_page_match(&mcc_upm_info, page_info);
            
            if ((match_found == ANNOUNCEMENT_MATCH) && 
                (old_match_found != NO_MATCH))
            {
              /* Make sure announcement page match never override other 
                 page match */
              match_found = old_match_found;
            }

            if (match_found == PAGE_MATCH)
            {
              /* ---------------------------------------------------------
              ** According to 2.1.1.2.1.2 in IS2000 Volume 4. The ack_type
              ** in the page response is set to 3 when page class is 2.
              ** --------------------------------------------------------- */
              page_info->ack_type = 3;
            }
          } /* end if class 2 or 3 */
        }

        /* Left shift TMSI partial address match result by 1 bit to
           prepare for next record page match operation */
        qw_shift(tmsi_match, 1);

        if ((qw_hi(tmsi_match) == 0) && (qw_lo(tmsi_match) == 0))
        {
          /* No more TMSI record with partial address match, done with page
             match operation */
          continue_match = FALSE;
        }
      }
      if (match_found == PAGE_MATCH)
      {
        /* Done with page match operation */
        continue_match = FALSE;
      }
      else
      {
        i++; /* Look at the next page in the array */
      }
    }
    else
    {
      /* Translation error, stop process the message */
      match_found = NO_MATCH;
      continue_match = FALSE;
    }
  } /* end while */

  if (match_found == ANNOUNCEMENT_MATCH)
  {
    MSG_MED("Announcement match", 0, 0, 0);
  }

  return (match_found);
 
} /* mcc_parse_upm */
#endif /* FEATURE_IS2000_REL_A_CC */

/*===========================================================================

FUNCTION MCC_PARSE_PAGE

DESCRIPTION
  This function takes a pointer to a page message and searches through all
  the pages in it to see if any are addressed to the mobile.  If one is
  found, information from the page is coppied into a general structure
  to avoid having to parse the page twice. Since a mobile specific page match
  has highest priority, if a match is found, all other page records can be
  discarded, if not found, all page records (including broadcast pages if
  supported) need to be checked.  This function is called from mccidl.c and
  mccsa.c.

DEPENDENCIES
  None.

RETURN VALUE
  PAGE_MATCH if a page is found which is addressed to the mobile.
  BROADCAST_MATCH if no page is found which is addressed to the mobile but a
    broadcast page is found which passed the filter and is not a duplicate.
  Otherwise, NO_MATCH.

SIDE EFFECTS
  Stores the new config and access message sequences.

===========================================================================*/

mccdma_page_match_result_type mcc_parse_page
(
  word new_state,
    /* Next state to be processed */
  boolean pending_bcast,
    /* Indicates if it is a pending broadcast page */
  mccrxtx_msg_type *msg_ptr,
    /* Pointer to received message */
  mcc_page_info_type *page_info
    /* Structure to return infomation stored in the page */
)
{
  mccdma_page_match_result_type match_found = NO_MATCH;
    /* Indicator that a page match was found */
  caii_pc_hdr_type msg_hdr;
    /* Fake message header so we can use mcc_detect_duplicate() */

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  if (msg_ptr->msg.gen.msg_type == CAI_GENERAL_PAGE_MSG)
  {
    match_found = mcc_parse_gpm(pending_bcast, msg_ptr, page_info);
  }
  else if ((msg_ptr->msg.gen.msg_type == CAI_SLT_PAGE_MSG) ||
           (msg_ptr->msg.gen.msg_type == CAI_PAGE_MSG))
  {
    match_found = mcc_parse_pm(msg_ptr, page_info);
  }
  #ifdef FEATURE_IS2000_REL_A_CC
  else if ((msg_ptr->msg.gen.msg_type == CAI_UNIVERSAL_PAGE_MSG) ||
           (msg_ptr->msg.gen.msg_type == CAI_UPM_FINAL_SEG_MSG))
  {
    match_found = mcc_parse_upm(msg_ptr, page_info);
  }
  #endif /* FEATURE_IS2000_REL_A_CC */

  else
  {
    /* Shall never be here */
    MSG_ERROR("Invalid page message %", msg_ptr->msg.gen.msg_type, 0, 0);
    return match_found;
  }

  #ifdef FEATURE_BROADCAST_SMS
  if (!mccbcsms_is_bcast_init() && (match_found == BROADCAST_MATCH))
  {
    /* if bc_index not initialized and we found a (e)bc rec, we need to store
     * the Page Message for later process */
    mccidl_save_bcast_page(msg_ptr);
    match_found = NO_MATCH;
  }
  #endif /* FEATURE_BROADCAST_SMS */

  /* ------------------------------------------------------------------
  ** Only need to check message sequence numbers if it is not a pending
  ** broadcast page and the main state is idle
  ** ------------------------------------------------------------------ */
  if ((MAIN_STATE(new_state) == CDMA_IDLE) && !pending_bcast)
  {
    /* Update message sequence */
    if (msg_ptr->msg.gen.msg_type == CAI_SLT_PAGE_MSG)
    {
      mcc_put_seq( msg_ptr->msg.slt_page.config_msg_seq, FALSE );
      mcc_update_acc_msg_seq(msg_ptr->msg.slt_page.acc_msg_seq);
    }
    else if (msg_ptr->msg.gen.msg_type == CAI_PAGE_MSG)
    {
      mcc_put_seq(msg_ptr->msg.page.config_msg_seq, FALSE);
      mcc_update_acc_msg_seq(msg_ptr->msg.page.acc_msg_seq);
    }
    else if (msg_ptr->msg.gen.msg_type == CAI_GENERAL_PAGE_MSG)
    {
      mcc_put_seq(msg_ptr->msg.gen_page.config_msg_seq, FALSE);
      mcc_update_acc_msg_seq(msg_ptr->msg.gen_page.acc_msg_seq);
    }
    #ifdef FEATURE_IS2000_REL_A_CC
    else if (msg_ptr->msg.gen.msg_type == CAI_UNIVERSAL_PAGE_MSG)
    {
      mcc_put_seq(msg_ptr->msg.upm.common.config_msg_seq, FALSE);
      mcc_update_acc_msg_seq(msg_ptr->msg.upm.common.acc_msg_seq);
    }
    else if (msg_ptr->msg.gen.msg_type == CAI_UPM_FINAL_SEG_MSG)
    {
      mcc_put_seq(mccdma_seg_univ_page.common.config_msg_seq, FALSE);
      mcc_update_acc_msg_seq(mccdma_seg_univ_page.common.acc_msg_seq);
    }
    #endif /* FEATURE_IS2000_REL_A_CC */
  }

  #ifdef FEATURE_HDR
#error code not present
  #endif /* FEATURE_DDTM_CNTL */

  if (match_found == PAGE_MATCH)
  {
    MCC_CHK_NULL_PTR(page_info)
    /* -----------------------------------------------------------------
    ** Make sure this is not a duplicate.  For page messages the only
    ** distinguishing factor for duplicate detection is the msg_seq.
    ** The ack_req field for page messages is implied to always be true.
    ** ----------------------------------------------------------------- */
    msg_hdr.ack_req = TRUE;
    msg_hdr.msg_seq = page_info->msg_seq;

    if (mcc_detect_duplicate( TRUE, &msg_hdr ))
    {
      /* If it's a duplicate, report that no page was found */
      match_found = NO_MATCH;
    }
    else
    {
      /* On MSM6500, we can let the 1X page propagate up to Data     */
      /* Services and DS will handle it. Therefore it can be removed.*/

//       #ifdef FEATURE_HDR_HYBRID_TEMP_MC
//       if( page_info->special_service                       &&
//           ds_is_srv_opt_ppp_pkt(page_info->service_option) &&
//           ds_is_hdr_call_active() )
//       {
//         /* HDR RLP is active and mobile receives a data page:
//            Since new PPP session has already been setup in HDR,
//            we should not use the old PPP session in 1X so
//            silently discarding the 1X data page. */
//
//         MSG_HIGH("DiscardDataPage-HdrRlpActive",0,0,0);
//         MSG_HIGH("Page type=%d SO=0x%x",
//                  page_typemsg_ptr->msg.gen.msg_type,
//                  page_info->service_option, 0);
//         match_found = FALSE;
//       }
//       else
//       #endif /* FEATURE_HDR_HYBRID */
      {
        MSG_HIGH("Rxed page msg type %d", msg_ptr->msg.gen.msg_type, 0, 0 );
        MSG_MED( "Special = %d  SO = 0x%x", page_info->special_service,
                page_info->service_option, 0 );
      }
    }
  }

  return (match_found);
 
} /* end mcc_parse_page() */

/*===========================================================================

FUNCTION MCC_VALIDATE_SO_REQ

DESCRIPTION
  Evaluate the service option paged by the base station based on the setting
  of FEATURE_ANSWER_VOICE_AS_DATA and FEATURE_EVRC_SO_MGMT, converting the SO
  if necessary.  The resulting SO is then evaluated and the resulting SO set
  cdma.so_req.

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  Sets value of cdma.so_req.

===========================================================================*/

#ifdef FEATURE_DS
static word voice_as_data_so_list[] =
{
  CAI_SO_VOICE_IS96A,
  CAI_SO_VOICE_EVRC

  #ifndef FEATURE_NO_13K_VOICE_SUPPORT
  ,CAI_SO_VOICE_13K_IS733
  ,CAI_SO_VOICE_13K
  #endif

  #ifdef FEATURE_SO_VOICE_SMV
  ,CAI_SO_VOICE_SMV
  #endif

  #ifdef FEATURE_VOC_4GV
  ,CAI_SO_VOICE_4GV_NB
  ,CAI_SO_VOICE_4GV_WB
  #endif /* FEATURE_VOC_4GV */ 

};
#endif // FEATURE_DS

void mcc_validate_so_req
(
  mcc_page_info_type *page_info
    /* Pointer to the body of the matched page */
)
{
  word paged_so;

  // First identify service option we are actually paged with
  if ( !page_info->special_service )
  {
    paged_so = CAI_SO_VOICE_IS96A;
  }
  else
  {
    paged_so = page_info->service_option;
  }

  // Second, if Prearrangement feature is enabled, see if we need to map the SO.
  #ifdef FEATURE_DS
  /*
   * This is only done when prearrangement feature (aka "answer voice as data")
   * is turned on.  If we are paged with any voice SO, depending on the value
   * of db item "DB_VOICE_AS_DATA", we may respond with a different SO (fax or
   * modem data).  This is what it does:
   *
   * Check what db item "DB_VOICE_AS_DATA" is set to.  If it is
   * "DB_VOICE_AS_DATA_NEVER", it'll just do the usual thing, i.e. respond with
   * the paged SO. If it is "DB_VOICE_AS_FAX_ONCE" or "DB_VOICE_AS_FAX_ALWAYS",
   * respond with SO 13 (CAI_SO_G3_FAX_13K_PRE707).
   *
   * If it is "DB_VOICE_AS_MODEM_ONCE" or "DB_VOICE_AS_MODEM_ALWAYS", respond
   * with SO 12 (CAI_SO_ASYNC_DATA_13K_PRE707).
   *
   * UI will take care of the "always" vs. "once" varieties, i.e. when it is
   * once, UI will expire it after responding to 1 successful page.  This
   * implies that only after the call has been successfully connected will the
   * "once" mode be reverted back to "never", which is the desirable behavior.
   *
   * If it is any other value (should never happen), an error message will be
   * emitted and it'll be treated the same as "DB_VOICE_AS_DATA_NEVER".
   */
  {
    db_items_value_type dbi;
    db_get(DB_VOICE_AS_DATA,&dbi);

    if (dbi.voice_as_data != DB_VOICE_AS_DATA_NEVER)
    {
      byte i;
      for (i = 0; i < sizeof(voice_as_data_so_list)/sizeof(word); i++)
      {
        /* If SO is a voice SO, then do something */
        if (paged_so == voice_as_data_so_list[i])
        {
          switch (dbi.voice_as_data)
          {
            case DB_VOICE_AS_FAX_ONCE:         /* Answer voice as fax once   */
            case DB_VOICE_AS_FAX_ALWAYS:       /* Always answer voice as fax */
              MSG_HIGH( "SO forced from %x to Group 3 FAX 13k (13)",
                        cdma.so_req, 0, 0 );
              paged_so = CAI_SO_G3_FAX_13K_PRE707;
              break;

            case DB_VOICE_AS_MODEM_ONCE:     /* Answer voice as modem once   */
            case DB_VOICE_AS_MODEM_ALWAYS:   /* Always answer voice as modem */
              MSG_HIGH( "SO forced from %x to Async. data 13k (12)",
                        cdma.so_req, 0, 0 );
              paged_so = CAI_SO_ASYNC_DATA_13K_PRE707;
              break;

            default:               /* This is an error.  Don't change the SO */
              ERR( "Unknown VOICE_AS_DATA mode: %d", dbi.voice_as_data, 0, 0 );
              break;
          }

          // Since we found the paged_so in our voice SO list, we can break here.
          break;
        } // if (paged_so == voice_as_data_so_list[i])
      } // for (i = 0; i < sizeof(voice_as_data_so_list)/sizeof(word); i++)
    } // if (dbi.voice_as_data != DB_VOICE_AS_DATA_NEVER)
  }
  #endif // FEATURE_DS

  #ifdef FEATURE_PASSPORT_MODE
  if(mcc_passport_mode_is_jcdma())
  {
  
    // For JCDMA, KDDI Specification requires to first accept unsupported data
    // service options (instead of rejecting by setting SO=0 in page
    // response message) and then later release the call after setting up the
    // traffic channel. As a result, we do not to get the page_response_so
    // for JCDMA.
    if(paged_so == CAI_SO_PPP_PKT_DATA_13K_PROPTRY ||
      paged_so == CAI_SO_PPP_PKT_DATA_PRE707)
    {
      snm_enable_so( paged_so);
    }
    else
    {
      // Third, do normal SO mapping
      paged_so = snm_get_page_response_so( paged_so );
    }
  }
  else
  #endif
  {  
    // Third, do normal SO mapping
    paged_so = snm_get_page_response_so( paged_so );
  }
  

  // Fourth, if EVRC SO management feature is turned, do the SO conversion if
  // necessary.  Note that if prearrangement is active, we do not have to do this
  // because the voice SO will be mapped to a data SO.  But with the featurization
  // of FEATURE_DS_VOICE_AS_DATA and FEATURE_EVRC_SO_MGMT, it is more readable
  // and easier to just do this blocked here regardless of what prearrangement
  // has done.  If prearrangement did happen, the SO conversion here won't
  // happen anyway.
  #ifdef FEATURE_EVRC_SO_MGMT
  {
    db_items_value_type item;

    db_get (DB_ROAM, &item);

    /* With modified service negotiation for EVRC we would like to propose EVRC
       in the page response only if all the following conditions are TRUE

       a. EVRC capability is enabled
       b. MS is not roaming
       c. Vocoder is EVRC capable
       d. Paged SO matches the Preferred Home Page SO or the SO specified
          in the home page SO is a wildcard
       e: One of the voice SOs is requested   */

    if (((mcc_pref_voice_so.evrc_capability_enabled) &&
         (item.roam == DB_ROAM_OFF) &&
      #ifdef FEATURE_MVS_MIGRATE
#error code not present
      #else
         (voc_capability_supported(VOC_CAP_IS127))) &&
      #endif /* FEATURE_MVS_MIGRATE */
         ((paged_so == mcc_pref_voice_so.home_page_voice_so) ||
           (mcc_pref_voice_so.home_page_voice_so == CAI_SO_VOICE_WILDCARD)) &&
         ((paged_so == CAI_SO_VOICE_IS96A) ||
          (paged_so == CAI_SO_VOICE_EVRC)
          #ifndef FEATURE_NO_13K_VOICE_SUPPORT
          || (paged_so == CAI_SO_VOICE_13K_IS733)
          || (paged_so == CAI_SO_VOICE_13K)
          #endif

          /*
           *  When the BS pages the mobile with SMV service option,
           *  it is likely that it is doing so to increase capacity.
           *  Maybe the cell has become quite loaded with voice
           *  calls and to accomodate every one, it has to reduce
           *  the quality a little bit. And hence it might be proposing
           *  SMV.
           *
           *  Thus if BS pages with SMV and we support SMV, do not change
           *  the SO in the page response.
           *
           *  However if the MS does not support SMV, then the SO in
           *  page response should be changed.
           *
          */

          #ifndef FEATURE_SO_VOICE_SMV
          || (paged_so == CAI_SO_VOICE_SMV)
          #endif
           /*
           *  When the BS pages the mobile with 4GV service option,
           *  it is likely that it is doing so to increase capacity.
           *  Maybe the cell has become quite loaded with voice
           *  calls and to accomodate every one, it has to reduce
           *  the quality a little bit. And hence it might be proposing
           *  4GV.
           *
           *  Thus if BS pages with 4GV and we support 4GV, do not change
           *  the SO in the page response.
           *
           *  However if the MS does not support 4GV, then the SO in
           *  page response should be changed.
           *
          */
          #ifndef FEATURE_VOC_4GV
          || ((paged_so == CAI_SO_VOICE_4GV_NB) ||
              (paged_so == CAI_SO_VOICE_4GV_WB))
          #endif /* FEATURE_VOC_4GV */

          ))
    {
      paged_so = CAI_SO_VOICE_EVRC;
    }
  }
  #endif

  // Finally evaluate it and see if an alternative has to be proposed.  If the
  // the SO is support by the mobile, snm_eval_paged_so returns CAI_SO_REJ.
  // This function call is required whenever the mobile gets paged;
  // otherwise, SNM will not be initialized properly.
  cdma.so_req = snm_eval_paged_so( paged_so );
 
} /* end mcc_validate_so_req() */

/*===========================================================================

FUNCTION MCC_IDLE_HO_RX

DESCRIPTION
  This procedure clears messages received from the current base station and
  sends a request to the receive task to perform idle handoff processing.

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  mcc_rxtx_q will be emptied.

===========================================================================*/

void mcc_idle_ho_rx( void )
{
  mccrxtx_cmd_type *rx_ptr;
    /* pointer to received messages */

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  /* report to DIAG that a significant event is about to take place */
  diag_inc_event_cnt();

  /* send command to RXC to perform Idle Handoff processing */
  mcc_rx_buf.idle_ho.hdr.command = RXC_IDLE_HO_F;

  mcc_rxc_cmd(&mcc_rx_buf);

  /* Remove any messages remaining from current base station */
  while ((rx_ptr = (mccrxtx_cmd_type*) q_get( &mcc_rxtx_q )) != NULL)
  {
    q_put( &mcc_rxtx_free_q, &rx_ptr->hdr.cmd_hdr.link );
  }

} /* mcc_idle_ho_rx */

/*===========================================================================

FUNCTION MCC_SEND_SRCH_ASSIGN

DESCRIPTION
  This procedure sends the Search task a list of pilot pn's and a cdma
  frequency and a band class for a channel assignment.

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  Tunes to a new CDMA frequency if one is specified in the paging channel
  assignment message.

===========================================================================*/

void mcc_send_srch_assign
(
  mccdma_chnasn_type *pc_assign_msg_ptr
    /* Message addressed to this mobile which specifies a Paging Channel
       assignment (ASSIGN_MODE equal to '001' or '101') */
)
{
  word i; /* loop counter */

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  if (pc_assign_msg_ptr->mode.am1.freq_incl)
  {
    /* For dual mode PCS, check band class and switch bands here */
    cdma.cdmach = pc_assign_msg_ptr->mode.am1.cdma_freq;

    MSG_MED( "Changing to new CDMA frequency %d", cdma.cdmach, 0, 0);

    if (((pc_assign_msg_ptr->msg_type == CAI_CHN_ASN_MSG) &&
       (pc_assign_msg_ptr->assign_mode == CAI_EXT_PAGING_CHAN_ASSIGN))
        || (pc_assign_msg_ptr->msg_type == CAI_EXT_CHN_ASN_MSG)
       )
    {
      /* Extended Paging Channel Assignment may specify a different band than
         the current one that we are in */
      MSG_MED( "Band Class %d",pc_assign_msg_ptr->mode.am1.band_class, 0, 0);

    }

    cdma.band_class = pc_assign_msg_ptr->mode.am1.band_class;

    /* Update the freq block/serv-sys */
    cdma.block_or_sys = (uint16) sdprl_map_chan_to_blksys(SD_MODE_CDMA, 
                                                          (sd_band_e_type) cdma.band_class, cdma.cdmach); 
      // func is returing enum (32 bits) into 16 bit field, 
      // but values are low enough to fit without any problem.
  }

  mcc_srch_buf.pc_assign.hdr.cmd = SRCH_PC_ASSIGN_F;
  mcc_srch_buf.pc_assign.cdma_freq = cdma.cdmach;
  mcc_srch_buf.pc_assign.band_class = cdma.band_class;
  mcc_srch_buf.pc_assign.assign_set_cnt =
    pc_assign_msg_ptr->mode.am1.num_pilots ;

  for (i = 0; i < mcc_srch_buf.pc_assign.assign_set_cnt; i++)
  {
    mcc_srch_buf.pc_assign.assign_set[i].pilot =
      (int2) pc_assign_msg_ptr->mode.am1.pilot_pn[i];
    MSG_MED("PC_ASSIGN pilot[%d]=%d",
            i, pc_assign_msg_ptr->mode.am1.pilot_pn[i],0);
  }

  mcc_srch_cmd( &mcc_srch_buf );
 
}/* mcc_send_srch_assign */

/*===========================================================================

FUNCTION MCC_PC_ASSIGN

DESCRIPTION
  This procedure does the required processing for a paging channel assignment
  (mode '001') or ext'd channel assignment from Idle or System Access state
  by a channel assignment or extended channel assignment msg.

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/
/*lint -esym(715, new_state) */
void mcc_pc_assign
(
  word new_state,
    /* Next state to be processed */
  mccdma_chnasn_type *pc_assign_msg_ptr
    /* Message addressed to this mobile which specifies a Paging Channel
       assignment (ASSIGN_MODE equal to '001') */
)
{
  mccsrch_rpt_type *rpt_ptr;
    /* Report received from searcher */
  mccrxtx_cmd_type *rx_ptr;
    /* Pointer to received message */
  dword sigs;
    /* Return signals from mcc_wait() call */
  boolean got_searcher_report = FALSE;
    /* Flag to tell we got the desiered report from searcher */
  bs_info_id_type bs_info_id;
    /* Fields uniquely identify BS */
  db_items_value_type dbi;
    /* DB items holder */

  #ifdef FEATURE_IS2000_REL_A_CC
  caix_chan_type old_ovhd_chan = cur_bs_ptr->ovhd_chan_info.ovhd_chan;
    /* Type of overhead channel for old BS */
  #endif /* FEATURE_IS2000_REL_A_CC */

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  /* ------------------------------------------------------
  ** Change channel if necessary and send searcher to find
  ** strongest pilot in pilot_pn list given in the message.
  ** ------------------------------------------------------ */

  MSG_MED( "CAM or ECAM mode '001' or '101'", 0, 0, 0);

  mcc_send_srch_assign( pc_assign_msg_ptr );

  /* --------------------------------------------------------------
  ** In the event of a bizarre race condition, searcher could have
  ** already put a different report on report queue than expected.
  ** We wait until we get the right report, forever if necessary.
  ** -------------------------------------------------------------- */
  do
  {
    sigs = mcc_wait( MCC_SRCH_Q_SIG | MCC_RXTX_Q_SIG );

    if ((sigs & MCC_RXTX_Q_SIG) != 0)
    {
      /* ----------------------------------------------------------
      ** We have been directed to another channel so no more useful
      ** messages should come on this paging channel.  To avoid
      ** filling the RXTX queue while we wait for searcher, we will
      ** throw them out.
      ** ---------------------------------------------------------- */
      if ((rx_ptr = (mccrxtx_cmd_type*) q_get( &mcc_rxtx_q )) == NULL)
      {
        /* There is no buffers on the queue,
         * print the error message
         */
        ERR( "Wrong signal-No buffer on mcc_rxtx_q to be processed", 0, 0, 0 );
      }
      else
      {
        cmd_done( &rx_ptr->hdr.cmd_hdr );
      }
    }
    else if ((sigs & MCC_SRCH_Q_SIG) != 0) /* Searcher has returned! */
    {
      if ((rpt_ptr = (mccsrch_rpt_type*) q_get( &mcc_srch_q )) == NULL)
      {
        /* This should not happen,
         * We receive a searcher report but there is no report
         * on the queue
         */
        ERR( "Wrong signal-No reports on mcc_srch_q", 0, 0, 0 );
      }
      else
      {
        if (rpt_ptr->hdr.rpt == SRCH_PC_ASSIGN_R)
        {
          got_searcher_report = TRUE;
          cdma.pagech = PRI_PAGECH;
          cdma.page_chan_s = 1;

          #ifdef FEATURE_IS2000_REL_A_CC
          if (old_ovhd_chan == CAIX_BCCH)
          {
            /* Set default value for Paging Channel data rate */
            cdma.prat = CAI_PC_1_RATE;
          }
          #endif /* FEATURE_IS2000_REL_A_CC */

          MSG_MED( "Searcher Returned PC_ASSIGN pilot = %d",
                   rpt_ptr->pc_assign.pilot, 0, 0);

          /* Update current base station information */
          bs_info_id.ovhd_chan = CAIX_PC;
          bs_info_id.pilot_pn = (word) rpt_ptr->pc_assign.pilot;
          bs_info_id.cdma_ch = cdma.cdmach;
          bs_info_id.band_class = cdma.band_class;
          bs_info_id.ovhd_walsh_ch = cdma.pagech;
          mcc_entry_init(&bs_info_id, FALSE);

          #ifdef FEATURE_IS2000_REL_D
#error code not present
          #endif /* FEATURE_IS2000_REL_D */

          /* Update DB with the new values */
          dbi.code_channel = cdma.pagech;
          db_put(DB_CODE_CHANNEL, &dbi);
          dbi.pilot_pn_off = bs_info_id.pilot_pn;
          db_put(DB_PILOT_PN_OFF, &dbi);

          /* Clear stored overheads */
          mcc_clear_ovhd_msgs();

          /* Notify RXC to skip frames */
          mcc_idle_ho_rx();

          /* Update Searcher's active set */
          mccidl_send_srch_idle_aset();

          /* Notify RXC skip frames */
          mcc_idle_ho_rx();

          /* Update Searcher's active set */
          mccidl_send_srch_idle_aset();

          /* -------------------------------------------------------------------
          ** MS perform channel monitoring supervision to see whether it needs
          ** to switch between PCH/BCCH/FCCCH.
          ** ------------------------------------------------------------------- */
          mccidl_supervise_channel_monitoring(new_state, NULL, FALSE);
        }

        /* Clean up report processing */
        cmd_done( &rpt_ptr->hdr.rpt_hdr );
      }
    } /* end if MCC_SRCH_Q_SIG */
    else
    {
      ERR_FATAL( "MCC_WAIT returned bad sig", 0, 0, 0); 
    }
  }
  while (!got_searcher_report);
 
} /* end mcc_pc_assign() */
/*lint +esym(715, new_state) */

#ifdef FEATURE_AUTH
/*===========================================================================

FUNCTION MCC_QUEUE_AUTH_RPT

DESCRIPTION
  This function enqueues a specified report from the Authentication Task
  to the CDMA subtask of the Main Control Task.

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.
===========================================================================*/

void mcc_queue_auth_rpt
(
  auth_rpt_type  *auth_rpt_buf_ptr
    /* Pointer to auth report buffer filled in by Auth task */
)
{
  auth_rpt_type  *rpt_ptr;
    /* Pointer to buffer from mc_auth_free_q to be queued on mc_auth_q */

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  if ((rpt_ptr = (auth_rpt_type*) q_get( &mc_auth_free_q )) != NULL)
  {
    /* Copy report information into free buffer */
    *rpt_ptr = *auth_rpt_buf_ptr;
    rpt_ptr->rpt_hdr.done_q_ptr = &mc_auth_free_q;

    /* Queue the report buffer */
    (void) q_link( rpt_ptr, &rpt_ptr->rpt_hdr.link );  /* init link */
    q_put( &mc_auth_q, &rpt_ptr->rpt_hdr.link ); /* and queue it */

    /* Signal CDMA subtask */
    (void) rex_set_sigs( &mc_tcb, MCC_Q_SIG );
  }
  else
  {
    ERR_FATAL( "No free buffers on mc_auth_free_q", 0, 0, 0 );
  }
 
} /* mcc_queue_auth_rpt */
#endif /* FEATURE_AUTH */

#ifdef FEATURE_UIM_RUIM
/*===========================================================================

FUNCTION MCC_QUEUE_UIM_RPT

DESCRIPTION
  This function enqueues a specified report from the UIM server Task
  to the CDMA subtask of the Main Control Task.

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.
===========================================================================*/

void mcc_queue_uim_rpt
(
  uim_rpt_type  *uim_rpt_buf_ptr
    /* Pointer to UIM report buffer filled in by UIM task */
)
{
  uim_rpt_type  *rpt_ptr;
    /* Pointer to buffer from mc_uim_free_q to be queued on mc_uim_q */

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  if ((rpt_ptr = (uim_rpt_type*) q_get( &mc_uim_free_q )) != NULL)
  {
    /* Copy report information into free buffer */
    *rpt_ptr = *uim_rpt_buf_ptr;
    rpt_ptr->rpt_hdr.done_q_ptr = &mc_uim_free_q;

    /* Queue the report buffer */
    (void) q_link( rpt_ptr, &rpt_ptr->rpt_hdr.link );  /* init link */
    q_put( &mc_uim_q, &rpt_ptr->rpt_hdr.link ); /* and queue it */

    /* Signal CDMA subtask */
    (void) rex_set_sigs( &mc_tcb, MCC_Q_SIG );
  }
  else
  {
    ERR_FATAL( "No free buffers on mc_uim_free_q", 0, 0, 0 );
  }
 
} /* mcc_queue_uim_rpt */
#endif /* FEATURE_UIM_RUIM */

/*===========================================================================

FUNCTION STUFF_SO_NUM

DESCRIPTION
  This function stores a specified so number into a specified terminal
  information record

DEPENDENCIES

RETURN VALUE
   This function returns an integer value equal to the adjusted
   length of the terminal info rec after the specified so number
   is added. If the specified so number won't fit, then the orignal
   record length is returned.

SIDE EFFECTS
  None

===========================================================================*/

int stuff_so_num
(
  word so,
    /* The so number to put into the specified terminal info rec. */
  int rec_siz,
    /* The size, in bytes, of the terminal info rec prior to
       inserting the so number. */
  word buf_siz,
    /* The size, in bytes, of the buffer which must contain the terminal
       info rec. */
  caii_term_rec_type *rec_ptr
    /* Ptr to terminal info rec. */
)
{
  /* If another so number will fit, add it to record. */

  if ( (word) rec_siz + sizeof(caii_term_rec_var_type) <= buf_siz )
  {
    rec_ptr->var[rec_ptr->fix.num_so++].service_option = so;
    rec_siz += (int) sizeof( caii_term_rec_var_type );
  }

  return ( rec_siz );
 
} /* stuff_so_num() */

/*===========================================================================

FUNCTION STUFF_SO_INFO

DESCRIPTION
  This function stores information about a specified so into a
  specified so information record

DEPENDENCIES

RETURN VALUE
   This function returns an integer value equal to the adjusted
   length of the so info rec after information for the specified
   so is added. If information for the specified so won't fit,
   then the orignal record length is returned.

SIDE EFFECTS
  None

===========================================================================*/

int stuff_so_info
(
  word so,
    /* The so for which to put information into the specified so
       info rec. */
  int rec_siz,
    /* The size, in bytes, of the so info rec prior to inserting
       information for the specified so. */
  word buf_siz,
    /* The size, in bytes, of the buffer which must contain the so
       info rec. */
  caii_so_rec_type *rec_ptr
    /* Ptr to the so info rec. */
)
{
  /* If information about so will fit, add it to record. */

  if ( (word) rec_siz + sizeof(caii_so_rec_var_type) <= buf_siz)
  {
    rec_ptr->var[rec_ptr->fix.num_so  ].fwd = TRUE;
    rec_ptr->var[rec_ptr->fix.num_so  ].rev = TRUE;
    rec_ptr->var[rec_ptr->fix.num_so++].so  = so;
    rec_siz += (int) sizeof(caii_so_rec_var_type);
  }

  return ( rec_siz );
 
} /* stuff_so_info() */

/*===========================================================================

FUNCTION MCC_INFO_BAND_CLASS

DESCRIPTION
  This function builds the band class information record into a specified
  buffer.

DEPENDENCIES

RETURN VALUE
  None
  
SIDE EFFECTS
  None

===========================================================================*/
void mcc_info_band_class
(
  caii_band_rec_type * rec_ptr
)
{
  uint8 i;

  for (i = 0; i < CAI_MAX_BAND_CLASSES; i++)
  {
    rec_ptr->band[i] = mcc_is_band_class_supported(i);
  }

} /* mcc_info_band_class() */

#ifdef FEATURE_MODIFIED_SCI
#error code not present
#endif /* FEATURE_MODIFIED_SCI */

/*===========================================================================

FUNCTION MCC_INFO_REC

DESCRIPTION
  This function builds a specified information record into a specified
  buffer.

DEPENDENCIES

RETURN VALUE
   This function returns an integer value as follows:
    -1 - if the type of the requested information record is invalid or
         the specified qualification information is invalid.
     0 - if the requested record will not fit in the specified buffer.
     n - where n is a positive integer indicating the size, in bytes,
         of the requested information record.

SIDE EFFECTS
  None

===========================================================================*/
int mcc_info_rec
(
  boolean idle,
    /* TRUE  if status request received on paging chan or forward common
                control channel;
       FALSE if on traffic chan */
  byte band,
    /* Band class qualifier */
  byte op_mode,
    /* Operating mode qualifier */
  byte req_typ,
    /* Type of information record requested */
  void *buf_ptr,
    /* Pointer to place to build information record. */
  word buf_siz,
    /* Size of information record buffer, in bytes */
  byte *reject_reason
    /* Reason for rejecting the request */
)
{
  int rec_siz = 0;
    /* For returning function value */
  uint16 j;
    /* Index variable */
  uint8 requested_op_mode_mask = (1 << op_mode);     /*lint !e734 */
    /* Which operating mode the information is requested for */
  boolean bad_qual_info = FALSE;
    /* Flag if the qual_info is bad for the given info record request */
  uint16 num_so_in_snm;
    /* Number of service options in SNM */
  int8 ota_sci;
    /* preferred SCI to send over the air */
  #ifdef FEATURE_IS2000_REL_D
#error code not present
  #endif /* FEATURE_IS2000_REL_D */

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  /*lint -e713 -e413*/
  /* Suppress lint errors related to use of the FENDPOS macro. */

  switch ( req_typ )
  {
    case CAI_TERM_INFO_REQ:
    case CAI_ROAM_INFO_REQ:
    case CAI_PWR_REQ:
    case CAI_SO_REQ:
    case CAI_MUX_REQ:
    case CAI_BAND_SUBCLASS_INFO_REC:
      /* These all require validation of op_mode and band */
      if (!mcc_is_band_class_supported(band))
      {
        /* Requested info for an unsupported band */
        bad_qual_info = TRUE;
      }
      else if ((requested_op_mode_mask & SUPPORTED_OP_MODES) == 0)
      {
        /* Requested info for an unsupported operating mode */
        bad_qual_info = TRUE;
      }
      /* ------------------------------------------------------------------
      ** If the target supports more than one band, an illegal band/op-mode
      ** combination can exist even though both the specified band AND op-
      ** mode are supported.  Here we check to make sure that is detected.
      ** ------------------------------------------------------------------ */

      else if ((op_mode == CAI_IS95_ANAL_OP_MODE) && 
               (!mcc_is_band_class_in_group(MCC_CELLULAR_GROUP, band)))
      {
        /* These op-modes never supported for other than 800MHz band */
        bad_qual_info = TRUE;
      }
      break;

    case CAI_OP_MODE_REQ:
      /* Op-mode only requires validation of the band */
      if (!mcc_is_band_class_supported(band))
      {
        /* Requested info for an unsupported band */
        bad_qual_info = TRUE;
      }
      break;

    default:
      /* All other records do not use band or op-mode qual-info */
      break;
  } /* End switch */

  if (bad_qual_info)
  {
    /* There is no need to process the request if the qual_info is bad */
    *reject_reason = CAI_REJ_CAP;
    return ( -1 );
  }

  switch ( req_typ )
  {
    case CAI_CALL_MODE_REQ:
    {
      caii_call_mode_rec_type  *rec_ptr = (caii_call_mode_rec_type*) buf_ptr;
        /* Local ptr var intialized to buf_ptr */

      if (idle)
      {
        /* Request doesn't make sense on paging channel or forward common
           control channel */
        rec_siz = -1;
        *reject_reason = CAI_REJ_UNSP;
      }
      else if ( sizeof(caii_call_mode_rec_type) > buf_siz )
      {
        rec_siz = 0;  /* Requested record won't fit */
      }
      else
      {
        rec_ptr->hdr.record_type = CAI_CALL_MODE_REC;
        rec_ptr->orig_mode = !cdma.call_orig;
        rec_ptr->pri_service =
          ((cdma.so_cur_pri == CAI_SO_NULL) ? 0 : cdma.so_cur_pri);
        rec_ptr->sec_service =
          ((cdma.so_cur_sec == CAI_SO_NULL) ? 0 : cdma.so_cur_sec);

        rec_siz = sizeof(caii_call_mode_rec_type);
      }

      break;
    } /* case CAI_CALL_MODE_REQ */

    case CAI_TERM_INFO_REQ:
    {
      caii_term_rec_type *rec_ptr = (caii_term_rec_type*) buf_ptr;
        /* Local ptr var intialized to buf_ptr */

      if ( sizeof(caii_term_rec_fix_type) > buf_siz )
      {
        rec_siz = 0; /* Record won't fit */
      }
      else
      {
        rec_ptr->fix.hdr.record_type = CAI_TERM_INFO_REC;
        rec_ptr->fix.mob_p_rev = cdma.mob_cai_rev;
        rec_ptr->fix.mob_mfg_code = B_PTR( cdma.esn )[3];
        #ifdef FEATURE_PASSPORT_MODE
        if(mcc_passport_mode_is_jcdma())
        {
          rec_ptr->fix.mob_model = B_PTR( cdma.esn )[2];
        }
        else
        #endif
        {
          rec_ptr->fix.mob_model = cdma.mob_model;
        }
        
        rec_ptr->fix.mob_firm_rev = cdma.mob_firm_rev;
        switch (op_mode)
        {
          case CAI_J_STD_008_OP_MODE:
            /* -------------------------------------------------------
            ** Actually this isn't an SCM for PCS, but instead a 1 bit
            ** to show it is not an SCM, a reserved bit, a 1 bit to
            ** show slotted mode operation, then 5 more reserved bits.
            ** Therefore 0xa0 is the effective SCM.
            ** ------------------------------------------------------- */
            /* For IS-95B backward compatibility:
            ** 1. If the phone is JSTD008 only or is talking to a JSTD
            **    BS ( P_REV_IN_USE is 1 ), we send 0xa0 as SCM.
            ** 2. If the phone has P_REV_IN_USE greater or equal 4,
            **    then we should send scm and set MSB to 1.
            ** -------------------------------------------------------*/
            if (!mcc_set_scm_for_band_class(band,&(rec_ptr->fix.scm)))
            {
              rec_ptr->fix.scm = 0xa0;
            }
            
            #ifdef FEATURE_MODIFIED_SCI
#error code not present
            #endif /* FEATURE_MODIFIED_SCI */
            {              
              ota_sci = mcc_get_last_sci_reg();

              #ifdef FEATURE_IS2000_REL_D
#error code not present
              #endif /* FEATURE_IS2000_REL_D */
              {
                rec_ptr->fix.slot_cycle_index = (byte) ((ota_sci > 0) ? ota_sci : 0);
                MSG_MED("Info rec reported slot cycle index : val %d", 
                  rec_ptr->fix.slot_cycle_index, 0, 0);
              }
            }

            break;

          case CAI_IS95_CDMA_OP_MODE:
            /* This can mean two different things based on our P_REV.
               If P_REV is 2 or 3, this can only be for band_class 0. But
               if the P_REV is 4 or above, OP_MODE of 1 can be either
               PCS or CELLULAR. So we need to check the BAND_CLASS qualifier */
            if (!mcc_set_scm_for_band_class(band, &(rec_ptr->fix.scm)))
            {
              rec_ptr->fix.scm = cdma.scm & 0x7F;
            }              
            
            #ifdef FEATURE_MODIFIED_SCI
#error code not present
            #endif /* FEATURE_MODIFIED_SCI */
            {
              ota_sci = mcc_get_last_sci_reg();

              #ifdef FEATURE_IS2000_REL_D
#error code not present
              #endif /* FEATURE_IS2000_REL_D */
              {
                rec_ptr->fix.slot_cycle_index = (byte) ((ota_sci > 0) ? ota_sci : 0);
                MSG_MED("Info rec reported slot cycle index : val %d", 
                  rec_ptr->fix.slot_cycle_index, 0, 0);
              }
            }
            break;

          #ifdef FEATURE_ACP
#error code not present
          #endif /* FEATURE_ACP */

          default:
            ERR_FATAL( "Impossible op_mode value", 0, 0, 0);  
            break;

        } /* End switch */

        rec_ptr->fix.local_ctrl = FALSE;

        rec_ptr->fix.num_so = 0;

        rec_siz = sizeof(caii_term_rec_fix_type);

        /* Don't put any service options for analog operating mode */
        if (op_mode != CAI_IS95_ANAL_OP_MODE)
        {
          // Loop through all service options in SNM
          num_so_in_snm = snm_get_total_SOs();
          for (j = 0; j < num_so_in_snm ; j++)
          {
            /* Note: if you would like to return ALL service options, instead of only
               ENABLED service options, remove this check. */
            if (snm_so_is_enabled(snm_get_SO_number(j)))
            {
              rec_siz =
                stuff_so_num ( snm_get_SO_number(j), rec_siz, buf_siz, rec_ptr );
            }
          }
        } /* if (not AMPS operating mode) */
      } /* else */ 
      break;
    } /* case CAI_TERM_INFO_REC */

    case CAI_ROAM_INFO_REQ:
    {
      caii_roam_rec_type  *rec_ptr = (caii_roam_rec_type*) buf_ptr;
        /* Local ptr var intialized to buf_ptr */

      if (sizeof(caii_roam_rec_fix_type) > buf_siz)
      {
        rec_siz = 0; /* Record won't fit */
      }
      else
      {
        rec_ptr->fix.hdr.record_type = CAI_ROAM_INFO_REC;
        rec_ptr->fix.accolc = cdma.accolc;
        rec_ptr->fix.num_vars = 0;

        /* For analog mode, set roam info to zero and no sid/nid pairs */
        if (op_mode == CAI_IS95_ANAL_OP_MODE)
        {
          rec_ptr->fix.mob_term_home = 0;
          rec_ptr->fix.mob_term_for_sid = 0;
          rec_ptr->fix.mob_term_for_nid = 0;

          rec_siz = sizeof(caii_roam_rec_fix_type);
        }
        else
        {
          rec_ptr->fix.mob_term_home = cdma.mob_term_home;
          rec_ptr->fix.mob_term_for_sid = cdma.mob_term_for_sid;
          rec_ptr->fix.mob_term_for_nid = cdma.mob_term_for_nid;

          rec_siz = sizeof(caii_roam_rec_fix_type);

          for (j = 0;
               #ifdef FEATURE_SSPR_ENHANCEMENTS
               (j < NV_MAX_HOME_SID_NID) &&
               #else
               (j < NV_MAX_SID_NID) &&
               #endif /* FEATURE_SSPR_ENHANCEMENTS */
                ((word) rec_siz + sizeof( caii_roam_rec_var_type ) <= buf_siz);
                j++ )
          {
            /* Note that there is a VERY small chance we would not be able
            ** to fit all (sid,nid) pairs, but in that event it is just as
            ** correct to send the ones we could fit as to not send any */

            if (cdma.pair[j].sid != 0)
            {
              rec_ptr->var[rec_ptr->fix.num_vars  ].sid = cdma.pair[j].sid;
              rec_ptr->var[rec_ptr->fix.num_vars++].nid = cdma.pair[j].nid;
              rec_siz += (int) sizeof( caii_roam_rec_var_type );
            }
          } /* for */
        } /* if (op_mode isn't analog) */
      } /* else */

      break;
    } /* case CAI_ROAM_INFO_REC */

    case CAI_SEC_STATUS_REQ:
    {
      caii_sec_stat_rec_type  *rec_ptr = (caii_sec_stat_rec_type*) buf_ptr;
        /* Local ptr var intialized to buf_ptr */

      if (idle)
      {
        /* Request doesn't make sense for paging chan or forward common
           control channel */
        rec_siz = -1;
        *reject_reason = CAI_REJ_UNSP;
      }
      else if (sizeof(caii_sec_stat_rec_type) > buf_siz)
      {
        rec_siz = 0; /* Record won't fit */
      }
      else
      {
        rec_ptr->hdr.record_type = CAI_SEC_REC;
        rec_ptr->auth_mode = cdma.auth_mode;
        rec_ptr->encrypt_mode = cdma.encrypt_mode;

        #ifdef FEATURE_AUTH
        rec_ptr->private_lcm = cdma.long_code.private_lcm_active;
        #else
        rec_ptr->private_lcm = FALSE;
        #endif

        rec_siz = sizeof(caii_sec_stat_rec_type);
      }
      break;
    } /* case CAI_SEC_STATUS_REQ */

    case CAI_IMSI_REQ:
    {
      caii_imsi_rec_type  *rec_ptr = (caii_imsi_rec_type*) buf_ptr;
        /* Local ptr var intialized to buf_ptr */

      if (sizeof(caii_imsi_rec_type) > buf_siz)
      {
        rec_siz = 0; /* Record won't fit */
      }
      else
      {
        rec_ptr->hdr.record_type = CAI_IMSI_REC;
        /* IMSI_CLASS_0_ADDR_NUM denotes the phone has a class 0 IMSI */
        if (cdma.imsi_addr_num == IMSI_CLASS_0_ADDR_NUM)
        {
          rec_ptr->imsi_class    = 0;
          rec_ptr->imsi_addr_num = 0;
        }
        else
        {
          rec_ptr->imsi_class    = 1;
          rec_ptr->imsi_addr_num = cdma.imsi_addr_num;
        }
        rec_ptr->mcc           = cdma.mcc;
        rec_ptr->imsi_11_12    = cdma.imsi_11_12;
        qw_equ(rec_ptr->imsi_s, cdma.imsi_s );

        rec_siz = sizeof(caii_imsi_rec_type);
      }
      break;
    } /* case CAI_IMSI_REQ */

    case CAI_ESN_REQ:
    {
      caii_esn_rec_type  *rec_ptr = (caii_esn_rec_type*) buf_ptr;
        /* Local ptr var intialized to buf_ptr */

      if (sizeof(caii_esn_rec_type) > buf_siz)
      {
        rec_siz = 0; /* Record won't fit */
      }
      else
      {
        rec_ptr->hdr.record_type = CAI_ESN_REC;
        rec_ptr->esn = cdma.esn;

        rec_siz = sizeof(caii_esn_rec_type);
      }
      break;
    } /* case CAI_ESN_REQ */

    case CAI_BAND_REQ:
    {
      caii_band_rec_type  *rec_ptr = (caii_band_rec_type*) buf_ptr;
        /* Local ptr var intialized to buf_ptr */

      if (sizeof(caii_band_rec_type) > buf_siz)
      {
        rec_siz = 0; /* Record won't fit */
      }
      else
      {
        rec_ptr->hdr.record_type = CAI_BAND_REC;

        mcc_info_band_class(rec_ptr); /* Get the band class info */

        rec_siz = sizeof(caii_band_rec_type);
      }
      break;
    } /* case CAI_BAND_REQ */

    case CAI_PWR_REQ:
    {
      caii_pwr_rec_type  *rec_ptr = (caii_pwr_rec_type*) buf_ptr;
        /* Local ptr var intialized to buf_ptr */

      if (sizeof(caii_pwr_rec_type) > buf_siz)
      {
        rec_siz = 0; /* Record won't fit */
      }
      else
      {
        rec_ptr->hdr.record_type = CAI_PWR_REC;

        switch (op_mode)
        {
          case CAI_J_STD_008_OP_MODE:
            /* 200 mW = -7dbW (PCS mode) */
            rec_ptr->max_eirp = 60 - 7;
            break;

          case CAI_IS95_CDMA_OP_MODE:
            /* 200 mW = -7dbW (CDMA mode) */
            rec_ptr->max_eirp = 60 - 7;
            break;

          #ifdef FEATURE_ACP
#error code not present
          #endif /* FEATURE_ACP */

          default:
            ERR_FATAL( "Impossible op_mode value", 0, 0, 0);  
            break;

        } /* End switch */

        rec_siz = sizeof(caii_pwr_rec_type);
      }
      break;
    } /* case CAI_PWR_REQ */

    case CAI_OP_MODE_REQ:
    {
      caii_op_mode_rec_type  *rec_ptr = (caii_op_mode_rec_type*) buf_ptr;
        /* Local ptr var intialized to buf_ptr */

      if (sizeof(caii_op_mode_rec_type) > buf_siz)
      {
        rec_siz = 0;
      }
      else
      {
        rec_ptr->hdr.record_type = CAI_OP_MODE_REC;

        if (mcc_is_band_class_in_group(MCC_CELLULAR_GROUP, band))
        {
          if (P_REV_IS_4_OR_GREATER)
          {
            /* Both Band classes are supported in IS-95B */
            rec_ptr->op_mode0 = TRUE;
            rec_ptr->op_mode1 = TRUE;
          }
          else
          {
            /* No product supports J_STD_008 at Cellular band */
            rec_ptr->op_mode0 = FALSE;
            rec_ptr->op_mode1 =
              ((SUPPORTED_OP_MODES & IS95_CDMA_OP_MODE_MASK) != 0);
          }
          rec_ptr->op_mode2 =
            ((SUPPORTED_OP_MODES & IS95_ANAL_OP_MODE_MASK) != 0);
          rec_ptr->op_mode3 = FALSE;  /* IS-91 wide analog */
          rec_ptr->op_mode4 = FALSE;  /* IS-91 narrow analog */        
        }
        else if (mcc_is_band_class_in_group(MCC_PCS_GROUP, band))
        {
          if (P_REV_IS_4_OR_GREATER)
          {
            /* Both Band classes are supported in IS-95B */
            rec_ptr->op_mode0 = TRUE;
            rec_ptr->op_mode1 = TRUE;
          }
          else
          {
            rec_ptr->op_mode0 =
              ((SUPPORTED_OP_MODES & J_STD_008_OP_MODE_MASK) != 0);
            /* No product supports IS95 at PCS band */
            rec_ptr->op_mode1 = FALSE;
          }
          /* No product supports AMPS at PCS band */
          rec_ptr->op_mode2 = FALSE;
          rec_ptr->op_mode3 = FALSE;  /* IS-91 wide analog */
          rec_ptr->op_mode4 = FALSE;  /* IS-91 narrow analog */
        }
        else 
        {
          ERR_FATAL( "Impossible band value", 0, 0, 0); 
        }

        rec_siz = sizeof(caii_op_mode_rec_type);
      }

      break;
    } /* case CAI_OP_MODE_REQ */

    case CAI_SO_REQ:
    {
      caii_so_rec_type  *rec_ptr = (caii_so_rec_type*) buf_ptr;
        /* Local ptr var intialized to buf_ptr */

      if (op_mode == CAI_IS95_ANAL_OP_MODE)
      {
        /* This record doesn't make any sense for Analog */
        rec_siz = -1;
        *reject_reason = CAI_REJ_UNSP;
      }
      else if (sizeof(caii_so_rec_fix_type) > buf_siz)
      {
        rec_siz = 0;
      }
      else
      {
        rec_ptr->fix.hdr.record_type = CAI_SO_REC;
        rec_ptr->fix.num_so = 0;
        rec_siz = sizeof(caii_so_rec_fix_type);

        // Loop through all service options in SNM.
        num_so_in_snm = snm_get_total_SOs();
        for (j = 0; j < num_so_in_snm ; j++)
        {
          /* Note: if you would like to return ALL service options, instead of only
             ENABLED service options, remove this check. */
          if (snm_so_is_enabled(snm_get_SO_number(j)))
          {
            rec_siz =
              stuff_so_info ( snm_get_SO_number(j), rec_siz, buf_siz, rec_ptr );
          }
        }
      } /* else */

      break;
    } /* case CAI_SO_REQ */

    case CAI_MUX_REQ:
    {
      caii_mux_rec_type  *rec_ptr = (caii_mux_rec_type*) buf_ptr;
        /* Local ptr var intialized to buf_ptr */

      if (op_mode == CAI_IS95_ANAL_OP_MODE)
      {
        /* This record doesn't make any sense for Analog */
        rec_siz = -1;
        *reject_reason = CAI_REJ_UNSP;
      }
      else if (sizeof(caii_mux_rec_fix_type) > buf_siz)
      {
        rec_siz = 0; /* Record won't fit */
      }
      else
      {
        rec_ptr->fix.hdr.record_type = CAI_MUX_REC;
        rec_ptr->fix.num_mux = 0;
        rec_siz = sizeof(caii_mux_rec_fix_type);

        if ((word) rec_siz + sizeof( caii_mux_rec_var_type ) <= buf_siz)
        {
          rec_ptr->var[ rec_ptr->fix.num_mux  ].mux = CAI_MUX1;
          rec_ptr->var[ rec_ptr->fix.num_mux  ].fwd_rates = CAI_RS1_ALL;
          rec_ptr->var[ rec_ptr->fix.num_mux++].rev_rates = CAI_RS1_ALL;
          rec_siz += (int) sizeof( caii_mux_rec_var_type );
        }
      #ifdef FEATURE_MVS_MIGRATE
#error code not present
      #else
        if (voc_capability_supported( VOC_CAP_IS733))
      #endif /* FEATURE_MVS_MIGRATE */
        {
          if ((word) rec_siz + sizeof( caii_mux_rec_var_type ) <= buf_siz)
          {
            rec_ptr->var[ rec_ptr->fix.num_mux  ].mux = CAI_MUX2;
            rec_ptr->var[ rec_ptr->fix.num_mux  ].fwd_rates = CAI_RS2_ALL;
            rec_ptr->var[ rec_ptr->fix.num_mux++].rev_rates = CAI_RS2_ALL;
            rec_siz += (int) sizeof( caii_mux_rec_var_type );
          }
        }

        #if defined(FEATURE_IS95B_MDR)
        // Check to make sure there's enough room and prev is higher than 3 before
        // report these mux. records.
        if (((word) rec_siz + 2*sizeof( caii_mux_rec_var_type ) <= buf_siz) &&
            #ifdef FEATURE_IS95B_MDR
            P_REV_IS_4_OR_GREATER )
            #else  //P_REV macros may not exist for Special MDR
             (mccsyobj_get_prev()) > 3 )
            #endif //FEATURE_IS95B_MDR
        {
          #ifdef FEATURE_PASSPORT_MODE
          // Check if MDR SO 22 (RS1) is currently enabled
          if (mcc_passport_mode_is_jcdma() &&
              snm_so_is_enabled(CAI_SO_MDR_PKT_DATA_FRS1_RRS1))
          #endif /* FEATURE_PASSPORT_MODE */
          {
            // Report the maximum supported forward mux option for rate set 1
            rec_ptr->var[ rec_ptr->fix.num_mux  ].mux = CAI_MAX_USABLE_MUX_RS1;
            rec_ptr->var[ rec_ptr->fix.num_mux  ].fwd_rates = CAI_RS1_ALL;
            rec_ptr->var[ rec_ptr->fix.num_mux++].rev_rates = 0;
            rec_siz += (int) sizeof( caii_mux_rec_var_type );
          }

          #ifdef FEATURE_PASSPORT_MODE
          // Check if MDR SO 25 (RS2) is currently enabled
          if (mcc_passport_mode_is_jcdma() &&
              snm_so_is_enabled(CAI_SO_MDR_PKT_DATA_FRS2_RRS2))
          #endif /* FEATURE_PASSPORT_MODE */
          {
            // And then the maximum supported forward mux option for rate set 2
            rec_ptr->var[ rec_ptr->fix.num_mux  ].mux = CAI_MAX_USABLE_MUX_RS2;
            rec_ptr->var[ rec_ptr->fix.num_mux  ].fwd_rates = CAI_RS2_ALL;
            rec_ptr->var[ rec_ptr->fix.num_mux++].rev_rates = 0;
            rec_siz += (int) sizeof( caii_mux_rec_var_type );
          }
        } // end enough space in record
        #endif /* defined(FEATURE_IS95B_MDR */
      } /* else */

      break;
    } /* case CAI_MUX_REQ */

    case CAI_SRV_CFG_REQ:
    {
      caii_srv_cfg_type  *rec_ptr = (caii_srv_cfg_type*) buf_ptr;
        /* Local ptr var intialized to buf_ptr */

      if (idle)
      {
        /* Request doesn't make sense on paging channel */
        rec_siz = -1;
        *reject_reason = CAI_REJ_UNSP;
      }
      else if (sizeof(caii_srv_cfg_type) > buf_siz)
      {
        rec_siz = 0; /* Record won't fit */
      }
      else
      {
        if (cdma.serv_neg)
        {
          srv_get_cur_cfg ( rec_ptr ); /* Get current service config */
          rec_ptr->hdr.record_type = CAI_RTC_SRV_CFG_REC;
          rec_siz = sizeof(caii_srv_cfg_type);
        }
        else
        {
          rec_siz = -1;
          *reject_reason = CAI_REJ_UNSP;
        }
      }

      break;
    } /* case CAI_SRV_CFG_REQ */

    case CAI_IMSI_M_REQ:
    {
      caii_imsi_rec_type  *rec_ptr = (caii_imsi_rec_type*) buf_ptr;
        /* Local ptr var intialized to buf_ptr */

      if (!(P_REV_IS_4_OR_GREATER))
      {
        /* Reject if P_REV_IN_USE is less than 4 */
        rec_siz = -1;
        *reject_reason = CAI_REJ_UNSP;
      }
      else if (sizeof(caii_imsi_rec_type) > buf_siz)
      {
        rec_siz = 0; /* Record won't fit */
      }
      else
      {
        rec_ptr->hdr.record_type = CAI_IMSI_M_REC;

        /* IMSI_CLASS_0_ADDR_NUM denotes the phone has a class 0 IMSI */
        if (cdma.imsi_m_addr_num == IMSI_CLASS_0_ADDR_NUM)
        {
          rec_ptr->imsi_class    = 0;
          rec_ptr->imsi_addr_num = 0;
        }
        else
        {
          rec_ptr->imsi_class    = 1;
          rec_ptr->imsi_addr_num = cdma.imsi_m_addr_num;
        }
        rec_ptr->mcc         = cdma.imsi_m_mcc;
        rec_ptr->imsi_11_12  = cdma.imsi_m_11_12;
        qw_equ(rec_ptr->imsi_s, cdma.imsi_m_s );

        rec_siz = sizeof(caii_imsi_rec_type);
      }
      break;
    } /* case CAI_IMSI_M_REQ */

    case CAI_IMSI_T_REQ:
    {
      caii_imsi_rec_type  *rec_ptr = (caii_imsi_rec_type*) buf_ptr;
        /* Local ptr var intialized to buf_ptr */

      if (!(P_REV_IS_4_OR_GREATER))
      {
        /* Reject if P_REV_IN_USE is less than 4 */
        rec_siz = -1;
        *reject_reason = CAI_REJ_UNSP;
      }
      else if (sizeof(caii_imsi_rec_type) > buf_siz)
      {
        rec_siz = 0; /* Record won't fit */
      }
      else
      {
        rec_ptr->hdr.record_type = CAI_IMSI_T_REC;

        /* IMSI_CLASS_0_ADDR_NUM denotes the phone has a class 0 IMSI */
        if (cdma.imsi_t_addr_num == IMSI_CLASS_0_ADDR_NUM)
        {
          rec_ptr->imsi_class    = 0;
          rec_ptr->imsi_addr_num = 0;
        }
        else
        {
          rec_ptr->imsi_class    = 1;
          rec_ptr->imsi_addr_num = cdma.imsi_t_addr_num;
        }
        rec_ptr->mcc           = cdma.imsi_t_mcc;
        rec_ptr->imsi_11_12    = cdma.imsi_t_11_12;
        qw_equ(rec_ptr->imsi_s, cdma.imsi_t_s );

        rec_siz = sizeof(caii_imsi_rec_type);
      }
      break;
    } /* case CAI_IMSI_T_REQ */

    case CAI_PWR_CTRL_REQ:
    {
      caii_pwr_ctrl_rec_type *rec_ptr = (caii_pwr_ctrl_rec_type*) buf_ptr;
      /* Local pointer var is initialized to a buffer pointer */
      if (sizeof(caii_pwr_ctrl_rec_type) > buf_siz)
      {
         rec_siz = 0;     /* record size won't fit */
      }
      else
      {
        rec_ptr->hdr.record_type = CAI_PWR_CTRL_REC;
        /* Get the info from our capability manager */
        rec_ptr->min_pwr_cntl_step = mccap_get_min_pwr_ctrl_step();
        rec_siz = sizeof(caii_pwr_ctrl_rec_type);
      }
      break;
    }

    /* ------------------------------------------------------------ */
    /* Enable the capability information record for all MOB_P_REVs  */
    /* even though this is first introduced for MOB_P_REV 4 mobiles */
    /* ------------------------------------------------------------ */
    case CAI_CAP_REQ:
    {
      caii_capa_rec_type *rec_ptr = (caii_capa_rec_type*) buf_ptr;
      /* Local pointer var is initialized to a buffer pointer */

      if (sizeof(caii_capa_rec_type) > buf_siz)
      {
         rec_siz = 0;     /* record size won't fit */
      }
      else
      {
        rec_ptr->hdr.record_type = CAI_CAPA_INFO_REC;

        rec_ptr->acc_entry_ho = mccap_item_is_supported (MCCAP_DF_AEHO);
        rec_ptr->acc_probe_ho = mccap_item_is_supported (MCCAP_DF_APHO);   

        #ifdef FEATURE_IS95B_ANALOG_SEARCH
        rec_ptr->analog_search = TRUE;
        #else
        rec_ptr->analog_search = FALSE;   /* Not supported */
        #endif /* FEATURE_IS95B_ANALOG_SEARCH */

        #ifdef FEATURE_IS95B_HOPPING_BEACON
        rec_ptr->hop_beacon = TRUE;
        #else
        rec_ptr->hop_beacon = FALSE;      /* Not supported */
        #endif /* FEATURE_IS95B_HOPPING_BEACON */

        rec_ptr->mahho = TRUE;

        #ifdef FEATURE_IS95B_PUF
        rec_ptr->puf = TRUE;
        #else
        rec_ptr->puf = FALSE;             /* Not supported */
        #endif /* FEATURE_IS95B_PUF */

        #ifdef FEATURE_IS95B_ANALOG_553
        rec_ptr->analog_553 = TRUE;
        #else
        rec_ptr->analog_553 = FALSE;      /* Not supported */
        #endif /* FEATURE_IS95B_ANALOG_553 */

       /* The following function populates all the IS2000
          related fields that were added to this IR */
        if (P_REV_IS_6_OR_GREATER)
        {
          mccap_get_advanced_capability_fields( rec_ptr );
        }

        #ifdef FEATURE_IS2000_REL_D
#error code not present
        #endif /* FEATURE_IS2000_REL_D */

        #ifdef FEATURE_IS2000_REL_C
        if (P_REV_IS_9_OR_GREATER)
        {
          rec_siz = FENDPOS(caii_capa_rec_type, rev_fch_gating_supported);
        }
        else
        #endif /* FEATURE_IS2000_REL_C */

        #ifdef FEATURE_IS2000_REL_B
        if (P_REV_IS_8_OR_GREATER &&
            (rec_ptr->var_supported || rec_ptr->flex_supported))
        {
          rec_siz = FENDPOS(caii_capa_rec_type, eram_supported);
        }
        else
        #endif /* FEATURE_IS2000_REL_B */

        #ifdef FEATURE_IS2000_REL_A
        if (P_REV_IS_7_OR_GREATER)
        {
          rec_siz = FENDPOS(caii_capa_rec_type, r_sch_ltu_tab_supported);
        }
        else
        #endif /* FEATURE_IS2000_REL_A */

        if (P_REV_IS_6_OR_GREATER)
        {
          if (rec_ptr->rlp_cap_blob_len)
          {
            rec_siz = FENDPOS( caii_capa_rec_type, rlp_cap_blob[(rec_ptr->rlp_cap_blob_len)-1]);
          }
          else
          {
            rec_siz = FENDPOS( caii_capa_rec_type, rlp_cap_blob_len);
          }
        }
        else
        {
          rec_siz = FENDPOS( caii_capa_rec_type, analog_553);
        }
      } /* end else of buffer size check */

      break;

    } /* case CAI_CAP_REQ */

    case CAI_CHAN_CFG_CAPA_INFO_REC:
    {
      caii_chan_cfg_capa_info_type  *rec_ptr = 
        (caii_chan_cfg_capa_info_type*) buf_ptr;

      rec_ptr->hdr.record_type = CAI_CHAN_CFG_CAPA_INFO_REC;
      mccap_get_chan_cfg_info(rec_ptr);

      #ifdef FEATURE_IS2000_REL_D
#error code not present
      #endif /* FEATURE_IS2000_REL_D */

      #ifdef FEATURE_IS2000_REL_C
      if (P_REV_IS_9_OR_GREATER)
      {
        if (rec_ptr->for_pdch_supported)
        {
          if (rec_ptr->for_sch_supported)
          {
            rec_siz = FENDPOS(caii_chan_cfg_capa_info_type, for_pdch_sch_supported);
          }
          else
          {
            rec_siz = FENDPOS(caii_chan_cfg_capa_info_type, for_pdch_fields);
          }
        }
        else
        {
          rec_siz = FENDPOS(caii_chan_cfg_capa_info_type, for_pdch_supported);
        }
      }
      else
      #endif /* FEATURE_IS2000_REL_C */

      #ifdef FEATURE_IS2000_REL_B
      if (P_REV_IS_8_OR_GREATER && rec_ptr->for_sch_supported)
      {
        rec_siz = FENDPOS(caii_chan_cfg_capa_info_type, ccsh_supported);
      }
      else 
      #endif /* FEATURE_IS2000_REL_B */

      #ifdef FEATURE_IS2000_REL_A
      if (P_REV_IS_7_OR_GREATER)
      {
        rec_siz = FENDPOS(caii_chan_cfg_capa_info_type, threex_cch_supported);
      }
      else
      #endif /* FEATURE_IS2000_REL_A */

      if (rec_ptr->rev_sch_supported)
      {
        if (rec_ptr->rev_sch_fields.rev_sch_num)
        {
          rec_siz = FENDPOS(caii_chan_cfg_capa_info_type,
            rev_sch_fields.rev_sch_recs[rec_ptr->rev_sch_fields.rev_sch_num-1]);
        }
        else
        {
          rec_siz = FENDPOS(caii_chan_cfg_capa_info_type, rev_sch_fields.rev_sch_num);
        }
      }
      else
      {
        rec_siz = FENDPOS(caii_chan_cfg_capa_info_type, rev_sch_supported);
      }

      break;
    }

    case CAI_EXT_MULT_OPT_INFO_REC:
    {
      caii_ext_mult_opt_info_type  *rec_ptr = (caii_ext_mult_opt_info_type*) buf_ptr;
      rec_ptr->hdr.record_type = CAI_EXT_MULT_OPT_INFO_REC;
      mccap_get_ext_mux_option_info( rec_ptr);

      #ifdef FEATURE_IS2000_REL_D
#error code not present
      #endif /* FEATURE_IS2000_REL_D */

      #ifdef FEATURE_IS2000_REL_C
      if (P_REV_IS_9_OR_GREATER)
      {
        if (rec_ptr->num_mo_for_pdch) 
        {
          rec_siz = FENDPOS(caii_ext_mult_opt_info_type, 
            mo_for_pdch[(rec_ptr->num_mo_for_pdch) - 1]);
        }
        else
        {
          rec_siz = FENDPOS(caii_ext_mult_opt_info_type, num_mo_for_pdch);
        }
      }
      else
      #endif /* FEATURE_IS2000_REL_C */

      if (rec_ptr->num_mo_rev_sch) 
      {
        rec_siz = FENDPOS(caii_ext_mult_opt_info_type, mo_rev_sch_recs[(rec_ptr->num_mo_rev_sch)-1]);
      }
      else
      {
        rec_siz = FENDPOS(caii_ext_mult_opt_info_type, num_mo_rev_sch);
      }

      break;
    }

    case  CAI_GEO_LOC_INFO_REC:
    {
      caii_geo_loc_info_type  *rec_ptr = (caii_geo_loc_info_type*) buf_ptr;
      rec_ptr->hdr.record_type = CAI_GEO_LOC_INFO_REC;
      rec_ptr->geo_loc = mccap_get_geo_loc_cap();
      rec_siz = sizeof(caii_geo_loc_info_type);
      break;
    }

    #ifdef FEATURE_IS2000_REL_A
    case CAI_HOOK_STATUS_REQ:
    {
      /* we don't suppport hook status as yet */
      rec_siz = -1;
      *reject_reason = CAI_REJ_CAP;
      break;
    }

    case CAI_ENC_CAP_REQ:
    {
      caii_enc_cap_type  *rec_ptr = (caii_enc_cap_type*) buf_ptr;
      rec_ptr->hdr.record_type = CAI_ENC_CAP_REC;
      mccap_get_encryption_cap_info(rec_ptr);
      rec_siz = sizeof(caii_enc_cap_type);
      break;
    }
    #endif /* FAETURE_IS2000_REL_A */

    #ifdef FEATURE_IS2000_REL_C
    case CAI_SIG_MSG_INT_CAP_REC:
    {
      caii_sig_msg_int_info_type *rec_ptr = (caii_sig_msg_int_info_type *) buf_ptr;
     
      if (!P_REV_IS_9_OR_GREATER)
      {
        /* Reject if P_REV_IN_USE is less than 9 */
        rec_siz = -1;
        *reject_reason = CAI_REJ_UNSP;
      }
      else if (sizeof(caii_sig_msg_int_info_type) > buf_siz)
      {
        rec_siz = 0; /* Record won't fit */
      }
      else
      {
        rec_ptr->hdr.record_type = CAI_SIG_MSG_INT_CAP_REC;
        mccap_get_sig_integrity_sup_info(rec_ptr);
        rec_siz = sizeof(caii_sig_msg_int_info_type);
      }

      break;
    }

    case CAI_UIM_ID_REC:
    {
      caii_uim_id_info_type *rec_ptr = (caii_uim_id_info_type *) buf_ptr;

      if (!P_REV_IS_9_OR_GREATER)
      {
        /* Reject if P_REV_IN_USE is less than 9 */
        rec_siz = -1;
        *reject_reason = CAI_REJ_UNSP;
      }
      else if (sizeof(caii_uim_id_info_type) > buf_siz)
      {
        rec_siz = 0; /* Record won't fit */
      }
      else
      {
        rec_ptr->hdr.record_type = CAI_UIM_ID_REC;

        #ifdef FEATURE_UIM_RUIM
        if (cdma.ruim_id_avail)
        {
          rec_ptr->uim_id_len = FSIZ(caii_uim_id_info_type, uim_id);
          rec_ptr->uim_id = cdma.ruim_id;
          rec_siz = sizeof(caii_uim_id_info_type);
        }
        else
        #endif /* FEATURE_UIM_RUIM */
        {
          rec_ptr->uim_id_len = 0;
          rec_siz = FSIZ(caii_uim_id_info_type, uim_id);
        }
      }

      break;
    }

    #endif /* FEATURE_IS2000_REL_C */

    #if defined(FEATURE_IS2000_REL_C) || defined(FEATURE_HWID_SUPPORT)
    case CAI_ESN_ME_REC:
    {
      caii_esn_me_info_type *rec_ptr = (caii_esn_me_info_type *) buf_ptr;

      #ifdef FEATURE_HWID_SUPPORT
      if (!P_REV_IS_6_OR_GREATER)
      #else
      if (!P_REV_IS_9_OR_GREATER)
      #endif 
      {
        /* Reject if P_REV_IN_USE is less than 6 with HWID feature */
        /* Reject if P_REV_IN_USE is less than 9 without HWID feature */
        rec_siz = -1;
        *reject_reason = CAI_REJ_UNSP;
      }
      else if (sizeof(caii_esn_me_info_type) > buf_siz)
      {
        rec_siz = 0; /* Record won't fit */
      }
      else
      {
        rec_ptr->hdr.record_type = CAI_ESN_ME_REC;
        rec_ptr->esn_me_len = FSIZ(caii_esn_me_info_type, esn_me);

        #ifdef FEATURE_UIM_RUIM
        rec_ptr->esn_me = cdma.esn_me;
        #else
        rec_ptr->esn_me = cdma.esn;
        #endif /* FEATURE_UIM_RUIM */

        rec_siz = sizeof(caii_esn_me_info_type);
      }

      break;
    }
    #endif /* FEATURE_IS2000_REL_C || FEATURE_HWID_SUPPORT */

    #ifdef FEATURE_IS2000_REL_D
#error code not present
    #endif /* FEATURE_IS2000_REL_D */

    #ifdef FEATURE_IS2000_REL_A
    case CAI_BAND_SUBCLASS_INFO_REC:
    {
      caii_band_subclass_type *rec_ptr = (caii_band_subclass_type *) buf_ptr;

      if (sizeof(caii_band_subclass_type) > buf_siz)
      {
        rec_siz = 0;     /* record size won't fit */
      }
      else
      {
        /* Query the RF for the band subclass information */
        dword band_sc_mask = rf_get_sub_class((rf_card_band_type) band);   

        rec_ptr->hdr.record_type =  CAI_BAND_SUBCLASS_INFO_REC;

        rec_siz = sizeof(caii_band_subclass_type);

        for (j=0; j < CAI_MAX_BAND_SUBCLASSES; j++)
        {
          rec_ptr->subband[j] = ((band_sc_mask & (1<<j)) != 0);
        }
      }
      break;
    }
    #endif /* FEATURE_IS2000_REL_A */

    #if defined(FEATURE_MEID_SUPPORT) || defined(FEATURE_IS2000_REL_D)
    case  CAI_MEID_REC:
    {
      caii_meid_info_type *rec_ptr = (caii_meid_info_type *) buf_ptr;

      if (mccap_is_meid_supported())
      {
        /* Populate meid info here */
        rec_ptr->hdr.record_type = CAI_MEID_REC;
        rec_ptr->meid_len = 7;
        qw_equ(rec_ptr->meid, cdma.meid);
        rec_siz = sizeof(caii_meid_info_type);
      }
      else
      {
        rec_siz = -1;

        /* The MEID for this MS was not programmed */ 
        *reject_reason = CAI_REJ_CODE;
      }

      break;
    }
    #endif /* FEATURE_MEID_SUPPORT || FEATURE_IS2000_REL_D */

    #if defined(FEATURE_MEID_SUPPORT) && defined(FEATURE_HWID_SUPPORT)
    case  CAI_MEID_ME_REC:
    {
      caii_meid_me_info_type *rec_ptr = (caii_meid_me_info_type *) buf_ptr;

      if (mccap_is_meid_supported())
      {
        /* Populate meid info here */
        rec_ptr->hdr.record_type = CAI_MEID_ME_REC;
        rec_ptr->meid_me_len = 7;
        #ifdef FEATURE_UIM_RUIM
        qw_equ(rec_ptr->meid_me, cdma.meid_me);
        #else
        qw_equ(rec_ptr->meid_me, cdma.meid);
        #endif /* FEATURE_UIM_RUIM */
        rec_siz = sizeof(caii_meid_me_info_type);
      }
      else
      {
        rec_siz = -1;

        /* The MEID for this MS was not programmed */ 
        *reject_reason = CAI_REJ_CODE;
      }

      break;
    }
    #endif /* FEATURE_MEID_SUPPORT && FEATURE_HWID_SUPPORT */
    
#ifdef FEATURE_IS2000_REL_D
#error code not present
    #endif /* FEATURE_IS2000_REL_D */

    default: /* Unknown request type */
      rec_siz = -1;

      /* P_REV check is added for backwards compatibility */
      if(P_REV_IS_4_OR_GREATER)
      {
       *reject_reason = CAI_REJ_MOD;  /* new for IS-95B */
      }
      else
      {
       *reject_reason = CAI_REJ_UNSP; /* backwards compatibility */       
      }
      break;

  } /* switch */

/*lint +e713 +e413*/

  return ( rec_siz );
 
} /* mcc_info_rec() */

/*lint -e826 */
/*===========================================================================

FUNCTION MCC_PROCESS_INFO_RECORDS

DESCRIPTION
  This function processes the Feature Notification, Alert With Information
  and Flash With Information messages.

DEPENDENCIES
  None.

RETURN VALUE
  TRUE if an Alert With Information Message contains a signal record with
  SIGNAL_TYPE field set to 1 or 2.  FALSE otherwise. (This information is
  needed during the Conversation State).

SIDE EFFECTS
  None.

===========================================================================*/
boolean mcc_process_info_records
(
  byte call_id,
    /* call_id for CM */
  byte msg_type,
    /* Message type */
  byte num_recs,
    /* Number of information records in message */
  byte *rec_ptr
    /* Pointer to information records */
)
{
  cm_mc_rpt_type *cmd_ptr;
    /* Pointer to command to send to UI task */
  word rec_num;
    /* Index through information records */
  word idx;
    /* Index */
  boolean signal_rec_rxed;
  boolean ret_val;
    /* Value to return to caller */
  boolean last_rec;
    /* Flag to indicate last info record from this message */
  word len;
    /* temp to hold expected len of ext display record */

  #ifdef FEATURE_PASSPORT_MODE
  /* Pointers to buffers that hold CallerID+CLIR information */
  cm_mc_rpt_type *calling_rec_ptr = NULL;
  cm_mc_rpt_type *clir_rec_ptr = NULL;
  #endif /* FEATURE_PASSPORT_MODE */

  #ifdef FEATURE_IS2000_REL_A
  caii_mc_ext_disp_type * mc_ext_disp_ptr;
  word idx2;
  #endif

  #ifdef FEATURE_IS2000_REL_C
  caii_enh_mc_ext_disp_type * enh_mc_ext_disp_ptr;
  #endif

  /*lint -e413*/
  /* Suppress lint errors related to use of the FENDPOS macro. */

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  /* Initialize return to indicate Signal info record not received */
  signal_rec_rxed = FALSE;
  ret_val         = TRUE;

  last_rec        = FALSE;

  for (rec_num = 1; rec_num <= num_recs; rec_num++)
  {
    #ifdef FEATURE_CDSMS
    if( ((caii_rec_hdr_type *) rec_ptr)->record_type == CAI_MSG_WAITING_REC )
    {
      cm_mt_dbm_msg( (byte) CAI_SHORT_MSG_SERVICES,
                     (byte) CM_SMS_FORMAT_MWI,
                     &(((caii_msg_waiting_rec_type *) rec_ptr)->msg_count),
                     (word) sizeof(((caii_msg_waiting_rec_type *) rec_ptr)->msg_count) );

      /* Move record pointer to next record */
      rec_ptr += sizeof( caii_msg_waiting_rec_type );

      rec_ptr = (byte*) ALIGN_DWORD((dword)rec_ptr);

      continue; /* go to the next record */
    }
    #endif /* defined (FEATURE_CDSMS) */

    if ((cmd_ptr = cm_mc_rpt_get_buf_else_err_fatal()) == NULL)
    {
      ERR_FATAL( "No buffers on cm_mc_rpt_free_q", 0, 0, 0 ); 
    }
    else
    {
      /* we need to see if we need to synthesize a signal record   */
      /* before we send the last message (because then it wouldn't */
      /* be the last message).                                     */
      if (rec_num == num_recs)
      {
        if ((signal_rec_rxed) || 
            ((msg_type != CAI_ALERT_MSG)
             #ifdef FEATURE_IS2000_REL_A
             && (msg_type != CAI_EXT_ALERT_W_INFO_MSG)
             #endif
            ) ||
            (((caii_rec_hdr_type *) rec_ptr)->record_type == CAI_SIGNAL_REC))
        {
          #ifdef FEATURE_PASSPORT_MODE
          if(mcc_passport_mode_is_jcdma())
          {
            /* If T53, set the last_rec to TRUE only if we did *not* receive
             * any part of Caller ID information. If we did receive the
             * the caller ID information, then set the last_rec to FALSE.
             * We set the last_rec at a later time when caller ID
             * information is sent to CM
             */
            if (calling_rec_ptr == NULL && clir_rec_ptr == NULL)
            {
              last_rec = TRUE;
            }
            else
            {
              last_rec = FALSE;
            }
          }
          else
          #endif
          {
          last_rec = TRUE;
          }
          

        }
      }

      switch (((caii_rec_hdr_type *) rec_ptr)->record_type)
      {
        case CAI_DISPLAY_REC:
          cmd_ptr->display.hdr.cmd = CM_DISPLAY_F;
          cmd_ptr->display.hdr.last_record = last_rec;

          cmd_ptr->display.call_id = call_id;

          if (((caii_display_rec_type *) rec_ptr)->fix.num_char >
                  CM_MAX_DATA_DIGITS )
          {
            cmd_ptr->display.num_chars = CM_MAX_DATA_DIGITS;
          }
          else
          {
            cmd_ptr->display.num_chars =
              ((caii_display_rec_type *) rec_ptr)->fix.num_char;
          }

          for (idx = 0; idx < cmd_ptr->display.num_chars; idx++)
          {
            cmd_ptr->display.chars[idx] =
              ((caii_display_rec_type *) rec_ptr)->var[idx];
          }

          /* Move record pointer to next record */
          rec_ptr += FENDPOS( caii_display_rec_type,
            var[((caii_display_rec_type *) rec_ptr)->fix.num_char - 1] );
          break;

        case CAI_CALLED_REC:
          cmd_ptr->called_party.hdr.cmd = CM_CALLED_PARTY_F;
          cmd_ptr->called_party.hdr.last_record = last_rec;
          cmd_ptr->called_party.number_type =
            ((caii_called_rec_type *) rec_ptr)->fix.number_type;
          cmd_ptr->called_party.number_plan =
            ((caii_called_rec_type *) rec_ptr)->fix.number_plan;

          cmd_ptr->called_party.call_id = call_id;

          if (((caii_called_rec_type *) rec_ptr)->fix.num_char >
                  CM_MAX_DATA_DIGITS )
          {
            cmd_ptr->called_party.num_chars = CM_MAX_DATA_DIGITS;
          }
          else
          {
            cmd_ptr->called_party.num_chars =
              ((caii_called_rec_type *) rec_ptr)->fix.num_char;
          }

          for (idx=0; idx < cmd_ptr->called_party.num_chars; idx++)
          {
            cmd_ptr->called_party.chars[idx] =
              ((caii_called_rec_type *) rec_ptr)->var[idx];
          }

          /* Move record pointer to next record */
          rec_ptr += FENDPOS( caii_called_rec_type,
            var[((caii_called_rec_type *) rec_ptr)->fix.num_char - 1] );
          break;

        case CAI_CALLING_REC:
          cmd_ptr->calling_party.hdr.cmd = CM_CALLING_PARTY_F;
          cmd_ptr->calling_party.hdr.last_record = last_rec;

          cmd_ptr->calling_party.call_id = call_id;

          cmd_ptr->calling_party.number_type =
            ((caii_calling_rec_type *) rec_ptr)->fix.number_type;
          cmd_ptr->calling_party.number_plan =
            ((caii_calling_rec_type *) rec_ptr)->fix.number_plan;
          cmd_ptr->calling_party.pi =
            ((caii_calling_rec_type *) rec_ptr)->fix.pi;
          cmd_ptr->calling_party.si =
            ((caii_calling_rec_type *) rec_ptr)->fix.si;

          if (((caii_calling_rec_type *) rec_ptr)->fix.num_char >
                  CM_MAX_DATA_DIGITS)
          {
            cmd_ptr->calling_party.num_chars = CM_MAX_DATA_DIGITS;
          }
          else
          {
            cmd_ptr->calling_party.num_chars =
              ((caii_calling_rec_type *) rec_ptr)->fix.num_char;
          }

          for (idx=0; idx < cmd_ptr->calling_party.num_chars; idx++)
          {
            cmd_ptr->calling_party.chars[idx] =
              ((caii_calling_rec_type *) rec_ptr)->var[idx];
          }

          /* Move record pointer to next record */
          rec_ptr += FENDPOS( caii_calling_rec_type,
            var[((caii_calling_rec_type *) rec_ptr)->fix.num_char - 1] );

          #ifdef FEATURE_PASSPORT_MODE

          if(mcc_passport_mode_is_jcdma())
          {
            /* Save the CAI_CALLING_REC in the temporary buffer. Please
             * note that we only save the most recently recieved CAI_CALLING_REC.
             */
            if (calling_rec_ptr != NULL)
            {
              /* We already received a cmd_ptr earlier. Now replace the old
               * one with this new one becuase we only save the most recently
               * recieved CAI_CALLING_REC. However, before we save the one,
               * return the buffer that contained the last CAI_CALLING_REC
               * to the free queue.
               */
              cm_mc_rpt_free_buf(calling_rec_ptr);
            }

            /* Save the current CAI_CALLING_REC */
            calling_rec_ptr = cmd_ptr;

            /* Set cmd_ptr to null to indicate no command to send to CM */
            cmd_ptr = NULL;
          }
          #endif

          break;

        case CAI_CONNECT_REC:
          cmd_ptr->connected_num.hdr.cmd = CM_CONNECTED_NUM_F;
          cmd_ptr->connected_num.hdr.last_record = last_rec;

          cmd_ptr->connected_num.call_id = call_id;

          cmd_ptr->connected_num.number_type =
            ((caii_calling_rec_type *) rec_ptr)->fix.number_type;
          cmd_ptr->connected_num.number_plan =
            ((caii_calling_rec_type *) rec_ptr)->fix.number_plan;
          cmd_ptr->connected_num.pi =
            ((caii_calling_rec_type *) rec_ptr)->fix.pi;
          cmd_ptr->connected_num.si =
            ((caii_calling_rec_type *) rec_ptr)->fix.si;

          if (((caii_calling_rec_type *) rec_ptr)->fix.num_char >
                  CM_MAX_DATA_DIGITS )
          {
            cmd_ptr->connected_num.num_chars = CM_MAX_DATA_DIGITS;
          }
          else
          {
            cmd_ptr->connected_num.num_chars =
             ((caii_calling_rec_type *) rec_ptr)->fix.num_char;
          }

          for (idx=0; idx < cmd_ptr->connected_num.num_chars; idx++)
          {
            cmd_ptr->connected_num.chars[idx] =
              ((caii_calling_rec_type *) rec_ptr)->var[idx];
          }

          /* Move record pointer to next record */
          rec_ptr += FENDPOS( caii_calling_rec_type,
            var[((caii_calling_rec_type *) rec_ptr)->fix.num_char - 1] );
          break;

        case CAI_SIGNAL_REC:
          signal_rec_rxed = TRUE;
          cmd_ptr->signal.hdr.cmd = CM_SIGNAL_F;
          cmd_ptr->signal.hdr.last_record = last_rec;

          cmd_ptr->signal.call_id = call_id;

          cmd_ptr->signal.signal_type =
            ((caii_signal_rec_type *) rec_ptr)->signal_type;

          if( (cmd_ptr->signal.signal_type != CAI_SIG_ISDN) &&
              (cmd_ptr->signal.signal_type != CAI_SIG_IS54B))
          {
            ret_val = FALSE;
          }
          cmd_ptr->signal.alert_pitch =
            ((caii_signal_rec_type *) rec_ptr)->alert_pitch;
          cmd_ptr->signal.signal =
            ((caii_signal_rec_type *) rec_ptr)->signal;

          /* Move record pointer to next record */
          rec_ptr += sizeof( caii_signal_rec_type );
          break;

        #ifndef FEATURE_CDSMS
        case CAI_MSG_WAITING_REC:
          cmd_ptr->msgs.hdr.cmd = CM_MSG_WAITING_F;
          cmd_ptr->msgs.hdr.last_record = last_rec;
          cmd_ptr->msgs.msgs    =
            ((caii_msg_waiting_rec_type *) rec_ptr)->msg_count;

          /* Move record pointer to next record */
          rec_ptr += sizeof( caii_msg_waiting_rec_type );
          break;
        #endif /* !FEATURE_CDSMS */

        #if defined( FEATURE_IS95B_WLL ) || defined( FEATURE_IS95B_WLL_ONLY )
#error code not present
        #endif /* defined FEATURE_IS95B_WLL || defined FEATURE_IS95B_WLL_ONLY */

        case CAI_LINE_CTRL_REC:
          cmd_ptr->line_ctrl.hdr.cmd = CM_LINE_CTRL_F;
          cmd_ptr->line_ctrl.hdr.last_record = last_rec;
          cmd_ptr->line_ctrl.polarity_included =
            ((caii_line_ctrl_rec_type *) rec_ptr)->pol_inc;
          cmd_ptr->line_ctrl.toggle            =
            ((caii_line_ctrl_rec_type *) rec_ptr)->tgl_mod;
          cmd_ptr->line_ctrl.reverse           =
            ((caii_line_ctrl_rec_type *) rec_ptr)->rev_pol;
          cmd_ptr->line_ctrl.power_denial      =
            ((caii_line_ctrl_rec_type *) rec_ptr)->pow_dtm;

          cmd_ptr->line_ctrl.call_id = call_id;
           /* Move record pointer to next record */
          rec_ptr += sizeof( caii_line_ctrl_rec_type );
          break;


        case CAI_EXT_DISPLAY_REC:
          /* fill out the ui command header */
          cmd_ptr->ext_disp.hdr.cmd = CM_EXT_DISPLAY_F;
          cmd_ptr->ext_disp.hdr.last_record = last_rec;

          cmd_ptr->ext_disp.call_id = call_id;

          /* get the length so we can check to see if we dropped any */
          len = ((caii_ext_disp_type *)rec_ptr)->fix.rec_len;

          /* build the record in the form used by CM */
          if (len != cm_ext_disp_bld_info((caii_ext_disp_type *)rec_ptr,
                                          &(cmd_ptr->ext_disp.disp_info)))
          {
            MSG_HIGH( "Dropped Extended info sub record(s)",0, 0, 0 );
          }
          rec_ptr += len + FPOS(caii_ext_disp_type,var);
          break;


        #ifdef FEATURE_IS2000_REL_A
        case CAI_MC_EXT_DISP_REC:
          /* Multiple-character Ext Display Record */
          mc_ext_disp_ptr = (caii_mc_ext_disp_type *)rec_ptr;
          rec_ptr += FENDPOS(caii_mc_ext_disp_type, num_displays);

          for (idx=0; idx < mc_ext_disp_ptr->num_displays; idx++)
          {
            rec_ptr = (byte *)ALIGN_DWORD((dword) rec_ptr);
            rec_ptr += FPOS(caii_mc_ext_disp_rec_type, recs[mc_ext_disp_ptr->displays[idx]->num_record]);

            for (idx2 = 0; idx2 < mc_ext_disp_ptr->displays[idx]->num_record; idx2++)
            {
              rec_ptr = (byte *)ALIGN_DWORD((dword) rec_ptr);
              rec_ptr += FPOS(caii_mc_ext_disp_rec_info_type, chari[mc_ext_disp_ptr->displays[idx]->recs[idx2]->num_fields]);
            }
          }

          /* We are holding off on the interface to CM/UI until the MC-Ext feature is needed */
          MSG_HIGH("Dropped multiple-character Extended info record(s)", 0,0,0);

          /* Put buffer back on queue */
          cm_mc_rpt_free_buf(cmd_ptr);
          /* Set cmd_ptr to null to indicate no command to send to UI */
          cmd_ptr = NULL;

          break;
        #endif /* FEATURE_IS2000_REL_A */

        #ifdef FEATURE_IS2000_REL_C
        case CAI_ENH_MC_EXT_DISP_REC:
          /* Enhanced Multiple-character Ext Display Record */
          enh_mc_ext_disp_ptr = (caii_enh_mc_ext_disp_type *)rec_ptr;
          rec_ptr += FENDPOS(caii_enh_mc_ext_disp_type, num_displays);

          /* there are num_displays + 1 display records */
          for (idx=0; idx < enh_mc_ext_disp_ptr->num_displays+1; idx++)
          {
            rec_ptr = (byte *)ALIGN_DWORD((dword) rec_ptr);
            rec_ptr += FPOS(caii_enh_mc_ext_disp_rec_type, recs[enh_mc_ext_disp_ptr->displays[idx]->num_record]);

            for (idx2 = 0; idx2 < enh_mc_ext_disp_ptr->displays[idx]->num_record; idx2++)
            {
              rec_ptr = (byte *)ALIGN_DWORD((dword) rec_ptr);

              rec_ptr += FPOS(caii_enh_mc_ext_disp_rec_info_type, chari[enh_mc_ext_disp_ptr->displays[idx]->recs[idx2]->num_fields]);
            }
          }

          /* We are holding off on the interface to CM/UI until the Enhanced MC-Ext feature is needed */
          MSG_HIGH("Dropped Enhanced Multiple-character Extended info record(s)", 0,0,0);

          /* Put buffer back on queue */
          cm_mc_rpt_free_buf(cmd_ptr);
          /* Set cmd_ptr to null to indicate no command to send to UI */
          cmd_ptr = NULL;

          break;
        #endif /* FEATURE_IS2000_REL_C */

        case CAI_REDIR_NUM_REC:

#ifdef FEATURE_REDIRECTING_NUMBER_INFO_REC
          cmd_ptr->redirecting_number.hdr.cmd = CM_REDIRECTING_NUMBER_F;
          cmd_ptr->redirecting_number.hdr.last_record = last_rec;

          cmd_ptr->redirecting_number.call_id = call_id;
          cmd_ptr->redirecting_number.number_type = 
            ((caii_redir_rec_type *) rec_ptr)->fix.number_type;
          cmd_ptr->redirecting_number.number_plan = 
          ((caii_redir_rec_type *) rec_ptr)->fix.number_plan;

          cmd_ptr->redirecting_number.pi_si_included = FALSE;
          cmd_ptr->redirecting_number.redirecting_reason_included = FALSE;

          if (!((caii_redir_rec_type*) rec_ptr)->fix.extension_bit1)
          {
            /* PI and SI are included if extension_bit_1 is 0 */
            cmd_ptr->redirecting_number.pi_si_included = TRUE;

            cmd_ptr->redirecting_number.pi = 
            ((caii_redir_rec_type *) rec_ptr)->fix.pi;

            cmd_ptr->redirecting_number.si = 
            ((caii_redir_rec_type *) rec_ptr)->fix.si;

            if (!((caii_redir_rec_type*) rec_ptr)->fix.extension_bit2)
            {
              /* If extension_bit_1 is 0 and extension_bit_2 is 0, 
                redirecting_reason is included */
              cmd_ptr->redirecting_number.redirecting_reason_included = TRUE;

              cmd_ptr->redirecting_number.redirecting_reason = 
              ((caii_redir_rec_type *) rec_ptr)->fix.redir_reason;
            }
          }

          if (((caii_redir_rec_type *) rec_ptr)->fix.num_char >
              CM_MAX_DATA_DIGITS )
          {
            cmd_ptr->redirecting_number.num_chars = CM_MAX_DATA_DIGITS;
          }
          else
          {
            cmd_ptr->redirecting_number.num_chars =
            ((caii_redir_rec_type *) rec_ptr)->fix.num_char;
          }

          for (idx=0; idx < cmd_ptr->redirecting_number.num_chars; idx++)
          {
            cmd_ptr->redirecting_number.chars[idx] =
            ((caii_redir_rec_type *) rec_ptr)->var[idx];
          }

#else 
          /* Put buffer back on queue */
            cm_mc_rpt_free_buf(cmd_ptr);
          /* Set cmd_ptr to null to indicate no command to send to UI */
          cmd_ptr = NULL;
#endif /* FEATURE_REDIRECTING_NUMBER_INFO_REC */

          /* Move record pointer to next record */
          rec_ptr += FENDPOS( caii_redir_rec_type,
                              var[((caii_redir_rec_type *) rec_ptr)->fix.num_char - 1] );

          break;

        #ifdef FEATURE_PASSPORT_MODE
        case CAI_NTL_SUP_SVC_REC:
        if (mcc_passport_mode_is_jcdma())
        {
          switch (((caii_ntl_sup_svc_type *) rec_ptr)->record_subtype)
          {
            case CAI_CLIR_REC:
              cmd_ptr->nss_clir_rec.hdr.cmd = CM_NSS_CLIR_F;
              cmd_ptr->nss_clir_rec.call_id = call_id;
              cmd_ptr->nss_clir_rec.cause =
                ((caii_clir_sup_svc_type *) rec_ptr)->cause;

              /* Move record pointer to next record */
              rec_ptr += sizeof( caii_clir_sup_svc_type );

              /* Save the CAI_CLIR_REC in the temporary buffer. Please note
               * that we only save the most recently recieved CAI_CLIR_REC.
               */
              if (clir_rec_ptr != NULL)
              {
                /* We already received a cmd_ptr earlier. Now replace the old
                 * one with this new one becuase we only save the most
                 * recently recieved CAI_CLIR_REC. However, before we save
                 * the one, return the buffer that contained the last
                 * CAI_CLIR_REC to the free queue.
                */
                cm_mc_rpt_free_buf(clir_rec_ptr);
              }

              /* Save the current CAI_CLIR_REC */
              clir_rec_ptr = cmd_ptr;

              /* Set cmd_ptr to null to indicate no command to send to CM */
              cmd_ptr = NULL;
              break;

            case CAI_RELEASE_REC:
              cmd_ptr->hdr.cmd = CM_NSS_REL_F;
              cmd_ptr->nss_rel_rec.hdr.last_record = last_rec;
              cmd_ptr->nss_rel_rec.call_id = call_id;

              /* Move record pointer to next record */
              rec_ptr += sizeof( caii_release_sup_svc_type );
              break;

            case CAI_AUDIO_CONTROL_REC:
              cmd_ptr->nss_aud_ctrl_rec.hdr.cmd = CM_NSS_AUD_CTRL_F;
              cmd_ptr->nss_aud_ctrl_rec.hdr.last_record = last_rec;
              cmd_ptr->nss_aud_ctrl_rec.call_id = call_id;
              cmd_ptr->nss_aud_ctrl_rec.down_link =
                ((caii_audio_ctl_sup_svc_type *) rec_ptr)->down_link;
              cmd_ptr->nss_aud_ctrl_rec.up_link =
                ((caii_audio_ctl_sup_svc_type *) rec_ptr)->up_link;

              /* Move record pointer to next record */
              rec_ptr += sizeof( caii_audio_ctl_sup_svc_type );
              break;

            default:
              cm_mc_rpt_free_buf(cmd_ptr);
              /* Set cmd_ptr to null to indicate no command to send to UI */
              cmd_ptr = NULL;
              break;
          }
          break;
        }
        #endif /* FEATURE_PASSPORT_MODE */

        default:
          /* ------------------------
          ** Unknown info record type
          ** ------------------------ */
          MSG_HIGH( "Unknown info rec %d",
                    ((caii_rec_hdr_type *) rec_ptr)->record_type, 0, 0 );
          cm_mc_rpt_free_buf(cmd_ptr);
          /* Set cmd_ptr to null to indicate no command to send to UI */
          cmd_ptr = NULL;
          /* Increment pointer so we point to next record */
          rec_ptr += sizeof( caii_rec_hdr_type );
          break;
      } /* switch */

      if (cmd_ptr != NULL)
      {
        /* Send command to UI */
        mcc_cm_mc_rpt( cmd_ptr );
      }
    }

    rec_ptr = (byte*) ALIGN_DWORD((dword)rec_ptr);

  } /* for */

  if (((msg_type == CAI_ALERT_MSG)
       #ifdef FEATURE_IS2000_REL_A
       || (msg_type == CAI_EXT_ALERT_W_INFO_MSG)
       #endif
      )
      && (!signal_rec_rxed))
  {
    /* -------------------------------------------------------------------
    ** If the Alert With Information Message contains a Signal information
    ** record, the mobile station should alert the user in accordance with
    ** the Signal information record; otherwise, the mobile station should
    ** use standard alert as defined in 7.7.5.5.
    ** ------------------------------------------------------------------- */
    if ((cmd_ptr = cm_mc_rpt_get_buf_else_err_fatal()) != NULL)
    {
      cmd_ptr->signal.hdr.cmd      = CM_SIGNAL_F;
      cmd_ptr->signal.signal_type  = CAI_SIG_IS54B;
      cmd_ptr->signal.alert_pitch  = CAI_PITCH_MED;
      cmd_ptr->signal.signal       = CAI_IS54B_L;

      cmd_ptr->signal.call_id = call_id;

      #ifdef FEATURE_PASSPORT_MODE
      if(mcc_passport_mode_is_jcdma())
      {      
        /* If T53, set the last_rec to TRUE only if we did *not* receive
         * any part of Caller ID information. If we did receive the
         * the caller ID information, then set the last_rec to FALSE.
         * We set the last_rec at a later time when caller ID
         * information is sent to CM
         */
        if (calling_rec_ptr == NULL && clir_rec_ptr == NULL)
        {
          cmd_ptr->signal.hdr.last_record = TRUE;
        }
        else
        {
          cmd_ptr->signal.hdr.last_record = FALSE;
        }
      }
      else
      #endif
      {
       cmd_ptr->signal.hdr.last_record = TRUE;
      }
      

      mcc_cm_mc_rpt( cmd_ptr );
    }
    else
    {
      ERR_FATAL( "No buffers on cm_mc_rpt_free_q", 0, 0, 0 ); 
    }
  } /* end if ((msg_type == CAI_ALERT_MSG) && (!signal_rec_rxed)) */

  #ifdef FEATURE_PASSPORT_MODE
  if(mcc_passport_mode_is_jcdma())
  {  
    /* -------------------------------------------------------------------
    ** Calling Party Number and CLIR haven't notified above until last
    ** record because UI needs both records to dislpay calling#, then
    ** we should notify these records here.
    ** -----------------------------------------------------------------*/

    if (calling_rec_ptr != NULL && clir_rec_ptr != NULL)
    {
      /* In this case, both the records are to be sent. Set the clir_rec_ptr
       * to be the last_record.
       */

      calling_rec_ptr->info_hdr.last_record = FALSE;
      clir_rec_ptr->info_hdr.last_record = TRUE;

      /* send the records to CM */
      mcc_cm_mc_rpt(calling_rec_ptr);
      mcc_cm_mc_rpt(clir_rec_ptr);
    }
    else if(calling_rec_ptr != NULL )
    {
      /* In this case, calling_rec_ptr is the last one
       */
     calling_rec_ptr->info_hdr.last_record = TRUE;

      /* send the record to CM */
      mcc_cm_mc_rpt(calling_rec_ptr);

    }
    else if(clir_rec_ptr != NULL)
    {
     /* In this case, calling_rec_ptr is the last one
      */
     clir_rec_ptr->info_hdr.last_record = TRUE;
  
     /* send the record to CM */
     mcc_cm_mc_rpt(clir_rec_ptr);

    }
    else
    {
     /* we did not recieve either, send nothing */
    }
  }
  #endif /* FEATURE_PASSPORT_MODE */

  return (ret_val);
 
} /* end mcc_process_info_records() */

/*===========================================================================

FUNCTION mcc_check_alert_signal_info

DESCRIPTION
  This function checks whether the info record contains a signal info record
  or not and it also checks the whether the signal type is set to 1 or 2
  if the info record contains a signal info record

DEPENDENCIES
  None.

RETURN VALUE
  TRUE if an Alert With Information Message contains a signal record with
  SIGNAL_TYPE field set to 1 or 2. or it does not contain a signal information record
  FALSE otherwise. (This information is needed during the Conversation State).

SIDE EFFECTS
  None.

===========================================================================*/
boolean mcc_check_alert_signal_info
(
  byte msg_type,
    /* Message type */
  byte num_recs,
    /* Number of information records in message */
  byte *rec_ptr
    /* Pointer to information records */
)
{
  boolean ret_val = TRUE;
    /* Value to return to caller */
  int i;

  #ifdef FEATURE_IS2000_REL_A
  word idx, idx2;
  caii_mc_ext_disp_type * mc_ext_disp_ptr;
  #endif

  #ifdef FEATURE_IS2000_REL_C
  caii_enh_mc_ext_disp_type * enh_mc_ext_disp_ptr;
  #endif

  /*lint -e413*/
  /* Suppress lint errors related to use of the FENDPOS macro. */

  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  for (i=0; i < num_recs; i++)
  {
      switch (((caii_rec_hdr_type *) rec_ptr)->record_type)
      {
        case CAI_DISPLAY_REC:

          /* Move record pointer to next record */
          rec_ptr += FENDPOS( caii_display_rec_type,
            var[((caii_display_rec_type *) rec_ptr)->fix.num_char - 1] );
          break;

        case CAI_CALLED_REC:
          /* Move record pointer to next record */
          rec_ptr += FENDPOS( caii_called_rec_type,
            var[((caii_called_rec_type *) rec_ptr)->fix.num_char - 1] );
          break;

        case CAI_CALLING_REC:

          /* Move record pointer to next record */
          rec_ptr += FENDPOS( caii_calling_rec_type,
            var[((caii_calling_rec_type *) rec_ptr)->fix.num_char - 1] );
          break;

        case CAI_CONNECT_REC:

          /* Move record pointer to next record */
          rec_ptr += FENDPOS( caii_calling_rec_type,
            var[((caii_calling_rec_type *) rec_ptr)->fix.num_char - 1] );
          break;

        case CAI_SIGNAL_REC:

          if( ( ((caii_signal_rec_type *) rec_ptr)->signal_type != CAI_SIG_ISDN ) &&
              ( ((caii_signal_rec_type *) rec_ptr)->signal_type != CAI_SIG_IS54B )  )
          {
            ret_val = FALSE;
          }

          /* Move record pointer to next record */
          rec_ptr += sizeof( caii_signal_rec_type );
          break;

        case CAI_MSG_WAITING_REC:

          /* Move record pointer to next record */
          rec_ptr += sizeof( caii_msg_waiting_rec_type );
          break;

        #if defined( FEATURE_IS95B_WLL ) || defined( FEATURE_IS95B_WLL_ONLY )
#error code not present
        #endif /*defined FEATURE_IS95B_WLL  || defined FEATURE_IS95B_WLL_ONLY*/

        case CAI_LINE_CTRL_REC:
           /* Move record pointer to next record */
          rec_ptr += sizeof( caii_line_ctrl_rec_type );
          break;

        case CAI_REDIR_NUM_REC:
          rec_ptr += FENDPOS( caii_redir_rec_type,
                              var[((caii_redir_rec_type *) rec_ptr)->fix.num_char - 1] );
          break;

        case CAI_EXT_DISPLAY_REC:
          rec_ptr += ((caii_ext_disp_type *)rec_ptr)->fix.rec_len + FPOS(caii_ext_disp_type,var);
          break;

        #ifdef FEATURE_IS2000_REL_A
        case CAI_MC_EXT_DISP_REC:
          mc_ext_disp_ptr = (caii_mc_ext_disp_type *)rec_ptr;
          rec_ptr += FENDPOS(caii_mc_ext_disp_type, num_displays);

          for (idx=0; idx < mc_ext_disp_ptr->num_displays; idx++)
          {
            rec_ptr = (byte *)ALIGN_DWORD((dword) rec_ptr);
            rec_ptr += FPOS(caii_mc_ext_disp_rec_type, recs[mc_ext_disp_ptr->displays[idx]->num_record]);

            for (idx2 = 0; idx2 < mc_ext_disp_ptr->displays[idx]->num_record; idx2++)
            {
              rec_ptr = (byte *)ALIGN_DWORD((dword) rec_ptr);
              rec_ptr += FPOS(caii_mc_ext_disp_rec_info_type, chari[mc_ext_disp_ptr->displays[idx]->recs[idx2]->num_fields]);
            }
          }

          break;
        #endif /* FEATURE_IS2000_REL_A */

        #ifdef FEATURE_IS2000_REL_C
        case CAI_ENH_MC_EXT_DISP_REC:
          enh_mc_ext_disp_ptr = (caii_enh_mc_ext_disp_type *)rec_ptr;
          rec_ptr += FENDPOS(caii_enh_mc_ext_disp_type, num_displays);

          /* there are num_displays + 1 display records */
          for (idx=0; idx < enh_mc_ext_disp_ptr->num_displays+1; idx++)
          {
            rec_ptr = (byte *)ALIGN_DWORD((dword) rec_ptr);
            rec_ptr += FPOS(caii_enh_mc_ext_disp_rec_type, recs[enh_mc_ext_disp_ptr->displays[idx]->num_record]);

            for (idx2 = 0; idx2 < enh_mc_ext_disp_ptr->displays[idx]->num_record; idx2++)
            {
              rec_ptr = (byte *)ALIGN_DWORD((dword) rec_ptr);

              rec_ptr += FPOS(caii_enh_mc_ext_disp_rec_info_type, chari[enh_mc_ext_disp_ptr->displays[idx]->recs[idx2]->num_fields]);
            }
          }
          break;
        #endif /* FEATURE_IS2000_REL_C */

        #ifdef FEATURE_T53
        case CAI_NTL_SUP_SVC_REC:
          if (((caii_ntl_sup_svc_type *) rec_ptr)->record_subtype == CAI_CLIR_REC)
          {
            rec_ptr += sizeof( caii_clir_sup_svc_type );
          }
          else if (((caii_ntl_sup_svc_type *) rec_ptr)->record_subtype == CAI_RELEASE_REC)
          {
            rec_ptr += sizeof( caii_release_sup_svc_type );
          }
          else if (((caii_ntl_sup_svc_type *) rec_ptr)->record_subtype == CAI_AUDIO_CONTROL_REC)
          {
            rec_ptr += sizeof( caii_audio_ctl_sup_svc_type );
          }
          break;
        #endif /* FEATURE_T53 */

        default:
          /* ------------------------
          ** Unknown info record type
          ** ------------------------ */
          rec_ptr += sizeof( caii_rec_hdr_type );
          break;
      } /* switch */

      rec_ptr = (byte*) ALIGN_DWORD((dword)rec_ptr);

  } /* for */

  return (ret_val);
 
} /* end mcc_check_alert_signal_info() */ /*lint !e715*/

/*lint +e826 */

/*===========================================================================

FUNCTION MCC_GET_STATE

DESCRIPTION
  Returns the current state of the phone.

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.
===========================================================================*/

byte mcc_get_state
( 
  void 
)
{
  byte state;
    /* Current state of the phone */

  #ifndef FEATURE_IS2000_REL_A
  word cc_substate;
  #endif

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  switch (cdma.curr_state)
  {
    case CDMA_PIL_ACQ:
    case CDMA_SYNC_ACQ:
    case CDMA_SYNC_MSG:
    case CDMA_JUMP:
    case CDMA_UNJUMP:
      state = DIAG_D_INIT_S;
    break;

    case CDMA_IDLE_INIT:
    case CDMA_IDLE_MSG_PENDING:
    case CDMA_UNSLOTTED:
    case CDMA_SLOTTED:
      state = DIAG_D_IDLE_S;
    break;

    case CDMA_OVHD:
    case CDMA_UPDATE_OVH:
      state = DIAG_D_OVHD_S;
    break;

    case CDMA_ORIG:
      state = DIAG_D_ORIG_S;
    break;

    case CDMA_PAGE_RES:
      state = DIAG_D_PAGE_RES_S;
    break;

    case CDMA_ORD_RES:
      state = DIAG_D_ORD_RES_S;
    break;

    case CDMA_REG:
      state = DIAG_D_REG_S;
    break;

    case CDMA_MSG:
      state = DIAG_D_MSG_S;
    break;

    case CDMA_TCI:
      state = DIAG_D_VC_INIT_S;
    break;

    case CDMA_TRAFFIC:
    {
      #ifndef FEATURE_IS2000_REL_A
      cc_substate = mccccim_get_cc_substate(0);

      switch(cc_substate)
      {
        case CDMA_CC_WFO:
          state = DIAG_D_WFO_S;
          break;
        case CDMA_CC_WFA:
          state = DIAG_D_WFA_S;
          break;
        case CDMA_CC_CONV:
          state = DIAG_D_CONV_S;
          break;
        case CDMA_CC_RELEASE:
        default:
          state = DIAG_D_RELEASE_S;
          break;

      }
      #else
      state = DIAG_D_CONV_S;
      #endif

    }
    break;

    case CDMA_RELEASE:
      state = DIAG_D_RELEASE_S;
    break;

    default:
      state = DIAG_NO_SVC_S;
    break;
  }

  return (state);

} /* mcc_get_state */

/*====================================================================

FUNCTION MCC_DEFAULT_IMSI_S1

DESCRIPTION
  This routine will compute default IMSI_S1 value as per IS-95B section 6.3.
  The algorithm is copied from uinv_default_imsi_s1.

DEPENDENCIES
  This function requires that the esn has been loaded into cdma.esn structure.

RETURN VALUE
  This function returns the encoded value of IMSI_S1 with the four least
  significant digits set to ESNp, converted directly from binary to decimal,
  modulo 10000. The other digits are set to zero.

SIDE EFFECTS

====================================================================*/

dword mcc_default_imsi_s1
(
  void
)
{
  word zero;        /* Encoding of three zero digits */
  word fourth;      /* Fourth form last decimal digit of the ESN */
  word last3;       /* Last three decimal digits of the ESN */

  /* Encode digits as per JSTD-008 section 2.3.1.1 */
  zero = 1110 - 111;
  last3 = cdma.esn % 1000;
  last3 += ((last3 / 100) == 0)? 1000 : 0;
  last3 += (((last3 % 100) / 10) == 0)? 100 : 0;
  last3 += ((last3 % 10) == 0)? 10 : 0;
  last3 -= 111;
  fourth = ((cdma.esn % 10000) / 1000);  /* In range 0-9 */

  if (fourth == 0)  {
    fourth = 10;
  }

  /* Concatenate results and return 24 bit value for imsi_s1 */
  /* Example: esn = 120406
  **   imsi_s1 = 000  0  406
  ** encodes to -> 999 10 395
  **      in hex = 3e7 a  18b
  **   in binary = 1111100111 1010 0110001011
  */
  return (((dword) zero << 14) | (fourth << 10) | last3);

} /* mcc_default_imsi_s1 */

/*===========================================================================

FUNCTION MCC_GET_AUTH_IMSI_S1

DESCRIPTION
  This function returns correct imsi_s1 for authentication purposes.

DEPENDENCIES
  None

RETURN VALUE
  IMSI_M_S1 if IMSI_M is programmed. Otherwise, return IMSI_T_S1 (See
  IS-95B section 6.3.12.1 ).

SIDE EFFECTS
  None

===========================================================================*/

dword mcc_get_auth_imsi_s1
(
  void
)
{
  /* Check to see if IMSI_M is programmed */
  if( cdma.imsi_m_s1 != mcc_default_imsi_s1() ||
      cdma.imsi_m_s2 != IMSI_S2_ZERO          ||
      cdma.imsi_m_mcc != IMSI_MCC_ZERO        ||
      cdma.imsi_m_11_12 != IMSI_11_12_ZERO )
  {
    /* Yes, it is programmed, return IMSI_M_S1 then */
    return cdma.imsi_m_s1;
  }
  else
  {
    /* IMSI_M not programmed, return IMSI_T_S1 instead */
    return cdma.imsi_t_s1;
  }

}  /* mcc_get_auth_imsi_s1 */

/*===========================================================================

FUNCTION MCC_GET_AUTH_IMSI_S2

DESCRIPTION
  This function returns correct imsi_s2 for authentication purposes.

DEPENDENCIES
  None

RETURN VALUE
  IMSI_M_S2 if IMSI_M is programmed. Otherwise, return IMSI_T_S2 (See
  IS-95B section 6.3.12.1 ).

SIDE EFFECTS
  None

===========================================================================*/

word mcc_get_auth_imsi_s2
(
  void
)
{
  /* Check to see if IMSI_M is programmed */
  if( cdma.imsi_m_s1 != mcc_default_imsi_s1() ||
      cdma.imsi_m_s2 != IMSI_S2_ZERO          ||
      cdma.imsi_m_mcc != IMSI_MCC_ZERO        ||
      cdma.imsi_m_11_12 != IMSI_11_12_ZERO )
  {
    /* Yes, it is programmed, return IMSI_M_S1 then */
    return cdma.imsi_m_s2;
  }
  else
  {
    /* IMSI_M not programmed, return IMSI_T_S1 instead */
    return cdma.imsi_t_s2;
  }

}  /* mcc_get_auth_imsi_s2() */

/*===========================================================================

FUNCTION MCC_SEND_SRCH_PARMS

DESCRIPTION
  This procedure sends the updated search parameters to the searcher.

DEPENDENCIES
  mcc_ho_real must have been updated appropriately

RETURN VALUE
  None.

SIDE EFFECTS
  The search parmameters in the searcher will be changed.

===========================================================================*/

void mcc_send_srch_parms( )
{
  /* send search command with new search parameters */
  mcc_srch_buf.parm.hdr.cmd       = SRCH_PARM_F;

  #ifdef FEATURE_FACTORY_TESTMODE
  #ifdef FEATURE_PLT
#error code not present
  #else /* FEATURE_PLT */
  if (ftm_mode == FTM_MODE)
  {
    ftm_set_srch_parms( &mcc_srch_buf.parm );
  }
  else
  #endif /* FEATURE_PLT */
  #endif /* FEATURE_FACTORY_TESTMODE */

  {
    /* Fill in the soft handoff parameters for search */
    mcc_srch_buf.parm.win_a         = mcc_ho_real.win_a;
    mcc_srch_buf.parm.win_n         = mcc_ho_real.win_n;
    mcc_srch_buf.parm.win_r         = mcc_ho_real.win_r;
    mcc_srch_buf.parm.t_add         = mcc_ho_real.t_add;
    mcc_srch_buf.parm.t_drop        = mcc_ho_real.t_drop;
    mcc_srch_buf.parm.t_comp        = mcc_ho_real.t_comp;
    mcc_srch_buf.parm.t_tdrop       = mcc_ho_real.t_tdrop;
    mcc_srch_buf.parm.nghbr_max_age = mcc_ho_real.nghbr_max_age;

    mcc_srch_buf.parm.soft_slope     = mcc_ho_real.soft_slope ;
    mcc_srch_buf.parm.add_intercept  = mcc_ho_real.add_intercept;
    mcc_srch_buf.parm.drop_intercept = mcc_ho_real.drop_intercept;

    if ((MAIN_STATE(cdma.curr_state) == CDMA_TC)
#ifdef FEATURE_VOIP_ON_HDR_HANDOFF_TO_1X
#error code not present
#endif
       )
    {
      /* If we are on the Traffic Channel or doing a VOIP-1X handoff,
         use the cdma variable to get the idle pgslot, because we 
         may have skipped Idle state, so the pgslot from Idle state 
         may not be accurate. */
      mcc_srch_buf.parm.pgslot             = cdma.idle_pgslot;

      /* The mobile has no ovhd info on TCH, so set the following fields
         to 0. */
      mcc_srch_buf.parm.sys_reselect_included = 0;
      mcc_srch_buf.parm.slot_cycle_index = 0;
      mcc_srch_buf.parm.bcast_index  = 0;
      mcc_srch_buf.parm.idle_mode = CAI_NORMAL_IDLE_MODE;
    }
    else
    {
      mcc_srch_buf.parm.pgslot             = mcc_get_idle_pgslot();
      mcc_srch_buf.parm.slot_cycle_index   = mccidl_get_slot_cycle_index_s();

      if (P_REV_IS_4_OR_GREATER)
      {
        mcc_srch_buf.parm.sys_reselect_included = cur_bs_ptr->csp.esp.reselect_included;
        mcc_srch_buf.parm.ec_thresh             = cur_bs_ptr->csp.esp.ec_thresh;
        mcc_srch_buf.parm.ec_io_thresh          = cur_bs_ptr->csp.esp.ec_io_thresh;
      }
      else
      {
        /* disable system reselect thresholds */
        mcc_srch_buf.parm.sys_reselect_included = 0;
      }

#ifdef FEATURE_BROADCAST_SMS
      if (mccbcsms_is_enabled())
      {
        mcc_srch_buf.parm.bcast_index  = cur_bs_ptr->csp.esp.bcast_index;
      }
      else
#endif /* FEATURE_BROADCAST_SMS */

      {
        mcc_srch_buf.parm.bcast_index  = 0;
      }

      /* For pseudo-idle mode operation. Searcher determines the SCI to be used 
         and ignores SCI specified in overheads. */
      if ( cdma.entry_ptr->entry == MCC_PSEUDO_IDLE )
      {
        mcc_srch_buf.parm.idle_mode = CAI_PSEUDO_IDLE_MODE;
      }
      else
      {
        mcc_srch_buf.parm.idle_mode = CAI_NORMAL_IDLE_MODE;
      }  
    }
  }

  MSG_4( MSG_SSID_1X_DCP, MSG_LEGACY_MED,
    "Srch parm: sci %d pgslot %d bcast_index %d idle_mode %d", 
    mcc_srch_buf.parm.slot_cycle_index, mcc_srch_buf.parm.pgslot, 
    mcc_srch_buf.parm.bcast_index, mcc_srch_buf.parm.idle_mode );

  mcc_srch_cmd( &mcc_srch_buf );  /* send command to searcher */

} /* mcc_send_srch_parms */

/*===========================================================================

FUNCTION mcc_set_layer2_ack

DESCRIPTION
  Set Layer2 ack info.

DEPENDENCIES

RETURN VALUE


SIDE EFFECTS

===========================================================================*/

void mcc_set_layer2_ack
(
  byte ack_seq,
  byte ack_type,
  byte valid_ack
)
{
  mcc_layer2.ack_seq   = ack_seq;
  mcc_layer2.ack_type  = ack_type;
  mcc_layer2.valid_ack = valid_ack;

} /* mcc_set_layer2_ack */

#if (defined (FEATURE_DH) && defined (FEATURE_DH_EXP))
/*===========================================================================

FUNCTION MCC_QUEUE_DH_RPT

DESCRIPTION
  This function enqueues a specified report from the DH Task
  to the CDMA subtask of the Main Control Task.

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.
===========================================================================*/

void mcc_queue_dh_rpt
(
  dh_rpt_type  *dh_rpt_buf_ptr
    /* Pointer to dh report buffer filled in by dh task */
)
{
  dh_rpt_type  *rpt_ptr;
    /* Pointer to buffer from mc_dh_free_q to be queued on mc_dh_q */

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  if ((rpt_ptr = (dh_rpt_type *)q_get( &mc_dh_free_q )) != NULL)
  {
    /* Copy report information into free buffer */
    *rpt_ptr = *dh_rpt_buf_ptr;
    rpt_ptr->rpt_hdr.done_q_ptr = &mc_dh_free_q;

    /* Queue the report buffer */
    (void) q_link( rpt_ptr, &rpt_ptr->rpt_hdr.link );  /* init link */
    q_put( &mc_dh_q, &rpt_ptr->rpt_hdr.link ); /* and queue it */

    /* Signal CDMA subtask */
    (void) rex_set_sigs( &mc_tcb, MCC_Q_SIG );
  }
  else
  {
    ERR_FATAL( "No free buffers on mc_dh_free_q", 0, 0, 0 );
  }
 
} /* mcc_queue_dh_rpt */
#endif /* FEATURE_DH */

#if (defined(FEATURE_CDSMS) || defined (FEATURE_DBM))
/*===========================================================================

FUNCTION MCC_SEND_MO_DBM_STATUS

DESCRIPTION
  This function sends a status report to the CM task for a mobile originated
  data burst message.

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/

void mcc_send_mo_dbm_status
(
  uint8 burst_type,
    /* Data Burst Message type, currently, it supports only SMS and PD */
  cm_dbm_mo_status_e_type err_code
    /* Mobile originated data burst message error code */
)
{
  cm_mc_rpt_type *cmd_ptr;    /* Pointer to handset command */

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  if ((cmd_ptr = cm_mc_rpt_get_buf_else_err_fatal()) != NULL)
  {
    cmd_ptr->mo_dbm_status.hdr.cmd = CM_MO_DBM_STATUS_F;
    cmd_ptr->mo_dbm_status.msg_type = burst_type;
    cmd_ptr->mo_dbm_status.status = err_code;
    cmd_ptr->mo_dbm_status.burst_overflow_size = mcc_burst_overflow_size;
    mcc_cm_mc_rpt( cmd_ptr );

    /* Reset the flag indicator of waiting for layer 2 ack. */
    cdma.dbm_wait_for_l2_ack = FALSE;

  }
  else
  {
    MSG_HIGH("No free buffers on cm_mc_rpt_free_q",0,0,0 );
  }

} /* mcc_send_mo_dbm_status */

/*===========================================================================

FUNCTION MCC_CHECK_MO_DBM_STATUS

DESCRIPTION
  This function checks if the MC is waiting for a layer2 ack for a specific
  data burst message (SMS or PD), and if so, sends out a status report to CM
  task.

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/

void mcc_check_mo_dbm_status
(
  cm_dbm_mo_status_e_type err_code
    /* Mobile originated data burst message error code */
)
{
  if (cdma.dbm_wait_for_l2_ack)
  {
    if (cdma.mo_dbm_type == CAI_SHORT_MSG_SERVICES)
    {
      /* A layer2 ack for SMS message is pending. Report back the status
       * to CM, and reset the flag */
      MSG_MED(" SMS status: %d", err_code, 0, 0);
      mcc_send_mo_dbm_status((byte) CAI_SHORT_MSG_SERVICES, err_code);

      if (err_code == CM_DBM_MO_OK)
      {
        /* Events report: SMS sent */
        event_report(EVENT_SMS_SENT);
      }
    }

    if (cdma.mo_dbm_type == CAI_POSITION_DET)
    {
      /* A layer2 ack for PD message is pending. Report back the status
       * to CM and reset the flag */
      MSG_MED(" PD status: %d", err_code, 0, 0);
      mcc_send_mo_dbm_status((byte) CAI_POSITION_DET, err_code);
    }

    if (cdma.mo_dbm_type == CAI_SHORT_DATA_BURST)
    {
      /* A layer2 ack for PD message is pending. Report back the status
       * to CM and reset the flag */
      MSG_MED(" SDB status: %d", err_code, 0, 0);
      mcc_send_mo_dbm_status((byte) CAI_SHORT_DATA_BURST, err_code);
    }

    cdma.dbm_wait_for_l2_ack = FALSE;
  }

} /* mcc_check_mo_dbm_status */

/*===========================================================================

FUNCTION MCC_SEND_MT_DBM_DATA

DESCRIPTION
  This function sends the data to the CM task for a mobile terminated
  data burst message.

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/

void mcc_send_mt_dbm_data
(
  uint8 dbm_burst_type,
    /* The data burst type */
  uint8* data,
    /* Pointer to the paylaod */
  uint8 data_len
    /* The number of bytes in the payload */
)
{
  cm_mc_rpt_type *cmd_ptr;    /* Pointer to handset command */

  /* Queue the message on CM's report queue */
  if ((cmd_ptr = cm_mc_rpt_get_buf_else_err_fatal()) != NULL)
  {
    cmd_ptr->mt_dbm_data.hdr.cmd    = CM_MT_DBM_DATA_F;

    cmd_ptr->mt_dbm_data.burst_type = dbm_burst_type;

    /* added the following line according to CM interface change */
    /* the burst_source need be set to be CM_DBM_FROM_1X here    */
    cmd_ptr->mt_dbm_data.burst_source = CM_DBM_FROM_1X;

    cmd_ptr->mt_dbm_data.num_bytes  = data_len;
    /*lint -e668*/
    memcpy(cmd_ptr->mt_dbm_data.payload, data, data_len);
    /*lint +e668*/

    mcc_cm_mc_rpt( cmd_ptr );
  }
  else
  {
    MSG_HIGH("No free buffers on cm_mc_rpt_free_q",0,0,0 );
  }
 
} /* mcc_send_mt_dbm_data */
#endif /* FEATURE_CDSMS || FEATURE_DBM */

/*===========================================================================

FUNCTION MCC_SEND_ORIG_FAILURE

DESCRIPTION
  This function sends a report to the CM task for a mobile originated SMS.

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  cdma.call_orig and cdma.otasp_call will be reset to FALSE.

===========================================================================*/

void mcc_send_orig_failure
(
  cm_call_orig_fail_e_type err_code,
    /* Error code for the orig failure */
  byte call_id,
    /* Call ID of the failed call */
  cm_call_orig_fail_layer_e_type layer
    /* Layer that the error occurred in */
)
{
  cm_mc_rpt_type *cmd_ptr;    /* Pointer to handset command */

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  /* Start reorder tone */
  if ((cmd_ptr = cm_mc_rpt_get_buf_else_err_fatal()) != NULL)
  {
    cmd_ptr->orig_fail.hdr.cmd = CM_CALL_ORIG_FAIL_F;
    cmd_ptr->orig_fail.code = err_code;
    cmd_ptr->orig_fail.call_id = call_id;
    cmd_ptr->orig_fail.layer = layer;
    mcc_cm_mc_rpt( cmd_ptr );
  }
  else
  {
    MSG_HIGH("No free buffers on cm_mc_rpt_free_q",0,0,0 );
  }

} /* mcc_send_orig_failure */

/*===========================================================================

FUNCTION MCC_UPDATE_BS_P_REV

DESCRIPTION
   Updates internal data and report events if necessary when BS _PREV is received.

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None

===========================================================================*/

void mcc_update_bs_p_rev
(
  uint8 bs_p_rev
)
{
  db_items_value_type db_item;
  uint8 tmp_p_rev;

  // First get old BS P_REV and see if it is changing
  db_get( DB_BS_P_REV, &db_item );

  if ( db_item.bs_p_rev != bs_p_rev )
  {
    // Generate new p_rev_in_use and update if necessary
    tmp_p_rev = (cdma.mob_cai_rev > bs_p_rev) ? bs_p_rev : cdma.mob_cai_rev;
    if (cdma.p_rev_in_use != tmp_p_rev)
    {
      // Update p_rev_in_use here.  Since we don't support P_REV 5, we need to
      // convert p_rev_in_use to 4 if it's 5
      if (tmp_p_rev == P_REV_IS95B + 1)
      {
        cdma.p_rev_in_use = P_REV_IS95B;
      }
      else
      {
        cdma.p_rev_in_use = tmp_p_rev;
      }

      // Now report the p_rev_in_use change
      event_report_payload(EVENT_P_REV_IN_USE_CHANGE,
        sizeof(event_p_rev_in_use_change_type), &cdma.p_rev_in_use );
      MSG_MED( "MS P_REV_IN_USE = %d", cdma.p_rev_in_use ,0,0);
    }

    // Now update BS_P_REV in db item and report the event
    db_item.bs_p_rev = bs_p_rev;
    db_put( DB_BS_P_REV, &db_item );
    event_report_payload(EVENT_BS_P_REV_CHANGE,
      sizeof(event_bs_p_rev_change_type), &bs_p_rev);
  }

} /* mcc_update_bs_p_rev */

/*===========================================================================

FUNCTION reg_retry_delay

DESCRIPTION
  This function binds a retry delay function to a function pointer. If an old
  retry delay function already exists, it overwrites it.

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/

void reg_retry_delay
(
  void (*ds_retry_delay_inform)(retry_delay_type retry_parm)
)
{
  retry_delay_inform = ds_retry_delay_inform;

} /* mccsch_reg_retry_delay */

/*===========================================================================

FUNCTION update_retry_delay

DESCRIPTION
  This function informs DS the latest retry delay duration by calling DS's
  registered retry delay processing function.  It uses the current system
  time and given retry delay to compute the actual retry timer duration in
  unit of milliseconds.

  ORIG retry timer duration = (time left before the next 80ms boundary +
  (time unit is 1 min)         least significant 7 bits of retry delay * 60000) ms
                                          or
  ORIG retry timer duration = (time left before the next 80ms boundary +
  (time unit is 1 sec)         retry delay * 1000) ms

  RRM/SCRM retry timer duration = (time left before the next 80ms boundary +
                               retry delay * 320) ms

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/

void update_retry_delay
(
  cai_retry_order_type  retry_type, /* Origination, Resource Request or SCRM */
  uint8 retry_delay
)
{
  qword             curr_time;  /* Current time, in 20 ms unit */
  uint16            time_left;  /* Time left before next 80 ms boundary, in 20 ms unit */
  retry_delay_type  rd;         /* Retry delay info */
  boolean           bad_type = FALSE; // Indicate if retry type is bad

  /* See if DS wants to be informed of retry delay changes */
  if (retry_delay_inform)
  {
    rd.retry_type = retry_type;

    /* Clear retry delay */
    if ((rd.retry_type == CAI_RETRY_CLR_ALL) || (retry_delay == 0))
    {
      rd.infinite_delay = FALSE;
      rd.retry_delay_duration = 0;

      /* Inform DS to clear all retry delay */
      retry_delay_inform(rd);
      MSG_MED("Clear retry delay %d", rd.retry_type, 0, 0);
    }

    /* Infinite retry delay */
    else if ( (rd.retry_type == CAI_RETRY_RESA || rd.retry_type == CAI_RETRY_SCRM)
               && (retry_delay == 0xff) )
    {
      #if defined(FEATURE_IS2000_SCH) || defined(FEATURE_IS2000_CHS)
      rd.infinite_delay = TRUE;
      rd.retry_delay_duration = 0xffff;
      /* Inform DS to stop SCRM/RRM */
      retry_delay_inform(rd);
      MSG_MED("Retry type %d set to infinite", rd.retry_type, 0, 0);
      #else
      bad_type = TRUE;
      #endif // defined(FEATURE_IS2000_SCH) || defined(FEATURE_IS2000_CHS)
    }

    /* Convert retry delay into retry timer duration in unit of ms */
    else
    {
      rd.infinite_delay = FALSE;

      /* Get current time in 20 ms units */
      ts_get_20ms_frame_offset_time(curr_time);

      /* Convert current time into 80 ms unit. qw_div() returns the
       remainder of the division, */

      /* -----------------------------------------------------------------
      ** Compute time left in 20 ms unit before the next system time 80 ms
      ** boundary
      ** ----------------------------------------------------------------- */
      time_left =  4 - qw_div(curr_time, curr_time, 4U);

      switch (rd.retry_type)
      {
        /* Origination retry delay */
        case CAI_RETRY_ORIG:

        #ifdef FEATURE_IS2000_REL_D
#error code not present
        #endif /* FEATURE_IS2000_REL_D */
        {
          /* Time unit is 1 min */
          if (retry_delay & 0x80)
          {
            /* Compute actual retry timer duration */
            rd.retry_delay_duration = time_left * 20 + (retry_delay & 0x7f) * 60000;
          }
          /* Time unit is 1 sec */
          else
          {
            /* Compute actual retry timer duration */
            rd.retry_delay_duration = time_left * 20 + retry_delay * 1000;
          }

          /* Inform DS the ORIG or/and SDB retry delay */
          retry_delay_inform(rd);
          MSG_MED("ORIG or/and  SDB retry delay=%d ms", rd.retry_delay_duration,
            0, 0);
          break;
        }
        case CAI_RETRY_SCRM:  // SCRM retry delay
        case CAI_RETRY_RESA:  // RRM retry delay
        {
          #if defined(FEATURE_IS2000_SCH) || defined(FEATURE_IS2000_CHS)
          /* Compute actual retry timer duration */
          rd.retry_delay_duration = time_left * 20 + retry_delay * 320;

          /* Inform DS the SCRM retry delay */
          retry_delay_inform(rd);
          MSG_MED("Type %d retry delay=%d ms", rd.retry_type, rd.retry_delay_duration,
            0);
          #else
          bad_type = TRUE;
          #endif // defined(FEATURE_IS2000_SCH) || defined(FEATURE_IS2000_CHS)
          break;
        }

        default:
        {
          bad_type = TRUE;
          break;
        }
      } /* end switch */
    } /* end if ((rd.retry_type == CAI_RETRY_CLR_ALL) || (retry_delay == 0)) */

    if ( bad_type )
    {
      MSG_MED("Invalid retry type %d.", rd.retry_type, 0, 0);
    }
  } /* end if retry_delay_inform */

} /* update_retry_delay */

/*===========================================================================

FUNCTION mcc_send_orig_fail_retry_delay

DESCRIPTION
  This function informs DS the latest retry delay duration for an origination
  by sending comand CM_CALL_FAIL_RETRY_ORDER_F to CM. It uses the current
  system time and given retry delay to compute the actual retry timer
  duration in unit of milliseconds.

  ORIG retry timer duration = (time left before the next 80ms boundary +
  (time unit is 1 min)         least significant 7 bits of retry delay * 60000) ms

  ORIG retry timer duration = (time left before the next 80ms boundary +
  (time unit is 1 sec)         retry delay * 1000) ms

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/

void mcc_send_orig_fail_retry_delay
(
  uint8  retry_type,
  uint8  retry_delay,
  uint16 so_req,
  uint8 call_id
)
{
  qword curr_time; /* Current time, in 20 ms unit */
  uint16 time_left; /* Time left before next 80 ms boundary, in 20 ms unit */
  cm_mc_rpt_type *cmd_ptr; /* Pointer to CM command */

  if ((retry_type != CAI_RETRY_CLR_ALL) && (retry_type != CAI_RETRY_ORIG) /*lint !e641 */
      #ifdef FEATURE_IS2000_REL_D
#error code not present
      #endif /* FEATURE_IS2000_REL_D */
     )
  {
    MSG_MED("Unsupported retry type", 0, 0, 0);
    return;
  }

  if ((cmd_ptr = cm_mc_rpt_get_buf_else_err_fatal()) == NULL)
  {
    MSG_HIGH("No free buffers on cm_mc_rpt_free_q", 0, 0, 0);
  }
  else
  {
    cmd_ptr->retry_order.hdr.cmd = CM_CALL_FAIL_RETRY_ORDER_F;
    cmd_ptr->retry_order.call_id = call_id;
    cmd_ptr->retry_order.retry_info.srv_opt = so_req;
    cmd_ptr->retry_order.retry_info.retry_type =
      (cm_retry_type_e_type) retry_type;
    cmd_ptr->retry_order.retry_info.infinite_delay = FALSE;

    /* Clear retry delay */
    if ((retry_type == CAI_RETRY_CLR_ALL) || (retry_delay == 0))  /*lint !e641 */
    {
      cmd_ptr->retry_order.retry_info.retry_delay_in_ms = 0;
    }
    /* Convert retry delay into retry timer duration in unit of ms */
    else
    {
      /* Get current time in 20 ms units */
      ts_get_20ms_frame_time(curr_time);

      /* Convert current time into 80 ms unit. qw_div() returns the
         remainder of the division, */

      /* -----------------------------------------------------------------
      ** Compute time left in 20 ms unit before the next system time 80 ms
      ** boundary
      ** ----------------------------------------------------------------- */
      time_left =  4 - qw_div(curr_time, curr_time, 4U);

      /* Time unit is 1 min */
      if (retry_delay & 0x80)
      {
        /* Compute actual retry timer duration */
        cmd_ptr->retry_order.retry_info.retry_delay_in_ms = time_left * 20 +
          (retry_delay & 0x7f) * 60000;
      }
      /* Time unit is 1 sec */
      else
      {
        /* Compute actual retry timer duration */
        cmd_ptr->retry_order.retry_info.retry_delay_in_ms = time_left * 20 +
          retry_delay * 1000;
      } /* end if (retry_delay & 0x80) */
    } /* end if ((retry_type == CAI_RETRY_CLR_ALL) || (retry_delay == 0)) */

    /* Send command to CM */
    MSG_MED("Inform DS orig retry delay thr CM", 0, 0, 0);
    mcc_cm_mc_rpt(cmd_ptr);
  }

} /* mcc_send_orig_fail_retry_delay */

/*===========================================================================

FUNCTION MCC_SYS_MEAS_CMD

DESCRIPTION
  The function processes system measurement request from system determination.
  It prepares the system measurement command and sends it to Search. If
  Search rejects the system measurement request, report to system
  determination of all pilots with minimum RXPWR / ECIO and determines
  whether continues on the current system based on the suggested SS-Action.

DEPENDENCIES
  None.

RETURN VALUE
  TRUE if stay on current system. Otherwise, FALSE.


SIDE EFFECTS
  None.
===========================================================================*/

boolean mcc_sys_meas_cmd()
{
  sd_ss_act_s_type ss_act_buf; /* SS-Action buffer */
  sd_sys_s_type    ss_sys;     /* System to be measured */
  #ifdef FEATURE_EXTENDED_PRL
  sd_band_e_type   ss_band;    /* System band */
  #endif
  boolean          stay_on_curr_sys = TRUE;
    /* Flag indicates whether continue on current system */
  int              i;

  /* Retrieve last SS-Action */
  (void) sd_ss_act_get(&ss_act_buf);

  /* Prepare system measurement request */
  mcc_srch_buf.sys_meas.hdr.cmd = SRCH_SYS_MEAS_F;
  mcc_srch_buf.sys_meas.meas_id = ss_act_buf.prm.meas.id;

  if (ss_act_buf.prm.meas.type == SD_SS_MEAS_TYPE_PILOT)
  {
    /* Pilot strength measurement */
    mcc_srch_buf.sys_meas.action = MEAS_ACTION_RXECIO;
  }
  else
  {
    /* RX power measurement */
    mcc_srch_buf.sys_meas.action = MEAS_ACTION_RXONLY;
  }

  /* Fill SRCH buffer with the list of system to be measured */
  mcc_srch_buf.sys_meas.list_cnt = 0;
  #ifdef FEATURE_EXTENDED_PRL  
  for (i=0; sd_ss_meas_list_get(i, &ss_sys.mode, &ss_band, &ss_sys.chan);
  #else
  for (i=0; sd_ss_meas_list_get(i, &ss_sys.mode, &ss_sys.band, &ss_sys.chan);
  #endif
    i++)
  {
    if (i >= MAX_SYS_MEAS_LIST_SIZ)
    {
      MSG_ERROR("System measurement list size exceed limit", 0, 0, 0);
      break;
    } /* end if (i >= MAX_SYS_MEAS_LIST_SIZ) */

    /* -------------------------
    ** Analog system measurement
    ** ------------------------- */
    #ifdef FEATURE_ACP
#error code not present
    #endif /* FEATURE_ACP */
    {
      mcc_srch_buf.sys_meas.meas_list[i].member_type = MEAS_MEMBER_CDMA;
      mcc_srch_buf.sys_meas.meas_list[i].member.cdma_member.band_class =
      #ifdef FEATURE_EXTENDED_PRL  
        ss_band; /*lint !e732 !e641 */
      #else
        ss_sys.band;
      #endif
      mcc_srch_buf.sys_meas.meas_list[i].member.cdma_member.cdma_freq =
        ss_sys.chan;
      mcc_srch_buf.sys_meas.list_cnt++;
    } /* end if (ss_sys.mode == SD_MODE_AMPS) */
  } /* end for */

  /* Fill in necessary fields of command block */
  mcc_srch_buf.hdr.cmd_hdr.done_q_ptr = NULL;
  mcc_srch_buf.hdr.cmd_hdr.task_ptr = &mc_tcb;
  mcc_srch_buf.hdr.cmd_hdr.sigs = MCC_CMD_SIG;

  (void) rex_clr_sigs( &mc_tcb,  MCC_CMD_SIG );

  /* Send Command to Search task */
  srch_cmd( &mcc_srch_buf );

  /* Wait for response. For PLT, this signal is set
     when SRCH task has completed the PLT command */
  (void) mcc_wait( (dword) MCC_CMD_SIG );
  (void) rex_clr_sigs( &mc_tcb,  MCC_CMD_SIG );

  /* -----------------------------------------------------------------------
   * Search task rejected system measurement request.
   * Report all pilots with miniumum RXpPWR or ECIO to system determination.
   * Determine whether to stay on current system based on the returned
   * SS-Action.
   * ----------------------------------------------------------------------- */
  if (mcc_srch_buf.hdr.status != SRCH_DONE_S)
  {
    MSG_MED("SRCH rejected sys meas req", 0, 0, 0);
    for (i=0; i < mcc_srch_buf.sys_meas.list_cnt; i++)
    {
      /* -------------------------
      ** Analog system measurement
      ** ------------------------- */
      if (mcc_srch_buf.sys_meas.meas_list[i].member_type == MEAS_MEMBER_AMPS)
      {
        if (!sd_ss_meas_list_rprt(i, (int2) NO_SYS_MEAS_RESULTS))
        {
          ERR("Unxpected AMPS sys meas rprt from SRCH", 0, 0, 0);
        }
      }
      /* -----------------------
      ** CDMA system measurement
      ** ----------------------- */
      else
      {
        if (mcc_srch_buf.sys_meas.action == MEAS_ACTION_RXECIO)
        {
          /* RX pwr measurement */
          if (!sd_ss_meas_list_rprt(i, (int2) NO_SYS_MEAS_RESULTS))
          {
            ERR("Unxpected CDMA sys meas rpt from SRCH", 0, 0, 0);
          }
        }
        else
        {
          /* Pilot strength measurement */
          if (!sd_ss_meas_list_rprt(i, (int2) NO_SYS_MEAS_RESULTS))
          {
            ERR("Unxpected CDMA sys meas rpt from SRCH", 0, 0, 0);
          }
        } /* end if (mcc_srch_buf.sys_meas.action == MEAS_ACTION_RXCECIO */

      } /* end if (mcc_srch_buf.sys_meas.meas_list[i].member_type ==
           MEAS_MEMBER_AMPS)*/

    } /* end for */

    /* Decide whether to stay on current system */
    if (!mcc_continue_on_curr_sys(MEAS_BACK_ALLOWED,
        sd_ss_ind_misc_meas_rprt(mcc_srch_buf.sys_meas.meas_id, TRUE, NULL)))
    {
      stay_on_curr_sys = FALSE;
    }
  }

  return (stay_on_curr_sys);

} /* mcc_sys_meas_cmd */

/*===========================================================================

FUNCTION MCC_SYS_MEAS_RPT

DESCRIPTION
  The function reports the system measurements based on search report to
  system determination.

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.
===========================================================================*/

void mcc_sys_meas_rpt
(
  mccsrch_rpt_type *rpt_ptr /* Pointer to SRCH report */
)
{
  int i;

  for (i=0; i < rpt_ptr->sys_meas.list_cnt; i++)
  {
    /* -------------------------
    ** Analog system measurement
    ** ------------------------- */
    if (rpt_ptr->sys_meas.meas_list[i].member_type == MEAS_MEMBER_AMPS)
    {
      MSG_MED("AMPS rx=%d",
        rpt_ptr->sys_meas.meas_list[i].member.amps_results.rx_pwr, 0, 0);
      if (!sd_ss_meas_list_rprt(i,
        (int2) rpt_ptr->sys_meas.meas_list[i].member.amps_results.rx_pwr))
      {
        ERR("Unxpected AMPS sys meas rprt from SRCH", 0, 0, 0);
      }
    }
    /* -----------------------
    ** CDMA system measurement
    ** ----------------------- */
    else
    {
      MSG_MED("CDMA rx=%d, ecio=%d",
        rpt_ptr->sys_meas.meas_list[i].member.cdma_results.rx_pwr,
        rpt_ptr->sys_meas.meas_list[i].member.cdma_results.ecio, 0);

      if (rpt_ptr->sys_meas.action == MEAS_ACTION_RXECIO)
      {
        /* RX pwr measurement */
        if (!sd_ss_meas_list_rprt(i,
          (int2) rpt_ptr->sys_meas.meas_list[i].member.cdma_results.ecio))
        {
          ERR("Unxpected CDMA sys meas rpt from SRCH", 0, 0, 0);
        }
      }
      else
      {
        /* Pilot strength measurement */
        if (!sd_ss_meas_list_rprt(i,
          (int2) rpt_ptr->sys_meas.meas_list[i].member.cdma_results.rx_pwr))
        {
          ERR("Unxpected CDMA sys meas rpt from SRCH", 0, 0, 0);
        }
      } /* end if (rpt_ptr->sys_meas.action == MEAS_ACTION_RXCECIO */

    } /* end if (rpt_ptr->sys_meas.meas_list[i].member_type == MEAS_MEMBER_AMPS)*/

  } /* end for */

} /* mcc_sys_meas_rpt */

/*===========================================================================

FUNCTION MCC_CONTINUE_ON_CURR_SYS

DESCRIPTION
  Based on passed in SS-Action, this function checks whether the MS can
  continue on its current system.

DEPENDENCIES
  None.

RETURN VALUE
  TRUE if SS-Action allows MS to continue on the current system.
  FALSE if SS-Action specifies change of mode or request dedicated
    system measurement.

SIDE EFFECTS
  None.
===========================================================================*/

boolean mcc_continue_on_curr_sys
(
  mcc_sys_meas_mode_type meas_mode, /* Type of measurment mode allowed */
  sd_ss_act_e_type       ss_action  /* SS-Action                       */
)
{
  boolean stay_on_curr_sys;
    /* True - continue on current system. False - Exit to MC */

  stay_on_curr_sys = TRUE;
  switch (ss_action)
  {
    /* Continue on the current system */
    case SD_SS_ACT_CONTINUE:
      break;

    /* Background system measurement */
    case SD_SS_ACT_MEAS_BACK:
      /* Background system measurement is supported in current state */
      if (meas_mode == MEAS_BACK_ALLOWED)
      {
        /* ------------------------------------------------------------
        ** Submit system measurement request to Search and decide
        ** whether to stay on current system based on returned feedback
        ** ------------------------------------------------------------ */
        stay_on_curr_sys = mcc_sys_meas_cmd();
      }
      else
      {
        MSG_MED("System measurement request ignored", 0, 0, 0);
      }
      break;

    /* Exit to MC */
    default:
      MSG_MED("Suggested SS-Action %d", ss_action, 0, 0);
      stay_on_curr_sys = FALSE;
      break;

  } /* end switch (ss_action) */

  return stay_on_curr_sys;

} /* mcc_continue_on_curr_sys */

/*===========================================================================

FUNCTION MCC_INFORM_SD_CHAN_ASSIGN

DESCRIPTION
  This function informs system determination of the paging / traffic channel
  assignement.

DEPENDENCIES
  None.

RETURN VALUE
  TRUE if the paging / traffic channel assignment is accepted by system
    determination.
  FALSE if the paging / traffic channel assignment is rejected by system
    determination.

SIDE EFFECTS
  None.
===========================================================================*/

boolean mcc_inform_sd_chan_assign
(
  mccdma_chnasn_type *chan_assign_msg_ptr,
    /* Message addressed to this mobile which specifies a Paging / Traffic
       Channel assignment */
  mcc_sys_meas_mode_type sys_meas_mode
    /* Mode of system measurement allowed */
)
{
  boolean chan_assign_accepted = FALSE;
   /* Indicates whether paging / traffic channel assignment is accepted */

  switch (chan_assign_msg_ptr->assign_mode)
  {
    case CAI_EXT_PAGING_CHAN_ASSIGN:
    case CAI_PAGING_CHAN_ASSIGN:
      /* Both Frequency and band class are not included in the message */
      if (!chan_assign_msg_ptr->mode.am1.freq_incl)
      {
        if (mcc_continue_on_curr_sys(sys_meas_mode,
          sd_ss_ind_cdma_opr_cam_cdma(SD_SS_HO_CDMA_CAM_TO_CDMA_PAG,
          (sd_band_e_type) cdma.band_class, cdma.cdmach, NULL)))
        {
          chan_assign_accepted = TRUE;
        }
      }
      /* Frequency and / or band class are included in the message */
      else
      {
        if ((chan_assign_msg_ptr->assign_mode == CAI_EXT_PAGING_CHAN_ASSIGN)
            || (chan_assign_msg_ptr->msg_type == CAI_EXT_CHN_ASN_MSG)
           )
        {
          if (mcc_continue_on_curr_sys(sys_meas_mode,
            sd_ss_ind_cdma_opr_cam_cdma(SD_SS_HO_CDMA_CAM_TO_CDMA_PAG,
            (sd_band_e_type) chan_assign_msg_ptr->mode.am1.band_class,
            chan_assign_msg_ptr->mode.am1.cdma_freq, NULL)))
          {
            chan_assign_accepted = TRUE;
          }
        }
        else if (mcc_continue_on_curr_sys(sys_meas_mode,
          sd_ss_ind_cdma_opr_cam_cdma(SD_SS_HO_CDMA_CAM_TO_CDMA_PAG,
          (sd_band_e_type) cdma.band_class, chan_assign_msg_ptr->mode.am1.cdma_freq, NULL)))
        {
          chan_assign_accepted = TRUE;
        }
      }
      break;

    case CAI_EXT_TRAF_CHAN_ASSIGN:
    case CAI_TRAFFIC_CHAN_ASSIGN:
      /* Both Frequency and band class are not included in the message */
      if (!chan_assign_msg_ptr->mode.am0.freq_incl)
      {
        if (mcc_continue_on_curr_sys(sys_meas_mode,
          sd_ss_ind_cdma_opr_cam_cdma(SD_SS_HO_CDMA_CAM_TO_CDMA_TRF,
          (sd_band_e_type) cdma.band_class, cdma.cdmach, NULL)))
        {
          chan_assign_accepted = TRUE;
        }
      }
      /* Frequency and / or band class are included in the message */
      else
      {
        if ((chan_assign_msg_ptr->assign_mode == CAI_EXT_TRAF_CHAN_ASSIGN)
            || (chan_assign_msg_ptr->msg_type == CAI_EXT_CHN_ASN_MSG)
            #ifdef FEATURE_MEID_SUPPORT
            || (chan_assign_msg_ptr->msg_type == CAI_MEID_EXT_CHN_ASN_MSG)
            #endif /* FEATURE_MEID_SUPPORT */
           )
        {
          if (mcc_continue_on_curr_sys(sys_meas_mode,
            sd_ss_ind_cdma_opr_cam_cdma(SD_SS_HO_CDMA_CAM_TO_CDMA_TRF,
            (sd_band_e_type) chan_assign_msg_ptr->mode.am0.band_class,
            chan_assign_msg_ptr->mode.am0.cdma_freq, NULL)))
          {
            chan_assign_accepted = TRUE;
          }
        }
        else if (mcc_continue_on_curr_sys(sys_meas_mode,
          sd_ss_ind_cdma_opr_cam_cdma(SD_SS_HO_CDMA_CAM_TO_CDMA_TRF,
          (sd_band_e_type) cdma.band_class, chan_assign_msg_ptr->mode.am0.cdma_freq, NULL)))
        {
          chan_assign_accepted = TRUE;
        }
      }
      break;

    case CAI_ACQ_ANALOG_SYSTEM:
      if (sd_ss_ind_cdma_opr_cam_amps(SD_SS_HO_CDMA_CAM_TO_AMPS_SYS,
        (sd_cell_sys_e_type) chan_assign_msg_ptr->mode.am2.analog_sys, 0, SD_WILDCARD_SID, NULL)
        == SD_SS_ACT_ACQ_AMPS)
      {
        chan_assign_accepted = TRUE;
      }
      break;

    case CAI_ANALOG_VCHAN_ASSIGN:
      /* function arg-2 not used inside the function */ 
      if (sd_ss_ind_cdma_opr_cam_amps(SD_SS_HO_CDMA_CAM_TO_AMPS_VOICE, (sd_cell_sys_e_type) 0, 
        chan_assign_msg_ptr->mode.am3.analog_chan,
        chan_assign_msg_ptr->mode.am3.sid, NULL)  == SD_SS_ACT_CONTINUE)
      {
        chan_assign_accepted = TRUE;
      }
      break;

    default:
      break;

  } /* end switch (chan_assign_msg_ptr->assign_mode) */

  return chan_assign_accepted;
 
} /* mcc_inform_sd_chan_assign */

/*===========================================================================

FUNCTION MCC_INFORM_SD_REDIRECTION

DESCRIPTION
  This function informs system determination of the redirection.

DEPENDENCIES
  None.

RETURN VALUE
  TRUE if the redirection is accepted by system determination.
  FALSE if the redirection is rejected by system determination.

SIDE EFFECTS
  None.
===========================================================================*/

boolean mcc_inform_sd_redirection
(
  sd_ss_redir_e_type  redir_type, /* Type of redirection */
  mcc_redir_info_type *redir_info /* Redirection information */
)
{
  boolean redir_accepted = FALSE;
   /* Indicates whether redirection is accepted */

  if (sd_ss_ind_cdma_opr_redir(redir_info->return_if_fail,
      redir_type, redir_info->redir_rec_list,
      redir_info->num_recs, NULL)
      == SD_SS_ACT_ACCEPT)
  {
    redir_accepted = TRUE;
  }

  return redir_accepted;

} /* mcc_inform_sd_redirection2 */

/*===========================================================================

FUNCTION MCC_SET_CURR_NAM

DESCRIPTION
  This function saves the new NAM when the NAM is changed.

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.
===========================================================================*/

void mcc_set_curr_nam
(
  mc_nam_changed_type nam_changed /* Pointer to new NAM info */
)
{
  mcc_exit.param.nam_change.nam = nam_changed.nam;
  mcc_exit.param.nam_change.orig_mode = nam_changed.orig_mode;
  mcc_exit.param.nam_change.mode_pref = nam_changed.mode_pref;
  mcc_exit.param.nam_change.band_pref = nam_changed.band_pref;
  mcc_exit.param.nam_change.roam_pref = nam_changed.roam_pref;
  mcc_exit.param.nam_change.hybr_pref = nam_changed.hybr_pref;

} /* mcc_set_curr_nam */

/*===========================================================================

FUNCTION  mcc_expired_timer_callback_func

DESCRIPTION
  This is the function that will be executed (in MC context) upon
  the expiration of a MC timer that uses the Asynchronous Procedure
  Call (APC) and "Extended Timers" features of REX+. The function queues the timer ID (argument)
  of the expired timer.

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/

void mcc_expired_timer_callback_func
(
  unsigned long expired_timer_id
)
{
  mc_expired_timer_type *item_ptr;

  if (!mcc_timer_on_callback_q(expired_timer_id))
  {
    // Add to Callback Queue

    if ( (item_ptr = (mc_expired_timer_type *) q_get(&mc_expired_timers_free_q)) == NULL)
    {
      // we are out of buffers to queue the expired timer, if the compile-time
      // limit is set according to not less than the # of timers we use, this shouldn't happen.
      MSG_ERROR("Ran out of buffers to indicate timer expiration", 0,0,0);
      return;
    }

    // queue the expired timer.
    item_ptr->expired_timer_id = expired_timer_id;
    q_put(&mc_expired_timers_q, &item_ptr->link);

  }

  // set the "universal" signal for callback timers.
  (void) rex_set_sigs( &mc_tcb, MCC_CALLBACK_TIMER_SIG );

} /* mcc_expired_timer_callback_func */

/*===========================================================================

FUNCTION  mcc_clr_timer_on_callback_q

DESCRIPTION
  This function is used to clear the entry of a timer (that
  uses the REX+ callback mechanism) in the queue that MC maintains.

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  The queue now does not contain an entry corresponding to the argument.

===========================================================================*/

void mcc_clr_timer_on_callback_q
(
  unsigned long timer_id
)
{
  mc_expired_timer_type *item_ptr;

  for ( item_ptr = (mc_expired_timer_type *)q_check(&mc_expired_timers_q);
        item_ptr != NULL;
        item_ptr = (mc_expired_timer_type *)q_next(&mc_expired_timers_q, &item_ptr->link)
      )
  {
    if (item_ptr->expired_timer_id == timer_id)
    {
      // timer ID found, delete
      q_delete(&mc_expired_timers_q, &item_ptr->link);

      // add to free queue
      q_put(&mc_expired_timers_free_q, &item_ptr->link);

      break;
    }
  } // for

  INTLOCK();

  if (q_check(&mc_expired_timers_q) == NULL)
  {
    // Queue is empty, can clear the callback timer signal.
    (void) rex_clr_sigs( &mc_tcb, MCC_CALLBACK_TIMER_SIG );
  }

  INTFREE();

} /* mcc_clr_timer_on_callback_q */

/*===========================================================================

FUNCTION  mcc_timer_on_callback_q

DESCRIPTION
  This function walks through the queue of expired timers to see if the
  argument (timer ID) is present in it.

DEPENDENCIES
  None.

RETURN VALUE
  TRUE, if timer ID on queue.
  FALSE, otherwise.

SIDE EFFECTS
  None.

===========================================================================*/

boolean mcc_timer_on_callback_q
(
  unsigned long timer_id
)
{
  mc_expired_timer_type *item_ptr;

  for ( item_ptr = (mc_expired_timer_type *)q_check(&mc_expired_timers_q);
        item_ptr != NULL;
        item_ptr = (mc_expired_timer_type *)q_next(&mc_expired_timers_q, &item_ptr->link)
      )
  {
    if (item_ptr->expired_timer_id == timer_id)
    {
      // Found timer_id on queue
      return TRUE;
    }
  } // for

  return FALSE;

} /* mcc_timer_on_callback_q */

/*===========================================================================

FUNCTION  mcc_get_type_specific_timer_on_callback_q

DESCRIPTION
  This function walks through the queue of expired timers to see if any timer matching the
  argument (query_timer_type) is present in it. If present, it retrieves the timer ID.

DEPENDENCIES
  None.

RETURN VALUE
  TRUE, if timer ID is being returned in argument.
  FALSE, otherwise.

SIDE EFFECTS
  None.

===========================================================================*/

boolean mcc_get_type_specific_timer_on_callback_q
(
  unsigned long   query_timer_type,
  unsigned long * ret_timer_id
)
{
  mc_expired_timer_type *item_ptr;

  for ( item_ptr = (mc_expired_timer_type *)q_check(&mc_expired_timers_q);
        item_ptr != NULL;
        item_ptr = (mc_expired_timer_type *)q_next(&mc_expired_timers_q, &item_ptr->link)
      )
  {
    if ( (item_ptr->expired_timer_id & MCC_TIMER_TYPE_MASK) == query_timer_type) /*lint !e737*/
    {
      // Found timer_id with matching timer_type on queue
      *ret_timer_id = item_ptr->expired_timer_id;
      return TRUE;
    }
  } // for

  // No timer on queue that matches the timer type specified - query_timer_type.
  return FALSE;
 
} /* mcc_get_type_specific_timer_on_callback_q */

/*===========================================================================

FUNCTION  mcc_purge_timer_callback_q

DESCRIPTION
  This function purges the queue of expired timers and clears the
  MCC_CALLBACK_TIMER_SIG

DEPENDENCIES
  None.

RETURN VALUE
  none.

SIDE EFFECTS
  None.

===========================================================================*/

void mcc_purge_timer_callback_q
(
  void
)
{
  mc_expired_timer_type *item_ptr;

  while ( (item_ptr = (mc_expired_timer_type *)q_get(&mc_expired_timers_q)) != NULL )
  {
    // add to free queue
    q_put(&mc_expired_timers_free_q, &item_ptr->link);
  }

  // Queue is empty, can clear the callback timer signal.
  (void) rex_clr_sigs( &mc_tcb, MCC_CALLBACK_TIMER_SIG );

} /* mcc_purge_timer_callback_q */

/*===========================================================================

FUNCTION  mcc_set_callback_timer

DESCRIPTION
  This function should be used to set timers that utilise the REX
  callback mechanism within MCC. It clears the timer, its entry in
  the callback queue, and sets the timer.

DEPENDENCIES
  None.

RETURN VALUE
  last countdown of timer

SIDE EFFECTS
  remove the entry, (if any), corresponding to callback_timer_id from
  callback queue.

===========================================================================*/

rex_timer_cnt_type mcc_set_callback_timer
(
    rex_timer_type       *p_timer,   /* pointer to timer to set */
    rex_timer_cnt_type    msecs,      /* value in milliseconds   */
    unsigned long         callback_timer_id
)
{
  /* For the new callback timer mechanism, it is necessary to clear timer
     and the entry in queue prior to setting the timer */
  rex_timer_cnt_type timer_count =
       mcc_clr_callback_timer( p_timer, callback_timer_id );

  (void) rex_set_timer( p_timer, msecs );

  return timer_count;

} /* mcc_set_callback_timer */

/*===========================================================================

FUNCTION  mcc_clr_callback_timer

DESCRIPTION
  This function should be used to clr timers that utilise the REX
  callback mechanism within MCC. It clears the timer and its entry in
  the callback queue.

DEPENDENCIES
  None.

RETURN VALUE
  countdown of timer

SIDE EFFECTS
  remove the entry, (if any), corresponding to callback_timer_id from
  callback queue.

===========================================================================*/

rex_timer_cnt_type mcc_clr_callback_timer
(
  rex_timer_type *p_timer,   /* pointer to timer to set */
  unsigned long callback_timer_id
)
{
  // Have REX+ clear the timer
  rex_timer_cnt_type timer_count = rex_clr_timer(p_timer);

  // remove item from queue, if it exists.
  mcc_clr_timer_on_callback_q(callback_timer_id);

  return timer_count;
 
} /* mcc_clr_callback_timer */

/*===========================================================================

FUNCTION MCC_INCREMENT_PAG_3_OR_FCCCH_3

DESCRIPTION
  This function increments count PAG_3 or FCCCH_3.

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.
===========================================================================*/

void mcc_increment_pag_3_or_fccch_3
(
  void
)
{
  word parm_stat;
    /* Status returned by parm_inc procedure */

  if (OVHD_CHAN_IS_PCH)
  {
    if ((parm_stat = parm_inc( CAI_PAG_3_ID, 1 )) != PARM_DONE_S)
    {
      MSG_HIGH( "Invalid parameter increment: status %d",
                parm_stat, 0, 0 );
    }
  }

  #ifdef FEATURE_IS2000_REL_A_CC
  else if (OVHD_CHAN_IS_BCCH)
  {
    if ((parm_stat = parm_inc( CAI_FCCCH_3_ID, 1 )) != PARM_DONE_S)
    {
      MSG_HIGH( "Invalid parameter increment: status %d",
                 parm_stat, 0, 0 );
    }
  }
  #endif /* FEATURE_IS2000_REL_A_CC */

  else
  {
    /* Should never be here */
    ERR("Invalid overhead channel %d", cur_bs_ptr->ovhd_chan_info.ovhd_chan, 0, 0);
  }

} /* mcc_increment_pag_3_or_fccch_3 */

#ifdef FEATURE_IS2000_REL_A_CC
/*===========================================================================

FUNCTION MCC_EMPTY_SEG_UPM_BUFFER

DESCRIPTION
  This function marks the status of Universal Page message reassembly buffer
  as invalid.


DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/

void mcc_empty_seg_upm_buffer(void)
{
  if (mccdma_seg_univ_page.valid)
  {
    mccdma_seg_univ_page.valid = FALSE;
  }

} /* mcc_empty_seg_upm_buffer */

/*===========================================================================

FUNCTION MCC_MATCH_UPM_IMSI_PARTIAL_ADDR

DESCRIPTION
  This function performs the Mobile Station IMSI partial address match
  operation for the Universal Page Message. The result of match is directly
  saved in the input partial address structure.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/

void mcc_match_upm_imsi_partial_addr
(
  caii_univ_page_interleaved_addr_type *addr_ptr
   /* Pointer to partial address structure */
)
{
  dword imsi_s_mask;

  if (!addr_ptr->imsi_included)
  {
    qw_set(addr_ptr->imsi_match, 0L, 0L);
  }
  else if (addr_ptr->imsi_bit_matched < addr_ptr->iaddr_portion_rxed)
  {
    /* Not yet done with IMSI partial address matching */
    imsi_s_mask = (1 << addr_ptr->imsi_bit_matched);
    if (!(qw_lo(cdma.imsi_s) & imsi_s_mask))
    {
      qw_set(addr_ptr->imsi_addr, ~qw_hi(addr_ptr->imsi_addr),
        ~qw_lo(addr_ptr->imsi_addr));
    }
    qw_set(addr_ptr->imsi_match, qw_hi(addr_ptr->imsi_match) &
      qw_hi(addr_ptr->imsi_addr), qw_lo(addr_ptr->imsi_match) &
      qw_lo(addr_ptr->imsi_addr));

    /* Record the number of IMSI partial address bits matched */
    addr_ptr->imsi_bit_matched++;
  }
 
} /* mcc_match_upm_imsi_partial_addr */

/*===========================================================================

FUNCTION MCC_MATCH_UPM_PARTIAL_ADDR

DESCRIPTION
  This function performs the Mobile Station partial address match operation
  for the Universal Page Message.

DEPENDENCIES
  None

RETURN VALUE
  TRUE if partial address match.
  FALSE partial address mismatch.

SIDE EFFECTS
  None

===========================================================================*/

boolean mcc_match_upm_partial_addr
(
  word len,
    /* Length of page block */
  byte *buf_ptr,
    /* Pointer to page block */
  word *offset_ptr,
    /* Pointer to offset from the page block to start next translation */
  caii_univ_page_interleaved_addr_type *addr_ptr
    /* Pointer to interleaved address structure */
)
{
  int i;

  if (addr_ptr->iaddr_portion_matched < CAI_UNIV_PAGE_INTERLEAVED_ADDR_PORTION)
  {
    /* Not yet done with partial address matching */

    if (!addr_ptr->initialized)
    {
      /* Mark partial address as initialized */
      addr_ptr->initialized = TRUE;

      /* Initialize address match mask and counters. */
      if (addr_ptr->bcast_included)
      {
        #ifdef FEATURE_IS2000_REL_A_CC_BCSMS
        if (mccbcsms_is_enabled())
        {
          addr_ptr->bcast_match = TRUE;
        }
        else
        #endif /* FEATURE_IS2000_REL_A_CC_BCSMS */
        {
          addr_ptr->bcast_match = FALSE;
        }
        addr_ptr->iaddr_portion_len = addr_ptr->num_bcast + 1;
      }
      else
      {
        addr_ptr->bcast_match = FALSE;
        addr_ptr->iaddr_portion_len = 0;
      }
      addr_ptr->bcast_bit_matched = 0;

      if (addr_ptr->imsi_included)
      {
        /*lint -e570*/
        qw_set(addr_ptr->imsi_match, CAII_UNIV_PAGE_PARTIAL_ADDR_MATCH,
          CAII_UNIV_PAGE_PARTIAL_ADDR_MATCH);
        /*lint +e570*/
        /* For unused address bits, set it to 0 */
        i = CAI_UNIV_PAGE_NUM_IMSI_REC_MAX - addr_ptr->num_imsi - 1;  /*lint !e834 */
        if (i > 0)
        {
          qw_shift(addr_ptr->imsi_match, -i);
          qw_shift(addr_ptr->imsi_match, +i);
        }
        addr_ptr->iaddr_portion_len += addr_ptr->num_imsi +1;
      }
      else
      {
        qw_set(addr_ptr->imsi_match, 0L, 0L);
      }
      addr_ptr->imsi_bit_matched = 0;

      if (addr_ptr->tmsi_included)
      {
        /*lint -e570*/
        qw_set(addr_ptr->tmsi_match, CAII_UNIV_PAGE_PARTIAL_ADDR_MATCH,
          CAII_UNIV_PAGE_PARTIAL_ADDR_MATCH);
        /*lint +e570*/
        /* For unused address bits, set it to 0 */
        i = CAI_UNIV_PAGE_NUM_TMSI_REC_MAX - addr_ptr->num_tmsi - 1; /*lint !e834 */
        if (i > 0)
        {
          qw_shift(addr_ptr->tmsi_match, -i);
          qw_shift(addr_ptr->tmsi_match, +i);
        }
        addr_ptr->iaddr_portion_len += addr_ptr->num_tmsi + 1;
      }
      else
      {
        qw_set(addr_ptr->tmsi_match, 0L, 0L);
      }
      addr_ptr->tmsi_bit_matched = 0;

      if (addr_ptr->res_type_included)
      {
        addr_ptr->iaddr_portion_len += addr_ptr->num_res_type + 1;
      }
    }

    /* Perform IMSI and TMSI partial address match */
    while (((qw_hi(addr_ptr->imsi_match) != 0) ||
            (qw_lo(addr_ptr->imsi_match) != 0) ||
            (qw_hi(addr_ptr->tmsi_match) != 0) ||
            (qw_lo(addr_ptr->tmsi_match) != 0)) &&
            (len >= *offset_ptr + addr_ptr->iaddr_portion_len) &&
            (addr_ptr->iaddr_portion_rxed <
             CAI_UNIV_PAGE_INTERLEAVED_ADDR_PORTION))
    {
      /* Translate interleaved address portion */
      if (xlate_ext_univ_page_interleaved_addr_field(CAII_UPM_ADDR_BLOCK,
          buf_ptr, offset_ptr, addr_ptr) == CAIX_DONE_S)
      {
        /* Perform IMSI partial address match */
        if (addr_ptr->imsi_bit_matched < addr_ptr->iaddr_portion_rxed)
        {
          if ((qw_hi(addr_ptr->imsi_match) != 0) ||
              (qw_lo(addr_ptr->imsi_match) != 0))
          {
            mcc_match_upm_imsi_partial_addr(addr_ptr);
          }
        }

        /* Perform TMSI partial address match */
        if (addr_ptr->tmsi_bit_matched < addr_ptr->iaddr_portion_rxed)
        {
          if ((qw_hi(addr_ptr->tmsi_match) != 0) ||
              (qw_lo(addr_ptr->tmsi_match) != 0))
          {
            tmsi_match_upm_partial_addr(addr_ptr);
          }
        }
      }
      else
      {
        /* Shall never be here, since translation is always good */
        MSG_MED("UPM xlate interleaved addr error", 0, 0, 0);
      }
    }

    #ifdef FEATURE_IS2000_REL_A_CC_BCSMS
    if (addr_ptr->bcast_match)
    {
      /* Translate all available interleaved address portion */
      while ((len >= *offset_ptr + addr_ptr->iaddr_portion_len) &&
             (addr_ptr->iaddr_portion_rxed <
              CAI_UNIV_PAGE_INTERLEAVED_ADDR_PORTION))
      {
        if (xlate_ext_univ_page_interleaved_addr_field(CAII_UPM_ADDR_BLOCK,
            buf_ptr, offset_ptr, addr_ptr) != CAIX_DONE_S)
        {
          /* Shall never be here, since translation is always good */
          MSG_MED("UPM xlate interleaved addr error", 0, 0, 0);
          break;
        }
      }

      /* Perform broadcast partial address match */
      if (addr_ptr->bcast_bit_matched < addr_ptr->iaddr_portion_rxed)
      {
        mccbcsms_match_upm_partial_addr(addr_ptr);
      }
    }
    #endif /* FEATURE_IS2000_REL_A_CC_BCSMS */

    addr_ptr->iaddr_portion_matched = addr_ptr->iaddr_portion_rxed;
  }

  return ((qw_hi(addr_ptr->imsi_match) != 0) ||
          (qw_lo(addr_ptr->imsi_match) != 0) ||
          (qw_hi(addr_ptr->tmsi_match) != 0) ||
          (qw_lo(addr_ptr->tmsi_match) != 0) || addr_ptr->bcast_match);
 
} /* mcc_match_upm_partial_addr */

/*===========================================================================

FUNCTION MCC_REASSEMBLE_SEG_UPM

DESCRIPTION
  This function reassembles Universal Page message from Universal Page
  segmenti messages.

DEPENDENCIES
  None

RETURN VALUE
  TRUE if reassembly okay. Otherwise, FALSE.

SIDE EFFECTS
  None

===========================================================================*/

boolean mcc_reassemble_seg_upm
(
  mccrxtx_msg_type *msg_ptr,
    /* Pointer to received message */
  caii_seg_univ_page_type *buf_ptr
    /* Pointer to buffer for storing reassembled segmented Universal Page
       message */
)
{
  word len;
  word slot, ref_slot, sci_mask;

  switch (msg_ptr->msg.gen.msg_type)
  {
    case CAI_UPM_FIRST_SEG_MSG:
      /* Mark the data in segemented UPM reassembly buffer is valid */
      buf_ptr->valid = TRUE;
      /* Mark the start of partial address matching */
      buf_ptr->interleaved_addr.initialized = FALSE;
      buf_ptr->interleaved_addr.iaddr_portion_matched =
      buf_ptr->interleaved_addr.iaddr_portion_rxed    =
      buf_ptr->page_block.offset                      =
      buf_ptr->page_block.length                      = 0;
      buf_ptr->segment_seq = CAII_UNIV_PAGE_FIRST_SEG;
      /* Mark message translation starts from the first field */
      buf_ptr->msg_field = CAII_UPM_CONFIG_MSG_SEQ;
      /* Record frame that first UPM segment received */
      qw_equ(buf_ptr->frame, msg_ptr->frm.frame);
      break;

    case CAI_UPM_MIDDLE_SEG_MSG:
    case CAI_UPM_FINAL_SEG_MSG:
      if (!buf_ptr->valid ||
          ((msg_ptr->msg.upsm.segment_seq == CAII_UNIV_PAGE_SECOND_SEG) &&
           ((msg_ptr->msg.gen.msg_type == CAI_UPM_FINAL_SEG_MSG) ||
            (buf_ptr->segment_seq != CAII_UNIV_PAGE_FIRST_SEG))) ||
          ((msg_ptr->msg.upsm.segment_seq > CAII_UNIV_PAGE_SECOND_SEG) &&
           ((buf_ptr->segment_seq == CAII_UNIV_PAGE_FIRST_SEG) ||
            (msg_ptr->msg.upsm.segment_seq != buf_ptr->segment_seq + 1))))
      {
        /* Segment received not in sequence, empty reassembly buffer */
        mcc_empty_seg_upm_buffer();
      }
      else
      {
        /* ---------------------------------------------------------------
        ** According  to IS2000 Release A LAC standard, Base station shall
        ** not transmit PDUs derived from the same Universal Page Block in
        ** different FCCCH slots.
        ** --------------------------------------------------------------- */
        /* Get position-in-slot */
        sci_mask = mccidl_get_sci_mask();
        /* Calculate the FCCCH slot that first UPM segment is received */
        ref_slot = ((word)(qw_lo(buf_ptr->frame)) >> 2) & sci_mask;
        /* Calculate the FCCCH slot that this UPM segment is received */
        slot = ((word)(qw_lo(msg_ptr->frm.frame)) >> 2) & sci_mask;
        if (slot != ref_slot)
        {
          /* Segment received in different FCCCH slots, empty reassembly
             buffer */
          mcc_empty_seg_upm_buffer();
        }
        else
        {
          buf_ptr->segment_seq = msg_ptr->msg.upsm.segment_seq;
        }
      }
      break;

    default:
      break;
  }

  if (buf_ptr->valid)
  {
    /* Figure out the number of bytes needed to store reassembled message */
    len = (msg_ptr->msg.upsm.page_seg_block_len +
           buf_ptr->page_block.length) / 8 ;
    if (8*len < (msg_ptr->msg.upsm.page_seg_block_len +
                 buf_ptr->page_block.length))
    {
      len++;
    }

    /* Check to see if enough buffer space to store reassembled message */
    if (len > CAII_SEG_UPM_MAX_MSG_BYTES)
    {
      /* Message too large, out of buffer space, empty reassembly buffer */
      MSG_MED("UPM too large", 0, 0, 0);
      mcc_empty_seg_upm_buffer();
    }
    else
    {
      if (msg_ptr->msg.upsm.page_seg_block_len > 0)
      {
        /* Append segment to the page block */
        b_copy(msg_ptr->msg.upsm.page_seg_block, 0, buf_ptr->page_block.data,
          buf_ptr->page_block.length, msg_ptr->msg.upsm.page_seg_block_len);
        buf_ptr->page_block.length += msg_ptr->msg.upsm.page_seg_block_len;

        /* Translate header fields of reassembled segmented Universal Page
           Message */
        if ((buf_ptr->msg_field != CAII_UPM_DONE) &&
            (buf_ptr->msg_field != CAII_UPM_ADDR_BLOCK) &&
        (xlate_ext_segmented_univ_page(buf_ptr) != CAIX_DONE_S))
        {
          /* Translation error, empty reassembly buffer */
          MSG_MED("UPM xlate error", 0, 0, 0);
          mcc_empty_seg_upm_buffer();
        }
      }
    }
  }

  return (buf_ptr->valid);
 
} /* mcc_reassemble_seg_upm */
#endif /* FEATURE_IS2000_REL_A_CC */

/*===========================================================================
FUNCTION mc_map_deact_to_mcc_exit_reason

DESCRIPTION
  This function maps the deact_reason received in MMoC's deactivate request
  to mcc_exit_name_type.

DEPENDENCIES
  None.

RETURN VALUE
  mcc_exit_reason

SIDE EFFECTS
  None.
===========================================================================*/

mcc_exit_name_type mc_map_deact_to_mcc_exit_reason
(
  prot_deact_e_type deact_reason
    /* MMoC's protocol deactivate reason.  */
)
{
  mcc_exit_name_type exit_reason = MCC_POWERDOWN;

  switch ( deact_reason )
  {
    case PROT_DEACT_PWR_DOWN:
      exit_reason = MCC_POWERDOWN;
      break;

    case PROT_DEACT_LPM:
      exit_reason = MCC_LPM;
      break;
    
    case PROT_DEACT_RESET:
      exit_reason = MCC_RESET;
      break;

    case PROT_DEACT_OFFLINE:
      exit_reason = MCC_OFFLINE_DIGITAL;
      break;

    case PROT_DEACT_SUBSC_CHGD:
    case PROT_DEACT_FTM:
      exit_reason = MCC_NAM_CHANGED;
      break;

    default:
      exit_reason = MCC_NAM_CHANGED;
      MSG_ERROR("Unknown deact_reason", 0, 0, 0 );
      break;

  } /* switch ( deact_reason ) */

  return exit_reason;

} /* mc_map_deact_to_mcc_exit_reason() */

/*===========================================================================
FUNCTION mc_map_gen_cmd_to_mcc_exit_reason

DESCRIPTION
  This function maps the MMOC's generic command received to
  mcc_exit_name_type.

DEPENDENCIES
  None.

RETURN VALUE
  mcc_exit_reason

SIDE EFFECTS
  None.
===========================================================================*/

mcc_exit_name_type mc_map_gen_cmd_to_mcc_exit_reason
(
  prot_gen_cmd_s_type *gen_cmd_ptr
    /* Pointer to the MMoC's generic command.  */
)
{
  mcc_exit_name_type exit_reason = MCC_PREF_SYS_CHANGED;

  switch ( gen_cmd_ptr->cmd_type )
  {
    case PROT_GEN_CMD_PREF_SYS_CHGD:
      exit_reason = MCC_PREF_SYS_CHANGED;
      break;

    case PROT_GEN_CMD_SS_TIMER_EXP:
      exit_reason = MCC_RESELECTION;
      break;

    case PROT_GEN_CMD_GET_NETWORKS_GW:
      exit_reason = MCC_PREF_SYS_CHANGED;
      break;

    case PROT_GEN_CMD_TERM_GET_NETWORKS_GW:
      exit_reason = MCC_PREF_SYS_CHANGED;
      break;

    default:
      ERR_FATAL("Unknown deact_reason", 0, 0, 0 );
      break;

  } /* switch ( deact_reason ) */

  if (exit_reason == MCC_RESELECTION)
  {
    event_report(EVENT_PREF_SYS_RESEL);
  }

  return exit_reason;
 
} /* mc_map_gen_cmd_to_mcc_exit_reason() */

/*===========================================================================

FUNCTION MCC_GET_SLOT_CYCLE_INDEX

DESCRIPTION
  Returns the slot cycle index preferred by the user.

DEPENDENCIES

RETURN VALUE
  Slot cycle index.

SIDE EFFECTS

===========================================================================*/

int8 mcc_get_slot_cycle_index
(
  void
)
{
  return cdma.slot_cycle_index_p;

} /* mcc_get_slot_cycle_index */

#ifdef FEATURE_HDR
#error code not present
#endif /* FEATURE_HDR */

#ifdef FEATURE_DBM
/*===========================================================================

FUNCTION MCC_SDB_SUPPORTED

DESCRIPTION
  This function returns if the Short Data Burst is supported by the current
  Base Station.

DEPENDENCIES
  None.

RETURN VALUE
  TRUE or FALSE.

SIDE EFFECTS
  None.

===========================================================================*/

boolean mcc_sdb_supported
(
  void
)
{
  /* Only when both the MS and BS supports SDB would we claim that SDB is 
     supported. */
  if ( P_REV_IS_6_OR_GREATER && mccap_item_is_supported(MCCAP_DF_SDB) )
  {
    if ( MAIN_STATE(cdma.curr_state) == CDMA_TC ) 
    {
      return cdma.sdb_supported;
    }
    else if ( ( OVHD_CHAN_IS_PCH && ESP_MSG_IS_RECVD )
              #ifdef FEATURE_IS2000_REL_A_CC
              || ( OVHD_CHAN_IS_BCCH && ANSI41SP_MSG_IS_RECVD )
              #endif /* FEATURE_IS2000_REL_A_CC */ 
            )
    {
      return cur_bs_ptr->csp.esp.sdb_supported;
    }
  }

  return FALSE;

} /* mcc_sdb_supported */
#endif /* FEATURE_DBM */

/*===========================================================================

FUNCTION mcc_set_current_preferred_sci

DESCRIPTION
  This sets the current preferred SCI value. The preferred SCI in NV is 
  not updated
  
DEPENDENCIES
  None.

RETURN VALUE
  The preferred sci value MS is actually set to

SIDE EFFECTS
  None.

===========================================================================*/

int8 mcc_set_current_preferred_sci
(
  int8 sci_p
)
{
  if (sci_p == MCC_NULL_SCI)
  {
    cdma.curr_sci_pref = cdma.slot_cycle_index_p;
  }
  else
  {
    cdma.curr_sci_pref = sci_p;

    /* Upper bound check */
    if (sci_p > MCC_MAX_SCI)
    {
      cdma.curr_sci_pref = MCC_MAX_SCI;
    }

    /* Lower bound check */
    if (sci_p < MCC_MIN_SCI)
    {
      cdma.curr_sci_pref = MCC_MIN_SCI;
    }
  } 

  return cdma.curr_sci_pref;

} /* mcc_set_current_preferred_sci */

/*===========================================================================

FUNCTION mcc_get_current_preferred_sci

DESCRIPTION
  Gets the current preferred SCI value. This may be different from the one
  that is stored in NV.
  
DEPENDENCIES
  None.

RETURN VALUE
  The current preferred sci value.

SIDE EFFECTS
  None.

===========================================================================*/

int8 mcc_get_current_preferred_sci
(
  void
)
{
  return cdma.curr_sci_pref;

} /* mcc_get_current_preferred_sci */

/*===========================================================================

FUNCTION mcc_set_last_sci_reg

DESCRIPTION
  This sets the last registered SCI value. The last registered SCI in NV is  
  not updated
  
DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/

void mcc_set_last_sci_reg
(
  int8 sci
)
{
  cdma.last_sci_reg = sci;

} /* mcc_set_last_sci_reg */

/*===========================================================================

FUNCTION mcc_get_last_sci_reg

DESCRIPTION
  Gets the last registered SCI value. This may be different from the one
  that is stored in NV.
  
DEPENDENCIES
  None.

RETURN VALUE
  The last registered sci value.

SIDE EFFECTS
  None.

===========================================================================*/

int8 mcc_get_last_sci_reg
(
  void
)
{
  return cdma.last_sci_reg;

} /* mcc_get_last_sci_reg */

/*===========================================================================

FUNCTION MCC_CONVERT_PREF_SCI_TO_OTA_SCI

DESCRIPTION
  
  This function converts the preferred SCI from internal format to OTA format 
  to be packed by parser.
  
  If the auto_msg_supported bit in ESP is set, it uses the following
  conversion mapping for Negative SCI.
                 
    Preferred SCI               OTA SCI
    -------------------------   -------
      0 - 1.28s (16 slots)       000
      1 - 2.56s (32 slots)       001
      2 - 5.12s (64 slots)       010
      3 - 10.24s (128 slots)     011
     -4 - 0.080s (1 slot)        100
     -3 - 0.160s (2 slots)       101
     -2 - 0.320s (4 slots)       110
     -1 - 0.640s (8 slots)       111
  

  Otherwise, it uses the following conversion mapping:

    Preferred SC  P_REV_IN_USE  Negative SCI supported  OTA SCI
    ------------- ------------  ----------------------  ---------------------
    > 0           N/A           N/A                     Same as preferred SCI
    < 0           < 11          N/A                     0
    < 0           >= 11         No                      0
    < 0           >= 11         Yes                     Same as preferred SCI
  
DEPENDENCIES
  None.

RETURN VALUE
  Preferred SCI value to be transmitted over the air

SIDE EFFECTS
  None.

===========================================================================*/

int8 mcc_convert_pref_sci_to_ota_sci
(
  void
)
{
  int8 ota_pref_sci = mcc_get_current_preferred_sci();

  #ifdef FEATURE_MODIFIED_SCI
#error code not present
  #endif /* FEATURE_MODIFIED_SCI */

  {
    #ifdef FEATURE_IS2000_REL_D_NEG_SCI
#error code not present
    #endif /* FEATURE_IS2000_REL_D_NEG_SCI */    
    {
      /* We are talking to a pre release D BS or Neg SCI is 
         not supported
       */
      if (ota_pref_sci < 0)
      {
        ota_pref_sci = 0;
      }
    }
  }

  return ota_pref_sci;

} /* mcc_convert_pref_sci_to_ota_sci */

/*===========================================================================

FUNCTION MCC_SET_OPERATIONAL_SCI

DESCRIPTION
  
  This function updates the operation SCI, slot_cycle_index_s
  from OTA MAX_SCI.
  
  If the auto_msg_supported bit in ESP is set, it uses negative SCIp,
  otherwise it used positive SCIp. The conversion mapping for Negative SCI
  is as follows

DEPENDENCIES
  None.

RETURN VALUE
  TRUE   - if overheads are current, slot_cycle_index_s is set appropriately.
  FALSE  - if overheads are not current, slot_cycle_index_s cannot be set.

SIDE EFFECTS
  slot_cycle_index_s is updated.

===========================================================================*/

boolean mcc_set_operational_sci
(
  void
)
{
  int8 sci_s   = 0;
#ifdef FEATURE_IS2000_REL_D
#error code not present
#endif

  if ( ( OVHD_CHAN_IS_PCH && SP_MSG_IS_RECVD && ESP_MSG_IS_RECVD )
      #ifdef FEATURE_IS2000_REL_A_CC
       || ( OVHD_CHAN_IS_BCCH && ANSI41SP_MSG_IS_RECVD && MCRRP_MSG_IS_RECVD )
      #endif /* FEATURE_IS2000_REL_A_CC */
     )
  {
    if ( mccreg_find_sid_nid( cur_bs_ptr->csp.sp.sid, 
                              cur_bs_ptr->csp.sp.nid ) != NULL ) 
    {
      /* Found the sid,nid. We are registered so use best slot_cycle */

      MSG_HIGH("SID %d NID %d FOUND on SID_NID list", cur_bs_ptr->csp.sp.sid,
        cur_bs_ptr->csp.sp.nid, 0);

#if defined (FEATURE_MODIFIED_SCI)  && defined (FEATURE_IS2000_REL_D)  
      if ( mcc_modified_sci_is_supported() )
      {
        /* This BS supports Negative SCI */
        sci_s = MIN( cdma.neg_slot_cycle_index_p,
                     cur_bs_ptr->csp.sp.max_slot_cycle_index );
      }
      else 
#endif /* FEATURE_MODIFIED_SCI */
      {
#ifdef FEATURE_IS2000_REL_D
#error code not present
 #else
        /* Standard for Rev 0, A, B, C Says - the mobile station shall use 
           a slot cycle index no greater than SLOT_CYCLE_INDEXs.           */
        MSG_MED("Updating Operational SCI , MS Preferred SCI = %d, BS Preferred SCI = %d",
                cdma.curr_sci_pref,cur_bs_ptr->csp.sp.max_slot_cycle_index,0);
        sci_s = MIN( cdma.curr_sci_pref, cur_bs_ptr->csp.sp.max_slot_cycle_index ); 
#endif
      }
    }
    else
    {
      /* -----------------------------------
      ** Haven't registered on this sid, nid
      ** pair.  Have to return to default.
      ** ----------------------------------- */
      MSG_HIGH("SID %d NID %d NOT FOUND on SID_NID list",cur_bs_ptr->csp.sp.sid,
        cur_bs_ptr->csp.sp.nid, 0);
      mcc_set_last_sci_reg(MCC_DEFAULT_SCI);
      sci_s = MIN( MCC_DEFAULT_SCI, cur_bs_ptr->csp.sp.max_slot_cycle_index ); /*lint !e734*/
    }

    mccidl_set_slot_cycle_index_s(sci_s);

    /* slot_cycle_index_s is set appropriately.. return TRUE */
    return TRUE;

  }

  MSG_HIGH("mcc_update_operational_sci returning FALSE", 0, 0, 0);

  /* slot_cycle_index_s is NOT set.. return FALSE */
  return FALSE;

} /* mcc_set_operational_sci */

/*===========================================================================

FUNCTION MCC_CALCULATE_SCI_MASK

DESCRIPTION
  Returns (power(2,i) - 1), where i = solt cycle index

DEPENDENCIES
  None.

RETURN VALUE
  sci_mask
  
SIDE EFFECTS
  None
  
===========================================================================*/

word mcc_calculate_sci_mask
(
  int8 sci
)
{
  int16 neg_sci = 0;
  /* calculate the (number of slots - 1) in a slot cycle based on the SCI 
   * (negative SCI values are permitted)
   */

   if (sci < 0)
   {
     neg_sci = -1 * sci;
     return ((16L >> neg_sci) - 1);  
   }
   else 
   {
     return ((16L << sci) - 1);  /*lint !e734 */
   }

} /* mcc_calculate_sci_mask */

/*===========================================================================

FUNCTION MCC_ADD_GROUP_IN_BAND_TABLE

DESCRIPTION
  Adds the specified group in the table. It also optionally adds all the
  band classes associated with it. If the bad group already exists, it will
  not add a duplicate group. Only band classes, if any associated with 
  this group are added.

DEPENDENCIES
  None.

RETURN VALUE
  Boolean - TRUE if successful, FALSE otherwise

SIDE EFFECTS
  None.

===========================================================================*/

boolean mcc_add_group_in_band_table
( 
  cdma_band_class_group_type gp,
  uint8 num_bands,
  uint8 * band_class
)
{
  uint8 group_cnt;
  uint8 band_cnt;
  uint8 i;

  if (!MCC_BAND_GROUP_IN_RANGE(gp))
  {
    MSG_HIGH("Illegal Band Group %d",gp,0,0);
    return FALSE;
  }

  /* Find or add the new group */
  for (group_cnt = 0; group_cnt < MAX_BAND_GROUPS; group_cnt++)
  {
    if ((mcc_band_table[group_cnt].band_group == gp) ||
        (mcc_band_table[group_cnt].band_group == MCC_MIN_GROUP))
    {
      mcc_band_table[group_cnt].band_group = gp;
      break;
    }
  }

  if (group_cnt >= MAX_BAND_GROUPS)
  {
    /* There is no room in band class table and gp does not 
       already exist in it
    */

    ERR_FATAL("Band Class Table FULL",0,0,0);
  }
  
  /* Now add the band classes in that group */
  for (band_cnt = 0; band_cnt < num_bands; band_cnt++)
  {
    for (i = 0; i < mcc_band_table[group_cnt].num_bands; i++ )
    {
      if (band_class[band_cnt] == mcc_band_table[group_cnt].band_class[i])
      {
        MSG_HIGH("Band Class %d already in grp %d",
                   band_class[band_cnt],
                   mcc_band_table[group_cnt].band_group,
                   0);
        break;
      }
    } /* for i */

    if (i >= mcc_band_table[group_cnt].num_bands)
    {
      /* The band class was not found under the group gp.
         Add it if there is room in the band class array.
      */

      if (i < CAI_MAX_BAND_CLASSES)
      {
        /* There is room in the band class array. */
        mcc_band_table[group_cnt].band_class[i] = band_class[band_cnt];
        mcc_band_table[group_cnt].num_bands++ ;
      }
      else
      {
        /* This should never happen.  We have added more band classes
           to the group than there are band classes. */
        ERR_FATAL("Band class array full for this group",0,0,0);
      }

    } /* if */
  } /* for band_cnt */
  
  return TRUE;
 
} /* mcc_add_group_in_band_table */

/*===========================================================================

FUNCTION MCC_ADD_BAND_CLASS_IN_GROUP
  Adds the specified band class in the specified group. It calls
  mcc_add_group_in_band_table to perform its task.
  
DESCRIPTION

DEPENDENCIES
  None.

RETURN VALUE
  Boolean - TRUE if successful, FALSE otherwise

SIDE EFFECTS
  None.

===========================================================================*/

boolean mcc_add_band_class_in_group
(
  cdma_band_class_group_type gp,
  uint8 band_class
)
{
  return mcc_add_group_in_band_table(gp, 1, &band_class);

} /* mcc_add_band_class_in_group */

/*===========================================================================

FUNCTION MCC_REMOVE_BAND_CLASS_FROM_GROUP
  Removes the band class from the specified group

DESCRIPTION

DEPENDENCIES
  None.

RETURN VALUE
  Boolean - TRUE if successful, FALSE otherwise

SIDE EFFECTS
  None.

===========================================================================*/

boolean mcc_remove_band_class_from_group
(
  cdma_band_class_group_type gp,
  uint8 band_class
)
{
  uint8 group_cnt = 0;
  uint8 band_cnt = 0;
  uint8 i = 0;

  if (!MCC_BAND_GROUP_IN_RANGE(gp))
  {
    MSG_HIGH("Illegal Band Group %d",gp,0,0);
    return FALSE;
  }

  /* Find group gp */
  for (group_cnt = 0; group_cnt < MAX_BAND_GROUPS; group_cnt++)
  {
    if (mcc_band_table[group_cnt].band_group == gp)
    {
      break;
    }
  } /* for group_cnt */

  if (group_cnt >= MAX_BAND_GROUPS)
  {
    /* The group gp does not exist in mcc_band_table */
    MSG_HIGH("Group %d not present in band class table",gp,0,0);
    return FALSE;
  }
  
  /* Find the band class in group gp */
  for (band_cnt = 0; 
       band_cnt < mcc_band_table[group_cnt].num_bands; 
       band_cnt++)
  {
    if (mcc_band_table[group_cnt].band_class[band_cnt] == band_class)
    {
      /* band_class to remove is present in group gp
      */
      break;

    }
  } /* for band_cnt */

  if (band_cnt >= mcc_band_table[group_cnt].num_bands)
  {
    return FALSE;
  }
      
  /* Shift all the following band classes one position to the front */
  for (i = band_cnt + 1; 
           i < mcc_band_table[group_cnt].num_bands; 
           i++)
  {
    mcc_band_table[group_cnt].band_class[i - 1] = 
      mcc_band_table[group_cnt].band_class[i];
  } 

  /* Decrement the num_bands to reflect the deletion of a band class 
  */
  mcc_band_table[group_cnt].num_bands--;

  /* The band_class from group gp is removed from mcc_band_table */
  return TRUE;

} /* mcc_remove_band_class_from_group */

/*===========================================================================

FUNCTION MCC_REMOVE_BAND_CLASS
  Removes the first encountered band class entry in any group. It calls
  mcc_remove_band_class_from_group to perform its job.

DESCRIPTION

DEPENDENCIES
  None.

RETURN VALUE
  Boolean - TRUE if successful, FALSE otherwise.

SIDE EFFECTS
  None.

===========================================================================*/

boolean mcc_remove_band_class
(
  uint8 band_class
)
{
  
  uint8 group_cnt = 0;
  boolean status = FALSE;

  /* For each group */
  for (group_cnt = 0; group_cnt < MAX_BAND_GROUPS; group_cnt++)
  {
    if (mcc_band_table[group_cnt].band_group != MCC_MIN_GROUP)
    {
      /* Check if the band class is in this group */
      status = mcc_remove_band_class_from_group(
                   mcc_band_table[group_cnt].band_group,
                   band_class);

    }

    if (status)
    {
      /* band_class has been found in this group */
      return status;
    }
  } /* for group_cnt */
  
  /* The band class does not exist in the table */
  MSG_ERROR("Band Class %d not present in table",band_class,0,0);

  return FALSE;

} /* mcc_remove_band_class */

/*===========================================================================

FUNCTION MCC_PRINT_BAND_CLASS_TABLE

DESCRIPTION
  Prints the band class table.

DEPENDENCIES
  None.

RETURN VALUE
  None

SIDE EFFECTS
  None.

===========================================================================*/

void mcc_print_band_class_table
(
  void
)
{
  uint8 group_cnt = 0;
  uint8 band_cnt = 0;

  MSG_HIGH("Printing Band Class Table",0,0,0);
  /* For each group */
  for (group_cnt = 0; group_cnt < MAX_BAND_GROUPS; group_cnt++)
  {
    MSG_HIGH("Band Group: %d, Num Bands: %d",
                   mcc_band_table[group_cnt].band_group,
                   mcc_band_table[group_cnt].num_bands,
                   0);
    for (band_cnt = 0; 
         band_cnt < mcc_band_table[group_cnt].num_bands; 
         band_cnt++)
    {
      MSG_HIGH("Band Class: %d",
                    mcc_band_table[group_cnt].band_class[band_cnt],
                    0,
                    0);

    }
  }

} /* mcc_print_band_class_table */

/*===========================================================================

FUNCTION MCC_BAND_CLASS_INIT

DESCRIPTION
  Initializes the band class table.

DEPENDENCIES
  None.

RETURN VALUE
  None

SIDE EFFECTS
  None.

===========================================================================*/

void mcc_band_class_init
(
  void
)
{
  /* Initialize the table with band classes */

  sys_band_mask_type band_mask = rfm_get_band_mask(RFCOM_TRANSCEIVER_0);

  if (SYS_BAND_MASK_BC0 & band_mask)
  {
    /* 800 MHz cellular band */
    (void) mcc_add_band_class_in_group(MCC_CELLULAR_GROUP, CAI_BAND_CLASS0); 
  }

  if (SYS_BAND_MASK_BC1 & band_mask)
  {
    /* 1.8 to 2.0 GHz band for PCS */
    (void) mcc_add_band_class_in_group(MCC_PCS_GROUP, CAI_BAND_CLASS1); 
  }

  if (SYS_BAND_MASK_BC3 & band_mask)
  {
    /* Japanese 800 MHz cellular band */
    (void) mcc_add_band_class_in_group(MCC_CELLULAR_GROUP, CAI_BAND_CLASS3); 
  }

  if (SYS_BAND_MASK_BC4 & band_mask)
  {
    /* Korean PCS band */
    (void) mcc_add_band_class_in_group(MCC_PCS_GROUP, CAI_BAND_CLASS4); 
  }

  if (SYS_BAND_MASK_BC5 & band_mask)
  {
    /* Band Class 5 */
    (void) mcc_add_band_class_in_group(MCC_PCS_GROUP, CAI_BAND_CLASS5); 
  }

  if (SYS_BAND_MASK_BC6 & band_mask)
  {
    /* Band Class 6 */
    (void) mcc_add_band_class_in_group(MCC_PCS_GROUP, CAI_BAND_CLASS6); 
  }

  if (SYS_BAND_MASK_BC7 & band_mask)
  {
    /* Band Class 7 */
    (void) mcc_add_band_class_in_group(MCC_PCS_GROUP, CAI_BAND_CLASS7); 
  }

  if (SYS_BAND_MASK_BC8 & band_mask)
  {
    /* Band Class 8 */
    (void) mcc_add_band_class_in_group(MCC_PCS_GROUP, CAI_BAND_CLASS8); 
  }

  if (SYS_BAND_MASK_BC9 & band_mask)
  {
    /* Band Class 9 */
    (void) mcc_add_band_class_in_group(MCC_PCS_GROUP, CAI_BAND_CLASS9); 
  }

  if (SYS_BAND_MASK_BC10 & band_mask)
  {
    /* Band Class 10 */
    (void) mcc_add_band_class_in_group(MCC_PCS_GROUP, CAI_BAND_CLASS10); 
  }

  if (SYS_BAND_MASK_BC11 & band_mask)
  {
    /* Band Class 11 */
    (void) mcc_add_band_class_in_group(MCC_PCS_GROUP, CAI_BAND_CLASS11); 
  }

  if (SYS_BAND_MASK_BC12 & band_mask)
  {
    /* Band Class 12 */
    (void) mcc_add_band_class_in_group(MCC_PCS_GROUP, CAI_BAND_CLASS12); 
  }

  /* We do not plan to support Band Class 13, so leave it out for now. */

  if (SYS_BAND_MASK_BC14 & band_mask)
  {
    /* Band Class 14 */
    (void) mcc_add_band_class_in_group(MCC_PCS_GROUP, CAI_BAND_CLASS14); 
  }

  if (SYS_BAND_MASK_BC15 & band_mask)
  {
    /* Band Class 15 */
    (void) mcc_add_band_class_in_group(MCC_PCS_GROUP, CAI_BAND_CLASS15); 
  }
} /* mcc_band_class_init */

/*===========================================================================

FUNCTION MCC_GET_GROUP_FOR_BAND_CLASS

DESCRIPTION
  Given the band class, it returns the group it belongs to.

DEPENDENCIES
  None.

RETURN VALUE
  The group that the specified band class belongs to.
                                                           
SIDE EFFECTS
  None.

===========================================================================*/

cdma_band_class_group_type mcc_get_group_for_band_class
(
  uint8 band_class
)
{
  uint8 group_cnt = 0;
  boolean status = FALSE;

  /* For each group */
  for (group_cnt = 0; group_cnt < MAX_BAND_GROUPS; group_cnt++)
  {
    if (mcc_band_table[group_cnt].band_group != MCC_MIN_GROUP)
    {
      /* Check if the band class is in this group */
      status = mcc_is_band_class_in_group(
                   mcc_band_table[group_cnt].band_group,
                   band_class);

    }

    if (status)
    {
      /* band_class has been found in this group */
      return mcc_band_table[group_cnt].band_group;
    }
  } /* for group_cnt */

  /* Did not find a group for this band class */
  return MCC_MIN_GROUP;  

} /* mcc_get_group_for_band_class */

/*===========================================================================

FUNCTION MCC_IS_CURRENT_BAND_CLASS_IN_GROUP

DESCRIPTION
  Determines if the current Band Class (cdma.band_class) belongs to the 
  specified group.

DEPENDENCIES
  None.

RETURN VALUE
  TRUE if yes, FALSE otherwise
                                                           
SIDE EFFECTS
  None.

===========================================================================*/

boolean mcc_is_current_band_class_in_group
(
  cdma_band_class_group_type gp
)
{
  return mcc_is_band_class_in_group(gp, cdma.band_class);

} /* mcc_is_current_band_class_in_group */

/*===========================================================================

FUNCTION MCC_IS_BAND_CLASS_IN_GROUP  
  
DESCRIPTION
  Determines if the specified Band Class belongs to the specified group.

DEPENDENCIES
  None.

RETURN VALUE
  TRUE if yes, FALSE otherwise

SIDE EFFECTS
  None.

===========================================================================*/

boolean mcc_is_band_class_in_group
(
  cdma_band_class_group_type gp,
  uint8 band_class
)
{
  uint8 group_cnt = 0;
  uint8 band_cnt = 0;

  if (!MCC_BAND_GROUP_IN_RANGE(gp))
  {
    MSG_HIGH("Illegal Band Group %d",gp,0,0);
    return FALSE;
  }

  /* Find or add the new group */
  for (group_cnt = 0; group_cnt < MAX_BAND_GROUPS; group_cnt++)
  {
    if (mcc_band_table[group_cnt].band_group == gp)
    {
      break;
    }
  }

  if (group_cnt >= MAX_BAND_GROUPS)
  {
    /* Group gp does not exist in the table
    */

    MSG_ERROR("Group %d not present in table",gp,0,0);
    return FALSE;
  }

  /* Find the band class in group gp */
  for (band_cnt = 0; 
       band_cnt < mcc_band_table[group_cnt].num_bands; 
       band_cnt++)
  {
    if (mcc_band_table[group_cnt].band_class[band_cnt] == band_class)
    {
      /* band_class is present in group gp
      */
      return TRUE;
    }
  } /* for band_cnt */

  /* The band class does not exist in group gp */
  return FALSE; 

} /* mcc_is_band_class_in_group */

/*===========================================================================

FUNCTION MCC_IS_BAND_CLASS_SUPPORTED

DESCRIPTION
  Determines if the specified band class is present in any group.

DEPENDENCIES
  None.

RETURN VALUE
  TRUE if the band class is supported, FALSE otherwise.
  
SIDE EFFECTS
  None.

===========================================================================*/

boolean mcc_is_band_class_supported
(
  uint8 band_class
)
{              
  uint8 group_cnt = 0;
  boolean status = FALSE;

  #ifdef FEATURE_PASSPORT_MODE
  sd_ss_band_pref_e_type band_pref = sd_misc_get_supp_band_pref();
  #endif

  /* For each group */
  for (group_cnt = 0; group_cnt < MAX_BAND_GROUPS; group_cnt++)
  {
    if (mcc_band_table[group_cnt].band_group != MCC_MIN_GROUP)
    {
      /* Check if the band class is in this group */
      status = mcc_is_band_class_in_group(
                                         mcc_band_table[group_cnt].band_group,
                                         band_class);

    }

    if (status)
    {
      #ifdef FEATURE_PASSPORT_MODE

      if(mcc_passport_mode_is_jcdma())
      {
    
        /* The mobile can support the given band class; check the user preference, 
           which can be set by NV or UI. */
        switch (band_class)
        {
          case CAI_BAND_CLASS0:
            status = (SD_SS_BAND_PREF_BC0 & band_pref) ? TRUE : FALSE;
            break;
          case CAI_BAND_CLASS1:
            status = (SD_SS_BAND_PREF_BC1 & band_pref) ? TRUE : FALSE;
            break;
          case CAI_BAND_CLASS3:
            status = (SD_SS_BAND_PREF_BC3 & band_pref) ? TRUE : FALSE;
            break;
          case CAI_BAND_CLASS4:
            status = (SD_SS_BAND_PREF_BC4 & band_pref) ? TRUE : FALSE;
            break;
           case CAI_BAND_CLASS5:
            status = (SD_SS_BAND_PREF_BC5 & band_pref) ? TRUE : FALSE;
            break;
          case CAI_BAND_CLASS6:
            status = (SD_SS_BAND_PREF_BC6 & band_pref) ? TRUE : FALSE;
            break;
          case CAI_BAND_CLASS7:
            status = (SD_SS_BAND_PREF_BC7 & band_pref) ? TRUE : FALSE;
            break;
          case CAI_BAND_CLASS8:
            status = (SD_SS_BAND_PREF_BC8 & band_pref) ? TRUE : FALSE;
            break;
          case CAI_BAND_CLASS9:
            status = (SD_SS_BAND_PREF_BC9 & band_pref) ? TRUE : FALSE;
            break;
          case CAI_BAND_CLASS10:
            status = (SD_SS_BAND_PREF_BC10 & band_pref) ? TRUE : FALSE;
            break;
          case CAI_BAND_CLASS11:
            status = (SD_SS_BAND_PREF_BC11 & band_pref) ? TRUE : FALSE;
            break;
          case CAI_BAND_CLASS12:
            status = (SD_SS_BAND_PREF_BC12 & band_pref) ? TRUE : FALSE;
            break;
          case CAI_BAND_CLASS14:
            status = (SD_SS_BAND_PREF_BC14 & band_pref) ? TRUE : FALSE;
            break;
          case CAI_BAND_CLASS15:
            status = (SD_SS_BAND_PREF_BC15 & band_pref) ? TRUE : FALSE;
            break; 
          default:
          break;
        }
      }
      #endif /* FEATURE_PASSPORT_MODE */

      /* band_class has been found in this group */
      return status;
    }
  } /* for group_cnt */

  return FALSE;  

} /* mcc_is_band_class_supported */

/*===========================================================================

FUNCTION MCC_SET_SCM_FOR_BAND_CLASS

DESCRIPTION
  Sets the SCM bits depending on which group the supplied band class belongs

DEPENDENCIES
  None.

RETURN VALUE
  TRUE if SCM bits were successfully set, FALSE otherwise.

SIDE EFFECTS
  None.

===========================================================================*/

boolean mcc_set_scm_for_band_class
(
  uint8 band_class, 
  byte * scm
)
{
  /* ------------------------------------------------------
  ** For IS-95B backward compatibility:
  ** 1. If the phone is JSTD008 only or is talking to a JSTD
  **    BS ( P_REV_IN_USE is 1 ), we send 0xa0 as SCM.
  ** 2. If the phone has P_REV_IN_USE greater or equal 4,
  **    then we should send scm and set MSB to 1.
  ** -------------------------------------------------------*/
                                     /*lint -e506 */
  if (P_REV_IS_1)
  {
    *scm = 0xa0;
    return TRUE;
  }

  if (!mcc_is_band_class_supported(band_class))
  {
    ERR("Unsupported BAND_CLASS=%d",band_class,0,0);
    return FALSE;
  }

  if (P_REV_IS_4_OR_GREATER &&
      ((band_class == CAI_BAND_CLASS1) ||
       (band_class == CAI_BAND_CLASS4) ||
       (band_class == CAI_BAND_CLASS14)))
  {
    *scm = (cdma.scm | 0x80);
    return TRUE;
  }

  *scm = ( cdma.scm & 0x7F );

  return TRUE;

} /* mcc_set_scm_for_band_class */

#ifdef FEATURE_SO_8026
#error code not present
#endif /* FEATURE_SO_8026 */

#if (defined (FEATURE_HDR_HANDOFF) && defined (FEATURE_IS2000_REL_A))
#error code not present
#endif /* FEATURE_HDR_HANDOFF && FEATURE_IS2000_REL_A */

#ifdef FEATURE_IS2000_REL_D
#error code not present
#endif /* FEATURE_IS2000_REL_D */

/*===========================================================================
FUNCTION MCC_CELLULAR_BAND_IS_SUPPORTED

DESCRIPTION
  This function checks whether cellular band class is supported or not.

DEPENDENCIES
  None

RETURN VALUE
  TRUE if the MS supports some cellular band class

SIDE EFFECTS
  None

===========================================================================*/
boolean mcc_cellular_band_is_supported(void)
{
  return (mcc_is_band_class_supported(CAI_BAND_CLASS0) || 
          mcc_is_band_class_supported(CAI_BAND_CLASS3));

} /* mcc_cellular_band_is_supported */

/*===========================================================================
FUNCTION MCC_PCS_BAND_IS_SUPPORTED

DESCRIPTION
  This function checks whether PCS band class is supported or not.

DEPENDENCIES
  None

RETURN VALUE
  TRUE if the MS supports some PCS band class

SIDE EFFECTS
  None

===========================================================================*/

boolean mcc_pcs_band_is_supported(void)
{
  return(mcc_is_band_class_supported(CAI_BAND_CLASS1) || 
         mcc_is_band_class_supported(CAI_BAND_CLASS4) ||
         mcc_is_band_class_supported(CAI_BAND_CLASS5) ||
         mcc_is_band_class_supported(CAI_BAND_CLASS6) ||
         mcc_is_band_class_supported(CAI_BAND_CLASS7) ||
         mcc_is_band_class_supported(CAI_BAND_CLASS8) ||
         mcc_is_band_class_supported(CAI_BAND_CLASS9) ||
         mcc_is_band_class_supported(CAI_BAND_CLASS10) ||
         mcc_is_band_class_supported(CAI_BAND_CLASS11) ||
         mcc_is_band_class_supported(CAI_BAND_CLASS12) ||
         mcc_is_band_class_supported(CAI_BAND_CLASS14) ||
         mcc_is_band_class_supported(CAI_BAND_CLASS15)); 

} /* mcc_pcs_band_is_supported */

/*===========================================================================

FUNCTION MCC_SEND_CM_SRV_SYS_INFO

DESCRIPTION
  This function sends the CM task CM_SRV_SYS_INFO_F

DEPENDENCIES
  None

RETURN VALUE
  None.

SIDE EFFECTS
  None

===========================================================================*/

void mcc_send_cm_srv_sys_info
(
  word rcv_fields,
  word sid,
  word nid,
  word reg_zone,
  byte pzid 
)
{
  cm_mc_rpt_type *cmd_ptr;
  db_items_value_type db_item;

  if ((cmd_ptr = cm_mc_rpt_get_buf_else_err_fatal()) != NULL)
  {
    cmd_ptr->hdr.cmd        = CM_SRV_SYS_INFO_F     ;
    /* Indicate what info are available */
    cmd_ptr->srv_sys_info.rcv_fields = rcv_fields;
    cmd_ptr->srv_sys_info.sid = sid;
    cmd_ptr->srv_sys_info.nid = nid;
    cmd_ptr->srv_sys_info.reg_zone = reg_zone;

    cmd_ptr->srv_sys_info.band_class = cdma.band_class;
    cmd_ptr->srv_sys_info.block_or_system = cdma.block_or_sys;

    /* Get BS_P_REV from DB so the code is common for IS95A and IS95B */
    db_get(DB_BS_P_REV, &db_item);
    cmd_ptr->srv_sys_info.bs_p_rev = db_item.bs_p_rev;
    cmd_ptr->srv_sys_info.p_rev_in_use= cdma.p_rev_in_use;

    cmd_ptr->srv_sys_info.packet_zone = pzid;

    #ifdef FEATURE_IS2000_REL_A_SVD
    /** If P_REV_IN_USE is less than 7, or
    ** BS does not support CCS, or
    ** MS does not support CCS,
    ** reject the ORIG
    */
    cmd_ptr->srv_sys_info.ccs_supported =
      cdma.cs_supported && P_REV_IS_7_OR_GREATER && mccap_item_is_supported(MCCAP_DF_CS);
    #endif /* FEATURE_IS2000_REL_A_SVD */

    /* send command to ui */
    MSG_HIGH("CM SRV SYS INFO sent", 0, 0, 0);
    mcc_cm_mc_rpt( cmd_ptr );
  }
  else
  {
    ERR("No free buffers on cm_mc_rpt_free_q",0,0,0 );
  }

} /*lint !e715 mcc_send_cm_srv_sys_info */

#if (defined(FEATURE_HIGH_PSIST) && defined (FEATURE_PASSPORT_MODE))
/*===========================================================================

FUNCTION MCC_SEND_CM_PSIST_INFO

DESCRIPTION
  This function sends the CM task CM_PSIST_F

DEPENDENCIES
  None

RETURN VALUE
  None.

SIDE EFFECTS
  None

===========================================================================*/

void mcc_send_cm_psist_info
(
 cm_psist_e_type psist
)
{
  cm_mc_rpt_type *cmd_ptr;

  if ((cmd_ptr = cm_mc_rpt_get_buf_else_err_fatal()) != NULL)
  {
    cmd_ptr->hdr.cmd = CM_PSIST_F;
    cmd_ptr->psist.psist = psist;

    MSG_MED("CM PSIST INFO sent.  Psist = %d", psist, 0, 0);
    mcc_cm_mc_rpt( cmd_ptr );
  }
  else
  {
    ERR("No free buffers on cm_mc_rpt_free_q",0,0,0 );
  }
} /* mcc_send_cm_psist_info */
#endif /* FEATURE_HIGH_PSIST && FEATURE_PASSPORT_MODE */

/*===========================================================================
FUNCTION MCC_WAIT_TRANSCEIVER_RESOURCE

DESCRIPTION
  This function sends a SRCH_ACCESS_F command to Search when MC requests an
  update of transceiver resource and wait until it obtains the resource or 
  request timeout or the lost of RF resource. 

  To be safe for the coding, a timer is applied while waiting for the searcher
  report SRCH_ACCESS_READY_R. For simplicity reason, the timer value is chosen
  to be the same as T33M which is the same as the one used in 
  mcc_request_transceiver_resource.

  When the timer goes out, it does not do anything here. Mobile will come into
  Access Failure when it assumes that the transceiver resource is available.

DEPENDENCIES
  None

RETURN VALUE
  Next state to be processed

SIDE EFFECTS
  None

===========================================================================*/

mcc_transceiver_request_status_type mcc_wait_transceiver_resource
(
  srch_access_perf_enum access_perf, 
  srch_access_type_enum access_type
)
{
  mccsrch_rpt_type *rpt_ptr;
  dword wait_mask, sigs;
  boolean continue_proc = TRUE;
  mcc_transceiver_request_status_type request_status = MCC_TRANSCEIVER_RESOURCE_GRANTED;

  /* Remove all the report items from the searcher report queue if any
   * before we send the command out. In this case, we will be sure that
   * any SRCH_ACCESS_READY_R report afterwards is in response to this request
   */
  while ( (rpt_ptr = (mccsrch_rpt_type *)q_get( &mcc_srch_q )) != NULL )
  {
    /* Searcher requested system restart, need to do system exit, no point to do
       request upgrade. */
    if ( rpt_ptr->hdr.rpt == SRCH_REQUEST_SYS_RESTART_R )
    {
      MSG_HIGH( "SRCH request sys restart", 0, 0, 0 );
      continue_proc = FALSE;
      request_status = MCC_TRANSCEIVER_RESOURCE_LOST;
      cmd_done( &rpt_ptr->hdr.rpt_hdr );
      break;
    }

    /* clean up report processing */
    cmd_done( &rpt_ptr->hdr.rpt_hdr );
  }

  if ( continue_proc )
  {
    /* Clear mcc_cmd_timer and MCC_CMD_TIMER_SIG signal and timer before we use it */
    (void) rex_clr_timer( &mcc_cmd_timer );
    (void) rex_clr_sigs( &mc_tcb,  MCC_CMD_TIMER_SIG );

    /* mcc_state_time has already been associated with MCC_STATE_TIMER_SIG
     * T33m is a timer that defines maximum time to enter Update Overhead Information
     * Substate of the System Access State, which may be the most close to
     * this waiting timer scenario
     *
     * The value of T33m is 300ms      */
    (void)rex_set_timer(&mcc_cmd_timer, CAI_T33M);

    /* set parameters for Searcher command SRCH_ACCESS_F */
    mcc_srch_buf.hdr.cmd = SRCH_ACCESS_F;
    mcc_srch_buf.access.access_perf = access_perf;
    mcc_srch_buf.access.access_type = access_type;

    /* send command SRCH_ACCESS_F to searcher */
    mcc_srch_cmd(&mcc_srch_buf);

    /* in a waiting loop untile either timer expires or a
     * searcher report SRCH_ACCESS_READY_R comes in
     */

    MSG_MED("Sent blocking transceiver request to Searcher", 0, 0, 0);

    while ( continue_proc )
    {
      /* set the wait mask to be MCC_SRCH_Q_SIG | MCC_CMD_TIMER_SIG
       * so it will return either by a timer expiration, or by a 
       * Searcher Report */
      wait_mask = MCC_SRCH_Q_SIG | MCC_CMD_TIMER_SIG;

      /* block waiting here after sending searcher command out */
      sigs = mcc_wait(wait_mask);

      /* otherwise, there must be a searcher report have come in.
       * check if there is a SRCH_ACCESS_READY_R came, if not,
       * issue another mcc_wait
       */
      if ( (sigs & MCC_SRCH_Q_SIG) != 0 )
      {
        if ( (rpt_ptr = (mccsrch_rpt_type *)q_get( &mcc_srch_q )) != NULL )
        {
          if ( rpt_ptr->hdr.rpt == SRCH_ACCESS_READY_R )
          {
            MSG_HIGH( "Received the SRCH_ACCESS_READY_R", 0, 0, 0 );
            request_status = MCC_TRANSCEIVER_RESOURCE_GRANTED;
            continue_proc = FALSE;
          }

          else if ( rpt_ptr->hdr.rpt == SRCH_REQUEST_SYS_RESTART_R )
          {
            MSG_HIGH( "SRCH request sys restart", 0, 0, 0 );
            request_status = MCC_TRANSCEIVER_RESOURCE_LOST;
            continue_proc = FALSE;
          }

          else
          {
            MSG_HIGH( "Invalid SRCH  rpt %d", rpt_ptr->hdr.rpt, 0, 0 );
          }

          /* clean up report processing */
          cmd_done( &rpt_ptr->hdr.rpt_hdr );
        }
      }

      else if ( (sigs & MCC_CMD_TIMER_SIG) != 0 )
      {
        /* if the waiting returns a timer expiry, then return a FALSE */    
        MSG_HIGH( "Timer expired to get the transceiver resource", 0, 0, 0 );
        request_status = MCC_TRANSCEIVER_REQUEST_TIMEOUT;
        continue_proc = FALSE;
      }

    } /* end while */

    /* Clear mcc_cmd_timer and MCC_CMD_TIMER_SIG signal and timer before return*/
    (void) rex_clr_timer( &mcc_cmd_timer );
    (void) rex_clr_sigs( &mc_tcb,  MCC_CMD_TIMER_SIG );
  }

  return request_status;

} /* mcc_wait_transceiver_resource */

/*===========================================================================
FUNCTION MCC_REQUEST_TRANSCEIVER_RESOURCE

DESCRIPTION
  This function sends a SRCH_ACCESS_F command to Search when MC requests 
  transceiver resource before it comes into Access State from Idle state. 
  Upon receiving this command, Search will try to acquire the transeiver 
  resource with specified parameters, and send a SRCH_ACCESS_READY_R when 
  the resource is ready.

  Timer T33 (300ms) is applied to the waiting for the reply from the Searcher.

  This function returns immediately after starting the timer and sending
  the request to the Searcher.

  Functionmccidl_proc_srch_rpt in mccidl.c will process the Search report 
  SRCH_ACCESS_READY_R. Function mccidl_online in mccidl.c will process the
  timer expiry of T33m. 
  
DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/

void mcc_request_transceiver_resource
(
  srch_access_perf_enum     access_perf, 
  srch_access_type_enum     access_type,
  word                      sa_tc_state
)
{
  mccsrch_rpt_type *rpt_ptr;
  mccsrch_rpt_type mcc_srch_rpt;

  if (cdma.transceiver_request_pending)
  {
    MSG_HIGH("Updating transceiver request from %d to %d", 
      cdma.ho_sa_tc_state, sa_tc_state, 0);
  }
  else 
  {
    MSG_HIGH("Requesting transceiver for %d", sa_tc_state, 0, 0);
  }

  #ifdef FEATURE_FULL_TIME_SHDR_3
  cm_unlock_hdr_resources();
    /* Make sure HDR Search declares system loss. */
  #elif defined FEATURE_FULL_TIME_SHDR
#error code not present
  #endif /* FEATURE_FULL_TIME_SHDR_3 */

  /* set the flag to indicate there is a request pending */
  cdma.transceiver_request_pending = TRUE;

  /* save the would-be sa_tc_state */
  /* the following statement is not really to set a cdma.ho_sa_tc_state
   * for AEHO use. here the variable is borrowed for SHDR to remember
   * who was requesting the transceiver resources.
  */
  cdma.ho_sa_tc_state = sa_tc_state;

  /* set parameters for Searcher command SRCH_ACCESS_F */
  mcc_srch_buf.hdr.cmd = SRCH_ACCESS_F;
  mcc_srch_buf.access.access_perf = access_perf;
  mcc_srch_buf.access.access_type = access_type;

  /* remove SRCH_ACCESS_READY_R reports from the searcher report queue if any
   * before we send the command out. In this case, we will be sure that
   * any SRCH_ACCESS_READY_R report afterwards is in response to this request
  */

  /* get the head item first */
  rpt_ptr = (mccsrch_rpt_type *)q_check( &mcc_srch_q );
  while (rpt_ptr != NULL)
  {
    /* check the first item in the queue */                                        
    if (rpt_ptr->hdr.rpt == SRCH_ACCESS_READY_R)
    {
      /* save this node first. later on this node will be used to continue
       * the search through the searcher report queue
       */
      memcpy((void *)(&mcc_srch_rpt), rpt_ptr, sizeof(mccsrch_rpt_type));

      /* delete this node from the queue mcc_srch_q first */
      q_delete(&mcc_srch_q, &rpt_ptr->hdr.rpt_hdr.link);

      /* the following function call will put the node into the free queue */
      cmd_done( &rpt_ptr->hdr.rpt_hdr);

      /* after the node is moved into the free queue, continue the serach on
       * the report queue, starting from the next node after this one
       */
      rpt_ptr = (mccsrch_rpt_type *)q_next(&mcc_srch_q, &mcc_srch_rpt.hdr.rpt_hdr.link);

    } /* rpt_ptr->hdr.rpt == SRCH_ACCESS_READY_R */
    else
    { 
      /* go to next queue item */
      rpt_ptr = (mccsrch_rpt_type *)q_next(&mcc_srch_q, &rpt_ptr->hdr.rpt_hdr.link);
    } /* rpt_ptr->hdr.rpt != SRCH_ACCESS_READY_R */
  } /* rpt_ptr != NULL */


  /* send command SRCH_ACCESS_F to searcher */
  mcc_srch_cmd(&mcc_srch_buf);

  MSG_MED("Sent non-blocking transceiver request to Searcher (%d)", 
    cdma.ho_sa_tc_state, 0, 0);

  /* Start the callback timer mcc_tr_request_timer. Timer value is T33m
   * T33m is a timer that defines maximum time to enter Update Overhead 
   * Information Substate of the System Access State, which may be the 
   * most close to this waiting timer scenario
   *
   * The value of T33m is 300ms      */
  /* note the function will clear the timer first before it starts a new timer */
  (void)mcc_set_callback_timer(&mcc_tr_request_timer, CAI_T33M, MCC_TR_REQUEST_TIMER_ID);

} /* mcc_request_transceiver_resource */

/*===========================================================================

FUNCTION MCC_SETUP_REJECT_ORD

DESCRIPTION
  This function sets up the reject order to be sent.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/

void mcc_setup_reject_ord
(
  byte ordq,
    /* reason for rejecting the message */
  byte rej_msg_type,
    /* the type of message being rejected */
  byte rej_order,
    /* if the rej_msg_type is an order, this is the type of order */
  byte rej_ordq
    /* if the rej_msg_type is an order, this is the ordq of the rej_order */
)
{
  cdma.sa_resp_msg                      = CAI_AC_ORD_MSG;
  cdma.sa_resp_ord.rej.order            = CAI_MS_REJECT_ORD;
  cdma.sa_resp_ord.rej.ordq             = ordq;
  cdma.sa_resp_ord.rej.rej_msg_type     = rej_msg_type;
  cdma.sa_resp_ord.rej.rejected_pdu_type= 0; //20ms regular message */

  if (rej_msg_type == CAI_PC_ORD_MSG)
  {
    cdma.sa_resp_ord.rej_ord.rej_order  = rej_order;
    cdma.sa_resp_ord.rej_ord.rej_ordq   = rej_ordq;
  }

} /* mcc_setup_reject_ord */

#if defined(FEATURE_IS2000_REL_C_AKA) || defined(FEATURE_IS2000_REL_A_AES)
#error code not present
#endif /*FEATURE_IS2000_REL_C_AKA || FEATURE_IS2000_REL_A_AES */
  
/*===========================================================================

FUNCTION MCC_SETUP_VPM_AND_AKEY

DESCRIPTION
  This function performs voice privacy setup and kicks off the process of
  calculating authentication keys.
  
DEPENDENCIES
  None

RETURN VALUE
  Voice privacy mode indicator. 

SIDE EFFECTS
  None.

===========================================================================*/

boolean mcc_setup_vpm_and_akey
(
  boolean voice_privacy_desired, /*lint -esym(715, voice_privacy_desired)*/
    /* Indicates whether VPM cannot supported for the call type. */
  uint8 bs_auth_mode              /*lint -esym(715, bs_auth_mode)*/
    /* Auth mode of BS. */
)
{
  #ifdef FEATURE_AUTH
  auth_cmd_type *auth_cmd_ptr;
    /* Pointer to authentication command */
  
  mcc_calc_key_in_progress = FALSE;
    /* Default calculating authentication keys not in progress. */
  #endif /* FEATURE_AUTH */

  /* Default voice privacy not enabled */
  cdma.vp_desired = FALSE;

  #ifdef FEATURE_AUTH
  if ( voice_privacy_desired )
  {
    /* ---------------------------------------------------------
    ** Voice privacy is enabled if authentication is enabled (bs
    ** sets auth to true in Ovhds, and FEATURE_AUTH is on), and
    ** the user wants voice privacy.  The user can enable voice
    ** privacy by setting the NV item "voice_priv" under Display,
    ** or by selecting Enhanced Privacy using the UI.
    ** --------------------------------------------------------- */
  
    /* Read privacy mode from NV */
    mcc_nv_buf.item = NV_VOICE_PRIV_I;
    mcc_nv_buf.cmd = NV_READ_F;
    mcc_nv_buf.data_ptr = &mcc_nv_data;

    if (mcc_nv_cmd(&mcc_nv_buf) == NV_DONE_S)
    {
      cdma.vp_desired = ( bs_auth_mode == CAI_AUTH ) ? mcc_nv_data.voice_priv : 0;
    }
    else
    {
      cdma.vp_desired = ( bs_auth_mode == CAI_AUTH ) ? TRUE : FALSE;
    }
#ifdef CUST_EDITION
    {
      db_items_value_type sa_value;
      
      sa_value.voice_privacy = cdma.vp_desired;
      db_put( DB_VOICE_PRIVACY, &sa_value );
    }
#endif
  }

  /* ---------------
  ** Setup auth key.
  ** --------------- */
  if ( bs_auth_mode == CAI_AUTH )
  {
    if ((auth_cmd_ptr = (auth_cmd_type*) q_get(&auth_free_q)) != NULL)
    {

      #ifdef FEATURE_UIM_RUIM

      #if defined(FEATURE_UIM_RUN_TIME_ENABLE)
      /* Determine if the R-UIM is available for this NAM */
      if (nv_rtre_use_ruim_for_nam(cdma.curr_nam))
      #endif /* FEATURE_UIM_RUN_TIME_ENABLE */

      {
        MSG_MED("Start key generation", 0, 0, 0);
        auth_cmd_ptr->hdr.command = AUTH_GENERATE_KEY_F;
        auth_cmd_ptr->key.vpm_first_octet = RUIM_FIRST_OCTET_OF_VPM;
        auth_cmd_ptr->key.vpm_last_octet = RUIM_LAST_OCTET_OF_VPM;

        /* We don't request a report since calc will finish in IDLE for sure */
        auth_cmd_ptr->key.rpt_function = NULL;
      }

      #if defined(FEATURE_UIM_RUN_TIME_ENABLE)
      else /* R-UIM is not available */
      #endif /* FEATURE_UIM_RUN_TIME_ENABLE */

      #endif /* FEATURE_UIM_RUIM */

      #if !defined(FEATURE_UIM_RUIM) || defined(FEATURE_UIM_RUN_TIME_ENABLE)
      {
        MSG_MED("Start key generation", 0, 0, 0);
        auth_cmd_ptr->hdr.command = AUTH_GENERATE_KEY_F;

        /* Assume for now that we always want voice privacy */
        auth_cmd_ptr->key.do_vpm = TRUE;

        /* We don't request a report since calc will finish in IDLE for sure */
        auth_cmd_ptr->key.rpt_function = NULL;
      }
      #endif /* !FEATURE_UIM_RUIM || FEATURE_UIM_RUN_TIME_ENABLE */

      mcc_auth_cmd(auth_cmd_ptr);

      if (auth_cmd_ptr->hdr.status == AUTH_RECEIVED_S)
      {
        /* Mark calculating authentication key in progress */
        mcc_calc_key_in_progress = TRUE;
      }
      else
      {
        /* We don't want voice privacy because we can't generate keys */
        cdma.vp_desired = FALSE;
      }
    }
    else
    {
      ERR("No free buffers on auth_free_q", 0, 0, 0);
      cdma.vp_desired = FALSE;
    }
  }
  #endif /* FEATURE_AUTH */

  return cdma.vp_desired;

} /* mcc_setup_vpm_and_akey */

#ifdef FEATURE_IS2000_REL_D
#error code not present
#endif /* FEATURE_IS2000_REL_D */

/*===========================================================================

FUNCTION mcc_check_if_traf_chan_assgn_is_okay

DESCRIPTION
  This function checks to see if a received traffic channel assignment is okay
DEPENDENCIES
  None

RETURN VALUE
  The reason for rejecting the ECAM.

SIDE EFFECTS
  None

===========================================================================*/

uint8 mcc_check_if_traf_chan_assgn_is_okay
(
  mccdma_chnasn_type *chnasn_ptr /* Pointer to matched (E)CAM message */
)
{
  mccdma_ca_ord_type *ca_ptr;  /* local variable to make copying easier */
  byte asgn_mode;  /* The assign mode of the Channel Assignment */
  byte msg_type;
  mcc_fwd_chan_mask_type fwd_chan_mask= MCC_F_RESERVED; /* used for TC assigments only */
  mcc_rev_chan_mask_type rev_chan_mask= MCC_R_RESERVED; /* used for TC assignments only */

  int i;

  /* First assign some local variables to make the code more readable */
  msg_type = chnasn_ptr->msg_type;
  asgn_mode = chnasn_ptr->assign_mode;
  ca_ptr = &chnasn_ptr->mode;

  #ifdef FEATURE_IS2000_REL_D
#error code not present
  #endif /* FEATURE_IS2000_REL_D */

  #ifdef FEATURE_IS2000_REL_C
  /* *****************************************************************
     DV specific to make sure EXT_CH_IND is good to use for subsequent
     channel mask generation.
     ***************************************************************** */

  if ((msg_type == CAI_EXT_CHN_ASN_MSG) &&
      (asgn_mode == CAI_PACKET_DATA_TRAF_CHAN_ASSIGN))
  { 
    if (!P_REV_IS_9_OR_GREATER)
    {
      /* Reject DV assign mode if P_REV in use is < 9 */
      MSG_HIGH("Reject DV assign mode if P_REV in use < 9", 0, 0, 0);
      return CAI_REJ_FIELD;
    }

    else if (!mccap_is_for_pdch_supported())
    {
      /* P_REV (checked above) is OK, but DV not supported */
      MSG_ERROR("DV not supported",0, 0, 0);
      return CAI_REJ_CAP;
    }
    else if (!mccap_is_ext_ch_ind_supported(ca_ptr->am0.ext_ch_ind))
    {
      MSG_ERROR("EXT_CH_IND %d in ECAM not supported", ca_ptr->am0.ext_ch_ind,
        0, 0);
      return CAI_REJ_CAP;
    }
  }
  #endif /* FEATURE_IS2000_REL_C */


  /* ****************************************
     CAM / ECAM : Traffic Assignment specific
     **************************************** */

  if ((asgn_mode == CAI_TRAFFIC_CHAN_ASSIGN) ||
      (asgn_mode == CAI_EXT_TRAF_CHAN_ASSIGN)
      #ifdef FEATURE_IS2000_REL_C
      || ((msg_type == CAI_EXT_CHN_ASN_MSG) &&
          (asgn_mode == CAI_PACKET_DATA_TRAF_CHAN_ASSIGN))
      #endif /* FEATURE_IS2000_REL_C */
     )
  {

    /* *************************************************************
       Figure out the channel masks that is to be used in subsequent 
       validation based on CH_IND or EXT_CH_IND.
       ************************************************************* */
    uint8 ota_ext_chind = CAI_EXT_CHIND_UNSET; /*lint !e641 */

    #ifdef FEATURE_IS2000_REL_C
    if (asgn_mode == CAI_PACKET_DATA_TRAF_CHAN_ASSIGN)
    {
      ota_ext_chind = ca_ptr->am0.ext_ch_ind;
    }
    #endif /* FEATURE_IS2000_REL_C */

    fwd_chan_mask = mccccl_code_channel_list.gen_fwd_chan_mask(
                      ca_ptr->am0.ch_ind, (cai_ext_chind_type) ota_ext_chind);
    rev_chan_mask = mccccl_code_channel_list.gen_rev_chan_mask(
                      ca_ptr->am0.ch_ind, (cai_ext_chind_type) ota_ext_chind);

    
    /* Check supported channel mask that based on CH_IND and EXT_CH_IND */
    if (!snm_verify_chan_mask_for_so(fwd_chan_mask, rev_chan_mask, cdma.so_req))
    {
      #ifdef FEATURE_IS2000_REL_C_DV
#error code not present
      #endif /* FEATURE_IS2000_REL_C_DV */
      {
        MSG_ERROR("Bad CH_IND specified:%d", ca_ptr->am0.ch_ind, 0, 0);
      }

      return CAI_REJ_CAP;
    }

    /* Check band class */
    if (ca_ptr->am0.freq_incl &&
        !mcc_is_band_class_supported(ca_ptr->am0.band_class))
    {
      /* TC Assignment to this Band Class is not supported */
      MSG_ERROR("Band class %d not supported", ca_ptr->am0.band_class, 0, 0);
      return CAI_REJ_CAP;
    }

    /* Check encryption mode */
    #ifdef FEATURE_IS2000_REL_A_AES
#error code not present
    #else
    else if ( (ca_ptr->am0.encrypt_mode != CAI_ENCRYPTION_DISABLED) &&
              (ca_ptr->am0.encrypt_mode != CAI_ENCRYPT_CALL_CTL_MSGS)
            )
    {
      /* Without AES feature, we don't support Enhanced or Extended Encryption,
         we only support Basic Encryption. When we do begin to support
         Extended Encryption, we need to examine the Signaling Encrypt
         Mode fields (mode, key size, key seq) that are in this message. */
      MSG_ERROR("Encrypt mode %d not supported", ca_ptr->am0.encrypt_mode, 0, 0);
      return CAI_REJ_CAP;
    }
    #endif /* FEATURE_IS2000_REL_A_AES */
    
    /* Check unsupported DEFAULT_CONFIG */
    if (ca_ptr->am0.granted_mode == CAI_GRANT_USE_DC)
    {
      if ((ca_ptr->am0.default_config == CAI_DEF_CONFIG_FOR1_REV2) ||
          (ca_ptr->am0.default_config == CAI_DEF_CONFIG_FOR2_REV1))
      {
        /* Assignment of forward and reverse traffic channel of different
           MUX/Rate Set is not supported */
        MSG_ERROR("Default config %d not supported", ca_ptr->am0.default_config,
          0, 0);
        return CAI_REJ_CAP;
      }

      if (!P_REV_IS_6_OR_GREATER &&
          (ca_ptr->am0.default_config > CAI_DEF_CONFIG_FOR2_REV1))
      {
        /* If our p_rev_in_use is less than 6, we don't want to deal with
           new default configurations */
        MSG_ERROR("Default config %d not supported", ca_ptr->am0.default_config,
          0, 0);
        return CAI_REJ_FIELD;
      }
    }

    /* Check BYPASS_ALERT_ANSWER */
    if ((asgn_mode == CAI_TRAFFIC_CHAN_ASSIGN) ||
        (asgn_mode == CAI_EXT_TRAF_CHAN_ASSIGN))
    {
      if (!cdma.call_orig && ca_ptr->am0.bypass_alert_answer &&
          CAI_IS_SO_VOICE(cdma.so_req)) 
      {
        /* If bypass_alert_answer is set to 1 for a circuit voice service
           option, we should reject it */
        MSG_ERROR("Reject ECAM, by_pass_alert is 1 for a circuit voice SO", 0, 0, 0);
        return CAI_REJ_FIELD;
      }
    }

    /* *********************************************
       P_REV >= 6 ECAM : Traffic Assignment specific
       ********************************************* */

    if ( P_REV_IS_6_OR_GREATER 
         && ( ( msg_type == CAI_EXT_CHN_ASN_MSG )
              #ifdef FEATURE_MEID_SUPPORT
              || ( msg_type == CAI_MEID_EXT_CHN_ASN_MSG )
              #endif /* FEATURE_MEID_SUPPORT */
            ) 
         && ( ( asgn_mode == CAI_TRAFFIC_CHAN_ASSIGN ) 
              || ( asgn_mode == CAI_EXT_TRAF_CHAN_ASSIGN )
              #ifdef FEATURE_IS2000_REL_C
              || ( asgn_mode == CAI_PACKET_DATA_TRAF_CHAN_ASSIGN )
              #endif /* FEATURE_IS2000_REL_C */
            )
       )
    {
      /* Check fundicated RCs */
      if (!mccap_check_rc_for_chan(
            fwd_chan_mask & (MCC_F_FCH | MCC_F_DCCH),
            rev_chan_mask & (MCC_R_FCH | MCC_R_DCCH),
            ca_ptr->am0.for_rc, ca_ptr->am0.rev_rc))
      { 
        MSG_ERROR("Incompatible Fundicated Channels and RCs specified", 0, 0, 0);
        return CAI_REJ_CAP;
      }

      /* Check compatibility of GRANTED_MODE and DEFAULT_CONFIG */
      if ((ca_ptr->am0.granted_mode == CAI_GRANT_USE_DC) &&
          (((ca_ptr->am0.default_config == CAI_DEF_CONFIG_FOR1_REV1) &&
           ((ca_ptr->am0.for_rc != CAI_RC_1) || (ca_ptr->am0.rev_rc != CAI_RC_1)))
          || 
          ((ca_ptr->am0.default_config == CAI_DEF_CONFIG_FOR2_REV2) &&
           ((ca_ptr->am0.for_rc != CAI_RC_2) || (ca_ptr->am0.rev_rc != CAI_RC_2)))))
      {
        /* If BS consciously set an RC incompatible with default config
           we are required to reject this message */
        MSG_ERROR("Default_config %d and RC mismatch-F:%d, R:%d",
          ca_ptr->am0.default_config, ca_ptr->am0.for_rc, ca_ptr->am0.rev_rc);
        return CAI_REJ_RC_MISMATCH;
      }

      if ((fwd_chan_mask & (MCC_F_FCH | MCC_F_DCCH)) && 
          (rev_chan_mask & (MCC_R_FCH | MCC_R_DCCH)))
      {
        /* Check rate set of forward and reverse RCs */
        if (RATE_SET_OF_FWD_RC(ca_ptr->am0.for_rc) !=   /*lint !e641 */
            RATE_SET_OF_REV_RC(ca_ptr->am0.rev_rc))
        {
          /* Assymmetric rate set specified. We do not support this. */
          MSG_ERROR("Assymetric F/R rate sets not supported", 0, 0, 0);
          return CAI_REJ_CAP;
        }

        /* Check forward and reverse RCs class mismatch */
        if (RC_CLASS(ca_ptr->am0.for_rc) != RC_CLASS(ca_ptr->am0.rev_rc))
        {
          /* Mismatch RC class. We do not support this. */
          MSG_ERROR("Asymmetic RC class not supported", 0, 0, 0);
          return CAI_REJ_CAP;
        }
      }

      #ifdef FEATURE_IS2000_REL_A
      /* Check SR3 fields */
      if (P_REV_IS_7_OR_GREATER && (asgn_mode == CAI_EXT_TRAF_CHAN_ASSIGN) &&
          (ca_ptr->am0.x3fl_1xrl_incl || 
           ((fwd_chan_mask & MCC_F_FCH) && ca_ptr->am0.x3_fch_info_incl) ||
           ((fwd_chan_mask & MCC_F_DCCH) && ca_ptr->am0.x3_dcch_info_incl)||
           ((fwd_chan_mask & (MCC_F_FCH | MCC_F_DCCH)) &&
            (ca_ptr->am0.x3_fch_info_incl || ca_ptr->am0.x3_dcch_info_incl))))
      {
        /* At this point, we don't support processing of SR3 fields. */
        MSG_ERROR("3x not supported", 0, 0, 0);
        return CAI_REJ_CAP;
      }
      #endif /* FEATURE_IS2000_REL_A */

      #ifdef FEATURE_IS2000_REL_C
      /* Check message integrity  support */
      if (P_REV_IS_9_OR_GREATER && ca_ptr->am0.msg_int_info_incl &&
          mccap_is_msg_integrity_supported())
      {
        MSG_ERROR("Msg Integrity not supported %d", 
          ca_ptr->am0.msg_int_info_incl, 0, 0);
        return CAI_REJ_CAP;
      }
      #endif /* FEATURE_IS2000_REL_C */

      #ifdef FEATURE_IS2000_REL_D
#error code not present
      #endif /* FEATURE_IS2000_REL_D */

      #ifdef FEATURE_IS2000_REL_C_DV
#error code not present
      #endif /* FEATURE_IS2000_REL_C_DV */

    } /* end if (P_REV_IS_6_OR_GREATER) ... */

    /* Check pilots */
    for (i=0; i<ca_ptr->am0.num_pilots; i++)
    {
      if (((fwd_chan_mask & MCC_F_FCH) &&
           !VALID_CODE_CHN(ca_ptr->am0.pilot_rec[i].code_chan_fch))
          || ((fwd_chan_mask & MCC_F_DCCH) && 
              mccap_item_is_supported(MCCAP_DF_DCCH) &&
              !VALID_CODE_CHN(ca_ptr->am0.pilot_rec[i].code_chan_dcch)) 
         )
      {
        MSG_ERROR("Invalid FCH/DCCH code channel", 0, 0, 0);
        return CAI_REJ_FIELD;
      }

      if (((fwd_chan_mask & MCC_F_FCH) &&
           !VALID_QOF_MASK_ID(ca_ptr->am0.pilot_rec[i].qof_mask_id_fch))
          || ((fwd_chan_mask & MCC_F_DCCH) &&
              mccap_item_is_supported(MCCAP_DF_DCCH) &&
              !VALID_QOF_MASK_ID(ca_ptr->am0.pilot_rec[i].qof_mask_id_dcch)) 
         )

      {
        MSG_ERROR("Invalid FCH/DCCH qof mask id", 0, 0, 0);
        return CAI_REJ_FIELD;
      }

      if (ca_ptr->am0.pilot_rec[i].add_pilot_rec_incl)
      {
        /* The following code is not p_rev checked because
           add_pilot_rec_incl is set to true only for prev 6 and after.
           The following code should work fine for both p_rev 6 and after */
        switch (ca_ptr->am0.pilot_rec[i].pilot_rec_type)
        {
          case CAI_PILOT_REC_1X_COM_TD:
            if (!mccap_is_td_mode_supported(ca_ptr->am0.pilot_rec[i].add_pilot_rec.rec0.td_mode))
            {
              MSG_ERROR("TD mode %d not supported",
                ca_ptr->am0.pilot_rec[i].add_pilot_rec.rec0.td_mode, 0, 0);
              return CAI_REJ_CAP;
            }
            break;

          #ifdef FEATURE_IS2000_REL_A
          case CAI_PILOT_REC_1X_AUX:
          case CAI_PILOT_REC_1X_AUX_TD:
            /* No AUX supported for now */
            MSG_ERROR("Pilot rec type %d not supported",
                ca_ptr->am0.pilot_rec[i].pilot_rec_type, 0, 0);
            return CAI_REJ_FIELD;
            /* break; */
          #endif /* FEATURE_IS2000_REL_A */

          default:
            MSG_HIGH("Unsupported add. pilot rec type %d", 
              ca_ptr->am0.pilot_rec[i].pilot_rec_type, 0, 0);
            return CAI_REJ_FIELD;
        } /* end switch */
      } 

      #ifdef FEATURE_IS2000_REL_D_DV
#error code not present
      #endif /* FEATURE_IS2000_REL_D_DV */

    } /* end for */
  } /* end if ((asgn_mode == CAI_TRAFFIC_CHAN_ASSIGN ) || ... */

  #if defined(FEATURE_IS2000_REL_C) || defined(FEATURE_MEID_SUPPORT)
  if ( 
       #ifdef FEATURE_IS2000_REL_C
       ( P_REV_IS_9_OR_GREATER && ( msg_type == CAI_EXT_CHN_ASN_MSG ) )
       #endif /* FEATURE_IS2000_REL_C */
        
       #if defined(FEATURE_IS2000_REL_C) && defined(FEATURE_MEID_SUPPORT)
       ||
       #endif /* FEATURE_IS2000_REL_C && FEATURE_MEID_SUPPORT */

       #ifdef FEATURE_MEID_SUPPORT
       ( msg_type == CAI_MEID_EXT_CHN_ASN_MSG )
       #endif /* FEATURE_MEID_SUPPORT */
     )
  {
    if (ca_ptr->am0.plcm_type_incl)
    {
      /* --------------------------------------------------------------------
      ** An MEID ECAM is rejected if the mobile does not have an MEID; if
      ** we got here, the mobile has an MEID, and all plcm_types are allowed.
      **
      ** A p-rev 9 ECAM can always set the plcm_type to ESN-based or 
      ** bs-specified.  If the MEID feature is defined, and the mobile has an
      ** MEID, it can also set the plcm_type to IMSI_M, IMSI_T, and MEID.
      ** -------------------------------------------------------------------- */
      switch (ca_ptr->am0.plcm_type)
      {
        case CAI_PLCM_TYPE_ESN:
        case CAI_PLCM_TYPE_BS_SPEC:
          break;

        #ifdef FEATURE_MEID_SUPPORT
        case CAI_PLCM_TYPE_IMSI_O_M:
          if (cdma.imsi_type == CDMA_IMSI_T)
          {
            MSG_ERROR("PLCM_TYPE IMSI_M rcvd in ECAM while using IMSI_T",0,0,0);
            return ( CAI_REJ_PLCM_TYPE_MISMATCH );
          }
          else if (!mccap_is_meid_supported())
          {
            MSG_ERROR("Mobile has no MEID, cannot use plcm_type: %d",ca_ptr->am0.plcm_type,0,0);
            return ( CAI_REJ_CAP );
          }
          break;

        case CAI_PLCM_TYPE_IMSI_O_T:
          if (cdma.imsi_type == CDMA_IMSI_M)
          {
            MSG_ERROR("PLCM_TYPE IMSI_T rcvd in ECAM while using IMSI_M",0,0,0);
            return ( CAI_REJ_PLCM_TYPE_MISMATCH );
          }
          else if (!mccap_is_meid_supported())
          {
            MSG_ERROR("Mobile has no MEID, cannot use plcm_type: %d",ca_ptr->am0.plcm_type,0,0);
            return ( CAI_REJ_CAP );
          }
          break;

        case CAI_PLCM_TYPE_MEID:
          if (!mccap_is_meid_supported())
          {
            MSG_ERROR("Mobile has no MEID, cannot use plcm_type: %d",ca_ptr->am0.plcm_type,0,0);
            return ( CAI_REJ_CAP );
          }
          break;
        #endif /* FEATURE_MEID_SUPPORT */

          default:
            MSG_ERROR("Unknown plcm_type: %d in ECAM", ca_ptr->am0.plcm_type,0,0);
        return ( CAI_REJ_FIELD );
      } /* switch on plcm_type */
    } /* if plcm_type_incl is 1 */
  } /* end of ECAM/MEID ECAM error-checking */
  #endif /* FEATURE_IS2000_REL_C || FEATURE_MEID_SUPPORT */

  #ifdef FEATURE_IS2000_REL_C_FCS
  /* Process ECAM for stored service configuration specific */
  return ( mccsyncid_validate_ecam(chnasn_ptr) );
  #else
  return 0;
  #endif /* FEATURE_IS2000_REL_C_FCS */

} /* mcc_check_if_traf_chan_assgn_is_okay */

/*===========================================================================

FUNCTION MCC_PROC_TRAF_CHAN_ASSGN

DESCRIPTION
  This function performs traffic channel assignment processing. 

DEPENDENCIES
  None

RETURN VALUE
  None.

SIDE EFFECTS
  None

===========================================================================*/

void mcc_proc_traf_chan_assgn
(
  mccdma_chnasn_type *chnasn_ptr
    /* Pointer to (Extended) Channel Assignment Message */
)
{
  static db_items_value_type value;
    /* Holds values for database updates */
  byte num_aset;
    /* Number of pilots to be set in the active set. */

  /* local variables to make copying and comparison easier */
  mccdma_ca_ord_type *ca_ptr;
  byte asgn_mode;
  byte msg_type;

  word cdma_channel;  // Resulting CDMA channel to use (can be the same)
  byte cdma_band;     // Resulting CDMA band to use (can be the same)

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  msg_type = chnasn_ptr->msg_type;
  asgn_mode = chnasn_ptr->assign_mode;
  ca_ptr = &chnasn_ptr->mode;

  /* Default to use service negotiation. */ 
  cdma.serv_neg = TRUE;

  if ((msg_type == CAI_CHN_ASN_MSG) && (asgn_mode == CAI_TRAFFIC_CHAN_ASSIGN))
  {
    /* This assign mode implies not to use service negotiation */
    cdma.serv_neg = FALSE;
  }


  /* The union am0 holds info about a TC assignment for both CAM and ECAM
   * including the extended TC assign of CAM */

  cdma.frame_offset = ca_ptr->am0.frame_offset;
  MSG_HIGH("CAM/ECAM with frame offset %d", ca_ptr->am0.frame_offset, 0, 0);

  /* Remember the idle pgslot for use on the Traffic Channel */
  cdma.idle_pgslot = mcc_get_idle_pgslot();

  /* Make sure ENCRYPT_MODE is within range */
  #ifdef FEATURE_IS2000_REL_A_AES
#error code not present
  #else
  if ((ca_ptr->am0.encrypt_mode != CAI_ENCRYPTION_DISABLED) &&
      (ca_ptr->am0.encrypt_mode != CAI_ENCRYPT_CALL_CTL_MSGS)
     )
  #endif /* FEATURE_IS2000_REL_A_AES */
  {
    MSG_ERROR("Encrypt mode %d not supported, changed to DISABLED", ca_ptr->am0.encrypt_mode, 0, 0);
    ca_ptr->am0.encrypt_mode = CAI_ENCRYPTION_DISABLED;
  }

  /* Assume encryption is disabled, and change if it is enabled */
  cdma.encrypt_mode = CAI_ENCRYPTION_DISABLED;

  #ifdef FEATURE_AUTH
  /* --------------------------------------------------------
  ** If authentication is enabled, set encrypt mode according
  ** to whether or not we are capable of doing encryption.
  ** -------------------------------------------------------- */
  /* Build command to Layer 2 task informing it of the encrypt mode */
  mcc_rxtx_buf.tc.hdr.command = RXTX_ENCRYPT_MODE_F;

  if ( 
       #ifdef FEATURE_UMTS_1X_HANDOVER_1XMSM
#error code not present
       #endif /* FEATURE_UMTS_1X_HANDOVER_1XMSM */
       (cur_bs_ptr->rp.auth == CAI_AUTH) && mcc_calc_key_in_progress 
     ) 
  {
    /* if voice privacy is desired, we need take the encrypt_mode */
    cdma.encrypt_mode = ca_ptr->am0.encrypt_mode;
  }

  #ifdef FEATURE_IS2000_REL_A_AES
#error code not present
  #endif /* FEATURE_IS2000_REL_A_AES */

  else
  {
    /* If we couldn't start key generation, we can't do encryption */
    cdma.encrypt_mode = CAI_ENCRYPTION_DISABLED;
  }

  #ifdef FEATURE_IS2000_REL_A_AES
#error code not present
  #endif /* FEATURE_IS2000_REL_A_AES */

  mcc_rxtx_buf.encrypt_mode.encrypt_mode = cdma.encrypt_mode;

  /* Send the command to layer 2 to tell it what encrypt mode to use */
  mcc_rxtx_cmd( &mcc_rxtx_buf );
  #endif /* FEATURE_AUTH */

  MSG_MED("SNM mode: %d Encrypt mode: %d", cdma.serv_neg, cdma.encrypt_mode, 0);

  #ifdef FEATURE_IS2000_REL_D
#error code not present
  #endif /* FEATURE_IS2000_REL_D */

  {
    /* ----------------------------------------------------------------
    ** By default, the mobile uses the ESN-based public long code mask.
    ** ---------------------------------------------------------------- */
    cdma.long_code.public_lcm_type = CAI_PLCM_TYPE_ESN;
  }

  #if defined(FEATURE_IS2000_REL_C) || defined(FEATURE_MEID_SUPPORT)
  /* -----------------------------------------------------------------
  * MEID ECAM can contain a plcm_type, and so can ECAM of p-rev >= 9.
  * ----------------------------------------------------------------- */
  if (ca_ptr->am0.plcm_type_incl &&
      (
       #ifdef FEATURE_IS2000_REL_C
       (P_REV_IS_9_OR_GREATER && (msg_type == CAI_EXT_CHN_ASN_MSG))
       #endif /* FEATURE_IS2000_REL_C */

       #if defined(FEATURE_IS2000_REL_C) && defined(FEATURE_MEID_SUPPORT)
       ||
       #endif /* FEATURE_IS2000_REL_C && FEATURE_MEID_SUPPORT */

       #ifdef FEATURE_MEID_SUPPORT
       (msg_type == CAI_MEID_EXT_CHN_ASN_MSG)
       #endif /* FEATURE_MEID_SUPPORT */
     ))
  {
    /* ----------------------------------------------------------------
    ** Store the public long code mask type.
    ** ---------------------------------------------------------------- */
    cdma.long_code.public_lcm_type = (cai_plcm_type) ca_ptr->am0.plcm_type;

    if (ca_ptr->am0.plcm_type == CAI_PLCM_TYPE_BS_SPEC)  /*lint !e641 */
    {
      /* ----------------------------------------------------------------
      ** This is the base-station specified plcm, so store the plcm_39.
      ** ---------------------------------------------------------------- */
      qw_set(cdma.long_code.bs_plcm_39, qw_hi(ca_ptr->am0.plcm_39),
             qw_lo(ca_ptr->am0.plcm_39));
    }
  }
  #endif /* FEATURE_IS2000_REL_C || FEATURE_MEID_SUPPORT */

  /* ------------------------------------------
  ** Go to Traffic Channel Initialization state
  ** Note that the channel assignment message
  ** is already stored in cdma.ca.
  ** ------------------------------------------ */

  /* we can only set as much as SRCH_ASET_MAX of pilots */
  if (SRCH_ASET_MAX < ca_ptr->am0.num_pilots)
  {
    num_aset = SRCH_ASET_MAX;
  }
  else
  {
    num_aset = ca_ptr->am0.num_pilots;
  }

  /* Clear the Access Handoff Flags before going to Traffic */
  cdma.ho_state = CDMA_IDLE_HO;
  MSG_MED( "Clearing HO state, going to Traffic",0,0,0);

  /* DB_CODE_CHANNEL is used in mclog_stat() which formats a MS status
  ** packet that is typically put at the top of log files.  DB_CODE_CHANNEL
  ** is updated to 32 for Sync and set to the PCH (in practice always 0) and
  ** only here for traffic.  This item only reflects a single code, so does
  ** not handle info regarding soft handoff, nor does it allow setting of
  ** code channels for multiple channels (FCH, DCCH, etc.).  This usage
  ** needs to be revisited, but for now set this value to a hopefully
  ** obvious invalid value. */
  value.code_channel = 0xFF;
  db_put(DB_CODE_CHANNEL, &value);

  if (ca_ptr->am0.freq_incl)
  {
    cdma_band = ca_ptr->am0.band_class;
    cdma_channel = ca_ptr->am0.cdma_freq;
  }
  else
  {
    cdma_band = cdma.band_class;
    cdma_channel = cdma.cdmach;
  }

  /* Now initialize CCL */
  mccccl_code_channel_list.initialize(cdma_band, cdma_channel, num_aset,
                                      ca_ptr->am0.pilot_rec, chnasn_ptr);

  #if defined(FEATURE_IS2000_REL_C_DV) || defined(FEATURE_IS2000_DV_SIG_TEST) 
#error code not present
  #endif /* FEATURE_IS2000_REL_C_DV || FEATURE_IS2000_DV_SIG_TEST */

  #if defined(FEATURE_IS2000_REL_C_FCS) || defined(FEATURE_IS2000_REL_D)
  /* Perform initialization for Service configuration restoration */
  mccsyncid_ecam_init(chnasn_ptr);
  #endif /* FEATURE_IS2000_REL_C_FCS || FEATURE_IS2000_REL_D */

  #ifdef FEATURE_IS2000_REL_D
#error code not present
  #endif /* FEATURE_IS2000_REL_D */
 
} /* mcc_proc_traf_chan_assgn */

/*===========================================================================

FUNCTION MCC_SEND_TXC_EARLY_JUMP_CMD

DESCRIPTION
  This function sends request to Transmit task to Initialize encoder timing. 

DEPENDENCIES
  None

RETURN VALUE
  None.

SIDE EFFECTS
  None

===========================================================================*/

void mcc_send_txc_early_jump_cmd (void)
{
  caii_accparm_type *ap_ptr;
    /* Pointer to current Access Parameters Message */

  /* -----------------------------------------------------------------
  ** TXC was not being commanded to start Access Channel operation for
  ** pseudo-idle activation. Now, need to command TXC to start Access 
  ** Channel operation.
  ** ----------------------------------------------------------------- */
  if ( ( cdma.entry_ptr->entry == MCC_PSEUDO_IDLE ) &&
       ( MAIN_STATE(cdma.curr_state) != CDMA_TC ) )
  {
    mcc_tx_buf.pc.hdr.command = TXC_PC_F;
    mcc_txc_cmd(&mcc_tx_buf);
  }

  mcc_tx_buf.jump.hdr.command = TXC_JUMP_F;

  if ((cdma.sa_tc_state == CDMA_PAGE_RES) || /*lint !e641 */
      (cdma.sa_tc_state == CDMA_ORIG)     || /*lint !e641 */
      (cdma.sa_tc_state == CDMA_TCI))  /*lint !e641 */
  {
    mcc_tx_buf.jump.hold_enc_clk_on = TRUE;
  }
  else
  {
    mcc_tx_buf.jump.hold_enc_clk_on = FALSE;
  }

  if ((cdma.sa_tc_state == CDMA_TCI) || (cdma.curr_state == CDMA_TCI)  /*lint !e641 */
      #ifdef FEATURE_IS2000_REL_A_CC
      || OVHD_CHAN_IS_BCCH
      #endif /* FEATURE_IS2000_REL_A_CC */
     ) 
  {
    mcc_tx_buf.jump.rn = 0;
  }
  else
  {
    if ((ap_ptr = (caii_accparm_type *) mcc_get_msg( CAI_ACC_PARM_MSG )) != NULL)
    {
      mcc_tx_buf.jump.rn = hash( cdma.esn, (word) (1 << ap_ptr->probe_pn_ran),
        (word) (14 * (cdma.esn & 0xFFFL)) );
    }
    else
    {
      ERR_FATAL("Jump to hyperspace before updating Access Params??",0,0,0);
    }
  }

  mcc_txc_cmd( &mcc_tx_buf ); /* Send command to Transmit task */

} /* mcc_send_txc_early_jump_cmd () */

#ifdef FEATURE_IS2000_REL_D
#error code not present
#endif /* FEATURE_IS2000_REL_D */

/*===========================================================================

FUNCTION MCC_CDMA_CHANS_ARE_SUPPORTED

DESCRIPTION
  This function indentifies a list of channel frequencies that the mobile 
  supported in input_redir_rec and copies these valid channels along
  with other information in input_redir_rec into output_redir_rec.  
  It returns TRUE if at least one freq belongs to the specified band in 
  input_redir_rec.
  
DEPENDENCIES
  None

RETURN VALUE
  Boolean value indicating whether at least one freq belongs to the 
  specified band class in input_redir_rec.

SIDE EFFECTS
  None

===========================================================================*/

boolean mcc_cdma_chans_are_supported 
(
  const caii_redirect_rec2_type  *input_redir_rec,
  sd_redir_rec_cdma_s_type *output_redir_rec
)
{
  uint8 chan_index;
  uint8 num_valid_chans = 0;

  #ifdef FEATURE_IS2000_REL_D
#error code not present
  #endif /* FEATURE_IS2000_REL_D */

  if (output_redir_rec == NULL) 
  {
    MSG_ERROR("NULL pointer passed in", 0, 0, 0);
    return FALSE;
  }

  if (!mcc_is_band_class_supported(input_redir_rec->band_class)) 
  {
    MSG_MED("Unsupported band %d", input_redir_rec->band_class, 0, 0);
    return FALSE;
  }

  #ifdef FEATURE_IS2000_REL_D
#error code not present
  #endif /* FEATURE_IS2000_REL_D */ 

  for (chan_index=0; 
       chan_index<input_redir_rec->num_chans && chan_index < CAI_MAX_CDMA_CHANS;
       chan_index++) 
  {
    #ifdef FEATURE_IS2000_REL_D
#error code not present
    #endif /* FEATURE_IS2000_REL_D */ 

    if (rfm_is_band_chan_supported((sys_band_class_e_type) input_redir_rec->band_class, 
                                   input_redir_rec->cdma_chan[chan_index]))  /*lint !e661 */
    {
      /* Band subclass info is not included.  Pass only the valid list of
      ** channel frequencies to SD later */
      output_redir_rec->chan[num_valid_chans] =
        input_redir_rec->cdma_chan[chan_index];  /*lint !e661 */
      num_valid_chans++;
    }
  }

  /* Band sub_class info is not passed to SD because they don't care 
     about it */
  output_redir_rec->band_class = (sd_band_e_type) input_redir_rec->band_class;
  output_redir_rec->expected_sid = input_redir_rec->expected_sid;
  output_redir_rec->expected_nid = input_redir_rec->expected_nid;
  output_redir_rec->num_chans = num_valid_chans;
  MSG_MED("%d chans belong to the specified band/band subclass",
          num_valid_chans, 0, 0);
  return TRUE;

} /* mcc_cdma_chans_are_supported() */

/*===========================================================================

FUNCTION MCC_CHECK_IF_CSCH_SRDM_IS_OKAY

DESCRIPTION
  This function validates the service redirection message received on common
  control channel to see if it can be supported by the MS.
  
DEPENDENCIES
  None

RETURN VALUE
  TRUE indicates MS supports the service redirection. Otherwise, FALSE.

SIDE EFFECTS
  None

===========================================================================*/

boolean mcc_check_if_csch_srdm_is_okay
(
  caii_serv_redirect_type *serv_redir_ptr
)
{
  /* Check if analog redirection is supported. */
  if ( ( serv_redir_ptr->record_type == CAI_ANALOG_REDIRECTION_REC ) &&
       !( SUPPORTED_OP_MODES & IS95_ANAL_OP_MODE_MASK ) ) //lint !e506 !e774
  {
    MSG_HIGH("Redirecting to unsupported AMPS mode", 0, 0, 0);
    
    return FALSE;
  }
          
  /* Check unsupported TACS redirection. */
  else if ( ( serv_redir_ptr->record_type == CAI_TACS_REDIRECTION_REC) ||
            ( serv_redir_ptr->record_type == CAI_JTACS_REDIRECTION_REC ) )
  {
    MSG_HIGH(" Redirecting to unsupported TACS", 0, 0, 0 );

    return FALSE;
  }

  /* Check if band class of CDMA redirection is supported. */
  else if ( ( serv_redir_ptr->record_type == CAI_CDMA_REDIRECTION_REC ) &&
            !mcc_is_band_class_supported( serv_redir_ptr->redir.rec2.band_class ) )
  {
    MSG_HIGH("Redirecting to unsupported band class %d", serv_redir_ptr->redir.rec2.band_class, 0, 0);

    return FALSE;
  }

  /* Check if redirection type is supported. */ 
  else if ( !P_REV_IS_4_OR_GREATER &&
            ( serv_redir_ptr->redirect_type != CAI_NORMAL_REDIRECTION ) )
  {
    MSG_HIGH("Redirect type %d not supported", serv_redir_ptr->redirect_type, 0, 0);

    return FALSE;
  }
  
  return TRUE;
        
} /* mcc_check_if_csch_srdm_is_okay */

/*===========================================================================

FUNCTION MCC_USE_PSEUDO_BS_INFO

DESCRIPTION
  This function saves the value of cur_bs_ptr to mcc_pseudo_bs_restore_ptr 
  and changes the value of cur_bs_ptr to point to pseudo BS entry.

  It also initializes the pseudo BS entry to make it as if MS has 
  some overhead info for the target 1x system of Other Technology -> 1X 
  handover.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/

void mcc_use_pseudo_bs_info
(
  byte band_class,
  word cdma_ch
)
{
  bs_info_id_type bs_info_id; /* Fields uniquely identify BS */

  /* Save a pointer to current 1x BS info */
  mcc_saved_current_bs_ptr = cur_bs_ptr;

  /* Create pseudo BS entry and make it as the current 1x BS info. */
  bs_info_id.ovhd_chan = CAIX_PC;
  bs_info_id.pilot_pn = PSEUDO_PILOT_PN;
  bs_info_id.cdma_ch = cdma_ch;
  bs_info_id.band_class = band_class;
  bs_info_id.ovhd_walsh_ch = 0;
  mcc_entry_init(&bs_info_id, FALSE);

  /* Pretend MS received SPM, ESPM, APM from Pseudo BS. */ 
  cur_bs_ptr->rx.sp_rx = TRUE;
  cur_bs_ptr->rx.esp_rx = TRUE;
  cur_bs_ptr->rx.ap_rx = TRUE;
  cur_bs_ptr->csp.sp.config_msg_seq = INVALID_SEQ;
  cur_bs_ptr->csp.esp.config_msg_seq = INVALID_SEQ;
  cur_bs_ptr->ap.acc_msg_seq = INVALID_SEQ;

  MSG_MED( "Use pseudo BS info", 0, 0, 0);

} /* mcc_use_pseudo_bs_info */

/*===========================================================================

FUNCTION MCC_USE_1X_BS_INFO

DESCRIPTION
  This function restore the value of cur_bs_ptr back to 
  mcc_saved_current_bs_ptr.

DEPENDENCIES
  mcc_use_pseudo_bs_info must be executed prior.

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/

void mcc_use_1x_bs_info
(
  void
)
{
  if ( cur_bs_ptr->pilot_pn == PSEUDO_PILOT_PN )
  {
    /* Mark pseudo BS entry as invalid. */
    cur_bs_ptr->valid_pn = FALSE;

    /* Restore cur_bs_ptr back to 1x BS info */
    if ( mcc_saved_current_bs_ptr != NULL )
    {
      MSG_MED( "Use 1x BS info", 0, 0, 0 );
      cur_bs_ptr = mcc_saved_current_bs_ptr;
    }
    else
    {
      MSG_ERROR( "Use 1st entry of 1x BS info", 0, 0, 0 );
      cur_bs_ptr = bs_info;
    }
  
    mcc_saved_current_bs_ptr = NULL;
  }

} /* mcc_use_1x_bs_info */

#ifdef FEATURE_IS2000_REL_A_AES
#error code not present
#endif /* FEATURE_IS2000_REL_A_AES */

/*===========================================================================

FUNCTION MCC_CHANGE_IMSI_TYPE

DESCRIPTION
  This function updates the imsi type, if necessary.

DEPENDENCIES
  None.

RETURN VALUE
  TRUE if the imsi was changed; FALSE otherwise.

SIDE EFFECTS
  The cdma imsi variables may change.

===========================================================================*/

boolean mcc_change_imsi_type
(
  boolean imsi_t_supported
)
{
  boolean imsi_o_changed = FALSE;

  /* Print IMSI_T_SUPPORTED only if we received a message with IS95B format */
  if (P_REV_IS_4_OR_GREATER)
  {
    MSG_MED("IMSI_T_SUPPORTED = %d", imsi_t_supported, 0, 0);
  }

  /* ---------------------------------------------------------------------
  ** If the parser receives a pre-IS95B ESPM, it indicates that by setting
  ** p_rev field to 0. Then MC will set p_rev_in_use either to 1 or 3,
  ** depending on the current band class. Therefore, if p_rev_in_use is
  ** less than 4, we must have been on a pre-IS95B base station. If the
  ** ESPM is in new format, we assumes that P_REV field must be above 3.
  ** We are not prepared to deal with the possibility of having a new
  ** ESPM with P_REV field less than 4.
  ** ---------------------------------------------------------------------*/
  if ((P_REV_IS_4_OR_GREATER) && imsi_t_supported)
  {
    /* --------------------------------------------------------------
    ** If the received message is in IS95B format, and
    ** IMSI_T_SUPPORTED is '1', we should load IMSI_T as IMSI_O if
    ** (1) IMSI_T is not yet loaded and
    ** (2) IMSI_T is programmed ( not default value )
    ** Otherwise, leave IMSI_M in IMSI_O.
    ** -------------------------------------------------------------- */
    if (cdma.imsi_type != CDMA_IMSI_T)
    {
      /* Check to see if IMSI_T is programmed */
      if (cdma.imsi_t_s1 != mcc_default_imsi_s1() ||
          cdma.imsi_t_s2 != IMSI_S2_ZERO          ||
          cdma.imsi_t_mcc != IMSI_MCC_ZERO        ||
          cdma.imsi_t_11_12 != IMSI_11_12_ZERO)
      {
        MSG_MED("Switching to IMSI_T",0,0,0);
        cdma.imsi_s1    = cdma.imsi_t_s1;
        cdma.imsi_s2    = cdma.imsi_t_s2;
        qw_equ( cdma.imsi_s, cdma.imsi_t_s );
        cdma.imsi_11_12 = cdma.imsi_t_11_12;
        cdma.mcc        = cdma.imsi_t_mcc;
        cdma.imsi_addr_num = cdma.imsi_t_addr_num;

        /* Set IMSI type flag to IMSI_T */
        cdma.imsi_type  = CDMA_IMSI_T;
        imsi_o_changed  = TRUE;
      }
    }
  }
  else
  {
    /* ---------------------------------------------------------------
    ** If we received a pre-IS95B ESPM, or an IS-95B ESPM but
    ** IMSI_T_SUPPORTED is '0', we should load IMSI_O with IMSI_M, if
    ** IMSI_M is not yet loaded.
    ** --------------------------------------------------------------- */
    if (cdma.imsi_type != CDMA_IMSI_M)
    {
      /* Set IMSI_O to IMSI_M, IMSI_M is stored internally */

      MSG_MED("Switching to IMSI_M",0,0,0);
      cdma.imsi_s1    = cdma.imsi_m_s1;
      cdma.imsi_s2    = cdma.imsi_m_s2;
      qw_equ( cdma.imsi_s, cdma.imsi_m_s );
      cdma.imsi_11_12 = cdma.imsi_m_11_12;
      cdma.mcc        = cdma.imsi_m_mcc;
      cdma.imsi_addr_num = cdma.imsi_m_addr_num;

      /* Set IMSI type flag to IMSI_T */
      cdma.imsi_type  = CDMA_IMSI_M;
      imsi_o_changed  = TRUE;
    }
  }

  return(imsi_o_changed);

} /* mcc_change_imsi_type */

/*===========================================================================

FUNCTION MCC_SET_IMSI_CLASS_0
  This function sets the address fields for a class zero imsi.

DEPENDENCIES
  None

RETURN VALUE
  Length of the class zero imsi.

SIDE EFFECTS
  None

===========================================================================*/

byte mcc_set_imsi_class_0
(
  caii_imsi_class_0_type *imsi
    /* Pointer to imsi address fields of message to be transmitted */
)
{
  byte len;
  /* Length of the class 0 imsi generated */
  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  if (cur_bs_ptr->csp.esp.mcc == cdma.mcc)
  {
    if (cur_bs_ptr->csp.esp.imsi_11_12 == cdma.imsi_11_12)
    {
      imsi->imsi_class_0_type = 0;
      len  = 5;
      qw_equ( imsi->type.zero.imsi_s, cdma.imsi_s );
    }
    else
    {
      imsi->imsi_class_0_type = 1;
      len  = 6;
      qw_equ( imsi->type.one.imsi_s, cdma.imsi_s );
      imsi->type.one.imsi_11_12 = cdma.imsi_11_12;
    }
  }
  else if (cur_bs_ptr->csp.esp.imsi_11_12 == cdma.imsi_11_12)
  {
    imsi->imsi_class_0_type = 2;
    len  = 6;
    qw_equ( imsi->type.two.imsi_s, cdma.imsi_s );
    imsi->type.two.mcc = cdma.mcc;
  }
  else
  {
    imsi->imsi_class_0_type = 3;
    len  = 7;
    qw_equ( imsi->type.three.imsi_s, cdma.imsi_s );
    imsi->type.three.mcc = cdma.mcc;
    imsi->type.three.imsi_11_12 = cdma.imsi_11_12;
  }

  return(len);

} /* mcc_set_imsi_class_0() */

/*===========================================================================

FUNCTION MCC_SET_IMSI_CLASS_1
  This function sets the address fields for a class one imsi.

DEPENDENCIES
  None

RETURN VALUE
  Length of the class one imsi.

SIDE EFFECTS
  None

===========================================================================*/

byte mcc_set_imsi_class_1
(
  caii_imsi_class_1_type *imsi
    /* Pointer to imsi address fields of message to be transmitted */
)
{
  byte len;
  /* Length of the class 1 imsi generated */
  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  if (cur_bs_ptr->csp.esp.mcc == cdma.mcc)
  {
    imsi->imsi_class_1_type = 0;
    len  = 6;
    qw_equ( imsi->type.zero.imsi_s, cdma.imsi_s );
    imsi->type.zero.imsi_11_12 = cdma.imsi_11_12;
    imsi->type.zero.imsi_addr_num = cdma.imsi_addr_num;
  }
  else
  {
    imsi->imsi_class_1_type = 1;
    len  = 7;
    qw_equ( imsi->type.one.imsi_s, cdma.imsi_s );
    imsi->type.one.mcc = cdma.mcc;
    imsi->type.one.imsi_11_12 = cdma.imsi_11_12;
    imsi->type.one.imsi_addr_num = cdma.imsi_addr_num;
  }
  return(len);

} /* mcc_set_imsi_class_1() */

/*===========================================================================

FUNCTION MCC_SET_ADDRESS
  This function sets the address fields for a message to be transmitted
  on the access channel.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/

void mcc_set_address
(
  caii_ac_hdr_type *hdr,
    /* Pointer to header of access channel message to be transmitted */
  word             msg_type /*lint -esym(715,msg_type*/
    /* message type */
)
{
  caii_ext_sysparm_type *esp_ptr;   /* Ptr to current ext sys params msg   */
  /*lint -esym(529,esp_ptr) */

  byte    set_msid_type;         /* msid type */

  /* initialize msid_type */
  set_msid_type = CAII_NO_MSID_TYPE;

#ifdef FEATURE_IS2000_REL_D
#error code not present
#endif /* FEATURE_IS2000_REL_D */

  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  /* See IS-95A section 6.7.1.3.1.1 for detalis on msid type and len values */

  /* ---------------------------------------------------------------------
  ** For an OTASP call, MSID 0 is used if the ESP is not sent or if MSID 0
  ** is requested.  Otherwise MSID 3 is used.  Note that for JSTD-008 MSID
  ** 3 should always be used because ESP is required and MSID 0 is not
  ** Allowed.  So the order is important... Test for no ESP or MSID 0
  ** first, MSID 3 or otasp_call second. (IS-683 section 3.2 footnote 2)
  ** --------------------------------------------------------------------- */
  if ( ( set_msid_type == CAII_NO_MSID_TYPE ) &&
       ( !cur_bs_ptr->csp.sp.ext_sys_parameter ||
         ( cur_bs_ptr->csp.esp.pref_msid_type == CAI_MIN_AND_ESN_ADDR_TYPE )
       )
     ) /*lint !e774 */
  {
    hdr->msid_type = 0;
    hdr->msid_len  = 9;
    hdr->msid.type0.min1 = cdma.imsi_s1;
    hdr->msid.type0.min2 = cdma.imsi_s2;
    hdr->msid.type0.esn  = cdma.esn;
  }
  else if (
#ifdef FEATURE_IS2000_REL_D
#error code not present
#endif /* FEATURE_IS2000_REL_D */

          ( (set_msid_type == CAII_NO_MSID_TYPE ) &&
#ifdef FEATURE_OTASP
            ( ( cur_bs_ptr->csp.esp.pref_msid_type == CAI_IMSI_AND_ESN_ADDR_TYPE ) ||
              cdma.otasp_call
            )
#else
            ( cur_bs_ptr->csp.esp.pref_msid_type == CAI_IMSI_AND_ESN_ADDR_TYPE )
#endif /* FEATURE_OTASP */
          )) /*lint !e774 */
  {
    hdr->msid_type = 3;

    //    #if defined (FEATURE_IS2000_REL_D) && defined (FEATURE_RUIM)
    //    sample code for RUIM
    //    if (uim is to be used) 
    //    {
    //        /* replace esn with UIMID */
    //        hdr->msid.type3.esn = UIMID;
    //    }
    //    else
    //    #endif /* (FEATURE_IS2000_REL_D) && (FEATURE_RUIM) */
    {
      /* fill in the ESN */
      hdr->msid.type3.esn = cdma.esn;
    }

    /* IMSI_CLASS_0_ADDR_NUM denotes the phone has a class 0 IMSI */
    if (cdma.imsi_addr_num == IMSI_CLASS_0_ADDR_NUM)
    {
      hdr->msid.type3.imsi.imsi_class = 0;
      hdr->msid_len = 4 + mcc_set_imsi_class_0( &hdr->msid.type3.imsi.class_type.zero );
    }
    else
    {
      hdr->msid.type3.imsi.imsi_class = 1;
      hdr->msid_len = 4 + mcc_set_imsi_class_1( &hdr->msid.type3.imsi.class_type.one );
    }
  }
  else if ( ( ( set_msid_type == CAII_NO_MSID_TYPE ) &&
              ( cur_bs_ptr->csp.esp.pref_msid_type == CAI_IMSI_ADDR_TYPE )
            )
#ifdef FEATURE_IS2000_REL_D
#error code not present
#endif /* FEATURE_IS2000_REL_D */
          ) /*lint !e774 */
  {
    hdr->msid_type = 2;
    /* IMSI_CLASS_0_ADDR_NUM denotes the phone has a class 0 IMSI */
    if (cdma.imsi_addr_num == IMSI_CLASS_0_ADDR_NUM)
    {
      hdr->msid.type2.imsi.imsi_class = 0;
      hdr->msid_len = mcc_set_imsi_class_0( &hdr->msid.type2.imsi.class_type.zero );
    }
    else
    {
      hdr->msid.type2.imsi.imsi_class = 1;
      hdr->msid_len = mcc_set_imsi_class_1( &hdr->msid.type2.imsi.class_type.one );
    }
  }
  else if ( ( ( set_msid_type == CAII_NO_MSID_TYPE ) &&
              ( cur_bs_ptr->csp.esp.pref_msid_type == CAI_TMSI_IMSI_PREF_MSID_TYPE )
            )
#ifdef FEATURE_IS2000_REL_D
#error code not present
#endif /* FEATURE_IS2000_REL_D */
          ) /*lint !e774 */
  {
    if ( cdma.assigning_tmsi.info.tmsi_code != INVALID_TMSI_CODE )
    {
      /* Set TMSI msid type to 5 */
      hdr->msid_type = CAI_TMSI_AC_MSID_ADDR_TYPE;

      esp_ptr = (caii_ext_sysparm_type *) mcc_get_msg(CAI_EXT_SYS_PARM_MSG);

      /* Determine if full TMSI needs to be sent */
      /* or just send the TMSI code.             */
      /* Check if TMSI zone has changed */
      /* If so, then send full TMSI information - zone and code */
      if ( (esp_ptr == NULL) ||
           (qw_cmp ( esp_ptr->tmsi_zone,
                     cdma.assigning_tmsi.info.tmsi_zone ) != 0) )
      {
        /* TMSI zone has changed, so send full TMSI */
        qw_equ(hdr->msid.type5.tmsi_zone, cdma.assigning_tmsi.info.tmsi_zone);
        hdr->msid.type5.tmsi_code = cdma.assigning_tmsi.info.tmsi_code;
        hdr->msid_len = cdma.tmsi_code_len + cdma.assigning_tmsi.zone_len;

        /* Set full TMSI timer if required */
        tmsi_set_full_timer ();
      }
      else
      {
        /* TMSI zone is the same, so send TMSI code */
        qw_set ( hdr->msid.type5.tmsi_zone, 0L, 0L );
        hdr->msid.type5.tmsi_code = cdma.assigning_tmsi.info.tmsi_code;
        hdr->msid_len = cdma.tmsi_code_len;
      }
    }
    /* TMSI code is invalid, register with IMSI */
    else
    {
      hdr->msid_type = 2;
      hdr->msid.type2.imsi.imsi_class = 0;
      hdr->msid_len = mcc_set_imsi_class_0( &hdr->msid.type2.imsi.class_type.zero );
    }
  }
  else if (cur_bs_ptr->csp.esp.pref_msid_type == CAI_TMSI_IMSI_ESN_MSID_TYPE)
  {
    if ( cdma.assigning_tmsi.info.tmsi_code != INVALID_TMSI_CODE )
    {
      /* Set TMSI msid type to 5 */
      hdr->msid_type = CAI_TMSI_AC_MSID_ADDR_TYPE;

      esp_ptr = (caii_ext_sysparm_type *) mcc_get_msg(CAI_EXT_SYS_PARM_MSG);

      /* Determine if full TMSI needs to be sent */
      /* or just send the TMSI code.             */
      /* Check if TMSI zone has changed */
      /* If so, then send full TMSI information - zone and code */
      if ( (esp_ptr == NULL) ||
           (qw_cmp ( esp_ptr->tmsi_zone,
                     cdma.assigning_tmsi.info.tmsi_zone ) != 0) )
      {
        /* TMSI zone has changed, so send full TMSI */
        qw_equ(hdr->msid.type5.tmsi_zone, cdma.assigning_tmsi.info.tmsi_zone);
        hdr->msid.type5.tmsi_code = cdma.assigning_tmsi.info.tmsi_code;
        hdr->msid_len = sizeof(dword) + cdma.assigning_tmsi.zone_len;

        /* Set full TMSI timer if required */
        tmsi_set_full_timer ();
      }
      else
      {
        /* TMSI zone is the same, so send TMSI code */
        qw_set ( hdr->msid.type5.tmsi_zone, 0L, 0L );
        hdr->msid.type5.tmsi_code = cdma.assigning_tmsi.info.tmsi_code;
        hdr->msid_len = cdma.tmsi_code_len;
      }
    }
    /* TMSI code is invalid, register with IMSI and ESN */
    else
    {
      hdr->msid_type = 3;
      hdr->msid.type3.imsi.imsi_class = 0;
      hdr->msid.type3.esn = cdma.esn;
      hdr->msid_len = 4 + mcc_set_imsi_class_0( &hdr->msid.type3.imsi.class_type.zero );
    }
  }
  else
  {
    if (mcc_is_band_class_in_group(MCC_PCS_GROUP, cdma.band_class))
    {
      /* --------------------------------------------------------------
      ** Invalid PREF_MSID_TYPE -> arbitrarily default to MSID_TYPE = 1
      ** MSID_TYPE = 0 is not supported in JSTD008.
      ** -------------------------------------------------------------- */
      hdr->msid_type = 1;
      hdr->msid.type1.esn = cdma.esn;
      hdr->msid_len = 4;
    }
    else
    {
      /* --------------------------------------------------------------
      ** Invalid PREF_MSID_TYPE -> arbitrarily default to MSID_TYPE = 0
      ** -------------------------------------------------------------- */
      hdr->msid_type = 0;
      hdr->msid_len  = 9;
      hdr->msid.type0.min1 = cdma.imsi_s1;
      hdr->msid.type0.min2 = cdma.imsi_s2;
      hdr->msid.type0.esn  = cdma.esn;
    }
  }

} /* end mcc_set_address() */

#if defined(FEATURE_AUTH) && defined (FEATURE_AUTH_DIGITS)
/*===========================================================================

FUNCTION MCC_CALCULATE_AUTH_DATA

DESCRIPTION
  This function figures out what the auth_data should be for authenticating
  an origination or SMS.

DEPENDENCIES
  The digits passed in must be in a byte array and be encoded in BCD.

RETURN VALUE
  The computed value of auth_data

SIDE EFFECTS
  None

===========================================================================*/

dword mcc_calculate_auth_data
(
  byte digits[],
    /* Dialed digits or destination address digits */
  byte array_len,
    /* The number of digits in the above array */
  boolean digit_mode
    /* TRUE if digits are ASCII, FALSE if digits are DTMF */
)
{
  byte last_digit;
  /* Index of last dialed digit in cdma.dial.digits array */
  byte num_digits;
  /* The number of digits to be used in auth_data */
  byte i;
  /* Index into cdma.dial.digits array */
  dword auth_data;
  /* The auth_data that is to be sent to the authentication routine */
  byte converted_digit;
  /* Digit converted to 4 bit DTMF code */

  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
  /* ------------------------------------------------------------
  ** For origination, auth_data is last six dialed digits in BCD.
  ** For SMS, the six digits come from the destination address.
  ** If less than six digits were dialed, the most significant
  ** bits are IMSI_S1.
  ** ------------------------------------------------------------
  ** To build auth_data, I first fill it with IMSI_S1, then,
  ** starting at the least significant bits, replace four bits
  ** with the supplied digit, until I run out or get to six digits.
  ** ------------------------------------------------------------ */
  last_digit = array_len - 1;

  /* compute how many digits we are to use */
  num_digits = (array_len < 6) ? array_len : 6;

  /* Fill auth_data with IMSI_S1 */
  auth_data = mcc_get_auth_imsi_s1();

  /* Then replace however much is necessary with supplied digits */
  for (i=0; i < num_digits; i++)
  {
    /* Mask out four bits of auth_data which digit will replace */
    auth_data &= (0xFFFFFFFFL ^ (0x0FL << (i*4)));

    /* Get the next digit, which can be ascii or DTMF already */
    converted_digit = digits[last_digit - i];

    /* If digit is not DTMF, convert it to DTMF as per 6.3.12.1.6 */
    if (digit_mode)
    {
      /* Digits, star, and pound are encoded as real DTMF codes */
      if (converted_digit == '*')
      {
        converted_digit = MCC_DIAL_STAR;
      }
      else if (converted_digit == '#')
      {
        converted_digit = MCC_DIAL_PND;
      }
      else if (converted_digit == '0')
      {
        converted_digit = MCC_DIAL_0;
      }
      else if (converted_digit >= '1' && converted_digit <= '9')
      {
        converted_digit -= '0';
      }
      else
      {
        /* All others are encoded as DTMF version of last decimal digit
         * of ASCII character code.  That is, the ASCII code modulo 10.
         */
        converted_digit %= 10;
        if (converted_digit == 0)
        {
          /* A zero is encoded as '1010' */
          converted_digit = MCC_DIAL_0;
        }
      }
    }

    /* OR in next digit */
    auth_data |= ((converted_digit & 0x0FL) << (i*4));
  }

  return(auth_data);

} /* mcc_calculate_auth_data () */
#endif /* #if defined(FEATURE_AUTH) && defined (FEATURE_AUTH_DIGITS) */

/*lint +e655 +e818 */
