/*****************************************************************************

  The following code is derived, directly or indirectly, from the SystemC
  source code Copyright (c) 1996-2007 by all Contributors.
  All Rights reserved.

  The contents of this file are subject to the restrictions and limitations
  set forth in the SystemC Open Source License Version 3.0 (the "License");
  You may not use this file except in compliance with such restrictions and
  limitations. You may obtain instructions on how to receive a copy of the
  License at http://www.systemc.org/. Software distributed by Contributors
  under the License is distributed on an "AS IS" basis, WITHOUT WARRANTY OF
  ANY KIND, either express or implied. See the License for the specific
  language governing rights and limitations under the License.

 *****************************************************************************/
 
/*=============================================================================
  @file gp_mandatory_extension_top.h
  
  @brief Generic Payload Optional Extension top module class
=============================================================================*/

/*****************************************************************************
  Original Authors:
    Anna Keist, ESLX
    Bill Bunton, ESLX
*****************************************************************************/

#ifndef __GP_MANDATORY_EXTENSION_TOP_H__
#define __GP_MANDATORY_EXTENSION_TOP_H__

#include "tlm.h"                              ///< TLM header
#include "at_target_memory.h"                 ///< at memory target
#include "lt_target.h"                        ///< lt target
#include "at_processor_abstraction.h"         ///< processor abstraction initiator
#include "SimpleBusInitiatorID.h"                        ///< simple bus 

class gp_mandatory_extension_top              ///<  top wrapper module 
: public sc_core::sc_module                   ///<  SystemC module base class
{
  public:

  gp_mandatory_extension_top                  ///<  constructor
  ( sc_core::sc_module_name name);            ///<  module name

  //=============================================================================
  // Member variables
  //=============================================================================
  private:
  
  SimpleBusInitiatorID<2, 2>          m_bus;                ///< simple bus
  at_target_memory         m_at_target_memory_1; ///< target 
  lt_target                m_lt_target_2;        ///< lt target
  at_processor_abstraction m_at_initiator_1;     ///< initiator
  at_processor_abstraction m_at_initiator_2;     ///< initiator

};
#endif /* __GP_MANDATORY_EXTENSION_TOP_H__ */
