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

#include <systemc>

#ifndef __TLM_DEBUG_PAYLOAD_H__
#define __TLM_DEBUG_PAYLOAD_H__

namespace tlm {

class tlm_debug_payload
{
public:
    // Start address of the transaction:
    sc_dt::uint64 address;
    
    // Indication of a read or write access:
    bool do_read;
    
    // Number of bytes to transfer:
    unsigned int num_bytes;
    
    // Pointer to the data array. Note that the data is always
    // organized in the endianness of the host machine (like
    // the data pointer in the generic payload):
    unsigned char* data;
};

} // namespace tlm

#endif /* __TLM_DEBUG_PAYLOAD_H__ */

