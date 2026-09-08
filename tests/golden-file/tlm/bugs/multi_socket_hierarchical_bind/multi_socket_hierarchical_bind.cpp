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

#include <tlm>
#include <tlm_utils/multi_passthrough_initiator_socket.h>
#include <tlm_utils/multi_passthrough_target_socket.h>

SC_MODULE(target)
{
  tlm_utils::multi_passthrough_target_socket<target>          targ_socket;
  tlm_utils::multi_passthrough_target_socket_optional<target> targ_optional;

  SC_CTOR(target)
    : targ_socket  ("targ_socket")
    , targ_optional("targ_optional")
  {
    std::cout << "Constructing " << name() << std::endl;
    targ_socket.register_b_transport(this, &target::b_transport);
    targ_optional.register_b_transport(this, &target::b_transport);
  }

private:
  void b_transport(int id, tlm::tlm_generic_payload&, sc_core::sc_time& )
  {
    std::cout
      << "Calling " << name() << "->b_transport"<< "(" << id << ")"
      << std::endl;
  }
};
// SC_MODULE(target)

SC_MODULE(parent_target)
{
  tlm_utils::multi_passthrough_target_socket<parent_target>          targ_socket;
  tlm_utils::multi_passthrough_target_socket_optional<parent_target> targ_optional;

  SC_CTOR(parent_target)
    : targ_socket  ("targ_socket")
    , targ_optional("targ_optional")
    , target_("target")
  {
    std::cout << "Constructing " << name() << std::endl;
    targ_socket( target_.targ_socket );
    targ_optional( target_.targ_optional );
  }

private:
  target target_;
};
// SC_MODULE(parent_target)

SC_MODULE(initiator)
{
  tlm_utils::multi_passthrough_initiator_socket<initiator>          init_socket;
  tlm_utils::multi_passthrough_initiator_socket_optional<initiator> init_optional;

  SC_CTOR(initiator)
    : init_socket  ("init_socket")
    , init_optional("init_optional")
  {
    std::cout << "Constructing " << name() << std::endl;
    SC_THREAD(thread);
  }

private:
  void thread()
  {
    const char* nm = sc_core::sc_get_current_process_handle().name();
    std::cout << "Starting " << nm << std::endl;

    tlm::tlm_generic_payload trans;
    sc_core::sc_time         delay;

    init_socket->b_transport(trans, delay);

    std::cout << "Exiting " << nm << std::endl;
  }
};
// SC_MODULE(initiator)

SC_MODULE(parent_initiator)
{
  tlm_utils::multi_passthrough_initiator_socket<parent_initiator>          init_socket;
  tlm_utils::multi_passthrough_initiator_socket_optional<parent_initiator> init_optional;

  SC_CTOR(parent_initiator)
    : init_socket  ("init_socket")
    , init_optional("init_optional")
    , init_("initiator")
  {
    std::cout << "Constructing " << name() << std::endl;
    init_.init_socket( init_socket );
    init_.init_optional( init_optional );
  }

private:
  initiator init_;
};
// SC_MODULE(parent_initiator)

int sc_main(int, char*[])
{
  parent_initiator parent_init("parent_init");
  parent_target    parent_targ("parent_targ");
  parent_init.init_socket( parent_targ.targ_socket );

  sc_core::sc_start();
  sc_core::sc_stop();
  return 0;
}
