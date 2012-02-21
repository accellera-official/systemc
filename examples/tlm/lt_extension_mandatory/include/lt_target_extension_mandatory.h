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

#ifndef __LT_TARGET_EXTENSION_MANDATORY_H__
#define __LT_TARGET_EXTENSION_MANDATORY_H__

#include "tlm.h"                                    // TLM header
#include "tlm_utils/simple_target_socket.h"         // simple socket header
#include "extension_initiator_id.h"                 // extension header
#include "reporting.h"                              // reporting utility routines

#include <cassert>                                  // assert ()

class lt_target_extension_mandatory
: public sc_core::sc_module
{
  public:
  
  typedef tlm::tlm_generic_payload  transaction_type;             ///< transaction type
  typedef tlm::tlm_dmi              dmi_type;                     ///< DMI type  
  typedef tlm::tlm_phase            phase_type;                   ///< phase type
  typedef tlm::tlm_sync_enum        sync_enum_type;               ///< synchronization type
  
  typedef tlm_utils::simple_target_socket< lt_target_extension_mandatory
                                         , 32
                                         , extension_initiator_id>
                                    target_socket_type;           ///< initiator socket type

  SC_HAS_PROCESS ( lt_target_extension_mandatory );

  lt_target_extension_mandatory                                   ///< constructor
  ( sc_core::sc_module_name name                                  ///< instance name
  , sc_core::sc_time        invalidate_dmi_time                   ///< DMI invalidation timeout
  );

  tlm::tlm_sync_enum                                              ///< response
  nb_transport_fw                                                 ///< non-blocking forward transport
  ( transaction_type  &trans                                      ///< transaction
  , phase_type        &phase                                      ///< phase
  , sc_core::sc_time  &t                                          ///< time
  );

  unsigned int                                                    ///< byte count
  transport_dbg                                                   ///< debug transport
  ( transaction_type  &transaction_ref                            ///< transaction
  );

  bool                                                            ///< DMI granted
  get_dmi_ptr                                                     ///< get DMI pointer
  ( transaction_type  &trans                                      ///< transaction
  , tlm::tlm_dmi      &dmi_properties                             ///< DMI properties
  );

  void
  invalidate_dmi_method                                           ///< invalidate DMI method
  ( void
  );
  
  // member variables
  
  public:
  
               target_socket_type m_socket;

  private:
  
  static const sc_dt::uint64      m_min_address = 0x0000000000000000; ///< address minimum (   0 )
  static const sc_dt::uint64      m_max_address = 0x000000000000018F; ///< address maximum ( 399 )
         const sc_core::sc_time   m_latency_read;                     ///< read latency
         const sc_core::sc_time   m_latency_write;                    ///< write latency
  
               unsigned char      m_memory [ m_max_address - m_min_address + 1 ]; ///< memory
  
               sc_core::sc_event  m_invalidate_dmi_event;             ///< DMI invalidation event
               sc_core::sc_time   m_invalidate_dmi_time;              ///< DMI invalidation time
};

#endif /* __LT_TARGET_EXTENSION_MANDATORY_H__ */
