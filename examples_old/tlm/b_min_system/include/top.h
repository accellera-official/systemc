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
 
/*=============================================================================
  @file top.h
  
  @brief Top module class
=============================================================================*/
#ifndef __TOP_H__
#define __TOP_H__

#include "traffic_generator.h"
#include "simple_memory.h" 
#include "rw_bus.h"

class top : public sc_module
{
public:

  top(sc_module_name name);


  traffic_generator          GENERATOR_1;
  traffic_generator          GENERATOR_2;
	  
  rw_bus<2, 2>               BUS;

  simple_memory              MEMORY_1;
  simple_memory              MEMORY_2;
};

#endif /* __TOP_H__ */
