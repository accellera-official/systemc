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

#include <typeinfo>
#include <typeindex>
#include <unordered_map>

#include <systemc>
#include <tlm>

const char* tlm_socket_category_name[] = {
  "TLM_UNKNOWN_SOCKET",
  "TLM_INITIATOR_SOCKET",
  "TLM_TARGET_SOCKET" };

template <typename T>
SC_MODULE(mod)
{
  T socket;

  std::unordered_map<std::type_index, std::string> tlm_base_protocol_types_names;

  SC_CTOR(mod)
  {
    // check if derived from tlm_base_socket_if
    tlm::tlm_base_socket_if* base = dynamic_cast<tlm::tlm_base_socket_if*>(&socket);
    sc_assert(base);

    tlm_base_protocol_types_names[std::type_index(typeid(tlm::tlm_base_protocol_types))] = "tlm_base_protocol_types";
    tlm_base_protocol_types_names[std::type_index(typeid(tlm::tlm_fw_transport_if<>))] = "tlm_fw_transport_if";
    tlm_base_protocol_types_names[std::type_index(typeid(tlm::tlm_bw_transport_if<>))] = "tlm_bw_transport_if";

    // check base methods
    sc_core::sc_port_base& base_port = base->get_base_port();
    std::cout << "port_base: " << base_port.name() << std::endl;

    const sc_core::sc_port_base& c_base_port = base->get_base_port();
    std::cout << "const port_base: " << c_base_port.name() << std::endl;

    sc_core::sc_export_base& base_export = base->get_base_export();  
    std::cout << "base_export: " << base_export.name() << std::endl;

    const sc_core::sc_export_base& c_base_export = base->get_base_export();
    std::cout << "const base_export: " << c_base_export.name() << std::endl;

    unsigned int width = base->get_bus_width();
    std::cout << "bus width: " << width << std::endl;

    std::type_index protocol_types = base->get_protocol_types();
    std::cout << "protocol_types: " 
              << tlm_base_protocol_types_names[protocol_types] 
              << std::endl;

    tlm::tlm_socket_category socket_category = base->get_socket_category();
    std::cout << "tlm_socket_category: " << tlm_socket_category_name[socket_category] << std::endl;

    bool is_multi_socket = base->is_multi_socket();
    std::cout << "is_multi_socket: " << is_multi_socket << std::endl;
  }

};

int sc_main(int argc, char* argv[])
{
  mod<tlm::tlm_initiator_socket<32> > m1("m1");  
  mod<tlm::tlm_target_socket<16> > m2("m2");  

  return 0;
}
