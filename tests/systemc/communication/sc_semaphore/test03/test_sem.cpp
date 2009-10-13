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

  test_sem.cpp -- 

  Original Author: Martin Janssen, Synopsys, Inc., 2002-02-15

 *****************************************************************************/

/*****************************************************************************

  MODIFICATION LOG - modifiers, enter your name, affiliation, date and
  changes you are making here.

      Name, Affiliation, Date:
  Description of Modification:

 *****************************************************************************/

#include "test_sem.h"

TestSem::TestSem( sc_module_name )
        :sem_1(5)
{
    SC_CTHREAD(body_1,clk.pos());
    SC_CTHREAD(body_2,clk.pos());
}

void TestSem::body_1()
{
    unsigned int loop_counter=0;
    char buf[BUFSIZ];
    
    while (loop_counter++<10 && !sem_1.wait())
    {
        sprintf(buf, "time %f => thread1 : took semaphore %d times\n",
                sc_time_stamp().to_double(), loop_counter);
        cout << buf << flush;
}

    sprintf(buf, "time %f => thread1 : value of semaphore = %d\n",
            sc_time_stamp().to_double(), sem_1.get_value());
    cout << buf << flush;
    
    sc_stop();
}

void TestSem::body_2()
{
    unsigned int loop_counter=0;
    char buf[BUFSIZ];

    do
    {
        wait(2);
        sem_1.post();
        sprintf(buf, "time %f => thread2 : posted semaphore 1\n",
                sc_time_stamp().to_double());
        cout << buf << flush;
    }
    while (loop_counter++ < 5);

    wait(100);
}
