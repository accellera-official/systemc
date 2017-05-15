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

 * simple example of async events
 *
 * Copyright (C) 2017, GreenSocs Ltd.
 *
 * Developped by  Mark Burton mark@greensocs.com
 *
 *****************************************************************************/


#include "systemc.h"
#include "async_event.h"

#ifndef IEEE_1666_CPLUSPLUS
#error This example Requires SystemC is built with c++11, you may use the async_event in a non c++11 environment.
#endif

#include <thread>
#include <chrono>

SC_MODULE(watchDog)
{
  async_event when;
  std::thread m_thread;
public:
  SC_CTOR(watchDog)
    {

      SC_METHOD(call_stop);
      sensitive << when;
      dont_initialize();

      SC_THREAD(startProcess);
    }
  
  ~watchDog() {
    m_thread.join();
  }
  

  void startProcess() 
    {
      m_thread=std::thread( [this] { this->process(); } );
    }
  void call_stop()
    {
      cout << "Asked to stop" << endl;
      sc_stop();
    }
  void process()
    {
      std::this_thread::sleep_for(std::chrono::seconds(1));
      when.notify(SC_ZERO_TIME);
    }
};




int sc_main(int , char* [])
{

  watchDog catcher("WatchDog");

  cout << "Start SystemC " << endl;
  sc_start();

  cout << "Program completed" << endl;
  return 0;
}
