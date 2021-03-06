#ifndef CAMSENSOR_SP0828_H
#define CAMSENSOR_SP0828_H

/*============================================================================

                 Camera Interface Device Driver Source File

   DESCRIPTION
     This file contains the definitions needed for the camera interface
     device driver.

============================================================================*/

/*============================================================================

                      EDIT HISTORY FOR FILE

 This section contains comments describing changes made to this file.
 Notice that changes are listed in reverse chronological order.

 when      who    what, where, why
 --------  -----  ----------------------------------------------------------
 10/06/29 wangliang    Initial Version


============================================================================*/

/*============================================================================
                        INCLUDE FILES
============================================================================*/

#include "camsensor.h"
#include "camctrl.h"

/*===========================================================================

FUNCTION      CAMSENSOR_SIC110A_INIT

DESCRIPTION
              Initialize the camera sensor;

DEPENDENCIES
  None

RETURN VALUE
  if successful
    TRUE
  else
    FALSE

SIDE EFFECTS
  None

===========================================================================*/

boolean camsensor_sp0828_init(camsensor_function_table_type *camsensor_function_table_ptr,
                               camctrl_tbl_type              *camctrl_tbl_ptr);

uint8 camsensor_sp0828_ycbcr_active(void);
uint8 camsensor_sp0828_ycbcr_unactive(void);

#endif /* CAMSENSOR_SIV100B_H */


