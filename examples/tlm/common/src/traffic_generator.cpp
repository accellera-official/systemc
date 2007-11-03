/*****************************************************************************

  The following code is derived, directly or indirectly, from the SystemC
  source code Copyright (c) 1996-2007 by all Contributors.
  All Rights reserved.

  The contents of this file are subject to the restrictions and limitations
  set forth in the SystemC Open Source License Version 2.4 (the "License");
  You may not use this file except in compliance with such restrictions and
  limitations. You may obtain instructions on how to receive a copy of the
  License at http://www.systemc.org/. Software distributed by Contributors
  under the License is distributed on an "AS IS" basis, WITHOUT WARRANTY OF
  ANY KIND, either express or implied. See the License for the specific
  language governing rights and limitations under the License.

 *****************************************************************************/
/*******************************************************************************
  @file traffic_generator.h

  @brief traffic generation class
  
 ******************************************************************************/
// Note: order of headers is important
#include "tlm.h"                                   ///< TLM headers
#include "examples_rw_if.h"                        ///< Convenience R/W interface 
#include "reporting.h"                             ///< Reporting convenience macros
#include "traffic_generator.h"                     ///< Our header

static char *report_source = "traffic_generator";

/*==============================================================================
  @fn traffic_generator::traffic_generator
  
  @brief traffic generator constructor
  
  @details
  
  @param name module name
  @param config traffic generator configuration
  
  @retval void
==============================================================================*/

traffic_generator::traffic_generator                ///< constructor
( sc_core::sc_module_name name,                     ///< module name
  traffic_generator_config& config                  ///< traffic generator configuration
)
  : sc_module(name)
  , m_config(config)
{
  SC_THREAD(traffic_generator_thread);
}
    
/*==============================================================================
  @fn traffic_generator::~traffic_generator
  
  @brief traffic generator destructor
  
  @details
  
  @param void
  
  @retval void
==============================================================================*/

traffic_generator::~traffic_generator(void)          ///< destructor
{
}

/*==============================================================================
  @fn traffic_generator::traffic_generator_thread
  
  @brief traffic generation thread
  
  @details
  
  @param void
  
  @retval void
  
  @details
    The following transactions can be generated.
    - 1. i-cache reads (block reads of cache line size)
    - 2. d-cache reads and writes (block reads and writes of cache line size)
    - 3. reads and writes (using mixed sizes of 1, 2, 4 and 8 bytes)
    The number of active requests may be default  8.
    A traffic_generator_config object must be initialized and passed to the 
    constructor to configure the text, stack and scratch regions available
    and the percentage of reads/writes for each region.

  @note
    1. traffic breakdown (integral percents)
      - i-cache reads     default 78%
      - d-cache reads     default 15%
      - d-cache writes    default  4%
      - data reads        default  2%
      - data writes       default  1%
==============================================================================*/

void 
traffic_generator::traffic_generator_thread(void)                     ///< traffic generator thread
{ 
  REPORT_INFO(report_source,  __FUNCTION__,"traffic_generator_thread: starting");

  sc_dt::uint64 ir = m_config.m_icache_read_percent;                  ///< 
  sc_dt::uint64 dr = ir + m_config.m_dcache_read_percent;             ///< 
  sc_dt::uint64 dw = dr + m_config.m_dcache_write_percent;            ///< 
  sc_dt::uint64 sr = dw + m_config.m_scratch_read_percent;            ///< 

  unsigned int  cache_line_size = m_config.m_cache_line_size;         ///< cache line size
  unsigned char *read_buffer    = new unsigned char[cache_line_size]; ///< read buffer
  unsigned char *write_buffer   = new unsigned char[cache_line_size]; ///< write buffer
  unsigned int  percentage;                                           ///< 
  sc_dt::uint64 address;                                              ///< raw address
  sc_dt::uint64 aligned_address_mask;                                 ///< cache aligned address mask
  sc_dt::uint64 cache_alligned_address;                               ///< cache aligned address

  for (unsigned int i=0; i < cache_line_size; i++)
  {
    write_buffer[i] = i;
  }

  // set randam generator seed
  srand (31415);

  while (true) {
    percentage              = rand() % 101;
    address                 = rand() % 4096;
    aligned_address_mask    = ~(cache_line_size - 1);
    cache_alligned_address  = address & aligned_address_mask; 

    memset(read_buffer, 0, cache_line_size);

    if (percentage <= ir)
    {
      // i-cache Read
      rw_port->read
        ( m_config.m_icache_base_address + (cache_alligned_address) 
        , cache_line_size
        , read_buffer);
    }
    else if (percentage <= dr)
    {
      // d-cache Read
      rw_port->read
        ( m_config.m_dcache_base_address + (cache_alligned_address)
        , cache_line_size
        , read_buffer);
    }
    else /* if (percentage <= dw) */
    {
      // d-cache Write
      rw_port->write
        ( m_config.m_dcache_base_address + (cache_alligned_address)
        , cache_line_size
        , write_buffer);
    }
/*  else if (percentage <= sr)
    {
      // scratch read
      rw_port->read
        ( m_config.m_scratch_base_address + address
        , cache_line_size
        , read_buffer);
    }
    else
    {
      // Scratch Write
      rw_port->write
        ( m_config.m_scratch_base_address + address
        , cache_line_size
        , write_buffer);
    } */
  }
  
  // clean up
  delete read_buffer;
  delete write_buffer;
};
