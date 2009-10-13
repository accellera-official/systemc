//----------------------------------------------------------------------
//   Copyright 2009 Forte Design Systems
//----------------------------------------------------------------------

#include <systemc.h>
    
SC_MODULE(top) {
public:
  SC_CTOR(top) {
    SC_THREAD(victim);
    h = sc_get_current_process_handle();
    SC_THREAD(perpetrator);
  }

  void victim() {
    cerr << sc_time_stamp() << ": starting victim thread" << endl;
    wait(10, SC_NS);
    cerr << sc_time_stamp() << "(" << sc_delta_count() << ")" 
         << ": victim thread resuming" << endl;
  }

  void perpetrator() {
    wait(2, SC_NS);
    cerr << sc_time_stamp() << ": in perpetrator suspending victim " << endl;
    h.suspend();
    wait(10, SC_NS);
    cerr << sc_time_stamp() << "(" << sc_delta_count() << ")" 
         << ": in perpetrator resuming victim " << endl;
    h.resume();
    wait(10, SC_NS);
    sc_stop();
  }

protected:
  sc_process_handle h;
  sc_process_handle h2;
};

int sc_main (int argc, char *argv[])
{
  top t("top");
  sc_start();
  return 0;
}

