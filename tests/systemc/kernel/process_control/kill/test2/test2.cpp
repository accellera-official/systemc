//----------------------------------------------------------------------
//   Copyright 2009 Cadence Design Systems, Inc.
//   All Rights Reserved Worldwide
//   Copyright 2009 Forte Design Systems, Inc.
// 
// test2: make sure kill on a process before the start of simulation
//        causes it to never execute.
//----------------------------------------------------------------------

#include <systemc.h>
    
SC_MODULE(top) {
public:
  SC_CTOR(top) {
    SC_THREAD(victim);
    sensitive << clk.pos();
    h = sc_get_current_process_handle();
    SC_THREAD(perpetrator);
  }

  void victim() {
    cerr << sc_time_stamp() << ": starting victim thread" << endl;
    wait();
    cerr << sc_time_stamp() << ": ending victim thread" << endl;
  }

  void perpetrator() {
    cerr << sc_time_stamp() << ": starting bullet dodger " << endl;
    ::sc_core::wait(20, SC_NS);
    cerr << sc_time_stamp() << ": ending bullet dodger" << endl;
    sc_stop();
  }

  sc_in<bool> clk;
  sc_process_handle h;
};

int sc_main (int argc, char *argv[])
{
  sc_clock clk("clk", 10, SC_NS, 0.5);
  top t("top");
  t.clk(clk);
  t.h.kill();
  sc_start();
  return 0;
}

