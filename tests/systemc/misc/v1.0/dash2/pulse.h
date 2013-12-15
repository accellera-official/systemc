/*****************************************************************************

  The following code is derived, directly or indirectly, from the SystemC
  source code Copyright (c) 1996-2014 by all Contributors.
  All Rights reserved.

  The contents of this file are subject to the restrictions and limitations
  set forth in the SystemC Open Source License (the "License");
  You may not use this file except in compliance with such restrictions and
  limitations. You may obtain instructions on how to receive a copy of the
  License at http://www.accellera.org/. Software distributed by Contributors
  under the License is distributed on an "AS IS" basis, WITHOUT WARRANTY OF
  ANY KIND, either express or implied. See the License for the specific
  language governing rights and limitations under the License.

 *****************************************************************************/

/*****************************************************************************
 
  pulse.h -- Definition of the pulse generator.
 
  Original Author: Ali Dasdan, Synopsys, Inc.
 
 *****************************************************************************/

/*****************************************************************************

  MODIFICATION LOG - modifiers, enter your name, affiliation, date and
  changes you are making here.

      Name, Affiliation, Date:
  Description of Modification:

 *****************************************************************************/

#ifndef PULSE_H
#define PULSE_H

SC_MODULE( gen_pulse_mod )
{
  // Ports:
  sc_in_clk    clk;          // Clock for the pulse generator.
  sc_in<bool>  start;        // Becomes true if the car's started.
  sc_in<int>   speed;        // Speed of the car set by the driver.
  sc_out<bool> speed_pulse;  // Pulses for the speedometer.
  sc_out<bool> dist_pulse;   // Pulses for the odometers.

  // Find the pulse period to produce speed.
  int find_period(int speed);

  // Generate pulses for speedometer and odometers.
  void gen_pulse_proc();

  SC_CTOR( gen_pulse_mod )
  {
    SC_THREAD( gen_pulse_proc );
    sensitive << clk << start;
  }

};

#endif

