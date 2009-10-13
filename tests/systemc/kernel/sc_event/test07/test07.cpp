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

  test07.cpp -- 

  Original Author: Martin Janssen, Synopsys, Inc., 2002-02-15

 *****************************************************************************/

/*****************************************************************************

  MODIFICATION LOG - modifiers, enter your name, affiliation, date and
  changes you are making here.

      Name, Affiliation, Date:
  Description of Modification:

 *****************************************************************************/

// test of sc_event's methods; with static method receiver

#include "systemc.h"

SC_MODULE( mod_a )
{
    sc_event e;

    void write( const char* msg )
    {
        cout << sc_delta_count() << ":" << sc_time_stamp()
             << " " << msg << "\n";
    }

    void sender()
    {
        // wait one delta cycle
        wait( SC_ZERO_TIME );

        while( true ) {

            // test cancel()
            cout << "*** cancel()\n";

            // immediate notification
            e.notify();
            write( "sender - immediate" );
            wait( SC_ZERO_TIME );

            // immediate notification -- canceled (no effect)
            e.notify();
            write( "sender - immediate" );
            e.cancel();
            write( "sender - canceled" );
            wait( SC_ZERO_TIME );

            // delta notification
            e.notify( SC_ZERO_TIME );
            write( "sender - delta" );
            wait( SC_ZERO_TIME );
            wait( SC_ZERO_TIME );

            // delta notification -- canceled
            e.notify( SC_ZERO_TIME );
            write( "sender - delta" );
            e.cancel();
            write( "sender - canceled" );
            wait( SC_ZERO_TIME );
            wait( SC_ZERO_TIME );

            // timed notification
            e.notify( 1, SC_NS );
            write( "sender - timed 1 ns" );
            wait( 1, SC_NS );
            wait( SC_ZERO_TIME );

            // timed notification -- canceled
            e.notify( 1, SC_NS );
            write( "sender - timed 1 ns" );
            e.cancel();
            write( "sender - canceled" );
            wait( 1, SC_NS );
            wait( SC_ZERO_TIME );

            // timed notifiation -- canceled
            e.notify( 2, SC_NS );
            write( "sender - timed 2 ns" );
            wait( 1, SC_NS );
            e.cancel();
            write( "sender - canceled" );
            wait( 1, SC_NS );
            wait( SC_ZERO_TIME );

            // test notify() -- the exception test is in test03.cpp
            cout << "*** notify()\n";

            // delta notification -- made immediate
            e.notify( SC_ZERO_TIME );
            write( "sender - delta" );
            e.notify();
            write( "sender - immediate" );
            wait( SC_ZERO_TIME );
            wait( SC_ZERO_TIME );

            // timed notification -- made immediate
            e.notify( 1, SC_NS );
            write( "sender - timed 1 ns" );
            e.notify();
            write( "sender - immediate" );
            wait( 1, SC_NS );
            wait( SC_ZERO_TIME );

            // timed notification -- made immediate
            e.notify( 2, SC_NS );
            write( "sender - timed 2 ns" );
            wait( 1, SC_NS );
            e.notify();
            write( "sender - immediate" );
            wait( 1, SC_NS );
            wait( SC_ZERO_TIME );

            // test notify(t)
            cout << "*** notify(t)\n";

            e.notify( SC_ZERO_TIME );
            write( "sender - delta" );
            e.notify( 1, SC_NS );
            write( "sender - timed 1 ns" );
            wait( 1, SC_NS );
            wait( SC_ZERO_TIME );

            e.notify( 1, SC_NS );
            write( "sender - timed 1 ns" );
            e.notify( SC_ZERO_TIME );
            write( "sender - delta" );
            wait( 1, SC_NS );
            wait( SC_ZERO_TIME );

            e.notify( 2, SC_NS );
            write( "sender - timed 2 ns" );
            e.notify( 1, SC_NS );
            write( "sender - timed 1 ns" );
            wait( 2, SC_NS );
            wait( SC_ZERO_TIME );

            e.notify( 1, SC_NS );
            write( "sender - timed 1 ns" );
            e.notify( 2, SC_NS );
            write( "sender - timed 2 ns" );
            wait( 2, SC_NS );
            wait( SC_ZERO_TIME );

            sc_stop();
            write( "sender - stop" );
            wait( SC_ZERO_TIME );
        }
    }

    bool receiver_first;

    void receiver()
    {
        if( receiver_first ) {
            receiver_first = false;
            return;
        }
        write( "receiver" );
    }

    SC_CTOR( mod_a )
    {
        SC_THREAD( sender );
        SC_METHOD( receiver );
        sensitive << e;
        receiver_first = true;
    }
};

int
sc_main( int, char*[] )
{
    mod_a a( "a" );

    sc_start();

    return 0;
}
