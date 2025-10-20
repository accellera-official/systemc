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

  fifo.cpp - This is the functionality file for the SystemC structure "fifo".

  Original Author: Rashmi Goswami, Synopsys, Inc.

 *****************************************************************************/

/*****************************************************************************

  MODIFICATION LOG - modifiers, enter your name, affiliation, date and
  changes you are making here.

      Name, Affiliation, Date:
  Description of Modification:

 *****************************************************************************/

#include "fifo.h"

    void fifo::pkt_in(const pkt& data_pkt)
    {
      regs[pntr++] = data_pkt; empty = false;
      if (pntr == 4) full = true;      
    }

    pkt fifo::pkt_out()
    {
       pkt temp;
       temp = regs[0];
       if (--pntr == 0) empty = true;
       else 
	{ 
            regs[0] = regs[1];
	    regs[1] = regs[2];
	    regs[2] = regs[3];
	    full = false;
        } 
      return(temp);  
    }
