#ifndef QMALLOC_H
#define QMALLOC_H
/*===========================================================================
                              <qmalloc.h>
DESCRIPTION
  Externalised functions for the qmalloc library

  Copyright (c) 2005 by QUALCOMM, Incorporated.  All Rights Reserved.
===========================================================================*/

/*===========================================================================
                             Edit History

$Header: //depot/asic/msmshared/services/common/qmalloc.h#1 $ $DateTime: 2006/02/23 15:00:31 $ $Author: chenroid $

when       who     what, where, why
--------   ---     ----------------------------------------------------------
09/09/05   TJH	   Created
===========================================================================*/
 

/*===========================================================================
                 Includes and Public Data Declarations
===========================================================================*/


/*===========================================================================

                     INCLUDE FILES FOR MODULE

===========================================================================*/

/*===========================================================================

                        DATA DECLARATIONS

===========================================================================*/

/*===========================================================================
  Constant / Define Declarations
===========================================================================*/

/*===========================================================================
  Type Declarations
===========================================================================*/

/*===========================================================================
  Global Constant Data Declarations 
===========================================================================*/

/*===========================================================================
                            Macro Definitions
===========================================================================*/

/*===========================================================================
                          Function Declarations
===========================================================================*/

#ifdef __cplusplus /* If this is reused in a C++ environment */
  extern "C" {
#endif

/*===========================================================================
FUNCTION Q_MALLOC

DESCRIPTION
  Allocates memory from the local heap if AMSS is not running, or the BREW
  heap once AMSS is started.

DEPENDENCIES
  None

RETURN VALUE
  Either a pointer to the area of memory allocated or NULL is no memory
  area is available.

SIDE EFFECTS
  None

===========================================================================*/
void *q_malloc (uint32 size);

/*===========================================================================
FUNCTION Q_FREE

DESCRIPTION
  Free memory from the Brew heap. Any memory allocated from the q_heap will
  be left allocated.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void q_free (void *ptr);
      
#ifdef __cplusplus /* If this is reused in a C++ environment */
  }
#endif

#endif /* Q_MALLOC_H */
