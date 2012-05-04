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
 
//=====================================================================
///  @file example_system_top.h
 
///  @brief This class instantiates components that compose the TLM2 
///         example system 

//=====================================================================
//  Original Authors:
//    Anna Keist, ESLX
//    Bill Bunton, ESLX
//    Jack Donovan, ESLX
//=====================================================================

#ifndef __EXAMPLE_SYSTEM_TOP_H__
#define __EXAMPLE_SYSTEM_TOP_H__

#include "tlm.h"                              // TLM header
#include "at_target_2_phase.h"                // at memory target
#include "initiator_top.h"                    // processor abstraction initiator
#include "models/SimpleBusAT.h"               // Bus/Router Implementation

/// Top wrapper Module
class example_system_top             
: public sc_core::sc_module                   //  SC base class
{
public:
	
/// Constructor
	
  example_system_top              
  ( sc_core::sc_module_name name); 

//Member Variables  ===========================================================
  private:
  SimpleBusAT<2, 2>       m_bus;                  ///< simple bus
  at_target_2_phase       m_at_target_2_phase_1;  ///< instance 1 target
  at_target_2_phase       m_at_target_2_phase_2;  ///< instance 2 target
  initiator_top           m_initiator_1;          ///< instance 1 initiator
  initiator_top           m_initiator_2;          ///< instance 2 initiator
};
#endif /* __EXAMPLE_SYSTEM_TOP_H__ */
