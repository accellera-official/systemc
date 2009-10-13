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
 
  driver.cpp -- Implementation of the driver.
 
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
#include "driver.h"

// Driver's output actions.
void
driver_mod::driver_out_proc()
{
  //  while (true) { 
  cout << "Driver is up @ " << sc_simulation_time() << endl;  

  // Car is at rest.
  speed_set = 0;
  reset = false;
  start = false;
  wait();
  
  cout << "Driver started the car @ " << sc_simulation_time() << endl;
  cout << "Driver set the speed to 40 km/h @ " << sc_simulation_time() << endl;
  start = true;
  speed_set = 40;
  wait();
  
  cout << "Driver set the speed to 120 km/h @ " << sc_simulation_time() << endl;
  speed_set = 120;
  wait();
  
  cout << "Driver reset the partial distance odometer @ " 
       << sc_simulation_time() << endl;
  cout << "Driver set the speed to 60 km/h @ " << sc_simulation_time() << endl;
  reset = true;
  speed_set = 60;
  wait();
  
  cout << "Driver set the speed to 40 km/h @ " << sc_simulation_time() << endl;
  speed_set = 40;
  wait();

  cout << "Driver stopped the car @ " << sc_simulation_time() << endl;
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
         << speed << " km/h @ " << sc_simulation_time() << endl;
  }

  if (angle.event()) {
    cout << "Current speedometer angle = " << angle
         << " degrees @ " << sc_simulation_time() << endl;
  }

  if (total.event()) {
    cout << "Current total distance displayed = " 
         << total << " km @ " << sc_simulation_time() << endl;  
  }

  if (partial.event()) {
    cout << "Current partial distance displayed = " 
         << partial << " km @ " << sc_simulation_time() << endl;  
  }
}

// End of file
