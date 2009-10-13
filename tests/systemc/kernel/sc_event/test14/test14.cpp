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

  test14.cpp -- 

  Original Author: Ucar Aziz, Synopsys, Inc., 2002-02-15
                   Martin Janssen, Synopsys, Inc., 2002-02-15

 *****************************************************************************/

/*****************************************************************************

  MODIFICATION LOG - modifiers, enter your name, affiliation, date and
  changes you are making here.

      Name, Affiliation, Date:
  Description of Modification:

 *****************************************************************************/

// test of wait(..) for dynamic sensitivity

#include "systemc.h"

SC_MODULE( mod_a )
{
    sc_event e1;
    sc_event e2;
    sc_event e3; 
    sc_event e4;
    sc_event e5;
    sc_event e6;

    void write( const char* msg )
    {
        cout <<"simulation time" << ":" << sc_simulation_time()
             << " " << msg << endl;
     
    }

    void sender1()
    {
      while(true){
       write( "sender_1    -e1 -e2" );
       e1.notify(10, SC_NS);
       e2.notify(20, SC_NS);
       wait(15, SC_NS, e4 | e6);
      }
    }

    void sender2()
    {
      while(true){
       write( "sender_2    -e3 -e4" );
       e3.notify(10, SC_NS);
       e4.notify(15, SC_NS);
       wait(20, SC_NS, e2 & e5);
      }
    }

    void receiver1()
    {
      while(true){
	wait(e1 & e3);
	write( "receiver_1  -e5" );
        e5.notify(10,SC_NS );
      }
    }
 
    void receiver2()
    {
      while(true){
	wait(e2 | e4);
	write( "receiver_2  -e6" );
        e6.notify(10,SC_NS );
      }
    }

    SC_CTOR( mod_a )
    {
        SC_THREAD(sender1);
        SC_THREAD(sender2);
        SC_THREAD(receiver1);
        SC_THREAD(receiver2);
    }
};


int
sc_main( int, char*[] )
{
    mod_a a( "a" );
    cout<<endl;
    cout<<"sender_1 notifies e1 after 10 ns, e2 after 20 ns\n"; 
    cout<<"sender_2 notifies e3 after 10 ns, e4 after 15 ns\n"; 
    cout<<"receiver_1 notifies e5 after 10 ns\n";
    cout<<"receiver_2 notifies e6 after 10 ns\n";
    cout << endl;
    sc_start(100,SC_NS);

    return 0;
}
