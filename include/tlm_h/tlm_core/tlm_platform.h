/*****************************************************************************

  The following code is derived, directly or indirectly, from the SystemC
  source code Copyright (c) 1996-2004 by all Contributors.
  All Rights reserved.

  The contents of this file are subject to the restrictions and limitations
  set forth in the SystemC Open Source License Version 2.4 (the "License");
  You may not use this file except in compliance with such restrictions and
  limitations. You may obtain instructions on how to receive a copy of the
  License at http://www.systemc.org/. Software distributed by Contributors
  under the License is distributed on an "AS IS" basis, WITHOUT WARRANTY OF
  ANY KIND, either express or implied. See the License for the specific
  language governing rights and limitations under the License.

 *****************************************************************************/
 
#ifndef __TLM_PLATFORM_H__
#define __TLM_PLATFORM_H__

/// standard scalar data types

#if !defined(_MSC_VER)

// Solaris seems to pick up the types from /usr/include/sys/int_types.h
#if !defined(__sun)
#include <stdint.h>
#endif

#else


// 8-bit ints
#if ( ! defined ( _INT8_T ) )
#define _INT8_T
typedef char   int8_t;
#endif /* ! _INT8_T */

#if ( ! defined ( _UINT8_T ) )
#define _UINT8_T
typedef unsigned char   uint8_t;
#endif /* ! _UINT8_T */

// 16-bit ints
#if ( ! defined ( _INT16_T ) )
#define _INT16_T
typedef short   int16_t;
#endif /* ! _INT16_T */

#if ( ! defined ( _UINT16_T ) )
#define _UINT16_T
typedef unsigned short   uint16_t;
#endif /* ! _UINT16_T */

// 32-bit ints
#if ( ! defined ( _INT32_T ) )
#define _INT32_T
typedef long   int32_t;
#endif /* ! _INT32_T */

#if ( ! defined ( _UINT32_T ) )
#define _UINT32_T
typedef unsigned long   uint32_t;
#endif /* ! _UINT32_T */

// 64-bit ints
#if ( ! defined ( _INT64_T ) )
#define _INT64_T
typedef long long   int64_t;
#endif /* ! _INT64_T */

#if ( ! defined ( _UINT64_T ) )
#define _UINT64_T
typedef unsigned long long   uint64_t;
#endif /* ! _UINT64_T */

#endif

#endif /* __TLM_PLATFORM_H__ */
