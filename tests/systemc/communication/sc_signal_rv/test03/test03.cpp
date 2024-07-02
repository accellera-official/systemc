/*****************************************************************************

  Licensed to Accellera Systems Initiative Inc. (Accellera) under one or
  more contributor license agreements.  See the NOTICE file distributed
  with this work for additional information regarding copyright ownership.
  Accellera licenses this file to you under the Apache License, Version 2.0
  (the "License"); you may not use this file except in compliance with the
  License.  You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

  Unless required by applicable law or agreed to in writing, software
  distributed under the License is distributed on an "AS IS" BASIS,
  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or
  implied.  See the License for the specific language governing
  permissions and limitations under the License.

 *****************************************************************************/

/*****************************************************************************

  test03.cpp -- Test binding error processing for signal_rv ports.

  Original Author: Andy Goodrich, Accellera, 2024-07-01

 *****************************************************************************/

/*****************************************************************************

  MODIFICATION LOG - modifiers, enter your name, affiliation, date and
  changes you are making here.

      Name, Affiliation, Date:
  Description of Modification:

 *****************************************************************************/

// test of sc_signal_rv port classes.

#include "systemc.h"

SC_MODULE( mod_a )
{
    // ports
    sc_out_rv<1> out1;
    sc_in_rv<1>  in;

    SC_CTOR( mod_a )
    {
    }
};

int
sc_main( int, char*[] )
{
    sc_signal<sc_lv<1> > sig_lv;

    sc_report_handler::set_actions( SC_ERROR, SC_DISPLAY );

    mod_a a( "a" );

    a.out1( sig_lv );
    a.in( sig_lv );

    sc_start();

    return 0;
}
