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

#ifndef __TLM_TARGET_H__
#define __TLM_TARGET_H__

#include "tlm.h"
#include "simple_target_socket.h"


class tlm_target : public sc_core::sc_module
{
public:

    SimpleTargetSocket< 32 > socket; 

    SC_HAS_PROCESS(tlm_target);
    
    // constructor & destructor
    tlm_target(sc_core::sc_module_name name_, 
               tlm::tlm_endianness endianness = tlm::TLM_LITTLE_ENDIAN);
    
    ~tlm_target() { }
    
    // interface methods
    tlm::tlm_sync_enum myNBTransport(tlm::tlm_generic_payload& trans, tlm::tlm_phase& phase, sc_core::sc_time& t);

private:

    unsigned int        mem[2];
    unsigned int        m_socket_width;
    tlm::tlm_endianness m_endianness;

}; 

#endif
