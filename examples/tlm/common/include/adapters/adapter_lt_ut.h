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
 @file adapter_lt_ut.h
 
 @brief LT-UT adapter class header
 
  Original Authors:
    Charles Wilson, ESLX
    Bill Bunton, ESLX
    
--------------------------------------------------------------------------------------- */

#ifndef __ADAPTER_LT_UT_H__ 
#define __ADAPTER_LT_UT_H__

#include "tlm.h"                                      ///< TLM headers
#include "ut_initiator.h"                             ///< UT initiator header
#include "SimpleBus.h"                                ///< SimpleBus

class adapter_lt_ut                                   ///< LT to UT adapter
:         public sc_core::sc_module                   ///< module base clase
, virtual public tlm::tlm_fw_nb_transport_if<>        ///< target socket
, virtual public tlm::tlm_bw_b_transport_if           ///< initiator socket
{
  // sockets and ports 

  public:

  tlm::tlm_target_socket<>      adapter_socket;       ///< TLM 2 adapter socket
  tlm::tlm_b_initiator_socket<> initiator_socket;     ///< UT initiator socket

  // constructors / destructor 

  public:

  adapter_lt_ut                                       ///< constructor
  ( sc_core::sc_module_name name                      ///< name 
  );
  
  ~adapter_lt_ut                                      ///< destructor
  ( void
  );

  // member functions
  
  public:
  
  tlm::tlm_sync_enum                                  ///< sync status
  nb_transport                                        ///< non-blocking transport
  ( tlm::tlm_generic_payload  &gp                     ///< generic payoad pointer
  , tlm::tlm_phase            &phase                  ///< transaction phase
  , sc_core::sc_time          &delay_time             ///< time it should take for transport
  );

  unsigned int                                        ///< result
  transport_dbg                                       ///< transport debug
  ( tlm::tlm_generic_payload   &payload               ///< generic payload
  );

  bool                                                ///< success / failure
  get_direct_mem_ptr                                  ///< get direct memory pointer
  ( tlm::tlm_generic_payload   &payload,              ///< address + extensions
    tlm::tlm_dmi               &dmi_data              ///< dmi data
  );
  
  void
  invalidate_direct_mem_ptr                           ///< invalidate_direct_mem_ptr
  ( sc_dt::uint64             start_range             ///< start range
  , sc_dt::uint64             end_range               ///< end range
  );

  void
  b_transport
  ( tlm::tlm_generic_payload  &trans                  ///< transaction
  );
}; 

#endif /* __ADAPTER_LT_UT_H__ */
