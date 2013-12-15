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

  test.h -- 

  Original Author: Martin Janssen, Synopsys, Inc., 2002-02-15

 *****************************************************************************/

/*****************************************************************************

  MODIFICATION LOG - modifiers, enter your name, affiliation, date and
  changes you are making here.

      Name, Affiliation, Date:
  Description of Modification:

 *****************************************************************************/

// sc_signal test;
// interface methods -- register_port()

#ifndef TEST_H
#define TEST_H

#include "systemc.h"

template <class T>
class writer
: public sc_module
{
public:

    // output port
    sc_out<T> out;

    // constructor
    writer( sc_module_name )
        {}
};

template <class T>
class top
: public sc_module
{
public:

    // channel
    sc_signal<T> sig;

    // modules
    writer<T> w1;
    writer<T> w2;

    // constructor
    top( sc_module_name )
    : sig(), w1( "w1" ), w2( "w2" )
    {
        w1.out( sig );
        w2.out( sig );
    }
};

#endif

// Taf!
