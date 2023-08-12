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

