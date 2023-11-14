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
/*****************************************************************************

  Original Author: Mark Burton, GreenSocs

 *****************************************************************************/

#ifndef ASYNC_EVENT_H
#define ASYNC_EVENT_H

/* this is a very simplistic implementation of an event that can be notified
 * from a different thread. Ensuring that the normal systemc semantics of
 * 'write-over-write' are maintained is left to the interested reader.
 */
class async_event : public sc_core::sc_prim_channel, public sc_event
{
private:
    sc_core::sc_time m_delay;
public:
    async_event(sc_module_name n = "")
    {
        async_attach_suspending();
    }

    void notify(sc_core::sc_time delay = SC_ZERO_TIME)
    {
        m_delay = delay; // no guarantee if event notified multiple times.
        async_request_update();
    }

protected:
    void update(void)
    {
        sc_event::notify(m_delay);
    }
};

#endif
