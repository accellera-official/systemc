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

  main.cpp - This file instantiates all processes and ties them together
             with signals.

  Original Author: Rashmi Goswami, Synopsys, Inc.

 *****************************************************************************/

/*****************************************************************************

  MODIFICATION LOG - modifiers, enter your name, affiliation, date and
  changes you are making here.

      Name, Affiliation, Date:
  Description of Modification:

 *****************************************************************************/

#include "systemc.h"
#include "pkt.h"
#include "switch_clk.h"
#include "sender.h"
#include "receiver.h"
#include "switch.h"


int
sc_main(int, char *[])
{
  sc_signal<pkt> pkt_in0;
  sc_signal<pkt> pkt_in1;
  sc_signal<pkt> pkt_in2;
  sc_signal<pkt> pkt_in3;
  sc_signal<pkt> pkt_out0;
  sc_signal<pkt> pkt_out1;
  sc_signal<pkt> pkt_out2;
  sc_signal<pkt> pkt_out3;

  sc_signal<sc_int<4> > id0, id1, id2, id3;
  
  sc_signal<bool> switch_cntrl;

  sc_clock clock1("CLOCK1", 75, SC_NS, 0.5, 0.0, SC_NS);
  sc_clock clock2("CLOCK2", 30, SC_NS, 0.5, 10.0, SC_NS);

  // Module instiatiations follow
  // Note that modules can be connected by hooking up ports 
  // to signals by name or by using a positional notation

  sender sender0("SENDER0");
  // hooking up signals to ports by name
  sender0.pkt_out(pkt_in0);
  sender0.source_id(id0);
  sender0.CLK(clock1);

  sender sender1("SENDER1");
  // hooking up signals to ports by position
  sender1(pkt_in1, id1, clock1);

  sender sender2("SENDER2");
  // hooking up signals to ports by name
  sender2.pkt_out(pkt_in2);
  sender2.source_id(id2);
  sender2.CLK(clock1);

  sender sender3("SENDER3");
  // hooking up signals to ports by position
  sender3( pkt_in3, id3, clock1 );
  
  switch_clk switch_clk1("SWITCH_CLK");
  // hooking up signals to ports by name
  switch_clk1.switch_cntrl(switch_cntrl);
  switch_clk1.CLK(clock2);

  mcast_pkt_switch switch1("SWITCH");
  // hooking up signals to ports by name
  switch1.switch_cntrl(switch_cntrl);
  switch1.in0(pkt_in0);
  switch1.in1(pkt_in1);
  switch1.in2(pkt_in2);
  switch1.in3(pkt_in3);
  switch1.out0(pkt_out0);
  switch1.out1(pkt_out1);
  switch1.out2(pkt_out2);
  switch1.out3(pkt_out3);
 
  receiver receiver0("RECEIVER0");
  // hooking up signals to ports by name  
  receiver0.pkt_in(pkt_out0);
  receiver0.sink_id(id0);

  receiver receiver1("RECEIVER1");
  // hooking up signals to ports by position
  receiver1( pkt_out1, id1 );

  receiver receiver2("RECEIVER2");
  // hooking up signals to ports by name
  receiver2.pkt_in(pkt_out2);
  receiver2.sink_id(id2);

  receiver receiver3("RECEIVER3");
  // hooking up signals to ports by position
  receiver3( pkt_out3, id3 );

  sc_start(0, SC_NS);

#ifndef __SUNPRO_CC
  id0.write(0); 
  id1.write(1);
  id2.write(2);
  id3.write(3);
#else
  // you cannot do that with SC5.0
  // since it doesn't support member templates
  id0.write(sc_int<4>(0));
  id0.write(sc_int<4>(1));
  id0.write(sc_int<4>(2));
  id0.write(sc_int<4>(3));
#endif
  sc_start();
  return 0;

}

