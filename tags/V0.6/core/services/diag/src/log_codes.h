#ifndef LOG_CODES_H
#define LOG_CODES_H

/*===========================================================================

                         Log Code Definitions

General Description
  This file contains log code definitions and is shared with the tools.

Copyright (c) 1991-2006 by QUALCOMM, Inc.  All Rights Reserved.
===========================================================================*/

/* DO NOT MODIFY THIS FILE WITHOUT PRIOR APPROVAL
**
** Log codes, by design, are a tightly controlled set of values.  
** Developers may not create log codes at will.
**
** Request new logs using the following process:
**
** 1. Send email to asw.diag.request requesting log code assignments.
** 2. Identify the log needed by name.
** 3. Provide a brief description for the log.
**
*/

/*===========================================================================

                             Edit History

$Header: //depot/asic/msmshared/services/diag/log_codes.h#151 $
   
when       who     what, where, why
--------   ---     ----------------------------------------------------------
11/02/01   sfh     Featurize common NAS log codes for UMTS.
10/30/01   sfh     Added log code for LOG_GPS_FATPATH_INFO_C.
10/24/01   sfh     Added updates for UMTS equipment ID and log codes.
06/27/01   lad     Added multiple equipment ID support.
05/22/01   sfh     Reserved log codes 158 - 168.
05/21/01   sfh     Keep confusing XXX_BASE_C names for backwards compatibility.
05/16/01   sfh     Reserved log code 155.
05/08/01   sfh     Reserved log codes 150 - 154.
04/06/01   lad     Added definitions of base IDs (such as LOG_WCDMA_BASE_C).
                   This is currently using temporary ID values in the 0x1000 
                   range.
02/23/01   lad     Created file from DMSS log.h.  Log codes only

===========================================================================*/

#include "comdef.h"

/* -------------------------------------------------------------------------
 * Data Declarations
 * ------------------------------------------------------------------------- */

/* -------------------------------------------------------------------------
 * Log equipment IDs.
 * The most significant 4 bits of the 16 bit log code is the equipment ID.
 * Orignally, the mobile was to have an ID, and base stations and other 
 * IDs.  As QCT technology diversifies, new equipment IDs are assigned to new
 * technology areas.  0x2000 and 0x3000 are reserved for legacy reasons, so 
 * the first 
 * addition starts at 0x4000.
 * ------------------------------------------------------------------------- */

#define LOG_1X_BASE_C    ((uint16) 0x1000)
#define LOG_WCDMA_BASE_C ((uint16) 0x4000)
#define LOG_GSM_BASE_C   ((uint16) 0x5000)
#define LOG_LBS_BASE_C   ((uint16) 0x6000)
#define LOG_UMTS_BASE_C  ((uint16) 0x7000)
#define LOG_TDMA_BASE_C  ((uint16) 0x8000)
#define LOG_DTV_BASE_C   ((uint16) 0xA000)
#define LOG_APPS_BASE_C  ((uint16) 0xB000)
#define LOG_DSP_BASE_C   ((uint16) 0xC000)

#define LOG_TOOLS_BASE_C ((uint16) 0xF000)

/* LOG_BASE_C is what was used before expanding the use of the equipment ID. 
 * TODO: Once all targets are using the "core" diag system, this should be
 * ommitted. */
#define LOG_BASE_C LOG_1X_BASE_C 

/* -------------------------------------------------------------------------
 * Log Codes
 *   These codes identify the kind of information contained in a log entry. 
 *   They are used in conjunction with the 'code' field of the log entry 
 *   header.  The data types associated with each code are defined below.
 * ------------------------------------------------------------------------- */

/* The upper 4 bits of the 16 bit log entry code specify which type
 * of equipment created the log entry. */

/* 0 Mobile Station temporal analyzer entry */
#define LOG_TA_C                                       (0x0 + LOG_1X_BASE_C)

/* 1 AGC values and closed loop power control entry */
#define LOG_AGC_PCTL_C                                 (0x1 + LOG_1X_BASE_C)

/* 2 Forward link frame rates and types entry */
#define LOG_F_MUX1_C                                   (0x2 + LOG_1X_BASE_C)

/* 3 Reverse link frame rates and types entry */
#define LOG_R_MUX1_C                                   (0x3 + LOG_1X_BASE_C)

/* 4 Access channel message entry */
#define LOG_AC_MSG_C                                   (0x4 + LOG_1X_BASE_C)

/* 5 Reverse link traffic channel message entry */
#define LOG_R_TC_MSG_C                                 (0x5 + LOG_1X_BASE_C)
                               
/* 6 Sync channel message entry */
#define LOG_SC_MSG_C                                   (0x6 + LOG_1X_BASE_C)

/* 7 Paging channel message entry */ 
#define LOG_PC_MSG_C                                   (0x7 + LOG_1X_BASE_C)

/* 8 Forward link traffic channel message entry */
#define LOG_F_TC_MSG_C                                 (0x8 + LOG_1X_BASE_C)

/* 9 Forward link vocoder packet entry */
#define LOG_VOC_FOR_C                                  (0x9 + LOG_1X_BASE_C)

/* 10 Reverse link vocoder packet entry */
#define LOG_VOC_REV_C                                  (0xA + LOG_1X_BASE_C)

/* 11 Temporal analyzer finger info only */
#define LOG_FING_C                                     (0xB + LOG_1X_BASE_C)

/* 12 Searcher pathlog info (Reused old SRCH logtype) */
#define LOG_SRCH_C                                     (0xC + LOG_1X_BASE_C)

/* 13 Position and speed information read from ETAK */
#define LOG_ETAK_C                                     (0xD + LOG_1X_BASE_C)

/* 14 Markov frame statistics */
#define LOG_MAR_C                                      (0xE + LOG_1X_BASE_C)

/* 15 New and improved temporal analyzer searcher info */
#define LOG_SRCH2_C                                    (0xF + LOG_1X_BASE_C)

/* 16 The Fujitsu handset information */
#define LOG_HANDSET_C                                  (0x10 + LOG_1X_BASE_C)

/* 17 Vocoder bit error rate mask */
#define LOG_ERRMASK_C                                  (0x11 + LOG_1X_BASE_C)

/* 18 Analog voice channel information */
#define LOG_ANALOG_INFO_C                              (0x12 + LOG_1X_BASE_C)

/* 19 Access probe information */
#define LOG_ACC_INFO_C                                 (0x13 + LOG_1X_BASE_C)

/* 20 Position & speed info read from GPS receiver */
#define LOG_GPS_C                                      (0x14 + LOG_1X_BASE_C)

/* 21 Test Command information */
#define LOG_TEST_CMD_C                                 (0x15 + LOG_1X_BASE_C)

/* 22 Sparse (20ms) AGC / closed loop power control entry */
#define LOG_S_AGC_PCTL_C                               (0x16 + LOG_1X_BASE_C)
  
/* 23 Notification of a band class change */
#define LOG_BAND_CHANGE_C                              (0x17 + LOG_1X_BASE_C)

/* 24 DM debug messages, if being logged via log services */
#define LOG_DBG_MSG_C                                  (0x18 + LOG_1X_BASE_C)

/* 25 General temporal analyzer entry */
#define LOG_GENRL_TA_C                                 (0x19 + LOG_1X_BASE_C)

/* 26 General temporal analyzer w/supplemental channels */
#define LOG_GENRL_TA_SUP_CH_C                          (0x1A + LOG_1X_BASE_C)

#ifdef FEATURE_PLT
#error code not present
#else
/* 27 EFS Usage Info - No implementation as yet */
#define LOG_EFS_INFO_C                                 (0x1B + LOG_1X_BASE_C)
#endif

/* 28 Analog Forward Channel */
#define LOG_ANALOG_FORW_C                              (0x1C + LOG_1X_BASE_C)

/* 29 Analog Reverse Channel */
#define LOG_ANALOG_REVS_C                              (0x1D + LOG_1X_BASE_C)

/* 30 Analog Handoff Entry */
#define LOG_ANALOG_HANDOFF_C                           (0x1E + LOG_1X_BASE_C)

/* 31 FM Slot Statistis entry */
#define LOG_ANALOG_FMSLOT_C                            (0x1F + LOG_1X_BASE_C)
  
/* 32 FOCC Word Sync Count entry */
#define LOG_ANALOG_WS_COUNT_C                          (0x20 + LOG_1X_BASE_C)

/* 33 */
#define LOG_RLP_PACKET_C                               (0x21 + LOG_1X_BASE_C)

/* 34 */
#define LOG_ASYNC_TCP_SEG_C                            (0x22 + LOG_1X_BASE_C)

/* 35 */
#define LOG_PACKET_DATA_IP_PACKETS_C                   (0x23 + LOG_1X_BASE_C)

/* 36 */
#define LOG_FNBDT_MESSAGE_LOG_C                        (0x24 + LOG_1X_BASE_C)

/* Begin IS-2000 LOG features */
 
/* 37 RLP RX Frames logging */ 
#define LOG_RLP_RX_FRAMES_C                            (0x25 + LOG_1X_BASE_C)
  
/* 38 RLP TX Frames logging */
#define LOG_RLP_TX_FRAMES_C                            (0x26 + LOG_1X_BASE_C)

/* 39 Reserved for additions to RLP frames */
#define LOG_RLP_RSVD1_C                                (0x27 + LOG_1X_BASE_C)

/* 40 Reserved for additions to RLP frames */
#define LOG_RLP_RSVD2_C                                (0x28 + LOG_1X_BASE_C)

/* 41 Forward Link Frame Types logging */
#define LOG_FWD_FRAME_TYPES_C                          (0x29 + LOG_1X_BASE_C)
  
/* 42 Reverse Link Frame Types logging */
#define LOG_REV_FRAME_TYPES_C                          (0x2A + LOG_1X_BASE_C)
  
/* 43 Fast Forward Power Control Parameters logging */
#define LOG_FFWD_PCTRL_C                               (0x2B + LOG_1X_BASE_C)

/* 44 Reverse Power Control Parameters logging */
#define LOG_REV_PCTRL_C                                (0x2C + LOG_1X_BASE_C)

/* 45 Searcher and Finger Information logging */
#define LOG_SRCH_FING_INFO_C                           (0x2D + LOG_1X_BASE_C)
  
/* 46 Service Configuration logging */
#define LOG_SVC_CONFIG_C                               (0x2E + LOG_1X_BASE_C)
  
/* 47 Active Set Configuration logging */
#define LOG_ASET_CONFIG_C                              (0x2F + LOG_1X_BASE_C)

/* 48 Quick Paging Channel logging */
#define LOG_QPCH_C                                     (0x30 + LOG_1X_BASE_C)

/* 49 RLP Statistics logging */
#define LOG_RLP_STAT_C                                 (0x31 + LOG_1X_BASE_C)

/* 50 Simple Test Data Service Option logging */
#define LOG_STDSO_C                                    (0x32 + LOG_1X_BASE_C)

/* 51 Pilot Phase Measurement results logging */
#define LOG_SRCH_PPM_RES_C                             (0x33 + LOG_1X_BASE_C)

/* 52 Pilot Phase Measurement Data Base logging */
#define LOG_SRCH_PPM_DB_C                              (0x34 + LOG_1X_BASE_C)

/* 53 Pilot Phase Measurement search results logging */
#define LOG_SRCH_PPM_C                                 (0x35 + LOG_1X_BASE_C)

/* 54 IS-801 forward link message */
#define LOG_GPS_FWD_MSG_C                              (0x36 + LOG_1X_BASE_C)

/* 55 IS-801 reverse link message */
#define LOG_GPS_REV_MSG_C                              (0x37 + LOG_1X_BASE_C)

/* 56 GPS search session statistics */
#define LOG_GPS_STATS_MSG_C                            (0x38 + LOG_1X_BASE_C)

/* 57 GPS search results */
#define LOG_GPS_SRCH_PEAKS_MSG_C                       (0x39 + LOG_1X_BASE_C)

/* 58 Factory Testmode logging */
#define LOG_FTM_C                                      (0x3A + LOG_1X_BASE_C)

/* 59 Multiple Peak Logging */
#define LOG_SRCH_GPS_MULTI_PEAKS_INFO_C                (0x3B + LOG_1X_BASE_C)

/* 60 Post processed search results logs */
#define LOG_SRCH_GPS_POST_PROC_C                       (0x3C + LOG_1X_BASE_C)

/* 61 FULL Test Data Service Option logging */
#define LOG_FTDSO_C                                    (0x3D + LOG_1X_BASE_C)

/* 62 Bluetooth logging */
#define LOG_BT_RESERVED_CODES_BASE_C                   (0x3E + LOG_1X_BASE_C)
/* Keep confusing name for backwards compatibility.  */
#define LOG_BT_BASE_C LOG_BT_RESERVED_CODES_BASE_C

/* 92 Bluetooth's last log code */
#define LOG_BT_LAST_C                                  (30 + LOG_BT_RESERVED_CODES_BASE_C)

/* 93 HDR log codes */
#define LOG_HDR_RESERVED_CODES_BASE_C                  (0x5D + LOG_1X_BASE_C)
/* Keep confusing name for backwards compatibility.  */
#define LOG_HDR_BASE_C LOG_HDR_RESERVED_CODES_BASE_C

/* 143 is HDR's last log code */
#define LOG_HDR_LAST_C                                 (50 + LOG_HDR_RESERVED_CODES_BASE_C)

/* 144 IS2000 DCCH Forward link channel */
#define LOG_FOR_DCCH_MSG_C                             (0x90 + LOG_1X_BASE_C)
#define LOG_DCCH_FWD_C                                 LOG_FOR_DCCH_MSG_C
  
/* 145 IS2000 DCCH Forward link channel */
#define LOG_REV_DCCH_MSG_C                             (0x91 + LOG_1X_BASE_C)
#define LOG_DCCH_REV_C                                 LOG_REV_DCCH_MSG_C

/* 146 IS2000 DCCH Forward link channel */
#define LOG_ZREX_C                                     (0x92 + LOG_1X_BASE_C)

/* 147 Active set info logging, similar to ASET_CONFIG, but simpler.  */
#define LOG_ASET_INFO_C                                (0x93 + LOG_1X_BASE_C)

/* 148 Pilot Phase Measurement four-shoulder-search resutlts logging */
#define LOG_SRCH_PPM_4SHOULDER_RES_C                   (0x94 + LOG_1X_BASE_C)

/* 149 Extended Pilot Phase Measurement Data Base logging */
#define LOG_SRCH_EXT_PPM_DB_C                          (0x95 + LOG_1X_BASE_C)

/* 150 GPS Visit Parameters */
#define LOG_GPS_VISIT_PARAMETERS_C                     (0x96 + LOG_1X_BASE_C)

/* 151 GPS Measurement */
#define LOG_GPS_MEASUREMENT_C                          (0x97 + LOG_1X_BASE_C)

/* 152 UIM Data */
#define LOG_UIM_DATA_C                                 (0x98 + LOG_1X_BASE_C)

/* 153 STDSO plus P2 */
#define LOG_STDSO_P2_C                                 (0x99 + LOG_1X_BASE_C)

/* 154 FTDSO plus P2 */
#define LOG_FTDSO_P2_C                                 (0x9A + LOG_1X_BASE_C)

/* 155 Search PPM Statistics */
#define LOG_SRCH_PPM_STATS_C                           (0x9B + LOG_1X_BASE_C)

/* 156 PPP Tx Frames */
#define LOG_PPP_TX_FRAMES_C                            (0x9C + LOG_1X_BASE_C)

/* 157 PPP Rx Frames */
#define LOG_PPP_RX_FRAMES_C                            (0x9D + LOG_1X_BASE_C)

/* 158-187 SSL reserved log codes */
#define LOG_SSL_RESERVED_CODES_BASE_C                  (0x9E + LOG_1X_BASE_C)
#define LOG_SSL_LAST_C                                 (29 + LOG_SSL_RESERVED_CODES_BASE_C)

/* 188-199 Puma reserved log codes */
/* 188 QPCH, version 2 */
#define LOG_QPCH_VER_2_C                               (0xBC + LOG_1X_BASE_C)

/* 189 Enhanced Access Probe */
#define LOG_EA_PROBE_C                                 (0xBD + LOG_1X_BASE_C)

/* 190 BCCH Frame Information */
#define LOG_BCCH_FRAME_INFO_C                          (0xBE + LOG_1X_BASE_C)

/* 191 FCCCH Frame Information */
#define LOG_FCCCH_FRAME_INFO_C                         (0xBF + LOG_1X_BASE_C)

/* 192 FDCH Frame Information */
#define LOG_FDCH_FRAME_INFO_C                          (0xC0 + LOG_1X_BASE_C)

/* 193 RDCH Frame Information */
#define LOG_RDCH_FRAME_INFO_C                          (0xC1 + LOG_1X_BASE_C)

/* 194 FFPC Information */
#define LOG_FFPC_INFO_C                                (0xC2 + LOG_1X_BASE_C)

/* 195 RPC Information */
#define LOG_RPC_INFO_C                                 (0xC3 + LOG_1X_BASE_C)

/* 196 Searcher and Finger Information */
#define LOG_SRCH_FING_INFO_VER_2_C                     (0xC4 + LOG_1X_BASE_C)

/* 197 Service Configuration, version 2 */
#define LOG_SRV_CONFIG_VER_2_C                         (0xC5 + LOG_1X_BASE_C)

/* 198 Active Set Information, version 2 */
#define LOG_ASET_INFO_VER_2_C                          (0xC6 + LOG_1X_BASE_C)

/* 199 Reduced Active Set */
#define LOG_REDUCED_ASET_INFO_C                        (0xC7 + LOG_1X_BASE_C)

/* 200 Search Triage Info */
#define LOG_SRCH_TRIAGE_INFO_C                         (0xC8 + LOG_1X_BASE_C)

/* 201 RDA Frame Information */
#define LOG_RDA_FRAME_INFO_C                           (0xC9 + LOG_1X_BASE_C)

/* 202 gpsOne fatpath information */
#define LOG_GPS_FATPATH_INFO_C                         (0xCA + LOG_1X_BASE_C)

/* 203 Extended AGC */
#define LOG_EXTENDED_AGC_C                             (0xCB + LOG_1X_BASE_C)

/* 204 Transmit AGC */
#define LOG_TRANSMIT_AGC_C                             (0xCC + LOG_1X_BASE_C)

/* 205 I/Q Offset registers */
#define LOG_IQ_OFFSET_REGISTERS_C                      (0xCD + LOG_1X_BASE_C)

/* 206 DACC I/Q Accumulator registers */
#define LOG_DACC_IQ_ACCUMULATOR_C                      (0xCE + LOG_1X_BASE_C)

/* 207 Register polling results */
#define LOG_REGISTER_POLLING_RESULTS_C                 (0xCF + LOG_1X_BASE_C)

/* 208 System arbitration module */
#define LOG_AT_SAM_C                                   (0xD0 + LOG_1X_BASE_C)

/* 209 Diablo searcher finger log */
#define LOG_DIABLO_SRCH_FING_INFO_C                    (0xD1 + LOG_1X_BASE_C)

/* 210 log reserved for dandrus */
#define LOG_SD20_LAST_ACTION_C                         (0xD2 + LOG_1X_BASE_C)

/* 211 log reserved for dandrus */
#define LOG_SD20_LAST_ACTION_HYBRID_C                  (0xD3 + LOG_1X_BASE_C)

/* 212 log reserved for dandrus */
#define LOG_SD20_SS_OBJECT_C                           (0xD4 + LOG_1X_BASE_C)

/* 213 log reserved for dandrus */
#define LOG_SD20_SS_OBJECT_HYBRID_C                    (0xD5 + LOG_1X_BASE_C)

/* 214 log reserved for jpinos */
#define LOG_BCCH_SIGNALING_C                           (0xD6 + LOG_1X_BASE_C)

/* 215 log reserved for jpinos */
#define LOG_REACH_SIGNALING_C                          (0xD7 + LOG_1X_BASE_C)

/* 216 log reserved for jpinos */
#define LOG_FCCCH_SIGNALING_C                          (0xD8 + LOG_1X_BASE_C)

/* 217 RDA Frame Information 2 */
#define LOG_RDA_FRAME_INFO_2_C                         (0xD9 + LOG_1X_BASE_C)

/* 218 */
#define LOG_GPS_BIT_EDGE_RESULTS_C                     (0xDA + LOG_1X_BASE_C)

/* 219 */
#define LOG_PE_DATA_C                                  (0xDB + LOG_1X_BASE_C)

/* 220 */
#define LOG_PE_PARTIAL_DATA_C                          (0xDC + LOG_1X_BASE_C)

/* 221 */
#define LOG_GPS_SINGLE_PEAK_SRCH_RESULTS_C             (0xDD + LOG_1X_BASE_C)

/* 222 */
#define LOG_SRCH4_SAMPRAM_C                            (0xDE + LOG_1X_BASE_C)

/* 223 */
#define HDR_AN_PPP_TX_FRAMES                           (0xDF + LOG_1X_BASE_C)

/* 224 */
#define HDR_AN_PPP_RX_FRAMES                           (0xE0 + LOG_1X_BASE_C)

/* 225 */
#define LOG_GPS_SCHEDULER_TRACE_C                      (0xE1 + LOG_1X_BASE_C)

/* 226 */
#define LOG_MPEG4_YUV_FRAME_C                          (0xE2 + LOG_1X_BASE_C)

/* 227 */
#define LOG_MPEG4_CLIP_STATS_C                         (0xE3 + LOG_1X_BASE_C)

/* 228 */
#define LOG_MPEG4_CLIP_STATS_VER2_C                    (0xE4 + LOG_1X_BASE_C)

/* 226-241 MMEG reserved. */
#define LOG_MPEG_RESERVED_CODES_BASE_C                 (0xF1 + LOG_1X_BASE_C)

/* 242-274 BREW reserved log range */
#define LOG_BREW_RESERVED_CODES_BASE_C                 (0xF2 + LOG_1X_BASE_C)
#define LOG_BREW_LAST_C                                (32 + LOG_BREW_RESERVED_CODES_BASE_C)

/* 275-339 PPP Extended Frames */
#define LOG_PPP_FRAMES_RESERVED_CODES_BASE_C           (0x113 + LOG_1X_BASE_C)
#define LOG_PPP_FRAMES_LAST_C                          (64 + LOG_PPP_FRAMES_RESERVED_CODES_BASE_C)

#define LOG_PPP_EXT_FRAMED_RX_UM_C                     (0x113 + LOG_1X_BASE_C)
#define LOG_PPP_EXT_FRAMED_RX_RM_C                     (0x114 + LOG_1X_BASE_C)
#define LOG_PPP_EXT_FRAMED_RX_AN_C                     (0x115 + LOG_1X_BASE_C)

#define LOG_PPP_EXT_FRAMED_TX_UM_C                     (0x123 + LOG_1X_BASE_C)
#define LOG_PPP_EXT_FRAMED_TX_RM_C                     (0x124 + LOG_1X_BASE_C)
#define LOG_PPP_EXT_FRAMED_TX_AN_C                     (0x125 + LOG_1X_BASE_C)

#define LOG_PPP_EXT_UNFRAMED_RX_UM_C                   (0x133 + LOG_1X_BASE_C)
#define LOG_PPP_EXT_UNFRAMED_RX_RM_C                   (0x134 + LOG_1X_BASE_C)
#define LOG_PPP_EXT_UNFRAMED_RX_AN_C                   (0x135 + LOG_1X_BASE_C)

#define LOG_PPP_EXT_UNFRAMED_TX_UM_C                   (0x143 + LOG_1X_BASE_C)
#define LOG_PPP_EXT_UNFRAMED_TX_RM_C                   (0x144 + LOG_1X_BASE_C)
#define LOG_PPP_EXT_UNFRAMED_TX_AN_C                   (0x145 + LOG_1X_BASE_C)

/* 340 LOG_PE_DATA_EXT_C */
#define LOG_PE_DATA_EXT_C                              (0x154 + LOG_1X_BASE_C)

/* REX Subsystem logs */
#define LOG_MEMDEBUG_C                                 (0x155 + LOG_1X_BASE_C)
#define LOG_SYSPROFILE_C                               (0x156 + LOG_1X_BASE_C)
#define LOG_TASKPROFILE_C                              (0x157 + LOG_1X_BASE_C)
#define LOG_COREDUMP_C                                 (0x158 + LOG_1X_BASE_C)

/* 341-349 REX subsystem logs */
#define LOG_REX_RESERVED_CODES_BASE_C                  (0x155 + LOG_1X_BASE_C)
#define LOG_REX_LAST_C                                 (8 + LOG_REX_RESERVED_CODES_BASE_C)

/* 350 LOG_PE_PARTIAL_DATA_EXT_C */
#define LOG_PE_PARTIAL_DATA_EXT_C                      (0x15E + LOG_1X_BASE_C)

/* 351 LOG_DIAG_STRESS_TEST_C */
#define LOG_DIAG_STRESS_TEST_C                         (0x15F + LOG_1X_BASE_C)

/* 352  LOG_WMS_READ_C */
#define LOG_WMS_READ_C                                 (0x160 + LOG_1X_BASE_C)

/* 353 Search Triage Info Version 2 */
#define LOG_SRCH_TRIAGE_INFO2_C                         (0x161 + LOG_1X_BASE_C)

/* 354 RLP Rx FDCH Frames */
#define LOG_RLP_RX_FDCH_FRAMES_C                        (0x162 + LOG_1X_BASE_C)


/* 355 RLP Tx FDCH Frames */
#define LOG_RLP_TX_FDCH_FRAMES_C                        (0x163 + LOG_1X_BASE_C)

/* 356-371 QTV subsystem logs */
#define LOG_QTV_RESERVED_CODES_BASE_C                   (0x164 + LOG_1X_BASE_C)
#define LOG_QTV_LAST_C                                  (15 + LOG_QTV_RESERVED_CODES_BASE_C)

/* 372 Searcher 4 1X */
#define LOG_SRCH4_1X_C                                  (0x174 + LOG_1X_BASE_C)

/* 373 Searcher sleep statistics */
#define LOG_SRCH_SLEEP_STATS_C                          (0x175 + LOG_1X_BASE_C)

/* 374 Service Configuration, version 3 */
#define LOG_SRV_CONFIG_VER_3_C                          (0x176 + LOG_1X_BASE_C)

/* 375 Searcher 4 HDR */
#define LOG_SRCH4_HDR_C                                 (0x177 + LOG_1X_BASE_C)

/* 376 Searcher 4 AFLT */
#define LOG_SRCH4_AFLT_C                                (0x178 + LOG_1X_BASE_C)

/* 377 Enhanced Finger Information */
#define LOG_ENH_FING_INFO_C                             (0x179 + LOG_1X_BASE_C)

/* 378 DV Information */
#define LOG_DV_INFO_C                                   (0x17A + LOG_1X_BASE_C)

/* 379 WMS set routes information */
#define LOG_WMS_SET_ROUTES_C                            (0x17B + LOG_1X_BASE_C)

/* 380 FTM Version 2 Logs */
#define LOG_FTM_VER_2_C                                 (0x17C + LOG_1X_BASE_C)

/* 381 GPS Multipeak logging */
#define LOG_SRCH_GPS_MULTI_PEAKS_SIMPLIFIED_INFO_C      (0x17D + LOG_1X_BASE_C)

/* 382 GPS Multipeak logging */
#define LOG_SRCH_GPS_MULTI_PEAKS_VERBOSE_INFO_C         (0x17E + LOG_1X_BASE_C)

/* 383-403 HDR reserved logs */
#define LOG_HDR_RESERVED_CODES_BASE_2_C                 (0x17F + LOG_1X_BASE_C)
#define LOG_HDR_LAST_2_C                                (20 + LOG_HDR_RESERVED_CODES_BASE_2_C)

/* RLP Rx - PDCH partial MuxPDU5 frames */
#define LOG_RLP_RX_PDCH_PARTIAL_MUXPDU5_FRAMES_C        (0x194 + LOG_1X_BASE_C)

/* RLP Tx - PDCH partial MuxPDU5 frames */
#define LOG_RLP_TX_PDCH_PARTIAL_MUXPDU5_FRAMES_C        (0x195 + LOG_1X_BASE_C)

/* RLP Rx internal details */
#define LOG_RLP_RX_INTERNAL_DETAILS_C                   (0x196 + LOG_1X_BASE_C)

/* RLP Tx internal details */
#define LOG_RLP_TX_INTERNAL_DETAILS_C                   (0x197 + LOG_1X_BASE_C)

/* MPEG4 Clip Statistics version 3 */
#define LOG_MPEG4_CLIP_STATS_VER3_C                     (0x198 + LOG_1X_BASE_C)

/* Mobile IP Performance */
#define LOG_MOBILE_IP_PERFORMANCE_C                     (0x199 + LOG_1X_BASE_C)

/* 410-430 Searcher reserved logs */
#define LOG_SEARCHER_RESERVED_CODES_BASE_C              (0x19A + LOG_1X_BASE_C)
#define LOG_SEARCHER_LAST_2_C                           (21 + LOG_SEARCHER_RESERVED_CODES_BASE_C)

/* 432-480 QTV reserved logs */
#define LOG_QTV2_RESERVED_CODES_BASE_C                  (0x1B0 + LOG_1X_BASE_C)
#define LOG_QTV2_LAST_C                                 (48 + LOG_QTV2_RESERVED_CODES_BASE_C)

#define LOG_QTV_PDS2_STATS                              (0x1B6 + LOG_1X_BASE_C)
#define LOG_QTV_PDS2_GET_REQUEST                        (0x1B7 + LOG_1X_BASE_C)
#define LOG_QTV_PDS2_GET_RESP_HEADER                    (0x1B8 + LOG_1X_BASE_C)
#define LOG_QTV_PDS2_GET_RESP_PCKT                      (0x1B9 + LOG_1X_BASE_C)
#define LOG_QTV_CMX_AUDIO_INPUT_DATA_C                  (0x1BA + LOG_1X_BASE_C)
#define LOG_QTV_RTSP_OPTIONS_C                          (0x1BB + LOG_1X_BASE_C)
#define LOG_QTV_RTSP_GET_PARAMETER_C                    (0x1BC + LOG_1X_BASE_C)
#define LOG_QTV_RTSP_SET_PARAMETER_C                    (0x1BD + LOG_1X_BASE_C)
#define LOG_QTV_VIDEO_BITSTREAM                         (0x1BE + LOG_1X_BASE_C)
#define LOG_ARM_VIDEO_DECODE_STATS                      (0x1BF + LOG_1X_BASE_C)
#define LOG_QTV_DSP_SLICE_BUFFER_C                      (0x1C0 + LOG_1X_BASE_C)
#define LOG_QTV_CMD_LOGGING_C                           (0x1C1 + LOG_1X_BASE_C)
#define LOG_QTV_AUDIO_FRAME_PTS_INFO_C                  (0x1C2 + LOG_1X_BASE_C)
#define LOG_QTV_VIDEO_FRAME_DECODE_INFO_C               (0x1C3 + LOG_1X_BASE_C)
#define LOG_QTV_RTCP_COMPOUND_RR_C                      (0x1C4 + LOG_1X_BASE_C)
#define LOG_QTV_FRAME_BUFFER_RELEASE_REASON_C           (0x1C5 + LOG_1X_BASE_C)
#define LOG_QTV_AUDIO_CHANNEL_SWITCH_FRAME_C            (0x1C6 + LOG_1X_BASE_C)
#define LOG_QTV_RTP_DECRYPTED_PKT_C                     (0x1C7 + LOG_1X_BASE_C)
#define LOG_QTV_PCR_DRIFT_RATE_C                        (0x1C8 + LOG_1X_BASE_C)

/* GPS PDSM logs */
#define LOG_PDSM_POSITION_REPORT_CALLBACK_C             (0x1E1 + LOG_1X_BASE_C)
#define LOG_PDSM_PD_EVENT_CALLBACK_C                    (0x1E2 + LOG_1X_BASE_C)
#define LOG_PDSM_PA_EVENT_CALLBACK_C                    (0x1E3 + LOG_1X_BASE_C)
#define LOG_PDSM_NOTIFY_VERIFY_REQUEST_C                (0x1E4 + LOG_1X_BASE_C)
#define LOG_PDSM_RESERVED1_C                            (0x1E5 + LOG_1X_BASE_C)
#define LOG_PDSM_RESERVED2_C                            (0x1E6 + LOG_1X_BASE_C)

/* Searcher Demodulation Status log */
#define LOG_SRCH_DEMOD_STATUS_C                         (0x1E7 + LOG_1X_BASE_C)

/* Searcher Call Statistics log */
#define LOG_SRCH_CALL_STATISTICS_C                      (0x1E8 + LOG_1X_BASE_C)

/* GPS MS-MPC Forward link */
#define LOG_MS_MPC_FWD_LINK_C                           (0x1E9 + LOG_1X_BASE_C)

/* GPS MS-MPC Reverse link */
#define LOG_MS_MPC_REV_LINK_C                           (0x1EA + LOG_1X_BASE_C)

/* Protocol Services Data */
#define LOG_DATA_PROTOCOL_LOGGING_C                     (0x1EB + LOG_1X_BASE_C)

/* MediaFLO reserved log codes */
#define LOG_MFLO_RESERVED_CODES_BASE_C                  (0x1EC + LOG_1X_BASE_C)
#define LOG_MFLO_LAST_C                                 (99 + LOG_MFLO_RESERVED_CODES_BASE_C)

/* GPS demodulation tracking header info */
#define LOG_GPS_DEMOD_TRACKING_HEADER_C                 (0x250 + LOG_1X_BASE_C)

/* GPS demodulation tracking results */
#define LOG_GPS_DEMOD_TRACKING_C                        (0x251 + LOG_1X_BASE_C)

/* GPS bit edge logs from demod tracking */
#define LOG_GPS_DEMOD_BIT_EDGE_C                        (0x252 + LOG_1X_BASE_C)

/* GPS demodulation soft decisions */
#define LOG_GPS_DEMOD_SOFT_DECISIONS_C                  (0x253 + LOG_1X_BASE_C)

/* GPS post-processed demod tracking results */
#define LOG_GPS_DEMOD_TRACKING_POST_PROC_C              (0x254 + LOG_1X_BASE_C)

/* GPS subframe log */
#define LOG_GPS_DEMOD_SUBFRAME_C                        (0x255 + LOG_1X_BASE_C)

/* F-CPCCH Quality Information */
#define LOG_F_CPCCH_QUALITY_INFO_C                      (0x256 + LOG_1X_BASE_C)

/* Reverse PDCCH/PDCH Frame Information */
#define LOG_R_PDCCH_R_PDCH_FRAME_INFO_C                 (0x257 + LOG_1X_BASE_C)

/* Forward G Channel Information */
#define LOG_F_GCH_INFO_C                                (0x258 + LOG_1X_BASE_C)

/* Forward G Channel Frame Information */
#define LOG_F_GCH_FRAME_INFO_C                          (0x259 + LOG_1X_BASE_C)

/* Forward RC Channel Information */
#define LOG_F_RCCH_INFO_C                               (0x25A + LOG_1X_BASE_C)

/* Forward ACK Channel Information */
#define LOG_F_ACKCH_INFO_C                              (0x25B + LOG_1X_BASE_C)

/* Forward ACK Channel ACKDA Information */
#define LOG_F_ACKCH_ACKDA_C                             (0x25C + LOG_1X_BASE_C)

/* Reverse REQ Channel Information */
#define LOG_R_REQCH_INFO_C                              (0x25D + LOG_1X_BASE_C)

/* Sleep Task Statistics */
#define LOG_SLEEP_STATS_C                               (0x25E + LOG_1X_BASE_C)

/* Sleep controller statistics 1X */
#define LOG_1X_SLEEP_CONTROLLER_STATS_C                 (0x25F + LOG_1X_BASE_C)

/* Sleep controller statistics HDR */
#define LOG_HDR_SLEEP_CONTROLLER_STATS_C                (0x260 + LOG_1X_BASE_C)

/* Sleep controller statistics GSM */
#define LOG_GSM_SLEEP_CONTROLLER_STATS_C                (0x261 + LOG_1X_BASE_C)

/* Sleep controller statistics WCDMA */
#define LOG_WCDMA_SLEEP_CONTROLLER_STATS_C              (0x262 + LOG_1X_BASE_C)

/* Sleep task and controller reserved logs */
#define LOG_SLEEP_APPS_STATS_C                          (0x263 + LOG_1X_BASE_C)
#define LOG_SLEEP_STATS_RESERVED2_C                     (0x264 + LOG_1X_BASE_C)
#define LOG_SLEEP_STATS_RESERVED3_C                     (0x265 + LOG_1X_BASE_C)

/* DV Information placeholder channel logs */
#define LOG_PDCCH_LO_SELECTED_C                         (0x266 + LOG_1X_BASE_C)
#define LOG_PDCCH_HI_SELECTED_C                         (0x267 + LOG_1X_BASE_C)
#define LOG_WALSH_SELECTED_C                            (0x268 + LOG_1X_BASE_C)
#define LOG_PDCH_BE_SELECTED_C                          (0x269 + LOG_1X_BASE_C)
#define LOG_PDCCH_LLR_SELECTED_C                        (0x26A + LOG_1X_BASE_C)
#define LOG_CQI_ACK_LO_SELECTED_C                       (0x26B + LOG_1X_BASE_C)
#define LOG_CQI_ACK_HI_SELECTED_C                       (0x26C + LOG_1X_BASE_C)
#define LOG_RL_GAIN_SELECTED_C                          (0x26D + LOG_1X_BASE_C)
#define LOG_PDCCH0_SNDA_SELECTED_C                      (0x26E + LOG_1X_BASE_C)
#define LOG_PDCCH1_SNDA_SELECTED_C                      (0x26F + LOG_1X_BASE_C)

/* 624 WMS Message List */
#define LOG_WMS_MESSAGE_LIST_C                          (0x270 + LOG_1X_BASE_C)

/* 625 Multimode Generic SIM Driver Interface */
#define LOG_MM_GENERIC_SIM_DRIVER_C                     (0x271 + LOG_1X_BASE_C)

/* 626 Generic SIM Toolkit Task */
#define LOG_GENERIC_SIM_TOOLKIT_TASK_C                  (0x272 + LOG_1X_BASE_C)

/* 627 Call Manager Phone events log */
#define LOG_CM_PH_EVENT_C                               (0x273 + LOG_1X_BASE_C)

/* 628 WMS Set Message List */
#define LOG_WMS_SET_MESSAGE_LIST_C                      (0x274 + LOG_1X_BASE_C)

/* 629-704 HDR reserved logs */
#define LOG_HDR_RESERVED_CODES_BASE_3_C                 (0x275 + LOG_1X_BASE_C)
#define LOG_HDR_LAST_3_C                                (75 + LOG_HDR_RESERVED_CODES_BASE_3_C)

/* 705 Call Manager call event log */
#define LOG_CM_CALL_EVENT_C                             (0x2C1 + LOG_1X_BASE_C)

/* 706-738 QVP reserved logs */
#define LOG_QVP_RESERVED_CODES_BASE_C                   (0x2C2 + LOG_1X_BASE_C)
#define LOG_QVP_LAST_C                                  (32 + LOG_QVP_RESERVED_CODES_BASE_C)

/* 739 GPS PE Position Report log */
#define LOG_GPS_PE_POSITION_REPORT_C                    (0x2E3 + LOG_1X_BASE_C)

/* 740 GPS PE Position Report Extended log */
#define LOG_GPS_PE_POSITION_REPORT_EXT_C                (0x2E4 + LOG_1X_BASE_C)

/* 741 log */
#define LOG_MDDI_HOST_STATS_C                           (0x2E5 + LOG_1X_BASE_C)

/* GPS Decoded Ephemeris */
#define LOG_GPS_DECODED_EPHEMERIS_C                     (0x2E6 + LOG_1X_BASE_C)
 
/* GPS Decoded Almanac */
#define LOG_GPS_DECODED_ALMANAC_C                       (0x2E7 + LOG_1X_BASE_C)

/* Transceiver Resource Manager */
#define LOG_TRANSCEIVER_RESOURCE_MGR_C                  (0x2E8 + LOG_1X_BASE_C)

/* GPS Position Engine Info */
#define LOG_GPS_POSITION_ENGINE_INFO_C                  (0x2E9 + LOG_1X_BASE_C)

/* 746-810 RAPTOR reserved log range */
#define LOG_RAPTOR_RESERVED_CODES_BASE_C                (0x2EA + LOG_1X_BASE_C)
#define LOG_RAPTOR_LAST_C                               (64 + LOG_RAPTOR_RESERVED_CODES_BASE_C)

/* QOS Specification Logging */

/* QOS Requested Log */
#define LOG_QOS_REQUESTED_C                             (0x32B + LOG_1X_BASE_C)

/* QOS Granted Log */
#define LOG_QOS_GRANTED_C                               (0x32C + LOG_1X_BASE_C)

/* QOS State Log */
#define LOG_QOS_STATE_C                                 (0x32D + LOG_1X_BASE_C)

#define LOG_QOS_MODIFIED_C                              (0x32E + LOG_1X_BASE_C)

#define LOG_QDJ_ENQUEUE_C                               (0x32F + LOG_1X_BASE_C)
#define LOG_QDJ_DEQUEUE_C                               (0x330 + LOG_1X_BASE_C)
#define LOG_QDJ_UPDATE_C                                (0x331 + LOG_1X_BASE_C)
#define LOG_QDTX_ENCODER_C                              (0x332 + LOG_1X_BASE_C)
#define LOG_QDTX_DECODER_C                              (0x333 + LOG_1X_BASE_C)

#define LOG_PORT_ASSIGNMENT_STATUS_C                    (0x334 + LOG_1X_BASE_C)

/* Protocol Services reserved log codes */
#define LOG_PS_RESERVED_CODES_BASE_C                    (0x335 + LOG_1X_BASE_C)
#define LOG_PS_LAST_C                                   (25 + LOG_PS_RESERVED_C)

#define LOG_PS_STAT_IP_C                                (0x335 + LOG_1X_BASE_C)
#define LOG_PS_STAT_GLOBAL_IPV4_C                       (0x335 + LOG_1X_BASE_C)
#define LOG_PS_STAT_GLOBAL_IPV6_C                       (0x336 + LOG_1X_BASE_C)
#define LOG_PS_STAT_GLOBAL_ICMPV4_C                     (0x337 + LOG_1X_BASE_C)
#define LOG_PS_STAT_GLOBAL_ICMPV6_C                     (0x338 + LOG_1X_BASE_C)
#define LOG_PS_STAT_GLOBAL_TCP_C                        (0x339 + LOG_1X_BASE_C)
#define LOG_PS_STAT_GLOBAL_UDP_C                        (0x33A + LOG_1X_BASE_C)

/* Call Manager Serving System event log */
#define LOG_CM_SS_EVENT_C                               (0x34F + LOG_1X_BASE_C)

/* VcTcxo manager�s automatic frequency control log */
#define LOG_TCXOMGR_AFC_DATA_C                          (0x350 + LOG_1X_BASE_C)

/* Clock transactions and general clocks status log */
#define LOG_CLOCK_C                                     (0x351 + LOG_1X_BASE_C)

/* GPS search processed peak results and their associated search parameters */
#define LOG_GPS_PROCESSED_PEAK_C                        (0x352 + LOG_1X_BASE_C)

#define LOG_MDSP_LOG_CHUNKS_C                           (0x353 + LOG_1X_BASE_C)

/* Periodic RSSI update log */
#define LOG_WLAN_RSSI_UPDATE_C                          (0x354 + LOG_1X_BASE_C)

/* Periodic Link Layer statistics log */
#define LOG_WLAN_LL_STAT_C                              (0x355 + LOG_1X_BASE_C)

/* QOS Extended State Log */
#define LOG_QOS_STATE_EX_C                              (0x356 + LOG_1X_BASE_C)

/* Bluetooth host HCI transmitted data */
#define LOG_BT_HOST_HCI_TX_C                            (0x357 + LOG_1X_BASE_C)

/* Bluetooth host HCI received data */
#define LOG_BT_HOST_HCI_RX_C                            (0x358 + LOG_1X_BASE_C)

/* Internal - GPS PE Position Report Part 3 */
#define LOG_GPS_PE_POSITION_REPORT_PART3_C              (0x359 + LOG_1X_BASE_C)

/* Extended log code which logs requested QoS */
#define LOG_QOS_REQUESTED_EX_C                          (0x35A + LOG_1X_BASE_C)

/* Extended log code which logs granted QoS */
#define LOG_QOS_GRANTED_EX_C                            (0x35B + LOG_1X_BASE_C)

/* Extended log code which logs modified QoS */
#define LOG_QOS_MODIFIED_EX_C                           (0x35C + LOG_1X_BASE_C)

/* Bus Monitor Profiling Info */
#define LOG_BUS_MON_PROF_INFO_C                         (0x35D + LOG_1X_BASE_C)

/* Pilot Phase Measurement Search results */
#define LOG_SRCH_PPM_RES_VER_2_C                        (0x35E + LOG_1X_BASE_C)

/* Pilot Phase Measurement Data Base */
#define LOG_SRCH_PPM_DB_VER_2_C                         (0x35F + LOG_1X_BASE_C)

/* Pilot Phase Measurement state machine */
#define LOG_PPM_SM_C                                    (0x360 + LOG_1X_BASE_C)

/* Robust Header Compression - Compressor */
#define LOG_ROHC_COMPRESSOR_C                           (0x361 + LOG_1X_BASE_C)

/* Robust Header Compression - Decompressor */
#define LOG_ROHC_DECOMPRESSOR_C                         (0x362 + LOG_1X_BASE_C)

/* Robust Header Compression - Feedback Compressor */
#define LOG_ROHC_FEEDBACK_COMPRESSOR_C                  (0x363 + LOG_1X_BASE_C)

/* Robust Header Compression - Feedback Decompressor */
#define LOG_ROHC_FEEDBACK_DECOMPRESSOR_C                (0x364 + LOG_1X_BASE_C)

/* Bluetooth HCI commands */
#define LOG_BT_HCI_CMD_C                                (0x365 + LOG_1X_BASE_C)

/* Bluetooth HCI events */
#define LOG_BT_HCI_EV_C                                 (0x366 + LOG_1X_BASE_C)

/* Bluetooth HCI Transmitted ACL data */
#define LOG_BT_HCI_TX_ACL_C                             (0x367 + LOG_1X_BASE_C)

/* Bluetooth HCI Received ACL data */
#define LOG_BT_HCI_RX_ACL_C                             (0x368 + LOG_1X_BASE_C)

/* Bluetooth SOC H4 Deep Sleep */
#define LOG_BT_SOC_H4DS_C                               (0x369 + LOG_1X_BASE_C)

/* UMTS to CDMA Handover Message */
#define LOG_UMTS_TO_CDMA_HANDOVER_MSG_C                 (0x36A + LOG_1X_BASE_C)

/* Graphic Event Data */
#define LOG_PROFILER_GRAPHIC_DATA_C                     (0x36B + LOG_1X_BASE_C)

/* Audio Event Data */
#define LOG_PROFILER_AUDIO_DATA_C                       (0x36C + LOG_1X_BASE_C)

/* GPS Spectral Information */
#define LOG_GPS_SPECTRAL_INFO_C                         (0x36D + LOG_1X_BASE_C)

/* AHB Performance Monitor LOG data */
#define LOG_APM_C                                       (0x36E + LOG_1X_BASE_C)

/* GPS Clock Report */
#define LOG_CONVERGED_GPS_CLOCK_REPORT_C                (0x36F + LOG_1X_BASE_C)

/* GPS Position Report */
#define LOG_CONVERGED_GPS_POSITION_REPORT_C             (0x370 + LOG_1X_BASE_C)

/* GPS Measurement Report */
#define LOG_CONVERGED_GPS_MEASUREMENT_REPORT_C          (0x371 + LOG_1X_BASE_C)

/* GPS RF Status Report */
#define LOG_CONVERGED_GPS_RF_STATUS_REPORT_C            (0x372 + LOG_1X_BASE_C)

/* VOIP To CDMA Handover Message - Obsoleted by 0x138B - 0x138D */
#define LOG_VOIP_TO_CDMA_HANDOVER_MSG_C                 (0x373 + LOG_1X_BASE_C)

/* GPS Prescribed Dwell Result */
#define LOG_GPS_PRESCRIBED_DWELL_RESULT_C               (0x374 + LOG_1X_BASE_C)

/* CGPS IPC Data */
#define LOG_CGPS_IPC_DATA_C                             (0x375 + LOG_1X_BASE_C)

/* CGPS Non IPC Data */
#define LOG_CGPS_NON_IPC_DATA_C                         (0x376 + LOG_1X_BASE_C)

/* CGPS Session Report */
#define LOG_CGPS_REP_EVT_LOG_PACKET_C                   (0x377 + LOG_1X_BASE_C)

/* CGPS PDSM Get Position */
#define LOG_CGPS_PDSM_GET_POSITION_C                    (0x378 + LOG_1X_BASE_C)

/* CGPS PDSM Set Parameters */
#define LOG_CGPS_PDSM_SET_PARAMETERS_C                  (0x379 + LOG_1X_BASE_C)

/* CGPS PDSM End Session */
#define LOG_CGPS_PDSM_END_SESSION_C                     (0x37A + LOG_1X_BASE_C)

/* CGPS PDSM Notify Verify Response */
#define LOG_CGPS_PDSM_NOTIFY_VERIFY_RESP_C              (0x37B + LOG_1X_BASE_C)

/* CGPS PDSM Position Report Callback */
#define LOG_CGPS_PDSM_POSITION_REPORT_CALLBACK_C        (0x37C + LOG_1X_BASE_C)

/* CGPS PDSM PD Event Callback */
#define LOG_CGPS_PDSM_PD_EVENT_CALLBACK_C               (0x37D + LOG_1X_BASE_C)

/* CGPS PDSM PA Event Callback */
#define LOG_CGPS_PDSM_PA_EVENT_CALLBACK_C               (0x37E + LOG_1X_BASE_C)

/* CGPS PDSM Notify Verify Request Callback */
#define LOG_CGPS_PDSM_NOTIFY_VERIFY_REQUEST_C           (0x37F + LOG_1X_BASE_C)

/* CGPS PDSM PD Command Error Callback */
#define LOG_CGPS_PDSM_PD_CMD_ERR_CALLBACK_C             (0x380 + LOG_1X_BASE_C)

/* CGPS PDSM PA Command Error Callback */
#define LOG_CGPS_PDSM_PA_CMD_ERR_CALLBACK_C             (0x381 + LOG_1X_BASE_C)

/* CGPS PDSM Position Error */
#define LOG_CGPS_PDSM_POS_ERROR_C                       (0x382 + LOG_1X_BASE_C)

/* CGPS PDSM Extended Status Position Report */
#define LOG_CGPS_PDSM_EXT_STATUS_POS_REPORT_C           (0x383 + LOG_1X_BASE_C)

/* CGPS PDSM Extended Status NMEA Report */
#define LOG_CGPS_PDSM_EXT_STATUS_NMEA_REPORT_C          (0x384 + LOG_1X_BASE_C)

/* CGPS PDSM Extended Status Measurement Report */
#define LOG_CGPS_PDSM_EXT_STATUS_MEAS_REPORT_C          (0x385 + LOG_1X_BASE_C)

/* CGPS Report Server TX Packet */
#define LOG_CGPS_REP_SVR_TX_LOG_PACKET_C                (0x386 + LOG_1X_BASE_C)

/* CGPS Report Server RX Packet */
#define LOG_CGPS_REP_SVR_RX_LOG_PACKET_C                (0x387 + LOG_1X_BASE_C)

/* UMTS To CDMA Handover Paging Channel Message */
#define LOG_UMTS_TO_CDMA_HANDOVER_PCH_MSG_C             (0x388 + LOG_1X_BASE_C)

/* UMTS To CDMA Handover Traffic Channel Message */
#define LOG_UMTS_TO_CDMA_HANDOVER_TCH_MSG_C             (0x389 + LOG_1X_BASE_C)

/* Converged GPS IQ Report */
#define LOG_CONVERGED_GPS_IQ_REPORT_C                   (0x38A + LOG_1X_BASE_C)

/* VOIP To CDMA Paging Channel Handover Message */
#define LOG_VOIP_TO_CDMA_PCH_HANDOVER_MSG_C             (0x38B + LOG_1X_BASE_C)

/* VOIP To CDMA Access Channel Handover Message */
#define LOG_VOIP_TO_CDMA_ACH_HANDOVER_MSG_C             (0x38C + LOG_1X_BASE_C)

/* VOIP To CDMA Forward Traffic Channel Handover Message */
#define LOG_VOIP_TO_CDMA_FTC_HANDOVER_MSG_C             (0x38D + LOG_1X_BASE_C)

/* QMI reserved logs */
#define LOG_QMI_RESERVED_CODES_BASE_C                   (0x38E + LOG_1X_BASE_C)
#define LOG_QMI_LAST_C                                  (32 + LOG_QMI_RESERVED_CODES_BASE_C)

/* QOS Info Code Update Log */
#define LOG_QOS_INFO_CODE_UPDATE_C                      (0x3AF + LOG_1X_BASE_C)

/* Transmit(Uplink) Vocoder PCM Packet Log */
#define LOG_TX_PCM_PACKET_C                             (0x3B0 + LOG_1X_BASE_C)

/* Audio Vocoder Data Paths */
#define LOG_AUDVOC_DATA_PATHS_PACKET_C                  (0x3B0 + LOG_1X_BASE_C)

/* Receive(Downlink) Vocoder PCM Packet Log */
#define LOG_RX_PCM_PACKET_C                             (0x3B1 + LOG_1X_BASE_C)

/* CRC of YUV frame log */
#define LOG_DEC_CRC_FRAME_C                             (0x3B2 + LOG_1X_BASE_C)

/* FLUTE Session Information */
#define LOG_FLUTE_SESSION_INFO_C                        (0x3B3 + LOG_1X_BASE_C)

/* FLUTE ADP File Information */
#define LOG_FLUTE_ADP_FILE_INFO_C                       (0x3B4 + LOG_1X_BASE_C)

/* FLUTE File Request Information */
#define LOG_FLUTE_FILE_REQ_INFO_C                       (0x3B5 + LOG_1X_BASE_C)

/* FLUTE FDT Instance Information */
#define LOG_FLUTE_FDT_INST_C                            (0x3B6 + LOG_1X_BASE_C)

/* FLUTE FDT Information */
#define LOG_FLUTE_FDT_INFO_C                            (0x3B7 + LOG_1X_BASE_C)

/* FLUTE File Log Packet Information */
#define LOG_FLUTE_FILE_INFO_C                           (0x3B8 + LOG_1X_BASE_C)

/* 3G 1X Parameter Overhead Information */
#define LOG_VOIP_TO_CDMA_3G1X_PARAMETERS_C              (0x3B9 + LOG_1X_BASE_C)

/* CGPS ME Job Info */
#define LOG_CGPS_ME_JOB_INFO_C                          (0x3BA + LOG_1X_BASE_C)

/* CGPS ME SV Lists */
#define LOG_CPGS_ME_SV_LISTS_C                          (0x3BB + LOG_1X_BASE_C)

/* Flexible Profiling Status */
#define LOG_PROFDIAG_GEN_STATUS_C                       (0x3BC + LOG_1X_BASE_C)

/* Flexible Profiling Results */
#define LOG_PROFDIAG_GEN_PROF_C                         (0x3BD + LOG_1X_BASE_C)

/* FLUTE ADP File Content Log Packet Information */
#define LOG_FLUTE_ADP_FILE_C                            (0x3BE + LOG_1X_BASE_C)

/* FLUTE FDT Instance File Content Log Packet Information */
#define LOG_FLUTE_FDT_INST_FILE_C                       (0x3BF + LOG_1X_BASE_C)

/* FLUTE FDT Entries Information */
#define LOG_FLUTE_FDT_ENTRIES_INFO_C                    (0x3C0 + LOG_1X_BASE_C)

/* FLUTE File Contents Log Packet Information */
#define LOG_FLUTE_FILE_C                                (0x3C1 + LOG_1X_BASE_C)

/* CGPS ME Time-Transfer Info */
#define LOG_CGPS_ME_TIME_TRANSFER_INFO_C                (0x3C2 + LOG_1X_BASE_C)

/* CGPS ME UMTS Time-Tagging Info */
#define LOG_CGPS_ME_UMTS_TIME_TAGGING_INFO_C            (0x3C3 + LOG_1X_BASE_C)

/* CGPS ME Generic Time Estimate Put lnfo */
#define LOG_CGPS_ME_TIME_EST_PUT_INFO_C                 (0x3C4 + LOG_1X_BASE_C)

/* CGPS ME Generic Freq Estimate Put lnfo */
#define LOG_CGPS_ME_FREQ_EST_PUT_INFO_C                 (0x3C5 + LOG_1X_BASE_C)

/* CGPS Slow Clock Report */
#define LOG_CGPS_SLOW_CLOCK_REPORT_C                    (0x3C6 + LOG_1X_BASE_C)

/* Converged GPS Medium Grid */
#define LOG_CONVERGED_GPS_MEDIUM_GRID_C                 (0x3C7 + LOG_1X_BASE_C)

/* Static information about the driver or device */
#define LOG_SNSD_INFO_C                                 (0x3C8 + LOG_1X_BASE_C)

/* Dynamic state information about the device or driver */
#define LOG_SNSD_STATE_C                                (0x3C9 + LOG_1X_BASE_C)

/* Data from a driver */
#define LOG_SNSD_DATA                                   (0x3CA + LOG_1X_BASE_C)
#define LOG_SNSD_DATA_C                                 (0x3CA + LOG_1X_BASE_C)

/* CGPS Cell DB Cell Change Info */
#define LOG_CGPS_CELLDB_CELL_CHANGE_INFO_C              (0x3CB + LOG_1X_BASE_C)

/* xScalar YUV frame log */
#define LOG_DEC_XSCALE_YUV_FRAME_C                      (0x3CC + LOG_1X_BASE_C)

/* CRC of xScaled YUV frame log */
#define LOG_DEC_XSCALE_CRC_FRAME_C                      (0x3CD + LOG_1X_BASE_C)

/* CGPS Frequency Estimate Report */
#define LOG_CGPS_FREQ_EST_REPORT_C                      (0x3CE + LOG_1X_BASE_C)

/* GPS DCME Srch Job Completed */
#define LOG_GPS_DCME_SRCH_JOB_COMPLETED_C               (0x3CF + LOG_1X_BASE_C)

/* CGPS ME Fastscan results  */
#define LOG_CGPS_ME_FASTSCAN_RESULTS_C                  (0x3D0 + LOG_1X_BASE_C)

/* XO frequency Estimation log */
#define LOG_XO_FREQ_EST_C                               (0x3D1 + LOG_1X_BASE_C)

/* Tcxomgr field calibration data  */
#define LOG_TCXOMGR_FIELD_CAL_C                         (0x3D2 + LOG_1X_BASE_C)

/* UMB Call Processing Connection Attempt */
#define LOG_UMBCP_CONNECTION_ATTEMPT_C                  (0x3D3 + LOG_1X_BASE_C)

/* UMB Call Processing Connection Release */
#define LOG_UMBCP_CONNECTION_RELEASE_C                  (0x3D4 + LOG_1X_BASE_C)

/* UMB Call Processing Page Message */
#define LOG_UMBCP_PAGE_MESSAGE_C                        (0x3D5 + LOG_1X_BASE_C)

/* UMB Call Processing OVHD Information */
#define LOG_UMBCP_OVHD_INFO_C                           (0x3D6 + LOG_1X_BASE_C)

/* UMB Call Processing Session Attempt */
#define LOG_UMBCP_SESSION_ATTEMPT_C                     (0x3D7 + LOG_1X_BASE_C)

/* UMB Call Processing Route Information */
#define LOG_UMBCP_ROUTE_INFO_C                          (0x3D8 + LOG_1X_BASE_C)

/* UMB Call Processing State Information */
#define LOG_UMBCP_STATE_INFO_C                          (0x3D9 + LOG_1X_BASE_C)

/* UMB Call Processing SNP */
#define LOG_UMBCP_SNP_C                                 (0x3DA + LOG_1X_BASE_C)

/* CGPS Session Early Exit Decision */
#define LOG_CGPS_SESSION_EARLY_EXIT_DECISION_C          (0x3DB + LOG_1X_BASE_C)

/* GPS RF Linearity Status */
#define LOG_CGPS_ME_RF_LINEARITY_INFO_C                 (0x3DC + LOG_1X_BASE_C)

/* CGPS ME 5ms IQ Sums */
#define LOG_CGPS_ME_5MS_IQ_SUMS_C                       (0x3DD + LOG_1X_BASE_C)

/* CGPS ME 20ms IQ Sums */
#define LOG_CPGS_ME_20MS_IQ_SUMS_C                      (0x3DE + LOG_1X_BASE_C)

/* ROHC Compressor Statistics */
#define LOG_ROHC_COMPRESSOR_STATS_C                     (0x3DF + LOG_1X_BASE_C)

/* ROHC Decompressor Statistics */
#define LOG_ROHC_DECOMPRESSOR_STATS_C                   (0x3E0 + LOG_1X_BASE_C)

/* Sensors - Kalman filter information */
#define LOG_SENSOR_KF_INFO_C                            (0x3E1 + LOG_1X_BASE_C)

/* Sensors - Integrated measurements */
#define LOG_SENSOR_INT_MEAS_C                           (0x3E2 + LOG_1X_BASE_C)

/* Sensors - Bias calibration values */
#define LOG_SENSOR_BIAS_CALIBRATION_C                   (0x3E3 + LOG_1X_BASE_C)

/* DTV ISDB-T Transport Stream Packets */
#define LOG_DTV_ISDB_TS_PACKETS                         (0x3E4 + LOG_1X_BASE_C)

/* DTV ISDB-T PES Packets */
#define LOG_DTV_ISDB_PES_PACKETS                        (0x3E5 + LOG_1X_BASE_C)

/* DTV ISDB-T Sections */
#define LOG_DTV_ISDB_SECTIONS                           (0x3E6 + LOG_1X_BASE_C)

/* DTV ISDB-T Buffering */
#define LOG_DTV_ISDB_BUFFERING                          (0x3E7 + LOG_1X_BASE_C)

/* WLAN System Acquisition and Handoff */
#define LOG_WLAN_SYS_ACQ_HO_C                           (0x3E8 + LOG_1X_BASE_C)

/* WLAN General Configurable Parameters */
#define LOG_WLAN_GEN_CONFIG_PARAMS_C                    (0x3E9 + LOG_1X_BASE_C)

/* UMB Physical Layer Channel and Interference Estimation */
#define LOG_UMB_PHY_RX_DPICH_CIE_C                      (0x3EA + LOG_1X_BASE_C)

/* UMB Physical Layer MMSE/MRC Demodulated Data Symbols (Low) */
#define LOG_UMB_PHY_RX_DATA_DEMOD_LOW_C                 (0x3EB + LOG_1X_BASE_C)

/* UMB Physical Layer MMSE/MRC Demodulated Data Symbols (High) */
#define LOG_UMB_PHY_RX_DATA_DEMOD_HIGH_C                (0x3EC + LOG_1X_BASE_C)

/* UMB Physical Layer DCH Decoder */
#define LOG_UMB_PHY_RX_DCH_DECODER_C                    (0x3ED + LOG_1X_BASE_C)

/* UMB Physical Layer DCH Statistics */
#define LOG_UMB_PHY_DCH_STATISTICS_C                    (0x3EE + LOG_1X_BASE_C)

/* UMB Physical Layer CqiPich Processing */
#define LOG_UMB_PHY_RX_CQIPICH_C                        (0x3EF + LOG_1X_BASE_C)

/* UMB Physical Layer MIMO/SIMO in CqiPich (High) */
#define LOG_UMB_PHY_RX_CQIPICH_CHANTAPS_HIGH_C          (0x3F0 + LOG_1X_BASE_C)

/* UMB Physical Layer MIMO/SIMO in CquiPich (Low) */
#define LOG_UMB_PHY_RX_CQIPICH_CHANTAPS_LOW_C           (0x3F1 + LOG_1X_BASE_C)

/* UMB Physical Layer Time-Domain Channel Taps (High) */
#define LOG_UMB_PHY_RX_PPICH_CHAN_EST_HIGH_C            (0x3F2 + LOG_1X_BASE_C)

/* UMB Physical Layer Time-Domain Channel Taps (Low) */
#define LOG_UMB_PHY_RX_PPICH_CHAN_EST_LOW_C             (0x3F3 + LOG_1X_BASE_C)

/* UMB Physical Layer AT Modulator */
#define LOG_UMB_PHY_TX_PICH_CONFIG_C                    (0x3F4 + LOG_1X_BASE_C)

/* UMB Physical Layer AT Modulator for R-ACK (High) */
#define LOG_UMB_PHY_TX_ACK_HIGH_C                       (0x3F5 + LOG_1X_BASE_C)

/* UMB Physical Layer AT Modulator for R-ACK (Low) */
#define LOG_UMB_PHY_TX_ACK_LOW_C                        (0x3F6 + LOG_1X_BASE_C)

/* UMB Physical Layer AT Modulator for R-PICH */
#define LOG_UMB_PHY_TX_PICH_C                           (0x3F7 + LOG_1X_BASE_C)

/* UMB Physical Layer AT Modulator for R-ACH (Access) */
#define LOG_UMB_PHY_TX_ACH_C                            (0x3F8 + LOG_1X_BASE_C)

/* UMB Physical Layer AT Modulator for R-ODDCCH (High) */
#define LOG_UMB_PHY_TX_ODCCH_HIGH_C                     (0x3F9 + LOG_1X_BASE_C)

/* UMB Physical Layer AT Modulator for R-ODDCCH (Low) */
#define LOG_UMB_PHY_TX_ODCCH_LOW_C                      (0x3FA + LOG_1X_BASE_C)

/* UMB Physical Layer AT Modulator for R-CDCCH */
#define LOG_UMB_PHY_TX_RCDCCH_CONFIG_C                  (0x3FB + LOG_1X_BASE_C)

/* UMB Physical Layer AT Modulator for CQI sent on RCDCCH */
#define LOG_UMB_PHY_TX_NONFLSS_CQICH_C                  (0x3FC + LOG_1X_BASE_C)

/* UMB Physical Layer AT Modulator for CQI sent on RCDCCH */
#define LOG_UMB_PHY_TX_FLSS_CQICH_C                     (0x3FD + LOG_1X_BASE_C)

/* UMB Physical Layer AT Modulator for PACH sent on RCDCCH */
#define LOG_UMB_PHY_TX_PAHCH_C                          (0x3FE + LOG_1X_BASE_C)

/* UMB Physical Layer AT Modulator for REQ sent on RCDCCH */
#define LOG_UMB_PHY_TX_REQCH_C                          (0x3FF + LOG_1X_BASE_C)

/* UMB Physical Layer AT Modulator for PSD sent on RCDCCH */
#define LOG_UMB_PHY_TX_PSDCH_C                          (0x400 + LOG_1X_BASE_C)

/* UMB Physical Layer AT Modulator for R-DCH */
#define LOG_UMB_PHY_TX_DCH_C                            (0x401 + LOG_1X_BASE_C)

/* UMB Physical Layer Time/Frequency/RxPower Estimate */
#define LOG_UMB_PHY_RX_TIME_FREQ_POWER_ESTIMATE_C       (0x402 + LOG_1X_BASE_C)

/* UMB Physical Layer FLCS Processing */
#define LOG_UMB_PHY_RX_FLCS_PROCESSING_C                (0x403 + LOG_1X_BASE_C)

/* UMB Physical Layer PBCCH Processing */
#define LOG_UMB_PHY_RX_PBCCH_PROCESSING_C               (0x404 + LOG_1X_BASE_C)

/* UMB Physical Layer SBCCH Processing */
#define LOG_UMB_PHY_RX_SBCCH_PROCESSING_C               (0x405 + LOG_1X_BASE_C)

/* UMB Physical Layer QPCH Processing */
#define LOG_UMB_PHY_RX_QPCH_PROCESSING_C                (0x406 + LOG_1X_BASE_C)

/* UMB Physical Layer MRC Demodulated Data Symbols (Preamble SBCCH/QPCH) */
#define LOG_UMB_PHY_RX_SBCCH_DEMOD_C                    (0x407 + LOG_1X_BASE_C)

/* UMB Physical Layer MRC Demodulated Data Symbols (Preamble PBCCH) */
#define LOG_UMB_PHY_RX_PBCCH_DEMOD_C                    (0x408 + LOG_1X_BASE_C)

/* UMB Physical Layer VCQI */
#define LOG_UMB_PHY_RX_VCQI_C                           (0x409 + LOG_1X_BASE_C)

/* UMB Physical Layer Acquisition Algorithm */
#define LOG_UMB_PHY_RX_INITIAL_ACQUISITION_C            (0x40A + LOG_1X_BASE_C)

/* UMB Physical Layer Handoff Search Algorithm */
#define LOG_UMB_PHY_RX_HANDOFF_SEARCH_C                 (0x40B + LOG_1X_BASE_C)

/* UMB RF RFFE Configuration Info */
#define LOG_UMB_AT_RFFE_CONFG_C                         (0x40C + LOG_1X_BASE_C)

/* UMB RF Calibrated Values After Powerup */
#define LOG_UMB_AT_RFFE_RX_CALIB_C                      (0x40D + LOG_1X_BASE_C)

/* UMB RF AGC Block in Acquisition Mode */
#define LOG_UMB_AT_RFFE_RX_ACQ_C                        (0x40E + LOG_1X_BASE_C)

/* UMB RF AGC Block in Idle Mode */
#define LOG_UMB_AT_RFFE_RX_IDLE_C                       (0x40F + LOG_1X_BASE_C)

/* UMB RF AGC Block in Connected Mode */
#define LOG_UMB_AT_RFFE_RX_CONNECTED_C                  (0x410 + LOG_1X_BASE_C)

/* UMB RF AGC Block in Connected Mode (FTM) */
#define LOG_UMB_AT_RFFE_RX_CONNECTED_FTM_C              (0x411 + LOG_1X_BASE_C)

/* UMB RF Jammer Detector Functionality */
#define LOG_UMB_AT_RFFE_RX_JAMMER_DETECTOR_FUNCTIONALITY_C  (0x412 + LOG_1X_BASE_C)

/* UMB RF Jammer Detector Response */
#define LOG_UMB_AT_RFFE_RX_JAMMER_DETECTOR_RESPONSE_C   (0x413 + LOG_1X_BASE_C)

/* UMB RF RFFE TX Power Control */
#define LOG_UMB_AT_RFFE_TX_BETA_SCALING_C               (0x414 + LOG_1X_BASE_C)

/* UMB Searcher Dump */
#define LOG_UMB_SEARCHER_DUMP_C                         (0x415 + LOG_1X_BASE_C)

/* UMB System Acquire */
#define LOG_UMB_SYSTEM_ACQUIRE_C                        (0x416 + LOG_1X_BASE_C)

/* UMB Set Maintenance */
#define LOG_UMB_SET_MAINTENANCE_C                       (0x417 + LOG_1X_BASE_C)

/* UMB QPCH */
#define LOG_UMB_QPCH_C                                  (0x418 + LOG_1X_BASE_C)

/* UMB RLL Forward Partial RP Packet */
#define LOG_UMB_RLL_FORWARD_PARTIAL_RP_C                (0x419 + LOG_1X_BASE_C)

/* UMB RLL Reverse Partial RP Packet */
#define LOG_UMB_RLL_REVERSE_PARTIAL_RP_C                (0x41A + LOG_1X_BASE_C)

/* UMB RLL Forward Signal Packet */
#define LOG_UMB_RLL_FORWARD_SIGNAL_C                    (0x41B + LOG_1X_BASE_C)

/* UMB RLL Reverse Signal Packet */
#define LOG_UMB_RLL_REVERSE_SIGNAL_C                    (0x41C + LOG_1X_BASE_C)

/* UMB RLL Forward Statistics */
#define LOG_UMB_RLL_FORWARD_STATS_C                     (0x41D + LOG_1X_BASE_C)

/* UMB RLL Reverse Statistics */
#define LOG_UMB_RLL_REVERSE_STATS_C                     (0x41E + LOG_1X_BASE_C)

/* UMB RLL IRTP */
#define LOG_UMB_RLL_IRTP_C                              (0x41F + LOG_1X_BASE_C)

/* UMB AP Forward Link MAC Packets */
#define LOG_UMB_AP_FL_MAC_PACKET_C                      (0x420 + LOG_1X_BASE_C)

/* UMB AP Reverse Link MAC Packets */
#define LOG_UMB_AP_RL_MAC_PACKET_C                      (0x421 + LOG_1X_BASE_C)

/* GPS Performance Statistics log */
#define LOG_CGPS_PERFORMANCE_STATS_C                    (0x422 + LOG_1X_BASE_C)

/* UMB Searcher General Status */
#define LOG_UMB_SRCH_GENERAL_STATUS_C                   (0x423 + LOG_1X_BASE_C)

/* UMB Superframe Scheduler */
#define LOG_UMB_SUPERFRAME_SCHEDULER_C                  (0x424 + LOG_1X_BASE_C)

/* UMB Sector List */
#define LOG_UMB_SECTOR_LIST_C                           (0x425 + LOG_1X_BASE_C)

/* UMB MAC Access Attempt Command */
#define LOG_UMB_MAC_ACCESS_ATTEMPT_CMD_C                (0x426 + LOG_1X_BASE_C)

/* UMB MAC Access Probe Information */
#define LOG_UMB_MAC_ACCESS_PROBE_INFO_C                 (0x427 + LOG_1X_BASE_C)

/* UMB MAC RTCMAC Package Information */
#define LOG_UMB_MAC_RTCMAC_PKG_INFO_C                   (0x428 + LOG_1X_BASE_C)

/* UMB MAC Super Frame Information */
#define LOG_UMB_MAC_SI_INFO_C                           (0x429 + LOG_1X_BASE_C)

/* UMB MAC Quick Channel Information */
#define LOG_UMB_MAC_QCI_INFO_C                          (0x42A + LOG_1X_BASE_C)

/* UMB MAC Paging Id List */
#define LOG_UMB_MAC_PAGING_ID_LIST_C                    (0x42B + LOG_1X_BASE_C)

/* UMB MAC Quick Paging Channel Information */
#define LOG_UMB_MAC_QPCH_INFO_C                         (0x42C + LOG_1X_BASE_C)

/* UMB MAC FTCMAC Information */
#define LOG_UMB_MAC_FTCMAC_PKG_INFO_C                   (0x42D + LOG_1X_BASE_C)

/* UMB MAC Access Grant Receiving */
#define LOG_UMB_MAC_ACCESS_GRANT_C                      (0x42E + LOG_1X_BASE_C)

/* UMB MAC Generic Debug Log */
#define LOG_UMB_MAC_GEN_DEBUG_LOG_PKG_C                 (0x42F + LOG_1X_BASE_C)

/* CGPS Frequency Bias Estimate */
#define LOG_CGPS_MC_FREQ_BIAS_EST_C                     (0x430 + LOG_1X_BASE_C)

/* UMB MAC Request Report Information Log */
#define LOG_UMB_MAC_REQCH_REPORT_INFO_C                 (0x431 + LOG_1X_BASE_C)

/* UMB MAC Reverse Link QoS Token Bucket Information Log */
#define LOG_UMB_MAC_RLQOS_TOKEN_BUCKET_INFO_C           (0x432 + LOG_1X_BASE_C)

/* UMB MAC Reverse Link QoS Stream Information Log */
#define LOG_UMB_MAC_RLQOS_STREAM_INFO_C                 (0x433 + LOG_1X_BASE_C)

/* UMB MAC Reverse Link QoS Allotment Information Log */
#define LOG_UMB_MAC_RLQOS_ALLOTMENT_INFO_C              (0x434 + LOG_1X_BASE_C)

/* UMB Searcher Recent State Machine Transactions */
#define LOG_UMB_SRCH_STM_ACTIVITY_C                     (0x435 + LOG_1X_BASE_C)

/* Performance Counters on ARM11 Profiling Information */
#define LOG_ARM11_PERF_CNT_INFO_C                       (0x436 + LOG_1X_BASE_C)

/* The last defined DMSS log code */
#define LOG_1X_LAST_C                                 ((0x436) + LOG_1X_BASE_C)

/* This is only here for old (pre equipment ID update) logging code */
#define LOG_LAST_C ( LOG_1X_LAST_C & 0xFFF )

/* -------------------------------------------------------------------------
 * Log Equipment IDs.
 * The number is represented by 4 bits.
 * ------------------------------------------------------------------------- */
typedef enum {
  LOG_EQUIP_ID_OEM   = 0, /* 3rd party OEM (licensee) use */
  LOG_EQUIP_ID_1X    = 1, /* Traditional 1X line of products */
  LOG_EQUIP_ID_RSVD2 = 2,
  LOG_EQUIP_ID_RSVD3 = 3,
  LOG_EQUIP_ID_WCDMA = 4,
  LOG_EQUIP_ID_GSM   = 5,
  LOG_EQUIP_ID_LBS   = 6,
  LOG_EQUIP_ID_UMTS  = 7,
  LOG_EQUIP_ID_TDMA  = 8,
  LOG_EQUIP_ID_BOA   = 9,
  LOG_EQUIP_ID_DTV   = 10,
  LOG_EQUIP_ID_APPS  = 11,
  LOG_EQUIP_ID_DSP   = 12,

  LOG_EQUIP_ID_LAST_DEFAULT = LOG_EQUIP_ID_DSP

} log_equip_id_enum_type;

#define LOG_EQUIP_ID_MAX 0xF /* The equipment ID is 4 bits */

/* Note that these are the official values and are used by default in 
   diagtune.h.
*/
#define LOG_EQUIP_ID_0_LAST_CODE_DEFAULT 0 
#define LOG_EQUIP_ID_1_LAST_CODE_DEFAULT LOG_1X_LAST_C
#define LOG_EQUIP_ID_2_LAST_CODE_DEFAULT 0 
#define LOG_EQUIP_ID_3_LAST_CODE_DEFAULT 0 
#define LOG_EQUIP_ID_4_LAST_CODE_DEFAULT 0
#define LOG_EQUIP_ID_5_LAST_CODE_DEFAULT 0
#define LOG_EQUIP_ID_6_LAST_CODE_DEFAULT 0 
#define LOG_EQUIP_ID_7_LAST_CODE_DEFAULT 0
#define LOG_EQUIP_ID_8_LAST_CODE_DEFAULT 0
#define LOG_EQUIP_ID_9_LAST_CODE_DEFAULT 0 
#define LOG_EQUIP_ID_10_LAST_CODE_DEFAULT 0 
#define LOG_EQUIP_ID_11_LAST_CODE_DEFAULT 0 
#define LOG_EQUIP_ID_12_LAST_CODE_DEFAULT 0 
#define LOG_EQUIP_ID_13_LAST_CODE_DEFAULT 0 
#define LOG_EQUIP_ID_14_LAST_CODE_DEFAULT 0
#define LOG_EQUIP_ID_15_LAST_CODE_DEFAULT 0

#endif /* LOG_CODES_H */

