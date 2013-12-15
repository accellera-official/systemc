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
 
  driver.cpp -- Implementation of the driver.
 
  Original Author: Ali Dasdan, Synopsys, Inc.
 
 *****************************************************************************/

/*****************************************************************************

  MODIFICATION LOG - modifiers, enter your name, affiliation, date and
  changes you are making here.

      Name, Affiliation, Date:
  Description of Modification:

 *****************************************************************************/

// $Log: driver.cpp,v $
// Revision 1.1.1.1  2006/12/15 20:26:24  acg
// systemc_tests-2.3
//
// Revision 1.4  2006/01/24 21:05:59  acg
//  Andy Goodrich: replacement of deprecated features with their non-deprecated
//  counterparts.
//
// Revision 1.3  2006/01/19 00:48:20  acg
// Andy Goodrich: Changes for the fact signal write checking is enabled.
//
// Revision 1.2  2006/01/18 00:23:51  acg
// Change over from SC_NO_WRITE_CHECK to sc_write_check_enable() call.
//

#define SC_NO_WRITE_CHECK
#include "systemc.h"
#include "const.h"
#include "driver.h"

// Driver's output actions.
void
driver_mod::driver_out_proc()
{
  //  while (true) { 
  cout << "Driver is up @ " << sc_time_stamp() << endl;  

  // Car is at rest.
  speed_set = 0;
  reset = false;
  start = false;
  wait();
  
  cout << "Driver started the car @ " << sc_time_stamp() << endl;
  cout << "Driver set the speed to 40 km/h @ " << sc_time_stamp() << endl;
  start = true;
  speed_set = 40;
  wait();
  
  cout << "Driver set the speed to 120 km/h @ " << sc_time_stamp() << endl;
  speed_set = 120;
  wait();
  
  cout << "Driver reset the partial distance odometer @ " 
       << sc_time_stamp() << endl;
  cout << "Driver set the speed to 60 km/h @ " << sc_time_stamp() << endl;
  reset = true;
  speed_set = 60;
  wait();
  
  cout << "Driver set the speed to 40 km/h @ " << sc_time_stamp() << endl;
  speed_set = 40;
  wait();

  cout << "Driver stopped the car @ " << sc_time_stamp() << endl;
  wait();

  sc_stop();
  //  }
}

// Driver's input actions.
void
driver_mod::driver_in_proc()
{
  if (speed.event()) {
    cout << "Current speed displayed = " 
         << speed << " km/h @ " << sc_time_stamp() << endl;
  }

  if (angle.event()) {
    cout << "Current speedometer angle = " << angle
         << " degrees @ " << sc_time_stamp() << endl;
  }

  if (total.event()) {
    cout << "Current total distance displayed = " 
         << total << " km @ " << sc_time_stamp() << endl;  
  }

  if (partial.event()) {
    cout << "Current partial distance displayed = " 
         << partial << " km @ " << sc_time_stamp() << endl;  
  }
}

// End of file
