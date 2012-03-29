/*******************************************************************************
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
 ******************************************************************************/

#include "lt_dmi_top.h"                 // top module
#include "tlm.h"                        // TLM header
#define REPORT_DEFINE_GLOBALS           // reporting overhead
#include "reporting.h"

//==============================================================================
///  @fn sc_main
//  
///  @brief sc_main for lt example
//  
///  @details
///    This is the SystemC entry point for an example system.  The argc and argv 
///    parameters are not used.  Simulation runtime is not specified when 
///    sc_start() is called, the example traffic generator will run to 
///    completion, ending the simulation. 
//
//==============================================================================
//
//  Original Authors:
//    Charles Wilson, ESLX
//    Jack Donovan, ESLX
//    Anna Keist, ESLX
//
//==============================================================================
int                                     // return status
sc_main                                 // SystemC entry point
  (int   argc                           // argument count
  ,char  *argv[]                        // argument vector
)
{
  REPORT_ENABLE_ALL_REPORTING ();
  
  lt_dmi_top top("top");                // instantiate a exmaple top module

  sc_core::sc_start();                  // start the simulation
//  sc_core::sc_start(2000,sc_core::SC_NS);

  return 0;                             // return okay status
}
