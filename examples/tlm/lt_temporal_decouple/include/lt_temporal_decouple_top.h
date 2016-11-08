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
///  @file lt_temporal_decoupling_top.h
// 
///  @brief Top level for lt example with temporal decoupling
//
//==============================================================================
//
//  Original Authors:
//    Jack Donovan, ESLX
//    Charles Wilson, ESLX
//    Anna Keist, ESLX
//
//==============================================================================

#ifndef __LT_TEMPORAL_DECOUPLE_TOP_H__
#define __LT_TEMPORAL_DECOUPLE_TOP_H__

#include "tlm.h"                        // TLM header
#include "lt_target.h"                  // lt memory target
#include "lt_synch_target.h"            // lt target requiring synch'ing
#include "initiator_top.h"              // "normal" lt iniator
#include "td_initiator_top.h"           // initiator_top with lt with td
#include "models/SimpleBusLT.h"         // Bus/Router Implementation

/// Top wrapper Module
class lt_temporal_decouple_top        // Declare the SC_MODULE   
: public sc_core::sc_module             
{
public:
	
/// Constructor	
  lt_temporal_decouple_top              
  ( sc_core::sc_module_name name); 

//Member Variables  ===========================================================
  private:
  SimpleBusLT<2, 2>       m_bus;               ///< simple bus
  lt_synch_target         m_lt_synch_target_1; ///< target requiring synch
  lt_target               m_lt_target_2;       ///< well behaved lt target
  td_initiator_top        m_td_initiator_1;    ///< LT with Temp Decoup
  initiator_top           m_initiator_2;       ///< Basic LT (no Temp Decoup)
};
#endif /* __LT_TEMPORAL_DECOUPLE_TOP_H__ */
