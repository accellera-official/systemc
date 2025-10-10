/*****************************************************************************

  Licensed to Accellera Systems Initiative Inc. (Accellera) under one or
  more contributor license agreements.  See the NOTICE file distributed
  with this work for additional information regarding copyright ownership.
  Accellera licenses this file to you under the Apache License, Version 2.0
  (the "License"); you may not use this file except in compliance with the
  License.  You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

  Unless required by applicable law or agreed to in writing, software
  distributed under the License is distributed on an "AS IS" BASIS,
  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or
  implied.  See the License for the specific language governing
  permissions and limitations under the License.

 *****************************************************************************/

#ifndef TLM_CORE_TLM_TARGET_SOCKET_H_INCLUDED_
#define TLM_CORE_TLM_TARGET_SOCKET_H_INCLUDED_

#include "tlm_core/tlm_2/tlm_sockets/tlm_base_socket_if.h"
#include "tlm_core/tlm_2/tlm_2_interfaces/tlm_fw_bw_ifs.h"
#include "sysc/communication/sc_communication_ids.h"

namespace tlm {

template <unsigned int BUSWIDTH = 32,
          typename FW_IF = tlm_fw_transport_if<>,
          typename BW_IF = tlm_bw_transport_if<> >
class tlm_base_target_socket_b
  : public tlm_base_socket_if
{
public:
  virtual ~tlm_base_target_socket_b() {}

  virtual sc_core::sc_port_b<BW_IF> &       get_base_port() = 0;
  virtual sc_core::sc_port_b<BW_IF> const & get_base_port() const = 0;
  virtual sc_core::sc_export<FW_IF> &       get_base_export() = 0;
  virtual sc_core::sc_export<FW_IF> const & get_base_export() const = 0;
  virtual                    FW_IF  &       get_base_interface() = 0;
  virtual                    FW_IF  const & get_base_interface() const = 0;
};

template <unsigned int BUSWIDTH,
          typename FW_IF,
          typename BW_IF> class tlm_base_initiator_socket_b;

template <unsigned int BUSWIDTH,
          typename FW_IF,
          typename BW_IF,
          int N,
          sc_core::sc_port_policy POL> class tlm_base_initiator_socket;

template <unsigned int BUSWIDTH = 32,
          typename FW_IF = tlm_fw_transport_if<>,
          typename BW_IF = tlm_bw_transport_if<>,
          int N = 1,
          sc_core::sc_port_policy POL = sc_core::SC_ONE_OR_MORE_BOUND>
class tlm_base_target_socket : public tlm_base_target_socket_b<BUSWIDTH, FW_IF, BW_IF>,
                               public sc_core::sc_export<FW_IF>
{
public:
  typedef FW_IF fw_interface_type;
  typedef BW_IF bw_interface_type;
  typedef sc_core::sc_port<bw_interface_type, N , POL> port_type;

  typedef sc_core::sc_export<fw_interface_type> export_type;
  typedef tlm_base_initiator_socket_b<BUSWIDTH,
                                      fw_interface_type,
                                      bw_interface_type>  base_initiator_socket_type;

  typedef tlm_base_target_socket_b<BUSWIDTH,
                                   fw_interface_type,
                                   bw_interface_type> base_type;

  template <unsigned int, typename, typename, int, sc_core::sc_port_policy>
  friend class tlm_base_initiator_socket;

public:
  tlm_base_target_socket()
  : export_type(sc_core::sc_gen_unique_name("tlm_base_target_socket"))
  , m_port(sc_core::sc_gen_unique_name("tlm_base_target_socket_port"))
  {
  }

  explicit tlm_base_target_socket(const char* name)
  : export_type(name)
  , m_port(sc_core::sc_gen_unique_name((std::string(name) + "_port").c_str()))
  {
  }

  virtual const char* kind() const
  {
    return "tlm_base_target_socket";
  }

#if defined(__clang__) || \
  (defined(__GNUC__) && ((__GNUC__ * 1000 + __GNUC_MINOR__) >= 4006))
// ignore warning about deliberately hidden "bind()" overloads
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Woverloaded-virtual"
#endif  

  //
  // Bind target socket to initiator socket
  // - Binds the port of the initiator socket to the export of the target
  //   socket
  // - Binds the port of the target socket to the export of the initiator
  //   socket
  //
  virtual void bind(base_initiator_socket_type& s)
  {
    // initiator.port -> target.export
    (s.get_base_port())(get_base_interface());
    // target.port -> initiator.export
    get_base_port()(s.get_base_interface());
  }

  void operator() (base_initiator_socket_type& s)
  {
    bind(s);
  }

  
  // "tlm_base_target_socket<>::bind(tlm_base_target_socket<>)"
  //
  // Bind a target socket to this object instance (target socket), a hierarchical bind. 
  // The bind is done to both the port and export.
  //
  // Arguments:
  //   s = socket to be bound to this object instance.
  //
  // Notes:
  //   (1) IEEE 1666 forbids the hierarchical bind of a multi-target socket to a non-multi-target
  //       socket, so we check if the object to be bound is multi-target and this object instance 
  //       is not multi-target.
  //   (2) The opposite hierarchical bind, a non-multi-target socket to a multi-target socket 
  //       will result in a compiler error, so we don't need to check that case.

  virtual void bind(base_type& s)
  {
    // Look for an illegal bind (see note 1 above.)

    if ( s.get_socket_category() == tlm::TLM_MULTI_TARGET_SOCKET ) {
      if ( tlm::TLM_MULTI_TARGET_SOCKET != get_socket_category() ) {
        SC_REPORT_ERROR(sc_core::SC_ID_INVALID_HIERARCHICAL_BIND_, this->name());
      }
    }

    // bind the exports

    (get_base_export())(s.get_base_export());

    // bind the ports

    (s.get_base_port())(get_base_port());
  }

  void operator() (base_type& s)
  {
    bind(s);
  }

  //
  // Bind interface to socket
  // - Binds the interface to the export
  //
  virtual void bind(fw_interface_type& ifs)
  {
    export_type* exp = &get_base_export();
    if( this == exp ) {
      export_type::bind( ifs ); // non-virtual function call
    } else {
      exp->bind( ifs );
    }
  }

  void operator() (fw_interface_type& s)
  {
    bind(s);
  }

#if defined(__clang__) || \
  (defined(__GNUC__) && ((__GNUC__ * 1000 + __GNUC_MINOR__) >= 4006))
#pragma GCC diagnostic pop
#endif

  //
  // Forward to 'size()' of port class
  //
  int size() const
  {
    return m_port.size();
  }

  //
  // Forward to 'operator->()' of port class
  //
  bw_interface_type* operator->()
  {
    return m_port.operator->();
  }

  //
  // Forward to 'operator[]()' of port class
  //
  bw_interface_type* operator[](int i)
  {
    return m_port.operator[](i);
  }

  // Implementation of tlm_base_socket_if functions
  virtual unsigned int                    get_bus_width() const
    { return BUSWIDTH; }
  virtual std::type_index                 get_protocol_types() const
    { return typeid(typename FW_IF::protocol_types); }
  virtual tlm_socket_category             get_socket_category() const
    { return TLM_TARGET_SOCKET; }
  virtual bool                            is_multi_socket() const final
    { return (N != 1); }

  // Implementation of tlm_base_target_socket_b functions
  virtual sc_core::sc_port_b<BW_IF> &       get_base_port()
    { return m_port; }
  virtual sc_core::sc_port_b<BW_IF> const & get_base_port() const
    { return m_port; }

  virtual                    FW_IF  &       get_base_interface()
    { return *this; }
  virtual                    FW_IF  const & get_base_interface() const
    { return *this; }

  virtual sc_core::sc_export<FW_IF> &       get_base_export()
    { return *this; }
  virtual sc_core::sc_export<FW_IF> const & get_base_export() const
    { return *this; }

protected:
  port_type m_port;
};


//
// Convenience blocking and non-blocking socket classes
//

template <unsigned int BUSWIDTH = 32,
          typename TYPES = tlm_base_protocol_types,
          int N = 1,
          sc_core::sc_port_policy POL = sc_core::SC_ONE_OR_MORE_BOUND>
class tlm_target_socket :
  public tlm_base_target_socket <BUSWIDTH,
                            tlm_fw_transport_if<TYPES>,
                            tlm_bw_transport_if<TYPES>,
                            N, POL>
{
  typedef tlm_base_target_socket<BUSWIDTH,
                                 tlm_fw_transport_if<TYPES>,
                                 tlm_bw_transport_if<TYPES>,
                                 N, POL> base_socket_type;
public:
  tlm_target_socket()
    : base_socket_type()
  {}

  explicit tlm_target_socket(const char* name)
    : base_socket_type(name)
  {}

  virtual const char* kind() const
  {
    return "tlm_target_socket";
  }
};

} // namespace tlm

#endif // TLM_CORE_TLM_TARGET_SOCKET_H_INCLUDED_
