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

#include "lt_target_extension_mandatory.h"

using namespace sc_core;

static const char *filename = "lt_target_extension_mandatory.cpp";  ///  filename for reporting

lt_target_extension_mandatory::lt_target_extension_mandatory    ///< constructor
( sc_module_name  name                                          ///< instance name
, sc_time         invalidate_dmi_time = sc_time(25, SC_NS)      ///< DMI invalidation timeout
)
: sc_module       ( name                    )                   ///< module
, m_socket        ( "LT_target_mandatory"   )                   ///< socket
, m_latency_read  ( sc_time ( 100, SC_NS )  )                   ///< read latency
, m_latency_write ( sc_time (  10, SC_NS )  )                   ///< write latency
{
  // register interface methods
  
  m_socket.register_nb_transport_fw    ( this, &lt_target_extension_mandatory::nb_transport_fw );
  m_socket.register_get_direct_mem_ptr ( this, &lt_target_extension_mandatory::get_dmi_ptr     );
  m_socket.register_transport_dbg      ( this, &lt_target_extension_mandatory::transport_dbg   );

  // prime the DMI timeout mechanism

  SC_METHOD ( invalidate_dmi_method );
  
  sensitive << m_invalidate_dmi_event;
  
  dont_initialize();
  
  m_invalidate_dmi_time = invalidate_dmi_time;
}

tlm::tlm_sync_enum                                              ///< response
lt_target_extension_mandatory::nb_transport_fw                  ///< non-blocking forward transport
( transaction_type  &trans                                      ///< transaction
, phase_type        &phase                                      ///< phase
, sc_time           &t                                          ///< time
)
{
  ostringstream  msg;                                        ///< log message

  assert ( phase == tlm::BEGIN_REQ );
  
  // check for extension

  extension_initiator_id  *extension_ptr;
  
  trans.get_extension ( extension_ptr );
  
  msg.str("");

  msg << "Extension ";
  
  if ( extension_ptr == 0 )
  {
    msg << "not present - ERROR";
    
    REPORT_FATAL ( filename,  __FUNCTION__, msg.str () );
  }
  else
  {
    msg << "present, Data: " << extension_ptr->m_initiator_id;
  
    REPORT_INFO ( filename,  __FUNCTION__, msg.str () );
    
    sc_dt::uint64   address = trans.get_address();
    unsigned char * data    = trans.get_data_ptr();
    
    assert ( address <= m_max_address );
  
    msg.str ( "" );
        
    switch ( trans.get_command () )
    {
      case tlm::TLM_WRITE_COMMAND:
      {
        // write command
        
        msg << "Write request - "
            << "Addr: 0x" << internal << setw ( sizeof(sc_dt::uint64) * 2 ) 
            << setfill( '0' ) << uppercase << hex << trans.get_address ();

        memcpy( &m_memory [ address ], data, sizeof(unsigned int) );
        t += sc_time(10, SC_NS);
        
        break;
      }
    
      case tlm::TLM_READ_COMMAND:
      {
        // read command
        
        msg << "Read request - "
            << "Addr: 0x" << internal << setw ( sizeof(sc_dt::uint64) * 2 ) 
            << setfill( '0' ) << uppercase << hex << trans.get_address ();
    
        memcpy( data, &m_memory [ address ], sizeof(unsigned int) );
        t += sc_time(100, SC_NS);
      
        break;
      }
      
      default:
      {
        // unknown command
        
        msg << "Unknown request";
    
        REPORT_FATAL ( filename,  __FUNCTION__, msg.str () );
        
        break;
      }
    }
      
    REPORT_INFO ( filename,  __FUNCTION__, msg.str () );
    
    trans.set_response_status ( tlm::TLM_OK_RESPONSE );
    trans.set_dmi_allowed     ( true                 );
  }

  // return completed, no phase update needed
  
  return tlm::TLM_COMPLETED;
}

unsigned int                                                    ///< byte count
lt_target_extension_mandatory::transport_dbg                    ///< debug transport
( transaction_type  &transaction_ref                            ///< transaction
)
{
  unsigned int num_bytes  = 0;
  unsigned int address    = (unsigned int) transaction_ref.get_address();
  
  if ( address <= m_max_address )
  {
    num_bytes = transaction_ref.get_data_length ();
    
    if ( ( address + num_bytes ) > m_max_address)
    {
      num_bytes = m_max_address - address + 1;
    }
    
    if ( transaction_ref.is_read () )
    {
      // read operation
      
      memcpy ( transaction_ref.get_data_ptr(), &m_memory [ address ], num_bytes );
    }
    else
    {
      // write operation
      
      memcpy ( &m_memory [ address ], transaction_ref.get_data_ptr(), num_bytes );
    }
  }
  
  return num_bytes;
}

bool                                                            ///< DMI granted
lt_target_extension_mandatory::get_dmi_ptr                      ///< get DMI pointer
( transaction_type  &trans                                      ///< transaction
, tlm::tlm_dmi      &dmi_properties                             ///< DMI properties
)
{
  bool           dmi_granted ( false );                         ///< DMI granted flag
  ostringstream  msg;                                           ///< log message
  
  // setup a timed event to invalidate the DMI pointer
  
  m_invalidate_dmi_event.notify ( m_invalidate_dmi_time );

  // check for extension
  
  extension_initiator_id *extension_ptr;
  
  trans.get_extension ( extension_ptr );
  
  msg.str ( "" );
  
  msg << "Extension ";
  
  if ( extension_ptr == 0 )
  {
    msg << "not present - ERROR";
    
    REPORT_FATAL ( filename,  __FUNCTION__, msg.str () );
  }
  else
  {
    msg << "present, Data: " << extension_ptr->m_initiator_id;
  
    REPORT_INFO ( filename,  __FUNCTION__, msg.str () );
    
    if (trans.get_address() <= m_max_address)
    {
      dmi_properties.allow_read_write   (                 );
      dmi_properties.set_start_address  ( m_min_address   );
      dmi_properties.set_end_address    ( m_max_address   );
      dmi_properties.set_dmi_ptr        ( m_memory        );
      dmi_properties.set_read_latency   ( m_latency_read  );
      dmi_properties.set_write_latency  ( m_latency_write );
  
      dmi_granted = true;
    }
    else
    {
      // should not happen
  
      dmi_properties.set_start_address  ( trans.get_address () );
      dmi_properties.set_end_address    ( trans.get_address () );
    }
 }
  
  return dmi_granted;
}

void
lt_target_extension_mandatory::invalidate_dmi_method            ///< invalidate DMI method
( void
)
{
    m_socket->invalidate_direct_mem_ptr ( m_min_address, m_max_address );
}
