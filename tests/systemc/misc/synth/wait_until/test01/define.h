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

  define.h -- 

  Original Author: Martin Janssen, Synopsys, Inc., 2002-02-15

 *****************************************************************************/

/*****************************************************************************

  MODIFICATION LOG - modifiers, enter your name, affiliation, date and
  changes you are making here.

      Name, Affiliation, Date:
  Description of Modification:

 *****************************************************************************/

#define CLOCK_PERIOD 100
#define DUTY_CYCLE 0.5
#define EVENT_TIME 50
#define TEST_TIME 50

#define long_wait wait(5*CLOCK_PERIOD)
#define single_cycle wait()
#define set_value(var,val) wait(EVENT_TIME); var = val; wait(CLOCK_PERIOD - EVENT_TIME)
#define test_value(actual, expected) \
	wait (TEST_TIME); if (expected != actual) \
                cout << "Mismatch. Expected: " << expected \
	        << ". Actual: " << actual << endl; \
	wait (CLOCK_PERIOD - TEST_TIME)
#define test_value_now(actual, expected) \
     if (expected != actual) cout << "Mismatch. Expected: " << expected \
	        << ". Actual: " << actual << endl;

