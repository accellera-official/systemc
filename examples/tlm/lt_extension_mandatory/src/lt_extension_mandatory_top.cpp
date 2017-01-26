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

/* --------------------------------------------------------------------------------

  @file lt_extension_mandatory_top.cpp

  @description
    This class instantiates components that compose the TLM 2 example system. The
     same block diagram is instantiated for each version, but with different
     components.

  Original Authors:
    Charles Wilson, ESLX
    Bill Bunton, ESLX
    Anna Keist, ESLX
    Jack Donovan, ESLX

-------------------------------------------------------------------------------- */

#include "lt_extension_mandatory_top.h"           // our header

/* --------------------------------------------------------------------------------

  @fn lt_extension_mandatory_top::lt_extension_mandatory_top

  @details
    The construcor method calls the bind methods to connect the example components. 

-------------------------------------------------------------------------------- */

lt_extension_mandatory_top::lt_extension_mandatory_top  
( sc_core::sc_module_name name                    ///< instance name
)
  : sc_core::sc_module                            ///< SystemC base class
    ( name                                        ///< instance name
    )
    
  , m_initiator                                   ///< initiator
    ( "m_initiator"                               ///< instance name
    , 5                                           ///< number of transactions
    , 0                                           ///< base address
    )
  
  , m_target                                      ///< target
    ( "m_target"                                  ///< instance name
    , sc_core::sc_time ( 25, sc_core::SC_NS )     ///< invalidate DMI timeout
    )
{
  /// bind the initiators to the target
  
  m_initiator.m_socket ( m_target.m_socket );
}
