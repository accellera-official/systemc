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

#ifndef __TLM_DMI_H__
#define __TLM_DMI_H__

#include <systemc>

// TLM_LITTLE_ENDIAN is defined there:
#include "tlm_h/tlm_trans/tlm_generic_payload/tlm_helpers.h"

namespace tlm {


// Class for signaling the requested access type to the target. The target
// is allowed to promote READ or WRITE requests to READ_WRITE.
//
// The tlm_direct_mem_if is templated over this interface, so that the user
// has the opportunity to stick extensions into the class. One application
// example could be to communicate a CPU ID to the target for situations
// where address handling might be different for each CPU.
class tlm_dmi_mode
{
public:
  enum Type { READ = 0x1, WRITE = 0x2, READ_WRITE = READ|WRITE };
  Type type;
    
};

class tlm_dmi
{
public:

  tlm_dmi()
  {
    init();
  }
  
  void init()
  {
    dmi_ptr           = 0;
    dmi_start_address = 0x0;
    dmi_end_address   = (sc_dt::uint64)-1;
    read_latency      = sc_core::SC_ZERO_TIME;
    write_latency     = sc_core::SC_ZERO_TIME;
    endianness        = TLM_LITTLE_ENDIAN;
  }
  
  // If the forward call is successful, the target returns the dmi_ptr,
  // which must point to the data element corresponding to the
  // dmi_start_address. The data is organized as a byte array with the
  // endianness of the target (endianness member of the tlm_dmi struct).
  unsigned char* dmi_ptr;
  
  // The absolute start and end addresses of the DMI region. If the decoder
  // logic in the interconnect changes the address field e.g. by masking, the
  // interconnect is responsible to transform the relative address back to an
  // absolute address again.
  sc_dt::uint64 dmi_start_address;
  sc_dt::uint64 dmi_end_address;

  // These members define the latency of read/write transactions. The
  // initiator must initialize these members to zero before requesting a
  // dmi pointer, because both the interconnect as well as the target can
  // add to the total transaction latency.
  // Depending on the 'type' attribute only one, or both of these attributes
  // will be valid.
  sc_core::sc_time  read_latency;
  sc_core::sc_time  write_latency;
  
  // The target sets the endianness of the data in the dmi_ptr array.
  tlm_endianness endianness;
};

} // namespace tlm

#endif /* TLM_DMI_HEADER */
