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

  test01.cpp --

  Original Author: Mark Burton, Qualcomm Innovation Center, Inc. 2024

 *****************************************************************************/

/*****************************************************************************

  MODIFICATION LOG - modifiers, enter your name, affiliation, date and
  changes you are making here.

      Name, Affiliation, Date:
  Description of Modification:

 *****************************************************************************/

#include "systemc.h"

SC_MODULE(mod_a) {
  SC_LOG_HANDLE((TST), "test_handler");
  SC_CTOR(mod_a) {
    for (auto l : sc_log::log_level_map) {
      auto i = l.first;
      SC_LOG_AT(i, name()) << " Log to name()" << " (at level "<< i<<")";
      SC_LOG_AT(i, ()) <<  " Log to default ()" << " (at level "<< i<<")";
      SC_LOG_AT(i, (TST)) << " Log to test_handler" << " (at level "<< i<<")";
    }

    SC_CRITICAL(()) << "SC_CRITICAL";
    SC_WARN(()) << "SC_WARN";
    SC_INFO(()) << "SC_INFO";
    SC_DEBUG(()) << "SC_DEBUG";
    SC_TRACE(()) << "SC_TRACE";
  }
};

class scp_logger_test : public sc_log::sc_log_global_logger_handler {
  sc_log::log_levels operator()(struct sc_log::sc_log_logger_cache &logger,
                                std::string_view scname,
                                const char *tname) const {
    if (logger.features.size() && logger.features[0] == "test_handler") {
      return sc_log::log_levels::INFO;
    }
    if (scname == "sc_log_test") {
      /* test every time, and dont cache */
      return sc_log::log_levels::WARN;
    }
    /* Cache this one which will catch the normal SCMOD case for mod_a */
    logger.level = sc_log::log_levels::TRACE;
    return sc_log::log_levels::TRACE;
  }
};
static scp_logger_test test_logger_handler;

void report_handler(const sc_core::sc_report& rep, const sc_core::sc_actions& actions)
{
    cout << "TEST REPORT: "<<sc_log::as_log(rep.get_verbosity()) << " : [" << rep.get_msg_type() <<"] "<< rep.get_msg() <<std::endl;
}


int sc_main(int, char *[]) {
  ::sc_core::sc_report_handler::set_verbosity_level(sc_core::SC_DEBUG);
  ::sc_core::sc_report_handler::set_handler(report_handler);
  for (int i = 0; i < 500; i += 50) {
    cout << i << " is log_level " << sc_log::as_log(i) << endl;
  }
  cout << "Test string based handler" << std::endl;
  for (auto l : sc_log::log_level_map) {
    SC_LOG_AT(l.first, "sc_log_test") << l.second;
  }

  cout << "test FMT string" << std::endl;
  SC_WARN()("Testing FMT Hello {}", "world");

  cout << "construct module" << std::endl;

  mod_a amod("MyMod");

  return 0;
}
