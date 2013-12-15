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
 
  speed.h -- Definition of the speedometer.
 
  Original Author: Ali Dasdan, Synopsys, Inc.
 
 *****************************************************************************/

/*****************************************************************************

  MODIFICATION LOG - modifiers, enter your name, affiliation, date and
  changes you are making here.

      Name, Affiliation, Date:
  Description of Modification:

 *****************************************************************************/

#ifndef SPEED_H
#define SPEED_H

SC_MODULE( speed_mod )
{
  // Input ports:
  sc_in_clk clk; // Clock to measure the time, needed to compute the speed.
  sc_in<bool> start; // Becomes true if the car's started.
  sc_in<bool> pulse; // Pulse coming from the pulse generator.

  // Output ports:
  sc_out<double> speed; // Displayed speed.
  sc_out<double> angle; // Displayed angle.

  // Internal signals:
  sc_signal<int> elapsed_time;

  // Find the elapsed_time between NUM_PULSES_FOR_SPEED pulses.
  void find_time_proc();

  // Compute speed.
  void read_speed_proc();

  SC_CTOR( speed_mod )
  {
    SC_METHOD( find_time_proc );
    sensitive << clk.pos();

    SC_THREAD( read_speed_proc );
    sensitive << pulse.pos();

    elapsed_time = 0;
  }

};

#endif

