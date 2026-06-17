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
  sc_log_types.h -- SystemC logging type definitions and utilities.
  Original Author: Eyck Jentzsch, MINRES Technologies GmbH
                   Mark Burton, Qualcomm Technologies, Inc.

  CHANGE LOG AT THE END OF THE FILE
 *****************************************************************************/

#ifndef _SC_LOG_TYPES_H_
#define _SC_LOG_TYPES_H_

#include <array>
#include <climits>
#include <cstring>
#include <functional>
#include <iostream>
#include <map>
#include <sstream>
#include <string>
#include <string_view>
#include <typeinfo>
#include <vector>

#include <sysc/utils/sc_report.h>

/** \ingroup sc_log
 *  @{
 */
/**@{*/
//! @brief Logging  utilities
namespace sc_core {

/************************
 * Logging levels are expressed directly as sc_core::sc_verbosity values:
 *   SC_LOW (100)=CRITICAL  SC_MEDIUM (200)=ALERT  SC_HIGH (300)=NOTE
 *   SC_FULL (400)=DETAIL   SC_DEBUG (500)=INTERNAL.  SC_NONE (0) is "off".
 *
 * A logging level is carried as a plain int (SC_LOG_AT accepts any
 * user-defined level), so the logger cache stores an int and uses the
 * sentinel below for "not yet resolved / inherit from context".
 ************************/

// "Level not yet resolved" sentinel for the logger cache.  This is an
// internal implementation marker (NOT part of the IEEE 1666 standard) and a
// plain int — deliberately NOT an sc_verbosity enumerator — so it never
// appears in the standardized sc_verbosity enum.
inline constexpr int SC_UNSET = INT_MAX;

/**
 * @fn sc_verbosity as_log(int)
 * @brief bucket a raw verbosity to the nearest level at or above it
 *
 * v <= 100 -> CRITICAL, <= 200 -> ALERT, <= 300 -> NOTE,
 * <= 400 -> DETAIL, otherwise INTERNAL.
 *
 * @param logLevel the raw verbosity
 * @return the bucketed log level
 */
inline sc_verbosity as_log(int logLevel) {
  if (logLevel <= sc_core::SC_LOW)    return sc_core::SC_LOW;
  if (logLevel <= sc_core::SC_MEDIUM) return sc_core::SC_MEDIUM;
  if (logLevel <= sc_core::SC_HIGH)   return sc_core::SC_HIGH;
  if (logLevel <= sc_core::SC_FULL)   return sc_core::SC_FULL;
  return sc_core::SC_DEBUG;
}

/* Convenience helper to detect if a type has a name() method.
 * This allows logging to work both inside and outside of SystemC hierarchy. */
class sc_log_priv__call_sc_name_fn {
  template <class T>
  static auto test(T *p) -> decltype(p->name(), std::true_type());
  template <class T> static auto test(...) -> decltype(std::false_type());

  template <class T>
  static constexpr bool has_method = decltype(test<T>(nullptr))::value;

public:
  // define a function IF the method exists
  template <class TYPE>
  auto operator()(TYPE *p) const
      -> std::enable_if_t<has_method<TYPE>, const char *> {
    return p->name();
  }

  // define a function IF NOT the method exists
  template <class TYPE>
  auto operator()(TYPE *p) const
      -> std::enable_if_t<!has_method<TYPE>, const char *> {
    return nullptr;
  }
};

/******************/

// Forward declaration for sc_log_impl (defined in sc_simcontext.h)
struct sc_log_impl;

/**
 * @brief Cached logging information for a logger instance.
 *
 * Type choices rationale:
 * - tag: string_view - references string literals passed to SC_LOG_HANDLE macros
 * - scname: string_view - references the result of this->name() which has
 *   static storage duration in sc_object
 * - typename_str: const char* - directly stores the pointer returned by
 *   typeid().name() which has static storage duration
 *
 * These choices avoid unnecessary string copies while maintaining safety
 * since all referenced strings have appropriate lifetimes.
 */
struct sc_log_logger_cache {
  int level = SC_UNSET;
  std::string tag{};           // Logger tag/identifier (owns the string)
  std::string scname{};        // Captured sc_object name (owns the string)
  const char* typename_str = nullptr; // Captured type name (from typeid(*this).name())

  /**
   * @brief Initialize the verbosity cache and/or return the cached value.
   *
   * This function checks if the level is already cached, and if not,
   * queries the dynamic verbosity function.
   *
   * @param file source file name
   * @param line source line number
   * @param local_tag optional local tag that overrides the effective name
   * @return log level for this logger
   */
  int get_log_verbosity_cached(const char *file, int line,
                                        std::string_view local_tag = {});

  /**
   * @brief Set the tag to a runtime-computed string.
   *
   * The cached level is reset so the verbosity function re-evaluates
   * with the new tag on the next log statement.
   *
   * @param new_tag the new tag string
   */
  void set_tag(std::string new_tag);

  /// Get/set the logger cache that most recently performed a verbosity
  /// check on this thread.  Used by the report handler to access
  /// scname/tag/typename.
  ///
  /// Lifecycle: set by get_log_verbosity_cached() (called during the
  /// SC_LOG verbosity check), cleared by ~sc_logger() (after the report
  /// handler has run).  Accessor functions ensure correct linkage across
  /// shared library boundaries.
  static sc_log_logger_cache* get_current();
  static void set_current(sc_log_logger_cache* p);
};

/**
 * @brief Factory for constructing sc_log_logger_cache objects with captured
 * context.
 *
 * Note: C++ cannot infer an owning object's name/type from within the cache's
 * default constructor, so the owning object pointer must be provided here (when
 * available).
 *
 */
struct sc_log_handle_factory {
  template <class TYPE>
  static sc_log_logger_cache make(int lvl, const char *tag_str,
                                  TYPE *p) {
    const char *n = sc_log_priv__call_sc_name_fn{}(p);
    const char *t = typeid(*p).name();
    return sc_log_logger_cache{
        lvl,
        tag_str ? std::string(tag_str) : std::string(),
        n ? std::string(n) : std::string(),
        t  // typeid().name() returns const char* with static storage
    };
  }

  static sc_log_logger_cache make_static(int lvl,
                                         const char *tag_str) {
    return sc_log_logger_cache{
        lvl,
        tag_str ? std::string(tag_str) : std::string(),
        std::string(),
        nullptr  // No type information for static loggers
    };
  }
};

/**
 * @brief Return list of logging parameters that have been used
 *
 */
std::vector<std::string> get_logging_parameters();

/**
 * @struct sc_logger
 * @brief the logger class
 *
 * The sc_logger creates a RTTI based output stream to be used similar to
 * std::cout
 *
 * @tparam SEVERITY
 */
template <sc_core::sc_severity SEVERITY, bool WITH_ACTIONS = false>
struct sc_logger {
  /**
   * @fn  sc_logger(const char*, int, int=sc_core::SC_MEDIUM)
   * @brief
   *
   * @param file where the log entry originates
   * @param line number where the log entry originates
   * @param verbosity the log level
   */
  sc_logger(const char *file, int line,
            int verbosity = sc_core::SC_MEDIUM)
      : t(nullptr), file(file), line(line), level(verbosity) {}


  sc_logger() = delete;

  sc_logger(const sc_logger &) = delete;

  sc_logger(sc_logger &&) = delete;

  sc_logger &operator=(const sc_logger &) = delete;

  sc_logger &operator=(sc_logger &&) = delete;
  /**
   * @fn  ~sc_logger()
   * @brief the destructor generating the SystemC report
   *
   * NB a destructor should not throw an exception, here we attempt to prevent
   * the sc_report_handler from throwing The ScLogging interface is _ONLY_ for
   * logging, simulation control should happen in user code.
   */
  virtual ~sc_logger() noexcept(true) {
    auto old = sc_core::sc_report_handler::set_actions(SEVERITY);
    if (WITH_ACTIONS == false) {
      sc_core::sc_report_handler::set_actions(
          SEVERITY, old & ~(sc_core::SC_THROW | sc_core::SC_INTERRUPT |
                            sc_core::SC_STOP | sc_core::SC_ABORT));
    }
    ::sc_core::sc_report_handler::report(
        SEVERITY, (t && *t) ? t : "SystemC", os.str().c_str(),
        static_cast<sc_core::sc_verbosity>(level), file, line);
    sc_core::sc_report_handler::set_actions(SEVERITY, old);
    sc_log_logger_cache::set_current(nullptr);
  }
  /**
   * @fn sc_logger& type()
   * @brief reset the category of the log entry
   *
   * @return reference to self for chaining
   */
  inline sc_logger &type() {
    this->t = nullptr;
    return *this;
  }
  /**
   * @fn sc_logger& type(const char*)
   * @brief set the category of the log entry
   *
   * @param t type of the log entry
   * @return reference to self for chaining
   */
  inline sc_logger &type(char const *t) {
    this->t = const_cast<char *>(t);
    return *this;
  }
  /**
   * @fn sc_logger& type(std::string const&)
   * @brief set the category of the log entry
   *
   * @param t type of the log entry
   * @return reference to self for chaining
   */
  inline sc_logger &type(std::string const &t) {
    this->t = const_cast<char *>(t.c_str());
    return *this;
  }
  /**
   * @fn std::ostream& get()
   * @brief  get the underlying ostringstream
   *
   * @return the output stream collecting the log message
   */
  inline std::ostream &get() { return os; };

protected:
  std::ostringstream os{};
  char *t{nullptr};
  const char *file;
  const int line;
  const int level;
};

} // namespace sc_core

// This macro is intentionally in the global namespace to allow it to be
// used as a member variable name in user classes without namespace qualification.
// The macro expands to _m_sc_log_log_level_cache_ which uses a leading underscore
// to indicate it's an implementation detail. While identifiers with leading
// underscores are generally reserved, this pattern (_m_*) is safe as it doesn't
// conflict with reserved patterns (__* or _Capital*).
#define SC_LOG_LOG_LEVEL_CACHE _m_sc_log_log_level_cache_
#define SC_LOG_LOG_LEVEL_CACHE_GLOBAL ::_m_sc_log_log_level_cache_

/** @} */ // end of sc_log
#endif    /* _SC_LOG_TYPES_H_ */
