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

// test of ports binding in hierarchical model

#include "systemc.h"

SC_MODULE( mod_a )
{

  sc_in<int> in;  
  sc_out<int> out;

  SC_CTOR( mod_a )
  { }
};

SC_MODULE( mod_b )
{

  sc_in<int> in;  
  sc_out<int> out;

  SC_CTOR( mod_b )
  { }
};
 
// parent model
SC_MODULE( mod_c )
{

  sc_in<int> input;  
  sc_out<int> output;
  sc_signal<int> buf;
  mod_a module_a;
  mod_b module_b;
  
  SC_CTOR( mod_c ):
    module_a("module_a"),
    module_b("module_b")
  {

    module_a.in(input);
    module_a.out(buf);
    module_b.in(buf);
    module_b.out(output);

   }
};


int
sc_main( int, char*[] )
{
  mod_c c("c");
  cout << "binding of models to parent model is completed\n";
  return 0;
}
