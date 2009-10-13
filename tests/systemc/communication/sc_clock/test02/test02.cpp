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

  test02.cpp -- 

  Original Author: Ucar Aziz, Synopsys, Inc., 2002-02-15
                   Martin Janssen, Synopsys, Inc., 2002-02-15

 *****************************************************************************/

/*****************************************************************************

  MODIFICATION LOG - modifiers, enter your name, affiliation, date and
  changes you are making here.

      Name, Affiliation, Date:
  Description of Modification:

 *****************************************************************************/

// test of the sc_clock::print

#include "systemc.h"


int
sc_main( int, char*[] )
{
    sc_time t1( 8, SC_NS );
    sc_time t2( 2, SC_NS );


    sc_clock c1( "c1", t1, 0.1, t2 );
    cout <<  "m_cur_val for c1( \"c1\", t1, 0.1, t2 ) is: ";
    c1.print(cout);
    cout << "\n";

    sc_clock c2( "c2", t1, 0.1, t2, false );
    cout <<  "m_cur_val for c2( \"c2\", t1, 0.1, t2, false ) is: ";
    c2.print(cout);
    cout << "\n";


    sc_clock c3( "c3", 8, SC_NS, 0.1 );
    cout <<  "m_cur_val for c3( \"c3\", t1, 0.1, t2 ) is: ";
    c3.print(cout);
    cout << "\n";

    sc_clock c4( "c4", 8, SC_NS, 0.1, false );
    cout <<  "m_cur_val for c4( \"c4\", t1, 0.1, t2, false ) is: ";
    c4.print(cout);
    cout << "\n";

    sc_clock c5( "c5", 8, SC_NS, 0.1, 2, SC_NS );
    cout <<  "m_cur_val for c5( \"c5\", t1, 0.1, t2 ) is: ";
    c5.print(cout);
    cout<< "\n";

    sc_clock c6( "c6", 8, SC_NS, 0.1, 2, SC_NS, false );
    cout <<  "m_cur_val for c6( \"c6\", t1, 0.1, t2, false ) is: ";
    c6.print(cout);
    cout<< "\n";

    return 0;
}
