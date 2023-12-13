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
#include <cstdint>


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
//     for smaller values. (See SC_BASE_VEC_DIGITS below).

// #define SC_BIGINT_CONFIG_TEMPLATE_CLASS_HAS_NO_BASE_CLASS
#define SC_BIGINT_CONFIG_TEMPLATE_CLASS_HAS_STORAGE
// #define SC_BIGINT_CONFIG_BASE_CLASS_HAS_STORAGE

#if !defined(SC_BIGINT_CONFIG_TEMPLATE_CLASS_HAS_NO_BASE_CLASS) && \
    !defined(SC_BIGINT_CONFIG_TEMPLATE_CLASS_HAS_STORAGE) && \
    !defined(SC_BIGINT_CONFIG_BASE_CLASS_HAS_STORAGE)
#error no BIGINT_CONFIG specified!
#endif

// SC_FREE_DIGIT - this macro is present to allow SC_BIGINT_CONFIG_BASE_CLASS_HAS_STORAGE to
// dispense with having an m_free boolean value, since it is sufficient to check the value of
// 'digit' against the address of 'base_vec' to determine if digit should be freed. If we settle on
// SC_BIGINT_CONFIG_BASE_CLASS_HAS_STORAGE as the configuration to use in the long run, all the
// instances of SC_FREE_DIGIT may be removed.

#if defined(SC_BIGINT_CONFIG_BASE_CLASS_HAS_STORAGE)
#   define SC_FREE_DIGIT(FLAG)
#else
#   define SC_FREE_DIGIT(FLAG) { m_free = FLAG; }
#endif

// SC_BASE_VEC_DIGITS - controls the size of the compile-time buffer contained in sc_signed and
// sc_unsigned values. This buffer is used in place of a malloc of storage for the object
// instance's value. The compile-time buffer's size is a trade-off between preventing malloc/free
// invocations for the storage, and the footprint of sc_signed and sc_unsigned instances.

#define SC_BASE_VEC_DIGITS 8

#if !defined(SC_BASE_VEC_DIGITS)
#error no SC_BASE_VEC_DIGITS specified!
#endif

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

typedef unsigned int sc_digit;        // type holding "digits" in big values.

#define BYTES_PER_DIGIT (std::numeric_limits<sc_digit>::digits/8)
#define BITS_PER_DIGIT  32


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
#   define SC_BIT_INDEX(BIT) ( (BIT)&(std::numeric_limits<sc_digit>::digits-1) )
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


using int64  = long long;
using uint64 = unsigned long long;

constexpr uint64 UINT64_ZERO   = 0ULL;
constexpr uint64 UINT64_ONE    = 1ULL;
constexpr uint64 UINT64_32ONES = 0x00000000ffffffffULL;

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

using fmtflags = ::std::ios::fmtflags;

constexpr small_type NB_DEFAULT_BASE = SC_DEC;

// For sc_int code:
#define LLWIDTH  BITS_PER_INT64
#define INTWIDTH BITS_PER_INT

typedef int64 int_type;
typedef uint64 uint_type;
#define SC_INTWIDTH 64
constexpr uint64 UINT_ZERO = UINT64_ZERO;
constexpr uint64 UINT_ONE = UINT64_ONE;

// Table providing the number of bits on in a byte, used in sc_vector_utils.h.
constexpr int byte_one_bits[256] = {
    0, // 0x0
    1, // 0x1
    1, // 0x2
    2, // 0x3
    1, // 0x4
    2, // 0x5
    2, // 0x6
    3, // 0x7
    1, // 0x8
    2, // 0x9
    2, // 0xa
    3, // 0xb
    2, // 0xc
    3, // 0xd
    3, // 0xe
    4, // 0xf
    1, // 0x10
    2, // 0x11
    2, // 0x12
    3, // 0x13
    2, // 0x14
    3, // 0x15
    3, // 0x16
    4, // 0x17
    2, // 0x18
    3, // 0x19
    3, // 0x1a
    4, // 0x1b
    3, // 0x1c
    4, // 0x1d
    4, // 0x1e
    5, // 0x1f
    1, // 0x20
    2, // 0x21
    2, // 0x22
    3, // 0x23
    2, // 0x24
    3, // 0x25
    3, // 0x26
    4, // 0x27
    2, // 0x28
    3, // 0x29
    3, // 0x2a
    4, // 0x2b
    3, // 0x2c
    4, // 0x2d
    4, // 0x2e
    5, // 0x2f
    2, // 0x30
    3, // 0x31
    3, // 0x32
    4, // 0x33
    3, // 0x34
    4, // 0x35
    4, // 0x36
    5, // 0x37
    3, // 0x38
    4, // 0x39
    4, // 0x3a
    5, // 0x3b
    4, // 0x3c
    5, // 0x3d
    5, // 0x3e
    6, // 0x3f
    1, // 0x40
    2, // 0x41
    2, // 0x42
    3, // 0x43
    2, // 0x44
    3, // 0x45
    3, // 0x46
    4, // 0x47
    2, // 0x48
    3, // 0x49
    3, // 0x4a
    4, // 0x4b
    3, // 0x4c
    4, // 0x4d
    4, // 0x4e
    5, // 0x4f
    2, // 0x50
    3, // 0x51
    3, // 0x52
    4, // 0x53
    3, // 0x54
    4, // 0x55
    4, // 0x56
    5, // 0x57
    3, // 0x58
    4, // 0x59
    4, // 0x5a
    5, // 0x5b
    4, // 0x5c
    5, // 0x5d
    5, // 0x5e
    6, // 0x5f
    2, // 0x60
    3, // 0x61
    3, // 0x62
    4, // 0x63
    3, // 0x64
    4, // 0x65
    4, // 0x66
    5, // 0x67
    3, // 0x68
    4, // 0x69
    4, // 0x6a
    5, // 0x6b
    4, // 0x6c
    5, // 0x6d
    5, // 0x6e
    6, // 0x6f
    3, // 0x70
    4, // 0x71
    4, // 0x72
    5, // 0x73
    4, // 0x74
    5, // 0x75
    5, // 0x76
    6, // 0x77
    4, // 0x78
    5, // 0x79
    5, // 0x7a
    6, // 0x7b
    5, // 0x7c
    6, // 0x7d
    6, // 0x7e
    7, // 0x7f
    1, // 0x80
    2, // 0x81
    2, // 0x82
    3, // 0x83
    2, // 0x84
    3, // 0x85
    3, // 0x86
    4, // 0x87
    2, // 0x88
    3, // 0x89
    3, // 0x8a
    4, // 0x8b
    3, // 0x8c
    4, // 0x8d
    4, // 0x8e
    5, // 0x8f
    2, // 0x90
    3, // 0x91
    3, // 0x92
    4, // 0x93
    3, // 0x94
    4, // 0x95
    4, // 0x96
    5, // 0x97
    3, // 0x98
    4, // 0x99
    4, // 0x9a
    5, // 0x9b
    4, // 0x9c
    5, // 0x9d
    5, // 0x9e
    6, // 0x9f
    2, // 0xa0
    3, // 0xa1
    3, // 0xa2
    4, // 0xa3
    3, // 0xa4
    4, // 0xa5
    4, // 0xa6
    5, // 0xa7
    3, // 0xa8
    4, // 0xa9
    4, // 0xaa
    5, // 0xab
    4, // 0xac
    5, // 0xad
    5, // 0xae
    6, // 0xaf
    3, // 0xb0
    4, // 0xb1
    4, // 0xb2
    5, // 0xb3
    4, // 0xb4
    5, // 0xb5
    5, // 0xb6
    6, // 0xb7
    4, // 0xb8
    5, // 0xb9
    5, // 0xba
    6, // 0xbb
    5, // 0xbc
    6, // 0xbd
    6, // 0xbe
    7, // 0xbf
    2, // 0xc0
    3, // 0xc1
    3, // 0xc2
    4, // 0xc3
    3, // 0xc4
    4, // 0xc5
    4, // 0xc6
    5, // 0xc7
    3, // 0xc8
    4, // 0xc9
    4, // 0xca
    5, // 0xcb
    4, // 0xcc
    5, // 0xcd
    5, // 0xce
    6, // 0xcf
    3, // 0xd0
    4, // 0xd1
    4, // 0xd2
    5, // 0xd3
    4, // 0xd4
    5, // 0xd5
    5, // 0xd6
    6, // 0xd7
    4, // 0xd8
    5, // 0xd9
    5, // 0xda
    6, // 0xdb
    5, // 0xdc
    6, // 0xdd
    6, // 0xde
    7, // 0xdf
    3, // 0xe0
    4, // 0xe1
    4, // 0xe2
    5, // 0xe3
    4, // 0xe4
    5, // 0xe5
    5, // 0xe6
    6, // 0xe7
    4, // 0xe8
    5, // 0xe9
    5, // 0xea
    6, // 0xeb
    5, // 0xec
    6, // 0xed
    6, // 0xee
    7, // 0xef
    4, // 0xf0
    5, // 0xf1
    5, // 0xf2
    6, // 0xf3
    5, // 0xf4
    6, // 0xf5
    6, // 0xf6
    7, // 0xf7
    5, // 0xf8
    6, // 0xf9
    6, // 0xfa
    7, // 0xfb
    6, // 0xfc
    7, // 0xfd
    7, // 0xfe
    8, // 0xff
};

} // namespace sc_dt


#endif
