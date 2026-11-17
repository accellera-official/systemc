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

// Terminate threads and kill methods at deterministic pseudo-random times.
// Count every spawned process deletion through simulation-context teardown.

#define SC_INCLUDE_DYNAMIC_PROCESSES
#include <systemc>
#include <iostream>
#include <utility>
#include <vector>

using namespace sc_core;

static const int processes_per_kind = 1000;
static const int last_kill_ns = 200;

static std::vector<int> deletions(2 * processes_per_kind);
static bool count_deletions = false;  // spawning destroys temporary copies of the bodies

static unsigned next_random()
{
    static unsigned state = 12345;
    state = state * 1103515245u + 12345u;
    return (state >> 16) & 0x7fff;
}

struct Body
{
    int id;
    bool is_thread;
    bool victim;
    int delay_ns;

    void operator()() const
    {
        if (is_thread)
            wait(victim ? sc_time(delay_ns, SC_NS) : sc_time(1, SC_SEC));
    }

    ~Body()
    {
        if (count_deletions)
            ++deletions[id];
    }
};

SC_MODULE(StaticProcesses)
{
    SC_CTOR(StaticProcesses)
    {
        SC_METHOD(method);
        SC_THREAD(thread);
    }

    void method() {}
    void thread() { wait(1, SC_SEC); }
};

SC_MODULE(Controller)
{
    std::vector<std::pair<int, sc_process_handle>> kills;  // time in ns, victim method

    SC_CTOR(Controller) { SC_THREAD(run); }

    void run()
    {
        for (int now = 1; now <= last_kill_ns + 10; ++now) {
            wait(1, SC_NS);
            for (auto& kill : kills) {
                if (kill.first == now) {
                    kill.second.kill();
                    kill.second = sc_process_handle();
                }
            }
        }
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

static int check(const std::vector<int>& expected, const char* when)
{
    int wrong = 0;
    for (size_t id = 0; id < deletions.size(); ++id) {
        if (deletions[id] != expected[id]) {
            if (wrong++ < 10)
                std::cout << when << ": process " << id << " deleted " << deletions[id]
                          << " times, expected " << expected[id] << std::endl;
        }
    }
    return wrong;
}

int sc_main(int, char*[])
{
    sc_simcontext* context = new_context();

    Controller* controller = new Controller("controller");
    std::vector<int> victims(deletions.size());
    int victim_threads = 0, victim_methods = 0;
    for (int i = 0; i < processes_per_kind; ++i) {
        if (i % 100 == 0)
            new StaticProcesses(sc_gen_unique_name("static"));

        const int thread_id = 2 * i;
        victims[thread_id] = next_random() % 2;
        victim_threads += victims[thread_id];
        sc_spawn(Body{thread_id, true, victims[thread_id] != 0, 1 + int(next_random() % last_kill_ns)},
                 sc_gen_unique_name("thread"));

        const int method_id = 2 * i + 1;
        victims[method_id] = next_random() % 2;
        victim_methods += victims[method_id];
        sc_spawn_options options;
        options.spawn_method();
        sc_process_handle method = sc_spawn(Body{method_id, false, false, 0}, sc_gen_unique_name("method"), &options);
        const int kill_ns = 1 + int(next_random() % last_kill_ns);
        if (victims[method_id])
            controller->kills.emplace_back(kill_ns, method);
    }
    count_deletions = true;

    sc_start(last_kill_ns + 20, SC_NS);
    std::cout << victim_threads << " of " << processes_per_kind << " threads returned, "
              << victim_methods << " of " << processes_per_kind << " methods killed" << std::endl;
    const int wrong_after_simulation = check(victims, "after simulation");
    std::cout << "after simulation: "
              << (wrong_after_simulation ? "wrong deletions" : "victims deleted once, others alive") << std::endl;

    // Finish started threads before context teardown deletes their coroutine package.
    sc_start(1, SC_SEC);
    controller->kills.clear();  // release any handle before the context goes away
    delete_context(context);
    const int wrong_after_teardown = check(std::vector<int>(deletions.size(), 1), "after teardown");
    std::cout << "after teardown: "
              << (wrong_after_teardown ? "wrong deletions" : "every process deleted once") << std::endl;

    // Teardown must also delete threads and methods that never started.
    count_deletions = false;
    deletions.assign(deletions.size(), 0);
    context = new_context();
    for (int i = 0; i < processes_per_kind; ++i) {
        sc_spawn_options thread_options;
        thread_options.dont_initialize();
        sc_spawn(Body{2 * i, true, false, 0}, sc_gen_unique_name("thread"), &thread_options);

        sc_spawn_options method_options;
        method_options.spawn_method();
        method_options.dont_initialize();
        sc_spawn(Body{2 * i + 1, false, false, 0}, sc_gen_unique_name("method"), &method_options);
    }
    count_deletions = true;
    delete_context(context);
    const int wrong_before_simulation = check(std::vector<int>(deletions.size(), 1), "before simulation teardown");
    std::cout << "before simulation teardown: "
              << (wrong_before_simulation ? "wrong deletions" : "every process deleted once") << std::endl;
    return wrong_after_simulation || wrong_after_teardown || wrong_before_simulation;
}
