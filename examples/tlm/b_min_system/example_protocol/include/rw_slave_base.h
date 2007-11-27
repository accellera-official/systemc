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
 * rw_slave_base.h
 * $Id:$
 * @brief
 * Slave base corresponding to the rw interface.
 * @Details
 * The slave base interfaces between blocking transport
 * and a convenience API (here rw_if).
 * 
 ********************************************************************************/
#ifndef __RW_SLAVE_BASE_H__
#define __RW_SLAVE_BASE_H__

#include "systemc.h"
#include "tlm.h"
#include "rw_if.h"

class rw_slave_base : virtual public tlm::tlm_fw_transport_if<>,
                      public rw_if
{
  public:
		
	virtual void b_transport(tlm::tlm_generic_payload & trans);
};

#endif /* __RW_SLAVE_BASE_H__ */
