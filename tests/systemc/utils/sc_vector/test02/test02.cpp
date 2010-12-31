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

  test02.cpp -- Test sc_vector

  Original Author: Philipp A. Hartmann, OFFIS, 2010-01-10

 *****************************************************************************/

/*****************************************************************************

  MODIFICATION LOG - modifiers, enter your name, affiliation, date and
  changes you are making here.

      Name, Affiliation, Date:
  Description of Modification:

 *****************************************************************************/

//#define USE_BOOST

#ifndef USE_BOOST
#  define SC_INCLUDE_DYNAMIC_PROCESSES
#endif

#include "systemc.h"

#include "sysc/utils/sc_vector.h"
using sc_core::sc_vector;

#ifdef USE_BOOST
#include <boost/bind.hpp>
#include <boost/bind/placeholders.hpp>
#endif

// ---- some classes
//
struct base : sc_object
{
  base( const char* n ) : sc_object(n) {}
};

struct derived_0 : base
{
  derived_0(const char* name) : base(name) {}
  const char* kind() const { return "derived_0"; }
};

struct derived_1 : public base
{
  derived_1(const char* name) : base(name) {}
  const char* kind() const { return "derived_1"; }
};

// plain function pointer
sc_object* fill_array( const char* n, size_t i )
{
  if( i%2 ) return new derived_1( n );
  return new derived_0( n );
}

SC_MODULE(DUT)
{
  sc_vector< base >            arr;
  sc_vector< sc_in<bool> >     inps;
  sc_vector< sc_signal<bool> > sigs;

  SC_CTOR(DUT);

  // member function as creator (use with sc_bind())
  sc_signal<bool>* init_sig_bind( const char* n, unsigned i )
  {
    sc_signal<bool>* sig = new sc_signal<bool>(n);
    inps[i]( *sig );
    return sig;
  }
};


DUT::DUT( sc_module_name )
  : arr("array")
  , inps("inps", 5)
  , sigs("sigs")
{
  arr.init( 3, fill_array );

  sigs.init( inps.size()
#if defined( SC_INCLUDE_DYNAMIC_PROCESSES )
	, sc_bind( &DUT::init_sig_bind, this, sc_unnamed::_1, sc_unnamed::_2 )
#elif defined( USE_BOOST )
	, boost::bind( &DUT::init_sig_bind, this, _1, _2 )
#endif
  );
}

int sc_main(int , char* [])
{
  DUT dut("dut");

  std::vector<sc_object*> children = dut.get_child_objects();

  for (size_t i=0; i<children.size(); ++i )
    cout << children[i]->name() << " - "
         << children[i]->kind()
         << endl;

  cout << "Program completed" << endl;
  return 0;
}
