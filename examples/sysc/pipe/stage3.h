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
 
  stage3.h -- This is the interface file for the stage3 module.
 
  Original Author: Amit Rao, Synopsys, Inc.
 
 *****************************************************************************/

/*****************************************************************************

  MODIFICATION LOG - modifiers, enter your name, affiliation, date and
  changes you are making here.

      Name, Affiliation, Date:
  Description of Modification:

 *****************************************************************************/

#ifndef STAGE3_H
#define STAGE3_H

struct stage3: sc_module {
    sc_in<double>  prod;     //input port 1
    sc_in<double>  quot;     //input port 2
    sc_out<double> powr;     //output port 1 
    sc_in<bool>    clk;      //clock

    void power();            //method implementing functionality

    //Constructor
    SC_CTOR( stage3 ){
        SC_METHOD( power );      //declare power as SC_METHOD and 
        dont_initialize();
        sensitive << clk.pos();  //make it sensitive to positive clock edge 
    }

};


#endif
