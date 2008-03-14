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
  @file at_target_4_phase.h
    
  @brief Approximately Timed (AT) TLM memory target

  @Details
    This class provides a tlm_slave_socket for the initiator to bind.
    It implements the tlm_fw_nb_transport_if to provide master to slave
    communications.

  @Note

===============================================================================*/
/*****************************************************************************
  Original Authors:
    Bill Bunton, ESLX
    Anna Keist, ESLX
*****************************************************************************/

#ifndef __AT_TARGET_4_PHASE_H__ 
#define __AT_TARGET_4_PHASE_H__

#include "tlm.h"                                      ///< TLM headers
#include <queue>                                      ///< queue header from std lib

//==============================================================================

class at_target_4_phase                         ///< LT target memory
:         public sc_core::sc_module             ///< module base clase
, virtual public tlm::tlm_fw_nb_transport_if<>  ///< target socket
{
  SC_HAS_PROCESS(at_target_4_phase);

//==============================================================================
// constructors / destructor 
//==============================================================================
public:
  at_target_4_phase                                 ///< constructor
  ( sc_core::sc_module_name   module_name           ///< module name
  , const unsigned int        ID                    ///< target ID
  , const char                *memory_socket        ///< socket name
  , sc_dt::uint64             base_address          ///< base address
  , sc_dt::uint64             memory_size           ///< memory size (bytes)
  , unsigned int              memory_width          ///< memory width (bytes)
  , const sc_core::sc_time    accept_delay          ///< accept delay
  , const sc_core::sc_time    read_response_delay   ///< read response delay
  , const sc_core::sc_time    write_response_delay  ///< write response delay
  );
    
  ~at_target_4_phase(void);                         ///< destructor
  
//==============================================================================
// member functions 
//==============================================================================
  public:
  tlm::tlm_sync_enum                         ///< sync status
  nb_transport                               ///< non-blocking transport
  (
     tlm::tlm_generic_payload &gp            ///< generic payoad pointer
  ,  tlm::tlm_phase           &phase         ///< transaction phase
  ,  sc_core::sc_time         &delay_time    ///< time it should take for transport
  );

//------------------------------------------------------------------------------
// memory operations implements RAM read and write operations 
//------------------------------------------------------------------------------
  private:
  void memory_operation                      ///< memory operation 
  (
    tlm::tlm_generic_payload  &gp            ///< generic payload
  );

//------------------------------------------------------------------------------
// Method 
//------------------------------------------------------------------------------
  private:
  void end_request_method(void);              
  void begin_response_method(void);           


//------------------------------------------------------------------------------
// transport_dbg() not implmented
  
  unsigned int                                ///< result
  transport_dbg                               ///< transport debug
  (
    tlm::tlm_generic_payload  &gp            ///< generic payload
  );

//------------------------------------------------------------------------------
// get_direct_mem_ptr not implmented
  
  bool                                        ///< success / failure
  get_direct_mem_ptr                          ///< get direct memory pointer
  (
    tlm::tlm_generic_payload   &payload,      ///< address + extensions
    tlm::tlm_dmi               &dmi_data      ///< dmi data
  );


//==============================================================================
// member variables 
//==============================================================================
  
  public:
  const unsigned int        m_ID;              ///< target ID

  tlm::tlm_target_socket<>  m_memory_socket;   ///< target socket

  private:
  sc_dt::uint64           m_base_address;      ///< base address
  sc_dt::uint64           m_memory_size;       ///< memory size
  unsigned int            m_memory_width;      ///< word size
  unsigned char           *m_memory;           ///< memory

  const sc_core::sc_time  m_accept_delay;         ///< accept request delay
  const sc_core::sc_time  m_read_response_delay;  ///< read response delay
  const sc_core::sc_time  m_write_response_delay; ///< write response delay

  std::queue<tlm::tlm_generic_payload*>   m_end_request_queue; ///< end request queue
  sc_core::sc_event                       m_end_request_event; ///< end request event

  std::queue<tlm::tlm_generic_payload*>   m_response_queue;    ///< response queue
  sc_core::sc_event                       m_begin_response_event; ///< begin response event

  std::queue<tlm::tlm_generic_payload*>   m_end_response_queue; ///< end response queue
  sc_core::sc_event                       m_end_response_event; ///< end resonse event

  bool begin_response_QActive;            ///< begin response queue active state
  bool end_request_QActive;               ///< end request queue active state

}; // end class at_target_4_phase

#endif /* __AT_TARGET_4_PHASE_H__ */
