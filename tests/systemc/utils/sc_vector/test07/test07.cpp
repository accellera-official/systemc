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

  test01.cpp -- Test sc_vector -- empty bindings

  Original Author: Philipp A. Hartmann, OFFIS, 2011-02-14

 *****************************************************************************/

/*****************************************************************************

  MODIFICATION LOG - modifiers, enter your name, affiliation, date and
  changes you are making here.

      Name, Affiliation, Date:
  Description of Modification:

 *****************************************************************************/

#include "systemc.h"

SC_MODULE( sub_module )
{
  sc_in<bool> in;
  SC_CTOR(sub_module) {}
};

SC_MODULE( module )
{
  // vector of sub-modules
  sc_vector< sub_module >  m_sub_vec;

  // vector of ports
  sc_vector< sc_in<bool> > in_vec;

  module( sc_core::sc_module_name, unsigned n_sub )
    : m_sub_vec( "sub_modules" )
    , in_vec( "in_vec" )
  {
    // bind ports of submodules (before initialisation of module vector)
    do_bind();

    // initialise module vector
    m_sub_vec.init( n_sub );

    // bind ports of submodules (before initialisation of port vector)
    do_bind();

    // delayed initialisation of port vector
    in_vec.init( n_sub );

    // bind ports of submodules (should be fine now)
    do_bind();
  }

  void do_bind()
  {
    try {
      // bind ports of sub-modules -- sc_assemble_vector
      sc_assemble_vector( m_sub_vec, &sub_module::in ).bind( in_vec );
    } catch( sc_report const & rpt ) {
      std::cout << rpt.what() << std::endl;
    }
  }
};

int sc_main(int , char* [])
{
  module m("dut", 4);
  sc_vector< sc_signal<bool> > s("sig");

  // bind ports to signals -- before initialisation of signal vector
  m.in_vec( s );

  s.init(4);

  // bind empty range
  m.in_vec( s.begin(), s.begin() );

  // bind with full range
  m.in_vec( s );

  sc_start( SC_ZERO_TIME );

  cout << "Success" << endl;
  return 0;
}
