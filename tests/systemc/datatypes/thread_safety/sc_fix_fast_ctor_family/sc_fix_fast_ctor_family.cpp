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
            sc_dt::sc_fix_fast varth;
            sc_dt::sc_fix_fast tmp1(8, 4);
            sc_dt::sc_fix_fast tmp2(sc_dt::SC_TRN, sc_dt::SC_WRAP);
            sc_dt::sc_fix_fast tmp3(16, 8, sc_dt::SC_TRN_ZERO, sc_dt::SC_SAT, 1);
            sc_dt::sc_fxcast_switch cast_switch;
            sc_dt::sc_fix_fast tmp4(cast_switch);
            sc_dt::sc_fix_fast tmp5(sc_dt::sc_fxtype_params(16, 8));
            total += varth.to_uint64() + tmp1.to_uint64() + tmp2.to_uint64() + tmp3.to_uint64() + tmp4.to_uint64() + tmp5.to_uint64();
        }
        combined_total += total;
    }

    {
        std::size_t total = 0;
        for (int i = 0; i < NUMRUNS; ++i) {
            sc_dt::sc_fix_fast varm(2);
            sc_dt::sc_fix_fast tmp1("3");
            sc_dt::sc_int<8> base = 7;
            sc_dt::sc_fix_fast tmp2(base);
            sc_dt::sc_uint<8> ubase = 9;
            sc_dt::sc_fix_fast tmp3(ubase);
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
    std::cout << "test sc_fix_fast_ctor_family started" << std::endl;
    
    std::thread my_thread[NUMTHREADS];

    for (int i = 0; i < NUMTHREADS; ++i)
        my_thread[i] = std::thread(thread_body);

    std::cout << run_workload() << std::endl;
    for (int i = 0; i < NUMTHREADS; ++i)
        my_thread[i].join();
    std::cout << "test sc_fix_fast_ctor_family finished" << std::endl;
    return 0;
}
