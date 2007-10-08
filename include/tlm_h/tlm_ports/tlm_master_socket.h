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

#ifndef TLM_MASTER_SOCKET_H
#define TLM_MASTER_SOCKET_H

#include <systemc.h>

template <typename IF> class tlm_slave_socket;

template <typename IF>
class tlm_master_socket : public sc_port<IF>
{
public:
  typedef IF interface_type;
  typedef sc_port<interface_type> port_type;
  typedef sc_export<interface_type> export_type;
  typedef tlm_slave_socket<interface_type> slave_socket_type;

  friend class tlm_slave_socket<interface_type>;

public:
  tlm_master_socket(const char* name) :
    port_type(name)
  {
  }

  //
  // Bind master socket to slave socket
  // - Binds the port of the master socket to the export of the slave socket
  // - Binds the port of the slave socket to the export of the master socket
  //
  void bind(slave_socket_type& s)
  {
    // master.port -> slave.export
    (*static_cast<port_type*>(this))(*static_cast<export_type*>(&s));
    // slave.port -> master.export
    s.mPort(mExport);
  }

  void operator() (slave_socket_type& s)
  {
    bind(s);
  }

  //
  // Bind master socket to master socket (hierarchical bind)
  // - Binds both the export and the port
  //
  void bind(tlm_master_socket& s)
  {
    // port
    (*static_cast<port_type*>(this))(*static_cast<port_type*>(&s));
    // export
    mExport(s.mExport);
  }

  void operator() (tlm_master_socket& s)
  {
    bind(s);
  }

  //
  // Bind interface to socket
  // - Binds the interface to the export of this socket
  //
  void bind(interface_type& ifs)
  {
    mExport(ifs);
  }

  void operator() (interface_type& s)
  {
    bind(s);
  }

protected:
  export_type mExport;
};

#endif
