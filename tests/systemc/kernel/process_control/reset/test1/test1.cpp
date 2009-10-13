//----------------------------------------------------------------------
//   Copyright 2009 Cadence Design Systems, Inc.
//   All Rights Reserved Worldwide
//----------------------------------------------------------------------

#include <systemc.h>
    
class A {
public:
  A() {}
  ~A() { cerr << "In A's dtor" << endl; }
};

SC_MODULE(top) {
public:
  SC_CTOR(top) {
    SC_THREAD(victim);
    h = sc_get_current_process_handle();
    SC_THREAD(perpetrator);
  }

  void victim() {
    A a;
    cerr << sc_time_stamp() << ": starting victim thread" << endl;
    ::sc_core::wait(100, SC_NS);
    cerr << sc_time_stamp() << ": ending victim thread" << endl;
  }

  void perpetrator() {
    wait(10, SC_NS);
    cerr << sc_time_stamp() << ": in perpetrator resetting victim " << endl;
    h.reset();
    cerr << sc_time_stamp() << ": in perpetrator after resetting victim " << endl;
  }

protected:
  sc_process_handle h;
};

int sc_main (int argc, char *argv[])
{
  top t("top");
  sc_start();
  return 0;
}

