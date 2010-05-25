#ifndef OI_STDDEFS_H
#define OI_STDDEFS_H
/*===========================================================================

                        EDIT HISTORY FOR MODULE

  This section contains comments describing changes made to the module.
  Notice that changes are listed in reverse chronological order.

$Header: //source/qcom/qct/wconnect/bthost/pf/rel/00.00.26/src/oi_stddefs.h#1 $  

$Log: $
*
*
*    #5        18 Oct  2006           PR
*  Merged OI Code v3.0.14rev1revQ
*
*    #4        30 May 2006            GS
* Overriding OI definitions for UINT64 and INT64 data types.
*
*
*    #3        23 Apr 2004            JH
* Remove definition of BIT22 and BIT23 which conflicts with the definition of 
*  the same name in .../services/1x/cp/distreg.h
*

===========================================================================*/
/**
 * @file    
 * This file contains Open Interface standard type definitions.
 *
 */
/**********************************************************************************
  $Revision: #1 $
  Copyright 2002 - 2003 Open Interface North America, Inc. All rights reserved.
***********************************************************************************/

#include "oi_cpu_dep.h"


#ifndef FALSE
#define FALSE 0         /**< This define statement sets FALSE as a preprocessor alias for 0. */
#endif

#ifndef TRUE
#define TRUE (!FALSE)  /**< This define statement sets TRUE as a preprocessor alias for !FALSE. */
#endif

#ifdef HEW_TOOLCHAIN
    #ifdef NULL
        #undef NULL         /**< Override HEW toolchain NULL definition */
    #endif
    #define NULL 0          /**< HEW toolchain does not allow us to compare (void*) type to function pointer */
#else
#ifndef NULL
        #define NULL ((void*)0) /**< This define statement sets NULL as a preprocessor alias for (void*)0 */
    #endif
#endif


#ifndef __INLINE__
#define __INLINE__     /**< If inlining is supported by the compiler, then __INLINE__ should be 
                            defined on the command line. Static functions that can be inlined 
                            should specify __INLINE__ in the function prototype. */
#endif

/**
 * @name  Maximum and minimum values for basic types
 * @{
 */
#define OI_INT8_MIN   ((OI_INT8)0x80)          /**< decimal value: -128 */
#define OI_INT8_MAX   ((OI_INT8)0x7F)          /**< decimal value: 127 */
#define OI_INT16_MIN  ((OI_INT16)0x8000)       /**< decimal value: -32768 */
#define OI_INT16_MAX  ((OI_INT16)0x7FFF)       /**< decimal value: 32767 */
#define OI_INT32_MIN  ((OI_INT32)0x80000000)   /**< decimal value: -2,147,483,648 */
#define OI_INT32_MAX  ((OI_INT32)0x7FFFFFFF)   /**< decimal value: 2,147,483,647 */
#define OI_UINT8_MIN  ((OI_UINT8)0)            /**< decimal value: 0 */
#define OI_UINT8_MAX  ((OI_UINT8)0xFF)         /**< decimal value: 255 */
#define OI_UINT16_MIN ((OI_UINT16)0)           /**< decimal value: 0 */
#define OI_UINT16_MAX ((OI_UINT16)0xFFFF)      /**< decimal value: 65535 */
#define OI_UINT32_MIN ((OI_UINT32)0)           /**< decimal value: 0 */
#define OI_UINT32_MAX ((OI_UINT32)0xFFFFFFFF)  /**< decimal value: 4,294,967,295 */

/**
 * @}
 */

/**
 * @name  Integer types required by the Service Discovery Protocol 
 * @{
 */ 

#ifndef OI_CODE
typedef unsigned long long UINT64;
typedef signed long long   INT64;
#endif

/** unsigned 64-bit integer as a structure of two unsigned 32-bit integers */
typedef struct {
    OI_UINT32 I1; /**< most significant 32 bits */
    OI_UINT32 I2; /**< least significant 32 bits */
} OI_UINT64;

#define OI_UINT64_MIN { (OI_UINT32)0x00000000, (OI_UINT32)0x00000000 }
#define OI_UINT64_MAX { (OI_UINT32)0XFFFFFFFF, (OI_UINT32)0XFFFFFFFF }

/** signed 64-bit integer as a structure of one unsigned 32-bit integer and one signed 32-bit integer */
typedef struct {
    OI_INT32  I1; /**< most significant 32 bits  as a signed integer */
    OI_UINT32 I2; /**< least significant 32 bits as an unsigned integer */
} OI_INT64;

#define OI_INT64_MIN { (OI_INT32)0x80000000, (OI_UINT32)0x00000000 }
#define OI_INT64_MAX { (OI_INT32)0X7FFFFFFF, (OI_UINT32)0XFFFFFFFF }

/** unsigned 128-bit integer as a structure of four unsigned 32-bit integers */
typedef struct {
    OI_UINT32 I1; /**< most significant 32 bits */
    OI_UINT32 I2; /**< second-most significant 32 bits */
    OI_UINT32 I3; /**< third-most significant 32 bits */
    OI_UINT32 I4; /**< least significant 32 bits */
} OI_UINT128;

#define OI_UINT128_MIN { (OI_UINT32)0x00000000, (OI_UINT32)0x00000000,  (OI_UINT32)0x00000000, (OI_UINT32)0x00000000 }
#define OI_UINT128_MAX { (OI_UINT32)0XFFFFFFFF, (OI_UINT32)0XFFFFFFFF,  (OI_UINT32)0XFFFFFFFF, (OI_UINT32)0XFFFFFFFF }

/** signed 128-bit integer as a structure of three unsigned 32-bit integers and one signed 32-bit integer */
typedef struct {
    OI_INT32  I1;  /**< most significant 32 bits as a signed integer */
    OI_UINT32 I2;  /**< second-most significant 32 bits as an unsigned integer */
    OI_UINT32 I3;  /**< third-most significant 32 bits as an unsigned integer */
    OI_UINT32 I4;  /**< least significant 32 bits as an unsigned integer */
} OI_INT128;

#define OI_INT128_MIN { (OI_UINT32)0x80000000, (OI_UINT32)0x00000000,  (OI_UINT32)0x00000000, (OI_UINT32)0x00000000 }
#define OI_INT128_MAX { (OI_UINT32)0X7FFFFFFF, (OI_UINT32)0XFFFFFFFF,  (OI_UINT32)0XFFFFFFFF, (OI_UINT32)0XFFFFFFFF }

/**
 * @}
 */


/**
 * type for ASCII character data items
 */
typedef char OI_CHAR;

/**
 * type for double-byte character data items
 */
typedef OI_UINT16 OI_CHAR16;

/**
 * types for UTF encoded strings.
 */
typedef OI_UINT8  OI_UTF8;
typedef OI_UINT16 OI_UTF16;
typedef OI_UINT32 OI_UTF32;


/**
 * @name Single-bit operation macros
 * @{
 * In these macros, x is the data item for which a bit is to be tested or set and y specifies which bit
 * is to be tested or set.
 */

/** This macro's value is TRUE if the bit specified by y is set in data item x. */
#define OI_BIT_TEST(x,y)   ((x) & (y))

/** This macro's value is TRUE if the bit specified by y is not set in data item x. */
#define OI_BIT_CLEAR_TEST(x,y)  (((x) & (y)) == 0)

/** This macro sets the bit specified by y in data item x. */
#define OI_BIT_SET(x,y)    ((x) |= (y))

/** This macro clears the bit specified by y in data item x. */
#define OI_BIT_CLEAR(x,y)  ((x) &= ~(y))

/** @} */

/**
 * The OI_ARRAYSIZE macro is set to the number of elements in an array 
 * (instead of the number of bytes, which is returned by sizeof()).
 */

#ifndef OI_ARRAYSIZE
#define OI_ARRAYSIZE(a) (sizeof(a)/sizeof(a[0]))
#endif

/**
 * @name Preprocessor aliases for individual bit positions
 *      Bits are defined here only if they are not already defined.
 * @{
 */
 
#ifndef BIT0

#define BIT0   0x00000001  /**< preprocessor alias for 32-bit value with bit 0 set, used to specify this single bit */
#define BIT1   0x00000002  /**< preprocessor alias for 32-bit value with bit 1 set, used to specify this single bit */
#define BIT2   0x00000004  /**< preprocessor alias for 32-bit value with bit 2 set, used to specify this single bit */
#define BIT3   0x00000008  /**< preprocessor alias for 32-bit value with bit 3 set, used to specify this single bit */
#define BIT4   0x00000010  /**< preprocessor alias for 32-bit value with bit 4 set, used to specify this single bit */
#define BIT5   0x00000020  /**< preprocessor alias for 32-bit value with bit 5 set, used to specify this single bit */
#define BIT6   0x00000040  /**< preprocessor alias for 32-bit value with bit 6 set, used to specify this single bit */
#define BIT7   0x00000080  /**< preprocessor alias for 32-bit value with bit 7 set, used to specify this single bit */
#define BIT8   0x00000100  /**< preprocessor alias for 32-bit value with bit 8 set, used to specify this single bit */
#define BIT9   0x00000200  /**< preprocessor alias for 32-bit value with bit 9 set, used to specify this single bit */
#define BIT10  0x00000400  /**< preprocessor alias for 32-bit value with bit 10 set, used to specify this single bit */
#define BIT11  0x00000800  /**< preprocessor alias for 32-bit value with bit 11 set, used to specify this single bit */
#define BIT12  0x00001000  /**< preprocessor alias for 32-bit value with bit 12 set, used to specify this single bit */
#define BIT13  0x00002000  /**< preprocessor alias for 32-bit value with bit 13 set, used to specify this single bit */
#define BIT14  0x00004000  /**< preprocessor alias for 32-bit value with bit 14 set, used to specify this single bit */
#define BIT15  0x00008000  /**< preprocessor alias for 32-bit value with bit 15 set, used to specify this single bit */
#define BIT16  0x00010000  /**< preprocessor alias for 32-bit value with bit 16 set, used to specify this single bit */
#define BIT17  0x00020000  /**< preprocessor alias for 32-bit value with bit 17 set, used to specify this single bit */
#define BIT18  0x00040000  /**< preprocessor alias for 32-bit value with bit 18 set, used to specify this single bit */
#define BIT19  0x00080000  /**< preprocessor alias for 32-bit value with bit 19 set, used to specify this single bit */
#define BIT20  0x00100000  /**< preprocessor alias for 32-bit value with bit 20 set, used to specify this single bit */
#define BIT21  0x00200000  /**< preprocessor alias for 32-bit value with bit 21 set, used to specify this single bit */
#ifdef OI_CODE /* conflict with definition in .../services/1x/cp/distreg.h */
#error code not present
#endif /* OI_CODE */
#define BIT24  0x01000000  /**< preprocessor alias for 32-bit value with bit 24 set, used to specify this single bit */
#define BIT25  0x02000000  /**< preprocessor alias for 32-bit value with bit 25 set, used to specify this single bit */
#define BIT26  0x04000000  /**< preprocessor alias for 32-bit value with bit 26 set, used to specify this single bit */
#define BIT27  0x08000000  /**< preprocessor alias for 32-bit value with bit 27 set, used to specify this single bit */
#define BIT28  0x10000000  /**< preprocessor alias for 32-bit value with bit 28 set, used to specify this single bit */
#define BIT29  0x20000000  /**< preprocessor alias for 32-bit value with bit 29 set, used to specify this single bit */
#define BIT30  0x40000000  /**< preprocessor alias for 32-bit value with bit 30 set, used to specify this single bit */
#define BIT31  0x80000000  /**< preprocessor alias for 32-bit value with bit 31 set, used to specify this single bit */

#endif  /* BIT0 et al */


/** @} */

/*****************************************************************************/
#endif /* OI_STDDEFS_H */

