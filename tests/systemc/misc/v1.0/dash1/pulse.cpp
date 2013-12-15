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
 
  pulse.cpp -- Implementation of the pulse generator.
 
  Original Author: Ali Dasdan, Synopsys, Inc.
 
 *****************************************************************************/

/*****************************************************************************

  MODIFICATION LOG - modifiers, enter your name, affiliation, date and
  changes you are making here.

      Name, Affiliation, Date:
  Description of Modification:

 *****************************************************************************/

// $Log: pulse.cpp,v $
// Revision 1.1.1.1  2006/12/15 20:26:24  acg
// systemc_tests-2.3
//
// Revision 1.3  2006/01/19 00:48:12  acg
// Andy Goodrich: Changes for the fact signal write checking is enabled.
//
// Revision 1.2  2006/01/18 00:23:50  acg
// Change over from SC_NO_WRITE_CHECK to sc_write_check_enable() call.
//

#define SC_NO_WRITE_CHECK
#include "systemc.h"
#include "const.h"
#include "pulse.h"

// Find the pulse period to produce speed.
// This function also rounds the period to the nearest integer.
int
gen_pulse_mod::find_period(int speed)
{
  if (speed <= 0)
    return 1;

  const double num = DIST_BETWEEN_TWO_PULSES * PERIODS_PER_HOUR / 2;

  double dp = num / speed;
  int    ip = int(dp);

  ip = ip + ((dp - ip) >= double(0.5) ? 1 : 0);

  return ip;
}

// Generate pulses for speedometer and odometers.
void
gen_pulse_mod::gen_pulse_proc()
{
  wait();

  speed_pulse = false;
  dist_pulse = false;

  // Wait until the car is started.
  do {
    wait();
  } while (start.read() == false);

  while (true) {

    speed_pulse = true;
    dist_pulse = true;
    AWAIT(find_period(speed));

    speed_pulse = false;
    dist_pulse = false;
    AWAIT(find_period(speed));
  }
}

// End of file
