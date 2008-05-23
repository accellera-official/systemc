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

#ifndef __TLM_DMI_H__
#define __TLM_DMI_H__

#include <systemc>

namespace tlm {

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
    dmi_read_latency  = sc_core::SC_ZERO_TIME;
    dmi_write_latency = sc_core::SC_ZERO_TIME;
  }
// Enum for signaling the granted access type to the initiator. 
// The initiator uses gp.m_command to indicate it intention (read/write)
//  The target is allowed to promote DMI_TYPE_READ or DMI_TYPE_WRITE
//  requests to DMI_TYPE_READ_WRITE.

  enum dmi_type_e
  { DMI_TYPE_IGNORE     = 0x00
  , DMI_TYPE_READ       = 0x01
  , DMI_TYPE_WRITE      = 0x02
  , DMI_TYPE_READ_WRITE = DMI_TYPE_READ | DMI_TYPE_WRITE
  };
  
  unsigned char*    get_dmi_ptr() const {return dmi_ptr;}
  sc_dt::uint64     get_start_address() const {return dmi_start_address;}
  sc_dt::uint64     get_end_address() const {return dmi_end_address;}
  sc_core::sc_time  get_read_latency() const {return dmi_read_latency;}
  sc_core::sc_time  get_write_latency() const {return dmi_write_latency;}
  dmi_type_e        get_granted_access() const {return dmi_type;}
  bool              is_read_allowed() const {return (dmi_type & DMI_TYPE_READ) != 0;}
  bool              is_write_allowed() const {return (dmi_type & DMI_TYPE_WRITE) != 0;}
  bool              is_read_write_allowed() const {return (dmi_type & DMI_TYPE_READ_WRITE) != 0;}

  void              set_dmi_ptr(unsigned char* p) {dmi_ptr = p;}
  void              set_start_address(sc_dt::uint64 addr) {dmi_start_address = addr;}
  void              set_end_address(sc_dt::uint64 addr) {dmi_end_address = addr;}
  void              set_read_latency(sc_core::sc_time t) {dmi_read_latency = t;}
  void              set_write_latency(sc_core::sc_time t) {dmi_write_latency = t;}
  void              set_granted_access(dmi_type_e t) {dmi_type = t;}
  void              allow_read() {dmi_type = DMI_TYPE_READ;}
  void              allow_write() {dmi_type = DMI_TYPE_WRITE;}
  void              allow_read_write() {dmi_type = DMI_TYPE_READ_WRITE;}

private:
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

  // Granted access
  dmi_type_e dmi_type;

  // These members define the latency of read/write transactions. The
  // initiator must initialize these members to zero before requesting a
  // dmi pointer, because both the interconnect as well as the target can
  // add to the total transaction latency.
  // Depending on the 'type' attribute only one, or both of these attributes
  // will be valid.
  sc_core::sc_time  dmi_read_latency;
  sc_core::sc_time  dmi_write_latency;
};

} // namespace tlm

#endif /* TLM_DMI_HEADER */
