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
///  @file example_main.cpp
///
///  @brief Example main instantiates the example system top and call 
///  sc_start() 
///
//=====================================================================
//  Original Authors:
//    Bill Bunton, ESLX
//    Anna Keist, ESLX
//    Charles Wilson, ESLX
//    Jack Donovan, ESLX
//=====================================================================

// define REPORT_DEFINE_GLOBALS in one location only

#define REPORT_DEFINE_GLOBALS

#include "reporting.h"              // reporting utilities
#include "at_4_phase_top.h"  		    // top module
#include "tlm.h"            			  // TLM header

//=====================================================================
///  @fn sc_main
//  
///  @brief SystemC entry point
//  
///  @details
///    This is the SystemC entry point for the example system.  The argc and argv 
///    parameters are not used.  Simulation runtime is not specified when 
///    “sc_start()” is called, the example’s traffic generator will run to 
///    completion, ending the simulation. 
///
//=====================================================================
int                                     // return status
sc_main                                 // SystemC entry point
  (int   argc                           // argument count
  ,char  *argv[]                        // argument vector
)
{
  REPORT_ENABLE_ALL_REPORTING ();
//  REPORT_DISABLE_INFO_REPORTING ();
  
  example_system_top top("top");        // instantiate a exmaple top module

  sc_core::sc_start();                  // start the simulation

  return 0;                             // return okay status
}
