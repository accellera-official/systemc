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

#ifndef _SC_LOG_TYPES_H_
#define _SC_LOG_TYPES_H_

#include <array>
#include <cstring>
#include <iostream>
#include <map>
#include <sstream>
#include <vector>
#include <climits>

#include <sysc/utils/sc_report.h>

/** \ingroup sc_log
 *  @{
 */
/**@{*/
//! @brief Logging  utilities
namespace sc_core {

/************************
 * Provide a set of names and conversions that are suitable for logging levels
 *based on SystemC "verbosity's"
 ************************/

enum class log_levels {
  NONE = sc_core::SC_NONE,
  CRITICAL = sc_core::SC_NONE,
  WARN = sc_core::SC_LOW,
  INFO = sc_core::SC_MEDIUM,
  DEBUG = sc_core::SC_HIGH,
  TRACE = sc_core::SC_DEBUG,

  UNSET = INT_MAX
};

const static std::map<log_levels, std::string> log_level_map = {
    {log_levels::CRITICAL, "CRITICAL"}, {log_levels::NONE, "NONE"},
    {log_levels::WARN, "WARN"},         {log_levels::INFO, "INFO"},
    {log_levels::DEBUG, "DEBUG"},       {log_levels::TRACE, "TRACE"}};

/**
 * @fn log as_log(int)
 * @brief safely convert an integer into a log level
 *
 * @param logLevel the logging level
 * @return the log level
 */
inline log_levels as_log(int logLevel) {
  auto m = log_level_map;
  for (auto l : m) {
    if (logLevel <= static_cast<int>(l.first)) {
      return l.first;
    }
  }
  return log_levels::TRACE;
}

/**
 * @fn log as_log(std::string)
 * @brief safely convert a string into a log level
 *
 * @param logName the string name for the log level
 * @return the log level
 */
inline log_levels as_log(std::string logName) {
  auto m = log_level_map;
  for (auto l : m) {
    if (logName == l.second)
      return l.first;
  }
  return log_levels::TRACE;
}
/**
 * @fn std::istream& operator >>(std::istream&, log&)
 * @brief read a log level from input stream e.g. used by boost::lexical_cast
 *
 * @param is input stream holding the string representation
 * @param val the value holding the resulting value
 * @return the input stream
 */
inline std::istream &operator>>(std::istream &is, log_levels &val) {
  std::string buf;
  is >> buf;
  val = as_log(buf);
  return is;
}
/**
 * @fn std::ostream& operator <<(std::ostream&, const log&)
 * @brief output the textual representation of the log level
 *
 * @param os output stream
 * @param val logging level
 * @return reference to the stream for chaining
 */
inline std::ostream &operator<<(std::ostream &os, log_levels const &val) {
  auto m = log_level_map;
  os << m[val];
  return os;
}

/* Convenience function to allow useage outside of SystemC heirarchy */
class call_sc_name_fn {
  template <class T>
  static auto test(T *p)
      -> decltype(p->name(), std::true_type());
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

/**
 * @brief cached logging information used in the (logger) form.
 *
 */
struct sc_log_logger_cache {
  log_levels level = log_levels::UNSET;
  std::string type = "";
  std::vector<std::string> features;

  /**
   * @brief Initialize the verbosity cache and/or return the cached value.
   *
   * @return log
   */
  log_levels get_log_verbosity_cached(const char *, const char *);
};

class sc_log_global_logger_handler {
public:
  virtual log_levels operator()(struct sc_log_logger_cache &logger,
                                std::string_view scname,
                                const char *tname) const = 0;
  sc_log_global_logger_handler();
};

inline log_levels get_log_verbosity() {
  return static_cast<log_levels>(
      ::sc_core::sc_report_handler::get_verbosity_level());
}
/**
 * @fn sc_core::sc_verbosity get_log_verbosity(const char*)
 * @brief get the scope-based verbosity level
 *
 * The function returns a scope specific verbosity level if defined (e.g. by
 * using a CCI param named "log_level"). Otherwise the global verbosity level
 * is being returned
 *
 * @param t the SystemC hierarchy scope name
 * @return the verbosity level
 */

log_levels get_log_verbosity(char const *t);
/**
 * @fn sc_core::sc_verbosity get_log_verbosity(const char*)
 * @brief get the scope-based verbosity level
 *
 * The function returns a scope specific verbosity level if defined (e.g. by
 * using a CCI param named "log_level"). Otherwise the global verbosity level
 * is being returned
 *
 * @param t the SystemC hierarchy scope name
 * @return the verbosity level
 */
inline log_levels get_log_verbosity(std::string const &t) {
  return get_log_verbosity(t.c_str());
}

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
           log_levels verbosity = sc_core::log_levels::INFO)
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
        SEVERITY, t ? t : "SystemC", os.str().c_str(),
        static_cast<sc_core::sc_verbosity>(level), file, line);
    sc_core::sc_report_handler::set_actions(SEVERITY, old);
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
   * @param t type of th elog entry
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
   * @param t type of th elog entry
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
  const log_levels level;
};

} // namespace sc_core

#define SC_LOG_LOG_LEVEL_CACHE _m_sc_log_log_level_cache_

/** @} */ // end of sc_log
#endif    /* _SC_LOG_TYPES_H_ */
