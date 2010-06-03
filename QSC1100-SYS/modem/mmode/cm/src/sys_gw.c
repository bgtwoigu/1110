/*===========================================================================

              S Y S T E M   G W   M O D U L E  

DESCRIPTION
  This header file contains functions  that are shared between Call Manager,
  Call Manager clients and the GSM/WCDMA protocol stacks.

Copyright (c) 2002 - 2006 by QUALCOMM INCORPORATED. All Rights Reserved.

Export of this technology or software is regulated by the U.S. Government.
Diversion contrary to U.S. law prohibited.

===========================================================================*/


/*===========================================================================

                      EDIT HISTORY FOR FILE

  This section contains comments describing changes made to the module.
  Notice that changes are listed in reverse chronological order.

  $Header: 

when       who     what, where, why
--------   ---     ----------------------------------------------------------
07/01/08   rm      Mainlining of MBMS changes
03/30/07    rp     Initial revision
===========================================================================*/


#include "sys.h"
/*lint -esym(766,sys_gw.h)*/
#include "sys_gw.h"
#include "string.h"
#include "cmidbg.h" 
#include "cmi.h"


#if (defined(FEATURE_WCDMA) || defined(FEATURE_GSM) )
#error code not present
#endif /* #if (defined(FEATURE_WCDMA) || defined(FEATURE_GSM))*/
