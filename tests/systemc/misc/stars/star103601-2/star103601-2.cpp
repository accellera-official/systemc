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

  star103601-2.cpp -- 

  Original Author: Martin Janssen, Synopsys, Inc., 2002-02-15

 *****************************************************************************/

/*****************************************************************************

  MODIFICATION LOG - modifiers, enter your name, affiliation, date and
  changes you are making here.

      Name, Affiliation, Date:
  Description of Modification:

 *****************************************************************************/

#include <systemc.h>
#include "test.h"

void test::reset_loop() {
  sc_uint<8> tmp, inp, caseexpr;
  
  wait();
  
  done = 0;
  dato = 0;
  tmp = 0;
  
  wait();
  operational_loop: while(1 != 0) {
    inp = dati.read();
    wait();
    inner : while(1 != 0) {
      dato = tmp;
      wait();

      caseexpr = inp;
      if(caseexpr == 1) {
        tmp = 1;
      } else if((caseexpr == 2) | (caseexpr == 3)) {
        tmp = 2;
      } else if(caseexpr != 4) {
        tmp = 5;
        break;
      } else {
        tmp = 4;
      }
      inp = inp + 1;
    }
    wait();
    done_loop : while(1) {
      dato = tmp;
      done = 1;
      wait();
    }
  }
}

