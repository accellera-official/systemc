
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
 
#pragma once

#if ( ! defined ( __TLM_PLATFORM_H__ ) )
#define __TLM_PLATFORM_H__

#if ( ! defined ( _MSC_VER ) )
#include <stdint.h>			/// standard scalar data types
#endif /* ! _MSC_VER */

// make sure we have the standard scalar data types

// 64-bit ints
#if ( ! defined ( _INT64_T ) )
#define _INT64_T
typedef long long   int64_t;
#endif /* ! _INT64_T */

#if ( ! defined ( _UINT64_T ) )
#define _UINT64_T
typedef unsigned long long   uint64_t;
#endif /* ! _UINT64_T */

#endif /* __TLM_PLATFORM_H__ */
