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

/* ---------------------------------------------------------------------------------------
 @file lt_target_memory.h
 
 @brief lt_target_memory class header
 
  Original Authors:
    Charles Wilson, ESLX
    Bill Bunton, ESLX
    
--------------------------------------------------------------------------------------- */

#ifndef __LT_TARGET_MEMORY_H__ 
#define __LT_TARGET_MEMORY_H__

#include "tlm.h"                                      ///< TLM headers

class lt_target_memory                                  ///< LT target memory
  :         public sc_core::sc_module                   ///< module base clase
  , virtual public tlm::tlm_fw_nb_transport_if<>        ///< target socket
{
  public:
  tlm::tlm_target_socket<>  m_memory_socket;          ///< target socket

  private:
  const unsigned int       m_ID;                      ///< initiator ID
  sc_dt::uint64            m_base_address;            ///< base address
  sc_dt::uint64            m_memory_size;             ///< memory size
  unsigned int             m_memory_width;            ///< word size
  unsigned char            *m_memory;                 ///< memory
  const sc_core::sc_time   m_read_delay;              ///< read delay
  const sc_core::sc_time   m_write_delay;             ///< write delay
  const sc_core::sc_time   m_refresh_delay;           ///< refresh delay
  const sc_core::sc_time   m_refresh_rate;            ///< frequency of refresh

  // constructors / destructor 

  private:
  
  lt_target_memory                                    ///< disabled default constructor
  ( void
  ); 

  public:
  
  lt_target_memory                                    ///< constructor
  ( sc_core::sc_module_name module_name               ///< module name 
  , const unsigned int      ID                        ///< initiator ID
  , const sc_dt::uint64     base_address              ///< memory base address 64-bit
  , const sc_dt::uint64     memory_size               ///< memory size 64-bit
  , const unsigned int      memory_width              ///< memory width (bytes)(2, 4, 8, 16)
  , const sc_core::sc_time  clock_period              ///< clock period for delays
  , const unsigned int      read_clocks               ///< number of clocks for read
  , const unsigned int      write_clocks              ///< number of clocks for write
  , const unsigned int      refresh_clocks            ///< number of clocks for refresh
  , const unsigned int      refresh_rate              ///< frequency of refresh
  );
  
  ~lt_target_memory                                   ///< destructor
  ( void
  );
  
  // member functions 
  
  public:
  tlm::tlm_sync_enum                          ///< sync status
  nb_transport(                               ///< non-blocking transport
      tlm::tlm_generic_payload &gp            ///< generic payoad pointer
    , tlm::tlm_phase           &phase         ///< transaction phase
    , sc_core::sc_time         &delay_time    ///< time it should take for transport
  );
  
  unsigned int                                ///< result
  transport_dbg(                              ///< transport debug
    tlm::tlm_generic_payload   &payload       ///< debug payload
  );
  
  bool                                        ///< success / failure
  get_direct_mem_ptr(                         ///< get direct memory pointer
    tlm::tlm_generic_payload   &payload,      ///< address + extensions
    tlm::tlm_dmi               &dmi_data      ///< dmi data
  );
};

#endif /* __LT_TARGET_MEMORY_H__ */
