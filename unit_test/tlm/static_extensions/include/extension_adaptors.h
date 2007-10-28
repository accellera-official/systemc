#ifndef __EXTENSIONS_ADAPTORS_H__
#define __EXTENSIONS_ADAPTORS_H__

#include "tlm.h"
#include "my_extension.h"

#include "simple_master_socket.h"
#include "simple_slave_socket.h"

template <unsigned int BUSWIDTH = 32>
class adapt_ext2gp : public sc_core::sc_module
{
    public:
    typedef my_extended_payload                        initiator_payload_type;
    typedef tlm::tlm_generic_payload                   target_payload_type;
    typedef SimpleMasterSocket<BUSWIDTH,
                               target_payload_type>    master_socket_type;
    typedef SimpleSlaveSocket<BUSWIDTH,
                              initiator_payload_type>  slave_socket_type;
    
    slave_socket_type  slave_socket;
    master_socket_type master_socket;

    SC_HAS_PROCESS(adapt_ext2gp);
    adapt_ext2gp(sc_core::sc_module_name name_)
        : sc_core::sc_module(name_)
    {
        REGISTER_NBTRANSPORT(slave_socket, forward_nb_transport);
        REGISTER_DEBUGTRANSPORT(slave_socket, transport_debug);
        REGISTER_DMI(slave_socket, get_dmi_pointer);
        
        REGISTER_NBTRANSPORT(master_socket, backward_nb_transport);
        REGISTER_INVALIDATEDMI(master_socket, invalidate_dmi_pointers);
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
        return master_socket->nb_transport(static_cast<target_payload_type&>(trans),
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
        return slave_socket->nb_transport(static_cast<initiator_payload_type&>(trans),
                                          phase,
                                          t);
    }

    //////////////////////////
    // simple call forwarders:
    //////////////////////////
    unsigned int transport_debug(tlm::tlm_debug_payload& trans)
    {
        return master_socket->transport_dbg(trans);
    }
    bool get_dmi_pointer(const sc_dt::uint64& address,
                       bool for_reads,
                       tlm::tlm_dmi& dmi_data)
    {
      return master_socket->get_direct_mem_ptr(address, for_reads, dmi_data);
    }
    void invalidate_dmi_pointers(sc_dt::uint64 start_range,
                                 sc_dt::uint64 end_range)
    {
        slave_socket->invalidate_direct_mem_ptr(start_range, end_range);
    }
};

template <unsigned int BUSWIDTH = 32>
class adapt_gp2ext : public sc_core::sc_module
{
    public:
    typedef tlm::tlm_generic_payload                   initiator_payload_type;
    typedef my_extended_payload                        target_payload_type;
    typedef SimpleMasterSocket<BUSWIDTH,
                               target_payload_type>    master_socket_type;
    typedef SimpleSlaveSocket<BUSWIDTH,
                              initiator_payload_type>  slave_socket_type;
    
    slave_socket_type  slave_socket;
    master_socket_type master_socket;

    SC_HAS_PROCESS(adapt_gp2ext);
    adapt_gp2ext(sc_core::sc_module_name name_)
        : sc_core::sc_module(name_)
    {
        // Optionally, we can initialize our private extension class
        // here, if required.

        REGISTER_NBTRANSPORT(slave_socket, forward_nb_transport);
        REGISTER_DEBUGTRANSPORT(slave_socket, transport_debug);
        REGISTER_DMI(slave_socket, get_dmi_pointer);
        
        REGISTER_NBTRANSPORT(master_socket, backward_nb_transport);
        REGISTER_INVALIDATEDMI(master_socket, invalidate_dmi_pointers);
    }

    ///////////////
    // NB transport
    ///////////////

    // Forward direction: We extend the payload on the fly (if needed).
    tlm::tlm_sync_enum forward_nb_transport(initiator_payload_type& trans,
                                            tlm::tlm_phase& phase,
                                            sc_core::sc_time& t)
    {
        // If the mandatory extension is not there, we need to add it.
        // Otherwise we don't touch the extension, so that we don't overwrite
        // it in e.g. a nonGP->GP->nonGP (initiator->master->target) setup.
        // Note, however, that there might be situations where we might need to
        // re-initialize the extension, e.g. for mutable data fields in
        // different system setups.
        if (!trans.get_extension(my_extension::ID))
        {
            m_ext.m_data = 13;
            trans.set_extension(&m_ext);
        }
        return master_socket->nb_transport(static_cast<target_payload_type&>(trans),
                                           phase,
                                           t);
    }
    // Backward direction: only static_cast needed.
    tlm::tlm_sync_enum backward_nb_transport(target_payload_type& trans,
                                             tlm::tlm_phase& phase,
                                             sc_core::sc_time& t)
    {
        return slave_socket->nb_transport(static_cast<initiator_payload_type&>(trans),
                                          phase,
                                          t);
    }

    //////////////////////////
    // simple call forwarders:
    //////////////////////////
    unsigned int transport_debug(tlm::tlm_debug_payload& trans)
    {
        return master_socket->transport_dbg(trans);
    }
    bool get_dmi_pointer(const sc_dt::uint64& address,
                       bool for_reads,
                       tlm::tlm_dmi& dmi_data)
    {
        return master_socket->get_direct_mem_ptr(address, for_reads, dmi_data);
    }
    void invalidate_dmi_pointers(sc_dt::uint64 start_range,
                               sc_dt::uint64 end_range)
    {
        slave_socket->invalidate_direct_mem_ptr(start_range, end_range);
    }

private:
    my_extension m_ext;
};

#endif
