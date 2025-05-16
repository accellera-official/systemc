/*******************************************************************************
 * Copyright 2017-2022 MINRES Technologies GmbH
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *******************************************************************************/
/*
 *  Created on: 19.09.2017
 *      Author: eyck@minres.com
 */ 

#include "sysc/log/sc_log_types.h"
#include "sysc/kernel/sc_simcontext.h"
#include <unordered_map>

namespace {
// Making this thread_local could cause thread copies of the same cache
// entries, but more likely naming will be thread local too, and this avoids
// races in the unordered_map

#ifdef DISABLE_REPORT_THREAD_LOCAL
std::unordered_map<uint64_t, sc_core::log_levels> lut;
#else
thread_local std::unordered_map<uint64_t, sc_core::log_levels> lut;
#endif

// BKDR hash algorithm
auto char_hash(char const *str) -> uint64_t {
  constexpr unsigned int seed = 131; // 31  131 1313 13131131313 etc//
  uint64_t hash = 0;
  while (*str) {
    hash = (hash * seed) + (*str);
    str++;
  }
  return hash;
}
} // namespace

sc_core::log_levels
sc_core::sc_log_logger_cache::get_log_verbosity_cached(const char *scname,
                                                      const char *tname = "") {
  if (level != sc_core::log_levels::UNSET) {
    return level;
  }

  if (!scname && features.size())
    scname = features[0].c_str();
  if (!scname)
    scname = "";

  type = std::string(scname);

  return sc_core::sc_get_curr_simcontext()->get_log_verbosity(*this, scname,
                                                              tname);
}

auto sc_core::get_log_verbosity(char const *str) -> sc_core::log_levels {
  auto k = char_hash(str);
  auto it = lut.find(k);
  if (it != lut.end()) {
    return it->second;
  }

  sc_core::sc_log_logger_cache tmp;
  lut[k] = tmp.get_log_verbosity_cached(str);
  return lut[k];
}

sc_core::sc_log_global_logger_handler::sc_log_global_logger_handler() {
  std::function<sc_core::log_levels(sc_core::sc_log_logger_cache &, const char *,
                                   const char *)>
      fn = [&](sc_core::sc_log_logger_cache &logger, const char *sc_name,
               const char *t_name) -> sc_core::log_levels {
    return operator()(logger, sc_name, t_name);
  };
  ::sc_core::sc_get_curr_simcontext()->set_log_verbosity_fn(fn);
  ::sc_core::sc_report_handler::set_verbosity_level(
      sc_core::SC_DEBUG); // Set the level in the core to DEBUG such that the
                          // handler can manage all levels of verbosity
}
