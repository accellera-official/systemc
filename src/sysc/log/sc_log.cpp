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


#include "sysc/log/sc_log_types.h"
#include "sysc/kernel/sc_simcontext.h"
#include <unordered_map>

namespace {
// Making this thread_local could cause thread copies of the same cache
// entries, but more likely naming will be thread local too, and this avoids
// races in the unordered_map

#ifdef DISABLE_REPORT_THREAD_LOCAL
std::unordered_map<uint64_t, sc_core::sc_log_level> lut;
#else
thread_local std::unordered_map<uint64_t, sc_core::sc_log_level> lut;
#endif

// BKDR hash algorithm
auto char_hash(std::string_view str) -> uint64_t {
  constexpr unsigned int seed = 131; // 31  131 1313 13131131313 etc//
  uint64_t hash = 0;
  for (char c: str) {
    hash = (hash * seed) + static_cast<unsigned char>(c);
  }
  return hash;
}
} // namespace

sc_core::sc_log_level
sc_core::sc_log_logger_cache::get_log_verbosity_cached(const char *file, int line, std::string_view scname,
                                                      const char *tname = "") {
  if (level != sc_core::sc_log_level::UNSET) {
    return level;
  }

  if (!scname.data() && features.size())
    scname = features[0];
  if (!scname.data())
    scname = "";

  type = scname;

  return sc_core::sc_log_impl::sc_get_log_verbosity(*this, file, line, scname, tname);
}

sc_core::sc_log_level sc_core::get_log_verbosity_uncached(const char *file, int line, std::string_view scname) {
  auto k = char_hash(scname);
  auto it = lut.find(k);
  if (it != lut.end()) {
    return it->second;
  }

  sc_core::sc_log_logger_cache tmp;
  lut[k] = tmp.get_log_verbosity_cached(file, line, scname);
  return lut[k];
}
