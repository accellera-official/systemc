//----------------------------------------------------------------------
//   Copyright 2009 Cadence Design Systems, Inc.
//   All Rights Reserved Worldwide
//   Copyright 2009 Forte Design Systems, Inc.
// 
// test2: make sure disabled process with a static sensitivity list
//        wakes up on its event when enabled.
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
    cerr << sc_time_stamp() << ": victim thread executing A" << endl;
    wait();
    cerr << sc_time_stamp() << ": victim thread executing B" << endl;
    wait();
    cerr << sc_time_stamp() << ": victim thread executing C" << endl;
    wait();
    cerr << sc_time_stamp() << ": ending victim thread" << endl;
  }

  void perpetrator() {
    wait(10, SC_NS);
    cerr << sc_time_stamp() << ": in perpetrator disabling victim " << endl;
    h.disable();
    ::sc_core::wait(15, SC_NS);
    cerr << sc_time_stamp() << ": in perpetrator enabling victim " << endl;
    h.enable();
    ::sc_core::wait(15, SC_NS);
    cerr << sc_time_stamp() << ": in perpetrator disabling victim " << endl;
    h.disable();
    ::sc_core::wait(15, SC_NS);
    cerr << sc_time_stamp() << ": in perpetrator enabling victim " << endl;
    h.enable();
    ::sc_core::wait(20, SC_NS);
    cerr << sc_time_stamp() << ": ending perpetrator thread" << endl;
    sc_stop();
  }

  sc_in<bool> clk;
protected:
  sc_process_handle h;
};

int sc_main (int argc, char *argv[])
{
  sc_clock clk("clk", 10, SC_NS, 0.5);
  top t("top");
  t.clk(clk);
  sc_start();
  return 0;
}

