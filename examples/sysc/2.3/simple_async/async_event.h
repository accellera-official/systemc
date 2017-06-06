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
/*
 * thread_safe_event.h
 *
 * Copyright (C) 2017, GreenSocs Ltd.
 *
 * Developped by Mark Burton mark@greensocs.com
 */
#ifndef ASYNC_EVENT_H_INCLUDED_
#define ASYNC_EVENT_H_INCLUDED_

#include <systemc>

class async_event
  : public  sc_core::sc_prim_channel
{
  sc_core::sc_time  m_delay;
  sc_core::sc_event m_event;

public:
  async_event(const char* name = sc_core::sc_gen_unique_name("async_event"))
    : sc_core::sc_prim_channel(name)
    , m_event((std::string(this->basename())+"_event").c_str())
  {
    // register this channel as "suspending", to not end the simulation
    // when we're running out of internal events
    async_attach_suspending();
  }

  // THREADSAFE METHOD:
  void notify(sc_core::sc_time delay = SC_ZERO_TIME)
    {
      m_delay = delay;
      async_request_update();
    }

  // only allow waiting for the event
  operator const sc_event&() const { return m_event; }

protected:
  void update(void)
    {
      // we're in the update phase of the SystemC kernel (thread)
      m_event.notify(m_delay);
    }
};

#endif // ASYNC_EVENT_H_INCLUDED_
