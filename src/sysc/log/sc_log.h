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

#ifndef _SC_LOG_H_
#define _SC_LOG_H_

#define SC_HAS_SC_LOG

#include <array>
#include <cstring>
#include <format>
#include <iostream>
#include <map>
#include <sstream>
#include <type_traits>
#include <vector>

#include <sysc/log/sc_log_types.h>

// Declaration of global default logger (defined in sc_log.cpp, in global
// namespace)
extern sc_core::sc_log_logger_cache SC_LOG_LOG_LEVEL_CACHE;

/******************************************************************************
 * PRIVATE IMPLEMENTATION HELPER MACROS
 *
 * These macros are internal implementation details and should NOT be used
 * directly by user code. They are prefixed with SC_LOG_PRIV__ to indicate
 * they are private implementation details.
 ******************************************************************************/

// 2-step token pasting (forces expansion before concatenation)
#define SC_LOG_PRIV__CAT_IMPL(a, b) a##b
#define SC_LOG_PRIV__CAT(a, b) SC_LOG_PRIV__CAT_EVAL(a, b)
#define SC_LOG_PRIV__CAT_EVAL(a, b) SC_LOG_PRIV__CAT_IMPL(a, b)

// Argument counting: supports 0, 1, or 2 arguments
#define SC_LOG_PRIV__NARG(...) SC_LOG_PRIV__NARG_IMPL(0, ##__VA_ARGS__, 2, 1, 0)
#define SC_LOG_PRIV__NARG_IMPL(_0, _1, _2, N, ...) N

// Dispatch to appropriate macro based on argument count
#define SC_LOG_PRIV__DISPATCH(func, ...)                                       \
  SC_LOG_PRIV__DISPATCH_IMPL(func, SC_LOG_PRIV__NARG(__VA_ARGS__), __VA_ARGS__)
#define SC_LOG_PRIV__DISPATCH_IMPL(func, count, ...)                           \
  SC_LOG_PRIV__CAT(func, count)(__VA_ARGS__)

// Detect whether a macro argument is a "logger handle" or a "tag"
#define SC_LOG_PRIV__IS_LOGGER_HANDLE(x)                                       \
  std::is_same_v<std::decay_t<decltype(x)>, sc_core::sc_log_logger_cache>

// Note: SC_LOG_PRIV__FMT_EMPTY_STR is both a const char* and a function-like
// macro When used without parentheses, it's the empty string; with parentheses,
// it calls std::format
static const char *SC_LOG_PRIV__FMT_EMPTY_STR = "";
#define SC_LOG_PRIV__FMT_EMPTY_STR(...) std::format(__VA_ARGS__)

// Internal verbosity check variants (used by public SC_LOG_AT macro)
#define SC_LOG_PRIV__VBSTY_CHECK0(lvl)                                         \
  ((SC_LOG_LOG_LEVEL_CACHE.level >= (lvl)) &&                                  \
   (SC_LOG_LOG_LEVEL_CACHE.get_log_verbosity_cached(__FILE__, __LINE__) >=     \
    (lvl)))

#define SC_LOG_PRIV__VBSTY_CHECK1(lvl, arg1)                                   \
  ([&](auto &&x) -> bool {                                                     \
    if constexpr (SC_LOG_PRIV__IS_LOGGER_HANDLE(x)) {                          \
      return ((x.level >= (lvl)) &&                                            \
              (x.get_log_verbosity_cached(__FILE__, __LINE__) >= (lvl)));      \
    } else {                                                                   \
      return SC_LOG_PRIV__VBSTY_CHECK2(lvl, SC_LOG_LOG_LEVEL_CACHE, x);        \
    }                                                                          \
  }(arg1))

#define SC_LOG_PRIV__VBSTY_CHECK2(lvl, logger, tag)                            \
  ((logger.level >= lvl) &&                                                    \
   (logger.get_log_verbosity_cached(__FILE__, __LINE__, tag) >= lvl))

#define SC_LOG_PRIV__VBSTY_CHECK_IMPL(count, lvl, ...)                         \
  SC_LOG_PRIV__CAT(SC_LOG_PRIV__VBSTY_CHECK, count)(lvl, ##__VA_ARGS__)

// Internal tag extraction variants (used by public SC_LOG_MSG macro)
#define SC_LOG_PRIV__GET_TAG0()                                                \
  (SC_LOG_LOG_LEVEL_CACHE.tag.empty() ? SC_LOG_LOG_LEVEL_CACHE.scname.data()   \
                                      : SC_LOG_LOG_LEVEL_CACHE.tag.data())

#define SC_LOG_PRIV__GET_TAG1(arg1)                                            \
  ([&](auto &&x) -> const char * {                                             \
    if constexpr (SC_LOG_PRIV__IS_LOGGER_HANDLE(x)) {                          \
      return (x.tag.empty() ? x.scname.data() : x.tag.data());                 \
    } else {                                                                   \
      return x;                                                                \
    }                                                                          \
  }(arg1))

#define SC_LOG_PRIV__GET_TAG2(logger, tag) tag

// Internal handle variants (used by public SC_LOG_HANDLE macro)
#define SC_LOG_PRIV__HANDLE0()                                                 \
  sc_core::sc_log_logger_cache SC_LOG_LOG_LEVEL_CACHE =                        \
      sc_core::sc_log_handle_factory::make(sc_core::sc_log_level::UNSET, "",   \
                                           this)

#define SC_LOG_PRIV__HANDLE1(tag_str)                                          \
  sc_core::sc_log_logger_cache SC_LOG_LOG_LEVEL_CACHE =                        \
      sc_core::sc_log_handle_factory::make(sc_core::sc_log_level::UNSET,       \
                                           tag_str, this)

#define SC_LOG_PRIV__HANDLE2(logger_name, tag_str)                             \
  sc_core::sc_log_logger_cache SC_LOG_PRIV__HANDLE_NAME(logger_name) =         \
      sc_core::sc_log_handle_factory::make(sc_core::sc_log_level::UNSET,       \
                                           tag_str, this)

// Internal static handle variants (used by public SC_LOG_HANDLE_STATIC macro)
#define SC_LOG_PRIV__HANDLE_STATIC1(tag_str)                                   \
  static sc_core::sc_log_logger_cache SC_LOG_LOG_LEVEL_CACHE =                 \
      sc_core::sc_log_handle_factory::make_static(                             \
          sc_core::sc_log_level::UNSET, tag_str)

#define SC_LOG_PRIV__HANDLE_STATIC2(logger_name, tag_str)                      \
  static sc_core::sc_log_logger_cache SC_LOG_PRIV__HANDLE_NAME(logger_name) =  \
      sc_core::sc_log_handle_factory::make_static(                             \
          sc_core::sc_log_level::UNSET, tag_str)

// Helper to get logger handle name
#define SC_LOG_PRIV__HANDLE_NAME(x) x

/******************************************************************************
 * PUBLIC API MACROS
 *
 * These macros form the public logging API and should be used by user code.
 ******************************************************************************/

/**
 * SC_LOG_HANDLE - Declare a logger handle in a class
 *
 * Usage:
 *   SC_LOG_HANDLE()                    // Default logger with no tag
 *   SC_LOG_HANDLE("MyTag")             // Default logger with tag
 *   SC_LOG_HANDLE(my_logger, "MyTag") // Named logger with tag
 */
#define SC_LOG_HANDLE(...)                                                     \
  SC_LOG_PRIV__DISPATCH(SC_LOG_PRIV__HANDLE, ##__VA_ARGS__)

/**
 * SC_LOG_HANDLE_STATIC - Declare a static/global logger handle
 *
 * Usage:
 *   SC_LOG_HANDLE_STATIC("MyTag")             // static logger with tag
 *   SC_LOG_HANDLE_STATIC(my_logger, "MyTag") // Named static logger with tag
 *
 * Note: The 0 and 1 argument forms create a file-local static logger that
 *       shadows the global SC_LOG_LOG_LEVEL_CACHE within that translation unit.
 *       This is useful for .cpp files that want their own default logger.
 */
#define SC_LOG_HANDLE_STATIC(...)                                              \
  SC_LOG_PRIV__DISPATCH(SC_LOG_PRIV__HANDLE_STATIC, ##__VA_ARGS__)

/**
 * SC_LOG_HANDLE_VECTOR - Declare a vector of logger handles
 */
#define SC_LOG_HANDLE_VECTOR(NAME)                                             \
  std::vector<sc_core::sc_log_logger_cache> SC_LOG_PRIV__HANDLE_NAME(NAME)

/**
 * SC_LOG_HANDLE_VECTOR_PUSH_BACK - Add a logger to a vector
 */
#define SC_LOG_HANDLE_VECTOR_PUSH_BACK(NAME, tag_str)                          \
  SC_LOG_PRIV__HANDLE_NAME(NAME).push_back(                                    \
      {sc_core::sc_log_level::UNSET, "", tag_str})

/**
 * SC_LOG_AT - Log a message at a specific level
 *
 * Usage:
 *   SC_LOG_AT(sc_core::sc_log_level::INFO) << "message";
 *   SC_LOG_AT(sc_core::sc_log_level::INFO, my_logger) << "message";
 *   SC_LOG_AT(sc_core::sc_log_level::INFO, "tag") << "message";
 */
#define SC_LOG_AT(lvl, ...)                                                    \
  if (SC_LOG_PRIV__VBSTY_CHECK_IMPL(SC_LOG_PRIV__NARG(__VA_ARGS__), lvl,       \
                                    ##__VA_ARGS__))                            \
  ::sc_core::sc_logger<::sc_core::SC_INFO, false>(__FILE__, __LINE__, lvl)     \
          .type(SC_LOG_PRIV__DISPATCH(SC_LOG_PRIV__GET_TAG, ##__VA_ARGS__))    \
          .get()                                                               \
      << SC_LOG_PRIV__FMT_EMPTY_STR

/**
 * Convenience macros for common log levels
 *
 * Usage:
 *   SC_CRITICAL() << "message";
 *   SC_WARN(my_logger) << "message";
 *   SC_INFO("tag") << "message";
 */
#define SC_CRITICAL(...)                                                       \
  SC_LOG_AT(sc_core::sc_log_level::CRITICAL, ##__VA_ARGS__)
#define SC_WARN(...) SC_LOG_AT(sc_core::sc_log_level::WARN, ##__VA_ARGS__)
#define SC_INFO(...) SC_LOG_AT(sc_core::sc_log_level::INFO, ##__VA_ARGS__)
#define SC_DEBUG(...) SC_LOG_AT(sc_core::sc_log_level::DEBUG, ##__VA_ARGS__)
#define SC_TRACE(...) SC_LOG_AT(sc_core::sc_log_level::TRACE, ##__VA_ARGS__)

#endif /* _SC_LOG_H_ */
