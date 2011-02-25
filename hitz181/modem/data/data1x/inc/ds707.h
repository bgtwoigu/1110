#ifndef DS707_H
#define DS707_H
/*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*
 
                              D S 7 0 7

GENERAL DESCRIPTION
  This is the general header file for the CDMA/1X/IS-707 data module.  This
  contains macros, typedefs, and #defines which are used throughout data.
  
EXTERNALIZED MACROS
  DS_ISDATA_RS1_PRE707()
    Is the given Service option rateset 1 and considered part of the pre-707
    service option set?
    
  DS_ISDATA_RS1_IS707()
    Is the given Service option rateset 1 and considered part of the 707
    service option set?
    
  DS_ISDATA_RS2_PRE707()
    Is the given Service option rateset 2 and considered part of the pre-707
    service option set?
    
  DS_ISDATA_RS2_QC_PROPTRY()
    Is the given Service option rateset 2 and considered part of the Qualcomm
    proprietary service option set?
    
  DS_ISDATA_IS707()
    Is the given Service option considered part of the 707 service option
    set (regardless of rate set)?
    
  DS_ISDATA_RS1()
    Is the given Service option rateset 1?
    
  DS_ISDATA_RS2()
    Is the given Service option rateset 2?
    
  DS_ISDATA()
    Is the given Service option a data service option?
  
  DS_ISDATA_G3_FAX()
    Is the given service option a fax service option?
    
  DS_ISDATA_ASYNC()
    Is the given service option an async service option?
    
  DS_ISDATA_PPP_PKT()
    Is the given service option a packet data service option?
    
  DS_ISDATA_RLP_TYPE2()
    Is the service option an MDR service option?
    
  DS_ISDATA_MDR()
    Is the service option an MDR service option?
    
  DS_ISDATA_RLP_TYPE3()
    Is the service option a 1x service option (SO 33)?
    
  DS_ISDATA_3G()
    Is the service option a 1x service option (SO 33)?
    
  DS_ISDATA_HDR()
    Is the service option an HDR service option?
  
EXTERNALIZED MACROS FOR CASE STATEMENTS
  DS_CASE_DATA_RS1_PRE707
    Case statements for rateset 1 service options which are in the pre-707
    service option set.
    
  DS_CASE_DATA_RS1_IS707
    Case statements for rateset 1 service options which are in the 707
    service option set.
    
  DS_CASE_DATA_RS2_PRE707
    Case statements for rateset 2 service options which are in the pre-707
    service option set.
    
  DS_CASE_DATA_RS2_QC_PROPTRY
    Case statements for rateset 2 service options which are in the
    proprietary service option set.

  DS_CASE_DATA_RS1
    Case statements for rateset 1 service options
  
  DS_CASE_DATA_RS2
    Case statements for rateset 1 service options
    
  DS_CASE_DATA
    Case statements for all data service options

INITIALIZATION AND SEQUENCING REQUIREMENTS
  None

 Copyright (c) 2002 by QUALCOMM Incorporated.  All Rights Reserved.
*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*/

/*===========================================================================

                      EDIT HISTORY FOR FILE

  $PVCSPath: L:/src/asw/MM_DATA/vcs/ds707.h_v   1.6   15 Nov 2002 17:38:26   akhare  $
  $Header: //source/qcom/qct/modem/data/1x/707/main/lite/inc/ds707.h#1 $ $DateTime: 2008/04/11 08:25:15 $ $Author: nsivakum $

when        who    what, where, why
--------    ---    -------------------------------------------------------
10/15/02    ak     Added default for Control Hold.
10/13/02    ak     Removed F92 enum.
09/24/02    atp    Added support for QOS. (1x Release-A feature).
07/29/02    ak     Changed some enums into #defines.  Matches up with a
                   bug fix in old dsmgr.
07/14/02    ak     Updated INVALID_SO to take CAI #define.
05/14/02    ak     Initial verion.
===========================================================================*/


/*===========================================================================
                     INCLUDE FILES FOR MODULE
===========================================================================*/
#include "comdef.h"
#include "customer.h"

#ifdef FEATURE_DATA_IS707
#include "cai.h"

/*===========================================================================
                        TYPEDEFS AND VARIABLES
===========================================================================*/
typedef enum
{
  DS_RATESET1 = 0,     /* Rate Set 1 */
  DS_RATESET2 = 1,     /* Rate Set 2 */
  DS_RATESET_COUNT
} ds707_rateset_type;

typedef word  ds707_so_type;       /* Service option type                  */

#ifdef FEATURE_IS2000_REL_A
/*---------------------------------------------------------------------------
  Type for non-assured QOS priority adjustment desired.
---------------------------------------------------------------------------*/
typedef enum
{
  DS707_QOS_DESIRED_PRI_MIN     = 0,     /* min value acc to IS-707        */
  DS707_QOS_DESIRED_PRI_DEFAULT = 0xD,   /* default value acc to IS-707    */
  DS707_QOS_DESIRED_PRI_MAX     = 0xF    /* max value acc to IS-707        */
} ds707_qos_desired_pri_enum_type;
#endif /* FEATURE_IS2000_REL_A */

/*---------------------------------------------------------------------------
                     SERVICE OPTION SET SETTINGS
---------------------------------------------------------------------------*/
#define  DS_SO_SET_PRE707      0  /* IS-707.0 (pre-707) SO numbers         */
#define  DS_SO_SET_PROPTRY     1  /* Qualcomm proprietary SO numbers       */
#define  DS_SO_SET_IS707       2  /* IS-707 SO numbers                     */
#define  DS_SO_SET_COUNT       3  /* used for boundary checks              */

/*---------------------------------------------------------------------------
                            MDR SETTTINGS
---------------------------------------------------------------------------*/
#define DS_MDR_MODE_MDR_ONLY   0  /* Use only MDR packet data services     */
#define DS_MDR_MODE_IF_AVAIL   1  /* Use MDR if available, else low-speed  */
#define DS_MDR_MODE_NO_MDR     2  /* Use only low-speed packet data        */
#define DS_MDR_MODE_SO33_PREF  3  /* Service Option 33 (1x channel)        */
#define DS_MDR_MODE_MAX        4  /* used for boundary checks              */

/*---------------------------------------------------------------------------
                     NETWORK MODEL (AT+CRM) SETTINGs
---------------------------------------------------------------------------*/
#define  DS_CRM_VAL_ASYNC_MODE      0   /* Async call                      */
#define  DS_CRM_VAL_RELAY_MODE      1   /* Relay mode packet call          */
#define  DS_CRM_VAL_NETMODEL_MODE   2   /* Network model packet call       */
#define  DS_CRM_VAL_MAX             3   /* For bounds checking             */


#define DS_MDR_MODE_DEFAULT    DS_MDR_MODE_SO33_PREF
#define NUM_MDR_MODES          DS_MDR_MODE_MAX


#define DS_TRTL_DEFAULT (TRUE)    /* R-SCH Throttle default                */
#define DS_SCRM_DEFAULT (TRUE)    /* SCRM default setting                  */

#define DS_QNC_DEFAULT  (TRUE)    /* QNC default setting                   */

#define DS_SO_SET_DEFAULT  (DS_SO_SET_IS707) /* default SO set to use      */

#define DS707_INVALID_SO  CAI_SO_NULL

#define DS_CHS_DEFAULT  0

/*---------------------------------------------------------------------------
  Is it a rateset 1 (Pre-707) data s.o.? 
---------------------------------------------------------------------------*/
#define DS_ISDATA_RS1_PRE707(so) \
  (((so) == CAI_SO_PPP_PKT_DATA_PRE707) || \
   ((so) == CAI_SO_ASYNC_DATA_PRE707) || \
   ((so) == CAI_SO_G3_FAX_PRE707))

#define DS_CASE_DATA_RS1_PRE707 \
  case CAI_SO_PPP_PKT_DATA_PRE707: \
  case CAI_SO_ASYNC_DATA_PRE707: \
  case CAI_SO_G3_FAX_PRE707:

/*---------------------------------------------------------------------------
  Is it a rateset 1 (IS-707) data s.o.? 
---------------------------------------------------------------------------*/
#define DS_ISDATA_RS1_IS707(so) \
  (((so) == CAI_SO_PPP_PKT_DATA_IS707) || \
   ((so) == CAI_SO_ASYNC_DATA_IS707) || \
   ((so) == CAI_SO_G3_FAX_IS707))

#define DS_CASE_DATA_RS1_IS707 \
  case CAI_SO_PPP_PKT_DATA_IS707: \
  case CAI_SO_ASYNC_DATA_IS707: \
  case CAI_SO_G3_FAX_IS707:

/*---------------------------------------------------------------------------
  Is it a rateset 2 (Pre-707) data s.o.? 
---------------------------------------------------------------------------*/
#define DS_ISDATA_RS2_PRE707(so) \
  (((so) == CAI_SO_PPP_PKT_DATA_13K_PRE707) || \
   ((so) == CAI_SO_ASYNC_DATA_13K_PRE707) || \
   ((so) == CAI_SO_G3_FAX_13K_PRE707))

#define DS_CASE_DATA_RS2_PRE707 \
  case CAI_SO_PPP_PKT_DATA_13K_PRE707: \
  case CAI_SO_ASYNC_DATA_13K_PRE707: \
  case CAI_SO_G3_FAX_13K_PRE707:

/*---------------------------------------------------------------------------
  Is it a rateset 2 (QC proprietary) data s.o.?
---------------------------------------------------------------------------*/
#define DS_ISDATA_RS2_QC_PROPTRY(so) \
  (((so) == CAI_SO_PPP_PKT_DATA_13K_PROPTRY) || \
   ((so) == CAI_SO_ASYNC_DATA_13K_PROPTRY) || \
   ((so) == CAI_SO_G3_FAX_13K_PROPTRY))

#define DS_CASE_DATA_RS2_QC_PROPTRY \
  case CAI_SO_PPP_PKT_DATA_13K_PROPTRY: \
  case CAI_SO_ASYNC_DATA_13K_PROPTRY: \
  case CAI_SO_G3_FAX_13K_PROPTRY:

/*---------------------------------------------------------------------------
  Is it a IS-707 data s.o.?
---------------------------------------------------------------------------*/
#define DS_ISDATA_IS707(so) \
  ( (DS_ISDATA_RS1_IS707(so)) || (DS_ISDATA_RS2_PRE707(so)) )
  
/*---------------------------------------------------------------------------
  Is it a rateset 1 data s.o.?
---------------------------------------------------------------------------*/
#define DS_ISDATA_RS1(so) \
  ( \
   (CAI_SO_PPP_PKT_DATA_3G == (so)) || \
    (CAI_SO_DATA == (so)) || \
    DS_ISDATA_RS1_PRE707(so) || \
    DS_ISDATA_RS1_IS707(so) || \
    (CAI_SO_MDR_PKT_DATA_FRS1_RRS1 == (so)) \
  )

#define DS_CASE_DATA_RS1 \
     case CAI_SO_PPP_PKT_DATA_3G: \
     case CAI_SO_DATA: \
     DS_CASE_DATA_RS1_PRE707 \
     DS_CASE_DATA_RS1_IS707 \
     case CAI_SO_MDR_PKT_DATA_FRS1_RRS1:

/*---------------------------------------------------------------------------
  Is it a rateset 2 data s.o.?
---------------------------------------------------------------------------*/
#define DS_ISDATA_RS2(so) \
  ( \
    DS_ISDATA_RS2_PRE707(so) || \
    DS_ISDATA_RS2_QC_PROPTRY(so) || \
    (CAI_SO_MDR_PKT_DATA_FRS2_RRS2 == (so)) \
  )

#define DS_CASE_DATA_RS2 \
     DS_CASE_DATA_RS2_PRE707 \
     DS_CASE_DATA_RS2_QC_PROPTRY \
     case CAI_SO_MDR_PKT_DATA_FRS2_RRS2:

/*---------------------------------------------------------------------------
  Is it a data s.o.?
---------------------------------------------------------------------------*/
#define DS_ISDATA(so) \
  (DS_ISDATA_RS1(so) || DS_ISDATA_RS2(so))

#define DS_CASE_DATA \
  DS_CASE_DATA_RS1 \
  DS_CASE_DATA_RS2

/*---------------------------------------------------------------------------
  Is it a {fax,async,packet} data s.o.?
---------------------------------------------------------------------------*/
#define DS_ISDATA_G3_FAX(so) \
  ((CAI_SO_G3_FAX_PRE707 == (so)) || \
   (CAI_SO_G3_FAX_IS707 == (so)) || \
   (CAI_SO_G3_FAX_13K_PRE707 == (so)) || \
   (CAI_SO_G3_FAX_13K_PROPTRY == (so)))

#define DS_ISDATA_ASYNC(so) \
  ((CAI_SO_ASYNC_DATA_PRE707 == (so)) || \
   (CAI_SO_ASYNC_DATA_IS707 == (so)) || \
   (CAI_SO_ASYNC_DATA_13K_PRE707 == (so)) || \
   (CAI_SO_ASYNC_DATA_13K_PROPTRY == (so)))

#define DS_ISDATA_PPP_PKT(so) \
  ((CAI_SO_PPP_PKT_DATA_3G == (so)) || \
   (CAI_SO_DATA == (so)) || \
   (CAI_SO_PPP_PKT_DATA_PRE707 == (so)) || \
   (CAI_SO_PPP_PKT_DATA_IS707 == (so)) || \
   (CAI_SO_PPP_PKT_DATA_13K_PRE707 == (so)) || \
   (CAI_SO_PPP_PKT_DATA_13K_PROPTRY == (so)) || \
   (CAI_SO_MDR_PKT_DATA_FRS1_RRS1 == (so)) || \
   (CAI_SO_MDR_PKT_DATA_FRS2_RRS2 == (so)))

#define DS_ISDATA_RLP_TYPE2(so) \
     ( (CAI_SO_MDR_PKT_DATA_FRS1_RRS1 == (so)) || \
       (CAI_SO_MDR_PKT_DATA_FRS2_RRS2 == (so)) )

#define DS_ISDATA_MDR(so)    DS_ISDATA_RLP_TYPE2(so)

#define DS_ISDATA_RLP_TYPE3(so) \
     ( (CAI_SO_PPP_PKT_DATA_3G == (so)) )

#define DS_ISDATA_3G(so)     DS_ISDATA_RLP_TYPE3(so)

#ifdef FEATURE_HDR
#error code not present
#endif
/*===========================================================================
                   EXTERNAL FUNCTION DEFINITIONS
===========================================================================*/
#endif /* FEATURE_DATA_IS707 */
#endif /* DS707_H            */

