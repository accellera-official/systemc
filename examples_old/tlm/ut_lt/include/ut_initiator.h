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
 @file ut_initiator.h
 
 @brief UT initiator class header
 
  Original Authors:
    Charles Wilson, ESLX
    
--------------------------------------------------------------------------------------- */

#ifndef __UT_INITIATOR_H__
#define __UT_INITIATOR_H__

#include "tlm.h"                                      ///< TLM headers

class ut_initiator                                    ///< UT initiator class
:         public sc_core::sc_module                   ///< SC module
, virtual public tlm::tlm_bw_b_transport_if           ///< TLM backward transport interface
{
  public:
  
  typedef tlm::tlm_phase                  phase_type;
  typedef tlm::tlm_sync_enum              sync_enum_type;
  typedef tlm::tlm_fw_b_transport_if<>    fw_interface_type;
  typedef tlm::tlm_bw_b_transport_if      bw_interface_type;
  typedef tlm::tlm_initiator_socket<32,
                                    fw_interface_type,
                                    bw_interface_type>
                                          initiator_socket_type;

  public:
  
  initiator_socket_type socket;                       ///< socket

  SC_HAS_PROCESS(ut_initiator);
  
  ut_initiator                                        ///< constructor
  ( sc_core::sc_module_name name                      ///< module name
  , const unsigned int      ID                        ///< initiator ID
  , const unsigned int      seed                      ///< random number generation seed
  );
  
  ~ut_initiator                                       ///< destructor
  ( void
  );

  bool                                                ///< command generated (true/false)
  setup_transaction                                   ///< setup_transaction
  ( tlm::tlm_generic_payload  &trans                  ///< transaction
  );

  void
  report_start                                        ///< report_start
  ( tlm::tlm_generic_payload  &trans                  ///< transaction
  );

  void
  report_end                                          ///< report_end
  ( tlm::tlm_generic_payload  &trans                  ///< transaction
  );

  void
  initiator_thread                                    ///< initiator_thread
  ( void
  );

  void
  invalidate_direct_mem_ptr                           ///< invalidate_mem_ptr
  ( sc_dt::uint64 start_range                         ///< start invalidation region
  , sc_dt::uint64 end_range                           ///< end invalidation region
  );

  private:
  
  const unsigned int      m_ID;                       ///< inititator ID
  const unsigned int      m_seed;                     ///< random number generation seed
        sc_core::sc_event mEndEvent;                  ///< end event
        unsigned int      mBaseAddress;               ///< base address
        unsigned int      mNrOfTransactions;          ///< transaction count
        unsigned int      mTransactionCount;          ///< transaction counter
        unsigned char     *m_data;                    ///< data pointer
};

#endif  /* __UT_INITIATOR_H__ */
