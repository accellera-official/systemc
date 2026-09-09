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
        sc_dt::sc_lv<32> varth("10100101101001011010010110100101");
        std::size_t total = 0;
        for (int i = 0; i < NUMRUNS; ++i)
            total += varth.to_string(sc_dt::SC_HEX).size();
        combined_total += total;
    }

    {
        sc_dt::sc_lv<32> varm("01011010010110100101101001011010");
        std::size_t total = 0;
        for (int i = 0; i < NUMRUNS; ++i)
            total += varm.to_string(sc_dt::SC_DEC).size();
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
    std::cout << "test sc_lv_to_string_numrep started" << std::endl;
    std::thread my_thread[NUMTHREADS];

    for (int i = 0; i < NUMTHREADS; ++i)
        my_thread[i] = std::thread(thread_body);

    std::cout << run_workload() << std::endl;
    for (int i = 0; i < NUMTHREADS; ++i)
        my_thread[i].join();
    std::cout << "test sc_lv_to_string_numrep finished" << std::endl;
    return 0;
}
