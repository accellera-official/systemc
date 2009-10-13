/*****************************************************************************

  The following code is derived, directly or indirectly, from the SystemC
  source code Copyright (c) 1996-2002 by all Contributors.
  All Rights reserved.

  The contents of this file are subject to the restrictions and limitations
  set forth in the SystemC Open Source License Version 2.3 (the "License");
  You may not use this file except in compliance with such restrictions and
  limitations. You may obtain instructions on how to receive a copy of the
  License at http://www.systemc.org/. Software distributed by Contributors
  under the License is distributed on an "AS IS" basis, WITHOUT WARRANTY OF
  ANY KIND, either express or implied. See the License for the specific
  language governing rights and limitations under the License.

 *****************************************************************************/

/*****************************************************************************
 
  dist.cpp -- Implementation of the odometers.
 
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
#include "dist.h"

bool dist_mod::prev_reset;

// Compute the total and partial distances travelled.
void
dist_mod::get_dist_proc()
{
  wait();

  double tmp_total = 0;
  double tmp_partial = 0;

  while (true) {

    // More than one pulse is needed for a distance increment.  This
    // function collects NUM_PULSES_FOR_DIST_INCR pulses for that
    // purpose.
    AWAIT(NUM_PULSES_FOR_DIST_INCR);

    if (start) {
      
      // Increment the distances:
      tmp_total += DIST_INCR;
      
      // This is to simulate reset.event():
      if (prev_reset != (bool) reset)
        tmp_partial = 0.0;  // If reset.event(), reset the partial distance.
      else
        tmp_partial += DIST_INCR;

      prev_reset = reset;

    }
    else {
      prev_reset = false;
      tmp_total = 0.0;
      tmp_partial = 0.0;
    }

    total = tmp_total;
    partial = tmp_partial;

  }
}

// End of file
