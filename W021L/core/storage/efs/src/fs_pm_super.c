/*===========================================================================

 fs_pm_super.c

 Superblock management

 Copyright (c) 2002-2009 by QUALCOMM Incorporated.
 All Rights Reserved.

 ===========================================================================

                        EDIT HISTORY FOR MODULE

  This section contains comments describing changes made to the module.
  Notice that changes are listed in reverse chronological order.

  $Header: //source/qcom/qct/core/storage/efs/main/latest/src/fs_pm_super.c#4 $ $DateTime: 2009/01/29 17:35:24 $ $Author: richardp $

when          who     what, where, why
--------      ---     ------------------------------------------------------
2009-01-20     yg     Allow configured number of bad blocks.
2008-11-03     rp     Introduce device abstraction.
2008-06-02     rp     Store latest super-page-id in RAM.
2008-05-08     rp     Handle write-failure in log-region.
2007-11-02     rp     Move fs_fresh_start variable to the fs_public.c file.
2007-10-18     rp     Featurized QEFS2.
2007-08-10     rp     Align buffers passed to flash on a 32-byte boundary.
2007-07-06     yg     Version handler to handle upgrades to dual alloc ver.
2007-06-13     yg     Added global to get region info for unit test.
2007-05-25    umr     Added support for EFS memory protection feature.
2007-02-23    dlb     Move read failures into device wrappers.
2006-12-13    sh      Lint fixes.
2006-10-09    dlb     Fix superblock version on fresh starts.
2006-09-11    dlb     Use flash driver wrappers.
2006-09-06    sh      Lint cleanup
2006-08-31    dlb     Add db gid upgrade support.
2006-08-31    dlb     Only upgrade version after upgrade is finished.
2006-08-17    dlb     Add log CRC upgrade support.
2006-08-07    dlb     Rename log2 to avoid C library conflicts.
2006-04-18    dlb     Add forward-updates of superblock.
2006-03-31    sh      Lint fixes.
2006-02-08    dlb     Fix erroneous dependency on write style.
2006-01-27    dlb     Use encapsulated write style query.
2006-01-17    dlb     Support multiple NOR write styles.
2005-12-14    dlb     Extra asserts for configuration sanity.
2005-10-30     sh     Lint cleanup.
2005-04-20    dlb     Support 2k pages.
2005-02-18    dlb     Lint fix.
2005-01-17    dlb     Cleanup from code review.
2005-01-04    dlb     Update copyright line.
2004-12-27    dlb     Replace factory image code.
2004-10-15    dlb     Update copyright header.
2004-10-08    dlb     Repair NOR and NAND fresh start.
2004-10-07    dlb     Whitespace cleanup.
2003-11-17    dlb     Check partition size on startup.
2003-11-13    dlb     Reduce R0 region to 0.
2003-09-09    dlb     Factory image works with bitmap and tree.
2003-06-18     gr     Fixed a bug in the factory start code. Also cleaned up
                      the formatting of some code.
2003-06-17    adm     Featurized factory start code.
2003-06-12    adm     Add support for factory start.
2003-05-03     gr     Added support for invalidating all superblocks. This
                      forces a fresh start on the next reboot.
2003-03-21     gr     Code cleanup.
2003-03-17     gr     Added a crc to the superblock.
2003-03-07    dlb     Initialize the superblock tables.
2002-08-08    drh     Created by dlb.  Added history header.  Change #if 1
                      surrounding printf() calls to #if 0
===========================================================================*/

#include "fs_efs2_cust.h"
#include "fs_device.h"

#ifdef FEATURE_EFS_QEFS2
#if defined (FEATURE_EFS_EFS2_ON_NAND) ||   \
    defined (FEATURE_EFS_EFS2_ON_NOR)

#include <stdio.h>
#include <string.h>
#include <stddef.h>

#include "customer.h"
#include "fs_sys_types.h"
#include "fs_err.h"

#include "fs_upgrade.h"

#include "fs_pm_super.h"
#include "fs_device.h"
#include "assert.h"

#include "crc.h"

#ifdef FEATURE_EFS_NAND_FACTORY_START
 #include "fs_factory.h"
#endif

#include "fs_protected_memory.h"
#include "fs_public.h"

/* Superblock and flash memory layout.
 *
 * Flash (at least NAND) is divided into a set of one or more regions.  The
 * first region (region 0) is always used to store user data, as well as
 * superblocks and log pages.
 *
 * This region 0 contains 4 copies of the superblock.  One of these will
 * always be the newest version.
 *
 * To make finding the superblocks easier, the locations are determined as
 * if there were no regions.  If the device is inefficient, and the forth
 * superblock doesn't fit in the last region, we will only use three.
 * More likely, though, would be that there would only be one region in
 * this case.
 *
 * The superblock code also computes the initial region boundaries when
 * there are no superblocks present.  Once defined, the regions will be
 * static for the remainder of the life of the filesystem on that device.
 * The relative sizes of the regions should be determined through
 * profiling. */

/* A pointer to the superblock data structure. Used for master reset
 * functionality.
 *
 * XXX: This needs to be removed to support multiple NAND
 * devices/partitions.
 */
static fs_super_t sb_ptr;

/* Return the smallest m, such taht 2^m >= value. */
static int fs_log2 (int value);

/* Compute the offsets where we would expect to find superblocks.  Bad
 * blocks may cause these to be advances, but this gives us our initial
 * guesses. */
/* static void compute_initial_superblock_offsets (fs_super_t super); */

/* Scan through the device looking for the most recent superblock.  Returns
 * the page this superblock is located in, or FS_INVALID_PAGE_ID if there was
 * no superblock found. */
static fs_page_id find_superblock (fs_super_t super);

/* Determine if this superblock version is compatible.  Returns non-zero if
 * it is. */
static int superblock_version_compatible (int is_nand, uint32 version);

static void compute_page_tables (fs_super_t super);

#ifdef FS_UNIT_TEST
#error code not present
#endif

/* Initial scan for the superblock.  This is done once on powerup.  It also
 * will compute and write out a clean superblock if one could not be found.
 */
void
fs_super_init (
    fs_super_t  super,
    efs_device_t dev,
    int *fresh_start)
{
  int block_size, page_size;
  fs_page_id offset;

  sb_ptr = super;

  block_size = fs_device_block_size (dev);
  page_size = fs_device_page_size (dev);

  /* Make sure the compiled page size matches the device page size. */
  if (page_size != EFS_PAGE_SIZE) {
    FS_ERR_FATAL ("Device page size different than configured", 0, 0 ,0);
  }

  /* Assert that our size is correct, just a coding sanity check. */
  if (sizeof (struct superblock_data) != EFS_PAGE_SIZE) {
    FS_ERR_FATAL ("Superblock does not match page size: super=%d, page=%d",
        sizeof (struct superblock_data), EFS_PAGE_SIZE, 0);
  }

  /* EFS2 uses shifts for block and page arithmetic, so make sure they are
   * both powers of two. */
  ASSERT ((block_size & (block_size - 1)) == 0);
  ASSERT ((page_size & (page_size - 1)) == 0);

  /* Initialize the obvious fields of the superblock data. */
  super->block_shift = fs_log2 (block_size);
  super->block_mask = 0xFFFFFFFF << (super->block_shift);
  super->dev = dev;
  super->latest_super_page = FS_INVALID_PAGE_ID;

  /* compute_initial_superblock_offsets (super); */

  offset = find_superblock (super);

#ifdef FEATURE_EFS_NAND_FACTORY_START
  {
    int is_nand = fs_device_is_nand_device (dev);

    /* If we didn't find a superblock, and we know this is a NAND device
     * (only NOR allows partial writes), then search for a factory image. */
    if ( (offset == FS_INVALID_PAGE_ID) && (is_nand) )
    {
      offset = fs_fact_startup_check (super, dev);
    }
  }
#endif

  /* Also, if someone with a debugger is requesting a fresh start, do so.
   */
  if (fs_fresh_start)
    offset = FS_INVALID_PAGE_ID;

  /* printf ("Use superblock at: %d (%d)\n", offset,
      super->segment_offsets[offset]); */

  /* If we didn't find any superblocks, create an initial one. */
  if (offset == FS_INVALID_PAGE_ID) {
    /* XXX: Fix to pass argument to support multiple devices. */

    /* Always use the latest version when creating a superblock.  Don't
     * want to ever see an old version in this case. */
    super->data->version = super->desired_version;

    fs_pm_super_invalidate_superblocks ();
    fs_super_create_superblock (super);
    *fresh_start = 1;
  }
  else
  {
    /* offset has the latest super page that we found. */
    super->latest_super_page = offset;

    /* The NOR specific superblock data was originally initialized to zero,
     * which happens to be the same as FS_DEVICE_WRITES_SIMPLE.  This check
     * here allows us to support the other flash devices without having to
     * bump the superblock version.  Simple devices will continue to work,
     * with the zero matching the enum. */
    if (FS_ISNOR (super) &&
        super->data->u.nor.style != FS_GET_WRITE_STYLE (super->dev))
    {
      /* This fatal error indicates that we have detected a previous
       * filesystem that didn't have proper support for multi-bit flash
       * devices.  The filesystem must be wiped manually to create a fresh
       * filesystem. */
      FS_ERR_FATAL ("Cannot transition from incorrect multi-bit support",
          0, 0, 0);
    }
    *fresh_start = 0;
  }

  super->total_pages = super->data->block_count * super->data->block_size;
#ifdef FS_UNIT_TEST
#error code not present
#endif
}

/* Compute the smallest m, such that 2^m >= value.  This could be
 * optimized, but isn't done too much. */
static int
fs_log2 (int value)
{
  int m = 0;
  while (1) {
    if ((1 << m) >= value)
      return m;
    m++;
  }
}

/* Determine if the first argument is greater or less than the second, in
 * terms of "age" which is a uint16. */
static int
age_less (uint16 a, uint16 b)
{
  if (a < 16384 && b >= 49152)
    return 0;
  if (b < 16384 && a >= 49152)
    return 1;
  return a < b;
}

/* Return 1 if the superblock passed in is valid and 0 if not.
 */
static int
check_superblock (struct superblock_data *sd, int is_nand,
    int block_count, int block_size)
{
  if (sd == NULL
      || sd->magic1 != FS_SUPER_MAGIC1
      || sd->magic2 != FS_SUPER_MAGIC2
      || !superblock_version_compatible (is_nand, sd->version))
    return 0;

  if (sd->block_size != (unsigned) block_size ||
      sd->block_count != (unsigned) block_count)
    return 0;

  /* Check 30 bit crc. */
  if (crc_30_calc ((byte *) sd, sizeof(struct superblock_data) * 8 - 32)
      != sd->crc32)
    return 0;
  else
    return 1;
}

/* Scan the appropriate region for a superblock.  For each valid superblock
 * we find, look at the age field.  Determine from this field which
 * superblock is the most recent. */
static fs_page_id
find_real_superblock (fs_super_t super, int is_nand)
{
  int result;
  uint32 block_size = fs_device_block_size (super->dev);
  uint32 block_count = fs_device_block_count (super->dev);
  uint32 total_pages = block_size * block_count;
  fs_page_id page;
  struct superblock_data *sd;
  uint16        max_age  = 0;
  fs_page_id    max_page = FS_INVALID_PAGE_ID;

  /* Scan the entire NOR flash for a superblock, but only the last several
   * blocks of a NAND flash. */

  if (is_nand)
    page = total_pages - (FS_NAND_LOG_REGION_BLOCKS << super->block_shift);
  else
    page = 0;

  for (; page < total_pages; page += block_size)
  {
    if (is_nand)
    {
      /* For NAND, we must read each page out. */
      sd = NULL;

      /* Read this page only if it is not marked as bad. */
      result = fs_device_bad_block_check (super->dev,
                                         page >> super->block_shift);
      if (result != FS_DEVICE_BAD_BLOCK)
      {
        result = fs_device_read_page (super->dev, page, (void *) super->data,
                                     FS_FOP_SUPER, FS_FLASH_FAIL_OK);
        if (result == FS_DEVICE_OK)
          sd = super->data;
      }
    }
    else
    {
      /* NOR lets us have pointers directly into the flash. */
      sd = fs_device_read_pointer (super->dev, page);
    }

    /* XXX: Verify checksum. */
    if (check_superblock (sd, is_nand, block_count, block_size))
    {
      if (max_page == FS_INVALID_PAGE_ID
          || (age_less (max_age, sd->age)))
      {
        max_age = sd->age;
        max_page = page;
      }
    }
  }

  if (max_page != FS_INVALID_PAGE_ID) {
    fs_device_read_page (super->dev, max_page,
        (void *) super->data, FS_FOP_SUPER, FS_FLASH_MUST_WORK);

    /* Remember the desired superblock version number.  fs_super_update()
     * will start using this once any updates are finished. */
    if (is_nand)
      super->desired_version = FS_SUPER_VERSION_NAND;
    else
      super->desired_version = FS_SUPER_VERSION_NOR;
  } else {
    /* Set the version field in the superblock so that the
     * create_superblocks can create the correct type. */
    if (is_nand)
      super->data->version = FS_SUPER_VERSION_NAND;
    else
      super->data->version = FS_SUPER_VERSION_NOR;
    super->desired_version = super->data->version;
  }

  return max_page;
}

/* Scan each block of the appropriate region for superblock. */
static fs_page_id
find_superblock (fs_super_t super)
{
  int is_nand = fs_device_is_nand_device (super->dev);
  return find_real_superblock (super, is_nand);
}

/* Create the initial superblock.  It is always written in the first
 * segment.  We assume that the device is unitialized.  The
 * find_superblocks function will have previously set the
 * super->data->version field to the correct version type to use (either
 * NAND or NOR).  We will base some of our initialization decisions on this
 * information. */
void
fs_super_create_superblock (fs_super_t super)
{
  int i;
  uint32 old_version = super->data->version;

  memset (super->data, 0, sizeof (struct superblock_data));

  /* Fill in all of the obvious information for the superblock. */
  super->data->page_header = 0x53000000;
  super->data->version = old_version;
  super->data->age = 0;
  super->data->magic1 = FS_SUPER_MAGIC1;
  super->data->magic2 = FS_SUPER_MAGIC2;
  super->data->block_size = fs_device_block_size (super->dev);
  super->data->page_size = fs_device_page_size (super->dev);
  super->data->block_count = fs_device_block_count (super->dev);

  /* Clear out the tables. */
  super->data->log_head = FS_INVALID_PAGE_ID;

  for (i = 0; i < FS_UPPER_DATA_COUNT; i++) {
    super->data->upper_data[i] = FS_INVALID_PAGE_ID;
  }

  if (FS_ISNOR (super)) {
    super->data->u.nor.style = FS_GET_WRITE_STYLE (super->dev);
  } else {
    /* Compute the NAND specific information. */
    compute_page_tables (super);

    for (i = 0; i < SUPERBLOCK_PAGE_ENTRIES; i++) {
      super->data->u.nand.tables[0][i] = FS_INVALID_PAGE_ID;
      super->data->u.nand.tables[1][i] = FS_INVALID_PAGE_ID;
    }
  }
}

/* Compute the sizes of the page tables. */
/* XXX: Handle the case where the last chunk of nodes doesn't fit in the
 * superblock. */
/* XXX: Do we need to compute based on potential blocks, or is this number
 * adequate. */
static void
compute_page_tables (fs_super_t super)
{
  int low_shift;
  long page_number;
  int levels = 0;
  long page_total, page_tmp, tmp;

  super->data->u.nand.nodes_per_page = super->data->page_size / 4;
  low_shift = fs_log2 (super->data->u.nand.nodes_per_page);

  /* Starting with the last possible page. */
  page_number = super->data->block_count * super->data->block_size - 1;

  while (page_number + 1 > SUPERBLOCK_PAGE_ENTRIES) {
    levels++;
    page_number >>= low_shift;
  }

  levels++;

  super->data->u.nand.page_depth = levels;
  super->data->u.nand.super_nodes = page_number + 1;

  /* Compute the total number of pages needed for page tables. */
  page_total = 0;
  page_tmp = super->data->block_count * super->data->block_size;

  while (page_tmp >= SUPERBLOCK_PAGE_ENTRIES) {
    page_tmp = (page_tmp + super->data->u.nand.nodes_per_page - 1) /
      super->data->u.nand.nodes_per_page;
    page_total += page_tmp;
  }
  page_total *= 2;

  /* Compute the boundaries of the various regions. */
  super->data->u.nand.num_regions = FS_NAND_REGION_COUNT;

  /* The log ends at the end of the device. */
  super->data->u.nand.regions[3] = super->data->block_count;

  /* The log and superblocks are the last region. */
  tmp = FS_NAND_LOG_REGION_BLOCKS;

  super->data->u.nand.regions[2] = super->data->u.nand.regions[3] - tmp;

  /* R0 region is a placeholder for possible future use. */
  tmp = 0;
  super->data->u.nand.regions[1] = super->data->u.nand.regions[2] - tmp;

  /* Compute space needed for the page tables.  The multiplication factor
   * here is mostly a performance tradeoff. */
  tmp = ((page_total + super->data->block_size - 1)
          / super->data->block_size) * 2 + 3 + FS_MIN_PTABLE_BAD_BLOCK_ALLOWED;

  super->data->u.nand.regions[0] = super->data->u.nand.regions[1] - tmp;

  if (super->segment_offsets[SUPER_SEGMENTS - 1]
      >= super->data->u.nand.regions[0]) {
    FS_ERR_FATAL ("Regions are too large to be useful here.", 0, 0, 0);
  }

#ifdef FS_UNIT_TEST_NOT
#error code not present
#endif
}

/* Do the actual update of the superblock. */
int
fs_super_update (
    fs_super_t  super,
    fs_page_id  page)
{
  int result;

  super->data->age++;

  /* If all of the updates are finished, bounce us to the newest version.
   */
  if (super->data->version != super->desired_version &&
      fs_upgrades_all_finished())
  {
    super->data->version = super->desired_version;
  }

  /* Compute and store a 30-bit CRC. */
  super->data->crc32 = crc_30_calc ((byte *) super->data,
      sizeof (struct superblock_data) * 8 - 32);

  result = fs_device_write_page (super->dev, page, (void *) super->data,
      FS_FOP_SUPER);

  /* XXX: Error recovery. */
  if (result != FS_DEVICE_DONE)
  {
    if (FS_ISNOR (super))
    {
      FS_ERR_FATAL ("Unable to write updated superblock", 0, 0, 0);
    }
  }

  return result;
}

/*
 * Erase all the blocks that contain superblocks. NAND-specific.
 */
static void
erase_nand_superblocks (fs_super_t super)
{
  fs_block_id block;
  fs_block_id block_count = fs_device_block_count (super->dev);
  int result;

  block = block_count - FS_NAND_LOG_REGION_BLOCKS;
  for (; block < block_count; block++)
  {
    result = fs_device_bad_block_check (super->dev, block);
    if (result == FS_DEVICE_BAD_BLOCK)
      continue; /* bad-block, so proceed with next block*/

    result = fs_device_erase_block (super->dev, block, FS_FOP_UNKNOWN);
    if (result != FS_DEVICE_OK)
    {
      result = fs_device_mark_block_bad (super->dev, block);
      if (result != FS_DEVICE_OK)
        FS_ERR_FATAL ("fs_mark_block_bad failed",0,0,0);
    }
  }
}

/*
 * Set the magic number in all superblocks to zero. NOR-specific.
 */
static void
invalidate_nor_superblocks (fs_super_t super)
{
  int page;
  fs_block_id block;
  int block_size = fs_device_block_size (super->dev);
  int block_count = fs_device_block_count (super->dev);
  int page_size = fs_device_page_size (super->dev);
  int total_pages = block_size * block_count;
  int magic1_offset = offsetof (struct superblock_data, magic1);
  int magic_reset_val = 0;

  page = 0;
  block = 0;
  for (; page < total_pages; page += block_size, block++) {
    /* Invalidate the superblock, as well as the last word of the last page
     * in the block (used for the ptables). */
    fs_device_partial_write (super->dev, page, &magic_reset_val,
        magic1_offset, 4);
    fs_device_partial_write (super->dev, page + block_size - 1,
        &magic_reset_val, page_size - 4, 4);
  }
}

/* Invalidate all the superblocks in the flash. On NOR flash, we do this by
 * setting one of the magic numbers in the superblock to zero. On NAND, we
 * erase all the blocks that contain superblocks.
 */
void
fs_pm_super_invalidate_superblocks (void)
{
  int is_nand = 0;

  if (sb_ptr)
  {
    is_nand = fs_device_is_nand_device (sb_ptr->dev);
    if (is_nand)
      erase_nand_superblocks (sb_ptr);
    else
      invalidate_nor_superblocks (sb_ptr);
  }
  else
  {
    FS_ERR_FATAL ("Invalid superblock pointer, master reset failed", 0, 0, 0);
  }
}

/* Determine if the detected superblock is compatible with this filesystem
 * code.  'find_real_superblock' will update the version field to the current
 * version.  This handles the case where there is a simple forward
 * compatibility issue, such as new log codes.  If any effort is needed to
 * convert data structures that will need to be added. */
static int
superblock_version_compatible (int is_nand, uint32 version)
{
  if (version < FS_SUPER_LOWEST_ENCODED_VERSION) {
    /* No versions before the encoding were supported. */
    return 0;
  }

  /* If the flash type mismatches, something is rather wrong. */
  if (is_nand != FS_SUPER_VERSION_IS_NAND (version)) {
    FS_ERR_FATAL ("Flash changed major types.", 0, 0, 0);
  }

  /* Now figure out what versions we support.  This case statement
   * intentionally has fall-throughs, since older versions will require
   * multiple upgrades.  The cases should be listed oldest first. */
  switch (FS_SUPER_VERSIONOF (version)) {

    case FS_SUPER_VERSION_QANDR:
      /* Long names added since then.  This is a forward-compatible change. */
    case FS_SUPER_VERSION_LONGNAME:
      /* Log CRC added.  This is a forward-compatible change. */

      /* Upgrades from these versions require in-place log updates. */
      fs_upgrade_add (FS_UPGRADE_LOG_ZERO_AFTER_CRC);
      //lint -fallthrough

    case FS_SUPER_VERSION_LOGCRC:
      /* Fix GID problem in database nodes. */
      fs_upgrade_add (FS_UPGRADE_DBGID_FIX);
      //lint -fallthrough

    case FS_SUPER_VERSION_DBGID:
      /* Fix the dual allocator in PM */
      fs_upgrade_add (FS_UPGRADE_DUAL_ALLOC_FIX);
      //lint -fallthrough

    case FS_SUPER_VERSION_DUAL_ALLOC:
      /* Native version. */
      break;

    default:
      /* Unknown previous version (probably newer).  Emit warning, and
       * indicate it is old. */
      FS_ERR ("Unsupported superblock version: 0x%04x",
          version, 0, 0);
      return 0;
  }

  return 1;
}

#endif /* defined (FEATURE_EFS_EFS2_ON_NAND) || (FEATURE_EFS_EFS2_ON_NOR) */
#endif /* FEATURE_EFS_QEFS2 */

