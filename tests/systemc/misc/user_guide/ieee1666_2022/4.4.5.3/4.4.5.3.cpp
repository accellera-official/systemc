// COMPLETE TEST:
//
// If the execution log matches the golden log this test compiled and executed
// correctly.

#include <systemc>
SC_MODULE(Top)
{
    Top(const char* name) : sc_module( sc_core::sc_module_name(name) ) {}
};
int sc_main( int argc, char* argv[] ) {
  using namespace sc_core;

  Top top("top");         // Instantiate module hierarchy

  sc_start(100.0, SC_NS); // Run for exactly 100 ns
  sc_start();             // Run until no more activity

  if (sc_get_status() == SC_PAUSED) {
    SC_REPORT_INFO("", "sc_stop called to terminate a paused simulation");
    sc_stop();
  }

  return 0;
}
