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
 
  driver.h -- Definition of the driver.
 
  Original Author: Ali Dasdan, Synopsys, Inc.
 
 *****************************************************************************/

/*****************************************************************************

  MODIFICATION LOG - modifiers, enter your name, affiliation, date and
  changes you are making here.

      Name, Affiliation, Date:
  Description of Modification:

 *****************************************************************************/

#ifndef DRIVER_H
#define DRIVER_H

SC_MODULE( driver_mod )
{
  // Input ports:
  sc_in_clk     clk;   // Clock for the actions of the driver.
  sc_in<double> speed;
  sc_in<double> angle;
  sc_in<double> total;
  sc_in<double> partial;

  // Output ports:
  sc_out<bool>  reset; // Set if the driver wants to reset the partial
                       // distance odometer.
  sc_out<int>   speed_set; // Speed of the car as set by the driver.
  sc_out<bool>  start;     // Set if the driver starts the car.

  // Driver's actions.
  void driver_out_proc();
  void driver_in_proc();

  SC_CTOR( driver_mod )
  {
    SC_CTHREAD( driver_out_proc, clk.pos() );

    SC_METHOD( driver_in_proc );
    sensitive << speed << angle << total << partial;
  }
};

#endif
