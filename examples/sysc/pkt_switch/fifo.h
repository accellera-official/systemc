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

  fifo.h - This is the header file for the SystemC structure "fifo".

  Original Author: Rashmi Goswami, Synopsys, Inc.

 *****************************************************************************/

/*****************************************************************************

  MODIFICATION LOG - modifiers, enter your name, affiliation, date and
  changes you are making here.

      Name, Affiliation, Date:
  Description of Modification:

 *****************************************************************************/

#ifndef FIFO_H_INCLUDED
#define FIFO_H_INCLUDED

#include "systemc.h"
#include "pkt.h"

struct fifo {

   pkt regs[4];
   bool full;
   bool empty;
   sc_uint<3> pntr;
  
   // constructor

   fifo()
    {
      full = false;
      empty = true;
      pntr = 0;
    }

  // methods
   
   void pkt_in(const pkt& data_pkt);
   
   pkt pkt_out();
};
#endif // FIFO_H_INCLUDED
