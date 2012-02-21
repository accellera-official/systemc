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

#ifndef __LT_INITIATOR_EXTENSION_MANDATORY_H__
#define __LT_INITIATOR_EXTENSION_MANDATORY_H__

#include "tlm.h"                                    // TLM headers
#include "tlm_utils/simple_initiator_socket.h"      // simple socket header
#include "extension_initiator_id.h"                 // extension header
#include "reporting.h"                              // reporting utility routines

#include <cassert>                                  // assert ()

class lt_initiator_extension_mandatory
: public sc_core::sc_module
{
  public:
  
  typedef tlm::tlm_generic_payload  transaction_type;       ///< transaction type
  typedef tlm::tlm_dmi              dmi_type;               ///< DMI type  
  typedef tlm::tlm_phase            phase_type;             ///< phase type
  typedef tlm::tlm_sync_enum        sync_enum_type;         ///< synchronization type
  
  typedef tlm_utils::simple_initiator_socket< lt_initiator_extension_mandatory
                                            , 32
                                            , extension_initiator_id>
                                    initiator_socket_type;  ///< initiator socket type

  SC_HAS_PROCESS( lt_initiator_extension_mandatory );
  
  lt_initiator_extension_mandatory                      ///< constructor
  ( sc_core::sc_module_name name                        ///< instance name
  , unsigned int            num_trans                   ///< number of transactions
  , sc_dt::uint64           base_address                ///< starting address
  );

  bool                                                  ///< transaction created
  create_transaction                                    ///< create new transaction
  ( transaction_type    &trans                          ///< transaction
  );
  
  void
  log_start                                             ///< log start of transaction
  ( transaction_type    &trans                          ///< transaction
  );

  void
  log_end                                               ///< log end of transaction
  ( transaction_type    &trans                          ///< transaction
  );

  void
  initiator_thread                                      ///< initiator thread
  ( void
  );
  
  sync_enum_type                                        ///< state
  nb_transport_bw                                       ///< non-blocking backward transport
  ( transaction_type    &trans                          ///< transaction
  , phase_type          &phase                          ///< phase
  , sc_core::sc_time    &t                              ///< time
  );

  void
  invalidate_dmi_properties                             ///< invalidate DMI properties
  ( void
  );

  void
  invalidate_direct_mem_ptr                             ///< invalidate DMI pointer
  ( sc_dt::uint64       start_range                     ///< start DMI range
  , sc_dt::uint64       end_range                       ///< end DMI range
  );

  //  member variables
  
  public:
  
  initiator_socket_type m_socket;                     ///< socket

  private:
  
  dmi_type              m_dmi_properties;             ///< DMI propteries
  sc_core::sc_event     mEndEvent;                    ///< end event
  unsigned int          m_transaction_max;            ///< transaction maximum
  sc_dt::uint64         m_base_address;               ///< address
  unsigned int          m_transaction_count;          ///< transaction count
  unsigned int          m_data;                       ///< transaction data
};

#endif /* __LT_INITIATOR_EXTENSION_MANDATORY_H__ */
