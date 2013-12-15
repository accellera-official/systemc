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

  star107755.cpp -- 

  Original Author: Martin Janssen, Synopsys, Inc., 2002-02-15

 *****************************************************************************/

/*****************************************************************************

  MODIFICATION LOG - modifiers, enter your name, affiliation, date and
  changes you are making here.

      Name, Affiliation, Date:
  Description of Modification:

 *****************************************************************************/

#include "systemc.h"
#include "regfile.h"

void test::reset_loop() {
  sc_uint<14> cell[2];
  sc_uint<14> cell1[2];

  sc_uint<14> dat0, dat3;
  sc_uint<14> out0, out3;
  
  dato.write(0);
  dat0 = dat3 = out0 = out3 = 0;
  ready.write(sc_logic('0'));
  done.write(sc_logic('0'));
  
  wait();
  while (1) {
    ready.write(sc_logic('1'));
    wait();

    ready.write(sc_logic('0'));
    dat0 = dati.read();
    wait();
    
    dat3 = dati.read();

    cell[0] = dat0;

    cell1[1] = dat3;

    out0 = cell[0];
    out3 = cell1[1];
    
    dato.write(out0);
    done.write(sc_logic('1'));
    wait();

    dato.write(out3);
    done.write(sc_logic('0'));
    wait();

  }
}
