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

#ifndef __TLM_DMI_H__
#define __TLM_DMI_H__

#include <systemc>

// TODO: where do we get TLM_LITTLE_ENDIAN from?

class tlm_dmi
{
public:

  tlm_dmi()
  {
    init();
  }
  
  void init()
  {
    dmi_ptr = 0;
    dmi_start_address = 0x0;
    dmi_end_address = (sc_dt::uint64)-1;
    type = READ_WRITE;
    read_latency = sc_core::SC_ZERO_TIME;
    write_latency = sc_core::SC_ZERO_TIME;
    endianness = TLM_LITTLE_ENDIAN;
  }
  
  // If the forward call is successful, the target returns the dmi_ptr,
  // which must point to the data element corresponding to the
  // dmi_start_address. The data is organized as a byte array with the
  // endianness of the target (endianness member of the tlm_dmi struct).
  unsigned char* dmi_ptr;
  
  // The absolut start and end addresses of the DMI region. If the decoder
  // logic in the interconnect changes the address field e.g. by masking, the
  // interconnect is responsible to transform the relative address back to a
  // absolute address again.
  sc_dt::uint64 dmi_start_address;
  sc_dt::uint64 dmi_end_address;

  // The type accesses this DMI range support. If eg the 'forReads' parameter
  // of the 'get_direct_mem_ptr' call is set to true, a target must set this
  // attribute to READ (in case the range is only for read accesses) or
  // READ_WRITE (in case the range supports both read and write accesses). If
  // the 'forReads' parameter is false a target must set this to WRITE or
  // READ_WRITE.
  enum Type { READ = 0x1, WRITE = 0x2, READ_WRITE = READ|WRITE };
  Type type;
    
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

#endif /* TLM_DMI_HEADER */
