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

//=====================================================================
/// @file lt_target.h
//
/// @brief special target to illustrate dmi access to memory
//
//=====================================================================
//  Original Authors:
//   Jack Donovan, ESLX
//   Charles Wilson, ESLX
//=====================================================================

#ifndef __LT_TARGET_H__
#define __LT_TARGET_H__

#include "tlm.h"                                // TLM headers
#include "memory.h"
#include "tlm_utils/simple_target_socket.h"

class lt_target
:     public sc_core::sc_module           	/// inherit from SC module base clase
//, virtual public tlm::tlm_fw_transport_if<>   /// inherit from TLM "forward interface"

{
// Member Methods =====================================================

  public:
//=====================================================================
///	@fn lt_target
///
///	@brief Constructor for Single Phase AT target
///
///	@details
///		Generic Single Phase target used in several examples.
///		Constructor offers several parameters for customization
///
//=====================================================================

  lt_target
  ( sc_core::sc_module_name   module_name           ///< SC module name
  , const unsigned int        ID                    ///< target ID
  , const char                *memory_socket        ///< socket name
  , sc_dt::uint64             memory_size           ///< memory size (bytes)
  , unsigned int              memory_width          ///< memory width (bytes)
  , const sc_core::sc_time    accept_delay          ///< accept delay (SC_TIME, SC_NS)
  , const sc_core::sc_time    read_response_delay   ///< read response delay (SC_TIME, SC_NS)
  , const sc_core::sc_time    write_response_delay  ///< write response delay (SC_TIME, SC_NS)
  );

  private:

/// b_transport() - Blocking Transport
  void                                                // returns nothing
  custom_b_transport
  ( tlm::tlm_generic_payload  &payload                // ref to payload
  , sc_core::sc_time          &delay_time             // delay time
  );

// Member Variables ===================================================

  public:

  typedef tlm::tlm_generic_payload  *gp_ptr;		///< generic payload pointer

  tlm_utils::simple_target_socket<lt_target>  m_memory_socket; ///<  target socket
  tlm_utils::simple_target_socket_optional<lt_target>  m_optional_socket; ///< optional extra socket

  private:

  const unsigned int        m_ID;                   ///< target ID
        sc_dt::uint64       m_memory_size;          ///< memory size (bytes)
        unsigned int        m_memory_width;         ///< word size (bytes)
  const sc_core::sc_time    m_accept_delay;         ///< accept delay
  const sc_core::sc_time    m_read_response_delay;  ///< read response delay
  const sc_core::sc_time    m_write_response_delay; ///< write response delays

        memory              m_target_memory;
};


#endif /* __LT_TARGET_H__ */
