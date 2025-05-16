/*******************************************************************************
 * Copyright 2016-2022 MINRES Technologies GmbH
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

#ifndef _SC_LOG_H_
#define _SC_LOG_H_

#define SC_HAS_SC_LOG

#include <array>
#include <cstring>
#include <iostream>
#include <map>
#include <sstream>
#include <vector>
#include <format>

#include <sysc/log/sc_log_types.h>

// must be global for macro to work.
static const char *_SC_LOG_FMT_EMPTY_STR = "";

/**
 * logging macros
 */

/**
 * Boilerplate convenience macros
 */
#define _SC_LOG_CAT(a, ...) _SC_LOG_PRIMITIVE_CAT(a, __VA_ARGS__)
#define _SC_LOG_PRIMITIVE_CAT(a, ...) a##__VA_ARGS__

#define _SC_LOG_IIF(c) _SC_LOG_PRIMITIVE_CAT(_SC_LOG_IIF_, c)
#define _SC_LOG_IIF_0(t, ...) __VA_ARGS__
#define _SC_LOG_IIF_1(t, ...) t

#define _SC_LOG_CHECK_N(x, n, ...) n
#define _SC_LOG_CHECK(...) _SC_LOG_CHECK_N(__VA_ARGS__, 0, )
#define _SC_LOG_PROBE(x) x, 1,

#define _SC_LOG_EXPAND(...) __VA_ARGS__

#define _SC_LOG_FIRST_ARG(f, ...) f
#define _SC_LOG_POP_ARG(f, ...) __VA_ARGS__

#define _SC_LOG_IS_PAREN(x) _SC_LOG_CHECK(_SC_LOG_IS_PAREN_PROBE x)
#define _SC_LOG_IS_PAREN_PROBE(...) _SC_LOG_PROBE(~)
/********/

/* default logger cache name */
#define SC_LOG_HANDLE_NAME(x) _SC_LOG_CAT(SC_LOG_LOG_LEVEL_CACHE, x)

/* User interface macros */
#define SC_LOG_HANDLE(...)                                                     \
  sc_core::sc_log_logger_cache _SC_LOG_IIF(                                     \
      _SC_LOG_IS_PAREN(_SC_LOG_FIRST_ARG(__VA_ARGS__)))(                       \
      SC_LOG_HANDLE_NAME(                                                      \
          _SC_LOG_EXPAND(_SC_LOG_FIRST_ARG _SC_LOG_FIRST_ARG(__VA_ARGS__))),   \
      SC_LOG_HANDLE_NAME()) = {                                                \
      sc_core::log_levels::UNSET,                                               \
      "",                                                                      \
      {_SC_LOG_IIF(_SC_LOG_IS_PAREN(_SC_LOG_FIRST_ARG(__VA_ARGS__)))(          \
          _SC_LOG_POP_ARG(__VA_ARGS__), ##__VA_ARGS__)}}

#define SC_LOG_HANDLE_VECTOR(NAME)                                             \
  std::vector<sc_core::sc_log_logger_cache> SC_LOG_HANDLE_NAME(NAME)
#define SC_LOG_HANDLE_VECTOR_PUSH_BACK(NAME, ...)                              \
  SC_LOG_HANDLE_NAME(NAME).push_back(                                          \
      {sc_core::log_levels::UNSET, "", {__VA_ARGS__}});

// critical thing is that the initial if 'fails' as soon as possible - if it is
// going to pass, we have all the time we want, as we will be logging anyway
// This HAS to be done as a macro, because the first argument may be a string
// or a cache'd level

/*** Helper macros for SC_LOG_ report macros ****/
#define SC_LOG_VBSTY_CHECK_CACHED(lvl, features, cached, ...)                     \
  (cached.level >= lvl) &&                                                     \
      (cached.get_log_verbosity_cached(sc_core::call_sc_name_fn()(this),        \
                                       typeid(*this).name()) >= lvl)

#define SC_LOG_VBSTY_CHECK_UNCACHED(lvl, ...)                                     \
  (::sc_core::get_log_verbosity(__VA_ARGS__) >= lvl)

#define SC_LOG_VBSTY_CHECK(lvl, ...)                                              \
  _SC_LOG_IIF(_SC_LOG_IS_PAREN(_SC_LOG_FIRST_ARG(__VA_ARGS__)))                \
  (SC_LOG_VBSTY_CHECK_CACHED(                                                     \
       lvl, _SC_LOG_FIRST_ARG(__VA_ARGS__),                                    \
       SC_LOG_HANDLE_NAME(                                                     \
           _SC_LOG_EXPAND(_SC_LOG_FIRST_ARG _SC_LOG_FIRST_ARG(__VA_ARGS__)))), \
   SC_LOG_VBSTY_CHECK_UNCACHED(lvl, ##__VA_ARGS__))

#define SC_LOG_GET_FEATURES(...)                                                  \
  _SC_LOG_IIF(_SC_LOG_IS_PAREN(_SC_LOG_FIRST_ARG(__VA_ARGS__)))                \
  (_SC_LOG_FIRST_ARG _SC_LOG_EXPAND((_SC_LOG_POP_ARG(                          \
       __VA_ARGS__,                                                            \
       SC_LOG_HANDLE_NAME(                                                     \
           _SC_LOG_EXPAND(_SC_LOG_FIRST_ARG _SC_LOG_FIRST_ARG(__VA_ARGS__)))   \
           .type))),                                                           \
   __VA_ARGS__)

#define _SC_LOG_FMT_EMPTY_STR(...) std::format(__VA_ARGS__)

#define SC_LOG_MSG(lvl, ...)                                                      \
  ::sc_core::sc_logger<::sc_core::SC_INFO, false>(__FILE__, __LINE__, lvl)       \
          .type(SC_LOG_GET_FEATURES(__VA_ARGS__))                                 \
          .get()                                                               \
      << _SC_LOG_FMT_EMPTY_STR
/*** End HELPER Macros *******/

#define SC_LOG_AT(lvl, ...)                                                    \
  if (SC_LOG_VBSTY_CHECK(lvl, __VA_ARGS__))                                       \
  SC_LOG_MSG(lvl, __VA_ARGS__)

#define SC_CRITICAL(...) SC_LOG_AT(sc_core::log_levels::CRITICAL, __VA_ARGS__)
#define SC_WARN(...) SC_LOG_AT(sc_core::log_levels::WARN, __VA_ARGS__)
#define SC_INFO(...) SC_LOG_AT(sc_core::log_levels::INFO, __VA_ARGS__)
#define SC_DEBUG(...) SC_LOG_AT(sc_core::log_levels::DEBUG, __VA_ARGS__)
#define SC_TRACE(...) SC_LOG_AT(sc_core::log_levels::TRACE, __VA_ARGS__)

#endif    /* _SC_LOG_H_ */
