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

using namespace sc_core;

SC_MODULE(top) {
public:

	void thread_1() {
		SC_REPORT_INFO("thread_1","waiting");
		wait(1,SC_NS); // be sure that thread_2 is already waiting
		SC_REPORT_INFO("thread_1","killing other thread");
		t2_hdl.kill();
		SC_REPORT_INFO("thread_1","thread finished");
	}

	void thread_2() {
		sc_event_or_list event_list;
		event_list |= sc_event::none();
		SC_REPORT_INFO("thread_2","waiting on sc_event_list");
		sc_core::wait(event_list); // will never finish
		SC_REPORT_INFO("thread_2","this will never be printed");
	}

	SC_CTOR(top)  {
		SC_THREAD(thread_1);
		SC_THREAD(thread_2);
		t2_hdl = sc_get_current_process_handle();
	}

	sc_process_handle t2_hdl;
};

int sc_main(int argc, char **argv) {
	top i_top("top");
	sc_start();
	return 0;
}
