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
/*******************************************************************************
 * @file
 * rw_initiator_socket.h
 * $Id:$
 * @brief
 * Initiator socket corresponding to the rw interface.
 * @Details
 * Initiator socket corresponding to the rw interface.
 * This socket is used by components to initiate transactions.
 * 
 ********************************************************************************/
#ifndef __RW_INITIATOR_SOCKET_H__
#define __RW_INITIATOR_SOCKET_H__

#include "systemc.h"
#include "tlm.h"
#include "rw_if.h"

class rw_initiator_socket : public tlm::tlm_initiator_socket<64,
                                                             tlm::tlm_fw_transport_if<>,
                                                             tlm::tlm_bw_transport_if>,
						    public virtual rw_if
{
  public:
	rw_initiator_socket(const char * name);
	
    virtual bool read(
        sc_dt::uint64     bus_address     ///< bus address
      , unsigned int      byte_count      ///< data transfer size
      , unsigned char*    read_buffer     ///< pointer to read buffer
      );

    virtual bool write(
        sc_dt::uint64     bus_address     ///< bus address
      , unsigned int      byte_count      ///< data transfer size
      , unsigned char*    write_buffer    ///< pointer to write buffer
       ); 
};

#endif /* __RW_INITIATOR_SOCKET_H__ */
