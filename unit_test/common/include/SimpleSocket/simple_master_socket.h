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

#ifndef SIMPLE_MASTER_SOCKET_H
#define SIMPLE_MASTER_SOCKET_H

#include "simple_socket_utils.h"
//#include "tlm.h"

template <typename TRANS>
class SimpleMasterSocket :
  public tlm_ports::tlm_master_socket<tlm::tlm_nonblocking_transport_if<TRANS> >
{
public:
  typedef TRANS transaction_type;
  typedef tlm::tlm_nonblocking_transport_if<transaction_type> interface_type;
  typedef tlm_ports::tlm_master_socket<interface_type> base_type;
  typedef tlm::tlm_phase phase_type;

private:
  typedef simple_socket_utils::Process<transaction_type> Process;

public:
  explicit SimpleMasterSocket(const char* name = "") :
    base_type(name),
    mProcess(mEndEvent)
  {
    mExport(mProcess);
  }

  sc_event& getEndEvent()
  {
    return mEndEvent;
  }

  // REGISTER_SOCKETPROCESS
  template <typename MODULE>
  void CB(MODULE* mod, bool (MODULE::*cb)(transaction_type&, phase_type&, sc_time&), int id)
  {
    mProcess.setCallBack(mod, static_cast<typename Process::MethodPtr>(cb));
    mProcess.setUserId(id);
  }

private:
  class MyProcess : public Process {
  public:
    typedef typename Process::MethodPtr MethodPtr;

  public:
    MyProcess(sc_event& endEvent) : Process(0, 0), mEndEvent(endEvent)
    {
    }

    void setCallBack(sc_module* mod, MethodPtr f)
    {
      this->mF = f;
      this->mMod = mod;
    }

    bool nb_transport(transaction_type& trans, phase_type& phase, sc_time& t)
    {
      if (this->mMod) {
        // REGISTER_SOCKETPROCESS was called, forward call
        return Process::nb_transport(trans, phase, t);

      } else {
        // No callback registered, handle protocol and notify event
        switch (phase) {
        case tlm::END_REQ:
          // Request phase ended
          return false;
        case tlm::BEGIN_RESP:
          assert(t == SC_ZERO_TIME); // FIXME: can t != 0?    
          mEndEvent.notify(t);
          // Not needed to update the phase if true is returned
          return true;
        case tlm::BEGIN_REQ: // fall-through
        case tlm::END_RESP: // fall-through
        default:
          // A slave should never call nb_transport with these phases
          assert(0); exit(1);
          return false;
        }
        return false;
      }
    }
  private:
    sc_event& mEndEvent;
  };

private:
  MyProcess mProcess;
  sc_event mEndEvent;
};

#endif
