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

#ifndef __TLM_SLAVE_SOCKET_H__
#define __TLM_SLAVE_SOCKET_H__

#include "tlm_h/tlm_annotated/tlm_fw_bw_ifs/tlm_fw_bw_ifs.h"
//#include <systemc>

using tlm_annotated::tlm_fw_nb_transport_if;
using tlm_annotated::tlm_bw_nb_transport_if;

template <unsigned int BUSWIDTH,
          typename FW_IF,
          typename BW_IF> class tlm_master_socket;

template <unsigned int BUSWIDTH = 32,
          typename FW_IF = tlm_fw_nb_transport_if<>,
          typename BW_IF = tlm_bw_nb_transport_if<> >
class tlm_slave_socket : public sc_core::sc_export<FW_IF>
{
public:
  typedef FW_IF fw_interface_type;
  typedef BW_IF bw_interface_type;
  typedef sc_core::sc_port<bw_interface_type> port_type;
  typedef sc_core::sc_export<fw_interface_type> export_type;
  typedef tlm_master_socket<BUSWIDTH,
                            fw_interface_type,
                            bw_interface_type> master_socket_type;

  friend class tlm_master_socket<BUSWIDTH,
                                 fw_interface_type,
                                 bw_interface_type>;

public:
  tlm_slave_socket(const char* name) :
    export_type(name)
  {
  }

  unsigned int get_bus_width() const
  {
    return BUSWIDTH;
  }

  //
  // Bind slave socket to master socket
  // - Binds the port of the master socket to the export of the slave socket
  // - Binds the port of the slave socket to the export of the master socket
  //
  void bind(master_socket_type& s)
  {
    // master.port -> slave.export
    (*static_cast<typename master_socket_type::port_type*>(&s))(*static_cast<export_type*>(this));
    // slave.port -> master.export
    mPort(s.mExport);
  }

  void operator() (master_socket_type& s)
  {
    bind(s);
  }

  //
  // Bind slave socket to slave socket (hierarchical bind)
  // - Binds both the export and the port
  //
  void bind(tlm_slave_socket& s)
  {
    // export
    (*static_cast<export_type*>(this))(*static_cast<export_type*>(s));
    // port
    mPort(s.mPort);
  }

  void operator() (tlm_slave_socket& s)
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

#endif
