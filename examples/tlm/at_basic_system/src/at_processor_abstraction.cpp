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
/*******************************************************************************
  @file at_processor_abstraction.cpp
  @Details
    This module instantiates the traffic generator and protocol initiator. It
    is responsible for binding the appropriate sockets and ports.
 
 ******************************************************************************/

/*****************************************************************************
  Original Authors:
    Bill Bunton, ESLX
    Anna Keist, ESLX
*****************************************************************************/

// Note: order of headers is important
#include "tlm.h"                               ///< TLM header
#include "traffic_generator.h"                 ///< TLM traffic generator
#include "at_initiator_4_phase.h"              ///< AT initiator
#include "at_processor_abstraction.h"          ///< AT processor abstraction


/*==============================================================================
  @fn at_processor_abstraction::at_processor_abstraction

  @brief class constructor

  @details
    This is the class constructor.
    initializes:
    1. AT processor abstraction socket (bind to initiator module's socket)
    2. AT initiator (TLM2 AT 4-phase protocol initiator)
    3. Traffic generator (Generate GPs to initiator)

  @param module_name module name
  @param ID (used for reporting
  @param seed (random number generator seed, passed to traffic generator)

  @retval void

  @note

==============================================================================*/

/*=============================================================================
  @fn at_processor_abstraction::at_processor_abstraction
  
  @brief at_processor_abstraction constructor
  
  @details
    This routine initialized an at_processor_abstraction class instance.
    
  @param module_name          module name
  @param ID                   processor abstraction ID (used in reporting
  @param seed                 seed (for randomization)
  
  @retval none
=============================================================================*/
at_processor_abstraction::at_processor_abstraction ///< constructor
( sc_core::sc_module_name 	      name             ///< module name
, const unsigned int              ID               ///< initiator ID
, const unsigned int              seed             ///< random number generator seed
)
  : sc_module                                      ///< SC base
    ( name                                         ///< module name
    )

  , at_processor_abstraction_socket
    ( "at_processor_abstraction_socket"            ///< socket name
    )

  , m_ID                           
    ( ID                                           ///< processor ID (for reporting)
    )

  , m_seed                         
    ( seed                                         ///< seed value 
    )

  , m_initiator                     
    ( "m_initiator_4_phase"                        ///< initiator name
    , ID                                           ///< set initiator ID to processor ID
    , sc_core::sc_time(7, sc_core::SC_NS)          ///< set initiator end rsp delay
    )

  , m_traffic_generator                           
    ( "m_traffic_generator"                        ///< traffic generator name
    , ID                                           ///< set traffic generator ID to processor ID
    , seed)                                        ///< set seed value
{

/*=============================================================================
 Bind internal ports
 ------------------  request_fifo   -----------
 |                 | ------------>  |         |
 |Traffic Generator|         	      |Initiator|
 |                 | response_fifo  |         |
 |                 | <------------  |         |
 ------------------                 -----------

==============================================================================*/
  m_traffic_generator.request_out_port  (m_request_fifo);
  m_initiator.request_in_port   (m_request_fifo);

  m_initiator.response_out_port (m_response_fifo);
  m_traffic_generator.response_in_port  (m_response_fifo);

/*=============================================================================
  Do initiator-socket to initiator-socket hierarchical binding 

  processor_abstraction
  -------------------------------------------------
  | -------------------       ---------------     |
  | |                 | ----> |            _|    _| processor abstraction
  | |traffic generator|       |initiator  [_]---[_]------> socket
  | |                 | <---- |             |     |
  | -------------------       ---------------     |
  -------------------------------------------------

==============================================================================*/
  m_initiator.initiator_socket(at_processor_abstraction_socket); 
}

/*==============================================================================
  @fn at_processor_abstraction::~at_processor_abstraction

  @brief class destructor

  @details
    This routine handles the class cleanup.

  @param void

  @retval void
==============================================================================*/
  at_processor_abstraction::~at_processor_abstraction(void) {   ///< destructor
 
}
