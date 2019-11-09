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

#include "lt_initiator_extension_mandatory.h"

using namespace sc_core;

static const char *filename = "lt_initiator_extension_mandatory.cpp";  ///  filename for reporting

lt_initiator_extension_mandatory::lt_initiator_extension_mandatory  ///< constructor
( sc_module_name name                                               ///< instance
, unsigned int            num_trans    = 5                          ///< number of transactions
, sc_dt::uint64           base_address = 0                          ///< base address
)
: sc_module               ( name                     )              ///< instance name
, m_socket                ( "LT_initiator_mandatory" )              ///< socket
, m_transaction_max       ( num_trans                )              ///< transaction maximum
, m_base_address          ( base_address             )              ///< base address
, m_transaction_count     ( 0                        )              ///< transaction count
{
  invalidate_dmi_properties ();
  
  // register transport methods
  
  m_socket.register_nb_transport_bw           ( this, &lt_initiator_extension_mandatory::nb_transport_bw           );
  m_socket.register_invalidate_direct_mem_ptr ( this, &lt_initiator_extension_mandatory::invalidate_direct_mem_ptr );
  
  // Initiator thread
  
  SC_THREAD ( initiator_thread );
}

bool                                                                ///< transaction created
lt_initiator_extension_mandatory::create_transaction                ///< create transaction
( transaction_type  &trans                                          ///< transaction
)
{
  static bool  new_transaction ( true );                            ///< new transaction flag
  
  if ( new_transaction )
  {
    // initialize DMI state:
    
    trans.set_dmi_allowed     ( false                     );
    trans.set_data_length     ( 4                         );
    trans.set_streaming_width ( trans.get_data_length ()  );
    
    if ( m_transaction_count < m_transaction_max )
    {
      trans.set_address ( m_base_address + ( 4 * m_transaction_count ) );
      
      m_data = m_transaction_count;
      
      trans.set_data_ptr  ( reinterpret_cast<unsigned char*>(&m_data) );
      trans.set_command   ( tlm::TLM_WRITE_COMMAND                    );
    }
    else if ( m_transaction_count < ( 2 * m_transaction_max ) )
    {
      trans.set_address ( m_base_address + ( 4 * ( m_transaction_count - m_transaction_max ) ) );
      
      m_data = 0;
      
      trans.set_data_ptr  ( reinterpret_cast<unsigned char*>(&m_data) );
      trans.set_command   ( tlm::TLM_READ_COMMAND                     );
    }
    else
    {
      new_transaction = false;
    }
    
    m_transaction_count++;
  }
  
  return new_transaction;
}

void
lt_initiator_extension_mandatory::log_start                         ///< log transaction start
( transaction_type  &trans                                          ///< transaction
)
{
  ostringstream  msg;                                               ///< log message
  
  msg.str ( "" );
  
  msg << "Creating " 
      << ( ( trans.get_command () == tlm::TLM_WRITE_COMMAND ) ? "write" : "read" )
      << " transaction - Addr: 0x" << internal << setw ( sizeof(sc_dt::uint64) * 2 ) 
      << setfill( '0' ) << uppercase << hex << trans.get_address ();
            
  if (trans.get_command() == tlm::TLM_WRITE_COMMAND)
  {
    msg << " Data: 0x" << internal << setw ( sizeof ( m_data ) * 2 ) 
        << setfill( '0' ) << uppercase << hex << m_data;
  }
  
  REPORT_INFO ( filename,  __FUNCTION__, msg.str () );
}

void
lt_initiator_extension_mandatory::log_end                           ///< log transaction end
( transaction_type  &trans                                          ///< transaction
)
{
  ostringstream msg;                                                ///< log message
  
  msg.str ( "" );
  
  msg << "Received ";

  if ( trans.get_response_status() != tlm::TLM_OK_RESPONSE )
  {
    msg << "invalid response " << trans.get_response_status ();
  }
  else
  {
    msg << "TLM_OK_RESPONSE";
    
    if (trans.get_command() == tlm::TLM_READ_COMMAND)
    {
      msg << ", Data: 0x" << internal << setw ( sizeof ( m_data ) * 2 ) 
          << setfill( '0' ) << uppercase << hex << m_data;
    }
  }
    
  REPORT_INFO ( filename,  __FUNCTION__, msg.str () );
}

void
lt_initiator_extension_mandatory::initiator_thread      ///< initiator thread
( void
)
{
  transaction_type  trans;                              ///< transaction
  phase_type        phase;                              ///< phase
  sc_time           t;                                  ///< time
  ostringstream     msg;                                ///< log message
  
  // make sure that our transaction has the proper extension:
  
  extension_initiator_id* extension_ptr = new extension_initiator_id ();
  
  extension_ptr->m_initiator_id = "generic ID";
  
  trans.set_extension ( extension_ptr );
  
  while ( create_transaction ( trans ) )
  {
    // initialize phase and time
    
    phase = tlm::BEGIN_REQ;
    t     = SC_ZERO_TIME;
    
    log_start ( trans );
    
    ///////////////////////////////////////////////////////////
    // DMI handling:
    // We use the DMI hint to check if it makes sense to ask for
    // DMI pointers. The pattern is:
    // - if the address is covered by a DMI region do a DMI access
    // - otherwise do a normal transaction
    //   -> check if we get a DMI hint and acquire the DMI pointers if it
    //      is set
    ///////////////////////////////////////////////////////////
    
    // Check if the address is covered by our DMI region
    if  (  ( trans.get_address () >= m_dmi_properties.get_start_address () )
        && ( trans.get_address () <= m_dmi_properties.get_end_address   () ) )
    {
      // We can handle the data here. As the logEndTransaction is
      // assuming something to happen in the data structure, we really
      // need to do this:
      
      trans.set_response_status(tlm::TLM_OK_RESPONSE);
      
      sc_dt::uint64 tmp = trans.get_address() - m_dmi_properties.get_start_address();
      
      if (trans.get_command() == tlm::TLM_WRITE_COMMAND)
      {
        *(unsigned int*)&m_dmi_properties.get_dmi_ptr()[tmp] = m_data;    
      }
      else
      {
        m_data = *(unsigned int*)&m_dmi_properties.get_dmi_ptr () [ tmp ];
      }
      
      // Do the wait immediately. Note that doing the wait here eats
      //  almost all the performance anyway, so we only gain something
      // if we're using temporal decoupling.
      
      if (trans.get_command() == tlm::TLM_WRITE_COMMAND)
      {
        wait ( m_dmi_properties.get_write_latency () );    
      }
      else
      {
        wait ( m_dmi_properties.get_read_latency () );
      }

      log_end ( trans );
    }
    else
    {
      // we need a full transaction
      switch (m_socket->nb_transport_fw(trans, phase, t))
      {
        case tlm::TLM_COMPLETED:
        {
          // Transaction Finished, wait for the returned delay
          wait(t);
          break;
        }
        
        case tlm::TLM_ACCEPTED:
        case tlm::TLM_UPDATED:
        {
          // Transaction not yet finished, wait for the end of it
          wait(mEndEvent);
          break;
        }
            
        default:
        {
          assert(0); exit(1);
        }
      }

      log_end ( trans );

      // Acquire DMI pointer if one is available:
      if (trans.is_dmi_allowed())
      {
        trans.set_write();
        
        dmi_type  tmp;
        
        if (m_socket->get_direct_mem_ptr(trans, tmp))
        {
          // FIXME: No support for separate read/write ranges
          assert(tmp.is_read_write_allowed());
          
          m_dmi_properties = tmp;
        }
      }
    }
  }
  
  delete extension_ptr;
  
  wait();  
}

tlm::tlm_sync_enum                                        ///< response
lt_initiator_extension_mandatory::nb_transport_bw         ///< non-blocking backward transport
( transaction_type & /*trans*/                            ///< transaction
, phase_type       & phase                                ///< phase
, sc_time          & t                                    ///< time
)
{
  tlm::tlm_sync_enum  response;                           ///< response
  ostringstream  msg;                                ///< log message
    
  switch (phase)
  {
    case tlm::END_REQ:
    {
      // Request phase ended
      response = tlm::TLM_ACCEPTED;
      
      break;
    }
    
    case tlm::BEGIN_RESP:
    {
      assert(t == SC_ZERO_TIME); // FIXME: can t != 0?
      
      mEndEvent.notify (t);
      
      // Not needed to update the phase if true is returned
      response = tlm::TLM_COMPLETED;
      
      break;
    }
    
    case tlm::BEGIN_REQ:
    case tlm::END_RESP:
    default:
    {
      // A target should never call nb_transport with these phases
      assert(0); exit(1);
    }
  }
  
  return response;
}

void
lt_initiator_extension_mandatory::invalidate_dmi_properties       ///< invalidate DMI properties
( void
)
{
  m_dmi_properties.set_start_address ( 1 );
  m_dmi_properties.set_end_address   ( 0 );
}

// Invalidate DMI pointer(s)

void
lt_initiator_extension_mandatory::invalidate_direct_mem_ptr
( sc_dt::uint64 start_range
, sc_dt::uint64 end_range
)
{
  ostringstream       msg;                     ///< log message
  
  msg.str ( "" );
  
  msg << "Received invalidate request - ";
      
  // see if request is in range
  
  if (  ( start_range <= end_range                             )
     && ( start_range <= m_dmi_properties.get_end_address   () )
     && ( end_range   >= m_dmi_properties.get_start_address () ) )
  {
    msg << "valid request";
      
    // invalidate the DMI properties
    
    invalidate_dmi_properties ();
  }
  else
  {
    msg << "not in our DMI address range";
  }

  REPORT_INFO ( filename,  __FUNCTION__, msg.str () );
}
