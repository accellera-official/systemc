/*****************************************************************************

  The following code is derived, directly or indirectly, from the SystemC
  source code Copyright (c) 1996-2008 by all Contributors.
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
  @file lt_at_switch_target.h
    
  @brief Loosely Timed (LT) to Approximately Timed (AT) TLM memory target

  @Details
    This target is initiatlized as an LT target.  It is designed to switch
    to an AT target when triggered by tlm command TLM_IGNORE_COMMAND combined
    with ignorable extensions.

  @Note

===============================================================================*/
/*****************************************************************************
  Original Authors:
    Bill Bunton, ESLX
    Anna Keist, ESLX
*****************************************************************************/

#ifndef __LT_AT_SWITCH_TARGET_H__ 
#define __LT_AT_SWITCH_TARGET_H__

#include "tlm.h"                                      ///< TLM headers
#include <queue>                                      ///< queue header from std lib

enum e_target_mode                        ///< Enumerated model state; in LT or AT
{
  LT_Annotated          	          ///< LT annotated model
, LT_Sync                                 ///< LT sync model
, AT_4_Phase                 		  ///< AT 4 phase model             
};

//==============================================================================
class lt_at_switch_target                       ///< LT target memory
:         public sc_core::sc_module             ///< module base clase
, virtual public tlm::tlm_fw_transport_if<>     ///< target socket
{
  SC_HAS_PROCESS(lt_at_switch_target);

//==============================================================================
// constructors / destructor 
//==============================================================================
public:
  lt_at_switch_target                               ///< constructor
  ( sc_core::sc_module_name   module_name           ///< module name
  , const unsigned int        ID                    ///< target ID
  , const char                *memory_socket        ///< socket name
  , sc_dt::uint64             base_address          ///< base address
  , sc_dt::uint64             memory_size           ///< memory size (bytes)
  , unsigned int              memory_width          ///< memory width (bytes)
  , const sc_core::sc_time    accept_delay          ///< accept delay
  , const sc_core::sc_time    read_response_delay   ///< read response delay
  , const sc_core::sc_time    write_response_delay  ///< write response delay
  , const sc_core::sc_time    LT2AT_trigger_time    ///< time to switch LT2AT
  , const sc_core::sc_time    AT2LT_trigger_time    ///< time to switch AT2LT
  );
    
  ~lt_at_switch_target(void);                        ///< destructor
  
//==============================================================================
// member functions 
//==============================================================================
  public:
  tlm::tlm_sync_enum                         ///< sync status
  nb_transport                               ///< non-blocking transport
  (
     tlm::tlm_generic_payload& transaction   ///< generic payoad pointer
  ,  tlm::tlm_phase&           phase         ///< transaction phase
  ,  sc_core::sc_time&         time          ///< time it should take for transport
  );

//------------------------------------------------------------------------------
// memory operations implements RAM read and write operations 
//------------------------------------------------------------------------------
  private:
  void memory_operation                      ///< memory operation 
  (
    tlm::tlm_generic_payload&  gp            ///< generic payload
  );

//------------------------------------------------------------------------------
// Methods and Threads 
//------------------------------------------------------------------------------
  private:
  void end_request_method(void);              
  void begin_response_method(void);           
  void LT2AT_switch_stimulus(void);
  void lt_begin_response_method(void);

//------------------------------------------------------------------------------
// transport_dbg() not implmented
  
  unsigned int                                ///< result
  transport_dbg                               ///< transport debug
  (
    tlm::tlm_generic_payload   &payload       ///< debug payload
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
  private:
  const unsigned int      m_ID;              ///< target ID
  
  public:
  tlm::tlm_target_socket<>  m_memory_socket;   ///< target socket

 private:
  sc_dt::uint64           m_base_address;      ///< base address
  sc_dt::uint64           m_memory_size;       ///< memory size
  unsigned int            m_memory_width;      ///< word size
  unsigned char           *m_memory;           ///< memory

  const sc_core::sc_time  m_accept_delay;         ///< accept request delay
  const sc_core::sc_time  m_read_response_delay;  ///< read response delay
  const sc_core::sc_time  m_write_response_delay; ///< write response delay
  const sc_core::sc_time  m_LT2AT_trigger_time;   ///< time to switch target mode to AT
  const sc_core::sc_time  m_AT2LT_trigger_time;   ///< time to switch target back to LT

  e_target_mode      m_LT_AT_mode;        ///< model style (2 or 4 phase)

  std::queue<tlm::tlm_generic_payload*>   m_end_request_queue; ///< end request queue
  sc_core::sc_event                       m_end_request_event; ///< end request event
  std::queue<tlm::tlm_generic_payload*>   m_response_queue;    ///< response queue
  sc_core::sc_event                       m_begin_response_event; ///< begin response event
  std::queue<tlm::tlm_generic_payload*>   m_end_response_queue; ///< end response queue
  sc_core::sc_event                       m_end_response_event; ///< end resonse event
  sc_core::sc_event                       m_lt_sync_event; ///< begin lt sync event
  sc_core::sc_event                       m_end_lt_sync_event;

  bool m_begin_response_QActive;            ///< begin response queue active state
  bool m_end_request_QActive;               ///< end request queue active state
  bool m_AT2LT_switch_active;               ///< end request queue active state

}; // end class lt_at_switch_target

#endif /* __LT_AT_SWITCH_TARGET_H__ */
