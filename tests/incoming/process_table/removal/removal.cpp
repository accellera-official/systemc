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

// Remove processes from both ends and the middle of method and thread lists.
// Each spawned process owns a body whose destructor logs process deletion.

#define SC_INCLUDE_DYNAMIC_PROCESSES
#include <systemc>
#include <iostream>
#include <string>
#include <vector>

using namespace sc_core;

static bool log_deletions = false;  // spawning destroys temporary copies of the bodies

struct Body
{
    std::string name;
    bool is_thread;
    bool victim;

    void operator()() const
    {
        // Victim threads terminate at once; the others wait for a second step.
        if (is_thread && !victim)
            wait(1, SC_SEC);
    }

    ~Body()
    {
        if (log_deletions)
            std::cout << "deleted " << name << std::endl;
    }
};

// The default context is never deleted, so the test uses its own.
static sc_simcontext* new_context()
{
    sc_simcontext* context = new sc_simcontext;
    sc_curr_simcontext = context;
    sc_default_global_context = context;
    return context;
}

static void delete_context(sc_simcontext* context)
{
    delete context;
    sc_curr_simcontext = nullptr;
    sc_default_global_context = nullptr;
}

int sc_main(int, char*[])
{
    sc_simcontext* context = new_context();

    // Lists after spawning, newest first: t7 t6 ... t0 and m7 m6 ... m0.
    const int count = 8;
    const bool victim[count] = {true, true, false, true, true, false, true, true};
    std::vector<sc_process_handle> threads(count), methods(count);
    for (int i = 0; i < count; ++i) {
        const std::string id = std::to_string(i);
        sc_process_handle thread = sc_spawn(Body{"t" + id, true, victim[i]}, ("t" + id).c_str());
        sc_spawn_options options;
        options.spawn_method();
        sc_process_handle method = sc_spawn(Body{"m" + id, false, victim[i]}, ("m" + id).c_str(), &options);
        if (victim[i]) {
            threads[i] = thread;
            methods[i] = method;
        }
    }
    log_deletions = true;

    // Victim threads terminate and victim methods are killed. The handles keep
    // them alive, so releasing a handle deletes its process.
    sc_start(1, SC_NS);
    for (int i = 0; i < count; ++i)
        if (victim[i])
            methods[i].kill();

    // tail, head, the new head, middle, the successor of a removed process,
    // and the process that became the tail
    for (int i : {0, 7, 6, 4, 3, 1}) {
        std::cout << "release " << i << std::endl;
        threads[i] = sc_process_handle();
        methods[i] = sc_process_handle();
    }

    // Finish started threads before context teardown deletes their coroutine package.
    std::cout << "finish surviving threads" << std::endl;
    sc_start(1, SC_SEC);

    // The surviving methods m5 m2 are deleted exactly once, newest first.
    std::cout << "delete simulation context" << std::endl;
    delete_context(context);
    std::cout << "done" << std::endl;
    return 0;
}
