#ifndef MCCSRID_H
#define MCCSRID_H

/*===========================================================================

          MAIN CONTROL SRID MANAGER

GENERAL DESCRIPTION
  This task manages the SRID.

DESCRIPTION
   This module manages the SRID resource for the mobile.

Copyright (c) 2001-2005 by QUALCOMM, Incorporated.  All Rights Reserved.

===========================================================================*/

/*===========================================================================
                      EDIT HISTORY FOR FILE

This section contains comments describing changes made to this file.
Notice that changes are listed in reverse chronological order.

$PVCSPath: L:/src/asw/MSM5100/CP_REL_A/vcs/mccsrid.h_v   1.4   28 Jun 2002 12:50:26   lhang  $
$Header: //source/qcom/qct/modem/1x/cp/rel/1h08/inc/mccsrid.h#1 $ $DateTime: 2009/05/29 03:54:56 $ $Author: prabhuk $

when       who     what, where, why
--------   ---     ----------------------------------------------------------
11/21/06   sb      Added support for VOIP-1X handoffs.
06/23/06   pg      Added support for RPC.
03/28/05   fc      Made FCS also available to Rel D.
03/12/04   fc      Added support for Release C fast call setup.
02/07/02   lh      Added mccsrid_free_all_owned_srids().
01/08/02   lh      Function name change.
08/28/01   lh      Bug fixes.
05/14/01   lh      Initial revision of the file.
===========================================================================*/

#include "comdef.h"
#include "caii.h"
#include "cmd.h"
#include "queue.h"

/*===========================================================================

                      PUBLIC DATA DECLARATIONS 

===========================================================================*/

#ifdef FEATURE_IS2000_REL_A_SVD

/* Number of SR_ID objects in the SRID array */
#define MCCSRID_SRID_NUM_MAX 7      /* Supports SR_ID index from 0 to 6 */


/* Type to identify the ownership of an SR_ID */
typedef enum 
{
  SRID_OWNER_NULL,      /* Reserved for the BS */
  SRID_OWNER_MC,        /* Owner ID used by MC */
  SRID_OWNER_DS,        /* Owner ID used by DS */
  SRID_OWNER_RESERVED   /* Owner ID reserved, do not use */
} mccsrid_srid_owner_type;


/* Error code returned by mccsrid_allocate_srid() if no available SR_ID */
#define MCCSRID_INVALID_SRID 0xFF



/*===========================================================================

                    PUBLIC FUNCTION DECLARATIONS 

===========================================================================*/

/* <EJECT> */
/*===========================================================================

FUNCTION mccsrid_initialize

DESCRIPTION
  Performs necessary initialization for the SR_ID objects.  This function
  is called upon power up initialization. 

DEPENDENCIES
  None

RETURN VALUE
  None.

SIDE EFFECTS
  This function erase all the SR_ID information. Caller needs to make sure
  that it is safe to call this function. 
===========================================================================*/
extern void mccsrid_initialize(void);


/* <EJECT> */
/*===========================================================================

FUNCTION mccsrid_print

DESCRIPTION
  Prints all the SR_ID to screen for diagnostic purpose. 

DEPENDENCIES
  None

RETURN VALUE
  None.

SIDE EFFECTS
  This function is not protected by INTLOCK. 
   
===========================================================================*/
extern void mccsrid_print(void);

/* <EJECT> */
/*===========================================================================

FUNCTION mccsrid_available_srid

DESCRIPTION
  This function returns the lowest unused SR_ID number to the caller.  But
  it does not allocate the sr_id.

DEPENDENCIES
  None

RETURN VALUE
  SR_ID number [1, MCCSRID_SRID_NUM_MAX-1], if a SR_ID is available. 
  Returns MCCSRID_INVALID_SRID if none of the SRID is available.

SIDE EFFECTS
  Interrupt is locked while accessing the SR_ID array.
===========================================================================*/
extern byte mccsrid_available_srid
(
 void
);

/* <EJECT> */
/*===========================================================================

FUNCTION mccsrid_allocate_srid

DESCRIPTION
  This function returns the lowest unused SR_ID number to the caller.

DEPENDENCIES
  None

RETURN VALUE
  SR_ID number [1, MCCSRID_SRID_NUM_MAX-1], if a SR_ID is available. 
  Returns MCCSRID_INVALID_SRID if none of the SRID is available.

SIDE EFFECTS
  Interrupt is locked while accessing the SR_ID array.
===========================================================================*/
extern byte mccsrid_allocate_srid
(
  mccsrid_srid_owner_type owner
    /* Owner ID of the allocation request */
);

#ifdef FEATURE_HTORPC_METACOMMENTS
/*~ FUNCTION mccsrid_allocate_srid */
#endif /* FEATURE_HTORPC_METACOMMENTS */

/* <EJECT> */
/*===========================================================================

FUNCTION mccsrid_check_specific_srid

DESCRIPTION
  This function is used to verify if an SR_ID is still reserved for the same
  owner. This function is called by MC when it receives an SR_ID from DS.

DEPENDENCIES
  None

RETURN VALUE
  TRUE if the SI_ID is used by the same owner. 

SIDE EFFECTS
===========================================================================*/
extern boolean mccsrid_check_specific_srid
(
  byte id 
    /* SR_ID number to check */
);


/* <EJECT> */
/*===========================================================================

FUNCTION mccsrid_free_srid

DESCRIPTION
  This function deallocates an SRID owned by the owner.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  Interrupt is locked while accessing the SR_ID array.
===========================================================================*/
extern void mccsrid_free_srid
(
  byte id, 
    /* SR_ID number to free */
  mccsrid_srid_owner_type owner
    /* Current owner of the SR_ID to free */
);

/* <EJECT> */
/*===========================================================================

FUNCTION mccsrid_free_all_owned_srids

DESCRIPTION
  This function deallocates all SRID owned by the owner.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  Interrupt is locked while accessing the SR_ID array.
===========================================================================*/
void mccsrid_free_all_owned_srids
(
  mccsrid_srid_owner_type owner
    /* Current owner of the SR_ID to free */
);

/* <EJECT> */
/*===========================================================================

FUNCTION mccsrid_update_srid

DESCRIPTION
  This function updates SR_ID from the connection record.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  Interrupt is locked while accessing the SR_ID array.
===========================================================================*/
extern void mccsrid_update_srid
(
  const caii_srv_cfg_type  *cfg_ptr 
    /* Ptr to the service configuration record for this update */
);


/* <EJECT> */
/*===========================================================================

FUNCTION mccsrid_release

DESCRIPTION
  This function is called when LL3 exits TC state. SR_ID manager changes 
  all the active SR_ID to available if the owner is NULL, otherwise, SR_ID
  manager changes active SR_ID to used if owner is not NULL. Notice that
  this function does not free an SRID if it has an owner.


DEPENDENCIES
  None. 

RETURN VALUE
  None

SIDE EFFECTS
  Interrupt is locked while accessing the SR_ID array.
===========================================================================*/
extern void mccsrid_release(void);


#endif /* FEATURE_IS2000_REL_A_SVD */

#if defined(FEATURE_IS2000_REL_C_FCS) || defined(FEATURE_IS2000_REL_D)
/*===========================================================================

FUNCTION MCCSRID_ALLOCATE_SPECIFIC_SRID

DESCRIPTION
  This function marks specified SR_ID as used if it is available  

DEPENDENCIES
  None

RETURN VALUE
  Specified SR_ID number if it is available.
  Otherwise, returns MCCSRID_INVALID_SRID. 

SIDE EFFECTS
  Interrupt is locked while accessing the SR_ID array.

===========================================================================*/
byte mccsrid_allocate_specific_srid
(
  byte id,
    /* SR_ID number */
  mccsrid_srid_owner_type owner
    /* Owner ID of the allocation request */
);
#endif /* FEATURE_IS2000_REL_C_FCS || FEATURE_IS2000_REL_D */

#endif /* MCCSRID_H */

