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
 
  display.h -- This is the interface file for the display module.
 
  Original Author: Amit Rao, Synopsys, Inc.
 
 *****************************************************************************/

/*****************************************************************************

  MODIFICATION LOG - modifiers, enter your name, affiliation, date and
  changes you are making here.

      Name, Affiliation, Date:
  Description of Modification:

 *****************************************************************************/

#ifndef DISPLAY_H
#define DISPLAY_H 

struct display : sc_module {
    sc_in<double> in;        // input port 1
    sc_in<bool>   clk;       // clock

    void print_result();     // method to display input port values
    
    //Constructor
    SC_CTOR( display ) {
	SC_METHOD( print_result ); // declare print_result as SC_METHOD and 
        dont_initialize();
	sensitive << clk.pos();    // make it sensitive to positive clock edge
    }
	
};

#endif
