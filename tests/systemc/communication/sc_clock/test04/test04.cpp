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

  test04.cpp -- 

  Original Author: Ucar Aziz, Synopsys, Inc., 2002-02-15
                   Martin Janssen, Synopsys, Inc., 2002-02-15

 *****************************************************************************/

/*****************************************************************************

  MODIFICATION LOG - modifiers, enter your name, affiliation, date and
  changes you are making here.

      Name, Affiliation, Date:
  Description of Modification:

 *****************************************************************************/

// test of sc_clock::time_stamp()

#include "systemc.h"


int
sc_main( int, char*[] )
{
    sc_time t1( 8, SC_NS );
    sc_time t2( 2, SC_NS );
    
    sc_clock c1( "c1", t1, 0.1, t2 );
    sc_start(t1);
    cout << "current time is: ";  
    cout << c1.time_stamp();
    cout << endl;    

    sc_start(t2);
    cout << "now, the current time is: ";  
    cout << c1.time_stamp();
    cout << endl;    

    return 0;
}
