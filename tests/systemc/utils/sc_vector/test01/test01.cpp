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

  test01.cpp -- Test sc_vector

  Original Author: Philipp A. Hartmann, OFFIS, 2010-01-10

 *****************************************************************************/

/*****************************************************************************

  MODIFICATION LOG - modifiers, enter your name, affiliation, date and
  changes you are making here.

      Name, Affiliation, Date:
  Description of Modification:

 *****************************************************************************/

#include "systemc.h"

#include "sysc/utils/sc_vector.h"
using sc_core::sc_vector;

SC_MODULE( sub_module )
{
  sc_in<bool> in;
  SC_CTOR(sub_module) {}
};

SC_MODULE( module )
{
  // vector of sub-modules
  sc_vector< sub_module > m_sub_vec;

  // vector of ports
  sc_vector< sc_in<bool> > in_vec;

  module( sc_core::sc_module_name, unsigned n_sub )
    : m_sub_vec( "sub_modules", n_sub ) // set name prefix, and create sub-modules
    // , in_vec()                       // use default constructor
    // , in_vec( "in_vec" )             // set name prefix
  {
    // delayed initialisation of port vector
    // here with default prefix sc_core::sc_gen_unique_name("vector")
    in_vec.init( n_sub );

    // bind ports of sub-modules -- sc_assemble_vector
    sc_assemble_vector( m_sub_vec, &sub_module::in ).bind( in_vec );
  }
};

int sc_main(int , char* [])
{
  module m("dut", 4);

  std::vector<sc_object*> children = m.get_child_objects();

  for (size_t i=0; i<children.size(); ++i )
    cout << children[i]->name() << " - "
         << children[i]->kind()
         << endl;

  cout << "Program completed" << endl;
  return 0;
}
