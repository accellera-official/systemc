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

  test03.cpp -- 

  Original Author: Martin Janssen, Synopsys, Inc., 2002-02-15

 *****************************************************************************/

/*****************************************************************************

  MODIFICATION LOG - modifiers, enter your name, affiliation, date and
  changes you are making here.

      Name, Affiliation, Date:
  Description of Modification:

 *****************************************************************************/

// test of immediate notification check in the update phase

#include "systemc.h"

class my_signal
: public sc_signal<int>
{
public:

    my_signal()
        : sc_signal<int>()
        {}

protected:

    virtual void update()
        {
            if( m_new_val != m_cur_val ) {
                m_cur_val = m_new_val;
                ((sc_event&)value_changed_event()).notify(); // immediate notification!?
            }
        }
};

int
sc_main( int, char*[] )
{
    my_signal sig;

    sig.write( 1 );

    sc_start();

    return 0;
}
