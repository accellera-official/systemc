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

  test01.cpp -- 

  Original Author: Martin Janssen, Synopsys, Inc., 2002-02-15

 *****************************************************************************/

/*****************************************************************************

  MODIFICATION LOG - modifiers, enter your name, affiliation, date and
  changes you are making here.

      Name, Affiliation, Date:
  Description of Modification:

 *****************************************************************************/

// test of sc_attribute - general test

#include "systemc.h"

int
sc_main( int, char*[] )
{
    sc_clock clk;

    sc_attribute<int> a1( "a1", 42 );
    sc_attribute<std::string> a2( "a2", "foobar" );

    clk.add_attribute( a1 );
    clk.add_attribute( a2 );

    sc_attr_base* p = clk.get_attribute( "a1" );
    cout << p->name() << endl;
    sc_attribute<int>* pi = dynamic_cast<sc_attribute<int>*>( p );
    if( pi != 0 ) {
        cout << pi->value << endl;
    }
    sc_attribute<std::string>* ps = dynamic_cast<sc_attribute<std::string>*>( p );
    if( ps != 0 ) {
        cout << ps->value << endl;
    }

    const sc_attr_cltn& attrs = clk.attr_cltn();
    sc_attr_cltn::const_iterator it = attrs.begin();
    for( ; it != attrs.end(); ++ it ) {
        cout << (*it)->name() << endl;
    }

    clk.remove_attribute( "a1" );
    clk.remove_attribute( "a2" );

    return 0;
}
