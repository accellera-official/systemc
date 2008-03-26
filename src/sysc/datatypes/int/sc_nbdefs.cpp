/*****************************************************************************

  The following code is derived, directly or indirectly, from the SystemC
  source code Copyright (c) 1996-2006 by all Contributors.
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

/*****************************************************************************

  sc_nbdefs.h -- Top level header file for arbitrary precision signed/unsigned
                 arithmetic. This file defines all the constants needed.

  Original Author: Ali Dasdan, Synopsys, Inc.
  
 *****************************************************************************/

/*****************************************************************************

  MODIFICATION LOG - modifiers, enter your name, affiliation, date and
  changes you are making here.

      Name, Affiliation, Date:
  Description of Modification:

 *****************************************************************************/


// $Log: sc_nbdefs.cpp,v $
// Revision 1.1.1.1  2006/12/15 20:31:36  acg
// SystemC 2.2
//
// Revision 1.3  2006/01/13 18:49:32  acg
// Added $Log command so that CVS check in comments are reproduced in the
// source.
//

#include "sysc/datatypes/int/sc_nbdefs.h"


namespace sc_dt
{

#ifdef SC_MAX_NBITS
const int MAX_NDIGITS      = DIV_CEIL(SC_MAX_NBITS) + 2;
// Consider a number with x bits another with y bits. The maximum
// number of bits happens when we multiply them. The result will have
// (x + y) bits. Assume that x + y <= SC_MAX_NBITS. Then, DIV_CEIL(x) +
// DIV_CEIL(y) <= DIV_CEIL(SC_MAX_NBITS) + 2. This is the reason for +2
// above. With this change, MAX_NDIGITS must be enough to hold the
// result of any operation.
#endif

// Support for the long long type. This type is not in the standard
// but is usually supported by compilers.
#ifndef WIN32
const uint64 UINT64_ZERO   = 0ULL;
const uint64 UINT64_ONE    = 1ULL;
const uint64 UINT64_32ONES = 0x00000000ffffffffULL;
#else
const uint64 UINT64_ZERO   = 0i64;
const uint64 UINT64_ONE    = 1i64;
const uint64 UINT64_32ONES = 0x00000000ffffffffi64;
#endif

const small_type NB_DEFAULT_BASE = SC_DEC;

#ifndef _32BIT_
const uint64 UINT_ZERO = UINT64_ZERO;
const uint64 UINT_ONE  = UINT64_ONE;
#else
const unsigned int UINT_ZERO = 0U;
const unsigned int UINT_ONE  = 1U;
#endif

} // namespace sc_dt
