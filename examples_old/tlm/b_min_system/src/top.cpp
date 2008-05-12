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
  @file top.cpp
  
  @brief Example top module
=============================================================================*/

#include "top.h"                           ///< our header

/*=============================================================================
  Constructor
=============================================================================*/
top::top(sc_module_name name): sc_module(name),
                               GENERATOR_1( "GENERATOR_1"),
                               GENERATOR_2( "GENERATOR_2"),
                               BUS( "BUS"),
                               MEMORY_1("MEMORY_1", 1024),
                               MEMORY_2("MEMORY_2", 1024)
{
  // perform component bindings
  
  GENERATOR_1.initiator_socket.bind(*(BUS.target_socket[0]));
  GENERATOR_2.initiator_socket.bind(*(BUS.target_socket[1]));
  
  (*BUS.initiator_socket[0])(MEMORY_1.target_socket);
  (*BUS.initiator_socket[1])(MEMORY_2.target_socket);
}

