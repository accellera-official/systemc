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

  arraytypes.h -- 

  Original Author: Martin Janssen, Synopsys, Inc., 2002-02-15

 *****************************************************************************/

/*****************************************************************************

  MODIFICATION LOG - modifiers, enter your name, affiliation, date and
  changes you are making here.

      Name, Affiliation, Date:
  Description of Modification:

 *****************************************************************************/

#include "arr_struct.h"

SC_MODULE( arraytypes ) {
  sc_in_clk clk;
  sc_in<bool>            reset;
  sc_in<arr_struct1>             in_value1;     
  sc_in<sc_lv<1> >              in_valid;      
  sc_out<arr_struct2>                   out_value1;    
  sc_out<sc_bit>                    out_valid;     

    SC_CTOR(arraytypes)
      {
	SC_CTHREAD (entry, clk.pos());
	reset_signal_is( reset, true );
      };
      
    void entry ();

};

