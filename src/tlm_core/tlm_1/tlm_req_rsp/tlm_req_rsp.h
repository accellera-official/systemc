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

#ifndef __TLM_REQ_RSP_H__
#define __TLM_REQ_RSP_H__

// The unannotated TLM interfaces

#include "tlm_core/tlm_1/tlm_req_rsp/tlm_1_interfaces/tlm_core_ifs.h"
#include "tlm_core/tlm_1/tlm_req_rsp/tlm_1_interfaces/tlm_master_slave_ifs.h"

// The channels : tlm_fifo, tlm_transport_channel and tlm_req_rsp_channel

#include "tlm_core/tlm_1/tlm_req_rsp/tlm_channels/tlm_req_rsp_channels/tlm_req_rsp_channels.h"

// Some non blocking ports to provide static sensitivity

#include "tlm_core/tlm_1/tlm_req_rsp/tlm_ports/tlm_nonblocking_port.h"


#endif /* __TLM_REQ_RSP_H__ */
