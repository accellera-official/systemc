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

  test05.cpp -- 

  Original Author: Martin Janssen, Synopsys, Inc., 2002-02-15

 *****************************************************************************/

/*****************************************************************************

  MODIFICATION LOG - modifiers, enter your name, affiliation, date and
  changes you are making here.

      Name, Affiliation, Date:
  Description of Modification:

 *****************************************************************************/

// test of the child objects of a module and the simcontext

#include "systemc.h"

SC_MODULE( mod_a )
{
    sc_in_clk clk;
    sc_out<int> out;

    int a;

    void main_action()
    {
        out = ++ a;
    }

    SC_CTOR( mod_a )
    {
        SC_METHOD( main_action );
        sensitive << clk.pos();
        a = 0;
    }
};

SC_MODULE( mod_b )
{
    sc_in<int> in;

    void main_action()
    {
        while( true ) {
            wait();
            cout << in.read() << endl;
        }
    }

    SC_CTOR( mod_b )
    {
        SC_THREAD( main_action );
        sensitive << in;
    }
};

SC_MODULE( mod_c )
{
    sc_in_clk clk;

    void main_action()
    {
        while( true ) {
            cout << sc_time_stamp() << endl;
            wait();
        }
    }

    mod_a a;
    mod_b b;
    sc_signal<int> sig;

    SC_CTOR( mod_c )
    : a( "a" ), b( "b" )
    {
        SC_CTHREAD( main_action, clk.neg() );
        a.clk( clk );
        a.out( sig );
        b.in( sig );
    }
};

void
print_child_objects( const ::std::vector<sc_object*>& child_objects_ )
{
    int size = child_objects_.size();
    cout << "***\n";
    for( int i = 0; i < size; ++ i ) {
        sc_object* object = child_objects_[i];
        cout << object->kind() << "  " << object->name() << endl;
    }
}

int
sc_main( int, char*[] )
{
    mod_a a( "a" );
    mod_b b( "b" );
    sc_clock clk;
    sc_signal<int> sig;

    a.clk( clk );
    a.out( sig );
    b.in( sig );

    mod_c c( "c" );
    c.clk( clk );

    sc_start(1, SC_NS);

    print_child_objects( sc_get_top_level_objects() );
    print_child_objects( a.get_child_objects() );
    print_child_objects( b.get_child_objects() );
    print_child_objects( c.get_child_objects() );
    print_child_objects( c.a.get_child_objects() );
    print_child_objects( c.b.get_child_objects() );

    return 0;
}
