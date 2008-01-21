/*****************************************************************************
 *
 *   The following code is derived, directly or indirectly, from the SystemC
 *   source code Copyright (c) 1996-2008 by all Contributors.
 *   All Rights reserved.
 *
 *   The contents of this file are subject to the restrictions and limitations
 *   set forth in the SystemC Open Source License Version 3.0 (the "License");
 *   You may not use this file except in compliance with such restrictions and
 *   limitations. You may obtain instructions on how to receive a copy of the
 *   License at http://www.systemc.org/. Software distributed by Contributors
 *   under the License is distributed on an "AS IS" basis, WITHOUT WARRANTY OF
 *   ANY KIND, either express or implied. See the License for the specific
 *   language governing rights and limitations under the License.
 *
 *   *****************************************************************************/

/* ---------------------------------------------------------------------------------------
 @file lt_direct_memory_interface.h
 
 @brief lt_direct_memory_interface class header
 
  Original Authors:
    Bill Bunton, ESLX
    Charles Wilson, ESLX
    
--------------------------------------------------------------------------------------- */

#ifndef __LT_DIRECT_MEMORY_INTERFACE_TOP_H__
#define __LT_DIRECT_MEMORY_INTERFACE_TOP_H__

#include "tlm.h"                                    ///< TLM headers
#include "lt_target.h"                              ///< LT target
#include "SimpleBus.h"                              ///< simple bus
#include "lt_processor_abstraction.h"               ///< LT processor abstraction

class lt_direct_memory_interface_top                ///< top wrapper module 
: public sc_core::sc_module                         ///< SystemC module base class
{
  public:

  lt_direct_memory_interface_top                    ///< constructor
  ( sc_core::sc_module_name name                    ///< module name
  );

  // Member variables

  private:
  
  SimpleBus<2, 2>           m_bus;                  ///< bus (2 initiators, 2 targets)

  lt_target                 m_lt_target_1;          ///< target 1
  lt_target                 m_lt_target_2;          ///< target 2
  lt_processor_abstraction  m_lt_initiator_1;       ///< initiator 1
  lt_processor_abstraction  m_lt_initiator_2;       ///< initiator 2
};

#endif /* __LT_DIRECT_MEMORY_INTERFACE_TOP_H__ */
