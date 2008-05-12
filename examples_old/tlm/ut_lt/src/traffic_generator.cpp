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
 @file traffic_generator.cpp
 
 @brief traffic generation routines
 
  Original Authors:
    Bill Bunton, ESLX
    Charles Wilson, ESLX
    
--------------------------------------------------------------------------------------- */

#include "reporting.h"                                ///< report output
#include "traffic_generator.h"                        ///< traffic generator
#include "mtrand.h"                                   ///< Mersenne Twister random number generator
#include "tlm.h"                                      ///< TLM headers

using namespace std;

//static const char *filename = "traffic_generator.cpp";  ///< filename for reporting

/*=============================================================================
  @fn traffic_generator::traffic_generator
  
  @brief traffic_generator constructor
  
  @details
    This routine initialized a traffic_generator class instance.
    
  @param name module name
  @param ID   initiator ID
  @param seed random number generator seed
  
  @retval none
=============================================================================*/
traffic_generator::traffic_generator                ///< constructor 
( sc_core::sc_module_name name                      ///< module name
, const unsigned int      ID                        ///< initiator ID
, const unsigned long     seed                      ///< random number generator seed
)
: sc_module (name)                                  ///< module name
, m_ID      (ID)                                    ///< initiator ID
, m_seed    (seed)                                  ///< random number generator seed
{
  m_read_buffer   = new unsigned char [buffer_size];
  m_write_buffer  = new unsigned char [buffer_size];
  
  SC_THREAD(traffic_generator_thread);
}

/*=============================================================================
  @fn traffic_generator::~traffic_generator
  
  @brief traffic_generator destructor
  
  @details
    This routine terminates a traffic_generator class instance.
    
  @param none
  
  @retval none
=============================================================================*/
traffic_generator::~traffic_generator               ///< destructor
( void
)
{
  // clean up
  delete[] m_read_buffer;
  delete[] m_write_buffer;
}

/*=============================================================================
  @fn traffic_generator::traffic_generator_thread
  
  @brief traffic_generator processing thread
  
  @details
    This routine generates traffic.
    
  @param none
  
  @retval none
=============================================================================*/
void
traffic_generator::traffic_generator_thread         ///< traffic_generator_thread
( void
)
{
  std::ostringstream  message;                      ///< log message
  unsigned int        write_count = 0;              ///< write counter
  
  message.str ("");
  message << m_ID << " - random number seed: 0x" << internal << setw( sizeof(m_seed) * 2 ) << setfill( '0' ) 
          << uppercase << hex << m_seed;
//  REPORT_INFO(filename, __FUNCTION__, message.str());

  // set random generator seed
  unsigned long init[4]       = { ( m_seed >>  0 ) & 0x000000FF
                                , ( m_seed >>  8 ) & 0x000000FF
                                , ( m_seed >> 16 ) & 0x000000FF
                                , ( m_seed >> 24 ) & 0x000000FF
                                };
  unsigned long length        = 4;
  
  MTRand_int32 irand(init, length);                  ///< 32-bit integer random number generator

  // clear the buffers
  memset ( m_write_buffer,  0, buffer_size );
  memset ( m_read_buffer,   0, buffer_size );
  
  // load the write buffer
  for ( unsigned int i = 0; i < buffer_size; i++ )
  {
    m_write_buffer [ i ] = (unsigned char)(i);
  }

  // do 32 reads and writes (in some random combination)
  for ( unsigned int i = 0; i < 8; i++ )
  {
    if ( irand() & 0x00000001 )
    {
      // queue command
      
      message.str ("");
      message << m_ID << " - command queued";
      
//      REPORT_INFO(filename,__FUNCTION__, message.str());
      
      tlm::tlm_generic_payload  *transaction  = new tlm::tlm_generic_payload;
      bool                      command_write = irand() % 2;
      
      transaction->set_command          (command_write ? tlm::TLM_WRITE_COMMAND : tlm::TLM_READ_COMMAND);
      transaction->set_address          ((irand() % 4096) | ((irand() % 2) ? 0x10000000 : 0x00000000));
      transaction->set_data_ptr         (command_write ? m_write_buffer : m_read_buffer);
      transaction->set_data_length      (buffer_size);
      transaction->set_response_status  (tlm::TLM_INCOMPLETE_RESPONSE);

      request_out_port->write (transaction);
      
      write_count++;
    }
    else if ( write_count )
    {
      // dequeue response
      
      message.str ("");
      message << m_ID << " - response dequeued";
      
//      REPORT_INFO(filename,__FUNCTION__, message.str());
      
      tlm::tlm_generic_payload *transaction;
      
      response_in_port->read(transaction);
      
      delete transaction;
      
      write_count--;
    }
    else
    {
      // attempting to dequeue a response before issuing a command
      message.str ("");
      message << m_ID << " - attempted to dequeue response before issuing command";
      
//    REPORT_INFO(filename,__FUNCTION__, message.str());
    }
  }

  wait (sc_core::SC_ZERO_TIME);
}
