/*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*

             C D M A   S Y S T E M   A C C E S S   S T A T E

GENERAL DESCRIPTION
  This module contains the state machine for the CDMA Sub-system,
  System Access state.  During this state the mobile attempts to access
  the system by sending an access request or access response messages to
  the base station on the Access Channel and waiting for the appropriate
  response.

EXTERNALIZED FUNCTIONS
  cdma_sa
    The entrance procedure to the System Access State which determines
    which sub-state needs to be processed and vectors control to the
    appropriate procedure.

INITIALIZATION AND SEQUENCING REQUIREMENTS
  It is assumed that the CDMA sub-system has been initialized before the
  System Access sub-state is entered.

Copyright (c) 1990-2009 by QUALCOMM, Incorporated.  All Rights Reserved.

*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*/

/*===========================================================================

                        EDIT HISTORY FOR MODULE

This section contains comments describing changes made to the module.
Notice that changes are listed in reverse chronological order.

$PVCSPath: L:/src/asw/MSM5100/CP_REL_A/vcs/mccsa.c_v   1.89   07 Oct 2002 08:08:28   varavamu  $
$Header: //source/qcom/qct/modem/1x/cp/rel/1h08/src/mccsa.c#2 $ $DateTime: 2009/06/02 00:23:21 $ $Author: anishg $

when       who     what, where, why
--------   ---     ----------------------------------------------------------
06/01/09   ag      Featurized otasp session status under FEATURE_OTASP_STATUS
05/29/09   ag      Added support for sending OTASP session status to CM
03/13/09   jj      Fixed the discrepency for an incorrect field reference
03/09/09   ag      New call orig failure to CM for max access probe failure
01/28/09   ag      Fixed handling of maximum invalid SPC validation attempts.
11/11/08   pk      Changing all generic txc_cmd to mcc_txc_cmd to make calls to 
                   TXC for access attempts synchronous
05/15/08   bb      MVS API Update feature changes
04/08/08   pg      Featurization and general code cleanup for memory reduction
                   effort.
04/07/08   pg      Process BCSMS while waiting for authentication procedures 
                   to complete.
03/28/08   pg      Process layer 3 fields of broadcast data burst message
                   during system access substate.
03/20/08   bb      Fixed klocwork errors
01/08/08   ag      Mainlined FEATURE_LPM
01/02/08   pg      Added some comments.
12/14/07   an      Added Passport Mode
12/13/07   sb      Check ddtm only in Idle state, not System Access.
06/07/07   pg      Lint clean up.
05/11/07   pg      More Lint Cleanup.
03/19/07   pg      Lint Cleanup.
03/15/07   pg      Set auth_data to 0 for DBM under FEATURE_JCDMA.
03/09/07   an      Put FEATURE_GPSONE featurization
03/09/07   pg      removed dependency on WMS and make sure it only depends on 
                   CM for SMS.
02/23/07   sb      Added support for operational mode feature.
02/15/07   an      Backout previous changes
02/15/07   an      On T72 expiration, suspend the access probes
01/23/07   pg      Inform TXC to get back to Idle State when there is access 
                   abort so it would stuck in the Jump State.
11/21/06   sb      Added support for VOIP-1X handoffs.
11/20/06   sb      Release Orders are no longer subject to persistence tests.
11/15/06   fc      Fixed the issue of MS not retransmission of origination or
                   page response after received PCH assignment (respond = 1)
                   for the scenario of Origination/Page Respond overrided
                   regristration.
                   channel assignment that requires 
10/24/06   fc      Changes to support reverse link timing transfer 
                   enhancement for UMTS to 1x Handover.
10/19/06   sb      Release Orders are now subject to persistence testing.
09/30/06   fc      Fixed featurization.
09/29/06   fh      AKA/AES: removed the AES testing code, and eliminated the
                            AES testing featurization of 
                            FEATURE_IS2000_REL_A_AES_BS_SUPPORTS
09/29/06   fh      Added a new parameter to caix_int_to_ext to indicate whether
                   the function call is for calculating access channel message 
                   size or for parsing before transmitting the message.
09/21/06   fh      AKA/AES: added a new access state 
                     MCC_ACCESS_STOPPED_MI_AES_RESYNC -- access stopped for 
                     AKA/AES crypto-sync procedure
09/19/06   fh      AKA/AES: changed the mechanism to send Security Mode Request
                    Message. Now it is sent under CDMA_MSG rather than 
                    CDMA_ORD_RESP
09/14/06   fh      Moved AES field setting from caix.c back into mccsa.c and rxtx.c
09/13/06   fh      Changed prototype of the function mccsa_setup_auth_msg_int_fields
                   so that it can handle AKA/MACI related process
09/02/06   fh      AKA/AES: Miscellaneous code review changes
08/29/06   fh      Fixed a bug to set enc_info_incl fields in ROP messages
08/29/06   fh      AKA/AES: cleaned up the usage of AES capability functions
08/16/06   pg      Added support for release D Packet Zone Hysteresis.
08/02/06   fc      Added the SD system exit reason reporting for system lost. 
07/25/06   fh      AKA/AES: put part of lab testing that is dependent on BS AES
                            support under a temporary feature of  
                            FEATURE_IS2000_REL_A_AES_BS_SUPPORTS
07/13/06   fh      AKA/AES: fixed the issue to set encryption fields for ROP
07/11/06   fc      Updated function prototype for mcc_send_cm_srv_sys_info.
07/10/06   fc      Changed reference to cdma.use_sync_id.
07/06/06   fc      Renamed SRCH_LOST_RF_RESOURCE_R to 
                   SRCH_REQUEST_SYS_RESTART_R.
06/29/06   fh      Fixed the bug that in some cases, the free item of rxtx_q was 
                   not put back to rxtx_free_q
06/23/06   pg      RPC cleanup.
06/13/06   fh      AKA/AES: Fixed a bug -- when AES is definied, it should also
                            generate a CMEA key for Origination
06/12/06   fc      Added support for service status order.
06/12/06   fc      Added the support to share the setting of sdb_supported 
                   in idle and traffic state.
06/09/06   fc      Fixed the data corruption issue of sharing a common data 
                   structure for gsrm/egsrm and srdm processing. 
06/07/06   fh      Added Voice Service AES encryption/decryption under the features
06/02/06   fc      Cleanup umts interface.
05/31/06   pg      Changed function mcc_inform_sd_redirection() interface.
05/30/06   fc      Fixed lint errors.
05/30/06   fc      Removed obsolete MC_DATA_END_F command.
05/26/06   fh      AKA/AES: fixed a coupling issue with and without AKA/AES builds.
                   If AKA or AES is defined, sdu_key_id should not be set to 0 all 
                   the time. Instead, it should follow the AKA/AES process.
                   This change has no impact to non AKA/AES builds.
05/24/06   fh      AKA/AES: misclaneous fixes within AKA/AES features
05/24/06   fc      Fixed the issue of coupling encryption with voice privacy.
05/23/06   fc      Renamed the variable and function related to waiting for
                   authentication signature.
                   Added support for RXC_PC_FS_TMO_R.
05/18/06   pg      Check if MC is waiting for auth signature when MO_DBM is
                   received.
05/18/06   fc      Added support for SRCH_LOST_RF_RESOURCE_R.
05/19/06   fc      Renamed matched_msg, tx_int_msg, tx_ext_msg to 
                   mccsa_matched_msg, mcc_tx_int_msg, mcc_rx_int_msg.
05/17/06   fh      Merged in new development of AKA/AES features
05/12/06   fh      Changes under AES feature: the parser function caix_int_to_ext
                   carries an extra argument to indicate whether to just
                   get the buffer length for access channel messages, or parsing
                   and AES encrypting the whole message.
04/27/06   fc      Fixed Rel C featurization.
04/26/06   an      Fixed lint errors
04/25/06   fh      Fixed a bug that mixed the use of mccsa_wait_for_auth_signature
04/24/06   fc      Fixed Rel A featurization.
04/18/06   fc      Added debug messages.
04/09/06   fc      Commented out sample code for Rel D MEID RUIM support.
04/02/06   fh      For AES, set max_dialed_digits to be zero for all digit_mode
                   if AES is supported but not enabled.
03/21/06   fc      Added missing break for MC_END_F command processing.
03/14/06   pg      Added checking for call_id in MC_END_F command.
03/14/06   sb      Added fix for KDDI related to high persistence.
03/01/06   fh      Added AES support
02/07/06   fc      Mainlined FEATURE_MC_SUPPORTS_SHDR.
02/07/06   fc      Corrected the reporting of EVENT_REDIRECTION.
01/20/06   fc      Merged the following :
           fh      Added support for AKA and MI.
01/06/06   fc      Fixed lint error.
12/20/05   sb      Replaced cm_mc_rpt_free_q with cm_mc_rpt_get_buf_else_err_fatal().
11/30/05   sb      Leave System Access if ddtm is on.
10/16/05   fc      Renamed mcc_convert_to_ota_sci to 
                   mcc_convert_pref_sci_to_ota_sci.
10/31/05   sb      Added CM error code; indicates voice blocked for all mobiles.
10/28/05   an      Made band class/subclass code review changes
10/27/05   an      Removed unnecessary else part in band_subclass_report
10/18/05   va      Copied number type and number plan into orig structure all the
                   time
10/15/05   fc      Cleanup NCC featurization.
10/14/05   an      Added Band Class/Subclass Reporting Feature.
09/20/05   fc      Fixed compilation error.
09/09/05   fh      Removed the code block that send SRCH_MCC_ACC_PROBE_F to Searcher.
                   This is no longer needed and Searcher will not support it
08/29/05   grl     Changed criteria for sending search immediate or low 
                   priority access attempts.
08/19/05   fc      Obsolete CM_REORDER_F and use CM_CALL_ORIG_FAIL_F instead
07/27/05   fc      Corrected the references to use_tmsi.
07/26/05   fc      Fixed lint errors.
07/25/05   fc      Changes for Rel D Addendum f-csh messages.
07/24/05   sb      Added support for tri-band under FEATURE_T53.
07/22/05   va      Removed is_jcdma_emergency flag
07/15/05   fc      Fixed lint errors.
07/11/05   fh      Cleaned up header file including from MUX:
                     removed including of rlgc.h
                     changed txc.h to txcmc.h
                     added including of rxcmc.h
                     changed rpc_nominal_RCCH_attribute_gain_table access
                      to a function call of rlgc_get_nominal_rcch_attribute_gain()
07/08/05   fh      Made the changes for SHDR phase II:
                    Changed the function call of mcc_send_srch_access_cmd to
                    mcc_wait_transceiver_resource.
07/07/05   sb      Fixed lint error.
07/12/05   ht      Added MEID Addressing support
07/11/05   ht      Added MEID support.
06/03/05   fc      Updated debug message.
05/03/05   fc      Merged the following from MSMSHARED:
04/27/05   an       Added support for 4GV service option
04/25/05   fc      Merged the following from MSMSHARED:
04/22/05   va       Fixed typo in page_resp/Orig in accessing thx_cch_supported
04/20/04   fc       Changes for the update of reg_sid, reg_nid, reg_reg_zone, 
                    auth and rand during AHO/APHO.
04/21/05   fc      Corrected APHO criteria check used to inform SRCH whether
                   AHO and APHO is allowed.
04/19/05   fc      Merged the following from MSMSHARED:
04/19/05   an       Changed function call ds_verify_so to 
                    ds707_extif_verify_so
03/24/05   an       Replaced ds707.h by ds707_extif.h
04/10/05   fc      For DCA with unknown SO, use SO1 in PRM.
04/04/05   fc      Merged the following from MSMSHARED:
03/31/05   fc       Added support for EPZID.
03/28/05   fc      Added support for DCA.
                   Changes to use mccidl_msg_handler to process GSRM and 
                   EGSRM.
03/23/05   fc      Merged the following from MSMSHARED:
03/25/05   an       Added APHO DF check in mccsa_is_apho_allowed
03/22/05   an       Mainlined FEATURE_ENHANCED_BAND_CLASS. Eliminated 
                    FEATURE_CDMA_800 and FEATURE_CDMA_1900
03/21/05   an       Mainlined IS2000 features and implemented the new
                    Dynamic Feature Design.
03/14/05   an       Added special dup detection function for ecam
03/11/05   fc      Merged the following from MSMSHARED:
03/11/05   fc       Changes to always process Searcher report irrespectively 
                    of P_REV_IN_USE to fix mcc_srch_q buffer overflow issue.
02/25/05   fc      Merged the following from MSMSHARED:
02/17/05   sb       Added event to indicate a registration was suppressed.
02/18/05   fc      Merged the following from MSMSHARED:
02/09/05   fh       Changed including of ds.h to be ds707.h, as data team requested
02/08/05   an       Mainlined IS95B features
02/16/05   sb      Fixed lint error.
02/09/05   va      Merge following from MSMSHARED Tip:
02/03/05   fc       Fixed lint error.
01/07/05   an       Removed feature FEATURE_SMS and FEATURE_UASMS
01/25/05   sb      Changed ordq for reject case to match Standards.
01/25/05   sb      Added support for the Fast Call Setup Order.
01/10/05   va      Merged following from REL_B_C archive:
12/16/04   pg        Moved the previous change into layer2_ack_processing().
12/10/04   sb        Fixed lint warnings.
12/06/04   yll       Added interface change for apiOne.
12/02/04   sb      Added validation to make sure DV ECAM includes a DV pilot.
11/05/04   va      Merged folowing from REL_B_C archive:
10/23/04   an        In wait_for_auth_signature() removed the wait for signal
                     MCC_STATE_TIMER_SIG.
11/05/04   sb      Moved mccdv_init_from_ecam so that channel mask is initialized.
10/20/04   pg      Abort DBM transmission after receiving ECAM by sending 
                   CM CM_DBM_MO_HOLD_ORIG rather than CM_DBM_MO_ABORT so that
                   CM will resend the DBM later in traffic channel state.
10/07/04   fc      Removed redundant featurization.
10/01/04   pg      To check if the ECAM is received during a MS termination call,
                   use "if (!cdma.call_orig)" instead of 
                   "if (curr_state == CDMA_PAGE_RES)".
09/28/04   fc      Removed the presence check on R-DV parms.
09/27/04   va      Corrected featurization, for_pdch_supported
09/27/04   va      Merged from REL B/C archive
09/08/04   fc       Fixed persistence test on reconnect msg.
08/16/04   bkm      Added missing () to mc_get_band_class calls in ERR macros.
08/13/04   pg       Added featurelization to avoid compiling error.
08/11/04   pg       Reject an ECAM that sets bypass_alert_answer to '1' for a
                    circuit voice SO.
08/10/04   pg       Added DBM transmission in Page Response Substate.
07/21/04   sj       Added support for umts to 1x handover.
07/07/04   yll      Fixed the auth_data for SDB authentication.
07/01/04   pg       Reject call origination with CM_CALL_ORIG_ERR_REDIR instead
                    of CM_CALL_ORIG_ERR_ACC_FAIL for MCC_REDIRECTION.
09/24/04   va      Added support for REL D fields.
09/16/04   fc      Added support for reverse DV.
08/23/04   az      Removed setting of sign_slot_cycle_index from page response,
                   origination and registration message.
08/23/04   pg      Added support for new RETRY_TYPE: '100' and '101'.
08/18/04   fc      Removed silk incl flags on overhead messages.
08/17/04   az      Release D Negative SCI changes
08/13/04   pg      Added featurelization to avoid compiling error.
08/10/04   pg      Added DBM transmission in Page Response Substate.
08/09/04   fc      Service negotiation changes for Release C and D.
08/09/04   pg      Process bypass_alert_answer field in ECAM only during a
                   MS termination call.
08/03/04   bkm     Reject ECAM if neither ECAM nor OVHD has any DV pilot info.
07/21/04   pg      Reject an ECAM that sets bypass_alert_answer to '1' for a
                   circuit voice SO. 
07/08/04   bkm     Fixed compile issue for DV.
07/06/04   bkm     Range check REV_CQICH/ACKCH_REPS from ECAM.
06/24/04   bkm     Range check NUM_SOFT/SOFTER_SWITCHING_SLOTS from ECAM.
06/16/04   fc      Added DF support for SYNC ID and reconnect message.
06/14/04   fc      SR_ID enum changes.
06/04/04   bkm     Lint cleanup.
06/03/04   bkm     Merged the following from IS2000_REL_B_C:
  05/21/04   fc      Lint clean up.
05/28/04   bkm     Merged the following from IS2000_REL_B_C:
  05/19/04   sb      Made following merge official via integrate.
  05/14/04   sb      Merged the following from msmshared:
    04/08/04   ph      Migrate call_id to ACP if we get Analog 
                       voice chan in ECAM.
    04/08/04   fc      Fixed the indexing of registration return_cause.
    04/05/04   bkm     Changed if condition in layer2_ack_processing() so that if 
                       ECAM is rxed while txing Rel Order, that access is not stopped. 
    03/05/04   bkm     Lint cleanup.
  04/09/04   sb      Cleaned up initialization of public long code mask.
  04/08/04   fc      Fixed the indexing of registration return cause.
04/08/04   bkm     Channel mask updates.
04/01/04   bkm     Updates for more concise cai_ext_chind enums.
04/01/04   bkm     Fixed compilation issue after merge.
03/26/04   bkm     Merged the following from IS2000_REL_B_C:
  03/19/04   sb      Added support for Release C Decoupling PLCM from ESN
  03/19/04   va      Merged following from MSMSHARED:
    02/27/04   jrp     Added REACH parameter support checking.
    02/27/04   bkm     Moved typedef for access_result_type to mccdma.h, that type
                       is now needed in mccreg.c.
  03/12/04   fc      Added support for Release C fast call setup.
03/18/04   bkm     Changed a function name.
03/11/04   bkm     Fixed some Rel C featurization.
02/19/04   bkm     Set DB_CODE_CHANNEL to invalid value to indicate
                   that this DB is really obsolete wrt TC operation.
02/05/04   bkm     Merged from IS2000_REL_B_C
  02/05/04   va      Merged following from MSMSHARED:
  01/16/04   lh      Rejection of AUTH related message in a non-AUTH build.
  01/05/04   pg      Mainlined FEATURE_MULTIMODE_ARCH.
02/05/04   bkm     Removed check with obsolete macro ALLOCATED_CDMA_CHN that
                   always returned true.
01/28/04   bkm     Updates to support soft/softer switch.
01/20/04   bkm     Fix compile warnings when REL_C_DV is not defined.
01/13/04   bkm     Use REL_C_DV featurization around DV code.  Do checks in
                   mccsa_check_if_chan_assign_is_ok using channel mask instead
                   of OTA values.  Fixed setting of chind and ext_chind in
                   send_page_response_msg().
01/05/04   bkm     Misc updates for DV F-PDCH support.
12/02/03   bkm     Unified CH_IND event reporting.
                   Added EXT_CH_IND to mccccl initializer for DV.
12/02/03   bkm     Adding support for ECAM AssignMode = '101' for DV.
12/19/03   va      Merged following from MSMSHARED:
 12/16/03  fc      Added support for EVENT_SEG_UPM_ADDR_MISMATCH.
 12/04/03  fc      Changed ERR to MSG_MED for SRCH_PC_MEAS_R not honored for
                   AHO/APHO.
12/03/03   az      Changed from MSG_MED to MSG_HIGH for release order in 
                   process_pc_order_msg()
11/13/03   yll     Moved the resetting of cdma.call_orig outside the function
                   mcc_send_orig_failure().
11/11/03   yll     Added support for JCDMA emergency calls.
11/11/03   az      Removed spurious message for unexpected burst type. 
11/06/03   yll     Fixed the wrong PSIST calculation for emergency calls.
11/18/03   fc      Moved the CAM Release B obsolete processing under page
                   response and origination substate.
11/03/03   ic      In process_feature_msg() set num_call_ids to 1 
10/23/03   va      Added setting RELC fields in Orig, Page Resp
                   & Reg messages. 
10/23/03   yll     Added support for updating HDR PANID.
10/23/03   bkm     Added support for enhanced Idle and Access HO events.
10/15/03   is      Fixed incorrect bs_info.lat assignment in cell/sector info
10/03/03   bkm     Added match information to EVENT_CP_MATCHED_MSG.
09/11/03   yll     Added FEATURE_ACP to analog code.
09/04/03   va      Added SD_SS_ACC_TERM_SO_DENIED as reason to SD if acc
                   fails due to ACCT.
08/25/03   anr     added changes to support setting base_lat and base_long
                   in BSINFO struct.
08/21/03   az      Changes to the string displayed with couple of MSG_HIGH's
08/11/03   bkm     Removed support for FEATURE_SPECIAL_MDR.
08/11/03   az      Added support for FEATURE_MODIFIED_SCI
08/08/03   fc      Added support for IS2000 Release C messages.
08/08/03   sb      Added support for Alternative Band Class.
08/01/03   yll     Added support for Dynamic Feature.
07/30/03   az      Added support to return overflow size for MO SDB
07/30/03   az      Added support for FEATURE_SO_8026
07/01/03   az      Added FEATURE_ENHANCED_BAND_CLASS
06/18/03   fc      Corrected the access abort due to invalid PSIST value
                   for response messages.
06/02/03   bkm     Lint cleanup.
06/03/03   sb      Added processing of EGSRM in System Access state.
06/02/03   fc      Changes to allow APHO only in origination or page response
                   substate.
05/20/03   ph      In Origination, default prev_sid_incl, prev_nid_incl, prev_pzid_incl.
05/13/03   fc      Obsolete CAM for P_REV in use greater than or equal to 8.
04/25/03   az      In check_if_chan_assgn_is_okay(), MS would accept ECAM
                   if its prev is greater than 6 even though the band class in
                   the message is not supported. Fixed it by converting all
                   "else if" to "if".
04/11/03   fc      Fixed missing cmd_done.
04/04/03   bkm     For JCDMA, on T42 timeout and Sys Lost access failures send
                   CM CM_CALL_ORIG_ERR_ACC_FAIL reason rather treating as a
                   Reorder so that so that silent redial still occurs.
03/28/03   va      Release B changes.
03/21/03   bkm     In send_origination_msg(), set num_alt_so in tx_int_msg.orig.
03/17/03   bkm     Removed invalid err msg regarding unknown numbering plan.
03/06/03   lh      Added Retry delay update to DS.
03/06/03   va      Changed SD reason for ACCT to SD_SS_ACC_TERM_REORDER  till
                   a new reason is defined by the SD folks.
02/25/03   fc      Corrected the building of ACCESS_HO_LIST after APHO.
02/13/03   ph      Fixed erroneous F3 message.
02/07/03   yll     Fixed the enc_info_incl field in ORM, PRM and RGM
02/05/03   fc      Added the processing to ignore old handoff report for AHO
                   and APHO.
01/27/03   jqi     Add support for band class 6.
                   Fixed-CR23081 CR23082.
01/22/03   jrp     Fixed select_mode_rate() to loop over just the entries
                   supplied in the EAPM, not one past the end.
01/15/03   fc      Fixed a compiler warning.
01/10/03   fc      Changes for forward compatibility of f-csh.
12/12/02   hxw     Cleared MCC_L2_ACK_TIMER_SIG properly.
12/02/02   yll     Added event logging for matched msg, call release.
11/26/02   ph      Got rid of NDSS featurization to remove compiler warning.
11/25/02   sk      Merged Force dormancy changes from MSM archive
11/22/02   sj      Removed sending CM_NDSS_ORIG_FAILURE rpt to CM.
11/21/02   lh      Added rejection of ECAM if AUX pilot is included.
11/20/02   jrp     Added featurization for REACH counters.
11/20/02   jrp     Added REACH counter support for CAI_EACH_BA_1_ID,
                   CAI_EACH_BA_2_ID, and CAI_EACH_BA_8_ID,
11/20/02   va      Included ds707_pkt_mgr.h under MM DATA feature
11/18/02   va      Moved mccsa_send_orig_fail_retry_delay to mccsup.c
11/12/02   bkm     Call mcc_ms_orig() when orig req received in SA state.
11/07/02   az      Mainlined FEATURE_IS95B
11/07/02   az      Removed the feature TMOBILE
11/06/02   fc      Changed pilot report processing to ignore old pilot
                   strength measurement reports.
10/25/02   yll     Don't set the layer 2 ack when receiving bcast message.
10/07/02   va      Make SO in origination to be 0 if we realize too late that
                   this is an invalid SO.
10/03/02   sb      Linted the code.
10/02/02   ph      Fixed featurization of CC.
10/02/02   sb      Set the layer to L3 after receiving an L2 ack.
10/02/02   az      Mainlined FEATURE_IS2000 and FEATURE_COMPLETE_SNM
09/30/02   sb      Mainlined FEATURE_SD20.
09/26/02   fc      Corrected the SD processing for GSRM.
09/18/02   az      Auth changes done for Chidu
09/04/02   AP      Fixed compile error for FEATURE_CDSMS.
08/30/02   ts      Added support for Run Time R-UIM Enable.
08/26/02   fc      Corrected the update of APHO substate.
08/19/02   yll     Fixed the compiling error.
08/13/02   HQ      Added FEATURE_CDSMS.
08/12/02   yll     Changes to include alt SOs in SO_BITMAP field in the
                   origination and page response message, even if max_alt_so
                   is equal to 0.
08/06/02   jrp     Changed MCCAP_REACH_RATE_SUPPORTED macro call to a
                   function call.
08/01/02   fc      Changes for mcc_parse_page function prototype.
08/01/02   jrp     Fixed non-release A compiler errors and warnings.
07/31/02   sb      Ensure (E)CAM is always discarded after call is released.
07/29/02   fc      Removed a redundant failed access registration.
07/08/02   az      Added FEATURE_DBM encapsulating Short Data Burst and
                   Position Determination
07/08/02   yll     Added support for MC Queue Watermark
07/01/02   jrp     Added support for REACH rlgain_common_pilot, ic_thresh,
                   and ic_max.
06/28/02   jrp     Fixed lint warnings.
06/28/02   lh      SVD code review comment changes.
06/27/02   fc      Changed UPM function names.
06/25/02   jrp     Added support for REACH APHO by populating the new overhead
                   parameters in TXC_REACH_PROBE_F during mccsa_apho_done().
06/25/02   az      Changed the name of the union for IMSI type specific
                   subfields from class to class_type.
06/20/02   fc      Corrected the processing of retry order. Made it applicable
                   to all data SOs.
06/19/02   lh      Fixed Rel0 compilation problems.
06/07/02   sj      Added CP support for multimode architecture.
05/22/02   fc      Changes to do reorder for T42M timeout (SD 2.0 only).
05/20/02   fc      Added support for Universal Page Message.
05/20/02   lh      Fixed uninitialized so_bitmap_ind for EOM.
05/07/02   fc      Updated comments.
05/06/02   yll     Changed the Broadcast SMS function names.
04/30/02   sb      Added layer to payload of mcc_send_orig_failure.
04/25/02   az      Corrected Fatal Error due to pref sys change as a prior
                   digital offline command was being ignored by CP.
04/23/02   sb      Added EVENT_ENCRYPTION_FAILURE
04/17/02   lh      Removed voc_acquire() from this file.
04/09/02   yll     Changed handling of MC_BCSMS_ENABLE_F to a function call.
04/03/02   va      Fixed ACCT issue wrt to wrong parenthesis  placement.
04/03/02   jrp     Calculates correct max message length for R-EACH access.
                   Added ABORT_NO_ACCEPTED_RATE_WORD to access_result_type.
                   Added TXC_REACH_PROBE_F supporting functions for probe commands
                   while in release A common channels.  Moved rate_word_satisfies_gain_limit
                   to rlgc.c.
03/28/02   sb      Added mcc_rand_parms_type.
03/26/02   fc      Corrected a condition check for sending release order.
03/21/02   fc      Corrected a problem of mobile stuck in system access
                   substate in the scenario of temporary fading of the new pilot
                   in the middle of access probe handoff process.
03/14/02   ck      Added setting of the SAVE REGS to 1 while generating the
                   AUTH SIGNATURE for registrations.
03/12/02   lh      d_sig_encrypt_req is now set based on our encryption capability.
                   Addendum 2 changes.
03/09/02   lh      Merged from common archive.
  02/20/02   az      Featurized 13K voice service option
02/22/02   fc      Added support for Release A common channels.
02/13/02   lh      Merged following from common archive.
 02/06/02   sh      Added hybird operation support
 01/30/02   az      Performing reorder when access failure occurs
                   due to fade timeout
 01/25/02   sj      Added support for Tiered Services - User zone feature.
 01/19/02   az      Added a parameter in a call to mcsys_process_pref_sys_change()
 01/12/02   az      Assigned favored_roam_ind variable from cmd queue to the exit
                   reason data structure and removed a check for SRDA_ENABLED
02/12/02   va      Added ACCT Blocked event reporting.
02/07/02   lh      Added missing call_ids and catch all for sr_id release.
01/25/02   jrp     Changed rel_code to rel_codes to comform to CM name change.
01/23/02   ph      Made cdma.sr_id valid for RELEASE A originations always.
01/23/02   va      Added error codes for ACCT back to CM.
01/17/02   va      Merged from 1.59 in MSM archive in sync with 2175.
 12/12/01  az      Added an additional check so that when there is a silent redial
                   command on the command queue, in access orig substate, you do not
                   check command queue in subsequent iterations.
 12/11/01  sj      Added support for Registration Accepted Order with ordq =
                   roam_indi. Will set the roaming indication on the phone
                   without triggering SD.
01/17/02   va      Added correct error code back to CM for ACCT block.
01/08/02   lh      Remove FEATURE_IS2000_REL_A_SVD from MC-CM interface.
                   Added code to prevent SR_ID leakage.
12/21/01   ph      Migrated mcc_l2_ack_timer to the callback timer mechanism.
12/20/01   va      Changed CM_INVALID_CALL_ID to CM_CALL_ID_UNASSIGNED
12/11/01   lh      Merged following from SVD branch.
                   11/30/01   va      Support for ACCT.
                   11/21/01   lh      Changes for CM/UI integration.
                   08/28/01   lh      Added support for Concurrent Service.
12/07/01   ph      Merged in from MSM_CP.05.00.108
                   11/30/01   hxw     Added FEATURE_GPSONE_NEW_L2_ACK to support delay L2 Ack
                                      mechanism.
                   11/16/01   fc      Fixed the problem of phone got stuck in system access state
                                      substate when MC_END_F is received in the middle of access
                                      probe handoff (PC loss) and later the access probe handoff
                                      got cancelled because valid message is received before T40M
                                      expired.
                                      Corrected the problem of not sending END ACK when MS back
                                      to Idle state from system access state.
                                      Corrected the initialization problem of mccsa_orig_ministate.
                   11/06/01   az      Added the sa_silent_redial_flag inside trimode feature definition
                   11/01/01   az      Added a flag so that when silent redial command is received
                                      in system access origination substate, the mobile does not
                                      immediately go to CDMA_EXIT but waits until the release order
                                      is received.
                   10/26/01   ht      Added code for BS calibration testing.
                   10/22/01   fc      Deleted the changes for system lost exit when (t40m-t72m)
                                      expires.
10/23/01   kk      Merged in from CP VU 99.
                   10/18/01   lcc     Corrected a problem in validation of ECAM which caused ECAM
                   to be rejected when default_config is 1 and forward/reverse
                   RCs are not 2 even when default_config field should be ignored.
                   10/11/01   lcc     Added feature to verify service option before originating
                   a DS SO.
10/03/01   kk      Added Qos Support.
10/02/01   ph      Merged in from CP VU 96
                   09/19/01   az      Copied the fwd_link_failure flag from the cmd queue to the
                                      exit reason data structure
                   08/29/01   sj      Moved the check for CAI_SO_VOICE_WILDCARD in cdma_sa()
                                      during originations to determine the preferred SO before
                                      doing voc_config(). Side Effect, this will fix the problem
                                      if the voc_config() is done for particular SO, however that
                                      SO is not used after snm_eval_so().
                                      Removed voc_config() in Access state, will be done when
                                      service negotiation is completed.
09/12/01   lh      Commented out RC checking in check_if_chan_assgn_is_okay().
09/05/01   fc      Changes in response to the function protype change
                   sd_ss_ind_cdma_opr_acc_term (SD2.0 only).
                   Changed function prototypes sa_process_page, sa_page_match
                   and auth_msg_proc.
                   Renamed is_in_access_ho_list to mccsa_is_in_access_ho_list.
08/21/01   fc      Added support for Base Station Reject Order, exit reasons
                   MCC_ENCRYPTION_FAILED and MCC_ACT_BLOCKED and A2 changes.
08/20/01   fc      For SMS/PD data burst, if the message is too long, report
                   to SD2.0 with the exit reason MCC_ACCESS_FAILED.
                   Added unsuccessful access registration for system lost exit.
08/22/01   sb      Changed pc_ord.sig_encrypt_mode to pc_ord.c_sig_encrypt_mode.
08/21/01   fc      Added support for Base Station Reject Order, exit reasons
                   MCC_ENCRYPTION_FAILED and MCC_ACT_BLOCKED and A2 changes.
08/20/01   fc      For SMS/PD data burst, if the message is too long, report
                   to SD2.0 with the exit reason MCC_ACCESS_FAILED.
                   Added unsuccessful access registration for system lost exit.
08/22/01   sb      Changed pc_ord.sig_encrypt_mode to pc_ord.c_sig_encrypt_mode.
08/16/01   kk      Merge from mainline MSM_CP.05.00.89
                   08/07/01   fc      Added JCDMA interface in SD2.0.
08/08/01   fc      Merged the following from common archive:
                   07/30/01   fc      Changed MC_MO_DBM_F failure reason to
                                      CM_DBM_MO_HOLD_ORIG (SD2.0 only).
                   07/16/01   fc      When (t40m - t72m) timer expires, declare
                                      loss of paging channel and back to system
                                      determination.
                   06/28/01   lcc     When initialize CCL, always initialize
                                      CHIND to contain rev. pilot.  For RC < 3,
                                      that bit is meaningless anyway.  This
                                      prevent CH_IND to contain invalid value of
                                      1.
06/26/01   ph      Merged in from MSM_CP.05.00.81
                   06/11/01   fc      Changes to support the split of fade report between idle
                                      and system access substates.
                   06/04/01   ht      Acquire vocoder only when it is a voice call.
                   05/29/01   fc      Changed mcc_inform_sd_redirection function prototype
                                     (SD20 only).
                   05/25/01   ts      Added changes to code for new UIM server.  Moved where BS
                                      challenge is sent to UIM server.
                   05/24/01   fc      Added the checking of whether current substate is
                                      CDMA_ORIG before reporting origination failure for analog
                                      handoff cases (SD20 only).
                   05/22/01   lcc     Added validation of CH_IND field in ECAM and some debug
                                      messages.
05/17/01   lh      Merged the following changes from the common archive
  05/02/01   lh      Fixed possible mis-alignment in process_feature_msg(),
                     process_sa_streq().
  04/18/01   fc      Featurized channel indicator change event report under
                     FEATURE_IS2000.
  04/12/01   fc      Added support of events report: Access attempt failure
                     (maximum number of access probes sent), Access attempt
                     failure (loss of paging channel), Channel indicator change,
                     SMS received, Registration performed, Lock, Unlock, NDSS off,
                     Release, Redirection, Registration rejected, Power down and
                     Call release request.
                     Renamed mcc_report_event_cp_state to
                     mclog_report_event_cp_state and mcc_report_event_timer to
                     mclog_report_event_timer.
  04/09/01   lcc     Added support for DCCH.
  03/28/01   fc      Added the report of GSRM to system determination 2.0.
  03/27/01   rc      Added a parameter to sd_ss_ind_user_ss_pref (SD 2.0 only).
03/27/01   kk      Merge from mainline
                   03/15/01   lh      Added clear cdma.call_orig before going back to idle, if
                   MS is in CDMA_ORIG.
                   03/06/01   fc      Added support of events report: Call processing state change,
                   and Timer expired.
                   03/05/01   yll     Added the sending Position Determination Messages on the
                   Access Channel.
03/08/01   ph      fixed bug in addressing variable.
03/07/01   ph      eliminated calculation of L2 length field length in processing
                   of ext. st. rsp msg to reflect new logic in compute_independent_msg_len().
03/07/01   ph      Changed usage of compute_independent_msg_len() and its
                   functionality to use parser to estimate the msg length
                   instead of doing it by hand for each msg.
03/01/01   kk      Merged from common archive to Release A.
02/26/01   ck      Fixed a bug where an END from UI was not handled correctly
                   when it was received while waiting for Auth signature.
02/16/01   fc      Added support for system determination 2.0. All changes
                   are featurized under FEATURE_SD20.
                   Cleanup debug messages.
02/16/01   sh      Added support for MC_END_F ack
02/16/01   va      Fixed a bug so that we do not do duplicate detection on
                   saved pending messages for which we already did the detection.
01/31/01   fc      Changed mccsa_send_orig_fail_retry_delay()  parameter
                   passing to include so_req and added type cast to retry_type
                   of CM command.
01/24/01   ych     Merged T-53/JCDMA features for SK.
01/16/01   jq      Made some naming changes to variables and functions. Added
                   FEAUTURE ifdefs to make it compilable when SCH is undefed
12/28/00   fc      Added support to abort origination (SO=33) in response to
                   retry order.
12/15/00   lh      Add support for MS originated Long Code Transistion Request
                   Order
12/01/00   cl      Added continuation symbol for feature defines that spanned
                   multiple lines.
11/28/00   ck      Changed the feature defines around the function
                   calculate_auth_data().
                   Added the issuing of a Base station Challenge command after
                   receving the result for an SSD update.
                   Added esn to the Update SSD command when FEATURE RUIM is
                   defined.
02/27/01   kk      modified Access Channel processing. All functions which format
                   outgoing messages are modified to include/set Release A
                   specific fields.
02/26/01   ph      added check for enh. encryption status from parser for Rel A.
02/26/01   ph      Code review changes, minor re-organization for REL A addi
                   Pilot Records.
01/29/01   ph      Bug fixes for REL A fcsch msgs - sec mode cmd msg,
                   ecam pilot rec checking, duplicate code, name change
                   of mccap_is_td_mode_supported()
01/10/01   va      Changed msg_okay macros into function calls.
01/10/01   ph      1. Added processing of Reg Acc Ord (with Roam Ind & Encryption)
                      to process_pc_order_msg()
                   2. Verification of Encryption, SR3, Addtl. Pilot Record fields of ECAM
                      in check_if_chan_assign_is_okay().
                   3. processing of Security Mode Command Msg to process_layer3().
11/02/00   lcc     Added support for FEATURE_COMPLETE_SNM.
11/01/00   lcc     Added proper support for setting DRS bit in ORM.
10/11/00   ts      Process the RUIM status when waiting for an auth signature.
                   The auth mode reference is passed to wait_for_auth_signature
10/11/00   pj      Changed sms_buf_ptr to dbm_buf_ptr for builds with AUTH defined.
10/02/00   yll     Added support for Position Determination DBM.
09/20/00   jq      Cleaned all lint errors.
09/12/00   cah     Removed the #ifdef feature checks for the srch_access_info
                   calls.
09/06/00   jq      Implemented FEATURE_FACTORY_TESTMODE based on FEATURE_PLT
09/01/00   ks      MS waits for MCC_SRCH_Q_SIG and MCC_TXC_PROBE_SIG in
                   wait_for_access_response() only if P_REV_IN_USE is >= 4.
08/18/00   fc      Added support for retry order.
08/17/00   ht      Add a condition to allow MS to send Release Order on
                   the Access Channel only when the P_REV is greater or
                   equal to 4.
08/11/00   va      Cleaned up lint errors.
07/28/00   va      Added reserved len  in calculation for max digits.
07/21/00   jq      Fixed errors about unexpected SRCh rpt
07/07/00   va      Corrected a bug in the compute_independent_msg_len()
                   function for origination and page resp
07/05/00   ht      Take a MCC_END_ACTIVATION exit when a Standard OTASP
                   origination failed due to Max Access Probes and the
                   srda_enabled is FALSE.
06/13/00   jq      Fixed some BAND_CLASS and P_REV checking errors for cdma2000
06/12/00   sh      added invalid msg length status checking for Status response
                   or Extended status response message.
06/05/00   fc      Fixed compiler warning in do_orign_auth() if FEATURE_AUTH
                   is defined but FEATURE_RUIM is not defined.
06/05/00   jq      Changed mcc_parse_page() parameter passing for BCSMS
06/02/00   lcc     For IS2000, moved update of ack_type in mcc_layer2 for response
                   to GPM to mcc_parse_page (in mccsup).  Also changes in ORM and
                   PRM to comply to published addendum.
05/30/00   jq      Cleaned one compile error with mcc_match_msg().
05/26/00   jq      Changed mcc_comp_addr parameter passing
05/26/00   cah     Fixed compile warning on digit_mode decl but not used.
05/26/00   ck      Added a call to auth_ruim_on() as soon as we enter SA state
                   to indicate to RUIM to power up.
                   Changed the interface for AUTH_SIGNATURE_F if FEATURE_RUIM
                   is defined.
                   Featurised calculate_auth_data under FEATURE_AUTH_DIGITS and
                   all the other places where auth data is being computed or
                   origination digits are being sent to RUIM.
05/24/00   cah     Fixed UASMS merge problem in send_sms_data_burst.
05/12/00   fc      Featurized call to srch_access_inform() also with
                   FEATURE_IS95B_SYS_RESELEC.
04/11/00   cah     Unrem'd var i which was undefined in function
                   check_if_chan_assgn_is_okay.
04/06/00   va      Merged with MSM3100_CP.03.00.17
           ram     To prevent SRCH from making other frequency neighbor
                   searches when we are in system access state, called
                   srch_access_inform() to inform SRCH whenever we enter
                   and exit system access state. This change is in cdma_sa().
           jq      Added broadcast SMS support
           fc      Fixed incorrect increment in count of request message
                   rather than count of response message in
                   send_order_response_msg.
           ry      Added OTAPA support
03/03/00   lh      Fixed no TMSI completion message in SA - CR12506.
03/30/00   ry      Removed unnecessary IS2000 featurization from
                   check_if_chan_assgn_is_okay()
03/28/00   ry      Fixed compiler errors introduced from the previous merge
03/28/00   ry      Added IS2000 featurization
           ry      Merged the following from PP (MSM3100_CP.03.00.10):
02/09/00   vn      Fixed another warning left before earlier.
02/03/00   vn      Fixed compiler warnings to compile it with feature -
                   MC_SILENT_REDIAL_AMPS - CR12834.
02/02/00   ht      Add check for NULL pointer on mc_auth_q.
01/19/99   ram     In the function process_message() while processing a
                   Global Service Redirection message, we were calling
                   cmd_done() twice for the same command from the rx queue.
                   This resulted in corruption of the queue and eventually
                   caused an 'undefined instruction exception' or a reset
                   of the phone. This only affected IS95B targets since
                   the GSRM was processed when FEATURE_IS95B_ACCESS_HANDOFF
                   or FEATURE_IS95B_ACCESS_PROBE_HANDOFF was defined.
01/19/99   ks      Setting SYS_PAR_MSG_SEQ & ACC_MSG_SEQ when going to Idle
                   state from either ORIG or PAGE_RES substates.
01/18/99   ks      Updated layer2_ack_processing to ignore validating Layer2
                   fields in case of CAM/ECAM during Orig or Page response.
01/04/00   fc      Added support for IS-733 service option number 17.
12/21/99   ram     Merged in the following change from the main trunk into the
                   trimode branch.
12/15/99   ks      Removed the feature ENHANCED_STANDBY_II.
12/10/99   vn      Removed reference to mcctcho_active_set (part of merge)
12/08/99   ck      Merged in the following changes.
                   ych     Added support for Trimode Silent Redial
           ram     Updated process_chan_assign_msg() to work for Trimode. If
                   we got a channel assignment to a different CDMA band,
                   we were ignoring the band change. But now we process the
                   inter-band channel assignment correctly.
01/27/00   va      Merged from GV40401
11/05/99   kk      Removed the feature FEATURE_MC_NEW_VOC_API, only one version
                   of voc_config() is used.
11/03/99   kmp     Changed around featurization of FEATURE_IS95B_ACCESS_PROBE_HO
                   in wait_for_access_response() when checking the for the
                   RX Fade report.
12/20/99   va      Support for new ORDQ
12/08/99   va      compute_independent_msg_length() function now takes into
                   account the new IS2000 fields also.
11/15/99   va      IS2000 Changes:
                   Pilot rpt fields are now part of internal mesg format. The mesg
                   is stored in the internal format for the next probe. The whole internal
                   format mesg along with the pilot rpt is converted into external
                   format before each probe.
                   Support for improved ECAM. New function mccsa_chan_assign_is_okay()
                   to verify the channel assignment messgae before processing it.
11/03/99   lcc     Merged from GD4021:
10/29/99   fc      Vocoder driver feature clean up. FEATURE_MC_NEW_VOC_API
                   replaced FEATURE_QDSP2.
10/26/99   fc      Fixed the case of MS sent wrong origination format for
                   P_REV 3 during NDSS.
10/22/99   abh     Changes for passing all the access parameters to TXC after APHO
10/21/99   kmp     Added the declaration of pointer cmd_ptr in cdma_sa() to
                   fix a compiler serious error. This is only for
                   FEATURE_IS95B_NDSS.
10/20/99   kk      If NDSS is disabled in the new system, MC will let CM know
                   to prevent the silent redial mechanism.
10/11/99   ry      Removed all references to mcctcho_active_set from
                   process_chan_assign_msg()
09/30/99   ks      removed unsued variable "auth_len" and added cast for
                   calculate_auth_data() function call to remove warnings.
09/22/99   lh      Merged following from MSM3100 archive.
           lh      Fixed the failure of EVRC origination.
           nmn     implemented IS-96A vocoder capability checking
           lh      Merged following changes for MSM3100.
           mk      Using FEATURE_QDSP2 for voc_config().
09/21/99   abh     Fixed the bug because of which more APHOs than the Max APHOs
                   allowed(by BS) were performed.
09/15/99   lh      Removed the duplicated logic in send_origination_msg() by
                   calling srv_eval_so().
10/28/99   lcc     Moved PLT related code to plt_mc.c.
09/16/99   lcc     Merged in PLT support from PLT archive.
09/14/99   abh     Fixed function mccsa_access_attempt_in_progress() so that
                   it returns TRUE only in System Access State.
09/14/99   abh     Fixed the case when the Ovhd Msg Processing caused MS to
                   exit to system determination(in case of AHO & APHO)
09/10/99   fc      Fixed send_order_response_msg() so that only P_REV_IN_USE
                   >= 4 will send Extended Status Response Msg.
09/08/99   kk      In case of an NDSS redirection, MC will let CM know
                   by sending an ORIG_FAILURE notification, hence there
                   is no need to send a reorder.
08/26/99   lh      Added release reason to report to CM. Fix for CR6158.
08/24/99   nmn     Fixed bugs related to determining Alt SO list for SO #1
08/24/99   fc      Fixed the bogus pilot reporting, by requesting on SRCH to
                   provide the count of pilots in Pilot Strength Measurement
                   report.
08/23/99   fc      Modified mccsa_update_srch_neighs() so that the count of
                   pilots will be incremented only if access attempted pilots
                   are appended to the neigbor set passed to SRCH.
08/21/99   ks      Added code review changes for Access Probe Handoff. Also
                   added Pilot reporting support for previously attempted
                   pilots during APHO.
08/19/99   lh      Changes at exit point to handle max access probe case.
08/18/99   kk      Fixed typo in CM notification.
08/18/99   jq      Added msg check for ECAM/CAM to reject ASSIGN_MODE=1,
                   RESPOND=0 in CDMA_ORIG substate. process_layer3(). Also
                   took out comments for by pass alert support, since this
                   feature is now supported
08/18/99   lh      When enter with MCC_ORIGINATION (SMS), after transmission
                   of SMS, exit to system determination with RELEASE reason.
08/17/99   ks      Added Layer2 Access in progress condition checking in
                   wait_for_access_respnonse() while Pilot Report processing.
08/15/99   kmp     Removed FEATURE_UASMS around digit_array.
08/13/99   lh      Taking out CM_CALL_ORIG_ERR_ABORT error code.
08/13/99   lh      CM and SMS phase 2 implementations.
08/13/99   jq      Added casting to q_get() return value in
                   build_ac_tmsi_assign_cmp() to clear compiler warning
08/06/99   nmn     Fixed sizing CR with compute_max_orig_digits()
08/06/99   nmn     Removed IS-96
08/04/99   fc      Fixed a merge typo.
08/03/99   fc      Renamed mccsa_mdr_in_progress() to mccsa_is_special_mdr().
08/03/99   fc      Added mccsa_mdr_in_progress() to standardize the conditions
                   checking for special MDR.
                   For special MDR, except for data calls with MDR options,
                   the signal MCC_TXC_PROBE_SIG will be ignored.
07/28/99   lh      CM and two-way SMS support added.
07/27/99   fc      Initialize max_message_body to 0 in mccsa_apho_done() to
                   eliminate errors with ARM 2.50 compiler.
07/24/99   abh     Changes for APHO because of Paging Channel Loss.
07/22/99   fc      Changes in Pilot Reporting to support Access Probe Handoff.
07/21/99   fc      Corrected the merge error in compute_max_orig_digits().
07/19/99   nmn     Implemented Alt SO in the send_page_response_msg and
                   send_origination_msg functions (surrounded by #ifdef of
                   FEATURE_IS95B or FEATURE_SPECIAL_MDR).  Also cleaned up
                   some origination code.
07/19/99   abh     Changes for Access Probe Handoff
07/09/99   ry      Updated process_chan_assign_msg() to copy pilot PNs into
                   mcctcho_active_set instead of cdma_active_set
06/24/99   ks      Setting handoff state of the Mobile After AHO and after
                   processing OVHD Msgs, based on the current ESPM.
06/21/99   kmp     In compute_max_orig_digits(), updated the the calculation
                   to include pilot reporting and alternative service options
                   when required. Corrected a spelling error in
                   send_origination_msg() for FEATURE_IS95B_NDSS. Removed
                   extern prototype for srv_eval_so() and added an #include for
                   srv.h. Merged in the following IS-95B changes.
06/21/99   jq      Redesign the Release sequence for END_F followed by
                   POWERDOWN_F so that Release ORD is always sent.
06/13/99   fc       Modified compute_ac_msg_num_plt_rpt() to avoid bogus
                   pilots being reported.
                   Modified compute_max_orig_digits() to include IS-95-B
                   standard requirement - assumption of N13m-1 number of
                   additional reported pilots when calculating the number of
                   dialed digits to be included in the origination message.
                   Modified update_aho_active_pilot(), setup_pilot_rpt() and
                   wait_for_srch_pilots_meas_rpt() to include data type cast.
06/09/99   jq      Modifications due to codereview
06/07/99   ks      Added processing for Registration Accept order with Roaming
                   Indication included in process_pc_order_msg(). The T72M timer
                   implementation for AHO and APHO is also supported in
                   wait_for_access_response().
06/04/99   jq      Added Release on Access Channel feature implementation
                   so a release order msg is sent when END/POWER key is
                   pressed in Origination Substate.
05/13/99   kmp     Replaced T_ACP, T_AUTH, T_SMS with FEATURE_ACP, FEATURE_AUTH,
                   FEATURE_SMS. Changed FEATURE_JSTD008 TMSI instances to
                   FEATURE_TMSI. Merged in the following IS-95B changes.
           ks      Overhead messages are processed in System Access substate
                   after an Access HO. Handling of CAM/ECAM and other Layer3
                   messages during AHO is also added.
           kk      Service Redirection with NDSS processing added.
           ks      Added a new function is_in_access_ho_list, which verifies
                   if a pilot is in ACCESS_HO_LIST.
           kmp     Fixed PACA msg processing valid states in process_layer3().
           ks      Added a function is_access_ho_allowed for T72M implementation.
           fc      Put in changes based on input from code review.
           fc      Put in changes based on the input from code review.
                   Added the clearance of access_attempted flag for the
                   active pilot and pilots in the neighbor list upon
                   received BS ACK.
           jq      Added comments and small changes due to code review
           fc      Removed MATCHED_NEIGHBOR_LIST() and modified
                   BUILD_ACCESS_HO_LIST(), UPDATE_ACCESS_HO_LIST(),
                   BUILD_OTHER_REPORTED_LIST() and
                   WAIT_FOR_SRCH_PILOTS_MEAS_RPT().
           ks      Resetting the Handoff State to CDMA_IDLE_HO before exiting.
           kmp     Made functions send_orignation_msg(), send_registration_msg(),
                   and send_page_response_msg() more readable. Added new parameter
                   for IS-95B to the compute_max_orig_digits() function for IS-95B.
           fc      Modified the pilot reporting implementation. Renamed
                   plt_rpt_bookeep to pilot_rpt_ovhd_rec. Sequenced the
                   execution of IS-95-B Default Value Setting and IS-95-B
                   Pilot Reporting Bookeeping to enhance code readabilty.
                   Removed the typo in send_page_response_msg().
           jq      Added support for the unified CAM/ECAM implementation
                   process_message(), process_layer3(),
                   process_chan_assignment_msg()
           lh      Authentication using IMSI_M.
           fc      Moved the definition of ACCESS_HO_LIST,
                   OTHER_REPORTED_LIST and PLT_RPT_BOOKEEP to MCCDMA.
                   Fixed the bug in pilot strength comparsion logic of
                   BUILD_ACCESS_HO_LIST() and BUILD_OTHER_REPORTED_LIST().
           fc      Removed excessive pilot reporting msgs for DM.
           fc      Added support for pilot reporting. Renamed
                   MCC_ACC_PROBE_SIG to MCC_TXC_PROBE_SIG. Deleted
                   tx_acc_msg, but instead added plt_rpt_bookeep for keeping
                   pilot reporting overhead info. Renamed
                   SETUP_ACC_MSG_PILOT_RPT to SETUP_PILOT_RPT.
           ks      Modified FEATURE_IS95B_AHO to FEATURE_IS95B_ACCESS_HO.
           ks      Added support for IS95B Access Handoff feature.
           fc      Fix the problem with Origination Continuation.
           lh      FEATURE_JSTD008 replaced with run-time P_REV and Band
                   Class checking.
           kmp     Added Phased Deployment support. Modified process_layer3(),
                   process_message(), send_origination_msg(), and
                   send_order_response_msg()
           fc      Change last_acc_msg_type and last_acc_msg_len to
                   tx_acc_msg.last_msg_type and tx_acc_msg.last_msg_len.
                   For Page Response, Origination and Registration Msg, set
                   msg internal format of p_rev_in_use whenever FEATURE_IS95B
                   is defined irrespective of P_REV_IN_USE value. Previously,
                   it is set if and only if FEATURE_IS95B is defined and
                   P_REV_IS_4_OR_GREATER.
           fc      Remove excessive line of code for updating the length of
                   Access Channel Messages.
           fc      Access channel messages default values.
           lh      Fix scm for backward compatibility.
           lh      IS-95B True IMSI implementation added.
04/16/99   ck      Function calculate_auth_data now takes three parameters and
                   hence updated a call to this function which passed only 2
                   parameters.
03/18/99   ck      Merged in the diffs between SPSW rev 1.156 and 1.158
03/18/99   ck      Updated auth_data for OTASP originations for use in
                   Re-Authentication.
02/12/99   lcc     Added initialization of CCL and pilot strength on OM and PRM
                   for special MDR release.
01/27/99   ck      Support for EVRC Service negotiation is featurised under
                   FEATURE_EVRC_SO_MGMT
01/12/99   lcc     Merged from MDR branch:
                   - Added verifying of SNM managed SOs in send_origination_msg.
                   - Changed usage of station class mark consistent between IS95A
                     and JSTD 008 (i.e. like in IS95B, no more usage of fields
                     ext_scm and slotted_mode).
11/24/98   br      Added changes for storing new NAM.
10/21/98   pms     Removed some suspicious _near variables.
10/16/98   ck      Added support for EVRC service negotiation.
10/02/98   pms     Neared more variables to save ROM space.
10/02/98   pms     Prevented data calls from retrying on AMPS when an
                   origination fails due to MAX_ACCESS_PROBES, etc. See CR6421.
09/15/98   pms     Removed conditional code so that process_reoder() is always
                   called when process_ds_call_failed() is called.  See CRs
                   6009,6152, 6048 and 6238.
08/25/98   ck      Added support for Rate Set 2 SMS(CAI_SO_RS2_SMS)
08/24/98   dna     Retain message encryption on CDMA->AMPS handoff by
                   separating auth_reset from auth_resync.
08/20/98   ck      Using VOC_CAP_IS733 instead of VOC_CAP_ISXXXX.
                   Using VOC_CAP_IS127 instead of VOC_CAP_EVRC.
08/11/98   ck      Added support for EVRC Service Option.
08/06/98   ych     Fixed the Lint warnings
08/04/98   ych     Merged SSPR, E911 and Acquisition improvements.
07/08/98   kis     The mobile now performs Power down process when attempting
                   a powerdown registration with PSIST is 11111. see CR 3913.
                   Also corrected come compilation errors when FEATURE_DS is not
                   enabled.
06/26/98   pms     Now calls process_ds_call_failed() when either UI or DS
                   end a call.  See CR5553.
06/26/98   pms     Fixed race condition associated with CR4502.
06/25/98   kis     Added code to support the ASCII digit mode call origination.
06/17/98   wjn     Added Feature E911 Support.
05/11/98   na      Call Processing now processes MC_DATA_END_F in System
                   Access state. Fixes CR 4337.
04/03/98   dna     Fixed SMS valid_ack to be set properly
03/20/98   abh     Review comments for PRL Force Mode
03/12/98   aks     Changed target-specific section into
                   FEATURE_VOICE_PRIVACY_SWITCH.
02/02/98   abh     Changes for PRL Forced Mode.
01/26/98   fkm     FEATURE_xxx Updates (via Automatic Scripts)
11/03/97   ldg     Replaced references to specific data service options with
                   a macro which represents the same information.
10/10/97   abh     Code for GSR changes, so the after a global redirection
                   to analog phone doesn't remain stuck in analog.
10/03/97   dna     Assume voice privacy is on if NV Item is NOT_ACTIVE.
08/05/97   lh      Added code for buffer leak prevention.
07/30/97   lh      Let system determination decide if a redirection is valid.
07/29/97   jrs     When exiting with a Max Access Probes or Access Denied,
                   send a reorder for dual-mode targets, and don't send a
                   reorder for other targets.
07/16/97   lh      Added logging out-of-buffer errors.
06/17/97   dhh     Configured for Q (T_Q).
06/16/97   dna     Set hold_orig when returning to idle
06/02/97   day     Declare variables as static to be compliant with ANSI C standard.
05/20/97   ck      Added support for Low Power Mode
04/18/97   dna     Changed some #ifdefs to make class 1 IMSI its own feature
04/09/97   lh      Use ifndef T_ACP for dual band phone.
02/07/97   jca     No longer sending a Reorder command to UI on failed
                   origination access due to ACCESS_DENIED.  This allows
                   automatic retry in AMPS mode for dual-mode phones.
01/25/97   dna     More flexible reject of status request.
01/18/97   jjw     Completed integration of AMPS data call capability
12/13/96   jca     Modified processing for MC_ORIGINATION_F command.
11/07/96   dna     Set MSID field appropriately for OTASP calls
09/26/96   jca     No longer sending a Reorder command to UI on failed
                   origination access due to MAX_ACCESS_PROBES.  This allows
                   automatic retry in AMPS mode for dual-mode phones.
09/13/96   dna     Added processing for OTASP activation origination, etc.
09/11/96   jca     Fixed DTMF decoding bug.
08/28/96   dna     Change entry from MCC_ORIGINATION if returning to idle
08/28/96   jpf     Set exit block's S.O.=0 if chan assign mode is ACQ_ANALOG.
07/30/96   jgr     Added new sms buffers system, changes are ifdef NEW_SMS.
07/14/96   day     Fix error fatal by setting the correct response message
                   type (cdma.sa_resp_msg) before sending status response
                   in process_sa_streq.
07/10/96   day     Add input parameter to identify page/non-page messages
                   when calling mcc_detect_duplicate.
06/25/96   dna     #defined IMSI_CLASS_0_ADDR_NUM
06/17/96   dna     Handle special cases for status request/response.
06/10/96   jpf     Use cur_bs_ptr to find BS tmsi_zone in set_address().
05/28/96   dna     Class 1 IMSI support.
05/15/96   dna     Changed auth_on to auth_mode and encryption_enabled to
                   encrypt_mode.  Fixed bug with orig continuation auth,
                   start key calc sooner, reject chan assign before ack.
04/15/96   jca     Modified code to use regionally declared NV command
                   buffers instead of stack variables to avoid potential
                   memory overwrite problems.
04/09/96   day     Added TMSI addressing modifications.  Using T_TMSI ifdef.
04/09/96   day     Changed msid type default to esn for J-STD-OO8 when setting
                   address.  Using FEATURE_JSTD008 ifdef.
03/21/96   dna     Now send ack_type 2 for response to General Page.
                   Changed default sloted_mode to 1 (slotted).
03/14/96   dna     Added support for PCS nom_pwr_ext access param. field.
03/13/96   day     Check assign mode validity.  Using FEATURE_JSTD008 ifdef.
03/13/96   day     Added service negotiation check.  Using FEATURE_JSTD008 ifdef.
03/13/96   day     Added PCS band class check.  Using FEATURE_JSTD008 ifdef.
03/13/96   day     Added J-STD-008 PCS message modifications.  Using FEATURE_JSTD008
                   ifdef.
02/09/96   jca     Added MCC_MAX_ACCESS_PROBES exit code.
01/02/96   dna     We now accept a duplicate with the right ack_seq as an ack.
12/12/95   jca     Modified code to send Page Response msg with "off hook"
                   service option if page rxed while WLL phone is off hook.
11/30/95   jca     Removed mccreg_get_mob_term check on received pages.
11/17/95   dna     Don't want Voice Privacy if key generation fails.
11/14/95   jca     Removed support for configurable service option 1 def.
10/09/95   day     Delete authentication data assignment to MIN for data
                   burst.
10/05/95   dna     Account for zero being a valid value for RAND.
10/04/95   day     Add SMS address format conversion for authentication data
                   and call calculate_auth_data for data burst
                   authentication.
10/03/95   dna     Made function calculate_auth_data for data burst auth
09/11/95   dna     Authenticate Data Bursts, UI notified for Orig Timeout
08/22/95   dna     Bug fixes for IMSI addressing
08/18/95   dna     Added support for origination continuation
08/17/95   jca     Added code to process info records in a Feature Notif Msg.
08/14/95   gb      Added new Loopback_13K service option support.
08/10/95   rdb     Added support for Status Request and Status Response
                   Messages.
08/10/95   gb      Added New Markov Service Options.
08/10/95   dna     Read NV item for voice privacy and write regional
                   variables for auth status message
08/04/95   rah     Removed vestigial marker comments.
08/03/95   day     Move data burst validation from caix.c to layer 3 level
                   so layer 2 ack will always be generated for this message.
08/03/95   dna     Added support for message encryption
08/02/95   day     Change sms_process_mt_msg parameter to pass by address
                   instead by value.  Code optimization eliminated the
                   code which updated this value.
07/28/95   day     Move local declaration to regional - prevent task stack
                   overflow when processing SMS mobile-originated message.
07/26/95   day     Move local declarations to regional - prevent task stack
                   overflow.
07/20/95   jca     Code updates to support subscriber access control.
07/13/95   gb      Added Service Negotiation support.
07/13/95   rah     Changed vocoder 8/13kbps configuration.
07/11/95   jah     Removed hard define of AUDIO_CONTROL_PATCH
07/07/95   dna     Voice Privacy, authentication code review changes and
                   bug fixes.
07/06/95   rah     Added vocoder 8/13kbps configuration.
07/06/95   jca     Now dealing with a single origination command.
06/30/95   jca     Now calling function mcsys_get_request_mode to get
                   REQUEST_MODE code for originations and page responses.
06/25/95   dy      Added support for mobile-originated and mobile-terminated
                   SMS messages.
06/20/95   jca     Added support for mobile-originated SO2 Loopback calls.
06/19/95   rdh     Temporary mods for audio path muting during origination.
06/09/95   jca     Mods to support new queue sigs interface to mcc_wait().
06/07/95   dna     Added Authentication.
05/31/95   gb      Added support for 13k service options.
05/25/95   jca     Updates for IS-95A registration procedures.
04/13/95   dna     Layer 2 processing changes and many others.
03/24/95   jca     Added code to handle new data service option defs.
01/10/95   dna     Ignore pages while ISS1 off hook.
12/13/94   dna     Removed multiple REORDER commands to UI.
                   Lock and maintenance required orders handled.
11/14/94   dna     Work for Channel Assignment Message.
11/11/94   jca     Deleted transmit clocks_on processing.
11/09/94   jah     Changes to use voc_acquire() and voc_release().
11/01/94   gb      Sector Lock test functionality added.
10/13/94   jca     Add new function calc_psist() and other misc bug fixes per
                   Dodge Johnson's email.
06/09/94   jca     Fixed TX clock state bug.
04/25/94   gb      Parameterised all IS95 timeouts in cai_tmo.
04/20/94   jca     Mods for IS-95 Phase 2 upgrade.
12/22/93   jca     Mods for IS-95 Phase 1 upgrade.
10/06/92   jai     Added access probe logging support
09/21/92   jai     Added user_end flag to prevent sending reorder when user
                   presses END to stop access after Base Station Ack is
                   received.
07/24/92   jai     Changed calling parameters to mcc_nv_cmd.
06/20/92   jai     Modified module for DMSS
01/08/91   jai     Created module.

===========================================================================*/

/*===========================================================================

                     INCLUDE FILES FOR MODULE

===========================================================================*/

#include "memory.h"
#include "comdef.h"
#include "qw.h"
#include "rex.h"
#include "queue.h"
#include "task.h"
#include "cai.h"
#include "caii.h"
#include "ran.h"
#include "err.h"
#include "caix.h"
#include "rxcmc.h"
#include "txcmc.h"
#include "mccdma.h"
#include "mc.h"
#include "mci.h"
#include "mccrxtx.h"
#include "parm.h"
#include "msg.h"
#include "cmd.h"
#include "voc.h"
#include "db.h"
#include "mccreg.h"
#include "mccrx.h"
#include "mcctcho.h"
#include "sd.h"
#include "target.h"
#include "customer.h"
#include "snm.h"
#include "mccccl.h"

#ifdef FEATURE_CDSMS
#include "wmsmsg.h"
#endif /* FEATURE_CDSMS */

#ifdef FEATURE_AUTH
#include "auth.h"
#endif /* FEATURE_AUTH */
#ifdef FEATURE_UIM_RUIM
#include "uim.h"
#endif /* FEATURE_UIM */
#ifdef FEATURE_DS
#include "ds707_extif.h"
#endif /* FEATURE_DS */
#include "tmsi.h"
#include "bit.h"
#include "mccsrch.h"
#include "srv.h"
#include "mccap.h"
#include "mccsch.h"
#include "ts.h"
#if (defined FEATURE_DBM && defined FEATURE_GPSONE && !defined FEATURE_CGPS)
#error code not present
#endif /* FEATURE_DBM && ! FEATURE_CGPS */

#include "mccsyobj.h"

#ifdef FEATURE_IS2000_REL_A
#ifdef FEATURE_IS2000_REL_A_SVD
#include "mccsrid.h"
#endif /* FEATURE_IS2000_REL_A_SVD */
#endif /* FEATURE_IS2000_REL_A */
#ifdef FEATURE_PASSPORT_MODE
#include "cm.h"
#endif
#include "event.h"
#include "mclog.h"
#include "mccbcsms.h"

#include "mmoc.h"

#ifdef FEATURE_UMTS_1X_HANDOVER_1XMSM
#error code not present
#endif /* FEATURE_UMTS_1X_HANDOVER_1XMSM */

#if defined(FEATURE_IS2000_REL_C_FCS) || defined(FEATURE_IS2000_REL_D) 
#include "mccsyncid.h"
#endif /* FEATURE_IS2000_REL_C_FCS || FEATURE_IS2000_REL_D */

#if defined(FEATURE_IS2000_REL_C_DV) || defined(FEATURE_IS2000_DV_SIG_TEST)
#error code not present
#endif

#if defined(FEATURE_IS2000_REL_C_AKA) || defined(FEATURE_IS2000_REL_A_AES)
#error code not present
#endif /* FEATURE_IS2000_REL_C_AKA */
       
#ifdef FEATURE_IS2000_REL_D
#error code not present
#endif /* FEATURE_IS2000_REL_D */

#ifdef FEATURE_OTASP
#include "otaspi.h"
#endif /* FEATURE_OTASP */
  
/*lint -e818 */
/*===========================================================================

                DEFINITIONS AND DECLARATIONS FOR MODULE

This section contains definitions for constants, macros, types, variables
and other items needed by this module.

===========================================================================*/

static struct
{
  boolean access_in_progress;
    /* Indicates if an access attempt is in progress */
  boolean pending_msg_flag;
    /* Indicates if there is a message pending */
  byte last_msg_type;
    /* Holds type of last message transmitted */
  byte last_ord_type;
    /* Holds the ord type of the last order transmitted */
  byte last_reg_msg_seq;
    /* Message sequence number of last registration or implicit
       registration sent */
  mccdma_matched_msg_type pending_msg;
    /* Holds pending message, if there is one */
  boolean dup_detect_done;
    /* Indicates if duplicate detection has already been performed
       on this message */
} mccsa_layer2;

/* Indicates which layer the origination failed in so we can inform CM */
static cm_call_orig_fail_layer_e_type mccsa_orig_fail_layer;

static boolean mccsa_layer3_msg_pending;
  /* Indicates if a message is pending after a Mobile has successfully
   * finished Access Handoff or Access Probe Handoff and while processing OVHD
   *  messages */

static boolean mccsa_is_aho_allowed(void);

word mccsa_send_order_response_msg
(
  word curr_state,
    /* Current substate */
  boolean          request     
    /* Flag for request or response */
);

word mccsa_send_data_burst
(
  word curr_state
    /* Current state being processed */
);

#ifdef FEATURE_SO_8026
#error code not present
#endif /* FEATURE_SO_8026 */

static caii_plt_rpt_type *plt_rpt_ptr;
  /* Pointer to pilot report fields embedded in the above internal struct */

static mccdma_matched_msg_type mccsa_matched_msg;
  /* Structure to hold message we are to process */

#ifdef FEATURE_IS2000_REL_A_CC
// Maximum message length for R-EACH using the selected rate_word.  This
// is used during a single access attempt.  Future access attempts may
// assign a different value to this max reflecting the maximum message for
// that access attempt.  Access messages may change during an attempt according
// to pilot information.  However, the message lengths may not exceed this value.
static word reach_max_message_length;
#endif /* FEATURE_IS2000_REL_A_CC */

/* Variables controlling the Access Probe Handoff Behaviour */
static int mccsa_num_apho_done = 0; /* Number of APHOs performed in this access attempt */

/* The following enumerated variable keeps control of the timing of the Access Probe */
/* Handoff, if an APHO is in progress.                                               */
static enum 
{
  APHO_NOT_IN_PROGRESS,
  WAIT_FOR_TXC_OK,          /* Rcvd Srch Report, waiting for TA timer to expire */
  APHO_IN_PROGRESS,         /* PN changed. Waiting for Ovhd Msgs to be downloaded */
  PC_LOSS_WAIT_FOR_SRCH,    /* T72m timer expired */
  PC_LOSS_APHO_IN_PROGRESS  /* T72m timer expired(on the new Pilot) while in APHO */
} mccsa_apho_substate;

/* Variable mccsa_init_aho is used to control race scenarios between APHO & L2 ack   */
/* If the APHO is terminated because of a Layer 2 ACK and a Handoff is to be         */
/* performed before proceeding further. This variable is set in following conditions */
/*    Case 1: When an access attempt is stopped because a Layer2 Ack is rcvd and MS  */
/*            is enabled to perform AHO for this access attempt. Or                  */
/*    Case 2: When an access attempt is stopped because L2 Ack is rcvd and MS needs  */
/*            to send back an Ack Response for this L2 Ack on access channel.        */
/*    And, Search has already reported a stronger pilot.                             */
/* After processing an OTA message, if mccsa_init_aho is TRUE, a handoff is performed*/
/* immediately.                                                                      */

static boolean mccsa_init_aho;

boolean mccsa_apho_in_progress(void);

static boolean mccsa_is_apho_allowed(void);

static boolean mccsa_is_apho_okay
(
  word pilot,
  word new_freq,
  byte new_band
);

static word mccsa_apho_done(word state);

#ifdef FEATURE_AUTH
word mccsa_wait_for_gen_auth_signature
(
  dword *auth,
    /* Where to place computed auth signature value */
#ifdef FEATURE_UIM_RUIM
  byte *auth_mode,
    /* Points to auth mode field in access message */
#endif /* FEATURE_UIM_RUIM */
  word curr_state
    /* current state being processed */
);

/* Flag to indicate if we are waiting for an AUTH signature */
static boolean mccsa_gen_auth_signature_in_progress;
#endif /* FEATURE_AUTH */

static access_result_type mccsa_send_access_cmd
(
  caii_tx_msg_type *tx_msg_ptr,  /* xExternal format message pointer */
  caii_plt_rpt_type *pilot_rpt_ptr,  /* External format message pointer */
  boolean          request,     /* Flag for request or response */
  word             msg_len,     /* Length of message */
  word             curr_state   /* Current substate */
);

#ifdef FEATURE_IS2000_REL_A
static boolean mccsa_is_call_blocked_by_acct
(
  word so,  /* SO chosen for the outgoing call */
  boolean is_emergency_call /* Indicates if this is an emergency call */
);

boolean mccsa_is_dbm_blocked_by_acct
(
  byte burst_type /* Burst Type of the outgoing DBM */
);
#endif

static word mccsa_compute_independent_msg_len( caii_tx_msg_type *tx_int_msg_ptr, caii_plt_rpt_type *plt_rpt_ptr);

#ifdef FEATURE_IS2000_REL_A_CC
access_result_type mccsa_select_mode_rate
(
  int size,                        /* Size of message to be transmitted */
  byte each_ba_rates_supported,    /* Supported rate words for the Basic Access mode on the EACH */
  byte rccch_rates_supported,      /* supported rate words on the RCCCH */
  byte num_mode_selection_entries, /* Numbers of entires of the Mode Selection table */
  caii_access_mode_rec_type *mode_selection, /* mode selection table */
  int16 rlgain_common_pilot,       /* Gain adjustment of EACH or RCCCH relative to reverse pilot */
  int *mode,                       /* Mode selected  */
  txc_each_rate_type *rate_word    /* Rate index selected */
);
#endif /* FEATURE_IS2000_REL_A_CC */

/* equals CAI_REL_NORMAL if END caused the release order,
** CAI_REL_PWDN, if POWER key press caused the release order
*/
static byte mccsa_release_cause;

/* the release process started if set CDMA_ORIG_RELS */
/* used to prevent repeated processing in case of multiple END being pressed
** and to carry the release indication while mobile is in the process of
** updating overhead information during Access Probe Handoff or Access Handoff.
** This variable should be initialized to CDMA_ORIG_NORM, when entering into
** System Access State
*/
static cdma_sa_orig_ministate_type mccsa_orig_ministate;

static plt_rpt_ovhd_rec_type  mccsa_pilot_rpt_ovhd_rec;
  /* Pilot reporting overhead info */
static aho_list_type mccsa_prev_access_ho_list;
  /* Previous Access handoff list */
static aho_list_type mccsa_access_ho_list;
  /* Current access handoff list */
static othrpt_list_type mccsa_other_reported_list;
  /* Current Other reported list */

#ifdef FEATURE_PASSPORT_MODE
static boolean mccsa_persistence_orig_ng;
  /* origination fail by persistence test */

static boolean mccsa_acct_all_mobiles_blocked_for_voice = FALSE;
/* If true, it means that all mobiles are blocked from making
   voice calls. */
#endif /* FEATURE_PASSPORT_MODE */

static boolean mccsa_access_attempt_is_request;

/* System access L3 counter type */
typedef enum
{
  SA_REQUEST,
  SA_RESPONSE,
  SA_FAIL
} mccsa_l3_counter_type; 

/*===========================================================================

FUNCTION MCCSA_POPULATE_ACH_ACCESS_CMD

DESCRIPTION
  This function populates the parameters in the txc_acc_cmd_type structure
  in preparation for a TXC_ACCESS_F or TXC_ACC_PROBE_F.

DEPENDENCIES
  Requires non-NULL ap_ptr, pointing to valid access parameters message.

RETURN VALUE
  The return value of mccsa_calc_psist() if the access message to be sent is a 
  response. Otherwise, NOT_ABORT.

SIDE EFFECTS
  This function modifies the global mcc_tx_buf union.
===========================================================================*/

access_result_type mccsa_populate_ach_access_cmd
(
  caii_accparm_type * ap_ptr,
  word curr_state
)
{
  mcc_tx_buf.acc.pam_sz =  ap_ptr->pam_sz;
  mcc_tx_buf.acc.max_cap_sz = ap_ptr->max_cap_sz;
  mcc_tx_buf.acc.bkoff = ap_ptr->bkoff;
  mcc_tx_buf.acc.probe_bkoff = ap_ptr->probe_bkoff;
  mcc_tx_buf.acc.acc_tmo = ap_ptr->acc_tmo;
  mcc_tx_buf.acc.init_pwr = ap_ptr->init_pwr;
  mcc_tx_buf.acc.pwr_step = ap_ptr->pwr_step;
  mcc_tx_buf.acc.num_step = ap_ptr->num_step;

  mcc_tx_buf.acc.nom_pwr = (ap_ptr->nom_pwr & 0x08)? (byte)(ap_ptr->nom_pwr | 0xF0) :
                                                     (ap_ptr->nom_pwr & 0x0F);
  if (P_REV_IS_1 || P_REV_IS_4_OR_GREATER)
  {
    if (ap_ptr->nom_pwr_ext == 1)
    {
      /* ----------------------------------------------------------------
      ** (nom_pwr_ext == 1) means nom_pwr is in the range (-24dB to -9dB)
      ** instead of (-8dB to 7dB), so we subtract 16dB from the nom_pwr
      ** adjustment to put it in the right range.
      ** ---------------------------------------------------------------- */
      mcc_tx_buf.acc.nom_pwr -= 16;
    }
  }
  mcc_tx_buf.acc.req = mccsa_access_attempt_is_request;
  mcc_tx_buf.acc.seq_num = (mcc_tx_buf.acc.req)?ap_ptr->max_req_seq:ap_ptr->max_rsp_seq;

  mcc_tx_buf.acc.rn = hash( cdma.esn, (word) (1 << ap_ptr->probe_pn_ran),
                           (word) (14 * (cdma.esn & 0xFFFL)) );
  mcc_tx_buf.acc.acc_chan = ap_ptr->acc_chan;   /* Number of access channels */
  mcc_tx_buf.acc.use_chan = FALSE;   /* Use specific channel */


  mcc_tx_buf.acc.pagech = cdma.pagech;                 /* Current Paging Channel */
  mcc_tx_buf.acc.base_id= cur_bs_ptr->csp.sp.base_id;    /* Base station identification */
  mcc_tx_buf.acc.pilot_pn = cur_bs_ptr->pilot_pn;      /* Pilot PN seq offset index */

  if (mccsa_access_attempt_is_request)
  {
    /* ---------------------------------------
    ** Compute value of P for persistence test
    ** --------------------------------------- */
    return (mccsa_calc_psist(CAI_ACH, ap_ptr, curr_state));
  }
  else
  {
    return NOT_ABORT;
  }

} /* mccsa_populate_ach_access_cmd */

#ifdef FEATURE_IS2000_REL_A_CC
/*===========================================================================

FUNCTION MCCSA_POPULATE_REACH_ACCESS_CMD

DESCRIPTION
  This function populates the parameters in the txc_reach_cmd_type structure
  in preparation for a TXC_REACH_F or TXC_REACH_PROBE_F.

DEPENDENCIES
  Requires non-NULL eap_ptr, pointing to valid access parameters message.

RETURN VALUE
  The return value of mccsa_calc_psist() if the access message to be sent is a 
  response. Otherwise, NOT_ABORT.

SIDE EFFECTS
  This function modifies the global mcc_tx_buf union.
===========================================================================*/

access_result_type mccsa_populate_reach_access_cmd
(
  caii_en_ac_parm_type *eap_ptr,
  word curr_state,
  byte access_mode,
  txc_each_rate_type rate_word
)
{
  /* Verify parameters are supported */
  if
  (!(
    /* Slot offsets must be zero */
    (eap_ptr->mode_parms[access_mode].each_slot_offset1 == 0)
    &&
    (eap_ptr->mode_parms[access_mode].each_slot_offset2 == 0)
    &&
    /* Must have preamble */
    (eap_ptr->mode_parms[access_mode].each_preamble_enabled == TRUE)
    &&
    /* Preamble length is 5ms multiple */
    ((((eap_ptr->mode_parms[access_mode].each_preamble_num_frac+1) 
    * (eap_ptr->mode_parms[access_mode].each_preamble_frac_duration+1)) % 4) == 0)
    &&
    /* Slot length is 5ms multiple */
    (((eap_ptr->mode_parms[access_mode].each_slot+1) % 4) == 0)
    &&
    /* No gating */
    (eap_ptr->mode_parms[access_mode].each_preamble_off_duration == 0)
    &&
    /* No add_duration supported currently */
    (eap_ptr->mode_parms[access_mode].each_preamble_add_duration == 0)
  ))
  {
    MSG_ERROR("Unsupported R-EACH parameter",0,0,0);
    return ABORT_UNSUPPORTED_PARAMETER;  /* Cancel access attempt */
  }

  mcc_tx_buf.reach.rate = rate_word;
  mcc_tx_buf.reach.req = mccsa_access_attempt_is_request;
  mcc_tx_buf.reach.acc_tmo = eap_ptr->acc_tmo;
  mcc_tx_buf.reach.bkoff = eap_ptr->mode_parms[access_mode].each_bkoff;
  mcc_tx_buf.reach.probe_bkoff = eap_ptr->mode_parms[access_mode].each_probe_bkoff;

  mcc_tx_buf.reach.init_pwr = eap_ptr->mode_parms[access_mode].each_init_pwr;

  /* -----------------------------------------------------------
  ** Compute the nominal power adjustment required by the access
  ** parameters message.
  ** ----------------------------------------------------------- */

  /* Sign extend nom_pwr */
  if (eap_ptr->mode_parms[access_mode].each_nom_pwr & 0x10)  /* Sign bit (5 bit field) */
  {
    mcc_tx_buf.reach.nom_pwr = (byte) (eap_ptr->mode_parms[access_mode].each_nom_pwr | 0xE0);
  }
  else
  {
    mcc_tx_buf.reach.nom_pwr = (byte) (eap_ptr->mode_parms[access_mode].each_nom_pwr & 0x1F);
  }

  mcc_tx_buf.reach.pwr_step = eap_ptr->mode_parms[access_mode].each_pwr_step;
  mcc_tx_buf.reach.num_step = eap_ptr->mode_parms[access_mode].each_num_step;

  /* ------------------------------------------------------------
  ** The following three parameters are used for constructing the
  ** Access Channel long code mask
  ** ------------------------------------------------------------ */
  mcc_tx_buf.reach.fccch   = cdma.fccch_id;  // Current FCC Channel.
  mcc_tx_buf.reach.base_id  = cur_bs_ptr->csp.sp.base_id;
  mcc_tx_buf.reach.pilot_pn = cur_bs_ptr->pilot_pn;

  if ( mccsa_access_attempt_is_request )
  {
    mcc_tx_buf.reach.seq_num = eap_ptr->max_req_seq;
  }
  else
  {
    mcc_tx_buf.reach.seq_num = eap_ptr->max_rsp_seq;
  }

  mcc_tx_buf.reach.preamble_enabled = eap_ptr->mode_parms[access_mode].each_preamble_enabled;
  mcc_tx_buf.reach.preamble_num_frac = eap_ptr->mode_parms[access_mode].each_preamble_num_frac;
  mcc_tx_buf.reach.preamble_frac_duration = eap_ptr->mode_parms[access_mode].each_preamble_frac_duration;
  mcc_tx_buf.reach.preamble_off_duration = eap_ptr->mode_parms[access_mode].each_preamble_off_duration;
  mcc_tx_buf.reach.preamble_add_duration = eap_ptr->mode_parms[access_mode].each_preamble_add_duration;
  // mcc_tx_buf.reach.each_acc_thresh = eap_ptr->mode_parms[access_mode].each_access_thresh;
  mcc_tx_buf.reach.slot = eap_ptr->mode_parms[access_mode].each_slot;
  mcc_tx_buf.reach.slot_offset1 = eap_ptr->mode_parms[access_mode].each_slot_offset1;
  mcc_tx_buf.reach.slot_offset2 = eap_ptr->mode_parms[access_mode].each_slot_offset2;

  // mcc_tx_buf.reach.num_rccch = eap_ptr->num_rccch;
  // mcc_tx_buf.reach.rccch_init_pwr = eap_ptr->rccch_init_pwr;
  // mcc_tx_buf.reach.rccch_nom_pwr = eap_ptr->rccch_nom_pwr;
  // mcc_tx_buf.reach.rccch_pre_enabled = eap_ptr->rccch_preamble_enabled;
  // mcc_tx_buf.reach.rccch_pre_num_frac = eap_ptr->rccch_preamble_num_frac;
  // mcc_tx_buf.reach.rccch_pre_frac = eap_ptr->rccch_preamble_frac_duration;
  // mcc_tx_buf.reach.rccch_pre_off = eap_ptr->rccch_preamble_off_duration;
  // mcc_tx_buf.reach.rccch_pre_add = eap_ptr->rccch_preamble_add_duration;
  // mcc_tx_buf.reach.rccch_slot = eap_ptr->rccch_slot;
  // mcc_tx_buf.reach.rccch_slot_offset1 = eap_ptr->rccch_slot_offset1;
  // mcc_tx_buf.reach.rccch_slot_offset2 = eap_ptr->rccch_slot_offset2;

  // mcc_tx_buf.reach.ra_pc_delay = eap_ptr->ra_pc_delay;
  // mcc_tx_buf.reach.eacam_cach_delay = eap_ptr->eacam_cach_delay;
  // mcc_tx_buf.reach.rccch_ho_supported = eap_ptr->rccch_ho_supported;
  // mcc_tx_buf.reach.rccch_ho_thresh = eap_ptr->rccch_ho_thresh;
  // mcc_tx_buf.reach.eacam_pccam_delay = eap_ptr->eacam_pccam_delay;
  // mcc_tx_buf.reach.num_cpcch = eap_ptr->num_cpcch;
  // mcc_tx_buf.reach.cpcch_rate = eap_ptr->cpcch_rate;
  // for(i=0; i<CAI_MAX_NUM_CPCCH; i++)
  // {
  //   mcc_tx_buf.reach.cpcch_code_chan[i] = eap_ptr->cpcch_code_chan[i];
  // }
  // mcc_tx_buf.reach.num_pcsch_ra = eap_ptr->num_pcsch_ra;

  // mcc_tx_buf.reach.num_cach = eap_ptr->num_cach;
  // mcc_tx_buf.reach.cach_code_rate = eap_ptr->cach_code_rate;
  // for(i=0; i<CAI_MAX_NUM_CACH; i++)
  // {
  //   mcc_tx_buf.reach.cach_code_chan[i] = eap_ptr->cach_code_chan[i];
  // }

  mcc_tx_buf.reach.num_each_ba = eap_ptr->num_each_ba;
  // mcc_tx_buf.reach.num_each_ra = eap_ptr->num_each_ra;

  mcc_tx_buf.reach.rlgain_common_pilot = eap_ptr->rlgain_common_pilot;
  mcc_tx_buf.reach.ic_thresh = eap_ptr->ic_thresh;
  mcc_tx_buf.reach.ic_max = eap_ptr->ic_max;

  if (mccsa_access_attempt_is_request)
  {
    /* ---------------------------------------
    ** Compute value of P for persistence test
    ** --------------------------------------- */
    return (mccsa_calc_psist(CAI_EACH, eap_ptr, curr_state));
  }
  else
  {
    return NOT_ABORT;
  }

} /* mccsa_populate_reach_access_cmd */
#endif /* FEATURE_IS2000_REL_A_CC */

/*===========================================================================

FUNCTION MCCSA_MAP_SA_STATE_TO_SA_REASON

DESCRIPTION
  This function determines the system access reason based on system access
  state.

DEPENDENCIES
  None.

RETURN VALUE
  System access reason.

SIDE EFFECTS
  None.
===========================================================================*/

sd_ss_acc_reas_e_type mccsa_map_sa_state_to_sa_reason
(
  word curr_state /* System access state */
)
{
  sd_ss_acc_reas_e_type sa_reason;

  switch (curr_state)
  {
    case CDMA_ORIG:
      sa_reason = SD_SS_ACC_REAS_ORIG;
      break;

    case CDMA_PAGE_RES:
      sa_reason = SD_SS_ACC_REAS_PAGE;
      break;

    case CDMA_REG:
      sa_reason = SD_SS_ACC_REAS_REG;
      break;

    case CDMA_MSG:
      sa_reason = SD_SS_ACC_REAS_DATA;
      break;

    default:
      sa_reason = SD_SS_ACC_REAS_OTHER;
      break;
  } /* end switch */

  return (sa_reason);

} /* mccsa_map_sa_state_to_sa_reason */


#ifdef FEATURE_GPS_BS_CAL_TEST
#error code not present
#endif

/*===========================================================================

FUNCTION MCCSA_COMPUTE_AC_MSG_NUM_PLT_RPT

DESCRIPTION
  This function computes the number of pilots that can be contained in
  Access Channel Message without exceeding the maximum capsule size.

DEPENDENCIES
  None.

RETURN VALUE
  Maximum number of pilots that can be reported in AC Msg.

SIDE EFFECTS
  None.

===========================================================================*/
byte mccsa_compute_ac_msg_num_plt_rpt
(
  word max_message_body /* maximum allowable length of AC Msg body */
)
{
  int  cal_num_plt; /* calculated number of pilots may be reported in AC Msg */
  byte num_plt_rpt; /* maximum number of pilots can be reported in AC Msg */

  /* ---------------------------------------------------------------------
  **  Calculated num of pilots may be reported in AC Msg
  **  = ( Max allowable length of AC Msg body -
  **      length of AC Msg excluding reporting additional pilot ) /
  *     Additional AC Msg length required for reporting an additional pilot)
  ** ---------------------------------------------------------------------*/
  cal_num_plt = ((int)(max_message_body - mccsa_pilot_rpt_ovhd_rec.msg_len)) / (int) sizeof(cai_plt_rpt_var_type);

  /* No additional pilot can be reported */
  if ( cal_num_plt <= 0 )
  {
     num_plt_rpt = 1; /* only Active pilot will be reported */
  }
  /* More than N13m pilots can be reported in AC Msg */
  else if ( cal_num_plt > CAI_N13M )
  {
    num_plt_rpt = CAI_N13M; /* only N13m pilots will be reported */
  }
  else
  {
    num_plt_rpt = (byte) (cal_num_plt);
  }

  return num_plt_rpt;

} /* end mccsa_compute_ac_msg_num_plt_rpt */

/*===========================================================================

FUNCTION MCCSA_GET_PREV_AHO_INDEX

DESCRIPTION
  This function gets the index of a pilot in the previous Access Handoff List.

DEPENDENCIES
  None.

RETURN VALUE
  Returns the index value of this pilot in the first Access Handoff List.
  "-1" indicates that this pilot is not in the first Access Handoff list.

SIDE EFFECTS
  None.

===========================================================================*/
int mccsa_get_prev_aho_index
(
  word pilot_pn
)
{
  int i;                /* loop counter */
  int idx = -1; /* index of pilot in Access Attempt list */

  for (i=0; i<mccsa_prev_access_ho_list.num_pilots; i++)
  {
    if (pilot_pn == mccsa_prev_access_ho_list.pilot_list[i].pilot_pn)
    {
      idx = i;
      break;
    }
  }
  return idx;

} /* end mccsa_get_prev_aho_index */

/*===========================================================================

FUNCTION MCCSA_GET_ACC_ATT_INDEX

DESCRIPTION
  This function gets the index of a pilot in Access Attempt list.

DEPENDENCIES
  None.

RETURN VALUE
  Returns the index value of this pilot in Access Attempt list.
  "-1" indicates that this pilot is not in Access Attempt list.

SIDE EFFECTS
  None.

===========================================================================*/
int mccsa_get_acc_att_index
(
  word pilot_pn
)
{
  int i;                /* loop counter */
  int acc_att_idx = -1; /* index of pilot in Access Attempt list */

  for (i=0; i<mccsa_pilot_rpt_ovhd_rec.num_pilots_attempted; i++)
  {
    if (pilot_pn == mccsa_pilot_rpt_ovhd_rec.attempt_list[i].pilot_pn)
    {
      acc_att_idx = i;
      break;
    }
  }
  return acc_att_idx;

} /* end mccsa_get_acc_att_index */

/*============================================================================

FUNCTION MCCSA_SET_PILOT_NGHBR_REC

DESCRIPTION
  This function sets the neighbor record info of pilot in Access Attempt list.

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

==============================================================================*/
void mccsa_set_pilot_nghbr_rec
(
  cdma_nghbr_set_type *acc_att_rec,
  cdma_nghbr_set_type *nghbr_rec
)
{
  acc_att_rec->search_pri = nghbr_rec->search_pri;
  acc_att_rec->freq_incl = nghbr_rec->freq_incl;
  acc_att_rec->band = nghbr_rec->band;
  acc_att_rec->freq = nghbr_rec->freq;
  acc_att_rec->config = nghbr_rec->config;
  if (P_REV_IS_4_OR_GREATER)
  {
    acc_att_rec->access_entry_ho = nghbr_rec->access_entry_ho;
    acc_att_rec->access_ho_allowed = TRUE;
    acc_att_rec->srch_win_nghbr = nghbr_rec->srch_win_nghbr;
    acc_att_rec->timing_incl = nghbr_rec->timing_incl;
    if (nghbr_rec->timing_incl)
    {
      acc_att_rec->nghbr_tx_offset = nghbr_rec->nghbr_tx_offset;
      acc_att_rec->nghbr_tx_duration = nghbr_rec->nghbr_tx_duration;
      acc_att_rec->nghbr_tx_period = nghbr_rec->nghbr_tx_period;
    }
  }

} /* end mccsa_set_pilot_nghbr_rec */

/*===========================================================================

FUNCTION MCCSA_FIND_AHO_WEAKEST_PILOT

DESCRIPTION
  This function find the weakest pilot in the ACCESS_HO_LIST excluding
  members to which access probes have been transmitted since transmitting the
  first access probe in the System Access State.

DEPENDENCIES
  None

RETURN VALUE
  0   - No pilot in the list can be swapped
  > 0 - Index of weakest pilot in the ACCESS_HO_LIST excluding members to
        which access probes have been transmitted since transmitting the
        first access probe in the System Access State.

SIDE EFFECTS
  None

===========================================================================*/
int mccsa_find_aho_weakest_pilot (void)
{
  int  i;                  /* Index of pilots in AHO list */
  int  weakest_plt_idx=-1; /* Index of weakest pilot in AHO list */
  byte weakest_plt_str=0;  /* Strength of weakest pilot */

  /* ---------------------------------------------------------------------
  ** Loop through all not access attempted members in the list to find the
  ** weakest pilot
  ** --------------------------------------------------------------------- */
  for (i=mccsa_pilot_rpt_ovhd_rec.num_pilots_attempted;
       i<mccsa_access_ho_list.num_pilots; i++)
  {
    /* ---------------------------------------------------------------------
    ** If not yet find a pilot that can be swapped, any not access attempted
    ** pilot can be swapped
    ** ------------------------------------------------------------------- */
    if (weakest_plt_idx == -1)
    {
      weakest_plt_idx = i;
      weakest_plt_str = mccsa_access_ho_list.pilot_list[i].pilot_strength;
    }
    /* --------------------------------------------------------------------
    ** If already find a tentative pilot that can be swapped, the criteria
    ** to determine this pilot is a better candidate for swapping are
    ** whether its strength is weaker.
    **
    ** Pilot strength is in negative db unit, so smaller magnitude in
    ** strength implies stronger pilot.
    ** -------------------------------------------------------------------- */
    else if (weakest_plt_str <
             mccsa_access_ho_list.pilot_list[i].pilot_strength)
    {
      weakest_plt_idx = i;
      weakest_plt_str = mccsa_access_ho_list.pilot_list[i].pilot_strength;
    }
  }

  return weakest_plt_idx;

} /* end mccsa_find_aho_weakest_pilot */

/*===========================================================================

FUNCTION MCCSA_SET_AHO_PILOT_INFO

DESCRIPTION
  This function sets the current pilot information from the Pilot Report list
  into the ACCESS_HO_LIST list.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void mccsa_set_aho_pilot_info
(
  aho_pilot_info_type* aho_pilot,
  pilot_rpt_list_type* srchrpt_pilot
)
{
  aho_pilot->pilot_pn = srchrpt_pilot->pilot_pn;
  aho_pilot->pilot_pn_phase = srchrpt_pilot->pilot_pn_phase;
  aho_pilot->pilot_strength = srchrpt_pilot->pilot_strength;

  /* ------------------------------------------------------------
  ** Active pilot's ACCESS_ATTEMPTED field is not set in Srch Rpt
  ** ------------------------------------------------------------  */
  if (aho_pilot->pilot_pn == cur_bs_ptr->pilot_pn)
  {
    mccsa_access_ho_list.pilot_list[0].access_attempted = TRUE;
  }
  else
  {
    aho_pilot->access_attempted = srchrpt_pilot->access_attempted;
  }

} /* end mccsa_set_aho_pilot_info */

/*===========================================================================

FUNCTION MCCSA_BUILD_ACCESS_HO_LIST

DESCRIPTION
  This function builds ACCESS_HO_LIST based on searcher pilot strength
  measurement report. Refer to IS-95B (6.6.3.1.7.1-2).

DEPENDENCIES
  Availability of pilot strength measurement report from searcher.

RETURN VALUE
  FALSE if fail to build ACCESS_HO_LIST else TRUE.

SIDE EFFECTS
  None.

===========================================================================*/
boolean mccsa_build_access_ho_list
(
  mccsrch_rpt_type *srch_rpt_ptr /* Current Searcher report */
)
{
  int i;           /* Index of srch report */
  int swap_idx;    /* Index of pilot in ACCESS_HO_LIST list to be swapped */
  int pplt_idx;    /* index of Previous pilot in ACCESS_HO_LIST */
  int att_idx;     /* Index of access attempted pilot in Access Attempt list */
  int other_att_idx;
    /* Index of access attempted pilot (other then Active, First and/or
       Previous pilot) in ACCESS_HO_LIST */
  boolean add_new_pilot_ok = TRUE;
    /* Indicates whether new pilot (other than active pilot) can be added to
       ACCESS_HO_LIST */
  boolean listupd_succeed = TRUE;
    /* Indicates whether ACCESS_HO_LIST is contructured successfully */

  if (mccsa_pilot_rpt_ovhd_rec.apho_rebuild)
  {
    if (!mccsa_pilot_rpt_ovhd_rec.first_probe && 
        (!P_REV_IS_4_OR_GREATER || !cur_bs_ptr->csp.esp.access_probe_ho ||
         !cur_bs_ptr->csp.esp.acc_ho_list_upd))
    {
      add_new_pilot_ok = FALSE;
    }
    /* Clear APHO rebuild flag */
    mccsa_pilot_rpt_ovhd_rec.apho_rebuild = FALSE;
  }

  /* Clear the Access Handoff list */
  mccsa_access_ho_list.num_pilots = 0;

  /* ------------------------------------------------
  ** Get index of Active pilot in access attempt list
  ** ------------------------------------------------ */
  att_idx = mccsa_get_acc_att_index(cur_bs_ptr->pilot_pn);

  /* ------------------------------------------
  ** Active pilot not in Access Attempt List
  ** ------------------------------------------ */
  if (att_idx == -1)
  {
    /* ----------------------------------------------------------------
    ** Failure to update the ACCESS_HO_LIST because MS cannot report
    ** all access attempted pilots in access channel messages without
    ** exceeding BS Max capsule size
    ** --------------------------------------------------------------- */
    if (mccsa_pilot_rpt_ovhd_rec.num_pilots_attempted ==
        mccsa_pilot_rpt_ovhd_rec.num_pilots_allowed)
    {
      listupd_succeed = FALSE; /* failed to update the list */
      MSG_MED("Failure in building ACCESS_HO_LIST", 0, 0, 0);
    }
    else
    {
      /* ---------------------------------------
      ** Add Active pilot to access attempt list
      ** --------------------------------------- */
      att_idx = mccsa_pilot_rpt_ovhd_rec.num_pilots_attempted;
      mccsa_pilot_rpt_ovhd_rec.attempt_list[att_idx].pilot_pn =
        cur_bs_ptr->pilot_pn;
      mccsa_pilot_rpt_ovhd_rec.num_pilots_attempted++;
    }
  }

  /* -------------------------------------------
  ** Active pilot already in Access Attempt List
  ** ------------------------------------------- */
  else
  {
    /* ----------------------------------------------------------------
    ** Failure to update the ACCESS_HO_LIST because MS cannot report
    ** all access attempted pilots in access channel messages without
    ** exceeding BS Max capsule size
    ** --------------------------------------------------------------- */
    if (mccsa_pilot_rpt_ovhd_rec.num_pilots_attempted >
        mccsa_pilot_rpt_ovhd_rec.num_pilots_allowed)
    {
      listupd_succeed = FALSE; /* failed to update the list */
      MSG_MED( "Failure in building ACCESS_HO_LIST", 0, 0, 0 );
    }
  } /* end if acc_att_idx = -1 */

  /* ---------------------------
  ** Process Srch Plt Str report
  ** --------------------------- */
  if (listupd_succeed)
  {
    /* ----------------------------------
    ** Add active pilot to ACCESS_HO_LIST
    ** ----------------------------------*/
    mccsa_access_ho_list.num_pilots = 1;

    mccsa_set_aho_pilot_info(&mccsa_access_ho_list.pilot_list[0],
      &srch_rpt_ptr->pilots_meas.pilot_list[0]);

    /* ---------------------------------------------------
    ** Additional pilot can be added in ACCESS_HO_LIST
    ** --------------------------------------------------- */
    if (mccsa_pilot_rpt_ovhd_rec.num_pilots_allowed > 1)
    {
      /* ---------------------------------------------------------------
      ** Set the index where we wil put Previous pilot in ACCESS_HO_LIST
      ** --------------------------------------------------------------- */
      /* No previous pilot - First probe */
      if (mccsa_pilot_rpt_ovhd_rec.prev_pilot == NO_PREV_PILOT)
      {
        pplt_idx = -1;
      }
      /* Active pilot, First pilot and Prev pilot are all distinct */
      else if ((mccsa_pilot_rpt_ovhd_rec.first_pilot !=
                mccsa_access_ho_list.pilot_list[0].pilot_pn) &&
               (mccsa_pilot_rpt_ovhd_rec.first_pilot !=
                mccsa_pilot_rpt_ovhd_rec.prev_pilot))
      {
        /* Reserve the second and third spot for First pilot and Previous
        pilot */
        pplt_idx = 2;
      }
      /* Either First pilot is the same as Active pilot or Prev pilot is the
         same as First pilot */
      else
      {
        /* Reserve the second spot for Prev pilot */
        pplt_idx = 1;
      }

      /* --------------------------------------------------------------
      ** Set the location where we start putting other access attempted
      ** pilots (excluding First and Previous pilot) in ACCESS_HO_LIST.
      ** Those pilots will be put right after Previous pilot.
      ** -------------------------------------------------------------- */
      other_att_idx = pplt_idx+1;

      /* --------------------------------------------------------------
      ** Set the location where we start putting not access attempted
      ** pilots in ACCESS_HO_LIST. Those pilots will be put right after
      ** all access attempted pilot.
      ** -------------------------------------------------------------- */
      mccsa_access_ho_list.num_pilots =
        mccsa_pilot_rpt_ovhd_rec.num_pilots_attempted;

      /* --------------------------------------
      ** Add additional pilot to ACCESS_HO_LIST
      ** -------------------------------------- */
      for (i=1; i<srch_rpt_ptr->pilots_meas.num_pilots+1; i++)
      {
        /* --------------------------------------------------------------
        ** Add pilot to the Access Handoff list if it meets the criteria:
        ** (1) It has been access attempted.
        ** -------------------------------------------------------------- */
        if (srch_rpt_ptr->pilots_meas.pilot_list[i].access_attempted)
        {
          /* ----------------------------------------------------------
          ** Put First pilot right after Active pilot in ACCESS_HO_LIST
          ** ---------------------------------------------------------- */
          if (srch_rpt_ptr->pilots_meas.pilot_list[i].pilot_pn ==
              mccsa_pilot_rpt_ovhd_rec.first_pilot)
          {
            mccsa_set_aho_pilot_info(&mccsa_access_ho_list.pilot_list[1],
              &srch_rpt_ptr->pilots_meas.pilot_list[i]);
          }
          /* ------------------------------------
          ** Put Previous pilot in ACCESS_HO_LIST
          ** ------------------------------------ */
          else if (srch_rpt_ptr->pilots_meas.pilot_list[i].pilot_pn ==
                   mccsa_pilot_rpt_ovhd_rec.prev_pilot)
          {
            if (pplt_idx < 0) 
            {
              ERR_FATAL("negative pplt_idx", 0, 0, 0);
            }
            mccsa_set_aho_pilot_info(
              &mccsa_access_ho_list.pilot_list[pplt_idx],
              &srch_rpt_ptr->pilots_meas.pilot_list[i]); 
          }
          /* ---------------------------------------------------
          ** Put other access attempted pilots in ACCESS_HO_LIST
          ** --------------------------------------------------- */
          else
          {
            mccsa_set_aho_pilot_info(
              &mccsa_access_ho_list.pilot_list[other_att_idx],
              &srch_rpt_ptr->pilots_meas.pilot_list[i]);
            /* Increment index */
            other_att_idx++;
          }
        }

        /* --------------------------------------------------------------
        ** Add pilot to the Access Handoff list if it meets the criteria:
        ** (2) ACC_UPD_LIST = 1: Pilot has not been access attempted, its
        **     ACCESS_HO_ALLOWED is enabled, its pilot strength is >
        **     T_ADD and including it in ACCESS_HO_LIST does not exceed
        **     max capsule size.
        **                       or
        ** (3) ACC_UPD_LIST = 0: Pilot is in the last ACCESS_HO_LIST and
        **     including it in ACCESS_HO_LIST does not exceed max capsule
        **     size.
        **
        ** Pilot strength is in negative db unit, so smaller magnitude in
        ** strength implies stronger pilot.
        ** -------------------------------------------------------------- */
        else if ((mccsa_pilot_rpt_ovhd_rec.num_pilots_attempted <
                  mccsa_pilot_rpt_ovhd_rec.num_pilots_allowed) &&
                 ((add_new_pilot_ok &&
                   srch_rpt_ptr->pilots_meas.pilot_list[i].access_ho_allowed &&
                   (srch_rpt_ptr->pilots_meas.pilot_list[i].pilot_strength <
                    mcc_ho_real.t_add)) ||
                  (!add_new_pilot_ok &&
                   (mccsa_get_prev_aho_index(
                      (word)srch_rpt_ptr->pilots_meas.pilot_list[i].pilot_pn) !=
                    -1))))
        {
          /* -------------------------------------------------
          ** Add pilot to ACCESS_HO_LIST until it is filled up
          ** ------------------------------------------------- */
          if (mccsa_access_ho_list.num_pilots <
              mccsa_pilot_rpt_ovhd_rec.num_pilots_allowed)
          {
            mccsa_set_aho_pilot_info(
              &mccsa_access_ho_list.pilot_list[mccsa_access_ho_list.num_pilots],
              &srch_rpt_ptr->pilots_meas.pilot_list[i]);
            /* Increment count */
            mccsa_access_ho_list.num_pilots += 1;
          }
          /* ------------------------------------------------
          ** Already filled up ACCESS_HO_LIST, swapping pilot
          ** ------------------------------------------------ */
          else
          {
            /* Find the index of weakest and not access attempted pilot in
               ACCESS_HO_LIST */
            swap_idx = mccsa_find_aho_weakest_pilot();

            /* -------------------------------------------------------
            ** Do swapping if the pilot which can be swapped is weaker
            **
            ** Pilot strength is in negative db unit, so smaller
            ** magnitude in strength implies stronger pilot.
            ** ------------------------------------------------------- */
            if ((swap_idx != -1) &&
                (srch_rpt_ptr->pilots_meas.pilot_list[i].pilot_strength <
                 mccsa_access_ho_list.pilot_list[swap_idx].pilot_strength))
            {
              mccsa_set_aho_pilot_info(
                &mccsa_access_ho_list.pilot_list[swap_idx],
                &srch_rpt_ptr->pilots_meas.pilot_list[i]);
            }
          }
        } /* end if srch_rpt_ptr->pilots_meas.pilot_list[i].access_attempted */
      }
    } /* end if mccsa_pilot_rpt_ovhd_rec.num_pilots_allowed > 1 */
  }

  /* -----------------------------------------
  ** Succeed in construction of ACCESS_HO_LIST
  ** ----------------------------------------- */
  if (listupd_succeed)
  {
    /* -----------
    ** First Probe
    ** ----------- */
    if (mccsa_pilot_rpt_ovhd_rec.first_probe)
    {
      /* Save First pilot PN */
      mccsa_pilot_rpt_ovhd_rec.first_pilot =
        mccsa_access_ho_list.pilot_list[0].pilot_pn;

      /* Clear first probe flag */
      mccsa_pilot_rpt_ovhd_rec.first_probe = FALSE;
    }

    /* Save access handoff list for next update */
    memcpy(&mccsa_prev_access_ho_list, &mccsa_access_ho_list,
      sizeof(aho_list_type));
  }

  return (listupd_succeed);

} /* end mccsa_build_access_ho_list */

/*===========================================================================

FUNCTION MCCSA_UPDATE_ACCESS_HO_LIST

DESCRIPTION
  This function updates ACCESS_HO_LIST based on searcher pilot strength
  measurement report. Refere to IS-95B (6.6.3.1.7.2).

DEPENDENCIES
  Availability of pilot strength measurement report from searcher.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/
void mccsa_update_access_ho_list
(
  mccsrch_rpt_type *srch_rpt_ptr /* Current Searcher report */
)
{
  int srch_idx; /* index of searcher pilot strength measurement report */
  int ahol_idx; /* index of access handoff list */


  /* ------------------------------------------------------
  ** Update PN offset, PN phase and access attempted fields
  ** ------------------------------------------------------ */
  for (ahol_idx=0; ahol_idx<mccsa_access_ho_list.num_pilots; ahol_idx++)
  {
    /* Match records against Searcher Pilot Strength Measurement report */
    for (srch_idx=0; srch_idx<srch_rpt_ptr->pilots_meas.num_pilots+1;
         srch_idx++)
    {
      if (srch_rpt_ptr->pilots_meas.pilot_list[srch_idx].pilot_pn ==
          mccsa_access_ho_list.pilot_list[ahol_idx].pilot_pn)
      {
        /* Update pilot info */
        mccsa_set_aho_pilot_info(&mccsa_access_ho_list.pilot_list[ahol_idx],
          &srch_rpt_ptr->pilots_meas.pilot_list[srch_idx]);
        break;
      }
    }
  } /* end for */

} /* end mccsa_update_access_ho_list */

/*===========================================================================

FUNCTION MCCSA_FIND_OTHRPT_WEAKEST_PILOT

DESCRIPTION
  This function find the weakest pilot in the OTHER_REPORTED_LIST list.

DEPENDENCIES
  None

RETURN VALUE
  Index of weakest pilot in the OTHER_REPORTED_LIST.

SIDE EFFECTS
  None

===========================================================================*/
int mccsa_find_othrpt_weakest_pilot (void)
{
  int  i;               /* index of pilots in Other Reported list */
  int  weakest_plt_idx; /* Index of weakest pilot in Other Reported list */
  byte weakest_plt_str; /* strength of weakest pilot in Other Reported list */

  /* initialize weakest pilot index and strength to be the first pilot
     in the list */
  weakest_plt_idx = 0;
  weakest_plt_str = mccsa_other_reported_list.pilot_list[0].pilot_strength;

  /* loop through the list to find the weakest pilot */
  for (i=1; i<mccsa_other_reported_list.num_pilots; i++)
  {
    /* --------------------------------------------------------------
    ** Pilot strength is in negative db unit, so smaller magnitude in
    ** strength implies stronger pilot.
    ** --------------------------------------------------------------*/
    if (weakest_plt_str <
        mccsa_other_reported_list.pilot_list[i].pilot_strength)
    {
      weakest_plt_idx = i;
      weakest_plt_str = mccsa_other_reported_list.pilot_list[i].pilot_strength;
    }
  }

  return weakest_plt_idx;

} /* end mccsa_find_othrpt_weakest_pilot */

/*===========================================================================

FUNCTION MCCSA_SET_OTHRPT_PILOT_INFO

DESCRIPTION
  This function sets the current pilot information from the Pilot Report list
  into the OTHER_PILOT_LIST list.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void mccsa_set_othrpt_pilot_info
(
  othrpt_pilot_info_type* othrpt_pilot,
  pilot_rpt_list_type*    srchrpt_pilot
)
{
  othrpt_pilot->pilot_pn = srchrpt_pilot->pilot_pn;
  othrpt_pilot->pilot_pn_phase = srchrpt_pilot->pilot_pn_phase;
  othrpt_pilot->pilot_strength = srchrpt_pilot->pilot_strength;

} /* end mccsa_set_othrpt_pilot_info */

/*===========================================================================

FUNCTION MCCSA_BUILD_OTHER_REPORTED_LIST

DESCRIPTION
  This function builds OTHER_REPORTED_LIST based on searcher pilot
  strength measurement report. Refer to IS-95B (6.6.3.1.7.3-4).

DEPENDENCIES
  Availability of pilot strength measurement report from searcher.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/
void mccsa_build_other_reported_list
(
  mccsrch_rpt_type *srch_rpt_ptr /* Current Searcher report */
)
{
  int i;        /* index of searcher pilot strength report */
  int swap_idx; /* Index of pilot in the list to be swapped */
  byte num_pilots_allowed; /* maximum allowed pilots in other reported list */

  /* Clear OTHER_REPORTED_LIST */
  mccsa_other_reported_list.num_pilots = 0;

  /* Calculated the number of pilots allowed in the list */
  num_pilots_allowed = mccsa_pilot_rpt_ovhd_rec.num_pilots_allowed -
                       mccsa_access_ho_list.num_pilots;

  /* -------------------------------------------------------------------
  ** Continue with building OTHER_REPORTED_LIST if more pilots can be
  ** contained in Access Channel Msg without exceeding Max capsule size.
  ** ------------------------------------------------------------------- */
  if (num_pilots_allowed > 0)
  {
    /* Process pilot strength report from Searcher excluding Active pilot */
    for (i=1; i<srch_rpt_ptr->pilots_meas.num_pilots+1; i++)
    {
      /* -----------------------------------------------------------------
      ** Add pilot to Other Reported list if it meets all of the following
      ** criteria:
      ** (1) It has not been access attempted.
      ** (2) Its ACCESS_HO_ALLOWED is disabled and its pilot strength >
      **     T_ADD.
      **
      ** Pilot strength is in negative db unit, so smaller magnitude in
      ** strength implies stronger pilot.
      ** ----------------------------------------------------------------- */
      if (!srch_rpt_ptr->pilots_meas.pilot_list[i].access_attempted &&
          (!srch_rpt_ptr->pilots_meas.pilot_list[i].access_ho_allowed &&
           (srch_rpt_ptr->pilots_meas.pilot_list[i].pilot_strength <
            mcc_ho_real.t_add)))
      {
        /* Add pilot to the list until the list is filled up */
        if (mccsa_other_reported_list.num_pilots < num_pilots_allowed)
        {
          mccsa_set_othrpt_pilot_info(
            &mccsa_other_reported_list.pilot_list[mccsa_other_reported_list.num_pilots],
            &srch_rpt_ptr->pilots_meas.pilot_list[i]);
          mccsa_other_reported_list.num_pilots += 1;
        }
        /* List is already filled up, swap the weakest pilot in the list */
        else
        {
          /* Find the index of weakest pilot in Other Reported list */
          swap_idx = mccsa_find_othrpt_weakest_pilot();

          /* -------------------------------------------------------
          ** Do swapping if the pilot which can be swapped is weaker
          **
          ** Pilot strength is in negative db unit, so smaller
          ** magnitude in strength implies stronger pilot.
          ** ----------------------------------------------------------*/
          if (srch_rpt_ptr->pilots_meas.pilot_list[i].pilot_strength <
              mccsa_other_reported_list.pilot_list[swap_idx].pilot_strength)
          {
            mccsa_set_othrpt_pilot_info(
              &mccsa_other_reported_list.pilot_list[swap_idx],
              &srch_rpt_ptr->pilots_meas.pilot_list[i]);
          }
        }
      } /* end if not access attempted, AHO allowed and Plt Str>T_ADD */
    }
  } /* end if number of pilots allowed > 0 */

} /* end mccsa_build_other_reported_list */

/*===========================================================================

FUNCTION MCCSA_SETUP_PILOT_RPT

DESCRIPTION
  This  function populates the pilot rpt information in the access channel mesg.
  Refer to IS-95B (6.7.1.3.1.3).
  Protoctype of this function was changed for IS2000

DEPENDENCIES
  None.

RETURN VALUE
  None

SIDE EFFECTS
  TX_EXT_AC_MSG.BODY will be updated.

===========================================================================*/

void mccsa_setup_pilot_rpt
(
  caii_plt_rpt_type *pilot_rpt_ptr
)
{
  int i, j;

  if (pilot_rpt_ptr == NULL)
  {
    return;
  }

  /* Set Active Pilot Strength based on info from ACCESS_HO_LIST */
  pilot_rpt_ptr->fix.act_plt_str = mccsa_access_ho_list.pilot_list[0].pilot_strength;

  /* Active Pilot is the first pilot on which an access probe was sent */
  if (mccsa_access_ho_list.pilot_list[0].pilot_pn ==
      mccsa_pilot_rpt_ovhd_rec.first_pilot)
  {
    pilot_rpt_ptr->fix.first_is_act = TRUE;
    pilot_rpt_ptr->fix.first_is_pta = FALSE;
  }
  /* The first pilot is the previous to the Active Pilot on which an
     Access Probe was sent */
  else if (mccsa_pilot_rpt_ovhd_rec.first_pilot ==
           mccsa_pilot_rpt_ovhd_rec.prev_pilot)
  {
    pilot_rpt_ptr->fix.first_is_act = FALSE;
    pilot_rpt_ptr->fix.first_is_pta = TRUE;
  }
  else
  {
    pilot_rpt_ptr->fix.first_is_act = FALSE;
    pilot_rpt_ptr->fix.first_is_pta = FALSE;
  }

 /* -------------------------------------------------------------------
 ** Report additional pilots only if one of the following condition is
 ** true
 ** (1) Origination or Page Response or Reconnect Msg.
 ** (2) PILOT_REPORT = 1 in ESPM.
 ** -------------------------------------------------------------------*/
  if ((mccsa_layer2.last_msg_type == CAI_ORIG_MSG) ||
      (mccsa_layer2.last_msg_type == CAI_PAGE_RESP_MSG) ||
      #ifdef FEATURE_IS2000_REL_C
      (mccsa_layer2.last_msg_type == CAI_AC_RECONNECT_MSG) ||
      #endif /* FEATURE_IS2000_REL_C */
      (P_REV_IS_4_OR_GREATER && cur_bs_ptr->csp.esp.pilot_report))
  {
    pilot_rpt_ptr->fix.num_add_plt = mccsa_access_ho_list.num_pilots +
                                     mccsa_other_reported_list.num_pilots - 1;
  }
  else
  {
    pilot_rpt_ptr->fix.num_add_plt = 0;
  }

  #ifdef FEATURE_IS2000_REL_A
  /* Auxiliary pilot's are not supported now, hence set the number of
     aux pilots to 0 to indicate that no auxiliary pilot power is reported.
     Note that when/if Auxiliary Pilots are implemented this part needs
     to change. */
  if (P_REV_IS_7_OR_GREATER)
  {
    pilot_rpt_ptr->aux_fix.num_aux_plt = 0;
  }
  #endif /* FEATURE_IS2000_REL_A */

  /* Set additional pilot info */
  if (pilot_rpt_ptr->fix.num_add_plt > 0)
  {
    /* Initialize index */
    j = 0;
    /* Put ACCESS_HO_LIST info */
    if (mccsa_access_ho_list.num_pilots > 1)
    {
      for (i=1; i < mccsa_access_ho_list.num_pilots; i++)
      {
        pilot_rpt_ptr->var[j].plt_pn_phase =
          (word) mccsa_access_ho_list.pilot_list[i].pilot_pn_phase;
        pilot_rpt_ptr->var[j].plt_strength =
          mccsa_access_ho_list.pilot_list[i].pilot_strength;
        pilot_rpt_ptr->var[j].acc_ho_en = TRUE;
        pilot_rpt_ptr->var[j].acc_attempted =
          mccsa_access_ho_list.pilot_list[i].access_attempted;
        j++;
      }
    }

    /* Put OTHER_REPORTED_LIST info */
    if (mccsa_other_reported_list.num_pilots > 0)
    {
      for (i=0; i < mccsa_other_reported_list.num_pilots; i++)
      {
        pilot_rpt_ptr->var[j].plt_pn_phase =
          (word) mccsa_other_reported_list.pilot_list[i].pilot_pn_phase;
        pilot_rpt_ptr->var[j].plt_strength =
          mccsa_other_reported_list.pilot_list[i].pilot_strength;
        pilot_rpt_ptr->var[j].acc_ho_en = FALSE;
        pilot_rpt_ptr->var[j].acc_attempted = FALSE;
        j++;
      }
    }
  }

} /* end mccsa_setup_pilot_rpt() */

/*===========================================================================

FUNCTION MCCSA_PROCESS_PLT_STR_MEAS_RPT

DESCRIPTION
  This function puts the pilot info from the Pilot Strength Measurement
  report into ACCESS_HO_LIST and OTHER_REPORTED_LIST.

DEPENDENCIES
  None

RETURN VALUE
  FALSE if failed to build ACCESS_HO_LIST, else TRUE.

SIDE EFFECTS
  None

===========================================================================*/
boolean mccsa_process_plt_str_meas_rpt
(
  mccsrch_rpt_type *srch_rpt_ptr /* Current Searcher report */
)
{
  boolean listupd_succeed; /* Flag if success in building or updating ACCESS_HO_LIST */

  listupd_succeed = TRUE;

  /* -----------------------------------------------------------------
  ** First Access Probe or ACC_HO_LIST_UPD = 1 in ESPM or Access probe
  ** handoff happened which trigger the rebuild of ACCESS_HO_LIST
  ** ----------------------------------------------------------------- */
  if (mccsa_pilot_rpt_ovhd_rec.first_probe ||
      (P_REV_IS_4_OR_GREATER && cur_bs_ptr->csp.esp.access_probe_ho &&
       cur_bs_ptr->csp.esp.acc_ho_list_upd) ||
      mccsa_pilot_rpt_ovhd_rec.apho_rebuild)
  {
    /* --------------------
    ** Build ACCESS_HO_LIST
    ** -------------------- */
    listupd_succeed = mccsa_build_access_ho_list(srch_rpt_ptr);
  }
  else
  {
    /* ---------------------
    ** Update ACCESS_HO_LIST
    ** --------------------- */
    mccsa_update_access_ho_list(srch_rpt_ptr);
  }

  /* --------------------------------------------------
  ** Success in build or update of ACCESS_HO_LIST
  ** -------------------------------------------------- */
  if (listupd_succeed)
  {
    /* -------------------------
    ** Build OTHER_REPORTED_LIST
    ** ------------------------- */
    mccsa_build_other_reported_list(srch_rpt_ptr);
  }
  /* -----------------------------------------------------------------------
  ** Failure in construction of ACCESS_HO_LIST because of MAX_CAP_SIZE limit
  ** which only occurs if Access Probe Handoff is allowed
  ** ----------------------------------------------------------------------- */
  else
  {
    /* ---------------------------------------------------------------------
    ** Clear the ACCESS_HO_LIST, OTHER_REPORTED_LIST and Access Attempt list
    ** --------------------------------------------------------------------- */
    mccsa_access_ho_list.num_pilots = 0;
    mccsa_prev_access_ho_list.num_pilots = 0;
    mccsa_other_reported_list.num_pilots = 0;
    mccsa_pilot_rpt_ovhd_rec.num_pilots_attempted = 0;
  }

  return (listupd_succeed);

} /* end mccsa_process_plt_str_meas_rpt */

/*===========================================================================

FUNCTION MCCSA_UPDATE_AHO_ACTIVE_PILOT

DESCRIPTION
  This function gets the Active Pilot info from Searcher and put it into
  ACCESS_HO_LIST.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void mccsa_update_aho_active_pilot(void)
{
  mccsa_access_ho_list.pilot_list[0].pilot_pn = (int2) cur_bs_ptr->pilot_pn;
  mccsa_access_ho_list.pilot_list[0].pilot_strength = (uint8) srch_get_pilot_ecio();
  mccsa_access_ho_list.pilot_list[0].access_attempted = TRUE;

} /* end mccsa_update_aho_active_pilot */

/*===========================================================================

FUNCTION MCCSA_WAIT_FOR_SRCH_PILOTS_MEAS_RPT

DESCRIPTION
  This function waits for searcher pilot strength measurement report at the
  beginning of access probe. If the Pilot Strength Measurement report is
  available, both ACCESS_HO_LIST and OTHER_REPORTED_LIST will be updated.
  If Searcher fails to return the Pilot Strength Measurement report because
  of missing reference pilot or whatever, we will just update Active Pilot
  info in ACCESS_HO_LIST.

DEPENDENCIES
  None

RETURN VALUE
  NOT_ABORT if success to build ACCESS_HO_LIST,
  ABORT_HO_LIST if fail to build ACCESS_HO_LIST,
  ABORT_SRCH_REQUEST_SYS_RESTART if transceiver resource lost.

SIDE EFFECTS
  Upon received the pilot strength measurement report, both ACCESS_HO_LIST
  and OTHER_REPORTED_LIST info will be updated.

===========================================================================*/

access_result_type mccsa_wait_for_srch_pilots_meas_rpt(void)
{
  mccsrch_rpt_type *srch_rpt_ptr; /* Current Searcher report */
  boolean continue_wait;          /* Status of Searcher report */
  access_result_type access_result = NOT_ABORT;

  /* Explicitly clear all Searcher reports */
  while ((srch_rpt_ptr = (mccsrch_rpt_type*) q_get(&mcc_srch_q)) != NULL)
  {
    if ( srch_rpt_ptr->hdr.rpt == SRCH_REQUEST_SYS_RESTART_R )
    {
      MSG_HIGH( "SRCH request sys restart", 0, 0, 0 );
      access_result = ABORT_SRCH_REQUEST_SYS_RESTART;
      cmd_done(&srch_rpt_ptr->hdr.rpt_hdr);
      break;
    }
    else
    {
      MSG_MED("Ignore Searcher report %d", srch_rpt_ptr->hdr.rpt, 0, 0);
      cmd_done(&srch_rpt_ptr->hdr.rpt_hdr);
    }
  }

  if ( access_result == NOT_ABORT )
  {
    (void) rex_clr_sigs(&mc_tcb, MCC_SRCH_Q_SIG);

    /* Command Searcher task to send pilot strength measurement information */
    do
    {
      mcc_srch_buf.pilot_rpt.hdr.cmd = SRCH_PILOT_LIST_F;
      mcc_srch_cmd( &mcc_srch_buf );

      if (mcc_srch_buf.pilot_rpt.hdr.status != SRCH_DONE_S)
      {
        MSG_HIGH("No ref sector, resent request", 0, 0, 0);
      }

    } while (mcc_srch_buf.pilot_rpt.hdr.status != SRCH_DONE_S);

    /* initialize status of Searcher report */
    continue_wait = TRUE;

    /* wait for Searcher's pilot strength measurement report */
    while (continue_wait)
    {
      (void) mcc_wait (MCC_SRCH_Q_SIG);

      if ((srch_rpt_ptr = (mccsrch_rpt_type*) q_get(&mcc_srch_q)) != NULL)
      {
        /* Pilot Strength Measurement report */
        if (srch_rpt_ptr->hdr.rpt == SRCH_AHO_PLIST_R)
        {
          continue_wait = FALSE;

          /* Report for current BS */
          if (srch_rpt_ptr->pilots_meas.pilot_list[0].pilot_pn ==
              cur_bs_ptr->pilot_pn)
          {
            /* ------------------------------------------------------------------
            ** Exit the wait loop and Process the Pilot Strength Measurement
            ** report as follow,
            ** (1) In case it is the first Access Probe or if ACC_HO_LIST_UPD = 1
            **     in ESPM, build ACCESS_HO_LIST and OTHER_REPORTED_LIST.
            ** (2) Otherwise, update ACCESS_HO_LIST and build
            **     OTHER_REPORTED_LIST.
            ** -----------------------------------------------------------------*/
            if ( !mccsa_process_plt_str_meas_rpt(srch_rpt_ptr) )
            {
              access_result = ABORT_HO_LIST;
            }
          }

          else
          {
            MSG_ERROR("SRCH_AHO_LIST_R error: meas %d aset %d",
              srch_rpt_ptr->pilots_meas.pilot_list[0].pilot_pn,
              cur_bs_ptr->pilot_pn, 0);
            access_result = ABORT_HO_LIST;
          }
        }

        else if ( srch_rpt_ptr->hdr.rpt == SRCH_REQUEST_SYS_RESTART_R )
        {
          MSG_HIGH( "SRCH request sys restart", 0, 0, 0 );
          continue_wait = FALSE;
          access_result = ABORT_SRCH_REQUEST_SYS_RESTART;
        }

        /* Unexpected type of Searcher report */
        else
        {
          /* -----------------------------------------------------------
          ** Clear the unexpected type of Searcher report from the queue
          ** Continue wait.
          ** -----------------------------------------------------------*/
          MSG_MED("Ignore Searcher report %d", srch_rpt_ptr->hdr.rpt, 0, 0);
        } /* end if (srch_rpt_ptr->hdr.rpt == SRCH_AHO_PLIST_R) */

        cmd_done (&srch_rpt_ptr->hdr.rpt_hdr);

      } /* end if ((srch_rpt_ptr = q_get(&mcc_srch_q)) != NULL) */

    } /* end while */

  } /* end if ( access_result == NOT_ABORT ) */

  return access_result;

} /* end mccsa_wait_for_srch_pilots_meas_rpt */

#ifdef FEATURE_IS2000_REL_A_CC
/*===========================================================================

FUNCTION MCCSA_CALCULATE_MAXIMUM_REACH_MESSAGE

DESCRIPTION
  This function calculates the max message body for a R-EACH message.

DEPENDENCIES
  None

RETURN VALUE
  The maximum message body in bits.

SIDE EFFECTS
  None.

===========================================================================*/

word mccsa_calculate_maximum_reach_message
(
  void
)
{
  int i;
  word max_message_body;
  caii_en_ac_parm_type *eap_ptr;    /* Pointer to current Enhanced Access Params Msg */

  if ( (eap_ptr = (caii_en_ac_parm_type *) mcc_get_msg( CAI_EN_AC_PARM_MSG ))
       == NULL )
  {
    ERR("No EAPM",0,0,0);
    max_message_body = 0;
  }
  else
  {
    /* Calculate the maximum message duration of all BA mode selections. */
    max_message_body = 0;
    for ( i=0; i <= eap_ptr->num_mode_selection_entries+1; i++ )
    {
      if (eap_ptr->mode_selection[i].access_mode == CAI_EA_MODE_BA)
      {
        if (max_message_body < eap_ptr->mode_selection[i].access_mode_max_duration)
        {
          max_message_body = eap_ptr->mode_selection[i].access_mode_max_duration;
        }
      }
    }

    /* Convert from max message duration to max message length */
    /*lint -e641 */
    if ( ( (eap_ptr->each_ba_rates_supported & (0x80 >> TXC_EACH_RATE_38400_20_V )) != 0 )
         || ( (eap_ptr->each_ba_rates_supported & (0x80 >> TXC_EACH_RATE_38400_10_V )) != 0 )
         || ( (eap_ptr->each_ba_rates_supported & (0x80 >> TXC_EACH_RATE_38400_5_V )) != 0 )
       )
    /*lint +e641 */
    {
      max_message_body *= 38400;
    }

    /*lint -e641 */
    else if ( ( (eap_ptr->each_ba_rates_supported & (0x80 >> TXC_EACH_RATE_19200_20_V )) != 0 )
              || ( (eap_ptr->each_ba_rates_supported & (0x80 >> TXC_EACH_RATE_19200_10_V )) != 0 )
            )
    /*lint +e641 */
    {
      max_message_body *= 19200;
    }

    else
    {
      max_message_body *= 9600;
    }

    // Adjust max message body by the CRC length and twice the message length record
    // to account for extended messages.
    if (max_message_body > CAI_AC_CRC_SIZE + 2*CAI_AC_LGTH_SIZE)
    {
      max_message_body = max_message_body - CAI_AC_CRC_SIZE - (2*CAI_AC_LGTH_SIZE); /*lint !e834 */ 
    }
    else
    {
      // Do not allow unsigned local to become negative (very large)
      max_message_body = 0;
    }

  }

  return max_message_body;

} /* mccsa_calculate_maximum_reach_message */
#endif // FEATURE_IS2000_REL_A_CC

/* ==========================================================================

FUNCTION MCCSA_FIND_SRCH_NSET_SWAP_PLT

DESCRIPTION
  This function find the first pilot in SRCH NSet which is neither in
  ACCESS_HO_LIST or OTHER_REPORTED_LIST.

DEPENDENCIES
  None.

RETURN VALUE
  Index of pilot in SRCH NSet which can be swapped.

SIDE EFFECTS
  None.

===========================================================================*/
int mccsa_find_srch_nset_swap_plt(void)
{
  int i;
  int swap_idx; /* index of pilot in SRCH NSet to be swapped */

  for (swap_idx=0; swap_idx<mcc_srch_buf.nset.nset_cnt; swap_idx++)
  {
    /* -------------------------------------------
    ** Check whether the pilot is in ACCESS_HO_LIST
    ** -------------------------------------------- */
    for (i=0; i<mccsa_access_ho_list.num_pilots; i++)
    {
      if (mcc_srch_buf.nset.nset[swap_idx].pilot ==
          mccsa_access_ho_list.pilot_list[i].pilot_pn)
      {
         break;
      }
    } /* end for ACCESS_HO_LIST */

    /* ------------------------------
    ** Pilot is not in ACCESS_HO_LIST
    ** ------------------------------ */
    if ((mccsa_access_ho_list.num_pilots == 0) ||
        (i == mccsa_access_ho_list.num_pilots))
    {
      /* -------------------------------------------------
      ** Check whether the pilot is in OTHER_REPORTED_LIST
      ** ------------------------------------------------- */
      for (i=0; i<mccsa_other_reported_list.num_pilots; i++)
      {
        if (mcc_srch_buf.nset.nset[swap_idx].pilot ==
            mccsa_other_reported_list.pilot_list[i].pilot_pn)
        {
          break;
        }
      } /* end for OTHER_REPORTED_LIST */

      /* ----------------------------------------
      ** Pilot is also not in OTHER_REPORTED_LIST
      ** ---------------------------------------- */
      if ((mccsa_other_reported_list.num_pilots == 0) ||
          (i == mccsa_other_reported_list.num_pilots))
      {
        break;
      } /* end if pilot is not in OTHER_REPORTED_LIST */
    }  /* end if pilot is not in ACCESS_HO_LIST */

  } /* end for */

  return (swap_idx);

} /* end mccsa_find_srch_nset_swap_plt */

/* ==========================================================================

FUNCTION MCCSA_ACCESS_ATTEMPT_IN_PROGRESS

DESCRIPTION
  This function checks whether access attempt is in progress.

DEPENDENCIES
  None.

RETURN VALUE
  TRUE if access in progress. Otherwise, FALSE.

SIDE EFFECTS
  None.

===========================================================================*/
boolean mccsa_access_attempt_in_progress(void)
{
  return ((mccsa_layer2.access_in_progress) &&
          (MAIN_STATE(cdma.curr_state) == CDMA_SYS_ACC)) ;

} /* end mccsa_access_attempt_is_in_progress */

/*===========================================================================

FUNCTION MCCSA_SET_PREV_PILOT

DESCRIPTION
  This function sets Previous pilot PN and its neighbor record in Access
  Attempt list.

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/
void mccsa_set_prev_pilot_info
(
  cdma_bs_type *old_bs_ptr
)
{
  int att_idx;           /* Index of pilot in access attempted list */
  int nghbr_idx;         /* Index of pilot in neighbor list */

  /* Set Previous pilot PN */
  mccsa_pilot_rpt_ovhd_rec.prev_pilot = (int2) old_bs_ptr->pilot_pn;

  /* Get index of Previous pilot in Access Attempt List */
  att_idx = mccsa_get_acc_att_index((word) mccsa_pilot_rpt_ovhd_rec.prev_pilot);

  /* check the valid ranges of att_idx values */
  if (att_idx == -1)
  {
	  MSG_HIGH ("Previous pilot is in the Acess Attempt List",0,0,0);
	  return;
  }

  /* -------------------------------------------------------------------
  ** It is possible that Previous pilot is not in the neighbor list of
  ** the new Active pilot. So what we can do is to assume Previous pilot
  ** has the same neighbor record info as new Active pilot - which we
  ** can get from neighbor list of Previous pilot. However, if the
  ** Previous pilot is in fact in the neighbor list of new Active pilot,
  ** we will override this assumption when mccsa_update_attempt_list()
  ** is called in send_srch_neighs()
  ** If previous pilot is not found in the access attempt list then return
  ** from this function
  ** ------------------------------------------------------------------- */
  for (nghbr_idx=0; (nghbr_idx < old_bs_ptr->nl.num_nghbr_pns); nghbr_idx++)
  {
    if (old_bs_ptr->nl.nset[nghbr_idx].pilot_pn == cur_bs_ptr->pilot_pn)
    {
      mccsa_set_pilot_nghbr_rec(&mccsa_pilot_rpt_ovhd_rec.attempt_list[att_idx],
        &cur_bs_ptr->nl.nset[nghbr_idx]);
      break;
    }
  }

} /* end mccsa_set_prev_pilot_info */

/*===========================================================================

FUNCTION MCCSA_UPDATE_ACCESS_ATTEMPT_LIST

DESCRIPTION
  This function updates Access Attempt list with info from neighbor list.

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/
void mccsa_update_access_attempt_list(void)
{
  int att_idx;           /* Index of pilot in access attempted list */
  int nghbr_idx;         /* Index of pilot in neighbor list */

  for (att_idx=0; ((att_idx<mccsa_pilot_rpt_ovhd_rec.num_pilots_attempted) && (att_idx < CAI_N13M));
       att_idx++)
  {
    for (nghbr_idx=0; nghbr_idx < cur_bs_ptr->nl.num_nghbr_pns; nghbr_idx++)
    {
      /* ------------------------------------------------------------
      ** If pilot exists in neighbor list, update its neighbor record
      ** in Access Attempt list.
      ** ------------------------------------------------------------ */
      if (cur_bs_ptr->nl.nset[nghbr_idx].pilot_pn ==
          mccsa_pilot_rpt_ovhd_rec.attempt_list[att_idx].pilot_pn)
      {
        mccsa_set_pilot_nghbr_rec(
          &mccsa_pilot_rpt_ovhd_rec.attempt_list[att_idx],
          &cur_bs_ptr->nl.nset[nghbr_idx]);
        break;
      }
    } /* end for */
  } /* end for */

} /* end mccsa_update_access_attempt_list */

/* ==========================================================================

FUNCTION MCCSA_UPDATE_SRCH_NEIGHS

DESCRIPTION
  This function updates the neighbor set which will be sent to Search task
  with info from Access Attempt list.

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/
void mccsa_update_srch_neighs(void)
{
  int n;
  int nset_idx;          /* Index of pilot in neighbor set */
  int att_idx;           /* Index of pilot in access attempt list */
  int old_nset_cnt;     /* Number of pilots in neighbor set before update */
  boolean pilot_in_nset; /* Flag if pilot is in the neighbor set */

  /* Save the count of pilots in neighbor set before update */
  old_nset_cnt = mcc_srch_buf.nset.nset_cnt;

  /* Loop through Access Attempt list */
  for (att_idx=0; att_idx<mccsa_pilot_rpt_ovhd_rec.num_pilots_attempted;
       att_idx++)
  {
    /* Not Active pilot */
    if (mccsa_pilot_rpt_ovhd_rec.attempt_list[att_idx].pilot_pn !=
        cur_bs_ptr->pilot_pn)
    {
      pilot_in_nset = FALSE;

      /* Match against Searcher Neighbor Set */
      for (nset_idx=0; nset_idx < old_nset_cnt; nset_idx++)
      {
        /* ---------------------------------
        ** Pilot is in Searcher Neighbor Set
        ** --------------------------------- */
        if (mccsa_pilot_rpt_ovhd_rec.attempt_list[att_idx].pilot_pn ==
            mcc_srch_buf.nset.nset[nset_idx].pilot)
        {
          /* ---------------------------------------------------------------
          ** Both ACCESS_HO_ALLOWED and ACCESS_ATTEMPTED fields must set to
          ** true so as to allow access handoff and access probe handoff on
          ** this pilot irrespective of what it is in ESPM
          ** -------------------------------------------------------------- */
          mcc_srch_buf.nset.nset[nset_idx].access_ho_allowed = TRUE;
          mcc_srch_buf.nset.nset[nset_idx].access_attempted = TRUE;
          pilot_in_nset = TRUE;
          break;
        }
      } /* end for */

      /* -------------------------------------------------------------------
      ** Pilot is not in neighbor list but has been access attemptted. For
      ** IS-95B standard, MS must report this pilot during pilot
      ** reporting. Also we have to to allow access handoff and access probe
      ** handoff on this pilot. Thus, we should add this pilot to Searcher
      ** Neighbor Set.
      ** ------------------------------------------------------------------- */
      if (!pilot_in_nset)
      {
        /* -----------------------------------------------
        ** Still has space to add more pilots to SRCH NSet
        ** ----------------------------------------------- */
        if (mcc_srch_buf.nset.nset_cnt < SRCH_NSET_MAX)
        {
          /* ----------------------------
          ** Append pilot to neighbor set
          ** ---------------------------- */
          n = mcc_srch_buf.nset.nset_cnt;

          /* Increment the neighbor set count */
          mcc_srch_buf.nset.nset_cnt++;
        }
        /* --------------------------------------------------------------
        ** SRCH NSet already filled up, swap the first pilot in SRCH NSet
        ** which is neither in ACCESS_HO_LIST or OTHER_REPORTED_LIST with
        ** access attempted pilot.
        ** -------------------------------------------------------------- */
        else
        {
          n = mccsa_find_srch_nset_swap_plt();
        }

        mcc_srch_buf.nset.nset[n].pilot =
          (int2) mccsa_pilot_rpt_ovhd_rec.attempt_list[att_idx].pilot_pn;
        mcc_srch_buf.nset.nset[n].pri =
          (byte) mccsa_pilot_rpt_ovhd_rec.attempt_list[att_idx].search_pri;
        mcc_srch_buf.nset.nset[n].freq_incl =
          mccsa_pilot_rpt_ovhd_rec.attempt_list[att_idx].freq_incl;
        mcc_srch_buf.nset.nset[n].band_class =
          (byte) mccsa_pilot_rpt_ovhd_rec.attempt_list[att_idx].band;
        mcc_srch_buf.nset.nset[n].cdma_freq =
          (word) mccsa_pilot_rpt_ovhd_rec.attempt_list[att_idx].freq;
        mcc_srch_buf.nset.nset[n].config =
          mccsa_pilot_rpt_ovhd_rec.attempt_list[att_idx].config;

        /* Set the Access Handoff control parameters */
        mcc_srch_buf.nset.nset[n].access_entry_ho =
          mccsa_pilot_rpt_ovhd_rec.attempt_list[att_idx].access_entry_ho;
        mcc_srch_buf.nset.nset[n].access_ho_allowed = TRUE;
        mcc_srch_buf.nset.nset[n].srch_win_nghbr =
          mccsa_pilot_rpt_ovhd_rec.attempt_list[att_idx].srch_win_nghbr;

        /* Set the Timing offsets of the Neighbor pilot */
        mcc_srch_buf.nset.nset[n].timing_incl =
          mccsa_pilot_rpt_ovhd_rec.attempt_list[att_idx].timing_incl;
        if (mccsa_pilot_rpt_ovhd_rec.attempt_list[att_idx].timing_incl)
        {
          mcc_srch_buf.nset.nset[n].nghbr_tx_offset =
            mccsa_pilot_rpt_ovhd_rec.attempt_list[att_idx].nghbr_tx_offset;
          mcc_srch_buf.nset.nset[n].nghbr_tx_duration =
            mccsa_pilot_rpt_ovhd_rec.attempt_list[att_idx].nghbr_tx_duration;
          mcc_srch_buf.nset.nset[n].nghbr_tx_period =
            mccsa_pilot_rpt_ovhd_rec.attempt_list[att_idx].nghbr_tx_period;
        }

        /* Set access attempted flag */
        mcc_srch_buf.nset.nset[n].access_attempted = TRUE;
      } /* end if (!pilot_in_nset) */
    }
  } /* end for */
} /* end mccsa_update_srch_neighs */

/*===========================================================================

FUNCTION MCCSA_SEND_RELEASE_ORDER_ON_AC

DESCRIPTION
  This function sends a release order to the BS on the access channel, and
  wait for the ACK.

DEPENDENCIES
  None.

RETURN VALUE
  The next state to be processed.

SIDE EFFECTS
  None.

===========================================================================*/
word mccsa_send_release_order_on_ac
(
  word curr_state,
    /* Current state being processed */
  byte ordq_type
    /* release ordq type */
)
{
  MSG_LOW("Initializing release order",0,0,0);
  cdma.sa_resp_msg = CAI_AC_ORD_MSG;
  cdma.sa_resp_ord.ordq.order = CAI_RELEASE_ORD;
  cdma.sa_resp_ord.ordq.ordq = ordq_type;

  /* Events report: Release */
  mclog_report_release(MCLOG_RELEASE_MS_ORDER);

  return (mccsa_send_order_response_msg(curr_state, FALSE));

} /* mccsa_send_release_order_on_ac() */

/*===========================================================================

FUNCTION MCCSA_ACCESS_HO_IS_ALLOWED

DESCRIPTION
  This function validates if the Mobile is allowed for Access Handoff and
  if the ACCESS_HO_LIST contains more than one pilot in the list.

DEPENDENCIES
  Depends on the P_REV_IN_USE (atleast 4) and Access Handoff Feature allowed.

RETURN VALUE
  boolean (TRUE/FALSE)

SIDE EFFECTS
  None.

===========================================================================*/
boolean mccsa_access_ho_is_allowed()
{
  boolean status = FALSE;

  if (P_REV_IS_4_OR_GREATER && cur_bs_ptr->csp.esp.access_ho && 
      (mccsa_access_ho_list.num_pilots > 1))
  {
    status = TRUE;
  }

  return (status);

} /* mccsa_access_ho_is_allowed */

/*===========================================================================

FUNCTION MCCSA_IS_IN_ACCESS_HO_LIST

DESCRIPTION
  This function validates if the Mobile is allowed for Access Handoff and
  if it is in the ACCESS_HO_LIST

DEPENDENCIES
  Depends on the P_REV_IN_USE (atleast 4) and Access Handoff Feature allowed.

RETURN VALUE
  boolean (TRUE/FALSE)

SIDE EFFECTS
  None.

===========================================================================*/
boolean mccsa_is_in_access_ho_list(word pilot_pn)
{
  boolean status = FALSE;
  int i;  /* Index in to the ACCESS_HO_LIST */

  for(i=0; i<mccsa_access_ho_list.num_pilots; i++)
  {
    if (pilot_pn == mccsa_access_ho_list.pilot_list[i].pilot_pn)
    {
      status = TRUE;
      break;
    }
  }
  return (status);

} /* mccsa_is_in_access_ho_list */

/*===========================================================================

FUNCTION MCCSA_TX_STOP

DESCRIPTION
  This function commands tx to stop transmitting accesses but keep the
  transmit clock running while an System Access Attempt is in progress.

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/

void mccsa_tx_stop( void )
{
  /* Command TXC to stop accesses */
  mcc_tx_buf.hdr.command = TXC_ACC_STOP_F;

  /* Signal when access attempt is finished */
  mcc_tx_buf.stop.tcb_ptr = &mc_tcb;
  mcc_tx_buf.stop.sigs    = MCC_TXC_DONE_SIG;

  /* Fill in necessary fields of command block */
  mcc_tx_buf.hdr.cmd_hdr.done_q_ptr = NULL;
  mcc_tx_buf.hdr.cmd_hdr.task_ptr   = &mc_tcb;
  mcc_tx_buf.hdr.cmd_hdr.sigs       = MCC_CMD_SIG;

  /* Clear both signals before txc_cmd() is called, because
   * both signals could be set by TXC before MC runs again.
   */
  (void) rex_clr_sigs( &mc_tcb, MCC_CMD_SIG );
  (void) rex_clr_sigs( &mc_tcb, MCC_TXC_PROBE_SIG );
  (void) rex_clr_sigs( &mc_tcb, MCC_TXC_DONE_SIG );
  txc_cmd( &mcc_tx_buf );

  /* Wait for command to be pulled out of TXC command queue */
  (void) mcc_wait( (dword) MCC_CMD_SIG );
  (void) rex_clr_sigs( &mc_tcb,  MCC_CMD_SIG );

  if (mcc_tx_buf.hdr.status == TXC_DONE_S)
  {
    /* Wait for access attempt to complete */
    (void) mcc_wait( (dword) MCC_TXC_DONE_SIG );
    (void) rex_clr_sigs( &mc_tcb, MCC_TXC_DONE_SIG );
  }
  else
  {
    /* Race condition -- access attempt already completed */
    MSG_HIGH( "Status = %d", mcc_tx_buf.hdr.status, 0, 0 );
  }

  mccsa_layer2.access_in_progress = FALSE;

} /* end mccsa_tx_stop() */

/*===========================================================================

FUNCTION MCCSA_HS_PROC

DESCRIPTION
  This function processes handset commands while an access attempt
  is in progress.

DEPENDENCIES
  None.

RETURN VALUE
  The next state to be processed.  If it is not necessary to change
  the current state as a result of the message received from the
  handset, the return state will be equal to the current state.

SIDE EFFECTS
  None.

===========================================================================*/

static word mccsa_hs_proc
(
  mc_msg_type *cmd_ptr,
    /* pointer to received handset command */
  word curr_state
    /* sub-state of System access currently being processed */
)
{
  word next_state = curr_state;
  boolean stop_access = FALSE;
  uint8 call_id_index;
  srch_access_type_enum access_type;

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

  cmd_ptr->hdr.status = MC_DONE_S;          /* Default, unless bad */

  switch (cmd_ptr->hdr.cmd)
  {
    case MC_END_F:
      if (curr_state == CDMA_ORIG)   /*lint !e641 */
      {
        MSG_HIGH( "Processing MC_END_F cmd", 0, 0, 0 );
        for (call_id_index=0; call_id_index<cmd_ptr->end.num_call_ids; 
             call_id_index++) 
        {
          if (cmd_ptr->end.call_ids[call_id_index] == cdma.call_id) 
          {
            /* ---------------------------------------------------------
            ** Cancel origination attempt and enter System Determination
            ** --------------------------------------------------------- */
            cdma.rel_orig = TRUE;
            if ( P_REV_IS_4_OR_GREATER )
            {
              mccsa_release_cause = CAI_REL_NORMAL;
              /*if release already in process, ignor new END key*/
              /* mcc_is_aho_ovhd_msg_okay() is false during Access Handoff */
              if ((mccsa_orig_ministate != CDMA_ORIG_RELS) &&
                  mcc_is_aho_ovhd_msg_okay()
                  && (mccsa_apho_substate == APHO_NOT_IN_PROGRESS)
                 )
              {
                #ifdef FEATURE_AUTH
                /* If we came here while waiting for auth signature, then we have
                   not begun the origination yet and so exit to IDLE state without
                   sending a release */
                if ( mccsa_gen_auth_signature_in_progress )
                {
                  mcc_exit.exit = MCC_RELEASE;
                  next_state    = CDMA_EXIT;
                  MSG_HIGH (" Recd an END while waiting for Auth signature ",0,0,0);
                }
                else
                #endif /* FEATURE_AUTH */
                {
                  mccsa_tx_stop();
                  mccsa_layer3_msg_pending = FALSE;
                  next_state = mccsa_send_release_order_on_ac(curr_state,
                    mccsa_release_cause);
                }
              }
              mccsa_orig_ministate = CDMA_ORIG_RELS;
              MSG_HIGH("Enter release substate",0,0,0);
            }
            else
            {
              /*
               * The P_REV is less than 4, we can not send release order
               * on the Access Channel, enter System Determination with
               * a release indication
               */
              stop_access   = TRUE;
              mcc_exit.exit = MCC_RELEASE;
              next_state    = CDMA_EXIT;
            }

            /* Events report: Call release request */
            event_report(EVENT_CALL_RELEASE_REQUEST);

            /* Only single origination is supported in System Access. As soon
               a match on call_id is identified, processing is done. */
            break;
          }
          else 
          {
            MSG_ERROR("Call_id %d from CM mismatched with current call!", 
                      cmd_ptr->end.call_ids[call_id_index], 0, 0);
            cmd_ptr->hdr.status = MC_BADPARM_S;
          } /* if (cmd_ptr->end.call_ids[call_id_index] == cdma.call_id) */
        } /* for (call_id_index) */
      } 
      else
      {
        MSG_HIGH( "Invalid UI cmd %d", cmd_ptr->hdr.cmd, 0,0 );
      }
      break;

    case MC_ORIGINATION_F:

      /* allow origination while registration is still in progress*/
      if ( curr_state == CDMA_REG ) /*lint !e641 */
      {
        /* Registration Stop  */
        MSG_HIGH( "REG preempted by ORIG", 0, 0, 0 );
        cmd_ptr->hdr.status = MC_DONE_S;

        /* Report that this Origination Message is causing us to 
           abandon a Registration Message. */
        mclog_report_registration_suppressed(cdma.reg_type);

        /* the following OTASP block is added to set OTASP call parameters */

        #ifdef FEATURE_OTASP
        /* ----------------------------------------
        ** Special processing for OTASP origination
        ** ---------------------------------------- */
        if (cmd_ptr->origination.otasp_activation)
        {
          if (!otasp_is_otasp_allowed())
          {
            cmd_ptr->hdr.status = MC_BADCMD_S;
            mcc_send_orig_failure(CM_CALL_ORIG_ERR_ABORT,
                                  cmd_ptr->origination.call_id,
                  	          CM_CALL_ORIG_ERR_LAYER_L3
                        	  );
            cdma.call_orig = FALSE;
            break;
	  }		

          cdma.otasp_call = USER_INIT_SRV_PROV;

          #ifdef FEATURE_UIM_RUIM
          #if defined( FEATURE_UIM_RUN_TIME_ENABLE )
          /* Determine if the R-UIM is available for this NAM */
          if (nv_rtre_use_ruim_for_nam(cdma.curr_nam))
          #endif /* FEATURE_UIM_RUN_TIME_ENABLE */
          {
            /* Set the NAM lock state in the R-UIM */
            otasp_set_ruim_nam_lock_state(USER_INIT_SRV_PROV);
          }
          #endif /* FEATURE_UIM_RUIM */

        }
        #endif /* FEATURE_OTASP */

        /* -------------------------
        ** User is initiating a call
        ** ------------------------- */
        mcc_ms_orig( &cmd_ptr->origination );

        /* Then origination */
        next_state = CDMA_ORIG;  /*lint !e641 */

        /* Reset layer 2 pending message flag */
        mccsa_layer2.pending_msg_flag = FALSE;
        mccsa_tx_stop();

        #if (defined (FEATURE_HDR_HANDOFF) && defined(FEATURE_IS2000_REL_A))
#error code not present
        #endif /* FEATURE_HDR_HANDOFF && FEATURE_IS2000_REL_A */

        /* -------------------------------------------------------------
        ** Since the mobile wants to do origination after entering access
        ** state for registration, we need to send Search the 
        ** Access command again, with high performance access type so 
        ** the RX Diversity is enabled if possible.
        ** ------------------------------------------------------------*/
        if ( mcc_wait_transceiver_resource(SRCH_HIGH_PERF_ACCESS,
             SRCH_ACCESS_MOBILE_ORIG_IMMED_TYPE) == MCC_TRANSCEIVER_RESOURCE_LOST )
        {
          /* Searcher requested system restart. Need to do system exit. */
          mcc_exit.exit = MCC_SRCH_REQUEST_SYS_RESTART;
          next_state = CDMA_EXIT;
        }

        break;
      }

      #if (defined(FEATURE_CDSMS) || defined (FEATURE_DBM))
      /* -----------------------------------------------------------
      ** If we are in CDMA_MSG and we receive ORIG, we shall abort
      ** the SMS or PD attempt and do origination instead.
      ** ----------------------------------------------------------- */
      if ( ( curr_state == CDMA_MSG ) && /*lint !e641 */
           ( cdma.sa_msg_tx_msg == CAI_AC_BURST_MSG )  /*lint !e641 */
         )
      {
        /* Reject MO DBM first */
        MSG_HIGH( "Rejecting MO DBM request in SA", 0, 0, 0 );
        mcc_check_mo_dbm_status(CM_DBM_MO_ABORT);
        cmd_ptr->hdr.status = MC_DONE_S;

        /* Then origination */
        mcc_ms_orig(&cmd_ptr->origination);
        next_state = CDMA_ORIG; /*lint !e641 */

        /* Reset layer 2 pending message flag */
        mccsa_layer2.pending_msg_flag = FALSE;
        mccsa_tx_stop();

        #if (defined (FEATURE_HDR_HANDOFF) && defined(FEATURE_IS2000_REL_A))
#error code not present
        #endif /* FEATURE_HDR_HANDOFF && FEATURE_IS2000_REL_A */
      }
      else
      {
        MSG_HIGH( "Invalid UI cmd %d", cmd_ptr->hdr.cmd, 0,0 );
        cmd_ptr->hdr.status = MC_BADCMD_S;
        /* ----------------------------
        ** Inform CM origination failed
        ** ---------------------------- */
        #ifndef FEATURE_IS2000_REL_A_SVD
        if ( curr_state == CDMA_ORIG )
        {
          mcc_send_orig_failure(CM_CALL_ORIG_ERR_ABORT,
                                cmd_ptr->origination.call_id,
                                mccsa_orig_fail_layer
                               );
        }
        else
        #endif /* !FEATURE_IS2000_REL_A_SVD */
        {
          mcc_send_orig_failure(CM_CALL_ORIG_ACC_IN_PROG,
                                cmd_ptr->origination.call_id,
                                mccsa_orig_fail_layer
                               );
        }

        if (curr_state != CDMA_ORIG) /*lint !e641 */
        {
          /* Reset call_orig flag only if this is the first call */
          cdma.call_orig = FALSE;
        }
      }

      #else /* defined(FEATURE_CDSMS) || defined (FEATURE_DBM) */

      MSG_HIGH( "Invalid UI cmd %d", cmd_ptr->hdr.cmd, 0,0 );
      cmd_ptr->hdr.status = MC_BADCMD_S;

      /* ----------------------------
      ** Inform CM origination failed
      ** ---------------------------- */
      #ifndef FEATURE_IS2000_REL_A_SVD
      if (curr_state == CDMA_ORIG)
      {
        mcc_send_orig_failure(CM_CALL_ORIG_ERR_ABORT,
                              cmd_ptr->origination.call_id,
                              mccsa_orig_fail_layer
                             );
      }
      else
      #endif /* !FEATURE_IS2000_REL_A_SVD */
      {
        mcc_send_orig_failure(CM_CALL_ORIG_ACC_IN_PROG,
                              cmd_ptr->origination.call_id,
                              mccsa_orig_fail_layer
                             );
      }

      if (curr_state != CDMA_ORIG)
      {
        /* Reset call_orig flag only if this is the first call */
        cdma.call_orig = FALSE;
      }
      #endif /* FEATURE_CDSMS || FEATURE_DBM */

      if (next_state == CDMA_ORIG) /*lint !e641 */
      {
        /* -------------------------------------------------------------
        ** Since the mobile wants to do origination after entering access
        ** state for registration or MO-DBM, we need to send Search the 
        ** Access command again, with high performance access type so 
        ** the RX Diversity is enabled if possible.
        ** ------------------------------------------------------------*/
        #ifdef FEATURE_IS2000_REL_A
        if ( cdma.is_emergency ) 
        {                
          access_type = SRCH_ACCESS_MOBILE_ORIG_IMMED_TYPE;
        }
        else
        #endif /* FEATURE_IS2000_REL_A */

        {
          access_type = SRCH_ACCESS_MOBILE_ORIG_TYPE;
        }
  
        if ( mcc_wait_transceiver_resource(SRCH_HIGH_PERF_ACCESS, 
             access_type) == MCC_TRANSCEIVER_RESOURCE_LOST )
        {
          /* Searcher requested system restart. Need to do system exit. */
          mcc_exit.exit = MCC_SRCH_REQUEST_SYS_RESTART;
          next_state = CDMA_EXIT;
        } 
      }

      break;

    #ifdef FEATURE_UMTS_1X_HANDOVER_1XMSM
#error code not present
    #endif /* FEATURE_UMTS_1X_HANDOVER_1XMSM */

    #if (defined(FEATURE_CDSMS) || defined (FEATURE_DBM))
    case MC_MO_DBM_F:
      /* If an MO DBM request is received while we are in SA, we reject most of the
         requests except the following cases. */
      MSG_MED("DBM rxed in SA", 0, 0, 0);

      #ifdef FEATURE_SO_8026
#error code not present
      #else /* FEATURE_SO_8026 */

      /* MS may transmit DBM in Page Response Substate if MS has 
         received confirmation of delivery of the Page Response
         Message or a Reconnect Message, and ACCT is not enabled 
         for the services number associated with the DBM. */
      if (curr_state == CDMA_PAGE_RES)  /*lint !e641 */
      {
        MSG_MED("DBM rxed in Page Response Substate", 0, 0, 0);

        if ( ( ( mcc_tx_int_msg.page_resp.msg_type == CAI_PAGE_RESP_MSG )
               #ifdef FEATURE_IS2000_REL_C
               || ( mcc_tx_int_msg.rcnm.msg_type == CAI_AC_RECONNECT_MSG )  
               #endif /* FEATURE_IS2000_REL_C */
             ) 
             && !mccsa_layer2.access_in_progress
             #ifdef FEATURE_AUTH
             && !mccsa_gen_auth_signature_in_progress  
                  /* we are not waiting for auth signature */
             #endif /* FEATURE_AUTH */
           )
        {

          MSG_MED("Delivery of PRM/RCNM confirmed", 0, 0, 0);


          /* ----------------------------------------------------------
          ** We've received a mobile orig DBM
          ** Since L2 ACK is always required for SDB delivered on
          ** Access Channel, set this flag
          ** ---------------------------------------------------------*/
          cdma.dbm_wait_for_l2_ack = TRUE;

          if( cmd_ptr->mo_dbm.dbm_buf_ptr != NULL )
          {
            cdma.mo_dbm_type = cmd_ptr->mo_dbm.burst_type;
            cdma.dbm_buf_ptr = cmd_ptr->mo_dbm.dbm_buf_ptr;
          }
          else
          {
            ERR_FATAL( "NULL DBM pointer", 0, 0, 0);  
          }

          /* user is initiating a data burst transmit */
          mcc_ms_sms(cmd_ptr);

          /* Send the MO DBM message */
          next_state = mccsa_send_data_burst( next_state );

        }
        else 
        {
          MSG_MED("Delivery of PRM/RCNM has not confirmed", 0, 0, 0);

          mcc_send_mo_dbm_status(cmd_ptr->mo_dbm.burst_type,
            CM_DBM_MO_HOLD_ORIG);

        }
      }
      else 
      {
        mcc_send_mo_dbm_status(cmd_ptr->mo_dbm.burst_type,
          CM_DBM_MO_HOLD_ORIG);

      }
      #endif /* FEATURE_SO_8026 */

      cmd_ptr->hdr.status = MC_DONE_S;
      break;
      #endif /* FEATURE_CDSMS || FEATURE_DBM */

    #ifdef FEATURE_BROADCAST_SMS
    case  MC_BCSMS_ENABLE_F:
      MSG_MED("Rxed BCSMS_ENABLE Cmd %d in SA",cmd_ptr->hdr.cmd,0,0);
      mccbcsms_enable(cmd_ptr->bcsms_enable.enable);
      break;
    #endif /* FEATURE_BROADCAST_SMS */

    case MC_CDMA_PROT_DEACT_CMD_F:
      /* First save the MMoC's transaction Id.
      */
      mcc_exit.mmoc_trans_id = cmd_ptr->deact_prot.info.trans_id;

      if ( cmd_ptr->deact_prot.info.reason == PROT_DEACT_PWR_DOWN )
      {
        /* Do the processing for power down deactivation.
        */
        mccsa_release_cause = CAI_REL_PWDN;
        if (curr_state == CDMA_ORIG)  /*lint !e641 */
        {
          if (mcc_is_aho_ovhd_msg_okay()
              && (mccsa_apho_substate == APHO_NOT_IN_PROGRESS)
             )
          {
            mccsa_tx_stop();
            mccsa_layer3_msg_pending = FALSE;
            next_state = mccsa_send_release_order_on_ac(curr_state,
              mccsa_release_cause);
          }
          mccsa_orig_ministate = CDMA_ORIG_RELS;
          break;
        }
        /* If we are not in the ORIG substate, we need to exit now */
      } /* if ( cmd_ptr->deact_prot.info.reason ) */
      stop_access = TRUE;
      mcc_exit.exit = mc_map_deact_to_mcc_exit_reason
                      (
                        cmd_ptr->deact_prot.info.reason
                      );
      next_state = CDMA_EXIT;
      cmd_ptr->hdr.status = MC_DONE_S;
      break;

    case MC_CDMA_PROT_GEN_CMD_F:
      /* Process the MMoC's generic protocol command.
      */
      if (!mcc_continue_on_curr_sys
           (
             MEAS_BACK_ALLOWED,
             mmoc_proc_prot_gen_cmd( FALSE,
                                     TRUE,
                                     &cmd_ptr->gen_cmd.info
                                   )
           )
         )
      {
         mcc_exit.exit = mc_map_gen_cmd_to_mcc_exit_reason
                         (
                           &cmd_ptr->gen_cmd.info
                         );
         next_state    = CDMA_EXIT;
         stop_access   = TRUE;
      }
      cmd_ptr->hdr.status = MC_DONE_S;
      break;

    #ifdef FEATURE_MVS_MIGRATE
#error code not present
    #endif /* FEATURE_MVS_MIGRATE */

    default:
      ERR( "Invalid UI cmd %d", cmd_ptr->hdr.cmd, 0, 0 );
      cmd_ptr->hdr.status = MC_BADPARM_S;
      break;
  } /* end switch (cmd_ptr->hdr.cmd) */

  if (stop_access)
  {
    /* Tell TXC to stop the access */
    mccsa_tx_stop();
  }

  cmd_done( &cmd_ptr->hdr.cmd_hdr );

  return (next_state);

} /* mccsa_hs_proc */

/*===========================================================================

FUNCTION MCCSA_LAYER2_ACK_PROCESSING

DESCRIPTION
  This function processes the VALID_ACK and ACK_SEQ layer 2 fields of a
  Paging Channel Message to see if the message contains an acknowledgement
  to the message currently being sent in the access attempt.
  This function also does registration processing.

DEPENDENCIES
  None

RETURN VALUE
  TRUE if a layer 2 ack is found, FALSE otherwise

SIDE EFFECTS
  Stops the access attempt in progress if a valid acknowledgement is found.

===========================================================================*/

boolean mccsa_layer2_ack_processing
(
  mccdma_matched_msg_type *matched_msg_ptr,
    /* Pointer to paging channel message to process layer 2 fields of */
  word curr_state
    /* Current substate being processed */
)
{
  boolean ack_found = FALSE;
    /* Flag that this message contains a valid layer 2 ack */

  #ifdef FEATURE_AUTH
  auth_cmd_type *auth_cmd_ptr;
    /* Pointer to authentication command */
  #endif /* FEATURE_AUTH */ 

  #ifdef FEATURE_PASSPORT_MODE
  cm_mc_rpt_type *cmd_ptr;
    /* Pointer to call manager report */
  #endif /* FEATURE_PASSPORT_MODE */

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  /*************************
   * Check for layer 2 ack *
   *************************/

  if ( ( (matched_msg_ptr->gen_pc.msg_type == CAI_CHN_ASN_MSG)
         || (matched_msg_ptr->gen_pc.msg_type == CAI_EXT_CHN_ASN_MSG)
         #ifdef FEATURE_MEID_SUPPORT
         || (matched_msg_ptr->gen_pc.msg_type == CAI_MEID_EXT_CHN_ASN_MSG)
         #endif /* FEATURE_MEID_SUPPORT */
       ) 
       && ( (curr_state == CDMA_PAGE_RES) /*lint !e641 */
            || ( (curr_state == CDMA_ORIG) /*lint !e641 */ 
                  && (mccsa_orig_ministate != CDMA_ORIG_RELS) /*lint !e641*/
               )
          ) 
       && !mcc_detect_duplicate_ecam(&matched_msg_ptr->gen_pc.hdr)
     )
  {

    #ifdef FEATURE_IS2000_REL_D
#error code not present
    #endif /* FEATURE_IS2000_REL_D */

    /***************************************************************************
    **                          CAM or non-DCA ECAM                           **
    ****************************************************************************/
    {

      #ifdef FEATURE_IS2000_REL_D
#error code not present
      #endif /* FEATURE_IS2000_REL_D */

      {
        /* ----------------------------------------------------------
        ** Ignore the VALID_ACK and ACK_SEQ fields in case of Channel
        ** Assignment Message or Extended Channel Assignment Message.
        ** Treated it as L2 ACK and implicit registration.
        ** ---------------------------------------------------------- */
        ack_found = TRUE;

        /* ---------------------------------------------------------------
        ** Force the received msg ack_seq to the last_reg_msg_seq, so that
        ** mccreg_success() is called below to update the REG parms.
        ** --------------------------------------------------------------- */
        matched_msg_ptr->gen_pc.hdr.ack_seq = mccsa_layer2.last_reg_msg_seq;
      }
    }

    #if defined(FEATURE_CDSMS) || defined(FEATURE_DBM)
    /* ----------------------------------------------------------------------
    ** If MS has sent a MO_DBM in Page Response Substate and has not received
    ** confirmation of the delivery, tell CM to transmit it later.
    ** ---------------------------------------------------------------------- */
    if ((curr_state == CDMA_PAGE_RES) &&  /*lint !e641 */
        (mcc_tx_int_msg.ac_burst.msg_type == CAI_AC_BURST_MSG)) 
    {
      if ((matched_msg_ptr->gen_pc.hdr.valid_ack) &&
          (matched_msg_ptr->gen_pc.hdr.ack_seq == mcc_layer2.msg_seq))
      {
        /* Notify SMS task that the layer2 ack is received */
        MSG_LOW("L2_ack_proc call mcc_check_mo_dbm_status", 0, 0, 0);
        mcc_check_mo_dbm_status( CM_DBM_MO_OK );
      }
      else
      {
        /* Notify CM task to resend the DBM later */
        MSG_LOW("L2_ack_proc call mcc_send_mo_dbm_status", 0, 0, 0);
        mcc_send_mo_dbm_status(mcc_tx_int_msg.ac_burst.burst_type, CM_DBM_MO_HOLD_ORIG);
      }
    }
    #endif /* FEATURE_CDSMS || FEATURE_DBM */

  }

  else if ((matched_msg_ptr->gen_pc.hdr.valid_ack) &&
           (matched_msg_ptr->gen_pc.hdr.ack_seq == mcc_layer2.msg_seq))
  {

    #if (defined(FEATURE_CDSMS) || defined (FEATURE_DBM))
    if ( (curr_state == CDMA_MSG) && /*lint !e641 */
         ( cdma.sa_msg_tx_msg == CAI_AC_BURST_MSG )  /*lint !e641 */
       )
    {
      /* notify SMS task that the layer2 ack is received */
      MSG_LOW("L2_ack_proc call mcc_check_mo_dbm_status", 0, 0, 0);
      mcc_check_mo_dbm_status( CM_DBM_MO_OK );
    }
    #endif /* FEATURE_CDSMS || FEATURE_DBM */

    #ifdef FEATURE_IS2000_REL_D
#error code not present
    #endif /* FEATURE_IS2000_REL_D */

    /* ---------------------------------
    ** Layer 2 acknowledgement received.
    ** --------------------------------- */
    ack_found = TRUE;
  }

  if (ack_found)
  {

    /* ------------------------------------
    ** Stop any access attempt in progress.
    ** ------------------------------------ */
    mccsa_tx_stop();

    /***********************************
     * Do registration processing.     *
     * See IS-95A section 6.6.5.5.3.1. *
     ***********************************/

    /* -----------------------------------------------------------------
    ** Compare ack_seq with the sequence number of the last registration
    ** message or implicit registration we sent.  If they match this is
    ** an ack to a registration.
    ** ----------------------------------------------------------------- */

    if (matched_msg_ptr->gen_pc.hdr.ack_seq == mccsa_layer2.last_reg_msg_seq)
    {
      mccreg_success();

      /* Events report: Registration performed */
      if (curr_state == CDMA_REG)  /*lint !e641 */
      {
        event_report_payload(EVENT_REGISTRATION_PERFORMED,
          sizeof(event_registration_performed_type), &cdma.reg_type);
      }

      #ifdef FEATURE_PASSPORT_MODE
      if(mcc_passport_mode_is_jcdma())
      {
        
        if ((cmd_ptr = cm_mc_rpt_get_buf_else_err_fatal()) != NULL)
        {
          cmd_ptr->hdr.cmd = CM_REG_SUCCESS_F;
  
          /* Send command to CM */
          MSG_HIGH("CM Registration Success", 0, 0, 0);
          mcc_cm_mc_rpt(cmd_ptr);
        }
        else
        {
          ERR("No free buffers on cm_mc_rpt_free_q",0,0,0 );
        }
      }
      #endif /* FEATURE_PASSPORT_MODE */

    }
    else
    {
      /* Failed access - perform registration processing */
      mccreg_idle_ack_or_fail();
    }

    /***************************************
     * Reset state timer if in Origination *
     * or Page Response Substates.         *
     ***************************************/

    if (((curr_state == CDMA_ORIG) /*lint !e641 */
         && (mccsa_orig_ministate != CDMA_ORIG_RELS)
        ) || (curr_state == CDMA_PAGE_RES)) /*lint !e641 */
    {
      /* ---------------------------------------------------------
      ** This is an ack to a page response or origination message.
      ** Set state timer and await further instructions.
      ** --------------------------------------------------------- */

      /* Reset the T42M timer */
      (void) rex_set_timer( &mcc_state_timer, cai_tmo.t42m );
    }

    #ifdef FEATURE_AUTH
    if ((mccsa_layer2.last_msg_type == CAI_AC_ORD_MSG) &&
        (cdma.sa_resp_ord.gen.order == CAI_BS_CHAL_ORD))
    {
      /* --------------------------------------------------
      ** This is the ack to a Base Station Challenge Order.
      ** Tell auth to set timer in case the Base Station
      ** Challenge Confirmation Order doesn't come in T64m
      ** -------------------------------------------------- */
      if ((auth_cmd_ptr = (auth_cmd_type*) q_get( &auth_free_q )) != NULL)
      {
        auth_cmd_ptr->hdr.command = AUTH_START_TIMER_F;
        auth_cmd_ptr->hdr.cmd_hdr.task_ptr = NULL;
        auth_cmd( auth_cmd_ptr );
      }
      else
      {
        ERR( "No free buffers on auth_free_q", 0, 0, 0 );
      }
    }

    #if ((defined FEATURE_IS2000_REL_C_AKA) && (defined FEATURE_IS2000_REL_D))
#error code not present
    #endif /* FEATURE_IS2000_REL_C_AKA && FEATURE_IS2000_REL_D */

    #endif /* FEATURE_AUTH */

  } /* end if (ack_found) */

  return (ack_found);

} /* end mccsa_layer2_ack_processing() */

/*===========================================================================

FUNCTION MCCSA_PAGE_MATCH

DESCRIPTION
  This function matches pages received during an access attempt.

DEPENDENCIES
  None

RETURN VALUE
  The next state to be processed.

SIDE EFFECTS
  None

===========================================================================*/
word mccsa_page_match
(
  mccrxtx_msg_type *msg_ptr,
    /* Pointer to received message */
  word curr_state
    /* Substate of System Access currently being processed */
)
{
  word  next_state = curr_state;
    /* Next state to be processed */
  mcc_page_info_type page_info;
    /* Information from a page which was addressed to the mobile */
  #ifdef T_W1
#error code not present
  #endif /* T_W1 */
  mccdma_page_match_result_type page_match;
    /* Page match result */
  srch_access_type_enum access_type;

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  /* ------------------------------------------------------------------
  ** mcc_parse_page returns NO_MATCH if none of the pages are addressed
  ** to the mobile, or if the page is a duplicate. Otherwise it
  ** returns information from the page in page_info.
  ** ------------------------------------------------------------------ */
  page_match = mcc_parse_page(next_state, FALSE, msg_ptr, &page_info);

  if (page_match != NO_MATCH)
  {
    /* -------------------------------------
    ** Increment the count PAG_3 or FCCCH_3.
    ** ------------------------------------- */
    mcc_increment_pag_3_or_fccch_3();

    /* Log the matched page - type of page_match corresponds to
    ** mclog_match_type for now
    */
    mclog_report_matched_msg(msg_ptr->msg.gen.msg_type,
                             (mclog_match_type) page_match);
  }

  #ifdef FEATURE_IS2000_REL_A_CC
  else if (msg_ptr->msg.gen.msg_type == CAI_UPM_FINAL_SEG_MSG)
  {
    mclog_report_event_seg_upm_addr_mismatch(MCLOG_FULL_ADDR_MISMATCH, /*lint !e641 */
      mccdma_seg_univ_page.segment_seq);
  }
  #endif /* FEATURE_IS2000_REL_A_CC */

  if (page_match == PAGE_MATCH)
  {
    next_state = CDMA_PAGE_RES;  /*lint !e641 */
    cdma.call_orig = FALSE;  /* Call is not mobile originated */
    mcc_validate_so_req( &page_info );

    #ifdef T_W1
#error code not present
    #endif /* T_W1 */

    MSG_HIGH( "Set cdma.so_req to 0x%x in SA state",  cdma.so_req, 0, 0 );

    /* Update Layer2 ack info for Page Response Message */
    mcc_set_layer2_ack(page_info.msg_seq, page_info.ack_type, TRUE);

    /* If the page is for an SMS, it is not an urgent page. */
    if (CAI_IS_SO_SMS(page_info.service_option))
    {
      access_type = SRCH_ACCESS_MOBILE_TERM_TYPE;
    }
    else
    {
      access_type = SRCH_ACCESS_MOBILE_TERM_IMMED_TYPE;
    }
    
    if ( mcc_wait_transceiver_resource(SRCH_HIGH_PERF_ACCESS, access_type) ==
         MCC_TRANSCEIVER_RESOURCE_LOST ) 
    {
      /* Searcher requested system restart. Need to do system exit. */
      mcc_exit.exit = MCC_SRCH_REQUEST_SYS_RESTART;
      next_state = CDMA_EXIT;
    }

  } /* end if (there was a page for us) */

  #ifdef FEATURE_IS2000_REL_A_CC
  if (OVHD_CHAN_IS_BCCH &&
      ((msg_ptr->msg.gen.msg_type == CAI_UPM_FIRST_SEG_MSG) ||
       (msg_ptr->msg.gen.msg_type == CAI_UPM_MIDDLE_SEG_MSG) ||
       (msg_ptr->msg.gen.msg_type == CAI_UPM_FINAL_SEG_MSG)))
  {
    /* ------------------------------------------------------------------
    ** Page match done, empty segmented Universal Page Message reassembly
    ** buffer.
    ** ------------------------------------------------------------------ */
    mcc_empty_seg_upm_buffer();
  }
  #endif /* FEATURE_IS2000_REL_A_CC */

  return (next_state);

} /* mccsa_page_match */

/*===========================================================================

FUNCTION MCCSA_SEND_REJECT

DESCRIPTION
  This function sets up the reject order to be sent and calls
  send_subsequent_msg() to transmit the reject order.

DEPENDENCIES
  None

RETURN VALUE
  Next state to be processed.

SIDE EFFECTS
  None

===========================================================================*/

word mccsa_send_reject
(
  word curr_state,
    /* Current substate */
  byte reject_reason,
    /* Reason for rejecting the message */
  mccdma_matched_msg_type *matched_msg_ptr
    /* Pointer to message we are rejecting */
)
{
  word next_state = curr_state;
    /* Next state to be processed */

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  cdma.sa_resp_msg                      = CAI_AC_ORD_MSG;
  cdma.sa_resp_ord.rej.order            = CAI_MS_REJECT_ORD;
  cdma.sa_resp_ord.rej.ordq             = reject_reason;
  cdma.sa_resp_ord.rej.rej_msg_type     = matched_msg_ptr->gen_pc.msg_type;
  cdma.sa_resp_ord.rej.rejected_pdu_type     = 0;//20ms regular pdu type

  if (matched_msg_ptr->gen_pc.msg_type == CAI_PC_ORD_MSG)
  {
    cdma.sa_resp_ord.rej_ord.rej_order  = matched_msg_ptr->pc_ord.order;
    cdma.sa_resp_ord.rej_ord.rej_ordq   = matched_msg_ptr->pc_ord.ordq;
  }

  next_state = mccsa_send_order_response_msg( curr_state, FALSE );

  return (next_state);

} /* mccsa_send_reject */

/*===========================================================================

FUNCTION MCCSA_PROCESS_CHAN_ASSIGN_MSG

DESCRIPTION
  This function processes Channel Assignment & Extended Channel Assignment
  Message.

DEPENDENCIES
  None

RETURN VALUE
  The next state to be processed.

SIDE EFFECTS
  None

===========================================================================*/

word mccsa_process_chan_assign_msg
(
  mccdma_matched_msg_type *matched_msg_ptr,
    /* Pointer to matched (Extended) Channel Assignment Message */
  word curr_state
    /* Substate of System Access currently being processed */
)
{
  word next_state = curr_state;
    /* Next state to be processed */

  /* local variables to make copying and comparison easier */
  mccdma_ca_ord_type *ca_ptr;
  byte asgn_mode;
  byte msg_type;

  #if defined(FEATURE_IS2000_REL_C_AKA) || defined(FEATURE_IS2000_REL_A_AES)
#error code not present
  #endif /* FEATURE_IS2000_REL_C_AKA || FEATURE_IS2000_REL_A_AES */

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  ca_ptr = &matched_msg_ptr->chnasn.mode;
  asgn_mode = matched_msg_ptr->chnasn.assign_mode;
  msg_type = matched_msg_ptr->chnasn.msg_type;

  #ifdef FEATURE_IS2000_REL_D
#error code not present
  #endif /* FEATURE_IS2000_REL_D */


  if ( (asgn_mode == CAI_EXT_TRAF_CHAN_ASSIGN) ||
       (asgn_mode == CAI_TRAFFIC_CHAN_ASSIGN)
       #ifdef FEATURE_IS2000_REL_C
       || ((msg_type == CAI_EXT_CHN_ASN_MSG) &&
           (asgn_mode == CAI_PACKET_DATA_TRAF_CHAN_ASSIGN))
       #endif /* FEATURE_IS2000_REL_C */
     )
  {
    /* ----------------------------------------------------------------------
    ** Inform system determination of traffic channel assignment.
    ** Determines the next state to be processed based on returned SS-Action.
    ** ---------------------------------------------------------------------- */
    if (!mcc_inform_sd_chan_assign(&matched_msg_ptr->chnasn, MEAS_BACK_IGNORED))
    {
      /* Should never happen. SD does not like ECAM. */
      MSG_HIGH("SD does not like ECAM", 0, 0, 0);

      /* Send reject. */ 
      next_state = mccsa_send_reject(curr_state, CAI_REJ_CAP, matched_msg_ptr);
    }

    #ifdef FEATURE_IS2000_REL_A_AES
#error code not present
    #endif /* FEATURE_IS2000_REL_A_AES */
    
    #if defined(FEATURE_IS2000_REL_C_AKA) || defined(FEATURE_IS2000_REL_A_AES)
#error code not present
    #endif /* FEATURE_IS2000_REL_C_AKA || FEATURE_IS2000_REL_A_AES */

    else
    {

      #ifdef FEATURE_IS2000_REL_D
#error code not present
      #endif /* FEATURE_IS2000_REL_D */

      if ( next_state != CDMA_EXIT )
      {
        /* Initiate processing for traffic channel assignment */
        mcc_proc_traf_chan_assgn(&mccsa_matched_msg.chnasn);

        /* Go to TCI. */
        next_state = CDMA_TCI; /*lint !e641 */
      }
    } 
  }
  else if ((asgn_mode == CAI_PAGING_CHAN_ASSIGN) ||
          ((msg_type == CAI_CHN_ASN_MSG) && (asgn_mode == CAI_EXT_PAGING_CHAN_ASSIGN)))
  {

    /* The union am1 holds info about a PC assignment for both CAM and ECAM
     * inclding the extended PC assign of CAM */

    /* -----------------------------------------------------------------
    ** Inform system determination of paging channel assignment.
    ** Determines the next state to be processed based on the returned
    ** SS-Action.
    ** ----------------------------------------------------------------- */
    if (mcc_inform_sd_chan_assign(&matched_msg_ptr->chnasn, MEAS_BACK_IGNORED))
    {
      /* ---------------------------------------
      ** We already rejected if there was not at
      ** least one pilot in the pilot_pn list.
      ** --------------------------------------- */
      if ((ca_ptr->am1.respond) || (curr_state == CDMA_ORIG)) /*lint !e641 */
      {
        /* -----------------------------------------
        ** Since it is illegal for the base station
        ** to set the RESPOND bit to FALSE for an
        ** origination, we assume it is set to TRUE.
        ** ----------------------------------------- */
        mcc_pc_assign(curr_state, &matched_msg_ptr->chnasn);

        /* --------------------------------------------------
        ** re-enter Update Overhead Info substate with a page
        ** response retransmission or origination indication.
        ** -------------------------------------------------- */
        next_state = CDMA_OVHD; /*lint !e641 */
        
        /* Update idle-to-system access transition state. This is important
           for scenario of registration overrided by origination / page response. */ 
        cdma.sa_tc_state = curr_state;
      }
      else /* curr_state == CDMA_PAGE_RES, Respond = 0 */
      {
        mcc_pc_assign(curr_state, &matched_msg_ptr->chnasn);
        next_state = CDMA_IDLE;
      }
    }
    else
    {
      next_state = mccsa_send_reject(curr_state, CAI_REJ_CAP, matched_msg_ptr);
    }
  }

  #ifdef FEATURE_ACP
#error code not present
  #endif /* FEATURE_ACP */

  else
  {
    MSG_ERROR("INVALID ASSIGN MODE, SHOULD NOT GET HERE", 0,0,0);

    /* --------------------------------------------------
    ** Channel Assignment with illegal assign mode!  This
    ** will never occur because caix.c flags this as an
    ** invalid message.
    ** -------------------------------------------------- */
  }
  return (next_state);

} /* end mccsa_process_chan_assign_msg() */

/*===========================================================================

FUNCTION MCCSA_PROCESS_ORDER_MSG

DESCRIPTION
  This function processes an Order Message.

DEPENDENCIES
  None

RETURN VALUE
  The next state to be processed.

SIDE EFFECTS
  None

===========================================================================*/

word mccsa_process_order_msg
(
  mccdma_matched_msg_type *matched_msg_ptr,
    /* Pointer to Paging Channel Order Message */
  word curr_state
    /* Substate of System Access currently being processed */
)
{
  word next_state = curr_state;
    /* Next state to be processed */
  cm_mc_rpt_type *cmd_ptr;
    /* Pointer to handset command */

  #ifdef FEATURE_AUTH
  auth_cmd_type *auth_cmd_ptr;
    /* Pointer to authentication command */
  auth_rpt_type *auth_rpt_ptr;
    /* Pointer to authentication report */
  #endif /* FEATURE_AUTH */

  #ifdef FEATURE_IS2000_REL_C_AKA
#error code not present
  #endif /* FEATURE_IS2000_REL_C_AKA */
  
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  /* --------------------------------------------------------------
  ** Perform Layer 3 processing of the Paging Channel Order Message
  ** -------------------------------------------------------------- */

  switch (matched_msg_ptr->pc_ord.order)
  {
    case CAI_BS_CHAL_CONF_ORD:
      MSG_HIGH( "BS Challenge Confirmation Order", 0, 0, 0 );

      #ifdef FEATURE_AUTH
      cdma.sa_resp_msg = CAI_AC_ORD_MSG;
      cdma.sa_resp_ord.ordq.order = CAI_SSD_UPDATE_ORD;

      /* Unless all goes will we will send an SSD Update Rejection Order */
      cdma.sa_resp_ord.ordq.ordq = CAI_SSD_UPDATE_REJ;

      if ((auth_cmd_ptr = (auth_cmd_type*) q_get( &auth_free_q )) != NULL)
      {
        auth_cmd_ptr->hdr.command = AUTH_FINISH_SSD_UPDATE_F;
        auth_cmd_ptr->f_ssd.authbs = matched_msg_ptr->pc_ord.authbs;
        auth_cmd_ptr->f_ssd.rpt_function = mcc_queue_auth_rpt;

        mcc_auth_cmd( auth_cmd_ptr );

        if ( auth_cmd_ptr->hdr.status == AUTH_RECEIVED_S )
        {
          /* Wait for auth to report back if SSD Update was successful */

          /* ----------------------------------
          ** Wait for MCC_AUTH_Q_SIG to be set.
          ** ---------------------------------- */
          MSG_MED( "Waiting for AUTH report", 0, 0, 0 );

          (void) mcc_wait( MCC_AUTH_Q_SIG );

          if ((auth_rpt_ptr = (auth_rpt_type*) q_get( &mc_auth_q )) == NULL)
          {
            /* We got a queue signal but no report on the AUTH queue
             * show no report error message
             */
            ERR("Wrong signal-No report on mc_auth_q",0,0,0 );
          }

          if ((auth_rpt_ptr != NULL) &&
              (auth_rpt_ptr->rpt_type == AUTH_FINISH_SSD_R) &&
              (auth_rpt_ptr->rpt.ssd_update_ok))
          {
            /* -----------------------------------------------------------
            ** Since we have changed the SSD, we need to recalculate authr
            ** next time we do a registration.  Setting cdma.authr_valid
            ** to FALSE will force this to happen.
            ** ----------------------------------------------------------- */
            cdma.authr_valid = FALSE;

            /* Send SSD Update Confirmation Order */
            cdma.sa_resp_ord.ordq.ordq = CAI_SSD_UPDATE_CONF;

            #if defined(FEATURE_OTASP) && defined(FEATURE_OTASP_STATUS)
            /* Send OTASP status to CM */
            otasp_report_status_to_cm(CM_OTASP_STATUS_SSD_UPDATED);
            #endif /* FEATURE_OTASP && FEATURE_OTASP_STATUS */
          }
          else /* report was invalid */
          {
            ERR( "Report from AUTH was invalid!", 0, 0, 0 );
          }

          if (auth_rpt_ptr != NULL)
          {
            cmd_done( &auth_rpt_ptr->rpt_hdr );
          }
        }
        else /* Status returned from command was an error condition */
        {
          ERR( "Bad status from AUTH %d", auth_cmd_ptr->hdr.status, 0, 0 );
        }
      }
      else
      {
        ERR( "No free buffers on auth_free_q", 0, 0, 0 );
      }
      next_state = mccsa_send_order_response_msg( curr_state, FALSE );
    #endif /* FEATURE_AUTH */

      break;

    case CAI_LOCAL_CTL_ORD:
      MSG_HIGH( "Local Control Order", 0, 0, 0 );
      break;

    case CAI_LOCK_OR_MAINT_ORD:

      /**************
       * Lock Order *
       **************/

      if (matched_msg_ptr->pc_ord.ordq == CAI_UNLOCK_MASK)
      {
        /* We are already unlocked.  Ignore the order. */

        /* Events report: Unlock */
        event_report(EVENT_UNLOCK);
      }
      else if (matched_msg_ptr->pc_ord.ordq & CAI_LUPC_MASK)
      {
        MSG_MED( "Lock UPC Order", 0, 0, 0 );

        /* ----------------------------
        ** Exit to System Determination
        ** ---------------------------- */
        next_state    = CDMA_EXIT;
        mcc_exit.exit = MCC_LOCK;

        /* ------------------------------------
        ** Lock reason equals four LSBs of ORDQ
        ** ------------------------------------ */
        mcc_nv_data.lckrsn_p.lock_order = TRUE;
        mcc_nv_data.lckrsn_p.lock_reason =
          matched_msg_ptr->pc_ord.ordq & 0x0F;

        /* Write reason for lock to NVRAM */
        mcc_nv_buf.item = NV_LCKRSN_P_I;
        mcc_nv_buf.cmd = NV_WRITE_F;
        mcc_nv_buf.data_ptr = &mcc_nv_data;

        (void) mcc_nv_cmd( &mcc_nv_buf );

        /* Send lock command to UI */
        if ((cmd_ptr = cm_mc_rpt_get_buf_else_err_fatal()) != NULL)
        {
          cmd_ptr->hdr.cmd = CM_LOCK_F;
          mcc_cm_mc_rpt( cmd_ptr );
        }
        else
        {
          ERR("No free buffers on cm_mc_rpt_free_q",0,0,0 );
        }

        /* Events report: Lock */
        event_report(EVENT_LOCK);

      }

      /******************************
       * Maintenance Required Order *
       ******************************/

      else if (matched_msg_ptr->pc_ord.ordq & CAI_MAINT_REQ_MASK)
      {
        MSG_MED( "Maintenance Required Order", 0, 0, 0 );

        /* Write reason for maintenance required to NVRAM */
        mcc_nv_buf.item = NV_MAINTRSN_I;
        mcc_nv_buf.cmd = NV_WRITE_F;
        mcc_nv_data.maintrsn.maint_order = TRUE;
        mcc_nv_data.maintrsn.maint_reason =
          matched_msg_ptr->pc_ord.ordq & 0x0F;
        mcc_nv_buf.data_ptr = &mcc_nv_data;

        (void) mcc_nv_cmd( &mcc_nv_buf );

        /* Send maintenance required command to UI */
        if ((cmd_ptr = cm_mc_rpt_get_buf_else_err_fatal()) != NULL)
        {
          cmd_ptr->hdr.cmd = CM_MAINTREQ_F;
          mcc_cm_mc_rpt( cmd_ptr );
        }
        else
        {
          ERR("No free buffers on cm_mc_rpt_free_q",0,0,0 );
        }
      }
      else
      {
        /* Invalid ORDQ.  Reject */
        next_state = mccsa_send_reject( curr_state, CAI_REJ_FIELD, matched_msg_ptr );
      }

      break;

    case CAI_REG_ORD:
      /* ----------------------------------------------------
      ** Registration accepted or registration rejected order
      ** ---------------------------------------------------- */
      if (matched_msg_ptr->pc_ord.ordq == CAI_REG_ACK)
      {
        /* ---------------------------
        ** Registration accepted order
        ** --------------------------- */
      }
      else if (matched_msg_ptr->pc_ord.ordq == CAI_REG_ACC)
      {
        /* --------------------------------------------
        ** Registration accepted order with ROAM_INDI
        ** ------------------------------------------ */
        MSG_MED("Rxed Registration Accept with ROAM_INDI: %d",
                matched_msg_ptr->pc_ord.roam_indi, 0,0);

        #ifdef FEATURE_REG_ACC_ROAM_IND
        sd_si_roam_update( (sd_roam_e_type) matched_msg_ptr->pc_ord.roam_indi );
        #else

        /* ------------------------------------------------------------
        **  The Roaming indicator would normally be set here with the
        **  value received from the Base Station. The standard indicates
        **  that it may be set. Notice the keyword "may". The Roaming
        **  Indicator is not set for of the following reasons:
        **
        **  1) The base station may transmit a value in conflict with
        **     the value of the roaming indicator stored in the PRL.
        **     This may present the user with incorrect, or unreliable
        **     roaming status.
        **  2) After registering, we may go into a fast fade scenario,
        **     and not register after we re-acquire the system. This
        **     may lead to a incorrect roaming status, since we have
        **     not received the roaming indicator, in the Registration
        **     accepted order.
        **
        **  Therefore, we simply display an F3 Message on the DM, and
        **  ignore the roaming indicator in this order. We still display
        **  the roaming indicator as programmed in the PRL.
        ** -----------------------------------------------------------*/
        #endif /* FEATURE_REG_ACC_ROAM_IND */

      }

      else if (matched_msg_ptr->pc_ord.ordq == CAI_REG_REJ)
      {
        /* ---------------------------
        ** Registration rejected order
        ** --------------------------- */
        if (!matched_msg_ptr->pc_ord.hdr.ack_req)
        {
          /* Registration rejected and no TMSI delete */
          /* so reset full TMSI timer.                */
          tmsi_reset_full_timer ();

          {
            /* ----------------------------
            ** Exit to System Determination
            ** ---------------------------- */
            mcc_exit.exit = MCC_REG_REJECTED;
            next_state    = CDMA_EXIT;
          }

          /* Events report: Registration rejected */
          event_report(EVENT_REGISTRATION_REJECTED);
        }
        else
        {
          /* -------------------------------------------------------
          ** We need to send an ack before processing the layer 3 of
          ** this message.  Save the layer 3 and send a BS_ACK_ORD.
          ** ------------------------------------------------------- */
          mccsa_layer2.pending_msg = *matched_msg_ptr;
          mccsa_layer2.pending_msg_flag = TRUE;
          mccsa_layer2.dup_detect_done = TRUE;
        }
      }
      else if (matched_msg_ptr->pc_ord.ordq == CAI_REG_REJ_DEL_TMSI)
      {
        /* ---------------------------
        ** Registration rejected order
        ** --------------------------- */
        if (!matched_msg_ptr->pc_ord.hdr.ack_req)
        {
          /* Registration rejected and TMSI delete   */
          /* so reset TMSI code and all TMSI timers. */
          tmsi_code_and_timer_reset ();

          {
            /* ----------------------------
            ** Exit to System Determination
            ** ---------------------------- */
            mcc_exit.exit = MCC_REG_REJECTED;
            next_state    = CDMA_EXIT;
          }

          /* Events report: Registration rejected */
          event_report(EVENT_REGISTRATION_REJECTED);
        }
        else
        {
          /* -------------------------------------------------------
          ** We need to send an ack before processing the layer 3 of
          ** this message.  Save the layer 3 and send a BS_ACK_ORD.
          ** ------------------------------------------------------- */
          mccsa_layer2.pending_msg = *matched_msg_ptr;
          mccsa_layer2.pending_msg_flag = TRUE;
          /* Mark that duplicate detction has been done on this msg */
          mccsa_layer2.dup_detect_done = TRUE;
        }
      }

      #ifdef FEATURE_IS2000_REL_A
      else if (matched_msg_ptr->pc_ord.ordq == CAI_REG_ACCEPT_ORDER_ROAM_IND_AND_ENC_INC)
      {
        /* We ignore the Roaming indicator, see reasoning above (CAI_REG_ACC) */


        #ifdef FEATURE_IS2000_REL_A_AES
#error code not present
        #else
        if (matched_msg_ptr->pc_ord.c_sig_encrypt_mode !=
            CAI_SIG_ENC_MODE_DISABLED)
        #endif /* FEATURE_IS2000_REL_A_AES */
        {
          /* ----------------------------------------------------
          ** At this point, we don't support Signaling Encryption
          ** ---------------------------------------------------- */
          /* --------------------------------------------------------
          ** Inform system determination of registration rejected and
          ** determines the next state to be processed based on the
          ** returned SS-Action.
          ** -------------------------------------------------------- */
          MSG_MED("Checking to see if it needs to exit CDMA", 0, 0, 0);
          if (!mcc_continue_on_curr_sys(MEAS_BACK_ALLOWED,
              sd_ss_ind_cdma_opr_reg_rej(NULL)))
          {
            MSG_MED("Exiting CDMA...", 0, 0, 0);
            next_state = CDMA_EXIT;
            mcc_exit.exit = MCC_REG_REJECTED;
          }
        }

        #if defined(FEATURE_IS2000_REL_C_AKA) || defined(FEATURE_IS2000_REL_A_AES)
#error code not present
        #endif /* FEATURE_IS2000_REL_C_AKA || FEATURE_IS2000_REL_A_AES */
      } /* CAI_REG_ACCEPT_ORDER_ROAM_IND_AND_ENC_INC */
      #endif /* FEATURE_IS2000_REL_A */

      else if (matched_msg_ptr->pc_ord.ordq != CAI_REG_REQ)
      {
        /* Invalid ORDQ.  Reject */
        next_state = mccsa_send_reject( curr_state, CAI_REJ_FIELD, matched_msg_ptr );
      }
      break;

    case CAI_RELEASE_ORD:
      MSG_HIGH("Release Order", 0, 0, 0);

      if ((curr_state == CDMA_ORIG) || (curr_state == CDMA_PAGE_RES) || /*lint !e641 */
          (curr_state == CDMA_REG)) /*lint !e641 */
      {
        /* ----------------------------------------------
        ** We only process release orders in Origination,
        ** Page Response, and Registration substates.
        ** ---------------------------------------------- */

        if (!matched_msg_ptr->pc_ord.hdr.ack_req)
        {
          /* First send CM release indication with reason */
          if (curr_state == CDMA_ORIG) /*lint !e641 */
          {
            if ((cmd_ptr = cm_mc_rpt_get_buf_else_err_fatal()) != NULL)
            {
              cmd_ptr->release.hdr.cmd = CM_CALL_RELEASE_F;
              cmd_ptr->release.num_call_ids = 1;
              cmd_ptr->release.call_ids[0] = cdma.call_id;
              cmd_ptr->release.rel_codes[0] = (cm_rel_reason_e_type)
                    (( matched_msg_ptr->pc_ord.ordq == CAI_REL_SO )?
                       CM_CALL_REL_SO_REJECT:CM_CALL_REL_NORMAL);

              mcc_cm_mc_rpt( cmd_ptr );
            }
            else
            {
              ERR("No free buffers on cm_mc_rpt_free_q",0,0,0 );
            }
          }

          /* ----------------------------
          ** Exit to System Determination
          ** ---------------------------- */
          mcc_exit.exit = MCC_RELEASE;
          next_state    = CDMA_EXIT;
          cdma.rel_orig = FALSE;

          /* Events report: Release */
          mclog_report_release(MCLOG_RELEASE_BS_ORDER);
        }
        else
        {
          /* -------------------------------------------------------
          ** We need to send an ack before processing the layer 3 of
          ** this message.  Save the layer 3 and send a BS_ACK_ORD.
          ** ------------------------------------------------------- */
          mccsa_layer2.pending_msg = *matched_msg_ptr;
          mccsa_layer2.pending_msg_flag = TRUE;
          /* Mark that duplicate detction has been done on this msg */
          mccsa_layer2.dup_detect_done = TRUE;
        }
      }
      break;

    case CAI_INTERCEPT_ORD:
      MSG_MED( "Intercept Order", 0, 0, 0 );

      if (curr_state == CDMA_ORIG) /*lint !e641 */
      {
        /* ---------------------------------------------------------
        ** We only process intercept orders in Origination substate.
        ** --------------------------------------------------------- */

        if (!matched_msg_ptr->pc_ord.hdr.ack_req)
        {
          /* --------------------
          ** Sound intercept tone
          ** -------------------- */
          process_intercept(curr_state);

          next_state = CDMA_EXIT;
          mcc_exit.exit = MCC_INTERCEPT;
        }
        else
        {
          /* -------------------------------------------------------
          ** We need to send an ack before processing the layer 3 of
          ** this message.  Save the layer 3 and send a BS_ACK_ORD.
          ** ------------------------------------------------------- */
          mccsa_layer2.pending_msg = *matched_msg_ptr;
          mccsa_layer2.pending_msg_flag = TRUE;
          /* Mark that duplicate detction has been done on this msg */
          mccsa_layer2.dup_detect_done = TRUE;
        }
      }
      break;

    case CAI_REORDER_ORD:
      MSG_MED( "Reorder Order", 0, 0, 0 );

      if (curr_state == CDMA_ORIG) /*lint !e641 */
      {
        /* -------------------------------------------------------
        ** We only process reorder orders in Origination substate.
        ** ------------------------------------------------------- */

        if (!matched_msg_ptr->pc_ord.hdr.ack_req)
        {
          next_state = CDMA_EXIT;
          mcc_exit.exit = MCC_REORDER;
        }
        else
        {
          /* -------------------------------------------------------
          ** We need to send an ack before processing the layer 3 of
             ** this message.  Save the layer 3 and send a BS_ACK_ORD.
          ** ------------------------------------------------------- */
          mccsa_layer2.pending_msg = *matched_msg_ptr;
          mccsa_layer2.pending_msg_flag = TRUE;
          /* Mark that duplicate detction has been done on this msg */
          mccsa_layer2.dup_detect_done = TRUE;
        }
      }
      break;

    case CAI_RETRY_ORD:
      MSG_MED( "Retry Order", 0, 0, 0 );
      if (P_REV_IS_6_OR_GREATER)
      {
        if ((matched_msg_ptr->pc_ord.retry_type == CAI_RETRY_CLR_ALL) || /*lint !e641 */
            (matched_msg_ptr->pc_ord.retry_type == CAI_RETRY_ORIG) /*lint !e641 */
      #ifdef FEATURE_IS2000_REL_D
#error code not present
      #endif
           )
        {
          if (!matched_msg_ptr->pc_ord.hdr.ack_req)
          {
            /* ----------------------------------------------------------
            ** If retry order is received in response to Origination,
            ** inform CM which also informs DS the update of retry delay
            ** of origination failure.
            ** ---------------------------------------------------------- */
            if (curr_state == CDMA_ORIG) /*lint !e641 */
            {

              #ifdef FEATURE_DS
              if (DS_ISDATA(cdma.so_req))
              {
                MSG_MED("Rxed Retry Order - Origination failed", 0, 0, 0);
                mcc_send_orig_fail_retry_delay(
                  matched_msg_ptr->pc_ord.retry_type,
                  matched_msg_ptr->pc_ord.retry_delay, cdma.so_req, cdma.call_id);

                update_retry_delay((cai_retry_order_type) matched_msg_ptr->pc_ord.retry_type, matched_msg_ptr->pc_ord.retry_delay);

                next_state = CDMA_EXIT;
                mcc_exit.exit = MCC_ACCESS_FAILED;
              }
              else
              #endif /* FEATURE_DS */

              {
                MSG_MED("Ignore retry order", 0, 0, 0);
              }
            }
            /* --------------------------------------------------------
            ** Otherwise, inform DS the update of retry delay directly.
            ** -------------------------------------------------------- */
            else
            {
              update_retry_delay((cai_retry_order_type) matched_msg_ptr->pc_ord.retry_type,
                matched_msg_ptr->pc_ord.retry_delay);
            }
          }
          else
          {
            /* -------------------------------------------------------
            ** We need to send an ack before processing the layer 3 of
            ** this message.  Save the layer 3 and send a BS_ACK_ORD.
            ** ------------------------------------------------------- */
            mccsa_layer2.pending_msg = *matched_msg_ptr;
            mccsa_layer2.pending_msg_flag = TRUE;
            /* Mark that duplicate detction has been done on this msg */
            mccsa_layer2.dup_detect_done = TRUE;
          } /* end if (!matched_msg_ptr->pc_ord.hdr.ack_req) */
        }
        else
        {
          /* -------------------------------------------------------
          ** Send a Mobile Station Reject Order (invalid ordq field)
          ** ------------------------------------------------------- */
          next_state = mccsa_send_reject(curr_state, CAI_REJ_FIELD, matched_msg_ptr);
        } /* end if CAI_RETRY_CLR_ALL or CAI_RETRY_ORIG or CAI_RETRY_SCRM */
      }
      else
      {
        /* -------------------------------------------
        ** Reject Order if P_REV_IN_USE is less than 6
        ** ------------------------------------------- */
        next_state = mccsa_send_reject(curr_state, CAI_REJ_UNSP, matched_msg_ptr);
      } /* end if (P_REV_IS_6_OR_GREATER) */
      break;

    #ifdef FEATURE_IS2000_REL_A
    case CAI_BS_REJ_ORD:
      MSG_MED( "BS Reject Order", 0, 0, 0 );
      /* --------------------------------------------------------------------
      ** Enhanced encryption is not supported, in the meanwhile MS just exit
      ** to system determination if Base Station Reject Order is received.
      ** -------------------------------------------------------------------- */
      next_state = CDMA_EXIT;
      mcc_exit.exit = MCC_ENCRYPTION_MI_FAILED;
      break;
    #endif /* FEATURE_IS2000_REL_A */

    #ifdef FEATURE_IS2000_REL_D
#error code not present
    #endif /* FEATURE_IS2000_REL_D */

    default:
      /* -----------------------
      ** Ignore all other orders
      ** ----------------------- */
      break;
  } /* end switch (msg_ptr->pc_ord.order) */

  return (next_state);

} /* end mccsa_process_order_msg() */

/*===========================================================================

FUNCTION MCCSA_INCREMENT_L3_COUNTER

DESCRIPTION
  This function increments L3 counter. 
  fields.

DEPENDENCIES
  None

RETURN VALUE
  Next state to be processed.

SIDE EFFECTS
  None

===========================================================================*/

void mccsa_increment_l3_counter
(
  mccsa_l3_counter_type counter_type 
)
{
  word parm_stat; /* Status returned by parm_inc procedure */

  if (counter_type == SA_REQUEST)
  {
    /* ----------------------------------------------------
    ** Increment number of Access Channel request messages
    ** generated by Layer 3
    ** ---------------------------------------------------- */
    #ifdef FEATURE_IS2000_REL_A_CC
    if (OVHD_CHAN_IS_BCCH)
    {
      if ((parm_stat = parm_inc(CAI_EACH_BA_1_ID, 1)) != PARM_DONE_S)
      {
        MSG_HIGH("Invalid parameter increment: status %d", parm_stat, 0, 0);
      }
    }
    else
    #endif /* FEATURE_IS2000_REL_A_CC */

    {
      if ((parm_stat = parm_inc(CAI_ACC_1_ID, 1)) != PARM_DONE_S)
      {
        MSG_HIGH("Invalid parameter increment: status %d", parm_stat, 0, 0);
      }
    }
  }
  else if (counter_type == SA_RESPONSE)
  {
    /* ----------------------------------------------------
    ** Increment number of Access Channel response messages
    ** generated by Layer 3
    ** ---------------------------------------------------- */
    #ifdef FEATURE_IS2000_REL_A_CC
    if (OVHD_CHAN_IS_BCCH)
    {
      if ((parm_stat = parm_inc(CAI_EACH_BA_2_ID, 1)) != PARM_DONE_S)
      {
        MSG_HIGH("Invalid parameter increment: status %d", parm_stat, 0, 0);
      }
    }
    else
    #endif /* FEATURE_IS2000_REL_A_CC */

    {
      if ((parm_stat = parm_inc( CAI_ACC_2_ID, 1 )) != PARM_DONE_S)
      {
        MSG_HIGH("Invalid parameter increment: status %d", parm_stat, 0, 0);
      }
    }
  }
  else if (counter_type == SA_FAIL)
  {
    /* ----------------------------------------------------
    ** Increment number of unsuccessful access attempt.
    ** ---------------------------------------------------- */
    #ifdef FEATURE_IS2000_REL_A_CC
    if (OVHD_CHAN_IS_BCCH)
    {
      if ((parm_stat = parm_inc(CAI_EACH_BA_8_ID, 1)) != PARM_DONE_S)
      {
        MSG_HIGH("Invalid parameter increment: status %d", parm_stat, 0, 0);
      }
    }
    else
    #endif /* FEATURE_IS2000_REL_A_CC */

    {
      if ((parm_stat = parm_inc( CAI_ACC_8_ID, 1 )) != PARM_DONE_S)
      {
        MSG_HIGH("Invalid parameter increment: status %d", parm_stat, 0, 0);
      }
    }
  }
} /* mccsa_increment_l3_counter */

#if defined(FEATURE_AUTH) && defined (FEATURE_AUTH_DIGITS)
/*===========================================================================

FUNCTION MCCSA_CALCULATE_DBM_AUTH_DATA

DESCRIPTION
  This function figures out what the auth_data should be for authenticating
  a non-SMS data burst message.

DEPENDENCIES
  None

RETURN VALUE
  The computed value of auth_data

SIDE EFFECTS
  None

===========================================================================*/

dword mccsa_calculate_dbm_auth_data
(
  byte burst_type,
    /* The burst type of the data burst message */
  byte chari[],
    /* The payload data of the data burst message */
  byte array_len
    /* The number of bytes in the above array */
)
{
  byte num_digits;
    /* The number of digits to be used in auth_data */
  byte i;
    /* Index into chari array */
  dword auth_data;
    /* The auth_data that is to be sent to the authentication routine */

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
                        
  switch ( burst_type )
  {
    /*---------------------------------------------------
    ** The authentication procedure for the SDB message
    ** will consist of 3 octets generated from the first
    ** 3 CHARi fields of the data burst message (or all 
    ** the bits of the CHARi fields, if less than 3 octets)
    ** See IS-707, 2.2.9.1
    **-------------------------------------------------*/
    case CAI_SHORT_DATA_BURST:

      /* compute how many digits we are to use */
      num_digits = ( array_len < 3 ) ? array_len : 3;

      /* Fill auth_data with IMSI_S1 if less than 3 octets 
      ** in the CHARi field. */
      auth_data = mcc_get_auth_imsi_s1();

      /* Starting at the least significant byte, replace the byte
      ** with however much is necessary with the supplied chari
      */
      for ( i = 0; i < num_digits; i++ )
      {
        /* Mask out 8 bits of auth_data which chari will replace */
        auth_data &= ( 0xFFFFFFFFL ^ ( 0xFFL << ( i * 8 )));

        /* OR in next chari byte */
        auth_data |= ( chari[num_digits - i - 1] << ( i * 8 )); /*lint !e834 */
      }        
      break;

    default:
      auth_data = 0;
      MSG_ERROR("Wrong data burst type %d", burst_type, 0, 0);
      break;
  }

  return auth_data;

} /* mccsa_calculate_dbm_auth_data () */

#endif /* FEATURE_AUTH and FEATURE_AUTH_DIGITS */

#ifdef FEATURE_IS2000_REL_C
/*===========================================================================

FUNCTION MCCSA_SETUP_MSG_INT_FIELDS

DESCRIPTION
  This function setups message integrity fields. These fields exist starting
  from REL C.

DEPENDENCIES
  None

RETURN VALUE
  Next state to be processed.

SIDE EFFECTS
  None

===========================================================================*/
/* Following parameters not used with some FEATURE define combinations */
/*lint -esym(715, burst_type) */
/*lint -esym(715, data_len) */
/*lint -esym(715, data_ptr) */
/*lint -esym(715, digit_mode) */
void mccsa_setup_msg_int_fields
(
  caii_tx_msg_type *msg_ptr,
  caii_ac_auth_msg_int_type *auth_msg_int_ptr
)
{

  #ifdef FEATURE_IS2000_REL_C_AKA
#error code not present
  #endif /* FEATURE_IS2000_REL_C_AKA */
  uint8 msg_type;

  msg_type = msg_ptr->gen_ac.msg_type;


  if ( P_REV_IS_9_OR_GREATER )
  {

    /* starting from P_REV 9, there is a new LAC field of sdu_key_id,
     * but it may only possibly be set to be non-zero when P_REV is 10 or above,
     * AND AKA is supported.
     * so we first set default value to be 0 here.
     */
    auth_msg_int_ptr->sdu_key_id = 0;

    /* by default, set maci_incl to be FALSE. It only be true when 
     * MACI feature is defined and P_REV in use is 10 or above
     */
    auth_msg_int_ptr->maci_incl = FALSE;

    /* for ROP messages, when P_REV is 9 or above, there are message integrity fields
     * to be set. For P_REV 9, the message integrity fields should not be included. 
     * for P_REV 10 or above, these fields should be set according to AKA procedure.
     *
     * so first we set default value to be FALSE, then check if it is P_REV 10 and
     * AKA is defined, call the AKA function mccaka_setup_rop_msg_int_fields. 
     */
    if ( msg_type == CAI_PAGE_RESP_MSG )
    {
      msg_ptr->page_resp.msg_int_info_incl = FALSE;
    }
    else if ( msg_type == CAI_ORIG_MSG )
    {
      msg_ptr->orig.msg_int_info_incl = FALSE;
    }
    else if ( msg_type == CAI_REG_MSG )
    {
      msg_ptr->reg.msg_int_info_incl = FALSE;
    }
  } /* P_REV_IS_9_OR_GREATER */
 

  /* Now check if it is P_REV 10 and AKA is defined, if so, we need set up the
   * fields according to the AKA procedure.
   */
  #ifdef FEATURE_IS2000_REL_C_AKA
#error code not present
  #endif /* FEATURE_IS2000_REL_C_AKA */
} /* mccsa_setup_msg_int_fields */
#endif /* FEATURE_IS2000_REL_C */

/*===========================================================================

FUNCTION MCCSA_SETUP_AUTH_MSG_INT_FIELDS

DESCRIPTION
  This function setups the authentication fields and message integrity 
  fields.

DEPENDENCIES
  None

RETURN VALUE
  Next state to be processed.

SIDE EFFECTS
  None

===========================================================================*/
/* Following parameters not used with some FEATURE define combinations */
/*lint -esym(715, burst_type) */
/*lint -esym(715, data_len) */
/*lint -esym(715, data_ptr) */
/*lint -esym(715, digit_mode) */
word mccsa_setup_auth_msg_int_fields
(
  word curr_state,
  caii_tx_msg_type *msg_ptr,
  byte burst_type,    /*lint -esym(715,burst_type)*/    
  byte data_len,      /*lint -esym(715,data_len)*/
  byte *data_ptr,     /*lint -esym(715,data_ptr)*/
  boolean digit_mode, /*lint -esym(715,digit_mode)*/
  byte num_of_digits, /*lint -esym(715,num_of_digits)*/
  caii_ac_auth_msg_int_type *auth_msg_int_ptr
)
{
  #ifdef FEATURE_AUTH
  auth_cmd_type *auth_cmd_ptr;
    /* Pointer to authentication command */
  #ifdef FEATURE_UIM_RUIM
  word i;
  #endif /* FEATURE_UIM_RUIM */
  #endif /* FEATURE_AUTH */

 
  
  word next_state = curr_state;
  uint8 msg_type;

  msg_type = msg_ptr->gen_ac.msg_type;

  #ifdef FEATURE_IS2000_REL_C
  /* starting from REL C, there are message integrity fields */
  mccsa_setup_msg_int_fields(msg_ptr, auth_msg_int_ptr);
  #endif /* FEATURE_IS2000_REL_C */

  if ((msg_type != CAI_REG_MSG) && (msg_type != CAI_ORIG_MSG) &&
      (msg_type != CAI_PAGE_RESP_MSG) && 
      #ifdef FEATURE_IS2000_REL_C
      (msg_type != CAI_AC_RECONNECT_MSG) &&
      #endif /* FEATURE_IS2000_REL_C */
      (msg_type != CAI_AC_BURST_MSG) && (msg_type != CAI_AC_TMSI_CMP_MSG))
  {
    auth_msg_int_ptr->auth_mode = CAI_NO_AUTH;
    return next_state;
  }

  #ifndef FEATURE_AUTH
  auth_msg_int_ptr->auth_mode = CAI_NO_AUTH;
  #else
  auth_msg_int_ptr->auth_mode = cur_bs_ptr->rp.auth;

  if (cur_bs_ptr->rp.auth == CAI_AUTH) 
  {
    if ((msg_type == CAI_REG_MSG) && cdma.authr_valid && 
        (cdma.rand == cur_bs_ptr->rp.rand)
    #ifdef FEATURE_IS2000_REL_C_AKA
#error code not present
    #endif /* FEATURE_IS2000_REL_C_AKA */
       )
    {
      /* For registration message and RAND has not changed, use stored value 
         for AUTHR */
      auth_msg_int_ptr->authr = cdma.authr;
      /* Fill in remaining auth related fields */
      auth_msg_int_ptr->randc = 
        (byte) ((cur_bs_ptr->rp.rand & 0xFF000000L) >> 24); /*lint !e737*/
      auth_msg_int_ptr->count = auth_get_count();
      return next_state;
    }

    /* -------------------------------------------------------
    ** Authentication is enabled, fill in auth related fields.
    ** We need to generate new authr for page response every
    ** time, to make sure registers are saved.
    ** ------------------------------------------------------- */
    if ((auth_cmd_ptr = (auth_cmd_type*) q_get( &auth_free_q)) != NULL)
    {

      #ifdef FEATURE_UIM_RUIM
      #ifdef FEATURE_UIM_RUN_TIME_ENABLE 
      /* Determine if the R-UIM is available for this NAM */
      if (nv_rtre_use_ruim_for_nam(cdma.curr_nam))
      #endif /* FEATURE_UIM_RUN_TIME_ENABLE */

      {
        auth_cmd_ptr->hdr.command = AUTH_SIGNATURE_F;
        auth_cmd_ptr->sig.rand_type = RUIM_GLOBAL_RAND_CHAL;
        auth_cmd_ptr->sig.rand_chal = cur_bs_ptr->rp.rand;

        if ((msg_type != CAI_ORIG_MSG) && (msg_type != CAI_AC_BURST_MSG))
        {
          /* Use IMSI_S1 as AUTH_DATA */
          auth_cmd_ptr->sig.dig_len = 0;
          for (i = 0; i < RUIM_RUN_CAVE_DIGITS ; i++)
          {
            auth_cmd_ptr->sig.digits[i] = 0;
          }
        }
        else if ((msg_type == CAI_AC_BURST_MSG) && 
                 (burst_type == CAI_SHORT_DATA_BURST))
        {
          uint8 num_bytes;

          num_bytes = (data_len < RUIM_RUN_CAVE_DIGITS) ? data_len: RUIM_RUN_CAVE_DIGITS;
          auth_cmd_ptr->sig.dig_len = num_bytes * 8;  /*lint !e734*/
          for (i = RUIM_RUN_CAVE_DIGITS;  0 < num_bytes <= i; i--, num_bytes--) /*lint !e503*/
          {
            /* Copy the payload of SDB to the digits array.
               Start from least significant byte.*/
            auth_cmd_ptr->sig.digits[i - 1] = data_ptr[num_bytes - 1];
          }
       
          for (; i > 0; i--)
          {
            /* If less than 3 bytes of payload, fill leading digits with 0 */
            auth_cmd_ptr->sig.digits[i - 1] = 0;
          }
        }
        else
        {
          /* Use digits as AUTH_DATA */

          #ifndef FEATURE_AUTH_DIGITS
          auth_cmd_ptr->sig.dig_len = 0;
          for (i = 0; i < RUIM_RUN_CAVE_DIGITS ; i++)
          {
            auth_cmd_ptr->sig.digits[i] = 0;
          }
          #else /* !FEATURE_AUTH_DIGITS */
          /* Set the number of bits in the digits */
          auth_cmd_ptr->sig.dig_len = 
            ((num_of_digits < (RUIM_RUN_CAVE_DIGITS * 2)) ?
              num_of_digits : (RUIM_RUN_CAVE_DIGITS * 2)) * 4 ;  /*lint !e734*/

          if ((msg_type == CAI_AC_BURST_MSG) && 
              (burst_type == CAI_POSITION_DET))
          {
            for (i = 0; i < RUIM_RUN_CAVE_DIGITS ; i++)
            {
              auth_cmd_ptr->sig.digits[i] = data_ptr[i];
            }
          }
          else
          {
            auth_calculate_digits(data_ptr, data_len, digit_mode,
              auth_cmd_ptr->sig.digits);
          }
          #endif /* FEATURE_AUTH_DIGITS */

        }

        auth_cmd_ptr->sig.process_control = RUIM_DEFLT_PROC_CNTRL;

        if ((msg_type == CAI_ORIG_MSG) || (msg_type == CAI_PAGE_RESP_MSG)
            #ifdef FEATURE_IS2000_REL_A
            || ((msg_type == CAI_REG_MSG) && P_REV_IS_7_OR_GREATER)
            #endif /* FEATURE_IS2000_REL_A */
            #ifdef FEATURE_IS2000_REL_C
            || (msg_type == CAI_AC_RECONNECT_MSG)
            #endif /* FEATURE_IS2000_REL_C */
           )
        {
          auth_cmd_ptr->sig.process_control |= RUIM_SAVE_REGS_ON;
        }

        auth_cmd_ptr->sig.esn = cdma.esn;
        auth_cmd_ptr->sig.rpt_function = mcc_queue_auth_rpt;
      }

      #ifdef FEATURE_UIM_RUN_TIME_ENABLE 
      else /* R-UIM is not available */
      #endif /* FEATURE_UIM_RUN_TIME_ENABLE */
      #endif /* FEATURE_UIM_RUIM */

      #if !defined(FEATURE_UIM_RUIM) || defined(FEATURE_UIM_RUN_TIME_ENABLE)
      {
        auth_cmd_ptr->hdr.command = AUTH_SIGNATURE_F;
        auth_cmd_ptr->sig.rand_chal = cur_bs_ptr->rp.rand;

        #ifndef FEATURE_AUTH_DIGITS
        auth_cmd_ptr->sig.auth_data = 0;
        #else
        if ((msg_type != CAI_ORIG_MSG) && (msg_type != CAI_AC_BURST_MSG))
        {
          /* Use IMSI_S1 as AUTH_DATA */
          auth_cmd_ptr->sig.auth_data = mcc_get_auth_imsi_s1();
        }
        #ifdef FEATURE_PASSPORT_MODE
        else if (mcc_passport_mode_is_jcdma() && (msg_type == CAI_AC_BURST_MSG)) 
        {
          auth_cmd_ptr->sig.auth_data = 0;
          MSG_HIGH("JCDMA build, set auth_data to 0 for DBM", 0,0,0);
        }
        #endif /* FEATURE_PASSPORT_MODE */
        else if ((msg_type == CAI_AC_BURST_MSG) && 
                 (burst_type == CAI_POSITION_DET))
        {
          /*---------------------------------------------------
          ** The authentication procedure for the PD message
          ** will consist of 3 octets generated from the first
          ** 3 CHARi fields of the data burst message. See
          ** IS-801, 3.2.2.2
          ** We have already checked at least 3 bytes for PD
          ** message.
          **-------------------------------------------------*/
          auth_cmd_ptr->sig.auth_data =
            ((data_ptr[0] << 16) | (data_ptr[1] << 8) | (data_ptr[2]));
        }
        else if ((msg_type == CAI_AC_BURST_MSG) && 
                 (burst_type == CAI_SHORT_DATA_BURST))
        {
          auth_cmd_ptr->sig.auth_data = 
            mccsa_calculate_dbm_auth_data(burst_type, data_ptr, data_len);
        }       
        else
        {
          auth_cmd_ptr->sig.auth_data = 
            mcc_calculate_auth_data(data_ptr, data_len, digit_mode);
        }
        #endif /* FEATURE_AUTH_DIGITS */

        if ((msg_type == CAI_ORIG_MSG) || (msg_type == CAI_PAGE_RESP_MSG)
            #ifdef FEATURE_IS2000_REL_A
            || ((msg_type == CAI_REG_MSG) && P_REV_IS_7_OR_GREATER)
            #endif /* FEATURE_IS2000_REL_A */
            #ifdef FEATURE_IS2000_REL_C
            || (msg_type == CAI_AC_RECONNECT_MSG)
            #endif /* FEATURE_IS2000_REL_C */
           )
        {
          auth_cmd_ptr->sig.save_reg = TRUE;
        }
        else
        {
          auth_cmd_ptr->sig.save_reg = FALSE;
        }

        auth_cmd_ptr->sig.rpt_function = mcc_queue_auth_rpt;
      }
      #endif /* !FEATURE_UIM_RUIM || FEATURE_UIM_RUN_TIME_ENABLE */

      mcc_auth_cmd(auth_cmd_ptr);

      if (auth_cmd_ptr->hdr.status == AUTH_RECEIVED_S)
      {
        /* Finally, we can fill in the auth related fields */
        auth_msg_int_ptr->randc = (byte) ((cur_bs_ptr->rp.rand & 0xFF000000L) >> 24); /*lint !e737*/
        auth_msg_int_ptr->count = auth_get_count();
        next_state = mccsa_wait_for_gen_auth_signature(&auth_msg_int_ptr->authr,
          #ifdef FEATURE_UIM_RUIM
          &auth_msg_int_ptr->auth_mode,
          #endif /* FEATURE_UIM_RUIM */
          curr_state);

        if (next_state == curr_state)
        {
          if (msg_type == CAI_REG_MSG)
          {
            /* Check the auth_mode to ensure the auth signature is valid */
            if (auth_msg_int_ptr->auth_mode == CAI_AUTH)
            {
              /* --------------------------------------------------------
              ** Update stored registration authr value to newly computed
              ** value, and remember RAND value used to calculate it.
              ** -------------------------------------------------------- */
              cdma.rand = cur_bs_ptr->rp.rand;
              cdma.authr = mcc_tx_int_msg.reg.ami.authr;
              cdma.authr_valid = TRUE;
            } 
          }
        }
      }
      else
      {
        /* ----------------------------------------------
        ** We have a serious problem and can not offer an
        ** authentication signature.  We set auth_mode to
        ** zero in the message to indicate this.
        ** ---------------------------------------------- */
        ERR("Bad status from AUTH %d", auth_cmd_ptr->hdr.status, 0, 0);
        auth_msg_int_ptr->auth_mode = CAI_NO_AUTH;
      }
    }
    else
    {
      ERR("No free buffers on auth_free_q", 0, 0, 0);
      /* We can not calculate authentication values */
      auth_msg_int_ptr->auth_mode = CAI_NO_AUTH;
    }
  } /* end if (authentication enabled) */
  #endif /* !FEATURE_AUTH */
 
  if ((msg_type == CAI_ORIG_MSG) || (msg_type == CAI_PAGE_RESP_MSG) 
      #ifdef FEATURE_IS2000_REL_C
      || (msg_type == CAI_AC_RECONNECT_MSG)
      #endif /* FEATURE_IS2000_REL_C */
     )
  {
    /* Record whether or not we send authentication data */
    cdma.auth_mode = auth_msg_int_ptr->auth_mode;
  }

  return next_state;

} /* mccsa_setup_auth_msg_int_fields */
/*lint +esym(715, burst_type) */
/*lint +esym(715, data_len) */
/*lint +esym(715, data_ptr) */
/*lint +esym(715, digit_mode) */

#if defined(FEATURE_CDSMS) || defined(FEATURE_DBM)
/*===========================================================================

FUNCTION MCCSA_BUILD_DBM

DESCRIPTION
  This function builds a Data Burst Message.

DEPENDENCIES
  None

RETURN VALUE
  next state

SIDE EFFECTS
  None

===========================================================================*/

word mccsa_build_dbm
(
  byte burst_type,
    /* Burst Type */
  byte data_len,
    /* Byte length of Data Burst data */
  byte *data_ptr,
    /* Pointer to Data Burst data */
  word curr_state
    /* Current state being processed */
)
{
  word msg_len;                   /* Length of bit-packed message */
  byte i;                         /* Index variable */
  access_result_type access_aborted = NOT_ABORT;
    /* Indicates if access attempt aborted */
  word next_state = curr_state;   /* Next state to be processed */
  mccsa_l3_counter_type counter_type;
  boolean digit_mode = FALSE; 
    /* Converted result of digit mode from enum to boolean */
  byte num_of_digits = 0;
  byte auth_data_len = data_len;  /* Length of AUTH data */
  byte *auth_data_ptr = data_ptr; /* Pointer to AUTH data */

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  MSG_HIGH("DBM type %d size %d", burst_type, data_len, 0);

  #ifdef FEATURE_DBM
  /*-----------------------------------------------------------------
  ** The PD message must have at least 3 bytes of CHARi in the data
  ** burst message, in the following format (IS-801, 3.2.4):
  **    Field                   length(bits)
  **    SESS_START              1
  **    SESS_END                1
  **    SESS_SOURCE             1
  **    SESS_TAG                5
  **    PD_MSG_TYPE             8
  **    NUM_REQUESTS            4
  **    NUM_RESPONSES           4
  ** These 3 bytes will be used as the AUTH_DATA in the auth
  ** procedure.
  **---------------------------------------------------------------*/
  if ((data_len < 3) && (burst_type == CAI_POSITION_DET))
  {
    ERR_FATAL("PD message format error.", 0, 0, 0);
  }

  /* ----------------------------------------------------------------
  ** The layer 2 and 3 are connected in that the message is a request
  ** if valid_ack is FALSE and a respone if VALID_ACK is TRUE.
  ** ---------------------------------------------------------------- */
  counter_type = (!mcc_layer2.valid_ack) ? SA_REQUEST : SA_RESPONSE;

  #ifdef FEATURE_SO_8026
#error code not present
  #endif /* FEATURE_SO_8026 */
  #endif /* FEATURE_DBM */

  /* -----------------------------
  ** Build the Data Burst Message
  ** ----------------------------- */
  mcc_tx_int_msg.ac_burst.msg_type = mccsa_layer2.last_msg_type = CAI_AC_BURST_MSG;

  mcc_tx_int_msg.ac_burst.hdr.msg_seq = mcc_layer2.msg_seq = 
    (mcc_layer2.msg_seq + 1) & 0x7; /* Modulo 8 sequence number */

  mcc_tx_int_msg.ac_burst.hdr.ack_seq  = mcc_layer2.ack_seq;
  mcc_tx_int_msg.ac_burst.hdr.ack_type = mcc_layer2.ack_type;
  mcc_tx_int_msg.ac_burst.hdr.ack_req  = TRUE;
  mcc_tx_int_msg.ac_burst.hdr.valid_ack = mcc_layer2.valid_ack;

  #ifdef FEATURE_IS2000_REL_A
  /* Setup encryption fields */
  mcc_tx_int_msg.ac_burst.enc.enc_fields_incl = FALSE;

  #ifdef FEATURE_IS2000_REL_A_AES
#error code not present
  #endif /* FEATURE_IS2000_REL_A_AES */

  #endif /* FEATURE_IS2000_REL_A */

  mcc_set_address(&mcc_tx_int_msg.ac_burst.hdr, mcc_tx_int_msg.ac_burst.msg_type);
 
  /* Setup authentication and message integrity fields */
  switch (burst_type)
  {

    #ifdef FEATURE_CDSMS 
    case CAI_SHORT_MSG_SERVICES:

      #if defined(FEATURE_AUTH) && defined(FEATURE_AUTH_DIGITS)
      digit_mode = ((cm_sms_msg_type*)cdma.dbm_buf_ptr)->address.digit_mode ==
                   (CM_DIGIT_MODE_8BIT_ASCII) ? TRUE : FALSE ;
      auth_data_len = num_of_digits = 
        ((cm_sms_msg_type*)cdma.dbm_buf_ptr)->address.number_of_digits; 
      auth_data_ptr = 
        (byte *) ((cm_sms_msg_type*)cdma.dbm_buf_ptr)->address.digits;
      #endif /* FEATURE_AUTH && FEATURE_AUTH_DIGITS */

      if (curr_state != (next_state = mccsa_setup_auth_msg_int_fields(
          curr_state, &mcc_tx_int_msg, burst_type, auth_data_len,
          auth_data_ptr, digit_mode, num_of_digits, &mcc_tx_int_msg.ac_burst.ami)))
      {
        /* Abort access now before it even begins */
        MSG_MED("Data Burst trans. interrupted during auth_sig", 0, 0, 0);

        mcc_send_mo_dbm_status(burst_type, CM_DBM_MO_L2_ACK_FAILURE);

        return (next_state);
      }
      break;
    #endif /* FEATURE_CDSMS */

    #ifdef FEATURE_DBM
    case CAI_POSITION_DET:

      #if defined(FEATURE_AUTH) && defined(FEATURE_AUTH_DIGITS)
      /*---------------------------------------------------
      ** The authentication procedure for the PD message
      ** will consist of 3 octets generated from the first
      ** 3 CHARi fields of the data burst message. See
      ** IS-801, 3.2.2.2
      ** We have already checked at least 3 bytes for PD
      ** message.
      **-------------------------------------------------*/
      digit_mode = FALSE;

      #ifdef FEATURE_UIM_RUIM
      num_of_digits = RUIM_RUN_CAVE_DIGITS * 8;
      #endif /* FEATURE)UIM_RUIM */

      #endif /* FEATURE_AUTH && FEATURE_AUTH_DIGITS */

      if (curr_state != (next_state = mccsa_setup_auth_msg_int_fields(
          curr_state, &mcc_tx_int_msg, burst_type, auth_data_len,
          auth_data_ptr, digit_mode, num_of_digits, &mcc_tx_int_msg.ac_burst.ami)))
      {
        /* Abort access now before it even begins */
        MSG_MED("Data Burst trans. interrupted during auth_sig", 0, 0, 0);
        mcc_send_mo_dbm_status((byte) burst_type, CM_DBM_MO_L2_ACK_FAILURE);
        return (next_state);
      }
      break;

    case CAI_SHORT_DATA_BURST:

      if (curr_state != (next_state = mccsa_setup_auth_msg_int_fields(
          curr_state, &mcc_tx_int_msg, burst_type, auth_data_len,
          auth_data_ptr, digit_mode, num_of_digits, &mcc_tx_int_msg.ac_burst.ami)))
      {
        /* Abort access now before it even begins */
        MSG_MED("Data Burst trans. interrupted during auth_sig", 0, 0, 0);

        #ifdef FEATURE_CDSMS
        mcc_send_mo_dbm_status(burst_type, CM_DBM_MO_L2_ACK_FAILURE);
        #endif /* FEATURE_CDSMS */

        return (next_state);
      }
      break;
    #endif /* FEATURE_DBM */

    default:
      /* Should never be here */
      MSG_ERROR("Invalid burst_type %d", burst_type, 0, 0);
      return(next_state);
      /* break; */
  }

  mcc_tx_int_msg.ac_burst.msg_number = 1;
  mcc_tx_int_msg.ac_burst.burst_type = burst_type;
  mcc_tx_int_msg.ac_burst.num_msgs = 1;

  for (i=0; i < data_len; i++)
  {
    mcc_tx_int_msg.ac_burst.chari[i] = *(data_ptr++);
  }

  mcc_tx_int_msg.ac_burst.num_fields = data_len;

  plt_rpt_ptr = &mcc_tx_int_msg.ac_burst.pilot_rpt;
  msg_len = mccsa_compute_independent_msg_len( &mcc_tx_int_msg, plt_rpt_ptr);
  access_aborted = mccsa_send_access_cmd(&mcc_tx_int_msg, plt_rpt_ptr,
                   (boolean) !mcc_layer2.valid_ack, msg_len, CDMA_MSG); /*lint !e641 */

  if (access_aborted == NOT_ABORT)
  {
    MSG_HIGH("Sent AC Data Burst Message", 0, 0, 0);

    /* Increment the counter for ACH/EACH messages generated by Layer 3. */
    mccsa_increment_l3_counter(counter_type);
  }

  else /* access was aborted */
  {
    /* Failed access - perform registration processing */
    mccreg_idle_ack_or_fail();

    /* Exit to System Determination */
    next_state    = CDMA_EXIT;

    if (access_aborted == ABORT_MSG_TOO_LONG)
    {
      MSG_MED("Message too large.", 0, 0, 0);

      mcc_send_mo_dbm_status((byte) burst_type, CM_DBM_MO_ACCESS_TOO_LARGE);

      mcc_exit.exit = MCC_ACCESS_FAILED;
    }

    #ifdef FEATURE_IS2000_REL_A
    else if (access_aborted == ABORT_ACCT_BLOCK)
    {
      MSG_MED("ACCT blocked.", 0, 0, 0);

      /* Other access failure case, we notify SMS */
      mcc_send_mo_dbm_status((byte) burst_type, CM_DBM_MO_ACCT_BLOCK);

      mcc_exit.exit = MCC_ACCT_BLOCKED;
    }
    #endif //FEATURE_IS2000_REL_A

    else
    {
      /* Other access failure case, we notify SMS */
      mcc_send_mo_dbm_status((byte) burst_type, CM_DBM_MO_L2_ACK_FAILURE);

      if ( access_aborted == ABORT_SRCH_REQUEST_SYS_RESTART )
      {
        mcc_exit.exit = MCC_SRCH_REQUEST_SYS_RESTART;
      }
      else
      {
       mcc_exit.exit = MCC_ACCESS_DENIED;
      }
    }
  }

  return (next_state);

} /* mccsa_build_dbm  */
#endif /* FEATURE_CDSMS || FEATURE_DBM */

/*===========================================================================

FUNCTION MCSSA_PROCESS_FEATURE_MSG

DESCRIPTION
  This function processes a Feature Notification Message.

DEPENDENCIES
  None

RETURN VALUE
  The next state to be processed.

SIDE EFFECTS
  None

===========================================================================*/

word mccsa_process_feature_msg
(
  mccdma_matched_msg_type *matched_msg_ptr,
    /* Pointer to Feature Notification Message */
  word curr_state
    /* Substate of System Access currently being processed */
)
{
  word next_state = curr_state;
    /* Next state to be processed */
  cm_mc_rpt_type *cmd_ptr;
    /* Pointer to command to send to CM task */

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  if (curr_state == CDMA_ORIG) /*lint !e641 */
  {
    if (matched_msg_ptr->pc_feat.release)
    {
      if (!matched_msg_ptr->pc_feat.hdr.ack_req)
      {
        /* First send CM release indication with reason */
        if ((cmd_ptr = cm_mc_rpt_get_buf_else_err_fatal()) != NULL)
        {
          cmd_ptr->release.hdr.cmd = CM_CALL_RELEASE_F;
          cmd_ptr->release.call_ids[0] = cdma.call_id;
          cmd_ptr->release.rel_codes[0] = CM_CALL_REL_NORMAL;
          cmd_ptr->release.num_call_ids = 1;

          mcc_cm_mc_rpt( cmd_ptr );
        }
        else
        {
          ERR("No free buffers on cm_mc_rpt_free_q",0,0,0 );
        }

        /* ----------------------------
        ** Exit to System Determination
        ** ---------------------------- */
        mcc_exit.exit = MCC_RELEASE;
        next_state    = CDMA_EXIT;
        cdma.rel_orig = FALSE;

        /* Events report: Release */
        mclog_report_release(MCLOG_RELEASE_BS_ORDER);
      }
      else
      {
        /* -------------------------------------------------------
        ** We need to send an ack before processing the layer 3 of
        ** this message.  Save the layer 3 and send a BS_ACK_ORD.
        ** ------------------------------------------------------- */
        mccsa_layer2.pending_msg = *matched_msg_ptr;
        mccsa_layer2.pending_msg_flag = TRUE;
        /* Mark that duplicate detction has been done on this msg */
        mccsa_layer2.dup_detect_done = TRUE;
      }
    }
  }

  (void) mcc_process_info_records(
                                   CM_CALL_ID_UNASSIGNED,
                                   matched_msg_ptr->pc_feat.msg_type,
                                   matched_msg_ptr->pc_feat.num_recs,
                                   (byte*)matched_msg_ptr->pc_feat.recs );

  return (next_state);

} /* end mccsa_process_feature_msg() */

/*===========================================================================

FUNCTION MCCSA_BUILD_TMSI_ASSIGN_CMP

DESCRIPTION
  This function builds a TMSI Assignment Completion Message.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
word mccsa_build_tmsi_assign_cmp
(
  word curr_state
    /* Substate of System Access currently being processed */
)
{

  word next_state = curr_state;
    /* Next state to be processed */

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  /* ---------------------------------------------
  ** Build the TMSI Assignment Completion Message
  ** --------------------------------------------- */
  mcc_tx_int_msg.ac_tmsi_cmp.msg_type = mccsa_layer2.last_msg_type =
    CAI_AC_TMSI_CMP_MSG;

  mcc_tx_int_msg.ac_tmsi_cmp.hdr.msg_seq = mcc_layer2.msg_seq = 
    (mcc_layer2.msg_seq + 1) & 0x7; /* Modulo 8 sequence number */

  mcc_tx_int_msg.ac_tmsi_cmp.hdr.ack_seq = mcc_layer2.ack_seq;
  mcc_tx_int_msg.ac_tmsi_cmp.hdr.ack_type = mcc_layer2.ack_type;
  mcc_tx_int_msg.ac_tmsi_cmp.hdr.ack_req = TRUE;
  mcc_tx_int_msg.ac_tmsi_cmp.hdr.valid_ack = TRUE;

  mcc_set_address(&mcc_tx_int_msg.ac_tmsi_cmp.hdr, mcc_tx_int_msg.ac_tmsi_cmp.msg_type);

  /* Setup authentication and message integrity fields */
  if (curr_state != (next_state = mccsa_setup_auth_msg_int_fields(curr_state, 
      &mcc_tx_int_msg, 0, 0, NULL, 0, 0, 
      &mcc_tx_int_msg.ac_tmsi_cmp.ami)))
  {
    /* Abort access now before it even begins */
    MSG_MED("TMSI Assign Cmp trans. interrupted during auth_sig", 0, 0, 0);
  }

  #ifdef FEATURE_IS2000_REL_A
  /* Setup encryption fields */

  mcc_tx_int_msg.ac_tmsi_cmp.enc.enc_fields_incl = FALSE;

  #ifdef FEATURE_IS2000_REL_A_AES
#error code not present
  #endif /* FEATURE_IS2000_REL_A_AES */

  #endif /* FEATURE_IS2000_REL_A */


  return (next_state);

} /* mccsa_build_tmsi_assign_cmp */

/*===========================================================================

FUNCTION MCCSA_PROCESS_TMSI_ASSIGN

DESCRIPTION
  This function processes a TMSI Assignment Message.

DEPENDENCIES
  None

RETURN VALUE
  The next state to be processed.

SIDE EFFECTS
  None

===========================================================================*/
word mccsa_process_tmsi_assign
(
  mccdma_matched_msg_type *matched_msg_ptr,
    /* Pointer to TMSI Assignment Message */
  word curr_state
    /* Next state to be processed */
)
{
  access_result_type access_aborted = NOT_ABORT;
    /* Flag if access attempt aborted */
  word msg_len;
    /* Length of bit-packed message */
  word next_state = curr_state;
    /* Next state to be processed */

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  /* --------------------------------------------------------------
  ** Perform Layer 3 processing of the TMSI Assignment Message
  ** -------------------------------------------------------------- */

  MSG_MED("Rxed TMSI Assignment in system access", 0, 0, 0);

  /* Save new TMSI information */
  tmsi_info_update (&matched_msg_ptr->pc_tmsi.data);

  /* Reset full TMSI timer */
  tmsi_reset_full_timer();

  /* Build TMSI Assignment Completion message */
  next_state = mccsa_build_tmsi_assign_cmp( curr_state );

  if (next_state == curr_state)
  {
    plt_rpt_ptr = &mcc_tx_int_msg.ac_tmsi_cmp.pilot_rpt;
    msg_len = mccsa_compute_independent_msg_len( &mcc_tx_int_msg, plt_rpt_ptr);
    access_aborted = mccsa_send_access_cmd( &mcc_tx_int_msg, plt_rpt_ptr, FALSE, msg_len,
                                      CDMA_ORD_RES ); /*lint !e641 */

    if (access_aborted == NOT_ABORT)
    {
      MSG_HIGH( "Sent AC TMSI Assignment Completion Message", 0, 0, 0 );

      /* Increment the counter for ACH/EACH messages generated by Layer 3. */
      mccsa_increment_l3_counter(SA_RESPONSE);
    }
    else /* access was aborted */
    {
      /* Failed access - perform registration processing */
      mccreg_idle_ack_or_fail();

      next_state = CDMA_EXIT;

      if ( access_aborted == ABORT_SRCH_REQUEST_SYS_RESTART )
      {
        mcc_exit.exit = MCC_SRCH_REQUEST_SYS_RESTART;
      }
      else
      {
        mcc_exit.exit = MCC_ACCESS_DENIED;
      }
    }
  }

  return (next_state);

} /* mccsa_process_tmsi_assign */

#if defined(FEATURE_CDSMS) || defined(FEATURE_DBM)
/*===========================================================================

FUNCTION MCCSA_PROCESS_DATA_BURST

DESCRIPTION
  This function processes a Data Burst Message.

DEPENDENCIES
  None

RETURN VALUE
  The next state to be processed.

SIDE EFFECTS
  None

===========================================================================*/
word mccsa_process_data_burst
(
  mccdma_matched_msg_type *matched_msg_ptr,
    /* Pointer to Data Burst Message */
  word curr_state
    /* Next state to be processed */
)
{
  word next_state = curr_state;
    /* Next state to be processed */
#ifdef FEATURE_PASSPORT_MODE
  cm_mc_rpt_type *cmd_ptr;
    /* Pointer to command to send to UI task */
#endif /* FEATURE_PASSPORT_MODE */

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  /* --------------------------------------------------------------
  ** Perform Layer 3 processing of the Data Burst Message
  ** -------------------------------------------------------------- */
  MSG_MED("Rxed Data Burst in system access", 0, 0, 0);

  #ifdef FEATURE_CDSMS
  /* Check if valid Data Burst SMS message before processing */
  if ((matched_msg_ptr->pc_burst.burst_type == CAI_SHORT_MSG_SERVICES) &&
      (matched_msg_ptr->pc_burst.msg_number == 1) &&
      (matched_msg_ptr->pc_burst.num_msgs == 1) &&
      (matched_msg_ptr->pc_burst.num_fields > 0))
  {
    /* Events report: SMS received */
    event_report(EVENT_SMS_RECEIVED);

    /* ------------ */
    /* SMS Message  */
    /* ------------ */

    /* Process mobile-terminated SMS message */
    cm_mt_dbm_msg((byte) CAI_SHORT_MSG_SERVICES,
                  (byte) CM_SMS_FORMAT_CDMA,
                  (byte*) matched_msg_ptr->pc_burst.chari,
                  (word) matched_msg_ptr->pc_burst.num_fields);

    return (next_state);  
  }
  #endif /* FEATURE_CDSMS */

  #ifdef FEATURE_DBM
  /* Check if valid Data Burst Position Determination message before
     processing */
  if ((matched_msg_ptr->pc_burst.burst_type == CAI_POSITION_DET) &&
     (matched_msg_ptr->pc_burst.msg_number == 1) &&
     (matched_msg_ptr->pc_burst.num_msgs == 1) &&
     (matched_msg_ptr->pc_burst.num_fields > 0))
  {
    /* ------------------------------- */
    /* Position Determination Message  */
    /* ------------------------------- */

    mcc_send_mt_dbm_data((uint8) CAI_POSITION_DET,   
      (byte*) matched_msg_ptr->pc_burst.chari,
      (uint8) matched_msg_ptr->pc_burst.num_fields);

    #ifdef FEATURE_GPSONE_NEW_L2_ACK
#error code not present
    #endif /* FEATURE_GPSONE_NEW_L2_ACK */

    return (next_state);

  }

  /* Check if valid Short Data Burst message before
     processing */
  if ((matched_msg_ptr->pc_burst.burst_type == CAI_SHORT_DATA_BURST) &&
      (matched_msg_ptr->pc_burst.msg_number == 1) &&
      (matched_msg_ptr->pc_burst.num_msgs == 1) &&
      (matched_msg_ptr->pc_burst.num_fields > 0))
  {
    /* ------------------------------- */
    /* Short Data Burst Message  */
    /* ------------------------------- */

    mcc_send_mt_dbm_data((uint8) CAI_SHORT_DATA_BURST,   
      (byte*) matched_msg_ptr->pc_burst.chari,
      (uint8) matched_msg_ptr->pc_burst.num_fields );

    #ifdef FEATURE_SO_8026
#error code not present
    #endif /* FEATURE_SO_8026 */

    return (next_state);
  }
  #endif /* FEATURE_DBM */

  #ifdef FEATURE_PASSPORT_MODE
  /* Check for an Extended Burst Type - International Data Burst */
  if (mcc_passport_mode_is_jcdma() &&
      (matched_msg_ptr->pc_burst.burst_type == CAI_EXTENDED_BURST_TYPE_INTL))
  {
    /* ----------------------------------- */
    /* Extended Burst Type - International */
    /* ----------------------------------- */
    if ((cmd_ptr = cm_mc_rpt_get_buf_else_err_fatal()) != NULL)
    {
      cmd_ptr->hdr.cmd = CM_EXT_BRST_INTL_F;
      cmd_ptr->ext_brst_intl_rec.mcc =
        b_unpackw( matched_msg_ptr->pc_burst.chari,
          FPOS( cai_ext_brst_intl_type, mcc ),
          FSIZ( cai_ext_brst_intl_type, mcc ));

      cmd_ptr->ext_brst_intl_rec.db_subtype =
        b_unpackb( matched_msg_ptr->pc_burst.chari,
          FPOS( cai_ext_brst_intl_type, db_subtype ),
          FSIZ( cai_ext_brst_intl_type, db_subtype ));

      cmd_ptr->ext_brst_intl_rec.chg_ind =
        b_unpackb( matched_msg_ptr->pc_burst.chari,
          FPOS( cai_ext_brst_intl_type, chg_ind ),
          FSIZ( cai_ext_brst_intl_type, chg_ind ));

      cmd_ptr->ext_brst_intl_rec.sub_unit =
        b_unpackb( matched_msg_ptr->pc_burst.chari,
          FPOS( cai_ext_brst_intl_type, sub_unit ),
          FSIZ( cai_ext_brst_intl_type, sub_unit ));

      cmd_ptr->ext_brst_intl_rec.unit =
        b_unpackb( matched_msg_ptr->pc_burst.chari,
          FPOS( cai_ext_brst_intl_type, unit ),
          FSIZ( cai_ext_brst_intl_type, unit ));

      mcc_cm_mc_rpt( cmd_ptr );
    }
    else
    {
      ERR("No buffers on cm_mc_rpt_free_q", 0, 0, 0);
    }
  }
  #endif /* FEATURE_PASSPORT_MODE */

  /* ----------------------------------------------------------------------
  ** We only get here if we did not recognize the Data Burst as one we know
  ** how to process. Just return for now.
  ** ---------------------------------------------------------------------- */
  return (next_state);

} /* mccsa_process_data_burst */
#endif /* FEATURE_CDSMS || FEATURE_DBM */

/*===========================================================================

FUNCTION MCCSA_PROCESS_STREQ

DESCRIPTION
  This function processes a Status Request Message

DEPENDENCIES
  None

RETURN VALUE
  The next state to be processed.

SIDE EFFECTS
  None

===========================================================================*/
word mccsa_process_streq
(
  mccdma_matched_msg_type *matched_msg_ptr,
    /* Pointer to Status Request Message */
  word curr_state
    /* Next state to be processed */
)
{
  caii_pc_streq_type  *req_ptr = &matched_msg_ptr->pc_streq;
    /* Pointer to status request message */
  caii_ac_strsp_type  *rsp_ptr = &cdma_strsp;
    /* Pointer to status response message */
  int rec_siz;
    /* For receiving status from called function */
  word all_rec_siz;
    /* For computing cumulative length of all records */
  word i;
    /* For indexing through rec information types */
  word next_state = curr_state;
    /* Next state to be processed */
  boolean ok;
    /* For detecting problems in loop */
  byte reject_reason = CAI_REJ_UNSP;
    /* Reason for rejecting status request message */
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  /* --------------------------------------------------------------
  ** Perform Layer 3 processing of the Status Request Message
  ** -------------------------------------------------------------- */

  MSG_MED( "Rxed Status Request in system access", 0, 0, 0 );

  if (P_REV_IS_4_OR_GREATER)
  {
     rsp_ptr->msg_type = CAI_AC_EXT_STRSP_MSG;
  }
  else
  {
     rsp_ptr->msg_type = CAI_AC_STRSP_MSG;
  }

  rsp_ptr->qual     = req_ptr->qual;
  rsp_ptr->band     = req_ptr->band;
  rsp_ptr->op_mode  = req_ptr->op_mode;
  rsp_ptr->num_recs = 0;

  /* -----------------------------------------------------
  ** If QUAL_INFO_TYPE doesn't include a field, it implies
  ** the current band class or operating mode.
  ** ----------------------------------------------------- */
  if (req_ptr->qual == CAI_NO_QUAL)
  {
    switch (mcc_get_group_for_band_class(cdma.band_class))
    {
      case MCC_CELLULAR_GROUP:
      {
        /* We need to set band and op_mode to CELLULAR (IS95 BC0)*/
        req_ptr->band = cdma.band_class;      
        req_ptr->op_mode = CAI_IS95_CDMA_BC0_OP_MODE;
        /* Notice that CAI_IS95_CDMA_OP_MODE is the same as
           CAI_IS95_CDMA_BC0_OP_MODE */
        break;

      }
      case MCC_PCS_GROUP:
      {
        /* We need to set band and op_mode to JSTD008 (IS95 BC1) */
        req_ptr->band = cdma.band_class;
        req_ptr->op_mode = CAI_IS95_CDMA_BC1_OP_MODE;
        /* Notice that CAI_J_STD_008_OP_MODE is the same as
           CAI_IS95_CDMA_BC1_OP_MODE */
        break;
      }
      default:
      {
        ERR("Unsupported band class=%d",mc_get_band_class(),0,0);
      }
    }                      
  }
  else if (req_ptr->qual == CAI_BAND_QUAL)
  {                                        /*lint -e506 */
    switch (mcc_get_group_for_band_class(cdma.band_class))
    {
      case MCC_CELLULAR_GROUP:
      {
        /* We need to set the op_mode to IS95 CDMA */
        req_ptr->op_mode = CAI_IS95_CDMA_BC0_OP_MODE;
        /* Notice that CAI_IS95_CDMA_OP_MODE is the same as
           CAI_IS95_CDMA_BC0_OP_MODE */
        break;

      }
      case MCC_PCS_GROUP:
      {
        /* We need to set the op_mode to JSTD008 */
        req_ptr->op_mode = CAI_IS95_CDMA_BC1_OP_MODE;
        /* Notice that CAI_J_STD_008_OP_MODE is the same as
           CAI_IS95_CDMA_BC1_OP_MODE */
        break;
      }
      default:
      {
        ERR("Unsupported band class=%d",mc_get_band_class(),0,0);
      }
    }                        
  }

  /* -------------------------
  ** Build information records
  ** ------------------------- */

  all_rec_siz = 0;
  ok = TRUE;

  if (req_ptr->num_fields == 0)
  {
    /* We can't send a response with no info, so we must reject */
    ok = FALSE;
  }

  for ( i = 0; i < req_ptr->num_fields; i++ )
  {
    rec_siz =
      mcc_info_rec ( TRUE, /* paging channel */
                     req_ptr->band,
                     req_ptr->op_mode,
                     req_ptr->rec_typ[ i ],
                     (void *) &rsp_ptr->recs[ all_rec_siz/sizeof(dword) ],
                     (word)(sizeof(rsp_ptr->recs) - all_rec_siz),
                     &reject_reason );

    if ( rec_siz < 0 )
    {
      ok = FALSE;
      break;
    }

    else if ( rec_siz == 0 )
    {
      break;
    }

    else
    {
      all_rec_siz += (word) rec_siz;
      //We do not want to cast away a non-dword aligned pointer into our struct, so adjust
      //accordingly
      all_rec_siz = (all_rec_siz%4)?(all_rec_siz+(sizeof(dword))-all_rec_siz%4):all_rec_siz;
      rsp_ptr->num_recs++;
    }
  } /* for */

  if ( ok )
  {
    if (P_REV_IS_4_OR_GREATER)
    {
      cdma.sa_resp_msg = CAI_AC_EXT_STRSP_MSG;
    }
    else
    {
      cdma.sa_resp_msg = CAI_AC_STRSP_MSG;
    }

    next_state = mccsa_send_order_response_msg( curr_state, FALSE );
  }
  else
  {
    next_state = mccsa_send_reject( curr_state, reject_reason, matched_msg_ptr );
  } /* end if (ok) */

  return (next_state);

} /* mccsa_process_streq */

/*===========================================================================

FUNCTION  mccsa_check_if_chan_assgn_is_okay

DESCRIPTION
  This function checks to see if a received channel assignment is okay
DEPENDENCIES
  None

RETURN VALUE
  The next state to be processed.

SIDE EFFECTS
  status field that is passed in by address is modified

===========================================================================*/

word mccsa_check_if_chan_assgn_is_okay
(
  mccdma_matched_msg_type  *msg_ptr, //Pointer to matched message
  word curr_state , // Current system access sub state
  boolean *status
)
{
  mccdma_ca_ord_type *ca_ptr;  /* local variable to make copying easier */
  byte asgn_mode;  /* The assign mode of the Channel Assignment */
  byte msg_type;
  word next_state = curr_state;

  *status = TRUE;

  /* ----------------------------------------
  ** We only process channel assignments from
  ** Page Response and Origination Substates.
  ** ---------------------------------------- */

  /* -----------------------------------------------------------
  ** We determine here if we are able to honor this assignment,
  ** or if we want to reject it.  We check for this here to make
  ** sure we don't send a layer 2 ack then reject it later.
  ** ----------------------------------------------------------- */

  /* First assign some local variables to make the code more readable */
  msg_type = msg_ptr->chnasn.msg_type;
  asgn_mode = msg_ptr->chnasn.assign_mode;
  ca_ptr = &msg_ptr->chnasn.mode;

  #ifdef FEATURE_IS2000_REL_D
#error code not present
  #endif /* FEATURE_IS2000_REL_D */

  if ((curr_state != CDMA_ORIG) && (curr_state != CDMA_PAGE_RES)) /*lint !e641 */
  {
    /* We are not in page resp or orig substate, why process chan assign */
    *status = FALSE; 
    return next_state;
  }

  /* ***********************
     Obsolete message or not
     *********************** */

  #ifdef FEATURE_IS2000_REL_B
  if (P_REV_IS_8_OR_GREATER && (msg_type == CAI_CHN_ASN_MSG)
      /* Not doing P_REV check in the middle of handoff */
      && mcc_is_aho_ovhd_msg_okay()
     )
  {
    /* Obsolete CAM for Release B and onwards */
    MSG_HIGH("Reject CAM, P_REV in use >= 8", 0, 0, 0);
    next_state = mccsa_send_reject(curr_state, CAI_REJ_CODE, msg_ptr);
    *status = FALSE;
    return (next_state);
  }
  #endif /* FEATURE_IS2000_REL_B */

  if (!P_REV_IS_4_OR_GREATER && (msg_type == CAI_EXT_CHN_ASN_MSG)
      /* Not doing P_REV check in the middle of handoff */
      && mcc_is_aho_ovhd_msg_okay()
     )
  {
    /* ECAM is only for IS95B and onwards */
    MSG_HIGH("Reject ECAM, P_REV in use < 4", 0, 0, 0);
    next_state = mccsa_send_reject(curr_state, CAI_REJ_CODE, msg_ptr);
    *status = FALSE;
    return (next_state);
  }

  #ifdef FEATURE_MEID_SUPPORT
  /* -----------------------------------------------------------------------
  ** Do some error-checking on the MEID Extended Channel Assignment Message: 
  ** the p-rev-in-use must be between 6 and 8, the phone must have an MEID, 
  ** and the assign_mode must be 4.  
  ** ----------------------------------------------------------------------- */
  if (msg_type == CAI_MEID_EXT_CHN_ASN_MSG)
  {
    if ( mcc_is_aho_ovhd_msg_okay() && 
         /* Not doing P_REV check in the middle of handoff */
         ( !P_REV_IS_6_OR_GREATER 
           #ifdef FEATURE_IS2000_REL_C
           || P_REV_IS_9_OR_GREATER
           #endif /* FEATURE_IS2000_REL_C */
       ) )
    {
      MSG_ERROR("Reject MEID ECAM, P_REV in use < 6 or > 8",0,0,0);
      *status = FALSE;
      return mccsa_send_reject( curr_state, CAI_REJ_STRUCT, msg_ptr );
    }
    if (!mccap_is_meid_supported())
    {
      MSG_ERROR("Reject MEID ECAM, mobile has no MEID",0,0,0);
      *status = FALSE;
      return mccsa_send_reject( curr_state, CAI_REJ_CAP, msg_ptr );
    }

    if (asgn_mode != CAI_EXT_TRAF_CHAN_ASSIGN)
    {
      MSG_ERROR("Reject MEID ECAM, Assign_mode not 4",0,0,0);
      *status = FALSE;
      return mccsa_send_reject( curr_state, CAI_REJ_FIELD, msg_ptr );
    }
  }
  #endif /* FEATURE_MEID_SUPPORT */

  #ifdef FEATURE_PASSPORT_MODE
  if(mcc_passport_mode_is_jcdma())
  {
    /*************
     T53 specific
     ************ */
    if ((cdma.band_class == CAI_BAND_CLASS3) ||
        (cdma.band_class == CAI_BAND_CLASS6))
    {
      if ((msg_type == CAI_CHN_ASN_MSG) &&
          ((asgn_mode == CAI_TRAFFIC_CHAN_ASSIGN) ||
           (asgn_mode == CAI_PAGING_CHAN_ASSIGN)))
      {
        /* Above Assignments are not supported by JCDMA phones
          This means service option negotiation isn't allowed */
        next_state = mccsa_send_reject( curr_state, CAI_REJ_FIELD, msg_ptr );
        *status = FALSE;
        return(next_state);
      }
    }
  }
  #endif /* FEATURE_PASSPORT_MODE */

  /* **************************
     Analog Assignment specific 
     ************************** */

  #ifndef FEATURE_ACP
  if ((asgn_mode == CAI_ACQ_ANALOG_SYSTEM) ||
      (asgn_mode == CAI_ANALOG_VCHAN_ASSIGN))
  {
    /* Assignment to Analog is not supported by PCS only phone*/
    next_state = mccsa_send_reject( curr_state, CAI_REJ_CAP, msg_ptr );
    *status = FALSE;
    return (next_state);
  }
  #endif/* !FEATURE_ACP */

  if (asgn_mode == CAI_ACQ_ANALOG_SYSTEM)
  {
    if (!mcc_is_band_class_in_group(MCC_CELLULAR_GROUP, ca_ptr->am2.band_class))
    {
      /* Analog Assignment to only BAND_CELL is supported */
      next_state = mccsa_send_reject( curr_state, CAI_REJ_CAP, msg_ptr );
      *status = FALSE;
      return (next_state);
    }
  }

  if (asgn_mode == CAI_ANALOG_VCHAN_ASSIGN)
  {
    if (!mcc_is_band_class_in_group(MCC_CELLULAR_GROUP, ca_ptr->am3.band_class))
    {
      /* Analog Assignment to only BAND_CELL is supported */
      next_state = mccsa_send_reject( curr_state, CAI_REJ_CAP, msg_ptr );
      *status = FALSE;
      return (next_state);
    }

    if (ca_ptr->am3.an_chan_type != CAI_ANALOG_WIDE_CHAN)
    {
      /* Only Wide Analog is supported */
      next_state = mccsa_send_reject( curr_state, CAI_REJ_CAP, msg_ptr );
      *status = FALSE;
      return (next_state);
    }
  }

  /* **************************
     Paging Assignment specific 
     ************************** */

  if ((asgn_mode == CAI_PAGING_CHAN_ASSIGN) || 
      ((msg_type == CAI_CHN_ASN_MSG) && 
       (asgn_mode == CAI_EXT_PAGING_CHAN_ASSIGN)))
  {
    if (ca_ptr->am1.num_pilots == 0)
    {
      /* --------------------------------------
      ** Paging Channel Assignment must include
      ** at least one pilot, so we reject it.
      ** -------------------------------------- */
      next_state = mccsa_send_reject(curr_state, CAI_REJ_FIELD, msg_ptr);
      *status = FALSE;
      return (next_state);
    }

    if (ca_ptr->am1.freq_incl &&
        !mcc_is_band_class_supported(ca_ptr->am1.band_class))
    {
      /* PC Assignment to this Band Class is not supported */
      next_state = mccsa_send_reject( curr_state, CAI_REJ_CAP, msg_ptr );
      *status = FALSE;
      return (next_state);
    }
  }

  /* ***************************
     Traffic Assignment specific 
     *************************** */
  if ((asgn_mode == CAI_TRAFFIC_CHAN_ASSIGN) ||
      (asgn_mode == CAI_EXT_TRAF_CHAN_ASSIGN)
      #ifdef FEATURE_IS2000_REL_C
      || ((msg_type == CAI_EXT_CHN_ASN_MSG) &&
          (asgn_mode == CAI_PACKET_DATA_TRAF_CHAN_ASSIGN))
      #endif /* FEATURE_IS2000_REL_C */
     )
  {
    uint8 rej_reason = mcc_check_if_traf_chan_assgn_is_okay(&msg_ptr->chnasn);

    if (rej_reason) 
    {
      next_state = mccsa_send_reject(curr_state, rej_reason, msg_ptr);
      *status = FALSE;
      return ( next_state );
    }
  }

  return next_state;

} /* mccsa_check_if_chan_assgn_is_okay */

/*===========================================================================

FUNCTION MCCSA_PROCESS_LAYER3

DESCRIPTION
  This function processes the layer 3 of messages (other than page messages).

DEPENDENCIES
  None

RETURN VALUE
  The next state to be processed.

SIDE EFFECTS
  None

===========================================================================*/

word mccsa_process_layer3
(
  mccdma_matched_msg_type *matched_msg_ptr,
    /* Pointer to message for which layer 3 processing is needed. */
  word curr_state
    /* Substate of System Access currently being processed */
)
{
  cm_mc_rpt_type *cmd_ptr;
    /* Pointer to command to send to UI task for NDSS */

  word next_state = curr_state;

  #ifdef FEATURE_AUTH
  dword randbs;
    /* Random number sent in base station challenge order */
  auth_cmd_type *auth_cmd_ptr;
    /* Pointer to authentication command */

  #ifdef FEATURE_UIM_RUIM
  auth_rpt_type *auth_rpt_ptr;
    /* Pointer to the AUTH report. */
  #endif /* FEATURE_UIM_RUIM */

  #endif /* FEATURE_AUTH */

  boolean status = TRUE;

  mcc_redir_info_type redir_info; /* Redirection info */
  sd_redir_rec_s_type curr_redir_rec;
  uint8 chan_index;

  #ifdef FEATURE_IS2000_REL_C_AKA
#error code not present
  #endif /* FEATURE_IS2000_REL_C_AKA */
  
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  /* -----------------------------------------
  ** Process Layer 3 of Paging Channel message
  ** ----------------------------------------- */
  switch (matched_msg_ptr->gen_pc.msg_type)
  {
    case CAI_PC_ORD_MSG:
      /* ----------------------------
      ** Paging Channel Order Message
      ** ---------------------------- */
      next_state = mccsa_process_order_msg( matched_msg_ptr, curr_state );
      break;

    case CAI_CHN_ASN_MSG:
      /* --------------------------
      ** Channel Assignment Message
      ** -------------------------- */
    case CAI_EXT_CHN_ASN_MSG:

    #ifdef FEATURE_MEID_SUPPORT
    case CAI_MEID_EXT_CHN_ASN_MSG:
    #endif /* FEATURE_MEID_SUPPORT */
      /* -----------------------------------
      ** Extended Channel Assignment Message
      ** ----------------------------------- */
      next_state = mccsa_check_if_chan_assgn_is_okay(matched_msg_ptr, 
                     curr_state, &status);
      if (status)
      {
        if (matched_msg_ptr->chnasn.hdr.ack_req)
        {
          /* -------------------------------------------------------
          ** We need to send an ack before processing the layer 3 of
          ** this message.  Save the layer 3 and send a BS_ACK_ORD.
          ** ------------------------------------------------------- */
          mccsa_layer2.pending_msg = *matched_msg_ptr;
          mccsa_layer2.pending_msg_flag = TRUE;
          /* Mark that duplicate detction has been done on this msg */
          mccsa_layer2.dup_detect_done = TRUE;
        }
        else 
        {
          next_state = mccsa_process_chan_assign_msg( matched_msg_ptr, curr_state );
        }
      }
      break;

    case CAI_FEATURE_MSG:
      /* ----------------------------
      ** Feature Notification Message
      ** ---------------------------- */
      if (curr_state != CDMA_MSG) /*lint !e641 */
      {
        /* -----------------------------------------------
        ** We do not process Feature Notification messages
        ** from Message Transmission substate.
        ** ----------------------------------------------- */
        next_state = mccsa_process_feature_msg( matched_msg_ptr, curr_state );
      }
      break;

    case CAI_PC_AUTH_MSG:
      /* --------------------------------
      ** Authentication Challenge Message
      ** -------------------------------- */
      MSG_HIGH( "Rxed Authentication Challenge Message", 0, 0, 0 );

      #ifdef FEATURE_IS2000_REL_D
#error code not present
      #endif /* FEATURE_IS2000_REL_D */

      #ifdef FEATURE_AUTH

      #if ((defined FEATURE_IS2000_REL_C_AKA) && (defined FEATURE_IS2000_REL_D))
#error code not present
      #else /* AKA && REL_D */
      cdma.sa_resp_msg = CAI_AC_AUTH_RESP_MSG;
      cdma.randu = matched_msg_ptr->pc_auth.randu;
      next_state = mccsa_send_order_response_msg( curr_state, FALSE );
      #endif /* FEATURE_IS2000_REL_C_AKA && FEATURE_IS2000_REL_D */      
      
      #else /* AUTH */

      /* No AUTH support, reject the message */
      MSG_HIGH( "No AUTH support in MS!", 0, 0, 0 );
      next_state = mccsa_send_reject( curr_state, CAI_REJ_CAP,
                                matched_msg_ptr );
      #endif /* FEATURE_AUTH */

      break;

    case CAI_PC_SSD_UP_MSG:
      /* ------------------
      ** SSD Update Message
      ** ------------------ */
      MSG_HIGH( "Rxed SSD Update Message", 0, 0, 0 );

      #ifdef FEATURE_AUTH

      /* Get a random number for the Base Station Challange. */
      randbs = ran_next();

      #ifdef FEATURE_UIM_RUIM

      #if defined( FEATURE_UIM_RUN_TIME_ENABLE )
      /* Determine if the R-UIM is available for this NAM */
      if (nv_rtre_use_ruim_for_nam(cdma.curr_nam))
      #endif /* FEATURE_UIM_RUN_TIME_ENABLE */
      {
        /* Get an auth command buffer. */
        if ((auth_cmd_ptr = (auth_cmd_type*) q_get( &auth_free_q )) != NULL)
        {
          /* Send the BS challenge command.  */
          auth_cmd_ptr->hdr.command = AUTH_BS_CHAL_F;
          auth_cmd_ptr->hdr.cmd_hdr.task_ptr = NULL;
          /* Use the random number as the seed*/
          auth_cmd_ptr->chal.randseed = randbs;
          /* Use the Auth reportback function. */
          auth_cmd_ptr->chal.rpt_function = mcc_queue_auth_rpt;

          /* Using the Auth queue signal */
          (void) rex_clr_sigs(&mc_tcb, MCC_AUTH_Q_SIG);

          /* Send the command and wait for the initial status */
          mcc_auth_cmd( auth_cmd_ptr );

          /* Check the initial response from auth */
          if ( auth_cmd_ptr->hdr.status == AUTH_RECEIVED_S )
          {
            /* Wait for the command to be done */
            /* Note that we are waiting on MCC_AUTH_Q_SIG */
            (void) mcc_wait( MCC_AUTH_Q_SIG );

            /* Retrieve the report from the auth report queue */
            auth_rpt_ptr = (auth_rpt_type*) q_get( &mc_auth_q );

            /* Did the command compete without any problems. */
            if (auth_rpt_ptr->rpt_status == UIM_PASS)
            {
              /* Process the RANDBS result */
              randbs = auth_rpt_ptr->rpt.randbs;
            }
            else /* report was invalid */
            {
              ERR( "Failure Report from AUTH!", 0, 0, 0 );
              /* Use zero as randbs.  This should fail the SSD update. */
              randbs = 0;
            } /* end if - check the status of the AUTH command. */

            /* Place the AUTH report buffer back on the free queue. */
            if (auth_rpt_ptr != NULL)
            {
              cmd_done( &auth_rpt_ptr->rpt_hdr );
            }
          }
          else /* Status returned from command was an error condition */
          {
            ERR( "Bad status from AUTH %d", auth_cmd_ptr->hdr.status, 0, 0 );
            /* Use zero as randbs.  This should fail the SSD update. */
            randbs = 0;
          } /* end if - the BS challenge command was received by auth */
        }
        else /* Unable to get an auth command buffer from the free queue. */
        {
          ERR( "No free buffers on auth_free_q", 0, 0, 0 );
          /* Use zero as randbs.  This should fail the SSD update. */
          randbs = 0;
        } /* Got a AUTH cmd buffer from the free queue. */
      }
      #endif /* FEATURE_UIM_RUIM */

      if ((auth_cmd_ptr = (auth_cmd_type*) q_get( &auth_free_q )) != NULL)
      {
        #ifdef FEATURE_UIM_RUIM

        #if defined( FEATURE_UIM_RUN_TIME_ENABLE )
        /* Determine if the R-UIM is available for this NAM */
        if (nv_rtre_use_ruim_for_nam(cdma.curr_nam))
        #endif /* FEATURE_UIM_RUN_TIME_ENABLE */
        {
          auth_cmd_ptr->hdr.command = AUTH_SSD_UPDATE_F;
          qw_equ (auth_cmd_ptr->ssd.randssd, matched_msg_ptr->pc_ssd.randssd);
          auth_cmd_ptr->ssd.process_control = RUIM_DEFLT_PROC_CNTRL;
          auth_cmd_ptr->ssd.esn = cdma.esn;
        }
        #if defined( FEATURE_UIM_RUN_TIME_ENABLE )
        else /* R-UIM is not available */
        #endif /* FEATURE_UIM_RUN_TIME_ENABLE */

        #endif /* FEATURE_UIM_RUIM */

        #if !defined( FEATURE_UIM_RUIM ) || defined( FEATURE_UIM_RUN_TIME_ENABLE )
        {
          auth_cmd_ptr->hdr.command = AUTH_SSD_UPDATE_F;
          auth_cmd_ptr->ssd.auth_data = mcc_get_auth_imsi_s1();
          auth_cmd_ptr->ssd.randbs = randbs;
          qw_equ (auth_cmd_ptr->ssd.randssd, matched_msg_ptr->pc_ssd.randssd);
        }
        #endif /* FEATURE_UIM_RUIM */

        mcc_auth_cmd( auth_cmd_ptr );
        if ( auth_cmd_ptr->hdr.status == AUTH_RECEIVED_S )
        {
          cdma.sa_resp_msg = CAI_AC_ORD_MSG;
          cdma.sa_resp_ord.chal.order = CAI_BS_CHAL_ORD;
          cdma.sa_resp_ord.chal.ordq = 0;
          cdma.sa_resp_ord.chal.randbs = randbs;
          next_state = mccsa_send_order_response_msg( curr_state, FALSE );
        }
        else
        {
          ERR( "Bad status from AUTH %d", auth_cmd_ptr->hdr.status, 0, 0 );
          next_state = mccsa_send_reject( curr_state, CAI_REJ_UNSP,
                                    matched_msg_ptr );
        }
      }
      else
      {
        ERR( "No free buffers on auth_free_q", 0, 0, 0 );
        next_state = mccsa_send_reject( curr_state, CAI_REJ_UNSP,
                                  matched_msg_ptr );
      }

      #else

      /* No AUTH support, reject the message */
      MSG_HIGH( "No AUTH support in MS!", 0, 0, 0 );
      next_state = mccsa_send_reject( curr_state, CAI_REJ_CAP,
                                matched_msg_ptr );
      #endif /* FEATURE_AUTH */

      break;

    case CAI_PC_TMSI_ASGN_MSG:
      /* -----------------------
      ** TMSI Assignment Message
      ** ----------------------- */
      MSG_HIGH( "Rxed TMSI Assignment Message in SA", 0, 0, 0 );
      next_state = mccsa_process_tmsi_assign( matched_msg_ptr, curr_state );
      break;

    case CAI_PC_BURST_MSG:
      /* ------------------
      ** Data Burst Message
      ** ------------------ */
      MSG_HIGH( "Rxed Data Burst Message", 0, 0, 0 );

      #if (defined(FEATURE_CDSMS) || defined(FEATURE_DBM))
      next_state = mccsa_process_data_burst( matched_msg_ptr, curr_state );
      #endif /* FEATURE_CDSMS || FEATURE_DBM */

      break;

    case CAI_PC_STREQ_MSG:
      /* ----------------------
      ** Status Request Message
      ** ---------------------- */
      MSG_HIGH( "Rxed Status Request Message", 0, 0, 0 );
      next_state = mccsa_process_streq( matched_msg_ptr, curr_state );
      break;

    case CAI_SERVICE_REDIR_MSG:
      /* ---------------------------
      ** Service Redirection Message
      ** --------------------------- */
      MSG_HIGH( "Rxed Service Redirection Msg", 0, 0, 0 );

      /* If the MS is redirected to an unsupported mode or
      ** band class, MS will resond back with a Reject Order  */
      if ( !mcc_check_if_csch_srdm_is_okay( &mccsa_matched_msg.serv_redir ) )
      {
        next_state = mccsa_send_reject( curr_state, CAI_REJ_CAP, matched_msg_ptr );
      }
      else  /* This message is not rejected so let's process it */
      {
        if (!matched_msg_ptr->serv_redir.hdr.ack_req)
        {
          /* If the record type the redirection is an NDSS off indication
             exit to System Determination with MCC_NDSS_OFF */
          if (mccsa_matched_msg.serv_redir.record_type == CAI_NDSS_OFF_INDICATION )
          {
            /* Reset TMSI timers and code if delete TMSI flag */
            /* has been set.                                  */
            if (mccsa_matched_msg.serv_redir.delete_tmsi)
            {
              tmsi_code_and_timer_reset();
            }

            next_state = CDMA_EXIT;
            mcc_exit.exit = MCC_NDSS_OFF;

            /* Events report: NDSS off */
            event_report(EVENT_NDSS_OFF);
          }
          else
          {
            curr_redir_rec.record_type = (sd_redir_rec_type_e_type)mccsa_matched_msg.serv_redir.record_type;
            curr_redir_rec.is_delete_tmsi = mccsa_matched_msg.serv_redir.delete_tmsi;

            if (curr_redir_rec.record_type == CAI_CDMA_REDIRECTION_REC) /*lint !e641 */
            {
              curr_redir_rec.rec.cdma_rec.band_class = 
                (sys_band_class_e_type)mccsa_matched_msg.serv_redir.redir.rec2.band_class;
              curr_redir_rec.rec.cdma_rec.expected_sid = 
                mccsa_matched_msg.serv_redir.redir.rec2.expected_sid;
              curr_redir_rec.rec.cdma_rec.expected_nid = 
                mccsa_matched_msg.serv_redir.redir.rec2.expected_nid;
              curr_redir_rec.rec.cdma_rec.num_chans = 
                mccsa_matched_msg.serv_redir.redir.rec2.num_chans;

              for (chan_index = 0; chan_index < mccsa_matched_msg.serv_redir.redir.rec2.num_chans; 
                   chan_index++) 
              {
                curr_redir_rec.rec.cdma_rec.chan[chan_index] = 
                  mccsa_matched_msg.serv_redir.redir.rec2.cdma_chan[chan_index];
              }
            }
            else if (curr_redir_rec.record_type == CAI_ANALOG_REDIRECTION_REC) /*lint !e641 */
            {
              curr_redir_rec.rec.amps_rec.expected_sid = 
                mccsa_matched_msg.serv_redir.redir.rec1.expected_sid;
              curr_redir_rec.rec.amps_rec.is_ignore_cdma =
                mccsa_matched_msg.serv_redir.redir.rec1.ignore_cdma;
              curr_redir_rec.rec.amps_rec.sys_ord =
                (sd_ss_sys_ord_e_type)mccsa_matched_msg.serv_redir.redir.rec1.sys_ordering;
              curr_redir_rec.rec.amps_rec.max_redirect_delay = 0;
            }

            redir_info.return_if_fail = mccsa_matched_msg.serv_redir.return_if_fail;
            redir_info.num_recs = 1;
            redir_info.redir_rec_list = &curr_redir_rec;
           
            /* ------------------------------------------
            ** Inform system determination of redirection
            ** ------------------------------------------ */
            if ((curr_state == CDMA_ORIG) && /*lint !e641 */
                (mccsa_matched_msg.serv_redir.redirect_type == CAI_NDSS_REDIRECTION))
            {
              if (!mcc_inform_sd_redirection(SD_SS_REDIR_CDMA_NDSS,
                  &redir_info))
              {
                next_state = mccsa_send_reject(curr_state, CAI_REJ_CAP,
                  matched_msg_ptr);

                return (next_state);
              }
            }
            else if (!mcc_inform_sd_redirection(SD_SS_REDIR_CDMA_SRDM, &redir_info))
            {
              next_state = mccsa_send_reject(curr_state, CAI_REJ_CAP,
                matched_msg_ptr);

              return (next_state);
            }

            /* Follow the Redirection */

            /* If Service Redirection is received as a response
               to an origination and redirect type indicates NDSS
               exit to system determination with dialed digits. */
            if ((curr_state == CDMA_ORIG) && /*lint !e641 */
                (mccsa_matched_msg.serv_redir.redirect_type == CAI_NDSS_REDIRECTION))
            {
              /* Send NDSS Redirection command to UI  */
              if ((cmd_ptr = cm_mc_rpt_get_buf_else_err_fatal()) != NULL)
              {
                cmd_ptr->ndss_redirection.hdr.cmd = CM_NDSS_REDIRECTION_F;
                cmd_ptr->ndss_redirection.call_id = cdma.call_id;
                mcc_cm_mc_rpt( cmd_ptr );
              }
              else
              {
                ERR("No free buffers on ui_cmd_free_q",0,0,0 );
              }
            }

            /* -------------------------------------------------------
            ** The mobile station shall enter the System Determination
            ** Substate with a redirection indication.
            ** ------------------------------------------------------- */
            next_state = CDMA_EXIT;
            mcc_exit.exit = MCC_REDIRECTION;


            /* Events report: Redirection */
            event_report(EVENT_REDIRECTION);

          }  /* end CAI_NDSS_OFF_INDICATION */
        } /* (!matched_msg_ptr->serv_redir.hdr.ack_req) */
        else
        {
          /* -------------------------------------------------------
          ** We need to send an ack before processing the layer 3 of
          ** this message.  Save the layer 3 and send a BS_ACK_ORD.
          ** ------------------------------------------------------- */
          mccsa_layer2.pending_msg = *matched_msg_ptr;
          mccsa_layer2.pending_msg_flag = TRUE;
          /* Mark that duplicate detction has been done on this msg */
          mccsa_layer2.dup_detect_done = TRUE;
        }
      } /* non-rejected message processing */

      break;

    case CAI_PACA_MSG:
      /* ----------------------
      ** PACA Message
      ** ---------------------- */
      MSG_HIGH( "Rxed PACA Message in System Access State", 0, 0, 0 );

      #ifdef FEATURE_IS95B_PACA
      /* PACA Message processing would occur here */
      MSG_MED ("PACA Not Currently Supported", 0, 0, 0);
      #else
      /* The PACA message is valid only in the following substates */
      if ((curr_state == CDMA_UPDATE_OVH) || (curr_state == CDMA_ORIG) || /*lint !e641 */
          (curr_state == CDMA_REG) || (curr_state == CDMA_MSG)) /*lint !e641 */
      {
        next_state = mccsa_send_reject(curr_state, CAI_REJ_CAP, matched_msg_ptr);
      }

      /************************************************************************/
      /* For all other substates, just let the PACA Message fall on the floor */
      /************************************************************************/
      #endif /* FEATURE_IS95B_PACA */

      break;

    #ifdef FEATURE_IS2000_REL_A
    case CAI_SEC_MODE_CMD_MSG:
      /* ----------------------
      ** SECURITY MODE COMMAND Message
      ** ---------------------- */
      MSG_HIGH( "Rxed Security Mode Command Message in System Access State", 0, 0, 0 );

      #ifdef FEATURE_IS2000_REL_A_AES
#error code not present
      #else
      if (matched_msg_ptr->sec_mode_cmd.c_sig_encrypt_mode !=
          CAI_SIG_ENC_MODE_DISABLED)
      {
        /* We don't support Signaling Encryption right now */
        MSG_HIGH("c_sig_encrypt_mode %d not supported", matched_msg_ptr->sec_mode_cmd.c_sig_encrypt_mode, 0, 0);
        next_state = mccsa_send_reject(curr_state, CAI_REJ_CAP, matched_msg_ptr);
      }
      #endif /* FEATURE_IS2000_REL_A_AES */

      #if defined(FEATURE_IS2000_REL_C_AKA) || defined(FEATURE_IS2000_REL_A_AES)
#error code not present
      #else

      #ifdef FEATURE_IS2000_REL_C
      else
      if (matched_msg_ptr->sec_mode_cmd.msg_int_info_incl && 
          ( P_REV_IS_9_OR_GREATER ) &&
          !mccap_is_msg_integrity_supported())
      {
        /* We don't support Message Integrity right now */
        next_state = mccsa_send_reject(curr_state, CAI_REJ_CAP, matched_msg_ptr);
      }
      #endif /* FEATURE_IS2000_REL_C */

      #endif /* FEATURE_IS2000_REL_C_AKA || FEATURE_IS2000_REL_A_AES */

      break;
    #endif /* FEATURE_IS2000_REL_A */

    #ifdef FEATURE_IS2000_REL_C
    case CAI_AUTH_REQ_MSG:
    {
      /*--------------------------------
        Authentication Request Message 
      --------------------------------*/

      MSG_HIGH ( "Received Auth Request Message in SA", 0, 0, 0 );

      #ifdef FEATURE_IS2000_REL_C_AKA
#error code not present
      #endif /* FEATURE_IS2000_REL_C_AKA */
      break;
    }
    #endif /* FEATURE_IS2000_REL_C */

    default:
      /* ------------------------------------------------
      ** The mobile station may ignore any other messages
      ** ------------------------------------------------ */
      break;
  } /* end switch (matched_msg_ptr->gen_pc.msg_type) */

  return (next_state);

} /* end mccsa_process_layer3() */

/*===========================================================================

FUNCTION MCCSA_PROC_PAGE

DESCRIPTION
  This function processes page messages received during the Mobile Station
  System Access attempt.

DEPENDENCIES
  None

RETURN VALUE
  The next state to be processed.

SIDE EFFECTS
  None

===========================================================================*/

word mccsa_proc_page
(
  mccrxtx_msg_type *msg_ptr,
    /* Pointer to received message */
  word curr_state
    /* Substate of System Access currently being processed */
)
{
  word next_state = curr_state;

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  boolean continue_page_match = TRUE;
    /* Indicates if we are going to do page match */

  if ( ( curr_state == CDMA_REG ) ||   /*lint !e641 */
       ( (curr_state == CDMA_MSG) 
         #ifdef FEATURE_IS2000_REL_A
         && ( cdma.sa_msg_tx_msg != CAI_AC_SMRM_MSG )           /*lint !e641 */
         #endif /* FEATURE_IS2000_REL_A */
         )  
     )
  {
    #ifdef FEATURE_IS2000_REL_A_CC
    if ((msg_ptr->msg.gen.msg_type == CAI_UPM_FIRST_SEG_MSG) ||
        (msg_ptr->msg.gen.msg_type == CAI_UPM_MIDDLE_SEG_MSG) ||
        (msg_ptr->msg.gen.msg_type == CAI_UPM_FINAL_SEG_MSG))
    {
      /* Reassemble Universal Page Message from received segments */
      if (mcc_reassemble_seg_upm(msg_ptr, &mccdma_seg_univ_page))
      {
        /* Perform partial address match if address block info is available */
        if ((mccdma_seg_univ_page.msg_field == CAII_UPM_ADDR_BLOCK) ||
            (mccdma_seg_univ_page.msg_field == CAII_UPM_DONE))
        {
          if (!mcc_match_upm_partial_addr(
              mccdma_seg_univ_page.page_block.length,
              mccdma_seg_univ_page.page_block.data,
              &mccdma_seg_univ_page.page_block.offset,
              &mccdma_seg_univ_page.interleaved_addr))
          {
            /* Partial address mismatch, no need to perform page match */

            mclog_report_event_seg_upm_addr_mismatch(MCLOG_PART_ADDR_MISMATCH, /*lint !e641 */
              mccdma_seg_univ_page.segment_seq);

            continue_page_match = FALSE;

            /* Done with reassembly and address match, empty reassembly buffer */
            mcc_empty_seg_upm_buffer();
          }
          else if (msg_ptr->msg.gen.msg_type != CAI_UPM_FINAL_SEG_MSG)
          {
            /* Not all segments of Universal Page Message received, delay
               page match */
            continue_page_match = FALSE;
          }
        }
        else
        {
          /* Not received enough info to proceed partial address match or page
             match */
          continue_page_match = FALSE;
        }
      }
      else
      {
        /* Failure to reassemble segmented Universal Page Message */
        continue_page_match = FALSE;
      }
    }
    else if (msg_ptr->msg.gen.msg_type == CAI_UNIVERSAL_PAGE_MSG)
    {
      if (!mcc_match_upm_partial_addr(
          msg_ptr->msg.upm.page_block.length,
          msg_ptr->msg.upm.page_block.data,
          &msg_ptr->msg.upm.page_block.offset,
          &msg_ptr->msg.upm.interleaved_addr))
      {
        /* Partial address mismatch, no need to perform page match */
        continue_page_match = FALSE;
      }
    }
    #endif /* FEATURE_IS2000_REL_A_CC */

    /* --------------------------------------------------------
    ** We have received a page in Registration Access Substate
    ** or while sending a data burst.
    ** If we declare a page match, abort the current access and
    ** handle the page.
    ** -------------------------------------------------------- */
    if (continue_page_match) //lint !e774
    {
      next_state = mccsa_page_match(msg_ptr, curr_state);

      if (next_state == CDMA_PAGE_RES) /*lint !e641 */
      {
        /* Reset layer 2 pending message flag */
        mccsa_layer2.pending_msg_flag = FALSE;
        mccsa_tx_stop();

        #if (defined(FEATURE_CDSMS) || defined (FEATURE_DBM))
        /* If we are going to Page Response, we have to notify SMS */
        if (( curr_state == CDMA_MSG ) /*lint !e641 */
            && ( cdma.sa_msg_tx_msg == CAI_AC_BURST_MSG )  /*lint !e641 */
           )
        {
          MSG_HIGH( "DBM dropped in SA for Page Response", 0, 0, 0 );
          mcc_check_mo_dbm_status(CM_DBM_MO_ABORT);
        }
        #endif /* FEATURE_CDSMS || FEATURE_DBM */
      
        if (curr_state == CDMA_REG) /*lint !e641 */
        {
          /* Report that a Page Response is pre-empting a Registration. */ 
          mclog_report_registration_suppressed(cdma.reg_type);
        }
      }
    }
  }
  else
  {
    /* We ignore pages from all other substates */
  }

  return (next_state);

} /* mccsa_proc_page() */

/*===========================================================================

FUNCTION MCCSA_PROC_MSG

DESCRIPTION
  This function does high level processing of messages received during the
  Mobile Station System Access attempt.  The layer 2 fields are examined and
  the appropriate layer 3 processing is done.  In the event that an ack is
  required for a message which has layer 3 processing which can not be done
  until after the ack is sent, the message is left pending and the ack sent.

DEPENDENCIES
  None

RETURN VALUE
  The next state to be processed.

SIDE EFFECTS
  None

===========================================================================*/

word mccsa_proc_msg
(
  mccrxtx_msg_type *rx_ptr,
    /* Pointer to message received from the base station */
  word curr_state
    /* Substate of System Access currently being processed */
)
{
  word next_state = curr_state;
    /* Next state to be processed */
  boolean continue_processing = FALSE;
    /* Flag if we should continue processing message */
  boolean perf_dup_detect = TRUE;
    /* Indicates if duplicate detection has to be performed, when we
    process pending messages we need to make sure this was not done already */

  #ifdef FEATURE_IS2000_REL_D
#error code not present
  #endif /* FEATURE_IS2000_REL_D */

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  #ifdef FEATURE_IS2000_REL_A_CC
  if (OVHD_CHAN_IS_BCCH &&
      (rx_ptr->msg.gen.msg_type != CAI_UPM_FIRST_SEG_MSG) &&
      (rx_ptr->msg.gen.msg_type != CAI_UPM_MIDDLE_SEG_MSG) &&
      (rx_ptr->msg.gen.msg_type != CAI_UPM_FINAL_SEG_MSG))
  {
    /* ----------------------------------------------------------------------
    ** Empty segmented Universal Page Message reassembly buffer if received
    ** message is not a UPM First Segment Message, UPM Middle Segment Message
    ** and UPM Final Segment.
    ** ---------------------------------------------------------------------- */
    mcc_empty_seg_upm_buffer();
  }
  #endif /* FEATURE_IS2000_REL_A_CC */

  if ( (OVHD_CHAN_IS_PCH && (rx_ptr->chn != RXC_CDMA_PC)) /*lint !e641 */
       #ifdef FEATURE_IS2000_REL_A_CC
       || (OVHD_CHAN_IS_BCCH && (rx_ptr->chn != RXC_CDMA_BCCH) && /*lint !e641 */
           (rx_ptr->chn != RXC_CDMA_FCCCH)) /*lint !e641 */
       #endif /* FEATURE_IS2000_REL_A_CC */
     )
  {
    /* --------------------------------
    ** Flag bad message with error call
    ** -------------------------------- */
    ERR( "Message received on wrong channel %d", rx_ptr->chn, 0, 0 );

    return (curr_state);
  }

  /******************************
   * Check message for correct  *
   * address and for duplicate. *
   ******************************/

  switch (rx_ptr->msg.gen.msg_type)
  {
    case CAI_PAGE_MSG:
    case CAI_SLT_PAGE_MSG:
    case CAI_GENERAL_PAGE_MSG:
    #ifdef FEATURE_IS2000_REL_A_CC
    case CAI_UNIVERSAL_PAGE_MSG:
    case CAI_UPM_FIRST_SEG_MSG:
    case CAI_UPM_MIDDLE_SEG_MSG:
    case CAI_UPM_FINAL_SEG_MSG:
    #endif /* FEATURE_IS2000_REL_A_CC */
      /* -------------------------------------------
      ** If we are not already handling a page there
      ** is a chance we are supposed to drop what we
      ** are doing and handle this page.
      ** ------------------------------------------- */

      /* If an Access Probe Handoff is in Progress,  then MS should wait */
      /* till the handoff is over, before processing this page.          */
      /* However, storing this page and responding to it later may not   */
      /* significantly improve the chances of sending page response.     */
      /* Waiting till HO gets over, MS downloads new OverHead params etc */
      /* might anyway cause significant delay and the Base Station will  */
      /* very likely timeout. So, to keep the code simple, we can just   */
      /* forget about this Page. Hopefully BS will retry this page and   */
      /* at that time MS will get this page.                             */
      if ((curr_state != CDMA_PAGE_RES) && (mccsa_apho_substate == APHO_NOT_IN_PROGRESS)) /*lint !e641 */
      {
        next_state = mccsa_proc_page(rx_ptr, curr_state);
      }
      /* Either we are not processing this page, or it has been processed */
      continue_processing = FALSE;
      break;

    case CAI_PC_ORD_MSG:
    case CAI_CHN_ASN_MSG:
    case CAI_PC_BURST_MSG:
    case CAI_FEATURE_MSG:
    case CAI_PC_AUTH_MSG:
    case CAI_PC_SSD_UP_MSG:
    case CAI_SERVICE_REDIR_MSG:
    case CAI_PC_STREQ_MSG:
    case CAI_PC_TMSI_ASGN_MSG:
    case CAI_EXT_CHN_ASN_MSG:
    #ifdef FEATURE_MEID_SUPPORT
    case CAI_MEID_EXT_CHN_ASN_MSG:
    #endif
    case CAI_PACA_MSG:
    #ifdef FEATURE_IS2000_REL_A
    case CAI_SEC_MODE_CMD_MSG:
    #endif
    #ifdef FEATURE_IS2000_REL_C
    case CAI_AUTH_REQ_MSG:
    #endif
      /* -------------------------
      ** Order or Directed Message
      ** ------------------------- */
      /* -------------------------------------------------------------
      ** mcc_match_msg returns TRUE if the message is addressed to us.
      ** If the address matches we must process the layer 2 fields.
      ** ------------------------------------------------------------- */
      continue_processing = mcc_match_msg(rx_ptr, &mccsa_matched_msg,
                            (
                             #ifdef FEATURE_IS2000_REL_D
#error code not present
                             #endif /* FEATURE_IS2000_REL_D */
                             TRUE), next_state);

      if ((continue_processing) &&
          (mccsa_matched_msg.gen_pc.msg_type == CAI_PC_BURST_MSG) &&
          (mccsa_matched_msg.gen_pc.hdr.addr_type == CAI_BCAST_ADDR_TYPE))
      {
        /* If this is a broadcast data burst message, we don't need */
        /* to process the layer 2 ack fields.                       */
        continue_processing = FALSE;
        next_state = mccsa_process_layer3( &mccsa_matched_msg, curr_state );
      }

      #ifdef FEATURE_IS2000_REL_A
      if (continue_processing && P_REV_IS_7_OR_GREATER)
      {
        /* Validate the decryption status of the parser */
        if (rx_ptr->parser_decryption_failure)
        {
          /* We don't support Ext. Encryption as yet, so this is a BS error, but
          we don't do anything other than ignore the message. After exiting
          access and returning to Idle state, if we continue to get signaling messages
          on f-csch that have ext. encryption, we will enter sys det (mccidl.c)  */

          continue_processing = FALSE;

          MSG_HIGH( "Access: Rxed Msg with Ext. Encryption fields, ignoring msg", 0, 0, 0 );
        }
      }
      #endif /* FEATURE_IS2000_REL_A */

      break;

    case CAI_SYS_PARM_MSG:
    case CAI_ACC_PARM_MSG:
    case CAI_EXT_SYS_PARM_MSG:
    case CAI_NL_MSG:
    case CAI_EXT_NL_MSG:
    case CAI_GEN_NL_MSG:
    case CAI_GLOBAL_REDIR_MSG:
    case CAI_EXT_GLB_REDIR_MSG:

    #ifdef FEATURE_IS2000_REL_A_CC
    case CAI_EN_AC_PARM_MSG:
    case CAI_ANSI41_SYS_PARM_MSG:
    case CAI_MC_RR_PARM_MSG:
    case CAI_UNIV_NL_MSG:
    #endif /* FEATURE_IS2000_REL_A_CC */

      /* --------------------------------------------------------------
      ** These Overhead Messages are processed in System Access State
      ** during Page Response substate and Origination substate during
      ** an Access Handoff or Access probe HO.
      ** ------------------------------------------------------------*/
      if (!mcc_is_aho_ovhd_msg_okay())
      {
        if ((curr_state == CDMA_ORIG) || (curr_state == CDMA_PAGE_RES) || /*lint !e641 */
            (mccsa_apho_substate == APHO_IN_PROGRESS))
        {
          MSG_MED("Sys Access OVHD message proc: %d", rx_ptr->msg.gen.msg_type,0,0);

          /* It is possible that the Processing of this Overhead message */
          /* required MS to leave System Access State and exit CDMA. If  */
          /* such is the case, then Stop TX and exit CDMA with MAX ACCESS*/
          /* Probe Indication.                                           */
          if  ( ( next_state = mccidl_msg_handler(rx_ptr, next_state) ) == CDMA_EXIT )
          {
            if (mcc_exit.exit == MCC_REDIRECTION)
            {
              mcc_exit.exit = MCC_ACCESS_FAILED_GSRM;
            }
            else
            {
              mcc_exit.exit = MCC_MAX_ACCESS_PROBES;
            }

            cdma.ho_state = CDMA_IDLE_HO;

            if (mccsa_layer2.access_in_progress)
            {
              mccsa_tx_stop();
              mccsa_apho_substate = APHO_NOT_IN_PROGRESS;
            }

            /* Failed access - perform registration processing */
            mccreg_idle_ack_or_fail();
            break;
          }

          if (mcc_is_aho_ovhd_msg_okay())
          {
            /* Process ignored overhead if any */
            if  ( ( next_state = mccidl_proc_ignored_ovhd(next_state) ) == CDMA_EXIT )
            {
              if (mcc_exit.exit == MCC_REDIRECTION)
              {
                mcc_exit.exit = MCC_ACCESS_FAILED_GSRM;
              }
              else
              {
                mcc_exit.exit = MCC_MAX_ACCESS_PROBES;
              }

              cdma.ho_state = CDMA_IDLE_HO;

              if (mccsa_layer2.access_in_progress)
              {
                mccsa_tx_stop();
                mccsa_apho_substate = APHO_NOT_IN_PROGRESS;
              }

              /* Failed access - perform registration processing */
              mccreg_idle_ack_or_fail();
              break;
            }

            /* Overhead messages are all updated. Stores new SID, NID and PZID and
               notify its reception if a handler function is registered. */
            mccsyobj_process_epzid(cur_bs_ptr->csp.sp.sid, cur_bs_ptr->csp.sp.nid, 
              (P_REV_IS_4_OR_GREATER ? cur_bs_ptr->csp.esp.packet_zone_id : MCCSYOBJ_PZID_UNKNOWN), CDMA_SYS_ACC);

            #ifdef FEATURE_IS2000_REL_D
#error code not present
            #endif /* FEATURE_IS2000_REL_D */

            /* -----------------------------------------------------------
            ** Following is the serving system info update for CM. It is
            ** done once we acquire a service.
            ** Notice that Digital indicator is on, CM can safely poll it
            ** to know whether the data is for CDMA or for AMPS. Also
            ** NO_SVC is set correctly now.
            ** ----------------------------------------------------------- */

             mcc_send_cm_srv_sys_info( CM_SS_SID_MASK|
                                       CM_SS_NID_MASK|
                                       CM_SS_REG_ZONE_MASK|
                                       CM_SS_PACKET_ZONE_MASK|
                                       CM_SS_BS_P_REV_MASK|
                                       CM_SS_P_REV_IN_USE_MASK, /*lint !e747 */
                                       cur_bs_ptr->csp.sp.sid,
                                       cur_bs_ptr->csp.sp.nid,
                                       cur_bs_ptr->csp.sp.reg_zone,
                                       (P_REV_IS_4_OR_GREATER ? cur_bs_ptr->csp.esp.packet_zone_id : MCCSYOBJ_PZID_UNKNOWN)
                                     );

            if (cdma.ho_state == CDMA_AHO)
            {
              if (mccsa_is_aho_allowed())
              {
                /* -------------------------------------------------------
                ** After Processing OVHD messages on the new BS after AHO,
                ** the handoff state is set to AHO if it is enabled by the
                ** new base station, otherwise it is set to default value.
                ** -------------------------------------------------------*/
                MSG_MED("AHO allowed", 0, 0, 0);
                cdma.ho_state = CDMA_AHO;
              }
              else
              {
                MSG_MED("AHO not allowed", 0, 0, 0);
                cdma.ho_state = CDMA_IDLE_HO;
              }
            }

            if  (mccsa_apho_substate == APHO_IN_PROGRESS)
            {
              if (cdma.ho_state == CDMA_APHO)
              {
                /* All the Overhead Messages have been updated. */
                /* Prepare for the next Pilot Report.           */
                next_state = mccsa_apho_done(curr_state);

                /* If exit System Access State because of some problems with */
                /* Access Probe Handoff, stop TXC.                               */
                if (next_state == CDMA_EXIT)
                {
                  mccsa_tx_stop();
                  /* Failed access - perform registration processing */
                  mccreg_idle_ack_or_fail();

                  return next_state;
                }
              }
              else
              {
                mccsa_apho_substate = APHO_NOT_IN_PROGRESS;
              }
            }

            /* -------------------------------------------------------------
            ** if we received a release during AHO, we need to send Release
            ** Order and throw away all pending layer3 messages after update
            ** OVHD is done.
            ** ------------------------------------------------------------*/
            if ((mccsa_orig_ministate == CDMA_ORIG_RELS)
                /* We could have come here when MS was waiting for TA timer to expire */
                && (mccsa_apho_substate == APHO_NOT_IN_PROGRESS)
                && ((mccsa_layer2.last_msg_type != CAI_AC_ORD_MSG) ||
                    (mccsa_layer2.last_ord_type != CAI_RELEASE_ORD)))
            {
              mccsa_layer3_msg_pending = FALSE;
              if (mccsa_layer2.access_in_progress)
              {
                mccsa_tx_stop();
              }
              mccsa_layer3_msg_pending = FALSE;
              next_state = mccsa_send_release_order_on_ac(curr_state,
                mccsa_release_cause);
            }

            if (mccsa_layer3_msg_pending)
            {
              /* -------------------------------------------------------------
              ** Mobile has received OVHD messages in System Access state.
              ** If there is any pending message while OVHD messages are being
              ** processed, the message is processed now.
              ** ------------------------------------------------------------*/
              mccsa_layer3_msg_pending = FALSE;
              mccsa_matched_msg = mccsa_layer2.pending_msg;
              /* If this message was already checked for duplicate,
                 don't do it again */
              perf_dup_detect = !mccsa_layer2.dup_detect_done;
              continue_processing = TRUE;
            }
          }
        }
        else
        {
          MSG_MED( "NO OVHD Messages Processing in this State",0,0,0);
        }
      } /* if (!mcc_is_aho_ovhd_msg_okay()) */

      break;

    default:
      /* The message type is one we do not process in System Access State */
      continue_processing = FALSE;
      break;
  }

  /*************************
   * Do layer 2 processing *
   *************************/

  if (continue_processing)
  {
    if (mccsa_layer2.access_in_progress)
    {
      /* ---------------------------------------------------------------------
      ** mccsa_layer2_ack_processing() returns TRUE if a layer 2 ack is found,
      ** otherwise FALSE is returned.
      ** --------------------------------------------------------------------- */
      if (!mccsa_layer2_ack_processing(&mccsa_matched_msg, curr_state))
      {

        #ifdef FEATURE_IS2000_REL_D
#error code not present
        #endif /* FEATURE_IS2000_REL_D */

        #ifdef FEATURE_IS2000_REL_C_AKA
#error code not present
        #endif /* FEATURE_IS2000_REL_C_AKA */

        {
          /* -----------------------------------------------------------
          ** We have an access in progress and did not get a layer2 ack.
          ** Exit now with no further processing of this message.
          ** Note that we could not have changed states.
          ** ----------------------------------------------------------- */
          return (curr_state);
        }
      }
      else
      {
        /*-------------------------------------------------------------
        ** Layer 2 Ack received. Do any processing that depends on this 
        ** condition here.
        ** ------------------------------------------------------------ */

        #ifdef FEATURE_IS2000_REL_A
        /* We did get a Layer 2 ack. In case this Layer 2 is sent
           as a response to packet data origination, we can claim
           this as a successful packet data call origination
           and update the old system information. */
        if (P_REV_IS_7_OR_GREATER)
        {
          if ((cdma.is_packet_data) && (curr_state == CDMA_ORIG)) /*lint !e641 */
          {
            mccsyobj_set_old_pd_orig_pzid(mccsyobj_get_pzid());
            mccsyobj_set_old_pd_orig_sid(mccsyobj_get_sid());
            mccsyobj_set_old_pd_orig_nid(mccsyobj_get_nid());
          }
        }
        #endif /* FEATURE_IS2000_REL_A */

        /* -------------------------------------------------------------------
        ** Set the cdma.ho_state to CDMA_IDLE_HO. This is done because at this
        ** stage it could be set to CDMA_APHO and MS is going out of APHO 
        ** region. However, after getting the layer 2 Ack MS will  not 
        ** necessarily go into AHO region. If it goes into AHO region, then 
        ** this setting of cdma.ho_state is just transient and after a couple 
        ** of statements it will be set to CDMA_AHO.
        ** ------------------------------------------------------------------- */
        cdma.ho_state = CDMA_IDLE_HO;

        /* Since an L2 ack has been received, we are now in Layer 3 for the 
           purpose of orig_count. */
        mccsa_orig_fail_layer = CM_CALL_ORIG_ERR_LAYER_L3;

        if (((curr_state == CDMA_ORIG) /*lint !e641 */
            && (mccsa_orig_ministate != CDMA_ORIG_RELS)
            ) || (curr_state == CDMA_PAGE_RES)) /*lint !e641 */
        {
          /* ---------------------------------------------------------
          ** This is an ack to a page response or origination message.
          ** Set AHO state if allowed based on the overhead.
          ** --------------------------------------------------------- */
          if (mccsa_is_aho_allowed())
          {
            MSG_MED("AHO allowed", 0, 0, 0);
            cdma.ho_state = CDMA_AHO;
          }
        }

        if (mccsa_apho_substate == WAIT_FOR_TXC_OK)
        {
          if (cdma.ho_state == CDMA_AHO)
          {
            /* -------------------------------------------------------------
            ** If the Access Probe Handoff was in progress when this layer 2
            ** ACK was recieved, then MS should perform an Access Handoff.
            ** However, it should be done only if the APHO has not been
            ** initiated already.
            ** ------------------------------------------------------------- */
            mccsa_init_aho = TRUE;
          }
          else if (cdma.ho_state == CDMA_IDLE_HO)
          {
            /* -------------------------------------------------------------
            ** If the APHO has not been initiated already, MS should stop
            ** the access probe handoff.
            ** ------------------------------------------------------------- */
            mccsa_apho_substate = APHO_NOT_IN_PROGRESS;
            /* -------------------------------------------------------------
            ** if we received a release during APHO, we need to send Release
            ** Order.
            ** ------------------------------------------------------------- */
            if ((mccsa_orig_ministate == CDMA_ORIG_RELS) &&
                ((mccsa_layer2.last_msg_type != CAI_AC_ORD_MSG) ||
                 (mccsa_layer2.last_ord_type != CAI_RELEASE_ORD)) &&
                mcc_is_aho_ovhd_msg_okay())
            {
              mccsa_layer3_msg_pending = FALSE;
              next_state = mccsa_send_release_order_on_ac(curr_state,
                mccsa_release_cause);
              return (next_state);
            }
          }
        }

        /* -------------------------
        ** Clear Access Attempt list
        ** ------------------------- */
        if (P_REV_IS_4_OR_GREATER)
        {
          mccsa_pilot_rpt_ovhd_rec.num_pilots_attempted = 0;
        }

        if ((mccsa_orig_ministate == CDMA_ORIG_RELS) &&
            (mccsa_layer2.last_msg_type == CAI_AC_ORD_MSG) &&
            (mccsa_layer2.last_ord_type == CAI_RELEASE_ORD))
        {
          next_state = CDMA_EXIT;
          /* set the exit indication to either normal or powerdown */
          if (mccsa_release_cause == CAI_REL_NORMAL)
          {
            mcc_exit.exit = MCC_RELEASE;
            MSG_HIGH("Release on AC completed with NORMAL indication.",0,0,0);
          }
          else
          {
            mcc_exit.exit = MCC_POWERDOWN;
            MSG_HIGH("Release on AC completed with POWERDOWN indication.",0,0,0);
          }
          mccsa_orig_ministate = CDMA_ORIG_NORM;//the release process is finished
          return (next_state);
        }

      }
    } /* end if (mccsa_layer2.access_in_progress) */

    /* ------------------------------------------------------
    ** Note we will never have a message pending if an access
    ** was not in progress when we received this message.
    ** ------------------------------------------------------ */

    /*********************************
     * Set up for layer 3 processing *
     *********************************/

    if (mcc_layer2.idle_msg_pending)
    {
      /*-------------------------------------------------------------
      ** If this flag is set it means we are in CDMA_ORD_RES substate
      ** sending an acknowledgement and are now going back to idle
      ** state to finish processing the message there.  Ignore
      ** ack_req and the layer 3 fields of this message and go back
      ** to idle state.
      ** ------------------------------------------------------------ */
      /* MS could be in the middle of Access Probe Handoff at this stage */
      /* However, we need not to do anything. Because, Tx will be stopped*/
      /* anyway and thus it will come out of suspended state.            */
      return (CDMA_IDLE_MSG_PENDING); /*lint !e641 */
    }
    else if (mccsa_layer2.pending_msg_flag)
    {
      /* --------------------------------------------------------
      ** We have a message pending, so we replace the current
      ** message with our pending message and process it instead.
      ** Note we ignore the ack_req field in this case, and will
      ** not make another access attempt before exiting the state.
      ** Also note that setting ack_req to false also keeps the
      ** pending message from being flagged as a duplicate.
      ** -------------------------------------------------------- */
      mccsa_matched_msg = mccsa_layer2.pending_msg;
      mccsa_matched_msg.gen_pc.hdr.ack_req = FALSE;
      mccsa_layer2.pending_msg_flag = FALSE;
      perf_dup_detect = !mccsa_layer2.dup_detect_done;
    }
    else if (mccsa_matched_msg.gen_pc.hdr.ack_req)
    {
      /* ------------------------------------------------------------
      ** There was no message pending, but there was an ack_required.
      ** Save Layer 2 field values for acknowledgement message.
      ** ------------------------------------------------------------ */
      mcc_layer2.ack_seq  = mccsa_matched_msg.gen_pc.hdr.msg_seq;
      mcc_layer2.ack_type = mccsa_matched_msg.gen_pc.hdr.addr_type;
      mcc_layer2.valid_ack = TRUE;
    }
    else
    {
      /* --------------------------------------------------
      ** There was no message pending and no ack required.
      ** If we send a layer 3 response or reject message,
      ** valid_ack is set to FALSE in the message.
      ** -------------------------------------------------- */
      mcc_layer2.valid_ack = FALSE;
    }

    /* -------------------------------------------------------
    ** Check if this is a duplicate message.  If it is, do not
    ** repeat the layer 3 processing we have already done.
    ** ------------------------------------------------------- */

    if (!perf_dup_detect || !mcc_detect_duplicate( FALSE, &mccsa_matched_msg.gen_pc.hdr))
    {
      /* --------------------------------
      ** Increment count PAG_3 or FCCCH_3
      ** -------------------------------- */

      if ((mccsa_matched_msg.gen_pc.msg_type != CAI_PC_ORD_MSG) ||
          (mccsa_matched_msg.pc_ord.order != CAI_BS_ACK_ORD))
      {
        /* ---------------------------------------------------------
        ** As long as this was not a BS_ACK_ORD we need to increment
        ** the count PAG_3 or FCCH_3.
        ** --------------------------------------------------------- */
        mcc_increment_pag_3_or_fccch_3();
      }

      /* Log the matched msg */
      mclog_report_matched_msg(mccsa_matched_msg.gen_pc.msg_type, MCLOG_MSG_MATCH);

      if ((mccsa_orig_ministate == CDMA_ORIG_RELS) &&
          ((mccsa_matched_msg.gen_pc.msg_type == CAI_CHN_ASN_MSG) ||
           #ifdef FEATURE_MEID_SUPPORT
           (mccsa_matched_msg.gen_pc.msg_type == CAI_MEID_EXT_CHN_ASN_MSG) ||
           #endif /* FEATURE_MEID_SUPPORT */
           (mccsa_matched_msg.gen_pc.msg_type == CAI_EXT_CHN_ASN_MSG)))
      {
        MSG_MED("Ignore CAM/ECAM - End key pressed", 0, 0, 0);
        return (curr_state);
      }

      /* --------------------------------------------------------------
      ** If a CAM/ECAM with no ACK required is received while the
      ** Mobile is processing Overhead messages after a successful
      ** Access Handoff or Access Probe Handoff, process it and setup
      ** Traffic Channel
      **-------------------------------------------------------------*/
      if (!mcc_is_aho_ovhd_msg_okay())
      {
        if (((mccsa_matched_msg.gen_pc.msg_type == CAI_CHN_ASN_MSG) ||
             #ifdef FEATURE_MEID_SUPPORT
             (mccsa_matched_msg.gen_pc.msg_type == CAI_MEID_EXT_CHN_ASN_MSG) ||
             #endif /* FEATURE_MEID_SUPPORT */
             (mccsa_matched_msg.gen_pc.msg_type == CAI_EXT_CHN_ASN_MSG))
            && (!mccsa_matched_msg.gen_pc.hdr.ack_req))
        {
          MSG_MED("Recvd CAM/ECAM with no ACK req: Process", 0, 0, 0);
        }
        else
        {
          mccsa_layer3_msg_pending = TRUE;
          mccsa_layer2.pending_msg = mccsa_matched_msg;
          /* Mark that we did not do duplicate detection for this mesg */
          mccsa_layer2.dup_detect_done = FALSE;
          MSG_MED("OVHD Messages not current, Hold any msg:", 0, 0, 0);
          return curr_state;
        }
      }

      /* -------------------------------------------------------------------
      ** If an Access Probe Handoff had been initiated, this layer 3 message
      ** should be kept pending until MS performs a Handoff and downloads
      ** the latest Ovhd parameters. This should be done if the layer 3
      ** message needs a response. Note that the layer 3 message could be
      ** Channel Assignment message as well. However, if it needs an Ack back
      ** MS sends back the acknowledgement before it actually processes the
      ** layer3 Ack. Because of the following code, MS is going to perform a
      ** handoff before doing so.
      ** ------------------------------------------------------------------- */
      if ((mccsa_apho_substate == WAIT_FOR_TXC_OK) &&
          (mccsa_matched_msg.gen_pc.hdr.ack_req))
      {
        mccsa_layer3_msg_pending = TRUE;
        mccsa_layer2.pending_msg = mccsa_matched_msg;
        /* Mark that dup detection has been done for this mesg*/
        mccsa_layer2.dup_detect_done = TRUE;
        mccsa_init_aho = TRUE;
        /* Set the ho_state to APHO at this time. When the ACK for the stored    
           message is received, the cdma.ho_state will be again set properly */
        cdma.ho_state = CDMA_APHO;
        MSG_MED("APHO in progress, Hold any msg:", 0, 0, 0);
        return curr_state;
      }

      #ifdef FEATURE_IS2000_REL_D
#error code not present
      #endif /* FEATURE_IS2000_REL_D */

      {
        /*******************
         * Process layer 3 *
         *******************/
        next_state = mccsa_process_layer3( &mccsa_matched_msg, curr_state );
      }
    } /* end if (not a duplicate) */

    /************************
     * Send Acknowledgement *
     * if still required.   *
     ************************/

    if ((!mccsa_layer2.access_in_progress) &&
        (mccsa_matched_msg.gen_pc.hdr.ack_req) &&
        (next_state == curr_state))
    {
      /* --------------------------------------------------
      ** We did not bundle the ack with a response message.
      ** We need to send an MS_ACK_ORD.  Note that if
      ** ack_req = 1 here, next_state should not have
      ** changed, but we check again just to be safe.
      ** -------------------------------------------------- */

      #if (defined(FEATURE_GPSONE_NEW_L2_ACK) || defined(FEATURE_SO_8026))
#error code not present
      #endif /* FEATURE_GPSONE_NEW_L2_ACK || FEATURE_SO_8026 */

      {


        cdma.sa_resp_msg                      = CAI_AC_ORD_MSG;
        cdma.sa_resp_ord.ordq.order           = CAI_MS_ACK_ORD;
        cdma.sa_resp_ord.ordq.ordq            = 0;

        next_state = mccsa_send_order_response_msg( curr_state, FALSE );
      }
    }

    #ifdef FEATURE_IS2000_REL_D
#error code not present
    #endif /* FEATURE_IS2000_REL_D */

    /******************************************************************
     * If we have received an ack to an Order Response, Registration  *
     * Message, or Data Burst, and have not changed states yet (i.e.  *
     * to CDMA_EXIT), we return to Idle State as long as we have not  *
     * started another access attempt (due to ack_req = 1 or a        *
     * response message required by the layer 3 processing).          *
     ******************************************************************/

    if ((!mccsa_layer2.access_in_progress) &&
        (next_state == curr_state) &&
          (  (curr_state == CDMA_ORD_RES) || /*lint !e641 */
             (curr_state == CDMA_REG) || /*lint !e641 */
             (curr_state == CDMA_MSG) /*lint !e641 */
             #ifdef FEATURE_SO_8026
#error code not present
             #endif /* FEATURE_SO_8026 */
          )
       )
    {
      /* ------------------------------------------------------
      ** We have completed our business in System Access state.
      ** We can now return to Idle state.
      ** ------------------------------------------------------ */
      next_state = CDMA_EXIT;
      mcc_exit.exit = MCC_ACCESS_COMPLETED;

      #ifdef FEATURE_SO_8026
#error code not present
      #endif /* FEATURE_SO_8026 */

    }
  } /* end if (continue_processing) */

  return (next_state);

} /* mccsa_proc_msg() */

/*===========================================================================

FUNCTION MCCSA_CALC_PSIST

DESCRIPTION
  This function calculates values to be used for the persistence test
  specified in IS-95 6.6.3.1.1.1 and 6.6.3.1.1.2.  The result is used
  in the TX task to compare against a pseudo-random number to determine
  if the mobile station will transmit during a given access slot.

DEPENDENCIES
  None

RETURN VALUE
  Returns a boolean value indicating if the access attempt should be
  aborted.

SIDE EFFECTS
  None

===========================================================================*/

access_result_type mccsa_calc_psist
(
  cai_tx_channel_id_type chan,
    /* one of {CAI_ACH, CAI_EACH} to indicate that the parm_ptr is a ap or eap */
  void *parm_ptr,
    /* Pointer to current Access Parameters Message */
  word curr_state
    /* Current System Access substate */
)
{
  caii_accparm_type *ap_ptr = (caii_accparm_type *) parm_ptr;        /* Pointer to current Access Parms Msg */

  #ifdef FEATURE_IS2000_REL_A_CC
  caii_en_ac_parm_type *eap_ptr;      /* Pointer to current Enhanced Access Params Msg */
  #endif /* FEATURE_IS2000_REL_A_CC */


  byte psist;
    /* The PSIST(N) value to be used in calculations */
  byte cancel;
    /* The value to compare PSIST(N) against to determine if the
       access attempt should be aborted */
  byte factor;
    /* The psist dividing factor */
  access_result_type access_aborted = NOT_ABORT;
    /* Indicates if access attempt should be aborted */

  byte psist_0_9 = ap_ptr->psist_0_9;      /* Persistence value for access overload classes
                                              0 through 9 */
  byte psist_10 = ap_ptr->psist_10;        /* Persistence value for Access Overload Class 10 */
  byte psist_11 = ap_ptr->psist_11;        /* Persistence value for Access Overload Class 11 */
  byte psist_12 = ap_ptr->psist_12;        /* Persistence value for Access Overload Class 12 */
  byte psist_13 = ap_ptr->psist_13;        /* Persistence value for Access Overload Class 13 */
  byte psist_14 = ap_ptr->psist_14;        /* Persistence value for Access Overload Class 14 */
  byte psist_15 = ap_ptr->psist_15;        /* Persistence value for Access Overload Class 15 */
  byte msg_psist = ap_ptr->msg_psist;      /* Persistence modifier for Access Channel attempts
                                              for message transmissions */
  byte reg_psist = ap_ptr->reg_psist;      /* Persistence modifier for Access Channel attempts
                                              for registrations */
  #ifdef FEATURE_IS2000_REL_A
  boolean psist_emg_incl = ap_ptr->psist_emg_incl;/* Emergency persistence included*/
  byte psist_emg = ap_ptr->psist_emg;      /* Persistence value for emergency
                                              access overload classes 0 through 9 */
  #endif /* FEATURE_IS2000_REL_A */

  dword *p = &(mcc_tx_buf.acc.p);                /* Persistence probability */
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  if ((chan != CAI_ACH) 
      #ifdef FEATURE_IS2000_REL_A_CC
      &&
      (chan != CAI_EACH)
      #endif /* FEATURE_IS2000_REL_A_CC */ 
     )
  {
    // chan must be one of {CAI_ACH, CAI_EACH}.
    ERR_FATAL("Unknown channel type",0,0,0);
  }

  #ifdef FEATURE_IS2000_REL_A_CC
  else if (chan == CAI_EACH)
  {
    eap_ptr = (caii_en_ac_parm_type *) parm_ptr;

    psist_0_9 = eap_ptr->psist_0_9_each;
    psist_10 = eap_ptr->psist_10_each;
    psist_11 = eap_ptr->psist_11_each;
    psist_12 = eap_ptr->psist_12_each;
    psist_13 = eap_ptr->psist_13_each;
    psist_14 = eap_ptr->psist_14_each;
    psist_15 = eap_ptr->psist_15_each;
    msg_psist = eap_ptr->msg_psist_each;
    reg_psist = eap_ptr->reg_psist_each;
    psist_emg_incl = TRUE;
    psist_emg = eap_ptr->psist_emg;
    p = &(mcc_tx_buf.reach.p);
  }
  #endif /* FEATURE_IS2000_REL_A_CC */

  if (cdma.accolc < 10)
  {
    cancel = 63;
    factor = 4;
    psist  = psist_0_9;

    #ifdef FEATURE_IS2000_REL_A
    if (P_REV_IS_7_OR_GREATER)
    {
      /* If an emergency call origination */
      if(cdma.call_orig && cdma.is_emergency)
      {
        if (psist_emg_incl)
        {
          cancel = 7;
          factor = 1;
          psist = psist_emg;
        }
        else
        {
          psist = 0;
        }
      }
    }
    #endif /* FEATURE_IS2000_REL_A */

    #ifdef FEATURE_PASSPORT_MODE
    if(mcc_passport_mode_is_jcdma())
    {
      if (!P_REV_IS_7_OR_GREATER && cdma.call_orig && cdma.is_emergency)
      {
        cancel = 7;
        factor = 1;
        psist = psist_11;
      }
    }
    #endif /* FEATURE_PASSPORT_MODE */ 

  }
  else  /* ACCOLCp is in range 10..15 */
  {
    cancel = 7;
    factor = 1;

    switch (cdma.accolc)
    {
      case 10:
        psist = psist_10;
        break;
      case 11:
        psist = psist_11;
        break;
      case 12:
        psist = psist_12;
        break;
      case 13:
        psist = psist_13;
        break;
      case 14:
        psist = psist_14;
        break;
      case 15:
        psist = psist_15;
        break;
      default:
        /* ----------------------------------------------
        ** Invalid ACCOLC value --> cancel access attempt
        ** ---------------------------------------------- */
        ERR( "Invalid ACCOLCp %d", cdma.accolc, 0, 0 );
        psist = cancel;
        break;
    } /* end switch (cdma.accolc) */
  } /* end else cdma.accolc == 10..15 */

  if (psist != cancel)
  {
    if (curr_state == CDMA_REG) /*lint !e641 */
    {
      *p =  (1UL << 31) >>
                              ((psist / factor ) + reg_psist);
    }
    else if (curr_state == CDMA_MSG) /*lint !e641 */
    {
      *p =  (1UL << 31) >>
                              ((psist / factor ) + msg_psist);
    }
    else
    {
      *p =  (1UL << 31) >> (psist / factor);
    }

    /* ----------------------------------------------------------
    ** Turn into full 32-bit word so we can compare with ran_next
    ** value generated in TXC
    ** ---------------------------------------------------------- */
    if (*p == (1UL << 31))
    {
      *p = 0xFFFFFFFFUL;
    }
    else
    {
      *p <<= 1;
    }
  }
  else
  {
    /* --------------------------------------------
    ** PSIST(N) = 7 or 63 --> Cancel access attempt
    ** -------------------------------------------- */
    access_aborted = ABORT_PSIST;
    MSG_HIGH( "PSIST(N) = %d --> Access Not Allowed", psist, 0, 0 );
  }

  return (access_aborted);

} /* end mccsa_calc_psist() */

/*===========================================================================

FUNCTION MCCSA_SEND_ACH_ACCESS_CMD

DESCRIPTION
  This function sends an Access command to the Transmit task for the ACH.

DEPENDENCIES
  None

RETURN VALUE
  TRUE if the access attempt was aborted, else FALSE.

SIDE EFFECTS
  None

===========================================================================*/

access_result_type mccsa_send_ach_access_cmd
(
  caii_tx_msg_type *tx_msg_ptr,  /* External format message pointer */
  caii_plt_rpt_type *pilot_rpt_ptr, /*IS2000 addition, ptr to pilot rpt fields of mesg */
  boolean          request,     /* Flag for request or response */
  word             msg_len,     /* Length of message */
  word             curr_state   /* Current substate */
)
{
  caii_accparm_type *ap_ptr;    /* Pointer to current Access Params Msg */
  access_result_type access_aborted;       /* Flag if access attempt aborted */
  word max_message_body;        /* Maximum number of bits message can have */
  word status;                  /* Message translation status */

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  access_aborted = NOT_ABORT;

  mccsa_access_attempt_is_request = request;

  /* Send command to transmit task to send access message */
  mcc_tx_buf.hdr.command = TXC_ACCESS_F;
  mcc_tx_buf.hdr.cmd_hdr.done_q_ptr = NULL;
  mcc_tx_buf.hdr.cmd_hdr.task_ptr = NULL;

  /* Signal when access attempt is finished */
  mcc_tx_buf.acc.tcb_ptr = &mc_tcb;
  mcc_tx_buf.acc.sigs = MCC_TXC_DONE_SIG;

  /* If we have valid access parameters give them to tx in the command */
  if ((ap_ptr = (caii_accparm_type *) mcc_get_msg( CAI_ACC_PARM_MSG )) != NULL)
  {
    /* ----------------------------------------------------------
    ** MAX_CAP_SZ is three less than the maximum number of frames
    ** the access channel message capsule can span.  Each frame
    ** can hold 88 data bits.  So the maximum message body length
    ** is, in bits, (MAX_CAP_SZ + 3) * 88 - (size of MSG_LENGTH
    ** field) - (size of CRC field).  See IS-95A section 6.7.1.2.
    ** ---------------------------------------------------------- */
    max_message_body = (ap_ptr->max_cap_sz + 3) * 88 -
                       CAI_AC_CRC_SIZE - CAI_AC_LGTH_SIZE; /*lint !e834 */


    if (P_REV_IS_4_OR_GREATER)
    {
      /* Bookeep Msg length excluding pilot report info */
      mccsa_pilot_rpt_ovhd_rec.msg_len = msg_len;

      /* Compute the number of pilots that can be contained in Access
         Channel Message without exceeding the maximum capsule size */
      mccsa_pilot_rpt_ovhd_rec.num_pilots_allowed =
        mccsa_compute_ac_msg_num_plt_rpt(max_message_body);

      /* ----------------------------------------------------------------
      ** First access probe : clear First pilot, Prev pilot, access probe
      ** handoff rebuild flag and access attempt list.
      ** ---------------------------------------------------------------- */
      mccsa_pilot_rpt_ovhd_rec.first_probe = TRUE;
      mccsa_pilot_rpt_ovhd_rec.first_pilot = NO_PREV_PILOT;
      mccsa_pilot_rpt_ovhd_rec.prev_pilot = NO_PREV_PILOT;
      mccsa_pilot_rpt_ovhd_rec.apho_rebuild = FALSE;
      mccsa_pilot_rpt_ovhd_rec.num_pilots_attempted = 0;

      /* Wait for searcher pilot strength measurement report and
         Update ACCESS_HO_LIST and OTHER_REPORTED_LIST */
      access_aborted = mccsa_wait_for_srch_pilots_meas_rpt();

      /* Succeed in building or updating ACCESS_HO_LIST */
      if ( access_aborted == NOT_ABORT )
      {
        /* Append pilot report information to Access Channel Message */
        mccsa_setup_pilot_rpt(pilot_rpt_ptr);
      }
    }

    if (access_aborted == NOT_ABORT)
    {
      if ((status = caix_int_to_ext( &msg_len, CAIX_AC,       /*lint !e641 */
                                     tx_msg_ptr, &mcc_tx_ext_msg,
                                     CAIX_PARSE_MSG
                                   )) != CAIX_DONE_S)
      {
        ERR( "Internal translation error %d", status, 0, 0 );
        if (status == CAIX_INV_LEN_S)
        {
          access_aborted = ABORT_MSG_TOO_LONG;  /* Cancel access attempt */
        }
        else
        {
          access_aborted = ABORT_XLATE_ERROR;  /* Cancel access attempt */
        }
      }
      else
      {
        mcc_tx_buf.acc.len = msg_len;
        mcc_tx_buf.acc.msg_ptr = mcc_tx_ext_msg.ac.body;

        if (mcc_tx_buf.acc.len <= max_message_body)
        {
          access_aborted = mccsa_populate_ach_access_cmd( ap_ptr, curr_state );
        }
        else
        {
          ERR("Message too large : len %d max %d", mcc_tx_buf.acc.len, max_message_body, 0 );
          access_aborted = ABORT_MSG_TOO_LONG;  /* Cancel access attempt */
          mcc_burst_overflow_size =
            (mcc_tx_buf.acc.len - max_message_body) / 8;
        }
      }
    }
  }
  else
  {
    ERR( "Internal error --> No access parameters", 0, 0, 0 );
    access_aborted = ABORT_NO_ACC_PARA;  /* Cancel access attempt */
  }

  if ( access_aborted == NOT_ABORT )
  {
    if (!mccsa_layer2.access_in_progress)
    {
      /* Proceed with access attempt */
      (void) rex_clr_sigs( &mc_tcb, MCC_TXC_DONE_SIG );

      /* Clear Access Probe Signal */
      (void) rex_clr_sigs( &mc_tcb, MCC_TXC_PROBE_SIG );

      /* Set the cdma.ho_state to indicate if the Access Probe */
      /* Handoff is allowed.                                   */
      /* First Initialise the Number of Access Probe Handoffs done */
      mccsa_num_apho_done =  0;

      /* Initialise the variable which controls the Handoff in the */
      /* boundary condition of APHO & AHO.                         */
      mccsa_init_aho = FALSE;

      /* And, finally initialise the APHO substate */
      mccsa_apho_substate = APHO_NOT_IN_PROGRESS;

      /* Clear Access Probe HO OK Signal */
      (void) rex_clr_sigs( &mc_tcb, MCC_TXC_APHO_OK_SIG );

      if (mccsa_is_apho_allowed())
      {
        MSG_MED("APHO allowed",0,0,0);
        cdma.ho_state = CDMA_APHO;
        /* Also, note that the function mccsa_is_apho_allowed()    */
        /* increments the count of access Probe Handoffs performed */
        /* which is not correct in this state. Reset it, therefore */
        mccsa_num_apho_done = 0;
      }
      else
      {
        MSG_MED("APHO not allowed",0,0,0);
        cdma.ho_state = CDMA_IDLE_HO;
        /* Fake that the Max Number of APHO have been performed */
        /* so that each time we do not end up performing lots of*/
        /* checks to figure out if APHO is allowed or not.      */
        mccsa_num_apho_done = cur_bs_ptr->csp.esp.max_num_probe_ho + 1;
      }

      mcc_txc_cmd( &mcc_tx_buf );

      /* Indicate that we are starting a new access attempt */
      mccsa_layer2.access_in_progress = TRUE;

      if (cdma.ho_state == CDMA_AHO)
      {
        /* Reset The HO state, this will be set when L2 Ack is received */
        cdma.ho_state = CDMA_IDLE_HO;
      }

      /* ------------------------------------------------------------
      ** Clear the state timer.  We set the timer when we complete
      ** an access attempt in Page Response or Origination Substates,
      ** in case a delayed response never comes.
      ** ------------------------------------------------------------ */
      (void) rex_clr_timer( &mcc_state_timer );
      (void) rex_clr_sigs( &mc_tcb,  MCC_STATE_TIMER_SIG );
    }
    else
    {
      ERR_FATAL( "Two access attempts at once.", 0, 0, 0);
    }
  }

  return access_aborted;

} /* end mccsa_send_ach_access_cmd() */

#ifdef FEATURE_IS2000_REL_A_CC
/*===========================================================================

FUNCTION MCCSA_SEND_REACH_ACC_PROBE_CMD

DESCRIPTION
  This function sends a reach probe command to the Transmit task.

DEPENDENCIES
  None

RETURN VALUE
  Next state to be processed.

SIDE EFFECTS
  MCC_TX_BUFS.REACH and PLT_RPT_OVHD_REC will be modified.

===========================================================================*/

word mccsa_send_reach_acc_probe_cmd
(
  word            curr_state,  /* Current state being processed */
  caii_tx_msg_type *tx_msg_ptr,  /* Internal format message pointer IS2000 change*/
  caii_plt_rpt_type *pilot_rpt_ptr /*Pointer into the plt rpt fields in the above pointer*/
)
{
  boolean access_aborted;        /* Flag if access attempt aborted */
  word next_state;               /* Next state to be processed */
  word msg_len, status;

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/


  access_aborted = FALSE;

  next_state = curr_state;

  /* Send command to transmit task to send access message */
  mcc_tx_buf.hdr.command = TXC_REACH_PROBE_F;
  mcc_tx_buf.hdr.cmd_hdr.done_q_ptr = NULL;
  mcc_tx_buf.hdr.cmd_hdr.task_ptr = NULL;

  /* Signal when access attempt is finished */
  mcc_tx_buf.reach.tcb_ptr = &mc_tcb;
  mcc_tx_buf.reach.sigs = MCC_TXC_DONE_SIG;

  /* If we have valid access parameters give them to tx in the command */
  if (( caii_en_ac_parm_type *) mcc_get_msg(CAI_EN_AC_PARM_MSG) != NULL)
  {
    /* Set up pilot report information in Access Channel Message */
    mccsa_setup_pilot_rpt(pilot_rpt_ptr);

    if ((status = caix_int_to_ext( &msg_len, CAIX_REACH,   /*lint !e641 */
                                   tx_msg_ptr, &mcc_tx_ext_msg,
                                   CAIX_PARSE_MSG
                                 )) != CAIX_DONE_S)
    {
      ERR( "Internal translation error %d", status, 0, 0 );
      access_aborted = TRUE;
    }
    else
    {
      mcc_tx_buf.reach.len = msg_len;
      mcc_tx_buf.reach.msg_ptr = mcc_tx_ext_msg.ac.body;

      // Check that the message is smaller than the maximum size
      // allowed by the base station.  The max is set in mccsa_select_rate().
      if ( mcc_tx_buf.reach.len > reach_max_message_length )
      {
        ERR( "Message too large", 0, 0, 0 );
        access_aborted = TRUE;  /* Cancel access attempt */
      }
    }
  }
  else
  {
    ERR( "Internal error --> No enhanced access parameters", 0, 0, 0 );
    access_aborted = TRUE;  /* Cancel access attempt */
  }

  if (!access_aborted)
  {
    MSG_MED( "Sent REACH Acc Probe Msg. PN=%d", (int) mcc_tx_buf.reach.pilot_pn, 0, 0 );
    mcc_txc_cmd( &mcc_tx_buf );
  }
  else
  {
    ERR( "Failure to send REACH Access Probe", 0, 0, 0 );

    /* Failed access - perform registration processing */
    mccreg_idle_ack_or_fail();

    /* Exit to System Determination */
    next_state    = CDMA_EXIT;
    mcc_exit.exit = MCC_ACCESS_DENIED;
  }
  return (next_state);

} /* end mccsa_send_reach_acc_probe_cmd() */

#endif // FEATURE_IS2000_REL_A_CC

/*===========================================================================

FUNCTION MCCSA_SEND_ACH_ACC_PROBE_CMD

DESCRIPTION
  This function sends an access probe command to the Transmit task.

DEPENDENCIES
  None

RETURN VALUE
  Next state to be processed.

SIDE EFFECTS
  MCC_TX_BUFS.AC and PLT_RPT_OVHD_REC will be modified.

===========================================================================*/

word mccsa_send_ach_acc_probe_cmd
(
  word            curr_state,  /* Current state being processed */
  caii_tx_msg_type *tx_msg_ptr,  /* Internal format message pointer IS2000 change*/
  caii_plt_rpt_type *pilot_rpt_ptr /*Pinter into the plt rpt fields in the above pointer*/
)
{
  caii_accparm_type *ap_ptr;    /* Pointer to current Access Params Msg */
  word max_message_body;        /* Maximum number of bits message can have */
  boolean access_aborted;       /* Flag if access attempt aborted */
  word next_state;              /* Next state to be processed */
  word msg_len, status;

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  access_aborted = FALSE;

  next_state = curr_state;

  /* Send command to transmit task to send access message */
  mcc_tx_buf.hdr.command = TXC_ACC_PROBE_F;
  mcc_tx_buf.hdr.cmd_hdr.done_q_ptr = NULL;
  mcc_tx_buf.hdr.cmd_hdr.task_ptr = NULL;

  /* Signal when access attempt is finished */
  mcc_tx_buf.acc.tcb_ptr = &mc_tcb;
  mcc_tx_buf.acc.sigs = MCC_TXC_DONE_SIG;

  /* If we have valid access parameters give them to tx in the command */
  if ((ap_ptr = (caii_accparm_type *) mcc_get_msg( CAI_ACC_PARM_MSG )) != NULL)
  {
    /* Set up pilot report information in Access Channel Message */
    mccsa_setup_pilot_rpt(pilot_rpt_ptr);

    if ((status = caix_int_to_ext( &msg_len, CAIX_AC,  /*lint !e641 */
                                   tx_msg_ptr, &mcc_tx_ext_msg,
                                   CAIX_PARSE_MSG
                                 )) != CAIX_DONE_S)
    {
      ERR( "Internal translation error %d", status, 0, 0 );
      access_aborted = TRUE;
    }
    else
    {
      mcc_tx_buf.acc.len = msg_len;
      mcc_tx_buf.acc.msg_ptr = mcc_tx_ext_msg.ac.body;


      /* ----------------------------------------------------------
      ** MAX_CAP_SZ is three less than the maximum number of frames
      ** the access channel message capsule can span.  Each frame
      ** can hold 88 data bits.  So the maximum message body length
      ** is, in bits, (MAX_CAP_SZ + 3) * 88 - (size of MSG_LENGTH
      ** field) - (size of CRC field).  See IS-95A section 6.7.1.2.
      ** ---------------------------------------------------------- */
      max_message_body = (ap_ptr->max_cap_sz + 3) * 88 -
                       CAI_AC_CRC_SIZE - CAI_AC_LGTH_SIZE; /*lint !e834 */
      if (mcc_tx_buf.acc.len > max_message_body)
      {
        ERR( "Message too large", 0, 0, 0 );
        access_aborted = TRUE;  /* Cancel access attempt */
      }
    }
  }
  else
  {
    ERR( "Internal error --> No access parameters", 0, 0, 0 );
    access_aborted = TRUE;  /* Cancel access attempt */
  }

  if (!access_aborted)
  {
    MSG_MED( "Sent Acc Probe Msg", 0, 0, 0 );
    mcc_txc_cmd( &mcc_tx_buf );
  }
  else
  {
    ERR( "Failure to send Access Probe", 0, 0, 0 );

    /* Failed access - perform registration processing */
    mccreg_idle_ack_or_fail();

    /* Exit to System Determination */
    next_state    = CDMA_EXIT;
    mcc_exit.exit = MCC_ACCESS_DENIED;
  }

  return (next_state);

} /* end mccsa_send_ach_acc_probe_cmd() */

/*===========================================================================

FUNCTION MCCSA_SEND_ACC_PROBE_CMD

DESCRIPTION
  This function sends an access probe command to the Transmit task by either
  calling send_ach_acc_probe_cmd or send_reach_acc_probe_cmd.

DEPENDENCIES
  None

RETURN VALUE
  Next state to be processed.

SIDE EFFECTS
  None.

===========================================================================*/

word mccsa_send_acc_probe_cmd
(
  word            curr_state,  /* Current state being processed */
  caii_tx_msg_type *tx_msg_ptr,  /* Internal format message pointer IS2000 change*/
  caii_plt_rpt_type *pilot_rpt_ptr /*Pointer into the plt rpt fields in the above pointer*/
)
{

  #ifdef FEATURE_IS2000_REL_A_CC
  if (OVHD_CHAN_IS_BCCH)
  {

    return mccsa_send_reach_acc_probe_cmd ( curr_state, tx_msg_ptr, pilot_rpt_ptr );
  }
  else
  #endif // FEATURE_IS2000_REL_A_CC

  {
    return mccsa_send_ach_acc_probe_cmd ( curr_state, tx_msg_ptr, pilot_rpt_ptr );
  }

} /* mccsa_send_acc_probe_cmd */

/*===========================================================================

FUNCTION mccsa_compute_independent_msg_len

DESCRIPTION
  This function computes the length of the access channel mesg excluding the
  fields whose length depends on the rest of the message length i.e it does
  not include variable length part of the pilot_rpt and
  orig digits (for origination). We need this info
  to figure out the number of pil reports we can accomodate.

DEPENDENCIES
  None

RETURN VALUE
  The length of the mesg so far in bits (It includes the Pilot Report Fixed
  part - the active pilot info - when applicable.)

SIDE EFFECTS

===========================================================================*/

static word mccsa_compute_independent_msg_len
(
  caii_tx_msg_type  * int_msg_ptr,
  caii_plt_rpt_type * int_msg_pilot_rpt_ptr
)
{
  word msg_size;

  // Set the pilot report to have no pilots, so the variable
  // parts of the pilot report are not included in the length calculation
  int_msg_pilot_rpt_ptr->fix.num_add_plt = 0;

  #ifdef FEATURE_IS2000_REL_A
  int_msg_pilot_rpt_ptr->aux_fix.num_aux_plt = 0;
  #endif

  // invoke the parser to get the length, note that the parser will be later invoked
  // again (in mccsa_send_access_cmd()) for packing the message.
  if (caix_int_to_ext( &msg_size, CAIX_AC, int_msg_ptr, &mcc_tx_ext_msg,     /*lint !e641 */
                       CAIX_CALC_MSG_LENGTH
                     ) != CAIX_DONE_S)  /*lint !e641 */
  { // parser returned failure
    msg_size = 0;
  }

  return msg_size;

} /* mccsa_compute_independent_msg_len */

#ifdef FEATURE_IS2000_REL_C
/*===========================================================================

FUNCTION MCCSA_SEND_RECONNECT_MSG

DESCRIPTION
  This function builds a Reconnect Message.

DEPENDENCIES
  None

RETURN VALUE
  Next state to be processed.

SIDE EFFECTS
  None

===========================================================================*/

word mccsa_send_reconnect_msg
(
  word curr_state,
    /* Current state being processed */
  boolean assured_mode
    /* Indicator whether the RCNM should be sent in assured mode */
)
{
  word next_state = curr_state;      /* Next state to be processed */
  access_result_type access_aborted; /* Flag if access attempt aborted */
  word msg_len;                      /* Length of message */
  word so_req = cdma.so_req;         /* SO requested */
  byte sr_id = 0;                    /* SR_ID to be restored */

  /* ---------------------------
  ** Build Reconnect Message
  ** --------------------------- */
  mcc_tx_int_msg.rcnm.msg_type = mccsa_layer2.last_msg_type = 
    CAI_AC_RECONNECT_MSG;

  mcc_tx_int_msg.rcnm.hdr.msg_seq = mcc_layer2.msg_seq = 
    (mcc_layer2.msg_seq + 1) & 0x7; /* Modulo 8 sequence number */

  mcc_tx_int_msg.rcnm.hdr.ack_seq = mcc_layer2.ack_seq;
  mcc_tx_int_msg.rcnm.hdr.ack_type = mcc_layer2.ack_type;
  mcc_tx_int_msg.rcnm.hdr.ack_req = assured_mode;

  if (curr_state == CDMA_PAGE_RES) /*lint !e641 */
  {
    mcc_tx_int_msg.rcnm.hdr.valid_ack = mcc_layer2.valid_ack;
  }
  else
  {
    #ifdef FEATURE_GPSONE_NEW_L2_ACK
#error code not present
    #else
    mcc_tx_int_msg.rcnm.hdr.valid_ack = FALSE;
    #endif /* FEATURE_GPSONE_NEW_L2_ACK */
  }

  mcc_set_address(&mcc_tx_int_msg.rcnm.hdr, mcc_tx_int_msg.rcnm.msg_type);
  
  /* Setup authentication and message integrity fields */
  if (curr_state != (next_state = mccsa_setup_auth_msg_int_fields(curr_state,
      &mcc_tx_int_msg, 0, 0, NULL, 0, 0, &mcc_tx_int_msg.rcnm.ami)))
  {
    /* Abort access now before it even begins */
    if (curr_state == CDMA_PAGE_RES) /*lint !e641 */
    {
      MSG_MED("Reconnect (PRM) interrupted during auth_sig", 0, 0, 0);
    }
    else
    {
      MSG_MED("Reconnect (ORM) interrupted during auth_sig", 0, 0, 0);
    }
    return (next_state);
  }

  /* Setup encryption fields */
  mcc_tx_int_msg.rcnm.enc.enc_fields_incl = FALSE;

  #ifdef FEATURE_IS2000_REL_A_AES
#error code not present
  #endif /* FEATURE_IS2000_REL_A_AES */

  if (next_state == CDMA_PAGE_RES) /*lint !e641 */
  {
    mcc_tx_int_msg.rcnm.orig_ind = FALSE;
  }
  else
  {
    mcc_tx_int_msg.rcnm.orig_ind = TRUE;

    /* -------------------------------------------------------------------
       Check to see if the SO is in the SNM, and enabled.
       If the SO is not in the SNM, it will check to make sure it is valid
       using srv_eval_so.

       It will return FALSE if:
       a) The SO is in the SNM, but is not enabled (ex: QC proprietary SO
          in a non-Proprietary phone, SO# 0x8020, 0x8021, 0x8022)
       b) The SO is not in the SNM, and srv returns a different value for
          it (ex: RS2 voice is requested, but RS2 is not supported. 
          srv_eval_so will return 8k voice or EVRC, so this function will 
          return FALSE.
       -------------------------------------------------------------------- */

    if (snm_so_is_enabled(cdma.so_req))
    {
      so_req = cdma.so_req;
    }
    else
    {
      /* This can only happen in the case of a wildcard voice origination 
         where NV variables for home/roam orig SO has been corrupted, so the
         best thing to do at this point make sure the origination fails, so
         set an invalid SO in the origination message */
      ERR("Invalid SO requested:%d, Making it 0", cdma.so_req, 0, 0 );
      so_req = CAI_SO_REJ;
    }
  }

  /* See if SYNC_ID can be specified */
  mcc_tx_int_msg.rcnm.sync_id_incl = FALSE; 

  #ifdef FEATURE_IS2000_REL_D
#error code not present
  #endif /* FEATURE_IS2000_REL_D */
  {
    mcc_tx_int_msg.rcnm.sync_id_incl = mccsyncid_find_compatible_sync_id(TRUE, so_req, 
      cdma.use_sync_id, &cdma.is_sr_id_valid, &cdma.sr_id, 
      &mcc_tx_int_msg.rcnm.sync_id_len, mcc_tx_int_msg.rcnm.sync_id, &sr_id);
  }
  
  if (mcc_tx_int_msg.rcnm.sync_id_incl)
  {
    if (mcc_tx_int_msg.rcnm.orig_ind)
    {
      mcc_tx_int_msg.rcnm.sr_id = sr_id;
    }
  }
  else
  {
    mcc_tx_int_msg.rcnm.service_option = so_req;
  }
 
  if (mcc_tx_int_msg.rcnm.orig_ind && !mcc_tx_int_msg.rcnm.sync_id_incl)
  {
    sr_id = CAI_SR_ID_1; /*lint !e641 */

    /*
    ** Let's find out what SR_ID to propose. The rule is:
    ** (1) If the call is a data call, use the supplied SR_ID.
    ** (2) if request for restoring service configuration,
    **     MC already allocated SR_ID. 
    ** (3) Otherwise, get one from the SRID Manager for (SVD)
    **     or hard code for non-SVD build. 
    */

    if (cdma.is_sr_id_valid)
    {
      sr_id = cdma.sr_id;
    }
    else
    {

      #ifdef FEATURE_IS2000_REL_A_SVD 
      /* CM does not supply an SR_ID for DS. Allocate one from SR_ID manager */
      sr_id = mccsrid_allocate_srid(SRID_OWNER_MC);

      if (sr_id == MCCSRID_INVALID_SRID)
      {
        /* No SR_ID available. Quit */
        MSG_ERROR("Invalid SR_ID", 0, 0, 0);
        mcc_exit.exit = MCC_ACCESS_FAILED;
        return CDMA_EXIT;
      }
      else
      {
        /* MC allocates SR_ID for UI, but not for DS. DS takes care of
        ** allocation & deallocation.
        */
        MSG_MED("SRID %d allocated", sr_id, 0, 0);
      }
      #endif /* FEATURE_IS2000_REL_A_SVD */

      /* Save a copy */
      cdma.is_sr_id_valid = TRUE;
      cdma.sr_id = sr_id;  
    }

    mcc_tx_int_msg.rcnm.sr_id = sr_id;
  }

  #ifdef FEATURE_IS2000_REL_D
#error code not present
  #endif /* FEATURE_IS2000_REL_D */

  /* Setup voice privacy fields and kick off calculating authentication key */
  #ifdef FEATURE_AUTH
  (void) mcc_setup_vpm_and_akey( FALSE, cur_bs_ptr->rp.auth );
  #else
  (void) mcc_setup_vpm_and_akey( FALSE, FALSE);
  #endif /* FEATURE_AUTH */

  plt_rpt_ptr = &mcc_tx_int_msg.rcnm.pilot_rpt;
  msg_len = mccsa_compute_independent_msg_len(&mcc_tx_int_msg, plt_rpt_ptr);

  access_aborted = mccsa_send_access_cmd(&mcc_tx_int_msg, plt_rpt_ptr,  
    mcc_tx_int_msg.rcnm.orig_ind, msg_len, next_state);

  if (access_aborted == NOT_ABORT)
  {
    MSG_HIGH("Sent Reconnect", 0, 0, 0);

    /* Save sequence number of last registration message */
    mccsa_layer2.last_reg_msg_seq = mcc_tx_int_msg.rcnm.hdr.msg_seq;

    /* Increment the counter for ACH/EACH messages generated by Layer 3. */
    if (curr_state == CDMA_PAGE_RES) /*lint !e641 */
    {
      mccsa_increment_l3_counter(SA_RESPONSE);
    }
    else
    {
      mccsa_increment_l3_counter(SA_REQUEST);
    }
  }

  else /* access was aborted */
  {
    /* Failed access - perform registration processing */
    mccreg_idle_ack_or_fail();

    /* Exit to System Determination */
    next_state    = CDMA_EXIT;

    if ( access_aborted == ABORT_SRCH_REQUEST_SYS_RESTART )
    {
      mcc_exit.exit = MCC_SRCH_REQUEST_SYS_RESTART;
    }

    #ifdef FEATURE_IS2000_REL_A
    else if (access_aborted == ABORT_ACCT_BLOCK)
    {
      /* Call blocked due to ACCT, Change exit reason */
      MSG_MED( "ACCT blocked", 0, 0, 0 );
      mcc_exit.exit = MCC_ACCT_BLOCKED;
    }
    #endif /* FEATURE_IS2000_REL_A */

    else
    {
      mcc_exit.exit = MCC_ACCESS_DENIED;
    }

    #ifdef FEATURE_PASSPORT_MODE
    if(mcc_passport_mode_is_jcdma())
    {
      /* Save the failure history by Persistence Test */
      if (access_aborted == ABORT_PSIST)
      {
        mccsa_persistence_orig_ng = TRUE;
      }
    }
    #endif /* FEATURE_PASSPORT_MODE */

  }

  return next_state;

} /* mccsa_send_reconnect_msg */
#endif /* FEATURE_IS2000_REL_C */

#ifdef FEATURE_IS2000_REL_D
#error code not present
#endif /* FEATURE_IS2000_REL_D */

/*===========================================================================

FUNCTION MCCSA_SEND_PAGE_RESPONSE_MSG

DESCRIPTION
  This function builds a Page Response Message.

DEPENDENCIES
  None

RETURN VALUE
  TRUE if the access attempt was aborted, else FALSE.

SIDE EFFECTS
  None

===========================================================================*/

word mccsa_send_page_response_msg
(
  word curr_state,
    /* Current state being processed */
  boolean assured_mode
    /* Indicator whether the PRM should be sent in assured mode */
)
{
  #ifdef FEATURE_IS2000_REL_A
  caii_enc_cap_type enc_capability;
  #endif /* FEATURE_IS2000_REL_A */

  word msg_len;
    /* Length of bit-packed message */
  access_result_type access_aborted;
    /* Flag if access attempt aborted */
  word next_state = curr_state;
    /* Next state to be processed */
  byte max_alt_so = 0;

  #ifdef FEATURE_USERZONE
  const sd_si_uz_info_s_type *uz_info_ptr;
  #endif /* FEATURE_USERZONE */

  #if defined(FEATURE_IS2000_REL_C_FCS) || defined(FEATURE_IS2000_REL_D)
  byte sr_id;    
    /* SR_ID to be restored */
  #endif /* FEATURE_IS2000_REL_C_FCS || FEATURE_IS2000_REL_D */

  int8 ota_sci;

  #if defined(FEATURE_IS2000_REL_C_AKA) && defined(FEATURE_IS2000_REL_A_AES)
#error code not present
  #endif /* FEATURE_IS2000_REL_C_AKA && FEATURE_IS2000_REL_A_AES */


/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  /* ---------------------------
  ** Build Page Response Message
  ** --------------------------- */
  mcc_tx_int_msg.page_resp.msg_type = mccsa_layer2.last_msg_type =
    CAI_PAGE_RESP_MSG;

  mcc_tx_int_msg.page_resp.hdr.msg_seq = mcc_layer2.msg_seq = 
    (mcc_layer2.msg_seq + 1) & 0x7; /* Modulo 8 sequence number */

  mcc_tx_int_msg.page_resp.hdr.ack_seq = mcc_layer2.ack_seq;
  mcc_tx_int_msg.page_resp.hdr.ack_type = mcc_layer2.ack_type;
  mcc_tx_int_msg.page_resp.hdr.ack_req = assured_mode;
  mcc_tx_int_msg.page_resp.hdr.valid_ack = mcc_layer2.valid_ack;

  mcc_set_address(&mcc_tx_int_msg.page_resp.hdr, mcc_tx_int_msg.page_resp.msg_type);

  /* Setup authentication and message integrity fields */
  if (curr_state != (next_state = mccsa_setup_auth_msg_int_fields(curr_state, 
      &mcc_tx_int_msg, 0, 0, NULL, 0, 0, &mcc_tx_int_msg.page_resp.ami)))
  {
    /* Abort access now before it even begins */
    MSG_MED("Page Response interrupted during auth_sig", 0, 0, 0);
    return (next_state);
  }

  #ifdef FEATURE_IS2000_REL_A
  /* Setup encryption fields */
  mcc_tx_int_msg.page_resp.enc.enc_fields_incl = FALSE;

  #ifdef FEATURE_IS2000_REL_A_AES
#error code not present
  #endif /* FEATURE_IS2000_REL_A_AES */

  /* If AES is enabled, but MI is not supported or enabled, this message must be proposing
   * a new SSEQ_H for AES use. Now it is the time to set the AES status fields of 
   * pending message type and pending SSEQ_H
   */
  #ifdef FEATURE_IS2000_REL_A_AES
#error code not present
  #endif /* FEATURE_IS2000_REL_A_AES */

  #endif /* FEATURE_IS2000_REL_A */

  #if defined(FEATURE_IS2000_REL_C_AKA) || defined(FEATURE_IS2000_REL_A_AES)
#error code not present
  #endif /* FEATURE_IS2000_REL_C_AKA || FEATURE_IS2000_REL_A_AES */

  {
    /* Setup voice privacy fields and kick off calculating authentication key */
    mcc_tx_int_msg.page_resp.pm = 
      #ifdef FEATURE_AUTH
      mcc_setup_vpm_and_akey( TRUE, cur_bs_ptr->rp.auth );
      #else
      mcc_setup_vpm_and_akey( TRUE, FALSE);
      #endif /* FEATURE_AUTH */
  }

  /* --------------------------------------------------------------
  ** Indicate mobile not capable of a CDMA to narrow analog handoff
  ** -------------------------------------------------------------- */
  mcc_tx_int_msg.page_resp.nar_an_cap = FALSE;

  mcc_tx_int_msg.page_resp.mob_term = mccreg_get_mob_term();

  ota_sci = mcc_convert_pref_sci_to_ota_sci();
  
  #ifdef FEATURE_MODIFIED_SCI
#error code not present
  #endif /* FEATURE_MODIFIED_SCI */
  {
    mcc_tx_int_msg.page_resp.slot_cycle_index = ((ota_sci > 0) ? ota_sci : -1 * ota_sci);  /*lint !e732*/

    #ifdef FEATURE_IS2000_REL_D
#error code not present
    #endif /* FEATURE_IS2000_REL_D */

    /* Record the reported slot cycle index that used in later operational SCI
       update */
    mccreg_set_reg_sci(ota_sci);
  }

  mcc_tx_int_msg.page_resp.mob_p_rev = cdma.mob_cai_rev;

  /* ------------------------------------------------------
  ** For IS-95B backward compatibility:
  ** 1. If the phone is JSTD008 only or is talking to a JSTD
  **    BS ( P_REV_IN_USE is 1 ), we send 0xa0 as SCM.
  ** 2. If the phone has P_REV_IN_USE greater or equal 4,
  **    then we should send scm and set MSB to 1.
  ** -------------------------------------------------------*/
  /*lint -e506 */
  if (!mcc_set_scm_for_band_class(cdma.band_class, &(mcc_tx_int_msg.page_resp.scm)))
  {
    mcc_tx_int_msg.page_resp.scm = 0xa0;
  }

  if (P_REV_IS_4_OR_GREATER)
  {
    if (mcc_tx_int_msg.page_resp.ami.auth_mode != CAI_NO_AUTH)
    {
      mcc_tx_int_msg.page_resp.encry_sup = 0x00;
    }
  }

  mcc_tx_int_msg.page_resp.request_mode = sd_misc_get_cdma_request_mode();

  mcc_tx_int_msg.page_resp.service_option = 
    #ifdef FEATURE_IS2000_REL_D
#error code not present
    #endif /* FEATURE_IS2000_REL_D */
    cdma.so_req;

  if ( (OVHD_CHAN_IS_PCH && SP_MSG_IS_RECVD && ESP_MSG_IS_RECVD 
       && cur_bs_ptr->csp.sp.ext_sys_parameter)
       #ifdef FEATURE_IS2000_REL_A_CC
       || (OVHD_CHAN_IS_BCCH && ANSI41SP_MSG_IS_RECVD)
       #endif /* FEATURE_IS2000_REL_A_CC */
     )
  {
    if (P_REV_IS_4_OR_GREATER)
    {
      if (CAI_AC_ALT_SO_MAX < cur_bs_ptr->csp.esp.max_num_alt_so)
      {
        max_alt_so = CAI_AC_ALT_SO_MAX;
      }
      else
      {
        max_alt_so = cur_bs_ptr->csp.esp.max_num_alt_so;
      }
    }
    else
    {
      max_alt_so = 0;
    }

    #ifdef FEATURE_IS2000_REL_A
    if (!P_REV_IS_7_OR_GREATER) //lint !e774 !e506
    #endif /* FEATURE_IS2000_REL_A */

    {
      if (max_alt_so > 0)
      {
        mcc_tx_int_msg.page_resp.num_alt_so = 
          snm_get_alt_so_list(cdma.so_req, max_alt_so,
          mcc_tx_int_msg.page_resp.alt_so);
      }
      else
      {
        mcc_tx_int_msg.page_resp.num_alt_so = 0x00;
      }
    }

    #ifdef FEATURE_IS2000_REL_A
    else /* P_REV_IS_7_OR_GREATER */
    {
      /* First, find all the alternate Service Options available.*/
      /* Note here the max_alt_so doesn't govern the number of   */
      /* SOs we might include in the SO_BITMAP. So we try to find*/
      /* as many alternate SOs as available.                     */
      mcc_tx_int_msg.page_resp.num_alt_so =
        snm_get_alt_so_list(cdma.so_req, CAI_AC_ALT_SO_MAX,
                            mcc_tx_int_msg.page_resp.alt_so);

      if (mcc_tx_int_msg.page_resp.num_alt_so > 0)
      {
        /* Now that we've decided what alternative Service Options MS
          will report, we need to convert those SO's to the bitmap to
          save space.

          Following function takes the first three parameters as input,
          fills the next three using them. It also may modify the
          first two parameters num_alt_so and alt_so. If any SO can not
          be represented in bitmap format, it is returned in alt_so
          byte array. Total number of such SO'a are returned in
          num_alt_so.

          As a result all fields related to alternative SO's and
          SO bitmap will be populated in this function. */

        snm_convert_alt_so_bitmap (
          cdma.so_req,                         // primary Service Option
          &mcc_tx_int_msg.page_resp.num_alt_so,    // total number of alt SO's
          mcc_tx_int_msg.page_resp.alt_so,         // byte array that contains
                                               // alternative SO's.
          &mcc_tx_int_msg.page_resp.so_bitmap_ind, // tells the length of the bitmap
          &mcc_tx_int_msg.page_resp.so_group_num,  // SO Group number that main SO
                                               // belongs to.
          &mcc_tx_int_msg.page_resp.so_bitmap      // SO bitmap
        );

        /* We have stuffed as many alt SOs in the SO_BITMAP as allowed. */
        /* Now make sure we report the corrent number of SOs in the     */
        /* alt_so list                                                  */
        if (max_alt_so < mcc_tx_int_msg.page_resp.num_alt_so)
        {
          mcc_tx_int_msg.page_resp.num_alt_so = max_alt_so;
        }
      }
      else
      {
        mcc_tx_int_msg.page_resp.so_bitmap_ind = SO_BITMAP_SIZE_0;
      }
    }
    #endif /* FEATURE_IS2000_REL_A */

  }

  MSG_HIGH("Page Response SO = %d %d", cdma.so_req, 
    mcc_tx_int_msg.page_resp.service_option, 0);

  #ifdef FEATURE_SO_8026
#error code not present
  #endif /* FEATURE_SO_8026 */

  if (P_REV_IS_6_OR_GREATER)
  {
    // We need to send this info only if the p_rev in use
    // is greater than 6
    mcc_tx_int_msg.page_resp.uzid_incl = 0;

    /* Set CH_IND and EXT_CH_IND */
    snm_get_pref_chan_ind_for_so(cdma.so_req, &mcc_tx_int_msg.page_resp.ch_ind
      #ifdef FEATURE_IS2000_REL_C_DV
#error code not present
      #endif /* FEATURE_IS2000_REL_C_DV */
      );

    snm_get_pref_fundicated_RCs(cdma.so_req, 
      (cai_radio_config_type *) &mcc_tx_int_msg.page_resp.for_rc_pref,
      (cai_radio_config_type *) &mcc_tx_int_msg.page_resp.rev_rc_pref); /*lint !e826 */

    mcc_tx_int_msg.page_resp.otd_supported = mccap_item_is_supported(MCCAP_DF_OTD); 
    mcc_tx_int_msg.page_resp.qpch_supported = mccap_item_is_supported(MCCAP_DF_QPCH); 
    mcc_tx_int_msg.page_resp.enhanced_rc = 1; // Enhanced RC support
    mcc_tx_int_msg.page_resp.dcch_supported = 
      mccap_get_ch_specific_fields(MCCAP_DCCH, &mcc_tx_int_msg.page_resp.dcch_fields);
    mcc_tx_int_msg.page_resp.fch_supported = 
      mccap_get_ch_specific_fields(MCCAP_FCH, &mcc_tx_int_msg.page_resp.fch_fields);
    mcc_tx_int_msg.page_resp.rev_fch_gating_req = mccap_item_is_supported(MCCAP_DF_REV_FCH_GATING);      

    #ifdef FEATURE_USERZONE
    uz_info_ptr = sd_si_uz_info_ptr_get();
    if (uz_info_ptr->uz_id != SD_INVALID_UZID)
    {
      mcc_tx_int_msg.page_resp.uzid_incl = TRUE;
      mcc_tx_int_msg.page_resp.uzid = (word) uz_info_ptr->uz_id;
    }
    #endif /* FEATURE_USERZONE */

  }

  #ifdef FEATURE_IS2000_REL_A
  /* include Release A specific fields, if PREV is >= 7 */
  if (P_REV_IS_7_OR_GREATER)
  {
    mcc_tx_int_msg.page_resp.sts_supported =
      mccap_is_td_mode_supported(CAI_TD_MODE_STS);

    /* 3X is not supported */
    mcc_tx_int_msg.page_resp.thx_cch_supported = mccap_get_thx_cch_supported();

    /* WLL is not supported */
    mcc_tx_int_msg.page_resp.wll_incl = 0;

    /* According to IS-2000 RelA A2 std about enc_info_incl field:     */
    /* "The mobile station shall set this field to 1 if it is unable to*/
    /* determine the base station support for encryption. The mobile   */
    /* station shall set this field to 0 if the base station does not  */
    /* support encryption or the mobile station does not support any of*/
    /* the encryption modes supported by the base station." So we use  */
    /* the following rule to set the field:                            */
    /* 1. MS doesn't support encryption,            set to 0           */
    /* 2. MS supports AUTH/CMEA, BS turns AUTH off, set to 0           */
    /* 3. MS supports AUTH/CMEA, BS turns AUTH on,  set to 1           */
    /* What if there is a access probe handoff? It's our understanding */
    /* that BS should not handoff the mobile to another BS having      */
    /* different encryption config, to prevent MS from repacking data  */

    if ( (mcc_tx_int_msg.page_resp.ami.auth_mode != CAI_NO_AUTH) &&
         (cur_bs_ptr->csp.esp.enc_supported)
       )
    {
 
      mcc_tx_int_msg.page_resp.enc_info_incl = TRUE;

      mccap_get_encryption_cap_info (&enc_capability);
      mcc_tx_int_msg.page_resp.sig_encrypt_sup = enc_capability.sig_enc_sup;
      /* User information encryption in Traffic Channel is not supported. */
      mcc_tx_int_msg.page_resp.ui_encrypt_sup = enc_capability.ui_enc_sup;

      /* We always request encryption if the BS and MS support it */
      mcc_tx_int_msg.page_resp.d_sig_encrypt_req = TRUE;
      mcc_tx_int_msg.page_resp.c_sig_encrypt_req = FALSE;
      mcc_tx_int_msg.page_resp.ui_encrypt_req = FALSE;
        
      #ifdef FEATURE_IS2000_REL_A_AES
#error code not present
      #endif /* FEATURE_IS2000_REL_A_AES */
    }
    else
    {
      mcc_tx_int_msg.page_resp.enc_info_incl = FALSE;
    }

    /* See if SYNC_ID can be specified */
    mcc_tx_int_msg.page_resp.sync_id_incl = FALSE;

    #if defined(FEATURE_IS2000_REL_C_FCS) || defined(FEATURE_IS2000_REL_D)

    #ifdef FEATURE_IS2000_REL_D
#error code not present
    #endif /* FEATURE_IS2000_REL_D */
    {
      mcc_tx_int_msg.page_resp.sync_id_incl = mccsyncid_find_compatible_sync_id(
        TRUE, cdma.so_req, cdma.use_sync_id, &cdma.is_sr_id_valid, &cdma.sr_id,
        &mcc_tx_int_msg.page_resp.sync_id_len, mcc_tx_int_msg.page_resp.sync_id, 
        &sr_id);
    }
    #endif /* FEATURE_IS2000_REL_C_FCS || FEATURE_IS2000_REL_D */

    if (!mcc_tx_int_msg.page_resp.sync_id_incl)
    {
      /* Save a copy of SR_ID */
      cdma.is_sr_id_valid = TRUE;
      cdma.sr_id = CAI_SR_ID_0; /*lint !e641 */
    }
  } /* is PREV_7_OR_GREATER  */
  #endif /* FEATURE_IS2000_REL_A */

  #ifdef FEATURE_IS2000_REL_B
  if (P_REV_IS_8_OR_GREATER)
  {
    if (cur_bs_ptr->csp.esp.band_class_info_req)
    {
      mcc_tx_int_msg.page_resp.alt_band_class_sup = 
        mcc_is_band_class_supported(cur_bs_ptr->csp.esp.alt_band_class);
    }
    else
    {
      mcc_tx_int_msg.page_resp.alt_band_class_sup = 0 ;
    }
  }
  #endif /* FEATURE_IS2000_REL_B */

  #ifdef FEATURE_IS2000_REL_C
  if (P_REV_IS_9_OR_GREATER)
  {
    mcc_tx_int_msg.page_resp.for_pdch_supported = mccap_get_ch_specific_fields(
      MCCAP_FOR_PDCH, &mcc_tx_int_msg.page_resp.for_pdch_fields);
  }
  
  #endif /* FEATURE_IS2000_REL_C */

  #ifdef FEATURE_IS2000_REL_D
#error code not present
  #endif /* FEATURE_IS2000_REL_D */

  plt_rpt_ptr = &mcc_tx_int_msg.page_resp.pilot_rpt;
  msg_len = mccsa_compute_independent_msg_len( &mcc_tx_int_msg, plt_rpt_ptr);

  access_aborted = 
    mccsa_send_access_cmd(&mcc_tx_int_msg, plt_rpt_ptr, FALSE, msg_len, CDMA_PAGE_RES); /*lint !e641 */

  if (access_aborted == NOT_ABORT)
  {
    MSG_HIGH("Sent Page Response", 0, 0, 0);

    /* Save sequence number of last registration message */
    mccsa_layer2.last_reg_msg_seq = mcc_tx_int_msg.page_resp.hdr.msg_seq;

    /* Increment the counter for ACH/EACH messages generated by Layer 3. */
    mccsa_increment_l3_counter(SA_RESPONSE);
  }
  else /* access was aborted */
  {
    /* Failed access - perform registration processing */
    mccreg_idle_ack_or_fail();

    /* Exit to System Determination */
    next_state    = CDMA_EXIT;

    if ( access_aborted == ABORT_SRCH_REQUEST_SYS_RESTART )
    {
      mcc_exit.exit = MCC_SRCH_REQUEST_SYS_RESTART;
    }
    else
    {
      mcc_exit.exit = MCC_ACCESS_DENIED;
    }
  }

  return (next_state);

} /* end mccsa_send_page_response_msg() */

/*===========================================================================

FUNCTION MCCSA_COMPUTE_MAX_ORIG_DIGITS

DESCRIPTION
  This function computes how many digits can fit in the origination message.

DEPENDENCIES
  None

RETURN VALUE
  The number of digits that will fit in the origination message.

SIDE EFFECTS
  None

===========================================================================*/
byte mccsa_compute_max_orig_digits
(
  word msg_size,
  boolean digit_mode
    /* The digit mode being used (4 bit DTMF or 8 bit w/specified format */
)
{
  word max_message_body;
    /* Maximum access channel message body */
  byte max_digits;
    /* Maximum dialed digits that will fit in the origination */
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  /* ---------------------------------------------------------------
  ** According to IS-95-B, we have to assume up to N13m-1 additional
  ** pilots may be reported in any access probes.
  ** --------------------------------------------------------------- */
  if (P_REV_IS_4_OR_GREATER)
  {
    msg_size += (CAI_N13M - 1) * sizeof( cai_plt_rpt_var_type );
  }

  #ifdef FEATURE_IS2000_REL_A_CC
  if (OVHD_CHAN_IS_BCCH)
  {
    max_message_body = mccsa_calculate_maximum_reach_message();
  }
  else
  #endif // FEATURE_IS2000_REL_A_CC

  {
    /* ----------------------------------------------------------
    ** MAX_CAP_SZ is three less than the maximum number of frames
    ** the access channel message capsule can span.  Each frame
    ** can hold 88 data bits.  So the maximum message body length
    ** is, in bits, (MAX_CAP_SZ + 3) * 88 - (size of MSG_LENGTH
    ** field) - (size of CRC field).  See IS-95A section 6.7.1.2.
    ** ---------------------------------------------------------- */
    max_message_body = (cur_bs_ptr->ap.max_cap_sz + 3) * 88 -
                       CAI_AC_CRC_SIZE - CAI_AC_LGTH_SIZE; /*lint !e834 */
  }

  #ifdef FEATURE_IS2000_REL_A_AES
#error code not present
  #endif /* FEATURE_IS2000_REL_A_AES */
  if (digit_mode == 0)
  {
    if (max_message_body < msg_size)
    {
      /* All dialed digits will be included in Origination Continuation */
      max_digits = 0;
    }
    else
    {
      /* Using standard 4 bit DTMF digits */
      max_digits = (byte) ((max_message_body - msg_size) / 4);
    }
  }
  else
  {
    if (max_message_body < msg_size)
    {
      /* All dialed digits will be included in Origination Continuation */
      max_digits = 0;
    }
    else
    {
      /* We need to determine if the digits are 4 or 8 bits */
      max_digits = (byte) ((max_message_body - msg_size) / 8);
    }
  }

  return (max_digits);
} /* mccsa_compute_max_orig_digits() */

/*===========================================================================

FUNCTION MCCSA_SEND_ORIGINATION_MSG

DESCRIPTION
  This function builds an Origination Message.

DEPENDENCIES
  None

RETURN VALUE
  TRUE if the access attempt was aborted, else FALSE.

SIDE EFFECTS
  ERR_FATAL if the service option set to originate is not a valid service
    option.

===========================================================================*/
word mccsa_send_origination_msg
(
  word curr_state
    /* Current state being processed */
)
{
  word mesg_len;

  #ifdef FEATURE_USERZONE
  const sd_si_uz_info_s_type *uz_info_ptr;
  #endif /* FEATURE_USERZONE */

  byte i;
    /* Index variable */
  access_result_type access_aborted;
    /* Flag if access attempt aborted */
  word next_state = curr_state;
    /* Next state to be processed */
  byte max_digits;
    /* Number of digits that will fit in the origination message */
  byte max_alt_so = 0;
  cai_radio_config_type for_rc_pref, rev_rc_pref;

  uint8 sr_id = CAI_SR_ID_1;   /*lint !e641 */

  #ifdef FEATURE_IS2000_REL_A
  caii_enc_cap_type enc_capability;
  #endif /* FEATURE_IS2000_REL_A */

  int8 ota_sci;

  #if defined(FEATURE_IS2000_REL_C_AKA) && defined(FEATURE_IS2000_REL_A_AES)
#error code not present
  #endif /* FEATURE_IS2000_REL_C_AKA && FEATURE_IS2000_REL_A_AES */


/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  /* -----------------------------
  ** Build the Origination Message
  ** ----------------------------- */
  mcc_tx_int_msg.orig.msg_type = mccsa_layer2.last_msg_type = CAI_ORIG_MSG;

  mcc_tx_int_msg.orig.hdr.msg_seq = mcc_layer2.msg_seq = 
    (mcc_layer2.msg_seq + 1) & 0x7; /* Modulo 8 sequence number */

  mcc_tx_int_msg.orig.hdr.ack_seq = mcc_layer2.ack_seq;
  mcc_tx_int_msg.orig.hdr.ack_type = mcc_layer2.ack_type;
  mcc_tx_int_msg.orig.hdr.ack_req = TRUE;

  #ifdef FEATURE_GPSONE_NEW_L2_ACK
#error code not present
  #else
  mcc_tx_int_msg.orig.hdr.valid_ack = FALSE;
  #endif /* FEATURE_GPSONE_NEW_L2_ACK */

  mcc_set_address(&mcc_tx_int_msg.orig.hdr, mcc_tx_int_msg.orig.msg_type);

  mcc_tx_int_msg.orig.mob_term = mccreg_get_mob_term();

  ota_sci = mcc_convert_pref_sci_to_ota_sci();
  
  #ifdef FEATURE_MODIFIED_SCI
#error code not present
  #endif /* FEATURE_MODIFIED_SCI */

  {    
    mcc_tx_int_msg.orig.slot_cycle_index = ((ota_sci > 0) ? ota_sci : -1 * ota_sci); /*lint !e732*/

    #ifdef FEATURE_IS2000_REL_D
#error code not present
    #endif /* FEATURE_IS2000_REL_D */

    /* Record the reported slot cycle index that used in later operational SCI
       update */
    mccreg_set_reg_sci(ota_sci);
  }

  mcc_tx_int_msg.orig.mob_p_rev = cdma.mob_cai_rev;

  /* ------------------------------------------------------
  ** For IS-95B backward compatibility:
  ** 1. If the phone is JSTD008 only or is talking to a JSTD
  **    BS ( P_REV_IN_USE is 1 ), we send 0xa0 as SCM.
  ** 2. If the phone has P_REV_IN_USE greater or equal 4,
  **    then we should send scm and set MSB to 1.
  ** -------------------------------------------------------*/
  /*lint -e506 */

  if (!mcc_set_scm_for_band_class(cdma.band_class, &(mcc_tx_int_msg.orig.scm)))
  {
    mcc_tx_int_msg.orig.scm = 0xa0;
  }

  mcc_tx_int_msg.orig.request_mode = sd_misc_get_cdma_request_mode();

  /**************************************************************
    This function checks to see if the SO is in the SNM, and enabled.
    If the SO is not in the SNM, it will check to make sure it is valid
    using srv_eval_so.

    It will return FALSE if:
      a) The SO is in the SNM, but is not enabled (ex: QC proprietary SO
         in a non-Proprietary phone, SO# 0x8020, 0x8021, 0x8022)
      b) The SO is not in the SNM, and srv returns a different value for
         it (ex: RS2 voice is requested, but RS2 is not supported. srv_eval_so
         will return 8k voice or EVRC, so this function will return FALSE.
  **************************************************************/

  if (snm_so_is_enabled(cdma.so_req))
  {
    if (cdma.so_req == CAI_SO_VOICE_IS96A)
    {
      mcc_tx_int_msg.orig.special_service = FALSE;
    }
    else
    {
      mcc_tx_int_msg.orig.special_service = TRUE;
      mcc_tx_int_msg.orig.service_option  = cdma.so_req;
    }
  }
  else
  {
    /* This can only happen inthe case of a wildcard voice origination where
    NV variables for home/roam orig SO has been corrupted, so the best
    thing to do at this point make sure the origination fails, so set an
    invalid SO in the origination message */
    ERR( "Invalid SO requested:%d, Making it 0", cdma.so_req, 0, 0 );
    mcc_tx_int_msg.orig.special_service = TRUE;
    mcc_tx_int_msg.orig.service_option  = CAI_SO_REJ;
  }

  /* The number of alternative service options is calculated here */
  /* since this is needed when computing the maximum number of    */
  /* origination digits.  Note: there is no P_REV check here, as  */
  /* the fields will not be included in the outgoing message if   */
  /* the p_rev_in_use is under 4 (anything before 95B)            */
  if ( (OVHD_CHAN_IS_PCH && SP_MSG_IS_RECVD && ESP_MSG_IS_RECVD 
       && cur_bs_ptr->csp.sp.ext_sys_parameter)
       #ifdef FEATURE_IS2000_REL_A_CC
       || (OVHD_CHAN_IS_BCCH && ANSI41SP_MSG_IS_RECVD)
       #endif /* FEATURE_IS2000_REL_A_CC */
     )
  {
    if (P_REV_IS_4_OR_GREATER)
    {
      if (CAI_AC_ALT_SO_MAX < cur_bs_ptr->csp.esp.max_num_alt_so)
      {
        max_alt_so = CAI_AC_ALT_SO_MAX;
      }
      else
      {
        max_alt_so = cur_bs_ptr->csp.esp.max_num_alt_so;
      }
    }
    else
    {
      max_alt_so = 0;
    }

    #ifdef FEATURE_IS2000_REL_A
    if (!P_REV_IS_7_OR_GREATER) //lint !e506 !e774
    #endif /* FEATURE_IS2000_REL_A */
    {
      if (max_alt_so > 0)
      {
        mcc_tx_int_msg.orig.num_alt_so =
          snm_get_alt_so_list(cdma.so_req, max_alt_so, mcc_tx_int_msg.orig.alt_so);
      }
      else
      {
        mcc_tx_int_msg.orig.num_alt_so = 0x00;
      }
    }

    #ifdef FEATURE_IS2000_REL_A
    else /* P_REV_IS_7_OR_GREATER */
    {
      /* First, find all the alternate Service Options available.*/
      /* Note here the max_alt_so doesn't govern the number of   */
      /* SOs we might include in the SO_BITMAP. So we try to find*/
      /* as many alternate SOs as available.                     */
      mcc_tx_int_msg.orig.num_alt_so =
        snm_get_alt_so_list(cdma.so_req, CAI_AC_ALT_SO_MAX,
                            mcc_tx_int_msg.orig.alt_so);

      if (mcc_tx_int_msg.orig.num_alt_so > 0)
      {
        /* Now that we've decided what alternative Service Options MS
          will report, we need to convert those SO's to the bitmap to
          save space.

          Following function takes the first three parameters as input,
          fills the next three using them. It also may modify the
          first two parameters num_alt_so and alt_so. If any SO can not
          be represented in bitmap format, it is returned in alt_so
          byte array. Total number of such SO'a are returned in
          num_alt_so.

          As a result all fields related to alternative SO's and
          SO bitmap will be populated in this function. */

        snm_convert_alt_so_bitmap (
          cdma.so_req,                    // primary Service Option
          &mcc_tx_int_msg.orig.num_alt_so,    // total number of alt SO's
          mcc_tx_int_msg.orig.alt_so,         // byte array that contains
                                          // alternative SO's.
          &mcc_tx_int_msg.orig.so_bitmap_ind, // tells the length of the bitmap
          &mcc_tx_int_msg.orig.so_group_num,  // SO Group number that main SO
                                          // belongs to.
          &mcc_tx_int_msg.orig.so_bitmap      // SO bitmap
        );

        /* We have stuffed as many alt SOs in the SO_BITMAP as allowed */
        /* Now make sure we report the right number of SOs in the      */
        /* alt_so list                                                 */
        if (max_alt_so < mcc_tx_int_msg.orig.num_alt_so)
        {
          mcc_tx_int_msg.orig.num_alt_so = max_alt_so;
        }
      }
      else
      {
        mcc_tx_int_msg.orig.so_bitmap_ind = SO_BITMAP_SIZE_0;
      }
    }
    #endif /* FEATURE_IS2000_REL_A */

  }

  /* -----------------------------------------------------
  ** Set Digit Mode Indicator for 4-bit DTMF or ASCII mode
  ** -----------------------------------------------------*/
  mcc_tx_int_msg.orig.digit_mode = cdma.digit_mode;

  #ifdef FEATURE_AUTH
  mcc_tx_int_msg.orig.ami.auth_mode = cur_bs_ptr->rp.auth;
  #else
  mcc_tx_int_msg.orig.ami.auth_mode = CAI_NO_AUTH;
  #endif /* FEATURE_AUTH */

  if (P_REV_IS_6_OR_GREATER)
  {
    // We need to send this info only if the p_rev in use
    // is greater than 6
    mcc_tx_int_msg.orig.uzid_incl = 0;

    /* Set CH_IND and EXT_CH_IND */
    snm_get_pref_chan_ind_for_so(cdma.so_req, &mcc_tx_int_msg.orig.ch_ind
      #ifdef FEATURE_IS2000_REL_C_DV
#error code not present
      #endif /* FEATURE_IS2000_REL_C_DV */
      );

    snm_get_pref_fundicated_RCs(cdma.so_req, &for_rc_pref, &rev_rc_pref);
    mcc_tx_int_msg.orig.for_rc_pref = for_rc_pref; /*lint !e641 */
    mcc_tx_int_msg.orig.rev_rc_pref = rev_rc_pref; /*lint !e641 */
    //revisit
    mcc_tx_int_msg.orig.otd_supported = mccap_item_is_supported(MCCAP_DF_OTD); // No OTD support
    mcc_tx_int_msg.orig.qpch_supported = mccap_item_is_supported(MCCAP_DF_QPCH); // QPCH support
    mcc_tx_int_msg.orig.enhanced_rc = TRUE; // Enhanced RC support
    mcc_tx_int_msg.orig.dcch_supported =
      mccap_get_ch_specific_fields(MCCAP_DCCH, &mcc_tx_int_msg.orig.dcch_fields);
    mcc_tx_int_msg.orig.fch_supported =
      mccap_get_ch_specific_fields(MCCAP_FCH, &mcc_tx_int_msg.orig.fch_fields);

    #ifdef FEATURE_IS2000_REL_A
    /* include Release A specific fields, if PREV is >= 7 */
    if (P_REV_IS_7_OR_GREATER)
    {
      /* Geo Location capability is determined compile-time. User
         preferences will be ignored at the time of capability reporting */
      mcc_tx_int_msg.orig.geo_loc_incl = TRUE;
      mcc_tx_int_msg.orig.geo_loc_type = mccap_get_geo_loc_cap();

      /* Silent Redial information is reflected from CM as received */
      if (cdma.orig_reason)
      {
        mcc_tx_int_msg.orig.orig_reason = CAI_SILENT_ORIG_ATTEMPT_ARQ_FAILURE;
        mcc_tx_int_msg.orig.orig_count = cdma.orig_count;
      }
      else  /* Not a silent redial attempt */
      {
        mcc_tx_int_msg.orig.orig_reason = CAI_NON_SILENT_ORIG_ATTEMPT_NON_ARQ_FAILURE;
        mcc_tx_int_msg.orig.orig_count = 0;
      }

      /* Indicate if mobile supports STS */
      mcc_tx_int_msg.orig.sts_supported =
          mccap_is_td_mode_supported(CAI_TD_MODE_STS);

      /* 3X is not supported */
      mcc_tx_int_msg.orig.thx_cch_supported = mccap_get_thx_cch_supported();

      /* WLL is not supported */
      mcc_tx_int_msg.orig.wll_incl = FALSE;

      /* CM has indicated an emergency call.  */
      if (cdma.is_emergency)
      {
        mcc_tx_int_msg.orig.global_emergency_call = TRUE;

        /* Note that this field is only included in case of an emergency call
           This flag indicates if the MS is to initiate a position location
           determination session with an emergency call. In the current
           design, MS does not start a position location determination session
           with this emergency call, hence this flag will always be zero. */
        mcc_tx_int_msg.orig.ms_init_pos_loc_ind =  0;
      }
      else
      {
        mcc_tx_int_msg.orig.global_emergency_call = 0;
      }

      /* If Data Services had requested to propose QoS parameters and if
         Base Station allows to propose, Qos Parameters are included */
      if (cdma.qos_parms_incl)
      {
        mcc_tx_int_msg.orig.qos_parms_incl = cdma.qos_parms_incl;
        mcc_tx_int_msg.orig.qos_parms_len = cdma.qos_parms_len;
        (void) memcpy( mcc_tx_int_msg.orig.qos_parms, cdma.qos_parms, mcc_tx_int_msg.orig.qos_parms_len
          );
      }
      else
      {
         /* Quality of Service parameters are not included */
         mcc_tx_int_msg.orig.qos_parms_incl = 0;
      }

      /* According to IS-2000 RelA A2 std about enc_info_incl field:     */
      /* "The mobile station shall set this field to 1 if it is unable to*/
      /* determine the base station support for encryption. The mobile   */
      /* station shall set this field to 0 if the base station does not  */
      /* support encryption or the mobile station does not support any of*/
      /* the encryption modes supported by the base station." So we use  */
      /* the following rule to set the field:                            */
      /* 1. MS doesn't support encryption,            set to 0           */
      /* 2. MS supports AUTH/CMEA, BS turns AUTH off, set to 0           */
      /* 3. MS supports AUTH/CMEA, BS turns AUTH on,  set to 1           */
      /* What if there is a access probe handoff? It's our understanding */
      /* that BS should not handoff the mobile to another BS having      */
      /* different encryption config, to prevent MS from repacking data  */

      if ( (mcc_tx_int_msg.orig.ami.auth_mode != CAI_NO_AUTH) &&
           (cur_bs_ptr->csp.esp.enc_supported)
         )
      {

        mcc_tx_int_msg.orig.enc_info_incl = TRUE;

        mccap_get_encryption_cap_info ( &enc_capability );
        mcc_tx_int_msg.orig.sig_encrypt_sup = enc_capability.sig_enc_sup;
        /* User information encryption in Traffic Channel is not supported. */
        mcc_tx_int_msg.orig.ui_encrypt_sup = enc_capability.ui_enc_sup;

        /* We always request encryption if the BS and MS support it */
        mcc_tx_int_msg.orig.d_sig_encrypt_req = TRUE;
        mcc_tx_int_msg.orig.c_sig_encrypt_req = FALSE;
        mcc_tx_int_msg.orig.ui_encrypt_req = FALSE;
          
        #ifdef FEATURE_IS2000_REL_A_AES
#error code not present
        #endif /* FEATURE_IS2000_REL_A_AES */        
      }
      else
      {
        mcc_tx_int_msg.orig.enc_info_incl = FALSE;
      }

      #if defined(FEATURE_IS2000_REL_C_FCS) || defined(FEATURE_IS2000_REL_D)
      if (mcc_tx_int_msg.orig.service_option != CAI_SO_REJ)
      {
        /* See if SYNC_ID can be specified */
        mcc_tx_int_msg.orig.sync_id_incl = mccsyncid_find_compatible_sync_id(
          TRUE, cdma.so_req, cdma.use_sync_id, &cdma.is_sr_id_valid, 
          &cdma.sr_id, &mcc_tx_int_msg.orig.sync_id_len, 
          mcc_tx_int_msg.orig.sync_id, &mcc_tx_int_msg.orig.sr_id);
      }
      else
      #endif /* FEATURE_IS2000_REL_C_FCS || FEATURE_IS2000_REL_D */
      {
        mcc_tx_int_msg.orig.sync_id_incl = FALSE;
      }

      /* Following three fields are only included in case of dormant packet
         data handoff. MS is required to include the previous SID, NID and
         the Packet Zone ID if there has been a change as a result of dormant
         packet data handoff.

         If the MS had successfully originated on a new system (this means it had
         received a Layer 2 ack as a response to a Packet Data Origination),
         old values must have been updated now.

         At this point if these parameters were modified, a dormant packet data
         handoff must have occured. Standard mandates inclusion of the previous
         values in outgoing Origination message if this is a packet data call.
         */

      if (cdma.is_dormant_packet_data)
      {
        /* Start with SID */
        if (mccsyobj_get_sid() != mccsyobj_get_old_pd_orig_sid())
        {
          mcc_tx_int_msg.orig.prev_sid_incl = TRUE;
          mcc_tx_int_msg.orig.prev_sid = mccsyobj_get_old_pd_orig_sid();
        }
        else
        {
          mcc_tx_int_msg.orig.prev_sid_incl = FALSE;
        }

        /* check NID */
        if (mccsyobj_get_nid() != mccsyobj_get_old_pd_orig_nid())
        {
          mcc_tx_int_msg.orig.prev_nid_incl = TRUE;
          mcc_tx_int_msg.orig.prev_nid = mccsyobj_get_old_pd_orig_nid();
        }
        else
        {
          mcc_tx_int_msg.orig.prev_nid_incl = FALSE;
        }

        /* and Packet Zone Id */
        if (mccsyobj_get_pzid() != mccsyobj_get_old_pd_orig_pzid())
        {
          mcc_tx_int_msg.orig.prev_pzid_incl = TRUE;
          mcc_tx_int_msg.orig.prev_pzid = mccsyobj_get_old_pd_orig_pzid();
        }
        else
        {
          mcc_tx_int_msg.orig.prev_pzid_incl = FALSE;
        }

      } /* is dormant packet data */
      else
      {
        mcc_tx_int_msg.orig.prev_sid_incl = FALSE;
        mcc_tx_int_msg.orig.prev_nid_incl = FALSE;
        mcc_tx_int_msg.orig.prev_pzid_incl = FALSE;
      }
    } /* is PREV_7_OR_GREATER  */
    #endif /* FEATURE_IS2000_REL_A */

    mcc_tx_int_msg.orig.drs = cdma.drs;
    mcc_tx_int_msg.orig.rev_fch_gating_req = mccap_item_is_supported(MCCAP_DF_REV_FCH_GATING);

    #ifdef FEATURE_USERZONE
    uz_info_ptr = sd_si_uz_info_ptr_get();
    if (uz_info_ptr->uz_id != SD_INVALID_UZID)
    {
      mcc_tx_int_msg.orig.uzid_incl = TRUE;
      mcc_tx_int_msg.orig.uzid = (word)uz_info_ptr->uz_id;
    }
    #endif /* FEATURE_USERZONE */

  }

  #ifdef FEATURE_IS2000_REL_B
  if (P_REV_IS_8_OR_GREATER)
  {
    if (cur_bs_ptr->csp.esp.band_class_info_req)
    {
      mcc_tx_int_msg.orig.alt_band_class_sup = mcc_is_band_class_supported(cur_bs_ptr->csp.esp.alt_band_class);
    }
    else
    {
      mcc_tx_int_msg.orig.alt_band_class_sup = 0 ;
    }

    mcc_tx_int_msg.orig.sdb_desired_only = 0 ;
  }
  #endif /* FEATURE_IS2000_REL_B */

  #ifdef FEATURE_IS2000_REL_C
  if (P_REV_IS_9_OR_GREATER)
  {
    mcc_tx_int_msg.orig.for_pdch_supported =
    mccap_get_ch_specific_fields(MCCAP_FOR_PDCH, &mcc_tx_int_msg.orig.for_pdch_fields);
  }
  #endif /* FEATURE_IS2000_REL_C */

  #ifdef FEATURE_IS2000_REL_D
#error code not present
  #endif /* FEATURE_IS2000_REL_D */
  
  /* We do this so that compute_independe..() returns as accurate as possible. */
  mcc_tx_int_msg.orig.num_fields = 0;
  mesg_len = mccsa_compute_independent_msg_len( &mcc_tx_int_msg, &(mcc_tx_int_msg.orig.pilot_rpt));

  max_digits = mccsa_compute_max_orig_digits(mesg_len, mcc_tx_int_msg.orig.digit_mode);

  if (cdma.dial.num_digits > max_digits)
  {
    mcc_tx_int_msg.orig.num_fields = max_digits;
    mcc_tx_int_msg.orig.more_fields = TRUE;
    cdma.remaining_digits = cdma.dial.num_digits - max_digits;
  }
  else
  {
    mcc_tx_int_msg.orig.num_fields = cdma.dial.num_digits;
    mcc_tx_int_msg.orig.more_fields = FALSE;
    cdma.remaining_digits = 0;
  }

  for (i=0; i < mcc_tx_int_msg.orig.num_fields; i++)
  {
    mcc_tx_int_msg.orig.chari[i] = cdma.dial.digits[i];
  }

  #ifdef FEATURE_GPS_BS_CAL_TEST
#error code not present
  #endif /* FEATURE_GPS_BS_CAL_TEST */

  /* Now add num of digits to the message len based on digit mode */
  mesg_len += (word) (mcc_tx_int_msg.orig.num_fields *
                      ((mcc_tx_int_msg.orig.digit_mode+1) * 4));

  /* auth_data is calculated and stored here as we need this data to
  ** process the re-authentication request  */

  #if defined(FEATURE_AUTH) && defined (FEATURE_OTASP) && \
      defined(FEATURE_AUTH_DIGITS)
  cdma.auth_data  = mcc_calculate_auth_data(mcc_tx_int_msg.orig.chari,
    mcc_tx_int_msg.orig.num_fields, mcc_tx_int_msg.orig.digit_mode );
  #else
  cdma.auth_data = 0;
  #endif /* FEATURE_AUTH */

  mcc_tx_int_msg.orig.number_type = cdma.number_type;
  mcc_tx_int_msg.orig.number_plan = cdma.number_plan;

  /* --------------------------------------------------------------
  ** Indicate mobile not capable of a CDMA to narrow analog handoff
  ** -------------------------------------------------------------- */
  mcc_tx_int_msg.orig.nar_an_cap = FALSE;

  /* Setup authentication and message integrity fields */
  if (curr_state != (next_state = mccsa_setup_auth_msg_int_fields(curr_state, 
      &mcc_tx_int_msg, 0, mcc_tx_int_msg.orig.num_fields, 
      mcc_tx_int_msg.orig.chari, mcc_tx_int_msg.orig.digit_mode, 
      mcc_tx_int_msg.orig.num_fields, &mcc_tx_int_msg.orig.ami)))
  {
    /* Abort access now before it even begins */
    MSG_MED("Orig interrupted during auth_sig", 0, 0, 0);
    return (next_state);
  }

  #ifdef FEATURE_IS2000_REL_A
  /* Setup encryption fields */
  mcc_tx_int_msg.orig.enc.enc_fields_incl = FALSE;

  #ifdef FEATURE_IS2000_REL_A_AES
#error code not present
  #endif /* FEATURE_IS2000_REL_A_AES */
 
  /* If AES is enabled, but MI is not supported or enabled, this message must be proposing
   * a new SSEQ_H for AES use. Now it is the time to set the AES status fields of 
   * pending message type and pending message SSEQ_H
   */
  #ifdef FEATURE_IS2000_REL_A_AES
#error code not present
  #endif /* FEATURE_IS2000_REL_A_AES */

  #endif /* FEATURE_IS2000_REL_A */

  #if defined(FEATURE_IS2000_REL_C_AKA) || defined(FEATURE_IS2000_REL_A_AES)
#error code not present
  #endif /* FEATURE_IS2000_REL_C_AKA || FEATURE_IS2000_REL_A_AES */

  {
    /* Setup voice privacy fields and kick off calculating authentication key */
    mcc_tx_int_msg.orig.pm = 
      #ifdef FEATURE_AUTH 
      mcc_setup_vpm_and_akey( TRUE, cur_bs_ptr->rp.auth );
      #else
      mcc_setup_vpm_and_akey( TRUE, FALSE);
      #endif /* FEATURE_AUTH */
  }

  // For special MDR, following is appended regardless of P_REV
  if ( P_REV_IS_4_OR_GREATER )
  {
    mcc_tx_int_msg.orig.paca_reorig = 0;
    mcc_tx_int_msg.orig.return_cause = sd_misc_get_cdma_return_cause();
    MSG_HIGH("Return Cause: %d", mcc_tx_int_msg.orig.return_cause, 0, 0);
    mcc_tx_int_msg.orig.more_records = 0;
    if (mcc_tx_int_msg.orig.ami.auth_mode != CAI_NO_AUTH)
    {
      mcc_tx_int_msg.orig.encry_sup = 0x00;
    }
    /* stay 0 until PACA feature is implemented */
    mcc_tx_int_msg.orig.paca_sup = 0;
  }

  #ifdef FEATURE_IS2000_REL_A
  /*
  ** Let's find out what SR_ID to propose. The rule is:
  ** (1) If the call is a data call, use the supplied SR_ID.
  ** (2) if request for restoring service configuration,
  **     MC already allocated SR_ID. 
  ** (3) Otherwise, get one from the SRID Manager for (SVD)
  **     or hard code for non-SVD build. 
  */

  sr_id = CAI_SR_ID_1; /*lint !e641 */

  if (cdma.is_sr_id_valid)
  {
    sr_id = cdma.sr_id;
  }
  else
  {
    #ifdef FEATURE_IS2000_REL_A_SVD 
    /* CM does not supply an SR_ID for DS. Allocate one from SR_ID manager */
    sr_id = mccsrid_allocate_srid(SRID_OWNER_MC);

    if (sr_id == MCCSRID_INVALID_SRID)
    {
      /* No SR_ID available. Quit */
      MSG_ERROR("Invalid SR_ID", 0, 0, 0);
      mcc_exit.exit = MCC_ACCESS_FAILED;
      return CDMA_EXIT;
    }
    else
    {
      /* MC allocates SR_ID for UI, but not for DS. DS takes care of
      ** allocation & deallocation.
      */
      MSG_MED("SRID %d allocated", sr_id, 0, 0);
    }
    #endif /* FEATURE_IS2000_REL_A_SVD */

    /* Save a copy */
    cdma.is_sr_id_valid = TRUE;
    cdma.sr_id = sr_id;  
  }
  #endif /* FEATURE_IS2000_REL_A */

  if ( P_REV_IS_6_OR_GREATER 
       #ifdef FEATURE_IS2000_REL_C
       && (!P_REV_IS_9_OR_GREATER || !mcc_tx_int_msg.orig.sync_id_incl)
       #endif /* FEATURE_IS2000_REL_C */
     )
  {
    mcc_tx_int_msg.orig.sr_id = sr_id;
  }

  plt_rpt_ptr = &mcc_tx_int_msg.orig.pilot_rpt;
  access_aborted =
    mccsa_send_access_cmd(&mcc_tx_int_msg, plt_rpt_ptr, TRUE, mesg_len, CDMA_ORIG); /*lint !e641 */

  if (access_aborted == NOT_ABORT)
  {
    MSG_HIGH( "Sent Origination Message", 0, 0, 0 );

    /* Save sequence number of last registration message */
    mccsa_layer2.last_reg_msg_seq = mcc_tx_int_msg.orig.hdr.msg_seq;

    /* Increment the counter for ACH/EACH messages generated by Layer 3. */
    mccsa_increment_l3_counter(SA_REQUEST);

    /* Perform some initialization for Service Negotiation Manager. */
    (void)snm_orig_init( cdma.so_req );
  }
  else /* access was aborted */
  {
    /* Failed access - perform registration processing */
    mccreg_idle_ack_or_fail();

    /* Exit to System Determination */
    next_state    = CDMA_EXIT;

    if ( access_aborted == ABORT_SRCH_REQUEST_SYS_RESTART )
    {
      mcc_exit.exit = MCC_SRCH_REQUEST_SYS_RESTART;
    }

    #ifdef FEATURE_IS2000_REL_A 
    else if (access_aborted == ABORT_ACCT_BLOCK)
    {
      MSG_MED( "ACCT blocked", 0, 0, 0 );
      /* Call blocked due to ACCT, Change exit reason */
      mcc_exit.exit = MCC_ACCT_BLOCKED;
    }
    #endif /* FEATURE_IS2000_REL_A */

    else
    {
      mcc_exit.exit = MCC_ACCESS_DENIED;
    }

    #ifdef FEATURE_PASSPORT_MODE
    if(mcc_passport_mode_is_jcdma())
    {    
      /* Save the failure history by Persistence Test */
      if (access_aborted == ABORT_PSIST)
      {
        mccsa_persistence_orig_ng = TRUE;
      }
    }
    #endif /* FEATURE_PASSPORT_MODE */

  }

  return (next_state);

} /* end mccsa_send_origination_msg() */

/*===========================================================================

FUNCTION MCCSA_SEND_REGISTRATION_MSG

DESCRIPTION
  This function build a Registration Message.

DEPENDENCIES
  None

RETURN VALUE
  TRUE if the access attempt was aborted, else FALSE.

SIDE EFFECTS
  None

===========================================================================*/
word mccsa_send_registration_msg
(
  word curr_state
    /* Current state being processed */
)
{
  #ifdef FEATURE_IS2000_REL_A
  caii_enc_cap_type enc_capability;
  #endif /* FEATURE_IS2000_REL_A */

  word msg_len;
    /* Length of bit-packed message */
  access_result_type access_aborted;
    /* Flag if access attempt aborted */
  mccsa_l3_counter_type counter_type;
  boolean request;
    /* Flag for request or response access */
  word next_state = curr_state;
    /* Next state to be processed */

  #ifdef FEATURE_USERZONE
  const sd_si_uz_info_s_type *uz_info_ptr;
  #endif /* FEATURE_USERZONE */

  int8 ota_sci;

  #if defined(FEATURE_IS2000_REL_C_AKA) && defined(FEATURE_IS2000_REL_A_AES)
#error code not present
  #endif /* FEATURE_IS2000_REL_C_AKA && FEATURE_IS2000_REL_A_AES */

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  /* ------------------------------
  ** Build the Registration Message
  ** ------------------------------ */
  mcc_tx_int_msg.reg.msg_type = mccsa_layer2.last_msg_type = CAI_REG_MSG;

  mcc_tx_int_msg.reg.hdr.msg_seq = mcc_layer2.msg_seq = 
    (mcc_layer2.msg_seq + 1) & 0x7; /* Modulo 8 sequence number */

  mcc_tx_int_msg.reg.hdr.ack_seq = mcc_layer2.ack_seq;
  mcc_tx_int_msg.reg.hdr.ack_type = mcc_layer2.ack_type;
  mcc_tx_int_msg.reg.hdr.ack_req = TRUE;

  if (cdma.reg_type == CAI_ORDERED_REG)
  {
    mcc_tx_int_msg.reg.hdr.valid_ack = TRUE;
    request = FALSE;
    counter_type = SA_RESPONSE;
  }
  else
  {
    #ifdef FEATURE_GPSONE_NEW_L2_ACK
#error code not present
    #else
    mcc_tx_int_msg.reg.hdr.valid_ack = FALSE;
    #endif /* FEATURE_GPSONE_NEW_L2_ACK */

    request = TRUE;
    counter_type = SA_REQUEST;
  }

  mcc_set_address(&mcc_tx_int_msg.reg.hdr, mcc_tx_int_msg.reg.msg_type);

   /* Setup authentication and message integrity fields */
  if (curr_state != (next_state = mccsa_setup_auth_msg_int_fields(curr_state, 
      &mcc_tx_int_msg, 0, 0, NULL, 0, 0, &mcc_tx_int_msg.reg.ami)))
  {
    /* Abort access now before it even begins */
    MSG_MED("Registration interrupted during auth_sig", 0, 0, 0);
    return (next_state);
  }

  #ifdef FEATURE_IS2000_REL_A
  /* Setup encryption fields */
  mcc_tx_int_msg.reg.enc.enc_fields_incl = FALSE;

  #ifdef FEATURE_IS2000_REL_A_AES
#error code not present
  #endif /* FEATURE_IS2000_REL_A */

  /* If AES is enabled, but MI is not supported or enabled, this message must be proposing
   * a new SSEQ_H for AES use. Now it is the time to set the AES status fields of 
   * pending message type and pending SSEQ_H
   */
  #ifdef FEATURE_IS2000_REL_A_AES
#error code not present
  #endif /* FEATURE_IS2000_REL_A_AES */

  #endif /* FEATURE_IS2000_REL_A */

  #if defined(FEATURE_IS2000_REL_C_AKA) || defined(FEATURE_IS2000_REL_A_AES)
#error code not present
  #endif /* FEATURE_IS2000_REL_C_AKA || FEATURE_IS2000_REL_A_AES */

  mcc_tx_int_msg.reg.reg_type = cdma.reg_type;

  ota_sci = mcc_convert_pref_sci_to_ota_sci();
  
  #ifdef FEATURE_MODIFIED_SCI
#error code not present
  #endif /* FEATURE_MODIFIED_SCI */
  {
    mcc_tx_int_msg.reg.slot_cycle_index = ((ota_sci > 0) ? ota_sci : -1 * ota_sci); /*lint !e732*/

    #ifdef FEATURE_IS2000_REL_D
#error code not present
    #endif /* FEATURE_IS2000_REL_D */

    /* Record the reported slot cycle index that used in later operational SCI
       update */
    mccreg_set_reg_sci(ota_sci);
  }

  mcc_tx_int_msg.reg.mob_p_rev = cdma.mob_cai_rev;

  /* ------------------------------------------------------
  ** For IS-95B backward compatibility:
  ** 1. If the phone is JSTD008 only or is talking to a JSTD
  **    BS ( P_REV_IN_USE is 1 ), we send 0xa0 as SCM.
  ** 2. If the phone has P_REV_IN_USE greater or equal 4,
  **    then we should send scm and set MSB to 1.
  ** -------------------------------------------------------*/
                                       /*lint -e506 */
  if (!mcc_set_scm_for_band_class(cdma.band_class, &(mcc_tx_int_msg.reg.scm)))
  {
    mcc_tx_int_msg.reg.scm = 0xa0;
  }

  mcc_tx_int_msg.reg.mob_term         = mccreg_get_mob_term();

  if (P_REV_IS_4_OR_GREATER)
  {
    /* Reason for the registration or access */
    mcc_tx_int_msg.reg.return_cause = sd_misc_get_cdma_return_cause();
  }

  if (P_REV_IS_6_OR_GREATER)
  {
    mcc_tx_int_msg.reg.qpch_supported = mccap_item_is_supported(MCCAP_DF_QPCH);
    mcc_tx_int_msg.reg.enhanced_rc = 1;
    mcc_tx_int_msg.reg.uzid_incl = 0;

    #ifdef FEATURE_USERZONE
    uz_info_ptr = sd_si_uz_info_ptr_get();
    if (uz_info_ptr->uz_id != SD_INVALID_UZID)
    {
      mcc_tx_int_msg.reg.uzid_incl = TRUE;
      mcc_tx_int_msg.reg.uzid = (word)uz_info_ptr->uz_id;
    }
    #endif /* FEATURE_USERZONE */

  }

  #ifdef FEATURE_IS2000_REL_A
  /* include Release A specific fields, if PREV is >= 7 */
  if (P_REV_IS_7_OR_GREATER)
  {
    /* Geo Location capability is determined compile-time. User
       preferences will be ignored at the time of capability reporting */
    mcc_tx_int_msg.reg.geo_loc_incl = TRUE;
    mcc_tx_int_msg.reg.geo_loc_type = mccap_get_geo_loc_cap();

    /* Indicate if mobile supports STS */
    mcc_tx_int_msg.reg.sts_supported =
      mccap_is_td_mode_supported(CAI_TD_MODE_STS);

    /* Indicate if mobile supports OTD */
    mcc_tx_int_msg.reg.otd_supported =
      mccap_is_td_mode_supported(CAI_TD_MODE_OTD);

    /* 3X is not supported */
    mcc_tx_int_msg.reg.thx_cch_supported = mccap_get_thx_cch_supported();

    /* WLL is not supported */
    mcc_tx_int_msg.reg.wll_incl = 0;

    /* According to IS-2000 RelA A2 std about enc_info_incl field:     */
    /* "The mobile station shall set this field to 1 if it is unable to*/
    /* determine the base station support for encryption. The mobile   */
    /* station shall set this field to 0 if the base station does not  */
    /* support encryption or the mobile station does not support any of*/
    /* the encryption modes supported by the base station." So we use  */
    /* the following rule to set the field:                            */
    /* 1. MS doesn't support encryption,            set to 0           */
    /* 2. MS supports AUTH/CMEA, BS turns AUTH off, set to 0           */
    /* 3. MS supports AUTH/CMEA, BS turns AUTH on,  set to 1           */
    /* What if there is a access probe handoff? It's our understanding */
    /* that BS should not handoff the mobile to another BS having      */
    /* different encryption config, to prevent MS from repacking data  */

    if ( (mcc_tx_int_msg.reg.ami.auth_mode != CAI_NO_AUTH) &&
         (cur_bs_ptr->csp.esp.enc_supported)
       )

    {
      mcc_tx_int_msg.reg.enc_info_incl = TRUE;

      mccap_get_encryption_cap_info ( &enc_capability );
      mcc_tx_int_msg.reg.sig_encrypt_sup = enc_capability.sig_enc_sup;
      /* User information encryption in Traffic Channel is not supported. */
      mcc_tx_int_msg.reg.ui_encrypt_sup = enc_capability.ui_enc_sup;

      /* Following field will only be set if MS request encryption on
       * signaling encryption to be turned on for signaling messages sent
       * on f-csch and r-csch. */
      mcc_tx_int_msg.reg.c_sig_encrypt_req = FALSE;
        
      #ifdef FEATURE_IS2000_REL_A_AES
#error code not present
      #endif /* FEATURE_IS2000_REL_A_AES */
    }
    else
    {
      mcc_tx_int_msg.reg.enc_info_incl = FALSE;
    }
  } /* is PREV_7_OR_GREATER  */
  #endif /* FEATURE_IS2000_REL_A */

  #ifdef FEATURE_IS2000_REL_D
#error code not present
  #endif

  plt_rpt_ptr = &mcc_tx_int_msg.reg.pilot_rpt;
  msg_len = mccsa_compute_independent_msg_len( &mcc_tx_int_msg, plt_rpt_ptr);
  access_aborted = mccsa_send_access_cmd(&mcc_tx_int_msg, plt_rpt_ptr, request, msg_len,
                   CDMA_REG); /*lint !e641 */

  if ( access_aborted == NOT_ABORT )
  {
    MSG_HIGH( "Sent Registration Msg type %d", cdma.reg_type, 0, 0 );

    /* Save sequence number of last registration message */
    mccsa_layer2.last_reg_msg_seq = mcc_tx_int_msg.reg.hdr.msg_seq;

    /* Increment the counter for ACH/EACH messages generated by Layer 3. */
    mccsa_increment_l3_counter(counter_type);
  }
  else /* access was aborted */
  {
    /* Failed access - perform registration processing */
    mccreg_idle_ack_or_fail();

    /* Exit to System Determination */
    next_state    = CDMA_EXIT;

    if ((curr_state == CDMA_REG) && (cdma.reg_type == CAI_POWERDOWN_REG)) /*lint !e641 */
    {
      if (cdma.powerdown_reg_type == CDMA_NORMAL)
      {
        /* --------------------------------------------------------------
        ** If this was a powerdown registration exit CDMA mode with
        ** powerdown indication regardless of success of access
        ** -------------------------------------------------------------- */
        mcc_exit.exit = MCC_POWERDOWN;
      }
      else if (cdma.powerdown_reg_type == CDMA_SHUTDOWN)
      {
        /* --------------------------------------------------------------
        ** If this was a powerdown registration exit CDMA mode with
        ** shutdown indication regardless of success of access
        ** -------------------------------------------------------------- */
        mcc_exit.exit = MCC_SHUTDOWN;
      }

      else if (cdma.powerdown_reg_type == CDMA_LPM)
      {
        /* --------------------------------------------------------------
        ** if this was a powerdown registration because of entry into
        ** Low Power Mode, exit CDMA mode with exit reason as LPM
        ** -------------------------------------------------------------- */
        mcc_exit.exit = MCC_LPM;
      }
      
    }

    else if ( access_aborted == ABORT_SRCH_REQUEST_SYS_RESTART )
    {
      mcc_exit.exit = MCC_SRCH_REQUEST_SYS_RESTART;
    }

    else
    {
      mcc_exit.exit = MCC_ACCESS_DENIED;
    }
  }

  return (next_state);

} /* mccsa_send_registration_msg() */

#ifdef FEATURE_IS2000_REL_C_AKA
#error code not present
#endif /* FEATURE_IS2000_REL_C_AKA */
 
#ifdef FEATURE_IS2000_REL_A
/*===========================================================================

FUNCTION

DESCRIPTION

DEPENDENCIES
  None.

RETURN VALUE
  
SIDE EFFECTS
  None.

===========================================================================*/
word mccsa_build_sec_mode_req_msg
(
  word    curr_state
)
{
  word    next_state = curr_state;

  #if defined(FEATURE_IS2000_REL_C_AKA) && defined(FEATURE_IS2000_REL_A_AES)
#error code not present
  #endif /* FEATURE_IS2000_REL_C_AKA && FEATURE_IS2000_REL_A_AES */

  /* ---------------------------------------------------------------- */

  /* Build the Security Mode Request Message  */
  mcc_tx_int_msg.smrm.msg_type = mccsa_layer2.last_msg_type = CAI_AC_SMRM_MSG;

  mcc_tx_int_msg.smrm.hdr.msg_seq = mcc_layer2.msg_seq = 
    (mcc_layer2.msg_seq + 1) & 0x7; /* Modulo 8 sequence number */

  mcc_tx_int_msg.smrm.hdr.ack_seq = mcc_layer2.ack_seq;
  mcc_tx_int_msg.smrm.hdr.ack_type = mcc_layer2.ack_type;
  mcc_tx_int_msg.smrm.hdr.ack_req = TRUE;
  mcc_tx_int_msg.smrm.hdr.valid_ack = TRUE;

  mcc_set_address(&mcc_tx_int_msg.smrm.hdr, mcc_tx_int_msg.smrm.msg_type);

  /* Setup authentication and message integrity fields */
  if (curr_state != (next_state = mccsa_setup_auth_msg_int_fields(curr_state, 
      &mcc_tx_int_msg, 0, 0, NULL, 0, 0, 
      &mcc_tx_int_msg.smrm.ami)))
  {
    /* Abort access now before it even begins */
    MSG_MED("Sec mode req msg interrupted during auth_sig", 0, 0, 0);
  }

  /* Setup encryption fields */
  mcc_tx_int_msg.smrm.enc.enc_fields_incl = FALSE;

  #ifdef FEATURE_IS2000_REL_A_AES
#error code not present
  #endif /* FEATURE_IS2000_REL_A_AES */

  /* If AES is enabled, but MI is not supported or enabled, this message must be proposing
   * a new SSEQ_H for AES use. Now it is the time to set the AES status fields of 
   * pending message type and pending SSEQ_H
   */
  #ifdef FEATURE_IS2000_REL_A_AES
#error code not present
  #endif /* FEATURE_IS2000_REL_A_AES */

  #if defined(FEATURE_IS2000_REL_C_AKA) || defined(FEATURE_IS2000_REL_A_AES)
#error code not present
  #endif /* FEATURE_IS2000_REL_C_AKA || FEATURE_IS2000_REL_A_AES */

  return (next_state);

} /* mccsa_build_sec_mod_req_msg */
#endif /* FEATURE_IS2000_REL_A */
 
/*===========================================================================

FUNCTION MCCSA_SETUP_ORDER

DESCRIPTION
  This function builds an Order message.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  Fills in the appropriate fields of the message buffer that is going
  to be transmitted.  A pointer to this buffer is passed in as an
  argument to the function.

===========================================================================*/
word mccsa_setup_order
(
  word curr_state,
    /* Current state */
  caii_tx_msg_type *tx_int_msg_ptr
    /* Pointer to Internal format Order Response Message */
)
{
  word next_state = curr_state; /* Next state to be processed */

  /* -------------------------------------------------------------
  ** cdma.sa_resp_ord holds specifics about the order to transmit.
  ** ------------------------------------------------------------- */

  /*********************
    * Set up the header *
    *********************/

  tx_int_msg_ptr->ac_ord.gen.msg_type = mccsa_layer2.last_msg_type = 
    CAI_AC_ORD_MSG;
  mccsa_layer2.last_ord_type = cdma.sa_resp_ord.gen.order;

  tx_int_msg_ptr->ac_ord.gen.hdr.msg_seq = mcc_layer2.msg_seq = 
    (mcc_layer2.msg_seq + 1) & 0x7; /* Modulo 8 sequence number */

  tx_int_msg_ptr->ac_ord.gen.hdr.ack_seq = mcc_layer2.ack_seq;
  tx_int_msg_ptr->ac_ord.gen.hdr.ack_req = TRUE;
  tx_int_msg_ptr->ac_ord.gen.hdr.ack_type= mcc_layer2.ack_type;

  /* Setup authentication and message integrity fields */
  if (curr_state != (next_state = mccsa_setup_auth_msg_int_fields(
      curr_state, &mcc_tx_int_msg, 0, 0, NULL, 0, 0,
      &mcc_tx_int_msg.ac_ord.gen.ami)))
  {
    /* Abort access now before it even begins */
    MSG_MED("Order response interrupted during auth_sig", 0, 0, 0);
    return (next_state);
  }

  #ifdef FEATURE_IS2000_REL_A
  /* Setup encryption fields */
  mcc_tx_int_msg.ac_ord.gen.enc.enc_fields_incl = FALSE;

  #ifdef FEATURE_IS2000_REL_A_AES
#error code not present
  #endif /* FEATURE_IS2000_REL_A_AES */

  #endif /* FEATURE_IS2000_REL_A */

  /* -------------------------------------------------
  ** valid_ack could be false if we are rejecting a
  ** message which did not require an acknowledgement.
  ** ------------------------------------------------- */
  tx_int_msg_ptr->ac_ord.gen.hdr.valid_ack = mcc_layer2.valid_ack;

  mcc_set_address(&tx_int_msg_ptr->ac_ord.gen.hdr, tx_int_msg_ptr->ac_ord.gen.msg_type);

  /*********************************
    * Fill in order specific fields *
    *********************************/

  switch ( cdma.sa_resp_ord.gen.order )
  {
    case CAI_MS_REJECT_ORD:
      tx_int_msg_ptr->ac_ord.rej.order        = CAI_MS_REJECT_ORD;
      tx_int_msg_ptr->ac_ord.rej.ordq         = cdma.sa_resp_ord.rej.ordq;
      tx_int_msg_ptr->ac_ord.rej.rej_msg_type =
        cdma.sa_resp_ord.rej.rej_msg_type;
      tx_int_msg_ptr->ac_ord.rej.rejected_pdu_type =
        cdma.sa_resp_ord.rej.rejected_pdu_type;

      if ( cdma.sa_resp_ord.rej.rej_msg_type == CAI_PC_ORD_MSG )
      {
        /* ----------------------------------------------
        ** We are rejecting an order, so we must include
        ** the order type and ordq of the rejected order.
        ** ---------------------------------------------- */
        tx_int_msg_ptr->ac_ord.rej_ord.rej_order =
          cdma.sa_resp_ord.rej_ord.rej_order;
        tx_int_msg_ptr->ac_ord.rej_ord.rej_ordq  =
          cdma.sa_resp_ord.rej_ord.rej_ordq;
      }
      break;

    case CAI_BS_CHAL_ORD:
      tx_int_msg_ptr->ac_ord.chal.order = CAI_BS_CHAL_ORD;
      tx_int_msg_ptr->ac_ord.chal.ordq = cdma.sa_resp_ord.chal.ordq;
      tx_int_msg_ptr->ac_ord.chal.randbs = cdma.sa_resp_ord.chal.randbs;
      break;

    case CAI_MS_ACK_ORD:
    case CAI_SSD_UPDATE_ORD:
    case CAI_LOCAL_CTL_RESP_ORD:
      tx_int_msg_ptr->ac_ord.gen.order = cdma.sa_resp_ord.gen.order;
      tx_int_msg_ptr->ac_ord.ordq.ordq = cdma.sa_resp_ord.ordq.ordq;
      break;

    case CAI_RELEASE_ORD:
      tx_int_msg_ptr->ac_ord.ordq.order = CAI_RELEASE_ORD;
      tx_int_msg_ptr->ac_ord.ordq.ordq  = cdma.sa_resp_ord.ordq.ordq;
      break;

    #ifdef FEATURE_IS2000_REL_D
#error code not present
    #endif /* FEATURE_IS2000_REL_D */

    #ifdef FEATURE_IS2000_REL_C
    case CAI_SEC_MODE_CMP_ORD:
      tx_int_msg_ptr->ac_ord.gen.order = cdma.sa_resp_ord.gen.order;
      tx_int_msg_ptr->ac_ord.ordq.ordq = cdma.sa_resp_ord.ordq.ordq;
      break;
    #endif /* FEATURE_IS2000_REL_C */

    default:
      ERR_FATAL ("Tried to respond with illegal order type %d",
        cdma.sa_resp_ord.gen.order, 0, 0 );
  } /* end switch (order type) */

 return next_state;

} /* mccsa_setup_order() */

/*===========================================================================

FUNCTION MCCSA_SEND_ORDER_RESPONSE_MSG

DESCRIPTION
  This function builds an Order/Message Response message.

DEPENDENCIES
  None

RETURN VALUE
  TRUE if the access attempt was aborted, else FALSE.

SIDE EFFECTS
  None

===========================================================================*/

word mccsa_send_order_response_msg
(
  word curr_state,
    /* Current state being processed */
  boolean          request     
    /* Flag for request or response */
)
{
  word msg_len=0;
    /* Length of bit-packed message */
  access_result_type access_aborted;
    /* Flag if access attempt aborted */
  word max_message_body;
   /* Maximum number of bits message can have */
  word next_state = curr_state;
    /* Next state to be processed */

  #ifdef FEATURE_AUTH
  boolean reject_auth_chal = FALSE;
    /* Indicator if we must reject the auth challenge message */
  auth_cmd_type *auth_cmd_ptr;
    /* Pointer to authentication command */
  #ifdef FEATURE_UIM_RUIM
  word i;
    /* index variable */
  byte auth_mode_param = CAI_AUTH;
    /* Send to mccsa_wait_for_gen_auth_signature to determine if signature
       is valid.  Auth mode is on by default. */
  #endif /*FEATURE_UIM_RUIM */
  #endif /*FEATURE_AUTH*/

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  /* -----------------------------------------
  ** Build the Order/Message Response Message
  ** ----------------------------------------- */

  /* ----------------------------------------------------
  ** cdma.sa_resp_msg holds the msg_type to respond with.
  ** ---------------------------------------------------- */
  switch (cdma.sa_resp_msg)
  {
    case CAI_AC_TMSI_CMP_MSG:
      /* Build TMSI Assignment Completion message */

      /* Check if authentication completed successfully */
      /* before continuing with transmit process.       */
      if (curr_state != (next_state = mccsa_build_tmsi_assign_cmp(curr_state)))
      {
        /* Abort access now before it even begins */
        return (next_state);
      }
      plt_rpt_ptr = &mcc_tx_int_msg.ac_tmsi_cmp.pilot_rpt;
      msg_len = mccsa_compute_independent_msg_len( &mcc_tx_int_msg, plt_rpt_ptr);
      break;

    case CAI_AC_ORD_MSG:
      if (curr_state != (next_state = mccsa_setup_order(curr_state, &mcc_tx_int_msg)))
      {
        /* Abort access now before it even begins */
        return (next_state);
      }
      plt_rpt_ptr = &mcc_tx_int_msg.ac_ord.gen.pilot_rpt;
      msg_len = mccsa_compute_independent_msg_len( &mcc_tx_int_msg, plt_rpt_ptr);
      break;

    #ifdef FEATURE_AUTH
    case CAI_AC_AUTH_RESP_MSG:
      /* --------------------------------------------------------
      ** First we generate authu.  If the auth_signature fails we
      ** send a reject order, otherwise we send an Authentication
      ** Challenge Response Order.
      ** -------------------------------------------------------- */
      if ((auth_cmd_ptr = (auth_cmd_type*) q_get( &auth_free_q )) != NULL)
      {
        #ifdef FEATURE_UIM_RUIM
        #if defined( FEATURE_UIM_RUN_TIME_ENABLE )
        /* Determine if the R-UIM is available for this NAM */
        if (nv_rtre_use_ruim_for_nam(cdma.curr_nam))
        #endif /* FEATURE_UIM_RUN_TIME_ENABLE */
        {
          auth_cmd_ptr->hdr.command = AUTH_SIGNATURE_F;
          auth_cmd_ptr->sig.rand_type = RUIM_UNIQUE_RAND_CHAL;
          auth_cmd_ptr->sig.rand_chal = cdma.randu;
          auth_cmd_ptr->sig.dig_len = 0;
          for (i = 0; i < 3 ; i++)
          {
             auth_cmd_ptr->sig.digits[i] = 0;
          }
          auth_cmd_ptr->sig.process_control = RUIM_DEFLT_PROC_CNTRL;
          auth_cmd_ptr->sig.esn = cdma.esn;
          auth_cmd_ptr->sig.rpt_function = mcc_queue_auth_rpt;
        }

        #if defined( FEATURE_UIM_RUN_TIME_ENABLE )
        else /* R-UIM is not available */
        #endif /* FEATURE_UIM_RUN_TIME_ENABLE */
        #endif /* FEATURE_UIM_RUIM */

        #if !defined( FEATURE_UIM_RUIM ) || defined( FEATURE_UIM_RUN_TIME_ENABLE )
        {
          auth_cmd_ptr->hdr.command = AUTH_SIGNATURE_F;
          auth_cmd_ptr->sig.rand_chal = (cdma.randu << 8) |
                            (mcc_get_auth_imsi_s2() & 0xFF);

          #ifndef FEATURE_AUTH_DIGITS
          auth_cmd_ptr->sig.auth_data = 0;
          #else
          auth_cmd_ptr->sig.auth_data = mcc_get_auth_imsi_s1();
          #endif /* FEATURE_AUTH_DIGITS */

          #ifdef FEATURE_IS2000_REL_D
#error code not present
          #else
          auth_cmd_ptr->sig.save_reg = FALSE;
          #endif /* FEATURE_IS2000_REL_D */
          auth_cmd_ptr->sig.rpt_function = mcc_queue_auth_rpt;
        }
        #endif /* !FEATURE_UIM_RUIM || FEATURE_UIM_RUN_TIME_ENABLE */

        mcc_auth_cmd( auth_cmd_ptr );

        if (auth_cmd_ptr->hdr.status == AUTH_RECEIVED_S)
        {
          next_state = mccsa_wait_for_gen_auth_signature( &mcc_tx_int_msg.auth_ch.authu,
            #ifdef FEATURE_UIM_RUIM
            &auth_mode_param,
            #endif /* FEATURE_UIM_RUIM */
            curr_state );

          if (next_state != curr_state)
          {
            /* Abort access now before it even begins */
            MSG_MED( "auth_resp interrupted during auth_sig", 0, 0, 0 );
            return (next_state);
          }

          #ifdef FEATURE_UIM_RUIM
          #if defined( FEATURE_UIM_RUN_TIME_ENABLE )
          /* Determine if the R-UIM is available for this NAM */
          if (nv_rtre_use_ruim_for_nam(cdma.curr_nam))
          #endif /* FEATURE_UIM_RUN_TIME_ENABLE */
          {
            /* Check the auth_mode_param to see if the signature is valid */
            if (auth_mode_param != CAI_AUTH)
            {
              /* -----------------------------------------------
              ** We can not get a valid signature,
              ** so we should reject this message.
              ** ----------------------------------------------- */
              ERR( "Cannot get a valid signature", 0,0,0);
              reject_auth_chal = TRUE;
            }
          }
          #endif /* FEATURE_UIM_RUIM */
        }
        else
        {
          /* -----------------------------------------------
          ** We have a serious problem and can not calculate
          ** authu, so we should reject this message.
          ** ----------------------------------------------- */
          ERR( "Bad status from AUTH %d", auth_cmd_ptr->hdr.status,0,0);
          reject_auth_chal = TRUE;
        }
      }
      else
      {
        ERR( "No free buffers on auth_free_q", 0, 0, 0 );
        reject_auth_chal = TRUE;
      }

      if (reject_auth_chal)
      {
        /* -------------------------------------------
        ** Something has gone wrong and we must reject
        ** the Authentication Challenge Message, so we
        ** setup a reject order instead of the
        ** Authentication Response Message.
        ** ------------------------------------------- */
        cdma.sa_resp_msg                      = CAI_AC_ORD_MSG;
        cdma.sa_resp_ord.rej.order            = CAI_MS_REJECT_ORD;
        cdma.sa_resp_ord.rej.ordq             = CAI_REJ_UNSP;
        cdma.sa_resp_ord.rej.rej_msg_type     = CAI_PC_AUTH_MSG;
        cdma.sa_resp_ord.rej.rejected_pdu_type= 0; // 20 ms pdu type

        if (curr_state != (next_state = mccsa_setup_order(curr_state, &mcc_tx_int_msg)))
        {
          /* Abort access now before it even begins */
          return (next_state);
        }

        plt_rpt_ptr = &mcc_tx_int_msg.ac_ord.gen.pilot_rpt;
        msg_len = mccsa_compute_independent_msg_len( &mcc_tx_int_msg, plt_rpt_ptr);
      }
      else
      {
        /* -------------------------------------------------------
        ** Build an Access Channel Authentication Response Message
        ** ------------------------------------------------------- */
        mcc_tx_int_msg.auth_ch.msg_type = mccsa_layer2.last_msg_type = 
          CAI_AC_AUTH_RESP_MSG;

        mcc_tx_int_msg.auth_ch.hdr.ack_seq = mcc_layer2.ack_seq;

        mcc_tx_int_msg.auth_ch.hdr.msg_seq = mcc_layer2.msg_seq = 
          (mcc_layer2.msg_seq + 1) & 0x7;

        mcc_tx_int_msg.auth_ch.hdr.ack_req = TRUE;
        mcc_tx_int_msg.auth_ch.hdr.valid_ack = mcc_layer2.valid_ack;
        mcc_tx_int_msg.auth_ch.hdr.ack_type = mcc_layer2.ack_type;

        mcc_set_address(&mcc_tx_int_msg.auth_ch.hdr, mcc_tx_int_msg.auth_ch.msg_type);

        /* Setup authentication and message integrity fields */
        if (curr_state != (next_state = mccsa_setup_auth_msg_int_fields(
            curr_state, &mcc_tx_int_msg, 0, 0, NULL, 0, 0,
            &mcc_tx_int_msg.auth_ch.ami)))
        {
          /* Abort access now before it even begins */
          MSG_MED("Auth Resp interrupted during auth_sig", 0, 0, 0);
          return (next_state);
        }

        #ifdef FEATURE_IS2000_REL_A
        /* Setup encryption fields */
        mcc_tx_int_msg.auth_ch.enc.enc_fields_incl = FALSE;

        #ifdef FEATURE_IS2000_REL_A_AES
#error code not present
        #endif /* FEATURE_IS2000_REL_A_AES */

        #endif /* FEATURE_IS2000_REL_A  */

        plt_rpt_ptr = &mcc_tx_int_msg.auth_ch.pilot_rpt;
        msg_len = mccsa_compute_independent_msg_len( &mcc_tx_int_msg, plt_rpt_ptr);
      }

      break;
    #endif /* FEATURE_AUTH */

    case CAI_AC_STRSP_MSG:
    case CAI_AC_EXT_STRSP_MSG:

      /* Fill in header fields for status response message */
      mcc_tx_int_msg.ac_strsp.msg_type =
      mccsa_layer2.last_msg_type   = cdma.sa_resp_msg;

      /* Transfer saved (extended if IS95B) status response message to tx_int_msg */
      mcc_tx_int_msg.ac_strsp = cdma_strsp;

      /* Fill in header fields for (extended if IS95B) status response message */
      mcc_tx_int_msg.ac_strsp.hdr.ack_seq  = mcc_layer2.ack_seq;

      mcc_tx_int_msg.ac_strsp.hdr.msg_seq  = mcc_layer2.msg_seq =
        (mcc_layer2.msg_seq + 1) & 0x7;

      mcc_tx_int_msg.ac_strsp.hdr.ack_req   = TRUE;
      mcc_tx_int_msg.ac_strsp.hdr.valid_ack = mcc_layer2.valid_ack;
      mcc_tx_int_msg.ac_strsp.hdr.ack_type  = mcc_layer2.ack_type;

      mcc_set_address(&mcc_tx_int_msg.ac_strsp.hdr, mcc_tx_int_msg.ac_strsp.msg_type);

       /* Setup authentication and message integrity fields */
      if (curr_state != (next_state = mccsa_setup_auth_msg_int_fields(
          curr_state, &mcc_tx_int_msg, 0, 0, NULL, 0, 0, 
          &mcc_tx_int_msg.ac_strsp.ami)))
      {
        /* Abort access now before it even begins */
        MSG_MED("Status response interrupted during auth_sig", 0, 0, 0);
        return (next_state);
      }

      #ifdef FEATURE_IS2000_REL_A
      /* Only for Extended Status Response Message, it needs to setup encryption fields */
      if ( cdma.sa_resp_msg == CAI_AC_EXT_STRSP_MSG )
      {
        mcc_tx_int_msg.ac_ext_strsp.enc.enc_fields_incl = FALSE;

        #ifdef FEATURE_IS2000_REL_A_AES
#error code not present
        #endif /* FEATURE_IS2000_REL_A_AES */
      }
      #endif /* FEATURE_IS2000_REL_A  */

      plt_rpt_ptr = &mcc_tx_int_msg.ac_strsp.pilot_rpt;
      msg_len = mccsa_compute_independent_msg_len(&mcc_tx_int_msg, plt_rpt_ptr);

      #ifdef FEATURE_IS2000_REL_A_CC
      if (OVHD_CHAN_IS_BCCH)
      {
        max_message_body = mccsa_calculate_maximum_reach_message();
      }
      else
      #endif // FEATURE_IS2000_REL_A_CC

      {
        /* ----------------------------------------------------------
        ** MAX_CAP_SZ is three less than the maximum number of frames
        ** the access channel message capsule can span.  Each frame
        ** can hold 88 data bits.  So the maximum message body length
        ** is, in bits, (MAX_CAP_SZ + 3) * 88 - (size of MSG_LENGTH
        ** field) - (size of CRC field).  See IS-95A section 6.7.1.2.
        ** ---------------------------------------------------------- */
        max_message_body = (cur_bs_ptr->ap.max_cap_sz + 3) * 88 -
                           CAI_AC_CRC_SIZE - CAI_AC_LGTH_SIZE; /*lint !e834 */
      }

     /* msg_len that we pass into mccsa_send_access_cmd need not include the
        plt reporting fix fields but we need that length to see if this
        status response is small enough, so have another temporary
        variable */
     if (msg_len > max_message_body)
     {

        /* --------------------------------------------------
        ** The STRESP message has grown bigger than the Access
        ** Channel Capsule size, so build the MS Reject Order.
        ** -------------------------------------------------- */
        cdma.sa_resp_msg                      = CAI_AC_ORD_MSG;
        cdma.sa_resp_ord.rej.order            = CAI_MS_REJECT_ORD;

        if(P_REV_IS_4_OR_GREATER)      /* new for IS-95B */
        {
          cdma.sa_resp_ord.rej.ordq           = CAI_REJ_LEN;
        /* This is a new ordq for IS-95B indicating the capsule is too large */
        }
        else
        {
          cdma.sa_resp_ord.rej.ordq           = CAI_REJ_UNSP;
        /* This check is to keep older P_REV phones backwards compatible */
        }

        cdma.sa_resp_ord.rej.rej_msg_type     = CAI_PC_STREQ_MSG;
        cdma.sa_resp_ord.rej.rejected_pdu_type= 0; //20ms regular pdu type

        if (curr_state != (next_state = mccsa_setup_order(curr_state, &mcc_tx_int_msg)))
        {
          /* Abort access now before it even begins */
          return (next_state);
        }

        plt_rpt_ptr = &mcc_tx_int_msg.ac_ord.gen.pilot_rpt;
        msg_len = mccsa_compute_independent_msg_len( &mcc_tx_int_msg, plt_rpt_ptr);
     }

     break;

    #ifdef FEATURE_IS2000_REL_C_AKA
#error code not present
    #endif /* FEATURE_IS2000_REL_C_AKA */

    default:
      ERR_FATAL ("Tried to respond with unexpected message type %d",
          cdma.sa_resp_msg, 0, 0 );
      break;

  } /* end switch (response message type) */

  access_aborted = mccsa_send_access_cmd( &mcc_tx_int_msg, plt_rpt_ptr, request,
                                    msg_len, CDMA_ORD_RES ); /*lint !e641 */

  if ( access_aborted == NOT_ABORT )
  {
    MSG_HIGH( "Sent order/resp msg type %d", cdma.sa_resp_msg, 0, 0 );
    if (cdma.sa_resp_msg == CAI_AC_ORD_MSG)
    {
      MSG_HIGH( "Order type %d", cdma.sa_resp_ord.gen.order, 0, 0 );
    }

    /* Increment the counter for ACH/EACH messages generated by Layer 3. */
    mccsa_increment_l3_counter(SA_REQUEST);
  }
  else /* access was aborted */
  {
    /* Failed access - perform registration processing */
    mccreg_idle_ack_or_fail();

    /* Exit to System Determination */
    next_state    = CDMA_EXIT;

    if ( access_aborted == ABORT_SRCH_REQUEST_SYS_RESTART )
    {
      mcc_exit.exit = MCC_SRCH_REQUEST_SYS_RESTART;
    }
    else
    {
      mcc_exit.exit = MCC_ACCESS_DENIED;
    }
  }

  return (next_state);

} /* mccsa_send_order_response_msg() */

#ifdef FEATURE_AUTH
/*===========================================================================

FUNCTION MCCSA_AUTH_PROC_MSG

DESCRIPTION
  This function performs Order and Message Processing while waiting for
  authentication procedures to complete.

DEPENDENCIES
  None

RETURN VALUE
  The next state to be processed.

SIDE EFFECTS
  None

===========================================================================*/

word mccsa_auth_proc_msg
(
  mccrxtx_msg_type *msg_ptr,
    /* Pointer to received message message */
  word curr_state
    /* Current state being processed */
)
{
  word i;
    /* Index through messages */
  boolean record_found = FALSE;
    /* Indicator of whether an order or channel assignment record
       directed to the mobile station was found */
  cm_mc_rpt_type *cmd_ptr;
    /* Pointer to handset command */
  word next_state = curr_state;

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  #ifdef FEATURE_IS2000_REL_A_CC
  if (OVHD_CHAN_IS_BCCH &&
      (msg_ptr->msg.gen.msg_type != CAI_UPM_FIRST_SEG_MSG) &&
      (msg_ptr->msg.gen.msg_type != CAI_UPM_MIDDLE_SEG_MSG) &&
      (msg_ptr->msg.gen.msg_type != CAI_UPM_FINAL_SEG_MSG))
  {
    /* ----------------------------------------------------------------------
    ** Empty segmented Universal Page Message reassembly buffer if received
    ** message is not a UPM First Segment Message, UPM Middle Segment Message
    ** and UPM Final Segment.
    ** ---------------------------------------------------------------------- */
    mcc_empty_seg_upm_buffer();
  }
  #endif /* FEATURE_IS2000_REL_A_CC */

  if (msg_ptr->msg.gen.msg_type == CAI_PC_ORD_MSG)
  {
    /* -----------------------------------------------------------
    ** Paging Channel Order Message --> search for an order record
    ** addressed to this mobile station
    ** ----------------------------------------------------------- */
    for (i=0; i < msg_ptr->msg.pc_ord.num_ords; i++)
    {
      if (mcc_comp_addr(&msg_ptr->msg.pc_ord.ords[i].gen.hdr
          ,NULL /*this field is only needed for BC SMS */
         ))
      {
        /* -----------------------------------------
        ** Found an order record addressed to mobile
        ** ----------------------------------------- */
        record_found = TRUE;
        break;
      }
    } /* end for */

    if ((record_found) &&
        (!mcc_detect_duplicate( FALSE, &msg_ptr->msg.pc_ord.ords[i].gen.hdr )))
    {
      /* -----------------------------------------------------------------
      ** Increment the count PAG_3 or FCCCH_3 unless we have a BS_ACK_ORD.
      ** ----------------------------------------------------------------- */
      if (msg_ptr->msg.pc_ord.ords[i].gen.order != CAI_BS_ACK_ORD)
      {
        mcc_increment_pag_3_or_fccch_3();
      }
      /* Log the matched message */
      mclog_report_matched_msg(msg_ptr->msg.gen.msg_type,MCLOG_MSG_MATCH);

      /* ------------------------------------------------
      ** The only order we process here is the Lock Until
      ** Power Cycled Order.  We ignore any other order.
      ** ------------------------------------------------ */
      if ((msg_ptr->msg.pc_ord.ords[i].gen.order == CAI_LOCK_OR_MAINT_ORD) &&
          (msg_ptr->msg.pc_ord.ords[i].ordq.ordq != CAI_UNLOCK_MASK) &&
          (msg_ptr->msg.pc_ord.ords[i].ordq.ordq & CAI_LUPC_MASK))
      {
        /* -----------------------------
        ** Lock Until Power-Cycled Order
        ** ----------------------------- */
        MSG_MED( "Rxed Lock Order in OVHD", 0, 0, 0 );
        next_state     = CDMA_EXIT;
        mcc_exit.exit = MCC_LOCK;
        mcc_nv_data.lckrsn_p.lock_order = TRUE;
        mcc_nv_data.lckrsn_p.lock_reason =
          msg_ptr->msg.pc_ord.ords[i].ordq.ordq & 0x0F;

        /* Write reason for lock to NVRAM */
        mcc_nv_buf.item = NV_LCKRSN_P_I;
        mcc_nv_buf.cmd = NV_WRITE_F;
        mcc_nv_buf.data_ptr = &mcc_nv_data;

        (void) mcc_nv_cmd( &mcc_nv_buf );

        /* Send lock command to UI */
        if ((cmd_ptr = cm_mc_rpt_get_buf_else_err_fatal()) != NULL)
        {
          cmd_ptr->hdr.cmd = CM_LOCK_F;
          mcc_cm_mc_rpt( cmd_ptr );
        }
        else
        {
          ERR("No free buffers on cm_mc_rpt_free_q",0,0,0 );
        }

        /* Events report: Lock */
        event_report(EVENT_LOCK);

      } /* end if (we have a lock order) */
    } /* end if (record_found) && (!mcc_detect_duplicate)) */
  } /* end if (msg_ptr->msg.gen.msg_type == CAI_PC_ORD_MSG) */

  else if ((msg_ptr->msg.gen.msg_type == CAI_PAGE_MSG) ||
           (msg_ptr->msg.gen.msg_type == CAI_SLT_PAGE_MSG))
  {
    if (curr_state != CDMA_PAGE_RES) /*lint !e641 */
    {
    /* -------------------------------------------
    ** If we are not already handling a page there
    ** is a chance we are supposed to drop what we
    ** are doing and handle this page.
    ** ------------------------------------------- */
      next_state = mccsa_proc_page(msg_ptr, curr_state);
    }
  }
  #ifdef FEATURE_BROADCAST_SMS
  else if (msg_ptr->msg.gen.msg_type == CAI_PC_BURST_MSG) 
  {
    boolean continue_processing = FALSE;

    continue_processing = mcc_match_msg(msg_ptr, 
                                        &mccsa_matched_msg, 
                                        (
                                         #ifdef FEATURE_IS2000_REL_D
#error code not present
                                         #endif /* FEATURE_IS2000_REL_D */
                                         TRUE), curr_state);

    if ((continue_processing) &&
        (mccsa_matched_msg.gen_pc.hdr.addr_type == CAI_BCAST_ADDR_TYPE))
    {
      MSG_HIGH("Rxed broadcast msg in mccsa_auth_proc_msg()", 0, 0, 0);
      next_state = mccsa_process_data_burst(&mccsa_matched_msg, curr_state);
    }
  }
  #endif /* FEATURE_BROADCAST_SMS */
  /*  We ignore any other message */

  return (next_state);

} /* mccsa_auth_proc_msg */

/*===========================================================================

FUNCTION MCCSA_WAIT_FOR_GEN_AUTH_SIGNATURE

DESCRIPTION
  This procedure waits for the authentication task to return an
  authentication signature value to be put in a transmittied message.
  While waiting we check for various other events and may decide to
  change states.  If this occurs the access attempt we were going to
  start is aborted.  If all goes well we receive the auth_signature
  and place it in the buffer supplied as one of the parameters.

DEPENDENCIES
  None.

RETURN VALUE
  The next state to be processed.

SIDE EFFECTS
  Coppies the auth signature value returned from the authentication task
  into a buffer pointed to by the first parameter.

===========================================================================*/
word mccsa_wait_for_gen_auth_signature
(
  dword *auth,
    /* Where to place computed auth signature value */

  #ifdef FEATURE_UIM_RUIM
  byte *auth_mode,
    /* Points to auth mode field in access message */
  #endif /* FEATURE_UIM_RUIM */

  word curr_state
    /* current state being processed */
)
{
  static mccrxtx_cmd_type *rx_ptr;
    /* Pointer to received message */
  mccrx_rpt_type *rpt_ptr;
    /* Pointer to Paging Channel fade report */
  auth_rpt_type  *auth_rpt_ptr;
    /* Pointer to authentication report. */
  mc_msg_type *mc_cmd_ptr;
    /* Pointer to UI command received on mc_cmd_q */
  dword sigs;
    /* Return signals from mcc_wait() call */
  boolean auth_sig_rxed = FALSE;
    /* Indicates if auth signature received */
  word next_state = curr_state;

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  /* Set the flag as we are waiting for Auth signature */
  mccsa_gen_auth_signature_in_progress = TRUE;

  /* -----------------------------------------------------------------
  ** We have not yet begun the access attempt, and if we change states
  ** before we get the auth_signature, this access will be aborted
  ** before it starts.  If we switch to another system access state
  ** (e.g. from CDMA_REG to CDMA_ORIG) we will return to the main
  ** state machine and perform that access instead.
  ** ----------------------------------------------------------------- */

  while ((next_state == curr_state) && (!auth_sig_rxed))
  {
    {
      sigs = mcc_wait( MCC_RXTX_Q_SIG   | MCC_RX_RPT_Q_SIG | 
                       MCC_CMD_Q_SIG | MCC_AUTH_Q_SIG );
    }

    /* ----------------------------------------------
    ** NOTE:  We ignore Searcher during System Access
    ** substates and just leave the commands on queue
    ** ---------------------------------------------- */

    /* ---------------------------------------------------------
    ** Check to see if we have received a Paging Channel message
    ** --------------------------------------------------------- */
    if ((sigs & MCC_RXTX_Q_SIG) != 0)
    {
      if ((rx_ptr = (mccrxtx_cmd_type*) q_get( &mcc_rxtx_q )) != NULL)
      {
        /* ---------------------------------------------------
        ** Process Paging Channel message and check if we need
        ** to stop accesses
        ** --------------------------------------------------- */
        if (rx_ptr->hdr.command == MCCRXTX_MSG_F)
        {
          next_state = mccsa_auth_proc_msg(&rx_ptr->msg, curr_state);
        }

        #if defined(FEATURE_IS2000_REL_C_AKA) || defined(FEATURE_IS2000_REL_A_AES)
#error code not present
        #endif /* FEATURE_IS2000_REL_C_AKA || FEATURE_IS2000_REL_A_AES */
    
        else
        {
          ERR( "Invalid RXTX command %d", rx_ptr->hdr.command, 0, 0 );
        }
        cmd_done( &rx_ptr->hdr.cmd_hdr );
      }
    } /* end if ((sigs & MCC_RXTX_Q_SIG) != 0) */

    /* ---------------------------------------------
    ** Check to see if we have received a UI command
    ** --------------------------------------------- */
    else if ((sigs & MCC_CMD_Q_SIG) != 0)
    {
      if ((mc_cmd_ptr = (mc_msg_type*) q_get( &mc_cmd_q )) != NULL)
      {
        next_state = mccsa_hs_proc( mc_cmd_ptr, curr_state );
      }
    } /* end else if ((sigs & MCC_CMD_Q_SIG) != 0) */

    /* --------------------------------------
    ** Check for a Paging Channel fade report
    ** -------------------------------------- */
    else if ((sigs & MCC_RX_RPT_Q_SIG) != 0)
    {
      if ((rpt_ptr = (mccrx_rpt_type*) q_get( &mcc_rx_rpt_q )) != NULL)
      {
        #ifdef FEATURE_MC_QUEUE_WATERMARK
        mc_update_watermark(MCWM_RX_FREE_Q);
        #endif

        if ( (rpt_ptr->hdr.rpt == RXC_PC_T40M_TMO_R) ||
             (rpt_ptr->hdr.rpt == RXC_PC_FS_TMO_R) )
        {
          MSG_HIGH( "Got fade report %d in SA 0x%x", rpt_ptr->hdr.rpt, curr_state, 0 );

          /* Failed access - perform registration processing */
          mccreg_idle_ack_or_fail();

          next_state = CDMA_EXIT;
          mcc_exit.exit = MCC_ACCESS_FAILED_SYS_LOST;

          /* Events report: Access attempt failure - loss of paging channel */
          event_report(EVENT_ACCESS_ATTEMPT_FAIL_LOSS_OF_PC_OR_FCCC);
        }
        else
        {
          MSG_MED( "Invalid rx rpt %d", rpt_ptr->hdr.rpt, 0, 0 );
        }

        cmd_done( &rpt_ptr->hdr.rpt_hdr );
      }
    } /* end else if ((sigs & MCC_RX_RPT_Q_SIG) != 0) */

    /* ------------------------------------------
    ** Check if auth signature has been received
    ** ------------------------------------------ */
    else if ((sigs & MCC_AUTH_Q_SIG) != 0)
    {
      if ((auth_rpt_ptr = (auth_rpt_type*) q_get( &mc_auth_q )) != NULL)
      {
        MSG_MED( "Got auth_sig report", 0, 0, 0 );

        switch ( auth_rpt_ptr->rpt_type )
        {
          case AUTH_SIGNATURE_R:
          {
            MSG_HIGH("Got AUTH_SIGNATURE_R", 0, 0, 0);
            auth_sig_rxed = TRUE;

            #ifdef FEATURE_UIM_RUIM
            /* Check if the RUIM successfully supplied the signature */
            if ((auth_rpt_ptr->rpt_status == RUIM_PASS)
                #if defined( FEATURE_UIM_RUN_TIME_ENABLE )
                /* Determine if the R-UIM is available for this NAM */
                || (!nv_rtre_use_ruim_for_nam(cdma.curr_nam))
                #endif /* FEATURE_UIM_RUN_TIME_ENABLE */
               )
            #endif /* FEATURE_UIM_RUIM */

            {
              /* --------------------------------------------
              ** Copy auth signature to appropriate location.
              ** -------------------------------------------- */
              *auth = auth_rpt_ptr->rpt.auth;
            }

            #ifdef FEATURE_UIM_RUIM
            else /* Failed getting the signature from the RUIM */
            {
              /* Set auth mode field to Do not include authentication signature */
              *auth_mode = CAI_NO_AUTH;

              ERR( "Failed getting signature from RUIM", 0, 0, 0 );

            } /* end if - the RUIM successfully supplied the signature */
            #endif /* FEATURE_UIM_RUIM */

            break;
          }

          #if defined(FEATURE_IS2000_REL_C_AKA) || defined(FEATURE_IS2000_REL_A_AES)
#error code not present
          #endif /* FEATURE_IS2000_REL_C_AKA || FEATURE_IS2000_REL_A_AES */

          default:
          {
            ERR( "Wrong report %d from AUTH!", auth_rpt_ptr->rpt_type, 0, 0 );

            break;
          }
        } /* switch */

        cmd_done( &auth_rpt_ptr->rpt_hdr );
      }
    } /* end else if (sigs & MCC_AUTH_Q_SIG) */
  } /* end while ((next_state == curr_state) && (!auth_sig_rxed)) */

  /* Reset the flag before exiting from this function */
  mccsa_gen_auth_signature_in_progress = FALSE;

  return (next_state);

} /* mccsa_wait_for_gen_auth_signature() */
#endif /* FEATURE_AUTH */

#ifdef FEATURE_IS2000_REL_A
/*===========================================================================

FUNCTION MCCSA_SEND_SMRM

DESCRIPTION
  This function sends Security Mode Request Message

DEPENDENCIES
  None

RETURN VALUE
  TRUE if the access attempt was aborted, else FALSE.

SIDE EFFECTS
  None

===========================================================================*/
word mccsa_send_smrm
(
  word curr_state
)
{

  word msg_len=0;
    /* Length of bit-packed message */
  access_result_type access_aborted;
    /* Flag if access attempt aborted */
  word next_state = curr_state;
    /* Next state to be processed */
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  if (curr_state != (next_state = mccsa_build_sec_mode_req_msg(curr_state)))
  {
    /* Abort access now before it even begins */
    return (next_state);
  }
  plt_rpt_ptr = &mcc_tx_int_msg.ac_tmsi_cmp.pilot_rpt;
  msg_len = mccsa_compute_independent_msg_len( &mcc_tx_int_msg, plt_rpt_ptr);

  access_aborted = mccsa_send_access_cmd( &mcc_tx_int_msg, plt_rpt_ptr, FALSE,
                                          msg_len, curr_state ); /*lint !e641 */

  if ( access_aborted == NOT_ABORT )
  {
    MSG_HIGH( "Sent Security Mode Request Message", 0, 0, 0 );

    /* Increment the counter for ACH/EACH messages generated by Layer 3. */
    mccsa_increment_l3_counter(SA_REQUEST);

    #if defined(FEATURE_IS2000_REL_C_AKA) || defined(FEATURE_IS2000_REL_A_AES)
#error code not present
    #endif /* FEATURE_IS2000_REL_C_AKA || FEATURE_IS2000_REL_A_AES */
  }
  else /* access was aborted */
  {
    /* Failed access - perform registration processing */
    mccreg_idle_ack_or_fail();

    /* Exit to System Determination */
    next_state    = CDMA_EXIT;

    if ( access_aborted == ABORT_SRCH_REQUEST_SYS_RESTART )
    {
      mcc_exit.exit = MCC_SRCH_REQUEST_SYS_RESTART;
    }
    else
    {
      mcc_exit.exit = MCC_ACCESS_DENIED;
    }
  }

  return (next_state);

} /* mccsa_send_smrm */

#endif /* FEATURE_IS2000_REL_A */


#if (defined(FEATURE_CDSMS) || defined(FEATURE_DBM))
/*===========================================================================

FUNCTION MCCSA_SEND_DATA_BURST

DESCRIPTION
  This function translates SMS or PD message and builds an
  Data Burst with for it.

DEPENDENCIES
  None

RETURN VALUE
  TRUE if the access attempt was aborted, else FALSE.

SIDE EFFECTS
  None

===========================================================================*/

word mccsa_send_data_burst
(
  word curr_state
    /* Current state being processed */
)
{
  word next_state = curr_state;
    /* Next state to be processed */

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  #ifdef FEATURE_DBM
  if (cdma.mo_dbm_type == CAI_POSITION_DET)
  {
    next_state = mccsa_build_dbm(CAI_POSITION_DET,
      (byte) ((cm_dbm_cmd_info_s_type *)cdma.dbm_buf_ptr)->num_bytes,
      ((cm_dbm_cmd_info_s_type *) cdma.dbm_buf_ptr)->payload, curr_state);
    return (next_state);
  }

  if (cdma.mo_dbm_type == CAI_SHORT_DATA_BURST)
  {
    if ( mcc_sdb_supported() ) 
    {
      if (((cm_dbm_cmd_info_s_type *) (cdma.dbm_buf_ptr))->trans_pref == 
          CM_DBM_TRANSMIT_ENH_ACCESS_OR_TRAFFIC)
      {
        #ifdef FEATURE_IS2000_REL_A_CC
        if (OVHD_CHAN_IS_BCCH)
        {
          next_state = mccsa_build_dbm(CAI_SHORT_DATA_BURST,
            (byte) ((cm_dbm_cmd_info_s_type *) cdma.dbm_buf_ptr)->num_bytes,
            ((cm_dbm_cmd_info_s_type *) (cdma.dbm_buf_ptr))->payload,
            curr_state);
          return (next_state);
        }
        #endif /* FEATURE_IS2000_REL_A_CC */
      }
      else
      {
        next_state = mccsa_build_dbm(CAI_SHORT_DATA_BURST,
          (byte) ((cm_dbm_cmd_info_s_type *) cdma.dbm_buf_ptr)->num_bytes,
          ((cm_dbm_cmd_info_s_type *) (cdma.dbm_buf_ptr))->payload,
          curr_state);
        return (next_state);
      }
    }

    /* Send back a reject to CM */
    mcc_send_mo_dbm_status(cdma.mo_dbm_type, CM_DBM_MO_OUT_OF_RESOURCES);

    next_state = CDMA_EXIT;
    mcc_exit.exit = MCC_ACCESS_FAILED;
    return (next_state);
  }
  #endif /* FEATURE_DBM */

  next_state = mccsa_build_dbm(CAI_SHORT_MSG_SERVICES,  
               (byte) ((cm_sms_msg_type*)cdma.dbm_buf_ptr)->ota.data_len,
               (byte*) ((cm_sms_msg_type*)cdma.dbm_buf_ptr)->ota.data,
               curr_state);

  return (next_state);

} /* end mccsa_send_data_burst */
#endif /* FEATURE_CDSMS || FEATURE_DBM */

/*===========================================================================

FUNCTION MCCSA_APHO_IN_PROGRESS

DESCRIPTION
  This function determines if the Access Probe Handoff is in progress

DEPENDENCIES

RETURN VALUE
  TRUE, if MS has performed APHO & after that is waiting for the Overhead
  information to get downloaded. FALSE otherwise.

SIDE EFFECTS
  None.
===========================================================================*/
boolean mccsa_apho_in_progress(void)
{
  return (mccsa_apho_substate == APHO_IN_PROGRESS);

} /* mccsa_apho_in_progress */

/*===========================================================================

FUNCTION MCCSA_IS_APHO_ALLOWED

DESCRIPTION
  This function determines if active pilot allows Access Probe Handoff.

DEPENDENCIES
  All the Overhead Parametres have been processed before calling this fn.
  mccsa_layer2.last_msg_type is properly set to the access message being
  set before calling this function.

RETURN VALUE
  TRUE, if APHO is allowed.

SIDE EFFECTS
  None.

===========================================================================*/
static boolean mccsa_is_apho_allowed()
{
  /* Check the Dynamic feature APHO */
   if (!mccap_item_is_supported (MCCAP_DF_APHO)) {
     MSG_MED("DF APHO not supported", 0, 0, 0);
     return FALSE;
   }

  /* Check the PREV in USE */
  /* This could have been changed in between because MS performed */
  /* Access Probe Handoff to an old Base Station.                 */
  if (!P_REV_IS_4_OR_GREATER)
  {
    MSG_MED("PREV < 4: APHO Not Allowed",0,0,0);
    return FALSE;
  }

  #ifdef FEATURE_IS2000_REL_D
#error code not present
  #endif /* FEATURE_IS2000_REL_D */

  /* PREV is OK for Access Probe Handoff */
  /* Check the Other conditions, like ESPM fields etc here */
  /* Note that these fields have been already checked when */
  /* Access attempt was started, but, after that because of*/
  /* an Access Probe Handoff, could have changed again.    */
  if (!cur_bs_ptr->csp.esp.access_probe_ho)
  {
    MSG_MED("ESPM->apho = FALSE",0,0,0);
    return FALSE;
  }

  /* Check access substate */
  if ((cdma.curr_state != CDMA_PAGE_RES) && (cdma.curr_state != CDMA_ORIG)) /*lint !e641 */
  {
    MSG_MED("Not in Page or Orig Resp substate", 0, 0, 0);
    return FALSE;
  }

  /* If we have already performed more than the maximum possible */
  /* Handoffs return FALSE.                                      */
  /* Note that MAX_NUM_PROBE_HO is set to 1 less than the no. of */
  /* maximum possinle Access Probe Handoffs.                     */
  /* Also note that this check MUST be performed only after the  */
  /* esp.access_probe_ho field check. Otherwise, max_num_probe_ho*/
  /* field has no meaning.                                       */
  if (mccsa_num_apho_done >= (cur_bs_ptr->csp.esp.max_num_probe_ho + 1))
  {
    MSG_MED("Max APHO Done", 0, 0, 0);
    return FALSE;
  }

  /* Check if the Access attempt is because of a message other */
  /* than Origination & Page Response & Reconnect.             */
  if ((mccsa_layer2.last_msg_type != CAI_ORIG_MSG) &&
      (mccsa_layer2.last_msg_type != CAI_PAGE_RESP_MSG)
      #ifdef FEATURE_IS2000_REL_C
      && (mccsa_layer2.last_msg_type != CAI_AC_RECONNECT_MSG)
      #endif /* FEATURE_IS2000_REL_C*/
     )
  {
    /* The check for this is not done using the variable cdma.curr_state    */
    /* Even if the Current state is CDMA_ORIG/PAGE_RES, MS could be sending */
    /* an Access Channel Message other than Orig/Page Response. For example */
    /* Consider the case when MS sends an Origination and the L2 Ack for it */
    /* comes back with an "Order Message" for which MS needs to send back a */
    /* Response. MS will be sending Order Response, but the state will be   */
    /* CDMA_ORIG. Check for this type of scenario as well.                  */
    if (!cur_bs_ptr->csp.esp.acc_probe_ho_other_msg)
    {
      MSG_MED("ESPM->apho_other_msg = FALSE",0,0,0);
      return FALSE;
    }
  }

  return TRUE;

} /* mccsa_is_apho_allowed */

/*===========================================================================

FUNCTION MCCSA_IS_APHO_OKAY

DESCRIPTION
  This function determines if the Access Probe Handoff is allowed to
  a particular Pilot or not.

DEPENDENCIES
  All the Overhead Parametres have been processed before calling this fn.

RETURN VALUE
  TRUE, if APHO is allowed.

SIDE EFFECTS
  None.

===========================================================================*/
static boolean mccsa_is_apho_okay
(
  word new_pilot,
  word new_freq,
  byte new_band_class
)
{
  int2 i;

  if (!mccsa_is_apho_allowed())
  {
    MSG_MED("APHO not allowed on current BS", 0, 0, 0);
    return FALSE;
  }

  /* ------------------------------------------------
  ** Already on the new pilot, no need to do handoff.
  ** ------------------------------------------------ */
  if ((new_pilot == cur_bs_ptr->pilot_pn) && (new_freq == cdma.cdmach) &&
      (new_band_class == cdma.band_class))
  {
    MSG_MED("Already on new pilot, no APHO", 0, 0, 0);
    return FALSE;
  }
  else
  /* -------------------------------------------------------
  ** Check if access probe handoff to this pilot is allowed.
  ** ------------------------------------------------------- */
  {
    i = mccidl_get_nghbr_index(new_pilot, new_freq, new_band_class);
    if (i == -1)
    {
      MSG_MED("Unknown pilot, no APHO", 0, 0, 0);
      return FALSE;
    }
    else if (((new_freq != cdma.cdmach) &&
              (new_freq != cur_bs_ptr->nl.nset[i].freq)) ||
             ((new_band_class != cdma.band_class) &&
              (new_band_class != cur_bs_ptr->nl.nset[i].band)))
    {
      MSG_HIGH("Ignored old HO rpt freq %d band %", new_freq,
        new_band_class, 0);
      return FALSE;
    }
    else if (!cur_bs_ptr->nl.nset[i].access_ho_allowed)
    {
      MSG_MED("APHO not allowed to new pilot", 0, 0, 0);
      return FALSE;
    }
  }

  /* Check if the Pilot is in ACC_HO_LIST */
  /* If not: then Check if it meets all the conditions to be */
  /* a candidate pilot for Access Probe Handoff. */

  /* Try to find this Pilot in the Neighbor List, and see, if the AccessHO*/
  /* is allowed on this Pilot or not.                                     */

  if (!mccsa_is_in_access_ho_list(new_pilot))
  {
    /* Checkout if the conditions for performing an APHO to a pilot */
    /* not in Access HO List are satisfied.                         */
    /* Refer to IS-95B Section 6.6.3.1.3.3 for these conditions.    */
    if ((mccsa_layer2.last_msg_type != CAI_ORIG_MSG) &&
        (mccsa_layer2.last_msg_type != CAI_PAGE_RESP_MSG)
        #ifdef FEATURE_IS2000_REL_C
        && (mccsa_layer2.last_msg_type != CAI_AC_RECONNECT_MSG)
        #endif /* FEATURE_IS2000_REL_C */
       )
    {
      MSG_MED("Pilot %d  not in AHO List",new_pilot,0,0);
      return FALSE; /* In all other access states APHO not allowed to a pilot
                       not in Access Handoff List.
                     */
    }

    /* ACC_HO_LIST_UPD field must be equal to '1' */
    if (!cur_bs_ptr->csp.esp.acc_ho_list_upd)
    {
      MSG_MED("Pilot %d  not in AHO List",new_pilot,0,0);
      MSG_MED("ESPM->AHO LIST Update = FALSE",0,0,0);
      return FALSE;
    }

    /* There is space in the Access HO List for the inclusion of this pilot */
    /* This condition will not be checked at this stage. The reasoning for  */
    /* that being - after the Access Handoff the Access Parameters and hence*/
    /* the max no. of permissible pilots in ACC_HO_LIST will change, and MS */
    /* needs to  make a check at that stage again. Checking with the current*/
    /* BS Parameters therefore is not required.                             */
  }

  /* All the conditions passed for Access Probe Handoffs */
  /* We are going to initiate this one.                  */
  mccsa_num_apho_done++;
  return TRUE;

} /* mccsa_is_apho_okay */

/*===========================================================================

FUNCTION MCCSA_APHO_DONE

DESCRIPTION
  This function performs the Access Probe Handoff PostProcessing.

DEPENDENCIES
  All the Overhead Parametres from the new Base stattion have been
  downloaded before calling this fn.

RETURN VALUE
  Next State. if Access Attempt have to be resumed on the new base station,
  the value returned remains same as the curr_state(argument to this fn.)
  If Access Attempt can not be resumed, CDMA_EXIT is returned. Note that this
  could happen because of several reasons, mainly because the new bs can not
  support the Access HO List this big. For detailed description, please refer
  to Access Probe Handoff SDD.

SIDE EFFECTS
  If Access Probing can be resumed on the new Base Station, this function sends
  a Pilot Report to TXC.
  If Access attempts can not be resumed on the new Base Station, this function
  sends a STOP Command to TXC.
===========================================================================*/

static word mccsa_apho_done(word curr_state)
{
  word next_state;
  caii_accparm_type * ap_ptr = NULL; /* Ptr to Access Params after APHO */

  #ifdef FEATURE_IS2000_REL_A_CC
  caii_en_ac_parm_type *eap_ptr = NULL;    /* Pointer to current Enhanced Access Params Msg */
  txc_each_rate_type rate_word;   /* Rate index selected */
  word total_msg_length;
  byte access_mode;           /* Access mode used for the EACH */
  word             msg_len;     /* Length of message */
  int mode;                       /* Index of mode selected  */
  #endif /* FEATURE_IS2000_REL_A_CC */

  word max_message_body = 0;  /* Maxm. Access Channel Message size after APHO */
  access_result_type access_aborted;       /* Flag if access attempt aborted */


  next_state = curr_state;

  /* Done with this APHO */
  mccsa_apho_substate = APHO_NOT_IN_PROGRESS;

  /* Check the newly downloaded Overhead Parametres to find out if the Access */
  /* attempt can still be resumed.                                            */
  if (!(P_REV_IS_4_OR_GREATER))
  {
    /* Handoff to a Non-IS95 Base station is currently not supported. */

    /* Failed access - perform registration processing */
    mccreg_idle_ack_or_fail();

    mcc_exit.exit =   MCC_SYSTEM_LOST;
    return CDMA_EXIT;
  }

  /* We need not to do any of the following processing if the layer 2 Ack has */
  /* already been recieved. It is possible that it could have arrived already.*/
  if (!mccsa_layer2.access_in_progress)
  {
    return next_state;
  }

  /* We need not Check if the GSRM is enabled, since it should have been    */
  /* checked by the function mccsa_proc_msg().                             */

  /* Make an attempt to build the new access Ho List */
  /* Compute the number of pilots that can be contained in Access Channel
     Message without exceeding the maximum capsule size. Note that this is
     being done because this could have been changed after APHO.
  */

  #ifdef FEATURE_IS2000_REL_A_CC
  if (OVHD_CHAN_IS_BCCH)
  {
    if ((eap_ptr = (caii_en_ac_parm_type *) mcc_get_msg( CAI_EN_AC_PARM_MSG )) != NULL)
    {
      max_message_body = mccsa_calculate_maximum_reach_message();
    }
    else
    {
      ERR_FATAL("APHO Done before updating Access Params??",0,0,0);
    }
  }
  else
  #endif // FEATURE_IS2000_REL_A_CC

  {
    if ((ap_ptr = (caii_accparm_type *) mcc_get_msg( CAI_ACC_PARM_MSG )) != NULL)
    {
      /* ----------------------------------------------------------
      ** MAX_CAP_SZ is three less than the maximum number of frames
      ** the access channel message capsule can span.  Each frame
      ** can hold 88 data bits.  So the maximum message body length
      ** is, in bits, (MAX_CAP_SZ + 3) * 88 - (size of MSG_LENGTH
      ** field) - (size of CRC field).  See IS-95A section 6.7.1.2.
      ** ---------------------------------------------------------- */
      max_message_body = (ap_ptr->max_cap_sz + 3) * 88 -
                         CAI_AC_CRC_SIZE - CAI_AC_LGTH_SIZE; /*lint !e834 */
    }
    else
    {
      ERR_FATAL("APHO Done before updating Access Params??",0,0,0);
    }
  }

  mccsa_pilot_rpt_ovhd_rec.num_pilots_allowed =
    mccsa_compute_ac_msg_num_plt_rpt(max_message_body);

  /* Search the AHO List to see, if an access attempt has been made on this pilot */
  /* Looks like the following variable is not used anywhere , so don't bother to
   * update it, leave the existing behavior for MSM3100 */

  /* Set the field rebuild to TRUE, so that the AHO is rebuilt */
  mccsa_pilot_rpt_ovhd_rec.apho_rebuild = TRUE;

  /* Send a command to Searcher requesting for the latest Pilot Report   */
  /* Note that the Neighbor List information must have been sent to SRCH */
  access_aborted =  mccsa_wait_for_srch_pilots_meas_rpt();

  if ( access_aborted != NOT_ABORT )
  {
    /* Some problem in rebuilding the Access HO List. */
    /* Abort the access attempt.                      */

    /* Failed access - perform registration processing */
    mccreg_idle_ack_or_fail();

    if ( access_aborted == ABORT_SRCH_REQUEST_SYS_RESTART )
    {
      mcc_exit.exit = MCC_SRCH_REQUEST_SYS_RESTART;
    }
    else
    {
      mcc_exit.exit = MCC_SYSTEM_LOST;
    }

    return CDMA_EXIT;
  }

  /* Change the parameters in the mcc_tx_buf */
  #ifdef FEATURE_IS2000_REL_A_CC
  if (OVHD_CHAN_IS_BCCH)
  {
    // Calculate the total message length by adding the CRC length and
    // the message length field.  If the LAC PDU size is greater than
    // 978, then the message length field is two bytes.  See LAC 2.1.1.5.1.2.
    msg_len = mccsa_pilot_rpt_ovhd_rec.msg_len;

    if (msg_len <= 978)
    {
      total_msg_length = msg_len + CAI_AC_CRC_SIZE + CAI_AC_LGTH_SIZE;
    }
    else
    {
      total_msg_length = msg_len + CAI_AC_CRC_SIZE + 2 * CAI_AC_LGTH_SIZE;
    }

    if ( (access_aborted = mccsa_select_mode_rate(total_msg_length,
          eap_ptr->each_ba_rates_supported, eap_ptr->rccch_rates_supported,
          eap_ptr->num_mode_selection_entries, eap_ptr->mode_selection,
          eap_ptr->rlgain_common_pilot, &mode, &rate_word )) == NOT_ABORT ) //lint !e613 Possible use of NULL pointer
    {
      access_mode = eap_ptr->mode_selection[mode].access_mode;
      access_aborted = mccsa_populate_reach_access_cmd(eap_ptr, curr_state,
        access_mode, rate_word);
    }
  }
  else
  #endif // FEATURE_IS2000_REL_A_CC

  {
    access_aborted = mccsa_populate_ach_access_cmd(ap_ptr, curr_state);
  }

  if (access_aborted == ABORT_PSIST)
  {
    next_state = CDMA_EXIT;
  }
  else
  {
    /* Append pilot report information to Access Channel Message */
    next_state = mccsa_send_acc_probe_cmd( curr_state,
      &mcc_tx_int_msg, plt_rpt_ptr );
  }

  if (next_state == CDMA_EXIT)
  {
    /* Failed access - perform registration processing */
    mccreg_idle_ack_or_fail();

    mcc_exit.exit = MCC_SYSTEM_LOST;
  }

  return next_state;

} /* mccsa_apho_done */

/*===========================================================================

FUNCTION MCCSA_IS_AHO_ALLOWED

DESCRIPTION
  This function determines if active pilot allows Access Handoff.

DEPENDENCIES
  All the Overhead Parametres have been processed before calling this fn.

RETURN VALUE
  TRUE, if AHO is allowed.

SIDE EFFECTS
  None.

===========================================================================*/
static boolean mccsa_is_aho_allowed()
{
  /* Check the PREV in USE */
  if (!P_REV_IS_4_OR_GREATER)
  {
    MSG_MED("PREV < 4: AHO Not Allowed",0,0,0);
    return FALSE;
  }

  #ifdef FEATURE_IS2000_REL_D
#error code not present
  #endif /* FEATURE_IS2000_REL_D */

  if (!cur_bs_ptr->csp.esp.access_ho ||
      !cur_bs_ptr->csp.esp.nghbr_set_access_info)
  {
    MSG_MED("BS does not support AHO", 0, 0, 0);
    return FALSE;
  }

  return TRUE;

} /* mccsa_is_aho_allowed */

/*===========================================================================

FUNCTION MCCSA_WAIT_FOR_ACCESS_RESPONSE

DESCRIPTION
  This function monitors the Paging Channel and command queues while in
  System Access State.

DEPENDENCIES
  None.

RETURN VALUE
  The next state to be processed.

SIDE EFFECTS
  None.

===========================================================================*/

word mccsa_wait_for_access_response
(
  word curr_state   /* Current substate */
)
{
  static mccrxtx_cmd_type *rx_ptr;
    /* Pointer to received message */
  mccrx_rpt_type *rpt_ptr;
    /* Pointer to Paging Channel fade report */
  word next_state;
    /* Next state to be processed */
  dword sigs;
    /* Return signals from mcc_wait() call */
  mc_msg_type *mc_cmd_ptr;
    /* Pointer to UI command received on mc_cmd_q */

  dword sa_wait_mask;
    /* This mask is used to set various signals on which Mobile waits */

  mccsrch_rpt_type *srch_rpt_ptr;
    /* Report from SRCH in case of a strong pilot */

  boolean listupd_succeed;
    /* Flag if building or updating ACCESS_HO_LIST is succedded */

  mccsrch_rpt_type mcc_srch_rpt; /* Current Searcher report */

  boolean resume_txc;
    /* Indicates if transmission is resumed after temporary suspension */

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  next_state = curr_state;

  /* Initialized mcc_srch_rpt */
  memset((void *)(&mcc_srch_rpt), 0xFF, sizeof(mccsrch_rpt_type));
  mcc_srch_rpt.hdr.rpt = SRCH_MAX_R;

  /* Clear System Access State Timer signal and timer - may use these later */
  (void) rex_clr_timer( &mcc_state_timer );
  (void) rex_clr_sigs( &mc_tcb,  MCC_STATE_TIMER_SIG );

  /* Initialize layer 2 pending message flag */
  mccsa_layer2.pending_msg_flag = FALSE;

  /* Initialize layer 3 pending message flag */
  mccsa_layer3_msg_pending = FALSE;

  /* Do while not end of this sequence or time to exit state */
  while (next_state == curr_state)
  {
    sa_wait_mask = MCC_RXTX_Q_SIG   | MCC_STATE_TIMER_SIG |
                   MCC_RX_RPT_Q_SIG | MCC_TXC_DONE_SIG    |
                   MCC_CMD_Q_SIG    | MCC_SRCH_Q_SIG; 

    /* ---------------------------------------------------------------
    ** IS95B Mobile with P_REV_IN_USE 4 or more, waits for SRCH_Q_SIG
    ** and MCC_TXC_PROBE_SIG for Pilot reporting and Access Handoffs
    ** --------------------------------------------------------------*/
    if (P_REV_IS_4_OR_GREATER)
    {
      sa_wait_mask |= MCC_TXC_PROBE_SIG;
    }

    if (mccsa_apho_substate == WAIT_FOR_TXC_OK)
    {
      sa_wait_mask |= MCC_TXC_APHO_OK_SIG;
    }

    #ifdef FEATURE_SO_8026
#error code not present
    #endif /* FEATURE_SO_8026 */

    /* Wait on the above set signal mask for further processing */
    sigs = mcc_wait(sa_wait_mask);

    /* ----------------------------------------------
    ** NOTE:  We ignore Searcher during System Access
    ** substates and just leave the commands on queue
    ** ---------------------------------------------- */

    /* ---------------------------------------------------------
    ** Check to see if we have received a Paging Channel message
    ** --------------------------------------------------------- */
    if ((sigs & MCC_RXTX_Q_SIG) != 0)
    {
      if ((rx_ptr = (mccrxtx_cmd_type*) q_get( &mcc_rxtx_q )) != NULL)
      {
        /* ---------------------------------------------------
        ** Process Paging Channel message and check if we need
        ** to stop accesses
        ** --------------------------------------------------- */
        if (rx_ptr->hdr.command == MCCRXTX_MSG_F)
        {
          next_state = mccsa_proc_msg( &rx_ptr->msg, curr_state );

          /* return the buffer to the free queue */
          cmd_done( &rx_ptr->hdr.cmd_hdr );

          /* A layer 2 ACK might have put the MS into AHO region */
          /* while an Access Probe Handoff was already due. In   */
          /* this case, perform an Access Handoff.               */
          /* Even though the flag mccsa_init_aho is set only when*/
          /* the ho_state is CDMA_AHO. However, if a CAM was rcvd*/
          /* directly, the MS would be out of AHO region and in  */
          /* TC state. And, in this case, MS shouldn't do AHO    */
          if ((next_state == curr_state) && mccsa_init_aho)
          {
            cdma_ho_type event_ho_type;
            /* If the cdma.ho_state is not CDMA_AHO, it means that MS came */
            /* out of APHO region because of a message which had a L2 Ack  */
            /* for the Access attempt. However, this ACK itself requires   */
            /* MS to send back an acknowledgement on the Access Channel.   */
            /* In this case, if the Searcher has already reported a        */
            /* stronger pilot, MS shall still perform a Handoff before     */
            /* sending a reponse on Access Channel.                        */
            /* This type of handoff will be treated as an APHO, despite of */
            /* the fact that MS has already got a L2 ack for the actual    */
            /* access attempt.                                             */

            if (cdma.ho_state != CDMA_AHO)
            {
              mccsa_apho_substate = APHO_IN_PROGRESS;
              event_ho_type = CDMA_APHO;
              MSG_MED("Initiate APHO",0,0,0);
            }
            else
            {
              mccsa_apho_substate = APHO_NOT_IN_PROGRESS;
              event_ho_type = CDMA_AHO;
              MSG_MED("Initiate AHO",0,0,0);
            }

            if (mcc_srch_rpt.hdr.rpt == SRCH_MAX_R) 
            {
              ERR_FATAL("Unintialized mcc_srch_rpt", 0, 0, 0);
            }

            next_state = mccidl_handoff((word) mcc_srch_rpt.pc_meas.pilot,
                                        (word) mcc_srch_rpt.pc_meas.freq,
                                        (byte) mcc_srch_rpt.pc_meas.band_class,
                                        next_state,
                                        (int8) mcc_srch_rpt.pc_meas.ecio_old_pilot,
                                        (int8) mcc_srch_rpt.pc_meas.ecio_new_pilot,
                                        event_ho_type); //mcc_srch_rpt is initialized by memcpy
          }

          mccsa_init_aho = FALSE;
        }

        #if defined(FEATURE_IS2000_REL_C_AKA) || defined(FEATURE_IS2000_REL_A_AES)
#error code not present
        #endif /* FEATURE_IS2000_REL_C_AKA || FEATURE_IS2000_REL_A_AES */
    
        else
        {
          ERR( "Invalid RXTX command %d", rx_ptr->hdr.command, 0, 0 );
          cmd_done( &rx_ptr->hdr.cmd_hdr );
        }
      }
    } /* end if ((sigs & MCC_RXTX_Q_SIG) != 0) */

    /* ----------------------------------------------------------
    ** Check to see if we have received a Pilot report from SRCH.
    ** ---------------------------------------------------------- */
    if ((sigs & MCC_SRCH_Q_SIG) != 0)
    {
      if ((srch_rpt_ptr = (mccsrch_rpt_type*) q_get(&mcc_srch_q)) != NULL)
      {
        if (srch_rpt_ptr->hdr.rpt == SRCH_PC_MEAS_R)
        {
          /* --------------------------------------------------------------
          ** Accept the SRCH_PC_MEAS_R report from SRCH only if the Mobile
          ** allowed for Access Handoff.
          ** -------------------------------------------------------------*/
          if (cdma.ho_state == CDMA_AHO)
          {
            /*--------------------------------------------------------------
            ** If mobile has received a Message and is in the process of
            ** sending a response to the Base station, check the
            ** ACCESS_HO_MSG_RSP and if enabled, perform the Access Handoff.
            **-------------------------------------------------------------*/
            if ((!mccsa_layer3_msg_pending) && (!mccsa_layer2.pending_msg_flag))
            {
              next_state = mccidl_handoff(
                             (word) srch_rpt_ptr->pc_meas.pilot,
                             (word) srch_rpt_ptr->pc_meas.freq,
                             (byte) srch_rpt_ptr->pc_meas.band_class,
                             next_state,
                             (int8) srch_rpt_ptr->pc_meas.ecio_old_pilot,
                             (int8) srch_rpt_ptr->pc_meas.ecio_new_pilot,
                              CDMA_AHO);
              MSG_MED("Access Handoff 0x%x", next_state, 0, 0);
              cmd_done(&srch_rpt_ptr->hdr.rpt_hdr);
            }
            else if (cur_bs_ptr->csp.esp.access_ho_msg_rsp)
            {
              /* ------------------------------------------------------
              ** Process the Search report in the System Access state.
              ** This report is used to perform Access Handoff.
              ** ------------------------------------------------------*/
              next_state = mccidl_handoff(
                             (word) srch_rpt_ptr->pc_meas.pilot,
                             (word) srch_rpt_ptr->pc_meas.freq,
                             (byte) srch_rpt_ptr->pc_meas.band_class,
                             next_state,
                             (int8) srch_rpt_ptr->pc_meas.ecio_old_pilot,
                             (int8) srch_rpt_ptr->pc_meas.ecio_new_pilot,
                             CDMA_AHO);
              MSG_MED("Access Handoff 0x%x", next_state, 0, 0);
              cmd_done(&srch_rpt_ptr->hdr.rpt_hdr);
            }
            else
            {
              MSG_MED("Access Handoff not allowed 0x%x", next_state, 0, 0);
              cmd_done(&srch_rpt_ptr->hdr.rpt_hdr);
            }
          }
          else

          /* Note the usage of cdma.ho_state. This variable is set to APHO */
          /* at the start of the Access Attempts, if APHO is at all allowed*/
          /* for that Access Attempt. Then, the fn mccsa_is_apho_allowed() */
          /* keeps on monitoring the Access Probed Handoffs in between the */
          /* Access Probes, and when the Max APHOs are done, this variable */
          /* is reset again. The order of the && operator, in the following*/
          /* if condition makes sure that this function is not called again*/
          /* during the same Access Attempt.                               */
          if ((cdma.ho_state == CDMA_APHO) &&
              (mccsa_is_apho_okay((word) srch_rpt_ptr->pc_meas.pilot,
               (word) srch_rpt_ptr->pc_meas.freq,
               srch_rpt_ptr->pc_meas.band_class)))
          {
            MSG_MED("Access Probe Handoff pending",0 ,0, 0);
            /* Store this PC Measurement report, so that it can be used later */
            memcpy((void *)(&mcc_srch_rpt),srch_rpt_ptr,sizeof(mccsrch_rpt_type));

            /* Set mcc_srch_rpt's done_q_ptr to NULL so that we do not end */
            /* up putting this local variable on the srch_free_q.          */
            mcc_srch_rpt.hdr.rpt_hdr.done_q_ptr = NULL;
            mcc_srch_rpt.hdr.rpt_hdr.task_ptr = NULL;

            /* And, return the srch_rpt_ptr buffer to the free queue.    */
            /* we already have copied all the relevent data to our local */
            /* buffer and we need not store this buffer anymore.         */
            cmd_done (&srch_rpt_ptr->hdr.rpt_hdr);

            if (mccsa_apho_substate != PC_LOSS_WAIT_FOR_SRCH &&
                mccsa_apho_substate != PC_LOSS_APHO_IN_PROGRESS &&
                mccsa_apho_substate != APHO_IN_PROGRESS)
            {
              /* Command TXC to suspend the transmission of Access Probes */
              mcc_tx_buf.hdr.command = TXC_ACC_SUSPEND_F;
              mcc_tx_buf.hdr.cmd_hdr.done_q_ptr = NULL;
              mcc_tx_buf.hdr.cmd_hdr.task_ptr = NULL;

              mcc_txc_cmd(&mcc_tx_buf);

              /* Wait till TXC responds back */
              mccsa_apho_substate = WAIT_FOR_TXC_OK;
            }
            else
            {
              if (mccsa_apho_substate == PC_LOSS_WAIT_FOR_SRCH ||
                  mccsa_apho_substate == PC_LOSS_APHO_IN_PROGRESS
                 )
              {
                MSG_MED("Initiate APHO (PC Loss)# %d",mccsa_num_apho_done,0,0);
              }
              else
              {
                MSG_MED("Initiate APHO (Insufficiency)# %d",mccsa_num_apho_done,0,0);
              }

              /* TXC is already in the suspended state. Initiate APHO */
              mccsa_apho_substate = APHO_IN_PROGRESS;
              next_state = mccidl_handoff((word) mcc_srch_rpt.pc_meas.pilot,
                                         (word) mcc_srch_rpt.pc_meas.freq,
                                         (byte) mcc_srch_rpt.pc_meas.band_class,
                                         next_state,
                                         (int8) mcc_srch_rpt.pc_meas.ecio_old_pilot,
                                         (int8) mcc_srch_rpt.pc_meas.ecio_new_pilot,
                                         CDMA_APHO);
            }
          }
          else
          {
            /* -----------------------------------------------------------
            ** Clear the unexpected Search report during System Access
            ** and continue wait for desired signals.
            ** -----------------------------------------------------------*/
            MSG_MED("AHO/APHO not allowed in the state", 0, 0, 0);
            cmd_done(&srch_rpt_ptr->hdr.rpt_hdr);
          }
        } /* end if (srch_rpt_ptr->hdr.rpt == SRCH_PC_MEAS_R) */

        else if (srch_rpt_ptr->hdr.rpt == SRCH_AHO_PLIST_R)
        {
          /* --------------------------------------------------
          ** Accept the pilot strength measurement only if
          ** (1) Layer2 Ack has not been received and,
          ** (2) Report is for the current base station.
          ** -------------------------------------------------- */
          if (P_REV_IS_4_OR_GREATER && mccsa_layer2.access_in_progress &&
              (srch_rpt_ptr->pilots_meas.pilot_list[0].pilot_pn ==
               cur_bs_ptr->pilot_pn))
          {
            /* Process the Pilot Strength Measurement report */
            listupd_succeed = mccsa_process_plt_str_meas_rpt(srch_rpt_ptr);
            cmd_done(&srch_rpt_ptr->hdr.rpt_hdr);

            /* Succeed in building or updating ACCESS_HO_LIST */
            if (listupd_succeed)
            {
              /* Not in the middle of Access Probe Handoff */
              if (mccsa_apho_substate == APHO_NOT_IN_PROGRESS)
              {
                /* Send Access Channel Msg with pilot reporting info  */
                next_state = mccsa_send_acc_probe_cmd(curr_state,
                  &mcc_tx_int_msg, plt_rpt_ptr );
                if (next_state == CDMA_EXIT)
                {
                  /* If the Access Probe Command failed, send a stop to TXC */
                  mccsa_tx_stop();
                }
              }
            }
            /* Failed to build ACCESS_HO_LIST */
            else
            {
              /* Command TXC to stop accesses */
              mccsa_tx_stop();

              /* Perform registration actions for a failed access */
              mccreg_idle_ack_or_fail();

              /* Exit to System Determination state */
              next_state    = CDMA_EXIT;
              mcc_exit.exit = MCC_ACCESS_DENIED;
            }
          }
          else
          {
            MSG_MED("Ignore SRCH_AHO_LIST_R: apho %d meas %d aset %d",
              mccsa_apho_substate,
              srch_rpt_ptr->pilots_meas.pilot_list[0].pilot_pn,
              cur_bs_ptr->pilot_pn);
            cmd_done(&srch_rpt_ptr->hdr.rpt_hdr);
          }
        } /* end if (srch_rpt_ptr->hdr.rpt == SRCH_AHO_PLIST_R) */

        else if (srch_rpt_ptr->hdr.rpt == SRCH_SYS_MEAS_R)
        {
          /* Report system measurements to System Determination */
          mcc_sys_meas_rpt(srch_rpt_ptr);
          /* -------------------------------------------------------------
          ** Inform system determination of system selection timer expires
          ** and reselection is not allowed in the current state.
          ** ------------------------------------------------------------- */
          (void) sd_ss_ind_misc_meas_rprt (srch_rpt_ptr->sys_meas.meas_id,
            FALSE, NULL);
          cmd_done(&srch_rpt_ptr->hdr.rpt_hdr);
        } /* end if (srch_rpt_ptr->hdr.rpt == SRCH_SYS_MEAS_R) */

        else if (srch_rpt_ptr->hdr.rpt == SRCH_WAKEUP_R)
        {
          MSG_MED("Ignoring SRCH_WAKEUP_R",0,0,0);
          cmd_done ( &srch_rpt_ptr->hdr.rpt_hdr );
        }

        else if (srch_rpt_ptr->hdr.rpt == SRCH_REQUEST_SYS_RESTART_R)
        {
          MSG_HIGH( "SRCH request sys restart", 0, 0, 0 );

          /* Command TXC to stop accesses */
          mccsa_tx_stop();

          /* Perform registration actions for a failed access */
          mccreg_idle_ack_or_fail();

          /* Exit to System Determination state */
          next_state    = CDMA_EXIT;
          mcc_exit.exit = MCC_SRCH_REQUEST_SYS_RESTART;

          cmd_done(&srch_rpt_ptr->hdr.rpt_hdr);
        }

        /* Unexpected report from Searcher */
        else
        {
          /* -----------------------------------------------------------
          ** Clear the unexpected Search report during System Access
          ** and continue wait for desired signals.
          ** -----------------------------------------------------------*/
          MSG_ERROR("Unexpected type of Searcher report %d", srch_rpt_ptr->hdr.rpt, 0, 0);
          cmd_done(&srch_rpt_ptr->hdr.rpt_hdr);
        }

      } /* end if ((srch_rpt_ptr = q_get(&mcc_srch_q))!= NULL)) */
    } /* end if ((sigs & MCC_SRCH_Q_SIG) != 0) */

    /* ---------------------------------------------
    ** Check to see if we have received a UI command
    ** --------------------------------------------- */
    else if ((sigs & MCC_CMD_Q_SIG) != 0)
    {
      if ((mc_cmd_ptr = (mc_msg_type*) q_get( &mc_cmd_q )) != NULL)
      {
        next_state = mccsa_hs_proc( mc_cmd_ptr, next_state );
      }
    } /* end else if ((sigs & MCC_CMD_Q_SIG) != 0) */

    /* --------------------------------------
    ** Check for a Paging Channel fade report
    ** -------------------------------------- */
    else if ((sigs & MCC_RX_RPT_Q_SIG) != 0)
    {
      if ((rpt_ptr = (mccrx_rpt_type*) q_get( &mcc_rx_rpt_q )) != NULL)
      {
        #ifdef FEATURE_MC_QUEUE_WATERMARK
        mc_update_watermark(MCWM_RX_FREE_Q);
        #endif

        if ( (rpt_ptr->hdr.rpt == RXC_PC_T40M_TMO_R) ||
             (rpt_ptr->hdr.rpt == RXC_PC_FS_TMO_R) )
        {
          MSG_HIGH( "Got fade report %d in SA 0x%x", rpt_ptr->hdr.rpt, curr_state, 0 );

          /* if release is unsuccessful due to fading, we want to exit to Sys
           * Det or powerdown instead of going to Idle */
          if ((curr_state == CDMA_ORIG) && /*lint !e641 */
              (mccsa_orig_ministate == CDMA_ORIG_RELS))
          {
            if (mccsa_release_cause == CAI_REL_PWDN)
            {
              mcc_exit.exit = MCC_POWERDOWN;
              MSG_HIGH( "Powerdown w/o rx ACK due to fading", 0, 0, 0 );
            }
            else
            {
              mcc_exit.exit = MCC_RELEASE;
              MSG_HIGH( "Exit to SysDet w/o rx ACK due to fading", 0, 0, 0 );
            }
            next_state = CDMA_EXIT;

            /* Command TXC to stop access */
            mccsa_tx_stop();

            /* Failed access - perform registration processing */
            mccreg_idle_ack_or_fail();

            cmd_done (&rpt_ptr->hdr.rpt_hdr);
            break;
          }

          #if (defined(FEATURE_CDSMS) || defined (FEATURE_DBM))
          /* Before we toggle HOLD_ORIG, notify SMS */
          if ((curr_state == CDMA_MSG) /*lint !e641 */
              && ( cdma.sa_msg_tx_msg == CAI_AC_BURST_MSG )  /*lint !e641 */
             ) /*lint !e641 */
          {
            MSG_HIGH( "SMS request dropped in CDMA_IDLE", 0, 0, 0 );
            mcc_check_mo_dbm_status( CM_DBM_MO_L2_ACK_FAILURE );
          }
          #endif /* FEATURE_CDSMS || FEATURE_DBM */

          next_state = CDMA_EXIT;
          mcc_exit.exit = MCC_ACCESS_FAILED_SYS_LOST;

          /* Command TXC to stop accesses */
          mccsa_tx_stop();

          /* Perform registration actions for a failed access */
          mccreg_idle_ack_or_fail();


          if ((curr_state == CDMA_ORIG) || (curr_state == CDMA_PAGE_RES)) /*lint !e641 */
          {
            #ifdef FEATURE_IS2000_REL_A_CC
            if (OVHD_CHAN_IS_BCCH)
            {
              /* Set MC_RR_PAR_MSG_SEQs & ACC_MSG_SEQs to NULL */
              cur_bs_ptr->mcrrpm_config_msg_seq =
                cur_bs_ptr->eapm.acc_msg_seq = INVALID_SEQ;
            }
            else
            #endif /* FEATURE_IS2000_REL_A_CC */
            {
              /* Set SYS_PAR_MSG_SEQs & ACC_MSG_SEQs to NULL */
              cur_bs_ptr->csp.sp.config_msg_seq =
                cur_bs_ptr->ap.acc_msg_seq = INVALID_SEQ;
            }
          }

          /* Events report: Access attempt failure - loss of paging channel */
          event_report(EVENT_ACCESS_ATTEMPT_FAIL_LOSS_OF_PC_OR_FCCC);
        }
        else if (rpt_ptr->hdr.rpt == RXC_PC_T72M_TMO_R)
        {
          MSG_HIGH( "Got T72M Fade report in SA 0x%x", curr_state, 0,0 );

          /* -------------------------------------------------------------
          ** Mobile received a T72M fade timer.  If the mobile is allowed
          ** for Access Handoff or Access Probe Handoff, APHO is performed
          ** while the Access Attempt is in progress. After a successful
          ** APHO, the mobile will enable the Transmitter and resumes.
          ** If AHO/APHO are not allowed, the mobile waits for either T40M
          ** timer to expire or for a Valid PC message from RX task.
          ** ------------------------------------------------------------*/
          if (cdma.ho_state == CDMA_APHO)
          {
            /* If the control is here, it means that the MS is in APHO region */
            /* and APHO is allowed.                                           */
            /* Just set APHO substate, so that if Search Reports a stronger   */
            /* pilot, the Access Probe Handoff is performed without waiting   */
            /* for TXC.                                                       */
            if (mccsa_apho_substate == APHO_NOT_IN_PROGRESS)
            {
              mccsa_apho_substate = PC_LOSS_WAIT_FOR_SRCH;
            }
            else if (mccsa_apho_substate == APHO_IN_PROGRESS)
            {
              mccsa_apho_substate = PC_LOSS_APHO_IN_PROGRESS;
            }
          }
          else if (mccsa_access_ho_is_allowed())
          {
            MSG_MED ("Perform (APHO if still probing) or AHO",0,0,0);

            /* -----------------------------------------------------------
            ** In case of Access Handoff, SRCH will report a strong pilot
            ** after T72M timer expires, otherwise the Fade is declared
            ** after T40M timer expires and Mobile goes back to IDLE.
            ** --------------------------------------------------------*/
          }
          else
          {
            /* Mobile is not allowed for Access Handoffs */
            MSG_MED ("Waiting for either T40M or Valid PC msg",0,0,0 );

            /* Don't need to suspend access probe here.  It's done in MUX
            ** code before sending RXC_PC_T72M_TMO_R to MC .*/
          }
        }
        else if (rpt_ptr->hdr.rpt == RXC_VALID_PC_MSG_R)
        {
          MSG_HIGH( " Recevied Valid PC msg before T40M timer",0,0,0);

          /* Mark TXC has not been resumed */
          resume_txc = FALSE;

          if (mccsa_apho_substate == PC_LOSS_WAIT_FOR_SRCH)
          {
            mccsa_apho_substate = APHO_NOT_IN_PROGRESS;
          }
          else if (mccsa_apho_substate == PC_LOSS_APHO_IN_PROGRESS)
          {
            mccsa_apho_substate = APHO_IN_PROGRESS;
            if (mcc_is_aho_ovhd_msg_okay())
            {
              /* All the Overhead Messages have been updated. */
              /* Prepare for the next Pilot Report.           */
              next_state = mccsa_apho_done(next_state);

              /* Mark TXC has been resumed by ACC_PROBE_F */
              resume_txc = TRUE;

              MSG_MED("Last APHO done %d %d 0x%x", cdma.ho_state,
                      mccsa_apho_substate, next_state);
            }
          }
          /* If exitting System Access State because of some problems with */
          /* Access Probe Handoff, stop TXC.                               */
          if (next_state == CDMA_EXIT)
          {
             mccsa_tx_stop();
             /* Failed access - perform registration processing */
             mccreg_idle_ack_or_fail();
          }
          else if (cdma.ho_state != CDMA_APHO ||
                   mccsa_apho_substate != APHO_IN_PROGRESS)
          {
            /* ---------------------------------------------------
            ** If we received MC_END_F during AHO, we need to send
            ** Release Order.
            ** --------------------------------------------------- */
            if ((mccsa_orig_ministate == CDMA_ORIG_RELS) &&
                 mcc_is_aho_ovhd_msg_okay()
                && (mccsa_apho_substate == APHO_NOT_IN_PROGRESS)
                && ((mccsa_layer2.last_msg_type != CAI_AC_ORD_MSG) ||
                    (mccsa_layer2.last_ord_type != CAI_RELEASE_ORD))
               )
            {
              if (resume_txc)
              {
                mccsa_tx_stop();
              }
              mccsa_layer3_msg_pending = FALSE;
              next_state = mccsa_send_release_order_on_ac(curr_state,
                mccsa_release_cause);
              MSG_MED("Proceed to send rel order", 0, 0, 0);
            }
            else
            {
              if (!resume_txc)
              {
                /* Re-enable the transmitter and resume the operation */
                txc_suspend_tx( FALSE );
                MSG_MED("Resume TXC", 0, 0, 0);
              }
            }
          }
          /* Otherwise the APHO is in progress and after that TXC will be commanded */
          /* to come out of suspended mode.                                         */
          else
          {
            MSG_MED("APHO in progress %d %d %d", mcc_is_aho_ovhd_msg_okay(),
              cdma.ho_state, mccsa_apho_substate);
          }
        }
        else
        {
          MSG_MED( "Invalid rx rpt %d", rpt_ptr->hdr.rpt, 0, 0 );
        }

        cmd_done( &rpt_ptr->hdr.rpt_hdr );
      }
    } /* end else if ((sigs & MCC_RX_RPT_Q_SIG) != 0) */

    /* ------------------------------------------
    ** Check if System Access Timer has timed out
    ** ------------------------------------------ */
    else if ((sigs & MCC_STATE_TIMER_SIG) != 0)
    {
      (void) rex_clr_sigs( &mc_tcb, MCC_STATE_TIMER_SIG );
      MSG_MED("Timer T42 expired", 0, 0, 0);

      next_state = CDMA_EXIT;
      mcc_exit.exit = MCC_ACCESS_TIMEOUT;

      #ifdef FEATURE_SO_8026
#error code not present
      #endif

      if ((curr_state == CDMA_ORIG) || (curr_state == CDMA_PAGE_RES)) /*lint !e641 */
      {

        #ifdef FEATURE_IS2000_REL_A_CC
        if (OVHD_CHAN_IS_BCCH)
        {
          /* Set MC_RR_PAR_MSG_SEQs & ACC_MSG_SEQs to NULL */
          cur_bs_ptr->mcrrpm_config_msg_seq =
            cur_bs_ptr->eapm.acc_msg_seq = INVALID_SEQ;
        }
        else
        #endif /* FEATURE_IS2000_REL_A_CC */

        {
          /* Set SYS_PAR_MSG_SEQs & ACC_MSG_SEQs to NULL */
           cur_bs_ptr->csp.sp.config_msg_seq =
             cur_bs_ptr->ap.acc_msg_seq = INVALID_SEQ;
        }
      }
      /* Event report: Timer T42m expired */
      mclog_report_event_timer(CAI_TIMER_T42M);
    } /* end else if ((sigs & MCC_STATE_TIMER_SIG) != 0) */

    /* ------------------------------------------------------------------
    ** Check if next probe has started in this access attempt
    ** ------------------------------------------------------------------ */
    else if (sigs & MCC_TXC_PROBE_SIG)
    {
      (void) rex_clr_sigs(&mc_tcb, MCC_TXC_PROBE_SIG);

      #ifdef FEATURE_IS2000_REL_D
#error code not present
      #endif /* FEATURE_IS2000_REL_D */

      /* -------------------------------------------------------------------
      ** If MC has requested TXC to suspend Access Probes or TXC is already
      ** suspended, then, this Signal has arrived because of a Race Condition.
      ** No action should be taken on it.
      ** -------------------------------------------------------------------*/
      if (mccsa_apho_substate == APHO_NOT_IN_PROGRESS)
      {
        /* ---------------------------------------------------------------
        ** Command Searcher task to send pilot strength measurement report
        ** --------------------------------------------------------------- */
        mcc_srch_buf.pilot_rpt.hdr.cmd = SRCH_PILOT_LIST_F;
        mcc_srch_cmd(&mcc_srch_buf);

        if (mcc_srch_buf.pilot_rpt.hdr.status != SRCH_DONE_S)
        {
          /* ---------------------------------------------------------------
          ** Searcher did not report, so reuse the previous measurements for
          ** all other pilots, but get the current value for the Active Set
          ** Pilot directly from Searcher
          ** ---------------------------------------------------------------*/
          MSG_MED("Srch didn't report, only update Active Pilot info",0,0,0);
          mccsa_update_aho_active_pilot();

          /* Send Access Channel Msg with pilot reporting info  */
          next_state = mccsa_send_acc_probe_cmd(curr_state, &mcc_tx_int_msg,
            plt_rpt_ptr);
        }
      }
    }

    else if (sigs & MCC_TXC_APHO_OK_SIG)
    {
      (void) rex_clr_sigs(&mc_tcb,MCC_TXC_APHO_OK_SIG);

      if ((cdma.ho_state == CDMA_APHO) &&
          (mccsa_apho_substate == WAIT_FOR_TXC_OK))
      {
         /* Initiate Access Probe HO */
         mccsa_apho_substate = APHO_IN_PROGRESS;
         MSG_MED("Initiate APHO(Insufficiency) # %d",mccsa_num_apho_done,0,0);
         next_state = mccidl_handoff((word) mcc_srch_rpt.pc_meas.pilot,
                                     (word) mcc_srch_rpt.pc_meas.freq,
                                     (byte) mcc_srch_rpt.pc_meas.band_class,
                                     next_state,
                                     (int8) mcc_srch_rpt.pc_meas.ecio_old_pilot,
                                     (int8) mcc_srch_rpt.pc_meas.ecio_new_pilot,
                                     CDMA_APHO);
      }
      else
      {
        /* TXC Sent OK when not expected?? */
        /* This could have happened because of a Race Condition - */
        /* MC got a layer 2 Ack, thus changing the state to AHO   */
        /* Or, may be IDLE(if layer 2 Ack came with CAM or APHO   */
        /* was in progress for message other than Orig/Page Res   */
        /* Just ignore this signal in such a condition.           */
        MSG_HIGH("Access Probe HO OK Sig Unexpected!!",0,0,0);
      }
    }

    /* ------------------------------------------------------------------
    ** Check if max probe sequences have been txed in this access attempt
    ** ------------------------------------------------------------------ */
    else if (sigs & MCC_TXC_DONE_SIG)
    {
      (void) rex_clr_sigs( &mc_tcb, MCC_TXC_DONE_SIG );

      MSG_HIGH( "Max access probes attempted", 0, 0, 0 );

      /* Increment the counter of number of unsuccessful access attempts. */
      mccsa_increment_l3_counter(SA_FAIL);

      #if (defined(FEATURE_CDSMS) || defined (FEATURE_DBM))
      if( ( curr_state == CDMA_MSG ) /*lint !e641 */
            && ( cdma.sa_msg_tx_msg == CAI_AC_BURST_MSG )  /*lint !e641 */
        )/*lint !e641 */
      {
        /* notify SMS task of the access failure */
        MSG_MED("wait_for_resp call mcc_check_mo_dbm_status", 0, 0, 0);
        mcc_check_mo_dbm_status( CM_DBM_MO_L2_ACK_FAILURE );
      }
      #endif /* FEATURE_CDSMS || FEATURE_DBM */

      /* Failed access - perform registration processing */
      mccreg_idle_ack_or_fail();

      /* Exit to System Determination state */
      next_state    = CDMA_EXIT;
      if ((curr_state == CDMA_ORIG) || (curr_state == CDMA_PAGE_RES) || /*lint !e641 */
          (curr_state == CDMA_REG)) /*lint !e641 */
      {
        mcc_exit.exit = MCC_MAX_ACCESS_PROBES;
        mccsa_orig_fail_layer = CM_CALL_ORIG_ERR_LAYER_L2;
      }
      else
      {
        mcc_exit.exit = MCC_SYSTEM_LOST;
      }
      /* if the failed release order is casued by powerdown, we want to exit
       * and powerdown instead of going to System Determination */
      if ((curr_state == CDMA_ORIG) && /*lint !e641 */
          (mccsa_orig_ministate == CDMA_ORIG_RELS) &&
          (mccsa_release_cause == CAI_REL_PWDN))
      {
        mcc_exit.exit = MCC_POWERDOWN;
        MSG_MED("Release process ended due to max_probe, power down",0,0,0);
      }

      /* ---------------------------------------------------------
      ** Events report: Access attempt failure - Maximum number of
      ** access probes sent.
      ** --------------------------------------------------------- */
      event_report(EVENT_ACCESS_ATTEMPT_FAIL_MAX_PROBES_SENT);

    } /* end else if (sigs & MCC_TXC_DONE_SIG) */

    #ifdef FEATURE_SO_8026
#error code not present
    #endif /* FEATURE_SO_8026 */

    #ifdef FEATURE_IS2000_REL_C_AKA
#error code not present
    #endif /* FEATURE_IS2000_REL_C_AKA */

    #if (defined(FEATURE_DDTM_CNTL) && defined(FEATURE_HDR))
#error code not present
    #endif /* (defined(FEATURE_DDTM_CNTL) && defined(FEATURE_HDR)) */

  } /* end while (next_state == curr_state) */

  if ((curr_state == CDMA_REG) && (cdma.reg_type == CAI_POWERDOWN_REG)) /*lint !e641 */
  {
    if (cdma.powerdown_reg_type == CDMA_NORMAL) {

      /* --------------------------------------------------------------
      ** If this was a powerdown registration exit CDMA mode with
      ** powerdown indication regardless of success of access
      ** -------------------------------------------------------------- */
      next_state    = CDMA_EXIT;
      mcc_exit.exit = MCC_POWERDOWN;
    }
    else if (cdma.powerdown_reg_type == CDMA_SHUTDOWN)
    {
      /* --------------------------------------------------------------
      ** The mobile attempted to send a powerdown registration message,
      ** as 1X is being shutdown.  Reset the registration status so
      ** that a powerup registration message will be sent when 1X
      ** is activated.
      ** -------------------------------------------------------------- */
      mccreg_reset_reg_status();

      /* --------------------------------------------------------------
      ** If this was a powerdown registration exit CDMA mode with
      ** shutdown indication regardless of success of access
      ** -------------------------------------------------------------- */
      next_state    = CDMA_EXIT;
      mcc_exit.exit = MCC_SHUTDOWN;
    }

    else if (cdma.powerdown_reg_type == CDMA_LPM) {
      /* --------------------------------------------------------------
      ** if this was a powerdown registration because of entry into
      ** Low Power Mode, exit CDMA mode with exit reason as LPM
      ** -------------------------------------------------------------- */

      next_state = CDMA_EXIT;
      mcc_exit.exit = MCC_LPM;
    }
    
    else
    {
      next_state = CDMA_EXIT;
      mcc_exit.exit = MCC_POWERDOWN;
      ERR_FATAL("Invalid registration type : %d",
                cdma.powerdown_reg_type, 0, 0);
    }
  }

  return (next_state);

} /* end mccsa_wait_for_access_response */

/*===========================================================================

FUNCTION CDMA_SA

DESCRIPTION
  This function is the lowest level state machine for the System Acces
  state.  It determines which substate needs to be processed and calls
  the appropriate procedure to process the substate.

DEPENDENCIES
  None.

RETURN VALUE
  The next state to be processed.

SIDE EFFECTS
  The mobile will attempt to access the system and send a message to the
  base station during the System Access state.

===========================================================================*/

word cdma_sa( void )
{
  /* Assume that the Idle state has sent a message to the Receive task to
     start a System Access fade timer */

  word new_state = cdma.curr_state;

  #ifdef FEATURE_EVRC_SO_MGMT
  db_items_value_type item; 
   /* storage for db items */
  #endif /* FEATURE_EVRC_SO_MGMT */

  #ifdef FEATURE_PASSPORT_MODE
  cm_mc_rpt_type *cmd_ptr;
  #endif /* FEATURE_PASSPORT_MODE */

  sd_ss_sys_lost_e_type sys_lost_reason = SD_SS_SYS_LOST_UNKNOWN;
    /* Reason for system lost */

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

  /* Initialize layer 2 flag to indicate no access attempt is in progress */
  mccsa_layer2.access_in_progress = FALSE;

  /* -------------------------------------------------------------------
  ** Initialize sequence number of last registration message or implicit
  ** registration.  This is used to determine what type of registration
  ** to do after a successful access.
  ** ------------------------------------------------------------------- */
  mccsa_layer2.last_reg_msg_seq = INVALID_SEQ;

  /* Initialize the buffer for internal and external format of (Enhanced)
     Access Channel message */
  memset( &mcc_tx_int_msg, 0, sizeof(caii_tx_msg_type) );
  memset( &mcc_tx_ext_msg, 0, sizeof(cai_tx_msg_type) );

  #ifdef FEATURE_UIM_RUIM

  #if defined( FEATURE_UIM_RUN_TIME_ENABLE )
  /* Determine if the R-UIM is available for this NAM */
  if (nv_rtre_use_ruim_for_nam(cdma.curr_nam))
  #endif /* FEATURE_UIM_RUN_TIME_ENABLE */

  {
    /* Indicate to RUIM to power on as UIM may be required for access */
    (void) uim_power_control(UIM_MC, TRUE);
  }
  #endif /* FEATURE_UIM_RUIM */

  #ifdef FEATURE_PASSPORT_MODE
  /* Initialize the persistence test NG flag */
  mccsa_persistence_orig_ng = FALSE;
  #endif /* FEATURE_PASSPORT_MODE */

  // Initialize the ministate to indicate not in release process
  mccsa_orig_ministate = CDMA_ORIG_NORM;

  mccsa_orig_fail_layer = CM_CALL_ORIG_ERR_LAYER_OTHER;

  #ifdef FEATURE_SO_8026
#error code not present
  #endif /* FEATURE_SO_8026 */

  #if defined(FEATURE_IS2000_REL_C_FCS) || defined(FEATURE_IS2000_REL_D)
  /* Clear Sync ID call setup variables */
  mccsyncid_clear_call_info();
  #endif /* FEATURE_IS2000_REL_C_FCS || FEATURE_IS2000_REL_D */

  #ifdef FEATURE_IS2000_REL_C_AKA
#error code not present
  #endif /*FEATURE_IS2000_REL_C_AKA */

  while (MAIN_STATE(new_state) == CDMA_SYS_ACC)
  {

    cdma.curr_state = new_state;

    #ifdef FEATURE_GPSONE_NEW_L2_ACK
#error code not present
    #endif /* FEATURE_GPSONE_NEW_L2_ACK */

    /* Event report: Call processing state change */
    mclog_report_event_cp_state(cdma.curr_state);

    switch (new_state)
    {
      case CDMA_ORIG:

        #ifdef FEATURE_EVRC_SO_MGMT
        /* UI would set the SO to CAI_SO_VOICE_WILDCARD if a 'voice' call is
           originated by user else would set the SO to the option requested by 
           DIAG. If the SO is a wildcard, then the preferred voice options 
           stored in NV are used to determine the service option to originate in */

        if (cdma.so_req == CAI_SO_VOICE_WILDCARD)
        {
          /* Get roam status of phone from db */
          db_get(DB_ROAM, &item);

          /* check to see if this origination is going to be put through in the
             home system */
          if (item.roam == DB_ROAM_OFF)
          {
            /* use the preferred service option for originations in home systems */
            cdma.so_req = mcc_pref_voice_so.home_orig_voice_so;
          }
          else
          {
            /* use the preferred SO for originations in roaming systems */
            cdma.so_req = mcc_pref_voice_so.roam_orig_voice_so;
          }

          cdma.so_req = snm_eval_so(cdma.so_req, NULL);
        }
        #endif /* FEATURE_EVRC_SO_MGMT */

        /* If this is a data call, the service option may need to change since
           P_REV may have changed by now.  So let's verify the So here. */
        if (DS_ISDATA(cdma.so_req))
        {
          cdma.so_req = ds707_extif_verify_so(cdma.so_req);
        }

        #ifdef FEATURE_IS2000_REL_C
        if (P_REV_IS_9_OR_GREATER && mccap_item_is_supported(MCCAP_DF_RECONNECT_MSG) &&
            cur_bs_ptr->csp.esp.reconnect_msg_ind &&
            DS_ISDATA(cdma.so_req) && cdma.is_dormant_packet_data & cdma.drs &&
            (mccsyobj_get_sid() == mccsyobj_get_old_pd_orig_sid()) &&
            (mccsyobj_get_nid() == mccsyobj_get_old_pd_orig_nid()) &&
            (mccsyobj_get_pzid() == mccsyobj_get_old_pd_orig_pzid())) 
        {
          /* Reconnect a dormant packet data service instance */
          new_state = mccsa_send_reconnect_msg(new_state, TRUE);
        }
        else
        #endif /* FEATURE_IS2000_REL_C */
        {
          new_state = mccsa_send_origination_msg(new_state);
        }
        break;

      case CDMA_PAGE_RES:

        #ifdef FEATURE_IS2000_REL_C
        if (P_REV_IS_9_OR_GREATER && mccap_item_is_supported(MCCAP_DF_RECONNECT_MSG) &&
            cur_bs_ptr->csp.esp.reconnect_msg_ind && DS_ISDATA(cdma.so_req))
        {
          /* Reconnect a dormant packet data service instance */
          new_state = mccsa_send_reconnect_msg(new_state,
            #ifdef FEATURE_IS2000_REL_D 
#error code not present
            #endif /* FEATURE_IS2000_REL_D */
            TRUE);
        }
        else
        #endif /* FEATURE_IS2000_REL_C */
        {
          new_state = mccsa_send_page_response_msg(new_state, 
            #ifdef FEATURE_IS2000_REL_D 
#error code not present
            #endif /* FEATURE_IS2000_REL_D */
            TRUE);
        }
        break;

      case CDMA_REG:
        new_state = mccsa_send_registration_msg( new_state );
        break;

      case CDMA_MSG:
        #ifdef FEATURE_IS2000_REL_A
        if ( cdma.sa_msg_tx_msg == CAI_AC_SMRM_MSG )
        {
          new_state = mccsa_send_smrm(new_state);
        }
        #endif /* FEATURE_IS2000_REL_A */
        #if (defined(FEATURE_CDSMS) || defined(FEATURE_DBM))
        if (cdma.sa_msg_tx_msg == CAI_AC_BURST_MSG)
        {
          new_state = mccsa_send_data_burst(new_state);
        }
        #endif /* FEATURE_CDSMS || FEATURE_DBM */
        break;

      case CDMA_ORD_RES:
        new_state = mccsa_send_order_response_msg(new_state, FALSE);
        break;

      default:
        ERR_FATAL("Internal error bad state %d", new_state, 0, 0);
    } /* end of switch */

    /* If we have changed state, we have not started an access attempt */
    if (new_state == cdma.curr_state)
    {
      new_state = mccsa_wait_for_access_response( new_state );
    }

    #ifdef FEATURE_SO_8026
#error code not present
    #endif /* FEATURE_SO_8026 */

    #ifdef FEATURE_AUTH
    /* -----------------------------------------------------------------
    ** We may be in the middle of doing some authentication calculation,
    ** like an auth signature, which sends a report when it completes.
    ** if this is the case, we wait for that report now to ensure it is
    ** not improperly interpreted later, and to ensure the report buffer
    ** is returned to the right queue.
    ** ----------------------------------------------------------------- */
    mcc_resync_auth();
    #endif /* FEATURE_AUTH */

  } /* end of while */

  (void) rex_clr_sigs(&mc_tcb, MCC_STATE_TIMER_SIG);
  (void) rex_clr_timer(&mcc_state_timer);

  #if (defined(FEATURE_CDSMS) || defined(FEATURE_DBM))
  /* ----------------------------------------------------------------
  ** We are about to leave SA, if there is an SMS still pending for
  ** Access Channel, the original SMS attempt must be interrupted for
  ** some reason, we notify SMS here. Notice that if there is an SMS
  ** still on the cmd queue, we won't set cdma.dbm_wait_for_l2_ack.
  ** i.e., if the flag is set, we must have pulled the SMS out and
  ** dropped somewhere.
  ** ---------------------------------------------------------------- */
  mcc_check_mo_dbm_status(CM_DBM_MO_L2_ACK_FAILURE);
  #endif /* FEATURE_CDSMS || FEATURE_DBM */

  if (new_state == CDMA_EXIT)
  {

    /* Send a message to tell TX to go back Idle state in case it got stuck
     * in the TXC_JUMP_STATE. If it is already out of that state, TXC will 
     * return BAD_CMD status. */
    mccsa_tx_stop();
    MSG_MED("Tell TXC to go back to IDLE State.",
            0,0,0);

    #ifdef FEATURE_IS2000_REL_A_SVD
    /*
    ** We are about to exit CDMA, make sure all the SR_IDs are properly
    ** released.
    */
    mccsrid_release();

    /*
    ** Catch all to prevent possible sr_id leak
    */
    mccsrid_free_all_owned_srids(SRID_OWNER_MC);
    #endif /* FEATURE_IS2000_REL_A_SVD */

    /* --------------------------------------------------------------------
    ** Take necessary action for origination in-progress based on exit code
    ** -------------------------------------------------------------------- */
    switch (mcc_exit.exit)
    {
      case MCC_ACCESS_COMPLETED:
        /* ---------------------------------------------------------------
        ** Inform system determination of access completion and determines
        ** the next state to be processed.
        ** --------------------------------------------------------------- */
        if (mcc_continue_on_curr_sys(MEAS_BACK_ALLOWED,
            sd_ss_ind_cdma_opr_acc_term(mccsa_map_sa_state_to_sa_reason(
            cdma.curr_state), SD_SS_ACC_TERM_NORMAL, NULL)))
        {
          new_state = CDMA_IDLE;
        }
        break;

      case MCC_ACCESS_DENIED:
        #ifdef FEATURE_PASSPORT_MODE
        if(mcc_passport_mode_is_jcdma())
        {       
          if ((cdma.curr_state == CDMA_ORIG) ||
              (cdma.curr_state == CDMA_PAGE_RES) ||
              (cdma.curr_state == CDMA_REG))
          {
            if ((cmd_ptr = cm_mc_rpt_get_buf_else_err_fatal()) != NULL)
            {
              cmd_ptr->hdr.cmd = CM_REG_FAILURE_F;

              /* Send command to CM */
              MSG_HIGH("CM Registration Failure", 0, 0, 0);
              mcc_cm_mc_rpt(cmd_ptr);
            }
            else
            {
              ERR("No free buffers on cm_mc_rpt_free_q", 0, 0, 0);
            }
          }
        }
        #endif /* FEATURE_PASSPORT_MODE */

        /* ------------------------------------------------------------
        ** Inform System Determination of Access exit and determine the
        ** next state to be processed based on suggested SS-Action.
        ** ------------------------------------------------------------ */
        if (mcc_continue_on_curr_sys(MEAS_BACK_ALLOWED,
            sd_ss_ind_cdma_opr_acc_term(mccsa_map_sa_state_to_sa_reason(
            cdma.curr_state), SD_SS_ACC_TERM_ACC_DEN, NULL)))
        {
          new_state = CDMA_IDLE;
        }

        if (cdma.curr_state == CDMA_ORIG) /*lint !e641 */
        {

          #ifdef FEATURE_PASSPORT_MODE
          if (mcc_passport_mode_is_jcdma() && mccsa_persistence_orig_ng)
          {
            /* Inform CM origination failed */
            mcc_send_orig_failure(CM_CALL_ORIG_ERR_CDMA_PSIST_NG, cdma.call_id,
                                  mccsa_orig_fail_layer);
            cdma.call_orig = FALSE;
          }
          else
          #endif /* FEATURE_PASSPORT_MODE */

          {
            /* Inform CM origination failed */
            mcc_send_orig_failure(CM_CALL_ORIG_ERR_ACC_FAIL, cdma.call_id,
                                  mccsa_orig_fail_layer);
            cdma.call_orig = FALSE;
          }
        }
        break;

      case MCC_ACCESS_FAILED:
        /* ------------------------------------------------------------
        ** Inform System Determination of Access exit and determine the
        ** next state to be processed based on suggested SS-Action.
        ** ------------------------------------------------------------ */
        if (mcc_continue_on_curr_sys(MEAS_BACK_ALLOWED,
            sd_ss_ind_cdma_opr_acc_term(mccsa_map_sa_state_to_sa_reason(
            cdma.curr_state), SD_SS_ACC_TERM_ACC_FAIL, NULL)))
        {
          new_state = CDMA_IDLE;
        }

        if (cdma.curr_state == CDMA_ORIG) /*lint !e641 */
        {
          /* Inform CM origination failed */
          mcc_send_orig_failure(CM_CALL_ORIG_ERR_ACC_FAIL, cdma.call_id,
                                mccsa_orig_fail_layer);
          cdma.call_orig = FALSE;
        }
        break;

      case MCC_ACCESS_FAILED_GSRM:
        if (cdma.curr_state == CDMA_ORIG) /*lint !e641 */
        {
          /* Inform CM origination failed */
          mcc_send_orig_failure(CM_CALL_ORIG_ERR_ACC_FAIL, cdma.call_id,
                                mccsa_orig_fail_layer);
          cdma.call_orig = FALSE;
        }
        break;

      case MCC_ACCESS_FAILED_SYS_LOST:
        /* ------------------------------------------------------------
        ** Inform System Determination of Access exit and determine the
        ** next state to be processed based on suggested SS-Action.
        ** ------------------------------------------------------------ */
        if (mcc_continue_on_curr_sys(MEAS_BACK_ALLOWED,
            sd_ss_ind_cdma_opr_acc_term(mccsa_map_sa_state_to_sa_reason(
            cdma.curr_state), SD_SS_ACC_TERM_ACC_FAIL, NULL)))
        {
            new_state = CDMA_IDLE;
        }

        if (cdma.curr_state == CDMA_ORIG) /*lint !e641 */
        {
          #ifdef FEATURE_PASSPORT_MODE
          if(mcc_passport_mode_is_jcdma())
          {
          
            mcc_send_orig_failure(CM_CALL_ORIG_ERR_ACC_FAIL, cdma.call_id,
                                  mccsa_orig_fail_layer);
          }
          else
          #endif
          {
          
            mcc_send_orig_failure(CM_CALL_ORIG_ERR_REORDER, cdma.call_id,
                                  mccsa_orig_fail_layer);
          }

          cdma.call_orig = FALSE;

        }
        break;

      case MCC_ACCESS_TIMEOUT:
        /* ------------------------------------------------------------
        ** Inform System Determination of Access exit and determine the
        ** next state to be processed based on suggested SS-Action.
        ** ------------------------------------------------------------ */
        if (mcc_continue_on_curr_sys(MEAS_BACK_ALLOWED,
            sd_ss_ind_cdma_opr_acc_term(mccsa_map_sa_state_to_sa_reason(
            cdma.curr_state), SD_SS_ACC_TERM_TIMEOUT, NULL)))
        {
          new_state = CDMA_IDLE;
        }

        if (cdma.curr_state == CDMA_ORIG) /*lint !e641 */
        {
          /* Inform CM origination failed */
          #ifdef FEATURE_PASSPORT_MODE
          if(mcc_passport_mode_is_jcdma())
          {
            mcc_send_orig_failure(CM_CALL_ORIG_ERR_ACC_FAIL, cdma.call_id,
                                  mccsa_orig_fail_layer);
          }
          else
          #endif
          {
            mcc_send_orig_failure(CM_CALL_ORIG_ERR_REORDER, cdma.call_id,
                                  mccsa_orig_fail_layer);
          }
          

          cdma.call_orig = FALSE;
        }
        break;

      case MCC_INTERCEPT:
        /* ---------------------------------------------------------------
        ** Inform system determination of intercept order and determine
        ** the next state to be processed based on the returned SS-Action.
        ** --------------------------------------------------------------- */
        if (mcc_continue_on_curr_sys(MEAS_BACK_ALLOWED,
            sd_ss_ind_cdma_opr_acc_term(mccsa_map_sa_state_to_sa_reason(
            cdma.curr_state), SD_SS_ACC_TERM_INTERCEPT, NULL)))
        {
          new_state = CDMA_IDLE;
        }

        if (cdma.curr_state == CDMA_ORIG)  /*lint !e641 */
        {
          /* Inform CM origination failed */
          mcc_send_orig_failure(CM_CALL_ORIG_ERR_ACC_FAIL, cdma.call_id,
                                mccsa_orig_fail_layer);
          cdma.call_orig = FALSE;
        }
        break;

      case MCC_LOCK:
        #ifdef FEATURE_PASSPORT_MODE
        if(mcc_passport_mode_is_jcdma())
        {
          if ((cdma.curr_state == CDMA_ORIG) || (cdma.curr_state == CDMA_PAGE_RES)
              || (cdma.curr_state == CDMA_REG))
          {
            if ((cmd_ptr = cm_mc_rpt_get_buf_else_err_fatal()) != NULL)
            {
              cmd_ptr->hdr.cmd = CM_REG_FAILURE_F;

              /* Send command to CM */
              MSG_HIGH("CM Registration Failure", 0, 0, 0);
              mcc_cm_mc_rpt(cmd_ptr);
            }
            else
            {
              ERR("No free buffers on cm_mc_rpt_free_q", 0, 0, 0);
            }
          }
        }
        #endif /* FEATURE_PASSPORT_MODE */

        /* ----------------------------------------------------------------
        ** Inform System Determination of Lock Order and determine the next
        ** state to be processed based on the returned SS-Action.
        ** --------------------------------------------------------------- */
        if (mcc_continue_on_curr_sys(MEAS_BACK_ALLOWED,
            sd_ss_ind_cdma_opr_lock(NULL)))
        {
          new_state = CDMA_IDLE;
        }
        if (cdma.curr_state == CDMA_ORIG) /*lint !e641 */
        {
          /* Inform CM origination failed */
          mcc_send_orig_failure(CM_CALL_ORIG_ERR_CDMA_LOCK, cdma.call_id,
                                mccsa_orig_fail_layer);
          cdma.call_orig = FALSE;
        }
        break;

      case MCC_MAX_ACCESS_PROBES:
        #ifdef FEATURE_PASSPORT_MODE
        if(mcc_passport_mode_is_jcdma())
        {
          if ((cdma.curr_state == CDMA_ORIG) ||
              (cdma.curr_state == CDMA_PAGE_RES) ||
              (cdma.curr_state == CDMA_REG))
          {
            if ((cmd_ptr = cm_mc_rpt_get_buf_else_err_fatal()) != NULL)
            {
              cmd_ptr->hdr.cmd = CM_REG_FAILURE_F;

              /* Send command to CM */
              MSG_HIGH("CM Registration Failure", 0, 0, 0);
              mcc_cm_mc_rpt(cmd_ptr);
            }
            else
            {
              ERR("No free buffers on cm_mc_rpt_free_q", 0, 0, 0);
            }
          }
        }
        #endif /* FEATURE_PASSPORT_MODE */

        /* ------------------------------------------------------------
        ** Inform System Determination of Access exit and determine the
        ** next state to be processed based on suggested SS-Action.
        ** ------------------------------------------------------------ */
        if (mcc_continue_on_curr_sys(MEAS_BACK_ALLOWED,
            sd_ss_ind_cdma_opr_acc_term(mccsa_map_sa_state_to_sa_reason(
            cdma.curr_state), SD_SS_ACC_TERM_MAX_ACC, NULL)))
        {
          new_state = CDMA_IDLE;
        }

        /* Inform CM origination failed */
        if (cdma.curr_state == CDMA_ORIG) /*lint !e641 */
        {
          /* Inform CM origination failed */
          mcc_send_orig_failure(CM_CALL_ORIG_ERR_MAX_ACC, cdma.call_id,
                                mccsa_orig_fail_layer);
          cdma.call_orig = FALSE;
        }
        break;

      case MCC_NEW_SYSTEM:
        if (cdma.ho_state == CDMA_AHO || cdma.ho_state == CDMA_APHO)
        {
          /* MS tried to handoff to a system with Nghbr Config set to 3 */
          /* We could be in TX Suspended State. Stop TX                 */
          mccsa_tx_stop();

          /* Failed access - perform registration processing */
          mccreg_idle_ack_or_fail();

          if (cdma.curr_state == CDMA_ORIG) /*lint !e641 */
          {
            /* Inform CM origination failed */
            mcc_send_orig_failure(CM_CALL_ORIG_ERR_HANDOFF, cdma.call_id,
                                  mccsa_orig_fail_layer);
            cdma.call_orig = FALSE;
          }
        }
        break;

      case MCC_NDSS_OFF:
        /* --------------------------------------------------------------
        ** Inform system determination of NDSS Off.
        ** Determine the next state to be processed based on the returned
        ** SS-Action.
        ** -------------------------------------------------------------- */
        if (mcc_continue_on_curr_sys(MEAS_BACK_ALLOWED,
            sd_ss_ind_cdma_opr_ndss_off(NULL)))
        {
          new_state = CDMA_IDLE;
        }
        break;

      case MCC_REDIRECTION:
        /* ------------------------------------------------------------
        ** Inform system determination of done with accept redirection.
        ** Determine the next state to be processed based on the
        ** returned SS-Action.
        ** ------------------------------------------------------------ */
        if (mcc_continue_on_curr_sys(MEAS_BACK_ALLOWED,
            sd_ss_ind_cdma_opr_acpt_rej(NULL)))
        {
          new_state = CDMA_IDLE;
        }

        if (cdma.curr_state == CDMA_ORIG) /*lint !e641 */
        {
          /* Inform CM origination failed */
          mcc_send_orig_failure(CM_CALL_ORIG_ERR_REDIR, cdma.call_id, 
                                mccsa_orig_fail_layer);
          cdma.call_orig = FALSE;
        }
        break;

      case MCC_RELEASE:
        /* -------------------------------------------------------------
        ** Inform system determination of release exit and determine the
        ** next state to be processed based on returned SS-Action.
        ** ------------------------------------------------------------- */
        if (((cdma.rel_orig) && (mcc_continue_on_curr_sys(MEAS_BACK_ALLOWED,
             sd_ss_ind_cdma_opr_acc_term(mccsa_map_sa_state_to_sa_reason(
             cdma.curr_state), SD_SS_ACC_TERM_MS_REL, NULL)))) ||
            ((!cdma.rel_orig) && (mcc_continue_on_curr_sys(MEAS_BACK_ALLOWED,
             sd_ss_ind_cdma_opr_acc_term(mccsa_map_sa_state_to_sa_reason(
             cdma.curr_state), SD_SS_ACC_TERM_BS_REL, NULL)))))
        {
          new_state = CDMA_IDLE;
        }
        /* ---------------------------------------------------------
        ** If the release is mobile originated, no response to CM.
        ** For a base station initiated release, CM is notified when
        ** the release message is received, so no further notification
        ** is required here.
        ** --------------------------------------------------------- */
        break;

      case MCC_REORDER:
        /* ---------------------------------------------------------------
        ** Inform system determination of reorder and determine the next
        ** state to be processed based on the returned SS-Action.
        ** --------------------------------------------------------------- */
        if (mcc_continue_on_curr_sys(MEAS_BACK_ALLOWED,
            sd_ss_ind_cdma_opr_acc_term(mccsa_map_sa_state_to_sa_reason(
            cdma.curr_state), SD_SS_ACC_TERM_REORDER, NULL)))
        {
          new_state = CDMA_IDLE;
        }

        if (cdma.curr_state == CDMA_ORIG) /*lint !e641 */
        {
          /* Inform CM origination failed */
          mcc_send_orig_failure(CM_CALL_ORIG_ERR_REORDER, cdma.call_id,
                                mccsa_orig_fail_layer);
          cdma.call_orig = FALSE;
        }
        break;

      case MCC_REG_REJECTED:
        /* --------------------------------------------------------
        ** Inform system determination of registration rejected and
        ** determines the next state to be processed based on the
        ** returned SS-Action.
        ** -------------------------------------------------------- */
        if (mcc_continue_on_curr_sys(MEAS_BACK_ALLOWED,
            sd_ss_ind_cdma_opr_reg_rej(NULL)))
        {
          new_state = CDMA_IDLE;
        }

        /* Inform CM origination failed */
        if (cdma.curr_state == CDMA_ORIG) /*lint !e641 */
        {
          /* Inform CM origination failed */
          mcc_send_orig_failure(CM_CALL_ORIG_ERR_ACC_FAIL, cdma.call_id,
                                mccsa_orig_fail_layer);
          cdma.call_orig = FALSE;
        }
        break;

      case MCC_SRCH_REQUEST_SYS_RESTART:
        sys_lost_reason = SD_SS_SYS_LOST_ADVISE_UNLOCK_RF;

        /*lint -fallthrough intentional */

      case MCC_SYSTEM_LOST:
        #ifdef FEATURE_PASSPORT_MODE
        if(mcc_passport_mode_is_jcdma())
        {
          if ((cdma.curr_state == CDMA_ORIG) ||
              (cdma.curr_state == CDMA_PAGE_RES) ||
              (cdma.curr_state == CDMA_REG))
          {
            if ((cmd_ptr = cm_mc_rpt_get_buf_else_err_fatal()) != NULL)
            {
              cmd_ptr->hdr.cmd = CM_REG_FAILURE_F;

              /* Send command to CM */
              MSG_HIGH("CM Registration Failure", 0, 0, 0);
              mcc_cm_mc_rpt(cmd_ptr);
            }
            else
            {
              ERR("No free buffers on cm_mc_rpt_free_q",0,0,0 );
            }
          }
        }
        #endif /* FEATURE_PASSPORT_MODE */

        /* -------------------------------------------------------------
        ** Inform system determination of system lost and determine the
        ** next state to be processed based on the returned SS-Action.
        ** ------------------------------------------------------------- */
        if ( mcc_continue_on_curr_sys( MEAS_BACK_ALLOWED,
               sd_ss_ind_cdma_opr_sys_lost2( sys_lost_reason, NULL ) )
           )
        {
          new_state = CDMA_IDLE;
        }

        /* Inform CM origination failed */
        if (cdma.curr_state == CDMA_ORIG) /*lint !e641 */
        {
          /* Inform CM origination failed */
          mcc_send_orig_failure(CM_CALL_ORIG_ERR_ACC_FAIL, cdma.call_id,
                                mccsa_orig_fail_layer);
          cdma.call_orig = FALSE;
        }
        break;

      #ifdef FEATURE_IS2000_REL_A
      case MCC_ACCT_BLOCKED:
        /* Report SCCT Blocked event */
        event_report(EVENT_ACCT_BLOCKED);
        /* ------------------------------------------------------------
        ** Inform System Determination of Access exit and determine the
        ** next state to be processed based on suggested SS-Action.
        ** ------------------------------------------------------------ */
        if (mcc_continue_on_curr_sys(MEAS_BACK_ALLOWED,
            sd_ss_ind_cdma_opr_acc_term(mccsa_map_sa_state_to_sa_reason(
            cdma.curr_state), SD_SS_ACC_TERM_SO_DENIED, NULL)))
        {
          new_state = CDMA_IDLE;
        }

        if (cdma.curr_state == CDMA_ORIG) /*lint !e641 */
        {

          #ifdef FEATURE_PASSPORT_MODE
          if (mcc_passport_mode_is_jcdma() && mccsa_acct_all_mobiles_blocked_for_voice)
          {
            mcc_send_orig_failure(CM_CALL_ORIG_ERR_ALL_ACCT_BLOCK, cdma.call_id,
                                  mccsa_orig_fail_layer);
          }
          else
          #endif /* FEATURE_PASSPORT_MODE */

          {
            mcc_send_orig_failure(CM_CALL_ORIG_ERR_ACCT_BLOCK, cdma.call_id,
                                  mccsa_orig_fail_layer);
          }

          cdma.call_orig = FALSE;
        }
        break;

      case MCC_ENCRYPTION_MI_FAILED:
        /* Report Encryption Failed event */
        event_report(EVENT_ENCRYPTION_FAILURE);
        /* ------------------------------------------------------------
        ** Inform System Determination of Access exit and determine the
        ** next state to be processed based on suggested SS-Action.
        ** ------------------------------------------------------------ */
        if (mcc_continue_on_curr_sys(MEAS_BACK_ALLOWED,
            sd_ss_ind_cdma_opr_acc_term(mccsa_map_sa_state_to_sa_reason(
            cdma.curr_state), SD_SS_ACC_TERM_ACC_DEN, NULL)))
        {
          new_state = CDMA_IDLE;
        }
        if (cdma.curr_state == CDMA_ORIG) /*lint !e641 */
        {
          mcc_send_orig_failure(CM_CALL_ORIG_ERR_ACC_FAIL, cdma.call_id,
                                  mccsa_orig_fail_layer);
          cdma.call_orig = FALSE;
        }
        break;
      #endif /* FEATURE_IS2000_REL_A */

      case MCC_AHO_ORIGINATION:
      case MCC_AHO_PAGE_RESPONSE:
      case MCC_AHO_WAIT_FOR_PAGE:
        if (cdma.curr_state == CDMA_ORIG) /*lint !e641 */
        {
          /* Inform CM origination failed */
          mcc_send_orig_failure(CM_CALL_ORIG_ERR_HANDOFF, cdma.call_id,
                                mccsa_orig_fail_layer);
          cdma.call_orig = FALSE;
        }
        break;

      case MCC_AHO_TC_ASSIGNMENT:
        /* Voice channel assignment, no response to CM */
        break;

      case MCC_LPM:
      case MCC_NAM_CHANGED:
      case MCC_OFFLINE_DIGITAL:
      case MCC_OFFLINE_ANALOG:
      case MCC_PREF_SYS_CHANGED:
      case MCC_POWERDOWN:
      case MCC_RESET:
      case MCC_SHUTDOWN:
        if (cdma.curr_state == CDMA_ORIG) /*lint !e641 */
        {
          /* Inform CM origination failed */
          mcc_send_orig_failure(CM_CALL_ORIG_ERR_ACC_FAIL, cdma.call_id,
                                mccsa_orig_fail_layer);
          cdma.call_orig = FALSE;
        }
        break;

      #if defined(FEATURE_IS2000_REL_C_AKA) || defined(FEATURE_IS2000_REL_A_AES)
#error code not present
      #endif /* FEATURE_IS2000_REL_C_AKA || FEATURE_IS2000_REL_A_AES */

      default:
        /* ------------------------------------------------------------
        ** There should be no way we could detect other exit conditions
        ** ------------------------------------------------------------ */
        ERR( "Invalid exit code %d", mcc_exit.exit, 0, 0 );
        if (cdma.curr_state == CDMA_ORIG) /*lint !e641 */
        {
          /* Inform CM origination failed */
          mcc_send_orig_failure(CM_CALL_ORIG_ERR_ACC_FAIL, cdma.call_id,
                                mccsa_orig_fail_layer);
          cdma.call_orig = FALSE;
        }
        break;

    }
  }
  else if (new_state == CDMA_TCI) /*lint !e641 */
  {
    if (!mcc_is_aho_ovhd_msg_okay())
    {
      /* Overhead messages are partially updated after AHO or APHO. Stores new
         SID, NID and PZID and notify its reception if a handler function is registered. */
      mccsyobj_process_epzid(cur_bs_ptr->csp.sp.sid, cur_bs_ptr->csp.sp.nid, 
        (P_REV_IS_4_OR_GREATER ? cur_bs_ptr->csp.esp.packet_zone_id : MCCSYOBJ_PZID_UNKNOWN), CDMA_SYS_ACC);

      #ifdef FEATURE_IS2000_REL_D
#error code not present
      #endif /* FEATURE_IS2000_REL_D */

      /* -----------------------------------------------------------
      ** Following is the serving system info update for CM. It is
      ** done once we acquire a service.
      ** Notice that Digital indicator is on, CM can safely poll it
      ** to know whether the data is for CDMA or for AMPS. Also
      ** NO_SVC is set correctly now.
      ** ----------------------------------------------------------- */

      mcc_send_cm_srv_sys_info( CM_SS_SID_MASK|
                                CM_SS_NID_MASK|
                                CM_SS_REG_ZONE_MASK|
                                CM_SS_PACKET_ZONE_MASK|
                                CM_SS_BS_P_REV_MASK|
                                CM_SS_P_REV_IN_USE_MASK, /*lint !e747 */
                                cur_bs_ptr->csp.sp.sid,
                                cur_bs_ptr->csp.sp.nid,
                                cur_bs_ptr->csp.sp.reg_zone,
                                (P_REV_IS_4_OR_GREATER ? cur_bs_ptr->csp.esp.packet_zone_id : MCCSYOBJ_PZID_UNKNOWN)
                              );
    }

    #ifdef FEATURE_GPSONE_NEW_L2_ACK
#error code not present
    #endif /* FEATURE_GPSONE_NEW_L2_ACK */

  }
  else
  {

    #ifdef FEATURE_IS2000_REL_A_SVD
    /*
    ** We are about to exit CDMA, make sure all the SR_IDs are properly
    ** released.
    */
    mccsrid_release();

    /*
    ** Catch all to prevent possible sr_id leak
    */
    mccsrid_free_all_owned_srids(SRID_OWNER_MC);
    #endif /* FEATURE_IS2000_REL_A_SVD */

  }

  /* MS going out of Access Handoff Region. Reset the cdma.ho_state */
  cdma.ho_state = CDMA_IDLE_HO;

  if ( ( cdma.entry_ptr->entry == MCC_PSEUDO_IDLE ) &&
       ( new_state == CDMA_IDLE ) )
  {
    /* Done with 1x access. Send request to Transmit task to go to CDMA mode
       so as to get ready for future UMTS to 1x Handover request. */
    mcc_tx_buf.cdma.hdr.command = TXC_CDMA_F;
    mcc_txc_cmd( &mcc_tx_buf );

  }

  return (new_state);

} /* cdma_sa */

#ifdef FEATURE_FACTORY_TESTMODE
/*===========================================================================

FUNCTION mccsa_set_access_in_progress

DESCRIPTION
  This function is only used by PLT and it's sole function is to set the
  access_in_progress member of layer2 data structure which is local to this
  file.

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/

void mccsa_set_access_in_progress
(
  void
)
{
  mccsa_layer2.access_in_progress = TRUE;
}
#endif // FEATURE_FACTORY_TESTMODE

#ifdef FEATURE_IS2000_REL_A
/*===========================================================================

FUNCTION MCCSA_IS_CALL_BLOCKED_BY_ACCT

DESCRIPTION
  This function checks if an origination with the given SO neds to be
  blocked due to ACCT ( Access Control Based on Call Type )

DEPENDENCIES
  None.

RETURN VALUE
  TRUE if call needs to be blocked, FALSE otherwise.

SIDE EFFECTS
  Origination will be aborted and MC can enter system determinationm as a
  result of this.

===========================================================================*/
boolean mccsa_is_call_blocked_by_acct
(
  word so,  /* SO chosen for the outgoing call */
  boolean is_emergency_call /* Indicates if this is an emergency call */
)
{
  uint8 so_grp_num,i;

  #ifdef FEATURE_PASSPORT_MODE
  mccsa_acct_all_mobiles_blocked_for_voice = FALSE;
  #endif

  if (cdma.accolc > 9)
  {
    return FALSE;
  }

  #ifdef FEATURE_IS2000_REL_A_CC
  if (OVHD_CHAN_IS_BCCH)
  {
    caii_en_ac_parm_type *eapm_ptr = &cur_bs_ptr->eapm;
    if (!eapm_ptr->acct_incl)
    {
      return FALSE;
    }
    if (!eapm_ptr->acct_incl_emg && is_emergency_call)
    {
      return FALSE;
    }
    if (eapm_ptr->acct_so_grp_incl)
    {
      so_grp_num = snm_get_so_grp_num( so );
      for (i=0; i<eapm_ptr->num_acct_so_grp+1; i++)
      {
        if (eapm_ptr->acct_so_grp_info[i].acct_so_grp == so_grp_num)
        {
          #ifdef FEATURE_PASSPORT_MODE                
          if (mcc_passport_mode_is_jcdma() &&
              (!eapm_ptr->acct_aoc_bitmap_incl || 
               (eapm_ptr->acct_so_grp_info[i].acct_aoc_bitmap2 == 0x1f)) &&
              (so_grp_num == CAI_SO_GROUP_VOICE))
          {
            mccsa_acct_all_mobiles_blocked_for_voice = TRUE;
            return TRUE;
          }
          else
          #endif /* FEATURE_PASSPORT_MODE */
          {
            if (!eapm_ptr->acct_aoc_bitmap_incl)
            {
              //This blocking applies to all mobiles
              return TRUE;
            }
            else
            {
              /* acct_aoc_bitmask2 is a 5 bit value embedded ina byte,
                 with LSB corresponding accolc_8_9, and 5th  LSB
                 corresponding to accolc_0_1 and the 3 MSBs are unused,
                 so we are trying to see if the bit corresponding to the MS's
                 ACCOLC is set by doing the following */
              if (eapm_ptr->acct_so_grp_info[i].acct_aoc_bitmap2 &
                  (0x1 << (4-(cdma.accolc>>1)) ) )
              {
                /* The bit correspodning to our ACCOLC value is set, so block */
                return TRUE;
              }
            }
          }
        }
      }
    }
    if (eapm_ptr->acct_so_incl)
    {
      for (i=0; i<eapm_ptr->num_acct_so+1; i++)
      {
        if (eapm_ptr->acct_so_info[i].acct_so == so)
        {
          #ifdef FEATURE_PASSPORT_MODE
          if (mcc_passport_mode_is_jcdma() &&
                (!eapm_ptr->acct_aoc_bitmap_incl || 
               (eapm_ptr->acct_so_info[i].acct_aoc_bitmap1 == 0x1f)) &&
              CAI_IS_SO_VOICE(so))
          {
            mccsa_acct_all_mobiles_blocked_for_voice = TRUE;
            return TRUE;
          }
          else
          #endif /* FEATURE_PASSPORT_MODE */

          if (!eapm_ptr->acct_aoc_bitmap_incl)
          {
            //This blocking applies to all mobiles
            return TRUE;
          }
          else
          {
            /* acct_aoc_bitmask2 is a 5 bit value embedded ina byte,
            with LSB corresponding accolc_8_9, and 5th  LSB
            corresponding to accolc_0_1 and the 3 MSBs are unused,
            so we are trying to see if the bit corresponding to the MS's
            ACCOLC is set by doing the following */
            if (eapm_ptr->acct_so_info[i].acct_aoc_bitmap1 &
                 (0x1 << (4-(cdma.accolc>>1)) ) )
            {
              /* The bit correspodning to our ACCOLC value is set, so block */
               return TRUE;
            }
          }
        }
      }
    }
  }
  else
  #endif /* FEATURE_IS2000_REL_A_CC */

  {
    caii_accparm_type * apm_ptr = &cur_bs_ptr->ap;
    if (!apm_ptr->acct_incl)
    {
      return FALSE;
    }

    if (!apm_ptr->acct_incl_emg && is_emergency_call)
    {
      return FALSE;
    }
    if (apm_ptr->acct_so_grp_incl)
    {
      so_grp_num = snm_get_so_grp_num( so );
      for (i=0; i<apm_ptr->num_acct_so_grp+1; i++)
      {
        if (apm_ptr->acct_so_grp_info[i].acct_so_grp == so_grp_num)
        {
          #ifdef FEATURE_PASSPORT_MODE
          if (mcc_passport_mode_is_jcdma() &&
              (!apm_ptr->acct_aoc_bitmap_incl || 
               (apm_ptr->acct_so_grp_info[i].acct_aoc_bitmap2 == 0x1f)) &&
              (so_grp_num == CAI_SO_GROUP_VOICE))
          {
            mccsa_acct_all_mobiles_blocked_for_voice = TRUE;
            return TRUE;
          }
          else
          #endif /* FEATURE_PASSPORT_MODE */

          if (!apm_ptr->acct_aoc_bitmap_incl)
          {
            //This blocking applies to all mobiles
            return TRUE;
          }
          else
          {
            /* acct_aoc_bitmap2 is a 5 bit value embedded ina byte,
               with LSB corresponding accolc_8_9, and 5th  LSB
               corresponding to accolc_0_1 and the 3 MSBs are unused,
               so we are trying to see if the bit corresponding to the MS's
               ACCOLC is set by doing the following */
            if (apm_ptr->acct_so_grp_info[i].acct_aoc_bitmap2 &
                (0x1 << (4-(cdma.accolc>>1)) ))
            {
              /* The bit correspodning to our ACCOLC value is set, so block */
              return TRUE;
            }
          }
        }
      }
    }
    if (apm_ptr->acct_so_incl)
    {
      for (i=0; i<apm_ptr->num_acct_so+1; i++)
      {
        if (apm_ptr->acct_so_info[i].acct_so == so)
        {

          #ifdef FEATURE_PASSPORT_MODE
          if (mcc_passport_mode_is_jcdma() &&
              (!apm_ptr->acct_aoc_bitmap_incl || 
               (apm_ptr->acct_so_info[i].acct_aoc_bitmap1 == 0x1f)) &&
              CAI_IS_SO_VOICE(so))
          {
            mccsa_acct_all_mobiles_blocked_for_voice = TRUE;
            return TRUE;
          }
          else
          #endif /* FEATURE_PASSPORT_MODE */

          if (!apm_ptr->acct_aoc_bitmap_incl)
          {
            //This blocking applies to all mobiles
            return TRUE;
          }
          else
          {
            /* acct_aoc_bitmask2 is a 5 bit value embedded ina byte,
            with LSB corresponding accolc_8_9, and 5th  LSB
            corresponding to accolc_0_1 and the 3 MSBs are unused,
            so we are trying to see if the bit corresponding to the MS's
            ACCOLC is set by doing the following */
            if (apm_ptr->acct_so_info[i].acct_aoc_bitmap1 &
                (0x1 << (4-(cdma.accolc>>1)) ) )
            {
              /* The bit correspodning to our ACCOLC value is set, so block */
              return TRUE;
            }
          }
        }
      }
    }
  }
  /* If we reach here it means we did not satisfy any blocking conditions,
     so don't block the call */
  return FALSE;
}

/*===========================================================================

FUNCTION MCCSA_IS_DBM_BLOCKED_BY_ACCT

DESCRIPTION
  This function checks if a Data Burst Message ( DBM)  with the  given
  burst type  needs to be blocked due to ACCT ( Access Control Based
  on Call Type )

DEPENDENCIES
  None.

RETURN VALUE
  TRUE if DBM needs to be blocked, FALSE otherwise.

SIDE EFFECTS
  DBM will be aborted and MC can enter system determinationm as a
  result of this.

===========================================================================*/
boolean mccsa_is_dbm_blocked_by_acct
(
  byte burst_type /* Burst Type of the outgoing DBM */
)
{
  switch (burst_type)
  {
    case CAI_SHORT_MSG_SERVICES:
      if (mccsa_is_call_blocked_by_acct(CAI_SO_SMS,0) &&
          mccsa_is_call_blocked_by_acct(CAI_SO_RS2_SMS,0) )
      {
        return TRUE;
      }
      return FALSE;

    case CAI_POSITION_DET:
      if (mccsa_is_call_blocked_by_acct(CAI_SO_RS1_PD,0) &&
          mccsa_is_call_blocked_by_acct(CAI_SO_RS2_PD,0) )
      {
        return TRUE;
      }
      return FALSE;

    default:
      return FALSE;
  }
} /* mccsa_is_dbm_blocked_by_acct */
#endif /* FEATURE_IS2000_REL_A */

#ifdef FEATURE_IS2000_REL_A_CC
/*===========================================================================

FUNCTION MCCSA_SELECT_RATE

DESCRIPTION

  Selects the preferred RATE_WORD based on rates supported by the BS, duration
  supported by BS, gain limit, and interference.

DEPENDENCIES
  None

RETURN VALUE
  NOT_ABORT if rate was found that satisfied the following criteria:
    BS can support the rate.
    BS supports the duration of the message, which is function of rate and message size
  An error code otherwise

SIDE EFFECTS
  None

===========================================================================*/
access_result_type mccsa_select_rate
(
  int size,                      /* Length of message to be sent */
  byte rates_supported,          /* Supported rate words */
  caii_access_mode_rec_type *mode_selection,  /* Mode selection for currently considered mode */
  int16 rlgain_common_pilot,     /* Gain adjustment of EACH or RCCCH relative to reverse pilot */
  txc_each_rate_type *rate_word, /* Rate index selected */
  word *allowed_max_reach_size    /* Allowed max reach message size */
)
{
  const int NUM_RATE_WORDS = 6;
  // Preferred rates for PDUs of length 0 to 172 bits.
  const txc_each_rate_type PREFERRED_RATES_0_172[] =
  {
    TXC_EACH_RATE_38400_5_V,
    TXC_EACH_RATE_19200_10_V,
    TXC_EACH_RATE_38400_10_V,
    TXC_EACH_RATE_9600_20_V,
    TXC_EACH_RATE_19200_20_V,
    TXC_EACH_RATE_38400_20_V,
  };

  // Preferred rates for PDUs of length 173 to 360 bits.
  const txc_each_rate_type PREFERRED_RATES_173_360[] =
  {
    TXC_EACH_RATE_38400_10_V,
    TXC_EACH_RATE_19200_20_V,
    TXC_EACH_RATE_38400_20_V,
    TXC_EACH_RATE_38400_5_V,
    TXC_EACH_RATE_19200_10_V,
    TXC_EACH_RATE_9600_20_V,
  };

  // Preferred rates for PDUs of length >= 361 bits.
  const txc_each_rate_type PREFERRED_RATES_361_INFINITE[] =
  {
    TXC_EACH_RATE_38400_20_V,
    TXC_EACH_RATE_38400_10_V,
    TXC_EACH_RATE_19200_20_V,
    TXC_EACH_RATE_38400_5_V,
    TXC_EACH_RATE_19200_10_V,
    TXC_EACH_RATE_9600_20_V,
  };

  // The number of information bits that corresponds to r according to Table
  // 2.1.3.4.2-1 in the Physical Layer standard.
  word ACC_NUM_BITS[] =
  {
    172, // TXC_EACH_RATE_9600_20_V
    360, // TXC_EACH_RATE_19200_20_V
    172, // TXC_EACH_RATE_19200_10_V
    744, // TXC_EACH_RATE_38400_20_V
    360, // TXC_EACH_RATE_38400_10_V
    172  // TXC_EACH_RATE_38400_5_V
  };

  // The frame duration that corresponds to r in units of 5ms.
  word ACC_FRAME_DURATION[] =
  {
    4, // TXC_EACH_RATE_9600_20_V
    4, // TXC_EACH_RATE_19200_20_V
    2, // TXC_EACH_RATE_19200_10_V
    4, // TXC_EACH_RATE_38400_20_V
    2, // TXC_EACH_RATE_38400_10_V
    1  // TXC_EACH_RATE_38400_5_V
  };

  const txc_each_rate_type *preferred_rates;
  txc_each_rate_type candidate_rates[6];
  int preferred_rate_word;
  int candidate_index=0;
  int num_candidates=0;
  int last_gain;
  txc_each_rate_type last_rate_word = TXC_EACH_RATE_9600_20_V;
  dword duration;
  boolean at_least_one_rate_exceeds_max = FALSE;
  word max_reach_size = 0;

  // Select preferred rate words by PDU size.

  if (size <= 172)
  {
    preferred_rates = PREFERRED_RATES_0_172;
  }
  else if (size >= 361)
  {
    preferred_rates = PREFERRED_RATES_361_INFINITE;
  }
  else // 173 <= size <= 360
  {
    preferred_rates = PREFERRED_RATES_173_360;
  }

  // Construct a list of candidate RATE_WORDs in order of preference.
  for (preferred_rate_word = 0; preferred_rate_word < NUM_RATE_WORDS; preferred_rate_word++)
  {
    *rate_word = preferred_rates[preferred_rate_word];

    // Require that BS and MS can support the rate.
    if ( (rates_supported & (0x80 >> *rate_word) ) /*lint !e641 */
         && mccap_reach_rate_supported(*rate_word)
       )
    {

      // This is the true computation of duration, and is an enhancement over
      // the formula given in the standard.
      // duration = ceil(size/ACC_NUM_BITS) * ACC_FRAME_DURATION;

      duration = ( ((word)size)/ACC_NUM_BITS[*rate_word]
                   + ( (((word)size) % ACC_NUM_BITS[*rate_word]) ? 1 : 0 ) // This is the ceiling function
                 ) * ACC_FRAME_DURATION[*rate_word];

      // Require that BS allows the duration the message would take with this rate
      if ( (duration >= mode_selection->access_mode_min_duration)
           && (duration <= mode_selection->access_mode_max_duration)
         )
      {
        // Add to the candidate list.
        candidate_rates[candidate_index++] = *rate_word;
      }
      else if (duration > mode_selection->access_mode_max_duration)
      {
        at_least_one_rate_exceeds_max = TRUE;

        max_reach_size = (word) (ACC_NUM_BITS[*rate_word] * mode_selection->access_mode_max_duration
          / ACC_FRAME_DURATION[*rate_word]);
        *allowed_max_reach_size = ( ( *allowed_max_reach_size < max_reach_size )
          ? max_reach_size : *allowed_max_reach_size);
      }
    }
  }

  num_candidates = candidate_index;

  if (num_candidates == 0)
  {
    if ( at_least_one_rate_exceeds_max )
    {
      return ABORT_MSG_TOO_LONG;
    }
    else
    {
      return ABORT_NO_ACCEPTED_RATE_WORD;
    }
  }

  // Now search candidates for first one that does not exceed the transmit gain limit

  candidate_index=0;
  while (candidate_index<num_candidates)
  {
    *rate_word = candidate_rates[candidate_index]; //lint !e644 candidate_rates may not have been initialized

    // If the current rate_word satisifes the gain limit,
    // or if it is the rate_word with the smallest gain
    // requirement, return the rate_word selected.
    if (rpc_reach_rate_word_satisfies_gain_limit(*rate_word, rlgain_common_pilot))
    {
      // Compute the maximum message length for future probes.
      // This is equal to ACC_NUM_BITS * floor( ACC_MAX_DURATION / ACC_FRAME_DURATION )

      reach_max_message_length = (word) (ACC_NUM_BITS[*rate_word] * mode_selection->access_mode_max_duration
        / ACC_FRAME_DURATION[*rate_word]);

      return NOT_ABORT;
    }

    // Keep an indication of the power requirement of this
    // failed rate around for future rate_word checks.  This
    // will allow us to avoid the costly calculation of the
    // power required.
    last_gain = rlgc_get_nominal_rcch_attribute_gain(*rate_word); /*lint !e641 */
    last_rate_word = *rate_word;

    // Increment loop index for next iteration
    candidate_index++;

    // Optimization to skip indicies that can quickly be eliminated because
    // their power requirement is larger than another rate_word that has already
    // been excluded because of its power requirement.
    while
    (
      candidate_index<num_candidates
      && rlgc_get_nominal_rcch_attribute_gain(candidate_rates[candidate_index]) > last_gain /*lint !e641 */
    )
    {
      candidate_index++;
    }
  } // end of while loop over candidates

  // There are no entries that satisfy the transmit gain limit. Select the rate
  // with the smallest power required.
  *rate_word = last_rate_word;  //lint !e771 last_rate_word conceivably not initialized

  // Compute the maximum message length for future probes.
  // This is equal to ACC_NUM_BITS * floor( ACC_MAX_DURATION / ACC_FRAME_DURATION )

  reach_max_message_length = (word) (ACC_NUM_BITS[*rate_word] * mode_selection->access_mode_max_duration
    / ACC_FRAME_DURATION[*rate_word]);

  return NOT_ABORT;

} /* mccsa_select_rate */

/*===========================================================================

FUNCTION MCCSA_SELECT_MODE_RATE

DESCRIPTION
  This function selects an access mode and rate based on the message size.
  The algorithm is taken directly from MAC standard, section
  2.2.1.1.2.2.4.3.

DEPENDENCIES
  None

RETURN VALUE
  NOT_ABORT if mode and rate_word are valid.
  An error code otherwise.

SIDE EFFECTS
  The parameters mode and rate_word are modified by this function.

===========================================================================*/

access_result_type mccsa_select_mode_rate
(
  int size,                        /* Size of message to be transmitted */
  byte each_ba_rates_supported,    /* Supported rate words for the Basic Access mode on the EACH */
  byte rccch_rates_supported,      /* supported rate words on the RCCCH */
  byte num_mode_selection_entries, /* Numbers of entires of the Mode Selection table */
  caii_access_mode_rec_type *mode_selection, /* mode selection table */
  int16 rlgain_common_pilot,       /* Gain adjustment of EACH or RCCCH relative to reverse pilot */
  int *mode,                       /* Mode selected  */
  txc_each_rate_type *rate_word    /* Rate index selected */
)
{
  int mode_index;
  access_result_type result;
  boolean at_least_one_rate_exceeds_max = FALSE;

  word allowed_max_reach_size;
    /* While selecting the mode/rate to use on REACH, this variable keeps 
       track of the maximum number of bits that can be transmitten successfully
     */


  /* Unused parameter */
  (void) rccch_rates_supported;

  /* Initialize the maximum number of bits that can be transmitted 
     successfully to 0.
  */
  allowed_max_reach_size = 0;

  for ( mode_index=0; mode_index < num_mode_selection_entries+1; mode_index++ )
  {
    *mode = mode_selection[mode_index].access_mode;
    if ( *mode == CAI_EA_MODE_BA )
    {
      result = mccsa_select_rate ( size, each_ba_rates_supported,
        &(mode_selection[mode_index]), rlgain_common_pilot,
        rate_word, // This is modified by select_rate
        &allowed_max_reach_size );

      if ( result == NOT_ABORT )
      {
        return NOT_ABORT;
      }
      else if ( result == ABORT_MSG_TOO_LONG )
      {
        at_least_one_rate_exceeds_max = TRUE;
      }
    }
    else if ( *mode == CAI_EA_MODE_RA )
    {
      /* Reservation access mode is not supported yet
       * If there are BA mode selections after this, they will
       * be considered.
       */
    }
    else
    {
      /* mode must be one of CAI_EA_MODE_BA or CAI_EA_MODE_RA */
      ERR( "Unknown R-EACH mode", 0, 0, 0 );
      return ABORT_NO_ACCEPTED_RATE_WORD;
    }
  }

  /* No modes selections were suitable */
  if ( at_least_one_rate_exceeds_max )
  {
    ERR( "No R-EACH rate word selected - message too long", 0, 0, 0 );

    if (size > allowed_max_reach_size)
    {
      mcc_burst_overflow_size = (uint16) ((size - allowed_max_reach_size) / 8);
    }
    else 
    {
      ERR( "size %d, allowed_max_reach_size %d", size, allowed_max_reach_size, 0 );
    }

    return ABORT_MSG_TOO_LONG;
  }
  else
  {
    ERR( "No R-EACH rate word selected", 0, 0, 0 );
    return ABORT_NO_ACCEPTED_RATE_WORD;
  }

} /* mccsa_select_mode_rate */

/*===========================================================================

FUNCTION MCCSA_SEND_REACH_ACCESS_CMD

DESCRIPTION
  This function sends an REACH Access command to the Transmit task.

DEPENDENCIES
  None

RETURN VALUE
  TRUE if the access attempt was aborted, else FALSE.

SIDE EFFECTS
  None

===========================================================================*/

access_result_type mccsa_send_reach_access_cmd
(
  caii_tx_msg_type *tx_msg_ptr,  /* External format message pointer */
  caii_plt_rpt_type *pilot_rpt_ptr, /*IS2000 addition, ptr to pilot rpt fields of mesg */
  boolean          request,     /* Flag for request or response */
  word             msg_len,     /* Length of message */
  word             curr_state   /* Current substate */
)
{
  caii_en_ac_parm_type *eap_ptr;    /* Pointer to current Enhanced Access Params Msg */
  int mode;                         /* Index of mode selected  */
  txc_each_rate_type rate_word;   /* Rate index selected */
  access_result_type access_aborted;/* Flag if access attempt aborted */
  word status;                      /* Message translation status */
  word max_message_body;
  word total_msg_length;
  byte access_mode;           /* Access mode used for the EACH */

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  access_aborted = NOT_ABORT;

  mccsa_access_attempt_is_request = request;

  /* Send command to transmit task to send access message */
  mcc_tx_buf.hdr.command = TXC_REACH_F;
  mcc_tx_buf.hdr.cmd_hdr.done_q_ptr = NULL;
  mcc_tx_buf.hdr.cmd_hdr.task_ptr = NULL;

  /* Signal when access attempt is finished */
  mcc_tx_buf.reach.tcb_ptr = &mc_tcb;
  mcc_tx_buf.reach.sigs = MCC_TXC_DONE_SIG;

  mcc_tx_buf.reach.len = msg_len;

  /* If we have valid access parameters give them to tx in the command */
  if
  (
    (eap_ptr = (caii_en_ac_parm_type *) mcc_get_msg( CAI_EN_AC_PARM_MSG ))
    != NULL
  )
  {
    /* Bookeep Msg length excluding pilot report info */
    mccsa_pilot_rpt_ovhd_rec.msg_len = msg_len;


    max_message_body = mccsa_calculate_maximum_reach_message();


    /* Compute the number of pilots that can be contained in Access
       Channel Message without exceeding the maximum capsule size */
    mccsa_pilot_rpt_ovhd_rec.num_pilots_allowed =
      mccsa_compute_ac_msg_num_plt_rpt(max_message_body);

    /* ----------------------------------------------------------------
    ** First access probe : clear First pilot, Prev pilot, access probe
    ** handoff rebuild flag and access attempt list.
    ** ---------------------------------------------------------------- */
    mccsa_pilot_rpt_ovhd_rec.first_probe = TRUE;
    mccsa_pilot_rpt_ovhd_rec.first_pilot = NO_PREV_PILOT;
    mccsa_pilot_rpt_ovhd_rec.prev_pilot = NO_PREV_PILOT;
    mccsa_pilot_rpt_ovhd_rec.apho_rebuild = FALSE;
    mccsa_pilot_rpt_ovhd_rec.num_pilots_attempted = 0;

    /* Wait for searcher pilot strength measurement report and
       Update ACCESS_HO_LIST and OTHER_REPORTED_LIST */
    access_aborted = mccsa_wait_for_srch_pilots_meas_rpt();

    /* Succeed in building or updating ACCESS_HO_LIST */
    if ( access_aborted == NOT_ABORT )
    {
      /* Append pilot report information to Access Channel Message */
      mccsa_setup_pilot_rpt(pilot_rpt_ptr);
    }

    if (access_aborted == NOT_ABORT)
    {
      if ((status = caix_int_to_ext( &msg_len, CAIX_REACH,  /*lint !e641 */
                                     tx_msg_ptr, &mcc_tx_ext_msg,
                                     CAIX_PARSE_MSG
                                   )) != CAIX_DONE_S)
      {
        ERR( "Internal translation error %d", status, 0, 0 );
        if (status == CAIX_INV_LEN_S)
        {
          access_aborted = ABORT_MSG_TOO_LONG;  /* Cancel access attempt */
        }
        else
        {
          access_aborted = ABORT_XLATE_ERROR;  /* Cancel access attempt */
        }
      }
      else
      {
        mcc_tx_buf.reach.len = msg_len;

        // Calculate the total message length by adding the CRC length and
        // the message length field.  If the LAC PDU size is greater than
        // 978, then the message length field is two bytes.  See LAC 2.1.1.5.1.2.
        if (msg_len <= 978)
        {
          total_msg_length = msg_len + CAI_AC_CRC_SIZE + CAI_AC_LGTH_SIZE;
        }
        else
        {
          total_msg_length = msg_len + CAI_AC_CRC_SIZE + 2 * CAI_AC_LGTH_SIZE;
        }

        access_aborted = mccsa_select_mode_rate ( total_msg_length,
          eap_ptr->each_ba_rates_supported, eap_ptr->rccch_rates_supported,
          eap_ptr->num_mode_selection_entries, eap_ptr->mode_selection,
          eap_ptr->rlgain_common_pilot, &mode, &rate_word );

        mcc_tx_buf.reach.msg_ptr = mcc_tx_ext_msg.ac.body;

        if (access_aborted == NOT_ABORT)
        {
          access_mode = eap_ptr->mode_selection[mode].access_mode;
          access_aborted = mccsa_populate_reach_access_cmd(eap_ptr, curr_state,
            access_mode, rate_word);
        }
      }
    }
  }
  else
  {
    ERR( "Internal error --> No enhanced access parameters", 0, 0, 0 );
    access_aborted = ABORT_NO_EA_PARA;  /* Cancel access attempt */
  }

  if ( access_aborted == NOT_ABORT )
  {
    if (!mccsa_layer2.access_in_progress)
    {
      /* Proceed with access attempt */
      (void) rex_clr_sigs( &mc_tcb, MCC_TXC_DONE_SIG );

      /* Clear Access Probe Signal */
      (void) rex_clr_sigs( &mc_tcb, MCC_TXC_PROBE_SIG );

      /* Set the cdma.ho_state to indicate if the Access Probe */
      /* Handoff is allowed.                                   */
      /* First Initialize the Number of Access Probe Handoffs done */
      mccsa_num_apho_done =  0;

      /* Initialize the variable which controls the Handoff in the */
      /* boundary condition of APHO & AHO.                         */
      mccsa_init_aho = FALSE;

      /* And, finally initialise the APHO substate */
      mccsa_apho_substate = APHO_NOT_IN_PROGRESS;

      /* Clear Access Probe HO OK Signal */
      (void) rex_clr_sigs( &mc_tcb, MCC_TXC_APHO_OK_SIG );

      if (mccsa_is_apho_allowed())
      {
        MSG_MED("APHO allowed", 0, 0, 0);
        cdma.ho_state = CDMA_APHO;
        /* Also, note that the function mccsa_is_apho_allowed()    */
        /* increments the count of access Probe Handoffs performed */
        /* which is not correct in this state. Reset it, therefore */
        mccsa_num_apho_done = 0;
      }
      else
      {
        MSG_MED("APHO not allowed",0,0,0);
        cdma.ho_state = CDMA_IDLE_HO;
        /* Fake that the Max Number of APHO have been performed */
        /* so that each time we do not end up performing lots of*/
        /* checks to figure out if APHO is allowed or not.      */
        mccsa_num_apho_done = cur_bs_ptr->csp.esp.max_num_probe_ho + 1;
      }

      MSG_HIGH("Sending REACH access attempt on PN=%d", mcc_tx_buf.reach.pilot_pn, 0, 0);
      mcc_txc_cmd( &mcc_tx_buf );

      /* Indicate that we are starting a new access attempt */
      mccsa_layer2.access_in_progress = TRUE;

      if (cdma.ho_state == CDMA_AHO)
      {
        /* Reset The HO state, this will be set when L2 Ack is received */
        cdma.ho_state = CDMA_IDLE_HO;
      }

      /* ------------------------------------------------------------
      ** Clear the state timer.  We set the timer when we complete
      ** an access attempt in Page Response or Origination Substates,
      ** in case a delayed response never comes.
      ** ------------------------------------------------------------ */
      (void) rex_clr_timer( &mcc_state_timer );
      (void) rex_clr_sigs( &mc_tcb,  MCC_STATE_TIMER_SIG );
    }
    else
    {
      ERR_FATAL( "Two access attempts at once.", 0, 0, 0);
    }
  }

  return (access_aborted);

} /* mccsa_send_reach_access_cmd */
#endif /* FEATURE_IS2000_REL_A_CC */

/*===========================================================================

FUNCTION MCCSA_SEND_ACCESS_CMD

DESCRIPTION
  This function sends an Access command to the Transmit task by calling
  either mccsa_send_ach_access_cmd or mccsa_send_reach_access_cmd.

DEPENDENCIES
  None

RETURN VALUE
  TRUE if the access attempt was aborted, else FALSE.

SIDE EFFECTS
  None

===========================================================================*/

static access_result_type mccsa_send_access_cmd
(
  caii_tx_msg_type *tx_msg_ptr,  /* External format message pointer */
  caii_plt_rpt_type *pilot_rpt_ptr, /*IS2000 addition, ptr to pilot rpt fields of mesg */
  boolean          request,     /* Flag indicates whether the access is a request or a response to BS.*/
  word             msg_len,     /* Length of message */
  word             curr_state   /* Current substate */
)
{
  #ifdef FEATURE_IS2000_REL_A
  if (P_REV_IS_7_OR_GREATER)
  {
    /* Check if this SO needs to be blocked due to ACCT (Access Control
    based on Call type */
    if ( (tx_msg_ptr->gen.msg_type == CAI_ORIG_MSG ) &&
        mccsa_is_call_blocked_by_acct(cdma.so_req, cdma.is_emergency))
    {
      MSG_HIGH("SO %d blocked due to ACCT", cdma.so_req,0,0);
      return (ABORT_ACCT_BLOCK);
    }
    if ( (tx_msg_ptr->gen.msg_type == CAI_AC_BURST_MSG ) &&
        mccsa_is_dbm_blocked_by_acct(tx_msg_ptr->ac_burst.burst_type))
    {
      MSG_HIGH("Burst Type %d blocked due to ACCT",
               tx_msg_ptr->ac_burst.burst_type,0,0);
      return (ABORT_ACCT_BLOCK);
    }
  }
  #endif

  #ifdef FEATURE_IS2000_REL_A_CC
  if (OVHD_CHAN_IS_BCCH)
  {
    return mccsa_send_reach_access_cmd (
      tx_msg_ptr, pilot_rpt_ptr, request, msg_len, curr_state );
  }
  else
  #endif // FEATURE_IS2000_REL_A_CC

  {
    return mccsa_send_ach_access_cmd (
      tx_msg_ptr, pilot_rpt_ptr, request, msg_len, curr_state );
  }

} /* mccsa_send_access_cmd */

#ifdef FEATURE_SO_8026
#error code not present
#endif /* FEATURE_SO_8026 */
/*lint +e818 */
