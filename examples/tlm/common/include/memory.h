/*******************************************************************************
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
*******************************************************************************/
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
    
   unsigned int          m_ID;                    ///< initiator ID
   sc_core::sc_time      m_read_delay;            ///< read delay
   sc_core::sc_time      m_write_delay;           ///< write delay
   sc_dt::uint64         m_memory_size;           ///< memory size (bytes)
   unsigned int          m_memory_width;          ///< memory width (bytes)
    
   unsigned char         *m_memory;               ///< memory
   
   bool                  m_previous_warning;      ///< limits to one message
    
}; 
 #endif /* __MEMORY_H__ */
