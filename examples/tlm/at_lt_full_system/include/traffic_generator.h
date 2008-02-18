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
 @file traffic_generator.h
 
 @brief traffic_generator class header
 
  Original Authors:
    Bill Bunton, ESLX
    Charles Wilson, ESLX
    
--------------------------------------------------------------------------------------- */

#ifndef __TRAFFIC_GENERATOR_H__
#define __TRAFFIC_GENERATOR_H__

#include "tlm.h"                                    ///< TLM headers

const unsigned int  buffer_size = 16;               ///< buffer size

class traffic_generator                             ///< traffic_generator
: public sc_core::sc_module                         ///< sc_module
{
  SC_HAS_PROCESS(traffic_generator);

  // ports, exports and sockets

  typedef tlm::tlm_generic_payload  *gp_ptr; 
  
  public:

  sc_core::sc_port<sc_core::sc_fifo_out_if <gp_ptr> > request_out_port; 
  sc_core::sc_port<sc_core::sc_fifo_in_if  <gp_ptr> > response_in_port;

  // Constructors / Destructor 

  public:
  
  traffic_generator                                 ///< constructor
  ( sc_core::sc_module_name name                    ///< module name
  , const unsigned int      ID                      ///< initiator ID
  , const unsigned long     seed                    ///< random number generator seed
  , const unsigned int      message_count           ///< number of messages to generate
  );
  
  ~traffic_generator                                ///< destructor
  ( void
  ); 

  // Private member variables and methods

  private:
  
  const unsigned int  m_ID;                         ///< initiator ID
  const unsigned long m_seed;                       ///< random number generator seed
  const unsigned int  m_message_count;              ///< number of messages to generate
  
  unsigned char       *m_read_buffer;               ///< read buffer 
  unsigned char       *m_write_buffer;              ///< write buffer
  unsigned int        m_queue_depth;                ///< sc_fifo depth
  
  void
  traffic_generator_thread                          ///< traffic_generator_thread
  ( void
  ); 
};

#endif /* __TRAFFIC_GENERATOR_H__ */
