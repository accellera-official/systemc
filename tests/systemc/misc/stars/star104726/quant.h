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

  quant.h -- 

  Original Author: Martin Janssen, Synopsys, Inc., 2002-02-15

 *****************************************************************************/

/*****************************************************************************

  MODIFICATION LOG - modifiers, enter your name, affiliation, date and
  changes you are making here.

      Name, Affiliation, Date:
  Description of Modification:

 *****************************************************************************/

#ifndef QUANTH
#define QUANTH

#include <systemc.h>
#include "global.h"

SC_MODULE(quant) {

  sc_in_clk        clk;

  sc_in<Coeff8x8>  data_in; 
  sc_in<bool>      start;
  sc_in<bool>      data_ok;

  sc_out<Coeff8x8> data_out;
  sc_out<bool>     ready;
  sc_out<bool>     data_out_ready;    

  void do_quant();

  SC_CTOR(quant) {
    SC_CTHREAD(do_quant,clk.pos());
  };

};


#endif

