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
 * @file
 *  traffic_generator.h
 *  $Id:$
 * @brief
 * This class uses a thread to generate traffic.
 * @Details
 * The following transactions can be generated.
 * - 1. i-cache reads (block reads of cache line size)
 * - 2. d-cache reads and writes (block reads and writes of cache line size)
 * - 3. reads and writes (using mixed sizes of 1, 2, 4 and 8 bytes)
 * The number of active requests may be default  8.
 * A traffic_generator_config object must be initialized and passed to the 
 * constructor to configure the icache, dcache and scratch regions available
 * and the percentage of reads/writes for each region.
 * 
 * Notes:
 * - portion of traffic i-cache reads        default 78% (integral percents)
 * - portion of traffic d-cache reads        default 15%
 * - portion of traffic d-cache writes       default  4%
 * - portion of traffic data reads           default  2%
 * - portion of traffic data writes          default  1%
 ******************************************************************************/

/*****************************************************************************
  Original Authors:
    Bill Bunton, ESLX
    Charles Wilson, ESLX
*****************************************************************************/

#ifndef __TRAFFIC_GENERATOR_H__
#define __TRAFFIC_GENERATOR_H__

#include "tlm.h"                                      ///< TLM headers

//==============================================================================
// struct traffic_generator_config
//
// This structure provides all of the information required to configure the
// traffic generator. It must be initiatilzed and passed to the traffic generator
// constructor.
//
// Note: the icache, dcache and scratch areas should be 4K aligned.
//==============================================================================

struct traffic_generator_config {
  public:
    unsigned int  m_cache_line_size;        ///< cache line size (64, 128 or 256)
    
    sc_dt::uint64 m_icache_base_address;    ///< i-cache base address
    sc_dt::uint64 m_icache_area;            ///< i-cache area in bytes
    unsigned int  m_icache_read_percent;    ///< i-cache reads percentage
    
    sc_dt::uint64 m_dcache_base_address;    ///< d-cache base address
    sc_dt::uint64 m_dcache_area;            ///< d-cache area in bytes
    unsigned int  m_dcache_read_percent;    ///< d-cache reads percentage
    unsigned int  m_dcache_write_percent;   ///< d-cache writes percentage
    
    sc_dt::uint64 m_scratch_base_address;   ///< scratch base address
    sc_dt::uint64 m_scratch_area;           ///< scratch area in bytes
    unsigned int  m_scratch_read_percent;   ///< scratch reads percentage
    unsigned int  m_scratch_write_percent;  ///< scratch writes percentage
};

class traffic_generator                     ///< module base class
: public sc_core::sc_module
{
  SC_HAS_PROCESS(traffic_generator);

  public:

  sc_core::sc_port<examples_rw_if> rw_port; ///< socket read/write port 

//==============================================================================
// Constructors & Destructor 
//
// Constructor: The traffic generator configuration object must be initialized
// and passed into the constructor.
//==============================================================================
  public:
  traffic_generator                         ///< constructor
  ( sc_core::sc_module_name   name          ///< module name
  , traffic_generator_config  &config       ///< traffic generator configuration
  );
  
  ~traffic_generator(void);                 ///< destructor

//==============================================================================
// Private member variables and methods
//==============================================================================
  private:
  void traffic_generator_thread(void);      ///< thread that generats traffic

  traffic_generator_config m_config;        ///< configuration information

};
#endif /* __TRAFFIC_GENERATOR_H__ */
