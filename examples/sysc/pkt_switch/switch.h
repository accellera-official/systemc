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
