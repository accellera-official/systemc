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

//// Bogus reset

#include <systemc>

using namespace sc_core;
using std::cout;
using std::endl;

struct M5: sc_module
{
  M5(sc_module_name _name)
  {
    SC_THREAD(ticker);
    SC_THREAD(calling);
    SC_METHOD(target);
      sensitive << ev;
      dont_initialize();
      t = sc_get_current_process_handle();
  }

  sc_process_handle t;
  sc_event ev;

  void ticker()
  {
    for (;;)
    {
      wait(10, SC_NS);
      ev.notify();
    }
  }

  void calling()
  {
    wait(15, SC_NS);
    // target runs at 10 NS due to notification of ev

    t.reset();
    // target runs at 15 NS due to reset.

    sc_stop();
  }

  void target()
  {
    cout << "Target called at " << sc_time_stamp() << endl;
  }
};

int sc_main(int argc, char* argv[])
{
  M5 m("m");

  sc_start();

  cout << endl << "Success" << endl;
  return 0;
}

