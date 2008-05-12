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

/*****************************************************************************
  Original Authors:
    Bill Bunton, ESLX
    Charles Wilson, ESLX
*****************************************************************************/

#ifndef __ADAPTER_TLM_2_1_H__ 
#define __ADAPTER_TLM_2_1_H__

#include "tlm.h"                                      ///< TLM headers
//#include "bus_types.h"
#include "basic_initiator_port.h"

typedef unsigned int ADDRESS_TYPE;
typedef unsigned int DATA_TYPE;

using basic_protocol::basic_initiator_port;


/*==============================================================================
  @file adapter_tlm_2_1.h
  
  $Id: adapter_tlm_2_1.h 251 2007-11-16 15:56:46Z wilsoncj $ 
  
  @brief Loosely Timed (LT) TLM memory target

  @Details
    This class provides a tlm_slave_socket for the initiator to bind.
    It implements the tlm_fw_transport_if to provide master to slave
    communications.

  @Note
    1. Out of range write request complete normally and do not change memory
       state 
    2. Out of range read request return zero, complete normally and do not change 
       memory state.  

===============================================================================*/

class adapter_tlm_2_1                                  ///< LT target memory
  :         public sc_core::sc_module                  ///< module base clase
  , virtual public tlm::tlm_fw_transport_if<>          ///< target socket
{
 
//==============================================================================
// sockets and ports 
//==============================================================================
  public:
  tlm::tlm_target_socket<>  adapter_socket;           ///< TLM 2 target socket

  basic_initiator_port<ADDRESS_TYPE,DATA_TYPE>        /// tlm 1 initiator port 
    tlm_1_initiator_port;                     

//==============================================================================
// constructors / destructor 
//==============================================================================
public:
  adapter_tlm_2_1 (                       ///< constructor
    sc_core::sc_module_name name          ///< name 
    );
  
  ~adapter_tlm_2_1(void);                 ///< destructor
  


//==============================================================================
// TLM2 non blocking membor function 
//==============================================================================
  
  public:
  tlm::tlm_sync_enum                          ///< sync status
  nb_transport(                               ///< non-blocking transport
      tlm::tlm_generic_payload &gp            ///< generic payoad pointer
    , tlm::tlm_phase           &phase         ///< transaction phase
    , sc_core::sc_time         &delay_time    ///< time it should take for transport
  );

//-----------------------------------------------------------------------------
/// @note transport_dbg() not implmented
//-----------------------------------------------------------------------------
  unsigned int                                ///< result
  transport_dbg(                              ///< transport debug
    tlm::tlm_generic_payload   &payload       ///< generic payload
  );

//-----------------------------------------------------------------------------  
/// @note transport_dbg() not implmented
//-----------------------------------------------------------------------------  
  bool                                        ///< success / failure
  get_direct_mem_ptr(                         ///< get direct memory pointer
    tlm::tlm_generic_payload   &payload,      ///< address + extensions
    tlm::tlm_dmi               &dmi_data      ///< dmi data
  );


 //==============================================================================
 // member variables 
 //==============================================================================
  
 
}; 

#endif /* __ADAPTER_TLM_2_1_H__ */
