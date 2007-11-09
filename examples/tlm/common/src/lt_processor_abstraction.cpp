/*****************************************************************************

  The following code is derived, directly or indirectly, from the SystemC
  source code Copyright (c) 1996-2007 by all Contributors.
  All Rights reserved.

  The contents of this file are subject to the restrictions and limitations
  set forth in the SystemC Open Source License Version 2.4 (the "License");
  You may not use this file except in compliance with such restrictions and
  limitations. You may obtain instructions on how to receive a copy of the
  License at http://www.systemc.org/. Software distributed by Contributors
  under the License is distributed on an "AS IS" basis, WITHOUT WARRANTY OF
  ANY KIND, either express or implied. See the License for the specific
  language governing rights and limitations under the License.

 *****************************************************************************/
//==============================================================================
/*
 */
//==============================================================================
// Note: order of headers is important
#include "tlm.h"                                   ///< TLM headers
#include "reporting.h"                             ///< Reporting convenience macros
#include "examples_rw_if.h"                        ///< R/W interface for ports
#include "traffic_generator.h"                     ///< Traffic generator header
#include "lt_initiator.h"                          ///< Initiator header
//#include "direct_execution.h"                    ///< Direct execution header
#include "lt_processor_abstraction.h"              ///< Our header

static char *msg_type = "lt_processor_abstraction";

//==============================================================================
// Create traffic generator configuration structure  
//==============================================================================
static const traffic_generator_config tg_config = 
{ 64                                ///< cache line size

, 0x0000000000000000                ///< i-cache base address
, 0x000000000000F000                ///< i-cache size
, 78                                ///< i-cache percentage
  
, 0x0000000000000000                ///< d-cachebase address
, 0x000000000000F000                ///< d-cache size
, 15                                ///< d-cache reads percentage
, 4                                 ///< d-cache writes percentage
  
, 0x0000000000000000                ///< scratch base address
, 0x000000000000F000                ///< scratch size
, 2                                 ///< scratch reads percentage
, 1                                 ///< scratch writes percentage
};               

//==============================================================================
// Constructor
//==============================================================================
lt_processor_abstraction::lt_processor_abstraction(     ///< constructor
    sc_core::sc_module_name name)                       ///< name
  : sc_module(name)
  , processor_abstraction_socket("processor_abstraction_socket") 
  , m_initiator("m_initiator")
  , m_tg_config(tg_config) 
  , m_traffic_generator("m_traffic_generator", m_tg_config)
//  , m_direct_execution("m_direct_execution", 0, sc_core::sc_time(1, sc_core::SC_NS))
  {

//----------------------------------------------------------
// Bind internal ports 
  m_traffic_generator.rw_port(m_initiator.rw_export);
//  m_direct_execution.rw_port(m_initiator.rw_export);

//----------------------------------------------------------
// Do initiator-socket to initiator-socket hierarchical binding 
  m_initiator.initiator_socket(processor_abstraction_socket);
}

//------------------------------------------------------------------------------
// Destructure
lt_processor_abstraction::~lt_processor_abstraction(void) {   ///< destructor
 
}
