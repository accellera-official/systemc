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

#include "tlm_gp_export.h"


class tlm_target
    : public sc_core::sc_module
    , virtual public tlm::nb_transport_if<tlm::tlm_generic_payload>
{
public:

    tlm::tlm_gp_export< 32 > bus_port; // bus data width in bits

    SC_HAS_PROCESS(tlm_target);
    
    // constructor & destructor
    tlm_target(sc_core::sc_module_name name_, 
               tlm::tlm_endianness endianness = tlm::TLM_LITTLE_ENDIAN);
    
    ~tlm_target() { }
    
    // interface methods
    void nb_transport(tlm::tlm_generic_payload* gp);
    unsigned int transport_dbg(tlm::tlm_generic_payload* gp) {return 0;}

    
private:

    unsigned int        mem[2];
    unsigned int        m_socket_width;
    tlm::tlm_endianness m_endianness;

}; 

#endif
