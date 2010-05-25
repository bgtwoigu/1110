#ifndef TMSI_H
#define TMSI_H
/*===========================================================================

     T M S I   S U P P O R T   F U N C T I O N S   A N D   D A T A

DESCRIPTION
  This file contains functions and constants for TMSI addressing
  support.

Copyright (c) 1996-2005 by QUALCOMM, Incorporated.  All Rights Reserved.

===========================================================================*/

/* <EJECT> */
/*===========================================================================

                      EDIT HISTORY FOR FILE

This section contains comments describing changes made to this file.
Notice that changes are listed in reverse chronological order.


$PVCSPath: L:/src/asw/MSM5100/CP_REL_A/vcs/tmsi.h_v   1.2   27 Jun 2002 19:50:38   fchan  $
$Header: //source/qcom/qct/modem/1x/cp/rel/1h08/src/tmsi.h#1 $ $DateTime: 2009/05/29 03:54:56 $ $Author: prabhuk $
   
when       who     what, where, why
--------   ---     ----------------------------------------------------------
04/26/06   an      Fixed Lint Errors
02/18/05   fc      Merged the following from MSMSHARED:
02/08/05   an       Mainlined IS95B features
06/27/02   fc      Changed UPM function names.
05/20/02   fc      Added support for Universal Page Message.
05/10/99   kmp     Changed FEATURE_JSTD008 to FEATURE_TMSI
04/09/96   day     Created file. 

===========================================================================*/

/* <EJECT> */
/*===========================================================================

                     INCLUDE FILES FOR MODULE

===========================================================================*/

#include "comdef.h"
#include "caii.h"
#include "mccdma.h"

/*===========================================================================

                        DATA DECLARATIONS

===========================================================================*/

#define MAX_TMSI_ZONE_LEN  64
  /* Maximum TMSI zone length in bits */

#define MAX_TMSI_CODE_LEN  4
  /* Maximum TMSI zone length in bytes */


/*===========================================================================

                      FUNCTION DECLARATIONS

===========================================================================*/

/*===========================================================================

FUNCTION TMSI_ADDRESS_MATCH

DESCRIPTION
  This function calculates the appropriate TMSI address based
  on the address information received and the stored TMSI information
  and determines if TMSI addresses match.  Returns boolean to
  indicate if match found; true => match found, false => no match.

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None

===========================================================================*/

extern boolean tmsi_address_match
(
  caii_pc_hdr_type *hdr_ptr
    /* Pointer to received message header with TMSI address information */
);

/*===========================================================================

FUNCTION TMSI_PAGE_MATCH

DESCRIPTION
  This function determines the type of page match operation and
  checks for TMSI page match based on page class and page subclass
  information provided in the received General Page Message.
  Returns boolean to indicate if match found; true => match found,
  false => no match.

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None

===========================================================================*/

extern mccdma_page_match_result_type tmsi_page_match
(
  caii_gen_page_rec_type *gen_ptr,
    /* Pointer to received general page message */
  mcc_page_info_type *page_info
    /* Structure to return infomation stored in the page */
);

/*===========================================================================

FUNCTION TMSI_RESET_FULL_TIMER

DESCRIPTION
  This function resets the full TMSI timer.

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None

===========================================================================*/

extern void tmsi_reset_full_timer
(
  void
);

/*===========================================================================

FUNCTION TMSI_CODE_AND_TIMER_RESET

DESCRIPTION
  This function resets the TMSI code and all TMSI timers.

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None

===========================================================================*/

extern void tmsi_code_and_timer_reset
(
  void
);

/*===========================================================================

FUNCTION TMSI_SET_FULL_TIMER

DESCRIPTION
  This function calculates full TMSI timeout value and sets the 
  full TMSI timer.

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None

===========================================================================*/

extern void tmsi_set_full_timer
(
  void
);

/*===========================================================================

FUNCTION TMSI_SET_EXP_TIMER

DESCRIPTION
  This function determines if a TMSI expiration timer needs to be set
  to check if TMSI expiration time has expired.  If timer is required,
  then it will calculate TMSI expiration timeout value by determining
  the delta value between system time and TMSI expiration timer and sets 
  the TMSI expiration timer to the delta value or maximum timeout value
  that can be set in rex_set_timer (word size), whichever is less.

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None

===========================================================================*/

extern void tmsi_set_exp_timer
(
  void
);

/*===========================================================================

FUNCTION TMSI_EXPIRED

DESCRIPTION
  This function determines if TMSI expiration timer has expired.
  Compared against system time.  Returns boolean value indicating
  if time has or has not expired.

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None

===========================================================================*/

extern boolean tmsi_expired
(
  void
);

/*===========================================================================

FUNCTION TMSI_DETERMINE_CODE_SIZE

DESCRIPTION
  Determine the size (in bytes) of the specified TMSI code.

DEPENDENCIES
  None

RETURN VALUE
  Returns the size of the specified TMSI code, in bytes.

SIDE EFFECTS
  None

===========================================================================*/
extern byte tmsi_determine_code_size
(
  dword tmsi_code   /* The TMSI code whose size is desired.   */
);

/*===========================================================================

FUNCTION TMSI_INFO_UPDATE

DESCRIPTION
  This function updates the current TMSI information and sets
  TMSI expiration time based on data received in the TMSI 
  assignment message.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
extern void tmsi_info_update
(
  caii_tmsi_data_type  *tmsi_data
    /* Received TMSI assignment message data */
);

/*===========================================================================

FUNCTION TMSI_POWERDOWN

DESCRIPTION
  This function saves the current TMSI information in NV before
  powering down.

DEPENDENCIES
  This procedure saves parameters in NV.  The NV task should be ready
  to receive commands when this procedure is called.

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
extern void tmsi_powerdown
(
  void
);

#ifdef FEATURE_IS2000_REL_A_CC
/*===========================================================================

FUNCTION TMSI_MATCH_UPM_PARTIAL_ADDR

DESCRIPTION
  This function performs the Mobile Station TMSI partial address bit match
  operation for the Universal Page Message. The result of match is directly
  saved in the input partial address structure. 

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None

===========================================================================*/

void tmsi_match_upm_partial_addr
(
  caii_univ_page_interleaved_addr_type *addr_ptr
);

/*===========================================================================

FUNCTION TMSI_UPM_PAGE_MATCH

DESCRIPTION
  This function determines the type of page match operation and
  checks for TMSI page match based on page class and page subclass
  information provided in the received Universal Page Record.
  Returns boolean to indicate if match found; true => match found,
  false => no match.

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None

===========================================================================*/

mccdma_page_match_result_type tmsi_upm_page_match
(
  caii_univ_page_rec_type *rec_ptr,
    /* Pointer to received Universal Page Record */
  mcc_page_info_type *page_info
    /* Structure to return infomation stored in the page */
);
#endif /* FEATURE_IS2000_REL_A_CC */
#endif /* TMSI_H */


