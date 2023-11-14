/*****************************************************************************

  Licensed to Accellera Systems Initiative Inc. (Accellera) under one or
  more contributor license agreements.  See the NOTICE file distributed
  with this work for additional information regarding copyright ownership.
  Accellera licenses this file to you under the Apache License, Version 2.0
  (the "License"); you may not use this file except in compliance with the
  License.  You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

  Unless required by applicable law or agreed to in writing, software
  distributed under the License is distributed on an "AS IS" BASIS,
  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or
  implied.  See the License for the specific language governing
  permissions and limitations under the License.

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
