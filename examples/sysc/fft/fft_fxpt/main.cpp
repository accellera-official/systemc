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
#include "fft.h"
#include "source.h"
#include "sink.h"

int sc_main(int , char* [])
{
 sc_signal<sc_int<16> > in_real;
 sc_signal<sc_int<16> > in_imag;
 sc_signal<bool> data_valid;
 sc_signal<bool> data_ack;
 sc_signal<sc_int<16> > out_real;
 sc_signal<sc_int<16> >  out_imag;
 sc_signal<bool> data_req;
 sc_signal<bool> data_ready;

 sc_clock clock("CLOCK", 10, SC_NS, 0.5, 0.0, SC_NS);

 fft FFT1("FFTPROCESS"); 
 FFT1.in_real(in_real);
 FFT1.in_imag(in_imag);
 FFT1.data_valid(data_valid);
 FFT1.data_ack(data_ack);
 FFT1.out_real(out_real);
 FFT1.out_imag(out_imag);
 FFT1.data_req(data_req);
 FFT1.data_ready(data_ready);
 FFT1.CLK(clock);

 source SOURCE1("SOURCEPROCESS");
 SOURCE1.data_req(data_req);
 SOURCE1.out_real(in_real);
 SOURCE1.out_imag(in_imag);
 SOURCE1.data_valid(data_valid);
 SOURCE1.CLK(clock);
 
 sink SINK1("SINKPROCESS");
 SINK1.data_ready(data_ready);
 SINK1.data_ack(data_ack);
 SINK1.in_real(out_real);
 SINK1.in_imag(out_imag);
 SINK1.CLK(clock);

 sc_start();

 return 0;
}
