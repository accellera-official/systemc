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
///  @file dmi_memory.h
//
///  @brief performs dmi "housekeeping" and actual reads and writes
//
//==============================================================================
//
//  Original Authors:
//    Jack Donovan, ESLX
//    Charles Wilson, ESLX
//
//==============================================================================

#ifndef __DMI_MEMORY_H__
#define __DMI_MEMORY_H__

#include "tlm.h"                                // TLM headers

class dmi_memory                          
{
// Member Methods  =============================================================
  
public:
    
//==============================================================================
/// @fn dmi_memory.h
//
///  @brief Constructor
//
//==============================================================================
  dmi_memory              
  (
    const unsigned int ID                 ///< initiator ID for messaging
  );

 //=============================================================================
 /// @fn operation
 ///
 ///  @brief Performs the DMI based Memory Operation specified in the GP
 ///
 ///  @details
 ///    Performs the operation specified by the GP
 ///    Returns after updating the status of the GP (if required)
 ///    and updating the time based upon initialization parameters
 ///
 ///===================================================================
  void  
  operation
  (
    tlm::tlm_generic_payload  &gp           ///< TLM2 GP reference 
  , sc_core::sc_time          &delay_time   ///< transaction delay 
  );    
   
 //=============================================================================
 /// @fn load_dmi_ptr
 //
 ///  @brief Configures the internal data structures with the DMI object
 //
 ///============================================================================ 
  void
  load_dmi_ptr
  (
    tlm::tlm_dmi &dmi_parameters
  );
  
  //============================================================================
  /// @fn invalidate_dmi_ptr
  //
  ///  @brief Invalidates a dmi pointer associated with an address range
  //
  ///===========================================================================   
  void
  invalidate_dmi_ptr
  (
    sc_dt::uint64     start_range
  , sc_dt::uint64     end_range
  );
 
  //============================================================================
  /// @fn is_address_dmi
  //
  ///  @brief Checks the address in the gp to see if in range of a dmi pointer
  //
  ///===========================================================================   	  
  bool 
  is_address_dmi
  (
    tlm::tlm_generic_payload  &gp        
  );   
  
   // Member Variables
   
   private:
    
   unsigned int                 m_ID;                    ///< initiator ID
   unsigned char               *m_data;
   unsigned int                 m_length;
   sc_dt::uint64                m_dmi_size;
   sc_dt::uint64                m_offset;
   sc_dt::uint64                m_address;
   sc_core::sc_time             m_dmi_read_latency;
   sc_core::sc_time             m_dmi_write_latency;
   sc_dt::uint64		m_dmi_base_address;
   tlm::tlm_command             m_command;
   unsigned char               *m_dmi_ptr;
   sc_dt::uint64                m_start_address;
   sc_dt::uint64                m_end_address;
   bool                         m_is_dmi_flag;
   tlm::tlm_dmi::dmi_access_e   m_granted_access;
}; 
 #endif /* __DMI_MEMORY_H__ */
