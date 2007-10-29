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

#ifndef _TLM_SLAVE_H
#define _TLM_SLAVE_H

#include "systemc.h"

#include "tlm.h"

#include "tlm_gp_export.h"
#include "tlm_memory.h"
#include "tlm_checkers.h"


class tlm_slave
	: public sc_module
	, virtual public tlm::nb_transport_if<tlm::tlm_generic_payload>
{
public:

	tlm::tlm_gp_export< 256 > bus_port; // bus data width in bits

    SC_HAS_PROCESS(tlm_slave);
    
	// constructor & destructor
	tlm_slave(sc_module_name _name, unsigned int start_address, unsigned int end_address);
	~tlm_slave() { }
    
    // interface methods
    void nb_transport(tlm::tlm_generic_payload* gp);
    unsigned int transport_dbg(tlm::tlm_generic_payload* gp) {return 0;}

    
private:

    sc_biguint<256>     reg256;
    sc_biguint<128>     reg128;
    unsigned long long  reg64;
    sc_uint<48>         reg48;
    unsigned int        reg32;
    unsigned short      reg16;
    unsigned char       reg8;

    unsigned int m_start_address;
    unsigned int m_end_address;
    tlm_memory m_mem;
    unsigned int m_mem_size;
    unsigned int m_incr_address;

	tlm::tlm_checker m_checker;

}; 

#endif
