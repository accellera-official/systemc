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
#include <unordered_set>

SC_MODULE(mod_a) {
  SC_LOG_HANDLE((TST), "test_handler");
  SC_CTOR(mod_a) {
    for (auto l : sc_core::log_level_map) {
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


/**********************************************************
 * The mechanism by which SC_LOG macros are enabled and
 * disabled is implementation-defined.
 *
 * The Accellera POC provides functions and a class for this
 * purpose.
 *
 * --------------------------------------------------------
 * Non-standard API:
 *
 * Install a callback used to determine the effective log
 * level for a given (sc_name, typ_name) pair.
 *
 * The cache parameter may be used to remember a computed
 * level. Once set, the function will not be re-called.
 * --------------------------------------------------------
 *
 * void sc_set_log_verbosity_fn(
 *     std::function<
 *         sc_core::log_levels(
 *             sc_core::sc_log_logger_cache&,
 *             const char*,
 *             const char*
 *         )
 *     > fn
 * );
 *
 * --------------------------------------------------------
 * Non-standard API:
 *
 * Query the current log verbosity for the given cache and
 * identifiers.
 *
 * If no callback has been installed, the implementation
 * shall fall back to the global report verbosity.
 * --------------------------------------------------------
 *
 * sc_core::log_levels sc_get_log_verbosity(
 *     sc_core::sc_log_logger_cache &logger,
 *     const char *sc_name,
 *     const char *typ_name);
 * );
 *
 * Together with the sc_log_logger_cache
 * class, this is used by the SC_LOG macros to determine
 * whether specific loggers should be enabled.
 **********************************************************/

/**********************************************************
 * Example:
 *
 * This demonstrates how one could construct a class around
 * the basic set_log_verbosity_fn API.
 *
 * A tool could use this to build additional functionality
 * (e.g. resetting cached values). This behavior is not
 * part of the standard, and tool environments may differ.
 **********************************************************/


class scp_logger_test {
  std::unordered_set<sc_core::sc_log_logger_cache*> loggers;
  sc_core::log_levels operator()(struct sc_core::sc_log_logger_cache &logger,
                                std::string_view scname,
                                const char *tname) {
    loggers.insert(&logger);
    if (logger.features.size() && logger.features[0] == "test_handler") {
      return sc_core::log_levels::INFO;
    }
    if (scname == "sc_log_test") {
      /* test every time, and dont cache */
      return sc_core::log_levels::WARN;
    }
    /* Cache this one which will catch the normal SCMOD case for mod_a */
    logger.level = sc_core::log_levels::TRACE;
    return sc_core::log_levels::TRACE;
  }
public:
  scp_logger_test() {
    std::function<sc_core::log_levels(sc_core::sc_log_logger_cache &,
                                      const char *, const char *)>
        fn = [&](sc_core::sc_log_logger_cache &logger, const char *sc_name,
                 const char *t_name) -> sc_core::log_levels {
      return operator()(logger, sc_name, t_name);
    };
    ::sc_core::sc_get_curr_simcontext()->set_log_verbosity_fn(fn);
    ::sc_core::sc_report_handler::set_verbosity_level(
        sc_core::SC_DEBUG); // Set the level in the core to DEBUG such that the
                            // handler can manage all levels of verbosity
  }
  void reset() {
    for (auto *logger : loggers) {
      if (logger) {
        logger->level = sc_core::log_levels::UNSET;
      }
    }
  }
};

static scp_logger_test test_logger_handler;

void report_handler(const sc_core::sc_report& rep, const sc_core::sc_actions& actions)
{
    cout << "TEST REPORT: "<<sc_core::as_log(rep.get_verbosity()) << " : [" << rep.get_msg_type() <<"] "<< rep.get_msg() <<std::endl;
}


int sc_main(int, char *[]) {
  ::sc_core::sc_report_handler::set_verbosity_level(sc_core::SC_DEBUG);
  ::sc_core::sc_report_handler::set_handler(report_handler);
  for (int i = 0; i < 500; i += 50) {
    cout << i << " is log_level " << sc_core::as_log(i) << endl;
  }
  cout << "Test string based handler" << std::endl;
  for (auto l : sc_core::log_level_map) {
    SC_LOG_AT(l.first, "sc_log_test") << l.second;
  }

  cout << "test FMT string" << std::endl;
  SC_WARN()("Testing FMT Hello {}", "world");

  cout << "construct module" << std::endl;

  mod_a amod("MyMod");

  return 0;
}
