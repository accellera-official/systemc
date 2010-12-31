/*****************************************************************************

  The following code is derived, directly or indirectly, from the SystemC
  source code Copyright (c) 1996-2010 by all Contributors.
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

  test06.cpp -- Test sc_vector::get_elements

  Original Author: Philipp A. Hartmann, OFFIS, 2010-11-04

 *****************************************************************************/

/*****************************************************************************

  MODIFICATION LOG - modifiers, enter your name, affiliation, date and
  changes you are making here.

      Name, Affiliation, Date:
  Description of Modification:

 *****************************************************************************/

#include "systemc"

#include "sysc/utils/sc_vector.h"

using sc_core::sc_object;
using sc_core::sc_vector;
using sc_core::sc_mutex;
using sc_core::sc_in;

void print_vector( const char * header,
                   const std::vector<sc_object*> & vec )
{
  std::cout << "\n-->-- " << header << " -->--\n";

  std::cout << " - size: " << vec.size() << "\n";

  for (size_t i=0; i<vec.size(); ++i )
    std::cout << " - "
              << vec[i]->name() << " - "
              << vec[i]->kind() << "\n";

  std::cout << "--<-- " << header << " --<--"
            << std::endl;
}

#define PRINT_VECTOR( Vector ) \
  print_vector( #Vector, Vector )

SC_MODULE( sub_module )
{
  sc_in<bool> in;
  SC_CTOR(sub_module) {}
};


SC_MODULE( module )
{
  // vector of sub-modules
  sc_vector< sub_module > sub_vec;

  // vector of ports
  sc_vector< sc_in<bool> > in_vec;

  SC_CTOR(module)
    : sub_vec( "sub_modules" )
    , in_vec( "in_vec" )
  {
    init(4);
  }

  void init( unsigned n_sub )
  {
    sub_vec.init(n_sub);
    in_vec.init(n_sub);
    // in_vec.init(n_sub); // second call fails

    // bind ports of sub-modules -- no dereference
    sc_assemble_vector(  sub_vec, &sub_module::in ).bind( in_vec );
  }

};

int sc_main(int, char* [])
{
  module m("dut");

  // m.sub_vec.init(3); // call from invalid context

  PRINT_VECTOR( m.get_child_objects() );

  PRINT_VECTOR( m.sub_vec.get_child_objects() );

  PRINT_VECTOR( m.sub_vec.get_elements() );

  PRINT_VECTOR( sc_assemble_vector( m.sub_vec, &sub_module::in ).get_elements() );

  std::cout << "\nProgram completed" << std::endl;
  return 0;
}
