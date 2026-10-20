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
            sc_dt::sc_fxval varth;
            sc_dt::sc_fxval tmp1(1);
            sc_dt::sc_fxval tmp2("3");
            sc_dt::sc_int<8> base = 7;
            sc_dt::sc_fxval tmp3(base);
            total += varth.to_uint64() + 3 * tmp1.to_uint64() + 5 * tmp2.to_uint64() + 7 * tmp3.to_uint64();
        }
        combined_total += total;
    }

    {
        std::size_t total = 0;
        for (int i = 0; i < NUMRUNS; ++i) {
            sc_dt::sc_fxval varm(2);
            sc_dt::sc_fxval tmp1(4);
            sc_dt::sc_fxval tmp2("5");
            sc_dt::sc_uint<8> base = 9;
            sc_dt::sc_fxval tmp3(base);
            total += varm.to_uint64() + 3 * tmp1.to_uint64() + 5 * tmp2.to_uint64() + 7 * tmp3.to_uint64();
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
    std::cout << "test sc_fxval_default_observer_ctors started" << std::endl;
    std::cout << "Starting Test sc_fxval_default_observer_ctors" << std::endl;
    std::thread my_thread[NUMTHREADS];

    for (int i = 0; i < NUMTHREADS; ++i)
        my_thread[i] = std::thread(thread_body);

    std::cout << run_workload() << std::endl;
    for (int i = 0; i < NUMTHREADS; ++i)
        my_thread[i].join();
    std::cout << "Finished Test sc_fxval_default_observer_ctors" << std::endl;
    std::cout << "test sc_fxval_default_observer_ctors finished" << std::endl;
    return 0;
}
