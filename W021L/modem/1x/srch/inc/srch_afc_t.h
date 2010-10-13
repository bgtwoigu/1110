#ifndef SRCH_AFC_T_H
#define SRCH_AFC_T_H
/*===========================================================================

             S E A R C H   A F C   H E A D E R    F I L E

DESCRIPTION
  This file contains type declarations for use by the Search AFC.

    Copyright (c) 2004 - 2008
                  by QUALCOMM, Inc.  All Rights Reserved.
===========================================================================*/

/*===========================================================================

                      EDIT HISTORY FOR FILE

This section contains comments describing changes made to this file.
Notice that changes are listed in reverse chronological order.

$Header: //source/qcom/qct/modem/1x/srch/rel/1h08/inc/srch_afc_t.h#1 $

when       who     what, where, why
--------   ---     ----------------------------------------------------------
07/28/08   aps     T_MSM8650B featurization, setting rot_factor to 4
11/09/07   aps     T_MDM7800 featurization, setting rot_factor to 4
10/09/07   sst     Created SRCH_AFC_ROT_SHIFT
04/12/07   sst     Changes to support new VCTCXO image for QSC60x5
06/06/06   pa      Added types for vco, rot and ppm values.
07/14/05   sfm     Mainlined FEATURE_1X_SRCH_USES_ROTATOR_AFC
09/14/04   jcm     Moved SRCH_AFC_LOG_BUFFER_SIZE from srchlog.h
09/14/04   jcm     Moved AFC types to srch_afc_t.h

===========================================================================*/

/*==========================================================================
                     INCLUDE FILES FOR MODULE
===========================================================================*/
#include "tcxomgr.h"


/*===========================================================================
             DEFINITIONS AND DECLARATIONS FOR MODULE
===========================================================================*/

/*=========================================================================
      Constants
==========================================================================*/

/* Defines the buffer size for the log packets */
/* This must be less than 255, since the variable used for indexing
   the buffers are 8 bit wide */
#define SRCH_AFC_LOG_BUFFER_SIZE 10

/* factor for Rotator units
   The units used in the code assumes that the rotator is in units
   of 1.1719 Hz per LSB. This define is used to correct instances
   where a specific error needs to be applied to the rotator with
   a value that was not originally read from the Rotator
   SRCH_AFC_ROT_FACTOR = (Target's Units) / 1.1719
   This should be a factor of 2, so the corresponding ROT_SHIFT
   indicates the number of bits need to be shifted
*/
#if ( ( defined( FEATURE_XO ) ) || ( defined( T_QSC60X5 ) )  || \
      ( defined( T_MDM7800 )) || defined( T_MSM8650B ))
/* for targets with FEATURE_XO defined or the QSC60X5 series,
   the Rotator is in units of 4.6875 Hz per LSB, or approx
   1/4 of the units */
#define SRCH_AFC_ROT_FACTOR     4
#define SRCH_AFC_ROT_SHIFT      2
#else
#error code not present
#endif

/*=========================================================================
      Typedefs
==========================================================================*/
/* AFC operating mode types - determines amount of feedback to trk_lo */
typedef enum
{
  /* legacy AFC. The AFC loop controls the VC-TCXO only */
  SRCH_AFC_MODE_VCTCXO_ONLY,

  /* Dual loop AFCs , with a fast inner loop controlling the finger rotators
     and a slow outer loop controlling the VC-TCXO. */

  /* VC-TCXO loop has a fixed bandwidth independent of received Ec/Io */
  SRCH_AFC_MODE_FIXED_VCO_ROT_AFC,

  /* VC-TCXO loop bandwidth is proportional to the received Ec/Io,
     similar to the inner loop bandwidth. */
  SRCH_AFC_MODE_ECIO_BASED_VCO_ROT_AFC,

  /* Rotator-only tracking */
  SRCH_AFC_MODE_ROTATOR_ONLY,

  /* Must remain last */
  SRCH_AFC_OPER_MODE_COUNT

} srch_afc_mode_enum_type;

/* AFC gain modes - determines bandwidth of rotator and vctcxo loops */
typedef enum
{
  SRCH_AFC_ACQ_XFER_GAIN = 0,  /* acquisition/transfer gains (quick) */
  SRCH_AFC_TRACKING_GAIN,      /* tracking gain (slow & smooth) */

  /* in Fixed AFC mode, the Gains are tied to Band class.  For consistency,
     I'm including them here, mapping them OVER the other two values. */
  SRCH_AFC_FIXED_CELL_GAIN = 0,
  SRCH_AFC_FIXED_PCS_GAIN =1,

  /* must stay last */
  SRCH_AFC_NUM_GAIN_MODES

} srch_afc_gain_mode_type;

typedef int16 srch_afc_vco_type;                 /* type for vco value     */
typedef int16 srch_afc_rot_type;                 /* type for rot value     */
typedef int16 srch_afc_ppm_type;                 /* type for ppm value     */
typedef int32 srch_afc_hr_rot_type;              /* type for HR rot value
                                                    always 1.1719/4 Hz/LSB */

/*=========================================================================
      Macros
==========================================================================*/

#endif /* SRCH_AFC_T_H */

