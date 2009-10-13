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
 
  speed.cpp -- Definition of the speedometer.
 
  Original Author: Ali Dasdan, Synopsys, Inc.
 
 *****************************************************************************/

/*****************************************************************************

  MODIFICATION LOG - modifiers, enter your name, affiliation, date and
  changes you are making here.

      Name, Affiliation, Date:
  Description of Modification:

 *****************************************************************************/

#include "systemc.h"
#include "const.h"
#include "speed.h"

// Find the elapsed_time between NUM_PULSES_FOR_SPEED pulses.
void speed_mod::find_time_proc()
{
  elapsed_time = elapsed_time + 1;
}

// Compute speed.
void
speed_mod::read_speed_proc()
{
  wait();

  double speed = 0.0;

  while (true) {

    // More than one pulse is needed to compute a distance and 
    // consequently, speed. This function collects NUM_PULSES_FOR_SPEED
    // pulses for that purpose.
    AWAIT(NUM_PULSES_FOR_SPEED);

    speed = DIST_BETWEEN_TWO_PULSES * PERIODS_PER_HOUR / elapsed_time;

    // Reset timer.
    elapsed_time = 0;

    cout << "Current speed displayed = " 
         << speed << " km/h @ " << sc_simulation_time() << endl;
    cout << "Current speedometer angle = " 
         << speed * MAX_ANGLE / MAX_SPEED 
         << " degrees @ " << sc_simulation_time() << endl;
  }
}

// End of file
