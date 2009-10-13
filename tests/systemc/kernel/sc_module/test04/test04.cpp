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

  test04.cpp -- 

  Original Author: Martin Janssen, Synopsys, Inc., 2002-02-15

 *****************************************************************************/

/*****************************************************************************

  MODIFICATION LOG - modifiers, enter your name, affiliation, date and
  changes you are making here.

      Name, Affiliation, Date:
  Description of Modification:

 *****************************************************************************/

// test of the unique name generation for objects

#include "systemc.h"

SC_MODULE( mod_a )
{
    sc_in<int> in;
    sc_out<int> out;
    SC_CTOR( mod_a ) {}
};

SC_MODULE( mod_b )
{
    sc_out<int> out;
    sc_in<int>  in;
    SC_CTOR( mod_b ) {}
};

SC_MODULE( mod_c )
{
    mod_a a;
    mod_b b;
    sc_signal<int> sig1;
    sc_signal<int> sig2;
    SC_CTOR( mod_c ) : a("a"), b("b")
    {
        a.in( sig1 );
        a.out( sig2 );
        b.out( sig1 );
        b.in( sig2 );
    }
};

int
sc_main( int, char*[] )
{
    sc_signal<int> sig1;
    sc_signal<int> sig2;
    mod_a a("a");
    mod_b b("b");
    mod_c c("c");
    a.in( sig1 );
    a.out( sig2 );
    b.out( sig1 );
    b.in( sig2 );

    sc_start(0);

    cout << sig1.name() << endl;
    cout << sig2.name() << endl;
    cout << a.name() << endl;
    cout << b.name() << endl;
    cout << c.name() << endl;
    cout << a.in.name() << endl;
    cout << a.out.name() << endl;
    cout << b.out.name() << endl;
    cout << b.in.name() << endl;
    cout << c.a.name() << endl;
    cout << c.b.name() << endl;
    cout << c.sig1.name() << endl;
    cout << c.sig2.name() << endl;
    cout << c.a.in.name() << endl;
    cout << c.a.out.name() << endl;
    cout << c.b.out.name() << endl;
    cout << c.b.in.name() << endl;

    return 0;
}
