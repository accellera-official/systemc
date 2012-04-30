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
///  @file lt_dmi_top.h
// 
///  @brief Top level interconnect and instantiation for lt with DMI example 
//
//==============================================================================
//
//  Original Authors:
//    Jack Donovan, ESLX
//    Anna Keist, ESLX
//    Charles Wilson, ESLX
//==============================================================================

#ifndef __LT_DMI_TOP_H__
#define __LT_DMI_TOP_H__

#include "tlm.h"                                // TLM header
#include "lt_dmi_target.h"                      // lt wih DMI support mem target
#include "initiator_top.h"                      // top for lt with DMI initiator
#include "models/SimpleBusLT.h"                 // Bus/Router Implementation

/// Top wrapper Module
class lt_dmi_top                                // Declare SC_MODULE
: public sc_core::sc_module                    
{
public:
	
  /// Constructor	
  lt_dmi_top( sc_core::sc_module_name name ); 

  void limit_thread(void);                      ///< limit_thread


//Member Variables  ============================================================  
private:
  SimpleBusLT<2, 2>       m_bus;                ///< simple bus
  sc_core::sc_time        m_simulation_limit;   ///< simulation time limit
  lt_dmi_target           m_lt_dmi_target_1;    ///< lt with dmi mem target
  lt_dmi_target           m_lt_dmi_target_2;    ///< 2nd lt with dmi mem target
  initiator_top           m_initiator_1;        ///< lt initiatior with dmi support
  initiator_top           m_initiator_2;        ///< 2nd initiator
};
#endif /* __LT_DMI_TOP_H__ */
