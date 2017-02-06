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
//==============================================================================
///  @file memory.h
//
///  @brief Object for isolationg memory operations from TLM "shell"
//
//==============================================================================
//
//  Original Authors:
//    Jack Donovan, ESLX
//
//==============================================================================

#ifndef __MEMORY_H__
#define __MEMORY_H__

#include "tlm.h"                                // TLM headers

class memory
{
// Member Methods  ====================================================

    memory(const memory&) /* = delete*/;
    memory& operator=(const memory&) /* = delete */;

  public:

//=====================================================================
/// @fn memory.h
//
///  @brief memory Constructor
//
///  @details
//       Initialize member variables, include allocating and initializing
//       the actual memory
//
//=====================================================================
  memory
  (
    const unsigned int ID                 ///< initiator ID for messaging
  , sc_core::sc_time   read_delay         ///< delay for reads
  , sc_core::sc_time   write_delay        ///< delay for writes
  , sc_dt::uint64      memory_size        ///< memory size (bytes)
  , unsigned int       memory_width       ///< memory width (bytes)
  );

 //====================================================================
 /// @fn operation
 ///
 ///  @brief Performs the Memory Operation specified in the GP
 ///
 ///  @details
 ///    Performs the operation specified by the GP
 ///    Returns after updating the status of the GP (if required)
 ///    and updating the time based upon initialization parameters
 ///
 ///===================================================================
  void
  operation(
      tlm::tlm_generic_payload  &gp           ///< TLM2 GP reference
    , sc_core::sc_time          &delay_time   ///< transaction delay
    );

 //====================================================================
 /// @fn get_delay
 ///
 ///  @brief Looks at GP and returns delay without doing GP Operation
 ///
 ///  @details
 ///    Performs the operation specified by the GP
 ///    Returns after updating the status of the GP (if required)
 ///    and updating the time based upon initialization parameters
 ///
 ///===================================================================
  void
  get_delay(
      tlm::tlm_generic_payload  &gp           ///< TLM2 GP reference
    , sc_core::sc_time          &delay_time   ///< time to be updated
    );

  unsigned char* get_mem_ptr(void);

  private:

/// Check the address vs. range passed at construction

  tlm::tlm_response_status
  check_address
  (
    tlm::tlm_generic_payload  &gp
  );

// Member Variables/Objects  ===================================================

   private:

   const unsigned int     m_ID;                   ///< initiator ID
   const sc_core::sc_time m_read_delay;           ///< read delay
   const sc_core::sc_time m_write_delay;          ///< write delay
   const sc_dt::uint64    m_memory_size;          ///< memory size (bytes)
   const unsigned int     m_memory_width;         ///< memory width (bytes)

   unsigned char          *m_memory;              ///< memory

   bool                   m_previous_warning;     ///< limits to one message

};
 #endif /* __MEMORY_H__ */
