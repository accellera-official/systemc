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

SC_MODULE( speed_read_mod )
{
  // Input ports:
  sc_in<bool> start; // Becomes true if the car's started.
  sc_in<bool> pulse; // Pulse coming from the pulse generator.
 
  // Output ports:
  sc_out<double> filtered_speed; // Filtered speed.  

  // Inout ports:
  sc_inout<int>  elapsed_time;

  // Internal signals:
  sc_signal<double> raw_speed;

  // Compute speed.
  void read_speed_proc();

  // Filter speed.
  void filter_speed_proc();

  SC_CTOR( speed_read_mod )
  {
    SC_THREAD( read_speed_proc );
    sensitive << pulse.pos();

    SC_METHOD( filter_speed_proc );
    sensitive << raw_speed;

    raw_speed = 0.0;
  }  
};

SC_MODULE( speed_pwm_mod )
{
  // Input ports:
  sc_in<bool> start; // Becomes true if the car's started.  
  sc_in<double> filtered_speed; 

  // Output ports:
  sc_out<double> speed; // Displayed speed.
  sc_out<double> angle; // Displayed angle.

  // Compute needle angle and drive the speedometer.
  void pwm_driver_proc();

  SC_CTOR( speed_pwm_mod )
  {
    SC_METHOD( pwm_driver_proc );
    sensitive << filtered_speed;    
  }  
};

SC_MODULE( speed_mod )
{
  // Input ports:
  sc_in_clk   clk; // Clock to measure the time, needed to compute the speed.
  sc_in<bool> start; // Becomes true if the car's started.
  sc_in<bool> pulse; // Pulse coming from the pulse generator.
 
  // Output ports:
  sc_out<double> speed; // Displayed speed.
  sc_out<double> angle; // Displayed angle.

  // Internal signals:
  sc_signal<int>    elapsed_time;
  sc_signal<double> filtered_speed;

  // Internal models:
  speed_read_mod *read_mod;
  speed_pwm_mod  *pwm_mod;

  // Find the elapsed_time between NUM_PULSES_FOR_SPEED pulses.
  void find_time_proc();

  SC_CTOR( speed_mod )
  {
    SC_METHOD( find_time_proc );
    sensitive << clk.pos();

    read_mod = new speed_read_mod("read_mod");
    pwm_mod = new speed_pwm_mod("pwm_mod");

    // read_mod->start.bind(start);
    // read_mod->pulse.bind(pulse);
    // read_mod->filtered_speed.bind(filtered_speed);
    // read_mod->elapsed_time.bind(elapsed_time);

    read_mod->start(start);
    read_mod->pulse(pulse);
    read_mod->filtered_speed(filtered_speed);
    read_mod->elapsed_time(elapsed_time);

    // (*read_mod)(start, pulse, filtered_speed, elapsed_time);

    // pwm_mod->start.bind(start);
    // pwm_mod->filtered_speed.bind(filtered_speed);
    // pwm_mod->speed.bind(speed);
    // pwm_mod->angle.bind(angle);

    pwm_mod->start(start);
    pwm_mod->filtered_speed(filtered_speed);
    pwm_mod->speed(speed);
    pwm_mod->angle(angle);

    // *pwm_mod << start << filtered_speed << speed << angle;

    elapsed_time = 0;
    filtered_speed = 0.0;
  }
};

#endif

