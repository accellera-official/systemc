/*****************************************************************************

  The following code is derived, directly or indirectly, from the SystemC
  source code Copyright (c) 1996-2008 by all Contributors.
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
 ******************************************************************************/

#ifndef __TRAFFIC_GENERATOR_H__
#define __TRAFFIC_GENERATOR_H__

#include "rw_initiator_socket.h"
#include "rw_master_base.h"

class traffic_generator : public sc_module,
                          virtual public rw_master_base
{
public:

  rw_initiator_socket           initiator_socket;

  SC_HAS_PROCESS(traffic_generator);
  traffic_generator(sc_module_name name);
  
  ~traffic_generator();                 

  void mem_test(sc_dt::uint64 base_address);
  
  void compute();
  
  virtual void invalidate_direct_mem_ptr(sc_dt::uint64 start_range,
                                         sc_dt::uint64 end_range);
};
#endif /* __TRAFFIC_GENERATOR_H__ */
