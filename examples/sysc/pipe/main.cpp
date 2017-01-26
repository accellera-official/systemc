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
 
  main.cpp -- This is the top level file instantiating the modules and
              binding ports to signals.
 
  Original Author: Amit Rao, Synopsys, Inc.
 
 *****************************************************************************/

/*****************************************************************************

  MODIFICATION LOG - modifiers, enter your name, affiliation, date and
  changes you are making here.

      Name, Affiliation, Date:
  Description of Modification:

 *****************************************************************************/

#include "systemc.h"
#include "stage1.h"
#include "stage2.h"
#include "stage3.h"
#include "display.h"
#include "numgen.h"

int sc_main(int, char *[])
{
  //Signals
  sc_signal<double> in1;
  sc_signal<double> in2;
  sc_signal<double> sum;
  sc_signal<double> diff;
  sc_signal<double> prod;
  sc_signal<double> quot;
  sc_signal<double> powr;  
  //Clock
  sc_signal<bool>   clk;
  
  numgen N("numgen");               //instance of `numgen' module
  N(in1, in2, clk );                //Positional port binding
  
  stage1 S1("stage1");              //instance of `stage1' module
  //Named port binding
  S1.in1(in1);
  S1.in2(in2);
  S1.sum(sum);
  S1.diff(diff);
  S1.clk(clk);
  
  stage2 S2("stage2");              //instance of `stage2' module
  S2(sum, diff, prod, quot, clk );  //Positional port binding

  stage3 S3("stage3");              //instance of `stage3' module
  S3( prod, quot, powr, clk);       //Positional port binding
  
  display D("display");             //instance of `display' module
  D(powr, clk);                     //Positional port binding 

  sc_start(0, SC_NS);               //Initialize simulation
  for(int i = 0; i < 50; i++){
      clk.write(1);
      sc_start( 10, SC_NS );
      clk.write(0);
      sc_start( 10, SC_NS );
  }

  return 0;
}

