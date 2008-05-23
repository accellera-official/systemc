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
  @file tlm_1_integration_top.h
  
  @brief LT top module class
=============================================================================*/

/*****************************************************************************
  Original Authors:
    Bill Bunton, ESLX
    Charles Wilson, ESLX
*****************************************************************************/

#ifndef __TLM_1_INTEGRATION_TOP_H__
#define __TLM_1_INTEGRATION_TOP_H__

#include "tlm.h"                              ///< TLM
#include "lt_target_memory.h"                 ///< memory target
#include "lt_processor_abstraction.h"         ///< processor abstraction initiator
#include "SimpleBus.h"                        ///< simple bus
#include "adapter_tlm_1_2_sync.h"             ///< adpter tlm 1 to 2
#include "adapter_tlm_2_1_sync.h"             ///< adpter tlm 2 to 1
#include "tlm_1_master_2.h"                   ///< tlm1 master 
#include "tlm_1_slave_memory_explicit.h"      ///< tlm1 slave memory 

class lt_top                                  ///< class lt_main
: public sc_core::sc_module                   ///< module base class
{
  public:

  lt_top                                      ///< constructor
  ( sc_core::sc_module_name name);            ///< module name



  //==============================================================================
  // Member variables
  //==============================================================================
  private:
  
  SimpleBus<2, 2>             m_bus;                          ///< bus

  lt_target_memory            m_target_1;                     ///< target 1
  lt_processor_abstraction    m_initiator_1;                  ///< initiator 1

  adapter_tlm_1_2_sync        m_adapter_tlm_1_2_sync;         ///< adapter 1 to 2 
  adapter_tlm_2_1_sync        m_adapter_tlm_2_1_sync;         ///< adapter 2 to 1
 
  tlm_1_master_2              m_tlm_1_master_2;               ///< tlm1 master 
  tlm_1_slave_memory_explicit m_tlm_1_slave_memory_explicit;  ///< tlm1 memory slave 

};
#endif /* __TLM_1_INTEGRATION_TOP_H__ */
