// COMPLETE TEST
//
// The example does not do much, so neither does the test.

#include "systemc"

SC_MODULE(M)
{
    M(sc_core::sc_module_name) {}
    // .  .  .
};

class N : public sc_core::sc_module {
  public:
    N(sc_core::sc_module_name) {}
    // .  .  .
};

int sc_main( int argc, char* argv[] )
{
    M m("m");
    N n("n");

    sc_core::sc_start(sc_core::SC_ZERO_TIME);
    std::cout << "program complete" << std::endl;
    return 0;
}
