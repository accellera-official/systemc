/**********************************************************************
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
 *********************************************************************/

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
