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

  main.cpp --

  Original Author: Martin Janssen, Synopsys, Inc., 2002-02-15

 *****************************************************************************/

/*****************************************************************************

  MODIFICATION LOG - modifiers, enter your name, affiliation, date and
  changes you are making here.

      Name, Affiliation, Date:
  Description of Modification:

 *****************************************************************************/

// This may look like C code, but it is really -*- C++ -*-
//
// main.cxx --
// Copyright Synopsys 1998
// Author          : Ric Hilderink
// Created On      : Wed Dec 30 12:41:10 1998
// Status          : none
//

#include <limits.h>

#define SC_INCLUDE_FX
#include "systemc.h"
# if (defined(__GNUC__) && (__GNUC__ >= 3))|| (defined(_MSC_VER) && (_MSC_VER >= 1300))
#   include "sstream"
#else
#   include "strstream.h"
#endif
#include "fx_precision_double.h"

extern void test_fx_float_limits(ostream&);
extern void test_fx_ufix_limits(ostream&);
extern void test_fx_fix_limits(ostream&);
extern void test_fx_fixed_limits(ostream&);
extern void test_fx_ufixed_limits(ostream&);

int sc_main( int, char** )
{
# if (defined(__GNUC__) && (__GNUC__ >= 3))|| (defined(_MSC_VER) && (_MSC_VER >= 1300))
    std::stringstream out;
# else
    strstream out;
# endif

  out.precision(15);

  test_fx_float_limits(out);
  test_fx_ufix_limits(out);
  test_fx_fix_limits(out);
  test_fx_fixed_limits(out);
  test_fx_ufixed_limits(out);

# if (defined(__GNUC__) && (__GNUC__ >= 3))|| (defined(_MSC_VER) && (_MSC_VER >= 1300))
    std::string s = out.str();
    for (int i = 0; i < s.length(); i++ )
# else
    out << '\0';
    char* s = out.str();
    for (int i = 0; i < strlen(s); i++ )
# endif

  {
    if (s[i+0] == 'i'  &&  s[i+1] == 'n'  &&  s[i+2] == 'f')
    {
      s[i+0] = 'I';
      i += 2;
      continue;
    }

    if (s[i+0] == 'n'  &&  s[i+1] == 'a'  &&  s[i+2] == 'n')
    {
      s[i+0] = 'N';
      s[i+2] = 'N';
      i += 2;
      continue;
    }
  }

  cout << s;

  return 0;
}
