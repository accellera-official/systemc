#define SC_INCLUDE_FX
#include <systemc>
#include <sstream>
#include <string>
#include <thread>

constexpr int NUMRUNS = 10000000;
constexpr int NUMTHREADS = 1;

std::size_t run_workload()
{
    std::size_t combined_total = 0;

    {
        std::size_t total = 0;
        for (int i = 0; i < NUMRUNS; ++i) {
            sc_dt::sc_fxcast_switch varth;
            total += varth.to_string().size();
        }
        combined_total += total;
    }

    {
        std::size_t total = 0;
        for (int i = 0; i < NUMRUNS; ++i) {
            sc_dt::sc_fxcast_switch varm;
            total += varm.to_string().size();
        }
        combined_total += total;
    }

    return combined_total;
}

void thread_body()
{
    std::cout << run_workload() << std::endl;
}

int sc_main(int argn, char *argc[])
{
    std::cout << "test sc_fxcast_switch_default_ctor started" << std::endl;
    std::thread my_thread[NUMTHREADS];

    for (int i = 0; i < NUMTHREADS; ++i)
        my_thread[i] = std::thread(thread_body);

    std::cout << run_workload() << std::endl;
    for (int i = 0; i < NUMTHREADS; ++i)
        my_thread[i].join();
    std::cout << "test sc_fxcast_switch_default_ctor finished" << std::endl;
    return 0;
}
