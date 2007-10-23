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

#ifndef __TLM_FW_BW_IFS_H__
#define __TLM_FW_BW_IFS_H__

#include "tlm_h/tlm_generic_payload/tlm_generic_payload.h"
#include <systemc>

using tlm_generic_payload_ns::tlm_generic_payload;
using tlm_generic_payload_ns::tlm_dmi;
using tlm_generic_payload_ns::tlm_debug_payload;

enum tlm_phase { BEGIN_REQ, END_REQ, BEGIN_RESP, END_RESP };
enum tlm_sync_enum { TLM_REJECTED, TLM_SYNC, TLM_SYNC_CONTINUE, TLM_COMPLETED };

////////////////////////////////////////////////////////////////////////////
// Basic interfaces
////////////////////////////////////////////////////////////////////////////
template <typename TRANS = tlm_generic_payload, typename PHASE = tlm_phase>
class tlm_nonblocking_transport_if : public virtual sc_core::sc_interface {
public:
  virtual tlm_sync_enum nb_transport(TRANS& trans, PHASE& phase, sc_core::sc_time& t) = 0;
};

template <typename TRANS = tlm_generic_payload>
class tlm_blocking_transport_if : public virtual sc_core::sc_interface {
public:
  virtual void b_transport(TRANS& trans) = 0;
};

///////////////////////
// DMI interfaces for getting and invalidating DMI pointers:
///////////////////////

// The semantics of the forward interface are as follows:
// 
// - An initiator that want to get direct access to a slave's memory region
//   can call the get_direct_mem_ptr method with the address parameter set to
//   the address that it wants to gain access to. The forReads parameter specifies
//   if a dmi range for read accesses or for write access must be returned.
// - The 'forReads' parameter is necessary because read and write ranges do not have
//   to have the same ranges. If they do, a slave can specify that the range is valid
//   for all accesses with the type attribute in the tlm_dmi structure.
// - The bus, if any, needs to decode the address and forward the call to
//   the corresponding slave. It needs to handle the address exactly as the
//   slave would expect on a transaction call, e.g. mask the address according
//   to the slave's address width.
// - If the slave supports DMI access for the given address, it sets the
//   data fields in the DMI struct and returns true.
// - If the slave does not support DMI access it needs to return false.
//   The slave can either set the correct address range in the DMI struct
//   to indicate the memory region where DMI is disallowed, or it can specify
//   the complete address range if it doesn't know it's memory range.
// - The bus must translate the addresses to the master's address space. This
//   must be the inverse operation of what the bus/interconnect needed to do
//   when forwarding the call.
// - In case the slave returned with an invalid region the bus/interconnect
//   must fill in the complete address region for the particular slave in the
//   DMI data structure.
//
// DMI hint optimization:
// 
// Initiators may use the DMI hint in the tlm_generic_payload to avoid
// unnecessary memory locking attempts. The recommended sequence of interface
// method calls would be:
//
// - The initiator first tries to check if it has a valid DMI region for the
//   address that it wants to access next.
// - If not, it performs a normal transaction.
// - If the DMI hint in this transaction is true, the initiator can try and
//   get the DMI region.
//
// Note that the DMI hint optimization is completely optional and every
// initiator model is free to ignore the DMI hint.

class tlm_fw_direct_mem_if : public virtual sc_core::sc_interface
{
public:
  virtual bool get_direct_mem_ptr(const sc_dt::uint64& address, bool forReads,
                                  tlm_dmi& dmi_data) = 0;
};

// The semantics of the backwards call is as follows:
//
// - If a slave wants to notify the system that all DMI pointers to its
//   memory are about to become invalid it must call the below interface
//   method with the range arguments set to its memory region and the 
//   invalidate_all argument set to false.
// - A bus/interconnect must forward this call to all masters that could
//   potentially have a DMI pointer to this memory. A safe implementation is
//   to call every attached master.
// - Each master must check if it has a pointer to the given memory and throw
//   this away.
//
// - A full DMI pointer invalidation, e.g. for a bus remap can be signaled
//   by setting the range: 0x0 - 0xffffffffffffffffull = (sc_dt::uint64)-1
// - A master must throw away all DMI pointers in this case.
//
// - Under no circumstances a model is allowed to call the get_direct_mem_ptr
//   from within the invalidate_direct_mem_ptr method, directly or indirectly.
//
class tlm_bw_direct_mem_if : public virtual sc_core::sc_interface
{
public:
  virtual void invalidate_direct_mem_ptr(sc_dt::uint64 start_range,
                                         sc_dt::uint64 end_range) = 0;
};

///////////////////////
// debug interface for memory access
///////////////////////
//
// This interface can be used to gain access to a slaves memory or registers
// in a non-intrusive manner. No side effects, waits or event notifications
// must happen in the course of the method.
//
// Semantics:
// - The initiator calls the transport_dbg method with the following arguments:
//   . address: The start address that it wants to peek or poke.
//   . num_bytes: The number of bytes that it requests to read or write.
//   . do_read: Indicates a read or write access.
//   . data: A pointer to the initiator-allocated data buffer, which must be
//           at least num_bytes large. The data is always organized in the
//           endianness of the machine.
// - The bus/interconnect will decode the address and forward the call to the
//   appropriate slave.
// - The slave must return the number of successfully transmitted bytes, where
//   this number must be <= num_bytes. Thus, a slave can safely return 0 if it
//   does not support debug transactions.
//
class tlm_transport_dbg_if : public virtual sc_core::sc_interface
{
public:
  // The return value of defines the number of bytes successfully
  // transferred.
  virtual unsigned int transport_dbg(tlm_debug_payload& r) = 0;
};

////////////////////////////////////////////////////////////////////////////
// Combined interfaces
////////////////////////////////////////////////////////////////////////////

// The forward non-blocking interface:
template <typename TRANS = tlm_generic_payload, typename PHASE = tlm_phase>
class tlm_fw_nb_transport_if
  : public virtual tlm_nonblocking_transport_if<TRANS, PHASE>
  , public virtual tlm_fw_direct_mem_if
  , public virtual tlm_transport_dbg_if
{};

// The backward non-blocking interface:
template <typename TRANS = tlm_generic_payload,
          typename PHASE = tlm_phase>
class tlm_bw_nb_transport_if
  : public virtual tlm_nonblocking_transport_if<TRANS, PHASE>
  , public virtual tlm_bw_direct_mem_if
{};

// The forward blocking interface:
template <typename TRANS = tlm_generic_payload>
class tlm_fw_transport_if
  : public virtual tlm_blocking_transport_if<TRANS>
  , public virtual tlm_fw_direct_mem_if
  , public virtual tlm_transport_dbg_if
{};

// The backward blocking interface:
class tlm_bw_transport_if
  : public virtual tlm_bw_direct_mem_if
{};

#endif /* __TLM_FW_BW_IFS_H__ */
