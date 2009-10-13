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

  const.h -- Constants for the dashboard controller.
 
  Original Author: Ali Dasdan, Synopsys, Inc.

 *****************************************************************************/

/*****************************************************************************

  MODIFICATION LOG - modifiers, enter your name, affiliation, date and
  changes you are making here.

      Name, Affiliation, Date:
  Description of Modification:

 *****************************************************************************/

#ifndef CONST_H
#define CONST_H

const int MAX_SPEED = 130; // Car speed in km/h.
const int MAX_ANGLE = 270; // Needle angle in the speedometer in degrees.

const double DIST_BETWEEN_TWO_PULSES = 0.00066 / 2; // 1/2 of a tire in km.
const double DIST_INCR               = 0.01; // One distance increment in km.

// Two pulses are needed to compute speed.
const int NUM_PULSES_FOR_SPEED     = 2;   

// DIST_INCR / DIST_BETWEEN_TWO_PULSES
const int NUM_PULSES_FOR_DIST_INCR = 61;  

const int SLOW_CLOCK_PERIOD0    = 10000;    // in 10 x milliseconds.
const int FAST_CLOCK_PERIOD1    = 50;       // in 10 x milliseconds.
const int ONE_HOUR              = 36000000; // in 10 x milliseconds.
const int PERIODS_PER_HOUR      = ONE_HOUR / FAST_CLOCK_PERIOD1;

#define AWAIT(N) \
  for (register int i = 0; i < N; ++i)  \
    wait();

#endif
