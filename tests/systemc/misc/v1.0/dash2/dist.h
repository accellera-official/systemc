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

  // Internal variables:
  static bool prev_reset;

  // Get the pulses for one distance increment.
  void get_dist_proc();

  // Compute total and partial distances.
  void compute_total_proc();
  void compute_partial_proc();

  // LCD display driver.
  void lcd_driver_proc();

  SC_CTOR( dist_mod )
  {
    SC_THREAD( get_dist_proc );
    sensitive_pos << pulse;

    SC_METHOD( compute_total_proc );
    sensitive << ok_for_incr;

    SC_METHOD( compute_partial_proc );
    sensitive << ok_for_incr;

    SC_METHOD( lcd_driver_proc );
    sensitive << total_dist << partial_dist;

    ok_for_incr = false;
    total_dist = 0.0;
    partial_dist = 0.0;
  }

};

#endif
