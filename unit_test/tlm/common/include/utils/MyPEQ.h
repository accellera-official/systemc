/*****************************************************************************

  The following code is derived, directly or indirectly, from the SystemC
  source code Copyright (c) 1996-2007 by all Contributors.
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

#ifndef MY_PEQ_H
#define MY_PEQ_H

//#include "tlm.h"
//#include <systemc.h>
#include <map>

class MyPEQ : public sc_module
{
public:
  typedef tlm::tlm_generic_payload transaction_type;

public:
  SC_HAS_PROCESS(MyPEQ);
  MyPEQ(sc_module_name name) : sc_module(name)
  {
  }

  void notify(transaction_type& trans, sc_time& t)
  {
    mScheduledEvents.insert(std::make_pair(t + sc_time_stamp(), &trans));
    mEvent.notify(t);
  }

  // needs to be called until it returns 0
  transaction_type* getNextTransaction()
  {
    if (mScheduledEvents.empty()) {
      return 0;
    }

    sc_time now = sc_time_stamp();
    if (mScheduledEvents.begin()->first <= now) {
      transaction_type* trans = mScheduledEvents.begin()->second;
      mScheduledEvents.erase(mScheduledEvents.begin());
      return trans;
    }

    mEvent.notify(mScheduledEvents.begin()->first - now);
    
    return 0;
  }

  sc_event& getEvent()
  {
    return mEvent;
  }

private:
  std::multimap<sc_time, transaction_type*> mScheduledEvents;
  sc_event mEvent;
};


#endif
