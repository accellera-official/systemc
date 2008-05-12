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
 
/* ---------------------------------------------------------------------------------------
 @file ut_lt_top.h
 
 @brief UT-LT top class header
 
  Original Authors:
    Charles Wilson, ESLX
    
--------------------------------------------------------------------------------------- */

#ifndef __UT_LT_TOP_H__
#define __UT_LT_TOP_H__

#include "tlm.h"                                    ///< TLM headers
#include "lt_target_memory.h"                       ///< memory target
#include "lt_processor_abstraction.h"               ///< processor abstraction initiator
#include "SimpleBus.h"                              ///< simple bus
#include "adapter_ut_lt.h"                          ///< adpter UT to LT
#include "adapter_lt_ut.h"                          ///< adpter LT to UT
#include "ut_initiator.h"                           ///< UT initiator 
#include "ut_target.h"                              ///< UT target memory 

class ut_lt_top                                     ///< top class
: public sc_core::sc_module                         ///< module base class
{
  public:

  ut_lt_top                                         ///< constructor
  ( sc_core::sc_module_name name                    ///< module name
  );
  
  // member variables

  private:
  
  SimpleBus<2, 2>           m_bus;                  ///< bus

  lt_target_memory          m_target_1;             ///< target 1
  lt_processor_abstraction  m_initiator_1;          ///< initiator 1

  adapter_ut_lt             m_adapter_ut_lt;        ///< adapter UT to LT 
  adapter_lt_ut             m_adapter_lt_ut;        ///< adapter LT to UT
 
  ut_initiator              m_ut_initiator;         ///< UT initiator 
  ut_target                 m_ut_target;            ///< UT target memory 
};
#endif /* __UT_LT_TOP_H__ */
