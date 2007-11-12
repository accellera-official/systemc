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

#ifndef __EXTENSIONS_ADAPTORS_H__
#define __EXTENSIONS_ADAPTORS_H__

#include "tlm.h"
#include "my_extension.h"

#include "simple_initiator_socket.h"
#include "simple_target_socket.h"

template <unsigned int BUSWIDTH = 32>
class adapt_ext2gp : public sc_core::sc_module
{
    public:
    typedef my_extended_payload                        initiator_payload_type;
    typedef tlm::tlm_generic_payload                   target_payload_type;
    typedef SimpleInitiatorSocket<BUSWIDTH,
                               target_payload_type>    initiator_socket_type;
    typedef SimpleTargetSocket<BUSWIDTH,
                              initiator_payload_type>  target_socket_type;
    
    target_socket_type  target_socket;
    initiator_socket_type initiator_socket;

    SC_HAS_PROCESS(adapt_ext2gp);
    adapt_ext2gp(sc_core::sc_module_name name_)
        : sc_core::sc_module(name_)
    {
        REGISTER_NBTRANSPORT(target_socket, forward_nb_transport);
        REGISTER_DEBUGTRANSPORT(target_socket, transport_debug);
        REGISTER_DMI(target_socket, get_dmi_pointer);
        
        REGISTER_NBTRANSPORT(initiator_socket, backward_nb_transport);
        REGISTER_INVALIDATEDMI(initiator_socket, invalidate_dmi_pointers);
    }

    ///////////////
    // NB transport
    ///////////////

    // Forward direction: The initiator calls this method with an extended
    // payload. We leave the extension class in the vector, and it will be
    // ignored by the GP target.
    tlm::tlm_sync_enum forward_nb_transport(initiator_payload_type& trans,
                                            tlm::tlm_phase& phase,
                                            sc_core::sc_time& t)
    {
        return initiator_socket->nb_transport(static_cast<target_payload_type&>(trans),
                                              phase,
                                              t);
    }
    // Backward direction: we can  assume here that the payload we get
    // as parameter is the same one that the initiator sent out. Thus, the
    // extension vector is known to be present.
    tlm::tlm_sync_enum backward_nb_transport(target_payload_type& trans,
                                             tlm::tlm_phase& phase,
                                             sc_core::sc_time& t)
    {
        return target_socket->nb_transport(static_cast<initiator_payload_type&>(trans),
                                           phase,
                                           t);
    }

    //////////////////////////
    // simple call forwarders:
    //////////////////////////
    unsigned int transport_debug(tlm::tlm_debug_payload& trans)
    {
        return initiator_socket->transport_dbg(trans);
    }
    bool get_dmi_pointer(const sc_dt::uint64& address,
                       bool for_reads,
                       tlm::tlm_dmi& dmi_data)
    {
      return initiator_socket->get_direct_mem_ptr(address,
                                                  for_reads,
                                                  dmi_data);
    }
    void invalidate_dmi_pointers(sc_dt::uint64 start_range,
                                 sc_dt::uint64 end_range)
    {
        target_socket->invalidate_direct_mem_ptr(start_range,
                                                 end_range);
    }
};

template <unsigned int BUSWIDTH = 32>
class adapt_gp2ext : public sc_core::sc_module
{
    public:
    typedef tlm::tlm_generic_payload                   initiator_payload_type;
    typedef my_extended_payload                        target_payload_type;
    typedef SimpleInitiatorSocket<BUSWIDTH,
                               target_payload_type>    initiator_socket_type;
    typedef SimpleTargetSocket<BUSWIDTH,
                              initiator_payload_type>  target_socket_type;
    
    target_socket_type  target_socket;
    initiator_socket_type initiator_socket;

    SC_HAS_PROCESS(adapt_gp2ext);
    adapt_gp2ext(sc_core::sc_module_name name_)
        : sc_core::sc_module(name_)
    {
        // Optionally, we can initialize our private extension class
        // here, if required.

        REGISTER_NBTRANSPORT(target_socket, forward_nb_transport);
        REGISTER_DEBUGTRANSPORT(target_socket, transport_debug);
        REGISTER_DMI(target_socket, get_dmi_pointer);
        
        REGISTER_NBTRANSPORT(initiator_socket, backward_nb_transport);
        REGISTER_INVALIDATEDMI(initiator_socket, invalidate_dmi_pointers);
    }

    ///////////////
    // NB transport
    ///////////////

    // Forward direction: We extend the payload on the fly (if needed).
    tlm::tlm_sync_enum forward_nb_transport(initiator_payload_type& trans,
                                            tlm::tlm_phase& phase,
                                            sc_core::sc_time& t)
    {
        // If the mandatory extension is not there, we need to add it and
        // store it so that we can re-construc the original state.
        // Otherwise we don't touch the extension, so that we don't overwrite
        // it in e.g. a nonGP->GP->nonGP (initiator->interconnect->target)
        // setup.
        // Note, however, that there might be situations where we might need to
        // re-initialize the extension, e.g. for mutable data fields in
        // different system setups.
        trans.get_extension(m_initiator_ext);
        if (!m_initiator_ext)
        {
            m_ext.m_data = 13;
            m_initiator_ext = trans.set_extension(&m_ext);
        }
        tlm::tlm_sync_enum tmp =
        initiator_socket->nb_transport(static_cast<target_payload_type&>(trans),
                                       phase,
                                       t);
        if (tmp == tlm::TLM_COMPLETED ||
            tmp == tlm::TLM_REJECTED)
        {
            m_initiator_ext = trans.set_extension(m_initiator_ext);            
        }
        return tmp;
    }
    // Backward direction: only restore of original extension and static_cast.
    tlm::tlm_sync_enum backward_nb_transport(target_payload_type& trans,
                                             tlm::tlm_phase& phase,
                                             sc_core::sc_time& t)
    {
        m_initiator_ext = trans.set_extension(m_initiator_ext);
        return target_socket->nb_transport(static_cast<initiator_payload_type&>(trans),
                                           phase,
                                           t);
    }

    //////////////////////////
    // simple call forwarders:
    //////////////////////////
    unsigned int transport_debug(tlm::tlm_debug_payload& trans)
    {
        return initiator_socket->transport_dbg(trans);
    }
    bool get_dmi_pointer(const sc_dt::uint64& address,
                         bool for_reads,
                         tlm::tlm_dmi& dmi_data)
    {
        return initiator_socket->get_direct_mem_ptr(address,
                                                    for_reads,
                                                    dmi_data);
    }
    void invalidate_dmi_pointers(sc_dt::uint64 start_range,
                                 sc_dt::uint64 end_range)
    {
        target_socket->invalidate_direct_mem_ptr(start_range,
                                                 end_range);
    }

private:
    my_extension  m_ext;
    my_extension* m_initiator_ext;
};

#endif
