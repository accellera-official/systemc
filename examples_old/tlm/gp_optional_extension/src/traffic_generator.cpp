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

#define SUPPRESS_INFO

#ifdef  SUPPRESS_INFO
#undef  REPORT_INFO
#define REPORT_INFO(x,y,z)
#else   /* SUPPRESS_INFO */
static char *filename = "traffic_generator.cpp";
#endif  /* SUPPRESS_INFO */

/*=============================================================================
  @fn traffic_generator::traffic_generator
  
  @brief traffic_generator constructor
  
  @details
    This routine initialized a traffic_generator class instance.
    
  @param name           module name
  @param ID             initiator ID
  @param seed           random number generator seed
  @param message_count  number of messages to generate
  
  @retval none
=============================================================================*/
traffic_generator::traffic_generator                ///< constructor 
( sc_core::sc_module_name name                      ///< module name
, const unsigned int      ID                        ///< initiator ID
, const unsigned long     seed                      ///< random number generator seed
, const unsigned int      message_count             ///< number of messages to generate
)
: sc_module       (name)                            ///< module name
, m_ID            (ID)                              ///< initiator ID
, m_seed          (seed)                            ///< random number generator seed
, m_message_count (message_count)                   ///< number of messages to generate
{
  
  m_read_buffer   = new unsigned char [buffer_size];
  m_write_buffer  = new unsigned char [buffer_size];
  
  // initialize the gp extension string type
  m_initiator_id << endl << " ******* INITIATOR "
                 << m_ID
                 << " MADE A REQUEST *******";

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
    
  message.str ("");
  message << m_ID << " - random number seed: 0x" << internal << setw( sizeof(m_seed) * 2 ) << setfill( '0' ) 
          << uppercase << hex << m_seed;
  REPORT_INFO(filename, __FUNCTION__, message.str());

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

  m_queue_depth   = request_out_port->num_free();

  unsigned int  chunks = m_message_count / m_queue_depth;                   ///< number of queue fills
  unsigned int  excess = m_message_count - ((chunks - 1) * m_queue_depth);  ///< size of last fill
  unsigned int  command_queued    = 1;                                      ///< queued counter
  unsigned int  command_dequeued  = 1;                                      ///< dequeued counter
  
  // process the chunks
  for (unsigned int chunk = chunks; chunk; chunk--)
  {
    // queue commands  
    for (unsigned int count = (chunk != 1) ? m_queue_depth : excess; count; count--)
    {
      message.str ("");
      message << m_ID << " - command " << dec << command_queued++ << " queued";
      
      REPORT_INFO(filename,__FUNCTION__, message.str());
      
      tlm::tlm_generic_payload  *transaction  = new tlm::tlm_generic_payload;
      bool                      command_write = irand() % 2;
      
      transaction->set_command          (command_write ? tlm::TLM_WRITE_COMMAND : tlm::TLM_READ_COMMAND);
      transaction->set_address          ((irand() % 4096) | ((irand() % 2) ? 0x10000000 : 0x00000000));
      transaction->set_data_ptr         (command_write ? m_write_buffer : m_read_buffer);
      transaction->set_data_length      (buffer_size);
      transaction->set_response_status  (tlm::TLM_INCOMPLETE_RESPONSE);

      // set the extension
      gp_extension::my_initiator_id_extension  *m_initiator_id_ext = 
        new gp_extension::my_initiator_id_extension;    // extension
      std::ostringstream initiator_id;
      m_initiator_id_ext->m_initiator_ID = m_initiator_id.str();
      transaction->set_extension        (m_initiator_id_ext); // register the extension

      request_out_port->write (transaction);
    }
    
    // dequeue responses
    for (unsigned int count = (chunk != 1) ? m_queue_depth : excess; count; count--)
    {
      message.str ("");
      message << m_ID << " - response " << dec << command_dequeued++ << " dequeued";
      
      REPORT_INFO(filename,__FUNCTION__, message.str());
      
      tlm::tlm_generic_payload *transaction;
      gp_extension::my_initiator_id_extension *m_initiator_id_ext;
      
      response_in_port->read(transaction);
      transaction->get_extension(m_initiator_id_ext);

      delete m_initiator_id_ext;
      delete transaction;
    }
  }

  wait (sc_core::SC_ZERO_TIME);
}
