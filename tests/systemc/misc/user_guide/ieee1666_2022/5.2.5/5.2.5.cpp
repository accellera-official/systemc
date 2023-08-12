#include "systemc.h"

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

    std::cout << "success" << std::endl;
}
