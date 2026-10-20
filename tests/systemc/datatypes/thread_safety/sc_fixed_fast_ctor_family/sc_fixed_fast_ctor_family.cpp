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
            sc_dt::sc_fixed_fast<16, 8> varth;
            sc_dt::sc_fxcast_switch cast_switch;
            sc_dt::sc_fixed_fast<16, 8> tmp1(cast_switch);
            sc_dt::sc_fixed_fast<16, 8> tmp2(1);
            total += varth.to_uint64() + 3 * tmp1.to_uint64() + 5 * tmp2.to_uint64();
        }
        combined_total += total;
    }

    {
        std::size_t total = 0;
        for (int i = 0; i < NUMRUNS; ++i) {
            sc_dt::sc_int<8> base = 7;
            sc_dt::sc_fixed_fast<16, 8> varm(base);
            sc_dt::sc_fxcast_switch cast_switch;
            sc_dt::sc_fixed_fast<16, 8> tmp1(2, cast_switch);
            total += varm.to_uint64() + 3 * tmp1.to_uint64();
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
    std::cout << "test sc_fixed_fast_ctor_family started" << std::endl;
    std::thread my_thread[NUMTHREADS];

    for (int i = 0; i < NUMTHREADS; ++i)
        my_thread[i] = std::thread(thread_body);

    std::cout << run_workload() << std::endl;
    for (int i = 0; i < NUMTHREADS; ++i)
        my_thread[i].join();
    std::cout << "test sc_fixed_fast_ctor_family finished" << std::endl;
    return 0;
}
