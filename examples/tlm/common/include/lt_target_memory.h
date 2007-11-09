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
#ifndef __LT_TARGET_MEMORY_H__ 
#define __LT_TARGET_MEMORY_H__
/*==============================================================================
  @file lt_target_memory.h
  
  $Id: lt_target_memory.h 189 2007-11-02 23:51:20Z wilsoncj $ 
  
  @brief Loosely Timed (LT) TLM memory target

  @Details
    This class provides a tlm_slave_socket for the initiator to bind.
    It implements the tlm_fw_nb_transport_if to provide master to slave
    communications.

  @Note
    1. Out of range write request complete normally and do not change memory
       state 
    2. Out of range read request return zero, complete normally and do not change 
       memory state.  

===============================================================================*/

class lt_target_memory                                  ///< LT target memory
  :         public sc_core::sc_module                   ///< module base clase
  , virtual public tlm::tlm_fw_nb_transport_if<>        ///< target socket
{
  //==============================================================================
  // member variables 
  //==============================================================================
  
  public:
  tlm::tlm_target_socket<>  m_memory_socket;   ///< target socket

  private:
  sc_dt::uint64            m_base_address;    ///< base address
  sc_dt::uint64            m_memory_size;     ///< memory size
  unsigned int             m_memory_width;    ///< word size
  unsigned char            *m_memory;         ///< memory
  const sc_core::sc_time   m_read_delay;      ///< read delay
  const sc_core::sc_time   m_write_delay;     ///< write delay
  const sc_core::sc_time   m_refresh_delay;   ///< refresh delay
  const sc_core::sc_time   m_refresh_rate;    ///< frequency of refresh

  //==============================================================================
  // constructors / destructor 
  //==============================================================================

  private:
  lt_target_memory (void);                    ///< disabled default constructor 

  public:
  lt_target_memory                            ///< constructor
  (   sc_core::sc_module_name  module_name    ///< module name 
    , sc_dt::uint64            base_address   ///< memory base address 64-bit
    , sc_dt::uint64            memory_size    ///< memory size 64-bit
    , unsigned int             memory_width   ///< memory width (bytes)(2, 4, 8, 16)
    , sc_core::sc_time         clock_period   ///< clock period for delays
    , unsigned int             read_clocks    ///< number of clocks for read
    , unsigned int             write_clocks   ///< number of clocks for write
    , unsigned int             refresh_clocks ///< number of clocks for refresh
    , unsigned int             refresh_rate   ///< frequency of refresh
  );
  
  ~lt_target_memory(void);                    ///< destructor
  
  //==============================================================================
  // member functions 
  //==============================================================================
  
  public:
  tlm::tlm_sync_enum                          ///< sync status
  nb_transport(                               ///< non-blocking transport
      tlm::tlm_generic_payload &gp            ///< generic payoad pointer
    , tlm::tlm_phase           &phase         ///< transaction phase
    , sc_core::sc_time         &delay_time    ///< time it should take for transport
  );
  
  /// @note transport_dbg() not implmented
  
  unsigned int                                ///< result
  transport_dbg(                              ///< transport debug
    tlm::tlm_debug_payload     &payload       ///< debug payload
  );
  
  /// @note transport_dbg() not implmented
  
  bool                                        ///< success / failure
  get_direct_mem_ptr(                         ///< get direct memory pointer
    const sc_dt::uint64        &address,      ///< address
    bool                       for_reads,     ///< for reads
    tlm::tlm_dmi               &dmi_data      ///< dmi data
  );

}; // end class lt_target_memory

#endif /* __LT_TARGET_MEMORY_H__ */
