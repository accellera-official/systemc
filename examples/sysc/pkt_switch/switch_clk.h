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

  switch_clk.h - This is the interface file for the synchronous process 
                 "switch_clk".

  Original Author: Rashmi Goswami, Synopsys, Inc.

 *****************************************************************************/

/*****************************************************************************

  MODIFICATION LOG - modifiers, enter your name, affiliation, date and
  changes you are making here.

      Name, Affiliation, Date:
  Description of Modification:

 *****************************************************************************/

#ifndef SWITCH_CLK_H_INCLUDED
#define SWITCH_CLK_H_INCLUDED

#include "systemc.h"

struct switch_clk: sc_module {
      sc_out<bool>   switch_cntrl;
      sc_in_clk CLK;

      SC_CTOR(switch_clk)
        {
           SC_METHOD(entry);
           dont_initialize();
           sensitive << CLK.pos(); 
         }
     void entry();
};
#endif // SWITCH_CLK_H_INCLUDED
