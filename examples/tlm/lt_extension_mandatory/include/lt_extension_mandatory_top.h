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
