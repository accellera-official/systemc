/*****************************************************************************

  Licensed to Accellera Systems Initiative Inc. (Accellera) under one or
  more contributor license agreements.  See the NOTICE file distributed
  with this work for additional information regarding copyright ownership.
  Accellera licenses this file to you under the Apache License, Version 2.0
  (the "License"); you may not use this file except in compliance with the
  License.  You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

  Unless required by applicable law or agreed to in writing, software
  distributed under the License is distributed on an "AS IS" BASIS,
  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or
  implied.  See the License for the specific language governing
  permissions and limitations under the License.

 *****************************************************************************/

/*****************************************************************************

  sc_nbdefs.h -- Top level header file for arbitrary precision signed/unsigned
                 arithmetic. This file defines all the constants needed.

  Original Author: Ali Dasdan, Synopsys, Inc.

 *****************************************************************************/

/*****************************************************************************

  MODIFICATION LOG - modifiers, enter your name, affiliation, date and
  changes you are making here.

      Name, Affiliation, Date: Torsten Maehne, Berner Fachhochschule, 2016-09-24
  Description of Modification: Move constant definitions to the header so that
                               that their value is known at compile time.

 *****************************************************************************/

// $Log: sc_nbdefs.h,v $
// Revision 1.7  2011/02/18 20:19:15  acg
//  Andy Goodrich: updating Copyright notice.
//
// Revision 1.6  2011/02/18 20:09:34  acg
//  Philipp A. Hartmann: added alternative #define for Windows to guard.
//
// Revision 1.5  2011/01/20 16:52:20  acg
//  Andy Goodrich: changes for IEEE 1666 2011.
//
// Revision 1.4  2010/02/08 18:35:55  acg
//  Andy Goodrich: Philipp Hartmann's changes for Solaris and Linux 64.
//
// Revision 1.2  2009/05/22 16:06:29  acg
//  Andy Goodrich: process control updates.
//
// Revision 1.1.1.1  2006/12/15 20:20:05  acg
// SystemC 2.3
//
// Revision 1.3  2006/01/13 18:49:32  acg
// Added $Log command so that CVS check in comments are reproduced in the
// source.
//

#ifndef SC_NBDEFS_H
#define SC_NBDEFS_H


#include "sysc/kernel/sc_cmnhdr.h"

#include <climits>

#if defined(__sun) || defined(__sun__)
#  include <inttypes.h>
#elif !defined(WIN32) && !defined(_WIN32)
#  include <stdint.h>
#endif

#include "sysc/utils/sc_iostream.h"
#include "sysc/utils/sc_string.h"       // For sc_numrep

// Activate support mixed operands for concatenation via the comma operator
#define SC_DT_MIXED_COMMA_OPERATORS


namespace sc_dt
{
// BIGINT CONFIGURATIONS
//
// One of these three #defines should be defined, but only one:
//
// SC_BIGINT_CONFIG_TEMPLATE_CLASS_HAS_NO_BASE_CLASS:
//     Configure sc_bigint and sc_biguint so that they do not have parent classes. That is,
//     sc_signed is not a parent of sc_bigint, and sc_unsigned is not a parent of sc_biguint.
//
// SC_BIGINT_CONFIG_TEMPLATE_CLASS_HAS_STORAGE:
//     Configure sc_bigint and sc_biguint so they have storage for their values rather than
//     relying on sc_signed and sc_unsigned to provide it.
//
// SC_BIGINT_CONFIG_BASE_CLASS_HAS_STORAGE:
//     Configure sc_bigint and sc_biguint so that sc_signed and sc_unsigned provide the storage
//     for their values. This includes the small vector support to eliminate malloc and free
//     for smaller values. (See SC_SMALL_VEC_DIGITS below).

// #define SC_BIGINT_CONFIG_TEMPLATE_CLASS_HAS_NO_BASE_CLASS
// #define SC_BIGINT_CONFIG_TEMPLATE_CLASS_HAS_STORAGE
// #define SC_BIGINT_CONFIG_BASE_CLASS_HAS_STORAGE

#if !defined(SC_BIGINT_CONFIG_TEMPLATE_CLASS_HAS_NO_BASE_CLASS) && \
    !defined(SC_BIGINT_CONFIG_TEMPLATE_CLASS_HAS_STORAGE) && \
    !defined(SC_BIGINT_CONFIG_BASE_CLASS_HAS_STORAGE)

  // default
  #define SC_BIGINT_CONFIG_BASE_CLASS_HAS_STORAGE

#endif

// SC_FREE_DIGIT - this macro is present to allow SC_BIGINT_CONFIG_BASE_CLASS_HAS_STORAGE to
// dispense with having an m_free boolean value, since it is sufficient to check the value of
// 'digit' against the address of 'small_vec' to determine if digit should be freed. If we settle on
// SC_BIGINT_CONFIG_BASE_CLASS_HAS_STORAGE as the configuration to use in the long run, all the
// instances of SC_FREE_DIGIT may be removed.

#if defined(SC_BIGINT_CONFIG_BASE_CLASS_HAS_STORAGE)
#   define SC_FREE_DIGIT(FLAG)
#else
#   define SC_FREE_DIGIT(FLAG) { m_free = FLAG; }
#endif

// SC_SMALL_VEC_DIGITS - controls the size of the compile-time buffer contained in sc_signed and
// sc_unsigned values. This buffer is used in place of a malloc of storage for the object
// instance's value. The compile-time buffer's size is a trade-off between preventing malloc/free
// invocations for the storage, and the footprint of sc_signed and sc_unsigned instances.

#define SC_SMALL_VEC_DIGITS 40

// Sign of a number:
#define SC_NEG       -1     // Negative number
#define SC_ZERO      0      // Zero
#define SC_POS       1      // Positive number
#define SC_NOSIGN    2      // Uninitialized sc_signed number

typedef unsigned char uchar;

// A small_type number is at least a char. Defining an int is probably
// better for alignment.
typedef int small_type;

// Attributes of a byte.
#define BITS_PER_BYTE   8
#define BYTE_RADIX      256
#define BYTE_MASK       255

// LOG2_BITS_PER_BYTE = log2(BITS_PER_BYTE), assuming that
// BITS_PER_BYTE is a power of 2.
#define LOG2_BITS_PER_BYTE 3

// Attributes of the unsigned int. These definitions are used mainly in
// the functions that are aware of the internal representation of
// digits, e.g., get/set_packed_rep().
#define BYTES_PER_DIGIT_TYPE  4
#define BITS_PER_DIGIT_TYPE   32

// Support for "digit" vectors used to hold the values of sc_signed,
// sc_unsigned, sc_bv_base,  and sc_lv_base data types. This type is also used
// in the concatenation support. An sc_digit is currently an unsigned 32-bit
// quantity. The typedef used is an unsigned int, rather than an unsigned long,
// since the unsigned long data type varies in size between 32-bit and 64-bit
// machines.

#define BYTES_PER_DIGIT 4
// #define BITS_PER_DIGIT  30
#define BITS_PER_DIGIT  32

typedef unsigned int sc_digit;        // type holding "digits" in big values.

#define DIGIT_RADIX     ((sc_carry)1 << BITS_PER_DIGIT)
#define DIGIT_MASK      (DIGIT_RADIX - 1)

// If the number of valid bits in an sc_digit is the same as the number of
// bits in the sc_digit optimize for that fact.
//
//   SC_BIT_INDEX   - return the index of this bit within its sc_digit.
//   SC_DIGIT_INDEX - return the index within an sc_digit vector of the
//                    supplied bit index.
//   SC_MASK_DIGIT  - mask the supplied sc_digit, keeping the valid bits
//                    within an sc_digit.
//   SC_BIT_MASK    - this is the mask for digits below the supplied bit
//   SC_BIT_MASK1   - this is the mask for digits below, and including
//                    the supplied bit
//   SC_DIGIT_COUNT - this is the number of digits required to accommodate
//                    the supplied number of bits.

#if BITS_PER_DIGIT == 32
#   define SC_BIT_INDEX(BIT) ( ( (BIT)&std::numeric_limits<sc_digit>::digits-1) )
#   define SC_DIGIT_INDEX(BIT_INDEX) ((BIT_INDEX)>>5)
#   define SC_MASK_DIGIT(v) (v)
#   define SC_DIGIT_COUNT(BIT_WIDTH) ((BIT_WIDTH+BITS_PER_DIGIT-1)/BITS_PER_DIGIT) 
#else
#   define SC_BIT_INDEX(BIT) ((BIT)%BITS_PER_DIGIT)
#   define SC_DIGIT_INDEX(BIT) ((BIT)/BITS_PER_DIGIT)
#   define SC_MASK_DIGIT(v) ((v) & DIGIT_MASK)
#   define SC_DIGIT_COUNT(BIT) (SC_DIGIT_INDEX(BIT)+1)
#endif
#define SC_BIT_MASK(BITS)  ~( (std::numeric_limits<sc_digit>::max()  ) << SC_BIT_INDEX(BITS) )
#define SC_BIT_MASK1(BITS) ~( (std::numeric_limits<sc_digit>::max()-1) << SC_BIT_INDEX(BITS) )

// Make sure that BYTES_PER_DIGIT = ceil(BITS_PER_DIGIT / BITS_PER_BYTE).

// Similar attributes for the half of a digit. Note that
// HALF_DIGIT_RADIX is equal to the square root of DIGIT_RADIX. These
// definitions are used mainly in the multiplication routines.
#define BITS_PER_HALF_DIGIT (BITS_PER_DIGIT / 2)
#define HALF_DIGIT_RADIX    (1ul << BITS_PER_HALF_DIGIT)
#define HALF_DIGIT_MASK     (HALF_DIGIT_RADIX - 1)

// DIV_CEIL2(x, y) = ceil(x / y). x and y are positive numbers.
#define DIV_CEIL2(x, y) (((x) - 1) / (y) + 1)

// DIV_CEIL(x) = ceil(x / BITS_PER_DIGIT) = the number of digits to
// store x bits. x is a positive number.
#define DIV_CEIL(x) DIV_CEIL2(x, BITS_PER_DIGIT)

// Support for the long long type. This type is not in the standard
// but is usually supported by compilers.
#ifndef _WIN32
#   if defined(__x86_64__) || defined(__aarch64_)
        typedef long long          int64;
        typedef unsigned long long uint64;
#   else
        typedef int64_t            int64;
        typedef uint64_t           uint64;
#   endif
    extern const uint64        UINT64_ZERO;
    extern const uint64        UINT64_ONE;
    extern const uint64        UINT64_32ONES;
#else
    typedef __int64            int64;
    typedef unsigned __int64   uint64;
    extern const uint64        UINT64_ZERO;
    extern const uint64        UINT64_ONE;
    extern const uint64        UINT64_32ONES;
#endif
#if BITS_PER_DIGIT < 32
    typedef unsigned int sc_carry;    // type of carry temporaries.
#else
    typedef uint64       sc_carry;    // type of carry temporaries.
#endif

// Bits per ...
// will be deleted in the future. Use numeric_limits instead
#define BITS_PER_CHAR    8
#define BITS_PER_INT    (sizeof(int) * BITS_PER_CHAR)
#define BITS_PER_LONG   (sizeof(long) * BITS_PER_CHAR)
#define BITS_PER_INT64  (sizeof(::sc_dt::int64) * BITS_PER_CHAR)
#define BITS_PER_UINT   (sizeof(unsigned int) * BITS_PER_CHAR)
#define BITS_PER_ULONG  (sizeof(unsigned long) * BITS_PER_CHAR)
#define BITS_PER_UINT64 (sizeof(::sc_dt::uint64) * BITS_PER_CHAR)

// Digits per ...
#define DIGITS_PER_CHAR   1
#define DIGITS_PER_INT    ((BITS_PER_INT+(BITS_PER_DIGIT-1))/BITS_PER_DIGIT)
#define DIGITS_PER_LONG   ((BITS_PER_LONG+(BITS_PER_DIGIT-1))/BITS_PER_DIGIT)
#define DIGITS_PER_INT64  ((BITS_PER_INT64+(BITS_PER_DIGIT-1))/BITS_PER_DIGIT)
#define DIGITS_PER_UINT   ((BITS_PER_UINT+(BITS_PER_DIGIT-1))/BITS_PER_DIGIT)
#define DIGITS_PER_ULONG  ((BITS_PER_ULONG+(BITS_PER_DIGIT-1))/BITS_PER_DIGIT)
#define DIGITS_PER_UINT64 ((BITS_PER_UINT64+(BITS_PER_DIGIT-1))/BITS_PER_DIGIT)

// Above, BITS_PER_X is mainly used for sc_signed, and BITS_PER_UX is
// mainly used for sc_unsigned.

#if defined( WIN32 ) || defined( __SUNPRO_CC ) || defined( __HP_aCC )
typedef unsigned long fmtflags;
#else
typedef ::std::ios::fmtflags fmtflags;
#endif

extern const small_type NB_DEFAULT_BASE ;

// For sc_int code:
#define LLWIDTH  BITS_PER_INT64
#define INTWIDTH BITS_PER_INT

typedef int64 int_type;
typedef uint64 uint_type;
#define SC_INTWIDTH 64
extern const uint64 UINT_ZERO;
extern const uint64 UINT_ONE;

} // namespace sc_dt


#endif
