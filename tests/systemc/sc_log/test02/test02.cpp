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

  test02.cpp -- Comprehensive test of sc_log API

  Original Author: Mark Burton, Qualcomm Innovation Center, Inc. 2024

  DESCRIPTION: This test exercises all aspects of the sc_log API:
  - All logger handle declaration variants
  - All log level macros
  - Stream and format string syntax
  - Logger handles with and without tags
  - Static and member logger handles
  - Vector of logger handles
  - Tag-based logging

 *****************************************************************************/

#include "systemc.h"
#include <iostream>
#include <map>
#include <string>

// Core no longer ships a level<->name map (textual names are an SCP
// convenience).  Tests that print level names keep their own local map.
static const std::map<sc_core::sc_verbosity, std::string> k_level_names = {
    {sc_core::SC_LOW, "CRITICAL"}, {sc_core::SC_MEDIUM, "ALERT"},
    {sc_core::SC_HIGH, "NOTE"},    {sc_core::SC_FULL, "DETAIL"},
    {sc_core::SC_DEBUG, "INTERNAL"}};
static std::string level_name(sc_core::sc_verbosity v) {
  auto it = k_level_names.find(v);
  return it == k_level_names.end() ? std::string("NONE") : it->second;
}

// Test 1: Global static logger handle with tag
// Note: The default global SC_LOG_LOG_LEVEL_CACHE is already declared in sc_log.h
SC_LOG_HANDLE_STATIC(GLOBAL_LOGGER_WITH_TAG, "GlobalTag");

// Test 2: Module with default logger (no tag)
SC_MODULE(test_module_default) {
  SC_LOG_HANDLE();
  
  SC_CTOR(test_module_default) {
    cout << "\n=== Test Module with Default Logger ===" << endl;
    
    // Test all log levels with stream syntax
    SC_CRITICAL() << "CRITICAL: Default logger, stream syntax";
    SC_ALERT() << "ALERT: Default logger, stream syntax";
    SC_NOTE() << "NOTE: Default logger, stream syntax";
    SC_DETAIL() << "DETAIL: Default logger, stream syntax";
    SC_INTERNAL() << "INTERNAL: Default logger, stream syntax";
    
    // Test format string syntax
    SC_ALERT()("ALERT: Default logger, format syntax with arg: {}", 42);
    SC_NOTE()("NOTE: Default logger, format syntax with multiple args: {} {}", "hello", "world");
  }
};

// Test 3: Module with tagged logger
SC_MODULE(test_module_tagged) {
  SC_LOG_HANDLE("ModuleTag");
  
  SC_CTOR(test_module_tagged) {
    cout << "\n=== Test Module with Tagged Logger ===" << endl;
    
    SC_CRITICAL() << "CRITICAL: Tagged logger";
    SC_ALERT() << "ALERT: Tagged logger";
    SC_NOTE() << "NOTE: Tagged logger";
    SC_DETAIL() << "DETAIL: Tagged logger";
    SC_INTERNAL() << "INTERNAL: Tagged logger";
    
    SC_NOTE()("NOTE: Tagged logger with format: {}", 123);
  }
};

// Test 4: Module with multiple named loggers
SC_MODULE(test_module_multi_logger) {
  SC_LOG_HANDLE(logger1, "Logger1Tag");
  SC_LOG_HANDLE(logger2, "Logger2Tag");
  SC_LOG_HANDLE();  // Default logger
  
  SC_CTOR(test_module_multi_logger) {
    cout << "\n=== Test Module with Multiple Named Loggers ===" << endl;
    
    // Test logger1
    SC_NOTE(logger1) << "NOTE: Using logger1";
    SC_ALERT(logger1)("ALERT: Using logger1 with format: {}", "test");
    
    // Test logger2
    SC_NOTE(logger2) << "NOTE: Using logger2";
    SC_DETAIL(logger2)("DETAIL: Using logger2 with format: {}", 456);
    
    // Test default logger
    SC_NOTE() << "NOTE: Using default logger";
    SC_INTERNAL()("INTERNAL: Using default logger with format");
  }
};

// Test 5: Module with vector of loggers
SC_MODULE(test_module_vector) {
  SC_LOG_HANDLE_VECTOR(logger_vec);
  
  SC_CTOR(test_module_vector) {
    cout << "\n=== Test Module with Vector of Loggers ===" << endl;
    
    // Populate vector
    SC_LOG_HANDLE_VECTOR_PUSH_BACK(logger_vec, "VecLogger0");
    SC_LOG_HANDLE_VECTOR_PUSH_BACK(logger_vec, "VecLogger1");
    SC_LOG_HANDLE_VECTOR_PUSH_BACK(logger_vec, "VecLogger2");
    
    // Use loggers from vector
    for (size_t i = 0; i < logger_vec.size(); i++) {
      SC_NOTE(logger_vec[i]) << "NOTE: Vector logger " << i;
      SC_ALERT(logger_vec[i])("ALERT: Vector logger {} with format", i);
    }
  }
};

// Test 6: SC_LOG_AT with different argument combinations
SC_MODULE(test_module_log_at) {
  SC_LOG_HANDLE(my_logger, "LogAtTag");
  
  SC_CTOR(test_module_log_at) {
    cout << "\n=== Test SC_LOG_AT Variants ===" << endl;
    
    // SC_LOG_AT with no additional args (uses default logger)
    SC_LOG_AT(sc_core::SC_HIGH) << "SC_LOG_AT: No args, stream syntax";
    SC_LOG_AT(sc_core::SC_MEDIUM)("SC_LOG_AT: No args, format syntax: {}", 1);
    
    // SC_LOG_AT with logger handle
    SC_LOG_AT(sc_core::SC_HIGH, my_logger) << "SC_LOG_AT: With logger handle";
    SC_LOG_AT(sc_core::SC_FULL, my_logger)("SC_LOG_AT: With logger handle, format: {}", 2);
    
    // SC_LOG_AT with explicit tag string
    SC_LOG_AT(sc_core::SC_HIGH, "ExplicitTag") << "SC_LOG_AT: With explicit tag";
    SC_LOG_AT(sc_core::SC_DEBUG, "ExplicitTag")("SC_LOG_AT: With explicit tag, format: {}", 3);
    
    // SC_LOG_AT with logger and tag
    SC_LOG_AT(sc_core::SC_MEDIUM, my_logger, "OverrideTag") << "SC_LOG_AT: Logger + override tag";
    SC_LOG_AT(sc_core::SC_HIGH, my_logger, "OverrideTag")("SC_LOG_AT: Logger + override tag, format: {}", 4);
  }
};

// Test 7: Using name() as tag
SC_MODULE(test_module_name_tag) {
  SC_LOG_HANDLE();
  
  SC_CTOR(test_module_name_tag) {
    cout << "\n=== Test Using name() as Tag ===" << endl;
    
    SC_LOG_AT(sc_core::SC_HIGH, name()) << "Using name() as tag";
    SC_LOG_AT(sc_core::SC_MEDIUM, name())("Using name() as tag with format: {}", name());
  }
};

// Test 8: Global logger usage
void test_global_loggers() {
  cout << "\n=== Test Global Static Loggers ===" << endl;
  
  SC_NOTE() << "NOTE: Global default logger (no tag)";
  SC_ALERT()("ALERT: Global default logger, format: {}", "test");
  
  SC_NOTE(GLOBAL_LOGGER_WITH_TAG) << "NOTE: Global logger with tag";
  SC_DETAIL(GLOBAL_LOGGER_WITH_TAG)("DETAIL: Global logger with tag, format: {}", 789);
}

// Test 9: Tag-based logging (using string literals as tags)
void test_tag_based_logging() {
  cout << "\n=== Test Tag-Based Logging ===" << endl;
  
  SC_NOTE("Tag1") << "NOTE: Using Tag1";
  SC_ALERT("Tag2")("ALERT: Using Tag2 with format: {}", "value");
  SC_DETAIL("Tag3") << "DETAIL: Using Tag3";
  SC_INTERNAL("Tag4")("INTERNAL: Using Tag4 with format: {}", 999);
}

// Test 10: All log levels
void test_all_log_levels() {
  cout << "\n=== Test All Log Levels ===" << endl;
  
  SC_CRITICAL("LevelTest") << "CRITICAL level test";
  SC_ALERT("LevelTest") << "ALERT level test";
  SC_NOTE("LevelTest") << "NOTE level test";
  SC_DETAIL("LevelTest") << "DETAIL level test";
  SC_INTERNAL("LevelTest") << "INTERNAL level test";
  
  SC_CRITICAL("LevelTest")("CRITICAL level test with format: {}", 1);
  SC_ALERT("LevelTest")("ALERT level test with format: {}", 2);
  SC_NOTE("LevelTest")("NOTE level test with format: {}", 3);
  SC_DETAIL("LevelTest")("DETAIL level test with format: {}", 4);
  SC_INTERNAL("LevelTest")("INTERNAL level test with format: {}", 5);
}

// Test 11: Complex format strings
void test_complex_formats() {
  cout << "\n=== Test Complex Format Strings ===" << endl;
  
  SC_NOTE("FormatTest")("Multiple args: {}, {}, {}", 1, 2, 3);
  SC_NOTE("FormatTest")("Mixed types: int={}, str={}, float={:.2f}", 42, "hello", 3.14159);
  SC_NOTE("FormatTest")("Nested braces: {{}} and value: {}", 123);
  SC_NOTE("FormatTest")("Empty format string");
}

// Simple verbosity control for testing
class test_verbosity_controller {
  std::map<std::string, sc_core::sc_verbosity> tag_levels;
  
public:
  test_verbosity_controller() {
    // Set different levels for different tags
    tag_levels["GlobalTag"] = sc_core::SC_HIGH;
    tag_levels["ModuleTag"] = sc_core::SC_FULL;
    tag_levels["Logger1Tag"] = sc_core::SC_MEDIUM;
    tag_levels["Logger2Tag"] = sc_core::SC_DEBUG;
    tag_levels["VecLogger0"] = sc_core::SC_HIGH;
    tag_levels["VecLogger1"] = sc_core::SC_MEDIUM;
    tag_levels["VecLogger2"] = sc_core::SC_FULL;
    tag_levels["LogAtTag"] = sc_core::SC_DEBUG;
    tag_levels["ExplicitTag"] = sc_core::SC_FULL;
    tag_levels["OverrideTag"] = sc_core::SC_HIGH;
    tag_levels["Tag1"] = sc_core::SC_HIGH;
    tag_levels["Tag2"] = sc_core::SC_MEDIUM;
    tag_levels["Tag3"] = sc_core::SC_FULL;
    tag_levels["Tag4"] = sc_core::SC_DEBUG;
    tag_levels["LevelTest"] = sc_core::SC_DEBUG;
    tag_levels["FormatTest"] = sc_core::SC_HIGH;
    
    std::function<sc_core::sc_verbosity(sc_core::sc_log_logger_cache &,
                                        const char *, int, std::string_view)>
        fn = [&](sc_core::sc_log_logger_cache &logger, const char *file,
                 int line,
                 std::string_view local_tag) -> sc_core::sc_verbosity {
      // Check if we have a specific level for this tag
      std::string tag_str;
      if (!logger.tag.empty()) {
        tag_str = std::string(logger.tag);
      } else if (!local_tag.empty()) {
        tag_str = std::string(local_tag);
      } else if (!logger.scname.empty()) {
        tag_str = std::string(logger.scname);
      }
      
      auto it = tag_levels.find(tag_str);
      if (it != tag_levels.end()) {
        if (logger.level == sc_core::SC_UNSET) {
          logger.level = it->second;
        }
        return it->second;
      }
      
      // Default to TRACE for everything else
      if (logger.level == sc_core::SC_UNSET) {
        logger.level = sc_core::SC_DEBUG;
      }
      return sc_core::SC_DEBUG;
    };
    
    ::sc_core::sc_log_impl::sc_set_log_verbosity_fn(fn);
    ::sc_core::sc_report_handler::set_verbosity_level(sc_core::SC_DEBUG);
  }
};

void report_handler(const sc_core::sc_report &rep,
                    const sc_core::sc_actions &actions) {
  cout << "[" << level_name(sc_core::as_log(rep.get_verbosity())) << "] "
       << rep.get_msg_type() << ": " << rep.get_msg() << endl;
}

int sc_main(int, char *[]) {
  ::sc_core::sc_report_handler::set_verbosity_level(sc_core::SC_DEBUG);
  ::sc_core::sc_report_handler::set_handler(report_handler);
  
  test_verbosity_controller verbosity_ctrl;
  
  cout << "========================================" << endl;
  cout << "SC_LOG API Comprehensive Test (test02)" << endl;
  cout << "========================================" << endl;
  
  // Test global loggers
  test_global_loggers();
  
  // Test tag-based logging
  test_tag_based_logging();
  
  // Test all log levels
  test_all_log_levels();
  
  // Test complex format strings
  test_complex_formats();
  
  // Instantiate test modules
  test_module_default mod_default("mod_default");
  test_module_tagged mod_tagged("mod_tagged");
  test_module_multi_logger mod_multi("mod_multi");
  test_module_vector mod_vector("mod_vector");
  test_module_log_at mod_log_at("mod_log_at");
  test_module_name_tag mod_name_tag("mod_name_tag");
  
  cout << "\n========================================" << endl;
  cout << "All tests completed successfully!" << endl;
  cout << "========================================" << endl;
  
  return 0;
}
