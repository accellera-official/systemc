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
        sc_dt::sc_int<32> varth = 324508639;
        std::size_t total = 0;
        for (int i = 0; i < NUMRUNS; ++i)
            total += static_cast<unsigned>(varth.bit(i & 7));
        combined_total += total;
    }

    {
        sc_dt::sc_int<32> varm = 38177486;
        std::size_t total = 0;
        for (int i = 0; i < NUMRUNS; ++i)
            total += static_cast<unsigned>(varm.bit((i + 2) & 7));
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
    std::cout << "test sc_int_bit started" << std::endl;
    std::thread my_thread[NUMTHREADS];

    for (int i = 0; i < NUMTHREADS; ++i)
        my_thread[i] = std::thread(thread_body);

    std::cout << run_workload() << std::endl;
    for (int i = 0; i < NUMTHREADS; ++i)
        my_thread[i].join();
    std::cout << "test sc_int_bit finished" << std::endl;
    return 0;
}
