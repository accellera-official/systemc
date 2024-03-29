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

  stage1_2.h -- 

  Original Author: Martin Janssen, Synopsys, Inc., 2002-02-15

 *****************************************************************************/

/*****************************************************************************

  MODIFICATION LOG - modifiers, enter your name, affiliation, date and
  changes you are making here.

      Name, Affiliation, Date:
  Description of Modification:

 *****************************************************************************/

/* Filename stage1_2.h */
/* This is the interface file for module `stage1_2' */

#include "systemc.h"

struct stage1_2 : public sc_module { 
  sc_signal<double> sum; // internal signal
  sc_signal<double> diff; // internal signal

  stage1 S1; // component
  stage2 S2; // component

  //Constructor 
  stage1_2(sc_module_name NAME,
	   sc_clock& TICK,
	   const sc_signal<double>& IN1,
	   const sc_signal<double>& IN2,
	   sc_signal<double>& PROD,
	   sc_signal<double>& QUOT)
    : sc_module(NAME)
    , sum("SigSum")
    , diff("SigDiff")
    , S1("Stage1", TICK, IN1, IN2, sum, diff)
    , S2("Stage2", TICK, sum, diff, PROD, QUOT)
  {
  }
};


