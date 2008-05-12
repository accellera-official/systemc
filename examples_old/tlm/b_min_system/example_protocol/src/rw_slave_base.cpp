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

/*=============================================================================
  @file rw_slave_base.cpp
  
  @brief Implementation of rw_slave_base
=============================================================================*/
#include "rw_slave_base.h"
#include "reporting.h"

/*=============================================================================
  Implementation of blocking transport

  This implementation calls a function of the convenience API (rw_if)
  depending on the content of the generic payload.
=============================================================================*/

void rw_slave_base::b_transport(tlm::tlm_generic_payload & trans)
{
	bool status;
	
	switch (trans.get_command())
	{
		case tlm::TLM_READ_COMMAND:
		{
			status = this->read(trans.get_address(), trans.get_data_length(), trans.get_data_ptr());
			
			break;
		}
		case tlm::TLM_WRITE_COMMAND:
		{
			status = this->write(trans.get_address(), trans.get_data_length(), trans.get_data_ptr());
			
			break;
		}
		
		default:
		{
			std::cerr << "Invalid command in payload!" << std::endl;
			break;
		}
	}
	
	if (status)
	{
		trans.set_response_status(tlm::TLM_OK_RESPONSE);
	}
	else
	{
		trans.set_response_status(tlm::TLM_GENERIC_ERROR_RESPONSE);
	}
}
