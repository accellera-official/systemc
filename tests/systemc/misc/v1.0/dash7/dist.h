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
 
  dist.h -- Definition of the odometers.
 
  Original Author: Ali Dasdan, Synopsys, Inc.
 
 *****************************************************************************/

/*****************************************************************************

  MODIFICATION LOG - modifiers, enter your name, affiliation, date and
  changes you are making here.

      Name, Affiliation, Date:
  Description of Modification:

 *****************************************************************************/

#ifndef DIST_H
#define DIST_H

SC_MODULE( dist_read_mod )
{
  // Input ports:
  sc_in<bool> pulse; // Pulse coming from the pulse generator.
  sc_in<bool> start; // Becomes true if the car's started.

  // Output ports:
  sc_out<bool> ok_for_incr;

  // Get the pulses for one distance increment.
  void get_dist_proc();

  SC_CTOR( dist_read_mod )
  {
    SC_THREAD( get_dist_proc );
    sensitive << pulse.pos();
  }
};

SC_MODULE( dist_compute_mod )
{
  // Input ports:
  sc_in<bool> reset; // Reset the partial distance odometer if true.
  sc_in<bool> start; // Becomes true if the car's started.
  sc_in<bool> ok_for_incr;

  // Output ports:
  sc_out<double> total_dist;
  sc_out<double> partial_dist;

  // Internal variables:
  static bool prev_reset;
  static double total_compute_dist;
  static double partial_compute_dist;

  // Compute total and partial distances.
  void compute_total_proc();
  void compute_partial_proc();

  SC_CTOR( dist_compute_mod )
  {
    SC_METHOD( compute_total_proc );
    sensitive << ok_for_incr;

    SC_METHOD( compute_partial_proc );
    sensitive << ok_for_incr;

    prev_reset = false;
    total_compute_dist = 0.0;
    partial_compute_dist = 0.0;
  }
};

SC_MODULE( dist_lcd_mod )
{
  // Input ports:
  sc_in<bool> start; // Becomes true if the car's started.
  sc_in<double> total_dist;
  sc_in<double> partial_dist;

  // Output ports:
  sc_out<double> total;   // Total distance.
  sc_out<double> partial; // Partial distance.

  // LCD display driver.
  void lcd_driver_proc();

  SC_CTOR( dist_lcd_mod )
  {
    SC_METHOD( lcd_driver_proc );
    sensitive << total_dist << partial_dist;
  }
};

SC_MODULE( dist_mod )
{
  // Ports:
  sc_in<bool> pulse; // Pulse coming ftom the pulse generator.
  sc_in<bool> reset; // Reset the partial distance odometer if true.
  sc_in<bool> start; // Becomes true if the car's started.

  // Output ports:
  sc_out<double> total;   // Total distance.
  sc_out<double> partial; // Partial distance.

  // Internal signals:
  sc_signal<bool>   ok_for_incr;
  sc_signal<double> total_dist;
  sc_signal<double> partial_dist;

  // Internal models:
  dist_read_mod *read_mod;
  dist_compute_mod *compute_mod;
  dist_lcd_mod *lcd_mod;

  SC_CTOR( dist_mod )
  {
    read_mod = new dist_read_mod("read_mod");
    compute_mod = new dist_compute_mod("compute_mod");
    lcd_mod = new dist_lcd_mod("lcd_mod");

    // read_mod->pulse.bind(pulse);
    // read_mod->start.bind(start);
    // read_mod->ok_for_incr.bind(ok_for_incr);
    read_mod->pulse(pulse);
    read_mod->start(start);
    read_mod->ok_for_incr(ok_for_incr);

    // compute_mod->reset.bind(reset);
    // compute_mod->start.bind(start);
    // compute_mod->ok_for_incr.bind(ok_for_incr);
    // compute_mod->total_dist.bind(total_dist);
    // compute_mod->partial_dist.bind(partial_dist);
    compute_mod->reset(reset);
    compute_mod->start(start);
    compute_mod->ok_for_incr(ok_for_incr);
    compute_mod->total_dist(total_dist);
    compute_mod->partial_dist(partial_dist);

    // lcd_mod->start.bind(start);
    // lcd_mod->total_dist.bind(total_dist);
    // lcd_mod->partial_dist.bind(partial_dist);
    // lcd_mod->total.bind(total);
    // lcd_mod->partial.bind(partial);
    lcd_mod->start.bind(start);
    lcd_mod->total_dist.bind(total_dist);
    lcd_mod->partial_dist(partial_dist);
    lcd_mod->total(total);
    lcd_mod->partial.bind(partial);

    ok_for_incr = false;
    total_dist = 0.0;
    partial_dist = 0.0;
  }
};

#endif
