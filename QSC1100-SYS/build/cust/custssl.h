#ifndef CUSTSSL_H
#define CUSTSSL_H
/*===========================================================================

            " C u s t - S S L "   H E A D E R   F I L E

DESCRIPTION
  Configuration for Secure Sockets Layer Feature.

  Copyright (c) 2001-2002  by QUALCOMM Incorporated. All Rights Reserved.
===========================================================================*/


/*===========================================================================

                      EDIT HISTORY FOR FILE

  This section contains comments describing changes made to the module.
  Notice that changes are listed in reverse chronological order.

$PVCSPath: L:/src/asw/COMMON/vcs/custssl.h_v   1.5   30 Aug 2003 20:26:56   omichael  $ $DateTime: 2008/02/06 14:29:17 $ $Author: t_jonl $
$Header: //source/qcom/qct/core/securemsm/baselines/qsc1100/main/latest/build/cust/custssl.h#1 $ $DateTime: 2008/02/06 14:29:17 $ $Author: t_jonl $ $DateTime: 2008/02/06 14:29:17 $ $Author: t_jonl $

when       who     what, where, why
--------   ---     ----------------------------------------------------------
08/05/02   om      Created file.

===========================================================================*/


/*===========================================================================

                         PUBLIC DATA DECLARATIONS

===========================================================================*/

/*---------------------------------------------------------------------------
                          Security Services
---------------------------------------------------------------------------*/
/* Security Services
*/

#ifdef FEATURE_SEC_SSL

  /* Use DNS lookups for server names */
  #define FEATURE_SEC_SSL_DNS_LOOKUP

  /* Enable large records by increasing the DSM small item count */
  #define FEATURE_SEC_SSL_LARGE_RECORDS

  /* Use sync ssl_writes (defaults to async) */
  //#define FEATURE_SEC_SSL_SYNC_WRITE

  /* Allow async execution callbacks for error conditions only */
  #define FEATURE_SEC_SSL_ERROR_CB_ONLY
	
  /* SSL Key Exchange debugging via F3 screen */
  #define FEATURE_SEC_SSL_KEYX_DEBUG

  /* SSL logging support */
  #undef FEATURE_SEC_SSL_LOGGING

  /* Periodic logging of SSL session status */
  #undef FEATURE_SEC_SSL_PERIODIC_LOGGING

  /* SSL test app */
  #ifdef FEATURE_SEC_TESTAPPS
#error code not present
  #else
  #undef FEATURE_SEC_SSL_TESTAPP
  #endif

  /* Log the certificate .DER data */
  #undef FEATURE_SEC_SSL_LOG_CERT

  /* SSL uses the EFS to store its trusted root CAs */
  #ifdef FEATURE_EFS

    /* EFS Certificate Authorities */
    #define FEATURE_SEC_SSL_EFS_CA

    /* Allow the NULL Root CA in EFS for certificate 
    ** processing, to assist the debugging. */
    #define FEATURE_SEC_SSL_ALLOW_NULL_CA

    /* Remove the invalid CA from the EFS. */
    #define FEATURE_SEC_SSL_REMOVE_INVALID_CA

  #endif /* FEATURE_EFS */

#endif  /* FEATURE_SEC_SSL */

#endif /* CUSTSSL_H */

