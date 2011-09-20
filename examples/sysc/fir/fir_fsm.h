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
 
  fir_fsm.h -- 
 
  Original Author: Rocco Jonack, Synopsys, Inc.
 
 *****************************************************************************/
 
/*****************************************************************************
 
  MODIFICATION LOG - modifiers, enter your name, affiliation, date and
  changes you are making here.
 
      Name, Affiliation, Date:
  Description of Modification:
    
 *****************************************************************************/

SC_MODULE(fir_fsm) {

  sc_in<bool>      clock;
  sc_in<bool>      reset;
  sc_in<bool>      in_valid;
  sc_out<unsigned> state_out;

  // defining the states of the ste machine
  enum {reset_s, first_s, second_s, third_s, output_s} state;

  SC_CTOR(fir_fsm) 
    { 
      SC_METHOD(entry);
      dont_initialize();
      sensitive << clock.pos();
    }
  void entry();
};

