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
  @file rw_target_socket.cpp
  
  @brief Implementation of rw_target_socket
=============================================================================*/
#include "rw_target_socket.h"

/*==============================================================================
  Constructor
==============================================================================*/

rw_target_socket::rw_target_socket(const char * name) 
    : tlm::tlm_target_socket<64,
                             tlm::tlm_fw_b_transport_if<>,
                             tlm::tlm_bw_b_transport_if>(name)
{
}
