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
/*==============================================================================
  @file adapter_tlm_2_1_sync.h
  
  $Id:  $ 
  
  @brief TLM 2 to TLM 1 loosely timed with synchronization 

  @Details
  This module implements two interfaces a TLM 2 target socket and a TLM 1 master 
  port. The TLM 2 socket uses the generic payload and the TLM 1 port implements 
  the base TLM 1 protocol from the TLM 1 kit. 

  Request received from the TLM2 socket are buffered in a FIFO and TLM_ACCECPTED 
  status is returned. An event is used to schedule a thread process to translate 
  an issue the buffered request to the TLM1 port. The use of the thread process 
  allows the connected TLM1 slave to block.  

  When the TLM 1 transaction completes the thread process makes a call to the 
  backward nb_transport interface of the target socket to return complete status 
  and the generic payload. 
    
===============================================================================*/

/*****************************************************************************
  Original Authors:
    Bill Bunton, ESLX
    Charles Wilson, ESLX
*****************************************************************************/

#ifndef __ADAPTER_TLM_2_1_SYNC_H__ 
#define __ADAPTER_TLM_2_1_SYNC_H__

#include "basic_initiator_port.h"
#include <queue>

typedef unsigned int ADDRESS_TYPE;
typedef unsigned int DATA_TYPE;

using basic_protocol::basic_initiator_port;


class adapter_tlm_2_1_sync                            ///< LT target memory
  :         public sc_core::sc_module                 ///< module base clase
  , virtual public tlm::tlm_fw_nb_transport_if<>      ///< target socket
{
   SC_HAS_PROCESS( adapter_tlm_2_1_sync );

//==============================================================================
// sockets and ports 
//==============================================================================
  public:
  tlm::tlm_target_socket<>  adapter_socket;           ///< TLM 2 target socket

  basic_initiator_port<ADDRESS_TYPE,DATA_TYPE>        /// tlm 1 initiator port 
    tlm_1_initiator_port;                     

//==============================================================================
// constructors / destructor 
//==============================================================================
public:
  adapter_tlm_2_1_sync (                      ///< constructor
    sc_core::sc_module_name name              ///< name 
    );
  
  ~adapter_tlm_2_1_sync(void);                ///< destructor

//==============================================================================
// TLM2 non blocking membor function 
//==============================================================================
  public:
  tlm::tlm_sync_enum                          ///< sync status
  nb_transport(                               ///< non-blocking transport
      tlm::tlm_generic_payload &gp            ///< generic payoad pointer
    , tlm::tlm_phase           &phase         ///< transaction phase
    , sc_core::sc_time         &delay_time    ///< time it should take for transport
  );

//-----------------------------------------------------------------------------
/// @note transport_dbg() not implmented
//-----------------------------------------------------------------------------
  unsigned int                                ///< result
  transport_dbg(                              ///< transport debug
    tlm::tlm_generic_payload   &payload       ///< generic payload
  );

//-----------------------------------------------------------------------------  
/// @note transport_dbg() not implmented
//-----------------------------------------------------------------------------  
  bool                                        ///< success / failure
  get_direct_mem_ptr(                         ///< get direct memory pointer
    tlm::tlm_generic_payload   &payload,      ///< address + extensions
    tlm::tlm_dmi               &dmi_data      ///< dmi data
  );


//==============================================================================
// Private member variables and methods
//==============================================================================
  std::queue<tlm::tlm_generic_payload *> m_new_request_queue;
  sc_core::sc_event m_new_request_event;

  void tlm_1_requestor_thread(void);
 
}; 

#endif /* __ADAPTER_TLM_2_1_SYNC_H__ */
