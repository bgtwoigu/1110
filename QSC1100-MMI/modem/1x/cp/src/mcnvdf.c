/*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*

               M C   N V   D E F A U L T   S E R V I C E

GENERAL DESCRIPTION
  This file contains the functions that fill an NV response buffer with default
  values for the NV items that are accessed from the RUIM.  This service is used
  by clients that need default data to go on-line when the RUIM card is 
  inaccessible.

EXTERNALIZED FUNCTIONS

  mc_set_esn_for_default
    Sets the esn value used for deriving default NV items.
    
  mc_nv_default_values
    Return the default value of a specified NV item.

INITIALIZATION AND SEQUENCING REQUIREMENTS

  mc_set_esn_for_default must be called before using 
  nv_ruim_default_values.
  
Copyright (c) 1991-2005 by QUALCOMM, Inc.  All Rights Reserved.

*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*/

/*===========================================================================
                        EDIT HISTORY FOR MODULE

$PVCSPath: L:/src/asw/MSM5100/CP_REL_A/vcs/mcnvdf.c_v   1.1.2.0   30 Nov 2001 17:31:22   fchan  $
$Header: //source/qcom/qct/modem/1x/cp/rel/1h08/src/mcnvdf.c#1 $ $DateTime: 2009/05/29 03:54:56 $ $Author: prabhuk $

when       who     what, where, why
--------   ---     ----------------------------------------------------------
05/03/05   fh      removed including of srchi.h, which is not necessary. CP is
                   supposed to include srchmc.h only for all Searcher references.
10/04/04   yll     Added the SCI default value in RUIM build.
09/11/03   yll     Added the featurization for ACP include files.
11/26/02   sj      Renamed the class member in nv_accolc_type to ACCOLCpClass.
01/08/01   ts      Added default values for NV_ANALOG_FIRSTCHP_I, NV_ANALOG_HOME_SID_I, 
                   NV_ACCOLC_I, NV_SID_NID_I and NV_HOME_SID_NID_I.
08/09/00   ts      Created file.  Needed a service to return default values for
                   NV items that reside on the RUIM.  

===========================================================================*/

/* <EJECT> */
/*===========================================================================

                     INCLUDE FILES FOR MODULE

===========================================================================*/
#include "customer.h"
#include "comdef.h"
#include "mccdma.h"
#ifdef FEATURE_ACP
#error code not present
#endif /* FEATURE_ACP */
#include "nv.h"
#include "prl.h"
#include "mobile.h"

/* <EJECT> */
/*===========================================================================

            LOCAL DEFINITIONS AND DECLARATIONS FOR MODULE

This section contains local definitions for constants, macros, types,
variables and other items needed by this module.

===========================================================================*/
/* This is the default analog home SID used by MC when the RUIM read fails */
#define MC_DEF_ANALOG_HOME_SID 1

/* This holds the esn that is initialized by UI and used to determine the MC
   default values derived from the esn. */
LOCAL nv_esn_type esn;

/* <EJECT> */
/*===========================================================================

FUNCTION MC_SET_ESN_FOR_DEFAULT

DESCRIPTION
  This function sets the static variable - esn - for use in deriving default
  values of some NV items processed by this service.

DEPENDENCIES
  This function must be called with the appropriate esn value before using
  the other functions.

RETURN VALUE
  None

SIDE EFFECTS
  The esn value will be set appropriately after this function is executed.

===========================================================================*/
void mc_set_esn_for_default ( 
  dword esn_value
)
{
  /* Set the static variable for use in deriving default NV items */
  esn.esn = esn_value;
} /* nvruim_set_esn_for_default () */


/* <EJECT> */
/*===========================================================================

FUNCTION NV_RUIM_DEFAULT_IMSI_S1

DESCRIPTION
  This function computes the default IMSI_S1 value as per IS-683 section 3.1.

DEPENDENCIES
  This function uses the static variable - esn to derive imsi_s1.  This 
  variable must be set by nvruim_set_esn_for_default before this function
  is used.

RETURN VALUE
  This function returns the encoded value of IMSI_S1 with the four least
  significant digits set to ESNp, converted directly from binary to decimal,
  modulo 10000.  The other digits are set to zero.

SIDE EFFECTS
  None.

===========================================================================*/
dword mc_default_imsi_s1 ( void )
{
  word zero;     /* Encoding of three zero digits */
  word fourth;   /* Fourth from last decimal digit of the ESN */
  word last3;    /* Last three decimal digits of the ESN */
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  /* Encode digits as per JSTD-008 section 2.3.1.1 */
  zero = 1110 - 111;
  last3 = (esn.esn % 1000);
  last3 += ((last3 / 100) == 0) ? 1000 : 0;
  last3 += (((last3 % 100) / 10) == 0) ? 100 : 0;
  last3 += ((last3 % 10) == 0) ? 10 : 0;
  last3 -= 111;
  fourth = ((esn.esn % 10000) / 1000); /* In range 0-9 */
  if (fourth == 0)
  {
    fourth = 10;
  }

#ifdef FEATURE_UI_IMSI_ALL_0_OK
#error code not present
#else
  /* Concatenate results and return 24 bit value for imsi_s1 */
  /* Example: esn = 120406
  **      imsi_s1 = 000  0  406
  **  encodes to -> 999  10 395
  **       in hex = 3e7  a  18b
  **    in binary = 1111100111 1010 0110001011
  */
  return (((dword) zero << 14) | (fourth << 10) | last3);
#endif

} /* mc_default_imsi_s1() */


/* <EJECT> */
/*===========================================================================

FUNCTION MC_DEFAULT_IMSI_T_S1

DESCRIPTION
  This function computes the default IMSI_T_S1 value as per IS-683 section 3.1.

DEPENDENCIES
  This function uses the static variable - esn to derive imsi_s1.  This 
  variable must be set by nvruim_set_esn_for_default before this function
  is used.

RETURN VALUE
  This function returns the encoded value of IMSI_T_S1 with the four least
  significant digits set to ESNp, converted directly from binary to decimal,
  modulo 10000.  The other digits are set to zero.

SIDE EFFECTS
  None.

===========================================================================*/
dword mc_default_imsi_t_s1 ( void )
{
  word zero;     /* Encoding of three zero digits */
  word fourth;   /* Fourth from last decimal digit of the ESN */
  word last3;    /* Last three decimal digits of the ESN */
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  /* Encode digits as per JSTD-008 section 2.3.1.1 */
  zero = 1110 - 111;
  last3 = (esn.esn % 1000);
  last3 += ((last3 / 100) == 0) ? 1000 : 0;
  last3 += (((last3 % 100) / 10) == 0) ? 100 : 0;
  last3 += ((last3 % 10) == 0) ? 10 : 0;
  last3 -= 111;
  fourth = ((esn.esn % 10000) / 1000); /* In range 0-9 */
  if (fourth == 0)
  {
    fourth = 10;
  }

  /* Concatenate results and return 24 bit value for imsi_s1 */
  /* Example: esn = 120406
  **      imsi_s1 = 000  0  406
  **  encodes to -> 999  10 395
  **       in hex = 3e7  a  18b
  **    in binary = 1111100111 1010 0110001011
  */
  return (((dword) zero << 14) | (fourth << 10) | last3);

} /* mc_default_imsi_t_s1() */


/* <EJECT> */

/*===========================================================================

FUNCTION MC_DEFAULT_VALUES

DESCRIPTION
  This function returns default values for the NV items that reside on 
  the RUIM.  This function is used to provide default values in the
  case the RUIM data is not accessible.

DEPENDENCIES
  This function uses nvruim_default_imsi_s1 and nvruim_default_imsi_t_s1
  which use the static - esn variable.  This variable must be set by 
  nvruim_set_esn_for_default before this function is used.


RETURN VALUE
  Return default values for the NV access.

SIDE EFFECTS
  None.

===========================================================================*/
void mc_default_values ( 
  nv_items_enum_type item,        /* which item */
  nv_item_type *data_ptr          /* pointer to data buffer for item */
)
{
  /* This is used as an index into processing all MINs of a SID NID list. */
  int min;
  /* This is used as an index into all available SID NID pairs. */
  int sid_nid_index;
  static byte dir_number[11] = "0000000000";

  /* Determine which item is needed */
  switch (item)
  {
    case NV_ESN_I:                /*   0 */
    case NV_ESN_CHKSUM_I:         /*   1 */
      /* These are handled at the NV RUIM functions. */
      break;

    #ifdef FEATURE_ACP
#error code not present
    #endif

    case NV_COUNT_I:              /*  31 */
      data_ptr->count.count = 0;
      break;

    case NV_MIN1_I:               /*  32 */
      data_ptr->min1.min1[NV_CDMA_MIN_INDEX] =  mc_default_imsi_s1() ;
      data_ptr->min1.min1[NV_ANALOG_MIN_INDEX] = IMSI_S1_ZERO ;
      break;
      
    case NV_MIN2_I:               /*  33 */
      data_ptr->min2.min2[NV_ANALOG_MIN_INDEX] = IMSI_S2_ZERO ;
      data_ptr->min2.min2[NV_CDMA_MIN_INDEX] = IMSI_S2_ZERO ;
      break;
      
    case NV_ACCOLC_I:             /*  37 */
      data_ptr->accolc.ACCOLCpClass[NV_ANALOG_MIN_INDEX] = esn.esn % 10;
      data_ptr->accolc.ACCOLCpClass[NV_CDMA_MIN_INDEX] = esn.esn % 10;
      break;

    case NV_SID_NID_I:            /*  38 */
      /* Fill all the MINs for this NAM */
      for (min = 0; min < NV_MAX_MINS; ++min )
      {
        /* Fill all the SID_NID pairs for this MIN */
        for (sid_nid_index = 0; sid_nid_index < NV_MAX_SID_NID; ++sid_nid_index)
        {
          data_ptr->sid_nid.pair[min][sid_nid_index].sid = PRL_WILDCARD_SID;
          data_ptr->sid_nid.pair[min][sid_nid_index].nid = PRL_WILDCARD_NID;
        } /* end for - index through all sid_nid pairs */
      } /* end for - index through all MINs */
      break;

    case NV_IMSI_MCC_I:           /* 176 */
      /* Default MCC is 000 in ASCII and 999 in min */
      data_ptr->imsi_mcc.imsi_mcc = IMSI_MCC_ZERO;      /* default to '000' */
      break;

    case NV_IMSI_11_12_I:         /* 177 */
      /* Default MNC is 00 in ASCII and 99 in min */
      data_ptr->imsi_11_12.imsi_11_12 = IMSI_11_12_ZERO;  /* default to '00' */
      break;

    case NV_DIR_NUMBER_I:         /* 178 */
      memcpy( (byte*)(data_ptr->dir_number.dir_number),
              dir_number, 10 );
      break;

    case NV_IMSI_ADDR_NUM_I:      /* 209 */
      data_ptr->imsi_addr_num.num = IMSI_CLASS_0_ADDR_NUM ;
      break;

    case NV_HOME_SID_NID_I:       /* 259 */
      /* Fill all the SID_NID pairs */
      for (sid_nid_index = 0; sid_nid_index < NV_MAX_HOME_SID_NID; ++sid_nid_index)
      {
        data_ptr->home_sid_nid.pair[sid_nid_index].sid = PRL_WILDCARD_SID;
        data_ptr->home_sid_nid.pair[sid_nid_index].nid = PRL_WILDCARD_NID;
      } /* end for - index through all sid_nid pairs */
      break;

    case NV_IMSI_T_S1_I:          /* 262 */
      data_ptr->imsi_t_s1.min1[NV_CDMA_MIN_INDEX] =  mc_default_imsi_t_s1() ;
      data_ptr->imsi_t_s1.min1[NV_ANALOG_MIN_INDEX] = IMSI_S1_ZERO ;
      break;
      
    case NV_IMSI_T_S2_I:          /* 263 */
      /* Default S2 is 000 in ASCII and 999 in min */
      data_ptr->imsi_t_s2.min2[NV_ANALOG_MIN_INDEX] = IMSI_S2_ZERO ;
      data_ptr->imsi_t_s2.min2[NV_CDMA_MIN_INDEX] = IMSI_S2_ZERO ;
      break;
      
    case NV_IMSI_T_MCC_I:         /* 264 */
      /* Default MCC is 000 in ASCII and 999 in min */
      data_ptr->imsi_t_mcc.imsi_mcc = IMSI_MCC_ZERO;      /* default to '000' */
      break;

    case NV_IMSI_T_11_12_I:       /* 265 */
      /* Default MNC is 00 in ASCII and 99 in min */
      data_ptr->imsi_t_11_12.imsi_11_12 = IMSI_11_12_ZERO;  /* default to '00' */
      break;

    case NV_IMSI_T_ADDR_NUM_I:    /* 266 */
      data_ptr->imsi_t_addr_num.num = IMSI_CLASS_0_ADDR_NUM ;
      break;

    #ifdef FEATURE_UIM_RUIM_SUPPORT_SCI
    case NV_SLOT_CYCLE_INDEX_I:
      data_ptr->slot_cycle_index    = slot_cycle_index;
      break;
    #endif
    
    default:
      /* Item is not supported by R-UIM                                       */
      break;
  }

} /* end - mc_default_values */



