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

int sc_main(int , char*[])
{
 sc_signal<float> in_real;
 sc_signal<float> in_imag;
 sc_signal<bool> data_valid;
 sc_signal<bool> data_ack;
 sc_signal<float> out_real;
 sc_signal<float> out_imag;
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
