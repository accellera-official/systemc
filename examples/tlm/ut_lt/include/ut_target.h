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

/* ---------------------------------------------------------------------------------------
 @file ut_target.h
 
 @brief UT target class header
 
  Original Authors:
    Charles Wilson, ESLX
    
--------------------------------------------------------------------------------------- */

#ifndef __UT_TARGET_H__
#define __UT_TARGET_H__

#include "tlm.h"                                      ///< TLM headers

class ut_target                                       ///< UT target class
:         public sc_core::sc_module                   ///< SC module
, virtual public tlm::tlm_fw_b_transport_if<>         ///< forward transport interface
{
  public:
  
  typedef tlm::tlm_fw_b_transport_if<>  fw_interface_type;
  typedef tlm::tlm_bw_b_transport_if    bw_interface_type;
  typedef tlm::tlm_target_socket<32,
                                 fw_interface_type,
                                 bw_interface_type>
                                        target_socket_type;

  public:
  
  target_socket_type socket;                          ///< socket

  public:
  
  ut_target                                           ///< constructor
  ( sc_core::sc_module_name name                      ///< module name
  , const unsigned int      ID                        ///< target ID
  , const sc_dt::uint64     base_address              ///< memory base address 64-bit
  , const sc_dt::uint64     memory_size               ///< memory size 64-bit
  , const unsigned int      memory_width              ///< memory width (bytes)(2, 4, 8, 16)
  , const sc_core::sc_time  clock_period              ///< clock period for delays
  , const unsigned int      read_clocks               ///< number of clocks for read
  , const unsigned int      write_clocks              ///< number of clocks for write
  );
  
  ~ut_target                                          ///< destructor
  ( void
  );

  void
  b_transport                                         ///< b_transport
  ( tlm::tlm_generic_payload  &trans                  ///< transaction
  );

  unsigned int                                        ///< ???
  transport_dbg                                       ///< transport_dbg
  ( tlm::tlm_debug_payload  &r                        ///< debug payload
  );

  bool                                                ///< success/failure
  get_direct_mem_ptr                                  ///< get_direct_mem_ptr
  ( const sc_dt::uint64     &address,                 ///< DMI address
          tlm::tlm_dmi_mode &dmi_mode,                ///< DMI mode
          tlm::tlm_dmi      &dmi_data                 ///< DMI data
  );

  private:
 
  const unsigned int      m_ID;                       ///< target ID 
        sc_dt::uint64     m_base_address;             ///< base address
        sc_dt::uint64     m_memory_size;              ///< memory size
        unsigned int      m_memory_width;             ///< word size
        unsigned char     *m_memory;                  ///< target memory
  const sc_core::sc_time  m_read_delay;               ///< read delay
  const sc_core::sc_time  m_write_delay;              ///< write delay
};

#endif  /* __UT_TARGET_H__ */
