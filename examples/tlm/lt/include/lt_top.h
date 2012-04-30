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
 ******************************************************************************/
 
//==============================================================================
///  @file lt_top.h
// 
///  @brief Top level interconnect and instantiation for lt example 
//
//==============================================================================
//
//  Original Authors:
//    Jack Donovan, ESLX
//    Charles Wilson, ESLX
//    Anna Keist, ESLX
//
//==============================================================================

#ifndef __LT_TOP_H__
#define __LT_TOP_H__

#include "tlm.h"                              // TLM header
#include "lt_target.h"                        // lt memory target
#include "at_target_1_phase.h"                // at and lt memory target
#include "initiator_top.h"                    // processor abstraction initiator
#include "models/SimpleBusLT.h"               // Bus/Router Implementation

/// Top wrapper Module
class lt_top                                  // Declare SC_MODULE
: public sc_core::sc_module                   
{
public:
	
/// Constructor	
  lt_top ( sc_core::sc_module_name name); 

//Member Variables  ===========================================================
  private:
  SimpleBusLT<2, 2>       m_bus;                  ///< simple bus
  at_target_1_phase       m_at_and_lt_target_1;   ///< combined blocking/non-blocking
  lt_target               m_lt_target_2;          ///< blocking with convenienece socket
  initiator_top           m_initiator_1;          ///< instance 1 initiator
  initiator_top           m_initiator_2;          ///< instance 2 initiator
};
#endif /* __LT_TOP_H__ */
