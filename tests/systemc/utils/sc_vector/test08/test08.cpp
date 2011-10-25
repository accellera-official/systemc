/*****************************************************************************

  The following code is derived, directly or indirectly, from the SystemC
  source code Copyright (c) 1996-2011 by all Contributors.
  All Rights reserved.

  The contents of this file are subject to the restrictions and limitations
  set forth in the SystemC Open Source License Version 3.0 (the "License");
  You may not use this file except in compliance with such restrictions and
  limitations. You may obtain instructions on how to receive a copy of the
  License at http://www.systemc.org/. Software distributed by Contributors
  under the License is distributed on an "AS IS" basis, WITHOUT WARRANTY OF
  ANY KIND, either express or implied. See the License for the specific
  language governing rights and limitations under the License.

 *****************************************************************************/

/*****************************************************************************

  test08.cpp -- sc_vector of objects not derived from sc_object
                (OSCI extension to IEEE 1666-2011)

  Original Author: Philipp A. Hartmann, OFFIS, 2011-10-01

 *****************************************************************************/

#include "systemc.h"

struct foo
{
  explicit
  foo( const char* nm )
    : name(nm)
  {}
  std::string name;
};

int sc_main( int, char*[] )
{
  sc_report_handler::set_actions( SC_ERROR, SC_DISPLAY );

  sc_vector< sc_event > ev_vec ( "evs", 1 );

  sc_assert( ev_vec.size() == 1 );
  // should print an error
  sc_assert( ev_vec.get_elements().size() == 0 );

  sc_vector< foo >      foo_vec( "foo", 1 );

  sc_assert( foo_vec.size() == 1 );
  sc_assert( sc_assemble_vector( foo_vec, &foo::name ).size() == 1 );
  // should print an error
  sc_assert( sc_assemble_vector( foo_vec, &foo::name )
               .get_elements().size() == 0 );

  cout << "\nSuccess" << endl;
  return 0;
}
