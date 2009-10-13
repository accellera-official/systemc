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

// test of module inheritance and sc_module_name

#include "systemc.h"

class base_mod
: public sc_module
{
public:
    base_mod( sc_module_name name_ )
        : sc_module( name_ )
        {}
};

class derived_mod
: public base_mod
{
public:
    derived_mod( sc_module_name name_ )
        : base_mod( name_ )
        {}
};

int
sc_main( int, char*[] )
{
    base_mod m1( "m1" );
    derived_mod m2( "m2" );

    sc_initialize();

    cout << m1.name() << endl;
    cout << m2.name() << endl;

    return 0;
}
