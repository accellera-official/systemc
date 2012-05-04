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

  fft.h - This is the interface file for the synchronous process "fft".

  Original Author: Rashmi Goswami, Synopsys, Inc.

 *****************************************************************************/

/*****************************************************************************

  MODIFICATION LOG - modifiers, enter your name, affiliation, date and
  changes you are making here.

      Name, Affiliation, Date:
  Description of Modification:

 *****************************************************************************/


struct fft: sc_module {
  sc_in<float> in_real;   
  sc_in<float> in_imag;   
  sc_in<bool> data_valid; 
  sc_in<bool> data_ack;   
  sc_out<float> out_real;        
  sc_out<float> out_imag;        
  sc_out<bool> data_req;         
  sc_out<bool> data_ready;       
  sc_in_clk CLK;

 
  SC_CTOR(fft)
    {
      SC_CTHREAD(entry, CLK.pos());
    }

 void entry();  
};      


