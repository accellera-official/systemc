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

  test03.cpp -- Test sc_vector

  Original Author: Philipp A. Hartmann, OFFIS, 2010-03-05

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
  // constructor with additional parameters
  sub_module( sc_core::sc_module_name, int param );
  // ...
};

SC_MODULE( module )
{
  sc_core::sc_vector< sub_module > m_sub_vec;          // vector of sub-modules

  module( sc_core::sc_module_name, unsigned n_sub );   // constructor

  struct mod_creator                                   // Creator struct
  {
    mod_creator( int p ) : param(p) {}                 // store parameter to forward

    sub_module* operator()( const char* name, size_t ) // actual creator function
    {
      return new sub_module( name, param );            // forward param to sub-module
    }
    int param;
  };

};

sub_module::sub_module( sc_core::sc_module_name, int )
  { /* empty */ }

module::module( sc_core::sc_module_name, unsigned n_sub )
  : m_sub_vec( "sub_modules" )                       // set name prefix
{
  m_sub_vec.init( n_sub, mod_creator(42) );          // init with custom creator
  // ...
}

int sc_main(int , char* [])
{
  module dut( "dut", 5 );

  typedef sc_core::sc_vector<sub_module> vec_type;
  vec_type const & children = dut.m_sub_vec;

  for( vec_type::const_iterator it = children.begin();
       it != children.end(); ++it )
  {
    cout << it->name() << " - "
         << (*it).kind()
         << endl;
  }

  for (size_t i=0; i < children.size(); ++i )
    cout << children[i].basename() << " - "
         << children[i].get_parent_object()->name()
         << endl;

  cout << "Program completed" << endl;
  return 0;
}
