/*****************************************************************************
 *
 *   The following code is derived, directly or indirectly, from the SystemC
 *   source code Copyright (c) 1996-2008 by all Contributors.
 *   All Rights reserved.
 *
 *   The contents of this file are subject to the restrictions and limitations
 *   set forth in the SystemC Open Source License Version 3.0 (the "License");
 *   You may not use this file except in compliance with such restrictions and
 *   limitations. You may obtain instructions on how to receive a copy of the
 *   License at http://www.systemc.org/. Software distributed by Contributors
 *   under the License is distributed on an "AS IS" basis, WITHOUT WARRANTY OF
 *   ANY KIND, either express or implied. See the License for the specific
 *   language governing rights and limitations under the License.
 *
 *   *****************************************************************************/

/* ---------------------------------------------------------------------------------------
 @file lt_target.h
 
 @brief lt_target class header
 
  Original Authors:
    Bill Bunton, ESLX
    Charles Wilson, ESLX
    
--------------------------------------------------------------------------------------- */

#ifndef __LT_TARGET_H__ 
#define __LT_TARGET_H__

#include <queue>                                    ///< standard queue

enum Model_Style_emum
{ LT_Annotated_e                                    ///< annotated  
, LT_Sync_e                                         ///< synchronized
};

class lt_target                                     ///< lt_target
:         public sc_core::sc_module                 ///< module base clase
, virtual public tlm::tlm_fw_nb_transport_if<>      ///< target socket interface
{
  SC_HAS_PROCESS(lt_target);

  // constructors / destructor 

  public:
  
  lt_target                                         ///< constructor
  ( sc_core::sc_module_name   module_name           ///< module name
  , const unsigned int        ID                    ///< target ID
  , const tlm::tlm_endianness endianness            ///< target endianness
  , const char                *memory_socket        ///< socket name
  , sc_dt::uint64             base_address          ///< base address
  , sc_dt::uint64             memory_size           ///< memory size (bytes)
  , unsigned int              memory_width          ///< memory width (bytes)
  , const sc_core::sc_time    accept_delay          ///< accept delay (SC_TIME)
  , const sc_core::sc_time    read_response_delay   ///< read response delay (SC_TIME)
  , const sc_core::sc_time    write_response_delay  ///< write response delay (SC_TIME)
  );
    
  ~lt_target                                        ///<  destructor
  ( void
  );
  
  // member functions 

  public:
  
  tlm::tlm_sync_enum                                ///< sync status
  nb_transport                                      ///< non-blocking transport
  ( tlm::tlm_generic_payload &gp                    ///< generic payoad pointer
  , tlm::tlm_phase           &phase                 ///< transaction phase
  , sc_core::sc_time         &delay_time            ///< time it should take for transport
  );

  // memory operations (read/write) 

  private:
  
  void
  memory_operation                                  ///< memory_operation
  ( tlm::tlm_generic_payload  &gp                   ///< generic payload
  );

  // Method 

  private:

  void
  begin_response                                    ///< begin_response
  ( void
  );

  // transport_dbg() not implmented
  
  unsigned int                                      ///< result
  transport_dbg                                     ///< transport debug
  ( tlm::tlm_debug_payload  &payload                ///< debug payload
  );

  // get_direct_mem_ptr() not implmented
  
  bool                                              ///< success / failure
  get_direct_mem_ptr                                ///< get direct memory pointer
  ( const sc_dt::uint64        &address             ///< address
  , tlm::tlm_dmi_mode          &dmi_mode            ///< DMI read/write mode
  , tlm::tlm_dmi               &dmi_data            ///< DMI data
  );

  // member variables
   
  typedef tlm::tlm_generic_payload  *gp_ptr;

  private:
  
  const unsigned int        m_ID;                   ///< target ID
  const tlm::tlm_endianness m_endianness;           ///< target endianness
  
  public:
  
  tlm::tlm_target_socket<>  m_memory_socket;        ///<  target socket

  private:
  
        sc_dt::uint64       m_base_address;         ///< base address
        sc_dt::uint64       m_memory_size;          ///< memory size (bytes)
        unsigned int        m_memory_width;         ///< word size (bytes)
  const sc_core::sc_time    m_accept_delay;         ///< accept delay
  const sc_core::sc_time    m_read_response_delay;  ///< read response delay
  const sc_core::sc_time    m_write_response_delay; ///< write response delays
        bool                m_DMI_granted;          ///< DMI pointer granted

        unsigned char       *m_memory;              ///< memory
        tlm::tlm_endianness m_host_endianness;      ///< host endianness (platform)
  
        std::queue<gp_ptr>  m_response_queue;       ///< response queue
        sc_core::sc_event   m_begin_response_event; ///< response event
};

#endif /* __LT_TARGET_H__ */
