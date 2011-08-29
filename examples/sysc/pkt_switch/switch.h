/*****************************************************************************

  The following code is derived, directly or indirectly, from the SystemC
  source code Copyright (c) 1996-2006 by all Contributors.
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

  switch.h - This is the interface file for the asynchronous process "switch".

  Original Author: Rashmi Goswami, Synopsys, Inc.

 *****************************************************************************/

/*****************************************************************************

  MODIFICATION LOG - modifiers, enter your name, affiliation, date and
  changes you are making here.

      Name, Affiliation, Date:
  Description of Modification:

 *****************************************************************************/

#ifndef SWITCH_H_INCLUDED
#define SWITCH_H_INCLUDED

#include "systemc.h"
#include "pkt.h"

struct mcast_pkt_switch : sc_module {

    sc_in<bool>  switch_cntrl;
    sc_in<pkt>  in0;
    sc_in<pkt>  in1;
    sc_in<pkt>  in2;
    sc_in<pkt>  in3;

    sc_out<pkt>  out0;
    sc_out<pkt>  out1;
    sc_out<pkt>  out2;
    sc_out<pkt>  out3;   

    SC_CTOR(mcast_pkt_switch) 
     {
      SC_THREAD(entry);
      sensitive << in0;
      sensitive << in1;
      sensitive << in2;
      sensitive << in3;
      sensitive << switch_cntrl.pos();
    }  

  void entry();  
 
};
#endif // SWITCH_H_INCLUDED
