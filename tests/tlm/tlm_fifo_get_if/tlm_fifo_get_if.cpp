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

#include <systemc>
#include <tlm>

SC_MODULE(prod)
{
  sc_core::sc_port<tlm::tlm_fifo_put_if<int>> out;

  SC_CTOR(prod)
  {
    SC_THREAD(proc);
  }

  void proc()
  {
    while (true)
    {

      if (out->nb_can_put())
      {
        out->nb_put(cnt);
        std::cout << name() << " put: " << cnt << std::endl;
        cnt++;
      }

      wait(sc_core::SC_ZERO_TIME);

      if (cnt == 20)
        sc_core::sc_stop();
    }
  }

private:
  int cnt{10};
};

SC_MODULE(cons)
{
  sc_core::sc_port<tlm::tlm_fifo_get_if<int>> in;

  SC_CTOR(cons)
  {
    SC_THREAD(proc);
  }

  void proc()
  {
    while (true)
    {
      int resp;
      if (in->nb_peek(resp))
      {
        in->nb_get(resp);
        std::cout << name() << " get: " << resp << std::endl;
      }

      wait(sc_core::SC_ZERO_TIME);
    }
  }
};

int sc_main(int argc, char *argv[])
{
  prod p("producer");
  cons c("consumer");

  tlm::tlm_fifo<int> fifo("fifo");

  p.out(fifo);
  c.in(fifo);

  sc_core::sc_start();

  return 0;
}
