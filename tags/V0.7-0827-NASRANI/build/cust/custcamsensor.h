#ifndef CUSTCAMSENSOR_H
#define CUSTCAMSENSOR_H
/*===========================================================================

            " C U S T C A M S E N S O R "   H E A D E R   F I L E

DESCRIPTION
  Used to enable/disable the required camsensor in the build.

  Copyright (c) 2004 - 2006 by QUALCOMM Incorporated. All Rights Reserved.
===========================================================================*/


/*===========================================================================

                      EDIT HISTORY FOR FILE

  This section contains comments describing changes made to the module.
  Notice that changes are listed in reverse chronological order.

when       who     what, where, why
--------   ---     ----------------------------------------------------------
05/19/08   chai     created new custcamsensor for ULC

===========================================================================*/


/* This definition is to be added by OEM in cust<target>.h files.
 * requires one or more of the following to be added as well - enables
 * all sensors. */

//#define USE_CAMSENSOR_MICRON_MT9D112_MU2M0YU
//#define USE_CAMSENSOR_GC0309
#define USE_CAMSENSOR_SIV121A
#define USE_CAMSENSOR_SIC110A
//#define USE_CAMSENSOR_OV7675
#endif /* CUSTCAMSENSOR_H */
