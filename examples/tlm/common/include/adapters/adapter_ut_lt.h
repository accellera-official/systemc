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
 @file adapter_ut_lt.h
 
 @brief UT-LT adapter class header
 
  Original Authors:
    Charles Wilson, ESLX
    Bill Bunton, ESLX
    
--------------------------------------------------------------------------------------- */

#ifndef __ADAPTER_UT_LT_H__
#define __ADAPTER_UT_LT_H__

#include "tlm.h"                                      ///< TLM headers
#include "ut_target.h"                                ///< UT target header
#include "SimpleBus.h"                                ///< SimpleBus 

class adapter_ut_lt                                   ///< adapter_ut_lt 
:         public sc_core::sc_module                   ///< module base class
, virtual public tlm::tlm_bw_nb_transport_if<>        ///< initiator socket
, virtual public tlm::tlm_fw_b_transport_if<>         ///< target socket
{
  public:

  // Ports, exports and sockets
  tlm::tlm_initiator_socket<> adapter_socket;         ///< initiator socket (adapter)
  tlm::tlm_b_target_socket<>  target_socket;          ///< UT target socket

  // constructor / destructor
  adapter_ut_lt                                       ///< constructor
  ( sc_core::sc_module_name name                      ///< name
  );

  ~adapter_ut_lt                                      ///< destructor
  ( void
  );

  // member functions 

  bool                                                ///< success/failure 
  read                                                ///< read 
  ( tlm::tlm_generic_payload  &trans                  ///< transaction
  );
  
  bool                                                ///< success/failure 
  write                                               ///< write 
  ( tlm::tlm_generic_payload  &trans                  ///< transaction
  );

  tlm::tlm_sync_enum                                  ///< phase
  nb_transport                                        ///< nb_transport
  ( tlm::tlm_generic_payload  &transaction            ///< transaction
  , tlm::tlm_phase            &phase                  ///< transaction phase
  , sc_core::sc_time          &time                   ///< elapsed time
  );

  void
  b_transport                                         ///< b_transport
  ( tlm::tlm_generic_payload  &trans                  ///< transaction
  );

  unsigned int                                        ///< bytes returned
  transport_dbg                                       ///< transport_dbg
  ( tlm::tlm_debug_payload  &r                        ///< debug payload
  );

  void
  invalidate_direct_mem_ptr                           ///< invalidate_direct_mem_ptr
  ( sc_dt::uint64 start_range                         ///< start range
  , sc_dt::uint64 end_range                           ///< end range
  );

  bool                                                ///< success/failure
  get_direct_mem_ptr                                  ///< get_direct_mem_ptr
  ( const sc_dt::uint64     &address,                 ///< DMI address
          tlm::tlm_dmi_mode &dmi_mode,                ///< DMI mode
          tlm::tlm_dmi      &dmi_data                 ///< DMI data
  );
};
#endif /* __ADAPTER_UT_LT_H__ */
