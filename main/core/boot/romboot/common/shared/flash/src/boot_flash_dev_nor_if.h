#ifndef BOOT_FLASH_DEV_NOR_IF_H
#define BOOT_FLASH_DEV_NOR_IF_H

/*===========================================================================

                           B o o t   L o a d e r
             N O R  F l a s h  D e v i c e  I n t e r f a c e
                           H e a d e r  F i l e

DESCRIPTION
  This header file contains the definition of the public interface to the
  boot-specific NOR flash device module.

INITIALIZATION AND SEQUENCING REQUIREMENTS
  None

Copyright (c) 2007 by QUALCOMM, Incorporated.  All Rights Reserved.
============================================================================*/

/*===========================================================================

                           EDIT HISTORY FOR FILE

This section contains comments describing changes made to this file.
Notice that changes are listed in reverse chronological order.

$Header: //depot/asic/msmshared/secboot2/shared/1_0/flash/boot_flash_dev_nor_if.h#1 $

when       who     what, where, why
--------   ---     ----------------------------------------------------------
01/04/08   MJS     Reworked for Secure Boot 2.0.
02/06/07   MJS     Initial revision.

============================================================================*/

/*===========================================================================
 
                           INCLUDE FILES

===========================================================================*/
#include "boot_comdef.h"
#include "boot_flash_dev_if.h"

/*===========================================================================

                      PUBLIC DATA DECLARATIONS
                      
===========================================================================*/

#define BOOT_FLASH_DEV_NOR_IF_VERSION 1


/* Forward declarations */
struct boot_flash_dev_nor_if_type;


/* Definition of the NOR boot device module virtual table structure */
typedef struct boot_flash_dev_nor_vtbl_type
{
  /*----------------------------------- 
   * Generic boot flash device interface 
   *---------------------------------*/
  /* This _must_ be the first element of the structure.  Provides 
   * interface to generic boot flash device flash routines.
   * NAND-specific flash device routines and variables follow
   * the generic interface and extend the interface.
   */
  boot_flash_dev_vtbl_type parent;

  /*===========================================================================

  FUNCTION POINTER  READ_NOR

  DESCRIPTION
    This function reads data from the NOR device and copies it to the 
    destination address in RAM.

  DEPENDENCIES
    None

  RETURN VALUE
    TRUE if the read was successful, otherwise FALSE.

  SIDE EFFECTS
    None

  ===========================================================================*/
  boolean (*read_nor)
  (
    void   *ram_addr,                        /* Destination address in RAM */
    uint32  flash_addr,                      /* Logical flash source address */
    uint32  size                             /* Size to copy in bytes */
  );

} boot_flash_dev_nor_vtbl_type;


/* Definition of public interface to NOR flash device module */
typedef struct boot_flash_dev_nor_if_type
{
  /* The module version structure */
  const boot_module_version_type *vsn;

  /* The module virtual function table */
  boot_flash_dev_nor_vtbl_type *vtbl;

} boot_flash_dev_nor_if_type;


/*===========================================================================

                      PUBLIC FUNCTION DECLARATIONS

===========================================================================*/


/* Must declare the singleton before we can reference the get_singleton()
 * functions in the below inline functions */
BOOT_SINGLETON_DECLARE(boot_flash_dev_nor_if);


/*=========================================================================

                           PUBLIC MODULE API

===========================================================================*/

/*=========================================================================

FUNCTION  BOOT_FLASH_DEV_NOR_CAST_PARENT_IF

 DESCRIPTION
  Takes a pointer to the NOR boot device interface and casts to the parent
  interface type (boot device interface).

DEPENDENCIES
  None
  
RETURN VALUE
  A pointer to a boot device interface

SIDE EFFECTS
  None

===========================================================================*/
__inline boot_flash_dev_if_type* boot_flash_dev_nor_cast_parent_if
(
  boot_flash_dev_nor_if_type *dev_if
)
{
  return (boot_flash_dev_if_type*) dev_if;
} /* boot_flash_dev_nor_cast_parent_if() */

/*=========================================================================

FUNCTION  BOOT_FLASH_DEV_NOR_OPEN_IMAGE

DESCRIPTION
  Function to initialize an instance of the boot translation module, using
  information obtained from the underlying boot device.  The base address
  and region size used to initialize the translation layer are obtained
  from the boot device (e.g. partition table) based on the given image type.

DEPENDENCIES
  None

RETURN VALUE
  Pointer to the boot translation layer public interface.

SIDE EFFECTS
  None

===========================================================================*/
__inline boot_flash_trans_if_type* boot_flash_dev_nor_open_image
(
  image_type image_id                     /* Image type to initialize */
)
{
  return boot_flash_dev_nor_if_get_singleton()->vtbl->parent.open_image( image_id );
} /* boot_flash_dev_nor_open_image() */

/*=========================================================================

FUNCTION  BOOT_FLASH_DEV_NOR_OPEN_PARTITION

DESCRIPTION
  Function to initialize an instance of the boot translation module, using
  information obtained from the underlying boot device.  The base address
  and region size used to initialize the translation layer are obtained
  from the boot device (e.g. partition table) based on the given partition
  name.

DEPENDENCIES
  None

RETURN VALUE
  Pointer to the boot translation layer public interface.

SIDE EFFECTS
  None

===========================================================================*/
__inline boot_flash_trans_if_type* boot_flash_dev_nor_open_partition
(
  const char *parti_name                     /* Partition to open */
)
{
  return boot_flash_dev_nor_if_get_singleton()->vtbl->parent.open_partition( parti_name );
} /* boot_flash_dev_nor_open_partition() */

/*=========================================================================

FUNCTION  BOOT_FLASH_DEV_NOR_CLOSE_IMAGE

DESCRIPTION
  Close an instance of the boot translation layer.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
__inline void boot_flash_dev_nor_close_image
(
  boot_flash_trans_if_type **p_trans_if
)
{
  boot_flash_dev_nor_if_get_singleton()->vtbl->parent.close_image( p_trans_if );
} /* boot_flash_dev_nor_close_image() */

/*=========================================================================

FUNCTION  BOOT_FLASH_DEV_NOR_CLOSE_PARTITION

DESCRIPTION
  Close an instance of the boot translation layer.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
__inline void boot_flash_dev_nor_close_partition
(
  boot_flash_trans_if_type **p_trans_if
)
{
  boot_flash_dev_nor_if_get_singleton()->vtbl->parent.close_partition( p_trans_if );
} /* boot_flash_dev_nor_close_partition() */

/*=========================================================================

FUNCTION  BOOT_FLASH_DEV_NOR_LOAD_MIBIB_HEADER

DESCRIPTION
  Function to load an image header from the MIBIB of the flash device
  based on the given image type.  This function returns false
  if the device does not have headers in the MIBIB area.

DEPENDENCIES
  None

RETURN VALUE
  TRUE if the operation was successful, FALSE otherwise

SIDE EFFECTS
  None

===========================================================================*/
__inline boolean boot_flash_dev_nor_load_mibib_header
(
  image_type image_id,
  mi_boot_image_header_type *header_ptr   /* Pointer to image header */
)
{
  return boot_flash_dev_nor_if_get_singleton()->vtbl->parent.load_mibib_header( image_id, header_ptr );
} /* boot_flash_dev_nor_load_mibib_header() */

/*=========================================================================

FUNCTION  BOOT_FLASH_DEV_NOR_SHARE_DATA

DESCRIPTION
  Exports data shared by the boot device.

DEPENDENCIES
  None

RETURN VALUE
  TRUE if the operation was successful, FALSE otherwise

SIDE EFFECTS
  None

===========================================================================*/
__inline boolean boot_flash_dev_nor_share_data
(
  struct bl_shared_data_type    *bl_shared_data
)
{
  return boot_flash_dev_nor_if_get_singleton()->vtbl->parent.share_data( bl_shared_data );
} /* boot_flash_dev_nor_share_data() */

/*=========================================================================

FUNCTION  BOOT_FLASH_DEV_NOR_SETUP_MPU

DESCRIPTION
  Setup the MPU for the boot device to enable a trusted environment.

DEPENDENCIES
  None

RETURN VALUE
  TRUE if the operation was successful, FALSE otherwise

SIDE EFFECTS
  None

===========================================================================*/
__inline boolean boot_flash_dev_nor_setup_mpu
(
  struct bl_shared_data_type    *bl_shared_data
)
{
  return boot_flash_dev_nor_if_get_singleton()->vtbl->parent.setup_mpu( bl_shared_data );
} /* boot_flash_dev_nor_setup_mpu() */

/*===========================================================================

 FUNCTION POINTER  BOOT_FLASH_DEV_READ_NOR

DESCRIPTION
  This function reads data from the NOR device and copies it to the 
  destination address in RAM.

DEPENDENCIES
  None

RETURN VALUE
  TRUE if the read was successful, otherwise FALSE.

SIDE EFFECTS
  None

===========================================================================*/
__inline boolean boot_flash_dev_nor_read_nor
(
  void   *ram_addr,                        /* Destination address in RAM */
  uint32  flash_addr,                      /* Logical flash source address */
  uint32  size                             /* Size to copy in bytes */
)
{
  return boot_flash_dev_nor_if_get_singleton()->vtbl->read_nor( ram_addr, flash_addr, size );
} /* boot_flash_dev_nor_read_nor() */


#endif /* BOOT_FLASH_DEV_NOR_IF_H */

