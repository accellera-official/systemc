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
#ifndef __SIMPLE_MEMORY_H__ 
#define __SIMPLE_MEMORY_H__
/*==============================================================================
  @file simple_memory.h
  
  $Id:$ 
  
  @brief Simple TLM memory target

  @Details
    This class provides a target socket for the initiator to bind.
    It implements the example "rw_if" interface to provide master to slave
    communications.

  @Note
    1. Out of range write request complete normally and do not change memory
       state 
    2. Out of range read request return zero, complete normally and do not change 
       memory state.  

===============================================================================*/

#include "systemc.h"
#include "rw_target_socket.h"
#include "rw_slave_base.h"

class simple_memory : public sc_module,
                      virtual public rw_slave_base
{
public:
  rw_target_socket                        target_socket;

  
  simple_memory(sc_module_name  module_name,
		        sc_dt::uint64   memory_size);
  
  ~simple_memory();
  
  // Convenience API
  virtual bool read(sc_dt::uint64     bus_address,
                    unsigned int      byte_count,
                    unsigned char*    read_buffer);

  virtual bool write(sc_dt::uint64     bus_address,
		             unsigned int      byte_count,
                     unsigned char*    write_buffer);
  
  // Not implemented for now

  // debug interface
  unsigned int transport_dbg(tlm::tlm_generic_payload & payload);

  // DMI interface
  bool get_direct_mem_ptr(tlm::tlm_generic_payload &payload,
                          tlm::tlm_dmi         & dmi_data);

private:
  sc_dt::uint64            m_memory_size;
  unsigned char           *m_memory;
};

#endif /* __SIMPLE_MEMORY_H__ */
