/*****************************************************************************

  The following code is derived, directly or indirectly, from the SystemC
  source code Copyright (c) 1996-2005 by all Contributors.
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

  design.h -- 

  Original Author: Martin Janssen, Synopsys, Inc., 2002-02-15

 *****************************************************************************/

/*****************************************************************************

  MODIFICATION LOG - modifiers, enter your name, affiliation, date and
  changes you are making here.

      Name, Affiliation, Date:
  Description of Modification:

 *****************************************************************************/


SC_MODULE(design) {
  // ports
  sc_in_clk    clock;
  sc_in<bool > take_the_data;
  sc_in<bool > data_request;
  sc_out<bool > data_ready;
  sc_out<bool > write_in_is_done;
  sc_in <DATA_TYPE > input;
  sc_out<DATA_TYPE > output;
  sc_signal<DATA_TYPE > ring_buffer[RING_BUFFER_SIZE];
  sc_signal<INDEX_TYPE > read_pointer;
  sc_signal<INDEX_TYPE > write_pointer;
  // processes
  void write_in_fifo();
  void read_out_fifo();
  SC_CTOR(design) {
    SC_CTHREAD(write_in_fifo, clock.pos() );
    SC_CTHREAD(read_out_fifo, clock.pos() );
    // bad reset
    write_pointer = (INDEX_TYPE)0 ;
    read_pointer = (INDEX_TYPE)0 ;
  }  

};
