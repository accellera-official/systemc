/*****************************************************************************

  The following code is derived, directly or indirectly, from the SystemC
  source code Copyright (c) 1996-2013 by all Contributors.
  All Rights reserved.

  The contents of this file are subject to the restrictions and limitations
  set forth in the SystemC Open Source License Version 3.1 (the "License");
  You may not use this file except in compliance with such restrictions and
  limitations. You may obtain instructions on how to receive a copy of the
  License at http://www.systemc.org/. Software distributed by Contributors
  under the License is distributed on an "AS IS" basis, WITHOUT WARRANTY OF
  ANY KIND, either express or implied. See the License for the specific
  language governing rights and limitations under the License.

*****************************************************************************/

/*****************************************************************************

  test-unchecked.cpp -- force SC_DEFAULT_WRITER_POLICY to SC_MANY_WRITERS
                        (for consistency checking)

  Original Author: Philipp A. Hartmann, OFFIS, 2013-11-05

*****************************************************************************/

#ifdef SC_DEFAULT_WRITER_POLICY
# undef SC_DEFAULT_WRITER_POLICY
#endif
#define SC_DEFAULT_WRITER_POLICY SC_MANY_WRITERS
#include <systemc>
