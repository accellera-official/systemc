/*****************************************************************************

  The following code is derived, directly or indirectly, from the SystemC
  source code Copyright (c) 1996-2002 by all Contributors.
  All Rights reserved.

  The contents of this file are subject to the restrictions and limitations
  set forth in the SystemC Open Source License Version 2.3 (the "License");
  You may not use this file except in compliance with such restrictions and
  limitations. You may obtain instructions on how to receive a copy of the
  License at http://www.systemc.org/. Software distributed by Contributors
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

// test of immediate event notification

#include "systemc.h"

SC_MODULE( mod_a )
{
    sc_event e_a;
    sc_event e_b;

    int n;

    void thread_a()
    {
        wait( SC_ZERO_TIME );
        n = 0;
        while( true ) {
            n ++;
            e_a.notify();
            wait( e_b );
        }
    }

    void thread_b()
    {
        while( true ) {
            wait( e_a );
            cout << simcontext()->delta_count() << " " << n << endl;
            if( n == 20 ) {
                sc_stop();
            }
            e_b.notify();
        }
    }

    SC_CTOR( mod_a )
    {
        SC_THREAD( thread_a );
        SC_THREAD( thread_b );
    }
};

int
sc_main( int, char*[] )
{
    mod_a a( "a" );

    sc_start();

    return 0;
}
