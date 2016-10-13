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
///  @file lt_extension_mandatory_top.h
 
///  @brief This class instantiates components that compose the TLM 2 example system 

//=====================================================================
//  Original Authors:
//    Charles Wilson, ESLX
//    Anna Keist, ESLX
//    Bill Bunton, ESLX
//    Jack Donovan, ESLX
//=====================================================================

#ifndef __LT_EXTENSION_MANDATORY_TOP_H__
#define __LT_EXTENSION_MANDATORY_TOP_H__

#include "tlm.h"                                  ///< TLM header
#include "lt_initiator_extension_mandatory.h"     ///< initiator header
#include "lt_target_extension_mandatory.h"        ///< target header

/// Top wrapper Module

class lt_extension_mandatory_top             
: public sc_core::sc_module                       ///< SystemC base class
{
  public:
	
  // constructor
	
  lt_extension_mandatory_top                      ///< constructor
  ( sc_core::sc_module_name name                  ///< instance name
  ); 

  // member variables
  
  private:
  
  lt_initiator_extension_mandatory  m_initiator;  ///< initiator
  lt_target_extension_mandatory     m_target;     ///< target
};

#endif /* __LT_EXTENSION_MANDATORY_TOP_H__ */
