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

  test13.cpp -- 

  Original Author: Ucar Aziz, Synopsys, Inc., 2002-02-15
                   Martin Janssen, Synopsys, Inc., 2002-02-15

 *****************************************************************************/

/*****************************************************************************

  MODIFICATION LOG - modifiers, enter your name, affiliation, date and
  changes you are making here.

      Name, Affiliation, Date:
  Description of Modification:

 *****************************************************************************/

// test of next_trigger() for static sensitivity

#include "systemc.h"

SC_MODULE( mod_a )
{
    sc_event e1;
    sc_event e2;
 
    void write( const char* msg )
    {
        cout <<"simulation time" << ":" << sc_time_stamp()
             << " " << msg << endl;
     
    }

    void sender()
    {
      write( "sender - e2" );
      e2.notify(10, SC_NS );
      next_trigger();
    }


    void receiver()
    {
        next_trigger();
	write( "receiver - e1" );
        e1.notify(SC_ZERO_TIME );
    }
 

    SC_CTOR( mod_a )
    {
        SC_METHOD( sender );
        sensitive << e1;
        SC_METHOD( receiver );
        sensitive << e2;
    }
};


int
sc_main( int, char*[] )
{
    mod_a a( "a" );
    cout<<"sender notifies e2 after 10 ns, receiver e1 at zero time" << endl;
    sc_start(50,SC_NS);

    return 0;
}
