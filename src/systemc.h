/*****************************************************************************

  The following code is derived, directly or indirectly, from the SystemC
  source code Copyright (c) 1996-2005 by all Contributors.
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

  systemc.h - Top-level include file for the SystemC library with usings.

  Original Author: Stan Y. Liao, Synopsys, Inc.

 *****************************************************************************/

/*****************************************************************************

  MODIFICATION LOG - modifiers, enter your name, affiliation, date and
  changes you are making here.

      Name, Affiliation, Date: Andy Goodrich, Forte Design Systems, 31 Mar 2005
  Description of Modification: Changes for namespace support.

 *****************************************************************************/

#ifndef SYSTEMC_H
#define SYSTEMC_H

#include "systemc"

// USINGS FOR THE sc_dt NAMESPACE:

using sc_dt::sc_bit;
using sc_dt::sc_logic;
using sc_dt::sc_bv_base;
using sc_dt::sc_bv;
using sc_dt::sc_lv_base;
using sc_dt::sc_lv;
using sc_dt::int64;
using sc_dt::uint64;

using sc_dt::sc_numrep;
using sc_dt::SC_NOBASE;
using sc_dt::SC_BIN;
using sc_dt::SC_OCT;
using sc_dt::SC_DEC;
using sc_dt::SC_HEX;
using sc_dt::SC_BIN_US;
using sc_dt::SC_BIN_SM;
using sc_dt::SC_OCT_US;
using sc_dt::SC_OCT_SM;
using sc_dt::SC_HEX_US;
using sc_dt::SC_HEX_SM;
using sc_dt::SC_CSD;
using sc_dt::sc_io_show_base;
using sc_dt::sc_string_old;

using sc_dt::SC_LOGIC_0;
using sc_dt::SC_LOGIC_1;
using sc_dt::SC_LOGIC_Z;
using sc_dt::SC_LOGIC_X;

using sc_dt::sc_length_param;
using sc_dt::sc_length_context;
using sc_dt::sc_signed;
using sc_dt::sc_bigint;
using sc_dt::sc_unsigned;
using sc_dt::sc_biguint;
using sc_dt::sc_int_base;
using sc_dt::sc_int;
using sc_dt::sc_uint_base;
using sc_dt::sc_uint;

// #ifdef SC_DT_DEPRECATED
using sc_dt::sc_logic_0;
using sc_dt::sc_logic_1;
using sc_dt::sc_logic_Z;
using sc_dt::sc_logic_X;
// #endif

#ifdef SC_INCLUDE_FX
    using sc_dt::sc_fxnum;
	using sc_dt::sc_fxnum_bitref;
    using sc_dt::sc_fxnum_fast;
    using sc_dt::sc_fix;
    using sc_dt::sc_fix_fast;
    using sc_dt::sc_ufix;
    using sc_dt::sc_ufix_fast;
    using sc_dt::sc_fixed;
    using sc_dt::sc_fixed_fast;
    using sc_dt::sc_ufixed;
    using sc_dt::sc_ufixed_fast;
    using sc_dt::sc_fxval;
    using sc_dt::sc_fxval_fast;
    using sc_dt::sc_fxcast_switch;
    using sc_dt::sc_fxcast_context;
    using sc_dt::sc_fxtype_params;
    using sc_dt::sc_fxtype_context;
    using sc_dt::sc_q_mode;
    using sc_dt::SC_RND;
    using sc_dt::SC_RND_ZERO;
    using sc_dt::SC_RND_MIN_INF;
    using sc_dt::SC_RND_INF;
    using sc_dt::SC_RND_CONV;
    using sc_dt::SC_TRN;
    using sc_dt::SC_TRN_ZERO;
    using sc_dt::sc_o_mode;
    using sc_dt::SC_SAT;
    using sc_dt::SC_SAT_ZERO;
    using sc_dt::SC_SAT_SYM;
    using sc_dt::SC_WRAP;
    using sc_dt::SC_WRAP_SM;
    using sc_dt::sc_switch;
    using sc_dt::SC_OFF;
    using sc_dt::SC_ON;
    using sc_dt::sc_fmt;
    using sc_dt::SC_F;
    using sc_dt::SC_E;
    using sc_dt::sc_context_begin;
    using sc_dt::SC_NOW;
    using sc_dt::SC_LATER;

#endif // SC_INCLUDE_FX

#if defined( _MSC_VER )
    using sc_dt::equal;
    using sc_dt::not_equal;
    using sc_dt::b_not;
    using sc_dt::b_and;
    using sc_dt::b_or;
    using sc_dt::b_xor;
    using sc_dt::lrotate;
    using sc_dt::rrotate;
    using sc_dt::reverse;
    using sc_dt::concat;
    using sc_dt::and_reduce;
    using sc_dt::or_reduce;
    using sc_dt::xor_reduce;
    using sc_dt::nand_reduce;
    using sc_dt::nor_reduce;
    using sc_dt::xnor_reduce;

#endif // defined( _MSC_VER )


// USINGS FOR sc_core:
//
// The explicit using for ::sc_core::wait is to remove an ambiguity with
// the constructor for the system's union wait on Unix and Linux. This
// causes problems with aCC, so users of aCC should explicitly select
// the SystemC wait functions using ::sc_core::wait(...). This is actually
// a good idea for SystemC programmers in general.

using namespace sc_core;
#if !defined( __HP_aCC )
    using ::sc_core::wait;
#endif // !defined( __HP_aCC )


// USINGS FOR I/O STREAM SUPPORT:

#if !defined( _MSC_VER ) && !defined( __BORLANDC__ )
#   if defined(__GNUC__) && (__GNUC__ == 2) // Maintain compatibility for 2.95.x
    	using std::strstream;
    	using std::strstreambuf;
    	using std::istrstream;
    	using std::ostrstream;
#   endif

    using std::ios;
    using std::streambuf;
    using std::streampos;
    using std::streamsize;
    using std::iostream;
    using std::istream;
    using std::ostream;
    using std::cin;
    using std::cout;
    using std::cerr;
    using std::endl;
    using std::flush;
    using std::dec;
    using std::hex;
    using std::oct;
#   if defined(linux)
        using std::noshowbase;
        using std::showbase;
#   endif // defined(linux)


    using std::fstream;
    using std::ifstream;
    using std::ofstream;

    using std::size_t;

    using std::memchr;
    using std::memcmp;
    using std::memcpy;
    using std::memmove;
    using std::memset;
    using std::strcat;
    using std::strncat;
    using std::strchr;
    using std::strrchr;
    using std::strcmp;
    using std::strncmp;
    using std::strcpy;
    using std::strncpy;
    using std::strcspn;
    using std::strspn;
    using std::strlen;
    using std::strpbrk;
    using std::strstr;
    using std::strtok;

#else
#if defined( _MSC_VER )
    // VC++6 with standard library

   using std::ios;
   using std::streambuf;
   using std::streampos;
   using std::streamsize;
   using std::iostream;
   using std::istream;
   using std::ostream;
   using std::strstream;
   using std::strstreambuf;
   using std::istrstream;
   using std::ostrstream;
   using std::cin;
   using std::cout;
   using std::cerr;
   using std::endl;
   using std::flush;
   using std::dec;
   using std::hex;
   using std::oct;

   using std::fstream;
   using std::ifstream;
   using std::ofstream;

#endif // MSC
#endif // Everyone else

#endif
