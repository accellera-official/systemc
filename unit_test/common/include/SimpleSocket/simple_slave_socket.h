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

#ifndef SIMPLE_SLAVE_SOCKET_H
#define SIMPLE_SLAVE_SOCKET_H

#include "simple_socket_utils.h"
//#include "tlm.h"

template <typename TRANS>
class SimpleSlaveSocket :
  public tlm_ports::tlm_slave_socket<tlm::tlm_nonblocking_transport_if<TRANS> >
{
public:
  typedef TRANS transaction_type;
  typedef tlm::tlm_nonblocking_transport_if<transaction_type> interface_type;
  typedef tlm_ports::tlm_slave_socket<interface_type> base_type;
  typedef tlm::tlm_phase phase_type;

private:
  typedef simple_socket_utils::Process<transaction_type> Process;

public:
  explicit SimpleSlaveSocket(const char* name = "") :
    base_type(name),
    mProcess(0)
  {
  }

  // REGISTER_SOCKETPROCESS
  template <typename MODULE>
  void CB(MODULE* mod, bool (MODULE::*cb)(transaction_type&, phase_type&, sc_time&), int id)
  {
    delete mProcess;
    mProcess = new Process(mod, static_cast<typename Process::MethodPtr>(cb));
    mProcess->setUserId(id);
    (*this)(*mProcess);
  }

private:
  Process* mProcess;
};

#endif
