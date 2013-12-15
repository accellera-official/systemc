/*****************************************************************************

  The following code is derived, directly or indirectly, from the SystemC
  source code Copyright (c) 1996-2014 by all Contributors.
  All Rights reserved.

  The contents of this file are subject to the restrictions and limitations
  set forth in the SystemC Open Source License (the "License");
  You may not use this file except in compliance with such restrictions and
  limitations. You may obtain instructions on how to receive a copy of the
  License at http://www.accellera.org/. Software distributed by Contributors
  under the License is distributed on an "AS IS" basis, WITHOUT WARRANTY OF
  ANY KIND, either express or implied. See the License for the specific
  language governing rights and limitations under the License.

*****************************************************************************/

/*****************************************************************************

  main.cpp -- Test consistency check of SC_DEFAULT_WRITER_POLICY setting
              (see test-unchecked.cpp)

  Original Author: Philipp A. Hartmann, OFFIS, 2013-11-05

*****************************************************************************/

#ifndef SC_DEFAULT_WRITER_POLICY
# define SC_DEFAULT_WRITER_POLICY SC_MANY_WRITERS
#endif
#include <systemc>

int sc_main(int,char*[])
{
  SC_REPORT_INFO( "sc_api_version", "in sc_main" );
  return 0;
}
