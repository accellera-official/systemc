#define SC_INCLUDE_DYNAMIC_PROCESSES

#include <systemc>

using namespace sc_core;
using std::cout;
using std::endl;

struct M1: sc_module
{
  M1(sc_module_name _name)
  {
    SC_THREAD(ticker);
    SC_THREAD(calling);
    SC_THREAD(target);
      t = sc_get_current_process_handle();
  }
  
  sc_process_handle t;
  sc_event ev;

  void ticker()
  {
    for (;;)
    {
      wait(10, SC_NS);
      ev.notify();
    }
  }
   
  void calling()
  {
    wait(15, SC_NS);
    // Target runs at time 10 NS due to notification
    
    t.suspend();
    wait(10, SC_NS);
    // Target does not run at time 20 NS while suspended
    
    t.resume();
    // Target runs at time 25 NS when resume is called
    
    wait(10, SC_NS);
    // Target runs at time 30 NS due to notification
    
    t.disable();
    wait(10, SC_NS);
    // Target does not run at time 40 NS while disabled
    
    t.enable();
    // Target does not run at time 45 NS when enable is called
    
    wait(10, SC_NS);
    // Target runs at time 50 NS due to notification
    
    sc_stop();
  }

  void target()
  {
    for (;;)
    {
      wait(ev);
      cout << "Target awoke at " << sc_time_stamp() << endl;
    }
  }
  
  SC_HAS_PROCESS(M1);
};

int sc_main(int argc, char* argv[])
{
  M1 m("m");
  
  sc_start();
  
  cout << endl << "Success" << endl;
  return 0;
}
  
