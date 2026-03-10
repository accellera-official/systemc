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
  sc_log.h --SystemC logging functions.
  Original Author: Eyck Jentzsch, MINRES Technologies GmbH
                   Mark Burton, Qualcomm Technologies, Inc.

  CHANGE LOG AT THE END OF THE FILE
 *****************************************************************************/

#include "sysc/kernel/sc_simcontext.h"
#include "sysc/log/sc_log_types.h"
#include <map>
#include <string_view>
#include <unordered_map>

namespace sc_core {

// Definition of log level map (declared as extern in sc_log_types.h)
// This avoids creating duplicate copies in each translation unit.
const std::map<sc_log_level, std::string> log_level_map = {
    {sc_log_level::CRITICAL, "CRITICAL"},
    {sc_log_level::NONE, "NONE"},
    {sc_log_level::WARN, "WARN"},
    {sc_log_level::INFO, "INFO"},
    {sc_log_level::DEBUG, "DEBUG"},
    {sc_log_level::TRACE, "TRACE"}
};

sc_log_level sc_log_logger_cache::get_log_verbosity_cached(
    const char *file, int line, std::string_view local_tag) {
  if (level != sc_log_level::UNSET) {
    return level;
  }

  return sc_log_impl::sc_get_log_verbosity(*this, file, line, local_tag);
}

} // namespace sc_core

// Global default logger with empty tag (in global namespace for proper name
// shadowing). This logger is used when no specific logger handle is provided.
// Note: Using string_view{} for empty views and nullptr for typename_str.
sc_core::sc_log_logger_cache SC_LOG_LOG_LEVEL_CACHE{
    sc_core::sc_log_level::UNSET,  // level
    std::string_view{},             // tag (empty)
    std::string_view{},             // scname (empty)
    nullptr                         // typename_str (no type info)
};
