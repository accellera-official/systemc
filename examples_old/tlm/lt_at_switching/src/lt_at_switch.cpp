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
  @file at_lt_full_system.cpp
  
  @brief Approximately Timed (AT) minimum system example testbed
  
=============================================================================*/

/*****************************************************************************
  Original Authors:
    Bill Bunton, ESLX
    Anna Keist, ESLX
*****************************************************************************/

#include "lt_at_switch_top.h"                         ///< top module
#include "tlm.h"                                      ///< TLM header

// temp spot for global structure
struct global_target_switch {
  sc_core::sc_time initial_time;
  int target_model_ref[2];              ///< storage for target model state 
  global_target_switch() {
    initial_time = sc_core::sc_time_stamp();
  }
};

/*=============================================================================
  @fn sc_main
  
  @brief entry point
  
  @details
    This is the SystemC entry point. It is called (eventually) by main ().
    
  @param argc argument count
  @param arvg argument vector
  
  @retval status typically 0
=============================================================================*/
int                                     ///< return status
sc_main                                 ///< SystemC entry point
( int   argc                            ///< argument count
, char  *argv[]                         ///< argument vector
)
{
  // instantiate a top module
  lt_at_switch_top top("top");
  
  // start the simulation
  sc_core::sc_start();

  sc_core::sc_stop();

  // return okay status
  return 0;
}
