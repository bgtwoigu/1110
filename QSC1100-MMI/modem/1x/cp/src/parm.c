/*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*

          M O B I L E   C A I   P A R A M E T E R   S E R V I C E S

GENERAL DESCRIPTION
  These services maintain the set of parameters that can be retrieved and
  set by the Retrieve Parameters message, the Parameters Response message,
  and the Set Parameters message.

EXTERNALIZED FUNCTIONS
  parm_inc
    This function increments a specified parameter.
  parm_add
    This function adds a specified amount to a parameter.
  parm_set
    This function sets a specified parameter to a specified value.
  parm_get
    This function retrieves a specified parameter.
  parm_diag_retrieve
    This function retrieves all the parameters for diag.
  parm_zero_set
    This function zeros all the parameters except the sync channel time
  parm_get_fer
    This function retrieves the statistics necessary to calculate frame
    error rate.

INITIALIZATION AND SEQUENCING REQUIREMENTS
  None.

Copyright (c) 1999-2005 by QUALCOMM, Incorporated.  All Rights Reserved.

*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*/

/*===========================================================================

                        EDIT HISTORY FOR MODULE

This section contains comments describing changes made to the module.
Notice that changes are listed in reverse chronological order.

$PVCSPath: L:/src/asw/MSM5100/CP_REL_A/vcs/parm.c_v   1.12   03 Oct 2002 10:51:12   phosabet  $
$Header: //source/qcom/qct/modem/1x/cp/rel/1h08/src/parm.c#1 $ $DateTime: 2009/05/29 03:54:56 $ $Author: prabhuk $

when       who     what, where, why
--------   ---     ----------------------------------------------------------
14/04/08   bb      To include the msg.h file
21/03/08   bb      Fixed klocwork errors
05/01/06   an      Fixed Lint Errors
01/19/06   fc      Fixed the null pointer reference issue.
12/10/04   sb      Fixed lint errors.
07/01/04   va      Corrected lint errors
06/30/04   bkm     Another round of lint cleanup (changing function signatures!).
05/28/04   bkm     Merged the following from IS2000_REL_B_C:
  03/05/04   bkm     Lint cleanup.
04/06/04   bkm     Some initial Rel D updates.
01/20/04   bkm     Fixed compile error for when REL_C_DV is not defined.
08/27/03   yll     Fixed a compiling error using RVCT2.0.
08/11/03   bkm     Removed support for FEATURE_SPECIAL_MDR.
10/03/02   ph      Lint Cleanup
10/01/02   fc      Added BCCH, FCCCH and EACH BA mode parameter support.
04/01/02   yll     Added Retrievable and Settable parameters for BCCH and 
                   FCCCH.
02/12/02   yll     Merged from common archive CP VU 116
                   01/22/02   bn      
                   Added support for a counter of consecutive DTX frames
01/10/02   lh      Added MuxPDU 5 counters for release A.
05/29/01   ht      (kjn) Changed FEATURE_IS2000 to FEATURE_DS_IS2000
                   to save memory.
04/10/01   lcc     Added counters for DCCH frames.
03/01/01   lad     Removed special IS2000 processing.  Cleaned up diag support.
11/29/00   ak      Added some XTRA PARMS for LTU CRC checking.
08/16/00   lad     Fixed IS2000 and IS95B parm processing to allow disabling.
07/11/00   cah     Rid compiler warning from is2000_id in parm_diag_retrieve.
07/06/00   yll     Added feature defintion to remove compiler error.
06/28/00   lad     Introduce backward compatibility flag that, when enabled,
                   forces PARM DIAG support to use the old DIAG V7 method.
                   The feature is called FEATURE_DIAG_V7_PKT_IFACE.
06/26/00   ak      Fixed parm_is2000_info indexing.
06/19/00   lad     Made performance optimizations and added DIAG support
                   from diagp.c (removed legacy DIAG support).  Moved IS2000
                   parms to separate array to save 800 bytes of RAM/ROM.
06/12/00   ak      Fixed SCH0 statistics; was missing a gap.
06/09/00   ak      Added statistics for IS 2000 SCH 0 mux.
06/09/00   va      Added #ifndef MSM5000_IRAM_FWD around parm_inc function
08/16/99   jq      Cleared a compiler warning in VALID_XTRA_PARM define
03/30/99   ram     Verified and check-ed in changes made for MDR
                   DM support by aaj. parm_diag_get function now
                   always requires 10 parameters to accomodate MDR
                   support.
02/26/99   aaj     Modified parm_info structure to include MDR
                   supplemental channel parameters and one new
                   struct is created for sup channel FERs.
09/01/98   lcc     Merged in ARM support.
07/16/96   jmk     fixed: parm_get_fer wasn't including MUX2 info.
08/16/95   jmk     fixed: MUX2 parms weren't getting cleared in parm_zero_set
08/16/95   jmk     Fixed off-by-one MUX2 param bug, modified parm_diag_get
08/14/95   gb      Added MUX2 parameter support.
10/17/94   jca     Fixed bug in len check in function parm_diag_get().
05/03/93   jai     Added PN-3118 compliance and support for diag.
08/10/92   gb      Initial revision.

===========================================================================*/

/*===========================================================================

                     INCLUDE FILES FOR MODULE

===========================================================================*/

#include "comdef.h"
#include "qw.h"
#include "cai.h"
#include "parm.h"
#include "msg.h"
#if !defined (FEATURE_DIAG_V7_PKT_IFACE)
#include "diagbuf.h"
#include "diagcmd.h"
#include "diagpkt.h"
#include "ts.h"
#endif

/*===========================================================================

                DEFINITIONS AND DECLARATIONS FOR MODULE

This section contains definitions for constants, macros, types, variables
and other items needed by this module.

===========================================================================*/
/*lint -e818 */

typedef struct
{
  uint32  value; /* value of parameter */
  uint16  len;   /* number of bits in parameter */
  boolean set;   /* indicator of whether parameter is settable */
} parm_struct_type;

/* Array of parameters - Note that parameters are stored as uint32 if they
   are 32 bits or less.  The translation services will correctly mask the
   number of bits for each parameter and the procedures which diag uses also
   mask the correct number of bits */
parm_struct_type parm_info[CAI_LAST_PARM_ID + 1] =
{ {   0, 0, FALSE},                     /* 0 */
  {   0, CAI_MUX1_REV_1_LEN, TRUE },
  {   0, CAI_MUX1_REV_2_LEN, TRUE },
  {   0, CAI_MUX1_REV_3_LEN, TRUE },
  {   0, CAI_MUX1_REV_4_LEN, TRUE },
  {   0, CAI_MUX1_REV_5_LEN, TRUE },
  {   0, CAI_MUX1_REV_6_LEN, TRUE },
  {   0, CAI_MUX1_REV_7_LEN, TRUE },
  {   0, CAI_MUX1_REV_8_LEN, TRUE },
  {   0, 0, FALSE },
  {   0, 0, FALSE },                    /* 10 */
  {   0, CAI_MUX1_REV_11_LEN, TRUE },
  {   0, CAI_MUX1_REV_12_LEN, TRUE },
  {   0, CAI_MUX1_REV_13_LEN, TRUE },
  {   0, CAI_MUX1_REV_14_LEN, TRUE },
  {   0, CAI_MUX1_FOR_1_LEN, TRUE },
  {   0, CAI_MUX1_FOR_2_LEN, TRUE },
  {   0, CAI_MUX1_FOR_3_LEN, TRUE },
  {   0, CAI_MUX1_FOR_4_LEN, TRUE },
  {   0, CAI_MUX1_FOR_5_LEN, TRUE },
  {   0, CAI_MUX1_FOR_6_LEN, TRUE },    /* 20 */
  {   0, CAI_MUX1_FOR_7_LEN, TRUE },
  {   0, CAI_MUX1_FOR_8_LEN, TRUE },
  {   0, CAI_MUX1_FOR_9_LEN, TRUE },
  {   0, CAI_MUX1_FOR_10_LEN, TRUE },
  {   0, CAI_MUX1_FOR_11_LEN, TRUE },
  {   0, CAI_MUX1_FOR_12_LEN, TRUE },
  {   0, CAI_MUX1_FOR_13_LEN, TRUE },
  {   0, CAI_MUX1_FOR_14_LEN, TRUE },
  {   0, CAI_PAG_1_LEN,     TRUE },
  {   0, CAI_PAG_2_LEN,     TRUE },     /* 30 */
  {   0, CAI_PAG_3_LEN,     TRUE },
  {   0, CAI_PAG_4_LEN,     TRUE },
  {   0, CAI_PAG_5_LEN,     TRUE },
  {   0, CAI_PAG_6_LEN,     TRUE },
  {   0, CAI_PAG_7_LEN,     TRUE },
  {   0, CAI_ACC_1_LEN,     TRUE },
  {   0, CAI_ACC_2_LEN,     TRUE },
  {   0, CAI_ACC_3_LEN,     TRUE },
  {   0, CAI_ACC_4_LEN,     TRUE },
  {   0, CAI_ACC_5_LEN,     TRUE },     /* 40 */
  {   0, CAI_ACC_6_LEN,     TRUE },
  {   0, CAI_ACC_7_LEN,     TRUE },
  {   0, CAI_ACC_8_LEN,     TRUE },
  {   0, CAI_LAYER2_RTC1_LEN, TRUE },
  {   0, CAI_LAYER2_RTC2_LEN, TRUE },
  {   0, CAI_LAYER2_RTC3_LEN, TRUE },
  {   0, CAI_LAYER2_RTC4_LEN, TRUE },
  {   0, CAI_LAYER2_RTC5_LEN, TRUE },
  {   0, 0, FALSE }, /* CAI_OTH_SYS_TIME_ID is > 32 bits in length.
                        Use parm_oth_sys_time */
  {   0, CAI_MUX2_REV_1_LEN, TRUE },    /* 50 */
  {   0, CAI_MUX2_REV_2_LEN, TRUE },
  {   0, CAI_MUX2_REV_3_LEN, TRUE },
  {   0, CAI_MUX2_REV_4_LEN, TRUE },
  {   0, CAI_MUX2_REV_5_LEN, TRUE },
  {   0, CAI_MUX2_REV_6_LEN, TRUE },
  {   0, CAI_MUX2_REV_7_LEN, TRUE },
  {   0, CAI_MUX2_REV_8_LEN, TRUE },
  {   0, CAI_MUX2_REV_9_LEN, TRUE },
  {   0, CAI_MUX2_REV_10_LEN, TRUE },
  {   0, CAI_MUX2_REV_11_LEN, TRUE },   /* 60 */
  {   0, CAI_MUX2_REV_12_LEN, TRUE },
  {   0, CAI_MUX2_REV_13_LEN, TRUE },
  {   0, CAI_MUX2_REV_14_LEN, TRUE },
  {   0, CAI_MUX2_REV_15_LEN, TRUE },
  {   0, CAI_MUX2_REV_16_LEN, TRUE },
  {   0, CAI_MUX2_REV_17_LEN, TRUE },
  {   0, CAI_MUX2_REV_18_LEN, TRUE },
  {   0, CAI_MUX2_REV_19_LEN, TRUE },
  {   0, CAI_MUX2_REV_20_LEN, TRUE },
  {   0, CAI_MUX2_REV_21_LEN, TRUE },   /* 70 */
  {   0, CAI_MUX2_REV_22_LEN, TRUE },
  {   0, CAI_MUX2_REV_23_LEN, TRUE },
  {   0, CAI_MUX2_REV_24_LEN, TRUE },
  {   0, CAI_MUX2_REV_25_LEN, TRUE },
  {   0, 0, FALSE },
  {   0, CAI_MUX2_FOR_1_LEN, TRUE },
  {   0, CAI_MUX2_FOR_2_LEN, TRUE },
  {   0, CAI_MUX2_FOR_3_LEN, TRUE },
  {   0, CAI_MUX2_FOR_4_LEN, TRUE },
  {   0, CAI_MUX2_FOR_5_LEN, TRUE },    /* 80 */
  {   0, CAI_MUX2_FOR_6_LEN, TRUE },
  {   0, CAI_MUX2_FOR_7_LEN, TRUE },
  {   0, CAI_MUX2_FOR_8_LEN, TRUE },
  {   0, CAI_MUX2_FOR_9_LEN, TRUE },
  {   0, CAI_MUX2_FOR_10_LEN, TRUE },
  {   0, CAI_MUX2_FOR_11_LEN, TRUE },
  {   0, CAI_MUX2_FOR_12_LEN, TRUE },
  {   0, CAI_MUX2_FOR_13_LEN, TRUE },
  {   0, CAI_MUX2_FOR_14_LEN, TRUE },
  {   0, CAI_MUX2_FOR_15_LEN, TRUE },   /* 90 */
  {   0, CAI_MUX2_FOR_16_LEN, TRUE },
  {   0, CAI_MUX2_FOR_17_LEN, TRUE },
  {   0, CAI_MUX2_FOR_18_LEN, TRUE },
  {   0, CAI_MUX2_FOR_19_LEN, TRUE },
  {   0, CAI_MUX2_FOR_20_LEN, TRUE },
  {   0, CAI_MUX2_FOR_21_LEN, TRUE },
  {   0, CAI_MUX2_FOR_22_LEN, TRUE },
  {   0, CAI_MUX2_FOR_23_LEN, TRUE },
  {   0, CAI_MUX2_FOR_24_LEN, TRUE },
  {   0, CAI_MUX2_FOR_25_LEN, TRUE },   /* 100 */
#if !defined(FEATURE_DS_IS2000) && \
    !defined(FEATURE_IS95B_MDR) && \
    !defined(FEATURE_IS2000_REL_A)
  {   0, CAI_MUX2_FOR_26_LEN, TRUE } };
#else
  {   0, CAI_MUX2_FOR_26_LEN, TRUE },
  {   0, CAI_SUPP1_REV_S_LEN, TRUE },           /* IS95B MDR parameters */
  {   0, CAI_SUPP1_REV_P_LEN, TRUE },
  {   0, CAI_SUPP2_REV_S_LEN, TRUE },
  {   0, CAI_SUPP2_REV_P_LEN, TRUE },
  {   0, CAI_SUPP3_REV_S_LEN, TRUE },
  {   0, CAI_SUPP3_REV_P_LEN, TRUE },
  {   0, CAI_SUPP4_REV_S_LEN, TRUE },
  {   0, CAI_SUPP4_REV_P_LEN, TRUE },
  {   0, CAI_SUPP5_REV_S_LEN, TRUE },   /* 110 */
  {   0, CAI_SUPP5_REV_P_LEN, TRUE },
  {   0, CAI_SUPP6_REV_S_LEN, TRUE },
  {   0, CAI_SUPP6_REV_P_LEN, TRUE },
  {   0, CAI_SUPP7_REV_S_LEN, TRUE },
  {   0, CAI_SUPP7_REV_P_LEN, TRUE },
  {   0, CAI_SUPP1_FWD_S_LEN, TRUE },
  {   0, CAI_SUPP1_FWD_P_LEN, TRUE },
  {   0, CAI_SUPP2_FWD_S_LEN, TRUE },
  {   0, CAI_SUPP2_FWD_P_LEN, TRUE },
  {   0, CAI_SUPP3_FWD_S_LEN, TRUE },   /* 120 */
  {   0, CAI_SUPP3_FWD_P_LEN, TRUE },
  {   0, CAI_SUPP4_FWD_S_LEN, TRUE },
  {   0, CAI_SUPP4_FWD_P_LEN, TRUE },
  {   0, CAI_SUPP5_FWD_S_LEN, TRUE },
  {   0, CAI_SUPP5_FWD_P_LEN, TRUE },
  {   0, CAI_SUPP6_FWD_S_LEN, TRUE },
  {   0, CAI_SUPP6_FWD_P_LEN, TRUE },
  {   0, CAI_SUPP7_FWD_S_LEN, TRUE },

#if !defined(FEATURE_DS_IS2000) && !defined(FEATURE_IS2000_REL_A)
  {   0, CAI_SUPP7_FWD_P_LEN, TRUE } }; /* 129 */
#else
  {   0, CAI_SUPP7_FWD_P_LEN, TRUE },

  { 0, 0, FALSE }, { 0, 0, FALSE }, { 0, 0, FALSE }, /* 130-132 */
  { 0, 0, FALSE }, { 0, 0, FALSE }, { 0, 0, FALSE }, /* 133-135 */
  { 0, 0, FALSE }, { 0, 0, FALSE }, { 0, 0, FALSE }, /* 136-138 */
  { 0, 0, FALSE }, { 0, 0, FALSE }, { 0, 0, FALSE }, /* 139-141 */
  { 0, 0, FALSE },                                   /* 142 */
  {   0, CAI_PARM_LEN_24, TRUE },   // CAI_MUX1_REV_DCCH_1_ID  143
  {   0, CAI_PARM_LEN_24, TRUE },   // CAI_MUX1_REV_DCCH_2_ID  144
  {   0, CAI_PARM_LEN_24, TRUE },   // CAI_MUX1_REV_DCCH_3_ID  145
  {   0, CAI_PARM_LEN_24, TRUE },   // CAI_MUX1_REV_DCCH_4_ID  146
  {   0, CAI_PARM_LEN_24, TRUE },   // CAI_MUX1_REV_DCCH_5_ID  147
  {0, 0, FALSE }, {0, 0, FALSE }, {0, 0, FALSE }, {0, 0, FALSE }, {0, 0, FALSE }, //148-152
  {   0, CAI_PARM_LEN_24, TRUE },   // CAI_MUX1_REV_DCCH_11_ID 153
  {   0, CAI_PARM_LEN_24, TRUE },   // CAI_MUX1_REV_DCCH_12_ID 154
  {   0, CAI_PARM_LEN_24, TRUE },   // CAI_MUX1_REV_DCCH_13_ID 155
  {   0, CAI_PARM_LEN_24, TRUE },   // CAI_MUX1_REV_DCCH_14_ID 156
  {   0, CAI_PARM_LEN_24, TRUE },   // CAI_MUX1_REV_DCCH_15_ID 157
  {0, 0, FALSE }, // 158

  {   0, CAI_PARM_LEN_24, TRUE },   // CAI_MUX1_FOR_DCCH_1_ID  159
  {   0, CAI_PARM_LEN_24, TRUE },   // CAI_MUX1_FOR_DCCH_2_ID  160
  {   0, CAI_PARM_LEN_24, TRUE },   // CAI_MUX1_FOR_DCCH_3_ID  161
  {   0, CAI_PARM_LEN_24, TRUE },   // CAI_MUX1_FOR_DCCH_4_ID  162
  {   0, CAI_PARM_LEN_24, TRUE },   // CAI_MUX1_FOR_DCCH_5_ID  163
  {0, 0, FALSE }, {0, 0, FALSE }, {0, 0, FALSE }, {0, 0, FALSE }, //164-167
  {   0, CAI_PARM_LEN_24, TRUE },   // CAI_MUX1_FOR_DCCH_10_ID 168
  {   0, CAI_PARM_LEN_24, TRUE },   // CAI_MUX1_FOR_DCCH_11_ID 169
  {   0, CAI_PARM_LEN_24, TRUE },   // CAI_MUX1_FOR_DCCH_12_ID 170
  {   0, CAI_PARM_LEN_24, TRUE },   // CAI_MUX1_FOR_DCCH_13_ID 171
  {   0, CAI_PARM_LEN_24, TRUE },   // CAI_MUX1_FOR_DCCH_14_ID 172
  {   0, CAI_PARM_LEN_24, TRUE },   // CAI_MUX1_FOR_DCCH_15_ID 173
  {0, 0, FALSE }, // 174

  {   0, CAI_PARM_LEN_24, TRUE },   // CAI_MUX2_REV_DCCH_1_ID  175
  {   0, CAI_PARM_LEN_24, TRUE },   // CAI_MUX2_REV_DCCH_2_ID  176
  {   0, CAI_PARM_LEN_24, TRUE },   // CAI_MUX2_REV_DCCH_3_ID  177
  {   0, CAI_PARM_LEN_24, TRUE },   // CAI_MUX2_REV_DCCH_4_ID  178
  {   0, CAI_PARM_LEN_24, TRUE },   // CAI_MUX2_REV_DCCH_5_ID  179
  {   0, CAI_PARM_LEN_24, TRUE },   // CAI_MUX2_REV_DCCH_6_ID  180
  {   0, CAI_PARM_LEN_24, TRUE },   // CAI_MUX2_REV_DCCH_7_ID  181
  {   0, CAI_PARM_LEN_24, TRUE },   // CAI_MUX2_REV_DCCH_8_ID  182
  {   0, CAI_PARM_LEN_24, TRUE },   // CAI_MUX2_REV_DCCH_9_ID  183
  {   0, CAI_PARM_LEN_24, TRUE },   // CAI_MUX2_REV_DCCH_10_ID 184
  {0, 0, FALSE }, {0, 0, FALSE }, {0, 0, FALSE }, {0, 0, FALSE }, //185-188
  {0, 0, FALSE }, {0, 0, FALSE }, {0, 0, FALSE }, {0, 0, FALSE }, //189-192
  {0, 0, FALSE }, {0, 0, FALSE }, {0, 0, FALSE }, {0, 0, FALSE }, //193-196
  {0, 0, FALSE }, {0, 0, FALSE }, {0, 0, FALSE }, {0, 0, FALSE }, //197-200
  {   0, CAI_PARM_LEN_24, TRUE },   // CAI_MUX2_REV_DCCH_27_ID 201
  {0, 0, FALSE }, // 202

  {   0, CAI_PARM_LEN_24, TRUE },   // CAI_MUX2_FOR_DCCH_1_ID  203
  {   0, CAI_PARM_LEN_24, TRUE },   // CAI_MUX2_FOR_DCCH_2_ID  204
  {   0, CAI_PARM_LEN_24, TRUE },   // CAI_MUX2_FOR_DCCH_3_ID  205
  {   0, CAI_PARM_LEN_24, TRUE },   // CAI_MUX2_FOR_DCCH_4_ID  206
  {   0, CAI_PARM_LEN_24, TRUE },   // CAI_MUX2_FOR_DCCH_5_ID  207
  {   0, CAI_PARM_LEN_24, TRUE },   // CAI_MUX2_FOR_DCCH_6_ID  208
  {   0, CAI_PARM_LEN_24, TRUE },   // CAI_MUX2_FOR_DCCH_7_ID  209
  {   0, CAI_PARM_LEN_24, TRUE },   // CAI_MUX2_FOR_DCCH_8_ID  210
  {   0, CAI_PARM_LEN_24, TRUE },   // CAI_MUX2_FOR_DCCH_9_ID  211
  {   0, CAI_PARM_LEN_24, TRUE },   // CAI_MUX2_FOR_DCCH_10_ID 212
  {0, 0, FALSE }, {0, 0, FALSE }, {0, 0, FALSE }, {0, 0, FALSE }, //213-216
  {0, 0, FALSE }, {0, 0, FALSE }, {0, 0, FALSE }, {0, 0, FALSE }, //217-220
  {0, 0, FALSE }, {0, 0, FALSE }, {0, 0, FALSE }, {0, 0, FALSE }, //221-224
  {0, 0, FALSE }, {0, 0, FALSE }, {0, 0, FALSE },                 //225-227
  {   0, CAI_PARM_LEN_24, TRUE },   // CAI_MUX2_FOR_DCCH_26_ID 228
  {   0, CAI_PARM_LEN_24, TRUE },   // CAI_MUX2_FOR_DCCH_27_ID 229
  {0, 0, FALSE }, // 230

  {   0, CAI_SCH0_REV_1X_LEN, TRUE },    /* 231 */
  {   0, CAI_SCH0_REV_2X_LEN, TRUE },
  {   0, CAI_SCH0_REV_4X_LEN, TRUE },
  {   0, CAI_SCH0_REV_8X_LEN, TRUE },
  {   0, CAI_SCH0_REV_16X_LEN,TRUE },
  {   0, CAI_SCH0_REV_LTU_LEN,TRUE },    /* 236 */

  { 0, 0, FALSE }, { 0, 0, FALSE }, { 0, 0, FALSE }, /* 237-239 */
  { 0, 0, FALSE }, { 0, 0, FALSE }, { 0, 0, FALSE }, /* 240-242 */
  { 0, 0, FALSE }, { 0, 0, FALSE },                  /* 243-244 */

  {   0, CAI_SCH0_FWD_1X_LEN, TRUE },    /* 245 */
  {   0, CAI_SCH0_FWD_2X_LEN, TRUE },
  {   0, CAI_SCH0_FWD_4X_LEN, TRUE },
  {   0, CAI_SCH0_FWD_8X_LEN, TRUE },
  {   0, CAI_SCH0_FWD_16X_LEN, TRUE },
  {   0, CAI_SCH0_FWD_LTU_LEN, TRUE },   /* 250 */
  {   0, CAI_SCH0_FWD_LTUOK_LEN, TRUE }

#ifdef FEATURE_IS2000_REL_A
  , { 0, 0, FALSE }, { 0, 0, FALSE }, { 0, 0, FALSE }, { 0, 0, FALSE }, /* 255 */                /* 255 */
  { 0, 0, FALSE }, { 0, 0, FALSE }, { 0, 0, FALSE }, 
 
#ifdef FEATURE_IS2000_REL_A_CC
  { 0, CAI_BCCH_1_LEN, TRUE }, 
  { 0, CAI_BCCH_2_LEN, TRUE },          /* 260 */
  { 0, CAI_BCCH_3_LEN, TRUE }, 
  { 0, CAI_BCCH_4_LEN, TRUE }, 
  { 0, CAI_BCCH_5_LEN, TRUE }, 
  { 0, CAI_FCCCH_1_LEN, TRUE }, 
  { 0, CAI_FCCCH_2_LEN, TRUE },         /* 265 */ 
  { 0, CAI_FCCCH_3_LEN, TRUE }, 
  { 0, CAI_FCCCH_4_LEN, TRUE },         /* 267 */ 
#else
  { 0, 0, FALSE }, { 0, 0, FALSE },     /* 260 */ 
  { 0, 0, FALSE }, { 0, 0, FALSE }, { 0, 0, FALSE }, { 0, 0, FALSE }, { 0, 0, FALSE }, /* 265 */
  { 0, 0, FALSE }, { 0, 0, FALSE }, 
#endif /* FEATURE_IS2000_REL_A_CC */

  { 0, 0, FALSE }, { 0, 0, FALSE }, { 0, 0, FALSE }, /* 270 */
  { 0, 0, FALSE }, { 0, 0, FALSE }, { 0, 0, FALSE }, { 0, 0, FALSE }, { 0, 0, FALSE },
  { 0, 0, FALSE }, { 0, 0, FALSE }, { 0, 0, FALSE }, { 0, 0, FALSE }, { 0, 0, FALSE }, /* 280 */
  { 0, 0, FALSE }, { 0, 0, FALSE }, { 0, 0, FALSE }, { 0, 0, FALSE }, { 0, 0, FALSE },
  { 0, 0, FALSE }, { 0, 0, FALSE }, { 0, 0, FALSE }, { 0, 0, FALSE }, { 0, 0, FALSE }, /* 290 */
  { 0, 0, FALSE }, { 0, 0, FALSE }, { 0, 0, FALSE }, { 0, 0, FALSE }, { 0, 0, FALSE },
  { 0, 0, FALSE }, { 0, 0, FALSE }, { 0, 0, FALSE }, { 0, 0, FALSE }, { 0, 0, FALSE }, /* 300 */
  
  { 0, 0, FALSE }, { 0, 0, FALSE }, { 0, 0, FALSE }, { 0, 0, FALSE }, { 0, 0, FALSE },
  { 0, 0, FALSE }, { 0, 0, FALSE }, { 0, 0, FALSE }, { 0, 0, FALSE }, { 0, 0, FALSE }, /* 310 */
  { 0, 0, FALSE }, { 0, 0, FALSE }, { 0, 0, FALSE }, { 0, 0, FALSE }, { 0, 0, FALSE },
  { 0, 0, FALSE }, { 0, 0, FALSE }, { 0, 0, FALSE }, { 0, 0, FALSE }, { 0, 0, FALSE }, /* 320 */
  { 0, 0, FALSE }, { 0, 0, FALSE }, { 0, 0, FALSE }, { 0, 0, FALSE }, { 0, 0, FALSE },
  { 0, 0, FALSE }, { 0, 0, FALSE }, { 0, 0, FALSE }, { 0, 0, FALSE }, { 0, 0, FALSE }, /* 330 */
  { 0, 0, FALSE }, 

  { 0, CAI_RSCH0_1_LEN, TRUE },         /* 332 */
  { 0, CAI_RSCH0_2_LEN, TRUE }, 
  { 0, CAI_RSCH0_3_LEN, TRUE }, 
  { 0, CAI_RSCH0_4_LEN, TRUE },         /* 335 */
  { 0, CAI_RSCH0_5_LEN, TRUE }, 
  { 0, CAI_RSCH0_6_LEN, TRUE }, 
  { 0, CAI_RSCH0_7_LEN, TRUE },         /* 338 */
  
  { 0, 0, FALSE }, { 0, 0, FALSE },     /* 340 */
  { 0, 0, FALSE }, { 0, 0, FALSE }, { 0, 0, FALSE }, { 0, 0, FALSE }, { 0, 0, FALSE },
  { 0, 0, FALSE }, { 0, 0, FALSE }, 
  
  { 0, CAI_RSCH0_LTU_LEN, TRUE },       /* 348 */
  
  { 0, 0, FALSE }, { 0, 0, FALSE },     /* 350 */
  { 0, 0, FALSE }, { 0, 0, FALSE }, { 0, 0, FALSE }, { 0, 0, FALSE }, { 0, 0, FALSE },
  { 0, 0, FALSE }, { 0, 0, FALSE }, { 0, 0, FALSE }, { 0, 0, FALSE }, { 0, 0, FALSE }, /* 360 */
  { 0, 0, FALSE }, { 0, 0, FALSE }, { 0, 0, FALSE }, { 0, 0, FALSE }, { 0, 0, FALSE },
  { 0, 0, FALSE }, { 0, 0, FALSE }, { 0, 0, FALSE }, { 0, 0, FALSE }, { 0, 0, FALSE }, /* 370 */
  { 0, 0, FALSE }, { 0, 0, FALSE }, { 0, 0, FALSE }, { 0, 0, FALSE }, { 0, 0, FALSE },
  { 0, 0, FALSE }, { 0, 0, FALSE }, { 0, 0, FALSE }, { 0, 0, FALSE }, { 0, 0, FALSE }, /* 380 */
  { 0, 0, FALSE }, { 0, 0, FALSE }, { 0, 0, FALSE }, { 0, 0, FALSE }, { 0, 0, FALSE },
  { 0, 0, FALSE }, { 0, 0, FALSE }, { 0, 0, FALSE }, { 0, 0, FALSE }, { 0, 0, FALSE }, /* 390 */
  { 0, 0, FALSE }, { 0, 0, FALSE }, { 0, 0, FALSE }, { 0, 0, FALSE }, { 0, 0, FALSE },
  { 0, 0, FALSE }, { 0, 0, FALSE }, { 0, 0, FALSE }, { 0, 0, FALSE }, { 0, 0, FALSE }, /* 400 */
  { 0, 0, FALSE }, { 0, 0, FALSE }, { 0, 0, FALSE }, { 0, 0, FALSE }, { 0, 0, FALSE },
  { 0, 0, FALSE }, { 0, 0, FALSE }, { 0, 0, FALSE }, { 0, 0, FALSE }, { 0, 0, FALSE }, /* 410 */
  { 0, 0, FALSE }, { 0, 0, FALSE }, { 0, 0, FALSE }, { 0, 0, FALSE }, { 0, 0, FALSE },
  { 0, 0, FALSE }, { 0, 0, FALSE }, { 0, 0, FALSE }, { 0, 0, FALSE }, { 0, 0, FALSE }, /* 420 */
  { 0, 0, FALSE }, { 0, 0, FALSE }, { 0, 0, FALSE }, { 0, 0, FALSE }, { 0, 0, FALSE },
  { 0, 0, FALSE }, { 0, 0, FALSE }, { 0, 0, FALSE }, { 0, 0, FALSE }, 
  
  { 0, CAI_FSCH0_1_LEN, TRUE },        /* 430 */
  { 0, CAI_FSCH0_2_LEN, TRUE }, 
  { 0, CAI_FSCH0_3_LEN, TRUE }, 
  { 0, CAI_FSCH0_4_LEN, TRUE }, 
  { 0, CAI_FSCH0_5_LEN, TRUE }, 
  { 0, CAI_FSCH0_6_LEN, TRUE },
  { 0, CAI_FSCH0_7_LEN, TRUE },        /* 436 */

  { 0, 0, FALSE },{ 0, 0, FALSE }, { 0, 0, FALSE }, { 0, 0, FALSE },                   /* 440 */
  { 0, 0, FALSE }, { 0, 0, FALSE }, { 0, 0, FALSE }, { 0, 0, FALSE }, { 0, 0, FALSE },
  
  { 0, CAI_FSCH0_LTU_LEN, TRUE },      /* 446 */
  { 0, CAI_FSCH0_LTUOK_LEN, TRUE }     /* 447 */

#ifdef FEATURE_IS2000_REL_A_CC
  , { 0, 0, FALSE }, { 0, 0, FALSE }, { 0, 0, FALSE }, /* 450 */
  { 0, 0, FALSE }, { 0, 0, FALSE }, { 0, 0, FALSE }, { 0, 0, FALSE }, { 0, 0, FALSE }, 
  { 0, 0, FALSE }, { 0, 0, FALSE }, { 0, 0, FALSE }, { 0, 0, FALSE }, { 0, 0, FALSE }, /* 460 */
  { 0, 0, FALSE }, { 0, 0, FALSE }, { 0, 0, FALSE }, { 0, 0, FALSE }, { 0, 0, FALSE }, 
  { 0, 0, FALSE }, { 0, 0, FALSE }, { 0, 0, FALSE }, { 0, 0, FALSE }, 
  { 0, CAI_EACH_BA_1_LEN, TRUE }, /* 470 */
  { 0, CAI_EACH_BA_2_LEN, TRUE }, 
  { 0, CAI_EACH_BA_3_LEN, TRUE }, 
  { 0, CAI_EACH_BA_4_LEN, TRUE }, 
  { 0, CAI_EACH_BA_5_LEN, TRUE }, 
  { 0, CAI_EACH_BA_6_LEN, TRUE }, /* 475 */ 
  { 0, CAI_EACH_BA_7_LEN, TRUE }, 
  { 0, CAI_EACH_BA_8_LEN, TRUE }, 
  { 0, 0, FALSE }, { 0, 0, FALSE }, { 0, 0, FALSE }, /* 480 */
  { 0, 0, FALSE }, { 0, 0, FALSE }, { 0, 0, FALSE }, { 0, 0, FALSE }, { 0, 0, FALSE }, 
  { 0, 0, FALSE }, { 0, 0, FALSE }, { 0, 0, FALSE }, { 0, 0, FALSE }, { 0, 0, FALSE }, /* 490 */
  { 0, 0, FALSE }, { 0, 0, FALSE },  
  { 0, CAI_BCCH_6_LEN, TRUE },  /* 493 */
  { 0, CAI_BCCH_7_LEN, TRUE }, 
  { 0, CAI_BCCH_8_LEN, TRUE },  /* 495 */
  { 0, CAI_BCCH_9_LEN, TRUE },
  { 0, 0, FALSE }, { 0, 0, FALSE }, 
  { 0, CAI_FCCCH_5_LEN, TRUE },  
  { 0, CAI_FCCCH_6_LEN, TRUE }, /* 500 */
  { 0, CAI_FCCCH_7_LEN, TRUE }, 
  { 0, CAI_FCCCH_8_LEN, TRUE }, 
  { 0, CAI_FCCCH_9_LEN, TRUE }, 
  { 0, CAI_FCCCH_10_LEN, TRUE }, 
  { 0, CAI_FCCCH_11_LEN, TRUE }, /* 505 */ 
#ifdef FEATURE_IS2000_REL_C_DV
#error code not present
#endif /* FEATURE_IS2000_REL_C_DV */

#endif /* FEATURE_IS2000_REL_A_CC */

#endif /* FEATURE_IS2000_REL_A */

  };
                    
#endif /* !defined(FEATURE_IS2000) && !defined(FEATURE_IS2000_REL_A) */

#endif /* !defined(FEATURE_IS95B_MDR) &&
          !defined(FEATURE_IS2000) &&
          !defined(FEATURE_IS2000_REL_A) */

#if defined (FEATURE_IS2000) || defined (FEATURE_IS95B_MDR) 
/* IS95B does not define CAI paramters for maintaining sup channel
   FER statistics.  But we need FER on sup channels for debugging
   and testing purposes. Hence we have defined new CAI paramters
   and new parm_info struct here */

/* Same holds true for IS95C */

parm_struct_type parm_xtra_info[] =
{
#if defined (FEATURE_IS95B_MDR)
  {   0, CAI_XTRA_SUPP1_FWD_ERR_LEN, TRUE }    /* Error frames on sup channel 1 */
 ,{   0, CAI_XTRA_SUPP2_FWD_ERR_LEN, TRUE }
 ,{   0, CAI_XTRA_SUPP3_FWD_ERR_LEN, TRUE }
 ,{   0, CAI_XTRA_SUPP4_FWD_ERR_LEN, TRUE }
 ,{   0, CAI_XTRA_SUPP5_FWD_ERR_LEN, TRUE }
 ,{   0, CAI_XTRA_SUPP6_FWD_ERR_LEN, TRUE }
 ,{   0, CAI_XTRA_SUPP7_FWD_ERR_LEN, TRUE }
#else
  { 0, 0, FALSE }
 ,{ 0, 0, FALSE }
 ,{ 0, 0, FALSE }
 ,{ 0, 0, FALSE }
 ,{ 0, 0, FALSE }
 ,{ 0, 0, FALSE }
 ,{ 0, 0, FALSE }
#endif /* (FEATURE_IS95B_MDR) */
#if defined (FEATURE_IS2000)
 ,{   0, CAI_XTRA_SCH0_FWD_SDU_ERR_LEN,   TRUE }
 ,{   0, CAI_XTRA_SCH0_FWD_SDU_DTX_LEN,   TRUE }
 ,{   0, CAI_XTRA_SCH0_FWD_BLANK_PDU_LEN, TRUE }
 ,{   0, CAI_XTRA_SCH0_REV_SDU_ERR_LEN,   TRUE }
 ,{   0, CAI_XTRA_SCH0_REV_SDU_DTX_LEN,   TRUE }
 ,{   0, CAI_XTRA_SCH0_REV_BLANK_PDU_LEN, TRUE }
 ,{   0, CAI_XTRA_SCH0_FWD_BAD_LTUS_LEN,  TRUE }
 ,{   0, CAI_XTRA_SCH0_FWD_SAVED_LTUS_LEN,TRUE }
 ,{   0, CAI_XTRA_SCH0_REV_SDU_CON_DTX_LEN,TRUE }
#endif /* !defined(FEATURE_IS2000) */
#if defined (FEATURE_IS2000_REL_A)
 ,{   0, CAI_XTRA_SCH0_FWD_BAD_MUXPDU5_LTUS_LEN,   TRUE }
 ,{   0, CAI_XTRA_SCH0_FWD_SAVED_MUXPDU5_LTUS_LEN, TRUE }
#endif
};
#endif /* (FEATURE_IS95B_MDR) || (FEATURE_IS2000) */

uint64 parm_oth_sys_time;
  /* Storage for Sync Channel message time parameter */

/*-------------------------------------------------------------------------*/

/* MACROS */
/* Macro to determine if parameter id is valid */
#define VALID_PARM( parm_id ) \
( (parm_id >= CAI_FIRST_PARM_ID ) && \
  (parm_id <= CAI_LAST_PARM_ID  ) && \
  (parm_info[ parm_id].len != 0 ) )

#if defined(FEATURE_IS2000) || defined (FEATURE_IS95B_MDR) 
#define VALID_XTRA_PARM( xtra_parm_id ) \
(((xtra_parm_id > CAI_XTRA_FIRST_PARM_ID) || \
  (xtra_parm_id == CAI_XTRA_FIRST_PARM_ID) \
 ) && \
 (xtra_parm_id <= CAI_XTRA_LAST_PARM_ID  ) && \
 (parm_xtra_info[ xtra_parm_id].len != 0 ) )
#endif /* (FEATURE_IS95B_MDR) || (FEATURE_SPECIAL_MDR) */

#define PARM_16_BITS 0x0000FFFF
  /* mask for retrieving 16 bits from uint32 */
#define PARM_24_BITS 0x00FFFFFF
  /* mask to retrieve 24 bits from uint32 */

/*===========================================================================

FUNCTION PARM_SET

DESCRIPTION
  This function sets a specified set of parameters.

DEPENDENCIES
  None.

RETURN VALUE
  PARM_DONE_S if the parameter set is ok.
  PARM_INV_S if a parameter id is out of the valid range.
  PARM_NOSET_S if the a parameter is not a settable parameter.

SIDE EFFECTS
  If the return parameter is PARM_DONE_S,  the parameters are set.  Otherwise
  the invalid parameter is returned where parm_id_ptr is pointed to.

===========================================================================*/
uint16 parm_set
(
  parm_set_type *set_ptr,
    /* parameter set to verify */
  uint16 *parm_id_ptr
    /* pointer to where to place 1st invalid parameter if parameter set is
       found to be invalid */
)
{
  unsigned int i; /* index through parameters */
  uint16 status;  /* status to return to calling procedure */
  uint16 id = 0;
  unsigned int num_parms;

  num_parms = set_ptr->num_parms;

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  status = PARM_DONE_S; /* initialize return status */

  /* Check to make sure all parameters can be set */
  for (i = 0; i < set_ptr->num_parms; i++)
  {
    id = set_ptr->parm[i].id;

    if ( !VALID_PARM( id ) )
    {
      status = PARM_INV_S; /* invalid parameter */
      break;
    }
    else if ( VALID_PARM( id ) && !parm_info[id].set )
    {
      status = PARM_NOSET_S;   /* parameter is not settable */
      break;
    }
  }/* for all parms in set command */

  if( status != PARM_DONE_S )
  {
    if (parm_id_ptr != NULL)
    {
      /* i is still pointing to the invalid parameter */
      *parm_id_ptr = id;
    }
  }
  else
  {
    /* Go ahead and set parameters.
    ** Note: It is assumed at this point that the parms are all valid.
    */
    for( i = 0; i < num_parms; i++ )
    {
      id = set_ptr->parm[i].id;

      if ( VALID_PARM( id ) )
      {
        parm_info[id].value = set_ptr->parm[i].value;
      }
    }
  }
  return( status );
 
} /* parm_set */

/*===========================================================================

FUNCTION PARM_STORE_SYNC_TIME

DESCRIPTION
  This function sets the Sync Channel time.

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  Sync channel time is stored in parm_oth_sys_time.

===========================================================================*/
void parm_store_sync_time
(
  qword sync_time
    /* Sync Channel time */
)
{
  parm_oth_sys_time = QW_CVT_Q2N(sync_time); //lint !e740 !e826 unusual pointer cast
  
} /* parm_store_sync_time */

/*===========================================================================

FUNCTION PARM_RET_SYNC_TIME

DESCRIPTION
  This function retrieves the Sync Channel time.

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  parm_oth_sys_time, stored from the most recent sync channel message,
  is stored in the location pointed to by sync_time

===========================================================================*/
void parm_ret_sync_time
(
  qword sync_time
    /* Sync Channel time */
)
{
  QW_CVT_Q2N(sync_time) = parm_oth_sys_time; //lint !e740 !e826 
  
} /* parm_ret_sync_time */

/*===========================================================================

FUNCTION PARM_GET

DESCRIPTION
  This function gets a specified set of parameters.

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  If a parameter in the set cannot be retrieved its length will be set to 0.
  Otherwise the parameter length and value will be returned.
===========================================================================*/
void parm_get
(
  parm_get_type *parm_ptr
    /* Pointer to set of parameters to get */
)
{
 
  int i;       /* Index through parameters */
  uint16 id;


/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  /* check for parameter validity */
  for( i = 0; i < parm_ptr->num_parms; i++ )
  {
    id = parm_ptr->parm[i].id;

    if( VALID_PARM( id ) )
    {
      parm_ptr->parm[i].len = parm_info[id].len;

      /* All params must be at most 32 bits.
      ** Any larger parameters must be handled separately.
      */
      parm_ptr->parm[i].value = parm_info[id].value;
    }
    else
    {
      /* Return length zero for unretrievable parameter */
      parm_ptr->parm[i].len = 0;
    }
  } /* for all parms requested */
 
} /* parm_get */

#if !defined (FEATURE_DIAG_V7_PKT_IFACE)
/*===========================================================================

FUNCTION PARM_DIAG_RETRIEVE

DESCRIPTION
  This function gets the requested range of parameters for a DIAG request.
  It also handles legacy DIAG parameter get commands.

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.
===========================================================================*/
diag_rsp_type * parm_diag_retrieve (
  diag_req_type *req_pkt_ptr,  /* pointer to request packet  */
  uint16 pkt_len               /* length of request packet   */
)
{
  uint64 time_stamp;  /* time stamp  */
  int i;        /* counter     */
  int j = 0;    /* Array index */
  diag_parm_retrieve_req_type *req_ptr = (diag_parm_retrieve_req_type *) req_pkt_ptr;
  diag_parm_retrieve_rsp_type *rsp_ptr = NULL; /* Pointer to response buffer */
  uint16 rsp_len = 0;            /* Length of response buffer  */

  /* Local vars to speed up access (avoid using PACKED diag pkt) */
  uint8                      cmd_code  = req_ptr->cmd_code;
  uint16                     start_id  = 0xFFFF;
  uint16                     end_id    = 0xFFFF;
  diag_parm_subcmd_enum_type subcmd = (diag_parm_subcmd_enum_type) 0xFF;

  /* First, we need to figure out how large our response is. */
  /* Use local vars to allow optimization of reads. */
  subcmd   = (diag_parm_subcmd_enum_type) req_ptr->subcmd;
  start_id = req_ptr->start_parm_id;
  end_id   = req_ptr->end_parm_id;

  /* Perform boundary checks */
  switch (subcmd)
  {
    case DIAG_PARM_STANDARD_RANGE_C:
      /* No need to process further.  Generate response here. */
      rsp_ptr = (diag_parm_retrieve_rsp_type *) diagbuf_pkt_alloc((uint16) FPOS(diag_parm_retrieve_rsp_type, time)); //lint !e413 use of NULL in FPOS

	  /* break from here if rsp_ptr is NULL */
	  if(rsp_ptr == NULL)
	  {
		 break;
	  }

      /* Copy the command code */
      rsp_ptr->cmd_code = cmd_code;
      rsp_ptr->subcmd = subcmd; /*lint !e641 */
      rsp_ptr->start_parm_id = CAI_FIRST_PARM_ID;
      rsp_ptr->end_parm_id = CAI_LAST_PARM_ID;

      break;

#if defined(FEATURE_DS_IS2000) || defined(FEATURE_IS95B_MDR) 
    case DIAG_PARM_EXTRA_RANGE_C:
      /* No need to process further.  Generate response here. */
      rsp_ptr = (diag_parm_retrieve_rsp_type *) diagbuf_pkt_alloc((uint16)FPOS(diag_parm_retrieve_rsp_type, time)); //lint !e413 use of NULL in FPOS

	  /* break from here if rsp_ptr is NULL */
	  if(rsp_ptr == NULL)
	  {
		 break;
	  }

      /* Copy the command code */
      rsp_ptr->cmd_code = cmd_code;
      rsp_ptr->subcmd = subcmd; /*lint !e641 */
      rsp_ptr->start_parm_id = CAI_XTRA_FIRST_PARM_ID;
      rsp_ptr->end_parm_id   = CAI_XTRA_LAST_PARM_ID;

      break;
#endif

    case DIAG_PARM_STANDARD_C:
      if ( start_id <= end_id &&
#if (CAI_FIRST_PARM_ID > 0) /* Don't check if first ID is 0 */
           start_id >= CAI_FIRST_PARM_ID &&
#endif
           end_id <= CAI_LAST_PARM_ID
         )
      {
        rsp_len = (uint16) FPOS(diag_parm_retrieve_rsp_type, parm[0]) +
                           FSIZ(diag_parm_retrieve_rsp_type, parm[0]) *
                           (end_id - start_id + 1); //lint !e734 !e834 precision loss
      }

      break;

#if defined(FEATURE_DS_IS2000) || defined(FEATURE_IS95B_MDR) 
    case DIAG_PARM_EXTRA_C:
      if ( start_id <= end_id &&
#if (CAI_XTRA_FIRST_PARM_ID > 0) /* Don't check if first ID is 0 */

           start_id >= CAI_XTRA_FIRST_PARM_ID &&
#endif
           end_id <= CAI_XTRA_LAST_PARM_ID)
      {
        rsp_len = (uint16) FPOS(diag_parm_retrieve_rsp_type, parm[0]) +
                           FSIZ(diag_parm_retrieve_rsp_type, parm[0]) *
                           (end_id - start_id + 1); //lint !e734 !e834 precision loss
      }

      break;
#endif /* FEATURE_IS95B_MDR || FEATURE_IS2000 */

    default:
      /* Unsupported request. rsp_len is 0, which will generate error packet. */
      break;
  }

  /* Now that we have determined packet length, lets get some PARMS! */
  if (rsp_ptr == NULL)
  {
    if (rsp_len)
    {
      /* Note: If we get this far, we can assume request values are valid */

      rsp_ptr = (diag_parm_retrieve_rsp_type *) diagbuf_pkt_alloc(rsp_len);

	  //check the validity of rsp_ptr
	  if (rsp_ptr == NULL)
	  {
		  return ((diag_rsp_type *) NULL);
	  }

      /* Get system time.  Use 64 bit types to be more efficient. */
      ts_get( QW_CVT_N2Q(&time_stamp) ); /*lint !e740*/

      INTLOCK();  /* get all statistics at same time */

      /* Now fill in the parm data. */
      switch (subcmd)
      {
        case DIAG_PARM_STANDARD_C:
          for (i = start_id; i <= end_id; i++)
          {
            if( VALID_PARM(i) )
            {
              rsp_ptr->parm[j++] =
                ( ( parm_info[i].len == 24 ) ?
                  ( parm_info[i].value & PARM_24_BITS ) :
                  ( parm_info[i].value & PARM_16_BITS ) );
            }
            else
            {
              rsp_ptr->parm[j++] = 0xFFFFFFFFUL; /* Set to -1 */
            }

          }

          break;

        case DIAG_PARM_EXTRA_C:
          for (i = start_id; i <= end_id; i++)
          {
  #if defined(FEATURE_DS_IS2000) || defined(FEATURE_IS95B_MDR) 
            if( VALID_XTRA_PARM(i) )
            {
              rsp_ptr->parm[j++] =
                ( ( parm_xtra_info[i].len == 24 ) ?
                  ( parm_xtra_info[i].value & PARM_24_BITS ) :
                  ( parm_xtra_info[i].value & PARM_16_BITS ) );
            }
            else
  #endif
            {
              rsp_ptr->parm[j++] = 0xFFFFFFFFUL;
            }
          }
          break;

        /* Default case is handled above */
      }  //lint !e787 DIAG_PARM_STANDARD_RANGE_C and DIAG_PARM_EXTRA_RANGE_C not used

      INTFREE();

      /* Copy the command code */
      rsp_ptr->cmd_code = cmd_code;

      rsp_ptr->time = time_stamp;

      /* Copy request parameters to the response (out of intlock) */
      rsp_ptr->start_parm_id = start_id;
      rsp_ptr->end_parm_id   = end_id;
      rsp_ptr->subcmd        = subcmd; /*lint !e641 */

    } /* if valid request */
    else
    {
      /* An invalid parameter was requested. */
      rsp_ptr = (diag_parm_retrieve_rsp_type *) ERR_PKT(DIAG_BAD_PARM_F);
    }
  }

  return ((diag_rsp_type *) rsp_ptr);
 
} /* parm_diag_retrieve */ //lint !e715 pkt_len not referenced

/*===========================================================================

FUNCTION PARM_DIAG_LEGACY_GET

DESCRIPTION
  This function supports legacy diagnostic commands for getting parms.
  It also handles legacy DIAG parameter get commands.

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/
diag_rsp_type * parm_diag_legacy_get (
  diag_req_type *req_ptr,      /* pointer to request packet  */
  uint16 pkt_len               /* length of request packet   */
)
{
  uint64 time_stamp;                   /* time stamp  */
  int i;                         /* counter     */

  diag_parm_get2_rsp_type *parm_get2_rsp_ptr = NULL;
#if defined(FEATURE_IS95B_MDR)
  diag_parm_get_is95b_rsp_type *parm_get_is95b_rsp_ptr = NULL;
#endif
   /* Pointer to response buffer */
  uint16 rsp_len = 0;            /* Length of response buffer  */

  /* Local var to speed up access (avoid using PACKED diag pkt) */
  uint8 cmd_code = req_ptr->cmd_code;

  /* First, we need to figure out how large our response is */
  switch (cmd_code)
  {
    /* The first two cases are legacy cases. */
    case DIAG_PARM_GET2_F:

      rsp_len = sizeof(diag_parm_get2_rsp_type);
      break;

#if defined(FEATURE_IS95B_MDR)
    case DIAG_PARM_GET_IS95B_F:
      rsp_len = sizeof(diag_parm_get_is95b_rsp_type);
      break;
#endif

    default:
      /* rsp_len is default 0 */
      break;
  }

  /* Now that we have determined packet length, lets get some PARMS! */
  if (rsp_len)
  {
    /* Note: If we get this far, we can assume request values are valid */

    parm_get2_rsp_ptr = (diag_parm_get2_rsp_type *) diagbuf_pkt_alloc(rsp_len);

#if defined(FEATURE_IS95B_MDR)
    parm_get_is95b_rsp_ptr = (diag_parm_get_is95b_rsp_type *) parm_get2_rsp_ptr; //lint !e740 !e826 
#endif

	/* return from here if parm_get2_rsp_ptr is NULL */
	if(parm_get2_rsp_ptr == NULL)
	{
		MSG_HIGH ("parm_get2_rsp_ptr is NULL",0,0,0);
		return (NULL);
	}

    /* Get system time.  Use 64 bit types to be more efficient. */
    ts_get( QW_CVT_N2Q(&time_stamp) ); /*lint !e740*/

    INTLOCK();  /* get all statistics at same time */

    switch (cmd_code)
    {
      /*-----------------------------------------------------------
        The first two cases are legacy cases.  Each legacy PARM
        is an addition to the previous definition.  Therefore,
        we start with the most recent and work our way down.
      -----------------------------------------------------------*/
#if defined(FEATURE_IS95B_MDR)
      case DIAG_PARM_GET_IS95B_F:

		  //do not process anything if parm_get_is95b_rsp_ptr is NULL 
		  if(parm_get_is95b_rsp_ptr == NULL)
		  {
			  MSG_HIGH ("parm_get_is95b_rsp_ptr is NULL",0,0,0);
			  return((diag_rsp_type *) parm_get2_rsp_ptr);
		  }

        for (i = 0; i < 7; i++) {
          parm_get_is95b_rsp_ptr->sup_for[i].secondary =
            parm_info[ CAI_SUPP1_FWD_S_ID + 2*i ].value & PARM_24_BITS;

          parm_get_is95b_rsp_ptr->sup_for[i].primary =
            parm_info[ CAI_SUPP1_FWD_S_ID + 2*(i+1) ].value & PARM_24_BITS;

          parm_get_is95b_rsp_ptr->sup_rev[i].secondary =
            parm_info[ CAI_SUPP1_REV_S_ID + 2*i ].value & PARM_24_BITS;

          parm_get_is95b_rsp_ptr->sup_rev[i].primary =
            parm_info[ CAI_SUPP1_REV_S_ID + 2*(i+1) ].value & PARM_24_BITS;

        }

        /* Now fill in the rest of the packet, which is the same as PARM_GET2 */
        /*lint -fallthrough */

#endif /* FEATURE_IS95B_MDR */

      case DIAG_PARM_GET2_F:

	    /* retrieve forward traffic channel statistics - they are all 24 bits */
        for( i = 0; i < 14; i++ )
        {
          parm_get2_rsp_ptr->mux_for[i] =
            parm_info[ CAI_MUX1_FOR_1_ID + i].value & PARM_24_BITS;
        }

        /* retrieve reverse traffic channel statistics - they are all 24 bits */
        for( i = 0; i < 14; i++ )
        {
          parm_get2_rsp_ptr->mux_rev[i] =
            parm_info[ CAI_MUX1_REV_1_ID + i].value & PARM_24_BITS;
        }

        /* retrieve Paging channel statistics */
        for( i = 0; i < PARM_NUM_PAG; i++ ) /*lint !e834 */
        {
          /* Paging Channel parameters are either 24 or 16 bits */
          parm_get2_rsp_ptr->pag[i] =
            ( ( parm_info[ CAI_PAG_1_ID + i].len == 24 ) ?
              ( parm_info[ CAI_PAG_1_ID + i].value & PARM_24_BITS ) :
              ( parm_info[ CAI_PAG_1_ID + i].value & PARM_16_BITS ) );
        }

        /* retrieve Access channel statistics - they are all 16 bits */
        for( i = 0; i < PARM_NUM_ACC; i++ ) /*lint !e834 */
        {
          parm_get2_rsp_ptr->acc[i] =
            parm_info[ CAI_ACC_1_ID + i].value & PARM_16_BITS;
        }

        /* retrieve Layer2 channel statistics - they are all 16 bits */
        for( i = 0; i < PARM_NUM_LAYER2; i++ ) /*lint !e834 */
        {
          parm_get2_rsp_ptr->layer2[i] = parm_info[ CAI_LAYER2_RTC1_ID + i].value & PARM_16_BITS;
        }

        parm_get2_rsp_ptr->sys_time = parm_oth_sys_time;

        for( i = 0; i < 26; i++ )
        {
          parm_get2_rsp_ptr->mux2_for[i] =
            parm_info[ CAI_MUX2_FOR_1_ID + i ].value & PARM_24_BITS;
        }

        for( i = 0; i < 26; i++ )
        {
          parm_get2_rsp_ptr->mux2_rev[i] =
            parm_info[ CAI_MUX2_REV_1_ID + i ].value & PARM_24_BITS;
        }

        break; /* Done with legacy processing */
    } //lint !e744 no default

    INTFREE();
	
    /* Copy the command code and the time stamp (recorded earlier) */
    parm_get2_rsp_ptr->cmd_code = cmd_code;

    parm_get2_rsp_ptr->time = time_stamp;

  } /* if valid request */
  else
  {
    /* An invalid parameter was requested. */
    parm_get2_rsp_ptr = (diag_parm_get2_rsp_type *) ERR_PKT(DIAG_BAD_PARM_F);
  }

  return ((diag_rsp_type *) parm_get2_rsp_ptr); /*lint !e826 */
 
} /* parm_diag_legacy_get */  //lint !e715 pkt_len not used

/*===========================================================================

FUNCTION PARM_DIAG_SET

DESCRIPTION
  This procedure processes a parameter set request. Item -1 means set
  all to 0.  Note that this routine DOES NOT call parm_set_verify before
  setting.  This bypasses the resettability restrictions. You have been
  warned.

DEPENDENCIES
  None.

RETURN VALUE
  Pointer to response packet.

SIDE EFFECTS
  None.

============================================================================*/
#define PARM_DIAG_ZERO_SET 0xFFFF
diag_rsp_type * parm_diag_set (
  diag_req_type *req_pkt_ptr,  /* pointer to request packet  */
  uint16 pkt_len               /* length of request packet   */
)
{
  uint64                      time_stamp;
  parm_set_type               set;
  uint16                      stat;
  uint16                      bogus_id;
  int i;
  diag_parm_set_req_type *req_ptr = (diag_parm_set_req_type *) req_pkt_ptr;
  diag_parm_set_rsp_type *rsp_ptr;
  const int rsp_len = sizeof(diag_parm_set_rsp_type);

  ts_get( QW_CVT_N2Q(&time_stamp) ); /*lint !e740*/

  if ( req_ptr->parm_id != PARM_DIAG_ZERO_SET )
  {
    set.num_parms = 1;
    set.parm[0].id = req_ptr->parm_id;
    set.parm[0].value = req_ptr->parm_value;
    stat = parm_set ( &set, &bogus_id );

    if ( stat != PARM_DONE_S )
    {
      return ( (diag_rsp_type *)ERR_PKT( DIAG_BAD_PARM_F ) );
    }
  }
  else
  {
    /* Set all PARMS to 0 */
    INTLOCK();  /* set them all at the same time - even if they are invalid */
    for( i = CAI_FIRST_PARM_ID; i <= CAI_LAST_PARM_ID; i++ )
    {
      parm_info[i].value = 0;
    }

#if defined(FEATURE_DS_IS2000) || defined(FEATURE_IS95B_MDR) 
    for( i = CAI_XTRA_FIRST_PARM_ID; i <= CAI_XTRA_LAST_PARM_ID; i++ )
    {
        if( parm_xtra_info[i].set )    /* Check if the parm is settable */
        {
            parm_xtra_info[i].value = 0;
        }/* if */
    }/* for */
#endif /* FEATURE_IS2000 || FEATURE_IS95B_MDR */
    INTFREE();
  }

  rsp_ptr = (diag_parm_set_rsp_type *) diagbuf_pkt_alloc(rsp_len);

  //check the rsp_ptr value
  if(rsp_ptr == NULL)
  {
	  MSG_HIGH("rsp_ptr: diag memory out of range",0,0,0);
	  return(NULL);
  }

  rsp_ptr->time = time_stamp;

  rsp_ptr->cmd_code = DIAG_PARM_SET_F;

  return ((diag_rsp_type *) rsp_ptr); /*lint !e826 */
 
}  /* parm_diag_set */ //lint !e715 pkt_len not used

#if defined(FEATURE_IS95B_MDR)
/*===========================================================================
FUNCTION PARM_DIAG_SUP_FER

DESCRIPTION
  This returns the Frame Error Rates for each channel (0-7).  Channel 0
  is the fundamental channel, 1-7 are supplemental channels (used for MDR).

DEPENDENCIES

RETURN VALUE
  Pointer to response packet.

SIDE EFFECTS
  None.
===========================================================================*/
diag_rsp_type * parm_diag_sup_fer (
  diag_req_type *req_pkt_ptr,  /* pointer to request packet  */
  word pkt_len                 /* length of request packet   */
)
{
  int           i;  /* counter */
  parm_fer_type fer_data;
  diag_sup_fer_req_type *req_ptr = (diag_sup_fer_req_type *) req_pkt_ptr;
  diag_sup_fer_rsp_type *rsp_ptr;
  const int rsp_len = sizeof(diag_sup_fer_rsp_type);

  /* Get parameter FER data */
  parm_get_fer ( &fer_data );

  rsp_ptr = (diag_sup_fer_rsp_type *) diagbuf_pkt_alloc(rsp_len);

  //check the rsp_ptr value
  if(rsp_ptr == NULL)
  {
	  MSG_HIGH("rsp_ptr: diag memory out of range",0,0,0);
	  return ((diag_rsp_type *) rsp_ptr);
  }

  /*------------------------------------------------------
   Copy fundamental channel information into response.
  ------------------------------------------------------*/
  rsp_ptr->total_frames[0] = fer_data.total_frames;
  rsp_ptr->bad_frames[0]   = fer_data.bad_frames;

  /*-----------------------------------------------------
   Calculate supplemental channel information.
  ------------------------------------------------------*/
  for( i = 0; i < CAI_MAX_NUM_SUP; i++ )
  {
      /* for each value of "i" we offset the parm_info by 2*i so as to get only
         the primary traffic part of frame number counting. We do not support
         secondary traffic */

      rsp_ptr->total_frames[i+1] = parm_info[ CAI_SUPP1_FWD_P_ID+2*i].value;
      /* total frames received on each sup channel */

      /* for xtra params we do not have separate primary and secondary
         error counts and hence we increment by "i" instead of "2*i" */

      rsp_ptr->bad_frames[i+1] = parm_xtra_info[ CAI_XTRA_SUPP1_FWD_ERR_ID+i].value;
      /* erasure frames on each sup channel */
  }

  rsp_ptr->cmd_code = req_ptr->cmd_code;

  /* Return the length of the packet. */
  return ((diag_rsp_type *) rsp_ptr);  /*lint !e826 */
 
} /* parm_diag_sup_fer */ //lint !e715 pkt_len not used
#endif /* defined(FEATURE_IS95B_MDR) */

#else /* FEATURE_DIAG_V7_PKT_IFACE */
/*===========================================================================

FUNCTION PARM_ZERO_SET

DESCRIPTION
  This function sets all of the settable and retrievable parameters to
  zero EXCEPT the sync channel time.

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.
===========================================================================*/

void parm_zero_set( void )
{
  word i;     /* used to index through parameters */

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  INTLOCK();  /* set them all at the same time - even if they are invalid */
  for( i= CAI_FIRST_PARM_ID;  i<= CAI_LAST_PARM_ID; i++ )
  {
    parm_info[i].value = 0;
  }

#if defined(FEATURE_DS_IS2000) || defined(FEATURE_IS95B_MDR)
  for( i = CAI_XTRA_FIRST_PARM_ID; i <= CAI_XTRA_LAST_PARM_ID; i++ )
  {
      if( parm_xtra_info[ i ].set )    /* Check if the parm is settable */
      {
          parm_xtra_info[i].value = 0;
      }/* if */
  }/* for */
#endif /* FEATURE_IS95B_MDR || FEATURE_IS2000 */
  INTFREE();

}/* parm_zero_set */

/*===========================================================================

FUNCTION PARM_DIAG_GET

DESCRIPTION
  This function gets the parameter sets for the diag task.

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.
===========================================================================*/

void parm_diag_get
(
  PACKED dword *for_tc_ptr,
    /* pointer to array to place Forward Traffic channel statistics in */
  PACKED dword *rev_tc_ptr,
    /* pointer to array to place Reverse Traffic channel statistics in */
  PACKED dword *pag_ptr,
    /* pointer to array to place Paging channel statistics in */
  PACKED dword *acc_ptr,
    /* pointer to array to place Access channel statistics in */
  PACKED dword *layer2_ptr,
    /* pointer to array to place Layer 2 statistics in */
  qword sys_time_ptr,
    /* pointer to qword to place system timer parameter in */
  PACKED dword *for_tc2_ptr,
    /* pointer to array to place MUX2 Forward Traffic channel statistics in */
  PACKED dword *rev_tc2_ptr,
    /* pointer to array to place MUX2 Reverse Traffic channel statistics in */
  PACKED dword *for_sup_ch_ptr,
    /* pointer to array to place SUPP Forward Traffic channel statistics in*/
  PACKED dword *rev_sup_ch_ptr
    /* pointer to array to place SUPP Reverse Traffic channel statistics in*/
)
{
  word i;  /* index through parameters */

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  INTLOCK();  /* get all statistics at same time */
  if( for_tc_ptr != NULL )
  {
    /* retrieve forward traffic channel statistics - they are all 24 bits */
    for( i=0; i< 14; i++ )
    {
      for_tc_ptr[i] = parm_info[ CAI_MUX1_FOR_1_ID + i].value & PARM_24_BITS;
    }
  }

  if( rev_tc_ptr != NULL )
  {
    /* retrieve reverse traffic channel statistics - they are all 24 bits */
    for( i=0; i< 14; i++ )
    {
      rev_tc_ptr[i] = parm_info[ CAI_MUX1_REV_1_ID + i].value & PARM_24_BITS;
    }
  }

  if( pag_ptr != NULL )
  {
    /* retrieve Paging channel statistics */
    for( i=0; i< PARM_NUM_PAG; i++ )
    {
      /* Paging Channel parameters are either 24 or 16 bits */
      pag_ptr[i] =  ( ( parm_info[ CAI_PAG_1_ID + i].len == 24 ) ?
                      ( parm_info[ CAI_PAG_1_ID + i].value & PARM_24_BITS ) :
                      ( parm_info[ CAI_PAG_1_ID + i].value & PARM_16_BITS ) );

    }
  }

  if( acc_ptr != NULL )
  {
    /* retrieve Access channel statistics - they are all 16 bits */
    for( i=0; i< PARM_NUM_ACC; i++ )
    {
      acc_ptr[i] =  parm_info[ CAI_ACC_1_ID + i].value & PARM_16_BITS;
    }
  }

  if( layer2_ptr != NULL )
  {
    /* retrieve Layer2 channel statistics - they are all 16 bits */
    for( i=0; i< PARM_NUM_LAYER2; i++ )
    {
      layer2_ptr[i] = parm_info[ CAI_LAYER2_RTC1_ID + i].value & PARM_16_BITS;
    }
  }

  if( sys_time_ptr != NULL )
  {
    qw_equ( sys_time_ptr, (unsigned long *) &parm_oth_sys_time );
  }

  if( for_tc2_ptr != NULL )
  {
    for( i=0; i<26; i++ )
    {
      for_tc2_ptr[i] = parm_info[ CAI_MUX2_FOR_1_ID + i ].value & PARM_24_BITS;
    }
  }

  if( rev_tc2_ptr != NULL )
  {
    for( i=0; i< 26; i++ )
    {
      rev_tc2_ptr[i] = parm_info[ CAI_MUX2_REV_1_ID + i ].value & PARM_24_BITS;
    }
  }

#if defined (FEATURE_IS95B_MDR)
  if (for_sup_ch_ptr != NULL) {
    for (i=0; i < 14; i++) {
      for_sup_ch_ptr[i] = parm_info[ CAI_SUPP1_FWD_S_ID + i ].value
                          & PARM_24_BITS;
    }
  }

  if (rev_sup_ch_ptr != NULL) {
    for (i=0; i < 14; i++) {
      rev_sup_ch_ptr[i] = parm_info[ CAI_SUPP1_REV_S_ID + i ].value
                          & PARM_24_BITS;
    }
  }
#endif /* defined (FEATURE_IS95B_MDR) */

  INTFREE();
 
} /* parm_diag_get */
#endif /* !defined (FEATURE_DIAG_V7_PKT_IFACE) */

/*===========================================================================

FUNCTION PARM_GET_FER

DESCRIPTION
  This function retrieves the total number of forward traffic channel frames
  received (both MUX1 and MUX2) and the number of "bad" frames
  ( number of MUX1 category 9 and 10 and MUX2 category 26 frames ).

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.
===========================================================================*/
void parm_get_fer
(
  parm_fer_type *data_ptr
    /* pointer to structure to put statistics in */
)
{
  uint16 i;  /* index for forward traffic channel statistics */

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  /* sum up number of frames received on the Forward traffic channel,
  ** both MUX1 and MUX2 frames */
  data_ptr->total_frames = 0;
  for( i=0; i< PARM_NUM_FOR_TC_MUX1; i++ ) /*lint !e834 */
  {
    data_ptr->total_frames+=
        ( parm_info[ CAI_MUX1_FOR_1_ID + i ].value & PARM_24_BITS );
  }
  for( i=0; i< PARM_NUM_FOR_TC_MUX2; i++ ) /*lint !e834 */
  {
    data_ptr->total_frames+=
        ( parm_info[ CAI_MUX2_FOR_1_ID + i ].value & PARM_24_BITS );
  }

  data_ptr->bad_frames = parm_info[ CAI_MUX1_FOR_9_ID ].value +
                         parm_info[ CAI_MUX1_FOR_10_ID ].value +
                         parm_info[ CAI_MUX2_FOR_26_ID ].value;
 
} /* parm_get_fer */

#ifndef MSM5000_IRAM_FWD
/*===========================================================================

FUNCTION PARM_INC

DESCRIPTION
  This function increments the value of a specified parameter.

DEPENDENCIES
  None.

RETURN VALUE
  PARM_DONE_S if the parameter is set.
  PARM_INV_S if the input parameter id is out of the valid range.

SIDE EFFECTS
  None.
===========================================================================*/
uint16 parm_inc
(
  uint16 parm_id,
    /* ID of parameter found in IS-95 */
  uint8  inc
    /* Amount to increment parameter by */
)
{
  uint16 status; /* status to return to calling procedure */

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  /* check for parameter validity */
  if ( !VALID_PARM( parm_id ) )

  {
    status = PARM_INV_S;  /* parameter out of range */
  }
  else
  {
    if( parm_id == CAI_OTH_SYS_TIME_ID )
    {
      parm_oth_sys_time += inc;
    }
    else
    {
      parm_info[parm_id].value += inc;
    }

    status = PARM_DONE_S;
  }

  return( status );

}/* parm_inc */
#endif //MSM5000_IRAM_FWD

#if defined (FEATURE_DS_IS2000) || defined (FEATURE_IS95B_MDR) 
/*===========================================================================

FUNCTION PARM_XTRA_INC

DESCRIPTION
  This function increments the value of a specified xtra parameter.

DEPENDENCIES
  None.

RETURN VALUE
  PARM_DONE_S if the parameter is set.
  PARM_INV_S if the input parameter id is out of the valid range.

SIDE EFFECTS
  None.
===========================================================================*/
uint16 parm_xtra_inc
(
  uint16 xtra_parm_id,
    /* ID of parameter found in IS-95 */
  uint8  inc
    /* Amount to increment parameter by, if 0xFF, then reset parameter to zero */
)
{
  uint16 status;    /* status to return to calling procedure */

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  /* check for parameter validity */
  if ( !VALID_XTRA_PARM( xtra_parm_id ) )
  {
    status = PARM_INV_S;  /* parameter out of range */
  }
  else
  {
    if (inc == 0xFF) /* reset the value to Zero */
    {
      parm_xtra_info[ xtra_parm_id ].value = 0;
    }
    else
    {
      parm_xtra_info[ xtra_parm_id ].value += inc;
    }
    status = PARM_DONE_S;
  }
  return( status );

}/* parm_xtra_inc */

#endif /* FEATURE_IS95B_MDR || FEATURE_IS2000 */

/*lint +e818 */
