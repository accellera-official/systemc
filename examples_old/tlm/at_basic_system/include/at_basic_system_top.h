/*****************************************************************************

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

 *****************************************************************************/
 
/*=============================================================================
  @file at_basic_system_top.h
  
  @brief AT top module class
=============================================================================*/

/*****************************************************************************
  Original Authors:
    Anna Keist, ESLX
    Bill Bunton, ESLX
*****************************************************************************/

#ifndef __AT_BASIC_SYSTEM_TOP_H__
#define __AT_BASIC_SYSTEM_TOP_H__

#include "tlm.h"                              ///< TLM header
#include "at_target_4_phase.h"                ///< at memory target
#include "at_processor_abstraction.h"         ///< processor abstraction initiator
#include "SimpleBus.h"                        ///< simple bus 

class at_basic_system_top                     ///<  top wrapper module 
: public sc_core::sc_module                   ///<  SystemC module base class
{
  public:

  at_basic_system_top                         ///<  constructor
  ( sc_core::sc_module_name name);            ///<  module name

  //=============================================================================
  // Member variables
  //=============================================================================
  private:
  
  SimpleBus<2, 2>          m_bus;                 ///< simple bus
  at_target_4_phase        m_at_target_4_phase_1; ///< target 
  at_target_4_phase        m_at_target_4_phase_2; ///< target 
  at_processor_abstraction m_at_initiator_1;      ///< initiator
  at_processor_abstraction m_at_initiator_2;      ///< initiator

};
#endif /* __AT_BASIC_SYSTEM_TOP_H__ */
