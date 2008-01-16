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
/*******************************************************************************
 * @file
 * rw_bus.h
 * $Id:$
 * @brief
 * Simple bus exposing rw interface sockets.
 * @Details
 * The bus routes transactions depending on their address.
 * Routing is generic code, the interface-dependent part regards
 * the type of the socket exposed, which could be passed as template
 * parameters.
 * 
 ********************************************************************************/

#ifndef __RW_BUS_H__
#define __RW_BUS_H__

#include "systemc.h"
#include "tlm.h"
#include "rw_initiator_socket.h"
#include "rw_target_socket.h"

template <int NR_OF_INITIATORS, int NR_OF_TARGETS>
class rw_bus : public sc_module,
               virtual public tlm::tlm_fw_b_transport_if<>,
               virtual public tlm::tlm_bw_b_transport_if
{
public:
  rw_target_socket      *target_socket[NR_OF_TARGETS];
  rw_initiator_socket   *initiator_socket[NR_OF_INITIATORS];

  rw_bus(sc_module_name name);
  ~rw_bus();
  
  // Forward interfaces
  virtual void b_transport(tlm::tlm_generic_payload & trans);
  
  unsigned int transport_dbg(tlm::tlm_debug_payload & payload);

  bool get_direct_mem_ptr(const sc_dt::uint64  & address, 
  		                  tlm::tlm_dmi_mode    & dmi_mode,
                          tlm::tlm_dmi         & dmi_data);

  // Backward interface
  virtual void invalidate_direct_mem_ptr(sc_dt::uint64 start_range,
                                         sc_dt::uint64 end_range);
  
private:
  unsigned int  getSocketId(const sc_dt::uint64& address);
  sc_dt::uint64 getAddressOffset(unsigned int socketId);
  sc_dt::uint64 getAddressMask(unsigned int socketId);
  void          limitRange(unsigned int    socketId, 
  		                   sc_dt::uint64 & low, 
  		                   sc_dt::uint64 & high);
};

#include "../src/rw_bus.tpp"

#endif
