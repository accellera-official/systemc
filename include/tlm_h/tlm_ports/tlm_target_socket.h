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

#ifndef __TLM_TARGET_SOCKET_H__
#define __TLM_TARGET_SOCKET_H__

//#include <systemc>
#include "tlm_h/tlm_annotated/tlm_fw_bw_ifs/tlm_fw_bw_ifs.h"

namespace tlm {

template <unsigned int BUSWIDTH,
          typename FW_IF,
          typename BW_IF> class tlm_initiator_socket;

template <unsigned int BUSWIDTH = 32,
          typename FW_IF = tlm_fw_nb_transport_if<>,
          typename BW_IF = tlm_bw_nb_transport_if<> >
class tlm_target_socket : public sc_core::sc_export<FW_IF>
{
public:
  typedef FW_IF                                 fw_interface_type;
  typedef BW_IF                                 bw_interface_type;
  typedef sc_core::sc_port<bw_interface_type>   port_type;
  typedef sc_core::sc_export<fw_interface_type> export_type;
  typedef tlm_initiator_socket<BUSWIDTH,
                            fw_interface_type,
                            bw_interface_type>  initiator_socket_type;

  friend class tlm_initiator_socket<BUSWIDTH,
                                    fw_interface_type,
                                    bw_interface_type>;

public:
  tlm_target_socket(const char* name)
  : export_type(name)
  , mPort(sc_core::sc_gen_unique_name((std::string(name) + "_port").c_str()))
  {
  }

  unsigned int get_bus_width() const
  {
    return BUSWIDTH;
  }

  //
  // Bind target socket to initiator socket
  // - Binds the port of the initiator socket to the export of the target
  //   socket
  // - Binds the port of the target socket to the export of the initiator
  //   socket
  //
  void bind(initiator_socket_type& s)
  {
    // initiator.port -> target.export
    (*static_cast<typename initiator_socket_type::port_type*>(&s))(*static_cast<export_type*>(this));
    // target.port -> initiator.export
    mPort(s.mExport);
  }

  void operator() (initiator_socket_type& s)
  {
    bind(s);
  }

  //
  // Bind target socket to target socket (hierarchical bind)
  // - Binds both the export and the port
  //
  void bind(tlm_target_socket& s)
  {
    // export
    (*static_cast<export_type*>(this))(*static_cast<export_type*>(&s));
    // port
    s.mPort(mPort);
  }

  void operator() (tlm_target_socket& s)
  {
    bind(s);
  }

  //
  // Bind interface to socket
  // - Binds the interface to the export
  //
  void bind(fw_interface_type& ifs)
  {
    (*static_cast<export_type*>(this))(ifs);
  }

  void operator() (fw_interface_type& s)
  {
    bind(s);
  }

  //
  // Forward to 'operator->()' of port class
  //
  bw_interface_type* operator->()
  {
    return mPort.operator->();
  }

protected:
  port_type mPort;
};

} // namespace tlm

#endif
