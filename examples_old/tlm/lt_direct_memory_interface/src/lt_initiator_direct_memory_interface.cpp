/*****************************************************************************
 *
 *   The following code is derived, directly or indirectly, from the SystemC
 *   source code Copyright (c) 1996-2008 by all Contributors.
 *   All Rights reserved.
 *
 *   The contents of this file are subject to the restrictions and limitations
 *   set forth in the SystemC Open Source License Version 3.0 (the "License");
 *   You may not use this file except in compliance with such restrictions and
 *   limitations. You may obtain instructions on how to receive a copy of the
 *   License at http://www.systemc.org/. Software distributed by Contributors
 *   under the License is distributed on an "AS IS" basis, WITHOUT WARRANTY OF
 *   ANY KIND, either express or implied. See the License for the specific
 *   language governing rights and limitations under the License.
 *
 *   *****************************************************************************/

/* ---------------------------------------------------------------------------------------
 @file lt_initiator_direct_memory_interface.h
 
 @brief LT initiator with temporal decoupling and direct memory interface
 
  Original Authors:
    Bill Bunton, ESLX
    Charles Wilson, ESLX
    
--------------------------------------------------------------------------------------- */

//==============================================================================

#include "reporting.h"                                ///< report output
#include "lt_initiator_direct_memory_interface.h"     ///< LT initiator with temporal decoupling
#include "tlm.h"                                      ///< TLM headers

using namespace sc_core;
using namespace std;

static const char *filename = "lt_initiator_direct_memory_interface.cpp"; ///< filename for reporting

/*=============================================================================
  @fn lt_initiator::lt_initiator
  
  @brief lt_initiator constructor
  
  @details
    This routine initialized an lt_initiator class instance.
    
  @param name module name
  @param ID   initiator ID
  @param seed random number generator seed
  
  @retval none
=============================================================================*/
lt_initiator::lt_initiator                          ///< constructor
( sc_module_name      name                          ///< module name
, const unsigned int  ID                            ///< initiator ID
, const unsigned long seed                          ///< random number generator seed
) 
: sc_module         (name)                          ///< module name
, m_ID              (ID)                            ///< initiator ID
, m_seed            (seed)                          ///< random number generator seed
, initiator_socket  ("initiator_socket")            ///< initiator socket
, m_DMI_active      (false)                         ///< DMI pointer available
{ 
  initiator_socket (*this);                         // bind initiator to the socket

  SC_THREAD(initiator_thread);                      // register thread process 

  tlm::tlm_quantumkeeper::set_global_quantum(sc_core::sc_time(500, sc_core::SC_NS));
  m_QuantumKeeper.reset();
}

/*=============================================================================
  @fn lt_initiator::~lt_initiator
  
  @brief lt_initiator constructor
  
  @details
    This routine terminates an lt_initiator class instance.
    
  @param name module name
  
  @retval none
=============================================================================*/
lt_initiator::~lt_initiator                         ///< destructor
( void
)
{
}  

/*=============================================================================
  @fn lt_initiator::initiator_thread
  
  @brief outbound data processing
  
  @details
    This routine handles outbound data.
    
  @param none
  
  @retval none
=============================================================================*/

void
lt_initiator::initiator_thread                      ///< initiator thread
( void
)
{  
  tlm::tlm_generic_payload *transaction_ptr;        ///< transaction pointer
  std::ostringstream msg;                           ///< log message

  while (true)
  {
    // get transaction GP from reques in FIFO 
    transaction_ptr = request_in_port->read();      // get request from input fifo 

    // setup request 
    tlm::tlm_phase phase  = tlm::BEGIN_REQ;         // Create phase objects 
    sc_time delay         = SC_ZERO_TIME;           // Create delay objects
    
    msg.str ("");
    msg << m_ID << " "
        << ((transaction_ptr->get_command () == tlm::TLM_READ_COMMAND) ? "R" : "W")
        << " A: 0x" << internal << setw( sizeof(transaction_ptr->get_address ()) * 2 ) << setfill( '0' ) 
        << uppercase << hex << transaction_ptr->get_address ()
        << " L: " << internal << setw( 2 ) << setfill( '0' ) << dec << transaction_ptr->get_data_length ();

    REPORT_INFO (filename, __FUNCTION__, msg.str() );
    
    //DMI call may modify address. Keep a copy.
	sc_dt::uint64 address   = transaction_ptr->get_address();

    // see if we've already gotten a DMI pointer for this range
    if ( ! m_DMI_active )
    {
      tlm::tlm_command command = transaction_ptr->get_command();
      // see if we can acquire a DMI pointer
      transaction_ptr->set_write();
      m_DMI_active    = initiator_socket->get_direct_mem_ptr ( *transaction_ptr
                                                              , m_DMI_read_write );
      
      transaction_ptr->set_address(address);
      transaction_ptr->set_command(command);

      if ( m_DMI_active )
      {
        msg.str ("");
        msg << m_ID << " - " << "got a DMI pointer";
        
        REPORT_INFO (filename, __FUNCTION__, msg.str() );
      }
      else
      {
        msg.str ("");
        msg << m_ID << " - " << "can't get a DMI pointer";
        
        REPORT_INFO (filename, __FUNCTION__, msg.str() );
      }
    }
    
    // see if we have a DMI pointer and the address is in it's range
    if (    ( m_DMI_active )
        &&  ( address >= m_DMI_read_write.get_start_address ())
        &&  ( address <= m_DMI_read_write.get_end_address ()) )
    {
      msg.str ("");
      msg << m_ID << " - " << "bypassing target";
      
      REPORT_INFO (filename, __FUNCTION__, msg.str() );

      unsigned char *data     = transaction_ptr->get_data_ptr();    ///< data pointer
      unsigned int  length    = transaction_ptr->get_data_length(); ///< data length
      sc_dt::uint64 dmi_size  = m_DMI_read_write.get_end_address()
                              - m_DMI_read_write.get_start_address(); ///< DMI memory size
      sc_dt::uint64  offset   = address
                              - m_DMI_read_write.get_start_address(); ///< offset to data

      // process the command
      switch ( transaction_ptr->get_command () )
      {
        case tlm::TLM_READ_COMMAND:
        {
          msg.str ("");
          msg << m_ID << " - R* -";
          msg << " A: 0x" << internal << setw( sizeof(address) * 2 ) << setfill( '0' ) 
              << uppercase << hex << address;
          msg << " L: " << internal << setw( 2 ) << setfill( '0' ) << dec << length;

          // clear read buffer
          memset(data, 0x00, length);

          msg << " D: 0x";
          
          for (unsigned int i = 0; i < length; i++)
          {
            if ( ( offset + i ) >= dmi_size )
            {
              break;
            }

            data[i] = m_DMI_read_write.get_dmi_ptr() [offset + i];
             
            msg << internal << setw( 2 ) << setfill( '0' ) << uppercase << hex << (unsigned int)data[i];
         }
          
          REPORT_INFO(filename, __FUNCTION__, msg.str());
          
          // update time
          m_QuantumKeeper.inc (m_DMI_read_write.get_read_latency());

          break;
        }
      
        case tlm::TLM_WRITE_COMMAND:
        {
          msg.str ("");
          msg << m_ID << " - W* -";
          msg << " A: 0x" << internal << setw( sizeof(address) * 2 ) << setfill( '0' ) 
              << uppercase << hex << address;
          msg << " L: " << internal << setw( 2 ) << setfill( '0' ) << dec << length;
          msg << " D: 0x";
          
          for (unsigned int i = 0; i < length; i++)
          {
            msg << internal << setw( 2 ) << setfill( '0' ) << uppercase << hex << (int)data[i];
          }
          
          REPORT_INFO(filename, __FUNCTION__, msg.str());

          for (unsigned int i = 0; i < length; i++)
          {
            if ( ( offset + i ) >= dmi_size )
            {
              break;
            }

            m_DMI_read_write.get_dmi_ptr() [offset + i] = data[i];
          }
          
          // update time
          m_QuantumKeeper.inc (m_DMI_read_write.get_write_latency());

          break;
        }
        
        default:
        {
          msg.str ("");
          msg << m_ID << " - Invalid GP request";
          
          REPORT_FATAL (filename, __FUNCTION__, msg.str());
          
          break;
        }
      }
      
      msg.str ("");
      msg << m_ID << " - time updated";
      
      REPORT_INFO (filename, __FUNCTION__, msg.str());
      
      // see if it's time to synchronize the quantum keeper
      if (m_QuantumKeeper.need_sync())
      {
        msg.str ("");
        msg << m_ID << " - " << "Sync Local time to SC time";
        
        REPORT_INFO (filename, __FUNCTION__, msg.str() );
        
        m_QuantumKeeper.sync();
      }
      
      response_out_port->write(transaction_ptr);
      
      msg.str ("");
      msg << m_ID << " - Completed LT request" << endl;
      msg << "      SC time    = " << sc_core::sc_time_stamp() << endl;
      msg << "      local time = " << m_QuantumKeeper.get_current_time();
          
      REPORT_INFO (filename, __FUNCTION__, msg.str());
    }
    else
    {
      // make the non-blocking call and decode returned status (tlm_sync_enum) 
      switch (initiator_socket->nb_transport
              ( *transaction_ptr
              , phase
              , m_QuantumKeeper.get_local_time()))
      {
        case tlm::TLM_COMPLETED:                      // LT taget response complete with annonated delay 
        {
          if (m_QuantumKeeper.need_sync())
          {
            msg.str ("");
            msg << m_ID << " - " << "Sync Local time to SC time";
            
            REPORT_INFO (filename, __FUNCTION__, msg.str() );
            
            m_QuantumKeeper.sync();
          }
          
          response_out_port->write(transaction_ptr);
          
          msg.str ("");
          
          if (transaction_ptr->get_response_status() != tlm::TLM_OK_RESPONSE)
          {
            msg << m_ID << " operation failed";
          }
          else
          {
            msg << m_ID << " - Completed LT request" << endl;
            msg << "      SC time    = " << sc_core::sc_time_stamp() << endl;
            msg << "      local time = " << m_QuantumKeeper.get_current_time();
          }
          
          REPORT_INFO (filename, __FUNCTION__, msg.str());

          break;
        }

        case tlm::TLM_ACCEPTED:                       // LT target accepted request
        {
          m_req_accepted_queue.push(transaction_ptr); 
          wait (m_req_accepted_event); 

          msg.str ("");
          msg << m_ID << " - Completed LT request forced sync" << endl;
          msg << "      local time       = " << m_QuantumKeeper.get_local_time() << endl;

          m_QuantumKeeper.reset();

          msg << "      Reset local time = " << m_QuantumKeeper.get_local_time(); 

          REPORT_INFO (filename, __FUNCTION__, msg.str());

          break;
        }

        case tlm::TLM_UPDATED:
        default:
        {
          msg.str ("");
          msg << m_ID << " - invalid response";
          
          REPORT_FATAL(filename,  __FUNCTION__, msg.str());
          
          break;
        }
      }
    }
  }
}

/*=============================================================================
  @fn lt_initiator::nb_transport
  
  @brief inbound non-blocking transport
  
  @details
    This routine handles inbound non-blocking transport.
    
  @param transaction_ref  transaction reference
  @param phase            transaction phase reference
  @param delay            transaction delay reference
  
  @retval phase synchronization phase
=============================================================================*/

tlm::tlm_sync_enum                                  ///< phase
lt_initiator::nb_transport                          ///< nb_transport
( tlm::tlm_generic_payload  &transaction_ref        ///< transaction
, tlm::tlm_phase            &phase                  ///< phase
, sc_time                   &delay                  ///< delay
)
{
  std::ostringstream        msg;                    ///< log message
  tlm::tlm_sync_enum        return_status = tlm::TLM_COMPLETED;
  
  tlm::tlm_generic_payload  *trans_ptr;

  switch (phase)
  {
    case tlm::BEGIN_RESP:
    { 
      trans_ptr = m_req_accepted_queue.front();
      
      if (trans_ptr == &transaction_ref)            // LT target completing request
      {
        m_req_accepted_queue.pop();
        m_req_accepted_event.notify(SC_ZERO_TIME);  // release requester thread
        response_out_port->write(&transaction_ref);
        
        msg.str ("");
        msg << m_ID << " - LT target completing request";
        
        REPORT_INFO (filename,  __FUNCTION__, msg.str());
        
        return_status = tlm::TLM_COMPLETED;
      }
      else
      {
        msg.str ("");
        msg << m_ID << " - transaction ptr not valid BEGIN_RESP";
        
        REPORT_FATAL(filename, __FUNCTION__, msg.str());
      }
      break;
    } 

    case tlm::END_REQ: 
    case tlm::BEGIN_REQ: 
    case tlm::END_RESP:
    {
      msg.str ("");
      msg << m_ID << " - phase undefined";
        
      REPORT_FATAL(filename, __FUNCTION__, msg.str());
      
      break;
    }

    default:
    {                     
      msg.str ("");
      msg << m_ID << " - phase not implemented";
      
      REPORT_FATAL(filename, __FUNCTION__, msg.str());
      
      break;
    }
  }
  
  return return_status;
}

/*=============================================================================
  @fn lt_initiator::invalidate_direct_mem_ptr
  
  @brief invalidate DMI pointer
  
  @details
    This routine invalidates a DMI pointer.
    
  @param name module name
  
  @retval none
=============================================================================*/

void
lt_initiator::invalidate_direct_mem_ptr             ///< invalidate_direct_mem_ptr
( sc_dt::uint64 start_range                         ///< start range
, sc_dt::uint64 end_range                           ///< end range
)
{
  std::ostringstream        msg;                    ///< log message

  if (    ( m_DMI_active )
      &&  ( m_DMI_read_write.get_start_address  ()>= start_range  )
      &&  ( m_DMI_read_write.get_start_address  ()<= end_range    )
      &&  ( m_DMI_read_write.get_end_address    ()>= start_range  )
      &&  ( m_DMI_read_write.get_end_address    ()<= end_range    ) )
  {
    msg.str ("");
    msg << m_ID << " - DMI pointer invalidated";
    
    REPORT_INFO(filename, __FUNCTION__, msg.str());
    
    m_DMI_active = false;
  }
}
