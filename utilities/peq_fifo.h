/*****************************************************************************

  The following code is derived, directly or indirectly, from the SystemC
  source code Copyright (c) 1996-2007 by all Contributors.
  All Rights reserved.

  The contents of this file are subject to the restrictions and limitations
  set forth in the SystemC Open Source License Version 3.0 (the "License");
  You may not use this file except in compliance with such restrictions and
  limitations. You may obtain instructions on how to receive a copy of the
  License at http://www.systemc.org/. Software distributed by Contributors
  under the License is distributed on an "AS IS" basis, WITHOUT WARRANTY OF
  ANY KIND, either express or implied. See the License for the specific
  language governing rights and limitations under the License.

 *****************************************************************************/

#ifndef __PEQ_FIFO_H__
#define __PEQ_FIFO_H__

//#include "tlm.h"
#include <systemc>
#include <map>

template <class PAYLOAD>
class peq_fifo : public sc_core::sc_module
{
public:
  typedef PAYLOAD transaction_type;
  typedef std::pair<const sc_core::sc_time, transaction_type*> pair_type;

public:
  SC_HAS_PROCESS(peq_fifo);
  peq_fifo(sc_core::sc_module_name name) : sc_core::sc_module(name)
  {
  }

  void notify(transaction_type& trans, sc_core::sc_time& t)
  {
    mScheduledEvents.insert(pair_type(t + sc_core::sc_time_stamp(), &trans));
    mEvent.notify(t);
  }

  // needs to be called until it returns 0
  transaction_type* get_next_transaction()
  {
    if (mScheduledEvents.empty()) {
      return 0;
    }

    sc_core::sc_time now = sc_core::sc_time_stamp();
    if (mScheduledEvents.begin()->first <= now) {
      transaction_type* trans = mScheduledEvents.begin()->second;
      mScheduledEvents.erase(mScheduledEvents.begin());
      return trans;
    }

    mEvent.notify(mScheduledEvents.begin()->first - now);
    
    return 0;
  }

  sc_core::sc_event& get_event()
  {
    return mEvent;
  }

private:
  std::multimap<const sc_core::sc_time, transaction_type*> mScheduledEvents;
  sc_core::sc_event mEvent;
};


#endif
