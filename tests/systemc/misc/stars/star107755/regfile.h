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

  regfile.h -- 

  Original Author: Martin Janssen, Synopsys, Inc., 2002-02-15

 *****************************************************************************/

/*****************************************************************************

  MODIFICATION LOG - modifiers, enter your name, affiliation, date and
  changes you are making here.

      Name, Affiliation, Date:
  Description of Modification:

 *****************************************************************************/

struct test : sc_module {
  
  sc_in<bool> reset;
  sc_in_clk clk;
  sc_in<sc_lv<14> >   dati;
  sc_out<sc_lv<14> >  dato;
  sc_out<sc_logic>    ready, done;
  
  SC_HAS_PROCESS( test );
  
  test (const char *NAME) : sc_module(NAME) {
    SC_CTHREAD( reset_loop, clk.pos() );
    watching(reset.delayed()==true);
    end_module();
  }
  
  void reset_loop();
};

