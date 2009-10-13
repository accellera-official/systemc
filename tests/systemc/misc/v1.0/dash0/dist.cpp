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
 
  dist.cpp -- Implementation of the odometers.
 
  Original Author: Ali Dasdan, Synopsys, Inc.
 
 *****************************************************************************/

/*****************************************************************************

  MODIFICATION LOG - modifiers, enter your name, affiliation, date and
  changes you are making here.

      Name, Affiliation, Date:
  Description of Modification:

 *****************************************************************************/

// $Log: dist.cpp,v $
// Revision 1.1.1.1  2006/12/15 20:37:20  acg
// SystemC Tests 2.2
//
// Revision 1.4  2006/01/24 21:05:51  acg
//  Andy Goodrich: replacement of deprecated features with their non-deprecated
//  counterparts.
//
// Revision 1.3  2006/01/19 00:48:10  acg
// Andy Goodrich: Changes for the fact signal write checking is enabled.
//
// Revision 1.2  2006/01/18 00:23:44  acg
// Change over from SC_NO_WRITE_CHECK to sc_write_check_enable() call.
//

#define SC_NO_WRITE_CHECK
#include "systemc.h"
#include "const.h"
#include "dist.h"

// Compute the total and partial distances travelled.
void
dist_mod::get_dist_proc()
{
  wait();

  double total = 0.0;
  double partial = 0.0;

  while (true) {

    // More than one pulse is needed for a distance increment.  This
    // function collects NUM_PULSES_FOR_DIST_INCR pulses for that
    // purpose.
    AWAIT(NUM_PULSES_FOR_DIST_INCR);

    // Increment the distances:
    total = total + DIST_INCR;
    partial = partial + DIST_INCR;

    cout << "Current total distance displayed = " 
         << total << " km @ " << sc_time_stamp() << endl;  
    cout << "Current partial distance displayed = " 
         << partial << " km @ " << sc_time_stamp() << endl;  

    if (total >= 3 * DIST_INCR)
      sc_stop();
  }
}

// End of file
