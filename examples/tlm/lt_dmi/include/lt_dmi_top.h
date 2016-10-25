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
