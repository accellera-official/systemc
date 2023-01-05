/*****************************************************************************

  Licensed to Accellera Systems Initiative Inc. (Accellera) under one or
  more contributor license agreements.  See the NOTICE file distributed
  with this work for additional information regarding copyright ownership.
  Accellera licenses this file to you under the Apache License, Version 2.0
  (the "License"); you may not use this file except in compliance with the
  License.  You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

  Unless required by applicable law or agreed to in writing, software
  distributed under the License is distributed on an "AS IS" BASIS,
  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or
  implied.  See the License for the specific language governing
  permissions and limitations under the License.

 *****************************************************************************/

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

#include "reporting.h"                  // reporting utilities
#include "at_ooo_top.h"  		        // top module
#include "tlm.h"            			// TLM header

//=====================================================================
///  @fn sc_main
//  
///  @brief SystemC entry point
//  
///  @details
///    This is the SystemC entry point for the example system.  The argc and argv 
///    parameters are not used.  Simulation runtime is not specified when 
///    “sc_start()“ is called, the example’s traffic generator will run to 
///    completion, ending the simulation. 
///
//=====================================================================
int                                     // return status
sc_main                                 // SystemC entry point
( int    /*argc*/                       // argument count
, char*  /*argv*/[]                     // argument vector
)
{
  REPORT_ENABLE_ALL_REPORTING ();
  
  example_system_top top("top");        // instantiate a exmaple top module

  sc_core::sc_start();                  // start the simulation

  return 0;                             // return okay status
}
