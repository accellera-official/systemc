// POC ISSUE:
//
// This example is supposed to execute until the TB returns from its tb_thread, but instead
// it executes forever, as the POC is treating the edges of the clock as pending activity. 
// 
// The work around until this is fixed is to put a loop around what was the while pending
// activity code.
//
// @@@@ ISSUES @@@@

#include <systemc>
using namespace sc_core;

SC_MODULE(TB)
{
    SC_CTOR(TB)
    {
        SC_CTHREAD(tb_thread,m_clk.pos());
    }
    void tb_thread()
    {
        for (size_t i = 0; true; ++i )
        {
	    std::cout << " tb   " << i << "   " << sc_time_stamp() << std::endl;
            wait();
        }
    }
    sc_in<bool> m_clk;
};

int sc_main( int argc, char* argv[] ) {

    sc_clock           clock;
    sc_signal<bool>    reset;
    TB                 tb("tb");

    tb.m_clk(clock);

  sc_start( SC_ZERO_TIME );                    // Run the initialization phase to create pending activity

  for ( size_t i = 0; i < 56; ++i ) {
      if ( sc_pending_activity() ) {
          std::cout << " main " << i << "   "  << sc_time_stamp() << "   " 
	            << sc_time_to_pending_activity() << std::endl;
          sc_start( sc_time_to_pending_activity() ); // Run up to the next activity
      }
  }

  return 0;
}

